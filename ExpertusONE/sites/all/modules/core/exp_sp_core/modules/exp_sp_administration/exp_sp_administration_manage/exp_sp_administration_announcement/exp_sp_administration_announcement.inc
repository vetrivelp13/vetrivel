<?php
/**
 * Announcement Admin inc file 
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 08-Jan-2013
 */
 
 /*
 * getAnnouncementTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */ 

function getAnnouncementTabContentType(){ 
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
} 


/*
 * AnnouncementtNarrowSearchFiltersHTML - Returns the html for narrow search filters  
 */
function AnnouncementtNarrowSearchFiltersHTML() {
	try {
	$renderfiltersets=array(); 
	$managedByOptions = getManagedByFilters();
	$getlang =array();
	$getlang = getActiveLanguages();
	$checkedlangStatus = array();
	$preferredlang = getUserPreferredLanguage();
	
	$getlangToTheme = array($preferredlang => $getlang[$preferredlang]); // assign the Prefered lang To new variable #0044293
	if(!empty($preferredlang)) {
		foreach($getlang as $key => $value){
			if($preferredlang != $key){
				$getlangToTheme[$key] = $value; // assign the Not prefered language To variable #0044293
			}
		}
	}
	$checkedlangStatus[$preferredlang] = TRUE;
  $renderfiltersets[]= theme('narrow_search_filterset_daterange', array('code' => 'announcementdaterange',
                                                                       'title' => t('LBL042'), //Date
                                                                       'dateRangeShowOption' => 'all', // dateRangeShowOption options are 'all' or 'past' or 'future'
                                                                       'title_description' => t('LBL043'), //Range. Describes the title. Shown in ().
                                                                       'from_default_text' => t('LBL220'), //From: mm-dd-yyyy
                                                                       'to_default_text' => t('LBL221'), //To: mm-dd-yyyy
                                                                      )
                                                                  );
  
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array('code' => 'announcemanage',
																  		'html_id' => 'announcemanageby_container',
																  		'title' => t('Managed By'),
  																		'divid' => 'managed_by',
																  		'checkboxes_list' => $managedByOptions)
																  );
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array('code' => 'announcementlang',
																		'html_id' => 'announcelang_container',
																		'title' => t('LBL038'),
  																		'divid' => 'language',
																		'checkboxes_list' => $getlangToTheme,
  																		'checked_list' => $checkedlangStatus)
  	                                                              	);
                                                          
  $filterSetforJS=array(array('type'=>'checkbox','code'=>'announcementlang'),
  											 array("type" => "checkbox", 'code' => "announcemanage"),
                         array("type"=> "daterange", "code"=>"announcementdaterange",
  							"from_tooltip" => "", "from_default_text" => t("LBL220"),
  							"to_tooltip" => "", "to_default_text" => t("LBL221"))
                       );
                                                        
  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS), 
                     ));  
  } catch (Exception $ex) {
  	watchdog_exception('AnnouncementtNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * AnnouncementSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function AnnouncementSearchResultsSortbarHTML() {  
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'announcementdetails-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'announcementdetails-sort-za'); 
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'announcementdetails-sort-new');
    
  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'announcementdetails-sort-new'
                              );   
  
  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('AnnouncementSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
} 

/*
 * AnnouncementSearchResultsActionbarHTML - Returns the html for search results action bar  
 */
