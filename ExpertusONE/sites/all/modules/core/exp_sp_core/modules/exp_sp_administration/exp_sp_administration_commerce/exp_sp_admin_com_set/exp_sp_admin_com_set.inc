<?php 
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getCommerceSettingTabContentType() . 
 */
 
function getCommerceSettingTabContentType(){
	try {
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getCommerceSettingTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * commerceSettingNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function commerceSettingNarrowSearchFiltersHTML() {
  try {
        $renderfiltersets=array();
        $narrowfilters =  array();
        $filterSetforJS=$narrowfilters;
      
     return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                           'enable_option' => false, 
                         ));  
  } catch (Exception $ex) {
    watchdog_exception('commerceSettingNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }   
}

/*
 * commerceSettingSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function commerceSettingSearchResultsSortbarHTML() {
  try{
  $sortbarLinksList = array();
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
  	watchdog_exception('commerceSettingSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
} 

/*
 * commerceSettingSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function commerceSettingSearchResultsActionbarHTML() {
   try {
  $actionbarLinksListforJS = array('export_path' => 'administration/commerce/setting/export/',
  					                'print_path' => 'administration/commerce/setting/print/'
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
  	watchdog_exception('commerceSettingSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCommercesettingAdminTabContentMainHTML - Returns the html for tab Commerce settings
 */
function getCommercesettingAdminTabContentMainHTML() { 
	try {    
   $narrowsearchTabSettingListforJS = array(
                                            'search_base_path' =>'administration/commerce/setting/search/all',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/commerce/setting/textfilter-autocomplete'                                            
                                          );                               
  
   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabSettingListforJS))
                              );
   } catch (Exception $ex) {
   	watchdog_exception('getCommercesettingAdminTabContentMainHTML', $ex);
   	expertusErrorThrow($ex);
   }
}
 
/*
 * getCommerceSettingTabContent		  - Returns the html for narrow search block on commerce settting search page
 */
