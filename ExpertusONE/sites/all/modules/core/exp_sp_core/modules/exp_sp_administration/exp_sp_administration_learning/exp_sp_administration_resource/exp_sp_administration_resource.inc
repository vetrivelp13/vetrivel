<?php
/**
 * Resource Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 09-Apr-2012
 */

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

function resourceJSObjectInfo() {
	try {
  $jsObjectInfo = array();
  $jsObjectInfo['name'] = 'narrowsearch'; // javascript object name (from exp_sp_administration_resource.js)
  $jsObjectInfo['init_id'] = 'root-admin'; 
  } catch (Exception $ex) {
  	watchdog_exception('resourceJSObjectInfo', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getResourceTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 							  - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */
function getResourceTabContentType(){
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getResourceTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
} 

/*
 * resourceNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function resourceNarrowSearchFiltersHTML() {
	try {
  $checkedStatus = array();
  $checkedStatus["lrn_res_fac_atv"] = TRUE;
  $allStatus = getProfileItemNamesKeyed('lrn_res_fac_');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4); 

  $allResCountry = fetchResourceCountryList();
  expDebug::dPrint(' $allResCountry = ' . print_r($allResCountry, true) , 4);
  
  $overAllDeliveryType = fetchResourceDeliveryTypes();
  expDebug::dPrint(' $overAllDeliveryType = ' . print_r($overAllDeliveryType, true) , 4);
  
  $renderfiltersets=array();
  
  if(count($overAllDeliveryType) > 1){
    $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'resourcetype',
                                                                 'title' => t('LBL036'),
                                                           		 'divid' => 'type',
                                                                 'checkboxes_list' => $overAllDeliveryType)
                                                          );
  }
  if(count($allStatus) > 1){
    $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'resstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus,
                                                           		 'checked_list' => $checkedStatus)
                                                          );
  }
  if(count($allResCountry) > 1){
    $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'rescountry',
                                                                 'title' => t('LBL039'),
                                                           		 'divid' => 'country',
                                                                 'checkboxes_list' => $allResCountry)
                                                          );
  }
  
  $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                           array('code' => 'rescitystate',
                                                                 'title' => t('Location'),
                                                                 'default_text'=> t('LBL310')
                                                                 )
                                                          ) ;
  
  $narrowfilters =  array();
  $narrowfilters[] = array("type" => "checkbox", "code" => "resourcetype");
  $narrowfilters[] = array("type" => "checkbox", "code" => "resstatus");
  $narrowfilters[] = array("type" => "checkbox", "code" => "rescountry");
  $narrowfilters[] = array("type" => "addltext", "code" => "rescitystate", "acpath" => "administration/learning/resource/citystate-autocomplete", "defaultText" => t('LBL310'));
  $filterSetforJS=$narrowfilters;
  
  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS), 
                     ));  
  } catch (Exception $ex) {
  	watchdog_exception('resourceNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * resourceSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function resourceSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'res-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'res-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'res-sort-new');
  
  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'res-sort-new'
                              );   
  
  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
  																						'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('resourceSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  } 
} 

/*
 * resourceSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function resourceSearchResultsActionbarHTML() {
  try {
  $actionbarLinksListforJS=array('export_path'=>'administration/learning/resource/export/',
  					             'print_path'=>'administration/learning/resource/print/'
                              );   
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL255'),
												 'add_page_path' =>  base_path() . 'administration/learning/resource/nojs/addedit', 
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,  												 'ctools_style' => 'ctools-modal-ctools-admin-resource-addedit-style',
                                                 'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),                                       
                                           )
         );
  } catch (Exception $ex) {
  	watchdog_exception('resourceSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getResourceTabContentMainHTML - Returns the html for tab content main 
 */
function getResourceTabContentMainHTML() {   
	try{
  $tabContentTypeArr =  getResourceTabContentType();  
  $tabContentType = $tabContentTypeArr['tab_content_type'];   
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/learning/resource/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/learning/resource/textfilter-autocomplete'                                            
                                          ); 
  
    $tabContentTypeStr=theme('narrow_search_tab_content',array(
                                                               'narrowsearch_tab_content_list'=> drupal_json_encode($narrowsearchTabContentListforJS),       
                                                              )
                            ); 
  }else{ /* This  is for non-narrow search */
    $tabContentTypeStr="";
  }  
  return $tabContentTypeStr;  
  } catch (Exception $ex) {
  	watchdog_exception('getResourceTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getresourceNarrowSearchBlockHTML - Returns the html for narrow search block on resource search page
 */
function getResourceTabContent() {
	try {
  $tabContentTypeArr =  getResourceTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type']; 
  
  $resourceDetailsResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => resourceNarrowSearchFiltersHTML(),           
    'rendered_narrow_search_sortbar' => resourceSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => resourceSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getResourceTabContentMainHTML(), 
  );
  echo drupal_json_encode($resourceDetailsResponse); 
  } catch (Exception $ex) {
  	watchdog_exception('getResourceTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}


function getFacilityIdBasedOnLocId($locationId){
   try { 
    // Select the DB table
    $select = db_select('slt_facility', 'facility');
    
    // Select fields to fetch
    $select->addField('facility',	'id', 'facility_id');
    
    // Set the condition(s)
    $select->condition('facility.location_id', $locationId);
    
    // Execute query and fetch the single values
    $facilityId  = $select->execute()->fetchField();
    
    return $facilityId;
    } catch (Exception $ex) {
    	watchdog_exception('getFacilityIdBasedOnLocId', $ex);
    	expertusErrorThrow($ex);
    }
}


function getLocationIdBasedOnFacilityId($facilityId){
   try { 
    // Select the DB table
    $select = db_select('slt_facility', 'facility');
    
    // Select fields to fetch
    $select->addField('facility',	'location_id', 'location_id');
    
    // Set the condition(s)
    $select->condition('facility.id', $facilityId);
    
    // Execute query and fetch the single values
    $locationId  = $select->execute()->fetchField();
    
    return $locationId;
    } catch (Exception $ex) {
    	watchdog_exception('getLocationIdBasedOnFacilityId', $ex);
    	expertusErrorThrow($ex);
    }
}

function getFacilityIdBasedOnOtherId($type, $id){
  try {
    $tableName = $type == 'Classroom' ? 'slt_classroom' : 'slt_equipment';  
    
    // Select the DB table
    $select = db_select($tableName, 'resource');
    
    // Select fields to fetch
    $select->addField('resource',	'facility_id', 'facility_id');
    
    // Set the condition(s)
    $select->condition('resource.id', $id);
    expDebug::dPrintDBAPI('exp_sp_administration_resource_addedit_form - getFacilityIdBasedOnOtherId -  Query : ' , $select);
    // Execute query and fetch the single values
    $facilityId  = $select->execute()->fetchField();
    
    return $facilityId;
    } catch (Exception $ex) {
    	watchdog_exception('getFacilityIdBasedOnOtherId', $ex);
    	expertusErrorThrow($ex);
    }
}
/* 
function getLocationTitleDirect($locationId){
	try{
    $select = db_select('slt_location', 'location');
    $select->addField('location',	'name', 'location_title');
    $select->condition('location.id', $locationId);
    $locationTitle  = $select->execute()->fetchField();
    return $locationTitle;
    } catch (Exception $ex) {
    	watchdog_exception('getLocationTitleDirect', $ex);
    	expertusErrorThrow($ex);
    }
} */

function getFacilityTitleBasedOnClsEqp($type,$id){
 try {
    $tableName = $type == 'Classroom' ? 'slt_classroom' : 'slt_equipment';  
    
    // Select the DB table
    $select = db_select($tableName, 'resource');
    $select->leftJoin('slt_facility', 'facility', 'facility.id = resource.facility_id');
    // Select fields to fetch
    $select->addField('facility',	'name', 'facility_title');
    
    // Set the condition(s)
    $select->condition('resource.id', $id);

    // Execute query and fetch the single values
    $facilityName  = $select->execute()->fetchField();
    
    return $facilityName;
    } catch (Exception $ex) {
    	watchdog_exception('getFacilityTitleBasedOnClsEqp', $ex);
    	expertusErrorThrow($ex);
    }
}

/*
 * getResourceDetailsHTML() - Renders an resource details as HTML to be displayed in jqGrid.  
 */
function getResourceDetailsHTML($res) {
	try {
  $detailsList = array();
  $actionList = array();
  if (!empty($res->type)) {
    $detailsList[] = array('detail' => $res->type, 'tooltip' => t('LBL036'));
  }
  if (!empty($res->status)) {
    $detailsList[] = array('detail' => $res->status, 'tooltip' => t('LBL102'));
  } 
  if (!empty($res->description)) {
    $detailsList[] = array('detail' => $res->description, 'tooltip' => $res->description);
  }
  
  if($res->type == 'Classroom' || $res->type == 'Equipment') {
    $detailsList[] = array('detail' => getFacilityTitleBasedOnClsEqp($res->type,$res->id), 'tooltip' => 'Facility');
  }
  
  if($res->type == 'Facility') {
    $facility_id = $res->facility_id;
  }
  
  if($res->type == 'Location') {
    $facility_id = $res->location_id;
  }
  
  if($res->type == 'Classroom') {
    $facility_id = $res->parent_pk;
  }
  
  if($res->type == 'Equipment') {
    $facility_id = $res->parent_pk;
  }
  
  $entityId = $res->type.'-'.$facility_id;
  
  $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/learning/resource/nojs/addedit',
					  'action_button_params' => $entityId,
  					  'action_button_params_child' => '',
  					  'ctools_style' => 'ctools-modal-ctools-admin-resource-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  $actionList[] = array('action_link_text' => t('LBL286'),
					  'action_page_path' => '',
					  'action_button_params' => '',
  					  'action_button_params_child' => '',
  					  'ctools_style' => 'ctools-modal-ctools-admin-resource-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
					  
  return theme('narrow-search-results-item-details',
                 array('title' => trim($res->title),
                       'details_list'=> $detailsList,
                       'delete_info' => t('MSG357').' '.strtolower(t('LBL320')),
                       'description'=> trim($res->short_desc),
                       'action_list'=> $actionList
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getResourceDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getResourceActionsHTML() - Renders an resource's actions as HTML to be displayed in jqGrid.  
 */
function getResourceActionsHTML($res) {
	try {
  return theme('narrow-search-results-item-actions',
                   array('edit_page_path' => '',
                         'edit_button_action_params' => array($res->id)
                        )
              );
  } catch (Exception $ex) {
  	watchdog_exception('getResourceActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchResourceCountryList() - Fetches all resource country list.  
 */

function fetchResourceCountryList() {
  // Select slt_resource table 
  try {
  $select = db_select('slt_facility', 'fac');

  // Left join the cod and name to get the country name 
  $select->leftJoin('slt_country', 'rsc', 'rsc.country_code = fac.contact_country');
  
  // Select fields/expression values to be fetched from the database.
  $select->addField('rsc', 'country_code', 'code');
  $select->addField('rsc', 'country_name', 'name');

  // Order results by name
  $select->orderBy('name');
  
  expDebug::dPrintDBAPI(' $select object = ' , $select);
 
  $resCountryList = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $resLangList = ' . print_r($resCountryList, true) , 3);
  
  return $resCountryList;
  } catch (Exception $ex) {
  	watchdog_exception('fetchResourceCountryList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchResourceDeliveryTypes() - Fetches all delivery type list.  
 */

function fetchResourceDeliveryTypes() {
  // Select slt_profile_list_items table 
  try {
  $select = db_select('slt_profile_list_items', 'splt');
  
  // Select fields/expression values to be fetched from the database.
  $select->addField('splt','code', 'code');
  $select->addField('splt','name', 'name');
  
  $select->condition(db_or()->
      condition('splt.code', 'cre_sys_obt_loc' , '=')->
      condition('splt.code', 'cre_sys_obt_flt' , '=')->
      condition('splt.code', 'cre_sys_obt_rom' , '=')->
      condition('splt.code', 'cre_sys_obt_equ' , '='));
      
  // Order results by name
  $select->orderBy('name');
  
  expDebug::dPrintDBAPI(' $select object = ' , $select);
 
  $overallDeliveryList = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('  $overallDeliveryList = ' . print_r( $overallDeliveryList, true) , 3);
  
  return $overallDeliveryList;
  } catch (Exception $ex) {
  	watchdog_exception('fetchResourceDeliveryTypes', $ex);
  	expertusErrorThrow($ex);
  }
}




/**
 * resourceStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function resourceStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" : 
                $field = $tableAlias . ".name";
                $direction = "ASC";
                break;
                  
    case "ZA" :
                $field = $tableAlias . ".name";
                $direction = "DESC";
                break;
   
    case "NewlyListed" :
                $field = $tableAlias . ".updated_on";
                $direction = "DESC";
                break;

    default:
                $field = $tableAlias . ".updated_on";
                $direction = "DESC";
                break;
  }

  $selectStmt->orderBy($field, $direction);
  } catch (Exception $ex) {
  	watchdog_exception('resourceStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendResourceListToJqGrid() - Prepares JSON response to JqGrid's request for resource
 */
function sendResourceListToJqGrid() {
  try {
  expDebug::dPrint('  ' .
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
                        'npage = "' . $_GET['npage'] , 4
  );
  
  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];

  $numRes = fetchResourceAll('COUNT');
  expDebug::dPrint(' $numRes = ' . print_r($numRes, true) , 4);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numRes);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  $resList = fetchResourceAll('LIST', $start, $limit);
  expDebug::dPrint(' $resList = ' . print_r($resList, true) , 4);  

  // Prepare and send the response
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numRes;
  
  $response->initial_sort_type_html_id = 'res-sort-az';
    
  $i=0;
  foreach ($resList as $res) {
    expDebug::dPrint(' $res = '. print_r($res, true) , 4);

    $res->row =  $i;
    $response->rows[$i]['id']   = $res->id;
    
    $response->rows[$i]['cell'] = array(
                                         'details' => getResourceDetailsHTML($res),
                                         /*'action' => getResourceActionsHTML($res)*/
                                         );

    $i++;    
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);
  
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendResourceListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getResourceAutoComplete() - Returns the matched Resource's title
 */
function getResourceAutoComplete()
{
	try {
  $searchCurText =  $_GET['z'];
  // Select the table
  $select = db_select('slt_master_search', 'smsr');
  // Add fields to fetch
  $select->addField('smsr', 'name', 'Title');
  $select->addField('smsr', 'type', 'type');
  
  //get the resource types
  $overAllDeliveryType = fetchResourceDeliveryTypes();
  $searchDeliveryTypes = array_values($overAllDeliveryType);
  
  // Add condition
  $select->condition(db_and()
                          ->condition('smsr.name', '%' . db_like($searchCurText) . '%', 'LIKE')
                          ->condition('smsr.type', $searchDeliveryTypes , 'IN')
                    );

  // Order in dictionary sort order
  $select->orderBy('name');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  $matchingResNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingOrgNames = '. print_r($matchingPrgNames, true) , 3);
  foreach ( $matchingResNames as $resName) {
    print $resName->title . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getResourceAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchResourceAll() - Fetches resource count / resource list for jqgrid after applying the narrow search filters
 */
function fetchResourceAll($op, $start, $limit,$api='') {
  // Select slt_resource table
  try {
  	if($api == 1){$limit = 10;}
  $select = db_select('slt_master_search', 'sltm');
    
  // Join required tables
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'sltm.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  
  $select->leftJoin('slt_location',  'loc', 'sltm.pk_id = loc.id AND sltm.type = \'Location\'');
  $select->leftJoin('slt_facility',  'fac', 'sltm.pk_id = fac.id AND sltm.type = \'Facility\'');
  $select->leftJoin('slt_classroom', 'cls', 'sltm.pk_id = cls.id AND sltm.type = \'Classroom\'');
  $select->leftJoin('slt_equipment', 'eqp', 'sltm.pk_id = eqp.id AND sltm.type = \'Equipment\'');
  
  $select->leftjoin('slt_country', 'sc', 'fac.contact_country = sc.country_code');
  $select->leftjoin('slt_state', 'st', 'fac.contact_state = st.state_code AND st.country_code = sc.country_code');
    
  
  if ($op == "COUNT") {
    // Get count of active resource
    $select->addExpression('COUNT(DISTINCT(sltm.id))', 'count');
  }
  else { // LIST
    // Select fields/expression values to be fetched from the database.
    $select->addField('fac', 'id', 'facility_id');
    $select->addField('loc', 'id', 'location_id');
    $select->addField('cls', 'id', 'classroom_id');
    $select->addField('eqp', 'id', 'equipment_id');
    $select->addField('sltm', 'pk_id', 'id');
    $select->addField('sltm', 'name', 'title');
    $select->addField('sltm', 'description', 'description');
    $select->addField('sltm', 'type', 'type');
    $select->addField('plistatus', 'name', 'status');
    $select->addField('sltm', 'parent_pk', 'parent_pk');
    $select->addField('sc', 'country_name', 'country_name');
	$select->addField('loc', 'name', 'name');
	$select->addField('loc', 'addr1', 'addr1');
	$select->addField('loc', 'addr2', 'addr2');
	$select->addField('loc', 'city', 'city');
	$select->addField('loc_stats', 'state_name', 'state');
	$select->addField('loc_ctry', 'country_name', 'country');
	$select->addField('loc', 'zipcode', 'zipcode');
	
  }

  // Set the conditions
  $overAllDeliveryType = fetchResourceDeliveryTypes();
  if(!empty($_GET['resourcetype'])) {
    $selectedFilters = $_GET['resourcetype'];
    if (!empty($selectedFilters)){
      $narrowSearchSet = explode("|", $selectedFilters);
      // Select slt_profile_list_items table 
      $rtselect = db_select('slt_profile_list_items', 'splt');
      $rtselect->addField('splt','name', 'name');
      $rtselect->condition('splt.code', $narrowSearchSet , 'IN');
      $typeList = $rtselect->execute()->fetchAll(PDO::FETCH_COLUMN);
      expDebug::dPrint(' $select = ' . print_r($typeList, true) , 3);
      $select->condition('sltm.type', $typeList , 'IN');
    }
  } else {
      //get the resource types
      $overAllDeliveryType = fetchResourceDeliveryTypes();
      $searchDeliveryTypes = array_values($overAllDeliveryType);
      $select->condition('sltm.type', $searchDeliveryTypes, 'IN');
      //$select->condition('sltm.status', array('lrn_res_fac_itv', 'lrn_res_loc_itv', 'lrn_res_rms_itv'), 'NOT IN');
      
  }
  
  if(isset($_GET['resstatus'])) {
    if($_GET['resstatus'] == "lrn_res_fac_atv") {
      $searchResourceStatus = array('lrn_res_loc_atv','lrn_res_fac_atv','lrn_res_rms_atv');
      $select->condition('sltm.status', $searchResourceStatus, 'IN');
    } else if($_GET['resstatus'] == "lrn_res_fac_itv") {
      $searchResourceStatus = array('lrn_res_loc_itv','lrn_res_fac_itv','lrn_res_rms_itv');
      $select->condition('sltm.status', $searchResourceStatus, 'IN');
    } 
  }
  else{
    $searchResourceStatus = array('lrn_res_loc_atv','lrn_res_fac_atv','lrn_res_rms_atv');  
    $select->condition('sltm.status', $searchResourceStatus, 'IN');
  }
 
  //condition only for country filter
  if(!empty($_GET['rescountry'])) {
    $selectedFilters = $_GET['rescountry'];
    expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
    if (!empty($selectedFilters)){
       $narrowSearchSet = explode("|", $selectedFilters);
       $select->condition('fac.contact_country', $narrowSearchSet , 'IN');  
    }
  }
  
  if(!empty($_GET['rescitystate'])) {
    $rescitystate = $_GET['rescitystate'];
    if($rescitystate == t('LBL310')){
      $rescitystate = '';
    }
     $select->condition(db_or()->     
         condition('fac.contact_city', '%' . db_like($rescitystate) . '%', 'LIKE')->
         condition('st.state_name', '%' . db_like($rescitystate) . '%', 'LIKE')
         );
         
    //selectAddNarrowSearchByTextCondition($select, 'fac', 'contact_city', 'rescitystate', t('Type a city or state'));
    //selectAddNarrowSearchByTextCondition($select, 'fac', 'contact_state', 'rescitystate', t('Type a city or state'));
  }
   
  /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
  if($api==1){  	
	 $select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
	 $select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
     $location_text_filter = stripslashes(getRawURLParam('textfilter'));   
	 if(trim($location_text_filter)!=''){
		   $loc_or_condition_obj=getLocationFilterQuery($location_text_filter,'1');  
		   $select->condition($loc_or_condition_obj);   
	 }
  }
  
  //$select->condition('sltm.type', array('Facility', 'Location'), 'IN');
  
  if ($op != "COUNT") {
    // Order the records
    resourceStmtAddOrderByClause($select, 'sltm', "AZ");
    // Group by res.id to eliminate duplicate resource records
    $select->groupBy('sltm.id');
  }
 
  expDebug::dPrintDBAPI(' $select obj = ' , $select); 
  
  if($op=="COUNT"){
  	// Execute the query
	  $result = $select->execute()->fetchField();
	  expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
  }
  elseif($op == "CSV"){
    $colHeadersMap = array(t('LBL083') => 'title',
                           t('LBL036') => 'type',                         
                           t('LBL039') => 'country_name',
                           t('LBL102') => 'status'
                          );
   SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap);
  }
  elseif($op == "PDF"){
    // % width of the columns must total to 100%
  	$colHeadersMap = array(t('LBL083') => array('title',        40),
                        	 t('LBL036') => array('type',         15),
                        	 t('LBL039') => array('country_name', 30),
                           t('LBL102') => array('status',       15),
  	                    );
  	
   $appliedFilters = array(t('LBL653') => checkboxFiltersetSelectionsToPrintableStr('resourcetype'),
                           t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('resstatus'),
                           t('LBL039') => textboxFiltersetValueToPrintableStr('rescountry'),
                           ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
                        );

  SendNarrowSearchResultsAsPDFFile($select, t('LBL320'), $appliedFilters, $colHeadersMap);
    
  }
  else{ // LIST
	  // Execute the query and fetch all the records
	  // Limit to $limit records
	  $select->range($start, $limit);
	  
	  //print $select;
	  $result = $select->execute()->fetchAll();
	  expDebug::dPrint('  LIST $result = ' . print_r($result, true) , 3);
  }
 return $result;
 } catch (Exception $ex) {
 	watchdog_exception('fetchResourceAll', $ex);
 	expertusErrorThrow($ex);
 }
}

/*
 * getDefaultOrgAddEditFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultResAddEditFormFieldValue($field, $form_state, $resDetails) {
 try {
  $defaultValue = '';
  
  $defaultValue = empty($form_state['values'][$field])? (empty($resDetails[$field])? '' : $resDetails[$field]) :
                                                        $form_state['values'][$field];

  expDebug::dPrint(' $defaultValue for field ' .
                                                $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultResAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

function getFacilityDetailsViewOld($facilityId){
 try {
  $select  = db_select('slt_facility','facility');
  $select -> innerjoin('slt_location','location','facility.location_id = location.id');
  $select -> addField('facility','id','facility_id');
  $select -> addField('facility','name','facility_name');
  
  $select -> addField('facility','contact_addr1','addr1');
  $select -> addField('facility','contact_addr2','addr2');
  $select -> addField('facility','contact_country','country');
  $select -> addField('facility','contact_state','state');
  $select -> addField('facility','contact_city','city');
  $select -> addField('facility','contact_zipcode','zipcode');
  $select -> addField('facility','contact_fname','first_name');
  $select -> addField('facility','contact_lname','last_name');
  $select -> addField('facility','contact_fax','fax');
  $select -> addField('facility','contact_phone','phone_no');
  $select -> addField('facility','contact_email','email');
  $select -> addField('facility','status','status');
  $select -> addField('facility','in_active_reason','status_inactive_reason');
  $select -> addField('facility','location_id','location_id');
  $select -> addField('location','name','location_name');
  
  $select -> condition('facility.id',$facilityId,'=');  
  
  $resourceList = $select->execute()->fetchAssoc();
    
  return $resourceList;
  } catch (Exception $ex) {
  	watchdog_exception('getFacilityDetailsViewOld', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewFacilityDetailsOld($form, &$form_state, $createUserId, $callFromAPI =0){
  $txn = db_transaction();

  try {
    
    if($form_state['values']['status'] == 'lrn_res_fac_itv') {
      $locationStatus = 'lrn_res_loc_itv';
    }else{
      $locationStatus = 'lrn_res_loc_atv';
    }
     /* Insert Statement for Location */      
      $insertLocStmt = db_insert('slt_location');
      $custom  = NULL;        	
      $fields = array(
                'name'               => $form_state['values']['location_name'],
                'contact_fname'      => '',
      			'contact_lname'      => '',
                'phone'              => '',
                /*'addr1'              => '',
                'addr2'              => '',
                'city'               => '',
                'state'              => '',
                'country'            => '',
                'zipcode'            => '',*/
                'direction'          => '',
                'latitude'           => 0,
                'longitude'          => 0,
                'timezone'           => '',
                'gmtoffset'          => 0,
                'dstoffset'          => 0,
      			'is_active'          => 1,
      			'status'             => $locationStatus,
                'created_by'         => $createUserId,
                'created_on'         => now(),
                'updated_by'         => $createUserId,
                'updated_on'         => now(),
      			'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
      $insertLocStmt->fields($fields);			            
      $locationId = $insertLocStmt->execute();
      
      $form_state['values']['location_id'] = $locationId;
      expDebug::dPrint(' $$locationId = ' . print_r($locationId, true) , 3);
      
      $inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
      $in_active_on          = $form_state['values']['status'] == 'lrn_res_fac_itv' ? now() : NULL;
      $in_active_reason      = $form_state['values']['status'] == 'lrn_res_fac_itv' ? $inactivereasontext : NULL; 
         
     /* Insert Statement for Facility  */           
      $insertFacStmt = db_insert('slt_facility');
      $custom  = NULL;    	
      $fields = array(
                'location_id'        => $locationId,                  
                //'location_name'      => 'LocationNameComesHere',
                'name'               => $form_state['values']['facility_name'],
                'contact_addr1'      => $form_state['values']['addr1'],
                'contact_addr2'      => $form_state['values']['addr2'],
                'contact_country'    => $form_state['values']['country'],
                'contact_state'      => $form_state['values']['state'],
                'contact_city'       => $form_state['values']['city'],
                'contact_zipcode'    => $form_state['values']['zipcode'],
                'contact_fname'      => $form_state['values']['first_name'],
                'contact_lname'      => $form_state['values']['last_name'],
                'contact_fax'        => $form_state['values']['fax'],
                'contact_phone'      => $form_state['values']['phone_no'],
                'contact_email'      => $form_state['values']['email'],
      			'status'             => $form_state['values']['status'],
      			'is_active'          => 1,
      			'in_active_on'       => $in_active_on,
                'in_active_reason'   => $in_active_reason,
                'created_by'         => $createUserId,
                'created_on'         => now(),
                'updated_by'         => $createUserId,
                'updated_on'         => now(),
      			'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
      $insertFacStmt->fields($fields);			            
      $facilityId = $insertFacStmt->execute();
      
      $form_state['values']['facility_id'] = $facilityId;

      expDebug::dPrint(' $facilityId = ' . print_r($facilityId, true) , 3);
      if($callFromAPI){ 
        return array((object)array('LocationId'=>$locationId, 'FacilityId'=>$facilityId));
      }     
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewFacilityDetailsOld', $ex);
    if($callFromAPI){ 
        //return array((object)array('LocationId'=>'Failiure', 'FacilityId'=>'Failiure'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Location Id or Facility Id is not valid";
        return $errobj;
    } 
    throw $ex;    
  }
  
  unset($txn);
  
  return $facilityId;
}

function updateFacilityDetailsOld($form, &$form_state, $createUserId, $callFromAPI =0){
  $txn = db_transaction();

  try {

    if($form_state['values']['status'] == 'lrn_res_fac_itv') {
      $locationStatus = 'lrn_res_loc_itv';
    }else{
      $locationStatus = 'lrn_res_loc_atv';
    }
    /* Update Statement for Location */
    $updateLocStmt = db_update('slt_location');
    $custom  = NULL;  
    $fields = array(
                'name'               => $form_state['values']['location_name'],
    			'status'             => $locationStatus,
                'updated_by'         => 1,
                'updated_on'         => now()   
              );
    
    $updateLocStmt->fields($fields);    
    $updateLocStmt->condition('id', $form_state['values']['location_id']);
    $numLocUpdated = $updateLocStmt->execute();
      	
    expDebug::dPrint(' $$numLocUpdated = ' . print_r($numLocUpdated, true) , 3);
    
    $inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
    $in_active_on          = $form_state['values']['status'] == 'lrn_res_fac_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'lrn_res_fac_itv' ? $inactivereasontext : NULL;
          
    /* Update Statement for Facility */    
    $updateFacStmt = db_update('slt_facility');
    $custom  = NULL;  
    $fields = array(
                'location_id'        => $form_state['values']['location_id'],                  
                //'location_name'      => 'LocationNameComesHereUpdated',
                'name'               => $form_state['values']['facility_name'],
                'contact_addr1'      => $form_state['values']['addr1'],
                'contact_addr2'      => $form_state['values']['addr2'],
                'contact_country'    => $form_state['values']['country'],
                'contact_state'      => $form_state['values']['state'],
                'contact_city'       => $form_state['values']['city'],
                'contact_zipcode'    => $form_state['values']['zipcode'],
                'contact_fname'      => $form_state['values']['first_name'],
                'contact_lname'      => $form_state['values']['last_name'],
                'contact_fax'        => $form_state['values']['fax'],
                'contact_phone'      => $form_state['values']['phone_no'],
                'contact_email'      => $form_state['values']['email'],
      			'status'             => $form_state['values']['status'],
      			'in_active_on'       => $in_active_on,
                'in_active_reason'   => $in_active_reason,    
                'updated_by'         => 1,
                'updated_on'         => now(),
                'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
    
    $updateFacStmt->fields($fields);    
    $updateFacStmt->condition('id', $form_state['values']['facility_id']);
    $numFacUpdated = $updateFacStmt->execute();
      	
    expDebug::dPrint(' $$numFacUpdated = ' . print_r($numFacUpdated, true) , 3);
    if($callFromAPI){ 
        return array((object)array('LocationId'=>$form_state['values']['location_id'], 'FacilityId'=>$form_state['values']['facility_id']));
    }
      
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateFacilityDetailsOld', $ex);
    if($callFromAPI){ 
        //return array((object)array('LocationId'=>'Failiure', 'FacilityId'=>'Failiure'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Location Id or Facility Id is not valid";
        return $errobj;
    }
    throw $ex;
  }

  unset($txn);  
}

function getCityStateAutoComplete(){
 try {
	  $searchCurText =  $_GET['z'];
	  
	  $selectCity = db_select('slt_facility', 'fac');
	  $selectCity->addField('fac', 'contact_city', 'name');
	  $selectCity->addExpression("CONCAT('city')", 'city_state');  
	  $selectCity->condition('fac.contact_city', '%' . db_like($searchCurText) . '%', 'LIKE');
	  $matchedResultsCity = $selectCity->execute()->fetchAll();
	  
	  $selectState = db_select('slt_facility', 'fac');
	  $selectState->leftjoin('slt_country', 'sc', 'fac.contact_country = sc.country_code');
	  $selectState->leftjoin('slt_state', 'st', 'fac.contact_state = st.state_code AND st.country_code = sc.country_code');
	  $selectState->addField('st', 'state_name', 'name');
	  $selectState->addExpression("CONCAT('state')", 'city_state');  
	  $selectState->condition('st.state_name', '%' . db_like($searchCurText) . '%', 'LIKE');
	  $matchedResultsState = $selectState->execute()->fetchAll();
	  
	  $mergedValues = array_merge($matchedResultsCity,$matchedResultsState);
	  foreach ( $mergedValues as $key => $resName) {
	    //print $resName->name."|". $resName->city_state. "\n";
	    print $resName->name. "\n";
	  }
  } catch (Exception $ex) {
  	watchdog_exception('getCityStateAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

function listLocationByRestAPI($start, $limit, $userid="") 
{
	try {
		$form=array();
		$form["form_id"]="";
	//	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$result = fetchResourceAll('',$start,$limit,1);
		$result['totalrow'] = fetchResourceAll('COUNT','','',1);
		expDebug::dPrint(' $count = ' . print_r($count, true) , 3);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('listLocationByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}


?>