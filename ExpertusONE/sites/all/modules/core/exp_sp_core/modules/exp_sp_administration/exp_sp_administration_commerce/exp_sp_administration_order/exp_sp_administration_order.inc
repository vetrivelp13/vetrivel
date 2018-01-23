<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getManageOrderTabContentType() - For order list admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getManageOrderTabContentType(){
	try {
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getManageOrderTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * manageOrderNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function manageOrderNarrowSearchFiltersHTML() {
  try {
      $checkedStatus = array();
      $allStatus = uc_order_status_options_list();
      unset($allStatus['processing']);
      unset($allStatus['in_checkout']);
      unset($allStatus['reserved_cancelled']);
      expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

      $renderfiltersets=array();

      $currencyOptions = getCurrencyFilters("Order");

      if(count($allStatus) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'orderstatus',
                                                                     'title' => t('LBL102'), //Status
                                                               		 'divid' => 'status',
                                                                     'checkboxes_list' => $allStatus,
                                                                     'checked_list' => $checkedStatus)
                                                              );
      }

      if(module_exists('exp_sp_commerce') && count($currencyOptions) > 1)
      {
	     $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
      		array('code' => 'ordercurrency',
      				'html_id' => 'catalogordercurrency_container',
      				'title' => t('LBL101'),
      				'divid' => 'currency',
      				'checkboxes_list' => $currencyOptions)
     	 );
      }

      $ordermax = getConfigValue('maxprice');
      $renderfiltersets[]=theme('narrow_search_filterset_slider',
      		array('code' => 'orderPrice',
      				'title' => t('LBL040'),
      				'startval' =>'0' ,
      				'endval' => $ordermax,
      				'prefix' => ''
      		)
      );


      $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'order_id',
                                                                     'title' => t('LBL884'),//Order No
                                                                     'default_text'=> t('LBL970') . ' ' . t('LBL884')//Type a Order Id
                                                                     )
                                                              ) ;
      /*$renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'order_username',
                                                                     'title' => t('LBL054'), //Username
                                                                     'default_text'=> t('LBL970') . ' '. t('LBL054') //Type a Username
                                                                     )
                                                              ) ;*/

      $renderfiltersets[]=theme('narrow_search_filterset_daterange',
                                                               array('code' => 'daterange',
                                                                     'title' => t('LBL042'),
                                                                     'dateRangeShowOption' => 'all') /*dateRangeShowOption Option is all or past or future*/
                                                              );

      $narrowfilters =  array();
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"orderstatus");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"ordercurrency");
      $narrowfilters[] = array("type"=> "addltext", "code"=>"order_id", "acpath" =>"administration/commerce/order/orderid-autocomplete","defaultText"=>t('LBL970'). ' ' .t('LBL884'));
      //$narrowfilters[] = array("type"=> "addltext", "code"=>"order_username", "acpath" =>"administration/commerce/order/username-autocomplete","defaultText"=>t('LBL970') . ' '. t('LBL054'));
      $narrowfilters[] = array("type"=>"slider",  "code"=>"orderPrice");
      $narrowfilters[] = array("type"=> "daterange","code"=> "daterange","from_tooltip"=> "Select From Date", "from_default_text" => t("LBL220"), "to_tooltip" => "Select To Date", "to_default_text" => t("LBL221"));
      $filterSetforJS=$narrowfilters;

     return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
  } catch (Exception $ex) {
    watchdog_exception('manageOrderNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * manageOrderSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function manageOrderSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'order-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'order-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'order-sort-new');

  $defaultSortOrder = array('last_sort_type'=>'NewlyListed',
  					                   'last_sort_type_html_id'=>'order-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($defaultSortOrder), // Currently being ignored
                                              'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('manageOrderSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * manageOrderSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function manageOrderSearchResultsActionbarHTML() {
   try {
  $actionbarLinksListforJS = array('export_path' => 'administration/commerce/order/export/',
  					                       'print_path'  => 'administration/commerce/order/print/'
                                  );
  $createPerm = (user_access('create orders')) ? true : false;
  return theme('narrow_search_actionbar', array( 'add_button_title'   => t('ACLBL0004'), // create order
												                         'add_page_path'      => 'administration/order/create',
												                         'show_print_button'  => true,
												                         'show_export_button' => true,
												                         'show_add_button'    => $createPerm,
                                        		     'ctools_style'       => 'ctools-modal-ctools-admin-order_edit-style',
                                                 'actionbar_list'     => drupal_json_encode($actionbarLinksListforJS),
  		                                           'page_code' => 'cme_pmt',
                                           )
               );
  } catch (Exception $ex) {
  	watchdog_exception('manageOrderSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getManageOrderAdminTabContentMainHTML - Returns the html for tab order list
 */
function getManageOrderAdminTabContentMainHTML() {
	try {
   $narrowsearchTabOrderListforJS = array(
                                            'search_base_path' =>'administration/commerce/order/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/commerce/order/textfilter-autocomplete'
                                          );

   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabOrderListforJS))
                              );
   } catch (Exception $ex) {
   	watchdog_exception('getManageOrderAdminTabContentMainHTML', $ex);
   	expertusErrorThrow($ex);
   }
}

/*
 * getManageOrderTabContent		  - Returns the html for narrow search block on order list search page
 */
function getManageOrderTabContent() {
	try {
  $tabContentTypeArr = getManageOrderTabContentType();

  $manageOrderResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => manageOrderNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => manageOrderSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => manageOrderSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getManageOrderAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','commerce'),
  );
  echo drupal_json_encode($manageOrderResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getManageOrderTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getManageOrderDetailsHTML() - Renders a survey detail as HTML to be displayed
 * in jqGrid.
 */
function getManageOrderDetailsHTML ($orders)
{
    try {
        expDebug::dPrint('ORDERERERE___>' . print_r($orders, 1), 4);
        $detailsList = array();
        if (! empty($orders->uc_order_id)) {
            $detailsList[] = array(
                    'detail' => t('LBL884') . ': ' . $orders->uc_order_id,
                    'tooltip' => t('LBL884') . ': ' . $orders->uc_order_id
            );
        }
        if (! empty($orders->purchase_date)) {
            $detailsList[] = array(
                    'detail' => t('LBL042') . ': ' .
                             dateTimeStrToUnivShort($orders->purchase_date, 'Y', 
                                    false, false, false),
                            'tooltip' => t('LBL042') . ': ' .
                             dateTimeStrToUnivShort($orders->purchase_date, 'Y', 
                                    false, false, false)
            );
        }
        if (! empty($orders->order_status)) {
            $orderstatus = uc_order_status_data($orders->order_status, 'title');
            expDebug::dPrint('$statuses : ' . print_r($orderstatus, true), 4);
            if ($orderstatus == 'Completed')
                $orderstatus = 'Payment received';
            $detailsList[] = array(
                    'detail' => '<span>' . t('LBL102') . ':</span> ' .
                             titleController('ORDER-PAYMENT-STATUS', 
                                    t($orderstatus), 10),
                            'tooltip' => t('LBL102') . ': ' . t($orderstatus)
            );
        }
        if (! empty($orders->order_total)) {
            // $detailsList[] = array('detail' => t('LBL040'). ':
            // '.$orders->currency_type. ' '. $orders->currency_symbol
            // .number_format($orders->order_total, 2, '.', ''), 'tooltip' =>
            // t('LBL040').': '.$orders->currency_symbol
            // .number_format($orders->order_total, 2, '.', ''));
            $detailsList[] = array(
                    'detail' => t('LBL040') . ': ' . $orders->currency_type . ' ' .
                             number_format($orders->order_total, 2, '.', ''),
                            'tooltip' => t('LBL040') . ': ' .
                             $orders->currency_type . ' ' .
                             number_format($orders->order_total, 2, '.', '')
            );
        }
        
        $actionList[] = array(
                'action_link_text' => t('LBL063'),
                'action_page_path' => 'administration/commerce/order/nojs/addedit',
                'action_button_params' => $orders->uc_order_id,
                'ctools_style' => 'ctools-modal-ctools-admin-order_edit-style',
                'js_object_info' => array(
                        'name' => 'narrowsearch', // provide the java script
                                                  // object name
                        'init_id' => 'root-admin'
                ) // provide the id on which you have
                                           // initialized/will be initializing
                                           // the javascript object
,
                'tooltip' => t('LBL063')
        );
        $viewOrderlist[] = array(
                'action_link_text' => 'Order'
        );
        
        /*
         * $newActionList[] = array('action_link_text' => t('LBL642'),
         * 'action_page_path' => '',
         * 'action_button_params' => $orders->uc_order_id,
         * 'action_button_params_text' => 'access',
         * 'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
         * 'js_object_info' => array (
         * 'name' => 'narrowsearch', // provide the java script object name
         * 'init_id' => 'root-admin', // provide the id on which you have
         * initialized/will be initializing the javascript object
         * ),
         * 'tooltip' => t('LBL642'),
         * 'entityId' => $orders->uc_order_id,
         * 'entityType' => 'cme_pmt',
         * 'icon_style_class'=>'access-tab-icon',
         * 'postype' => 'topleft',
         * 'poslwid' => 70,
         * );
         */
        $orders->entityType = 'cme_pmt';
        expDebug::dPrint('$orders ayyappans '.print_r($orders, 1));
        return theme('narrow-search-results-item-details', 
                array(
                        'title' => trim($orders->lastname) . ', ' . trim($orders->firstname),
                        'additional_title_info' => trim($orders->username),
                        'details_list' => $detailsList,
                        'description' => '',
                        'action_list' => $actionList,
                        'view_list' => $viewOrderlist,
                        'new_action_list' => $newActionList,
                        'entity_multi_action' => $orders, // Code moved
                                                          // into Action
                                                          // list
                        'sumedit' => $orders->sumedit,
                        'sumdelete' => $orders->sumdelete
                ));
    } catch (Exception $ex) {
        watchdog_exception('getManageOrderDetailsHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getManageOrderActionsHTML() - Renders order list actions as HTML to be displayed in jqGrid.
 */
function getManageOrderActionsHTML($orders) {
	try {
  return theme('narrow-search-results-item-actions',
                     array('catalog_details' => $orders)
               );
  } catch (Exception $ex) {
  	watchdog_exception('getManageOrderActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * manageOrderSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function manageOrderSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = "per" . ".last_name";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = "per" . ".last_name";
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
  	watchdog_exception('manageOrderSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendManageOrderListToJqGrid() - Prepares JSON response to JqGrid's request for order list Admin
 */
function sendManageOrderListToJqGrid ()
{
    try {
        // //clear the admin order related session and cookie
        unset($_SESSION['cartform']);
        unset($_SESSION['admin_shop_cart_currency_code']);
        unset($_SESSION['admin_shop_cart_currency_sym']);
        unset($_COOKIE['admin_shop_cart_currency_code']);
        unset($_COOKIE['admin_shop_cart_currency_sym']);
        setcookie('admin_shop_cart_currency_code', null, - 1, '/');
        setcookie('admin_shop_cart_currency_sym', null, - 1, '/');
        expDebug::dPrint(
                'page = "' . $_GET['page'] . '" ,' . 'rows = "' . $_GET['rows'] .
                         '" ,' . 'sort(sidx) = "' . $_GET['sidx'] . '" ,' .
                         'order(sord) = "' . $_GET['sord'] . '" ,' . 'search = "' .
                         $_GET['search'] . '" ,' . 'nd = "' . $_GET['nd'] . '" ,' .
                         'id = "' . $_GET['id'] . '" ,' . 'oper = "' .
                         $_GET['oper'] . '" ,' . 'editoper = "' .
                         $_GET['editoper'] . '" ,' . 'addoper = "' .
                         $_GET['addoper'] . '" ,' . 'deloper = "' .
                         $_GET['deloper'] . '" ,' . 'totalrows = "' .
                         $_GET['totalrows'] . '" ,' . 'argumentType = "' .
                         $argumentType . '" ,' . 'npage = "' . $_GET['npage'], 4);
        
        $page = $_GET['page']; // get the requested page
        $limit = $_GET['rows'];
        
/*         $numOrder = fetchManageOrderAll('COUNT');
        
        expDebug::dPrint('$numOrder = ' . print_r($numOrder, true), 4);
        
        $total_pages = 0;
        $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, 
                $numOrder); */
        $start = calculate_offset($page, $limit);
        expDebug::dPrint('$start = ' . print_r($start, true), 4);
        
        $orderList = fetchManageOrderAll('LISTS', $start, $limit);
        expDebug::dPrint('orderList = ' . print_r($orderList, true), 4);
        
        // Prepare and send the response
        $response = new stdClass();
        $response->page = $page;
        $response->total = $total_pages;
        $response->records = $numOrder;
        
        $response->initial_sort_type_html_id = 'order-sort-new';
        
        $i = 0;
        foreach ($orderList as $orders) {
            expDebug::dPrint('$orders = ' . print_r($orders, true), 4);
            
            $orders->row = $i;
            $orders->object_type = 'Order';
            $response->rows[$i]['id'] = $orders->slt_order_id;
            
            $response->rows[$i]['cell'] = array(
                    'image' => theme('narrow-search-results-item-image', 
                            array(
                                    'results' => $orders
                            )),
                    'details' => getManageOrderDetailsHTML($orders),
                    'action' => getManageOrderActionsHTML($orders)
            );
            
            $i ++;
        }
        
        expDebug::dPrint('$response = ' . print_r($response, true), 4);
        
        echo drupal_json_encode($response);
    } catch (Exception $ex) {
        watchdog_exception('sendManageOrderListToJqGrid', $ex);
        expertusErrorThrow($ex);
    }
}

function orderSearchTables(){
	// Select table
	try {
  $userId = empty($userId)? getIdOfLoggedInUser() : $userId;
  $select = db_select('uc_orders', 'ucorder');
  $select->leftJoin('uc_order_line_items', 'ordersitem', 'ordersitem.order_id =ucorder.order_id ');
  $select->LeftJoin('slt_order', 'orders', 'ucorder.order_id =orders.uc_order_id');
 // $select->LeftJoin('slt_order_items', 'orditem', 'orditem.order_id=orders.id');
  $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=orders.id AND group_map.entity_type = \'cme_pmt\' and group_map.group_type = 1 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  // $select->leftJoin('slt_owner_details','detail','detail.group_id = catacs.id and detail.owner_id = '.$userId.'');
  $select->LeftJoin('slt_person', 'per', 'per.id = orders.user_id');
  $select->LeftJoin('slt_profile_list_items', 'spli', 'spli.attr1 = ucorder.currency_type');

  // Select fields/expression values to be fetched from the database.
  $select->addField('orders','id','slt_order_id');
  $select->addField('ucorder','order_id','uc_order_id');
  $select->addField('ucorder','order_status','order_status');
  $select->addField('ucorder','order_total','order_total');
  $select->addField('ucorder','currency_type','currency_type');
  $select->addField('spli','attr2','currency_symbol');
  $select->addField('ucorder','product_count','product_count');
  $select->addField('orders','created_on','purchase_date');
  $select->addField('ordersitem','amount','discount_amount');
  $select->addField('orders','order_total_amt','price');
  $select->addField('per','last_name','lastname');
  $select->addField('per','first_name','firstname');
  $select->addField('per','user_name','username');
  $select->condition('orders.uc_order_id', '', 'IS NOT NULL');
  $select->condition('ucorder.order_status', 'reserved_cancelled', '!=');
  $select->condition('ucorder.order_status', 'in_checkout', '!=');
  $select->condition('per.user_name', array('admin','guest'),'NOT IN');
  $select->groupBy('ucorder.order_id');
  //filterByUserAccessPrivileges($select, 'catacs','','group_map',1);
  return $select;
  } catch (Exception $ex) {
  	watchdog_exception('orderSearchTables', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchManageOrderAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchManageOrderAll($op, $start='', $limit='',$callfrom='') {
  try {
     expDebug::dPrint('callfrom = ' .$callfrom,4);
  $select = orderSearchTables();
  if(isset($_GET['orderstatus']) && !empty($_GET['orderstatus'])){
    if (strpos($_GET['orderstatus'],'payment_received') !== false) 
	  $_GET['orderstatus'] .= '|completed';
    selectAddNarrowSearchINCondition($select, 'ucorder', 'order_status', 'orderstatus');
  }
  if(isset($_GET['ordercurrency']) && !empty($_GET['ordercurrency'])){
    selectAddNarrowSearchINCondition($select, 'ucorder', 'currency_type', 'ordercurrency');
  }
  if(isset($_GET['order_id']) && !empty($_GET['order_id'])){
    selectAddNarrowSearchByTextCondition($select, 'ucorder', 'order_id', 'order_id',t('LBL970') . ' '. t('LBL884'));
  }
  /*if(isset($_GET['order_username']) && !empty($_GET['order_username'])){
    selectAddNarrowSearchByTextCondition($select, 'per', 'user_name', 'order_username',t('LBL970') . ' '. t('LBL054'));
  }*/
  if(isset($_GET['orderPrice']) && !empty($_GET['orderPrice'])){
    selectAddNarrowSearchSliderCondition($select, 'ucorder', 'order_total', 'orderPrice');
  }
  if(isset($_GET['daterange']) && !empty($_GET['daterange'])){
    selectAddNarrowSearchByDateRangeCondition($select, 'orders', 'created_on','daterange');
  }
  if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
    selectAddNarrowSearchByTextMultipleORCondition($select, "per", array("full_name")); // #39363 - Issue fix
  }
  $select->groupBy('orders.id');
  if($op != "COUNT"){
    manageOrderSearchSelectStmtAddOrderByClause($select, 'orders');
  }
 
  filterByUserAccessPrivileges($select, 'ucorder','',1,'cme_pmt','Order','orders');
  expDebug::dPrintDBAPI('$select before count', $select);
  if (strpos(cbOrderFiltersetSelectionsToPrintableStr('orderstatus'),'Completed') !== false) {
  	$orderStatusChange = str_replace("| Completed"," ",cbOrderFiltersetSelectionsToPrintableStr('orderstatus'));
  }
  else {
  	$orderStatusChange = cbOrderFiltersetSelectionsToPrintableStr('orderstatus');
  }

  if($callfrom == 'admincalendar')
  {
  	$search_type = $_REQUEST["search_type"];
	$search_text =  str_replace("Order by ","",$_REQUEST["search_text"]);
	if($search_type != null && $search_type != "" && (strtolower($search_text) != strtolower(t('LBL304'))))
   		$select->condition('per.user_name', '%' . db_like($search_text) . '%', 'LIKE');

  }

  if($op == "COUNT"){
  	$result = $select->execute()->rowCount();
	expDebug::dPrint('COUNT $result 111= ' . print_r($result,true),4);
	return $result;
  }
  elseif($op == "CSV"){
	  $colHeadersMap = array(t('LBL054')     => 'username',
	                         t('LBL884')     => 'uc_order_id',
	  											 t('LBL101')		 => 'currency_type',
                           t('LBL827')     => 'order_total',
                           t('LBL102')     => 'order_status');

    $filterArray[] = array(t('LBL1025').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL884').':'.textboxFiltersetValueToPrintableStr('order_id'));
    $filterArray[] = array(t('LBL102').':'.$orderStatusChange);
    $filterArray[] = array(t('LBL101').':'.cbOrderFiltersetSelectionsToPrintableStr('ordercurrency'));
    $filterArray[] = array(t('LBL040').':'.sliderFiltersetValueToPrintableStr('orderPrice'));
    $filterArray[] = array(t('LBL042').':'.daterangeFiltersetValueToPrintableStr('daterange'));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());

  	SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'order_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL054')     => array('username', 30),
                           t('LBL884')     => array('uc_order_id',15),
    											 t('LBL101')		 => array('currency_type',10),
                           t('LBL827')     => array('order_total', 30),
    					   t('LBL102') 	   => array('order_status', 25));


    $appliedFilters = array(t('LBL1025')  => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL884') => textboxFiltersetValueToPrintableStr('order_id'),
    						            t('LBL102') => $orderStatusChange,
    									t('LBL101') => cbOrderFiltersetSelectionsToPrintableStr('ordercurrency'),
						                t('LBL040') =>  sliderFiltersetValueToPrintableStr('orderPrice'),
						                t('LBL042') => daterangeFiltersetValueToPrintableStr('daterange'),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    );
    SendNarrowSearchResultsAsPDFFile($select, t('Orders'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'order_admin_search');
  }
  else {
  	
  		add_row_id_condition($select, 'orders', 'id');
	    // Limit to $limit records
	    if($callfrom != 'admincalendar')
  			$select->range($start, $limit);
	    expDebug::dPrintDBAPI('$select with range set', $select);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	    return $result;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchManageOrderAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function order_admin_search_csv_row_data_alter($rowData) {
  try {
    $rowData[0]->order_total  = number_format($rowData[0]->order_total, 2, '.', '');
    $rowData[0]->order_status = uc_order_status_data($rowData[0]->order_status, 'title');
  }
  catch (Exception $ex) {
    watchdog_exception('order_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function order_admin_search_pdf_row_data_alter($rowData) {
  try {
    $rowData[0]->order_total  = number_format($rowData[0]->order_total, 2, '.', '');
    $rowData[0]->order_status = uc_order_status_data($rowData[0]->order_status, 'title');
  }
  catch (Exception $ex) {
    watchdog_exception('order_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getOrderIdFilterAutoComplete() - Returns the matched state from uc_order table.
 */
function getOrderIdFilterAutoComplete(){
  try{
  $searchText =  $_GET['z'];
  // Select the table
  $select = orderSearchTables();
  // Add condition
  $select->condition('ucorder.order_id', '%' . db_like($searchText) . '%', 'LIKE');
  $select->orderBy('ucorder.order_id');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  $matching = $select->execute()->fetchAll();
  expDebug::dPrint(' $$matching = '. print_r($matching, true),3);

  foreach ($matching as $orderid) {
    print $orderid->uc_order_id . "\n";
  }
  }catch (Exception $ex) {
    watchdog_exception('getOrderIdFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function cbOrderFiltersetSelectionsToPrintableStr() - Returns the selected checkbox filters pipe separated
 *                                                  Assumed, all code is in
 */
function cbOrderFiltersetSelectionsToPrintableStr($filtersetCode) {
	try {
  $selectedFilters = getURLParam($filtersetCode);
  if($filtersetCode == "ordercurrency")
  		return $selectedFilters;
  expDebug::dPrint(' $selectedFilters = ' . print_r($selectedFilters, true) , 4);
  $selectedFiltersAsString = '';
  if (!empty($selectedFilters)){
    $checkBoxFiltersCodeList = explode("|", $selectedFilters);
    $firstCode = true;
    foreach ($checkBoxFiltersCodeList as $code) {
      $value = uc_order_status_data($code, 'title');
      if ($firstCode) {
        $selectedFiltersAsString .= $value;
        $firstCode = false;
      }
      else {
        $selectedFiltersAsString .= ' | ' . $value;
      }
    } // end foreach
  } // end if

  //$selectedFiltersAsString = (empty($selectedFiltersAsString))? 'All' : $selectedFiltersAsString;
  expDebug::dPrint(' $selectedFiltersAsString = ' . print_r($selectedFiltersAsString, true) , 4);

  return $selectedFiltersAsString;
  } catch (Exception $ex) {
  	watchdog_exception('cbOrderFiltersetSelectionsToPrintableStr', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 *  Get a admin comments from uc_order_admin_comments id
 */

function admin_comments_load($ucOrderId,$uid){
	try{
  $selectStmt = db_select('uc_order_admin_comments', 'ord');
  $selectStmt->condition('ord.order_id', $ucOrderId);
  //$selectStmt->condition('ord.uid', $uid , '!='); //commented this line for the ticket (#45829)
  $selectStmt->condition('ord.uid', 0 , '!=');
  $selectStmt->addField('ord', 'message');
  $selectStmt->addField('ord', 'comment_id');
  $selectStmt->range(0, 1);
  $selectStmt->orderBy('ord.comment_id','desc');
  expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
  $message = $selectStmt->execute()->fetchAll();
  return $message;
	}catch (Exception $ex) {
    watchdog_exception('admin_comments_load', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 *  Get a admin comments updated
 */
function adminOrderCommentUpdated($form_state){
	try{
	$updComment  = db_update('uc_order_admin_comments');
  $updComment->fields(array(
                'message' => $form_state['admin_comment'],
  ));
  $updComment->condition('comment_id', $form_state['admin_comment_id']);
  expDebug::dPrintDBAPI(' updComment SQL = ' , $updComment);
  $updComment->execute();
		  
		  //Don't delete this update this is report flat table related update.
		  $updComment  = db_update('slt_order');
		  $updComment->fields(array(
		  		'updated_on' => now(),
		  ));
		  $updComment->condition('uc_order_id', $form_state['mapped_order_id']);
		  expDebug::dPrintDBAPI(' uc_order_id SQL = ' , $updComment);
		  $updComment->execute();
	}catch (Exception $ex) {
    watchdog_exception('adminOrderCommentUpdated', $ex);
    expertusErrorThrow($ex);
  }
}

function exp_sp_administration_order_update_narrow_action($order_id, $order_update_status){
	try {
	expDebug::dPrint("exp_sp_administration_order_update_narrow_action : ".$order_id . "--- " . $order_update_status,5);
	if($order_id){
		$order = uc_order_load($order_id);
		$order_current_status = $order->order_status;
		$updateUser = getIdOfLoggedInUser();
		expDebug::dPrint(" order_current_status ".$order_current_status,4);
		expDebug::dPrint(" order_update_status ".$order_update_status,4);
		expDebug::dPrint(" order ".print_r($order,true),4);
		if ($order_update_status != $order_current_status && $order_current_status != 'canceled') {
			updateUserOrder($order_id, $order_update_status, $order_current_status, $updateUser);
		}
	}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_order_update_narrow_action', $ex);
		expertusErrorThrow($ex);
	}
}
function updatePoNumber($poNumber,$oderId) {
	try{
		$updateFields = array();
		$updateFields['po_number'] = $poNumber;
		$poUpdate = db_update('uc_payment_po');
		$poUpdate->fields($updateFields);
		$poUpdate->condition('order_id', $oderId,'=');
		expDebug::dPrintDBAPI('$updateFields', $updateFields);
		$poUpdate->execute();
	}catch (Exception $ex) {
		watchdog_exception('updatePoNumber', $ex);
		expertusErrorThrow($ex);
	}
}
function updateUserOrder($order_id, $order_update_status, $order_current_status, $updateUser, $callFromAPI = 0){
	try {
	if($order_update_status == 'canceled'){
		$updatedorder = uc_order_load($order_id);
		$sltOrderId = getSltOrderId($order_id);
		$compStsCount = getCancelStatusFromSltOrderId($sltOrderId);
		/* if($compStsCount){
			return;
		} */
		$refund_amount = 0;
		$cancelArray = array();
		foreach($updatedorder->products as $product){
			$nodeId = $product->nid;
			$entityDetails = getEntityDetailsFromNodeId($nodeId);
			$entityId   = $entityDetails[0]->entity_id;
			$entityType = $entityDetails[0]->entity_type;
			$orderStatus = getSltOrderItemDetails($sltOrderId, $entityId, $entityType);
			if($orderStatus[0]->order_status != 'cme_pmt_sts_rjt'){
				$discountAmt = getDropListDiscountAmount($nodeId,$order_id);
				$unit_price =  getDropListUnitPrice($nodeId,$order_id);
				$unit_price = $unit_price - $discountAmt;
				if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_cur' || $entityType == 'cre_sys_obt_trn'){
					$baseType 		= 'tp';
					$enrId 				= getMasterEnrIdFromOId($entityId, $sltOrderId);
					$waitlistItem = getDropTPWaitlistStatus($enrId);
				}
				else{
					$baseType 		= getClassDeliveryType($entityId, 'Short');
					$enrId 				= getEnrIdFromOId($entityId, $sltOrderId);
					$waitlistItem = getDropClsWaitlistStatus($enrId);
				}
				$taxForOrderItem = getChargedTaxForOrderItem($sltOrderId, $entityId, $baseType);
				$refundArray 		 = getRefundAmount($entityId, $baseType, $unit_price, $waitlistItem, $taxForOrderItem);
				$refund_amount 	+= $refundArray["refund_value"];

				$cancelStdObj = new stdClass();
				$cancelStdObj->refund_amount = $refundArray["refund_value"];
				$cancelStdObj->base_type     = $baseType;
				$cancelStdObj->enr_id        = $enrId;
				$cancelStdObj->wait_list     = $waitlistItem;
				$cancelStdObj->entity_id     = $entityId;
				$cancelArray[] = $cancelStdObj;
			}
		}
		expDebug::dPrint('cybersource  refund_amount= ' . print_r($refund_amount, true) , 4);
		expDebug::dPrint('cybersource  cancelArray= ' . print_r($cancelArray, true) , 4);
		$canCanceEnr = 1;
		//  Condition is included to avoid refund process flow as PAYMETRIC has not implemented with refund method
		$payment_gateway = variable_get('uc_payment_credit_gateway','');
		if(($updatedorder->payment_method == 'credit' && $payment_gateway != 'exp_paymetric') && $order_current_status != 'canceled' && $refund_amount > 0){
			//Refund Logic here
			$currency_type = getCurrencyTypeFromEntityId($sltOrderId);
			if($currency_type == null || $currency_type=='')
			{
				$currency_type	= ''; // 0056924 - default $ removal
			}
			$reply = refund_cc($sltOrderId, $order_id, $refund_amount,"",$currency_type);
			expDebug::dPrint(' $reply from cybersource = ' . print_r($reply, true) , 4);
			if($reply->serviceresponse=="success"){
				$canCanceEnr = 1;
			}
			else{
				$canCanceEnr = 0;
			}
		}
		if(count($cancelArray)){
			$selectStmt = db_select('slt_order', 'sor');
			$selectStmt->condition('sor.id', $sltOrderId);
			$selectStmt->addField('sor', 'user_id');
			$selectStmt->range(0, 1);
			expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
			$userId = $selectStmt->execute()->fetchField();
			foreach($cancelArray as $attributes){
				$refund_amt = $attributes->refund_amount;
				$baseType			 = $attributes->base_type;
				$enrId 				 = $attributes->enr_id;
				$entityId 		 = $attributes->entity_id;
				if($canCanceEnr && $enrId){
					if($baseType == 'tp'){
						$param =  array();
						$param['master_enroll_id'] = $enrId;
						$param['prg_id'] 					 = $entityId;
						$param['is_refunded']      = 1;
						$param['refund_amount']    = $refund_amt;
						$param['user_id']          = $userId;
						$param['update_user_id']   = $updateUser;
						$param['cancel_from']      = 'admin';
						$param['uc_order_id']			 = $order_id;
						$param['no_notify']   		 = 1;
						$param['from_manager']  	 = 0;
						dropTPLineItem($param);
					}
					else{
						$param =  array();
						$param['enroll_id'] 			 = $enrId;
						$param['is_refunded']      = 1;
						$param['refund_amount']    = $refund_amt;
						$param['user_id']          = $userId;
						$param['update_user_id']   = $updateUser;
						$param['cancel_from']      = 'admin';
						$param['uc_order_id']			 = $order_id;
						$param['from_instructor']  = 0;
						$param['from_manager'] 		 = 0;
						$param['base_type']        = strtoupper($baseType);
						$param['no_notify']   		 = 1;
						dropClassLineItem($param);
					}
				}
			}
		}
		else if($canCanceEnr){
			cancelUpertCartOrder($order_id, $sltOrderId, $updateUser);
		}

		if($canCanceEnr && !$callFromAPI){
			drupal_set_message(t('MSG576'),'status',false);
		}
	}
	else{
		if(uc_order_update_status($order_id, $order_update_status)) {
			//Added for LMS logic implementation in admin update the order
			$updatedorder = uc_order_load($order_id);
			$updatedorder->update_from = 'admin';
	    exp_sp_cart_uc_checkout_complete($updatedorder, $callFromAPI);
	    if(!$callFromAPI){
	    	drupal_set_message(t('MSG576'),'status',false);
	    }

	    //Audit trail for order status change
	    $changefields = array(
	      'order_id'  => $order_id,
	      'status'    => $order_update_status,
	    );
	    $default[0] = new stdClass();
	    $default[0]->order_id = $order_id;
	    $default[0]->status   = $order_current_status;
	    make_audit_trail_entries($changefields, $updateUser , $default ,$order_id, 'Updated','Commerce','exp_sp_administration_order','updateUserOrder',
	                                          array('order_id', 'status'));
	  }
	}
	} catch (Exception $ex) {
		watchdog_exception('updateUserOrder', $ex);
		expertusErrorThrow($ex);
	}
}

function updateOrderByRestAPI( &$form_state,$updateUser="")
{
	try {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $order_id            = $form_state['values']['order_id'];
  $order_update_status = $form_state['values']['change_status'];
	if($order_id){
	  $order = uc_order_load($order_id);
	  $order_current_status = $order->order_status;
	  if(!$order_current_status || ($order_update_status != 'payment_received' && $order_update_status != 'canceled') || $order_current_status == 'in_checkout' || $order_current_status == 'canceled'){
	  	$errobj=new stdClass();
	  	$errobj->isValidateError = 1;
	  	$errobj->errcode = 'L_012';
	  	$errobj->errormsg = "Unable to update the order";
	  	return $errobj;
	  }
	  else if ($order_update_status == $order_current_status) {
	  	$errobj=new stdClass();
	  	$errobj->isValidateError = 1;
	  	$errobj->errcode = 'L_012';
	  	$errobj->errormsg = "Unable to update the order";
	  	return $errobj;
	  }
		else if(($order_current_status == 'payment_received' || $order_current_status == 'completed') && $order_update_status != 'canceled' ){
	   	 $errobj=new stdClass();
	  	 $errobj->isValidateError = 1;
	  	 $errobj->errcode = 'L_012';
	  	 $errobj->errormsg = "Unable to update the order";
	  	 return $errobj;
	  }
	  else{
	 	  updateUserOrder($order_id, $order_update_status, $order_current_status, $updateUser,1);
	 	  return array((object)array('id'=>$order_id));
	  }
	}
	} catch (Exception $ex) {
		watchdog_exception('updateOrderByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function getCancelStatusFromSltOrderId($sltOrderId){
	try{
	if(!$sltOrderId){
		return 0;
	}
  $selectStmt = db_select('slt_enrollment', 'enr');
	$selectStmt->addExpression('COUNT(1)', 'count');
  $selectStmt->condition('enr.order_id', $sltOrderId);
  $selectStmt->condition('enr.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_att'), 'IN');
  expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
  $compCount = $selectStmt->execute()->fetchField();
  return $compCount;
	}catch (Exception $ex) {
    watchdog_exception('getCancelStatusFromSltOrderId', $ex);
    expertusErrorThrow($ex);
  }
}

function dropPolicyListItem($ucOrderId,$nodeId,$enrId){
	try{
		if($ucOrderId && $nodeId){
			$lmsOrderId = getSltLMSOrderId($ucOrderId);
			$entityDetails= getEntityDetailsFromNodeId($nodeId);
			$entityId   = $entityDetails[0]->entity_id;
			$entityType = $entityDetails[0]->entity_type;
			if($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_cur' || $entityType == 'cre_sys_obt_trn'){
				$compStatus = getCancelStatusFromEnrId($enrId,'tp');
				/* if($compStatus){
					return;
				} */
				$waitlistItem = getDropTPWaitlistStatus($enrId);
				$baseType = 'tp';
			}
			else{
				$compStatus = getCancelStatusFromEnrId($enrId,'class');
				/* if($compStatus){
					return;
				} */
				$waitlistItem = getDropClsWaitlistStatus($enrId);
				$baseType = getClassDeliveryType($entityId, 'Short');
			}

			$discountAmt = getDropListDiscountAmount($nodeId,$ucOrderId);
			$unit_price =  getDropListUnitPrice($nodeId,$ucOrderId);
			$unit_price = $unit_price - $discountAmt;
			$taxForOrderItem = getChargedTaxForOrderItem($lmsOrderId, $entityId, $baseType);
			$refundArray = getRefundAmount($entityId, $baseType, $unit_price, $waitlistItem, $taxForOrderItem);
			$refund_amount = $refundArray["refund_value"];
			$currency_type = getCurrencyTypeFromEntityId($lmsOrderId);
			if($currency_type == null || $currency_type=='')
			{
				$currency_type	= ''; // 0056924 - default $ removal
			}
			$paymentType = getPaymentTypeFromUcOrderId($ucOrderId);
			$canCanceEnr = 1;
			//  Condition is included to avoid refund process flow as PAYMETRIC has not implemented with refund method
			$gateway = variable_get('uc_payment_credit_gateway','');
			if($paymentType == 'credit' && $gateway != 'exp_paymetric'){
				if($refund_amount > 0) {
					$reply = refund_cc($lmsOrderId, $ucOrderId, $refund_amount,"",$currency_type);
					expDebug::dPrint(' $reply from cybersource = ' . print_r($reply, true) , 4);
					if($reply->serviceresponse=="success"){
						$canCanceEnr = 1;
					}
					else{
						$canCanceEnr = 0;
					}
				}
			}
			if($canCanceEnr){
				$selectStmt = db_select('slt_order', 'sor');
				$selectStmt->condition('sor.id', $lmsOrderId);
				$selectStmt->addField('sor', 'user_id');
				$selectStmt->range(0, 1);
				expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
				$userId = $selectStmt->execute()->fetchField();
				if($baseType == 'tp'){
					$param =  array();
					$param['master_enroll_id'] = $enrId;
					$param['prg_id'] 					 = $entityId;
					$param['is_refunded']      = 1;
					$param['refund_amount']    = $refund_amount;
					$param['user_id']          = $userId;
					$param['update_user_id']   = getSltpersonUserId();
					$param['cancel_from']      = 'admin';
					$param['uc_order_id']			 = $ucOrderId;
					$param['from_manager']  	 = 0;
					dropTPLineItem($param);
				}
				else{
					$param =  array();
					$param['enroll_id'] 			 = $enrId;
					$param['is_refunded']      = 1;
					$param['refund_amount']    = $refund_amount;
					$param['user_id']          = $userId;
					$param['update_user_id']   = getSltpersonUserId();
					$param['cancel_from']      = 'admin';
					$param['uc_order_id']			 = $ucOrderId;
					$param['from_instructor']  = 0;
					$param['from_manager']  	 = 0;
					$param['base_type']        = strtoupper($baseType);
					dropClassLineItem($param);
				}
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('dropPolicyListItem', $ex);
		expertusErrorThrow($ex);
	}
}


function getEntityDetailsFromNodeId($nodeId){
	try{
		$selectStmt = db_select('slt_node_learning_activity', 'nde');
		$selectStmt->condition('nde.node_id', $nodeId);
		$selectStmt->addField('nde', 'entity_id');
		$selectStmt->addField('nde',	'entity_type');
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		$results = $selectStmt->execute()->fetchAll();
		return $results;
	}catch (Exception $ex) {
		watchdog_exception('getEntityDetailsFromNodeId', $ex);
		expertusErrorThrow($ex);
	}
}

function getDropTPWaitlistStatus($masterEnrId){
	try{
		$selectStmt = db_select('slt_master_enrollment', 'menr');
		$selectStmt->condition('menr.id', $masterEnrId);
		$selectStmt->addField('menr', 'overall_status');
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		$results = $selectStmt->execute()->fetchField();
		return $results;
	}catch (Exception $ex) {
		watchdog_exception('getDropTPWaitlistStatus', $ex);
		expertusErrorThrow($ex);
	}
}

function getDropClsWaitlistStatus($enrId){
	try{
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->condition('enr.id', $enrId);
		$selectStmt->addField('enr', 'waitlist_flag');
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		$results = $selectStmt->execute()->fetchField();
		return $results;
	}catch (Exception $ex) {
		watchdog_exception('getDropClsWaitlistStatus', $ex);
		expertusErrorThrow($ex);
	}
}

function getCurrencyTypeFromEntityId($orderId){
	try{
		$selectCurrencyTyp = db_select('slt_order','curr');
		$selectCurrencyTyp->addField('curr','currency_type');
		$selectCurrencyTyp->condition('curr.id', $orderId);
		$currTyp = $selectCurrencyTyp->execute()->fetchField();
		return $currTyp;
	}catch (Exception $ex) {
		watchdog_exception('getCurrencyTypeFromEntityId', $ex);
		expertusErrorThrow($ex);
	}
}

function getPaymentTypeFromUcOrderId($ucOrderId){
	try{
		$selectType = db_select('uc_orders','uo');
		$selectType->addField('uo','payment_method');
		$selectType->condition('uo.order_id', $ucOrderId);
		$payType = $selectType->execute()->fetchField();
		return $payType;
	}catch (Exception $ex) {
		watchdog_exception('getPaymentTypeFromUcOrderId', $ex);
		expertusErrorThrow($ex);
	}
}

function loadUserCreateOrder($searchKeyword, $excludedId){
 try {
	$page      = $_GET['page'];
	$limit     = $_GET['rows'];
	$sidx      = $_GET['sidx'];
	$sord      = $_GET['sord'];
	$uniqueId  =  'add-user-admin-order';

	$count = 0;
	expDebug::dPrint('loadUserCreateOrder  ---> '.$mode ." ------ ".$searchKeyword."-----".$excludedId,4);
	$searchType = $_GET['searhType'];
	$listquery = getUsersQueryForOrderCreate($searchKeyword,$excludedId);

	$count = count($listquery->execute()->fetchAll());
	$xorderBy = ($sidx != '') ? $sidx : 'full_name';
	$listquery->orderBy($xorderBy,$sord);
	/* Paging Related */
	$start = $limit * $page - $limit;
	$listquery->range($start, $limit);
	// Execute query
	$queryResult = $listquery->execute();

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

	$srch_result_response->total     = $total_pages;
	$srch_result_response->records 	 = $count;
	$srch_result_response->page      = $page;
	$srch_result_response->unique_id = $uniqueId;

	$_SESSION['jq_count'] = array();
	$i=0;
	expDebug::dPrint('grid data  loadUserCreateOrder---> '.print_r($queryResult,true),4);
	foreach ($queryResult as $rec_search_results){
		$srch_result_response->rows[$i]['id']   = $rec_search_results->user_id;
		$srch_result_response->rows[$i]['cell'] = array(
				'id' 	              => $rec_search_results->user_id,
				'user_name' 	      	=> $rec_search_results->user_name,
				'full_name' 	      	=> $rec_search_results->full_name,
				'org_name' 	      	=> ($rec_search_results->org_name) ? $rec_search_results->org_name : '',
				'AddUserOrder'  		=> $rec_search_results->drupal_uid,
		);
		$i++;
	}
	echo drupal_json_encode($srch_result_response);
	} catch (Exception $ex) {
		watchdog_exception('loadUserCreateOrder', $ex);
		expertusErrorThrow($ex);
	}
}

function getUsersQueryForOrderCreate($searchKeyword, $excludedId){
	try {
	global $user;
	$excludedUserId = array();
	$excludedUserId[] = $user->uid;
	if($excludedId){
		$excludedUserId[] = $excludedId;
	}
	$select = db_select('slt_person','per');
	$select->leftJoin('users', 'u', 'u.name = per.user_name');
	$select->leftJoin('slt_organization', 'org', 'org.id = per.org_id');
	$select->addField('u','uid','drupal_uid');
	$select->addField('per','id','user_id');
	$select->addField('per','user_name');
	$select->addField('per','full_name');
	$select->addField('org','name','org_name');
	if(count($excludedUserId)){
		$select->condition('u.uid',$excludedUserId,'NOT IN');
	}
	$select->condition('per.user_name',array('admin','guest'),'NOT IN');
	if(!empty($searchKeyword) && $searchKeyword != t('LBL036').' '.t('LBL107').' '.t('LBL644').' '.t('LBL054')){
      $select->condition(db_or()->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')
                                ->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
  }
	$select->condition('per.status','cre_usr_sts_atv');
	//$select->orderBy('per.user_name');
	expDebug::dPrintDBAPI('list of users in order' ,   $select);
	return $select;
	} catch (Exception $ex) {
		watchdog_exception('getUsersQueryForOrderCreate', $ex);
		expertusErrorThrow($ex);
	}
}

function getAdminOrderUserAutoComplete() {
	try {
	global $user;
	$searchText =  $_GET['z'];
	$excludedUserId = array();
	if($_GET['excluded_user_id']){
		$excludedUserId[] = urldecode($_GET['excluded_user_id']);
	}
	$excludedUserId[] = $user->uid;
	$select = db_select('slt_person', 'per');
	$select->leftJoin('users', 'u', 'u.name = per.user_name');
	$select->addField('per', 'user_name');

	if(!empty($searchText)){
  	$select->condition('per.user_name', '%' . db_like($searchText) . '%' ,'like');
  }
  $select->condition('per.user_name',array('admin','guest'),'NOT IN');
  $select->condition('per.status','cre_usr_sts_atv','=');
	if (count($excludedUserId)) {
		$select->condition('u.uid', $excludedUserId, 'NOT IN');
	}
	$select->orderBy('per.user_name');
	expDebug::dPrint('query for order autocomplete',$select);
	$matchingNames = $select->execute()->fetchAll();
	expDebug::dPrint(' $$matchingNames = '. print_r($matchingNames, true) , 3);

	foreach ($matchingNames as $userName) {
		print $userName->user_name . "\n";
	}
	} catch (Exception $ex) {
		watchdog_exception('getAdminOrderUserAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

function loadOrderAddProduct($searchKeyword, $userId){
	try {
	module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
	$sidx      = $_GET['sidx'];
	$sord      = $_GET['sord'];
    
	$srcSource = getConfigValue('solr_search');
	if($srcSource == 0){
	  $lnrSrch = new LnrSearchCatalog();
		$resultQryCnt = qryForloadProduct($lnrSrch,$searchKeyword, $userId , 0,'AddTraining');
	//expDebug::dPrintDBAPI('values for the query result',$resultQryCnt);
		$lnrSrch->select = $resultQryCnt;
		$lnrSrch->calculatePagination(1);
	//$start = $lnrSrch->start;

		$limit = $lnrSrch->limit;
		$totalPages = $lnrSrch->total_pages;
		$countQry = $lnrSrch->qry_search_count;
		$page = $lnrSrch->page;

	// Below code is handeld in exp_sp_administration_learning.inc (in loadCtoolDataGridType())
  // Update both file if any changes required.
		$crPage = explode('~',$_COOKIE['current_page']);
		if(!empty($page) && $page==1 && $type != "trainings"){
			$isUpdate = explode('#',$crPage[2]);
			if($isUpdate[1]==1){
				$page= ($isUpdate[0])!=0?$isUpdate[0]:1;
		}
	}
	$start = getOffsetOfFirstRecordInPage($page, $totalPages, $limit, $countQry);
	expDebug::dPrint('start values for order screen available trainings'.$countQry,3);

	$lnrSrch = new LnrSearchCatalog();
	$resultQry = qryForloadProduct($lnrSrch, $searchKeyword, $userId , 1,'AddTraining');
	$xorderBy = ($sidx != '') ? $sidx : 'cls_title';
	$lnrSrch->select->orderBy($xorderBy,$sord);
	$lnrSrch->select->range($start, $limit);
	$lnrSrch->select = $resultQry;


	$qry_search_results = $lnrSrch->select->execute()->fetchAll();
	expDebug::dprint('result of add product------------->'.print_r($qry_search_results,true),4);
	expDebug::dPrintDBAPI('Over all Product qry----->' ,   $lnrSrch->select);
	}
	
	else {
	    require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
	    try{
	        expDebug::dPrint("Solr Search Testing",1);
	        $solrObj = new CatalogSolrSearch();
	        expDebug::dPrint("Call solr my oder result",1);
	        $search_results = $solrObj->getSolrSearchResultMyorder();
	        expDebug::dPrint("Called success solr my oder result",1);
	        expDebug::dPrint('Result set from solr main result'.print_r($search_results,true),5);
	        $qry_search_results = $search_results['records'];
	        expDebug::dPrint('Result set from solr'.print_r($qry_search_results,true),5);
	        $count = $search_results['recCount'];
	        expDebug::dPrint('Result set count'.print_r($count,true),5);
	        
// 	        $lnrSrch->qry_search_count = $search_results['recCount'];
// 	        $lnrSrch->page    = $_GET['page'];
// 	        $lnrSrch->total_pages   = ceil($lnrSrch->qry_search_count/$_GET['rows']);
	        
	       // $limit = $lnrSrch->limit;
	        $totalPages = ceil($search_results['recCount']/$_GET['rows']);
	        $countQry = $search_results['recCount'];
	        $page = $_GET['page'];
	        
	        expDebug::dPrint('Result set count $totalPages'.print_r($totalPages,true),4);
	        expDebug::dPrint('Result set count $countQry'.print_r($countQry,true),4);
	        expDebug::dPrint('Result set count $page'.print_r($page,true),4);
	        
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
	    }
	    expDebug::dPrint("Solr search result - Final ".print_r($search_results,1),4);
	}
	
	foreach($qry_search_results as $key => $valarr){
		foreach($valarr as $keyval=>$arrval){
		    expDebug::dPrint('arr val changedLATHA---'.print_r($key,true),5);
		    expDebug::dPrint('arr val changedLATHA-->>> '.print_r($valarr,true),5);
			if( $keyval == 'cls_title' ||  $keyval == 'cls_short_description')
					$qry_search_results[$key]->$keyval = htmlentities($arrval, ENT_QUOTES,"UTF-8");
			if($keyval == 'object_type'){   // Changing object type to class to enable add to cart -- modified by latha
			    //vcl,wbt,vod,ilt
			    expDebug::dPrint('object type keyval', 1);
			    expDebug::dPrint('object type keyval valarr'.print_r($arrval,true),5);
			    if($arrval == 'lrn_cls_dty_ilt' || $arrval == 'lrn_cls_dty_vcl' || $arrval == 'lrn_cls_dty_wbt' || $arrval == 'lrn_cls_dty_vod')
			    {
			        
			        $qry_search_results[$key]->$keyval = "Class";
			        expDebug::dPrint('arr val changed111111111111'.print_r($qry_search_results[$key]->$keyval,true),5);
			    }
			}
		}
	}
	
	
	 $srch_result_response->total   = $totalPages;
	
	$srch_result_response->records = $countQry;
	
	$srch_result_response->page    = $page;
	
	$srch_result_response->unique_id =  'add-product-admin-order';
	$srch_result_response->userId  = $userId; 
	
	//Added by Subin. Ticket #59698
		$getUserInfo = getPersonDetails($userId,array('preferred_currency'));
        $getUserPrefCur = $getUserInfo['preferred_currency'];
        $allow_currency_change = restrict_user_currency_select();
        $getCurrencySymbol = getCurrencySymbol($getUserPrefCur);
		$preferredCurrencysym = empty($getCurrencySymbol['attr2'])? '' : $getCurrencySymbol['attr2'];
        if(!empty($userId) && ($getUserPrefCur != $_SESSION['admin_shop_cart_currency_code']) && $allow_currency_change == TRUE){
            $_SESSION['admin_shop_cart_currency_code'] = $getUserPrefCur;
            $_SESSION['admin_shop_cart_currency_sym'] = $preferredCurrencysym;
        }
        
	$_SESSION['jq_count'] = array();
	$i=0;
	foreach ($qry_search_results as $rec_search_results) {
		// Modified for Multi currency support
		if($rec_search_results->currency_type==null || $rec_search_results->currency_type=='') {
			$rec_search_results->currency_type='$';
		}

		if($rec_search_results->export_compliance == null)
			$rec_search_results->export_compliance = 'N';

		if($rec_search_results->price == null || $rec_search_results->price == '')
			$rec_search_results->price = '0.00';
		else{
		  //$userDefaultCurrencyArr = getUserDefaultCurrency();
		  $courrency_code = (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))? $_SESSION['admin_shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];
		  $rec_search_results->price = db_query('select slf_convert_price('.$rec_search_results->price.',\''.$rec_search_results->currency_code.'\',\''.$courrency_code.'\') ')->fetchField();
		}

		$rec_search_results->row =  $i;
		$srch_result_response->rows[$i]['id']   = $rec_search_results->cls_id;

		if($rec_search_results->object_type == 'Class') {
			 $rec_search_results->available_seats = getAvailableSeats($rec_search_results->cls_id);
			$rec_search_results->node_id 				 = getNodeIdResults($rec_search_results->cls_id,'cre_sys_obt_cls');

			//$rec_search_results->session_details = getSessionDetails($rec_search_results->crs_id,$rec_search_results->cls_id,$rec_search_results->delivery_type_code);
			$rec_search_results->multi_register  = is_multiregistration_allowed($rec_search_results->cls_id);
			$rec_search_results->waitlist_seats  = getWaitlistCatalogInfo($rec_search_results->cls_id);

			$condCount = 1;
			$courseID  = $rec_search_results->crs_id;
			$classID   = $rec_search_results->cls_id;
			$pastDateCount    = checkPastDateOrNot($classID,$rec_search_results->delivery_type_code);
			$preRequisite     = checkCatalogPrerequisite($courseID,'cre_sys_obt_crs',$userId);
			$getRegister      = getRegisteredOrNot($courseID,$classID,$userId);
			$registerEndDateCheck = 0;
			$startdate = Date("Y-m-d");
			if($rec_search_results->registration_end_on !='' || $rec_search_results->registration_end_on != null) {
				if (strtotime($rec_search_results->registration_end_on) < strtotime($startdate)) {
					$registerEndDateCheck = 1;
				}
			}
			
			$rec_search_results->enrolled_id         = $getRegister->enrolled_id;
			$rec_search_results->comp_status         = $getRegister->comp_status;
			$rec_search_results->enrolled_status     = $getRegister->enrolled_status;
			$rec_search_results->waitlist_flag       = $getRegister->waitlist_flag;
			$rec_search_results->waitlist_priority   = $getRegister->waitlist_priority;
			$rec_search_results->is_cart_added       = isAdminCartAdded($rec_search_results->node_id);
			//$rec_search_results->session_details 		 = ($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt" || $rec_search_results->delivery_type_code == "lrn_cls_dty_vcl") ? getSessionDetails($rec_search_results->crs_id,$rec_search_results->cls_id,$rec_search_results->delivery_type_code,'',$userId) : '';

			$waitlist   = 0;
			$waitlist_register = 0;
			$waitlisted = (($rec_search_results->enrolled_status == 'lrn_crs_reg_wtl') || ($rec_search_results->waitlist_flag == 'lrn_crs_reg_wtl') ) ? 1 : 0;
			if(($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt" || $rec_search_results->delivery_type_code == "lrn_cls_dty_vcl") && (($rec_search_results->available_seats == 0) || ($rec_search_results->enrolled_status == 'lrn_crs_reg_wtl') || ($rec_search_results->waitlist_flag == 'lrn_crs_reg_wtl'))) {
					$waitlist_register  = $rec_search_results->waitlist_seats;
					$waitlist           = 1;
			}
			$trainingStatus = t('LBL645');
			$canAdded = 1;
			if(($rec_search_results->enrolled_id) && $rec_search_results->multi_register == 0 ) {
				$trainingStatus       = (($rec_search_results->comp_status == 'lrn_crs_cmp_cmp') ? t('Completed') : ((($rec_search_results->enrolled_status == 'lrn_crs_reg_wtl') || ($rec_search_results->waitlist_flag == 'lrn_crs_reg_wtl')) ? t('LBL190') : t('Registered')));
				$waitlist_register  = ($trainingStatus !== t('LBL190')) ? 0 : $waitlist_register;
				$canAdded = 0;
			}
			else if((($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt" || $rec_search_results->delivery_type_code == "lrn_cls_dty_vcl") && $rec_search_results->available_seats == 0 && $rec_search_results->waitlist_seats == 0) && ($condCount == 1)) {
				$trainingStatus =  t('LBL046');
				$canAdded = 0;
			}
			else if((($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt" || $rec_search_results->delivery_type_code == "lrn_cls_dty_vcl") && $pastDateCount > 0) && ($condCount == 1)) {
				$trainingStatus =  t('LBL105');
				$canAdded = 0;
			}
			else if(($registerEndDateCheck <> 0) && ($condCount == 1)) {
				$trainingStatus =  t('LBL047');
				$canAdded = 0;
			}
			else{
				if(count($preRequisite)>0) {
					$trainingStatus =  t('LBL230');
					$canAdded = 0;
				}
				else if($condCount == 1)  {
					if($rec_search_results->is_cart_added){
						$trainingStatus =  t('LBL049');
						$canAdded = 0;
					}
					else if(($rec_search_results->price)){
						if($waitlist_register > 0){
							$trainingStatus =  t('Waitlist');
						}
						else{
							$trainingStatus =  t('LBL645');
						}
						$canAdded = 1;
					}
				}
			}

			//Added by Shobana on Oct 28, 2013 for #0025128
		/*	if($rec_search_results->delivery_type_code == "lrn_cls_dty_vcl"){
				if(count($rec_search_results->session_details) > 0) {
					$sDay = $rec_search_results->session_details[0]['session_start_date_format'];
					$sTime = $rec_search_results->session_details[0]['session_start_time_format'];
					$sTimeForm = $rec_search_results->session_details[0]['session_start_time_form'];

					if(count($rec_search_results->session_details) > 1) {
						$sessLenEnd = count($rec_search_results->session_details)-1;
						$eDay = $rec_search_results->session_details[$sessLenEnd]['session_start_date_format'];
						$eTime = $rec_search_results->session_details[$sessLenEnd]['session_start_end_format'];
						$eTimeForm = $rec_search_results->session_details[$sessLenEnd]['session_end_time_form'];

						$sessionDate = $sDay .' '. $sTime .' '.$sTimeForm.' to '.$eDay. ' ' .$eTime.' '.$eTimeForm;
					}else {

						$eTime = $rec_search_results->session_details[0]['session_start_end_format'];
						$eTimeForm = $rec_search_results->session_details[0]['session_end_time_form'];

						$sessionDate = $sDay .' '. $sTime.' '.$sTimeForm.' to '.$eTime.' '.$eTimeForm;
					}

				}
			}elseif($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt"){
				if(count($rec_search_results->session_details) > 0) {
					$sDay = $rec_search_results->session_details[0]['ilt_session_start_date_format'];
					$sTime = $rec_search_results->session_details[0]['ilt_session_start_time_format'];
					$sTimeForm = $rec_search_results->session_details[0]['ilt_session_start_time_form'];
					expDebug::dPrint('session date details $sTimeForm'.print_r($sTimeForm,true));
					if(count($rec_search_results->session_details) > 1) {
						$sessLenEnd = count($rec_search_results->session_details)-1;
						$eDay = $rec_search_results->session_details[$sessLenEnd]['ilt_session_start_date_format'];
						$eTime = $rec_search_results->session_details[$sessLenEnd]['ilt_session_start_end_format'];
						$eTimeForm = $rec_search_results->session_details[$sessLenEnd]['ilt_session_end_time_form'];

						$sessionDate = $sDay .' '. $sTime .' '. $sTimeForm.' to '.$eDay. ' ' .$eTime.' '.$eTimeForm;
					}else {

						$eTime = $rec_search_results->session_details[0]['ilt_session_start_end_format'];
						$eTimeForm = $rec_search_results->session_details[0]['ilt_session_end_time_form'];

						$sessionDate = $sDay .' '. $sTime.' '.$sTimeForm.' to '.$eTime.' '.$eTimeForm;
					}
				}
			} */
			expDebug::dPrint('session date details'.print_r($sessionDate,true),4);
			$passData = array();
			$passData['canAdded']  = $canAdded;
			$passData['NodeId'] 	 = $rec_search_results->node_id;
			$passData['ClassId']	 = $classID;
			$passData['CourseId']	 = $courseID;
			$passData['lmsUId'] 	 = $userId;
			$passData['ObjectType']= 'Class';
			$passData['is_cart_added'] = $rec_search_results->is_cart_added;

		}else{
			$rec_search_results->course_to_complete = getCoursestobeComplete($rec_search_results->cls_id);

			$rec_search_results->node_id = getNodeIdResults($rec_search_results->cls_id,$rec_search_results->object_type);

			$resultsMasterEnroll = getObjectRegisteredOrNot($rec_search_results->cls_id,$userId);

			$rec_search_results->is_cart_added = isAdminCartAdded($rec_search_results->node_id);
			$passData = "data={'NodeId' :'".$rec_search_results->node_id."','TpId':'".$rec_search_results->cls_id."'}";

			$preRequisite = checkCatalogPrerequisite($rec_search_results->cls_id,'cre_sys_obt_trp',$userId);

			$rec_search_results->prm_available_seats = getObjectAvailableSeats($rec_search_results->cls_id,$userId);

			$rec_search_results->waitlist_status=-1;

			if($rec_search_results->prm_available_seats == 0)
				$rec_search_results->waitlist_status = getProgramWaitlistStatus($rec_search_results->cls_id,$userId);

			if(($rec_search_results->waitlist_status > 0) && ($resultsMasterEnroll->master_enrolled_id == '' || $resultsMasterEnroll->master_enrolled_id == null || $resultsMasterEnroll->master_enrolled_id == 0)) {
				$waitlist_register  = $rec_search_results->waitlist_status;
				$waitlist           = 1;
			} else {
				$waitlist_register = 0;
				$waitlist          = 0;
			}

			if($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_wtl') {
				$waitlist_st = 1;
			} else {
				$waitlist_st = 0;
			}

			$trainingStatus = t('LBL645');
			$canAdded = 1;
			$registerStatus = '';

			if($resultsMasterEnroll->master_enrolled_id) {
				$canAdded = 0;
				if($rec_search_results->object_type == 'cre_sys_obt_crt' && $resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_exp') {
					$trainingStatus = t('LBL429');
				}
				else{
					$trainingStatus = ($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_cmp') ? t('Completed') : (($waitlist_st > 0) ? t('LBL190') : t('Registered'));
				}
			}
			else if($rec_search_results->prm_available_seats == 0 && $rec_search_results->waitlist_status == 0) {
				$trainingStatus = t('LBL046');
				$canAdded = 0;
			}
			else {
				if(count($preRequisite)>0) {
					$trainingStatus =  t('LBL230');
					$canAdded = 0;
				}
				else  if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce))  {
					if($rec_search_results->is_cart_added){
						$trainingStatus =  t('LBL049');
						$canAdded = 0;
					}
					else if(($rec_search_results->price)){
						if($waitlist_register > 0){
							$trainingStatus =  t('Waitlist');
						}
						else{
							$trainingStatus =  t('LBL645');
						}
						$canAdded = 1;

						/* NO CLASS CHECKING FUNCTIONALITY */
						$maxCount = getCourseClassCountForLP($rec_search_results->cls_id,$userId,'N');

						if($maxCount[0]->cnt == '-1' || $maxCount[0]->cnt == 0){
							$trainingStatus = t('LBL636');
							$canAdded = 0;  	// no classes under any one of the courses
						} else if($maxCount[0]->cnt == 1) {
							$registerStatus = "singleclass";
						} else {
							$registerStatus = "multiregister";
						}
					}
				}
			}
			$passData = array();
			$passData['canAdded']  = $canAdded;
			$passData['NodeId'] 	 = $rec_search_results->node_id;
			$passData['PrgId']	   = $rec_search_results->cls_id;
			$passData['lmsUId'] 	 = $userId;
			$passData['RegStatus'] = $registerStatus;
			$passData['ObjectType']= $rec_search_results->object_type;
			$passData['RegUserId'] = $userId;
			$passData['is_cart_added'] = $rec_search_results->is_cart_added;
		}
		$location = ($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt") ? getLocationNameDts($rec_search_results->location_id) : '';

		if($rec_search_results->delivery_type_code == "lrn_cls_dty_ilt") {
			$locationSessionDet = t($rec_search_results->delivery_type_name) . ' | '.t($location) .' | '. $sessionDate;
		}
		else if($rec_search_results->delivery_type_code == "lrn_cls_dty_vcl") {
			$locationSessionDet = t($rec_search_results->delivery_type_name) . ' | ' .$sessionDate;
		}else{
			$locationSessionDet = t($rec_search_results->delivery_type_name);
		}

// 		if($userId > 0)
//     	$userInfo = getUserInformations($userId);
//     else
//     	$userInfo = getDefaultCurrency();
		$selected_option=(isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))?$_SESSION['admin_shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];
		$getCurrencySymbol = getCurrencySymbol($selected_option);
		$currencySymbol = empty($getCurrencySymbol['attr2'])? '' : $getCurrencySymbol['attr2'];
		//$currencySymbol = (isset($_SESSION['admin_shop_cart_currency_sym']) && !empty($_SESSION['admin_shop_cart_currency_sym']))?$_SESSION['admin_shop_cart_currency_sym']:$_SESSION['user_preferred_currency_sym'];

		expDebug::dPrint('FINAL CURRENY SYM---->'.print_r($currencySymbol,1),4);

		$srch_result_response->rows[$i]['id']   = $i;
		//$delTypeNme = getTypeTitle($rec_search_results);
		$srch_result_response->rows[$i]['cell'] = array(
				'id' 	          		=> $rec_search_results->cls_id,
				'cls_code' 	      		=> $rec_search_results->cls_code,
				'cls_title' 	      	=> $rec_search_results->cls_title,
				'delivery_type_name' 	=> t($rec_search_results->delivery_type_name),
				'language'				=> $rec_search_results->language,
				'price' 	      		=> $currencySymbol.''.$rec_search_results->price,
				'Status' 	      		=> $trainingStatus,
				'AddProductOrder' 		=> $passData,
				'locationSessionDet'	=> $locationSessionDet,
		);
		$i++;
	}
	echo drupal_json_output($srch_result_response);
	} catch (Exception $ex) {
		watchdog_exception('loadOrderAddProduct', $ex);
		expertusErrorThrow($ex);
	}
    
}

function qryForloadProduct($lnrSrch, $searchKeyword, $userId,$isSort,$queryfrom=''){
	try {
	expDebug::dPrint('Query From = '.$queryfrom.' UserId= '.$userId,4);
	$adminId = getIdOfLoggedInUser();
	$maxPrice = getConfigValue('maxprice');
	$delType = $lnrSrch->setParamVal('dl_type');
	$lnrSrch->select = catalogClassSearchQuery($userId,'',$queryfrom,1);
	$lnrSrch->searchTitleClass($lnrSrch->setParamVal('title'));
	$lnrSrch->searchTypeClass($lnrSrch->setParamVal('dl_type'),'');
	$lnrSrch->select->condition('cls.status','lrn_cls_sts_atv','=');
	$lnrSrch->searchPriceClass('$0.1-$'.$maxPrice);
	$lnrSrch->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
	$lnrSrch->select->groupby('cls.id');
	//filterByUserAccessPrivileges($lnrSrch->select, 'cls', $userId,'','cre_sys_obt_cls');
	if($queryfrom == 'AddTraining' && $userId)
	{
		
		// Learner class results based on learner access
		$lnrSrchLrn = new LnrSearchCatalog();
		$lnrSrchLrn->select = catalogClassSearchQuery($userId,'','access_filter',1); // access_filter
		$lnrSrchLrn->searchTitleClass($lnrSrchLrn->setParamVal('title'));
		$lnrSrchLrn->searchTypeClass($lnrSrchLrn->setParamVal('dl_type'),'');
		$lnrSrchLrn->select->condition('cls.status','lrn_cls_sts_atv','=');
		$lnrSrchLrn->searchPriceClass('$0.1-$'.$maxPrice);
		$lnrSrchLrn->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
		$lnrSrchLrn->select->groupby('cls.id');
		//filterByUserAccessPrivileges($lnrSrchLrn->select, 'cls', $userId,'','cre_sys_obt_cls');
		
		// Learner class results based on previous enrollments
		$select_query = db_query('select GROUP_CONCAT( distinct(enroll.class_id)) from slt_enrollment as enroll where enroll.user_id = '.$userId.'
				AND ((enroll.comp_status IN (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\') OR enroll.comp_status IS NULL)
				AND enroll.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_ppv\'))');
		$enrolledList = $select_query->fetchField();
		expDebug::dPrintDBAPI('Find register course  from slt_enrollment ---> ' , $select_query);
		
		if(empty($enrolledList))
			$enrolledList = 0;
		
		$lnrSrchLrn->select->where('cls.id NOT IN('.$enrolledList.')');
		
		$lrnClassList =  $lnrSrchLrn->select->execute()->fetchCol('cls_id');
		
		//expDebug::dPrintDBAPI('$lnrSrchLrn->select learner query---> ' , $lnrSrchLrn->select);
		//expDebug::dPrint('$lnrSrchLrn->select learner query result ---> ' . print_r($lrnClassList, 1));
		
		// admin class list
		$lnrSrchAdmin = new LnrSearchCatalog();
		$lnrSrchAdmin->select = catalogClassSearchQuery($userId,'','access_filter',1); // access_filter
		$lnrSrchAdmin->searchTitleClass($lnrSrchAdmin->setParamVal('title'));
		$lnrSrchAdmin->searchTypeClass($lnrSrchAdmin->setParamVal('dl_type'),'');
		$lnrSrchAdmin->select->condition('cls.status','lrn_cls_sts_atv','=');
		$lnrSrchAdmin->searchPriceClass('$0.1-$'.$maxPrice);
		$lnrSrchAdmin->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
		$lnrSrchAdmin->select->groupby('cls.id');
		filterByUserAccessPrivileges($lnrSrchAdmin->select, 'cls', $adminId, 1,'cre_sys_obt_cls', '', 'cls');
		$admClassList =  $lnrSrchAdmin->select->execute()->fetchCol('cls_id');
		
		//expDebug::dPrintDBAPI('$lnrSrchAdmin->select admin query---> ' , $lnrSrchAdmin->select);
		//expDebug::dPrint('$lnrSrchAdmin->select admin query result ---> ' . print_r($admClassList, 1));
		
		// filter a class have access to admin and learner.
		$finalClassList = array_intersect($admClassList, $lrnClassList);
		
		expDebug::dPrint('Final class list' . print_r($finalClassList, 1), 4);
		
		if (empty($finalClassList))
			$finalClassList = 0;
		else
			$finalClassList = implode(',', $finalClassList);
		
		$lnrSrch->select->where('cls.id IN('.$finalClassList.')');
		
		$timeZone = date_default_timezone(false);
		expDebug::dPrint('TIME ZONE'.print_r($timeZone,true),4);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		expDebug::dPrint('Default TIME ZONE'.print_r($defaultTimezone,true),4);
		$sessionTZ = new DateTimeZone($defaultTimezone);
		expDebug::dPrint('USER TIME ZONE'.print_r($sessionTZ,true),4);
		$toDay = new DateTime(null,$sessionTZ) ;
		expDebug::dPrint('Today TIME'.print_r($toDay,true),4);
		$toDayDT = $toDay->format('Y-m-d H:i');
		$toDay = $toDay->format('Y-m-d');

		$lnrSrch->select->where( '(if(cls.delivery_type IN  (\'lrn_cls_dty_ilt\', \'lrn_cls_dty_vcl\'),
				(\''.$toDayDT.'\' < DATE_FORMAT(CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8\'%Y-%m-%d\'),\' \',ses.start_time),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone), \''.$defaultTimezone.'\'),\'%Y-%m-%d %H:%i\'))
				AND (if(DATE_FORMAT(CONVERT_TZ(ses.start_date ,(select pro2.attr2 from slt_profile_list_items pro2 where pro2.code= ses.timezone), \''.$defaultTimezone.'\'),\'%Y-%m-%d %H:%i\') = \''.$toDay.'\',
				\''.$toDayDT.'\' < CONVERT_TZ(CONCAT(DATE_FORMAT(ses.start_date, _utf8\'%Y-%m-%d\'),\' \',ses.end_time) ,(select pro3.attr2 from slt_profile_list_items pro3 where pro3.code= ses.timezone), \''.$defaultTimezone.'\'),
				1=1)),1=1) )');

		$lnrSrch->select->where('cls.registration_end_on >= now() or cls.registration_end_on is null');

		/* $lnrSrch->select->where( '(if(cls.delivery_type IN  (\'lrn_cls_dty_ilt\', \'lrn_cls_dty_vcl\'),
				(\''.$todaydate.'\' >= DATE_FORMAT(CONVERT_TZ(ses.start_date, \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),\'%Y-%m-%d %H:%i\')) AND (if(DATE_FORMAT(CONVERT_TZ(ses.start_date, \''.$defaultTimezone.'\' ,(select pro2.attr2 from slt_profile_list_items pro2 where pro2.code= ses.timezone)),\'%Y-%m-%d %H:%i\') = \''.$toDay.'\',
						\''.$toDayDT.'\' <= CONVERT_TZ(CONCAT(DATE_FORMAT(ses.end_date, _utf8\'%Y-%m-%d\'),\' \',ses.end_time), \''.$defaultTimezone.'\' ,(select pro3.attr2 from slt_profile_list_items pro3 where pro3.code= ses.timezone)),
						1=1)),1=1) )'); */
	}
	else{
		$lnrSrch->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
	}
	$classQuery = $lnrSrch->select;

	if($delType == 'cre_sys_obt_crt' || $delType == 'cre_sys_obt_cur' || $delType == 'cre_sys_obt_trn' || !$delType){
		$lnrSrch->select = catalogTPSearchQuery($userId,'',1);
		$lnrSrch->searchTitleTP($lnrSrch->setParamVal('title'));
		$lnrSrch->searchTypeTP('',$lnrSrch->setParamVal('dl_type'));
		$lnrSrch->select->condition('prm.status','lrn_lpn_sts_atv','=');
		$lnrSrch->searchPriceTP('$0.1-$'.$maxPrice);
		if($queryfrom == 'AddTraining' && $userId)
		{
			// learner access based  training plan list
			$lnrSrchLrnTp = new LnrSearchCatalog();
			$lnrSrchLrnTp->select = catalogTPSearchQuery($userId, 'access_filter',1);
			$lnrSrchLrnTp->searchTitleTP($lnrSrchLrnTp->setParamVal('title'));
			$lnrSrchLrnTp->searchTypeTP('',$lnrSrchLrnTp->setParamVal('dl_type'));
			$lnrSrchLrnTp->select->condition('prm.status','lrn_lpn_sts_atv','=');
			$lnrSrchLrnTp->searchPriceTP('$0.1-$'.$maxPrice);
			//filterByUserAccessPrivileges($lnrSrchLrnTp->select,'prm',$userId,'','','Program');
			
			$select_query = db_query('select GROUP_CONCAT( distinct(masenr.program_id)) from slt_master_enrollment as masenr where masenr.user_id = '.$userId.'
												AND masenr.overall_status IN (\'lrn_tpm_ovr_enr\',\'lrn_tpm_ovr_inp\',\'lrn_tpm_ovr_ppv\',\'lrn_tpm_ovr_ppm\',\'lrn_tpm_ovr_wtl\',\'lrn_tpm_ovr_exp\')');
			$result = $select_query->fetchField();

			/* Class count funtionality Start */
			$selectCls = db_select('slt_module_crs_mapping', 'crs');
			//$selectCls->leftjoin('slt_module_crs_mapping', 'crs','crs.program_id = tp.id');
			$selectCls->leftjoin('slt_course_class', 'cls','crs.course_id = cls.course_id');
			//77049: Commerce - Can't able to Create Order from Admin -->Order.
			//$selectCls->leftjoin('slt_course_class_session', 'ses','ses.class_id=cls.id AND ses.course_id=cls.course_id');
			$selectCls->leftjoin('slt_course_content_mapper', 'content','content.class_id=cls.id AND content.course_id=cls.course_id');
			$selectCls->leftjoin('slt_profile_list_items', 'master_view','master_view.code=cls.delivery_type');
			//$selectCls->addExpression('COUNT(DISTINCT(cls.id))', 'maxcnt');
			//$selectCls->addField('crs','program_id', 'tpid');
			$selectCls->addField('crs','course_id', 'crsid');
			// Completion status condition added by Vincent on 05 Dec, 2013 for #0026273
			$selectCls->leftjoin('slt_enrollment', 'enr','enr.course_id=cls.course_id AND user_id IN ('.$userId.') AND enr.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\')
						AND IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');

			$selectCls->where('IF(enr.course_id=cls.course_id,enr.class_id=cls.id,1=1)');
			//77049: Commerce - Can't able to Create Order from Admin -->Order.
			$selectCls->where('IF(enr.course_id=cls.course_id AND enr.class_id=cls.id,1=1,cls.status = \'lrn_cls_sts_atv\'
					AND IF(master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\', (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = cls.id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= cs.timezone)),NOW()) = 0), 1=1)
					AND (IF(master_view.attr1=\'WBT\' OR master_view.attr1=\'VOD\', content.id IS NOT NULL AND cls.status=\'lrn_cls_sts_atv\', 1=1))
			)');
			$selectCls->where('cls.registration_end_on >= now() or cls.registration_end_on is null');

			$selectCls->groupby('crs.course_id');

			$resultCls = $selectCls->execute()->fetchAll(PDO::FETCH_COLUMN);
			expDebug::dPrintDBAPI('Find Already Registerd Content not null and session > today Query---> ' , $selectCls);
			expDebug::dPrint('TP CLASS COUNT ===>'.print_r($resultCls,true),4);

			$selectCrsCnt = db_select('slt_module_crs_mapping','mapping');
			//$selectCrsCnt->addField('mapping','program_id', 'tpid');
			$selectCrsCnt->addField('mapping','course_id', 'crsid');
			//expDebug::dPrintDBAPI(' $selectCrsCnt = ' , $selectCrsCnt);
			$crsmappingcls = $selectCrsCnt->execute()->fetchAll(PDO::FETCH_COLUMN);
			expDebug::dPrintDBAPI('Find all course  from slt_module_crs_mapping ---> ' , $selectCrsCnt);
			expDebug::dPrint('TP CLASS COUNT mapping ===>'.print_r($crsmappingcls,true),4);
			$diff_tp_arr = array_diff($crsmappingcls,$resultCls);
			//expDebug::dPrint('TP test difference ===>'.print_r($diff_tp_arr,1) ,4);
			if(count($diff_tp_arr)>0)
			{
				$diff_crs_str = implode(',', $diff_tp_arr);
				$selectdiffCrs = db_select('slt_module_crs_mapping','mapping');
				$selectdiffCrs->addField('mapping','program_id', 'tpid');
				$selectdiffCrs->where('mapping.course_id IN('.$diff_crs_str.')');
				$diff_tp_res = $selectdiffCrs->execute()->fetchAll(PDO::FETCH_COLUMN);
				expDebug::dPrintDBAPI('Find tp id from diff course ids ---> ' , $selectdiffCrs);
				$diff_tp_res = array_filter($diff_tp_res, 'strlen');
				$diff_tp_str = implode(',', $diff_tp_res);
				if(!empty($result))
					$result .= ",".$diff_tp_str;
				else
					$result = $diff_tp_str;

			}
			expDebug::dPrint('DIFF CLASS COUNT mapping ===>'.print_r($diff_tp_arr,true),4);
			/* Class count funtionality END */

			//expDebug::dPrint('Arry VALUSE ARE==111'.$result,4);
			if(empty($result))
				$result =0;
			
			$lnrSrchLrnTp->select->where('prm.id NOT IN('.$result.')');
			$lrnTpList =  $lnrSrchLrnTp->select->execute()->fetchCol('cls_id');
			
			//expDebug::dPrintDBAPI('$$lnrSrchLrnTp->select learner query---> ' , $lnrSrchLrnTp->select);
			//expDebug::dPrint('$$lnrSrchLrnTp->select learner query result ---> ' . print_r($lrnTpList, 1));
			
			// admin access based training plan list
			$lnrSrchAdminTp = new LnrSearchCatalog();
			$lnrSrchAdminTp->select = catalogTPSearchQuery($userId, 'access_filter',1);
			$lnrSrchAdminTp->searchTitleTP($lnrSrchAdminTp->setParamVal('title'));
			$lnrSrchAdminTp->searchTypeTP('',$lnrSrchAdminTp->setParamVal('dl_type'));
			$lnrSrchAdminTp->select->condition('prm.status','lrn_lpn_sts_atv','=');
			$lnrSrchAdminTp->searchPriceTP('$0.1-$'.$maxPrice);
			filterByUserAccessPrivileges($lnrSrchAdminTp->select, 'prm', $adminId, 1,'cre_sys_obt_trp', 'tp', 'prm');
			$admTpList =  $lnrSrchAdminTp->select->execute()->fetchCol('cls_id');
			
			// filter a tp have access to admin and learner.
			$finalTpList = array_intersect($admTpList, $lrnTpList);
			
			
			//expDebug::dPrintDBAPI('$lnrSrchAdminTp->select admin query---> ' , $lnrSrchAdminTp->select);
			//expDebug::dPrint('$lnrSrchAdminTp->select admin query result ---> ' . print_r($admTpList, 1));
			
			expDebug::dPrint('$finalTpList : ' . print_r($finalTpList, 1), 4);
			
			if (empty($finalTpList))
				$finalTpList = 0;
			else 
				$finalTpList = implode(',', $finalTpList);
			
			  $lnrSrch->select->where('prm.id IN('.$finalTpList.')');
		}
		$lnrSrch->select->groupby('prm.id');
		//filterByUserAccessPrivileges($lnrSrch->select,'prm', $userId,'','','Program');
		$tpQuery = $lnrSrch->select;
	}
	/* if($isSort){
		$lnrSrch->searchSortBy('AZ');
	} */

	if($delType){
		if($delType == 'cre_sys_obt_crt' || $delType == 'cre_sys_obt_cur' || $delType == 'cre_sys_obt_trn'){
			return $tpQuery;
		}
		else{
			return $classQuery;
		}
	}
	$resultQry = $classQuery->union($tpQuery,'UNION ALL');
	return $resultQry;
	} catch (Exception $ex) {
		watchdog_exception('qryForloadProduct', $ex);
		expertusErrorThrow($ex);
	}
}

function getAdminAddProductAutoComplete() {
try {
	$searchText =  $_GET['z'];
	$userId = $_GET['user_id'];
	$maxPrice = getConfigValue('maxprice');
	module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');

	// Implementation of Solr
	$srcSource = getConfigValue('solr_search');
	if($srcSource == 0){
		$lnrSrch = new LnrSearchCatalog();
		$lnrSrch->select = catalogClassSearchQuery($userId,'','',1);
		$lnrSrch->searchTitleClass($searchText);
	//$lnrSrch->searchTypeClass($lnrSrch->setParamVal('dl_type'),'');
		$lnrSrch->select->condition('cls.status','lrn_cls_sts_atv','=');
		$lnrSrch->searchPriceClass('$0.1-$'.$maxPrice);
		$lnrSrch->select->where('cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null');
		$lnrSrch->select->groupby('cls.id');
	//filterByUserAccessPrivileges($lnrSrch->select, 'cls', $userId,'','cre_sys_obt_cls');
		$classQuery = $lnrSrch->select;

		$lnrSrch->select = catalogTPSearchQuery($userId,'',1);
		$lnrSrch->searchTitleTP($searchText);
	//$lnrSrch->searchTypeTP($lnrSrch->setParamVal('dl_type'),'');
		$lnrSrch->select->condition('prm.status','lrn_lpn_sts_atv','=');
		$lnrSrch->searchPriceTP('$0.1-$'.$maxPrice);
		$lnrSrch->select->groupby('prm.id');
	//filterByUserAccessPrivileges($lnrSrch->select,'prm',$userId,'','','Program');
		$tpQuery = $lnrSrch->select;
	
		$lnrSrch->searchSortBy($lnrSrch->setParamVal('sortby'));
		$resultQry1 = $classQuery->union($tpQuery,'UNION ALL');
		$lnrSrch->select = $resultQry1;

		expDebug::dPrintDBAPI('Product qry' ,   $lnrSrch->select);
		$qry_search_results = $lnrSrch->select->execute()->fetchAll();
		expDebug::dPrint('auto complete list solr off'.print_r($qry_search_results,true),5);
	}

	else{
	    require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
	    try{
	       // expDebug::dPrint("Solr Search Testing",1);
	        $solrObj = new CatalogSolrSearch();
	        
	        //expDebug::dPrint("Call solr my oder result",1);
	        $search_results = $solrObj->getSolrSearchResultMyorder();
	        
	        expDebug::dPrint('Result set from solr main result ac'.print_r($search_results,true),5);
	        $qry_search_results = $search_results['records'];
	        expDebug::dPrint('Result set from solr ac '.print_r($qry_search_results,true),5);
	        $count = $search_results['recCount'];
	        expDebug::dPrint('Result set count ac '.print_r($count,true),5);
	        
	        expDebug::dPrint('auto complete list solr on'.print_r($qry_search_results,true),5);
	        
	        }
	    catch(Exception $e){
	        expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
	    }
	}

	foreach ($qry_search_results as $results) {
		print $results->cls_title . "\n";
	}
	} catch (Exception $ex) {
		watchdog_exception('getAdminAddProductAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
    
}
/*function returns the user informations*/
function getUserInfoAutoComplete($field) {
	try {
	$searchText =  $_GET['z'];
	$limit = isset($_GET['limit']) ? $_GET['limit'] : 100;
	$select = db_select('slt_person','per');
	$select->leftJoin('users','usr','usr.name = per.user_name');
	$select->leftJoin('slt_organization','org','org.id = per.org_id');
	$select->addField('usr','uid','id');
	$select->addField('per',$field);
	if($field == "first_name" || $field == "last_name"){
	$select->addField('per','user_name');
	}else{
		$select->addField('per','first_name');
	}
	$select->addField('org','name','org_name');
	$select->condition('per.'.$field,'%'.db_like($searchText).'%','LIKE');
	$select->condition('per.status','cre_usr_sts_atv','=');
	$select->condition('per.user_name', array('admin','guest'),'NOT IN');
	$select->range(0, $limit); // change by ayyappan for #46102
	filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per'); // Ticket No : 0068235
	$userCount = $select->countQuery()->execute()->fetchField();
	expDebug::dPrintDBAPI('AutocompleteQuery', $select);
	$results = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
// 	$userCount = count($results);
	expDebug::dPrint('getUserInfoAutoComplete'.print_r($userCount,true),4);

	foreach ($results as $res) {
		if($field == "first_name" || $field == "last_name"){
		print $res['id'].EXP_AC_SEPARATOR.$res[$field].EXP_AC_SEPARATOR.$res['user_name'].EXP_AC_SEPARATOR.$res['org_name'].EXP_AC_SEPARATOR.$userCount."\n";
		}else{
			print $res['id'].EXP_AC_SEPARATOR.$res[$field].EXP_AC_SEPARATOR.$res['first_name'].EXP_AC_SEPARATOR.$res['org_name'].EXP_AC_SEPARATOR.$userCount."\n";
		}
	}
	} catch (Exception $ex) {
		watchdog_exception('getUserInfoAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Funtion to return the list of Learning Objects criteria like Cert,curricula and learning plan etc.
 *
 * @param
 *
 * @return
 *   Results of Learning Object Types
 */
function getOrderLearningObjectType(){
	try {
	$select = db_select('slt_profile_list_items','splt');
	$select->addField('splt','code');
	$select->addField('splt','name');
	$select->condition(db_and()->
			condition(db_or()->
					condition('splt.code', 'cre_sys_obt_crt' , '=')->
					condition('splt.code', 'cre_sys_obt_cur' , '=')->
					condition('splt.code', 'cre_sys_obt_trn' , '=')));

	$select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));

	$select->condition(db_and()->condition('splt.is_active','Y','='));

	$result2 = $select->execute()->fetchAllKeyed();
	$result1 = getDeliveryTypeList();
	return array_merge($result1,$result2);
	} catch (Exception $ex) {
		watchdog_exception('getOrderLearningObjectType', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Funtion to return the Cart Status.
 *
 * @param
 *	   Class ID/TP ID
 * @return
 */
function isAdminCartAdded($nodeId = array()) {
	try{
		$orderId = $_SESSION['cart_admin_order'];
		$result = 0;
		if($orderId){
			$select = db_select('uc_order_products', 'uccart');
			$select->addExpression('count(1)','cnt');
			$select->condition('uccart.order_id',$orderId,'=');
			$select->condition('uccart.nid',$nodeId,is_array($nodeId) ? 'IN':'=');
			$result = $select->execute()->fetchField();

		}
		return ($result > 0) ? 1 : 0;
	}catch (Exception $ex) {
		watchdog_exception('isAdminCartAdded', $ex);
		expertusErrorThrow($ex);
	}
}

function admin_cart_update(&$form){
	try{
		$orderId =	$_SESSION['cart_admin_order'];
		$order   = uc_order_load($orderId);
		$commands = array();
		$panes = _uc_checkout_pane_list();
	foreach ($panes as $id => $pane) {
		if (($id == 'clone_cart') && $pane['enabled']) {
			if($id == 'clone_cart'){
				$form_state['order_from'] = 'admin_clone_cart';
			}
			$return = $pane['callback']('view', $order, $form, $form_state);
			// Add the pane if any display data is returned from the callback.
			if (is_array($return) && (!empty($return['description']) || !empty($return['contents']))) {
				// Create the fieldset for the pane.
				$form['panes'][$id] = array(
						'#type' => 'fieldset',
						'#title' => $pane['title'],
						'#description' => !empty($return['description']) ? $return['description'] : NULL,
						'#collapsible' => $pane['collapsible'],
						//'#collapsed' => variable_get('uc_use_next_buttons', FALSE) ? $collapsed : FALSE,
						'#id' => $id . '-pane',
						'#theme' => isset($return['theme']) ? $return['theme'] : NULL,
				);
				// Add the contents of the fieldset if any were returned.
				if (!empty($return['contents'])) {
					$form['panes'][$id] = array_merge($form['panes'][$id], $return['contents']);
					$_SESSION['cartform']= $form['panes'][$id];
					expDebug::dPrint('$form[panes][$id] = ' . print_r($return['contents'], true), 5);
				}
				// Log that this pane was actually displayed.
				$displayed[$id] = TRUE;
			}
		}
	}
	expDebug::dPrint('addProductOrderCallBack clone cart : '.print_r($form['panes']['clone_cart'],true) , 5);
	$output = drupal_render($form['panes']['clone_cart']);
	$legOutput = substr($output,strpos($output,'<legend>'));
	$finalOutput = substr($legOutput,0,strrpos($legOutput,'</fieldset>'));
	echo $finalOutput;
	}catch (Exception $ex){
		watchdog_exception('admin_cart_update', $ex);
		expertusErrorThrow($ex);
	}
}

?>