function getCommerceSettingTabContent() {
	try {
  $tabContentTypeArr = getCommerceSettingTabContentType();
       
  $settingResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => commerceSettingNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => commerceSettingSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => commerceSettingSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getCommerceSettingAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','commerce'), 
  ); 
  echo drupal_json_encode($settingResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getCommerceSettingTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCommerceSettingDetailsHTML() - Renders a commerce detail as HTML to be displayed in jqGrid.  
 */
function getCommerceSettingDetailsHTML($setting) { 
	try {
  $detailsList = array();
  $actionList[] = array('action_link_text' => t('LBL063'), //Edit
  					    'action_page_path' => $setting->edit_url_path, //path',
					    'action_button_params' => '',//$setting->expertus_module_name,
  					    'ctools_style' => $setting->ctools_styles,
  					     'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                        'tooltip' => t('LBL063')
					        );
					 
  return theme('narrow-search-results-item-details',
                 array('title' => trim($setting->display_name),                        
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.strtolower(t('Tax').' '.t('MSG616')),
                       'description'=> trim($setting->display_description), 
                 	     'action_list'=> $actionList
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getCommerceSettingDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCommerceSettingActionsHTML() - Renders commerce setting actions as HTML to be displayed in jqGrid.  
 */
function getCommerceSettingActionsHTML($setting) {
	try {
  return theme('narrow-search-results-item-actions',
                     array('catalog_details' => $setting)
               ); 
  } catch (Exception $ex) {
  	watchdog_exception('getCommerceSettingActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendCommerceSettingListToJqGrid() - Prepares JSON response to JqGrid's request for commerce setting Admin
 */
function sendCommerceSettingListToJqGrid() {
  try {
  expDebug::dPrint(     'page = "' . $_GET['page'] . '" ,' .
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
  						          'argumentType = "' . $argumentType . '" ,' .
                        'npage = "' . $_GET['npage'], 5);
  
  $page = 1;//$_GET['page']; // get the requested page
  $limit = 1;
  $settingList = fetchCommerceSettingAll();
  $numSetting = count($settingList);
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numSetting);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);
  expDebug::dPrint('$$MouleList = ' . print_r($settingList, true), 4);  

  // Prepare and send the response
  $response= new stdClass();
  $response->page = 1;
  $response->total = 1;
  $response->records = $numSetting;

  $response->initial_sort_type_html_id = 'setting-sort-new'; 
  $i=0;
  foreach ($settingList as  $key=>$setting) {
    $setting = array_to_object($setting);
    $setting->row =  $i;
    $setting->object_type = 'commercesetting';
    $response->rows[$i]['id']   = $i;
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $setting)),
    									 'details' => getCommerceSettingDetailsHTML($setting),
                                         'action'  => getCommerceSettingActionsHTML($setting),
                                          
                                         );

    $i++;    
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);
  expDebug::dPrint('$response module info = '. print_r($setting, true), 4);
  
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendCommerceSettingListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * fetchCommerceSettingAll() - Fetches Commerce list / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchCommerceSettingAll() {
 try { 
 $settingInfoList = getSettingInfo();
  if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
    $settingInfoList = selectAddNarrowSearchSettingTextCondition($settingInfoList, $_GET['textfilter'], 'display_name');
  }
 return $settingInfoList;
 } catch (Exception $ex) {
 	watchdog_exception('fetchCommerceSettingAll', $ex);
 	expertusErrorThrow($ex);
 }
}

/*
 * selectAddNarrowSearchSettingTextCondition() Narrow filter option for textbox option
 */
function selectAddNarrowSearchSettingTextCondition($settinglist, $filterval, $filtersetCode) {
	try {
  if (!empty($filterval)){
    expDebug::dPrint(' narrowSearchSet = ' . print_r($narrowSearchSet, true) , 4);
    $countModule = count($settinglist);
    for($i=0; $i < $countModule; $i++) {
       $assignedVal = $settinglist[$i][$filtersetCode];
       if(stripos(trim($assignedVal),$filterval) === false ){       
          unset($settinglist[$i]);
       }
    }
  }
  expDebug::dPrint(' module list = ' . print_r($settinglist, true) , 4);
  return $settinglist;  
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchSettingTextCondition', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * getSettingInfo() Get the Details of all module lists
 */
function getSettingInfo() {
	try {
  global $theme_key;
  $countryIcon = ($theme_key == "expertusoneV2") ? 'admin-icon country' : 'country-setting';
  $settingInfo[] = array(
    'expertus_module_name' => 'exp_sp_administration_country_setting',
    'display_name' => t('LBL039').' '.t('LBL1021'),//'Country Setting'
    'display_description' => t('MSG661'),
    'imagepath' => $countryIcon,
    'ctools_styles' => 'ctools-modal-ctools-admin-country-setting-addedit-scroll-wrapper',
    'edit_url_path' => 'administration/commerce/setting/countrysetting/nojs/addedit',
  );
  
  $payIcon = ($theme_key == "expertusoneV2") ? 'admin-icon payment' : 'payment-method';
  $settingInfo[] = array(
    'expertus_module_name' => 'exp_sp_administration_payment_method',
    'display_name' => t('LBL548'),//Payment Method',
    'display_description' =>t('MSG662'), 
    'imagepath' => $payIcon,
    'ctools_styles' => 'ctools-modal-ctools-admin-payment-method-addedit-scroll-wrapper',
    'edit_url_path' => 'administration/commerce/setting/paymentmethod/nojs/addedit',
  );
  
  $cancellationIcon = ($theme_key == "expertusoneV2") ? 'admin-icon can-policy' : 'cancellation-policy';
  $settingInfo[] = array(
    'expertus_module_name' => 'exp_sp_administration_cancellation_policy',
    'display_name' => t('LBL1022'),//Cancellation Policy,
    'imagepath' => $cancellationIcon,
    'display_description' =>t('MSG663'), 
    'ctools_styles' => 'ctools-modal-ctools-admin-cancellation-policy-addedit-scroll-wrapper',
    'edit_url_path' => 'administration/commerce/setting/cancellationpolicy/nojs/addedit',
  );
  return $settingInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getSettingInfo', $ex);
  	expertusErrorThrow($ex);
  }
}



/*
 * getsettingNameFilterAutoComplete() get the setting name in the auto-complete list
 */
function getsettingNameFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];
  $settingList = getSettingInfo();
  foreach($settingList as $setting) {
    if(stripos(trim($setting['display_name']),$searchText) !== false ){
        print $setting['display_name'] . "\n";
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('getsettingNameFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
?>