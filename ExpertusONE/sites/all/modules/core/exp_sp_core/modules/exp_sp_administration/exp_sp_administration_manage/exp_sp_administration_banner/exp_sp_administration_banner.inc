<?php
/**
 * Banner Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

 /*
 * getBannerTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */

function getBannerTabContentType(){
	try {
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getBannerTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getBannerLanguage() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix and
 * 								Fetch all related announcement language.The result is ordered by name.
 */
function getBannerLanguage($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  // Select the table
  try {
  $select = db_select('slt_announcement_master','announcement');

  //Distinct the Resultset
  $select->distinct();

  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');

  // Left join the slt_profile_list_items to get language names
  $select->leftJoin('slt_profile_list_items', 'spli', 'announcement.lang_code = spli.code');

  // Set the conditions
  $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');

  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  $select->condition(db_and()->condition('announcement.status', 'cbn_anm_sts_del', '!='));

  if (!$alsoFetchInactiveItems) {
  	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }

  // Order results by name
  $select->orderBy('name');

  expDebug::dPrintDBAPI(' $select object = ' , $select);

  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getBannerLanguage', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * bannertNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function bannerNarrowSearchFiltersHTML() {
	try {
  $checkedStatus = array();
  //$checkedStatus["cbn_anm_sts_atv"] = TRUE;
  $statusOrder=array('cbn_anm_sts_atv','cbn_anm_sts_itv','cbn_anm_sts_dft');
  $allStatus = bannerProfileListItems($statusOrder);

  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);


  $allLang = getBannerLanguage('cre_sys_lng_');
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

  expDebug::dPrint(' $allLang = ' . print_r($allLang, true) , 3);


  $renderfiltersets=array();
  //$detailsList[] = array('detail' => t('LBL241').$surveyDetailsQuestions, 'tooltip' => t('LBL240'));
  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'announcementstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus,
                                                                 'checked_list' => $checkedStatus )
                                                          );


  if(count($allLang) > 1 ){
   $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'announcementlang',
                                                                 'title' => t('LBL038'),
                                                           		 'divid' => 'language',
                                                                 'checkboxes_list' => $getlangToTheme,
                                                           		 'checked_list' => $checkedlangStatus)
                                                          );
  }

  $renderfiltersets[]=theme('narrow_search_filterset_daterange', array('code' => 'announcementdaterange',
                                                                       'title' => t('LBL622'), //Display Date
                                                                       'dateRangeShowOption' => 'all', // dateRangeShowOption options are 'all' or 'past' or 'future'
                                                                       'title_description' => t('LBL043'), //Range. Describes the title. Shown in ().
                                                                       'from_default_text' => t('LBL220'), //From: mm-dd-yyyy
                                                                       'to_default_text' => t('LBL221'), //To: mm-dd-yyyy
                                                                      )
                                                          );


  $filterSetforJS=array(array('type'=>'checkbox','code'=>'announcementstatus'),
                         array('type'=>'checkbox','code'=>'announcementlang'),
                         array("type"=> "daterange", "code"=>"announcementdaterange",
  							"from_tooltip" => "", "from_default_text" => t("LBL220"),
  							"to_tooltip" => "", "to_default_text" => t("LBL221"))
                       );

  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS),
                     ));
  } catch (Exception $ex) {
  	watchdog_exception('bannerNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * bannerSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function bannerSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'bannerdetails-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'bannerdetails-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'bannerdetails-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'bannerdetails-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('bannerSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * bannerSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function bannerSearchResultsActionbarHTML() {
  try {
  $actionbarLinksListforJS=array('export_path'=>'administration/manage/banner/export/',
  					             'print_path'=>'administration/manage/banner/print/'
                              );
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL617'),
                                                 'show_add_button' => true,
																									'add_page_path' => base_path() . 'administration/manage/banner/nojs/addedit',
												 													'show_print_button'=>true,
												 													'show_export_button'=>true,
                                                  'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                                                  'ctools_style'=>'ctools-modal-ctools-admin-announcement-addedit-style',
  		                                            'page_code' => 'cbn_anm_typ_ban',
                                                )
         );
  } catch (Exception $ex) {
  	watchdog_exception('bannerSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBannerTabContentMainHTML - Returns the html for tab content main
 */
function getBannerTabContentMainHTML() {
	try {
  $tabContentTypeArr =  getBannerTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/manage/banner/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/banner/textfilter-autocomplete'
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
  	watchdog_exception('getBannerTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBannerTabContent		  - Returns the html for narrow search block on announcement search page
 * 									  - Prepares JSON response for Survey Details.
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Expert Settings.
 */
function getBannerTabContent() {
	try {
  $tabContentTypeArr =  getBannerTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];

  $bannerResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => bannerNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar' => bannerSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => bannerSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getBannerTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','manage'),
  );
  echo drupal_json_encode($bannerResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getBannerTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getChildSurveyDetailsQuestionCount() - Function to get the count of the questions(active only) under Survey Details
 * SELECT * FROM `slt_survey_groups_questions`  AS sgq LEFT JOIN  `slt_survey_questions` AS sq ON sgq.question_id=sq.id WHERE sq.status='sry_qtn_sts_atv' AND sgq.survey_id=4;
 */
function getChildSurveyDetailsQuestionCount($surveydetailsId) {
  // Select slt_survey_groups_questions table
  try {
  $select = db_select('slt_survey_groups_questions', 'surveygroup');

  // Get count of active Questions
  $select->addExpression('COUNT(surveygroup.id)', 'count');

  $select->leftJoin('slt_survey_questions', 'surveyquestion', 'surveygroup.question_id=surveyquestion.id');

  // Set the conditions
  $select->condition('surveygroup.survey_id', $surveydetailsId, '=');
  //$select->condition('surveyquestion.status', 'sry_qtn_sts_atv', '=');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execure the query
  $numSurveyDetails = $select->execute()->fetchField();
  expDebug::dPrint(' $numSurveyDetails111 = ' . print_r($numSurveyDetails, true) , 3);

  return $numSurveyDetails;
  } catch (Exception $ex) {
  	watchdog_exception('getChildSurveyDetailsQuestionCount', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBannerHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getBannerHTML($banner) {
  try {
  $detailsList = array();
  $actionList = array();
  $bannerActivateStr="";
  /*if (!empty($banner->code)) {
    $detailsList[] = array('detail' => $banner->code, 'tooltip' => t('LBL096'));
  }*/
  if (!empty($banner->language)) {
    $langchar = substr($banner->language, 0, 3);
    $detailsList[] = array('detail' => ($langchar == 'Sim') ? t('Chi') :t($langchar), 'tooltip' => t('LBL038').': '.t($banner->language));
  }
  /*if (!empty($banner->status)) {
    $detailsList[] = array('detail' => $banner->status, 'tooltip' => t('LBL102'));
  }	*/

  if (!empty($banner->date_activate) && $banner->date_activate!="0000-00-00 00:00:00") {
    $bannerActivateStr.=  date("F j, Y",strtotime($banner->date_activate));
  }

  if (!empty($banner->date_deactivate) && $banner->date_deactivate!="0000-00-00 00:00:00") {
     if($bannerActivateStr!="") $bannerActivateStr.=" ";
     $bannerActivateStr.=t("to")." ". date("F j, Y",strtotime($banner->date_deactivate));
  }

  if($bannerActivateStr!=""){
    $detailsList[] = array('detail' => t('LBL628')." ".$bannerActivateStr, 'tooltip' => t('LBL628')." ".$bannerActivateStr);
  }else{
     $detailsList[] = array('detail' => t('LBL629'), 'tooltip' => t('LBL629'));
  }

  $actionList[] = array('action_link_text' => t('LBL063'),
					  'action_page_path' => 'administration/manage/banner/nojs/addedit',
					  'action_button_params' => $banner->id,
                      'action_button_param_text' => 'banner',
  					  'ctools_style' => 'ctools-modal-ctools-admin-announcement-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  $actionList[] = array('action_link_text' => t('LBL286'),
  					  'action_page_path' => '',
					  'action_button_params' => $banner->id,
  					  'action_button_surveyassement_id' => $banner->id,
                      'action_button_param_text' => 'banner',
  					  'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );
 /*$actionList[] = array('action_link_text' => t('Catalog Control'),
					  'action_page_path' => 'administration/catalog-access/nojs',
					  'action_button_params' => $banner->id.'/cre_sys_obt_anm/'.trim($banner->title),
                      'ctools_style' => 'ctools-modal-ctools-admin-mro-catalog-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL239')
					  );*/
  $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $banner->id,
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'entityId' => $banner->id,
  		'entityType' => 'cbn_anm_typ_ban',
  		'icon_style_class'=>'access-tab-icon',
  		'postype' => 'topleft',
  		'poslwid' => '',
  );
  $viewlist[] = array('action_link_text' => 'Banner',
  					  'action_page_path' => 'administration/view-banner/nojs/addedit/'.$banner->id.'',
					  'action_button_params' => $banner->id,
  					  'action_button_surveyassement_id' => $banner->id,
  					  'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),

					  );


  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($banner->title),
                       'details_list' => $detailsList,
                       'delete_info' => t('MSG357').' '.t('LBL623'),
                       'action_list'=> $actionList,
                       'description'=> trim($banner->description),
                       'promote_action' => $promoteAction,
                 		   'new_action_list'=> $newActionList,
                 		   'sumedit' => $banner->sumedit,
                 		   'sumdelete' => $banner->sumdelete,
                 		   'view_list' => $viewlist,
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getBannerHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBannerActionsHTML() - Renders a announcement's actions as HTML to be displayed in jqGrid.
 */
function getBannerActionsHTML($banner) {
 /* return theme('narrow-search-results-item-actions',
                   array('edit_page_path' => '',
                         'edit_button_action_params' => array($banner->id)
                        )
              );*/
    try {
     return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $banner)
              );
     } catch (Exception $ex) {
     	watchdog_exception('getBannerActionsHTML', $ex);
     	expertusErrorThrow($ex);
     }
}

/**
 * announcementSearchSelectStmtAddOrderByClause() - Function to fetch and add announcement selected sort order type to the select statement
 */
function announcementSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".title";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias . ".title";
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
  	watchdog_exception('announcementSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendBannerListToJqGrid() - Prepares JSON response to JqGrid's request for Banner
 */
function sendBannerListToJqGrid() {
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

/*   $numBanner = fetchBannerAll('COUNT');

  expDebug::dPrint(' $numBanner = ' . print_r($numBanner, true) , 3);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numBanner); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);


  $bannerList = fetchBannerAll('LISTS', $start, $limit);
  expDebug::dPrint(' $bannerList = ' . print_r($bannerList, true) , 3);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numBanner;
                                             // have to do this.
  $response->initial_sort_type_html_id = 'bannerdetails-sort-new';

  $i=0;
  foreach ($bannerList as $banner) {
    expDebug::dPrint(' $banner = '. print_r($banner, true) , 3);

    $banner->row =  $i;
    $response->rows[$i]['id']   = $banner->id;

    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $banner)),
                                         'details' => getBannerHTML($banner),
                                         'action' => getBannerActionsHTML($banner)
                                         );

    $i++;
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendBannerListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBannerTextFilterAutoComplete() - Returns the matched announcement title
 */
function getBannerTextFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_announcement_master', 'announcement');

  // Add fields to fetch
  $select->addExpression('DISTINCT(announcement.title)', 'title');

  selectAddNarrowSearchByTextMultipleORCondition($select, "announcement", array("title", "code", "description", "shortdesc"), "z");
  $select->condition('announcement.status',array('cbn_anm_sts_atv','cbn_anm_sts_itv'),'IN');

  filterByUserAccessPrivileges($select, 'announcement','',1,'cbn_anm_typ_ban','','announcement'); // #46933 - Group access check
  // Order in dictionary sort order
  $select->orderBy('title');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // print_r($select->__toString());
  $matchingBannerNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingBannerNames = ' . print_r($matchingBannerNames, true) , 3);

  foreach ($matchingBannerNames as $announcementName) {
    print rawurlencode($announcementName->title) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getBannerTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchBannerAll() - Fetches announcement count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchBannerAll($op = NULL , $start = NULL , $limit = NULL) {
  try {
  // Select table
  $select = db_select('slt_announcement_master', 'announcement');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=announcement.id AND group_map.entity_type = \'cbn_anm_typ_ban\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cbn_anm_typ_ban\''); */
  // Left join the users, slt_survey_groups_questions and slt_survey_questions table to get the survey details
 // $select->leftJoin('slt_survey_groups_questions', 'surveygroup', 'surveydetails.id = surveygroup.survey_id');
  //$select->leftJoin('slt_survey_questions', 'surveyquestion', 'surveygroup.question_id = surveyquestion.id');
  $select->leftJoin('slt_profile_list_items', 'plilang', 'announcement.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'announcement.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  $select->condition('announcement.status', array('cbn_anm_sts_del','cbn_anm_sts_dft'), 'NOT IN');
  if($op=='COUNT'){
     // Get count of active survey
     $select->addExpression('COUNT(DISTINCT(announcement.id))', 'COUNT');
  }
  else{
  	$select->distinct();
    // Select fields/expression values to be fetched from the database.
 // $select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
 // $select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
	$select->addField('announcement', 'id', 'id');
	$select->addField('announcement', 'title', 'title');
	$select->addField('announcement', 'ann_type', 'ann_type');
	$select->addField('announcement', 'code', 'code');
	$select->addField('announcement', 'date_activate', 'date_activate');
	$select->addField('announcement', 'date_deactivate', 'date_deactivate');
	$select->addField('announcement', 'banner_seq_num', 'banner_seq_num');
	$select->addField('announcement', 'shortdesc', 'description');
	$select->addField('announcement', 'status', 'status_code');
	$select->addField('announcement', 'banner_thumbnail', 'banner_thumbnail');
	$select->addField('announcement', 'banner_large', 'banner_large');
	$select->addField('plilang', 'name', 'language');
	$select->addField('plistatus', 'name', 'status');
	$select->addExpression('\'bannertype\'', 'object_type');
	$select->groupBy('announcement.id');
  }
  filterByUserAccessPrivileges($select, 'announcement','',1,'cbn_anm_typ_ban','','announcement');
  // Set the conditions
  //$defaultBannerStatus = isset($_GET['announcementstatus']) ? '' : 'cbn_anm_sts_atv';
  selectAddNarrowSearchINCondition($select, 'announcement', 'status', 'announcementstatus');
  NarrowSearchByBannerLanguage($select,'announcementlang');
  selectAddNarrowSearchByDateRangeMultiCondition($select, 'announcementdaterange');
  selectAddNarrowSearchByTextMultipleORCondition($select, "announcement", array("title", "code", "description", "shortdesc"), "textfilter");

  //$dbdate= getAnnounmentDate(1);
 // selectAddNarrowSearchByTextCondition($select, 'surveyquestion', 'question_txt', 'surveyquestion', t("Type a survey question"));

  if($op != "COUNT"){
    // Order the records
    announcementSearchSelectStmtAddOrderByClause($select, 'announcement', "AZ");
  }

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  if($op == "COUNT"){
  	$result = $select->execute()->fetchField();
	expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
	return $result;
  }
  elseif($op == "CSV"){
	  $colHeadersMap = array(t('LBL083') => 'title',
                           t('LBL038') => 'language',
                           t('LBL102') => 'status',
                           t('LBL620') => 'date_activate',
	                       t('LBL654') => 'date_deactivate',
	                       t('LBL618') => 'banner_seq_num',
                       );

     $filterArray[] = array(t('LBL623').':'.textboxFiltersetValueToPrintableStr('textfilter'));
     $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('announcementstatus'));
     $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('announcementlang'));
     $filterArray[] = array(t('LBL622').':'.daterangeFiltersetValueToPrintableStr('announcementdaterange', true));
     $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());

  	 SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'banner_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL083') => array('title',          17),
                           t('LBL038') => array('language',        15),
    					   t('LBL102') => array('status',          18),
                           t('LBL620') => array('date_activate',   18),
                           t('LBL654') => array('date_deactivate', 18),
                           t('LBL618') => array('banner_seq_num',  14)
                        );


    $appliedFilters = array(t('LBL623') => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('announcementstatus'),
    						            t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('announcementlang'),
                            t('LBL622') => daterangeFiltersetValueToPrintableStr('announcementdaterange', true),
                            ucwords(t('LBL011')) => ucwords(getNarrowSearchSortTitle()),
                        );

    SendNarrowSearchResultsAsPDFFile($select, t('LBL623'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'banner_admin_search');
  }
  else{
  		add_row_id_condition($select, 'announcement');
	     // Limit to $limit records
	     $select->range($start, $limit);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    expDebug::dPrintDBAPI("banner select query ",$select);
	    /*--#38539: UI issue home page - fix --*/
	    foreach($result as $banner) {
	     $imageToCheck = (checkFrontSidebarExist()) ? $banner->banner_thumbnail : $banner->banner_large; // Check image based on the front sidebar status
	     $banner->image_avilable = file_exists($base_path . $imageToCheck);
	    }
	    expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3);
	    return $result;

  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchBannerAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function banner_admin_search_csv_row_data_alter($rowData) {
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
    watchdog_exception('banner_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function banner_admin_search_pdf_row_data_alter($rowData) {
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
    watchdog_exception('banner_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

function getDefaultBannerAddEditFormFieldValue($field, $form_state, $announcementDetails) {
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
  	watchdog_exception('getDefaultBannerAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}
function getBannerDetails($announcementId, $fields){
	try {
  if (empty($fields)) {
    // Get all fields
    $fields = array('id', 'tite', 'code', 'lang_code', 'description', 'status', 'date_activate','shortdesc',
					'date_deactivate', 'banner_seq_num', 'banner_thumbnail', 'banner_large', 'alert_priority', 'cal_event_date', 'created_by', 'created_on', 'updated_by',
					'updated_on', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
  }

  // Select table
  $selectStmt = db_select('slt_announcement_master', 'announcement');
  $selectStmt->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = announcement.lang_code');
  $selectStmt->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = announcement.status');

  // Set conditions
  $selectStmt->condition('announcement.id', $announcementId);

  // Select fields and/or add expressions
  foreach($fields as $fieldName) {
    $selectStmt->addField('announcement', $fieldName, $fieldName);
  }
  $selectStmt->addField('prof1','name','language_name');
  $selectStmt->addField('prof2','name','status_name');

  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  return $result->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getBannerDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewBanner($form, &$form_state){

  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state1 = ' . print_r($form_state['storage']['files_details'], true) , 5);

   $txn = db_transaction();
		try{
		    expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
			$vEntityTitle 	= $form_state['values']['title'];
			$vShortDesc 	= $form_state['values']['shortdesc']['value'];
			$vDesc 			= $form_state['values']['shortdesc']['value'];
			$vEntityType    ='cbn_anm_typ_ban';
			$EntityLang     = 'en';
			//$oDrupal = new DrupalBaseService();
			//$oDrupal->setDrupalBaseDir();
			global $user;
			$oNode = new StdClass();
			$oNode->uid=$user->uid;
			$oNode->language=$EntityLang;
			$oNode->title=$vEntityTitle;
			$oNode->body=$vDesc;

			$oNode->type=$vEntityType;
			$oNode->created=time();
			$oNode->changed=time();
			$forum_new->teaser_include=1;
			$oNode->revision=1;
			$node->teaser=$vShortDesc;

			$oNode->format=1;
			$oNode->log='';
			$oNode->name=$user->name;
			$oNode->date='';
			$oNode->status=1;
			$oNode->promote=0;
			$oNode->sticky=0;
			$oNode->op="Save";
			$oNode->submit="Save";
			$oNode->preview="Preview";
			$oNode->form_id="forum_node_form";
			$oNode->comment=COMMENT_NODE_READ_WRITE;


			node_save($oNode);
           expDebug::dPrint('node_save : $form_state = ' . print_r($form_state, true) , 5);
			//die('sdsdsd');
			//$result	=	array();
			//$result['nid']	=	$oNode->nid;
			//$result['vid']	=	$oNode->vid;
			//$oDrupal->restoreWorkingDir();
			//return $result;


	if(!empty($form_state['values']['announcement_banner'])){
	include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
     $validators  = array(
     'file_validate_extensions' => array('png jpg jpeg gif'), // Validate extensions.
          // 'file_validate_is_image' => array(), // Validates file is really an image.
         // 'file_validate_image_resolution' => array('85x85'),
     );

   }

  //  if(!empty($form_state['storage']['files_details'])){
      //  $newBannerName=$form_state['storage']['files_details'];
 //  }else {
  //     $newBannerName=null;
  //  }

  // $imageExtension = file_validate_is_image(&$_FILES['files']['tmp_name']['announcement_banner']);


    //$_FILES['announcement_banner']=$form_state['storage']['files_details'];
    expDebug::dPrint(' $imageExtension = ' . print_r($imageExtension, true) , 4);
  // exit();

	$announcement = file_save_upload('announcement_banner', $validators,$destination = FALSE);
   expDebug::dPrint(' $filedirectory = ' . print_r($announcement, true) , 4);
	if(!empty($announcement)) {
            if (!$announcement->status) { // i.e. if picture is a temporary file
              // Prepare the pictures directory.
              $filedirectory = file_default_scheme() . '://' . variable_get('announcement_attachment_path', 'images');
             expDebug::dPrint(' $filedirectory = ' . print_r($filedirectory, true) , 4);
              file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);

              // Prepare the unique file name to place the picture under the $filedirectory.
              expDebug::dPrint(' $info returned by image_get_info = ' . print_r($info, true) , 4);
              $destination = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$announcement->filename);
              expDebug::dPrint(' $destination = ' . print_r($destination, true) , 4);

              // Move the temporary file into the final location.
              if ($announcementmove = file_move($announcement, $destination, FILE_EXISTS_RENAME)) {
                expDebug::dPrint(' $picture returned by file_move() = ' . print_r($announcementmove, true) , 4);

                $announcementmove->status = FILE_STATUS_PERMANENT;
                $save_announcement = file_save($announcementmove);
                expDebug::dPrint(' $picture returned by file_save() = ' . print_r($save_announcement, true) , 4);

                $sitepath      ="sites/default/files/images/";
                $newBannerName =$sitepath.$save_announcement->filename;
              }
            }
          }

      $announcement_large = file_save_upload('announcement_banner_large', $validators,$destination = FALSE);
      if(!empty($announcement_large)) {
      	if (!$announcement_large->status) { // i.e. if picture is a temporary file
      		// Prepare the pictures directory.
      		$filedirectory = file_default_scheme() . '://' . variable_get('announcement_attachment_path', 'images');
      		expDebug::dPrint(' $filedirectory Lar = ' . print_r($filedirectory, true) , 4);
      		file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);

      		// Prepare the unique file name to place the picture under the $filedirectory.
      		expDebug::dPrint(' $info returned by image_get_info Lar = ' . print_r($info, true) , 4);
      		$destination = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$announcement_large->filename);
      		expDebug::dPrint(' $destination Lar= ' . print_r($destination, true) , 4);

      		// Move the temporary file into the final location.
      		if ($announcementmove_large = file_move($announcement_large, $destination, FILE_EXISTS_RENAME)) {
      			expDebug::dPrint(' $picture returned by file_move() Lar= ' . print_r($announcementmove_large, true) , 4);

      			$announcementmove_large->status = FILE_STATUS_PERMANENT;
      			$save_announcement_large = file_save($announcementmove_large);
      			expDebug::dPrint(' $picture returned by file_save() Lar= ' . print_r($save_announcement_large, true) , 4);

      			$sitepath      ="sites/default/files/images/";
      			$newBannerNameLarge =$sitepath.$save_announcement_large->filename;
      		}
      	}
      }
      if(!empty($form_state['values']['date_activate'])){
      $old_dateformat=str_replace("-","/",$form_state['values']['date_activate']);
      $date_activate= date_format(date_create( $old_dateformat),'Y-m-d H:i');
      }
      if(!empty($form_state['values']['date_deactivate'])){
      $old_dateformat1=str_replace("-","/",$form_state['values']['date_deactivate']);
      $date_deactivate= date_format(date_create( $old_dateformat1),'Y-m-d H:i');
       }

      expDebug::dPrint('Trigger element = ' . print_r($form_state['storage']['submit_triggering_element'], true) , 5);
     $trigger_element = $form_state['storage']['submit_triggering_element'];
     if($trigger_element == 'saveandpublish'){
       $banner_status = 'cbn_anm_sts_atv';
     }
     else if($trigger_element == 'save'){
       $banner_status = 'cbn_anm_sts_itv';
     }
     else if($form_state['values']['crs_status']){
  	$banner_status =$form_state['values']['status'];
     }
     // This issue is fixed For this ticket #0037467
     if(!empty($date_deactivate) && empty($date_activate)){
     	$date_activate = date('Y-m-d');
     }
      $createUserId = getIdOfLoggedInUser();
      	// Select the table to insert the record in
      $insertStmt = db_insert('slt_announcement_master');
      $fields = array(
                  'title'                  => $form_state['values']['title'],
                  'code'                   => null,//$form_state['values']['code'],
                  'node_id'                => $oNode->nid,
                  'vid'                    => $oNode->vid,
                  'created_by'             => $createUserId,
                  'created_on'             => now(),
      			  'updated_by'             => $createUserId,
      			  'updated_on'             => now(),
                  'ann_type'               => 'cbn_anm_typ_ban',
                  'description'            => null,
                  'shortdesc'              => $form_state['values']['shortdesc']['value'],
                  'lang_code'              => $form_state['values']['lang_code'],
                  'status'       		       => $banner_status, //$form_state['values']['status'],
                  'banner_thumbnail'       => $newBannerName,
      			  'banner_large'       	   => $newBannerNameLarge,
                  'date_activate'   	   => empty($date_activate) ? NULL :$date_activate,
                  'date_deactivate'        => empty($date_deactivate) ? NULL : $date_deactivate,
                  'banner_seq_num'    	   => $form_state['values']['banner_seq_num'],

                );
     $insertStmt->fields($fields);

    $form_state['storage']['image_set']= $newBannerName;
    $form_state['storage']['image_set_large']= $newBannerNameLarge; // to store temp values
    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

     $announcementId = $insertStmt->execute();
     updateGroupMappingDetails($announcementId, 'cbn_anm_typ_ban', $createUserId,'emptyadd',0);
     $form_state['values']['id'] = $announcementId;
     //Audit trail enrtries
    $form_state['values']['banner_thumbnail']=$newBannerName;
    $form_state['values']['banner_large']=$newBannerNameLarge;
    make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$announcementId, 'Inserted','cbn_anm_typ_ban','exp_sp_administration_announcement','Add new Banner',
                                         array('title', 'code', 'shortdesc', 'lang_code', 'status','banner_seq_num','date_activate','date_deactivate','banner_thumbnail'));


     return array((object)array('id'=>$announcementId));

		}catch (Exception $ex){
          $txn->rollback();
          watchdog_exception('addNewBanner', $ex);
          throw $ex;

		}
	}


 function updateBannerDetails($form, &$form_state){

    try{

      	$vEntityTitle 		= $requestData->Title;
      	$vShortDesc 		= $requestData->ShortDesc;
      	$vDesc 				= $requestData->Desc ;
      	$vNodeId 			= $requestData->NodeId;
      	$vRevisionId 		= $requestData->RevisionId;

      	$vEntityTitle 	    = $form_state['values']['title'];
      	$vShortDesc 	    = $form_state['values']['shortdesc']['value'];
      	$vDesc 			    = $form_state['values']['shortdesc']['value'];
      	$vNodeId 			= $form_state['values']['node_id'];//$requestData->node_id;
      	$vRevisionId 		= $form_state['values']['vid'];//$requestData->RevisionId;
      	$vEntityType        ='cbn_anm_typ_ban';
      	$EntityLang         = 'en';

          $userid="";
          if($paramuserid=="")
          {
          	global $user;
          	$userid=$user->uid;
          }
          else
          {
          	$userid=$paramuserid;
          }

  		//global $user;
  		$oNode = new StdClass();

  		$oNode->nid=$vNodeId;
  		$oNode->vid=$vNodeId;

  		$oNode->uid=$userid;//$user->uid;
  		$oNode->language=$EntityLang;
  		$oNode->title=$vEntityTitle;
  		$oNode->body=$vDesc;

  		$oNode->type=$vEntityType;
  		$oNode->created=time();
  		$oNode->changed=time();
  		$forum_new->teaser_include=1;
  		$oNode->revision=1;
  		$node->teaser=$vShortDesc;

  		$oNode->format=1;
  		$oNode->log='';
  		$oNode->name=$user->name;
  		$oNode->date='';
  		$oNode->status=1;
  		$oNode->promote=0;
  		$oNode->sticky=0;
  		$oNode->op="Save";
  		$oNode->submit="Save";
  		$oNode->preview="Preview";
  		$oNode->form_id="forum_node_form";
  		$oNode->comment=COMMENT_NODE_READ_WRITE;


  		node_save($oNode);
  		$result	=	array();
  		$result['nid']	=	$oNode->nid;
  		//$oDrupal->restoreWorkingDir();
  		//return $result['nid'];


          $old_dateformat=str_replace("-","/",$form_state['values']['date_activate']);
          $date_activate= date_format(date_create( $old_dateformat),'Y-m-d H:i:s');
          $old_dateformat1=str_replace("-","/",$form_state['values']['date_deactivate']);
          $date_deactivate= date_format(date_create( $old_dateformat1),'Y-m-d H:i:s');

          //if(!empty($form_state['values']['announcement_banner'])){
  	    include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
          $validators  = array(
             'file_validate_extensions' => array('png jpg jpeg gif'), // Validate extensions.
             'file_validate_is_image' => array(), // Validates file is really an image.
           // 'file_validate_image_resolution' => array('85x85'),
          );

          //}
          $newBannerName=null;
          if(!empty($form_state['storage']['files_details'])){
              $newBannerName=$form_state['storage']['files_details'];
              $numUpdated = db_update('slt_announcement_master')
                            ->fields(array('banner_thumbnail' => $newBannerName))
                            ->condition('id', $form_state['values']['id'])
                            ->execute();
                    $form_state['storage']['image_set']= $newBannerName;
                    $form_state['values']['banner_thumbnail']=$newBannerName;
          }

      	$announcement = file_save_upload('announcement_banner',$validators);
      	if(!empty($announcement)) {
              if (!$announcement->status) { // i.e. if picture is a temporary file
                // Prepare the pictures directory.
                $filedirectory = file_default_scheme() . '://' . variable_get('announcement_attachment_path', 'images');
                expDebug::dPrint(' $filedirectory = ' . print_r($filedirectory, true), 4);
                file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);

                // Prepare the unique file name to place the picture under the $filedirectory.
                expDebug::dPrint(' $info returned by image_get_info = ' . print_r($info, true) , 4);
                $destination = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$announcement->filename);
                expDebug::dPrint(' $destination = ' . print_r($destination, true) , 4);

                // Move the temporary file into the final location.
                if ($announcementmove = file_move($announcement, $destination, FILE_EXISTS_RENAME)) {
                   expDebug::dPrint(' $picture returned by file_move() = ' . print_r($announcementmove, true) , 4);

                   $announcementmove->status = FILE_STATUS_PERMANENT;
                   $save_announcement = file_save($announcementmove);
                   expDebug::dPrint(' $picture returned by file_save() = ' . print_r($save_announcement, true) , 4);

                   $sitepath      ="sites/default/files/images/";
                   $newBannerName =$sitepath.$save_announcement->filename;

                   $numUpdated = db_update('slt_announcement_master')
                        ->fields(array('banner_thumbnail' => $newBannerName))
                        ->condition('id', $form_state['values']['id'])
                        ->execute();
                   $form_state['storage']['image_set']= $newBannerName;
                   $form_state['values']['banner_thumbnail']=$newBannerName;
                }
              }
           } else {
           	// set default db value to the form
           	$banner = getBannerDetails($form_state['values']['id'], array('banner_thumbnail'));
           	$form_state['storage']['image_set'] = $banner['banner_thumbnail'];
           	$form_state['values']['banner_thumbnail'] =$banner['banner_thumbnail'];

           }

           $announcement_large = file_save_upload('announcement_banner_large',$validators);
           if(!empty($announcement_large)) {
           	if (!$announcement_large->status) { // i.e. if picture is a temporary file
           		// Prepare the pictures directory.
           		$filedirectory = file_default_scheme() . '://' . variable_get('announcement_attachment_path', 'images');
           		file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);

           		// Prepare the unique file name to place the picture under the $filedirectory.
           		$destination = file_stream_wrapper_uri_normalize($filedirectory . '/'.REQUEST_TIME . '-' .$announcement_large->filename);

           		// Move the temporary file into the final location.
           		if ($announcementmove_large = file_move($announcement_large, $destination, FILE_EXISTS_RENAME)) {
           			$announcementmove_large->status = FILE_STATUS_PERMANENT;
           			$save_announcement_large = file_save($announcementmove_large);
           			$sitepath      ="sites/default/files/images/";
           			$newBannerNameLarge =$sitepath.$save_announcement_large->filename;

           			$numUpdated = db_update('slt_announcement_master')
           			->fields(array('banner_large' => $newBannerNameLarge))
           			->condition('id', $form_state['values']['id'])
           			->execute();
           			$form_state['storage']['image_set_large']= $newBannerNameLarge;
           			$form_state['values']['banner_large']=$newBannerNameLarge;
           		}
           	}
           } else {
           	// set default db value to the form
           	$banner = getBannerDetails($form_state['values']['id'], array('banner_large'));
           	$form_state['storage']['image_set_large'] = $banner['banner_large'];
           	$form_state['values']['banner_large'] =$banner['banner_large'];
           }

           // Select table to update
           $updateStmt = db_update('slt_announcement_master');

          // Set conditions
          $updateStmt->condition('id', $form_state['values']['id']);
          $formid=$form_state['values']['id'];
          $default= db_query("SELECT title,code,node_id,shortdesc,lang_code,status,date_activate,date_deactivate,banner_seq_num,banner_thumbnail from slt_announcement_master where id=:uid",array(":uid"=>$formid))->fetchAll();
          expDebug::dPrintDBAPI(' default_values = ' , $default);

          expDebug::dPrint('Trigger element = ' . print_r($form_state['storage']['submit_triggering_element'], true) , 5);
          $trigger_element = $form_state['storage']['submit_triggering_element'];
          if($trigger_element == 'saveandpublish'){
             $banner_status = 'cbn_anm_sts_atv';
          } else if($trigger_element == 'save'){
             $banner_status = 'cbn_anm_sts_itv';
          } elseif($trigger_element == 'unpublish'){
             $banner_status = 'cbn_anm_sts_itv';
          } else if($form_state['values']['crs_status']){
    	       $banner_status = $form_state['values']['status'];
          }
          // This issue is fixed For this ticket #0037467
          if(!empty($date_deactivate) && empty($date_activate)){
          	$date_activate = date('Y-m-d');
          }
          $createUserId = getIdOfLoggedInUser();

          if($trigger_element == 'unpublish'){
              $updateStmt = db_update('slt_announcement_master');
              $updateStmt->condition('id', $form_state['values']['id']);
              $fields = array(
              				'status' => $banner_status,
              				'updated_by'             => $createUserId,
              				'updated_on'             => now(),
              );

          } else {

            $updateStmt = db_update('slt_announcement_master');
            $updateStmt->condition('id', $form_state['values']['id']);

            $fields = array(
                  'title'                  => $form_state['values']['title'],
                  'code'                   =>  null, //$form_state['values']['code'],
                  'node_id'                => $oNode->nid,
                  'vid'                    => $oNode->vid,
                  'updated_by'             => $createUserId,//$user->uid,
                  'updated_on'             => now(),
                  'ann_type'               => 'cbn_anm_typ_ban',
               //  'description'            => $form_state['values']['description']['value'],
                   'description'           => null,
                  'shortdesc'              => $form_state['values']['shortdesc']['value'],
                  'lang_code'              => $form_state['values']['lang_code'],
                  'status'       		     => $banner_status, //$form_state['values']['status'],
                  //'banner_thumbnail'       => $sitepath.$save_announcement->filename,
                 'date_activate'   	   => empty($date_activate) ? NULL :$date_activate,
                 'date_deactivate'        => empty($date_deactivate) ? NULL : $date_deactivate,
                  'banner_seq_num'    	 => $form_state['values']['banner_seq_num'],

                );
          }
          $form_state['storage']['image_set']= $default[0]->banner_thumbnail;
          $RemovePhoto = $form_state['values']['rem_link'];

          //To delete the picture if the checkbox is checked
          if($RemovePhoto == 1){
          //if ($form_state['values']['rem_link'] == 1) {
          	$fields['banner_thumbnail']=null;
          	$form_state['storage']['image_set']=null;
            chmod($default[0]->banner_thumbnail, 777);
          	unlink($default[0]->banner_thumbnail);
          	$form_state['values']['banner_thumbnail']=null;

          }

          $updateStmt->fields($fields);
          expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
          // Execute query
          $numUpdated = $updateStmt->execute();

          make_audit_trail_entries($fields, $user->uid , $default ,$formid, 'Updated','cbn_anm_typ_ban','exp_sp_administration_announcement','updateBannerDetails',
                                             array('title', 'code', 'shortdesc', 'description', 'lang_code', 'status','banner_seq_num','date_activate','date_deactivate','banner_thumbnail'));


  		return array((object)array('id'=>$form_state['values']['id']));

	} catch (Exception $e){
		throw new SoapFault("SPLMS",$e->getMessage());
	}

 }

 function uniqueBannerCode($form_state) {
  // Select slt_program table
  try {
  $select = db_select('slt_announcement_master', 'annumnt');
  $select->addExpression('COUNT(annumnt.id)', 'count');
  // Set the conditions
  $select->condition('annumnt.code', $form_state['values']['code'], '=');
  if (!empty($form_state['values']['id'])) { // An update operation, do not compare with self's title
    $select->condition('annumnt.id', $form_state['values']['id'], '!=');
  }

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Execute the query
  $numAnn = $select->execute()->fetchField();
  expDebug::dPrint(' $numPrg = ' . print_r($numAnn, true) , 3);

  if ($numAnn > 0) {
    return false; // Not unique
  }

  return true; // a valid new name
  } catch (Exception $ex) {
  	watchdog_exception('uniqueBannerCode', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewBannerByRestAPI(&$form_state){
	try {
    $form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	expDebug::dPrint(' addNewBannerByRestAPI created ANN:' );
	$annId =  addNewBanner($form,$form_state);
	expDebug::dPrint(' addNewBannerByRestAPI created tp:' . print_r($annId,true) ,5);
	return $annId;
	} catch (Exception $ex) {
		watchdog_exception('addNewBannerByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function updateBannerByRestAPI(&$form_state){
  try{
    $form=array();
		$form["form_id"]="";
		$announcementId = $form_state['values']['id'];
    $select = db_select('slt_announcement_master', 'announcement');
    $select->addField('announcement','node_id','node_id');
    $select->addField('announcement','vid','vid');
    $select->condition('announcement.id', $announcementId);
    $select->range(0,1);
    $details = $select->execute()->fetchAssoc();
    $form_state['values']['node_id'] = $details['node_id'];
    $form_state['values']['vid'] = $details['vid'];
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		expDebug::dPrint(' addNewBannerByRestAPI created ANN:' );
		$annId =  updateBannerDetails($form,$form_state);
		expDebug::dPrint(' addNewBannerByRestAPI created tp:' . print_r($annId,true) ,5);
		return $annId;
	} catch (Exception $ex) {
		watchdog_exception('updateBannerByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function bannerProfileListItems($codePrefix){
	try {
  //$statusOrder=array('cbn_anm_sts_atv','cbn_anm_sts_itv','cbn_anm_sts_dft');
  $select = db_query('SELECT code,name FROM slt_profile_list_items WHERE code IN(\'cbn_anm_sts_atv\',\'cbn_anm_sts_itv\',\'cbn_anm_sts_dft\') AND is_active = \'Y\' ORDER BY FIELD(CODE,\'cbn_anm_sts_atv\',\'cbn_anm_sts_itv\',\'cbn_anm_sts_dft\')');
  $result = $select->fetchAllKeyed();
  expDebug::dPrintDBAPI(' addNewBannerByRestAPI created tp:' , $result );
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('bannerProfileListItems', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addTextFormatFieldToAddEditForm() -  Use this function to add a text format to the addedit form
 */
function annouoncementaddTextFormatFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $valueFormat = 'full_html', $tabIndex = null, $mandatory = false, $readOnly = false) {
	try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all text format fields.
  $attributes['class'][] = 'addedit-edit-textarea';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textarea';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }
   // Create the form element
  $form[$id] = array(
    '#type' => 'text_format',
    '#format'=> $valueFormat,
    '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
     '#suffix' => addEditGetSuffix('FIELD'),
    '#suffix' => '<div class="admin-desc-character-count"><span id="max_char_count_announcement"></div>'.addEditGetSuffix('FIELD'),
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,

  );
  } catch (Exception $ex) {
  	watchdog_exception('annouoncementaddTextFormatFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function getAnnounmentDate($annid) {
 try {
 	$select = db_select('slt_announcement_master', 'ann');
  // Set the conditions
  $select->condition('ann.id', $annid, '=');
  $select->addField('ann','date_activate','date_activate');
  $select->addField('ann','date_deactivate','date_deactivate');
  // Execute the query
  $result = $select->execute()->fetchAll();
  expDebug::dPrint('  PDF $pdfList after mod = ' . print_r($result, true) , 3);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getAnnounmentDate', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the survey assessment
 */
function publishOrUnpublishBanner($bannerId, $bannerType ){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    $status  = getBannerStatus($bannerId);
    $updateStmt = db_update('slt_announcement_master');
    $updatedstatus  = ($status=='cbn_anm_sts_atv')?"cbn_anm_sts_itv":"cbn_anm_sts_atv";

    $updateStmt->condition('id', $bannerId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();

    add_audit_trail_entry($currUserId, $bannerId, 'cbn_anm_typ_ban', 'exp_sp_administration_banner', 'publishOrUnpublishbanner', 'Updated banner Status',
		                                     null, $status, $updatedstatus);

	$returnStatus  = ($updatedstatus=='cbn_anm_sts_atv')?t('LBL571'):t('LBL570');
	print $updatedstatus.'|'.$returnStatus;
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishBanner', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of the program
 */
function getBannerStatus($bannerId){
	try {
  $select = db_select('slt_announcement_master', 'ban');
  $select->addField('ban','status', 'status');
  $select->condition('ban.id',$bannerId);
  // Execute the query
  $survStatus = $select->execute()->fetchField();
  return $survStatus;
  } catch (Exception $ex) {
  	watchdog_exception('getBannerStatus', $ex);
  	expertusErrorThrow($ex);
  }
}

function bannersubmitAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $bannerId, $subStage = '', $emptyId = '') {
	try {
  global $theme_key;
  expDebug::dPrint(' addEditFormWithCancelAndSaveClose() called.');

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
  $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;


  if($subStage == 'details' || empty($subStage)) {
    $bannerStatus     = '';
    if (!empty($bannerId)) {
      $bannerDetails   = getAllBannerDetails($bannerId, array('id, status, title'));
      $bannerStatus    =  $bannerDetails[0]->status;
    }
   $isSaveParent = 0;
   $leftBg = '';
   if($theme_key == 'expertusoneV2'){
   	$leftBg = '<div class="admin-save-button-left-bg"></div>';
   }
   if(empty($bannerId) || $bannerStatus != 'cbn_anm_sts_atv'){
      $isSaveParent = 1;
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = 'increasePopWidth();';
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-pub-unpub-button-container">'.$leftBg,
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
      $savePublishSuffix = ((($resultCnt==0) && ($bannerStatus == 'cbn_anm_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';
    }
	  $saveButtonId = 'catalog-course-save-publish';
	  $savePublishAttributes['onclick'] = array();
	  $savePublishAttributes['tabindex'][] = '9';

      $savePublishAttributes['onclick'][] = 'increasePopWidth();';
     $buttonLabel = ($bannerStatus == 'cbn_anm_sts_atv') ? t('LBL141') : t('LBL614');
     $savePublishAttributes['class'][] = (($resultCnt==0) && ($bannerStatus == 'cbn_anm_sts_atv')) ? 'edit-catalog-course-save-publish' : 'pub-unpub-only-save-btn';
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

	if(($bannerId) && $bannerStatus == 'cbn_anm_sts_atv'){
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
	$deleteCheck = adminVisibilityForAddEditDelete($bannerId,'cbn_anm_typ_ban','cbn_anm_typ_ban','slt_announcement_master');
	if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
    if($bannerId && ($bannerStatus == 'cbn_anm_sts_itv')){
      if($resultCnt == 0){
        $displayTitle = titleController('NARROW_SEARCH', $bannerDetails[0]->title);
  	   $saveButtonId = 'catalog-course-delete';

  	   $deleteAttributes['onclick'] = array();
        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357'). ' '.
                                              strtolower(t('LBL623')).' '.escape_string($bannerDetails[0]->title) . '","'.$bannerId.'","banner"); return false;';
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
  // Catalog control
  $entityId          	 = $bannerId;//.',"'.$entityType
/*  if(!empty($emptyId)){
  	$entityId = 0;
  } else {
    $emptyId = 0;
  }*/

  if(!empty($entityId)){
  	$emptyId = 0;
  } else {
    $entityId = 0;
  }


  $entityType        	 = 'cbn_anm_typ_ban';
  $qtipIdInit        	 = $entityId.'_'.$entityType;

  $qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'cbn_anm_typ_ban','url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."','wid':650,'heg':'270','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-banner-".$entityId."'}";


  //Temporary Hide
  $form['addedit-left-col-extend-func'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
    			  "<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-banner-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptAccessObj.");\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
    			  '</div>',
  );

  $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';

  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  	'#prefix' => '<div style="display:none;"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('bannersubmitAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}


function getAllBannerDetails($bannerId,$fields) {
  try{
  $select = db_select('slt_announcement_master', 'announcement');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = announcement.lang_code');
  $select->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = announcement.status');
  $select->addField('announcement','id','ban_id');
  $select->addField('announcement','title','title');
  $select->addField('announcement','status','status');

  $select->condition('announcement.id',$bannerId,'=');
  $banList = $select->execute()->fetchAll();

  return $banList;
  } catch (Exception $ex) {
  	watchdog_exception('getAllBannerDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function getBannerTitleDirect($bannerId){
	try {
	$selectBan = db_select('slt_announcement_master','ban');
	$selectBan->addField('ban','title', 'ban_title');
	$selectBan->condition('ban.id', $bannerId,'=');
	$result =  $selectBan->execute()->fetchAssoc();
	return $result['ban_title'];
	} catch (Exception $ex) {
		watchdog_exception('getBannerTitleDirect', $ex);
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
  	expDebug::dPrint('addBannerFileUploadControlToAddEditForm true part');
    $prefix = addEditGetPrefix('DATEFIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('DATEFIELD');
  }
  else {
  	expDebug::dPrint('addBannerFileUploadControlToAddEditForm else part');
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

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-datefield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // Set the data required by Drupal.behaviors.addDatePickerToAddEditDateField to attach datepicker, etc.
  $attributes['data-datePickerDatesDisplayOption'] = $datePickerDatesDisplayOption;
  $attributes['data-emptyDateFieldText'] = $emptyDateFieldText;
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
   //$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_DATEFIELD', $title, $mandatory);
   // $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_DATEFIELD');
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
    '#maxlength' => $maxChars
  );
  } catch (Exception $ex) {
  	watchdog_exception('addDateFieldToAddEditFormNew', $ex);
  	expertusErrorThrow($ex);
  }
}

function delete_banner_images($form, &$form_state){

  $bannerId = $form_state['triggering_element']['#ajax']['banner_id'];
  expDebug::dPrint('banner id value' . print_r($bannerId, true) , 5);

  $txn = db_transaction();
  try {
    if($bannerId){
    	if ($form_state['triggering_element']['#name'] == 'Delete Large banner') {
    		$deleteField = 'banner_large';
    		$defaultField = 'banner_thumbnail';
    	} else {
    		$deleteField = 'banner_thumbnail';
    		$defaultField = 'banner_large';
    	}

    	$select = db_select('slt_announcement_master', 'annumnt');
        $select->addField('annumnt', $deleteField,'bannerimage');
        $select->condition('annumnt.id', $bannerId, '=');
        $getImage = $select->execute()->fetchField();

        $fields[$deleteField]=null;
      	$form_state['storage']['image_set']=null;
      	chmod($getImage, 777);
      	unlink($getImage);
      	$form_state['values'][$deleteField]=null;

      	// set default db value to the another image element
      	$banner = getBannerDetails($form_state['values']['id'], array($defaultField));
      	$form_state['values'][$defaultField] =$banner[$defaultField];
      	if ($deleteField == 'banner_large') {
      		$form_state['storage']['image_set_large']=null;
      	}

      	$updateStmt = db_update('slt_announcement_master');
        $updateStmt->condition('id',$bannerId);
        $updateStmt->fields($fields);
        $numUpdated = $updateStmt->execute();
        $form_state['storage']['edited_entity'] = TRUE;
        $form_state['rebuild'] = TRUE;
        /*$deleteStmt = db_update('slt_announcement_master');
        $deleteStmt->condition('id', $bannerId);
        $numUpdated = $deleteStmt->execute();*/


    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('delete_banner_images', $ex);
    throw $ex;
  }

  unset($txn);

  $form_state['display_class'] = TRUE;


}

/*
 * selectAddNarrowSearchByDateRangeMultiCondition() - To the $selectStmt add Search by a date range if set by the user
 */
function selectAddNarrowSearchByDateRangeMultiCondition($selectStmt,$filtersetCode) {
	try {
  $dateRangeFilters = getURLParam($filtersetCode); // getURLParam() removes dash character from the dates
  expDebug::dPrint('$dateRangeFilters = ' . print_r($dateRangeFilters, true), 4);

  if (!empty($dateRangeFilters)) {
    $andcondition = db_and();
    $dateRange = explode("|", $dateRangeFilters);
    expDebug::dPrint('$dateRange = ' . print_r($dateRange, true), 4);
    $dateRange[0] = date_format(date_create_from_format('m-d-Y', $dateRange[0]), 'm-d-Y');
    $dateRange[1] = date_format(date_create_from_format('m-d-Y', $dateRange[1]), 'm-d-Y');
//     if(!empty($dateRange[0]) || !empty($dateRange[1])){
//     if (!empty($dateRange[0])) {
//       $dateRange[0] = date_format(date_create_from_format('mdY', $dateRange[0]), 'm-d-Y'); // inserting dash char in dates
//       $andcondition->where($tableAlias . '.' . $tableField2 ." between '".convertDateToDBDateFormat($dateRange[0],false)."' and '2999-12-31 00:00:00'". ' OR ' .
//       											$tableAlias . '.' . $tableField1 ." between '".convertDateToDBDateFormat($dateRange[0],false)."' and '2999-12-31 00:00:00'");
//     }

//     if (!empty($dateRange[1])) {
//     	$dateRange[1] = date_format(date_create_from_format('mdY', $dateRange[1]), 'm-d-Y'); // inserting dash char in dates
//       $andcondition->where($tableAlias . '.' . $tableField2 ." between '1970-01-01 00:00:00' and  '". convertDateToDBDateFormat($dateRange[1],false)."'". ' OR ' .
//       											$tableAlias . '.' . $tableField1 ." between '1970-01-01 00:00:00' and  '". convertDateToDBDateFormat($dateRange[1],false)."'");
//     }
//     $selectStmt->condition(db_or()->isnull($tableAlias . '.' . $tableField1)->isnull($tableAlias . '.' . $tableField1)->condition($andcondition));
//     }
    if(!empty($dateRange[0]) && !empty($dateRange[1])){
    	$selectStmt->condition('announcement.date_activate', convertDateToDBDateFormat($dateRange[0],false), '>=');
    	$selectStmt->condition('announcement.date_deactivate', convertDateToDBDateFormat($dateRange[1],false), '<=');
   }
   else if(!empty($dateRange[0]) && $dateRange[1]==''){
    	$selectStmt->condition('announcement.date_activate', convertDateToDBDateFormat($dateRange[0],false), '>=');
   }
    else if(!empty($dateRange[1]) && $dateRange[0]==''){
    	$selectStmt->condition('announcement.date_deactivate', convertDateToDBDateFormat($dateRange[1],false), '<=');

    }
  } // if
  //$andcondition = db_and()->condition('ann.date_activate', $currentDateTime, '<=')->condition('ann.date_deactivate', $currentDateTime, '>=');
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByDateRangeMultiCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_viewbanner_addedit_form($js = NULL, $entityId){
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
					array('id' => 'exp_sp_administration_view_bannerdetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
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


function exp_sp_administration_view_bannerdetails_addedit_form($form, &$form_state,$renderMode, $bannerId){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$courseId---->'.$bannerId);

		$wrapperId = 'viewbanner-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$bannerdetails = getBannerDetailsInfoView($bannerId);

		expDebug::dPrint('$$$$bannerdetails-->'.print_r($bannerdetails,true),5);
		$form['narrow_bannerview'] = array(
				'#markup'       => theme('narrow_bannerview', array('record' => $bannerdetails)),
		);

		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_bannerdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getBannerDetailsInfoView($bannerId){
	try{
		$select = db_select('slt_announcement_master', 'announcement');
		$select->leftJoin('slt_profile_list_items', 'plilang', 'announcement.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('slt_profile_list_items', 'plistatus', 'announcement.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
		$select->addField('announcement', 'id', 'id');
		$select->addField('announcement', 'title', 'title');
		$select->addField('announcement', 'ann_type', 'ann_type');
		$select->addField('announcement', 'code', 'code');
		$select->addField('announcement', 'date_activate', 'date_activate');
		$select->addField('announcement', 'date_deactivate', 'date_deactivate');
		$select->addField('announcement', 'banner_seq_num', 'banner_seq_num');
		$select->addField('announcement', 'shortdesc', 'description');
		$select->addField('announcement', 'status', 'status_code');
		$select->addField('announcement', 'banner_thumbnail', 'banner_thumbnail');
		$select->addField('plilang', 'name', 'language');
		$select->addField('plistatus', 'name', 'status');
		$select->addExpression('\'bannertype\'', 'object_type');
		$select->groupBy('announcement.id');
		$select->condition('announcement.id',$bannerId, '=');
		expDebug::dPrintDBAPI('execute banner query',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('v$result$result$result->'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getBannerDetailsInfoView', $ex);
		expertusErrorThrow($ex);
	}
}

function NarrowSearchByBannerLanguage($selectStmt,$filtersetCode) {
	try{
		$preferredlang = getUserPreferredLanguage();
		$langFilter= !isset($_GET['announcementlang']) ? $preferredlang : $_GET['announcementlang'];
		if (!empty($langFilter)){
			$langCode = explode("|", $langFilter);
			$selectStmt->condition('announcement.lang_code', $langCode, 'IN');
		}
	} catch (Exception $ex) {
		watchdog_exception('NarrowSearchByAnnouncementLanguage', $ex);
		expertusErrorThrow($ex);
	}
}
?>