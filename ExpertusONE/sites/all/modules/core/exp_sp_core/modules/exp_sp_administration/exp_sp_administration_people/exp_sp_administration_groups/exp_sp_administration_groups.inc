<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/**
 * Role Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 1-April-2011
 */
function getRoleTabContentType(){
    try {
      $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is whether contain the narrow search result  or some other result like commerce */
      return $jsObjectInfo;
  } catch (Exception $ex) {
    watchdog_exception('getRoleTabContentType', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * roleNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function roleNarrowSearchFiltersHTML() {
    try {
      $checkedStatus = array();
      $checkedStatus["cre_sec_sts_atv"] = TRUE;
      $allStatus = getProfileItemNamesKeyed('cre_sec_sts_',$langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false,'cre_sec_sts_del');
      expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);
      $all_language_types = getAllStaticLanguageGrp();
      if(!empty($allStatus)){
      $allStatus['cre_sec_sts_atv'] = t('Active');
      $allStatus['cre_sec_sts_itv'] = t('Suspended');
      }

      $role = array('ste_con_mgr'=>'Manager', 'ste_con_ins'=>'Instructor');

      $result1 = adminVisibilityForGlobalAdd('cre_sec','count'); // Admin Group count
      $result2 = adminVisibilityForGlobalAdd('cre_sec_learner','count'); // Learner Group count
      $privelage = privilegeChecking($result1[0]->count,$result2[0]->count);
      expDebug::dPrint('$privelage Filter : '.$privelage, 4);
      if($privelage == 'both'){ // Only this filter is avaible when user have both admin and learner privilege
      $group = array('1'=>t('Administrator'), '0'=>t('Learner'));
          $groupTheme = theme('narrow_search_filterset_checkboxes',
                        array(  'code'            => 'group',
                                'title'           => t('Group'),
                                'divid'           => 'group',
                                'checkboxes_list' => $group)
                        );
      }

        $filterSetforJS=array(
                                            array('type'=>'checkbox','code'=>'grpstatus'),

                                            array('type'=>'checkbox','code'=>'group'),

                              /*array("type" => "addltext","code" => "username",
                              "acpath" => "administration/people/group/username-autocomplete", "defaultText" => t("LBL181")),*/

                                            array("type" => "addltext", "code" => "grporg",
                                    "acpath" => "administration/people/group/org", "defaultText" => t("LBL182")),

                                            array("type" => "addltext", "code" => "grpjobrole",
                                    "acpath" => "administration/people/group/jobrole", "defaultText" => t("LBL764")),

                                            array("type" => "addltext", "code" => "grploc",
                                            "acpath" => "administration/people/group/location", "defaultText" => t("LBL970").' '.t("LBL152")),

                                            array("type" => "addltext", "code" => "grpempl",
                                            "acpath" => "administration/people/group/empltype", "defaultText" => t("LBL036").' '.t("LBL174")),

                                            array("type" => "addltext", "code" => "grpdep",
                                            "acpath" => "administration/people/group/dept", "defaultText" => t("LBL036").' '.t("LBL179")),

                                            array("type" => "addltext", "code" => "grpusrtyp",
                                            "acpath" => "administration/people/group/usertype", "defaultText" => t("LBL970").' '.t("User").t("LBL036")),

                                            array("type" => "addltext", "code" => "grpcontry",
                                            "acpath" => "administration/people/group/country", "defaultText" => t("LBL970").' '.t("LBL039")),

                                            array('type'=>'checkbox','code'=>'grplang'),

                                            array('type'=>'checkbox','code'=>'role'),
                                        );

        return theme('narrow_search_filters_holder',
                array('rendered_filtersets_list' => array(
                        theme('narrow_search_filterset_checkboxes',
                                array('code' => 'grpstatus',
                                        'title' => t('LBL102'),
                                        'divid' => 'status',
                                        'checkboxes_list' => $allStatus)
                        ),

                        $groupTheme,

                        theme('narrow_search_filterset_checkboxes',
                                array('code' => 'role',
                                        'title' => t('LBL579'),
                                        'divid' => 'roles',
                                        'checkboxes_list' => $role)
                        ),
                        theme('narrow_search_filterset_checkboxes',
                                array('code' => 'grplang',
                                        'title' => t('LBL038'),
                                        'divid' => 'language',
                                        'checkboxes_list' => $all_language_types)
                        ),
                        /*theme('narrow_search_filterset_addltext',
                         array('code' => 'username',
                                'title' => t('LBL054'),
                                'default_text' => t('LBL181'))
                        ),*/

                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grpcontry',
                                        'title' => t('LBL039'),
                                        'default_text' => t("LBL970").' '.t("LBL039"))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grploc',
                                        'title' => t('LBL152'),
                                        'default_text' => t("LBL970").' '.t("LBL152"))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grpdep',
                                        'title' => t('LBL179'),
                                        'default_text' => t("LBL036").' '.t("LBL179"))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grpempl',
                                        'title' => t('LBL174'),
                                        'default_text' => t("LBL036").' '.t("LBL174"))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grpjobrole',
                                        'title' => t('LBL133'),
                                        'default_text' => t('LBL764'))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grporg',
                                        'title' => t('Organization'),
                                        'default_text' => t('LBL182'))
                        ),
                        theme('narrow_search_filterset_addltext',
                                array('code' => 'grpusrtyp',
                                        'title' => t('LBL173'),
                                        'default_text' => t("LBL970").' '.t("User").t("LBL036"))
                        ),
                ),
                        'filter_set_list' => drupal_json_encode($filterSetforJS),
                ));
  } catch (Exception $ex) {
    watchdog_exception('roleNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * roleSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function roleSearchResultsSortbarHTML() {
 try {
      $sortbarLinksList = array();
      $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('AZ'), 'sort_type' => 'AZ', 'html_id' => 'role-sort-az');
      $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('ZA'), 'sort_type' => 'ZA', 'html_id' => 'role-sort-za');
      //$sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('Number'), 'sort_type' => 'Number', 'html_id' => 'role-sort-num');
      $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('NewlyListed'), 'sort_type' => 'NewlyListed', 'html_id' => 'role-sort-new');

      $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
                                   'last_sort_type_html_id'=>'role-sort-new'
                                   );
        return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                                  'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                                  'title' => t('LBL011'),
                                   )
                                   );
    }catch(Exception $ex) {
        watchdog_exception('roleSearchResultsSortbarHTML', $ex);
      expertusErrorThrow($ex);
    }
}

/*
 * roleSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function roleSearchResultsActionbarHTML($argType) {
    try {
        
      // Checking create action
      $result1 = adminVisibilityForGlobalAdd('cre_sec','count'); // Admin Group count
      $result2 = adminVisibilityForGlobalAdd('cre_sec_learner','count'); // Learner Group count
      $privilege = privilegeChecking($result1[0]->count,$result2[0]->count);    
      
      if($privilege == 'admin')
        $create_button = t('LBL817').' '.t('Administrator').' '.t('Group');
      else if($privilege == 'learner')
        $create_button = t('LBL817').' '.t('Learner').' '.t('Group');
      else
        $create_button = t('LBL1036');
      
      $actionbarLinksListforJS=array('export_path'=>'administration/people/group/export/',
                                     'print_path'=>'administration/people/group/print/'
                                     );
                                     return theme('narrow_search_actionbar', array( 'add_button_title' => $create_button,
                                                     'add_page_path' => base_path() . 'administration/people/group/type/nojs/addedit',
                                                     'show_print_button'=>true,
                                                     'show_export_button'=>true,
                                                     'show_add_button'=>true,
                             'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                                                 'ctools_style'=>'ctools-modal-ctools-admin-role-addedit-style',
                                     'grey_out_add_button' => $argType,
                                     'page_code' => 'cre_sec',
                                     )
                                     );
     }catch(Exception $ex) {
      watchdog_exception('roleSearchResultsActionbarHTML', $ex);
      expertusErrorThrow($ex);
   }
}

/*
 * getRoleTabContentMainHTML - Returns the html for tab content main
 */
function getRoleTabContentMainHTML() {
    try {
      $tabContentTypeArr =  getRoleTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];
      if($tabContentType=="narrow_search"){
        $narrowsearchTabContentListforJS=array(
                                                'search_base_path'=>'administration/people/group/search/all/',
                                                'show_top_text_filter' => true,
                                                'text_filter_ac_path' => 'administration/people/group/textfilter-autocomplete'
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
    watchdog_exception('getRoleTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getRoleFTabContent - Returns the html for narrow search block on role search page
 */
function getRoleTabContent($argType) {
    try {
      $tabContentTypeArr =  getRoleTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];

      $roleDetailsResponse=array(
        'rendered_tab_content_type' => $tabContentType,
        'rendered_narrow_search_filters' => roleNarrowSearchFiltersHTML(),
        'rendered_narrow_search_sortbar' => roleSearchResultsSortbarHTML(),
        'rendered_narrow_search_actionbar' => roleSearchResultsActionbarHTML($argType),
        'rendered_tab_content_main'=>getRoleTabContentMainHTML(),
        'rendered_main_div' => getAdminModules('administration','people'),
        'rendered_script' => "countryBind",
      );
      echo drupal_json_encode($roleDetailsResponse);
  } catch (Exception $ex) {
    watchdog_exception('getRoleTabContent', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getRoleDetailsHTML() - Renders an role's details as HTML to be displayed in jqGrid.
 */
function getRoleDetailsHTML($role) {
    try {

        expDebug::dPrint('getRoleDetailsHTML'.print_r($role,true),4);
      $detailsList = array();
      $actionList = array();
      $newActionList = array();
      //$UserList=fetchUserListToGroup($role->id,'');
      $Userquery = fetchUserListToGroup($role->id,'','count');
      $Usercount = $Userquery->execute()->fetchField();
      //$Usercount=count($UserList->execute()->fetchAll());
      //$entityType          = 'cre_sec';
      /*if (!empty($role->number)) {
        $additionalTitleInfo = $role->number;
      }
      if (!empty($role->type)) {
        $detailsList[] = array('detail' => t($role->type), 'tooltip' => t('LBL244').': '.$role->type);
      }
      /*  if (!empty($role->status)) {
       $detailsList[] = array('detail' => $role->status, 'tooltip' => t('LBL102'));
       }
       */
      $type = ($role->is_admin == 1)? 'admin':'learner';
      //$userCount=getUserDetailList($role->id,0,0,'','',1);
      //$detailsList[] =  array('detail' => t('LBL160').':'.' '.$userCount, 'tooltip' => t('LBL160'));
      $group_tooltip = t(ucfirst($type))." ".t(ucfirst('group'));
      $detailsList[] =  array('detail' => t(ucfirst($type).' group'), 'tooltip' => $group_tooltip);
      $actionList[] = array('action_link_text' => t('LBL063'),
                          'action_page_path' => 'administration/people/group/'.$type.'/nojs/addedit',
                          'action_button_params' => $role->id,
                          'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
                          'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
      ),
                          'tooltip' => t('LBL063')
      );

      $actionList[] = array('action_link_text' => t('LBL286'),
          'action_page_path' => '',
          'action_button_params' => $role->id,
          'action_button_param_text' => 'Grp',
          'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
          'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
      ),
          'tooltip' => t('LBL286')
      );

     //44133: German-old-ui-Icon not changing when user is activate and suspend
     if(strtolower($role->status_code) == 'cre_sec_sts_atv'){
        $newActionList[] = array('action_link_text' => t('LBL572'),
                            'action_page_path' => '',
                            'action_button_params' => $role->id,
                            'action_button_params_text' => 'deactivate',
                            'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
                            'is_admin' => $role->is_admin,
                            // 'owner_cnt' => ownerExistOrNot($role->id),
                            'js_object_info' => array (
                                                'name' => 'narrowsearch', // provide the java script object name
                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
        ),
                            'tooltip' => t('LBL572'),
                            'icon_style_class'=>'active-deactive-tab-icon Suspend'
                            );
      } else {
        $newActionList[] = array('action_link_text' => t('LBL573'),
                            'action_page_path' => '',
                            'action_button_params' => $role->id,
                            'action_button_params_text' => 'activate',
                            'ctools_style' => 'ctools-modal-ctools-admin-role-addedit-style',
                            'is_admin' => $role->is_admin,
                            // 'owner_cnt' => ownerExistOrNot($role->id),
                            'js_object_info' => array (
                                                'name' => 'narrowsearch', // provide the java script object name
                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
        ),
                            'tooltip' => t('LBL573'),
                            'icon_style_class'=>'active-deactive-tab-icon Activate'
                            );
      }
      $viewgrplist[] = array('action_link_text' => 'Groups',
            'action_page_path' => 'administration/view-group/nojs/addedit/'.$role->id.'',
            'action_button_params' =>  $role->id,
            'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
            'js_object_info' => array (
                    'name' => 'narrowsearch', // provide the java script object name
                    'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
            ),
      );
      
$group_list = array( 'title' => trim(htmlentities($role->name, ENT_QUOTES, 'UTF-8')),   //change by ayyappans for 40360: Issue in showing titles on hover
                            'delete_info' => t('MSG646').' '.$Usercount.' '.t('MSG647').' '.t('MSG357').' '.strtolower(t('Group')),
                            'details_list'=>$detailsList,
                                'action_list'=> $actionList,
                            'new_action_list'=>$newActionList,
                            'object_type'=>'Grp',
                            'description'=>$role->description,
                                                'entity_multi_action' => $role,
                                                'sumedit' => $role->sumedit,
                                                'sumdelete' => $role->sumdelete,
                                                'grp_code' => $role->grp_code,
                                                //'activeuserCnt' => $UserList,
                                                'view_list' =>$viewgrplist
      );
expDebug::dPrint('$group_list '.print_r($group_list, 1));
      return theme('narrow-search-results-item-details',
              $group_list
      );

  } catch (Exception $ex) {
    watchdog_exception('getRoleDetailsHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * roleSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function roleSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias, $sortby = '') {
    try {
        
        if($sortby == ''){
            $sortby = getURLParam('sortby');
        }
        expDebug::dPrint('testes ' .$sortby);
  switch ($sortby) {
    case "AZ" :
      $field = $tableAlias . ".name";
      $direction = "ASC";
      break;

    case "ZA" :
      $field = $tableAlias . ".name";
      $direction = "DESC";
      break;

    case "Number" :
      $field = $tableAlias . ".number";
      $direction = "ASC";
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
    watchdog_exception('roleSearchSelectStmtAddOrderByClause', $ex);
    expertusErrorThrow($ex);
  }
}

function sendRolesListToJqGrid() {
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

      /* $numRole = fetchRolesAll('COUNT');
      expDebug::dPrint(' $numRole = ' . print_r($numRole, true) , 4);

      $total_pages = 0;
      $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numRole); */
      $start = calculate_offset($page, $limit);
      expDebug::dPrint(' $start = ' . print_r($start, true) , 4);


      $roleList = fetchRolesAll('LISTS', $start, $limit);
      expDebug::dPrint(' $roleList = ' . print_r($roleList, true) , 4);

      // Prepare and send the response
      $response = new stdClass();
      $response->page = $page;
      $response->total = $total_pages;
      $response->records = $numRole;

      // $jsObjectInfo = roleJSObjectInfo();
      //$response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
      // have to do this.
      $response->initial_sort_type_html_id = 'role-sort-new';

      //$response->filters_html = roleNarrowSearchFiltersHTML($jsObjectInfo);

      $i=0;
      foreach ($roleList as $role) {
        expDebug::dPrint(' $role = '. print_r($role, true) , 4);
        $role->row =  $i;
        $response->rows[$i]['id']   = $role->id;
        $role->entityId = $role->id;
            $role->entityType = 'cre_sec';
        $response->rows[$i]['cell'] = array('details' => getRoleDetailsHTML($role),
        );

        $i++;
      }

      expDebug::dPrint(' $response record = '. print_r($response, true) , 4);

      echo drupal_json_encode($response);
  } catch (Exception $ex) {
    watchdog_exception('sendRolesListToJqGrid', $ex);
    expertusErrorThrow($ex);
  }
}

function fetchRolesAll($op=NULL, $start=NULL, $limit=NULL, $callFrom = '', $sort_by = '') {
     try {
      // Select slt_security_role table
        expDebug::dPrint("Get values : ".print_r($_GET,true),4);

        $select = db_select('slt_groups', 'grp');
        $select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
              if($op=='COUNT'){
        // Get count of active users
        $select->addExpression('COUNT(DISTINCT grp.id)', 'count');
      }
      else{
        // Select fields/expression values to be fetched from the database.
        $select->distinct();
        $select->addField('grp', 'id', 'id');
        $select->addField('grp', 'name', 'name');
        $select->addField('grp', 'code', 'grp_code');
        $select->addField('grp', 'status', 'status_code');
        $select->addField('grp', 'is_admin', 'is_admin');
        $select->addField('grp', 'is_manager', 'is_manager');
        $select->addField('grp', 'is_instructor', 'is_instructor');
    if($sort_by != ''){
        roleSearchSelectStmtAddOrderByClause($select, 'grp', $sort_by);
        }
        expDebug::dPrintDBAPI(' $select =',$select);
      }
      selectAddNarrowSearchByTextMultipleORCondition($select, "grp", array("name"), "textfilter");
      if((isset($_GET['group'])) && (!empty($_GET['group']) || $_GET['group'] != '')){
        selectAddNarrowSearchINCondition($select, 'grp', 'is_admin', 'group');
      }

      if(isset($_GET['role']) && !empty($_GET['role'])){
        $valuefilters = $_GET['role'];
        if (!empty($valuefilters)){
            $valueRange = explode("|", $valuefilters);
            if(!empty($valueRange[0]) && !empty($valueRange[1])){
                $fieldname1   = ($valueRange[0] == 'ste_con_mgr') ? 'is_manager' : 'is_instructor';
                $fieldval1    = ($valueRange[0] == 'ste_con_mgr') ? 'Y' : 'Y';
                $pdfrolename1 = ($valueRange[0] == 'ste_con_mgr') ? 'Manager' : 'Instructor';
                $fieldname2   = ($valueRange[1] == 'ste_con_mgr') ? 'is_manager' : 'is_instructor';
                $fieldval2    = ($valueRange[1] == 'ste_con_mgr') ? 'Y' : 'Y';
                $pdfrolename2 = ($valueRange[0] == 'ste_con_mgr') ? 'Manager' : 'Instructor';
                $select->condition(db_or()
                        ->condition('grp'.'.'.$fieldname1, $fieldval1, '=')
                        ->condition('grp'.'.'.$fieldname2, $fieldval2, '='));
                $roleNames = implode(',',array($pdfrolename1,$pdfrolename2));
            }
            elseif(!empty($valueRange[0])) {
                $fieldname    = ($valueRange[0] == 'ste_con_mgr') ? 'is_manager' : 'is_instructor';
                $fieldval     = ($valueRange[0] == 'ste_con_mgr') ? 'Y' : 'Y';
                $pdfrolename  = ($valueRange[0] == 'ste_con_mgr') ? 'Manager' : 'Instructor';
                $select->condition('grp'.'.'.$fieldname, $fieldval, '=');
                $roleNames    = $pdfrolename;
            }
        }
      }

      if(isset($_GET['grpstatus']) && !empty($_GET['grpstatus'])){
        $select->leftJoin('slt_profile_list_items', 'plistatus', 'grp.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
        selectAddNarrowSearchINCondition($select, 'grp', 'status', 'grpstatus');
      }
      if(isset($_GET['grporg']) && !empty($_GET['grporg'])){
        $tableAliasFields = array('grp.org_id','org.name');
        $select->leftJoin('slt_organization', 'org', 'FIND_IN_SET(org.id,grp.org_id)>0');
        selectAddNarrowSearchByTextMultiValue($select,$tableAliasFields, "grporg");
      }
      if(isset($_GET['grpjobrole']) && !empty($_GET['grpjobrole'])){
        $tableAliasFields = array('grp.job_role','listitem.name');
        $select->leftJoin('slt_profile_list_items', 'listitem', 'FIND_IN_SET(listitem.code,grp.job_role)>0 AND listitem.lang_code = \'cre_sys_lng_eng\'');
        selectAddNarrowSearchByTextMultiValue($select, $tableAliasFields, "grpjobrole");
      }
      if(isset($_GET['grpusrtyp']) && !empty($_GET['grpusrtyp'])){
        $tableAliasFields = array('grp.user_type','item.name');
        $select->leftJoin('slt_profile_list_items', 'item', 'FIND_IN_SET(item.code,grp.user_type)>0 AND item.lang_code = \'cre_sys_lng_eng\'');
            selectAddNarrowSearchByTextMultiValue($select,$tableAliasFields, "grpusrtyp");
      }
      if(isset($_GET['grpempl']) && !empty($_GET['grpempl'])){
        $tableAliasFields = array('grp.employment_type','emptype.name');
        $select->leftJoin('slt_profile_list_items', 'emptype', 'FIND_IN_SET(emptype.code,grp.employment_type)>0 AND emptype.lang_code = \'cre_sys_lng_eng\'');
        selectAddNarrowSearchByTextMultiValue($select, $tableAliasFields, "grpempl");
      }
      if(isset($_GET['grpdep']) && !empty($_GET['grpdep'])){
        $tableAliasFields = array('grp.department','dept.name');
        $select->leftJoin('slt_profile_list_items', 'dept', 'FIND_IN_SET(dept.code,grp.department)>0 AND dept.lang_code = \'cre_sys_lng_eng\'');
        selectAddNarrowSearchByTextMultiValue($select, $tableAliasFields, "grpdep");
      }
      if(isset($_GET['grplang']) && !empty($_GET['grplang'])){
        $select->leftJoin('slt_profile_list_items', 'lang', 'FIND_IN_SET(lang.code,grp.language)>0 AND lang.lang_code = \'cre_sys_lng_eng\'');
        selectAddNarrowSearchINCondition($select, 'grp', 'language', 'grplang');
      }
      if(isset($_GET['grploc']) && !empty($_GET['grploc'])){
        $tableAliasFields = array('grp.state','st.state_name');
        if(($_GET['check_all_state']) && ($op != "PDF")) {
          $select->leftJoin('slt_state', 'st', 'FIND_IN_SET(concat(st.country_code,\'-\',st.state_code),grp.state)>0 or grp.state = \'All\'');
        }
        else {
            $select->leftJoin('slt_state', 'st', 'FIND_IN_SET(concat(st.country_code,\'-\',st.state_code),grp.state)>0');
        }
        selectAddNarrowSearchByTextMultiValue($select, $tableAliasFields, "grploc");
      }

      if(isset($_GET['grpcontry']) && !empty($_GET['grpcontry'])){
        $tableAliasFields = array('grp.country','con.country_name');
        $select->leftJoin('slt_country', 'con', 'FIND_IN_SET(con.country_code,grp.country)>0');
        selectAddNarrowSearchByTextMultiValue($select, $tableAliasFields, "grpcontry");
      }

     $groupStatusAPI = trim(getURLParam('grpstatus'));
     if ($callFrom == 'API' && $groupStatusAPI) { // #46953 - retrive group list based on the api status
        $select->condition('grp.status', array($groupStatusAPI), 'IN');
     } else {
        $select->condition('grp.status', array('cre_sec_sts_del'), 'NOT IN');
     }
     $result1 = adminVisibilityForGlobalAdd('cre_sec','count'); // Admin Group count

     $result2 = adminVisibilityForGlobalAdd('cre_sec_learner','count'); // Learner Group count
     $privelage = privilegeChecking($result1[0]->count,$result2[0]->count);
     expDebug::dPrint('$privelage : '.$privelage, 4);
     if($privelage == 'admin' || ($callFrom == 'API' && (trim(getURLParam('is_admin')) == '1'))){
        $select->condition('grp.is_admin', 1);// only Admin group as to be placed in list
        if($callFrom != 'API' )
        filterByUserAccessPrivileges($select, 'grp','',1,'cre_sec','Group','grp','','admin');
     }else if($privelage == 'learner'|| ($callFrom == 'API' && (trim(getURLParam('is_admin')) == '0'))){
        $select->condition('grp.is_admin', 0);// only Learner group as to be placed in list
        if($callFrom != 'API' )
        filterByUserAccessPrivileges($select, 'grp','',1,'cre_sec','Group','grp','','learner');
     }else if($callFrom == 'API') {
        $userid = trim(getURLParam('userid'));
        expDebug::dPrint('group list api -userid : '.$userid, 4);
        if($userid != 1 && (trim(getURLParam('is_admin')) == '1'))
            filterByUserAccessPrivileges($select, 'grp','',$userid,'cre_sec','Group','grp','','admin');
        else if($userid != 1 && (trim(getURLParam('is_admin')) == '0'))
            filterByUserAccessPrivileges($select, 'grp','',$userid,'cre_sec','Group','grp','','learner');
        else
            if($userid != 1 && (trim(getURLParam('is_admin')) == ''))
            filterByUserAccessPrivileges($select, 'grp','',$userid,'cre_sec','Group','grp','','');
     }else{
        if($callFrom != 'API' )
        filterByUserAccessPrivileges($select, 'grp','',1,'cre_sec','Group','grp','','Both'); // Both Groups
     }
     if($op != "COUNT"){
        // Order the records
        roleSearchSelectStmtAddOrderByClause($select, 'grp');
        $select->groupBy('grp.id');
      }

      if($op == "COUNT"){
        expDebug::dPrintDBAPI('$select',$select);
        $result = $select->execute()->fetchField();
        expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
        return $result;
      }
      elseif ($op == "CSV") {

        // Select slt_security_role table
        $getValues = $_GET;
        $response = listOfAvailableGroupDetailCSV($select);
        expDebug::dPrint(' $colHeadersMap $response = ' . print_r( $response, true) , 4);
        $colHeadersMap = array(
                t('LBL107')          => 'name',
                t('LBL102')          => 'grpstatus',
                t('LBL179')          => 'grpdep',
                t('LBL174')          => 'grpempl',
                t('LBL175')          =>  'hire_date',
                t('LBL344')          => 'grpjobrole',
                t('LBL038')          => 'grplang',
                t('Organization')    => 'grporg',
                t('Role')            => 'role',
                t('LBL173')          => 'grpusrtyp',
                t('LBL039')          => 'grpcontry',
                t('LBL152')          => 'grploc',
                
                //t('Users')                     => 'userslist'
        );
        // Applied the filter option  for CSV


        $filterArray[] = array(t('LBL3046').':'.textboxFiltersetValueToPrintableStr('textfilter'));
        $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('grpstatus'));
        $filterArray[] = array(t('LBL179').':'.textboxFiltersetValueToPrintableStr('grpdep'));
        $filterArray[] = array(t('LBL174').':'.textboxFiltersetValueToPrintableStr('grpempl'));
        $filterArray[] = array(t('LBL344').':'.textboxFiltersetValueToPrintableStr('grpjobrole'));
        $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('grplang'));
        $filterArray[] = array(t('Organization').':'.textboxFiltersetValueToPrintableStr('grporg'));
        $filterArray[] = array(t('Role').':'.$roleNames);
        $filterArray[] = array(t('LBL173').':'.textboxFiltersetValueToPrintableStr('grpusrtyp'));
        $filterArray[] = array(t('LBL039').':'.textboxFiltersetValueToPrintableStr('grpcontry'));
        $filterArray[] = array(t('LBL152').':'.textboxFiltersetValueToPrintableStr('grploc'));
        $filterArray = array_filter($filterArray);


        $empty_array = array();
        if(!empty($response))
            SendNarrowSearchResultsAsCSVFile($response, $colHeadersMap, 'export', $filterArray, '', 'role_admin_search');
        else
            SendNarrowSearchResultsAsCSVFile($empty_array, $colHeadersMap, 'export', $filterArray, '', 'role_admin_search');
      }
      elseif($op == "PDF"){
        // % width of the columns must total to 100%
        /*      $colHeadersMap = array(t('LBL107') => array('name',44),
                                        );
               $appliedFilters = array(t('Group') => textboxFiltersetValueToPrintableStr('textfilter'),
                                       t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('grpstatus'),
                               ucfirst(t('LBL011')) => getNarrowSearchSortTitle()
                              );
               SendNarrowSearchResultsAsPDFFile($select, t('Group'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'role_admin_search');*/
        $getValues = $_GET;
        $response = listOfAvailableGroupDetailCSV($select);
        
        expDebug::dPrint(' $colHeadersMap $response = ' . print_r( $response, true) , 4);
        $appliedFilters = array(
                t('LBL3046')          => textboxFiltersetValueToPrintableStr('textfilter'),
                t('LBL102')          => checkboxFiltersetSelectionsToPrintableStr('grpstatus'),
                t('LBL179')          => textboxFiltersetValueToPrintableStr('grpdep'),
                t('LBL174')          => textboxFiltersetValueToPrintableStr('grpempl'),
                t('LBL344')          => textboxFiltersetValueToPrintableStr('grpjobrole'),
                t('LBL038')          => checkboxFiltersetSelectionsToPrintableStr('grplang'),
                t('Organization')    => textboxFiltersetValueToPrintableStr('grporg'),
                t('Role')            => $roleNames,
                t('LBL173')          => textboxFiltersetValueToPrintableStr('grpusrtyp'),
                t('LBL039')          => textboxFiltersetValueToPrintableStr('grpcontry'),
                t('LBL152')          => textboxFiltersetValueToPrintableStr('grploc'),
                //t('Users')                     => 'userslist'
        );
        $appliedFilters = array_filter($appliedFilters);


       $colHeadersMap = array(
            t('LBL107')          => array('name', 44),
            t('LBL102')          => array('grpstatus', 25),
            t('LBL179')          => array('grpdep', 25),
            t('LBL174')          => array('grpempl', 20),
            t('LBL175')          => array('hire_date',25),
            t('LBL344')          => array('grpjobrole', 20),
            t('LBL038')          => array('grplang', 20),
            t('Organization')    => array('grporg', 20),
            t('Role')            => array('role', 20),
            t('LBL173')          => array('grpusrtyp', 20),
            t('LBL039')          => array('grpcontry', 20),
            t('LBL152')          => array('grploc', 20),
            
            //t('Users')           => array('userslist', 20),
        );


        $empty_array = array();
        if(!empty($response))
          SendNarrowSearchResultsAsPDFFile($response, t('Group'), $appliedFilters, $colHeadersMap, 'export', 11, 'L', 'role_admin_search');
        else
          SendNarrowSearchResultsAsPDFFile($empty_array, t('Group'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'role_admin_search');
      }
      else{
         add_row_id_condition($select, 'grp');
         // Limit to $limit records
         if (is_numeric($limit) && is_numeric($start)) {
            $select->range($start, $limit);
         }
         expDebug::dPrintDBAPI(' $select obj = ' , $select);
        // Execure the query and fetch all the records
        $result = $select->execute()->fetchAll();
        expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3);
        return $result;
      }
  } catch (Exception $ex) {
    watchdog_exception('fetchRolesAll', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function role_admin_search_csv_row_data_alter($rowData) {
  try {
    $rowData[0]->user = getRoleUsereName($rowData[0]->user);
    $rowData[0]->description = strip_tags($rowData[0]->description);
  }
  catch (Exception $ex) {
    watchdog_exception('role_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function role_admin_search_pdf_row_data_alter($rowData) {
  try {
    $rowData[0]->user = getRoleUsereName($rowData[0]->user);
    $rowData[0]->description = strip_tags($rowData[0]->description);
  }
  catch (Exception $ex) {
    watchdog_exception('role_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

function getRoleTextFilterAutoComplete() {
    try {
      $searchText =  $_GET['z'];

      // Select the table
      $select = db_select('slt_groups', 'grp');

      // Add fields to fetch
      $select->addField('grp', 'name', 'name');

      // Add condition
      $select->condition('grp.name', '%' . db_like($searchText) . '%', 'LIKE');
      $select->condition('grp.status',array('cre_sec_sts_itv','cre_sec_sts_atv'),'IN');

      // Order in dictionary sort order
      $select->orderBy('name');

      expDebug::dPrintDBAPI(' $select obj = vince' , $select);

      $result1 = adminVisibilityForGlobalAdd('cre_sec','count'); // Admin Group count
      $result2 = adminVisibilityForGlobalAdd('cre_sec_learner','count'); // Learner Group count
      $privelage = privilegeChecking($result1[0]->count,$result2[0]->count);
      expDebug::dPrint('$privelage : '.$privelage, 4);
      if($privelage == 'admin'){
        $select->condition('grp.is_admin', 1);// only Admin group as to be placed in list
      }else if($privelage == 'learner'){
        $select->condition('grp.is_admin', 0);// only Learner group as to be placed in list
      }
      filterByUserAccessPrivileges($select,'grp','',1,'cre_sec','autocomplete','grp');
      $select->groupBy('grp.name');
      $matchingRoleNames = $select->execute()->fetchAll();
      expDebug::dPrint(' $matchingRoleNames = '. print_r($matchingRoleNames, true) , 3);

      foreach ($matchingRoleNames as $roleName) {
        print rawurlencode($roleName->name) . "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getRoleTextFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * New Group creation Insert
 * */
function addNewGroup($form, &$form_state, $paramuserid="", $callFromAPI = 0,$grpType)
{
   expDebug::dPrint(' $form ' . print_r($form, true) , 5);
   expDebug::dPrint(' $form_state ' . print_r($form_state, true) , 5);
   $duplicatechk = $form_state['values']['grp_war'];
   $duplicateId = (!empty($form_state['values']['grp_war_grpId'])) ? $form_state['values']['grp_war_grpId'] : null;
   $userid = $paramuserid;
     try {
     $sortedattribute = sortgrpAttributes($form_state);
     $insertStmt = db_insert('slt_groups');
     /* if($callFromAPI == '1') {
       $status = "cre_sec_sts_itv";
     } else { */
       $status = "cre_sec_sts_atv";
    // }
     $fields = array(
                'name'              => trim($form_state['values']['name']),
                'status'            => $status,
                'duplicate'         => $duplicateId,
                'is_admin'          => ($grpType == 'admin')? 1 : 0,
                'org_id'            => $sortedattribute['org_id'],
                'user_type'         => $sortedattribute['user_type'],
                'employment_type'   => $sortedattribute['employment_type'],
                'country'           => $sortedattribute['country'],
                'state'             => $sortedattribute['state'],
                'department'        => $sortedattribute['department'],
                'job_role'          => $sortedattribute['job_role'],
                'language'          => $sortedattribute['language'],
                'is_manager'        => $sortedattribute['is_manager'],
                'is_instructor'     => $sortedattribute['is_instructor'],
                'created_by'        => $userid,
                'created_on'        => now(),
                      'updated_by'        => $userid,
                      'updated_on'        => now()
              );

    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);

    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

    // Execute the insert statement. Fetch the new roleId.
    try{
        $txn = db_transaction();
        $grpId = $insertStmt->execute();
        expDebug::dPrint('$grpId = ' . $grpId,4);
        if($callFromAPI) {
            $classCreateUser = $paramuserid;
        }
        else {
            $classCreateUser = getIdOfLoggedInUser();
        }
        if(!empty($form_state['values']['atr_ste_con_hdt'])){
             $hireModes = array_unique(explode(',',$form_state['values']['atr_ste_con_hdt']));
             expDebug::dPrint("mode >> ". print_r($hireModes,true),5);
            if(in_array("ste_con_hdt_ltn",$hireModes)){
                $ltn_start = formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_ltn']));
                }
            if(in_array("ste_con_hdt_gtn",$hireModes)){
                $gtn_start =  formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_gtn']));
            }
            if(in_array("ste_con_hdt_btw",$hireModes)){
                $btw_start = formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_btw']));
                $btw_End = formatHireDate(trim($form_state['values']['hire_end']));
            }
            expDebug::dPrint("between values ". $btw_start . "<< end".$btw_End );
                 /* Hire date Update in group attribute table*/
                 $insertAttr = db_insert('slt_group_attributes');
                 $fields = array(
                        'group_id'          => $grpId,
                        'attr_type'         => 'HireDate',
                'on_or_after_start_date'         => (!empty($gtn_start) ? $gtn_start : null),
                'on_or_before_start_date'       => (!empty($ltn_start) ? $ltn_start : null),
                'between_start_date'            => (!empty($btw_start) ? $btw_start : null),
                'between_end_date'  => (!empty($btw_End) ? $btw_End : null),
                        'created_by'        => $userid,
                        'created_on'        => now(),
                        'updated_by'        => $userid,
                        'updated_on'        => now()
                      );
        
                // Select the fields and the values to be inserted
                $insertAttr->fields($fields);
            expDebug::dPrintDBAPI(' $insertStmt group_attributes obj = ' , $insertAttr);
                $insertAttr->execute();
             }
        updateGroupMappingDetails($grpId, 'cre_sec', $classCreateUser,'emptyadd',0);
        // Save the new organization's id in the hidden id form field.
        $form_state['values']['id'] =  $grpId;

        $code ='grp_'.$grpId;
        $updateStmt = db_update('slt_groups');
        $updateStmt->condition('id', $grpId);
        $updateStmt->fields(array('code'    => $code));
        $Updatedcode = $updateStmt->execute();
        /* 
        if($grpType == 'admin'){
            $insertStmt = db_insert('slt_owner_details');
            $fields = array(
                    'group_id'          => $grpId,
                    'owner_id'          => $userid,
                    'created_by'        => $userid,
                    'created_on'        => now(),
                    'updated_by'        => $userid,
                    'updated_on'        => now()
            );
            $insertStmt->fields($fields);
            expDebug::dPrintDBAPI("Insert owner details  ---->",$insertStmt);
            $insertStmt->execute();
            
            $loggedUserId = getSltpersonUserId();
            //Insert group_audit to track added owners
            $insertStmt = db_insert('slt_group_audit');
            $fields = array(
                    'group_id'        => $grpId,
                    'user_id'         => $userid,
                    'operation'       => 'owneradded',
                    'criteria_match'  => NULL,
                    'updated_by'      => $loggeduserId,
                    'updated_on'      => now()
            );
            $insertStmt->fields($fields);
            $insertStmt->execute();
        }
        */
      
        unset($txn);
    }catch(Exception $e){
        $txn->rollback();
        unset($txn);
        throw new Exception($e);
    }

    expDebug::dPrint(' $Updatedcode ' . print_r($Updatedcode, true) , 3);
    if($grpType == 'admin'){
        try{
            //Entry for securitymanagementAdmin
            require_once "./modules/user/user.admin.inc";
                $rname  = trim($form_state['values']['name']);
                $min_form_state = array( "values" =>  array( "op" => t('Add role'), "name" => "$rname" ) );
              user_admin_role_validate(null, $min_form_state);
              $form_errors = form_get_errors();
              if (empty( $form_errors )||$form_errors=='') {
                  user_admin_role_submit(null, $min_form_state);
              }
        }catch(Exception $e){
            throw new Exception($e);
        }
        // Remove tmp space
        unset($form_state['storage']['acvalues']);
        //grpUserInsert($grpId);
        
        //  add user in drupal table
        $role_id = getDrupalRoleId($grpId);
        $user_Id = getDrupalIdOfUser($userid);
        
         
        $num_of_results = db_select('users_roles', 'roles');
        $num_of_results->condition('roles.uid',$user_Id ,'=');
        $num_of_results->condition('roles.rid',$role_id ,'=');
        $num_of_results->addField('roles', 'rid','rid');
        $result_count =  $num_of_results->execute()->rowCount();
        
        expDebug::dPrintDBAPI('$num_of_results ' , $num_of_results);
        
        /* if($result_count == '0') {
            $select1 = db_insert('users_roles');
            $select1->fields(array('uid' => $user_Id, 'rid' => $role_id));
            expDebug::dPrintDBAPI('Insert userlist ' , $select1);
            $select1->execute();
    } */
        
    }
    //Audit Entry
    $formFieldsList = array('name', 'description','status','created_by','created_on','department','employment_type','job_role','language','org_id','is_manager','is_instructor','user_type','country','state');

    make_audit_trail_entries($fields, getIdOfLoggedInUser(), null, $grpId , 'Inserted', 'cre_sec', 'exp_sp_administration_groups', 'addNewGroup',
                                          $formFieldsList);

    return array((object)array('id'=>$grpId));
  }
  catch (Exception $ex) {
    watchdog_exception('addNewGroup', $ex);
    //return array((object)array('id'=>"failure"));
    $errobj=new stdClass();
    $errobj->isValidateError = 1;
    $errobj->errcode = 'L_012';
    $errobj->errormsg = "Error in Group Id.";
    return $errobj;
  }
}
/*
 * updateRoleDetails - Updates an existing role details when the user clicks save
 */
function updateRoleDetails($form, &$form_state, $paramuserid="", $callFromAPI = 0,$grpType) {
  expDebug::dPrint(' $form ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state ' . print_r($form_state, true) , 5);
  $userid = $paramuserid;
  try {
    $grpId = $form_state['values']['id'];
    $duplicatechk = $form_state['values']['grp_war'];
    $duplicateId = (!empty($form_state['values']['grp_war_grpId'])) ? $form_state['values']['grp_war_grpId'] : null;
    $sortedattribute = sortgrpAttributes($form_state);
    expDebug::dPrint('hire search mode: ' . $hire_search , 5);
    expDebug::dPrint('hire start mode: ' . $hire_start,5);
    expDebug::dPrint('hire end mode: ' . $hire_end,5);
    // Select table to update
    $updateStmt = db_update('slt_groups');

    // Set conditions
    $updateStmt->condition('id', $grpId);
    $formid=$form_state['values']['id'];
    $default= db_query("SELECT id,name,status from slt_groups where id=:uid",array(":uid"=>$formid))->fetchAll();

    $fields = array(
        'name'              => trim($form_state['values']['name']),
        'duplicate'         => $duplicateId,
        'org_id'            => $sortedattribute['org_id'],
        'user_type'         => $sortedattribute['user_type'],
        'employment_type'   => $sortedattribute['employment_type'],
        'country'           => $sortedattribute['country'],
        'state'             => $sortedattribute['state'],
        'department'        => $sortedattribute['department'],
        'job_role'          => $sortedattribute['job_role'],
        'language'          => $sortedattribute['language'],
        'is_manager'        => $sortedattribute['is_manager'],
        'is_instructor'     => $sortedattribute['is_instructor'],
        'updated_by'        => $userid,
        'updated_on'        => now()
              );

    // Select the fields and the values to be updated
    $updateStmt->fields($fields);

    expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);

    // Execute query

    $numUpdated = $updateStmt->execute();
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);

    /* Group Attribute update*/
    
    if(!empty($form_state['values']['atr_ste_con_hdt'])){
         $hireModes = array_unique(explode(',',$form_state['values']['atr_ste_con_hdt']));
        
         /* Hire date Delete in group attribute table*/ 
         $delMapQry = db_delete('slt_group_attributes');
         $delMapQry->condition('group_id',$grpId,'=');
         expDebug::dPrintDBAPI('delete group attributes' , $delMapQry);
         $delMapQry->execute();
         
        if(in_array("ste_con_hdt_ltn",$hireModes)){
            $ltn_start = formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_ltn']));
            }
        if(in_array("ste_con_hdt_gtn",$hireModes)){
            $gtn_start =  formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_gtn']));
        }
        if(in_array("ste_con_hdt_btw",$hireModes)){
            $btw_start = formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_btw']));
            $btw_End = formatHireDate(trim($form_state['values']['hire_end']));
        }
        /* Hire date Update in group attribute table*/
            
             $insertAttr = db_insert('slt_group_attributes');
             $fields = array(
                    'group_id'          => $grpId,
                    'attr_type'         => 'HireDate',
                //'on_or_after_start_date'         => (!empty($gtn_start) ? $gtn_start : null),
                //'on_or_before_start_date'     => (!empty($ltn_start) ? $ltn_start : null),
                //'between_start_date'          => (!empty($btw_start) ? $btw_start : null),
                //'between_end_date'    => (!empty($btw_End) ? $btw_End : null),
                    'created_by'        => $userid,
                    'created_on'        => now(),
                    'updated_by'        => $userid,
                    'updated_on'        => now()
                  );
        if(!empty($gtn_start))
                $fields['on_or_after_start_date'] = $gtn_start;
        if(!empty($ltn_start))
                $fields['on_or_before_start_date'] = $ltn_start;
        if(!empty($btw_start)){
                $fields['between_start_date'] = $btw_start;
                $fields['between_end_date'] = $btw_End;
        }
            // Select the fields and the values to be inserted
            $insertAttr->fields($fields);
        expDebug::dPrintDBAPI(' Update attributes obj = ' , $insertAttr);
            $insertAttr->execute();
      }else{
        /* Hire date Delete in group attribute table*/
          $selt = db_select('slt_group_attributes','g');
          $selt->addField('g','id','id');
          $selt->condition('group_id',$grpId,'=');
          $selt1=$selt->execute()->fetchAll();
          
        $delMapQry = db_delete('slt_group_attributes');
        $delMapQry->condition('group_id',$grpId,'=');
        expDebug::dPrintDBAPI('delete group attributes' , $delMapQry);
        $delMapQry->execute();
        
        foreach($selt1 as $val){
            $fields = array(
                    'table_name' => 'slt_group_attributes',
                    'entity_id' => $val->id,
                    'entity_type' => 'group_attributes',
                    'module_name' => 'exp_sp_administration_groups',
                    'functionality_name' => 'updateRoleDetails',
                    'api_name' => 'GroupUpdateAPI',
                    'deleted_on' => now(),
                    'deleted_by' => getIdOfLoggedInUser(),
                    'parent1_entity_id' => $grpId,
                    'parent1_entity_type' => 'group_id',
                    
            );
            deleted_log_entry($fields);
         }

         }

    //Remove duplicate entry from non-conflicting groups
    if($form_state['values']['grp_war'] == 0){
        $select = db_select('slt_groups','grp');
        $select->addField('grp','id','id');
        $select->where('FIND_IN_SET('.$grpId.',duplicate)>0');
        expDebug::dPrintDBAPI('Fetch role Id ' , $select);
        $dupgrpId =$select->execute()->fetchCol();
        expDebug::dPrint(' $select ' . print_r($dupgrpId, true) , 3);

        foreach($dupgrpId as $dId){
            $userDelete=db_query('UPDATE slt_groups SET duplicate = TRIM(BOTH \',\' FROM REPLACE(
                    CONCAT(\',\', duplicate, \',\'),CONCAT(\',\', :grpid, \',\'), \',\'))WHERE id = :did',array('grpid'=>$grpId,'did'=>$dId));
            expDebug::dPrintDBAPI('User delete query',$userDelete);
        //  $userDelete->execute();
        }
    }
    //Entry for securitymanagementAdmin
    if($grpType == 'admin'){
        $select = db_select('role','role');
        $select->addField('role','rid','rid');
        $select->condition('role.name',$default[0]->name,'=');
        expDebug::dPrintDBAPI('Fetch role Id ' , $select);
        $role_id =$select->execute()->fetchField();

        expDebug::dPrintDBAPI('Fetch role Id ' , $select);

        expDebug::dPrint("Role update starts", 4);
            $rname      = trim($form_state['values']['name']);
            //$role_id  = $form_state['values']['id'];
            expDebug::dPrint("Drupal role id:".$role_id, 4);
            $action = $role_id>0?"Save role":"Add role";
            expDebug::dPrint("Role action in update :".$action, 4);
            require_once "./modules/user/user.admin.inc";
            $min_form_state = array( "values" =>  array( "op" => t($action), "name" => "$rname","rid" => $role_id ) );
            expDebug::dPrint("min state".print_r($min_form_state,true),5);
            if(strtolower($rname)=='anonymous user' or strtolower($rname)=='authenticated user'){
                    $form_errors['name']    = 'You can not edit this role.';
            }else{
                 user_admin_role_validate(null, $min_form_state);
                 $form_errors = form_get_errors();
            }
          if (empty( $form_errors )) {
               user_admin_role_submit(null, $min_form_state);
          }
        //  grpUserDelete($grpId,$role_id);
         // grpUserInsert($grpId);
    }
    
    $checkStatus = array ('new','inprogress');
    $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$grpId);
    
    
    $userslist = ($back_gd_running == 0) ? getUserDetailList($grpId,0,0,'','','',1) :getUserListForGroups($grpId, 0, 0,'usrtit');

    // Commented out for 87900: While saving a group "503 service unavailable error" is showing.
    /* 
    $comp_status = array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_exp');
    $overlAllStatus = array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_exp');
    expDebug::dPrint("Grp User Liset 1111".print_r($userslist, 1), 3);
    
	//    db_query("update slt_group_mapping sgm,  slt_enrollment se set se.mandatory=null where sgm.group_id=:group_id AND se.comp_status NOT IN (:comp_status) AND sgm.mro='cre_sys_inv_man' AND
	//             ((sgm.entity_id=se.class_id AND sgm.entity_type='cre_sys_obt_cls') OR (sgm.entity_id=se.course_id AND sgm.entity_type='cre_sys_obt_crs'))", array(':group_id' => $grpId,":comp_status"=>$comp_status));
    
	//     $tp_type = array("cre_sys_obt_crt", "cre_sys_obt_cur", "cre_sys_obt_trn");
	//     db_query("update slt_group_mapping sgm,  slt_master_enrollment sme set sme.mandatory=null where sgm.group_id=:group_id AND sme.overall_status NOT IN (:overall_status) AND sgm.mro='cre_sys_inv_man' AND
	//             sgm.entity_id=sme.program_id AND sgm.entity_type IN (:entity_type)", array(':group_id' => $grpId,':overall_status' => $overlAllStatus,':entity_type' => $tp_type));
    
    
    // Update all enrollment as Mandatory if any users fall in this group 
    $arg = array(':group_id' => $grpId, ":comp_status"=>$comp_status,":mandatory"=>NULL);
    updateRegistrationsAsMandatory($arg);
    // Update all master enrollments as Mandatory if any users fall in this group 
    $arg = array(':group_id' => $grpId, ":overall_status"=>$overlAllStatus,":mandatory"=>NULL,":entity_type" => $tp_type,);
    updateMasterRegistrationsAsMandatory($arg);
    
    $userIdArr = array();
    foreach($userslist as $arr){
        $userIdArr[] = $arr->user_id;
    }
    if(count($userIdArr) > 0){
	//         db_query("update slt_group_mapping sgm,  slt_enrollment se set se.mandatory='Y' where sgm.group_id=:group_id AND se.comp_status NOT IN (:comp_status) AND sgm.mro='cre_sys_inv_man' AND
	//                 ((sgm.entity_id=se.class_id AND sgm.entity_type='cre_sys_obt_cls') OR (sgm.entity_id=se.course_id AND sgm.entity_type='cre_sys_obt_crs')) AND se.user_id IN (:user_id)", array(':group_id' => $grpId, ':user_id' => $userIdArr,":comp_status"=>$comp_status));
	//         db_query("update slt_group_mapping sgm,  slt_master_enrollment sme set sme.mandatory='1' where sgm.group_id=:group_id AND sme.overall_status NOT IN (:overall_status) AND sgm.mro='cre_sys_inv_man' AND
	//                 sgm.entity_id=sme.program_id AND sgm.entity_type IN (:entity_type) AND sme.user_id IN (:user_id)", array(':group_id' => $grpId,':overall_status' => $overlAllStatus,':entity_type' => $tp_type, ':user_id' => $userIdArr));
        
        // Update all enrollment as Mandatory if any users fall in this group 
        $arg = array(':group_id' => $grpId, ':user_id' => $userIdArr,":comp_status"=>$comp_status,":mandatory"=>'Y');
        updateRegistrationsAsMandatory($arg);
        // Update all master enrollments as Mandatory if any users fall in this group 
        $arg = array(':group_id' => $grpId, ':user_id' => $userIdArr,":overall_status"=>$overlAllStatus,":mandatory"=>'1',":entity_type" => $tp_type,);
        updateMasterRegistrationsAsMandatory($arg);
    }
    //Compliance update
	//     db_query("update slt_group_mapping sgm,  slt_enrollment se set se.is_compliance=null where sgm.group_id=:group_id AND se.comp_status NOT IN (:comp_status) AND se.is_compliance='1' AND
	//             ((sgm.entity_id=se.class_id AND sgm.entity_type='cre_sys_obt_cls') OR (sgm.entity_id=se.course_id AND sgm.entity_type='cre_sys_obt_crs'))", array(':group_id' => $grpId,":comp_status"=>$comp_status));
    
    // Update all enrollment as NON Compliance of the users who fall in this group and the course is compliance
    $arg = array(':group_id' => $grpId, ":comp_status"=>$comp_status,":compliance"=>NULL);
    updateRegistrationsAsCompliance($arg);
    
    if(count($userIdArr) > 0){      
	//         $retrt = db_query("update slt_group_mapping sgm,  slt_enrollment se set se.is_compliance='1' where sgm.group_id=:group_id AND se.comp_status NOT IN (:comp_status) AND sgm.mro='cre_sys_inv_opt' AND
	//                 ((sgm.entity_id=se.class_id AND sgm.entity_type='cre_sys_obt_cls') OR (sgm.entity_id=se.course_id AND sgm.entity_type='cre_sys_obt_crs')) AND se.user_id IN (:user_id)", array(':group_id' => $grpId, ':user_id' => $userIdArr,":comp_status"=>$comp_status));
        
        // Update all enrollment as Compliance if any users fall in this group and the course is compliance
        $arg = array(':group_id' => $grpId, ':user_id' => $userIdArr,":comp_status"=>$comp_status,":compliance"=>1);
        updateRegistrationsAsCompliance($arg);
        
    } */
    
    
    
    
    //Audit Trail Entry
    $formFieldsList = array('name', 'description','status','created_by','created_on','department','employment_type','job_role','language','org_id','is_manager','is_instructor','user_type','country','state');

    make_audit_trail_entries($fields, getIdOfLoggedInUser() , $default ,$formid, 'Updated', 'cre_sec', 'exp_sp_administration_groups',
                                         'updateRoleDetails', $formFieldsList);
    //addTaxonomyTermForOrg($form_state['values']['id'], $form_state['values']['name'], '', $form_state['values']['status']);

    // Insert Tax Exempted value in slt_entity_profile_mapping table
    //saveTaxExemptedOrgCustomProfile($form_state['values']['id'], $form_state['values']['tax_exempted'], $userid);

    if ($callFromAPI) {
      return array((object)array(
           'group_id'    =>$form_state['values']['id'],
           'group_name'  =>$form_state['values']['name'],
           'status'      => 'updated'));
    } else {
      return array((object)array('id'=>$form_state['values']['id']));
    }

  }
  catch (Exception $ex) {
    expDebug::dPrint("ERROR -- ".print_r($ex,true),1);
    watchdog_exception('updateRoleDetails', $ex);
    //return array((object)array('id'=>'failure'));
    $errobj=new stdClass();
    $errobj->isValidateError = 1;
    $errobj->errcode = 'L_012';
    $errobj->errormsg = "Error in RoleDetails.";
    return $errobj;
  }
}

function updateRegistrationsAsCompliance($args){
    $qry = "
            UPDATE
                slt_enrollment e 
            INNER JOIN (
              SELECT DISTINCT c1.course_id crs_id 
              FROM slt_group_mapping sgm
              INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type='cre_sys_obt_cls')  
                OR (sgm.entity_id=c1.course_id AND sgm.entity_type='cre_sys_obt_crs')
              WHERE sgm.group_id = :group_id 
              AND sgm.mro='cre_sys_inv_opt'
              AND c1.status = 'lrn_cls_sts_atv'
              AND c1.is_compliance = 1) crs ON  crs.crs_id = e.course_id
            SET e.is_compliance = :compliance
            WHERE e.comp_status NOT IN ( :comp_status ) 
             ";
    
    $qry .= (isset($args[':user_id'])) ? " AND e.user_id IN ( :user_id ) " : '';
    $qry .= ($args[':compliance'] == 1) ? " AND e.is_compliance IS NULL " : ' AND e.is_compliance IS NOT NULL ';
    
  expDebug::dPrintDBAPI("Update enrollments as compliance when group changes ",$qry,$args);
  $update = db_query($qry,$args);
}

function updateRegistrationsAsMandatory($args){
    $qry = "
            UPDATE
                slt_enrollment e 
            INNER JOIN (
              SELECT DISTINCT c1.course_id crs_id 
              FROM slt_group_mapping sgm
              INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type='cre_sys_obt_cls')  
                OR (sgm.entity_id=c1.course_id AND sgm.entity_type='cre_sys_obt_crs')
              WHERE sgm.group_id = :group_id 
              AND sgm.mro='cre_sys_inv_man'
              AND c1.status = 'lrn_cls_sts_atv') crs ON  crs.crs_id = e.course_id
            SET e.mandatory = :mandatory
            WHERE e.comp_status NOT IN ( :comp_status ) 
            ";
    
    $qry .= (isset($args[':user_id'])) ? " AND e.user_id IN ( :user_id ) " : '';
    $qry .= ($args[':mandatory'] == 'Y') ? "AND e.mandatory IS NULL" : 'AND e.mandatory IS NOT NULL';
  expDebug::dPrintDBAPI("Update enrollments as mandatory when group changes ",$qry,$args);
  $update = db_query($qry,$args);
}

function updateMasterRegistrationsAsMandatory($args){
    $qry = "
            UPDATE
                slt_master_enrollment e 
            INNER JOIN (
              SELECT DISTINCT c1.id crs_id 
              FROM slt_group_mapping sgm
              INNER JOIN slt_program c1 ON sgm.entity_id=c1.id AND sgm.entity_type IN ( :entity_type )
              WHERE sgm.group_id = :group_id 
              AND sgm.mro='cre_sys_inv_man'
              AND c1.status = 'lrn_lpn_sts_atv') crs ON  crs.crs_id = e.program_id
            SET e.mandatory = :mandatory,e.updated_on=now()
            WHERE e.overall_status NOT IN ( :overall_status ) 
            ";
    
    $qry .= (isset($args[':user_id'])) ? " AND e.user_id IN ( :user_id ) " : '';
    $qry .= ($args[':mandatory'] == 'Y') ? "AND e.mandatory IS NULL" : 'AND e.mandatory IS NOT NULL';
  expDebug::dPrintDBAPI("Update manster enrollments as mandatory when group changes ",$qry,$args);
  $update = db_query($qry,$args);
}

function activateOrDeactivateGrp($grpId,$status='',$printResult=true){
    expDebug::dPrint('status : '. $status ,4);
  $currUserId = getSltpersonUserId();
  $grpId=orgValidate($grpId); // Added by ganeshbabuv to avoid the security issue on June 15th 2015 4:00 PM
  try {
    $is_admin = adminGrporNot($grpId);
    $grpCode = getGrpCode($grpId);
    if($status==''){
     $status  = getGrpStatus($grpId);
     $updatedstatus  = ($status == 'cre_sec_sts_atv') ? "cre_sec_sts_itv" : "cre_sec_sts_atv";
     }else{
     $updatedstatus  = ($status == 'Activate') ? "cre_sec_sts_atv" : "cre_sec_sts_itv";
     if($status == 'Activate'){
        $status = 'cre_sec_sts_itv';
       }else{
        $status = 'cre_sec_sts_atv';
       }
    }
    $updateStmt = db_update('slt_groups');
    $updateStmt->condition('id', $grpId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
    expDebug::dPrintDBAPI(' $updateStmt ' ,  $updateStmt);
    if($updatedstatus == "cre_sec_sts_itv"){
         syncSolrData('Course');
         syncSolrData('Program');
         //syncSolrData('User','','Bulk');
         
         $sel = db_select('slt_group_mapping','g');
         $sel->addField('g','id','id');
         $sel->addField('g','entity_id','entity_id');
         $sel->addField('g','group_type','group_type');
         $sel->condition('group_id',$grpId,'=');
         $sel1=$sel->execute()->fetchAll();
         
       $delMapQry = db_delete('slt_group_mapping');
       $delMapQry->condition('group_id',$grpId,'=');
       expDebug::dPrintDBAPI('delete group mapping' , $delMapQry);
       $delMapQry->execute();
       
       foreach($sel1 as $val)
       {
           $fields = array(
                   'table_name' => 'slt_group_mapping',
                   'entity_id' => $val->id,
                   'entity_type' => 'group_mapping',
                   'module_name' => 'exp_sp_administration_groups',
                   'functionality_name' => 'activateOrDeactivateGrp',
                   'api_name' => null,
                   'deleted_on' => now(),
                   'deleted_by' => getIdOfLoggedInUser(),
                   'parent1_entity_id' => $val->entity_id,
                   'parent1_entity_type' => 'entity_id',
                   'parent2_entity_id' => $grpId,
                   'parent2_entity_type' => 'group_id',
                   'parent3_entity_id' => $val->group_type,
                   'parent3_entity_type' => 'group_type',
           );
           deleted_log_entry($fields);
           //deleted_log_entry('slt_group_mapping',$val->id,'group_mapping','exp_sp_administration_groups','activateOrDeactivateGrp',null,getIdOfLoggedInUser());  
       }
       
      updateGroupMappingDetails('','', '','suspendupdate',$grpId);
    } 
    //Entry for securitymanagementAdmin
    if($is_admin == 1){ //|| $grpCode == 'grp_ins' || $grpCode == 'grp_mgr' ){
        if($updatedstatus=='cre_sec_sts_atv'){
            $updateStmt = db_update('slt_admin_group_users');
            $updateStmt->condition('group_id', $grpId);
            $updateStmt->fields(array(
                    'status'    => 'cre_sec_sts_atv',
                    'updated_on'    => now(),
                    'updated_by'    => $currUserId
            ));
            $updateStmt->execute();
        }else{
            $updateStmt = db_update('slt_admin_group_users');
            $updateStmt->condition('group_id', $grpId);
            $updateStmt->fields(array(
                    'status'    => 'cre_sec_sts_itv',
                    'updated_on'    => now(),
                    'updated_by'    => $currUserId
            ));
            expDebug::dPrintDBAPI('delete userlist ' , $select1);
            $updateStmt->execute();
        }
        $select = db_select('role','role');
        $select->join('slt_groups','grp','role.name=grp.name');
        $select->addField('role','rid','rid');
        $select->addField('grp','userslist','ulist');
          $select->condition('grp.id',$grpId,'=');
          expDebug::dPrintDBAPI('Fetch role Id ' , $select);
          $dbquery = $select->execute()->fetchAll();
          $role_id = $dbquery[0]->rid;
          $newUserList = $dbquery[0]->ulist;
          if(!empty($newUserList)){
                $newUserListArray = explode(',',$newUserList);
                foreach($newUserListArray as $perId){
                    $uId = getDrupalIdOfUser($perId);
                    if($updatedstatus == "cre_sec_sts_atv"){
                        $users_roles = db_select('users_roles', 'roles');
                        $users_roles->condition('roles.uid',$uId ,'=');
                        $users_roles->condition('roles.rid',$role_id ,'=');
                        $users_roles->addField('roles', 'rid','rid');
                        $result_count =  $users_roles->execute()->rowCount();
                        expDebug::dPrintDBAPI('select from users_roles in activate group ' , $users_roles);
                        if($result_count == '0') {
                        $select1 = db_insert('users_roles');
                    $select1->fields(array('uid' => $uId, 'rid' => $role_id));
                    expDebug::dPrintDBAPI('Insert userlist ' , $select1);
                    $select1->execute();
                        }
                    }else if($updatedstatus == "cre_sec_sts_itv"){
                        $select1 = db_delete('users_roles');
                    $select1->condition('uid',$uId);
                    $select1->condition('rid',$role_id);
                    expDebug::dPrintDBAPI('delete userlist ' , $select1);
                    $select1->execute();
                        $fields = array(
                            'table_name' => 'users_roles',
                            'entity_id' => $uId,
                            'entity_type' => 'group_users',
                            'module_name' => 'exp_sp_administration_groups',
                            'functionality_name' => 'activateOrDeactivateGrp',
                            'deleted_on' => now(),
                            'deleted_by' => getIdOfLoggedInUser(),
                            'parent1_entity_id' => $role_id,
                            'parent1_entity_type' => 'rid',
                            'parent2_entity_id' => $uId,
                            'parent2_entity_type' => 'uid',
                    
                    );
                    
                    deleted_log_entry($fields);
                    }
                }
            }
            // To reset stored active session details (implemented for performance)
        // resetStoredSessionsForActiveUsers();
    } 
    // Added by Vincent on 03 Feb, 2017. For #0071964
    populateGroupUsersInFlatTable($grpId,'','GroupStatusChange');
    
    //Audit Trail entry
    add_audit_trail_entry($currUserId, $grpId, 'cre_sec', 'exp_sp_administration_groups', 'activateOrDeactivateGrp', 'Updated Role Status',
                                             null, $status, $updatedstatus);
    if($printResult){
      print   ($updatedstatus == 'cre_sec_sts_atv') ? t('LBL572') : t('LBL573');
    }
  }
  catch (Exception $ex) {
    watchdog_exception('activateOrDeactivateRole', $ex);
    throw new Exception($ex);
  }
}
/*
 * getDefaultRoleAddEditFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultRoleAddEditFormFieldValue($field, $form_state, $roleDetails) {
    try {
      $defaultValue = '';
      switch ($field) {
        case 'shordesc':
          $defaultValue = (empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='')?
                                        ((empty($roleDetails[$field]) || $roleDetails[$field]=='')? '' : $roleDetails[$field]) :
                                        $form_state['values'][$field]['value'];
          break;
          default:   // For all other fields
          $defaultValue = getDefaultAddEditFormFieldValue($field, $form_state, $roleDetails);
          break;
      }

      expDebug::dPrint(' $defaultValue for field ' . $field . ' = ' . print_r($defaultValue, true) , 4);

      return $defaultValue;
  } catch (Exception $ex) {
    watchdog_exception('getDefaultRoleAddEditFormFieldValue', $ex);
    expertusErrorThrow($ex);
  }
}

function getGrpStatus($grpId){
    try {
      $select = db_select('slt_groups', 'grp');
      $select->addField('grp','status', 'status');
      $select->condition('grp.id',$grpId);
      $result = $select->execute()->fetchField();
      // Execute the query
      return $result;
  } catch (Exception $ex) {
    watchdog_exception('getGrpStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function addUsersDetails($entityId,$entityType){
  try {
     expDebug::dPrint(' $entityId '.$entityId.' $entityType '.$entityType,4);
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'entity_type'  => $entityType,
       'call_from'    => 'add_user'
      );

      $formId                 = 'addusr_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $addUsersResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($addUsersResponse);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('addUsersDetails', $ex);
    expertusErrorThrow($ex);
  }

}

function attachUsersDetails($form, &$form_state){
  try {
    include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
    $entityId = $form_state['triggering_element']['#ajax']['role_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];

    expDebug::dPrint(' $entityId '.$entityId.' $entityType '.$entityType,4);
    $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'entity_type'  => $entityType,
       'call_from'    => 'attach_user'
      );

    $commands = array();
    $commands[] = exp_ctools_modal_adjust();
    $commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
    $builtForm            = drupal_build_form('addusr_addedit_form_html', $form_state);
    $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
    return array('#type' => 'ajax', '#commands' => $commands);

      /*$formId                 = 'addusr_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $addUsersResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($addUsersResponse);
      exit;*/

  } catch (Exception $ex) {
    watchdog_exception('attachUsersDetails', $ex);
    expertusErrorThrow($ex);
  }

}


function showAttachUserToRole($form, &$form_state){
    try {
    include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
    $entityId = $form_state['triggering_element']['#ajax']['role_id'];
    $entityType = 'cre_sec';

     expDebug::dPrint(' $entityId '.$entityId.' $entityType '.$entityType ,4);
     $form_state = array(
            'no_redirect'  => TRUE,
            'build_info'   => array('args' => array('ajax')),
            'role_id'      => $entityId,
            'entity_type'  => $entityType,
         // 'form_arg_type' => $argType
            'mode'         => 'view'
          );

      $commands = array();
      $commands[] = exp_ctools_modal_adjust();
        $commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
        $builtForm  = drupal_build_form('display_assigned_users', $form_state);
        $commands[] = ajax_command_refresh_narrow_search_results('onclose');
        $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));

        return array('#type' => 'ajax', '#commands' => $commands);
    } catch (Exception $ex) {
        watchdog_exception('showAttachUserToRole', $ex);
        expertusErrorThrow($ex);
    }
}

function addusr_addedit_form_html($form,&$form_state) {
  try {
    return displayAddUsersDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type']);
  } catch (Exception $ex) {
    watchdog_exception('addusr_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }

}

function displayAddUsersDetails($form,&$form_state,$roleId, $userId){
    try {
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
        $call_from    = $form_state['call_from'];
        $role_id = $form_state['entity_id'];
        $entityType = 'cre_sec';
        $userId= $form_state['user_id'];
        $excludedRoleId = '';
        $uniqueId = "grpAddUsers-".$role_id."-".$userId;
        $wrapperId = 'security-role-detail-wrapper';

        $displayMode = 'edit';
        $form_state['display_mode'] = $displayMode;
        $form = array();
        $form['#prefix'] = /* (stripos($_SERVER['HTTP_USER_AGENT'], 'chrome') !== false
                                            || (stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') !== false)? '</form>' : '' ) */ '</form>'.'<div id="security-role-detail-wrapper">';
        $form['#suffix'] = '</div>';
        $setminHight =  'admin-add-users-tab-datagrid-wrapper';
        $form['edit-class-details-start'] = array(
          '#prefix' => "<div id='add-user-role-page' class = 'add-users-grp'>",
        );

        if(isset($addAttributes))
            unset($addAttributes['onclick']);
      $addAttributes['class'][] = 'narrow-search-actionbar-orange-btnBG';
      if($call_from=='add_user'){
        $addAttributes['data-wrapperid'] = array('role-detail-wrapper');
      }else{
        $addAttributes['data-wrapperid'] = array('security-role-detail-wrapper');
      }
      $form['add-class-list'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="catalog-class-addedit-form-details"><div id="add-class-list-wrapper">'
      );
         /* hidden input type to carry the selected users */
      $form['hidden_idlist'] = array(
          '#type' => 'hidden',
          '#name' => 'hidden_idlist_'.$uniqueId,
          '#id' => 'datagrid-idlist-'.$uniqueId,
        );
      $paintMultiAction  = "<div id='search-list-class-title-keyword' class='search-list-keyword' style='display:block;'>";
      $paintMultiAction .= "<span id='search-dropdwn-list-class'>";
      $paintMultiAction .= "<input type='hidden' id='search_all_user_type-hidden' value='usrtit' />";
      $paintMultiAction .= "<input type='hidden' id='addusername-search-autocomplete_hidden' value='".sanitize_data(t('LBL181'))."' />";
      $paintMultiAction .= "<span id='select-list-class-dropdown-adduser' class='select-list-dropdown-class'>".t('LBL054')."</span>";
      $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-class-dropdown-link' onclick='$(\"#root-admin\").data(\"peoplegroup\").moreRoleHideShow();'> </a>";
      $paintMultiAction .= "<ul id='select-list-class-dropdown-list' class='more-drop-down'>";
      $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreUserSearchTypeText('".addslashes(t('LBL054'))."','usrtitle','role');\">".t('LBL054')."</li>";
      $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreUserSearchTypeText('".addslashes(t('LBL691'))."','fultitle','role');\">".t('LBL691')."</li>";
      $paintMultiAction .= "</ul>";
      $paintMultiAction .= "</span></div>";

      $form['class_multi_search_container'] = array(
         '#type' => 'markup',
         '#markup' => '<div class="role_multi_search_container_add">', //class="admin_class_multi_search_container"
         );
      $form['more_add_enrolluser_search'] = array(
         '#type' => 'markup',
         '#markup' => $paintMultiAction,
         );
      $form['class_auto-search'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-class-auto-search-container">',
          );
      $userSearchType = '';
      $userSearchType = $form_state['storage']['search_usr_type'];
      $searchAjaxEvent = 'click';

      $acMenuPath = 'administration/people/group/user-name-autocomplete/'.$role_id.'/'.'addusers';

        $excludedRoleId =0;
      $default_text_title = t('LBL181');
      $autoCompleteAttributes['class'][] = 'admin_class_ac_input_mainform';
      $autoCompleteAttributes['onkeypress'][]   = 'disableEnrterKey(event);'; // added by vincent for prevent the enter key submit in search box
      addACFieldForMultiSelect($form, 'addusername-search-autocomplete', $acMenuPath, '', '', '', array(), array('entity_id'  =>  $role_id, 'entity_type' => "'".$entityType."'" ,'search_type' => "$('#search_all_user_type-hidden').val()",'excluded_role_id' => "'".$excludedRoleId."'", 'id_required' => 0), false, false, $default_text_title,$autoCompleteAttributes);
      $form['class_auto-search-end'] = array(
          '#type' => 'markup',
          '#markup' => '</div>',
          );

      $form['tip-position'] = array(
          '#type' => 'markup',
          '#markup' => '<input type="hidden" id="qtip_position" name="qtip_position" value="">',
          );

      $excludedRoleId =0;
      $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "grpAddUsers", $(\'#addusername-search-autocomplete\').val(), "'.$role_id.'", "'.$userId.'", "'.$excludedRoleId .'"); return false;';
      $searchAttributes['Class'][] = 'admin-pagination-search-go narrow-text-search';
      $form['search_addusers'] = array(
            '#type' => 'submit',
            '#id' => 'search-addusers',
            '#title' => t('LBL304'), //Search
            '#name' => 'Search addusers',
            '#submit' => array('search_addusers'), // No such function search_attachcourse(), apparently property is ignored.
            '#attributes' => $searchAttributes,
          );
      $form['class_multi_search_container-end'] = array(
          '#type' => 'markup',
          '#markup' => '<div class= "filter-search-start-date-right-bg"></div></div>',
          );
        $form['edit-class-details-paginate'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="admin-add-course-training-plan" class="display_user_list"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
        '#suffix' => '<div>',
      );
      $form['role_user_view_grid_markup'] = array(
          '#type' => 'markup',
          '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common" style="display:none;"></div></div>',
       );
        $form['role_user_grid_load'] = array(
          '#type' => 'markup',
        //  '#prefix' => '<div style="display:block"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'view\', \'RoleDisplayUsers\', \'\', \''.$role_id.'\', \''.$entityType.'\', \''.$excludedRoleId.'\');" width="100" height="132" /></div>',
          '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'grpAddUsers\', $(\'#addusername-search-autocomplete\').val(), \''.$role_id.'\', \''.$userId.'\',\'0\');" width="100" height="132" /></div>',
        );
        $form['role_user_grid_load_end'] = array(
          '#type' => 'markup',
          '#prefix' => '</div></div>',
        );

      $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
      if($call_from=='add_user'){
        $cancelAttributes['onclick'][]         =  "closeQtip('','$role_id'); return false;"; //($call_from=='add_user') ? "$('.qtip-active').remove();return false;" : "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
      }else{
        $cancelAttributes['onclick'][]         =  "closeQtip('','$role_id'); return false;"; //($call_from=='add_user') ? "$('.qtip-active').remove();return false;" : "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
      }
      $cancelAttributes['data-wrapperid'] = array($wrapperId);

      $form['cancel_adduser_edit_mode'] = array(
         '#type' => 'submit',
         '#value' => t('LBL123'),
         '#title' => t('LBL123'),
         '#prefix' => '<div id="attach_user" ><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
         '#suffix' =>   '<div class="white-btn-bg-right"></div>',
         '#name' => 'Cancel addusers',
         '#attributes' => $cancelAttributes,
       );
      module_load_include('inc', 'exp_sp_administration_learning', 'exp_sp_administration_learning');
      $type='grpAddUsers';
      $listquery = fetchGrpUserList($role_id,'',$type);
      expDebug::dPrintDBAPI('listquery', $listquery);
      if($call_from=='add_user'){
        $callback = 'addUsersCallBack';
        $wrapper='role-detail-wrapper';
        $setAttributes['data-wrapperid'] = 'role-detail-wrapper';
      }else{
        $callback = 'showAttachUserToRole';
        $wrapper = 'security-role-detail-wrapper';
        $setAttributes['data-wrapperid'] = 'security-role-detail-wrapper';
      }
      if(count($listquery) != 0){
         expDebug::dPrint('count($listquery)'.count($listquery), 4);
         $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
         $form['set_attachcourse_edit_mode'] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-button-container role-add-user-btn"><div class="admin-save-button-left-bg"></div>',
            '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div></div>',
            '#title' => t('LBL1038'),
            '#value' => t('LBL287'),
            '#name' => t('LBL287'),
            '#submit' => array('addUsersDetailsList'),
            '#attributes' => $setAttributes,
            '#ajax' => array(
                'wrapper' => $wrapper,
                'callback' => $callback,
                'role_id' =>$role_id,
                'user_id' =>$userId,
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
      }
      return $form;
  } catch (Exception $ex) {
    watchdog_exception('displayAddUsersDetails', $ex);
    expertusErrorThrow($ex);
  }
}


function addUsersCallBack($form, &$form_state){
    try{
      $roleId = $form_state['triggering_element']['#ajax']['role_id'];
      $entityType = 'cre_sec';

      expDebug::dPrint('triggering elemnt 1 -'.$form_state['triggering_element']['#ajax']['role_id'],4);

      expDebug::dPrint('exp_sp_administration_grouping_users_callback formstateee : '.print_r($form_state,true) , 5);


      $min_form_state = array(
       'ajax' => TRUE,
       're_render' => TRUE,
       'no_redirect' => TRUE,
       'role_id' => $roleId,
       'build_info' => array('args' => array('ajax'))
      );

      $form_state = $min_form_state;
      //$form_state['role_id'] = $roleId;

      $formId = 'exp_sp_administration_groups_attaching_user_form';
      $builtForm = drupal_build_form($formId, $form_state);

      $commands = array();
      $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
      $commands[] = ajax_command_invoke(NULL, "attachClose", array($roleId,$entityType));
      $commands[] = ajax_command_refresh_narrow_search_results('onclose');
      //$commands[] = exp_ctools_modal_adjust();

      return array(
            '#type' => 'ajax',
            '#commands' => $commands
      );
    }catch (Exception $ex) {
    watchdog_exception('addUsersCallBack', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * returns the list of users who are member of a groups based on attributes of group and user 
 * @param unknown $grpId
 * @param number $start
 * @param number $end
 * @param unknown $userName
 * @param unknown $userSearchType
 * @param string $count
 * @param string $fetchId
 * @return A|unknown
 */
function getUserDetailList($grpId,$start = 0,$end = 0,$userName,$userSearchType,$count='',$fetchId ='',$insert_users=0){
    try {
         
         $grpDetails = getGrpDetails($grpId,
                        array('code',
                            'is_admin',
                            'org_id',
                            'user_type',
                            'employment_type',
                            'country',
                            'state',
                            'department',
                            'job_role',
                            'language',
                            'is_manager',
                            'is_instructor'
                        ));
         $grpCode=$grpDetails['code'];
         $isadm = $grpDetails['is_admin'];
         $hireDetails = getHireDetails($grpId);
         $userId = array(1,2);
         
         if($insert_users==1){
            $select = db_select('slt_person','per');
            $select->join('slt_groups','grp');
            $select->join('users','usr','usr.name = per.user_name');
            $select->distinct();
            if(!empty($count)){
                $select->addExpression('count(distinct per.id)', 'cnt');
            }else{
                $select->addField('per','id','user_id');
                $select->addField('usr','uid','uid');
                $select->addField('usr','status','status');
                if(empty($fetchId)){
                    $select->addField('per','user_name','uname');
                    $select->addField('per','full_name','fname');
                }
            }
            $select->condition('per.id',$userId,'not in');
            $select->condition('grp.id',$grpId,'=');
            //$select->condition('per.status','cre_usr_sts_atv','=');
            $select->condition('per.status','cre_usr_sts_del','!='); // Added by Vincent on 3 Feb, 2017 Deleted user should not shown.
            if($grpCode != 'grp_ins' && $grpCode != 'grp_mgr' && $grpCode != 'grp_adm' && $grpCode != 'grp_sup'){
                if($isadm){
                    $flg=0;
                    $select1=array();
                    if(!empty($grpDetails['org_id'])){
                        $select1[] = ("if(grp.org_id='All',per.org_id is not null,FIND_IN_SET(per.org_id,grp.org_id)>0)");
                        $flg=1;
                    }
                    if(!empty($grpDetails['user_type'])){
                        $select1[] = ("if(grp.user_type='All',per.user_type is not null,FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 )");
                        $flg=1;
                    }
                    if(!empty($grpDetails['employment_type'])){
                        $select1[] = ("if(grp.employment_type='All',per.employment_type is not null,FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0)");
                        $flg=1;
                    }
                    if(!empty($grpDetails['country'])){
                        $select1[] = ("if(grp.country='All',per.country is not null,FIND_IN_SET(ifnull(per.country,''),grp.country)>0)");
                        $flg=1;
                    }
                    if(!empty($grpDetails['state'])){
                        $select1[] = ("if(grp.state='All',per.state is not null,FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0)");
                        $flg=1;
                    }
                    if(!empty($grpDetails['department'])){
                        $select1[] = ("if(grp.department='All',per.dept_code is not null,FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0)");
                        $flg=1;
                    }
                    if(!empty($grpDetails['job_role'])){
                        //7306: Support of users to have more than 1 job role association
                        $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
                        $select1[] = ("if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))");
                        $flg=1;
                    }
                    if(!empty($grpDetails['language'])){
                        $select1[] = ("if(grp.language='All',per.preferred_language is not null,FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0)");
                        $flg=1;
                    }
                    if($grpDetails['is_manager']=='Y' && $grpDetails['is_instructor']=='Y'){
                        $select1[] = ("(ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)");
                        $flg=1;
                    }
                    if($grpDetails['is_instructor']=='Y' && $grpDetails['is_manager']=='N'){
                        $select1[] = ("ifnull(per.is_instructor,'N') = grp.is_instructor");
                        $flg=1;
                    }
                    if($grpDetails['is_manager']=='Y' && $grpDetails['is_instructor']=='N'){
                        $select1[] = ("ifnull(per.is_manager,'N') = grp.is_manager");
                        $flg=1;
                    }
                        
                    if(count($hireDetails) > 0){
                        $ordbVal = '';
                        if(!empty($hireDetails[0]->between_start_date) && !empty($hireDetails[0]->between_end_date)) {
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') between (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) AND (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))") : " OR ". ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') between (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) AND (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))");
                        }
                        if(!empty($hireDetails[0]->on_or_after_start_date)){
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))"): " OR ".("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))");
                        }
                        if(!empty($hireDetails[0]->on_or_before_start_date)){
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))"): " OR ".("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))");
                        }
                        $select1[] = '('.$ordbVal.')';
                        $flg=1;
                    }
                    if($flg==1){
                        $cond = implode(' AND ',$select1);
                        $select->where("((".$cond." AND ((grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))) OR (FIND_IN_SET(per.id,grp.added_users)>0))");
                    }else{
                        $select->where("(if(grp.added_users is not null,(FIND_IN_SET(per.id,grp.added_users)>0),1=2))");
                    }
                }else{
                    if(count($hireDetails) > 0){
                        $ordbVal = '';
                        if(!empty($hireDetails[0]->between_start_date) && !empty($hireDetails[0]->between_end_date)) {
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') between (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) AND (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))") : " OR ". ("(DATE_FORMAT(per.hire_date) between (DATE_FORMAT('".$hireDetails[0]->between_start_date."','%Y-%m-%d')) AND (DATE_FORMAT('".$hireDetails[0]->between_end_date."','%Y-%m-%d')))");
                        }
                        if(!empty($hireDetails[0]->on_or_after_start_date)){
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))"): " OR ".("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') >= (DATE_FORMAT('".$hireDetails[0]->on_or_after_start_date."','%Y-%m-%d')))");
                        }
                        if(!empty($hireDetails[0]->on_or_before_start_date)){
                            $ordbVal .= empty($ordbVal) ? ("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))"): " OR ".("(DATE_FORMAT(per.hire_date,'%Y-%m-%d') <= (DATE_FORMAT('".$hireDetails[0]->on_or_before_start_date."','%Y-%m-%d')))");
                        }
                        $or_cond = '('.$ordbVal.')';
                    }else{
                        $or_cond = "(1=1)";
                    }
                    $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
                    $select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
                                                AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
                                                AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
                                                AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
                                            AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
                                            AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
                                            AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
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
                                            AND ".$or_cond."
                                                AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
                }
            }else{
                if($grpCode == 'grp_ins'){
                    $select->where("grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0");
                    $select->condition('per.is_instructor','Y','=');
                }
                else if($grpCode == 'grp_mgr'){
                    $select->where("grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0");
                    $select->condition('per.is_manager','Y','=');
                }
                else{
                    $select->where("(FIND_IN_SET(per.id,grp.added_users)>0 and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))");
                }
            
            }
            if(!empty($userName)&& $userName!=t('LBL181') && $userSearchType=='usrtit' ){
                $select->condition(db_or()->condition('per.user_name', '%' . db_like($userName) . '%' ,'like')
                        ->condition('per.full_name', '%' . db_like($userName) . '%' ,'like'));
            }
            if(!empty($userName)&& $userName!=t('LBL036'). ' '.t('LBL691') && $userSearchType=='fulltit' ){
                $select->condition('per.full_name', '%' . db_like($userName) . '%', 'LIKE');
            }
            if(($start > 0) || ($end > 0)) {
                $select->range($start,$end);
            }
            
            if(!empty($count)){
                $result = $select->execute()->fetchField();
                return $result;
            }
            
            //$select->orderBy('enable', 'desc');
            expDebug::dPrintDBAPI('list of users in role' ,   $select);
            //if(empty($fetchId)){
            $result = $select->execute()->fetchAll();
            // }else{
            //  $result = $select->execute()->fetchCol();
            // }
            
            expDebug::dPrint(' $result = ' . print_r($result, true) ,3);
            return $result;
    }else{
        //   if($isadm == 0 ){
        $select = db_select('slt_person','per');
        $select->innerjoin('slt_group_user_mapping','gmap','gmap.user_id = per.id');
        $select->innerjoin('users','usr','usr.name = per.user_name');
        $select->condition('per.id',$userId,'not in');
        $select->condition('per.status','cre_usr_sts_del','!=');
        $select->condition('gmap.group_id',$grpId,'=');
        $select->condition('gmap.user_type','R','!=');
        if(!empty($count)){
            $select->addExpression('count(distinct per.id)', 'cnt');
        }else{
            $select->addField('per','id','user_id');
            $select->addField('usr','uid','uid');
            $select->addField('usr','status','status');
            if(empty($fetchId)){
                $select->addField('per','user_name','uname');
                $select->addField('per','full_name','fname');
            }
        }
        if(!empty($userName)&& $userName!=t('LBL181') && $userName!=t('LBL036'). ' '.t('LBL691') ){
            if($userSearchType=='usrtit' || $userSearchType=='usrtitle')
                $select->condition('per.user_name', '%' . db_like($userName) . '%' ,'like');
            else
                $select->condition('per.full_name', '%' . db_like($userName) . '%', 'LIKE');
        }
        
        if(($start > 0) || ($end > 0)) {
            $select->range($start,$end);
        }
        expDebug::dPrintDBAPI('list of users in role' ,   $select);
        if(!empty($count)){
            $result = $select->execute()->fetchField();
            return $result;
        }
        $result = $select->execute()->fetchAll();
        
        
        //  expDebug::dPrint(' $result 122222= ' . print_r($result, true) ,3);
        return $result;
        // }
                    
         }
      
   }catch(Exception $ex) {
      watchdog_exception('getUserDetailList', $ex);
      expertusErrorThrow($ex);
   }
}

/*
 * function to fetch userlist from group user mapping table
 * */
function getUserListForGroups($grpId,$start = 0,$end = 0,$userName='',$userSearchType='',$count='',$qry=0){
    try{
        $userId = array(1,2);
        $select = db_select('slt_group_user_mapping','map');
        $select->join('slt_person','per','map.user_id = per.id');
        $select->join('slt_groups','grp','grp.id = map.group_id');
        $select->join('users','usr','usr.name = per.user_name');
        $select->distinct();
        if(!empty($count)){
            $select->addExpression('count(distinct per.id)', 'cnt');
        }else{
            $select->addField('per','id','user_id');
            $select->addField('usr','uid','uid');
            $select->addField('usr','status','status');
            if(empty($fetchId)){
                $select->addField('per','user_name','uname');
                $select->addField('per','full_name','fname');
            }
        }
        $select->condition('per.id',$userId,'not in');
        $select->condition('map.group_id',$grpId,'=');
        //$select->condition('per.status','cre_usr_sts_atv','=');
        $select->condition('per.status','cre_usr_sts_del','!=');
        if(!empty($userName)&& $userName!=t('LBL181') && $userSearchType=='usrtit'){
            $select->condition(db_or()->condition('per.user_name', '%' . db_like($userName) . '%' ,'like')
                    ->condition('per.full_name', '%' . db_like($userName) . '%' ,'like'));
        }
        if(!empty($userName)&& $userName!=t('LBL036'). ' '.t('LBL691') && $userSearchType=='fulltit' ){
            $select->condition('per.full_name', '%' . db_like($userName) . '%', 'LIKE');
        }
        if(($start > 0) || ($end > 0)) {
            $select->range($start,$end);
        }
        
        if(!empty($count)){
            $result = $select->execute()->fetchField();
            return $result;
        }
        if($qry == 1){
            return $select;
        }
        
        expDebug::dPrintDBAPI('list of users in role' ,   $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint(' $result = ' . print_r($result, true) ,3);
        return $result;
        
    }catch(Exception $ex) {
      watchdog_exception('getUserListForGroups', $ex);
      expertusErrorThrow($ex);
   }
    
}


function addPermissionsDetails($callFrom, $entityId, $entityType,$popupId) {
    try {
        expDebug::dPrint('addPermissionsDetails popup id >> '.$popupId,4);
        
        $form_state = array(
        'no_redirect'  => TRUE,
        'build_info'   => array('args' => array('ajax')),
        'entity_id'    => $entityId,
        'entity_type'  => $entityType,
        'form_arg_type' => $argType,
        'call_from'         =>$callFrom,
        'popupId'   => $popupId,
      );
      expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
      $formId                 = ($callFrom == 'view') ? 'view_permissions_addedit_form_html' : 'permissions_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $response       = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($response);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('addPermissionsDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function attachUserToRole($entityId, $entityType){
    try {
        expDebug::dPrint('addUsersToRole'.$entityId,4);
        $form_state = array(
        'no_redirect'  => TRUE,
        'build_info'   => array('args' => array('ajax')),
        'role_id'    => $entityId,
        'entity_type'  => $entityType,
     //   'form_arg_type' => $argType
            'mode'               => 'view'
      );
      expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
      $formId                 = 'display_assigned_users';
      $builtForm              = drupal_build_form($formId, $form_state);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $response       = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($response);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('attachUserToRole', $ex);
    expertusErrorThrow($ex);
  }
}
function ViewUserListForGroup($callFrom, $entityId, $entityType){
    try {
        expDebug::dPrint('addUsersToRole'.$callFrom,4);
        $form_state = array(
                'no_redirect'  => TRUE,
                'build_info'   => array('args' => array('ajax')),
                'role_id'    => $entityId,
                'entity_type'  => $entityType,
                //   'form_arg_type' => $argType
                'mode'               => 'view',
                'call_from'         =>$callFrom
        );
        expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
        $formId                 = 'view_display_assigned_users';
        $builtForm              = drupal_build_form($formId, $form_state);
        $renderedTabContentHTML = drupal_render($builtForm);
        $drupalSettings         = getDrupalSettings();

        $response       = array(
                'drupal_settings'     => $drupalSettings,
                'render_content_main' => $renderedTabContentHTML,
        );

        echo drupal_json_output($response);
        exit;
    } catch (Exception $ex) {
        watchdog_exception('attachUserToRole', $ex);
        expertusErrorThrow($ex);
    }
}

function permissions_addedit_form_html($form,&$form_state){
    try {
      expDebug::dPrint('permission detail : '.print_r($form_state,true), 4);
        global $theme_key;
        $entityId = $form_state['entity_id'];
      $entityType = $form_state['entity_type'];
      $callFrom = $form_state['call_from'];
      $popupId  = $form_state['popupId'];
      $keyType = $entityId.'_'.$entityType;
      $uniqueId = 'permissions-'.$entityId.'-'.$entityType;
      //$formName = 'Capabilities';
      $formName= 'add-permissions';

      //$wrapperId = 'permissions-grid-wrapper-'.$uniqueId;
      //$customHiddenButtonId = 'equv-save-esign';
      expDebug::dPrint('valuess for form state keywords'.$uniqueId,5);
      expDebug::dPrint('$entityId'.$entityId, 4);

      $dynamicwrapperid="permissions-addedit-form".$form_state['entity_id'].$form_state['entity_type'];
      $popuprenderPermId = "popup_container_qtip_permissions_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
      $popupDisplayId                = "qtipPermissionsqtip_visible_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
      $form = permissionsNewAddEditFormWithCancelAndSave($dynamicwrapperid);

      addHiddenValueToAddEditForm($form, 'entity_id', $form_state['entity_id']);
      addHiddenValueToAddEditForm($form, 'entity_type', $form_state['entity_type']);
      addHiddenValueToAddEditForm($form, 'empty_id', $form_state['empty_id']);

      //All available permissionlist
      $permissionsList  = fetchPermissionsList($entityId);
      expDebug::dprint('permission list '.print_r($permissionsList,true),5);
      // Get mapped permissions for the Entity Id and Entity Type
      $permissionMapping = getMappedPermissionMapping($entityId);
      expDebug::dprint('child $permissionMapping '.print_r($permissionMapping,true),5);
      $parentpermMapping = getMappedPermissionMapping($entityId,1);
      expDebug::dprint('parent $permissionMapping'.print_r($parentpermMapping,true),5);

      $form[$formName] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="admin-add-permissions" class="add-permission-class" >',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
        '#suffix' => '</div>',
      );

      $form[$formName]['attr_action'] = array(
          '#type' => 'hidden',
          '#name' => 'attr_action',
          '#value' => 'add_permissions',
      );

      $readOnlyType = $readonly == true ? 'disabled="disabled"' : '';
      expDebug::dPrint('permission list value : '.print_r($permissionsList,true), 4);
      if(!empty($permissionsList)){

         $form[$formName]['permissions_fieldset'] = array(
            '#type' => 'markup',
            //'#prefix' => '<table width="98%" cellpadding="4" cellspacing="0" border="0"><tr class="permission-header-tr"><td colspan="1" class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('All').'</td><td class="certificate-column checkbox-unselected"><input type="checkbox" id = "select-all-checkbox" name="multi-select-checkbox" onclick="checkedAll(this,\''.$uniqueId.'\');checkboxSelectedUnselectedCommon(this);"></td></tr>',//<div id="permissions-fieldset-wrapper" class="admin-tab-list-row">',
                '#prefix' => '<table width="100%" cellpadding="4" cellspacing="0" border="0" id="permission-table"><tr class="permission-header-tr">
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column permission-theader-first">'.t('LBL1005').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL816').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL817').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL063').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL286').'</td>',
            '#suffix' => '</tr></table>',
         );
         $form[$formName]['perm-add-start'] = array(
                '#type' => 'markup',
                '#prefix' => '<div id="admin-add-scroll" class="admin-add-class">',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',

         );
         $form[$formName]['permissions_td_start'] = array(
                '#type' => 'markup',
                '#prefix' =>'<table width="100%" cellpadding="4" cellspacing="0" border="0" id="permission-td-table"><tr class="permission-header-td">',
                '#suffix' => '</tr>',
         );



        $i=0;
        $j=0;
        $k=0;
        $l=0;
        foreach($permissionsList as $key => $val){
                
            // #custom_attribute_0078975 - to bypass the custom attribute module if not enable
          /* if($val->code=='cre_sec_pmn_adm_005_cusattr' && !module_exists('exp_sp_administration_customattribute')){
              continue;
           }*/
            
          $checkedStatus= $checkedstsadd = $checkedstsdelete = $checkedstsedit = '';
          $checkedClass =$checkedclsadd = $checkedclsedit = $checkedclsdelete = 'checkbox-unselected';
          $checkedsubcls = 'checkbox-unselected-readonly';
          $disstyle = ($theme_key=='expertusoneV2')? 'display:none;cursor:default;' : 'cursor:default;';
          $checkedsubattr = '';
          // Already marked permissions should be checked
          if(in_array($val->code, $permissionMapping)){
            $checkedStatus = 'checked';
            $checkedClass = 'checkbox-selected';
            $checkedsubcls = ($theme_key=='expertusoneV2')? 'checkbox-selected-readonly' : 'checkbox-selected';
            $checkedsubattr = 'checked="checked"';
          }
          if(in_array('priv_add-'.$val->code, $permissionMapping)){
            $checkedstsadd = 'checked';
            $checkedclsadd = 'checkbox-selected';
          }
          if(in_array('priv_edit-'.$val->code, $permissionMapping)){
            $checkedstsedit = 'checked';
            $checkedclsedit = 'checkbox-selected';
          }
          if(in_array('priv_delete-'.$val->code, $permissionMapping)){
            $checkedstsdelete = 'checked';
            $checkedclsdelete = 'checkbox-selected';
          }
          //addTwoColumnRowToAddEditForm($form, 'add_permissions_frequencylabel_'.$val->code);
          $class='permission-detail-tr-odd';
          if($j==1){
            $class='permission-detail-tr-even';
            $j=0;
          }else{
            $j=1;
          }
          expDebug::dprint('permission mapping'.$hideshow, 4);
          // Multilanguage Support
          if($val->pname == 'Course/Class'){
            $val->pname = t('Course').'/'.t('Class');
          }else if($val->pname == 'People Setting'){
            $val->pname = t('People').' '.t('LBL1021');
          }else if($val->pname == 'Survey Questions'){
            $val->pname = t('Survey').' '.t('LBL240');
          }else if($val->pname == 'Assessment Questions'){
            $val->pname = t('Assessment').' '.t('LBL240');
          }else if($val->pname == 'Content'){
            $val->pname = t('Content');
          }else if($val->pname == 'Banner'){
            $val->pname = t('LBL623');
          }else if($val->pname == 'Notification'){
            $val->pname = t('LBL204');
          }else if($val->pname == 'Certificate'){
            $val->pname = t('LBL205');
          }else if($val->pname == 'Announcement'){
            $val->pname = t('LBL350');
          }else if($val->pname == 'Custom Attribute' && module_exists('exp_sp_administration_customattribute')){//#custom_attribute_0078975
                $val->pname = t('LBL2015');
          }else if($val->pname == 'Manage Admin API'){
            $val->pname = t('Manage').' '.t('Admin').' '.t('API');
          }else if($val->pname == 'Interactions Studio'){
            $val->pname = t('LBL3021').' '.t('LBL3088');
          }else if($val->pname == 'Video'){
            $val->pname = t('Video');
          }else if($val->pname == 'Presentation'){
            $val->pname = t('LBL3018');
          }else if($val->pname == 'Discounts'){
            $val->pname = t('Discount');
          }else if($val->pname == 'Commerce Setting'){
            $val->pname = t('Commerce').' '.t('LBL1021');
          }else if($val->pname == 'View All Reports'){
            $val->pname = t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
          }else if($val->pname == 'View Assigned Reports'){
            $val->pname = t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
          }else if($val->pname == 'Module'){
            $val->pname = t('LBL1003');
          }else if($val->pname == 'Reports'){
            $val->pname = t('REPORTS');
          }else if($val->pname == 'Configuration'){
            $val->pname = t('LBL1264');
          }
          if(empty($val->parent2_perm)){
            $checkedparent= $checkedparadd = $checkedparedit = $checkedpardel = '';
            $parentchkcls =$parentaddcls = $parentedicls = $parentdelcls = 'checkbox-unselected';
            $childcls= 'checkbox-unselected-readonly';
            $checkedparattr = '';
            // Already marked permissions should be checked
            if(in_array($val->code, $parentpermMapping)){
                $checkedparent = 'checked';
                $parentchkcls = 'checkbox-selected';
                $childcls = ($theme_key=='expertusoneV2')? 'checkbox-selected-readonly' :'checkbox-selected';
                $checkedparattr = 'checked="checked"';
            }
            if(in_array('priv_add-'.$val->code, $parentpermMapping)){
                $checkedparadd = 'checked';
                $parentaddcls = 'checkbox-selected';
            }
            if(in_array('priv_edit-'.$val->code, $parentpermMapping)){
                $checkedparedit = 'checked';
                $parentedicls = 'checkbox-selected';
            }
            if(in_array('priv_delete-'.$val->code, $parentpermMapping)){
                $checkedpardel = 'checked';
                $parentdelcls = 'checkbox-selected';
            }
            $k++;
            $toggle='';
            $oid = '0';
            if(!empty($permissionsList[$i+1]->parent2_perm)){
                $toggle = '<span class="more-text"><a class="show-short-text more-icon-close " onclick="permdetailsShowHide(this,\''.$k.'\',\'parentlevel\');" href="javascript:void(0)">&nbsp;</a></span>';
                $oid = $val->id;
            }
            if($val->pname =='Manage Discussion'){
                $val->pname =t('Manage').' '.t('DISCUSSIONS');
            }else if($val->pname == 'View All Reports'){
                $val->pname =t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'View Assigned Reports'){
                $val->pname =t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
            }
            $markup = '<tr width="100%" class="'.$class.'">
                    <td class="user-list-detail certificate-column">
                        <span class="vtip" title="'.sanitize_data(t($val->pname)).'"> '.titleController('ADMIN-GROUPS-PNAME-PARENT',t($val->pname),45).'</span>'.$toggle.
                    '</td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$parentchkcls.' chk-view">
                            <input type="checkbox" '.$readOnlyType.' id = "group_view_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_parent_permissions'.$uniqueId.'[]" obname="group_view_'.$oid.'" value="'.$val->code.'" '.$checkedparent.'>
                        </div>
                    </td>';
            if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                            <div class="'.$parentaddcls.' chk-create">
                                <input type="checkbox" '.$readOnlyType.' id = "group_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedparadd.'>
                            </div>
                        </td>
                        <td class="user-list-checkbox certificate-column">
                            <div class="'.$parentedicls.' chk-edit">
                                <input type="checkbox" '.$readOnlyType.' id = "group_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedparedit.'>
                            </div>
                        </td>
                        <td class="user-list-checkbox certificate-column">
                            <div class="'.$parentdelcls.' chk-create">
                                <input type="checkbox" '.$readOnlyType.' id = "group_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedpardel.'>
                            </div>
                        </td>';
            }else{
                    $markup .='<td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.' chk-create">
                            <input type="checkbox" disabled="disabled" id = "group_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedparattr.' >
                        </div>
                    </td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.'  chk-edit">
                            <input type="checkbox" disabled="disabled" id = "group_edit_'.$i.'_'.$oid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedparattr.'>
                        </div>
                    </td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.' chk-create">
                            <input type="checkbox" disabled="disabled" id = "group_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedparattr.' >
                        </div>
                    </td>';
            }
            $form[$formName]['permissions_table_row_head'.$i] = array(
                    '#type' => 'markup',
                    '#prefix' => $markup,
                    '#suffix' => '</tr>',
            );
          }else{
            // Multilanguage Support
            if($val->pname == 'Course/Class'){
                $val->pname = t('Course').'/'.t('Class');
            }else if($val->pname == 'People Setting'){
                $val->pname = t('People').' '.t('LBL1021');
            }else if($val->pname == 'Survey Questions'){
                $val->pname = t('Survey').' '.t('LBL240');
            }else if($val->pname == 'Assessment Questions'){
                $val->pname = t('Assessment').' '.t('LBL240');
            }else if($val->pname == 'Content'){
                $val->pname = t('Content');
            }else if($val->pname == 'Banner'){
                $val->pname = t('LBL623');
            }else if($val->pname == 'Notification'){
                $val->pname = t('LBL204');
            }else if($val->pname == 'Certificate'){
                $val->pname = t('LBL205');
            }else if($val->pname == 'Announcement'){
                $val->pname = t('LBL350');
            }else if($val->pname == 'Custom Attribute' && module_exists('exp_sp_administration_customattribute')){//#custom_attribute_0078975
                $val->pname = t('LBL2015');
            }else if($val->pname == 'Manage Admin API'){
                $val->pname = t('Manage').' '.t('Admin').' '.t('API');
            }else if($val->pname == 'Interactions Studio'){
                $val->pname = t('LBL3021').' '.t('LBL3088');
            }else if($val->pname == 'Video'){
                $val->pname = t('Video');
            }else if($val->pname == 'Presentation'){
                $val->pname = t('LBL3018');
            }else if($val->pname == 'Discounts'){
                $val->pname = t('Discount');
            }else if($val->pname == 'Commerce Setting'){
                $val->pname = t('Commerce').' '.t('LBL1021');
            }else if($val->pname == 'View All Reports'){
                $val->pname = t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'View Assigned Reports'){
                $val->pname = t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'Module'){
                $val->pname = t('LBL1003');
            }else if($val->pname == 'Administrator group'){
                $val->pname = t('Administrator').' '.strtolower(t('Group'));
            }else if($val->pname == 'Learner group'){
                $val->pname = t('Learner').' '.strtolower(t('Group'));
            }

          if(!empty($val->sub_parent)){
                  $l++;
                  $toggle='';
                  $pid = $val->id;
                  $toggle = '<span class="more-text"><a class="show-short-text more-icon-close " onclick="permdetailsShowHide(this,\''.$l.'\',\'subparentlevel\');" href="javascript:void(0)">&nbsp;</a></span>';
                  //$oid = $val->id;
            $markup = '<tr width="100%" class="'.$class.' sub-level child'.$k.'" style="display: none;">
                <td class="user-list-detail certificate-column ">
                  <span class="vtip" title="'.sanitize_data(t($val->pname)).'"> '.titleController('ADMIN-GROUPS-PNAME-CHILD',t($val->pname),45).'</span>'.$toggle.'
                </td>
                <td class="user-list-checkbox certificate-column">
                    <div class="'.$checkedClass.' chk-view">
                  <input type="checkbox" '.$readOnlyType.' id = "subgroup_view_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_view_'.$oid.'" onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="subgroup_view_'.$oid.'_'.$pid.'" value="'.$val->code.'" '.$checkedStatus.'>
                    </div>
                </td>';
            if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_create_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_create_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_edit_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_delete_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
                }else{
                    $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_create_'.$i.'_'.$oid.'_'.$pid.'"  class = "attach-permission-cls subgroup_create_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_edit_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subgroup_delete_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                };
              }else if(!empty($val->sub_parent_id)){

                  $markup = '<tr width="100%" class="'.$class.' sub-level sub-sub-level'.$k.' subchild'.$l.'" style="display: none;">
                  <td class="user-list-detail certificate-column ">
                  <span class="vtip" title="'.t($val->pname).'"> '.titleController(' ',t($val->pname),45).'</span>'.$hideshow.'
                  </td>
                  <td class="user-list-checkbox certificate-column">
                  <div class="'.$checkedClass.' chk-view">
                  <input type="checkbox" '.$readOnlyType.' id = "subchild_view_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_view_'.$oid.'" onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="subchild_view_'.$oid.'_'.$pid.'" value="'.$val->code.'" '.$checkedStatus.'>
                  </div>
                  </td>';
                  if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_create_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_create_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_edit_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_delete_'.$oid.'" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
      } else {
                    $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subchild_create_'.$i.'_'.$oid.'_'.$pid.'"  class = "attach-permission-cls subchild_create_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "subchild_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_edit_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subchild_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls subchild_delete_'.$oid.'" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                };
            }else{ 
              $markup = '<tr width="100%" class="'.$class.' sub-level child'.$k.'" style="display: none;">
                  <td class="user-list-detail certificate-column ">
                      <span class="vtip" title="'.sanitize_data(t($val->pname)).'"> '.titleController('ADMIN-GROUPS-PNAME-CHILD',t($val->pname),45).'</span>'.$hideshow.'
                  </td>            
                  <td class="user-list-checkbox certificate-column">
                      <div class="'.$checkedClass.' chk-view">
                          <input type="checkbox" '.$readOnlyType.' id = "sub_view_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="sub_view_'.$oid.'" value="'.$val->code.'" '.$checkedStatus.'>
                      </div>
                  </td>';
                    if($val->parmstat != 1){
                        $markup .= '<td class="user-list-checkbox certificate-column">
                                                    <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
                    }else{
                        $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "sub_create_'.$i.'_'.$oid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "sub_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_edit_'.$oid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "sub_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_delete_'.$oid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                            };
              }
          $form[$formName]['permissions_table_row_child'.$i] = array(
            '#type' => 'markup',
            '#prefix' => $markup,
            '#suffix' => '</tr>',
          ); 
          
          }
          $i++;
          
          //$form['permissions_fieldset']['permissions_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->code);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][0] = array('#type' => 'hidden', '#value' => $val->permission);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][1] = array('#type' => 'hidden', '#value' => $val->parent1_perm);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][2] = array('#type' => 'hidden', '#value' => $val->parent2_perm);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][3] = array('#type' => 'hidden', '#value' => $val->object_code);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][4] = array('#type' => 'hidden', '#value' => $val->parent3_perm);
          //endAddEditTwoColumnRow($form);
          
        }
        
        
        $form[$formName]['permissions_td_end'] = array(
            '#type' => 'markup',
            '#suffix' => '</table>',
         );  
        $form[$formName]['perm-add-end'] = array(
                '#type' => 'markup',
                '#suffix' => '</div>',
        );
        
      } else {
        
          $form[$formName]['no-records-msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="no-records-msg">'.t('There are no permissions available for this Group').'</div>'
          );
      }

        permissionsAddEditFormWithCancelAndSaveClose($form,$dynamicwrapperid,'newtheme-save-button','newtheme-cancel-link',$entityId,$callFrom,false,'',false,$popupId);

      return $form;

  } catch (Exception $ex) {
    watchdog_exception('permissions_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }
}

function permissions_addedit_form_html_submit ($form, &$form_state){
  try {
       $entityId      = $form_state['entity_id'];
       $entityType    = $form_state['entity_type'];
       $uniqueid      = 'permissions-'.$entityId.'-'.$entityType;
       expDebug::dPrint(' $uniqueid '.$uniqueid,4);
       updatePermissionsList($entityId, $entityType, $uniqueid, $form_state);
       populateGroupUsersInFlatTable($entityId,'','DeleteGroup');
       $form_state['rebuild'] = TRUE;
   } catch (Exception $ex) {
        watchdog_exception('permissions_addedit_form_html_submit', $ex);
        expertusErrorThrow($ex);
   }

}

function permissions_addedit_form_html_validate ($form, &$form_state){
    try {
        $group_array  = array();
        $entityId      = $form_state['entity_id'];
        $entityType    = $form_state['entity_type'];
        $uniqueid      = 'permissions-'.$entityId.'-'.$entityType;
        expDebug::dPrint(' permissions_addedit_form_html_validate '.$uniqueid,4);
        $result = prepareGrpPrivilegeUpdate($entityId, $entityType, $uniqueid, $form_state);
        expDebug::dPrint("result array -->>> ".print_r($result,true),5);
        $insArray = $result['insArray'];
        $select = db_select('slt_group_privilege', 'priv');
        $select->addExpression('group_concat(priv.page_code)','page_code');
        $select->addExpression('group_concat(priv.access_page)','access_page');
        $select->condition('priv.group_id', $entityId ,'=');
        $result_set = $select->execute()->fetchAll();
        expDebug::dPrint(" result_set   ". print_r($result_set,true) );
        expDebug::dPrint(" result_set   11111". $result_set[0]->page_code );
        $exist_pagecode = explode(',',$result_set[0]->page_code);
        $exist_access = explode(',',$result_set[0]->access_page);
        expDebug::dPrint("existing access page  ". print_r(($exist_access),true),5 );
        foreach($insArray as $key){
            $group_array[] = $key['page_code'];
            $priv_code[] = $key['access_page'];
        }
        expDebug::dPrint("existing page code  ". print_r($exist_pagecode,true). "<<< group array >>" .print_r($group_array,true),5 );
        $page_diff = array_diff(array_filter($exist_pagecode), array_filter($group_array));
        $priv_diff = array_diff(array_filter($exist_access), array_filter($priv_code));
        expDebug::dPrint("array difference -->> ".print_r($page_diff,true). '<<access diff>> '. print_r($priv_diff,true),5);
        $grp_map_Count = 0;
        if(count($page_diff) > 0){

            if(in_array('cre_sys_obt_crs',$page_diff)){
                array_push($page_diff, "cre_sys_obt_cls");
            }
            if(in_array('cre_sys_obt_trp',$page_diff)){
                array_push($page_diff, "cre_sys_obt_cur", "cre_sys_obt_crt","cre_sys_obt_trn");
            }

            $select = db_select('slt_group_mapping','map');
            $select->addExpression('count(1)','cnt');
            $select->condition('map.group_id', $entityId, '=');
            $select->condition('map.entity_type', $page_diff ,'IN');
            expDebug::dPrintDBAPI('mapping query -->>> ',$select );
            $grp_map_Count =  $select->execute()->fetchField();

            expDebug::dPrint("group map count -->> ".$grp_map_Count );
            if($grp_map_Count > 0){
                $result = t("MSG753");
                form_set_error('',$result);
            }
        }
        $report_arr = array('cre_sec_pmn_nrp','cre_sec_pmn_nrp_001','cre_sec_pmn_nrp_002','cre_sec_pmn_nrp_003');
        if(count(array_intersect($report_arr, $priv_diff)) > 0){
            $select = db_select('slt_report_details','rep');
            $select->addExpression('count(1)','cnt');
            $select->where('(FIND_IN_SET('.$entityId.',rep.report_visibility)>0)');
            $select->condition('rep.status','cre_rpt_rps_del','!=');
            expDebug::dPrintDBAPI('report query -->>> ',$select);
            $rep_arr_cnt =  $select->execute()->fetchField();

            expDebug::dPrint("report map count -->> ".$rep_arr_cnt );
            if($rep_arr_cnt > 0){
                $result = t("MSG753");
                form_set_error('',$result);
            }
        }

    } catch (Exception $ex) {
        watchdog_exception('permissions_addedit_form_html_submit', $ex);
        expertusErrorThrow($ex);
    }

}

/*
 * permissionsNewAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom
 */
function permissionsNewAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
 try {
      $form = array();
      $form['#prefix'] = /* (stripos($_SERVER['HTTP_USER_AGENT'], 'chrome') !== false
                                            || (stripos($_SERVER['HTTP_USER_AGENT'], 'firefox') !== false)? '</form>' : '' ) */'</form>'  .'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
      $form['#suffix'] = '</div>';

      // Add a class to the form so that we can apply custom style (margin, padding, etc).
      $form['#attributes'] = array('class' => array('addedit-form'));
      $form['#attributes']['autocomplete'] = 'off';
      $form['#tree'] = TRUE; // Show full path of values (hierarchical values)

      $form['addedit-left-col-start'] = array(
        '#type' => 'markup',
        '#markup' => '<div>',
      );


      return $form;
  } catch (Exception $ex) {
    watchdog_exception('permissionsNewAddEditFormWithCancelAndSave', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * permissionsAddEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave()
 */
function permissionsAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = '', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $entityId = "", $callFrom, $expandPopup = FALSE, $saveTabIndex = '', $esignature=false,$popupId='') {
    try {
        global $theme_key;
        $customHiddenButtonId = 'esign-button-'.$wrapperId;

      // Remove the temporary storage for API's to exchange information during form creation
      unset($form['addedit_tmp_storage']);

      if (isset($saveTabIndex) && $saveTabIndex > 0) {
        $saveAttributes['tabindex'] = $saveTabIndex;
        $cancelAttributes['tabindex'] = $saveTabIndex+1;
      }

    //  $ajaxCallbackCancel = 'addEditRepaintForm';
      $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.


      // Using this class to applying bg color to the save button
      $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';

      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-bubble-close';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      // Also used by multiselect to show input as selected for only the current form
      $cancelAttributes['data-wrapperid'] = array($wrapperId);

      // A common class for all addedit form cancel button, for styling to appear as a link.
      $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
      $cancelAttributes['class'][] = 'tags-close-button';
      $cancelAttributes['onclick'] = "closeQtip('','$entityId');  return false; ";
      //'$("#'.$popuprenderCertsId.'").closest(".qtip-active").hide(); $(\'#'.$popupDisplayId.'_disp\').html(\'\'); return false;';

      if($callFrom=='edit'){
        $aClass = 'button-margin-edit';
      }else{
        $aClass = 'button-margin-search';
      }
      $setcls = ($theme_key=='expertusoneV2')?'':'resize-save_btn';
      $form[$cancelButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div id="permission-cancel-save-btn" class="'.$aClass.' permission-button-size addedit-form-cancel-and-certificate-done-actions-row certificate-btn-container">' .
                    '<div class="addedit-form-cancel-container-actions '.$setcls.'"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
        '#value' => t('LBL123'),
        '#attributes' => $cancelAttributes,
      );
      //expDebug::dPrint(' yogaraja checked here---> ' . print_r($popupId, true) , 4);
      // Add the Save button
     // if(strpos($popupId ,"search_permissionqtip") > 0 || strpos($popupId ,"qtip_permissions_disp") !== false)
        $ajaxCallbackSave = 'attachPrivilegeCallBack';

      $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

      $saveAttributes['class'][] = 'admin-save-button-middle-bg';

      //Increase the width of the popup while clicking on save button
     /* if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
      $saveAttributes['onclick'][] = 'increasePopWidth();';
      }*/

      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
      $saveAttributes['data-wrapperid'] = array($wrapperId);
      $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
        '#suffix' =>
                   '<div class="admin-save-button-right-bg"></div></div>' .
                   '<div class="clearBoth"></div></div>' .
                 '<div class="clearBoth"></div></div>',
        '#value' =>  t('LBL569'),
            '#name' => 'save',
        '#attributes' => $saveAttributes,
          '#ajax' => array(
              'wrapper' => $wrapperId,
              'callback' => $ajaxCallbackSave,
            'triggerEvent' => 'hidden-save',
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

      $form['addedit-left-col-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="clearBoth"></div>',
      );
  } catch (Exception $ex) {
    watchdog_exception('permissionsAddEditFormWithCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}
function attachPrivilegeCallBack($form, &$form_state){
    try {
        $statusMsgHTML = theme('status_messages');
        if(!empty($statusMsgHTML)) {
            $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
        }
        expDebug::dPrint("Pagination final call result by Priya - ".print_r($commands,true),4);
        return array(
                '#type' => 'ajax',
                '#commands' => $commands
        );
        
    } catch (Exception $ex) {
        watchdog_exception('attachCourseCallBack', $ex);
        expertusErrorThrow($ex);
    }
}

function fetchPermissionsList($permId){
    try {
        expDebug::dPrint('fetch permission',4);
        
        // #custom_attribute_0078975 - to bypass the custom attribute module if not enable
        $custattr_mod="";
        if(!module_exists('exp_sp_administration_customattribute')){            
            $custattr_mod=',\'cre_sec_pmn_adm_005_cusattr\'';
        }
        
        $select = db_query("SELECT
          lst.id id,
          lst.code code,
          lst.name pname,
          lst.attr3 object_code,
          lst.attr4 permission,
          lst2.attr4 parent1_perm,
          lst3.attr4 parent2_perm,
          lst4.attr4 parent3_perm,
          lst.stats3 parmstat,
          lst.stats2 sub_parent,
          lst.stats2_desc sub_parent_id
        from
          slt_profile_list_items lst
          left join slt_profile_list_items lst2 on lst2.id = lst.parent_id and lst2.code like 'cre_sec_pmn_%'
          left join slt_profile_list_items lst3 on lst3.id = lst2.parent_id and lst3.code like 'cre_sec_pmn_%'
          left join slt_profile_list_items lst4 ON lst4.id = lst.stats2_desc AND lst4.code LIKE 'cre_sec_pmn_%'
        where
           lst.code not in('cre_sec_pmn_adm','cre_sec_pmn_rep' $custattr_mod)
            and lst.code like 'cre_sec_pmn_%'
            and lst.is_active = 'Y'
        order by lst.code,lst.stats4"
        );
        expDebug::dPrintDBAPI('list of permissions in role' ,   $select);
        $result = $select->fetchAll();
      return $result;
  }catch(Exception $ex){
    watchdog_exception('fetchPermissionsList', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * updatePermissionsList() - Updated the PermissionList for the Entity Id and Entity Type
 */
function updatePermissionsList($entityId, $entityType, $uniqueid, $form_state){
    try{
    $loggedUserId = getSltpersonUserId();
    // Audit Entry
    //$result1=getpermissiondetails($entityId);
    //expDebug::dPrint('5454545'.print_r($result1[0]->permli,true));
    //expDebug::dPrint('5454545'.print_r($resul1t[0]->acces,true));
    //$existpermissionlist[0]->permissionlist = $result1[0]->permli;
    //$existpermissionlist[0]->accesspermission = $result1[0]->acces;
    $loggedUserId = getSltpersonUserId();
    $AuditEntityType = 'cre_sec_pmn_adm_002_003';
    $result = prepareGrpPrivilegeUpdate($entityId, $entityType, $uniqueid, $form_state);
    $prmArray = $result['prmArray'];
    $insArray = $result['insArray'];
    $privArray = $result['privArray'];
    
    // Redefine permission array if single array contain two or more permissions
    $tmp = implode(',',$prmArray);
    $prmArray = explode(',',$tmp);
      expDebug::dPrint("Final Drupal permissions array for insert ---> ".print_r($prmArray,true),4);
      $diff = array();
      $priv_count = count($privArray);
      
      $ins_count = count($insArray);
      $diff1 = array();
      foreach($privArray as $key5=>$val5){
        if (!isset($insArray[$key5])){
            if(is_array($val5)) {
                //$diff1[$key5]=  array_keys($val5,1);
                if($val5['priv_add'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Removed', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_add for '.$key5) );
                }
                if($val5['priv_edit'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Removed', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_edit for '.$key5) );
                }
                if($val5['priv_delete'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Removed', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_delete for '.$key5) );
                }
                make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Removed', $AuditEntityType, 'exp_sp_administration_groups',
                        'Updatedpermissionlist',array('access_page for '.$key5) );
            }
        }
      }
      if($ins_count > 0){
        foreach ($insArray as $key => $value){
            if (isset($privArray[$key])){
                    $diff[$key] =  array_diff_assoc($value, $privArray[$key]);
            }
            else {
                if($value['priv_add'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Added', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_add for '.$key) );
                }
                if($value['priv_edit'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Added', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_edit for '.$key) );
                }
                if($value['priv_delete'] == 1){
                    make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Added', $AuditEntityType, 'exp_sp_administration_groups',
                            'Updatedpermissionlist',array('priv_delete for '.$key) );
                }
                make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Added', $AuditEntityType, 'exp_sp_administration_groups',
                        'Updatedpermissionlist',array('access_page for '.$key) );
            
            }
        }
      }
      if(count($diff) > 0) {
        foreach($diff as $key1=>$val1) {
          foreach( (array_values($val1)) as $dk=>$dv)   {
            if($dv == 1){
            make_audit_trail_entries('1', $loggedUserId , null ,$entityId, 'Added', $AuditEntityType, 'exp_sp_administration_groups',
                    'Updatedpermissionlist',array_keys($val1.'for'.$key1) );
           }
           if($dv == 0){
            make_audit_trail_entries('0', $loggedUserId , null ,$entityId, 'Removed', $AuditEntityType, 'exp_sp_administration_groups',
                    'Updatedpermissionlist',array_keys($val1.'for'.$key1) );
           }
         }
      }
    }
     // start database operations
    try{
        $role_id =getDrupalRoleId($entityId);
        $txn = db_transaction();

        // Delete previous entries
        $deletestmt = db_delete('slt_group_privilege');
        $deletestmt->condition('group_id',$entityId);
        expDebug::dPrintDBAPI('delete privilege ' , $deletestmt);
        $deletestmt->execute();

        // Insert new permissions in privilege table
        if(!empty($insArray)){
            foreach($insArray as $key){
                $insertstmt = db_insert('slt_group_privilege');
                $fields = array_merge($key,array(
                        'created_by'        => $loggedUserId,
                        'created_on'        => now(),
                        'updated_by'        => $loggedUserId,
                        'updated_on'        => now()
                ));
                $insertstmt->fields($fields);
                expDebug::dPrintDBAPI('list of parent permissions mapping in role', $insertstmt);
                $insertstmt->execute();
            }
        }

        if(!empty($prmArray)){
            // update groups tables with prmission list
            $select = db_update('slt_groups');
            $select->fields(array('access_permissions'=>implode(',',$prmArray)));
                $select->condition('id', $entityId, '=');
            expDebug::dPrintDBAPI('list of permissions access in role', $select);
            $select->execute();

            // Delete drupal previous permissions
            db_query('DELETE FROM {role_permission} WHERE rid = :rid', array(':rid' => $role_id));

            // Insert new premissions in Drupal table
            foreach($prmArray as $permission){
          expDebug::dPrint("Permission list Starts :".$permission,4);
          $result  = db_insert('role_permission');
          $result->fields(array('rid' => $role_id, 'permission' => trim($permission)));
          expDebug::dPrintDBAPI('Insert Permission ' , $result);
          $result->execute();
        }
        // To reset stored active session details (implemented for performance)
        // resetStoredSessionsForActiveUsers();
        }
        drupal_set_message(t('LBL272').' '.t('MSG600').'.');
        unset($txn);
    }catch(Exception $e){
        $txn->rollback();
        unset($txn);
        throw new Exception($e);
    }
        //Audit trail entry
        //$result=getpermissiondetails($entityId);
        //expDebug::dPrint('5454545'.print_r($result[0]->permli,true));
        //expDebug::dPrint('5454545'.print_r($result[0]->acces,true));
        //$updatedPermissionList['permissionlist'] = $result[0]->permli ;
        //$updatedPermissionList['accesspermission'] = $result[0]->acces ;
    //  make_audit_trail_entries($updatedPermissionList, getIdOfLoggedInUser() , $existpermissionlist ,$entityId, 'Updated', $entityType, 'exp_sp_administration_groups',
     //                                          'Updatedpermissionlist', array('permissionlist','accesspermission'));

  } catch (Exception $ex) {
    watchdog_exception('updatePermissionsList', $ex);
    throw $ex;
  }
}

/*
 * function to prepare privilege code for update
*
* */
function prepareGrpPrivilegeUpdate($entityId, $entityType, $uniqueid, $form_state){
     
    // Child Permission Insert and Update
    $AvailablePermList = $form_state['input']['permissions_fieldset']['permissions_hidden'];
    expDebug::dPrint(' $AvailablePermList '.print_r($form_state['input']['permissions_fieldset']['permissions_hidden'],true), 4);
    $PermissionSelected = $form_state['input']['attach_add_permissions'.$uniqueid];//mode_permissions
    expDebug::dPrint('$PermissionSelected '.print_r($PermissionSelected,true), 4);
    $PermissionMode = $form_state['input']['mode_permissions'.$uniqueid];//mode_permissions
    expDebug::dPrint(' $$PermissionMode '.print_r($PermissionMode,true), 4);

    //Parent Permission
    $parent_sele = $form_state['input']['attach_add_parent_permissions'.$uniqueid];//mode_permissions
    $parent_perm = $form_state['input']['mode_parent_permissions'.$uniqueid];//mode_permissions
    expDebug::dPrint('$parent_sele '.print_r($parent_sele,true).' $parent_perm '.print_r($parent_perm,true),4);
    $sel = db_select('slt_group_privilege', 'sp');
    $sel->addField('sp','group_id', 'group_id');
    $sel->addField('sp','access_page', 'access_page');
    $sel->addField('sp','priv_add', 'priv_add');
    $sel->addField('sp','priv_edit', 'priv_edit');
    $sel->addField('sp','priv_delete', 'priv_delete');
    $sel->addField('sp','page_code', 'page_code');
    $sel->addField('sp','is_parent', 'is_parent');
    $sel->condition('sp.group_id', $entityId,'=');
    $selData = $sel->execute()->fetchAll();     
    //$result=getpermissiondetails($entityId);
    //$selData1 = array($selData);
    $privArray = array();
    foreach($selData as $key=>$val) {
        $privArray[$val->access_page] = (array)$val;    
    }     
    // Prepare insert array for privilege table
    $insArray = array();
    $prmArray = array();
    // Parent premissions
    foreach($parent_sele as $key){
        $insArray[$key]=array();
        $insArray[$key]['group_id']=$entityId;
        $insArray[$key]['access_page']=$key;
        $insArray[$key]['priv_add']=in_array('priv_add-'.$key,$parent_perm)?1:0;
        $insArray[$key]['priv_edit']=in_array('priv_edit-'.$key,$parent_perm)?1:0;
        $insArray[$key]['priv_delete']=in_array('priv_delete-'.$key,$parent_perm)?1:0;
        $insArray[$key]['page_code']=null;
        $insArray[$key]['is_parent']=1;
        $prmArray[] = $AvailablePermList[$key][2];
        $prmArray[] = $AvailablePermList[$key][1];
        $prmArray[] = $AvailablePermList[$key][0];
    }
    // Child level permissions
    foreach($PermissionSelected as $key){
        $insArray[$key]=array();
        $insArray[$key]['group_id']=$entityId;
        $insArray[$key]['access_page']=$key;
    //68360: Need to provide add,edit and delete privilege when user select view privilege for order and discount.
    //0068210: Admin can edit the discount even if edit button is disabled for the following scenario - revered the 'cme_dis' condition - this will addressed in later
        if($AvailablePermList[$key][3]=='cme_pmt' || $AvailablePermList[$key][3]=='cme_dis' 
                || $key == 'cre_sec_pmn_adm_cme_003' ||$AvailablePermList[$key][3]=='cre_ntn' ||$AvailablePermList[$key][3]=='cre_cer' || $AvailablePermList[$key][3]=='cre_sys_obt_loc'){
            $insArray[$key]['priv_add']=1;
            $insArray[$key]['priv_edit']=1;
            $insArray[$key]['priv_delete']=1;
        }else{
            $insArray[$key]['priv_add']=in_array('priv_add-'.$key,$PermissionMode)?1:0;
            $insArray[$key]['priv_edit']=in_array('priv_edit-'.$key,$PermissionMode)?1:0;
            $insArray[$key]['priv_delete']=in_array('priv_delete-'.$key,$PermissionMode)?1:0;
        }
        $insArray[$key]['page_code']=$AvailablePermList[$key][3];
        $insArray[$key]['is_parent']=null;
        $prmArray[] = $AvailablePermList[$key][2];
        $prmArray[] = $AvailablePermList[$key][1];
        $prmArray[] = $AvailablePermList[$key][0];
    }
    /*-- #38628 - Disable administer menu when manage API permission only selected --*/
    /*$manageApiPerm = 'cre_sec_pmn_adm_005_api';
    $arrayLen = count($PermissionSelected);
    if(in_array($manageApiPerm, $PermissionSelected) && $arrayLen == 1) {
     $insArray = array(end($insArray));
     $prmArray = array(end($prmArray));
    }*/
    
    /*-- 0054043: Manager Group users Click and seeing the Administer Tab is blank --*/
    
    $manageApiPerm = 'cre_sec_pmn_adm_005_api';
    $admin_perm1 = 'SmartPortal Admin Perm, Administration Perm';
    $admin_perm2 = 'Manage Admin Perm';
   
    $count=array_count_values($prmArray);
    if($count[$admin_perm1] == 1 && in_array($manageApiPerm, $PermissionSelected)){
        $key = array_search($admin_perm1, $prmArray);
        if (false !== $key) {
            unset($prmArray[$key]);
    }
        $key = array_search($admin_perm2, $prmArray);
        if (false !== $key) {
            unset($prmArray[$key]);
        }
    }
        expDebug::dPrint("Prepared permissions array for insert ---> ".print_r($insArray,true),4);
    expDebug::dPrint("Prepared Drupal permissions array for insert ---> ".print_r($prmArray,true),4);
    // Make unique permissions array
    $prmArray = array_unique($prmArray);
    // Unset if any empty array
    $a = array_search('',$prmArray,true);
    if(gettype($a) == 'integer')
        unset($prmArray[$a]);
     
    $result = array();
    $result['privArray'] = $privArray;
    $result['insArray'] = $insArray;
    $result['prmArray'] = $prmArray;
    return $result;
}
/*
 * getMappedPermissionMapping() - Get Mapped Permission for the Group Id
 */
function getMappedPermissionMapping($entityId,$parent =''){
        try {
            $select = db_select('slt_groups','grp');
            $select->leftJoin('slt_group_privilege','priv','grp.id = priv.group_id');
            $select->addField('priv','access_page','code');
            $select->addExpression('if(priv.priv_add = 1,concat(\'priv_add\',\'-\',priv.access_page),\'\')', 'padd');
            $select->addExpression('if(priv.priv_edit = 1,concat(\'priv_edit\',\'-\',priv.access_page),\'\')', 'pedit');
            $select->addExpression('if(priv.priv_delete = 1,concat(\'priv_delete\',\'-\',priv.access_page),\'\')', 'pdelete');
            $select->condition('grp.id', $entityId, '=');
            //Condition to take parent code or not
            if(!empty($parent))
                $select->condition('priv.is_parent', '1', '=');
            else
                $select->condition('priv.is_parent','','IS NULL');

            expDebug::dPrintDBAPI('getMappedPermissionMapping', $select);
            $result = $select->execute()->fetchAll();
             expDebug::dPrint('$$result '.print_r($result,true),5);
             foreach($result as $key=>$val){
                $view_code[]=$val->code;
                if(!empty($val->padd))
                    $view_code[]= $val->padd;
                if(!empty($val->pedit))
                    $view_code[]= $val->pedit;
                if(!empty($val->pdelete))
                    $view_code[]= $val->pdelete;
             }
            expDebug::dPrint('$$result '.print_r($view_code,true),5);
            return $view_code;
        }catch(Exception $ex) {
            watchdog_exception('getMappedPermissionMapping', $ex);
            expertusErrorThrow($ex);
        }
}

/**
 * returns the list of users (group members) matched for the search string from the given $grpId
 * @param unknown $grpId
 * @param unknown $modId
 */
function getUsernameAutocompletesearch($grpId,$modId){
    $userName = $_GET['z'];
    $searchType = $_GET['search_type'];
    
    $data = explode('/',$_REQUEST['q']);
    $entityId = $data[4];
    expDebug::dPrint("entityId value ".print_r($entityId,true), 4 );
    try {
         $grpCode = getGrpCode($grpId);
         $isadm = adminGrporNot($grpId);
         $userId = array(1,2);
         
         //Solr Auto Complete Implementation
         $srcSource = getConfigValue('solr_search');
         expDebug::dPrint("solr off/on :  ".print_r($srcSource,true), 4 );
         if($srcSource == 0){ 
          $userId = array(1,2);
          
          if($modId=="listusers"){
    //   if($isadm == 0 ){
            $select = db_select('slt_person','per');
            $select->innerjoin('slt_group_user_mapping','gmap','gmap.user_id = per.id');
            $select->innerjoin('users','usr','usr.name = per.user_name');
            $select->condition('per.id',$userId,'not in');
            $select->condition('per.status','cre_usr_sts_atv','=');
            $select->condition('gmap.group_id',$grpId,'=');
            $select->condition('gmap.user_type','R','!=');
            if(!empty($count)){
                $select->addExpression('count(distinct per.id)', 'cnt');
            }else{
                $select->addField('per','id','user_id');
                $select->addField('usr','uid','uid');
                $select->addField('usr','status','status');
                if(empty($fetchId)){
                    $select->addField('per','user_name','uname');
                    $select->addField('per','full_name','fname');
                }
            }
          }else{
            $select = db_select('slt_person','per');
            $select->innerjoin('users','usr','usr.name = per.user_name');
            $select->condition('per.status','cre_usr_sts_atv','=');
            if(!empty($count)){
                $select->addExpression('count(distinct per.id)', 'cnt');
            }else{
                $select->addField('per','id','user_id');
                $select->addField('usr','uid','uid');
                $select->addField('usr','status','status');
                if(empty($fetchId)){
                    $select->addField('per','user_name','uname');
                    $select->addField('per','full_name','fname');
                }
            }
            
            
            
            $userId = array(1,2);
            $select1 = db_select('slt_person','persn');
            $select1->innerjoin('slt_group_user_mapping','gmap','gmap.user_id = persn.id');
            $select1->innerjoin('users','usr','usr.name = persn.user_name');
            $select1->condition('persn.id',$userId,'not in');
            $select1->condition('persn.status','cre_usr_sts_atv','=');
            $select1->condition('gmap.group_id',$grpId,'=');
            $select1->condition('gmap.user_type','R','!=');
            $select1->addField('persn','id','user_id');
            $select1->where('persn.id = per.id');
            $select->notExists($select1);
          }
       // $select->where('owner.owner_id is null');
       filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per'); // #46933 - group access visiblity check
       
      if(!empty($userName)&& $userName!=t('LBL036'). ' '.t('LBL691') ){
       if($searchType == 'fultit' || $searchType =='fultitle')
            $select->condition('per.full_name', '%' . db_like($userName) . '%', 'LIKE');
       else
            $select->condition('per.user_name', '%' . db_like($userName) . '%', 'LIKE');
      }
      
      
            expDebug::dPrintDBAPI('getUsernameAutocompletesearch', $select);
            $matchingUserNames = $select->execute()->fetchAll();
            expDebug::dPrint('matchingUserNames names'.print_r($matchingUserNames,true),4);
         }    
        
    else{
             require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
             try {
                 expDebug::dPrint("Solr checker", 1);
                 $solrObj = new UserSolrSearch();
                 if($modId == 'addusers'){
                      $solrList = $solrObj->getSolrSearchResultGroupAddUsersAutoComplete($searchType,$entityId);
                      expDebug::dPrint('Result set from solr'.print_r($solrList,true),5);
                  }
                 else{
                     $solrList = $solrObj->getSolrSearchResultGroupAddedUsersAutoComplete($searchType,$entityId);
                     expDebug::dPrint('Result set from solr'.print_r($solrList,true),5);
                 } 
                 $matchingUserNames = $solrList['records'];
                 expDebug::dPrint('Result set from solr auto complete'.print_r($matchingUserNames,true),5);
                 $count = $solrList['recCount'];
                 expDebug::dPrint('Count of Result set from solr Auto Complete'.print_r($count,true),5);
                 // $start = getOffsetOfFirstRecordInPage($page,$total_pages, $limit, $countquery);
             } catch (Exception $e) {
                 expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
                  
             }
         }
         expDebug::dPrint('searchType type down'.print_r($searchType,true),4);
        foreach ($matchingUserNames as $userName) {
            if($searchType == 'fultit' || $searchType == 'fultitle')
                print $userName->fname . "\n";
            else
                print $userName->uname . "\n";
            }
   }catch (Exception $ex) {
        watchdog_exception('getUsernameAutocompletesearch', $ex);
        expertusErrorThrow($ex);
    }
    }

function loadCtoolDataGridTypeForRole($mode, $type, $searchKeyword, $entityId, $entityType, $excludedId){
    try {
      $page      = $_POST['page'];
      $limit     = $_POST['rows'];
      $sidxVal   = $_POST['sidx'];
      $sord      = $_POST['sord'];
      $tab       = $_POST['tab'];
      $uniqueId  =  "$type-$entityId-$entityType";

      /*
       * 0045967: Exporting user list in the groups screen not showing correctly
       * we need to store storing order in session for further exporting puropose
       */
//    if($type == 'RoleDisplayUsers'){
      $_SESSION['fetchUserListToGroup_sorting_order'] = $sord;
      // $search_type = $_POST['searhType'];
      $queryRequired = 1;
      $fetchFields = array();

      $mode = empty($mode) ? 'view' : $mode;
      $countAvailable = 0;
      $count = 0;
      $searchKeyword= str_replace('|','/',$searchKeyword);

      // unneccessary Checking Mode both are same action is happening.
      /* if($mode == 'view'){

        $listquery = fetchUserListToGroup($entityId,$searchKeyword);
      }else{
        $listquery = fetchUserListToGroup($entityId,$searchKeyword);
      } */
        if(!empty($sidxVal)){
            $sidx = escape_string($sidxVal);
        }
        
        // Solr Implementation
        $srcSource = getConfigValue('solr_search');
        expDebug::dPrint("solr off/on :  ".print_r($srcSource,true), 4 );
        $checkStatus = array ('new','inprogress');
        $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$entityId);
        expDebug::dPrint("background job >>> ". $back_gd_running);
        if($srcSource == 0 || $srcSource == 1){
            $listquery = ($back_gd_running == 0) ? fetchUserListToGroup($entityId,$searchKeyword) : getUserListForGroups($entityId, 0, 0,'usrtit',$searchKeyword,'',1);
      if($sidx !=''){
        if($sidx == "Status"){
            $listquery->orderBy("pli.name",$sord);
        }else if($sidx == "user_name" || $sidx == "full_name") {
            $listquery->orderBy("per.".$sidx,$sord);
        }
      }else{
        $listquery->orderBy("per.full_name",$sord);
        }

      if($countAvailable == 0){
        if($back_gd_running == 0){
            $Userquery = fetchUserListToGroup($entityId,$searchKeyword,'count');
            $count = $Userquery->execute()->fetchField();
        }else{
            $count = getUserListForGroups($entityId, 0, 0,'usrtit',$searchKeyword,1);
        }
        //$count = count($listquery->execute()->fetchAll());
       }
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
      /* Paging Related */
      $start = $limit * $page - $limit;
      $listquery->range($start, $limit);
      // Execute query
      //$queryResult = $listquery->execute()->fetchAll();
      //0041780: Loader icon still loadin when we user removed from group
      if($count>0){
        $queryResult = $listquery->execute()->fetchAll();
    }  
    }
    
    else{
    require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
            try {
                  $solrObj = new UserSolrSearch();
                  $solrList = $solrObj->getSolrSearchResultGroupAddedUsers($entityId);
                  $queryResult = $solrList['records'];
                  $count = $solrList['recCount'];
                  $total_pages  = ceil($count/$limit);
            } catch (Exception $e) {
                expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
            
            }
    } 
      $srch_result_response_roster->total   = $total_pages;
      $srch_result_response_roster->records = $count;
      $srch_result_response_roster->page    = $page;
      $srch_result_response_roster->unique_id    = $uniqueId;
      $srch_result_response_roster->msg    = '';

      $_SESSION['jq_count'] = array();
      $i=0;

    // $userStatistics = fetchUserDetailList($listquery);
    if($count>0){
        foreach ($queryResult as $rec_search_results){
            $rec_search_results-> status = ($rec_search_results-> status == 'Suspended') ? 'Suspend': $rec_search_results-> status;
          $srch_result_response_roster->rows[$i]['id']   = $i;
          $srch_result_response_roster->rows[$i]['cell'] = array(
             'id'                 => $rec_search_results -> user_id,
             'user_name'            => $rec_search_results-> uname,
             'full_name'            => $rec_search_results-> fname,
             'Status'             => t($rec_search_results-> status),
             'deleteAddedUser'    => $rec_search_results-> user_id,
          );
          $i++;
}

    }else{
        $srch_result_response_roster->msg  = "No Records Found";
    }  
     /* }else{  
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
      $_SESSION['fetchUserListToGroup_sorting_order'] = $sord;

        $search_type = $_POST['searhType'];
      $queryRequired = 1;
      $fetchFields = array();

      $mode = empty($mode) ? 'view' : $mode;
      $countAvailable = 0;
      $count = 0;
      $searchKeyword= str_replace('|','/',$searchKeyword);

      // unneccessary Checking Mode both are same action is happening.
//    if($mode == 'view'){

//      $listquery = fetchUserListToGroup($entityId,$searchKeyword);
//    }else{
//      $listquery = fetchUserListToGroup($entityId,$searchKeyword);
//    }
        if(!empty($sidxVal)){
            $sidx = mysql_real_escape_string($sidxVal);
        }

        $listquery = fetchGrpOwnerUserList($entityId,$searchKeyword,$type,'',$search_type);
      if($sidx !=''){
        if($sidx == "Status"){
            $listquery->orderBy("pli.name",$sord);
        }else if($sidx == "user_name" || $sidx == "full_name") {
            $listquery->orderBy("per.".$sidx,$sord);
        }
      }else{
        $listquery->orderBy("per.full_name",$sord);
      }

      if($countAvailable == 0){
            //$listquery = fetchGrpOwnerUserList($entityId,$searchKeyword,$type);
            $Userquery = fetchGrpOwnerUserList($entityId,$searchKeyword,$type,'count',$search_type);
        $count = $Userquery->execute()->fetchField();
        //$count = count($listquery->execute()->fetchAll());
      }
      if( $count >0 ){
        $total_pages = ceil($count/$limit);
      }else{
        $total_pages = 0;
      }
        expDebug::dPrint('total pages '. $total_pages . '$pages '. $page, 5);
      if($page > $total_pages)
        $page = $total_pages;
      if($start < 0) {
        $start = 0;
      }
      // Paging Related
      $start = $limit * $page - $limit;
      $listquery->range($start, $limit);
      // Execute query
      //$queryResult = $listquery->execute()->fetchAll();
      //0041780: Loader icon still loadin when we user removed from group
      if($count>0){
        $queryResult = $listquery->execute()->fetchAll();
      }
        expDebug::dPrintDBAPI("list Query--->",$listquery);
      $srch_result_response_roster->total   = $total_pages;
      $srch_result_response_roster->records = $count;
      $srch_result_response_roster->page    = $page;
      $srch_result_response_roster->unique_id    = $uniqueId;
      $srch_result_response_roster->msg    = '';

      $_SESSION['jq_count'] = array();
      $i=0;

    // $userStatistics = fetchUserDetailList($listquery);
    if($count>0){
        foreach ($queryResult as $rec_search_results){
            $rec_search_results-> status = ($rec_search_results-> status == 'Suspended') ? 'Suspend': $rec_search_results-> status;
          $srch_result_response_roster->rows[$i]['id']   = $i;
          $srch_result_response_roster->rows[$i]['cell'] = array(
             'id'                 => $rec_search_results -> user_id,
             'user_name'            => $rec_search_results-> uname,
             'full_name'            => $rec_search_results-> fname,
                        'name'             => $rec_search_results-> gname,
             'deleteAddedUser'    => $rec_search_results-> user_id,
          );
          $i++;
        }

    }else{
        $srch_result_response_roster->msg  = "No Records Found";
    }
         
      }*/
    expDebug::dPrint("ROLE USER LIST ::: ".print_r($srch_result_response_roster,true),5);
        echo drupal_json_encode($srch_result_response_roster);
    } catch (Exception $ex) {
        watchdog_exception('loadCtoolDataGridTypeForRole', $ex);
        expertusErrorThrow($ex);
    }
}
/**
 * returns list of users who are members of the given $grpId (based on the attributes match)
 * for default Administers group, the attributes match will not be considered
 * @param unknown $grpId
 * @param unknown $searchKeyword
 * @param string $field
 * @return SelectQuery
 */
function fetchUserListToGroup($grpId,$searchKeyword,$field = ''){
    try {
        
        
        $searchType = $_POST['searhType'];
        expDebug::dPrint("ROLE USER checckkkk ".print_r($searchType,true),5);
    
         //$grpCode = getGrpCode($grpId);
         //$isadm = adminGrporNot($grpId);
         $grpDetails = getGrpDetails($grpId,
                        array('code',
                            'is_admin',
                            'org_id',
                            'user_type',
                            'employment_type',
                            'country',
                            'state',
                            'department',
                            'job_role',
                            'language',
                            'is_manager',
                            'is_instructor'
                        ));
         $grpCode=$grpDetails['code'];
         $isadm = $grpDetails['is_admin'];
         // Get hire date details
         $hireDetails = getHireDetails($grpId);
         
       $userId = array(1,2);
     //  if($isadm == 0 ){
        $select = db_select('slt_person','per');
        $select->innerjoin('slt_group_user_mapping','gmap','gmap.user_id = per.id');
        $select->join('slt_profile_list_items','pli','per.status=pli.code');
        $select->condition('per.id',$userId,'not in');
        $select->condition('per.status','cre_usr_sts_del','!=');
        $select->condition('gmap.group_id',$grpId,'=');
        $select->condition('gmap.user_type','R','!=');
       if(empty($field)){
           $select->addField('per','id','user_id');
           $select->addField('per','user_name','uname');
           $select->addField('pli','name','status');
           $select->addField('per','full_name','fname');
         }else if($field == 'fullname'){
         $select->addField('per','full_name','fname');
         }else{
             $select->addExpression('count(distinct per.id)', 'cnt');
         }
         if((!empty($searchKeyword)) && ($searchKeyword != 'undefined') && $searchKeyword !=t('LBL181') && $searchKeyword !=t('Type Full Name')){
            if($searchType=='usrtit' || $searchType=='usrtitle'){
                $select->condition('per.user_name', '%' . db_like($searchKeyword) . '%' ,'like');
            }else{
                $select->condition('per.full_name', '%' . db_like($searchKeyword) . '%' ,'like');
            }
         }
         
         
        
        expDebug::dPrintDBAPI('list of users in role' ,   $select);
      /*    if(!empty($count)){
            //$result = $select->execute()->fetchField();
            return $select;
        } */
       //   $result = $select->execute()->fetchAll();
       
       
        //  expDebug::dPrint(' $result 122222= ' . print_r($result, true) ,3);
        return $select;
    //   }
       
       
      
       } catch (Exception $ex) {
        watchdog_exception('fetchUserListToGroup', $ex);
        expertusErrorThrow($ex);
       }
}

/* Function not in use - #0046888
 * function getpermissiondetails($entityId){
    try {
        $select = db_select('slt_security_role','role');
        $select->condition('id', $entityId, '=');
        $select->addField('role','permissionlist','permli');
        $select->addField('role','accesspermission','acces');
        expDebug::dPrintDBAPI('getpermissiondetails', $select);
        $result = $select->execute()->fetchAll();
        expDebug::dPrint('getpermissiondetails' .   print_r($result,true),5);
        return $result;
        } catch (Exception $ex) {
            watchdog_exception('getpermissiondetails', $ex);
            expertusErrorThrow($ex);
        }
}*/

function roleNameDuplicatecheck($role_name,$roleId) {
    try {
      $select = db_select ('slt_groups');
      $select->condition('id',$roleId,'!=');
      $select->condition('name',$role_name,'=');
      $select->condition('status','cre_sec_sts_del','!=');
      $select->addExpression('count(1)');
      expDebug::dPrintDBAPI('roleNameDuplicatecheck' , $select);
      $result = $select->execute()->fetchField();
        return $result;
    } catch (Exception $ex) {
        watchdog_exception('roleNameDuplicatecheck', $ex);
        expertusErrorThrow($ex);
    }
}

function getRoleUsereName($userlist){
    if(empty($userlist)){
    return '';
  }
  $userlistArray = explode(',',$userlist);
  try{
      $i=0;
      foreach($userlistArray as $key){
        $select = db_select('slt_person', 'per');
        $select->addField('per','user_name', 'uname');
        $select->condition('id', $key , '=');
        expDebug::dPrintDBAPI('$userlistArray' , $select);
        $userlistArray[$i]=$select->execute()->fetchField();
        $i++;
      }
      $userlistval = implode(', ',$userlistArray);
      expDebug::dPrint('getRoleUsereName' .   print_r($userlistval,true),4);
      return $userlistval;
  }catch (Exception $ex) {
    watchdog_exception('getRoleUsereName', $ex);
    expertusErrorThrow($ex);
  }
}

function getGroupAttributes(){
    $select = db_select('slt_site_configuration','grpconf');
    $select->addField('grpconf','id');
    $select->addField('grpconf','code');
    $select->addField('grpconf','name');
    $select->condition('grpconf.enabled','1','=');
    expDebug::dPrintDBAPI('Selected groups  ' , $select);
    return $select->execute()->fetchAll();
}

function getAttributeList($attr,$id){
    $result=array();
    switch($attr){
        case "ste_con_org" :
            $result = getOrganizations($id);
            echo drupal_json_output($result);
            break;
        case "ste_con_dep" :
            $result = getProfileItems($id,'department','cre_usr_dpt_');
            echo drupal_json_output($result);
            break;
        case "ste_con_lng" :
            $result = getProfileItems($id,'language','cre_sys_lng_');
            echo drupal_json_output($result);
            break;
        case "ste_con_ety" :
            $result = getProfileItems($id,'employment_type','cre_usr_etp_');
            echo drupal_json_output($result);
            break;
        case "ste_con_jbr" :
            $result = getProfileItems($id,'job_role','cre_usr_jrl_');
            echo drupal_json_output($result);
            break;
        case "ste_con_cnt" :
            $result = getCountryState($id,'country');
            echo drupal_json_output($result);
            break;
        case "ste_con_ste" :
            $result = getCountryState($id,'state');
            echo drupal_json_output($result);
            break;
        case "ste_con_usr" :
            $result = getProfileItems($id,'user_type','cre_usr_ptp_');
            echo drupal_json_output($result);
            break;
        case "ste_con_rol" :
            $result = getRoleSelected($id);
            echo drupal_json_output($result);
            break;
        case "ste_con_hdt" :
            $result = getHireDateSelected($id);
            echo drupal_json_output($result);
            break;          
        default :
            break;
    }
    //return $result;
}

function getAttributeListByText($gid='',$attr,$gaddedt=''){
    switch($attr){
        case "ste_con_org" :
            return getOrganizationByText($gid);
            break;
        case "ste_con_dep" :
            return getProfileItemsByText('cre_usr_dpt_','department',$gid,$gaddedt);
            break;
        case "ste_con_lng" :
            return getProfileItemsByText('cre_sys_lng_','language',$gid,$gaddedt);
            break;
        case "ste_con_ety" :
            return getProfileItemsByText('cre_usr_etp_','employment_type',$gid,$gaddedt);
            break;
        case "ste_con_jbr" :
            return getProfileItemsByText('cre_usr_jrl_','job_role',$gid,$gaddedt);
            break;
        case "ste_con_cnt" :
            return getCountryStateByText('country',$gid);
            break;
        case "ste_con_ste" :
            return getCountryStateByText('state',$gid);
            break;
        case "ste_con_usr" :
            return getProfileItemsByText('cre_usr_ptp_','user_type',$gid);
            break;
        case "ste_con_rol" :
            return getRoleSelected($gid,1);
        case "ste_con_hdt" :
            return getHireDateSelected($gid,1);
            break;
        default :
            break;
    }
}

function pageRange($count,$limit,&$page,&$start){
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
}

function getOrganizations($id){
    $searchText =  $_GET['z'];
    $page = $_GET['page'];
    $limit = $_GET['rows'];
    $start = $limit * $page - $limit;

    $select = db_select('slt_organization', 'org');
    $select->leftJoin('slt_groups','grp','(grp.org_id=\'All\' or find_in_set(org.id,grp.org_id)>0) and grp.id=:gid',array('gid'=>$id));
    $select->addField('org', 'id','opt');
    $select->addField('org', 'name','val');
    $select->addExpression('if(grp.org_id=\'All\' or grp.id is not null,1,0)','sel');
    $select->condition('org.status', 'cre_org_sts_act');
    filterByUserAccessPrivileges($select, 'org','',1,'cre_org','','org');

    if($searchText != '' && $searchText != null)
        $select->condition('org.name', '%' . db_like($searchText) . '%', 'LIKE');

    $select->orderBy('sel','desc');
    $select->orderBy('val');
//  expDebug::dPrintDBAPI("Access getOrganizationLst countQuery ",$select->countQuery());
    $count = $select->countQuery()->execute()->fetchField();
    pageRange($count,$limit,$page,$start);
    $select->range($start,$limit);
    expDebug::dPrintDBAPI("Access getOrganizationLst Query ",$select);
    $organization = $select->execute()->fetchAll();
    $output = array();
    $output['records'] = $count ;
    $output['page'] = $page ;
    $output['data'] = $organization;
    return $output;
}

function getOrganizationByText($gId=''){
    $searchText =  $_GET['z'];
  // Select the table
  $select = db_select('slt_organization', 'org');

  // Add fields to fetch
  $select->addField('org', 'name', 'name');

  // Add condition
  if($gId===''){
    $select->condition('org.name', '%' . db_like($searchText) . '%', 'LIKE');
  }else{
    //To get default selected value for edit screen
    $select->leftJoin('slt_groups','grp','(grp.org_id=\'All\' or find_in_set(org.id,grp.org_id)>0) and grp.id=:gid',array('gid'=>$gId));
    $select->addField('org', 'id', 'code');
    $select->addExpression('if(grp.org_id=\'All\',1,0)','allsel');
    $select->condition('grp.id',$gId,'=');
  }
  $select->condition('org.status','cre_org_sts_act');
  filterByUserAccessPrivileges($select, 'org','',1,'cre_org','','org');
  // Order in dictionary sort order
  $select->orderBy('name');
  if(isset($_GET['limit']) && !empty($_GET['limit'])) {
    $select->range(null, $_GET['limit']);
  }
  expDebug::dPrintDBAPI(' $select obj = vince' , $select);
  $matchingorgNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingRoleNames = '. print_r($matchingorgNames, true) , 4);

  if($gId!==''){
    /*// Get total org count
    $select = db_select('slt_organization', 'org');
    $select->addExpression('count(1)','recCnt');
    $select->condition('org.status','cre_org_sts_act');
    $rst = $select->execute()->fetchField();
    expDebug::dPrintDBAPI(' $select count ' , $select);
    return array('recCnt'=>$rst,'result'=>$matchingorgNames);*/
    return $matchingorgNames;
  }else{
      foreach ($matchingorgNames as $orgName) {
        print $orgName->name . "\n";
      }
  }
}

function getProfileItems($id,$field,$code){
    $searchText =  $_GET['z'];
    $page = $_GET['page'];
    $limit = $_GET['rows'];
    $start = $limit * $page - $limit;

    // Getting total records count
    $selCnt = db_select('slt_profile_list_items', 'pl');
    $selCnt->leftJoin('slt_groups','grp','(grp.'.$field.'=\'All\' or find_in_set(pl.code,grp.'.$field.')>0) and grp.id=:gid',array('gid'=>$id));
    $selCnt->addExpression('COUNT(1)', 'cnt');
    $selCnt->condition('pl.code', '%' . db_like($code) . '%', 'LIKE');
    $selCnt->condition('pl.is_active', 'Y');
    filterByUserAccessPrivileges($selCnt, 'pl','',1,substr($code, 0, -1),'','pl');
    if($searchText != '' && $searchText != null)
        $selCnt->condition('pl.name', '%' . db_like($searchText) . '%', 'LIKE');

    expDebug::dPrintDBAPI(" get the count of records ",$selCnt);
    $count = $selCnt->execute()->fetchField();

    // Getting the record details
    $select = db_select('slt_profile_list_items', 'pl');
    $select->leftJoin('slt_groups','grp','(grp.'.$field.'=\'All\' or find_in_set(pl.code,grp.'.$field.')>0) and grp.id=:gid',array('gid'=>$id));
    $select->addField('pl', 'code','opt');
    $select->addField('pl', 'name','val');
    $select->addExpression('if(grp.'.$field.'=\'All\' or grp.id is not null,1,0)','sel');
    $select->condition('pl.code', '%' . db_like($code) . '%', 'LIKE');
    $select->condition('pl.is_active', 'Y');
    filterByUserAccessPrivileges($select, 'pl','',1,substr($code, 0, -1),'','pl');

    if($searchText != '' && $searchText != null)
        $select->condition('pl.name', '%' . db_like($searchText) . '%', 'LIKE');

    $select->orderBy('sel','desc');
    $select->orderBy('val');

    pageRange($count,$limit,$page,$start);
    $select->range($start,$limit);
    expDebug::dPrintDBAPI("Access getOrganizationLst Query ",$select);
    $organization = $select->execute()->fetchAll();
    expDebug::dPrint('autocomplete enter'.$searchText);
    $output = array();
    $output['records'] = $count ;
    $output['page'] = $page ;
    $output['data'] = $organization;
    return $output;
}

function getProfileItemsByText($code,$type,$gid,$gaddedt=''){
    $searchText =  $_GET['z'];
  // Select the table
  $select = profileListTextQuery($code,$type,$gid,$searchText='');

  if(!empty($gaddedt)){
        $select->range(0,1);
  }
  expDebug::dPrintDBAPI(' $select obj for groups' , $select);

  $matchingorgNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingRoleNames = '. print_r($matchingorgNames, true).'qqwertyyyuuu'.$gaddedt , 3);

  if(!empty($gaddedt) && $matchingorgNames[0]->allsel == 0){
    $selectAll = profileListTextQuery($code,$type,$gid);
    expDebug::dPrintDBAPI("Query for All selected options in groups",$selectAll);
    $matchingorgNames = $selectAll->execute()->fetchAll();
    expDebug::dPrint("Result for All selected options in groups".print_r($matchingorgNames,1),3);
  }

  if($gid!==''){
    /*// Get total record count
    $select = db_select('slt_profile_list_items', 'pl');
    $select->addExpression('count(1)','recCnt');
    $select->condition('pl.code', '%' . db_like($code) . '%', 'LIKE');
    $select->condition('pl.is_active', 'Y');
    expDebug::dPrintDBAPI(' $select count ' , $select);
    $rst = $select->execute()->fetchField();
    return array('recCnt'=>$rst,'result'=>$matchingorgNames);*/
    return $matchingorgNames;
  }else{
      foreach ($matchingorgNames as $orgName) {
        print $orgName->name . "\n";
      }
  }
}

function getCountryState($id,$type){
    $searchText =  $_GET['z'];
    $page = $_GET['page'];
    $limit = $_GET['rows'];
    $opt = isset($_GET['country'])?urldecode($_GET['country']):'';
    $start = $limit * $page - $limit;
    expDebug::dPrint("Selected Country --> ".$opt);
    $fld1 = ($type=='country')?'country_code':'state_code';
    $fld2 = ($type=='country')?'country_name':'state_name';
    $fld3 = ($type=='country')?'country':'state';
    $tbl = ($type=='country')?'slt_country':'slt_state';

    $select = db_select($tbl, 'cont');
    if($type=='country')
        $select->leftJoin('slt_groups','grp','(grp.'.$fld3.'=\'All\' or find_in_set(cont.country_code,grp.country)>0) and grp.id=:gid',array('gid'=>$id));
    else
        $select->leftJoin('slt_groups','grp','(grp.'.$fld3.'=\'All\' or find_in_set(concat(cont.country_code,\'-\',cont.state_code),grp.state)>0) and grp.id=:gid',array('gid'=>$id));

    if($type=='country')
        $select->addField('cont', $fld1,'opt');
    else
            $select->addExpression('concat(cont.country_code,\'-\',cont.state_code)', 'opt');

    $select->addField('cont', $fld2,'val');
    $select->addExpression('if(grp.'.$fld3.'=\'All\' or grp.id is not null,1,0)','sel');

    if($searchText != '' && $searchText != null)
        $select->condition('cont.'.$fld2, '%' . db_like($searchText) . '%', 'LIKE');
    $country = explode(',',$opt);
  if($type =='state' && in_array('All',$country) != true)
    $select->condition('cont.country_code', $country, 'IN');

  $select->groupBy('val,cont.country_code');
    $select->orderBy('sel','desc');
    $select->orderBy('val');
    $count = count($select->execute()->fetchAll());
    pageRange($count,$limit,$page,$start);
    $select->range($start,$limit);
    expDebug::dPrintDBAPI("Qry ",$select);
    $res = $select->execute()->fetchAll();

    $output = array();
    $output['records'] = $count ;
    $output['page'] = $page ;
    $output['data'] = $res;
    return $output;
}

function getCountryStateByText($type,$gId=''){
    $searchText =  $_GET['z'];
    $opt = isset($_GET['country'])?urldecode($_GET['country']):'';
    $fld1 = ($type=='country')?'country_code':'state_code';
    $fld2 = ($type=='country')?'country_name':'state_name';
    $fld3 = ($type=='country')?'country':'state';
    $tbl = ($type=='country')?'slt_country':'slt_state';
  // Select the table
  $select = db_select($tbl, 'cont');

  // Add fields to fetch
  $select->addField('cont', $fld2, 'name');
  $country = explode(',',$opt);
  // Add condition
  if($type == 'state' && in_array('All',$country) != true){
    if($opt!=''){
        $select->condition('cont.country_code', explode(',',$opt), 'IN');
    }else if($gId!==''){
            $select->where('grp.country=\'All\' or find_in_set(cont.country_code,grp.country)>0');
    }
  }
  // Order in dictionary sort order
  if($gId!==''){
    if($type=='country')
        $select->addField('cont', $fld1, 'code');
    else
        $select->addExpression('concat(cont.country_code,\'-\',cont.state_code)', 'code');

    $select->addExpression('if(grp.'.$fld3.'=\'All\',1,0)','allsel');
    if($type=='country')
        $select->leftJoin('slt_groups','grp','(grp.'.$fld3.'=\'All\' or find_in_set(cont.country_code,grp.country)>0) and grp.id=:gid',array('gid'=>$gId));
    else
        $select->leftJoin('slt_groups','grp','(grp.'.$fld3.'=\'All\' or find_in_set(concat(cont.country_code,\'-\',cont.state_code),grp.state)>0) and grp.id=:gid',array('gid'=>$gId));

    $select->condition('grp.id',$gId,'=');
  }else{
    $select->condition('cont.'.$fld2, '%' . db_like($searchText) . '%', 'LIKE');
  }
  $select->orderBy('name');
    $select->groupBy('cont.'.$fld2);
  expDebug::dPrintDBAPI(' $select obj = vince' , $select);

  $matchingorgNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingRoleNames = '. print_r($matchingorgNames, true) , 4);

    if($gId!==''){
        // Get total record count
        /*$select = db_select($tbl, 'cont');
        if($type == 'state'){
            $select->where('grp.country=\'All\' or find_in_set(cont.country_code,grp.country)>0');
            $select->leftJoin('slt_groups','grp','(find_in_set(concat(cont.country_code,\'-\',cont.state_code),grp.state)>0) and grp.id=:gid',array('gid'=>$gId));
        }
        $select->addExpression('count(1)','recCnt');
        expDebug::dPrintDBAPI(' $select count ' , $select);
        $rst = $select->execute()->fetchField();
    return array('recCnt'=>$rst,'result'=>$matchingorgNames);*/
        return $matchingorgNames;
    }else{
      foreach ($matchingorgNames as $orgName) {
        print $orgName->name . "\n";
      }
  }
}

function getRoleSelected($gId,$flg=0){
    $select = db_select('slt_groups', 'grp');
    $select->addExpression('ifnull(grp.is_manager,\'N\')','manager');
    $select->addExpression('ifnull(grp.is_instructor,\'N\')','instructor');
    $select->condition('grp.id',$gId,'=');
    expDebug::dPrintDBAPI(' $select obj = vince' , $select);

  $rtn = $select->execute()->fetchAll();
  if($flg===0){
      expDebug::dPrint(' $matchingRoleNames = '. print_r($rtn, true) , 3);
      $result = array(
                    (object) array('opt'=>'ins','val'=>'Instructor','sel'=>($rtn[0]->instructor == 'Y')? 1: 0),
                    (object) array('opt'=>'mgr','val'=>'Manager','sel'=>($rtn[0]->manager == 'Y')? 1: 0)    //for 45491: Manager role selected, but doesn't shown in Group Edit popup
            );
      $output = array();
        $output['records'] = 2 ;
        $output['page'] = 1 ;
        $output['data'] = $result;
        return $output;
  }else{
    $ins = ($rtn[0]->instructor=='Y')?array('code'=>'ins','name'=>'Instructor'):'';
    $mgr = ($rtn[0]->manager=='Y')?array('code'=>'mgr','name'=>'Manager'):'';
    $out = (!is_array($ins)?(!is_array($mgr)?array():array((object)$mgr))
                :(!is_array($mgr)?array((object)$ins):array((object)$ins,(object)$mgr)));
    expDebug::dPrint("OUTPUT ".print_r($out,true),5);
    return $out;
  }
}


function getHireDateSelected($gId, $flg=0){
    $select = db_select('slt_groups', 'grp');
    $select->leftjoin('slt_group_attributes','attr','attr.group_id = grp.id');
    $select->addField('attr', 'on_or_after_start_date', 'on_or_after_start_date');
    $select->addField('attr', 'on_or_before_start_date', 'on_or_before_start_date');
    $select->addField('attr', 'between_start_date', 'between_start_date');
    $select->addField('attr', 'between_end_date', 'between_end_date');
    $select->condition('grp.id',$gId,'=');
    $select->condition('attr.attr_type','HireDate','=');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);

    $rtn = $select->execute()->fetchAll();
    expDebug::dPrint("result record >> ". print_r($rtn,1),5);
    /*$hire_search = $rtn[0]->hire_search_mode;
    $hire_start_gtn  = ($rtn[0]->hire_start != null && $hire_search == 'ste_con_hdt_gtn') ? date('m-d-Y', strtotime($rtn[0]->hire_start)) : '';
    $hire_start_ltn  = ($rtn[0]->hire_start != null && $hire_search == 'ste_con_hdt_ltn') ? date('m-d-Y', strtotime($rtn[0]->hire_start)) : '';
    $hire_start_btw  = ($rtn[0]->hire_start != null && $hire_search == 'ste_con_hdt_btw') ? date('m-d-Y', strtotime($rtn[0]->hire_start)) : '';
    $hire_end_btw    = ($rtn[0]->hire_end != null) ? date('m-d-Y', strtotime($rtn[0]->hire_end)) : '';
    */
    if($flg===0){
        $attributes = 'data-datePickerDatesDisplayOption = "all" data-emptyDateFieldText = "'.t('LBL112').'" '; // data-datePickerTooltip = ""
        expDebug::dPrint(' getHireDateSelected $matchingRoleNames = '. var_export($rtn, true) , 3);
        //$mode = '';
        //foreach($rtn as $val){
            //$mode .= empty($mode)?$val->hire_search_mode : ','.$val->hire_search_mode;
            if(!empty($rtn[0]->on_or_after_start_date)){
                $hire_start_gtn = date('m-d-Y', strtotime($rtn[0]->on_or_after_start_date));
            }
            if(!empty($rtn[0]->on_or_before_start_date)){
                $hire_start_ltn = date('m-d-Y', strtotime($rtn[0]->on_or_before_start_date)); 
        }
            if(!empty($rtn[0]->between_start_date)){
                $hire_start_btw = date('m-d-Y', strtotime($rtn[0]->between_start_date)); 
                $hire_end_btw  = date('m-d-Y', strtotime($rtn[0]->between_end_date));
            }
        //}
        //$hire_search = explode(",",$mode);
        $result = array(
                (object) array(
                        'opt' =>'ste_con_hdt_gtn',
                        'val' =>t('LBL1136'),
                        'sel' => (!empty($rtn[0]->on_or_after_start_date))? 1: 0,
                        'ui_data' => '<input id="hire_start_ste_con_hdt_gtn" type="text" data-datePickerDatesDisplayOption = "hiredate" data-emptyDateFieldText = "'.t('LBL112').'"  value="'.$hire_start_gtn.'"  maxlength="10" size="10" name="hire_start_ste_con_hdt_gtn" onchange="updHireDate(this);" class="ghire_start addedit-edit-datefield" readonly>'                        
                ),
                (object) array(
                        'opt' =>'ste_con_hdt_ltn',
                        'val' => t('LBL1138'),
                        'sel' => (!empty($rtn[0]->on_or_before_start_date))? 1: 0,
                        'ui_data' => '<input id="hire_start_ste_con_hdt_ltn" type="text" data-datePickerDatesDisplayOption = "hiredate" data-emptyDateFieldText = "'.t('LBL112').'"  value="'.$hire_start_ltn.'"  maxlength="10" size="10" name="hire_start_ste_con_hdt_ltn" onchange="updHireDate(this);" class="ghire_start addedit-edit-datefield" readonly></span>'
                ),
                (object) array(
                        'opt' =>'ste_con_hdt_btw',
                        'val' =>t('LBL1144'),
                        'sel' => (!empty($rtn[0]->between_start_date))? 1: 0,
                        'ui_data' => '<input id="hire_start_ste_con_hdt_btw" type="text" data-datePickerDatesDisplayOption = "hiredate" value="'.$hire_start_btw.'" maxlength="10" size="10" name="hire_start_ste_con_hdt_btw" onchange="updHireDate(this);" class="ghire_start addedit-edit-datefield" readonly>'. drupal_strtolower(t('LBL647')).'<input type="text" data-datePickerDatesDisplayOption = "hiredate" data-emptyDateFieldText = "'.t('LBL112').'"  maxlength="10" size="10" value="'.$hire_end_btw.'" id="hire_end_ste_con_hdt_btw" name="hire_end_ste_con_hdt_btw" onchange="updHireDate(this);" class="ghire_end addedit-edit-datefield" readonly>'
                )
                
        );
        
        
        $output = array();
        $output['records'] = 3 ;
        $output['page'] = 1 ;
        $output['data'] = $result;
        return $output;
    }else{
        //$out = array();
        $result = array();
        //foreach($rtn as $val){
        if(count($rtn) > 0){    
            /*$hire_search = $val->hire_search_mode; 
            $start = $val->hire_start; 
            $end = $val->hire_end; 
            $hire_start_btw  = ($start != null) ? date('m-d-Y', strtotime($start)) : '';
            $hire_end_btw    = ($end != null) ? date('m-d-Y', strtotime($end)) : '';*/
            //if ($hire_search != null) {
            $stringValues = array( 'ste_con_hdt_gtn' => 'On or after',  'ste_con_hdt_ltn' => 'On or before',  'ste_con_hdt_btw' => 'Between');
            //expDebug::dPrint('hire search mode value: ' . $hire_search);
            //$dateString = ($hire_search == 'ste_con_hdt_btw') ? $hire_start_btw . ' to ' . $hire_end_btw : $hire_start_btw;
            //$finString = $stringValues[$hire_search] . ' ' . $dateString;
            if(!empty($rtn[0]->on_or_after_start_date)){
                $hire_start  = ($rtn[0]->on_or_after_start_date != null) ? date('m-d-Y', strtotime($rtn[0]->on_or_after_start_date)) : '';
            $result[] = (object) array(
                                'code' => 'ste_con_hdt_gtn',
                                'name' =>  $stringValues['ste_con_hdt_gtn'] . ' ' . $hire_start,
                    );
            }
            if(!empty($rtn[0]->on_or_before_start_date)){
                $hire_start  = ($rtn[0]->on_or_before_start_date != null) ? date('m-d-Y', strtotime($rtn[0]->on_or_before_start_date)) : '';
                $result[] = (object) array(
                                'code' => 'ste_con_hdt_ltn',
                                'name' => $stringValues['ste_con_hdt_ltn'] . ' ' . $hire_start,
                        );
            }
            if(!empty($rtn[0]->between_start_date)){
                $hire_start  = ($rtn[0]->between_start_date != null) ? date('m-d-Y', strtotime($rtn[0]->between_start_date)) : '';
                $hire_end  = ($rtn[0]->between_end_date != null) ? date('m-d-Y', strtotime($rtn[0]->between_end_date)) : '';
                $result[] = (object) array(
                                'code' => 'ste_con_hdt_btw',
                                'name' =>  $stringValues['ste_con_hdt_btw'] .' ' .$hire_start . ' and ' . $hire_end,
                        );
            }
            //} 
            /*else {
            $out = array();
            }*/
        //}
        }
        $out = count($result) > 0 ? $result : array(); 
        expDebug::dPrint("getHireDateSelected OUTPUT ".print_r($out,true),4);
        return $out;
    }
}

function getMandatoryMasterEnrollId($grpId,$usrId){
    try{
        $select = db_select('slt_group_mapping','map');
        //$select->addField('enr','id', 'EnrollId');
        $select->addExpression('GROUP_CONCAT(enr.id)', 'EnrollId');
        //$select->leftjoin('slt_enrollment', 'enr', 'enr.class_id = map.entity_id and map.mro = \'cre_sys_inv_man\'');
        $select->leftjoin('slt_master_enrollment', 'enr', '(if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = enr.program_id AND grp_map.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_obt_trn\'))>0,(enr.program_id = map.entity_id),
                (enr.program_id = map.entity_id))) and map.group_type = 0 and map.mro = \'cre_sys_inv_man\'');
        $select->condition('map.group_id', $grpId, '=');
        $select->condition('enr.user_id', $usrId, '=');
        expDebug::dPrintDBAPI("getremovedusermasterEnrollID ",$select);
        $enrollId =$select->execute()->fetchField();
        expDebug::dPrint("Enrollid Result ".print_r($enrollId,true),5);
        return $enrollId;
    }catch (Exception $ex) {
        watchdog_exception('getMandatoryMasterEnrollId', $ex);
        expertusErrorThrow($ex);
    }
}
function getMandatoryEnrollId($grpId,$usrId){
    try{
        $select = db_select('slt_group_mapping','map');
        //$select->addField('enr','id', 'EnrollId');
        $select->addExpression('GROUP_CONCAT(enr.id)', 'EnrollId');
        //$select->leftjoin('slt_enrollment', 'enr', 'enr.class_id = map.entity_id and map.mro = \'cre_sys_inv_man\'');
        $select->leftjoin('slt_enrollment', 'enr', '(if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = enr.class_id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(enr.class_id = map.entity_id),
(enr.course_id = map.entity_id))) and map.group_type = 0 and map.mro = \'cre_sys_inv_man\'');
        $select->condition('map.group_id', $grpId, '=');
        $select->condition('enr.user_id', $usrId, '=');
        expDebug::dPrintDBAPI("getremoveduserEnrollID ",$select);
        $enrollId =$select->execute()->fetchField();
        expDebug::dPrint("Enrollid Result ".print_r($enrollId,true),5);
        return $enrollId;
    }catch (Exception $ex) {
        watchdog_exception('getMandatoryEnrollId', $ex);
        expertusErrorThrow($ex);
    }
}
function getComplianceEnrollId($grpId,$usrId){
    try{
        $select = db_select('slt_group_mapping','map');
        //$select->addField('enr','id', 'EnrollId');
        $select->addExpression('GROUP_CONCAT(enr.id)', 'EnrollId');
        //$select->leftjoin('slt_enrollment', 'enr', 'enr.class_id = map.entity_id and map.mro = \'cre_sys_inv_man\'');
        $select->leftjoin('slt_enrollment', 'enr', '(if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = enr.class_id AND grp_map.entity_type = \'cre_sys_obt_cls\')>0,(enr.class_id = map.entity_id),
                (enr.course_id = map.entity_id))) and map.group_type = 0');
        $select->leftJoin('slt_course_template','crs','crs.id = enr.course_id');
        $select->condition('map.group_id', $grpId, '=');
        $select->condition('enr.user_id', $usrId, '=');
        $select->condition('crs.is_compliance', '1', '=');
        expDebug::dPrintDBAPI("getremoveduserEnrollID ",$select);
        $enrollId =$select->execute()->fetchField();
        expDebug::dPrint("Enrollid Result ".print_r($enrollId,true),5);
        return $enrollId;
    }catch (Exception $ex) {
        watchdog_exception('getComplianceEnrollId', $ex);
        expertusErrorThrow($ex);
    }
}
function updateRemoveUser($grpId,$usrId, $callByAPI=false){
    try{
        $loggeduserId = getSltpersonUserId();
        $grpcode = getGrpCode($grpId);
        $select = db_select('slt_groups','grp');
        $select->addExpression('count(1)');
        $select->condition('id', $grpId, '=');
        $select->where('FIND_IN_SET('.$usrId.',added_users)>0');
        expDebug::dPrintDBAPI('Fetch role Id ' , $select);
        $addedId =$select->execute()->fetchField();//check whether the user is added_users r not
        $is_admin = adminGrporNot($grpId);
        // Commented for #87900: While saving a group "503 service unavailable error" is showing.
        /* 
        $enrollId = getMandatoryEnrollId($grpId,$usrId);
        if(!empty($enrollId)){
            $Enrollupdate=db_query('UPDATE slt_enrollment SET mandatory=NULL,updated_on=now() where id IN('.$enrollId.')');
            expDebug::dPrintDBAPI('update mandatory field update ',$Enrollupdate);
        }
        $complianceEnrollId = getComplianceEnrollId($grpId,$usrId);
        if(!empty($complianceEnrollId)){
            $ComplainceEnrollupdate=db_query('UPDATE slt_enrollment SET is_compliance=NULL,updated_on=now() where id IN('.$complianceEnrollId.')');
            expDebug::dPrintDBAPI('update Complaince field update ',$ComplainceEnrollupdate);
        }

        $masterEnrollId = getMandatoryMasterEnrollId($grpId,$usrId);
        if(!empty($masterEnrollId)){
            $MasterEnrollupdate=db_query('UPDATE slt_master_enrollment SET mandatory=NULL,updated_on=now() where id IN('.$masterEnrollId.')');
            expDebug::dPrintDBAPI('update mandatory field update for TP ',$MasterEnrollupdate);
        }
         */
        if($is_admin){
            //Entry for securitymanagementAdmin
            $select = db_delete('slt_admin_group_users');
            $select->condition('group_id',$grpId);
            $select->condition('user_id',$usrId);
            expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $select);
            $select->execute();
        }
        
        // Get the fields before deleting
        $select = db_select('slt_group_user_mapping', 'gumap');
        $select->addField('gumap','id','id');
        $select->addField('gumap','group_id','group_id');
        $select->addField('gumap','user_id','user_id');
        $select->addField('gumap','group_type','group_type');
        $select->condition('group_id',$grpId);
        $select->condition('user_id',$usrId);
        $gumapId =$select->execute()->fetchAll();
        //To sync solr
        syncSolrData('User');
        
        
        
        foreach($gumapId as $val){
            if($val->group_type == 1){
                $grp_type = 'Admin Group';
            }
            else if($val->group_type == 0){
                $grp_type = 'Learner Group';
            }
            // Make deleted log entry
            $fields = array(
                    'table_name' => 'slt_group_user_mapping',
                    'entity_id' => $val->id,
                    'entity_type' => 'group_user_mapping',
                    'parent1_entity_id' => $val->group_type,
                    'parent1_entity_type' => $grp_type,
                    'parent2_entity_id' => $val->group_id,
                    'parent2_entity_type' => 'group_id',
                    'parent3_entity_id' => $val->user_id,
                    'parent3_entity_type' => 'user_id',
                    'module_name' => 'exp_sp_administration_groups',
                    'functionality_name' => 'updateRemoveUser',
                    'api_name' => $_REQUEST['apiname'],
                    'deleted_on' => now(),
                    'deleted_by' => $loggeduserId
            );
            deleted_log_entry($fields);
        }
        
        if($addedId > 0 && $grpcode != 'grp_ins' && $grpcode != 'grp_mgr'){ // remove the users from added_user list
                $userDelete=db_query('UPDATE slt_groups SET added_users = if(REPLACE(CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\') = \',\',
                        NULL ,TRIM(BOTH \',\' FROM REPLACE(
                        CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\'))) WHERE id = :grpid',array('userid'=>$usrId,'grpid'=>$grpId));
                expDebug::dPrintDBAPI('User delete query',$userDelete);


                //  $userDelete->execute(); // No need To execute this
                //Insert group_audit to track removed and added users
                $insertStmt = db_insert('slt_group_audit');
                $fields = array(
                        'group_id'        => $grpId,
                        'user_id'         => $usrId,
                        'operation'       => 'removed',
                        'criteria_match'  => 'N',
                        'updated_by'      => $loggeduserId,
                        'updated_on'      => now()
                );
                $insertStmt->fields($fields);
                $insertStmt->execute();
                
                
                //Added by Vincent on 23 Jan, 2017 for #0071187
                $select = db_delete('slt_group_user_mapping');
                $select->condition('group_id',$grpId);
                $select->condition('user_id',$usrId);
                expDebug::dPrintDBAPI('delete userlist slt_group_user_mapping' , $select);
                $select->execute();
                
        }else{ // remove the user from removed_users list
            $loggeduserId = getSltpersonUserId();
            $existingUsersList=getUserListField($grpId,'removed_users');
      $updateUser = db_update('slt_groups');
      $updateUser->condition('id', $grpId, '=');
      $updateUser->fields(array(
                        'removed_users' =>  ((empty($existingUsersList)) ? '' : $existingUsersList . ',') .$usrId,
                        'updated_on'    => now(),
                        'updated_by'    => $loggeduserId
                        ));
      expDebug::dPrintDBAPI('QUERY DEFAULT ', $updateUser);
        $updateUser->execute();
        
        //Update remove users list
        //Added by Vincent on 23 Jan, 2017 for #0071187
        $update = db_update('slt_group_user_mapping');
        $update->condition('group_id',$grpId);
        $update->condition('user_id',$usrId);
        $update->fields(array('user_type' =>'R'));
        expDebug::dPrintDBAPI('delete userlist slt_group_user_mapping' , $update);
        $update->execute();
        
        
        $userDelete=db_query('UPDATE slt_groups SET added_users = if(REPLACE(CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\') = \',\',
                        NULL ,TRIM(BOTH \',\' FROM REPLACE(
                        CONCAT(\',\', added_users, \',\'),CONCAT(\',\', :userid, \',\'), \',\'))) WHERE id = :grpid',array('userid'=>$usrId,'grpid'=>$grpId));
                expDebug::dPrintDBAPI('User delete query',$userDelete);
        //  $userDelete->execute();
                
            //Insert group_audit to track removed and added users
        $insertStmt = db_insert('slt_group_audit');
        $fields = array(
                'group_id'         => $grpId,
                'user_id'          => $usrId,
                'operation'        => 'removed',
                'criteria_match'   => 'Y',
                'updated_by'       => $loggeduserId,
                'updated_on'       => now()
        );
        $insertStmt->fields($fields);
        $insertStmt->execute();
        }
        $is_admin = adminGrporNot($grpId);
        if($is_admin == 1){ //|| $grpcode == 'grp_ins' || $grpcode == 'grp_mgr'){
            //Update users_list in slt_groups
            $userDelete=db_query('UPDATE slt_groups SET userslist = if(REPLACE(CONCAT(\',\', userslist, \',\'),CONCAT(\',\', :userid, \',\'), \',\') = \',\',
                    NULL, TRIM(BOTH \',\' FROM REPLACE(
                    CONCAT(\',\', userslist, \',\'),CONCAT(\',\', :userid, \',\'), \',\'))) WHERE id = :grpid',array('userid'=>$usrId,'grpid'=>$grpId));
            expDebug::dPrintDBAPI('User delete query',$userDelete);
            // $userDelete->execute();
            //Check whether user is a owner of the group
            /* $exist_query = db_select('slt_owner_details');
            $exist_query->addExpression('count(1)');
            $exist_query->condition('group_id', $grpId, '=');
            $exist_query->condition('owner_id',$usrId,'=');
            expDebug::dPrintDBAPI('Users exists in group or not ' , $exist_query);
            $exist_or_not =$exist_query->execute()->fetchField();
            expDebug::dPrint('exists or not check '. $exist_or_not);
            if($exist_or_not == 0 ){ */
            //delete users from drupal users table
            $role_id = getDrupalRoleId($grpId);
            $uId = getDrupalIdOfUser($usrId);
            $deleteuser = db_delete('users_roles');
            $deleteuser->condition('uid',$uId);
            $deleteuser->condition('rid',$role_id);
            expDebug::dPrintDBAPI('delete userlist ' , $deleteuser);
            $deleteuser->execute();

            $fields = array(
                    'table_name' => 'users_roles',
                    'entity_id' => $uId,
                    'entity_type' => 'group_users',
                    'module_name' => 'exp_sp_administration_groups',
                    'functionality_name' => 'updateRemoveUser',
                    'deleted_on' => now(),
                    'deleted_by' => getIdOfLoggedInUser(),
                    'parent1_entity_id' => $role_id,
                    'parent1_entity_type' => 'rid',
                    'parent2_entity_id' => $uId,
                    'parent2_entity_type' => 'uid',
                     
            );
             
            deleted_log_entry($fields);
            // To reset stored active session details (implemented for performance)
      // resetStoredSessionsForActiveUsers();
            // }
        }
        /* //Instructor and Manager Update in person table
        if($grpcode == 'grp_ins' || $grpcode == 'grp_mgr'){
            $colName = ($grpcode == 'grp_ins') ? array('is_instructor' =>'N') : array('is_manager' =>'0') ;
            $updatesmt = db_update('slt_person');
            $updatesmt->condition('id', $usrId, '=');
            $updatesmt->fields($colName);
            expDebug::dPrintDBAPI('QUERY DEFAULT slt_person instructor---> ', $updatesmt);
            $updatesmt->execute();
        } */
        // Select the fields and the values to be inserted

        $entityType = 'cre_sec';
        $updatedUsersList['removed_users'] = getUserListField($grpId,'removed_users');
        make_audit_trail_entries($updatedUsersList, $loggeduserId , $existingUsersList ,$grpId, 'deleteandUpdated', $entityType, 'exp_sp_administration_groups',
                'UpdatedUserlist', array('removed_users'));
        // updated_on is updated so that group mapping in solr collection will get deleted when user is removed from the group.
        $updateStmt = db_update('slt_person');
        $updateStmt->condition('id', $usrId);
        $updateStmt->fields(array('updated_on' => now()));
        expDebug::dPrintDBAPI('$updateStmt for user updated_on sync', $updateStmt);
        $Updatedcode = $updateStmt->execute();
        //To sync solr
        syncSolrData('User');
        syncSolrData('Enrollment');
        syncSolrData('MasterEnrollment');
        if ($callByAPI == true && (!empty($updatedUsersList))) {
          if (!empty($uId) || !empty($addedId))
            return "success" ;
          else
            return "User Name not available in the Group";
        }
        else if ($callByAPI == true && (empty($updatedUsersList)))
          return false;
    } catch (Exception $ex) {
        watchdog_exception('updateRemoveUser', $ex);
        expertusErrorThrow($ex);
    }
}

function getUserListField($grpId,$fieldname) {
    try{
        // Select slt_security_role table
        $select = db_select('slt_groups', 'grps');

        // Get count of removed users
        $select->addField('grps',$fieldname, 'userlist');

        // Set the conditions
        $select->condition('grps.id', $grpId, '=');

        expDebug::dPrintDBAPI('getRemoveUserListField $select obj = ' , $select);

        // Execure the query
        $userList = $select->execute()->fetchField();
        expDebug::dPrint('$userList = ' . $userList,4);

        return $userList;
    } catch (Exception $ex) {
        watchdog_exception('getRoleUserListField', $ex);
        expertusErrorThrow($ex);
    }
}

function removedUserorNot($userId,$grpId){
    try{
        // Select slt_security_role table
        $select = db_select('slt_groups', 'grps');

        // Get count of removed users
        $select->addExpression('count(1)');
        $select->condition('grps.id', $grpId, '=');
        // Set the conditions
        $select->where('find_in_set('.$userId.',removed_users)');

        expDebug::dPrintDBAPI('getRemoveUserListField $select obj = ' , $select);

        // Execure the query
        $count = $select->execute()->fetchField();
        expDebug::dPrint('$userListremovedUserorNot = ' . $userList,4);

        return $count;
    } catch (Exception $ex) {
        watchdog_exception('removedUserorNot', $ex);
        expertusErrorThrow($ex);
    }
}

function sortgrpAttributes($form_state){
    try{
        $groupAttributes = getGroupAttributes();
        $list = array();
        $manager    = 'N';
        $instructor = 'N';
        foreach($groupAttributes as $attr){
            $formVal = isset($form_state['values']['atr_'.$attr->code])?trim($form_state['values']['atr_'.$attr->code]):'';
            if(!empty($formVal) && $attr->code != 'ste_con_rol'){
                $l = explode(',',$formVal);
                asort($l);
                if(in_array('0',$l)) {
                    array_shift($l);
                }
                $formVal =implode(",",$l);
            }else if($attr->code == 'ste_con_rol'){
                // Ram : stripos should be a String and Search Value Fixed For this Ticket #0039080
                if(!empty($formVal)){
                    if(stripos($formVal, 'mgr')!==false)
                        $manager = 'Y';
                    if(stripos($formVal, 'ins')!==false)
                        $instructor ='Y' ;
                }
            }
            $list[$attr->code] = $formVal;
        }
        $fields = array(
                'org_id'                  => isset($list['ste_con_org']) && !empty($list['ste_con_org'])?trim($list['ste_con_org']):null,
                'user_type'             => isset($list['ste_con_usr']) && !empty($list['ste_con_usr'])?trim($list['ste_con_usr']):null,
                'employment_type'   => isset($list['ste_con_ety']) && !empty($list['ste_con_ety'])?trim($list['ste_con_ety']):null,
                'country'                 => isset($list['ste_con_cnt']) && !empty($list['ste_con_cnt'])?trim($list['ste_con_cnt']):null,
                'state'                   => isset($list['ste_con_ste']) && !empty($list['ste_con_ste'])?trim($list['ste_con_ste']):null,
                'department'            => isset($list['ste_con_dep']) && !empty($list['ste_con_dep'])?trim($list['ste_con_dep']):null,
                'job_role'              => isset($list['ste_con_jbr']) && !empty($list['ste_con_jbr'])?trim($list['ste_con_jbr']):null,
                'language'              => isset($list['ste_con_lng']) && !empty($list['ste_con_lng'])?trim($list['ste_con_lng']):null,
                'is_manager'            => $manager,
                'is_instructor'       => $instructor,
                'on_or_after_start_date'       => isset($form_state['values']['hidden_hire_start_ste_con_hdt_gtn']) && !empty($form_state['values']['hidden_hire_start_ste_con_hdt_gtn'])?formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_gtn'])):null,
                'on_or_before_start_date'       => isset($form_state['values']['hidden_hire_start_ste_con_hdt_ltn']) && !empty($form_state['values']['hidden_hire_start_ste_con_hdt_ltn'])?formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_ltn'])):null,
                'between_start_date'       => isset($form_state['values']['hidden_hire_start_ste_con_hdt_btw']) && !empty($form_state['values']['hidden_hire_start_ste_con_hdt_btw'])?formatHireDate(trim($form_state['values']['hidden_hire_start_ste_con_hdt_btw'])):null,
                'between_end_date'       => isset($form_state['values']['hire_end']) && !empty($form_state['values']['hire_end'])?formatHireDate(trim($form_state['values']['hire_end'])):null,
            );
        expDebug::dPrint("sortgrpAttributes array list fields  ".print_r($fields,true), 4);
        return $fields;
    }catch (Exception $ex) {
        watchdog_exception('sortgrpAttributes', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * returns the list of groups whose attributes are similar to the given group id ($roleId)
 * @param unknown $roleId
 * @param unknown $sortedattribute
 * @param unknown $grpType
 * @param string $attrCheck
 * @return number|unknown
 */
function duplicateGrpAttributecheck($roleId,$sortedattribute,$grpType,$attrCheck='') {
    try {
        $select = db_select ('slt_groups','grp');
        $select->leftJoin('slt_group_attributes', 'attr', 'grp.id = attr.group_id');
        
        if(!empty($roleId) && empty($attrCheck))
            $select->condition('grp.id',$roleId,'!=');

        if(!empty($attrCheck))
            $select->condition('grp.id',$roleId,'=');

        if(empty($attrCheck)){ // For checking the attribute values changed or not.
            $select->addExpression('group_concat(grp.name SEPARATOR \', \')','name');
        $select->condition('grp.code',array("grp_adm","grp_sup","grp_mgr","grp_ins"),'NOT IN');
        $select->condition('grp.status','cre_sec_sts_atv','=');
            // Condition added For this Ticket #0040094 -- To Get a Duplicate attribute should check admin Or learner group
            $select->condition('grp.is_admin',($grpType == 'admin')? 1 : 0,'=');
        }
        $select->condition('grp.org_id',$sortedattribute['org_id'],empty($sortedattribute['org_id']) ? 'is null':'=');
        $select->condition('grp.user_type',$sortedattribute['user_type'],empty($sortedattribute['user_type']) ? 'is null':'=');
        $select->condition('grp.employment_type',$sortedattribute['employment_type'],empty($sortedattribute['employment_type']) ? 'is null':'=');
        $select->condition('grp.country',$sortedattribute['country'],empty($sortedattribute['country']) ? 'is null':'=');
        $select->condition('grp.state',$sortedattribute['state'],empty($sortedattribute['state']) ? 'is null':'=');
        $select->condition('grp.department',$sortedattribute['department'],empty($sortedattribute['department']) ? 'is null':'=');
        $select->condition('grp.job_role',$sortedattribute['job_role'],empty($sortedattribute['job_role']) ? 'is null':'=');
        $select->condition('grp.language',$sortedattribute['language'],empty($sortedattribute['language']) ? 'is null':'=');
        $select->condition('attr.on_or_after_start_date',$sortedattribute['on_or_after_start_date'],empty($sortedattribute['on_or_after_start_date']) ? 'is null':'=');
        $select->condition('attr.on_or_before_start_date',$sortedattribute['on_or_before_start_date'],empty($sortedattribute['on_or_before_start_date']) ? 'is null':'=');
        $select->condition('attr.between_start_date',$sortedattribute['between_start_date'],empty($sortedattribute['between_start_date']) ? 'is null':'=');
        $select->condition('attr.between_end_date',$sortedattribute['between_end_date'],empty($sortedattribute['between_end_date']) ? 'is null':'=');
        $select->addExpression('GROUP_CONCAT(DISTINCT(grp.id))', 'dupId');
        expDebug::dPrintDBAPI('duplicateGrpAttributecheck' , $select);

        if(!empty($attrCheck)){
            $result = $select->execute()->fetchField();
            return (!empty($result)) ? 0 : 1;
        }else{
        $result = $select->execute()->fetchAll();
        return $result;
        }

    } catch (Exception $ex) {
        watchdog_exception('duplicateGrpAttributecheck', $ex);
        expertusErrorThrow($ex);
    }
}
/**
 * function to fetch group code.
 */
function getGrpCode($grpId){
    $select = db_select('slt_groups','grp');
    $select->addField('grp','code','code');
    $select->condition('grp.id',$grpId,'=');
    expDebug::dPrintDBAPI('$select ' , $select);
    $grpCode = $select->execute()->fetchField();
    return $grpCode;
}

/**
 * get drupal role_id from group Id.
 */
function getDrupalRoleId($grpId){

    $select = db_select('role','role');
    $select->join('slt_groups','grp','role.name=grp.name');
    $select->addField('role','rid','rid');
    $select->condition('grp.id',$grpId,'=');
    expDebug::dPrintDBAPI('Fetch role Id ' , $select);
    $role_id = $select->execute()->fetchField();
    return $role_id;
}
/**
 * function to insert users in groups and users_roles table.
 */
function grpUserInsert($grpId){
    $txn = db_transaction();
    try{
        //if($grpType == 1){
            //Entry for securitymanagementAdmin
            $role_id = getDrupalRoleId($grpId);
            //Get GroupDetails
            $grp_details= getGrpDetails($grpId,array('code','name','status','is_admin','created_by'));
    
            //get users from list
            $userslist = getUserDetailList($grpId,0,0,'','','',1,1);
            $plist='';
            $val_str = '';
            $user_add_query = 'INSERT INTO slt_admin_group_users ( group_id,group_code,user_id,status, created_by, created_on, updated_by, updated_on) VALUES ';
            $status = $grp_details['status'];
            $val_str .= '('.$grpId.', "'.$grp_details['code'].'", null, "'.$status.'", "'.$grp_details['created_by'].'", now(), '.$grp_details['created_by'].', now() )';
            foreach($userslist as $usrdt){
                $plist .= empty($plist)?$usrdt->user_id:",".$usrdt->user_id;
                $val_str .=  ',('.$grpId.', "'.$grp_details['code'].'", "'.$usrdt->user_id.'", "'.$status.'", "'.$grp_details['created_by'].'", now(), '.$grp_details['created_by'].', now() )';
                $uId = $usrdt->uid;
                $select1 = db_insert('users_roles');
                $select1->fields(array('uid' => $uId, 'rid' => $role_id));
                expDebug::dPrintDBAPI('Insert userlist ' , $select1);
                $select1->execute();
            }
    
            //To track users in admin groups
            $updateStmt = db_update('slt_groups');
            $updateStmt->condition('id', $grpId);
            $updateStmt->fields(array('userslist' => $plist));
            $Updatedcode = $updateStmt->execute();
            //add the user in slt_admin_group_users
            if(!empty($val_str)){
                $insert_qry = $user_add_query.' '.$val_str;
                expDebug::dPrint("INSERT QUERY : ".$insert_qry , 4);
                $insert = db_query($insert_qry);
            }
        //}
        //populateGroupUsersInFlatTable($grpId,'','DeleteGroup');
        unset($txn);
        // To reset stored active session details (implemented for performance)
    // resetStoredSessionsForActiveUsers();
    }catch(Exception $e){
        $txn->rollback();
        unset($txn);
        watchdog_exception('grpUserInsert', $e);
        expertusErrorThrow($e);
    }
}

/*
 * function to remove users from drupal user table.
 * */
function grpUserDelete($grpId,$role_id = ''){
    try{
        $txn = db_transaction();
        
        if(empty($role_id)){
            $role_id = getDrupalRoleId($grpId);
        }
        
        //Remove user from group table
        $updateStmt = db_update('slt_groups');
        $updateStmt->condition('id', $grpId);
        $updateStmt->fields(array('userslist' => null));
        $Updatedcode = $updateStmt->execute();

        
        // make a bulk entry for users_roles deletion
        $user_id = getIdOfLoggedInUser();
        $delete_role = db_select('users_roles', 'u');
        $delete_role->addField('u', 'uid', 'entity_id');
        $delete_role->addField('u', 'uid', 'parent2_entity_id');
        $delete_role->addExpression('"' . 'users_roles' . '"', 'table_name');
        $delete_role->addExpression('"' . 'group_users' . '"', 'entity_type');
        $delete_role->addExpression('"' . 'exp_sp_administration_groups' . '"', 'module_name');
        $delete_role->addExpression('"' . 'grpUserDelete' . '"', 'functionality_name');
        $delete_role->addExpression('"' . now() . '"', 'deleted_on');
        $delete_role->addExpression('"' . $user_id . '"', 'deleted_by');
        $delete_role->addExpression('"' . $role_id . '"', 'parent1_entity_id');
        $delete_role->addExpression('"' . 'rid' . '"', 'parent1_entity_type');
        $delete_role->addExpression('"' . 'uid' . '"', 'parent2_entity_type');
        $delete_role->condition('rid', $role_id, '=');
        expDebug::dPrintDBAPI('select users_rols which are going to be deleted', $delete_role);
        deleted_log_entry_bulk_from_select($delete_role, array('entity_id', 'parent2_entity_id', 'table_name',  'entity_type', 'module_name', 'functionality_name', 'deleted_on', 'deleted_by', 'parent1_entity_id', 'parent1_entity_type',  'parent2_entity_type'));
        
        
        //Entry for securitymanagementAdmin
        $select1 = db_delete('users_roles');
        $select1->condition('rid',$role_id);
        expDebug::dPrintDBAPI('delete userlist ' , $select1);
        $select1->execute();
        
        //Entry for securitymanagementAdmin
        $select = db_delete('slt_admin_group_users');
        $select->condition('group_id',$grpId);
        expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $select);
        $select->execute();
        
        unset($txn);
        // To reset stored active session details (implemented for performance)
    // resetStoredSessionsForActiveUsers();
    }catch(Exception $e){
        $txn->rollback();
        unset($txn);
        throw new Exception($e);
    }
}

function getAtrributeForNarrowSearch($atrr){
    switch($atrr){
        case 'org' :
            getOrganizationAutoComplete();
        break;
        case 'jobrole' :
            getAdminUserJobRoleAutoComplete();
        break;
        case 'location' :
            getAdminUserStateAutoComplete();
        break;
        case 'empltype' :
            getAdminUserEmplTypeAutoComplete();
        break;
        case 'dept' :
            getAdminUserDeptAutoComplete();
        break;
        case 'usertype' :
            getAdminUserTypeAutoComplete();
        break;
        case 'country' :
            getAdminCountryAutoComplete();
        break;
        case 'lang' :
            getAdminLanguageAutoComplete();
        break;
    }
}

function getAdminUserStateAutoComplete() {
    try{
        $searchstate =  $_GET['z'];
        $searchCountrycode = $_GET['country'];
        $searchCountrycode = explode('|',$searchCountrycode);
        //select query for state based on country
        $selectState = db_select('slt_country', 'sc');
        $selectState->leftjoin('slt_state', 'st', 'st.country_code = sc.country_code');
        $selectState->distinct();
        $selectState->addField('st', 'state_name', 'name');
        $selectState->condition('sc.country_code', $searchCountrycode , 'IN');
        $selectState->condition('st.state_name', '%' . db_like($searchstate) . '%', 'LIKE');
        expDebug::dPrintDBAPI(' $selectState OBJ = ' , $selectState);
        $matchedResultsState = $selectState->execute()->fetchAll();
        //Add 'All' in front of fetching query
        array_unshift($matchedResultsState,(object) array('name' => 'All'));
        foreach ( $matchedResultsState as $key => $resName) {
            print $resName->name. "\n";
        }
    } catch (Exception $ex) {
        watchdog_exception('getAdminUserStateAutoComplete', $ex);
        expertusErrorThrow($ex);
    }
}

function getAllStaticLanguageGrp(){

    //fetch language from List Values
    try{
        $select = db_select('slt_profile_list_items','splt');
        $select->addField('splt','code', 'code');
        $select->addField('splt','name', 'name');
        $select->condition('splt.code', db_like('cre_sys_lng_') . '%', 'LIKE');
        $select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));
        $select->condition(db_and()->condition('splt.is_active','Y','='));
        $select->orderBy('name','asc');
        expDebug::dPrintDBAPI('$select to fetch languages', $select);
        $result = $select->execute()->fetchAllKeyed();
        expDebug::dPrint(' Language ' . print_r($result,true),5);
        return $result;
    }catch (Exception $ex) {
        watchdog_exception('getAllStaticLanguageGrp', $ex);
        expertusErrorThrow($ex);
    }

}

function exp_sp_administration_viewgrp_addedit_form($js = NULL, $entityId){
    try{
        expDebug::dPrint('$entityId-->'.$entityId, 4);
        global $theme_key;
        $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
        if($theme_key == 'expertusoneV2') {
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning_v2.css');
        }else{
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.css');
        }
        //CUSTOM SCROLLBAR "JSscrollbar"
        drupal_add_js(drupal_get_path('module', 'exp_sp_lnrreports') .'/jscrollbar/jquery.jscrollpane.js', $js_module_optional);
        $catalog_style = array(
                'ctools-viewscreen-wrapper' => array(
                        'modalSize' => array(
                                'width' => 660,
                                'minHeight' => 500,
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
                    array('id' => 'exp_sp_administration_view_grpdetails_addedit_form', 'title' => (t('LBL816'))) //view
            );
            view_form_display($formIdList, $argsList);
        }
        else {
            throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewclass_addedit_form() : nojs');
        }
    }catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_viewgrp_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * returns the Drupal form for Group view dialog
 * @param unknown $form
 * @param unknown $form_state
 * @param unknown $renderMode
 * @param unknown $grpId
 * @return string
 */
function exp_sp_administration_view_grpdetails_addedit_form($form, &$form_state,$renderMode, $grpId){
    try {
        $wrapperId = 'viewgroup-detail-wrapper';

        $form = newAddEditFormWithCancelAndSave($wrapperId);
        $form['view-scroll-start'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="view-scroll-wrapper">'
        );

        $groupAttributes = getGroupAttributes();
        $attrCnt = count($groupAttributes);
        $height = $attrCnt * 28 + 200; // hight of each attribute is 28, height of multiselect countrol 230

        expDebug::dPrint("Group Attributes 1 --- >".print_r($groupAttributes,true),5);
        $hasItem=count($groupAttributes);
        $leftpane='';
        //$rightpane='<div class="narrow-search-results-item-detail right-header">'.t('This group is based on following user attributes').'</div>';
        $attrLst='';
        $defaultOpen='';
        $cnt=0;
        foreach($groupAttributes as $attr){
            expDebug::dPrint('$attr' .print_r($attr,true),5);
            $class='';
            $gid = $grpId;
            // Get selected items for each attribute
            $selOpt = getAttributeListByText($gid,$attr->code,1);
            $default=array();
            // Prepare default values for each attribute
            if($selOpt[0]->allsel==0){
                foreach($selOpt as $dt){
                    if($attr->name == 'Hire Date') {
                        $default['name'] .= '<span class="item-short-desc">'.$dt->name.'</span>';
                    }
                    else {
                        $default['name']=empty($default['name'])?$dt->name:$default['name'].", ".$dt->name;
                    }
                    $default['code']=empty($default['code'])?$dt->code:$default['code'].",".$dt->code;
                }
            }else{
                $default['name'] = t('LBL1039');
                $default['code'] = 'All';
            }
            $class=empty($default)?'empty-group':'list-group';

            if($defaultOpen == '' && $class=='list-group')
                $defaultOpen = $attr->code;
            // Hiddem field to store default values
            addHiddenFieldToAddEditForm($form, 'atr_'.$attr->code, $default['code']);
            expDebug::dPrint('$class '.print_r($class, 1));
            if($class !='empty-group'){ // conditioned for this ticket #0042617
            // building left and right side panes
            if($attr->name == 'Country'){
                $attr->name = t('LBL039');
            }else if($attr->name == 'Department'){
                $attr->name = t('LBL179');
            }else if($attr->name == 'Employment Type'){
                $attr->name = t('LBL174');
            }else if($attr->name == 'Role'){
                $attr->name = t('Role');
            }else if($attr->name == 'Job Role'){
                $attr->name = t('LBL133');
            }else if($attr->name == 'Language'){
                $attr->name = t('LBL038');
            }else if($attr->name == 'State'){
                $attr->name = t('LBL152');
            }else if($attr->name == 'User Type'){
                $attr->name = t('LBL173');
            }
            expDebug::dPrint('$defaultCode '.$defaultCode);
            if($defaultCode != 'grp_adm' && $defaultCode != 'grp_sup' && $defaultCode != 'grp_ins' && $defaultCode != 'grp_mgr'){
                //$leftpane .= '<div onClick="$(\'#root-admin\').data(\'peoplegroup\').callMultiSelect(\''.$attr->code.'\',\''.$defaultId.'\');"><span class="item-short-desc '.$class.'">'.t($attr->name).'</span><span class="right-tip-arrow" /></div><div id="avil_'.$attr->code.'" class="narrow-search-results-item-detail"></div>';
                $rightpane .= '<div class="attr-content"><span class="'.$class.' attr-list-item" id="header_'.$attr->code.'">'.t($attr->name).':</span></span><span id="sel_'.$attr->code.'" class="viewright-groupdetails">'.$default['name'].'</span></div>';
                if($hasItem>1){
                    $class = ($class=='list-group')? $class." and-class": $class;
                    $rightpane .= '<div class="'.$class.' sel-attr-div" id="and_'.$attr->code.'"><span class="item-short-desc sel-attr-separator">'.t('LBL647').'</span></div>';
                }
            }else{
                //$leftpane .= '<div><span class="item-short-desc '.$class.'">'.t($attr->name).'</span><span class="right-tip-arrow" /></div><div id="avil_'.$attr->code.'" class="narrow-search-results-item-detail"></div>';
                $msg = '';
                switch($defaultCode){
                    case 'grp_adm':
                    case 'grp_sup':
                        $msg = t("All administors");
                        break;
                    case 'grp_ins':
                        $msg = t("All Instructors");
                        break;
                    case 'grp_mgr':
                        $msg = t("All Managers");
                        break;
                }
                if($cnt==0){
                    //$rightpane .= '<div class="attr-content-nonesel narrow-search-results-item-detail right-header"><table class="attr-content-nonesel"><tr><td>'.$msg.'</td></tr></table></div>';
                    $rightpane .= '<div class="attr-content-nonesel-div narrow-search-results-item-detail right-header"><span class="attr-content-nonesel-span">'.$msg.'</span></div>';
                    if($defaultCode == 'grp_adm' || $defaultCode == 'grp_sup')
                        $height = $height - 200;
                    else
                        $height = $height - 120;
                }
                $cnt = 1;
            }
           }
            $attrLst .= ($hasItem==0)?$attr->code:','.$attr->code;
            $hasItem--;
        }

        $form['addedit-group_container_avail'] = array(
                '#type'   => 'markup',
                '#markup' => '<div class="attr-pane"><div class="attr-select-pane">',
        );

        $form['addedit-group_container_sel'] = array(
                '#type'   => 'markup',
                '#markup' => '</div><div id="attr-separator"></div><div class="attr-selected-pane">',
        );

        // Adding right side pane in form
        $form['addedit-group_right_pane'] = array(
                '#type'   => 'markup',
                '#markup' => $rightpane,
        );
        $defaultOpen = $defaultOpen!=''?$defaultOpen:$groupAttributes[0]->code;
        addHiddenFieldToAddEditForm($form, 'attr_open', $defaultOpen);
        // Hidden field to store available list of attributes
        addHiddenFieldToAddEditForm($form, 'atr_list', $attrLst);
        $form['addedit-group_container_selend'.$attr->code] = array(
                '#type'   => 'markup',
                '#markup' => '</div></div>',
        );

        $entityId            = $grpId;
        $entityType          = 'cre_sec';
        $qtipIdInit          = $entityId.'_'.$entityType;
        $qtipgroupshow   = "'dispGrpView':'Y'"; // show the Popup outside
        $qtipOptAttachmentObj  = "{'entityId':".$entityId.",'entityType':'cre_sec',
                                   'url':'people/roles/add-permissions/view/".$entityId."/".$entityType."/qtip_permissions_disp_".$qtipIdInit."',
                                   'popupDispId':'qtip_permissions_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachIdqtip_addpermissions_visible_disp_".$qtipIdInit."',
                                   'wid':550,'heg':'270','postype':'topleft','poslwid':'20','qdis':'ctool',
                                   'linkid':'visible-group-".$entityId."','scrollid':'admin-add-scroll','dispDown':'Y',".$qtipgroupshow."}";

        $adduserVisibpopupId ='qtip_visible_adduser_disp_'.$entityId.'_'.$entityType;
        $qtipUsersClassObj     = "{'entityId':".$entityId.",'entityType':'cre_sec','url':'administration/people/groups/view-users/viewscreen/".$entityId."/".$entityType."',
                                   'popupDispId':'view-users".$adduserVisibpopupId."','catalogVisibleId':'qtipAttachIdqtip_addroletousers_visible_disp_".$qtipIdInit."',
                                   'wid':520,'heg':'270','postype':'topleft','poslwid':'28','qdis':'ctool','linkid':'visible-priv-users-".$entityId."',
                                   'dispDown':'Y',".$qtipgroupshow."}";

        expDebug::dPrint('qtip object : '.print_r($qtipOptAttachmentObj,true), 5);


        $paintMultiAction .='<div id="wrap-perm-user-container">';
        //43063: Privileges option showing learner group also when view mode.
        $is_admin = adminGrporNot($grpId);
        if($is_admin){
        $paintMultiAction .= '<div id="wrap-perm-user-list" class="permission-div"><div class="permission-tab-icon vtip"  title="'.t('LBL1035').'"></div>'.
                "<div id='qtip_permissions_disp_".$qtipIdInit."'><a id='visible-group-".$entityId."' class='tab-title vtip' title='".t('LBL1035')."' onclick = \"callVisibility(".$qtipOptAttachmentObj.");\">".t('LBL1035')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".//<div class=\"tab-seperator\"></div><div class=\"tags-tab-icon\"></div>".
                '</div>';
        $paintMultiAction .=  "<div class='tab-seperator'></div>";
        }
        $paintMultiAction .= "<div id='wrap-perm-user-list'><div class='users-tab-icon vtip' title='".t('Users')."'></div>
                              <div id='view-users".$adduserVisibpopupId."' class='tab-title' style='position:relative;'>
                                <a id='visible-priv-users-".$entityId."' class=\"tab-title vtip\" title='".t('Users')."' onclick = \"callVisibility(".$qtipUsersClassObj."); return false;\">".t('Users')."</a>
                               <span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
                              </div></div>"; //ADD Users
        $paintMultiAction .='</div>';

   $form['addedit-left-col-extend-users'] = array(
        '#type' => 'markup',
        '#markup' => $paintMultiAction,
   );

        $form['view-scroll-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
        );

        return $form;
    }
    catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_view_grpdetails_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}

function view_permissions_addedit_form_html($form,&$form_state){
    try {
        expDebug::dPrint('view_permissions_addedit_form_html'.print_r($form_state,true),5);
        global $theme_key;
        $entityId = $form_state['entity_id'];
      $entityType = $form_state['entity_type'];
      $callFrom = $form_state['call_from'];
      $keyType = $entityId.'_'.$entityType;
      $uniqueId = 'permissions-'.$entityId.'-'.$entityType;
      //$formName = 'Capabilities';
      $formName= 'add-permissions';
      
      //$wrapperId = 'permissions-grid-wrapper-'.$uniqueId;
      //$customHiddenButtonId = 'equv-save-esign';
      expDebug::dPrint('valuess for form state keywords'.$uniqueId,5);
      expDebug::dPrint('$entityId'.$entityId, 4);
      
      $dynamicwrapperid="permissions-addedit-form".$form_state['entity_id'].$form_state['entity_type'];
      $popuprenderPermId = "popup_container_qtip_permissions_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
      $popupDisplayId                = "qtipPermissionsqtip_visible_disp_".$form_state['entity_id'].'_'.$form_state['entity_type'];
      $form = permissionsNewAddEditFormWithCancelAndSave($dynamicwrapperid);
      
      addHiddenValueToAddEditForm($form, 'entity_id', $form_state['entity_id']);
      addHiddenValueToAddEditForm($form, 'entity_type', $form_state['entity_type']);
      addHiddenValueToAddEditForm($form, 'empty_id', $form_state['empty_id']);
      
      //All available permissionlist 
      $permissionsList  = fetchPermissionsList($entityId);
      // Get mapped permissions for the Entity Id and Entity Type
      $permissionMapping = getMappedPermissionMapping($entityId);
      $parentpermMapping = getMappedPermissionMapping($entityId,1);
      
      $form[$formName] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="admin-add-permissions" class="add-permission-class" >',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
        '#suffix' => '</div>',
      );
      
      $form[$formName]['attr_action'] = array(
          '#type' => 'hidden',
          '#name' => 'attr_action',
          '#value' => 'add_permissions',
      );  
      
        $readOnlyType ='disabled="disabled"';
        expDebug::dPrint('permission list value'.print_r($permissionsList,true),4);
      if(!empty($permissionsList)){
        
         $form[$formName]['permissions_fieldset'] = array(
            '#type' => 'markup',
                '#prefix' => '<table width="100%" cellpadding="4" cellspacing="0" border="0" id="permission-table"><tr class="permission-header-tr">
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column permission-theader-first">'.t('LBL1005').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL816').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL817').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL063').'</td>
                <td class="adduser-title-heading user-list-header adduser-title-heading-permission certificate-column">'.t('LBL286').'</td>',
            '#suffix' => '</tr></table>',
         );  
         $form[$formName]['perm-add-start'] = array(
                '#type' => 'markup',
                '#prefix' => '<div id="admin-add-scroll" class="admin-add-class">',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
                
         );
         $form[$formName]['permissions_td_start'] = array(
                '#type' => 'markup',
                '#prefix' =>'<table width="100%" cellpadding="4" cellspacing="0" border="0" id="permission-td-table"><tr class="permission-header-td">',
                '#suffix' => '</tr>',
         );
         
         
    
        $i=0;
        $j=0;
        $k=0;
        $l=0;
        foreach($permissionsList as $key => $val){
            expDebug::dPrint('$val->parmstat '.print_r($val->parmstat, 1));
          $checkedStatus= $checkedstsadd = $checkedstsdelete = $checkedstsedit = ''; 
          $checkedClass =$checkedclsadd = $checkedclsedit = $checkedclsdelete = 'checkbox-unselected-readonly';
          $checkedsubcls = 'checkbox-unselected-readonly';
          $disstyle = ($theme_key=='expertusoneV2')? 'display:none;cursor:default;' : 'cursor:default;';
          $checkedsubattr = '';
          // Already marked permissions should be checked
          if(in_array($val->code, $permissionMapping)){
            $checkedStatus = '';
            $checkedClass = 'checkbox-selected-readonly';
            $checkedsubcls = 'checkbox-selected-readonly';
            $checkedsubattr = 'checked="checked"';
          }
          if(in_array('priv_add-'.$val->code, $permissionMapping)){
            $checkedstsadd = '';
            $checkedclsadd = 'checkbox-selected-readonly';
          }
          if(in_array('priv_edit-'.$val->code, $permissionMapping)){
            $checkedstsedit = '';
            $checkedclsedit = 'checkbox-selected-readonly';
          }
          if(in_array('priv_delete-'.$val->code, $permissionMapping)){
            $checkedstsdelete = '';
            $checkedclsdelete = 'checkbox-selected-readonly';
          }
          $class='permission-detail-tr-odd';
          if($j==1){
            $class='permission-detail-tr-even';
            $j=0;
          }else{
            $j=1;
          }
          expDebug::dprint('permission mapping'.$hideshow, 4);
          // Multilanguage Support
          if($val->pname == 'Course/Class'){
            $val->pname = t('Course').'/'.t('Class');
          }else if($val->pname == 'People Setting'){
            $val->pname = t('People').' '.t('LBL1021');
          }else if($val->pname == 'Survey Questions'){
            $val->pname = t('Survey').' '.t('LBL240');
          }else if($val->pname == 'Assessment Questions'){
            $val->pname = t('Assessment').' '.t('LBL240');
          }else if($val->pname == 'Content'){
            $val->pname = t('Content');
          }else if($val->pname == 'Banner'){
            $val->pname = t('LBL623');
          }else if($val->pname == 'Notification'){
            $val->pname = t('LBL204');
          }else if($val->pname == 'Certificate'){
            $val->pname = t('LBL205');
          }else if($val->pname == 'Announcement'){
            $val->pname = t('LBL350');
          }else if($val->pname == 'Custom Attribute' && module_exists('exp_sp_administration_customattribute')){//#custom_attribute_0078975
            $val->pname = t('LBL2015');
          }else if($val->pname == 'Manage Admin API'){
            $val->pname = t('Manage').' '.t('Admin').' '.t('API');
          }else if($val->pname == 'Interactions Studio'){
            $val->pname = t('LBL3021').' '.t('LBL3088');
          }else if($val->pname == 'Video'){
            $val->pname = t('Video');
          }else if($val->pname == 'Presentation'){
            $val->pname = t('LBL3018');
          }else if($val->pname == 'Discounts'){
            $val->pname = t('Discount');
          }else if($val->pname == 'Commerce Setting'){
            $val->pname = t('Commerce').' '.t('LBL1021');
          }else if($val->pname == 'View All Reports'){
            $val->pname = t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
          }else if($val->pname == 'View Assigned Reports'){
            $val->pname = t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
          }else if($val->pname == 'Module'){
            $val->pname = t('LBL1003');
          }else if($val->pname == 'Reports'){
            $val->pname = t('REPORTS');
          }else if($val->pname == 'Configuration'){
            $val->pname = t('LBL1264');
          }
          if(empty($val->parent2_perm)){
            $checkedparent= $checkedparadd = $checkedparedit = $checkedpardel = '';
            $parentchkcls =$parentaddcls = $parentedicls = $parentdelcls = 'checkbox-unselected';
            $childcls= 'checkbox-unselected-readonly';
            $checkedparattr = '';
            // Already marked permissions should be checked
            if(in_array($val->code, $parentpermMapping)){
                $checkedparent = '';
                $parentchkcls = 'checkbox-selected-readonly';
                $childcls = 'checkbox-selected-readonly';
                $checkedparattr = 'checked="checked"';
            }
            if(in_array('priv_add-'.$val->code, $parentpermMapping)){
                $checkedparadd = '';
                $parentaddcls = 'checkbox-selected-readonly';
            }
            if(in_array('priv_edit-'.$val->code, $parentpermMapping)){
                $checkedparedit = '';
                $parentedicls = 'checkbox-selected-readonly';
            }
            if(in_array('priv_delete-'.$val->code, $parentpermMapping)){
                $checkedpardel = '';
                $parentdelcls = 'checkbox-selected-readonly';
            }
            $k++;
            $toggle='';
            $oid = '0';
            if(!empty($permissionsList[$i+1]->parent2_perm)){
                $toggle = '<span class="more-text"><a class="show-short-text more-icon-close" onclick="permdetailsShowHide(this,\''.$k.'\',\'parentlevel\');" href="javascript:void(0)">&nbsp;</a></span>';
                $oid = $val->id;
            }
            if($val->pname =='Manage Discussion'){
                $val->pname =t('Manage').' '.t('DISCUSSIONS');
            }else if($val->pname == 'View All Reports'){
                $val->pname =t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'View Assigned Reports'){
                $val->pname =t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
            }
            $markup = '<tr width="100%" class="'.$class.'">
                    <td class="user-list-detail certificate-column">
                        <span class="vtip" title="'.sanitize_data(t($val->pname)).'"> '.titleController('ADMIN-GROUPS-PNAME-PARENT',t($val->pname),45).'</span>'.$toggle.
                    '</td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$parentchkcls.' chk-view">
                            <input type="checkbox" '.$readOnlyType.' id = "group_view_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_parent_permissions'.$uniqueId.'[]" obname="group_view_'.$oid.'" value="'.$val->code.'" '.$checkedparent.'>
                        </div>
                    </td>';
            if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                            <div class="'.$parentaddcls.' chk-create">
                                <input type="checkbox" '.$readOnlyType.' id = "group_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedparadd.'>
                            </div>
                        </td>
                        <td class="user-list-checkbox certificate-column">
                            <div class="'.$parentedicls.' chk-edit">
                                <input type="checkbox" '.$readOnlyType.' id = "group_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedparedit.'>
                            </div>
                        </td>
                        <td class="user-list-checkbox certificate-column">
                            <div class="'.$parentdelcls.' chk-create">
                                <input type="checkbox" '.$readOnlyType.' id = "group_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_parent_permissions'.$uniqueId.'[]" obname="group_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedpardel.'>
                            </div>
                        </td>';
            }else{
                    $markup .='<td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.' chk-create">
                            <input type="checkbox" disabled="disabled" id = "group_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedparattr.' >
                        </div>
                    </td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.'  chk-edit">
                            <input type="checkbox" disabled="disabled" id = "group_edit_'.$i.'_'.$oid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedparattr.'>
                        </div>
                    </td>
                    <td class="user-list-checkbox certificate-column">
                        <div class="'.$childcls.' chk-create">
                            <input type="checkbox" disabled="disabled" id = "group_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_parent_permissions'.$uniqueId.'[]" obname="group_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedparattr.' >
                        </div>
                    </td>';
            }
            $form[$formName]['permissions_table_row_head'.$i] = array(
                    '#type' => 'markup',
                    '#prefix' => $markup,
                    '#suffix' => '</tr>',
            );
          }else{
            // Multilanguage Support
            if($val->pname == 'Course/Class'){
                $val->pname = t('Course').'/'.t('Class');
            }else if($val->pname == 'People Setting'){
                $val->pname = t('People').' '.t('LBL1021');
            }else if($val->pname == 'Survey Questions'){
                $val->pname = t('Survey').' '.t('LBL240');
            }else if($val->pname == 'Assessment Questions'){
                $val->pname = t('Assessment').' '.t('LBL240');
            }else if($val->pname == 'Content'){
                $val->pname = t('Content');
            }else if($val->pname == 'Banner'){
                $val->pname = t('LBL623');
            }else if($val->pname == 'Notification'){
                $val->pname = t('LBL204');
            }else if($val->pname == 'Certificate'){
                $val->pname = t('LBL205');
            }else if($val->pname == 'Announcement'){
                $val->pname = t('LBL350');
            }else if($val->pname == 'Custom Attribute' && module_exists('exp_sp_administration_customattribute')){//#custom_attribute_0078975
                $val->pname = t('LBL2015');
            }else if($val->pname == 'Manage Admin API'){
                $val->pname = t('Manage').' '.t('Admin').' '.t('API');
            }else if($val->pname == 'Interactions Studio'){
                $val->pname = t('LBL3021').' '.t('LBL3088');
            }else if($val->pname == 'Video'){
                $val->pname = t('Video');
            }else if($val->pname == 'Presentation'){
                $val->pname = t('LBL3018');
            }else if($val->pname == 'Discounts'){
                $val->pname = t('Discount');
            }else if($val->pname == 'Commerce Setting'){
                $val->pname = t('Commerce').' '.t('LBL1021');
            }else if($val->pname == 'View All Reports'){
                $val->pname = t('LBL816').' '.t('LBL1039').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'View Assigned Reports'){
                $val->pname = t('LBL816').' '.t('LBL736').' '.ucfirst(strtolower(t('REPORTS')));
            }else if($val->pname == 'Module'){
                $val->pname = t('LBL1003');
            }else if($val->pname == 'Administrator group'){
                $val->pname = t('Administrator').' '.strtolower(t('Group'));
            }else if($val->pname == 'Learner group'){
                $val->pname = t('Learner').' '.strtolower(t('Group'));
            }
            expDebug::dPrint('profile list items value ---'. print_r($val,true),4);
          
          if(!empty($val->sub_parent)){
                  $l++;
                  $toggle='';
                  $pid = $val->id;
                  $toggle = '<span class="more-text"><a class="show-short-text more-icon-close" onclick="permdetailsShowHide(this,\''.$l.'\',\'subparentlevel\');" href="javascript:void(0)">&nbsp;</a></span>';
                  //$oid = $val->id;
                  $markup = '<tr width="100%" class="'.$class.' sub-level child'.$k.'" style="display: none;">
                  <td class="user-list-detail certificate-column ">
                  <span class="vtip" title="'.sanitize_data(t($val->pname)).'"> '.titleController('ADMIN-GROUPS-PNAME-CHILD',t($val->pname),45).'</span>'.$toggle.'
                  </td>
                  <td class="user-list-checkbox certificate-column">
                  <div class="'.$checkedClass.' chk-view">
                  <input type="checkbox" '.$readOnlyType.' id = "subgroup_view_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="subgroup_view_'.$oid.'_'.$pid.'" value="'.$val->code.'" '.$checkedStatus.'>
                  </div>
                  </td>';
                  if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_create_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subgroup_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subgroup_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
                }else{
                    $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_create_'.$i.'_'.$oid.'_'.$pid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subgroup_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subgroup_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                };
              }else if(!empty($val->sub_parent_id)){
                
                  $markup = '<tr width="100%" class="'.$class.' sub-level sub-sub-level'.$k.' subchild'.$l.'" style="display: none;">
                  <td class="user-list-detail certificate-column ">
                  <span class="vtip" title="'.t($val->pname).'"> '.titleController(' ',t($val->pname),45).'</span>'.$hideshow.'
                  </td>
                  <td class="user-list-checkbox certificate-column">
                  <div class="'.$checkedClass.' chk-view">
                  <input type="checkbox" '.$readOnlyType.' id = "subchild_view_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="subchild_view_'.$oid.'" value="'.$val->code.'" '.$checkedStatus.'>
                  </div>
                  </td>';
                  if($val->parmstat != 1){
                $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_create_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "subchild_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="subchild_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
                }else{
                    $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subchild_create_'.$i.'_'.$oid.'_'.$pid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_create_'.$oid.'_'.$pid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "subchild_edit_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_edit_'.$oid.'_'.$pid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "subchild_delete_'.$i.'_'.$oid.'_'.$pid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="subchild_delete_'.$oid.'_'.$pid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                };
            }else{ 
              $markup = '<tr width="100%" class="'.$class.' sub-level child'.$k.'" style="display: none;">
                  <td class="user-list-detail certificate-column ">
                      <span class="vtip" title="'.t($val->pname).'"> '.titleController('ADMIN-GROUPS-PNAME-CHILD',t($val->pname),45).'</span>'.$hideshow.'
                  </td>            
                  <td class="user-list-checkbox certificate-column">
                      <div class="'.$checkedClass.' chk-view">
                          <input type="checkbox" '.$readOnlyType.' id = "sub_view_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this,\''.$val->parmstat.'\');" isParent="'.$val->parmstat.'" name="attach_add_permissions'.$uniqueId.'[]" obname="sub_view_'.$oid.'" value="'.$val->code.'" '.$checkedStatus.'>
                      </div>
                  </td>';
                    if($val->parmstat != 1){
                        $markup .= '<td class="user-list-checkbox certificate-column">
                                                    <div class="'.$checkedclsadd.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_create_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedstsadd.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsedit.' chk-edit">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_edit_'.$oid.'" value="priv_edit-'.$val->code.'" '.$checkedstsedit.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedclsdelete.' chk-create">
                                                <input type="checkbox" '.$readOnlyType.' id = "sub_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' onclick="setPrivilege(this);" name="mode_permissions'.$uniqueId.'[]" obname="sub_delete_'.$oid.'" value="priv_delete-'.$val->code.'" '.$checkedstsdelete.'>
                                            </div>
                                        </td>';
                    }else{
                        $markup .= '<td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "sub_create_'.$i.'_'.$oid.'"  class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_create_'.$oid.'" value="priv_add-'.$val->code.'" '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column">
                                            <div class="'.$checkedsubcls.'  chk-edit">
                                                <input type="checkbox" disabled="disabled" id = "sub_edit_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_edit_'.$oid.'" value="priv_edit-'.$val->code.'"  '.$checkedsubattr.'>
                                            </div>
                                        </td>
                                        <td class="user-list-checkbox certificate-column ">
                                            <div class="'.$checkedsubcls.' chk-create">
                                                <input type="checkbox" disabled="disabled" id = "sub_delete_'.$i.'_'.$oid.'" class = "attach-permission-cls" style ='.$disstyle.' name="mode_permissions'.$uniqueId.'[]" obname="sub_delete_'.$oid.'" value="priv_delete-'.$val->code.'"  '.$checkedsubattr.' >
                                            </div>
                                        </td>';
                            };
              }
          $form[$formName]['permissions_table_row_child'.$i] = array(
            '#type' => 'markup',
            '#prefix' => $markup,
            '#suffix' => '</tr>',
          ); 
          
          }
          $i++;
          
          //$form['permissions_fieldset']['permissions_hidden'][$val->code] = array('#type' => 'hidden', '#value' => $val->code);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][0] = array('#type' => 'hidden', '#value' => $val->permission);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][1] = array('#type' => 'hidden', '#value' => $val->parent1_perm);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][2] = array('#type' => 'hidden', '#value' => $val->parent2_perm);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][3] = array('#type' => 'hidden', '#value' => $val->object_code);
          $form['permissions_fieldset']['permissions_hidden'][$val->code][4] = array('#type' => 'hidden', '#value' => $val->parent3_perm);
          //endAddEditTwoColumnRow($form);
          
        }
        
        
        $form[$formName]['permissions_td_end'] = array(
            '#type' => 'markup',
            '#suffix' => '</table>',
         );  
        $form[$formName]['perm-add-end'] = array(
                '#type' => 'markup',
                '#suffix' => '</div>',
        );
        
      } else {
        
          $form[$formName]['no-records-msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="no-records-msg">'.t('There are no permissions available for this Group').'</div>'
          );
      }
       
//      permissionsAddEditFormWithCancelAndSaveClose($form,$dynamicwrapperid,'newtheme-save-button','newtheme-cancel-link',$entityId,$callFrom);
     
      return $form;
    
  } catch (Exception $ex) {
        watchdog_exception('view_permissions_addedit_form_html', $ex);
        expertusErrorThrow($ex);
    }
}

function view_display_assigned_users($form, &$form_state) {
    try{
    include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
    include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
    $role_id = $form_state['role_id'];
    $userId= $form_state['user_id'];
    $entityType = 'cre_sec';
    $excludedRoleId = '';
    $uniqueId = "RoleDisplayUsers-$role_id-$entityType";
    $wrapperId = 'security-role-detail-wrapper-list';
    $form = array(); 
    $form['#prefix'] = /* (stripos($_SERVER['HTTP_USER_AGENT'], 'chrome') !== false
                                        || (stripos($_SERVER['HTTP_USER_AGENT'], 'firefox') !== false)? '</form>' : '' ) */ '</form>'.'<div id="security-role-detail-wrapper-list">';
    $form['#suffix'] = '</div>';
    $setminHight =  'admin-add-users-tab-datagrid-wrapper';
    $form['edit-class-details-start'] = array(
      '#prefix' => "<div id='add-user-role-page' class = 'assigned-users-grps'>",
    );
    
    if(isset($addAttributes))
        unset($addAttributes['onclick']);

 $form['add-class-list'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="catalog-class-addedit-form-details"><div id="add-class-list-wrapper">'
  );

//      $UserList=fetchUserListToGroup($role_id,'');
     $Userquery = fetchUserListToGroup($role_id,'','count');
     $Usercount = $Userquery->execute()->fetchField();
     //$Usercount=count($UserList->execute()->fetchAll());
     expDebug::dPrint('userscount'.count($Usercount),4);
     if($Usercount !=0){
            
            $form['class_auto-search'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-class-auto-search-container">',
        );
        
        $form['class_auto-search-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div>',
        );
     
        $searchAttributes['class'][] = 'admin-pagination-role-search-go';
        $searchAttributes['class'][] = 'addedit-form-expertusone-throbber';
        $searchAttributes['data-wrapperid'][] = 'security-role-detail-wrapper-list';
        $form['search_add_urs'] = array(
            '#attributes' => $searchAttributes,
        );
        
     //}
    $form['edit-class-details-paginate'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-add-course-training-plan" class="display_user_list"><div id="role-'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
    '#suffix' => '<div>',
  );
  $form['role_user_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common" style="display:none;"></div></div>',
   );      
    $form['role_user_grid_load'] = array( 
      '#type' => 'markup',
      '#prefix' => '<div style="display:block"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'viewscreen\', \'RoleDisplayUsers\', \'\', \''.$role_id.'\', \''.$entityType.'\', \''.$excludedRoleId.'\');" width="100" height="132" /></div>',
    ); 
    $form['role_user_grid_load_end'] = array( 
      '#type' => 'markup',
      '#prefix' => '</div></div>',
    );
     
    $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg  white-btn-bg-middle';
  $cancelAttributes['onclick'][]         =  "closeQtip('','$entityId'); return false;";
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
      
  $form['cancel_assigneduser_edit_mode'] = array(
     '#type' => 'submit',
     '#value' => t('LBL123'),
     '#title' => t('LBL123'),
     '#prefix' => '<div id="assigned_user" ><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div>',
     '#name' => 'Cancel addusers',
     '#attributes' => $cancelAttributes,
   );

     }else{
                $msg= t('MSG403');
            
        
        $form['addroleuser_view_grid_no_msg'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div>',
          );
     }
        
  return $form;
  }catch (Exception $ex) {
        watchdog_exception('display_assigned_users', $ex);
        expertusErrorThrow($ex);
    }
}

function addNewGroupByRestAPI(&$form_state,$createuserid="",$gid) {

  try {
    
        $config=getConfig("exp_sp");
        $instant_count = $config['instant_load_count'];
        $back_ground = FALSE ;
    
        expDebug::dPrint(' addNewGroupByRestAPI() called ');
        expDebug::dPrint(' addNewGroupByRestAPI() form_state value '.print_r($form_state, true),4);
        $form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes  //added by ayyappans for 46437: While creating group within single quote('').
        $mandatoryField = addEditCheckMandatoryFields($form, $form_state, array('name'));
        $hireStart = '';
        $hireEnd = '';
        $hire_or_after = trim($form_state['values']['hire_on_or_after']);
        $hire_or_before = trim($form_state['values']['hire_on_or_before']);
        $hire_between = $form_state['values']['hire_between'];
        if($mandatoryField) {
          return array((object)array('Id'=>$mandatoryField));
          exit;
        }

         $is_admin = $form_state['values']['is_admin'];

         expDebug::dPrint(' addNewGroupByRestAPI() form_state value is_admin '.print_r($is_admin, true),4);
         if($is_admin == '1'){
           $grpType = 'admin';
         } else {
           $grpType = 'learner';
         }

         if(!empty($form_state['values']['job_role'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['job_role'],'cre_usr_jrl_','job role', 'no','job_role',$gid);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['user_type'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['user_type'],'cre_usr_ptp_','user type', 'no','user_type',$gid);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['jobtitle'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['jobtitle'],'cre_usr_jtl_','job title', 'no','jobtitle',$gid);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['employment_type'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['employment_type'],'cre_usr_etp_','employment type', 'no','employment_type',$gid);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['department'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['department'],'cre_usr_dpt_','department', 'no','department',$gid);
            //expDebug::dPrint(' JJJJJJ '.print_r($gid, true),4);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['country'])){
            $roleErrmsg = is_valid_country($form_state['values']['country'], 'yes');
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['state'])){
            if(empty($form_state['values']['country'])){
                $countryErrmsg=new stdClass();
                $countryErrmsg->isValidateError = 1;
                $countryErrmsg->errormsg = "Country field should not be empty";
                return $countryErrmsg;
            }
            $roleErrmsg = is_valid_state($form_state['values']['country'],$form_state['values']['state'], 'yes');
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['language'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['language'],'cre_sys_lng_','preferred language', 'no','language',$gid);
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
         }
         if(!empty($form_state['values']['role'])) {
            //$role = array_unique(explode(',',$form_state['values']['roles']));
            $role = $form_state['values']['role'];
            if($role != 'mgr' && $role !='ins'
                    && $role != 'ins,mgr' && $role != 'mgr,ins'){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "The given role is invalid";
                return $roleErrmsg;
            }
         }
         if(!empty($hire_or_after) && !preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hire_or_after)){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire on or after date format is wrong";
                return $roleErrmsg; 
            
                }
                
        if (!empty($hire_or_after) && !validateDateMonthYear($hire_or_after,'m-d-Y',70,5)) {
            $roleErrmsg = new stdClass ();
            $roleErrmsg->isValidateError = 1;
            $roleErrmsg->errormsg = "Hire On Or After date should be within range of 70 years past and 5 years in future from current date.";
            return $roleErrmsg;
        }
        if(!empty($hire_or_before) && !preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hire_or_before)){
            $roleErrmsg = new stdClass ();
            $roleErrmsg->isValidateError = 1;
            $roleErrmsg->errormsg = "Hire date format is wrong";
            return $roleErrmsg;
        }
            
        if (!empty($hire_or_before) && !validateDateMonthYear($hire_or_before,'m-d-Y',70,5)) {
            $roleErrmsg = new stdClass ();
            $roleErrmsg->isValidateError = 1;
            $roleErrmsg->errormsg = "Hire On or Before date should be within range of 70 years past and 5 years in future from current date.";
            return $roleErrmsg;
        }

       if(!empty($hire_between)) {
         
         $dateArr = explode("and",$hire_between);
         $hireStart = trim($dateArr[0]);
         $hireEnd = trim($dateArr[1]);
         if(!preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hireStart) || (!empty($hireEnd) && (!preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hireEnd)))){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire date format is wrong";
                return $roleErrmsg; 
        }
        if (empty($hireStart) || $hireStart ==  '' || empty($hireEnd) || $hireEnd == '') { // either start or end date is empty
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire start and end date is required";
                return $roleErrmsg; 
            }
            
         if(!empty($hireStart)&& !empty($hireEnd))  {
            if(!validateDateMonthYear($hireStart,'m-d-Y',70,5) || !validateDateMonthYear($hireEnd,'m-d-Y',70,5)){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire Date should be within range of  70 years from past and 5 years in future from current date.";
                return $roleErrmsg;
            }
           // start date less then end date
            $hireEndTemp = str_replace("-","/", $hireEnd);   
            $hireStartTemp = str_replace("-","/", $hireStart);
            if (strtotime($hireEndTemp) < strtotime($hireStartTemp) ) {
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "End date cannot be less than the start date.";
                return $roleErrmsg;
            }
            
         }
    }
       $modeArr = array();
       if(!empty($hire_or_after)){
            $modeArr[] =  'ste_con_hdt_gtn';
            $form_state['values']['hidden_hire_start_ste_con_hdt_gtn'] = $hire_or_after;
       }
       if(!empty($hire_or_before)){
            $modeArr[] =  'ste_con_hdt_ltn';
            $form_state['values']['hidden_hire_start_ste_con_hdt_ltn'] = $hire_or_before;
       }
       if(!empty($hire_between)){
            $modeArr[] =  'ste_con_hdt_btw';
            $form_state['values']['hidden_hire_start_ste_con_hdt_btw'] = $hireStart;
            $form_state['values']['hire_end'] = $hireEnd;
       }
      
       
      
        $groupAttributes = array(
            'ste_con_dep' => 'department',
                'ste_con_ety' => 'employment_type',
                'ste_con_org' => 'org_id',
                'ste_con_jbr' => 'job_role',
                'ste_con_lng' => 'language',
                'ste_con_rol' => 'role',
                'ste_con_usr' => 'user_type',
                'ste_con_cnt' => 'country',
                'ste_con_ste' => 'state'
                //'ste_con_hdt' => 'hire_mode'
        );

        foreach($groupAttributes as $key => $attr){
            $attr_arr = explode(",", $form_state['values'][$attr]);
            if(count($attr_arr) > 1)
            {
                $uniqueAttrValue = array_unique($attr_arr);
                $newAttrArr = array();
                foreach($uniqueAttrValue as $attr_key=>$attr_val)
                {
                    if(trim($attr_val) != '')
                        $newAttrArr[] = trim($attr_val);
                }
                $form_state['values'][$attr] = implode(",", $newAttrArr);
            }
            
                $form_state['values']['atr_'.$key] = $form_state['values'][$attr];
            
        }
         expDebug::dPrint(' addNewGroupByRestAPI() form_state value is_admin '.print_r($grpType, true),4);

         //Hire Date changes
         $form_state['values']['atr_ste_con_hdt'] = count($modeArr > 0) ? implode(',',$modeArr) : '';
         
        $dupcnt = roleNameDuplicatecheck($form_state['values']['name'],'0');
        if ($dupcnt != 0) {
          expDebug::dPrint('duplicate value',4);
          $dupcnt = form_set_error('name', t('MSG644') .' '.$form_state['values']['name'] .t('LBL271').' '.t('MSG645'));
          return array((object)array('Id'=>$dupcnt));
          exit;
        }
        $grpArr = addNewGroup($form, $form_state, $createuserid, $callFromAPI = 1,$grpType);
        $grId = $grpArr[0]->id;
        expDebug::dPrint('test group id >> '.print_r($grpArr,1),5);
        $total_no_of_users = getUserDetailList($grId, 0, 0,'','',1);
        $is_admin = adminGrporNot($grId);
        if($total_no_of_users <= $instant_count){
            if($is_admin == 1){
                grpUserDelete($grId);
                grpUserInsert($grId);
            }
            populateGroupUsersInFlatTable($grId,'','DeleteGroup');
            
        }else{
            $back_ground = TRUE; 
            if($is_admin == 1){
                grpUserDelete($grId);
                $grp_details= getGrpDetails($grId,array('code','name','status','is_admin','created_by'));
                //Default entry in admin groups table
                $user_add_query = 'INSERT INTO slt_admin_group_users ( group_id,group_code,user_id,status, created_by, created_on, updated_by, updated_on) VALUES ';
                $status = $grp_details['status'];
                $val_str .= '('.$grId.', "'.$grp_details['code'].'", null, "'.$status.'", "'.$grp_details['created_by'].'", now(), '.$grp_details['created_by'].', now() )';
                 
                //Insert query
                $insert_qry = $user_add_query.' '.$val_str;
                expDebug::dPrint("INSERT QUERY : ".$insert_qry , 4);
                $insert = db_query($insert_qry);
                 
            }
            //mark the update action to be handled in backgroud cron job
            expDebug::dPrint('Inside else case where total count exceeds config limit:: '.$total_no_of_users,4);
            // Check certification monitor job status
            $jobname = 'group_bulk_user_upload';
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
            $inProgressJobs = fetchBackgroundListForGroups($cTStatus,'inlist',$grId);
            expDebug::dPrint("inprogress job before insert >>> ". $inProgressJobs);
            if($inProgressJobs==0){
                $insertStmt = db_insert('slt_temp_background_jobs');
                $jobId = date('dmyHis');
                $insertStmt->fields(array(
                        'program_id' => $grId,
                        'job_name' => 'GU-'.$jobId,
                        'created_by' => $createuserid,
                        'created_on' => now(),
                        'updated_by' => $createuserid,
                        'updated_on' => now(),
                        'custom1' => $total_no_of_users,
                        'custom2' => '',
                        'custom3' => '',
                        'custom4' => '',
                        'status' => 'new'
                ));
                expDebug::dPrintDBAPI("slt_temp_background_jobs check",$insertStmt,5);
                $insertStmt->execute();
                 
                //Entry for securitymanagementAdmin
                $delete = db_delete('slt_group_user_mapping');
                $delete->condition('group_id',$grId);
                expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $delete);
                $delete->execute();
                expDebug::dPrint("complete the process");
                 
            }
            
             
        }
        if($back_ground == TRUE){
            return array((object)array('Id'=>"Group created successfully and users are being added to the group. A notification will be sent to you on completion."));
        }
        
        if($grId) {
          return array((object)array('Id'=>$grId));
        }
        else{
          //return array((object)array('Id'=>'Failure'));
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Invalid UserId.";
            return $errobj;
        }
      } catch (Exception $ex) {
      watchdog_exception('addNewUserByRestAPI', $ex);
      expertusErrorThrow($ex);
    }
}

function updateGroupByRestAPI(&$form_state,$updateuserid="") {
  try {
    
    $config = getConfig("exp_sp");
    $instant_count = $config['instant_load_count'];
    $back_ground = FALSE;
    
    /*given form state values - validation part - start here */
    $form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
    $form_state['values'] = array_map('trim', $form_state['values']);
    $mandatoryField = addEditCheckMandatoryFields($form, $form_state, array('name'));
    expDebug::dPrint(' updateGroupByRestAPI() form_state value '.print_r($updateuserid, true),4);

    $hireStart = '';
    $hireEnd = '';
    $hire_or_after = $form_state['values']['hire_on_or_after'];
    $hire_or_before = $form_state['values']['hire_on_or_before'];
    $hire_between = $form_state['values']['hire_between'];
    
    if($mandatoryField) {
      return array((object)array('Id'=>$mandatoryField));
      exit;
    }
    $grpId = trim($form_state['values']['id']);

    if(!($grpId > 0 && is_numeric($grpId)))
    {
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->field = 'id';
        $errobj->errormsg = "Invalid id";
        return $errobj;
    }
    $groupExist = getGrpCode($grpId); // #46952: group exist validated
    if (empty($groupExist)) {
        //return array((object)array('Id'=> 'Invalid Group id'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid Group id.";
        return $errobj;
        exit;
    }
    if(($groupExist == "grp_ins" || $groupExist == "grp_mgr" || $groupExist == "grp_adm" || $groupExist == "grp_sup") && $updateuserid > 1){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errormsg = "Don't have access admin API";
        return $errobj;
        exit;
    }
        if(($groupExist == "grp_adm" || $groupExist == "grp_sup") && $updateuserid == 1){
            if(!empty($form_state['values']['org_id']) || !empty($form_state['values']['user_type']) || !empty($form_state['values']['employment_type']) || !empty($form_state['values']['country']) || !empty($form_state['values']['state']) || !empty($form_state['values']['department']) || !empty($form_state['values']['job_role']) || !empty($form_state['values']['language']) || !empty($form_state['values']['role'])){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;               
            $errobj->errormsg = "Don't have access admin API";
                return $errobj;
                exit;
            }
        }
        if($groupExist == "grp_mgr" && $updateuserid == 1){
            if(!empty($form_state['values']['org_id']) || !empty($form_state['values']['user_type']) || !empty($form_state['values']['employment_type']) || !empty($form_state['values']['country']) || !empty($form_state['values']['state']) || !empty($form_state['values']['department']) || !empty($form_state['values']['job_role']) || !empty($form_state['values']['language']) 
                    || (empty($form_state['values']['role']) || $form_state['values']['role'] != 'mgr')){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errormsg = "Don't have access admin API";
                return $errobj;
                exit;
            }
        }
        if($groupExist == "grp_ins" && $updateuserid == 1){
            if(!empty($form_state['values']['org_id']) || !empty($form_state['values']['user_type']) || !empty($form_state['values']['employment_type']) || !empty($form_state['values']['country']) || !empty($form_state['values']['state']) || !empty($form_state['values']['department']) || !empty($form_state['values']['job_role']) || !empty($form_state['values']['language'])
                    || (empty($form_state['values']['role']) || $form_state['values']['role'] != 'ins')){
                $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errormsg = "Don't have access admin API";
            return $errobj;
            exit;
        }
      }
      //Back ground job is running or not
      $checkStatus = array ('new','inprogress');
      $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$$grpId);
      if($back_gd_running){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errormsg = "Back ground job is running...";
            return $errobj;
            exit;
      }
    //$is_admin = $form_state['values']['is_admin'];
    $is_admin = adminGrporNot($grpId);
    expDebug::dPrint(' updateGroupByRestAPI() $is_admin '.$is_admin,4);
    expDebug::dPrint(' addNewGroupByRestAPI() form_state value is_admin '.print_r($is_admin, true),4);
    if($is_admin == '1'){
      $grpType = 'admin';
    } else {
      $grpType = 'learner';
    }
    $dupcnt = roleNameDuplicatecheck($form_state['values']['name'],$form_state['values']['id']);
    if ($dupcnt != 0) {
      expDebug::dPrint('duplicate value',4);
      $dupcnt = form_set_error('name', t('MSG644') .' '.$form_state['values']['name'] .t('LBL271').' '.t('MSG645'));
      return array((object)array('Id'=>$dupcnt));
      exit;
    }
   if(!empty($form_state['values']['job_role'])) {
      $roleErrmsg = checkValidUserAtrribute($form_state['values']['job_role'],'cre_usr_jrl_','job role', 'no');
      if($roleErrmsg !== TRUE){
        return $roleErrmsg;
      }
    }
      if(!empty($form_state['values']['user_type'])) {
                $roleErrmsg = checkValidUserAtrribute($form_state['values']['user_type'],'cre_usr_ptp_','user type', 'no');
                if($roleErrmsg !== TRUE){
                  return $roleErrmsg;
                }
        }
        if(!empty($form_state['values']['jobtitle'])) {
                $roleErrmsg = checkValidUserAtrribute($form_state['values']['jobtitle'],'cre_usr_jtl_','job title', 'no');
                if($roleErrmsg !== TRUE){
                  return $roleErrmsg;
                }
        }
        if(!empty($form_state['values']['employment_type'])) {
                $roleErrmsg = checkValidUserAtrribute($form_state['values']['employment_type'],'cre_usr_etp_','employment type', 'no');
                if($roleErrmsg !== TRUE){
                  return $roleErrmsg;
                }
        }
        if(!empty($form_state['values']['department'])) {
                $roleErrmsg = checkValidUserAtrribute($form_state['values']['department'],'cre_usr_dpt_','department', 'no');
                if($roleErrmsg !== TRUE){
                  return $roleErrmsg;
                }
        }
      if(!empty($form_state['values']['country'])){
                $roleErrmsg = is_valid_country($form_state['values']['country'], 'yes');
                if($roleErrmsg !== TRUE){
                      return $roleErrmsg;
                }
            }
        if(!empty($form_state['values']['state'])){
            if(empty($form_state['values']['country'])){
                $countryErrmsg=new stdClass();
                $countryErrmsg->isValidateError = 1;
                $countryErrmsg->errormsg = "Country field should not be empty";
                return $countryErrmsg;
            }
            $roleErrmsg = is_valid_state($form_state['values']['country'],$form_state['values']['state'], 'yes');
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
        }
      if(!empty($form_state['values']['language'])) {
            $roleErrmsg = checkValidUserAtrribute($form_state['values']['language'],'cre_sys_lng_','preferred language', 'no');
            if($roleErrmsg !== TRUE){
                return $roleErrmsg;
            }
        }
      if(!empty($form_state['values']['role'])) {
            //$role = array_unique(explode(',',$form_state['values']['roles']));
            $role = $form_state['values']['role'];
            if($role != 'mgr' && $role !='ins'
                    && $role != 'ins,mgr' && $role != 'mgr,ins'){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "The given role is invalid";
                return $roleErrmsg;
            }
        }
        
        //Hire date validation
        if(!empty($hire_or_after) && !preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hire_or_after)){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
            $roleErrmsg->errormsg = "Hire on or after date format is wrong";
                return $roleErrmsg; 
            
          }
         if (!empty($hire_or_after) && !validateDateMonthYear($hire_or_after,'m-d-Y',70,5)) {
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire On Or After date should be within 70 years past and 5 years in future from current date.";
                return $roleErrmsg;
            
          }
        if(!empty($hire_or_before) && !preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hire_or_before)){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire date format is wrong";
                return $roleErrmsg; 
          }
         if (!empty($hire_or_before) && !validateDateMonthYear($hire_or_before,'m-d-Y',70,5)) {
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire On Or Before date should be within 50 years past and 5 years in future from current date.";
                return $roleErrmsg;
            }
         
        if(!empty($hire_between)) {
        
            $dateArr = explode("and",$hire_between);
         $hireStart = $dateArr[0];
         $hireEnd = $dateArr[1];
         if(!preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hireStart) || (!empty($hireEnd) && (!preg_match("/^(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])-[0-9]{4}$/",$hireEnd)))){
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire date format is wrong";
                return $roleErrmsg; 
         }
            if (empty($hireStart) || $hireStart ==  '' || empty($hireEnd) || $hireEnd == '') { // either start or end date is empty
                $roleErrmsg=new stdClass();
                $roleErrmsg->isValidateError = 1;
                $roleErrmsg->errormsg = "Hire start and end date is required";
                return $roleErrmsg; 
            }
                    
            if (!empty($hireStart) && !empty($hireEnd)) {
                
                if(!validateDateMonthYear($hireStart,'m-d-Y',70,5) || !validateDateMonthYear($hireEnd,'m-d-Y',70,5)){
                    $roleErrmsg=new stdClass();
                    $roleErrmsg->isValidateError = 1;
                    $roleErrmsg->errormsg = "Hire Date should be within 50 years from past and 5 years in future from current date.";
                    return $roleErrmsg;
                }
                // start date less then end date
                $hireEndTemp = str_replace("-","/", $hireEnd);
                $hireStartTemp = str_replace("-","/", $hireStart);
                if (strtotime($hireEndTemp) < strtotime($hireStartTemp) ) {
                    $roleErrmsg=new stdClass();
                    $roleErrmsg->isValidateError = 1;
                    $roleErrmsg->errormsg = "End date cannot be less than the start date.";
                    return $roleErrmsg; 
                }
            }
            
         }
        $modeArr = array();
        if(!empty($hire_or_after)){
            $modeArr[] =  'ste_con_hdt_gtn';
            $form_state['values']['hidden_hire_start_ste_con_hdt_gtn'] = $hire_or_after;
        }
        if(!empty($hire_or_before)){
            $modeArr[] =  'ste_con_hdt_ltn';
            $form_state['values']['hidden_hire_start_ste_con_hdt_ltn'] = $hire_or_before;
        }
        if(!empty($hire_between)){
            $modeArr[] =  'ste_con_hdt_btw';
            $form_state['values']['hidden_hire_start_ste_con_hdt_btw'] = $hireStart;
            $form_state['values']['hire_end'] = $hireEnd;
       }
        
        //Hire Date changes
        $form_state['values']['atr_ste_con_hdt'] = count($modeArr > 0) ? implode(',',$modeArr) : '';
        
        
        
 /*validation part end */


    $groupAttributes = array(
            'ste_con_dep' => 'department',
            'ste_con_ety' => 'employment_type',
            'ste_con_org' => 'org_id',
            'ste_con_jbr' => 'job_role',
            'ste_con_lng' => 'language',
            'ste_con_rol' => 'role',
            'ste_con_usr' => 'user_type',
            'ste_con_cnt' => 'country',
            'ste_con_ste' => 'state',
            //'ste_con_hdt' => 'hire_mode'
    );

    foreach($groupAttributes as $key => $attr){
        $attr_arr = explode(",", $form_state['values'][$attr]);
        if(count($attr_arr) > 1)
        {
            $uniqueAttrValue = array_unique($attr_arr);
            $newAttrArr = array();
            foreach($uniqueAttrValue as $attr_key=>$attr_val)
            {
                if(trim($attr_val) != '')
                    $newAttrArr[] = trim($attr_val);
            }
            $form_state['values'][$attr] = implode(",", $newAttrArr);
        }
        $form_state['values']['atr_'.$key] = $form_state['values'][$attr];
    }

    $updateId = updateRoleDetails($form, $form_state, $updateuserid, $callFromAPI = 1,$grpType);
    
    $total_no_of_users = getUserDetailList($grpId, 0, 0,'','',1);
    if($total_no_of_users <= $instant_count){
        if($is_admin == 1){
            grpUserDelete($grpId);
            grpUserInsert($grpId);
        }
        populateGroupUsersInFlatTable($grpId,'','DeleteGroup');
         
    }else{
        $back_ground = TRUE;
        if($is_admin == 1){
            grpUserDelete($grpId);
            $grp_details= getGrpDetails($grpId,array('code','name','status','is_admin','created_by'));
            //Default entry in admin groups table
            $user_add_query = 'INSERT INTO slt_admin_group_users ( group_id,group_code,user_id,status, created_by, created_on, updated_by, updated_on) VALUES ';
            $status = $grp_details['status'];
            $val_str .= '('.$grpId.', "'.$grp_details['code'].'", null, "'.$status.'", "'.$grp_details['created_by'].'", now(), '.$grp_details['created_by'].', now() )';
             
            //Insert query
            $insert_qry = $user_add_query.' '.$val_str;
            expDebug::dPrint("INSERT QUERY : ".$insert_qry , 4);
            $insert = db_query($insert_qry);
             
        }
        //mark the update action to be handled in backgroud cron job
        expDebug::dPrint('Inside else case where total count exceeds config limit:: '.$total_no_of_users,4);
        // Check certification monitor job status
        $jobname = 'group_bulk_user_upload';
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
        $inProgressJobs = fetchBackgroundListForGroups($cTStatus,'inlist',$grpId);
        expDebug::dPrint("inprogress job before insert >>> ". $inProgressJobs,5);
        if($inProgressJobs==0){
            $insertStmt = db_insert('slt_temp_background_jobs');
            $jobId = date('dmyHis');
            $insertStmt->fields(array(
                    'program_id' => $grpId,
                    'job_name' => 'GU-'.$jobId,
                    'created_by' => $updateuserid,
                    'created_on' => now(),
                    'updated_by' => $updateuserid,
                    'updated_on' => now(),
                    'custom1' => $total_no_of_users,
                    'custom2' => '',
                    'custom3' => '',
                    'custom4' => '',
                    'status' => 'new'
            ));
            expDebug::dPrintDBAPI("slt_temp_background_jobs check",$insertStmt,5);
            $insertStmt->execute();
             
            //Entry for securitymanagementAdmin
            $delete = db_delete('slt_group_user_mapping');
            $delete->condition('group_id',$grpId);
            expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $delete);
            $delete->execute();
             
        }
         
    
    }

    if($updateId) {
      return array((object)array('Id'=>$updateId));
    }
    else{
      //return array((object)array('Id'=>'Failure'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid Id.";
        return $errobj;
    }
  } catch (Exception $ex) {
    watchdog_exception('updateGroupByRestAPI', $ex);
    expertusErrorThrow($ex);
  }
}

function addUsersDetailsListByRestAPI(&$form_state,$createuserid="") {
  try{
    $mandatoryField = addEditCheckMandatoryFields($form, $form_state, array('group_name','user_name'));
    if($mandatoryField) {
      //return array((object)array('Id'=>$mandatoryField));
      $errobj=new stdClass();
      $errobj->isValidateError = 1;
      $errobj->errcode = 'L_012';
      $errobj->errormsg = $mandatoryField;
      return $errobj;
      exit;
    }
    $form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
    $grpName       = trim($form_state['values']['group_name']);
    $userName      = $form_state['values']['user_name'];
    $addOrRemove   = strtolower(trim($form_state[values][add_or_remove]));
    $operations    = array('add', 'remove', 'delete');

    if (!in_array($addOrRemove, $operations)) {
      //return array((object)array('Id' => 'Failure. Invalid operation key'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid operation key.";
        return $errobj;
    }
    if(!empty($grpName) && !empty($userName)) {
      $isGrpAvailableId = getIdForGroupName($grpName);
      if($isGrpAvailableId == 0) {
        //return array((object)array('Id' => 'Failure. Invalid GroupName-'.$grpName ));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid GroupName-".$grpName;
        return $errobj;
      }
      $explodedUserIdValue = array_map('trim', explode(',', $userName));
      foreach($explodedUserIdValue as $key => $value) {
        if(getIdForUserNameForGroup($value) == 0) {
          //return array((object)array('Id' => 'Failure. Invalid username-'.$value ));
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Invalid username-".$value;
            return $errobj;
        }
        else {
          $get_user_id[] =  getIdForUserNameForGroup($value);
        }
      }
      if($get_user_id) {
        $user_Id = implode(',',$get_user_id );
      }
    }
    unset($_REQUEST['id']);
    $uniqueId  = "grpAddUsers-$isGrpAvailableId-$user_Id";
    $form_state['values']['group_id'] = $isGrpAvailableId;
    $form_state['values']['user_id'] = $user_Id;
    $form_state['input']['hidden_idlist_'.$uniqueId] = $user_Id;
    $form_state['hidden_idlist_'.$uniqueId] = $user_Id;
    
    $checkemptygroup = objectLevelAccessCheckInGroupMap('cre_usr',array($user_Id));
    expDebug::dPrint(' check11 '.print_r($checkemptygroup, true),4);
    if(!empty($checkemptygroup)){
        $check=apiUseraccesscheck($createuserid,'cre_usr',$user_Id);
        expDebug::dPrint(' check1111 '.print_r($check, true),4);
        if(empty($check)) {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_013';
            $errobj->errormsg = "user dont have this privilege";
            return $errobj;
        }
        expDebug::dPrint(' check1111 '.print_r($check, true),4);
    }
    
    if($addOrRemove == 'add') {
      $addOrRemoveUsersResult = addUsersDetailsList($form, $form_state, $callByAPI=true);
    }
    if($addOrRemove == 'remove' || $addOrRemove == 'delete' ) {
        
      foreach($get_user_id as $key=>$value) {
        $alreadyRemovedUser =  explode(',', alreadyRemovedUser($isGrpAvailableId));
        if(in_array($value,$alreadyRemovedUser )) {
          $addOrRemoveUsersResult = 'User already removed';
        }
        else{
          $addOrRemoveUsersResult = updateRemoveUser($isGrpAvailableId,$value,$callByAPI = true);
        }
        if($addOrRemoveUsersResult == false) {
          $addOrRemoveUsersResult = 'Failure';
        }
      }
    }

    if($addOrRemoveUsersResult) {
      return array((object)array('Id'=>$addOrRemoveUsersResult));
    }
    else{
      //return array((object)array('Id'=>'Failure'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Error.";
        return $errobj;
    }
    syncSolrData('User');
    
  }catch (Exception $ex) {
    watchdog_exception('addUsersDetailsListByRestAPI', $ex);
    expertusErrorThrow($ex);
  }
}



/**
 *   This function used for API group list.
 */
function listOfAvailableGroupDetail(&$form_state,$createuserid="") {

    $start = $form_state[values][start];
    $limit   = $form_state[values][limit];
    if(isset($form_state[values][filter])) {
      foreach ($form_state[values][filter] as $key=>$value) {
        $_GET[$key] = $value;
      }
    }

  $_GET['check_all_state'] = true;
  $seletedGroup = fetchRolesAll('LISTS',$start,$limit, 'API');
  $groupAttributes = getGroupAttributes();
  $responseCode = array('ste_con_dep'=>'grpdep',
                'ste_con_ety'=>'grpempl',
                'ste_con_jbr'=>'grpjobrole',
                'ste_con_lng'=>'grplang',
                'ste_con_org'=>'grporg',
                'ste_con_rol'=>'role',
                'ste_con_usr'=>'grpusrtyp',
                'ste_con_cnt'=>'grpcontry',
                'ste_con_ste'=>'grploc',
                'ste_con_hdt'=>'hire_date'
  );
  if(!empty($seletedGroup)) {
    expDebug::dPrint(' listOfAvailableGroupDetailByRestAPI() $seletedGroup value '.print_r($seletedGroup, true),4);
    $cnt = '';
    foreach ($seletedGroup as $key => $value) {
        $cnt++;
      $response[$key]['Id'] = $value->id;
      $response[$key]['name'] = $value->name;
      $response[$key]['grpstatus'] = getProfileListItemName($value->status_code);
      $response[$key]['is_admin'] = $value->is_admin;
      if($response[$key]['is_admin']==1) {
      $userListQuery = fetchUserListToGroup($value->id, '','fullname');
      expDebug::dPrintDBAPI('users list query ' ,$userListQuery);
      $executeUserList = $userListQuery->execute()->fetchCol();
      $userslist = implode(',',$executeUserList);
      $response[$key]['userslist'] = $userslist;
      if(!empty($executeUserList)) {
        $userValue = groupDetailUserListForAPI($executeUserList);
            $response[$key]['users_list'] = $userslist;
      }
      else{
        $response[$key]['users_list'] ="";
      }
      }
      foreach($groupAttributes as $attr){
        $gid = $value->id;
        $selOpt[$gid][$attr->code] = getAttributeListByText($gid,$attr->code);
        if(count($selOpt[$gid][$attr->code]) != 0) {

          $response[$key][$responseCode[$attr->code]] = groupDetailListForAPI($selOpt[$gid][$attr->code]);

        }
        else {
          $response[$key][$responseCode[$attr->code]] = "";
        }
      }
    }
  }
 else {
   $response = "No values available";
 }
  $groupDetailListForAPI = groupDetailListForAPI($seletedGroup,$groupAttributes,$selOpt);
  expDebug::dPrint(' listOfAvailableGroupDetailByRestAPI() $response value '.print_r($response, true),4);
  //return array((object)array('msgString'=>$response));;
  $response['totalrow'] = fetchRolesAll('COUNT', '', '', 'API');
  return $response;
}


/**
 *   This function used for API group list.
 *   @$seletedGroup having group detail
 *   @$selOpt having group id with its attributes values
 *   This function merging two things as our requirement for group List api
 */
function groupDetailListForAPI($selOpt) {
  expDebug::dPrint(' entered into groupDetailListForAPI '.print_r($selOpt, true),4);
  $optional_array = array();
  foreach ($selOpt as $row) {
    if (isset($row->name)) {
      $optional_array[] = $row->name;
    }
  }
  return implode(',',$optional_array);
}

function groupDetailUserListForAPI($selOpt) {
  expDebug::dPrint(' entered into groupDetailUserListForAPI '.print_r($selOpt, true),4);
  $optional_array = array();
  foreach ($selOpt as $row) {
    if (isset($row->user_id)) {
      //$optional_array[] = 'Full Name :'.$row->fname.''.'ID:'.$row->user_id.''.',';
      $optional_array[] = $row->fname;
    }
  }
  return implode(',',$optional_array);
}


function is_numeric_array($array) {
  foreach ($array as $a=>$b) {
    if (!is_numeric($b)) {
      return false;
    }
  }
  return true;
}

/*
 * getIdForUserName - Get slt_person table id for $userName
*/
function getIdForUserNameForGroup($userName) {
  try{
    expDebug::dPrint(' $userName = ' . print_r($userName, true),4);
    // Select table
    $selectStmt = db_select('slt_person', 'per');
    // Set conditions
    $selectStmt->condition('per.user_name', $userName);
    $selectStmt->condition('per.status', 'cre_usr_sts_atv');
    // Select fields and/or add expressions
    $selectStmt->addField('per', 'id');
    expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
    // Execute query
    $result = $selectStmt->execute()->fetchField();
    $personId = empty($result)? 0 : $result;
    expDebug::dPrint(' returning $personId = ' . $personId,3);
    return $personId;
  }catch (Exception $ex) {
    watchdog_exception('getIdForUserName', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getIdForGroupName - Get slt_groups table id for $grpName
*/
function getIdForGroupName($grpName) {
  try{
    expDebug::dPrint(' $grpName = ' . print_r($grpName, true),4);
    // Select table
    $selectStmt = db_select('slt_groups', 'grp');
    // Set conditions
    $selectStmt->condition('grp.name', $grpName);
    //$selectStmt->condition('grp.status', 'cre_sec_sts_atv');

    // Select fields and/or add expressions
    $selectStmt->addField('grp', 'id');
    expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
    // Execute query
    $result = $selectStmt->execute()->fetchField();
    $groupId = empty($result)? 0 : $result;
    expDebug::dPrint(' returning $groupId = ' . $personId,3);
    return $groupId;
  }catch (Exception $ex) {
    watchdog_exception('getIdForGroupName', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getNameForGroupId - Get slt_groups table name for $grpId
*/
function getNameForGroupId($grpId) {
    try{
        expDebug::dPrint(' $grpId = ' . print_r($grpId, true),4);
        // Select table
        $selectStmt = db_select('slt_groups', 'grp');
        // Set conditions
        $selectStmt->condition('grp.id', $grpId);
        $selectStmt->condition('grp.status', 'cre_sec_sts_atv');

        // Select fields and/or add expressions
        $selectStmt->addField('grp', 'name');
        expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
        // Execute query
        $result = $selectStmt->execute()->fetchField();
        $groupName = empty($result)? 0 : $result;
        expDebug::dPrint(' returning $groupId = ' . $personId,3);
        return $groupName;
    }catch (Exception $ex) {
        watchdog_exception('getNameForGroupId', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getIdForUserName - Get slt_person table id for $userName
*/
function alreadyRemovedUser($grpId) {
  try{
    expDebug::dPrint(' $grpName = ' . print_r($grpName, true),4);
    // Select table
    $selectStmt = db_select('slt_groups', 'grp');
    // Set conditions
    $selectStmt->condition('grp.id', $grpId);
    $selectStmt->condition('grp.status', 'cre_sec_sts_atv');
    // Select fields and/or add expressions
    $selectStmt->addField('grp', 'removed_users');
    expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
    // Execute query
    $result = $selectStmt->execute()->fetchField();
    $removedUsers = empty($result)? 0 : $result;
    expDebug::dPrint(' returning $groupId = ' . $personId,3);
    return $removedUsers;
  }catch (Exception $ex) {
    watchdog_exception('getIdForGroupName', $ex);
    expertusErrorThrow($ex);
  }
}


function fetchGroupUserList($entityId, $entityType, $searchType = '', $searchKeyword = '', $eliminateCancel = '',  $exportOption =''){
    try{

        $searchKeyword= str_replace('|','/',$searchKeyword);
        $result = fetchUserListToGroup($entityId,$searchKeyword);
        if($exportOption=='CSV'){
            $colHeadersMap = array(t('LBL107') => 'fname',
                    t('LBL054') => 'uname',
                    t('LBL102') => 'status'
            );
        }
        $sord = $_SESSION['fetchUserListToGroup_sorting_order'];
        if(isset($sord)) {
            $result->orderBy("per.full_name",$sord);
        }

        expDebug::dPrint('$colHeadersMap : '.print_r($colHeadersMap , true));
        SendNarrowSearchResultsAsCSVFile($result, $colHeadersMap, 'export');
    }catch (Exception $ex) {
        watchdog_exception('fetchGroupUserList', $ex);
        expertusErrorThrow($ex);
    }
}//End fetchGroupUserList()

function listOfAvailableGroupDetailCSV(&$select) {
    try {
    $select->addExpression('IF(grp.department = \'All\', (select group_concat(name separator \', \') from slt_profile_list_items where code like \'cre_usr_dpt_%\' and is_active = \'Y\'), slf_getGroupAttributesNames(grp.department, \'Attributes\'))', 'grpdep');
    $select->addExpression('IF(grp.user_type = \'All\', (select group_concat(name separator \', \') from slt_profile_list_items where code like \'cre_usr_ptp_%\' and is_active = \'Y\'), slf_getGroupAttributesNames(grp.user_type, \'Attributes\'))', 'grpusrtyp');
    $select->addExpression('IF(grp.employment_type = \'All\', (select group_concat(name separator \', \') from slt_profile_list_items where code like \'cre_usr_etp_%\' and is_active = \'Y\'), slf_getGroupAttributesNames(grp.employment_type, \'Attributes\'))', 'grpempl');
    $select->addExpression('IF(grp.language = \'All\', (select group_concat(name separator \', \') from slt_profile_list_items where code like \'cre_sys_lng_%\' and is_active = \'Y\'), slf_getGroupAttributesNames(grp.language, \'Attributes\'))', 'grplang');
    $select->addExpression('IF(grp.job_role = \'All\', (select group_concat(name separator \', \') from slt_profile_list_items where code like \'cre_usr_jrl_%\' and is_active = \'Y\'), slf_getGroupAttributesNames(grp.job_role, \'Attributes\'))', 'grpjobrole');
    $select->addExpression('IF((grp.country = \'All\'), (SELECT group_concat(cont.country_name separator \', \') FROM slt_country cont), slf_getGroupAttributesNames(grp.country, \'Country\'))', 'grpcontry');
    $select->addExpression('IF((grp.state = \'All\'), (select GROUP_CONCAT(state_name separator \', \') FROM slt_state state where (grp.country=\'All\' or find_in_set(state.country_code, grp.country)>0)), slf_getGroupAttributesNames(grp.state, \'State\'))', 'grploc');
    $select->addExpression('IF((grp.org_id = \'All\'), (select group_concat(name separator \', \') from slt_organization where status = \'cre_org_sts_act\'), slf_getGroupAttributesNames(grp.org_id, \'Org\'))', 'grporg');
    $select->addExpression('IF(grp.is_manager = \'N\', IF(grp.is_instructor = \'Y\', \'Instructor\', \'\'), IF(grp.is_instructor = \'Y\', concat(\'Instructor\', \', \', \'Manager\'), \'Manager\'))', 'role');
    $select->addExpression('CONCAT(if(grpatt.on_or_after_start_date is not null ,concat(\'On or after : \',Date_Format(grpatt.on_or_after_start_date,\'%Y/%m/%d\')),\'\'),
if(grpatt.on_or_before_start_date is not null ,concat(if(grpatt.on_or_after_start_date is not null,\',\',\'\'),\'On or before : \',Date_Format(grpatt.on_or_before_start_date,\'%Y/%m/%d\')),\'\'),
if(grpatt.between_start_date is not null ,concat(if(grpatt.on_or_after_start_date is not null or grpatt.on_or_before_start_date is not null,\',\',\'\'),\'Between : \',Date_Format(grpatt.between_start_date,\'%Y/%m/%d\') ,\' and \',Date_Format(grpatt.between_end_date,\'%Y/%m/%d\')),\'\'))','hire_date');
    $select->leftJoin('slt_profile_list_items', 'plistatus', 'grp.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
    $select->addField('plistatus', 'name', 'grpstatus');
    expDebug::dPrintDBAPI('fetchRolesAll select groups ' , $select);
    $result_set = $select->execute()->fetchAll(PDO::FETCH_ASSOC);
    return $result_set;
    
  /* expDebug::dPrint(' listOfAvailableGroupDetailCSV() $getValues value '.print_r($getValues, true),4);
  $start = 0;
  $limit = fetchRolesAll('COUNT');
  expDebug::dPrint(' $numRole = ' . print_r($numRole, true) , 4);
  foreach ($getValues as $key=>$value) {
    $_GET[$key] = $value;
  }
  //$_GET['check_all_state'] = true;
  expDebug::dPrint(' $_GET value in listOfAvailableGroupDetailCSV = ' . print_r($_GET, true) , 4);
  $seletedGroup = fetchRolesAll('LISTS',$start, $limit);
  $groupAttributes = getGroupAttributes();
  expDebug::dPrint(' listOfAvailableGroupDetailByRestAPI() $seletedGroup value '.print_r($seletedGroup, true),4);
  $responseCode = array('ste_con_dep'=>'grpdep',
      'ste_con_ety'=>'grpempl',
      'ste_con_jbr'=>'grpjobrole',
      'ste_con_lng'=>'grplang',
      'ste_con_org'=>'grporg',
      'ste_con_rol'=>'role',
      'ste_con_usr'=>'grpusrtyp',
      'ste_con_cnt'=>'grpcontry',
      'ste_con_ste'=>'grploc');
  if(!empty($seletedGroup)) {
    foreach ($seletedGroup as $key => $value) {
      $response[$key]['Id'] = $value->id;
      $response[$key]['name'] = $value->name;
      $response[$key]['grpstatus'] = getProfileListItemName($value->status_code);
      //$userListQuery = fetchUserListToGroup($value->id, '','fullname');
      //expDebug::dPrintDBAPI('users list query ' ,$userListQuery);
      //$executeUserList = $userListQuery->execute()->fetchCol();
      //$userslist = implode(',',$executeUserList);
      //$response[$key]['userslist'] = $userslist;
      foreach($groupAttributes as $attr){
        $gid = $value->id;
        $selOpt[$gid][$attr->code] = getAttributeListByText($gid,$attr->code);
        if(count($selOpt[$gid][$attr->code]) != 0) {
          $response[$key][$responseCode[$attr->code]] = groupDetailListForAPI($selOpt[$gid][$attr->code]);
        }
        else {
          $response[$key][$responseCode[$attr->code]] = "";
        }
      }
    }
  }
  expDebug::dPrint(' listOfAvailableGroupDetailCSV Attribute set '. print_r($groupAttributes,true) , 5);
 // $groupDetailListForAPI = groupDetailListForAPI($seletedGroup,$groupAttributes,$selOpt);
  expDebug::dPrint(' listOfAvailableGroupDetailByRestAPI() $response value '.print_r($response, true),4);
  return $response; */
    } catch (Exception $ex) {
        watchdog_exception('listOfAvailableGroupDetailCSV', $ex);
        expertusErrorThrow($ex);
    }
}

function profileListTextQuery($code,$type,$gid,$searchText){
    $searchText =  $_GET['z'];
     // Select the table
  $select = db_select('slt_profile_list_items', 'pl');

  // Add fields to fetch
  $select->addField('pl', 'name', 'name');

  // Add condition
  // change by ayyappans for 42903: If the attributes is selected as "All", then while filtering using list groups through API response shows incorrectly
  $select->condition('pl.code', '%' . db_like($code) . '%', 'LIKE');
  if($gid===''){
      $select->condition('pl.name', '%' . db_like($searchText) . '%', 'LIKE');
  }else{
    $select->addField('pl','code','code');
    $select->leftJoin('slt_groups','grp','(grp.'.$type.'=\'All\' or find_in_set(pl.code,grp.'.$type.')>0) and grp.id=:gid',array('gid'=>$gid));
    $select->addExpression('if(grp.'.$type.'=\'All\',1,0)','allsel');
    $select->condition('grp.id',$gid,'=');
  }
  
  filterByUserAccessPrivileges($select, 'pl','',1,substr($code, 0, -1),'','pl');
  $select->condition('pl.is_active', 'Y');
  // Order in dictionary sort order
  $select->orderBy('name');
  return $select;
}
/*
function attachOwnersDetails($form, &$form_state){
    try {expDebug::dPrint("test");
        include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
        $entityId = $form_state['triggering_element']['#ajax']['role_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
         
        expDebug::dPrint(' $entityId '.$entityId.' $entityType '.$entityType,4);
        $form_state = array(
                'no_redirect'  => TRUE, // To confirm whether necessary
                'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
                'entity_id'    => $entityId,
                'entity_type'  => $entityType,
                'call_from'    => 'add_owner'
        );

        $commands = array();
        $commands[] = exp_ctools_modal_adjust();
        $commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
        $builtForm            = drupal_build_form('addowner_addedit_form_html', $form_state);
        $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
        return array('#type' => 'ajax', '#commands' => $commands);
         
//      $formId                 = 'addusr_addedit_form_html';
//       $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
//      $renderedTabContentHTML = drupal_render($builtForm);
//      $drupalSettings         = getDrupalSettings();

//      $addUsersResponse = array(
//              'drupal_settings'     => $drupalSettings,
//              'render_content_main' => $renderedTabContentHTML,
//      );
         
//      echo drupal_json_output($addUsersResponse);
//      exit;
         
    } catch (Exception $ex) {
        watchdog_exception('attachUsersDetails', $ex);
        expertusErrorThrow($ex);
    }

}
*/
/* function showAttachOwnersToRole($form, &$form_state){
    try {
        include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
        $entityId = $form_state['triggering_element']['#ajax']['role_id'];
        $entityType = 'cre_sec';

        expDebug::dPrint(' $entityId '.$entityId.' $entityType '.$entityType ,4);
        $form_state = array(
                'no_redirect'  => TRUE,
                'build_info'   => array('args' => array('ajax')),
                'role_id'      => $entityId,
                'entity_type'  => $entityType,
                // 'form_arg_type' => $argType
            'mode'         => 'view'
        );

        $commands = array();
        $commands[] = exp_ctools_modal_adjust();
        $commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($entityId));
        $builtForm  = drupal_build_form('display_assigned_owners', $form_state);
        $commands[] = ajax_command_refresh_narrow_search_results('onclose');
        $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
         
        return array('#type' => 'ajax', '#commands' => $commands);
    } catch (Exception $ex) {
        watchdog_exception('showAttachOwnersToRole', $ex);
        expertusErrorThrow($ex);
    }
} */
/*
function addowner_addedit_form_html($form,&$form_state) {
    try {   
        return displayAddOwnerDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type']);
    } catch (Exception $ex) {
        watchdog_exception('addowner_addedit_form_html', $ex);
        expertusErrorThrow($ex);
    }

}

function displayAddOwnerDetails($form,&$form_state,$roleId, $userId){
    try {       
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
        $call_from    = $form_state['call_from'];
        $role_id = $form_state['entity_id'];
    
        $entityType = 'cre_sec';
        $userId= $form_state['user_id'];
        $excludedRoleId = '';
        $uniqueId = "grpAddOwners-".$role_id."-".$userId;
        $wrapperId = 'security-role-detail-wrapper';

        $displayMode = 'edit';
        $form_state['display_mode'] = $displayMode;
        $form = array();
        $form['#prefix'] = 
//                (stripos($_SERVER['HTTP_USER_AGENT'], '   rome') !== false
//              || (stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox') !== false)? '</form>' : '' )
                '</form>'.'<div id="security-role-detail-wrapper" class="owner-user-class">';
        $form['#suffix'] = '</div>';
        $setminHight =  'admin-add-users-tab-datagrid-wrapper';
        $form['edit-class-details-start'] = array(
          '#prefix' => "<div id='add-user-role-page' class = 'add-users-grp'>",
        );
        
        $type='grpAddOwners';
        $listquery = fetchGrpOwnerUserList($role_id,'',$type);
        expDebug::dPrintDBAPI('listquery', $listquery);
    //  $count = $listquery[0]->cnt;
     expDebug::dPrint("call_from -- >".$call_from);
        if(isset($addAttributes))
            unset($addAttributes['onclick']);
        $addAttributes['class'][] = 'narrow-search-actionbar-orange-btnBG';
        if($call_from=='add_owner'){
            $addAttributes['data-wrapperid'] = array('role-detail-wrapper');
        }else{
            $addAttributes['data-wrapperid'] = array('security-role-detail-wrapper');
        }
        $form['add-class-list'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="catalog-class-addedit-form-details"><div id="add-class-list-wrapper">'
        );
//      hidden input type to carry the selected users
        $form['hidden_idlist'] = array(
                '#type' => 'hidden',
                '#name' => 'hidden_idlist_'.$uniqueId,
                '#id' => 'datagrid-idlist-'.$uniqueId,
        );
        $form['hidden_group_id'] = array(
                '#type' => 'hidden',
                '#name' => 'hidden_group_id',
                '#value' => $role_id,       
        );
        $paintMultiAction  = "<div id='search-list-class-title-keyword' class='search-list-keyword' style='display:block;'>";
        $paintMultiAction .= "<span id='search-dropdwn-list-class'>";
        $paintMultiAction .= "<input type='hidden' id='search_all_user_type-hidden' value='usrtit' />";
        $paintMultiAction .= "<input type='hidden' id='addusername-search-autocomplete_hidden' value='".t('LBL181')."' />";
        $paintMultiAction .= "<span id='select-list-class-dropdown-adduser' class='select-list-dropdown-class'>".t('LBL054')."</span>";
        $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-class-dropdown-link dropdown-link-font ' onclick='$(\"#root-admin\").data(\"peoplegroup\").moreRoleHideShow();'> </a>";
        $paintMultiAction .= "<ul id='select-list-class-dropdown-list' class='more-drop-down'>";
        $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreUserSearchTypeText('".t('LBL054')."','usrtitle','role');\">".t('LBL054')."</li>";
        $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreUserSearchTypeText('".t('Groups')."','grptitle','role');\">".t('Groups')."</li>";
        $paintMultiAction .= "</ul>";
        $paintMultiAction .= "</span></div>";

        $form['class_multi_search_container'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="role_multi_search_container_add">', //class="admin_class_multi_search_container"
        );
        $form['more_add_enrolluser_search'] = array(
                '#type' => 'markup',
                '#markup' => $paintMultiAction,
        );
        $form['class_auto-search'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="admin-class-auto-search-container">',
        );
        $userSearchType = '';
        $userSearchType = $form_state['storage']['search_usr_type'];
        $searchAjaxEvent = 'click';

        $acMenuPath = 'administration/people/group/owner-name-autocomplete/'.$role_id.'/'.'addusers/grpAddOwners';
         
        $excludedRoleId =0;
        $default_text_title = t('LBL181');
        $autoCompleteAttributes['class'][] = 'admin_class_ac_input_mainform';
        $autoCompleteAttributes['onkeypress'][]   = 'disableEnrterKey(event);'; // added by vincent for prevent the enter key submit in search box
        addACFieldForMultiSelect($form, 'addusername-search-autocomplete', $acMenuPath, '', '', '', array(), array('entity_id'  =>  $role_id, 'entity_type' => "'".$entityType."'" ,'search_type' => "$('#search_all_user_type-hidden').val()",'excluded_role_id' => "'".$excludedRoleId."'", 'id_required' => 0), false, false, $default_text_title,$autoCompleteAttributes);
        $form['class_auto-search-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>',
        );
         
        $form['tip-position'] = array(
                '#type' => 'markup',
                '#markup' => '<input type="hidden" id="qtip_position" name="qtip_position" value="">',
        );

        $excludedRoleId =0;
        $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("edit", "grpAddOwners", $(\'#addusername-search-autocomplete\').val(), "'.$role_id.'", "'.$userId.'", "'.$excludedRoleId .'"); return false;';
        $searchAttributes['Class'][] = 'admin-pagination-search-go narrow-text-search';
        $form['search_addusers'] = array(
                '#type' => 'submit',
                '#id' => 'search-addusers',
                '#title' => t('LBL304'), //Search
                '#name' => 'Search addusers',
                '#submit' => array('search_addusers'), // No such function search_attachcourse(), apparently property is ignored.
                '#attributes' => $searchAttributes,
        );
        $form['class_multi_search_container-end'] = array(
                '#type' => 'markup',
                '#markup' => '<div class= "filter-search-start-date-right-bg"></div></div>',
        );
        $form['edit-class-details-paginate'] = array(
                '#type' => 'markup',
                '#prefix' => '<div id="admin-add-course-training-plan" class="display_user_list"><div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
                '#suffix' => '<div>',
        );
        $form['role_user_view_grid_markup'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common" style="display:none;"></div></div>',
        );
        $form['role_user_grid_load'] = array(
          '#type' => 'markup',
                //  '#prefix' => '<div style="display:block"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'view\', \'RoleDisplayUsers\', \'\', \''.$role_id.'\', \''.$entityType.'\', \''.$excludedRoleId.'\');" width="100" height="132" /></div>',
          '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'grpAddOwners\', $(\'#addusername-search-autocomplete\').val(), \''.$role_id.'\', \''.$userId.'\',\'0\');" width="100" height="132" /></div>',
        );
        $form['role_user_grid_load_end'] = array(
          '#type' => 'markup',
          '#prefix' => '</div></div>',
        );

        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
        if($call_from=='add_user'){
            $cancelAttributes['onclick'][]         =  "closeQtip('','$role_id'); return false;"; //($call_from=='add_user') ? "$('.qtip-active').remove();return false;" : "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
        }else{
            $cancelAttributes['onclick'][]         =  "closeQtip('','$role_id'); return false;"; //($call_from=='add_user') ? "$('.qtip-active').remove();return false;" : "$('#root-admin').data('narrowsearch').removeActiveQtip(); return false;";
        }
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
         
        $form['cancel_adduser_edit_mode'] = array(
                '#type' => 'submit',
                '#value' => t('LBL123'),
                '#title' => t('LBL123'),
                '#prefix' => '<div id="add_owner" ><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
                '#suffix' =>   '<div class="white-btn-bg-right"></div>',
                '#name' => 'Cancel addusers',
                '#attributes' => $cancelAttributes,
        );
        module_load_include('inc', 'exp_sp_administration_learning', 'exp_sp_administration_learning');
    
        if($call_from=='add_user'){
            $callback = 'addOwnersCallBack';
            $wrapper='role-detail-wrapper';
            $setAttributes['data-wrapperid'] = 'role-detail-wrapper';
        }else{
            $callback = 'showAttachOwnersToRole';
            $wrapper = 'security-role-detail-wrapper';
            $setAttributes['data-wrapperid'] = 'security-role-detail-wrapper';
        }
        if(count($listquery) != 0){
            expDebug::dPrint('count($listquery)'.count($listquery), 4);
            $setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
            $form['set_attachcourse_edit_mode'] = array(
                    '#type' => 'submit',
                    '#prefix' => '<div class="admin-save-button-container role-add-user-btn"><div class="admin-save-button-left-bg"></div>',
                    '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div></div>',
                    '#title' => t('LBL1038'),
                    '#value' => t('LBL287'),
                    '#name' => t('LBL287'),
                    '#submit' => array('addOwnersDetailsList'),
                    '#attributes' => $setAttributes,
                    '#ajax' => array(
                            'wrapper' => $wrapper,
                            'callback' => $callback,
                            'role_id' =>$role_id,
                            'user_id' =>$userId,
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
        }
        return $form;
    } catch (Exception $ex) {
        watchdog_exception('displayAddOwnerDetails', $ex);
        expertusErrorThrow($ex);
    }
}
*/
/*
function addOwnerDetails($entityId, $entityType){
    try {
        expDebug::dPrint('addUsersToRole'.$entityId,4);
        $form_state = array(
                'no_redirect'  => TRUE,
                'build_info'   => array('args' => array('ajax')),
                'role_id'    => $entityId,
                'entity_type'  => $entityType,
                //   'form_arg_type' => $argType
            'mode'               => 'view'
        );
        expDebug::dPrint('checking for calling permissions'.$entityId.'flow of'.$entityType,4);
        $formId                 = 'display_assigned_owners';
        $builtForm              = drupal_build_form($formId, $form_state);
        $renderedTabContentHTML = drupal_render($builtForm);
        $drupalSettings         = getDrupalSettings();

        $response       = array(
                'drupal_settings'     => $drupalSettings,
                'render_content_main' => $renderedTabContentHTML,
        );

        echo drupal_json_output($response);
        exit;
    } catch (Exception $ex) {
        watchdog_exception('attachUserToRole', $ex);
        expertusErrorThrow($ex);
    }
}
/*
function getOwnernameAutocompletesearch($grpId,$modId,$type){
    try{
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
        include_once(drupal_get_path('module', 'exp_sp_core') .'/exp_sp_core.inc');
        $userName = $_GET['z'];
        $searchType = $_GET['search_type'];
        expDebug::dPrint("Type------>".$type);
        expDebug::dPrint("Search Type------>".$searchType);
        $select = fetchGrpOwnerUserList($grpId,$userName,$type,'',$searchType,'autosearch');
        expDebug::dPrintDBAPI("asdsads",$select);
        $matchingUserNames = $select->execute()->fetchAll();
    
        foreach ($matchingUserNames as $userName) {
            if($searchType == 'fultit')
                print $userName->fname . "\n";
            else if($searchType == 'grptitle')
                print $userName->gname . "\n";
            else 
                print $userName->uname . "\n";
            }
        
    }catch(Exception $ex) {
            watchdog_exception('getOwnernameAutocompletesearch', $ex);
            expertusErrorThrow($ex);
        }   
}

function updateRemoveOwner($grpId,$usrId, $callByAPI=false){
    try{
        expDebug::dPrint("group id --> user id -->".$grpId."   ".$usrId);
        $deleteuser = db_delete('slt_owner_details');
        $deleteuser->condition('group_id',$grpId);
        $deleteuser->condition('owner_id',$usrId);
        expDebug::dPrintDBAPI('delete Owner -----> ' , $deleteuser);
        $deleteuser->execute();
        
        $loggedUserId = getSltpersonUserId();
        //Insert group_audit to track added owners
        $insertStmt = db_insert('slt_group_audit');
        $fields = array(
                'group_id'        => $grpId,
                'user_id'         => $usrId,
                'operation'       => 'ownerremoved',
                'criteria_match'  => NULL,
                'updated_by'      => $loggedUserId,
                'updated_on'      => now()
        );
        $insertStmt->fields($fields);
        $insertStmt->execute();
        
        //Check whether user is already a member of the group
        $exist_query = db_select('slt_groups');
        $exist_query->addExpression('count(1)');
        $exist_query->condition('id', $grpId, '=');
        $exist_query->where('FIND_IN_SET('.$usrId.',userslist)>0');
        expDebug::dPrintDBAPI('Users exists in group or not ' , $exist_query);
        $exist_or_not =$exist_query->execute()->fetchField();
        expDebug::dPrint('exists or not check '. $exist_or_not);
        
        if($exist_or_not == 0){
            // Delete from drupal table 
            $role_id = getDrupalRoleId($grpId);
            $uId = getDrupalIdOfUser($usrId);
            $deleteuser = db_delete('users_roles');
            $deleteuser->condition('uid',$uId);
            $deleteuser->condition('rid',$role_id);
            expDebug::dPrintDBAPI('delete ownerlist ' , $deleteuser);
            $deleteuser->execute();
        }
    }catch (Exception $ex) {
        watchdog_exception('updateRemoveOwner', $ex);
        expertusErrorThrow($ex);
    }
}

function fetchGroupOwnerUserList($entityId, $entityType, $searchType = '', $searchKeyword = '', $eliminateCancel = '',  $exportOption =''){
    try{
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
        expDebug::dPrint("testing the export url",5);
        $searchKeyword= str_replace('|','/',$searchKeyword);
        $result = fetchGrpOwnerUserList($entityId,$searchKeyword,'RoleDisplayOwners','',$searchType);
        if($exportOption=='CSV'){
            $colHeadersMap = array(t('LBL107') => 'fname',
                    t('LBL054') => 'uname',
                    t('Groups') => 'gname'
            );
        }
        $sord = $_SESSION['fetchUserListToGroup_sorting_order'];
        if(isset($sord)) {
            $result->orderBy("per.full_name",$sord);
        }
        expDebug::dPrintDBAPI('test export ', $result);
        expDebug::dPrint('$colHeadersMap : '.print_r($colHeadersMap , true));
        SendNarrowSearchResultsAsCSVFile($result, $colHeadersMap, 'export');
    }catch (Exception $ex) {
        watchdog_exception('fetchGroupUserList', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * returns the count of default/OOB groups matched for the given $group_id
 * @param unknown $group_id
 * @return $defaultgrpCount
 */
function fethDefaultGroup($group_id){
    try{
        $array = array("grp_ins", "grp_mgr", "grp_adm", "grp_sup");
        $sel = db_select("slt_groups", "grp");
        $sel->addExpression("COUNT(grp.id)","default_group");
        $sel->condition('grp.id', $group_id);
        $sel->condition('grp.code',$array,'IN');
        expDebug::dPrintDBAPI("fethDefaultGroup------>",$sel);
        $defaultgrpCount = $sel->execute()->fetchField();
        return $defaultgrpCount;
    }catch(Exception $e){
        watchdog_exception('fethDefaultGroup', $e);
        expertusErrorThrow($e);
    }
}

/*
 * 
 *Check whether user is a owner of the group
 * 
 * */
 
/* function ownerExistOrNot($grpId){
        $exist_query = db_select('slt_owner_details');
        $exist_query->addExpression('count(1)');
        $exist_query->condition('group_id', $grpId, '=');
        $exist_query->condition('owner_id',array(1,2),'NOT IN');
        expDebug::dPrintDBAPI('Users exists in group or not ' , $exist_query);
        $exist_or_not =$exist_query->execute()->fetchField();
        return $exist_or_not;
} */

function formatHireDate($date = '',$from = '') {
    try {
        if ($date == '') 
            return '';
        
        $date = str_replace("-","/", $date);
        if(empty($from))
            $result= date_format(date_create($date),'Y-m-d');
        else 
            $result= date_format(date_create($date),'m-d-Y');
        return $result;
    } catch(Exception $e) {
        
    }
}

function getHireDetails($gId){
    try {
        $select = db_select('slt_groups', 'grp');
        $select->leftjoin('slt_group_attributes','attr','attr.group_id = grp.id');
        $select->addField('attr', 'on_or_after_start_date', 'on_or_after_start_date');
        $select->addField('attr', 'on_or_before_start_date', 'on_or_before_start_date');
        $select->addField('attr', 'between_start_date', 'between_start_date');
        $select->addField('attr', 'between_end_date', 'between_end_date');
        $select->condition('grp.id',$gId,'=');
        $select->condition('attr.attr_type','HireDate','=');
        expDebug::dPrintDBAPI(' $select getHireDetails = ' , $select);
        $rtn = $select->execute()->fetchAll();
        expDebug::dPrint('checking for getHireDetails'.print_r($rtn,true),4);
        return $rtn;
        
    } catch(Exception $e) {
        
    }   
}

function fetchBackgroundListForGroups($status,$condition='equals',$grpId=0,$jobName = 'GU-'){
    try {
        $select = db_select('slt_temp_background_jobs','tpj');
        if($condition == 'inlist'){
            $select->addExpression('count(tpj.id)','cnt');
            $select->condition('tpj.status', $status, 'IN');
        }elseif($condition == 'equals'){
            $select->addField('tpj','id','job_id');
            $select->addField('tpj','program_id','program_id');
            $select->addField('tpj','job_name','job_name');
            $select->addField('tpj','custom1');
            $select->addField('tpj','custom2');
            $select->addField('tpj','custom3');
            $select->addField('tpj','custom4');
            $select->addField('tpj','created_by');
            $select->condition('tpj.status', $status, '=');
        }
        if($grpId!=0){
            $select->condition('tpj.program_id', $grpId , '=');
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

/*
 * Back ground process for group bulk user upload
 * 
 * */
function batchGUProcess(){
    try{
        $limit = 15000;
        $st = microtime(true);
        $jobDetails = fetchBackgroundListForGroups('inprogress');
        expDebug::dPrint("fetchCTBackgroundList in progress::".print_r($jobDetails,1),4);
        if(count($jobDetails) == 0){
            // no jobs in progress
            expDebug::dPrint("Inside batch process called ::",4);
            $jobDetails = fetchBackgroundListForGroups('new');
            expDebug::dPrint("fetchBackgroundListForGroups in new::".print_r($jobDetails,1),4);
            if(count($jobDetails) == 0){
                // No jobs to be processed
                $jobGUName = 'group_bulk_user_upload';
                $checkCTStatus     = check_monitor_jobs_status($jobGUName);
                if($checkCTStatus['custom0']==1){
                    // Update monitoring jobs table to turn off the handle
                    $updatejobCTName = db_update('slt_monitoring_jobs');
                    $updatejobCTName->fields(array(
                            'custom0'=>0,
                            'updated_on' => now()
                    ));
                    $updatejobCTName->condition('job_name',$jobGUName,'=');
                    $updatejobCTName->execute();
                }
                return;
            }
        }
        $GrpId = $jobDetails[0]->program_id;
        $remarks = $jobDetails[0]->custom3;
        
        //Flat table population 

        $select  = db_select('slt_groups','grp');
        $select->distinct();
        $select->Join('slt_person','per');
        $select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
        $select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
        $select->addField('per','id','user_id');
        $select->addField('per','status','user_status');
        $select->addField('grp','id','group_id');
        $select->addField('grp','is_admin','group_type');
        $select->addField('grp','status','group_status');
        $select->addExpression('IF(FIND_IN_SET(per.id, grp.added_users)>0,"A","M")', 'user_type');
        $select->addField('grp','created_by','created_by');
        $select->addField('grp','created_on','created_on');
        $select->addField('grp','updated_by','updated_by');
        $select->addField('grp','updated_on','updated_on');
        $select->condition('grp.id',$GrpId,'=');
        $select->where('per.id NOT IN (1,2)
                          AND grp.status != \'cre_sec_sts_del\'
                          AND per.status != \'cre_usr_sts_del\'
    AND 
    if((grp.is_admin = 1 and (grp.org_id is null and grp.user_type is null and grp.employment_type is null and grp.country is null and grp.state is null and grp.department is null and grp.job_role is null and grp.language is null and grp.is_manager=\'N\' AND grp.is_instructor=\'N\' AND grpatt.id is null)),
    ((grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0) AND FIND_IN_SET(per.id,grp.added_users)>0),
    ((((if(grp.org_id=\'All\',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
                                AND (if(grp.user_type=\'All\',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,\'\'),grp.user_type)>0 ))
                                AND (if(grp.employment_type=\'All\',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,\'\'),grp.employment_type)>0))
                                AND (if(grp.country=\'All\',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,\'\'),grp.country)>0))
                                AND (if(grp.state=\'All\',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,\'-\',per.state),\'\'),grp.state)>0))
                                AND (if(grp.department=\'All\',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,\'\'),grp.department)>0))
                                AND (if(grp.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,\'\'),ifnull(grp.job_role,\'\'))>0))
                                AND (if(grp.language=\'All\',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,\'\'),grp.language)>0))
                                AND (CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\')
                                  THEN
                                  (ifnull(per.is_manager,\'N\') = grp.is_manager or ifnull(per.is_instructor,\'N\') = grp.is_instructor)
                                  WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\')
                                  THEN
                                  (ifnull(per.is_manager,\'N\') = grp.is_manager)
                                  WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\')
                                  THEN
                                  (ifnull(per.is_instructor,\'N\') = grp.is_instructor)
                                  ELSE
                                  1=1
                                  END)
                                    AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR
                        if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
                        if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1))
                                AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)) 
    ))
                                AND NOT EXISTS( select map.user_id,map.group_id from slt_group_user_mapping map where map.group_id = '.$GrpId.' and map.user_id = per.id)');
        
        
        $select->range(0,$limit);
        $grpIns = db_insert('slt_group_user_mapping');
        $grpIns->from($select);
        expDebug::dPrintDBAPI('Select query for populate user list in slt_group_user_mapping ',$select);
        
        if(empty($remarks)) {
        $itr_start_time = microtime(true);
        } else {
        $itr_start_time = microtime(true)."__".$remarks;
        }
        // Update the query execution mins in custom4 once process is in running state
        $update = db_update ( 'slt_temp_background_jobs' );
        $update->fields ( array (
            'status' => "running",
            'custom3' => $itr_start_time
         ) );
        $update->condition ( 'id', $jobDetails[0]->job_id, '=' );
        expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update in running state 111 ---> ", $update ,5);
        $update->execute();
         
        expDebug::dPrintDBAPI('Insert query for populate user list in slt_group_user_mapping ',$grpIns);
        $grpIns->execute();        
       
        //Removed users insert
        $sel  = db_select('slt_groups','grp');
        $sel->Join('slt_person','per');
        $sel->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
        $sel->addField('per','id','user_id');
        $sel->addField('per','status','user_status');
        $sel->addField('grp','id','group_id');
        $sel->addField('grp','is_admin','group_type');
        $sel->addField('grp','status','group_status');
        $sel->addExpression('"R"', 'user_type');
        $sel->addField('grp','created_by','created_by');
        $sel->addField('grp','created_on','created_on');
        $sel->addField('grp','updated_by','updated_by');
        $sel->addField('grp','updated_on','updated_on');
        $sel->condition('grp.id',$GrpId,'=');
        $sel->where('FIND_IN_SET(per.id, grp.removed_users)>0 AND NOT EXISTS( select map.user_id,map.group_id from slt_group_user_mapping map where map.group_id = '.$GrpId.' and map.user_id = per.id)');
            
        $grpRmv = db_insert('slt_group_user_mapping');
        $grpRmv->from($sel);
        expDebug::dPrintDBAPI('Select query for populate user list in slt_group_user_mapping ',$sel);
        expDebug::dPrintDBAPI('Insert query for populate user list in slt_group_user_mapping ',$grpRmv);
        $grpRmv->execute();        
       
    
        $is_admin = adminGrporNot($GrpId);
        if($is_admin == 1){
            
            
            //populate users in admin groups
            $selStmt = db_select('slt_group_user_mapping','map');
            $selStmt->distinct();
            $selStmt->leftJoin('slt_groups','grp','grp.id = map.group_id');
            $selStmt->addField('map','group_id','group_id');
            $selStmt->addField('grp','code','code');
            $selStmt->addField('map','user_id','user_id');
            $selStmt->addField('grp','status','status');
            $selStmt->addField('grp','created_by','created_by');
            $selStmt->addField('grp','created_on','created_on');
            $selStmt->addField('grp','updated_by','updated_by');
            $selStmt->addField('grp','updated_on','updated_on');
            $selStmt->condition('map.user_type','R','!=');
            $selStmt->where('map.group_id = '.$GrpId.' AND NOT EXISTS( select map.user_id,map.group_id from slt_admin_group_users adm where adm.group_id = '.$GrpId.' and adm.user_id = map.user_id)');
            $selStmt->range(0,$limit);
            $grpUsrIns = db_insert('slt_admin_group_users');
            $grpUsrIns->from($selStmt);
            $grpUsrIns->fields(array('group_id','group_code','user_id','status', 'created_by', 'created_on', 'updated_by', 'updated_on'));
            expDebug::dPrintDBAPI('Select query for populate user list in slt_admin_group_users ',$selStmt);
            expDebug::dPrintDBAPI('Insert query for populate user list in slt_admin_group_users ',$grpUsrIns);
            $grpUsrIns->execute();
            
            $role_id = getDrupalRoleId($GrpId);
            
            //users role insert
            
            $selectStmt  = db_select('slt_group_user_mapping','map');
            $selectStmt->distinct();
            $selectStmt->leftJoin('slt_groups','grp','grp.id = map.group_id');
            $selectStmt->leftJoin('role','rl','rl.name = grp.name');
            $selectStmt->leftJoin('slt_person','per','map.user_id = per.id');
            $selectStmt->leftJoin('users','usr','usr.name = per.user_name');
            $selectStmt->addField('usr','uid','uid');
            $selectStmt->addField('rl','rid','rid');
            $selectStmt->condition('grp.id',$GrpId,'=');
            $selectStmt->condition('usr.uid','','IS NOT NULL');
            $selectStmt->condition('map.user_type','R','!=');
            $selectStmt->where('map.group_id = '.$GrpId.' AND NOT EXISTS( select uid,rid from users_roles usrl where usrl.rid = '.$role_id.' and usrl.uid = usr.uid)');
            $selectStmt->range(0,$limit);
            $rolIns = db_insert('users_roles');
            $rolIns->from($selectStmt);
            expDebug::dPrintDBAPI('Select query for populate user list in users_roles ',$selectStmt);
            expDebug::dPrintDBAPI('Insert query for populate user list in users_roles ',$rolIns);
            $rolIns->execute();
            
            //grpUserInsert($GrpId);
            
        }
            //populateGroupUsersInFlatTable($GrpId,'','DeleteGroup');
        $total_result = $jobDetails[0]->custom1;
        $records = ($jobDetails[0]->custom2)+$limit;
        if ($records < $total_result) {
            $status = "inprogress";
            // Update the status once process is in progress
            $update = db_update ( 'slt_temp_background_jobs' );
            $update->fields ( array (
                    'status' => $status,
                    'custom2' => $records,
                    'custom3' => '',
                    'updated_on' => now()
            ) );
            $update->condition ( 'id', $jobDetails[0]->job_id, '=' );
            expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update in progress", $update ,2);
            $update->execute();
        }  // update status in progress
        else {
            
            if($is_admin == 1){
               // Final update for userslist    
                $selstmt = db_select('slt_group_user_mapping', 'grp');
                $selstmt->addExpression('GROUP_CONCAT(distinct grp.user_id)','userids');
                $selstmt->condition('grp.user_type','R','!=');
                $selstmt->condition('grp.group_id',$GrpId,'=');
                expDebug::dPrintDBAPI(' $select getUserDetails = ' , $selstmt);
                $plist = $selstmt->execute()->fetchField();
                
                $updateStmt = db_update('slt_groups');
                $updateStmt->condition('id', $GrpId);
                $updateStmt->fields(array('userslist' => $plist));
                $Updatedcode = $updateStmt->execute();
            }
            
            $status = "completed";
            // Update the status once process is in progress
            $update = db_update('slt_temp_background_jobs');
            $update->fields (array (
                    'status' => $status,
                    'updated_on' => now(),
                    'custom2' => $total_result,
                    'custom3' => '',
            ));
            $update->condition('id', $jobDetails[0]->job_id, '=');
            expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update completed", $update );
            $update->execute();
        
            $total_record = $jobDetails[0]->custom1;
            $success_record = $total_result;
            $failure_record = ($jobDetails[0]->custom1 - $success_record);
            // Send mail to the job initiator begins
            $created_by = $jobDetails[0]->created_by;
            $selectStmt = db_select ( 'slt_person', 'prsn' );
            $selectStmt->addField ( 'prsn', 'id', 'id' );
            $selectStmt->addField ( 'prsn', 'first_name', 'first_name' );
            $selectStmt->addField ( 'prsn', 'full_name', 'full_name' );
            $selectStmt->addField ( 'prsn', 'email', 'email' );
            $selectStmt->addField ( 'prsn', 'preferred_language', 'preferred_language' );
            $selectStmt->condition ( 'prsn.id', $created_by );
            $selectStmt->condition ( 'prsn.status', 'cre_usr_sts_atv' );
            $usrDetails = $selectStmt->execute()->fetchAssoc();
            expDebug::dPrint ( ' $user email details result = ' . print_r ( $usrDetails, true ), 3 );
            //$to = $usrDetails['email'];
            $grpDetails = getGrpDetails($GrpId);
            
            $notificationInfo = array();
            $notificationInfo['message_id'] = 'group_bulk_user_upload';
            $notificationInfo['message_type'] = 'Group Bulk User background process completion';
            $notificationInfo['send_type'] = 'php mailer';
            $notificationInfo['lang_code'] = 'cre_sys_lng_eng';
            
            $notificationInfo['tokens_string'] = 'group_name>|' . $grpDetails ['name'] . '~|'.
                    'first_name>|' . $usrDetails ['first_name'] . '~|'.
                    'total_records>|' . $total_record . '~|'.
                    'sucess_records>|' . $success_record . '~|'.
                    'failed_records>|' . $failure_record . '~|';
            generateGUNotification ($notificationInfo, $usrDetails);
         
            $checkStatus = array ('new','inprogress');
            $jobCount = fetchBackgroundListForGroups($checkStatus,'inlist');
            if ($jobCount == 0) {
                $jobGUName = 'group_bulk_user_upload';
                $checkCTStatus = check_monitor_jobs_status($jobGUName);
                if ($checkCTStatus['custom0'] == 1) {
                    // Update monitoring jobs table to turn off the handle
                    $update = db_update ( 'slt_monitoring_jobs' );
                    $update->fields ( array (
                            'custom0' => 0,
                            'updated_on' => now ()
                    ) );
                    $update->condition ( 'job_name', $jobGUName, '=' );
                    $update->execute ();
                }
            }
        }
        $ent = microtime(true);
        expDebug::dPrint("Total Execution time 3333 -- ( $ent - $st = ".($ent-$st)." -- Time in minutes = ".(($ent-$st)/60).")",4);
    //}
    }catch (Exception $ex) {
    	if(empty($remarks)){
    		$remarks = $itr_start_time."__".$ex;
    	}else{
    		$remarks  = $remarks."__".$ex;    		
    	}
        $processed_cnt = ($jobDetails[0]->custom4)+1;
        $status = ($processed_cnt == 3) ? 'failed' : 'inprogress';  
        // Update the status once process is in progress
        $update = db_update('slt_temp_background_jobs');
        $update->fields (array (
                'status' => $status,
                'updated_on' => now(),
                'custom3' => $remarks,
                'custom4' => $processed_cnt,
        ));
        $update->condition('id', $jobDetails[0]->job_id, '=');
        expDebug::dPrintDBAPI ( "batchCTProcess::slt_temp_background_jobs status update completed", $update );
        $update->execute();
        
        watchdog_exception('fetchCTBackgroundList', $ex);
        throw $ex;
    }
    
}

// Generate notification for group bulk user upload

// Notification triggered once the content transfer background process completes.
function generateGUNotification($notificationInfo='',$userDetails = array()) {
    try {
        expDebug::dPrint("notification part starts >> ".print_r($userDetails,true),5);
        
        $setLanguage = 'cre_sys_lng_eng';
        expDebug::dPrint("email id check".$emailId,5);
            
        $notificationTemplateInfo = getNotificationTemplateInfo('group_bulk_user_upload', 'cre_sys_lng_eng');
            
        // Notification for users whose Email Id given in the Job creation.
        if(!empty($userDetails['email']) && $notificationTemplateInfo['status']=='cre_ntn_sts_atv'){
            $nid = db_insert('slt_notification') // Table name no longer needs {}
            ->fields(array(
                    'msg_id' => $notificationInfo['message_id'],
                    'lang_code' => $userDetails['preferred_language'],
                    'msg_type' => $notificationInfo['message_type'],
                    'token_str' => $notificationInfo['tokens_string'],
                    'user_id' => 1,
                    'send_to_id' => $userDetails['id'],
                    'send_to_name' => $userDetails['first_name'],
                    'send_to_email' => $userDetails['email'],
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

?>