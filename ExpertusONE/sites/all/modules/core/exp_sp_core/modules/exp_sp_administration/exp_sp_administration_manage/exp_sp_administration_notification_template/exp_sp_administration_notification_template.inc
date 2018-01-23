<?php
/**
 * Notification Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

/*
 * getNotificationTabContentType() - For Notification admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getNotificationTabContentType(){
	try {
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * notificationNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function notificationNarrowSearchFiltersHTML($argumentype) {
  try {
  $allStatus = getProfileItemNamesKeyed('cre_ntn_sts_');
  $allType = getOptionalEntityType();
  foreach($allType as $optValue){
    $optType[$optValue->entity_code] = $optValue->entity_type;
  }
  expDebug::dPrint(' $allType ' . print_r($allType, true) , 4);
  $allStatus['cre_ntn_sts_atv'] = t('LBL921');
  $allStatus['cre_ntn_sts_itv'] = t('LBL922');
  $optType['cre_sys_obt_cls']   = t('Class');
  $optType['cre_sys_obt_cnt']		=	t('Content');
  $optType['cre_sys_obt_evt']		=	t('Mobile App');
  $optType['cre_sys_obt_trp']		=	t('Training Plan');
  $optType['cre_sys_obt_usr']		=	t('User');
  $optType['cre_sys_obt_rpt']   = t('Report'); //Report
  $optType['cre_sys_obt_rcl']   = t('Request Class'); //Request Class

  //$allLang = getNotificationLanguage('cre_sys_lng_');
  //expDebug::dPrint(' $allLang = ' . print_r($allLang, true) , 3);


  $renderfiltersets=array();
  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'notificationstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus)
                                                          );


  /*if(count($allLang) > 1 ){
   $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'notificationlang',
                                                                 'title' => t('LBL038'),
                                                                 'checkboxes_list' => $allLang)
                                                          );
  } */
  if($argumentype == 'notification_template'){

  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'notificationType',
                                                                 'title' => t('LBL036'), //Type
                                                           		 'divid' => 'type',
                                                                 'checkboxes_list' => $optType)
                                                          );

  }

  $filterSetforJS=array(array('type'=>'checkbox','code'=>'notificationstatus'),
                        array('type'=>'checkbox','code'=>'notificationType')
                         /*array('type'=>'checkbox','code'=>'notificationlang')*/
                       );

  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS),
                     ));
  }catch (Exception $ex) {
  	watchdog_exception('notificationNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * notificationSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function notificationSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'notification-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'notification-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'notification-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'notification-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  }catch (Exception $ex) {
  	watchdog_exception('notificationSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * notificationSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function notificationSearchResultsActionbarHTML($argumentype) {
	try {
  global $user;
  $loggedUserName = $user->name;

  if($argumentype == 'notification_template') {
    if($loggedUserName == 'admin'){
      $basepath = base_path() . 'administration/manage/notification_template/nojs/addedit';
      $buttonTitle = t('LBL917');
    }else{
      $basepath = '#';
      $buttonTitle = '';
    }
  $actionbarLinksListforJS=array('export_path'=>'administration/manage/notification_template/export/',
  					             'print_path'=>'administration/manage/notification_template/print/'
                              );
  return theme('narrow_search_actionbar', array( 'add_button_title' => $buttonTitle,
                                                 'show_add_button' => true,
												 												 'add_page_path' => $basepath,
												 												 'show_print_button'=>true,
												 												 'show_export_button'=>true,
                                                 'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                                                 'ctools_style'=>'ctools-modal-ctools-admin-notification-addedit-style',
                                                 'grey_out_add_button' => $argumentype,
  																							 'page_code' => 'cre_ntn',
                                           )
         );
  } else {
    global $user;
    $drupaluid    = getDrupalIdOfUser($user->uid);
    $account      = user_load($user->uid,TRUE);
    $adminPerm    = user_access('Certificate Admin Perm',$account);
    if((($adminPerm)) || $loggedUserName == 'admin'){
      $basepath = base_path() . 'administration/manage/certificate/nojs/addedit';
    }else{
      $basepath = '#';
    }
  	$actionbarLinksListforJS=array('export_path'=>'administration/manage/certificate/export/',
  					             'print_path'=>'administration/manage/certificate/print/'
                              );
    return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL939'),
                                                 'show_add_button' => true,
												 												 'add_page_path' => $basepath,
												 									       'show_print_button'=>true,
												 												 'show_export_button'=>true,
                                                 'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                                                 'ctools_style'=>'ctools-modal-ctools-admin-notification-addedit-style',
                                                 'grey_out_add_button' => $argumentype,
    																						 'page_code' => 'cre_cer', // Removed commented line and enable create privilege check for  #0050371
                                           )
         );
  }
  }catch (Exception $ex) {
  	watchdog_exception('notificationSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getNotificationTabContentMainHTML - Returns the html for tab notification main
 */
function getNotificationTabContentMainHTML($argumentype) {
	try {
  $tabContentTypeArr =  getNotificationTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
  	if($argumentype == 'notification_template'){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/manage/notification_template/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/notification_template/textfilter-autocomplete'
                                          );
  	} else {
  	  $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/manage/certificate/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/certificate/textfilter-autocomplete'
                                          );
  	}

    $tabContentTypeStr=theme('narrow_search_tab_content',array(
                                                               'narrowsearch_tab_content_list'=> drupal_json_encode($narrowsearchTabContentListforJS),
                                                              )
                            );
  }else{ /* This  is for non-narrow search */
    $tabContentTypeStr="";
  }
  return $tabContentTypeStr;
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getNotificationTabContent		  - Returns the html for narrow search block on notification search page
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Export Settings.
 */
function getNotificationTabContent($argumentype) {
	try {
  $tabContentTypeArr =  getNotificationTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];

  $notificationResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => notificationNarrowSearchFiltersHTML($argumentype),
    'rendered_narrow_search_sortbar' => notificationSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => notificationSearchResultsActionbarHTML($argumentype),
    'rendered_tab_content_main'=>getNotificationTabContentMainHTML($argumentype),
    'rendered_main_div' => getAdminModules('administration','manage'),
  );
  echo drupal_json_encode($notificationResponse);
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}





/*
 * sendNotificationListToJqGrid() - Prepares JSON response to JqGrid's request for Notification
 */
function sendNotificationListToJqGrid($argumentype) {
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

/*   $numNotification = fetchAllNotification($argumentype, 'COUNT');

  expDebug::dPrint(' $$numNotification = ' . print_r($numNotification, true) , 3);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numNotification); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);


  $notificationList = fetchAllNotification($argumentype, 'LISTS',  $start, $limit);
  expDebug::dPrint(' $$notificationList = ' . print_r($notificationList, true) , 3);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numNotification;
                                             // have to do this.
  $response->initial_sort_type_html_id = 'notification-sort-new';

  $i=0;
  foreach ($notificationList as $notification) {
    expDebug::dPrint(' $notification = '. print_r($notification, true) , 3);

    $notification->row =  $i;
    $notification->object_type = $argumentype;
    $response->rows[$i]['id']   = $notification->id;
    $response->rows[$i]['cell'] = array( 'details' => getNotificationHTML($notification),
                                         'action' => getNotificationActionsHTML($notification)
                                         );

    $i++;
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

  echo drupal_json_encode($response);
  }catch (Exception $ex) {
  	watchdog_exception('sendNotificationListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getNotificationTextFilterAutoComplete() - Returns the matched notification title
 */
function getNotificationTextFilterAutoComplete($argType) {
	try {
  if($argType == 'notification_template'){
    $typeOption = 'Email';
  }else{
  	$typeOption = 'Certificate';
  }
  $searchText =  $_GET['z'];
  expDebug::dPrint('sfgdfsg'.$searchText);
  // Select the table
  $select = db_select('slt_notification_info', 'notification');

  // Add fields to fetch
  $select->addExpression('DISTINCT(notification.notification_title)', 'title');

  selectAddNarrowSearchByTextMultipleORCondition($select, "notification", array("notification_title","notification_description"), 'z');
  $select->condition('notification.status',array('cre_ntn_sts_atv','cre_ntn_sts_itv'),'IN');
  $select->condition('notification.send_options',$typeOption,'=');
  if($argType=='notification_template'){
  	filterByUserAccessPrivileges($select, 'notification','',1,'cre_ntn','','notification');
  
  } else if($argType == 'certificate'){
  	filterByUserAccessPrivileges($select, 'notification','',1,'cre_cer','','notification');
  
  }
  // Order in dictionary sort order
  $select->orderBy('notification_title');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // print_r($select->__toString());
  $matchingNotificationNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingNotificationNames = ' . print_r($matchingNotificationNames, true) , 3);

  foreach ($matchingNotificationNames as $notificationName) {
    print rawurlencode($notificationName->title) . "\n";
  }
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}


function fetchAllNotification($argType = NULL , $op = NULL ,$start = NULL , $limit = NULL){
	try {
		global $user;
  if($argType=='notification_template'){
  	$notType ='cre_ntn';
    $sendType='Email';
    $pdfTitleText=t('LBL204');
  }else{
    $sendType='Certificate';
    $pdfTitleText=t('LBL205');
  }
// Select table
  $select = db_select('slt_notification_info', 'noti_info');
 /*  $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=noti_info.id AND group_map.entity_type = \''.$notType.'\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$notType.'\''); */
  $select->leftJoin('slt_profile_list_items', 'pli_lang', 'noti_info.lang_code = pli_lang.code AND pli_lang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'pli_status', 'noti_info.status = pli_status.code AND pli_status.lang_code = \'cre_sys_lng_eng\' ');
  $select->leftJoin('slt_profile_list_items', 'pli_type', 'noti_info.entity_type = pli_type.code AND pli_type.lang_code = \'cre_sys_lng_eng\' ');
  $select->condition('noti_info.send_options', $sendType,'=');

  if($op=='COUNT'){
     // Get count of active notification
     $select->addExpression('COUNT(noti_info.id)', 'COUNT');
  }
  else{
	  // Select fields/expression values to be fetched from the database.
  //	$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
  	//$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
		$select->addField('noti_info', 'id', 'id');
		$select->addField('noti_info', 'notification_code', 'code');
		$select->addField('noti_info', 'notification_description', 'description');
		$select->addField('noti_info', 'notification_title', 'title');
		$select->addField('noti_info', 'notification_type', 'freq_type');
		$select->addField('noti_info', 'entity_type', 'entity_type');
		$select->addExpression("(select GROUP_CONCAT(pli_rpt.name SEPARATOR ', ' ) from slt_profile_list_items pli_rpt where find_in_set(pli_rpt.code,noti_info.notification_sendto))", 'sendmailto');
		$select->addField('pli_lang', 'name', 'lang');
		$select->addField('pli_type', 'name', 'type_name');
		$select->addField('pli_status', 'name', 'status');
		$select->addField('pli_status', 'code', 'status_code');
  }
  if($argType=='notification_template'){
  	filterByUserAccessPrivileges($select, 'noti_info','',1,'cre_ntn','','noti_info');

  }else if($argType == 'certificate'){
  	filterByUserAccessPrivileges($select, 'noti_info','',1,'cre_cer','','noti_info');

  }
  selectAddNarrowSearchINCondition($select, 'noti_info', 'status', 'notificationstatus');
  selectAddNarrowSearchINCondition($select, 'noti_info', 'entity_type', 'notificationType');
  selectAddNarrowSearchByTextMultipleORCondition($select, "noti_info", array("notification_title", "notification_code","notification_description",), "textfilter");

  if($op != "COUNT"){
    // Order the records
    notificationSearchSelectStmtAddOrderByClause($select, 'noti_info');
    if($user->uid != '1')
    	$select->groupBy('noti_info.id');
  }

  expDebug::dPrintDBAPI('$select', $select);

  if($op == "COUNT"){
  	$result = $select->execute()->fetchField();
	  expDebug::dPrint('COUNT $result = ' . $result ,3);
	return $result;
  }
  elseif($op == "CSV"){
	if($argType == 'notification_template'){
	$colHeadersMap = array(t('LBL083') => 'title',
                           t('LBL102') => 'status',
                           t('LBL940') => 'freq_type',
                           t('LBL918') => 'sendmailto',
                           t('LBL229') => 'description');
	}else{
	  $colHeadersMap = array(t('LBL083') => 'title',
                           t('LBL102') => 'status',
                           t('LBL229') => 'description');
	}


    $filterArray[] = array($pdfTitleText.': '.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').': '.checkboxFiltersetSelectionsToPrintableStr('notificationstatus'));

    if($argType == 'notification_template'){
	$filterArray[] = array(t('LBL036').': '.checkboxFiltersetSelectionsToPrintableStr('notificationType'));
    }

    $filterArray[] = array(ucfirst(t('LBL011')).': '.getNarrowSearchSortTitle());
  	SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray);

  } else if($op == "PDF") {
    if($argType == 'notification_template'){
    // % width of the columns must total to 100%
      $colHeadersMap = array(t('LBL083') => array('title', 25),
                      // t('LBL038') => array('lang', 25),
    				   t('LBL102') => array('status', 10),
    				   t('LBL940') => array('freq_type', 15),
    				   t('LBL918') => array('sendmailto', 20),
    				   t('LBL229') => array('description', 25));


      $appliedFilters = array($pdfTitleText => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('notificationstatus'),
                             t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('notificationType'),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
                             );
      SendNarrowSearchResultsAsPDFFile($select, t('LBL984'), $appliedFilters, $colHeadersMap);
    }else{
      $colHeadersMap = array(t('LBL083') => array('title', 25),
    				   t('LBL102') => array('status', 17),
    				   t('LBL229') => array('description', 25));

      $appliedFilters = array($pdfTitleText => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('notificationstatus'),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
    );
    SendNarrowSearchResultsAsPDFFile($select, t('LBL985'), $appliedFilters, $colHeadersMap);
    }
  } else{
  		add_row_id_condition($select, 'noti_info');
	     // Limit to $limit records
	     $select->range($start, $limit);
	     expDebug::dPrintDBAPI('$select with range set', $select);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	    return $result;
  }
  }catch (Exception $ex) {
  	watchdog_exception('fetchAllNotification', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getNotificationHTML() - Renders a notification detail as HTML to be displayed in jqGrid.
 */
function getNotificationHTML($notification) {
  try {
  $detailsList = array();
  $actionList = array();
  $notificationActivateStr="";
  global $user;
  $loggedUserName = $user->name;
  if($notification->object_type == 'notification_template'){
    $deleteTitle= t('LBL204');
    if (!empty($notification->sendmailto)) {
      $sendmailto_arr = explode(",", $notification->sendmailto);
      if(count($sendmailto_arr) > 0)
      {
      	  $arr_val = array();
      	  foreach($sendmailto_arr as $k=>$v){
      	  	$arr_val[] = t(trim($v));
	      }
	      $notification->sendmailto = implode(", ",$arr_val);
      }
      $detailsList[] = array('detail' => t('LBL918').': '.$notification->sendmailto, 'tooltip' => t('LBL918').': '.$notification->sendmailto);
    }
    $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/manage/notification_template/nojs/addedit',
					  'action_button_params' => $notification->id,
                      'action_button_param_text' => 'notification',
  					  'ctools_style' => 'ctools-modal-ctools-admin-notification-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
	if($loggedUserName == 'admin'){
      $actionList[] = array('action_link_text' => t('LBL286'),
        'action_page_path' => '',
        'action_button_params' => $notification->id,
        'action_button_param_text' => 'notification_template',
        'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
        'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                 ),
        'tooltip' => t('LBL286')
      );
	}
  } else {
    $deleteTitle= t('LBL205');
  	$actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/manage/certificate/nojs/addedit',
					  'action_button_params' => $notification->id,
                      'action_button_param_text' => 'notification',
  					  'ctools_style' => 'ctools-modal-ctools-admin-notification-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );

	$actionList[] = array('action_link_text' => t('LBL286'),
      'action_page_path' => '',
      'action_button_params' => $notification->id,
      'action_button_param_text' => 'certificate',
      'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
      'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                 ),
      'tooltip' => t('LBL286')
    );
  }
  if($notification->object_type == 'notification_template' || $notification->object_type == 'certificate'){ // #47244 - Data visibility added in Certificate
  	$newActionList[] = array('action_link_text' => t('LBL642'),
  			'action_page_path' => '',
  			'action_button_params' => $notification->id,
  			'action_button_params_text' => 'access',
  			'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  			'js_object_info' => array (
  					'name' => 'narrowsearch', // provide the java script object name
  					'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  			),
  			'tooltip' => t('LBL642'),
  			'entityId' => $notification->id,
  			'entityType' => ($notification->object_type == 'notification_template') ? 'cre_ntn' : 'cre_cer',
  			'icon_style_class'=>'access-tab-icon',
  			'postype' => 'topleft',
  			'poslwid' => 40,
  	);
  }
  $viewlist[] = array('action_link_text' => 'Notification',
  		'action_page_path' => 'administration/view-notification/nojs/addedit/'.$notification->id.'',
  		'action_button_params' => $notification->id,
  		'action_button_surveyassement_id' => $notification->id,
  		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),

  );

  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($notification->title),
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.$deleteTitle,
                       'action_list'=> $actionList,
                       'description'=> trim($notification->description),
                 		   'new_action_list'=> $newActionList,
                 		   'sumedit' => $notification->sumedit,
                 		   'sumdelete' => $notification->sumdelete,
                 			 'view_list' => $viewlist,
                 			 'object_type'=>'notification',
                      )
                );
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getNotificationActionsHTML() - Renders a notification's actions as HTML to be displayed in jqGrid.
 */
function getNotificationActionsHTML($notification) {
		try {
     return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $notification)
              );
     }catch (Exception $ex) {
     	watchdog_exception('getNotificationActionsHTML', $ex);
     	expertusErrorThrow($ex);
     }
}

/**
 * notificationSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function notificationSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".notification_title";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias . ".notification_title";
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
  }catch (Exception $ex) {
  	watchdog_exception('notificationSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fucntion to retrive the details of notification
 */
function getAllNotificationDetails($notificationId){
	try{
  $select = db_select('slt_notification_info', 'notification');
  $select->leftjoin('slt_profile_list_items','spli','spli.code = notification.entity_type');
  $select->addField('notification','id','id');
  $select->addField('notification','notification_title','title');
  $select->addField('notification','notification_description','short_description');
  $select->addField('notification','notification_code');
  $select->addField('notification','status','status');
  $select->addField('notification','notification_sendto','sendmailto');
  $select->addField('notification','notification_type','notify_type');
  $select->addField('notification','frequency_data_value','freq_data');
  $select->addField('notification','notification_cc','notify_cc');
  $select->addField('notification','notification_admin','notify_admin');
  $select->addField('notification','notify_before');
  $select->addField('spli','name','entity_type');
  $select->condition('notification.id', $notificationId);
  expDebug::dPrintDBAPI('getAllNotificationDetails',$select);
  //$select->range(0,1);
  $result = $select->execute();
  return $result->fetchAssoc();
	}catch(Exception $ex){
		watchdog_Exception('getAllNotificationDetails',$ex);
		ExpertusErrorThrow($ex);
	}
}


/**
 * addNewNotificationDetails() - New Notification Entry
 */
function addNewNotificationDetails($form, &$form_state, $createUserId, $callFromAPI =0){
  $txn = db_transaction();
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 5);
  expDebug::dPrint('$form_state values = storage values ' . print_r($form_state['values']['sendmailto'], true), 5);
  try {
    $insertStmt  	= db_insert('slt_notification_info');
    $nfields       	= notificationFormatInputString($form, $form_state,$createUserId);
    /*-- #37404: Issue in Notification module unable to create new notification --*/
    $nfields['frequency_data_value'] = ($nfields['frequency_data_value'] == '') ? 0 : $nfields['frequency_data_value'];
    $insertStmt->fields($nfields);
    expDebug::dPrintDBAPI('insertStmt', $insertStmt);
    $notificationId 		= $insertStmt->execute();
    //getting notification id and appended for notification code
    updateGroupMappingDetails($notificationId,$nfields['type'],getIdOfLoggedInUser(),'emptyadd',0);
    if($form_state['values']['page_type'] == 'notification_template') {
      $noitificationCode = 'notify_'.$notificationId;
	  $notification_sendto = implode(',',$form_state['values']['sendmailto']);
    }else{
      $noitificationCode = 'certify_'.$notificationId;
    }
    $updateStmt = db_update('slt_notification_info');
    $fields = array(
    		'notification_code' => $noitificationCode
    );
    $updateStmt->condition('id',$notificationId);
    $updateStmt->fields($fields);
    $updateStmt->execute();
    $insertfields = $form_state['values'];
	$insertfields[title] = $form_state['values']['name'];
    $insertfields[description] = $form_state['values']['short_description']['value'];
	$insertfields[entity_type]= $form_state['values']['entity_type'];
	$insertfields[notify_type]= $form_state['values']['notify_type'] ['value'];
	$insertfields[sendmailto]= $notification_sendto;
	$insertfields[notify_cc]= $form_state['values']['cc_mail'];
	
   if ($form_state['values']['page_type'] == 'notification_template'){
    $emptyId = $form_state['values']['empty_id'];
    _updateAccessEntity($notificationId, 'cre_cer', $emptyId);
    make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$notificationId, 'Inserted','cre_ntn_sts_itv','exp_sp_administration_notification','addNewNotificationDetails',
                                          array('lang_code','name', 'description','notify_type','entity_type','sendmailto','notify_cc'));
   }else{
   	make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$notificationId, 'Inserted','cre_ntn_sts_itv','exp_sp_administration_notification','addNewNotificationDetails',
                                          array('lang_code','name','description'));
   }
   
    // Save the new notification's id in the hidden id form field.
    $form_state['values']['id']     = $notificationId;
    return array((object)array('id'=>$notificationId));
  } catch (Exception $ex) {
    $txn->rollback();
    expDebug::dPrint("Error in addNewNotificationDetails() exp_sp_administrion_notification.inc ".print_r($ex,true));
    expDebug::dPrint("Error in addNewNotificationDetails() exp_sp_administrion_notification.inc ".$ex->__toString());
    watchdog_exception('addNewNotificationDetails', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Notification Id.";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);
}



function notificationFormatInputString($form, &$form_state,$createUserId) {
	try {

   $nullVal           = NULL;
   $trigger_element = $form_state['storage']['submit_triggering_element'];
   $page_type = $form_state['values']['page_type'];
   $ccMail = $form_state['values']['cc_mail'];
   expDebug::dPrint('the checking of format input string'.$trigger_element,4);
   expDebug::dPrint('values of form state for checking'.print_r($form_state['values']['sendmailto'],true),5);
   if($page_type == 'notification_template'){
     $notification_sendto = implode(',',$form_state['values']['sendmailto']);
   }else{
     $notification_sendto = $nullVal;
   }

   if(($page_type == 'notification_template')&& $form_state['values']['entity_type']){
   	$entityTypeCode = getEntityTypeCode($form_state['values']['entity_type']);
   }else{
     $entityTypeCode = 'cre_cer';
   }
   if($page_type == 'notification_template'){
     $notificationSendOption = 'Email';
   }else{
   	 $notificationSendOption = 'certificate';
   }
   $notification_status = '';
   if($trigger_element == 'saveandpublish'){
     $notification_status = 'cre_ntn_sts_atv';
   }
   else if($trigger_element == 'save'){
     $notification_status = 'cre_ntn_sts_itv';
   }
   else if($form_state['values']['status']){
	 $notification_status =$form_state['values']['status'];
   }
   if($form_state['values']['notify_type'] == 'Triggered'){
    $form_state['values']['freq_data'] = (!empty($form_state['values']['freq_data'])) ? $form_state['values']['freq_data'] : '0';
   }
   expDebug::dPrint('$form_state[][freq_data] : '.print_r($form_state['values']['freq_data'], true), 4);
   $fields = array(

                'notification_title'              => $form_state['values']['name'],
                'notification_description'        => $form_state['values']['short_description']['value'],
                'status'             => $notification_status,
                'lang_code'          => 'cre_sys_lng_eng',
                'notification_type'  => ($page_type == 'notification_template') ? $form_state['values']['notify_type'] : $nullVal,
                'frequency_data_value'=> ($page_type == 'notification_template' && $form_state['values']['freq_data'] != '') ? $form_state['values']['freq_data'] : $nullVal,
   				'notify_before'				=> ($page_type == 'notification_template') ? $form_state['values']['notify_before'] : $nullVal,
                'notification_sendto'=> $notification_sendto,
                'entity_type'        => $entityTypeCode,
                'notification_cc'    => ($page_type == 'notification_template' && ($ccMail != t('MSG698'))) ? $form_state['values']['cc_mail'] : $nullVal,
                'send_options'       => $notificationSendOption,
                'locale_code'        => '',
                'custom0'            => $nullVal,
                'custom1'            => $nullVal,
                'custom2'            => $nullVal,
                'custom3'            => $nullVal,
                'custom4'            => $nullVal,

              );
   if($form_state['values']['notification_id']){
     $fields['updated_by'] = $createUserId;
     $fields['updated_on'] = now();
   }
   else{
     $fields['created_by'] = $createUserId;
     $fields['created_on'] = now();
     $fields['updated_by'] = $createUserId;
     $fields['updated_on'] = now();
     $fields['type'] = ($page_type == 'notification_template') ? 'cre_ntn' : 'cre_cer';
   }

   if($trigger_element == 'unpublish'){
     $notification_status = 'cre_ntn_sts_itv';
     $fields = array( 'status'  => $notification_status );
   }
   expDebug::dPrint('$fields = ' . print_r($fields,true), 4);
   return $fields;
   }catch (Exception $ex) {
   	watchdog_exception('notificationFormatInputString', $ex);
   	expertusErrorThrow($ex);
   }
}
function getEntityTypeCode($entitytype){
	try {
	$select = db_select('slt_profile_list_items','spli');
	$select->addField('spli','code','entity_type');
	$select->condition('spli.name',$entitytype,'=');
	$select->condition('spli.code', 'cre_sys_obt_%', 'LIKE');
	expDebug::dPrintDBAPI('getEntityTypeCode',$select);
	$result = $select->execute()->fetchField();
	return $result;
	}catch (Exception $ex) {
		watchdog_exception('getEntityTypeCode', $ex);
		expertusErrorThrow($ex);
	}
}
function updateNotificationDetails($form, &$form_state, $updateUserId, $callFromAPI =0) {
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
  $txn = db_transaction();

  try {

  	$defaultQryStr = 'SELECT notification_description as description, notification_title as title, status from slt_notification_info where id = :cid';
  	$defaultQryStrArgs = array(':cid' => $form_state['values']['notification_id']);
  	$defaultQry = db_query($defaultQryStr, $defaultQryStrArgs);
  	expDebug::dPrintDBAPI('$defaultQry', $defaultQry, $defaultQryStrArgs);
  	$default = $defaultQry->fetchAll();
  	expDebug::dPrint('$default = ' . print_r($default, true) );

  	$updateStmt = db_update('slt_notification_info');
    $fields     = notificationFormatInputString($form, $form_state,$updateUserId);
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['notification_id']);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);

    // Execute the update statement.
    $numUpdated = $updateStmt->execute();
    $updatefields = $form_state['values'];
	$updatefields[title] = $form_state['values']['name'];
    $updatefields[description] = $form_state['values']['short_description']['value'];
	//$updatefields[title]  =  $form_state['values']['name'];
	
	$notification_title = trim($form_state['values']['title']);

    make_audit_trail_entries($updatefields, $updateUserId , $default ,$form_state['values']['notification_id'], 'Updated','cre_ntn','exp_sp_administration_notification','updateNotificationDetails',
                                          array( 'title','description'));

     

    if($callFromAPI){
      return array((object)array('id'=>$form_state['values']['template_id']));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateNotificationDetails', $ex);
    if($callFromAPI){
     // return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Notification Title.";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);

}
/*
 * function to publish and unpublish the notification
 */
function publishOrUnpublishNotification($notificationType,$notificationId){
  $currUserId = getSltpersonUserId();
  $status  = getNotificationStatus($notificationId);
  $txn = db_transaction();
  try {
    $updateStmt = db_update('slt_notification_info');
    $updatedstatus  = ($status=='cre_ntn_sts_atv')?"cre_ntn_sts_itv":"cre_ntn_sts_atv";

    $updateStmt->condition('id', $notificationId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
   expDebug::dPrintDBAPI('query for publish and unpublish',$updateStmt);
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishNotification', $ex);
    ExpertusErrorThrow($ex);
  }
  unset($txn);
  add_audit_trail_entry($currUserId, $notificationId, $notificationType, 'exp_sp_administration_notification_template', 'publishOrUnpublishNotification', 'Updated notification Status',
		                                     null, $status, $updatedstatus);

	$returnStatus  = ($updatedstatus=='cre_ntn_sts_atv')?t('LBL920'):t('LBL919');
	expDebug::dPrint('return status for publish and unpublish'.$returnStatus,4);
	expDebug::dPrint('updated status for publish and unpublish'.$updatedstatus,4);
	print $updatedstatus.'|'.$returnStatus;
}


function getNotificationStatus($notificationId) {
	$txn = db_transaction();
	try {
		$select = db_select('slt_notification_info','notify_info');
		$select->addField('notify_info','status','status');
		$select->condition('notify_info.id',$notificationId,'=');
		expDebug::dPrintDBAPI('query for status',$select);
		$result = $select->execute()->fetchField();
		return $result;
	}catch(Exception $ex){
		$txn->rollback();
		watchdog_exception('getNotificationStatus',$ex);
		ExpertusErrorThrow($ex);
	}unset($txn);
}


function notificationTemplateToDelete($notifyId) {
	try{
	$select = db_select('slt_notification_frame','frame');
	$select->addExpression('count(1)','notifycount');
	$select->condition('frame.notification_id',$notifyId,'=');
	$result = $select->execute()->fetchField();
	return $result;
	}catch(Exception $ex){
	  watchdog_Exception('notificationTemplateToDelete',$ex);
	  ExpertusErrorThrow($ex);
	}
}

function getAvailableTemplateNotification($notifyId) {
	try{
	$select = db_select('slt_notification_frame','frame');
	$select->leftJoin('slt_profile_list_items', 'pli_lang', 'frame.lang_code = pli_lang.code');
	$select->addField('frame','lang_code');
	$select->condition('frame.notification_id',$notifyId,'=');
	$select->condition('pli_lang.is_active','Y');
	expDebug::dPrintDBAPI('querygetAvailableTemplateNotification',$select);
	$result = $select->execute()->fetchAll();
	return $result;
	}catch(Exception $ex){
	  watchdog_Exception('getAvailableTemplateNotification',$ex);
	  ExpertusErrorThrow($ex);
	}
}

function getOptionalEntityType() {
	try{
	$select = db_select('slt_profile_list_items','spli');
	$select->addField('spli','name','entity_type');
	$select->addField('spli','code','entity_code');
	$select->condition('spli.code', array("cre_sys_obt_cls","cre_sys_obt_usr","cre_sys_obt_cnt","cre_sys_obt_trp","cre_sys_obt_evt","cre_sys_obt_shr", "cre_sys_obt_rpt", "cre_sys_obt_rcl"), 'IN');
	expDebug::dPrintDBAPI('query for getOptionalEntityType',$select);
	$result = $select->execute()->fetchAll();
	expDebug::dPrint('result for option getOptionalEntityType'.print_r($result,true),4);
	return $result;
	}catch(Exception $ex){
	  watchdog_Exception('getOptionalEntityType',$ex);
	  ExpertusErrorThrow($ex);
	}
}
function getNotificationTemplateDetails($templateId){
	try{
	  $select = db_select('slt_notification_frame','frame');
	  $select->addfield('frame','notification_subject','subject');
	  $select->addField('frame','notification_template','template');
	  $select->addField('frame','sms_message','sms_message');
	  $select->addField('frame','lang_code','lang_code');
	  $select->condition('frame.id',$templateId,'=');
	  expDebug::dPrintDBAPI('getNotificationTemplateDetails',$select);
	  $result = $select->execute()->fetchAssoc();
	  return $result;
	}catch(Exception $ex){
		watchdog_Exception('getNotificationTemplateDetails',$ex);
		ExpertusErrorThrow($ex);
	}
}

function fetchAllTemplate($op, $lang, $start, $limit,$id){
// Select table
try {
  $select = db_select('slt_notification_frame', 'notify_frame');
  $select->leftJoin('slt_profile_list_items', 'pli_lang', 'notify_frame.lang_code = pli_lang.code');
  $select->leftJoin('slt_person', 'person', 'person.id = notify_frame.created_by');
  $select->condition('notify_frame.lang_code',$lang,'IN');
  $select->condition('notify_frame.notification_id',$id,'=');

  if($op=='COUNT'){
     // Get count of active notification
     $select->addExpression('COUNT(notify_frame.id)', 'COUNT');
  }
  else{
	  // Select fields/expression values to be fetched from the database.
		$select->addField('notify_frame', 'id', 'id');
		$select->addField('notify_frame', 'created_on');
		$select->addField('pli_lang', 'name', 'lang');
		$select->addField('person', 'full_name', 'created_by');
  }

  if($op == "COUNT"){
  	$result = $select->execute()->fetchField();
	  expDebug::dPrint('COUNT $result = ' . $result ,3);
	return $result;
  }
  else{
	     // Limit to $limit records
	     $select->range($start, $limit);
	     expDebug::dPrintDBAPI('$select with range set', $select);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	    return $result;
  }
  }catch (Exception $ex) {
  	watchdog_exception('fetchAllTemplate', $ex);
  	expertusErrorThrow($ex);
  }
}


function getNotificationTitleDirect($notificationId){
  try{
	$selectCls = db_select('slt_notification_info','info');
	$selectCls->addField('info','notification_title', 'notify_title');
	$selectCls->condition('info.id', $notificationId,'=');
	$result =  $selectCls->execute()->fetchAssoc();
	return $result['notify_title'];
  }catch (Exception $ex) {
    watchdog_exception('getNotificationTitleDirect', $ex);
    expertusErrorThrow($ex);
  }
}

function addNewTemplate($form, &$form_state, $createUserId,$callFromAPI =0){
	$txn = db_transaction();
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 5);
  expDebug::dPrint('$form_state values = storage values ' . print_r($form_state['storage'], true), 5);
  try {
    $insertStmt  	= db_insert('slt_notification_frame');
    $nfields       	= templateFormatInputString($form, $form_state,$createUserId);
    $insertStmt->fields($nfields);
    expDebug::dPrintDBAPI('insertStmt', $insertStmt);
    $templateId 		= $insertStmt->execute();
    //getting notification id and appended for notification code

    $insertfields = $form_state['values'];
    $insertfields[description] = $form_state['values']['message']['value'];
	$insertfields[sms_message] = $form_state['values']['sms_text'];
    make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$templateId, 'Inserted','cre_ntn_sts_itv','exp_sp_administration_notification_template','addNewTemplate',
                                          array('lang_code','subject','description','sms_message'));
    // Save the new notification's id in the hidden id form field.
    $form_state['values']['template_id']     = $templateId;
    return array((object)array('id'=>$templateId));
  } catch (Exception $ex) {
    $txn->rollback();
    expDebug::dPrint("Error in addNewTemplate() exp_sp_administrion_notification_template.inc ".print_r($ex,true));
    expDebug::dPrint("Error in addNewTemplate() exp_sp_administrion_notification_template.inc ".$ex->__toString());
    watchdog_exception('addNewTemplate', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Template Id.";
    	return $errobj;
    }
    throw $ex;
}
}

function templateFormatInputString($form, &$form_state,$createUserId) {
	try {
   $nullVal           = NULL;
   expDebug::dPrint('the checking of format input string'.$trigger_element,4);
   expDebug::dPrint('values of form state for checking'.print_r($form_state['values'],true),5);
   expDebug::dPrint('$form_state of to display mail sent to'.$notification_sendto,5);
  if($form_state['argument_type'] == 'notification_template'){
     $sendOption = 'Email';
   }else{
   	 $sendOption = 'certificate';
   }

   $fields = array(
                'notification_id'              => $form_state['values']['notification_id'],
                'send_options'        => $sendOption,
                'lang_code'          => $form_state['values']['lang_code'],
                'notification_subject'=> $form_state['values']['subject'],
                'notification_template'        => $form_state['values']['message']['value'],
                'header_template'    => '',
                'footer_template'    => '',
   		        'sms_message'        => ($form_state['values']['sms_text'] == t('LBL1235')) ? '' : $form_state['values']['sms_text'],
                'custom0'            => $nullVal,
                'custom1'            => $nullVal,
                'custom2'            => $nullVal,
                'custom3'            => $nullVal,
                'custom4'            => $nullVal,
              );
   if($form_state['values']['template_id']){
     $fields['updated_by'] = $createUserId;
     $fields['updated_on'] = now();
   }
   else{
     $fields['created_by'] = $createUserId;
     $fields['created_on'] = now();
     $fields['updated_by'] = $createUserId;
     $fields['updated_on'] = now();
   }

   expDebug::dPrint('$fields Velu = ' . print_r($fields,true), 4);
   return $fields;
   }catch (Exception $ex) {
   	watchdog_exception('templateFormatInputString', $ex);
   	expertusErrorThrow($ex);
   }
}

function updateTemplateDetails($form, &$form_state, $updateUserId, $callFromAPI =0) {
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
  $txn = db_transaction();

  try {

  	$defaultQryStr = 'SELECT lang_code, notification_subject, notification_template as description, send_options from slt_notification_frame where id = :cid';
  	$defaultQryStrArgs = array(':cid' => $form_state['values']['template_id']);
  	$defaultQry = db_query($defaultQryStr, $defaultQryStrArgs);
  	expDebug::dPrintDBAPI('$defaultQry', $defaultQry, $defaultQryStrArgs);
  	$default = $defaultQry->fetchAll();
  	expDebug::dPrint('$default = ' . print_r($default, true) );

  	$updateStmt = db_update('slt_notification_frame');
    $fields     = templateFormatInputString($form, $form_state,$updateUserId);
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['template_id']);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
	
    // Execute the update statement.
    $numUpdated = $updateStmt->execute();
    $trigger_element = $form_state['storage']['submit_triggering_element'];
    $updatefields = $form_state['values'];
    $updatefields[description] = $form_state['values']['message']['value'];
	$updatefields[lang_code]   = $form_state['values']['lang_code'];

    make_audit_trail_entries($updatefields, $updateUserId , $default ,$form_state['values']['template_id'], 'Updated','cre_ntn','exp_sp_administration_notification','updateTemplateDetails',
                                          array('description','lang_code'));


    if($callFromAPI){
      return array((object)array('id'=>$form_state['values']['notification_id']));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateTemplateDetails', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Notification Template.";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);

}

function fetchKeywordsList($entityType){
	try{
		$select = db_select('slt_notification_keyword','notifykey');
		$select->addField('notifykey','id','kid');
		$select->addField('notifykey','keyword','keywords');
		$select->addField('notifykey','description','keyworddescription');
		//$select->condition(db_and()
  	                     //->condition('notifykey.id', $entityId, '=')
  	                     //->where('FIND_IN_SET(' . $entityType . ', notifykey.allowed_entities) > 0')
  	    $select->where("FIND_IN_SET('".$entityType ."',notifykey.allowed_entities) > 0");
		expDebug::dPrintDBAPI('fetchKeywordsList',$select);
		return $select;
	}catch(Exception $ex){
		watchdog_Exception('fetchKeywordsList',$ex);
		ExpertusErrorThrow($ex);
	}
}

function recipientsForNotification(){
  try{
  	$select = db_select('slt_profile_list_items','profile');
  	$select->addField('profile','name');
  	$select->addfield('profile','code');
  	$select->condition('profile.code','cre_ntn_rpt_%','LIKE');
  	expDebug::dPrintDBAPI('recipientsForNotification',$select);
  	$result = $select->execute()->fetchAll();
  	return $result;
  }catch(Exception $ex){
  	watchdog_Exception('recipientsForNotification',$ex);
  	ExpertusErrowThrow($ex);
  }
}

function entityTypeForKeywords($notifyId){
 try{
  	$select = db_select('slt_notification_info','info');
  	$select->addField('info','entity_type','type');
  	$select->condition('info.id',$notifyId,'=');
  	expDebug::dPrintDBAPI('entityTypeForKeywords',$select);
  	$result = $select->execute()->fetchField();
  	return $result;
  }catch(Exception $ex){
  	watchdog_Exception('entityTypeForKeywords',$ex);
  	ExpertusErrowThrow($ex);
  }
}
function notificationSendToOptions($notifyId){
 try{
  	$select = db_select('slt_notification_info','info');
  	$select->addField('info','send_options','sendtype');
  	$select->condition('info.id',$notifyId,'=');
  	expDebug::dPrintDBAPI('notificationSendToOptions',$select);
  	$result = $select->execute()->fetchField();
  	return $result;
  }catch(Exception $ex){
  	watchdog_Exception('notificationSendToOptions',$ex);
  	ExpertusErrowThrow($ex);
  }
}

function getNotificationFrameCount($notificationId){
	try {
  $select = db_select('slt_notification_frame', 'frame');
  $select->join('slt_notification_info','info','frame.notification_id = info.id');
  $select->addExpression('COUNT(1)','total_template');
  $select->condition('info.id', $notificationId, '=');
  //$select->condition('info.status', 'cre_ntn_sts_atv', '!=');
  expDebug::dPrintDBAPI('$select', $select);
  $total_class = $select->execute()->fetchField();

  if ($total_class== 0){
    return 0;
  } else {
    return 1;
  }
  }catch (Exception $ex) {
  	watchdog_exception('getNotificationFrameCount', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAvailableLanguagesForTemplate($notificationId,$defaultLang = '',$countTmpl = ''){
	try {
  $select = db_select('slt_profile_list_items','spli');
  $select->where("spli.code  NOT IN (select frame.lang_code from slt_notification_frame frame where frame.notification_id = '".$notificationId."' and frame.lang_code != '".$defaultLang."')");
  $select->condition('spli.code','cre_sys_lng_%','LIKE');
  $select->condition('spli.is_active','Y','=');
  $select->orderBy('spli.name','ASC');
  expDebug::dPrintDBAPI('$select ' , $select);
  if($countTmpl == 'LIST'){
    $select->addExpression('COUNT(1)','total_template');
    $result = $select->execute()->fetchField();
  }else{
    $select->addField('spli','code');
    $select->addField('spli','name');
    $result = $select->execute()->fetchAllKeyed();
  }
  expDebug::dPrint('$result = ' . print_r($result, true));

  // Return results
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('getAvailableLanguagesForTemplate', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * Get list of Languages in slt_profile_list_items
 */
function getNotificationsAllLanguage($notificationId,$defaultLang = '',$countTmpl = ''){
 try{
  $select = db_select('slt_profile_list_items', 'spfa');
  $select->join('slt_profile_list_items','spfb','spfa.parent_id = %alias.id');
  $select->condition('spfb.code', 'cre_sys_lng');
  $select->Condition('spfa.is_active','Y');
  $select->orderBy('spfa.name','ASC');
 if($countTmpl == 'COUNT'){
    $select->addExpression('COUNT(1)','total_template');
    $languages = $select->execute()->fetchField();
  }else if($countTmpl == 'LIST'){
    $select->addField('spfa', 'code');
    $select->addField('spfa', 'name');
    $select->addField('spfa', 'attr1','drupallangcode');
    $select->where("spfa.code  NOT IN (select frame.lang_code from slt_notification_frame frame where frame.notification_id = '".$notificationId."' and frame.lang_code != '".$defaultLang."')");
    $languages = $select->execute()->fetchAll();

  }else{
    $select->addField('spfa', 'code');
    $select->addField('spfa', 'name');
    $select->addField('spfa', 'attr1','drupallangcode');
    $languages = $select->execute()->fetchAll();
  }
  expDebug::dPrintDBAPI('values to check getNotificationsAllLanguage',$select);
  expDebug::dPrint('result of getNotificationsAllLanguage'.print_r($languages,true));
  return $languages;
  }catch (Exception $ex) {
    watchdog_exception('getNotificationsAllLanguage', $ex);
    expertusErrorThrow($ex);
  }

}
/*
 * addTextFieldToAddEditForm() -  Use this function to add a text field to the addedit form
 */
function addTextFieldToAddEditFormNotify(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 60, $description = false) {
  try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text fields.
  $attributes['class'][] = 'addedit-edit-textfield';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }
  if ($id == 'subject') {
  	$attributes['onclick'][] = 'inbutBoxOnClick(this);';
  }
  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }
  if($defaultValue==t('MSG447')){
    $attributes['class'][] = 'narrow-search-filterset-daterange-empty';
  }
  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

  // Create the form element
  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars,
    '#description' => $description
  );
  }catch (Exception $ex) {
  	watchdog_exception('addTextFieldToAddEditFormNotify', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * addEditNotificationRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditNotificationRepaintForm($form, &$form_state) {
	try {
  expDebug::dPrint(' addEditRepaintForm() called.');
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint('  $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
  expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['values'], true) , 5);

  // If a new entity was added, build additional forms
  if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {
    // Create the additional now editable forms if any present
    $formInfoList = $form_state['storage']['form_info_list'];
    $argsList = $form_state['build_info']['args'];
    array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList

    $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
  }

  $commands = array();
  $notifyType  = $form_state['values']['notify_type'];
  $freqData  = $form_state['values']['freq_data'];
  $notifyCode  = $form_state['values']['notification_code'];

  // Build HTML for the custom fields block if triggering element is Add custom or Delete custom button
  // We paint only the custom fields block in this case, and not the full form.
  if (!empty($form_state['triggering_element']['#name']) &&
               ($form_state['triggering_element']['#name'] == 'Add custom' || substr($form_state['triggering_element']['#name'], 0, 13) == 'Delete custom')) {
    $customFieldsWrapperId = $form_state['triggering_element']['#ajax']['custom_fields_wrapper_id'];
    expDebug::dPrint(' $customFieldsWrapperId = ' . print_r($customFieldsWrapperId, true) , 5);

    $customFieldsFormElements = array();
    $startCapture = false;
    foreach ($form as $elemKey => $elemAttrib) {
      expDebug::dPrint(' $elemKey = ' . print_r($elemKey, true) , 4);
      expDebug::dPrint(' $elemAttrib = ' . print_r($elemAttrib, true), 4);

      if (!$startCapture && $elemKey != 'customfields_wrapper_start') {
        continue;
      }

      $startCapture = true;
      $customFieldsFormElements[] = $form[$elemKey];
      //$customFieldsFormElementsKeysForDebugging[] = $elemKey;

      if ($elemKey == 'customfields_wrapper_end') {
        break; // break out of form; we have got all the fields that we want to repaint
      }
    }
    expDebug::dPrint(' $customFieldsFormElementsKeysForDebugging = ' . print_r($customFieldsFormElementsKeysForDebugging, true) , 4);

    $renderHTML = drupal_render($customFieldsFormElements);
    expDebug::dPrint(' $renderHTML = ' . print_r($renderHTML, true)  , 4);
    $commands[] = ajax_command_replace('#' . $customFieldsWrapperId, $renderHTML);
  }
  else { // repaint the full form
    $commands[] = ajax_command_replace(NULL, drupal_render($form));
  }

  // Render additional forms, if moving from add to edit
  if (!empty($renderedAddlForms)) {
    $commands[] = ajax_command_after(NULL, $renderedAddlForms);
  }

  // Show the status messages, if any.
  // Ensure the status messages are auto removed after a brief while
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  	// Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  }

  // Resize the modal backdrop, adjust the model top (and left) to optimal
  $commands[] = exp_ctools_modal_adjust();

  // If a new entity was added or an existing entity was edited, inform ctools to refresh narrow search results no modal close.
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
  	$refresh_grid = ((isset($form_state['storage']['edited_entity']) && $form_state['storage']['edited_entity']) ? false : true);
    $commands[] = ajax_command_refresh_narrow_search_results('onclose', $refresh_grid);
  }

  if($form_state['values']['attr_action'] == 'certificate' && $form_state['triggering_element']['#name']=='save'){
     $defaulttCertificate=$form_state['default_certificate_id'];
     $commands[] = ajax_command_invoke(NULL, "validateCertificate",array($defaulttCertificate));
  }
  if (isset($form_state['cutom_fields_wrapper_id'])){
    $customWrapperId = $form_state['cutom_fields_wrapper_id'];
    $customErrorFlag = $form_state['custom_fields_error'];
    $commands[] = ajax_command_invoke(NULL, "validateCustomFields", array($customWrapperId, $customErrorFlag));
  }

  if ($form_state['triggering_element']['#ajax']['add_custom_gray_field'] == true){
    expDebug::dPrint(' $form_state[triggering_element][#ajax][add_custom_gray_field] = ' . $form_state['triggering_element']['#ajax']['add_custom_gray_field'] , 5);
    $commands[] = ajax_command_invoke(NULL, "addCustomGrayConversion", array($form_state['custom_fields']['count']));
  }

  /* For create Location in Class Page*/
  if($form_state['triggering_element']['#ajax']['from_tooltip_location'] == 'Yes'){
    $commands[] = ajax_command_invoke(NULL, "classCreateLocation", array($form_state['storage']['locationId'],$form_state['storage']['locationName'],$form_state['triggering_element']['#ajax']['from_tooltip_location_classId']));
  }
  /*---*/

  if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && (!$form_state['esign_validation_status'])){
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $drupalUserId  = $form_state['triggering_element']['#ajax']['drupalUserId'];
    $drupalUserId  = $form_state['values'][$drupalUserId];
    $esignObj  = '{"popupDiv":"modalContent","esignFor":"AddAdmin","drupalUserId":"'.$drupalUserId.'","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup'] == true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false; // this is ignored, as form state is not saved after the ajax callback is executed
    }
  }
  $commands[] = ajax_command_invoke(NULL, "validateNotificationFrequencyType",array($notifyType,$freqData,$notifyCode));
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  return array('#type' => 'ajax', '#commands' => $commands);
  }catch (Exception $ex) {
  	watchdog_exception('addEditNotificationRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addDropDownToAddEditForm - Use this function to add a dropdown (select) to the addedit form
 */
function addNotificationDropDownToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array(),$notifyComp = '') {
	try {
  //Using the below veriable we can find current loaded theme name
  global $theme_key;

  //$attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  if($attributes['class']){
    $attributes['class'] = array($attributes['class']);
  }
  if(is_array($attributes['class'])){
    $attributes['class'][] = 'addedit-edit-' . $id;
  }
  else{
    $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  }
  $attributes['onchange'][] = 'textfieldTitleChange(this);';

  // Add class to apply common style to all drop-downs
  $attributes['class'][] = 'addedit-edit-dropdown';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-dropdown';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($mandatory) {
    $nullOption[] = t('LBL674');
  } else {
    $nullOption[] = t('LBL674');
  }
  //$finalOptions = array_merge($nullOption, $options);

  $finalOptions = $nullOption + $options;
  foreach($finalOptions as $key => $value){
  	$finalOptions[$key] = t($value);
  }

  $prefix = '';
  $suffix = '';
  $freqPrefix = '';
  $freqSuffix = '';
  if($theme_key == 'expertusoneV2') {
    if($defaultOption == 'Scheduled' && $notifyComp == 1){
	  $newuiPrefix = '<div class="expertus-dropdown-bg notify-type-dropdown"><div class="expertus-dropdown-icon ">';
	  $freqPrefix = '<div id="notify-freq">';
	  $freqSuffix = '</div>';
    }else{
	  $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
    }
	  $newuiSuffix = '</div></div>';
  } else {
	  $newuiPrefix = '';
	  $newuiSuffix = '';
  }

  if (empty($form['addedit_tmp_storage']['two_column'])) {
  	$prefix = addEditGetPrefix('FIELD', $title, $mandatory);
  	$suffix = addEditGetSuffix('FIELD');
  }
  else {
  	if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
  	} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
  	} else {
  		throw new Exception('exp_sp_administration.inc : addDropDownToAddEditForm() : Attempt to add a third field to a two column row.');
  	}
  }
  $form[$id] = array(
    '#type' => 'select',
    '#prefix' => $freqPrefix.$prefix.$newuiPrefix,
    '#suffix' => $newuiSuffix . $suffix.$freqSuffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#options'=> $finalOptions,
    '#default_value' => $defaultOption,
    '#disabled' => $readOnly,
    '#attributes' => $attributes
  );

  $replacedId = str_replace('_', '-', $id);
	$form['load_select_drop_down_' . $id] = array (
		'#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="textfieldTitleChangeField(\'edit-'.$replacedId.'\');" />',
	);
	}catch (Exception $ex) {
		watchdog_exception('addNotificationDropDownToAddEditForm', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_viewnotification_addedit_form($js = NULL, $entityId){
	try{
		global $theme_key;
		$css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		if($theme_key == 'expertusoneV2') {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning_v2.css', $css_theme_optional);
		}else{
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.css', $css_theme_optional);
		}
		//CUSTOM SCROLLBAR "JSscrollbar"
		drupal_add_js(drupal_get_path('module', 'exp_sp_lnrreports') .'/jscrollbar/jquery.jscrollpane.js', $js_module_optional);
		expDebug::dPrint('sabbeeee-->'.$entityId);
		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 730,
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
					array('id' => 'exp_sp_administration_view_notificationdetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewnotification_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_notificationdetails_addedit_form($form, &$form_state,$renderMode, $notId){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$$notId---->'.$notId);

		$wrapperId = 'viewnotification-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$notificationdetails = getNotificationDetailsForView($notId);

		expDebug::dPrint('$notificationdetails-->'.print_r($notificationdetails,true),5);
		if (!empty($notificationdetails['sendmailto'])) {
			$sendmailto_arr = explode(",", $notificationdetails['sendmailto']);
			if(count($sendmailto_arr) > 1)
			{
				$arr_val = array();
				foreach($sendmailto_arr as $k=>$v){
					$arr_val[] = t(trim($v));
				}
				$notificationdetails['sendmailto'] = implode(", ",$arr_val);
			}
		}
		$form['narrow_notifyview'] = array(
				'#markup'       => theme('narrow_notifyview', array('record' => $notificationdetails)),
		);

		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_notificationdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getNotificationDetailsForView($notId){
	try{
		$select = db_select('slt_notification_info', 'notification');
		$select->leftjoin('slt_profile_list_items','spli','FIND_IN_SET(spli.code,notification.notification_sendto)>0');
		$select->addField('notification','id','id');
		$select->addField('notification','notification_title','title');
		$select->addField('notification','notification_description','short_description');
		$select->addExpression('group_concat(spli.name)','sendmailto');
		$select->addField('notification','notification_type','notify_type');
		$select->addField('notification','frequency_data_value','freq_data');
		$select->addField('notification','notification_cc','notify_cc');
		$select->addField('notification','notification_admin','notify_admin');
		$select->addField('notification','send_options','send_options');
		$select->addField('spli','name','entity_type');
		$select->condition('notification.id', $notId);
		expDebug::dPrintDBAPI('getAllNotificationDetails',$select);
		//$select->range(0,1);
		$result = $select->execute();
		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getNotificationDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}
function templatefornotify($notId){
	try{
		$languageList = getNotificationsAllLanguage();
		$drupalLanguageList = locale_language_list('native');
		foreach ($languageList as $id => $row) {
			if (array_key_exists($row->drupallangcode, $drupalLanguageList)) {
				$ptLanguage[$row->code] = $row->code;
			}
		}
		$select = db_select('slt_notification_frame', 'notify_frame');
		$select->leftJoin('slt_profile_list_items', 'pli_lang', 'notify_frame.lang_code = pli_lang.code');
		$select->leftJoin('slt_person', 'person', 'person.id = notify_frame.created_by');
		$select->condition('notify_frame.lang_code',$ptLanguage,'IN');
		$select->condition('notify_frame.notification_id',$notId,'=');

		// Select fields/expression values to be fetched from the database.
		$select->addField('notify_frame', 'id', 'id');
		$select->addField('notify_frame', 'created_on');
		$select->addField('pli_lang', 'name', 'lang');
		$select->addField('person', 'full_name', 'created_by');

		// Limit to $limit records
		//        $select->range($start, $limit);
		expDebug::dPrintDBAPI('$select with range setdddd', $select);
		// Execure the query and fetch all the records
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getNotificationDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}
?>