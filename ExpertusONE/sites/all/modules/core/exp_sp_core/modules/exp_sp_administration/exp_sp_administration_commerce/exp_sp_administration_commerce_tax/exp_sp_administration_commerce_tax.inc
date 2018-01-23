<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getCommerceTaxTabContentType() - For tax setting admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getCommerceTaxTabContentType(){
	try {
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getCommerceTaxTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * commerceTaxNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function commerceTaxNarrowSearchFiltersHTML() {
  try {
    $checkedStatus = array();
    //$checkedStatus["lrn_lpn_sts_atv"] = TRUE;
    $allStatus = getProfileItemNamesKeyed('cme_tax_sts_');
    expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

    $renderfiltersets=array();

    if(count($allStatus) > 1){
      $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
     																array('code' => 'taxstatus',
                                                                     'title' => t('LBL102'),
      																 'divid' => 'status',
                                                                     'checkboxes_list' => $allStatus,
                                                                     'checked_list' => $checkedStatus)
      );
    }


    $renderfiltersets[]=theme('narrow_search_filterset_addltext',
    array('code' => 'tax_vatnumbers',
                                                                     'title' => t('LBL973'),//VAT No
                                                                     'default_text'=> t('LBL970') . ' ' . t('LBL973')//Type a VAT Number
    )
    ) ;
    $renderfiltersets[]=theme('narrow_search_filterset_addltext',
    array('code' => 'tax_productcode',
                                                                     'title' => t('LBL972'), //Product Code
                                                                     'default_text'=> t('LBL970') . ' '. t('LBL972') //Type a Product Code
    )
    ) ;
/*    $renderfiltersets[]=theme('narrow_search_filterset_addltext',
    array('code' => 'tax_country',
                                                                     'title' => t('LBL039'), //Country
                                                                     'default_text'=> t('LBL970') . ' '. t('LBL039')
    )
    ) ;*/

    /*$renderfiltersets[]=theme('narrow_search_filterset_addltext',
     array('code' => 'tax_states',
     'title' => t('LBL152'),
     'default_text'=> t('LBL970') . ' '. t('LBL152')
     )
     ) ;*/


    $narrowfilters =  array();
    $narrowfilters[] = array("type"=> "checkbox", "code"=>"taxstatus");
    $narrowfilters[] = array("type"=> "addltext", "code"=>"tax_vatnumbers", "acpath" =>"administration/commerce/tax/vat-autocomplete","defaultText"=>t('LBL970'). ' ' .t('LBL973'));
    $narrowfilters[] = array("type"=> "addltext", "code"=>"tax_productcode", "acpath" =>"administration/commerce/tax/procode-autocomplete","defaultText"=>t('LBL970') . ' '. t('LBL972'));
    //$narrowfilters[] = array("type"=> "addltext", "code"=>"tax_country", "acpath" =>"administration/commerce/tax/country-autocomplete","defaultText"=>t('LBL970') . ' '. t('LBL039'));
    //$narrowfilters[] = array("type"=> "addltext", "code"=>"tax_states", "acpath" =>"administration/commerce/tax/state-autocomplete","defaultText"=>t('LBL970') . ' '. t('LBL152'));
    $narrowfilters[] = array("type"=> "daterange","code"=> "daterange","from_tooltip"=> "Select From Date", "from_default_text" => t("LBL220"), "to_tooltip" => "Select To Date", "to_default_text" => t("LBL221"));

    $filterSetforJS=$narrowfilters;

    return theme('narrow_search_filters_holder',
    array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS), 
    ));
  } catch (Exception $ex) {
    watchdog_exception('commerceTaxNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * commerceTaxSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function commerceTaxSearchResultsSortbarHTML() {
try{
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'tax-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'tax-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'tax-sort-new');

  $defaultSortOrder = array('last_sort_type'=>'NewlyListed',
  					                   'last_sort_type_html_id'=>'tax-sort-new'
  					                   );

  					                   return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($defaultSortOrder), // Currently being ignored
                                              'title' => t('LBL011'),
  					                   )
  					                   );
} catch (Exception $ex) {
  	watchdog_exception('commerceTaxSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
 }		                    
}

/*
 * commerceTaxSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function commerceTaxSearchResultsActionbarHTML() {
  try{ 
  $actionbarLinksListforJS = array('export_path' => 'administration/commerce/tax/export/',
  					                       'print_path' => 'administration/commerce/tax/print/'
  					                       );
  					                       return theme('narrow_search_actionbar', array( 'add_button_title'   => t('LBL969'),
												                         'add_page_path'      => 'administration/commerce/tax/nojs/addedit',
												                         'show_print_button'  => true,
												                         'show_export_button' => true,
												                         'show_add_button'    => true,
                                        		     'ctools_style'       => 'ctools-modal-ctools-admin-tax-addedit-style',
                                                 'actionbar_list'     => drupal_json_encode($actionbarLinksListforJS),                                       
  					                       )
  					                       );
} catch(Exception $ex) {
 		watchdog_exception('commerceTaxSearchResultsActionbarHTML', $ex);
 		expertusErrorThrow($ex);
 }
}

/*
 * getCommerceTaxAdminTabContentMainHTML - Returns the html for tab tax settings
 */
