<?php
/**
 * Custom Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */
include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');

 /*
 * getCustomTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */

function getCustomTabContentType(){
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getCustomTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getCustomLanguage() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix and
 * 								Fetch all related announcement language.The result is ordered by name.
 */
/*
 * CustomtNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */

function CustomNarrowSearchFiltersHTML() {
	try {  $checkedStatus = array();
  //$checkedStatus["cbn_anm_sts_atv"] = TRUE;
  $statusOrder=array('cre_cattr_sts_atv','cre_cattr_sts_itv','cre_cattr_sts_del');
  $allStatus = CustomProfileListItems($statusOrder);
  $allType = getCustomattributeTypeFilters();
  $allStatus1 = getProfileStatusItemNamesKeyed('cre_cattr_sts_');
  $allStatus1['cre_cattr_sts_atv'] = t('LBL921');
  $allStatus1['cre_cattr_sts_itv'] = t('LBL922');
  $allMandatory = getMandatory('cre_sys_inv_');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus1, true) , 4);
  $allType1 = getEntityOptions('','ASC');
  $managedByOptions = getManagedByFilters();
  $checkedlangStatus = array(); // #45326 - default language selection added
  $preferredlang = getUserPreferredLanguage();

  $getlangToTheme = array($preferredlang => $allLang[$preferredlang]);
  if(!empty($preferredlang)) {
  	foreach($allLang as $key => $value){
  		if($preferredlang != $key){
  			$getlangToTheme[$key] = $value;
  		}
  	}
  }
  $checkedlangStatus[$preferredlang] = TRUE;
  $renderfiltersets=array();
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'customdatatype',
                                                                 'title' => t('LBL036'),
                                                           		 'divid' => 'type',
                                                                 'checkboxes_list' => $allType)
                                                           );
  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
  	                                                       array('code' => 'customstatus',
  				                                                 'title' => t('LBL102'),
  				                                                 'divid' => 'status',
  				                                                 'checkboxes_list' => $allStatus1)
                                                           );
  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
  		                                                   array('code' => 'custommandatory',
  				                                                 'title' => t('Mandatory'),
  				                                                 'divid' => 'Mandatory',
  				                                                 'checkboxes_list' => $allMandatory)
                                                           );
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'customscreen',
                                                                 'title' => t('LBL2019'),
                                                           		 'divid' => 'Screen',
                                                                 'checkboxes_list' => $allType1)
                                                           );
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'custommanage',
                                                           		 'html_id' => 'contentmanageby_container',
                                                                 'title' => t('Managed By'),
                                                           		 'divid' => 'managed_by',
                                                                 'checkboxes_list' => $managedByOptions)
                                                           );
														    $filterSetforJS = array();
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'customdatatype');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'customstatus');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'custommandatory');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'customscreen');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'custommanage');
  
