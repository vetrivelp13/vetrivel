<?php

/**
 * Content Author Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
include_once $_SERVER["DOCUMENT_ROOT"] . "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_administration_content.inc";

/*
 * getContentAuthorAdminTabContentType() - For content admin this should be narrow-search to initialize filters, text filters, jgGrid, etc.
 */

function getContentAuthorAdminTabContentType()
{
    try {
        $jsObjectInfo['tab_content_type'] = 'narrow_search';
        return $jsObjectInfo;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorAdminTabContentType', $ex);
        expertusErrorThrow($ex);
    }
}


function getContentAuthorDetails($contentId)
{
    try {
        $select = db_select("slt_content_master", "scm");
        $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = scm.lang_code');
        $select->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = scm.status');
        $select->innerJoin('slt_content_version', 'scv', 'scv.content_master_id = scm.id');
        $select->innerJoin('slt_content_master_h5p_rel', 'h5prel', 'h5prel.content_master_id = scm.id');
        $select->innerJoin('h5p_nodes', 'h5p', 'h5p.content_id = h5prel.h5p_id');
        $select->addField('scm', 'id', 'id');
        $select->addField('scv', 'title', 'title');
        $select->addField('scm', 'code', 'code');
        $select->addField('scm', 'lang_code', 'language');
        $select->addField('scm', 'status', 'status');
        $select->addField('prof1', 'name', 'language_name');
        $select->addField('prof2', 'name', 'status_name');
        $select->addField('scm', 'description', 'short_description');
        $select->addField('scm', 'subtype', 'subtype');
        $select->addField('scv', 'title', 'video');
        $select->addField('h5p', 'json_content', 'interactions');
        $select->addField('scv', 'hosted_type', 'hostedtype');
        $select->condition('scm.id', $contentId, '=');
        expdebug::dPrintDBAPI('$select', $select);
        $contentFields = $select->execute()->fetchAll();
        expdebug::dPrint('$contentFields' . print_r($contentFields, true), 3);
        return $contentFields;
    }
    catch (Exception $ex) {
        watchdog_exception('getCourseDetail', $ex);
        expertusErrorThrow($ex);
    }
}


/*
 * getContentStatusFilters() - Get the status narrow search filters for content admin
 */
