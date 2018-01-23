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
	try{
  $checkedStatus = array();
  $checkedStatus["lrn_res_loc_atv"] = TRUE;
  $allStatus = _getProfileStatusItemNamesKeyed('lrn_res_loc_');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4); 

  $allResCountry = fetchResourceCountryList();
  expDebug::dPrint(' $allResCountry = ' . print_r($allResCountry, true) , 4);
  
  $overAllDeliveryType = fetchResourceDeliveryTypes();
  expDebug::dPrint(' $overAllDeliveryType = ' . print_r($overAllDeliveryType, true) , 4);
  
  $renderfiltersets=array();
  
  /*if(count($overAllDeliveryType) > 1){
    $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'resourcetype',
                                                                 'title' => t('LBL036'),
                                                                 'checkboxes_list' => $overAllDeliveryType)
                                                          );
  }*/
  if(count($allStatus) > 1){
    $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'resstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus)
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
	                                                                 'title' => t('LBL708'),
	                                                           		 'divid' => 'city_state',
	                                                           		 'from'=>'AdminManageLocationPage', 
	                                                           		 'name'=>'LocationFilter',  
	                                                                 'default_text'=> t('LBL1321')
                                                                 )
																 
                                                          ) ; /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */

  $narrowfilters =  array();
  $narrowfilters[] = array("type" => "checkbox", "code" => "resourcetype");
  $narrowfilters[] = array("type" => "checkbox", "code" => "resstatus");
  $narrowfilters[] = array("type" => "checkbox", "code" => "rescountry");
  $narrowfilters[] = array("type" => "addltext", "code" => "rescitystate", "acpath" => "administration/manage/location/citystate-autocomplete", "defaultText" => t('LBL1321')); /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
  
  $filterSetforJS = $narrowfilters;
  
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
  $actionbarLinksListforJS=array('export_path'=>'administration/manage/location/export/',
  					             'print_path'=>'administration/manage/location/print/'
                              );   
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL603'),
												 'add_page_path' =>  base_path() . 'administration/manage/location/nojs/addedit', 
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
  											 'ctools_style' => 'ctools-modal-ctools-admin-resource-addedit-style',
                         'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS), 
  											 'page_code' => 'cre_sys_obt_loc',
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
	try {    
  $tabContentTypeArr =  getResourceTabContentType();  
  $tabContentType = $tabContentTypeArr['tab_content_type'];   
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/manage/location/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/location/textfilter-autocomplete'                                            
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
    'rendered_main_div' => getAdminModules('administration','manage'),
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
    expDebug::dPrintDBAPI('exp_sp_administration_resource_addedit_form - getFacilityIdBasedOnOtherId -  Query : ', $select);
    // Execute query and fetch the single values
    $facilityId  = $select->execute()->fetchField();
    
    return $facilityId;
    } catch (Exception $ex) {
    	watchdog_exception('getFacilityIdBasedOnOtherId', $ex);
    	expertusErrorThrow($ex);
    }
}

