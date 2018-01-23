<?php
/*
 * getModuleInfoTabContentType() .
*/

function getConfigTabContentType(){
	try {
		$jsObjectInfo['tab_content_type'] = 'narrow_search';
		return $jsObjectInfo;
	} catch (Exception $ex) {
		watchdog_exception('getConfigTabContentType', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * commerceSettingNarrowSearchFiltersHTML - Returns the html for narrow search filters
*/
function ConfigNarrowSearchFiltersHTML() {
	try {
		$renderfiltersets=array();
		$narrowfilters =  array();
		$filterSetforJS=$narrowfilters;

		
		
		$configInfo         = fetchConfigList();
		$countModule         = count($configInfo);
		$drupalImage    = array();
		$drupalExpertusName  = array();
		for($i=0;$i<$countModule;$i++) {
			$drupalImage[] =  $configInfo[$i]['imagepath'];
			$drupalExpertusName[] = $configInfo[$i]['display_name'];
		}
		expDebug::dPrint('$$drupalImage = '. print_r($drupalImage, true), 4);
		return theme('narrow_search_filters_holder',
				array('rendered_filtersets_list' => $renderfiltersets,
						'filter_set_list' => drupal_json_encode($filterSetforJS),
						'enable_option' => false,
				));
	} catch (Exception $ex) {
		watchdog_exception('ConfigNarrowSearchFiltersHTML', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * moduleInfoSearchResultsSortbarHTML - Returns the html for search results sort bar
*/
function ConfigSearchResultsSortbarHTML() {
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
		watchdog_exception('ConfigSearchResultsSortbarHTML', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * moduleInfoSearchResultsActionbarHTML - Returns the html for search results action bar
*/
function ConfigSearchResultsActionbarHTML() {
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
		watchdog_exception('ConfigSearchResultsActionbarHTML', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getModuleInfoAdminTabContentMainHTML - Returns the html for tab Module settings
*/
function getConfigAdminTabContentMainHTML() {
	try {
		$narrowsearchTabModuleListforJS = array(
				'search_base_path' =>'administration/sitesetup/config/search/all',
				'show_top_text_filter' => true,
				'text_filter_ac_path' => 'administration/sitesetup/config/textfilter-autocomplete'
		);

		return theme('narrow_search_tab_content',
				array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabModuleListforJS))
		);
	} catch (Exception $ex) {
		watchdog_exception('getConfigAdminTabContentMainHTML', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getModuleTabContent		  - Returns the html for narrow search block on mdule search page
*/
function getConfigTabContent() {
	try {
		expDebug::dPrint('enterkkk');
		$tabContentTypeArr = getConfigTabContentType();
			
		$moduleResponse = array(
				'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
				'rendered_narrow_search_filters'   => ConfigNarrowSearchFiltersHTML(),
				'rendered_narrow_search_sortbar'   => ConfigSearchResultsSortbarHTML(),
				'rendered_narrow_search_actionbar' => ConfigSearchResultsActionbarHTML(),
				'rendered_tab_content_main'        => getConfigAdminTabContentMainHTML(),
				'rendered_main_div'                => getAdminModules('administration','sitesetup'),
		);
		expDebug::dPrint('enterkkk222'.$moduleResponse);
		expDebug::dPrint('$$$moduleResponse = '. print_r($moduleResponse, true), 4);
		echo drupal_json_encode($moduleResponse);
	} catch (Exception $ex) {
		watchdog_exception('getConfigTabContent', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getConfigInfoDetailsHTML() - Renders a module detail as HTML to be displayed in jqGrid.  
 */
function getConfigInfoDetailsHTML($module) { 
 try {
 	$detailsList = array();
  $actionList[] = array('action_link_text' => t('LBL063'), //Edit
  					   					 'action_page_path' =>  $module->edit_url_path, //path',
					   						 'action_button_params' => '',//$module->expertus_module_name,
                         'action_button_param_text' => '',
  					    				 'ctools_style' => $module->ctools_styles,
  					     				 'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                        'tooltip' => t('LBL063')
					        );
  
  return theme('narrow-search-results-item-details',
  		array('title' => trim($module->display_name),
  				'details_list' => $detailsList,
  				'description'=> trim($module->display_description),
  				'delete_info' => t('MSG357').' '.strtolower(t('Tax').' '.t('MSG616')),
  				'action_list'=> $actionList
  		)
  );

} catch (Exception $ex) {
	watchdog_exception('getConfigInfoDetailsHTML', $ex);
	expertusErrorThrow($ex);
}
}

/*
 * getmoduleNameFilterAutoComplete() get the drupal module name in the auto-complete list
*/
function getConfigNameFilterAutoComplete() {
	try{
	  $searchText =  $_GET['z'];
		$moduleList = fetchConfigList(false);
		foreach($moduleList as $module) {
			if(stripos(trim($module['display_name']),$searchText) !== false ){
				print $module['display_name'] . "\n";
			}
		} 
	} catch (Exception $ex) {
		watchdog_exception('getConfigNameFilterAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * sendConfigListToJqGrid() - Prepares JSON response to JqGrid's request for module Admin
*/
function sendConfigListToJqGrid() {
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
  
 /* global $theme_key;
  if($theme_key == 'expertusoneV2') {
  	//NEWUI THEME STYLE SHEET
  	drupal_add_css(drupal_get_path('module', 'exp_sp_administration_config') .'/exp_sp_administration_config_v2.css', $css_theme_optional);
  } else {
  	drupal_add_css(drupal_get_path('module', 'exp_sp_administration_config') .'/exp_sp_administration_config.css', $css_theme_optional);
  }*/
  
  $page = 1;//$_GET['page']; // get the requested page
  $limit = 1;
  $configList = fetchConfigList();
  $numModule = count($configList);
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numModule);


  // Prepare and send the response
  $response->page = 1;
  $response->total = 1;
  $response->records = $numModule;

  $response->initial_sort_type_html_id = 'module-sort-new'; 
  $i=0;
  foreach ($configList as  $key=>$module) {  	
  	expdebug::dPrint('key--->'.$key);
  	expdebug::dPrint('$module--->'.$module);
    $module = array_to_object($module);
    $module->row =  $i;
    $module->object_type = 'config';
    //$module->imagepath = $module['imagepath'];
   // $module->display_description = t('Groups are created in the system so that we can group users in the system and provide them priveleges');
    $response->rows[$i]['class']   = 'admin-config-info';
    $response->rows[$i]['id']   = $i;
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $module)),
    																		  'details' => getConfigInfoDetailsHTML($module),
                                        );
    $i++;    
  }
  
  expDebug::dPrint('$response = '. print_r($response, true), 4);
  expDebug::dPrint('$response module info = '. print_r($module, true), 4);
  
		echo drupal_json_encode($response);
	} catch (Exception $ex) {
		watchdog_exception('sendConfigListToJqGrid', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * fetchConfigList() Get the Details of all config lists
*/
function fetchConfigList() {
	try {
		global $theme_key;
		expDebug::dPrint('$fetchConfigList simbu = ');
		$configInfo[] = array(
				'expertus_module_name' => 'exp_sp_administration_config_group',
				'display_name' => t('Group'), //Web Analytics
				'display_description' => t('Attributes linked to a group act as a criteria to group users under it. This module can be used to enable/disable attributes to be used as a criteria for this grouping.'), 
				'imagepath' => 'group-admin-icon',
				'ctools_styles' => 'ctools-modal-ctools-admin-group-style',
				'edit_url_path' => 'administration/sitesetup/config/group/nojs',
		);
		$configInfo[] = array(
				'expertus_module_name' => 'exp_sp_administration_config_currency',
				'display_name' => t('LBL101'), //Web Analytics
				'display_description' => t('Currency can be configured in the site to enable admin users to create trainings with different currency for users to purchase in their prefered currency'),
				'imagepath' => 'currency-admin-icon',
				'ctools_styles' => 'ctools-modal-ctools-admin-currency-style',
				'edit_url_path' => 'administration/sitesetup/config/currency/nojs',
		);
		return $configInfo;
	} catch (Exception $ex) {
		watchdog_exception('fetchConfigList', $ex);
		expertusErrorThrow($ex);
	}
}

?>