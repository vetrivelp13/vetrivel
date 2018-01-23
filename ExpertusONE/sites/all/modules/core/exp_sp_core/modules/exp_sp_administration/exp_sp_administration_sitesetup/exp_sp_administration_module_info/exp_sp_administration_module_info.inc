<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/*
 * getModuleInfoTabContentType() .
 */

function getModuleInfoTabContentType(){
    try{
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
    watchdog_exception('getModuleInfoTabContentType', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * moduleInfoNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function moduleInfoNarrowSearchFiltersHTML() {
  try {
      $checkedStatus = array();
      $allStatus = array('1' => t('LBL919'),'2' => t('LBL920'));
      expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

      $renderfiltersets=array();

      if(count($allStatus) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'modulestatus',
                                                                     'title' => t('LBL102'),//Status
                                                                     'divid' => 'status',
                                                                     'checkboxes_list' => $allStatus,
                                                                     'checked_list' => $checkedStatus)
                                                              );
      }

     $checkedModules = array();

     $modulesInfo         = getModulesInfo(false);
     $countModule         = count($modulesInfo);
     $drupalModuleName    = array();
     $drupalExpertusName  = array();
     for($i=0;$i<$countModule;$i++) {
          $drupalModuleName[] =  $modulesInfo[$i]['expertus_module_name'];
          $drupalExpertusName[] = $modulesInfo[$i]['display_name'];
     }
     $allModules = array_combine(array_values($drupalModuleName),array_values($drupalExpertusName));
     expDebug::dPrint('$allModules List = ' . print_r($allModules, true), 4);
    /* if(count($allModules) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'modulenames',
                                                                     'title' => t('LBL1005'),//Modules
                                                                     'checkboxes_list' => $allModules,
                                                                     'checked_list' => $checkedModules)
                                                              );
      } */
      $narrowfilters =  array();
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"modulestatus");
      //$narrowfilters[] = array("type"=> "checkbox", "code"=>"modulenames");
      $filterSetforJS=$narrowfilters;

     return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
  } catch (Exception $ex) {
    watchdog_exception('moduleInfoNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * moduleInfoSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function moduleInfoSearchResultsSortbarHTML() {
  try{
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
    watchdog_exception('moduleInfoSearchResultsSortbarHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * moduleInfoSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function moduleInfoSearchResultsActionbarHTML() {
  try{
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
    watchdog_exception('moduleInfoSearchResultsActionbarHTML', $ex);
    expertusErrorThrow($ex);
}
}
/*
 * getModuleInfoAdminTabContentMainHTML - Returns the html for tab Module settings
 */
function getModuleInfoAdminTabContentMainHTML() {
    try{
   $narrowsearchTabModuleListforJS = array(
                                            'search_base_path' =>'administration/sitesetup/moduleinfo/search/all',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/sitesetup/moduleinfo/textfilter-autocomplete'
                                          );

   return theme('narrow_search_tab_content',
                              array('narrowsearch_tab_content_list' => drupal_json_encode($narrowsearchTabModuleListforJS))
                              );
} catch (Exception $ex) {
    watchdog_exception('getModuleInfoAdminTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
}
}

/*
 * getModuleTabContent        - Returns the html for narrow search block on mdule search page
 */
function getModuleInfoTabContent() {
    try {
  $tabContentTypeArr = getModuleInfoTabContentType();

  $moduleResponse = array(
    'rendered_tab_content_type'        => $tabContentTypeArr['tab_content_type'],
    'rendered_narrow_search_filters'   => moduleInfoNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar'   => moduleInfoSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => moduleInfoSearchResultsActionbarHTML(),
    'rendered_tab_content_main'        => getModuleInfoAdminTabContentMainHTML(),
    'rendered_main_div'                => getAdminModules('administration','sitesetup'),
  );
  echo drupal_json_encode($moduleResponse);
  } catch (Exception $ex) {
    watchdog_exception('getModuleInfoTabContent', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getModuleInfoDetailsHTML() - Renders a module detail as HTML to be displayed in jqGrid.
 */
function getModuleInfoDetailsHTML($module) {
 try {
  $detailsList = array();
  if($module->status ==2){ $edit_path = "";}else {$edit_path = $module->edit_url_path;}
  if($module->display_name == t("Commerce") || $module->display_name == t("LBL855") || $module->display_name == t("Rating") || $module->display_name == t("LBL1247") || $module->display_name == t("LBL1248") || $module->display_name == t("Admin Calendar") || $module->display_name == t('LBL3021').t(' ').t('LBL3088') || $module->display_name == t("LBL2015")){  //#custom_attribute_0078975
    $actionList = array();
  }else{
  $actionList[] = array('action_link_text' => t('LBL1023'), //Edit
                        'action_page_path' => $edit_path, //path',
                        'action_button_params' => '',//$module->expertus_module_name,
                        'action_module_status' => $module->status,
                        'action_button_param_text' => '',
                        'ctools_style' => $module->ctools_styles,
                         'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                        'tooltip' => t('LBL1023')
                            );
  }
  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters(trim($module->display_name)),
                       'details_list' => $detailsList,
                       'description'=> trim($module->display_description),
                       'delete_info' => t('MSG357').' '.strtolower(t('Tax').' '.t('MSG616')),
                       'action_list'=> $actionList
                      )
                );
} catch (Exception $ex) {
    watchdog_exception('getModuleInfoDetailsHTML', $ex);
    expertusErrorThrow($ex);
}
}

/*
 * getModuleInfoActionsHTML() - Renders module actions as HTML to be displayed in jqGrid.
 */
function getModuleInfoActionsHTML($module) {
    try {
  return theme('narrow-search-results-item-actions',
                     array('catalog_details' => $module)
               );
  } catch (Exception $ex) {
    watchdog_exception('getModuleInfoActionsHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * sendModuleInfoListToJqGrid() - Prepares JSON response to JqGrid's request for module Admin
 */
function sendModuleInfoListToJqGrid() {
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

  $page = $_GET['page'];//$_GET['page']; // get the requested page
  $limit = $_GET['rows'];
/*   $moduleList = fetchModuleInfoAll();
  $numModule = count($moduleList);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numModule); */
  $start = calculate_offset($page, $limit);
  $moduleList = fetchModuleInfoAll();
  //expDebug::dPrint('requ '.print_r($_REQUEST, 1) );
 //expDebug::dPrint('$MouleList fetchModuleInfoAll = ' . print_r($moduleList, true), 4);
  if(isset($_REQUEST['jqgrid_row_id'])) {
    foreach ($moduleList as  $key=>$module) {
        //expDebug::dPrint('mod name '.$module['expertus_module_name']);
        if($module['expertus_module_name'] == $_REQUEST['jqgrid_row_id']) {
            $moduleFound = $module;
            //expDebug::dPrint('$moduleFound $module'.print_r($module, 1));
        }
    }
    //expDebug::dPrint('$moduleFound '.print_r($moduleFound, 1));
    $moduleList = array($moduleFound);
  } else {
    $moduleList = array_slice($moduleList, $start, $limit);
  }
  expDebug::dPrint('$MouleList = ' . print_r($moduleList, true), 4);

  // Prepare and send the response
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numModule;

  $response->initial_sort_type_html_id = 'module-sort-new';
  $i=0;
  foreach ($moduleList as  $key=>$module) {
    //expDebug::dPrint('$module in for'.print_r($module, 1), 1);
    $module = array_to_object($module);
    $module->row =  $i;
    $module->object_type = 'moduleinfo';
    $response->rows[$i]['class']   = 'admin-module-info';
    $response->rows[$i]['id']   = $module->expertus_module_name;
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $module)),
                                         'details' => getModuleInfoDetailsHTML($module),
                                         'action'  => getModuleInfoActionsHTML($module),

                                         );

    $i++;
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);
  expDebug::dPrint('$response module info = '. print_r($module, true), 4);

  echo drupal_json_encode($response);
  } catch (Exception $ex) {
    watchdog_exception('sendModuleInfoListToJqGrid', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * fetchModuleInfoAll() - Fetches Module list / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchModuleInfoAll() {
 try {
 $moduleInfoList = getModulesInfo(true);
 if(!empty($_GET['modulestatus']) && isset($_GET['modulestatus'])){
    expDebug::dPrint(' condtions true = ' . print_r($_GET['modulestatus'], true) , 4);
    $moduleInfoList = selectAddNarrowSearchModuleINCondition($moduleInfoList, $_GET['modulestatus'], 'status');
  }
 /*if(!empty($_GET['modulenames']) && isset($_GET['modulenames'])){
    expDebug::dPrint(' modulename = ' . print_r($_GET['modulename'], true) , 4);
    $moduleInfoList = selectAddNarrowSearchModuleINCondition($moduleInfoList, $_GET['modulenames'], 'expertus_module_name');
  }*/
  if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
    $moduleInfoList = selectAddNarrowSearchModuleTextCondition($moduleInfoList, $_GET['textfilter'], 'display_name');
  }
 return $moduleInfoList;
 } catch (Exception $ex) {
    watchdog_exception('fetchModuleInfoAll', $ex);
    expertusErrorThrow($ex);
 }
}
/*
 * selectAddNarrowSearchModuleINCondition() Narrow filter option for checkbox option
 */
function selectAddNarrowSearchModuleINCondition($modulelist, $filterval, $filtersetCode) {
    try{
  if (!empty($filterval)){
    $narrowSearchSet = explode("|", $filterval);
    expDebug::dPrint(' narrowSearchSet = ' . print_r($narrowSearchSet, true) , 4);
    $narrowcount = count($narrowSearchSet);
    if($narrowcount){
      $countModule = count($modulelist);
      for($i=0; $i < $countModule; $i++) {
        $assignedVal = $modulelist[$i][$filtersetCode];
        if(!in_array($assignedVal, $narrowSearchSet)){
          unset($modulelist[$i]);
        }
      }
    }
  }
  return $modulelist;
  } catch (Exception $ex) {
    watchdog_exception('selectAddNarrowSearchModuleINCondition', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * selectAddNarrowSearchModuleTextCondition() Narrow filter option for textbox option
 */
function selectAddNarrowSearchModuleTextCondition($modulelist, $filterval, $filtersetCode) {
    try {
  if (!empty($filterval)){
    expDebug::dPrint(' narrowSearchSet = ' . print_r($narrowSearchSet, true) , 4);
    $countModule = count($modulelist);
    for($i=0; $i < $countModule; $i++) {
       $assignedVal = $modulelist[$i][$filtersetCode];
       if(stripos(trim($assignedVal),$filterval) === false ){
          unset($modulelist[$i]);
       }
    }
  }
  expDebug::dPrint(' module list = ' . print_r($modulelist, true) , 4);
  return $modulelist;
  } catch (Exception $ex) {
    watchdog_exception('selectAddNarrowSearchModuleTextCondition', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getModulesInfo() Get the Details of all module lists
 */
function getModulesInfo($getStatus = false) {

    global $theme_key;
    global $user;

    try{
            include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";       
        /* Added for Visibility check */
        $select = db_select('slt_profile_list_items','spli');
        $select->addField('spli','code');
        $select->condition('spli.code',db_like('cre_ste_mod_').'%','LIKE');
        $select->groupBy('spli.code');
        expDebug::dPrintDBAPI('Query for visibility groups checking',$select);
        $result = $select->execute()->fetchCol();
        expDebug::dPrint('get settings visibility'.print_r($result,true),4);
        
        $is_module_admin_access = is_module_admin_access();
        expDebug::dPrint('is_module_admin_access'.print_r($is_module_admin_access,true),4);
        expDebug::dPrint('$user->uid'.print_r($user->uid,true),4);
        
     
    //Display all modules, if logged_in user is a Drupal Admin
 if($user->uid == '1') {
        
            if(in_array('cre_ste_mod_web',$result)){
            $drupal_module_list = array('googleanalytics');
            $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
            $modulesInfo[] = array(
                    'expertus_module_name' => 'exp_sp_administration_webanalytics',
                    'drupal_module_name' => $drupal_module_list,
                    'display_name' => t('LBL990'), //Web Analytics
                    'display_description' => t('Web analytics is the measurement, collection, analysis and reporting of internet data for purposes of understanding and optimizing web usage.It helps one to estimate how traffic to a website changes. Web analytics provides information about the number of visitors to a website and the number of page views. It helps gauge traffic and popularity trends which is useful for market research.'), //Web analytics is the measurement, collection, analysis and reporting of internet data for purposes of understanding and optimizing web usage.It helps one to estimate how traffic to a website changes. Web analytics provides information about the number of visitors to a website and the number of page views. It helps gauge traffic and popularity trends which is useful for market research.
                    'imagepath' => 'web-analytics-admin-icon',
                    'ctools_styles' => 'ctools-modal-ctools-admin-webanalytics-style',
                    'edit_url_path' => 'administration/sitesetup/moduleinfo/webanalytics/nojs',
                    'status' => $status,
            );
        }
        if(in_array('cre_ste_mod_pas',$result)){
            $drupal_module_list = array('password_policy');
            $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
            $modulesInfo[] = array(
                    'expertus_module_name' => 'exp_sp_administration_pwdpolicy',
                    'drupal_module_name' => $drupal_module_list,
                    'display_name' => t('LBL1040'), //Password Policy
                    'display_description' => t('A password Policy is a set of rules designed to enhance computer security by encouraging users to employ strong passwords and use them properly. The purpose of this policy is to establish a standard for creation of strong passwords, the protection of those passwords, and the frequency of change.'), //Password policy description (to be updated).
                    'imagepath' => 'password-policy-admin-icon',
                    'ctools_styles' => 'ctools-modal-ctools-admin-pwdpolicy-style',
                    'edit_url_path' => 'administration/sitesetup/moduleinfo/pwdpolicy/nojs',
                    'status' => $status,
                    'enable_option'=> 'No',
            );
        }

  $drupal_module_list_ldap = array('ldap_servers','ldap_authentication');
  $status_ldap = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list_ldap):'';
  $drupal_module_list_saml = array('saml');
  $status_saml = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list_saml):'';
 // $drupal_module_list_adfs = array('adfs');
 // $status_adfs = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list_adfs):'';
  $greyout_ldap = false;
  $greyout_saml = false;
 // $greyout_adfs = false;
  if($status_ldap == 1 || $status_saml == 1 || $status_adfs == 1){
    if($status_ldap == 1){
      $greyout_saml = true;
//    $greyout_adfs = true;
     }else if( $status_saml == 1){
        $greyout_ldap = true;
 //     $greyout_adfs = true;
     }/*else{
            $greyout_ldap = true;
            $greyout_saml = true;
     }*/
  }
  //change by ayyappans for 32931: The LDAP login not working, showing the error 'System error , please contact admin'.
  $status_ldap = 2; //status 2 for disabled
  $greyout_ldap = 1;
  if(in_array('cre_ste_mod_ldp',$result)){
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_ldap',
            'drupal_module_name' => $drupal_module_list_ldap,
            'display_name' => t('LBL1041'), //LDAP
            'display_description' => t('LDAP is an Internet protocol that email and other programs use to look up information from a server. Directory Services may provide any organized set of records, often with a hierarchical structure. An LDAP directory can be distributed among many servers.'), //LDAP description (to be updated).
            'imagepath' => 'ldap-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-ldap-setting-addedit-scroll-wrapper',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/ldap/nojs/addedit',
            'status' => $status_ldap,
            'greyout'=> $greyout_ldap,
            'enable_option'=> 'No'  //"No" for disabling publish/unpublish function call
    );
  }
  if(in_array('cre_ste_mod_sml',$result)){
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_saml',
            'drupal_module_name' => $drupal_module_list_saml,
            'display_name' => t('LBL1185'), //SAML
            'display_description' => t('Security Assertion Markup Language (SAML) is an XML-based open standard data format for exchanging authentication and authorization data between parties, in particular, between an identity provider and a service provider.'), //SAML description (to be updated).
            'imagepath' => 'saml-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-saml-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/saml/nojs/addedit',
            'status' => $status_saml,
            'greyout'=> $greyout_saml,

    );
  }

 /* $modulesInfo[] = array(
        'expertus_module_name' => 'exp_sp_administration_adfs',
        'drupal_module_name' => $drupal_module_list_adfs,
        'display_name' => t('LBL1186'), //ADFS
        'display_description' => t('MSG691'), //SAML description (to be updated).
        'imagepath' => 'adfs-admin-icon',
        'ctools_styles' => 'ctools-modal-ctools-admin-adfs-style',
        'edit_url_path' => 'administration/sitesetup/moduleinfo/adfs/nojs/addedit',
        'status' => $status_adfs,
        'greyout'=> $greyout_adfs,
  );*/
  if(in_array('cre_ste_mod_usr',$result)){
    $drupal_module_list = array('exp_sp_admin_userpoints');
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_userpoints',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL1061'), //User Points
            'display_description' => t('User points is a module that can distribute or assign points to registered users according to some of the users actions and activity in the system. Use the configure function to setup the points for each activity and badge in the system.'), //User Points description (to be updated).
            'imagepath' => 'userpoint-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-user-points-addedit-wrapper',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/userpoints/nojs/addedit',
            'status' => $status,
    );
  }
  if(in_array('cre_ste_mod_cmr',$result)){
    $drupal_module_list = array('uc_cart','uc_cart_links','uc_store','uc_product','uc_catalog','uc_credit','uc_cybersource','uc_discounts','uc_order','uc_payment','uc_payment_pack','uc_product_kit','uc_po','uc_termsofservice','exp_sp_administration_commerce','exp_sp_administration_commerce_discounts','exp_sp_administration_order','exp_sp_admin_com_set','exp_sp_administration_commerce_tax','exp_sp_commerce_admin','exp_sp_cart','exp_sp_commerce','exp_sp_commerce_learner','exp_sp_payment','exp_sp_cybersource_tax');
    //if($theme_key == 'expertusoneV2'){
    $imagepathclass = 'commerce-admin-icon';
    //}else{
    //  $imagepathclass = 'payment-method';
    //}
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_module_info_commerce',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Commerce'), //Commerce
            'display_description' => t('E-commerce can be used to enable/disable commerce across the ExpertusONE site. Commercial transactions can take place only after this module is enabled by the super admin.
                                       When enabled, e-commerce components (such as discounts, tax details, orders, cart, payment core etc.) behave as per the administration settings configured in the module.'),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }
  if(in_array('cre_ste_mod_cht',$result)){
    $drupal_module_list = array('nodejs','drupalchat');
    $imagepathclass = 'chat-admin-icon';
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'drupalchat',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL855'), //Chat
            'display_description' => t("Chat is designed for real-time communication between the users 'signed in' to the chat via instant text messages. This feature can be used by learners and trainers alike to interact with each other or with multiple users at the same time, thus resulting in a faster, collaborative learning experience."),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }
  if(in_array('cre_ste_mod_rte',$result)){
    $drupal_module_list = array('votingapi','fivestar','exp_sp_fivestar');
    $imagepathclass = 'rating-admin-icon';
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_fivestar',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Rating'), //Rating
            'display_description' => t('This module can be used to enable/disable rating feature across the ExpertusONE site. When enabled, this feature allows learners to rate a completed training based on their experience about it. Learners can review theses ratings as a standard for the overall course satisfaction.'),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }

  /*streaming server option- module setting up start here */
  if(in_array('cre_ste_mod_str',$result)){
    $drupal_module_list = array('exp_sp_stream_server');
    $imagepathclass = 'video-stream-icon';
    expDebug::dPrint('exp_sp_administration_content= ' . print_r($drupal_module_list, true) , 4);
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_stream_server',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Video Streaming'), //Streaming server
            'display_description' => t('This module can be used to enable/disable live streaming of video content. When disabled, video content is launched via progressive download. When enabled, it is launched as a live stream resulting in instant video playback. This feature also allows a user to to jump forward and backward through the video, thus providing a more interactive learning platform.'),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }
  /*streaming server option- module setting up end here */


  if(in_array('cre_ste_mod_trl',$result)){
    $esignature = array('exp_sp_esignature');
    $esignatureStatus = getDrupalModuleStatus($esignature);
    if($esignatureStatus == 1){
        $drupal_module_list = array('exp_sp_esignature','exp_sp_audit_trail');
    }else{
        $drupal_module_list = array('exp_sp_audit_trail');
    }
    $imagepathclass = 'audit-admin-icon';
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_audit_trail',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL1247'), //Audit Trail
            'display_description' => t('An audit trail is a security-relevant chronological record, set of records, and/or destination and source of records that provide documentary evidence of the sequence of activities that have affected at any time a specific operation, procedure, or event.'),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }
  if(in_array('cre_ste_mod_esg',$result)){
    $auditTrail = array('exp_sp_audit_trail');
    $auditTrailStatus = getDrupalModuleStatus($auditTrail);
    expDebug::dPrint('$$exp_sp_esignature22 = ' . print_r($auditTrailStatus, true) , 4);
    if($auditTrailStatus == 2){
        $drupal_module_list = array('exp_sp_audit_trail','exp_sp_esignature');
    }else{
        $drupal_module_list = array('exp_sp_esignature');
    }
    $imagepathclass = 'esign-admin-icon';
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_esignature',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL1248'), //E-Signature
            'display_description' => t('E-signature feature can be enabled/disabled to authenticate changes made by a user in the ExpertusONE site. When enabled, the feature prompts the user for password to save the changes done by him. This feature can be used as an evidence of user activity.'),
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }

  if(in_array('cre_ste_mod_reg',$result)){
    $drupal_module_list = array('exp_sp_userregister');
    $imagepathclass = 'userreg-admin-icon';
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_userregister',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL1296'),
            'display_description' => t('LBL1297'),
            'ctools_styles' => 'ctools-modal-ctools-admin-userreg-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/userreg/nojs',
            'imagepath' => $imagepathclass,
            'status' => $status,
    );
  }
  
  if(in_array('cre_ste_mod_shr',$result)){
    $drupal_module_list = array('Share Module');
    $shareStatus = getShareModuleStatus($drupal_module_list);
    $status = ($getStatus==true)? $shareStatus['status']:'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'Share Module',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Share'),
            'display_description' => t('MSG820'),
            'ctools_styles' => 'ctools-modal-ctools-admin-share-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/share/nojs',
            'imagepath' => 'share-admin-icon',
            'status' => $status,
    );
  }

  if(in_array('cre_ste_mod_cdn',$result)){
    $drupal_module_list = array('cloud_files');
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'cloud_files',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('CDN – Cloud files'), //Cloud Files
            'display_description' => t('The Content Delivery Network (CDN) feature allows significant reduction in loading time while viewing a content. A CDN has servers all across the world. When enabled, it automatically determines the fastest route between the ExpertusONE Server hosting the content and the Learner trying to access the content. The content is then launched from a server nearest to a Learner, thus resulting significant reduction in loading time for viewing the content.'), //Cloud Files description (to be updated).
            'imagepath' => 'cdn-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-cdn-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/cloudfiles/nojs/addedit',
            'status' => $status,
    );
  }
  
  if(in_array('cre_ste_mod_sf',$result)){
    $drupal_module_list = array('salesforce_integration');
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'salesforce_integration',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Salesforce'),
            'display_description' => t('SalesForce integration component.'), //SalesForce Settings description (to be updated).
            'imagepath' => 'sf-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-sf-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/salesforceintegration/nojs/addedit',
            'status' => $status,
    );
  }


  if(in_array('cre_ste_mod_ipr',$result)){
    $drupal_module_list_iprange = array('IP Ranges');
    $status_iprange = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list_iprange):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_ipr',
            'drupal_module_name' => $drupal_module_list_iprange,
            'display_name' => t('LBL1302'), //IP Ranges
            'display_description' => t('LBL1310'), //IP Ranges description (to be updated).
            'imagepath' => 'iprange-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-iprange-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/ipranges/nojs',
            'status' => 1,
            'enable_option'=> 'No',
    );
  }

    if(in_array('cre_ste_mod_cal',$result)){
    $drupal_module_list = array('exp_sp_admincalendar');
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_admincalendar',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('Admin Calendar'), 
            'display_description' =>t('Admin calendar used to view entities in calendar format'), 
            'imagepath' => 'admincal-admin-icon',
            'status' => $status,
    );
  }

    if(in_array('cre_ste_mod_usp',$result)){
         $drupal_module_list = array('exp_sp_usps_integration');
         $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
         $modulesInfo[] = array(
                 'expertus_module_name' => 'exp_sp_usps_integration',
                 'drupal_module_name' => $drupal_module_list,
                 'display_name' => t('User Address Validation'),
                 'display_description' => t('MSG832'), 
                 'imagepath' => 'uspsapi-admin-icon',
                 'ctools_styles' => 'ctools-modal-ctools-admin-uspsapi-style',
                 'edit_url_path' => 'administration/sitesetup/moduleinfo/uspsapi/nojs/addedit',
                 'status' => $status,
         );
     }
    
 if(in_array('cre_ste_mod_ti',$result)){
        $drupal_module_list = array('exp_sp_theme_customization');
        $modulesInfo[] = array(
              'expertus_module_name' => 'ti',
              'display_name' => t('Theme Customization'),
              'display_description' => t(' This module is for theme customization. It can be used to configure icons for Courses, Classes (Video, Web-based, Classroom and Virtual class) and Training Plans (Certification, Curricula, and Learning Plan).'),
              'imagepath' => 'ticon-admin-icon',
              'ctools_styles' => 'ctools-modal-ctools-admin-ticon-style',
              'edit_url_path' => 'administration/sitesetup/moduleinfo/trainingicons/nojs/addedit',
              'status' => 1,
              'enable_option'=> 'No',
      );
  }

