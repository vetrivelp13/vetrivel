<?php

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/**
 * Organization Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */


/*
 * orgJSObjectInfo() - Get module's javascript object's info.
 */
function orgJSObjectInfo() {
	try {
  $jsObjectInfo = array();
  $jsObjectInfo['name'] = 'narrowsearch'; // javascript object name (from exp_sp_administration_organization.js)
  $jsObjectInfo['init_id'] = 'root-admin'; // div id on which the javascript object is to be loaded.
  $jsObjectInfo['filterSets']=array(array("type"=>"checkbox","code"=>"orgstatus"),
  array("type"=>"checkbox","code"=>"orgtype")
  );
  $jsObjectInfo['searchBasePath'] = 'administration/people/organization/search/all/'; // search path
  $jsObjectInfo['lastSortType'] = 'AZ'; // Loading data mode in ascending / decending order
  $jsObjectInfo['lastSortTypeHtmlId'] = 'org-sort-az'; // Html id for sorting
  $jsObjectInfo['showExportOption'] = true; // Show export option TRUE or FALSE;
  $jsObjectInfo['resultsExportPath'] = 'administration/people/organization/export/'; // Path for result export
  $jsObjectInfo['showPrintOption'] = true; // Show print option True or False;
  $jsObjectInfo['resultsPrintPath'] = 'administration/people/organization/print/'; // Path for result print
  $jsObjectInfo['showTopTextFilter'] = true; // Show top text filter option TRUE or FALSE
  $jsObjectInfo['text_filter_ac_path'] = 'administration/people/organization/textfilter-autocomplete'; // Path for autocomplete at Text filter
  $jsObjectInfo['showAddOption'] = true; // Show add button option TRUE or FALSE
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('orgJSObjectInfo', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getOrgTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */
function getOrgTabContentType(){
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getOrgTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * orgNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function orgNarrowSearchFiltersHTML() {
	try {
  $checkedStatus = array();
  $checkedStatus["cre_org_sts_act"] = TRUE;
  $allStatus = getProfileItemNamesKeyed('cre_org_sts_');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

  $allOrgTypes = getProfileItemNamesKeyed('cre_org_typ_');
  expDebug::dPrint(' $allOrgTypes = ' . print_r($allOrgTypes, true) , 4);

  $filterSetforJS=array(array('type'=>'checkbox','code'=>'orgstatus'),
  array('type'=>'checkbox','code'=>'orgtype'),
  );

  return theme('narrow_search_filters_holder',
  array('rendered_filtersets_list' => array(
  		theme('narrow_search_filterset_checkboxes',
  				array('code' => 'orgstatus',
  						'title' => t('LBL102'),
  						'divid' => 'status',
  						'checkboxes_list' => $allStatus)
  				// 'checked_list' => $checkedStatus)
  		),
  		theme('narrow_search_filterset_checkboxes',
  				array('code' => 'orgtype',
  						'title' => strtolower(t('LBL036')),
  						'divid' => 'type',
  						'checkboxes_list' => $allOrgTypes)
  		)
  ),
                      'filter_set_list' => drupal_json_encode($filterSetforJS),
  ));
  } catch (Exception $ex) {
  	watchdog_exception('orgNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * orgSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function orgSearchResultsSortbarHTML() {
 try {
  $sortbarLinksList = array();

  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('AZ'), 'sort_type' => 'AZ', 'html_id' => 'org-sort-az');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('ZA'), 'sort_type' => 'ZA', 'html_id' => 'org-sort-za');
  //$sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('Number'), 'sort_type' => 'Number', 'html_id' => 'org-sort-num');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('NewlyListed'), 'sort_type' => 'NewlyListed', 'html_id' => 'org-sort-new');
  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'org-sort-new'
  					           );

  					           return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
  					           )
  					           );

} catch (Exception $ex) {
	watchdog_exception('orgSearchResultsSortbarHTML', $ex);
	expertusErrorThrow($ex);
}
}


/*
 * orgSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function orgSearchResultsActionbarHTML() {
 try {
  $actionbarLinksListforJS=array('export_path'=>'administration/people/organization/export/',
  					             'print_path'=>'administration/people/organization/print/'
  					             );
  					             return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL158'),
												 'add_page_path' => base_path() . 'administration/people/organization/nojs/addedit',
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
                         	'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  							          'ctools_style'=>'ctools-modal-ctools-admin-organization-addedit-style',
  					             	'page_code' => 'cre_org',
  					             )
  					             );

  					             //return theme('narrow_search_actionbar', array('js_object_info' => $jsObjectInfo, 'add_button_title' => t('Add Organization'), 'add_page_path' => base_path() . 'administration/people/organization/nojs/addedit'));
  					             /*
  					              return theme('narrow_search_actionbar', array('js_object_info' => $jsObjectInfo, 'add_button_title' => t('Add Organization'), 'add_page_path' => base_path() . 'administration/people/organization/addedit'));
  					              */
} catch (Exception $ex) {
	watchdog_exception('orgSearchResultsActionbarHTML', $ex);
	expertusErrorThrow($ex);
}
}
/*
 * getOrgTabContentMainHTML - Returns the html for tab content main
 */
