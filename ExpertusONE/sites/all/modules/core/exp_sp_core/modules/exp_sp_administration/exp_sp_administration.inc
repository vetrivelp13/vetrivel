<?php
/**
 * Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */


/*
 * getURLParam() - Extract and return a parameter from the current URL
 */
function getURLParam($filtersetCode){
	try {
		// when using api simulator, the parameters are being sent by POST method
  return isset($_GET[$filtersetCode]) ? stripRegStatus($_GET[$filtersetCode]) : (isset($_POST[$filtersetCode]) ? stripRegStatus($_POST[$filtersetCode]):'');
  } catch (Exception $ex) {
  	watchdog_exception('getURLParam', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getRawURLParam() - Extract and return a parameter from the current URL
 *                  - Does not call stripRegStatus()
 */
function getRawURLParam($filtersetCode){
  try {
  	// when using api simulator, the parameters are being sent by POST method
  //44552: search not working in survey and assessment page.
  return isset($_GET[$filtersetCode]) ? rawurldecode($_GET[$filtersetCode]) : (isset($_POST[$filtersetCode]) ? rawurldecode($_POST[$filtersetCode]) : '');
  } catch (Exception $ex) {
    watchdog_exception('getURLParam', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * selectAddNarrowSearchINCondition() - To the $selectStmt add an IN condition clause for the selected filters in $filtersetCode.
 *                                $tableAlias
 */
 function selectAddNarrowSearchINCondition($selectStmt, $tableAlias, $tableField, $filtersetCode,$defaultStatus = NULL) {
 	try {
 $selectedFilters = getURLParam($filtersetCode);
 if (!empty($selectedFilters) || $selectedFilters != ''){
    $narrowSearchSet = explode("|", $selectedFilters);
    expDebug::dPrint(' $$narrowSearchSet = ' . print_r($selectedFilters, true) , 4);
    $selectStmt->condition($tableAlias . '.' . $tableField, $narrowSearchSet, 'IN');
  }
  elseif(!empty($defaultStatus)){
    $narrowSearchSet = explode("|", $defaultStatus);
    $selectStmt->condition($tableAlias . '.' . $tableField, $narrowSearchSet, 'IN');
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchINCondition', $ex);
  	expertusErrorThrow($ex);
  }
}
/*Viswanathan added #0073397*/
function selectAddNarrowSearchByHiredateCondition($selectStmt, $tableAlias, $tableField, $filtersetCode,$defaultStatus = NULL) {
	try {
		$selectedFilters = getURLParam($filtersetCode);
		if (!empty($selectedFilters) || $selectedFilters != '|'){
			$narrowSearchSet = explode("|", $selectedFilters);
			$hireDateStart = date_format(date_create_from_format('m-d-Y',$narrowSearchSet[0]), 'Y-m-d');		
			$hireDateEnd = date_format(date_create_from_format('m-d-Y', $narrowSearchSet[1]), 'Y-m-d');
			if(!empty($hireDateStart) && !empty($hireDateEnd)){
				$selectStmt->condition($tableAlias . '.' . $tableField, $hireDateStart, '>=');
			    $selectStmt->condition($tableAlias . '.' . $tableField, $hireDateEnd, '<=');
			}else if (!empty($hireDateStart) && $hireDateEnd =='' ){
				$selectStmt->condition($tableAlias . '.' . $tableField, $hireDateStart, '>=');
			}else if (!empty($hireDateEnd) && $hireDateStart=='' ){
				$selectStmt->condition($tableAlias . '.' . $tableField, $hireDateEnd, '<=');
			}
		}
	
	} catch (Exception $ex) {
		watchdog_exception('selectAddNarrowSearchHireDateCondition', $ex);
		expertusErrorThrow($ex);
	}
}/*Viswanathan added #0073397*/
/*
 * selectAddNarrowSearchSliderCondition
 */
function selectAddNarrowSearchSliderCondition($selectStmt, $tableAlias, $tableField, $filtersetCode){
	try{
  $valuefilters= getURLParam($filtersetCode);
  expDebug::dPrint(' $valuefilters = ' . print_r($valuefilters, true) , 4);
  if (!empty($valuefilters)){
    $valueRange = explode("|", $valuefilters);
  if (!empty($valueRange[0])) {
      $selectStmt->condition($tableAlias . '.' . $tableField, $valueRange [0], '>=');
    }
    if (!empty($valueRange[1])) {
      $selectStmt->condition($tableAlias . '.' . $tableField, $valueRange [1], '<=');
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchSliderCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByTextConditionExactMatch() - To the $selectStmt add Search by Text condition if set by the user exact match
*/
function selectAddNarrowSearchByTextConditionExactMatch($selectStmt, $tableAlias, $tableField, $filtersetCode = 'textfilter', $defaultText = '') {
	try{
		$textFilterStr = getRawURLParam($filtersetCode);
		expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);
		if (!empty($textFilterStr) && $textFilterStr != $defaultText){
			$selectStmt->condition($tableAlias . '.' . $tableField, rawurldecode($textFilterStr), '=');
		}
	} catch (Exception $ex) {
		watchdog_exception('selectAddNarrowSearchByTextCondition', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * selectAddNarrowSearchByTextCondition() - To the $selectStmt add Search by Text condition if set by the user
 */
function selectAddNarrowSearchByTextCondition($selectStmt, $tableAlias, $tableField, $filtersetCode = 'textfilter', $defaultText = '') {
	try{
  $textFilterStr = getRawURLParam($filtersetCode);
  expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);

  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
    $selectStmt->condition($tableAlias . '.' . $tableField, '%' . db_like($textFilterStr) . '%', 'LIKE');
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByTextCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByTextMultipleORCondition() - To the $selectStmt add Search by Text condition if set by the user
 */
function selectAddNarrowSearchByTextMultipleORCondition($selectStmt, $tableAlias, $tableFieldsList = array(), $filtersetCode = 'textfilter', $defaultText = '',$api='') {
	try{
		if($api == 1){
			$mutipleTextFilterStr = stripslashes(getRawURLParam($filtersetCode));
		}else{
			////53638: Home page widgets - Top search bar - Quote based string search not working
			if($filtersetCode=='textfilter')
				$mutipleTextFilterStr = getRawURLParam($filtersetCode);
			else
			    $mutipleTextFilterStr = getRawURLParam($filtersetCode);
		}
  expDebug::dPrint(' $mutipleTextFilterStr = ' . print_r($mutipleTextFilterStr, true) , 4);
  $query_values=array();

  $orQuery = db_or();
  foreach ($tableFieldsList as $field) {
    $orQuery = $orQuery->condition($tableAlias . '.' . $field, '%' . db_like($mutipleTextFilterStr) . '%', 'LIKE');
  }
  $selectStmt->condition($orQuery);
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByTextMultipleORCondition', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * SearchByFilterForMultipleTableORCondition() - To the $selectStmt add Search by city and state condition if set by the user
*/
function SearchByFilterForMultipleTableORCondition($selectStmt, $filtersetCode = 'textfilter', $defaultText = '',$tableAliasFields) {
	try {
	if (!empty($filtersetCode) && $filtersetCode != $defaultText){
		$orQuery = db_or();
		foreach($tableAliasFields as $table){
		$orQuery = $orQuery->condition($table, '%' . db_like($filtersetCode) . '%', 'LIKE');
		expDebug::dPrintDBAPI('SearchByFilterForMultipleTableORCondition', $selectStmt);
		}
		$selectStmt->condition($orQuery);
	}
	} catch (Exception $ex) {
		watchdog_exception('SearchByFilterForMultipleTableORCondition', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * selectAddNarrowSearchByCityStateCondition() - To the $selectStmt add Search by city and state condition if set by the user
 */
function selectAddNarrowSearchByCityStateCondition($selectStmt, $filtersetCode = 'textfilter', $defaultText = '') {
	try{
		if(arg(2) == 'group' && $filtersetCode == 'All'){
			$textFilterStr = $filtersetCode;
		}else{
  		$textFilterStr = getURLParam($filtersetCode);
		}
  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
    $orQuery = db_or();
    if(arg(2) == 'group' && $textFilterStr == 'All'){
    	$textFilterStr = '';
    }
    $orQuery = $orQuery->condition('loc.city', '%' . db_like($textFilterStr) . '%', 'LIKE');
    $orQuery = $orQuery->condition('st.state_name', '%' . db_like($textFilterStr) . '%', 'LIKE');
    $orQuery = $orQuery->condition('loc.name', '%' . db_like($textFilterStr) . '%', 'LIKE');
    $selectStmt->condition($orQuery);
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByCityStateCondition', $ex);
  	expertusErrorThrow($ex);
  }
}
 
/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
 * selectAddNarrowSearchByLocationCondition() - To the $selectStmt add Search by city and state condition if set by the user 
 */
function selectAddNarrowSearchByLocationCondition($selectStmt, $filtersetCode = 'textfilter', $defaultText = '') {
	try{
		if(arg(2) == 'group' && $filtersetCode == 'All'){
			$textFilterStr = $filtersetCode;
		}else{
		   $textFilterStr = getURLParam($filtersetCode);
		}
			
	  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
	    
			    if(arg(2) == 'group' && $textFilterStr == 'All'){
			    	$textFilterStr = '';
			    }  
				 
				$loc_or_condition_obj=getLocationFilterQuery($textFilterStr,'1'); 
	  			$selectStmt->condition($loc_or_condition_obj); 
		
	  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByLocationCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByCityStateConditionUser() - To the $selectStmt add Search by city and state condition if set by the user
 */
function selectAddNarrowSearchByCityStateConditionUser($selectStmt, $filtersetCode = 'textfilter', $defaultText = '') {
	try {
  $textFilterStr = getURLParam($filtersetCode);
  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
    $orQuery = db_or();
    $orQuery = $orQuery->condition('per.city', '%' . db_like($textFilterStr) . '%', 'LIKE');
    $orQuery = $orQuery->condition('state.state_name', '%' . db_like($textFilterStr) . '%', 'LIKE');
    $selectStmt->condition($orQuery);
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByCityStateConditionUser', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByDateRangeCondition() - To the $selectStmt add Search by a date range if set by the user
 */
function selectAddNarrowSearchByDateRangeCondition($selectStmt, $tableAlias, $tableField, $filtersetCode) {
	try{
  $dateRangeFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $dateRangeFilters = ' . print_r($dateRangeFilters, true) , 4);
  if($dateRangeFilters=='From: mm-dd-yyyy|To: mm-dd-yyyy'){ // check for multi-language search added by Vincent
  	$dateRangeFilters='';
  }
  if (!empty($dateRangeFilters)) {
    $dateRange = explode("|", $dateRangeFilters);
    if (!empty($dateRange[0])) {
    	//38071: Narrow Results > Date Range Search issue(changed 'mdY' to 'm-d-Y')
    	$dateRange[0] = date_format(date_create_from_format('m-d-Y', $dateRange[0]), 'm-d-Y');
      $selectStmt->condition($tableAlias . '.' . $tableField, convertDateToDBDateFormat($dateRange[0]), '>=');
    }

    if (!empty($dateRange[1])) {
    	//38071: Narrow Results > Date Range Search issue (changed 'mdY' to 'm-d-Y')
    	$dateRange[1] = date_format(date_create_from_format('m-d-Y', $dateRange[1]), 'm-d-Y');
      $selectStmt->condition($tableAlias . '.' . $tableField, convertDateToDBDateFormat($dateRange[1], true), '<=');
    }
  } // if
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByDateRangeCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByTextMultiValue() - To the $selectStmt add search value from multi select auto complete.
*/
function selectAddNarrowSearchByTextMultiValue($selectStmt, $tableAliasFields ,$filtersetCode = 'textfilter') {
	try{
		$mutipleTextFilterStr = getRawURLParam($filtersetCode);
		expDebug::dPrint(' $mutipleTextFilterStr = ' . print_r($mutipleTextFilterStr, true) , 4);
		if (!empty($mutipleTextFilterStr) && $mutipleTextFilterStr!='' && isset($mutipleTextFilterStr)){
			$FilterStr = rtrim(strtolower($mutipleTextFilterStr), ", \t\n");//remove last comma from variable
			$multipleTextval = array_map('trim',explode(',',$FilterStr));
			//expDebug::dPrint(' $mutipleTextFilterStr111 = ' . count($multipleTextval) , 4);
			//if(count($multipleTextval) > 0){
				if(in_array('all',$multipleTextval)){
					$selectStmt->isNotNull($tableAliasFields[0]);
				}else{
					$selectStmt->condition($tableAliasFields[1], $multipleTextval , 'IN');
				}
			//}
		}
	} catch (Exception $ex) {
		watchdog_exception('selectAddNarrowSearchByTextMultipleORCondition', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Adds condition for Manage By filter to the search query for the various admin pages: Course, Class, TP, Content, Announcement
 * Returns one of the three values, 'me', 'others', '', so that on page load Manage By checkbox can be checked.
 */

function selectAddNarrowSearchManageByCondition($page, $selectStmt, $tableAlias, $filterName) {
  try {
    expDebug::dPrint('$_GET = ' . print_r($_GET, true), 4);
    $loggedInUserId = getSltpersonUserId();

    // On page load, none of the filters are sent by jqgrid to the server. So we can easily know whether it is page load or a refresh of jqgrid due
    // to narrow filters.
    // For class admin, since course page is already loaded, we check the value of hidden checkbox clspgopened
    $isPageLoad = ($page == 'class')? empty($_GET['clspgopened']) : !isset($_GET[$filterName]);
    $manageByFilterValArr = empty($_GET[$filterName])? array() : explode('|', $_GET[$filterName]);
    expDebug::dPrint('$manageByFilterValArr = ' . print_r($manageByFilterValArr, true), 4);

    $numFilterValues = count($manageByFilterValArr);
    expDebug::dPrint('$numFilterValues = ' . $numFilterValues, 4);

    $userManagedRecCount = fetchManagedByRecordsCount($loggedInUserId, $page);
    expDebug::dPrint('$userManagedRecCount = ' . $userManagedRecCount, 4);

    // Only on page load we decide whether to show records managed by the logged in user or others
    // based on whether or not the user has any managed records
    if ($isPageLoad) { // on page load narrow search
      expDebug::dPrint('is page load', 4);
      if ($userManagedRecCount > 0) {
        $filterBy = 'me';
      }
      else {
        $filterBy = 'both';
      }
    }
    else { // user triggered narrow search
      expDebug::dPrint('is user triggered narrow search', 4);
      if ($numFilterValues == 0 || $numFilterValues == 2) {
        $filterBy = 'both';
      }
      else if ($manageByFilterValArr[0] == 'cre_sys_fop_me') {
        $filterBy = 'me';
      }
      else { //$manageByFilterValArr[0] == 'cre_sys_fop_oth'
        $filterBy = 'others';
      }
    }

    // We add condition to the query only when either of the filter is applied. Otherwise no condition is required.

    if ($filterBy == 'me') {
      $selectStmt->condition(db_or()->condition($tableAlias . '.created_by', $loggedInUserId, '=')
                                    ->condition($tableAlias . '.updated_by', $loggedInUserId, '='));
    }
    else if ($filterBy == 'others') {
      $selectStmt->condition(db_or()->condition($tableAlias . '.created_by', $loggedInUserId, '!=')
                                    ->condition($tableAlias . '.updated_by', $loggedInUserId, '!='));
    }

    if ($isPageLoad) {
      return $filterBy;
    }

    return '';
  }
  catch (Exception $ex) {
    watchdog_exception('selectAddNarrowSearchManageByCondition', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * common function to get the content count which is created and updated by the user
*/
function fetchManagedByRecordsCount($userId,$op){
  try{
    if($op == 'class'){
      $select = db_select('slt_course_class','crs');
      $select->condition('crs.status','lrn_cls_sts_del','!=');
    }else if($op == 'course'){
      $select = db_select('slt_course_template','crs');
      $select->condition('crs.status','lrn_crs_sts_del','!=');
    }else if($op == 'tp'){
      $select = db_select('slt_program','crs');
      $select->condition('crs.status','lrn_lpn_sts_del','!=');
    }else if($op == 'content'){
      $select = db_select('slt_content_master','crs');
      $select->condition('crs.status','lrn_cnt_sts_del','!=');
    }else if($op == 'announce'){
      $select = db_select('slt_site_notice','crs');
      $select->condition('crs.status','cre_sys_obt_not_del','!=');
    }else if($op == 'customattribute' && module_exists('exp_sp_administration_customattribute')){ /*#custom_attribute_00789758*/
        $select = db_select('slt_custom_attr','crs');
        $select->condition('crs.status','cre_cattr_sts_del','!=');
    }
    $select->addExpression('COUNT(1)','COUNT');
    $select->condition(db_or()
        ->condition('crs.created_by', $userId, '=')
        ->condition('crs.updated_by', $userId, '='));
    expDebug::dPrintDBAPI('fetchManagedByRecordsCount',$select);
    $result=$select->execute()->fetchField();
    expDebug::dPrint('result of fetchManagedByRecordsCount'.print_r($result,true),3);
    return $result;
  }catch (Exception $ex) {
    expDebug::dPrint('Exception in fetchmanageby'.print_r($ex,true),1);
    watchdog_exception('fetchManagedByRecordsCount', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function checkboxFiltersetSelectionsToString() - Returns the selected checkbox filters pipe separated
 *                                                  Assumed, all code is in
 */
function checkboxFiltersetSelectionsToPrintableStr($filtersetCode) {
	try{
  $selectedFilters = getURLParam($filtersetCode);
  if($filtersetCode == "tpcurrency" || $filtersetCode == "classcurrency" || $filtersetCode == "coursecurrency") // Changed by Gayathri for 0070892
  	 return $selectedFilters;
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
       	if ($code == 'cre_sys_obt_ord' && $filtersetCode == 'customscreen'){
    		$value = t('LBL1025');
    	}
     	else if($code == 'cre_cattr_sts_atv')
     	{
     		$value = t('LBL921');   
     		 
     	}
     	else if($code == 'cre_cattr_sts_itv')
     	{
     		$value = t('LBL922');
     		     		 
     	}
    	else {
      $value = getProfileListItemName($code);
    	}
      if ($firstCode) {
        $selectedFiltersAsString .= t($value);
        $firstCode = false;
      }
      else {
        $selectedFiltersAsString .= ' | ' . t($value);
      }
    } // end foreach
  } // end if

  //$selectedFiltersAsString = (empty($selectedFiltersAsString))? 'All' : $selectedFiltersAsString;
  expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);

  return $selectedFiltersAsString;
  } catch (Exception $ex) {
  	watchdog_exception('checkboxFiltersetSelectionsToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function checkboxFiltersetCountrySelectionsToString() - Returns the selected country checkbox filters pipe separated
 */
function checkboxFiltersetCountrySelectionsToPrintableStr($filtersetCode) {
	try{
  $selectedFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
      $value = getCountryName($code);
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
  	watchdog_exception('checkboxFiltersetCountrySelectionsToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function checkboxFiltersetcustomdatatypesToPrintableStr() - Returns the textbox filterset value after urldecode.
 */
// custom attribute data types #custom_attribute_0078975
function checkboxFiltersetcustomdatatypesToPrintableStr($filtersetCode) {
    try{
        $selectedFilters = getURLParam($filtersetCode);
        expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
        $selectedFiltersAsString = '';
        if (!empty($selectedFilters)){
            $checkBoxFiltersCodeList = explode("|", $selectedFilters);
            $firstCode = true;
            foreach ($checkBoxFiltersCodeList as $code) {
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
                if ($code == 'cattr_type_txtbox')
                {
                    $value = t('LBL2009');
                }
                else if($code == 'cattr_type_txtarea')
                {
                    $value = t('LBL2010');
                }
                else if($code == 'cattr_type_dropdown')
                {
                    $value = t('LBL2007');
                }
                else if($code == 'cattr_type_radio')
                {
                    $value = t('LBL2008');
                }
                else if($code == 'cattr_type_checkbox')
                {
                    $value = t('LBL2006');
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

        //$selectedFiltersAsString = (empty($selectedFiltersAsString))? 'All' : $selectedFiltersAsString;
        expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);

        return $selectedFiltersAsString;
    } catch (Exception $ex) {
        watchdog_exception('checkboxFiltersetcustomdatatypesToPrintableStr', $ex);
        expertusErrorThrow($ex);
    }
}
/*
 * function textboxFiltersetValueToPrintableStr() - Returns the textbox filterset value after urldecode.
 */
function textboxFiltersetValueToPrintableStr($filtersetCode, $defaultText) {
	try {
  $textFilterStr = urldecode(getURLParam($filtersetCode));
  expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);
  $retStr = '';
  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
    $retStr = $textFilterStr;
  }

  expDebug::dPrint('$retStr = ' . print_r($retStr, true) , 4);
  return $retStr;
  } catch (Exception $ex) {
  	watchdog_exception('textboxFiltersetValueToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function textboxFiltersetValueToPrintableStr() - Returns the textbox filterset value after urldecode.
 */

function sliderFiltersetValueToPrintableStr($filtersetCode) {
	try{
  $sliderFilterStr = urldecode(getURLParam($filtersetCode));
  expDebug::dPrint(' $sliderFilterStr = ' . print_r($sliderFilterStr, true) , 4);
  $retStr = '';
  if (!empty($sliderFilterStr)){
    $retStr = str_replace("|","-",$sliderFilterStr);
  }
  return $retStr;
  } catch (Exception $ex) {
  	watchdog_exception('sliderFiltersetValueToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function daterangeFiltersetValueToPrintableStr() - Returns the daterange filterset value with the dates in international format.
 */
function daterangeFiltersetValueToPrintableStr($filtersetCode, $stripped = false) {
	try {
  $dateRangeFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $dateRangeFilters = ' . print_r($dateRangeFilters, true) , 4);
  $retStr = '';
  if (!empty($dateRangeFilters)) {
    $dateRange = explode("|", $dateRangeFilters);

    if (!empty($dateRange[0])) {
    	if ($stripped) {
    		$dateRange[0] = date_format(date_create_from_format('mdY', $dateRange[0]), 'm-d-Y');  // inserting dash char in dates
    	}
      $retStr .= t('LBL649').': ';
      $retStr .= dateTimeStrToUnivShort(convertDateToDBDateFormat($dateRange[0]), true, false, false, false);
    }
    else {
      $retStr .= '';
    }

    if (!empty($dateRange[1])) {
      if ($stripped) {
      	$dateRange[1] = date_format(date_create_from_format('mdY', $dateRange[1]), 'm-d-Y');  // inserting dash char in dates
      }
      $retStr .= '    '.t('LBL621').': ';
      $retStr .= dateTimeStrToUnivShort(convertDateToDBDateFormat($dateRange[1], true), true, false, false, false);
    }
    else {
      $retStr .= '';
    }
  }

  expDebug::dPrint('$retStr = ' . print_r($retStr, true) , 4);
  return $retStr;
  } catch (Exception $ex) {
  	watchdog_exception('daterangeFiltersetValueToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOffsetOfFirstRecordInPage() - Calculates and returns the offset of the first record on $page page.
 *                                  Also calculates and returns $totalPages and updates $page to last page if
 *                                  $page is beyond $totalPages.
 */
function getOffsetOfFirstRecordInPage(&$page, &$totalPages, $maxRecordsPerPage, $totalRecords,$isFromSearch=true) {
  // Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
  try{
  $crPage = $_COOKIE['current_page'];
  $updatePage='';
  $page= (int)$page;// pagination Float value truncator
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
  if(strrpos($_SERVER['HTTP_REFERER'],'admincalendar') === false)//#61417
  setcookie("current_page",$updatePage,null,"/","","","");
  if( $totalRecords > 0 ) {
    $totalPages = ceil($totalRecords / $maxRecordsPerPage);
  }
  else {
    $totalPages = 0;
  }

  if ($page > $totalPages) {
    $page = $totalPages;
  }

  $start = ($maxRecordsPerPage * $page) - $maxRecordsPerPage; // do not put $maxRecordsPerPage*($page - 1)
  if ($start < 0) {
    $start = 0;
  }

  return $start;
  } catch (Exception $ex) {
  	watchdog_exception('getOffsetOfFirstRecordInPage', $ex);
  	expertusErrorThrow($ex);
  }
}

function updatePaginationCookie($p=0){
	try{
	$crPage = $_COOKIE['current_page'];
	$level = explode('~',$crPage);
	($p==1)?$level[0] = str_replace('#0','#1',$level[0]):$level[1] = str_replace('#0','#1',$level[1]);
	$updatePage = implode('~',$level);
	setcookie('current_page',$updatePage);
	} catch (Exception $ex) {
		watchdog_exception('updatePaginationCookie', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * array_to_CSV() -
 * http://stackoverflow.com/questions/155097/microsoft-excel-mangles-diacritics-in-csv-files
 */
function array_to_CSV($data) {
  // NOTE: Do not handle exceptions here locally as the same function is also called by cron php.
  $sep  = "\t";
  $eol  = "\r\n";

  for ($i = 0; $i < count($data) ; $i++) {
    // Replaces a double quote with two double quotes
    $data[$i] = str_replace('"', '""', $data[$i]);
  }

  return '"' . implode('"' . $sep . '"', $data) . '"' . $eol;
}

/**
 * Converts report title, filters and headers to csv text to be prepended to the exported report
 */
function convertFiltersHeadersToCSVString($colHeadersMap, $filterArray, $reportHeader) {
	// NOTE: Do not handle exceptions here locally as the same function is also called by cron php.
  $csvContent = ''; // For saving the UTF-8 csv content
  $csvColumnHeaders = array(); // For extracting column headers from $colHeadersMap
  $csvDataCols = array(); // For extracting column indexes from $colHeadersMap
                            // to be used later to extract the csv data rows from $data
  if ($reportHeader != '') {
    $csvContent .= $reportHeader;
    $csvContent .= "\r\n\r\n";
  }

  if (count($filterArray) > 0) {
    foreach ($filterArray as $filters) {
      $csvContent .= array_to_CSV($filters);
    }
    $csvContent .= "\r\n";
  }

  // Show the column headers if $colHeadersMap is specified
  if (!empty($colHeadersMap)) {
    foreach ($colHeadersMap as $colTitle => $dataColIdx) {
      $csvColumnHeaders[] = $colTitle;
      $csvDataCols[] = $dataColIdx;
    }

    expDebug::dPrint('$csvColumnHeaders = ' . print_r($csvColumnHeaders, true) , 4);
    expDebug::dPrint('$csvDataCols = ' . print_r($csvDataCols, true) , 4);

    // Generate csv for the column headers
    $csvContent .= array_to_CSV($csvColumnHeaders);
  }

  expDebug::dPrint('$csvContent = ' . $csvContent, 4);
  return array($csvContent, $csvDataCols);
}

/**
 * Converts narrow search results to csv format data
 */
function convertNarrowSearchResultsToCSVString($data, $colHeadersMap = array(), $filterArray = array(), $reportHeader = '') {
	// NOTE: Do not handle exceptions here locally as the same function is also called by cron php.
  list($csvContent, $csvDataCols) = convertFiltersHeadersToCSVString($colHeadersMap, $filterArray, $reportHeader);

  if (isset($_GET['cron_key']) && empty($data)) {
  	$csvContent .= t('MSG278', array(),  array('langcode' => 'en-us')) . "\r\n"; //There are no records.
  }
  else {
	  // Loop through the data and generate csv data rows.
	  foreach ($data as $dataRow) {
	    expDebug::dPrint(' $dataRow = ' . print_r($dataRow, true) , 4);

	    // Prepare csv data row
	    $csvDataRow = array();

	    if (empty($csvDataCols)) {
	      foreach ($dataRow as $fieldValue) {
	        $csvDataRow[] = $fieldValue;
	      } // end foreach
	    }
	    else {
	      // Prepare csv file row based on user specified data columns in $colHeadersMap
	      foreach ($csvDataCols as $dataColIdx) {
	        $csvDataRow[] = $dataRow[$dataColIdx];
	      } // end foreach
	    }

	    expDebug::dPrint(' $csvDataRow = ' . print_r($csvDataRow, true), 4);

	    // Generate the csv data row
	    $csvContent .= array_to_CSV($csvDataRow);
	  } // end foreach
  }
  expDebug::dPrint(' $csvContent = ' . print_r($csvContent, true), 4);

  // Convert csv content to 'UTF-16LE
  $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');

  // Find byte length
  $csvContentLen = mb_strlen($csvContent, '8bit'); // recommended over strlen() as the latter can be overloaded in php.ini to behave like mb_strlen()
  expDebug::dPrint('$csvContentLen = ' . $csvContentLen, 4);

  return array($csvContent, $csvContentLen);
}

/**
 * Sends narrow search results as downloadable CSV file to the client (browser)
 */
function SendNarrowSearchResultsAsCSVFile($data, $colHeadersMap = array(), $asFilename = 'export',
                                                            $filterArray = array(), $reportHeader = '', $entity = '') {
	try {
	  $start = microtime(true);

	  drupal_set_time_limit(3600); // 1 hour Give sufficient time for query execution and CSV generation.

	  $asFilename = getDownloadFileName($asFilename, "csv");

	  if(!empty($filterArray)) {
	  	foreach($filterArray as $key => $value) {
	  		if(is_array($value)) {
	  			$value_filter_explode = explode(":", $value[0]);
	  			if(!empty($value_filter_explode[1])) {
	  				$value_filter_array[$key] = $value;
	  			}
	  		}
	  	}
	  }
	  $filterArray = $value_filter_array;

	  if (is_array($data)) { // old method - should not be used anymore or anywhere. Has the risk of exhausting all available memory.
	  	expDebug::dPrint('$data is an array', 4);
	    list($csvContent, $csvContentLen) = convertNarrowSearchResultsToCSVString($data, $colHeadersMap, $filterArray, $reportHeader);
	    printCSVOutputHeaders($asFilename, $csvContentLen);
	    $outstream = fopen("php://output", 'wb');
	    fwrite($outstream, $csvContent);
	    fclose($outstream);
	  }
	  else if (is_object($data)) {
	  	expDebug::dPrint('$data is an object');

	  	$tmpFolder = 'tmp';

	    if (!empty($entity) && function_exists($entity . '_csv_column_headers_alter')) {
	    	call_user_func($entity . '_csv_column_headers_alter', array(&$colHeadersMap));
	    }
	    list($csvContent, $csvDataCols) = convertFiltersHeadersToCSVString($colHeadersMap, $filterArray, $reportHeader);

      $objClass = get_class($data);
      expDebug::dPrint('$objClass = ' . print_r($objClass, true), 4);
      if ($objClass == 'SelectQuery' || $objClass == 'ExpertusSelectQueryExtender') {
        $dbstart = microtime(true);
        $csvList = $data->execute();
        $dbelapsed = microtime(true) - $dbstart;
        expDebug::dPrint("db_select()->execute() took $dbelapsed seconds.", 4);
      }
      else {
        $csvList = $data;
      }

      $numRows = $csvList->rowCount();
      expDebug::dPrint('$numRows = ' . $numRows, 4);

      $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('tmpcsv', true) . '.csv';
      expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);
      $tmpFile = fopen($tmpFileFullPath, 'wb');

      if ($numRows <= 0) {
        $csvContent .= t('MSG278', array(),  array('langcode' => 'en-us')) . "\r\n"; //There are no records.
        $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');
        fwrite($tmpFile, $csvContent);
      }
      else {
        $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');
        fwrite($tmpFile, $csvContent);

        expDebug::dPrint('$csvDataCols = ' . print_r($csvDataCols, true) , 4);

        $convstart = microtime(true);
        foreach ($csvList as $dataRow) {
        	if(is_array($dataRow)){
        		$dataRow =  array_change_key_case($dataRow,CASE_LOWER);
        		$dataRow = (object) $dataRow;
        	}
          //expDebug::dPrint(' $dataRow = ' . print_r($dataRow, true) , 4);
          if ((!empty($entity)) && function_exists($entity . '_csv_row_data_alter')) {
            call_user_func($entity . '_csv_row_data_alter', array(&$dataRow));
            //expDebug::dPrint('$dataRow after alter = ' . print_r($dataRow, true), 4);
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
            	if(arg(1) == 'commerce') {
            	  if($dataRow->$dataColIdx == 'Completed') {
            		$dataRow->$dataColIdx = 'Payment received';
            	  }
            	}
              $csvDataRow[] = $dataRow->$dataColIdx;
            } // foreach
          }
          //expDebug::dPrint(' $csvDataRow = ' . print_r($csvDataRow, true), 4);

          // Generate the csv data row
          $csvContent = array_to_CSV($csvDataRow);
          $csvContent = mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');
          fwrite($tmpFile, $csvContent);
        } // foreach
        $convelapsed = microtime(true) - $convstart;
        expDebug::dPrint("Fetching and conversion of data rows took $convelapsed seconds.", 4);
      }
      fclose($tmpFile);
      $fileSize = filesize($tmpFileFullPath);
      expDebug::dPrint('$fileSize = ' . $fileSize, 4);
      printCSVOutputHeaders($asFilename, $fileSize);
      expPrintFileToClient($tmpFileFullPath);
      unlink($tmpFileFullPath);
	  }
	  else if (is_string($data)) { //@TODO
	  }
	  else {
	  	throw new Exception('unknown data type');
	  }

	  $elapsed = microtime(true) - $start;
	  expDebug::dPrint("Total generate and send data as CSV took $elapsed seconds.", 5);

	  die;
	}
	catch (Exception $ex) {
  	watchdog_exception('SendNarrowSearchResultsAsCSVFile', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Prints a file to the client as cannot use PHP built-in function printfile() for large files
 */
function expPrintFileToClient($fileName) {
  try {
    ob_flush();
    flush();
  	$printStart = microtime(true);
	  expDebug::dPrint('In expPrintFileToClient with file name = ' . $fileName, 4);
	  $file = fopen($fileName, 'rb');
	  expDebug::dPrint('file opened', 4);
	  // see http://www.richnetapps.com/the-right-way-to-handle-file-downloads-in-php/ point 4
	  while(!feof($file)) {
      print(@fread($file, 8192));
      ob_flush();
      flush();
    }
	  expDebug::dPrint('file printed to client', 4);
	  fclose($file);

	  $printElapsed = microtime(true) - $printStart;
	  expDebug::dPrint("Sending file to client took $printElapsed seconds.", 4);
  }
  catch (Exception $ex) {
    watchdog_exception('expPrintFileToClient', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Prints CSV file headers to client
 */
function printCSVOutputHeaders($asFilename, $csvContentLen) {
	try {
	  /*** Output the csv headers ***/

	  ob_clean(); //to clean the output buffers, or else some stray data in the buffer can cause some breaks.

	  /* HEADERS FOR THE CSV FILE */
	  header('Cache-Control: no-store, no-cache'); //cache must not store any part of either this response, submit the request to the origin server for validation before releasing a cached copy, every time
	  header("Pragma: no-cache"); //cache MUST NOT use the response to satisfy a subsequent request without successful revalidation with the origin server
	  header("Expires: 0"); //Another instruction to prevent caching
	  // http://stackoverflow.com/questions/155097/microsoft-excel-mangles-diacritics-in-csv-files
	  header('Content-Description: File Transfer');
	  header('Content-Type: application/vnd.ms-excel');
	  // Specify response is a file attachment. Set file name.
	  header('Content-disposition: attachment; filename="' . $asFilename . '"');
	  // Prevent auto conversion of /n to /n/r
	  // See http://stackoverflow.com/questions/2232103/how-do-i-get-csv-file-to-download-on-ie-works-on-firefox
	  header("Content-Transfer-Encoding: binary"); // returns the file as binary octets, that forces all the browers to use this content transfer encoding mechanism.
	  header('Content-Type: application/force-download', false); // recommended for IE7 to open/save dialog
	  //header("Content-Type: application/octet-stream", false, 200); //returning the content type as octet stream and forcing the OK status.
	  header('Pragma: public');
	  header('Content-Length: '. $csvContentLen);
	  expDebug::dPrint('CSV headers are sent to the client');
  }
  catch (Exception $ex) {
    watchdog_exception('printCSVOutputHeaders', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Prepares the download file name - Adds a datetime stamp to the file name
 */
function getDownloadFileName($baseName, $type){
	try {
    return $baseName."_".date('dmY')."_".date('His').".".$type;
  }
  catch (Exception $ex) {
  	watchdog_exception('getDownloadFileName', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Creates mpdf object to be used for pdf generation
 */
function createMpdfObject($orientation, $title) {
  // NOTE: Do not handle exceptions here locally as the same function is also called by cron php.
  //       Let the caller handle any exception.
  require_once('sites/all/commonlib/mpdf/mpdf.php');
  $mpdf = new mPDF('UTF-8', (($orientation == 'L')? 'A4-L' : 'A4'));
  $mpdf->SetTitle($title);
  $mpdf->SetAuthor(variable_get('site_name', 'ExpertusONE'));

  $mpdf->SetDisplayMode('real');
  $mpdf->pagenumSuffix = ' / ';
  $mpdf->SetFooter('|{PAGENO}{nbpg}|');

  // @TODO: See http://mpdf1.com/manual/index.php?tid=408 for a probable memory problem fix,
  //       Also see https://code.google.com/p/wkhtmltopdf/ for an alternative tool
  $mpdf->keep_table_proportions = true;
  $mpdf->simpleTables = true;
  $mpdf->packTableData = true;
  $mpdf->cacheTables = true;

  //$mpdf->SetProtection(array('print'));

  $mpdf->useAdobeCJK = true; // Use Adobe CJK fonts for CJK languages
  $mpdf->SetAutoFont(AUTOFONT_ALL); // AUTOFONT_CJK | AUTOFONT_THAIVIET | AUTOFONT_RTL | AUTOFONT_INDIC // AUTOFONT_ALL
                                        // () = default ALL, 0 turns OFF (default initially)

  return $mpdf;
}

/**
 * Converts narrow search results to mPDF object
 */
function createMpdfObjectForNarrowSearchResults(&$data = array(), $title = 'Search Results', $appliedFilters = array(),
                                                                     $colHeadersMap = array(), $fontSize = 11, $orientation = 'P') {
  // NOTE: Do not handle exceptions here locally as the same function is also called by cron php.
  //       Let the caller handle any exception.
  expDebug::dPrint('$data = ' . print_r($data, true), 5);
  expDebug::dPrint('$title = ' . print_r($title, true), 5);
  expDebug::dPrint('$appliedFilters = ' . print_r($appliedFilters, true), 5);
  expDebug::dPrint('$colHeadersMap = ' . print_r($colHeadersMap, true), 5);
  expDebug::dPrint('$fontSize = ' . print_r($fontSize, true), 5);
  expDebug::dPrint('$orientation = ' . print_r($orientation, true), 5);

  $content = theme('search-results-pdf', array(
                   'data' => $data,
                   'title' => $title,
                   'applied_filters' => $appliedFilters,
                   'header_field_map' => $colHeadersMap,
                   'font_size' => $fontSize
              ));
  $mpdf = createMpdfObject($orientation, $title);
  $mpdf->WriteHTML($content);

  return $mpdf;
}

/**
 * Implements print to pdf functionality from the UI
 */
function SendNarrowSearchResultsAsPDFFile(&$data = array(), $title = 'Search Results', $appliedFilters = array(),
                                                               $colHeadersMap = array(), $asFilename = 'export',
                                                                    $fontSize = 11, $orientation = 'P', $entity = '',$column_cont_msg=false) {
  try {
  	$start = microtime(true);
	  expDebug::dPrint('$data = ' . print_r($data, true), 5);
	  expDebug::dPrint('$title = ' . print_r($title, true), 5);
	  expDebug::dPrint('$appliedFilters = ' . print_r($appliedFilters, true), 5);
	  expDebug::dPrint('$colHeadersMap = ' . print_r($colHeadersMap, true), 5);
	  expDebug::dPrint('$asFilename = ' . print_r($asFilename, true), 5);
      expDebug::dPrint('$fontSize = ' . print_r($fontSize, true), 5);
      expDebug::dPrint('$orientation = ' . print_r($orientation, true), 5);

    ini_set('memory_limit','512M');
    drupal_set_time_limit(3600); // 1 hour Give sufficient time for query execution and CSV generation.

    if (is_array($data)) { // old method - should not be used anymore or anywhere. Has the risk of exhausting all available memory.
      expDebug::dPrint('$data is an array', 4);
      $mpdf = createMpdfObjectForNarrowSearchResults($data, $title, $appliedFilters, $colHeadersMap, $fontSize, $orientation);
    }
    else if (is_object($data)) {
      expDebug::dPrint('$data is an object');

      if (!empty($entity) && function_exists($entity . '_pdf_column_headers_alter')) {
        call_user_func($entity . '_pdf_column_headers_alter', array(&$colHeadersMap));
      }

      $objClass = get_class($data);
      expDebug::dPrint('$objClass = ' . print_r($objClass, true), 4);
      if ($objClass == 'SelectQuery' || $objClass == 'ExpertusSelectQueryExtender') {
        $dbstart = microtime(true);
        $pdfList = $data->execute();
        $dbelapsed = microtime(true) - $dbstart;
        expDebug::dPrint("db_select()->execute() took $dbelapsed seconds.", 4);
      }
      else {
        $pdfList = $data;
      }

      $numRows = $pdfList->rowCount();
      expDebug::dPrint('$numRows = ' . $numRows, 4);

      $content = theme('search-results-pdf', array('title' => $title,
                                                   'applied_filters' => array_filter($appliedFilters),
      		                                         /*'applied_filters' => $appliedFilters,*/
                                                   'header_field_map' => $colHeadersMap,
                                                   'font_size' => $fontSize,
                                                   'only_header' => true));
      $mpdf = createMpdfObject($orientation, $title);
      $mpdf->WriteHTML($content, 2, true, false);
      unset($content);

      if ($numRows <= 0) {
      	$content =
      	  '<tr>' .
            '<td align="left" valign="top" colspan="' . count($colHeadersMap) . '" align="center">' . t('MSG278') . '</td>' . //There are no records.
          '</tr>';
      	expDebug::dPrint('$content = ' . $content, 4);
      	$mpdf->WriteHTML($content, 2, false, false);
      	unset($content);
      }
      else {
        $convstart = microtime(true);

	      $pdfMaxRecords = 500; // default 500 records
	      // Allow override through exp_sp.ini
	      require_once('sites/all/services/GlobalUtil.php');
	      $gutil = new GlobalUtil();
	      $expConfig = $gutil->getConfig();
	      $configPdfMaxRecords = trim($expConfig['pdf_max_records']);
	      if (!empty($configPdfMaxRecords)) {
	        $pdfMaxRecords = $configPdfMaxRecords;
	      }
	      $rowsToPrint = ($numRows > $pdfMaxRecords)? $pdfMaxRecords : $numRows;

        $i = 0;
        foreach ($pdfList as $dataRow) {
        	if(is_array($dataRow)){
        		$dataRow =  array_change_key_case($dataRow,CASE_LOWER);
        		$dataRow = (object) $dataRow;
        	}
          //expDebug::dPrint('$dataRow = ' . print_r($dataRow, true) , 4);
          if ((!empty($entity)) && function_exists($entity . '_pdf_row_data_alter')) {
            call_user_func($entity . '_pdf_row_data_alter', array(&$dataRow));
            //expDebug::dPrint('$dataRow after alter = ' . print_r($dataRow, true), 4);
          }

          $mpdf->WriteHTML('<tr>', 2, false, false);
          foreach ($colHeadersMap as $hlabel => $hdetails) {
         // expDebug::dPrint("hlabel = $hlabel, hdetails = ".print_r($hdetails,true).", dataRow->hdetails[0] before = ".print_r($dataRow->$hdetails[0],true)."", 4);
          if(arg(1) == 'commerce') {
	       	if($dataRow->$hdetails[0] == 'Completed')  {
	       		$dataRow->$hdetails[0] = 'Payment received';
	       	}
	      }
          	$mpdf->WriteHTML('<td align="left" valign="top">' . $dataRow->$hdetails[0] . '</td>', 2, false, false);
          } // foreach
          $mpdf->WriteHTML('</tr>', 2, false, false);
          $i += 1;
          if ($i >= $rowsToPrint) {
          	break;
          }
        } // foreach
        $convelapsed = microtime(true) - $convstart;
        expDebug::dPrint("Fetching and conversion of data rows took $convelapsed seconds.", 4);

        if($column_cont_msg) {
        	$msgFontSize = (($fontSize > 8)? 8 : $fontSize);
        	$content_column=
        	'<tr>' .
        	'<td align="left" valign="top" colspan="' . count($colHeadersMap) .
        	'" align="right" style="color:red; font-size:' . $msgFontSize . 'pt; border-bottom:none; border-right:none; border-left:none; padding-top: 1em; padding-right: 0;">' .
        	t('MSG724') . ' (12). ' . t('MSG706') . //There is a limit to the number of records that can be exported in pdf format
        	//Use csv format for list of all records.
        	'</td>' .
        	'</tr>';
        	expDebug::dPrint('$$content_column = ' . $content_column, 4);
        	$mpdf->WriteHTML($content_column, 2, false, false);
        	unset($content_column);
        }

        if ($numRows > $pdfMaxRecords) {
        	$msgFontSize = (($fontSize > 8)? 8 : $fontSize);
	        $content =
	          '<tr>' .
	            '<td align="left" valign="top" colspan="' . count($colHeadersMap) .
	                '" align="right" style="color:red; font-size:' . $msgFontSize . 'pt; border-bottom:none; border-right:none; border-left:none; padding-top: 1em; padding-right: 0;">' .
	              t('MSG705') . ' (' . $pdfMaxRecords . '). ' . t('MSG706') . //There is a limit to the number of records that can be exported in pdf format
	                                                                         //Use csv format for list of all records.
	            '</td>' .
	          '</tr>';
	        expDebug::dPrint('$content = ' . $content, 4);
	        $mpdf->WriteHTML($content, 2, false, false);
	        unset($content);
        }

      }
      $content = theme('search-results-pdf', array('only_footer' => true));
      $mpdf->WriteHTML($content, 2, false, true);
      unset($content);
    }
    $outputstart = microtime(true);
    $asFilename = getDownloadFileName($asFilename, "pdf");
    expDebug::dPrint('outputting html to file ' . $asFilename, 5);
    $mpdf->Output($asFilename, 'D');
    $outputelapsed = microtime(true) - $outputstart;
    expDebug::dPrint("Output pdf to client took $outputelapsed seconds.", 4);

    $elapsed = microtime(true) - $start;
	  expDebug::dPrint("Total generate and send data as PDF took $elapsed seconds.", 4);
  }
  catch(Exception $ex) {
    watchdog_exception('SendNarrowSearchResultsAsPDFFile', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getNarrowSearchSortTitle() - get sort title for $code argument if provided or for code in url param sortby
 */
function getNarrowSearchSortTitle($code = '', $default='AZ') {
	try{
  $code = empty($code)? getURLParam('sortby') : $code;
  $code = empty($code)? $default : $code;

  switch ($code) {
    case "AZ" :
                return t('LBL017');

    case "ZA" :
                return t('LBL018');

    case "Number" :
                return t('LBL161');

    case "NewlyListed" :
    case "Time" : // used by lnrreports
                return t('LBL044');

    default:
                return 'No such Sort Code';
  } // end switch
  } catch (Exception $ex) {
  	watchdog_exception('getNarrowSearchSortTitle', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * getAdminModules($baseModule,$subModule) - Get the submodules under a modules in Admin
 */
function getAdminModules($baseModule,$subModule) {
	try{
  $tmpModuleStr=trim($baseModule);
  if($tmpModuleStr!=""){
    $tmpModuleStr.="/".$subModule;
  }
  if($subModule=='learning'){
    $subModule_title= t('Learning');
  }elseif($subModule=='people'){
     $subModule_title= t('People');
  }elseif($subModule=='survey'){
     $subModule_title= t('Survey');
  }elseif($subModule=='assessment'){
     $subModule_title= t('Assessment');
  }elseif($subModule=='manage'){
     $subModule_title= t('Manage');
  }elseif($subModule=='contentauthor'){
    $subModule_title = t('LBL3021').t(' ').t('LBL3088').t(' (Beta)');
  }elseif($subModule=='commerce'){
     $subModule_title= t('Commerce');
  }elseif($subModule=='sitesetup'){
     $subModule_title= t('Site Setting');
  }elseif($subModule=='setting'){
     $subModule_title= t('LBL1021');
  }else{
   $subModule_title= $subModule;
  }
  $tmpMenuArrLinks=getTabDetailsForAdminGroup($tmpModuleStr);

  $themeHolderStr=theme('root_admin_tab_holder',
                      array(
                                      'tab_title' => $subModule_title,
                            'admin_sub_links_arr' => $tmpMenuArrLinks,
                           )
                       );
   //echo $themeHolderStr;
   return $themeHolderStr;
   } catch (Exception $ex) {
   	watchdog_exception('getAdminModules', $ex);
   	expertusErrorThrow($ex);
   }
}

/**
 * getTabDetailsForAdminGroup() - Given a base menu path, finds all first level children from menu_router table to show as tabs
 *                                in the admin screen.
 */
function getTabDetailsForAdminGroup($basePath) {
	try{
		$basePath = $basePath . '/';
		if(isset($_SESSION['BasePath'][$basePath]))
			return $_SESSION['BasePath'][$basePath];
  $routerTableData = menu_get_router(); // Get the menu router data
  //expDebug::dPrint(' $routerTableData = ' . print_r($routerTableData, true) , 5);

  $basePathLength = strlen($basePath);
  $tabDetailsList = array(); // for saving the fetched tabs list details
  foreach ($routerTableData as $key => $value) {
    if ($value['type'] == MENU_CALLBACK && strpos($key, $basePath) === 0) { // strpos() may return Boolean FALSE, but may also return a non-Boolean
                                                      // value 0 which evaluates to FALSE. So the === operator is required here.
      expDebug::dPrint(' found $key = ' . print_r($key, true) , 4);
      $pathExtension = substr($key, $basePathLength);

      expDebug::dPrint(' $pathExtension = ' . print_r($pathExtension, true) , 4);
      
      global $user;
      
      if($key=='administration/sitesetup/config' && $pathExtension=="config" && $user->uid!='1'){ //Not to show "Administer > SiteSetup > Configuration page" to Administer Users.
      	expDebug::dPrint('Logged user is not a Drupal Admin. So, need not to show the Administer > SiteSetup > Configuration page.', 4);
      }else{
      $haspermission=false;
      $userpermission = $value['access arguments'];
      foreach($userpermission as $k => $v){
      	if($haspermission != true)
      		$haspermission = check_permission($v);
      }
      expDebug::dPrint(' $userpermission = ' . print_r($userpermission, true) , 4);
      if (strpos($pathExtension, '/') === false && $haspermission=== true) {
        $tabDetails = array();
        $tabDetails['title'] = $value['title'];
        $tabDetails['path'] = $key;
        $tabDetails['weight'] = empty($value['weight'])? 900: $value['weight']; // Show the unweighted links at the end
        $tabDetailsList[] = $tabDetails;
      }
    }
   }
  } // end foreach

  expDebug::dPrint(' $tabDetailsList = ' . print_r($tabDetailsList, true) , 4);

  // Sort the tabs details list by weight ascending. Remove the weight field as not required after the sort.
  $tabDetailsSortedList = record_sort($tabDetailsList, 'weight', true);  // 3rd arg = true instructs record_sort to remove the field
  expDebug::dPrint(' $tabDetailsSortedList = ' . print_r($tabDetailsSortedList, true) , 4);
  $_SESSION['BasePath'][$basePath] = $tabDetailsSortedList;
  return $tabDetailsSortedList;
  } catch (Exception $ex) {
  	watchdog_exception('getTabDetailsForAdminGroup', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * ctools_addedit_form_submit_success() -
 */
function ctools_addedit_form_submit_success() {
	try{
  expDebug::dPrint(' In ctools_addedit_form_submit_success()');
  } catch (Exception $ex) {
  	watchdog_exception('ctools_addedit_form_submit_success', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_ctools_modal_multi_ajaxonly_forms_display -
 */
function exp_ctools_modal_multi_ajaxonly_forms_display($formInfoList = array(), $argsList = array(), $otherArgsList = array(),$commands = array()) {
	try {
  if (empty($formInfoList)) {
      throw new Exception('exp_sp_administration.inc : exp_ctools_modal_multi_ajax_forms_display() : $formIdList is empty.');
  }
  ctools_include('modal');
  ctools_include('ajax');

  // Prepare title for the modal dialog
  $title = empty($formInfoList[0]['title']) ? drupal_get_title() : $formInfoList[0]['title'];

  $renderMode = 'EDIT';
  if (empty($argsList)) {
    $renderMode = 'ADD';
  }

  $output = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, $renderMode, $otherArgsList);

  //$commands = array();
  // If the form has not yet been rendered, render it.
  $commands[] = ctools_modal_command_display($title, $output);
  $commands[] = exp_destroy_ckeditors_on_modal_close();
  $commands[0]['output'] = mb_convert_encoding($commands[0]['output'], "HTML-ENTITIES", "UTF-8");
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  print ajax_render($commands);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('exp_ctools_modal_multi_ajaxonly_forms_display', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_ctools_modal_multi_ajaxonly_forms_display_narrow_refresh_onclose -
*/
function exp_ctools_modal_multi_ajaxonly_forms_display_narrow_refresh_onclose($formInfoList = array(), $argsList = array(), $otherArgsList = array(),$commands = array()) {
	try {
		if (empty($formInfoList)) {
			throw new Exception('exp_sp_administration.inc : exp_ctools_modal_multi_ajaxonly_forms_display_narrow_refresh_onclose() : $formIdList is empty.');
		}
		ctools_include('modal');
		ctools_include('ajax');

		// Prepare title for the modal dialog
		$title = empty($formInfoList[0]['title']) ? drupal_get_title() : $formInfoList[0]['title'];

		$renderMode = 'EDIT';
		if (empty($argsList)) {
			$renderMode = 'ADD';
		}

		$output = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, $renderMode, $otherArgsList);

		//$commands = array();
		// If the form has not yet been rendered, render it.
		$commands[] = ctools_modal_command_display($title, $output);
		$commands[] = exp_destroy_ckeditors_on_modal_close();
		$commands[] = ajax_command_refresh_narrow_search_results('onclose');
		expDebug::dPrint(' exp_ctools_modal_multi_ajaxonly_forms_display $commands = ' . print_r($commands, true) , 4);

		print ajax_render($commands);
		exit;
	} catch (Exception $ex) {
		watchdog_exception('exp_ctools_modal_multi_ajaxonly_forms_display', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * exp_ctools_modal_multi_ajaxonly_forms_render -
 */
function exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList = array(), $argsList = array(), $renderMode = 'ADD', $otherArgsList = array(),$files = array()) {
	try{
  expDebug::dPrint(' $formInfoList = ' . print_r($formInfoList, true) , 4);
  expDebug::dPrint(' $argsList = ' . print_r($argsList, true) ,4);
  expDebug::dPrint(' $renderMode = ' . print_r($renderMode, true) , 4);
  expDebug::dPrint(' $files = ' . print_r($files, true) , 4);

  $min_form_state = array(
   'ajax' => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed.
                   // Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
   're_render' => FALSE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
   'no_redirect' => TRUE, // Prevent any redirection, required forms are built in ajax callback
   'build_info' => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
  );

  // To handle the view and edit mode
  if(isset($argsList['display_mode'])){
    $form_state['build_info']['args']['display_mode'] = $argsList['display_mode'];
  }

  $output = '';
  switch ($renderMode) {
    case 'ADD':
      expDebug::dPrint(' In switch case ' . $renderMode , 4);
      $formId = $formInfoList[0]['id'];
      // Build the form state
      $form_state = $min_form_state;
      $form_state['storage']['form_info_list'] = $formInfoList; // The forms list needs to be saved for later building dependant forms when new record
                                                                // has been added.
      foreach ($argsList as $arg) {
        $form_state['build_info']['args'][] = $arg;
      }
		  if(count($files)>0){
		  	$form_state['build_info']['files'] = $files;
		  }
      $form_state['build_info']['other_args'] = $otherArgsList;
      $output .= exp_ctools_modal_multi_ajaxonly_forms_renderform($formId, $form_state);
      break;

    case 'EDIT-RELOAD':
      expDebug::dPrint(' In switch case ' . $renderMode , 4);
      $formId = $formInfoList[1]['id'];

      // Build the form state
      $form_state = $min_form_state;
      $form_state['storage']['form_info_list'] = $formInfoList; // The forms list needs to be saved for later building dependant forms when new record
                                                                // has been added.
      foreach ($argsList as $arg) {
        $form_state['build_info']['args'][] = $arg;
      }
  		if(count($files)>0){
		  	$form_state['build_info']['files'] = $files;
		  }
      $form_state['build_info']['other_args'] = $otherArgsList;
      $output .= exp_ctools_modal_multi_ajaxonly_forms_renderform($formId, $form_state);
      break;

    case 'EDIT':
      expDebug::dPrint(' In switch case ' . $renderMode , 4);
      $formId = $formInfoList[0]['id'];

      // Build the form state
      $form_state = $min_form_state;
      $form_state['storage']['form_info_list'] = $formInfoList; // The forms list needs to be saved for later building dependant forms when new record
                                                                // has been added.
      foreach ($argsList as $arg) {
        $form_state['build_info']['args'][] = $arg;
      }
  		if(count($files)>0){
		  	$form_state['build_info']['files'] = $files;
		  }
      $form_state['build_info']['other_args'] = $otherArgsList;
      $output .= exp_ctools_modal_multi_ajaxonly_forms_renderform($formId, $form_state);

    case 'ADD-TO-EDIT': // Render second form onwards
      expDebug::dPrint(' In switch case ' . $renderMode , 4);
      $i = 1;
      $numForms = count($formInfoList);
      for ($i = 1; $i < $numForms; $i++) {
        $formId = $formInfoList[$i]['id'];
        $formTitle = $formInfoList[$i]['title'];
        // Build the form state
        $form_state = $min_form_state;
        foreach ($argsList as $arg) {
          $form_state['build_info']['args'][] = $arg;
        }
	      if(count($files)>0){
			  	$form_state['build_info']['files'] = $files;
			  }
        $form_state['build_info']['other_args'] = $otherArgsList;
        $renderedForm = exp_ctools_modal_multi_ajaxonly_forms_renderform($formId, $form_state);
        //Using the below veriable we can find current loaded theme name
				global $theme_key;
        if (!empty($renderedForm)) {
        	if($theme_key == 'expertusoneV2') {
	          // Render the title of additional forms
	           if(!empty($formInfoList[$i]['type'] ) && $formInfoList[$i]['type'] == 'program_tabs'){
	          		$output .= '<div id="'.$formId.'"></div>';
	           }else{
	          $output .= '<div class="addedit-addl-form-title"><div class="block-title-left"><div class="block-title-right"><span class="addedit-addl-form-title-text block-title-middle">' . $formTitle . '</span></div></div></div>';
	          $output .= '<div id="'.$formId.'" class="addedit-addl-form-newui-action-btn"></div>';
	           }
        	} else {
        		$output .= '<div class="addedit-addl-form-title"><span class="addedit-addl-form-title-text">' . $formTitle . '</span></div>';
        	}
          // Append form to output
          $output .= $renderedForm;
        }
      } // end for
      break;
  } // end switch

  expDebug::dPrint(' $output = ' . print_r($output, true) , 4);
  return $output;
  } catch (Exception $ex) {
  	watchdog_exception('exp_ctools_modal_multi_ajaxonly_forms_render', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_ctools_modal_multi_ajaxonly_forms_renderform() -
 */
function exp_ctools_modal_multi_ajaxonly_forms_renderform($formId, &$form_state) {
	try{
  expDebug::dPrint(' $formId = ' . print_r($formId, true) , 5);
  expDebug::dPrint(' before drupal_build_form() $form_state = ' . print_r($form_state, true) , 5);
  $builtForm = drupal_build_form($formId, $form_state);
  expDebug::dPrint(' after drupal_build_form() $form_state = ' . print_r($form_state, true) , 5);

  if (!empty($form_state['executed'])) {
    // This should not have happened as form with #ajax sumit buttons only.
    throw new Exception('exp_sp_administration.inc : exp_ctools_modal_multi_ajaxonly_forms_renderform() : $form_state[executed] is set.');
  }

  expDebug::dPrint(' $builtForm = ' . print_r($builtForm, true) , 4);

  $renderedForm = '';
  if (is_array($builtForm)) {
    $renderedForm = drupal_render($builtForm);
    expDebug::dPrint(' $renderedForm = ' . print_r($renderedForm, true) , 4);
  }

  return $renderedForm;
  } catch (Exception $ex) {
  	watchdog_exception('exp_ctools_modal_multi_ajaxonly_forms_renderform', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * ajax_command_refresh_narrow_search_results() - Interface to a custom ajax command to refresh a jgrid.
 */
function ajax_command_refresh_narrow_search_results($refreshMode, $refreshGrid) {
	try{
		expDebug::dPrint('debug back trace '.print_r(debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS), 1));
		expDebug::dPrint('Commands '.$refreshMode.'-'.$refreshGrid);
  return array (
    'command' => 'refreshnarrowsearchresults',
    'js_object_selector' => '#root-admin',
    'js_object' => 'narrowsearch',
    'refreshMode' => $refreshMode,
  	'refreshGrid' => $refreshGrid
  );
  } catch (Exception $ex) {
  	watchdog_exception('ajax_command_refresh_narrow_search_results', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_ctools_modal_adjust() - Interface to a custom ajax command to resize ctools modal backdrop when ctools modal size has increased and
 *                         to adjusts ctools modal top to the top of the view area when the ctools modal size stretches beyond the view area.
 */
function exp_ctools_modal_adjust() {
	try{
  return array (
    'command' => 'CtoolsModalAdjust'
  );
  } catch (Exception $ex) {
  	watchdog_exception('exp_ctools_modal_adjust', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * exp_destroy_ckeditors_on_modal_close() - This is run when at ctools modal close to destroy any ckeditors present when we close
 *                                                   the modal by clicking on x.
 */
function exp_destroy_ckeditors_on_modal_close() {
	try{
  return array (
    'command' => 'destroyCKEditorsOnCtoolsModalClose',
  );
  } catch (Exception $ex) {
  	watchdog_exception('exp_destroy_ckeditors_on_modal_close', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addedit_hide_messages() - Interface to a custom ajax command to hide messages after some delay.
 * return ''- display messages are changed as close button is enabled, so no need for delay.
 */
function addedit_hide_messages($wrapperId = '', $delay = 20000) {
  /*return array (
    'command' => 'addEditHideMessages',
    'wrapper_id' => $wrapperId,
    'delay' => $delay
  );*/
	try{
	return '';
	} catch (Exception $ex) {
		watchdog_exception('addedit_hide_messages', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addedit_popup_dialog_scroll() - Interface to a custom ajax command to hide messages after some delay.
 */
function addedit_popup_dialog_scroll($wrapperId = '') {
	try {
  return array (
    'command' => 'addEditPopupDialogScroll',
    'wrapper_id' => $wrapperId
  );
  } catch (Exception $ex) {
  	watchdog_exception('addedit_popup_dialog_scroll', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addEditGetPrefix() -  Provide HTML for #prefix for the addedit form fields and actions
 */
/*#custom_attribute_0078975*/
function addEditGetPrefix($type, $fieldTitle = '', $mandatory = false, $separatorSymbol = ':', $prefixId = '',$pagefrom='',$info_symbol_arr=array()){
 try{
  $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";
   
  //Information details
 if(isset($info_symbol_arr) && !empty($info_symbol_arr)){ 
     $infoSymbol='<span id="'.$info_symbol_arr['span_id'].'">
                  <img src="'.$info_symbol_arr['image_path'].'" class="'.$info_symbol_arr['class_name'].'" title="'.$info_symbol_arr['msg'].'" onload="vtip();">
                </span>';
 }

  switch (strtoupper($type)){
    case 'FIELD':
      return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
               '<div class="addedit-new-field-title">' .
                 $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
               '</div>' .
               '<div class="addedit-new-field-value">';
      break;

    case 'SINGLEFIELD':
      return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
                 '<div class="addedit-singlefield-title">' .$infoSymbol.
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
                 $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
               '</div>' .
               '<div class="addedit-new-field-value">' .
                 '<div class="addedit-new-datefield-value">';
    	break;

    case 'DOUBLEFIELD':
      return '<div class="addedit-new-field" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
               '<div class="addedit-new-field-title">' .
                 $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
               '</div>' .
               '<div class="addedit-new-field-value">';
      break;

    case 'DOUBLEDROPDOWN':
      return  '<div class="addedit-new-field-title dropspecific-width" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
                $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
              '</div>' .
              '<div class="addedit-new-field-value">';
      break;

    case 'STATUSFIELD':
      return  '<div class="addedit-new-field-title textspecific-width" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
                $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
              '</div>' .
              '<div class="addedit-new-field-value addedit-textbox-daterange-date-container">';
      break;

    case 'DOUBLEDROPDOWNCURRENCY':
      return  '<div class="addedit-new-field-title curreny-type-label" '.(!empty($prefixId) ? ('id="'.$prefixId.'"') : '').' >' .
                $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
              '</div>' .
              '<div class="addedit-new-field-value">';
      break;

    case 'TWOCOL_FIRSTCOL_FIELD':
		  return '<div class= "addedit-twocol-firstcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
		           '<div class="addedit-new-field-title">' .
                     '<span class="addedit-label">'.$fieldTitle . $separatorSymbol .'</span>'. ($mandatory? $mandatorySymbol : '') .$infoSymbol.
		           '</div>' .
		           '<div class="addedit-firstcol-field-value">';
		  break;

    case 'TWOCOL_SECONDCOL_FIELD':
    if($pagefrom == 'exp_meeting' || $pagefrom == 'other_meet'){
		  return  '<div class= "addedit-twocol-secondcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
                '<div class="addedit-new-field-title vtip" title='.$fieldTitle.'>' .
                  titleController('PRESENTER-LABEL',$fieldTitle).$separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
                '</div>' .
                '<div class="addedit-secondcol-field-value">';
    }else{
		  return  '<div class= "addedit-twocol-secondcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
                '<div class="addedit-new-field-title test">' .
                  '<span class="addedit-label">'.$fieldTitle . $separatorSymbol .'</span>'. ($mandatory? $mandatorySymbol : '') .$infoSymbol.
                '</div>' .
                '<div class="addedit-secondcol-field-value">';
    }
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
                 $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
               '</div>' .
               '<div class="addedit-firstcol-field-value">' .
                 '<div class="addedit-new-datefield-value">';
      break;

    case 'TWOCOL_SECONDCOL_DATEFIELD':
      return  '<div class= "addedit-twocol-secondcol" ' . (!empty($prefixId) ? ('id="'.$prefixId.'"') : '').'>' .
                '<div class="addedit-new-field-title">' .
                  $fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .$infoSymbol.
                '</div>' .
                '<div class="addedit-secondcol-field-value">' .
                  '<div class="addedit-new-datefield-value">';
      break;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addEditGetPrefix', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addEditGetSuffix() - Provide HTML for #prefix for the addedit form fields and actions
 */
function addEditGetSuffix($type, $fieldTitle = '', $mandatory = false, $separatorSymbol = ':'){
 try{
  $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";

  switch (strtoupper($type)){
    case 'FIELD':
      return   '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
      break;

    case 'DATEFIELD' :
      return     '</div>' .
                 '<div class="clearBoth"></div>' .
               '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
      break;

    case 'ACTION':
      //return '</span></td></tr></table>';
      return '<div class="admin-save-button-right-bg"></div></div></div></span><div style="clear: both;"></div></div>';
      break;

    case 'CHECKBOX_TITLE_SUFFIXED' :
      return     '<div class="addedit-checkbox-suffixed-title">' .
                   $fieldTitle . ($mandatory? $mandatorySymbol : '') .
                 '</div>' .
               '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
    	break;

    case 'DOUBLEFIELD':
      return   '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
      break;

    case 'DOUBLEFIELDCLOSE':
      return   '</div>' ;
      break;

    case 'TWOCOL_FIRSTCOL_FIELD':
      return     '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
        break;

    case 'TWOCOL_SECONDCOL_FIELD':
         return     '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
         break;

    case 'TWOCOL_FIRSTCOL_CHECKBOX_TITLE_SUFFIXED' :
      return     '<div class="addedit-checkbox-suffixed-title">' .
                   $fieldTitle . ($mandatory? $mandatorySymbol : '') .
                 '</div>' .
               '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
      break;

    case 'TWOCOL_SECONDCOL_CHECKBOX_TITLE_SUFFIXED' :
      return     '<div class="addedit-checkbox-suffixed-title">' .
                   $fieldTitle . ($mandatory? $mandatorySymbol : '') .
                 '</div>' .
               '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
      break;

    case 'TWOCOL_FIRSTCOL_DATEFIELD':
      return     '</div>' .
                 '<div class="clearBoth"></div>' .
               '</div>' .
             '<div class="clearBoth"></div>' .
           '</div>';
        break;

    case 'TWOCOL_SECONDCOL_DATEFIELD':
         return    '</div>' .
                   '<div class="clearBoth"></div>' .
                 '</div>' .
               '<div class="clearBoth"></div>' .
             '</div>';
         break;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addEditGetSuffix', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * newAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom
 */
function newAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
  try{
  $form = array();
  //  $form_close_tag = (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '');
  $form_close_tag =  '</form>';
  $form['#prefix'] = $form_close_tag.'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';
  // Add a class to the form so that we can apply custom style (margin, padding, etc).
  $form['#attributes'] = array('class' => array('addedit-form'));
  $form['#attributes']['autocomplete'] = 'off';
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)
  $form['addedit-right-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-right-col">',
  );

  /* TO DO - Misc buttons need to be redone. There is no nojs fallback anymore for forms, so there would be no Back To Search button
   * $form['action_buttons'] = array(
    '#type' => 'markup',
    '#markup' => empty($miscAction)? '' : theme('addedit-misc-actions', $miscAction),
  );*/

  $form['addedit-right-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );

  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-left-col">',
  );

  // These hidden form fields are needed to be able to communicate to ctools modal to refresh grid on close after a save op
  //addHiddenValueToAddEditForm($form, 'js_object_init_id', 'root-admin');
  //addHiddenValueToAddEditForm($form, 'js_object', 'narrowsearch');

  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('newAddEditFormWithCancelAndSave', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * callback function for saving and closing the form in same click
 */
function addEditSaveAndCloseForm($form, &$form_state){
	try {
		$entityId      = $form_state['entity_id'];
		$entityType    = $form_state['entity_type'];
		$commands = array();
		$commands[] = saveandclosePopup($entityId, $entityType);
		expDebug::dPrint('$commands = ' . print_r($commands, true), 4);
		print ajax_render($commands);
		exit;

	} catch (Exception $ex) {
  	watchdog_exception('addEditSaveAndCloseForm', $ex);
  	expertusErrorThrow($ex);
  }

}
/**
 *  Wrapper for Drupal ajax command saveandclosePopup
 */

function saveandclosePopup($entityId, $entityType) {
	try {
		return array(
      'command' => 'saveandclosePopup',
			'entities' => $entityId.'_'.$entityType,
  );

	} catch (Exception $ex) {
  	watchdog_exception('saveandclosePopup', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditRepaintForm($form, &$form_state) {
	try{
  expDebug::dPrint(' addEditRepaintForm() called.', 5);
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint('  $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
  expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);

  // If a new entity was added, build additional forms
  if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {
    // Create the additional now editable forms if any present
    $formInfoList = $form_state['storage']['form_info_list'];
    $argsList = $form_state['build_info']['args'];
    array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList

    $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
  }

  $commands = array();

  // Build HTML for the custom fields block if triggering element is Add custom or Delete custom button
  // We paint only the custom fields block in this case, and not the full form.
  if (!empty($form_state['triggering_element']['#name']) &&
               ($form_state['triggering_element']['#name'] == 'Add custom' || substr($form_state['triggering_element']['#name'], 0, 13) == 'Delete custom')) {
    $customFieldsWrapperId = $form_state['triggering_element']['#ajax']['custom_fields_wrapper_id'];
    expDebug::dPrint(' $customFieldsWrapperId = ' . print_r($customFieldsWrapperId, true) , 5);

    $customFieldsFormElements = array();
    //$customFieldsFormElementsKeysForDebugging = array();
    $startCapture = false;
    foreach ($form as $elemKey => $elemAttrib) {
      expDebug::dPrint(' $elemKey = ' . print_r($elemKey, true) , 4);
      expDebug::dPrint(' $elemAttrib = ' . print_r($elemAttrib, true), 4);

      if (!$startCapture && $elemKey != 'customfields_wrapper_start') {
        continue;
      }

      $startCapture = true;
      $customFieldsFormElements[] = $form[$elemKey];
      //$customFieldsFormElementsKeysForDebugging[] = $elemKey;

      if ($elemKey == 'customfields_wrapper_end') {
        break; // break out of form; we have got all the fields that we want to repaint
      }
    }
    expDebug::dPrint(' $customFieldsFormElementsKeysForDebugging = ' . print_r($customFieldsFormElementsKeysForDebugging, true) , 4);

    $renderHTML = drupal_render($customFieldsFormElements);
    expDebug::dPrint(' $renderHTML = ' . print_r($renderHTML, true)  , 4);
    $commands[] = ajax_command_replace('#' . $customFieldsWrapperId, $renderHTML);
  }
  else { // repaint the full form
    $commands[] = ajax_command_replace(NULL, drupal_render($form));
  }

  // Render additional forms, if moving from add to edit
  if (!empty($renderedAddlForms)) {
    $commands[] = ajax_command_after(NULL, $renderedAddlForms);
  }

  if(!empty($form_state['input']['attr_action']) && $form_state['input']['attr_action'] == 'add_permissions')
    $divId = '#bubble-face-table   #show_expertus_message' ; 
  else 
    $divId = '#show_expertus_message';
  

    //#custom_attribute_0078975 - If create custom attribute through shortcut  
  if($form_state['storage']['from_shortcut']=='1' && module_exists('exp_sp_administration_customattribute')){
       $divId = '.addedit-customattributes-wrapper #new-customattribute-wrapper #bubble-face-table   #show_expertus_message' ; 
  } 
  
  // Show the status messages, if any.
  // Ensure the status messages are auto removed after a brief while
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  	// Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html($divId, $statusMsgHTML);
  }

  // Resize the modal backdrop, adjust the model top (and left) to optimal
  $commands[] = exp_ctools_modal_adjust();

  // If a new entity was added or an existing entity was edited, inform ctools to refresh narrow search results no modal close.
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
  	$refresh_grid = ((isset($form_state['storage']['edited_entity']) && $form_state['storage']['edited_entity']) ? false : true);
    $commands[] = ajax_command_refresh_narrow_search_results('onclose', $refresh_grid);
  }

  if($form_state['values']['attr_action'] == 'certificate' && $form_state['triggering_element']['#name']=='save'){
     $defaulttCertificate=$form_state['default_certificate_id'];
     $commands[] = ajax_command_invoke(NULL, "validateCertificate",array($defaulttCertificate));
  }
  if (isset($form_state['cutom_fields_wrapper_id'])){
    $customWrapperId = $form_state['cutom_fields_wrapper_id'];
    $customErrorFlag = $form_state['custom_fields_error'];
    $commands[] = ajax_command_invoke(NULL, "validateCustomFields", array($customWrapperId, $customErrorFlag));
  }

  if ($form_state['triggering_element']['#ajax']['add_custom_gray_field'] == true){
    expDebug::dPrint(' $form_state[triggering_element][#ajax][add_custom_gray_field] = ' . $form_state['triggering_element']['#ajax']['add_custom_gray_field'] , 5);
    $commands[] = ajax_command_invoke(NULL, "addCustomGrayConversion", array($form_state['custom_fields']['count']));
  }

  /* For create Location in Class Page*/
  if($form_state['triggering_element']['#ajax']['from_tooltip_location'] == 'Yes'){
    $commands[] = ajax_command_invoke(NULL, "classCreateLocation", array($form_state['storage']['locationId'],$form_state['storage']['locationName'],$form_state['triggering_element']['#ajax']['from_tooltip_location_classId']));
  }
  /*---*/
  //calling scroll function in security role
  $var =$form_state['triggering_element']['#ajax']['wrapper'];
  if(strpos($var,'permissions-addedit-form')!== false){
  	$commands[] = ajax_command_invoke(NULL, "scrollPerm", array($var));
  }
  /*==*/
  //calling scroll function in tags
  $var =$form_state['triggering_element']['#ajax']['wrapper'];
  if(strpos($var,'tag-fieldset-wrapper')!== false){
  	$commands[] = ajax_command_invoke(NULL, "scrollTag", array($var));
  }
  if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && (!$form_state['esign_validation_status'])){
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $drupalUserId  = $form_state['triggering_element']['#ajax']['drupalUserId'];
    $drupalUserId  = $form_state['values'][$drupalUserId];
    $esignObj  = '{"popupDiv":"modalContent","esignFor":"AddAdmin","drupalUserId":"'.$drupalUserId.'","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup'] == true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false; // this is ignored, as form state is not saved after the ajax callback is executed
    }
  }
  //0053694: SF-Survey | Survey code fadeout
  $commands[] = ajax_command_invoke(NULL, "callVtip", array(1));
  $commands[0]['data'] = mb_convert_encoding($commands[0]['data'], "HTML-ENTITIES", "UTF-8");
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditCancelRepaintForm() - This function returns the form to the client for re-rendering.
 */
function addEditCancelRepaintForm($form, &$form_state) {
	try{
  expDebug::dPrint(' addEditCancelRepaintForm() called.');
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
  expDebug::dPrint(' $form_state[custom_fields] = ' . print_r($form_state['custom_fields'], true) , 5);
  $commands = array();
  $commands[] = ajax_command_replace(NULL, drupal_render($form));
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
    // Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  }
  $commands[] = exp_ctools_modal_adjust();

  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditCancelRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave()
 */
function addEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $expandPopup = FALSE, $saveTabIndex = '', $entityId = '',$esignature=false) {
	try {
  $customHiddenButtonId = 'esign-button-'.$wrapperId;
  expDebug::dPrint(' addEditFormWithCancelAndSaveClose() called.');

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);

  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }

  // Add the Cancel button
  $ajaxCallbackCancel = 'addEditCancelRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit cancel buttons
  //$cancelAttributes['class'][] = 'addedit-edit-cancel';

  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  if(empty($entityId)){
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
  }

  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions">',
    //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
    '#value' => t('LBL109'),
    '#name' => 'cancel',
    '#attributes' => $cancelAttributes,
    '#ajax' => array(
      'wrapper' => $wrapperId,
      'callback' => $ajaxCallbackCancel,
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

  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';

  // Using this class to applying bg color to the save button
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  //Increase the width of the popup while clicking on save button
  if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
  $saveAttributes['onclick'][] = 'increasePopWidth();';
  }

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $saveAttributes['data-wrapperid'] = array($wrapperId);

/*	  $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' =>
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' => t('LBL141'),
	    '#name' => 'save',
	    '#attributes' => $saveAttributes,
	    '#ajax' => array(
	      'wrapper' => $wrapperId,
	      'callback' => $ajaxCallbackSave,
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

  	 //$saveAttributes['onclick'] = 'hideMessageInfo();';
  	 $saveAttributes['class'] = array('common-esign-save');//IMPORTANT!!!!!!! do not use this class anywhere else -- only for esign click ------- IMPORTANT!!!!!!!
  	 $form['common-esign-save'] = array(
	      '#type' => 'submit',
	      '#value' => t('LBL141'),
  	      '#name' => 'save',
      	'#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',

  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
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
*/

  if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $esignature){
      $form[$saveButtonId] = array(
    	      '#type' => 'submit',
    	      '#value' => t('LBL141'),
              '#name' => 'save',
    	   	  '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    	       '#suffix' =>
    	               '<div class="admin-save-button-right-bg"></div></div>' .
    	               '<div class="clearBoth"></div></div>' .
    	             '<div class="clearBoth"></div></div>',
      	      '#id' =>$saveButtonId,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'customButtonId'=>$customHiddenButtonId,
    		      'keypress' => true,
    		      'progress' => array(
    		        'type' => 'throbber',
    		        'message' => ''
    		      ),
    	    	)
    	      );
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][]='alert(1);';
      $form[$customHiddenButtonId] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
    	      '#suffix' => '</span>',
    	      '#id' =>$customHiddenButtonId,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
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
  }else{
      //$saveAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' =>
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' => t('LBL141'),
      	'#name' => 'save',
	    '#attributes' => $saveAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'triggerEvent' => 'hidden-save',
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
  }//end if esignature check

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('addEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * newAddEditForm() - Use this function to create your addedit form with a Save button at the top
 */
function newAddEditForm($miscAction = array()) {
  try{
  $form = array();
  $form['#prefix'] = '<div id="addedit-form-wrapper" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';

  // Add a class to the form so that we can apply custom style (margin, padding, etc).
  $form['#attributes'] = array('class' => array('addedit-form-wrapper'));
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)

  $form['addedit-right-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-right-col">',
  );

  // Add Save button
  $wrapper = 'addedit-form-wrapper';
  $ajaxCallback = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-save'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $attributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $attributes['data-wrapperid'] = array($wrapper);


  $form['save'] = array(
    '#type' => 'submit',
    '#prefix' => addEditGetPrefix('ACTION'),
    '#suffix' => addEditGetSuffix('ACTION'),
    '#value' => t('LBL141'),
    '#attributes' => $attributes,
    '#ajax' => array(
      'wrapper' => $wrapper,
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

  $form['action_buttons'] = array(
    '#type' => 'markup',
    '#markup' => empty($miscAction)? '' : theme('addedit-misc-actions', $miscAction),
  );

  $form['addedit-right-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );

  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-left-col">',
  );

  // These hidden form fields are needed to be able to communicate to ctools modal to refresh grid on close after a save op
  //addHiddenValueToAddEditForm($form, 'js_object_init_id', 'root-admin');
  //addHiddenValueToAddEditForm($form, 'js_object', 'narrowsearch');

  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('newAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditFormClose() - Use this function to create your addedit form
 */
function addEditFormClose(&$form) {
	try{
  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('addEditFormClose', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addHiddenValueToAddEditForm - Use this function to add hidden values to the addedit form
 *                               These values are not passed to the client.
 */
function addHiddenValueToAddEditForm(&$form, $id, $defaultValue = '') {
	try{
  $form[$id] = array(
    '#type' => 'value',
    '#value' => $defaultValue
  );
  } catch (Exception $ex) {
  	watchdog_exception('addHiddenValueToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


function addHiddenValueToAddEditFormLoc(&$form, $id, $defaultValue = '') {
	try{
  $form[$id] = array(
    '#type' => 'hidden',
    '#id'   => $id,
    '#default_value' => $defaultValue
  );
  } catch (Exception $ex) {
  	watchdog_exception('addHiddenValueToAddEditFormLoc', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addHiddenFieldToAddEditForm - Use this function to add hidden field to the addedit form
 *                               These values are included in the page HTML, but are not displayed to the user.
 */
function addHiddenFieldToAddEditForm(&$form, $id, $defaultValue = '') {
	try{
  $form[$id] = array(
    '#type' => 'hidden',
    '#default_value' => $defaultValue,
    '#attributes' => array('id'=>$id) // Added by Vincent on 26 Mar, 2012
  );
  } catch (Exception $ex) {
  	watchdog_exception('addHiddenFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addTextFieldToAddEditForm() -  Use this function to add a text field to the addedit form
 */
/*#custom_attribute_0078975*/
function addTextFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 2050, $viewWidthInChars = 60, $description = false,$attrDet = array(),$info_symbol_arr=array()) {
  try{
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
  if($attrDet['class']){
		$attributes['class'][] = $attrDet['class'];
	}
  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }
  if($defaultValue==t('MSG447')){
    $attributes['class'][] = 'narrow-search-filterset-daterange-empty';
  }
//#custom_attribute_0078975
 // Default field text when empty - custom attribute 
  if(trim($attrDet['data-default-text'])!=''){
      $attributes['data-default-text'] = array(t($attrDet['data-default-text']));   
      $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.t($attrDet['data-default-text']).'");';
      $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.t($attrDet['data-default-text']).'");';
     if(trim($defaultValue)==''){
        $defaultValue=t($attrDet['data-default-text']);
        $attributes['class'][] = 'input-field-grey'; 
     }
  } 

  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title, $mandatory,':','','',$info_symbol_arr);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr); 
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr); 
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

  } catch (Exception $ex) {
  	watchdog_exception('addTextFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addTimePickerFieldToAddEditForm() -  Use this function to add a time picker field to the addedit form
 *                                      The javascript behaviour is implemented on the class exp-timepicker-for-addedit-form in
 *                                      file exp_sp_administration_addedit_behaviours.js. You have to add the js file.
 *                                      See report schedules addedit form for how to use.
*/
function addTimePickerFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 60, $description = false) {
 try{
	// Prepare the additional HTML attributes for the textfield
	$attributes = array();
	$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

	// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	// each time the form is re-built.
	$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
	$attributes['class'][] = 'exp-timepicker';
	$attributes['class'][] = 'exp-timepicker-for-addedit-form';

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
	if($defaultValue==t('MSG447')){
		$attributes['class'][] = 'narrow-search-filterset-daterange-empty';
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
		} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
			$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
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
	} catch (Exception $ex) {
		watchdog_exception('addTimePickerFieldToAddEditForm', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addPasswordFieldToAddEditForm() -  Use this function to add a password field to the addedit form
 */
function addPasswordFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 60, $description = false) {
  try{
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
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

  // Create the form element
  $form[$id] = array(
    '#type' => 'password',
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
  } catch (Exception $ex) {
  	watchdog_exception('addPasswordFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addEditGetPrefix() -  Provide HTML for #prefix for the addedit form fields and actions
 */
function addEditGetPrefixLabelEditable($type, $fieldTitle = '', $mandatory = false, $separatorSymbol = ':', $prefixId = '', $editable = false, $labelDetails = '', $loaderWrapper = ''){
try{
  $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";
  $inputField = '';
  if($editable == true){
    $toolTipClass = ' custom-field-label';
    $toolTipTitle = ' title="'.$fieldTitle.'"';
    $inputField = '<div class="addedit-new-field-title" id="custom_input_'.$prefixId.'" style="display:none;" >'.
    	'<input value="'.$fieldTitle.'" class="addedit-edit-custom-label-field form-text" data-default-text="' . t("LBL404") . '" type="text" onblur="updateCustomLabel(this.value, \''.$labelDetails.'\', \''.str_replace('custom_value_', '', $prefixId).'\', \''.$loaderWrapper.'\');" onkeydown="updateCustomLabelKeyDown(event, this.value, \''.$labelDetails.'\', \''.str_replace('custom_value_', '', $prefixId).'\', \''.$loaderWrapper.'\');" /></div>';
    $fieldTitle = titleController('ADMIN-ADD-CUSTOM-LABEL', $fieldTitle,10);
    $labelId = 'custom_label_'.$prefixId;
  }

  switch (strtoupper($type)){
    case 'FIELD':
      return '<div class="addedit-new-field" '.(!empty($prefixId) ? 'id="'.$prefixId.'"' : '').'>' .
               '<div class="addedit-new-field-title'.$toolTipClass.'" '.(!empty($labelId) ? ' id="'.$labelId.'"' : '').'>' .
                 '<span class="custom-label-editable-border">'.$fieldTitle . $separatorSymbol . ($mandatory? $mandatorySymbol : '') .'</span>'.
               '</div>' .
                 $inputField.
               '<div class="addedit-new-field-value">';
      break;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addEditGetPrefixLabelEditable', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addTextFieldToAddEditForm() -  Use this function to add a text field to the addedit form
 */
function addTextFieldToAddEditFormEditable(&$form, $id, $title = '', $defaultValue = '',
                                                 $tabIndex = null, $mandatory = false, $readOnly = false,
                                                     $maxChars = 99999, $viewWidthInChars = 60,
                                                        $description = false, $editable = false,
                                                           $labelDetails = '', $loaderWrapper = '', $attributes = array()) {
  try{
  // Prepare the additional HTML attributes for the textfield
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'][] = 'addedit-edit-' . $id; // This class must be prefixed with 'addedit-edit-' followed by the form field id.

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
  if($defaultValue==t('MSG447')){
    $attributes['class'][] = 'narrow-search-filterset-daterange-empty';
  }
  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    if($editable == true){
      $prefix = addEditGetPrefixLabelEditable('FIELD', $title, $mandatory, ':', $id, $editable, $labelDetails, $loaderWrapper);
    } else {
      $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
    }
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
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
  } catch (Exception $ex) {
  	watchdog_exception('addTextFieldToAddEditFormEditable', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addTextAreaFieldToAddEditForm() -  Use this function to add a text area to the addedit form
 */
/*#custom_attribute_0078975*/
function addTextAreaFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false,$info_symbol_arr=array()) {
 try{
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text area fields.
  $attributes['class'][] = 'addedit-edit-textarea';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Create the form element
  $form[$id] = array(
    '#type' => 'textarea',
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory,':','','',$info_symbol_arr),
    '#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,

  );
  } catch (Exception $ex) {
  	watchdog_exception('addTextAreaFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addTextFormatFieldToAddEditForm() -  Use this function to add a text format to the addedit form
 */
function addTextFormatFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $valueFormat = 'full_html', $tabIndex = null, $mandatory = false, $readOnly = false, $charLimit = 1000) {
 try{
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text format fields.
  $attributes['class'][] = 'addedit-edit-textarea';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textarea';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }
  $attributes['data'] = $id;
  
  $attributes['max-char'] = $charLimit; 

  	//$attributes['onKeyUp'] = 'limitTextareaChars("'.$id.'", '.$charLimit.'); return false;';

  //$charLimitLeft = $charLimit - strlen($defaultValue);
    $charLimitLeft = getEditorWordCount($defaultValue);

  // Create the form element
  $form[$id] = array(
    '#type' => 'text_format',
  	'#name' => $id,
    '#format'=> $valueFormat,
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
    //'#suffix' => '<div class="admin-desc-character-count"><span id="max_char_count_overall">&nbsp;</span></div>'.addEditGetSuffix('FIELD'),
    '#suffix' => '<div  class="admin-desc-character-count catalog-desc-count"><span id="char_count_'.$id.'">'.$charLimitLeft.'</span>'.' ' .t('LBL419').' '.$charLimit.' '.t('LBL420').'.</div>'.addEditGetSuffix('FIELD'),
    //'#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,

  );
  } catch (Exception $ex) {
  	watchdog_exception('addTextFormatFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

	 /**
    * Takes the given HTML data, replaces all its HTML tags with nothing, splits the result by spaces,
    * and outputs the array length i.e. number of words.
    *
    * Note: Duplicated from ckeditor plugin.js, and converted to php and changed the method name as getEditorWordCount
    *
    * @param string htmlData HTML Data
    * @return int Word Count
    */
    function getEditorWordCount($htmlData) {
    	try{
			$text = $htmlData;
			//removes HTML tags
			$text = strip_tags($text);
			// remove all redundant blank symbols
			$text = preg_replace('/\s+/', ' ', $text);
			// remove all blank symbols at the start and at the end
			$text = trim(str_replace("&nbsp;", '', $text));
			$symbols = strlen($text);
			return $symbols;
			} catch (Exception $ex) {
				watchdog_exception('getEditorWordCount', $ex);
				expertusErrorThrow($ex);
			}
    }
/*
 * addTextAreaFieldForShortDescription() -  Use this function to add a text area for short description
 */
/*#custom_attribute_0078975*/
function addTextAreaFieldForShortDescription(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $charLimit = 1000, $defaultText = '',$attrDet = array(),$info_symbol_arr=array(),$pagefrom='') {
 try{
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text area fields.
  $attributes['class'][] = 'addedit-edit-textarea';
  $attributes['class'][] = 'textformat-textarea-editor';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  //$attributes['onKeyDown'] = 'limitTextareaChars('.$id.', '.$charLimit.'); return false;';
  if($pagefrom == 'customattr'){ //If come from custom attribute
      $attributes['onKeyUp'] = 'limitCustomAttributeTextareaChars("'.$id.'", '.$charLimit.'); return false;';  
  }else{
  $attributes['onKeyUp'] = 'limitTextareaChars("'.$id.'", '.$charLimit.'); return false;';
  } 
  
   // Default field text when empty - custom attribute - Added by ganesh for custom attribute - #custom_attribute_0078975
  if(trim($attrDet['data-default-text'])!=''){ 
      $attributes['data-default-text'] = array(t($attrDet['data-default-text']));   
      $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.t($attrDet['data-default-text']).'");';
      $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.t($attrDet['data-default-text']).'");';
     if(trim($defaultValue)==''){
        $defaultValue=t($attrDet['data-default-text']);
        $attributes['class'][] = 'input-field-grey'; 
     }
  }

  //$charLimitLeft = $charLimit - strlen($defaultValue);
    $charLimitLeft = ($defaultValue != $defaultText) ? strlen($defaultValue) : 0 ;
  // Create the form element
  $form[$id] = array(
    '#type' => 'textarea',
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory,':','','',$info_symbol_arr),
    '#suffix' => '<div class="admin-desc-character-count catalog-desc-count"><span id="char_count_'.$id.'">'.$charLimitLeft.'</span>'.' ' .t('LBL419').' '.$charLimit.' '.t('LBL420').'.</div>'.addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,

  );
  } catch (Exception $ex) {
  	watchdog_exception('addTextAreaFieldForShortDescription', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * addACFieldToAddEditForm -  Use this function to add an autocomplete text field to the addedit form
 */
function addACFieldToAddEditForm(&$form, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false) {
 try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';


  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($readOnly) {
      $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  }
  else {
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                          $dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
                              $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                                $paramName . ': ' . $paramValue;

    }

     expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);


    $attributes['onfocus'] = '(function ($) {' .
                                'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
  }

  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#attributes' => $attributes,
  );
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addACFieldToAddEditFormInitViaBehavior -  This function adds an autocomplete text field to the addedit form where
 *                                           the autocomplete plugin is initilized on text field using Drupal Behaviour.
 *                                           This way the autocomplete plugin is initialized once and not each time the field
 *                                           gains focus.
 *
 *                                           Also, by using $acOptionsList argument you can override/specify any autocomplete plugin option.
 *                                           See manager autocomplete field in user admin for an example usage.
 */
function addACFieldToAddEditFormInitViaBehavior(&$form, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null,
                                                  $dependsUponFields = array(), $addlDependencies = array(), $acOptionsList = array(),
                                                     $mandatory = false, $readOnly = false, $preTextValue = '',$maxChars = 256, $viewWidthInChars = 60) {
	try{
  $autocompleteWithId = false;
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';
  if ($viewWidthInChars > $maxChars) {
  	$viewWidthInChars = $maxChars;
  }
  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($readOnly) {
      $attributes['disabled'] = 'disabled';
      $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  }
  else {
  	// Prepare the Javascript object string for autocomplete extraParams option
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                          $dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
                              $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
      $extraParams .= (($extraParams == '')? '' : ', ') . $paramName . ': ' . $paramValue;
    }
    expDebug::dPrint(' $extraParams = ' . print_r($extraParams, true) , 4);

    // Minimum set of autocomplete options
    $acMinOptionsList = array(
      'minChars' => 3,
      'max' => 50,
      'autoFill' => false,
      'mustMatch' => false,
      'matchContains' => false,
      'cacheLength' => 1,
      'delay' => 500,
      'inputClass' => 'ac_input',
      'loadingClass' => 'ac_loading',
      'extraParams' => '{' . $extraParams . '}',
    );

    // Merge any autocomplete options provided by API caller to the minimum set
    $acOptionsList = array_merge($acMinOptionsList, $acOptionsList);
    expDebug::dPrint(' $acOptionsList = ' . print_r($acOptionsList, true) , 4);

    // Prepare the Javascript object string of all autocomplete options
    $acOtionsStr = '{';
    foreach($acOptionsList as $acOptionKey => $acOptionValue) {
      if (is_bool($acOptionValue)) {
        $acOptionValue = $acOptionValue? 'true' : 'false';
      }
      else if (is_string($acOptionValue) && $acOptionKey != 'extraParams' &&
                                   (strlen(trim($acOptionValue)) < 12 || substr(trim($acOptionValue), 0, 8) != 'function')) {
        $acOptionValue = '"' . $acOptionValue . '"';
      }

      $acOtionsStr .= $acOptionKey . ': ' . $acOptionValue . ', ';
      expDebug::dPrint(' built till now $acOtionsStr = ' . print_r($acOtionsStr, true) , 4);
    } // end foreach
    if ($acOtionsStr != '{') { // Remove the ending ', '
      $acOtionsStr = substr($acOtionsStr, 0, -2);
    }
    $acOtionsStr .= '}';
    expDebug::dPrint(' $acOtionsStr = ' . print_r($acOtionsStr, true) , 4);

    // Set arguments for autocomplete plugin as data attributes in the text field.
    // These will be processed by Drupal.behaviors.expAddAutocompleteToField to add autocomplete plugin to this text field
    $attributes['data-exp-acfield-url'] = $acMenuPath;
    $attributes['data-exp-acfield-options'] = $acOtionsStr;
    $attributes['class'][] = 'exp-init-acfield'; // Drupal behavior will look for this class to add autcomplete to the textfield

    // Set any pretext string as a data attributes in the text field.
    // Pretext behavior is implemented in Drupal.behaviors.expAddAutocompleteToField when this is a non-null string
    $attributes['data-exp-acfield-pretext'] = $preTextValue;
  }

  // Determine suffix and prefix
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addACFieldToAddEditFormInitViaBehavior() : Attempt to add a third field to a two column row.');
    }
  }

  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#attributes' => $attributes,
  	'#size' => $viewWidthInChars,
  	'#maxlength' => $maxChars,
  );
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldToAddEditFormInitViaBehavior', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addACFieldToAddEditFormWithId -  Use this function to add an autocomplete text field to the addedit form
 */
function addACFieldToAddEditFormWithId(&$form, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false, $preTextValue='', $locationCapacity = false) {
  try{
	$autocompleteWithId = false;
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';


  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($readOnly) {
	  //Added by Jayalal for tkt no. 0016733
      $attributes['disabled'] = 'disabled';
      $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  }
  else {
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                          $dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
                              $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
      	if($paramName=='id'){ // Added by vincent on 26 Mar, 2012 to handle result id in autocomplete
      		$autocompleteWithId=true;
      	}
      $extraParams .= (($extraParams == '')? '' : ', ') .
                                $paramName . ': ' . $paramValue;

    }
    if($locationCapacity) {
      $extraParams .=  ",capacityId : '.addedit-edit-max_seats' ";
    }

     if($preTextValue == t('LBL114')) {
          $readingTypeClass = 'input-field-grey';
          $defaultValue = $preTextValue;
          $attributes['class'][] = $readingTypeClass; // Add another class
          $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'");';
          $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'");';

     }

     expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);

       if($autocompleteWithId==true){ // Added by vincent on 26 Mar, 2012 to show text in UI when use autocomplete with id
      		$attributes['onfocus'][] = '(function ($) {' .
      		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                  'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                      '{' .
                                                        'minChars: 3, ' .
                                                        'max: 50, ' .
                                                        'autoFill: true, ' .
                                                        'mustMatch: false, ' .
                                                        'matchContains: false, ' .
      													'formatItem:function(pmRec){return pmRec[1]},'.
                                                        'inputClass: "ac_input", ' .
                                                        'loadingClass: "ac_loading", ' .
      		                                            (($locationCapacity) ? 'capacityParams: true,' : '').
                                                        'extraParams: {' . $extraParams . '}' .

                                                       '}' .
                                                    ');' .
                                            '}' .
                                 ')(jQuery);';
       }else{

          $attributes['onfocus'] = '(function ($) {' .
                                'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
       }
  }

  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

     
	 
  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#attributes' => $attributes,
  );
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldToAddEditFormWithId', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addACFieldForMultiSelect -  Use this function to add an autocomplete text field to the addedit form
 */
function addACFieldForMultiSelect(&$form, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false, $preTextValue = '',$attributes = array(), $maxlength= 0) {

 try{
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.

  // If the attributes havie class already
  if(is_array($attributes['class'])) {
    $attributes['class'][] = 'addedit-edit-' . $id; // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  } else {
    $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  }

  //$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';

  global $theme_key;
  if($theme_key == 'expertusoneV2') {
  	$prefix = '<div class="filter-search-start-date-left-bg"></div>';
  	$suffix = '';
  	$attributes['class'][] = 'filter-search-start-date-middle-bg';
  } else {
  	$prefix = '<div>';
  	$suffix = '</div>';
  }


  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }
  $isMultiSelect = 0;
  if ($readOnly) {
      $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  }
  else {
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                          $dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
                              $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
      if($paramName == 'search_type'){
        $isMultiSelect = 1;
      }
      $extraParams .= (($extraParams == '')? '' : ', ') .
                                $paramName . ': ' . $paramValue;

    }

     expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);

     if(!empty($preTextValue)) { // && ($preTextValue == 'Type Course Title' || $preTextValue == 'Type User Name' || $preTextValue == 'Type Survey Name' || $preTextValue == 'Type Assessment Name' || $preTextValue == 'Type Content Code')) {
          $readingTypeClass = 'input-field-grey';
          $defaultValue = $preTextValue;
    			if($id == 'listvalues-autocomplete' && $preTextValue != t('LBL970').' '.t('LBL107')){
						$readingTypeClass = '';
						$defaultValue = t('LBL970').' '.t('LBL107');
					}

          $attributes['class'][] = $readingTypeClass; // Add another class
          $searchTypeHiddenId = '';
          if($isMultiSelect == 1){
            $searchTypeHiddenId = $id .'_hidden';
          }
          $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'","'.$searchTypeHiddenId.'");';
          $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'","'.$searchTypeHiddenId.'");';

     }

    $attributes['onfocus'][] = '(function ($) {' .
                                'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
  }
  //Using the below veriable we can find current loaded theme name
  if($maxlength > 0)
  {
  	$form[$id] = array(
  			'#type' => 'textfield',
  			'#id' => $id,
  			'#prefix' => $prefix,
  			'#suffix' => $suffix,
  			'#title_display' => 'invisible',
  			'#default_value' => $defaultValue,
  			'#attributes' => $attributes,
  			'#maxlength' => $maxlength,
  	);
  }
  else
  {
  	$form[$id] = array(
  			'#type' => 'textfield',
  			'#id' => $id,
  			'#prefix' => $prefix,
  			'#suffix' => $suffix,
  			'#title_display' => 'invisible',
  			'#default_value' => $defaultValue,
  			'#attributes' => $attributes,
  	);
  }
  /*$form[$id.'-addltext-clr'] = array(
    '#type' => 'markup',
    '#markup' => "<span id='datagrid-autocomplete-addltext-clr' class='clr-txt' style='display: none' onclick=\"$('body').data('mulitselectdatagrid').clearACTextBoxFields('".$id."','".$defaultValue."');\">".t('LBL307')."</span>",
  );*/
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldForMultiSelect', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addACFieldSetToAddEditForm -  Use this function to add an autocomplete text fieldset to the addedit form
 */
function addACFieldSetToAddEditForm(&$form, $fieldsetName, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false, $preTextValue, $attributes = array()) {
 try{
    //$attributes = array();
    $autocompleteWithId=false;
    $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $attributes['class'][] = 'addedit-edit-' . $id; // This class must be prefixed with 'addedit-edit-' followed by the form field id.

    // Add a classes to apply common style to all autocomplete fields
    $attributes['class'][] = 'addedit-edit-ac-textfield';
    $attributes['class'][] = 'ac_input';


    if (isset($tabIndex) && $tabIndex > 0) {
      $attributes['tabindex'] = $tabIndex;
    }
    if ($readOnly) {
      $attributes['readonly'] = 'readonly';
      $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
    } else {
      $extraParams = '';
      foreach ($dependsUponFields as $dependancy) {
        $extraParams .= (($extraParams == '')? '' : ', ') .
                            $dependancy .
                              ': function() { return jQuery(".addedit-edit-' .
                                $dependancy . '").val(); }';
      }

      foreach ($addlDependencies as $paramName => $paramValue) {
      	if($paramName=='id'){ // Added by vincent on 26 Mar, 2012 to handle result id in autocomplete
      		$autocompleteWithId=true;
      	}
        $extraParams .= (($extraParams == '')? '' : ', ') .
                                  $paramName . ': ' . $paramValue;
      }

       expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);

       if($preTextValue == t('LBL578') || $preTextValue == 'Enter a User' || $preTextValue == 'Enter Survey Title' || $preTextValue == 'Enter Assessment Title' || $preTextValue == 'Enter Content Title' || $preTextValue == 'Enter Course Title' ) {

            $readingTypeClass = 'input-field-grey';
            $defaultValue = $preTextValue;
            $attributes['class'][] = $readingTypeClass; // Add another class
            $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'");';
            $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'");';

       }
       if($autocompleteWithId==true){ // Added by vincent on 26 Mar, 2012 to show text in UI when use autocomplete with id
      		$attributes['onfocus'][] = '(function ($) {' .
      		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                  'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                      '{' .
                                                        'minChars: 3, ' .
                                                        'max: 50, ' .
                                                        'autoFill: true, ' .
                                                        'mustMatch: false, ' .
                                                        'matchContains: false, ' .
      																									'formatItem:function(pmRec){return pmRec[1]},'.
                                                        'inputClass: "ac_input", ' .
                                                        'loadingClass: "ac_loading", ' .
                                                        'extraParams: {' . $extraParams . '}' .
                                                       '}' .
                                                    ');' .
                                            '}' .
                                 ')(jQuery);';
       }else{
       		$attributes['onfocus'][] = '(function ($) {' .
       		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                  'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                      '{' .
                                                        'minChars: 3, ' .
                                                        'max: 50, ' .
                                                        'autoFill: true, ' .
                                                        'mustMatch: false, ' .
                                                        'matchContains: false, ' .
                                                        'inputClass: "ac_input", ' .
                                                        'loadingClass: "ac_loading", ' .
                                                        'extraParams: {' . $extraParams . '}' .
                                                       '}' .
                                                    ');' .
                                            '}' .
                                 ')(jQuery);';
       }
    }

  $form[$fieldsetName.'_fieldset'][$fieldsetName][$fieldsetName.'_name_'.$id] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="'.$fieldsetName.'-details-row admin-text-field-column"><table cellpadding="0" cellspacing="0" border="0"><tr><td><div id="'.$fieldsetName.'-fieldset-autocomplete-wrapper-'.$id.'">',
        '#suffix' => '</div></td>',
        '#title' => $title,
        '#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#attributes' => $attributes,
  		'#size' => 59,
      );
  if($preTextValue == t('LBL578')) {
      $form[$fieldsetName.'_fieldset'][$fieldsetName][$fieldsetName.'_name_'.$id]['#maxlength']=150;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldSetToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}



/*
 * addACFieldSetAdditionalToAddEditForm -  Use this function to add an autocomplete text fieldset to the addedit form
 */
function addACFieldSetAdditionalToAddEditForm(&$form, $fieldsetName, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false) {
 try{
  $attributes = array();
  $autocompleteWithId=false;
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';
  $attributes['class'][] = 'admin-session-detail-textfield';


  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($readOnly) {
    $attributes['readonly'] = 'readonly';
    $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  } else {
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
                          $dependancy .
                            ': function() { return jQuery(".addedit-edit-' .
                              $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
    	if($paramName=='id'){ // Added by vincent on 26 Mar, 2012 to handle result id in autocomplete
    		$autocompleteWithId=true;
    	}
      $extraParams .= (($extraParams == '')? '' : ', ') .
                                $paramName . ': ' . $paramValue;
    }

     expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);


     if($autocompleteWithId==true){ // Added by vincent on 26 Mar, 2012 to show text in UI when use autocomplete with id
    		$attributes['onfocus'] = '(function ($) {' .
    		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
    																									'formatItem:function(pmRec){return pmRec[1]},'.
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
     }else{
     		$attributes['onfocus'] = '(function ($) {' .
     		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                    '{' .
                                                      'minChars: 3, ' .
                                                      'max: 50, ' .
                                                      'autoFill: true, ' .
                                                      'mustMatch: false, ' .
                                                      'matchContains: false, ' .
                                                      'inputClass: "ac_input", ' .
                                                      'loadingClass: "ac_loading", ' .
                                                      'extraParams: {' . $extraParams . '}' .
                                                     '}' .
                                                  ');' .
                                          '}' .
                               ')(jQuery);';
     }
  }

  $form[$fieldsetName.'_fieldset'][$fieldsetName][$fieldsetName.'_name_'.$id] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><td colspan="2">',
        '#suffix' => '</td></tr>',
        '#title' => $title,
        //'#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#attributes' => $attributes,
  		'#size' => 59,
      );
  } catch (Exception $ex) {
  	watchdog_exception('addACFieldSetAdditionalToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * addDropDownToAddEditForm - Use this function to add a dropdown (select) to the addedit form
 */
/*#custom_attribute_0078975*/
function addDropDownToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array(),$pagefrom ='',$info_symbol_arr=array()) {
 try {
  //Using the below veriable we can find current loaded theme name
  global $theme_key;
  $title_class ='';
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
  
  if($pagefrom == 'exp_meeting' || $pagefrom == 'other_meet'){
  	//$attributes['onclick'][] = 'selectDropdownOnclick(this);';
  	$title_class = $pagefrom;
  	}else if($pagefrom == 'web_ex'){
  		$title_class = 'exp_meeting';
  }

  // Add class to apply common style to all drop-downs
  $attributes['class'][] = 'addedit-edit-dropdown';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-dropdown';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($mandatory) {
    $nullOption[] = t('LBL674');
  } else {
    $nullOption[] = t('LBL674');
  }
  //$finalOptions = array_merge($nullOption, $options);
  if($pagefrom == 'saml' || $pagefrom == 'announcement' || $pagefrom == 'tp' || $pagefrom == 'customattribute') {
  	$finalOptions = $options;
  }
  else {
  	$finalOptions = $nullOption + $options;
  }

  foreach($finalOptions as $key => $value){
  	$finalOptions[$key] = t($value);
  }

  $prefix = '';
  $suffix = '';
  if($pagefrom == 'exp_meeting' || $pagefrom == 'other_meet' || $pagefrom == 'web_ex'){
	  $prefix .= '<td id="'.$title_class.'" >';
	  $suffix .= '</td>';
  }
  
  if($theme_key == 'expertusoneV2') {
  	if($id=='crs_language' || $id=='lang_code')
	  	$newuiPrefix = '<div class="expertus-dropdown-bg crs-lang-dropdown"><div class="expertus-dropdown-icon">';
	  else
	  	$newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
	  $newuiSuffix = '</div></div>';
  } else {
	  $newuiPrefix = '';
	  $newuiSuffix = '';
  }

  if (empty($form['addedit_tmp_storage']['two_column'])) {
  	$prefix .= addEditGetPrefix('FIELD', $title);
  	$suffix .= addEditGetSuffix('FIELD');
  }
  else {
  	if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix .= addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr);
        $suffix .= addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
  		if($pagefrom == 'exp_meeting' || $pagefrom == 'other_meet')
            $prefix .= addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory,":",'',$pagefrom,$info_symbol_arr);
      	else
        $prefix .= addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr); 
        $suffix .= addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }
  
  $form[$id] = array(
    '#type' => 'select',
    '#prefix' => $prefix.$newuiPrefix,
    '#suffix' => $newuiSuffix . $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#options'=> $finalOptions,
    '#default_value' => $defaultOption,
    '#disabled' => $readOnly,
    '#attributes' => $attributes
  );
	if($pagefrom == 'exp_meeting' || $pagefrom == 'web_ex' || $pagefrom == 'other_meet')
		$form[$id]['#validated']=true;
	
  $replacedId = str_replace('_', '-', $id);
	$form['load_select_drop_down_' . $id] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);

	} catch (Exception $ex) {
		watchdog_exception('addDropDownToAddEditForm', $ex);
		expertusErrorThrow($ex);
	}
}

/*

function addLocationDropDownToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array()) {
  //$attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  $attributes['onchange'][] = 'textfieldTitleChange(this);';

  // Add class to apply common style to all drop-downs
  $attributes['class'][] = 'addedit-edit-dropdown';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-dropdown';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($mandatory) {
    $nullOption[] = 'Select';
  } else {
    $nullOption[] = 'Select';
  }

  $addLocationOption[] = 'Add Location';

  //$finalOptions = array_merge($nullOption, $options);
  $finalOptions = $nullOption + $options;
  $finalOptions = array_merge($finalOptions, $addLocationOption);

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
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }
  $form[$id] = array(
    '#type' => 'select',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#options'=> $finalOptions,
    '#default_value' => $defaultOption,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    //'#multiple' => true,
  );

  $replacedId = str_replace('_', '-', $id);
	$form['load_select_drop_down_' . $id] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
}

*/
function getCustomAttributeMultiOptionDropDownMinWidth(&$form,$entity_type,$min_width)
{
	try {	
				if($entity_type=='cre_sys_obt_cls'){ // if class entity
				
					if($form['addedit_tmp_storage']['two_column']['current_column']=='1'){ //if second column
						$min_width='161';
					}
					if($form['addedit_tmp_storage']['two_column']['current_column']=='2')
					{
						$min_width='113';
					}
				}
				if($entity_type=='cre_sys_obt_trp'){ // if class entity
					if($form['addedit_tmp_storage']['two_column']['current_column']=='2')
					{ //if second column
						$min_width='131';
					}
					if($form['addedit_tmp_storage']['two_column']['current_column']=='1')
					{
						$min_width='134';
					}
				}
				if($entity_type=='cre_org'){ // if class entity
					if($form['addedit_tmp_storage']['two_column']['current_column']=='2')
					{ //if second column
						$min_width='110';
					}
					if($form['addedit_tmp_storage']['two_column']['current_column']=='1')
					{ //if second column
						$min_width='150';
					}
				}
				if($entity_type=='cre_sys_obt_cnt'){ // if class entity
					$min_width='153';
				}
				if($entity_type=='cre_sys_obt_loc'){ // if class entity
					if($form['addedit_tmp_storage']['two_column']['current_column']=='1')
					{
						$min_width='146';
					}
				}
				
				return $min_width;
				
		} catch (Exception $ex) {
			watchdog_exception('getCustomAttributeMultiOptionDropDownMinWidth', $ex);
			expertusErrorThrow($ex);
		}
}

/*
 * addMultiselectDropDownToAddEditForm - Use this function to add a multiselect dropdown (select) to the addedit form
 */
/*#custom_attribute_0078975*/
function addMultiselectDropDownToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = array(), $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array(), $customDropdown = array(), $optionLength = 0,$is_manager_validtaion=false,$colspan='',$is_instructor_validtaion=false,$info_symbol_arr=array(),$pagefrom='',$entity_type='') {
  //$attributes = array();
  try {
    global $theme_key;
  $tdclass = !empty($colspan) ? $attributes['class'] : '';
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all drop-downs
  $attributes['class'][] = 'addedit-edit-dropdown';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-dropdown';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($mandatory) {
    $nullOption[] = t('LBL674');
  } else {
    $nullOption[] = t('LBL674');
  }
  //$finalOptions = array_merge($nullOption, $options);
  //$finalOptions = $nullOption + $options;
  $finalOptions = $options;
  foreach($finalOptions as $key => $value){
  	$finalOptions[$key] = t($value);
  }
  if(!empty($colspan)){
  	$class = "admin-addedit-class-".$tdclass;
  	$prefix = '<td class="'.$class.'">';
  	$suffix = '</td>';
  }else{
  	$prefix = '';
  	$suffix = '';
  }
  
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix .= addEditGetPrefix('FIELD', $title, $mandatory,':','','',$info_symbol_arr);
  	$suffix .= addEditGetSuffix('FIELD');
  }
  else {
  	if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix .= addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr);
        $suffix .= addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix .= addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory,':','','',$info_symbol_arr);
        $suffix .= addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }
  //$defaultOption = array('is_instructor', 'is_manager');
  $onchange = '';
  if($id =='roles' && $is_manager_validtaion){
  	$attributes['onchange'] =  array('checkManagerUncheck();');
  }
  if($id =='roles' && $is_instructor_validtaion){
  	$attributes['onchange'] =  array('checkInstructor();');
  }
     $img_cls='';    
    if($pagefrom == 'customattr')
    {       
        if($theme_key == 'expertusoneV2') {
            if($id=='crs_language' || $id=='lang_code')
                $newuiPrefix = '<div class="expertus-dropdown-bg crs-lang-dropdown"><div class="expertus-dropdown-icon">';
            else
                $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
            $newuiSuffix = '</div></div>';
        }  
        $img_cls='class="mul-opt-dd-img"';          
    }
    else {          
        $newuiPrefix = '';
        $newuiSuffix = '';
    }
  //change by ayyappans for 33432: Issue in Users
  if(empty($customDropdown)) {
  	$form[$id] = array(
  			'#type' => 'select',
  			'#multiple' => TRUE,
                '#prefix' => $prefix.$newuiPrefix,
                '#suffix' => $newuiSuffix . $suffix,
  			'#title' => $title,
  			'#title_display' => 'invisible',
  			'#options'=> $finalOptions,
  			'#default_value' => $defaultOption,
  			'#disabled' => $readOnly,
  			'#attributes' => $attributes
  	);
  	$multiselectName = 'edit-'.str_replace('_', '-', $id);
	$controlTitle = $optionLength != 0 ? 'restrictMultiselectTitle(\''.$multiselectName.'\', \'admin-multiselect-dropdown\')' : '';
	//$controlTitle = $optionLength != 0 ? 'restrictMultiselectTitle(\''.$multiselectName.'\', '.$optionLength.')' : '';
     
    $min_width='146'; //As default - Custom Attribute 
    expDebug::dPrint('$entity_type='.$entity_type,5); 
    if($entity_type!='' && $pagefrom == 'customattr')
    {
       $min_width=getCustomAttributeMultiOptionDropDownMinWidth($form,$entity_type,$min_width);
    } 

  	$form['load_multiselect_'.$id] = array(
    		'#prefix' => '<img '.$img_cls.'style="display:none" src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="$(\'select[id^='.$multiselectName.']\').multiselect({ selectedList: 0, uncheckAllText: true, minWidth: '.$min_width.', height: -1, entityname: \''.$entity_type.'\', pagefrom: \''.$pagefrom.'\', noneSelectedText: \''.t('LBL674').'\', classes: \''.$multiselectName.'\' }).multiselectfilter();'.$controlTitle.'" />',   		
  	);
  } else {
  		$selectedCount = count($defaultOption);
  		expDebug::dPrint('count of'.$selectedCount, 4);
  		$selectedText = $selectedCount > 0 && is_array($defaultOption) ? $selectedCount.' selected' : t('LBL674');
  		$prefix .= '<div id="menu_'.$id.'" onClick="$(\'#root-admin\').data(\'peopleuser\').callMultiSelect(\''.$customDropdown['user_id'].'\', \'' . $id . '\');" class="ui-multiselect multiselect-custom-dropdown"><span class="dropdown-arrow ui-icon ui-icon-triangle-2-n-s"></span><span class="selected-list-text">' . $selectedText . '</span>';
  		$prefix .= '</div><div id="avil_' . $id .'" class="multiselect-custom-dropdown-results" style="display:none"></div>';
  		$prefix .= '<img style="display:none" src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="$(\'#root-admin\').data(\'peopleuser\').updateSelectedText(\''.$id.'\');"/>';	//change by ayyappans for 41196: Can't able to save the page,system gets hang for a while
  		 
  		$form['load_multiselect_'.$id] = array(
  				'#type' => 'hidden',
  				'#prefix' => $prefix,
  				'#suffix' => $suffix,
  				'#default_value' => implode(',', $defaultOption),
  				'#attributes' => array('id' => 'load_multiselect_'.$id)
  		);
  	
  }
  } catch (Exception $ex) {
  	watchdog_exception('addMultiselectDropDownToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addRadioSetToAddEditForm -  Use this function to add a radio buttons set to the addedit form
 */
function addRadioSetToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false) {
 try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all radio set fields
  $attributes['class'][] = 'addedit-edit-radioset';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-radios';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
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
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }

  $form[$id] = array (
  '#type' => 'radios',
  '#prefix' => $prefix,//addEditGetPrefix('FIELD', $title, $mandatory, '?'),
  '#suffix' => $suffix,//addEditGetSuffix('FIELD'),
  '#title' => $title,
  '#title_display' => 'invisible',
  '#default_value' =>  $defaultValue,
  '#options' => $options,
  '#disabled' => $readOnly,
  '#attributes' => $attributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addRadioSetToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addRadioToFieldSetAddEditForm -  Use this function to add a radio buttons set to the addedit form inside fieldset
 */
function addRadioToFieldSetAddEditForm(&$form, $fieldsetName, $id, $title = '', $options = array(), $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false) {
try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all radio set fields
  $attributes['class'][] = 'addedit-edit-radioset';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-radios';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }
  $form[$fieldsetName.'_fieldset'][$fieldsetName][$fieldsetName.'_radioname_'.$id] = array (
  '#type' => 'radios',
  '#prefix' => '<td class="admin-page-radio-button">',
  //'#suffix' => '</td>',
  '#title' => $title,
  '#title_display' => 'invisible',
  '#default_value' =>  $defaultValue,
  '#options' => $options,
  '#disabled' => $readOnly,
  '#attributes' => $attributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addRadioToFieldSetAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addCheckboxToAddEditForm -  Use this function to add a checkbox to the addedit form
 */
function addCheckBoxToAddEditForm(&$form, $id, $title = '', $titlePrefixed = true, $defaultValue = 0, $tabIndex = null, $mandatory = false, $readOnly = false) {
 try{
  $attributes = array();

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all checkbox fields
  $attributes['class'][] = 'addedit-edit-checkbox';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-checkbox';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
	  if ($titlePrefixed == true) {
	    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
	    $suffix = addEditGetSuffix('FIELD');
	  }
	  else {
	    $prefix = addEditGetPrefix('CHECKBOX_TITLE_SUFFIXED');
	    $suffix = addEditGetSuffix('CHECKBOX_TITLE_SUFFIXED', $title, $mandatory);
	  }
  }
  else {
  	if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
	    if ($titlePrefixed == true) {
	      $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
	      $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
	    }
	    else {
	      $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_CHECKBOX_TITLE_SUFFIXED');
	      $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_CHECKBOX_TITLE_SUFFIXED', $title, $mandatory);
	    }
	    $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
  	  if ($titlePrefixed == true) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
      }
      else {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_CHECKBOX_TITLE_SUFFIXED');
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_CHECKBOX_TITLE_SUFFIXED', $title, $mandatory);
      }
      $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	}
  	else {
  		throw new Exception('exp_sp_administration.inc : addCheckBoxToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }
  $form[$id] = array (
    '#type' => 'checkbox',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' =>  $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addCheckBoxToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addFileUploadControlToAddEditForm() - Adds a file upload field to addedit form
 */
function addFileUploadControlToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 61) {
  // Prepare the additional HTML attributes for the textfield
  try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all file upload fields
  $attributes['class'][] = 'addedit-edit-file-upload';

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

  $form[$id] = array(
    '#type' => 'file',
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
    '#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars
  );
  } catch (Exception $ex) {
  	watchdog_exception('addFileUploadControlToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addDateFieldToAddEditForm() -  Use this function to add a date field to the addedit form
 */
/*#custom_attribute_0078975*/
function addDateFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $emptyDateFieldText = 'mm-dd-yyyy', $datePickerDatesDisplayOption = 'all', $datePickerTooltip = '', $tabIndex = null,
                                           $mandatory = false, $readOnly = false, $viewWidthInChars = 10, $maxChars = 40,$info_symbol_arr=array()) {

  // Prepare the additional HTML attributes for the textfield
  try{
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all file upload fields
  $attributes['class'][] = 'addedit-edit-datefield';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-datefield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Set the data required by Drupal.behaviors.addDatePickerToAddEditDateField to attach datepicker, etc.
  $attributes['data-datePickerDatesDisplayOption'] = $datePickerDatesDisplayOption;
  $attributes['data-emptyDateFieldText'] = $emptyDateFieldText;
  $attributes['data-datePickerTooltip'] = $datePickerTooltip;

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('DATEFIELD', $title, $mandatory,':','','',$info_symbol_arr);
    $suffix = addEditGetSuffix('DATEFIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_DATEFIELD', $title, $mandatory,':','','',$info_symbol_arr);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_DATEFIELD', $title, $mandatory,':','','',$info_symbol_arr);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addDateFieldToAddEditForm() : Attempt to add a third field to a two column row.');
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
    '#maxlength' => $maxChars
  );
  } catch (Exception $ex) {
  	watchdog_exception('addDateFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addMultiSelectFieldToAddEditForm -  Use this function to add a multiselect to the addedit form
 */
function addMultiSelectFieldToAddEditForm(&$form, $id, $title = '', $options = array(), $preSelectedOptions = array(), $tabIndex = null, $viewAreaSize = 10, $mandatory = false) {
 try{
  $attributes = array();

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all multiselect fields
  $attributes['class'][] = 'addedit-edit-multiselect';

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Add a 'none_selected' option to $options. Label for this would be &nbsp
  if (!isset($options['none_selected'])) {
  	$options['none_selected'] = '&nbsp';
  }

  // Add the 'none_selected' option to the $preSelectedOptions as the last option.

 if (!isset($preSelectedOptions['none_selected'])) {
   $preSelectedOptions[] = 'none_selected';
  }

  $form[$id] = array (
    '#type' => 'multiselect',
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory, ':', 'addedit-wrapper-multiselect-' . $id),
    '#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' =>  $preSelectedOptions,
    '#options' => $options,
    '#required' => FALSE,
    '#multiple' => TRUE,
    '#size' => $viewAreaSize,
    '#attributes' => $attributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addMultiSelectFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addAjaxDependantDropDownPairToAddEditForm_callback() - Ajax callback of the affector dropdown in addAjaxDependantDropDownPairToAddEditForm()
 */
function addAjaxDependantDropDownPairToAddEditForm_callback($form, $form_state) {
	try{
  return $form[$form_state['storage']['affected_dropdown']];
  } catch (Exception $ex) {
  	watchdog_exception('addAjaxDependantDropDownPairToAddEditForm_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addAjaxDependantDropDownPairToAddEditForm - Use this function to add an ajax enabled dropdown (select) pair to the addedit form. The user selection in
 *                                             the first dropdown (affector) decides the options displayed in the second dropdown (affected).
 */
function addAjaxDependantDropDownPairToAddEditForm(&$form, $affector = array(), $affected = array()) {
	try{
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
  $affectorAttributes['data-wrapperid'] = array($affected['wrapper_id']);


  if (isset($affector['tab_index']) && $affector['tab_index'] > 0) {
    $affectorAttributes['tabindex'] = $affector['tab_index'];
  }

  $affectorOptions = $affector['fetch_options']();
  if ($affector['mandatory']) {
    $nullOption[''] = t('LBL674');
  } else {
    $nullOption[''] = t('LBL674');
  }

  //$finalAffectorOptions = array_merge($nullOption, $affectorOptions);
  $finalAffectorOptions = $nullOption + $affectorOptions;
  expDebug::dPrint(' $$affectorOptions = ' . print_r($affectorOptions, true) , 4);
  expDebug::dPrint(' $$finalAffectorOptions = ' . print_r($finalAffectorOptions, true) , 4);

  $form[$affector['id']] = array(
    '#type' => 'select',
    '#prefix' => addEditGetPrefix('FIELD', $affector['title'], $affector['mandatory']),
    '#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $affector['title'],
    '#title_display' => 'invisible',
    '#options'=> $finalAffectorOptions,
    '#default_value' => $affector['default_value'],
    '#attributes' => $affectorAttributes,
    '#ajax' => array(
      'wrapper' => $affected['wrapper_id'],
      'callback' => addAjaxDependantDropDownPairToAddEditForm_callback,
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

  $affectedOptions = $affected['fetch_options']($affector['default_value']);

  if (empty($affector['default_value'])) {
    //$nullOption[''] = t('Select' . ' ' . $affector['title']);
    $nullOption[''] = t('LBL674');
  }
  else if ($affected['mandatory']) {
    $nullOption[''] = t('LBL674');
  }
  else if (empty($affectedOptions)) {
    $nullOption[''] = t('No' . ' ' . $affected['title']);
  }
  else {
    $nullOption[''] = t('LBL674');
  }
  //$finalAffectedOptions = array_merge($nullOption, $affectedOptions);
  $finalAffectedOptions = $nullOption + $affectedOptions;
  expDebug::dPrint(' $finalAffectedOptions = ' . print_r($finalAffectedOptions, true), 4);

  $form[$affected['id']] = array(
    '#type' => 'select',
    '#prefix' => addEditGetPrefix('FIELD', $affected['title'], $affected['mandatory'], ':', $affected['wrapper_id']),
    '#suffix' => addEditGetSuffix('FIELD'),
    '#title' => $affected['title'],
    '#title_display' => 'invisible',
    '#options' => $finalAffectedOptions,
    '#default_value' => $affected['default_value'],
    '#attributes' => $affectedAttributes,
/*    '#states' => array(
      // Disable this field when nothing is selected in the country option.
      'disabled' => array(':input[name="' . $affector['id'] . '"]' => array('value' => ''))
    ), */
  );
  } catch (Exception $ex) {
  	watchdog_exception('addAjaxDependantDropDownPairToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * clearAffectedFieldValueAndRebuildAddEditForm() -
 */
function clearAffectedFieldValueAndRebuildAddEditForm($form, &$form_state) {
	try{
  expDebug::dPrint(' In clearAffectedFieldValueAndRebuildAddEditForm()');
	$affectedFieldId = $form_state['triggering_element']['#ajax']['affected_id'];
  unset($form_state['values'][$affectedFieldId]);
  unset($form_state['input'][$affectedFieldId]);
  $form_state['storage']['affected_dropdown'] = $affectedFieldId;
  } catch (Exception $ex) {
  	watchdog_exception('clearAffectedFieldValueAndRebuildAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addAffectorAjaxDownToAddEditForm() -
 */
function addAffectorDropDownToAddEditForm(&$form, $affector = array()) {
	try{
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
  $affectorAttributes['data-wrapperid'] = array($affector['affected_wrapper_id']);

  if (isset($affector['tab_index']) && $affector['tab_index'] > 0) {
    $affectorAttributes['tabindex'] = $affector['tab_index'];
  }

  $affectorOptions = $affector['fetch_options']();
  if ($affector['mandatory']) {
    $nullOption[''] = t('LBL674');
  } else {
    $nullOption[''] = 'None Selected';
  }

  //$finalAffectorOptions = array_merge($nullOption, $affectorOptions);
  $finalAffectorOptions = $nullOption + $affectorOptions;
  expDebug::dPrint(' $affectorOptions = ' . print_r($affectorOptions, true) ,4);
  expDebug::dPrint(' $finalAffectorOptions = ' . print_r($finalAffectorOptions, true) , 4);

  $prefix = '';
  $suffix = '';
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

  $form[$affector['id']] = array(
    '#type' => 'select',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $affector['title'],
    '#title_display' => 'invisible',
    '#options'=> $finalAffectorOptions,
    '#default_value' => $affector['default_value'],
    '#attributes' => $affectorAttributes,
    '#validate' => array('clearAffectedFieldValueAndRebuildAddEditForm'),
    '#ajax' => array(
      'wrapper' => $affector['affected_wrapper_id'],
      'callback' => addAjaxDependantDropDownPairToAddEditForm_callback,
      'affected_id' => $affector['affected_id'],
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
  } catch (Exception $ex) {
  	watchdog_exception('addAffectorDropDownToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addAffectedDropDownToAddEditForm() -
 */
function addAffectedDropDownToAddEditForm(&$form, $affected = array()) {
	try{
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
  if (empty($affected['affector_default_value'])) {
    //$nullOption[''] = t('Select' . ' ' . $affected['affector_title']);
    $nullOption[''] = t('LBL674');
    $readOnly = true;
  }
  else if ($affected['mandatory']) {
    $nullOption[''] = t('LBL674');
  }
  else if (empty($affectedOptions)) {
    $nullOption[''] = t('No' . ' ' . $affected['title']);
    $readOnly = true;
  }
  else {
    $nullOption[''] = t('LBL674');
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

  $form[$affected['id']] = array(
    '#type' => 'select',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $affected['title'],
    '#title_display' => 'invisible',
    '#options' => $finalAffectedOptions,
    '#default_value' => $affected['default_value'],
    '#attributes' => $affectedAttributes,
    '#disabled' => $readOnly,
  );
  } catch (Exception $ex) {
  	watchdog_exception('addAffectedDropDownToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addAjaxDependantDropDownPairToUserFormState - Use this function to add an ajax enabled dropdown (select) pair to the addedit form. The user selection in
 *                                             the first dropdown (affector) decides the options displayed in the second dropdown (affected).
 */
function addAjaxDependantDropDownPairToUserFormState(&$form, $affector = array(), $affected = array()) {
	try{
  global $theme_key;
  $affectedAttributes = array();
  $affectedAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $affectedAttributes['class'] = array('addedit-edit-' . $affected['id']); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  // Add classes for adding style
  $affectedAttributes['class'][] = 'addedit-edit-dropdown';
  $affectedAttributes['class'][] = 'addedit-dependant-dropdown-affected';
  $affectedAttributes['onchange'][] = 'textfieldTitleChange(this)';

  if (isset($affected['tab_index']) && $affected['tab_index'] > 0) {
    $affectedAttributes['tabindex'] = $affected['tab_index'];
  }

  $affectedOptions = $affected['fetch_options']($affector['default_value']);

  if (empty($affector['default_value'])) {
    //$nullOption[''] = t('Select' . ' ' . $affector['title']);
    $nullOption[''] = t('LBL674');
  }
  else if (($affected['id'] == 'state' && $affected['mandatory'] == 1) && (empty($affectedOptions))) {
    $nullOption[''] = t('No'). ' ' . $affected['title'];
  }
  else if ($affected['mandatory']) {
    $nullOption[''] = t('LBL674');
  }
  else if (empty($affectedOptions)) {
    $nullOption[''] = t('No') . ' ' . $affected['title'];
  }
  else {
    $nullOption[''] = t('LBL674');
  }

  //$finalAffectedOptions = array_merge($nullOption, $affectedOptions);
  $finalAffectedOptions = $nullOption + $affectedOptions;
  expDebug::dPrint(' $finalAffectedOptions = ' . print_r($finalAffectedOptions, true) , 4);

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
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }
  if($theme_key == 'expertusoneV2') {
	  $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
	  $newuiSuffix = '</div></div>';
  } else {
	  $newuiPrefix = '';
	  $newuiSuffix = '';
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
/*    '#states' => array(
      // Disable this field when nothing is selected in the country option.
      'disabled' => array(':input[name="' . $affector['id'] . '"]' => array('value' => ''))
    ), */
  );
  $affectId = $affected['id'];
  $replacedId = str_replace('_', '-', $affectId);
	$form['load_select_drop_down_' . $affectId] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
	} catch (Exception $ex) {
		watchdog_exception('addAjaxDependantDropDownPairToUserFormState', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addAjaxDependantDropDownPairToUserFormCountry - Use this function to add an ajax enabled dropdown (select) pair to the addedit form. The user selection in
 *                                             the first dropdown (affector) decides the options displayed in the second dropdown (affected).
 */
function addAjaxDependantDropDownPairToUserFormCountry(&$form, $affector = array(), $affected = array(), $wrapperId = '') {
	try {
  global $theme_key;
  $affectorAttributes = array();
  $affectorAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $affectorAttributes['class'] = array('addedit-edit-' . $affector['id']); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  // Add classes for adding style
  $affectorAttributes['class'][] = 'addedit-edit-dropdown';
  $affectorAttributes['class'][] = 'addedit-dependant-dropdown-affector';
  $affectorAttributes['onchange'][] = 'textfieldTitleChange(this)';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $affectorAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag

  $wrapperId = empty($wrapperId) ? 'user-basic-addedit-form' : $wrapperId;
  $affectorAttributes['data-wrapperid'] = array($wrapperId);


  if (isset($affector['tab_index']) && $affector['tab_index'] > 0) {
    $affectorAttributes['tabindex'] = $affector['tab_index'];
  }

  $affectorOptions = $affector['fetch_options']();
  if ($affector['mandatory']) {
    $nullOption[''] = t('LBL674');
  } else {
    $nullOption[''] = t('LBL674');
  }

  //$finalAffectorOptions = array_merge($nullOption, $affectorOptions);
  $finalAffectorOptions = $nullOption + $affectorOptions;
  expDebug::dPrint(' $$affectorOptions = ' . print_r($affectorOptions, true) , 4);
  expDebug::dPrint(' $$finalAffectorOptions = ' . print_r($finalAffectorOptions, true) , 4);

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
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

  if($theme_key == 'expertusoneV2') {
	  $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
	  $newuiSuffix = '</div></div>';
  } else {
	  $newuiPrefix = '';
	  $newuiSuffix = '';
  }

  $form[$affector['id']] = array(
    '#type' => 'select',
    '#prefix' => $prefix . $newuiPrefix,
    '#suffix' => $newuiSuffix . $suffix,
    '#title' => $affector['title'],
    '#title_display' => 'invisible',
    '#options'=> $finalAffectorOptions,
    '#default_value' => $affector['default_value'],
    '#attributes' => $affectorAttributes,
    '#ajax' => array(
      'wrapper' => $affected['wrapper_id'],
      'callback' => addAjaxDependantDropDownPairToAddEditForm_callback,
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
  $affectId = $affector['id'];
  $replacedId = str_replace('_', '-', $affectId);
	$form['load_select_drop_down_' . $affectId] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
	} catch (Exception $ex) {
		watchdog_exception('addAjaxDependantDropDownPairToUserFormCountry', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * function addTwoColumnRowToAddEditForm() -
 */
function addTwoColumnRowToAddEditForm(&$form, $id = '') {
	try{
	if (empty($id)) {
		$id = 'twocolrow';
	}
	$form['addedit_tmp_storage']['two_column']['id'] = $id;
	$form['addedit_tmp_storage']['two_column']['current_column'] = 0;

	$form[$id] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-new-field addedit-new-field-two-col" id ="two-col-row-' . $id . '">',
  );
	} catch (Exception $ex) {
		watchdog_exception('addTwoColumnRowToAddEditForm', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * function endTwoColumnAddEditForm() -
 */
function endAddEditTwoColumnRow(&$form) {
	try{
	$id = $form['addedit_tmp_storage']['two_column']['id'];
  $form[$id . '-end'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="clearBoth"></div>' .
               '</div>',
  );

  // Remove the two column element from $form['addedit_tmp_storage']
  unset($form['addedit_tmp_storage']['two_column']);
  } catch (Exception $ex) {
  	watchdog_exception('endAddEditTwoColumnRow', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * function addSingleColumnRowToAddEditForm() - Added for custom attribute by ganesh
 */
/*#custom_attribute_0078975*/
function addSingleColumnRowToAddEditForm(&$form, $id = '') {
    try{
    if (empty($id)) {
        $id = 'singlecolrow';
    }
    $form['addedit_tmp_storage']['single_column']['id'] = $id;
    $form['addedit_tmp_storage']['single_column']['current_column'] = 0;

    $form[$id] = array(
    '#type' => 'markup',
    '#markup' => '<div class="addedit-new-field addedit-new-field-single-col" id ="single-col-row-' . $id . '">',
  );
    } catch (Exception $ex) {
        watchdog_exception('addSingleColumnRowToAddEditForm', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * function endAddEditSingleColumnRow() - Added for custom attribute by ganesh
 */
/*#custom_attribute_0078975*/
function endAddEditSingleColumnRow(&$form) {
    try{
    $id = $form['addedit_tmp_storage']['single_column']['id'];
      $form[$id . '-end'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="clearBoth"></div>' .
                   '</div>',
      );

      // Remove the two column element from $form['addedit_tmp_storage']
      unset($form['addedit_tmp_storage']['single_column']);
      
  } catch (Exception $ex) {
    watchdog_exception('endAddEditSingleColumnRow', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addFieldSetToAddEditForm - Use this function to add a FieldSet to the form
 */
function addFieldSetToAddEditForm(&$form, $fieldsetId, $title = '',$collapsible = FALSE ,$collapsed = FALSE) {
	try{
  $form[$fieldsetId] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => $collapsible,
    '#collapsed' => $collapsed
  );
  } catch (Exception $ex) {
  	watchdog_exception('addFieldSetToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * addFieldSetToAddEditForm - Use this function to add a fieldsetwrapper <div> to MRO Control and Catalog Access in courses and classes
 */
function addFieldSetToAddEditFormMROCatalog(&$form, $fieldsetId, $title = '',$collapsible = FALSE ,$collapsed = FALSE) {
	try{
  $form[$fieldsetId] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => $collapsible,
    '#collapsed' => $collapsed,
    '#prefix'=> '<div class="fieldsetwrapper">',
   '#suffix'=> '</div>'
  );
  } catch (Exception $ex) {
  	watchdog_exception('addFieldSetToAddEditFormMROCatalog', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditCheckMandatoryFields() - Flag an error message if value is missing in the mandatory fields.
 */
function addEditCheckMandatoryFields($form, &$form_state, $mandatoryFields = array()) {
  try{
  $form_state["storage"]["form_validate"] = true; /* to retain form if there is any error*/
  foreach ($mandatoryFields as $fieldName) {
    $fieldValue = trim($form_state['values'][$fieldName]);
    if (empty($fieldValue)) {
      expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' is missing.' , 5);
      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101'));
      $form_state["storage"]["form_validate"] = false;
      $form_state['esign_show_popup']= false;
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('addEditCheckMandatoryFields', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * trimAddEditTextFields() - trim addedit form text fields
 */
function trimAddEditTextFields(&$form_state, $textFieldsList = array()) {
	try{
  foreach ($textFieldsList as $textFieldId) {
    $form_state['values'][$textFieldId] = trim($form_state['values'][$textFieldId]);
    expDebug::dPrint(' Trimmed ' . $textFieldId . '. New value = \'' . $form_state['values'][$textFieldId] . '\'.' , 5);
  }
  } catch (Exception $ex) {
  	watchdog_exception('trimAddEditTextFields', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getDefaultAddEditFormFieldValue() - The function determines the default value for a form field when field id is same as field name in the DB table
 *                                     (DB record is available in $details).
 */
function getDefaultAddEditFormFieldValue($field, $form_state, $dbDetail) {
 try {
  $defaultValue = empty($form_state['values'][$field])? (empty($dbDetail[$field])? '' : $dbDetail[$field]) :
                                                        $form_state['values'][$field];
  expDebug::dPrint(' $defaultValue for field ' .
                                                $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 *
 */
function getLocationName($locationId) {
	try{
  if (empty($locationId)) {
    return '';
  }

  // Select slt_location table
  $select = db_select('slt_location', 'loc');

  // Get org id
  $select->addField('loc', 'name', 'name');

  // Set the conditions
  $select->condition('loc.id', $locationId, '=');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execure the query
  $result = $select->execute()->fetch();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  if (empty($result)) {
    return ''; // Does not exist
  }

  return $result->name;
  } catch (Exception $ex) {
  	watchdog_exception('getLocationName', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchSliderConditionWithNull
 */

function selectAddNarrowSearchSliderConditionWithNull($selectStmt, $tableAlias, $tableField, $filtersetCode){
	try{
  $valuefilters= getURLParam($filtersetCode);
  expDebug::dPrint(' $valuefilters = ' . print_r($valuefilters, true) , 4);
  if (!empty($valuefilters)){
    $valueRange = explode("|", $valuefilters);
    if($valueRange[0] == 0){
	  $orQuery = db_or();
	  $orQuery->isNull($tableAlias . '.' . $tableField);
      if (!empty($valueRange[1])) {
	    $orQuery->condition($tableAlias . '.' . $tableField, $valueRange [1], '<=');
	  }
	  $selectStmt->condition($orQuery);
    }
    else{
	  if (!empty($valueRange[0])) {
	    $selectStmt->condition($tableAlias . '.' . $tableField, $valueRange [0], '>=');
	  }
	  if (!empty($valueRange[1])) {
	    $selectStmt->condition($tableAlias . '.' . $tableField, $valueRange [1], '<=');
	  }
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchSliderConditionWithNull', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAllCourseAutoComplete() {
    try{
        $searchText =  $_GET['z'];
        $excludedCourseId = $_GET['excluded_course_id'];
        $idRequired = isset($_GET['id_required']) ? $_GET['id_required'] : 1;
        $search_type =  $_GET['search_type'];
        $delivery_mode = $_GET['delivery_mode'];

        // Solr implementation for attach course auto complete
        $srcSource = getConfigValue('solr_search');
        if($srcSource == 0){
            // Select the table
            if($search_type == 'name' || empty($search_type)){
                $select = db_select('slt_course_template', 'course');
                // Add fields to fetchgetTpCourseAutoComplete
                $select->addField('course', 'id', 'id');
                $select->addField('course', 'title', 'title');
                $select->addField('course', 'code', 'code');

                // Add condition
                //$select->condition('course.title', '%' . db_like($searchText) . '%', 'LIKE');
                $select->condition('course.status', 'lrn_crs_sts_atv','=');
                $select->condition(db_or()
  	                     ->condition('course.title', '%' . db_like($searchText) . '%', 'LIKE')
  	                     ->condition('course.code', '%' . db_like($searchText) . '%', 'LIKE')

                );

                if (!empty($excludedCourseId)) {
	                $excludedCourseId = urldecode($excludedCourseId);
	                $excludedCourseIdArr = explode(',', $excludedCourseId);
	  				$select->condition('course.id', $excludedCourseIdArr, 'NOT IN');
  				}
  				filterByUserAccessPrivileges($select,'course','',1,'cre_sys_obt_crs','attach','course');
  				// Order in dictionary sort order
  				$select->orderBy('title');
                $matchingCourseNames = $select->execute()->fetchAll();
                expDebug::dPrint(' $matchingCourseNames = '. print_r($matchingCourseNames, true) , 3);

         	}else if($search_type == 'tags') {
  				getCourseTagnameAutoComplete();
        	}
        }
        else {
      	  require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
     	  try {
              $solrObj = new CatalogSolrSearch();
              $search_results = $solrObj->attachCourseAutoComplete($excludedCourseId);
              $matchingCourseNames = $search_results['records'];
          	  $count = $search_results['recCount'];       
          }catch(Exception $e){
        	expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
         }
        }

        foreach ($matchingCourseNames as $courseName) {
        expDebug::dPrint("Came out of loop ");
        if($idRequired == 0){
        print $courseName->title . "\n";
        } else {
          print $courseName->id."|".$courseName->title .  ' (' . $courseName->code . ')'  . "\n";
      }
  }
  }
   catch (Exception $ex) {
  	watchdog_exception('getAllCourseAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAllCourseTpAutoComplete(){
 try{
 $searchText =  $_GET['z'];
  $excludedCourseId = $_GET['excluded_course_id'];
  $idRequired = isset($_GET['id_required']) ? $_GET['id_required'] : 1;
  $delivery_mode = $_GET['delivery_mode'];
  $search_type =  $_GET['search_type'];
  if($search_type == 'name'){
  // Select the table
  $select = db_select('slt_course_template', 'course');
  // Add fields to fetchgetTpCourseAutoComplete
  $select->addField('course', 'id', 'id');
  $select->addField('course', 'title', 'title');
  $select->addField('course', 'code', 'code');
  $select->addField('course', 'status', 'status');
  // Add condition
  //$select->condition('course.title', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('course.status', 'lrn_crs_sts_atv', '=');
  $select->condition(db_or()
  	                     ->condition('course.title', '%' . db_like($searchText) . '%', 'LIKE')
  	                     ->condition('course.code', '%' . db_like($searchText) . '%', 'LIKE')

  	                  );

  if (!empty($excludedCourseId)) {
    $excludedCourseId = urldecode($excludedCourseId);
    $excludedCourseIdArr = explode(',', $excludedCourseId);
  	$select->condition('course.id', $excludedCourseIdArr, 'NOT IN');
  }
  filterByUserAccessPrivileges($select,'course','',1,'cre_sys_obt_crs','attach','course');  
  // Order in dictionary sort order
 // $select->orderBy('title');

   $select1 = db_select('slt_program', 'program');
  // Add fields to fetchgetTpCourseAutoComplete
  $select1->addField('program', 'id', 'id');
  $select1->addField('program', 'title', 'title');
  $select1->addField('program', 'code', 'code');
  $select1->addField('program', 'status', 'status');
  // Add condition
  $select1->condition('program.status', 'lrn_lpn_sts_atv', '=');
  $select1->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE');
  $select1->condition(db_or()
  	                     ->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE')
  	                     ->condition('program.code', '%' . db_like($searchText) . '%', 'LIKE')

  	                  );

  if (!empty($excludedCourseId)) {
    $excludedCourseId = urldecode($excludedCourseId);
    $excludedCourseIdArr = explode(',', $excludedCourseId);
  	$select1->condition('program.id', $excludedCourseIdArr, 'NOT IN');
  }
  filterByUserAccessPrivileges($select1,'program','',1,'','attach','program');  
  //$select1->orderBy('title');

    $select1->union($select,'UNION ALL');
  $matchingCourseNames = $select1->execute()->fetchAll();
  expDebug::dPrintDBAPI('$matchingCourseNames', $select1);
  expDebug::dPrint(' $matchingCourseNames = '. print_r($matchingCourseNames, true) , 3);

  foreach ($matchingCourseNames as $courseName) {
    if($idRequired == 0){
      print $courseName->title . "\n";
    } else {
      print $courseName->id."|".$courseName->title .  ' (' . $courseName->code . ')'  . "\n";
    }
  }
  }
  if($search_type == 'tags'){
  	getTrainingplanTagnameAutoComplete();
  }
  } catch (Exception $ex) {
  	watchdog_exception('getAllCourseTpAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCourseIdFromCode($code){
  try{
   $select = db_select('slt_course_template', 'crsmap');

   $select->addField('crsmap', 'id', 'id');
  // Set the conditions
  $select->condition('crsmap.code', $code, '=');
   $courseid = $select->execute()->fetchField();
   expDebug::dPrint(' $insertStmt obj = ' . print_r($courseid, true) , 3);
   return $courseid;
   } catch (Exception $ex) {
   	watchdog_exception('getCourseIdFromCode', $ex);
   	expertusErrorThrow($ex);
   }
}


function isAssociatedCourseExits($entityId,$associatedEntityId,$entityType,$type,$preRequisiteType='') {
try{
  	$select = db_select('slt_common_mapping','mapping');
		$select->addExpression('COUNT(mapping.id)', 'mappedcrscnt');
		$select->condition('mapping.id1', $entityId,'=');
		$select->condition('mapping.id2', $associatedEntityId,'=');
		$select->condition('mapping.object_type', $entityType,'=');
		$select->condition('mapping.type', $type,'=');
		$select->condition('mapping.prereq_type', $preRequisiteType,'=');

		$totalcrsmappingcnt = $select->execute()->fetchField();

		return $totalcrsmappingcnt;
		} catch (Exception $ex) {
			watchdog_exception('isAssociatedCourseExits', $ex);
			expertusErrorThrow($ex);
		}
}

function getAllTpAutoComplete() {
	try{
  $searchText =  $_GET['z'];
  $excludedCourseId = $_GET['excluded_course_id'];
  // Select the table
  $select = db_select('slt_program', 'program');
  // Add fields to fetchgetTpCourseAutoComplete
  $select->addField('program', 'id', 'id');
  $select->addField('program', 'title', 'title');
  $select->addField('program', 'code', 'code');

  // Add condition
  $select->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition(db_or()
  	                     ->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE')
  	                     ->condition('program.code', '%' . db_like($searchText) . '%', 'LIKE')

  	                  );

  if (!empty($excludedCourseId)) {
    $excludedCourseId = urldecode($excludedCourseId);
    $excludedCourseIdArr = explode(',', $excludedCourseId);
  	$select->condition('program.id', $excludedCourseIdArr, 'NOT IN');
  }

  // Order in dictionary sort order
  $select->orderBy('title');
  $matchingProgramNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $$matchingProgramNames = '. print_r($matchingProgramNames, true) , 3);

  foreach ($matchingProgramNames as $programName) {

    print $programName->id."|".$programName->title .  ' (' . $programName->code . ')'  . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getAllTpAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}//End  getAllTpAutoComplete()

/**
 * 0024925: Enforce equivalence in training plans
 * for this ticket we created equivalence option for training plan
 * don't delete it. Its may need for feature enhancement
 * When uncommanding it the previous function getAllTpAutoComplete() dont need
 *
 *
function getAllTpAutoComplete($from = false) {
	try{
  $searchText =  $_GET['z'];
  $excludedCourseId = $_GET['excluded_course_id'];
  // Select the table
  $select = db_select('slt_program', 'program');
  // Add fields to fetchgetTpCourseAutoComplete
  $select->addField('program', 'id', 'id');
  $select->addField('program', 'title', 'title');
  $select->addField('program', 'code', 'code');

  // Add condition
  $select->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE');
 if($from = 'trainingPlan'){
    $select->condition('program.status','lrn_lpn_sts_atv','=');
 }
  $select->condition(db_or()
  	                     ->condition('program.title', '%' . db_like($searchText) . '%', 'LIKE')
  	                     ->condition('program.code', '%' . db_like($searchText) . '%', 'LIKE')

  	                  );

  if (!empty($excludedCourseId)) {
    $excludedCourseId = urldecode($excludedCourseId);
    $excludedCourseIdArr = explode(',', $excludedCourseId);
  	$select->condition('program.id', $excludedCourseIdArr, 'NOT IN');
  }

  // Order in dictionary sort order
  $select->orderBy('title');
  $matchingProgramNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $$matchingProgramNames = '. print_r($matchingProgramNames, true) , 3);
  if($from = 'trainingPlan'){
    foreach ($matchingProgramNames as $programName) {

      print $programName->title. "\n";
    }
  }
  else {
    foreach ($matchingProgramNames as $programName) {
      print $programName->id."|".$programName->title .  ' (' . $programName->code . ')'  . "\n";
    }
   }

  } catch (Exception $ex) {
  	watchdog_exception('getAllTpAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}//End  getAllTpAutoComplete()
*/

function getProgramIdFromCode($code){
	try{
  $select = db_select('slt_program', 'prgmap');
  $select->addField('prgmap', 'id', 'id');
  // Set the conditions
  $select->condition('prgmap.code', $code, '=');
  $programid = $select->execute()->fetchField();
  expDebug::dPrint(' $programid = ' . print_r($programid, true) , 3);
  return $programid;
  } catch (Exception $ex) {
  	watchdog_exception('getProgramIdFromCode', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getAdminCityStateAutoComplete() - Returns the matched city and state name
 */
function getAdminCityStateAutoComplete() {
	try{
  $searchCurText =  $_GET['z'];

  $selectCity = db_select('slt_location', 'loc');
  $selectCity->distinct();
  $selectCity->addField('loc', 'city', 'name');
  $selectCity->addExpression("CONCAT('city')", 'city_state');
  $selectCity->condition('loc.city', '%' . db_like($searchCurText) . '%', 'LIKE');
  expDebug::dPrintDBAPI(' $selectCity OBJ = ' , $selectCity);
  $matchedResultsCity = $selectCity->execute()->fetchAll();

  $selectState = db_select('slt_country', 'sc');
  $selectState->leftjoin('slt_state', 'st', 'st.country_code = sc.country_code');
  $selectState->distinct();
  $selectState->addField('st', 'state_name', 'name');
  $selectState->addExpression("CONCAT('state')", 'city_state');
  $selectState->condition('st.state_name', '%' . db_like($searchCurText) . '%', 'LIKE');
  expDebug::dPrintDBAPI(' $selectState OBJ = ' , $selectState);
  $matchedResultsState = $selectState->execute()->fetchAll();

  $mergedValues = array_merge($matchedResultsCity,$matchedResultsState);
  foreach ( $mergedValues as $key => $resName) {
    //print $resName->name."|". $resName->city_state. "\n";
    print $resName->name. "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getAdminCityStateAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}



/*  Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
 * getAdminLocationAutoComplete() - Returns the matched string in Location Address and City or State and Country, Pincode
 */
 
function getAdminLocationAutoComplete($from_page='',$from_action='') {
	try{
 			
 		$location =  $_GET['z'];
		
		 expDebug::dPrint('$from_page ='.$from_page,5);
		 expDebug::dPrint('$from_action ='.$from_action,5);
		 expDebug::dPrint('$location ='.$location,5);
  
        $select = db_select('slt_location', 'sl');
		$select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=sl.country');
		$select->leftJoin('slt_state','loc_stats','loc_stats.state_code = sl.state and loc_stats.country_code=sl.country');
		
	
		$select->addExpression('DISTINCT sl.name', 'name');
		$select->addExpression('sl.id', 'loc_id');
		$select->addExpression('sl.addr1', 'addr1');
		$select->addExpression('sl.addr2', 'addr2');
		$select->addExpression('sl.city', 'city');
		$select->addExpression('sl.zipcode', 'zipcode');
		$select->addExpression('loc_stats.state_name', 'state_name');
		$select->addExpression('loc_ctry.country_name', 'country_name');  
		 
		
		if($from_page=='location' && $from_action=='citystate-autocomplete'){ //Remove the country filter if come from admin manage loction page
			
			$loc_or_condition = db_or()->condition('sl.name', '%' . db_like($location) . '%', 'LIKE')
	    								   ->condition('sl.addr1', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('sl.addr2', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('sl.city', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('loc_stats.state_name', '%' . db_like($location) . '%', 'LIKE') 
										   ->condition('sl.zipcode', '%' . db_like($location) . '%', 'LIKE');  
			
		}else{
			$loc_or_condition = db_or()->condition('sl.name', '%' . db_like($location) . '%', 'LIKE')
	    								   ->condition('sl.addr1', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('sl.addr2', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('sl.city', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('loc_stats.state_name', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('loc_ctry.country_name', '%' . db_like($location) . '%', 'LIKE')
										   ->condition('sl.zipcode', '%' . db_like($location) . '%', 'LIKE'); 
		} 
		
		$select->condition($loc_or_condition);   
		 
		filterByUserAccessPrivileges($select, 'sl','',1,'cre_sys_obt_loc','','sl');
		
		$qry_loc_autocomplete = $select->execute()->fetchAll();
				
		foreach ($qry_loc_autocomplete as $rec_loc_autocomplete) {
			
			$loc_str=trim($rec_loc_autocomplete->name);
			
			if(trim($rec_loc_autocomplete->addr1)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->addr1);
			}
			if(trim($rec_loc_autocomplete->addr2)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->addr2);
			}
			if(trim($rec_loc_autocomplete->city)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->city);
			}
			
			if(trim($rec_loc_autocomplete->zipcode)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->zipcode);
			}
			
			if(trim($rec_loc_autocomplete->state_name)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->state_name);
			}
			
			if(trim($rec_loc_autocomplete->country_name)!=''){
				$loc_str.=", ".trim($rec_loc_autocomplete->country_name);
			} 
			
			print $loc_str."\n"; 
		}  
   
  } catch (Exception $ex) {
  	watchdog_exception('getAdminLocationAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getParentAdminLinks() -
 */
function getParentAdminLinks($firstLinkOnly = false){
	try{
  global $user;
  $is_module_admin_access = is_module_admin_access();
  expDebug::dPrint("is_module_admin_access funciton returns  =  ".$is_module_admin_access,5);
  
  //$parentArr = cache_get('parent_array')->data;
  if(isset($_SESSION['parent_array']) && !$firstLinkOnly){
  	return $_SESSION['parent_array'];
  }

  $parentArray=array();

  if(module_exists('exp_sp_administration_learning')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/learning");
    $totalSubAdminLinks = count($subAdminLinks);
    if($totalSubAdminLinks>0){
    	if ($firstLinkOnly == true) {
    		return $subAdminLinks[0]['path'];
    	}
      $parentArray[]=array('code' => 'learning','title' => t('Learning'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }
  if(module_exists('exp_sp_administration_people')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/people");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'people','title' => t('People'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }
  if(module_exists('exp_sp_administration_survey')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/survey");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'survey','title' => t('Survey'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }
  if(module_exists('exp_sp_administration_survey')){
    $subAdminLinks= getTabDetailsForAdminGroup("administration/assessment");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'assessment','title' => t('Assessment'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }
  if(module_exists('exp_sp_administration_manage')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/manage");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'manage','title' => t('Manage'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }
 if(module_exists('exp_sp_administration_contentauthor')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/contentauthor");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'contentauthor','title' => t('LBL3021').t(' ').t('LBL3088'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
      
    }
  }

  if(module_exists('exp_sp_administration_commerce')){
    $subAdminLinks = getTabDetailsForAdminGroup("administration/commerce");
    $totalSubAdminLinks=count($subAdminLinks);
    if($totalSubAdminLinks>0){
      if ($firstLinkOnly == true) {
        return $subAdminLinks[0]['path'];
      }
      $parentArray[]=array('code' => 'commerce','title' => t('Commerce'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
    }
  }

  if (($user->uid == 1) || ($is_module_admin_access == 1)) { // #46665: site setting feature should be available only for drupal admin  ,   #0055025: SEO support for Expertusone
	 if(module_exists('exp_sp_administration_sitesetup')){
	    $subAdminLinks = getTabDetailsForAdminGroup("administration/sitesetup");
	    $totalSubAdminLinks=count($subAdminLinks);
	    if($totalSubAdminLinks>0){
	      if ($firstLinkOnly == true) {
	        return $subAdminLinks[0]['path'];
	      }
	      $parentArray[]=array('code' => 'sitesetup','title' => t('Site Setting'),'path' => $subAdminLinks[0]['path'],'admin_sub_menu_arr' => $subAdminLinks);
	    }
	  }
 }
  if ($firstLinkOnly == true) {
    // There is no admin module
    drupal_goto('page');
    return false;
  }

  expDebug::dPrint(' $parentArray = ' . print_r($parentArray, true) , 4);
  //cache_set('parent_array',$parentArray,'cache',CACHE_TEMPORARY);
  $_SESSION['parent_array'] = $parentArray;
  return $parentArray;
  } catch (Exception $ex) {
  	watchdog_exception('getParentAdminLinks', $ex);
  	expertusErrorThrow($ex);
  }
}


function filterSpecialCharacters($string){
	try{
  $isutfstr = mb_detect_encoding($string,"ASCII",true);
  //$isutfstr = mb_detect_encoding($string,"ASCII");
  //$isutfstr = mb_detect_encoding($string,"auto",true);
  //$isutfstr = mb_detect_encoding($string,"auto");
  if (is_string($isutfstr)){
	  if (preg_match('/[\'^£$%&*()}{@#~?><>,"|=_+¬-]/', $string)) {
	  	$string = trim($string);
	  	//$string = addslashes($string);
	  	//$string = htmlentities($string,'','UTF-8');
	  	$string = htmlentities($string);
	  	$string = utf8_encode($string);
	  }
  }
  return $string;
  } catch (Exception $ex) {
  	watchdog_exception('filterSpecialCharacters', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getDrupalSettings() - Returns drupal settings to be returned to the client
 */
function getDrupalSettings() {
	try {
  $scripts = drupal_add_js();
  if (!empty($scripts['settings'])) {
    $settings = $scripts['settings'];
    $settings = call_user_func_array('array_merge_recursive', $settings['data']);
  }

  return $settings;
  } catch (Exception $ex) {
  	watchdog_exception('getDrupalSettings', $ex);
  	expertusErrorThrow($ex);
  }
}



function delete_object_process($objectId, $objectType) {
	try {
	$updatedUserId = getSltpersonUserId();
	$entity_type = array();
  if ($objectType == "Class") {
  	$entity_type = array('cre_sys_obt_cls');
  	$clslangcode = db_query("select lang_code from slt_course_class where id =:pid",array(":pid"=>$objectId))->fetchField();
    $select = db_update('slt_course_class');
    $select->fields(array('status'     => 'lrn_cls_sts_del',
    					  'lang_code' 	=> $clslangcode.'_deleted_'.$objectId,
        				  'updated_on' => now(),
              			  'updated_by' => $updatedUserId
                          ));
     $select->condition('id', $objectId, '=');
     $result = $select->execute();
     updateTags($objectId,$objectType); // To Delete the Tag entity and Update the tagdefn of the Class
     updateGroupMappingDetails($objectId, 'cre_sys_obt_cls', '','emptydelete',0);
     add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_cls', 'exp_sp_administration_catalog', 'delete_object_process', 'Class Deleted');
	 $objectType = 'cre_sys_obt_cls';
	 syncSolrData('Class'); //To sync solr
  }
  else if ($objectType == "Course") {
  	 $entity_type = array('cre_sys_obt_crs');
  	 $clslangcode = db_query("select lang_code from slt_course_class where id =:pid",array(":pid"=>$objectId))->fetchField();
  	 $class_id = getClassIds($objectId,'all',true);
	 foreach($class_id as $cls){
	    $new_Ids[] = $cls->classid;
	 }
     $select = db_update('slt_course_class');
     $select->fields(array('status'     => 'lrn_cls_sts_del',
     					   'lang_code' 	=> $clslangcode.'_deleted_'.$objectId,
        				   'updated_on' => now(),
              			   'updated_by' => $updatedUserId
                           ));
     $select->condition('course_id', $objectId, '=');
     $result = $select->execute();
     // delete access for class
     deleteAttachedGroup(implode(',',$new_Ids),'cre_sys_obt_cls');     
     if(count($new_Ids)>0){
     	foreach($new_Ids as $cls){
   	 		add_audit_trail_entry($updatedUserId, $cls, 'cre_sys_obt_cls', 'exp_sp_administration', 'delete_object_access', 'Access Deleted');
     	}    
     }
     // to get the Class id of the Course which is attached.
     $query = db_select('slt_course_class', 'class');
     $query->addField('class', 'id');
     $query->condition('class.course_id', $objectId);
     $results = $query->execute()->fetchAll();
     foreach ($results as $record){
     	expDebug::dPrint('$record : '.print_r($record->id, true), 4);
     	updateTags($record->id,'Class'); // To Delete the Tag entity and Update the tagdefn of the Class
     	updateGroupMappingDetails($record->id, 'cre_sys_obt_cls', '','emptydelete',0); //delete the flat table
     }
     $crslangcode = db_query("select lang_code from slt_course_template where id=:pid",array(":pid"=>$objectId))->fetchField();
     $select = db_update('slt_course_template');
     $select->fields(array('status'     => 'lrn_crs_sts_del',
     					   'lang_code' 	=> $crslangcode.'_deleted_'.$objectId,
        				   'updated_on' => now(),
              			   'updated_by' => $updatedUserId
                          ));
     $select->condition('id', $objectId, '=');
     $result = $select->execute();
     updateTags($objectId,$objectType);  // To Delete the Tag entity and Update the tagdefn of the Course
     updateGroupMappingDetails($objectId, 'cre_sys_obt_crs', '','emptydelete',0); //delete the flat table
     add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_crs', 'exp_sp_administration_catalog', 'delete_object_process', 'Course Deleted');
     $objectType = 'cre_sys_obt_crs';
     syncSolrData('Course'); //To sync solr
  }
  else if ($objectType == "TP") {
  	$prglangcode = db_query("select lang_code from slt_program where id =:pid",array(":pid"=>$objectId))->fetchField();
  	$entity_type = array('cre_sys_obt_trp','cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn');
  	 $tpType= db_query("select object_type from slt_program where id =:pid",array(":pid"=>$objectId))->fetchField();
     $select = db_update('slt_program');
     $select->fields(array('status'     => 'lrn_lpn_sts_del',
     					   'lang_code' 	=> $prglangcode.'_deleted_'.$objectId,
        				   'updated_on' => now(),
              			   'updated_by' => $updatedUserId
                           ));
     $select->condition('id', $objectId, '=');
     $result = $select->execute();
     updateGroupMappingDetails($objectId, $tpType, '','emptydelete',0);
     updateTags($objectId,$objectType); // To Delete the Tag entity and Update the tagdefn of the Tp
     add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_trp', 'exp_sp_administration_program', 'delete_object_process', 'TP Deleted');
  	 $objectType = 'cre_sys_obt_trp';
  	 syncSolrData('Program'); //To sync solr
  }
  else if ($objectType == "User") {
  	$entity_type = array('cre_usr');
  	 $personDetails = getPersonDetails($objectId, array('email', 'user_name'));
  	 $drupalUserId = getDrupalIdOfUser($objectId);
  	 $groupDetail = getGroupsDetails($objectId);
  	 $txn = db_transaction();
  	 try {
       $updateStmt1 = db_update('slt_person');
       $updateStmt1->condition('id', $objectId, '=');
       $updateStmt1->fields(array('status'     => 'cre_usr_sts_del',
                                  'email'      => $personDetails['email'] . '-deleted-' . $objectId,
       								//Commented for ticket - 15055
                                    //'user_name'  => $personDetails['user_name'] . '-deleted-' . $objectId,
                                    //uncommented - 0041257
                                   'user_name'  => $personDetails['user_name'] . '-deleted-' . $objectId,
        					       'updated_on' => now(),
              				       'updated_by' => $updatedUserId
                             ));
       $updateStmt1->execute();

       $updateStmt2 = db_update('users');
       $updateStmt2->condition('uid', $drupalUserId);
       $updateStmt2->fields(array('status' => '0',
                                  'mail'   => $personDetails['email'] . '-deleted-' . $objectId,
                                  'name'   => $personDetails['user_name'] .'-deleted-' . $objectId// this line is added for Tickets:0028583 for allow a user to use a username if it is previously deleted.
       								//Commented for ticket - 15055
                                  //'name'   => $personDetails['user_name'] . '-deleted-' . $objectId,
                                ));
       $updateStmt2->execute();
       updateGroupMappingDetails($objectId, 'cre_usr', '','emptydelete',0);
       //Added for #0064675
       foreach($groupDetail as $key => $value)
       {
       	if(isset($value->name))
       	{
       		$grpId = $value->id;
       	}
       	$userDelete = db_query('UPDATE slt_groups SET added_users = if(REPLACE(CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\') = \',\',
       			NULL ,TRIM(BOTH \',\' FROM REPLACE(
       			CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\'))) WHERE id = :grpid',array('userid'=>$objectId,'grpid'=>$grpId));
       
       	$userDelete1 = db_query('UPDATE slt_groups SET userslist = if(REPLACE(CONCAT(\',\', userslist, \',\'),CONCAT(\',\', :userid, \',\'), \',\') = \',\',
       			NULL, TRIM(BOTH \',\' FROM REPLACE(
       			CONCAT(\',\', userslist, \',\'),CONCAT(\',\', :userid, \',\'), \',\'))) WHERE id = :grpid',array('userid'=>$objectId,'grpid'=>$grpId));
       	$insertStmt = db_insert('slt_group_audit');
       	$fields = array(
       			'group_id'         => $grpId,
       			'user_id'		   => $objectId,
       			'operation'        => 'User Deleted',
       			'criteria_match'   => 'Y',
       			'updated_by'       => $updatedUserId,
       			'updated_on'       => now()
       	);
       	$insertStmt->fields($fields);
       	$insertStmt->execute();
       }
       syncSolrData('User'); //To sync solr
    }
    catch (Exception $ex) {
      $txn->rollback();
      watchdog_exception('useradmin delete_object_process', $ex);
      throw $ex;
    }
    // Commit the transaction
    unset($txn);
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_usr', 'exp_sp_administration_user', 'delete_object_process', 'User Deleted');
  	 $objectType = 'cre_sys_obt_usr';
  }
  else if ($objectType == "Organization") {
  	$entity_type = array('cre_org');
    $select = db_update('slt_organization');
    $select->fields(array('status' => 'cre_org_sts_del',
        					        'updated_on'    => now(),
              				    'updated_by'    => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, 'cre_org', '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_org', 'exp_sp_administration_organization', 'delete_object_process', 'Organization Deleted');
  	$objectType = 'cre_sys_obt_org';
  }
	else if ($objectType == "Grp") {
	$entity_type = array('cre_sec','cre_sec_learner');
    $select = db_update('slt_groups');
    $select->fields(array('status'     => 'cre_sec_sts_del',
        					        'updated_on' => now(),
              				    'updated_by' => $updatedUserId
                          ));
     $select->condition('id', $objectId, '=');
     $result = $select->execute();
     updateGroupMappingDetails($objectId, 'cre_sec', '','emptydelete',0);
     updateGroupMappingDetails('','', '','deletegroup',$objectId);
     add_audit_trail_entry($updatedUserId, $objectId, 'cre_sec', 'exp_sp_administration_groups', 'delete_object_process', 'Role Deleted');

     	$select = db_select('role','role');
	  	$select->join('slt_groups','srole','role.name=srole.name');
	    $select->addField('role','rid','rid');
	    $select->addField('srole','is_admin','is_admin');
		  $select->condition('srole.id',$objectId,'=');
		  expDebug::dPrintDBAPI('Fetch role Id ' , $select);
		  $role_dt =$select->execute()->fetchAssoc();
		  if($role_dt['is_admin']==1){
		  	//Entry for securitymanagementAdmin
		  	$select = db_delete('slt_admin_group_users');
		  	$select->condition('group_id',$objectId);
		  	expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $select);
		  	$select->execute();
		  }
		  
		  $loggeduserId = getSltpersonUserId();
		  // Get the fields before deleting
		  $select = db_select('slt_group_user_mapping', 'gumap');
		  $select->addField('gumap','id','id');
		  $select->addField('gumap','group_id','group_id');
		  $select->addField('gumap','user_id','user_id');
		  $select->addField('gumap','group_type','group_type');
		  $select->condition('group_id',$objectId);
		  $gumapId =$select->execute()->fetchAll();
		  
		  // Added by Vincent on 20 Jan,2017. For #0071187
		  $delgrp = db_delete('slt_group_user_mapping');
		  $delgrp->condition('group_id',$objectId);
		  expDebug::dPrintDBAPI('delete userlist slt_group_user_mapping' , $delgrp);
		  $delgrp->execute();

		  foreach($gumapId as $val){
		      if($val->group_type == 1){
		          $grp_type = 'Admin Group';
		      }
		      else if($val->group_type == 0){
		          $grp_type = 'Learner Group';
		      }
		      // Make deleted log entry
		      $fields = array(
		              'table_name' => 'slt_group_user_mapping',
		              'entity_id' => $val->id,
		              'entity_type' => 'group_user_mapping',
		              'parent1_entity_id' => $val->group_type,
		              'parent1_entity_type' => $grp_type,
		              'parent2_entity_id' => $val->group_id,
		              'parent2_entity_type' => 'group_id',
		              'parent3_entity_id' => $val->user_id,
		              'parent3_entity_type' => 'user_id',
		              'module_name' => 'exp_sp_administration',
		              'functionality_name' => 'delete_object_process',
		              'api_name' => $_REQUEST['apiname'],
		              'deleted_on' => now(),
		              'deleted_by' => $loggeduserId		              
		      );
		      deleted_log_entry($fields);
		  }
		  
		  //To sync solr
	  	  syncSolrData('User','','Bulk');
			  
		  if(!empty($role_dt['rid']) && $role_dt['is_admin']==1){
		  	$role_id = $role_dt['rid'];
		  	
		  	$sel = db_select('users_roles','u');
		  	$sel->addField('u','uid','uid');
		  	$sel->condition('rid',$role_id,'=');
		  	expDebug::dPrintDBAPI('select userlist1 >>>' , $sel);
		  	$sel1=$sel->execute()->fetchAll();
		  	
		    $select1 = db_delete('users_roles');
		    $select1->condition('rid',$role_id);
		    expDebug::dPrintDBAPI('delete userlist1 ' , $select1);
		    $select1->execute();

		   foreach($sel1 as $val)
		    {
		        $fields = array(
		                'table_name' => 'users_roles',
		                'entity_id' => $val->uid,
		                'entity_type' => 'group_users',
		                'module_name' => 'exp_sp_administration',
		                'functionality_name' => 'delete_object_process',
		                'deleted_on' => now(),
		                'deleted_by' => getIdOfLoggedInUser(),
		                'parent1_entity_id' => $role_id,
		                'parent1_entity_type' => 'rid',
		                'parent2_entity_id' => $val->uid,
		                'parent2_entity_type' => 'uid',
		    
		        );
		        deleted_log_entry($fields);
		    }

		    $select2 = db_delete('role_permission');
		    $select2->condition('rid',$role_id);
		    expDebug::dPrintDBAPI('delete userlist2 ' , $select2);
		    $select2->execute();

		    $select3 = db_delete('role');
		    $select3->condition('rid',$role_id);
		    expDebug::dPrintDBAPI('delete userlist3 ' , $select3);
		    $select3->execute();
		  }
		  $objectType = 'cre_sec';
  }
  else if ($objectType == "SurAss") {
  	$entity_type = array('sry_det_typ_sry','sry_det_typ_ass');
  	$SurAsscode = db_query("select code from slt_survey where id =:pid",array(":pid"=>$objectId))->fetchField();
  	$SurAssType= db_query("select type from slt_survey where id =:pid",array(":pid"=>$objectId))->fetchField();
    $select = db_update('slt_survey');
    $select->fields(array('status'     => 'sry_det_sry_del',
    					  'code' 	=> $SurAsscode.'_deleted_'.$objectId,
        				  'updated_on' => now(),
              			  'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, $SurAssType, '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_sur', 'exp_sp_administration_survey', 'delete_object_process', 'Survey/ Asssessment Deleted');
    $objectType = 'cre_sys_obt_sur';
  }
  else if ($objectType == "SurAssQues"){
  	$entity_type = array('sry_det_typ_sry_qus','sry_det_typ_ass_qus');
  	$SurAssQuesType= db_query("select entity_type from slt_survey_questions where id =:pid",array(":pid"=>$objectId))->fetchField();
    $select = db_update('slt_survey_questions');
    $select->fields(array('status'     => 'sry_qtn_sts_del',
        					        'updated_on' => now(),
              				    'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, $SurAssQuesType, '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_sur', 'exp_sp_administration_survey', 'delete_object_process', 'Question Deleted');
  	 $objectType = 'cre_sys_obt_sur';
  }
  else if ($objectType == "Content"){
  	$contentcode = db_query("select code from slt_content_master where id =:pid",array(":pid"=>$objectId))->fetchField();
  	$entity_type = array('cre_sys_obt_cnt');
    $select = db_update('slt_content_master');
    $select->fields(array('status'     => 'lrn_cnt_sts_del',
    					  'code' 	=> $contentcode.'_deleted_'.$objectId,
                          'updated_on' => now(),
                          'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    
    $select_version = db_update('slt_content_version');
    $select_version->fields(array('status'     => 'lrn_cnt_sts_del',
    		'updated_on' => now(),
    		'updated_by' => $updatedUserId
    ));
    $select_version->condition('content_master_id', $objectId, '=');
    $result_version = $select_version->execute();
    
    //44353: When an admin user deletes the content the physical file should be deleted from the content folder
    $filepathSelect  = db_select('slt_content_version','version');
    $filepathSelect->addField('version','file_path');
    $filepathSelect->addField('version','content_sub_type');
    $filepathSelect->condition('version.content_master_id',$objectId,'=');
    $result = $filepathSelect->execute()->fetchAll();
    expDebug::dPrint(' deleteContent Result '.print_r($result,true),4);
    $CDNfilelist =  array();
    $rackspace_cloud_container = variable_get('rackspace_cloud_container');
    $uploadDirectory = './sites/default/files/contentupload';
    foreach ($result as $val){
    	if(!empty($val->file_path) && ($val->file_path != $uploadDirectory) && ($val->file_path != $uploadDirectory.'/')){
    		expDebug::dPrint(' deleteContentVersion delete file path '.$val->file_path);
    		/* get all the file paths */
    		$directory = new RecursiveDirectoryIterator($val->file_path);
    		foreach (new RecursiveIteratorIterator($directory, RecursiveIteratorIterator::CHILD_FIRST) as $filename=>$current) {
    			// $CDNfilelist[] =  str_replace('\\','/',str_replace('./sites/default/files','/TESTContainer', $current->getPathName()));
    			$CDNfilelist[] =  str_replace('\\','/',str_replace('./sites/default/files','/'.$rackspace_cloud_container, $current->getPathName()));
    		}
    		//$CDNfilelist[] = str_replace('./sites/default/files','/TESTContainer', $val->file_path);
    		$CDNfilelist[] = str_replace('./sites/default/files','/'.$rackspace_cloud_container, $val->file_path);
	     	removedir($val->file_path);
	     	unlink($val->file_path.'.zip');
    	}
    	if(trim($val->content_sub_type) =='lrn_cnt_typ_vod') {
    		$folder_name = end(explode('/', $val->file_path));
    		expDebug::dPrint(' ffmpeg queue folder name'.$folder_name);
    		if (db_table_exists(slt_ffmpeg_queue)) {
    			db_delete('slt_ffmpeg_queue')
    				->condition('folder_name',$folder_name)
    				->execute();
    		}
    	}
    	//delete the converted files folder like pdf and swf
    	if(trim($val->content_sub_type) =='lrn_cnt_typ_knc') {
    		$uploaded_folder_name =  end(explode('/', $val->file_path));
	    	//upload path variable from config
	    	$config=getConfig("exp_sp");
	    	$content_upload_path = $config['content_upload_path'];
	    	$designation_folder = $content_upload_path.'/contentupload/convertioncontent/'.$uploaded_folder_name;
	    	if(is_dir($designation_folder)){
	    		removedir($designation_folder);
    }
    	}
    }
    updateGroupMappingDetails($objectId, 'cre_sys_obt_cnt', '','emptydelete',0);
    updateTags($objectId,$objectType); // To Delete the Tag entity and Update the tagdefn of the Content
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'delete_object_process', 'Content Deleted');

    /*Start of remove from CDN if CDN is enabled*/

    try {

    	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc');
    	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
    	$cdn_status  = getDrupalModuleStatus(array('cloud_files'));
    	expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' .print_r($cdn_status,true), 4);

    	if($cdn_status == 1 )
    	{
    		$rackspace_cloud_region = variable_get('rackspace_cloud_region');
    		$rackspace_cloud_container = variable_get('rackspace_cloud_container');

    		expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_container->' . print_r($rackspace_cloud_container, true) , 4);
    		expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_region->' . print_r($rackspace_cloud_region, true) , 4);

    		$client = initializeCDN();
    		$client->authenticate();
    		$object_store_service = $client->objectStoreService('cloudFiles', $rackspace_cloud_region);
    		$container = $object_store_service->getContainer($rackspace_cloud_container);

    		expDebug::dPrint(" Delete CDN Content - CDNfilelist ".print_r($CDNfilelist, true) , 4);
    		$object_store_service->bulkDelete($CDNfilelist);


    		add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'delete_object_process', 'CDN Content Deleted');
    	}
		 $objectType = 'cre_sys_obt_cnt';
    } catch (Exception $ex) {
    	watchdog_exception('deleteContentVersion : Delete CDN Content ', $ex);
    	expDebug::dPrint(' deleteContentVersion : Delete CDN Content Exception->' . print_r($ex, true) , 1);
    	//expertusErrorThrow($ex);
  }

    /*End of remove from CDN if CDN is enabled*/

  }
  else if ($objectType == "location"){
  	$entity_type = array('cre_sys_obt_loc');
    $select = db_update('slt_location');
    $select->fields(array('status'     => 'lrn_res_loc_del',
                          'updated_on' => now(),
                          'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, 'cre_sys_obt_loc', '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_loc', 'exp_sp_administration_location', 'delete_object_process', 'Location Deleted');
  	$objectType = 'cre_sys_obt_loc';
  }
//#custom_attribute_0078975
  else if ($objectType == "Custom" && module_exists('exp_sp_administration_customattribute')){
    //$entity_type = array('cre_sys_obt_cattr');
    $select = db_update('slt_custom_attr');
    $select->fields(array('status'     => 'cre_cattr_sts_del',
            'updated_on' => now(),
            'updated_by' => $updatedUserId
    ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    //updateGroupMappingDetails($objectId, 'cre_sys_obt_cattr', '','emptydelete',0);  
    add_audit_trail_entry($updatedUserId, $objectId, 'cre_sys_obt_cattr', 'exp_sp_administration_customattribute', 'delete_object_process', 'Custom Attribute Deleted');
    $objectType = 'cre_sys_obt_cattr';
    
    
    //Get current mapping details of custom attribute after save the new changes
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_screen.inc');
        $cur_attr_map_arr=getEntityMappingDetails($objectId);  
        expDebug::dPrint(' Unmapping the field $cur_attr_map_arr = '.print_r($cur_attr_map_arr,true), 5);
         
        if(count($cur_attr_map_arr)>0){
            foreach($cur_attr_map_arr as $cur_key => $cur_val){
                
                emptyEntityTableColumnValue($cur_key,$cur_val,$updatedUserId);
                
                /*
                if(!array_key_exists($pre_key,$cur_attr_map_arr)){
                    //Unmap respective coulumn 
                    expDebug::dPrint('There is no mapping for this entity ='.$pre_key.'--'.$pre_val, 5);
                    emptyEntityTableColumnValue($pre_key,$pre_val);
                }else{
                    expDebug::dPrint(' Mapping is there= '.$pre_key.'--'.$pre_val, 5);
                }
                */
            }
        } 
    
    
  }
  else if($objectType == "banner"){
  	$entity_type = array('cbn_anm_typ_ban');
    $select = db_update('slt_announcement_master');
    $select->fields(array('status'     => 'cbn_anm_sts_del',
                          'updated_on' => now(),
                          'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, 'cbn_anm_typ_ban', '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cbn_anm_typ_ban', 'exp_sp_administration_banner', 'delete_object_process', 'Banner Deleted');
  	$objectType = 'cbn_anm_typ_ban';
  }else if($objectType == "announcement"){
  	$entity_type = array('cre_sys_obt_not');
    $select = db_update('slt_site_notice');
    $select->fields(array('status'     => 'cre_sys_obt_not_del',
                          'updated_on' => now(),
                          'updated_by' => $updatedUserId
                         ));
    $select->condition('id', $objectId, '=');
    $result = $select->execute();
    updateGroupMappingDetails($objectId, 'cre_sys_obt_not', '','emptydelete',0);
    add_audit_trail_entry($updatedUserId, $objectId, 'cbn_anm_typ_ban', 'exp_sp_administration_banner', 'delete_object_process', 'Banner Deleted');
  	$objectType = 'cre_sys_obt_not';
  }else if($objectType == "UnlockUser"){
    $deleteStmt = db_delete('flood');
    $deleteStmt->condition('event', 'failed_login_attempt_user', '=');
    $deleteStmt->condition('identifier', $objectId.'-' . '%', 'LIKE');
    $deleteStmt->execute();
    expDebug::dPrintDBAPI('UnlockUser Delete = ' , $deleteStmt);
  }
  else if($objectType == "Tax"){
    $deleteStmt = db_delete('slt_tax_product_code_mapping');
    $deleteStmt->condition('tax_settings_id', $objectId , '=');
    $deleteStmt->execute();
    expDebug::dPrintDBAPI('slt_tax_product_code_mapping Delete = ' , $deleteStmt);
    $deleteTax = db_delete('slt_tax_settings');
    $deleteTax->condition('id', $objectId , '=');
    $deleteTax->execute();
    expDebug::dPrintDBAPI('slt_tax_settings Delete = ' , $deleteTax);
    add_audit_trail_entry($updatedUserId, $objectId, 'Commerce', 'exp_sp_administration_commerce_tax', 'delete_object_process', 'Tax Deleted');
  }else if($objectType == "certificate" || $objectType == "notification_template"){
  	$entity_type = array('cre_ntn','cre_cer');
  	$Type= db_query("select type from slt_notification_info where id =:pid",array(":pid"=>$objectId))->fetchField();
  	expDebug::dPrint('$default notify type = ' . print_r($Type, true) );
    $deleteStmt = db_delete('slt_notification_frame');
    $deleteStmt->condition('notification_id', $objectId , '=');
    $deleteStmt->execute();
    expDebug::dPrintDBAPI('slt_notification_frame Delete = ' , $deleteStmt);
    $deleteTax = db_delete('slt_notification_info');
    $deleteTax->condition('id', $objectId , '=');
    $deleteTax->execute();
    expDebug::dPrintDBAPI('slt_notification_info Delete = ' , $deleteTax);
    updateGroupMappingDetails($objectId, $Type, '','emptydelete',0);
    $objectType = 'cre_ntn';
  }
  if(count($entity_type)>0){
  	
   	deleteAttachedGroup($objectId,$entity_type);
   	add_audit_trail_entry($updatedUserId, $objectId, $objectType, 'exp_sp_administration', 'delete_object_access', 'Access Deleted');
  }
  } catch (Exception $ex) {
  	watchdog_exception('delete_object_process', $ex);
  	expertusErrorThrow($ex);
  }
}

function delete_object_record($objectId,$objectType){
	try{
		if (strpos($objectId, VAR_START_CHAR) !== false)
			$objectId = (int) core_decrypt($objectId);
	expDebug::dPrint('$objectType : '.$objectId, 4);
  $resultCount = '';
  expDebug::dPrint('$objectType : '.$objectType, 4);
  if($objectType == "Classdelete") {
    $resultCount = getEnrolledClassDeleteAccess($objectId);
  }else if($objectType == "Class"){
    $resultCount = ifClassCanBeCanceled($objectId);
  }else if($objectType == "Course"){
    $resultCount = getEnrolledCourseDeleteAccess($objectId);
  }else if($objectType == "TP"){
    $resultCount = getEnrolledTPDeleteAccess($objectId);
  }else if($objectType == "User"){
    $resultCount = getEnrolledUserDeleteAccess($objectId);
  }else if($objectType == "Organization"){
    $resultCount = getOrganizationDeleteAccess($objectId);
  }else if($objectType == "SurAss"){
    $resultCount = getSurveyAssessmentDeleteAccess($objectId);
  }else if($objectType == "Content"){
    $resultCount = getContentDeleteAccess($objectId);
  } else if($objectType == "location"){
    $resultCount = getLocationDeleteAccess($objectId);
  } else if($objectType == "Custom" && module_exists('exp_sp_administration_customattribute')){//#custom_attribute_0078975
    $resultCount = getCustomAttributeDeleteAccess($objectId);
  }else if($objectType == "banner"){
    $resultCount = getBannerDeleteAccess($objectId);
  }else if($objectType == "announcement"){
    $resultCount = getAnnouncementDeleteAccess($objectId);
  }else if($objectType == "SurAssQues"){
    $resultCount = getQuestionEnrolledDeleteAccess($objectId);
  }else if($objectType == "Tax"){
    $resultCount = 0;
  }else if($objectType == "certificate" || $objectType == "notification_template"){
    $resultCount = getCertificateDeteteAccess($objectId);
  }else if($objectType == "Grp"){
    $map_count = getGroupsDeleteAccess($objectId);
    $checkStatus = array ('new','inprogress');
    $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$objectId);
    if($map_count || $back_gd_running){
    	$resultCount = (!empty($map_count)) ? $map_count : $back_gd_running;
    }
  }


  //print $resultCount;
  return $resultCount;
  } catch (Exception $ex) {
  	watchdog_exception('delete_object_record', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * getEnrolledClassDeleteAccess - Use this function to delete Class
 */

function getEnrolledClassDeleteAccess($classId){
 try{
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(1)','total_enroll');
  $select->condition('enroll.class_id', $classId, '=');
  $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
  $select->condition('enroll.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_nsw') ,'NOT IN');
  $total_enroll = $select->execute()->fetchField();

  $selectCls = db_select('slt_course_class_session', 'cs');
  $selectCls->addExpression('COUNT(1)','total_cls');
  $selectCls->condition('cs.class_id', $classId, '=');
  $selectCls->where("CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) >= NOW()");
  $futuredClass = $selectCls->execute()->fetchField();




  if ($total_enroll== 0 && $futuredClass == 0){
    return 0;
  } else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getEnrolledClassDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getEnrolledCourseDeleteAccess - Use this function to delete Course
 */

function getEnrolledCourseDeleteAccess($courseId){
 try{
 	//Added left join for #0057290 
  $select = db_select('slt_enrollment', 'enroll');
  $select->leftJoin('slt_course_class','cls','cls.id = enroll.class_id');
  $select->addExpression('COUNT(1)','total_enroll');
  $select->condition('enroll.course_id', $courseId, '=');
  $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
  $select->condition('enroll.comp_status', 'lrn_crs_cmp_cmp' ,'<>');
  $select->condition('cls.status', 'lrn_cls_sts_del' ,'<>');
  $total_enroll = $select->execute()->fetchField();

  $selectCls = db_select('slt_course_class_session', 'cs');
  $selectCls->leftJoin('slt_course_class','cls','cls.id = cs.class_id');
  $selectCls->addExpression('COUNT(1)','total_cls');
  $selectCls->condition('cs.course_id', $courseId, '=');
  $selectCls->condition('cls.status', 'lrn_cls_sts_atv', '=');
  $selectCls->where("CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) >= NOW()");
  $futuredClass = $selectCls->execute()->fetchField();

  $selectCrs = db_select('slt_module_crs_mapping', 'map');
  $selectCrs->leftJoin('slt_program', 'prg', 'prg.id = map.program_id');
  $selectCrs->addExpression('COUNT(1)','totRec');
  $selectCrs->condition('map.course_id', $courseId, '=');
  $selectCrs->condition('prg.status',array('lrn_lpn_sts_atv','lrn_lpn_sts_itv'), 'IN');
  $crsAssociated = $selectCrs->execute()->fetchField();

  $selectrecertity = db_select('slt_re_certify', 'cer');
  $selectrecertity->leftJoin('slt_program', 'cerprg', 'cerprg.id = cer.program_id');
  $selectrecertity->addExpression('COUNT(1)','certifycnt');
  $selectrecertity->condition('cer.course_id', $courseId, '=');
  $selectrecertity->condition('cerprg.status',array('lrn_lpn_sts_atv','lrn_lpn_sts_itv'), 'IN');
  $total_recertify = $selectrecertity->execute()->fetchField();

  if ($total_enroll== 0 && $futuredClass == 0 && $crsAssociated == 0 && $total_recertify == 0){
    return 0;
  } else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getEnrolledCourseDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getLocationDeleteAccess - Use this function to delete the Location
 */

function getLocationDeleteAccess($objectId){
	try{
  $select = db_select('slt_course_class', 'cls');
  $select->addExpression('COUNT(1)','class_count');
  $select->condition('cls.location_id', $objectId);
  $select->condition('cls.status', array('lrn_cls_sts_del','lrn_cls_sts_can','lrn_cls_sts_dld') ,'NOT IN');
  $select->condition('cls.location_id', '', 'IS NOT NULL');
  expDebug::dPrintDBAPI('query for loc', $select);
  $classCount = $select->execute()->fetchField();
  return $classCount;
  } catch (Exception $ex) {
  	watchdog_exception('getLocationDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getLocationSuspendAccess - Use this function to suspend the Location
 */

function getLocationSuspendAccess($objectId){
	try{
  $select = db_select('slt_course_class', 'cls');
  $select->leftJoin('slt_course_class_session', 'cs', 'cs.location_id = cls.location_id');
  $select->addExpression('COUNT(1)','class_count');
  $select->condition('cls.location_id', $objectId);
  $select->condition('cls.location_id', '', 'IS NOT NULL');
  $select->condition('cls.status', array('lrn_cls_sts_del','lrn_cls_sts_can','lrn_cls_sts_dld') ,'NOT IN');
  $select->where("CONCAT(DATE(cs.start_date),' ',CONCAT(cs.start_time,':00')) >= NOW()");
  expDebug::dPrintDBAPI('suspend loc', $select);
  $classCount = $select->execute()->fetchField();
  return $classCount;
  } catch (Exception $ex) {
  	watchdog_exception('getLocationSuspendAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getEnrolledTPDeleteAccess - Use this function to delete Certification/Curricula or Learning Plan
 */

function getEnrolledTPDeleteAccess($objectId){
	try{
  $select = db_select('slt_master_enrollment', 'master_enroll');
  $select->addExpression('COUNT(1)','total_master_enroll');
  $select->condition('master_enroll.program_id', $objectId, '=');
  $select->condition('master_enroll.overall_status',array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_wtl'), 'IN');

  $master_enrolled = $select->execute()->fetchField();

  return $master_enrolled;
  } catch (Exception $ex) {
  	watchdog_exception('getEnrolledTPDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getEnrolledUserDeleteAccess - Use this function to delete user
 */
function getEnrolledUserDeleteAccess($objectId){
	try{
  $rec_del=0;
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(1)','total_enroll');
  $select->condition('enroll.user_id', $objectId, '=');
  $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
  $select->where("enroll.comp_status in ('lrn_crs_cmp_enr','lrn_crs_cmp_inc') or enroll.comp_status is null");
  $total_enroll = $select->execute()->fetchField();

  $select1 = db_select('slt_person', 'per');
  $select1->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
  $select1->addExpression('COUNT(1)','total_person');
  $select1->condition('per.status', 'cre_usr_sts_atv', '=');
  $select1->where("per.manager_id=$objectId OR per_oth_mng.manager_id =$objectId");
  expDebug::dPrintDBAPI('getEnrolledUserDeleteAccess = ' , $select1);
  $total_person = $select1->execute()->fetchField();

  if($total_enroll>0 || $total_person>0){
  	$rec_del = 1;
  }else{
  	$rec_del=0;
  }
  return $rec_del;
  } catch (Exception $ex) {
  	watchdog_exception('getEnrolledUserDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrganizationDeleteAccess - Use this function to delete user
 */
function getOrganizationDeleteAccess($objectId){
	try{
  $select = db_select('slt_person', 'per');
  $select->leftJoin('slt_person_other_organization', 'spoo', 'spoo.user_id = per.id and spoo.is_direct=\'N\'');
  $select->addExpression('COUNT(1)','total_org');
  $select->condition(db_or()->condition('per.org_id', $objectId, '=')->condition('spoo.organization_id',$objectId));
  $select->condition('per.status', 'cre_usr_sts_del', '!=');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  $total_org = $select->execute()->fetchField();

  $select = db_select('slt_organization', 'org');
  $select->addExpression('COUNT(org.id)', 'count');
  $select->condition('org.parent_org_id', $objectId, '=');
  $select->condition('org.status', 'cre_org_sts_act', '=');
  expDebug::dPrintDBAPI('getOrganizationDeleteAccess = ' , $select);
  $numOrg = $select->execute()->fetchField();

  //  Check if the Organization is Associated To the Group
  $selectgrp = db_select('slt_groups', 'grp');
  $selectgrp->addExpression('COUNT(grp.id)', 'count');
  $selectgrp->where('FIND_IN_SET(' . $objectId . ', grp.org_id) > 0');
  $selectgrp->condition('grp.status','cre_sec_sts_del','!=');
  expDebug::dPrintDBAPI('$selectgrp = ' , $selectgrp);
  $numGrp = $selectgrp->execute()->fetchField();
  $sel_org = ($numGrp > 0) ? 1 : 0;

  if($total_org > 0 || $numOrg > 0 || $sel_org > 0){
  	$rec_org = 1;
  }else{
  	$rec_org=0;
  }
  return $rec_org;
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getSurveyAssessmentDeleteAccess - Use this function to delete survey
 */

function getSurveyAssessmentDeleteAccess ($objectId){
	try{
		// Get Active Class Count -- Fixed For this Ticket #0038530
		$selectCls =  db_select('slt_survey_mapping', 'survmap');
		$selectCls->leftJoin('slt_course_class', 'cls', 'cls.id = survmap.object_id AND survmap.object_type = \'cre_sys_obt_cls\'');
		$selectCls->addExpression('COUNT(1)','total_enroll');
		$selectCls->condition('cls.status',array('lrn_cls_sts_atv','lrn_cls_sts_itv'), 'IN');
		$selectCls->condition('survmap.survey_id', $objectId, '=');
		expDebug::dPrintDBAPI('$selectCls', $selectCls);
		$classCount = $selectCls->execute()->fetchField();
		// Get Active TP Count
		$selectCrs =  db_select('slt_survey_mapping', 'survmap');
		$selectCrs->leftJoin('slt_program', 'prg', 'prg.id = survmap.object_id AND survmap.object_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
		$selectCrs->addExpression('COUNT(1)','totRec');
		$selectCrs->condition('prg.status',array('lrn_lpn_sts_atv','lrn_lpn_sts_itv'), 'IN');
		$selectCrs->condition('survmap.survey_id', $objectId, '=');
		expDebug::dPrintDBAPI('$selectCrs', $selectCrs);
		$tpCount = $selectCrs->execute()->fetchField();

		if ($classCount == 0 && $tpCount == 0 ){
			return 0;
		} else {
			return 1;
		}
		// Previous Code For Reference --> needs to Be Deleted In Code Refactoring
		/* $select = db_select('slt_survey_mapping', 'survmap');
		$select->addExpression('COUNT(1)','total_enroll');
		$select->condition('survmap.survey_id', $objectId, '=');
		// $select->condition('survmap.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv','lrn_crs_cmp_cmp'), 'IN');
		$total_enroll = $select->execute()->fetchField();
		return $total_enroll; */
	} catch (Exception $ex) {
		watchdog_exception('getSurveyAssessmentDeleteAccess', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getbannerDeleteAccess - Use this function to delete survey
 */

function getbannerDeleteAccess ($objectId){
	try{
  $select = db_select('slt_announcement_master', 'bann');
  $select->addExpression('COUNT(1)','total_enroll');
  $select->condition('bann.id', $objectId, '=');
  $select->condition('bann.status', 'cbn_anm_sts_atv', '=');
 // $select->condition('survmap.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv','lrn_crs_cmp_cmp'), 'IN');
  $total_enroll = $select->execute()->fetchField();
  return $total_enroll;
  } catch (Exception $ex) {
  	watchdog_exception('getbannerDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getAnnouncementDeleteAccess - Use this function to delete survey
*/

function getAnnouncementDeleteAccess ($objectId){
	try{
	$select = db_select('slt_site_notice', 'notice');
	$select->addExpression('COUNT(1)','total_announcement');
	$select->condition('notice.id', $objectId, '=');
	$select->condition('notice.status', 'cre_sys_obt_not_atv', '=');
	$total_announcement = $select->execute()->fetchField();
	return $total_announcement;
	} catch (Exception $ex) {
		watchdog_exception('getAnnouncementDeleteAccess', $ex);
		expertusErrorThrow($ex);
	}
}
// custom attribute check #custom_attribute_0078975
function getCustomAttributeDeleteAccess ($objectId){
    try{
        $select = db_select('slt_custom_attr', 'cusatt');
        $select->Join('slt_custom_attr_mapping', 'cusmap', 'cusmap.cattr_id=cusatt.id');
        $select->addExpression('COUNT(1)','total_customattribute');
        $select->condition('cusatt.id', $objectId, '=');
        //$select->condition('cusatt.status', 'cre_cattr_sts_del', '=');
        $select->condition('cusatt.status','cre_cattr_sts_del', '!=');
        expDebug::dPrintDBAPI('query for custom attribute', $select);

        $total_custom = $select->execute()->fetchField();
        if ($total_custom == 0){
            return 0;
        } else {
            return 1;
        }
        //return $total_announcement;
    } catch (Exception $ex) {
        watchdog_exception('getCustomAttributeDeleteAccess', $ex);
        expertusErrorThrow($ex);
    }
}
/*
 * getContentDeleteAccess - Use this function to delete survey
 */

function getContentDeleteAccess ($objectId){
	try{
  $usecount = false;
  $select = db_select('slt_course_content_mapper', 'map');
  $select->Join('slt_course_class', 'cls', 'cls.id=map.class_id');
  $select->Join('slt_enrollment', 'en', 'en.class_id=map.class_id');
  $select->addExpression('COUNT(1)','usecount');
  $select->condition('map.content_id', $objectId, '=');
  $select->condition('en.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl') ,'IN');
  $select->condition('cls.status', array('lrn_cls_sts_del','lrn_cls_sts_can','lrn_cls_sts_dld') ,'NOT IN');
  //have added the condtion for user moved to completed or not.
  $select->condition('en.comp_status', array('lrn_crs_cmp_cmp') ,'NOT IN');
  expDebug::dPrintDBAPI('query for con', $select);
  //0038524: Delete Option Issues-Content
  $usecountEntroll = $select->execute()->fetchField();

  $select = db_select('slt_course_content_mapper', 'map');
  $select->Join('slt_course_class', 'cls', 'cls.id=map.class_id');
  $select->addExpression('COUNT(1)','usecount');
  $select->condition('map.content_id', $objectId, '=');
  $select->condition('cls.status', array('lrn_cls_sts_del','lrn_cls_sts_can','lrn_cls_sts_dld') ,'NOT IN');
  $select->condition('cls.status', array('lrn_cls_sts_atv') ,'IN');
  $usecountClass = $select->execute()->fetchField();

  if(($usecountEntroll > 0) || ($usecountClass > 0)) {
    $usecount = true;
  }
  return $usecount;
  } catch (Exception $ex) {
  	watchdog_exception('getContentDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getQuestionEnrolledDeleteAccess - Use this function to delete question
 */
function getQuestionEnrolledDeleteAccess($objectId){
	try{
  $select = db_select('slt_survey_groups_questions', 'group_question');
  $select->addExpression('COUNT(1)','total_group_enroll');
  $select->condition('group_question.question_id', $objectId, '=');
  $select->Join('slt_survey', 'surmap', 'group_question.survey_id = surmap.id');
  $select->condition('surmap.status', 'sry_det_sry_del', '!=');
  expDebug::dPrintDBAPI('select', $select);
  $question_enrolled = $select->execute()->fetchField();

  return $question_enrolled;
  } catch (Exception $ex) {
  	watchdog_exception('getQuestionEnrolledDeleteAccess', $ex);
  	expertusErrorThrow($ex);
  }

}

/*
 * getCertificateDeteteAccess() - For Enable/Disable Delete Class
*/
function getCertificateDeteteAccess($certificateId){
	try{
		$select = db_select('slt_notification_info', 'snfty');
		$select->condition('snfty.notification_code', 'prncert');
		$select->condition('snfty.id', $certificateId);
		$select->addExpression('COUNT(1)', 'cnt');
		$isDefault = $select->execute()->fetchField();
		if($isDefault){
			return 1; //Should not allow to delete default certificate.
		}
		$selectMap = db_select('slt_entity_notification_mapping', 'senm');
		$selectMap->condition('senm.notify_id', $certificateId);
		$selectMap->addExpression('COUNT(1)', 'count');
		$result = $selectMap->execute()->fetchField();
		expDebug::dPrintDBAPI('getCertificateDeteteAccess $selectCode = ' , $selectMap);
		expDebug::dPrint('getCertificateDeteteAccess $selectCodeCnt = ' . print_r($result, true));
		return $result;

	}catch (Exception $ex) {
		watchdog_exception('getCertificateDeteteAccess', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getGroupsDeleteAccess() - For Enable/Disable Delete Class
*/
function getGroupsDeleteAccess($groupId){
	try{
		$select = db_select('slt_group_mapping', 'grpmap');
		$select->condition('grpmap.group_id', $groupId,'=');
	//	$select->condition('snfty.id', $certificateId);
		$select->addExpression('COUNT(1)', 'cnt');
		$groupsMapCnt = $select->execute()->fetchField();

		expDebug::dPrintDBAPI('getGroupsDeleteAccess $selectCode = ' , $select);
		expDebug::dPrint('getGroupsDeleteAccess $selectCodeCnt = ' . print_r($groupsMapCnt, true));
		/*if($groupsMapCnt==0){
			$Userquery = fetchUserListToGroup($groupId,'','count');
			$Usercount = $Userquery->execute()->fetchField();
			$groupsMapCnt = $Usercount>0 ? 1 : 0;
		}*/
		$defaultgrp      = fethDefaultGroup($groupId);//#59565
		if($defaultgrp == 1){
			return true;
		}
		if($groupsMapCnt >0){
			return true;
		}else{
			return false;
		}
		
		//return $groupsMapCnt;

	}catch (Exception $ex) {
		watchdog_exception('getGroupsDeleteAccess', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getContentTypeImageClass() - Returns the content type image based on the Object type.
 */
function getContentTypeImageClass($typecode){
	try {
  if($typecode == 'lrn_cnt_typ_aic' )
  	return "content-aicc";
  else if($typecode == 'lrn_cnt_typ_knc' )
  	return "content-knowledge-cont";
  else if($typecode == 'lrn_cnt_typ_srm_s12' )
  	return "content-scorm";
  else if($typecode == 'lrn_cnt_typ_srm_s24' )
  	return "content-scorm";
  else if($typecode == 'lrn_cnt_typ_vod' )
  	return "content-video";
  else if($typecode == 'lrn_cnt_typ_srm_tnc')
  	return "content-tincan";
  else
    return null;
  } catch (Exception $ex) {
  	watchdog_exception('getContentTypeImageClass', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * checkClassValidity() - If ILT or VC class, check whether the class has already commenced. If class has already commenced or there are no
 *                        future sessions set flag field in return object to 0, else set it to 1.
 *                        In the return object include information: tota sessions, past sessions, in progress sessions, future sessions.
 *
 *                        If WBT or VDO class, if content is attached to class, set flag field in return object to 1, else set it to 0.
 *                        In the return object return attached content count.
 *
 *                        Status of the class and the corresponding course to which the class belongs is also returned
 *                        in classstatus and coursestatus respectively.
 */
function checkClassValidity($classId, $deliveryType) {
	try {
		$retObj = new stdClass; //Init the object to be returned

		// Get class and course status for the class and save in $retObj
		$select = db_select('slt_course_class', 'cls');
		$select->leftJoin('slt_course_template', 'crs', 'cls.course_id = crs.id');
		$select->addField('cls', 'status', 'classstatus');
		$select->addField('crs', 'status', 'coursestatus');
		$select->condition('cls.id', $classId, '=');
	  expDebug::dPrintDBAPI('$select', $select);
	  $result = $select->execute()->fetchAll();
	  expDebug::dPrint('$result = ' . print_r($result, true) , 3);
	  $retObj->classstatus = $result[0]->classstatus;
	  $retObj->coursestatus = $result[0]->coursestatus;

	  if ($deliveryType == 'lrn_cls_dty_ilt' || $deliveryType == 'lrn_cls_dty_vcl') {
	  	// Find total session count, count of sessions already completed, count of sessions in progress, count of sessions in future

	  	// Get date, start_time, end_time for all the sessions of this class. For VC also get PHP timezone.
	  	$select = db_select('slt_course_class_session', 'ses');
	  	$select->condition('ses.class_id', $classId, '=');
	  	$select->addExpression('CONCAT(date(ses.start_date), \' \', ses.start_time, \':00\')', 'start_date_time');
	  	$select->addExpression('CONCAT(date(ses.start_date), \' \', ses.end_time, \':00\')', 'end_date_time');
	    $select->leftjoin('slt_profile_list_items', 'splitz', 'ses.timezone = splitz.code  AND ses.timezone IS NOT NULL  AND splitz.lang_code = \'cre_sys_lng_eng\'');
	    $select->addField('splitz', 'attr2', 'phptimezone');
	    expDebug::dPrintDBAPI('$select', $select);
	  	$sessionsList = $select->execute()->fetchAll();
	  	expDebug::dPrint('$sessionsList = ' . print_r($sessionsList, true) , 3);
	  	expDebug::dPrint('$session->phptimezone = ' . print_r($session->phptimezone, true) , 4);

	  	$retObj->totalSessions = count($sessionsList);
	    $retObj->pastSessions = 0;
	    $retObj->inProgressSessions = 0;
	    $retObj->futureSessions = 0;
      foreach ($sessionsList as $session) {
        if ($deliveryType == 'lrn_cls_dty_ilt' && empty($session->phptimezone)) {
          $curDateTimeUX = time();
	        $sesStartDateTimeUX = strtotime($session->start_date_time);
	        expDebug::dPrint('$sesStartDateTimeUX = ' . $sesStartDateTimeUX, 4);
	        $sesEndDateTimeUX = strtotime($session->end_date_time);
	        expDebug::dPrint('$sesEndDateTimeUX = ' . $sesEndDateTimeUX, 4);

	        if ($sesEndDateTimeUX <= $curDateTimeUX) {
	        	$retObj->pastSessions += 1;
	        }
	        else if ($sesStartDateTimeUX <= $curDateTimeUX && $sesEndDateTimeUX > $curDateTimeUX) {
	        	$retObj->inProgressSessions += 1;
	        }
	        else if ($sesStartDateTimeUX > $curDateTimeUX) {
	        	$retObj->futureSessions += 1;
	        }
        }
        else{
	        // Get current datetime in the VC timezone
	      	$sessionTZ = new DateTimeZone($session->phptimezone);
          $curDateTimeInSessionTZ = new DateTime(null, $sessionTZ);
          expDebug::dPrint('$curDateTimeInSessionTZ = ' . $curDateTimeInSessionTZ->format('Y-m-d H:i:sP'), 4);

	        // Convert session start datetime and end datetime to DateTime objects for comparing
          $sesStartDateTimeInSessionTZ = new DateTime($session->start_date_time, $sessionTZ);
          $sesEndDateTimeInSessionTZ = new DateTime($session->end_date_time, $sessionTZ);

	        // Compare the dates saving results in the return object
	        if ($sesEndDateTimeInSessionTZ <= $curDateTimeInSessionTZ) {
	          $retObj->pastSessions += 1;
	        }
	        else if ($sesStartDateTimeInSessionTZ <= $curDateTimeInSessionTZ && $sesEndDateTimeInSessionTZ > $curDateTimeInSessionTZ) {
	          $retObj->inProgressSessions += 1;
	        }
	        else if ($sesStartDateTimeInSessionTZ > $curDateTimeInSessionTZ) {
	          $retObj->futureSessions += 1;
	        }
        }

      }
	    // If class has already commenced or there are no future sessions in the class set flag field in return object to 0, else set it to 1.
	    $retObj->flag = (($retObj->pastSessions + $retObj->inProgressSessions) == 0 && $retObj->futureSessions > 0)? 1 : 0;
	  }
	  else if ($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod') {
	    $select = db_select('slt_course_content_mapper', 'map');
	    $select->addExpression('COUNT(map.id)', 'count');
	    $select->condition('map.class_id', $classId, '=');
	    expDebug::dPrintDBAPI('$select', $select);
	    $retObj->contentCount = $select->execute()->fetchField();
	    expDebug::dPrint('$retObj->contentCount = ' . $retObj->contentCount , 3);

	    // If content is attached to the class, set flag field in return object to 1, else set it to 0
	    $retObj->flag = ($retObj->contentCount > 0)? 1 : 0;
	  }
	  else {
	  	throw new Exception('Unknown class delivery type');
	  }

	  expDebug::dPrint('$retObj = ' . print_r($retObj, true), 4);
		return $retObj;
	}
	catch (Exception $ex) {
    watchdog_exception('checkClassValidity', $ex);
    expertusErrorThrow($ex);
	}
}

function getIfCourseValid($catalogId){
	try {
	$courseCount = db_query('select count(crs.id) as sesAvailable from slt_course_template crs
							join slt_course_class cls on
							crs.id = cls.course_id and crs.id = '.$catalogId)->fetchAll();

	return $courseCount;
	} catch (Exception $ex) {
		watchdog_exception('getIfCourseValid', $ex);
		expertusErrorThrow($ex);
	}
}
function getIfCourseCompliance($courseId){
	try {
		$iscompliance = db_query('select is_compliance from slt_course_template crs
							where
							crs.id = '.$courseId)->fetchField();

		return $iscompliance;
	} catch (Exception $ex) {
		watchdog_exception('getIfCourseValid', $ex);
		expertusErrorThrow($ex);
	}
}


function getIfClassSession($classId){
	try{
  $select = db_select('slt_course_class_session', 'ses');
  $select->addExpression('COUNT(1)','total_ses');
  $select->condition('ses.class_id', $classId, '=');
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getIfClassSession', $ex);
  	expertusErrorThrow($ex);
  }
}

function getIfProgramValid($programId){
	try{
	$courseCount = db_query('select count(prg.id) as sesAvailable from slt_program prg join slt_module_crs_mapping map on
							prg.id = map.program_id and prg.id = '.$programId.' where  (map.course_id <> \'\' and map.course_id is not null)')->fetchAll();
	return $courseCount;
	} catch (Exception $ex) {
		watchdog_exception('getIfProgramValid', $ex);
		expertusErrorThrow($ex);
	}
}

function ifClassCanBeCanceled($classId){
try{
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(1)','total_enroll');
  $select->condition('enroll.class_id', $classId, '=');
  $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
  // if user as Completed Status Means We wont allow Him To Delete the Class lrn_crs_cmp_cmp.
  //Changed for #0057290
  $select1= db_or();
  $select1 = $select1->condition('enroll.comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_cmp') ,'NOT IN')->isNull('enroll.comp_status');
  $select->condition($select1);

  expDebug::dPrintDBAPI('$select', $select);

  $total_enroll = $select->execute()->fetchField();

  if ($total_enroll== 0){
    return 0;
  } else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('ifClassCanBeCanceled', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * numClassDirectRegistrationEnrollments() - Returns number of direct enrollments to a class.
 */
function numClassDirectRegistrationEnrollments($classId) {
	try{
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(1)', 'count');
  $select->condition('enroll.class_id', $classId, '=');
  $select->condition('enroll.reg_status', array('lrn_crs_reg_wtl', 'lrn_crs_reg_ppm', 'lrn_crs_reg_cnf', 'lrn_crs_reg_ppv'), 'IN');
  $select->condition('enroll.comp_status', array('lrn_crs_cmp_cmp', 'lrn_crs_cmp_nsw') ,'NOT IN');
  $select->isNull('enroll.master_enrollment_id');

  expDebug::dPrintDBAPI('$numClassDirectRegistrationEnrollments', $select);

  $directEnrollCount = $select->execute()->fetchField();

  return $directEnrollCount;
  } catch (Exception $ex) {
  	watchdog_exception('numClassDirectRegistrationEnrollments', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * numClassTPRegistrationEnrollments() - Returns number of enrollments to a class done by enrolling to a training program
 */
function numClassTPRegistrationEnrollments($classId) {
	try{
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(1)', 'count');
  $select->condition('enroll.class_id', $classId, '=');
  $select->condition('enroll.reg_status', array('lrn_crs_reg_wtl', 'lrn_crs_reg_ppm', 'lrn_crs_reg_cnf', 'lrn_crs_reg_ppv'), 'IN');
  $select->condition('enroll.comp_status', array('lrn_crs_cmp_cmp', 'lrn_crs_cmp_nsw') ,'NOT IN');
  $select->isNotNull('enroll.master_enrollment_id');

  expDebug::dPrintDBAPI('$select', $select);

  $viaTPEnrollCount = $select->execute()->fetchField();

  return $viaTPEnrollCount;
  } catch (Exception $ex) {
  	watchdog_exception('numClassTPRegistrationEnrollments', $ex);
  	expertusErrorThrow($ex);
  }
}

function getIfClassBeDelete($courseId){
	try{
  $select = db_select('slt_course_template', 'crs');
  $select->join('slt_course_class','cls','crs.id = cls.course_id');
  $select->addExpression('COUNT(1)','total_class');
  $select->condition('crs.id', $courseId, '=');
  $select->condition('cls.status', 'lrn_cls_sts_del', '!=');
  expDebug::dPrintDBAPI('$select', $select);
  $total_class = $select->execute()->fetchField();
  if ($total_class== 0){
    return 0;
  } else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getIfClassBeDelete', $ex);
  	expertusErrorThrow($ex);
  }
}

function getIfCourseBeUnpublished($courseId){
 try {
  $select = db_select('slt_course_template', 'crs');
  $select->join('slt_course_class','cls','crs.id = cls.course_id');
  $select->addExpression('COUNT(1)','total_class');
  $select->condition('crs.id', $courseId, '=');
  $select->condition('cls.status', 'lrn_cls_sts_atv', '=');
  expDebug::dPrintDBAPI('$select', $select);
  $total_class = $select->execute()->fetchField();

  if ($total_class== 0){
    return 0;
  } else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getIfCourseBeUnpublished', $ex);
  	expertusErrorThrow($ex);
  }
}


function checkSessionContent($classId){
	try{
		$select = db_select('slt_course_class_session', 'ses');
  	$select->addExpression('COUNT(1)','total_ses');
  	$select->condition('ses.class_id', $classId, '=');
  	$sessionCnt = $select->execute()->fetchField();

  	$selectCont = db_select('slt_course_content_mapper', 'map');
  	$selectCont->addExpression('COUNT(1)','total_map');
  	$selectCont->condition('map.class_id', $classId, '=');
  	$contentCnt = $selectCont->execute()->fetchField();

  	if ($sessionCnt == 0 && $contentCnt == 0){
  		return 0;
  	}else{
  		return 1;
  	}
  	} catch (Exception $ex) {
  		watchdog_exception('checkSessionContent', $ex);
  		expertusErrorThrow($ex);
  	}
}

function checkIfOrgMatchesWithId($orgId, $orgName){
	try{
		$select = db_select('slt_organization', 'org');
  	$select->addField('org','name','name');
  	$select->condition('org.id', $orgId, '=');
  	$orgNameResult = $select->execute()->fetchField();
  	if($orgNameResult == $orgName){
  		expDebug::dPrint('yes yes yes');
  		return $orgId;
  	}else{
  		$select = db_select('slt_organization', 'org');
	  	$select->addField('org','id','id');
	  	$select->condition('org.name', $orgName, '=');
	  	$select->condition('org.status', 'cre_org_sts_act','=');
	  	$select->range(0,1);
	  	$orgIdResult = $select->execute()->fetchField();

	  	return $orgIdResult;
  	}
  	} catch (Exception $ex) {
  		watchdog_exception('checkIfOrgMatchesWithId', $ex);
  		expertusErrorThrow($ex);
  	}
}

function checkIfNameMatchesWithId($perId, $perName){
	try{
		$select = db_select('slt_person', 'pers');
  	$select->addField('pers','full_name','full_name');
  	$select->condition('pers.id', $perId, '=');
  	$perNameResult = $select->execute()->fetchField();
  	if($perNameResult == $perName){
  		return $perId;
  	}else{
  		$select = db_select('slt_person', 'pers');
	  	$select->addField('pers','id','id');
	  	$select->condition('pers.full_name', $perName, '=');
	  	$select->condition('pers.status', 'cre_usr_sts_atv','=');
	  	$select->range(0,1);
	  	$perIdResult = $select->execute()->fetchField();

	  	return $perIdResult;
  	}
  	} catch (Exception $ex) {
  		watchdog_exception('checkIfNameMatchesWithId', $ex);
  		expertusErrorThrow($ex);
  	}
}
/*
Function - getIfSurveyAssessmentValid - to check if the survey/assessment has questions
attached to it
*/
function getIfSurveyAssessmentValid($surAssessId){
	try{
		$selectCnt = db_select('slt_survey_groups_questions', 'quest');
  	$selectCnt->addExpression('COUNT(1)','total_sur_ass');
  	$selectCnt->condition('quest.survey_id', $surAssessId, '=');
  	$surCnt = $selectCnt->execute()->fetchField();

	if ($surCnt == 0){
  		return 0;
  	}else{
  		return 1;
  	}
  	} catch (Exception $ex) {
  		watchdog_exception('getIfSurveyAssessmentValid', $ex);
  		expertusErrorThrow($ex);
  	}
}

/*
 * addMultiselectDropDownToAddEditForm - Use this function to add a multiselect dropdown (select) to the addedit form
 */
function addPostLoadSingleSelectDropDownToAddEditForm(&$form, $classId, $formId, $ajaxTriggerName, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array()) {
	try{
	$initialAttributes = array();
	$wrapper = 'postload-dropdown-wrapper-'.$classId;
  $wraperClass= 'postload_ajax_dropdown_'.$classId;
	// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $initialAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $initialAttributes['data-wrapperid'] = array($wrapper);

  $initialAttributes['class'][]  = 'initial-addedit-edit-' . $classId;
  $initialAttributes['onclick'][]= 'handlePostLoadDropdown(\'postload-dropdown-wrapper-initial-'.$classId.'\',\'postload-dropdown-wrapper-' . $classId.'\');';
  $initialAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete
  $initialAttributes['class'][] = 'addedit-edit-dropdown';
  $initialAttributes['onchange'][] = 'textfieldTitleChange(this);';

  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $classId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  $attributes['onchange'][] = 'textfieldTitleChange(this);';
  // Add class to apply common style to all drop-downs
  $attributes['class'][] = 'addedit-edit-dropdown';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-dropdown';
    $initialAttributes['class'][] = 'addedit-readonly-dropdown';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
    $initialAttributes['tabindex'] = $tabIndex;
  }

  if ($mandatory) {
    $nullOption[] = t('LBL674');
  } else {
    $nullOption[] = t('LBL674');
  }
  //$finalOptions = array_merge($nullOption, $options);
  if(sizeOf($options)==1){
    $finalOptions = $options;
  }else{
    $finalOptions = $nullOption + $options;
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
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }



  $intialWrapper = 'postload-dropdown-wrapper-initial-'.$classId;
  $initialWraperClass= 'postload_ajax_dropdown_inital-'.$classId;

  //if(empty($defaultOption)){
    expDebug::dPrint('Inside plugin default value:'.$defaultOption , 4);
  if(sizeOf($options)<=1){
	  $form[$initialWraperClass]['wrapper'] = array(
	    '#prefix' => '<div id="'.$intialWrapper.'" >',
	    '#suffix' => '</div>',
	  );
  }else{
  	$form[$initialWraperClass]['wrapper'] = array(
	    '#prefix' => '<div id="'.$intialWrapper.'" style="display:none;">',
	    '#suffix' => '</div>',
	  );
  }

  $form[$initialWraperClass]['wrapper'][$formId] = array(
    '#type' => 'select',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#options'=> $finalOptions,
    '#default_value' => $defaultOption,
    '#disabled' => $readOnly,
    '#attributes' => $initialAttributes,
    '#validated' => true,
  	'#ajax' => array(
      'event'=>'click',
      'callback' =>'ajax_post_load_callback',
      'wrapper' => $wrapper,
      //'method'=>'replace',
      $ajaxTriggerName =>true,
      'wrapperClass'=>$wraperClass,
      'progress' => array(
        'type' => 'throbber',
        'message' => ''
      ),
    ),
  );
  //}
   expDebug::dPrint('Inside plugin default sizeOF value 222 :'.$classId.'=>'.sizeOf($options) , 4);
   if(sizeOf($options)<=1){
	  $form[$wraperClass]['wrapper'] = array(
	    '#prefix' => '<div id="'.$wrapper.'" style="display:none;">',
	    '#suffix' => '</div>',
	  );
  }else{
  	$form[$wraperClass]['wrapper'] = array(
	    '#prefix' => '<div id="'.$wrapper.'" >',
	    '#suffix' => '</div>',
	  );
  }

  //$defaultOption = array('is_instructor', 'is_manager');
  $form[$wraperClass]['wrapper'][$formId] = array(
    '#type' => 'select',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#options'=> $finalOptions,
    '#default_value' => $defaultOption,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#validated' => true
  );

  $replacedId = str_replace('_', '-', $classId);
  if(sizeOf($options)<=1){
    expDebug::dPrint('Inside plugin default second value:'.$defaultOption , 4);
    $form['load_select_drop_down_' . $classId] = array (
  		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="handlePostLoadDropdown(\'postload-dropdown-wrapper-' . $classId.'\',\'postload-dropdown-wrapper-initial-'.$classId.'\'); textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
    );
  }else{
    $form['load_select_drop_down_' . $classId] = array (
  		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="handlePostLoadDropdown(\'postload-dropdown-wrapper-initial-' . $classId.'\',\'postload-dropdown-wrapper-'.$classId.'\'); textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
    );
  }
  } catch (Exception $ex) {
  	watchdog_exception('addPostLoadSingleSelectDropDownToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * Function for handling ajax request
 */
function ajax_post_load_callback(&$form, $form_state) {
	try{
  $wrapperClass=$form_state['triggering_element']['#ajax']['wrapperClass'];
  return $form[$wrapperClass]['wrapper'];
  } catch (Exception $ex) {
  	watchdog_exception('ajax_post_load_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

/* admin side time zone without time conversion
 * added for the ticket no:0021181: VC Time zone for Admin catalog screen */
function getSessionDetailsAdmin($courseid,$classid,$delType,$timeFor="") {
	try {
		$selectSess = db_select('slt_course_class_session', 'sess');
  $selectSess->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
  //$selectSess->leftjoin('slt_facility', 'splt_fac', 'splt_fac.id = sess.facility_id');
  $selectSess->leftjoin('slt_location', 'splt_loc', 'splt_loc.id = sess.location_id');
  $selectSess->leftjoin('slt_profile_list_items', 'loc_timezone', 'loc_timezone.code=splt_loc.timezone');
  $selectSess->addField('sess','id', 'session_id');
  $selectSess->addField('sess','start_date', 'session_start');
  $selectSess->addField('sess','end_date', 'session_end');
  $selectSess->addField('sess','start_time', 'session_start_time');
  $selectSess->addField('sess','end_time', 'session_end_time');
  $selectSess->addField('sess','title', 'session_title');
  $selectSess->addField('sess','course_id', 'course_id');
  $selectSess->addField('sess','class_id', 'class_id');
  $selectSess->addField('splt_timezone','attr2', 'session_timezone');
  $selectSess->addField('splt_timezone','name', 'session_fulltimezone');
  $selectSess->addField('loc_timezone','attr2', 'location_timezone');
  $selectSess->addField('loc_timezone','name', 'location_fulltimezone');
  $selectSess->addField('splt_loc','name', 'name');
  $selectSess->addField('splt_loc','addr1', 'contact_addr1');
  $selectSess->addField('splt_loc','addr2', 'contact_addr2');
  //$selectSess->addField('splt_loc','country', 'contact_country');
  //$selectSess->addField('splt_loc','state', 'contact_state');
  $selectSess->addExpression('(select country_name from slt_country where country_code = splt_loc.country)', 'contact_country');
  $selectSess->addExpression('(select state_name from slt_state where state_code = splt_loc.state and country_code =  splt_loc.country)', 'contact_state');
  $selectSess->addField('splt_loc','city', 'contact_city');
  $selectSess->addField('splt_loc','zipcode', 'contact_zipcode');
  $selectSess->condition('sess.class_id', $classid,'=');
  $selectSess->condition('sess.course_id', $courseid,'=');
  $selectSess->orderBy('sess.start_date', 'asc');

  $sessQryRes = $selectSess->execute()->fetchAll();

  $sessionDetails = array();
  foreach ($sessQryRes as $record) {

       $ses_start_date_form     = explode(" ",$record->session_start);
   	   $ses_start_date_format   = $ses_start_date_form[0].$record->session_start_time;
   	   $ses_end_date_form       = explode(" ",$record->session_start);
   	   $ses_end_date_format     = $ses_end_date_form[0].$record->session_end_time;

   	   $sessionDetails[] = array(
     	  	'session_id'                 => $record->session_id,
     		'courseid'                   => $courseid,
     		'classid'                    => $classid,
     		'facility_id'				 => $record->facility_id,
            'session_start'              => $session_start,
            'session_start_day'			 => $session_day,
            'session_end'                => $record->session_end,
     	    'session_start_time'         => $start_time,
     	    'session_end_time'           => $end_time,
            'session_title'				 => $record->session_title,
            'session_start_format'       => ($timeFor == "enroll") ? date_format(date_create($ses_start_date_format),'M d, Y g:i') : date_format(date_create($ses_start_date_format),'M d, Y'),
     	    'session_end_format'         => date_format(date_create($ses_end_date_format),'M d, Y g:i'),
     		'session_start_date_format'  => date_format(date_create($ses_start_date_format),'M d, Y'),
     	    'session_end_date_format'    => date_format(date_create($ses_end_date_format),'M d, Y'),
     	    'session_start_time_format'  => date_format(date_create($ses_start_date_format),'g:i'),
     	    'session_start_end_format'   => date_format(date_create($ses_end_date_format),'g:i'),
     	    'session_start_time_form'    => date_format(date_create($ses_start_date_format),'A'),
     	    'session_end_time_form'      => date_format(date_create($ses_end_date_format),'A'),
            'session_name'			 	 => $record->name,
     		'session_address1'			 => $record->contact_addr1,
    		'session_address2'			 => $record->contact_addr2,
     		'session_country'			 => $record->contact_country,
     		'session_state'			 	 => $record->contact_state,
     		'session_city'			     => $record->contact_city,
     		'session_zipcode'			 => $record->contact_zipcode,
            'sess_timezone'				 => ($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone,
     		'sess_fulltimezone'			 => ($delType == 'lrn_cls_dty_vcl') ? $record->session_fulltimezone : $record->location_fulltimezone
     	   );
  }
		return $sessionDetails;
	}catch (Exception $ex) {
    watchdog_exception('getSessionDetailsAdmin', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * get Groups userlist
 */
function getRoleUserListField($roleId) {
	try{
	expDebug::dPrint('$getRoleUserListField = ' . $roleId,4);
  // Select slt_security_role table
  $select = db_select('slt_groups', 'grp');

  // Get count of active users
  $select->addField('grp','userslist', 'userslist');

  // Set the conditions
 	// $select->condition('role.status', 'cre_sec_sts_atv', '=');
  $select->condition('grp.id', $roleId, '=');

  expDebug::dPrintDBAPI('getRoleUserListField $select obj = ' , $select);

  // Execure the query
  $userList = $select->execute()->fetchField();
  expDebug::dPrint('$userList = ' . $userList,4);

  return $userList;
  } catch (Exception $ex) {
  	watchdog_exception('getRoleUserListField', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Redirects to the first admin page of the user. Menu callback of ?q=administration
 */
function redirectToFirstAdminScreen() {
	try{
		expDebug::dPrint("menus link ".print_R("ss",true),5);
  if (user_access('Administration Perm')) {
    $firstAdminLink = getParentAdminLinks(true);
    drupal_goto($firstAdminLink);
  }
  } catch (Exception $ex) {
  	watchdog_exception('redirectToFirstAdminScreen', $ex);
  	expertusErrorThrow($ex);
  }
}

function view_form_display($formInfoList = array(), $argsList = array(), $otherArgsList = array()) {
	try {
		expDebug::dPrint('$$argsList 1111111t---->'.print_r($argsList,true),5);
		if (empty($formInfoList)) {
			throw new Exception('exp_sp_administration.inc : exp_ctools_modal_multi_ajax_forms_display() : $formIdList is empty.');
		}
		ctools_include('modal');
		ctools_include('ajax');

		// Prepare title for the modal dialog
		$title = empty($formInfoList[0]['title']) ? drupal_get_title() : $formInfoList[0]['title'];

		$renderMode = 'EDIT';
		if (empty($argsList)) {
			$renderMode = 'ADD';
		}

		$output = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, $renderMode, $otherArgsList);

		$commands = array();

		// If the form has not yet been rendered, render it.
		$commands[] = ctools_modal_command_display($title, $output);
		$commands[] = exp_destroy_ckeditors_on_modal_close();
		$commands[] = callCourseDetailWidget();
		$commands[] = ajax_command_invoke(NULL, "scrollView", array($argsList[0]));
		expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

		print ajax_render($commands);
		exit;
	} catch (Exception $ex) {
		watchdog_exception('exp_ctools_modal_multi_ajaxonly_forms_display', $ex);
		expertusErrorThrow($ex);
	}
}
function callCourseDetailWidget(){
	try{
		return array(
				'command' => 'callCourseDetailWidget',
			);

	}catch (Exception $ex) {
		watchdog_exception('callCourseDetailWidget', $ex);
		expertusErrorThrow($ex);
	}
}
function blank_page(){
	$html = '<div class="narrow-search-no-records no-priv">'.t('ERR039').'</div>';
	return $html;
}

/**
 * @author : Ramkumar
 * @desc : updateTags is for Update the Tags Details after the entity_type is Deleted.
 * @param $objectId
 * @param $objectType
 */
function updateTags($objectId,$objectType) {
	try{
		expDebug::dPrint('$objectId : '.$objectId.' $objectType : '.$objectType, 4);
		// select The activity count
		$selectStmt = db_select('slt_tag_entity', 'tagentity');
		$selectStmt->leftJoin('slt_tagdefn', 'td', 'tagentity.tagid = td.id');
		//check for Tp .
		if($objectType =='TP'){
			$selectStmt->condition('tagentity.entity_type', array('Certification','Learning Plan','Curricula') ,'IN');
		}
		else{
			$selectStmt->condition('tagentity.entity_type', $objectType,'=');
		}
		$selectStmt->condition('tagentity.entity_id', $objectId,'=');
		$selectStmt->addField('td','id', 'tagid');
		$selectStmt->addField('td','activity_count', 'activity_count');
		expDebug::dPrintDBAPI('updateTags $selectStmt = ' , $selectStmt);
		// Execute the query
		$actvityCount = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('$actvityCount = ' . print_r($actvityCount, true),4);
		foreach($actvityCount as $record)
		{
			expDebug::dPrint('$record = ' . print_r($record->tagid, true),4);
			
			// Get the fields before deleting
			$loggeduserId = getSltpersonUserId();
			$select = db_select('slt_tag_entity', 'tagent');
			$select->addField('tagent','id','id');
			$select->addField('tagent','tagid','tagid');
			$select->addField('tagent','entity_id','entity_id');
			$select->addField('tagent','entity_type','entity_type');
			if($objectType !='TP'){
				$select->condition('entity_type', $objectType,'=');
			}else{
				$select->condition('entity_type', array('Certification','Learning Plan','Curricula') ,'IN');
			}
			$select->condition('entity_id', $objectId,'=');
			$tagEntInfo =$select->execute()->fetchAll();

			// delete the Tag Entity
			$deleteStmt = db_delete('slt_tag_entity');
			if($objectType !='TP'){
				$deleteStmt->condition('entity_type', $objectType,'=');
			}else{
				$deleteStmt->condition('entity_type', array('Certification','Learning Plan','Curricula') ,'IN');
			}
			$deleteStmt->condition('entity_id', $objectId,'=');
			expDebug::dPrintDBAPI('$deleteStmt = ' , $deleteStmt);
			$deleteStmt->execute();

			foreach ($tagEntInfo as $val){
			    // Make deleted log entry
			    $fields = array(
			            'table_name' => 'slt_tag_entity',
			            'entity_id' => $val->id,
			            'entity_type' => 'tag_entity',
			            'parent1_entity_id' => $val->entity_id,
			            'parent1_entity_type' => $val->entity_type,
			            'parent2_entity_id' => $val->tagid,
			            'parent2_entity_type' => 'tagid',
			            'module_name' => 'exp_sp_administration',
			            'functionality_name' => 'updateTags',
			            'api_name' => $_REQUEST['apiname'],
			            'deleted_on' => now(),
			            'deleted_by' => $loggeduserId
			    );
			    deleted_log_entry($fields);
			}
			
			
			// Update the Tag definition
			$updStmt = db_update('slt_tagdefn');
			$fields = array(
					'activity_count'     => $record->activity_count-1,
			);
			$updStmt->fields($fields);
			$updStmt->condition('id',$record->tagid,'=');
			expDebug::dPrintDBAPI('$updStmt = ' , $updStmt);
			$tagDefIdUpd = $updStmt->execute();
		}
	} catch (Exception $ex) {
		watchdog_exception('updateTags', $ex);
		expertusErrorThrow($ex);
	}
}

function updateTagActivityCount($entity_id,$entity_type,$trigger_element,$callFromAPI='')
{
		$selectStmt = db_select('slt_tag_entity','slt_etag');
      	$selectStmt->addExpression('tagid','tagid');
      	$selectStmt->condition('slt_etag.entity_id',$entity_id,'=');
		$selectStmt->condition('slt_etag.entity_type',$entity_type,'=');
		expDebug::dPrintDBAPI("Course-Update tag activity Count POPUP",$selectStmt,5);
      	$tag_count_entity = $selectStmt->execute()->fetchCol();
		expDebug::dPrint('Course-Update tag activity popup-->'.print_r($tag_count_entity,true). '---'.count($tag_count_entity).'---'.$trigger_element,5);
		$activestatuscodes = array("Coursesaveandpublish", "saveandpublish", "lrn_cls_sts_atv", "lrn_crs_sts_atv", "lrn_lpn_sts_atv", "sry_qtn_sts_atv","saveandshow","lrn_cnt_sts_atv");
		$deactivestatuscodes = array("Courseunpublish", "unpublish", "lrn_cls_sts_itv", "lrn_crs_sts_itv", "lrn_lpn_sts_itv", "sry_qtn_sts_itv","hidecatalog","lrn_cnt_sts_itv");

		if(count($tag_count_entity) > 0){
			$checkCourseClassTpExists = checkCourseClassTpExists($entity_id,$entity_type);
			expDebug::dPrint('Course-Update tag activity $checkCourseClassTpExists-->'.$callFromAPI.'----'.$checkCourseClassTpExists.'---'.print_r($activestatuscodes,true).'---'.print_r($deactivestatuscodes,true),5);
			
		if (($callFromAPI=='' && in_array($trigger_element, $activestatuscodes)) || ($callFromAPI==1 && in_array($checkCourseClassTpExists, $deactivestatuscodes) && in_array($trigger_element, $activestatuscodes)))
  		{
  			$updStmt = db_update('slt_tagdefn');
			$updStmt->expression('activity_count', 'activity_count + 1');
			$updStmt->condition('id', $tag_count_entity, 'IN');
			$tagDefIdUpd = $updStmt->execute();
			 expDebug::dPrintDBAPI("Course-Update sql popup1111 >>>>>>>>>." , $updStmt);
		}elseif(($callFromAPI=='' && in_array($trigger_element, $deactivestatuscodes)) || ($callFromAPI==1 && in_array($checkCourseClassTpExists, $activestatuscodes) && in_array($trigger_element, $deactivestatuscodes))){
			$updStmt = db_update('slt_tagdefn');
			$updStmt->expression('activity_count', 'activity_count - 1');
			$updStmt->condition('activity_count','0','>');
			$updStmt->condition('id', $tag_count_entity, 'IN');
			$tagDefIdUpd = $updStmt->execute();
			 expDebug::dPrintDBAPI("Course-Update sql popup2222 >>>>>>>>>." , $updStmt);
		}
	}
}
function updateGroupMappingDetails($entityId,$entityType, $creatorId,$action,$group_id,$clonefrom=array(),&$statements=array()){
	expDebug::dPrint('updateGroupMappingDetails INSIDE ' . print_r($entityId,1),4);
	expDebug::dPrint('updateGroupMappingDetails $entityType ' . print_r($entityType,1),4);
	expDebug::dPrint('updateGroupMappingDetails $$creatorId ' . print_r($creatorId,1),4);
	expDebug::dPrint('updateGroupMappingDetails $$$action ' . print_r($action,1),4);
	expDebug::dPrint('updateGroupMappingDetails $$$$group_id ' . print_r($group_id,1),4);
	expDebug::dPrint('updateGroupMappingDetails $$$$$clonefrom ' . print_r($clonefrom,1),4);
	if($action == 'update'){
		
		$update = db_update('slt_mapped_entities','map');
		$fieldsArr= array(
				'group_id' => $group_id,
				'updated_by' => $creatorId,
				'updated_on' => now()
		);
		$update->fields($fieldsArr);
		$update->condition('group_id','0','!=');
		$update->condition('group_id','-1','!=');
		$update->condition('map','entity_id', $entityId);
		$update->condition('map','entity_type', $entityType);
		expDebug::dPrintDBAPI('updateGroupMappingDetails Update group attached ' , $update);
		//$update->execute();
		if(!empty($statements)) {
			$statements[] = $update;
		} else {
			$update->execute();
		}
		
	}elseif($action == 'emptyadd'){
		
			$insObj  = db_insert('slt_mapped_entities');
			$fieldsArr= array(
					'entity_id' => $entityId,
					'entity_type' => $entityType,
					'group_id' => 0,
					'owner_id' => $creatorId,
					'created_by' => $creatorId,
					'created_on' => now(),
					'updated_by' => $creatorId,
					'updated_on' => now()
			);
			$insObj->fields($fieldsArr);
			expDebug::dPrintDBAPI('updateGroupMappingDetails addupdate group attached44 ' , $insObj);
			// $insObj->execute();
			if(!empty($statements)) {
				$statements[] = $insObj;
			} else {
				$insObj->execute();
			}
			 
	}elseif($action == 'emptydelete'){
	    // Get the fields before deleting
	    $loggeduserId = getSltpersonUserId();
	    $select = db_select('slt_mapped_entities', 'mapent');
	    $select->addField('mapent','id','id');
	    $select->addField('mapent','entity_id','entity_id');
	    $select->addField('mapent','entity_type','entity_type');
	    $select->condition('entity_id',$entityId);
	    $select->condition('entity_type',$entityType);
	    $mapEntId =$select->execute()->fetchObject();
		
		$delQry = db_delete('slt_mapped_entities');
		$delQry->condition('entity_id',$entityId,'=');
		$delQry->condition('entity_type',$entityType, '=');
		expDebug::dPrintDBAPI('updateGroupMappingDetails delete emptydelete attached yoga ' , $delQry);
		//$delQry->execute();
		if(!empty($statements)) {
			$statements[] = $delQry;
		} else {
			$delQry->execute();
		}
			 
		// Make deleted log entry
		$fields = array(
		        'table_name' => 'slt_mapped_entities',
		        'entity_id' => $mapEntId->id,
		        'entity_type' => 'mapped_entity',
		        'parent1_entity_id' => $mapEntId->entity_id,
		        'parent1_entity_type' => $mapEntId->entity_type,
		        'module_name' => 'exp_sp_administration',
		        'functionality_name' => 'updateGroupMappingDetails',
		        'api_name' => $_REQUEST['apiname'],
		        'deleted_on' => now(),
		        'deleted_by' => $loggeduserId
		);
		deleted_log_entry($fields,$statements);
			 
	}elseif($action == 'add'){
			
			$insObj  = db_insert('slt_mapped_entities');
			$insObj->fields(array(
				'entity_id' => $entityId,
				'entity_type' => $entityType,
				'group_id' => $group_id,
				'created_by' => $creatorId,
				'created_on' => now(),
				'updated_by' => $creatorId,
				'updated_on' => now()
				));
			
			expDebug::dPrintDBAPI(" updateGroupMappingDetails--->>>>>>>>>." , $insObj);
			//$insObj->execute();
			if(!empty($statements)) {
				$statements[] = $insObj;
			} else {
				$insObj->execute();
			}
			
			$update = db_update('slt_mapped_entities');
			$fieldsArr= array(
					'group_id' => -1,
			);
			$update->fields($fieldsArr);
			$update->condition('group_id', '0','=');
			$update->condition('entity_id', $entityId);
			$update->condition('entity_type', $entityType);
			expDebug::dPrintDBAPI('updateGroupMappingDetails Update group attached ' , $update);
			//$update->execute();
			if(!empty($statements)) {
				$statements[] = $update;
			} else {
				$update->execute();
			}
			
	}elseif ($action == 'addupdate'){
		
		$select = db_select('slt_mapped_entities', 'sme');
		$select->addField('sme','id');
		$select->condition('sme.group_id', 0,'=');
		$select->condition('entity_id',$entityId,'=');
		$select->condition('entity_type',$entityType, '=');
		$orderCount = $select->execute()->rowCount();
		if($orderCount>0){
			$update = db_update('slt_mapped_entities');
			$fieldsArr= array(
					'group_id' => -1,
			);
			$update->fields($fieldsArr);
			$update->condition('group_id', '0','=');
			$update->condition('entity_id', $entityId);
			$update->condition('entity_type', $entityType);
			expDebug::dPrintDBAPI('updateGroupMappingDetails addupdate group attached11 ' , $update);
			//$update->execute();
			if(!empty($statements)) {
				$statements[] = $update;
			} else {
				$update->execute();
			}
		}else{
			
			$insObj  = db_insert('slt_mapped_entities');
			$fieldsArr= array(
					'entity_id' => $entityId,
					'entity_type' => $entityType,
					'group_id' => -1,
					'owner_id' => $creatorId,
					'created_by' => $creatorId,
					'created_on' => now()
			);
			$insObj->fields($fieldsArr);
			expDebug::dPrintDBAPI('updateGroupMappingDetails addupdate group attached22 ' , $insObj);
			// $insObj->execute();
			if(!empty($statements)) {
				$statements[] = $insObj;
			} else {
				$insObj->execute();
			}
		}
		foreach ($group_id as $k=>$value){
			$fieldsArr = array();
			$insObj  = db_insert('slt_mapped_entities');
			$fieldsArr= array(
					'entity_id' => $entityId,
					'entity_type' => $entityType,
					'group_id' => $value->group_id,
					'created_by' => $creatorId,
					'created_on' => $value->created_on,
					'updated_by' => $value->updated_by,
					'updated_on' => $value->updated_on
			);
			$insObj->fields($fieldsArr);
			expDebug::dPrintDBAPI('updateGroupMappingDetails addupdate group attached33 ' , $insObj);
			//$insObj->execute();
			if(!empty($statements)) {
				$statements[] = $insObj;
			} else {
				$insObj->execute();
			}
		}
	}elseif($action == 'delete'){
			// Get the fields before deleting
			$loggeduserId = getSltpersonUserId();
		
			$select = db_select('slt_mapped_entities', 'mapent');
			$select->addField('mapent','id','id');
			$select->addField('mapent','entity_id','entity_id');
			$select->addField('mapent','entity_type','entity_type');
			$select->condition('group_id','0','!=');
	        $select->condition('group_id','-1','!=');
	        $select->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
			$select->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
			if(!empty($group_id))
			    $select->condition('group_id',$group_id,is_array($group_id) ? 'IN' : '=');
			$mapEntId =$select->execute()->fetchAll();
			
			$delQry = db_delete('slt_mapped_entities');
			$delQry->condition('group_id','0','!=');
			$delQry->condition('group_id','-1','!=');
			$delQry->condition('entity_id',$entityId,is_array($entityId) ? 'IN' : '=');
			$delQry->condition('entity_type',$entityType,is_array($entityType) ? 'IN' : '=');
			if(!empty($group_id))
			$delQry->condition('group_id',$group_id,is_array($group_id) ? 'IN' : '=');
			expDebug::dPrintDBAPI('updateGroupMappingDetails delete group attached yoga ' , $delQry);
			//$delQry->execute();
			if(!empty($statements)) {
				$statements[] = $delQry;
			} else {
				$delQry->execute();
			}
			
			foreach($mapEntId as $val){
			    // Make deleted log entry
			    $fields = array(
			            'table_name' => 'slt_mapped_entities',
			            'entity_id' => $val->id,
			            'entity_type' => 'mapped_entity',
			            'parent1_entity_id' => $val->entity_id,
			            'parent1_entity_type' => $val->entity_type,
			            'module_name' => 'exp_sp_administration',
			            'functionality_name' => 'updateGroupMappingDetails',
			            'api_name' => $_REQUEST['apiname'],
			            'deleted_on' => now(),
			            'deleted_by' => $loggeduserId
			    );
			    deleted_log_entry($fields,$statements);
			}
			
			$update = db_update('slt_mapped_entities');
			$fieldsArr= array(
					'group_id' => 0,
			);
			$update->fields($fieldsArr);
			$update->condition('group_id', '-1','=');
			$update->condition('entity_id', $entityId);
			$update->condition('entity_type', $entityType);
			expDebug::dPrintDBAPI('updateGroupMappingDetails Update group attached ' , $update);
			//$update->execute();
			if(!empty($statements)) {
				$statements[] = $update;
			} else {
				$update->execute();
			}
			
	}elseif($action == "suspendupdate" || $action == "deletegroup" ){
		$select = db_select('slt_mapped_entities', 'sme');
		$select->addField('sme','entity_id');
		$select->addField('sme', 'entity_type');
		$select->condition('group_id',$group_id,'=');
		expDebug::dPrintDBAPI('queryres ' , $select);
		$QryRes = $select->execute()->fetchAll();
		expDebug::dPrint('associated entities ' . print_r($QryRes,1),4);
		foreach($QryRes as $entity){
			$select = db_select('slt_mapped_entities', 'sme');
			$select->addField('sme','group_id');
			$select->condition('group_id','-1','!=');
			$select->condition('group_id','0','!=');
			$select->condition('sme.entity_id',$entity->entity_id,'=');
			$select->condition('entity_type',$entity->entity_type, '=');
			expDebug::dPrintDBAPI('associated groups ' , $select);
			$orderCount = $select->execute()->rowCount();
			if($orderCount == 1){
				$update = db_update('slt_mapped_entities');
				$fieldsArr= array(
						'group_id' => 0,
				);
				$update->fields($fieldsArr);
				$update->condition('group_id','-1','=');
				$update->condition('entity_id', $entity->entity_id);
				$update->condition('entity_type',$entity->entity_type);
				expDebug::dPrintDBAPI('updateGroupMappingDetails Update group attached ' , $update);
				//$update->execute();
				if(!empty($statements)) {
					$statements[] = $update;
				} else {
					$update->execute();
				}
			}
		}
		
		// Get the fields before deleting
		$loggeduserId = getSltpersonUserId();
		$select = db_select('slt_mapped_entities', 'mapent');
		$select->addField('mapent','id','id');
		$select->addField('mapent','entity_id','entity_id');
		$select->addField('mapent','entity_type','entity_type');
		$select->condition('group_id',$group_id);
		$mapEntId =$select->execute()->fetchObject();
		
		$delQry = db_delete('slt_mapped_entities');
		$delQry->condition('group_id',$group_id,'=');
		expDebug::dPrintDBAPI('updateGroupMappingDetails delete suspended groupmapping ' , $delQry);
		//$delQry->execute();
		if(!empty($statements)) {
			$statements[] = $delQry;
		} else {
			$delQry->execute();
		}
		expDebug::dPrint("delete qry".print_r($mapEntId,true),5);
		if(!empty($mapEntId) && count($mapEntId) > 0){
			// Make deleted log entry
			$fields = array(
			        'table_name' => 'slt_mapped_entities',
			        'entity_id' => $mapEntId->id,
			        'entity_type' => 'mapped_entity',
			        'parent1_entity_id' => $mapEntId->entity_id,
			        'parent1_entity_type' => $mapEntId->entity_type,
			        'module_name' => 'exp_sp_administration',
			        'functionality_name' => 'updateGroupMappingDetails',
			        'api_name' => $_REQUEST['apiname'],
			        'deleted_on' => now(),
			        'deleted_by' => $loggeduserId
			);
			deleted_log_entry($fields,$statements);
		}
	}elseif($action == 'bulkadd'){
		$mapFields = array('entity_type', 'group_id');
			
		$mapquery = db_select('slt_mapped_entities', 'map');
		$mapquery->fields('map', $mapFields);
		$mapquery->addExpression($entityId, 'entity_id');
		$mapquery->addExpression('if(group_id = -1,' . $creatorId . ',0)', 'owner_id');
		$mapquery->addExpression($creatorId, 'created_by');
		$mapquery->addExpression('now()', 'created_on');
		$mapquery->addExpression($creatorId, 'updated_by');
		$mapquery->addExpression('now()', 'updated_on');
		$mapquery->condition('map.entity_id', $clonefrom['entity_id'], '=');
		$mapquery->condition('map.entity_type', $clonefrom['entity_type'], '=');
		expDebug::dPrintDBAPI('Catalog mapping clone query', $mapquery);
		
	/* 	db_insert('slt_mapped_entities')
		->from($mapquery)
		->execute(); */
		$insert_stat = db_insert('slt_mapped_entities')
		->from($mapquery);
		if(!empty($statements)) {
			$statements[] = $insert_stat;
		} else {
			$insert_stat->execute();
		}
	}
	
}
/**
 * return admin group or not.
 */
function adminGrporNot($grpId){

	$select = db_select('slt_groups','grp');
	$select->addField('grp','is_admin','is_admin');
	$select->condition('grp.id',$grpId,'=');
	expDebug::dPrintDBAPI('Fetch role Id ' , $select);
	$is_admin = $select->execute()->fetchField();
	return $is_admin;
}
function getGrpDetails($roleId, $fields) {
	try{
		if (empty($fields)) {
			// Get all fields
			$fields = array('id', 'name', 'status','access_permissions','userslist',
					'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
		}

		// Select table
		$selectStmt = db_select('slt_groups', 'grp');

		// Set conditions
		$selectStmt->condition('grp.id', $roleId);

		// Select fields and/or add expressions
		foreach($fields as $fieldName) {
			$selectStmt->addField('grp', $fieldName, $fieldName);
		}

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();

		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getGrpDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function checkCourseClassTpExists($entityId,$entityType,$status)
{
  try {
  	 if($entityType == 'Course'){
  	 	 $query = db_select('slt_course_template', 'slt_db');
		 $status_type = 'lrn_crs_sts_atv';
  	 }elseif($entityType == 'Learning Plan'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Certification'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Curricula'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Class'){
  	 	 $query = db_select('slt_course_class', 'slt_db');
		 $status_type = 'lrn_cls_sts_atv';
  	 }elseif($entityType == 'sry_det_typ_sry_qus'){
  	 	 $query = db_select('slt_survey_questions', 'slt_db');
		 $status_type = 'sry_qtn_sts_atv';
  	 }elseif($entityType == 'sry_det_typ_ass_qus'){
  	 	 $query = db_select('slt_survey_questions', 'slt_db');
		 $status_type = 'sry_qtn_sts_atv';
  	 }elseif($entityType == 'Content'){
  	 	 $query = db_select('slt_content_master', 'slt_db');
		 $status_type = 'lrn_cnt_sts_atv';
  	 }
	 	 
	 
	  $query->addField('slt_db','status','recdstatus');
	  $query->condition('slt_db.id', $entityId);
	  expDebug::dPrintDBAPI("checkCourseClassTpExists sql >>>>>>>>>." , $query);
	  return $query->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('checkCourseClassTpExists', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Populate the users who blongs to a group into the flat table
 * 
 * This flat table will be used to filter the records for the logged in user
 * Earlier the filter will be achived by joining group table with person table
 * that will causes the system performance when there are more number of users
 * and group. Because there is no direct mapping to both the tables.
 * Hence this new flat table will be added to keep the users list with their 
 * respective groups.  
 * 
 * @param $grpId
 * @return unknown_type
 */
function populateGroupUsersInFlatTable($grpId,$userId='',$opt='Delete',$usrSync=TRUE){
	try{
		$is_admin = 0;
		$del_flag = FALSE;
		// Return without doing anything if both group id and user id is empty
		if(empty($grpId) && empty($userId))
			return TRUE;
	
	if(!empty($grpId))
		$is_admin = adminGrporNot($grpId);
	expDebug::dPrint("Inside populate a group data".print_r($is_admin,true),4);
	if($is_admin){
		$select  = db_select('slt_groups','grp');
		$select->Join('slt_person','per');
			$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->addField('per','id','user_id');
		$select->addField('per','status','user_status');
		$select->addField('grp','id','group_id');
		$select->addField('grp','is_admin','group_type');
		$select->addField('grp','status','group_status');
		$select->addExpression('IF(FIND_IN_SET(per.id, grp.added_users)>0,"A","M")', 'user_type');
		$select->addField('grp','created_by','created_by');
		$select->addField('grp','created_on','created_on');
		$select->addField('grp','updated_by','updated_by');
		$select->addField('grp','updated_on','updated_on');
		if(!empty($grpId))
			$select->condition('grp.id',$grpId,'=');
		$select->condition('grp.status','cre_usr_sts_del','!=');
		$select->condition('grp.is_admin',1,'=');
		if(!empty($userId))
			$select->condition('per.id',$userId,'=');
		$select->where('FIND_IN_SET(per.id, grp.userslist)>0');
	}else{
		$select  = db_select('slt_groups','grp');
			$select->distinct();
		$select->Join('slt_person','per');
			$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
		$select->addField('per','id','user_id');
		$select->addField('per','status','user_status');
		$select->addField('grp','id','group_id');
		$select->addField('grp','is_admin','group_type');
		$select->addField('grp','status','group_status');
		$select->addExpression('IF(FIND_IN_SET(per.id, grp.added_users)>0,"A","M")', 'user_type');
		$select->addField('grp','created_by','created_by');
		$select->addField('grp','created_on','created_on');
		$select->addField('grp','updated_by','updated_by');
		$select->addField('grp','updated_on','updated_on');
		if(!empty($grpId))
			$select->condition('grp.id',$grpId,'=');
		if(!empty($userId))
			$select->condition('per.id',$userId,'=');
		$select->where('per.id NOT IN (1,2)
						  AND grp.status != "cre_sec_sts_del"
						  AND per.status != \'cre_usr_sts_del\'
						  AND grp.is_admin = 0
						    AND (((if(grp.org_id=\'All\',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
						  	AND (if(grp.user_type=\'All\',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,\'\'),grp.user_type)>0 ))
						  	AND (if(grp.employment_type=\'All\',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,\'\'),grp.employment_type)>0))
						  	AND (if(grp.country=\'All\',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,\'\'),grp.country)>0))
						  	AND (if(grp.state=\'All\',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,\'-\',per.state),\'\'),grp.state)>0))
						  	AND (if(grp.department=\'All\',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,\'\'),grp.department)>0))
						  	AND (if(grp.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,\'\'),ifnull(grp.job_role,\'\'))>0))
						  	AND (if(grp.language=\'All\',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,\'\'),grp.language)>0))
						    AND (CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\')
						      THEN
						      (ifnull(per.is_manager,\'N\') = grp.is_manager or ifnull(per.is_instructor,\'N\') = grp.is_instructor)
						      WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\')
						      THEN
						      (ifnull(per.is_manager,\'N\') = grp.is_manager)
						      WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\')
						      THEN
						      (ifnull(per.is_instructor,\'N\') = grp.is_instructor)
						      ELSE
						      1=1
						      END)
								AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1))
						  	AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)) ');
	}
	
	/*
	 * below two delete operations are changed to updated for Solr Sync.
	 * otherwise the recode will get removed form the group user map table
	 * before the solr sync will happen hance solr sync will not find the 
	 * records exactly. The actual delete will be called after the solr sync.
	 */
	if($opt=='Delete'){
			// Delete all the list of users from the flat table for the given 
		// group id (if any)
		/*$grpDel = db_delete('slt_group_user_mapping');
		if(!empty($grpId) && empty($userId))
			$grpDel->condition('group_id',$grpId,'=');
		if(!empty($userId))
			$grpDel->condition('user_id',$userId,'=');
		expDebug::dPrintDBAPI('Query for delete all user list in slt_group_user_mapping ',$grpDel);
		$grpDel->execute();*/
		
		$grpDel = db_update('slt_group_user_mapping');
		$grpDel->fields(array('custom0'=>'DEL',
													'updated_on'=>now()));
		if(!empty($grpId) && empty($userId))
			$grpDel->condition('group_id',$grpId,'=');
		if(!empty($userId))
			$grpDel->condition('user_id',$userId,'=');
		expDebug::dPrintDBAPI('Query for delete all user list in slt_group_user_mapping ',$grpDel);
		$grpDel->execute();
		
		$del_flag = TRUE;
	}
	
	if($opt=='DeleteGroup'){
			// Delete all the list of users from the flat table for the given 
		// group id (if any)
		/*$grpDel = db_delete('slt_group_user_mapping');
		if(!empty($grpId))
			$grpDel->condition('group_id',$grpId,'=');
		if(!empty($userId))
			$grpDel->condition('user_id',$userId,'=');
		expDebug::dPrintDBAPI('Query for delete all user list in slt_group_user_mapping ',$grpDel);
		$grpDel->execute();*/
		
		$grpDel = db_update('slt_group_user_mapping');
		$grpDel->fields(array('custom0'=>'DEL',
													'updated_on'=>now()));
		if(!empty($grpId))
			$grpDel->condition('group_id',$grpId,'=');
		if(!empty($userId))
			$grpDel->condition('user_id',$userId,'=');
		expDebug::dPrintDBAPI('Query for delete all user list in slt_group_user_mapping ',$grpDel);
		$grpDel->execute();
		
		$del_flag = TRUE;
	}
	
		if($opt == 'UserStatusChange'){ 
			$status = db_query('SELECT status FROM slt_person where id = :uid ',array(':uid'=>$userId))->fetchField();
			
			$loggedInUserId = getSltpersonUserId();
			$grpUpd = db_update('slt_group_user_mapping');
			$grpUpd->fields(array('user_status'     => $status,
	        				  'updated_on' => now(),
	              			  'updated_by' => $loggedInUserId
	                          ));
	        $grpUpd->condition('user_id',$userId,'=');
	        expDebug::dPrintDBAPI("Update user status in flat table -- ",$grpUpd);
	        $grpUpd->execute();
		}else if($opt == 'GroupStatusChange'){
			$status = db_query('SELECT status FROM slt_groups where id = :gid ',array(':gid'=>$grpId))->fetchField();
			
			$loggedInUserId = getSltpersonUserId();
			$grpUpd = db_update('slt_group_user_mapping');
			$grpUpd->fields(array('group_status'     => $status,
	        				  'updated_on' => now(),
	              			  'updated_by' => $loggedInUserId
	                          ));
	        $grpUpd->condition('group_id',$grpId,'=');
	        expDebug::dPrintDBAPI("Update group status in flat table -- ",$grpUpd);
	        $grpUpd->execute();
		}else{
			// Populate the list
			$grpIns = db_insert('slt_group_user_mapping');
			$grpIns->from($select);
			expDebug::dPrintDBAPI('Select query for populate user list in slt_group_user_mapping ',$select);
			expDebug::dPrintDBAPI('Insert query for populate user list in slt_group_user_mapping ',$grpIns);
			$grpIns->execute();
			
			//Removed users insert
			$sel  = db_select('slt_groups','grp');
			$sel->Join('slt_person','per');
			$sel->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
			$sel->addField('per','id','user_id');
			$sel->addField('per','status','user_status');
			$sel->addField('grp','id','group_id');
			$sel->addField('grp','is_admin','group_type');
			$sel->addField('grp','status','group_status');
			$sel->addExpression('"R"', 'user_type');
			$sel->addField('grp','created_by','created_by');
			$sel->addField('grp','created_on','created_on');
			$sel->addField('grp','updated_by','updated_by');
			$sel->addField('grp','updated_on','updated_on');
			if(!empty($grpId))
				$sel->condition('grp.id',$grpId,'=');
			
			$sel->condition('grp.status','cre_usr_sts_del','!=');
			
			if(!empty($userId))
				$sel->condition('per.id',$userId,'=');
			
			$sel->where('FIND_IN_SET(per.id, grp.removed_users)>0');
			
			$grpRmv = db_insert('slt_group_user_mapping');
			$grpRmv->from($sel);
			expDebug::dPrintDBAPI('Select query for populate user list in slt_group_user_mapping ',$sel);
			expDebug::dPrintDBAPI('Insert query for populate user list in slt_group_user_mapping ',$grpRmv);
			$grpRmv->execute();
			
			
			
		}
		//To sync solr
		if($usrSync == TRUE) {
			syncSolrData('User','','Bulk');
		}
		
		// Delete form slt_group_user_mapping
		if($del_flag == TRUE){
		    
		    // Get the fields before deleting
		    $select = db_select('slt_group_user_mapping', 'gumap');
		    $select->addField('gumap','id','id');
		    $select->addField('gumap','group_id','group_id');
		    $select->addField('gumap','user_id','user_id');
		    $select->addField('gumap','group_type','group_type');
		    $select->condition('custom0','DEL','=');
		    $gumapId =$select->execute()->fetchAll();
		    
			$delq = db_delete('slt_group_user_mapping');
			$delq->condition('custom0','DEL','=');
			$delq->execute();
			
			foreach($gumapId as $val){
			    if($val->group_type == 1){
			        $grp_type = 'Admin Group';
		}
			    else if($val->group_type == 0){
			        $grp_type = 'Learner Group';
			    }
			    // Make deleted log entry
			    $fields = array(
			            'table_name' => 'slt_group_user_mapping',
			            'entity_id' => $val->id,
			            'entity_type' => 'group_user_mapping',
			            'parent1_entity_id' => $val->group_type,
			            'parent1_entity_type' => $grp_type,
			            'parent2_entity_id' => $val->group_id,
			            'parent2_entity_type' => 'group_id',
			            'parent3_entity_id' => $val->user_id,
			            'parent3_entity_type' => 'user_id',
			            'module_name' => 'exp_sp_administration',
			            'functionality_name' => 'populateGroupUsersInFlatTable',
			            'api_name' => null,
			            'deleted_on' => now(),
			            'deleted_by' => 1
			    );
			    deleted_log_entry($fields);
			}
			
		}
	}catch(Exception $e){
		expDebug::dPrint("Error in Flat table update - ".$e->getMessage(),1);
		expertusErrorThrow($e);
	}
}
?>