$filterSetforJS=array(array('type'=>'checkbox','code'=>'customdatatype'),
		array('type'=>'checkbox','code'=>'customstatus'),
		array('type'=>'checkbox','code'=>'custommandatory'),
                         array('type'=>'checkbox','code'=>'customscreen'),
                    array('type'=>'checkbox','code'=>'custommanage'),
                       );

  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS),
                     ));
  } catch (Exception $ex) {
  	watchdog_exception('CustomNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * CustomSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function CustomSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'Customdetails-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'Customdetails-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'Customdetails-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'Customdetails-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('CustomSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
  * CustomSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function CustomSearchResultsActionbarHTML($argType) {
  try {
  $actionbarLinksListforJS=array('export_path'=>'administration/manage/customattribute/export/',
  					             'print_path'=>'administration/manage/customattribute/print/'
                              );
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL817'),
                                                 'show_add_button' => true,
												//'add_page_path' => base_path() . 'administration/manage/banner/nojs/addedit',
												'add_page_path' => base_path() . 'administration/manage/customattribute/nojs/addedit',
												//'add_page_path' => 'administration/learning/course-class/nojs/addedit', 
												'show_print_button'=>true,
												'show_export_button'=>true,
			                                      'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
			                                      'ctools_style'=>'ctools-modal-ctools-admin-customattribute-addedit-scroll-wrapper',
			                                       //'page_code' => 'cbn_anm_typ_ban',
			       	  					          'grey_out_add_button' => $argType,
			       	  					          'page_code' => 'cre_sys_obt_cattr',
			                                    )
         );
  } catch (Exception $ex) {
  	watchdog_exception('CustomSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCustomTabContentMainHTML - Returns the html for tab content main
 */
function getCustomTabContentMainHTML() {
	try {
  $tabContentTypeArr =  getCustomTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                           // 'search_base_path'=>'administration/manage/banner/search/all/',
                                            'search_base_path'=>'administration/manage/customattribute/search/all/',
                                            
                                            'show_top_text_filter' => true,
                                            //'text_filter_ac_path' => 'administration/manage/banner/textfilter-autocomplete'
                                            'text_filter_ac_path' => 'administration/manage/customattribute/textfilter-autocomplete'
                                            
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
  	watchdog_exception('getCustomTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCustomTabContent		  - Returns the html for narrow search block on announcement search page
 * 									  - Prepares JSON response for Survey Details.
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Expert Settings.
 */
function getCustomattributeTabContent($argType) {
	try {
		  expDebug::dPrint(' hi = ' . print_r($argType, true) , 3);
		
  $tabContentTypeArr =  getCustomTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];

  $CustomResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => CustomNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar' => CustomSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => CustomSearchResultsActionbarHTML($argType),
    'rendered_tab_content_main'=>getCustomTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','manage'),
  );
  		  expDebug::dPrint(' hi 1= ' . print_r($CustomResponse, true) , 3);
  
  echo drupal_json_encode($CustomResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getCustomTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * getCustomHTML() - Renders a custom attribute detail as HTML to be displayed in jqGrid.
 */
function getCustomHTML($Custom) {
  try {
  $detailsList = array();
  $actionList = array();
  $CustomActivateStr="";
  /*if (!empty($Custom->code)) {
    $detailsList[] = array('detail' => $Custom->code, 'tooltip' => t('LBL096'));
  }*/
  if (!empty($Custom->language)) {
    $langchar = substr($Custom->language, 0, 3);
    $detailsList[] = array('detail' => ($langchar == 'Sim') ? t('Chi') :t($langchar), 'tooltip' => t('LBL038').': '.t($Custom->language));
  }
  /*if (!empty($Custom->status)) {
    $detailsList[] = array('detail' => $Custom->status, 'tooltip' => t('LBL102'));
  }	*/
  if (!empty($Custom->cattr_type)) {
  	//expDebug::dPrint(' $Custom cdswc= '. print_r($Custom, true) , 3);
  	//expDebug::dPrint(' $Custom dcsac= '. print_r($Custom->cattr_type, true) , 3);
  	// custom attribute types in the list page
  	if ($Custom->cattr_type == 'cattr_type_checkbox'){
  		$detailsList[] = array('detail' => t('LBL2006'), 'tooltip' => t('LBL2006'));
  		$editcustom = 'checkbox';
  	}
  	else if ($Custom->cattr_type == 'cattr_type_date'){
  		$detailsList[] = array('detail' => t('LBL042'), 'tooltip' => t('LBL042'));
  		$editcustom = 'date';
  	}
  	else if ($Custom->cattr_type == 'cattr_type_dropdown'){
  		$detailsList[] = array('detail' => t('LBL2007'), 'tooltip' => t('LBL2007'));
  		$editcustom = 'dropdown';
  	}
  	else if ($Custom->cattr_type == 'cattr_type_radio'){
  		$detailsList[] = array('detail' => t('LBL2008'), 'tooltip' => t('LBL2008'));
  		$editcustom = 'radio';
  	}
  	else if ($Custom->cattr_type == 'cattr_type_txtarea'){
  		$detailsList[] = array('detail' => t('LBL2010'), 'tooltip' => t('LBL2010'));
  		$editcustom = 'textarea';
  	}
  	else if ($Custom->cattr_type == 'cattr_type_txtbox'){
  		$detailsList[] = array('detail' => t('LBL2009'), 'tooltip' => t('LBL2009'));
  		$editcustom = 'textbox';
  	}
  }
  // details in the list page
  if (!empty($Custom->cattr_mandatory)) {
  
  if ($Custom->cattr_mandatory == 1)
  	{  
  		$detailsList[] = array('detail' => t('Mandatory'), 'tooltip' => t('Mandatory'));
  	}
  }
  else{
  	$detailsList[] = array('detail' => t('Optional'), 'tooltip' => t('Optional'));
  	 
  }
  
  $actionList[] = array('action_link_text' => t('LBL063'),//edit
					  'action_page_path' => 'administration/manage/customattribute/nojs/addedit',
					  'action_button_params' => $editcustom,
                      'action_button_params_text' => 'Custom',
  		              //'action_button_params_child' => core_encrypt($Custom->id), 
  		              'action_button_params_child' => $Custom->id,
  		
  					 // 'ctools_style' => 'ctools-modal-ctools-admin-customattribute-addedit-style',
  					 'ctools_style' => 'ctools-modal-ctools-admin-customattribute-addedit-scroll-wrapper', 
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  $actionList[] = array('action_link_text' => t('LBL286'),//delete
  					  'action_page_path' => '',
					  'action_button_params' => $Custom->id,
  					  'action_button_surveyassement_id' => $Custom->id,
                      'action_button_param_text' => 'Custom',
  					  'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );
 
  $newActionList[] = array('action_link_text' => t('LBL2005'),//Show in screen    
  										
  		//'action_page_path' => 'administration/manage/customattribute/nojs/addedit',
  		//'action_page_path' => 'administration/manage/customattribute/shortcut/showinscreen',
  		//'action_button_params' => $Custom->id,
  		 'action_button_params' => $editcustom.'/'.$Custom->id.'',  
  		//'action_button_params_text' => 'custom',
  		'action_button_params_text' => 'show_in_screen',
  		'action_button_params_child' => 'from_show_in_screen',
  		'ctools_style' => 'ctools-modal-ctools-admin-customattribute-addedit-scroll-wrapper',  
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL2005'),
  		'entityId' => $Custom->id,
  		'entityType' => 'cre_sys_obt_cattr',
  		//'icon_style_class'=> ($Custom->sumedit>0) ? 'showinscreen-tab-icon' : 'showinscreen-disable-tab-icon',
  		'icon_style_class'=>  'showinscreen-tab-icon',
  		'postype' => 'topleft',
  		'poslwid' => '',
  );
  $viewlist[] = array('action_link_text' => 'customattribute',
  					  'action_page_path' => 'administration/view-customattribute/nojs/addedit/'.$Custom->id.'',
					  'action_button_params' => $Custom->id,
  					  'action_button_surveyassement_id' => $Custom->id,
  					  'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),

					  );

  $Custom->entityType = 'cre_sys_obt_cattr';

  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($Custom->title),
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.strtolower(t('LBL2015')),
                       'action_list'=> $actionList,
                       'description'=> trim($Custom->description),
                       'promote_action' => $promoteAction,
                 		   'new_action_list'=> $newActionList,
                 		 'sumedit' => $Custom->sumedit,
                 		   'sumdelete' => $Custom->sumdelete,
                 		   'view_list' => $viewlist,
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getCustomHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCustomActionsHTML() - Renders a announcement's actions as HTML to be displayed in jqGrid.
 */
function getCustomActionsHTML($Custom) {
    try {
     return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $Custom)
              );
     } catch (Exception $ex) {
     	watchdog_exception('getCustomActionsHTML', $ex);
     	expertusErrorThrow($ex);
     }
}

/**
 * CustomSearchSelectStmtAddOrderByClause() - Function to fetch and add custom attribute selected sort order type to the select statement
 */
function CustomSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".cattr_name";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias . ".cattr_name";
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
  	watchdog_exception('CustomSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendCustomListToJqGrid() - Prepares JSON response to JqGrid's request for Custom
 */
function sendCustomListToJqGrid() {
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
  
  $numCustom = fetchCustomAttributeAll('COUNT');
  expDebug::dPrint(' $numCustom = ' . print_r($numCustom, true) , 3);

  $total_pages = 0;
  //$start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numCustom);
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);


  $CustomList = fetchCustomAttributeAll('LISTS', $start, $limit);
  expDebug::dPrint(' $CustomList = ' . print_r($CustomList, true) , 3);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numCustom;
                                             // have to do this.
  $response->initial_sort_type_html_id = 'Customdetails-sort-new';

  $i=0;
  $CustomList1=$CustomList[0];
  // Mananged By behavior related code
  $userData = new stdClass();
  $userData->managedBy = $CustomList[1];
  $response->userdata = $userData;
  

  //Dynamic translation - start 
  $config=getConfig('exp_sp');
  if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
  {
      
      global $user;
      
      $translationStrings = array();
      $translationids= array();
  foreach ($CustomList1 as $Custom) {
          $title    =   $Custom->title;
          $helptext =   $Custom->cattr_help_txt;
          
          $translationStrings[]=$title;
          $translationStrings[]=$helptext;
          
          $translationids[] = $Custom->id;
      }
      expDebug::dPrint(' $translatedLabels before = ' . print_r($translationStrings, true) , 3);
      
      $translatedLabels = getTranslationsForArrayStrings($translationStrings,$user->language,$translationids,"cre_sys_obt_cattr");
      expDebug::dPrint(' $translatedLabels  after = ' . print_r($translatedLabels, true) , 3); 
  }
  
  $modExist =module_exists("exp_sp_administration_customattribute") ;
  foreach ($CustomList1 as $Custom) {
    
      if($modExist && isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
      {
        $Custom->title          = strval($translatedLabels[$Custom->title]);
        $Custom->cattr_help_txt = strval($translatedLabels[$Custom->cattr_help_txt]);
      }
    
    expDebug::dPrint(' $Custom = '. print_r($Custom, true) , 3);
    $Custom->object_type = 'customattribute';
    
    $Custom->row =  $i;
    $response->rows[$i]['id']   = $Custom->id;
    $response->rows[$i]['cell'] = array( 'details' => getCustomHTML($Custom),
                                         'action' => getCustomActionsHTML($Custom)
                                         );

    $i++;
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendCustomListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCustomTextFilterAutoComplete() - Returns the matched announcement title
 */
function getCustomTextFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_custom_attr', 'cusatt');

  // Add fields to fetch
  $select->addExpression('DISTINCT(cusatt.cattr_name)', 'title');
  //$tableAliasFields = array('announcement.cattr_name');
  
  //SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);
  
  selectAddNarrowSearchByTextMultipleORCondition($select, "cusatt", array("cattr_name"), "z");
  //selectAddNarrowSearchByTextMultipleORCondition($select, "announcement", array("title", "code", "description", "shortdesc"), "z");
  
  //$select->condition('announcement.status',array('cbn_anm_sts_atv','cbn_anm_sts_itv'),'IN');

  filterByUserAccessPrivileges($select, 'cusatt','',1,'cre_sys_obt_cattr','','cusatt'); // #46933 - Group access check 
  // Order in dictionary sort order
  $select->orderBy('cattr_name');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // print_r($select->__toString());
  $matchingCustomNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingCustomNames = ' . print_r($matchingCustomNames, true) , 3);

  foreach ($matchingCustomNames as $announcementName) {
    print rawurlencode($announcementName->title) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getCustomTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchCustomAll() - Fetches custom attribute count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchCustomAttributeAll($op = NULL , $start = NULL , $limit = NULL,$callFrom='') {
  try {
  // Select table
  $select = db_select('slt_custom_attr', 'cusatt');
  //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cusatt.id AND group_map.entity_type = \'cre_sys_obt_cattr\' and group_map.group_type = 1');
  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  //$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_cattr\''); 
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'cusatt.status = plistatus.code');
  $select->leftJoin('slt_custom_attr_mapping','cusatt1','cusatt1.cattr_id = cusatt.id');
  $select->condition('cusatt.status', array('cre_cattr_sts_del'), 'NOT IN');
  if($op=='COUNT'){
     // Get count of active custom attributes
     $select->addExpression('COUNT(DISTINCT(cusatt.id))', 'COUNT');
  }
  else{
  	$select->distinct();  
	$select->addField('cusatt', 'id', 'id');
	$select->addField('cusatt', 'cattr_name', 'title');
	$select->addField('cusatt', 'cattr_type', 'cattr_type');
	$select->addField('cusatt', 'cattr_mandatory', 'cattr_mandatory');
	$select->addField('plistatus', 'name', 'custom_attribute_status');
	$select->addField('cusatt', 'status', 'status');
	$select->addExpression("CASE WHEN cusatt.cattr_type = 'cattr_type_txtbox' THEN 'Text Box' WHEN cusatt.cattr_type = 'cattr_type_txtarea' THEN 'Text Area' WHEN cusatt.cattr_type = 'cattr_type_dropdown'
	THEN 'Drop Down'  WHEN cusatt.cattr_type = 'cattr_type_radio' THEN 'Radio Button'  WHEN cusatt.cattr_type = 'cattr_type_checkbox'  THEN 'Check Box'  ELSE cusatt.cattr_type END", "cattr_type1");
	$select->addExpression('if(cusatt.cattr_mandatory = 1,\'Yes\',\'No\')','cusatt_mandatory1');
	$select->addExpression("CASE WHEN cusatt1.entity_type = 'cre_sys_obt_crs' THEN 'Course' WHEN cusatt1.entity_type = 'cre_sys_obt_cls' THEN 'Class' WHEN cusatt1.entity_type = 'cre_sys_obt_trp'
	THEN 'Training Plan'  WHEN cusatt1.entity_type = 'cre_usr' THEN 'User'  WHEN cusatt1.entity_type = 'cre_org'  THEN 'Organization' 
    WHEN cusatt1.entity_type = 'cre_sys_obt_cnt' THEN 'Content' WHEN cusatt1.entity_type = 'cre_sys_obt_loc' THEN 'Location' WHEN cusatt1.entity_type = 'cre_sys_obt_ord' THEN 'Order'
    ELSE cusatt1.entity_type END", "asso_screen");		
	$select->addExpression('if(cusatt1.entity_screen_opt = 1
			AND cusatt1.entity_api_opt = 1,"SCREEN and API",if(cusatt1.entity_screen_opt = 1,"SCREEN",if(cusatt1.entity_api_opt = 1,"API","")))', 'type_of_association');
	if($op != "CSV" && $op != "PDF"){
	$select->addExpression("GROUP_CONCAT(cusatt1.entity_type)", "entity_type");
	$select->addExpression("GROUP_CONCAT(cusatt1.entity_screen_opt)", 'entity_screen_opt');
	$select->addExpression("GROUP_CONCAT(cusatt1.entity_api_opt)", 'entity_api_opt');
    $select->groupBy('cusatt.id');
	}
	else if($op == "CSV" || $op == "PDF")
	{		
		$select->addField('cusatt1', 'entity_type', 'entity_type');
		$select->addField('cusatt1', 'entity_screen_opt', 'entity_screen_opt');
		$select->addField('cusatt1', 'entity_api_opt', 'entity_api_opt');
	}
  }
  filterByUserAccessPrivileges($select, 'cusatt','',1,'cre_sys_obt_cattr','','cusatt');
  // Set the conditions
  selectAddNarrowSearchINCondition($select, 'cusatt', 'cattr_type', 'customdatatype');
  selectAddNarrowSearchINCondition($select, 'cusatt', 'status', 'customstatus');
  selectAddNarrowSearchINCondition($select, 'cusatt1', 'entity_type', 'customscreen');
  $managefilter = '';
  if ($callFrom != 'API') {
  	$managefilter =   selectAddNarrowSearchManageByCondition('customattribute', $select, 'cusatt', 'custommanage');
  	expDebug::dPrint(' $managefilter = ' . print_r($managefilter, true) , 3);
  	 
  }
  if(!empty($_GET['custommandatory']))
  {
  if($_GET['custommandatory']=='cre_sys_inv_man'){
  	 $select->condition('cusatt.cattr_mandatory', 1, '=');
  }
  else if($_GET['custommandatory']=='cre_sys_inv_opt')
  {
  	 $select->condition('cusatt.cattr_mandatory', 0, '=');
  }
  }
  selectAddNarrowSearchByTextMultipleORCondition($select, "cusatt", array("cattr_name"), "textfilter");
  

  if($op != "COUNT"){
    // Order the records
    CustomSearchSelectStmtAddOrderByClause($select, 'cusatt', "AZ");
  }

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  if($op == "COUNT"){
  	$result = $select->execute()->fetchField();
	expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
	return $result;
  }
  elseif($op == "CSV"){
  	$colHeadersMap = array(t('Label') => 'title',
  			t('LBL036') => 'cattr_type1',
  			t('Mandatory') => 'cusatt_mandatory1',
  			t('LBL102') => 'custom_attribute_status',
  			t('LBL2017') => 'asso_screen',
  			t('LBL2021') => 'type_of_association'
  	);
     $filterArray[] = array(t('LBL2015').':'.textboxFiltersetValueToPrintableStr('textfilter'));
     $filterArray[] = array(t('LBL036').':'.checkboxFiltersetcustomdatatypesToPrintableStr('customdatatype'));
     $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('customstatus'));
     $filterArray[] = array(t('Mandatory').':'.checkboxFiltersetSelectionsToPrintableStr('custommandatory'));
     $filterArray[] = array(t('LBL2019').':'.checkboxFiltersetSelectionsToPrintableStr('customscreen'));
     $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('custommanage'));
    
   $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
  	 SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'Custom_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('Label') => array('title',20),
  			t('LBL036') => array('cattr_type1',20),
    		t('Mandatory') => array('cusatt_mandatory1',20),
    		t('LBL102') => array('custom_attribute_status',20),
    		t('LBL2017') => array('asso_screen',20),
    		t('LBL2021') => array('type_of_association',20)
    		
                        );
    $appliedFilters = array(t('LBL2015') => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL036') => checkboxFiltersetcustomdatatypesToPrintableStr('customdatatype'),
    		 		        t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('customstatus'),    		
    					    t('Mandatory') => checkboxFiltersetSelectionsToPrintableStr('custommandatory'),
    						t('LBL2019') => checkboxFiltersetSelectionsToPrintableStr('customscreen'),
    						t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('custommanage'),
    		
    		ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
    		);                       
    SendNarrowSearchResultsAsPDFFile($select, t('LBL2015'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'Custom_admin_search');
  }
  else{
  	// LIST
  	// Execute the query and fetch all the records
  	add_row_id_condition($select, 'cusatt', 'id');
	    // Limit to $limit records
	    $select->range($start, $limit);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    
	    expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3);
	    $newResult[0]=$result;
	    $newResult[1]=$managefilter;
        return $newResult;	     


  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchCustomAttributeAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * listCustomAttributeAPI() - This function using for 'List of Custom Attributes API'  
 */
function listCustomAttributeAPI($start = NULL, $limit = NULL, $rows, $name, $type, $entity_type, $status, $userid) {
	try {
		
		$attr_type = array('textbox'=>'cattr_type_txtbox', 'textarea'=>'cattr_type_txtarea', 'dropdown'=>'cattr_type_dropdown', 'radiobutton'=>'cattr_type_radio', 'checkbox'=>'cattr_type_checkbox');
		
		$entity_map_type = array('course'=>'cre_sys_obt_crs', 'class'=>'cre_sys_obt_cls', 'trainingplan'=>'cre_sys_obt_trp', 'user'=>'cre_usr', 'organization'=>'cre_org', 'group'=>'cre_sys_obt_grp', 'content'=>'cre_sys_obt_cnt', 'location'=>'cre_sys_obt_loc', 'order'=>'cre_sys_obt_ord');
		
		
		//$attr_type = array('Text Box'=>'cattr_type_txtbox', 'Text Area'=>'cattr_type_txtarea', 'Drop Down'=>'cattr_type_dropdown', 'Radio Button'=>'cattr_type_radio', 'Check Box'=>'cattr_type_checkbox');
		
		//$entity_map_type = array('Course'=>'cre_sys_obt_crs', 'Class'=>'cre_sys_obt_cls', 'Training Plan'=>'cre_sys_obt_trp', 'User'=>'cre_usr', 'Organization'=>'cre_org','Content'=>'cre_sys_obt_cnt', 'Location'=>'cre_sys_obt_loc', 'Order'=>'cre_sys_obt_ord');
		
		// Select table
		$select = db_select('slt_custom_attr', 'cusatt');
		$select->leftJoin('slt_custom_attr_mapping','cusmap','cusmap.cattr_id=cusatt.id');
		$select->leftJoin('slt_custom_attr_options','cusopt','cusopt.cattr_id=cusatt.id');
		//$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cusatt.id  and group_map.group_type = 1');
		//$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
		//$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_cattr\'');
		$select->distinct();
		$select->addField('cusatt', 'id', 'id');
		$select->addField('cusatt', 'cattr_name', 'name');
		//$select->addField('cusatt', 'cattr_type', 'type');
		$select->addExpression("CASE WHEN cusatt.cattr_type = 'cattr_type_txtbox' THEN 'Text Box' WHEN cusatt.cattr_type = 'cattr_type_txtarea' THEN 'Text Area' WHEN cusatt.cattr_type = 'cattr_type_dropdown'
	   THEN 'Drop Down'  WHEN cusatt.cattr_type = 'cattr_type_radio' THEN 'Radio Button'  WHEN cusatt.cattr_type = 'cattr_type_checkbox'  THEN 'Check Box'  ELSE cusatt.cattr_type END", "type");
		//$select->addField('cusatt', 'cattr_datatype', 'datatype');
		$select->addExpression("CASE WHEN cusatt.cattr_datatype = 'cattr_dtype_text' THEN 'Text' WHEN cusatt.cattr_datatype = 'Number' THEN 'Number' ELSE cusatt.cattr_datatype END", "datatype");
		$select->addField('cusatt', 'cattr_length', 'datalength');
		$select->addField('cusatt', 'cattr_mandatory', 'mandatory');
		$select->addField('cusatt', 'cattr_unique', 'attr_unique');
		$select->addField('cusatt', 'cattr_help_txt', 'help_txt');
		$select->addField('cusatt', 'cattr_multiple_opt', 'multiple_option');
		$select->addField('cusatt', 'cattr_display_opt', 'display_option');
		$select->addExpression('GROUP_CONCAT(DISTINCT cusopt.opt_name)', 'opt_name');
		//$select->addField('cusmap', 'entity_type', 'entity_type');
		$select->addExpression('TRIM(BOTH \',\' FROM (GROUP_CONCAT(DISTINCT if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_crs","Course",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_cls","Class",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_trp","TP",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_usr","User",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_org","Organization",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cattr_ent_grp","Group",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_cnt","Content",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_loc","Location",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_ord","Order",""),\'\'))))', 'entity_type');
		$select->addField('cusmap', 'entity_screen_opt', 'entity_screen_option');
		$select->addField('cusmap', 'entity_api_opt', 'entity_api_option');
		$select->addField('cusatt', 'status', 'status');
		$select->groupBy('cusatt.id');
				
		if($status) {
			$status_split = explode(',',$status);
			$select->condition('cusatt.status', $status_split, 'IN');
		}
		
		if($name)
		{
			$name = strtolower(str_replace(' ','',$name));				
			$select->condition('cusatt.cattr_name', '%' . db_like($name) . '%', 'LIKE');
		}
		
		if($type) {
			$type_split = explode(',',$type);
			
			foreach($type_split as $value) {
				$value = strtolower(str_replace(' ','',$value));				
				$type_concate[] = $attr_type[$value];
			}
			
			$select->condition('cusatt.cattr_type', $type_concate, 'IN');
		}
		
		if($entity_type) {			
				
			$entitytype_split = explode(',',$entity_type);
							
			foreach($entitytype_split as $value) {				
				$value = strtolower(str_replace(' ','',$value));				
				$entitytype_concate[] = $entity_map_type[$value];				
			}
			
			$select->condition('cusmap.entity_type', $entitytype_concate, 'IN');
		}

		filterByUserAccessPrivileges($select, 'cusatt','',$userid,'cre_sys_obt_cattr','','cusatt');
		
		// Limit to $limit records
		$select->range($start, $limit);
			expDebug::dPrintDBAPI("listCustomAttributeAPI Select Query ==> ",$select);
		// Execute the query and fetch all Records
		$result = $select->execute()->fetchAll();
			expDebug::dPrint('listCustomAttributeAPI Final $result ==> ' . print_r($result, true) , 3);
		
		$config=getConfig('exp_sp');
		if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1"){
		    $user =  getDrupalUserDataForTranslation($userid);
		    expDebug::dPrint("drupalUserId:".$user->uid);
		  
		  $translationStringsForCustAttr = array();
		  $translationStringsForCustAttrOptions = array();
		  
		  $translationids= array();
		  expDebug::dPrint("count:".count($result));
		  for($i = 0; $i < count($result);$i++) {
		      $row = $result[$i];
		      expDebug::dPrint("name:".$row->name);
		      
		      $title    =   $row->name;
		      $helptext =   $row->help_txt;
		      $opt_name =   $row->opt_name;
		      
		      $translationStringsForCustAttr[]=$title;
		      $translationStringsForCustAttr[]=$helptext;
		      
		      if($opt_name != null && $opt_name != ""){
		          
		          $optarr = explode(",",$opt_name);
		          for($j = 0; $j < count($optarr); $j++ )
		              $translationStringsForCustAttrOptions[] =$optarr[$j];
		      }
		      
		          $translationids[] = $row->id;
		  }
		  expDebug::dPrint(' $translatedLabels before api = ' . print_r($translationStringsForCustAttr, true) , 3);
		  
		  $translatedLabels = getTranslationsForArrayStrings($translationStringsForCustAttr,$user->language,$translationids,"cre_sys_obt_cattr");
		  $translatedLabelsOptions = getTranslationsForArrayStrings($translationStringsForCustAttrOptions,$user->language,"","custom_attribute_options");
		  
		  expDebug::dPrint(' $translatedLabels after api  = ' . print_r($translatedLabels, true) , 3);
		  expDebug::dPrint(' $translatedLabelsOptions after api  = ' . print_r($translatedLabelsOptions, true) , 3);
		  
		  for($i = 0; $i < count($result);$i++) {
		      $result[$i]->name = strval($translatedLabels[$result[$i]->name]);
		      $result[$i]->help_txt = strval($translatedLabels[$result[$i]->help_txt]);
		      if($result[$i]->opt_name != null)
		          $opt_name = trim($result[$i]->opt_name) ;
		      
		      if( $opt_name != "")
		      {
		          
		          expDebug::dPrint('opt name===== ' . $opt_name , 3);
		          $optarr = explode(",",$opt_name);
		          $translatedOptName = "";
		          for($j = 0; $j < count($optarr); $j++ )
		          {
		              $translatedLbl = strval($translatedLabelsOptions[$optarr[$j]]); 
		              $translatedLbl = $translatedLbl == ""?$optarr[$j]:$translatedLbl;
		              
		              if($translatedOptName == "")
		                  $translatedOptName = $translatedLbl;
		              
		              else
		                  $translatedOptName .= ",".$translatedLbl;
		          }
		      }
		      $result[$i]->opt_name = $translatedOptName;
		  }
		}
		
		
		
		return $result;
		
	} catch (Exception $ex) {
		watchdog_exception('listCustomAttributeAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addCustomAttributeAPI() - This function using for Create custom attributes through API
 */
function addCustomAttributeAPI(&$form_state, $createuserid="") {
	try {
		expDebug::dPrint('addCustomAttributeAPI form_state ==> ' . print_r($form_state, true) , 3);
		expDebug::dPrint('addCustomAttributeAPI createUserId ==> ' . print_r($createuserid, true) , 3);
		
		$form=array();
		//$error_messages = array();
		$form["form_id"]="";
		
		$attr_type = array('textbox'=>'cattr_type_txtbox', 'textarea'=>'cattr_type_txtarea', 'dropdown'=>'cattr_type_dropdown', 'radiobutton'=>'cattr_type_radio', 'checkbox'=>'cattr_type_checkbox');
		
		$attr_datatype = array('number'=>'cattr_dtype_numbers','text'=>'cattr_dtype_text');
		
		$attr_entitytype = array('course'=>'cre_sys_obt_crs', 'class'=>'cre_sys_obt_cls', 'trainingplan'=>'cre_sys_obt_trp', 'user'=>'cre_usr', 'organization'=>'cre_org', 'content'=>'cre_sys_obt_cnt', 'location'=>'cre_sys_obt_loc', 'order'=>'cre_sys_obt_ord');
		$attr_entitytypecode = array('cre_sys_obt_crs'=>'Course', 'cre_sys_obt_cls'=>'Class', 'cre_sys_obt_trp'=>'TrainingPlan', 'cre_usr'=>'User', 'cre_org'=>'Organization', 'cre_sys_obt_cnt'=>'Content', 'cre_sys_obt_loc'=>'Location', 'cre_sys_obt_ord'=>'Order');
		
		
		if(!empty($form_state['values']['type'])) {
			$type_split = explode(',',$form_state['values']['type']);
			expDebug::dPrint('addCustomAttributeAPI type_split ==> ' . print_r($type_split, true) , 3);
				
			foreach($type_split as $value) {
				$value = strtolower(str_replace(' ','',$value));
				expDebug::dPrint('addCustomAttributeAPI after trim ==> ' . print_r($value, true) , 3);
				if(!empty($attr_type[$value]))
					$type_concate[] = $attr_type[$value];
			}
			expDebug::dPrint('addCustomAttributeAPI type_concate ==> ' . print_r($type_concate, true) , 3);
			$form_state['values']['type'] = $type_concate[0];
			
			if(count($type_split)>1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Type field should not allowed more than 1 value";
				$error_messages[] = $errobj;
			}
			if(empty($type_concate)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid attribute type";
				$error_messages[] = $errobj;
			}
			
			$select_name = db_select('slt_custom_attr', 'cattr');
			$select_name->addField('cattr','cattr_name','attributename');
			$select_name->condition('cattr.cattr_name', $form_state['values']['name'],'=');
			$select_name->condition('cattr.cattr_type', $type_concate[0],'=');
			$select_name->condition('cattr.status', 'cre_cattr_sts_del','!=');
			expDebug::dPrintDBAPI('Select name query -->',$select_name);
			$result_name = $select_name->execute()->fetchAssoc();
			expDebug::dPrint(' result_name - '.print_r($result_name,true), 5);
			
			if($result_name['attributename'] != ''){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Attribute Name already exists";
				$error_messages[] = $errobj;
			}
		}
				
		if(!empty($form_state['values']['name']) && trim((is_numeric(substr($form_state['values']['name'],0,1))))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name should start with A-Z or a-z";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['name']) && trim(strlen($form_state['values']['name']))>30){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name should not exceed 30 characters";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['name']) && (!preg_match('/^[A-Za-z0-9 ]+$/', $form_state['values']['name']))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name accepts only the following characters:a-z A-Z 0-9 ";
			$error_messages[] = $errobj;
		}
		
		if(($form_state['values']['type'] == 'cattr_type_txtbox') && empty(trim($form_state['values']['datatype']))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datatype field Should not be empty";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['datatype']) && $form_state['values']['type'] == 'cattr_type_txtbox') {
			$datatype_split = explode(',',$form_state['values']['datatype']);
			expDebug::dPrint('addCustomAttributeAPI datatype_split ==> ' . print_r($datatype_split, true) , 3);
		
			foreach($datatype_split as $value) {
				$value = strtolower(str_replace(' ','',$value));
				if(!empty($attr_datatype[$value]))
					$datatype_concate[] = $attr_datatype[$value];
			}
			expDebug::dPrint('addCustomAttributeAPI datatype_concate ==> ' . print_r($datatype_concate, true) , 3);
			$form_state['values']['datatype'] = $datatype_concate[0];
		
			if(count($datatype_split)>1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "DataType field should not allowed more than 1 value";
				$error_messages[] = $errobj;
			}
			if(empty($datatype_concate)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid attribute Datatype";
				$error_messages[] = $errobj;
			}
		}
		
		if(($form_state['values']['type'] == 'cattr_type_txtbox' || $form_state['values']['type'] == 'cattr_type_txtarea') && empty(trim($form_state['values']['datalength']))){		
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datalength field Should not be empty";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['datalength']) && (!is_numeric($form_state['values']['datalength']) || ($form_state['values']['datalength']) < 1 || preg_match ("/[^0-9]/", $form_state['values']['datalength']))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datalength field accepts only numeric values";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['datalength']) && is_numeric($form_state['values']['datalength'])){
			if($datatype_concate[0]=='cattr_dtype_numbers' && $form_state['values']['datalength']>11){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "datalength should not exceed 11";
				$error_messages[] = $errobj;
			}
				
			if($datatype_concate[0]=='cattr_dtype_text' && $form_state['values']['datalength']>255){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "datalength should not exceed 255";
				$error_messages[] = $errobj;
			}
		
			if($form_state['values']['type']=='cattr_type_txtarea' && $form_state['values']['datalength']>500){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "datalength should not exceed 500";
				$error_messages[] = $errobj;
			}
		}		
		if(!empty($form_state['values']['mandatory'])){
			if(!in_array($form_state['values']['mandatory'],array('0','1'))){
			//if($form_state['values']['mandatory'] != 0 || $form_state['values']['mandatory'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "mandatory field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		if($form_state['values']['type'] == 'cattr_type_txtbox' && !empty($form_state['values']['attr_unique'])){
			if(!in_array($form_state['values']['attr_unique'],array('0','1'))){
			//if($form_state['values']['attr_unique'] != 0 || $form_state['values']['attr_unique'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "attr_unique field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		if(!empty($form_state['values']['help_txt']) && trim(strlen($form_state['values']['help_txt']))>255){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "help_txt should not exceed 255 characters.";
			$error_messages[] = $errobj;
		}
		if(!empty($form_state['values']['help_txt']) && (!preg_match('/^[A-Za-z0-9_ .-]+$/', $form_state['values']['help_txt']))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "help_txt accepts only the following characters:A-Z a-z 0-9 _.-";
			$error_messages[] = $errobj;
		}
		
		if($form_state['values']['type'] == 'cattr_type_dropdown' && !empty($form_state['values']['multiple_option'])){
			if(!in_array($form_state['values']['multiple_option'],array('0','1'))){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "multiple_option field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		if(($form_state['values']['type'] == 'cattr_type_radio' || $form_state['values']['type'] == 'cattr_type_checkbox') && !empty($form_state['values']['display_option'])){
			expDebug::dPrint('addCustomAttributeAPI display option ==> ' . print_r($form_state['values']['display_option'], true) , 3);
				
			//if($form_state['values']['display_option'] != 'H' || $form_state['values']['display_option'] != 'V'){
			
			$form_state['values']['display_option']=strtoupper(trim($form_state['values']['display_option']));
			if(!in_array($form_state['values']['display_option'],array('H','V'))){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "display_option field accepts only H or V";
				$error_messages[] = $errobj;
			}
		}
		
		if(($form_state['values']['type'] == 'cattr_type_dropdown' || $form_state['values']['type'] == 'cattr_type_radio' || $form_state['values']['type'] == 'cattr_type_checkbox') && empty($form_state['values']['opt_name'])){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "opt_name is required";
			$error_messages[] = $errobj;
		}		
		if(($form_state['values']['type'] == 'cattr_type_dropdown' || $form_state['values']['type'] == 'cattr_type_radio' || $form_state['values']['type'] == 'cattr_type_checkbox') && !empty($form_state['values']['opt_name'])){
			$options_split = explode(',',trim($form_state['values']['opt_name']));
				expDebug::dPrint('addCustomAttributeAPI options_split ==> ' . print_r($options_split, true) , 3);
			$form_state['values']['customoptions']['count'] = count($options_split);

			$r = 1;
			$dup_arr=array();
			$dupCount=0;
			$empty_val=0;
			//$optlen=0;
			$optcnt=0;
			$inv_opt_arr=array();
			$dup_opt_arr=array();
			foreach($options_split as $optkey=>$optvalue) { 

				if($optvalue==''){
					$empty_val++;
				}
				
				$form_state['values']['customoptions']['choices']['choice-text-'.$r]  = $optvalue; 
				
				if(in_array($form_state['values']['customoptions']['choices']['choice-text-' .$r],$dup_arr)){ 
					$dupCount++; 
					$dup_opt_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];
				}
				$dup_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];

				$optlen = strlen($form_state['values']['customoptions']['choices']['choice-text-' . $r]);
				if($optlen>25)
				{
					$optcnt++;
					$inv_opt_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];
				}
				$r++;
			}
			
			if(count($options_split)>10){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_names are not allowed more than 10.";
				$error_messages[] = $errobj;
			}
			//TO check empty value
			if($empty_val>0){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name is required";
				$error_messages[] = $errobj;
			}
			
			//To check duplicate options
			
			if($dupCount>0){
				$dup_opt_str=implode(',',$dup_opt_arr);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name ".$dup_opt_str." already exists";
				$error_messages[] = $errobj;
			}
			
			//To check options length
				
			if($optcnt>0){
				$inv_opt_str=implode(',',$inv_opt_arr);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name ".$inv_opt_str." should not exceed 25 characters.";
				$error_messages[] = $errobj;
			}
			
			expDebug::dPrint('addCustomAttributeAPI options assigned to form_state ==> ' . print_r($form_state, true) , 3); 
			
		}
		
		if(!empty($form_state['values']['entity_type'])) {
			$entitytype_split = explode(',',trim($form_state['values']['entity_type']));
			expDebug::dPrint('addCustomAttributeAPI entitytype_split ==> ' . print_r($entitytype_split, true) , 3);
		
			foreach($entitytype_split as $value) {
				$value = strtolower(str_replace(' ','',$value));
				if(!empty($attr_entitytype[$value]))
					$entitytype_concate[] = $attr_entitytype[$value];
			}
			expDebug::dPrint('addCustomAttributeAPI entitytype_concate ==> ' . print_r($entitytype_concate, true) , 3);
			//$form_state['values']['datatype'] = $entitytype_concate[0];
		
			if(empty($entitytype_concate)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid attribute entitytype";
				$error_messages[] = $errobj;
			}
			$entcnt = 0;
			$inv_ent_arr = array();
			foreach($entitytype_concate as $key=>$value){
				$select = db_select('slt_custom_attr_mapping', 'cmap');
				$select->innerJoin('slt_custom_attr', 'cattr','cmap.cattr_id=cattr.id');
				$select->addExpression('COUNT(distinct cmap.cattr_id)','entitytype_count');
				$select->condition('cmap.entity_type', $value,'=');
				$select->condition('cattr.status', 'cre_cattr_sts_del','!=');
				$select->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
				expDebug::dPrintDBAPI('Select entity_type query -->',$select);
				$result = $select->execute()->fetchAssoc();
				expDebug::dPrint(' $result - '.print_r($result,true), 5);
				
				if($result['entitytype_count']>=10)
				{
					$entcnt++;
					$inv_ent_arr[]=$attr_entitytypecode[$value];
				}				
			}
			if($entcnt>0){
				$dup_ent_str=implode(',',$inv_ent_arr);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "entity_types are not allowed more than 10 attributes for ".$dup_ent_str."";
				$error_messages[] = $errobj;
			}
		}


        if(!empty($form_state['values']['entity_type']) && (trim($form_state['values']['entity_screen_option']) == '' || trim($form_state['values']['entity_api_option'])=='')) {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Without set entity_screen_option or entity_api_option could not associate the entity_type";
            $error_messages[] = $errobj;
        } 
        
		
		if(empty($form_state['values']['entity_type']) && $form_state['values']['entity_screen_option'] != '') {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Without entity_type could not process entity_screen_option";
			$error_messages[] = $errobj;
		}
        
        /*if(empty($form_state['values']['entity_type']) && $form_state['values']['entity_screen_option'] != '') {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Without entity_type could not process entity_screen_option";
            $error_messages[] = $errobj;
        } */

        if(trim($form_state['values']['entity_type'])!='' && trim($form_state['values']['entity_screen_option'])!='' && trim($form_state['values']['entity_api_option'])!=''){   
            $entity_type_arr=explode(',',trim($form_state['values']['entity_type']));
            expDebug::dPrint('$entity_type_arr='.print_r($entity_type_arr,1),5);
            
            if(count($entity_type_arr)>0){
               // $valid_entity_arr=array('Course','Class','Training Plan','User','Organization','Content','Location','Order','Group');
                $valid_entity_arr=array('course','class','trainingplan','user','organization','content','location','order');
                $invalid_entity_str='';
                foreach($entity_type_arr as $entity_key=>$entity_name){
                     if(!in_array(strtolower($entity_name),$valid_entity_arr)){
                         $invalid_entity_str.=','.$entity_name;
                     }
                }
                
                if($invalid_entity_str!=''){
                        $invalid_entity_str = ltrim($invalid_entity_str, ',');
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Please enter valid entity type.Given the invalid entity type(s) ".$invalid_entity_str;
                        $error_messages[] = $errobj;
                     
                }
                
            }
            
            $count_entity_type=count($entity_type_arr);
            $count_entity_screen_option=count(explode(',',trim($form_state['values']['entity_screen_option'])));
            $count_entity_api_option=count(explode(',',trim($form_state['values']['entity_api_option']))); 
            
            if($count_entity_type!=$count_entity_screen_option){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Entity association is not proper with entity screen option.Please check the association with entity type and screen option";
                $error_messages[] = $errobj;
            }
            if($count_entity_type!=$count_entity_api_option){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Entity association is not proper with entity api option.Please check the association with entity type and api option";
                $error_messages[] = $errobj;
            }

            if($count_entity_type>0 && $count_entity_type==$count_entity_api_option && $count_entity_type==$count_entity_screen_option && $count_entity_api_option==$count_entity_screen_option){
                  
                  $tmp_screen_arr=explode(',',trim($form_state['values']['entity_screen_option']));                  
                  $tmp_screen_arr=explode(',',trim($form_state['values']['entity_screen_option']));
                  $tmp_api_arr=explode(',',trim($form_state['values']['entity_api_option']));
                  $invalid_api_screen_map_str='';
                  foreach($tmp_screen_arr as $scr_key=>$scr_val){
                      $tmp_scr_val=$tmp_screen_arr[$scr_key];
                      $tmp_api_val=$tmp_api_arr[$scr_key];
                      $tmp_entity_val=$entity_type_arr[$scr_key];
                      if($tmp_scr_val=='1' && $tmp_api_val=='0'){
                          $invalid_api_screen_map_str.=','.$tmp_entity_val;
                      } 
                      
                  }
                  if($invalid_api_screen_map_str!=''){
                        $invalid_api_screen_map_str = ltrim($invalid_api_screen_map_str, ',');
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Entity association is not proper for API. Please map 1 for API if set 1 for screen  to this entities  ".$invalid_api_screen_map_str;
                        $error_messages[] = $errobj;
                  }
                  
                  
              }
            
        } 		

		if($form_state['values']['entity_screen_option'] != '') {
			$entity_view_split = explode(',',$form_state['values']['entity_screen_option']);
			expDebug::dPrint('addCustomAttributeAPI entity_view_split ==> ' . print_r($entity_view_split, true) , 3);
									
			foreach($entity_view_split as $value) {
				expDebug::dPrint('addCustomAttributeAPI $entity_view_split $value==> ' . print_r($value, true) , 3);
				
				if(!in_array($value,array('0','1')))
				{
                $errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "entity_screen_option accepts only 0 or 1";
				$error_messages[] = $errobj;			
			}
		}
		}
		
		if(empty($form_state['values']['entity_type']) && $form_state['values']['entity_api_option'] != '') {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Without entity_type could not process entity_api_option";
			$error_messages[] = $errobj;
		}
		
		if($form_state['values']['entity_api_option'] != '') {
			$entity_edit_split = explode(',',$form_state['values']['entity_api_option']);
			expDebug::dPrint('addCustomAttributeAPI entity_edit_split ==> ' . print_r($entity_edit_split, true) , 3);
			foreach($entity_edit_split as $value) {
				
				expDebug::dPrint('addCustomAttributeAPI entity_edit_split $value==> ' . print_r($value, true) , 3);
				
				if(!in_array($value,array('0','1')))
				{
					$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "entity_api_option accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		}
		
		if($form_state['values']['entity_api_option'] == '' && $form_state['values']['entity_screen_option'] != '') {
			$entity_edit_split = explode(',',$form_state['values']['entity_screen_option']);
			expDebug::dPrint('addCustomAttributeAPI entity_edit_split even empty ==> ' . print_r($entity_edit_split, true) , 3);
		}
		
		if(!empty($form_state['values']['status']) && ($form_state['values']['status'] !='cre_cattr_sts_atv' && $form_state['values']['status'] !='cre_cattr_sts_itv')) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid status";
			$error_messages[] = $errobj;
		}
		
		expDebug::dPrint('addCustomAttributeAPI error_messages ==> ' . print_r($error_messages, true) , 3);
		
		if(count($error_messages) > 0) {
			$errors = new stdClass();
			$errors->isValidateError = 1;
			$errors->errors = $error_messages;
			return $errors;
		} else {
			include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
			$custom_attr_id = addNewCustomAttributeDetails($form, $form_state, $createuserid, 1);
			
			expDebug::dPrint('addCustomAttributeAPI custom_attr_id ==> ' . print_r($custom_attr_id, true) , 3);
			
    			if(!empty($form_state['values']['entity_type'])) {
    				
            			$i=0;
            			foreach($entitytype_concate as $key=>$value) {
            			
            				expDebug::dPrint('addCustomAttributeAPI foreach entitytype_concate - '.$i.' ==> ' . print_r($entitytype_concate, true) , 3);
            				expDebug::dPrint('addCustomAttributeAPI foreach key ==> ' . print_r($key, true) , 3);
            				expDebug::dPrint('addCustomAttributeAPI foreach value ==> ' . print_r($value, true) , 3);
            				expDebug::dPrint('addCustomAttributeAPI foreach entity_view_split ==> ' . print_r($entity_view_split, true) , 3);
            				expDebug::dPrint('addCustomAttributeAPI foreach entity_edit_split ==> ' . print_r($entity_edit_split, true) , 3);
            				
                            /*
            				$select = db_select('slt_custom_attr_mapping', 'cmap');
            				$select->innerJoin('slt_custom_attr', 'cattr','cmap.cattr_id=cattr.id');
            				$select->addExpression('MAX(cmap.entity_ref_tbl_col)','entitytype_column');
            				$select->condition('cmap.entity_type', $value,'=');
            				$select->condition('cattr.status', 'cre_cattr_sts_del','!=');
            				$select->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
            				expDebug::dPrintDBAPI('Select maximum entity_type column query -->',$select);
            				$result = $select->execute()->fetchAssoc();
            				expDebug::dPrint(' $result maximum entity_type column - '.print_r($result,true), 5);
            				
            				if(!empty($result['entitytype_column'])) {
            					$entitytype_column = substr($result['entitytype_column'],8);
            					expDebug::dPrint('addCustomAttributeAPI entitytype_column after split- '.print_r($entitytype_column,true), 5);
            					$entitytype_column = 'e1_cattr'.($entitytype_column + 1);
            				} else {
            					$entitytype_column = 'e1_cattr0';
            				}
            				expDebug::dPrint('addCustomAttributeAPI entitytype_column - '.print_r($entitytype_column,true), 5);
            				
                            */
                             
                             $ent_attr_map_code=$value;                       
                             $free_ent_arr=getFreeCustomFields($ent_attr_map_code);
                             $tmp_arr=array();
                             if(count($free_ent_arr[$ent_attr_map_code]['free_fields'])>0){
                                foreach($free_ent_arr[$ent_attr_map_code]['free_fields'] as $key => $val){
                                    $tmp_arr[]=$val;
                                }
                             }
                
                            expDebug::dPrint('$tmp_arr='.print_r($tmp_arr,true),5);
                             
                            $entitytype_column=$tmp_arr[0];           
                            
                            expDebug::dPrint('addCustomAttributeAPI entitytype_column - '.$ent_attr_map_code.'-'.print_r($entitytype_column,true), 5);                 
                             
            				if($entity_view_split[$i] != 0 || $entity_edit_split[$i] != 0) {
            					
            					if($entity_edit_split[$i] != '' && $entity_edit_split[$i] == 0 && $entity_view_split[$i] == 1)
            						$entity_edit_split[$i] = $entity_view_split[$i];
            				
            					$insertStmt = db_insert('slt_custom_attr_mapping');
            					 
            					$fields = array(
            							'cattr_id'                => $custom_attr_id[0]->id,
            							'entity_type'             => $value,
            							'entity_ref_tbl_col'      => $entitytype_column,
            							'entity_screen_opt'    	  => ($entity_view_split[$i] != '') ? $entity_view_split[$i] : 0,
            							'entity_api_opt'         => ($entity_edit_split[$i] != '') ? $entity_edit_split[$i] : 0
            							 
            					);
            					$insertStmt->fields($fields);
            					expDebug::dPrintDBAPI('addCustomAttributeAPI insert screen details -->',$insertStmt);
            					$screenId = $insertStmt->execute();
            					
            					make_audit_trail_entries($fields, $createuserid , null ,$custom_attr_id[0]->id, 'Inserted','cre_sys_obt_cattr','exp_sp_administration_customattribute','addNewScreenDetails',
            							array('cattr_id', 'entity_type', 'entity_ref_tbl_col', 'entity_screen_opt', 'entity_api_opt'));
            				}
            				$i++;
        			}
        			
        			if($screenId) {
        				//Update slt_custom_attr table
        				$attrFields['updated_by'] = $createuserid;
        				$attrFields['updated_on'] = now();
        				
        				$update = db_update('slt_custom_attr');
        				$update->fields($attrFields);
        				$update->condition('id', $custom_attr_id[0]->id,'=');
        				$update->execute();
        			}
    		}
		}
		
		return $custom_attr_id;
		
	} catch (Exception $ex) {
		watchdog_exception('addCustomAttributeAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * updateCustomAttributeAPI() - This function using for Update existing custom attributes through API
 */
function updateCustomAttributeAPI(&$form_state, $createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		
		//$attr_type = array('Text Box'=>'cattr_type_txtbox', 'Text Area'=>'cattr_type_txtarea', 'Drop Down'=>'cattr_type_dropdown', 'Radio Button'=>'cattr_type_radio', 'Check Box'=>'cattr_type_checkbox');
		
		$attr_datatype = array('number'=>'cattr_dtype_numbers','text'=>'cattr_dtype_text');
		
		$attr_entitytype = array('course'=>'cre_sys_obt_crs', 'class'=>'cre_sys_obt_cls', 'trainingplan'=>'cre_sys_obt_trp', 'user'=>'cre_usr', 'organization'=>'cre_org', 'group'=>'cre_sys_obt_grp', 'content'=>'cre_sys_obt_cnt', 'location'=>'cre_sys_obt_loc', 'order'=>'cre_sys_obt_ord');
		$attr_entitytypecode = array('cre_sys_obt_crs'=>'Course', 'cre_sys_obt_cls'=>'Class', 'cre_sys_obt_trp'=>'TrainingPlan', 'cre_usr'=>'User', 'cre_org'=>'Organization', 'cre_sys_obt_cnt'=>'Content', 'cre_sys_obt_loc'=>'Location', 'cre_sys_obt_ord'=>'Order');
		
		$select_name = db_select('slt_custom_attr', 'cattr');
		$select_name->addField('cattr','cattr_name','attributename');
		$select_name->condition('cattr.cattr_name', $form_state['values']['name'],'=');
		$select_name->condition('cattr.id', $form_state['values']['id'],'!=');
		$select_name->condition('cattr.status', 'cre_cattr_sts_del','!=');
		expDebug::dPrintDBAPI('Select name query -->',$select_name);
		$result_name = $select_name->execute()->fetchAssoc();
		expDebug::dPrint(' result_name - '.print_r($result_name,true), 5);
		
		$selectType = db_select('slt_custom_attr', 'cattr');
		$selectType->addField('cattr','cattr_type','attributetype');
		$selectType->condition('cattr.id', $form_state['values']['id'],'=');
		$selectType->condition('cattr.status', 'cre_cattr_sts_del','!=');
		expDebug::dPrintDBAPI('Select Type query -->',$selectType);
		$resultType = $selectType->execute()->fetchAssoc();
		expDebug::dPrint(' resultType - '.print_r($resultType,true), 5);
		
		$select1 = db_select('slt_custom_attr_mapping', 'cmap');
		$select1->innerJoin('slt_custom_attr', 'cattr','cmap.cattr_id=cattr.id');
		$select1->addField('cattr','cattr_length','cattr_length');
		$select1->addField('cattr','cattr_datatype','cattr_datatype');
		$select1->addField('cattr','cattr_mandatory','cattr_mandatory');
		$select1->addField('cattr','cattr_unique','cattr_unique');
		$select1->addField('cattr','cattr_multiple_opt','cattr_multiple_opt');
		$select1->addField('cattr','cattr_display_opt','cattr_display_opt');
		
		
		
		$select1->addExpression('COUNT(distinct cmap.cattr_id)','entitytype_count');
		$select1->condition('cattr.status', 'cre_cattr_sts_del','!=');
		$select1->condition('cmap.cattr_id', $form_state['values']['id'],'=');
		$select1->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
		expDebug::dPrintDBAPI('Select entity_type query -->',$select1);
		$result1 = $select1->execute()->fetchAssoc();
		expDebug::dPrint(' $result1 - '.print_r($result1,true), 5);
		
			
		if($result_name['attributename'] != ''){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name already exists";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['name']) && trim((is_numeric(substr($form_state['values']['name'],0,1))))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name should start with A-Z or a-z";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['name']) && trim(strlen($form_state['values']['name']))>30){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name should not have more than 30 characters";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['name']) && (!preg_match('/^[A-Za-z0-9_ ]+$/', $form_state['values']['name']))){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Attribute Name should not contain special characters";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['datatype']) && ($resultType['attributetype'] == 'cattr_type_txtbox')) {
			$datatype_split = explode(',',$form_state['values']['datatype']);
			expDebug::dPrint('updateCustomAttributeAPI datatype_split ==> ' . print_r($datatype_split, true) , 3);
		
			foreach($datatype_split as $value) {
				$value = strtolower(str_replace(' ','',$value));
				if(!empty($attr_datatype[$value]))
					$datatype_concate[] = $attr_datatype[$value];
			}
			expDebug::dPrint('updateCustomAttributeAPI datatype_concate ==> ' . print_r($datatype_concate, true) , 3);
			$form_state['values']['datatype'] = $datatype_concate[0];
		
			if($result1['entitytype_count'] >= 1 && $result1['cattr_datatype']!=$form_state['values']['datatype'])
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "DataType field cannot be modified now as there are active associations for this custom attribute.";
				$error_messages[] = $errobj;
			}
			
			if(count($datatype_split)>1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "DataType field should not allowed more than 1 value";
				$error_messages[] = $errobj;
			}
			if(empty($datatype_concate)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid attribute Datatype";
				$error_messages[] = $errobj;
			}
			
		}
		
		//if(($resultType['attributetype'] == 'cattr_type_txtbox' || $resultType['attributetype'] == 'cattr_type_txtarea') && (!empty($form_state['values']['datalength']) && $result1['entitytype_count'] >= 1) && $result1['cattr_length']!=$form_state['values']['datalength'])
		if(($resultType['attributetype'] == 'cattr_type_txtbox' || $resultType['attributetype'] == 'cattr_type_txtarea') && ((!empty($form_state['values']['datalength']) || $form_state['values']['datalength']==0) && $result1['entitytype_count'] >= 1) && $result1['cattr_length']!=$form_state['values']['datalength'])
		
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datalength field cannot be modified now as there are active associations for this custom attribute.";
			$error_messages[] = $errobj;
		}
		if(($resultType['attributetype'] == 'cattr_type_txtbox' || $resultType['attributetype'] == 'cattr_type_txtarea') && empty($form_state['values']['datalength'])){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datalength field Should not be empty";
			$error_messages[] = $errobj;
		}
		
		
		
		//if(!empty($form_state['values']['datalength']) && !is_numeric($form_state['values']['datalength'])){
			if(!empty($form_state['values']['datalength']) && (!is_numeric($form_state['values']['datalength']) || ($form_state['values']['datalength']) < 1 || preg_match ("/[^0-9]/", $form_state['values']['datalength']))){
					
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "datalength field accepts only numeric values";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['datalength']) && is_numeric($form_state['values']['datalength'])){
			if($datatype_concate[0]=='cattr_dtype_numbers' && $form_state['values']['datalength']>11){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "datalength should not have more than 11";
				$error_messages[] = $errobj;
			}
		
			if($datatype_concate[0]=='cattr_dtype_text' && $form_state['values']['datalength']>255){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "datalength should not have more than 255";
				$error_messages[] = $errobj;
			}
		}
		if((!empty($form_state['values']['mandatory']) || $form_state['values']['mandatory']==0) && $result1['entitytype_count'] >= 1 && $result1['cattr_mandatory']!=$form_state['values']['mandatory']){
		//if($result1['entitytype_count'] >= 1 && $result1['cattr_mandatory']!=$form_state['values']['mandatory']){
				//if($form_state['values']['mandatory'] != 0 || $form_state['values']['mandatory'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "mandatory field cannot be modified now as there are active associations for this custom attribute.";
				$error_messages[] = $errobj;
			
		}
		if(!empty($form_state['values']['mandatory'])){
			if(!in_array($form_state['values']['mandatory'],array('0','1'))){
			//if($form_state['values']['mandatory'] != 0 || $form_state['values']['mandatory'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "mandatory field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		
		if($resultType['attributetype'] == 'cattr_type_txtbox' && (!empty($form_state['values']['attr_unique']) || $form_state['values']['attr_unique']==0) && $result1['entitytype_count'] >= 1 && $result1['cattr_unique']!=$form_state['values']['attr_unique']){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "attr_unique field cannot be modified now as there are active associations for this custom attribute.";
				$error_messages[] = $errobj;
			
		}
		if($resultType['attributetype'] == 'cattr_type_txtbox' && !empty($form_state['values']['attr_unique'])){
			if(!in_array($form_state['values']['attr_unique'],array('0','1'))){
			//if($form_state['values']['attr_unique'] != 0 || $form_state['values']['attr_unique'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "attr_unique field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		if($resultType['attributetype'] == 'cattr_type_dropdown' && (!empty($form_state['values']['multiple_option']) || $form_state['values']['multiple_option']==0) && $result1['entitytype_count'] >= 1 && $result1['cattr_multiple_opt']!=$form_state['values']['multiple_option']){
				//if($form_state['values']['multiple_option'] != 0 || $form_state['values']['multiple_option'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "multiple_option field cannot be modified now as there are active associations for this custom attribute.";
				$error_messages[] = $errobj;
			
		}
		if($resultType['attributetype'] == 'cattr_type_dropdown' && !empty($form_state['values']['multiple_option'])){
			if(!in_array($form_state['values']['multiple_option'],array('0','1'))){
			//if($form_state['values']['multiple_option'] != 0 || $form_state['values']['multiple_option'] != 1){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "multiple_option field accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		
		if(($resultType['attributetype'] == 'cattr_type_radio' || $resultType['attributetype'] == 'cattr_type_checkbox') && !empty($form_state['values']['display_option']) && $result1['entitytype_count'] >= 1 && $result1['cattr_display_opt']!=$form_state['values']['display_option']){
			expDebug::dPrint('display option ==> ' . print_r($form_state['values']['display_option'], true) , 3);
		
			//if($form_state['values']['display_option'] != 'H' || $form_state['values']['display_option'] != 'V'){
				
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "display_option field cannot be modified now as there are active associations for this custom attribute.";
				$error_messages[] = $errobj;
			
		}
		if(($resultType['attributetype'] == 'cattr_type_radio' || $resultType['attributetype'] == 'cattr_type_checkbox') && !empty($form_state['values']['display_option'])){
			expDebug::dPrint('display option ==> ' . print_r($form_state['values']['display_option'], true) , 3);
				
			//if($form_state['values']['display_option'] != 'H' || $form_state['values']['display_option'] != 'V'){
			$form_state['values']['display_option']=strtoupper(trim($form_state['values']['display_option']));
			if(!in_array($form_state['values']['display_option'],array('H','V'))){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "display_option field accepts only H or V";
				$error_messages[] = $errobj;
			}
		}
		
		if(($resultType['attributetype'] == 'cattr_type_dropdown' || $resultType['attributetype'] == 'cattr_type_radio' || $resultType['attributetype'] == 'cattr_type_checkbox') && empty($form_state['values']['opt_name'])){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "opt_name field Should not be empty";
			$error_messages[] = $errobj;
		}
		
		if(!empty($form_state['values']['opt_name'])){
			$options_split = explode(',',trim($form_state['values']['opt_name']));
			expDebug::dPrint('updateCustomAttributeAPI options_split ==> ' . print_r($options_split, true) , 3);
			$form_state['values']['customoptions']['count'] = count($options_split);
		
			$r = 1;
			$dup_arr=array();
			$dupCount=0;
			$empty_val=0;
			$optcnt=0;
			$inv_opt_arr=array();
			$dup_opt_arr=array();
			foreach($options_split as $optkey=>$optvalue) {
				if($optvalue==''){
				  $empty_val++;
				}
				$form_state['values']['customoptions']['choices']['choice-text-'.$r]  = $optvalue;
				if(in_array($form_state['values']['customoptions']['choices']['choice-text-' .$r],$dup_arr)){
					$dupCount++;
					$dup_opt_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];						
				}				
				$dup_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];
				$optlen = strlen($form_state['values']['customoptions']['choices']['choice-text-' . $r]);				
				if($optlen>25)
				{
					$optcnt++;
					$inv_opt_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $r];
				}
				$r++;
			}
			
			if(count($options_split)>10){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_names are not allowed more than 10.";
				$error_messages[] = $errobj;
			}
			//TO check empty value
			if($empty_val>0){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name is required.";
				$error_messages[] = $errobj;
			}
			
			//To check duplicate options
			if($dupCount>0){
				$dup_opt_str=implode(',',$dup_opt_arr);				
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name ".$dup_opt_str." already exists";
				$error_messages[] = $errobj;
			}
			
			//To check options length
			
			if($optcnt>0){
				$inv_opt_str=implode(',',$inv_opt_arr);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "opt_name ".$inv_opt_str." should not exceed 25 characters.";
				$error_messages[] = $errobj;
			}
			expDebug::dPrint('updateCustomAttributeAPI options assigned to form_state ==> ' . print_r($form_state, true) , 3);
		}
		
		if(!empty($form_state['values']['entity_type'])) {
			$entitytype_split = explode(',',trim($form_state['values']['entity_type']));
			expDebug::dPrint('updateCustomAttributeAPI entitytype_split ==> ' . print_r($entitytype_split, true) , 3);
		
			foreach($entitytype_split as $value) {
				$value = strtolower(str_replace(' ','',$value));
				if(!empty($attr_entitytype[$value]))
					$entitytype_concate[] = $attr_entitytype[$value];
			}
			expDebug::dPrint('updateCustomAttributeAPI entitytype_concate ==> ' . print_r($entitytype_concate, true) , 3);
		
			if(empty($entitytype_concate)){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid attribute entitytype";
				$error_messages[] = $errobj;
			}
			$entcnt = 0;
			$inv_ent_arr = array();
			foreach($entitytype_concate as $key=>$value){
				$value = strtolower(str_replace(' ','',$value));
				$select = db_select('slt_custom_attr_mapping', 'cmap');
				$select->innerJoin('slt_custom_attr', 'cattr','cmap.cattr_id=cattr.id');
				$select->addExpression('COUNT(distinct cmap.cattr_id)','entitytype_count');
				$select->condition('cmap.entity_type', $value,'=');
				$select->condition('cattr.status', 'cre_cattr_sts_del','!=');
				$select->condition('cmap.cattr_id', $form_state['values']['id'],'!=');
				$select->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
				expDebug::dPrintDBAPI('Select entity_type query -->',$select);
				$result = $select->execute()->fetchAssoc();
				expDebug::dPrint(' $result - '.print_r($result,true), 5);
				if($result['entitytype_count']>=10)
				{
					$entcnt++;
					$inv_ent_arr[]=$attr_entitytypecode[$value];
				}
				
			}
			if($entcnt>0){
				$dup_ent_str=implode(',',$inv_ent_arr);
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "entity_types are not allowed more than 10 attributes for ".$dup_ent_str."";
				$error_messages[] = $errobj;
			}
		}

        if(!empty($form_state['values']['entity_type']) && (trim($form_state['values']['entity_screen_option']) == '' || trim($form_state['values']['entity_api_option'])=='')) {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Without set entity_screen_option or entity_api_option could not associate the entity_type";
            $error_messages[] = $errobj;
        }
		
		if(empty($form_state['values']['entity_type']) && $form_state['values']['entity_screen_option'] != '') {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Without entity_type could not process entity_screen_option";
			$error_messages[] = $errobj;
		}
         
        if(trim($form_state['values']['entity_type'])!='' && trim($form_state['values']['entity_screen_option'])!='' && trim($form_state['values']['entity_api_option'])!=''){   
            $entity_type_arr=explode(',',trim($form_state['values']['entity_type']));
            expDebug::dPrint('$entity_type_arr='.print_r($entity_type_arr,1),5);
            
            if(count($entity_type_arr)>0){
                //$valid_entity_arr=array('course','class','training plan','user','organization','content','location','order','group');
                $valid_entity_arr=array('course','class','trainingplan','user','organization','content','location','order');
                
                $invalid_entity_str='';
                foreach($entity_type_arr as $entity_key=>$entity_name){
                     if(!in_array(strtolower($entity_name),$valid_entity_arr)){
                         $invalid_entity_str.=','.$entity_name;
                     }
                }
                
                if($invalid_entity_str!=''){
                        $invalid_entity_str = ltrim($invalid_entity_str, ',');
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Please enter valid entity type.Given the invalid entity type(s) ".$invalid_entity_str;
                        $error_messages[] = $errobj;
                     
                }
                
            }
            
            $count_entity_type=count($entity_type_arr);
            $count_entity_screen_option=count(explode(',',trim($form_state['values']['entity_screen_option'])));
            $count_entity_api_option=count(explode(',',trim($form_state['values']['entity_api_option']))); 
            
            if($count_entity_type!=$count_entity_screen_option){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Entity association is not proper with entity screen option.Please check the association with entity type and screen option";
                $error_messages[] = $errobj;
            }
            if($count_entity_type!=$count_entity_api_option){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Entity association is not proper with entity api option.Please check the association with entity type and api option";
                $error_messages[] = $errobj;
            }
                
              if($count_entity_type>0 && $count_entity_type==$count_entity_api_option && $count_entity_type==$count_entity_screen_option && $count_entity_api_option==$count_entity_screen_option){
                  $tmp_screen_arr=explode(',',trim($form_state['values']['entity_screen_option']));                  
                  $tmp_screen_arr=explode(',',trim($form_state['values']['entity_screen_option']));
                  $tmp_api_arr=explode(',',trim($form_state['values']['entity_api_option']));
                  $invalid_api_screen_map_str='';
                  foreach($tmp_screen_arr as $scr_key=>$scr_val){
                      $tmp_scr_val=$tmp_screen_arr[$scr_key];
                      $tmp_api_val=$tmp_api_arr[$scr_key];
                      $tmp_entity_val=$entity_type_arr[$scr_key];
                      if($tmp_scr_val=='1' && $tmp_api_val=='0'){
                          $invalid_api_screen_map_str.=','.$tmp_entity_val;
                      } 
                      
                  }
                  if($invalid_api_screen_map_str!=''){
                        $invalid_api_screen_map_str = ltrim($invalid_api_screen_map_str, ',');
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Entity association is not proper for API. Please map 1 for API if set 1 for screen  to this entities  ".$invalid_api_screen_map_str;
                        $error_messages[] = $errobj;
                  }
                  
                  
              }
            
        }

		
		if($form_state['values']['entity_screen_option'] != '') {
			$entity_view_split = explode(',',$form_state['values']['entity_screen_option']);
			expDebug::dPrint('updateCustomAttributeAPI entity_view_split ==> ' . print_r($entity_view_split, true) , 3);
		foreach($entity_view_split as $value) {
				expDebug::dPrint('updateCustomAttributeAPI $entity_view_split $value==> ' . print_r($value, true) , 3);
			
				if(!in_array($value,array('0','1')))
				{
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "entity_screen_option accepts only 0 or 1";
					$error_messages[] = $errobj;
				}
			}
			}
				
		
		if(empty($form_state['values']['entity_type']) && $form_state['values']['entity_api_option'] != '') {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Without entity_type could not process entity_api_option";
			$error_messages[] = $errobj;
		}
		
		if($form_state['values']['entity_api_option'] != '') {
			$entity_edit_split = explode(',',$form_state['values']['entity_api_option']);
			expDebug::dPrint('updateCustomAttributeAPI entity_edit_split ==> ' . print_r($entity_edit_split, true) , 3);
		foreach($entity_edit_split as $value) {
				
				expDebug::dPrint('updateCustomAttributeAPI entity_edit_split $value==> ' . print_r($value, true) , 3);
				
				if(!in_array($value,array('0','1')))
				{
					$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "entity_api_option accepts only 0 or 1";
				$error_messages[] = $errobj;
			}
		}
		}
		
		if(!empty($form_state['values']['status']) && ($form_state['values']['status'] !='cre_cattr_sts_atv' && $form_state['values']['status'] !='cre_cattr_sts_itv')) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid status";
			$error_messages[] = $errobj;
		}
		
		expDebug::dPrint('updateCustomAttributeAPI error_messages ==> ' . print_r($error_messages, true) , 3);
		
		if(count($error_messages) > 0) {
			$errors = new stdClass();
			$errors->isValidateError = 1;
			$errors->errors = $error_messages;
			return $errors;
		} else {
			include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
			$update_custom_attrid = updateCustomAttributeDetails($form, $form_state, $createuserid,1);
			
			expDebug::dPrint('updateCustomAttributeAPI custom_attr_id ==> ' . print_r($update_custom_attrid, true) , 3);
			
			if(!empty($form_state['values']['entity_type'])) {
				include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_screen.inc');
                
                 //Get Previous mapping details of custom attribute 
                 $mapped_cus_attr_id=$form_state['values']['id'];
                 $prev_attr_map_arr=getEntityMappingDetails($mapped_cus_attr_id);
                 expDebug::dPrint(' $prev_attr_map_arr = '.print_r($prev_attr_map_arr,true), 5);
                
                
				//removing already mapped entities
				$deleteMapQuery = db_delete('slt_custom_attr_mapping')->condition('cattr_id', $form_state['values']['id']);
				expDebug::dPrintDBAPI('updateCustomAttributeAPI deleteQuery for mapped entities  = ', $deleteMapQuery); 
				$deleteMapQuery->execute();
				add_audit_trail_entry($createuserid, $form_state['values']['id'], 'Custom Attribute' , 'exp_sp_administration_customattribute', 'Screen', 'Deleting attribute screen mappings', null, null , null);
			
				$i=0;
				foreach($entitytype_concate as $key=>$value) {
						
					expDebug::dPrint('addCustomAttributeAPI foreach entitytype_concate - '.$i.' ==> ' . print_r($entitytype_concate, true) , 3);
					expDebug::dPrint('addCustomAttributeAPI foreach key ==> ' . print_r($key, true) , 3);
					expDebug::dPrint('addCustomAttributeAPI foreach value ==> ' . print_r($value, true) , 3);
					expDebug::dPrint('addCustomAttributeAPI foreach entity_view_split ==> ' . print_r($entity_view_split, true) , 3);
					expDebug::dPrint('addCustomAttributeAPI foreach entity_edit_split ==> ' . print_r($entity_edit_split, true) , 3);
			
					/*$select = db_select('slt_custom_attr_mapping', 'cmap');
					$select->innerJoin('slt_custom_attr', 'cattr','cmap.cattr_id=cattr.id');
					$select->addExpression('MAX(cmap.entity_ref_tbl_col)','entitytype_column');
					$select->condition('cmap.entity_type', $value,'=');
					$select->condition('cattr.status', 'cre_cattr_sts_del','!=');
					$select->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
					expDebug::dPrintDBAPI('Select maximum entity_type column query -->',$select);
					$result = $select->execute()->fetchAssoc();
					expDebug::dPrint(' $result maximum entity_type column - '.print_r($result,true), 5);
			
					if(!empty($result['entitytype_column'])) {
						$entitytype_column = substr($result['entitytype_column'],8);
						expDebug::dPrint('addCustomAttributeAPI entitytype_column after split- '.print_r($entitytype_column,true), 5);
						$entitytype_column = 'e1_cattr'.($entitytype_column + 1);
					} else {
						$entitytype_column = 'e1_cattr0';
					}*/
                     $ent_attr_map_code=$value;
                     $free_ent_arr=getFreeCustomFields($ent_attr_map_code);
                     $tmp_arr=array();
                     if(count($free_ent_arr[$ent_attr_map_code]['free_fields'])>0){
                        foreach($free_ent_arr[$ent_attr_map_code]['free_fields'] as $key => $val){
                            $tmp_arr[]=$val;
                        }
                     }
        
                    expDebug::dPrint('$tmp_arr='.print_r($tmp_arr,true),5);
                     
                    $entitytype_column=$tmp_arr[0];     
                    
                    expDebug::dPrint('updateCustomAttributeAPI entitytype_column - '.$ent_attr_map_code.'-'.print_r($entitytype_column,true), 5);        
					 
					if($entity_view_split[$i] != 0 || $entity_edit_split[$i] != 0) {
						
						if($entity_edit_split[$i] != '' && $entity_edit_split[$i] == 0 && $entity_view_split[$i] == 1)
							$entity_edit_split[$i] = $entity_view_split[$i];
			
						$insertStmt = db_insert('slt_custom_attr_mapping');
			
						$fields = array(
								'cattr_id'                => $update_custom_attrid[0]->id,
								'entity_type'             => $value,
								'entity_ref_tbl_col'      => $entitytype_column,
								'entity_screen_opt'    	  => ($entity_view_split[$i] != '') ? $entity_view_split[$i] : 0,
								'entity_api_opt'         => ($entity_edit_split[$i] != '') ? $entity_edit_split[$i] : 0
			
						);
						$insertStmt->fields($fields);
						expDebug::dPrintDBAPI('addCustomAttributeAPI insert screen details -->',$insertStmt);
						$screenId = $insertStmt->execute();
							
						make_audit_trail_entries($fields, $createuserid , null ,$update_custom_attrid[0]->id, 'Inserted','cre_sys_obt_cattr','exp_sp_administration_customattribute','addNewScreenDetails',
								array('cattr_id', 'entity_type', 'entity_ref_tbl_col', 'entity_screen_opt', 'entity_api_opt'));
					}
					$i++;
				}
					
				if($screenId) {
					//Update slt_custom_attr table
					$attrFields['updated_by'] = $createuserid;
					$attrFields['updated_on'] = now();
			
					$update = db_update('slt_custom_attr');
					$update->fields($attrFields);
					$update->condition('id', $custom_attr_id[0]->id,'=');
					$update->execute();
				}
                
                
                //Get current mapping details of custom attribute after save the new changes
                $cur_attr_map_arr=getEntityMappingDetails($mapped_cus_attr_id);
                expDebug::dPrint(' $cur_attr_map_arr = '.print_r($cur_attr_map_arr,true), 5);
                 
                if(count($prev_attr_map_arr)>0){
                    foreach($prev_attr_map_arr as $pre_key => $pre_val){
                        
                        if(!array_key_exists($pre_key,$cur_attr_map_arr)){
                            //Unmap respective coulumn 
                            expDebug::dPrint('There is no mapping for this entity ='.$pre_key.'--'.$pre_val, 5);
                            emptyEntityTableColumnValue($pre_key,$pre_val,$createuserid);
                        }else{
                            expDebug::dPrint(' Mapping is there= '.$pre_key.'--'.$pre_val, 5);
                        }
                        
                    }
                } 
                
			}
		}
		return $update_custom_attrid;
	} catch (Exception $ex) {
		watchdog_exception('updateCustomAttributeAPI', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function Custom_admin_search_csv_row_data_alter($rowData) {
  try {
  	//expDebug::dPrint('$rowData[0] = ' . print_r($rowData[0], true), 4);
    if (!empty($rowData[0]->date_activate)) {
      $rowData[0]->date_activate = date_format(date_create($rowData[0]->date_activate), 'Y-m-d');
    }
    if (!empty($rowData[0]->date_deactivate)) {
      $rowData[0]->date_deactivate = date_format(date_create($rowData[0]->date_deactivate), 'Y-m-d');
    }
    //expDebug::dPrint('$rowData[0] after alter = ' . print_r($rowData[0], true), 4);
  }
  catch (Exception $ex) {
    watchdog_exception('Custom_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function Custom_admin_search_pdf_row_data_alter($rowData) {
  try {
    //expDebug::dPrint('$rowData[0] = ' . print_r($rowData[0], true), 4);
    if (!empty($rowData[0]->date_activate)) {
      $rowData[0]->date_activate = dateTimeStrToUnivShort($rowData[0]->date_activate, true, false, false, false);
    }
    if (!empty($rowData[0]->date_deactivate)) {
      $rowData[0]->date_deactivate = dateTimeStrToUnivShort($rowData[0]->date_deactivate, true, false, false, false);
    }
    //expDebug::dPrint('$rowData[0] after alter = ' . print_r($rowData[0], true), 4);
  }
  catch (Exception $ex) {
    watchdog_exception('Custom_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

function getDefaultCustomAddEditFormFieldValue($field, $form_state, $announcementDetails) {
	try {
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
  	watchdog_exception('getDefaultCustomAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}
function CustomProfileListItems($codePrefix){
	try {
  //$statusOrder=array('cbn_anm_sts_atv','cbn_anm_sts_itv','cbn_anm_sts_dft');
  $select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code IN(\'cre_cattr_sts_atv\',\'cre_cattr_sts_itv\',\'cre_cattr_sts_del\') AND is_active = \'Y\' ORDER BY FIELD(CODE,\'cre_cattr_sts_atv\',\'cre_cattr_sts_itv\',\'cre_cattr_sts_del\')');
  $result = $select->fetchAllKeyed();
  expDebug::dPrintDBAPI(' addNewCustomByRestAPI created tp:' , $result );
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('CustomProfileListItems', $ex);
  	expertusErrorThrow($ex);
  }
}
function publishOrUnpublishCustom($customId){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {  		
	    $status  = getCustomStatus($customId);
	    $updateStmt = db_update('slt_custom_attr');
	    $updatedstatus  = ($status=='cre_cattr_sts_atv')? "cre_cattr_sts_itv" : "cre_cattr_sts_atv";  
	    
	    $updateStmt->condition('id', $customId);
	    $updateStmt->fields(array(
	                        'status'      => $updatedstatus,                                                   
	                        'updated_on'    => now(),
	                        'updated_by'    => $currUserId
	                        ));                       
	    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);
	
	    $updateStmt->execute();
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishLocationStatus', $ex);
    throw $ex;
  }
    unset($txn);
  add_audit_trail_entry($currUserId, $customId, 'cre_cattr_sts', 'exp_sp_administration_customattribute', 'publishOrUnpublishCustom', 'Updated customattribute Status',
			                                     null, $status, $updatedstatus);
	     
		  	$returnStatus  = ($updatedstatus=='cre_cattr_sts_atv')?t('LBL920'):t('LBL919');
		  	print $updatedstatus.'|'.$returnStatus;
}
/*function getCustomcount1($locationId) {
	try {
		$select = db_select('slt_person', 'per');
		$select->leftJoin('slt_custom_attr_mapping','cusatt','cusatt.entity_ref_tbl_col = per.e1_cattr0');
		$select->leftJoin('slt_custom_attr','cusatt1','cusatt1.id = cusatt.cattr_id');
		
		
		$select->addField('cusatt1','cattr_name','cattr_name');
		$select->condition('cusatt1.status','cre_cattr_sts_atv','=');
		$select->condition('cusatt1.id',$locationId);
		// Execute the query
		$result = $select->execute()->fetchAll();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('getCustomcountFromClass', $ex);
		expertusErrorThrow($ex);
	}
}*/
function getCustomStatus($customId) {
	try {
		$select = db_select('slt_custom_attr', 'cusatt');
		$select->addField('cusatt','status', 'status');
		$select->condition('cusatt.id',$customId);
		// Execute the query
		$locStatus = $select->execute()->fetchField();
		return $locStatus;
	} catch (Exception $ex) {
		watchdog_exception('getCustomStatus', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_viewCustom_addedit_form($js = NULL, $entityId){
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
								'width' => 650,
								'minHeight' => 400,
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
					array('id' => 'exp_sp_administration_view_Customdetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewCustom_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_view_Customdetails_addedit_form($form, &$form_state,$renderMode, $CustomId){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$courseId---->'.$CustomId);

		$wrapperId = 'viewCustom-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$Customdetails = getCustomDetailsInfoView($CustomId);

		expDebug::dPrint('$$$$Customdetails-->'.print_r($Customdetails,true),5);
		$form['narrow_Customview'] = array(
				'#markup'       => theme('narrow_Customview', array('record' => $Customdetails)),
		);

		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_Customdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getCustomattributeTypeFilters() - Get the status narrow search filters for custom attribute
 */
function getCustomattributeTypeFilters($langCode = 'cre_sys_lng_eng' , $alsoFetchInactiveItems = false) {
		try {
			$ent_type_arr1 = array();
			$ent_type_arr1['cattr_type_checkbox']=t('LBL2006');
			//$ent_type_arr1['cattr_type_date']=t('LBL042');
			$ent_type_arr1['cattr_type_dropdown']=t('LBL2007');
			$ent_type_arr1['cattr_type_radio']=t('LBL2008');
			$ent_type_arr1['cattr_type_txtarea']=t('LBL2010');
			$ent_type_arr1['cattr_type_txtbox']= t('LBL2009');
				
 return $ent_type_arr1;
  }
  catch(Exception $ex){
  watchdog_exception(
  'getContentStatusFilters', $ex);
  expertusErrorThrow($ex);
  }
  }
  /*function getCustomattributeScreenFilters($langCode = 'cre_sys_lng_eng' , $alsoFetchInactiveItems = false) {
  	try {
  		$ent_type_arr = array();
  		$ent_type_arr['cattr_ent_crs']=t('Course');
  		$ent_type_arr['cattr_ent_cls']=t('Class');
  		$ent_type_arr['cattr_ent_tp']=t('Training Plan');
  		$ent_type_arr['cre_usr ']=t('User');
  		$ent_type_arr['cattr_ent_org ']=t('Organization');
  		$ent_type_arr['cattr_ent_grp']=t('Group');
  		$ent_type_arr['cattr_ent_cont']=t('Content');
  		$ent_type_arr['cattr_ent_loc']=t('Location');
  		$ent_type_arr['cattr_ent_ord']=t('Order');
  
  		return $ent_type_arr;
  	}
  	catch(Exception $ex){
  		watchdog_exception(
  		'getContentStatusFilters', $ex);
  		expertusErrorThrow($ex);
  	}
  }*/
  function getProfileStatusItemNamesKeyed($codePrefix,$alsoFetchInactiveItems = false) {
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
  		$select->condition('spli.code', array('cre_cattr_sts_del')  , 'not in');
  		//$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  		if (!$alsoFetchInactiveItems) {
  			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  		}
  
  		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
  
  		$result = $select->execute()->fetchAllKeyed();
  		expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  		// Return results
  		return $result;
  	} catch (Exception $ex) {
  		watchdog_exception('getProfileStatusItemNamesKeyed', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  function getMandatory($codePrefix,$alsoFetchInactiveItems = false) {
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
  		$select->condition('spli.code', array('cre_sys_inv_man','cre_sys_inv_opt')  , 'in');
  		//$select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  		if (!$alsoFetchInactiveItems) {
  			$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  		}
  
  		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
  
  		$result = $select->execute()->fetchAllKeyed();
  		expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  		// Return results
  		return $result;
  	} catch (Exception $ex) {
  		watchdog_exception('ss', $ex);
  		expertusErrorThrow($ex);
  	}
  }

function getCustomAttributeValues($entity_type, $orderby=NULL){
	try {
		
		$entity_map_type = array('Course'=>'cre_sys_obt_crs', 'Class'=>'cre_sys_obt_cls', 'Training Plan'=>'cre_sys_obt_trp', 'User'=>'cre_usr', 'Organization'=>'cre_org', 'Group'=>'cattr_ent_grp', 'Content'=>'cre_sys_obt_cnt', 'Location'=>'cre_sys_obt_loc', 'Order'=>'cre_sys_obt_ord');
		$entity_type = $entity_map_type[$entity_type];
		
		$select = db_select('slt_custom_attr', 'cattr');
		$select->innerJoin('slt_custom_attr_mapping', 'cmap','cmap.cattr_id=cattr.id');
		$select->addField('cattr','cattr_name','attributename');
		$select->addField('cattr','cattr_type','attributetype');
		$select->addField('cattr','cattr_code','attributecode');
		$select->addField('cattr','cattr_datatype','attributedatatype');
		$select->addField('cattr','cattr_length','attributelength');
		$select->addField('cattr','cattr_mandatory','attributmandatory');
		$select->addField('cattr','cattr_unique','attributeunique');
		$select->addField('cattr','cattr_help_txt','attributehelptxt');
		$select->addField('cattr','cattr_seq','attributesequence');
		$select->addField('cmap','entity_ref_tbl_col','attributerefercolumn');
		$select->condition('cmap.entity_type', $entity_type,'=');
		$select->condition('cattr.status', 'cre_cattr_sts_atv','=');
		$select->condition('cmap.entity_api_opt', '1','=');
		if($orderby == 'mandatory')
			$select->orderBy('cattr.cattr_mandatory','DESC');
		else
			$select->orderBy('cmap.entity_ref_tbl_col');
		//$select->where('cmap.entity_screen_opt=1 or cmap.entity_api_opt=1');
		expDebug::dPrintDBAPI('Custom Attribute Select Query -->',$select);
		$result_customattr = $select->execute()->fetchAll();
		expDebug::dPrint('Custom Attribute Query Result - '.print_r($result_customattr,true), 5);
		
		return $result_customattr;
		
	} catch (Exception $ex) {
		watchdog_exception('getCustomAttributeValues', $ex);
		expertusErrorThrow($ex);
	}
}

function validateCustomAttribute($form_state,$entity_type) {
	try {
	expDebug::dPrint('validateCustomAttribute formstate ==> '.print_r($form_state,true), 5);
	expDebug::dprint('$entity_type = '.$entity_type,5);
	
	
	foreach($form_state['values'] as $ckey=>$cvalue){
		expDebug::dPrint('validateCustomAttribute foreach ==> '.print_r(strpos($ckey, 'c_'),true), 5);
		if(strpos($ckey, 'c_')===0) {
			$select_attr = db_select('slt_custom_attr', 'cattr');
			$select_attr->innerJoin('slt_custom_attr_mapping', 'cmap','cmap.cattr_id=cattr.id');
			$select_attr->leftJoin('slt_custom_attr_options', 'copt','copt.cattr_id=cattr.id');
			$select_attr->addField('cattr','cattr_name','attributename');
			$select_attr->addField('cattr','cattr_code','attributecode');				
			$select_attr->addField('cattr','cattr_type','attributetype');
			$select_attr->addField('cattr','cattr_multiple_opt','attrmultipleoptions');
			$select_attr->addField('cattr','cattr_datatype','attributedatatype');
			$select_attr->addField('cattr','cattr_length','attributelength');
			$select_attr->addField('cattr','cattr_unique','attributeunique');
			$select_attr->addField('cmap','entity_ref_tbl_col','attributerefercolumn');
			$select_attr->addExpression('group_concat(distinct copt.opt_name)','attributeoptions');
			$select_attr->condition('cattr.cattr_code', $ckey,'=');
			$select_attr->condition('cattr.status', 'cre_cattr_sts_atv','=');
			$select_attr->condition('cmap.entity_type', $entity_type, '=');
				
				expDebug::dPrintDBAPI('Validation Custom Attribute Select Query--> ',$select_attr);
			$result_attr = $select_attr->execute()->fetchAssoc();
				expDebug::dPrint('Validation Custom Attribute query Result--> '.print_r($result_attr,true), 5);
				expDebug::dPrint('Validation Custom Attribute value--> '.print_r(strlen($cvalue),true), 5);
			if($cvalue != ''){	
				if(($result_attr['attributedatatype'] == 'cattr_dtype_numbers') && !is_numeric($cvalue)){
					$error = $result_attr['attributename']." accept only numeric values";
					break;
				}
				if((($result_attr['attributedatatype'] != '' && $result_attr['attributetype'] == 'cattr_type_txtbox') || $result_attr['attributetype'] == 'cattr_type_txtarea') && (strlen(str_replace('.','',$cvalue)) > $result_attr['attributelength'])){
					$error = $result_attr['attributename']." length should not more than ".$result_attr['attributelength'];
					break;
				}
				/*if($result_attr['attributeunique']==1){
					$select_per = db_select('slt_person', 'per');
					$select_per->addExpression('count(per.id)','entityduplicate');
					$select_per->condition('per.'.$result_attr['attributerefercolumn'], $cvalue,'=');
					if($form_state['values']['id'] != '')
						$select_per->condition('per.id', $form_state['values']['id'],'!=');
						expDebug::dPrintDBAPI('Validation Entity duplicate Select Query--> ',$select_per);
					$result_per = $select_per->execute()->fetchAssoc();
						expDebug::dPrint('Validation Entity duplicate query Result--> '.print_r($result_per,true), 5);
					
					if($result_per['entityduplicate'] > 0){
						$error = $result_attr['attributename']." already exists";
						break;
					}
				}*/
				
				
				if($result_attr['attributeunique']==1){ //if it is unique value
					$att = $result_attr['attributecode'];
					
					expDebug::dprint('$att = '.$att,5);
					$upd_id=$form_state['values']['id'];
					 
					if($entity_type=='cre_sys_obt_loc'){
						$upd_id=$form_state['values']['loc_id'];
					}else if($entity_type=='cre_sys_obt_crs'){
						$upd_id=$form_state['values']['crs_id'];
					}
					
					$fieldValue = trim($form_state['values'][$att]);
					expDebug::dprint('$fieldValue = '.$fieldValue,5);
						
					if(!empty($result_attr['attributename'])) { 
						if (checkUniqueOrNot($fieldValue,$result_attr['attributerefercolumn'],$entity_type,$upd_id,'1')) {
							
							$error = $result_attr['attributename']." already exists";
							break;
								
						}
					}
				
				}
				if($result_attr['attributetype'] == 'cattr_type_checkbox' || $result_attr['attributetype'] == 'cattr_type_radio' || $result_attr['attributetype'] == 'cattr_type_dropdown'){
					$actualOptions = explode(',',$result_attr['attributeoptions']);
					expDebug::dPrint('Validation actual options --> '.print_r($actualOptions,true), 5);
					
					$inputOptions = explode(',',$cvalue);
					
					$inputOptionsCount = count($inputOptions);
					
					expDebug::dPrint('Validation input options --> '.print_r($inputOptions,true), 5);
					
					$matchOptionsCount = count(array_intersect($actualOptions, array_map('trim',$inputOptions)));
					
					expDebug::dPrint('Validation match options result --> '.print_r($matchOptionsCount,true), 5);
					
					if($result_attr['attributetype'] == 'cattr_type_radio' || ($result_attr['attributetype'] == 'cattr_type_dropdown' && ($result_attr['attrmultipleoptions'] == '' || $result_attr['attrmultipleoptions'] == 0))){
						if($inputOptionsCount > 1) {
							//$error = $result_attr['attributename']." accepts only one option";
							$error = "Incorrect value for " . $result_attr['attributename'];
							break;
						}
					}
					
					if($inputOptionsCount != $matchOptionsCount){
						$error = "Incorrect value for " . $result_attr['attributename'];
						//$error = $result_attr['attributename']." has invalid option";
						break;
					}
				}
			}
		}
	}
	return $error;
	} catch (Exception $ex) {
		watchdog_exception('validateCustomAttribute', $ex);
		expertusErrorThrow($ex);
	}
} 
function getCustomAttributeMappingColumn($code,$entity_type){
	try {
		$select_attr = db_select('slt_custom_attr', 'cattr');
		$select_attr->innerJoin('slt_custom_attr_mapping', 'cmap','cmap.cattr_id=cattr.id');
		$select_attr->addField('cmap','entity_ref_tbl_col','crefercolumn');
		$select_attr->condition('cattr.cattr_code', $code,'=');
		$select_attr->condition('cattr.status', 'cre_cattr_sts_atv','=');
		$select_attr->condition('cmap.entity_type', $entity_type,'=');
		
			expDebug::dPrintDBAPI('getCustomAttributeMappingColumn Select Query--> ',$select_attr);
		$result_attr = $select_attr->execute()->fetchAssoc();
			expDebug::dPrint('getCustomAttributeMappingColumn query Result--> '.print_r($result_attr,true), 5);
		return $result_attr;
	} catch (Exception $ex) {
		watchdog_exception('getCustomAttributeMappingColumn', $ex);
		expertusErrorThrow($ex);
	}
}
function moveArrayElement(&$array, $a, $b) {
	$p1 = array_splice($array, $a, 1);
	$p2 = array_splice($array, 0, $b);
	$array = array_merge($p2,$p1,$array);
	return $array;
}




/*
 * listCustomAttributeAPIForTranslation() - This function using for translation popup'
 */
function listCustomAttributeAPIForTranslation($start = NULL, $limit = NULL, $rows, $name, $type, $entity_type, $status, $userid) {
    try {
        
        $attr_type = array('textbox'=>'cattr_type_txtbox', 'textarea'=>'cattr_type_txtarea', 'dropdown'=>'cattr_type_dropdown', 'radiobutton'=>'cattr_type_radio', 'checkbox'=>'cattr_type_checkbox');
        
        $entity_map_type = array('course'=>'cre_sys_obt_crs', 'class'=>'cre_sys_obt_cls', 'trainingplan'=>'cre_sys_obt_trp', 'user'=>'cre_usr', 'organization'=>'cre_org', 'group'=>'cre_sys_obt_grp', 'content'=>'cre_sys_obt_cnt', 'location'=>'cre_sys_obt_loc', 'order'=>'cre_sys_obt_ord');
        
        
        //$attr_type = array('Text Box'=>'cattr_type_txtbox', 'Text Area'=>'cattr_type_txtarea', 'Drop Down'=>'cattr_type_dropdown', 'Radio Button'=>'cattr_type_radio', 'Check Box'=>'cattr_type_checkbox');
        
        //$entity_map_type = array('Course'=>'cre_sys_obt_crs', 'Class'=>'cre_sys_obt_cls', 'Training Plan'=>'cre_sys_obt_trp', 'User'=>'cre_usr', 'Organization'=>'cre_org','Content'=>'cre_sys_obt_cnt', 'Location'=>'cre_sys_obt_loc', 'Order'=>'cre_sys_obt_ord');
        
        // Select table
        $select = db_select('slt_custom_attr', 'cusatt');
        $select->leftJoin('slt_custom_attr_mapping','cusmap','cusmap.cattr_id=cusatt.id');
        $select->leftJoin('slt_custom_attr_options','cusopt','cusopt.cattr_id=cusatt.id');
        //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cusatt.id  and group_map.group_type = 1');
        //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
        //$select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_cattr\'');
        $select->distinct();
        $select->addField('cusatt', 'id', 'id');
        $select->addField('cusatt', 'cattr_name', 'name');
        //$select->addField('cusatt', 'cattr_type', 'type');
        
        $select->addExpression("CASE WHEN cusatt.cattr_type = 'cattr_type_txtbox' THEN 'Text Box' WHEN cusatt.cattr_type = 'cattr_type_txtarea' THEN 'Text Area' WHEN cusatt.cattr_type = 'cattr_type_dropdown'
	   THEN 'Drop Down'  WHEN cusatt.cattr_type = 'cattr_type_radio' THEN 'Radio Button'  WHEN cusatt.cattr_type = 'cattr_type_checkbox'  THEN 'Check Box'  ELSE cusatt.cattr_type END", "type");
        //$select->addField('cusatt', 'cattr_datatype', 'datatype');
        $select->addExpression("CASE WHEN cusatt.cattr_datatype = 'cattr_dtype_text' THEN 'Text' WHEN cusatt.cattr_datatype = 'Number' THEN 'Number' ELSE cusatt.cattr_datatype END", "datatype");
        $select->addField('cusatt', 'cattr_length', 'datalength');
        $select->addField('cusatt', 'cattr_mandatory', 'mandatory');
        $select->addField('cusatt', 'cattr_unique', 'attr_unique');
        $select->addField('cusatt', 'cattr_help_txt', 'help_txt');
        $select->addField('cusatt', 'cattr_multiple_opt', 'multiple_option');
        $select->addField('cusatt', 'cattr_display_opt', 'display_option');
        $select->addExpression('GROUP_CONCAT(DISTINCT cusopt.opt_name)', 'opt_name');
        $select->addExpression('GROUP_CONCAT(DISTINCT cusopt.id)', 'opt_ids');
        //$select->addField('cusmap', 'entity_type', 'entity_type');
        $select->addExpression('TRIM(BOTH \',\' FROM (GROUP_CONCAT(DISTINCT if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_crs","Course",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_cls","Class",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_trp","TP",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_usr","User",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_org","Organization",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cattr_ent_grp","Group",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_cnt","Content",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_loc","Location",""),\'\'),
		if(cusmap.entity_screen_opt = 1,if(cusmap.entity_type = "cre_sys_obt_ord","Order",""),\'\'))))', 'entity_type');
        $select->addField('cusmap', 'entity_screen_opt', 'entity_screen_option');
        $select->addField('cusmap', 'entity_api_opt', 'entity_api_option');
        $select->addField('cusatt', 'status', 'status');
        $select->groupBy('cusatt.id');
        
        if($status) {
            $status_split = explode(',',$status);
            $select->condition('cusatt.status', $status_split, 'IN');
        }
        
        if($name)
        {
            $name = strtolower(str_replace(' ','',$name));
            $select->condition('cusatt.cattr_name', '%' . db_like($name) . '%', 'LIKE');
        }
        
        if($type) {
            $type_split = explode(',',$type);
            
            foreach($type_split as $value) {
                $value = strtolower(str_replace(' ','',$value));
                $type_concate[] = $attr_type[$value];
            }
            
            $select->condition('cusatt.cattr_type', $type_concate, 'IN');
        }
        
        if($entity_type) {
            
            $entitytype_split = explode(',',$entity_type);
            
            foreach($entitytype_split as $value) {
                $value = strtolower(str_replace(' ','',$value));
                $entitytype_concate[] = $entity_map_type[$value];
            }
            
            $select->condition('cusmap.entity_type', $entitytype_concate, 'IN');
        }
        
        filterByUserAccessPrivileges($select, 'cusatt','',$userid,'cre_sys_obt_cattr','','cusatt');
        
        // Limit to $limit records
        $select->range($start, $limit);
        expDebug::dPrintDBAPI("listCustomAttributeAPI Select Query ==> ",$select);
        // Execute the query and fetch all Records
        $result = $select->execute()->fetchAll();
        expDebug::dPrint('listCustomAttributeAPI Final $result ==> ' . print_r($result, true) , 3);
        
        /*$config=getConfig('exp_sp');
        if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1"){
            $user =  getDrupalUserDataForTranslation($userid);
            expDebug::dPrint("drupalUserId:".$user->uid);
            
            $translationStringsForCustAttr = array();
            $translationStringsForCustAttrOptions = array();
            
            $translationids= array();
            expDebug::dPrint("count:".count($result));
            for($i = 0; $i < count($result);$i++) {
                $row = $result[$i];
                expDebug::dPrint("name:".$row->name);
                
                $title    =   $row->name;
                $helptext =   $row->help_txt;
                $opt_name =   $row->opt_name;
                
                $translationStringsForCustAttr[]=$title;
                $translationStringsForCustAttr[]=$helptext;
                
                if($opt_name != null && $opt_name != ""){
                    
                    $optarr = explode(",",$opt_name);
                    for($j = 0; $j < count($optarr); $j++ )
                        $translationStringsForCustAttrOptions[] =$optarr[$j];
                }
                
                $translationids[] = $row->id;
            }
            expDebug::dPrint(' $translatedLabels before api = ' . print_r($translationStringsForCustAttr, true) , 3);
            
            $translatedLabels = getTranslationsForArrayStrings($translationStringsForCustAttr,$user->language,$translationids,"cre_sys_obt_cattr");
            $translatedLabelsOptions = getTranslationsForArrayStrings($translationStringsForCustAttrOptions,$user->language,"","custom_attribute_options");
            
            expDebug::dPrint(' $translatedLabels after api  = ' . print_r($translatedLabels, true) , 3);
            expDebug::dPrint(' $translatedLabelsOptions after api  = ' . print_r($translatedLabelsOptions, true) , 3);
            
            for($i = 0; $i < count($result);$i++) {
                $result[$i]->name = strval($translatedLabels[$result[$i]->name]);
                $result[$i]->help_txt = strval($translatedLabels[$result[$i]->help_txt]);
                if($result[$i]->opt_name != null)
                    $opt_name = trim($result[$i]->opt_name) ;
                    
                    if( $opt_name != "")
                    {
                        
                        expDebug::dPrint('opt name===== ' . $opt_name , 3);
                        $optarr = explode(",",$opt_name);
                        $translatedOptName = "";
                        for($j = 0; $j < count($optarr); $j++ )
                        {
                            if($translatedOptName == "")
                                $translatedOptName = strval($translatedLabelsOptions[$optarr[$j]]);
                                else
                                    $translatedOptName .= ",".strval($translatedLabelsOptions[$optarr[$j]]);
                        }
                    }
                    $result[$i]->opt_name = $translatedOptName;
            }
        }
        */
        
        
        return $result;
        
    } catch (Exception $ex) {
        watchdog_exception('listCustomAttributeAPI', $ex);
        expertusErrorThrow($ex);
    }
}

?>