function AnnouncementSearchResultsActionbarHTML() { 
  try{
  $actionbarLinksListforJS=array('export_path'=>'administration/manage/announcement/export/',
  					             'print_path'=>'administration/manage/announcement/print/'
                              );   
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('Create Announcement'),
                                                  'show_add_button' => true,
												                          'add_page_path' => base_path() . 'administration/manage/announcement/nojs/addedit', 
												                          'show_print_button'=>true,
												                          'show_export_button'=>true,
                                                  'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS), 
                                                  'ctools_style'=>'ctools-modal-ctools-admin-announcement-notice-addedit-style',
  		                                            'page_code' => 'cre_sys_obt_not',
  		                                        
                                           )
         ); 
  } catch (Exception $ex) {
  	watchdog_exception('AnnouncementSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }  
}

/*
 * getAnnouncementTabContentMainHTML - Returns the html for tab content main 
 */
function getAnnouncementTabContentMainHTML() {   
	try{  
  $tabContentTypeArr =  getAnnouncementTabContentType();  
  $tabContentType=$tabContentTypeArr['tab_content_type'];   
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/manage/announcement/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/announcement/textfilter-autocomplete'                                            
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
  	watchdog_exception('getAnnouncementTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}
 
/*
 * getAnnouncementTabContent		  - Returns the html for narrow search block on announcement search page
 * 									  - Prepares JSON response for Survey Details.
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Expert Settings.
 */
function getAnnouncementTabContent() { 
	try{
  $tabContentTypeArr =  getAnnouncementTabContentType();  
  $tabContentType=$tabContentTypeArr['tab_content_type'];  
       
  $bannerResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => AnnouncementtNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar' => AnnouncementSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => AnnouncementSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getAnnouncementTabContentMainHTML(), 
    'rendered_main_div' => getAdminModules('administration','manage'),
  ); 
  echo drupal_json_encode($bannerResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getAnnouncementHTML() - Renders a announcement detail as HTML to be displayed in jqGrid.  
 */
function getAnnouncementHTML($ann) { 
  try{  
  $detailsList = array();
  $actionList = array(); 
  $bannerActivateStr="";
  if (!empty($ann->language)) {
    $langchar = substr($ann->language, 0, 3);
    $detailsList[] = array('detail' => t($langchar), 'tooltip' => t('LBL038').': '.t($ann->language));
  }	
  if (!empty($ann->from_date) && $ann->from_date!="0000-00-00 00:00:00") { 
    $bannerActivateStr.=  date("F j, Y",strtotime($ann->from_date));
  }
  
  if (!empty($ann->to_date) && $ann->to_date!="0000-00-00 00:00:00") { 
     if($bannerActivateStr!="") $bannerActivateStr.=" ";
     $bannerActivateStr.=t("to")." ". date("F j, Y",strtotime($ann->to_date));
  }
  if($bannerActivateStr!=""){
    $detailsList[] = array('detail' => t('LBL628')." ".$bannerActivateStr, 'tooltip' => t('LBL628')." ".$bannerActivateStr);
  }else{
     $detailsList[] = array('detail' => t('LBL629'), 'tooltip' => t('LBL629'));
  }
  
  $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/manage/announcement/nojs/addedit', 
					  'action_button_params' => $ann->id,
                      'action_button_param_text' => 'announcement',  
  					  'ctools_style' => 'ctools-modal-ctools-admin-announcement-notice-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  $actionList[] = array('action_link_text' => t('LBL286'),
  					  'action_page_path' => '',
					  'action_button_params' => $ann->id,
  					  'action_button_surveyassement_id' => $ann->id,
                      'action_button_param_text' => 'announcement',
  					  'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );    					  
  $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $ann->id,
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'entityId' => $ann->id,
  		'entityType' => 'cre_sys_obt_not',
  		'icon_style_class'=>'access-tab-icon',
  		'postype' => 'topleft',
  		'poslwid' => '',
  );
  $viewlist[] = array('action_link_text' => 'Announcement',
  		'action_page_path' => 'administration/view-announce/nojs/addedit/'.$ann->id.'',
  		'action_button_params' => $ann->id,
  		'action_button_surveyassement_id' => $ann->id,
  		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		 
  );
  
					  		  					  
  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($ann->fullname),                        
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.t('LBL350'),
                       'action_list'=> $actionList,
                       'description'=> trim($ann->description),
                       'promote_action' => $promoteAction, 
                 			 'new_action_list'=> $newActionList,
  										 'sumedit' => $ann->sumedit,
  										 'sumdelete' => $ann->sumdelete,
                 		   'view_list' =>$viewlist
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementHTML', $ex);
  	expertusErrorThrow($ex);
  }           
}

/*
 * getAnnouncementActionsHTML() - Renders a announcement's actions as HTML to be displayed in jqGrid.  
 */
function getAnnouncementActionsHTML($ann) { 
	try{      
     return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $ann)
              );  
     } catch (Exception $ex) {
     	watchdog_exception('getAnnouncementActionsHTML', $ex);
     	expertusErrorThrow($ex);
     }         
}
 
/**
 * announcementNoticeSearchSelectStmtAddOrderByClause() - Function to fetch and add announcement selected sort order type to the select statement
 */
function announcementNoticeSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try{
  switch (getURLParam('sortby')) {
  	
    case "AZ" : 
                $field = $tableAlias[0] . ".full_name";
                $direction = "ASC";
                break;
                  
    case "ZA" :
                $field = $tableAlias[0] . ".full_name";
                $direction = "DESC";
                break; 
                  
    case "NewlyListed" :
                $field = $tableAlias[1] . ".created_on";
                $direction = "DESC";
                break;

    default:
                $field = $tableAlias[1] . ".created_on";
                $direction = "DESC";
                break;
  }

  $selectStmt->orderBy($field, $direction);
  } catch (Exception $ex) {
  	watchdog_exception('announcementNoticeSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendBannerListToJqGrid() - Prepares JSON response to JqGrid's request for Banner
 */
function sendAnnouncementListToJqGrid() {
  try{
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

/*   $annCount = fetchAllAnnouncements('COUNT');
   
  expDebug::dPrint(' $annCount = ' . print_r($annCount, true) , 3);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $annCount); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  
  $announcementList = fetchAllAnnouncements('LISTS', $start, $limit);
  expDebug::dPrint(' $announcementList = ' . print_r($announcementList, true) , 3);  

  // Prepare and send the response
  $response= new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $annCount; 
                                             // have to do this.
  $response->initial_sort_type_html_id = 'announcementdetails-sort-new'; 
    
  $i=0;
  $announcementList1=$announcementList[0];
  
  // Mananged By behavior related code
  $userData = new stdClass();
  $userData->managedBy = $announcementList[1];
  $response->userdata = $userData;
  
  foreach ($announcementList1 as $ann) { 
    expDebug::dPrint(' $$ann = '. print_r($ann, true) , 3);

    $ann->row =  $i;
    $response->rows[$i]['id']   = $ann->id;
    
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $ann)),
                                        'details' => getAnnouncementHTML($ann),
                                        'action' => getAnnouncementActionsHTML($ann)
                                         );

    $i++;    
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);
  
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendAnnouncementListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getAnnouncementTextFilterAutoComplete() - Returns the matched announcement title
 */
function getAnnouncementTextFilterAutoComplete() { 
	try{
  $searchText =  $_GET['z'];
  // Select the table
  $select = db_select('slt_site_notice', 'notice');
  $select->leftJoin('slt_person', 'per', 'per.id = notice.user_id');
  // Add fields to fetch
  $select->addField('notice', 'description', 'description');
  $select->addExpression('per.full_name','full_name');			
  $tableAliasFields = array('notice.description','per.full_name');
  SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);
  $select->condition('notice.status',array('cre_sys_obt_not_atv','cre_sys_obt_not_itv'),'IN');

  filterByUserAccessPrivileges($select,'notice','',1,'cre_sys_obt_not','attach','notice'); // #46933 - group access visiblity check
  // Order in dictionary sort order
  $select->orderBy('full_name');
  $select->groupBy('full_name');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // print_r($select->__toString());
  $matchingBannerNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingBannerNames = ' . print_r($matchingBannerNames, true) , 3);

  foreach ($matchingBannerNames as $announcementName) {
    print rawurlencode($announcementName->full_name) . "\n";
  }  
	} catch (Exception $ex) {
  	watchdog_exception('getAnnouncementTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
} 

/*
 * fetchAllAnnouncements() - Fetches announcement count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchAllAnnouncements($op, $start='', $limit='',$callFrom='') { 
	try{
	$searchText = $_GET['textfilter'];
	expDebug::dPrint('checck the values d'.$searchText,4);
  // Select table
  $select = db_select('slt_site_notice', 'notice');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=notice.id AND group_map.entity_type = \'cre_sys_obt_not\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\''); */
  $select->leftJoin('slt_person', 'per', 'notice.user_id = per.id');
  $select->leftJoin('slt_profile_list_items', 'plilang', 'notice.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'notice.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  $select->condition('notice.status', 'cre_sys_obt_not_del', '!=');
  if($op=='COUNT'){
     // Get count of active Announcement
     $select->addExpression('COUNT(DISTINCT(notice.id))', 'COUNT');
  }
  else{
  	$select->distinct(); 
    // Select fields/expression values to be fetched from the database.
	$select->addField('notice', 'id', 'id');
	$select->addField('per', 'full_name', 'fullname');
	$select->addField('per', 'user_name', 'username');
	$select->addField('notice', 'from_date', 'from_date');
	$select->addField('notice', 'to_date', 'to_date'); 
	$select->addField('notice', 'from_time', 'from_time'); 
	$select->addField('notice', 'to_time', 'to_time');
	$select->addField('notice', 'description', 'description');
	$select->addField('notice', 'status', 'status_code'); 
	$select->addField('notice', 'priority', 'priority');
	$select->addField('plilang', 'name', 'language');
	$select->addField('plistatus', 'name', 'status');
	$select->addExpression('\'announcement\'', 'object_type');   
	$select->groupBy('notice.id');
  } 
  filterByUserAccessPrivileges($select, 'notice','',1,'cre_sys_obt_not','','notice');
  $tableAliasFields = array('notice.description','per.full_name');
  // Set the conditions
  NarrowSearchByNoticeDateRangeMultiCondition($select,'announcementdaterange');
  NarrowSearchByAnnouncementLanguage($select,'announcementlang');
  
  SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);
  
  $managefilter = '';
  if ($callFrom != 'API') {
  	$managefilter = selectAddNarrowSearchManageByCondition('announce', $select, 'notice', 'announcemanage');
  }
  
  if($op != "COUNT"){
    // Order the records
    announcementNoticeSearchSelectStmtAddOrderByClause($select, array('per','notice'), "AZ"); 
  }
  
  expDebug::dPrintDBAPI(' $select obj = ' , $select);  
    
  if($op == "COUNT"){  
  	$result = $select->execute()->fetchField();  
	expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
	return $result;
  }
  elseif($op == "CSV"){
	  $colHeadersMap = array(t('LBL083') => 'fullname',
                           t('LBL038') => 'language',
                           t('LBL102') => 'status',
                           t('LBL620') => 'from_date',
	                         t('LBL621') => 'to_date',
	                         t('Priority') => 'priority', 
                       );
	  
     $filterArray[] = array(t('LBL350').':'.textboxFiltersetValueToPrintableStr('textfilter'));
     $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('announcementstatus'));
     $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('announcementlang'));      
     $filterArray[] = array(t('LBL622').':'.daterangeFiltersetValueToPrintableStr('announcementdaterange'));  
     $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
     $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('announcemanage'));

  	 SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'anm_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL083') => array('fullname',17),                       
                           t('LBL038') => array('language',15),
    					   t('LBL102') => array('status',18),
                           t('LBL620') => array('from_date',18),
                           t('LBL654') => array('to_date',18),
                           t('Priority') => array('priority',14)
                        );
    
    
    $appliedFilters = array(t('LBL350') => textboxFiltersetValueToPrintableStr('textfilter'),
    		t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('announcementstatus'),
    		t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('announcementlang'),
    		t('LBL622') => daterangeFiltersetValueToPrintableStr('announcementdaterange'),
    		ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    		t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('announcemanage'),
    );

    SendNarrowSearchResultsAsPDFFile($select, t('LBL350'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'anm_admin_search');
  }
  else{
  		add_row_id_condition($select, 'notice');
	     // Limit to $limit records 
	     $select->range($start, $limit);
	    // Execure the query and fetch all the records
	    expDebug::dPrintDBAPI('date range--->>', $select);
	    $result = $select->execute()->fetchAll();
	    expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3); 
	    $newResult[0]=$result;
	    $newResult[1]=$managefilter;
	    return $newResult;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchAllAnnouncements', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function anm_admin_search_csv_row_data_alter($rowData) {
  try {
    if (!empty($rowData[0]->from_date)) {
      $rowData[0]->from_date = date_format(date_create($rowData[0]->from_date), 'Y-m-d');
    }
    if (!empty($rowData[0]->to_date)) {
      $rowData[0]->to_date = date_format(date_create($rowData[0]->to_date), 'Y-m-d');
    }  

    if (strtolower($rowData[0]->status) == 'inactive') {
      $rowData[0]->status = t('Unpublished');
    }
    else {
      $rowData[0]->status = t('Published');
    }

    if ($rowData[0]->priority == 1) {
      $rowData[0]->priority = t('LBL1244');
    }
    else {
      $rowData[0]->priority = t('LBL1243');
    }
  }
  catch (Exception $ex) {
    watchdog_exception('anm_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function anm_admin_search_pdf_row_data_alter($rowData) {
  try {
    if (!empty($rowData[0]->from_date)) {
      $rowData[0]->from_date = date_format(date_create($rowData[0]->from_date), 'Y-m-d');
    }
    if (!empty($rowData[0]->to_date)) {
      $rowData[0]->to_date = date_format(date_create($rowData[0]->to_date), 'Y-m-d');
    }  

    if (strtolower($rowData[0]->status) == 'inactive') {
      $rowData[0]->status = t('Unpublished');
    }
    else {
      $rowData[0]->status = t('Published');
    }

    if ($rowData[0]->priority == 1) {
      $rowData[0]->priority = t('LBL1244');
    }
    else {
      $rowData[0]->priority = t('LBL1243');
    }
  }
  catch (Exception $ex) {
    watchdog_exception('anm_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

function getDefaultAnnouncementAddEditFormFieldValue($field, $form_state, $announcementDetails) {
	try{
  $defaultValue = '';  
  switch ($field) {
    case 'shordesc':
      $defaultValue = (empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='')?
                                    ((empty($announcementDetails[$field]) || $announcementDetails[$field]=='')? '' : $announcementDetails[$field]) :
                                    $form_state['values'][$field]['value'];
  	  break;
    default:   // For all other fields
      $defaultValue = getDefaultAddEditFormFieldValue($field, $form_state, $announcementDetails);
      break;
  } // end switch */
  
  expDebug::dPrint(' $defaultValue for field ' . $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultAnnouncementAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}
function getAnnouncementDetails($announcementId, $fields){
	try{
  if (empty($fields)) {
    // Get all fields
    $fields = array('id', 'user_id', 'description', 'lang_code', 'status','from_date', 'to_date', 'from_time','to_time',
				'priority', 'created_by', 'created_on', 'updated_by','updated_on', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
  }
  
  // Select table
  $selectStmt = db_select('slt_site_notice', 'notice');
  $selectStmt->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = notice.lang_code');
  $selectStmt->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = notice.status');
  
  // Set conditions
  $selectStmt->condition('notice.id', $announcementId);

  // Select fields and/or add expressions
  foreach($fields as $fieldName) {
    $selectStmt->addField('notice', $fieldName, $fieldName);
  }
  $selectStmt->addField('prof1','name','language_name');
  $selectStmt->addField('prof2','name','status_name');
  
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();
  
  return $result->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewAnnouncement($form, &$form_state){
 try { 
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state1 = ' . print_r($form_state['storage']['files_details'], true) , 5);
  $loggedInUserId = getIdOfLoggedInUser();
   $txn = db_transaction();
		try{
		  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
     
      expDebug::dPrint('Trigger element = ' . print_r($form_state['storage']['submit_triggering_element'], true) , 5);
     $trigger_element = $form_state['storage']['submit_triggering_element'];
     if($trigger_element == 'saveandpublish'){     
       $ann_status = 'cre_sys_obt_not_atv';
     }
     else if($trigger_element == 'save'){
       $ann_status = 'cre_sys_obt_not_itv';
     }
     else if($form_state['values']['crs_status']){
  		 $ann_status =$form_state['values']['status'];
     } 
     
     if(!empty($form_state['values']['from_date'])){
     	$old_dateformat=str_replace("-","/",$form_state['values']['from_date']);
     	$from_date= date_format(date_create( $old_dateformat),'Y-m-d');
     }
     if(!empty($form_state['values']['to_date'])){
     	$old_dateformat1=str_replace("-","/",$form_state['values']['to_date']);
     	$to_date= date_format(date_create( $old_dateformat1),'Y-m-d');
     }
      
     // Select the table to insert the record in
      $insertStmt = db_insert('slt_site_notice');
      $fields = array(
                  'user_id'         => $loggedInUserId,
                  'description'     => $form_state['values']['description']['value'],
                  'lang_code'       => $form_state['values']['lang_code'],
                  'status'          => $ann_status,
                  'from_date'       => $from_date,
                  'to_date'         => empty($to_date) ? NULL : $to_date,
                  'from_time'       => $form_state['values']['from_time'],
                  'to_time'         => $form_state['values']['to_time'],
                  'priority'        => $form_state['values']['priority'],
                  'created_by'      => $loggedInUserId,
                  'created_on'      => now(),
                  'updated_by'      => $loggedInUserId,
                  'updated_on'   	  => now(),
                );
     $insertStmt->fields($fields);
     
    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);  
    
     $announcementId = $insertStmt->execute();
     updateGroupMappingDetails($announcementId,'cre_sys_obt_not',$loggedInUserId,'emptyadd',0);
     $form_state['values']['id'] = $announcementId;
     //Audit trail enrtries
    make_audit_trail_entries($fields, $loggedInUserId , null ,$announcementId, 'Inserted','cre_sys_obt_not','exp_sp_administration_announcement','Add new Announcement',
                                         array('user_id', 'description', 'lang_code', 'status', 'from_date','to_date','to_time','priority'));
     
 
     return array((object)array('id'=>$announcementId));  
    
		}catch (Exception $ex){
          $txn->rollback();
          watchdog_exception('banner insert', $ex);
          throw $ex;  
   
		}
		} catch (Exception $ex) {
			watchdog_exception('addNewAnnouncement', $ex);
			expertusErrorThrow($ex);
		}
	}
	

 function updateAnnouncementDetails($form, &$form_state, $paramuserid=""){
 
    try{
    	  $loggedInUserId = getIdOfLoggedInUser();
        $old_dateformat=str_replace("-","/",$form_state['values']['from_date']);
        $from_date = date_format(date_create( $old_dateformat),'Y-m-d');
        $old_dateformat1 = str_replace("-","/",$form_state['values']['to_date']);
        $to_date = date_format(date_create( $old_dateformat1),'Y-m-d');  		
        $formid=$form_state['values']['id'];
        expDebug::dPrint('Trigger element = ' . print_r($form_state['storage']['submit_triggering_element'], true) , 5);
        
        $formid=$form_state['values']['id'];
        $default= db_query("SELECT user_id,description,lang_code,status,from_date,to_date,from_time,to_time,priority from slt_site_notice where id=:uid",array(":uid"=>$formid))->fetchAll();
        $trigger_element = $form_state['storage']['submit_triggering_element'];
        if($trigger_element == 'saveandpublish'){     
          $ann_status = 'cre_sys_obt_not_atv';
        }else if($trigger_element == 'save'){
          $ann_status = 'cre_sys_obt_not_itv';
        } elseif($trigger_element == 'unpublish'){
           $ann_status = 'cre_sys_obt_not_itv';
        } else if($form_state['values']['crs_status']){
    	     $ann_status = $form_state['values']['status'];
        } 
        if($trigger_element == 'unpublish'){
            $updateStmt = db_update('slt_site_notice');    
            $updateStmt->condition('id', $form_state['values']['id']);
            $fields = array(
              				'status' => $ann_status,
              				'updated_by'             => $userid,
              				'updated_on'             => now(),
            );
              
         } else {
            $updateStmt = db_update('slt_site_notice');    
            $updateStmt->condition('id', $form_state['values']['id']);
            $fields = array(
            		'description'     => $form_state['values']['description']['value'],
            		'lang_code'       => $form_state['values']['lang_code'],
            		'status'          => $ann_status,
            		'from_date'       => $from_date,
            		'to_date'         => empty($to_date) ? NULL : $to_date,
            		'from_time'       => $form_state['values']['from_time'],
            		'to_time'         => $form_state['values']['to_time'],
            		'priority'        => $form_state['values']['priority'],
            		'updated_by'      => $loggedInUserId,
            		'updated_on'   	  => now(),
            );
          }    
          $updateStmt->fields($fields);
          expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
          // Execute query
          $numUpdated = $updateStmt->execute();
     
          make_audit_trail_entries($fields, $loggedInUserId , $default ,$formid, 'Updated','cre_sys_obt_not','exp_sp_administration_announcement','updateAnnouncementDetails',
                                             array('user_id', 'description', 'lang_code', 'status', 'from_date', 'to_date','from_time','to_time','priority'));
              
         
  		return array((object)array('id'=>$form_state['values']['id']));

	} catch (Exception $e){
		throw new SoapFault("SPLMS",$e->getMessage());
	}
 
 }

function addNewAnnouncementByRestAPI(&$form_state){
	try{
    $form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	expDebug::dPrint(' addNewAnnouncementByRestAPI created ANN:' );
	$annId =  addNewAnnouncement($form,$form_state);
	expDebug::dPrint(' addNewAnnouncementByRestAPI' . print_r($annId,true) ,5);
	return $annId;
	
	} catch (Exception $ex) {
		watchdog_exception('addNewAnnouncementByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function updateAnnouncemnetByRestAPI(&$form_state,$paramuserid=""){
  try {
  $form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$annId =  updateAnnouncementDetails($form,$form_state, $paramuserid);
	return $annId;
	} catch (Exception $ex) {
		watchdog_exception('updateAnnouncemnetByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function announcementProfileListItems($codePrefix){
	try {
  //$statusOrder=array('cbn_anm_sts_atv','cbn_anm_sts_itv','cbn_anm_sts_dft');
  $select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code IN(\'cre_sys_obt_not_atv\',\'cre_sys_obt_not_itv\') AND is_active = \'Y\' ORDER BY FIELD(CODE,\'cre_sys_obt_not_atv\',\'cre_sys_obt_not_itv\')');
  $result = $select->fetchAllKeyed();
  expDebug::dPrintDBAPI(' announcementProfileListItems' , $result );
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('announcementProfileListItems', $ex);
  	expertusErrorThrow($ex);
  }
} 

function getAnnouncementDate($noticeId) {
	try {
 	$select = db_select('slt_site_notice', 'notice');
  // Set the conditions 
  $select->condition('notice.id', $noticeId, '=');
  $select->addField('notice','from_date','from_date');
  $select->addField('notice','to_date','to_date');
  // Execute the query
  $result = $select->execute()->fetchAll();
  expDebug::dPrint('  PDF $pdfList after mod = ' . print_r($result, true) , 3);  
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementDate', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the announcement
 */
function publishOrUnpublishAnnouncement($annId, $annType ){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    $status  = getAnnouncementStatus($annId);
    $updateStmt = db_update('slt_site_notice');
    $updatedstatus  = ($status=='cre_sys_obt_not_atv')?"cre_sys_obt_not_itv":"cre_sys_obt_not_atv";  
    
    $updateStmt->condition('id', $annId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,                                                   
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
   
    add_audit_trail_entry($currUserId, $annId, 'cre_sys_obt_not', 'exp_sp_administration_announcement', 'publishOrUnpublishAnnouncement', 'Updated announcement Status',
		                                     null, $status, $updatedstatus);
	
	$returnStatus  = ($updatedstatus=='cre_sys_obt_not_atv')?t('LBL571'):t('LBL570');
	print $updatedstatus.'|'.$returnStatus;	                                     
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishAnnouncement', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of the program
 */
function getAnnouncementStatus($annId){
	try {
  $select = db_select('slt_site_notice', 'notice');
  $select->addField('notice','status', 'status');
  $select->condition('notice.id',$annId);
  // Execute the query
  $annStatus = $select->execute()->fetchField();
  return $annStatus; 
  } catch (Exception $ex) {
  	watchdog_exception('getAnnouncementStatus', $ex);
  	expertusErrorThrow($ex);
  } 
}

function announcementsubmitAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $annId, $subStage = '', $emptyId = '') {
	try {
  expDebug::dPrint(' announcementsubmitAddEditFormWithCancelAndSaveClose() called.');
  global $theme_key;
  $customHiddenButtonId = 'last-button-account';
  
  $esignHiddenButt = 'esign-'.$wrapperId;
  
  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);
  
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
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
  
  $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
  $cancelLabel  = t('LBL123'); // Close
  
  $cancelAttributes['tabindex'][] = '10';
  $prfixPgLeft = '';
  $suffixPgRight = '';
  if($theme_key == 'expertusoneV2'){
  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  }
  $form[$cancelButtonId] = array(
  		'#type' => 'submit',
  		'#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
  		'<div class="addedit-form-cancel-container-actions">'. $prfixPgLeft,
  		'#suffix' => $suffixPgRight,
  		'#value' => $cancelLabel,
  		'#attributes' => $cancelAttributes
  );
  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';
  
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  
  // Using this class to applying bg color to the save button
  //$saveAttributes['class'][] = 'admin-save-button-middle-bg';
  $saveAttributes['class'][] = 'edit-catalog-course-save-publish';


  //Increase the width of the popup while clicking on save button
  $saveAttributes['onclick'][] = 'increasePopWidth();';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);
  $saveAttributes['tabindex'][] = '8';
  $eSignatureAvailable = true;
  

  if(empty($subStage)) {
    $announcementStatus     = '';    
    if (!empty($annId)) { 
      $announcementStatus    =  getAnnouncementStatus($annId);
    }    
   $isSaveParent = 0;
   $leftBg = '';
   if($theme_key == 'expertusoneV2'){
   	$leftBg = '<div class="admin-save-button-left-bg"></div>';
   }
   if(empty($annId) || $announcementStatus != 'cre_sys_obt_not_atv'){
      $isSaveParent = 1;
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = 'increasePopWidth();';	 
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-pub-unpub-button-container">' .$leftBg,
	    '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span></div>',
	    '#value' => t('LBL141'),
      	'#name' => 'save',
	    '#attributes' => $saveAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'customButtonId'=>$customHiddenButtonId,
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
   
    if($isSaveParent){
      $form['addedit-button-group-ul-start'] = array(
        '#type' => 'markup',
        '#markup' => '<ul class="catalog-pub-add-list">',
      ); //  catalog-save-btn
      $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
      $savePublishSuffix = '</li>';
    }else{
      $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container">'.$leftBg;
      $savePublishSuffix = ((($resultCnt==0) && ($announcementStatus == 'cre_sys_obt_not_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';      
    }
	  $saveButtonId = 'catalog-course-save-publish';
	  $savePublishAttributes['onclick'] = array();
	  $savePublishAttributes['tabindex'][] = '9';

      $savePublishAttributes['onclick'][] = 'increasePopWidth();';
     $buttonLabel = ($announcementStatus == 'cre_sys_obt_not_atv') ? t('LBL141') : t('LBL614');
     $savePublishAttributes['class'][] = (($resultCnt==0) && ($announcementStatus == 'cre_sys_obt_not_atv')) ? 'edit-catalog-course-save-publish' : 'pub-unpub-only-save-btn';
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => $savePublishPrefix,
	    '#suffix' => $savePublishSuffix,
	    '#value' => $buttonLabel,
      	'#name' => 'saveandpublish',
	    '#attributes' => $savePublishAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'customButtonId'=>$customHiddenButtonId,
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
	  
   if(!$isSaveParent){
      $form['addedit-button-group-ul-start'] = array(
        '#type' => 'markup',
        '#markup' => '<ul class="catalog-pub-add-list">',
      );
    }
    
	if(($annId) && $announcementStatus == 'cre_sys_obt_not_atv'){
	  if($resultCnt == 0){
  	    $saveButtonId = 'catalog-course-save-unpublish';
  	    $saveUnPublishAttributes['onclick'] = array();
        $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
        $form[$saveButtonId] = array(
  	    '#type' => 'submit',
  	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    '#suffix' => '</li>',
  	    '#value' => t('LBL571'),
        '#name' => 'unpublish',
  	    '#attributes' => $saveUnPublishAttributes,
        '#ajax' => array(
    	      'wrapper' => $wrapperId,
    	      'callback' => $ajaxCallbackSave,
              'customButtonId'=>$customHiddenButtonId,
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
	}
	$deleteCheck = adminVisibilityForAddEditDelete($annId,'cre_sys_obt_not','cre_sys_obt_not','slt_site_notice');
    if($annId && ($announcementStatus == 'cre_sys_obt_not_itv') && ($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1)){
      if($resultCnt == 0){ 
  	   $saveButtonId = 'catalog-course-delete';
  	   
  	   $deleteAttributes['onclick'] = array();
       $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357'). ' '.
                                              strtolower(t('LBL350')).'","'.$annId.'","announcement"); return false;';
       $deleteAttributes['class'][] = 'admin-delete-button-bg';
       $form[$saveButtonId] = array(
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
      '#markup' => '</ul><div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
    ); 
  }
else{
   $saveAttributes['onclick'] = array();
  $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
  $saveAttributes['onclick'][] = 'increasePopWidth();';	 
  $form[$saveButtonId] = array(
    '#type' => 'submit',
    '#value' => t('LBL141'), // Save
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
    
} 
  unset($saveAttributes['tabindex']);
  $saveAttributes['onclick'] = array();
  $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';	 
  $saveAttributes['onclick'][] = 'increasePopWidth();';
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
		      'callback' => 'addEditRepaintForm',
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
  
   
  $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  	//'#prefix' => '<div style="display:none;"></div>',
  );	       
  } catch (Exception $ex) {
  	watchdog_exception('announcementsubmitAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAnnouncementTitle($annId){
	try {
	$selectAnn = db_select('slt_site_notice','notice');
	$selectAnn->leftjoin('slt_person','per','per.id = notice.user_id');
	$selectAnn->addField('per','full_name', 'fullname');
	$selectAnn->condition('notice.id', $annId,'=');
	$result =  $selectAnn->execute()->fetchAssoc();
	return $result['ann_title'];
	} catch (Exception $ex) {
		watchdog_exception('getAnnouncementTitle', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addFileUploadControlToAddEditForm() - Adds a file upload field to addedit form
 */
function addBannerFileUploadControlToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 61) {
	try {
  // Prepare the additional HTML attributes for the textfield 
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
  
 $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('DATEFIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('DATEFIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addDateFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }
  
  
  $form[$id] = array(
    '#type' => 'file',
   // '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
    //'#suffix' => addEditGetSuffix('FIELD'),
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
  	watchdog_exception('addBannerFileUploadControlToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addDateFieldToAddEditFormNew(&$form, $id, $title = '', $defaultValue = '', $emptyDateFieldText = 'mm-dd-yyyy', $datePickerDatesDisplayOption = 'all', $datePickerTooltip = '', $tabIndex = null,
                                           $mandatory = false, $readOnly = false, $viewWidthInChars = 10, $maxChars = 40) {
	try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all file upload fields
  $attributes['class'][] = 'addedit-edit-datefield';  
  
  $attributes['class'][] = 'addedit-readonly-datefield';
  $attributes['readonly'] = 'readonly';
  

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Set the data required by Drupal.behaviors.addDatePickerToAddEditDateField to attach datepicker, etc.
  $attributes['data-datePickerDatesDisplayOption'] = $datePickerDatesDisplayOption;
  //$attributes['data-emptyDateFieldText'] = $emptyDateFieldText;
  $attributes['data-datePickerTooltip'] = $datePickerTooltip;

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('DATEFIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('DATEFIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addDateFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }
   $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_DATEFIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_DATEFIELD');

  // Create the form element

  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
  	'#disabled' => $readOnly,
    '#default_value' => $defaultValue,
    '#attributes' => $attributes, 
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars
  );
  } catch (Exception $ex) {
  	watchdog_exception('addDateFieldToAddEditFormNew', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * NarrowSearchByNoticeDateRangeMultiCondition() - To the $selectStmt add Search by a date range if set by the user 
 */
function NarrowSearchByNoticeDateRangeMultiCondition($selectStmt, $filtersetCode) {
	try{
  $dateRangeFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $dateRangeFilters = ' . print_r($dateRangeFilters, true) , 4);
  if (!empty($dateRangeFilters) && $dateRangeFilters != "|") {
    $dateRange = explode("|", $dateRangeFilters);
    //$dateRange[0]= str_replace("-","/",$dateRange[0]);
    //$from_date = date_format(date_create($dateRange[0]),'Y-m-d');
    $from_date = date_format(date_create_from_format('m-d-Y', $dateRange[0]), 'Y-m-d');
    //$dateRange[1] = str_replace("-","/",$dateRange[1]);
    //$to_date = date_format(date_create($dateRange[1]),'Y-m-d');
    $to_date = date_format(date_create_from_format('m-d-Y', $dateRange[1]), 'Y-m-d');
    expDebug::dPrint(' $from_date = ' . $from_date , 4);
    expDebug::dPrint(' $to_date = ' . $to_date , 4);
    if(!empty($from_date) && !empty($to_date)){
    	$selectStmt->condition('notice.from_date', $from_date, '>=');
    	$selectStmt->condition('notice.to_date', $to_date, '<=');
    }else if (!empty($from_date) && $to_date =='' ){
    	$selectStmt->condition('notice.from_date', $from_date, '>=');
    }else if (!empty($to_date) && $from_date=='' ){
    	$selectStmt->condition('notice.to_date', $to_date, '<=');
    }
  } 
  } catch (Exception $ex) {
  	watchdog_exception('NarrowSearchByNoticeDateRangeMultiCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

function NarrowSearchByAnnouncementLanguage($selectStmt,$filtersetCode) {
	try{
	$preferredlang = getUserPreferredLanguage();
	$langFilter= !isset($_GET['announcementlang']) ? $preferredlang : $_GET['announcementlang'];
	if (!empty($langFilter)){
		$langCode = explode("|", $langFilter);
		$selectStmt->condition('notice.lang_code', $langCode, 'IN');
	}
	} catch (Exception $ex) {
		watchdog_exception('NarrowSearchByAnnouncementLanguage', $ex);
		expertusErrorThrow($ex);
	}
}

function get_announcement_details($userid,$callfromAPI = 0){
	try{
		global $theme_key;
		$user_id = getIdOfLoggedInUser();
		db_merge('slt_person_preference')
		->key(array('user_id' => $user_id))
		->updateFields(array(
				'announcement_visited'   	  => now()
				
		))
		->insertFields(array(
				'user_id' => $user_id,
				'widget_preference' => null,
				'widget_deleted' => null,
				'created_by' => $user_id,
				'created_on' => now(),
				'updated_by' => $user_id,
				'updated_on' => now(),
				'announcement_visited' => now()
		))
		->execute();
		
		$announcementListCount = fetchAnnouncementUserWidget('count',$userid);
		
		if(isset($_REQUEST['page']) && isset($_REQUEST['rows'])) {
            $page = $_REQUEST['page']; // get the requested page
            $limit = $_REQUEST['rows'];
            $total_pages = 0;
            
            $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $announcementListCount);
        } else {
            expDebug::dPrint('$first '.$_GET['first'].' and last '.$_GET['last'] ,4);
            
            $start = max(0, intval($_GET['first']) - 1);
            $last  = max($start + 1, intval($_GET['last']) - 1);
            
            $limit = $last - $start + 1;
            expDebug::dPrint('$$start'.$start.' and limit '.$limit, 4);
        }
		
		$announcementListDetails = fetchAnnouncementUserWidget('list', $userid, $start, $limit);
		expDebug::dPrint('$announcementListDetails'.print_r($announcementListDetails,true),4);
		
		if($callfromAPI){
			return $announcementListDetails;
		}
		// Prepare and send the response
		$response= new stdClass();
		$response->page = $page;
		$response->total = $total_pages;
		$response->records = $announcementListCount;
			
		$i=0;
		foreach ($announcementListDetails as $announcement) {
			$announcement->description = preg_replace("/<a(.*?)>/", "<a$1 target=\'_blank\'>", $announcement->description);
			$response->rows[$i]['id']=$announcement->uid;
			$response->rows[$i]['personid']=$announcement->personid;
			$response->rows[$i]['cell']['id'] = $announcement->uid;
			$response->rows[$i]['cell']['personid'] = core_encrypt($announcement->personid);
			$response->rows[$i]['cell']['nid'] = $announcement->nid;
			$response->rows[$i]['cell']['description'] = $announcement->description;
			$response->rows[$i]['cell']['from_date'] = Date('M,d Y',strtotime($announcement->from_date));
			$response->rows[$i]['cell']['announce_shortdesc']	= titleController('ANNOUNCEMENT_SHORTTITLE', $announcement->description,80);
			$response->rows[$i]['cell']['announce_fulldescription'] = $announcement->description;
			$response->rows[$i]['cell']['css'] = 'exp-announce-rec';

			// Get screen_name from full_name
			/*$exp_ufn = explode(" ",$announcement->full_name);
	 	if(count($exp_ufn) > 2){
	 		$response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".substr($exp_ufn[2], 0, 1);
	 	}
	 	else {
	 		$response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".substr($exp_ufn[1], 0, 1);
	 	}*/
	 	
	 	$response->rows[$i]['cell']['screen_name'] = $announcement->full_name;

	 	// Create the image url from uri
	 	if($theme_key == 'expertusoneV2'){
	 		if (!empty($announcement->uri)) {
	 			$response->rows[$i]['cell']['image_url'] = file_create_url($announcement->uri);
	 		}
	 		else {
	 			$response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/expertusonev2_default_user.png');
	 		}
	 	} else{
	 		if (!empty($announcement->uri)) {
	 			$response->rows[$i]['cell']['image_url'] = file_create_url($announcement->uri);
		 	}
		 	else {
		 		$response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/default_user.png');
		 	}
	 	}

	 	$i++;
		}
	  if (count($announcementListDetails) > 0) {
			$response->rows[0]['cell']['css'] .= ' exp-announce-first-rec';
			$response->rows[count($announcementListDetails) - 1]['cell']['css'] .= ' exp-announce-last-rec';
		} 
		echo drupal_json_encode($response);
	}catch(Exception $ex) {
		expDebug::dPrint('get_announcement_details'.$ex,4);
		watchdog_exception('get_announcement_details', $ex);
		expertusErrorThrow($ex);
	}
}

function fetchAnnouncementUserWidget($op,$userid='',$start='',$limit='') {
	try{
		$userPreferredLang = getUserPreferredLanguage();
		expDebug::dPrint('get $userPreferred Language'.$userPreferredLang,4);
		if(empty($userid))
			$userid  = getIdOfLoggedInUser();
		$userTimezone = getUserPHPTimezoneName($userid);
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$select = db_select('slt_site_notice', 'snote');
		$select->leftJoin('slt_site_notice_dismiss', 'snd', 'snd.nid = snote.id and snd.user_id = '.$userid);
		$select->leftJoin('slt_person', 'p', 'p.id = snote.user_id');
		$select->leftJoin('users', 'u', 'u.name = p.user_name');
		$select->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
		//$select->leftJoin('slt_catalog_access', 'catace', 'catace.entity_id = snote.id and catace.entity_type = \'cre_sys_obt_not\'');
	/* 	$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id = snote.id and group_map.entity_type = \'cre_sys_obt_not\'');
		$select->leftJoin('slt_groups','catace','catace.id=group_map.group_id and (catace.is_admin =0) and catace.status =\'cre_sec_sts_atv\''); */
		
		$select->isNull('snd.id');
		$select->where("CONCAT( DATE_FORMAT(snote.from_date, _utf8'%Y-%m-%d'),' ',snote.from_time,':00') < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW())");
		$select->where("CONCAT( DATE_FORMAT(snote.to_date, _utf8'%Y-%m-%d'),' ',snote.to_time,':00') > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW()) or to_date is null"); 
		$select->condition('snote.status', 'cre_sys_obt_not_atv', '=');
		$select->condition('snote.lang_code', $userPreferredLang , '=');
		filterByUserAccessPrivileges($select, 'snote',$userid,'','cre_sys_obt_not');
		
		$select->orderBy('snote.priority','desc');
		$select->orderBy('snote.created_on','desc');
		
		
		if($_REQUEST['apiname'] && $_REQUEST['mobile_request']){
		$select->where("Not FIND_IN_SET($userid, snote.viewed) or snote.viewed is null");
		}
		
		expDebug::dPrintDBAPI(' Fetch announcement query COUNT =11111 ' );
			
		// Execute the query and fetch all the records
		if($op == 'count'){
			// Limit to $limit records
			$select->addExpression('COUNT(DISTINCT(snote.id))', 'count');
			//$select->addExpression('COUNT(1)','count');
			expDebug::dPrintDBAPI(' Fetch announcement query COUNT = ' , $select );
			$announcementList = $select->execute()->fetchField();
		}
		else{
			$select->distinct(); 
			$select->addField('snote', 'id', 'nid');
			$select->addField('u', 'uid', 'uid');
			$select->addField('p', 'id', 'personid');
			$select->addField('p', 'full_name', 'full_name');
			$select->addField('pic', 'uri', 'uri');			
			$select->addField('snote', 'description', 'description');
			$select->addField('snote', 'from_date', 'from_date');
			$select->range($start, $limit);
			$selectInd = $select->extend("ExpertusSelectQueryExtender");
			$selectInd->setIndex(array('snote'=>'sli_siteann_lan'));
			$announcementList = $select->execute()->fetchAll();
			expDebug::dPrintDBAPI('Fetch announcement query LIST = ' , $selectInd);
			expDebug::dPrint(' $$announcementList = ' . print_r($announcementList, true) , 4);
		}
		expDebug::dPrint('result for the announcement details'.print_r($announcementList,true),4);
		return $announcementList;
	}catch(Exception $ex) {
		expDebug::dPrint('fetchAnnouncementAll'.$ex,4);
		watchdog_exception('fetchAnnouncementAll', $ex);
		expertusErrorThrow($ex);
	}
}
function dismiss_announcement($nid,$userid){
	$txn = db_transaction();
	try{
		$select = db_insert('slt_site_notice_dismiss');
		$select->fields(array('nid'     => $nid,
				'user_id'     => $userid,
				'created_by'     => $userid,
				'created_on'     => now()));
		$result = $select->execute();
		expDebug::dPrintDBAPI('dismiss announcement Query',$select);
		//Audit Entry
		add_audit_trail_entry($userid, $nid, 'cre_sys_obt_usr', 'exp_sp_administration_announcement', 'dismissannouncement', 'Deleted ',
				$userid, null, $result);
	}catch(Exception $ex) {
		$txn->rollback();
		expDebug::dPrint('dismiss_announcement'.$ex,4);
		watchdog_exception('dismiss_announcement', $ex);
		expertusErrorThrow($ex);
	}
	unset($txn);
}
function getActiveLanguages(){
	try{
		$select = db_select('slt_profile_list_items', 'spfa');
		$select->addField('spfa', 'code', 'langcode');
		$select->addField('spfa', 'name','langname');
		$select->condition('spfa.code', 'cre_sys_lng_%','LIKE');
		$select->Condition('spfa.is_active','Y');
		$select->orderBy('langname', 'ASC');
		expDebug::dPrintDBAPI('$select-->',$select);
		$languages = $select->execute()->fetchAllKeyed();
		expDebug::dPrint('$languages-->'.print_r($languages,true),4);
		return $languages;
		
	}catch (Exception $ex) {
		watchdog_exception('getActiveLanguages', $ex);
		expertusErrorThrow($ex);
	}

}
function exp_sp_administration_viewanounce_addedit_form($js = NULL, $entityId){
	try{
		global $theme_key;
		$css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
		if($theme_key == 'expertusoneV2') {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
		}else{
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
		}
		expDebug::dPrint('sabbeeee-->'.$entityId);
		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 600,
								'minHeight' => 300,
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
					array('id' => 'exp_sp_administration_view_announcedetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewbanner_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_announcedetails_addedit_form($form, &$form_state,$renderMode, $annId){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$$annId---->'.$annId);

		$wrapperId = 'viewannounce-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$announcedetails = getAnnounceDetailsInfoView($annId);

		expDebug::dPrint('$$announcedetails-->'.print_r($announcedetails,true),5);
		$form['narrow_announceview'] = array(
				'#markup'       => theme('narrow_announceview', array('record' => $announcedetails)),
		);

		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_announcedetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getAnnounceDetailsInfoView($annId){
	try{
		$select = db_select('slt_site_notice', 'notice');
		$select->leftJoin('slt_profile_list_items', 'plilang', 'notice.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
		$select->addField('notice', 'id', 'id');
		$select->addField('notice', 'description', 'description');
		$select->addField('notice', 'from_date', 'from_date');
		$select->addField('notice', 'to_date', 'to_date');
		$select->addField('notice', 'from_time', 'from_time');
		$select->addField('notice', 'to_time', 'to_time');
		$select->addField('notice', 'priority', 'priority');
		$select->addField('plilang', 'name', 'language');
		$select->condition('notice.id',$annId, '=');
		expDebug::dPrintDBAPI('execute banner query',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('v$result$result$result->'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getBannerDetailsInfoView', $ex);
		expertusErrorThrow($ex);
	}
}
function notificationforann(){
	$userid  = getIdOfLoggedInUser();
	$userPreferredLang = getUserPreferredLanguage();
	$userTimezone = getUserPHPTimezoneName($userid);
	$timeZone = date_default_timezone(false);
	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	
	$select_time = db_select('slt_person_preference','pre');
	$select_time->addField('pre', 'announcement_visited');
	$select_time->condition('pre.user_id',$userid,'=');
	expDebug::dPrintDBAPI(' Last visit time = ' , $select_time );
	$time = $select_time->execute()->fetchField();
	expDebug::dPrintDBAPI(' Last visit time = ' , $time );
	$select = db_select('slt_site_notice', 'snote');
	$select->leftJoin('slt_site_notice_dismiss', 'snd', 'snd.nid = snote.id and snd.user_id = '.$userid);
	$select->leftJoin('slt_person', 'p', 'p.id = snote.user_id');
	$select->leftJoin('users', 'u', 'u.name = p.user_name');
	$select->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
	$select->leftJoin('slt_person_preference', 'pref', 'pref.user_id = p.id');
	$select->isNull('snd.id');
	$select->where("CONCAT( DATE_FORMAT(snote.from_date, _utf8'%Y-%m-%d'),' ',snote.from_time,':00') < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW())");
    $select->where("CONCAT( DATE_FORMAT(snote.to_date, _utf8'%Y-%m-%d'),' ',snote.to_time,':00') > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,'".$userTimezone[0]->zone_name."'),NOW()) or to_date is null");
	$select->condition('snote.status', 'cre_sys_obt_not_atv', '=');
	$select->condition('snote.lang_code', $userPreferredLang , '=');
	if($time){
		$select->condition('snote.updated_on',$time,'>');
	}
	
	filterByUserAccessPrivileges($select, 'snote',$userid,'','cre_sys_obt_not');
	$select->addExpression('COUNT(DISTINCT(snote.id))', 'count');
	//$select->addExpression('COUNT(1)','count');
	expDebug::dPrintDBAPI(' Fetch notificatiin announceemnt query COUNT = ' , $select );
	$new_announcement = $select->execute()->fetchField();
	expDebug::dPrintDBAPI(' Fetch notificatiin announceemnt query COUNT = ' , $new_announcement );
	return $new_announcement;
}
?>
