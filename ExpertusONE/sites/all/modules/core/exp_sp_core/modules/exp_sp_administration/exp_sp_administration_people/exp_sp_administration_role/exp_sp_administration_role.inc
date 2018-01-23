<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/**
 * Role Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 1-April-2011
 */
function getRoleTabContentType(){
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is whether contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getRoleTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * roleNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function roleNarrowSearchFiltersHTML() {
	try {
  $checkedStatus = array();
  $checkedStatus["cre_sec_sts_atv"] = TRUE;
  $allStatus = getProfileItemNamesKeyed('cre_sec_sts_',$langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false,'cre_sec_sts_del');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);
  if(!empty($allStatus)){
  $allStatus['cre_sec_sts_atv'] = t('LBL573');
  $allStatus['cre_sec_sts_itv'] = t('LBL572');
  }
  $filterSetforJS=array(array('type'=>'checkbox','code'=>'rolestatus'),
                  array("type" => "addltext","code" => "username",
                  "acpath" => "administration/people/role/username-autocomplete", "defaultText" => t("LBL181")),
  );

  return theme('narrow_search_filters_holder',
                        array('rendered_filtersets_list' => array(
                                                    theme('narrow_search_filterset_checkboxes',
                                                    array('code' => 'rolestatus',
                                                    'title' => t('LBL102'),
                                                    'divid' => 'status',
                                                    'checkboxes_list' => $allStatus)
                                                    ),
                                                    theme('narrow_search_filterset_addltext',
                                                    array('code' => 'username',
                                                    'title' => t('LBL054'),
                                                    'default_text' => t('LBL181'))
                                                    ),
                            ),
                      'filter_set_list' => drupal_json_encode($filterSetforJS), 
  ));
  } catch (Exception $ex) {
  	watchdog_exception('roleNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * roleSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function roleSearchResultsSortbarHTML() {
 try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('AZ'), 'sort_type' => 'AZ', 'html_id' => 'role-sort-az');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('ZA'), 'sort_type' => 'ZA', 'html_id' => 'role-sort-za');
  //$sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('Number'), 'sort_type' => 'Number', 'html_id' => 'role-sort-num');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('NewlyListed'), 'sort_type' => 'NewlyListed', 'html_id' => 'role-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'role-sort-new'
  					           );

  					           return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
  					           )
  					           );
  					           } catch (Exception $ex) {
  					           	watchdog_exception('roleSearchResultsSortbarHTML', $ex);
  					           	expertusErrorThrow($ex);
  					           }			            
}

/*
 * roleSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function roleSearchResultsActionbarHTML() {
try {
  $actionbarLinksListforJS=array('export_path'=>'administration/people/role/export/',
  					             'print_path'=>'administration/people/role/print/'
  					             );
  					             return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL1036'),
												 'add_page_path' => base_path() . 'administration/people/role/nojs/addedit', 
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
                         	'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  							'ctools_style'=>'ctools-modal-ctools-admin-role-addedit-style',     
  					             )
  					             );
  					             } catch (Exception $ex) {
  					             	watchdog_exception('roleSearchResultsActionbarHTML', $ex);
  					             	expertusErrorThrow($ex);
  					             }					              
  					             
}

/*
 * getRoleTabContentMainHTML - Returns the html for tab content main
 */