function getLocationTitleDirect($locationId){
	try {
    $select = db_select('slt_location', 'location');
    $select->addField('location',	'name', 'location_title');
    $select->condition('location.id', $locationId);
    $locationTitle  = $select->execute()->fetchField();
    return $locationTitle;
    } catch (Exception $ex) {
    	watchdog_exception('getLocationTitleDirect', $ex);
    	expertusErrorThrow($ex);
    }
}

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
  global $theme_key;
  $detailsList = array();
  $actionList = array();
  $newActionList = array(); // 'capacity');country_name
  if (!empty($res->capacity)) {
    $detailsList[] = array('detail' => t('LBL318').': '.$res->capacity, 'tooltip' => t('LBL318').': '.$res->capacity);
  }
  if (!empty($res->country_name)) {
    $detailsList[] = array('detail' => t('LBL039').': '.t($res->country_name), 'tooltip' => t('LBL039').': '.t($res->country_name));
  } 
  if (!empty($res->equipment)) {
    $detailsList[] = array('detail' => t('Equipment').': '._getEquipmentInfo($res->equipment), 'tooltip' => t('Equipment').': '._getEquipmentInfo($res->equipment));
  }
  
  
  /*if (!empty($res->description)) {
    $detailsList[] = array('detail' => $res->description, 'tooltip' => $res->description);
  }
  
  if($res->type == 'Classroom' || $res->type == 'Equipment') {
    $detailsList[] = array('detail' => getFacilityTitleBasedOnClsEqp($res->type,$res->id), 'tooltip' => 'Facility');
  }
  */
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
					  'action_page_path' => 'administration/manage/location/nojs/addedit',
					  'action_button_params' => $res->location_id,
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
					  'action_button_params' => $res->location_id,
  					  'action_button_params_child' => '',
  					  'action_button_param_text' => 'location',
  					  'ctools_style' => 'ctools-modal-ctools-admin-resource-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );
					  
    /*if($res->location_status == 'lrn_res_loc_atv'){
        $newActionList[] = array('action_link_text' => t('LBL572'),
    				  		'action_page_path' => '', 
    				  		'action_button_params' => $res->location_id,
        					'action_button_params_text' => 'deactivate',
    					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
    					    'js_object_info' => array (
                                                'name' => 'narrowsearch', // provide the java script object name
                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                         ),
                        	'tooltip' => t('LBL572'),
    				  );
      } else {
        $newActionList[] = array('action_link_text' => t('LBL573'),
    				  		'action_page_path' => '', 
    				  		'action_button_params' => $res->location_id,
        					'action_button_params_text' => 'activate',
    					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
    					    'js_object_info' => array (
                                                'name' => 'narrowsearch', // provide the java script object name
                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                         ),
                        	'tooltip' => t('LBL573')
    			  );
      }*/

  $newActionList[] = array('action_link_text' => t('LBL1282'),
  		'action_page_path' => '',
  		'action_button_params' => $res->email_id,
  		'action_button_params_text' => 'send_message',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL1282'),
  		'icon_style_class'=>'send-message-tab-icon'
  );

     
  /*$newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $res->location_id,
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'entityId' => $res->location_id,
  		'entityType' => 'cre_sys_obt_loc',
  		'icon_style_class'=>'access-tab-icon'
  );*/
  $res->entityType = 'cre_sys_obt_loc';
  return theme('narrow-search-results-item-details',
                 array('title' => trim($res->title),
                       'details_list'=> $detailsList,
                       'delete_info' => t('MSG357').' '.strtolower(t('LBL320')),
                       'description'=> trim($res->short_desc),
                       'action_list'=> $actionList,
                 		'view_list'=> $newActionList,
                 		'entity_multi_action' => $res,
                       'new_action_list'=> $newActionList,                       
                 	     'object_type'=>'location',   
                 		'sumedit' => $res->sumedit,
                 		'sumdelete' => $res->sumdelete,
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
 /* return theme('narrow-search-results-item-actions',
                   array('edit_page_path' => '',
                         'edit_button_action_params' => array($res->id)
                        )
              );*/
    return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $res)
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
	try {
  // Select slt_resource table 
  //$select = db_select('slt_facility', 'fac');
  $select = db_select('slt_location', 'loc');

  // Left join the cod and name to get the country name 
  $select->leftJoin('slt_country', 'rsc', 'rsc.country_code = loc.country');
  
  // Select fields/expression values to be fetched from the database.
  $select->addField('rsc', 'country_code', 'code');
  $select->addField('rsc', 'country_name', 'name');
  
  $select->condition('loc.status', array('lrn_res_loc_del'), 'NOT IN');

  // Order results by name
  $select->orderBy('name');
  
  expDebug::dPrintDBAPI(' $select object = ' , $select);
 
  $resCountryList = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $resLangList = ' . print_r(array_unique($resCountryList), true) , 3);
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
  
  expDebug::dPrintDBAPI(' $select SQL = ' , $select);
  
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
                $field = $tableAlias . ".created_on";
                $direction = "DESC";
                break;

    default:
                $field = $tableAlias . ".created_on";
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
                        'npage = "' . $_GET['npage'] , 4
  );
  
  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];

