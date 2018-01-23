<?php

/**
 * Content Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getContentAdminTabContentType() - For content admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getContentAdminTabContentType(){
	try {
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getContentAdminTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getContentStatusFilters() - Get the status narrow search filters for content admin
 */
function getContentStatusFilters() {
	try {
  $select = db_query('SELECT code, name FROM slt_profile_list_items ' .
                     'WHERE code like \'lrn_cnt_sts_%\' ' .
                            'AND code != \'lrn_cnt_sts_del\' ' .
                            'AND is_active = \'Y\' ' .
                     'ORDER BY FIELD(code, \'lrn_cnt_sts_atv\', \'lrn_cnt_sts_itv\')');
  $result = $select->fetchAllKeyed();

  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getContentStatusFilters', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getContentLanguageFilters() - Get the list of all languages currently used in content admin to show as narrow search filters
 */
function getContentLanguageFilters($langCode = 'cre_sys_lng_eng') {
  // Select the table
  try {
  $select = db_select('slt_content_master','cont_mast');

  // Left join the slt_profile_list_items to get language names
  $select->leftJoin('slt_profile_list_items', 'spli', 'cont_mast.lang_code = spli.code');

  // Select the fields to fetch
  $select->distinct();
  $select->addField('spli', 'code', 'code');
  $select->addField('spli', 'name', 'name');

  // Set the conditions
  $select->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  $select->condition(db_and()->condition('cont_mast.status', 'lrn_cnt_sts_del', '!='));
  $select->condition(db_and()->condition('spli.is_active', 'Y', '='));

  // Order results by name
  $select->orderBy('name');

  expDebug::dPrintDBAPI('$select', $select);

  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);

  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getContentLanguageFilters', $ex);
  	expertusErrorThrow($ex);
  }
}


function getContentTypeFilters($langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  // Select the table
  try {
  $select = db_select('slt_profile_list_items','spli');

  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');

  // Order results by name
  $select->orderBy('name');

  // Set the conditions
  $select->condition('spli.code', db_like('lrn_cnt_typ_') . '%', 'LIKE');
 // $select->condition('spli.code', db_like('lrn_cnt_typ_') . '%' . db_like('_') . '%', 'NOT LIKE');
  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  if (!$alsoFetchInactiveItems) {
    $select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }

  expDebug::dPrintDBAPI('$select', $select);

  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);

  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getContentTypeFilters', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * contentNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function contentNarrowSearchFiltersHTML() {
 try {
  $checkedStatus = array();
  $checkedManagedStatus = array();
  $allStatus = getContentStatusFilters();
  expDebug::dPrint('$allStatus = ' . print_r($allStatus, true), 4);

  $allLang = getContentLanguageFilters();
  expDebug::dPrint('$allLang = ' . print_r($allLang, true), 4);

  $allType = getContentTypeFilters();
  expDebug::dPrint('$allType = ' . print_r($allType, true), 4);

  $managedByOptions = getManagedByFilters();

  $renderfiltersets = array();
 // Comment the Type filter search box
  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'contentstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus,
                                                            	 'checked_list' => $checkedStatus)
                                                           );


  if(count($allLang) > 1) {
    $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                             array('code' => 'contentlang',
                                                                 'title' => t('LBL038'),
                                                             	 'divid' => 'language',
                                                                 'checkboxes_list' => $allLang)
                                                             );
  }

  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'contenttype',
                                                                 'title' => t('LBL036'),
                                                           		 'divid' => 'type',
                                                                 'checkboxes_list' => $allType)
                                                           );

  $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'contentmanage',
                                                           		 'html_id' => 'contentmanageby_container',
                                                                 'title' => t('Managed By'),
                                                           		 'divid' => 'managed_by',
                                                                 'checkboxes_list' => $managedByOptions)
                                                           );

  $weightedTags = getWeightedTags(array('Content'));
  if (!empty($weightedTags)) {
	  $renderfiltersets[]=theme('narrow_search_filterset_tags',
	  		array('code' => 'Contenttag',
	  				  'html_id' => 'catalogclasstag_container',
	  				  'title' => t('LBL191'), //Tags
	  				  'tags_for_cloud' => $weightedTags,
	  		)
	  );
  }

  $filterSetforJS = array();
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'contentstatus');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'contentlang');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'contenttype');
  $filterSetforJS[] = array('type' => 'checkbox', 'code' => 'contentmanage');
  if (!empty($weightedTags)) {
    $filterSetforJS[] = array("type"=> "tagscloud", "code" => "Contenttag");
  }

  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                     'filter_set_list'          => drupal_json_encode($filterSetforJS),
                    )
              );
  } catch (Exception $ex) {
  	watchdog_exception('contentNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * contentSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function contentSearchResultsSortbarHTML() {
  try {
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'content-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'content-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'content-sort-new');

  $defaultSortOrder = array('last_sort_type'=>'NewlyListed',
  					                   'last_sort_type_html_id'=>'content-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($defaultSortOrder), // Currently being ignored
                                              'title' => t('LBL011'),
                                              )
              );
  } catch (Exception $ex) {
  	watchdog_exception('contentSearchResultsSortbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * contentSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function contentSearchResultsActionbarHTML() {
  try {
  $actionbarLinksListforJS = array('export_path' => 'administration/manage/content/export/',
  					                       'print_path' => 'administration/manage/content/print/'
                                  );
  return theme('narrow_search_actionbar', array( 'add_button_title'   => t('LBL596'),
												                         'add_page_path'      => 'administration/manage/content/nojs/addedit',
												                         'show_print_button'  => true,
												                         'show_export_button' => true,
												                         'show_add_button'    => true,
                                        		     'ctools_style'       => 'ctools-modal-ctools-admin-content-addedit-style',
                                                 'actionbar_list'     => drupal_json_encode($actionbarLinksListforJS),
  																							 'page_code' => 'cre_sys_obt_cnt',
                                           )
               );
  } catch (Exception $ex) {
  	watchdog_exception('contentSearchResultsActionbarHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getContentAdminTabContentMainHTML - Returns the html for tab content main
 */
function getContentAdminTabContentMainHTML() {
	try {
   $narrowsearchTabContentListforJS = array(
                                            'search_base_path' =>'administration/manage/content/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/manage/content/textfilter-autocomplete'
                                          );

   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabContentListforJS))
                              );
   } catch (Exception $ex) {
   	watchdog_exception('getContentAdminTabContentMainHTML', $ex);
   	expertusErrorThrow($ex);
   }
}

/*
 * getSurveyDetailsTabContent		  - Returns the html for narrow search block on content search page
 */
function getContentAdminTabContent() {
	try {
  $tabContentTypeArr = getContentAdminTabContentType();

  $contentResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => contentNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => contentSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => contentSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getContentAdminTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','manage'),
  );
  echo drupal_json_encode($contentResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getContentAdminTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getContentTextFilterAutoComplete() - Returns the matched content codes
 */
function getContentTextFilterAutoComplete() {
	try {
  $searchText =  $_GET['z'];
  // Select the table
  $select = db_select('slt_content_master', 'cont_mast');
  $select->leftjoin('slt_tag_entity', 'te', "cont_mast.id=te.entity_id and te.entity_type='Content'");
  $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');

  // Add fields to fetch
  $select->addField('cont_mast','code', 'code');
 // $select->addField('td', 'tagname', 'tagname');
  $select->condition('cont_mast.status','lrn_cnt_sts_del', '!=');
 
  $select->condition('cont_mast.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'NOT IN');
  
 // selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array("code", "description"), "z");
 $tableAliasFields = array('cont_mast.code','cont_mast.description','td.tagname');
  SearchByFilterForMultipleTableORCondition($select,$searchText,'Search',$tableAliasFields);
  
  filterByUserAccessPrivileges($select,'cont_mast','',1,'cre_sys_obt_cnt','attach','cont_mast'); // #46933 - group access visiblity check
  //Groupby id to avoid duplicates #71622
  $select->groupBy('cont_mast.id');
  
  // Order in dictionary sort order
  $select->orderBy('code');
  expDebug::dPrintDBAPI('$select', $select);

  $matchingContentCodesList = $select->execute()->fetchAll();
  expDebug::dPrint('$matchingContentCodesList = ' . print_r($matchingContentCodesList, true) , 3);

  foreach ($matchingContentCodesList as $matchingContentCode) {
    print rawurlencode($matchingContentCode->code) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getContentTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getContentDetailsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getContentDetailsHTML($content) {
	try {
		expDebug::dPrint('$content$content'.print_r($content,true),4);
		$detailsList = array();
		if (!empty($content->type)) {
			$detailsList[] = array('detail' => t($content->type), 'tooltip' => t('LBL036').': '.$content->type);
		}
		if (!empty($content->lang)) {
			//$langchar = substr($content->lang, 0, 3); commented for new theme change
			$detailsList[] = array('detail' => t($content->lang), 'tooltip' => t('LBL038').': '.$content->lang);
		}
		if (!empty($content->lesson_cnt) && $content->content_type != 'lrn_cnt_typ_vod') {
			$lessonCount = t('LBL854').': '.$content->lesson_cnt;
			$detailsList[] = array('detail' => $lessonCount, 'tooltip' => t('LBL854').': '.$content->lesson_cnt);
		}

		$actionList[] = array('action_link_text' => t('LBL063'),
				'action_page_path' => 'administration/manage/content/nojs/addedit',
				'action_button_params' => core_encrypt($content->id),
				'ctools_style' => 'ctools-modal-ctools-admin-content-addedit-style',
				'row_id' => $content->row,
				'manageby' => $content->manageby,
				'js_object_info' => array (
						'name' => 'narrowsearch', // provide the java script object name
						'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
				),
				'tooltip' => t('LBL063')
		);

		$actionList[] = array('action_link_text' => t('LBL286'),
				'action_page_path' => '',
				'action_button_params' => core_encrypt($content->id),
				'action_button_param_text' => 'Content',
				'ctools_style' => 'ctools-modal-ctools-admin-content-addedit-style',
				'js_object_info' => array (
						'name' => 'narrowsearch', // provide the java script object name
						'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
				),
				'tooltip' => t('LBL286')
		);
		$viewContentlist[] = array( 'action_link_text' => 'Content',
				'action_page_path' => 'administration/manage/view-content/nojs/addedit/'.core_encrypt($content->id).'',
				'action_button_params' => core_encrypt($content->id),
				'action_button_param_text' => $content->code,
				'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
				'js_object_info' => array (
						'name' => 'narrowsearch', // provide the java script object name
						'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
				),
				'tooltip' => t('view')
		);

		$content->entityType = 'cre_sys_obt_cnt';
		return theme('narrow-search-results-item-details',
				array('title' => trim($content->code),
						'details_list' => $detailsList,
						'delete_info' => t('MSG357').' '.strtolower(t('Content')),
						'description'=> trim($content->description),
						'action_list'=> $actionList,
						'view_list'=> $viewContentlist,
						'entity_multi_action' => $content,
						'sumedit' => $content->sumedit,
						'sumdelete' => $content->sumdelete,
				)
		);

	} catch (Exception $ex) {
		watchdog_exception('getContentDetailsHTML', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getContentActionsHTML() - Renders content's actions as HTML to be displayed in jqGrid.
 */
function getContentActionsHTML($content) {
//	$content->object_type = 'Content';
	try {
  return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $content)
              );
  } catch (Exception $ex) {
  	watchdog_exception('getContentActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * contentSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function contentSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".code";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias . ".code";
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
  	watchdog_exception('contentSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendContentListToJqGrid() - Prepares JSON response to JqGrid's request for Content Admin
 */
function sendContentListToJqGrid() {
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

/*   $numContent = fetchContentAll('COUNT');

  expDebug::dPrint('$numContent = ' . print_r($numContent, true), 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numContent); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);


  $contentList = fetchContentAll('LISTS', $start, $limit);
  expDebug::dPrint('$contentList = ' . print_r($contentList, true), 4);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numContent;

  $response->initial_sort_type_html_id = 'content-sort-new';

  $i=0;
  $contentList1=$contentList[0];

  // Mananged By behavior related code
  $userData = new stdClass();
  $userData->managedBy = $contentList[1];
  $response->userdata = $userData;

  foreach ($contentList1 as $content) {
    expDebug::dPrint('$content = '. print_r($content, true), 4);

    $content->row =  $i;
    $content->object_type = 'Content';
    $content->object_type_code = 'cre_sys_obt_cnt';
    $response->rows[$i]['id']   = $content->id;

    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $content)),
                                         'details' => getContentDetailsHTML($content),
                                         'action' => getContentActionsHTML($content)
                                         );

    $i++;
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendContentListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * fetchContentAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchContentAll($op=NULL, $start=NULL, $limit=NULL,$callFrom='') {
	try {
  $searchText =  $_GET['textfilter'];
  $userId = getSltpersonUserId();
  // Select table
  $select = db_select('slt_content_master', 'cont_mast');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cont_mast.id AND group_map.entity_type = \'cre_sys_obt_cnt\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_cnt\''); */
  $select->leftJoin('slt_profile_list_items', 'pli_type', 'cont_mast.type = pli_type.code AND pli_type.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'pli_lang', 'cont_mast.lang_code = pli_lang.code AND pli_lang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'pli_status', 'cont_mast.status = pli_status.code AND pli_status.lang_code = \'cre_sys_lng_eng\' ');
  $select->leftJoin('slt_content_version', 'cont_vers', 'cont_mast.id = cont_vers.content_master_id');
  $select->leftjoin('slt_tag_entity', 'te', "cont_mast.id=te.entity_id and te.entity_type='Content'");
  $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
  $select->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');  
  $select->condition('cont_mast.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'NOT IN');
  
  if($op=='COUNT'){
     // Get count of active survey
     $select->addExpression('COUNT(DISTINCT(cont_mast.id))', 'COUNT');
  }
  else{
	  // Select fields/expression values to be fetched from the database.
  //	$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
  //	$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
		$select->addField('cont_mast', 'id', 'id');
		$select->addField('cont_mast', 'code', 'code');
		$select->addField('cont_mast', 'description', 'description');
		$select->addField('cont_mast', 'type', 'content_type');
		$select->addField('pli_type', 'name', 'type');
		$select->addField('pli_lang', 'name', 'lang');
		$select->addField('pli_status', 'name', 'status');
		$select->addField('pli_status', 'code', 'status_code');
		$select->addField('cont_vers', 'total_lesson', 'lesson_cnt');
		$select->addField('cont_vers', 'id', 'version_id');
		//$select->addField('td', 'tagname', 'tagname');
		$select->groupBy('cont_mast.id');
  }
  expDebug::dPrint('$_GET[Contenttag]'.$_GET['Contenttag'],5);
	 if(!empty($_GET['Contenttag'])){
  //	$select->leftjoin('slt_tag_entity', 'te', "cont_mast.id=te.entity_id and te.entity_type='Content'");
  //	$select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
  	$select->condition('td.tagname', rawurldecode($_GET['Contenttag']),'=');
  	$select->groupBy('cont_mast.id');
  }
  filterByUserAccessPrivileges($select, 'cont_mast','',1,'cre_sys_obt_cnt','','cont_mast');
  selectAddNarrowSearchINCondition($select, 'cont_mast', 'status', 'contentstatus');
  selectAddNarrowSearchINCondition($select, 'cont_mast', 'lang_code', 'contentlang');
  selectAddNarrowSearchINCondition($select, 'cont_mast', 'type', 'contenttype');
  //selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array("code", "description"));
   $tableAliasFields = array('cont_mast.code','cont_mast.description','td.tagname');
   SearchByFilterForMultipleTableORCondition($select,$searchText,'Search',$tableAliasFields);
  $select->condition('cont_vers.status', 'lrn_cnt_sts_atv','=');
  //$select->groupBy('cont_vers.content_master_id');

  // Check added by Vincent on Dec 2, 2013
  // Manage by fillter is not require for API calls
  $managefilter = '';
  if($callFrom != 'API'){
	  $managefilter = selectAddNarrowSearchManageByCondition('content', $select, 'cont_mast', 'contentmanage');
  }
  expDebug::dPrint('$managefilter = ' . $managefilter, 4);

  if($op != "COUNT"){
    // Order the records
    contentSearchSelectStmtAddOrderByClause($select, 'cont_mast');
  }

  expDebug::dPrintDBAPI('$select $select before count', $select);

  if($op == "COUNT"){
  	$newResult = $select->execute()->fetchField();
	  expDebug::dPrint('COUNT $result = ' . $newResult ,3);
	return $newResult;
  }
  elseif($op == "CSV"){
	  $colHeadersMap = array(
	    t('LBL107') => 'code',
			t('LBL036') => 'type',
      t('LBL038') => 'lang',
      t('LBL102') => 'status',
      t('LBL854') => 'lesson_cnt'
    );
	  $filterArray[] = array(t('Content').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('contentstatus'));
    /*$filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('contentlang'));*/
    $filterArray[] = array(t('LBL191').':'.textboxFiltersetValueToPrintableStr('Contenttag'));
	  $filterArray[] = array(t('LBL036').':'.checkboxFiltersetSelectionsToPrintableStr('contenttype'));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
    $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('contentmanage'));
  	SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'content_admin_search');
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL107') => array('code',     40),
    					    t('LBL036') => array('type',     18),
                  t('LBL038') => array('lang',     15),
    					    t('LBL102') => array('status',   17),
    					    t('LBL854') => array('lesson_cnt',10)
    				);

    $appliedFilters = array(t('Content') => textboxFiltersetValueToPrintableStr('textfilter'),
    		t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('contentstatus'),
    		/*t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('contentlang'),*/
    		t('LBL191') => textboxFiltersetValueToPrintableStr('Contenttag'),
    		t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('contenttype'),
    		ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    		t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('contentmanage')
    );

    SendNarrowSearchResultsAsPDFFile($select, t('Content'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'content_admin_search');
  }
  else {
  	
  		add_row_id_condition($select, 'cont_mast');
	     // Limit to $limit records
	     $select->range($start, $limit);
	     expDebug::dPrintDBAPI('$select with range set', $select);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    $newResult[0]=$result;
	    $newResult[1]=$managefilter;
	    expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);
	    return $newResult;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchContentAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function content_admin_search_csv_row_data_alter($rowData) {
  try {
    if ($rowData[0]->content_type == 'lrn_cnt_typ_vod') {
      $rowData[0]->lesson_cnt = '';
    }
  }
  catch (Exception $ex) {
    watchdog_exception('content_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function content_admin_search_pdf_row_data_alter($rowData) {
  try {
    if ($rowData[0]->content_type == 'lrn_cnt_typ_vod') {
      $rowData[0]->lesson_cnt = '';
    }
  }
  catch (Exception $ex) {
    watchdog_exception('content_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the content
 */
function publishOrUnpublishContent($contentId) {
	try {
  $currUserId = getSltpersonUserId();
  $status  = getContentStatus($contentId);

  $txn = db_transaction();
  try {
    $updateStmt = db_update('slt_content_master');
    $updatedstatus  = ($status=='lrn_cnt_sts_atv')? "lrn_cnt_sts_itv" : "lrn_cnt_sts_atv";

    $updateStmt->condition('id', $contentId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
	 /* Update tag activity Count */
	updateTagActivityCount($contentId,'Content',$updatedstatus);	
	/* Update tag activity Count */
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('change content status', $ex);
    throw $ex;
  }
  unset($txn); // Commit transaction

  add_audit_trail_entry($currUserId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'publishOrUnpublishContent', 'Changed Content Status',
                                         null, $status, $updatedstatus);

  $returnStatus  = ($updatedstatus=='lrn_cnt_sts_atv')? t('LBL571') : t('LBL570');
  print $updatedstatus . '|' . $returnStatus;
  } catch (Exception $ex) {
  	watchdog_exception('publishOrUnpublishContent', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * fucntion to retrive the current status of the content
 */
function getContentStatus($contentId){
	try {
  $select = db_select('slt_content_master', 'cont_mast');
  $select->addField('cont_mast', 'status', 'status');
  $select->condition('cont_mast.id', $contentId);
  expDebug::dPrintDBAPI('$select', $select);
  // Execute the query
  $contentStatus = $select->execute()->fetchField();
  expDebug::dPrint('$contentStatus = ' . $contentStatus , 3);
  return $contentStatus;
  } catch (Exception $ex) {
  	watchdog_exception('getContentStatus', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fucntion to retrive the details of content
 */
function getAllContentDetails($contentId){
	try {
  $select = db_select('slt_content_master', 'content');
  $select->innerjoin('slt_content_version', 'ver', 'ver.content_master_id = content.id');
  $select->addField('content','id','id');
  $select->addField('content','code','title');
  $select->addField('content','type','content_type');
  $select->addField('content','description','short_description');
  $select->addField('content','lang_code','lang_code');
  $select->addField('content','status','status');
  $select->addField('content','conversion_status','conversion_status');
  $select->addField('ver','hosted_type','hosted_type');
  $select->addExpression('(select name from slt_profile_list_items where code = content.lang_code)','language_name');
  $select->condition('content.id', $contentId);
  $select->range(0,1);
  $result = $select->execute();
  return $result->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getAllContentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * addNewContenteDetails() - New Content Entry
 */
function addNewContenteDetails($form, &$form_state, $createUserId, $callFromAPI =0){
  $txn = db_transaction();
  expDebug::dPrint('$form_state values  = addNewContenteDetails' . print_r($form_state['values'], true), 4);
  try {
    $insertStmt  	= db_insert('slt_content_master');
    $nfields       	= contentFormatInputString($form, $form_state,$createUserId);
    $config=getConfig("exp_sp");
    $convert_pdf_on_background		= $config['convert_pdf_on_background'];
    $mylearn_version 				= $config['mylearn_version'];
    $content_conversion				= $config['content_conversion'];
    $pdf_conversion_status_arr = explode('~~~',$form_state['values']['pdf_conversion_status']);
    if($mylearn_version && $content_conversion && ($convert_pdf_on_background == 1 || $pdf_conversion_status_arr[0]=='0') && $form_state['values']['content_type'] =='lrn_cnt_typ_knc'){
    	$nfields['conversion_status'] = 'lrn_cnt_sts_pen';
    	$nfields['conversion_details'] = serialize($pdf_conversion_status_arr);
    	$nfields['custom4'] = $pdf_conversion_status_arr[1];
    	$nfields['custom3'] = $pdf_conversion_status_arr[2];
    	drupal_set_message(t('MSG799'));
    }
    $insertStmt->fields($nfields);
    expDebug::dPrintDBAPI('insertStmt', $insertStmt);
    $contentId 		= $insertStmt->execute();
    updateGroupMappingDetails($contentId, 'cre_sys_obt_cnt', getIdOfLoggedInUser(),'emptyadd',0);
    expDebug::dPrint('contentId = ' . $contentId);
    
//    $form_state['values']['save'] = strtolower($form_state['values']['save']);
 //   expDebug::dPrint("lower case value" .print_r($form_state['values']['save'],true),5);
    
    
    $insertfields = $form_state['values'];
    expDebug::dPrint('insertfields = ' . print_r($insertfields,true),5);
    $insertfields[description] = $form_state['values']['short_description']['value'];
    $insertfields[code] = $form_state['values']['content_data_url'];
    
    include_once(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info.inc');
    $cdn_status  = getDrupalModuleStatus(array('cloud_files'));
	/* Update tag activity Count */
	updateTagActivityCount($contentId,'Content',$form_state['values']['status']);	
	/* Update tag activity Count */
    if($cdn_status == 1){
    	add_audit_trail_entry(getIdOfLoggedInUser(), $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'getUploadContentData', 'Content Uploaded In CDN');
    	make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$contentId, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_content','addNewContenteDetails',
    			array('code','content_type', 'lang_code', 'description'));
    }else {
    	make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null ,$contentId, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_content','addNewContenteDetails',
    			array('code','content_type', 'lang_code', 'description'));
    }

    // Save the new content's id in the hidden id form field.
    $form_state['values']['id']     = $contentId;
  //  expDebug::dPrint('insertfields update master= ' . print_r($form_state['values'],true),5);
    updateContentMasterTitle($form_state);
    /* added extra param mode to differentiate when
		 the insert is happening - whetherthe version is the first
    			or the second - inorder to update
					version number in content version table */
  //  expDebug::dPrint('insertfields insert master = ' . print_r($form_state['values'],true),5);
    insertMasterContent($form_state,$createUserId,"AddContent");
    return array((object)array('id'=>$contentId));
  } catch (Exception $ex) {
    $txn->rollback();
    expDebug::dPrint("Error in addNewContenteDetails() exp_sp_administrion_content.inc ".print_r($ex,true));
    expDebug::dPrint("Error in addNewContenteDetails() exp_sp_administrion_content.inc ".$ex->__toString());
    watchdog_exception('AddContentAdmin', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Content type Invalid";
    	return $errobj;

    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);
}

function updateContentMasterTitle($form_state){
	try {
  expDebug::dPrint('$form_state[content_storage] = ' . print_r($form_state['content_storage'], true), 4);
  // getting hosted type -- issue fix for 0084046
  $hostedtype = $form_state['content_storage'][0]->hostedtype ;
  expDebug::dPrint('key values ' . print_r($hostedtype, true), 4);
  
  $i = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
  //$title = ($form_state['content_storage'][$i]->master_title) ? $form_state['content_storage'][$i]->master_title : $form_state['content_storage'][$i]->title;
  $title = empty($form_state['content_storage'][$i]->orig_file_name)? $form_state['content_storage'][$i]->title : $form_state['content_storage'][$i]->orig_file_name;
  $title = empty($title)? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
  $exists = addEditContentValidate($form_state,$form_state['values']['id'],"Add",$title);
  $title = ($exists == "exists") ? ($title.'-'.$form_state['values']['id']) : $title;
  
  $contnttype = $form_state['values']['content_type'];
  // checking if its a video and not a url and changing extension to lowercase -- issue fix for 0084046
  if($contnttype == 'lrn_cnt_typ_vod' && $hostedtype != 2){
        $file_array = explode(".", $title);
        end($file_array);
        $key = key($file_array);
        expDebug::dPrint("key" . print_r($key, true), 5);
        $file_array[$key] = strtolower($file_array[$key]);
        expDebug::dPrint("filename check" . print_r($file_array, true), 5);
        $title = implode(".",$file_array);
  }
  expDebug::dPrint('UTF8 title = ' . $title, 4);
  $updateStmt = db_update('slt_content_master');
  $updateStmt->fields(array('code'  => $title));
  $updateStmt->condition('id', $form_state['values']['id']);
  expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
  $updateStmt->execute();
  } catch (Exception $ex) {
  	watchdog_exception('updateContentMasterTitle', $ex);
  	expertusErrorThrow($ex);
  }
}


function updateContentDetails($form, &$form_state, $updateUserId, $callFromAPI =0) {
  expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
  $txn = db_transaction();

  try {

  	$defaultQryStr = 'SELECT description, type as content_type, status from slt_content_master where id = :cid';
  	$defaultQryStrArgs = array(':cid' => $form_state['values']['id']);
  	$defaultQry = db_query($defaultQryStr, $defaultQryStrArgs);
  	expDebug::dPrintDBAPI('$defaultQry', $defaultQry, $defaultQryStrArgs);
  	$default = $defaultQry->fetchAll();
  	expDebug::dPrint('$default = ' . print_r($default, true),4 );

  	$updateStmt = db_update('slt_content_master');
    $fields     = contentFormatInputString($form, $form_state, $updateUserId);
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['id']);
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);

    // Execute the update statement.
    $numUpdated = $updateStmt->execute();
	 /* Update tag activity Count */
	 $triggerElement = $form_state['storage']['submit_triggering_element'];
	updateTagActivityCount($form_state['values']['id'],'Content',$triggerElement);	
	/* Update tag activity Count */
    /* during edit , only if the file upload happens or a url is added ,
    	then insert a new record into content version */
    if($form_state['content_storage']){
    	insertMasterContent($form_state,$updateUserId,"EditContent");
    }
    $trigger_element = $form_state['storage']['submit_triggering_element'];
    $updatefields = $form_state['values'];
    $updatefields[description] = $form_state['values']['short_description']['value'];
    
    make_audit_trail_entries($updatefields, $updateUserId , $default ,$form_state['values']['id'], 'Updated','cre_sys_obt_cnt','exp_sp_administration_content','updateContentDetails',
                                          array('content_type','description'));

     $content_title = trim($form_state['values']['content_title1']);
    if($content_title!="" && !empty($content_title)){
     updateContentLessonTitle($form_state);
    }

    if($callFromAPI){
      return array((object)array('id'=>$form_state['values']['id']));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateContentDetails', $ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failiure'));
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Content Details Invalid";
    	return $errobj;
    }
    throw $ex;
  }

  // Commit the transaction
  unset($txn);

}

function updateContentLessonTitle($form_state){
	try {
  $title = $form_state['values']['content_title1'];
  $title = empty($title)? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
  expDebug::dPrint('UTF-8 $title = ' . $title, 4);
  $updateStmt = db_update('slt_content_master');
  $updateStmt->fields(array('code'  => $title));
  $updateStmt->condition('id', $form_state['values']['id']);
  expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
  $updateStmt->execute();

  $status = getContentStatus($form_state['values']['id']);
  expDebug::dPrint('$status = ' . $status, 4);

  /* Ranjani -- commented out to not edit title in version */
  /*$updateVersionStmt = db_update('slt_content_version');
  $updateVersionStmt->fields(array(
  					'title'  => $title,
            'status' => $status));
  $updateVersionStmt->condition('id', $form_state['values']['id']);
  expDebug::dPrintDBAPI('$updateVersionStmt', $updateVersionStmt);
  $updateVersionStmt->execute();*/
  } catch (Exception $ex) {
  	watchdog_exception('updateContentLessonTitle', $ex);
  	expertusErrorThrow($ex);
  }
}

function contentFormatInputString($form, &$form_state,$createUserId) {
	try {
   $nullVal           = NULL;
   $trigger_element = $form_state['storage']['submit_triggering_element'];
   $content_status = '';
   if($trigger_element == 'saveandpublish'){
     $content_status = 'lrn_cnt_sts_atv';
   }
   else if($trigger_element == 'save'){
     $content_status = 'lrn_cnt_sts_itv';
   }
   else if($trigger_element == 'unpublish'){
   	$content_status = 'lrn_cnt_sts_itv';
   }

   else if($form_state['values']['status']){
	 $content_status =$form_state['values']['status'];
   }
   $fields = array(
                //'code'               => '',
                'type'     	         => $form_state['values']['content_type'],
                'lang_code'          => $form_state['values']['lang_code'],
                'description'        => $form_state['values']['short_description']['value'],
                'status'             => $content_status,
                'active'			       => 'Y',
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
     $fields['code'] = '';
   }

   if($trigger_element == 'unpublish'){
     $content_status = 'lrn_cnt_sts_itv';
     $fields = array( 'status'  => $content_status );
   }
   expDebug::dPrint('$fields = ' . print_r($fields,true), 4);
   return $fields;
   } catch (Exception $ex) {
   	watchdog_exception('contentFormatInputString', $ex);
   	expertusErrorThrow($ex);
   }
}

function insertMasterContent($form_state,$createUserId,$mode){
	try {
   expDebug::dPrint("insertMasterContent -- >" . print_r($form_state['content_storage'],true),4);
   // getting hosted type -- issue fix for 0084046
   $hostedtype = $form_state['content_storage'][0]->hostedtype ;
   expDebug::dPrint('key values ' . print_r($hostedtype, true), 4);
   
   $totalContent = count($form_state['content_storage']);
   $status = ($mode == "AddContent") ? 'lrn_cnt_sts_atv' : 'lrn_cnt_sts_itv';//getContentStatus($form_state['values']['id']);
   $nullVal = NULL;
   $contentDataIdx = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic')? 1 : 0;
   if (empty($form_state['content_storage'][$contentDataIdx]->orig_file_name)) {
	   // Added by Vincent on May 10, 2013 for Scorm and AICC version title should be the uploaded file name instead of lesson title #0023551
	   if ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic' ||
	           $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_s12' ||
	             $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_s24' || $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_tnc') {
		   $tname = ($form_state['content_storage'][0]->file_name) ? $form_state['content_storage'][0]->file_name : $form_state['content_storage'][1]->file_name;
			 $title1 = explode('_',$tname);
			 $title = $title1[0];
			 for ($i=1; $i < sizeOf($title1) - 1; $i++) {
			   $title = $title . '_' . $title1[$i];
			 }
			 $title = $title . '.zip';
	   }
	   else {
	   	 $title = ($form_state['content_storage'][0]->title)? $form_state['content_storage'][0]->title : $form_state['content_storage'][1]->title;
	   }
   }
   else {
   	 $title = $form_state['content_storage'][$contentDataIdx]->orig_file_name;
   }

   $title = empty($title)? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
   expDebug::dPrint("insertMasterContent title-- >" . print_r($title,true),4);
   
   $contnttype = $form_state['values']['content_type'];
   expDebug::dPrint("contnttype check" . print_r($contnttype, true), 5);
   // checking if its a video and not a url and changing extension to lowercase -- issue fix for 0084046
   if($contnttype == 'lrn_cnt_typ_vod' && $hostedtype != 2){
       $file_array = explode(".", $title);
       end($file_array);
       $key = key($file_array);
       $file_array[$key] = strtolower($file_array[$key]);
       expDebug::dPrint("filename check" . print_r($file_array, true), 5);
       $title = implode(".",$file_array);
   }
   expDebug::dPrint("insertMasterContent title version-- >" . print_r($title,true),4);
   
   $insertContentVersion  	= db_insert('slt_content_version');
   $sizePath = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
   $maxVersion = db_select('slt_content_version', 'version');
   $maxVersion->addExpression('max(version)','max_version');
   $maxVersion->condition('version.content_master_id', $form_state['values']['id']);
   $existingVersion = $maxVersion->execute()->fetchField();
   expDebug::dPrintDBAPI(' max version query --------> '.$maxVersion);

   $fields = array(
            'content_master_id'     => $form_state['values']['id'],
            'content_sub_type'      => $form_state['values']['content_type'],
            'title'                 => $title,
            'hosted_type'           => empty($form_state['content_storage'][0]->hostedtype) ? 1 : $form_state['content_storage'][0]->hostedtype,
            'total_lesson'          => $totalContent,
            'version'			    => ($existingVersion + 1),
            'overall_mastery_score' => empty($form_state['content_storage'][0]->masteryscore) ? 0 : $form_state['content_storage'][0]->masteryscore,
            'status'                => $status,
   			'published_by'          => ($mode == "AddContent") ? $createUserId : null,
            'published_on'          => ($mode == "AddContent") ? now() : null,
   			'file_size'			    => $form_state['content_storage'][$sizePath]->file_size,
   			'file_path'			    => $form_state['content_storage'][$sizePath]->file_path,
            'created_by'            => $createUserId,
            'created_on'            => now(),
   			'updated_by'            => $createUserId,
   			'updated_on'            => now(),
            'custom0'               => $nullVal,
            'custom1'               => $nullVal,
            'custom2'               => $nullVal,
            'custom3'               => $nullVal,
            'custom4'               => $nullVal,
          );
   $insertContentVersion->fields($fields);
   expDebug::dPrintDBAPI('$insertContentVersion', $insertContentVersion);
   $contentVersionId 	= $insertContentVersion->execute();
   make_audit_trail_entries(array('version' => ($existingVersion + 1) ), getIdOfLoggedInUser(),$existingVersion ,$form_state['values']['id'], 'Inserted',$form_state['values']['content_type'],'exp_sp_administration_content','insertMasterContent',
   		array('version'));
   $i = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
   $totalContent = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? ($totalContent + 1) : $totalContent;
   //insert into slt_content_lesson
 
   for($i; $i < $totalContent; $i++){
     $insertContentLesson  	= db_insert('slt_content_lesson');
     $title = $form_state['content_storage'][$i]->title;
     $title = empty($title)? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
    
     expDebug::dPrint("title check" . print_r($title, true), 5);
     
     // getting hosted type -- issue fix for 0084046
     $hostedtype = $form_state['content_storage'][0]->hostedtype ;
     expDebug::dPrint('key values ' . print_r($hostedtype, true), 4);
     $contnttype = $form_state['values']['content_type'];
     expDebug::dPrint("contnttype check 2" . print_r($contnttype, true), 5);
     // checking if its a video and not a url and changing extension to lowercase -- issue fix for 0084046
     if($contnttype == 'lrn_cnt_typ_vod' && $hostedtype != 2){
         $file_array = explode(".", $title);
         end($file_array);
         $key = key($file_array);
         $file_array[$key] = strtolower($file_array[$key]);
         expDebug::dPrint("filename check" . print_r($file_array, true), 5);
         $title = implode(".",$file_array);
     }
     expDebug::dPrint("title check" . print_r($title, true), 5);
      
     $fields= array(
            'content_version_id' => $contentVersionId,
            'title'              => $title,
            'launchurl'          => empty($form_state['content_storage'][$i]->launchurl) ? '' : $form_state['content_storage'][$i]->launchurl,
            'masteryscore'       => empty($form_state['content_storage'][$i]->masteryscore) ? 0 : $form_state['content_storage'][$i]->masteryscore,
            'datafromlms'		 => empty($form_state['content_storage'][$i]->datafromlms) ? '' : $form_state['content_storage'][$i]->datafromlms,
            'created_by'         => $createUserId,
            'created_on'         => now(),
     		'updated_by'         => $createUserId,
     		'updated_on'         => now(),
            'custom0'            => $nullVal,
            'custom1'            => $nullVal,
            'custom2'            => $nullVal,
            'custom3'            => $nullVal,
            'custom4'            => $nullVal,
          );
     $insertContentLesson->fields($fields);
     expDebug::dPrintDBAPI('$insertContentLesson', $insertContentLesson);
     $insertContentLesson->execute();
   }
   } catch (Exception $ex) {
   	watchdog_exception('insertMasterContent', $ex);
   	expertusErrorThrow($ex);
   }
}

function getUploadContentData($form, &$form_state,$sourceField,$verId){
	try {
  include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
  include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_scorm_parser.inc');
  include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_tincanscorm_parser.inc');
  expDebug::dPrint("getUploadContentData called...");
  $isZipFile = false;
  if($form_state['values']['content_type']=='lrn_cnt_typ_knc'){
	  $validators  = array(
	    'file_validate_extensions' => array('txt doc docx ppt pptx pps ppsx xls xlsx pdf png jpg jpeg gif')//Ticket#0029042 added two file extensions(pps,ppsx)
	  );
  }else if($form_state['values']['content_type']=='lrn_cnt_typ_vod'){
  	$validators  = array(
	    'file_validate_extensions' => array('mp4')
	  );
  }else{
  	$validators  = array(
	    'file_validate_extensions' => array('zip')
	  );
	  $isZipFile = true;
  }
  expDebug::dPrint("File formats -- ".print_r($validators,true));
  $contentData = file_save_upload($sourceField,$validators);
 // expDebug::dPrint('$contentData returned by file_save_upload() = ' . print_r($contentData, true), 4);
  $flDir='';
  if(!empty($contentData)) {
    if (!$contentData->status) { // i.e. if picture is a temporary file
      // Prepare the pictures directory.
      $contentData_directory = file_default_scheme() . '://' . variable_get('content_upload_path', 'contentupload');
      if(!$isZipFile){
      	$fn = explode('.',$contentData->filename);
      	$fname = $fn[0];
      	if(sizeOf($fn)>2){
      		for($i=1;$i<sizeOf($fn)-1;$i++){
      			$fname = $fname . "." . $fn[$i];
      		}
      	}
      	$flDir = $fname."_".$verId;
      	$contentData_directory = $contentData_directory . '/' . $flDir;
      }
      $uploadDirectory = $contentData_directory;
     // expDebug::dPrint('$contentData_directory = ' . print_r($contentData_directory, true), 4);
      file_prepare_directory($contentData_directory, FILE_CREATE_DIRECTORY);
      //drupal_chmod($contentData_directory, 0777);
      // Prepare the unique file name to place the picture under the $contentData_directory.
      $info = image_get_info($contentData->uri);
      //expDebug::dPrint('$info returned by image_get_info = ' . print_r($info, true), 4);
      //$destination = file_stream_wrapper_uri_normalize($contentData_directory . '/picture-' . $drupalUid . '-' . REQUEST_TIME . '.' . $info['extension']);
      $destination = $contentData_directory;
      expDebug::dPrint('$destination = ' . print_r($destination, true),4);
      expDebug::dPrint('$contentData returned by file_move() before move= ' . print_r($contentData, true),4);

      // Move the temporary file into the final location.
      if ($contentData = file_move($contentData, $destination, FILE_EXISTS_REPLACE)) {
        expDebug::dPrint('$contentData returned by file_move() = ' . print_r($contentData, true), 4);

        // Update file status from temporary to permanent so that cron does not clean it out
        $contentData->status = FILE_STATUS_PERMANENT;
        $contentData = file_save($contentData);
        expDebug::dPrint('$contentData returned by file_save() = ' . print_r($contentData, true), 4);
        $uploadFile = $contentData->uri;
  	    $fileNameFind = explode('/',$contentData->uri);
  	    $uploadedFile = $fileNameFind[(count($fileNameFind)-1)];
  	    $fileName = basename($uploadedFile,".zip");
        // Increment file usage count in file_usage table
        //file_usage_add($contentData, 'user', 'user', $drupalUid);
      }
      $config=getConfig("exp_sp");
      $mylearn_version = $config['mylearn_version'];
      $content_conversion = $config['content_conversion'];      
      $convert_pdf_on_background = $config['convert_pdf_on_background'];
      expDebug::dPrint('$$convert_pdfswf_on_background value = ' . print_r($convert_pdf_on_background, true), 4);
      expDebug::dPrint('$$$mylearn_version value = ' . print_r($mylearn_version, true), 4);
      if($form_state['values']['content_type']=='lrn_cnt_typ_knc' && $mylearn_version && $content_conversion && $convert_pdf_on_background == 0){
      		file_convertion_logic($contentData,0,$form, $form_state);
    }
  }
  }

  expDebug::dPrint("uploadFile >>> ".$uploadFile , 4);
  expDebug::dPrint("fileName >>> $fileName" , 4);
  //include_once(drupal_get_path('module', 'system') .'/system.archiver.inc');
  $uploadDirectory = './sites/default/files/contentupload';
  $uploadFile = $uploadDirectory.'/'.$uploadedFile;
  $getFoldername  ="contentupload/";
  expDebug::dPrint("fileName1 >>> $fileName" , 4);
  if($isZipFile){
  	  expDebug::dPrint("OS details ---------->".print_r(PHP_OS,1) , 4);
	  $zip = new ZipArchive;
	  if ($zip->open($uploadFile) === TRUE) {
	  	expDebug::dPrint("fileName with directory >>> $contentData_directory.'/'.$fileName" , 4);
	  	if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
	  		$zip->extractTo($contentData_directory.'/'.$fileName);
	     	$zip->close();
	  	}else {
	  		$config=getConfig("exp_sp");
	      	$cmd = "unzip ".$config['content_upload_path'].'/'.variable_get('content_upload_path', 'contentupload').'/'.$fileName.' -d '.$config['content_upload_path'].'/'.variable_get('content_upload_path', 'contentupload').'/'.$fileName;
	      	expDebug::dPrint("fileName with directory >>>".$cmd , 4);
	      	system($cmd);
	  	}
	  	expDebug::dPrint("extractFilePath start ok", 4);
	  } else {
	     expDebug::dPrint("extractFilePath start failed", 4);
	  }
	  $setFileExit = $uploadDirectory.'/'.$fileName.'';
  }else{
  	$setFileExit = $uploadDirectory.'/'.$flDir.'';
  }
  expDebug::dPrint("fileName2 >>> $fileName" , 4);
  //expDebug::dPrint("Uploded File Path >>> ".$setFileExit , 4);

  $hasSubDirectory = getTotalSubDir($setFileExit);
  expDebug::dPrint("fileName3 >>> $fileName" , 4);
  //expDebug::dPrint("Total Sub Directory >>> ".$hasSubDirectory , 4);

  $contentType = $form_state['values']['content_type'];

  if($contentType == "lrn_cnt_typ_vod"){
  	 $setNewDirectory = $getFoldername.$flDir;
  	 $conversion_needed = getConfigValue("convert_video_for_mobile_access");
  	 $conversion_background = getConfigValue("convert_video_on_background");
 	 $ffmpeg_home= getConfigValue("ffmpeg_home").'/bin';
  	 //expDebug::dPrint("conversion_needed >>> $conversion_needed" , 4);
  	 expDebug::dPrint("hello1:".$conversion_needed , 4);
  	 if(isset($conversion_needed) && $conversion_needed == "1" && $conversion_background == "0" )
  	 {
  	   convertVideoForMobileAccess($setNewDirectory,$ffmpeg_home);
  	 }
  	 $setNewDirectory = $setNewDirectory."/".$fileName;
  	 expDebug::dPrint("hello2 ok:".$conversion_needed , 4);
  }else{
  	if($isZipFile)
  		$setNewDirectory = $getFoldername.$fileName;
  	else
  		$setNewDirectory = $getFoldername.$flDir;
  }

  $realPath='';

  expDebug::dPrint("setNewDirectory2 >>> $setNewDirectory $fileName $setFileExit" , 4);

  $gutil = new GlobalUtil();
  $geturlpath = $gutil->getConfig();
  $pathFromini = $geturlpath["content_upload_path"];
  $urlpath  = $pathFromini.'/'.$setNewDirectory;
  $localStorePath = $urlpath ;
  /*$val = $_REQUEST['contentvalue'];
  $aval = explode(',',$val);
  $vRowIndex = $aval[0];
  $isKnowledgeContent =$aval[1];
  $contentType = $aval[2];*/

  expDebug::dPrint("urlpath >>> ".$urlpath , 4);

  /*video streaming supporting - copy a file to red5 server streams folder -start here */
  if($contentType == "lrn_cnt_typ_vod"){
  	$site_name = variable_get('site_name');
  	$source_path =  $urlpath;
  	$config=getConfig("exp_sp");

  	//getting configuration things start here.

  	//$red5_server_on = $config["red5_server_on"];
  	$red5_server_path = $config["red5_server_path"];

//   	$content_server_ip = $config["content_server_ip"];
//   	$content_server_path = $config["content_server_path"];
//   	$content_server_username = $config["content_server_username"];
//   	$content_server_password = $config["content_server_password"];
  	//getting configuration things end here.

  	include_once(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info.inc');
  	$module_name = array('exp_sp_stream_server');
  	$stream_server_status  = getDrupalModuleStatus($module_name);
  	expDebug::dPrint('$stream_server_status = ' .print_r($stream_server_status,true), 4);

  	if($stream_server_status == 1) {
  		$stream_server  = true;
  	} else {
  		$stream_server  = false;
  	}
  	//if streaming_server is in off status we need to convert in a single format for mobile purbose
  	
  	if(($conversion_background == "1") && ($conversion_needed == "1")) {
  		convertVideoForRed5Server($source_path,$folder_path,$fileName,$ffmpeg_home,$flDir,$stream_server);
  		copy($source_path, $desination_path);
  	}
  }

  /*video streaming support -end here */


  $inputValues = array();
  //$contentType="AICC";
  if($contentType != "lrn_cnt_typ_aic"){
  	if($contentType == 'lrn_cnt_typ_srm_s12' || $contentType == 'lrn_cnt_typ_srm_s24')
  	{
  		expDebug::dPrint("setFileExit >>> ".file_exists($setFileExit.'/imsmanifest.xml') , 4);
  		if(file_exists($setFileExit.'/imsmanifest.xml'))
  		{
  			$realPath = $urlpath.'/imsmanifest.xml';
  		}
  		else
  		{
  			$isSubDirectory = get_dirs($setFileExit,'scorm');
  			if(empty($isSubDirectory)){
  			  return  '';
  			}
  			$realPath = $urlpath.'/'.$isSubDirectory.'/imsmanifest.xml';
  		}
  		expDebug::dPrint('$realPath '. $realPath );
  		$inObj = new stdClass();
  		$inObj->ContentURLPath = $realPath;
  		$inObj->basePath = $pathFromini;

  		$inputValues = ContentMasterScormValidate($inObj);
  		if(!empty($inputValues[0]->version)){
  			if(($contentType == 'lrn_cnt_typ_srm_s12' && $inputValues[0]->version!=1.2 && empty($inputValues[0]->scorm)) || ($contentType == 'lrn_cnt_typ_srm_s24' && $inputValues[0]->version==1.2 && empty($inputValues[0]->scorm2004))  ){
  				form_set_error('', t('LBL613'));
  			}
  		}
  		elseif(!empty($inputValues[0]->new_format_version)){
  			if(($contentType == 'lrn_cnt_typ_srm_s12' && $inputValues[0]->new_format_version!="ADL SCORM 1.2" && empty($inputValues[0]->scorm) ) || ($contentType == 'lrn_cnt_typ_srm_s24' && $inputValues[0]->new_format_version=="ADL SCORM 1.2" && empty($inputValues[0]->scorm2004))  ){
  			form_set_error('', t('LBL613'));
  		}
  	}
  	}
  	elseif($contentType == 'lrn_cnt_typ_srm_tnc'){
  		expDebug::dPrint("setFileExit >>> ".file_exists($setFileExit.'/tincan.xml') , 4);
  		if(file_exists($setFileExit.'/tincan.xml'))
  		{
  			$realPath = $urlpath.'/tincan.xml';
  		}
  		else
  		{
  			$isSubDirectory = get_dirs($setFileExit,'tincan');
  			if(empty($isSubDirectory)){
  				return  '';
  			}
  			$realPath = $urlpath.'/'.$isSubDirectory.'/tincan.xml';
  		}
  		expDebug::dPrint('$realPath '. $realPath );
  		$inObj = new stdClass();
  		$inObj->ContentURLPath = $realPath;

  		$inputValues = ContentMasterTinCanValidate($inObj);
  	}else{
  	  $inputValues = ContentMasterKnowledgeContentValidate($urlpath,$fileName,$contentType,$setFileExit);
  	}
  }else{
  	include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_aicc_parser.inc');

  	$files = glob("$setFileExit/{*.AU,*.au}",GLOB_BRACE);
  	$aubasename;
  	if(sizeOf($files)>0){
      $aubasename = basename($files[0]);
      $realPath = $setFileExit;
      $realPath_au = $urlpath.'/'.basename($files[0]);
      $files1 = glob("$setFileExit/{*.CRS,*.crs}",GLOB_BRACE);
      $realPath_crs = $urlpath.'/'.basename($files1[0]);
      $files2 = glob("$setFileExit/{*.CST,*.cst}",GLOB_BRACE);
      $realPath_cst = $urlpath.'/'.basename($files2[0]);
      $files3 = glob("$setFileExit/{*.DES,*.des}",GLOB_BRACE);
      $realPath_des = $urlpath.'/'.basename($files3[0]);

  	}else{
      $isSubDirectory = get_dirs_aicc($setFileExit);
      $realPath = $setFileExit.'/'.$isSubDirectory;
      expDebug::dPrint("Real Path ".$realPath , 4);
      $urlpath = $urlpath.'/'.$isSubDirectory;
      $files = glob("$realPath/{*.AU,*.au}",GLOB_BRACE);
      $aubasename = basename($files[0]);
      $realPath_au = $urlpath.'/'.basename($files[0]);
      $files1 = glob("$realPath/{*.CRS,*.crs}",GLOB_BRACE);
      $realPath_crs = $urlpath.'/'.basename($files1[0]);
      $files2 = glob("$realPath/{*.CST,*.cst}",GLOB_BRACE);
      $realPath_cst = $urlpath.'/'.basename($files2[0]);
      $files3 = glob("$realPath/{*.DES,*.des}",GLOB_BRACE);
      $realPath_des = $urlpath.'/'.basename($files3[0]);
  	}
  	$inObj = new stdClass();
  	$inObj->auContentURLPath = $realPath_au;
  	$inObj->crsContentURLPath = $realPath_crs;
  	$inObj->cstContentURLPath = $realPath_cst;
  	$inObj->desContentURLPath = $realPath_des;
  	$inObj->baseFileName=$aubasename;
  	$havingSubFolder  = getTotalSubDir($realPath);
  	$inObj->hostedType=($havingSubFolder)?"EXP_HOSTED":"NON_EXP_HOSTED";
    expDebug::dPrint("hostedType" . $inObj->hostedType , 4);
  	$inputValues = ContentAICCValidate($inObj);
  }
  $sizePath = ($contentType != "lrn_cnt_typ_aic") ? 0 : 1;
  $inputValues[$sizePath]->file_size = getUploadedFileSize($contentData->filesize);
  $inputValues[$sizePath]->file_path = $setFileExit;
  $inputValues[$sizePath]->file_name = $fileName;
  expDebug::dPrint("inputValues >>> ".print_r($inputValues, true) , 4);

  /* Start of CDN upload - Only for Video & Knowledge Content */
  try {
  	include_once(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info.inc');
  	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
  	$cdn_status  = getDrupalModuleStatus(array('cloud_files'));
  	expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' .print_r($cdn_status,true), 4);
  	if($cdn_status == 1 && ($contentType == "lrn_cnt_typ_vod" || $contentType == "lrn_cnt_typ_knc"))
  	{
  		expDebug::dPrint(" Upload Content to CDN fileName -  ".print_r($fileName, true) , 4);
  		expDebug::dPrint(" Upload Content to CDN localStorePath -  ".print_r($localStorePath, true) , 4);

  		$cdnUploadFile = '';
  		$CDNuploadfolder = '';
  		$CDNuploadfolder = str_replace('./sites/default/files','',$setFileExit);

  			if($contentType == "lrn_cnt_typ_vod"){
  				$cdnUploadFile = fopen($localStorePath, 'r');
  			}
  			else {
  				$cdnUploadFile = fopen($localStorePath.'/'.$fileName, 'r');
  			}

  		$rackspace_cloud_region = variable_get('rackspace_cloud_region');
  		$rackspace_cloud_container = variable_get('rackspace_cloud_container');

  		expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_container->' . print_r($rackspace_cloud_container, true) , 4);
  		expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_region->' . print_r($rackspace_cloud_region, true) , 4);

  		$client = initializeCDN();
  		$client->authenticate();
  		$object_store_service = $client->objectStoreService('cloudFiles', $rackspace_cloud_region);
  		$container = $object_store_service->getContainer($rackspace_cloud_container);
  		$container->uploadObject('/'.$CDNuploadfolder.'/'.$fileName, $cdnUploadFile);

  		}

  } catch (Exception $ex) {
  	watchdog_exception('getUploadContentData : CDN upload', $ex);
  	expDebug::dPrint(' getUploadContentData : CDN upload Exception->' . print_r($ex, true) , 1);
  	add_audit_trail_entry(getIdOfLoggedInUser(), '0', 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'getUploadContentData', 'Content Uploaded Failed In CDN');
  	//expertusErrorThrow($ex);
  }
  /*End of CDN upload*/
  return $inputValues;
  } catch (Exception $ex) {
  	watchdog_exception('getUploadContentData', $ex);
  	expertusErrorThrow($ex);
  }
}

function file_convertion_logic($content_upload_details,$from_cron=0,$form, &$form_state){
	include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
	expDebug::dPrint(' convertion logic->' . print_r($content_upload_details, true) , 4);
		//upload path variable from config
		$config=getConfig("exp_sp");
	$jod_converter_url = $config['jod_converter_url'];
		$content_upload_path = $config['content_upload_path'];
	if($from_cron==0){
		$image_ext =  array('gif','png' ,'jpg', 'jpeg','pdf');
		//find expention of upload file
		$file_arr  = explode(".", $content_upload_details->filename);
		$ext = strtolower(end(($file_arr)));
		if(!in_array($ext, $image_ext)){
			$uri_arr = explode('/',$content_upload_details->uri);
			$cnt = count($uri_arr);
			$file_name_folder = $uri_arr[$cnt-2];
			expDebug::dPrint(' convertion logic test->' . print_r($file_name_folder, true) , 4);
			//create convertion folder name
			$source_path = $content_upload_path.'/contentupload/'.$file_name_folder.'/'.$content_upload_details->filename;
			$designation_folder = $content_upload_path.'/contentupload/convertioncontent/'.$file_name_folder;
			$oldmask = umask(0);
			mkdir($designation_folder, 0777,true);
			umask($oldmask);
			$file_arr_cnt = count($file_arr);
			$file_name = '';			
			for($i=0;$i<($file_arr_cnt-1);$i++){
				if($file_name=='')
					$file_name=$file_arr[$i];
				else
					$file_name=$file_name.'.'.$file_arr[$i];
			}	
			$designation_path = $designation_folder.'/'.$file_name.'.pdf';
			if(is_dir($designation_folder)){
				if($ext!='pdf'){
	 				$comment = 'wget '.$jod_converter_url.' \    --post-file='.$source_path.' \    --header="Content-Type: '.$content_upload_details->filemime.'" \    --header="Accept: application/pdf" \    --output-document='.$designation_path. ' > /dev/null 2>&1 & echo $!; ';			
	 				//$comment = 'wget '.$jod_converter_url.' \    --post-file='.$source_path.' \    --header="Content-Type: '.$content_upload_details->filemime.'" \    --header="Accept: application/pdf" \    --output-document='.$designation_path;
					expDebug::dPrint(' jod convetion comment->' . print_r($comment, true) , 4);
					$pid= exec($comment, $output);
					$comment_start_time = time();
					expDebug::dPrint(' jod convetion output->' . print_r($output, true) , 4);
					expDebug::dPrint(' jod convetion pid->' . print_r($pid, true) , 4);
					$new_launch_url =DIRECTORY_SEPARATOR.'sites/default/files/contentupload/convertioncontent/'.$file_name_folder. DIRECTORY_SEPARATOR.$file_name. '.'. 'pdf';
					expDebug::dPrint(' jod convetion new_launch_url->' . print_r($_SERVER['DOCUMENT_ROOT'].$new_launch_url, true) , 4);
					$form_state['values']['pdf_conversion_status'] = '1'.'~~~'.''.'~~~'.$comment_start_time;;
					$file_exist_flag = 0;
					$end_time = time()+20;
					expDebug::dPrint(' unix end time ->' . print_r($end_time, true) , 4);
					do{
						sleep(2);
						$cur_time = time();
						expDebug::dPrint(' unix current time ->' . print_r($cur_time, true) , 4);
						$finfo = finfo_open(FILEINFO_MIME_TYPE);
						$mime = finfo_file($finfo, $_SERVER['DOCUMENT_ROOT'].$new_launch_url);
						expDebug::dPrint(' jod convetion mime->' . print_r($mime, true) , 4);
						$new_file_root_url = $_SERVER['DOCUMENT_ROOT'].$new_launch_url;
						expDebug::dPrint(' jod file url->' . print_r($new_file_root_url, true) , 4);
						expDebug::dPrint(' file_exists ->' . var_export(file_exists($new_file_root_url), true) , 4);
						$file_size = filesize($_SERVER['DOCUMENT_ROOT'].$new_launch_url);
						expDebug::dPrint(' file_size ->' . var_export($file_size, true) , 4);
						if($mime == 'application/pdf' && file_exists($new_file_root_url) && $file_size>0 && !is_dir("/proc/$pid")){
							expDebug::dPrint(' loop come hrer ->' . print_r($mime, true) , 4);
							$file_exist_flag = 1;
// 							$form_state['values']['pdf_conversion_status'] = 0;
// 							$kill_comment = 'kill -9 '.$pid. ' > /dev/null 2>&1 & echo $!; ';
// 							$pid1= exec($kill_comment, $output1);
// 							expDebug::dPrint(' jod convetion output1->' . print_r($output1, true) , 4);
// 							expDebug::dPrint(' jod convetion pid1->' . print_r($pid1, true) , 4);
						}
					}while($file_exist_flag==0 && ($cur_time < $end_time));
					if($file_exist_flag==0){
						$form_state['values']['pdf_conversion_status'] = '0'.'~~~'.$pid.'~~~'.$comment_start_time;
						//$kill_comment = 'kill -9 '.$pid. ' > /dev/null 2>&1 & echo $!; ';
						//$pid1= exec($kill_comment, $output1);
						//expDebug::dPrint(' jod convetion output1->' . print_r($output1, true) , 4);
						//expDebug::dPrint(' jod convetion pid1->' . print_r($pid1, true) , 4);
					}
					expDebug::dPrint(' jod convetion mime->' . print_r($mime, true) , 4);
					expDebug::dPrint(' jod form_state convertion_staus->' . print_r($form_state['values']['pdf_conversion_status'], true) , 4);
					
				}
			}
		}
	}
	if($from_cron==1){
		//find expention of upload file
		$url_arr  = explode("/", $content_upload_details->launchurl);
		$filename = end($url_arr);
		$file_arr = explode(".", $filename);
		$ext = strtolower(end($file_arr));
		$image_ext =  array('gif','png' ,'jpg', 'jpeg','pdf');
		if(!in_array($ext, $image_ext)){
			$uri_arr = explode('/',$content_upload_details->file_path);
			$designation_folder = end($uri_arr);;
			//create convertion folder name
			$source_path = $content_upload_path.'/contentupload/'.$designation_folder.'/'.$filename;
			$designation_folder = $content_upload_path.'/contentupload/convertioncontent/'.$designation_folder;
			$oldmask = umask(0);
			mkdir($designation_folder, 0777,true);
			umask($oldmask);
			$file_arr_cnt = count($file_arr);
			$file_name = '';
			for($i=0;$i<($file_arr_cnt-1);$i++){
				if($file_name=='')
				$file_name=$file_arr[$i];
				else
					$file_name=$file_name.'.'.$file_arr[$i];
			}
			$designation_path = $designation_folder.'/'.$file_name.'.pdf';
			
			if(is_dir($designation_folder)){
				if($ext!='pdf'){
					expDebug::dPrint(' designation folder name->' . print_r($designation_path, true) , 4);
					$filetype = _mime_content_type($source_path);
					expDebug::dPrint(' designation file type->' . print_r($filetype, true) , 4);
					
					$comment = 'wget '.$jod_converter_url.' \  --post-file='.$source_path.' \  --header="Content-Type: '.$filetype.'" \  --header="Accept: application/pdf" \  --output-document='.$designation_path.' > /dev/null 2>&1 & echo $!';
					expDebug::dPrint(' jod convetion comment->' . print_r($comment, true) , 4);
					$pid= exec($comment, $output);
					expDebug::dPrint(' jod convetion output->' . print_r($output, true) , 4);
					expDebug::dPrint(' jod convetion pid->' . print_r($pid, true) , 4);
					$updateStmt = db_update('slt_content_master');
					$updateStmt->condition('id', $content_upload_details->id);
					$fields = array(
							'custom4' => $pid,
							'custom3' =>time()
					);
					$updateStmt->fields($fields);
					$updateStmt->execute();
					return true;
				}
			}
		}
	}
}
function convertpdftoswf($source_file,$designation_file){
	$comment = '/usr/local/bin/pdf2swf '.$source_file.' -o '.$designation_file.' -I -T 9 -f -t -G -s storeallcharacters  2>&1';
	expDebug::dPrint(' swf convetion comment->' . print_r($comment, true) , 4);
	exec($comment,$output);
	expDebug::dPrint(' swf convetion output->' . print_r($output, true) , 4);
}
function convertVideoForMobileAccess($sourceDir,$ffmpeg_home)
{
	try {
	$output = "";
	$fileDir = $_SERVER["DOCUMENT_ROOT"]."/sites/default/files/".$sourceDir;
	if(strpos($fileDir, ".mp4")>0)
	{
		$tmp_arr = split("/",$fileDir);
		$fileDir = "";
		for($i=0;$i<count($tmp_arr)-1;$i++)
		{
			$fileDir.="/".$tmp_arr[$i];
		}
	}
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_video_conversion_for_mobile.inc";
	$videoBasename = get_video_filename_from_fs($fileDir);
	$srcFileName = $fileDir."/".$videoBasename;//.".mp4";
	$mobileVideoFileName = $fileDir."/"."mobile_".$videoBasename;

	expDebug::dPrint("srcFileName:".$srcFileName , 4);
	expDebug::dPrint("mobilesrcFileName :".$mobileVideoFileName , 4);

	$is_installed = ffmpeg_installed($ffmpeg_home);
	expDebug::dPrint("ffmpeg is_installed:".$is_installed , 4);
	if($is_installed == true)
	{
		$_SESSION['VODUploaded'] = $srcFileName;
		expDebug::dPrint("VODUploaded:".$srcFileName , 4);
		convertVideoForMobileAccessForMP4($srcFileName,$mobileVideoFileName,$ffmpeg_home);
	}
	else
	{
		form_set_error('', "Video conversion is not available in the server. Please contact your site admin.");
		//throw error;
	}
	} catch (Exception $ex) {
		watchdog_exception('convertVideoForMobileAccess', $ex);
		expertusErrorThrow($ex);
	}
}
function getTotalSubDir($dir){
	try {
    $folderCount = 0;
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$folderCount++;
			}
		}
		@closedir($handle);
	}
	return $folderCount;
	} catch (Exception $ex) {
		watchdog_exception('getTotalSubDir', $ex);
		expertusErrorThrow($ex);
	}
}

function getTotalSubFile($dir){
	try {
    $folderCount = 0;
    $fileCount = 0;
    $listFiles = array();
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$folderCount++;
			}
			else {
			    if($file != '.' && $file != '..'){
			     $listFiles[] = $file;
			     $fileCount++;
			    }
			    clearstatcache();
			}
		}
		@closedir($handle);
	}
	expDebug::dPrint('getTotalSubFile ' . $folderCount . ' file count ' .$fileCount , 4);
	expDebug::dPrint($listFiles , 4);
	if($folderCount == 0){
	  return $listFiles;
	}
	else{
	 return array();
	}
	} catch (Exception $ex) {
		watchdog_exception('ParseAICC', $ex);
		expertusErrorThrow($ex);
	}
}

function get_dirs($dir,$type = ''){
	try {
  expDebug::dPrint('get_dirs called ' . $dir , 4);
	// global $dirs;
	$getDirectoryName='';
	// if (!isset($dirs)){$dirs = '';}
	$preg_match = ($type == 'tincan') ? "/tincan.xml/" : "/imsmanifest.xml/";
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		//expDebug::dPrint('preg match -->>' . $dir);
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$opendirectory = @opendir($dir.$file);
				while ($filedir = @readdir($opendirectory)) {
					//expDebug::dPrint('preg match -->>' . $filedir);
					if (preg_match($preg_match,$filedir)) {
						$getDirectoryName = $file;
					}
				}
			}
		}
		@closedir($handle);
	}
	expDebug::dPrint('get_dirs getDirectoryName ' . $getDirectoryName , 4);
	return $getDirectoryName;
	} catch (Exception $ex) {
		watchdog_exception('ParseAICC', $ex);
		expertusErrorThrow($ex);
	}
}

function get_dirs_aicc($dir){
	try {
	// global $dirs;
	$getDirectoryName='';
	// if (!isset($dirs)){$dirs = '';}
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$opendirectory = @opendir($dir.$file);
				$f=glob("$dir/$file/{*.AU,*.au}",GLOB_BRACE);
				if(sizeOf($f)>0)
					$getDirectoryName = $file;
			}
		}
		@closedir($handle);
	}
	return $getDirectoryName;
	} catch (Exception $ex) {
		watchdog_exception('ParseAICC', $ex);
		expertusErrorThrow($ex);
	}
}

function ContentMasterScormValidate($inObj) {
	try {
  $aResult = ParseScormIMSManifestFile($inObj);
  $aResult = array_values($aResult);
  $aContentInfo='';

  /*$val = $_REQUEST['contentvalue'];
  $aval = explode(',',$val);*/
  $vRowIndex = 1;

  /*$outData="<Content>";
  $outData.="<RowIndex>$vRowIndex</RowIndex>";
  $outData .= "<totallesson>".count($aResult)."</totallesson>";*/

  /**
   * $contentPath - Added by Vincent to get the relative path for the upload content
   * for issue #5576 - On 07-Nov-2011
   */
  $contentPath = substr_replace($inObj->ContentURLPath,'',0,strpos($inObj->ContentURLPath,'/sites'));
  $countResult = count($aResult);
  for($vArrCount=0; $vArrCount < $countResult; $vArrCount++) {

  	$aContentInfo[$vArrCount]->title          = $aResult[$vArrCount]['title'];
  	$aContentInfo[$vArrCount]->masteryscore   = $aResult[$vArrCount]['masteryscore'];
  	$aContentInfo[$vArrCount]->href           = $aResult[$vArrCount]['href'];
  	$aContentInfo[$vArrCount]->launchurl      = str_replace('imsmanifest.xml',$aResult[$vArrCount]['href'],$contentPath);
  	$aContentInfo[$vArrCount]->datafromlms    = $aResult[$vArrCount]['datafromlms'];
  	$aContentInfo[$vArrCount]->version        = $aResult[$vArrCount]['version'];
  	$aContentInfo[$vArrCount]->scorm        = $aResult[$vArrCount]['scorm'];
  	$aContentInfo[$vArrCount]->scorm2004        = $aResult[$vArrCount]['scorm2004'];
  	$aContentInfo[$vArrCount]->new_format_version        = $aResult[$vArrCount]['new_format_version'];

  	/*$outData .= "<LessonItem>";
  	$outData .= "<title>".$aContentInfo[$vArrCount]->title."</title>";
  	$outData .= "<masteryscore>".$aContentInfo[$vArrCount]->masteryscore."</masteryscore>";
  	$outData .= "<launchurl>".$aContentInfo[$vArrCount]->launchurl."</launchurl>";
  	$outData .= "<datafromlms>".$aContentInfo[$vArrCount]->datafromlms."</datafromlms>";
  	$outData .= "</LessonItem>";*/
  }
  //$outData .= "</Content>";
  return $aContentInfo;
  } catch (Exception $ex) {
  	watchdog_exception('ParseAICC', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * Validate tincan content.
 */
function ContentMasterTinCanValidate($inObj){
	//expDebug::dPrint('scorm path AAAA----->>>'. print_r($inObj,true),5 );
	$aResult = ParsetinCanScormXmlFile($inObj->ContentURLPath);
	$aResult = array_values($aResult);
	$aContentInfo='';
	//expDebug::dPrint('scorm path BBBB--->>> '. print_r($aResult,true),5 );
	$vRowIndex = 1;
	expDebug::dPrint('content url'.$inObj->ContentURLPath, 4);
	$contentPath = substr_replace($inObj->ContentURLPath,'',0,strpos($inObj->ContentURLPath,'/sites'));
	//expDebug::dPrint('scorm path CCCCC--->>> '. $contentPath,5 );
	$countResult = count($aResult);
	for($vArrCount=0; $vArrCount < $countResult; $vArrCount++) {
		$aContentInfo[$vArrCount]->name          = $aResult[$vArrCount]['name'];
		$aContentInfo[$vArrCount]->launch   = $aResult[$vArrCount]['launch'];
		$aContentInfo[$vArrCount]->launchurl      = str_replace('tincan.xml',$aResult[$vArrCount]['launch'],$contentPath);
	}
	expdebug::dPrint('content information -->> '.print_r($aContentInfo,true),5 );
	return $aContentInfo;
}

function ContentAICCValidate($inObj){
  try{
	$aResult = ParseAICC($inObj);
  $aContentInfo='';

  /*$val = $_REQUEST['contentvalue'];
  $aval = explode(',',$val);*/
  $vRowIndex = 1;

  /*$outData="<Content>";
  $outData.="<RowIndex>$vRowIndex</RowIndex>";
  $outData .= "<totallesson>".$aResult[0]->TotlaLesson."</totallesson>";
  $outData .= "<coursetitle>".$aResult[0]->CourseTitle."</coursetitle>";*/

  /**
   * $contentPath - Added by Vincent to get the relative path for the upload content
   * for issue #5576 - On 07-Nov-2011
   */
  $contentPath = substr_replace($inObj->auContentURLPath,'',0,strpos($inObj->auContentURLPath,'/sites'));
  expDebug::dPrint("ContentAICCValidate contentPath ".count($aResult) , 4);
  for($vArrCount=1; $vArrCount < count($aResult); $vArrCount++) {
    	$aContentInfo[$vArrCount]->title          = $aResult[$vArrCount]->Title;
    	$aContentInfo[$vArrCount]->masteryscore   = $aResult[$vArrCount]->MasteryScore;
    	//$aContentInfo[$vArrCount]->href           = $aResult[$vArrCount]['href'];
    	if($inObj->hostedType=="EXP_HOSTED")
    		$aContentInfo[$vArrCount]->launchurl      = str_replace($inObj->baseFileName,$aResult[$vArrCount]->LaunchURL,$contentPath);
    	else
    		$aContentInfo[$vArrCount]->launchurl      = $aResult[$vArrCount]->LaunchURL;
    	$aContentInfo[$vArrCount]->datafromlms    = $aResult[$vArrCount]->datafromlms;
    	expDebug::dPrint('ContentAICCValidate Launch URL ' .$aContentInfo[$vArrCount]->launchurl , 4);
    	/*$outData .= "<LessonItem>";
    	$outData .= "<title>".$aContentInfo[$vArrCount]->title."</title>";
    	$outData .= "<masteryscore>".$aContentInfo[$vArrCount]->masteryscore."</masteryscore>";
    	$outData .= "<launchurl>".urlencode($aContentInfo[$vArrCount]->launchurl)."</launchurl>";
    //	$outData .= "<datafromlms>".$aContentInfo[$vArrCount]->datafromlms."</datafromlms>";
    	$outData .= "</LessonItem>";*/
  }
  //$outData .= "</Content>";
  return $aContentInfo;
  }catch(Exception $e){
  	expDebug::dPrint("Error -->".print_r($e,true),1);
  }
}

function ContentMasterKnowledgeContentValidate($urlpath,$filename,$contentType,$setFileExit) {
	try {
  /*$val = $_REQUEST['contentvalue'];
  $aval = explode(',',$val);*/
  $vRowIndex = 1;
  /**
   * $urlpath - Added by Vincent to get the relative path for the upload content
   * for issue #5576 - On 07-Nov-2011
   */
  $urlSet = 0;
  if($contentType == 'lrn_cnt_typ_knc'){
    expDebug::dPrint('ContentMasterKnowledgeContentValidate FileExitpath'.$setFileExit , 4);
    $fileList = getTotalSubFile($setFileExit);
    $fileCnt = count($fileList);
    if($fileCnt == 1){
      $urlpath = $urlpath.'/'.$fileList[0];
     // $urlSet = 1;  //Commented by Vincent on May 9, 2013. to fix #0023551
    }
  }
  expDebug::dPrint('ContentMasterKnowledgeContentValidate urlpath'.$urlpath , 4);
  if($urlSet == 0){
    $urlpath = substr_replace($urlpath,'',0,strpos($urlpath,'/sites'));
  }
  /*$outData="<Content>";
  $outData.="<RowIndex>$vRowIndex</RowIndex>";
  $outData.="<launchurlbase>".$urlpath."</launchurlbase>";
  $outData.= "</Content>";
  return $outData;*/
  $aContentInfo='';
  $aContentInfo[0]->title          = $filename;
  $aContentInfo[0]->masteryscore   = 0;
  $aContentInfo[0]->href           = '';
  $aContentInfo[0]->launchurl      = $urlpath;
  $aContentInfo[0]->datafromlms    = '';
  return $aContentInfo;
  } catch (Exception $ex) {
  	watchdog_exception('ParseAICC', $ex);
  	expertusErrorThrow($ex);
  }
}

function URLContentValidate($urlpath) {
	try {
  $aContentInfo='';
  $masterTitle = parse_url($urlpath) ;
  $contentTitle = $masterTitle['host'];
  $fileNameFind = explode('//',$urlpath,2);
  $title = $fileNameFind[(count($fileNameFind)-1)];
  $aContentInfo[0]->master_title   = $contentTitle;
  $aContentInfo[0]->title          = $title;
  $aContentInfo[0]->hostedtype     = 2;
  $aContentInfo[0]->masteryscore   = 0;
  $aContentInfo[0]->href           = '';
  $aContentInfo[0]->launchurl      = $urlpath;
  $aContentInfo[0]->datafromlms    = '';
  return $aContentInfo;
  } catch (Exception $ex) {
  	watchdog_exception('ParseAICC', $ex);
  	expertusErrorThrow($ex);
  }
}

function listAvailableContentByRestAPI($start, $limit, $userid="")
{
	try {
	$form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$result = fetchContentAll('',$start,$limit,'API');
	$result = $result[0];
  	$cntResults = count($result);
  	if($cntResults){
    	$result['totalrow'] = $cntResults;
  	}
	return $result;
	} catch (Exception $ex) {
		watchdog_exception('ParseAICC', $ex);
		expertusErrorThrow($ex);
	}
}

function getAdminContentVersionList($contentId)
{
	try{
	  $select = db_select('slt_content_version', 'version');
	  $select->addExpression('count(1)','version_cnt');
	  $select->condition('version.content_master_id', $contentId);
	  $select->condition('version.status', 'lrn_cnt_sts_del','<>');
	  $result = $select->execute();
	  return $result->fetchField();
	}catch (Exception $ex) {
	  watchdog_exception('getAdminContentVersionList', $ex);
	  expertusErrorThrow($ex);
  	}
}

function getAdminContentVersionRecords($contentId,$start, $limit){
	try{
	  $select = db_select('slt_content_version', 'version');
	  $select->addField('version','id','version_id');
	  $select->addField('version','content_master_id','master_id');
	  $select->addField('version','content_sub_type','sub_type');
	  $select->addField('version','title','version_title');
	  $select->addField('version','hosted_type','hosted_type');
	  $select->addField('version','total_lesson','version_lessons');
	  $select->addField('version','version','version_number');
	  $select->addField('version','overall_mastery_score','version_score');
	  $select->addField('version','status','version_status');
	  $select->addField('version','file_size','version_file_size');
	  $select->addField('version','file_path','version_file_path');
	  $select->addField('version','created_by','version_creator');
	  $select->addField('version','created_on','version_created');
	  $select->addField('version','updated_by','version_updator');
	  $select->addField('version','published_on','version_updated');
	  $select->condition('version.content_master_id', $contentId);
	  $select->condition('version.status', 'lrn_cnt_sts_del','<>');
	  $select->orderBy('version_number','DESC');
	  $select->range($start, $limit);
	  expDebug::dPrintDBAPI(' getAdminContentVersionRecords SQLLL ------>     ' ,  $select);
	  $result =  $select->execute()->fetchAll();
	  expDebug::dPrint(' query result ---- > '. print_r($result,true),4);
	  return $result;
	}catch (Exception $ex) {
	  watchdog_exception('getAdminContentVersionRecords', $ex);
	  expertusErrorThrow($ex);
  	}
}

function getContentVersionResults($contentId){
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

  $numContent = getAdminContentVersionList($contentId);

  expDebug::dPrint(' count version $numContent = ' . $numContent,4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numContent);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);


  $contentVersionList = getAdminContentVersionRecords($contentId,$start, $limit);
  expDebug::dPrint('$contentList = ' . print_r($contentVersionList, true),4);

  // Prepare and send the response
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numContent;
  $response->contentId  = $contentId;

  //$response->initial_sort_type_html_id = 'content-sort-new';

  $i=0;
  foreach ($contentVersionList as $contentVersion) {
    expDebug::dPrint('$content = '. print_r($contentVersion, true),4);
    $cTStatus = array('inprogress');
    $jobCount = fetchCTBackgroundList($cTStatus,'inlist',$contentId);
    $contentVersion->content_transfer = ($jobCount > 0) ? 1 : 0;

    $contentVersion->row =  'tr_'.$i;
    $response->rows[$i]['id']   = 'tr_'.$i;
    $response->rows[$i]['cell'] = array(
    							  'detail' => theme('exp_sp_administration_content_version_list', array('detail' => $contentVersion,'contentId'=>$contentId))
                                  );
	$_SESSION['jq_modal'][$i] = $contentVersion->version_id;
    $i++;
  }
  expDebug::dPrint('$$srch_result_response = '. print_r($response, true),4);
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('ParseAICC', $ex);
  	expertusErrorThrow($ex);
  }
}


function getActiveUsersForContent($contentId,$versionId){
	try{
	  $select = getUsersQueryForVersionList($contentId,$versionId,'','','count');
	  expDebug::dPrintDBAPI('getActiveUsersForContent SQLLL ------>     ' ,  $select);
	  $result =  $select->execute()->fetchAll();
	  expDebug::dPrint(' getActiveUsersForContent query result ---- > '. print_r($result,true),4);
	  return count($result);
	}catch (Exception $ex) {
	  watchdog_exception('getActiveUsersForContent', $ex);
	  expertusErrorThrow($ex);
  	}

}

function getOtherVersions($contentId,$versionId){
    try{
	  $select = getUsersQueryForVersionList($contentId,$versionId,'','','otherversions');
	  expDebug::dPrintDBAPI('getOtherVersions SQLLL ------>     ' ,  $select);
	  $result =  $select->execute()->fetchAll();
	  expDebug::dPrint(' getOtherVersions query result ---- > '. print_r($result,true),4);
	  return $result;
	}catch (Exception $ex) {
	  watchdog_exception('getOtherVersions', $ex);
	  expertusErrorThrow($ex);
  	}
}

function cancelContentVersionTConflict($contentId,$versionId,$jobId){
	$status = "cancelled";
	$update = db_update('slt_temp_background_jobs');
	$update->fields(array(
			'status'=> $status,
			'updated_on' => now()
	));
	$update->condition('id',$jobId,'=');
	expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs status update cancelled due to publishing other version $versionId",$update);
	$update->execute();
	setContentVersionToDefault($contentId,$versionId);
}

function checkContentVersionTConflict($contentId,$versionId){
	$cTStatus = 'inprogress';
	$inProgressJobs = fetchCTBackgroundList($cTStatus,'equals',$contentId);
	$cTStatus =  'new';
	$newJobs = fetchCTBackgroundList($cTStatus,'equals',$contentId);
	$return = array("newitems"=>$newJobs[0]->job_id, "inprogress"=>$inProgressJobs[0]->job_id);
	print json_encode($return);
}

function setContentVersionToDefault($contentId,$versionId){
	expDebug::dPrint(' setContentVersionToDefault Inside ---- > '. print_r($contentId,true),4);
	$txn = db_transaction();
  	try{
		$currUserId = getSltpersonUserId();
		$updateDefault = db_update('slt_content_version');
	    $updateDefault->condition('id', $versionId);
	    $updateDefault->fields(array(
	                        'status'      => 'lrn_cnt_sts_atv',
	    					'published_on'    => now(),
	                        'published_by'    => $currUserId,
	                        'updated_on'    => now(),
	                        'updated_by'    => $currUserId
	                        ));
	    expDebug::dPrintDBAPI('setContentVersionToDefault $updateDefault  --> ',$updateOtherVersions);
	    $updateDefault->execute();


		$updateOtherVersions = db_update('slt_content_version');
		$updateOtherVersions->condition('id', $versionId,'<>');
		$updateOtherVersions->condition('content_master_id', $contentId);
		$updateOtherVersions->condition('status', 'lrn_cnt_sts_del','<>');

	    $updateOtherVersions->fields(array(
	                        'status'      => 'lrn_cnt_sts_itv',
	                        'updated_on'    => now(),
	                        'updated_by'    => $currUserId
	                        ));
	    expDebug::dPrintDBAPI('setContentVersionToDefault $updateOtherVersions  --> ',$updateOtherVersions);
	    $updateOtherVersions->execute();
  	}catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('setContentVersionToDefault', $ex);
	    throw $ex;
  	}

  	unset($txn); // Commit txn

}

function deleteContentVersion($versionId){
  $txn = db_transaction();
  try{
	$currUserId = getSltpersonUserId();
	$deleteVersion = db_update('slt_content_version');
    $deleteVersion->condition('id', $versionId);
    $deleteVersion->fields(array(
                        'status'      => 'lrn_cnt_sts_del',
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    expDebug::dPrintDBAPI('deleteContentVersion delete query --> ',$deleteVersion);
    $deleteVersion->execute();

    /* commented part to be used for folder deletion on version delete
     * commented because even completed users will still need the folders for the launch of those contents
     */
    ////44353: When an admin user deletes the content the physical file should be deleted from the content folder
    $filepathSelect  = db_select('slt_content_version','version');
    $filepathSelect->addField('version','file_path');
    $filepathSelect->addField('version','content_sub_type');
    $filepathSelect->condition('version.id',$versionId,'=');
    $filePath_res = $filepathSelect->execute()->fetchAll();
    $filePath = $filePath_res[0]->file_path;
    $content_type = $filePath_res[0]->content_sub_type;
    expDebug::dPrint(' deleteContentVersion delete file path TT'.print_r($filePath,true),4);
    $uploadDirectory = './sites/default/files/contentupload';
    if(!empty($filePath) && ($filePath != $uploadDirectory) && ($filePath != $uploadDirectory.'/')){
 			removedir($filePath);
    	unlink($filePath . '.zip');
    	//delete the converted files folder like pdf and swf
    	if($content_type == 'lrn_cnt_typ_knc'){
	    	$uploaded_folder_name =  end(explode('/', $filePath));
	    	//upload path variable from config
	    	$config=getConfig("exp_sp");
	    	$content_upload_path = $config['content_upload_path'];
	    	$designation_folder = $content_upload_path.'/contentupload/convertioncontent/'.$uploaded_folder_name;
	    	if(is_dir($designation_folder)){
	    		removedir($designation_folder);
    }
    	}
    }
    
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteContentVersion', $ex);
    throw $ex;
  }

  unset($txn); // Commit txn
}


function getUserDetailsToMove($contentId,$versionId){
  try {

       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'content_id'    => $contentId,
       'version_id'  => $versionId
      );

      $formId                 = 'exp_sp_moveusers_version_form';
      $builtForm              = drupal_build_form($formId, $form_state,$contentId,$versionId);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $moveUsersResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($moveUsersResponse);
      exit;

  } catch (Exception $ex) {
    watchdog_exception('getUserDetailsToMove', $ex);
    expertusErrorThrow($ex);
  }

}

function exp_sp_moveusers_version_form($form,&$form_state) {
  try {
    return displayMoveUsersDetails($form,$form_state,$form_state['content_id'], $form_state['version_id']);
  } catch (Exception $ex) {
    watchdog_exception('exp_sp_moveusers_version_form', $ex);
    expertusErrorThrow($ex);
  }

}

function displayMoveUsersDetails(&$form,&$form_state,$contentId, $versionId){
	try {
	    global $theme_key;
      $formName = 'move-users';
      $uniqueId = 'ContentMoveUsers-'.$contentId.'-'.$versionId;
      $wrapperId = 'content-grid-wrapper-'.$uniqueId;
      $customHiddenButtonId = 'equv-save-esign';
      if($form_state['submitted']==1){
      	$cTStatus = array('inprogress');
      	$inProgressJobs = fetchCTBackgroundList($cTStatus,'inlist',$contentId);
      	if($inProgressJobs!=0){
      		drupal_set_message(t('MSG993'));/*Viswanathan modified the MSG string for #77773*/
      		return;
      	}
      	$cTStatus =  array('new');
      	$newJobCount = fetchCTBackgroundList($cTStatus,'inlist',$contentId);
      	if($newJobCount > 1){
      	$cTStatus = 'new';
      	$newJob = fetchCTBackgroundList($cTStatus,'equals',$contentId);
      		$job = $newJob[0];
      		expDebug::dPrint(' print displayMoveUsersDetails job details ----> '.print_r($job,true),4);
      		$jobId = $job->job_id;
      		$status = "cancelled";
      		// Update the status once process is in progress
      		$update = db_update('slt_temp_background_jobs');
      		$update->fields(array(
      				'status'=> $status,
      				'updated_on' => now()
      		));
      		$update->condition('id',$jobId,'=');
      		expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs status update cancelled due to overriding request",$update);
      		$update->execute();
      	}
      }      

  	  /* wrapper id for grid */
      $setminHight = 'admin-attach-course-tab-datagrid-wrapper';
	  $form[$formName] = array(
	    '#type' => 'markup',
	    '#prefix' => '<div id="admin-move-users-content"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
	    '#suffix' => '</div></div></div>',
	  );

	  $form[$formName]['add_multi_search_container_start'] = array(
	      '#type' => 'markup',
	      '#markup' => '<div class="admin_add_multi_search_container">',
      );

      /* hidden input type to carry the selected users */
      $form[$formName]['hidden_idlist'] = array(
	      '#type' => 'hidden',
	      '#name' => 'hidden_idlist_'.$uniqueId,
	      '#id' => 'datagrid-idlist-'.$uniqueId,
      );

      $form[$formName]['hidden_completedvalue'] = array(
      		'#type' => 'hidden',
      		'#name' => 'hidden_completedvalue_'.$uniqueId,
      		'#id' => 'datagrid-completedvalue-'.$uniqueId,
      );
      
      $form[$formName]['hidden_move_content_users'] = array(
      		'#type' => 'hidden',
      		'#name' => 'hidden_move_content_users_'.$contentId.'-'.$versionId,
      		'#id' => 'move_content_users-'.$contentId.'-'.$versionId,
      );
      $form[$formName]['search_all_moveuser_type-hidden'] = array(
	      '#type' => 'hidden',
	      '#name' => 'search_all_moveuser_type-hidden',
	      '#attributes' => array('id' => 'search_all_moveuser_type-hidden'),
      	  '#value' => 'user',
      );
      /* select drop down for search by username and class title */
      $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list'>";
      //$paintMultiAction .= "<input type='hidden' id='search_all_moveuser_type-hidden' value='user' />";
      $paintMultiAction .= "<input type='hidden' id='contentmoveusers-autocomplete_hidden' value='".t('LBL181')."' />";
      $paintMultiAction .= "<span id='select-moveuser-dropdown' class='select-moveuser-dropdown'>".t('LBL054')."</span>";
      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-moveuser-dropdown-link dropdown-link-font ' onclick='$(\"#select-moveuser-dropdown-list\").slideToggle();$(\"#select-moveuser-dropdown-list li:last\").css(\"border-bottom\",\"0px none\");'>&nbsp;</a>";
      $paintMultiAction .= "<ul id='select-moveuser-dropdown-list'>";
      $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreMoveUserSearchTypeText('".stripApostrophe(t('LBL054'))."','user','TP');\">".t('LBL054')."</li>";
      $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreMoveUserSearchTypeText('".stripApostrophe(t('LBL262'))."','classtitle','TP');\">".t('LBL262')."</li>";
      $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreMoveUserSearchTypeText('".stripApostrophe(t('LBL102'))."','status','TP');\">".t('LBL102')."</li>";
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";

      /*$form[$formName]['add_moveusers_search'] = array(
	      '#type' => 'markup',
	      '#markup' => '<div class="admin_add_multi_auto_search">',
      );*/

  	  $form[$formName]['more_add_moveusers_search'] = array(
	      '#type' => 'markup',
	      '#markup' => $paintMultiAction,
      );
      $excludedCourseId =0;
      $searchAttributes['onClick'][] = 'searchVersionDataGrid("edit", "ContentMoveUsers", $(\'#contentmoveusers-autocomplete\').val(), '.$contentId.', '.$versionId.', "'.$excludedCourseId.'"); return false;';
      /* code for autocomplete */
      $acMenuPath = 'administration/manage/content/allassocversionusers-autocomplete';

      $form[$formName]['add_moveuser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );

      $default_text_title = t('LBL181');
      $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form[$formName], 'contentmoveusers-autocomplete', $acMenuPath, '', '', '', array(), array('entity_id'  =>  $contentId, 'entity_type' => $versionId ,'search_type' => "$('#search_all_moveuser_type-hidden').val()",'excluded_course_id' => "'".$excludedCourseId."'", 'id_required' => 0), false, false, $default_text_title,$autoCompleteAttributes);



      $form[$formName]['add_moveusers_search_end'] = array(
	      '#type' => 'markup',
	      '#markup' => '</div>',
      );
      /* search icon */
      $searchAttributes['Class'][] = 'admin-pagination-search-go';
      $searchAttributes['Class'][] = 'float-left';
	  $form[$formName]['search_moveusers'] = array(
	        '#type' => 'submit',
	        '#title' => t('LBL304'), //Search
	        '#name' => 'Search moveuser',
	        '#submit' => array('search_moveuser'),// function search_moveuser does not exist
	        '#attributes' => $searchAttributes,
      );
      $filterSuffix = '';
      if($theme_key == 'expertusoneV2') {
      	$filterSuffix = '<div class="filter-search-start-date-right-bg"></div>';
      }

      $form[$formName]['add_multi_search_container_end'] = array(
	      '#type' => 'markup',
	      '#markup' => $filterSuffix . '</div><div class="clearBoth"></div>',
      );

      $form[$formName]['moveusers_view_grid_markup'] = array(
	      '#type' => 'markup',
	      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
      );
      $excludedCourseId =0;
      $form[$formName]['moveusers_view_grid_load'] = array(
	      '#type' => 'markup',
	      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); loadDataGrid(\'edit\', \'ContentMoveUsers\', $(\'#contentmoveusers-autocomplete\').val(), \''.$contentId.'\', \''.$versionId.'\',\'0\');" width="100" height="132" /></div>',
      );

      $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg';
      $cancelAttributes['onclick'][] = '$(".qtip-active").remove(); isPopupOpen=0; $.fn.refreshVersionList('.$contentId.'); return false;'; //was .active-qtip-div
      $cancelAttributes['data-wrapperid'] = array($wrapperId);
			$prfixPgLeft = '';
			$suffixPgRight = '';
			if($theme_key == 'expertusoneV2'){
			  $prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
			  $suffixPgRight = '<div class="white-btn-bg-right"></div>';
			  $cancelAttributes['class'][] = 'white-btn-bg-middle';
			}
      $form[$formName]['cancel_moveusers_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL123'),
          '#title' => t('LBL123'),
          '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom content-save-cancel-btnGrp"><div class="addedit-form-cancel-container-actions admin-save-button-container">' . $prfixPgLeft,
      	  '#suffix' => $suffixPgRight,
          '#name' => 'Cancel moveusers',
          '#attributes' => $cancelAttributes,
      );

      /* move users to version button */

	  $currDefaultVersion = getDefaultContentVersion($contentId);
      $setAttributes = array();
      $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
      $setAttributes['onclick'][] = ' isPopupOpen=0; setCompleteUserValue(\''.$uniqueId.'\'); $.fn.refreshVersionList('.$contentId.'); return false;'; //was .active-qtip-div
            $setAttributes['data-wrapperid'] = 'content-grid-wrapper-'.$uniqueId;
            $form[$formName]['set_moveusers_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<div class="admin-save-pub-unpub-button-container transfer-users-grp">' .
                             '<div class="admin-save-button-left-bg"></div>',
      		    '#suffix' =>   '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'catalog-pub-add-list-move\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>
      		                 </div>' .
                           '<ul class="catalog-pub-add-list-move">',
              '#title' => t('LBL947'),
              '#value' => t('LBL947'),
              '#name' => 'Move_Selected',
              '#submit' => array('moveUsersDetailsList'),
              '#attributes' => $setAttributes,
              '#ajax' => array(
        		    'wrapper' => 'content-grid-wrapper-'.$uniqueId,
        		    'callback' => 'moveUsersCallBack',
                    'content_id' =>$contentId,
                    'version_id' =>$versionId,
            		'default_version' => $currDefaultVersion[0]->version_id,
              		'default_lesson' => $currDefaultVersion[0]->lesson_id,
              	      'method' => 'replace',
            	      'effect' => 'none', // 'fade',
            	      'event'=>'click',
            	      'keypress' => true,
            	      'progress' => array(
            	        'type' => 'throbber',
            	        'message' => ''
            	      ),
               ),
            );
            /* move all users to version button */
            $moveAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $moveAttributes['onclick'] = 'setCompleteUserValue(\''.$uniqueId.'\');';
            $moveAttributes['data-wrapperid'] = 'content-grid-wrapper-'.$uniqueId;
            $form[$formName]['set_moveallusers_edit_mode'] = array(
              '#type' => 'submit',
              '#prefix' => '<li class="save-pub-unpub-sub-menu">',
      		  '#suffix' => '</li>',
              '#title' => t('LBL948'),
              '#value' => t('LBL948'),
              '#name' => 'Move_All',
              '#submit' => array('moveAllUsersDetailsList'),
              '#attributes' => $moveAttributes,
              '#ajax' => array(
        		    'wrapper' => 'content-grid-wrapper-'.$uniqueId,
        		    'callback' => 'moveUsersCallBack',
                    'content_id' =>$contentId,
                    'version_id' =>$versionId,
            		'default_version' => $currDefaultVersion[0]->version_id,
              		'default_lesson' => $currDefaultVersion[0]->lesson_id,
              	      'method' => 'replace',
            	      'effect' => 'none', // 'fade',
            	      'event'=>'click',
            	      'keypress' => true,
            	      'progress' => array(
            	        'type' => 'throbber',
            	        'message' => ''
            	      ),
               ),
            );

      $form[$formName]['attach_course_button_end'] = array(
	        '#type' => 'markup',
	        '#markup' => '</ul>',
      );

  	  $form[$formName]['addedit-left-col-end'] = array(
	      '#type' => 'markup',
	      '#markup' => '</div></div>',
  	  );


  expDebug::dPrint(' print form ----> '.print_r($form,true),4);
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('displayMoveUsersDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function getUserClassForVersionAutocomplete(){
	try{
		$contentId   = $_GET['entity_id'];
		$versionId = $_GET['entity_type'];
		$nameSubstr = $_GET['z'];
		$searchType = $_GET['search_type'];
		$select = getUsersQueryForVersionList($contentId,$versionId,$nameSubstr,$searchType,"autocomplete-search");
		expDebug::dPrintDBAPI('getUserClassForVersionAutocomplete  $select SQL = ' , $select);
	    $matchingQuery = $select->execute()->fetchAll();
	    foreach ($matchingQuery as $result) {
	    	 print $result->name . "\n";
	    }
	}catch (Exception $ex) {
	    watchdog_exception('getUserClassForVersionAutocomplete', $ex);
	    expertusErrorThrow($ex);
  	}


}

function moveUsersDetailsList($form, &$form_state) {

    $contentId = $form_state['content_id'];
    $versionId = $form_state['version_id'];
	$triggeringElement = $form_state['input']['_triggering_element_name'];
    $uniqueId  = "ContentMoveUsers-$contentId-$versionId";
	if($form_state['input']['hidden_completedvalue_'.$uniqueId] == 1){
		$commands = array();
		$commands[] = displayConfirmationmessagewizard($contentId, $versionId, $triggeringElement);
		print ajax_render($commands);
		exit;
	}
  $txn = db_transaction();

  
  if(empty($form_state['input']['hidden_idlist_'.$uniqueId]) && empty($form_state['hidden_idlist_'.$uniqueId])){
  	drupal_set_message(t('ERR106'));
  }
  try {
    $currDefaultVersion = $form_state['triggering_element']['#ajax']['default_version'];
    $currDefaultLesson = $form_state['triggering_element']['#ajax']['default_lesson'];
    $configLimit = getConfigValue('content_move_all_users_limit');
    expDebug::dPrint('content_move_all_users_limit variable from cnf: ' . $configLimit, 4);

    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
      $userId = getSltpersonUserId();
      if (!empty($form_state['input']['hidden_idlist_'.$uniqueId])) {
      	  $hiddenIds = 	str_replace('-',',',$form_state['input']['hidden_idlist_'.$uniqueId]);
      }
      else {
      	  $hiddenIds = 	str_replace('-',',',$form_state['hidden_idlist_'.$uniqueId]);
      }
      $associateIdListArray = explode(',', $hiddenIds);
      expDebug::dPrint("assoc array -----------> ".print_r($associateIdListArray,true),4);
      if($associateIdListArray[0] == 0){
      	$associateIdListArray = array_slice($associateIdListArray, 1);
      }
      if (count($associateIdListArray) <= $configLimit ) {
	  expDebug::dPrint('ARRAY DEFASULT VERSION ----> '.$currDefaultVersion,4);
	  //select mapping ids for audit trail
	  $getUpdatedMapIds = db_select('slt_enrollment_content_mapping', 'ecm');
	  $getUpdatedMapIds->addField('ecm', 'id');
		  $getUpdatedMapIds->addField('ecm', 'version_id');
	    $getUpdatedMapIds->condition('ecm.id', $associateIdListArray,'IN');
	  $getUpdatedMapIds->condition('version_id', $currDefaultVersion, '!=');
	  expDebug::dPrintDBAPI(' $getUpdatedMapIds', $getUpdatedMapIds);
		  $result1 = $getUpdatedMapIds->execute()->fetchAssoc();
	  expDebug::dPrint(' result from ecm'.print_r($result1, 5));
	  
		  if(count($result1) > 0){
			  
	  // commented for 61786
	  // foreach ($result1 as $map) {
		  add_audit_trail_entry($userId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'moveUsersDetailsList', 'Transferring users from one content to another content',
			null,$form_state['version_id'] , $currDefaultVersion );
	 // }
	  $updateVersion = db_update('slt_enrollment_content_mapping');
      $updateVersion->condition('id', $associateIdListArray,'IN');
      $updateVersion->fields(array(
                        'version_id'      => $currDefaultVersion,
                        'updated_on'    => now(),
                        'updated_by'    => $userId
                        ));
       expDebug::dPrintDBAPI('QUERY DEFAULT ---> ',$updateVersion);
      $updateVersion->execute();

      /* added for notification trigger on moving users */
      $clsDetails = db_select("slt_course_class","cls");
      $clsDetails->join("slt_enrollment_content_mapping","emap","cls.id = emap.class_id");
      $clsDetails->join("slt_person","per","per.id = emap.user_id");
	  $clsDetails->addField('emap','id','id');
      $clsDetails->addField('emap','user_id','user_id');
	  $clsDetails->addField('per','first_name','first_name');
	  $clsDetails->addField('emap','enroll_id','enroll_id');
	  $clsDetails->addField('cls','title','class_title');
	  $clsDetails->addField('cls','code','class_code');
      $clsDetails->condition('emap.id', $associateIdListArray,'IN');
      expDebug::dPrintDBAPI('QUERY DEFAULT beforeee ---> ',$clsDetails);
      $result = $clsDetails->execute()->fetchAll();

      $enrollIds = '';
	  foreach($result as $emapId){
			$enrollIds .= (empty($enrollIds) ? '' : ',').$emapId->enroll_id;
	  }
	  $enrollIds = explode(',', $enrollIds);
      $updScore = db_update('slt_enrollment');
      $updScore->condition('id', $enrollIds,'IN');
	      $updScore->condition('comp_status', 'lrn_crs_cmp_cmp','!='); // 0070552 - exclude completed class enrollments in score update
      $updScore->fields(array(
                        'score'      => 0,
                        'updated_on'    => now(),
                        'updated_by'    => '1'
                        ));
      expDebug::dPrintDBAPI('QUERY DEFAULT set score ===0---> ',$updScore);
      $updScore->execute();

      /*
       * update progress information
       */
      include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
      foreach($enrollIds as $enr){
      	$enrollDetails = getEnrollmentDetails($enr,array('master_enrollment_id','comp_status'));
      	
      	if($enrollDetail->comp_status != 'lrn_crs_cmp_cmp'){
	      	//get launch details
	      	$launchDetails = getLaunchDetail($enr);
	      	$progress = get_progress($launchDetails, $enr,1);
	      	expDebug::dPrint("progress percentage >> ". $progress);
	      	$Updfields = array();
	      	if($progress == 100){
	      		$Updfields['comp_status'] = 'lrn_crs_cmp_cmp';
	      		$Updfields['comp_date'] = now();
	      	}
	      	
	      	$Updfields['progress'] = $progress;
	      	$Updfields['updated_on'] = now();
	      	$Updfields['updated_by'] =  '1';
	      	
	      	$updScore = db_update('slt_enrollment');
	      	$updScore->condition('id', $enr,'=');
	      	$updScore->condition('comp_status', 'lrn_crs_cmp_cmp','!='); // 0070552 - exclude completed class enrollments in score update
	      	$updScore->fields($Updfields);
	      	expDebug::dPrintDBAPI('QUERY DEFAULT set progress ---> ',$updScore);
	      	$updScore->execute();
	      	 if(!empty($enrollDetails->master_enrollment_id)){
	      		$programId = getProgramIdFromEnrollment($enrollDetails->master_enrollment_id);
	      		updateTPEnrollmentOverallPercentComplete($programId, $enrollDetails->master_enrollment_id,'','content_version');
	      	} 
      	}
      		
      }
      

		     $extsumm = db_select('slt_attendance_summary','summary');
		     $extsumm->addExpression('GROUP_CONCAT(summary.enrollment_id)','cnt');
		     $extsumm->condition('summary.content_version_id',$currDefaultVersion,'=');
		     $extsumm->condition('summary.enrollment_id',$enrollIds,'IN');
		     expDebug::dPrintDBAPI("Attendance summary Exist or not for the version",$extsumm);
		     $enrllId = $extsumm->execute()->fetchField();


		     $enrllId = explode(',',$enrllId);
		     $enrllIdUpdExist = array_intersect($enrollIds, $enrllId);
		     expDebug::dPrint('existing enroll to insert'.print_r($enrllIdUpdExist,1),4);
		     $enrllIdInsExist = array_diff($enrollIds, $enrllIdUpdExist);
		     expDebug::dPrint('existing enroll id to update '.print_r($enrllIdInsExist,1),4);
		      if(count($enrllIdInsExist)>0){
		      	$recFields = array('enrollment_id', 'user_id', 'course_id', 'class_id', 'last_attempt_date');

		      	$recquery = db_select('slt_attendance_summary', 'summ');
		      	$recquery->innerJoin('slt_enrollment','enr','enr.id = summ.enrollment_id');
		      	$recquery->fields('summ',$recFields);
		      	$recquery->addExpression($currDefaultVersion,'content_version_id');
		      	$recquery->addExpression($currDefaultLesson,'lesson_id');
		      	$recquery->addExpression("'lrn_crs_cmp_cmp'",'status');
		      	$recquery->addExpression('sum(summ.total_attempts)','total_attempts');
		      	$recquery->addExpression('sum(summ.total_attempts)','prevcont_attempt');
		      	$recquery->addExpression("'completed'",'content_status');
		      	$recquery->addExpression("'completed'",'completion_status');
		      	$recquery->addExpression($userId,'created_by');
		      	$recquery->addExpression("now()",'created_on');
		      	$recquery->addExpression($userId,'updated_by');
		      	$recquery->addExpression("now()",'updated_on');
		      	$recquery->condition('enr.id', $enrllIdInsExist,'IN');
		      	$recquery->condition('summ.content_version_id', $result1['version_id'],'=');
		      	$recquery->condition('enr.comp_status', 'lrn_crs_cmp_cmp','=');
		      	$recquery->isNotNull('summ.id');
		      	$recquery->groupBy('summ.enrollment_id');
		      	expDebug::dPrintDBAPI('Attendance Summary records', $recquery);
		      	
		      	db_insert('slt_attendance_summary')
		      	->from($recquery)
		      	->execute(); 
		      	expDebug::dPrintDBAPI('insert from ', db_insert('slt_attendance_summary')->from($recquery));
  }

		      if(count($enrllIdUpdExist)>0) {
		      	$enrllIdUpdExist = implode(',',$enrllIdUpdExist);
		      	 $updSumm = "UPDATE slt_attendance_summary sas
		      			INNER JOIN (SELECT summ.enrollment_id AS enrollment_id, summ.user_id AS user_id, 
		      			summ.content_version_id AS content_version_id ,summ.last_attempt_date AS last_attempt_date, 
		      			sum(summ.total_attempts) AS total_attempts
								FROM 
								slt_attendance_summary summ
								INNER JOIN slt_enrollment enr ON enr.id = summ.enrollment_id
								WHERE  (enr.id IN (:arg1 )) AND (summ.content_version_id = :vrs )
								AND (enr.comp_status = :cmpsts ) 
								GROUP BY summ.enrollment_id) b ON b.enrollment_id = sas.enrollment_id
		      			SET sas.total_attempts = (b.total_attempts) , 
		      			sas.prevcont_attempt = b.total_attempts - sas.total_attempts
		      			WHERE sas.content_version_id = :curver AND sas.enrollment_id IN (:arg1 ) AND sas.lesson_id = :curless "; 
		      	 $arg = array(':arg1'=>$enrllIdUpdExist,':cmpsts' => 'lrn_crs_cmp_cmp', 
		      	 		':vrs' => $result1['version_id'], ':curver' => $currDefaultVersion,':curless' => $currDefaultLesson);
		      	 expDebug::dPrintDBAPI("update attendance summary",$updSumm,$arg);
		      	 db_query($updSumm,$arg);

}

		      checkIfNotificationValid($contentId,$result);
		      //$enrCnt = getEnrollMapCount($contentId,$currDefaultVersion);
		      //$trsnUsrCnt = count($associateIdListArray);
		      /* if(empty($enrCnt)){
		      	drupal_set_message(t('LBL1039').' '.t('MSG774').'.');
		      }else{
		      if($trsnUsrCnt == 1)
		      	drupal_set_message($trsnUsrCnt.' '.t('MSG775').'.');
		      else */
		      	drupal_set_message(t('MSG774').'.');
		     // }
					$form_state['input']['hidden_idlist_'.$uniqueId] = '';
	      $form_state['rebuild'] = TRUE;
	    }
      } else {
      	// show a warning message, when a selected counts hits maximum level
      	//drupal_set_message(t('MSG790'));
      	 $totalResults=count($associateIdListArray);
      	expDebug::dPrint('Inside else case where total count exceeds config limit:: '.$totalResults,4);
      	// Check certification monitor job status
      	$jobname = 'content_transfer_attendance_summary';
      	$checkStatus     = check_monitor_jobs_status($jobname);
      	if($checkStatus['custom0']==0){
      		// Update monitoring jobs table to pick the process in next run
      		$update = db_update('slt_monitoring_jobs');
      		$update->fields(array(
      				'custom0'=>1,
      				'updated_on' => now()
      		));
      		$update->condition('job_name',$jobname,'=');
      		$update->execute();
      	}else{
      		//skip updating the monitor jobs table
      		 
      }	     	
      	$cTStatus = array('inprogress');
      	$inProgressJobs = fetchCTBackgroundList($cTStatus,'inlist',$contentId);
      	if($inProgressJobs==0){
      		$insertStmt = db_insert('slt_temp_background_jobs');
      		$jobId = date('dmyHis');
      		//order of params is content id, old version id ,lession id and new version id
      		$allIds = $contentId."--".$versionId."--".$currDefaultLesson."--".$currDefaultVersion;
      		
      		$form_state_params = $form_state['input']['contentmoveusers-autocomplete']."--".$form_state['input']['search_all_moveuser_type-hidden'];
      		$allIds .= "--".$form_state_params;
      		$rangeBegins = 0;
      		//$serializedData = serialize($result);
      		//CT -- Content Transfer
      		$insertStmt->fields(array(
      				'job_name' => 'CT-'.$jobId,
      				'created_by' => $userId,
      				'created_on' => now(),
      				'updated_by' => $userId,
      				'updated_on' => now(),
      				'custom1' => $allIds,
      				'custom2' => $hiddenIds,
      				'custom3' => $totalResults,
      				'custom4' => $rangeBegins,
      				'status' => 'new'
      		));
      		expDebug::dPrintDBAPI("slt_temp_background_jobs check",$insertStmt,5);
      		$insertStmt->execute();
      		// A message for bulk transfer is initiated and the user will be notified on completion
      		drupal_set_message(t('MSG992'));/*Viswanathan modified the MSG string for #77773*/
  }
      	
      	$form_state['input']['hidden_idlist_'.$uniqueId] = '';
      	$form_state['rebuild'] = TRUE; 
  }
  }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('moveUsersDetailsList', $ex);
    throw $ex;
  }
  syncSolrData('Enrollment');
  syncSolrData('MasterEnrollment');
  unset($txn); // Commit txn

}

function moveAllUsersDetailsList($form, &$form_state){
	
	$contentId = $form_state['content_id'];
	$versionId = $form_state['version_id'];
	$triggeringElement = $form_state['input']['_triggering_element_name'];
    	$currDefaultVersion = $form_state['triggering_element']['#ajax']['default_version'];
        $currDefaultLesson = $form_state['triggering_element']['#ajax']['default_lesson'];
    
    	$userId = getSltpersonUserId();
    	$configLimit = getConfigValue('content_move_all_users_limit');
    	expDebug::dPrint('content_move_all_users_limit variable from cnf: ' . $configLimit, 4);
    expDebug::dPrint(' getUsersQueryForVersionList param 3: ' . $form_state['input']['contentmoveusers-autocomplete'], 4);
    expDebug::dPrint(' getUsersQueryForVersionList param 4: ' . $form_state['input']['search_all_moveuser_type-hidden'], 4);
    $selectCount = getUsersQueryForVersionList($contentId,$versionId,$form_state['input']['contentmoveusers-autocomplete'],$form_state['input']['search_all_moveuser_type-hidden'],'searchCount');
    $totalResults 	= $selectCount->execute()->rowCount();
    expDebug::dPrint(' getUsersQueryForVersionList resultCount: ' . print_r($totalResults,true), 4);
    if($totalResults > $configLimit){
    	//else{
    		//mark the update action to be handled in backgroud cron job
    		expDebug::dPrint('Inside else case where total count exceeds config limit:: '.$totalResults,4);
    		// Check certification monitor job status
    		$jobname = 'content_transfer_attendance_summary';
    		$checkStatus     = check_monitor_jobs_status($jobname);
    		if($checkStatus['custom0']==0){
    			// Update monitoring jobs table to pick the process in next run
    			$update = db_update('slt_monitoring_jobs');
    			$update->fields(array(
    					'custom0'=>1,
    					'updated_on' => now()
    			));
    			$update->condition('job_name',$jobname,'=');
    			$update->execute();
    		}else{
    			//skip updating the monitor jobs table
    	
    		}
    		$cTStatus = array('inprogress');
    		$inProgressJobs = fetchCTBackgroundList($cTStatus,'inlist',$contentId);
    		if($inProgressJobs==0){
    			$insertStmt = db_insert('slt_temp_background_jobs');
    			$jobId = date('dmyHis');
    			//order of params is content id, old version id ,lession id and new version id
    			$allIds = $contentId."--".$versionId."--".$currDefaultLesson."--".$currDefaultVersion;
    			 
    			$form_state_params = $form_state['input']['contentmoveusers-autocomplete']."--".$form_state['input']['search_all_moveuser_type-hidden'];
    			$allIds .= "--".$form_state_params;
    			$rangeBegins = 0;
    			//$serializedData = serialize($result);
    			//CT -- Content Transfer
    			$insertStmt->fields(array(
    					'job_name' => 'CT-'.$jobId,
    					'created_by' => $userId,
    					'created_on' => now(),
    					'updated_by' => $userId,
    					'updated_on' => now(),
    					'custom1' => $allIds,
    					'custom2' => '',
    					'custom3' => $totalResults,
    					'custom4' => $rangeBegins,
    					'status' => 'new'
    			));
    			expDebug::dPrintDBAPI("slt_temp_background_jobs check",$insertStmt,5);
    			$insertStmt->execute();
    			// A message for bulk transfer is initiated and the user will be notified on completion
    			drupal_set_message(t('MSG992'));/*Viswanathan modified the MSG string for #77773*/
    		}
    		
    	
    		//}
    		$form_state['input']['hidden_idlist_'.$uniqueId] = '';
    		$form_state['rebuild'] = TRUE;
    	//}
    }//end of max configured limit exceeds
    else{
		$select = getUsersQueryForVersionList($contentId,$versionId,$form_state['input']['contentmoveusers-autocomplete'],$form_state['input']['search_all_moveuser_type-hidden'],'search');
		$resultExecute 	= $select->execute();
    	/* $totalResults 	= $resultExecute->rowCount();
	expDebug::dPrint('ctotal results count: ' . $totalResults, 4);
    	exit; */
	if ($totalResults > 0) {
				$result 		= $resultExecute->fetchAll();
		$hiddenIds = '';
		$enrollIds = '';
		foreach($result as $emapId){
			$enrollIds .= (empty($enrollIds) ? '' : ',').$emapId->enroll_id;
			$emapIds = str_replace('-',',',$emapId->emap_id);
					$hiddenIds .= (empty($hiddenIds) ? '' : ',').$emapIds;
									
					if(empty($form_state['input']['hidden_completedvalue_'.$uniqueId]) && $emapId->enroll_sts == 'Completed'){
						$form_state['input']['hidden_completedvalue_'.$uniqueId] = 1;
		}
				}
				if($form_state['input']['hidden_completedvalue_'.$uniqueId] == 1 && $form_state['input']['hidden_move_content_users_'.$contentId.'-'.$versionId] != 2){
				 $commands = array();
				 $commands[] = displayConfirmationmessagewizard($contentId, $versionId, $triggeringElement);
				 print ajax_render($commands);
				 exit;
				} 
				$associateIdListArray = explode(',', $hiddenIds);
			  $ienrIdArray = explode(',', $enrollIds);
		$tmp_SQL = "SELECT '".$currDefaultVersion."' as version_id, '".now()."' as updated_on, '".$userId."' as updated_by , ecm.id from";
		$tmp_SQL.= " slt_enrollment_content_mapping ecm where ecm.id IN (:arg1 )";
		$args = array(':arg1'=>$associateIdListArray);
		//expDebug::dPrintDBAPI("Building temporary table::",$tmp_SQL,$args);
		$temp_table= db_query_temporary((String)$tmp_SQL,$args);
		expDebug::dPrint('temporary table name : ' . print_r($temp_table,true), 4);
		$tmp_SQL2 = "SELECT enr.id from";
		$tmp_SQL2.= " slt_enrollment enr where enr.id IN (:arg1 )";
		$args = array(':arg1'=>$ienrIdArray);
		//expDebug::dPrintDBAPI("Building temporary table::",$tmp_SQL,$args);
		$temp_table2= db_query_temporary((String)$tmp_SQL2,$args);
		expDebug::dPrint('temporary table name : ' . print_r($temp_table2,true), 4);
		$txn = db_transaction();
		try{
			if($totalResults <= $configLimit){
				$updateVersion = "UPDATE slt_enrollment_content_mapping ecm
			      			INNER JOIN ".$temp_table." t ON t.id = ecm.id
			      			SET ecm.version_id = t.version_id ,
			      			ecm.updated_on = t.updated_on ,
			      			ecm.updated_by = t.updated_by ";
				$arg=array();
			    $updateVersion .= " WHERE t.id = ecm.id";
				//expDebug::dPrintDBAPI('QUERY DEFAULT ALLL---> ',$updateVersion);
				db_query($updateVersion,$arg);
      	// commented for 61786
		//foreach($result as $emapId){
				add_audit_trail_entry($userId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'moveUsersDetailsList', 'Transferring users from one content to another content',null,$versionId, $currDefaultVersion);
		//}
				$updateScore = "UPDATE slt_enrollment enr
			      			INNER JOIN ".$temp_table2." t ON t.id = enr.id
			      			SET enr.score = 0 ,
			      			enr.updated_on = :updOn ,
			      			enr.updated_by = :updBy ";
			    $updateScore .= " WHERE t.id = enr.id AND enr.comp_status != :compStatus "; // 0070552 - exclude completed class enrollments in score update
				$arg = array(':updOn' => now(),':updBy' => $userId,':compStatus' => 'lrn_crs_cmp_cmp');
				//expDebug::dPrintDBAPI('QUERY DEFAULT ALLL set score ===0---> ',$updateScore,arg);
				db_query($updateScore,$arg);
				expDebug::dPrint('total results---> '.$totalResults,4);
				expDebug::dPrint('config limit ===1---> '.$configLimit,4);
			      	
			    $extsumm = db_select('slt_attendance_summary','summary');
			    $extsumm->addExpression('GROUP_CONCAT(summary.enrollment_id)','cnt');
			    $extsumm->condition('summary.content_version_id',$currDefaultVersion,'=');
			    $extsumm->condition('summary.enrollment_id',$ienrIdArray,'IN');
			    expDebug::dPrintDBAPI("Attendance summary Exist or not for the version",$extsumm);
			    $enrllId = $extsumm->execute()->fetchField();
			    $enrllId = explode(',',$enrllId);
			    $enrllIdUpdExist = array_intersect($ienrIdArray, $enrllId);
			    expDebug::dPrint('existing enroll to insert'.print_r($enrllIdUpdExist,1),4);
			    $enrllIdInsExist = array_diff($ienrIdArray, $enrllIdUpdExist);
			    expDebug::dPrint('existing enroll id to update '.print_r($enrllIdInsExist,1),4);
			    if(count($enrllIdInsExist)>0){
			    	$recFields = array('enrollment_id', 'user_id', 'course_id', 'class_id', 'last_attempt_date');
			    	$recquery = db_select('slt_attendance_summary', 'summ');
			    	$recquery->innerJoin('slt_enrollment','enr','enr.id = summ.enrollment_id');
			    	$recquery->fields('summ',$recFields);
			    	$recquery->addExpression($currDefaultVersion,'content_version_id');
			    	$recquery->addExpression($currDefaultLesson,'lesson_id');
			    	$recquery->addExpression("'lrn_crs_cmp_cmp'",'status');
			    	$recquery->addExpression('sum(summ.total_attempts)','total_attempts');
			    	$recquery->addExpression('sum(summ.total_attempts)','prevcont_attempt');
			    	$recquery->addExpression("'completed'",'content_status');
			    	$recquery->addExpression("'completed'",'completion_status');
			    	$recquery->condition('enr.id', $enrllIdInsExist,'IN');
					$recquery->condition('summ.content_version_id', $versionId,'=');
			    	$recquery->condition('enr.comp_status', 'lrn_crs_cmp_cmp','=');
			    	$recquery->isNotNull('summ.id');
			    	$recquery->groupBy('summ.enrollment_id');
			    	expDebug::dPrintDBAPI('Attendance Summary records', $recquery);
					db_insert('slt_attendance_summary')->from($recquery)->execute();
					}//enrllIdInsExists>0
			    if(count($enrllIdUpdExist)>0) {
			    	$enrllIdUpdExist = implode(',',$enrllIdUpdExist);
			    	$updSumm = "UPDATE slt_attendance_summary sas
			      			INNER JOIN (SELECT summ.enrollment_id AS enrollment_id, summ.user_id AS user_id,
			      			summ.content_version_id AS content_version_id ,summ.last_attempt_date AS last_attempt_date,
			      			sum(summ.total_attempts) AS total_attempts
									FROM
									slt_attendance_summary summ
									INNER JOIN slt_enrollment enr ON enr.id = summ.enrollment_id
									WHERE  (enr.id IN (:arg1 )) AND (summ.content_version_id = :vrs )
									AND (enr.comp_status = :cmpsts )
									GROUP BY summ.enrollment_id) b ON b.enrollment_id = sas.enrollment_id
			      			SET sas.total_attempts = (b.total_attempts) ,
			      			sas.prevcont_attempt = b.total_attempts - sas.total_attempts
			      			WHERE sas.content_version_id = :curver AND sas.enrollment_id IN (:arg1 ) AND sas.lesson_id = :curless ";
			    	$arg = array(':arg1'=>$enrllIdUpdExist,':cmpsts' => 'lrn_crs_cmp_cmp',
			    			':vrs' => $result1['version_id'], ':curver' => $currDefaultVersion,':curless' => $currDefaultLesson);
			    	expDebug::dPrintDBAPI("update attendance summary",$updSumm,$arg);
			    	db_query($updSumm,$arg);
					}//enrllIdUpdExists>0
					checkIfNotificationValid($contentId,$result);
    				drupal_set_message(t('MSG774').'.');
				//}
				$form_state['input']['hidden_idlist_'.$uniqueId] = '';
				$form_state['rebuild'] = TRUE;
			}//$totalResults <= $configLimit
			    
	}catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('moveAllUsersDetailsList', $ex);
	    throw $ex;
  	}
  	unset($txn); // Commit txn
		
    		}//totalresult >0
    	
}
syncSolrData('Enrollment');	
  	}

function fetchCTBackgroundList($status,$condition='equals',$contentId=0,$jobName = 'CT-'){
	try {
		$select = db_select('slt_temp_background_jobs','tpj');
		if($condition == 'inlist'){
			$select->addExpression('count(tpj.id)','cnt');
			$select->condition('tpj.status', $status, 'IN');
		}elseif($condition == 'equals'){
			$select->addField('tpj','id','job_id');
			$select->addField('tpj','job_name','job_name');
			$select->addField('tpj','custom1','contentversionid');
			$select->addField('tpj','custom2','hiddenids');
			$select->addField('tpj','custom3','totalresults');
			$select->addField('tpj','custom4','rangebegins');
			$select->addField('tpj','created_by','userId');
			$select->condition('tpj.status', $status, '=');
		}
		if($contentId!=0){
			$select->condition('tpj.custom1', $contentId.'--%', 'LIKE');
		}
		$select->condition('tpj.job_name', '%'.db_like($jobName).'%', 'LIKE');
		if($condition == 'equals'){
			$select->orderBy('tpj.id');
			$select->range(0,1);
			expDebug::dPrintDBAPI('inside comes slt_temp_background_jobs',$select);
			$jobDetails = $select->execute()->fetchAll();
		}elseif($condition == 'inlist'){
			expDebug::dPrintDBAPI('inside comes slt_temp_background_jobs',$select);
			$jobDetails = $select->execute()->fetchField();
		}
		expDebug::dPrint("fetchCTBackgroundList::".print_r($jobDetails,1),4);
		return $jobDetails;
	}catch (Exception $ex) {
		watchdog_exception('fetchCTBackgroundList', $ex);
		throw $ex;
	}
}


function batchCTProcess(){
	$configLimit = getConfigValue('content_move_all_users_limit');
	$jobDetails = fetchCTBackgroundList('inprogress');
	expDebug::dPrint("fetchCTBackgroundList in progress::".print_r($jobDetails,1),4);
	if(count($jobDetails) == 0){
		// no jobs in progress
		$jobDetails = fetchCTBackgroundList('new');
		expDebug::dPrint("fetchCTBackgroundList in new::".print_r($jobDetails,1),4);
		if(count($jobDetails) == 0){
			// No jobs to be processed
			$jobCTName = 'content_transfer_attendance_summary';
			$checkCTStatus     = check_monitor_jobs_status($jobCTName);
			if($checkCTStatus['custom0']==1){
				// Update monitoring jobs table to turn off the handle
				$updatejobCTName = db_update('slt_monitoring_jobs');
				$updatejobCTName->fields(array(
						'custom0'=>0,
						'updated_on' => now()
				));
				$updatejobCTName->condition('job_name',$jobname,'=');
				$updatejobCTName->execute();
			}
			return;
		}// new == 0
	}// inprogress ==0
	expDebug::dPrint("fetchCTBackgroundList inside  in list case::".print_r($jobDetails,1),4);
	$job = $jobDetails[0];
	$contentVersionId = explode("--",$job->contentversionid);
	expDebug::dPrint("contentVersionId inside  in list case::".print_r($contentVersionId,1),4);
	$totalResultsCount = $job->totalresults;
	$userId = $job->userid;
	$jobId = $job->job_id;
	$stored_hiddenIds = $job->hiddenids;
	$start = $job->rangebegins;	
	$contentId = $contentVersionId[0];
	$versionId = $contentVersionId[1];
	$currDefaultLesson = $contentVersionId[2];
	$currDefaultVersion = $contentVersionId[3];
	$param3 = $contentVersionId[4];
	$param4 = $contentVersionId[5];
	$start = $job->rangebegins;
	if ($stored_hiddenIds != '' || $stored_hiddenIds != null) {
		expDebug::dPrint("Batch transfer for selected users begins -----------> ".print_r($stored_hiddenIds,true),4);
		// transfer selected users
		$associateIdListArray = explode(',', $stored_hiddenIds);
		expDebug::dPrint("assoc array -----------> ".print_r($associateIdListArray,true),4);
		if($associateIdListArray[0] == 0){
			$associateIdListArray = array_slice($associateIdListArray, 1);
		}
		if(count($associateIdListArray) > $configLimit){
			$newAssociatedIdListArray = implode(",",array_slice($associateIdListArray, $configLimit));
			$associateIdListArray = array_slice($associateIdListArray, $start,$configLimit);
			
		}else{
			$newAssociatedIdListArray = implode(",",array_slice($associateIdListArray, $configLimit));
		}
		if(count($associateIdListArray)>0){
			/*old code as is starts */

			expDebug::dPrint('ARRAY DEFASULT VERSION ----> '.$currDefaultVersion,4);
			//select mapping ids for audit trail
			$getUpdatedMapIds = db_select('slt_enrollment_content_mapping', 'ecm');
			$getUpdatedMapIds->addField('ecm', 'id');
			$getUpdatedMapIds->addField('ecm', 'version_id');
			$getUpdatedMapIds->condition('ecm.id', $associateIdListArray,'IN');
			$getUpdatedMapIds->condition('version_id', $currDefaultVersion, '!=');
			expDebug::dPrintDBAPI(' $getUpdatedMapIds', $getUpdatedMapIds);
			$result1 = $getUpdatedMapIds->execute()->fetchAssoc();
			expDebug::dPrint(' result from ecm'.print_r($result1, 5));
			
			if(count($result1) > 0){
					
				// commented for 61786
				// foreach ($result1 as $map) {
				add_audit_trail_entry($userId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'moveUsersDetailsList', 'Transferring users from one content to another content',
						null,$form_state['version_id'] , $currDefaultVersion );
				// }
				$updateVersion = db_update('slt_enrollment_content_mapping');
				$updateVersion->condition('id', $associateIdListArray,'IN');
				$updateVersion->fields(array(
						'version_id'      => $currDefaultVersion,
						'updated_on'    => now(),
						'updated_by'    => $userId
				));
				expDebug::dPrintDBAPI('QUERY DEFAULT ---> ',$updateVersion);
				$updateVersion->execute();
			
				/* added for notification trigger on moving users */
				$clsDetails = db_select("slt_course_class","cls");
				$clsDetails->join("slt_enrollment_content_mapping","emap","cls.id = emap.class_id");
				$clsDetails->join("slt_person","per","per.id = emap.user_id");
				$clsDetails->addField('emap','id','id');
				$clsDetails->addField('emap','user_id','user_id');
				$clsDetails->addField('per','first_name','first_name');
				$clsDetails->addField('emap','enroll_id','enroll_id');
				$clsDetails->addField('cls','title','class_title');
				$clsDetails->addField('cls','code','class_code');
				$clsDetails->condition('emap.id', $associateIdListArray,'IN');
				expDebug::dPrintDBAPI('QUERY DEFAULT beforeee ---> ',$clsDetails);
				$result = $clsDetails->execute()->fetchAll();
			
				$enrollIds = '';
				foreach($result as $emapId){
					$enrollIds .= (empty($enrollIds) ? '' : ',').$emapId->enroll_id;
				}
				$enrollIds = explode(',', $enrollIds);
				$updScore = db_update('slt_enrollment');
				$updScore->condition('id', $enrollIds,'IN');
				$updScore->condition('comp_status', 'lrn_crs_cmp_cmp','!='); // 0070552 - exclude completed class enrollments in score update
				$updScore->fields(array(
						'score'      => 0,
						'updated_on'    => now(),
						'updated_by'    => '1'
				));
				expDebug::dPrintDBAPI('QUERY DEFAULT set score ===0---> ',$updScore);
				$updScore->execute();
			
				$extsumm = db_select('slt_attendance_summary','summary');
				$extsumm->addExpression('GROUP_CONCAT(summary.enrollment_id)','cnt');
				$extsumm->condition('summary.content_version_id',$currDefaultVersion,'=');
				$extsumm->condition('summary.enrollment_id',$enrollIds,'IN');
				expDebug::dPrintDBAPI("Attendance summary Exist or not for the version",$extsumm);
				$enrllId = $extsumm->execute()->fetchField();
			
			
				$enrllId = explode(',',$enrllId);
				$enrllIdUpdExist = array_intersect($enrollIds, $enrllId);
				expDebug::dPrint('existing enroll to insert'.print_r($enrllIdUpdExist,1),4);
				$enrllIdInsExist = array_diff($enrollIds, $enrllIdUpdExist);
				expDebug::dPrint('existing enroll id to update '.print_r($enrllIdInsExist,1),4);
				if(count($enrllIdInsExist)>0){
					$recFields = array('enrollment_id', 'user_id', 'course_id', 'class_id', 'last_attempt_date');
			
					$recquery = db_select('slt_attendance_summary', 'summ');
					$recquery->innerJoin('slt_enrollment','enr','enr.id = summ.enrollment_id');
					$recquery->fields('summ',$recFields);
					$recquery->addExpression($currDefaultVersion,'content_version_id');
					$recquery->addExpression($currDefaultLesson,'lesson_id');
					$recquery->addExpression("'lrn_crs_cmp_cmp'",'status');
					$recquery->addExpression('sum(summ.total_attempts)','total_attempts');
					$recquery->addExpression('sum(summ.total_attempts)','prevcont_attempt');
					$recquery->addExpression("'completed'",'content_status');
					$recquery->addExpression("'completed'",'completion_status');
					$recquery->condition('enr.id', $enrllIdInsExist,'IN');
					$recquery->condition('summ.content_version_id', $result1['version_id'],'=');
					$recquery->condition('enr.comp_status', 'lrn_crs_cmp_cmp','=');
					$recquery->isNotNull('summ.id');
					$recquery->groupBy('summ.enrollment_id');
					expDebug::dPrintDBAPI('Attendance Summary records', $recquery);
					db_insert('slt_attendance_summary')
					->from($recquery)
					->execute();
				}
			
				if(count($enrllIdUpdExist)>0) {
					$enrllIdUpdExist = implode(',',$enrllIdUpdExist);
					$updSumm = "UPDATE slt_attendance_summary sas
		      			INNER JOIN (SELECT summ.enrollment_id AS enrollment_id, summ.user_id AS user_id,
		      			summ.content_version_id AS content_version_id ,summ.last_attempt_date AS last_attempt_date,
		      			sum(summ.total_attempts) AS total_attempts
								FROM
								slt_attendance_summary summ
								INNER JOIN slt_enrollment enr ON enr.id = summ.enrollment_id
								WHERE  (enr.id IN (:arg1 )) AND (summ.content_version_id = :vrs )
								AND (enr.comp_status = :cmpsts )
								GROUP BY summ.enrollment_id) b ON b.enrollment_id = sas.enrollment_id
		      			SET sas.total_attempts = (b.total_attempts) ,
		      			sas.prevcont_attempt = b.total_attempts - sas.total_attempts
		      			WHERE sas.content_version_id = :curver AND sas.enrollment_id IN (:arg1 ) AND sas.lesson_id = :curless ";
					$arg = array(':arg1'=>$enrllIdUpdExist,':cmpsts' => 'lrn_crs_cmp_cmp',
							':vrs' => $result1['version_id'], ':curver' => $currDefaultVersion,':curless' => $currDefaultLesson);
					expDebug::dPrintDBAPI("update attendance summary",$updSumm,$arg);
					db_query($updSumm,$arg);
			
				}
			
				checkIfNotificationValid($contentId,$result,'bulk');
				}
			
			/* old code as is ends */
		}
			/*code to update temp job and monitor jobs starts*/
				$newRange = $start + $configLimit;
				if ($totalResultsCount > $newRange) {
					$status = "inprogress";
					// Update the status once process is in progress
					$update = db_update ( 'slt_temp_background_jobs' );
					$update->fields ( array (
							'status' => $status,
							'custom4' => $newRange,
							'custom2' => $newAssociatedIdListArray,
							'updated_on' => now ()
					) );
					$update->condition ( 'id', $jobId, '=' );
					expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update in progress", $update );
					$update->execute ();
				}  // update status in progress
				else {
					$status = "completed";
					$updatedOn = now ();
					// Update the status once process is in progress
					$update = db_update ( 'slt_temp_background_jobs' );
					$update->fields ( array (
							'status' => $status,
							'updated_on' => $updatedOn,
							'custom2' => $newAssociatedIdListArray,
							'custom4' => $totalResultsCount
					) );
					$update->condition ( 'id', $jobId, '=' );
					expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update completed", $update );
					$update->execute ();
						
					// Send mail to the job initiator begins
					// Select table
					$selectStmt = db_select ( 'slt_person', 'prsn' );
					$selectStmt->addField ( 'prsn', 'first_name', 'first_name' );
					$selectStmt->addField ( 'prsn', 'full_name', 'full_name' );
					$selectStmt->addField ( 'prsn', 'email', 'email' );
					$selectStmt->condition ( 'prsn.id', $userId );
					$selectStmt->condition ( 'prsn.status', 'cre_usr_sts_atv' );
					$tokenResult = $selectStmt->execute ();
					$tokenResult = $tokenResult->fetchAssoc ();
					expDebug::dPrint ( ' $user email details result = ' . print_r ( $tokenResult, true ), 3 );
					$to = $tokenResult ['email'];
					$contentDetails = getAllContentDetails ( $contentId );
					$subj = 'Transfer of Users to another version of content, ' . $contentDetails ['title'] . ', is completed - ' . $updatedOn;
					// Transfer of Users to another version of content, <Content Name>, is completed
					// $attachmentFilePath = getConfigValue('dataload_file_path')."/".$selectResult[0]->file_name;
					$body = 'content_title>|' . $contentDetails ['title'] . '~|' .'version_number>|'.$versionId.'~|'. 'first_name>|' . $tokenResult ['first_name'] . '~|' . 'created_by>|' . $tokenResult ['full_name'] . '~|' . 'completed_on>|' . $updatedOn . '~|' . 'total_records>|' . $totalResultsCount . '~|';
					generateCTNotification ( $subj, $body, $tokenResult ['full_name'], $to, $result->created_by );
						
					$checkStatus = array (
							'new',
							'inprogress'
					);
					$jobCount = fetchCTBackgroundList ( $checkStatus, 'inlist' );
					if ($jobCount == 0) {
						$jobCTName = 'content_transfer_attendance_summary';
						$checkCTStatus = check_monitor_jobs_status ( $jobCTName );
						if ($checkCTStatus ['custom0'] == 1) {
							// Update monitoring jobs table to turn off the handle
							$update = db_update ( 'slt_monitoring_jobs' );
							$update->fields ( array (
									'custom0' => 0,
									'updated_on' => now ()
							) );
							$update->condition ( 'job_name', $jobCTName, '=' );
							$update->execute ();
						}
					}
				} // update status completed
			/*code to update temp job and monitor jobs ends*/
		//if (count($associateIdListArray) <= $configLimit ) {}
				expDebug::dPrint("Batch transfer for selected users ends -----------> ".print_r($stored_hiddenIds,true),4);
	}//transfer selected users ends 
	else {
		expDebug::dPrint("Batch transfer for all users begins -----------> ".print_r($jobId,true),4);
		// transfer all users
		
	$range = "0::".$configLimit;
	$select = getUsersQueryForVersionList($contentId,$versionId,$param3,$param4,'search',0,$range);
	$resultExecute 	= $select->execute();
	$totalResults 	= $resultExecute->rowCount();
	if ($totalResults > 0) {
		$result 		= $resultExecute->fetchAll();
		$hiddenIds = '';
		$enrollIds = '';
		foreach($result as $emapId){
			$enrollIds .= (empty($enrollIds) ? '' : ',').$emapId->enroll_id;
			$emapIds = str_replace('-',',',$emapId->emap_id);
			$hiddenIds .= (empty($hiddenIds) ? '' : ',').$emapIds;
		}
		$associateIdListArray = explode(',', $hiddenIds);
		$ienrIdArray = explode(',', $enrollIds);
			
			/**
			 * newly moved to batch mode begins *
			 */
		$tmp_SQL = "SELECT '".$currDefaultVersion."' as version_id, '".now()."' as updated_on, '".$userId."' as updated_by , ecm.id from";
		$tmp_SQL.= " slt_enrollment_content_mapping ecm where ecm.id IN (:arg1 )";
			$args = array (
					':arg1' => $associateIdListArray 
			);
		//expDebug::dPrintDBAPI("Building temporary table::",$tmp_SQL,$args);
			$temp_table = db_query_temporary ( ( string ) $tmp_SQL, $args );
		expDebug::dPrint('batch process temporary table name1 : ' . print_r($temp_table,true), 4);
		$tmp_SQL2 = "SELECT enr.id from";
		$tmp_SQL2.= " slt_enrollment enr where enr.id IN (:arg1 )";
			$args = array (
					':arg1' => $ienrIdArray 
			);
		//expDebug::dPrintDBAPI("Building temporary table::",$tmp_SQL,$args);
			$temp_table2 = db_query_temporary ( ( string ) $tmp_SQL2, $args );
		expDebug::dPrint('batch process temporary table name2 : ' . print_r($temp_table2,true), 4);

		$txn = db_transaction();
		try{
			$updateVersion = "UPDATE slt_enrollment_content_mapping ecm
		      			INNER JOIN ".$temp_table." t ON t.id = ecm.id
		      			SET ecm.version_id = t.version_id ,
		      			ecm.updated_on = t.updated_on ,
		      			ecm.updated_by = t.updated_by ";
			$arg=array();
			$updateVersion .= " WHERE t.id = ecm.id";
			expDebug::dPrintDBAPI('batch process QUERY ECM range version update---> ',$updateVersion);
			db_query($updateVersion,$arg);
			// commented for 61786
			//foreach($result as $emapId){
			add_audit_trail_entry($userId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'moveUsersDetailsList', 'Transferring users from one content to another content',null,$versionId, $currDefaultVersion);
			//}
			$updateScore = "UPDATE slt_enrollment enr
		      			INNER JOIN ".$temp_table2." t ON t.id = enr.id
		      			SET enr.score = 0 ,
		      			enr.updated_on = :updOn ,
		      			enr.updated_by = :updBy ";
			$updateScore .= " WHERE t.id = enr.id AND enr.comp_status != :compStatus "; // 0070552 - exclude completed class enrollments in score update
				$arg = array (
						':updOn' => now (),
						':updBy' => $userId,
						':compStatus' => 'lrn_crs_cmp_cmp' 
				);
			expDebug::dPrintDBAPI('batch process QUERY enr range set score ===0---> ',$updateScore,arg);
			db_query($updateScore,$arg);
			expDebug::dPrint('batch process total results---> '.$totalResults,4);
			expDebug::dPrint('batch process config limit ===1---> '.$configLimit,4);
				/**
				 * newly moved to batch mode ends *
				 */

			$extsumm = db_select('slt_attendance_summary','summary');
			$extsumm->addExpression('GROUP_CONCAT(summary.enrollment_id)','cnt');
			$extsumm->condition('summary.content_version_id',$currDefaultVersion,'=');
			$extsumm->condition('summary.enrollment_id',$ienrIdArray,'IN');
			expDebug::dPrintDBAPI("batchCTProcess::Attendance summary Exist or not for the version",$extsumm);
			$enrllId = $extsumm->execute()->fetchField();
			$enrllId = explode(',',$enrllId);
			$enrllIdUpdExist = array_intersect($ienrIdArray, $enrllId);
			expDebug::dPrint('batchCTProcess::existing enroll to insert'.print_r($enrllIdUpdExist,1),4);
			$enrllIdInsExist = array_diff($ienrIdArray, $enrllIdUpdExist);
			expDebug::dPrint('batchCTProcess::existing enroll id to update '.print_r($enrllIdInsExist,1),4);
			if(count($enrllIdInsExist)>0){
					$recFields = array (
							'enrollment_id',
							'user_id',
							'course_id',
							'class_id',
							'last_attempt_date' 
					);
				$recquery = db_select('slt_attendance_summary', 'summ');
				$recquery->innerJoin('slt_enrollment','enr','enr.id = summ.enrollment_id');
				$recquery->fields('summ',$recFields);
				$recquery->addExpression($currDefaultVersion,'content_version_id');
				$recquery->addExpression($currDefaultLesson,'lesson_id');
				$recquery->addExpression("'lrn_crs_cmp_cmp'",'status');
				$recquery->addExpression('sum(summ.total_attempts)','total_attempts');
				$recquery->addExpression('sum(summ.total_attempts)','prevcont_attempt');
				$recquery->addExpression("'completed'",'content_status');
				$recquery->addExpression("'completed'",'completion_status');
				$recquery->condition('enr.id', $enrllIdInsExist,'IN');
				$recquery->condition('summ.content_version_id', $versionId,'=');
				$recquery->condition('enr.comp_status', 'lrn_crs_cmp_cmp','=');
				$recquery->isNotNull('summ.id');
				$recquery->groupBy('summ.enrollment_id');
				expDebug::dPrintDBAPI('batchCTProcess::Attendance Summary records', $recquery);
				db_insert('slt_attendance_summary')->from($recquery)->execute();
			}//enrllIdInsExists>0
			if(count($enrllIdUpdExist)>0) {
				$enrllIdUpdExist = implode(',',$enrllIdUpdExist);
				$updSumm = "UPDATE slt_attendance_summary sas
				INNER JOIN (SELECT summ.enrollment_id AS enrollment_id, summ.user_id AS user_id,
						summ.content_version_id AS content_version_id ,summ.last_attempt_date AS last_attempt_date,
						sum(summ.total_attempts) AS total_attempts
						FROM
						slt_attendance_summary summ
						INNER JOIN slt_enrollment enr ON enr.id = summ.enrollment_id
						WHERE  (enr.id IN (:arg1 )) AND (summ.content_version_id = :vrs )
						AND (enr.comp_status = :cmpsts )
						GROUP BY summ.enrollment_id) b ON b.enrollment_id = sas.enrollment_id
				SET sas.total_attempts = (b.total_attempts) ,
				sas.prevcont_attempt = b.total_attempts - sas.total_attempts
				WHERE sas.content_version_id = :curver AND sas.enrollment_id IN (:arg1 ) AND sas.lesson_id = :curless ";
					$arg = array (
							':arg1' => $enrllIdUpdExist,
							':cmpsts' => 'lrn_crs_cmp_cmp',
							':vrs' => $result1 ['version_id'],
							':curver' => $currDefaultVersion,
							':curless' => $currDefaultLesson 
					);
				expDebug::dPrintDBAPI("batchCTProcess::update attendance summary",$updSumm,$arg);
				db_query($updSumm,$arg);
			}//enrllIdUpdExist>0
				checkIfNotificationValid ( $contentId, $result, 'bulk' );
			//update temp & jobs tables
			$newRange = $start + $configLimit;
			if($totalResultsCount > $newRange ){
				$status = "inprogress";
				// Update the status once process is in progress
				$update = db_update('slt_temp_background_jobs');
				$update->fields(array(
						'status'=> $status,
						'custom4' => $newRange,
						'updated_on' => now()
				));
				$update->condition('id',$jobId,'=');
				expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs status update in progress",$update);
				$update->execute();
			}//update status in progress
			else{
				$status = "completed";
				$updatedOn = now();
				// Update the status once process is in progress
				$update = db_update('slt_temp_background_jobs');
				$update->fields(array(
						'status'=> $status,
						'updated_on' => $updatedOn,
						'custom4' => $totalResultsCount
				));
				$update->condition('id',$jobId,'=');
				expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs status update completed",$update);
				$update->execute();
					
				//Send mail to the job initiator begins
				// Select table
				$selectStmt = db_select('slt_person', 'prsn');
				$selectStmt->addField('prsn', 'first_name', 'first_name');
				$selectStmt->addField('prsn', 'full_name', 'full_name');
				$selectStmt->addField('prsn', 'email', 'email');
				$selectStmt->condition('prsn.id', $userId);
				$selectStmt->condition('prsn.status', 'cre_usr_sts_atv');
				$tokenResult = $selectStmt->execute();
				$tokenResult = $tokenResult->fetchAssoc();
				expDebug::dPrint(' $user email details result = ' . print_r($tokenResult, true) , 3);
				$to = $tokenResult['email'];
				$contentDetails = getAllContentDetails($contentId);
				$subj = 'Transfer of Users to another version of content, '.$contentDetails['title'].', is completed - '.$updatedOn;
				//Transfer of Users to another version of content, <Content Name>, is completed
				//	$attachmentFilePath = getConfigValue('dataload_file_path')."/".$selectResult[0]->file_name;
					$body = 'content_title>|' . $contentDetails ['title'] . '~|' .'version_number>|'.$versionId.'~|'. 'first_name>|' . $tokenResult ['first_name'] . '~|' . 'created_by>|' . $tokenResult ['full_name'] . '~|' . 'completed_on>|' . $updatedOn . '~|' . 'total_records>|' . $totalResultsCount . '~|';
				generateCTNotification($subj,$body,$tokenResult['full_name'],$to,$result->created_by);
					
					$checkStatus = array (
							'new',
							'inprogress' 
					);
				$jobCount = fetchCTBackgroundList($checkStatus,'inlist');
				if($jobCount == 0){
					$jobCTName = 'content_transfer_attendance_summary';
					$checkCTStatus     = check_monitor_jobs_status($jobCTName);
					if($checkCTStatus['custom0']==1){
						// Update monitoring jobs table to turn off the handle
						$update = db_update('slt_monitoring_jobs');
						$update->fields(array(
								'custom0'=>0,
								'updated_on' => now()
						));
						$update->condition('job_name',$jobCTName,'=');
						$update->execute();
					}
				}
			}//update status completed
			}catch (Exception $ex){
				$txn->rollback();
				watchdog_exception('batchCTProcess', $ex);
				throw $ex;
			}
			unset($txn);
		}//$totalResults>0
		else{
			$status = "completed";
			// Update the status once process is in progress
			$update = db_update('slt_temp_background_jobs');
			$update->fields(array(
					'status'=> $status,
					'updated_on' => now()
			));
			$update->condition('id',$jobId,'=');
			expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs status update with 0 results completed",$update);
			$update->execute();
			$checkStatus = array (
					'new',
					'inprogress' 
			);
			$jobCount = fetchCTBackgroundList($checkStatus,'inlist');
			expDebug::dPrintDBAPI("batchCTProcess::slt_temp_background_jobs job count",$jobCount);
			if($jobCount == 0){
				$jobCTName = 'content_transfer_attendance_summary';
				$checkCTStatus     = check_monitor_jobs_status($jobCTName);
				if($checkCTStatus['custom0']==1){
					// Update monitoring jobs table to turn off the handle
					$update = db_update('slt_monitoring_jobs');
					$update->fields(array(
							'custom0'=>0,
							'updated_on' => now()
					));
					$update->condition('job_name',$jobname,'=');
					$update->execute();
				}
			}
		}
		expDebug::dPrint("Batch transfer for all users ends -----------> ".print_r($jobId,true),4);
	}
	syncSolrData('Enrollment');
}

	// Notification triggered once the content transfer background process completes.
	function generateCTNotification($subMsg='',$bodyMsg='',$name='',$emailId='',$userid='') {
		try {
			expDebug::dPrint("notification part starts".$fileName,5);
			$notificationInfo = array();

			$notificationInfo['tokens_string'] = 'subject>|' . $subMsg . '~|' . $bodyMsg;

			$notificationInfo['message_id'] = 'content_background_transfer';
			$notificationInfo['message_type'] = 'Content transfer job status to user';
			$notificationInfo['send_type'] = 'php mailer';
			$notificationInfo['lang_code'] = 'cre_sys_lng_eng';
			$setLanguage = 'cre_sys_lng_eng';
			expDebug::dPrint("email id check".$emailId,5);
			
			$notificationTemplateInfo = getNotificationTemplateInfo('content_background_transfer', 'cre_sys_lng_eng');
			
			// Notification for users whose Email Id given in the Job creation.
			if($emailId && $notificationTemplateInfo['status']=='cre_ntn_sts_atv'){
				$nid = db_insert('slt_notification') // Table name no longer needs {}
				->fields(array(
						'msg_id' => $notificationInfo['message_id'],
						'lang_code' => $setLanguage,
						'msg_type' => $notificationInfo['message_type'],
						'token_str' => $notificationInfo['tokens_string'],
						'user_id' => 1,
						'send_to_id' => $userid,
						'send_to_name' => $name,
						'send_to_email' => $emailId,
						'send_type' => $notificationInfo['send_type'],
						'send_status' => 'N',
						'attach_content' => '1',
						'created_by'=> $userid,
						'created_on' => now(),
						'updated_on' => now()
				))
				->execute();
			}
		} catch(Exception $e){
			expDebug::dPrint("ERROR in generateCTNotification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

function moveUsersCallBack($form, &$form_state){
	try {
  $contentId = $form_state['triggering_element']['#ajax']['content_id'];
  $versionId = $form_state['triggering_element']['#ajax']['version_id'];

  //expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
  $triggeringElementName = $form_state['triggering_element']['#name'];

  $commands = array();
  if ($triggeringElementName == 'Move_All') {
  	//$commands[] = ajax_command_invoke(NULL,  "closeMoveUsersQtip");
    //$commands[] = ajax_command_invoke(NULL,  "refreshVersionList", array($contentId));
    $commands[] = ajax_command_invoke(NULL,  "refreshMoveUsersQtip", array($contentId, $versionId));
  }
  else {
    $commands[] = ajax_command_invoke(NULL,  "refreshMoveUsersQtip", array($contentId, $versionId));
  }
  $popupId = '#show_expertus_message';
  if(empty($form_state['input']['hidden_idlist_'.$uniqueId]) && empty($form_state['hidden_idlist_'.$uniqueId])){
  	$popupId = '#paintContentqtip_visible_disp_moveusers_'.$contentId.'_'.$versionId .' #show_expertus_message';
  }
  $statusMsgHTML = theme('status_messages');
  //expDebug::dPrint(' $statusMsgHTML = with in call back ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  	// Show the messages
  	//$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
  	$commands[] = ajax_command_html($popupId, $statusMsgHTML);
  }

  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
  } catch (Exception $ex) {
  	watchdog_exception('moveUsersCallBack', $ex);
  	expertusErrorThrow($ex);
  }
}


function getDefaultContentVersion($contentId){
	try {
	  $select = db_select('slt_content_version', 'version');
	  $select->innerJoin('slt_content_lesson','les','les.content_version_id = version.id');
	  $select->addField('les', 'id','lesson_id');
	  $select->addField('version','id','version_id');
	  $select->addField('version','version','version_number');
	  $select->condition('version.content_master_id', $contentId);
	  $select->condition('version.status', 'lrn_cnt_sts_atv');
	  $select->groupBy('version.id');
	  $result = $select->execute();
	  expDebug::dPrintDBAPI('getDefaultContentVersion QUERY DEFAULT ---> ',$select);
	  return $result->fetchAll();
	}catch (Exception $ex) {
	  watchdog_exception('getDefaultContentVersion', $ex);
	  expertusErrorThrow($ex);
  	}
}

function getLatestVersionId(){
try {
	  $select = db_select('slt_content_version', 'version');
	  $select->addField('version','id','version_id');
	  $select->orderBy('version.id','DESC');
	  $select->range(0,1);
	  $result = $select->execute();
	  expDebug::dPrintDBAPI('getLatestVersionId QUERY ---> ',$select);
	  return $result->fetchField();
	}catch (Exception $ex) {
	  watchdog_exception('getLatestVersionId', $ex);
	  expertusErrorThrow($ex);
  	}
}

/*
	function to get the list of lessons for a content version
	--> to use parameters for preview of content
*/
function getLessonList($versionId){
	try {
	  $userId  = getIdOfLoggedInUser();
	  $select = db_select('slt_content_lesson', 'lesson');
	  $select->join('slt_content_version', 'version','lesson.content_version_id = version.id');
	  $select->join('slt_profile_list_items', 'lst','version.content_sub_type = lst.code');
	  $select->addField('lesson','id','lessonid');
	  $select->addField('lesson','title','title');
	  $select->addField('lesson','launchurl','launchurl');
	  $select->addField('version','content_sub_type','contentypecode');
	  $select->addField('version','hosted_type','contenthostedtype');
	  $select->addField('lst','name','contentype');
	  $select->condition('lesson.content_version_id',$versionId,'=');
	  $result = $select->execute()->fetchAll();
	  expDebug::dPrintDBAPI('getLessonList QUERY ---> ',$select);
	  expDebug::dPrint("result  getLessonList --> ".print_r($result,true),4);

	  include_once(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info.inc');
	  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	  $cdn_status  = getDrupalModuleStatus(array('cloud_files'));
	  expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' .print_r($cdn_status,true), 4);
	  $config=getConfig("exp_sp");
	  $mylearn_version 			= $config['mylearn_version'];
	  $content_conversion		= $config['content_conversion'];
	  $image_ext =  array('gif','png' ,'jpg', 'jpeg');
	  $i=0;
	  foreach($result as $response){
	  	$aiccId = $userId."-0-0-".$response->lessonid."-0-0-0";
	  	$enc1 = new Encrypt();
	  	$aiccId = $enc1->encrypt($aiccId);
	  	$result[$i]->AICC_SID = $aiccId;
	  	if($cdn_status == 1 && $result[$i]->contenthostedtype != '2'){
	  		$result[$i]->contenthostedtype = "3";
	  	}
	  	if($mylearn_version && $content_conversion){
	  		$new_launch_url = '';
	  		$old_launch_url = '';
  			$old_launch_url = $response->launchurl;
  			$copy_content_details = $response;
  			if($response->contentype=='Knowledge Content' && strpos($response->launchurl,'/files/contentupload')>0){
  				//find expention of upload file
  				$url_arr  = explode("/", $response->launchurl);
  				$filename = end($url_arr);
  				$file_arr = explode(".", $filename);
  				$ext = strtolower(end($file_arr));
  				if($ext == 'pdf' || in_array($ext, $image_ext))
  					$new_launch_url = $response->launchurl;
  				else{
  					$info = pathinfo($response->launchurl);
  					//$new_launch_url = str_replace($ext,'pdf',$content_details['launchurl']);
  					$new_launch_url = $info['dirname']. DIRECTORY_SEPARATOR	. $info['filename']	. '.'. 'pdf';
  					$new_launch_url = str_replace('contentupload','contentupload/convertioncontent',$new_launch_url);
  					$finfo = finfo_open(FILEINFO_MIME_TYPE);
  					$mime = finfo_file($finfo, $_SERVER['DOCUMENT_ROOT'].$new_launch_url);
  					$new_file_root_url = $config['pdf_temp_dir'].$new_launch_url;
  					if($mime != 'application/pdf' && !file_exists($new_file_root_url)){
  						$new_launch_url =$response->launchurl;
  						expDebug::dPrint('exist check comes here----------->'.print_r(file_exists($new_file_root_url),1),4);
  					}
  					expDebug::dPrint('FIND PDFT MIME TYPE URL----------->'.print_r($new_launch_url,1),4);
  				}
  				$result[$i]->launchurl =$new_launch_url;
//   				unset($copy_content_details->launchurl);
//   				$replace_key= array_search($old_launch_url, $copy_content_details);
//   				$result[$i][$replace_key] = $new_launch_url;
  			}
	  	}
	  	$i++;
	  }
	  expDebug::dPrint("result  getLessonList final--> ".print_r($result,true),4);
	  return   $result;

	}catch (Exception $ex) {
	  watchdog_exception('getLessonList', $ex);
	  expertusErrorThrow($ex);
  	}

}

/* Function to check if the name given to the content already exists */
function addEditContentValidate(&$form_state,$content_id,$mode,$title) {
  try {
  	  $form_state["storage"]["form_validate"] = true;
      $select = db_select("slt_content_master","cont");
      $select->addField('cont','id','cont_id');
      $select->addField('cont','code','cont_name');
      if(!empty($content_id)) {
       $select->condition('cont.id',array($content_id),'NOT IN');
      }

      $contList = $select->execute()->fetchAll();
      if($mode == "Edit"){
      	$cont_name        = $form_state['values']['content_title1'];
      }else{
      	$cont_name        = $title;
      }
      $cont_name_flag   = false;

      foreach($contList as $cont_list) {
       if(strtolower($cont_list->cont_name) == strtolower($cont_name)) {
          $cont_name_flag = true;
        }
      }
      if($cont_name_flag && $mode =="Edit") {
        form_set_error('content_title1', t('LBL949'));
        $form_state["storage"]["form_validate"] = false;
        $form_state['esign_show_popup']= false;
      }else if($cont_name_flag && $mode =="Add"){
      	return "exists";
      }
  } catch (Exception $ex) {
    watchdog_exception('addEditContentValidate', $ex);
    expertusErrorThrow($ex);
  }
}


function checkIfNotificationValid($contentId,$result,$notificationVol='regular'){
	try {
		/*
		 * Start # 0039858 -  Salesforce Chatter Feed
		* Added By : Ganesh Babu V, Oct 17th 2014 2:45 PM
		* Description: Send the notification parametre to post the feed to salesforce user according to notification type and notification code
		* Notification Type : Content Move Users
		* Ticket : #0039858: Implement the salesforce chatter feed in ExpertusONE
		*/

		if(module_exists('salesforce_integration')){
			module_load_include('inc','salesforce_integration','salesforce_integration_chatter');
			expDebug::dPrint("Salesforce_integration - Content Enrolled User result = ".print_r($result,true),4);
			$count_enr_cont_users=count($result);
			if($count_enr_cont_users>0){ //Get content enrollmed user list and send to saleforce to trigger feed
				foreach($result as $emapId){
					postSalesforceFeed('content_move_users','content_move_users',$contentId, $emapId->enroll_id, $emapId->user_id);
				}
			}
		}

		/* End # 0039858 -  Salesforce Chatter Feed */
			$notifyOverrideFlag = null;
    	$overrideContent = '';
    	$notificationCode = null;

		$overrideInfo = getEntityNotificationOverrideInfo($contentId, 'cre_sys_obt_cnt', 'content_move_users', 'Email');
        expDebug::dPrint(' contentmoveusers $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        if($overrideInfo != null) {
              $notifyOverrideFlag = $overrideInfo['notify_flag'];
              $overrideContent = $overrideInfo['override_content'];
        }
		$notificationTemplateInfo = getNotificationTemplateInfo('content_move_users', 'cre_sys_lng_eng');
		$notifyFlag = null;

  		// Both 'Y' and 'cre_ntn_sts_atv' are active statuses
  		if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
  			$notifyFlag = 'Y';
  		}
	    if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {  // Added
	  		$notifyOverrideFlag = 'Y';
  		}
	  	$frameString = true;
	  	if ($notifyFlag != 'Y' || ($notifyOverrideFlag != null && $notifyOverrideFlag != 'Y')) {
	  		if($notifyFlag != 'Y'){
	  			// If the notification is disabled status in template, do not generate notification
		    	expDebug::dPrint('Notification is disabled');
		    	$frameString = false;
	  		}else{
	  			//If the notification status has been overridden and set to disabled, do not generate notification.
	  			expDebug::dPrint(' Notification is overridded and disabled');
	  			$frameString = false;
	  		}
	  	}

	  	if($frameString){


	      	$notificationInfo = null;

	      	foreach($result as $notiInfo){
				  $notificationInfo['tokens_string'] = 'first_name>|' . $notiInfo->first_name . '~|'.
				                                       'class_title>|' . $notiInfo->class_title . '~|'.
				  										'class_code>|' . $notiInfo->class_code . '~|';
				  $notificationInfo['message_id']    = 'content_move_users';
				  $notificationInfo['message_type']  = 'Move Users To Content Version';
				  $notificationInfo['send_type']     = 'php mailer';
				  $notificationInfo['lang_code']     = '';
				  callCommonNotificationInsert('ContentMoveUsers',$notificationInfo,$notificationTemplateInfo,$notiInfo->user_id,$overrideContent,'',$notificationVol);

	      	}
	  	}

	  	} catch (Exception $ex) {
	  		watchdog_exception('ParseAICC', $ex);
	  		expertusErrorThrow($ex);
	  	}
}

function loadMoveUserDataGridType($mode, $type, $searchKeyword, $entityId, $entityType, $excludedId){
 try {
  $page      = $_GET['page'];
  $limit     = $_GET['rows'];
  $sidx      = $_GET['sidx'];
  $sord      = $_GET['sord'];
  $tab       = $_GET['tab'];
  $uniqueId  =  "$type-$entityId-$entityType";

  $xorderBy = 'user_name';
  //   37977: sorting option is missing to Username and Full name
  if($sidx !=''){
  	if($sidx == 'UserName') {
  		$xorderBy = 'user_name';
  	}else if($sidx == 'ClassTitle') {
  		$xorderBy = 'cls.title';
  	}else if($sidx == 'Status') {
  		$xorderBy = 'ests.name';
  	}
  	else {
  		$xorderBy = 'full_name';
  	}

  }
  expDebug::dPrint('$xorderBy'.$xorderBy);
  $queryRequired = 1;
  $fetchFields = array();

  $mode = empty($mode) ? 'view' : $mode;
  $countAvailable = 0;
  $count = 0;
  $searchKeyword= str_replace('|','/',$searchKeyword);

  /* case for retrieving users for the version for which the inprogress was clicked on */
  expDebug::dPrint('ContentMoveUsers  ---> '.$entityId ." ------ ".$entityType."-----".$queryRequired." ------ ".$searchKeyword );
  $searchType = $_GET['searhType'];
  $listquery = getUsersQueryForVersionList($entityId,$entityType,$searchKeyword,$searchType,'search');
  //$listquery = fetchVersionUsersList($entityId, $entityType, $searchKeyword);

  $count = count($listquery->execute()->fetchAll());


  /* Paging Related */
  $start = $limit * $page - $limit;
  $listquery->range($start, $limit);
  $listquery->orderBy($xorderBy,$sord);
  expDebug::dPrintDBAPI('grid query for enroll , enrolltp , content move users -----> AFTER QUERY EXEC ',$listquery);
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

  $srch_result_response_roster->total   = $total_pages;
  $srch_result_response_roster->records = $count;
  $srch_result_response_roster->page    = $page;
  $srch_result_response_roster->unique_id    = $uniqueId;

  $_SESSION['jq_count'] = array();
  $i=0;
  /* send data to move users qtip grid */
  expDebug::dPrint('grid data ContentMoveUsers  ---> '.print_r($queryResult,true),4);
  foreach ($queryResult as $rec_search_results){
    $srch_result_response_roster->rows[$i]['id']   = $rec_search_results->emap_id;
    $srch_result_response_roster->rows[$i]['cell'] = array(
         	 'id' 	              => $rec_search_results->user_id,
          	 'UserName' 	      => $rec_search_results->user_name,
             'FullName' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->full_name).'">'.titleController('MULTI-GRID-CONTENTMOVEUSERS-TITLE',$rec_search_results->full_name).'</span>',
    		'ClassTitle' 	      => '<span class="vtip" title="'.sanitize_data($rec_search_results->class_title).'">'.titleController('MULTI-GRID-CONTENTMOVEUSERS-CLASSTITLE',$rec_search_results->class_title).'</span>',
    		'Status' 	      => '<span class="vtip" title="'.sanitize_data(t($rec_search_results->enroll_sts)).'">'.titleController('MULTI-GRID-CONTENTMOVEUSERS-ENROLLMENTSTATUS',t($rec_search_results->enroll_sts)).'</span>',
             'MultiselectCheck'   => $rec_search_results->emap_id,
       );
       $i++;
    }


  echo drupal_json_encode($srch_result_response_roster);
  } catch (Exception $ex) {
  	watchdog_exception('ParseAICC', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
This function is for retrieving values for the data grid on first
load or search also for search by autocomplete and calculating count for pagination also
Arguments --- > Content Master Id, The Id of the version for which the In Progress
count is clicked on, the text in the autocomplete textbox
 and the value selected in the dropdown
*/
function getUsersQueryForVersionList($contentId,$versionId,$searchKeyword,$searchType,$callFor,$inclComp = 0,$range=0){
	try{
		$select = db_select('slt_enrollment', 'enroll');
		$select->join('slt_enrollment_content_mapping','emap','enroll.id = emap.enroll_id and enroll.course_id = emap.course_id and enroll.class_id = emap.class_id');
	    $select->join('slt_person','per','per.id = emap.user_id');
	    $select->innerJoin('slt_profile_list_items','ests','ests.code = enroll.comp_status');
	    if($callFor != "count" && $callFor != "otherversions"){
	    	$select->join('slt_course_class','cls','cls.id = emap.class_id and cls.course_id = emap.course_id');
	    }
	    if($callFor == "search"){
		    $select->addExpression('group_concat(emap.id SEPARATOR \'-\')','emap_id');
		    $select->addField('emap','user_id','user_id');
		    $select->addField('per','user_name','user_name');
		    $select->addField('per','full_name','full_name');
		    $select->addField('per','first_name','first_name');
		    $select->addField('cls','title','class_title');
		    $select->addField('cls','code','class_code');
		    $select->addField('enroll','id','enroll_id');
		    $select->addField('ests','name','enroll_sts');
	    }elseif($callFor == "autocomplete-search"){
	    	if($searchType == "user"){
	    		$select->addExpression('distinct(per.user_name)','name');
	    	}elseif($searchType == "classtitle") {
	    		$select->addExpression('distinct(cls.title)','name');
	    	}elseif($searchType == "status") {
	    		$select->addExpression('distinct(ests.name)','name');
	    	}
	    }elseif($callFor == "count" || $callFor == "searchCount"){
	    	$select->addExpression('count(1)','number_active');
	    }elseif($callFor == "otherversions"){
	    	$select->join('slt_content_version','ver','ver.id = emap.version_id');
	    	$select->distinct();
	    	$select->addField('ver','content_master_id','other_content_id');
		    $select->addField('ver','id','other_version_id');
		    $select->addField('ver','title','other_version_title');
		    $select->addField('ver','published_on','other_version_publishedon');
	    }
	    $select->condition('emap.content_id',$contentId,'=');
	    if($callFor == "otherversions"){
	    	$select->condition('emap.version_id',$versionId,'<>');
	    	$select->condition('ver.status','lrn_cnt_sts_del','<>');
	    }else{
	    	$select->condition('emap.version_id',$versionId,'=');
	    }
	    $select->condition('enroll.reg_status', array('lrn_crs_reg_cnf') ,'IN');
	    $compstsArry = ($inclComp == 1) ? array('lrn_crs_cmp_inp','lrn_crs_cmp_enr') : array('lrn_crs_cmp_inp','lrn_crs_cmp_enr','lrn_crs_cmp_cmp');
	    $select->condition('enroll.comp_status', $compstsArry ,'IN');

	    if($callFor != "count"){

		    $searchTypeVal = t('LBL181');
		    if($searchType == 'user'){
		      $searchTypeVal = t('LBL181');
		    }
		    elseif($searchType == 'classtitle'){
		      $searchTypeVal = t('LBL951');
		    }elseif($searchType == 'status'){
		      $searchTypeVal = t('LBL036'). ' ' .t('LBL102');
		    }

		    if($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
		       if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal ){
		        $select->condition(db_or()->condition('per.full_name', '%' . db_like($searchKeyword) . '%' , 'LIKE')
		                                ->condition('per.user_name', '%' . db_like($searchKeyword) . '%' , 'LIKE'));
		       }
		    }
			if($searchType == 'classtitle' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
		       if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal ){
		        $select->condition(db_or()->condition('cls.title', '%' . db_like($searchKeyword) . '%' , 'LIKE')
		                                ->condition('cls.code', '%' . db_like($searchKeyword) . '%' , 'LIKE'));
		       }
		    }
		    if($searchType == 'status' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
		    	if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal ){
		    		$select->condition('ests.name', '%' . db_like($searchKeyword) . '%' , 'LIKE');
	    }
		    }
	    }
	    $select->groupBy('emap.user_id');
	    $select->groupBy('emap.class_id');
	    $select->groupBy('ests.name');
	    expDebug::dPrint('range - QUERY FETCH VERSION = ' .$range, 4);
	    $pos = strpos($range, "::");
	    if ($pos !== false) {
	    	$range = explode("::", $range);
	    	expDebug::dPrint('range 2 - QUERY FETCH VERSION = ' .print_r($range,true), 4);
	    	$select->range($range[0],$range[1]);
	    }
	    expDebug::dPrintDBAPI("QUERY FETCH VERSION USERS getUsersQueryForVersionList --> ",$select);
	    return $select;
	}catch (Exception $ex) {
    	watchdog_exception('getUsersQueryForVersionList', $ex);
    	expertusErrorThrow($ex);
  	}


}

/*
	common function to get the onclick for content version preview
	-- single lesson versions
*/
function getOnclick($lessonList, $versionId, $holderId){
	try{
		$params = "";
	    $lessontitle = $lessonList[0]->title;
	    $launchurl =  $lessonList[0]->launchurl;
	    $contentType = $lessonList[0]->contentype;
	    $lessonId =  $lessonList[0]->lessonid;
	    $contentypeCode = $lessonList[0]->contentypecode;
	    $contenthostedtype = $lessonList[0]->contenthostedtype;
	    $aicc_sid = $lessonList[0]->AICC_SID;

	    include_once(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info.inc');
	    $cdn_status  = getDrupalModuleStatus(array('cloud_files'));
	    expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' .print_r($cdn_status,true), 4);
	    expDebug::dPrint('Knowledge Type Velu = ' . print_r($contentypeCode, true) , 3);
	     //This accept http:// and https://. Ticket #0025197

			//h5pcustomization
	     	// /?q=h5p/embed/263
	     	$h5pRef = substr($launchurl,0,13);
	     	expDebug::dPrint('Suresh URL...'.$launchurl." ====h5pRef:".$h5pRef);
	     	if($h5pRef == "/?q=h5p/embed" || $h5pRef== "?q=h5p/embed/")
	    	//if(strpos($launchurl,"h5p")>0)
	    	{
	    		//do not add protocol
	    		$launchurl = $launchurl."&preview=true";
	    	}
	    	else 
	    	if(($contentypeCode == 'lrn_cnt_typ_knc' || $contentypeCode == 'lrn_cnt_typ_vod') && ($contenthostedtype == '2' || $contenthostedtype == '3')){
	    		if (!preg_match("~^(?:ht)tps?://~i", $launchurl)) {
	    			$launchurl = "http://" . $launchurl;
	    		}
	    	 }

	     	if($h5pRef == "/?q=h5p/embed" || $h5pRef	== "?q=h5p/embed/")
			{
	    	 	$h5ptoken = drupal_get_token();
	    	 	$onclick = "href=\"javascript:void(0);\" onclick=\"launchAdminH5PPreview('".$launchurl."','".$contentypeCode."','".$h5ptoken."')\""." data={}";
	    	 
	    	 }	 
	    	else if($contentypeCode != "lrn_cnt_typ_vod"){
	    		 $params = "{'Id':'".$lessonId."','url1':'".rawurlencode(($launchurl))."','ErrMsg':'error','contentType':'".$contentType."','AICC_SID':'".$aicc_sid."','cdnmodulestatus':'".$cdn_status."'}";
	    		$onclick = 'onclick = "$(\'#root-admin\').data(\'narrowsearch\').launchLesson('.$params.')" ';
	    	}else{
	    		$popupVideoPreview = '{' .
	    				'  "id" : "video-preview-popup-' . $versionId . '"' . // HTML id given to the qtip popup box in
	    				// exp_sp_administration_content.module:previewVideoInQtipPopup().
	    		', "holderId" : "' . $holderId . '"' . // The div in which to render the qtip popup dialog box
	    		', "renderedHeight": 230' . // Height is set to auto for the qtip dialog box. Video player initialization
	    		// happens after the qtip is rendered. Therefore the final rendered height
	    		// is required to calculate the top of qtip box. This could have been coded directly into
	    		// the Drupal.ajax.prototype.commands.expVideoPreviewQtip().
	    		'}';
	    		 
	    		expDebug::dPrint('$popupVideoPreview = ' . $popupVideoPreview, 4);
	    		$onclick = ' href="/?q=ajax/administration/manage/content/preview/' . $versionId . '" data=\'' . $popupVideoPreview . '\' ';

	    	}
	    	expDebug::dPrint('$onclick = ' . print_r($onclick, true), 4);
	    	return  $onclick;
	}catch (Exception $ex) {
    	watchdog_exception('getOnclick', $ex);
    	expertusErrorThrow($ex);
  	}
}
function exp_sp_administration_viewcontent_addedit_form($js = NULL, $entityId){
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
								'width' => 680,
								'minHeight' => 350,
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
					array('id' => 'exp_sp_administration_view_contentdetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewcourse_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_contentdetails_addedit_form($form, &$form_state,$renderMode, $contentId){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$courseId---->'.$contentId);

		$wrapperId = 'viewcontent-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$contentdetails = getContentDetailsInfoView($contentId);

		expDebug::dPrint('$$$contentdetails-->'.print_r($contentdetails,true),5);
		$form['narrow_contentview'] = array(
				'#markup'       => theme('narrow_contentview', array('record' => $contentdetails)),
		);

		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_coursedetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}
function getContentDetailsInfoView($contentId) {
	try{
		$select = db_select('slt_content_master','mast');
		$select->leftJoin('slt_profile_list_items', 'slpi', 'mast.type = slpi.code');
		$select->leftJoin('slt_profile_list_items', 'prof', 'mast.lang_code = prof.code');
		$select->addField('mast','id','id');
		$select->addField('mast','code','code');
		$select->addField('mast','description','description');
		$select->addField('prof','name','lang');
		$select->addField('slpi','name','type');
		$select->condition('mast.id',$contentId,'=');
		expDebug::dPrintDBAPI('$select sabeena=',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('v$result$result$result->'.print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getContentDetailsInfoView', $ex);
		expertusErrorThrow($ex);
	}
}
function getVersionDetailsInfoView($contentId){
	try{
		expDebug::dPrint('$contentId->'.$contentId,4);
		$select = db_select('slt_content_version', 'version');
		$select->leftJoin('slt_person', 'person', 'person.id = version.created_by');
		$select->leftJoin('slt_content_master', 'mast', 'mast.id = version.content_master_id');
		$select->addField('version','id','version_id');
		$select->addField('version','title','version_title');
		$select->addField('version','created_by','version_creator');
		$select->addField('version','created_on','version_created');
		$select->addField('version','updated_by','version_updator');
		$select->addField('version','published_on','version_updated');
		$select->addField('person','full_name','created_by');
		$select->condition('version.content_master_id', $contentId, '=');
		$select->condition('version.status', 'lrn_cnt_sts_del', '!=');
		expDebug::dPrintDBAPI(' getAdminContentVersionRecords SQLLL ------>     ' ,  $select);
		$result =  $select->execute()->fetchAll();
		expDebug::dPrint(' query result ---- > '. print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getAdminContentVersionRecords', $ex);
		expertusErrorThrow($ex);
	}
}


function convertVideoForRed5Server($srcFileName,$desination_folder,$fileName,$ffmpeg_home, $flDir, $video_stream = false){
	try {
		$config=getConfig("exp_sp");
		$ffmpeg_servers	= explode(',',$config['ffmpeg_server_ip']);

		if(!empty($ffmpeg_servers[0])) {
			$table_name = 'slt_ffmpeg_queue';

			if (!db_table_exists($table_name)) {
				$table_schema = array(
						//description part
						'description' => 'ffmpeg queue processing maintaing table.',
						//added fields
						'fields' => array(
								'id' => array('type' => 'serial', 'not null' => TRUE),
								'timestamp' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE, 'default' => '0'),
								'folder_name'=> array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'name' => array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'bit_rate' => array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'user_id' => array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'user_email' => array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'user_preferred_language' => array('type' => 'varchar', 'length' => 500, 'not null' => TRUE, 'default' => ''),
								'progress_status' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE, 'default' => '0'),
						),
						'primary key' => array('id')
				);

				db_create_table($table_name, $table_schema);
			}

			if($video_stream) {
				$bit_rates = array('256', '512', '900');
			} else {
				$bit_rates = array('256');
			}
			$user_id = getIdOfLoggedInUser();
			$person_detail = getUserInformations($user_id);
			expDebug::dPrint(' new ffmpeg queue ....person id = ' . print_r($user_id, true) , 3);
			expDebug::dPrint(' new ffmpeg queue ....person details = ' . print_r($person_detail, true) , 3);
			if(empty($person_detail[preferred_language])) {
				$person_detail[preferred_language] = "cre_sys_lng_eng";
			}

			foreach($bit_rates as $key => $value) {
				if(!empty($fileName) && !empty($flDir)) {
					$insertStmt = db_insert('slt_ffmpeg_queue');
					$insertStmt->fields(array(
							'timestamp'  		=> time(),
							'folder_name'		=> $flDir,
							'name'       		=> $fileName,
							'bit_rate'	 		=> $value,
							'user_id'    	  	=> $user_id,
							'user_email'      	=> $person_detail[email],
							'user_preferred_language' => $person_detail[preferred_language],
							'progress_status' 	=> 0,

					));
					expDebug::dPrintDBAPI(' $insertStmt = ' , $insertStmt);
					$ffmpeg_queue = $insertStmt->execute();
				}
			}

		}

	} catch (Exception $ex) {
		watchdog_exception('ssh2 and conversion error', $ex);
		expertusErrorThrow($ex);
	}
}



function streamingOrNot($content_uploaded_path,$file_name, $conversion_format) {
	expDebug::dPrint("value for stream processStatus pathToFile1:$content_uploaded_path",4);
	expDebug::dPrint("value for stream processStatus pathToFile2:$file_name",4);

	$file_name = $file_name.'.mp4';
	$log_file_name = "ffmpeg_log.txt";
	$file_name_array = array();
	foreach($conversion_format as $key=>$value) {
		$file_name_array[$key] = drupal_realpath($content_uploaded_path.'/'.$value);
	}
	$flag = 0;
	$check_array = array();
	//check file available or not...
	foreach($file_name_array as $key => $value) {
		$path_to_file 		= $value .'/'. $file_name;
		$path_to_log_file	= $value .'/'. $log_file_name;
		if((fileReadableCheck($path_to_file) && (logFileCheck($path_to_log_file)))){
			$flag = 1;
			$check_array[$path_to_file] = $flag;
		}
		else {
			$flag = 0;
			$check_array[$path_to_file] = $flag;
		}
	}
	if(($flag == 1) &&(!in_array(0,$check_array))) {
		return true;
	}
	else {
		return false;
	}
}

function fileReadableCheck($pathToFile) {
	$filename = $pathToFile;
	$file = fopen($pathtofile, "r");
	$file_status =  fstat($file);

	$config=getConfig("exp_sp");
	$red5_server_path = $config['red5_server_path'];
	$red5_server_content_path = $config['red5_server_content_path'];

	$red5_server_contentupload_path = $red5_server_path.'/webapps/oflaDemo/streams'.$red5_server_content_path;
	expDebug::dPrint(" filename123:$pathToFile",4);
	expDebug::dPrint(" content path:$red5_server_contentupload_path",4);
	if (file_exists($pathToFile) && is_readable ($pathToFile)) {
		expDebug::dPrint("File can be readable path to file is : $pathToFile",4);
		return true;
	}else {
		expDebug::dPrint("File cannot  be readable path to file is:$pathToFile",4);
		return false;
	}
}


function logFileCheck($pathToFile) {
	expDebug::dPrint("File can be readable path to file is log file: $pathToFile",4);
	if (file_exists($pathToFile) && is_readable ($pathToFile)) {
		if(fileCheckString($pathToFile, 'successfully decoded'))
			return true;
		else
			return false;
	} else {
		return false;
	}
}

function fileCheckString($pathToFile, $searchFor) {
	expDebug::dPrint("check if string is available in that file...enterted.",4);
	$file = $pathToFile;
	$searchfor = $searchFor;

	// get the file contents, assuming the file to be readable (and exist)
	$contents = file_get_contents($file);
	// escape special characters in the query
	$pattern = preg_quote($searchfor, '/');
	// finalise the regular expression, matching the whole line
	$pattern = "/^.*$pattern.*\$/m";
	// search, and store all matching occurences in $matches
	if(preg_match_all($pattern, $contents, $matches)){
		expDebug::dPrint("Successfully Decoded",4);
		return true;
	}
	else{
		expDebug::dPrint("Log file pattern not match",4);
		return false;
	}
}

function displayConfirmationmessagewizard($contentId, $versionId, $trgElmt){
	try{
		return array(
				'command' => 'displayContentConfirmationmessagewizard',
				'cId' => $contentId,
				'cvId' => $versionId,
				'trgElmt' => $trgElmt
		);

	}catch (Exception $ex) {
		watchdog_exception('callCourseDetailWidget', $ex);
		expertusErrorThrow($ex);
	}
}

function getEnrollMapCount($contentId,$versionId){
	try{
		expDebug::dPrint("Content Id passed to the function".$contentId.'Version id'.$versionId);
		$getMapIdsCount = db_select('slt_enrollment_content_mapping', 'ecm');
		$getMapIdsCount->addExpression('COUNT(1)','emapcnt');
		$getMapIdsCount->condition('ecm.content_id', $contentId,'=');
		$getMapIdsCount->condition('version_id', $versionId, '!=');
		expDebug::dPrintDBAPI(' getEnrollMapCount', $getMapIdsCount);
		$result = $getMapIdsCount->execute()->fetchField();
		return $result;
		expDebug::dPrint("Count of enrollment mapping id not in current version".print_r($result,1));
	}catch(Exception $ex){

	}
}

?>