// For Content Authoring Enable/Disable

  if(in_array('cre_ste_mod_aut',$result)){
    $drupal_module_list = array('exp_sp_administration_contentauthor','h5p','h5peditor','tincanapi','tincanapi_content_types','tincanapi_links','tincanapi_media','h5p_tincan_bridge');
     
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
         
    $h5pcontentcount = getPublishedContentAvailability();
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_contentauthor',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL3021').t(' ').t('LBL3088'),
            'display_description' =>t('This module can be used to create interactive learning contents (such as videos and presentations) and attach them to a class.'),
            'imagepath' => 'contentAut-admin-icon',
            'published_content'=>$h5pcontentcount,
            'status' => $status,
    );
  }
  
 }


 //check whether logged in user is Drupal Admin or Administer Group Users
 
 if($is_module_admin_access == '1') {
   if(in_array('cre_ste_mod_mtg',$result)){
    $drupal_module_list = array('custom_meta'); //Custom Meta - Drupal contributed module for SEO MetaTag support in E1
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $modulesInfo[] = array(
            'expertus_module_name' => 'custom_meta',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL1317'), //Meta Tag
            'display_description' => t('LBL1320'), //Meta Tag description (to be updated).
            'imagepath' => 'metatag-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-metatag-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/metatag/nojs/addedit',
            'status' => $status,
    );
  }
   
   if(in_array('cre_ste_mod_wex',$result)){
    $status = ($getStatus==true)? getprofileliststatus('webex'):'';
    $enable=checkfuturewebexsession();
    $modulesInfo[] = array(
            'expertus_module_name' => 'webex',
            'display_name' => t('WebEx Meeting'),
            'display_description' => t('LBL3011'),
            'imagepath' => 'webex-admin-icon',
            'ctools_styles' => 'ctools-modal-ctools-admin-webex-style',
            'edit_url_path' => 'administration/sitesetup/moduleinfo/webex/nojs',
            'status' => $status,
            'enable_option'=> $enable,
    );
  }

  //#custom_attribute_0078975
  if(in_array('cre_ste_mod_cattr',$result)){
    $drupal_module_list = array('exp_sp_administration_customattribute');
    $status = ($getStatus==true)? getDrupalModuleStatus($drupal_module_list):'';
    $enable1=getcustomattributemodulestatus();
    if ($enable1 == true)
        $enable = 'No';
    else 
        $enable = 'Yes' ;
    expDebug::dPrint("status of custom".print_r($status,true),5);
 
   //#custom_attribute_0078975
    $modulesInfo[] = array(
            'expertus_module_name' => 'exp_sp_administration_customattribute',
            'drupal_module_name' => $drupal_module_list,
            'display_name' => t('LBL2015'),
            'display_description' =>t('MSG1110'),
            'imagepath' => 'customattribute-admin-icon',
            'status' => $status,
            'enable_option'=> $enable,
    );
  }

 }
  return $modulesInfo;
  } catch (Exception $ex) {
    watchdog_exception('getModulesInfo', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getDrupalModuleStatus() Get the Drupal module status
 */

function getDrupalModuleStatus($moduleName){
    try{
  $select = db_select('system', 'sys');  
  $select->condition('sys.name', $moduleName, 'IN');
  $select->addField('sys', 'name', 'module_name');
  $select->addField('sys', 'status', 'module_status');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  $result = $select->execute()->fetchAll();
  $count_details = count($result);
  $getStatus =array();
  if(!empty($result)){
  for($i=0; $i<$count_details;$i++){
    $getStatus[] = $result[$i]->module_status ;
  }
  if(in_array(0,$getStatus)){
     $status = 2;
  }else{
     $status=1;
  }
  }else {
    $status = 2;
  }
  expDebug::dPrint('$drupalModuleNameList = ' . print_r($result, true) , 4);
  expDebug::dPrint('Testing Status = ' . print_r($status, true) , 4);
  return $status;
  } catch (Exception $ex) {
    watchdog_exception('getDrupalModuleStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function getprofileliststatus($name=''){
    try {
    if($name == 'webex'){
    $select = db_select('slt_profile_list_items', 'spfl');
    $select->condition('spfl.name', $name);
    $select->condition('spfl.code','lrn_cls_vct_web');
    $select->addField('spfl', 'is_active','status');
    $result = $select->execute()->fetchAll();
    if(!empty($result[0]->status)){
        if($result[0]->status == 'Y'){
            $status = 1;
        }else {
            $status = 2;
        }
    }
    expDebug::dPrint('getprofileliststatus = ' . print_r($status, true) , 4);
    return $status;
}
    }catch(Exception $ex){
        watchdog_exception('getprofileliststatus', $ex);
    }
}
/*
 * function to publish and unpublish the Module
 */
#custom_attribute_0078975
function publishOrUnpublishModule($moduleName, $enabledModuleList) {
  try {
  include_once $_SERVER["DOCUMENT_ROOT"]."//sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";
  $moduleList = getModulesInfo(true);
  $count      = count($moduleList);
  //expDebug::dPrint('get all modules info'.print_r($moduleList,true), 4);
  $disableList = array();
  for($i=0; $i < $count;$i++){
    if(($moduleName == 'exp_sp_administration_ldap') || ($moduleName == 'exp_sp_administration_saml') || ($moduleName == 'exp_sp_administration_adfs') || ($moduleName == 'exp_sp_stream_server')){
      if(($moduleList[$i]['expertus_module_name'] == 'exp_sp_administration_ldap') || ($moduleList[$i]['expertus_module_name'] == 'exp_sp_administration_saml') || ($moduleList[$i]['expertus_module_name'] == 'exp_sp_administration_adfs') || ($moduleList[$i]['expertus_module_name'] == 'exp_sp_stream_server')){
        $disableList[] = $moduleList[$i]['drupal_module_name'];
      }
      expDebug::dPrint('disableList'.print_r($disableList,true), 4);
    }
    if($moduleList[$i]['expertus_module_name']==$moduleName && $moduleList[$i]['expertus_module_name']!='webex' && $moduleList[$i]['expertus_module_name']!='Share Module'){
      $keyvalue=$i+1;
      $enableList = $moduleList[$i]['drupal_module_name'];
      expDebug::dPrint("get status of module enable list".print_r($enableList,true), 4);
     $getStatus  = $moduleList[$i]['status'];
     expDebug::dPrint("get status of module $i".print_r($getStatus,true), 4);
     break;
    }  
    
    if($moduleName=='Share Module'){
        if($moduleList[$i]['expertus_module_name']=='Share Module'){
            $keyvalue=$i+1;
          $getStatus  = $moduleList[$i]['status'];
          if($getStatus == '1'){//disabled state
             share_changeStatus('cre_ntn_sts_itv',$getStatus,'N');
          }else{
             share_changeStatus('cre_ntn_sts_atv',$getStatus,'Y');
          } 
           break;
      }
    }

    if($moduleName == 'exp_sp_stream_server') {
     if(!ffmpegAvailableOrNot()) {
        expDebug::dPrint("FFMPEG server configuration is needed",4);
        break;
    }
  }
  if($moduleList[$i]['expertus_module_name'] == 'webex' && $moduleName == 'webex'){ //#custom_attribute_0078975
    $keyvalue=$i+1;
    $update = db_update('slt_profile_list_items');
    $getStatus  = $moduleList[$i]['status'];
    $update->fields(array('is_active' => ($getStatus == 1)? 'N' : 'Y'));
    $update->condition('name','webex');
    $update->condition('code','lrn_cls_vct_web');
    expDebug::DPrintDBAPI('update webex status' ,$update,5);
    $update->execute();
     break;
  }
  }
  //enable the module
  $txn = db_transaction();
  try {
      if($getStatus==2){
         if(count($disableList)){
            foreach($disableList as $listdiasble){
                module_disable($listdiasble,false);
            }
          }
          $updatedstatus =1;
          module_enable($enableList,false);
      }else{
        $updatedstatus =2;
        module_disable($enableList,false);
      }
      
     if($moduleName == "exp_sp_administration_contentauthor")
        contentAuthor_permission($getStatus);
     
  } catch(Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishModule-EnableorDisable', $ex);
    throw $ex;
  }
  unset($txn);

  $fields = array('status' => $updatedstatus);
  // We don't have module id in the search screen. So placed array position value and should concat module name with entity type.
  add_audit_trail_entry(getIdOfLoggedInUser(), $keyvalue, $moduleName, 'exp_sp_administration_module_info', 'EnableorDisable', 'Updated model status',
                                             null, $getStatus, $updatedstatus);
  // define static var
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  // clear drupal performance cache
  drupal_flush_all_caches();
  expDebug::dPrint('get all modules'.print_r($enableList,true), 4);
  $returnStatus  = ($updatedstatus=='1')? t('LBL920') : t('LBL919');
  //#custom_attribute_0078975 // call migration process 
  if($moduleName == "exp_sp_administration_customattribute" && $returnStatus == 'Disable'){ 
          expDebug::dPrint('Call Migration', 4);
         invokeUserCustomAttributeMigrationBackgroundProcess(); 
  }
  
  print $updatedstatus . '|' . $returnStatus;
  }
  catch (Exception $ex) {
    watchdog_exception('publishOrUnpublishModule', $ex);
    throw $ex;
  }
}

/*
 * migrateCustomAttributeFromCustomFields() - To invoke the PHP file in background to create custom fields autometically according to custom fields and migrate it according to custom attribute structure
 * //#custom_attribute_0078975
 */
 
function invokeUserCustomAttributeMigrationBackgroundProcess() {  
    try {
        global $base_url, $user;
        expDebug::dPrint('invokeUserCustomAttributeMigrationBackgroundProcess' . var_export($user, 1), 4);
        
        $headers[] = 'Accept: application/json, text/plain, */*';
        $headers[] = 'Connection: Keep-Alive';
        $headers[] = 'Content-type: application/x-www-form-urlencoded;charset=UTF-8';
    
        $url    = $base_url. '?q=administration/manage/customattribute/migration-process';  
        $data   = array(
                'learner_id'        => $userId,
        );
        $cookie = array();
        foreach( $_COOKIE as $key => $value ) {
            $cookie[] = "{$key}={$value}";
        };
        $cookie = implode('; ', $cookie);
        $headers[]  = 'Cookie:' . $cookie;
        
        $curl   = curl_init();
        $curl_options = array(
                //CURLOPT_COOKIEJAR => NULL,
                //CURLOPT_URL => $url,
                CURLOPT_FOLLOWLOCATION => FALSE,
                CURLOPT_RETURNTRANSFER => FALSE,
                CURLOPT_SSL_VERIFYPEER => FALSE, // Required to make the tests run on https.
                CURLOPT_SSL_VERIFYHOST => FALSE, // Required to make the tests run on https.
                CURLOPT_HEADER => FALSE,
                //CURLOPT_BINARYTRANSFER => TRUE,
                CURLOPT_TIMEOUT => 1,
                CURLOPT_CONNECTTIMEOUT => 1,
                CURLOPT_HTTPHEADER => $headers,
                // CURLOPT_USERAGENT => 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Firefox/51.0',
                CURLOPT_HEADER => 0,
                CURLOPT_FORBID_REUSE => TRUE,
                //CURLOPT_DNS_CACHE_TIMEOUT => 10,
                CURLOPT_FRESH_CONNECT => TRUE
        );
        $param = http_build_query($data);
        curl_setopt_array($curl, $curl_options);
        curl_setopt($curl, CURLOPT_POSTFIELDS, $param); // Set post param
        curl_setopt($curl, CURLOPT_URL, $url);
        $result = curl_exec($curl);
        curl_close($curl);
    } catch(Exeception $ex){
        watchdog_exception('invokeUserCustomAttributeMigrationBackgroundProcess', $ex);
        expertusErrorThrow($ex);
    }
} 

/*
 * getmoduleNameFilterAutoComplete() get the drupal module name in the auto-complete list
 */
function getmoduleNameFilterAutoComplete() {
    try{
  $searchText =  $_GET['z'];
  $moduleList = getModulesInfo(false);
  foreach($moduleList as $module) {
    if(stripos(trim($module['display_name']),$searchText) !== false ){
        print $module['display_name'] . "\n";
    }
  }
  } catch (Exception $ex) {
    watchdog_exception('getmoduleNameFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

function ffmpegAvailableOrNot() {
    $config=getConfig("exp_sp");
    if(isset($config['ffmpeg_server_ip'])) {
        $ffmpeg_servers = explode(',',$config['ffmpeg_server_ip']);
        expDebug::dPrint('ffmpeg_servers'.print_r($ffmpeg_servers,true), 4);
        if(!empty($ffmpeg_servers[0])) {
            expDebug::dPrint('ffmpeg configuration available', 4);
            return true;
        } else {
            expDebug::dPrint('ffmpeg configuration Not available', 4);
            return false;
        }
    }else {
        expDebug::dPrint('ffmpeg configuration Not available', 4);
        return false;
    }
}

function contentAuthor_permission($status){
    $updateStmt = db_update('slt_profile_list_items');
    $updateStmt->condition('code', array('cre_sec_pmn_adm_aut','cre_sec_pmn_adm_aut_001','cre_sec_pmn_adm_aut_002'),'IN');
    
        if($status == 2){   
            $updateStmt -> fields(array('is_active' => 'Y', 'updated_on' => now()));
        }
        else{
            $updateStmt -> fields(array('is_active' => 'N', 'updated_on' => now()));
        }
        
        expDebug::dPrintDBAPI('Abhishek $UPDATEE query', $updateStmt);
        
            $updateStmt -> execute();
    
}



function checkfuturewebexsession(){
    $query=db_select('slt_course_class_session','ses');
    $query->join('slt_course_class','cls','cls.id=ses.class_id');
    $query->addExpression('MAX(ses.start_date)','date');
    $query->condition('ses.type','lrn_cls_vct_web','=');
    $query->condition('cls.status','lrn_cls_sts_atv','=');
    $result=$query->execute()->fetchAll();
        $ses_date=$result[0]->date;
    if($ses_date == '' || $ses_date == null){
        return 'Yes';
    }else{
        $ses_date = new DateTime($ses_date);
        $ses_date = $ses_date->format('Y-m-d');
        $ses_date=strtotime($ses_date);
        $today=date('Y-m-d');
        $today=strtotime($today);
        if ($ses_date >= $today){
            return 'No';
            
        }else{
                return 'Yes';
         }
}
}

function getPublishedContentAvailability(){
$select = db_select('slt_content_master', 'cont_mast');
$select->condition('cont_mast.status', 'lrn_cnt_sts_atv', '=');
$select->condition('cont_mast.subtype', array('h5p-video-mp4','h5p-video-webm','h5p-Youtube','h5p-Vimeo','h5p-presentatn'),'IN');
$select->addExpression('COUNT(DISTINCT(cont_mast.id))', 'COUNT');
expDebug::dPrintDBAPI('Abhishek $select before count', $select);
$h5pcontentcount = $select->execute()->fetchField();
expDebug::dPrint('COUNT $result abhi = ' . $h5pcontentcount ,3);

if(intval($h5pcontentcount,10) > 0)
    $h5pcontentcount = 1;
else
    $h5pcontentcount = 0;

return $h5pcontentcount;
}

?>