/*   $numRes = fetchResourceAll('COUNT');
  expDebug::dPrint(' $numRes = ' . print_r($numRes, true) , 4);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numRes); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  $resList = fetchResourceAll('LIST', $start, $limit);
  expDebug::dPrint(' $resList = ' . print_r($resList, true) , 4);  

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numRes;
  
  $response->initial_sort_type_html_id = 'res-sort-new';
    
  $i=0;
  foreach ($resList as $res) {
    expDebug::dPrint(' $res = '. print_r($res, true) , 4);

    $res->row =  $i;
    $res->object_type = 'location';
    $response->rows[$i]['id']   = $res->id;
    
    $response->rows[$i]['cell'] = array(
                                         'details' => getResourceDetailsHTML($res),
                                         'action' => getResourceActionsHTML($res)
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
  $select->condition('smsr.status',array('lrn_res_loc_atv','lrn_res_loc_itv'),'IN');                    

  // Order in dictionary sort order
  $select->orderBy('name');
  expDebug::dPrintDBAPI(' $select SQL = ' , $select);
  
  $matchingResNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingOrgNames = '. print_r($matchingPrgNames, true) , 3);
  foreach ( $matchingResNames as $resName) {
    print rawurlencode($resName->title) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getResourceAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchResourceAll() - Fetches resource count / resource list for jqgrid after applying the narrow search filters
 */
