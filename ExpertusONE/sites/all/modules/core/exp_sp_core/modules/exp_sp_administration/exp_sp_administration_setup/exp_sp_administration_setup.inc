<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getModuleInfoTabContentType() . 
 */
 
function getSetUpTabContentType(){
	try {
		$jsObjectInfo['tab_content_type'] = 'narrow_search';
		return $jsObjectInfo;
	} catch (Exception $ex) {
		watchdog_exception('getSetUpTabContentType', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * commerceSettingNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function setUpNarrowSearchFiltersHTML() {
	try {
		$renderfiltersets=array();
		$narrowfilters =  array();
		$filterSetforJS=$narrowfilters;

		return theme('narrow_search_filters_holder',
				array('rendered_filtersets_list'  => $renderfiltersets,
					   'filter_set_list' 		   => drupal_json_encode($filterSetforJS),
						'enable_option'            => false,
				));
	} catch (Exception $ex) {
    watchdog_exception('setUpNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }   
}

/*
 * moduleInfoSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function setUpSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'module-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'module-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'module-sort-new');
    
  $defaultSortOrder = array('last_sort_type'=>'NewlyListed',
  					                   'last_sort_type_html_id'=>'module-sort-new'
                              );
  
  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=>drupal_json_encode($defaultSortOrder), // Currently being ignored
                                              'title' => t('LBL011'),
                                              'sortbar_enable' => false,   
                                              )
              );
   
} catch (Exception $ex) {
	watchdog_exception('setUpSearchResultsSortbarHTML', $ex);
	expertusErrorThrow($ex);
}
} 

/*
 * moduleInfoSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function setUpSearchResultsActionbarHTML() {
 try {  
  $actionbarLinksListforJS = array('export_path' => 'administration/sitesetup/moduleinfo/export/',
  					                       'print_path' => 'administration/sitesetup/moduleinfo/print/'
                                  );   
  return theme('narrow_search_actionbar', array( 'add_button_title'   => '',
												                         'add_page_path'      => '',
												                         'show_print_button'  => false,
												                         'show_export_button' => false,
												                         'show_add_button'    => false,
                                        		     'ctools_style'       => '',
                                                 'actionbar_list'     => drupal_json_encode($actionbarLinksListforJS),                                       
                                           )
               );
} catch (Exception $ex) {
	watchdog_exception('setUpSearchResultsActionbarHTML', $ex);
	expertusErrorThrow($ex);
}
}

/*
 * getModuleInfoAdminTabContentMainHTML - Returns the html for tab Module settings
 */
function getSetUpAdminTabContentMainHTML() {     
	try {
   $narrowsearchTabModuleListforJS = array(
                                            'search_base_path' =>'administration/people/setup/search/all',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/people/setup/textfilter-autocomplete'                                            
                                          );                               
  
   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabModuleListforJS))
                              );
} catch (Exception $ex) {
	watchdog_exception('getSetUpAdminTabContentMainHTML', $ex);
	expertusErrorThrow($ex);
}
}
 
/*
 * getModuleTabContent		  - Returns the html for narrow search block on mdule search page
 */
function getSetUpTabContent() {
	try {
  $tabContentTypeArr = getSetUpTabContentType();
       
  $moduleResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => setUpNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => setUpSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => setUpSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getSetUpAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','people'), 
  ); 
  echo drupal_json_encode($moduleResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getSetUpTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getModuleInfoDetailsHTML() - Renders a module detail as HTML to be displayed in jqGrid.  
 */
function getSetUpDetailsHTML($listValues) { 
 try {
  $detailsList = array();
 
  $actionList[] = array('action_link_text' => t('LBL063'), //Edit
  					    'action_page_path' => $listValues->edit_path, //path',
					    'action_button_params' => '',//$module->expertus_module_name,
                        //'action_module_status' => $module->status,
                        'action_button_param_text' => '',
  					    'ctools_style' => 'ctools-modal-ctools-admin-setup-addedit-wrapper',
  					     'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                        'tooltip' => t('LBL1023')
					        );
					 
  return theme('narrow-search-results-item-details',
                 array('title' => trim($listValues->name),                        
                       'details_list' => $detailsList,
                       'description'=> trim($listValues->display_description),
                       'delete_info' => t('MSG357').' '.strtolower(t('Tax').' '.t('MSG616')),
                 	   'action_list'=> $actionList
                      )
                );
} catch (Exception $ex) {
	watchdog_exception('getSetUpDetailsHTML', $ex);
	expertusErrorThrow($ex);
}
}

function sendSetUpListToJqGrid() {
 try { 
  $page = $_GET['page']; // get the requested page 
  $limit = $_GET['rows'];
  $page_type = arg(1);
  if($page_type == 'people') {
    $code = 'cre_usr';
  }

  /* $numListValues = fetchAllListValues($code,'COUNT');
   
  expDebug::dPrint(' $$$numListValues = ' . print_r($numListValues, true) , 3);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numListValues);
   */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  $userListValues = fetchAllListValues($code,'LISTS',  $start, $limit);
  expDebug::dPrint(' $userListValues list = ' . print_r($userListValues, true) , 3);  
  
  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numListValues; 
                                             // have to do this.
  
  $i=0;
  foreach ($userListValues as $listValues) { 
    
    $otherValues = getListValuesDetails($listValues->code);
    expDebug::dPrint('$listValues = '. print_r($otherValues, true) , 3);
    $listValues->imagepath = $otherValues['imageClass'];
    $listValues->edit_path = $otherValues['edit_path'];
    $listValues->display_description = $otherValues['display_description'];
    $listValues->object_type = 'listvalues';

    $listValues->row =  $i;
    $response->rows[$i]['id']   = $listValues->id;
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $listValues)),
    									 'details' => getSetUpDetailsHTML($listValues)
                                         );
    $i++;    
  }
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendSetUpListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchAllListValues() - Fetches list values / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchAllListValues($code=NULL,$op=NULL,$start=NULL, $limit=NULL){
  try {
// Select table
	$userId = getIdOfLoggedInUser();
  $select = db_select('slt_profile_list_items', 'spli');
  $select->leftJoin('slt_groups','grp','grp.is_admin = 1 AND FIND_IN_SET('.$userId.',userslist) > 0');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = grp.id');
  $select->condition('spli.code', '%'.$code.'_%','LIKE');
  $select->condition('spli.code', array('cre_usr_gtp','cre_usr_sts'),'NOT IN');
  $select->where('if('.$userId.'= 1,1=1,spli.code = priv.page_code)');
  $select->where('spli.parent_id = (select id from slt_profile_list_items where code = \'cre_usr\')');
  $select->orderBy('spli.name','ASC');

  if($op=='COUNT'){
     $select->addExpression('COUNT(DISTINCT(spli.id))', 'count');
  }
  else{
	  // Select fields/expression values to be fetched from the database.
	$select->addField('spli','id','id');
	$select->addField('spli', 'name', 'name');
	$select->addField('spli', 'code', 'code');
	$select->addField('spli', 'lang_code', 'lang_code');
	$select->addField('spli', 'parent_id', 'parentId');
	$select->addField('spli', 'is_active', 'status');
	$select->groupBy('spli.id');
  }
  
  selectAddNarrowSearchByTextMultipleORCondition($select, "spli", array("name"), "textfilter");
  filterByUserAccessPrivileges($select, 'spli','',1,'','people_setting');
  expDebug::dPrintDBAPI('$select', $select);  
    
  if($op == "COUNT"){  
  	$result = $select->execute()->fetchField();  
	  expDebug::dPrint('COUNT $result = ' . $result ,3);
	return $result;
  }
  else{
    // Limit to $limit records 
  	add_row_id_condition($select, 'spli');
	$select->range($start, $limit);
	expDebug::dPrintDBAPI('$select with range set', $select);
	// Execure the query and fetch all the records
	$result = $select->execute()->fetchAll();
	expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	return $result;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchAllListValues', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 *  Getting the ctools and image icon class fro the list values
 */
function getListValuesDetails($code) {
	try {
	global $theme_key;
  $detailsText = array();
  if($code == 'cre_usr_dpt') {
  	$depIconCls = ($theme_key == 'expertusoneV2') ? 'department admin-icon' : 'department crs-icon';
  	$detailsText['ctoolstyle'] = '';
    $detailsText['imageClass'] = $depIconCls;
    $detailsText['edit_path'] = 'administration/people/setup/Department/cre_usr_dpt/nojs/addedit';
    $detailsText['display_description'] = t('MSG675');
  }
  else if($code == 'cre_usr_etp') {
  	$empIconCls = ($theme_key == 'expertusoneV2') ? 'emp-type admin-icon' : 'emp-type crs-icon';
  	$detailsText['ctoolstyle'] = '';
    $detailsText['imageClass'] = $empIconCls;
    $detailsText['edit_path'] = 'administration/people/setup/Employment Type/cre_usr_etp/nojs/addedit';
    $detailsText['display_description'] = t('MSG676');
  }
  else if($code == 'cre_usr_jrl') {
  	$roleIconCls = ($theme_key == 'expertusoneV2') ? 'job-role admin-icon' : 'job-role crs-icon';
  	$detailsText['ctoolstyle'] = '';
    $detailsText['imageClass'] = $roleIconCls;
    $detailsText['edit_path'] = 'administration/people/setup/Job Role/cre_usr_jrl/nojs/addedit';
    $detailsText['display_description'] = t('MSG677');
  }
  else if($code == 'cre_usr_jtl') {
  	$jobIconCls = ($theme_key == 'expertusoneV2') ? 'job-title admin-icon' : 'job-title crs-icon';
  	$detailsText['ctoolstyle'] = '';
    $detailsText['imageClass'] = $jobIconCls;
    $detailsText['edit_path'] = 'administration/people/setup/Job Title/cre_usr_jtl/nojs/addedit';
    $detailsText['display_description'] = t('MSG678');
  }
  else if($code == 'cre_usr_ptp') {
  	$upIconCls = ($theme_key == 'expertusoneV2') ? 'user-type admin-icon' : 'user-type crs-icon';
  	$detailsText['ctoolstyle'] = '';
    $detailsText['imageClass'] = $upIconCls;
    $detailsText['edit_path'] = 'administration/people/setup/User Type/cre_usr_ptp/nojs/addedit';
    $detailsText['display_description'] = t('MSG679');
  }
  return $detailsText;
  } catch (Exception $ex) {
  	watchdog_exception('getListValuesDetails', $ex);
  	expertusErrorThrow($ex);
  }
}
function getListValueSetUpDetailList($entityType,$type,$start = 0,$limit = 0,$searchText='', $profileListName='') {
	try {
		$select = db_select ( 'slt_profile_list_items', 'spli' );
		$select->condition ( 'spli.code', '%' . $entityType . '_%', 'LIKE' );
		if (! empty ( $profileListName )) {
			$select->condition ( 'spli.code', '%' . $profileListName . '%', 'LIKE' );
		}
		if (! empty ( $searchText ))
			$select->condition ( 'spli.name', '%' . db_like ( $searchText ) . '%', 'LIKE' );
		$select->condition ( 'spli.is_active', 'Y', '=' );
		// filterByUserAccessPrivileges($select, 'spli','',1,rtrim($entityType, "_"),'','spli');
		if ($type == 'COUNT') {
			// $select->addExpression('COUNT(1)','id');
			$select->addExpression ( 'COUNT(DISTINCT(spli.id))', 'count' );
			filterByUserAccessPrivileges($select, 'spli','',1,$entityType,'user_attribute','spli');
			expDebug::dPrintDBAPI ( "count list query ---->", $select );
			$result = $select->execute ()->fetchField ();
		} else {
			$select->addField ( 'spli', 'id' );
			$select->addField ( 'spli', 'code' );
			$select->addField ( 'spli', 'name' );
			filterByUserAccessPrivileges($select, 'spli','',1,$entityType,'user_attribute','spli');
			$select->groupBy ( 'spli.id' );
			$totalRecords = $select->countQuery ()->execute ()->fetchField ();
			if (($start > 0) || ($limit > 0)) {
				$select->range ( $start, $limit );
			}
			expDebug::dPrintDBAPI ( 'query for showing list values', $select );
			$result = $select->execute ()->fetchAll ();
			$result ['totalrow'] = $totalRecords;
		}
		expDebug::dPrintDBAPI ( 'query for showing list values', $select );
		return $result;
	} catch ( Exception $ex ) {
		watchdog_exception ( 'getListValueSetUpDetailList', $ex );
		expertusErrorThrow ( $ex );
	}
}

function chkValuesMappedtoUserData($listCode, $entityId)
{
	try {
		if($listCode == "cre_usr_dpt" || $listCode == "cre_usr_jtl" || $listCode == "cre_usr_etp" || $listCode == "cre_usr_ptp" || $listCode == "cre_usr_jrl")
		{
			$select = db_select('slt_profile_list_items','spli');
			if($listCode == "cre_usr_jtl")
				$select->innerJoin('slt_person','sp','sp.job_title=spli.code');
			elseif($listCode == "cre_usr_dpt")
				$select->innerJoin('slt_person','sp','sp.dept_code=spli.code');
			elseif($listCode == "cre_usr_etp")
				$select->innerJoin('slt_person','sp','sp.employment_type=spli.code');
			elseif($listCode == "cre_usr_ptp")
				$select->innerJoin('slt_person','sp','sp.user_type=spli.code');
			elseif($listCode == "cre_usr_jrl")
			{
				$select->innerJoin('slt_person_jobrole_mapping','spjm','spjm.job_role=spli.code');
				$select->innerJoin('slt_person','sp','sp.id=spjm.user_id');
			}
			
			$select->condition('spli.id', $entityId,'=');
			
			$select->addExpression('COUNT(spli.id)','id');
			$result = $select->execute()->fetchField();
			
			expDebug::dPrintDBAPI('query for showing list values',$select);
			return $result;
		}
		else
			return 0;
	} catch (Exception $ex) {
		watchdog_exception('chkValuesMappedtoUserData', $ex);
		expertusErrorThrow($ex);
	}
}
function addListValuesDetails($code,$entityId,$entityType){
  try {
  	 $display_mode = ($entityId) ? 'edit': 'add';
     $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'code'  => $code,
       'list_id'    => $entityId,
       'list_code'  => $entityType,
       'display_mode' => $display_mode
       //'list_count' => $listCount
     );      
      
      $formId                 = 'exp_sp_administration_setup_addedit_list_form';
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
    watchdog_exception('addListValuesDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function getSetUpListValuesTextFilterAutoComplete() {
	try {
		$userId = getIdOfLoggedInUser();
  $searchText =  $_GET['z'];
  // Select the table
  $select = db_select('slt_profile_list_items', 'proflist');
  $select->leftJoin('slt_groups','grp','grp.is_admin = 1 AND FIND_IN_SET('.$userId.',userslist) > 0');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = grp.id');
  // Add fields to fetch
  $select->addField('proflist', 'name', 'name');
  // Add condition
  $select->condition('proflist.name', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('proflist.code','cre_usr_%','LIKE');
  $select->where('if('.$userId.'= 1,1=1,proflist.code = priv.page_code)');
  $select->where('proflist.parent_id = (select id from slt_profile_list_items where code = \'cre_usr\')');
 // filterByUserAccessPrivileges($select, 'proflist','',1,'','people_setting','proflist');
   filterByUserAccessPrivileges($select, 'proflist','',1,'','people_setting','proflist');
  // Order in dictionary sort order
  $select->orderBy('name');
  expDebug::dPrintDBAPI("getSetUpListValuesTextFilterAutoComplete --- >",$select);
  $matchingListNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $$matchingListNames = '. print_r($matchingListNames, true) , 3);
  foreach ($matchingListNames as $listName) {
    print rawurlencode($listName->name) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getSetUpListValuesTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * getting the values of list values to display with in the popup
 */
function getListValuesSetUpDetails($id,$code) {
	try {
  $selectStmt = db_select('slt_profile_list_items','spli');
  $selectStmt->addField('spli','code');
  $selectStmt->addField('spli','name');
  $selectStmt->condition('spli.id',$id,'=');
  $selectStmt->condition('spli.code',$code,'=');
  $selectStmt->condition('spli.is_active','Y','=');
  $result = $selectStmt->execute()->fetchAssoc();
  expDebug::dPrint('getListValuesSetUpDetails'.print_r($result,true));
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getListValuesSetUpDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function clearUserGrpAccess($listValueId, $entityType)
{
	try {
		expDebug::dPrint('clearUserGrpAccess parameters : '.print_r(array($listValueId, $entityType),true));
		$loginUserId = getIdOfLoggedInUser();
		$AccArr = array('cre_usr_dpt','cre_usr_etp','cre_usr_jrl','cre_usr_jtl','cre_usr_ptp');
		if((empty($listValueId) || $listValueId == 0) && (in_array($entityType, $AccArr)) && $loginUserId > 0)
		{
			$deleteUserGrp = db_delete('slt_group_mapping')
			->condition('entity_type', $entityType)
			->condition('created_by', $loginUserId)
			->condition('entity_id', 0)
			->execute();
			
			expDebug::dPrintDBAPI('$deleteUserGrp = ' , $deleteUserGrp);
		}
	} catch (Exception $ex) {
	  	watchdog_exception('clearUserGrpAccess', $ex);
	  	expertusErrorThrow($ex);
	}
	
}
function exp_sp_administration_setup_addedit_list_form($form,&$form_state){
	try {
 	expDebug::dPrint('values to be printed for list values'.print_r($form_state,true));
	$wrapperId = 'addedit-listvalues-detail-wrapper';
	$listValueId = $form_state['list_id'];
	$listTypeCode = $form_state['list_code'];
	$entityType = $form_state['code'];
	$displayMode = 'edit';
 	$form_state['display_mode'] = $displayMode;
	$form = array(); 
	
	clearUserGrpAccess($listValueId, $entityType);
	
	if(!empty($listValueId)) {
      $listDetails = getListValuesSetUpDetails($listValueId,$listTypeCode);
    }else{
    	$emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
    }
    $form = newAddEditFormWithCancelAndSave($wrapperId);
    $defaultValue =  getDefaultAddEditFormFieldValue('name', $form_state, $listDetails);
    addTextFieldToAddEditForm($form, 'name', t('LBL107'), $defaultValue, 1, true,'', 256); //0054796
    
    addEditFormWithListValuesCancelAndSaveClose($form, $wrapperId,2,'listvalues-save-button','listvalues-cancel-button',$listValueId,$emptyId,$entityType);  
    
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_setup_addedit_list_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function addEditFormWithListValuesCancelAndSaveClose(&$form, $wrapperId = 'userpoints-admin-form', $tabIndex,$saveButtonId = 'save-button', $cancelButtonId = 'cancel-button',$listid,$emptyId = '',$entityType) {
  global $theme_key;
  try {
    // Button ids
    $saveButtonId = 'listvalues-save-button';
    $cancelButtonId = 'listvalues-cancel-button';
    $esignHiddenButt = 'esign-' . $wrapperId;

    // Add the Cancel button
    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

    // Using this class to applying bg color to the save button
    $cancelAttributes['class'][] = 'admin-action-button-middle-bg';

    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $cancelAttributes['data-wrapperid'] = array($wrapperId);

    // A common class for all addedit form cancel button, for styling to appear as a link.
    $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
    //$cancelAttributes['onclick'][] = "closeQtip('','$wrapperId');  return false;";
    expDebug::dPrint('arg--->'.arg(5));
    // 36850: Code Re-Factoring (31. People > Settings > Department > Add Another popup - Close button is not working. This issue exists in all the sub pages of Settings tab. )
		/* if(arg(5)!='')
			$cancelAttributes['onclick'][] = 'closeQtip("","'.$wrapperId.'"); $(".page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues").removeAttr("style");return false;';
	  else */
			$cancelAttributes['onclick'][] = '$(".qtip-active").remove();$(".page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues").removeAttr("style");return false;'; 
	  
    $cancelAttributes['tabindex'][] = $tabIndex + 1;
    if($theme_key == 'expertusoneV2') {
      $prefix = '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">'.
                        '<div class="white-btn-bg-container"><div class="white-btn-bg-left"></div><div class="white-btn-bg-middle">';
    
      $suffix = '</div><div class="white-btn-bg-right"></div></div>';
    }else {
      $prefix = '<div style="clear:both;"></div>' .
                   '<div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions">';
    
      $suffix = '';
    }

    
    $entityId          	 = $listid;
    if(!empty($emptyId)){
    	$entityId = 0;
    } else {
    	$emptyId = 0;
    }

     	$qtipIdInit        	 = $entityId.'_'.$entityType;
     	$qtipOptAccessObjacc      = "{'entityId':'$qtipIdInit','entityType':'$entityType',
     	'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."',
     	'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
     	'wid':650,'heg':'270','postype':'middle','poslwid':'40','qdis':'',
     	'linkid':'visible-attribute-".$entityId."'}";
     	
     	$form['addedit-left-col-extend-func_acc'] = array(
     	'#type' => 'markup',
     	'#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' ,
     	'#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
     	"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-attribute-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObjacc.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$qtipIdInit."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
     			'</div>',
     	);

    
    $form[$cancelButtonId] = array(
      '#type' => 'button',
      '#prefix' => $prefix,
    // Suffix to close the two divs is added in #suffix property of Save button
      '#suffix' => $suffix,
      '#value' => ($theme_key == 'expertusoneV2') ? t('LBL109') : t('LBL123') , //Close
      '#name' => 'cancel',
      '#attributes' => $cancelAttributes,
    );

    // Add the Save button
    $ajaxCallbackSave = 'addListValuesCallBack';

    // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
    // each time the form is re-built.
    $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

    // Using this class to applying bg color to the save button
    $saveAttributes['class'][] = 'admin-save-button-middle-bg';
    $saveAttributes['tabindex'][] = $tabIndex;
    // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
    $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
    // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
    $saveAttributes['data-wrapperid'] = array('addedit-listvalues-detail-wrapper');
   
    $eSignatureAvailable = true;

    if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
      $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#value' => t('LBL141'), // Save
        '#name' => 'save',
        '#prefix' => '<div class="admin-save-button-container">' .
                       '<div class="admin-save-button-left-bg"></div>',
        '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                     '</div>' .
                     '<div class="clearBoth"></div>' .
                   '</div>' . // see #prefix of close button above
                   '<div class="clearBoth"></div>' .
                 '</div>', // see #prefix of close button above
        '#id' => $saveButtonId,
        '#attributes' => $saveAttributes,
        '#ajax' => array(
          'wrapper' => 'list-values-detail-wrapper',
          'callback' => $ajaxCallbackSave,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=> 'click',
          'drupalUserId' => 'drupal_uid',
          'customButtonId'=> $esignHiddenButt,
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
            ),
            )
            );
    }
    else {
      $saveAttributes['onclick'] = array();     
      /*-- close tip does not show validation message on error. so, old code reverted --*/
      //$saveAttributes['onclick'][] = '$("#' . $esignHiddenButt . '").click();';
      $saveAttributes['onclick'][] = '$("#' . $esignHiddenButt . '").click(); return false;';
      /*if(arg(5)!='')
      	$saveAttributes['onclick'][] = "closeQtip('','$wrapperId');  return false;";
      else
      	$saveAttributes['onclick'][] = '$(".qtip-active").remove();  return false;';*/
      $form[$saveButtonId] = array(
        '#type' => 'button',
        '#prefix' => '<div class="admin-save-button-container">' .
                       '<div class="admin-save-button-left-bg"></div>',
        '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                     '</div>' .
                     '<div class="clearBoth"></div>' .
                   '</div>' . // see #prefix of close button above
                   '<div class="clearBoth"></div>' .
                 '</div>', // see #prefix of close button above
        '#value' => t('LBL141'), // Save
        '#name' => 'save',
        '#attributes' => $saveAttributes,
        /*'#ajax' => array(
          'wrapper' => 'list-values-detail-wrapper',
          'callback' => $ajaxCallbackSave,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=> 'click',
          'drupalUserId' => 'drupal_uid',
          'customButtonId'=> $esignHiddenButt,
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
            ),
            )*/
            );
    }

    unset($saveAttributes['tabindex']);
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
      '#type' => 'submit',
      '#value' => t('hidden-Save'),
      '#name' => 'hidden-Save',
      '#prefix' => '<span id = "esign-admin-div" style="display:none">',
      '#suffix' => '</span>',
      '#id' =>$esignHiddenButt,
      '#attributes' => $saveAttributes,
      '#ajax' => array(
        'wrapper' => 'list-values-detail-wrapper',
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

          // Close form div
          $form['addedit-left-col-end'] = array(
      '#type' => 'markup',
      '#markup' =>   '</div>' ,
                   //'<div class="clearBoth"></div>',
          );
  }
  catch (Exception $ex) {
    watchdog_exception('addEditFormWithListValuesCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * exp_sp_administration_userpoints_addedit_form_validate : validate function for user points
 */
function exp_sp_administration_setup_addedit_list_form_validate($form, &$form_state) {
  try {
    expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
    expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state, true), 5);
    //0042093: Fileds needs to be trimmed
    $form_state['values']['name'] = trim(decode_entities($form_state['values']['name']));
    $form_state['hidden-Save-submit'] = false;
    $form_state['esign_show_popup']= false;
    $form_state['storage']['success-status']= true;
     
   /* if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
      $validationTrigger  = 'save';
    }
    else{*/
      $validationTrigger  = 'hidden-Save';
   //}

    if ($form_state['triggering_element']['#name'] == $validationTrigger) {
      $form_state['esign_show_popup']= true;
      if(empty($form_state['values']['name']) || $form_state['values']['name'] == '') {
        expDebug::dPrint('values of the form state for form set error'.$form_state['values']['name']);
        form_set_error('name', t('LBL107')." ".t('ERR101'));
        $form_state['esign_show_popup']= false;
        $form_state['storage']['success-status']= false;
      }else{
          $new_name = expertus_xss_validate_security($form_state['values']['name']);
          if($new_name != $form_state['values']['name']){
              expDebug::dPrint('Inside form set error ');
              form_set_error('name',t('LBL1273'));
              $form_state['esign_show_popup']= false;
            $form_state['storage']['success-status']= false;
          }
      }
      
      // /*--  #40125: Need alert message - Check the duplication entry --*/
     //  if ($form_state['values']['name'] != '' && $form_state['storage']['warn-msg'] != 1 ) { #61257
      if ($form_state['values']['name'] != '') {
        //0042093: Fileds needs to be trimmed
        //0042387: Issue in people ->setting attributes
        $checkDuplicate = checkDuplicateEntity($form_state['values']['name'], $form_state['list_code'],$form_state['list_id']);
        if ($checkDuplicate) { 
         form_set_error('name',t('LBL949'));
         $form_state['esign_show_popup']= false;
         $form_state['storage']['success-status']= false;
        // $form_state['storage']['warn-msg']= 1;
         } 
        
      }
      //}
    }
    
    if ($form_state['triggering_element']['#name'] == 'hidden-Save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-Save') {
      $form_state['hidden-Save-submit'] = true;
    }

    $form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
    // showing popup.
    if (!($form_state['hidden-Save-submit'])) {
      unset($_SESSION['esign_validate']); // for every button other than hidden-Save, unset $_SESSION['esign_validate'].
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_setup_addedit_list_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * exp_sp_administration_userpoints_addedit_form_submit : updating the values to the table given in the form
 */
function exp_sp_administration_setup_addedit_list_form_submit($form, &$form_state) {
//   expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state, true), 5);
//   expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);
//   expDebug::dPrint('form_state[code] ' . print_r($form_state['code'], true), 5);
  

  try {
    $eSignValidated = true;
    /*if (module_exists('exp_sp_esignature') && isActiveEsignRegion()) {
      if($form_state['hidden-Save-submit'] == true && $_SESSION['esign_validate'] ==  1) {
        $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
      }
      else {
        $eSignValidated = false;
        $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
      }
    }
    else {*/
      $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
    //}
     
    if ($eSignValidated) {
      $form_state['esign_validation_status'] = true;
      $loggedInUserId = getIdOfLoggedInUser();
      if (empty($form_state['list_id']) && $form_state['list_id']=='') { // Add
        $listId = addNewListValues($form, $form_state, $loggedInUserId);
        $form_state['build_info']['args'][] = $listId;
        $form_state['storage']['new_entity'] = TRUE;
        $form_state['storage']['display_mode'] = 'Edit';
      }
      else{
      	$checkDuplicate = checkDuplicateEntity($form_state['values']['name'],$form_state['code'],$form_state['list_id']);
		  if ($checkDuplicate) { 
         form_set_error('name',t('LBL949'));
         $form_state['esign_show_popup']= false;
         $form_state['storage']['success-status']= false;
         //$form_state['storage']['warn-msg']= 1;
         }
		 else
        updateListValues($form, $form_state, $loggedInUserId);
      }
      $form_state['values'] = array(); // no further use
      $form_state['input'] = array(); // on form rebuild show form default values to user to show data actually saved in db (e.g. trimmed values)
      $form_state['rebuild'] = true; // rebuild form
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_setup_addedit_list_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}
function addNewListValues($form, $form_state, $createdUserId, $callFromAPI =0,$dtLoad='') {
  $txn = db_transaction();
    try {  
/*    if(($callFromAPI) && ($form_state['list_code'] != ('cre_usr_dpt' || 'cre_usr_etp' || 'cre_usr_jrl' || 'cre_usr_jtl' || 'cre_usr_ptp'))) {
       return array((object)array('id'=>"failure"));
    }else {*/
      $insertStmt  	= db_insert('slt_profile_list_items');
      $nfields       	= listValuesFormatInputString($form, $form_state,$createdUserId,$dtLoad);
      $insertStmt->fields($nfields);
      expDebug::dPrintDBAPI('insertStmt', $insertStmt);
      $listId 		= $insertStmt->execute();
      
      $AccArr = array('cre_usr_dpt','cre_usr_etp','cre_usr_jrl','cre_usr_jtl','cre_usr_ptp');
      if($listId > 0 && in_array($form_state['list_code'], $AccArr))
      {
	      $updateUserGrp = db_update('slt_group_mapping')
	      ->fields(array('entity_id' => $listId, 'custom0' => null, 'custom1' => null, 'custom2' => null, 'custom3' => null, 'custom4' => null))
	      ->condition('entity_type', $form_state['list_code'])
	      ->condition('created_by', $createdUserId)
	      ->condition('entity_id', 0)
	      ->execute();
      }
      
      $insertfields = $form_state['values'];
      expDebug::dPrint('scfsdfdaryqoirqir'.$listId.'code values'.print_r($nfields,1));
      make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$listId, 'Inserted',$form_state['list_code'],'exp_sp_administration_setup','addNewListValues',
                                            array('name', 'id','lang_code'));  
      $form_state['values']['id']     = $listId;
    	if($callFromAPI){
      	return array((object)array('id'=>$listId,'code'=>$nfields['code']));
      }else if($dtLoad='DAPI'){
      	return array((object)array('status'=>'success'));
      }else{
      	return array((object)array('id'=>$listId));
      }
    //}
  }
  catch(Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewListValues', $ex);
    if($dtLoad='DAPI'){
    	return array((object)array('status'=>'failure','remarks'=>$ex->getMessage()));
  }
}
}
function updateListValues($form, &$form_state, $updateUserId, $callFromAPI =0,$dtLoad='') {
  expDebug::dPrint('$form_state values = ' . print_r($form_state, true), 4);
  $txn = db_transaction();
  try {
    /*if(($callFromAPI) && ($form_state['list_code'] != ('cre_usr_dpt' || 'cre_usr_etp' || 'cre_usr_jrl' || 'cre_usr_jtl' || 'cre_usr_ptp'))) {
      return array((object)array('id'=>"failure"));
    }else {*/
      $defaultQryStr = 'SELECT code,name,is_active from slt_profile_list_items where id = :cid';
      $defaultQryStrArgs = array(':cid' => $form_state['list_id']);
      $defaultQry = db_query($defaultQryStr, $defaultQryStrArgs);
      expDebug::dPrintDBAPI('$defaultQry', $defaultQry, $defaultQryStrArgs);
      $default = $defaultQry->fetchAll();
      expDebug::dPrint('$default = ' . print_r($default, true) );
      	
      $updateStmt = db_update('slt_profile_list_items');
      $fields     = array(
    				'name' => $form_state['values']['name'],
                    'updated_by' => $updateUserId,
                    'updated_on' => now(),
      							'custom_dataload' => ($dtLoad == 'DAPI') ? $form_state['values']['custom_dataload'] : NULL
      );
      if($dtLoad == 'DAPI'){
      	$fields['is_active'] =  $form_state['is_active'];
      }
      $fields = trimInputFields($fields); // #42124 - Trim input values
      $updateStmt->fields($fields);
      $updateStmt->condition('id', $form_state['list_id']);
      $updateStmt->condition('code', '%'.$form_state['list_code'].'%','LIKE');
      expDebug::dPrintDBAPI('$updateStmt', $updateStmt);

      // Execute the update statement.
      $numUpdated = $updateStmt->execute();
      $updatefields = $form_state['values'];
      $updateUserId = ($updateUserId == 'Feed' ) ? 1 : $updateUserId;
      make_audit_trail_entries($updatefields, $updateUserId , $default ,$form_state['list_id'], 'Updated',$form_state['list_code'],'exp_sp_administration_setup','updateListValues',
          array('name'));
    //}
      if($callFromAPI && empty($dtLoad)){
      return array((object)array('id'=>$form_state['values']['id']));
      }else if($dtLoad='DAPI' && !empty($numUpdated)){
      	return array((object)array('status'=>'success'));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateListValues', $ex);
    if($callFromAPI && empty($dtLoad)){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Updating list value";
    	return $errobj;
      //return array((object)array('id'=>'Failiure'));
    }else if($dtLoad='DAPI'){
      	return array((object)array('status'=>'failure','remarks'=>$ex->getMessage()));
    }
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);

}
function listValuesFormatInputString($form, $form_state,$createdUserId,$dtLoad='') {
	try {
  $nullVal           = NULL;
  $status = '';
  if(empty($form_state['list_id']) && empty($dtLoad)){
    
    $listCount = getListValueSetUpDetailList($form_state['list_code'],'COUNT');
    $listCodeCount = $listCount + 1;
    $list_id = explode('_',$form_state['list_code']);
    $listIdStmt = db_select('slt_profile_list','prls');
    $listIdStmt -> addField('prls','id');
    $listIdStmt-> condition('prls.code',$list_id[0],'=');
    $listIdresult = $listIdStmt->execute()->fetchField();
    
    $parentIdStmt = db_select('slt_profile_list_items','spli');
    $parentIdStmt->addField('spli','id');
    $parentIdStmt->condition('spli.code',$form_state['list_code'],'=');
    $parentIdresult = $parentIdStmt->execute()->fetchField();
    $code = getSequenceCodeForListValues($form_state['list_code'],$listCodeCount);
  }else if($dtLoad == 'DAPI'){
  	$code = $form_state["list_code"];
  	$listIdresult = $form_state['list_id'];
  	$parentIdresult = $form_state['parent_id'];
  	$status = $form_state['is_active'];
  }
  
  $fields = array(               
                'name'               => $form_state['values']['name'],
                'code'               => $code,
                'list_id'            => $listIdresult,
                'parent_id'          => $parentIdresult,
                'lang_code'          => 'cre_sys_lng_eng',
                'is_active'          => empty($status) ? 'Y' : $status,
                'created_by'		 => $createdUserId,
                'created_on'		 => now(),
  				'updated_by'		 => $createdUserId,
  				'updated_on'		 => now(),
                'custom0'            => $nullVal,
                'custom1'            => $nullVal,
                'custom2'            => $nullVal,
                'custom3'            => $nullVal,
                'custom4'            => $nullVal,  
  				'custom_dataload' => ($dtLoad == 'DAPI') ? $form_state['values']['custom_dataload'] : NULL
   );
   $fields = trimInputFields($fields); // #42124 - Trim input values
   return $fields;
   } catch (Exception $ex) {
   	watchdog_exception('listValuesFormatInputString', $ex);
   	expertusErrorThrow($ex);
   }
}

function addListValuesCallBack($form, &$form_state){
  try{
  $code = $form_state['code'];
  $storageCode = $form_state['storage']['success-status'];
  expDebug::dPrint('triggering elemnt 1 -'.print_r($form_state['storage']['success-status'],true));
  
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'code' => $code,
   'list_code' => $form_state['list_code'],
   'list_id'  => $form_state['list_id'],
   'build_info' => array('args' => array('ajax')) 
  );
  $commands = array();
  $form_state = $min_form_state;
  //$form_state['role_id'] = $roleId;
  if($storageCode==1) {
  $formId = 'exp_sp_administration_setup_addedit_form';
  $builtForm = drupal_build_form($formId, $form_state);
  /*--  #40125: Need alert message  --*/
  $entityId = ($form_state['list_id'] == '') ? 0 : $form_state['list_id'];
  $commands[] = ajax_command_invoke(NULL, "cancelPopUp", array($entityId, $form_state['list_code']));
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  }else{
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
   // Show the messages
    $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
  }
  }
  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
	}catch (Exception $ex) {
    watchdog_exception('addListValuesCallBack', $ex);
    expertusErrorThrow($ex);
  }
}
function getSequenceCodeForListValues($entityType,$actCount) {
	try {
		do {
			if($actCount < 10){
				$codeCount = '00'.$actCount;
			}else if (($actCount > 10) && ($actCount < 99)) {
				$codeCount = '0'.$actCount;
			}else{
				$codeCount = $actCount;
			}
			$codeExist = $entityType.'_'.$codeCount;
			$select = db_select('slt_profile_list_items','slp');
			$select->addExpression('COUNT(slp.id)','idcount');
			$select->condition('slp.code',$codeExist,'=');
			$result = $select->execute()->fetchField();
			if($result == 0){
				return $codeExist;
			}else{
				$actCount++;
			}

		} while ($result != 0);
	} catch (Exception $ex) {
		watchdog_exception('getSequenceCodeForListValues', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function for adding list values from api
 */
function addNewListValuesByRestAPI(&$form_state,$paramuserid="") {
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
		$form=array();
		$form["form_id"]="";
		$form_state['storage']['list_name'] = $form_state['values']['name'] = stripslashes($form_state['values']['name']);
		$form_state['list_code'] = $form_state['values']['code'];
		
		$AccArr = array('cre_usr_dpt','cre_usr_etp','cre_usr_jrl','cre_usr_jtl','cre_usr_ptp');
		if(!(in_array($form_state['values']['code'],$AccArr))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Code";
			return $errobj;
			//return array((object)array('code'=>'Invalid Code'));
		}
		$checkDuplicate = checkDuplicateEntity($form_state['values']['name'], $form_state['values']['code']);
		if($checkDuplicate){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Name already exists";
			return $errobj;
			//duplicate check(validation)
		}
		return addNewListValues($form,$form_state,$paramuserid,1);
	} catch (Exception $ex) {
		watchdog_exception('addNewListValuesByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function updateListValueDetailsByRestAPI(&$form_state,$paramuserid="") {
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
		$form=array();
		$form["form_id"]="";
		$form_state['list_id'] = $form_state['values']['id'];
		$form_state['list_code'] = $form_state['values']['code'];
		$AccArr = array('cre_usr_dpt','cre_usr_etp','cre_usr_jrl','cre_usr_jtl','cre_usr_ptp');
		$form_state['values']['name'] = stripslashes($form_state['values']['name']);
		if(!(in_array($form_state['values']['code'],$AccArr))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Code";
			return $errobj;
			//return array((object)array('id'=>'Invalid Code'));
		}
		
		if($form_state['values']['id'] > 0)
		{
				
			$result = db_select('slt_profile_list_items', 'spli')->fields(NULL, array('spli.id'))
			->condition('spli.code', '%'.$form_state['values']['code'].'%','LIKE')
			->condition('spli.id', $form_state['values']['id'])
			->countQuery()->execute()->fetchField();
		
			if($result <= 0){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Code and ID does not match";
				return $errobj;
				//return array((object)array('id'=>'Code and ID does not match'));
			}	
			//expDebug::dPrint("Row Cnt : ".print_r($result, 1));
		}
		else{
			$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid ID";
				return $errobj;
			//return array((object)array('id'=>'Invalid ID'));
		}
		$listId = updateListValues($form,$form_state,$paramuserid,1);
		return $listId;
	} catch (Exception $ex) {
		watchdog_exception('updateListValueDetailsByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function listUserListValuesByRestAPI($start, $limit, &$form_state) {
	try {
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		expDebug::dPrint('listUserListValuesByRestAPI'.print_r($form_state,true));
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		return getListValueSetUpDetailList(trim($form_state['values']['code']),'LIST',$start,$limit,trim($form_state['values']['textfilter']));
	} catch (Exception $ex) {
		watchdog_exception('listUserListValuesByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * To Check The Entity type are Avalable in Active and Inactive Group
 */
function Groups_availablecheck($entityType,$listCode) {
	try {
		expDebug::dPrint('Groups_availablecheck $entityType : '.$entityType .' $listCode : '.$listCode, 4);
		$select = db_select('slt_groups','grp');
		if($listCode == 'cre_usr_dpt'){
			$CheckBoth = db_or();
			// $CheckBoth->condition('grp.department', 'All','='); // Commented All For this ticket #0041777
			$CheckBoth->where('FIND_IN_SET(\''.$entityType.'\',grp.department)>0');
			$select->condition($CheckBoth);
				
		}else if($listCode == 'cre_usr_etp'){
			$CheckBoth = db_or();
			// $CheckBoth->condition('grp.employment_type', 'All','='); // Commented All For this ticket #0041777
			$CheckBoth->where('FIND_IN_SET(\''.$entityType.'\',grp.employment_type)>0');
			$select->condition($CheckBoth);
		}else if($listCode == 'cre_usr_jrl'){
			$CheckBoth = db_or();
			//$CheckBoth->condition('grp.job_role', 'All','='); // Commented All For this ticket #0041777
			$CheckBoth->where('FIND_IN_SET(\''.$entityType.'\',grp.job_role)>0');
			$select->condition($CheckBoth);
		}else if($listCode == 'cre_usr_ptp'){
			$CheckBoth = db_or();
			// $CheckBoth->condition('grp.user_type', 'All','='); // Commented All For this ticket #0041777
			$CheckBoth->where('FIND_IN_SET(\''.$entityType.'\',grp.user_type)>0');
			$select->condition($CheckBoth);
		}else{
			return 0;
		}
		$select->condition('grp.status','cre_sec_sts_del','!=');
		$select->addExpression('count(1)');
		expDebug::dPrintDBAPI('roleNameDuplicatecheck' , $select);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('Total result : '.$result, 4);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('Groups_availablecheck', $ex);
		expertusErrorThrow($ex);
	}
}

function checkDuplicateEntity($entityName, $listCode,$id='') {
	try {
		//0042093: Fileds needs to be trimmed->for remove tap and white spaces
		$entityName = preg_replace('/\s+/', ' ',$entityName);
		$query = db_select('slt_profile_list_items', 'list')
		->fields('list', array('id'))
		//->condition('code', '%' . $listCode . '_%', 'LIKE')
		->condition('code', '%' . $listCode . '%', 'LIKE')
		->condition('name', $entityName,'=')
		//0042387: Issue in people ->setting attributes
		->condition('is_active', 'Y', '=');
		if($id != ''){
			$query->condition('id', $id, '!=');
		}
		expDebug::dPrintDBAPI('checkDuplicateEntity' , $query);
		$result =  $query->execute();
		return $result->rowCount();
	} catch(Exception $e) {
		watchdog_exception('checkDuplicateEntity', $ex);
		expertusErrorThrow($ex);
	}
}


function add_list_search_callback($form,&$form_state){
	try {
		expDebug::dPrint('page call for add_list_search_callback-->>'.print_r($form_state['values']['listvalues-autocomplete'],true));
		//$listCode = $form_state['triggering_element']['#ajax']['list_code'];
		//$question_pgn_page = $form_state['triggering_element']['#ajax']['question_pgn_page'];

		$form_state = array();

		$form_state['rebuild'] = TRUE;

		$min_form_state = array(
				'ajax' => TRUE,
				're_render' => TRUE,
				'no_redirect' => TRUE,
				'list_id' => $listId,
				'code' => $code,
				'searchText' => $form_state['values']['listvalues-autocomplete'],
				'build_info' => array('args' => array('ajax'))
		);

		$form_state = $min_form_state;

		//$form_state['storage']['page'] = $question_pgn_page;

		$formId = 'exp_sp_administration_setup_addedit_form';
		$builtForm = drupal_build_form($formId, $form_state);

		$commands = array();
		$commands[] = ajax_command_replace(NULL, drupal_render($builtForm));

		return array(
				'#type' => 'ajax',
				'#commands' => $commands
		);
	} catch (Exception $ex) {
		watchdog_exception('add_list_pagination_callback', $ex);
		expertusErrorThrow($ex);
	}

}
function getSetupListEntitiesAutoComplete(){
	try{
		$entityId   = $_GET['entity_id'];
		expDebug::dPrint("Checking the auto complete values for the list setup".print_r($_GET,true),4);
		$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], $entityId.'_');
		foreach ($itemsList as $code => $name) {
			print $name . "\n";
		}
		
	}
	catch (Exception $ex) {
		watchdog_exception('getSetupListEntitiesAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}
?>