function getRoleTabContentMainHTML() {
	try {
  $tabContentTypeArr =  getRoleTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/people/role/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/people/role/textfilter-autocomplete'                                            
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
  	watchdog_exception('getRoleTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getRoleFTabContent - Returns the html for narrow search block on role search page
 */
function getRoleTabContent() {
	try {
  $tabContentTypeArr =  getRoleTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
   
  $roleDetailsResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => roleNarrowSearchFiltersHTML(),           
    'rendered_narrow_search_sortbar' => roleSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => roleSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getRoleTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','people'), 
  );
  echo drupal_json_encode($roleDetailsResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getRoleTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * geRoleLearnersCount() - Function to get the count of active users.

 */
function getRoleUsersCount($roleId) {
	try {
  // Select slt_security_role table
  $select = db_select('slt_security_role', 'role');

  // Get count of active users 
  $select->addField('role','userlist', 'userlist');
 
  // Set the conditions
  $select->condition('role.id', $roleId, '=');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execure the query
  $numLearners = $select->execute()->fetchField();
  $usercnt = 0;
  if($numLearners){
  	$newvalue = explode(',', $numLearners);
  	$userCount = count($newvalue);
  	for($i=0;$i<$userCount;$i++){
	  	$select = db_select('slt_person', 'pre');
	  	$select->addField('pre','status', 'status');
	  	$select->condition('pre.id', $newvalue[$i], '='); 
	  	$UsrSts = $select->execute()->fetchField();
	  	$usercnt = ($UsrSts == 'cre_usr_sts_atv') ? $usercnt+1 : $usercnt; 	
  	}
  }
  expDebug::dPrint(' active users ' .$usercnt);
  return $usercnt;
  } catch (Exception $ex) {
  	watchdog_exception('getRoleUsersCount', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getRoleDetailsHTML() - Renders an role's details as HTML to be displayed in jqGrid.
 */
function getRoleDetailsHTML($role) {
	try {
	
	expDebug::dPrint('getRoleDetailsHTML'.print_r($role,true),4);
  $detailsList = array();
  $actionList = array();
  $newActionList = array();
  //$entityType        	 = 'cre_sec';
  /*if (!empty($role->number)) {
    $additionalTitleInfo = $role->number;
  }
  if (!empty($role->type)) {
    $detailsList[] = array('detail' => t($role->type), 'tooltip' => t('LBL244').': '.$role->type);
  }
  /*  if (!empty($role->status)) {
   $detailsList[] = array('detail' => $role->status, 'tooltip' => t('LBL102'));
   }
   */
  
  $userCount=getRoleUsersCount($role->id);
  $detailsList[] =  array('detail' => t('LBL160').':'.' '.$userCount, 'tooltip' => t('LBL160'));
  
  $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/people/role/nojs/addedit', 
					  'action_button_params' => $role->id,
  					  'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                      'tooltip' => t('LBL063')
  );
  	
  $actionList[] = array('action_link_text' => t('LBL286'),
      'action_page_path' => '', 
      'action_button_params' => $role->id,
      'action_button_param_text' => 'Role',
      'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
      'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
      'tooltip' => t('LBL286')
  );

 //44133: German-old-ui-Icon not changing when user is activate and suspend
 if(strtolower($role->status_code) == 'cre_sec_sts_atv'){
    $newActionList[] = array('action_link_text' => t('LBL572'),
				  		'action_page_path' => '', 
				  		'action_button_params' => $role->id,
    					'action_button_params_text' => 'deactivate',
					    'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL572'),
                        'icon_style_class'=>'active-deactive-tab-icon  Suspend'
                        );
  } else {
  	expDebug::dPrint('role id-----'.$role->id);
    $newActionList[] = array('action_link_text' => t('LBL573'),
				  		'action_page_path' => '', 
				  		'action_button_params' => $role->id,
    					'action_button_params_text' => 'activate',
					    'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL573'),
                        'icon_style_class'=>'active-deactive-tab-icon  Activate'
                        );
  }
	
  //print_r($detailsList);
  return theme('narrow-search-results-item-details',
  array( 'title' => trim($role->name),
  //'additional_title_info' => trim($additionalTitleInfo),
                       	'delete_info' => t('MSG646').' '.$userCount.' '.t('MSG647').' '.t('MSG357').' '.strtolower(t('LBL1037')),
                       	'details_list'=>$detailsList,
                 		    'action_list'=> $actionList,
                        'new_action_list'=>$newActionList,
                        'object_type'=>'Role',
                        'description'=>$role->description,
  											'entity_multi_action' => $role,
  )
  );
  
  expDebug::dPrint('role.inc---->123'.print_r($role,true));
  } catch (Exception $ex) {
  	watchdog_exception('getRoleDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * roleSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function roleSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
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
  	watchdog_exception('roleSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

function sendRolesListToJqGrid() {
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

  $numRole = fetchRolesAll('COUNT');
  expDebug::dPrint(' $numRole = ' . print_r($numRole, true) , 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numRole);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);

	
  $roleList = fetchRolesAll('LISTS', $start, $limit);
  expDebug::dPrint(' $roleList = ' . print_r($roleList, true) , 4);

  // Prepare and send the response
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numRole;

  // $jsObjectInfo = roleJSObjectInfo();
  //$response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
  // have to do this.
  $response->initial_sort_type_html_id = 'role-sort-new';

  //$response->filters_html = roleNarrowSearchFiltersHTML($jsObjectInfo);
	
  $i=0;
  foreach ($roleList as $role) {
    expDebug::dPrint(' $role = '. print_r($role, true) , 4);
    $role->row =  $i;
    $response->rows[$i]['id']   = $role->id;
    $role->entityId = $role->id;
		$role->entityType = 'cre_sec';
    $response->rows[$i]['cell'] = array('details' => getRoleDetailsHTML($role),
    );

    $i++;
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendRolesListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchRolesAll($op, $start, $limit) {
	try {
	  // Select slt_security_role table
	$select = db_select('slt_security_role', 'role');
	
	$select->leftJoin('slt_profile_list_items', 'plistatus', 'role.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
	  if($op=='COUNT'){
    // Get count of active users
    $select->addExpression('COUNT(role.id)', 'count');
  }
  else{
    // Select fields/expression values to be fetched from the database.
    $select->addField('role', 'id', 'id');
    $select->addField('role', 'name', 'name');
    $select->addField('role', 'description', 'description');
    $select->addField('role', 'status', 'status_code');
    $select->addField('role', 'permissionlist', 'permission');
    $select->addField('role', 'userlist', 'user');
    expDebug::dPrintDBAPI(' $select =',$select);
  } 
	if(isset($_GET['username']) && !empty($_GET['username'])){
      	$select->Join('slt_person', 'per', '1=1');
        $select->where('FIND_IN_SET(per.id,ifnull(role.userlist,0))>0');
      	selectAddNarrowSearchByTextCondition($select, 'per', 'user_name', 'username', t('LBL181')); // search by attached courseName
      }
   selectAddNarrowSearchINCondition($select, 'role', 'status', 'rolestatus'); 
   selectAddNarrowSearchByTextMultipleORCondition($select, "role", array("name"), "textfilter");
  
   $select->condition('role.status', array('cre_sec_sts_del'), 'NOT IN');
   expDebug::dPrintDBAPI(' $select obj = ' , $select);
 if($op != "COUNT"){
    // Order the records
    roleSearchSelectStmtAddOrderByClause($select, 'role', "AZ");
  }
  
  
  
  if($op == "COUNT"){
    $result = $select->execute()->fetchField();
    expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
    return $result;
  }
  elseif ($op == "CSV") {
    $colHeadersMap = array(
      t('LBL107') => 'name',
      t('LBL229') => 'description',
      t('LBL160') => 'user'
    );
  
	  expDebug::dPrint(' $colHeadersMap = ' . print_r( $colHeadersMap, true) , 4);
    SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', array(), '', 'role_admin_search');
  }
  elseif($op == "PDF"){ 
    // % width of the columns must total to 100%
  	$colHeadersMap = array(t('LBL107') => array('name',      44),
                           t('LBL229') => array('description',    15), 
                        	 t('LBL160') => array('user',    15),
  	                        );
   $appliedFilters = array(t('Role') => textboxFiltersetValueToPrintableStr('textfilter'),
                           t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('rolestatus'),
                           ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
                          );

  SendNarrowSearchResultsAsPDFFile($select, t('Role'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'role_admin_search');
    
  }
  else{
     // Limit to $limit records
     $select->range($start, $limit);
     expDebug::dPrint(' sdghskasdjas');
    // Execure the query and fetch all the records
    $result = $select->execute()->fetchAll();
    expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3); 
    return $result;
  } 
  } catch (Exception $ex) {
  	watchdog_exception('fetchRolesAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function role_admin_search_csv_row_data_alter($rowData) {
  try {
    $rowData[0]->user = getRoleUsereName($rowData[0]->user);
    $rowData[0]->description = strip_tags($rowData[0]->description);
  }
  catch (Exception $ex) {
    watchdog_exception('role_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function role_admin_search_pdf_row_data_alter($rowData) {
  try {
    $rowData[0]->user = getRoleUsereName($rowData[0]->user);
    $rowData[0]->description = strip_tags($rowData[0]->description);
  }
  catch (Exception $ex) {
    watchdog_exception('role_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}
  
function getRoleTextFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_security_role', 'role');

  // Add fields to fetch
  $select->addField('role', 'name', 'name');

  // Add condition
  $select->condition('role.name', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('role.status',array('cre_sec_sts_itv','cre_sec_sts_atv'),'IN');

  // Order in dictionary sort order
  $select->orderBy('name');

  expDebug::dPrintDBAPI(' $select obj = vince' , $select);

  $matchingRoleNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingRoleNames = '. print_r($matchingRoleNames, true) , 3);

  foreach ($matchingRoleNames as $roleName) {
    print $roleName->name . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getRoleTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
function getRoleDetails($roleId, $fields) {
  try{
  if (empty($fields)) {
    // Get all fields
    $fields = array('id', 'name','description', 'status','permissionlist','userlist',
					'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
  }
  
  // Select table
  $selectStmt = db_select('slt_security_role', 'role');
  
  // Set conditions
  $selectStmt->condition('role.id', $roleId);

  // Select fields and/or add expressions
  foreach($fields as $fieldName) {
    $selectStmt->addField('role', $fieldName, $fieldName);
  }
    
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();
  
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getRoleDetails', $ex);
    expertusErrorThrow($ex);
  }  
}
/** adding new roles*/
function addNewRole($form, &$form_state, $paramuserid="", $callFromAPI = 0) 
{
   expDebug::dPrint(' $form = adddddd' . print_r($form, true) , 5);
   expDebug::dPrint(' $form_state = adddroleeeee' . print_r($form_state, true) , 5);  
  
  $userid = $paramuserid;
      
  $txn = db_transaction();

  try {
    
     $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
     $in_active_on          = $form_state['values']['status'] == 'cre_sec_sts_itv' ? now() : NULL;
     $in_active_reason      = $form_state['values']['status'] == 'cre_sec_sts_itv' ? $inactivereasontext : NULL; 
     $description = trim($form_state['values']['description']['value']);
     if($callFromAPI){
       drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
       $description = trim($form_state['values']['description']);
     }
         
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_security_role');
    $fields = array(
                'name'              => trim($form_state['values']['name']),
                'description'       => $description,
                'status'            => 'cre_sec_sts_atv',
                'created_by'        => $userid,
                'created_on'        => now(),
    						'updated_by'        => $userid,
    						'updated_on'        => now()
              );


    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);
  
    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);  
      
    // Execute the insert statement. Fetch the new roleId. 
    $roleId = $insertStmt->execute();
    expDebug::dPrint(' sabeee $roleId = ' . $roleId);  
   
    // Save the new organization's id in the hidden id form field.
    $form_state['values']['id'] =  $roleId;
    
    //Entry for securitymanagementAdmin
  	require_once "./modules/user/user.admin.inc";
		$rname	= trim($form_state['values']['name']);
		$min_form_state = array( "values" =>  array( "op" => t('Add role'), "name" => "$rname" ) );
	  user_admin_role_validate(null, $min_form_state);
	  $form_errors = form_get_errors();
	  if (empty( $form_errors )||$form_errors=='') {
	      user_admin_role_submit(null, $min_form_state);
	      }
      
    // Remove tmp space
    unset($form_state['storage']['acvalues']);
    //Audit Entry
    $formFieldsList = array('name', 'description','status','created_by','created_on');
    
    make_audit_trail_entries($fields, getIdOfLoggedInUser(), null, $roleId , 'Inserted', 'cre_sec', 'exp_sp_administration_role', 'addNewRole',
                                          $formFieldsList);
     
    return array((object)array('id'=>$roleId));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewRole', $ex);
    return array((object)array('id'=>"failure"));
  }
}
/*
 * updateRoleDetails - Updates an existing role details when the user clicks save
 */
function updateRoleDetails($form, &$form_state, $paramuserid="", $callFromAPI = 0) {
   expDebug::dPrint(' $form = 12334567890' . print_r($form, true) , 5);
   expDebug::dPrint(' $form_state = sabiiiii' . print_r($form_state, true) , 5);
  
  $userid = $paramuserid;
      
  $txn = db_transaction();

  try {

/*     $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
     $in_active_on          = $form_state['values']['status'] == 'cre_org_sts_itv' ? now() : NULL;
     $in_active_reason      = $form_state['values']['status'] == 'cre_org_sts_itv' ? $inactivereasontext : NULL; 
*/    
     $description = trim($form_state['values']['description']['value']);
     if($callFromAPI){
       drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
       $description = trim($form_state['values']['description']);
     }     
    // Select table to update
    $updateStmt = db_update('slt_security_role');
      
    // Set conditions
    $updateStmt->condition('id', $form_state['values']['id']);
    $formid=$form_state['values']['id'];
    $default= db_query("SELECT id,name,description,status from slt_security_role where id=:uid",array(":uid"=>$formid))->fetchAll();
    expDebug::dPrintDBAPI(' default_values = ' , $default);

    $fields = array(
      'name'              => trim($form_state['values']['name']),
      'description'       => $description,
      'status'            => 'cre_sec_sts_atv',
      'updated_by'        => $userid,
      'updated_on'        => now()
    );

    
    
    // Select the fields and the values to be updated
    $updateStmt->fields($fields);
  
    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);
    
    // Execute query
    $numUpdated = $updateStmt->execute();
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);
    
    //Entry for securitymanagementAdmin
    $select = db_select('role','role');
    $select->addField('role','rid','rid');
    $select->condition('role.name',$default[0]->name,'=');
    expDebug::dPrintDBAPI('Fetch role Id ' , $select);
    $role_id =$select->execute()->fetchField();
    
    expDebug::dPrintDBAPI('Fetch role Id ' , $select);
    
  	expDebug::dPrint("Role update starts");
		$rname 		= trim($form_state['values']['name']);
		//$role_id	= $form_state['values']['id'];
		expDebug::dPrint("Drupal role id:".$role_id);
		$action = $role_id>0?"Save role":"Add role";
		expDebug::dPrint("Role action in update :".$action);
		require_once "./modules/user/user.admin.inc";
		$min_form_state = array( "values" =>  array( "op" => t($action), "name" => "$rname","rid" => $role_id ) );
		expDebug::dPrint("min state".print_r($min_form_state,true));
		if(strtolower($rname)=='anonymous user' or strtolower($rname)=='authenticated user'){
				$form_errors['name']	= 'You can not edit this role.';
		}else{
		     user_admin_role_validate(null, $min_form_state);
		     $form_errors = form_get_errors();
		}
	  if (empty( $form_errors )) {
	       user_admin_role_submit(null, $min_form_state);
	  }
  
    //Audit Trail Entry
    $formFieldsList = array('name', 'description','status');

    make_audit_trail_entries($fields, getIdOfLoggedInUser() , $default ,$formid, 'Updated', 'cre_sec', 'exp_sp_administration_role',
                                         'updateRoleDetails', $formFieldsList);
    //addTaxonomyTermForOrg($form_state['values']['id'], $form_state['values']['name'], '', $form_state['values']['status']);
  
    // Insert Tax Exempted value in slt_entity_profile_mapping table
    //saveTaxExemptedOrgCustomProfile($form_state['values']['id'], $form_state['values']['tax_exempted'], $userid);
    return array((object)array('id'=>$form_state['values']['id']));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateRoleDetails', $ex);
    return array((object)array('id'=>'failure'));
  }
}
function activateOrDeactivateRole($roleId,$status='',$printResult=true){
	expDebug::dPrint('actor deact'.$status);
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    if($status==''){
     $status  = getRoleStatus($roleId);
     $updatedstatus  = ($status == 'cre_sec_sts_atv') ? "cre_sec_sts_itv" : "cre_sec_sts_atv";
     }else{
     $updatedstatus  = ($status == 'Activate') ? "cre_sec_sts_atv" : "cre_sec_sts_itv";
     if($status == 'Activate'){
     	$status = 'cre_sec_sts_itv';
       }else{
     	$status = 'cre_sec_sts_atv';
       }
    }
    $updateStmt = db_update('slt_security_role');
    $updateStmt->condition('id', $roleId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,                                                   
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
    expDebug::dPrintDBAPI(' sabeena ' ,  $updateStmt);
    //Entry for securitymanagementAdmin
    $select = db_select('role','role');
  	$select->join('slt_security_role','srole','role.name=srole.name');
    $select->addField('role','rid','rid');
    $select->addField('srole','userlist','ulist');
	  $select->condition('srole.id',$roleId,'=');
	  expDebug::dPrintDBAPI('Fetch role Id ' , $select);
	  $dbquery = $select->execute()->fetchAll();
	  $role_id = $dbquery[0]->rid;
	  $newUserList = $dbquery[0]->ulist;
	  if(!empty($newUserList)){
		    $newUserListArray = explode(',',$newUserList);
		    foreach($newUserListArray as $new){
		    	$results = db_select('users','usr');
	  			$results->join('slt_person','per','usr.name=per.user_name');
	  			$results->addField('usr','uid','uid');
	  			$results->condition('per.id',$new,'=');
	  			$usersId = $results->execute()->fetchField();
	  			if($updatedstatus == "cre_sec_sts_atv"){
		  			$select1 = db_insert('users_roles');
		        $select1->fields(array('uid' => $usersId, 'rid' => $role_id));
		        expDebug::dPrintDBAPI('Insert userlist ' , $result);
		        $select1->execute();
	  			}else if($updatedstatus == "cre_sec_sts_itv"){
	  				$select1 = db_delete('users_roles');
	        	$select1->condition('uid',$usersId);
	        	$select1->condition('rid',$role_id);
	        	expDebug::dPrintDBAPI('delete userlist ' , $select1);
	        	$select1->execute();
	  			}
		    }
	    }
	    
    //Audit Trail entry
    add_audit_trail_entry($currUserId, $roleId, 'cre_sec', 'exp_sp_administration_role', 'activateOrDeactivateRole', 'Updated Role Status',
		                                     null, $status, $updatedstatus);
    if($printResult){
      print   ($updatedstatus == 'cre_sec_sts_atv') ? t('LBL572') : t('LBL573');
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('activateOrDeactivateRole', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * getDefaultRoleAddEditFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultRoleAddEditFormFieldValue($field, $form_state, $roleDetails) {
	try {
 $defaultValue = '';  
  switch ($field) {
    case 'shordesc':
      $defaultValue = (empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='')?
                                    ((empty($roleDetails[$field]) || $roleDetails[$field]=='')? '' : $roleDetails[$field]) :
                                    $form_state['values'][$field]['value'];
  	  break;
      default:   // For all other fields
      $defaultValue = getDefaultAddEditFormFieldValue($field, $form_state, $roleDetails);
      break;
  }
     
  expDebug::dPrint(' $defaultValue for field ' . $field . ' = ' . print_r($defaultValue, true) , 4);
  
  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultRoleAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}
function getAvailableUserrole($roleId) {
	try{
	$select = db_select('slt_security_role','role');
	$select->addExpression('count(userlist)','count_users');
	$select->condition('role.id',$roleId,'=');
	$result = $select->execute()->fetchField();
	expDebug::dPrintDBAPI(' sabeena 123466789' ,$select);
	return $result;
	}catch(Exception $ex){
		watchdog_error('getAvailableUserrole',$ex);
		expertusErrorThrow($ex);
	}
}

function getRoleStatus($roleId){
	try {
  $select = db_select('slt_security_role', 'role');
  $select->addField('role','status', 'status');
  $select->condition('role.id',$roleId);
  // Execute the query
  return $select->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('getRoleStatus', $ex);
  	expertusErrorThrow($ex);
  }
}
function addUsersDetails($entityId,$entityType){
  try {
  	 expDebug::dPrint('touching function');
  	 expDebug::dPrint('entittt-->'.$entityId.'entyp-->'.$entityType);
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'entity_type'  => $entityType,
       'call_from'    => 'add_user'
      );      
      
      $formId                 = 'addusr_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);   
      $drupalSettings         = getDrupalSettings();
    
      $addUsersResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      ); 
     
      echo drupal_json_output($addUsersResponse);
      exit;
       
  } catch (Exception $ex) {
    watchdog_exception('addUsersDetails', $ex);
    expertusErrorThrow($ex);
  }
  
}

function attachUsersDetails($form, &$form_state){
  try {
  	include_once(drupal_get_path('module', 'exp_sp_administration_role') .'/exp_sp_administration_role.inc');
  	   $entityId = $form_state['triggering_element']['#ajax']['role_id'];
			 $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  	
  	 expDebug::dPrint('touching function');
  	 expDebug::dPrint('entittt-->'.$entityId.'entyp-->'.$entityType);
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'entity_type'  => $entityType,
       'call_from'    => 'attach_user'
      );      
      
      $commands = array();
      $commands[] = exp_ctools_modal_adjust();
    	$commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
    	$builtForm            = drupal_build_form('addusr_addedit_form_html', $form_state);
   		$commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
    	return array('#type' => 'ajax', '#commands' => $commands);
    	
      /*$formId                 = 'addusr_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);   
      $drupalSettings         = getDrupalSettings();
    
      $addUsersResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      ); 
     
      echo drupal_json_output($addUsersResponse);
      exit;*/
       
  } catch (Exception $ex) {
    watchdog_exception('attachUsersDetails', $ex);
    expertusErrorThrow($ex);
  }
  
}


function showAttachUserToRole($form, &$form_state){
	try {
  	include_once(drupal_get_path('module', 'exp_sp_administration_role') .'/exp_sp_administration_role.inc');
  	$entityId = $form_state['triggering_element']['#ajax']['role_id'];
  	$entityType = 'cre_sec';

  	 expDebug::dPrint('entittt-->'.$entityId.'entyp-->'.$entityType);
     $form_state = array(
		    'no_redirect'  => TRUE,
		    'build_info'   => array('args' => array('ajax')),
		    'role_id'    => $entityId,
		    'entity_type'  => $entityType,
		 //   'form_arg_type' => $argType
		 		'mode'				 => 'view'
		  );        
      
      $commands = array();
      $commands[] = exp_ctools_modal_adjust();
    	$commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
    	$builtForm  = drupal_build_form('display_assigned_users', $form_state);
    	$commands[] = ajax_command_refresh_narrow_search_results('onclose');
   		$commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
   		
    	return array('#type' => 'ajax', '#commands' => $commands);
    	} catch (Exception $ex) {
    		watchdog_exception('showAttachUserToRole', $ex);
    		expertusErrorThrow($ex);
    	}
}

function addusr_addedit_form_html($form,&$form_state) {
  try {
    return displayAddUsersDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type']);
  } catch (Exception $ex) {
    watchdog_exception('addusr_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }
  
}

function displayAddUsersDetails($form,&$form_state,$roleId, $userId){
	try {
 	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
 	expdebug::dPrint('user value'.$form_state['user_id']);
 	$call_from    = $form_state['call_from'];
	$role_id = $form_state['entity_id'];
	$entityType = 'cre_sec';
	$userId= $form_state['user_id'];
	$excludedRoleId = '';
	$uniqueId = "RoleAddUsers-".$role_id."-".$userId;
	$wrapperId = 'security-role-detail-wrapper';
	
	$displayMode = 'edit';
 	$form_state['display_mode'] = $displayMode;
  	
  	
	$form = array(); 
	$form['#prefix'] = (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) .'<div id="security-role-detail-wrapper">';
    $form['#suffix'] = '</div>';
	$setminHight =  'admin-add-users-tab-datagrid-wrapper';
	$form['edit-class-details-start'] = array(
	  '#prefix' => "<div id='add-user-role-page'>",
	);
	
	if(isset($addAttributes))
      	unset($addAttributes['onclick']);
  $addAttributes['class'][] = 'narrow-search-actionbar-orange-btnBG';
  if($call_from=='add_user'){
  	$addAttributes['data-wrapperid'] = array('role-detail-wrapper');
  }else{
  	$addAttributes['data-wrapperid'] = array('security-role-detail-wrapper');
  }
  $form['add-class-list'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="catalog-class-addedit-form-details"><div id="add-class-list-wrapper">'
  );
     /* hidden input type to carry the selected users */
    $form['hidden_idlist'] = array(
      '#type' => 'hidden',
      '#name' => 'hidden_idlist_'.$uniqueId,
      '#id' => 'datagrid-idlist-'.$uniqueId,
    );
  $paintMultiAction  = "<div id='search-list-class-title-keyword' class='search-list-keyword' style='display:block;'>";
     $paintMultiAction .= "<span id='search-dropdwn-list-class'>";
     $paintMultiAction .= "<input type='hidden' id='search_all_user_type-hidden' value='usrtit' />";
     $paintMultiAction .= "<input type='hidden' id='addusername-search-autocomplete_hidden' value='".t('LBL181')."' />";
     $paintMultiAction .= "<span id='select-list-class-dropdown-adduser' class='select-list-dropdown-class'>".t('LBL054')."</span>";
     $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-class-dropdown-link dropdown-link-font ' onclick='moreRoleHideShow();'> </a>";                                                                
     $paintMultiAction .= "<ul id='select-list-class-dropdown-list' class='more-drop-down'>";
     $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreAdduserSearchTypeText('".t('LBL054')."','usrtit','role');\">".t('LBL054')."</li>";
     $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreAdduserSearchTypeText('".t('LBL691')."','fultit','role');\">".t('LBL691')."</li>";    
     $paintMultiAction .= "</ul>";
     $paintMultiAction .= "</span></div>";
   
     $form['class_multi_search_container'] = array(
     '#type' => 'markup',
     '#markup' => '<div class="role_multi_search_container_add">', //class="admin_class_multi_search_container"
     );
     $form['more_add_enrolluser_search'] = array(
     '#type' => 'markup',
     '#markup' => $paintMultiAction,
     );
     $form['class_auto-search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin-class-auto-search-container">',
      );
      $userSearchType = '';
      $userSearchType = $form_state['storage']['search_usr_type'];
      // onload="$(\'#root-admin\').data(\'narrowsearch\').classDisplayPagination('.$courseId.','.$classId.');"
      $searchAjaxEvent = 'click';
     // $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "enrolltpuser", $(\'#enrolltpuser-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "0"); return false;';
      
    
      $acMenuPath = 'administration/people/role/user-name-autocomplete/'.$role_id.'/'.'addusers';
      
			$excludedRoleId =0;
      $default_text_title = t('LBL181'); 
      $autoCompleteAttributes['class'][] = 'admin_class_ac_input_mainform';
     // $autoCompleteAttributes['class'][] = 'admin_ac_class_txtbox';
      //$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      /*addACFieldForMultiSelect($form, 'username-search-autocomplete', $acMenuPath, '', '', '', array(),
                                  array('search_type' => "$('#search_all_user_type-hidden').val()"), false, false,
                                  $default_text_title, $autoCompleteAttributes);*/
      $autoCompleteAttributes['onkeypress'][]   = 'disableEnrterKey(event);'; // added by vincent for prevent the enter key submit in search box
     // $autoCompleteAttributes['onkeyup'][]   = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "RoleAddUsers", $(\'#addusername-search-autocomplete\').val(), "'.$role_id.'", "'.$userId.'", "'.$excludedRoleId .'"); return false;';
      addACFieldForMultiSelect($form, 'addusername-search-autocomplete', $acMenuPath, '', '', '', array(), array('entity_id'  =>  $role_id, 'entity_type' => "'".$entityType."'" ,'search_type' => "$('#search_all_user_type-hidden').val()",'excluded_role_id' => "'".$excludedRoleId."'", 'id_required' => 0), false, false, $default_text_title,$autoCompleteAttributes);
      $form['class_auto-search-end'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      
      $form['tip-position'] = array(
      '#type' => 'markup',
      '#markup' => '<input type="hidden" id="qtip_position" name="qtip_position" value="">',
      );

      
      $excludedRoleId =0;
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "RoleAddUsers", $(\'#addusername-search-autocomplete\').val(), "'.$role_id.'", "'.$userId.'", "'.$excludedRoleId .'"); return false;';
      //$acMenuPath = 'administration/people/role/user-name-autocomplete/'.$role_id.'/'.'addusers';
      
      $searchAttributes['Class'][] = 'admin-pagination-search-go narrow-text-search';
       $form['search_addusers'] = array(
        '#type' => 'submit',
       	'#id' => 'search-addusers',
        '#title' => t('LBL304'), //Search
        '#name' => 'Search addusers',
        '#submit' => array('search_addusers'), // No such function search_attachcourse(), apparently property is ignored.
        '#attributes' => $searchAttributes, 
      );
      $form['class_multi_search_container-end'] = array(
      '#type' => 'markup',
      '#markup' => '<div class= "filter-search-start-date-right-bg"></div></div>',
      );
	$form['edit-class-details-paginate'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-add-course-training-plan" class="display_user_list"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
    '#suffix' => '<div>',
  );
  $form['role_user_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common" style="display:none;"></div></div>',
   );      
	$form['role_user_grid_load'] = array( 
	  '#type' => 'markup',
	//  '#prefix' => '<div style="display:block"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'view\', \'RoleDisplayUsers\', \'\', \''.$role_id.'\', \''.$entityType.'\', \''.$excludedRoleId.'\');" width="100" height="132" /></div>',
	  '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'RoleAddUsers\', $(\'#addusername-search-autocomplete\').val(), \''.$role_id.'\', \''.$userId.'\',\'0\');" width="100" height="132" /></div>',
	); 
	$form['role_user_grid_load_end'] = array( 
	  '#type' => 'markup',
	  '#prefix' => '</div></div>',
	); 
	
   $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
   $cancelAttributes['onclick'][]         = ($call_from=='add_user') ? "$('.qtip-active').remove();return false;" : "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
   $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
   $form['cancel_adduser_edit_mode'] = array(
     '#type' => 'submit',
     '#value' => t('LBL123'),
     '#title' => t('LBL123'),
     '#prefix' => '<div id="attach_user" ><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
     '#name' => 'Cancel addusers',
     '#attributes' => $cancelAttributes,
   );
   module_load_include('inc', 'exp_sp_administration_learning', 'exp_sp_administration_learning');
   $type='RoleAddUsers';
   $listquery = fetchUserRoleList($role_id,'',$type);
   expDebug::dPrintDBAPI('listquery', $listquery);
   if($call_from=='add_user'){
   	$callback = 'addUsersCallBack';
   	$wrapper='role-detail-wrapper';
   	$setAttributes['data-wrapperid'] = 'role-detail-wrapper';
   }else{
   	$callback = 'showAttachUserToRole';
   	$wrapper = 'security-role-detail-wrapper';
   	$setAttributes['data-wrapperid'] = 'security-role-detail-wrapper';
   }
   if(count($listquery) != 0){
      expDebug::dPrint('count($listquery)'.count($listquery));
      $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
      $form['set_attachcourse_edit_mode'] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="admin-save-button-container role-add-user-btn"><div class="admin-save-button-left-bg"></div>',
        '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div></div>',
        '#title' => t('LBL1038'), 
        '#value' => t('LBL287'), 
        '#name' => t('LBL287'),
        '#submit' => array('addUsersDetailsList'),
        '#attributes' => $setAttributes,
        '#ajax' => array(
	        'wrapper' => $wrapper,
	        'callback' => $callback,
	        'role_id' =>$role_id,
	        'user_id' =>$userId,
	        'method' => 'replace',
	        'effect' => 'none', // 'fade',
	        'event'=>'click',
	        'keypress' => true,
	        'progress' => array(
	           'type' => 'throbber',
	           'message' => ''
	         ),
      	),  
      );
    }

  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('displayAddUsersDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function addUsersCallBack($form, &$form_state){  
	try{
  $roleId = $form_state['triggering_element']['#ajax']['role_id'];
  $entityType = 'cre_sec';
  
  expDebug::dPrint('triggering elemnt 1 -'.$form_state['triggering_element']['#ajax']['role_id']);
  
  expDebug::dPrint('exp_sp_administration_grouping_users_callback formstateee : '.print_r($form_state,true) , 5);
  
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'role_id' => $roleId,
   'build_info' => array('args' => array('ajax')) 
  );
  
  $form_state = $min_form_state;
  //$form_state['role_id'] = $roleId;
  
  $formId = 'exp_sp_administration_role_attaching_user_form';
  $builtForm = drupal_build_form($formId, $form_state);

  $commands = array();
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  $commands[] = ajax_command_invoke(NULL, "attachClose", array($roleId,$entityType));
  $commands[] = ajax_command_refresh_narrow_search_results('onclose');
  //$commands[] = exp_ctools_modal_adjust(); 

  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
	}catch (Exception $ex) {
    watchdog_exception('addUsersCallBack', $ex);
    expertusErrorThrow($ex);
  }
}

function getUserDetailList($roleId,$start = 0,$end = 0,$userName,$userSearchType){
	//$userName = $_GET['z'];
	try {
		 expDebug::dPrint('role id 333---'.$start.'dfds'.$end);
	   $userId = array(1,2);
	   $select = db_select('slt_person','pre');
	   $select->addField('pre','id','user_id');
	   $select->addField('pre','user_name','uname');
	   $select->addField('pre','full_name','fname');
	   $select->addField('pli','name','status');
	   $select->join('slt_profile_list_items','pli','pre.status=pli.code');
	   $select->condition('pre.id',$userId,'not in');
	   	expDebug::dPrint('role id 4444---'.$roleId);
	   $select->where("FIND_IN_SET(pre.id,(select ifnull(r.userlist,0) from slt_security_role r where id=$roleId))>0");
	   

      if(!empty($userName)&& $userName!=t('LBL181') && $userSearchType=='usrtit'){
      $select->condition(db_or()->condition('pre.user_name', '%' . db_like($userName) . '%' ,'like')
  	                             ->condition('pre.full_name', '%' . db_like($userName) . '%' ,'like'));
      }
      if(!empty($userName)&& $userName!=t('LBL036'). ' '.t('LBL691') && $userSearchType=='fulltit' ){
       $select->condition('pre.full_name', '%' . db_like($userName) . '%', 'LIKE'); 
      }
      if(($start > 0) || ($end > 0)) {
          $select->range($start,$end);
      }
      expDebug::dPrintDBAPI('list of users in role' ,   $select);
      $result = $select->execute()->fetchAll();
      expDebug::dPrint(' $result = ' . print_r($result, true) ,3);
      return $result;
      } catch (Exception $ex) {
      	watchdog_exception('getUserDetailList', $ex);
      	expertusErrorThrow($ex);
      }
}  

function addPermissionsDetails($callFrom, $entityId, $entityType) {
	try {
	expDebug::dPrint('addPermissionsDetails'.$entityId,4);
 $form_state = array(
    'no_redirect'  => TRUE,
    'build_info'   => array('args' => array('ajax')),
    'entity_id'    => $entityId,
    'entity_type'  => $entityType,
    'form_arg_type' => $argType,
    'call_from'			=>$callFrom
  );      
  expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
  $formId                 = 'permissions_addedit_form_html';
  $builtForm              = drupal_build_form($formId, $form_state);
  $renderedTabContentHTML = drupal_render($builtForm);   
  $drupalSettings         = getDrupalSettings();

  $response       = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
  ); 
 
  echo drupal_json_output($response);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('addPermissionsDetails', $ex);
  	expertusErrorThrow($ex);
  }  
}

function attachUserToRole($entityId, $entityType){
	try {
	expDebug::dPrint('addUsersToRole'.$entityId,4);
 	$form_state = array(
    'no_redirect'  => TRUE,
    'build_info'   => array('args' => array('ajax')),
    'role_id'    => $entityId,
    'entity_type'  => $entityType,
 //   'form_arg_type' => $argType
 		'mode'				 => 'view'
  );      
  expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
  $formId                 = 'display_assigned_users';
  $builtForm              = drupal_build_form($formId, $form_state);
  $renderedTabContentHTML = drupal_render($builtForm);   
  $drupalSettings         = getDrupalSettings();

  $response       = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
  ); 
 
  echo drupal_json_output($response);
  exit; 
  } catch (Exception $ex) {
  	watchdog_exception('attachUserToRole', $ex);
  	expertusErrorThrow($ex);
  } 
}

function permissions_addedit_form_html($form,&$form_state){
	try {
	expDebug::dPrint('permission detail'.print_r($form_state,true));
	
	$entityId = $form_state['entity_id'];
  $entityType = $form_state['entity_type'];
  $callFrom = $form_state['call_from'];
  $keyType = $entityId.'_'.$entityType;
  $uniqueId = 'permissions-'.$entityId.'-'.$entityType;
  //$formName = 'Capabilities';
  $formName= 'add-permissions';
  
  //$wrapperId = 'permissions-grid-wrapper-'.$uniqueId;
  //$customHiddenButtonId = 'equv-save-esign';
  expDebug::dPrint('valuess for form state keywords'.$uniqueId,5);
  expDebug::dPrint('$entityId'.$entityId);
  
  $dynamicwrapperid="permissions-addedit-form".$form_state['entity_id'].$form_state['entity_type'];
  $popuprenderPermId = "popup_container_qtip_permissions_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
  $popupDisplayId				 = "qtipPermissionsqtip_visible_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
  $form = permissionsNewAddEditFormWithCancelAndSave($dynamicwrapperid);
  
  addHiddenValueToAddEditForm($form, 'entity_id', $form_state['entity_id']);
  addHiddenValueToAddEditForm($form, 'entity_type', $form_state['entity_type']);
  addHiddenValueToAddEditForm($form, 'empty_id', $form_state['empty_id']);
  
  //All available permissionlist 
  $permissionsList  = fetchPermissionsList($entityId);
  expDebug::dprint('permission list--->'.print_r($permissionsList,true));
  // Get mapped permissions for the Entity Id and Entity Type
  $permissionMapping = getMappedPermissionMapping($entityId, $entityType);
  expDebug::dprint('permission mapping'.print_r($permissionMapping,true));
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-add-permissions" class="add-permission-class" >',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
    '#suffix' => '</div></div>',
  );
  
  $form[$formName]['attr_action'] = array(
      '#type' => 'hidden',
      '#name' => 'attr_action',
      '#value' => 'add_permissions',
  );  
  
  $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';
  expDebug::dPrint('permission list value'.print_r($permissionsList,true));
  if(!empty($permissionsList)){

  	$form[$formName]['permissions_table_start'] = array(
        '#type' => 'markup',
     );  
     
     $form[$formName]['permissions_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => '<table width="98%" cellpadding="4" cellspacing="0" border="0"><tr class="permission-header-tr"><td colspan="1" class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL1039').'</td><td class="certificate-column checkbox-unselected"><input type="checkbox" id = "select-all-checkbox" name="multi-select-checkbox" onclick="checkedAll(this,\''.$uniqueId.'\');checkboxSelectedUnselectedCommon(this);"></td></tr>',//<div id="permissions-fieldset-wrapper" class="admin-tab-list-row">',
        '#suffix' => '</div>',
     );  

    $i=0;
    $j=0;
    foreach($permissionsList as $key => $val){
      $checkedStatus=''; 
      // Already marked permissions should be checked
      $checkedStatus = in_array($val->code, $permissionMapping) ? 'checked' : '';
      $checkedClass = in_array($val->code, $permissionMapping) ? 'checkbox-selected' : 'checkbox-unselected';
      //addTwoColumnRowToAddEditForm($form, 'add_permissions_frequencylabel_'.$val->code);
      $class='permission-detail-tr-odd';
      if($j==1){
      	$class='permission-detail-tr-even';
      	$j=0;
      }else{
      	$j=1;
      }
      $form[$formName]['permissions_table_row'.$i] = array(
        '#type' => 'markup',
        '#prefix' => '<tr class="'.$class.'"><td class="user-list-detail certificate-column"><span class="vtip" title="'.$val->pname.'"> '.titleController('ADMIN-ROLE-PNAME',$val->pname,45).'</span></td><td class="certificate-column '.$checkedClass.'"><input type="checkbox" '.$readOnlyType.' id = "attach_add_permissions" class = "attach-permission-cls" onclick="checkboxSelectedUnselectedCommon(this);uncheckAllPermission();" name="attach_add_permissions'.$uniqueId.'[]" value="'.$val->code.'" '.$checkedStatus.'>',
        '#suffix' => '</td></tr>',
      ); 

      //$form['permissions_fieldset']['permissions_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->code);
      $form['permissions_fieldset']['permissions_hidden'][$val->code][0] = array('#type' => 'hidden', '#value' => $val->permission);
      $form['permissions_fieldset']['permissions_hidden'][$val->code][1] = array('#type' => 'hidden', '#value' => $val->parent1_perm);
      $form['permissions_fieldset']['permissions_hidden'][$val->code][2] = array('#type' => 'hidden', '#value' => $val->parent2_perm);
      //endAddEditTwoColumnRow($form);

      $i++;
      
    }
    
    $form[$formName]['permissions_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table>',
     );  
    
  } else {
    
      $form[$formName]['no-records-msg'] = array(
      	'#type' => 'markup',
      	'#markup' => '<div class="no-records-msg">'.t('There are no permissions available for this role').'</div></div>'
      );
  }
   
	permissionsAddEditFormWithCancelAndSaveClose($form,$dynamicwrapperid,'newtheme-save-button','newtheme-cancel-link',$popuprenderPermId,$popupDisplayId,$callFrom);
 
  return $form;
	
  } catch (Exception $ex) {
  	watchdog_exception('permissions_addedit_form_html', $ex);
  	expertusErrorThrow($ex);
  }
}

function permissions_addedit_form_html_submit ($form, &$form_state){
  try {
   expDebug::dPrint(":Calling business form : ".print_r($form_state,true));
   expDebug::dPrint('role_id'.print_r($form_state['entity_id'],true));
   $entityId      = $form_state['entity_id'];
   $entityType    = $form_state['entity_type'];
   $uniqueid      = 'permissions-'.$entityId.'-'.$entityType;
   expDebug::dPrint('role_id---->'.$uniqueid);
   updatePermissionsList($entityId, $entityType, $uniqueid, $form_state);
   $form_state['rebuild'] = TRUE; 
   } catch (Exception $ex) {
   	watchdog_exception('permissions_addedit_form_html_submit', $ex);
   	expertusErrorThrow($ex);
   }   
  
}

/*
 * permissionsNewAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom 
 */
function permissionsNewAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
 try { 
  $form = array();
  $form['#prefix'] = (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome') || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') )? '</form>' : '' ) .'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
  $form['#suffix'] = '</div>';
  
  // Add a class to the form so that we can apply custom style (margin, padding, etc).  
  $form['#attributes'] = array('class' => array('addedit-form'));
  $form['#attributes']['autocomplete'] = 'off';
  $form['#tree'] = TRUE; // Show full path of values (hierarchical values)
  
  $form['addedit-left-col-start'] = array(
    '#type' => 'markup',
    '#markup' => '<div>',
  );

      
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('permissionsNewAddEditFormWithCancelAndSave', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * permissionsAddEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave()
 */
function permissionsAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = '', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $popuprenderCertsId = "",$popupDisplayId='', $callFrom, $expandPopup = FALSE, $saveTabIndex = '', $esignature=false) {
	try {
	$customHiddenButtonId = 'esign-button-'.$wrapperId;

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);
  
  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }
  
  $ajaxCallbackCancel = 'addEditCancelRepaintForm';
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  
  
  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-bubble-close';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
  $cancelAttributes['class'][] = 'tags-close-button';
  $cancelAttributes['onclick'] = "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
  //'$("#'.$popuprenderCertsId.'").closest(".qtip-active").hide(); $(\'#'.$popupDisplayId.'_disp\').html(\'\'); return false;';
  
  if($callFrom=='edit'){
  	$aClass = 'button-margin-edit';
  }else{
  	$aClass = 'button-margin-search';
  }
  $setcls = ($theme_key=='expertusoneV2')?'':'resize-save_btn';
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="permission-cancel-save-btn" class="'.$aClass.' permission-button-size addedit-form-cancel-and-certificate-done-actions-row certificate-btn-container">' .
     			'<div class="addedit-form-cancel-container-actions '.$setcls.'"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );
  
  // Add the Save button
  $ajaxCallbackSave = 'addEditSaveAndCloseForm';
  
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  //Increase the width of the popup while clicking on save button
 /* if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
  $saveAttributes['onclick'][] = 'increasePopWidth();';
  }*/
  
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  $form[$saveButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
    '#suffix' => 
               '<div class="admin-save-button-right-bg"></div></div>' .
               '<div class="clearBoth"></div></div>' .
             '<div class="clearBoth"></div></div>',
    '#value' =>  t('LBL569'),
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

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('permissionsAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}
function displayUserList(){
		try {
		
		} catch (Exception $ex) {
			watchdog_exception('displayUserList', $ex);
			expertusErrorThrow($ex);
		}
}
function fetchPermissionsList($permId){
	try {
	expDebug::dPrint('fetch permission');
	$select = db_query("SELECT 
  lst.id id,
  lst.code code,
  lst.name pname,
  lst.attr4 permission,
  lst2.attr4 parent1_perm,
  lst3.attr4 parent2_perm 
from  
  slt_profile_list_items lst
  left join slt_profile_list_items lst2 on lst2.id = lst.parent_id and lst2.code like 'cre_sec_pmn_%'
  left join slt_profile_list_items lst3 on lst3.id = lst2.parent_id and lst3.code like 'cre_sec_pmn_%'
where
  lst.id not in (select parent_id from  slt_profile_list_items where code like 'cre_sec_pmn_%')
  and lst.code like 'cre_sec_pmn_%'
order by lst.name,lst.parent_id"
	);
	expDebug::dPrintDBAPI('list of permissions in role' ,   $select);
 	$result = $select->fetchAll();	
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('fetchPermissionsList', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * updatePermissionsList() - Updated the PermissionList for the Entity Id and Entity Type  
 */
function updatePermissionsList($entityId, $entityType, $uniqueid, $form_state){
	
	  $txn = db_transaction();
  	try{
  	// Audit Entry	
    $result1=getpermissiondetails($entityId);
  	expDebug::dPrint('5454545'.print_r($result1[0]->permli,true));
  	expDebug::dPrint('5454545'.print_r($resul1t[0]->acces,true));
  	$existpermissionlist[0]->permissionlist = $result1[0]->permli;
  	$existpermissionlist[0]->accesspermission = $result1[0]->acces;
 		
    // Permission form values  
    $AvailablePermList = $form_state['input']['permissions_fieldset']['permissions_hidden'];
    expDebug::dPrint('$AvailablePermList'.print_r($form_state['input'],true));
    $PermissionSelected = $form_state['input']['attach_add_permissions'.$uniqueid];
    expDebug::dPrint('$PermissionSelected'.print_r($PermissionSelected,true));
    $AccessPerm = array();
    for($i=0;$i<sizeOf($PermissionSelected);$i++){
    	$AccessPerm1[$i]= $AvailablePermList[$PermissionSelected[$i]][0];
    	$AccessPerm2[$i]= $AvailablePermList[$PermissionSelected[$i]][1];
    	$AccessPerm3[$i]= $AvailablePermList[$PermissionSelected[$i]][2];
    }
    expDebug::dPrint('$AccessPerm1'.print_r($AccessPerm1,true));
    expDebug::dPrint('$AccessPerm2'.print_r($AccessPerm2,true));
    expDebug::dPrint('$AccessPerm3'.print_r($AccessPerm3,true));
    $IndivPerm=array_unique($AccessPerm1);
    $Parentperm = array_unique($AccessPerm2);
    $MainParentperm = array_unique($AccessPerm3);
    expDebug::dPrint('$AccessPerm1---'.print_r($IndivPerm,true));
    expDebug::dPrint('$AccessPerm2---'.print_r($Parentperm,true));
    expDebug::dPrint('$AccessPerm3--'.print_r($MainParentperm,true));
    $AccesPermList =array_merge($IndivPerm,$Parentperm,$MainParentperm);
    expDebug::dPrint('$AccesPermList'.print_r($AccesPermList,true));
    
    //Update permissionlist in table
    $PermissionSelected = empty($PermissionSelected) ? array() : $PermissionSelected;
    if(!empty($PermissionSelected)){
    		foreach($PermissionSelected as $key){
    			$PermissionSelectedList = empty($PermissionSelectedList)? $key : $PermissionSelectedList.','.$key;
    		}
    	}else{
    		$PermissionSelectedList = '';
    	}
    	expDebug::dPrint('update listttttt'.$PermissionSelectedList);
    	$select = db_update('slt_security_role');
    	$select->fields(array('permissionlist'=>$PermissionSelectedList));	
			$select->condition('id', $entityId, '=');
  		expDebug::dPrintDBAPI('list of permissions mapping in role', $select);
  		$select->execute();
  		
  		//Update Access permission in table
  		$FinalAccesPermList = '';
    if(!empty($AccesPermList)){
    		foreach($AccesPermList as $key){
    			$FinalAccesPermList = empty($FinalAccesPermList)? $key : $FinalAccesPermList.','.$key;
    		}
    	}else{
    		$FinalAccesPermList = '';
    	}
    	expDebug::dPrint('update access list'.$FinalAccesPermList);
    	$select = db_update('slt_security_role');
    	$select->fields(array('accesspermission'=>$FinalAccesPermList));	
			$select->condition('id', $entityId, '=');
  		expDebug::dPrintDBAPI('list of permissions access in role', $select);
  		$select->execute();
  		
  		//Entry for securitymanagementAdmin
  		
  		$select = db_select('role','role');
  		$select->join('slt_security_role','srole','role.name=srole.name');
    	$select->addField('role','rid','rid');
	    $select->condition('srole.id',$entityId,'=');
	    expDebug::dPrintDBAPI('Fetch role Id ' , $select);
	    $role_id =$select->execute()->fetchField();
	    
			db_query('DELETE FROM {role_permission} WHERE rid = :rid', array(':rid' => $role_id));
  	  $permissionArray  = explode(',',$FinalAccesPermList);
      expDebug::dPrint("Permission Creation  :", 4);
      if(!empty($permissionArray)&&!empty($FinalAccesPermList)){
        foreach($permissionArray as $permission){
          expDebug::dPrint("Permission list Starts :".$permission);
          if($permission){
	          $result  = db_insert('role_permission');
	          $result->fields(array('rid' => $role_id, 'permission' => trim($permission)));
	          expDebug::dPrintDBAPI('Insert Permission ' , $result);
	          $result->execute();
          }
        }
      }
			
			  		
  		//Audit trail entry
  		$result=getpermissiondetails($entityId);
  		expDebug::dPrint('5454545'.print_r($result[0]->permli,true));
  		expDebug::dPrint('5454545'.print_r($result[0]->acces,true));
  		$updatedPermissionList['permissionlist'] = $result[0]->permli ;
  		$updatedPermissionList['accesspermission'] = $result[0]->acces ;
  		make_audit_trail_entries($updatedPermissionList, getIdOfLoggedInUser() , $existpermissionlist ,$entityId, 'Updated', $entityType, 'exp_sp_administration_role',
            	                                 'Updatedpermissionlist', array('permissionlist','accesspermission'));
  		
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updatePermissionsList', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
	
}

/*
 * getMappedPermissionMapping() - Get Mapped Business Rules for the Entity Id and Entity Type  
 */
function getMappedPermissionMapping($entityId, $entityType){
		try {
			$select = db_select('slt_security_role','role');	
			$select->condition('id', $entityId, '=');
			$select->addField('role','permissionlist','permli');
			expDebug::dPrintDBAPI('getMappedPermissionMapping', $select);
			$result = $select->execute()->fetchField();
			$permResult = explode(",", $result);
			return $permResult;
			} catch (Exception $ex) {
				watchdog_exception('getMappedPermissionMapping', $ex);
				expertusErrorThrow($ex);
			}	
}

function getUsernameAutocompletesearch($roleId,$modId){
	try{
		$nameSubstr = $_GET['z']; 
	  $entityId   = $roleId;
	  $searchType = $_GET['search_type'];
	  expDebug::dPrint('111111---'.$nameSubstr);
		expDebug::dPrint('222222'.$entityId);
		expDebug::dPrint('3333333'.$searchType);
	  $select = db_select('slt_security_role','rol');
	  $select->join('slt_person','per');
	  $select->addExpression('DISTINCT(per.user_name)', 'name');
	  $select->condition('rol.id', $entityId, '=');
	  $select->condition('per.status','cre_usr_sts_atv','=');
		if($searchType == 'usrtit' || $searchType ==t('LBL107') || $searchType == ''||$searchType=='fultit'){
    	  if(!empty($nameSubstr)){
    	    $select->condition('per.user_name', '%' . db_like($nameSubstr) . '%' , 'LIKE');
    	  }
    	  
    	  $select->orderBy('name');
	  }
	  if(!empty($modId) && ($modId =='listusers')){
	  $select->where("FIND_IN_SET(per.id,ifnull(rol.userlist,0))>0");
	  }
	  else{
	  	$select->where("FIND_IN_SET(per.id,ifnull(rol.userlist,0))=0");
	  }
	  expDebug::dPrintDBAPI('getUsernameAutocompletesearch', $select);
		$matchingUserNames = $select->execute()->fetchAll();
  	expDebug::dPrint(' $matchingUserNames = '. print_r($matchingUserNames, true),3);

  	foreach ($matchingUserNames as $userName) {
    		print $userName->name . "\n";
  		}
		}catch (Exception $ex) {
    watchdog_exception('getUsernameAutocompletesearch', $ex);
    expertusErrorThrow($ex);
  }   
}

function loadCtoolDataGridTypeForRole($mode, $type, $searchKeyword, $entityId, $entityType, $excludedId){
	try {
  expDebug::dPrint('RoleAddUsers ---> 333 '.$entityId ." ------ ".$mode);
 // die('RoleAddUsers ---> 333 '.$entityId ." ------ ".$mode);
  //include_once(drupal_get_path('module', 'exp_sp_administration_role') .'/exp_sp_administration_role.inc');
  $page      = $_POST['page']; 
  $limit     = $_POST['rows']; 
  $sidx      = $_POST['sidx'];
  $sord      = $_POST['sord'];
  $tab       = $_POST['tab'];
  $uniqueId  =  "$type-$entityId-$entityType";
  
  $queryRequired = 1;
  $fetchFields = array();
  
  $mode = empty($mode) ? 'view' : $mode;
  $countAvailable = 0;
  $count = 0;
  $searchKeyword= str_replace('|','/',$searchKeyword);
  if($mode == 'view'){

  	$listquery = fetchUserListToRole($entityId,$searchKeyword);
  }else{
  	$listquery = fetchUserListToRole($entityId,$searchKeyword);
  }
  if($sidx !=''){
  	if($sidx == "Status"){
  		$listquery->orderBy("pli.name",$sord);
  	}else if($sidx == "user_name" || $sidx == "full_name") {
  		$listquery->orderBy("pre.".$sidx,$sord);
  	}
  }
   
  if($countAvailable == 0){
    $count = count($listquery->execute()->fetchAll());
  }
expDebug::dPrint('RoleAddUsers ---> 111'.$entityId ." ------ ".$mode);
  /* Paging Related */
  $start = $limit * $page - $limit;
  $listquery->range($start, $limit);
expDebug::dPrint('RoleAddUsers ---> 222'.$listquery);
  // Execute query
  $queryResult = $listquery->execute()->fetchAll();
  expDebug::dPrint('RoleAddUsers ---> 333'.$entityId ." ------ ".$mode);
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
expDebug::dPrint('RoleAddUsers ---> 4444'.$entityId ." ------ ".$mode);
  $srch_result_response_roster->total   = $total_pages;
  $srch_result_response_roster->records = $count;
  $srch_result_response_roster->page    = $page;
  $srch_result_response_roster->unique_id    = $uniqueId;
  $srch_result_response_roster->msg    = '';
  
  $_SESSION['jq_count'] = array();
  $i=0;

expDebug::dPrint('RoleAddUsers ---> 555'.print_r($queryResult,true));
       // $userStatistics = fetchUserDetailList($listquery);
        expDebug::dPrint('grid data RoleAddUsers  ---> '.print_r($listquery,true),4);
    if($count>0){
        foreach ($queryResult as $rec_search_results){
        	$rec_search_results-> status = ($rec_search_results-> status == 'Suspended') ? 'Suspend': $rec_search_results-> status;
          $srch_result_response_roster->rows[$i]['id']   = $i;
          $srch_result_response_roster->rows[$i]['cell'] = array(
          	 'id' 	              => $rec_search_results -> user_id,
          	 'user_name' 	        => $rec_search_results-> uname,
             'full_name' 	        => $rec_search_results-> fname,
             'Status'             => $rec_search_results-> status,
             'Remove'             => $rec_search_results-> user_id,
          );
          $i++;  
        }  

    }else{
    	$srch_result_response_roster->msg  = "No Records Found";
    	/*$form = array(); 
			$form['#prefix'] = '<div id="security-role-detail-wrapper-list">';
			$form['#suffix'] = '</div>';
			$setminHight =  'admin-add-users-tab-datagrid-wrapper';
			$form['edit-class-details-start'] = array(
	  		'#prefix' => "<div id='add-user-role-page'>",
			);
			$form['add-users-button'] = array(
          '#type' => 'button',
     //     '#prefix' => '<div id="attach_user'.$qtipIdInit.'" onclick ="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'add\', \'RoleDisplayUsers\', \'\', \''.$role_id.'\', \''.$entityType.'\', \''.$excludedRoleId.'\'); return false;" 
       //   							class="admin-save-button-container add-class-button"><div class="narrow-search-actionbar-orange-btnLeft"></div><div class="narrow-search-actionbar-orange-btnBG">',
       
       //   '#suffix' => t("Add Users").'</div><div class="narrow-search-actionbar-orange-btnRight"></div></div>',
          '#prefix' => '<div id="attach_user_list" class="admin-save-button-container add-class-button"><div class="narrow-search-actionbar-orange-btnLeft"></div><div class="narrow-search-actionbar-orange-btnBG">',
	        '#suffix' => '</div><div class="narrow-search-actionbar-orange-btnRight"></div></div>',
          '#value' => t("Add Users"),
          '#name' => "Add Users",
          '#limit_validation_errors' => array(),
          '#attributes' => $addAttributes,
	 				'#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => 'attachUsersDetails',
	        'role_id' => $role_id,
	        'entity_type' => $entityType,
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
		     $builtForm              = drupal_build_form($form, $form_state);
  		 $renderedTabContentHTML = drupal_render($builtForm);   
  		 $drupalSettings         = getDrupalSettings();

		   $response       = array(
		    	'drupal_settings'     => $drupalSettings,
		    	'render_content_main' => $renderedTabContentHTML,
		    ); 
 
     		echo drupal_json_output($response);*/
    }
    expDebug::dPrint("ROLE USER LIST ::: ".print_r($srch_result_response_roster,true),5);
    echo drupal_json_encode($srch_result_response_roster);
    } catch (Exception $ex) {
    	watchdog_exception('loadCtoolDataGridTypeForRole', $ex);
    	expertusErrorThrow($ex);
    }
}

function fetchUserListToRole($roleId,$searchKeyword){
	try {
	   $userId = array(1,2);
	   $select = db_select('slt_person','pre');
	   $select->addField('pre','id','user_id');
	   $select->addField('pre','user_name','uname');
	   $select->addField('pre','full_name','fname');
	   $select->addField('pli','name','status');
	   $select->join('slt_profile_list_items','pli','pre.status=pli.code');
	   $select->condition('pre.id',$userId,'not in');
	   $select->where("FIND_IN_SET(pre.id,(select ifnull(r.userlist,0) from slt_security_role r where id=$roleId))>0");
		 if((!empty($searchKeyword)) && ($searchKeyword != 'undefined') && $searchKeyword !=t('LBL181') && $searchKeyword !=t('Type Full Name')){
     		$select->condition(db_or()->condition('pre.user_name', '%' . db_like($searchKeyword) . '%' ,'like')
  	                             		->condition('pre.full_name', '%' . db_like($searchKeyword) . '%' ,'like'));
      }
	   expDebug::dPrintDBAPI('list of users in role' ,   $select);
	   
	   return $select;
	   } catch (Exception $ex) {
	   	watchdog_exception('fetchUserListToRole', $ex);
	   	expertusErrorThrow($ex);
	   }
}

function deleteUserRole($roleId,$entityType,$userId){
	try {
		expDebug::dPrint('User deluuu');  
		$existingUsersList=getRoleUserListField($roleId);
		$userDelete=db_query('UPDATE slt_security_role SET userlist = TRIM(BOTH \',\' FROM REPLACE(
                          CONCAT(\',\', userlist, \',\'),CONCAT(\',\', '.$userId.', \',\'), \',\'))WHERE id = '.$roleId.'');                                                                     
    expDebug::dPrintDBAPI('User delete query',$userDelete);    
    $userDelete->execute();
    
    //Entry for securitymanagementAdmin
     	$select = db_select('role','role');
  		$select->join('slt_security_role','srole','role.name=srole.name');
    	$select->addField('role','rid','rid');
	    $select->condition('srole.id',$roleId,'=');
	    expDebug::dPrintDBAPI('Fetch role Id ' , $select);
	    $role_id = $select->execute()->fetchField();
	    if(!empty($userId)){
		    	$results = db_select('users','usr');
	  			$results->join('slt_person','per','usr.name=per.user_name');
	  			$results->addField('usr','uid','uid');
	  			$results->condition('per.id',$userId,'=');
	  			$usersId = $results->execute()->fetchField();
	  			
	  			$select1 = db_delete('users_roles');
	        $select1->condition('uid',$usersId);
	        $select1->condition('rid',$role_id);
	        expDebug::dPrintDBAPI('delete userlist ' , $select1);
	        $select1->execute();
	        /* slt_person entry for Instructor and manager*/
	    	 	if($roleId == 3){
		        $updatesmt = db_update('slt_person');    
	      		$updatesmt->condition('id', $userId, '=');
	      		$updatesmt->fields(array(
	                        'is_instructor' => 'N'
	                        ));
	      		expDebug::dPrintDBAPI('QUERY DEFAULT slt_person instructor---> ', $updatesmt);               
	     			$updatesmt->execute();
	        }
	        if($roleId == 2){
	        	$updatesmt = db_update('slt_person');    
	      		$updatesmt->condition('id', $userId, '=');
	      		$updatesmt->fields(array(
	                        'is_manager' => 'N'
	                        ));
	      		expDebug::dPrintDBAPI('QUERY DEFAULT slt_person manager---> ', $updatesmt);               
	     			$updatesmt->execute();
	        }
	    }
    //Audit Trail Entry
   	expDebug::dPrint('existing list'.$existingUsersList);
    $entityType = 'cre_sec';
   	$updatedUsersList['user_list'] = getRoleUserListField($roleId);
    $existUsersList[0]->user_list = $existingUsersList;
    expDebug::dPrint('existing list'.print_r($existUsersList,true));
    make_audit_trail_entries($updatedUsersList, getIdOfLoggedInUser() , $existUsersList ,$roleId, 'deleteandUpdated', $entityType, 'exp_sp_administration_role',
            	                                 'UpdatedUserlist', array('user_list'));
		return $userDelete;
		} catch (Exception $ex) {
			watchdog_exception('deleteUserRole', $ex);
			expertusErrorThrow($ex);
		}
}

function getpermissiondetails($entityId){
	try {
		$select = db_select('slt_security_role','role');	
		$select->condition('id', $entityId, '=');
		$select->addField('role','permissionlist','permli');
		$select->addField('role','accesspermission','acces');
		expDebug::dPrintDBAPI('getMappedPermissionMapping', $select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('getpermissiondetails' .   print_r($result,true));
		return $result;
		} catch (Exception $ex) {
			watchdog_exception('getpermissiondetails', $ex);
			expertusErrorThrow($ex);
		}
}

function roleNameDuplicatecheck($role_name,$roleId) {
	try {
  $select = db_select ('slt_security_role');
  $select->condition('id',$roleId,'!=');
  $select->condition('name',$role_name,'=');
  $select->condition('status','cre_sec_sts_del','!=');
  $select->addExpression('count(1)');
  expDebug::dPrintDBAPI('roleNameDuplicatecheck' , $select);
  $result = $select->execute()->fetchField();
	return $result;
	} catch (Exception $ex) {
		watchdog_exception('roleNameDuplicatecheck', $ex);
		expertusErrorThrow($ex);
	}
}

function getRoleUsereName($userlist){
	 if(empty($userlist)){
    return '';
  }
  $userlistArray = explode(',',$userlist);
  try{
  $i=0;	
  foreach($userlistArray as $key){
  	$select = db_select('slt_person', 'per');
    $select->addField('per','user_name', 'uname');
    $select->condition('id', $key , '=');
    expDebug::dPrintDBAPI('$userlistArray' , $select);
    $userlistArray[$i]=$select->execute()->fetchField();
    $i++;
  }
  $userlistval = implode(', ',$userlistArray);
  expDebug::dPrint('getRoleUsereName' .   print_r($userlistval,true));
  return $userlistval;
  }catch (Exception $ex) {
    watchdog_exception('getRoleUsereName', $ex);
    expertusErrorThrow($ex);
  }
}
?>