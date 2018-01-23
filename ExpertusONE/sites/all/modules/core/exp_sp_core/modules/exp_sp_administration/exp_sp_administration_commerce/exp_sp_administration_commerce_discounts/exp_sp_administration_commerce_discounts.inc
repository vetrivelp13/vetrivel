<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
/*
 * getCommerceDiscountsTabContentType() - For discount setting admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getCommerceDiscountsTabContentType(){
        try{
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsTabContentType', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * commerceDiscountsNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function commerceDiscountsNarrowSearchFiltersHTML() {
  try {
      $checkedStatus = array();
      $allStatus = array('1' => t('Active'),'99' => t('Suspended'));

      $renderfiltersets=array();

      if(count($allStatus) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'discountStatus',
                                                                     'title' => t('LBL102'), //Status
                                                                         'divid' => 'status',
                                                                     'checkboxes_list' => $allStatus,
                                                                     'checked_list' => $checkedStatus)
                                                              );
      }

     $checkedType  = array();
     $discountType = discount_type_options();
     $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'discountType',
                                                                     'title' => t('LBL1110'), //Discount Type
                                                                         'divid' => 'discount_type',
                                                                     'checkboxes_list' => $discountType,
                                                                     'checked_list' => $checkedType)
                                                              );
     /*$checkedQualify = array();
     $qualifyType    = qualifying_type_options();
     $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'discountQualification',
                                                                     'title' => t('Qualification Type'), //Qualification Type
                                                                     'checkboxes_list' => $qualifyType,
                                                                     'checked_list' => $checkedQualify)
                                                              );*/
     $checkedObjType    = array();
     $classDeliveryType = getProfileItemNamesKeyed('lrn_cls_dty_');
     $tpDeliveryType    = fetchTPDeliveryTypes();
     $objectType = $classDeliveryType + $tpDeliveryType;
     $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'discountObjectType',
                                                                     'title' => t('LBL036'), //Type
                                                                         'divid' => 'type',
                                                                     'checkboxes_list' => $objectType,
                                                                     'checked_list' => $checkedObjType)
                                                              );

      $narrowfilters =  array();
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"discountStatus");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"discountType");
      //$narrowfilters[] = array("type"=> "checkbox", "code"=>"discountQualification");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"discountObjectType");
      $narrowfilters[] = array("type"=> "daterange","code"=> "daterange","from_tooltip"=> "Select From Date", "from_default_text" => t("LBL220"), "to_tooltip" => "Select To Date", "to_default_text" => t("LBL221"));

      $filterSetforJS=$narrowfilters;

     return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
  } catch (Exception $ex) {
    watchdog_exception('commerceDiscountsNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * commerceDiscountsSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function commerceDiscountsSearchResultsSortbarHTML() {
  try{
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'discount-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'discount-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'discount-sort-new');

  $defaultSortOrder = array('last_sort_type'=>'NewlyListed',
                                                           'last_sort_type_html_id'=>'discount-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($defaultSortOrder), // Currently being ignored
                                              'title' => t('LBL011'),
                                              )
              );
  }catch (Exception $ex) {
        watchdog_exception('commerceDiscountsSearchResultsSortbarHTML', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * commerceDiscountsSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function commerceDiscountsSearchResultsActionbarHTML() {
 try{
  $actionbarLinksListforJS = array('export_path' => 'administration/commerce/discounts/export/',
                                                               'print_path' => 'administration/commerce/discounts/print/'
                                  );
  return theme('narrow_search_actionbar', array( 'add_button_title'   => t('LBL1111'), //Create Discount
                                                                                                                         'add_page_path'      => 'administration/commerce/discounts/nojs/addedit',
                                                                                                                         'show_print_button'  => true,
                                                                                                                         'show_export_button' => true,
                                                                                                                         'show_add_button'    => true,
                                                             'ctools_style'       => 'ctools-modal-ctools-admin-discounts-addedit-style',
                                                 'actionbar_list'     => drupal_json_encode($actionbarLinksListforJS),
                                                                                                                                                                                         'page_code' => 'cme_dis',
                                           )
               );
  }catch (Exception $ex) {
        watchdog_exception('commerceDiscountsSearchResultsActionbarHTML', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * getCommerceDiscountsAdminTabContentMainHTML - Returns the html for tab discount settings
 */
function getCommerceDiscountsAdminTabContentMainHTML() {
        try{
   $narrowsearchTabDiscountListforJS = array(
                                            'search_base_path' =>'administration/commerce/discounts/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/commerce/discounts/textfilter-autocomplete'
                                          );

   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabDiscountListforJS))
                              );
   }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsAdminTabContentMainHTML', $ex);
        expertusErrorThrow($ex);
   }
}

/*
 * getCommerceDiscountsTabContent                 - Returns the html for narrow search block on discount search page
 */
function getCommerceDiscountsTabContent() {
        try{
  $tabContentTypeArr = getCommerceDiscountsTabContentType();

  $discountsResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => commerceDiscountsNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => commerceDiscountsSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => commerceDiscountsSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getCommerceDiscountsAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','commerce'),
  );
  echo drupal_json_encode($discountsResponse);
  }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsTabContent', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * getCommerceDiscountsDetailsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getCommerceDiscountsDetailsHTML($discount) {
try{
  $detailsList = array();
  if (!empty($discount->dis_type)) {
        $disType = discount_type_options();
    $stateChar = titleController('EXP-SP-ADMIN-COMMERCE-DISCOUNTS-TYPE',$disType[$discount->dis_type], 30);
    $detailsList[] = array('detail' => t('LBL1110'). ': '. $stateChar, 'tooltip' => t('LBL1110').': '.$disType[$discount->dis_type]);
  }
  $actionList[] = array('action_link_text' => t('LBL063'),
                                                    'action_page_path' => 'administration/commerce/discounts/nojs/addedit',
                                                      'action_button_params' => $discount->id,
                                                    'ctools_style' => 'ctools-modal-ctools-admin-discounts-addedit-style',
                                                    'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                        'tooltip' => t('LBL063')
                                                );
  $newActionList[] = array('action_link_text' => t('LBL642'),
                'action_page_path' => '',
                'action_button_params' => $discount->id,
                'action_button_params_text' => 'access',
                'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
                'js_object_info' => array (
                                'name' => 'narrowsearch', // provide the java script object name
                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                ),
                'tooltip' => t('LBL642'),
                'entityId' => $discount->id,
                'entityType' => 'cme_dis',
                'icon_style_class'=>'access-tab-icon',
                'postype' => 'topleft',
                'poslwid' => 40,
  );
  $discount->entityType = 'cme_dis';
  return theme('narrow-search-results-item-details',
                 array('title' => sanitize_data(trim($discount->dis_name)),
                       'details_list' => $detailsList,
                       'delete_info' => '',
                       'description'=> $discount->dis_desc,
                                         'new_action_list'=> $newActionList,
                             'view_list'=> $newActionList,
                                   //'entity_multi_action' => $discount,
                             'action_list'=> $actionList,
                                   'sumedit' => $discount->sumedit,
                                   'sumdelete' => $discount->sumdelete,
                      )
                );
  }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsDetailsHTML', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * getCommerceDiscountsActionsHTML() - Renders discount actions as HTML to be displayed in jqGrid.
 */
function getCommerceDiscountsActionsHTML($discount) {
        try{
  return theme('narrow-search-results-item-actions',
                     array('catalog_details' => $discount)
               );
  }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsActionsHTML', $ex);
        expertusErrorThrow($ex);
  }
}

/**
 * commerceDiscountsStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function commerceDiscountsStmtAddOrderByClause($selectStmt, $tableAlias) {
        try{
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".name";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias  . ".name";
                $direction = "DESC";
                break;

    case "NewlyListed" :
                $field = $tableAlias . ".insert_timestamp";
                $direction = "DESC";
                break;

    default:
                $field = $tableAlias . ".insert_timestamp";
                $direction = "DESC";
                break;
  }

  $selectStmt->orderBy($field, $direction);
  }catch (Exception $ex) {
        watchdog_exception('commerceDiscountsStmtAddOrderByClause', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * sendCommerceDiscountsOrderListToJqGrid() - Prepares JSON response to JqGrid's request for discount Admin
 */
function sendCommerceDiscountsOrderListToJqGrid() {
  try{
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

  /*$numDiscount = fetchDiscountsAll('COUNT');

  expDebug::dPrint('$numDiscount = ' . print_r($numDiscount, true), 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numDiscount); */
  
  $start = calculate_offset($page, $limit);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);


  $discountList = fetchDiscountsAll('LISTS', $start, $limit);
  expDebug::dPrint('discountList = ' . print_r($discountList, true), 4);

  // Prepare and send the response
  $response= new stdClass();
  $response->page = $page;
//   $response->total = $total_pages;
//   $response->records = $numDiscount;

  $response->initial_sort_type_html_id = 'discount-sort-new';

  $i=0;
  foreach ($discountList as $discount) {
    expDebug::dPrint('$discount = '. print_r($discount, true), 4);

    $discount->row =  $i;
    $discount->object_type = 'Discount';
    $response->rows[$i]['id']   = $discount->id;

    $response->rows[$i]['cell'] = array( 'details' => getCommerceDiscountsDetailsHTML($discount),
                                         'action'  => getCommerceDiscountsActionsHTML($discount)
                                         );

    $i++;
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);

  echo drupal_json_encode($response);
  }catch (Exception $ex) {
        watchdog_exception('getCommerceDiscountsActionsHTML', $ex);
        expertusErrorThrow($ex);
  }
}