function getOrgTabContentMainHTML() {
	try{
  $tabContentTypeArr =  getOrgTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/people/organization/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/people/organization/textfilter-autocomplete'
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
  	watchdog_exception('getOrgTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrgTabContent - Returns the html for narrow search block on organization search page
 */
function getOrgTabContent() {
	try {
  $tabContentTypeArr =  getOrgTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];

  $orgDetailsResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => orgNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar' => orgSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => orgSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getOrgTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','people'),
  );
  echo drupal_json_encode($orgDetailsResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getOrgTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getChildOrgsCount() - Function to get the count of the first level child organizations (active only) of an organization
 * SELECT * FROM slt_organization WHERE parent_org_id=1 AND STATUS='cre_org_sts_act'
 */
function getChildOrgsCount($orgId) {
  // Select slt_organization table
  try {
  $select = db_select('slt_organization', 'org');

  // Get count of active organizations
  $select->addExpression('COUNT(org.id)', 'count');

  // Set the conditions
  if(is_array($orgId)) {
  	$select->addField('org', 'parent_org_id', 'parent_org_id');
  	$select->condition('org.parent_org_id', $orgId, 'IN');
  	$select->groupBy('org.parent_org_id');
  } else {
  	$select->condition('org.parent_org_id', $orgId, '=');
  }
  
  $select->condition('org.status', 'cre_org_sts_act', '=');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execure the query
  if(is_array($orgId)) {
  	$numOrg = $select->execute()->fetchAllAssoc('parent_org_id');
  } else {
  	$numOrg = $select->execute()->fetchField();
  }
  
  expDebug::dPrint(' $numOrg = ' . print_r($numOrg, true) , 3);

  return $numOrg;
  } catch (Exception $ex) {
  	watchdog_exception('getChildOrgsCount', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getOrgLearnersCount() - Function to get the count of active learners from an organization.
 * SELECT * FROM slt_person WHERE org_id=1 AND STATUS='cre_usr_sts_atv'
 */
function getOrgLearnersCount($orgId) {
  // Select slt_organization table
  try {
  $select = db_select('slt_person', 'person');

  // Get count of active organizations
  $select->addExpression('COUNT(person.id)', 'count');

  // Set the conditions
  $select->condition('person.status', 'cre_usr_sts_atv', '=');
  if(is_array($orgId)) {
  	$select->addField('person', 'org_id', 'org_id');
  	$select->condition('person.org_id', $orgId, 'IN');
  	$select->groupBy('person.org_id');
  } else {
  	$select->condition('person.org_id', $orgId, '=');
  }
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  // Execure the query
  if(is_array($orgId)) {
  	$numLearners = $select->execute()->fetchAllAssoc('org_id');
  } else {
  	$numLearners = $select->execute()->fetchField();
  }
  expDebug::dPrint(' $numLearners = ' . print_r($numLearners, true) , 3);

  return $numLearners;
  } catch (Exception $ex) {
  	watchdog_exception('getOrgLearnersCount', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrganizationDetailsHTML() - Renders an organization's details as HTML to be displayed in jqGrid.
 */
function getOrganizationDetailsHTML($org) {
	try {
  global $theme_key;
  $detailsList = array();
  $actionList = array();
  $newActionList = array();
  expDebug::dPrint('organisation id--->>'.print_r($org,true),5);
  $org->entityType = 'cre_org';
  if (!empty($org->number)) {
    $additionalTitleInfo = $org->number;
  }
  if (!empty($org->type)) {
    $detailsList[] = array('detail' => t($org->type), 'tooltip' => t('LBL244').': '.$org->type);
  }
  /*  if (!empty($org->status)) {
   $detailsList[] = array('detail' => $org->status, 'tooltip' => t('LBL102'));
   }
   */
//   $childOrgCount= getChildOrgsCount($org->id);
  $detailsList[] = array('detail' => t('LBL159').':'.' '.$org->child_orgcount, 'tooltip' => t('LBL159'));


//   $learnerCount=getOrgLearnersCount($org->id);
  //$detailsList[] =  array('detail' => t('LBL160').':'.' '.$learnerCount, 'tooltip' => t('LBL160'));



  expDebug::dPrint(' $detailsList = ' . print_r($detailsList, true) , 4);
  $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/people/organization/nojs/addedit',
					  'action_button_params' => $org->id,
  					  'ctools_style' => 'ctools-modal-ctools-admin-organization-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                      'tooltip' => t('LBL063')
  );

  $actionList[] = array('action_link_text' => t('LBL286'),
      'action_page_path' => '',
      'action_button_params' => $org->id,
      'action_button_param_text' => 'Organization',
      'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
      'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
      'tooltip' => t('LBL286')
  );
//   $orgActiveUserscount = getOrganizationUsersResult($org->id,'',false,true);
  $orgActiveUserscount = $org->active_users_count;
  expDebug::dPrint('$orgActiveUserscount'.t('LBL572').' ='.$orgActiveUserscount);
  //44133: German-old-ui-Icon not changing when user is activate and suspend
  if(strtolower($org->status_code) == 'cre_org_sts_act'){

  	expDebug::dPrint('$org->id ='.$org->id);
    $newActionList[] = array('action_link_text' => t('LBL572'),
				  		'action_page_path' => '',
				  		'action_button_params' => $org->id,
    					'action_org_users_count' => $orgActiveUserscount,
    					'action_button_params_text' => 'deactivate',
					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL572'),
                        'icon_style_class'=>'active-deactive-tab-icon  Suspend'
                        );
  } else {
    $newActionList[] = array('action_link_text' => t('LBL573'),
				  		'action_page_path' => '',
				  		'action_button_params' => $org->id,
    					'action_button_params_text' => 'activate',
					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL573'),
                        'icon_style_class'=>'active-deactive-tab-icon  Activate'
                        );
  }
  //Added by Velu
  $orgActiveUsers = t('LBL160').': '.$orgActiveUserscount;

  $newActionList[] = array('action_link_text' => $orgActiveUsers,
				  		'action_page_path' => '',
				  		'action_button_params' => $org->id,
				  		'action_button_params_text' => 'orgusers',
  						'action_org_users_count' => $orgActiveUserscount,
				  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
				  		'js_object_info' => array (
				  				'name' => 'narrowsearch', // provide the java script object name
				  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
				  		),
				  		'tooltip' => t('LBL160'),
				  		'orgid'=> $org->id,
				  		'icon_style_class'=>'users-tab-icon vtip'
  );//End

  $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $org->id,
  		'action_button_params_text' => 'access',
  		//'action_org_users_count' => $orgActiveUserscount,
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL160'),
  		'orgid'=> $org->id,
  		'entityId' => $org->id,
  		'entityType' => 'cre_org',
  		'icon_style_class'=>'access-tab-icon',
  		'postype' => 'middle',
  		'poslwid' => 20,
  );
  $viewOrglist[] = array('action_link_text' => 'Organization',
  		'action_page_path' => 'administration/view-org/nojs/addedit/'.$org->id.'',
  		'action_button_params' =>  $org->id,
  		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  );

  //print_r($detailsList);
  return theme('narrow-search-results-item-details',
  array( 'title' => sanitize_data($org->name),
  //'additional_title_info' => trim($additionalTitleInfo),
                       	'delete_info' => t('MSG357').' '.strtolower(t('Organization')),
                       	'details_list'=>$detailsList,
                 		    'action_list'=> $actionList,
                        'new_action_list'=>$newActionList,
                        'object_type'=>'Organization',
                        'description'=>$org->description,
  		                  'sumedit' => $org->sumedit,
  		                  'sumdelete' => $org->sumdelete,
  											'view_list' =>$viewOrglist
  )
  );
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrganizationActionsHTML() - Renders an organization's actions as HTML to be displayed in jqGrid.
 */
function getOrganizationActionsHTML($org) {
  //'js_object_info' => orgJSObjectInfo(),
  try {
  return theme('narrow-search-results-item-actions',
  array('edit_page_path' => base_path() . 'administration/people/organization/nojs/addedit',
  //'edit_page_path' => base_path() . 'administration/people/organization/addedit',
                         'edit_button_action_params' => array($org->id),
                   	     'ctools_style'=>'ctools-modal-ctools-admin-organization-addedit-style',
  )
  );
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * orgSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function orgSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
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

    case "Number" :
      $field = $tableAlias . ".number";
      $direction = "ASC";
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
  	watchdog_exception('orgSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getOrgSortTitle() - get sort title for $code argument if provided or for code in url param sortby
 */
function getOrgSortTitle($code = '') {
	try {
  $code = empty($code)? getURLParam('sortby') : $code;
  switch ($code) {
    case "AZ" :
      return t('LBL017');

    case "ZA" :
      return t('LBL018');

    case "Number" :
      return t('LBL161');

    case "NewlyListed" :
      return t('LBL044');

    default:
      return t('LBL044');
  } // end switch
  } catch (Exception $ex) {
  	watchdog_exception('getOrgSortTitle', $ex);
  	expertusErrorThrow($ex);
  }
}



/*
 * sendOrganizationsListToJqGrid() - Prepares JSON response to JqGrid's request for organization
 */
function sendOrganizationsListToJqGrid() {
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

  /* $numOrg = fetchOrganizationsAll('COUNT');
  expDebug::dPrint(' $numOrg = ' . print_r($numOrg, true) , 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numOrg);
   */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);


  $orgList = fetchOrganizationsAll('LISTS', $start, $limit);
  expDebug::dPrint(' $orgList = ' . print_r($orgList, true) , 4);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
//   $response->total = $total_pages;
//   $response->records = $numOrg;

  // $jsObjectInfo = orgJSObjectInfo();
  //$response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
  // have to do this.
  $response->initial_sort_type_html_id = 'org-sort-new';

  //$response->filters_html = orgNarrowSearchFiltersHTML($jsObjectInfo);
  $orgIds = array_map(create_function('$org', 'return $org->id;'), $orgList);
  expDebug::dPrint('all org ids '.print_r($orgIds, 1), 5);
  $child_orgs_count = array();
  if(!empty($orgIds)) {
  	$child_orgs_count = getChildOrgsCount($orgIds);
  }
  expDebug::dPrint('getChildOrgsCount of all org ids '.print_r($child_orgs_count, 1), 5);
  $active_users_count = array();
  if(!empty($orgIds)) {
  	$active_users_count = getOrganizationUsersResult($orgIds, '', false, true);
  }
  
  expDebug::dPrint('getOrganizationUsersResult of all org ids '.print_r($active_users_count, 1), 5);
  $i=0;
  foreach ($orgList as $org) {
    expDebug::dPrint(' $org = '. print_r($org, true) , 4);
    expDebug::dPrint('start time of $org = '. $org->name , 5);
    $org->row =  $i;
    $org->child_orgcount = (isset($child_orgs_count[$org->id]) ? $child_orgs_count[$org->id]->count : 0);
    $org->active_users_count = (isset($active_users_count[$org->id]) ? $active_users_count[$org->id]->count : 0);
    $response->rows[$i]['id']   = $org->id;
	//'image' => theme('narrow-search-results-item-image', array('results' => $org))
    $response->rows[$i]['cell'] = array('image' => '',
                                         'details' => getOrganizationDetailsHTML($org),
                                         'action' => getOrganizationActionsHTML($org)
    );

    $i++;
    expDebug::dPrint('end time of $org = '. $org->name , 5);
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendOrganizationsListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrganizationTextFilterAutoComplete() - Returns the matched organization title
 */
function getOrganizationTextFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_organization', 'org');

  // Add fields to fetch
  $select->addExpression('DISTINCT(org.name)', 'name');

  // Add condition
  $select->condition('org.name', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('org.status',array('cre_org_sts_itv','cre_org_sts_act'),'IN');
  filterByUserAccessPrivileges($select,'org','',1,'cre_org','autocomplete','org');
  // Order in dictionary sort order
  $select->orderBy('name');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  $matchingOrgNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingOrgNames = '. print_r($matchingOrgNames, true) , 3);

  foreach ($matchingOrgNames as $orgName) {
    print rawurlencode($orgName->name) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addNewOrganization() - Save a new organization details in the database
 */
function addNewOrganization($form, &$form_state, $paramuserid="", $callFromAPI = 0,$dtLoad='')
{
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  $userid = $paramuserid;
  $txn = db_transaction();
  try {

    $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
    $in_active_on          = $form_state['values']['status'] == 'cre_org_sts_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'cre_org_sts_itv' ? $inactivereasontext : NULL;
    if(!$callFromAPI)
    $description = $form_state['values']['description']['value'];
    if($callFromAPI){
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      $description = $form_state['values']['description'];
    }
    //Organization number should be generated and inserted and it is consider as a unique column.
    if(empty($form_state['values']['number'])){
	    $select = db_select('slt_organization', 'org');
	    $select->addExpression('max(org.id)','id');
	    $LastInsertId = $select->execute()->fetchField();
	    $org_number = 'org_'.($LastInsertId+1);
    }else{
    	$org_number = $form_state['values']['number'];
    }
    $apiname = (isset($_GET['apiname']) ? $_GET['apiname'] : $_POST['apiname']);
    expDebug::dPrint('$apiname : '.print_r($apiname , true), 4);
    if(!empty($form_state['values']['parent']) && $apiname == 'AddOrganizationAPI'){ // Parent Organaisation Exist Check
    	expDebug::dPrint('Parent Org Id : '.$form_state['values']['parent'] , 4);
    	include_once(drupal_get_path('module', 'exp_sp_administration_user') .'/exp_sp_administration_user.inc');
    	$orgExist = is_organization($form_state['values']['parent']); // Checking Org Exist
    	if (empty($orgExist)) {
    		if($callFromAPI){
	    		$errobj=new stdClass();
	    		$errobj->isValidateError = 1;
	    		$errobj->errcode = 'L_012';
	    		$errobj->errormsg = "Parent Organization does not exist.";
	    		return $errobj;
    		}
    		else
    			return array((object)array('id'=>'Parent Organization does not exist.'));
    	}
    }
    expDebug::dPrint(' $org_number = ' . $org_number);
    
    if($callFromAPI)
    {
    $ret_tmp = getcustomattributemodulestatus();
    if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
        $custom_attr_error = validateCustomAttribute($form_state,'cre_org');
        expDebug::dPrint('Create Org API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
        if($custom_attr_error != '') {
            $customErrmsg=new stdClass();
            $customErrmsg->isValidateError = 1;
            $customErrmsg->errormsg = $custom_attr_error;
            return $customErrmsg;
        }
    }
    }
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_organization');
    $fields = array(
                'name'              => $form_state['values']['name'],
    			'number'			=> $org_number,
                'description'       => $description,
    			//'number'          => $form_state['values']['number'],
                'type'              => $form_state['values']['type'],
                'status'            => 'cre_org_sts_act',
   				/*'add1'            => $form_state['values']['add1'],
     			'add2'              => $form_state['values']['add2'],
     			'city'              => $form_state['values']['city'],
     			'state'             => $form_state['values']['state'],
    			'state' 			=> $stateCode,
    			'country'           => $form_state['values']['country'],
     			'zip'               => $form_state['values']['zip'],*/
                'contact_name'      => $form_state['values']['contact'],
    			/*'phone_no'        => $form_state['values']['phone_no'],
     			'email'             => $form_state['values']['email'],*/
                'cost_center'       => $form_state['values']['cost_center'],
    			//'additional_info' => $form_state['values']['additional_info'],
    			/*'in_active_on'    => $in_active_on,
     			'in_active_reason'  => $in_active_reason,*/
                'created_by'        => $userid,
                'created_on'        => now(),
    			'updated_by'        => $userid,
    			'updated_on'        => now(),
    			'custom_dataload'   => ($dtLoad == 'DAPI') ? $form_state['values']['custom_dataload'] : NULL
    		);

    $userid = ($userid == 'Feed')? 1 : $userid;

    // Parent organization
    if (!empty($form_state['storage']['acvalues']['parentorg']['id'])) {

      $fields['parent_org_id'] = $form_state['storage']['acvalues']['parentorg']['id'];

      // We may have to clean up $form_state input and values for parent_org_id and parent fields,
      // so that they are refreshed with the latest db values
      if (empty($form_state['values']['parent_org_id']) ||
      $form_state['values']['parent_org_id'] != $form_state['storage']['acvalues']['parentorg']['id'] ||
      empty($form_state['values']['parent']) ||
      $form_state['values']['parent'] != $form_state['storage']['acvalues']['parentorg']['name']) {
        unset($form_state['input']['parent']);
        unset($form_state['values']['parent']);
        unset($form_state['input']['parent_org_id']);
        unset($form_state['values']['parent_org_id']);
      }
    }
    else {

      $fields['parent_org_id'] = NULL;

      unset($form_state['input']['parent']);
      unset($form_state['values']['parent']);
      unset($form_state['input']['parent_org_id']);
 
      unset($form_state['values']['parent_org_id']);
    }

    // Select the fields and the values to be inserted
    $fields = trimInputFields($fields); // #42124 - Trim input values
    $insertStmt->fields($fields);

    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

    // Execute the insert statement. Fetch the new org id.
    $orgId = $insertStmt->execute();
    expDebug::dPrint(' $orgId = ' . print_r($orgId, true) , 3);

    //Update Organization number
    if($callFromAPI) {
    	$classCreateUser = $paramuserid;
    }
    else {
    	$classCreateUser = getIdOfLoggedInUser();
    }
    updateGroupMappingDetails($orgId, 'cre_org', $classCreateUser,'emptyadd',0);
    
    // Save the new organization's id in the hidden id form field.
    $form_state['values']['id'] =  $orgId;

    // Remove tmp space
    unset($form_state['storage']['acvalues']);

    // Generate an organization number from org id for this organization and save it in the slt_organization table.
    /* generateAndSaveOrgNumber($form, $form_state,$userid); */ // Saved the number in $form_state['values']['number']

    addTaxonomyTermForOrg($orgId, $form_state['values']['name'],'','cre_org_sts_act');

    // Insert Tax Exempted value in slt_entity_profile_mapping table
    saveTaxExemptedOrgCustomProfile($orgId, $form_state['values']['tax_exempted'], $userid);

    $formFieldsList = array('name', 'description', 'type', 'contact_name','cost_center');
    if ($callFromAPI == 0) {
      $formFieldsList[] = 'parent_org_id';
    }

    make_audit_trail_entries($fields, $userid, null, $orgId , 'Inserted', 'cre_sys_obt_org', 'exp_sp_administration_organization', 'addNewOrganization',
    $formFieldsList);

    //if($callFromAPI && module_exists('exp_sp_administration_customattribute')){     //#custom_attribute_0078975 - Check module status/Save Custom Attributes
    if($callFromAPI)
    {
    	$ret_tmp = getcustomattributemodulestatus();
    	if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        saveCustomAttributesInAddEditForm($form, $form_state, $orgId, 'cre_org', $userid,'Inserted',1);
    }
    } 
    if($dtLoad == 'DAPI' && !empty($orgId)){
      	return array((object)array('status'=>'success'));
    }

    return array((object)array('id'=>$orgId));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewOrganization', $ex);
    if($dtLoad == 'DAPI'){
    	return array((object)array('status'=>'failure','remarks'=>$ex->getMessage()));
    }
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in create organization";
    	return $errobj;
    }else
    		return array((object)array('id'=>"failure"));
  }
}

/*
 * generateAndSaveOrgNumber - Generates an organization number from org id for this organization and
 *                            saves it in the slt)organization table.
 */
/*
 function generateAndSaveOrgNumber($form, &$form_state,$userid) {
 expDebug::dPrint('exp_sp_administration_organization.inc : generateAndSaveOrgNumber() : $form = ' . print_r($form, true));
 expDebug::dPrint('exp_sp_administration_organization.inc : generateAndSaveOrgNumber() : $form_state = ' . print_r($form_state, true));



 // Construct the organization number from the organization id
 $orgNumber =  str_pad( $form_state['values']['id'], 9, 0, STR_PAD_LEFT);

 // Select table to update
 $updateStmt = db_update('slt_organization');

 // Set conditions
 $updateStmt->condition('id', $form_state['values']['id']);

 // Set new values for field(s)
 $updateStmt->fields(array(
 'number' => $orgNumber,
 'updated_by' => $userid,
 'updated_on' => now()
 ));

 expDebug::dPrint('exp_sp_administration_organization.inc : generateAndSaveOrgNumber() : $updateStmt obj = ' . print_r($updateStmt, true));
 expDebug::dPrint('exp_sp_administration_organization.inc : generateAndSaveOrgNumber() : $updateStmt SQL = ' . print_r($updateStmt->__toString(), true));

 // Execute query
 $numUpdated = $updateStmt->execute();
 expDebug::dPrint('exp_sp_administration_organization.inc : generateAndSaveOrgNumber() : $numUpdated = ' . print_r($numUpdated, true));

 // Save the number on the form.
 $form_state['values']['number'] = $orgNumber;
 }
 */
/*
 * addNewTaxonomyTermForNewOrg() - Adds new taxonomy data and hieararchy entry
 *     SELECT SUBSTRING_INDEX(SUBSTRING_INDEX(VALUE,"\"",-2),"\"",1) INTO orgvocabularyno FROM variable WHERE NAME LIKE 'organization_nav_vocabulary';
 IF orgvocabularyno IS NOT NULL THEN
 SELECT COUNT(*) INTO term_org_count FROM taxonomy_term_data WHERE vid=orgvocabularyno AND description=xid;
 IF term_org_count > 0 THEN
 IF xstatus = 'cre_org_sts_act' THEN
 UPDATE taxonomy_term_data SET
 `name` = CONCAT(xname,'-',xnumber),
 `weight` = 0
 WHERE vid=orgvocabularyno AND description = xid;
 ELSE
 UPDATE taxonomy_term_data SET
 `name` = CONCAT(xname,'-',xnumber,' (InActive)'),
 `weight` = 1
 WHERE vid=orgvocabularyno AND description=xid;
 END IF;
 ELSEIF xstatus = 'cre_org_sts_act' THEN
 INSERT INTO taxonomy_term_data(
 `vid`,
 `name`,
 `description`
 ) VALUES (
 orgvocabularyno,
 CONCAT(xname,'-',xnumber),
 xid
 );
 SELECT LAST_INSERT_ID() INTO xterminsid FROM DUAL;
 INSERT INTO taxonomy_term_hierarchy(
 `tid`
 ) VALUES (
 xterminsid
 );
 END IF;

 */

function addTaxonomyTermForOrg($orgId, $orgName, $orgNumber, $status) {
  expDebug::dPrint(' $orgId = ' . print_r($orgId, true) , 4);
  $orgName = trimInput($orgName);
  $orgNumber = trimInput($orgNumber);
  $orgVocabularyNumber = variable_get('organization_nav_vocabulary');

  if (empty($orgVocabularyNumber)) {
    expDebug::dPrint(' variable organization_nav_vocabulary not set or set to null');
    return;
  }

  $txn = db_transaction();

  try {
    $orgTermCount = getTaxonomyTermCount($orgVocabularyNumber, $orgId);

    if ($orgTermCount <= 0 && $status == 'cre_org_sts_act') {
      // Select the table
      $insertStmt1 = db_insert('taxonomy_term_data');

      // Select the fields and the values to be inserted
      $insertStmt1->fields(array(
        'vid' => $orgVocabularyNumber,
        'name' => $orgName . '-' . $orgNumber,
        'description' => $orgId
      ));

      expDebug::dPrintDBAPI(' $insertStmt1 obj = ' , $insertStmt1);

      // Execute the insert statement. Fetch the new taxonomy term data id for saving in taxonomy_term_hiearchy.
      $taxonomyTermDataId = $insertStmt1->execute();
      expDebug::dPrint(' $taxonomyTermDataId = ' . print_r($taxonomyTermDataId, true) , 3);

      // Select the table to insert the record in
      $insertStmt2 = db_insert('taxonomy_term_hierarchy');

      // Select the fields and the values to be inserted
      $insertStmt2->fields(array(
        'tid' => $taxonomyTermDataId
      ));

      expDebug::dPrintDBAPI(' $insertStmt2 obj = ' , $insertStmt2);

      // Execute the insert statement. Fetch the new org id.
      $taxonomyTermHiearchyId = $insertStmt2->execute();
      expDebug::dPrint(' $taxonomyTermHiearchyId = ' . print_r($taxonomyTermHiearchyId, true) , 3);
    }
    else if ($orgTermCount > 0) {

      $orgTermName = $orgName . '-' . $orgNumber . ' (InActive)';
      $weight = 1;

      if ($status == 'cre_org_sts_act') {
        $orgTermName = $orgName . '-' . $orgNumber;
        $weight = 0;
      }

      // Select table to update
      $updateStmt = db_update('taxonomy_term_data');

      // Set conditions
      $updateStmt->condition('vid', $orgVocabularyNumber);
      $updateStmt->condition('description', $orgId);

      // Select the fields and the values to be updated
      $updateStmt->fields(array(
        'name' => $orgTermName,
        'weight' => $weight
      ));

      expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);

      // Execute query
      $numUpdated = $updateStmt->execute();
      expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addTaxonomyTermForOrg', $ex);
  }
}

/*
 * getColumnForTaxExemptedOrg - Returns the column name in slt_entity_profile_mapping used for saving tax exempted flag
 */
function getColumnForTaxExemptedOrg() {
	try {
  return 'col3'; // Hard-coded because we cannot let admin re-configure tax exempted. It will always show on org add screen as radios.
  // If required, can be replaced with concat('col' , (select cols from slt_profile_config where code = 'cre_prf_ctg_cus_tax' and lang_code='cre_sys_lng_eng'))
  } catch (Exception $ex) {
  	watchdog_exception('getColumnForTaxExemptedOrg', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * saveTaxExemptedOrgCustomProfile() - Saves an organization's tax exemption status.
 */
function saveTaxExemptedOrgCustomProfile($orgId, $isTaxExempted,$userid) {
  expDebug::dPrint(' $orgId = ' . print_r($orgId, true) , 4);



  $colName = getColumnForTaxExemptedOrg();

  $entityType = getCodeForCustomProfile('Organization');

  $choiceId = getProfileItemIdFromCode($isTaxExempted);

  $txn = db_transaction();

  try {
    //If an entry for ($entityType, $orgId)) exists in slt_entity_profile_mapping table update $colName
    //else insert a new row for $entityType, $orgId saving $choiceId in $colName
    if (existsEntityProfileMapingRow($entityType, $orgId)) {
      // Select table to update
      $updateStmt = db_update('slt_entity_profile_mapping');

      // Set conditions
      $updateStmt->condition('entity_type', $entityType);
      $updateStmt->condition('entity_id', $orgId);

      // Select the fields and the values to be updated
      $updateStmt->fields(array(
      $colName => $choiceId,
        'updated_by' => $userid,
        'updated_on' => now()
      ));

      expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);

      // Execute query
      $numUpdated = $updateStmt->execute();
      expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);

    }
    else {
      // Select the table to insert the record in
      $insertStmt = db_insert('slt_entity_profile_mapping');

      // Select the fields and the values to be inserted
      $insertStmt->fields(array(
        'entity_id'   => $orgId,
        'entity_type' => $entityType,
      	$colName      => $choiceId,
        'created_by'  => $userid,
        'created_on'  => now(),
      	'updated_by'  => $userid,
      	'updated_on'  => now()
      ));

      expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

      // Execute the insert statement.
      $insertId = $insertStmt->execute();
      expDebug::dPrint(' $insertId = ' . print_r($insertId, true) , 3);
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('saveTaxExemptedOrgCustomProfile', $ex);
  }
}

/*
 * getTaxExemptedOrgCustomProfile() - Fetches an organization's tax exempted flag from slt_entity_profile_mapping table
 */
function getTaxExemptedOrgCustomProfile($orgId) {
	try {
  expDebug::dPrint(' $orgId = ' . print_r($orgId, true) , 4);

  $colName = getColumnForTaxExemptedOrg();

  $entityType = getCodeForCustomProfile('Organization');

  $isTaxExempted = getProfileSettingsColumn($entityType, $orgId, $colName);
  expDebug::dPrint(' $isTaxExempted = ' . print_r($isTaxExempted, true) , 4);

  if (empty($isTaxExempted)) {
    return 'cre_sys_yon_no';
  } else {
    return getProfileItemCodeFromId($isTaxExempted);
  }
  } catch (Exception $ex) {
  	watchdog_exception('getTaxExemptedOrgCustomProfile', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getDefaultOrgAddEditFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultOrgAddEditFormFieldValue($field, $form_state, $orgDetails) {
	try {
  $defaultValue = '';

  switch ($field) {
    case 'tax_exempted' :
      if (! empty($form_state['values']['tax_exempted'])) {
        $defaultValue = $form_state['values']['tax_exempted'];
      }
      else if (empty($orgDetails)) {
        $defaultValue = 'cre_sys_yon_no';
      }
      else {
        $isTaxExempted = getTaxExemptedOrgCustomProfile($orgDetails['id']);
        expDebug::dPrint(' $isTaxExempted = ' . print_r($isTaxExempted, true) , 4);

        $defaultValue = $isTaxExempted;
      }
      break;

    case 'parent': // Cannot have underscore in autocomplete field id
      $dbField = 'parent_org_id';
      expDebug::dPrint(' $orgDetails[parent_org_id] ' .
      print_r($orgDetails[$dbField], true) , 4);
      expDebug::dPrint(' $form_state[values][parentorg] ' .
      print_r($form_state['values'][$field], true) , 5);
      $defaultValue = empty($form_state['values'][$field]) ?
      (empty($orgDetails[$dbField]) ? '' : getOrgName($orgDetails[$dbField]))
      : $form_state['values'][$field];
      break;

    case 'contact': // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['contact']) ?
      (empty($orgDetails['contact_name']) ? '' : $orgDetails['contact_name'])
      : $form_state['values']['contact'];
      break;
    case 'description':
      $defaultValue = empty($form_state['values']['description']['value']) || $form_state['values']['description']['value']==''? (empty($orgDetails['description']) || $orgDetails['description']==''? '' : $orgDetails['description']) : $form_state['values']['description']['value'];
      break;
    default:   // For all other fields
      $defaultValue = empty($form_state['values'][$field]) ?
      (empty($orgDetails[$field]) ? '' : $orgDetails[$field])
      : $form_state['values'][$field];
      break;
  } // end switch

  expDebug::dPrint(' $defaultValue for field ' .
  $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultOrgAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * updateOrganizationDetails - Updates an existing organization details when the user clicks save
 */
function updateOrganizationDetails($form, &$form_state, $paramuserid="", $callFromAPI = 0,$dtLoad='') {
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  $userid = $paramuserid;
  $txn = db_transaction();
  try {
    /*     $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
     $in_active_on          = $form_state['values']['status'] == 'cre_org_sts_itv' ? now() : NULL;
     $in_active_reason      = $form_state['values']['status'] == 'cre_org_sts_itv' ? $inactivereasontext : NULL;
     */
  	if(!$callFromAPI)
    $description = $form_state['values']['description']['value'];
    if($callFromAPI){
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      $description  = $form_state['values']['description'];
      $orgids 		= getImmediateChildrenOrgIds($form_state['values']['id']); // Child Org Id
      expDebug::dPrint('$orgids : '.print_r($orgids , true), 4);
    }

    $apiname = (isset($_GET['apiname']) ? $_GET['apiname'] : $_POST['apiname']);
    if(!empty($form_state['values']['parent']) && $apiname == 'UpdateOrganizationAPI'){ // same Organaisation Update Exist Check
    	if ($form_state['values']['parent'] == $form_state['values']['id']) {
    		if($callFromAPI){
	    		$errobj=new stdClass();
	    		$errobj->isValidateError = 1;
	    		$errobj->errcode = 'L_012';
	    		$errobj->errormsg = 'Parent Org id and Org id should not be same';
	    		return $errobj;
    		}else{
    			return array((object)array('id'=> 'Parent Org id and Org id should not be same'));
    		}
    	}
    	if(in_array($form_state['values']['parent'], $orgids)) { // Organisation child Check
    		if($callFromAPI){
    			$errobj=new stdClass();
    			$errobj->isValidateError = 1;
    			$errobj->errcode = 'L_012';
    			$errobj->errormsg = "Parent Org id and Child Org id should not be same";
    			return $errobj;
    		}else
    			return array((object)array('id'=> 'Parent Org id and Child Org id should not be same'));
    	}
    }
    // Select table to update
    $updateStmt = db_update('slt_organization');

    // Set conditions
    $updateStmt->condition('id', $form_state['values']['id']);
    $formid=$form_state['values']['id'];
    $default= db_query("SELECT id,name,description, parent_org_id ,type,contact_name ,cost_center from slt_organization where id=:uid",array(":uid"=>$formid))->fetchAll();
    expDebug::dPrintDBAPI(' default_values = ' , $default);
    $fields = array(
      'name'              => $form_state['values']['name'],
      'description'       => $description,
    //'number'            => $form_state['values']['number'],
      'type'              => $form_state['values']['type'],
    //'status'            => $form_state['values']['status'],
    /*'add1'              => $form_state['values']['add1'],
     'add2'              => $form_state['values']['add2'],
     'city'              => $form_state['values']['city'],
     'state'             => $form_state['values']['state'],*/
    //'state'           => $stateCode,
    /*'country'           => $form_state['values']['country'],
     'zip'               => $form_state['values']['zip'],*/
      'contact_name'      => $form_state['values']['contact'],
    /*'phone_no'          => $form_state['values']['phone_no'],
     'email'             => $form_state['values']['email'],*/
      'cost_center'       => $form_state['values']['cost_center'],
    //'additional_info'   => $form_state['values']['additional_info'],
    /*'in_active_on'      => $in_active_on,
     'in_active_reason'  => $in_active_reason,*/
      'updated_by'        => $userid,
      'updated_on'        => now(),
    	'custom_dataload' => ($dtLoad == 'DAPI') ? $form_state['values']['custom_dataload'] : NULL
    );
    $userid = ($userid == 'Feed') ? 1 : $userid;
    if (!empty($form_state['storage']['acvalues']['parentorg']['id'])) {

      $fields['parent_org_id'] = $form_state['storage']['acvalues']['parentorg']['id'];

      // We may have to clean up $form_state input and values for parent_org_id and parent fields,
      // so that they are refreshed with the latest db values
      if (empty($form_state['values']['parent_org_id']) ||
      $form_state['values']['parent_org_id'] != $form_state['storage']['acvalues']['parentorg']['id'] ||
      empty($form_state['values']['parent']) ||
      $form_state['values']['parent'] != $form_state['storage']['acvalues']['parentorg']['name']) {
        unset($form_state['input']['parent']);
        unset($form_state['values']['parent']);
        unset($form_state['input']['parent_org_id']);
        unset($form_state['values']['parent_org_id']);
      }
    }
    else {

      $fields['parent_org_id'] = NULL;

      unset($form_state['input']['parent']);
      unset($form_state['values']['parent']);
      unset($form_state['input']['parent_org_id']);
      unset($form_state['values']['parent_org_id']);
    }

    // Select the fields and the values to be updated
    $fields = trimInputFields($fields); // #42124 - Trim input values
    $updateStmt->fields($fields);

    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);

    // Execute query
    $numUpdated = $updateStmt->execute();
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);

    $formFieldsList = array('name', 'description','type', 'contact_name', 'cost_center');
    if ($callFromAPI == 0) {
      $formFieldsList[] = 'parent_org_id';
    }

    make_audit_trail_entries($fields, $userid , $default ,$formid, 'Updated', 'cre_sys_obt_org', 'exp_sp_administration_organization',
                                         'updateOrganizationDetails', $formFieldsList);

    addTaxonomyTermForOrg($form_state['values']['id'], $form_state['values']['name'], '', $form_state['values']['status']);

    // Insert Tax Exempted value in slt_entity_profile_mapping table
    saveTaxExemptedOrgCustomProfile($form_state['values']['id'], $form_state['values']['tax_exempted'], $userid);
    if($dtLoad='DAPI'){
    	return array((object)array('id'=>$form_state['values']['id'],'status'=>'success'));
    }
    return array((object)array('id'=>$form_state['values']['id']));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateOrganizationDetails', $ex);
    if($dtLoad='DAPI'){
    	return array((object)array('id'=>'failure','status'=>'failure','remarks'=>$ex->getMessage()));
    }
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in update organization";
    	return $errobj;
    }else
    	return array((object)array('id'=>'failure'));
  }
}

/*
 * uniqueOrgName() - Checks that the orgname is unique. There should be no other organization in the system with that name.
 *          For an edit operation, do not compare with the self name in the database
 *          select COUNT(id) from slt_organization where id != currentId (if exists) and name = new name
 */
/* function uniqueOrgName($form_state) {
 // Select slt_organization table
 $select = db_select('slt_organization', 'org');

 // Get count of matching organizations
 $select->addExpression('COUNT(org.id)', 'count');

 // Set the conditions
 $select->condition('org.name', $form_state['values']['name'], '=');
 if (!empty($form_state['values']['id'])) { // An update operation, do not compare with self's title
 $select->condition('org.id', $form_state['values']['id'], '!=');
 }

 expDebug::dPrint('exp_sp_administration_organization.inc : uniqueOrgName() : $select obj = ' . print_r($select, true));
 expDebug::dPrint('exp_sp_administration_organization.inc : uniqueOrgName() : $select = ' . print_r($select->__toString(), true));

 // Execute the query
 $numOrg = $select->execute()->fetchField();
 expDebug::dPrint('exp_sp_administration_organization.inc : uniqueOrgName() : $numOrg = ' . print_r($numOrg, true));

 if ($numOrg > 0) {
 return false; // Not unique
 }

 return true; // a valid new name
 }
 /*
 * uniqueOrgCode() - Checks whether the combination of organization name and organization code are unique
 *          For an edit operation, do not compare
 *          select COUNT(id) from slt_organization where id != currentId (if exists) and name = new name
 */
function uniqueOrgCode($form_state) {
	try {
  // Select slt_organization table
  $select = db_select('slt_organization', 'org');

  // Get count of matching organizations
  $select->addExpression('COUNT(org.id)', 'count');

  // Set the conditions
  $select->condition('org.number', $form_state['values']['number'], '=');
  //$select->condition('org.name', $form_state['values']['name'], '=');

  if (!empty($form_state['values']['id'])) { // An update operation, do not compare with self's title
    $select->condition('org.id', $form_state['values']['id'], '!=');
  }

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execute the query
  $numOrg = $select->execute()->fetchField();
  expDebug::dPrint(' $numOrg = ' . print_r($numOrg, true) , 3);

  if ($numOrg > 0) {
    return false; // Not unique
  }

  return true; // a valid new name
  } catch (Exception $ex) {
  	watchdog_exception('uniqueOrgCode', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getOrgIdFromName() - Fetch organization id for the organization name, if the name exists and is unique
 */
function getOrgIdFromName($orgName) {
	try {
  if (empty($orgName)) {
    return null;
  }

  // Select slt_organization table
  $select = db_select('slt_organization', 'org');

  // Get org id
  $select->addField('org', 'id', 'id');

  // Set the conditions
  $select->condition('org.name', $orgName, '=');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execure the query
  $result = $select->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  if (count($result) <> 1) {
    return null; // Does not exist or two or more records exist with the same name
  }

  return $result[0]->id;
  } catch (Exception $ex) {
  	watchdog_exception('getOrgIdFromName', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * fetchOrganizationsAll() - Fetches all organizations listing depends upon the filters.
 */
function fetchOrganizationsAll($op=NULL, $start=NULL, $limit=NULL, $pattern = '', $fetchchildOrgandUser = false, $callFromAPI = 0) {
  // Select slt_organization table
  try {
  $select = db_select('slt_organization', 'org');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=org.id AND group_map.entity_type = \'cre_org\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_org\''); */
  // Left join the users, slt_person and slt_profile_list_items table to get the user name
  $select->leftJoin('slt_profile_list_items', 'plitype', 'org.type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'org.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_state', 'state', 'org.state = state.state_code AND org.country = state.country_code');
  $select->leftJoin('slt_country', 'country', 'org.country = country.country_code');
  if($op=='COUNT'){
    // Get count of active organizations
    $select->addExpression('COUNT(DISTINCT(org.id))', 'count');
  }
  else{
    // Select fields/expression values to be fetched from the database.
  	//$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
  	//$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
    $select->addField('org', 'id', 'id');
    $select->addField('org', 'name', 'name');
    //$select->addField('org', 'number', 'number');
    $select->addField('plitype', 'name', 'type');
    $select->addField('plistatus', 'name', 'status');
    //$select->addField('state', 'state_name', 'statename');
    //$select->addField('org', 'state', 'statecode');
    //$select->addField('org', 'country', 'countrycode');
    //$select->addField('country', 'country_name', 'country_name');
    //$select->addField('country', 'country_code', 'country_code');
    $select->addExpression('\'Organization\'', 'object_type');
    $select->addField('org', 'description', 'description');
    $select->addField('org', 'status', 'status_code');
    $select->groupBy('org.id');
  }

  // Delete entries should not display
  $select->condition('org.status', array('cre_org_sts_del'), 'NOT IN');

  // Set the conditions
  //$defaultOrgStatus = isset($_GET['orgstatus']) ? '' : 'cre_org_sts_act'; // for showing only active records
  $defaultOrgStatus = '';
  selectAddNarrowSearchINCondition($select, 'org', 'status', 'orgstatus', $defaultOrgStatus);
  selectAddNarrowSearchINCondition($select, 'org', 'type', 'orgtype');
  //change by ayyappans for 40036: New RestAPI - getUserDetails
  if($pattern != '') {
  	$select->condition('org.name', $pattern, 'LIKE');
  }
  else {
  	selectAddNarrowSearchByTextCondition($select, 'org', 'name');
  }

  $select->condition('org.name','','<>');
  filterByUserAccessPrivileges($select, 'org','',1,'cre_org','','org');
  if($op != "COUNT"){
    // Order the records
    orgSearchSelectStmtAddOrderByClause($select, 'org', "AZ");
  }

  //if($callFromAPI && module_exists('exp_sp_administration_customattribute')){ //#custom_attribute_0078975 - Check module status/Adding Custom Attribute Fields to Display Columns
  if($callFromAPI)
  {
  	$ret_tmp = getcustomattributemodulestatus();
  	if($ret_tmp == true){ 
    include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
    $attrValues = getCustomAttributeValues('Organization','mandatory');
    foreach($attrValues as $ckey => $cvalue){
  
    	
        //$select->addField('org', strtolower(str_replace(' ','_',$cvalue->attributename)), strtolower(str_replace(' ','_',$cvalue->attributename)));
                //$select->addField('org', strtolower(str_replace(' ','_',$cvalue->attributename)), strtolower(str_replace(' ','_',$cvalue->attributename)));
            	$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0`
            			WHERE FIND_IN_SET(`opt0`.`opt_code`,`org`.`$cvalue->attributerefercolumn`)),`org`.`$cvalue->attributerefercolumn`),`org`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));
    	
    }
  }
  	$ret_tmp = getcustomattributemodulestatus();
  	if($ret_tmp == true){
  	expDebug::dPrint(' $_POST' . print_r($_POST, true) , 5);
  
  	foreach($_POST as $key=>$value) {
  		expDebug::dPrint(' $value ' . print_r($value, true) , 5);
  
  		if(strpos($key,'c_')===0 && $value !=''){
  			expDebug::dPrint(' $key ' . print_r($key, true) , 5);
  
  			//$entityMappingColumn = getCustomAttributeMappingColumn($key);
  			$entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_org');
  			$per_column = 'org.'.$entityMappingColumn['crefercolumn'];
  			expDebug::dPrint(' $per_column' . print_r($per_column, true) , 5);
  
  			/*$select->condition($per_column, '%' . db_like($value) . '%', 'LIKE');*/
  		
  		$custom_attr_option_code=$per_column." LIKE '%".$value."%'";
  		
  		expDebug::dPrint(' $customattr_opt_o_pos' . print_r($customattr_opt_o_pos, true) , 5);
  			include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
  			$cus_attr_opt_code_arr=explode(",",$value);
  			if(count($cus_attr_opt_code_arr)>0){
  				foreach($cus_attr_opt_code_arr as $o_key => $o_val){
  					expDebug::dPrint(' $$o_val' . print_r($o_val, true) , 5);
  					if($o_val!='')
  					{
  						$custom_attr_option_code.=" OR ".$per_column." LIKE '%".getCustomAttributeCode($o_val,'o')."%'";
  		            }
  				}
  			}
  		
  		$select->where($custom_attr_option_code);
  	}
  } //#custom_attribute_0078975 - End Check module status
  }
  }
  
//   expDebug::dPrintDBAPI(' $select obj = ' , $select);

  if($op == "COUNT"){
    $result = $select->execute()->fetchField();
    expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
    return $result;
  }
  else if ($op == 'CSV' || $op == 'PDF') {
  	expDebug::dPrintDBAPI('original csv query $select', $select);
  	// Remove field and expressions from query which are not needed for export of data
  	/*
  	 * 0046628: Unable to export organization, system throws website error
  	 */
  	/*$csvFields = & $select->getFields();
  	expDebug::dPrint('$csvFields = ' . print_r($csvFields, true), 4);
  	unset($csvFields['description']);
  	unset($csvFields['status_code']);*/
  	$csvExpressions = & $select->getExpressions();
  	expDebug::dPrint('$csvExpressions = ' . print_r($csvExpressions, true), 4);
  	unset($csvExpressions['object_type']);
  	$orderByPart = & $select->getOrderBy();
  	expDebug::dPrint('$orderByPart = ' . print_r($orderByPart, true), 4);
  	$sortOrder = reset($orderByPart);
  	$sortCol = key($orderByPart);
  	$orderByPart = array(); // remove sort from inner query.
  	// Add number and created_on fields to search results as the records may be ordered by these apart from name (see orgSearchSelectStmtAddOrderByClause())
  	$select->addField('org', 'number', 'number');
  	$select->addField('org', 'created_on', 'created_on');

  	getChildOrgsUserCount($select);
  	$select->orderBy($sortCol, $sortOrder); // add sort to outer query
  	expDebug::dPrintDBAPI('select pdf orgs', $select, 4);
  	if ($op == "CSV") {
  		$colHeadersMap = array(
  				t('LBL107') => 'name',
  				t('LBL036') => 'type',
  				t('LBL102') => 'status',
  				t('LBL159') => 'childorgs',
  				t('LBL160') => 'lnrcount'
  		);
  		expDebug::dPrint('$colHeadersMap = ' . print_r($colHeadersMap, true), 4);

  		$filterArray = array(
  				array(t('Organization'). ':' . textboxFiltersetValueToPrintableStr('textfilter')),
  				array(t('LBL102') . ':' . checkboxFiltersetSelectionsToPrintableStr('orgstatus')),
  				array(t('LBL036') . ':' . checkboxFiltersetSelectionsToPrintableStr('orgtype')),
  				array(ucfirst(t('LBL011')) . ':' . getNarrowSearchSortTitle())
  		);
  		expDebug::dPrint('$filterArray = ' . print_r($filterArray, true), 4);
  		SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray);
  	}
  	else {
  		// % width of the columns must total to 100%
  		$colHeadersMap = array(
  				t('LBL107') => array('name',      44),
  				t('LBL036') => array('type',      12),
  				t('LBL102') => array('status',    14),
  				t('LBL159') => array('childorgs', 20),
  				t('LBL160') => array('lnrcount',  10)
  		);
  		expDebug::dPrint('$colHeadersMap = ' . print_r($colHeadersMap, true), 4);

  		$appliedFilters = array(
  				t('Organization') => textboxFiltersetValueToPrintableStr('textfilter'),
  				t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('orgstatus'),
  				t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('orgtype'),
  				ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
  		);
  		expDebug::dPrint('$appliedFilters = ' . print_r($appliedFilters, true), 4);

  		SendNarrowSearchResultsAsPDFFile($select, t('Organization'), $appliedFilters, $colHeadersMap);
  	}
  }
  else{
    // Limit to $limit records
    //if($start != null && $limit != null) {
    if($callFromAPI) {
    	expDebug::dPrintDBAPI('fetch organization count query', $select->countQuery());
  		$totalRecords = $select->countQuery()->execute()->fetchField(); // #46262 - list of records attached
    }
  	add_row_id_condition($select, 'org');
  	if ($limit) { // #45034 - limit value should be postive number
    	$select->range($start, $limit);
    }
    if($fetchchildOrgandUser) {	//change by ayyappans for 41566: API Review - Data Missing in List Organisation API
    	getChildOrgsUserCount($select);
        
        //if($callFromAPI && module_exists('exp_sp_administration_customattribute')){ //#custom_attribute_0078975 - Check module status/Adding Custom Attribute Fields to Display Columns
        if($callFromAPI)
        {
        	$ret_tmp = getcustomattributemodulestatus();
        	if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
            $attrValues = getCustomAttributeValues('Organization','mandatory');
            foreach($attrValues as $ckey => $cvalue){
            	$select->addField('org', strtolower(str_replace(' ','_',$cvalue->attributename)), strtolower(str_replace(' ','_',$cvalue->attributename)));
                //$select->addField('org', strtolower(str_replace(' ','_',$cvalue->attributename)), strtolower(str_replace(' ','_',$cvalue->attributename)));
            	/*$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0`
            			WHERE FIND_IN_SET(`opt0`.`opt_code`,`org`.`$cvalue->attributerefercolumn`)),`org`.`$cvalue->attributerefercolumn`),`org`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));*/
            	
            }
        }
        }
        
    }
    expDebug::dPrintDBAPI('fetch organization query', $select);
    // Execure the query and fetch all the records
    $result = $select->execute()->fetchAll();

    if ($totalRecords && $callFromAPI)
    $result['totalrow'] = $totalRecords;
//     expDebug::dPrintDBAPI('test query ', $select);
    expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3);
    return $result;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchOrganizationsAll', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewOrganizationByRest( &$form_state,$paramuserid="")
{
	try{
		$form=array();
		$form["form_id"]="";
		// Type Validation For this ticket #0046174
		$type = strtolower($form_state['values']['type']);
		if(!in_array($type, array('cre_org_typ_ext','cre_org_typ_int'))) { // Organisation Type Check
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Organization Type does not exist.";
			return $errobj;
			//return array((object)array('id'=>'Organization Type does not exist.'));
		}

		$form_state['storage']['acvalues']['parentorg']['id'] = $form_state['values']['parent'];
		$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
		//36820: Irrelevant escape character is getting inserted on processing through API
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		return addNewOrganization($form,$form_state,$paramuserid,1);
	} catch (Exception $ex) {
		watchdog_exception('addNewOrganizationByRest', $ex);
		expertusErrorThrow($ex);
	}
}

function updateOrganizationDetailsByRest( &$form_state,$paramuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		$orgId = trim($form_state['values']['id']);
		$orgExist = getOrganizationDetails($orgId, array('id'));
		if (empty($orgExist)) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Organization id. Please check it once";
			return $errobj;
			//return array((object)array('id'=>'Invalid Organization id. Please check it once'));
		}
		// Type Validation For this ticket #0046174
		$type = strtolower($form_state['values']['type']);
		if(!in_array($type, array('cre_org_typ_ext','cre_org_typ_int'))) { // Organisation Type Check
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Organization Type does not exist.";
			return $errobj;
			//return array((object)array('id'=>'Organization Type does not exist.'));
		}

		$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
		$form_state['storage']['acvalues']['parentorg']['id'] = $form_state['values']['parent'];
		$status = strtolower(trim($form_state['values']['status']));
		$status_array = array('inactivate', 'activate');
		if (!in_array($status, $status_array)) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid status. Please use Activate or InActivate";
			return $errobj;
			//return array((object)array('id'=>'Invalid status. Please use Activate or InActivate'));
		}
        $ret_tmp = getcustomattributemodulestatus();
        if($ret_tmp == true){
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
            $custom_attr_error = validateCustomAttribute($form_state,'cre_org');
            expDebug::dPrint('Update Org API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
            if($custom_attr_error != '') {
                $customErrmsg=new stdClass();
                $customErrmsg->isValidateError = 1;
                $customErrmsg->errormsg = $custom_attr_error;
                return $customErrmsg;
            }
        } //#custom_attribute_0078975 - End Check module status
        
		//36820: Irrelevant escape character is getting inserted on processing through API
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		$orgId = updateOrganizationDetails($form,$form_state,$paramuserid,1);
		if($orgId->isValidateError != 1 && ($orgId[0]->id) > 0){
			activateOrDeactivateOrg($orgId[0]->id,$form_state['values']['status'],false);
            
            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
            //Save Custom Attributes #custom_attribute_0078975
            $ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
            saveCustomAttributesInAddEditForm($form, $form_state, $orgId[0]->id, 'cre_org', $paramuserid,'Updated',1);
            } //#custom_attribute_0078975 - End Check module status
		}
		return $orgId;
	} catch (Exception $ex) {
		watchdog_exception('updateOrganizationDetailsByRest', $ex);
		expertusErrorThrow($ex);
	}
}

function listOrganizationByRestAPI($start, $limit, $userid="")
{
	try {
  $form=array();
  $form["form_id"]="";
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $limit = (empty($limit) || $limit > 1000) ? 1000: $limit;
  //expDebug::dPrint(' start '.$start .'--limit--' . $limit);
  return fetchOrganizationsAll('',$start,$limit, '', 1, 1);
  } catch (Exception $ex) {
  	watchdog_exception('listOrganizationByRestAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * orgAddEditFormWithCancelAndSaveClose() -
 */
function orgAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $orgDetails = array(),$emptyId) {
	try {
  global $theme_key;
	expDebug::dPrint(' userAddEditFormWithCancelAndSaveClose() called.');

  $customHiddenButtonId = 'org-button-account';

  $ajaxCallbackCancel = 'addEditCancelRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

  // For common styling of all addedit cancel buttons
  // $cancelAttributes['class'][] = 'addedit-edit-cancel';
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $cancelAttributes['tabindex'][] ='8';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
  $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
	$setcls = ($theme_key=='expertusoneV2')?'':(empty($orgDetails['id']))?'resize-save_btn':'resize-save_btn4';
  $saveCls = !empty($orgDetails) ? 'org-save-edit':'org-save-add';
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
     '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions '.$setcls.' '.$saveCls.'"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
  //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
    '#value' => t('LBL123'), // Close
    '#attributes' => $cancelAttributes,
  );

  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';
  $saveAttributes['tabindex'][] = '7';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);

  $esignHiddenButt = 'esign-'.$wrapperId;
  $esignHiddenButt1 = 'status-upd-esign-'.$wrapperId;

  if(!empty($orgDetails)){//IF EDIT MODE
    $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
    $activesuspend = getOrganizationDeleteAccess($orgDetails['id']);
    expDebug::dPrint('$activesuspend : '.$activesuspend, 4);
    if($activesuspend  > 0){
    	$suffix ='<div class="admin-save-button-right-bg"></div>';
    }else{
    	$suffix ='<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>';
    }
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){ //IF ESIGNATURE ENABLED PERFORM SAVE
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
      $setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
      $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#value' => t('LBL141'),
            '#name' => 'save',
            '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="'.$setclass.'"></div>',
            '#suffix' => "$suffix</div>",
      	  '#id' =>$saveButtonId,
      	  '#attributes' => $saveAttributes,
            '#ajax' => array(
      	      'wrapper' => $wrapperId,
      	      'callback' => $ajaxCallbackSave,
      	      'method' => 'replace',
      	      'effect' => 'none', // 'fade',
      	      'event'=>'click',
                'customButtonId'=>$esignHiddenButt,
      	      'keypress' => true,
      	      'progress' => array(
      	        'type' => 'throbber',
      	        'message' => ''
      	        ),
      	        )
      	        );
    }else{//ESIGN NOT ENABLED PERFORM SAVE
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
      $setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
      $form[$saveButtonId] = array(
  	    '#type' => 'button',
          '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="'.$setclass.'"></div>',
      	  '#suffix' => "$suffix</div>",
          '#value' => t('LBL141'),
          '#name' => 'save',
  	      '#attributes' => $saveAttributes,
      );
    }
    //TO CALL THE HIDDED BUTTON RELATED TO ESIGN PROCESS
    unset($saveAttributes['tabindex']);
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
	    '#type' => 'submit',
	    '#value' => t('hidden-save'),
	    '#name'	=> 'hidden-save',
      	'#prefix' => '<span id = "esign-admin-div" style="display:none">',
	    '#suffix' => '</span>',
	    '#id' =>$esignHiddenButt,
  		'#attributes' => $saveAttributes,
	    '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
		      'method' => 'replace',
		      'effect' => 'none', // 'fade',
		      'event'=>'click',
  			  'orgActionStatus'=>'',
		      'keypress' => true,
		      'progress' => array(
		      'type' => 'throbber',
		      'message' => ''
		      ),
		      )
		      );


		      $form['addedit-button-group-ul-start'] = array(
          '#type' => 'markup',
          '#markup' => '<ul class="catalog-pub-add-list">',
		      );

		      if($orgDetails['status'] == 'cre_org_sts_act'){//GET ORG STATUS
		        $activateSuspendLabel  = t('LBL572');
		        $orgActionStatus  = 'Suspend';
		      }
		      else{
		        $activateSuspendLabel  = t('LBL573');
		        $orgActionStatus  = 'Activate';
		      }
		      $activateSuspendButtonId = 'org-activate-suspend-button';
		      if($activesuspend == 0){
		      if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){//IF ESIGNATURE ENABLED PERFORM STATUS UPDATE
		        $activateSuspendAttributes['onclick'] = array();
		        //$activateSuspendAttributes['onclick'][] = 'increasePopWidth();';
		        $form[$activateSuspendButtonId] = array(
		        		'#type' => 'submit',
		        		'#value' => $activateSuspendLabel,
		        		'#prefix' => '<li class="save-pub-unpub-sub-menu">',
		        		'#suffix' => '</li>',
		        		'#name' => 'org-activate-suspend',
		        		'#id' =>$activateSuspendButtonId,
		        		'#attributes' => $activateSuspendAttributes,
		        		'#ajax' => array(
		        				'wrapper' => $wrapperId,
		        				'callback' => $ajaxCallbackSave,
		        				'method' => 'replace',
		        				'effect' => 'none', // 'fade',
		        				'event'=>'click',
		        				'customButtonId'=>$esignHiddenButt1,
		        				'keypress' => true,
		        				'progress' => array(
		        						'type' => 'throbber',
		        						'message' => ''
		        				),
		        		)
		        );
		      }else{//IF ESIGNATURE NOT ENABLED PERFORM STATUS UPDATE
		        $activateSuspendAttributes['onclick'] = array();
		        $activateSuspendAttributes['onclick'][] = '$("#'.$esignHiddenButt1.'").click(); return false;';
		        $form[$activateSuspendButtonId] = array(
  	    	'#type' => 'submit',
  	    	'#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    	'#suffix' => '</li>',
            '#value' => $activateSuspendLabel,
          	'#name' => 'org-activate-suspend',
    	    '#attributes' => $activateSuspendAttributes,
		        );
		      }

		      $activateSuspendAttributes['onclick'] = array();
		      $form[$esignHiddenButt1] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'org-status-upd-hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
    	      '#suffix' => '</span>',
    	      '#id' =>$esignHiddenButt1,
      		  '#attributes' => $activateSuspendAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'orgActionStatus'=>$orgActionStatus,
    		      'keypress' => true,
    		      'progress' => array(
    		      'type' => 'throbber',
    		      'message' => ''
    		      ),
    		      )
    		      );
		      }
		      $deleteCheck = adminVisibilityForAddEditDelete($orgDetails['id'],'cre_org','cre_org','slt_organization');
    		      $deleteAccess = delete_object_record($orgDetails['id'],'Organization');
    		      $deleteAccesschildorg = getChildOrgsCount($orgDetails['id']);
    		      if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
			    		      if(!$deleteAccess && !$deleteAccesschildorg){
			    		        $displayTitle = titleController('ADMIN-ORG-NAME', $orgDetails['name'],34);
			    		        $deleteButtonId = 'org-delete-dropdown-button';
			    		        $deleteAttributes['onclick'] = array();
			    		        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('Organization')).' '.escape_string($orgDetails['name']) . '","'.$orgDetails['id'].'","Organization"); return false;';
			    		        $deleteAttributes['class'][] = 'admin-delete-button-bg';
			    		        $form[$deleteButtonId] = array(
			    	    					'#type' => 'button',
			    	    					'#prefix' => '<li class="save-pub-unpub-sub-menu">',
			    	    					'#suffix' => '</li>',
			    	    					'#value' => t('LBL286'),
			            				'#name' => 'delete',
			    	    					'#attributes' => $deleteAttributes,
			    		        );
			    		      }
    		      }
    		      $form['addedit-button-group-ul-end'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="clearBoth"></div></div>' .
  	             '<div class="clearBoth"></div></div></ul>',
    		      );
  }else{
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
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
                    'customButtonId'=>$esignHiddenButt,
      		      'keypress' => true,
      		      'progress' => array(
      		        'type' => 'throbber',
      		        'message' => ''
      		        ),
      		        )
      		        );
    }else{
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
      $form[$saveButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      '#suffix' =>
                 '<div class="admin-save-button-right-bg"></div></div>' .
                 '<div class="clearBoth"></div></div>' .
               '<div class="clearBoth"></div></div>',
        '#value' => t('LBL141'),
      	'#name' => 'save',
      '#attributes' => $saveAttributes,
      );
    }
    unset($saveAttributes['tabindex']);
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
	      '#suffix' => '</span>',
	      '#id' =>$esignHiddenButt,
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
		      'method' => 'replace',
		      'effect' => 'none', // 'fade',
		      'event'=>'click',
    //'userActionStatus'=>'',
		      'keypress' => true,
		      'progress' => array(
		      'type' => 'throbber',
		      'message' => ''
		      ),
		      )
		      );

  }

  $entityId          	 = $orgDetails['id'];
  if(!empty($emptyId)){
  	$entityId = 0;
  } else {
  	$emptyId = 0;
  }

  $entityType        	 = 'cre_org';
  $qtipIdInit        	 = $entityId.'_'.$entityType;

  $qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'$entityType','url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."','wid':650,'heg':'270','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-org-".$entityId."'}";

  $form['addedit-left-col-extend-func'] = array(
  		'#type' => 'markup',
  		'#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-org-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
  		'</div>',
  );


  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('orgAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function to activate and deactivate the organization
 */
function activateOrDeactivateOrg($orgId,$status='',$printResult=true,$dtld=0){
  if($dtld == 0){
	$currUserId = getSltpersonUserId();
  }else{
  	$currUserId = 'Feed';
  }
  $txn = db_transaction();
  try {
    if($status==''){
      $status  = getOrganizationStatus($orgId);
      $updatedstatus  = ($status == 'cre_org_sts_act') ? "cre_org_sts_itv" : "cre_org_sts_act";
    }else{
      $updatedstatus  = ($status == 'Activate') ? "cre_org_sts_act" : "cre_org_sts_itv";
      if($status == 'Activate'){
        $status = 'cre_org_sts_itv';
      }else{
        $status = 'cre_org_sts_act';
      }
    }
    $updateStmt = db_update('slt_organization');
    $updateStmt->condition('id', $orgId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
    ));
    $updateStmt->execute();
    $currUserId = ($currUserId == 'Feed') ? 1 : $currUserId;
    add_audit_trail_entry($currUserId, $orgId, 'cre_sys_obt_org', 'exp_sp_administration_organization', 'activateOrDeactivateOrg', 'Updated organization Status',
    null, $status, $updatedstatus);
    if($printResult){
      print   ($updatedstatus == 'cre_org_sts_act') ? t('LBL572') : t('LBL573');
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('activateOrDeactivateOrg', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of user
 */
function getOrganizationStatus($orgId){
	try {
  $select = db_select('slt_organization', 'org');
  $select->addField('org','status', 'status');
  $select->condition('org.id',$orgId);
  // Execute the query
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationStatus', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addEditCanDeleteOrg() - decision to show or not show delete action button in the more menu is made here
 */
function addEditCanDeleteOrg($orgId) {
	try {
  if (!empty($orgId) && delete_object_record($orgId, 'Organization') <= 0) {
    return true;
  }

  return false;
  } catch (Exception $ex) {
  	watchdog_exception('addEditCanDeleteOrg', $ex);
  	expertusErrorThrow($ex);
  }
}

function getOrgUsersListForGrid($orgid,$keyword=''){
	try{
		$page      = $_GET['page'];
		$limit     = $_GET['rows'];
		$count      = getOrganizationUsersResult($orgid,$keyword,false,true);
		$start = $limit * $page - $limit;
		$listquery  = getOrganizationUsersResult($orgid,$keyword,true,false,$start,$limit);

		if($_GET['sidx'] == 'Name' || empty($_GET['sidx'])){
			$sortby = 'person.full_name';
		}elseif($_GET['sidx'] == 'UserName'){
			$sortby = 'person.user_name';
		}elseif($_GET['sidx'] == 'JobTitle'){
			$sortby = 'pli1.name';
		}elseif($_GET['sidx'] == 'UserType'){
			$sortby = 'pli1.name';
		}elseif($_GET['sidx'] == 'ManagerName'){
			$sortby = 'person1.full_name';
		}else{
			$sortby = 'person.full_name';
		}

		$sort = ( empty($_GET['sidx']) ) ? 'asc' : $_GET['sord'] ;
		$listquery->orderBy($sortby, $sort);
		$queryResult = $listquery->execute();

		expDebug::dPrintDBAPI(' getOrgUsersListForGrid $queryResult = ', $queryResult);
		expDebug::dPrintDBAPI(' getOrgUsersListForGrid $listquery = ', $listquery);

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

		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = $orgid.'-orgusers';
		$srch_result_response_roster->orgid    = $orgid;
		$srch_result_response_roster->type    = 'orgusers';
		$_SESSION['jq_count'] = array();
		$i=0;
		foreach ($queryResult as $rec_search_results){
			$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->id;
			$srch_result_response_roster->rows[$i]['cell'] = array(
					'id' 	      => $rec_search_results->id,
					'Name'    => '<span class="vtip" title="'.sanitize_data($rec_search_results->full_name).'">'.titleController('ADMIN-ORG-FNAME', $rec_search_results->full_name,8).'</span>',
					'UserName' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->user_name).'">'.titleController('ADMIN-ORG-UNAME', $rec_search_results->user_name,8).'</span>',
					'JobTitle' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->job_title).'">'.titleController('ADMIN-ORG-JOBTITLE',$rec_search_results->job_title,8).'</span>',
					'UserType' 	  => '<span class="vtip" title="'.sanitize_data($rec_search_results->user_type).'">'.titleController('ADMIN-ORG-USERTYPE', $rec_search_results->user_type,8).'</span>',
					'ManagerName' => '<span class="vtip" title="'.sanitize_data($rec_search_results->manager_name).'">'.titleController('ADMIN-ORG-MANAGER-NAME', $rec_search_results->manager_name,8).'</span>',
			);
			$i++;
		}
		if($count >= 1){
			$srch_result_response_roster->msg  = t('MSG403');
		}
		echo drupal_json_encode($srch_result_response_roster);
	}catch (Exception $ex) {
		watchdog_exception('getOrgUsersListForGrid', $ex);
		expertusErrorThrow($ex);
	}
}//End getOrgUsersListForGrid()

function getOrganizationUsersResult($orgid,$keyword='',$queryRequired=false,$countRequired=false,$start=null,$limit=null){
	try{
		$select = db_select('slt_person', 'person');
		if($countRequired){
			$select->addExpression('count(person.id)', 'count');
		}else{
			$select->addField('person','full_name','full_name');
			$select->addField('person','user_name','user_name');
			$select->leftJoin('slt_person', 'person1', 'person1.id = person.manager_id');
			$select->addField('person1','full_name','manager_name');
			$select->leftJoin('slt_profile_list_items', 'pli1', 'pli1.code = person.job_title');
			$select->addField('pli1','name','job_title');
			$select->leftJoin('slt_profile_list_items', 'pli2', 'pli2.code = person.user_type');
			$select->addField('pli2','name','user_type');
		}
		$select->condition('person.status', 'cre_usr_sts_atv');
		if(is_array($orgid)) {
			$select->condition('person.org_id', $orgid, 'IN');
			$select->addField('person', 'org_id', 'org_id');
			$select->groupBy('person.org_id');
		} else {
			$select->condition('person.org_id', $orgid, '=');
		}
		
		if($keyword!='' && $keyword != t('LBL1230')){
			$select->condition(db_or()
					->condition('person.full_name', '%' . db_like($keyword) . '%', 'LIKE')
					->condition('person.user_name', '%' . db_like($keyword) . '%', 'LIKE')
					);
		}
		if($countRequired){
			expDebug::dPrintDBAPI(' getOrganizationUsersResult $select $countRequired = ' , $select);
			if(is_array($orgid)) {
				$orgUsersCount = $select->execute()->fetchAllAssoc('org_id');
			} else {
				$orgUsersCount = $select->execute()->fetchField();
			}
			
			return $orgUsersCount;
		}else if($queryRequired){
		  	$select->range($start, $limit);
		    return $select;
  		}
		expDebug::dPrintDBAPI(' getOrganizationUsersResult $select = ' , $select);
		return $select;
	}catch (Exception $ex) {
		watchdog_exception('getOrganizationUsersResult', $ex);
		expertusErrorThrow($ex);
	}
}//End getOrganizationUsersResult()

function getOrgUsersExportCSV($orgid,$keyword=''){
	try{
		$select = db_select('slt_person', 'person');
		$select->addField('person','full_name','full_name');
		$select->addField('person','user_name','user_name');
		$select->leftJoin('slt_person', 'person1', 'person1.id = person.manager_id');
		$select->addField('person1','full_name','manager_name');
		$select->leftJoin('slt_profile_list_items', 'pli1', 'pli1.code = person.job_title');
		$select->addField('pli1','name','job_title');
		$select->leftJoin('slt_profile_list_items', 'pli2', 'pli2.code = person.user_type');
		$select->addField('pli2','name','user_type');
		$select->condition('person.status', 'cre_usr_sts_atv');
		$select->condition('person.org_id', $orgid);
		if($keyword!='' && $keyword != t('LBL1230')){
			$select->condition(db_or()
					->condition('person.full_name', '%' . db_like($keyword) . '%', 'LIKE')
					->condition('person.user_name', '%' . db_like($keyword) . '%', 'LIKE')
			);
		}
		expDebug::dPrintDBAPI(' getOrgUsersExportCSV $select ' , $select);
		$colHeadersMap = array(
				t('LBL107') => 'full_name',
				t('LBL054') => 'user_name',
				t('LBL073') => 'job_title',
				t('LBL173') => 'user_type',
				t('LBL134') => 'manager_name'
		);
		SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export');
	}catch (Exception $ex) {
		watchdog_exception('getOrgUsersExportCSV', $ex);
		expertusErrorThrow($ex);
	}
}//End getOrgUsersExportCSV()

function exp_sp_administration_vieworg_addedit_form($js = NULL, $entityId){
	try{
		//0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
		expDebug::dPrint('sabbeeee-->'.$entityId);
	  global $theme_key;
		  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
      if($theme_key == 'expertusoneV2') {
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning_v2.css');
      }else{
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.css');
      }
      //CUSTOM SCROLLBAR "JSscrollbar"
      drupal_add_js(drupal_get_path('module', 'exp_sp_lnrreports') .'/jscrollbar/jquery.jscrollpane.js', $js_module_optional);
		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 660,
								'minHeight' => 350,
								'Height' => auto,
						),
						'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
				)
		);

		drupal_add_js($catalog_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();
			$argsList[] = $entityId;

			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_view_orgdetails_addedit_form', 'title' => (t('LBL816'))) //view
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewclass_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_vieworg_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_orgdetails_addedit_form($form, &$form_state,$renderMode, $orgId){
	try {
		include_once(drupal_get_path('module', 'exp_sp_coursedetail') .'/exp_sp_coursedetail.inc');
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$entityId---->'.$orgId);

		$wrapperId = 'vieworganization-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		 $form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);

	  $orgdetails = getOrgDetailsForView($orgId);
		expDebug::dPrint('$orgdetails-->'.print_r($orgdetails,true),5);

		$form['narrow_orgview'] = array(
				'#markup'       => theme('narrow_orgview', array('results' => $orgdetails, 'type' => 'organization')),
		);
		 $form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_orgdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getOrgDetailsForView($orgId){
	try{
		$select = db_select('slt_organization', 'org');
		$select->leftJoin('slt_profile_list_items', 'slpi', 'org.type = slpi.code');
		$select->leftJoin('slt_organization', 'orga', 'orga.id = org.parent_org_id');
		$select->addField('org','id','id');
		$select->addField('org','name','name');
		$select->addField('slpi','name','type');
		$select->addField('org','description','description');
		$select->addField('org','cost_center','cost_center');
		$select->addField('orga','name','parent');
		$select->addField('org','contact_name','contact');
		$select->condition('org.id',$orgId,'=');
		expDebug::dPrintDBAPI('$select sabeena=',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('v$result$result$result->'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getOrgDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}

function getChildOrgsUserCount(&$query) {
	// Modify the query for getting child org count
	//$query->leftJoin('slt_organization', 'org1', 'org.id = org1.parent_org_id AND org1.status = \'cre_org_sts_act\'' );
	$query->groupBy('org.id');
	//$query->addExpression('COUNT(CASE WHEN org1.id IS NOT NULL THEN org1.id ELSE NULL END)', 'childorgs');
	//Changed join query to sub query for #0050574
	/* $query->addExpression('(select COUNT(CASE WHEN org1.id IS NOT NULL THEN org1.id ELSE NULL END) AS childorgs from slt_organization org1
												where org1.parent_org_id is not null
												and org.id = org1.parent_org_id
												AND org1.status = \'cre_org_sts_act\')','childorgs'); */
	// Expand the above query to also get org learner count
	$query = db_select($query, 'org');

// 	$query->leftJoin('slt_person', 'per', 'org.id = per.org_id AND per.status = \'cre_usr_sts_atv\'');
// 	$query->leftJoin('slt_organization', 'org1', 'org1.parent_org_id = org.id AND per.status != \'cre_org_sts_del\'');
	$query->groupBy('org.id');
	$query->addField('org', 'id', 'id'); // #46215: PS -API: Issue faced in List of Organization API's
	$query->addField('org', 'description', 'description');
	$query->addField('org', 'name', 'name');
	$query->addField('org', 'type', 'type');
	$query->addField('org', 'status', 'status');
// 	$query->addField('org', 'childorgs', 'childorgs');

	/**
	 * The following tables have been added after applying limit to make the select query run faster.
	 * If any changes are done against this, it has to be validated against slt_organization table with 1 lakh record
	 * 50574: Getting the 'MySQL server has gone away' error while calling the rest api 'ListOrganizationAPI'
	 */
	$display_columns = explode(',', getURLParam('display_cols'));
	if(getURLParam('apiname') == '' || in_array('lnrcount', $display_columns) ) {
		$selectLearners = db_select('slt_person', 'orgLnr');
		$selectLearners->addField('orgLnr', 'org_id');
		$selectLearners->addExpression('COUNT(id)', 'lnrcount');
		$selectLearners->condition('orgLnr.org_id', '', 'IS NOT NULL');
		$selectLearners->where('orgLnr.status = \'cre_usr_sts_atv\'');
		$selectLearners->groupBy('orgLnr.org_id');
		$query->addJoin('LEFT', $selectLearners, 'per', 'per.org_id = org.id');
		$query->addExpression('IFNULL(per.lnrcount, 0)', 'lnrcount');
		expDebug::dPrintDBAPI('joining slt_person for getting learner count', $query);
	}
	if(getURLParam('apiname') == '' || in_array('childorgs', $display_columns) ) {
		$selectchildOrgs = db_select('slt_organization', 'childOrg');
		$selectchildOrgs->addField('childOrg', 'parent_org_id');
		$selectchildOrgs->addExpression('COUNT(id)', 'childorgs');
		$selectchildOrgs->where('childOrg.parent_org_id != childOrg.id');
		$selectchildOrgs->condition('childOrg.parent_org_id', '', 'IS NOT NULL');
		$selectchildOrgs->where('childOrg.status = \'cre_org_sts_act\'');
		$selectchildOrgs->groupBy('childOrg.parent_org_id');
		$query->addJoin('LEFT', $selectchildOrgs, 'org1', 'org.id = org1.parent_org_id');
		$query->addExpression('IFNULL(org1.childorgs, 0)', 'childorgs');
		expDebug::dPrintDBAPI('joining slt_organization for getting child org count', $query);
	}
// 	$query->addExpression('COUNT(CASE WHEN per.id IS NOT NULL THEN per.id ELSE NULL END)', 'lnrcount');
// 	$query->addExpression('COUNT(CASE WHEN org1.id IS NOT NULL THEN org1.id ELSE NULL END)', 'childorgs');
}
?>