function fetchResourceAll($op = NULL, $start = NULL , $limit = NULL) {
	try {
  // Select slt_resource table
  $select = db_select('slt_master_search', 'sltm');
 // $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=sltm.id AND group_map.entity_type = \'cre_sys_obt_loc\' and group_map.group_type = 1');
 // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
      
  // Join required tables
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'sltm.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  
  //$select->leftJoin('slt_location',  'loc', 'sltm.pk_id = loc.id AND sltm.type = \'Location\'');
  $select->innerJoin('slt_location',  'loc', 'sltm.pk_id = loc.id AND sltm.type = \'Location\'');
  //$select->leftJoin('slt_facility',  'fac', 'sltm.pk_id = fac.id AND sltm.type = \'Facility\'');
  //$select->leftJoin('slt_classroom', 'cls', 'sltm.pk_id = cls.id AND sltm.type = \'Classroom\'');
  //$select->leftJoin('slt_equipment', 'eqp', 'sltm.pk_id = eqp.id AND sltm.type = \'Equipment\'');
  
  $select->leftjoin('slt_country', 'sc', 'loc.country = sc.country_code');
    
  
  if ($op == "COUNT") {
    // Get count of active resource
    $select->addExpression('COUNT(DISTINCT(sltm.id))', 'count');
  }
  else { // LIST
    // Select fields/expression values to be fetched from the database.
    //$select->addField('fac', 'id', 'facility_id');

    $select->addField('loc', 'id', 'location_id');
    //$select->addField('cls', 'id', 'classroom_id');
    //$select->addField('eqp', 'id', 'equipment_id');
    $select->addField('sltm', 'pk_id', 'id');
    $select->addField('loc', 'capacity', 'capacity');
    $select->addField('loc', 'name', 'title');
    $select->addField('sltm', 'description', 'description');
    $select->addField('sltm', 'type', 'type');
    $select->addField('plistatus', 'name', 'status');
    $select->addField('loc', 'status', 'location_status');
    $select->addField('loc', 'email_id', 'email_id');
    $select->addField('loc', 'equipment', 'equipment');
    $select->addField('sltm', 'parent_pk', 'parent_pk');
    $select->addField('sc', 'country_name', 'country_name');
    $select->condition('loc.status', 'lrn_res_loc_del', '!=');
    $select->groupBy('sltm.id');
  }
  //filterByUserAccessPrivileges($select, 'catacs','','group_map',1);
  filterByUserAccessPrivileges($select, 'loc','',1,'cre_sys_obt_loc','location','loc');
  // Set the conditions (join additional tables as necessary)
  $selectedFilters = stripApostrophe($_GET['resourcetype']);
  if(!empty($selectedFilters)) {
    if (!empty($selectedFilters)){
      $narrowSearchSet = explode("|", $selectedFilters);
      // Select slt_profile_list_items table 
      $rtselect = db_select('slt_profile_list_items', 'splt');
      $rtselect->addField('splt','name', 'name');
      $rtselect->condition('splt.code', $narrowSearchSet , 'IN');
      expDebug::dPrintDBAPI(' $rtselect SQL = ' , $rtselect);
      
      $typeList = $rtselect->execute()->fetchAll(PDO::FETCH_ASSOC);
      expDebug::dPrint(' $typeList = ' . print_r($typeList, true) , 3);
      $select->condition('sltm.type', $typeList , 'IN');
    }
  } else {
      //get the resource types
      $overAllDeliveryType = fetchResourceDeliveryTypes();
      $searchDeliveryTypes = array_values($overAllDeliveryType);
      $select->condition('sltm.type', $searchDeliveryTypes, 'IN');
      //$select->condition('sltm.status', array('lrn_res_fac_itv', 'lrn_res_loc_itv', 'lrn_res_rms_itv'), 'NOT IN');
      
  }
  
  $selectedStatusFilters = stripApostrophe($_GET['resstatus']);
  if(!empty($selectedStatusFilters)) {
  	$selectedStatusFiltersList = explode("|", $selectedStatusFilters);
  	$searchResourceStatus = array();
  	foreach ($selectedStatusFiltersList as $selectedStatusFilter) {
  		switch ($selectedStatusFilter) {
  			case 'lrn_res_loc_atv' :
  				$searchResourceStatus[] = 'lrn_res_loc_atv';
  				$searchResourceStatus[] = 'lrn_res_fac_atv';
  				$searchResourceStatus[] = 'lrn_res_rms_atv';
  				break;
  			case 'lrn_res_loc_itv' :
  				$searchResourceStatus[] = 'lrn_res_loc_itv';
  				$searchResourceStatus[] = 'lrn_res_fac_itv';
  				$searchResourceStatus[] = 'lrn_res_rms_itv';
  				break;
  		} // end switch
  	} // end foreach
  }
  else{
    $searchResourceStatus = array('lrn_res_loc_atv','lrn_res_fac_atv','lrn_res_rms_atv','lrn_res_loc_itv','lrn_res_fac_itv','lrn_res_rms_itv');
  }
  $select->condition('loc.status', $searchResourceStatus, 'IN');
  
  //condition only for country filter
  $selectedFilters = stripApostrophe($_GET['rescountry']);
    expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
    if (!empty($selectedFilters)){
       $narrowSearchSet = explode("|", $selectedFilters);
       $select->condition('loc.country', $narrowSearchSet , 'IN');
  } 
  
  /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
  
	$select->leftjoin('slt_state', 'st', 'loc.state = st.state_code AND loc.country = st.country_code');	
	  
	$rescitystate = $_GET['rescitystate']; // #65628 - for securityfix 
	if(!empty($rescitystate) && $rescitystate!= t('LBL1321')) { // Exclude the country check option due to using country filter
		 $loc_or_condition_obj=getLocationFilterQuery($rescitystate,'','loc','st','sc');
		 expDebug::dPrint('$loc_or_condition_obj ='.print_r($loc_or_condition_obj,true),5);  
		 $select->condition($loc_or_condition_obj);
	}  
	  
	  /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
	$mutipleTextFilterStr = getRawURLParam('textfilter'); 
	expDebug::dPrint(' $mutipleTextFilterStr = ' . print_r($mutipleTextFilterStr, true) , 4);
		 
	if (!empty($mutipleTextFilterStr)){		
		 $loc_or_condition_obj1=getLocationFilterQuery($mutipleTextFilterStr,'1','loc','st','sc');    
		 expDebug::dPrint('$loc_or_condition_obj1 ='.print_r($loc_or_condition_obj1,true),5); 
		 $select->condition($loc_or_condition_obj1);     
	} 
  
  if ($op != "COUNT") {
    // Order the records
    resourceStmtAddOrderByClause($select, 'loc', "AZ");
    // Group by res.id to eliminate duplicate resource records
    $select->groupBy('sltm.id');
  }
  
  if ($op=="COUNT") {
    expDebug::dPrintDBAPI(' COUNT $select SQL = ' , $select);
  	
  	// Execute the query
	  $result = $select->execute()->fetchField();
	  expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
  }
  elseif ($op == "CSV") {
    expDebug::dPrintDBAPI(' CSV $select SQL = ' , $select);
  	
    // Execute the query and fetch all the records in an indexed array.
    $colHeadersMap = array(t('LBL311') => 'title',
                           t('LBL318') => 'capacity',
                           t('Equipment') => 'equipment',
                           t('LBL039') => 'country_name',
                           t('LBL102') => 'status'
                        );
   $filterArray[] = array(t('Location').':'.textboxFiltersetValueToPrintableStr('textfilter'));
   $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('resstatus'));
   $filterArray[] = array(t('LBL039').':'.checkboxFiltersetCountrySelectionsToPrintableStr('rescountry'));
   $filterArray[] = array(t('LBL708').':'.textboxFiltersetValueToPrintableStr('rescitystate'));
   $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
                          
   SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap ,'export', $filterArray, '', 'loc_admin_search'); 
  }
  elseif ($op == "PDF") {
    expDebug::dPrintDBAPI(' PDF $select SQL = ' , $select);    
    // % width of the columns must total to 100%
  	$colHeadersMap = array(t('LBL311') => array('title',        20),
                        	 t('LBL318') => array('capacity',     15),
  							           t('Equipment') => array('equipment',    30),
                        	 t('LBL039') => array('country_name', 20),
                           t('LBL102') => array('status',       15),
  	                    );
  	
   $appliedFilters = array(t('Location') => textboxFiltersetValueToPrintableStr('textfilter'),
                           t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('resstatus'),
                           t('LBL039') => checkboxFiltersetCountrySelectionsToPrintableStr('rescountry'),
                           t('LBL708') => textboxFiltersetValueToPrintableStr('rescitystate'),
                           ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
                          );

  SendNarrowSearchResultsAsPDFFile($select, t('Location'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'loc_admin_search');
    
  }
  else{ // LIST
	  // Execute the query and fetch all the records
  		add_row_id_condition($select, 'sltm', 'pk_id');
	  // Limit to $limit records
	  $select->range($start, $limit);

    expDebug::dPrintDBAPI(' LIST $select SQL = ' , $select);
	  
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

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function loc_admin_search_csv_row_data_alter($rowData) {
  try {
    $rowData[0]->equipment = _getEquipmentInfo($rowData[0]->equipment);
  }
  catch (Exception $ex) {
    watchdog_exception('loc_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function loc_admin_search_pdf_row_data_alter($rowData) {
  try {
    $rowData[0]->equipment = _getEquipmentInfo($rowData[0]->equipment);
  }
  catch (Exception $ex) {
    watchdog_exception('loc_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

function modifyLocationResultvalues($resultData) {
	try {
  $preResult = array();
  $i = 0;
  foreach($resultData as $res){
    $preResult[$i]['location_id'] = $res['location_id'];
    $preResult[$i]['title'] = $res['title'];
    $preResult[$i]['capacity'] = $res['capacity'];
    $preResult[$i]['equipment'] = _getEquipmentInfo($res['equipment']);    
    $preResult[$i]['country_name'] = $res['country_name'];
    $preResult[$i]['status'] = $res['status'];

    $i = $i+1;
  }
  expDebug::dPrint('Modify data resulttttt :'.print_r($preResult,true) , 4);
  return $preResult;
  //_getEquipmentInfo($listKey)
  } catch (Exception $ex) {
  	watchdog_exception('modifyLocationResultvalues', $ex);
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
    	$errobj->errormsg = "Error in Location/Facility Id.";
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
    	$errobj->errormsg = "Error in Location/Facility Id.";
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

function activateOrDeactivateLocation($locationId,$status='',$printResult=true){
  $updatedUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
  	$location_detailst = getLocationcountFromClass($locationId);
  	$locationcount = ((count($location_detailst)>0) ? count($location_detailst) : 0);
  	if($locationcount<=0)
  	{
	     $status  = getLocationStatus($locationId);
	     $updateStmt = db_update('slt_location');
	     $locationStatus  = ($status == 'lrn_res_loc_atv') ? "lrn_res_loc_itv" : "lrn_res_loc_atv";
	     
	   // add_audit_trail_entry($updatedUserId, $personId, 'cre_sys_obt_usr', 'exp_sp_administration_user', 'activateOrDeactivatePeople', 'Updated User Status', null, $status, $personStatus);
	   
	     
	    $updateStmt->condition('id', $locationId);
	    $updateStmt->fields(array(
	                       'status'      => $locationStatus,                                                  
	                       'updated_on'    => now(),
	                       'updated_by'    => $updatedUserId
	                       ));
	    $updateStmt->execute();
	    
	    if($printResult){
	      print   ($locationStatus == 'lrn_res_loc_atv') ? t('LBL572') : t('LBL573');
	    }
	  }
  	else{
			print 'not_able_do|'.$location_detailst[0]->loc_name;
  	}
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('activateOrDeactivateLocation', $ex);
    throw $ex;
  }
  unset($txn);
}

function getLocationcountFromClass($locationId) {
	try {
			$select = db_select('slt_course_class', 'cls');	
			$select->leftJoin('slt_location','loc','cls.location_id = loc.id');
			$select->addField('loc','name','loc_name');
			$select->addField('cls','id','cls_id');
			$select->condition('cls.status','lrn_cls_sts_atv','=');
			$select->condition('cls.location_id',$locationId);
			// Execute the query
			$result = $select->execute()->fetchAll();
			return $result;
		} catch (Exception $ex) {
		watchdog_exception('getLocationcountFromClass', $ex);
		expertusErrorThrow($ex);
	}
}

function getLocationStatus($locationId) {
	try {
  $select = db_select('slt_location', 'loc');
  $select->addField('loc','status', 'status');
  $select->condition('loc.id',$locationId);
  // Execute the query
  $locStatus = $select->execute()->fetchField();
  return $locStatus; 
  } catch (Exception $ex) {
  	watchdog_exception('getLocationStatus', $ex);
  	expertusErrorThrow($ex);
  }
}

function _getProfileStatusItemNamesKeyed($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  // Select the table
  try {
  $select = db_select('slt_profile_list_items','spli');
  
  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');
  
  // Order results by name
  $select->orderBy('name');
  
  // Set the conditions
  $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
  $select->condition('spli.code', array('lrn_res_loc_del')  , 'not in');
  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  if (!$alsoFetchInactiveItems) {
  	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }
  
  expDebug::dPrintDBAPI(' $select SQL = ' , $select);
  
  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('_getProfileStatusItemNamesKeyed', $ex);
  	expertusErrorThrow($ex);
  }
}

function _getEquipmentInfo($listKey) {
  try {
  $listKeys = explode(",",$listKey);
  $select = db_select('slt_profile_list_items','spli');
  
  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');
  
  // Order results by name
  $select->orderBy('name');
  
  // Set the conditions
  
  $select->condition('spli.code', $listKeys  , 'IN');
  //$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  if (!$alsoFetchInactiveItems) {
  	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }
  
  expDebug::dPrintDBAPI(' $select object = ' , $select);
  
  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $result12344555 = ' . print_r($result, true), 3);
  
  // Return results
  return implode(", ",$result);
  } catch (Exception $ex) {
  	watchdog_exception('_getEquipmentInfo', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the survey assessment
 */
function publishOrUnpublishLocationStatus($locationId, $printResult = true){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
  	$location_detailst = getLocationcountFromClass($locationId);
  	$locationcount = ((count($location_detailst)>0) ? count($location_detailst) : 0);
  	if($locationcount<=0)
  	{
	    $status  = getLocationStatus($locationId);
	    $updateStmt = db_update('slt_location');
	    $updatedstatus  = ($status=='lrn_res_loc_atv')? "lrn_res_loc_itv" : "lrn_res_loc_atv";  
	    
	    $updateStmt->condition('id', $locationId);
	    $updateStmt->fields(array(
	                        'status'      => $updatedstatus,                                                   
	                        'updated_on'    => now(),
	                        'updated_by'    => $currUserId
	                        ));                       
	    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);
	
	    $updateStmt->execute();
	   
	    add_audit_trail_entry($currUserId, $locationId, 'lrn_res_loc', 'exp_sp_administration_location', 'publishOrUnpublishLocationStatus', 'Updated Location Status',
			                                     null, $status, $updatedstatus);
	
		  if ($printResult) {
		  	$returnStatus  = ($updatedstatus=='lrn_res_loc_atv')?t('LBL572'):t('LBL573');
		    print $updatedstatus.'|'.$returnStatus;
		  }
  	}
  	else{
  		print 'not_able_do|'.$location_detailst[0]->loc_name;
  	}
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishLocationStatus', $ex);
    throw $ex;
  }
  unset($txn);
}

/*-- #35708: Session duplication issue while updating location --*/
function getLocationTimezone($locationId) {
 try {
  $select = db_select('slt_location', 'loc');
  $select->addField('loc','timezone', 'timezone');
  $select->condition('loc.id',$locationId);
  // Execute the query
  $locTimezone = $select->execute()->fetchField();
  return $locTimezone;
 } catch (Exception $ex) {
  watchdog_exception('getLocationTimezone', $ex);
  expertusErrorThrow($ex);
 }
}
?>