/*
 * fetchDiscountsAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchDiscountsAll($op, $start='', $limit='', $fromApi = 0, $fields = array()) {
  try{
  // Select table
  $select = db_select('uc_discounts', 'dis');
  $select->leftJoin('uc_discounts_terms', 'terms', 'dis.discount_id = terms.discount_id');
  $select->leftJoin('slt_discounts', 'cdis', 'cdis.uc_discount_id = dis.discount_id');  
  $select->leftJoin('taxonomy_term_hierarchy', 'th', 'terms.term_id = th.tid');
  $select->leftJoin('taxonomy_term_data', 'td', 'terms.term_id = td.tid');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=dis.discount_id AND group_map.entity_type = \'cme_dis\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin = 1) and catacs.status =\'cre_sec_sts_atv\''); */
  if($op=='COUNT'){
     // Get count of Discount
     $select->addExpression('COUNT(DISTINCT(dis.discount_id))', 'COUNT');
  }
  else {
                // Select fields/expression values to be fetched from the database.
                if(empty($fields) ) {
                        $select->addField('dis', 'discount_id', 'id');
                        $select->addField('dis', 'description', 'dis_desc');
                        $select->addField('dis', 'is_active', 'dis_status');
                        $select->addField('dis', 'discount_type', 'dis_type');
                        $select->addField('dis', 'name', 'dis_name');
                        $select->addField('td', 'description', 'delivery_type');
                        $select->groupBy('dis.discount_id');
                }
                if($fromApi) {
//                      $select->leftJoin('uc_discounts_codes', 'cde', 'dis.discount_id= cde.discount_id');
                        foreach ($fields as $table_alias => $table_fields) {
                                foreach ($table_fields as $field => $alias) {
                                        $select->addField($table_alias, $field, $alias);
                                }
                        }
                }
  }

  //Commented for not restricting the access privilege.
  //filterByUserAccessPrivileges($select, 'catacs','',1,'group_map');
  filterByUserAccessPrivileges($select, 'dis','',1,'cme_dis','discount','cdis');  
  if(isset($_GET['discountStatus']) && !empty($_GET['discountStatus'])){
        $_GET['discountStatus'] = (int)($_GET['discountStatus']);
        $discountStatus = str_replace(99, 0, $_GET['discountStatus']);
    $narrowSearchStatus = explode("|", $discountStatus);
    $select->condition('dis.is_active', $narrowSearchStatus, 'IN');
    //selectAddNarrowSearchINCondition($select, 'dis', 'is_active', 'discountStatus');
  }
        if(isset($_GET['discountType']) && !empty($_GET['discountType'])){
                $_GET['discountType'] = (int)$_GET['discountType'];
    selectAddNarrowSearchINCondition($select, 'dis', 'discount_type', 'discountType');
  }
        /*if(isset($_GET['discountQualification']) && !empty($_GET['discountQualification'])){
    selectAddNarrowSearchINCondition($select, 'dis', 'status', 'discountQualification');
  }*/
        if(isset($_GET['discountObjectType']) && !empty($_GET['discountObjectType'])){
                $selectedFilters = escape_string($_GET['discountObjectType']);
          if (!empty($selectedFilters)){
                $orQuery = db_or();
            $narrowSearchSet = explode("|", $selectedFilters);
            $narroWIn = implode("','" , $narrowSearchSet);
            $orQuery->condition('td.description', $narrowSearchSet, 'IN');
            $orQuery->where('th.parent IN (select ttd.tid from taxonomy_term_data ttd where ttd.description IN(\''.$narroWIn.'\'))');
            $select->condition($orQuery);
          }
  }

  if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
    selectAddNarrowSearchByTextMultipleORCondition($select, "dis", array("name"));
  }
  $select->groupBy('dis.discount_id');
  if($op != "COUNT"){
    commerceDiscountsStmtAddOrderByClause($select, 'dis');
  }
  expDebug::dPrintDBAPI('$select before count', $select);

  if($op == "COUNT"){
        $result = $select->execute()->rowCount();
          expDebug::dPrint('COUNT $result 111= ' . print_r($result,true),4);
          return $result;
  }
  elseif($op == "CSV"){
          $colHeadersMap = array(t('Discount')     => 'dis_name',
                                 t('LBL036')     => 'dis_type',
                           t('LBL102')     => 'dis_status');

    $filterArray[] = array(t('Discount').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.discountFiltersetSelectionsToPrintableStr('discountStatus',$disType));
    $filterArray[] = array(t('Discount') . ' '. t('LBL036').':'.discountFiltersetSelectionsToPrintableStr('discountType',$disType));
    //$filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('discountQualification'));
    $filterArray[] = array(t('LBL036').':'.checkboxFiltersetSelectionsToPrintableStr('discountObjectType'));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());

        SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'discount_admin_search');
  }
  else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('Discount')     => array('dis_name', 30),
                           t('LBL036')     => array('dis_type',25),
                                                     t('LBL102')           => array('dis_status', 15));


    $appliedFilters = array(t('Discount') => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') => discountFiltersetSelectionsToPrintableStr('discountStatus',$disType),
                            t('Discount') . ' '. t('LBL036') => discountFiltersetSelectionsToPrintableStr('discountType',$disType),
                            //t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('discountQualification'),
                            t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('discountObjectType'),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    );

    SendNarrowSearchResultsAsPDFFile($select, t('Discount'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'discount_admin_search');// LBL969: Create Discount

  } else{
            add_row_id_condition($select, 'dis', 'discount_id');
            // Limit to $limit records
            if (is_numeric($limit) && is_numeric($start)) {
                $select->range($start, $limit);
            }
            expDebug::dPrintDBAPI('$select with range set', $select);
            // Execure the query and fetch all the records
            $result = $select->execute()->fetchAll();
            expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
            return $result;
  }
  }catch (Exception $ex) {
        watchdog_exception('fetchDiscountsAll', $ex);
        expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function discount_admin_search_csv_row_data_alter($rowData) {
  try {
    $disTypeOpt = discount_type_options();
    $rowData[0]->dis_type = $disTypeOpt[$rowData[0]->dis_type];
    $rowData[0]->dis_status = ($rowData[0]->dis_status)? t('Active') : t('Suspended');
  }
  catch (Exception $ex) {
    watchdog_exception('discount_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function discount_admin_search_pdf_row_data_alter($rowData) {
  try {
    $disTypeOpt = discount_type_options();
    $rowData[0]->dis_type = $disTypeOpt[$rowData[0]->dis_type];
    $rowData[0]->dis_status = ($rowData[0]->dis_status)? t('Active') : t('Suspended');
  }
  catch (Exception $ex) {
    watchdog_exception('discount_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function cbOrderFiltersetSelectionsToPrintableStr() - Returns the selected checkbox filters pipe separated
 *                                                  Assumed, all code is in
 */
function discountFiltersetSelectionsToPrintableStr($filtersetCode,$disType) {
        try{
  $selectedFilters = getURLParam($filtersetCode);
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
        if($filtersetCode == 'discountStatus'){
        $value = ($code == 99 ) ? str_replace(99, t('Suspended'), $code) : str_replace(1, t('Active'), $code);
        }
        else if($filtersetCode == 'discountType'){
                $value = $disType[$code];
        }
      if ($firstCode) {
        $selectedFiltersAsString .= $value;
        $firstCode = false;
      }
      else {
        $selectedFiltersAsString .= ' | ' . $value;
      }
    } // end foreach
  } // end if
  expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);
  return $selectedFiltersAsString;
  }catch (Exception $ex) {
        watchdog_exception('discountFiltersetSelectionsToPrintableStr', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * fucntion to retrive the current status of the discount
 */
function getDiscountStatus($discountId){
        try{
  $select = db_select('uc_discounts', 'dis');
  $select->addField('dis', 'is_active');
  $select->condition('dis.discount_id', $discountId);
  expDebug::dPrintDBAPI('$select', $select);
  // Execute the query
  $discountStatus = $select->execute()->fetchField();
  expDebug::dPrint('getDiscountStatus = ' . $discountStatus , 3);
  return $discountStatus;
  }catch (Exception $ex) {
        watchdog_exception('getDiscountStatus', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * fetchTPDeliveryTypes() - Fetches all delivery type list.
 */

function fetchTPDeliveryTypes() {
  try {
    // Select slt_profile_list_items table
    $select = db_select('slt_profile_list_items', 'splt');
    // Select fields/expression values to be fetched from the database.
    $select->addField('splt','code', 'code');
    $select->addField('splt','name', 'name');
    $select->condition(db_or()->
        condition('splt.code', 'cre_sys_obt_crt' , '=')->
        condition('splt.code', 'cre_sys_obt_cur' , '=')->
        condition('splt.code', 'cre_sys_obt_trn' , '='));
    // Order results by name
    $select->orderBy('name');
    expDebug::dPrintDBAPI(' $select object = ' , $select);
    $overallDeliveryList = $select->execute()->fetchAllKeyed();
    expDebug::dPrint('  $overallDeliveryList = ' . print_r( $overallDeliveryList, true) , 3);

    return $overallDeliveryList;
  } catch (Exception $ex) {
    watchdog_exception('fetchTPDeliveryTypes', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the Discount
 */
function publishOrUnpublishDiscounts($discountId) {
  $currUserId = getSltpersonUserId();
  $status  = getDiscountStatus($discountId);
  $updatedstatus  = ($status)? 0 : 1;
  $txn = db_transaction();
  try {
    $updateStmt = db_update('uc_discounts');
    $updateStmt->condition('discount_id', $discountId);
    $updateStmt->fields(array(
                        'is_active'      => $updatedstatus,
                        ));
    $updateStmt->execute();
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishDiscount', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn); // Commit transaction

  add_audit_trail_entry($currUserId, $discountId, 'Commerce', 'exp_sp_administration_commerce_discounts', 'publishOrUnpublishDiscount', 'Changed Discount Status',
                                        null, $status, $updatedstatus);

  $returnStatus  = ($updatedstatus)? t('LBL572') : t('LBL573');
  print $updatedstatus . '|' . $returnStatus;

}

/*
 * getDiscountTextFilterAutoComplete() - Returns the matched discount
 */

function getDiscountTextFilterAutoComplete()
{
  try {
      $searchCurText =  $_GET['z'];
      $select = db_select('uc_discounts', 'dis');
      $select->leftJoin('slt_discounts', 'cdis', 'cdis.uc_discount_id = dis.discount_id');      
      $select->addExpression('DISTINCT(dis.name)', 'name');
      $select->condition('dis.name', '%' . db_like($searchCurText) . '%', 'LIKE');
      $select->orderBy('dis.name');
      filterByUserAccessPrivileges($select, 'dis','',1,'cme_dis','discount','cdis');      
      expDebug::dPrintDBAPI(' $select obj = ' , $select);
      $matching = $select->execute()->fetchAll();
      expDebug::dPrint(' $matching = '. print_r($matching, true) , 3);
      foreach ($matching as $discount) {
        print rawurlencode($discount->name) . "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getDiscountTextFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getDiscountsDetails($discountId)
 */
function getDiscountsDetails($discountId,$type = ''){
        try{
  $select = db_select("uc_discounts","dis");
  $select->leftJoin('uc_discounts_codes', 'cde', 'dis.discount_id= cde.discount_id');
  $select->leftJoin('slt_discounts', 'sdis', 'sdis.uc_discount_id= dis.discount_id');
  $select->addField('dis','discount_id', 'id');
  $select->addField('dis','name');
  $select->addField('dis','description');
  $select->addField('dis','discount_type');
  $select->addField('dis','discount_amount');
  $select->addField('dis','max_uses');
  $select->addField('dis','max_uses_per_user');
  $select->addField('dis','max_uses_per_code');
  $select->addField('dis','qualifying_amount');
  $select->addField('dis','is_active');
  $select->addField('dis','can_be_combined_with_other_discounts','combine_discount');
  $select->addField('sdis','currency_type','currency_type');
  $select->addField('sdis','id','slt_discount_id');
  $select->addField('cde','code');
  $select->condition('dis.discount_id',$discountId,'=');
  expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
  if($type == 'fetchall'){
    $discountList = $select->execute()->fetchAll();
  }
  elseif ($type == 'count'){
        $discountList = $select->countQuery()->execute()->fetchField();
  }
  else{
    $discountList = $select->execute()->fetchAssoc();
  }
  expDebug::dPrint(' $result = ' . print_r($discountList, true) , 3);
  return $discountList;
  }catch (Exception $ex) {
        watchdog_exception('getDiscountsDetails', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 * getDiscountApplicableAllTrainings($discountId) Fetcing is applicable for all trainings or not. Return 0 or 1
 */
function getDiscountApplicableAllTrainings($discountId){
        try{
        $allTrainingTid     = variable_get('delivery_type_all_tid', 0);
        $allTPTrainingTid   = variable_get('tp_all_tid', 0);

  if(!variable_get('delivery_type_all_tid', 0)){
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
    $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
    $delivery_type_all_tid = ($delivery_type_all_tid) ? $delivery_type_all_tid : 0;
    variable_set('delivery_type_all_tid', $delivery_type_all_tid);
    $allTrainingTid = $delivery_type_all_tid;
  }
  if(!variable_get('tp_all_tid', 0)){
                $selectTPTip = db_select('taxonomy_term_data','termdata');
                $selectTPTip->addField('termdata', 'tid');
                $selectTPTip->condition('termdata.name', '--All Tp--');
                expDebug::dPrintDBAPI(' $selectTPTip = ' ,$selectTPTip);
                $allTPTrainingTid   = $selectTPTip->execute()->fetchField();
                variable_set('tp_all_tid', $allTPTrainingTid);
  }

        $select = db_select('uc_discounts_terms','term');
        $select->addExpression('COUNT(1)', 'mappedterm');
        $select->condition('term.discount_id', $discountId);
        $select->condition(db_or()
               ->condition('term.term_id', $allTrainingTid)
               ->condition('term.term_id', $allTPTrainingTid)
               );
        expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
        $isAllTraining = $select->execute()->fetchField();
  $isApplicable = ($isAllTraining == 2) ? 1 : 0;
        return $isApplicable;
        }catch (Exception $ex) {
                watchdog_exception('getDiscountApplicableAllTrainings', $ex);
                expertusErrorThrow($ex);
        }
}

/*
 * getDiscountApplicableAllUsers($discountId) Fetcing is applicable for all users or not. Return 0 or 1
 */
function getDiscountApplicableAllUsers($discountId , $returnType = ''){
        try{
        $select = db_select('uc_discounts_roles','roles');
        $select->addExpression('COUNT(1)', 'mappedroles');
        $select->condition('roles.discount_id', $discountId);
        $select->condition('roles.role_id', 100);
        expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
        $isAllRoles = $select->execute()->fetchField();
        if($returnType == 'roles'){
                return $isAllRoles;
        }
        $allOrganizationTid = variable_get('organization_all_tid', 0);
        $selectTerm = db_select('uc_discounts_terms','term');
        $selectTerm->addExpression('COUNT(1)', 'mappedterm');
        $selectTerm->condition('term.discount_id', $discountId);
        $selectTerm->condition('term.term_id', $allOrganizationTid);
        expDebug::dPrintDBAPI(' $select OBJ = ' ,$selectTerm);
        $isAllOrg = $selectTerm->execute()->fetchField();
  if($returnType == 'orgterm'){
                return $isAllOrg;
        }
        $isApplicable = ($isAllRoles && $isAllOrg) ? 1 : 0;
        return $isApplicable;
        }catch (Exception $ex) {
                watchdog_exception('getDiscountApplicableAllUsers', $ex);
                expertusErrorThrow($ex);
        }
}

function addEditDiscountCodeValidate(&$form_state,$discount_id,$callFromAPI=false) {
    expDebug::dPrint('$$updStmt----'.print_r($form_state,true),3);
  try {
    $select = db_select("uc_discounts_codes","cde");
    $select->addExpression('COUNT(1)', 'mappedcode');
    if(!empty($discount_id)) {
      $select->condition('cde.discount_id', $discount_id, '!=');
    }
    //Added ',' for #0069024 
   $newline = $callFromAPI ? ',' : "\n";
	$discont_codes = $callFromAPI ? explode($newline,strtolower(trim($form_state['values']['code']))): explode($newline,strtolower(trim($form_state['values']['dis_code'])));
        expDebug::dPrint('addeditdisc---'.print_r($discont_codes,true),5);
        $discont_codes = array_filter($discont_codes);
        $count_of_input_codes = count($discont_codes);
        
        $count_of_unique_codes = count(array_unique($discont_codes));
        
      if($count_of_input_codes != $count_of_unique_codes){
           $disCode =1;
//             if(!$callFromAPI){
//              form_set_error('dis_code', t('ERR131'));
//              $form_state["storage"]["form_validate"] = false;
//              $form_state['esign_show_popup']= false;
//               return;
//            }
//            else{
//                return false;
//            }
       }else{
            expDebug::dPrintDBAPI(' $select OBJ discont_codes = ' ,print_r($discont_codes,1),5);
 	$select->condition('cde.code ', $discont_codes,'IN');
        expDebug::dPrintDBAPI(' $select OBJ = ' ,$select);
	 $disCode = $select->execute()->fetchField();
       }
       
       
         
    if($disCode) {
        if(!$callFromAPI){
            form_set_error('dis_code', t('ERR131'));
            $form_state["storage"]["form_validate"] = false;
            $form_state['esign_show_popup']= false;
        }
       else
        {
            return false;
        }
    }
   
    else if($disCode==0){
         
        return true;
    }
     
    }
     catch (Exception $ex) {
    watchdog_exception('addEditDiscountCodeValidate', $ex);
    expertusErrorThrow($ex);
  }
}

function addNewDiscountDetails($form, &$form_state, $createUserId,$callFromAPI =0) {
  $txn = db_transaction();
  try {
    $insertStmt         = db_insert('uc_discounts');
    $fields       = discountFormatInputString($form, $form_state,$createUserId);
    $insertStmt->fields($fields);
    expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
    $discountId                         = $insertStmt->execute();
    updateGroupMappingDetails($discountId,'cme_dis',$createUserId,'emptyadd',0);
    //Insert discount code
    $newline = $callFromAPI ? '\n' : "\n";
   if(strpos($form_state['values']['dis_code'], $newline) !== FALSE) {
        $disc_codes =   explode( $newline, $form_state['values']['dis_code'] );
        foreach($disc_codes as $discCodes){
                $discCodes = trim($discCodes);
                if(!empty($discCodes))
                {
                        $codeArray = array('discount_id' => $discountId, 'code' => $discCodes);
                        $insStmt = db_insert('uc_discounts_codes');
                        $insStmt->fields($codeArray);
                        //$insStmt->condition('discount_id', $discountId);
                        expDebug::dPrintDBAPI('$$updStmt', $insStmt);
                        $insStmt->execute();
                }
        }
   }else{
        //Insert discount code
    $codeArray = array('discount_id' => $discountId, 'code' => trim($form_state['values']['dis_code']));
        $qryCode = db_insert('uc_discounts_codes')
        ->fields($codeArray);
        $qryCode->execute();
   }
    //Insert Discount Roles
    if($form_state['values']['dis_applicable_user']){
        $roleArray = array('discount_id' => $discountId, 'role_id' => 100,'role_code'=>'All');
        $qryRoles = db_insert('uc_discounts_roles')
                   ->fields($roleArray);
        $qryRoles->execute();
        //All organization
        $termArray = array('discount_id' => $discountId, 'term_id' => variable_get('organization_all_tid', 0));
            $qryTerms = db_insert('uc_discounts_terms')
                   ->fields($termArray);
        $qryTerms->execute();
    }

    //Insert Discount Terms
    if($form_state['values']['dis_applicable_training']){
        $termIds = array();
        $termIds[] = variable_get('delivery_type_all_tid', 0);
            $termIds[] = variable_get('tp_all_tid', 0);
            foreach ($termIds as $term_id){
                if($term_id){
                        $termArray = array('discount_id' => $discountId, 'term_id' => $term_id);
                $qryTerms = db_insert('uc_discounts_terms')
                   ->fields($termArray);
                  $qryTerms->execute();
                }
            }
    }
    $currencyType = (!empty($form_state['values']['dis_applicable_currency']) && $form_state['values']['dis_type'] == 3) ? $form_state['values']['dis_applicable_currency'] : null;
    $discTermArray = array('uc_discount_id' => $discountId,'discount_name'=>$form_state['values']['dis_name'],'currency_type'=>$currencyType,'created_by'=>$createUserId, 'created_on'=>now(), 'updated_by'=>$createUserId, 'updated_on'=>now(),);
    $slt_dic_query = db_insert('slt_discounts')
    ->fields($discTermArray);
    expDebug::dPrintDBAPI('Insert query for slt_discount', $slt_dic_query);
    $slt_dic_query->execute();

    expDebug::dPrint(' discountId = ' . $discountId, 4);

    if($form_state['values']['dis_type'] == 3){
            $fields['currency_type'] = $form_state['values']['dis_applicable_currency'];
            $modifiedCols = array('name', 'discount_type', 'discount_amount', 'max_uses','max_uses_per_user', 'max_uses_per_code', 'activates_on', 'is_active','currency_type');
    }else
        $modifiedCols = array('name', 'discount_type', 'discount_amount', 'max_uses','max_uses_per_user', 'max_uses_per_code', 'activates_on', 'is_active');

    make_audit_trail_entries($fields, $createUserId , NULL ,$discountId, 'Inserted','Commerce','exp_sp_administration_commerce_discounts','addNewDiscountDetails',
                                          $modifiedCols);
    $form_state['values']['id']     = $discountId;
    return array((object)array('id'=>$discountId));
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewDiscountDetails', $ex);
    unset($txn);
    if($callFromAPI){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Unable to  create a discount.";
        return $errobj;
//      return array((object)array('id' => "Failiure"));
    }
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

function updateDiscountDetails($form, &$form_state, $createUserId,$callFromAPI =0) {
  $txn = db_transaction();
  if($form_state['values']['create_user_id'])
      $loggedInUserId = $form_state['values']['create_user_id'];
  else
      $loggedInUserId  = getIdOfLoggedInUser();
  try {
        $discountId = $form_state['values']['mapped_discount_id'];
        if(!$discountId){
                return;
        }
        $defaultDetails   = getDiscountsDetails($discountId ,'fetchall');
    $updateStmt = db_update('uc_discounts');
    $fields     = discountFormatInputString($form, $form_state,$createUserId);
    $updateStmt->fields($fields);
    $updateStmt->condition('discount_id', $discountId);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $numUpdated = $updateStmt->execute();
	if(strpos($form_state['values']['dis_code'], "\n") !== FALSE || strpos($form_state['values']['dis_code'], ",") !== FALSE) { //Added "," for #0071490
        //Added callFromAPI for #0071490
        if($callFromAPI == 1){
            $disc_codes = explode( ",", $form_state['values']['dis_code'] );
        
        }else{
            $disc_codes = explode( "\n", $form_state['values']['dis_code'] );
        }
        $query_sel = db_select('uc_discounts_codes','d');
        $query_sel->addField('d','discount_code_id','discount_code_id');
        $query_sel->condition('discount_id',$discountId,'=');
        $query1 = $query_sel->execute()->fetchAll();
        expDebug::dPrint('discount array delete'.print_r($query1,true),4);
        
    	$query = db_delete('uc_discounts_codes')
    	->condition('discount_id',$discountId,'=');
    	$query->execute();
    	
    	
    	//Make delete log entries
    	foreach ($query1 as $val){
    	    $fields = array(
    	            'table_name' => 'uc_discounts_codes',
    	            'entity_id' => $val->discount_code_id,
    	            'entity_type' => 'discounts',
    	            'module_name' => 'exp_sp_administration_commerce_discounts',
    	            'functionality_name' => 'updateDiscountDetails',
    	            'api_name' => 'UpdateDiscountAPI',
    	            'deleted_on' => now(),
    	            'deleted_by' => $loggedInUserId,
    	            'parent1_entity_id' => $discountId,
    	            'parent1_entity_type' => 'discount_id',
    	
    	    );
    	    deleted_log_entry($fields);
    	}expDebug::dPrint('uc_discounts_codes table log');
    	
    	foreach($disc_codes as $discCodes){
    		$discCodes = trim($discCodes);
    		if(!empty($discCodes))
    		{
    		  
    			$codeArray = array('discount_id' => $discountId, 'code' => $discCodes);
	    		$insStmt = db_insert('uc_discounts_codes');
	    		$insStmt->fields($codeArray);
	    		expDebug::dPrintDBAPI('$insStmt', $insStmt);
	    		$insStmt->execute();
    		}
        }
    }    
    else{
	
    	//Added for #0071490 
        if(count($defaultDetails > 1)){
            expDebug::dPrint('$insStmt----->');
            if($form_state['values']['create_user_id'])
                $loggedInUserId = $form_state['values']['create_user_id'];
            else
                $loggedInUserId  = getIdOfLoggedInUser();
            
            $query_sel = db_select('uc_discounts_codes','d');
            $query_sel->addField('d','discount_code_id','discount_code_id');
            $query_sel->condition('discount_id',$discountId,'=');
            $query1 = $query_sel->execute()->fetchAll();
            expDebug::dPrint('discount array delete'.print_r($query1,true),4);
            
            $query = db_delete('uc_discounts_codes')
            ->condition('discount_id',$discountId,'=');
            $query->execute();
            
            //Make delete log entries
            foreach ($query1 as $val){
                $fields = array(
                        'table_name' => 'uc_discounts_codes',
                        'entity_id' => $val->discount_code_id,
                        'entity_type' => 'discounts',
                        'module_name' => 'exp_sp_administration_commerce_discounts',
                        'functionality_name' => 'updateDiscountDetails',
                        'api_name' => 'UpdateDiscountAPI',
                        'deleted_on' => now(),
                        'deleted_by' => $loggedInUserId,
                        'parent1_entity_id' => $discountId,
                        'parent1_entity_type' => 'discount_id',
                        
                );
                deleted_log_entry($fields);
            }expDebug::dPrint('uc_discounts_codes table log');
            
            $codeArray = array('discount_id' => $discountId, 'code' => $form_state['values']['dis_code']);
	   		$insStmt = db_insert('uc_discounts_codes');
	    	$insStmt->fields($codeArray);
	    	expDebug::dPrintDBAPI('$insStmt', $insStmt);
	    	$insStmt->execute();
    		}
        else{
            if($defaultDetails[0]->code != $form_state['values']['dis_code']){
                    $codeArray = array('code' => trim($form_state['values']['dis_code']));
                    $updStmt = db_update('uc_discounts_codes');
                    $updStmt->fields($codeArray);
                    $updStmt->condition('discount_id', $discountId);
                    expDebug::dPrintDBAPI('$$updStmt', $updStmt);
                    $updStmt->execute();
            }
        }
  }
    $currencyType = (!empty($form_state['values']['dis_applicable_currency']) && $form_state['values']['dis_type'] == 3) ? $form_state['values']['dis_applicable_currency'] : null;
    if($form_state['values']['dis_applicable_user'] == 0){
        $discTermArray = array('uc_discount_id' => $discountId,'discount_name'=>$form_state['values']['dis_name'],'currency_type'=>$currencyType,'updated_by'=>$createUserId, 'updated_on'=> now());
    }else{
        $discTermArray = array('uc_discount_id' => $discountId,'discount_name'=>$form_state['values']['dis_name'],'org_id'=>null,'job_role'=>null,'user_type'=>null,'currency_type'=>$currencyType,'updated_by'=>$createUserId, 'updated_on'=> now());
    }
    $slt_dic_query = db_update('slt_discounts')
    ->fields($discTermArray);
    $slt_dic_query->condition('uc_discount_id', $discountId);
    expDebug::dPrintDBAPI('update query for slt_discount', $slt_dic_query);
    $slt_dic_query->execute();

    //Insert Discount Terms

    if($form_state['values']['dis_applicable_training']){
        discountsTermsDelete($discountId,'orgterm');
        $termIds = array();
        $termIds[] = variable_get('delivery_type_all_tid', 0);
            $termIds[] = variable_get('tp_all_tid', 0);
            foreach ($termIds as $term_id){
                if($term_id){
                        $termArray = array('discount_id' => $discountId, 'term_id' => $term_id);
                $qryTerms = db_insert('uc_discounts_terms')
                   ->fields($termArray);
                  $qryTerms->execute();
                }
            }
    }
    else if(getDiscountApplicableAllTrainings($discountId)){
        discountsTermsDelete($discountId,'orgterm');
    }

    //Insert Discount Roles
    if($form_state['values']['dis_applicable_user']){
        discountsRolesDelete($discountId);
        /* Remove the Group Id */
        $updateStmt = db_update('slt_discounts');
        $fields1 = array('group_id'=>'');
        $updateStmt->fields($fields1);
        $updateStmt->condition('uc_discount_id', $discountId);
                expDebug::dPrintDBAPI('$updateStmt for slt_discount', $updateStmt);
        $updateStmt->execute();

        $roleArray = array('discount_id' => $discountId, 'role_id' => 100,'role_code'=>'All');
        $qryRoles = db_insert('uc_discounts_roles')
        ->fields($roleArray);
        $qryRoles->execute();
        //All organization
        discountsOrgTermsDelete($discountId);
        $termArray = array('discount_id' => $discountId, 'term_id' => variable_get('organization_all_tid', 0));
        $qryTerms = db_insert('uc_discounts_terms')
        ->fields($termArray);
        $qryTerms->execute();
    }
    else {
        if(getDiscountApplicableAllUsers($discountId,'roles')){
                discountsRolesDelete($discountId);
        }
        if(getDiscountApplicableAllUsers($discountId,'orgterm')){
                discountsOrgTermsDelete($discountId);
        }
    }

    expDebug::dPrint(' discountId = ' . $discountId, 4);
    if($form_state['values']['dis_type'] == 3){
        $fields['currency_type'] = $form_state['values']['dis_applicable_currency'];
        $modifiedCols = array('name', 'discount_type', 'discount_amount', 'can_be_combined_with_other_discounts' ,'max_uses','max_uses_per_user', 'max_uses_per_code', 'activates_on', 'is_active','currency_type');
    }else{
        $modifiedCols = array('name', 'discount_type', 'discount_amount', 'can_be_combined_with_other_discounts' ,'max_uses','max_uses_per_user', 'max_uses_per_code', 'activates_on', 'is_active');
    }

    make_audit_trail_entries($fields, $createUserId , $defaultDetails ,$discountId, 'Updated','Commerce','exp_sp_administration_commerce_discounts','updateDiscountDetails',$modifiedCols);
    $form_state['values']['id']     = $discountId;
    return array((object)array('id'=>$discountId));
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateDiscountDetails', $ex);
    if($callFromAPI){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Unable to update disocunt.";
        return $errobj;
    }
    unset($txn);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

/**
 * Deletes all discountsRolesDelete rows for a discount.
 */
function discountsRolesDelete($discount_id) {
        try{
  $query = db_delete('uc_discounts_roles')
                                 ->condition('discount_id',$discount_id,'=');
  $query->execute();
  }catch (Exception $ex) {
        watchdog_exception('discountsRolesDelete', $ex);
        expertusErrorThrow($ex);
  }
}

/**
 * Deletes all uc_discounts_terms rows for a discount.
 */
function discountsTermsDelete($discount_id, $exclude = '') {
        try{
        if(!$exclude){
        $query = db_delete('uc_discounts_terms')
        ->condition('discount_id',$discount_id,'=');
        $query->execute();
        }
        else if($exclude == 'orgterm'){
                $orgTids = getOrgDiscountsTerms($discount_id);
                $query = db_delete('uc_discounts_terms');
        $query->condition('discount_id',$discount_id);
        if(count($orgTids)){
                $query->condition('term_id', $orgTids ,'NOT IN');
        }
        expDebug::dPrintDBAPI("discountsTermsDelete Query ",$query);
        $query->execute();
        }
        }catch (Exception $ex) {
                watchdog_exception('discountsTermsDelete', $ex);
                expertusErrorThrow($ex);
        }
}


function discountFormatInputString($form, &$form_state,$createUserId) {
   try {

     $custom                = NULL;
     $trigger_element = $form_state['storage']['submit_triggering_element'];
     if($trigger_element == 'saveandpublish'){
       $is_active = 1;
     }
     else if($trigger_element == 'save'){
       $is_active = 0;
     }
     else if(isset($form_state['values']['is_active'])){
           $is_active =$form_state['values']['is_active'];
     }
     $discountAmt = ($form_state['values']['dis_type'] == 3) ?  $form_state['values']['dis_typeval'] : ($form_state['values']['dis_typeval'] / 100);
     $fields = array(
                  'name'               => $form_state['values']['dis_name'],
                  'description'        => $form_state['values']['dis_description']['value'],
                  'qualifying_type'    => 1,
                  'qualifying_amount'  => $form_state['values']['dis_min_price'],
                  'has_qualifying_amount_max'  => 0,
                  'qualifying_amount_max'      => 0,
                  'discount_type'      => $form_state['values']['dis_type'],
                  'discount_amount'    => $discountAmt,
                  'requires_code'      => 1,
                  'filter_type'        => FILTER_TYPE_TERMS,
                  'has_role_filter'    => 1,
                  'use_only_discounted_products_to_qualify' => 0,
                  'requires_single_product_to_qualify'      => 0,
                                'required_product'       => '',
                  'max_times_applied'    => 0,
                  'limit_max_times_applied'                                                             => 0,
                  'can_be_combined_with_other_discounts'          => $form_state['values']['dis_combine'],
                  'max_uses'           => $form_state['values']['dis_max_uses'],
                  'max_uses_per_user'  => $form_state['values']['dis_max_uses_per_user'],
                  'max_uses_per_code'  => $form_state['values']['dis_max_uses_per_code'],
                                                        'has_activation'     => 0,
                                                        'activates_on'       => time(),
                  'has_expiration'     => 0,
                  'expiration'         => 0,
                  'is_active'          => $is_active,
                                                        'weight'             => 0
                );
     if(!$form_state['values']['mapped_discount_id']){
         $fields['insert_timestamp'] = time();
     }
     if($trigger_element == 'unpublish'){
       $fields = array( 'is_active'  => 0 );
     }
     return $fields;
   } catch (Exception $ex) {
    watchdog_exception('discountFormatInputString', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To get organization list
 */
function getDiscountOrganizationList($org_name='', $selectedId=array(), $type, $iscount='', $page){
  try {
                $start = $page*15;
                $select = db_select('taxonomy_term_data', 'td');
                $select->join('slt_organization', 'org' ,'org.id = td.description');
                $select->join('taxonomy_vocabulary', 'voc' ,'voc.vid = td.vid');
                if($iscount=='Count'){
                        $select->addExpression('count(1)','count');
                }else{
              $select->addField('td', 'tid','org_id');
            $select->addField('org', 'name','org_name');
                }
      $select->condition('voc.machine_name', 'organization_nav_vocabulary');
      $select->condition('org.status', 'cre_org_sts_act');
      if(!empty($org_name)){
        $select->condition('org.name', '%' . db_like($org_name) . '%', 'LIKE');
      }

      if(sizeOf($selectedId)>0){
        if($type!='Include'){
                $select->condition('td.tid', $selectedId,'NOT IN');
                $select->range($start,15);
        }else{
                $select->condition('td.tid', $selectedId,'IN');
        }
      }else{
        $select->range($start,15);
      }

                        expDebug::dPrintDBAPI("getDiscountOrganizationList Query ",$select);
      if($iscount=='Count'){
        $organization = $select->execute()->fetchField();
      }else{
        $select->orderBy('org.name');
        $organization = $select->execute()->fetchAll();
      }
     return $organization;
   }catch (Exception $ex) {
    watchdog_exception('getDiscountOrganizationList', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To get getOrgDiscountsTerms
 */
function getOrgDiscountsTerms($discountId){
  try {
                $select = db_select('uc_discounts_terms', 'dt');
                $select->join('taxonomy_term_data', 'td' ,'td.tid = dt.term_id');
                $select->join('taxonomy_vocabulary', 'voc' ,'voc.vid = td.vid');
                $select->addField('td', 'tid','org_id');
      $select->condition('voc.machine_name', 'organization_nav_vocabulary');
      $select->condition('dt.discount_id', $discountId);
      expDebug::dPrintDBAPI("getOrgDiscountsTerms Query ",$select);
      $select->orderBy('td.name');
      $result = $select->execute();
      $organization = array();
      foreach ($result as $res) {
        $organization[] = $res->org_id;
      }
     return $organization;
   }catch (Exception $ex) {
    watchdog_exception('getOrgDiscountsTerms', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To get getOrgDiscountsTerms
 */
function getDiscountsRoles($discountId , $roleCode){
  try {
                $select = db_select('uc_discounts_roles', 'roles');
                $select->join('slt_profile_list_items', 'pli' ,'pli.id = roles.role_id');
                $select->addField('roles', 'role_id');
      $select->condition('pli.code', db_like($roleCode) . '%', 'LIKE');
      $select->condition('roles.discount_id', $discountId);
      expDebug::dPrintDBAPI("getDiscountsRoles Query ",$select);
      $select->orderBy('pli.name');
      $result = $select->execute();
      $roles = array();
      foreach ($result as $res) {
        $roles[] = $res->role_id;
      }
     return $roles;
   }catch (Exception $ex) {
    watchdog_exception('getDiscountsRoles', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getProfileItemNamesKeyedForDiscounts() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix.
 *                              The result is ordered by name. This function is used to build selection controls (e.g. dropdown and checkboxes)
 *                              for the new admin screen but can be used in other places as well.
 */
function getProfileItemNamesKeyedForDiscounts($codePrefix, $langCode = 'cre_sys_lng_eng', $name, $selectedId=array(), $type, $iscount='', $page) {
  try {
                $start = $page*15;
                $select = db_select('slt_profile_list_items','spli');
                  // Select the fields to fetch
                  if($iscount=='Count'){
                        $select->addExpression('count(1)','count');
                  }else{
                        $select->addField('spli','id', 'id');
                        $select->addField('spli','name', 'name');
                  }
      $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
                $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
      if(!empty($name)){
        $select->condition('spli.name', '%'. db_like($name) . '%', 'LIKE');
      }

      if(sizeOf($selectedId)>0){
        if($type!='Include'){
                $select->condition('spli.id', $selectedId,'NOT IN');
                $select->range($start,15);
        }else{
                $select->condition('spli.id', $selectedId,'IN');
        }
      }else{
        $select->range($start,15);
      }

                        expDebug::dPrintDBAPI("getProfileItemNamesKeyedForDiscounts Query ",$select);
      if($iscount=='Count'){
        $result = $select->execute()->fetchField();
      }else{
        $select->orderBy('spli.name');
        $result = $select->execute()->fetchAllKeyed();
      }
     return $result;
   }catch (Exception $ex) {
    watchdog_exception('getProfileItemNamesKeyedForDiscounts', $ex);
    expertusErrorThrow($ex);
  }

}

function updateDiscountRoles($form, &$form_state, $createUserId,$callFromAPI =0) {
  $txn = db_transaction();
  try {

         $discountId = $form_state['values']['mapped_discount_id'];
        if(!$discountId){
                return;
        }
        //$group_id = $form_state['values']['hidden_idlist'];
        $group_id = explode(',',$form_state['values']['hidden_idlist']);
        $i=1;
        foreach($group_id as $key => $values){
                $grps = explode('-',$values);
                $grpsid .= $grps[0];
                $grpsid .= ($i == count($group_id)) ? '':',';
                $i++;

        }
                //if(!empty($grpsid)){
        $updateStmt = db_update('slt_discounts');
    $fields = array('group_id'=>$grpsid,'updated_by'=>$createUserId, 'updated_on'=> now());
    $updateStmt->fields($fields);
    $updateStmt->condition('uc_discount_id', $discountId);
    expDebug::dPrintDBAPI('$updateStmt for slt_discount', $updateStmt);
    $updateStmt->execute();
                //}
        //Insert Discount Roles
   /* discountsRolesDelete($discountId);
    if(count($form_state['input']['jobroles_selected_id']) || count($form_state['input']['usertype_selected_id'])){
        $roleIds1['Job Role'] = explode(',', $form_state['input']['jobroles_selected_id']);
        $roleIds2['User Type'] = explode(',', $form_state['input']['usertype_selected_id']);
        $roleIds = array_merge($roleIds1, $roleIds2);
        expDebug::dPrint(' roleIds = ' . print_r($roleIds, true), 4);
        foreach ($roleIds as $key => $roleId){
                expDebug::dPrint(' roleIds = $role_id$role_id' . $key . 'keyyy'.print_r($role_id, true), 4);
                $role_code = $key;
                foreach($roleId as $role_id){
                        if($role_id){
                                $roleArray = array('discount_id' => $discountId, 'role_id' => $role_id,'role_code'=>$role_code);
                                $qryRoles = db_insert('uc_discounts_roles')
                                ->fields($roleArray);
                                $qryRoles->execute();
                        }
                }
        }
    }

    //Insert Discount Terms
    discountsOrgTermsDelete($discountId);
    if(count($form_state['input']['org_selected_id'])){
        $termIds = explode(',', $form_state['input']['org_selected_id']);
        $orgTid = variable_get('organization_all_tid', 0);
            foreach ($termIds as $term_id){
                if($term_id && $term_id != $orgTid){
                        $termArray = array('discount_id' => $discountId, 'term_id' => $term_id);
                $qryTerms = db_insert('uc_discounts_terms')
                   ->fields($termArray);
                  $qryTerms->execute();
                }
            }

    }

    $orgId = ($form_state['input']['org_selected_id']) ? '1' : '';
    $jobRole = ($form_state['input']['jobroles_selected_id']) ? '1' : '';
    $userType = ($form_state['input']['usertype_selected_id']) ? '1' : '';

    $updateStmt = db_update('slt_discounts');
    $fields = array('org_id'=>$orgId,'job_role'=>$jobRole,'user_type'=>$userType);
    $updateStmt->fields($fields);
    $updateStmt->condition('uc_discount_id', $discountId);
    expDebug::dPrintDBAPI('$updateStmt for slt_discount', $updateStmt);
    $numUpdated = $updateStmt->execute();
    $orgId = ($form_state['input']['org_selected_id']) ? '1' : '';
    $jobRole = ($form_state['input']['jobroles_selected_id']) ? '1' : '';
    $userType = ($form_state['input']['usertype_selected_id']) ? '1' : '';

    $updateStmt = db_update('slt_discounts');
    $fields = array('org_id'=>$orgId,'job_role'=>$jobRole,'user_type'=>$userType);
    $updateStmt->fields($fields);
    $updateStmt->condition('uc_discount_id', $discountId);
    expDebug::dPrintDBAPI('$updateStmt for slt_discount', $updateStmt);
    $numUpdated = $updateStmt->execute(); */

  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateDiscountRoles', $ex);
    unset($txn);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

/**
 * Deletes all discountsOrgTermsDelete rows for a discount.
 */
function discountsOrgTermsDelete($discountId) {
        try{
        $orgTerms = getOrgDiscountsTerms($discountId);
        foreach ($orgTerms as $term_id){
                if($term_id){
                $del = db_delete('uc_discounts_terms');
                $del->condition('discount_id',$discountId);
                $del->condition('term_id',$term_id);
                expDebug::dPrintDBAPI("discountsOrgTermsDelete",$del);
                $del->execute();
                }
        }
        }catch (Exception $ex) {
                watchdog_exception('discountsOrgTermsDelete', $ex);
                expertusErrorThrow($ex);
        }
}

function getDiscountOrgList($org_name,$selectedIds,$page){
        try{
        if($org_name == (t('LBL182'))){
                $org_name='';
        }
        $org_name=$org_name=='-'?'':$org_name;
        $selectedIds=$selectedIds=='-'?array():$selectedIds;
        $orgDetailsArray = explode(',',$selectedIds);
        $orgListArray = getDiscountOrganizationList($org_name,$orgDetailsArray,'Exclude','',$page);
        $orgList='';
  foreach($orgListArray as $key => $value){
        $orgId = $value->org_id;
    $orgName = $value->org_name;
    $orgNameRestricted = titleController('EXP-SP-ADMIN-COMMERCE-DISCOUNTS-ORGNAME', $orgName,30);
    $orgList .= '<li class="movable" id="'.$orgId.'" onkeydown="selectKeyDownAccess('.$orgId.', event, \'organization-available\')" ><span class="vtip" data="'.$orgName.'" title="'.sanitize_data($orgName).'">'.$orgNameRestricted.'</span></li>';
  }
  if(sizeOf($orgListArray)>=15)
        $orgList .= '<li id="org_more"><span onclick="getOrganizationList($(\'#org_list_sel\').val(),\'more\',\'discounts\')">'.t('LBL543').'</span></li>';
  print $orgList;
  }catch (Exception $ex) {
        watchdog_exception('getDiscountOrgList', $ex);
        expertusErrorThrow($ex);
  }

}

function getDiscountJobRoles($jobrole,$selectedIds,$page){
        try{
        if($jobrole == (t('LBL036').' '.t('LBL133'))){
                $jobrole='';
        }
        $jobrole=$jobrole=='-'?'':$jobrole;
        $selectedIds=$selectedIds=='-'?array():$selectedIds;
        $jobroleSelectArray1 = explode(',',$selectedIds);
        foreach($jobroleSelectArray1 as $key=>$value){
                $tmp = explode('-',$value);
                $jobroleSelectArray[] = $tmp[0];
        }
        $jobRolesArray = getProfileItemNamesKeyedForDiscounts("cre_usr_jrl_",'cre_sys_lng_eng',$jobrole,$jobroleSelectArray,'Exclude','',$page);
        $jobRoles='';
        foreach($jobRolesArray as $roleId => $roleName){
                $jobRoles .= '<li class="movable" id='.$roleId.'  tabindex="0" onkeydown="selectKeyDownAccess('.$roleId.', event, \'jobroles-available\')" ><span class="vtip" data="'.$roleName.'" title="'.sanitize_data($roleName).'">'.titleController('EXP-SP-ADMIN-COMMERCE-DISCOUNTS-ROLENAME', $roleName,30).'</span></li>';
        }
        if(sizeOf($jobRolesArray)>=15)
     $jobRoles .= '<li id="jobrole_more"><span onclick="getJobRoles($(\'#jobrole_list_sel\').val(),\'more\')">'.t('LBL543').'</span></li>';

  print $jobRoles;
  }catch (Exception $ex) {
        watchdog_exception('getDiscountJobRoles', $ex);
        expertusErrorThrow($ex);
  }
}

function getDiscountUserType($usertype,$selectedIds,$page){
        try{
        if($usertype == (t('LBL036').' '.t('LBL173'))){
                $usertype='';
        }
        $usertype=$usertype=='-'?'':$usertype;
        $selectedIds=$selectedIds=='-'?array():$selectedIds;
        $usertypeSelectArray = explode(',',$selectedIds);
        $userTypeArray = getProfileItemNamesKeyedForDiscounts("cre_usr_ptp_",'cre_sys_lng_eng',$usertype,$usertypeSelectArray,'Exclude','',$page);
        //$userTypeArray = getProfileItemNamesKeyed("cre_usr_ptp_",'cre_sys_lng_eng', false, '',$usertype, 25);
  $userType = '';
  foreach($userTypeArray as $userTypeKey => $userTypeName){
    $userTypeRestricted = titleController('EXP-SP-ADMIN-COMMERCE-DISCOUNTS-USERTYPENAME', $userTypeName,30);
          $userType .= '<li class="movable" id="'.$userTypeKey.'" tabindex="0" onkeydown="selectKeyDownAccess('.$userTypeKey.', event, \'usertype-available\')"><span class="vtip" data="'.$userTypeName.'" title="'.sanitize_data($userTypeName).'">'.$userTypeRestricted.'</span></li>';
  }
  if(sizeOf($userTypeArray)>=15)
        $userType .= '<li id="usertype_more"><span onclick="getUserTypes($(\'#user_type_sel\').val(),\'more\')">'.t('LBL543').'</span></li>';

  print $userType;
  }catch (Exception $ex) {
        watchdog_exception('getDiscountUserType', $ex);
        expertusErrorThrow($ex);
  }
}

function addtrainings($form, &$form_state) {
        try{
         $form_state['storage']['mode'] = 'edit';
   $form_state['rebuild'] = TRUE;
  }catch (Exception $ex) {
        watchdog_exception('addtrainings', $ex);
        expertusErrorThrow($ex);
  }

}

function trainingsGrid($form, $form_state) {
        try{
   $formName = $form_state['form_unique_name'];
   return $form[$formName];
   }catch (Exception $ex) {
        watchdog_exception('trainingsGrid', $ex);
        expertusErrorThrow($ex);
   }
}

function cancelAddTrainings($form, &$form_state){
        try{
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  }catch (Exception $ex) {
        watchdog_exception('cancelAddTrainings', $ex);
        expertusErrorThrow($ex);
  }
}

function addDiscountsForTrainings($form, &$form_state){
  $txn = db_transaction();
  try {
           $discountId = $form_state['triggering_element']['#ajax']['discount_id'];
           $uniqueId = "trainings-$discountId-Discounts";
           if($form_state['triggering_element']['#name'] == "commonsave"){
                   if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
                      $userId = getSltpersonUserId();
                      if (!empty($form_state['input']['hidden_idlist_'.$uniqueId])) {
                          $hiddenIds =  str_replace('-',',',$form_state['input']['hidden_idlist_'.$uniqueId]);
                      }
                      else {
                          $hiddenIds =  str_replace('-',',',$form_state['hidden_idlist_'.$uniqueId]);
                      }
                      $insertTermIds = explode(',', $hiddenIds);
                      foreach($insertTermIds as $insertTermId){
                                   $insertDiscountTerm          = db_insert('uc_discounts_terms');
                                   $fields = array(
                                            'discount_id'  => $discountId,
                                                        'term_id'          => $insertTermId,

                                          );
                                   $insertDiscountTerm->fields($fields);
                                   $DisTermId   = $insertDiscountTerm->execute();
                                   add_audit_trail_entry(getIdOfLoggedInUser(), $discountId, 'Discount' , 'exp_sp_administration_commerce_discounts', 'Discounts', 'Added training', null, null , $insertTermId);
                      }
                   }
           }else{
                        $saveType = $form_state['triggering_element']['#ajax']['training_type'];
                        $takeTermIdAll = db_select('taxonomy_term_data','term_data');
                        $takeTermIdAll->addField('term_data', 'tid');
                        $takeTermIdAll->condition('term_data.description', $saveType);
                        $taxTidAll = $takeTermIdAll->execute()->fetchAll();
                        if(count($taxTidAll) > 0) {             //for 42527: System throws error message when clicking select all learning plan and save in the Discount module
                                $checkifAlreadyAvail = db_select('uc_discounts_terms','dis');
                                $checkifAlreadyAvail->addField('dis', 'term_id');
                                $checkifAlreadyAvail->condition('dis.discount_id', $discountId);
                                $checkifAlreadyAvail->condition('dis.term_id', $taxTidAll[0]->tid);
                                $alreadyAll = $checkifAlreadyAvail->execute()->fetchAll();

                                if(count($alreadyAll) == 0){
                                        if(in_array($saveType,array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'))){
                                                $insertTerms = db_select('taxonomy_term_data','td');
                                                $insertTerms->join('taxonomy_index','tin','tin.tid = td.tid');
                                                $insertTerms->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type =\'cre_sys_obt_cls\'');
                                                $insertTerms->join('slt_course_class','cls','cls.id = nodesl.entity_id and cls.price > 0  and cls.id = td.description');
                                                $insertTerms->addExpression('GROUP_CONCAT(td.tid)','term_id_insert');
                                                $insertTerms->condition('cls.status', 'lrn_cls_sts_atv');
                                                $insertTerms->condition('td.vid', 3);
                                                $insertTerms->condition('cls.delivery_type', $saveType);
                                        }
                                        if(in_array($saveType,array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'))){
                                                $insertTerms = db_select('taxonomy_term_data','td');
                                                $insertTerms->join('taxonomy_index','tin','tin.tid = td.tid');
                                                $insertTerms->join('slt_node_learning_activity','nodesl','tin.nid = nodesl.node_id and nodesl.entity_type in (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')');
                                                $insertTerms->join('slt_program','prg','prg.id = nodesl.entity_id and prg.price > 0  and prg.id = td.description');
                                                $insertTerms->addExpression('GROUP_CONCAT(td.tid)','term_id_insert');
                                                $insertTerms->condition('prg.status', 'lrn_lpn_sts_atv');
                                                $insertTerms->condition('td.vid', 5);
                                                $insertTerms->condition('prg.object_type', $saveType);
                                        }
                                        $insIds = $insertTerms->execute()->fetchAll();
                                        $deleteDisTerm = db_delete('uc_discounts_terms');
                                        $deleteDisTerm->condition('discount_id', $discountId);
                                        $deleteDisTerm->condition('term_id', explode(',',$insIds[0]->term_id_insert),'IN');
                                        $deleteDisTerm->execute();

                                        $insertDiscountTerm     = db_insert('uc_discounts_terms');
                                        $fields = array(
                                                        'discount_id'  => $discountId,
                                                        'term_id'          => $taxTidAll[0]->tid,
                                        );
                                        $insertDiscountTerm->fields($fields);
                                        $DisTermId      = $insertDiscountTerm->execute();
                                }
                        }
           }
          $form_state['storage']['mode'] = 'view';
          $form_state['rebuild'] = TRUE;
        }catch (Exception $ex) {
            $txn->rollback();
            watchdog_exception('addDiscountsForTrainings', $ex);
            throw $ex;
        }

        unset($txn);

}

/*
 * getDiscountApplicablegroups($discountId) Fetcing is applicable for all groups or not. Return 0 or 1
*/
function getDiscountApplicablegroups($discountId){
        try{
                $select = db_select('slt_discounts','disc');
                $select->addField('disc','group_id');
                $select->condition('disc.uc_discount_id', $discountId);
                expDebug::dPrintDBAPI(' $select getDiscountApplicablegroups OBJ = ' ,$select);
                $isAllGroups = $select->execute()->fetchField();
                $isApplicable = ($isAllGroups != null) ? 1 : 0;
                return $isApplicable;
        }catch (Exception $ex) {
                watchdog_exception('getDiscountApplicablegroups', $ex);
                expertusErrorThrow($ex);
        }
}
/*
 * getDiscountApplicabletotrainings($discountId) Fetcing is applicable for all groups or not. Return 0 or 1
*/
function getDiscountApplicabletotrainings($discountId){
        try{
                $classQuery = db_select('uc_discounts_terms','dis_terms');
                $classQuery->join('taxonomy_index','tax_index','tax_index.tid = dis_terms.term_id');
                $classQuery->join('taxonomy_term_data','term_data','term_data.name IN(\'--All Delivery types--\', \'--All Tp--\')');
                $classQuery->addExpression('group_concat(distinct dis_terms.term_id)','trainingsadded');
                $classQuery->condition('dis_terms.discount_id',$discountId,'=');
                $classQuery->range(0,1);
                expDebug::dPrintDBAPI(' $select getDiscountApplicabletotrainings OBJ = ' ,$classQuery);
                $isAllTrainings = $classQuery->execute()->fetchField();
                $isApplicable = ($isAllTrainings != null) ? 1 : 0;
                return $isApplicable;
        }catch (Exception $ex) {
                watchdog_exception('getDiscountApplicabletotrainings', $ex);
                expertusErrorThrow($ex);
        }
}

/**
 * @author ayyappans
 * @method to create discount from api simulator
 * @param array $params
 * @return object:response
 */
function createDiscountByRestAPI($params) {
        try {
                return addOrUpdateDiscountForRestAPI($params);
        } catch (Exception $e) {
                watchdog_exception('addOrUpdateDiscountForRestAPI', $e);
                expertusErrorThrow($e);
        }
}

/**
 * @author ayyappans
 * @method to update discount by id from api simulator
 * @param array $params
 * @return object:response
 */
function updateDiscountByRestAPI($params) {

        try {
                return addOrUpdateDiscountForRestAPI($params, 1);
        } catch (Exception $e) {
                watchdog_exception('addOrUpdateDiscountForRestAPI', $e);
                expertusErrorThrow($e);
        }
}

/**
 * @author ayyappans
 * @method to create/update discount by id based on $updateApi parameter
 * @param array $params
 * @param bool $updateApi
 * @return object:response
 */
function addOrUpdateDiscountForRestAPI($params, $updateApi = 0) {
	try {
		$type = array('percentage_off' => 2, 'fixed_amount_off' => 3);
		$params['values']['type'] = $type[$params['values']['type']];
		if(empty($params['values']['type'])) {
			if($updateApi) {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Type can be either percentage_off or fixed_amount_off.";
				return $errobj;
			}
			else {
			return array((object)array('id' => "type can be either percentage_off or fixed_amount_off."));
			}
		}
		if(($params['values']['type'] == 3) && empty($params['values']['currency_type'])){
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Currency Type should not be null for fixed_amount_off";
			return $errobj;
		}else if($params['values']['type'] == 3){
			$curr_valid = currency_isValidOrNot($params['values']['currency_type']);
 			if(empty($curr_valid)){
 				$errobj=new stdClass();
 				$errobj->isValidateError = 1;
 				$errobj->errcode = 'L_012';
 				$errobj->errormsg = "Currency Type is not Valid";
 				return $errobj;
 			}
 		}
		if($updateApi) {
			//check if discount id is valid
			$count = getDiscountsDetails($params['values']['id'], 'count');
			if($count <= 0) {
				if($updateApi) {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "No discount found for given id.";
					return $errobj;
				}
				else {
					return array((object)array('id' => "No discount found for given id."));
				}

			}
		}
			  $dis_id = $updateApi ? $params['values']['id'] : '';
                
                $discount_codes = explode(',',trim($params['values']['code']));
                
                 
                $discont_codes = array_filter($discont_codes);
                
                 
					
				 $dis_cnt=addEditDiscountCodeValidate($params,$dis_id,true);
                  if(!$dis_cnt)
                        {
                   $errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Code already exist";
					return $errobj;
						}
                

                if(($params['values']['type'] == 3) && empty($params['values']['currency_type'])){
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Currency Type should not be null for fixed_amount_off";
                        return $errobj;
                }else if($params['values']['type'] == 3){
                        $curr_valid = currency_isValidOrNot($params['values']['currency_type']);
                        if(empty($curr_valid)){
                                $errobj=new stdClass();
                                $errobj->isValidateError = 1;
                                $errobj->errcode = 'L_012';
                                $errobj->errormsg = "Currency Type is not Valid";
                                return $errobj;
                        }
                }
				    if($params['values']['is_active']==1)
                     {
                    if($params['values']['for_all_trainings']!=1)
                    {
                       $errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "select training details ";
					return $errobj;
                    }
                    if($params['values']['for_all_users']!=1)
                    {
                       $errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "select users details. ";
					return $errobj;
                    }
                }
                $form_fields = array("id" => "mapped_discount_id", "name" => "dis_name", "description" => "dis_description", "type" => "dis_type", "value" => "dis_typeval","currency_type"=>"dis_applicable_currency", "max_uses" => "dis_max_uses", "min_price" => "dis_min_price", "max_uses_per_user" => "dis_max_uses_per_user", "max_uses_per_code" => "dis_max_uses_per_code", "code" => "dis_code", "for_all_trainings" => "dis_applicable_training", "for_all_users" => "dis_applicable_user", "combine_with_others" => "dis_combine", "userid" => "userid", "is_active" => "is_active");

		$form=array();
		$form["form_id"]="";
		$form_state = array();
		foreach ($params['values'] as $key => $value) {
			$form_element = $form_fields[$key];
			if($key == 'description') {
				$form_state['values'][$form_element]['value'] = $value;
			}
			else {
				$form_state['values'][$form_element] = $value;
			}
		}
		checkDiscountYesNoOptions($form_state);
		exp_sp_administration_discounts_addedit_form_validate($form, $form_state, 1);
		$form_validation = form_get_errors();
		if (!empty($form_validation)){
			expDebug::dprint('yuvan check00' .print_r($form_validation,true),5);
		
		foreach ($form_validation as $key=>$value) {
			$form_element = array_search($key, $form_fields);
			$validate[] = str_replace($key, $form_element, $value);
		}
		}
		if(!empty($validate)){
			return array((object)array('id' => implode(', ', $validate)));
		}
		$loggedInUserId = $form_state['values']['userid'];
		array_walk_recursive($form_state['values'], 'exp_sp_stripslashes');	//to strip slashes on spl characters
		if(!$updateApi) {
			$returnArr = addNewDiscountDetails($form, $form_state, $loggedInUserId, 1);
		}
		else {
			/*	update discount by id*/
			$returnArr = updateDiscountDetails($form, $form_state, $loggedInUserId, 1);
		}
		return $returnArr;
	} 
	catch (Exception $e) {
		watchdog_exception('addOrUpdateDiscountForRestAPI', $e);
		expertusErrorThrow($e);
	}
}

function checkDiscountYesNoOptions($form_state) {
        $fields = array("is_active", "dis_applicable_training", "dis_applicable_user", "dis_combine");
        foreach ($fields as $field) {
                if(!in_array($form_state['values'][$field], array('0', '1'))) {
                        form_set_error($field, $field.' can be either 0 or 1');
                }
        }
}

function exp_sp_stripslashes(&$item, $key) {
        $item = stripcslashes($item);
}
/**
 * @author ayyappans
 * @method to list discounts
 * @param array $arguments
 * @return object:response
 */
function listDiscountByRestAPI($arguments) {
//      $start, $limit, $sortby, $textfilter, $discount_status, $discount_type
        try {
                $validate =  validate_list_discount_api($arguments);
                if(!empty($validate)) {
                        $error->isValidateError = true;
                        for ($index = 0; $index < count($validate); $index++) {
                                if(!isset($error->errormsg)) {
                                        $error->errormsg = ($index+1).'. '.$validate[$index];
                                }
                                else {
                                        $error->errormsg = $error->errormsg."\n".($index+1).'. '.$validate[$index];
                                }
                        }
                        return $error;
                }
                $fields = array(
                                'dis' => array(
                                                'discount_id' => 'id',
                                                'name' => 'name',
                                                'description' => 'description',
                                                'is_active' => 'is_active',
                                                'discount_type' => 'type',
                                                'discount_amount' => 'value',
                                                'max_uses' => 'max_uses',
                                                'max_uses_per_user' => 'max_uses_per_user',
                                                'max_uses_per_code' => 'max_uses_per_code',
                                                'qualifying_amount' => 'min_price',
                                                'can_be_combined_with_other_discounts' => 'combine_with_others'
                                ),
                                /* 'cde' => array(
                                                'code' => 'code'
                                ) */
                );
                $type = array('percentage_off' => 2, 'fixed_amount_off' => 3);
                if(in_array(($arguments['discount_status']), array('0', '1'))) {
                        $_GET['discountStatus'] = $arguments['discount_status'] == '0' ? 99 : $arguments['discount_status'];
                }
                if(!empty($arguments['discount_type'])) {
                        $_GET['discountType'] = $type[$arguments['discount_type']];
                }
                if(!empty($arguments['textfilter'])) {
                        $_GET['textfilter'] = $arguments['textfilter'];
                }
                if(!empty($arguments['sortby'])) {
                        $_GET['sortby'] = $arguments['sortby'];
                }
                $discounts = fetchDiscountsAll('', $arguments['start'], $arguments['limit'], 1, $fields);
                $count = fetchDiscountsAll('COUNT', $arguments['start'], $arguments['limit'], 1, $fields);
                include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_commerce/exp_sp_administration_commerce_discounts/exp_sp_administration_commerce_discounts_trainings.inc";
                foreach ($discounts as $discount) {
                        $discount->for_all_trainings    = getDiscountApplicableAllTrainings($discount->id) == 1 ? 'yes' : 'No';
                        $discount->for_all_users        = getDiscountApplicableAllUsers($discount->id) == 1 ? 'yes' : 'No';
                        $discount->combine_with_others  = $discount->combine_with_others == 1 ? 'Yes' : 'No';
                        $discount->is_active                    = $discount->is_active == 1 ? 'Yes' : 'No';
                        $defaultAmount                                  = ($discount->value) ? $discount->value : '';
                        $discount->value                                = ($discount->type == 2 && $defaultAmount) ? ($defaultAmount * 100) : $defaultAmount;
                        $discount->type                                 = array_search($discount->type, $type);
                        $discount->groups                               = implode(', ', getDiscountGroupAccessDetailsForApi($discount->id));
                        $discount->code                                 = implode(",", getDiscountCodes($discount->id));
                        $trainings                                              = fetchAlreadyAddedTrainings($discount->id);
                        $training_array = array();
                        foreach ($trainings as $training) {
                                $training_array[] = $training->entity_id;
                        }
                        $discount->trainings = implode(', ', $training_array);
                }
                $discounts['totalrow'] = $count;
                return $discounts;
        } catch (Exception $ex) {
                watchdog_exception('listOrganizationByRestAPI', $ex);
                expertusErrorThrow($ex);
        }
}

function validate_list_discount_api(&$arguments) {
        $validate = array();
//      $arguments = array_map('strtolower', $arguments);
        if(!empty($arguments['sortby']) && !in_array($arguments['sortby'], array('AZ', 'ZA', 'NewlyListed'))) {
                $validate[] = 'possible values for sortby are AZ, ZA and NewlyListed';
        }
        if(!empty($arguments['start']) && (!is_numeric($arguments['start']) || $arguments['start'] < 0)) {
                $validate[] = 'start can be greater than eqaul to zero';
        }
        if(!empty($arguments['limit']) && (!is_numeric($arguments['limit']) || $arguments['limit'] < 0)) {
                $validate[] = 'limit can be greater than eqaul to zero';
        }
        if(!empty($arguments['discount_status']) && !in_array($arguments['discount_status'], array('0', '1'))) {
                $validate[] = 'discount_status can be either 0 or 1';
        }
        if(!empty($arguments['discount_type']) && !in_array($arguments['discount_type'], array('percentage_off', 'fixed_amount_off'))) {
                $validate[] = 'discount_type can be either percentage_off or fixed_amount_off';
        }
        return $validate;
}

function getDiscountGroupAccessDetailsForApi($entityId) {
        try {
                $select = db_select('slt_groups', 'grps');
                $select->leftjoin('slt_discounts', 'disc', 'FIND_IN_SET(grps.id, disc.group_id) > 0 and disc.uc_discount_id = '.$entityId.'');
                $select->addField('grps', 'name', 'grp_name');
                $select->condition('grps.status', 'cre_sec_sts_atv');
                $select->condition('grps.is_admin', 1, '!=');
                $select->condition('disc.group_id', null, 'is not null');
                $select->orderBy('grps.name');
                $results = $select->execute()->fetchCol(0);
                return $results;
        } catch (Exception $ex) {
                watchdog_exception('getDiscountGroupAccessDetailsForApi', $ex);
                expertusErrorThrow($ex);
        }
}

function getDiscountCodes($discountId) {
        try{
                $select = db_select("uc_discounts","dis");
                $select->leftJoin('uc_discounts_codes', 'cde', 'dis.discount_id= cde.discount_id');
                $select->addField('cde','code');
                $select->condition('dis.discount_id',$discountId,'=');
                $discountList = $select->execute()->fetchCol(0);
                return $discountList;
        } catch (Exception $ex) {
                watchdog_exception('getDiscountCodes', $ex);
                expertusErrorThrow($ex);
        }
}

function getSelectedCurrency($discountId){
try{
                $select = db_select("slt_discounts","dis");
                $select->addField('dis','currency_type','code');
                $select->condition('dis.id',$discountId,'=');
                $discountCur = $select->execute()->fetchField();
                $currenyCode = (empty($discountCur)) ? '' : $discountCur;
                return $currenyCode;
        } catch (Exception $ex) {
                watchdog_exception('getDiscountCodes', $ex);
                expertusErrorThrow($ex);
        }
}
?>