function getCommerceTaxAdminTabContentMainHTML() {
	try{
  $narrowsearchTabTaxListforJS = array(
                                            'search_base_path' =>'administration/commerce/tax/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/commerce/tax/textfilter-autocomplete'                                            
                                            );

                                            return theme('narrow_search_tab_content',
                                            array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabTaxListforJS))
                                            );
 } catch(Exception $ex) {
    watchdog_exception('getCommerceTaxAdminTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
  }                                       
}

/*
 * getCommerceTabContent		  - Returns the html for narrow search block on tax search page
 */
function getCommerceTabContent() {
	try {
  $tabContentTypeArr = getCommerceTaxTabContentType();
   
  $commerceResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => commerceTaxNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => commerceTaxSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => commerceTaxSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getCommerceTaxAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','commerce'), 
  );
  echo drupal_json_encode($commerceResponse);
  } catch(Exception $ex) {
  	watchdog_exception('getCommerceTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCommerceTaxDetailsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getCommerceTaxDetailsHTML($tax) {
	try{
  $detailsList = array();
  if (!empty($tax->nexus) && $tax->states) {
    $states = getTaxStateName($tax->states,$tax->country_id);
    $stateChar = titleController('ADMIN-COMMERCE-TAX-STATENAME',t('LBL982'). ': '.$states, 30);
    $detailsList[] = array('detail' =>  $stateChar, 'tooltip' => t('LBL982').': '.$states);
  }
  if (!empty($tax->vat_numbers)) {
    $vat_numbers = titleController('ADMIN-COMMERCE-VAT-NUMBERS',t('LBL973'). ': '. $tax->vat_numbers, 30);
    $detailsList[] = array('detail' => $vat_numbers, 'tooltip' => t('LBL973').': '.sanitize_data($tax->vat_numbers));
  }

  $actionList[] = array('action_link_text' => t('LBL063'),
  					            'action_page_path' => 'administration/commerce/tax/nojs/addedit',
					              'action_button_params' => $tax->id,
  					            'ctools_style' => 'ctools-modal-ctools-admin-tax-addedit-style',
  					            'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                        'tooltip' => t('LBL063')
  );

  $actionList[] = array('action_link_text' => t('LBL286'),
  					            'action_page_path' => '',
					              'action_button_params' => $tax->id,
                        'action_button_param_text' => 'Tax',
  					            'ctools_style' => 'ctools-modal-ctools-admin-tax-addedit-style',
  					            'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                        'tooltip' => t('LBL286')
  );

  /* $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $tax->id,
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'entityId' => $tax->id,
  		'entityType' => 'cre_usr',
  		'icon_style_class'=>'access-tab-icon'
  ); */
  return theme('narrow-search-results-item-details',
  array('title' => trim($tax->country_name),
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.strtolower(t('Tax').' '.t('MSG616')),
                       'description'=> '', 
  									//	 'new_action_list'=> $newActionList,
                 	     'action_list'=> $actionList
  )
  );
  } catch(Exception $ex) {
  	watchdog_exception('getCommerceTaxDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getCommerceTaxActionsHTML() - Renders tax actions as HTML to be displayed in jqGrid.
 */
function getCommerceTaxActionsHTML($tax) {
	try {
  return theme('narrow-search-results-item-actions',
  array('catalog_details' => $tax)
  );
  } catch(Exception $ex) {
  	watchdog_exception('getCommerceTaxDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * commerceTaxSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function commerceTaxSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
      $field = "cntry" . ".country_name";
      $direction = "ASC";
      break;

    case "ZA" :
      $field = "cntry" . ".country_name";
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
  } catch(Exception $ex) {
  	watchdog_exception('commerceTaxSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendCommerceOrderListToJqGrid() - Prepares JSON response to JqGrid's request for Tax Admin
 */
function sendCommerceOrderListToJqGrid() {
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

  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];

/*   $numTax = fetchTaxAll('COUNT');
   
  expDebug::dPrint('$$numTax = ' . print_r($numTax, true), 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numTax); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);


  $taxList = fetchTaxAll('LISTS', $start, $limit);
  expDebug::dPrint('$$taxList = ' . print_r($taxList, true), 4);

  // Prepare and send the response
  $response= new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numTax;

  $response->initial_sort_type_html_id = 'tax-sort-new';

  $i=0;
  foreach ($taxList as $tax) {
    expDebug::dPrint('$tax = '. print_r($tax, true), 4);

    $tax->row =  $i;
    $tax->object_type = 'Tax';
    $response->rows[$i]['id']   = $tax->id;

    $response->rows[$i]['cell'] = array( 'details' => getCommerceTaxDetailsHTML($tax),
                                         'action'  => getCommerceTaxActionsHTML($tax)
    );

    $i++;
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);

  echo drupal_json_encode($response);
  } catch(Exception $ex) {
  	watchdog_exception('sendCommerceOrderListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * fetchTaxAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchTaxAll($op='', $start='', $limit='',$callFromAPI = 0) {
 try {
  // Select table
  $select = db_select('slt_tax_settings', 'taxs');
  //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=taxs.id AND group_map.entity_type = \'cme_tax\' and group_map.group_type = 1');
  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_profile_list_items', 'pli_status', 'taxs.status = pli_status.code AND pli_status.lang_code = \'cre_sys_lng_eng\' ');
  $select->leftJoin('slt_tax_product_code_mapping', 'pmap', 'pmap.tax_settings_id = taxs.id');
  $select->leftJoin('uc_countries', 'cntry', 'cntry.country_iso_code_2 = taxs.country');
  //$select->condition('taxs.status', array('cme_tax_sts_del'), 'NOT IN');

  if($op=='COUNT'){
    // Get count of active survey
    $select->addExpression('COUNT(DISTINCT(taxs.id))', 'COUNT');
  }
  else{
    // Select fields/expression values to be fetched from the database.
    $select->addField('taxs', 'id', 'id');
    $select->addField('taxs', 'country', 'country');
    $select->addField('cntry', 'country_id', 'country_id');
    $select->addField('cntry', 'country_name', 'country_name');
    $select->addField('taxs', 'states', 'states');
    $select->addField('taxs', 'nexus', 'nexus');
    $select->addField('taxs', 'vat_numbers', 'vat_numbers');
    $select->addField('pli_status', 'name', 'status');
    $select->addField('pli_status', 'code', 'status_code');
    $select->addField('pmap', 'delivery_type', 'delivery_type');
    $select->addField('pmap', 'tax_product_code', 'tax_product_code');
    $select->groupBy('taxs.id');
  }
  //Commented for not restricting the access privilege.
  //filterByUserAccessPrivileges($select, 'catacs','','group_map',1);
  
  if(isset($_GET['taxstatus']) && !empty($_GET['taxstatus'])){
    selectAddNarrowSearchINCondition($select, 'taxs', 'status', 'taxstatus');
  }
  if(isset($_GET['tax_vatnumbers']) && !empty($_GET['tax_vatnumbers'])){
    selectAddNarrowSearchByTextCondition($select, 'taxs', 'vat_numbers', 'tax_vatnumbers',t('LBL970') . ' '. t('LBL973'));
  }
  if(isset($_GET['tax_productcode']) && !empty($_GET['tax_productcode'])){
    selectAddNarrowSearchByTextCondition($select, 'pmap', 'tax_product_code', 'tax_productcode',t('LBL970') . ' '. t('LBL972'));
  }
  /*if(isset($_GET['tax_country']) && !empty($_GET['tax_country'])){
    selectAddNarrowSearchByTextCondition($select, 'cntry', 'country_name', 'tax_country',t('LBL970') . ' '. t('LBL039'));
  }*/
  /*if(isset($_GET['tax_states']) && !empty($_GET['tax_states'])){
   $select->leftJoin('uc_zones', 'zones', 'zones.zone_code IN(taxs.states)');
   selectAddNarrowSearchByTextCondition($select, 'zones', 'zone_name', 'tax_states');
   }*/
  if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
    selectAddNarrowSearchByTextMultipleORCondition($select, "cntry", array("country_name"));
  }
  $select->groupBy('taxs.id');
  if($op != "COUNT"){
    commerceTaxSearchSelectStmtAddOrderByClause($select, 'taxs');
  }
  expDebug::dPrintDBAPI('$select before count', $select);

  if($op == "COUNT"){
    $result = $select->execute()->fetchAll();
    expDebug::dPrint('COUNT $result 111= ' . print_r($result,true),4);
    return count($result);
  }
  elseif($op == "CSV"){
    $colHeadersMap = array(t('LBL039')     => 'country_name',
    t('LBL971')     => 'states',
    t('LBL973')     => 'vat_numbers',
    t('LBL102')     => 'status');

    $filterArray[] = array(t('Tax').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('taxstatus'));
    $filterArray[] = array(t('LBL973').':'.textboxFiltersetValueToPrintableStr('tax_vatnumbers'));
    $filterArray[] = array(t('LBL972').':'.textboxFiltersetValueToPrintableStr('tax_productcode'));
    //$filterArray[] = array(t('LBL039').':'.checkboxFiltersetSelectionsToPrintableStr('tax_country'));
    //$filterArray[] = array(t('LBL152').':'.checkboxFiltersetSelectionsToPrintableStr('tax_states'));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());

    SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'tax_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL039')     => array('country_name', 30),
    t('LBL971')     => array('states',25),
    t('LBL973')     => array('vat_numbers', 30),
    t('LBL102') 	   => array('status', 15));


    $appliedFilters = array(t('Tax') => textboxFiltersetValueToPrintableStr('textfilter'),
    t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('taxstatus'),
    t('LBL973') => textboxFiltersetValueToPrintableStr('tax_vatnumbers'),
    t('LBL972') => textboxFiltersetValueToPrintableStr('tax_productcode'),
    //t('LBL039') => textboxFiltersetValueToPrintableStr('tax_country'),
    //t('LBL152') => textboxFiltersetValueToPrintableStr('tax_states'),
    ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    );

    SendNarrowSearchResultsAsPDFFile($select, t('LBL969'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'tax_admin_search');
  }
  else {
  	add_row_id_condition($select, 'taxs');
    // Limit to $limit records
  	if (is_numeric($limit) && is_numeric($start)) {
    	$select->range($start, $limit);
    }
    expDebug::dPrintDBAPI('$select with range set', $select);
    // Execure the query and fetch all the records
    $result = $select->execute()->fetchAll();
    /* if($callFromAPI){	// total records count should be calculated irrespective of limit and start
    $totRecQry  = db_query('select found_rows() as tot_records');
    $count  = $totRecQry->fetchField();
    $result['count'] = $count;
    }*/
    expDebug::dPrint('LIST $result = ' . print_r($result,true) , 3);
    return $result;
  }
  } catch(Exception $ex) {
  	watchdog_exception('fetchTaxAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function tax_admin_search_csv_row_data_alter($rowData) {
  try {
    $rowData[0]->states = getTaxStateName($rowData[0]->states, $rowData[0]->country_id);
  }
  catch (Exception $ex) {
    watchdog_exception('tax_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function tax_admin_search_pdf_row_data_alter($rowData) {
  try {
    $rowData[0]->states = getTaxStateName($rowData[0]->states, $rowData[0]->country_id);
  }
  catch (Exception $ex) {
    watchdog_exception('tax_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fucntion to retrive the current status of the tax
 */
function getTaxStatus($taxId,$callForCountry = 0){
	try {
		$select = db_select('slt_tax_settings', 'taxset');
		if($callForCountry)
			$select->addField('taxset', 'country', 'country');
		else
			$select->addField('taxset', 'status', 'statusTax');
		$select->condition('taxset.id', $taxId);
		expDebug::dPrintDBAPI('$select', $select);
		// Execute the query
		$taxStatus = $select->execute()->fetchField();
		expDebug::dPrint('taxStatus = ' . $taxStatus , 3);
		return $taxStatus;
	} catch(Exception $ex) {
		watchdog_exception('getTaxStatus', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUCCountries() - Fetch country list.
 */
function getUCTaxCountries($defaultCountry){
  try{
    $select = db_select('uc_countries', 'countries');
    $select->addField('countries','country_iso_code_2');
    $select->addField('countries','country_name');
    $select->where("countries.country_iso_code_2  NOT IN (select tax.country from slt_tax_settings tax where tax.country != '".$defaultCountry."')");
    $select->orderBy('countries.country_name');
    expDebug::dPrintDBAPI('$select', $select);
    return $select->execute()->fetchAllKeyed();
  }catch (Exception $ex) {
    watchdog_exception('getUCTaxCountries', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getStatesForCountry($countryCode) - Fetch states list of a country
 */
function getStatesForUCTaxCountry($countryCode){
  try{
    expDebug::dPrint(' $countryCode = ' . print_r($countryCode, true),4);
    if (empty($countryCode) || ($countryCode != 'US' && $countryCode != 'CA')) {
      return array();
    }

    $select = db_select('uc_countries', 'uccon');
    $select->addField('uccon', 'country_id');
    $select->condition('uccon.country_iso_code_2', $countryCode);
    expDebug::dPrintDBAPI('$select', $select);
    $countryId = $select->execute()->fetchField();
    if(empty($countryId)) {
      return array();
    }
    $select = db_select('uc_zones', 'zone');
    $select->addField('zone', 'zone_code');
    $select->addField('zone', 'zone_name');
    $select->condition('zone.zone_country_id', $countryId, '=');
    $select->orderBy('zone.zone_name');
    $select->groupBy('zone.zone_name');

    expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);

    return $select->execute()->fetchAllKeyed();
  }catch (Exception $ex) {
    watchdog_exception('getStatesForUCTaxCountry', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTaxDetails($taxId)
 */
function getTaxDetails($taxId,$type = ''){
	try {
  $select = db_select("slt_tax_settings","taxset");
  $select->leftJoin('slt_tax_product_code_mapping', 'taxmap', 'taxmap.tax_settings_id = taxset.id');
  $select->addField('taxset','id', 'tax_id');
  $select->addField('taxset','country');
  $select->addField('taxset','states');
  $select->addField('taxset','nexus');
  $select->addField('taxset','status');
  $select->addField('taxset','vat_numbers');
  $select->addField('taxmap','id','tax_mapid');
  $select->addField('taxmap','tax_settings_id');
  $select->addField('taxmap','delivery_type');
  $select->addField('taxmap','tax_product_code');
  $select->addField('taxset','created_by');
  $select->addField('taxset','created_on');
  $select->condition('taxset.id',$taxId,'=');
  expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
  if($type == 'fetchall'){
    $taxList = $select->execute()->fetchAll();
  }
  else{
    $taxList = $select->execute()->fetchAssoc();
  }
  expDebug::dPrint(' $result = ' . print_r($taxList, true) , 3);
  return $taxList;
  } catch(Exception $ex) {
  	watchdog_exception('getTaxDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getTaxCountryName($taxId)
 */
function getTaxCountryName($iso2Code){
  if(empty($iso2Code)){
    return '';
  }
  try {
    $select = db_select('uc_countries', 'uccon');
    $select->addField('uccon', 'country_name');
    $select->condition('uccon.country_iso_code_2', $iso2Code);
    expDebug::dPrintDBAPI('$select', $select);
    $countryName= $select->execute()->fetchField();
    return $countryName;
  }catch (Exception $ex) {
    watchdog_exception('getTaxCountryName', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTaxStateName($taxId)
 */
function getTaxStateName($zoneCode,$countryId){
  if(empty($zoneCode)){
    return '';
  }
  $zoneArray = explode(',',$zoneCode);
  try {
    $select = db_select('uc_zones', 'zone');
    $select->addField('zone', 'zone_name');
    $select->condition('zone.zone_code', $zoneArray , 'IN');
    if($countryId)
    $select->condition('zone.zone_country_id', $countryId);
    expDebug::dPrintDBAPI('$select', $select);
    $zoneName= $select->execute()->fetchAll();
    $state = array();
    foreach($zoneName as $zone)
    {
      $state[] = $zone->zone_name;
    }
    return implode(', ',$state);
  }catch (Exception $ex) {
    watchdog_exception('getTaxStateName', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 * addNewTaxDetails() - New Tax Entry
 */
function addNewTaxDetails($form, &$form_state, $createUserId, $callFromAPI = 0){
	$txn = db_transaction();
	expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
	try {
		$insertStmt  	= db_insert('slt_tax_settings');
		$nfields      = taxFormatInputString($form, $form_state,$createUserId,$callFromAPI);
		$insertStmt->fields($nfields);
		expDebug::dPrintDBAPI('insertStmt', $insertStmt);
		$taxId 		= $insertStmt->execute();
		expDebug::dPrint('$taxId = ' . $taxId,4);
		$insertfields = $form_state['values'];

		make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$taxId, 'Inserted','Commerce','exp_sp_administration_commerce_tax','addNewTaxDetails',
				array('country','vat_numbers', 'status'));
		// Save the new tax id in the hidden id form field.
		$form_state['values']['id']     = $taxId;
		iudTaxProductCodeMapping($form_state,$createUserId,"AddTax");
		if($callFromAPI == 1){
			return array((object)array('id'=>$taxId));
		}
	} catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('addNewTaxDetails', $ex);
		if($callFromAPI){
			$errobj=new stdClass();
	    	$errobj->isValidateError = 1;
	    	$errobj->errcode = 'L_012';
	    	$errobj->errormsg = "Unable to create a tax.";
	    	return $errobj;
		}
		else {
			return array((object)array('id'=>'Failiure'));
		}
		throw $ex;
	}

	// Commit the transaction
	unset($txn);
}

/**
 * updateTaxDetails() - New Tax Entry
 */
function updateTaxDetails($form, &$form_state, $updateUserId, $callFromAPI = 0){
  $txn = db_transaction();
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
  try {
    $updateStmt = db_update('slt_tax_settings');
    $fields     = taxFormatInputString($form, $form_state,$updateUserId,$callFromAPI);
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['id']);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $numUpdated = $updateStmt->execute();
    $insertfields = $form_state['values'];
    iudTaxProductCodeMapping($form_state,$updateUserId,"UpdateTax");
    return array((object)array('id'=>$form_state['values']['id']));
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateTaxDetails', $ex);
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Unable to update a tax id";
    	return $errobj;
	}
	else {
		return array((object)array('id'=>'Failiure'));
	}
    throw $ex;
  }

  // Commit the transaction
  unset($txn);
}

function addNewTaxByRestAPI( &$form_state,$paramuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		expDebug::dPrint('$form_state : '.print_r($form_state , true), 4);
		$form_state = stripslashes_deep($form_state);
		if(!empty($form_state['values']['country'])){ // Lower To Upper case Conversion To Country.
			$form_state['values']['country'] = strtoupper($form_state['values']['country']);
		}
		if(!empty($form_state['values']['states'])){ // Lower To Upper case Conversion To states.
			$form_state['values']['states'] = strtoupper($form_state['values']['states']);
		}
		$getCountry = getCountryName($form_state['values']['country']);

		if(empty($getCountry)){ // Validate Country
			$error -> errormsg = 'Invalid Country Code.';
			$error -> isValidateError = true;
			return $error;
		}
		$fetched_All_Taxes = fetchTaxAll();
		$checkCountry = False;
		foreach ($fetched_All_Taxes as $fetched_tax){
			if($fetched_tax->country == $form_state['values']['country']){
				$checkCountry = True;
			}
		}
		if($checkCountry){ // Valdiate Country attached Previously
		    $error=new stdClass();
			$error -> errormsg = 'Tax already added to this Country.';
			$error -> isValidateError = true;
			return $error;
		}

		$StatesAllowed = array('US','CA');
		if(!in_array($form_state['values']['country'], $StatesAllowed) && !empty($form_state['values']['states'])){ // state availability
			$error -> errormsg = 'States only can add For United States or Canada.';
			$error -> isValidateError = true;
			return $error;
		}
		if(in_array($form_state['values']['country'], $StatesAllowed) && !empty($form_state['values']['states'])){ //
			$statename = getStateName($form_state['values']['states'], $form_state['values']['country']);
			if(empty($statename)){
				$error -> errormsg = 'Invalid State';
				$error -> isValidateError = true;
				return $error;
			}
		}
		$StatusAllowed = array('cme_tax_sts_itv','cme_tax_sts_atv');
		if(!in_array($form_state['values']['status'], $StatusAllowed)){ //
			$error -> errormsg = 'Invalid Status';
			$error -> isValidateError = true;
			return $error;
		}
		return addNewTaxDetails($form,$form_state,$paramuserid,1);
	} catch(Exception $ex) {
		watchdog_exception('addNewTaxByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateTaxByRestAPI( &$form_state,$paramuserid=""){
	$error= new stdClass();
	try {
		$form=array();
		$form["form_id"]="";
		expDebug::dPrint('$form_state : '.print_r($form_state , true), 4);
		$form_state = stripslashes_deep($form_state);
		if(!empty($form_state['values']['country'])){ // Lower To Upper case Conversion To Country.
			$form_state['values']['country'] = strtoupper($form_state['values']['country']);
		}
		if(!empty($form_state['values']['states'])){ // Lower To Upper case Conversion To states.
			$form_state['values']['states'] = strtoupper($form_state['values']['states']);
		}
		$taxidAvailable = getTaxStatus($form_state['values']['id']);
		expDebug::dPrint('$taxidAvailable : '.$taxidAvailable, 4);
		if(empty($taxidAvailable)){ // Validate Country
			$error -> errormsg = 'Tax id does not exist.';
			$error -> isValidateError = true;
			return $error;
		}
		$getCountry = getCountryName($form_state['values']['country']);

		if(empty($getCountry)){ // Validate Country
			$error -> errormsg = 'Invalid Country Code.';
			$error -> isValidateError = true;
			return $error;
		}
		$taxIdPresent = getTaxStatus($form_state['values']['id'],1);
		if($taxIdPresent != $form_state['values']['country']){ // Country Code Will Change
			$error -> errormsg = 'Country Code will not change.';
			$error -> isValidateError = true;
			return $error;
		}

		$StatesAllowed = array('US','CA');
		if(!in_array($form_state['values']['country'], $StatesAllowed) && !empty($form_state['values']['states'])){ // state availability
			$error -> errormsg = 'States only can add For United States or Canada.';
			$error -> isValidateError = true;
			return $error;
		}
		if(in_array($form_state['values']['country'], $StatesAllowed) && !empty($form_state['values']['states'])){
			$statename = getStateName($form_state['values']['states'], $form_state['values']['country']);
			if(empty($statename)){ // State Code Validation
				$error -> errormsg = 'Invalid State';
				$error -> isValidateError = true;
				return $error;
			}
		}
		$StatusAllowed = array('cme_tax_sts_itv','cme_tax_sts_atv');
		if(!in_array($form_state['values']['status'], $StatusAllowed)){ // Status Code Validate
			$error -> errormsg = 'Invalid Status';
			$error -> isValidateError = true;
			return $error;
		}
		return updateTaxDetails($form,$form_state,$paramuserid,1);
	} catch(Exception $ex) {
		watchdog_exception('updateTaxByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function listTaxByRestAPI( $start, $limit, $userid="")
{
	try{
  $form=array();
  $form["form_id"]="";
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  expDebug::dPrint('list api configuration for tax logic');
  $getlpQryRes = fetchTaxAll('',$start,$limit);
  //$lpQryRes = $getlpQryRes['result'];
//   $lpQryCnt = $getlpQryRes['count'];
  if(count($getlpQryRes)){
    $getlpQryRes['totalrow'] = fetchTaxAll('COUNT');
  }
  return $getlpQryRes;
  } catch(Exception $ex) {
  	watchdog_exception('listTaxByRestAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

function taxFormatInputString($form, &$form_state,$createUserId,$callFromAPI) {
	try {
  $nullVal           = NULL;
  $trigger_element = $form_state['storage']['submit_triggering_element'];
  $taxstatus = '';
  if($trigger_element == 'saveandpublish'){
    $taxstatus = 'cme_tax_sts_atv';
  }
  else if($trigger_element == 'save' || $trigger_element == 'unpublish'){
    $taxstatus = 'cme_tax_sts_itv';
  }
  else if($form_state['values']['status']){
    $taxstatus = $form_state['values']['status'];
  }
  $country = $form_state['values']['country'];
  $nexus = ($country == 'US' || $country == 'CA') ? 'yes' : $nullVal;
  $vat_numb = $form_state['values']['vat_number'];
  $vat_numbers = ($vat_numb) ? explode(',',$vat_numb) : $nullVal;
  if(count($vat_numbers)){
    foreach($vat_numbers as $key => $value){
      $vat_numbers[$key] = trim($value);
    }
    $vat_numbers = implode(',',$vat_numbers);
  }
  if($callFromAPI){
    $states = $form_state['values']['states'];
  }
  else{
    $states = implode(',',$form_state['values']['states']);
  }
  $fields = array(
                'country'     	     => $country,
                'states'             => $states,
                'nexus'              => $nexus,
                'status'             => $taxstatus,
                'vat_numbers'			   => $vat_numbers,                
                'custom0'            => $nullVal,
                'custom1'            => $nullVal,
                'custom2'            => $nullVal,
                'custom3'            => $nullVal,
                'custom4'            => $nullVal,  
  );
  if($form_state['values']['id']){
    $fields['updated_by'] = $createUserId;
    $fields['updated_on'] = now();
  }
  else{
    $fields['created_by'] = $createUserId;
    $fields['created_on'] = now();
    $fields['updated_by'] = $createUserId;
    $fields['updated_on'] = now();
  }
  expDebug::dPrint('$fields = ' . print_r($fields,true), 4);
  return $fields;
  } catch(Exception $ex) {
  	watchdog_exception('taxFormatInputString', $ex);
  	expertusErrorThrow($ex);
  }
}

function iudTaxProductCodeMapping($form_state,$createUserId,$mode){
	try {
  $delTypes = getProfileItemNamesKeyed('lrn_cls_dty_');
  $delTpTypes = getProfileItemNamesKeyed('cre_sys_obt_trp');
  $deliveryTypes = $delTypes + $delTpTypes;
  expDebug::dPrint(' delTypes = ' . print_r($deliveryTypes, true) , 4);
  $taxDetails = array();
  if($form_state['values']['id']){
    $taxDetails = getTaxDetails($form_state['values']['id'],'fetchall');
  }
  $taxId = $form_state['values']['id'];
  $taxDelDetails = array();
  foreach($taxDetails as $key => $val){
    $taxDelDetails[$val->delivery_type] = $val->tax_product_code;
  }
  expDebug::dPrint(' $taxDelDetails = ' .  print_r($taxDelDetails,true) , 4);
  foreach($deliveryTypes as $deltype => $value){
    $nullVal = NULL;
    $fields = array(
              'tax_settings_id'    => $taxId,
              'delivery_type'      => $deltype,
              'tax_product_code'   => trim($form_state['values'][$deltype]),
              'custom0'            => $nullVal,
              'custom1'            => $nullVal,
              'custom2'            => $nullVal,
              'custom3'            => $nullVal,
              'custom4'            => $nullVal,  
    );
    $taxProductCode = $taxDelDetails[$deltype];
    if(empty($taxProductCode) && $form_state['values'][$deltype]){
      $fields['created_by'] = $createUserId;
      $fields['created_on'] = now();
      $fields['updated_by'] = $createUserId;
      $fields['updated_on'] = now();
      expDebug::dPrint('$fields = ' . print_r($fields,true), 4);
      $insertTaxProductCodeMapping  	= db_insert('slt_tax_product_code_mapping');
      $insertTaxProductCodeMapping->fields($fields);
      expDebug::dPrintDBAPI('$insertTaxProductCodeMapping', $insertTaxProductCodeMapping);
      $mappingId 	= $insertTaxProductCodeMapping->execute();
    }
    else if($taxProductCode && $form_state['values'][$deltype]){
      $fields['updated_by'] = $createUserId;
      $fields['updated_on'] = now();
      expDebug::dPrint('$fields = ' . print_r($fields,true), 4);
      $mapId = taxProductMapId($taxId, $deltype);
      $updateStmt = db_update('slt_tax_product_code_mapping');
      $updateStmt->fields($fields);
      $updateStmt->condition('id', $mapId);
      expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
      $updateStmt->execute();
    }
    else if($taxProductCode && empty($form_state['values'][$deltype])){
      $mapId = taxProductMapId($taxId, $deltype);
      $deleteStmt = db_delete('slt_tax_product_code_mapping');
      $deleteStmt->condition('id', $mapId);
      expDebug::dPrintDBAPI('$updateStmt', $deleteStmt);
      $deleteStmt->execute();
    }
  }
  } catch(Exception $ex) {
  	watchdog_exception('iudTaxProductCodeMapping', $ex);
  	expertusErrorThrow($ex);
  }
}

function taxProductMapId($taxId,$deltype){
  try{
    $select = db_select('slt_tax_product_code_mapping', 'pmap');
    $select->addField('pmap', 'id');
    $select->condition('pmap.tax_settings_id', $taxId);
    $select->condition('pmap.delivery_type', $deltype);
    $select->range(0,1);
    expDebug::dPrintDBAPI('$select', $select);
    $taxProductMapId = $select->execute()->fetchField();
    return $taxProductMapId;
  }catch (Exception $ex) {
    watchdog_exception('taxProductMapId', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCommerceTextFilterAutoComplete() - Returns the matched vat
 */

function getCommerceTextFilterAutoComplete()
{
  try {
    $searchCurText =  $_GET['z'];
    // Select the table
    $select = db_select('slt_tax_settings', 'taxs');
    $select->leftJoin('uc_countries', 'cntry', 'cntry.country_iso_code_2 = taxs.country');
    // Add fields to fetch
    $select->addField('cntry', 'country_name');
    // Add condition
    $select->condition(db_or()
    ->condition('cntry.country_name', '%' . db_like($searchCurText) . '%', 'LIKE')
    ->condition('taxs.vat_numbers', '%' . db_like($searchCurText) . '%', 'LIKE')
    );
    // Order in dictionary sort order
    $select->groupBy('cntry.country_name');
    $select->orderBy('cntry.country_name');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $matching = $select->execute()->fetchAll();
    expDebug::dPrint(' $matching = '. print_r($matching, true) , 3);

    foreach ($matching as $vat) {
      print rawurlencode($vat->country_name) . "\n";
    }
  } catch (Exception $ex) {
    watchdog_exception('getCommerceTextFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCommerceVATFilterAutoComplete() - Returns the matched vat no  from slt_tax_settings table.
 */
function getCommerceVATFilterAutoComplete() {
  try{
    $searchText =  $_GET['z'];
    // Select the table
    $select = db_select('slt_tax_settings', 'tax');
    // Add fields to fetch
    $select->addField('tax', 'vat_numbers');
    // Add condition
    $select->condition('tax.vat_numbers', '%' . db_like($searchText) . '%', 'LIKE');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);

    $matchingVats = $select->execute()->fetchAll();
    expDebug::dPrint(' $$matchingVats = '. print_r($matchingVats, true),3);

    foreach ($matchingVats as $vats) {
      $vatArr = explode(',',$vats->vat_numbers);
      foreach($vatArr as $vat){
        if(stripos(trim($vat),$searchText) !== false ){
          print $vat . "\n";
        }
      }
    }
  }catch (Exception $ex) {
    watchdog_exception('getCommerceVATFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCommercePCodeFilterAutoComplete() - Returns the matched product code from slt_tax_product_code_mapping table.
 */
function getCommercePCodeFilterAutoComplete() {
  try{
    $searchText =  $_GET['z'];
    // Select the table
    $select = db_select('slt_tax_product_code_mapping', 'map');
    // Add fields to fetch
    $select->addField('map', 'tax_product_code');
    // Add condition
    $select->condition('map.tax_product_code', '%' . db_like($searchText) . '%', 'LIKE');
    $select->groupBy('map.tax_product_code');
    $select->orderBy('map.tax_product_code');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $matching = $select->execute()->fetchAll();
    expDebug::dPrint(' $$matching = '. print_r($matching, true),3);

    foreach ($matching as $pcode) {
      print $pcode->tax_product_code . "\n";
    }
  }catch (Exception $ex) {
    watchdog_exception('getCommercePCodeFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCommerceCountryFilterAutoComplete() - Returns the matched country from slt_tax_product_code_mapping table.
 */
function getCommerceCountryFilterAutoComplete() {
  try{
    $searchText =  $_GET['z'];
    // Select the table
    $select = db_select('uc_countries', 'cntr');
    // Add fields to fetch
    $select->addField('cntr', 'country_name');
    // Add condition
    $select->condition('cntr.country_name', '%' . db_like($searchText) . '%', 'LIKE');
    $select->orderBy('cntr.country_name');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $matching = $select->execute()->fetchAll();
    expDebug::dPrint(' $$matching = '. print_r($matching, true),3);

    foreach ($matching as $country) {
      print $country->country_name . "\n";
    }
  }catch (Exception $ex) {
    watchdog_exception('getCommerceCountryFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCommerceStateFilterAutoComplete() - Returns the matched state from slt_tax_product_code_mapping table.
 */
function getCommerceStateFilterAutoComplete() {
  try{
    $searchText =  $_GET['z'];
    // Select the table
    $select = db_select('uc_zones', 'zone');
    // Add fields to fetch
    $select->addField('zone', 'zone_name');
    // Add condition
    $select->condition('zone.zone_name', '%' . db_like($searchText) . '%', 'LIKE');
    $select->orderBy('zone.zone_name');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $matching = $select->execute()->fetchAll();
    expDebug::dPrint(' $$matching = '. print_r($matching, true),3);

    foreach ($matching as $zone) {
      print $zone->zone_name . "\n";
    }
  }catch (Exception $ex) {
    watchdog_exception('getCommerceStateFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the tax
 */
function publishOrUnpublishTax($taxId) {
  $currUserId = getSltpersonUserId();
  $status  = getTaxStatus($taxId);
  $updatedstatus  = ($status == 'cme_tax_sts_atv')? 'cme_tax_sts_itv' : 'cme_tax_sts_atv';
  $txn = db_transaction();
  try {
    $updateStmt = db_update('slt_tax_settings');
    $updateStmt->condition('id', $taxId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,                                                   
                        'updated_on'  => now(),
                        'updated_by'  => $currUserId
    ));
    $updateStmt->execute();
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishTax', $ex);
    throw $ex;
  }
  unset($txn); // Commit transaction

  add_audit_trail_entry($currUserId, $taxId, 'Commerce', 'exp_sp_administration_commerce_tax', 'publishOrUnpublishTax', 'Changed Tax Status',
  null, $status, $updatedstatus);

  $returnStatus  = ($updatedstatus=='cme_tax_sts_atv')? t('LBL572') : t('LBL573');
  print $updatedstatus . '|' . $returnStatus;

}
?>