function getContentAuthorStatusFilters()
{
    try {
        $select = db_query('SELECT code, name FROM slt_profile_list_items ' . 'WHERE code like \'lrn_cnt_sts_%\' ' . 'AND code != \'lrn_cnt_sts_del\' ' . 'AND is_active = \'Y\' ' . 'ORDER BY FIELD(code, \'lrn_cnt_sts_atv\', \'lrn_cnt_sts_itv\')');
        $result = $select->fetchAllKeyed();
        
        // Return results
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorStatusFilters', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * getContentLanguageFilters() - Get the list of all languages currently used in content admin to show as narrow search filters
 */
function getContentAuthorLanguageFilters($langCode = 'cre_sys_lng_eng')
{
    // Select the table
    try {
        $select = db_select('slt_content_master', 'cont_mast');
        
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
        expDebug::dPrint('$result = ' . print_r($result, true), 3);
        
        // Return results
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorLanguageFilters', $ex);
        expertusErrorThrow($ex);
    }
}

function getContentAuthorTypeFilters($langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false)
{
    // Select the table
    try {
        $select = db_select('slt_profile_list_items', 'spli');
        
        // Select the fields to fetch
        $select->addField('spli', 'code', 'code');
        $select->addField('spli', 'name', 'name');
        
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
        expDebug::dPrint('$result = ' . print_r($result, true), 3);
        
        // Return results
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorTypeFilters', $ex);
        expertusErrorThrow($ex);
    }
}

function getContentAuthorVideoFilters()
{
    try {
        //Since it is in beta we are using pre-defined values.	
        $result                  = array();
        $result['h5p-video-mp4'] = MP4;
        
        $result['h5p-video-webm'] = WebM;
        
        $result['h5p-Youtube'] = Youtube;
        $result['h5p-Vimeo'] = Vimeo;
        
        // Return results
        return $result;
        
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorVideoFilters', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * contentAuthorNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function contentAuthorNarrowSearchFiltersHTML($subModule = null)
{
    try {
        $checkedStatus        = array();
        $checkedManagedStatus = array();
        $allStatus            = getContentAuthorStatusFilters();
        expDebug::dPrint('$allStatus = ' . print_r($allStatus, true), 4);
        
        $allLang = getContentAuthorLanguageFilters();
        expDebug::dPrint('$allLang = ' . print_r($allLang, true), 4);
        
        $allType = getContentAuthorTypeFilters();
        expDebug::dPrint('$allType = ' . print_r($allType, true), 4);
        
        $managedByOptions = getManagedByFilters();
        
        $videoTypes = getContentAuthorVideoFilters();
        
        
        $renderfiltersets   = array();
        // Comment the Type filter search box
        $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array(
            'code' => 'contentstatus',
            'title' => t('LBL102'),
            'divid' => 'status',
            'checkboxes_list' => $allStatus,
            'checked_list' => $checkedStatus
        ));
        
/*         if (count($allLang) > 1) {
            $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array(
                'code' => 'contentlang',
                'title' => t('LBL038'),
                'divid' => 'language',
                'checkboxes_list' => $allLang
            ));
        } */
        expDebug::dPrint('to see the submodule = ' . $subModule);
        if ($subModule == 'video') {
            $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array(
                'code' => 'contentauthor_videotype',
                'html_id' => 'contentauthor_videotype',
                'title' => t('Type'),
                'divid' => 'video type',
                'checkboxes_list' => $videoTypes
            ));
        }
        
        
        $renderfiltersets[] = theme('narrow_search_filterset_checkboxes', array(
            'code' => 'contentmanage',
            'html_id' => 'contentmanageby_container',
            'title' => t('Managed By'),
            'divid' => 'managed_by',
            'checkboxes_list' => $managedByOptions
        ));
     //Removing tags blocks from Content Author listing page.   
      /*   $weightedTags = getWeightedTags(array(
            'Content'
        ));
        if (!empty($weightedTags)) {
            $renderfiltersets[] = theme('narrow_search_filterset_tags', array(
                'code' => 'Contenttag',
                'html_id' => 'catalogclasstag_container',
                'title' => t('LBL191'), //Tags
                'tags_for_cloud' => $weightedTags
            ));
        } */
        
        
        
        
        
        $filterSetforJS   = array();
        $filterSetforJS[] = array(
            'type' => 'checkbox',
            'code' => 'contentstatus'
        );
        $filterSetforJS[] = array(
            'type' => 'checkbox',
            'code' => 'contentlang'
        );
        $filterSetforJS[] = array(
            'type' => 'checkbox',
            'code' => 'contenttype'
        );
        $filterSetforJS[] = array(
            'type' => 'checkbox',
            'code' => 'contentmanage'
        );
        $filterSetforJS[] = array(
            'type' => 'checkbox',
            'code' => 'contentauthor_videotype'
        );
        
        if (!empty($weightedTags)) {
            $filterSetforJS[] = array(
                "type" => "tagscloud",
                "code" => "Contenttag"
            );
        }
        
        return theme('narrow_search_filters_holder', array(
            'rendered_filtersets_list' => $renderfiltersets,
            'filter_set_list' => drupal_json_encode($filterSetforJS)
        ));
    }
    catch (Exception $ex) {
        watchdog_exception('contentAuthorNarrowSearchFiltersHTML', $ex);
        expertusErrorThrow($ex);
        
    }
}

/*
 * contentauthorSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function contentAuthorSearchResultsSortbarHTML($submodule)
{
    try {
        $sortbarLinksList   = array();
        $sortbarLinksList[] = array(
            'title' => t('LBL017'),
            'sort_type' => 'AZ',
            'html_id' => 'content-sort-az'
        );
        $sortbarLinksList[] = array(
            'title' => t('LBL018'),
            'sort_type' => 'ZA',
            'html_id' => 'content-sort-za'
        );
        $sortbarLinksList[] = array(
            'title' => t('LBL044'),
            'sort_type' => 'NewlyListed',
            'html_id' => 'content-sort-new'
        );
        
        $defaultSortOrder = array(
            'last_sort_type' => 'NewlyListed',
            'last_sort_type_html_id' => 'content-sort-new'
        );
        
        return theme('narrow_search_sortbar', array(
            'links_list' => $sortbarLinksList,
            'sortbar_list' => drupal_json_encode($defaultSortOrder), // Currently being ignored
            'title' => t('LBL011')
        ));
    }
    catch (Exception $ex) {
        watchdog_exception('contentauthorSearchResultsSortbarHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * contentauthorSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function contentAuthorSearchResultsActionbarHTML($submodule)
{
    try {
        if ($submodule == 'video')
            $pageAccess = 'cre_sys_obt_cnt_aut';
        if ($submodule == 'presentation')
            $pageAccess = 'cre_sys_obt_cnt_aut_pre';
        
        $actionbarLinksListforJS = array(
            'export_path' => 'administration/contentauthor/' . $submodule . '/export/',
            'print_path' => 'administration/contentauthor/' . $submodule . '/print/'
        );
        return theme('narrow_search_actionbar', array(
            'add_button_title' => t('LBL817').' '.t('LBL3018'),
            'add_page_path' => 'administration/contentauthor/' . $submodule . '/nojs/addedit',
            'show_print_button' => true,
            'show_export_button' => true,
            'show_add_button' => true,
            'ctools_style' => 'ctools-modal-ctools-admin-content-addedit-style',
            'actionbar_list' => drupal_json_encode($actionbarLinksListforJS),
            'page_code' => $pageAccess
        ));
    }
    catch (Exception $ex) {
        watchdog_exception('contentAuthorSearchResultsActionbarHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getContentAuthorAdminTabContentMainHTML - Returns the html for tab content main
 */
function getContentAuthorAdminTabContentMainHTML($submodule)
{
    try {
        $narrowsearchTabContentListforJS = array(
            'search_base_path' => 'administration/contentauthor/' . $submodule . '/search/all/',
            'show_top_text_filter' => true,
            'text_filter_ac_path' => 'administration/contentauthor/' . $submodule . '/textfilter-autocomplete'
        );
        
        return theme('narrow_search_tab_content', array(
            'narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabContentListforJS)
        ));
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorAdminTabContentMainHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getSurveyDetailsTabContent		  - Returns the html for narrow search block on content search page
 */
function getContentAuthorAdminTabContent($submodule)
{
    try {
        $tabContentTypeArr = getContentAuthorAdminTabContentType();
        $contentResponse   = array(
            'rendered_tab_content_type' => $tabContentTypeArr['tab_content_type'],
            'rendered_narrow_search_filters' => contentAuthorNarrowSearchFiltersHTML($submodule),
            'rendered_narrow_search_sortbar' => contentAuthorSearchResultsSortbarHTML($submodule),
            'rendered_narrow_search_actionbar' => contentAuthorSearchResultsActionbarHTML($submodule),
            'rendered_tab_content_main' => getContentAuthorAdminTabContentMainHTML($submodule),
            'rendered_main_div' => getAdminModules('administration', 'contentauthor')
        );
        echo drupal_json_encode($contentResponse);
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorAdminTabContent', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getContentAuthorTextFilterAutoComplete() - Returns the matched content codes
 */
function getContentAuthorTextFilterAutoComplete()
{
    try {
        // Select the table
        $select = db_select('slt_content_master', 'cont_mast');
        
        // Add fields to fetch
        $select->addField('cont_mast', 'code', 'code');
        $select->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
        
        $pos = strpos($_REQUEST["q"], "contentauthor/presentation/textfilter-autocomplete");
        if ($pos > 0) {
            $select->condition('cont_mast.type', 'lrn_cnt_typ_knc');
            $select->condition('cont_mast.subtype', 'h5p-presentatn');
        } else {
            $select->condition(db_or()->condition('cont_mast.subtype', 'h5p-video-mp4')->condition('cont_mast.subtype', 'h5p-video-webm')->condition('cont_mast.subtype', 'h5p-Youtube')->condition('cont_mast.subtype', 'h5p-Vimeo'));
            $select->condition('cont_mast.type', 'lrn_cnt_typ_vod');
        }
        
        selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array(
            "code",
            "description"
        ), "z");
        if ($pos > 0) {
        	filterByUserAccessPrivileges($select,'cont_mast','',1,'cre_sys_obt_cnt_aut_pre','attach','cont_mast'); // #83727 - group access visiblity check
        }else {
        	filterByUserAccessPrivileges($select,'cont_mast','',1,'cre_sys_obt_cnt_aut','attach','cont_mast'); // #83727 - group access visiblity check
        }
        // Order in dictionary sort order
        $select->orderBy('code');
        
        expDebug::dPrintDBAPI('$select', $select);
        
        $matchingContentCodesList = $select->execute()->fetchAll();
        expDebug::dPrint('$matchingContentCodesList = ' . print_r($matchingContentCodesList, true), 3);
        
        foreach ($matchingContentCodesList as $matchingContentCode) {
            print rawurlencode($matchingContentCode->code) . "\n";
        }
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorTextFilterAutoComplete', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getContentDetailsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getContentAuthorDetailsHTML($content)
{
    try {
        expDebug::dPrint('$content$content' . print_r($content, true), 4);
        
        $submodule = "video";
        if ($_REQUEST["q"] == "administration/contentauthor/presentation/search/all/")
            $submodule = "presentation";
        
        $detailsList = array();
        if (!empty($content->type)) {
            $detailsList[] = array(
                'detail' => t($content->type),
                'tooltip' => t('LBL036') . ': ' . $content->type
            );
        }
        if (!empty($content->lang)) {
            //$langchar = substr($content->lang, 0, 3); commented for new theme change
            $detailsList[] = array(
                'detail' => t($content->lang),
                'tooltip' => t('LBL038') . ': ' . $content->lang
            );
        }
        if (!empty($content->lesson_cnt) && $content->content_type != 'lrn_cnt_typ_vod') {
            $lessonCount   = t('LBL854') . ': ' . $content->lesson_cnt;
            $detailsList[] = array(
                'detail' => $lessonCount,
                'tooltip' => t('LBL854') . ': ' . $content->lesson_cnt
            );
        }
        
        
        $actionList[] = array(
            'action_link_text' => t('LBL063'),
            'action_page_path' => 'administration/contentauthor/' . $submodule . '/nojs/addedit',
            'action_button_params' => core_encrypt($content->id),
            'ctools_style' => 'ctools-modal-ctools-admin-content-addedit-style',
            'row_id' => $content->row,
            'manageby' => $content->manageby,
            'js_object_info' => array(
                'name' => 'narrowsearch', // provide the java script object name
                'init_id' => 'root-admin' // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => t('LBL063')
        );
        
        $actionList[]      = array(
            'action_link_text' => t('LBL286'),
            'action_page_path' => '',
            'action_button_params' => core_encrypt($content->id),
            'action_button_param_text' => 'Content',
            'ctools_style' => 'ctools-modal-ctools-admin-content-addedit-style',
            'js_object_info' => array(
                'name' => 'narrowsearch', // provide the java script object name
                'init_id' => 'root-admin' // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => t('LBL286')
        );
        $viewContentlist[] = array(
            'action_link_text' => 'Content Author',
            'action_page_path' => 'administration/contentauthor/' . $submodule . '/view-content/nojs/addedit/' . core_encrypt($content->id) . '',
            'action_button_params' => core_encrypt($content->id),
            'action_button_param_text' => $content->code,
            'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
            'js_object_info' => array(
                'name' => 'narrowsearch', // provide the java script object name
                'init_id' => 'root-admin' // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => t('view')
        );
        
        $content->entityType = 'cre_sys_obt_cnt';
        return theme('narrow-search-results-item-details', array(
            'title' => trim($content->code),
            'details_list' => $detailsList,
            'delete_info' => t('MSG357') . ' ' . strtolower(t('Content')),
            'description' => trim($content->description),
            'action_list' => $actionList,
            'view_list' => $viewContentlist,
            'entity_multi_action' => $content,
            'sumedit' => $content->sumedit,
            'sumdelete' => $content->sumdelete
        ));
        
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorDetailsHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getContentAuthorActionsHTML() - Renders content's actions as HTML to be displayed in jqGrid.
 */
function getContentAuthorActionsHTML($content)
{
    //	$content->object_type = 'Content';
    try {
        return theme('narrow-search-results-item-actions', array(
            'catalog_details' => $content
        ));
    }
    catch (Exception $ex) {
        watchdog_exception('getContentActionsHTML', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * contentAuthorSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function contentAuthorSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias)
{
    try {
        switch (getURLParam('sortby')) {
            case "AZ":
                $field     = $tableAlias . ".code";
                $direction = "ASC";
                break;
            
            case "ZA":
                $field     = $tableAlias . ".code";
                $direction = "DESC";
                break;
            
            case "NewlyListed":
                $field     = $tableAlias . ".created_on";
                $direction = "DESC";
                break;
            
            default:
                $field     = $tableAlias . ".created_on";
                $direction = "DESC";
                break;
        }
        
        $selectStmt->orderBy($field, $direction);
    }
    catch (Exception $ex) {
        watchdog_exception('contentAuthorSearchSelectStmtAddOrderByClause', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * sendContentListToJqGrid() - Prepares JSON response to JqGrid's request for Content Admin
 */
function sendContentAuthorListToJqGrid()
{
    try {
        
        
        expDebug::dPrint('page = "' . $_GET['page'] . '" ,' . 'rows = "' . $_GET['rows'] . '" ,' . 'sort(sidx) = "' . $_GET['sidx'] . '" ,' . 'order(sord) = "' . $_GET['sord'] . '" ,' . 'search = "' . $_GET['search'] . '" ,' . 'nd = "' . $_GET['nd'] . '" ,' . 'id = "' . $_GET['id'] . '" ,' . 'oper = "' . $_GET['oper'] . '" ,' . 'editoper = "' . $_GET['editoper'] . '" ,' . 'addoper = "' . $_GET['addoper'] . '" ,' . 'deloper = "' . $_GET['deloper'] . '" ,' . 'totalrows = "' . $_GET['totalrows'] . '" ,' . 'argumentType = "' . $argumentType . '" ,' . 'npage = "' . $_GET['npage'], 5);
        
        $page  = $_GET['page'];
        // get the requested page
        $limit = $_GET['rows'];
        
       /*  $numContent = fetchContentAuthorAll('COUNT');
        
        expDebug::dPrint('$numContent = ' . print_r($numContent, true), 4);
        
        $total_pages = 0;
        $start       = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numContent); 
        expDebug::dPrint('$start = ' . print_r($start, true), 4); */
        
        
        $start = calculate_offset($page, $limit);
        expDebug::dPrint('$start = ' . print_r($start, true), 4);
        
        $contentList = fetchContentAuthorAll('LISTS', $start, $limit);
        expDebug::dPrint('$contentList = ' . print_r($contentList, true), 4);
        
        // Prepare and send the response
        $response->page    = $page;
        $response->total   = $total_pages;
        $response->records = $numContent;
        
        $response->initial_sort_type_html_id = 'content-sort-new';
        
        $i            = 0;
        $contentList1 = $contentList[0];
        
        // Mananged By behavior related code
        $userData            = new stdClass();
        $userData->managedBy = $contentList[1];
        $response->userdata  = $userData;
        
        foreach ($contentList1 as $content) {
            expDebug::dPrint('$content = ' . print_r($content, true), 4);
            
            $content->row              = $i;
            $content->object_type      = 'Content Author';
            //$content -> object_type = 'Content';
            $content->object_type_code = 'cre_sys_obt_cnt';
            //$content -> subtype = 'h5p';
            
            $var = $content->id;
            expDebug::dPrint('AbhishekContentID---->' . $var);    
            $vid_type = $content->subtype;
            if ($vid_type == "h5p-video-mp4")
                $mime = "video-mp4";
            else if ($vid_type == "h5p-video-webm")
                $mime = "video-webm";
            else if ($vid_type == "h5p-Vimeo")
                $mime = "video-vimeo";
            else 
                $mime = "video-youtube";
            
            
            
        $pos = strpos($_REQUEST["q"], "contentauthor/presentation/search/all/");
            if ($pos > 0) {
                $content->type       = 'presentation';
                $content->video_type = 'contentAuthor-knowledge-cont';
                $content->title='Presentation';
            } else {
                $content->type       = 'video';
                if ($vid_type == "h5p-video-mp4")
                	$content->title = "Mp4 video";
                else if ($vid_type == "h5p-video-webm")
                	$content->title = "Webm video";
                	else if($vid_type == "h5p-Vimeo")
                	{
                	  $content->title = "Vimeo video";
                	}else {
                	$content->title = "Youtube video";
                	}
                $content->video_type = $mime;
                	
            }   
            
            
            $response->rows[$i]['id'] = $content->id;
            
            $response->rows[$i]['cell'] = array(
                'image' => theme('narrow-search-results-item-image', array(
                    'results' => $content
                )),
                'details' => getContentAuthorDetailsHTML($content),
                'action' => getContentAuthorActionsHTML($content)
            );
            
            $i++;
        }
        
        expDebug::dPrint('$response = ' . print_r($response, true), 4);
        
        echo drupal_json_encode($response);
        
    }
    catch (Exception $ex) {
        watchdog_exception('sendContentListToJqGrid', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * fetchContentAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchContentAuthorAll($op, $start, $limit, $callFrom = '')
{
    try {
        $userId = getSltpersonUserId();
        // Select table
        $select = db_select('slt_content_master', 'cont_mast');
        $select->leftJoin('slt_profile_list_items', 'pli_type', 'cont_mast.type = pli_type.code AND pli_type.lang_code = \'cre_sys_lng_eng\'');
        $select->leftJoin('slt_profile_list_items', 'pli_lang', 'cont_mast.lang_code = pli_lang.code AND pli_lang.lang_code = \'cre_sys_lng_eng\'');
        $select->leftJoin('slt_profile_list_items', 'pli_status', 'cont_mast.status = pli_status.code AND pli_status.lang_code = \'cre_sys_lng_eng\' ');
        $select->leftJoin('slt_content_version', 'cont_vers', 'cont_mast.id = cont_vers.content_master_id');
        $select->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
   
        expDebug::dPrint('submodule checkinggggg' . $submod);
        $pos = strpos($_REQUEST["q"], "contentauthor/presentation/search/all/");
        if ($pos > 0) {
            $select->condition('cont_mast.subtype', 'h5p-presentatn');
            $select->condition('cont_mast.type', 'lrn_cnt_typ_knc');
        } else {
            $select->condition(db_or()->condition('cont_mast.subtype', 'h5p-video-mp4')->condition('cont_mast.subtype', 'h5p-video-webm')->condition('cont_mast.subtype', 'h5p-Youtube')->condition('cont_mast.subtype', 'h5p-Vimeo'));
            $select->condition('cont_mast.type', 'lrn_cnt_typ_vod');
        }
        
        
        if ($op == 'COUNT') {
            // Get count of active survey
            $select->addExpression('COUNT(DISTINCT(cont_mast.id))', 'COUNT');
        } else {
      
            $select->addField('cont_mast', 'id', 'id');
            //$select -> addField('cont_mast', 'code', 'code');
            $select->addField('cont_vers', 'title', 'code');
            $select->addField('cont_mast', 'description', 'description');
            $select->addField('cont_mast', 'type', 'content_type');
            
            $select->addField('cont_mast', 'subtype', 'subtype');
            
            
            $select->addField('pli_type', 'name', 'type');
            $select->addField('pli_lang', 'name', 'lang');
            $select->addField('pli_status', 'name', 'status');
            $select->addField('pli_status', 'code', 'status_code');
            $select->addField('cont_vers', 'total_lesson', 'lesson_cnt');
            $select->addField('cont_vers', 'id', 'version_id');
            $select->groupBy('cont_mast.id');
        }
        expDebug::dPrint('$_GET[Contenttag]' . $_GET['Contenttag'], 5);
        if (!empty($_GET['Contenttag'])) {
            $select->leftjoin('slt_tag_entity', 'te', "cont_mast.id=te.entity_id and te.entity_type='Content'");
            $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
            $select->condition('td.tagname', rawurldecode($_GET['Contenttag']), '=');
            $select->groupBy('cont_mast.id');
        }
        //filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut', '', 'cont_mast');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'status', 'contentstatus');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'lang_code', 'contentlang');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'type', 'contenttype');
        
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'subtype', 'contentauthor_videotype');
        
        selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array(
            "code",
            "description"
        ));
        $select->condition('cont_vers.status', 'lrn_cnt_sts_atv', '=');
        //$select->groupBy('cont_vers.content_master_id');
        
        if ($pos > 0) {
            filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut_pre', '', 'cont_mast');
        } else {
            filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut', '', 'cont_mast');
        }
        
        // Check added by Vincent on Dec 2, 2013
        // Manage by fillter is not require for API calls
        $managefilter = '';
        if ($callFrom != 'API') {
            $managefilter = selectAddNarrowSearchManageByCondition('content', $select, 'cont_mast', 'contentmanage');
        }
        expDebug::dPrint('$managefilter = ' . $managefilter, 4);
        
        if ($op != "COUNT") {
            // Order the records
            contentAuthorSearchSelectStmtAddOrderByClause($select, 'cont_mast');
        }
        
        expDebug::dPrintDBAPI('$select $select before count', $select);
        
        if ($op == "COUNT") {
            $newResult = $select->execute()->fetchField();
            expDebug::dPrint('COUNT $result = ' . $newResult, 3);
            return $newResult;
        } elseif ($op == "CSV") {
            $colHeadersMap = array(
                t('LBL107') => 'code',
              //  t('LBL036') => 'type',
                t('Video').t(' ').t('LBL036') => 'subtype',
              //  t('LBL038') => 'lang',
                t('LBL102') => 'status',
             //   t('LBL854') => 'lesson_cnt'
            );
            $filterArray[] = array(
                t('Content') . ':' . textboxFiltersetValueToPrintableStr('textfilter')
            );
            $filterArray[] = array(
                t('LBL102') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentstatus')
            );
            /*$filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('contentlang'));*/
            $filterArray[] = array(
                t('LBL191') . ':' . textboxFiltersetValueToPrintableStr('Contenttag')
            );
            //$filterArray[] = array(t('LBL036') . ':' . checkboxFiltersetSelectionsToPrintableStr('contenttype'));
            
            $filterArray[] = array(
                t('LBL036') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentauthor_videotype')
            );
            
            
            $filterArray[] = array(
                ucfirst(t('LBL011')) . ':' . getNarrowSearchSortTitle()
            );
            $filterArray[] = array(
                t('Managed By') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentmanage')
            );
            SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'contentauthor_admin_search');
        } else if ($op == "PDF") {
            // % width of the columns must total to 100%
            $colHeadersMap = array(
                t('LBL107') => array(
                    'code',
                    40
                ),
                t('Video').t(' ').t('LBL036') => array(
                    'subtype',
                    33
                ),
             /*    t('LBL038') => array(
                    'lang',
                    15
                ), */
                t('LBL102') => array(
                    'status',
                    27
                ),
              /*   t('LBL854') => array(
                    'lesson_cnt',
                    10
                ) */
            );
            
            $appliedFilters = array(
                t('Content') => textboxFiltersetValueToPrintableStr('textfilter'),
                t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('contentstatus'),
                /*t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('contentlang'),*/
                t('LBL191') => textboxFiltersetValueToPrintableStr('Contenttag'),
                t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('contenttype'),
                ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
                t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('contentmanage')
            );
            
            SendNarrowSearchResultsAsPDFFile($select, t('Content'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'contentauthor_admin_search');
        } else {
        	add_row_id_condition($select, 'cont_mast');
            // Limit to $limit records
            $select->range($start, $limit);
            expDebug::dPrintDBAPI('$select with range set', $select);
            // Execure the query and fetch all the records
            $result       = $select->execute()->fetchAll();
            $newResult[0] = $result;
            $newResult[1] = $managefilter;
            expDebug::dPrint('LIST $result = ' . print_r($result, true), 3);
            return $newResult;
        }
    }
    catch (Exception $ex) {
        watchdog_exception('fetchContentAll', $ex);
        expertusErrorThrow($ex);
    }
}


function fetchContentAuthorPresentationAll($op, $start, $limit, $callFrom = '')
{
    try {
        $userId = getSltpersonUserId();
        // Select table
        $select = db_select('slt_content_master', 'cont_mast');
       
        $select->leftJoin('slt_profile_list_items', 'pli_type', 'cont_mast.type = pli_type.code AND pli_type.lang_code = \'cre_sys_lng_eng\'');
        $select->leftJoin('slt_profile_list_items', 'pli_lang', 'cont_mast.lang_code = pli_lang.code AND pli_lang.lang_code = \'cre_sys_lng_eng\'');
        $select->leftJoin('slt_profile_list_items', 'pli_status', 'cont_mast.status = pli_status.code AND pli_status.lang_code = \'cre_sys_lng_eng\' ');
        $select->leftJoin('slt_content_version', 'cont_vers', 'cont_mast.id = cont_vers.content_master_id');
        $select->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
  
        expDebug::dPrint('submodule checkinggggg' . $submod);
        $pos  = strpos($_REQUEST["q"], "contentauthor/presentation/search/all/");
        $pos1 = strpos($_REQUEST["q"], "contentauthor/video/search/all/");
        
        $select->condition('cont_mast.subtype', 'h5p-presentatn');
        $select->condition('cont_mast.type', 'lrn_cnt_typ_knc');
        
        
        
        if ($op == 'COUNT') {
            // Get count of active survey
            $select->addExpression('COUNT(DISTINCT(cont_mast.id))', 'COUNT');
        } else {
          
            $select->addField('cont_mast', 'id', 'id');
            //$select -> addField('cont_mast', 'code', 'code');
            $select->addField('cont_vers', 'title', 'code');
            $select->addField('cont_mast', 'description', 'description');
            $select->addField('cont_mast', 'type', 'content_type');
            
            $select->addField('cont_mast', 'subtype', 'subtype');
            
            
            $select->addField('pli_type', 'name', 'type');
            $select->addField('pli_lang', 'name', 'lang');
            $select->addField('pli_status', 'name', 'status');
            $select->addField('pli_status', 'code', 'status_code');
            $select->addField('cont_vers', 'total_lesson', 'lesson_cnt');
            $select->addField('cont_vers', 'id', 'version_id');
            $select->groupBy('cont_mast.id');
        }
        expDebug::dPrint('$_GET[Contenttag]' . $_GET['Contenttag'], 5);
        if (!empty($_GET['Contenttag'])) {
            $select->leftjoin('slt_tag_entity', 'te', "cont_mast.id=te.entity_id and te.entity_type='Content'");
            $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
            $select->condition('td.tagname', rawurldecode($_GET['Contenttag']), '=');
            $select->groupBy('cont_mast.id');
        }
        //filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut', '', 'cont_mast');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'status', 'contentstatus');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'lang_code', 'contentlang');
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'type', 'contenttype');
        
        selectAddNarrowSearchINCondition($select, 'cont_mast', 'subtype', 'contentauthor_videotype');
        
        selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array(
            "code",
            "description"
        ));
        $select->condition('cont_vers.status', 'lrn_cnt_sts_atv', '=');
        //$select->groupBy('cont_vers.content_master_id');
        
        if ($pos > 0) {
            filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut_pre', '', 'cont_mast');
        } else {
            filterByUserAccessPrivileges($select, 'cont_mast', '', 1, 'cre_sys_obt_cnt_aut', '', 'cont_mast');
        }
        
        // Check added by Vincent on Dec 2, 2013
        // Manage by fillter is not require for API calls
        $managefilter = '';
        if ($callFrom != 'API') {
            $managefilter = selectAddNarrowSearchManageByCondition('content', $select, 'cont_mast', 'contentmanage');
        }
        expDebug::dPrint('$managefilter = ' . $managefilter, 4);
        
        if ($op != "COUNT") {
            // Order the records
            contentAuthorSearchSelectStmtAddOrderByClause($select, 'cont_mast');
        }
        
        expDebug::dPrintDBAPI('$select $select before count', $select);
        
        if ($op == "COUNT") {
            $newResult = $select->execute()->fetchField();
            expDebug::dPrint('COUNT $result = ' . $newResult, 3);
            return $newResult;
        } elseif ($op == "CSV") {
            $colHeadersMap = array(
                t('LBL107') => 'code',
                t('LBL036') => 'type',
               // t('LBL038') => 'lang',
                t('LBL102') => 'status',
              //  t('LBL854') => 'lesson_cnt'
            );
            $filterArray[] = array(
                t('Content') . ':' . textboxFiltersetValueToPrintableStr('textfilter')
            );
            $filterArray[] = array(
                t('LBL102') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentstatus')
            );
            /*$filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('contentlang'));*/
            $filterArray[] = array(
                t('LBL191') . ':' . textboxFiltersetValueToPrintableStr('Contenttag')
            );
            //$filterArray[] = array(t('LBL036') . ':' . checkboxFiltersetSelectionsToPrintableStr('contenttype'));
            
            $filterArray[] = array(
                t('LBL036') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentauthor_videotype')
            );
            
            
            $filterArray[] = array(
                ucfirst(t('LBL011')) . ':' . getNarrowSearchSortTitle()
            );
            $filterArray[] = array(
                t('Managed By') . ':' . checkboxFiltersetSelectionsToPrintableStr('contentmanage')
            );
            SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'content_admin_search');
        } else if ($op == "PDF") {
            // % width of the columns must total to 100%
            $colHeadersMap = array(
                t('LBL107') => array(
                    'code',
                    40
                ),
                t('LBL036') => array(
                    'type',
                    33
                ),
               /*  t('LBL038') => array(
                    'lang',
                    15
                ), */
                t('LBL102') => array(
                    'status',
                    27
                ),
               /*  t('LBL854') => array(
                    'lesson_cnt',
                    10
                ) */
            );
            
            $appliedFilters = array(
                t('Content') => textboxFiltersetValueToPrintableStr('textfilter'),
                t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('contentstatus'),
                /*t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('contentlang'),*/
                t('LBL191') => textboxFiltersetValueToPrintableStr('Contenttag'),
                t('LBL036') => checkboxFiltersetSelectionsToPrintableStr('contenttype'),
                ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
                t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('contentmanage')
            );
            
            SendNarrowSearchResultsAsPDFFile($select, t('Content'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'content_admin_search');
        } else {
        	add_row_id_condition($select, 'cont_mast');
            // Limit to $limit records
            $select->range($start, $limit);
            expDebug::dPrintDBAPI('$select with range set', $select);
            // Execure the query and fetch all the records
            $result       = $select->execute()->fetchAll();
            $newResult[0] = $result;
            $newResult[1] = $managefilter;
            expDebug::dPrint('LIST $result = ' . print_r($result, true), 3);
            return $newResult;
        }
    }
    catch (Exception $ex) {
        watchdog_exception('fetchContentAuthorPresentationAll', $ex);
        expertusErrorThrow($ex);
    }
}


/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function contentauthor_admin_search_csv_row_data_alter($rowData)
{
    try {
        if ($rowData[0]->content_type == 'lrn_cnt_typ_vod') {
            $rowData[0]->lesson_cnt = '';
        }
        if ($rowData[0]->subtype == 'h5p-video-mp4') {
            $rowData[0]->subtype = "Mp4 video";
        }
        else if($rowData[0]->subtype == 'h5p-video-webm')
        {
            $rowData[0]->subtype = "Webm video";
        }
        else if($rowData[0]->subtype == 'h5p-Youtube')
        {
            $rowData[0]->subtype = "Youtube video";
        }
        else if($rowData[0]->subtype == 'h5p-Vimeo')
        {
            $rowData[0]->subtype = "Vimeo video";
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
function contentauthor_admin_search_pdf_row_data_alter($rowData)
{
    try {
        if ($rowData[0]->content_type == 'lrn_cnt_typ_vod') {
            $rowData[0]->lesson_cnt = '';
        }
        if ($rowData[0]->subtype == 'h5p-video-mp4') {
            $rowData[0]->subtype = "Mp4 video";
        }
        else if($rowData[0]->subtype == 'h5p-video-webm')
        {
            $rowData[0]->subtype = "Webm video";
        }
        else if($rowData[0]->subtype == 'h5p-Youtube')
        {
            $rowData[0]->subtype = "Youtube video";
        }
        else if($rowData[0]->subtype == 'h5p-Vimeo')
        {
            $rowData[0]->subtype = "Vimeo video";
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
function publishOrUnpublishContentAuthor($contentId)
{
    try {
        $currUserId = getSltpersonUserId();
        $status     = getContentAuthorStatus($contentId);
        
        $txn = db_transaction();
        try {
            $updateStmt    = db_update('slt_content_master');
            $updatedstatus = ($status == 'lrn_cnt_sts_atv') ? "lrn_cnt_sts_itv" : "lrn_cnt_sts_atv";
            
            $updateStmt->condition('id', $contentId);
            $updateStmt->fields(array(
                'status' => $updatedstatus,
                'updated_on' => now(),
                'updated_by' => $currUserId
            ));
            $updateStmt->execute();
            /* Update tag activity Count */
            updateTagActivityCount($contentId, 'Content', $updatedstatus);
            /* Update tag activity Count */
        }
        catch (Exception $ex) {
            $txn->rollback();
            watchdog_exception('change content status', $ex);
            throw $ex;
        }
        unset($txn);
        // Commit transaction
        
        add_audit_trail_entry($currUserId, $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'publishOrUnpublishContentAuthor', 'Changed Content Status', null, $status, $updatedstatus);
        
        $returnStatus = ($updatedstatus == 'lrn_cnt_sts_atv') ? t('LBL571') : t('LBL570');
        print $updatedstatus . '|' . $returnStatus;
    }
    catch (Exception $ex) {
        watchdog_exception('publishOrUnpublishContentAuthor', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * fucntion to retrive the current status of the content
 */
function getContentAuthorStatus($contentId)
{
    try {
        $select = db_select('slt_content_master', 'cont_mast');
        $select->addField('cont_mast', 'status', 'status');
        $select->condition('cont_mast.id', $contentId);
        expDebug::dPrintDBAPI('$select', $select);
        // Execute the query
        $contentStatus = $select->execute()->fetchField();
        expDebug::dPrint('$contentStatus = ' . $contentStatus, 3);
        return $contentStatus;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentAuthorStatus', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * addNewContenteDetails() - New Content Entry
 */
function addNewContentAuthorDetails($form, &$form_state, $createUserId, $callFromAPI = 0)
{
    $txn = db_transaction();
    expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
    expDebug::dPrint('$form_state values neww = ' . print_r($form_state['input'], true), 4);
    try {
        $insertStmt = db_insert('slt_content_master');
        $nfields    = contentAuthorFormatInputString($form, $form_state, $createUserId);
        $insertStmt->fields($nfields);
        expDebug::dPrintDBAPI('insertStmt', $insertStmt);
        $contentId = $insertStmt->execute();
        
        updateGroupMappingDetails($contentId, 'cre_sys_obt_cnt', getIdOfLoggedInUser(), 'emptyadd', 0); //for making entry in slt_mapped_entities for Report wizard
        
        expDebug::dPrint('contentId12322222 = ' . $contentId);
        $insertfields              = $form_state['values'];
        $insertfields[description] = $form_state['values']['short_description']['value'];
     //   $insertfields[code]        = $form_state['values']['content_data_url'];
        $insertfields[code]        = $form_state['values']['title'];
        
        
        
        include_once(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
        $cdn_status = getDrupalModuleStatus(array(
            'cloud_files'
        ));
        /* Update tag activity Count */
        //updateTagActivityCount($contentId, 'Content', $form_state['values']['status']);
        /* Update tag activity Count */
        if ($cdn_status == 1) {
            add_audit_trail_entry(getIdOfLoggedInUser(), $contentId, 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'getUploadContentData', 'Content Uploaded In CDN');
            make_audit_trail_entries($insertfields, getIdOfLoggedInUser(), null, $contentId, 'Inserted', 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'addNewContentAuthorDetails', array(
                'code',
                'content_type',
                'lang_code',
                'description'
            ));
        } else {
            make_audit_trail_entries($insertfields, getIdOfLoggedInUser(), null, $contentId, 'Inserted', 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'addNewContentAuthorDetails', array(
                'code',
                'content_type',
                'lang_code',
                'description'
            ));
        }
        
        // Save the new content's id in the hidden id form field.
        $form_state['values']['id'] = $contentId;
        
        /* added extra param mode to differentiate when
        the insert is happening - whetherthe version is the first
        or the second - inorder to update
        version number in content version table */
        insertMasterContentAuthor($form_state, $createUserId, "AddContent");
        $Vid_title                    = updateContentAuthorMasterTitle($form_state);
        //makeh5papicall($form,$form_state);
        $form_state['input']['title'] = $Vid_title;
        expDebug::dPrint('videotype2222222222 = ' . $Vid_title);
        
        
        return array(
            (object) array(
                'id' => $contentId,
                'title' => $Vid_title
            )
        );
    }
    catch (Exception $ex) {
        $txn->rollback();
        expDebug::dPrint("Error in addNewContenteDetails() exp_sp_administrion_content.inc " . print_r($ex, true));
        expDebug::dPrint("Error in addNewContenteDetails() exp_sp_administrion_content.inc " . $ex->__toString());
        watchdog_exception('AddContentAdmin', $ex);
        if ($callFromAPI) {
            //return array((object)array('id'=>'Failiure'));
            $errobj                  = new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode         = 'L_012';
            $errobj->errormsg        = "Content type Invalid";
            return $errobj;
            
        }
        throw $ex;
    }
    
    // Commit the transaction
    unset($txn);
}

function updateContentAuthorMasterTitle($form_state)
{
    
    try {
        expDebug::dPrint('$form_state[content_storage] = ' . print_r($form_state['content_storage'], true), 4);
        
        $i = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
     
        expDebug::dPrint('$form_state[content_storage]Abhishek = ' . print_r($form_state['content_storage'][$i]->title, true), 4);
        expDebug::dPrint('$form_state[content_storage]Abhishek = ' . print_r($form_state['content_storage'][$i]->orig_file_name, true), 4);
        $title  = $form_state['values']['title'];
       
        $exists = addEditContentAuthorValidate($form_state, $form_state['values']['id'], "Add", $title);
        $title  = ($exists == "exists") ? ($title . '-' . $form_state['values']['id']) : $title;
        expDebug::dPrint('UTF8 title = ' . $title, 4);
  
        $updateStmt = db_update('slt_content_master');
        expDebug::dPrint("title test..." . $form_state["values"]["video"] . "..uploadtype.." . $_SESSION["uploadtype"]);
        
        if ($_SESSION["uploadtype"] == "embed") {         
            $updateStmt->fields(array(
                'code' => $title
            ));
        } else {
            $updateStmt->fields(array(
                'code' => $title
            ));
        }
        
        $updateStmt->condition('id', $form_state['values']['id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
        $updateStmt->execute();
        
        $updateStmt = db_update('slt_content_version');
        expDebug::dPrint("title test..." . $form_state["values"]["video"] . "..uploadtype.." . $_SESSION["uploadtype"]);
        
        if ($_SESSION["uploadtype"] == "embed") {
            $updateStmt->fields(array(
                'title' => $title
            ));
        } else {
 
            $updateStmt->fields(array(
                'title' => $title
            ));
        }
        
        $updateStmt->condition('content_master_id', $form_state['values']['id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
        $updateStmt->execute();
        
        
        
        return $title;
        
        
    }
    catch (Exception $ex) {
        watchdog_exception('updateContentMasterTitle', $ex);
        expertusErrorThrow($ex);
    }
}

function updateContentAuthorDetails($form, &$form_state, $updateUserId, $callFromAPI = 0)
{
    expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true), 4);
    $txn = db_transaction();
    
    try {
        
        $defaultQryStr     = 'SELECT code, description, type as content_type, status from slt_content_master where id = :cid';
        $defaultQryStrArgs = array(
            ':cid' => $form_state['values']['id']
        );
        $defaultQry        = db_query($defaultQryStr, $defaultQryStrArgs);
        expDebug::dPrintDBAPI('$defaultQry', $defaultQry, $defaultQryStrArgs);
        $default = $defaultQry->fetchAll();
        expDebug::dPrint('$default = ' . print_r($default, true), 4);
        expDebug::dPrint('form state for save n publish check = ' . print_r($form_state, true), 4);
        $updateStmt = db_update('slt_content_master');
        $fields     = contentAuthorFormatInputString($form, $form_state, $updateUserId);
        $updateStmt->fields($fields);
        $updateStmt->condition('id', $form_state['values']['id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
        
        // Execute the update statement.
        $numUpdated     = $updateStmt->execute();
        /* Update tag activity Count */
        $triggerElement = $form_state['storage']['submit_triggering_element'];
        updateTagActivityCount($form_state['values']['id'], 'Content', $triggerElement);
        /* Update tag activity Count */
        /* during edit , only if the file upload happens or a url is added ,
        then insert a new record into content version */
        //if ($form_state['content_storage']) {
        //	insertMasterContentAuthor($form_state, $updateUserId, "EditContent");
        //}
        $trigger_element           = $form_state['storage']['submit_triggering_element'];
        $updatefields              = $form_state['values'];
        $updatefields[description] = $form_state['values']['short_description']['value'];
        $updatefields[code]       = $form_state['values']['title'];
        expDebug::dPrint('Abhishek Code1233' . print_r($updatefields, true), 4);
       /*  make_audit_trail_entries($updatefields, $updateUserId, $default, $form_state['values']['id'], 'Updated', 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'updateContentAuthorDetails', array(
            'content_type',
            'description',
            'title'
        )); */
        make_audit_trail_entries($updatefields, $updateUserId, $default, $form_state['values']['id'], 'Updated', 'cre_sys_obt_cnt', 'exp_sp_administration_contentauthor', 'updateContentAuthorDetails', array(
        		'description',
        		'code'
        ));
        
        $content_title = trim($form_state['values']['title']);
        if ($content_title != "" && !empty($content_title)) {
            updateContentAuthorLessonTitle($form_state);
        }
        
        if ($callFromAPI) {
            return array(
                (object) array(
                    'id' => $form_state['values']['id']
                )
            );
        }
    }
    catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('updateContentAuthorDetails', $ex);
        if ($callFromAPI) {
            //return array((object)array('id'=>'Failiure'));
            $errobj                  = new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode         = 'L_012';
            $errobj->errormsg        = "Content Details Invalid";
            return $errobj;
        }
        throw $ex;
    }
    
    // Commit the transaction
    unset($txn);
    
}

function updateContentAuthorLessonTitle($form_state)
{
    try {
        $title = $form_state['values']['title'];
        $title = empty($title) ? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
        expDebug::dPrint('UTF-8 $title = ' . $title, 4);
        
        
        
        $updateStmt = db_update('slt_content_master');
        $updateStmt->fields(array(
            'code' => $title
        ));
        $updateStmt->condition('id', $form_state['values']['id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
        $updateStmt->execute();
        
        
        /* Ranjani -- commented out to not edit title in version */
        $updateVersionStmt = db_update('slt_content_version');
        
        $updateVersionStmt->fields(array(
            'title' => $title
        ));
        $updateVersionStmt->condition('content_master_id', $form_state['values']['id']);
        expDebug::dPrintDBAPI('$updateVersionStmt', $updateVersionStmt);
        $updateVersionStmt->execute();
    }
    catch (Exception $ex) {
        watchdog_exception('updateContentLessonTitle', $ex);
        expertusErrorThrow($ex);
    }
}

function contentAuthorFormatInputString($form, &$form_state, $createUserId)
{
    try {
        $nullVal         = NULL;
        //	$trigger_element = $form_state['storage']['_triggering_element_name'];
        $trigger_element = $form_state['input']['_triggering_element_name'];
        //	$trigger_element = 'saveandpublish';
        $content_status  = '';
        expDebug::dPrint('the save type ::' . $trigger_element);
        if ($trigger_element == 'saveandpublish') {
            $content_status = 'lrn_cnt_sts_atv';
        } else if ($trigger_element == 'save') {
            $content_status = 'lrn_cnt_sts_itv';
        } else if ($trigger_element == 'unpublish') {
            $content_status = 'lrn_cnt_sts_itv';
        } else if ($form_state['values']['status']) {
            $content_status = $form_state['values']['status'];
        }
        $form_state['values']['content_type'] = "lrn_cnt_typ_vod";
        
        $fields = array(
            //'subtype'               => 'h5p',
            'subtype' => updateContentAuthorSubtype($form_state),
            'type' => $form_state['values']['content_type'],
            'lang_code' => 'cre_sys_lng_eng',
            'description' => $form_state['values']['short_description']['value'],
            'status' => $content_status,
            'active' => 'Y',
            'custom0' => $nullVal,
            'custom1' => $nullVal,
            'custom2' => $nullVal,
            'custom3' => $nullVal,
            'custom4' => $nullVal
        );
        if ($form_state['values']['id']) {
            $fields['updated_by'] = $createUserId;
            $fields['updated_on'] = now();
        } else {
            $fields['created_by'] = $createUserId;
            $fields['created_on'] = now();
            $fields['updated_by'] = $createUserId;
            $fields['updated_on'] = now();
            $fields['code']       = '';
        }
        
        if ($trigger_element == 'unpublish') {
            $content_status = 'lrn_cnt_sts_itv';
            $fields         = array(
                'status' => $content_status
            );
        }
        expDebug::dPrint('$fields = ' . print_r($fields, true), 4);
        return $fields;
    }
    catch (Exception $ex) {
        watchdog_exception('contentFormatInputString', $ex);
        expertusErrorThrow($ex);
    }
}

function insertMasterContentAuthor(&$form_state, $createUserId, $mode)
{
    try {
        //For inserting the video
        $form_state['values']['content_type'] = "lrn_cnt_typ_vod";
        $form_state['values']['lang_code']    = "cre_sys_lng_eng";
        
        expDebug::dPrint("insertMasterContentAuthor -- >" . print_r($form_state['content_storage'], true), 4);
        $totalContent   = "1"; // count($form_state['content_storage']);
        $status         = ($mode == "AddContent") ? 'lrn_cnt_sts_atv' : 'lrn_cnt_sts_itv';
        //getContentStatus($form_state['values']['id']);
        $nullVal        = NULL;
        $contentDataIdx = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
        if (empty($form_state['content_storage'][$contentDataIdx]->orig_file_name)) {
            // Added by Vincent on May 10, 2013 for Scorm and AICC version title should be the uploaded file name instead of lesson title #0023551
            if ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic' || $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_s12' || $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_s24' || $form_state['values']['content_type'] == 'lrn_cnt_typ_srm_tnc') {
                $tname  = ($form_state['content_storage'][0]->file_name) ? $form_state['content_storage'][0]->file_name : $form_state['content_storage'][1]->file_name;
                $title1 = explode('_', $tname);
                $title  = $title1[0];
                for ($i = 1; $i < sizeOf($title1) - 1; $i++) {
                    $title = $title . '_' . $title1[$i];
                }
                $title = $title . '.zip';
            } else {
                $title = ($form_state['content_storage'][0]->title) ? $form_state['content_storage'][0]->title : $form_state['content_storage'][1]->title;
            }
        } else {
            $title = $form_state['content_storage'][$contentDataIdx]->orig_file_name;
        }
        
        if ($_SESSION["uploadtype"] == "embed")
            $title = $form_state["values"]["title"];
        else
            $title = $form_state["values"]["title"];
        
        
        $title                = empty($title) ? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
        $insertContentVersion = db_insert('slt_content_version');
        $sizePath             = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
        $maxVersion           = db_select('slt_content_version', 'version');
        $maxVersion->addExpression('max(version)', 'max_version');
        $maxVersion->condition('version.content_master_id', $form_state['values']['id']);
        $existingVersion = $maxVersion->execute()->fetchField();
        expDebug::dPrintDBAPI(' max version query --------> ' . $maxVersion);
        if ($_SESSION["uploadtype"] == "upload") {
            $form_state['content_storage'][0]->hostedtype = "1";
            expDebug::dPrintDBAPI(' sureshh:' . print_r($form_state['content_storage'], true));
            
            //$form_state['values']['video'] = $form_state['content_storage'][0] -> title;
            
        } else
            $form_state['content_storage'][0]->hostedtype = "2";
        
        $fields = array(
            'content_master_id' => $form_state['values']['id'],
            'content_sub_type' => $form_state['values']['content_type'],
            'title' => $title,
            'hosted_type' => empty($form_state['content_storage'][0]->hostedtype) ? 1 : $form_state['content_storage'][0]->hostedtype,
            'total_lesson' => $totalContent,
            'version' => ($existingVersion + 1),
            'overall_mastery_score' => empty($form_state['content_storage'][0]->masteryscore) ? 0 : $form_state['content_storage'][0]->masteryscore,
            'status' => $status,
            'published_by' => ($mode == "AddContent") ? $createUserId : null,
            'published_on' => ($mode == "AddContent") ? now() : null,
            'file_size' => $form_state['content_storage'][$sizePath]->file_size,
            'file_path' => $form_state['content_storage'][$sizePath]->file_path,
            'created_by' => $createUserId,
            'created_on' => now(),
            'updated_by' => $createUserId,
            'updated_on' => now(),
            'custom0' => $nullVal,
            'custom1' => $nullVal,
            'custom2' => $nullVal,
            'custom3' => $nullVal,
            'custom4' => $nullVal
        );
        $insertContentVersion->fields($fields);
        expDebug::dPrintDBAPI('$insertContentVersion', $insertContentVersion);
        $contentVersionId = $insertContentVersion->execute();
        make_audit_trail_entries(array(
            'version' => ($existingVersion + 1)
        ), getIdOfLoggedInUser(), $existingVersion, $form_state['values']['id'], 'Inserted', $form_state['values']['content_type'], 'exp_sp_administration_contentauthor', 'insertMasterContentAuthor', array(
            'version'
        ));
        $i = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
        
        $totalContent = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? ($totalContent + 1) : $totalContent;
        expDebug::dPrintDBAPI('suresh $totalContent', $totalContent);
        
        //insert into slt_content_lesson
        for ($i; $i < $totalContent; $i++) {
            $insertContentLesson = db_insert('slt_content_lesson');
            $title               = $title; // $form_state['content_storage'][$i] -> title;
            $title               = empty($title) ? '' : mb_detect_encoding($title, 'UTF-8') == 'UTF-8' ? $title : utf8_encode($title);
            if ($_SESSION["uploadtype"] == "upload")
                $form_state['values']['video'] = $form_state['content_storage'][$i]->launchurl;
            expDebug::dPrint("Inserting content lession:" . $form_state['values']['video']);
            $fields = array(
                'content_version_id' => $contentVersionId,
                'title' => $title,
                'launchurl' => empty($form_state['content_storage'][$i]->launchurl) ? '' : $form_state['content_storage'][$i]->launchurl,
                'masteryscore' => empty($form_state['content_storage'][$i]->masteryscore) ? 0 : $form_state['content_storage'][$i]->masteryscore,
                'datafromlms' => empty($form_state['content_storage'][$i]->datafromlms) ? '' : $form_state['content_storage'][$i]->datafromlms,
                'created_by' => $createUserId,
                'created_on' => now(),
                'updated_by' => $createUserId,
                'updated_on' => now(),
                'custom0' => $nullVal,
                'custom1' => $nullVal,
                'custom2' => $nullVal,
                'custom3' => $nullVal,
                'custom4' => $nullVal
            );
            $insertContentLesson->fields($fields);
            expDebug::dPrintDBAPI('$insertContentLesson', $insertContentLesson);
            $insertContentLesson->execute();
        }
    }
    catch (Exception $ex) {
        watchdog_exception('insertMasterContent', $ex);
        expertusErrorThrow($ex);
    }
}

function getUploadContentAuthorData($form, &$form_state, $sourceField, $verId)
{
    try {
        include_once $_SERVER["DOCUMENT_ROOT"] . "/includes/file.inc";
    //    include_once(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_scorm_parser.inc');
     //   include_once(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_tincanscorm_parser.inc');
     
        expDebug::dPrint("getUploadContentData called...");
        expDebug::dPrint("all details 1122 abhishek test -- " . print_r($form_state, true));
        $isZipFile = false;
        if ($form_state['values']['content_type'] == 'lrn_cnt_typ_knc') {
            $validators = array(
                'file_validate_extensions' => array(
                    'txt doc docx ppt pptx pps ppsx xls xlsx pdf png jpg jpeg gif'
                ) //Ticket#0029042 added two file extensions(pps,ppsx)
            );
        } else if ($form_state['values']['content_type'] == 'lrn_cnt_typ_vod') {
            $validators = array(
                'file_validate_extensions' => array(
                    'webm mp4'
                )
            );
        } else {
            $validators = array(
                'file_validate_extensions' => array(
                    'zip'
                )
            );
            $isZipFile  = true;
        }
        expDebug::dPrint("File formats -- " . print_r($validators, true));
        $contentData = file_save_upload($sourceField, $validators);
        //expDebug::dPrint('$contentData returned by file_save_upload() = ' . print_r($contentData, true), 4);
        $flDir       = '';
        if (!empty($contentData)) {
            if (!$contentData->status) { // i.e. if picture is a temporary file
                // Prepare the pictures directory.
                $contentData_directory = file_default_scheme() . '://h5peditor/videos'; // . variable_get('content_upload_path', 'contentupload');
                if (!$isZipFile) {
                    $fn    = explode('.', $contentData->filename);
                    $fname = $fn[0];
                    if (sizeOf($fn) > 2) {
                        for ($i = 1; $i < sizeOf($fn) - 1; $i++) {
                            $fname = $fname . "." . $fn[$i];
                        }
                    }
                    $flDir                 = $fname . "_" . $verId;
                    $contentData_directory = $contentData_directory . '/' . $flDir;
                }
                $uploadDirectory = $contentData_directory;
                expDebug::dPrint('$contentData_directory = ' . print_r($contentData_directory, true), 4);
                file_prepare_directory($contentData_directory, FILE_CREATE_DIRECTORY);
                //drupal_chmod($contentData_directory, 0777);
                // Prepare the unique file name to place the picture under the $contentData_directory.
                $info        = image_get_info($contentData->uri);
                //expDebug::dPrint('$info returned by image_get_info = ' . print_r($info, true), 4);
                //$destination = file_stream_wrapper_uri_normalize($contentData_directory . '/picture-' . $drupalUid . '-' . REQUEST_TIME . '.' . $info['extension']);
                $destination = $contentData_directory;
                //$destination = str_replace("contentupload","h5peditor/videos",$contentData_directory);
                
                //$destination = "public://h5peditor/videos/1video_50";
                expDebug::dPrint('$destination = ' . print_r($destination, true), 4);
                expDebug::dPrint('$contentData returned by file_move() before move= ' . print_r($contentData, true), 4);
                
                // Move the temporary file into the final location.
                if ($contentData = file_move($contentData, $destination, FILE_EXISTS_REPLACE)) {
                    expDebug::dPrint('$contentData returned by file_move() = ' . print_r($contentData, true), 4);
                    
                    // Update file status from temporary to permanent so that cron does not clean it out
                    $contentData->status = FILE_STATUS_PERMANENT;
                    $contentData         = file_save($contentData);
                    expDebug::dPrint('$contentData returned by file_save() = ' . print_r($contentData, true), 4);
                    $uploadFile   = $contentData->uri;
                    $fileNameFind = explode('/', $contentData->uri);
                    $uploadedFile = $fileNameFind[(count($fileNameFind) - 1)];
                    $fileName     = basename($uploadedFile, ".zip");
                    // Increment file usage count in file_usage table
                    //file_usage_add($contentData, 'user', 'user', $drupalUid);
                }
            }
        }
        
        expDebug::dPrint("uploadFile >>> " . $uploadFile, 4);
        expDebug::dPrint("fileName >>> $fileName", 4);
        //include_once(drupal_get_path('module', 'system') .'/system.archiver.inc');
        //$destination = str_replace("contentupload","h5peditor/videos",$contentData_directory);
        
        
        $uploadDirectory = './sites/default/files/h5peditor/videos';
        $uploadFile      = $uploadDirectory . '/' . $uploadedFile;
        $getFoldername   = "h5peditor/videos/";
        expDebug::dPrint("fileName1 >>> $fileName", 4);
        if ($isZipFile) {
            $zip = new ZipArchive;
            if ($zip->open($uploadFile) === TRUE) {
                $zip->extractTo($contentData_directory . '/' . $fileName);
                $zip->close();
                expDebug::dPrint("extractFilePath start ok", 4);
            } else {
                expDebug::dPrint("extractFilePath start failed", 4);
            }
            $setFileExit = $uploadDirectory . '/' . $fileName . '';
        } else {
            $setFileExit = $uploadDirectory . '/' . $flDir . '';
        }
        expDebug::dPrint("fileName2 >>> $fileName", 4);
        //expDebug::dPrint("Uploded File Path >>> ".$setFileExit , 4);
        
        $hasSubDirectory = getTotalSubDir($setFileExit);
        expDebug::dPrint("fileName3 >>> $fileName", 4);
        //expDebug::dPrint("Total Sub Directory >>> ".$hasSubDirectory , 4);
        
        $contentType = $form_state['values']['content_type'];
        
        if ($contentType == "lrn_cnt_typ_vod") {
            $setNewDirectory       = $getFoldername . $flDir;
            $conversion_needed     = getConfigValue("convert_video_for_mobile_access");
            $conversion_background = getConfigValue("convert_video_on_background");
            $ffmpeg_home           = getConfigValue("ffmpeg_home") . '/bin';
            //expDebug::dPrint("conversion_needed >>> $conversion_needed" , 4);
            expDebug::dPrint("hello1:" . $conversion_needed, 4);
            
            if(strpos($fileName, ".webm")>0)
                convertVideoForMobileAccessCA($setNewDirectory."/".$fileName, $ffmpeg_home,'webmToMp4'); //adding an additional parameter
            
            if (isset($conversion_needed) && $conversion_needed == "1" && $conversion_background == "0") {
            	/* if(strpos($fileName, ".webm")>0)
                	convertVideoForMobileAccessCA($setNewDirectory."/".$fileName, $ffmpeg_home);
            	else */
            		convertVideoForMobileAccessCA($setNewDirectory, $ffmpeg_home);
            }
            $setNewDirectory = $setNewDirectory . "/" . $fileName;
            expDebug::dPrint("hello2 ok:" . $conversion_needed, 4);
        } else {
            if ($isZipFile)
                $setNewDirectory = $getFoldername . $fileName;
            else
                $setNewDirectory = $getFoldername . $flDir;
        }
        
        $realPath = '';
        
        expDebug::dPrint("setNewDirectory2 >>> $setNewDirectory $fileName $setFileExit", 4);
        
        $gutil          = new GlobalUtil();
        $geturlpath     = $gutil->getConfig();
        $pathFromini    = $geturlpath["content_upload_path"];
        $urlpath        = $pathFromini . '/' . $setNewDirectory;
        $localStorePath = $urlpath;
       
   //     setNewDirectory2 >>> h5peditor/videos/Testing_video_102/Testing_video.mp4 Testing_video.mp4 ./sites/default/files/h5peditor/videos/Testing_video_102
        
        expDebug::dPrint("urlpath >>> " . $urlpath, 4);
        
        /*video streaming supporting - copy a file to red5 server streams folder -start here */
        
        if ($contentType == "lrn_cnt_typ_vod") {
            $site_name   = variable_get('site_name');
            $source_path = $urlpath;
            $config      = getConfig("exp_sp");
            
            //getting configuration things start here.
            
            //$red5_server_on = $config["red5_server_on"];
            $red5_server_path = $config["red5_server_path"];
            
            
            include_once(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
            $module_name          = array(
                'exp_sp_stream_server'
            );
            $stream_server_status = getDrupalModuleStatus($module_name);
            expDebug::dPrint('$stream_server_status = ' . print_r($stream_server_status, true), 4);
            
            if ($stream_server_status == 1) {
                $stream_server = true;
            } else {
                $stream_server = false;
            }
            //if streaming_server is in off status we need to convert in a single format for mobile purbose
            
            if (($conversion_background == "1") && ($conversion_needed == "1")) {
            	expDebug::dPrint('$stream_server_status111 = ' .$source_path ."====" .$folder_path . "====" .$fileName, 4);
            	//$stream_server_status111 = /expertusone/peteam/apache2/htdocs/H5P_TRUNK_MERGED_MAY/ExpertusONE/sites/default/files/h5peditor/videos/Thislislvideolafterledit_203/Thislislvideolafterledit.mp4========Thislislvideolafterledit.mp4
            	 
            	$from_h5p = true;
            	//$verId -= 1;
            	expDebug::dPrint("get the version IDDDD112233 of last updated content -  " .$verId);
            	//get the respective h5p ID
            	expDebug::dPrint("get the record type111 -  " . $form_state['values']['recordtype']);
            	expDebug::dPrint("get the record id222 -  " . $form_state['values']['id']);
            	//empty($form_state['values']['id']
            	if(empty($form_state['values']['id']) && $form_state['values']['recordtype'] == 'new')
            	{
            	 $mast_id = getLatestContentAuthorVersionId(); // fetch the latest h5p content id.
            	 expDebug::dPrint("Inside new file condition :: get the record id222 -  " . $mast_id);
            	// $mast_id -= 1;
            	 $h5pID = getH5PContentId($mast_id);
            	 $h5pID[0]->h5p_content_id += 1;
            	 expDebug::dPrint("Inside new file condition :: get the record id222 -  " .  $h5pID[0]->h5p_content_id);
            	}
            	else {
            		$h5pID = getH5PContentId($form_state['values']['id']);
            		expDebug::dPrint("Inside old file condition :: get the record id222 -  " . $form_state['values']['id']);
            		
            	}
            	
            	expDebug::dPrint("get the new flDir2222qqqq -  " . $h5pID[0]->h5p_content_id);
            	$flDir = $h5pID[0]->h5p_content_id . '/videos/' . $flDir;
            	expDebug::dPrint("get the new flDir2222 -  " .$flDir);
                convertVideoForRed5Server($source_path, $folder_path, $fileName, $ffmpeg_home, $flDir, $stream_server);
                copy($source_path, $desination_path);
            }
        } 
        
        /*video streaming support -end here */
        
        $inputValues = array();
        //$contentType="AICC";
        if ($contentType != "lrn_cnt_typ_aic") {
            
                $inputValues = ContentMasterKnowledgeContentValidate($urlpath, $fileName, $contentType, $setFileExit);
            
        } 
        
        $sizePath                          = ($contentType != "lrn_cnt_typ_aic") ? 0 : 1;
        $inputValues[$sizePath]->file_size = getUploadedFileSize($contentData->filesize);
        $inputValues[$sizePath]->file_path = $setFileExit;
        $inputValues[$sizePath]->file_name = $fileName;
        expDebug::dPrint("inputValues >>> " . print_r($inputValues, true), 4);
        
        /* Start of CDN upload - Only for Video & Knowledge Content */
        
        try {
            include_once(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
            include_once($_SERVER["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
            $cdn_status = getDrupalModuleStatus(array(
                'cloud_files'
            ));
            expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' . print_r($cdn_status, true), 4);
            if ($cdn_status == 1 && ($contentType == "lrn_cnt_typ_vod" || $contentType == "lrn_cnt_typ_knc")) {
                expDebug::dPrint(" Upload Content to CDN fileName -  " . print_r($fileName, true), 4);
                expDebug::dPrint(" Upload Content to CDN localStorePath -  " . print_r($localStorePath, true), 4);
                expDebug::dPrint(" Upload Content to CDN setFileExit -  " . print_r($setFileExit, true), 4);
               // /expertusone/peteam/apache2/htdocs/H5P_TRUNK_MERGED_MAY/ExpertusONE/sites/default/files/h5peditor/videos/Testlforlaccess_3/Testlforlaccess.mp4
               //  sites/default/files/h5peditor/videos/Testlforlaccess_3
                $cdnUploadFile   = '';
                $CDNuploadfolder = '';
                $CDNuploadfolder = str_replace('./sites/default/files', '', $setFileExit);
                
                if ($contentType == "lrn_cnt_typ_vod") {
                    $cdnUploadFile = fopen($localStorePath, 'r');
                } else {
                    $cdnUploadFile = fopen($localStorePath . '/' . $fileName, 'r');
                }
                
                $rackspace_cloud_region    = variable_get('rackspace_cloud_region');
                $rackspace_cloud_container = variable_get('rackspace_cloud_container');
                
                expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_container->' . print_r($rackspace_cloud_container, true), 4);
                expDebug::dPrint(' Upload Content to CDN - $rackspace_cloud_region->' . print_r($rackspace_cloud_region, true), 4);
                
                $client = initializeCDN();
                $client->authenticate();
                $object_store_service = $client->objectStoreService('cloudFiles', $rackspace_cloud_region);
                $container            = $object_store_service->getContainer($rackspace_cloud_container);
                
                $CDNuploadfolder = str_replace('/h5peditor', '', $CDNuploadfolder);
                
                expDebug::dPrint("hello11111:" . $CDNuploadfolder . "===" . $fileName . "====" . $cdnUploadFile);
              
                $container->uploadObject('/' . $CDNuploadfolder . '/' . $fileName, $cdnUploadFile);
                
            }
            
        }
        catch (Exception $ex) {
            watchdog_exception('getUploadContentData : CDN upload', $ex);
            expDebug::dPrint(' getUploadContentData : CDN upload Exception->' . print_r($ex, true), 1);
            add_audit_trail_entry(getIdOfLoggedInUser(), '0', 'cre_sys_obt_cnt', 'exp_sp_administration_content', 'getUploadContentData', 'Content Uploaded Failed In CDN');
            //expertusErrorThrow($ex);
        }
        
       /*End of CDN upload*/
        return $inputValues;
    }
    catch (Exception $ex) {
        watchdog_exception('getUploadContentData', $ex);
        expertusErrorThrow($ex);
    }
}

function get_video_filename_from_fs_ca($fileDir)
{
	try {
		$videoBasename = "";
		if ($handle = opendir($fileDir)) {
			while (false !== ($file = readdir($handle)))        {
				$lowerCaseFileName = strtolower($file);
				if ($file != '.' && $file != '..' && (substr($lowerCaseFileName, -strlen('.mp4')) === '.mp4' || substr($lowerCaseFileName, -strlen('.webm')) === '.webm') &&
				(strpos($lowerCaseFileName, 'mobile_') === false || strpos($lowerCaseFileName, 'mobile_') !== 0))	{
					$videoBasename = $file;// substr($file, 0, strrpos($file, '.') + 0);
					break;
				}
			}
			closedir($handle);
		}
		return $videoBasename;
	} catch (Exception $ex) {
		watchdog_exception('get_video_filename_from_fs', $ex);
		expertusErrorThrow($ex);
	}
}


function convertVideoForMobileAccessCA($sourceDir,$ffmpeg_home,$from='')
{
	try {
		$output = "";
		$fileDir = $_SERVER["DOCUMENT_ROOT"]."/sites/default/files/".$sourceDir;
		expDebug::dPrint("fileDir Suresh:".$fileDir);
		if(strpos($fileDir, ".mp4")>0)
		{
			$tmp_arr = split("/",$fileDir);
			$fileDir = "";
			for($i=0;$i<count($tmp_arr)-1;$i++)
			{
				$fileDir.="/".$tmp_arr[$i];
			}
		}
		else if(strpos($fileDir, ".webm")>0)
		{
			$tmp_arr = split("/",$fileDir);
			$fileDir = "";
			for($i=0;$i<count($tmp_arr)-1;$i++)
			{
			$fileDir.="/".$tmp_arr[$i];
			}
			}
			expDebug::dPrint("suresh fileDir::".$fileDir);
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_video_conversion_for_mobile.inc";
	$videoBasename = get_video_filename_from_fs_ca($fileDir);
	$srcFileName = $fileDir."/".$videoBasename;//.".mp4";
	
	if($from == "webmToMp4")
	    $mobileVideoFileName = $fileDir."/".$videoBasename;
	  else
	     $mobileVideoFileName = $fileDir."/"."mobile_".$videoBasename;

	expDebug::dPrint("srcFileName:".$srcFileName , 4);
	expDebug::dPrint("mobilesrcFileName :".$mobileVideoFileName , 4);

	$is_installed = ffmpeg_installed($ffmpeg_home);
	expDebug::dPrint("ffmpeg is_installed:".$is_installed , 4);
	if($is_installed == true)
		{
		$_SESSION['VODUploaded'] = $srcFileName;
		expDebug::dPrint("VODUploaded:".$srcFileName , 4);
		if(strpos($sourceDir,".webm") > 0 && $from == "webmToMp4")
		{
			$mobileVideoFileName = str_replace(".webm",".mp4",$mobileVideoFileName);
				
			convertVideoForMobileAccessForWEBM_TO_MP4($srcFileName,$mobileVideoFileName,$ffmpeg_home);
		}
		else
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

function getAdminContentAuthorVersionList($contentId)
{
    try {
        $select = db_select('slt_content_version', 'version');
        $select->addExpression('count(1)', 'version_cnt');
        $select->condition('version.content_master_id', $contentId);
        $select->condition('version.status', 'lrn_cnt_sts_del', '<>');
        $result = $select->execute();
        return $result->fetchField();
    }
    catch (Exception $ex) {
        watchdog_exception('getAdminContentVersionList', $ex);
        expertusErrorThrow($ex);
    }
}

function getAdminContentAuthorVersionRecords($contentId, $start, $limit)
{
    try {
        $select = db_select('slt_content_version', 'version');
        $select->addField('version', 'id', 'version_id');
        $select->addField('version', 'content_master_id', 'master_id');
        $select->addField('version', 'content_sub_type', 'sub_type');
        $select->addField('version', 'title', 'version_title');
        $select->addField('version', 'hosted_type', 'hosted_type');
        $select->addField('version', 'total_lesson', 'version_lessons');
        $select->addField('version', 'version', 'version_number');
        $select->addField('version', 'overall_mastery_score', 'version_score');
        $select->addField('version', 'status', 'version_status');
        $select->addField('version', 'file_size', 'version_file_size');
        $select->addField('version', 'file_path', 'version_file_path');
        $select->addField('version', 'created_by', 'version_creator');
        $select->addField('version', 'created_on', 'version_created');
        $select->addField('version', 'updated_by', 'version_updator');
        $select->addField('version', 'published_on', 'version_updated');
        $select->condition('version.content_master_id', $contentId);
        $select->condition('version.status', 'lrn_cnt_sts_del', '<>');
        $select->orderBy('version_number', 'DESC');
        $select->range($start, $limit);
        expDebug::dPrintDBAPI(' getAdminContentVersionRecords SQLLL ------>     ', $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint(' query result ---- > ' . print_r($result, true), 4);
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getAdminContentVersionRecords', $ex);
        expertusErrorThrow($ex);
    }
}


function getActiveUsersForContentAuthor($contentId, $versionId)
{
    try {
        $select = getUsersQueryForVersionList($contentId, $versionId, '', '', 'count');
        expDebug::dPrintDBAPI('getActiveUsersForContent SQLLL ------>     ', $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint(' getActiveUsersForContent query result ---- > ' . print_r($result, true), 4);
        return count($result);
    }
    catch (Exception $ex) {
        watchdog_exception('getActiveUsersForContent', $ex);
        expertusErrorThrow($ex);
    }
    
}

function deleteContentAuthorVersion($versionId)
{
    $txn = db_transaction();
    try {
        $currUserId    = getSltpersonUserId();
        $deleteVersion = db_update('slt_content_version');
        $deleteVersion->condition('id', $versionId);
        $deleteVersion->fields(array(
            'status' => 'lrn_cnt_sts_del',
            'updated_on' => now(),
            'updated_by' => $currUserId
        ));
        expDebug::dPrintDBAPI('deleteContentVersion delete query --> ', $deleteVersion);
        $deleteVersion->execute();
        
        /* commented part to be used for folder deletion on version delete
         * commented because even completed users will still need the folders for the launch of those contents
         */
        ////44353: When an admin user deletes the content the physical file should be deleted from the content folder
        $filepathSelect = db_select('slt_content_version', 'version');
        $filepathSelect->addField('version', 'file_path');
        $filepathSelect->condition('version.id', $versionId, '=');
        $filePath = $filepathSelect->execute()->fetchField();
        expDebug::dPrint(' deleteContentVersion delete file path ' . $filePath);
      //  $uploadDirectory = './sites/default/files/contentupload';
        $uploadDirectory = './sites/default/files/h5p/content';
        if (!empty($filePath) && ($filePath != $uploadDirectory) && ($filePath != $uploadDirectory . '/')) {
            removedir($filePath);
            unlink($filePath . '.zip');
        }
    }
    catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('deleteContentVersion', $ex);
        throw $ex;
    }
    
    unset($txn);
    // Commit txn
}

function getDefaultContentAuthorVersion($contentId)
{
    try {
        $select = db_select('slt_content_version', 'version');
        $select->addField('version', 'id', 'version_id');
        $select->addField('version', 'version', 'version_number');
        $select->condition('version.content_master_id', $contentId);
        $select->condition('version.status', 'lrn_cnt_sts_atv');
        $result = $select->execute();
        expDebug::dPrintDBAPI('getDefaultContentVersion QUERY DEFAULT ---> ', $select);
        return $result->fetchAll();
    }
    catch (Exception $ex) {
        watchdog_exception('getDefaultContentVersion', $ex);
        expertusErrorThrow($ex);
    }
}

function getLatestContentAuthorVersionId()
{
    try {
       /*   $select = db_select('slt_content_version', 'version');
        $select->addField('version', 'id', 'version_id');
        $select->orderBy('version.id', 'DESC');
        $select->range(0, 1);
        $result = $select->execute();
        expDebug::dPrintDBAPI('getLatestVersionId QUERY ---> ', $select);
        return $result->fetchField();  */
        
    	 $select = db_select('slt_content_master', 'mast');
    	$select->addField('mast', 'id', 'id');
    	$select->condition('mast.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'IN');
    	$select->orderBy('mast.id', 'DESC');
    	$select->range(0, 1);
    	$result = $select->execute();
    	expDebug::dPrintDBAPI('getLatestContentAuthorVersionId QUERY ---> ', $select);
    	return $result->fetchField(); 
    }
    catch (Exception $ex) {
        watchdog_exception('getLatestContentAuthorVersionId', $ex);
        expertusErrorThrow($ex);
    }
}

/*
function to get the list of lessons for a content version
--> to use parameters for preview of content
*/
function getContentAuthorLessonList($versionId)
{
    try {
        $userId = getIdOfLoggedInUser();
        $select = db_select('slt_content_lesson', 'lesson');
        $select->join('slt_content_version', 'version', 'lesson.content_version_id = version.id');
        $select->join('slt_profile_list_items', 'lst', 'version.content_sub_type = lst.code');
        $select->addField('lesson', 'id', 'lessonid');
        $select->addField('lesson', 'title', 'title');
        $select->addField('lesson', 'launchurl', 'launchurl');
        $select->addField('version', 'content_sub_type', 'contentypecode');
        $select->addField('version', 'hosted_type', 'contenthostedtype');
        $select->addField('lst', 'name', 'contentype');
        $select->condition('lesson.content_version_id', $versionId, '=');
        $result = $select->execute()->fetchAll();
        expDebug::dPrintDBAPI('getLessonList QUERY ---> ', $select);
        expDebug::dPrint("result  getLessonList --> " . print_r($result, true), 4);
        
        include_once(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
        include_once($_SERVER["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
        $cdn_status = getDrupalModuleStatus(array(
            'cloud_files'
        ));
        expDebug::dPrint('CDN - Cloud Files Modules is enabled = ' . $cdn_status . '---->' . print_r($cdn_status, true), 4);
        
        $i = 0;
        foreach ($result as $response) {
            $aiccId               = $userId . "-0-0-" . $response->lessonid . "-0-0-0";
            $enc1                 = new Encrypt();
            $aiccId               = $enc1->encrypt($aiccId);
            $result[$i]->AICC_SID = $aiccId;
            if ($cdn_status == 1 && $result[$i]->contenthostedtype != '2') {
                $result[$i]->contenthostedtype = "3";
            }
            $i++;
        }
        expDebug::dPrint("result  getLessonList final--> " . print_r($result, true), 4);
        return $result;
        
    }
    catch (Exception $ex) {
        watchdog_exception('getLessonList', $ex);
        expertusErrorThrow($ex);
    }
    
}

/* Function to check if the name given to the content already exists */
function addEditContentAuthorValidate(&$form_state, $content_id, $mode, $title)
{
    try {
        $form_state["storage"]["form_validate"] = true;
        $select                                 = db_select("slt_content_master", "cont");
        $select->addField('cont', 'id', 'cont_id');
        $select->addField('cont', 'code', 'cont_name');
        if (!empty($content_id)) {
            $select->condition('cont.id', array(
                $content_id
            ), 'NOT IN');
        }
        
        $contList = $select->execute()->fetchAll();
        if ($mode == "Edit") {
            $cont_name = $form_state['values']['title'];
        } else {
            $cont_name = $title;
        }
        $cont_name_flag = false;
        
        foreach ($contList as $cont_list) {
            if (strtolower($cont_list->cont_name) == strtolower($cont_name)) {
                $cont_name_flag = true;
            }
        }
        if ($cont_name_flag && $mode == "Edit") {
            form_set_error('title', t('LBL949'));
            $form_state["storage"]["form_validate"] = false;
            $form_state['esign_show_popup']         = false;
        } else if ($cont_name_flag && $mode == "Add") {
            return "exists";
        }
    }
    catch (Exception $ex) {
        watchdog_exception('addEditContentValidate', $ex);
        expertusErrorThrow($ex);
    }
}

function addEditFormWithContentAuthorCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $contentId, $emptyId = '', $subStage = '', $objectString = '')
{
    try {
        expDebug::dPrint('suresh addEditFormWithContentCancelAndSaveClose ');
        
        global $theme_key;
        expDebug::dPrint(' addEditFormWithContentCancelAndSaveClose() called.');
        
        $customHiddenButtonId = 'last-button-account';
        
        $esignHiddenButt = 'esign-' . $wrapperId;
        
        addHiddenFieldToAddEditForm($form, 'ENTITY_ID', $contentId);
        
        // Remove the temporary storage for API's to exchange information during form creation
        unset($form['addedit_tmp_storage']);
        
        // Add the Cancel button
        $ajaxCallbackCancel        = 'addEditCancelRepaintForm';
        $cancelAttributes          = array();
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $cancelAttributes['class'] = array(
            'addedit-edit-' . $cancelButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit cancel buttons
        //$cancelAttributes['class'][] = 'addedit-edit-cancel';
        
        // Using this class to applying bg color to the save button
        $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $cancelAttributes['class'][]        = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $cancelAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        
        // A common class for all addedit form cancel button, for styling to appear as a link.
        $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
        
        $cancelAttributes['onclick']    = "$('.popups-close .close').click();Drupal.CTools.Modal.dismiss();return false;";// $('#root-admin').data('narrowsearch').refreshGrid();return false;";
        $cancelLabel                    = t('LBL123'); // Close
        $cancelAttributes['tabindex'][] = '6';
        $prfixPgLeft                    = '';
        $suffixPgRight                  = '';
        if ($theme_key == 'expertusoneV2') {
            $prfixPgLeft                 = '<div class="white-btn-bg-left"></div>';
            $suffixPgRight               = '<div class="white-btn-bg-right"></div>';
            $cancelAttributes['class'][] = 'white-btn-bg-middle';
        }
        $entityId = $contentId;
        if (!empty($emptyId)) {
            $entityId = 0;
        } else {
            $emptyId = 0;
        }
        if ($subStage == 'details' || empty($subStage)) {
            $tagsentityType = $objectString;
            $tagsentityType = str_replace(' ', '-', $tagsentityType);
            if (empty($contentId)) {
                $contentId      = 0;
                $tagsentityType = "Content";
            }
            
            $tagsentityId = $contentId; //.',"'.$entityType
            if (empty($tagsentityId)) {
                $tagsentityId = 0;
            }
            $qtipTagsIdInit = $tagsentityId . '_' . $tagsentityType;
            expDebug::dPrint('$tagsentityType :' . $tagsentityType);
            if ($theme_key == 'expertusoneV2') {
                $tagsvisibPopupId = 'qtip_visible_disp_tags_' . $qtipTagsIdInit;
                $qtipTagsOptObj   = "{'entityId':" . $tagsentityId . ",'entityType':'" . $tagsentityType . "',
												  		'url':'administration/catalog-tags/ajax/" . $tagsentityId . "/" . $tagsentityType . "/" . $emptyId . "',
												  		'popupDispId':'" . $tagsvisibPopupId . "','catalogVisibleId':'renderTagsId" . $qtipTagsIdInit . "',
												  		'wid':400,'heg':'170','postype':'bottomleft','poslwid':'','qdis':'ctool',
															'linkid':'visible-content-ctool-tags-" . $tagsentityId . "'}";
            } else {
                $tagsvisibPopupId = 'qtip_visible_disp_tags_' . $qtipTagsIdInit;
                $qtipTagsOptObj   = "{'entityId':" . $tagsentityId . ",'entityType':'" . $tagsentityType . "',
												  		'url':'administration/catalog-tags/ajax/" . $tagsentityId . "/" . $tagsentityType . "/" . $emptyId . "',
												  		'popupDispId':'" . $tagsvisibPopupId . "','catalogVisibleId':'renderTagsId" . $qtipTagsIdInit . "',
												  		'wid':400,'heg':'140','postype':'bottomleft','poslwid':'','qdis':'ctool',
															'linkid':'visible-content-ctool-tags-" . $tagsentityId . "'}";
            }
            
            $entityId = $contentId;
            if (!empty($emptyId)) {
                $entityId = 0;
            } else {
                $emptyId = 0;
            }
             
            $entityType          = 'cre_sys_obt_cnt';
            $qtipIdInit          = $entityId . '_' . $entityType;
            $qtipOptAccessObjacc = "{'entityId':" . $entityId . ",'entityType':'$entityType',
															  	'url':'administration/catalogaccess/" . $entityId . "/" . $entityType . "/" . $emptyId . "',
															  	'popupDispId':'qtip_visible_disp_" . $qtipIdInit . "','catalogVisibleId':'qtipAccessqtip_visible_disp_" . $qtipIdInit . "',
															  	'wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool',
															  	'linkid':'visible-content-" . $entityId . "'}";
            
            $form['addedit-left-col-extend-func_acc'] = array(
                '#type' => 'markup',
                '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row ">',
                '#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>' . "<div id='qtip_visible_disp_" . $qtipIdInit . "'><a id='visible-content-" . $entityId . "' onclick = \"callVisibility(" . $qtipOptAccessObjacc . ");\" class=\"tab-title\">" . t('LBL642') . "</a><span id='visible-popup-" . $entityId . "' class='qtip-popup-visible' style='display:none;position:absolute; left:0px; top:0px;'></span></div>" . '</div>'
            );
            
            $form['addedit-left-col-extend-func'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="crs-tab-titles-container Content-tab-titles-container"><div class="tags-tab-icon"></div>' . "<div id='" . $tagsvisibPopupId . "'><a id='visible-content-ctool-tags-" . $tagsentityId . "' class=\"tab-title\" onclick = \"callVisibility(" . $qtipTagsOptObj . ");\">" . t('LBL191') . "</a>
  			<span id='visible-popup-" . $tagsentityId . "' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>" . '</div>'
            );
   
        }
        
        
        
        $form[$cancelButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="addedit-form-cancel-container-actions">' . $prfixPgLeft,
            '#suffix' => $suffixPgRight,
            '#value' => $cancelLabel,
            '#name' => $cancelLabel,
            '#attributes' => $cancelAttributes
        );
        
        // Add the Save button
        $ajaxCallbackSave = 'addEditContentAuthorRepaintForm';
        $saveAttributes   = array();
        
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $saveAttributes['class'] = array(
            'addedit-edit-' . $saveButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit save buttons
        //$saveAttributes['class'][] = 'addedit-edit-save';
        
        // Using this class to applying bg color to the save button
        //$saveAttributes['class'][] = 'admin-save-button-middle-bg';
        $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
        
        
        //Increase the width of the popup while clicking on save button
        $saveAttributes['onclick'][] = 'increasePopWidth();';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][]        = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        $saveAttributes['tabindex'][]     = '4';
        
        $contentStatus = '';
        if (!empty($contentId)) {
            $contentDetails = getContentAuthorDetails($contentId);
            $contentStatus  = $contentDetails[0]->status;
        }
        
        expDebug::dPrint("Shri1::" . $contentStatus);
        $isSaveParent = 0;
        $leftBg       = '';
        if ($theme_key == 'expertusoneV2') {
            $leftBg = '<div class="admin-save-button-left-bg"></div>';
        }
        $setButtonId = 'content_save_btn';
        if (empty($contentId) || $contentStatus != 'lrn_cnt_sts_atv') {
            $isSaveParent                = 1;
            $saveAttributes['onclick']   = array();
            expDebug::dPrint("ABHI Author123::" . print_r($form, true));
            addHiddenFieldToAddEditForm($form, 'VIDEO_SAVE', "TRUE");
            $saveAttributes['onclick'][] = 'increasePopWidth();';
            $form[$saveButtonId]         = array(
                '#type' => 'submit',
                '#id' => $setButtonId,
                '#prefix' => '<div class="admin-save-pub-unpub-button-container">' . $leftBg,
                '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
                '#value' => t('LBL141'),
            	//'#value' => t('hello12'),
                '#name' => 'save',
                '#attributes' => $saveAttributes,
                '#ajax' => array(
                    'wrapper' => $wrapperId,
                    'callback' => $ajaxCallbackSave,
                    'customButtonId' => $customHiddenButtonId,
                    'method' => 'replace',
                    'effect' => 'none', // 'fade',
                    'event' => 'click',
                    'keypress' => true,
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => ''
                    )
                )
            );
        }
        
        if ($contentId) {
            $resultCnt = delete_object_record($contentId, 'Content');
        }
        
        if ($isSaveParent) {
            $setButtonId                           = 'content_save_pub_btn';
            $form['addedit-button-group-ul-start'] = array(
                '#type' => 'markup',
                '#markup' => '<ul class="catalog-pub-add-list">'
            );
            $savePublishPrefix                     = '<li class="save-pub-unpub-sub-menu">';
            $savePublishSuffix                     = '</li>';
        } else {
            $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container">' . $leftBg;
            $savePublishSuffix = ((($resultCnt == 0) && ($contentStatus == 'lrn_cnt_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>') . '</div>';
        }
        $saveButtonId                            = 'catalog-course-save-publish';
        $savePublishAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        $savePublishAttributes['onclick']        = array();
        $savePublishAttributes['tabindex'][]     = '5';
        
        $savePublishAttributes['onclick'][] = 'increasePopWidth();';
        $buttonLabel                        = ($contentStatus == 'lrn_cnt_sts_atv') ? t('LBL141') : t('LBL614');
        $savePublishAttributes['class']     = array(
            'addedit-edit-' . $saveButtonId
        );
        $savePublishAttributes['class'][]   = 'addedit-form-expertusone-throbber';
        $saveOnlyBtnCls                     = ($theme_key == 'expertusoneV2') ? 'admin-save-button-middle-bg' : 'pub-unpub-only-save-btn';
        $savePublishAttributes['class'][]   = (($resultCnt == 0) && ($contentStatus == 'lrn_cnt_sts_atv')) ? 'edit-catalog-course-save-publish' : $saveOnlyBtnCls;
        if ($contentStatus != 'lrn_cnt_sts_atv')
        	addHiddenFieldToAddEditForm($form, 'VIDEO_SAVE_AND_PUBLISH', "TRUE");
        addHiddenFieldToAddEditForm($form, 'VIDEO_CONTENT_STATUS', $contentStatus);
        $form[$saveButtonId]                = array(
            '#type' => 'submit',
            '#id' => $setButtonId,
            '#prefix' => $savePublishPrefix,
            '#suffix' => $savePublishSuffix,
            '#value' => $buttonLabel,
            '#name' => 'saveandpublish',
            '#attributes' => $savePublishAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => $ajaxCallbackSave,
                'customButtonId' => $customHiddenButtonId,
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
        if (!$isSaveParent) {
            $form['addedit-button-group-ul-start'] = array(
                '#type' => 'markup',
                '#markup' => '<ul class="catalog-pub-add-list">'
            );
        }
        expDebug::dPrint('Checking the type of save called :::' . $saveButtonId);
        expDebug::dPrint("Shri1...." . $contentId . "....contentStatus:" . $contentStatus);
        //expDebug::dPrint("Shri1....".$contentStatus);
        if (($contentId) && $contentStatus == 'lrn_cnt_sts_atv') {
            if ($resultCnt == 0) {
                $saveButtonId                              = 'catalog-course-save-unpublish';
                $saveUnPublishAttributes['onclick']        = array();
                $saveUnPublishAttributes['onclick'][]      = 'increasePopWidth();';
                $saveUnPublishAttributes['class']          = array(
                    'addedit-edit-' . $saveButtonId
                );
                $saveUnPublishAttributes['class'][]        = 'addedit-form-expertusone-throbber';
                $saveUnPublishAttributes['data-wrapperid'] = array(
                    $wrapperId
                );
                addHiddenFieldToAddEditForm($form, 'VIDEO_UNPUBLISH', "TRUE");
                $form[$saveButtonId]                       = array(
                    '#type' => 'submit',
                    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                    '#suffix' => '</li>',
                    '#value' => t('LBL571'),
                    '#name' => 'unpublish',
                    '#attributes' => $saveUnPublishAttributes,
                    '#ajax' => array(
                        'wrapper' => $wrapperId,
                        'callback' => $ajaxCallbackSave,
                        'customButtonId' => $customHiddenButtonId,
                        'method' => 'replace',
                        'effect' => 'none', // 'fade',
                        'event' => 'click',
                        'keypress' => true,
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => ''
                        )
                    )
                );
            }
        }
        $deleteCheck = adminVisibilityForAddEditDelete($entityId, 'cre_sys_obt_cnt_aut', 'cre_sys_obt_cnt_aut', 'slt_content_master');
        if ($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1) {
            if (($contentId)) {
                if ($resultCnt == 0) {
                    $displayTitle = titleController('EXP-SP-ADMIN-CONTENT-MODULE-TITLE', $contentDetails[0]->title, 34);
                    $saveButtonId = 'catalog-course-delete';
                    
                    $deleteAttributes['onclick']   = array();
                    $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("' . t('MSG357') . ' ' . strtolower(t('Content')) . ' ' . escape_string($contentDetails[0]->code) . '","' . $contentId . '","content"); return false;';
                    $deleteAttributes['class'][]   = 'admin-delete-button-bg';
                    $form[$saveButtonId]           = array(
                        '#type' => 'button',
                        '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                        '#suffix' => '</li>',
                        '#value' => t('LBL286'),
                        '#name' => 'delete',
                        '#attributes' => $deleteAttributes
                    );
                }
            }
        }
        
        $form['addedit-button-group-ul-end'] = array(
            '#type' => 'markup',
            '#markup' => '</ul><div class="clearBoth"></div></div>' . '<div class="clearBoth"></div></div>'
        );
        unset($saveAttributes['tabindex']);
        $saveAttributes['onclick']   = array();
        $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
        $saveAttributes['onclick'][] = 'increasePopWidth();';
        $form[$esignHiddenButt]      = array(
            '#type' => 'submit',
            '#value' => t('hidden-save'),
            '#name' => 'hidden-save',
            '#prefix' => '<span id = "esign-admin-div" style="display:none">',
            '#suffix' => '</span>',
            '#id' => $esignHiddenButt,
            '#attributes' => $saveAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => 'addEditContentAuthorRepaintForm',
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
        
        
        
        
        $form['addedit-left-col-end'] = array(
            '#type' => 'markup',
            '#markup' => '</div><div class="clearBoth"></div>'
        );
    }
    catch (Exception $ex) {
        watchdog_exception('addEditFormWithContentCancelAndSaveClose', $ex);
        expertusErrorThrow($ex);
    }
}



function addEditFormWithContentAuthorPresentationCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $contentId, $emptyId = '', $subStage = '', $objectString = '')
{
    try {
        expDebug::dPrint('suresh addEditFormWithContentCancelAndSaveClose ' . $emptyId . "======" .$contentId);
        
        global $theme_key;
        expDebug::dPrint(' addEditFormWithContentCancelAndSaveClose() called.');
        
        $customHiddenButtonId = 'last-button-account';
        
        $esignHiddenButt = 'esign-' . $wrapperId;
        
        // Remove the temporary storage for API's to exchange information during form creation
        unset($form['addedit_tmp_storage']);
        
        // Add the Cancel button
        $ajaxCallbackCancel        = 'addEditCancelRepaintForm';
        $cancelAttributes          = array();
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $cancelAttributes['class'] = array(
            'addedit-edit-' . $cancelButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit cancel buttons
        //$cancelAttributes['class'][] = 'addedit-edit-cancel';
        
        // Using this class to applying bg color to the save button
        $cancelAttributes['class'][] = 'admin-action-button-middle-bg';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $cancelAttributes['class'][]        = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $cancelAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        
        // A common class for all addedit form cancel button, for styling to appear as a link.
        $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
        
        $cancelAttributes['onclick']    = 'Drupal.CTools.Modal.dismiss(); return false;';
        $cancelLabel                    = t('LBL123'); // Close
        $cancelAttributes['tabindex'][] = '6';
        $prfixPgLeft                    = '';
        $suffixPgRight                  = '';
        if ($theme_key == 'expertusoneV2') {
            $prfixPgLeft                 = '<div class="white-btn-bg-left"></div>';
            $suffixPgRight               = '<div class="white-btn-bg-right"></div>';
            $cancelAttributes['class'][] = 'white-btn-bg-middle';
        }
        $entityId = $contentId;
        if (!empty($emptyId)) {
            $entityId = 0;
        } else {
            $emptyId = 0;
        }
        if ($subStage == 'details' || empty($subStage)) {
            $tagsentityType = $objectString;
            $tagsentityType = str_replace(' ', '-', $tagsentityType);
            if (empty($contentId)) {
                $contentId      = 0;
                $tagsentityType = "Content";
            }
            
            $tagsentityId = $contentId; //.',"'.$entityType
            if (empty($tagsentityId)) {
                $tagsentityId = 0;
            }
            $qtipTagsIdInit = $tagsentityId . '_' . $tagsentityType;
            expDebug::dPrint('$tagsentityType :' . $tagsentityType);
            if ($theme_key == 'expertusoneV2') {
                $tagsvisibPopupId = 'qtip_visible_disp_tags_' . $qtipTagsIdInit;
                $qtipTagsOptObj   = "{'entityId':" . $tagsentityId . ",'entityType':'" . $tagsentityType . "',
				'url':'administration/catalog-tags/ajax/" . $tagsentityId . "/" . $tagsentityType . "/" . $emptyId . "',
				'popupDispId':'" . $tagsvisibPopupId . "','catalogVisibleId':'renderTagsId" . $qtipTagsIdInit . "',
				'wid':400,'heg':'170','postype':'bottomleft','poslwid':'','qdis':'ctool',
				'linkid':'visible-content-ctool-tags-" . $tagsentityId . "'}";
            } else {
                $tagsvisibPopupId = 'qtip_visible_disp_tags_' . $qtipTagsIdInit;
                $qtipTagsOptObj   = "{'entityId':" . $tagsentityId . ",'entityType':'" . $tagsentityType . "',
				'url':'administration/catalog-tags/ajax/" . $tagsentityId . "/" . $tagsentityType . "/" . $emptyId . "',
				'popupDispId':'" . $tagsvisibPopupId . "','catalogVisibleId':'renderTagsId" . $qtipTagsIdInit . "',
				'wid':400,'heg':'140','postype':'bottomleft','poslwid':'','qdis':'ctool',
				'linkid':'visible-content-ctool-tags-" . $tagsentityId . "'}";
            }
            
             $entityId = $contentId;
            if (!empty($emptyId)) {
                $entityId = 0;
            } else {
                $emptyId = 0;
            }
       
            
            $entityType          = 'cre_sys_obt_cnt';
            $qtipIdInit          = $entityId . '_' . $entityType;
            $qtipOptAccessObjacc = "{'entityId':" . $entityId . ",'entityType':'$entityType',
			'url':'administration/catalogaccess/" . $entityId . "/" . $entityType . "/" . $emptyId . "',
			'popupDispId':'qtip_visible_disp_" . $qtipIdInit . "','catalogVisibleId':'qtipAccessqtip_visible_disp_" . $qtipIdInit . "',
			'wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool',
			'linkid':'visible-content-" . $entityId . "'}";
            
            $form['addedit-left-col-extend-func_acc'] = array(
                '#type' => 'markup',
                '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">',
                '#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>' . "<div id='qtip_visible_disp_" . $qtipIdInit . "'><a id='visible-content-" . $entityId . "' onclick = \"callVisibility(" . $qtipOptAccessObjacc . ");\" class=\"tab-title\">" . t('LBL642') . "</a><span id='visible-popup-" . $entityId . "' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>" . '</div>'
            ); 
            
            $form['addedit-left-col-extend-func'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="crs-tab-titles-container Content-tab-titles-container"><div class="tags-tab-icon"></div>' . "<div id='" . $tagsvisibPopupId . "'><a id='visible-content-ctool-tags-" . $tagsentityId . "' class=\"tab-title\" onclick = \"callVisibility(" . $qtipTagsOptObj . ");\">" . t('LBL191') . "</a>
					<span id='visible-popup-" . $tagsentityId . "' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>" . '</div>'
            );
        }
        
        
        
        $form[$cancelButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="addedit-form-cancel-container-actions" style="display:none;">' . $prfixPgLeft,
            '#suffix' => $suffixPgRight,
            '#value' => $cancelLabel,
            '#name' => $cancelLabel,
            '#attributes' => $cancelAttributes
        );
        
        // Add the Save button
        $ajaxCallbackSave = 'addEditContentAuthorRepaintForm';
        $saveAttributes   = array();
        
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $saveAttributes['class'] = array(
            'addedit-edit-' . $saveButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit save buttons
        //$saveAttributes['class'][] = 'addedit-edit-save';
        
        // Using this class to applying bg color to the save button
        //$saveAttributes['class'][] = 'admin-save-button-middle-bg';
        $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
        
        
        //Increase the width of the popup while clicking on save button
        $saveAttributes['onclick'][] = 'increasePopWidth();';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][]        = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        $saveAttributes['tabindex'][]     = '4';
        
        $contentStatus = '';
        if (!empty($contentId)) {
            $contentDetails = getContentAuthorDetails($contentId);
            $contentStatus  = $contentDetails[0]->status;
        }
        $isSaveParent = 0;
        $leftBg       = '';
        if ($theme_key == 'expertusoneV2') {
            $leftBg = '<div class="admin-save-button-left-bg"></div>';
        }
        $setButtonId = 'content_save_btn';
        expDebug::dPrint("abhi check1111111" . $contentId . " == " . $contentStatus);

        $is_attached =  getContentDeleteAccess($contentId);
        expDebug::dPrint("abhi 444444455555555" . $is_attached . " == " . $contentId);
        addHiddenFieldToAddEditForm($form, 'PRESENTATION_PUBLISHED', $is_attached);
        
        if (empty($contentId) || $contentStatus != 'lrn_cnt_sts_atv') {
            $isSaveParent              = 1;
            $saveAttributes['onclick'] = array();
            
            //  $saveAttributes['onclick'][] = 'savePresentationH5P();';
            //	$saveAttributes['onclick'][] = 'saveInteractionsH5PWrapper();';
            addHiddenFieldToAddEditForm($form, 'PRESENTATION_SAVE', "TRUE");
            $saveAttributes['onclick'][] = 'increasePopWidth();';
            
            $form[$saveButtonId] = array(
                '#type' => 'submit',
                '#id' => $setButtonId,
                '#prefix' => '<div class="admin-save-pub-unpub-button-container">' . $leftBg,
                '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
                '#value' => t('LBL141'),
                '#name' => 'save',
                
                //'#submit' => array('saveInteractionsH5PWrapper'),
                //$saveAttributes['onsubmit'][] = 'saveInteractionsH5PWrapper();',
                
                '#attributes' => $saveAttributes,
                '#ajax' => array(
                    'wrapper' => $wrapperId,
                    'callback' => $ajaxCallbackSave,
                    'customButtonId' => $customHiddenButtonId,
                    'method' => 'replace',
                    'effect' => 'none', // 'fade',
                    'event' => 'click',
                    'keypress' => true,
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => ''
                    )
                )
            );
            
            
            //$isSaveParent = 1;
            //  $form["eee"]=array('#type' => 'markup', '#suffix' => '<input type="button" onclick="savePresentationH5P();" name="Save" class="presentation_save" id="content_save_btn" value="Save"></input>', );
            
            
        }
        
        if ($contentId) {
            $resultCnt = delete_object_record($contentId, 'Content');
        }
        expDebug::dPrint("abhi check222222" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent);
        if ($isSaveParent) {
            expDebug::dPrint("abhi check3333" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent);
            $setButtonId                           = 'content_save_pub_btn';
            $form['addedit-button-group-ul-start'] = array(
                '#type' => 'markup',
                '#markup' => '<ul class="catalog-pub-add-list">'
            );
            $savePublishPrefix                     = '<li class="save-pub-unpub-sub-menu">';
            $savePublishSuffix                     = '</li>';
        } else {
            expDebug::dPrint("abhi check4444444" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent . "=========" . $resultCnt);
            
            $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container">' . $leftBg;
            $savePublishSuffix = ((($resultCnt == 0) && ($contentStatus == 'lrn_cnt_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>') . '</div>';
        }
        $saveButtonId                            = 'catalog-course-save-publish';
        $savePublishAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        $savePublishAttributes['onclick']        = array();
        $savePublishAttributes['tabindex'][]     = '5';
        
        expDebug::dPrint("abhi check5555555555" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent . "=========" . $resultCnt);
        
        //	$savePublishAttributes['onclick'][] = 'saveInteractionsH5PWrapper_presentation();';
        
        $savePublishAttributes['onclick'][] = 'increasePopWidth();';
        $buttonLabel                        = ($contentStatus == 'lrn_cnt_sts_atv') ? t('LBL141') : t('LBL614');
        $savePublishAttributes['class']     = array(
            'addedit-edit-' . $saveButtonId
        );
        $savePublishAttributes['class'][]   = 'addedit-form-expertusone-throbber';
        $saveOnlyBtnCls                     = ($theme_key == 'expertusoneV2') ? 'admin-save-button-middle-bg' : 'pub-unpub-only-save-btn';
        $savePublishAttributes['class'][]   = (($resultCnt == 0) && ($contentStatus == 'lrn_cnt_sts_atv')) ? 'edit-catalog-course-save-publish' : $saveOnlyBtnCls;
        
        if ($contentStatus != 'lrn_cnt_sts_atv')
            addHiddenFieldToAddEditForm($form, 'PRESENTATION_SAVE_AND_PUBLISH', "TRUE");
        addHiddenFieldToAddEditForm($form, 'CONTENT_STATUS', $contentStatus);
        $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#id' => $setButtonId,
            '#prefix' => $savePublishPrefix,
            '#suffix' => $savePublishSuffix,
            '#value' => $buttonLabel,
            '#name' => 'saveandpublish',
            '#attributes' => $savePublishAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => $ajaxCallbackSave,
                'customButtonId' => $customHiddenButtonId,
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        expDebug::dPrint("abhi check66666666666" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent . "=========" . $resultCnt);
        
        if (!$isSaveParent) {
            expDebug::dPrint("abhi check7777777777" . $contentId . " == " . $contentStatus . "=====" . $isSaveParent . "=========" . $resultCnt);
            
            $form['addedit-button-group-ul-start'] = array(
                '#type' => 'markup',
                '#markup' => '<ul class="catalog-pub-add-list">'
            );
        }
        
        expDebug::dPrint("Shri::" . $contentId . " == " . $contentStatus);
        
        if (($contentId) && $contentStatus == 'lrn_cnt_sts_atv') {
            if ($resultCnt == 0) {
                $saveButtonId                         = 'catalog-course-save-unpublish';
                $saveUnPublishAttributes['onclick']   = array();
                $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
                
                //		$saveUnPublishAttributes['onclick'][] = 'saveInteractionsH5PWrapper_presentation();';
                
                addHiddenFieldToAddEditForm($form, 'PRESENTATION_UNPUBLISH', "TRUE");
                $saveUnPublishAttributes['class']          = array(
                    'addedit-edit-' . $saveButtonId
                );
                $saveUnPublishAttributes['class'][]        = 'addedit-form-expertusone-throbber';
                $saveUnPublishAttributes['data-wrapperid'] = array(
                    $wrapperId
                );
                $form[$saveButtonId]                       = array(
                    '#type' => 'submit',
                    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                    '#suffix' => '</li>',
                    '#value' => t('LBL571'),
                    '#name' => 'unpublish',
                    '#attributes' => $saveUnPublishAttributes,
                    '#ajax' => array(
                        'wrapper' => $wrapperId,
                        'callback' => $ajaxCallbackSave,
                        'customButtonId' => $customHiddenButtonId,
                        'method' => 'replace',
                        'effect' => 'none', // 'fade',
                        'event' => 'click',
                        'keypress' => true,
                        'progress' => array(
                            'type' => 'throbber',
                            'message' => ''
                        )
                    )
                );
            }
        }
        $deleteCheck = adminVisibilityForAddEditDelete($entityId, 'cre_sys_obt_cnt_aut_pre', 'cre_sys_obt_cnt_aut_pre', 'slt_content_master');
        if ($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1) {
            if (($contentId)) {
                if ($resultCnt == 0) {
                    $displayTitle = titleController('EXP-SP-ADMIN-CONTENT-MODULE-TITLE', $contentDetails[0]->title, 34);
                    $saveButtonId = 'catalog-course-delete';
                    
                    $deleteAttributes['onclick'] = array();
                    addHiddenFieldToAddEditForm($form, 'PRESENTATION_DELETE', "TRUE");
                    $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("' . t('MSG357') . ' ' . strtolower(t('Content')) . ' ' . mysqli_escape_string($contentDetails[0]->title) . '","' . $contentId . '","Content"); return false;';
                    $deleteAttributes['class'][]   = 'admin-delete-button-bg';
                    $form[$saveButtonId]           = array(
                        '#type' => 'button',
                        '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                        '#suffix' => '</li>',
                        '#value' => t('LBL286'),
                        '#name' => 'delete',
                        '#attributes' => $deleteAttributes
                    );
                }
            }
        }
        
        $form['addedit-button-group-ul-end'] = array(
            '#type' => 'markup',
            '#markup' => '</ul><div class="clearBoth"></div></div>' . '<div class="clearBoth"></div></div>'
        );
        unset($saveAttributes['tabindex']);
        $saveAttributes['onclick']   = array();
        $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
        $saveAttributes['onclick'][] = 'increasePopWidth();';
        $form[$esignHiddenButt]      = array(
            '#type' => 'submit',
            '#value' => t('hidden-save'),
            '#name' => 'hidden-save',
            '#prefix' => '<span id = "esign-admin-div" style="display:none">',
            '#suffix' => '</span>',
            '#id' => $esignHiddenButt,
            '#attributes' => $saveAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => 'addEditContentAuthorRepaintForm',
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
        
        
        
        
        $form['addedit-left-col-end'] = array(
            '#type' => 'markup',
            '#markup' => '</div><div class="clearBoth"></div>'
        );
    }
    catch (Exception $ex) {
        watchdog_exception('addEditFormWithContentCancelAndSaveClose', $ex);
        expertusErrorThrow($ex);
    }
}


/*
 * addEditContentAuthorRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditContentAuthorRepaintForm($form, &$form_state)
{
    try {
        expDebug::dPrint("Suresh Author::" . print_r($form_state, true));
        if ($form_state["values"]["VIDEO_DELETE"] == "TRUE") {
            $form["videofilenamedisplay"] = "";
        }
        if ($form_state["values"]["POSTER_DELETE"] == "TRUE") {
            $form["posterfilenamedisplay"] = "";
        }
        if($form_state["values"]["recordtype"] == "upload"){
            $form["url"]["#disabled"] = true;
        }        
        $addtoedit = false;
        if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {
            // Create the additional now editable forms if any present
            $formInfoList = $form_state['storage']['form_info_list'];
            $argsList     = $form_state['build_info']['args'];
            array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList
            expDebug::dPrint(' $$argsList = ' . print_r($argsList, true), 4);
            $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
            $addtoedit         = true;
            
        }
        
        $commands   = array();
        $commands[] = ajax_command_replace(NULL, drupal_render($form));
        
        if (!empty($renderedAddlForms)) {
            $commands[] = ajax_command_after(NULL, $renderedAddlForms);
        }
        
        // Show the status messages, if any.
        // Ensure the status messages are auto removed after a brief while
        $statusMsgHTML = theme('status_messages');
        expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true), 4);
        if (!empty($statusMsgHTML)) {
            // Show the messages
            //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
            $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
            $commands[] = ajax_command_invoke(NULL, "hideUploadVideoControlAndDisplayLabel");
            $commands[] = ajax_command_invoke(NULL, "putCustomLabelForContentAuthor");
            $commands[] = ajax_command_invoke(NULL, "videoOnClose"); //ajax_command_refresh_narrow_search_results('onclose');
            
            
            
            
            
        }
        
        // Resize the modal backdrop, adjust the model top (and left) to optimal
        $commands[] = exp_ctools_modal_adjust();
        
        // If a new entity was added or an existing entity was edited, inform ctools to refresh narrow search results no modal close.
        if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['step'] == TRUE || $form_state['storage']['step'] == "edit") {
            
            //$commands[] = ajax_command_invoke(NULL, "redirectToInteractions");
        	$commands[] = ajax_command_refresh_narrow_search_results('onclose');
            $commands[] = ajax_command_invoke(NULL, "hideUploadVideoControlAndDisplayLabel");
            
            
        }
        
        if (module_exists('exp_sp_esignature') && isActiveEsignRegion()) {
            $custButtonId = "esign-contentauthor-addedit-form"; // $form_state['triggering_element']['#ajax']['customButtonId'];
            $drupalUserId = $form_state['triggering_element']['#ajax']['drupalUserId'];
            $drupalUserId = $form_state['values'][$drupalUserId];
            $esignObj     = '{"popupDiv":"modalContent","esignFor":"AddAdmin","drupalUserId":"' . $drupalUserId . '","esignButtId":"' . $custButtonId . '"}';
            if ($form_state['esign_show_popup'] == true) {
                $commands[]                     = ajax_command_invoke(NULL, 'getNewEsignPopup', array(
                    $esignObj
                ));
                $form_state['esign_show_popup'] = false; // this is ignored, as form state is not saved after the ajax callback is executed
            }
        }
        
        expDebug::dPrint("addtoedit:" . $addtoedit);
        if ($addtoedit || $form_state['storage']['substep'] == "details")
            $commands[] = ajax_command_invoke(NULL, "hideUploadVideoControlAndDisplayLabel");
        //$commands[] = ajax_command_invoke(NULL, "putCustomLabelForContentAuthor" );
        expDebug::dPrint('suresh... $commands = ' . print_r($commands, true), 4);
        
        return array(
            '#type' => 'ajax',
            '#commands' => $commands
        );
    }
    catch (Exception $ex) {
        watchdog_exception('addEditContentRepaintForm', $ex);
        expertusErrorThrow($ex);
    }
}
function getH5PContentId($content_master_id)
{
    
    try {
        $select = db_select('slt_content_master_h5p_rel', 'mast');
        $select->innerJoin('h5p_nodes', 'h5p', 'mast.h5p_id = h5p.content_id');
        
        $select->addField('mast', 'id', 'id');
        $select->addField('h5p', 'content_id', 'h5p_content_id');
        $select->addField('h5p', 'nid', 'h5p_id');
        $select->addField('h5p', 'json_content', 'json_content');
        $select->condition('mast.content_master_id', $content_master_id, '=');
        expDebug::dPrintDBAPI('$select sabeena11111=', $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint('v$result$result$result->' . print_r($result, true), 4);
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentDetailsInfoView', $ex);
        expertusErrorThrow($ex);
    }
}
function getContentAuthorDetailsInfoView($contentId)
{
    try {
        expDebug::dPrint('JustACheck-->' . print_r($contentId, true), 4);
        $select = db_select('slt_content_master', 'mast');
        $select->leftJoin('slt_profile_list_items', 'slpi', 'mast.type = slpi.code');
        $select->leftJoin('slt_profile_list_items', 'prof', 'mast.lang_code = prof.code');
        $select->innerJoin('slt_content_master_h5p_rel', 'h5prel', 'h5prel.content_master_id = mast.id');
        $select->innerJoin('h5p_nodes', 'h5p', 'h5p.content_id = h5prel.h5p_id');
        $select->addField('mast', 'id', 'id');
        $select->addField('mast', 'code', 'code');
        //$select -> addField('mast', 'type', 'type');
        $select->addField('mast', 'description', 'description');
        $select->addField('prof', 'name', 'lang');
        $select->addField('slpi', 'name', 'type');
        $select->addField('mast', 'subtype', 'subtype');
        $select->addField('h5p', 'json_content', 'interactions');
        $select->condition('mast.id', $contentId, '=');
        expDebug::dPrintDBAPI('$select sabeena=', $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint('v$result$result$result->' . print_r($result, true), 4);
        return $result;
    }
    catch (Exception $ex) {
        watchdog_exception('getContentDetailsInfoView', $ex);
        expertusErrorThrow($ex);
    }
}

function list_ca_forms(&$form, &$form_state, $defaultValue)
{
    try {
         $form['content_author_disp'] = array(
            '#type' => 'markup',
            '#prefix' => '<div class="catalog-course-basic-addedit-action-disp" id="catalog-course-basic-addedit-action-disp" style="display:none;">'
        ); 
        
        if (!isset($form_state['storage']['substep'])) {
            $form_state['storage']['substep'] = '';
        }
  //      $isSelected               = ($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep'])) ? 'selected' : '';
        $form['edit_btn_details'] = array(
            '#type' => 'submit',
            '#value' => t('Video'),
            '#name' => 'details',
            '#prefix' => '<ul><li><span>',
            '#suffix' => '</span></li>',
            '#attributes' => array(
                'Class' => array(
                    'form-submit addedit-form-expertusone-throbber edit-edit-btn-details',
               
                ),
                'data-wrapperid' => 'contentauthor-addedit-form'
            ),
            '#ajax' => array(
                'callback' => 'addEditContentAuthorRepaintForm',
                'wrapper' => 'contentauthor-addedit-form',
                'event' => 'click'
            )
        );
        $entityId                 = $form['mapped_course_id']['#value'];
        expDebug::dPrint(":::::suresh step:" . $form_state['storage']['step']);
        expDebug::dPrint(":::::suresh substep:" . $form_state['storage']['substep']);
        expDebug::dPrint(":::::suresh TEST entityid:" . $entityId);
        
//         if ($entityId) {
        //    $isSelected                  = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';
            $form['edit_btn_prerequest'] = array(
                '#type' => 'button',
            	//'#type' => 'submit',
                '#value' => t('LBL287') . t(' ') . t('LBL3021'),
                '#name' => 'prerequisite',
                '#prefix' => '<li><span>',
                '#suffix' => '</span></li>',
                '#attributes' => array(
                    'Class' => array(
                        'form-submit addedit-form-expertusone-throbber edit-edit-btn-prerequest',
                      
                    ),
                    'data-wrapperid' => 'contentauthor-addedit-form'
                )
            ); 
            
//         } 
        /* else {
            $form['edit_btn_prerequest_disabled'] = array(
                '#type' => 'item',
            	'#title' => t('LBL287') . t(' ') . t('LBL3021'),
                '#prefix' => '<li><span class="deactive-links">',
                '#suffix' => '</span></li>'
            );
            
            $form['course_actions_ul_container_disp_end'] = array(
                '#type' => 'markup',
                '#suffix' => '</ul>'
            );
        } */
        $form['course_actions_disp_end'] = array(
            '#type' => 'markup',
            '#suffix' => '</div>'
        );
    }
    catch (Exception $ex) {
        watchdog_exception('list_forms', $ex);
        expertusErrorThrow($ex);
    }
}


function content_share_display($courseId, $contentId, $deliveryType)
{
    expDebug::dPrint("content_share_display....." . $contentId . "==" . $courseId);
    try {
        
        $form_state = array(
            'no_redirect' => TRUE,
            'build_info' => array(
                'args' => array(
                    'ajax'
                )
            ),
            'enroll_entity_id' => $contentId,
            'enroll_entity_type' => 'cre_sys_obt_cnt',
            'class_id' => $contentId,
            
            'call_from' => 'catalog_enroll_display'
        );
        
        $formId                 = 'catalog_enroll_form_html1';
        $builtForm              = drupal_build_form($formId, $form_state);
        $renderedTabContentHTML = drupal_render($builtForm);
        $drupalSettings         = getDrupalSettings();
        
        $response = array(
            'drupal_settings' => $drupalSettings,
            'render_content_main' => $renderedTabContentHTML
        );
        
        echo drupal_json_output($response);
        exit;
    }
    catch (Exception $ex) {
        watchdog_exception('content_share_display', $ex);
        expertusErrorThrow($ex);
    }
}


function catalog_enroll_form_html1($form, &$form_state)
{
    try {
        expDebug::dPrint("catalog_enroll_form_html1...." . print_r($form_state, true));
        include_once(drupal_get_path('module', 'exp_sp_administration_catalog') . '/exp_sp_administration_catalog.inc');
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') . '/exp_sp_administration_class_register.inc');
        
        expDebug::dPrint("catalog_enroll_form_html1: --", 4);
        $entityId   = $form_state['enroll_entity_id'];
        $entityType = $form_state['enroll_entity_type'];
        $uniqueId   = $entityId . '-' . $entityType;
        
        $form['addedit-enroll_launch_start'] = array(
            '#type' => 'markup',
            '#markup' => '<div id="catalog-class-basic-addedit-form-container" class="catalog-course-basic-addedit-form-container">'
        );
        displayContentShareRegister($form, $form_state, $form_state['course_id'], $entityId, $entityType, $uniqueId, $form_state['enroll_delivery_type'], $isclose = 1);
        $form['addedit-enroll_launch_end'] = array(
            '#type' => 'markup',
            '#markup' => '</div>'
        );
        return $form;
    }
    catch (Exception $ex) {
        watchdog_exception('catalog_enroll_form_html', $ex);
        expertusErrorThrow($ex);
    }
}

function catalog_enroll_form_html1_validate($form, &$form_state)
{
    expDebug::dPrint('catalog_enroll_form_html_validate triggered.');
    
    try {
        //exp_sp_administration_catalog_class_addedit_form_validate($form, $form_state);
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') . '/exp_sp_administration_class_register.inc');
        //enrollUserValidateCheck($form_state);
        $triggerElementByName = $form_state['triggering_element']['#name'];
        if ($triggerElementByName == 'hidden-save' || $triggerElementByName == 'set-enrolluser') {
            $form_state['hidden-save-submit'] = true;
            $form_state['esign_show_popup']   = false;
        }
    }
    catch (Exception $ex) {
        watchdog_exception('catalog_enroll_form_html_validate', $ex);
        expertusErrorThrow($ex);
    }
}

function catalog_enroll_form_html1_submit($form, &$form_state)
{
    try {
        expDebug::dPrint("Catalog class catalog_enroll_form_html1_submit: " . print_r($form_state, true), 5);
        $entityType = 'cre_sys_obt_cnt';
        $entityId   = $form_state['enroll_entity_id'];
        $uniqueId   = $entityId . '-' . $entityType;
        expDebug::dPrint("Catalog class catalog_enroll_form_html1_submit entityType 1: " . $entityType, 4);
        expDebug::dPrint("Catalog class catalog_enroll_form_html1_submit uniqueId 1: " . $uniqueId, 4);
        if (isset($form_state['values']['attr_action']) && isset($entityId)) {
            expDebug::dPrint("Catalog class catalog_enroll_form_html1_submit entityType 2: " . $entityType, 4);
            if ($form_state['values']['attr_action'] == 'shareuser') {
                expDebug::dPrint("Catalog class catalog_enroll_form_html1_submit entityType 3: " . $entityType, 4);
                addNewEnrollUserDetails1($form, $form_state, $entityId, $entityType);
                
                // Clear enrolluser-autocomplete form textfield as switching from enroll user form to roster form - fix for ticket #0022120
                //clearClassRosterEnrollUserAutocomplete($form_state);
            }
            $form_state['rebuild'] = TRUE;
            unset($_SESSION['esign_validate']);
        }
    }
    catch (Exception $ex) {
        watchdog_exception('catalog_enroll_form_html_submit', $ex);
        expertusErrorThrow($ex);
    }
}

function displayContentShareRegister(&$form, &$form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType = '', $isclose = 0)
{
    try {
        
        global $secure_entities;
        expDebug::dPrint(' $clsDeliveryType = ' . $clsDeliveryType, 4);
        
        $customHiddenButtonId = 'enroll-user-toclass-esign-button';
        if (in_array($entityType, $secure_entities)) {
            $entityId_encrypted = core_encrypt($entityId); // URL encryption
        } else {
            $entityId_encrypted = $entityId;
        }
        $uniqueId = "shareuser-$entityId_encrypted-$entityType";
        
        $wrapperId                      = 'shareuser-grid-wrapper-' . $uniqueId;
        $formName                       = 'shareuser_form_' . $uniqueId;
        $form_state['form_unique_name'] = $formName;
        
        
        $select = db_select("slt_content_master", "scm");
        $select->addField('scm', 'id', 'id');
        $select->addField('scm', 'type', 'type');
        
        $select->condition('scm.id', $entityId, '=');
        $result = $select->execute()->fetchAll();
        expDebug::dPrint('v$result$result$result->' . print_r($result, true), 4);
        // $msg = t('MSG506');
        //$msg = t('The video is not shared.Click on share with others to share.');
        if ($result[0]->type == 'lrn_cnt_typ_vod')
            $msg = t('MSG786') . t(' ') . t('MSG787');
        else
            $msg = t('MSG785') . t(' ') . t('MSG787');
        
        $form['attr_action'] = array(
            '#type' => 'hidden',
            '#name' => 'attr_action',
            '#value' => 'shareuser'
            
        );
        
        $setminHight = ($entityType == 'cre_sys_obt_cnt') ? 'admin-course-class-tab-datagrid-wrapper' : '';
        
        $form[$formName] = array(
            '#type' => 'markup',
            '#prefix' => '<div id="admin-data-grid"><div id="admin_list_values_container" class="enrolled-display-controller"><div id="' . $wrapperId . '" class="admin-datagrid-pagination enrolluser-class-grid-wrapper ' . $setminHight . '">',
            '#suffix' => '</div></div></div></div>'
        );
        
        $form[$formName]['hidden_idlist'] = array(
            '#type' => 'hidden',
            '#name' => 'hidden_idlist_' . $uniqueId,
            '#id' => 'datagrid-idlist-' . $uniqueId
        );
        
        $form[$formName]['hidden_valuelist'] = array(
            '#type' => 'hidden',
            '#name' => 'hidden_valuelist_' . $uniqueId,
            '#id' => 'hidden-valuelist-' . $uniqueId
        );
        
        
        $uniqueId2      = $entityId . $entityType;
        $registrationId = '#registration_date_container_' . $uniqueId2;
        $checkPastClass = false;
        $isPastClass    = false;
        
        $displayMode = $form_state['storage']['mode'];
        
        $enrolluserDetails = fetchShareUserList($courseId, $entityId, $entityType, '', '', '', '', '', $displayMode, 1);
        $userIdArr         = array();
        $excludedUserId    = '""';
        $count             = $enrolluserDetails[0]->enrollment_count;
        $excludedUserId    = implode(',', $userIdArr);
        $excludedUserId    = $excludedUserId == "" ? 0 : $excludedUserId;
        
        $listAvailableUserResult = loadCtoolDataGridForEnrollUser1('', 0, $entityId, 1);
        expDebug::dPrint("Available user:" . $listAvailableUserResult);
        if ($count == 0 && empty($displayMode)) {
            
            if ($isPricedClass > 0.00)
                $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-greyout-bg';
            else
                $addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            
            $addAttributes['data-wrapperid'] = array(
                $wrapperId
            );
            
            if (isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view') {
                
                $form[$formName]['add_prereqiuisite_text'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="no-records-msg" id="expertus-no-online-users-msg-' . $uniqueId . '">' . t('There are no users enrolled for this class.') . '</div></div>'
                );
                
            } else {
                
                if ($listAvailableUserResult == 0) {
                    $msg = t('MSG788');
                    
                    //51141: Modified by joolavasavi
                    $form[$formName]['enrollment_start'] = array(
                        '#type' => 'markup',
                        '#markup' => '<table id="enrollment-upload-assign" cellpadding="1" cellspacing="0" width="100%" border="0"><tr>'
                    );
                    
                    $form[$formName]['add_enrolluser'] = array(
                        '#type' => 'markup',
                        '#prefix' => '<td width="40%" class="clsEnrollUser"><div class="dt-add-btn-stage-one enrollment-upload-container" ><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
                        '#suffix' => '<div class="admin-save-button-middle-greyout-bg">' . t('LBL3036') . '</div><div class="admin-save-button-right-greyout-bg"></div></span>'
                    );
                    
                    $form[$formName]['add_enrolluser_text'] = array(
                        '#type' => 'markup',
                        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-' . $uniqueId . '">' . $msg . '</div></div></td>'
                    );
                    
                    
                    
                } else {
                    
                    $form[$formName]['attachment_part_disp_start'] = array(
                        '#type' => 'markup',
                        '#markup' => '<table id="enrollment-upload-assign" cellpadding="1" cellspacing="0" width="100%" border="0"><tr>'
                    );
                    
                    $form[$formName]['attachment_upload_file_disp_start'] = array(
                        '#type' => 'markup',
                        '#markup' => '<td width="40%" class="clsEnrollUser">'
                    );
                    
                    if ($isPricedClass > 0.00) {
                        $prefix  = 'admin-save-button-left-greyout-bg';
                        $suffix  = 'admin-save-button-right-greyout-bg';
                        $disable = true;
                    } else {
                        $prefix  = 'admin-save-button-left-bg';
                        $suffix  = 'admin-save-button-right-bg';
                        $disable = false;
                    }
                    $form[$formName]['add_enrolluser'] = array(
                        '#type' => 'submit',
                        '#prefix' => '<div><div class="dt-add-btn-stage-one enrollment-upload-container"><span class="add-button-courseclass-tp"><div class="' . $prefix . '"></div>',
                        '#suffix' => '<div class="' . $suffix . '"></div></span></div>',
                        '#title' => t('Share User'),
                        '#name' => 'Add enrolluser',
                        //'#value' => t('Share User'),
                        '#value' => t('LBL3036'),
                        '#disabled' => $disable,
                        '#submit' => array(
                            'add_shareuser'
                        ),
                        '#attributes' => $addAttributes,
                        '#ajax' => array(
                            'callback' => 'shareUserGrid',
                            'wrapper' => $wrapperId
                            //'arguments' => array('Presentation Admin Perm'),
                        )
                    );
                    $enrollmentUpload                  = '<span class="attach-btn-group-middle"><div class="admin-save-button-left-bg"></div><span class="admin-save-button-middle-bg" >' . t('LBL609') . '</span><div class="admin-save-button-right-bg"></div></span>';
                    if ($isPricedClass > 0) {
                        $disableRosterUpload = true;
                        $enrollmentUpload    = '<span class="attach-btn-group-middle"><div class="admin-save-button-left-greyout-bg"></div><span class="admin-save-button-middle-greyout-bg" >' . t('LBL609') . '</span><div class="admin-save-button-right-greyout-bg"></div></span>';
                    }
                    $form[$formName]['add_enrolluser_text']      = array(
                        '#type' => 'markup',
                        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-' . $uniqueId . '">' . $msg . '</div></div></td>'
                    );
                    $path                                        = base_path() . drupal_get_path('module', 'uc_credit');
                    $form[$formName]['attachment_link_url_disp'] = array(
                        '#type' => 'markup',
                        '#markup' => '<td class="attachment-seperate-display-td-cls" width="5%"><span class="attachment-seperate-display"></span><span class="or-seperate-display"> ' . t('LBL644') . ' </span><span class="attachment-seperate-display"></span>
      			</td><td width="40%" class="clsEnrollUpload">
      			<div class="enrollment-upload-container">' . $enrollmentUpload . '</div>'
                    );
                    $browseAttributes                            = array();
                    $browseAttributes['class'][]                 = 'attach-browse';
                    $browseAttributes['class'][]                 = 'enrol-upload-browse-hid';
                    $browseAttributes['onChange']                = 'document.getElementById(\'enrollment_upload_done_btn\').click()';
                    $form[$formName]['enrollment_upload_file']   = array(
                        '#type' => 'file',
                        '#id' => 'enrollment_upload_file',
                        '#title' => t('Choose a file'),
                        '#attributes' => $browseAttributes,
                        '#size' => 1,
                        '#theme_wrappers' => array()
                    );
                    if ($isPricedClass > 0) {
                        $form[$formName]['enrollment_upload_file']['#disabled'] = TRUE;
                    }
                    $form[$formName]['attachment_upload_file_disp_end'] = array(
                        '#type' => 'markup',
                        '#markup' => '<div class="admin-enroll-empty-text-msg" id="upload-file-id">' . t('MSG740') . '<img src="' . $path . '/images/info.png" class="vtip info-enr-upload"
      												title="' . t("MSG741") . "<br><br>-" . t("MSG742") . '<br>-' . t('MSG743') . '" />
      												</div></td></tr></table>'
                    );
                    
                    $doneBtnAttributes                         = array();
                    $doneBtnAttributes['class'][]              = 'addedit-form-expertusone-throbber';
                    $doneBtnAttributes['data-wrapperid']       = array(
                        $wrapperId
                    );
                    $doneBtnAttributes['onclick'][]            = 'return false;';
                    $form[$formName]["enrollment_upload_done"] = array(
                        '#type' => 'submit',
                        '#id' => 'enrollment_upload_done_btn',
                        '#prefix' => '<div style="display:none;">',
                        '#suffix' => '</div>',
                        '#value' => t('Done'),
                        '#name' => 'done',
                        '#submit' => array(
                            'enrollment_upload'
                        ),
                        '#attributes' => $doneBtnAttributes,
                        '#ajax' => array(
                            'wrapper' => $wrapperId,
                            'callback' => 'shareUserGrid',
                            'entity_id' => $entityId,
                            'entity_type' => $entityType,
                            'method' => 'replace',
                            'effect' => 'none', // 'fade',
                            'event' => 'click',
                            'keypress' => true,
                            'progress' => array(
                                'type' => 'throbber',
                                'message' => ''
                            )
                        )
                    );
                }
            }
            
        } else {
            expDebug::dPrint("wel..." . $displayMode);
            $searchAttributes = array();
            if ($displayMode == 'edit') {
                
                $paintMultiAction = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
                $paintMultiAction .= "<span id='search-dropdwn-list'>";
                $paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
                $paintMultiAction .= "<input type='hidden' id='enrolluser-autocomplete_hidden' value='" . t('LBL181') . "' />";
                $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>" . t('LBL054') . "</span>";
                $paintMultiAction .= "<a  id='admin-dropdown-arrow' class='select-list-dropdown-link' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
                $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
                $paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','Class');\">" . t('LBL054') . "</li>";
                $paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','Class');\">" . t('LBL107') . "</li>";
                $paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','Class');\">" . t('Organization') . "</li>";
                $paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','Class');\">" . t('Group') . "</li>";
                $paintMultiAction .= "</ul>";
                $paintMultiAction .= "</span></div>";
                
                
                $form[$formName]['enroll_multi_search_container'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="admin_add_multi_search_container">'
                );
                $form[$formName]['more_add_enrolluser_search']    = array(
                    '#type' => 'markup',
                    '#markup' => $paintMultiAction
                );
                
                $acMenuPath = 'administration/contentauthor/video/notshareduser-autocomplete';
                
                $form[$formName]['add_enrolluser_search'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="admin_add_multi_auto_search">'
                );
                
                $autoCompleteAttributes['class'][]     = 'admin_ac_input_mainform';
                $autoCompleteAttributes['onkeydown'][] = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
                addACFieldForMultiSelect($form[$formName], 'enrolluser-autocomplete', $acMenuPath, '', '', '', array(), array(
                    'entity_id' => "'$entityId'",
                    'entity_type' => "'$entityType'",
                    'search_type' => "$('#search_all_enroll_type-hidden').val()"
                ), false, false, t('LBL181'), $autoCompleteAttributes);
                
                $form[$formName]['add_enrolluser_search1'] = array(
                    '#type' => 'markup',
                    '#markup' => '</div>'
                );
                
                $searchlink                           = "<a title = '" . t("LBL304") . "' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"shareuser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0);'></a>";
                $form[$formName]['search_enrolluser'] = array(
                    '#type' => 'markup',
                    '#markup' => $searchlink
                );
                $form[$formName][]                    = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>'
                );
                
            } else {
                $displayMode      = 'view';
                $paintMultiAction = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
                $paintMultiAction .= "<span id='search-dropdwn-list'>";
                $paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
                $paintMultiAction .= "<input type='hidden' id='enrolluser-autocomplete_hidden' value='" . t('LBL181') . "' />";
                $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>" . t('LBL054') . "</span>";
                $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
                $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
                $paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','Class');\">" . t('LBL054') . "</li>";
                $paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','Class');\">" . t('LBL107') . "</li>";
                $paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','Class');\">" . t('Organization') . "</li>";
                $paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','Class');\">" . t('Group') . "</li>";
                $paintMultiAction .= "</ul>";
                $paintMultiAction .= "</span></div>";
                
                $form[$formName]['enroll_multi_search_container'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="admin_add_multi_search_container">'
                );
                $form[$formName]['more_add_enrolluser_search']    = array(
                    '#type' => 'markup',
                    '#markup' => $paintMultiAction
                );
                
                $acMenuPath = 'administration/contentauthor/video/shareduser-autocomplete';
                
                $form[$formName]['add_enrolluser_search'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="admin_add_multi_auto_search">'
                );
                
                
                $autoCompleteAttributes['class'][]     = 'admin_ac_input_mainform';
                $autoCompleteAttributes['onkeydown'][] = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
                addACFieldForMultiSelect($form[$formName], 'enrolluser-autocomplete', $acMenuPath, '', '', '', array(), array(
                    'entity_id' => "'$entityId'",
                    'entity_type' => "'$entityType'",
                    'id_required' => 0,
                    'search_type' => "$('#search_all_enroll_type-hidden').val()"
                ), false, false, t('LBL181'), $autoCompleteAttributes);
                
                $form[$formName]['add_enrolluser_search1'] = array(
                    '#type' => 'markup',
                    '#markup' => '</div>'
                );
                
                $searchAttributes['Class'][] = 'admin-pagination-search-go';
                $searchlink                  = "<a title = '" . t("LBL304") . "' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"shareuser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0);'></a>";
                
                $form[$formName]['search_enrolluser'] = array(
                    '#type' => 'markup',
                    '#markup' => $searchlink
                );
                $form[$formName][]                    = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>'
                );
                
                $enrollment_icon_class = '';
                if ($isPricedClass > 0) {
                    $enrollment_icon_class = ' disabled';
                }
                $addClassName                                   = ($form_state['call_from'] == 'catalog_enroll_display') ? '' : 'alter-margin-top';
                $browseAttributes                               = array();
                $browseAttributes['class'][]                    = 'attach-browse';
                $browseAttributes['class'][]                    = 'enrol-upload-browse-hid-view';
                $browseAttributes['class'][]                    = 'vtip';
                $browseAttributes['class'][]                    = 'browse-button-vtip';
                $browseAttributes['class'][]                    = 'info-enr-upload';
                $browseAttributes['onChange']                   = 'document.getElementById(\'enrollment_upload_done_btn_view\').click()';
                $browseAttributes['title']                      = ''; //empty text for title - If enabled vtip behaviour will be affected
                $browseAttributes['titletemp']                  = $isPricedClass > 0 ? t('LBL1246') : t("MSG741") . "<br><br>-" . t("MSG742") . '<br>-' . t('MSG743');
                $form[$formName]['enrollment_upload_file_view'] = array(
                    '#type' => 'file',
                    '#id' => 'enrollment_upload_file_view',
                    '#title' => t('Choose a file'),
                    '#attributes' => $browseAttributes,
                    '#size' => 1,
                    '#theme_wrappers' => array()
                );
                if ($isPricedClass > 0) {
                    $form[$formName]['enrollment_upload_file_view']['#disabled'] = TRUE;
                }
               
                
            }
            
            $form[$formName]['enrolluser_view_grid_markup'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="datagrid-div-' . $uniqueId . '"><table id="datagrid-container-' . $uniqueId . '"></table><div id="pager-datagrid-' . $uniqueId . '" class="pager-datagrid-common"></div></div>'
            );
            
            $form[$formName]['statistics_count'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="enrollment-statistics-count" id="statistics-count-' . $uniqueId . '"></div>'
            );
            
            $displayModeDataGrid                       = $form_state['storage']['display_mode'] == 'view' ? 'view_only' : $displayMode;
            $form[$formName]['content_view_grid_load'] = array(
                '#type' => 'markup',
                '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-' . $uniqueId . '\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'' . $displayModeDataGrid . '\', \'shareuser\', \'\', \'' . $entityId_encrypted . '\', \'' . $entityType . '\', \'0\');" width="100" height="132" /></div>'
            );
            
            if ($displayMode == 'view') {
                $addAnotherAttributes['class'][]        = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
                $addAnotherAttributes['data-wrapperid'] = array(
                    $wrapperId
                );
                
                
                if ($isclose) {
                    $btnclose = 1;
                } else {
                    $btnclose = 0;
                }
                addEditSaveFormForEnrollUser1($form, $formName, $wrapperId, 'save-button', 'cancel-button', '', $entityId, $entityType, $uniqueId, $listAvailableUserResult, $clsDeliveryType, true, $classHasContent, $btnclose);
                
            } else if ($displayMode == 'edit') {
                $cancelAttributes['class'][]        = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
                $cancelAttributes['data-wrapperid'] = array(
                    $wrapperId
                );
                
                $form[$formName]['cancel_enrolluser_edit_mode'] = array(
                    '#type' => 'submit',
                    '#value' => t('LBL109'),
                    '#title' => t('LBL109'),
                    '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2"><div class="white-btn-bg-left"></div>',
                    '#suffix' => '<div class="white-btn-bg-right"></div>',
                    '#name' => 'Cancel enrolluser',
                    '#submit' => array(
                        'cancelEnrollUserDetails'
                    ),
                    '#attributes' => $cancelAttributes,
                    '#ajax' => array(
                        'callback' => 'shareUserGrid',
                        'wrapper' => $wrapperId
                    )
                );
                
                
                $enrollFormElementName = 'set_enrolluser_edit_mode';
                $enrollUserPrefix      = '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>';
                if ((strtotime($sessionStartdate) < strtotime($currentdate)) && $checkPastClass) {
                    $isPastClass                                          = true;
                    $sessionStartdateForPicker                            = date('m/d/Y', strtotime($sessionStartdate));
                    $registrationContainer                                = <<<EOD
<div class="registration_container_roster" id="registration_date_container_$uniqueId2">
<a style="float: right; position: relative; cursor: pointer;" class="qtip-button"  onClick="$('$registrationId').hide();"><div class="admin-bubble-close"> </div></a>
$registrationdatelbl:  <input size="8" type="text" name="overall_registration_date_$uniqueId2" id="overall_registration_date_$uniqueId2" class="enrollement-date-picker" value="$sessionStartdateForPicker"/>
	<div id="enroll-user-registration-date-popup" class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span onClick="$('$registrationId').hide();" class="admin-action-button-middle-bg white-btn-bg-middle">$closelbl</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg registration-button-space"></div><input type="button" class="admin-save-button-middle-bg"  value="$donelbl" onClick="$('#root-admin').data('narrowsearch').fillRegistrationDate('$uniqueId', '$uniqueId2'); $('#add_new_user_$uniqueId2').click();  return false;"/><div class="admin-save-button-right-bg"></div></div>
</div>
EOD;
                    $enrollUserPrefix                                     = $registrationContainer . '<div class="admin-save-button-container" onClick="$(\'#root-admin\').data(\'narrowsearch\').registrationDateQTip(\'' . $uniqueId2 . '\',\'' . $entityId . '\',\'' . $isPastClass . '\',\'' . $sessionStartdate . '\'
							); return false;"><div class="admin-save-button-left-bg"></div><input class="admin-save-button-middle-bg" onclick="clearMessages();" type="button" value="Share User" name="set-enrolluser">';
                    $enrollFormElementName                                = 'set_enrolluser_edit_mode_pastclass';
                    $form[$formName]['hidden_registration_date_required'] = array(
                        '#type' => 'hidden',
                        '#name' => 'hidden_registration_date_required',
                        '#id' => 'hidden_registration_date_required',
                        '#value' => 'required'
                    );
                    expDebug::dPrint("isPastClass " . "hidden field added", 5);
                }
                expDebug::dPrint("isPastClass" . print_r($isPastClass, 1), 5);
                
                if ($listAvailableUserResult != 0) {
                    
                    
                    if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && 1 == 0) { // e-Signature option should disabled
                        $buttonName = 'esign-enrolluser';
                    } else {
                        $buttonName = 'set-enrolluser';
                    }
                    $setAttributes['class'][]        = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
                    $setAttributes['data-wrapperid'] = array(
                        $wrapperId
                    );
                    $setAttributes['onclick'][]      = 'clearMessages();';
                    if ($isPastClass) {
                        $setAttributes['class'][] = 'element-invisible';
                    }
                    $form[$formName][$enrollFormElementName] = array(
                        '#type' => 'submit',
                        '#id' => 'add_new_user_' . $uniqueId2,
                        '#prefix' => $enrollUserPrefix,
                        '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
                        '#title' => t('LBL305'),
                        '#value' => t('Share'),
                        '#name' => $buttonName,
                        '#attributes' => $setAttributes,
                        '#ajax' => array(
                        	'callback' => 'addEditRepaintShareToObject',
                            'wrapper' => $wrapperId,
                            'entity_id' => $entityId,
                            'entity_type' => $entityType,
                            'course_id' => $courseId,
                            'entity_plugin' => 'enrolluser',
                            'customButtonId' => $customHiddenButtonId,
                            'method' => 'replace',
                            'effect' => 'none', // 'fade',
                            'event' => 'click',
                            'keypress' => true,
                            'progress' => array(
                                'type' => 'throbber',
                                'message' => ''
                            )
                        )
                    );
                    $setAttributes                           = array();
                    $form[$formName][$customHiddenButtonId]  = array(
                        '#type' => 'submit',
                        '#value' => t('hidden-save'),
                        '#name' => 'hidden-save',
                        '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
                        '#suffix' => '</span>',
                        '#id' => $customHiddenButtonId,
                        '#attributes' => $setAttributes,
                        '#ajax' => array(
                            'wrapper' => $wrapperId,
                        	'callback' => 'addEditRepaintShareToObject',
                            'entity_id' => $entityId,
                            'entity_type' => $entityType,
                            'course_id' => $courseId,
                            'method' => 'replace',
                            'effect' => 'none', // 'fade',
                            'event' => 'click',
                            'keypress' => true,
                            'progress' => array(
                                'type' => 'throbber',
                                'message' => ''
                            )
                        )
                    );
                }
            }
            $form_state['storage']['mode'] = '';
        }
    }
    catch (Exception $ex) {
        watchdog_exception('displayClassRegisterDetails', $ex);
        expertusErrorThrow($ex);
    }
}


function addEditSaveFormForEnrollUser1(&$form, $formName, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $subStage = '', $entityId, $entityType, $uniqueId, $listAvailableUserResult, $clsDeliveryType = '', $courseClassStatus, $classHasContent, $btnclose = 0)
{
    try {
        global $theme_key;
        // Remove the temporary storage for API's to exchange information during form creation
        $esignHiddenButt    = 'esign-' . $wrapperId;
        // Add the Cancel button
        $ajaxCallbackCancel = 'addEditCancelRepaintForm';
        //$ajaxCallbackCancel = 'enrollUserGrid';
        
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $cancelAttributes['class'] = array(
            'addedit-edit-' . $cancelButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit cancel buttons
        //$cancelAttributes['class'][] = 'addedit-edit-cancel';
        
        // Using this class to applying bg color to the save button
        $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $cancelAttributes['class'][]        = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $cancelAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        
        // A common class for all addedit form cancel button, for styling to appear as a link.
        $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
        
        // Remove the error messages or success messages on clicking cancel
        if ($btnclose)
            $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();' . "closeQtip('','$entityId');  return false; ";
        else
            $cancelAttributes['onclick'] = '$(".active-qtip-div").remove();' . "closeQtyp('qtip_editclass_visible_dispid_" . core_encrypt($entityId) . "','$entityId');closeQtyp('#qtipAttachIdqtip_addclass_visible_disp_" . core_encrypt($entityId) . "','$entityId'); return false; ";
        
        $uniqueId2 = $entityId . $entityType;
        
        $form[$formName]['attachment_link_url_disp-view'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-and-save-actions-row enroll-edit-save-cancel-btn">'
        );
        
        
        
        $doneBtnAttributes                                  = array();
        $doneBtnAttributes['class'][]                       = 'addedit-form-expertusone-throbber';
        $doneBtnAttributes['data-wrapperid']                = array(
            $wrapperId
        );
        $doneBtnAttributes['onclick'][]                     = 'return false;';
        $form[$formName]["enrollment_upload_done_btn_view"] = array(
            '#type' => 'submit',
            '#id' => 'enrollment_upload_done_btn_view',
            '#prefix' => '<div style="display:none;">',
            '#suffix' => '</div>',
            '#value' => 'uploaduser',
            '#name' => 'done',
            '#submit' => array(
                'enrollment_upload'
            ),
            '#attributes' => $doneBtnAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => 'shareUserGrid',
                'entity_id' => $entityId,
                'entity_type' => $entityType,
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
        $form[$formName][$cancelButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="addedit-form-cancel-container-actions enrolluser-save-cancel-btn-container"><div class="white-btn-bg-left"></div>',
            '#suffix' => '<div class="white-btn-bg-right"></div>',
            '#value' => t('LBL123'),
            '#name' => 'cancel',
            '#attributes' => $cancelAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => $ajaxCallbackCancel,
                'entity_id' => $entityId,
                'entity_type' => $entityType,
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
        // Add the Save button
        
        $ajaxCallbackSave = 'shareUserGridCallback';
        //$wrapperId        = 'add-class-list-wrapper';
        
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $saveAttributes['class']      = array(
            'addedit-edit-' . $saveButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        $saveOtherAttributes['class'] = array(
            'addedit-edit-' . $saveButtonId
        ); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
        
        // For common styling of all addedit save buttons
        
        // Using this class to applying bg color to the save button
        $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
        
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][]      = 'addedit-form-expertusone-throbber';
        $saveOtherAttributes['class'][] = 'addedit-form-expertusone-throbber';
        
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid']      = array(
            $wrapperId
        );
        $saveOtherAttributes['data-wrapperid'] = array(
            $wrapperId
        );
        
        //Increase the width of the popup while clicking on save button
        $saveAttributes['onclick'][] = 'increasePopWidth();';
        
        // Remove the error messages or success messages on clicking submit
        
        $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;
        $completionId        = '#completion_date_container_' . $uniqueId2;
        
        $currentdate = date('m/d/Y');
        //           $date = getClassStartDate($entityId,false);
        if (isset($form[$formName]['hidden_session_start_date'])) {
            $date = date('m/d/Y', strtotime($form[$formName]['hidden_session_start_date']['#value']));
        } else {
            $date = '';
        }
        expDebug::dPrint('session date modi' . $date);
        $date                = empty($date) ? $currentdate : $date;
        // EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader($wrapperId);
        $completiondatelbl   = t('LBL706');
        $closelbl            = t('LBL123');
        $donelbl             = t('LBL569');
        $completionContainer = <<<EOD
<div class="completion_container_roster" id="completion_date_container_$uniqueId2">
<a style="float: right; position: relative; cursor: pointer;" class="qtip-button"  onClick="$('$completionId').hide();"><div class="admin-bubble-close"> </div></a>
$completiondatelbl:  <input size="8" type="text" name="overall_completion_date_$uniqueId2" id="overall_completion_date_$uniqueId2" class="enrollement-date-picker" value="$date"/>
	<div id="enroll-user-completion-date-popup" class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span onClick="$('$completionId').hide();" class="admin-action-button-middle-bg white-btn-bg-middle">$closelbl</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg completion-button-space"></div><input type="button" class="admin-save-button-middle-bg"  value="$donelbl" onClick="$('#root-admin').data('narrowsearch').fillCompletionDate('$uniqueId', '$uniqueId2'); $('#completedandsave_$uniqueId2').click();  return false;"/><div class="admin-save-button-right-bg"></div></div>
</div>
EOD;
             
        
        
        $qtipOptWaivedObj = "{'entityId' : '" . $entityId . "'" . ", 'entityType' : '" . entityType . "'" . ", 'url' : 'administration/enrollment/exempted/all/" . $entityId . "/0/class/0'" . ", 'popupDispId' : 'enrolled-all-exempted-" . $entityId . "'" . ", 'catalogVisibleId' : 'enrolled-all-exempted-disp'" . ", 'wid' : 400" . ", 'heg' : '200'" . ", 'postype' : 'topright'" . ",'poslwid' :'' " . ", 'qdis' : 'ctool'" . ", 'linkid' : 'visible-all-exempted-" . $entityId . "','enrId' : '" . entityId . "'" . ", 'waivedfrom' : 'all'}";
        
        
        $setclass                       = ($theme_key == 'expertusoneV2') ? 'admin-save-button-left-bg' : '';
        $isSaveParent                   = 1;
        $saveAttributes['onclick']      = array();
        $saveAttributes['onclick'][]    = 'clearMessages();';
       
        $form[$formName][$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-pub-unpub-button-container"><span id="setwaived-visible-container"><span id="enrolled-all-exempted-' . $entityId . '" class="enrolled-exempted-all-status"><a id="visible-all-exempted-' . $entityId . '" class="enrolled-all-exempted" onclick = "exemptedVisibility(' . $qtipOptWaivedObj . ');"></a><span id="visible-popup-' . $entityId . '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span></span></span><div class="' . $setclass . '"></div>',
            '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn" style="display:none">&nbsp;</span></div></div>' . $completionContainer . '<ul class="catalog-pub-add-list">',
            //'#value' => t('LBL141'),
            '#value' => t('LBL3036'),
            '#name' => 'Add enrolluser',
            '#submit' => array(
                'add_shareuser'
            ),
            '#attributes' => $saveOtherAttributes,
            '#ajax' => array(
                'wrapper' => $wrapperId,
                'callback' => 'shareUserGrid',
                'entity_id' => $entityId,
                'entity_type' => $entityType,
                //  'customButtonId'=>$esignHiddenButt,
                'method' => 'replace',
                'effect' => 'none', // 'fade',
                'event' => 'click',
                'keypress' => true,
                'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                )
            )
        );
        
      
        
        // #0016788 - Classroom Session should not be able to mark complete for future sessions - Implemented
        $ses_start = getClassSessionStartDate($entityId);
        $tz        = $ses_start[0]->timezone_attr;
        if (!$tz)
            $tz = date_default_timezone(FALSE);
        $sessionTZ      = new DateTimeZone($tz);
        $strtotimeNow   = new DateTime(null, $sessionTZ);
        $stDate         = date_format(date_create($ses_start[0]->ses_start_date . " " . $ses_start[0]->start_time), 'Y-m-d H:i:sP');
        $ses_start_date = new DateTime($stDate, $sessionTZ);
        expDebug::dPrint("SESSION TIME CHECK - Current Date -- " . $strtotimeNow->format('Y-m-d H:i:sP') . "  ----- Session Start Date " . $ses_start_date->format('Y-m-d H:i:sP'), 4);
        if ($strtotimeNow >= $ses_start_date) {
            
           
            
        }
        
        $qtipId                      = 'completedandsave_' . $uniqueId2;
        $saveButtonId                = 'catalog-enrolluser-complete-save-hidden';
        $saveAttributes['onclick']   = array();
        $saveAttributes['onclick'][] = 'clearMessages();';

        $showEnrollUser              = 1;
        if (($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl')) {
            include_once(drupal_get_path('module', 'exp_sp_administration_learning') . '/exp_sp_administration_session_details.inc');
            $cls_seat_info     = _getClassInfo($entityId);
            $max_seats         = $cls_seat_info[0]->max_seats;
            $waitlist_count    = $cls_seat_info[0]->waitlist_count;
            $totAvailableSeats = $max_seats + $waitlist_count;
            $totEnroll         = getTotalClassEnroll($entityId);
            $showEnrollUser    = ($totAvailableSeats > $totEnroll) ? 1 : 0;
        }
        
        if ($entityType == 'cre_sys_obt_cls') {
            $classValidRec = checkClassValidity($entityId, $clsDeliveryType);
        }
        expDebug::dPrint(' Calss Status = ' . print_r($classValidRec, true), 5);
        expDebug::dPrint(' Class Status = ' . $classValidRec->classstatus, 5);
        $isPricedClass = getSltClassPrice($entityId);
        if ($isPricedClass == 0) {
            expDebug::dPrint('price is zero');
        }
        //  if ($listAvailableUserResult > 0 && $classHasContent && $showEnrollUser && $classValidRec->classstatus != 'lrn_cls_sts_can' && $isPricedClass == 0 ) 
        {
            $saveOtherAttributes['onclick']                             = array();
            $saveButtonId                                               = 'catalog-enrolluser-newuser';
            $form[$formName][$saveButtonId]['add_enrolluser_view_mode'] = array(
                '#type' => 'submit',
                '#title' => t('Share User'),
                '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                '#suffix' => '</li>',
                '#value' => t('Share User'),
                '#name' => 'Add enrolluser',
                '#submit' => array(
                    'add_shareuser'
                ),
                '#attributes' => $saveOtherAttributes,
                '#ajax' => array(
                    'wrapper' => $wrapperId,
                    'callback' => 'shareUserGrid',
                    'entity_id' => $entityId,
                    'entity_type' => $entityType,
                    //  'customButtonId'=>$esignHiddenButt,
                    'method' => 'replace',
                    'effect' => 'none', // 'fade',
                    'event' => 'click',
                    'keypress' => true,
                    'progress' => array(
                        'type' => 'throbber',
                        'message' => ''
                    )
                )
            );
        }
        $form[$formName]['addedit-button-group-ul-end'] = array(
            '#type' => 'markup',
            '#markup' => '</ul>'
        );
       
        $form[$formName]['addedit-left-col-end']        = array(
            '#type' => 'markup',
            '#markup' => '</div></div>'
        );
        
        $form[$formName]['load-scripts' . $uniqueId] = array(
            );
        
    }
    catch (Exception $ex) {
        watchdog_exception('addEditSaveFormForEnrollUser1', $ex);
        expertusErrorThrow($ex);
    }
}


function fetchShareUserList($courseId, $entityId, $entityType, $queryRequiredOrExportSearchType = '', $searchKeyword = '', $eliminateCancel = '', $exportOption = '', $searchType = '', $mode = '', $isCount = 0)
{
    try {
        if ($exportOption == 'CSV') {
            // There seems a limit to number of page arguments that can be in hook_menu from where this function is called for export to csv.
            $searchType    = $queryRequiredOrExportSearchType;
            $queryRequired = 'no';
        } else {
            $searchType    = $_GET['searhType'];
            $queryRequired = $queryRequiredOrExportSearchType;
        }
        $loggedInUserId = getIdOfLoggedInUser();
        
        expDebug::dPrint(' $CheckingForEntityID = ' . $entityId, 4);
        
        $select = db_select('slt_h5p_content_share', 'enroll');
        $select->join('slt_content_master_h5p_rel', 'rel', 'rel.content_master_id = ' . $entityId);
        $select->join('slt_person', 'per', 'per.id = enroll.object_id and enroll.h5p_content_id= rel.h5p_id'); // and enroll.object_id !='.$loggedInUserId);
        
        $select->leftjoin('slt_organization', 'org', 'org.id = per.org_id');
        $select->leftjoin('slt_groups', 'grp', 'FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');
        //	$select->leftjoin('slt_groups','grp1','FIND_IN_SET(per.id, grp1.userslist) > 0 AND (grp1.status = \'cre_sec_sts_atv\') ');
        
        //ADDING FOR #0074032
        $select->leftJoin('slt_group_privilege', 'p', 'grp.id=p.group_id');
        
        if ($isCount == 0) {
            $select->addField('enroll', 'id', 'id');
            $select->addField('enroll', 'h5p_content_id', 'h5p_content_id');
            $select->addField('enroll', 'object_id', 'registered_user_id');
            $select->addField('per', 'user_name', 'user_name');
            $select->addField('per', 'full_name', 'full_name');
            $select->addField('enroll', 'status', 'Status');
            $select->addField('enroll', 'created_on', 'created_on');
            $select->addField('enroll', 'updated_on', 'updated_on');
            $select->addField('org', 'name', 'organization_name');
            //$select->addField('grp','name','name');
            //$select->addExpression('group_concat(grp.name)','name');
            $select->addExpression('(select group_concat(g.name) from slt_groups g where find_in_set( per.id,g.userslist)>0)', 'name');
        } else {
            $select->addExpression('count(1)', 'enrollment_count');
            
        }
        
        //ADDING FOR #0074032
        $select->condition('p.priv_add', 1, '=');
        $select->condition('p.page_code', 'cre_sys_obt_crs');
        
        $select->where('FIND_IN_SET(per.id,grp.userslist)>0');
        
        
        
        
        expDebug::dPrint(' $searchKeyword = ' . $searchKeyword, 4);
        $searchTypeVal = t('LBL181');
        if ($searchType == 'user') {
            $searchTypeVal = t('LBL181');
        } elseif ($searchType == 'fullname') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL107');
        } elseif ($searchType == 'org') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL137');
        } elseif ($searchType == 'jobrole') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL133') . ' ' . t('LBL107'); //t('LBL133');
        } elseif ($searchType == 'usertype') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL173');
        } elseif ($searchType == 'manager') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL134'); //t('LBL180');
        } elseif ($searchType == 'status') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL102');
        }
        if ($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal) {
            if (!empty($searchKeyword) && $searchKeyword != $searchTypeVal) {
                $select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
                $filterArray[] = array(
                    t('LBL054') . ':' . $searchKeyword
                );
            }
        } elseif ($searchType == 'fullname') {
            $select->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $filterArray[] = array(
                t('LBL107') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'status') {
            $waivedCnt    = 0;
            $allRegStatus = getProfileItemNamesKeyed('lrn_crs_reg_');
            expDebug::dPrint('$allRegStatus = ' . print_r($allRegStatus, true), 4);
            $searchRegStatusList = array();
            foreach ($allRegStatus as $code => $name) {
                if (mb_stripos($name, $searchKeyword, null, 'UTF-8') !== false) {
                    $searchRegStatusList[] = $code;
                }
            }
            expDebug::dPrint('$searchRegStatusList = ' . print_r($searchRegStatusList, true), 4);
            
            $allCompStatus = getProfileItemNamesKeyed('lrn_crs_cmp_');
            expDebug::dPrint('$allCompStatus = ' . print_r($allCompStatus, true), 4);
            $searchCompStatusList = array();
            if (mb_stripos(t('Waived'), $searchKeyword, null, 'UTF-8') !== false) {
                /*$searchCompStatusList[] = '';
                $searchCompStatusList[] = '';*/
                $waivedCnt = 1;
            } else {
                foreach ($allCompStatus as $code => $name) {
                    if (mb_stripos($name, $searchKeyword, null, 'UTF-8') !== false) {
                        $searchCompStatusList[] = $code;
                    }
                }
            }
            
            expDebug::dPrint('$searchCompStatusList = ' . print_r($searchCompStatusList, true), 4);
            if (empty($waivedCnt)) {
                if (empty($searchRegStatusList) && empty($searchCompStatusList)) {
                    $select->where('1=0');
                } elseif (empty($searchCompStatusList)) {
                    $select->condition('enroll.reg_status', $searchRegStatusList, 'IN');
                } elseif (empty($searchRegStatusList)) {
                    $select->condition('enroll.comp_status', $searchCompStatusList, 'IN');
                } else {
                    $select->condition(db_or()->condition('enroll.reg_status', $searchRegStatusList, 'IN')->condition('enroll.comp_status', $searchCompStatusList, 'IN'));
                }
            } else {
                $select->where('if((SELECT exm.exempted_status as exmsts from slt_enrollment_exempted exm
						where exm.enrollment_id = enroll.id
						AND enroll.comp_status IN (\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\') AND
						exm.enroll_type = \'class\' order by exm.id desc limit 1) = 1 ,1=1,1=0)');
            }
            
            $filterArray[] = array(
                t('LBL102') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'org') {
            //$select->leftjoin('slt_organization','org','org.id = per.org_id');
            $select->condition('org.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $filterArray[] = array(
                t('Organization') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'jobrole') {
            $jobRoleCode = 'cre_usr_jrl_';
            $select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
            $select->leftjoin('slt_profile_list_items', 'splt', 'splt.code = spjm.job_role');
            $select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $select->condition('splt.code', db_like($jobRoleCode) . '%', 'LIKE');
            $filterArray[] = array(
                t('LBL133') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'usertype') {
            $select->leftjoin('slt_profile_list_items', 'splt', 'splt.code = per.user_type');
            $select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $select->condition('splt.code', db_like('cre_usr_ptp_') . '%', 'LIKE');
            $filterArray[] = array(
                t('LBL173') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'manager') {
            $select->leftjoin('slt_person', 'pers1', 'pers1.id = per.manager_id');
            $select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
            $filterArray[] = array(
                t('Manager') . ':' . $searchKeyword
            );
        } elseif ($searchType == 'group' && $searchKeyword != t('LBL1270')) { // Type a user name
            //}elseif($searchType == 'group'){
            expDebug::dPrintDBAPI('When is this called ==', $searchType);
            //	$select->join('slt_groups', 'grp');
            $select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
            $select->join('slt_profile_list_items', 'pli', 'per.status=pli.code');
            $select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		   			AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		   			AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		   			AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		   			AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		   			AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
						AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role IS NULL OR FIND_IN_SET(ifnull(spjm.job_role,''),ifnull(grp.job_role,''))>0))
		   			AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		   			AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
		   			AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
            $select->condition('grp.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            //$select->condition('grp.is_admin',0,'=');
            $select->condition('grp.is_admin', 1, '=');
            $select->condition('enroll.status', 'shared');
            $filterArray[] = array(
                t('Group') . ':' . $searchKeyword
            );
        }
        
       
        if ($isCount == 0) {
            $select->groupBy('enroll.object_id');
        }
        expDebug::dPrintDBAPI(' SQL fetch fetchShareUserList = ', $select);
        $select->condition('enroll.status', 'shared');
        if (empty($queryRequired)) {
            return $select->execute()->fetchAll();
        } else {
            if ($exportOption == 'CSV') {
                $colHeadersMap = array(
                    t('LBL054') => 'user_name',
                    t('LBL107') => 'full_name',
                    t('LBL102') => 'exempted_overallstatus',
                    t('LBL668') => 'score'
                );
                
                
                SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'class_roster');
            } else {
                return $select;
            }
        }
    }
    catch (Exception $ex) {
        watchdog_exception('fetchEnrollUserList', $ex);
        expertusErrorThrow($ex);
    }
    
}


/*
 * addEditRepaintShareToObject() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *
 */
function addEditRepaintShareToObject($form, &$form_state)
{
    try {
        expDebug::dPrint('addEditRepaintShareToObject calleed on');
        global $secure_entities;
        $form_state['storage']['mode'] = 'edit';
        
        $formName   = $form_state['form_unique_name'];
        $entityId   = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];        
        if (in_array($entityType, $secure_entities)) {
        	  $uniqueId = "shareuser-" . core_encrypt($entityId) . "-$entityType"; // URL encryption to retrive data      
        } else {
        	$uniqueId = "shareuser-" . $entityId . "-$entityType"; // URL encryption to retrive data
        }
        
        $commands = array();
        if ($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_' . $uniqueId]) || !empty($form_state['hidden_idlist_' . $uniqueId]))) {
            $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
        }
        $commands[] = exp_ctools_modal_adjust();
        
        $statusMsgHTML = theme('status_messages');
        expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true), 4);
        if (!empty($statusMsgHTML)) {
            // Show the messages
            $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
        }
        
        if (module_exists('exp_sp_esignature') && isActiveEsignRegion()) {
            $custButtonId = $form_state['triggering_element']['#ajax']['customButtonId'];
            $esignObj     = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"' . $custButtonId . '"}';
            if ($form_state['esign_show_popup'] == true)
                $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup', array(
                    $esignObj
                ));
        }
        return array(
            '#type' => 'ajax',
            '#commands' => $commands
        );
    }
    catch (Exception $ex) {
        watchdog_exception('addEditRepaintShareToObject', $ex);
        expertusErrorThrow($ex);
    }
}

function addNewShareContent($form, &$form_state, $entityId, $entityType, $associatedUserId)
{
    try { 
        $nodeSelect = db_select('slt_content_master_h5p_rel', 'entity');
        $nodeSelect->addField('entity', 'h5p_id', 'h5p_id');
        $nodeSelect->condition('entity.content_master_id', $entityId);
        
        $nodeResult = $nodeSelect->execute()->fetchAll();
        
        $cart_msg    = "";
        $object_type = "user";
        $object_id   = $associatedUserId; //$userId;
        
        $h5p_content_id = $nodeResult[0]->h5p_id;
        $createUserId   = getIdOfLoggedInUser();
        $insertShare    = db_insert('slt_h5p_content_share');
        $fields         = array(
            'status' => 'Shared',
            'object_type' => $object_type,
            'object_id' => $object_id,
            'h5p_content_id' => $h5p_content_id,
            'created_by' => $createUserId,
            'created_on' => now(),
            'updated_by' => $createUserId,
            'updated_on' => now()
        );
        $insertShare->fields($fields);
     //   $insertId = $insertShare->execute();
        $cart_msg = "Shared";
        
        //sendCourseClassDetail1($entityId,$entityType,$associatedUserId);
      //  checkShareNotificationOverRideInfo1($entityId, $entityType, $associatedUserId, 'share_notification_h5p', getIdOfLoggedInUser());
        
        
        
        expDebug::dPrint("addNewShareContent:::" . $insertId);
    }
    catch (Exception $ex) {
        $cart_msg = "Error";
        watchdog_exception('addEditRepaintShareToObject', $ex);
        expertusErrorThrow($ex);
    }
    
    return $cart_msg;
}


function addNewEnrollUserDetails1($form, &$form_state, $entityId, $entityType)
{
    global $secure_entities;
    try {
        
        expDebug::dPrint("addNewEnrollUserDetails1...." . print_r($form_state, 3));
        
        $entityId             = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType           = $form_state['triggering_element']['#ajax']['entity_type'];
        $associateIdListArray = array();
        
        $valuesSuccess    = array();
        $valuesUnSuccess  = array();
        $valuesEquWarning = array();
        $valuesRecurReg   = array();
        
        
        if (in_array($entityType, $secure_entities)) {
            $uniqueId = "shareuser-" . core_encrypt($entityId) . "-$entityType"; // URL Encryption to reretive data
        } else {
            $uniqueId = "shareuser-" . $entityId . "-$entityType"; // URL Encryption to reretive data
        }
        
        if (!empty($form_state['input']['hidden_idlist_' . $uniqueId]) || !empty($form_state['hidden_idlist_' . $uniqueId])) {
            if (!empty($form_state['input']['hidden_idlist_' . $uniqueId]))
                $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_' . $uniqueId]);
            else
                $associateIdListArray = explode(',', $form_state['hidden_idlist_' . $uniqueId]);
        }
        
        $flag = "";
        foreach ($associateIdListArray as $associatedUserId) {
            $valuesReturn = addNewShareContent($form, $form_state, $entityId, $entityType, $associatedUserId);
            $Regsvalue    = '';
            expDebug::dPrint("addNewEnrollUserDetails valuesReturn...." . $valuesReturn . "===" . t('LBL190'));
            
            if ($valuesReturn == t('LBL190')) {
                $valuesSuccess[] = "<ul><li><div><span class='enrolluser-name-list'>" . $fullName . "</span> - <span>" . $valuesReturn . "</span></div></li></ul>";
                $Regsvalue .= 'default';
           /*  } else if ($valuesReturn == t('Shared')) {
                $Regsvalue .= 'Shared';
            } */
                } else if ($valuesReturn == 'Shared') { //Changes made for multi-language support
                	$Regsvalue .= 'Shared';
                }
            
            $flag = 0;
        }
        if (count($valuesSuccess) > 0) {
            $returnMsg   = implode('', $valuesSuccess);
            $displayTxt1 = '<div><div class="registered-msg-enrolluser">' . t('MSG714') . '</div><div class="registered-msg-enrolluser-list">' . $returnMsg . '</div>';
            drupal_set_message($displayTxt1);
        }
        if (count($valuesUnSuccess) > 0) {
            $returnMsg   = implode('', $valuesUnSuccess);
            $displayTxt2 = t('MSG432') . " " . t('MSG433');
            drupal_set_message($displayTxt2, 'error');
            $resultcnt = count($valuesUnSuccess);
            for ($i = 0; $i < $resultcnt; $i++) {
                $displayTxt2 = $valuesUnSuccess[$i];
                drupal_set_message($displayTxt2, 'error');
            }
        }
        
        if ($Regsvalue != '') {
            if (!strstr($Regsvalue, 'default')) {
                $displayTxt1 = '<div><div class="registered-msg-enrolluser">' . t('Shared') . '</div>';
                drupal_set_message($displayTxt1);
                
            } else if (strstr($Regsvalue, 'Shared')) {
                $displayTxt1 = '<div><div class="registered-msg-enrolluser">' . t('Others') . ' ' . t('Shared') . '</div>';
                drupal_set_message($displayTxt1);
            }
        } else {
            $flag = 1;
        }
        
        
        if ($flag == 0) {
            $form_state['storage']['mode'] = 'view';
            $form_state['rebuild']         = TRUE;
        }
        expDebug::dPrint("addNewEnrollUserDetails2..$uniqueId.." . $form_state['input']['hidden_idlist_' . $uniqueId]);
        expDebug::dPrint("addNewEnrollUserDetails2 form .." . $form_state['hidden_idlist_' . $uniqueId]);
    }
    catch (Exception $ex) {
        //$txn->rollback();
        watchdog_exception('addNewEnrollUserDetails', $ex);
        throw $ex;
    }
    
  
    
}

//loadCtoolDataGridForEnrollUser1('', 0, $entityId, 1);

function loadCtoolDataGridForEnrollUser1($searchKeyword, $queryRequired = '', $entityId = '', $countRequired = '', $is_compliance = '', $deliveryType = '')
{
    try {
        $searchType = $_GET['searhType'];
        expDebug::dPrintDBAPI('Abhishek checking search type=', $searchType);
        $sord   = $_GET['sord'];
        $select = db_select('slt_person', 'per');
        
        //ADDING FOR GROUP FILTER
        $select->leftJoin('slt_groups', 'grp', 'FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');
        
        //ADDING FOR #0074032
        $select->leftJoin('slt_group_privilege', 'p', 'grp.id=p.group_id');
        
        if ($countRequired) {
            $select->addExpression('count(per.id)', 'count');
        } else {
            $select->addField('per', 'id', 'id');
            $select->addField('per', 'user_name', 'user_name');
            $select->addField('per', 'full_name', 'full_name');
            //$select->addExpression('(select mgr.full_name from slt_person mgr where mgr.id = per.manager_id)', 'manager_name');
            $select->addExpression('(select name from slt_organization where id = per.org_id)', 'organization_name');
            $select->addExpression('(select group_concat(g.name) from slt_groups g where find_in_set( per.id,g.userslist)>0)', 'name');
        }
        //ADDING FOR #0074032	
        $select->condition('p.priv_add', 1, '=');
        $select->condition('p.page_code', 'cre_sys_obt_crs');
        
        $select->condition('per.status', 'cre_usr_sts_atv');
        $select->condition('per.user_name', array(
            'admin',
            'guest'
        ), 'NOT IN');
        
        $select->where('FIND_IN_SET(per.id,grp.userslist)>0');
        
        $searchTypeVal = t('LBL181');
        if ($searchType == 'user') {
            $searchTypeVal = t('LBL181');
        } else if ($searchType == 'fullname') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL107');
        } elseif ($searchType == 'org') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL137');
        } elseif ($searchType == 'jobrole') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL133') . ' ' . t('LBL107'); //t('LBL133');
        } elseif ($searchType == 'usertype') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL173');
        } elseif ($searchType == 'manager') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL134'); //t('LBL180');
        } elseif ($searchType == 'status') {
            $searchTypeVal = t('LBL036') . ' ' . t('LBL102');
        }
        
        if ($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal) {
            if (!empty($searchKeyword) && $searchKeyword != $searchTypeVal) {
                $select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            }
        } elseif ($searchType == 'fullname') {
            $select->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
        } elseif ($searchType == 'org') {
            $select->leftjoin('slt_organization', 'org', 'org.id = per.org_id');
            $select->condition('org.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
        } elseif ($searchType == 'jobrole') {
            $select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
            $select->leftjoin('slt_profile_list_items', 'splt', 'splt.code = spjm.job_role');
            //$select->leftjoin('slt_profile_list_items','splt','splt.code = per.job_role');
            $select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $select->condition('splt.code', db_like('cre_usr_jrl_') . '%', 'LIKE');
        } elseif ($searchType == 'usertype') {
            $select->leftjoin('slt_profile_list_items', 'splt', 'splt.code = per.user_type');
            $select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            $select->condition('splt.code', db_like('cre_usr_ptp_') . '%', 'LIKE');
        } elseif ($searchType == 'manager') {
            $select->leftjoin('slt_person', 'pers1', 'pers1.id = per.manager_id');
            $select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
        } elseif ($searchType == 'group' && $searchKeyword != t('LBL1270')) { // Type a group name
            //$select->join('slt_groups', 'grp');
            $select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
            $select->join('slt_profile_list_items', 'pli', 'per.status=pli.code');
            $select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		   			AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		   			AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		   			AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		   			AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		   			AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
						AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role IS NULL OR FIND_IN_SET(ifnull(spjm.job_role,''),ifnull(grp.job_role,''))>0))
		   			AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		   			AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
		   			AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
            $select->condition('grp.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
            //	$select->condition('grp.is_admin',0,'=');
            $select->condition('grp.is_admin', 1, '=');
        }
        $select1 = db_select('slt_h5p_content_share', 'enroll');
        $select1->addField('enroll', 'object_id');
        
        $select1->join('slt_content_master_h5p_rel', 'rel', 'rel.content_master_id = ' . $entityId . '  and enroll.h5p_content_id = rel.h5p_id');
        
        $select1->where("enroll.object_id = per.id and enroll.status='Shared'");
        
        
        $select->notExists($select1);
        
        if (!$countRequired) {
            // asc and desc for Enroll User => username In admin Roster #0038272
            if ($sord == 'desc') {
                $select->orderBy('per.full_name', 'DESC');
            } else {
                $select->orderBy('per.full_name');
            }
        }
        $select->distinct();
        
        expDebug::dPrintDBAPI('loadCtoolDataGridForEnrollUser1 $select1=', $select);
        expDebug::dPrint('loadCtoolDataGridForEnrollUser1 ABHISHEKcheck=' . print_r($select, true), 4);
        if (empty($queryRequired)) {
            if ($countRequired) {
                return $select->execute()->fetchField();
            } else {
                return $select->execute()->fetchAll();
            }
        } else {
            return $select;
        }
    }
    catch (Exception $ex) {
        watchdog_exception('loadCtoolDataGridForEnrollUser1', $ex);
        expertusErrorThrow($ex);
    }
}


function add_shareuser($form, &$form_state)
{
    try {
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') . '/exp_sp_administration_class_register.inc');
        
        // Clear enrolluser-autocomplete form textfield as switching from roster form form to enroll user form - fix for ticket #0022120
        clearClassRosterEnrollUserAutocomplete($form_state);
        
        $form_state['storage']['mode'] = 'edit';
        $form_state['rebuild']         = TRUE;
    }
    catch (Exception $ex) {
        watchdog_exception('add_enrolluser', $ex);
        expertusErrorThrow($ex);
    }
}


function shareUserGrid($form, &$form_state)
{
    try {
        $formName = $form_state['form_unique_name'];
        return $form[$formName];
    }
    catch (Exception $ex) {
        watchdog_exception('shareUserGrid', $ex);
        expertusErrorThrow($ex);
    }
}

function shareUserGridCallback($form, &$form_state)
{
    try {
        global $secure_entities;
        $formName   = $form_state['form_unique_name'];
        $entityId   = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
        
        unset($_SESSION['exempted-class-' . $entityId]);
        
        if (in_array($entityType, $secure_entities)) {
            $uniqueId = "enrolluser-" . core_encrypt($entityId) . "-$entityType"; // URL encryption
        } else {
            $uniqueId = "enrolluser-$entityId-$entityType"; // URL encryption
        }
        $commands = array();
        if (!empty($form_state['input']['hidden_idlist_' . $uniqueId])) {
            $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
        }
        // Cleared char length for the Message - fix for ticket #0035601
        // $statusMsgHTML = theme('status_messages',array('char_length' => 100));
        $statusMsgHTML = theme('status_messages');
        expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true), 4);
        if (!empty($statusMsgHTML)) {
            // Show the messages
            
            $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
        }
        $commands[] = exp_ctools_modal_adjust();
        
        return array(
            '#type' => 'ajax',
            '#commands' => $commands
        );
    }
    catch (Exception $ex) {
        watchdog_exception('enrollUserGridCallback', $ex);
        expertusErrorThrow($ex);
    }
}


/*
 * getSharedUserAutoComplete() - Returns the matched person names that are enrolled to a class
 */
function getSharedUserAutoComplete()
{
    try {
        $nameSubstr = $_GET['z'];
        
        $entityId   = orgValidate($_GET['entity_id']);
        $entityType = $_GET['entity_type'];
        $searchType = $_GET['search_type'];
        
        $contentDet = db_select('slt_content_master_h5p_rel', 'rel');
        $contentDet->addField('rel', 'h5p_id', 'h5p_id');
        $contentDet->condition('rel.content_master_id', $entityId, '=');
        $result = $contentDet->execute()->fetchAll();
        $contentId = $result[0]->h5p_id;
        
        
        // Select tables
        $select = db_select('slt_h5p_content_share', 'enroll');
        $select->join('slt_person', 'per', 'per.id = enroll.object_id and enroll.h5p_content_id=' . $contentId);
        
        // Set conditions
        //$select->condition('enroll.h5p_content_id', $courseId, '=');
        if ($searchType == 'user' || $searchType == '') {
            if (!empty($nameSubstr)) {
                //Modified for the ticket no:18743
                $select->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'like');
            }
        } else if ($searchType == 'fullname') {
            //Modified for the ticket no:0029263
            $select->addExpression('DISTINCT(per.full_name)', 'name');
            $select->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'like');
        }
        
        elseif ($searchType == 'org') {
            $select->leftjoin('slt_organization', 'org', 'org.id = per.org_id');
            $select->addExpression('DISTINCT(org.name)', 'name');
            $select->condition('org.status', array(
                'cre_org_sts_itv',
                'cre_org_sts_act'
            ), 'IN');
            $select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
            $select->orderBy('org.name');
        } elseif ($searchType == 'group') {
            //$select = db_select('slt_groups', 'grps');
            $select->leftjoin('slt_groups', 'grp', 'FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');
            $select->leftjoin('slt_group_mapping', 'grpmap', 'grp.id = grpmap.group_id');
            $select->addExpression('DISTINCT(grp.name)', 'name');
            $select->condition('grp.name', '%' . db_like($nameSubstr) . '%', 'like');
            $select->condition('grp.is_admin', 1, '=');
            $select->orderBy('name');
            
        }
        
        
        
        $select->condition('enroll.status', array(
            'Shared',
            'NotShared'
        ), 'IN');
        if ($searchType == 'user' || $searchType == t('LBL107') || $searchType == '') {
            // Fields to fetch
            $select->addExpression('DISTINCT(per.user_name)', 'name');
            $select->orderBy('name');
        }
        
        expDebug::dPrintDBAPI('suresh $select SQL = enrollment ', $select);
        
        $matchingQuery = $select->execute()->fetchAll();
        expDebug::dPrint(' $matchingPersons = ' . print_r($matchingQuery, true), 3);
        
        foreach ($matchingQuery as $result) {
            if ($searchType == 'status') {
                print t($result->name) . "\n";
            } else {
                print $result->name . "\n";
            }
        }
        
    }
    catch (Exception $ex) {
        watchdog_exception('getEnrolledUserAutoComplete', $ex);
        expertusErrorThrow($ex);
    }
}


/*
 * getNotSharedUserAutoComplete() - Returns the matched person names that can be enrolled to a class and are not already enrolled
 */
function getNotSharedUserAutoComplete()
{
    try {
        $entityId   = $_GET['entity_id'];
        $entityType = $_GET['entity_type'];
        $searchType = $_GET['search_type'];
        $nameSubstr = $_GET['z'];
        
        
        $contentDet = db_select('slt_content_master_h5p_rel', 'rel');
        $contentDet->addField('rel', 'h5p_id', 'h5p_id');
        $contentDet->condition('rel.content_master_id', $entityId, '=');
        $result = $contentDet->execute()->fetchAll();
        $contentId = $result[0]->h5p_id;
        
        
        $runQuery = 1;
        if ($searchType == 'user' || $searchType == '') {
            // Select the table
            $select = db_select('slt_person', 'per');
            
            //Adding for Autofilter showing all admin names
            $select->leftJoin('slt_groups', 'grp', 'FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');
            $select->leftJoin('slt_group_privilege', 'p', 'grp.id=p.group_id');
            
            
            // Add fields to fetch
            $select->addExpression('DISTINCT(per.user_name)', 'name');
            
           
            // Add conditions
            if (!empty($nameSubstr)) {
                //Modified for the ticket no:18743
                $select->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'like');
            }
            //Adding for Autofilter showing all admin names
            $select->condition('p.priv_add', 1, '=');
            $select->condition('p.page_code', 'cre_sys_obt_crs');
            
            $chkEnrollments = true;
            if ($searchType == 'user' && $entityType == 'cre_sys_obt_cls') {
                $select2 = db_select('slt_business_rule_mapping', 'busrule');
                $select2->addField('busrule', 'entity_id');
                $select2->condition('busrule.entity_id', $entityId, '=');
                $select2->condition('busrule.business_rule_code', 'cre_sys_brl_rra', '=');
                
                $result2 = $select2->execute();
                $row_cnt = $result2->rowCount();
                if ($row_cnt > 0)
                    $chkEnrollments = false;
            }
          //removing as the person who created his name not being shown even though he is removed from the list  
           /*  if ($chkEnrollments) {
                $select1 = db_select('slt_h5p_content_share', 'enr');
                $select1->addField('enr', 'object_id');
                $select1->condition('enr.h5p_content_id', $contentId, '=');
                $select1->where('enr.object_id = per.id');
                $select->notExists($select1);
            } */
            
            $select->condition('per.user_name', array(
                'admin',
                'guest'
            ), 'NOT IN');
            $select->condition('per.status', "cre_usr_sts_atv");
            $select->orderBy('name');
        } elseif ($searchType == 'fullname') {
            $select = db_select('slt_person', 'per');
            
            //Adding for Autofilter showing all admin FullNames
            $select->leftJoin('slt_groups', 'grp', 'FIND_IN_SET(per.id, grp.userslist) > 0 AND (grp.status = \'cre_sec_sts_atv\') ');
            $select->leftJoin('slt_group_privilege', 'p', 'grp.id=p.group_id');
            
            $select->addExpression('DISTINCT(per.full_name)', 'name');
            $select->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'like');
            if (!empty($excludedUserId)) {
                $select->condition('per.id', $excludedUserId, 'NOT IN');
            }
            //Adding for Autofilter showing all admin names
            $select->condition('p.priv_add', 1, '=');
            $select->condition('p.page_code', 'cre_sys_obt_crs');
            
            
            $select->condition('per.full_name', array(
                'admin',
                'guest'
            ), 'NOT IN');
            $select->condition('per.status', "cre_usr_sts_atv");
            $select->orderBy('name');
        } elseif ($searchType == 'org') {
            $select = db_select('slt_organization', 'org');
            $select->addExpression('DISTINCT(org.name)', 'name');
            $select->condition('org.status', array(
                'cre_org_sts_itv',
                'cre_org_sts_act'
            ), 'IN');
            $select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
            $select->orderBy('name');
        } /* elseif ($searchType == 'jobrole') {
            $select = getMatchingProfileItemNamesKeyedautocomplete($_GET['z'], 'cre_usr_jrl_');
            $select->leftjoin('slt_person_jobrole_mapping', 'permap', 'permap.job_role = spli.code');
            $select->leftjoin('slt_person', 'per', 'per.id = permap.user_id');
            
        }  elseif ($searchType == 'usertype') {
            $itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_ptp_');
            foreach ($itemsList as $code => $name) {
                print $name . "\n";
            }
            $runQuery = 0;
        }  elseif ($searchType == 'manager') {
            $select = db_select('slt_person', 'per');
            $select->addExpression('DISTINCT(per.full_name)', 'name');
            $select->condition(db_or()->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'like')->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'like'));
            $select->condition('per.user_name', array(
                'admin',
                'guest'
            ), 'NOT IN');
            $select->condition('per.status', "cre_usr_sts_atv");
            $select->where('per.is_manager=\'Y\'');
            $select->orderBy('name');
        } */ elseif ($searchType == 'group') {
            $select = db_select('slt_groups', 'grps');
            $select->leftjoin('slt_group_mapping', 'grpmap', 'grps.id = grpmap.group_id');
            $select->addExpression('DISTINCT(grps.name)', 'name');
            $select->condition('grps.name', '%' . db_like($nameSubstr) . '%', 'like');
            $select->condition('grps.is_admin', 1, '=');
            $select->orderBy('name');
            
        }    
        if ($runQuery) {
            expDebug::dPrintDBAPI(' $select SQL = ', $select);
            
            $matchingQuery = $select->execute()->fetchAll();
            expDebug::dPrint(' $matchingPersons = ' . print_r($matchingPersons, true), 3);
            foreach ($matchingQuery as $result) {
                print $result->name . "\n";
            }
        }
    }
    catch (Exception $ex) {
        watchdog_exception('getNotEnrolledUserAutoComplete', $ex);
        expertusErrorThrow($ex);
    }
}

function getClonedVersionIdPath($srcpath, $content_version_id)
{
    $explodePath                                    = explode("/", $srcpath); //sample $path videos/20video_1/20video.mp4
    $updateNewCVIdArr                               = explode("_", $explodePath[1]);
    $updateNewCVIdArr[count($updateNewCVIdArr) - 1] = $content_version_id; //cloned conten version id
    $explodePath[1]                                 = implode("_", $updateNewCVIdArr);
    $clonedPath                                     = implode("/", $explodePath); //$path
    return $clonedPath;
}

function copyh5pfile($srcPath, $destPath)
{
    $path = pathinfo($destPath);
    mkdir($path['dirname'], 0777, true);
    expDebug::dPrint("Final form after destpath: " . print_r($destPath, true));
    $file              = new stdClass();
    $file->filename    = basename($srcPath);
    $file->filepath    = $srcPath;
    $file->filemime    = file_get_mimetype($srcPath);
    $file->filesize    = filesize($srcPath);
    $file->uid         = 1;
    $file->status      = FILE_STATUS_PERMANENT;
    $file->uri         = $srcPath;
    $file->display     = 1;
    $file->description = '';
    $file              = file_copy($file, $destPath, FILE_EXISTS_REPLACE);
}

function createH5PRecordPresentation($title)
{
    global $user;
    $iContentNode                    = new stdClass();
    $iContentNode->uid               = $user->uid;
    $iContentNode->type              = 'h5p_content';
    $iContentNode->language          = 'en-us';
    $iContentNode->title             = $title;
    $iContentNode->status            = 1;
    $iContentNode->promote           = 0;
    $iContentNode->sticky            = 0;
    $iContentNode->comment           = 2;
    $iContentNode->disable           = 0;
    $iContentNode->content_master_id = 'dummy';
    // reference to slt_content_master
    
    $jsondata                   = '{"presentation":{"slides":[{"elements":[],"keywords":[],"slideBackgroundSelector":{}}],"keywordListEnabled":true,"globalBackgroundSelector":{},"keywordListAlwaysShow":false,"keywordListAutoHide":false,"keywordListOpacity":90},"l10n":{"slide":"Slide","yourScore":"Your Score","maxScore":"Max Score","goodScore":"Congratulations! You got @percent correct!","okScore":"Nice effort! You got @percent correct!","badScore":"You got @percent correct.","Total":"Total","showSolutions":"Show solutions","retry":"Retry","title":"Title","author":"Author","lisence":"License","license":"License","exportAnswers":"Export text","copyright":"Rights of use","hideKeywords":"Hide keywords list","showKeywords":"Show keywords list","fullscreen":"Fullscreen","exitFullscreen":"Exit fullscreen","prevSlide":"Previous slide","nextSlide":"Next slide","currentSlide":"Current slide","lastSlide":"Last slide","solutionModeTitle":"Exit solution mode","solutionModeText":"Solution Mode","summaryMultipleTaskText":"Multiple tasks","scoreMessage":"You achieved:","shareFacebook":"Share on Facebook","shareTwitter":"Share on Twitter","summary":"Summary","solutionsButtonTitle":"Show comments","printTitle":"Print","printIngress":"How would you like to print this presentation?","printAllSlides":"Print all slides","printCurrentSlide":"Print current slide"},"override":{"activeSurface":false,"overrideButtons":false,"overrideShowSolutionButton":false,"overrideRetry":false,"hideSummarySlide":true}}';
    $iContentNode->nid          = '';
    $iContentNode->vid          = '';
    $iContentNode->changed      = '';
    $iContentNode->h5p          = '';
    $iContentNode->json_content = $jsondata;
    //$iContentNode -> filtered = $jsondata;
    
    $iContentNode->main_library_id = 13;
    $iContentNode->embed_type      = 'div';
    $iContentNode->h5p_type        = 'create';
    $iContentNode->h5p_library     = 'H5P.CoursePresentation 1.9';
    $iContentNode->created         = time();
    $iContentNode->revision        = 0;
    $iContentNode->timestamp       = time();
    $iContentNode->op              = "Save";
    $iContentNode->submit          = "Save";
    $iContentNode->preview         = "Preview";
    $iContentNode->form_id         = "h5p_content_node_form";
    expDebug::dPrint("Final form save: " . print_r($iContentNode, 1));
    
    node_save($iContentNode);
    $h5p_id           = h5p_get_content_id($iContentNode);
    $stdObj           = new stdClass();
    $stdObj->h5pId    = $h5p_id;
    $stdObj->jsonData = $jsondata;
    return $stdObj;
    //addNewShareContent($form, $form_state,$returnArr[0]->id,'',$loggedInUserId);
}

function insertH5PRecord($title, $content_master_id, $videoLink, $mime, $posterImage, $posterImageMime, $show_solution, $retry, $interactions, $summary, $bookmarks, $passingScore = '')
{
    $interactions = json_encode($interactions);
    $summary      = json_encode($summary);
    $bookmarks    = json_encode($bookmarks);
    if ($show_solution == "")
        $show_solution = "false";
    else
        $show_solution = "true";
    if ($retry == "")
        $retry = "false";
    else
        $retry = "true";
    
    global $user;
    $iContentNode                    = new stdClass();
    $iContentNode->uid               = $user->uid;
    $iContentNode->type              = 'h5p_content';
    $iContentNode->language          = 'en-us';
    $iContentNode->title             = $title;
    $iContentNode->status            = 1;
    $iContentNode->promote           = 0;
    $iContentNode->sticky            = 0;
    $iContentNode->comment           = 2;
    $iContentNode->disable           = 0;
    $iContentNode->content_master_id = $content_master_id;
    // reference to slt_content_master
    
    $jsondata                   = '{"interactiveVideo":{"video":{"passingscore":"' . $passingScore . '","title":"' . $iContentNode->title . '","startScreenOptions":{"hideStartTitle":false},"files":[{"path":"' . $videoLink . '","mime":"' . $mime . '","copyright":{"license":"U"}}],"poster":{"path":"' . $posterImage . '","mime":"' . $posterImageMime . '","copyright":{"license":"U"},"width":1988,"height":1062},"copyright":""},"assets":{"interactions":' . $interactions . ',"bookmarks":' . $bookmarks . '},"summary":{"task":{"library":"H5P.Summary 1.4","params":{"intro":"","summaries":[{"tip":""}],"solvedLabel":"Progress:","scoreLabel":"Wrong answers:","resultLabel":"Your result","response":{"scorePerfect":{"title":"PERFECT!","message":"You got everything correct on your first try!"},"scoreOver70":{"title":"Great!","message":"You got most of the statements correct on your first try."},"scoreOver40":{"title":"Ok","message":"You got some of the statements correct on your first try."},"scoreOver0":{"title":"A few mistakes.","message":"Have another try!"}},"summary":"You got @score of @total statements (@percent %) correct on your first try."},"subContentId":"086a6ae2-3ee8-4974-82ba-d9a0b9c6f98b"},"displayAt":3}},"override":{"overrideButtons":true,"overrideShowSolutionButton":' . $show_solution . ',"overrideRetry":' . $retry . '},"l10n":{"interaction":"Interaction","play":"Play","pause":"Pause","mute":"Mute","unmute":"Unmute","quality":"Video Quality","fullscreen":"Fullscreen","exitFullscreen":"Exit Fullscreen","summary":"Summary","bookmarks":"Bookmarks","defaultAdaptivitySeekLabel":"Continue"}}';
    $iContentNode->nid          = '';
    $iContentNode->vid          = '';
    $iContentNode->changed      = '';
    $iContentNode->h5p          = '';
    $iContentNode->json_content = $jsondata;
    //$iContentNode -> filtered = $jsondata;
    
    $iContentNode->main_library_id = 36;
    $iContentNode->embed_type      = 'div';
    $iContentNode->h5p_type        = 'create';
    $iContentNode->h5p_library     = 'H5P.InteractiveVideo 1.9';
    $iContentNode->created         = time();
    $iContentNode->revision        = 0;
    $iContentNode->timestamp       = time();
    $iContentNode->op              = "Save";
    $iContentNode->submit          = "Save";
    $iContentNode->preview         = "Preview";
    $iContentNode->form_id         = "h5p_content_node_form";
    expDebug::dPrint("Final form save: " . print_r($iContentNode, 1));
    
    node_save($iContentNode);
    $h5p_id = h5p_get_content_id($iContentNode);
    return $h5p_id;
    //addNewShareContent($form, $form_state,$returnArr[0]->id,'',$loggedInUserId);
}

function updateSharedUser($contentId, $usersId, $callByAPI = false)
{
    try {
        expDebug::dPrint('$contentId: ' . $contentId);
        expDebug::dPrint('$usersId' . $usersId);
        $queryArgs  = array(
            ':status' => 'Not shared',
            ':contentId' => $contentId,
            ':usersId' => $usersId
        );
        $userDelete = db_query("UPDATE slt_h5p_content_share h5p_share inner join slt_content_master_h5p_rel slt_mas on slt_mas.h5p_id = h5p_share.h5p_content_id SET h5p_share.status = :status where slt_mas.content_master_id = :contentId and h5p_share.id IN (:usersId)", $queryArgs);
        
        expDebug::dPrintDBAPI('User delete query', $userDelete, $queryArgs);
        
    }
    catch (Exception $ex) {
        watchdog_exception('updateRemoveUser', $ex);
        expertusErrorThrow($ex);
    }
}

function checkShareNotificationOverRideInfo1($objectId1, $entityType, $associatedUserId, $notificationCode, $userId)
{
    try {
        $notifyOverrideFlag = null;
        $overrideContent    = '';
        $overrideInfo       = getEntityNotificationOverrideInfo($objectId1, $entityType, $notificationCode, 'Email');
        expDebug::dPrint(' share $overrideInfo = ' . print_r($overrideInfo, true), 4);
        if ($overrideInfo != null) {
            $notifyOverrideFlag = $overrideInfo['notify_flag'];
            $overrideContent    = $overrideInfo['override_content'];
        }
        $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
        $notifyFlag               = null;
        
        // Both 'Y' and 'cre_ntn_sts_atv' are active statuses
        if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
            $notifyFlag = 'Y';
        }
        if ($notifyOverrideFlag == 'cre_ntn_sts_atv') { // Added
            $notifyOverrideFlag = 'Y';
        }
        $frameString = true;
        if ($notifyFlag != 'Y' || ($notifyOverrideFlag != null && $notifyOverrideFlag != 'Y')) {
            if ($notifyFlag != 'Y') {
                // If the notification is disabled status in template, do not generate notification
                expDebug::dPrint('Notification is disabled');
                $frameString = false;
            } else {
                //If the notification status has been overridden and set to disabled, do not generate notification.
                expDebug::dPrint(' Notification is overridded and disabled');
                $frameString = false;
            }
        }
        $getPersonDetails   = getPersonDetails($userId, array(
            'email',
            'full_name'
        ));
        $getReceiverDetails = getPersonDetails($associatedUserId, array(
            'email',
            'full_name'
        ));
        
        
        
        $referemail   = $getPersonDetails['email'];
        $receivermail = $getReceiverDetails['email'];
        

       //changing for #0075027 and #0075024
        $select = db_select('slt_content_master','sc');
        $select->addField('sc', 'type');
        $select->addField('sc', 'created_by');
        $select->addField('sc', 'code');
        $select->condition('sc.id', $objectId1);
        $content_details = $select->execute()->fetchAll();
        
        expdebug::dPrint('$contentFields112222333333' . print_r($content_details, true), 3);
        
        $content_type_code  = $content_details[0]->type;
        $content_created_by = $content_details[0]->created_by;
        $code               = $content_details[0]->code;
        expDebug::dPrint('notificationssss details1111abhishekaaa ---' . $content_created_by);
        expDebug::dPrint('notificationssss details1111abhishekbbb ---' . $code);
        expDebug::dPrint('notificationssss details1111abhishekccc ---' . $content_type_code);
        
        if ($content_type_code == "lrn_cnt_typ_vod")
            $content_type = "Video";
        else if ($content_type_code == "lrn_cnt_typ_knc")
            $content_type = "Knowledge Content";
        
        $receiver_name = getPersonDetails($associatedUserId, array(
            'email',
            'first_name',
            'id'
        ));
        $first_name    = $receiver_name['first_name'];
        $receiver_id   = $receiver_name['id'];
        $sender_name   = $getPersonDetails['full_name'];
        
        expDebug::dPrint('notificationssss details1111 ---' . $receiver_id . "++++" . $first_name . "++++" . $sender_name);
        
        $receivername = $getReceiverDetails['full_name'];
        
        expDebug::dPrint('notificationssss details22222 ---' . $receivername);
        
        $receiver_id1 = getUserIdFromEmail($receivermail);
        
        if ($frameString) {
            $notificationInfo                  = null;
            $notificationInfo['tokens_string'] = 'content_type>|' . $content_type . '~|' . 'content_title>|' . $code . '~|' . 'first_name>|' . $first_name . '~|' . 'SharefromName>|' . $sender_name . '~|';
            'cmp_name>|' . 'Expertus' . '~|' . 'site_url>|' . 'xyz.com';
            
            $notificationInfo['message_id']   = $notificationCode;
            $notificationInfo['message_type'] = 'Share Notification';
            $notificationInfo['send_type']    = 'php mailer';
            $notificationInfo['lang_code']    = 'cre_sys_lng_eng';
            $notificationInfo['sendToEmail']  = $receivermail;
            // $notificationInfo['ccopy']         = $receivermail;
            $notificationInfo['referfrom']    = $referemail;
            $notificationInfo['send_to_id']   = $receiver_id;
            $notificationInfo['send_to_name'] = $receivername;
            //  $notificationInfo['sendival']      = $notifyResult['sendival'];
            //  $notificationInfo['userExistinLMS'] = $notifyResult['userExistinLMS'];
            expDebug::dPrint('Notification Template Info' . print_r($notificationTemplateInfo, true), 4);
            expDebug::dPrint('Notification Info' . print_r($notificationInfo, true), 4);
            
            //Adding for #0075024	  
            if ($receiver_id != $content_created_by)
                callCommonNotificationInsert($notificationCode, $notificationInfo, $notificationTemplateInfo, $userId, $overrideContent);
        }
        
    }
    catch (Exception $ex) {
        watchdog_exception('checkShareNotificationOverRideInfo1', $ex);
        expertusErrorThrow($ex);
    }
}



function exp_sp_administration_viewcontentauthor_addedit_form($js = NULL, $entityId)
{
    try {
        
        global $theme_key;
        $css_theme_optional = array(
            'type' => 'file',
            'group' => CSS_THEME
        );
        if ($theme_key == 'expertusoneV2') {
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration_view_v2.css', $css_theme_optional);
        } else {
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration_view.css', $css_theme_optional);
        }
        expDebug::dPrint('sabbeeee-->' . $entityId);
        $catalog_style = array(
            'ctools-viewscreen-wrapper' => array(
                'modalSize' => array(
                    'width' => 680,
                    'minHeight' => 350,
                    'Height' => auto
                ),
                'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme'
            )
        );
        
        drupal_add_js($catalog_style, 'setting');
        
        if ($js) {
            // Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
            $argsList   = array();
            $argsList[] = $entityId;
            // Prepare list of forms to be displayed in the modal
            $formIdList = array(
                array(
                    'id' => 'exp_sp_administration_view_contentdetails_addedit_form1',
                    'title' => (t('LBL816'))
                ) //LBL816
            );
            expDebug::dPrint('$form $classdetails---->555555' . print_r($formIdList, true), 5);
            view_form_display($formIdList, $argsList);
        } else {
            throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
        }
    }
    catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_viewcourse_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}


function exp_sp_administration_view_contentdetails_addedit_form1($form, &$form_state, $renderMode, $contentId)
{
    try {
        
        //$contentId = 2;
        expDebug::dPrint('$form 1111111t---->' . print_r($form, true), 5);
        expDebug::dPrint('$form_state input---->' . print_r($form_state['input'], true), 5);
        expDebug::dPrint('$form_state values---->' . print_r($form_state['values'], true), 5);
        expDebug::dPrint('$$courseId---->' . $contentId);
        
        $wrapperId                 = 'viewcontent-detail-wrapper';
        //$wrapperId = 'contentauthor-addedit-form';
        $form                      = newAddEditFormWithCancelAndSave($wrapperId);
        $form['view-scroll-start'] = array(
            '#type' => 'markup',
            '#markup' => '<div id="view-scroll-wrapper">'
        );
        $contentdetails            = getContentAuthorDetailsInfoView($contentId);
        
        expDebug::dPrint('$$$contentdetails111-->' . print_r($contentdetails, true), 5);
        $form['narrow_contentview'] = array(
            '#markup' => theme('narrow_contentview', array(
                'record' => $contentdetails
            ))
        );
        
        $form['view-scroll-end'] = array(
            '#type' => 'markup',
            '#markup' => '</div>'
        );
        
        return $form;
    }
    catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_view_coursedetails_addedit_form1', $ex);
        expertusErrorThrow($ex);
    }
}

function updateContentAuthorSubtype($form_state)
{
    
    try {
        expDebug::dPrint('sudh::' . $form_state["values"]["video"], 4);
        
        $i = ($form_state['values']['content_type'] == 'lrn_cnt_typ_aic') ? 1 : 0;
        
        expDebug::dPrint('$form_state[content_storage]Abhishekhhhh = ' . print_r($form_state['content_storage'][$i]->title, true), 4);
        expDebug::dPrint('$form_state[content_storage]Abhishekhhhhhhhh = ' . print_r($form_state['content_storage'][$i]->orig_file_name, true), 4);
        expDebug::dPrint("uploadtype:::" . $_SESSION["uploadtype"]);
        
        if ($_SESSION["uploadtype"] == "embed") {
            $vid_typ = "h5p-Youtube";
           if(strpos($form_state["values"]["video"],"vimeo.com")>0)
             $vid_typ = "h5p-Vimeo";
        } else {
            if ($form_state['content_storage'][$i]->orig_file_name != "")
                $ext = pathinfo($form_state['content_storage'][$i]->orig_file_name, PATHINFO_EXTENSION);
            else {
                if (is_array($form_state["values"]["video"]))
                    $ext = pathinfo("//" . implode("/", $form_state["values"]["video"]), PATHINFO_EXTENSION);
                else
                    $ext = pathinfo("//" . $form_state["values"]["video"], PATHINFO_EXTENSION);
                
            }
            expDebug::dPrint('Trying for file extensions = ' . $ext);
            if ($ext == "mp4" || $ext == "Mp4")
                $vid_typ = "h5p-video-mp4";
            else if ($ext == "webm" || $ext == "Webm")
                $vid_typ = "h5p-video-webm";
        }
        
        return $vid_typ;
        
        
    }
    catch (Exception $ex) {
        watchdog_exception('updateContentAuthorSubtype', $ex);
        expertusErrorThrow($ex);
    }
}


?>
