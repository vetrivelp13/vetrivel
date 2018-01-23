<?php

/**
 * Catalog Admin inc file 
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 06-Feb-2012
 */

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc";


/*
 * getCatalogTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 *                            - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */
function getCatalogTabContentType(){
  try {
    $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
    return $jsObjectInfo;
   } catch (Exception $ex) {
    watchdog_exception('getCatalogTabContentType', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * catalogNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function catalogNarrowSearchFiltersHTML() {
  try {
        // Added for performance improvement
        global $user;
//          expDebug::dPrint(' logged_in_user_profile = ' . print_r($user, true) , 4);
        $cache_cat_filter_key = 'catalog_filters_'.$user->language;
        $catFilters = cache_get($cache_cat_filter_key)->data;
        if(!empty($catFilters)){
            $renderfiltersets = $catFilters['renderfiltersets'];
            //Course tags
            $weightedCourseTags = getWeightedTags(array('Course'));
          if (!empty($weightedCourseTags)) {
            $renderfiltersets['catalogcoursetag1']=theme('narrow_search_filterset_tags',
              array('code' => 'catalogcoursetag',
                            'html_id' => 'catalogcoursetag_container',
                            'title' => t('LBL191'), //Tags
                            'tags_for_cloud' => $weightedCourseTags
                       )
             );
          }
          else
         {
            $renderfiltersets['catalogcoursetag1']="";
          }

            // Class tags
            $weightedClassTags = getWeightedTags(array('Class'));
          if (!empty($weightedClassTags)) {
            $renderfiltersets['catalogclasstag1']=theme('narrow_search_filterset_tags',
                                                                                                                        array('code' => 'catalogclasstag',
                                                                                                                                'html_id' => 'catalogclasstag_container',
                                                                                                                                'title' => t('LBL191'), //Tags
                                                                                                                                'tags_for_cloud' => $weightedClassTags
                                                                                                                                    )
                                                                                                                              );
          }
          else
         {
            $renderfiltersets['catalogclasstag1']="";
          }
            $filterSetforJS = $catFilters['filterSetforJS'];
            return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
        }

        $catFilters = array();
      //$allStatus = getProfileItemNamesKeyed('lrn_crs_sts_');
      $allCourseStatus = getStatusCatalogFilter('lrn_crs_sts_');
      $allCourseStatusChange = array();
      $checkedCourseStatus = array();
      foreach($allCourseStatus as $key =>$val){
        $allCourseStatusChange[$key] = $val;
      }

      $allClassStatus = getStatusCatalogFilter('lrn_cls_sts_');
      $allClassStatusChange = array();
      $checkedClassStatus = array();
      foreach($allClassStatus as $key =>$val){
        $allClassStatusChange[$key] = $val;
      }

      $checkedCatalogType = array();
      $checkedCatalogType["Course"] = TRUE;

      expDebug::dPrint(' $allStatusChange = ' . print_r($allCourseStatusChange, true) , 4);

      $allCourseLangTypes = fetchCatalogLanguagesList('Course');
      $allClassLangTypes = fetchCatalogLanguagesList('Class');
      expDebug::dPrint(' $allLangTypes = ' . print_r($allCourseLangTypes, true) , 4);

      $allCatalogType = fetchCatalogTypes();
      expDebug::dPrint(' $allCatalogType = ' . print_r($allCatalogType, true) , 4);

      $classDeliveryType = getProfileItemNamesKeyed('lrn_cls_dty_');
      expDebug::dPrint(' $$classDeliveryType = ' . print_r($classDeliveryType, true) , 4);

      $overAllCountry = fetchCatalogCountryList();
      expDebug::dPrint(' $$overAllCountry = ' . print_r($overAllCountry, true) , 4);

      $renderfiltersets=array();
      $allLanguages = array();
      $selectedLanguages = array();

      $managedByOptions = getManagedByFilters();

      $currencyOptions = getCurrencyFilters();
      // Course Filters
      if(count($allCatalogType) > 1){
        $renderfiltersets['catalogtype1']= theme('narrow_search_filterset_radio',
                                                               array('code' => 'catalogtype',
                                                                     'html_id' => 'catalogtype_container',
                                                                     'title' => t('LBL036'),
                                                                     'radio_list' => $allCatalogType,
                                                                     'radio_checked_list' => $checkedCatalogType)
                                                              );
      }

      if(count($allCourseStatus) > 1){
        $renderfiltersets['catalogcoursestatus1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'catalogcoursestatus',
                                                                     'html_id' => 'catalogcoursestatus_container',
                                                                     'title' => t('LBL102'),
                                                                     'divid' => 'status',
                                                                     'checkboxes_list' => $allCourseStatusChange,
                                                                     'checked_list' => $checkedCourseStatus)
                                                              );
      }

      if(count($allCourseLangTypes) > 1){
        $renderfiltersets['courselangtype1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'courselangtype',
                                                                     'html_id' => 'courselangtype_container',
                                                                     'title' => t('LBL038'),
                                                                     'divid' => 'language',
                                                                     'checkboxes_list' => $allCourseLangTypes)
                                                              );
      }
      $renderfiltersets['coursemanage1'] = theme('narrow_search_filterset_checkboxes',
                                                                array('code' => 'coursemanage',
                                                                        'html_id' => 'catalogcoursemanageby_container',
                                                                        'title' => t('Managed By'),
                                                                        'divid' => 'managed_by',
                                                                        'checkboxes_list' => $managedByOptions)
                                                          );

    // Added by Gayathri for 0070892
  if(module_exists('exp_sp_commerce') && count($currencyOptions) > 1)
      {
          $renderfiltersets['coursecurrency1'] = theme('narrow_search_filterset_checkboxes',
                array('code' => 'coursecurrency',
                        'html_id' => 'catalogcoursecurrency_container',
                        'title' => t('LBL101'),
                        'divid' => 'currency',
                        'checkboxes_list' => $currencyOptions)
          );
      }

      $classmax = getConfigValue('maxprice');
      if(module_exists('exp_sp_commerce') && ($classmax > 0)) {
        $renderfiltersets['coursePrice1']=theme('narrow_search_filterset_slider',
                                                               array('code' => 'coursePrice',
                                                                     'html_id' => 'coursePrice_container',
                                                                     'title' => t('LBL040'),
                                                                     'startval' =>'0' ,
                                                                      'endval' => $classmax,
                                                                      'prefix' => ''
                                                                     )
                                                                   );
      }


      $weightedCourseTags = getWeightedTags(array('Course'));
      if (!empty($weightedCourseTags)) {
        $renderfiltersets['catalogcoursetag1']=theme('narrow_search_filterset_tags',
          array('code' => 'catalogcoursetag',
                        'html_id' => 'catalogcoursetag_container',
                        'title' => t('LBL191'), //Tags
                        'tags_for_cloud' => $weightedCourseTags
                   )
         );
      }

      // Class Filters

      if(count($allClassStatus) > 1){
        $renderfiltersets['catalogclassstatus1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'catalogclassstatus',
                                                                     'html_id' => 'catalogclassstatus_container',
                                                                     'title' => t('LBL102'),
                                                                     'divid' => 'status',
                                                                     'checkboxes_list' => $allClassStatusChange,
                                                                     'checked_list' => $checkedClassStatus)
                                                              );
      }
      if(count($classDeliveryType) > 1){
        $renderfiltersets['deliverytype1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'deliverytype',
                                                                     'html_id' => 'classdeliverytype_container',
                                                                     'title' => t('LBL084'),
                                                                     'divid' => 'delivery_type',
                                                                     'checkboxes_list' => $classDeliveryType)
                                                              );
      }
      $renderfiltersets['classmanage1'] = theme('narrow_search_filterset_checkboxes',
                                                                array('code' => 'classmanage',
                                                                        'html_id' => 'catalogclassmanageby_container',
                                                                        'title' => t('Managed By'),
                                                                        'divid' => 'managed_by',
                                                                        'checkboxes_list' => $managedByOptions)
                                                          );

      if(module_exists('exp_sp_commerce') && count($currencyOptions) > 1)
      {
          $renderfiltersets['classcurrency1'] = theme('narrow_search_filterset_checkboxes',
                array('code' => 'classcurrency',
                        'html_id' => 'catalogclasscurrency_container',
                        'title' => t('LBL101'),
                        'divid' => 'currency',
                        'checkboxes_list' => $currencyOptions)
          );
      }

            $renderfiltersets['clspgopened1'] = theme('narrow_search_filterset_checkboxes',
                                                                array('code' => 'clspgopened',// hidden filter for differentiating course and class
                                                                        'html_id' => 'class-admin-page-opened',
                                                                        'title' => t('Class Page Opened'),
                                                                        'divid' => 'class_page_opened',
                                                                        'checkboxes_list' => array('cls_pg_opened' => 'Class Page Opened'))
                                                          );
      //$classmax = getMaxClassPrice();
        $classmax = getConfigValue('maxprice');
      if(module_exists('exp_sp_commerce') && ($classmax > 0)) {
        $renderfiltersets['classPrice1']=theme('narrow_search_filterset_slider',
                                                               array('code' => 'classPrice',
                                                                     'html_id' => 'classPrice_container',
                                                                     'title' => t('LBL040'),
                                                                     'startval' =>'0' ,
                                                                      'endval' => $classmax,
                                                                      'prefix' => ''
                                                                     )
                                                                   );
      }

      $renderfiltersets['classdaterange1']=theme('narrow_search_filterset_daterange',
                                                               array('code' => 'classdaterange',
                                                                     'html_id' => 'classdaterange_container',
                                                                     'title' => t('LBL042'), //Date
                                                                     'dateRangeShowOption' => 'all', //dateRangeShowOption options are 'all' or 'past' or 'future'
                                                                     'title_description' => t('LBL043'), //Range. Describes the title. Shown in ().
                                                                     'from_default_text' => t('LBL220'), //From: mm-dd-yyyy
                                                                     'to_default_text' => t('LBL221'), //To: mm-dd-yyyy
                                                                    )
                                                              );
      if(count($allClassLangTypes) > 1){
        $renderfiltersets['classlangtype1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'classlangtype',
                                                                     'html_id' => 'classlangtype_container',
                                                                     'title' => t('LBL038'),
                                                                     'divid' => 'language',
                                                                     'checkboxes_list' => $allClassLangTypes)
                                                              );
      }

      if(count($overAllCountry) > 1){
        $renderfiltersets['countrytype1']= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'countrytype',
                                                                     'title' => t('LBL039'),
                                                                     'divid' => 'country',
                                                                     'checkboxes_list' => $overAllCountry)
                                                              );
      }
      $renderfiltersets['classContent1']=theme('narrow_search_filterset_addltext',
                                                                 array('code' => 'classContent',
                                                                       'html_id' => 'classContent_container',
                                                                         'title' => t('Content'),
                                                                       'default_text'=> t("LBL756")
                                                                       )
                                                                );
      $renderfiltersets['classSurvey1']=theme('narrow_search_filterset_addltext',
                                                                 array('code' => 'classSurvey',
                                                                       'html_id' => 'classSurvey_container',
                                                                         'title' => t('Survey'),
                                                                       'default_text'=> t("LBL545")
                                                                       )
                                                                );
     $renderfiltersets['classAssessment1']=theme('narrow_search_filterset_addltext',
                                                                 array('code' => 'classAssessment',
                                                                       'html_id' => 'classAssessment_container',
                                                                         'title' => t('Assessment'),
                                                                       'default_text'=> t("LBL545")
                                                                       )
                                                                );
      $weightedClassTags = getWeightedTags(array('Class'));
      if (!empty($weightedClassTags)) {
        $renderfiltersets['catalogclasstag1']=theme('narrow_search_filterset_tags',
                                                                                                                    array('code' => 'catalogclasstag',
                                                                                                                            'html_id' => 'catalogclasstag_container',
                                                                                                                            'title' => t('LBL191'), //Tags
                                                                                                                            'tags_for_cloud' => $weightedClassTags
                                                                                                                                )
                                                                                                                          );
      }
     
      /* Added/Changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
       * Changed the Locatin title and Default Text */
      
      $renderfiltersets['classLocation1']=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'classLocation',
                                                                     'html_id' => 'classLocation_container',                                                                     
                                                                     'title' => t('Location'), //Added by ganeshbabuv on March 15th 2017  for  Location Filter #66404
                                                                     'default_text'=> t('LBL1321')
                                                                     )
                                                              ) ;

      $renderfiltersets['classInstructor1']=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'classInstructor',
                                                                     'html_id' => 'classInstructor_container',
                                                                     'title' => t('Instructor'),
                                                                     'default_text'=> t("LBL259")
                                                                     )
                                                              );


      $narrowfilters =  array();
      $narrowfilters[] = array("type"=>"radio", "code"=>"catalogtype");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"deliverytype");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"catalogclassstatus");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"catalogcoursestatus");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"courselangtype");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"classlangtype");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"countrytype");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"coursemanage");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"classmanage");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"clspgopened");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"classcurrency");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"coursecurrency"); // Added by Gayathri for 0070892
      if (!empty($weightedCourseTags)) {
        $narrowfilters[] = array("type" => "tagscloud", "code" => "catalogcoursetag");
      }
      if (!empty($weightedClassTags)) {
        $narrowfilters[] = array("type" => "tagscloud", "code" => "catalogclasstag");
      }
      $narrowfilters[] = array("type"=> "addltext", "code"=>"classLocation",
                                    "acpath" => "administration/learning/catalog/location-autocomplete", "defaultText" => t("LBL1321")); /* Added/Changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
      $narrowfilters[] = array("type"=> "addltext", "code"=>"classInstructor",
                                    "acpath" => "administration/learning/instructor/instructor-autocomplete-withoutid", "defaultText"=> t("LBL259"));
      $narrowfilters[] = array("type"=> "daterange", "code"=>"classdaterange",
                                "from_tooltip" => "", "from_default_text" => t("LBL220"),
                                "to_tooltip" => "", "to_default_text" => t("LBL221"));
      $narrowfilters[] = array("type"=> "addltext", "code"=>"classContent",
                                    "acpath" => "administration/learning/catalog/content-autocomplete", "defaultText" => t("LBL756"));
      $narrowfilters[] = array("type"=> "addltext", "code"=>"classSurvey",
                                    "acpath" => "administration/learning/admin/survey/autocomplete", "defaultText" => t("LBL545"));
      $narrowfilters[] = array("type"=> "addltext", "code"=>"classAssessment",
                                    "acpath" => "administration/learning/admin/assessment/autocomplete", "defaultText" => t("LBL545"));

      if(module_exists('exp_sp_commerce')){
        $narrowfilters[] = array("type"=>"slider",  "code"=>"classPrice");
        $narrowfilters[] = array("type"=>"slider",  "code"=>"coursePrice"); // Added by Gayathri for 0070892
      }
      $filterSetforJS=$narrowfilters;
      $catFilters = array('renderfiltersets' => $renderfiltersets, 'filterSetforJS' => $filterSetforJS, 'clsTagPos' => $clsTagPos, 'crsTagPos' => $crsTagPos);
      cache_set($cache_cat_filter_key,$catFilters);
      return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));

  } catch (Exception $ex) {
    watchdog_exception('catalogNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * catalogSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function catalogSearchResultsSortbarHTML() {
  try {
    $sortbarLinksList = array();
    $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'cat-sort-az');
    $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'cat-sort-za');
    $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'cat-sort-new');

    $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
                                   'last_sort_type_html_id'=>'cat-sort-new'
                                );

    return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                                'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                                                                            'title' => t('LBL011'),
                                                )
                );

  } catch (Exception $ex) {
    watchdog_exception('catalogSearchResultsSortbarHTML', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * catalogSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function catalogSearchResultsActionbarHTML() {
  try {
      $actionbarLinksListforJS=array('export_path'=>'administration/learning/catalog/export/',
                                     'print_path'=>'administration/learning/catalog/print/'
                                  );
      return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL258'),'enroll_upload_button' => t('upload'),
                                                     'add_page_path' => 'administration/learning/course-class/nojs/addedit',
                                                     'show_print_button'=>true,
                                                     'show_export_button'=>true,
                                                     'show_add_button'=>true,
                             'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                                                 'page_code' => 'cre_sys_obt_crs',
                    )
             );
  } catch (Exception $ex) {
    watchdog_exception('catalogSearchResultsActionbarHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCourseClassTabContentMainHTML - Returns the html for tab content main
 */
function getCourseClassTabContentMainHTML() {
  try {
      $tabContentTypeArr =  getCatalogTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];
      if($tabContentType=="narrow_search"){
        $narrowsearchTabContentListforJS=array(
                                                'search_base_path'=>'administration/learning/catalog/search/all/',
                                                'show_top_text_filter' => true,
                                                'text_filter_ac_path' => 'administration/learning/catalog/textfilter-autocomplete'
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
    watchdog_exception('getCourseClassTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCatalogTabContent - Returns the html for narrow search block on program search page
 */
function getCatalogTabContent() {
  try {
      $tabContentTypeArr =  getCatalogTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];

      $catalogDetailsResponse=array(
      'rendered_tab_content_type' => $tabContentType,
      'rendered_narrow_search_filters' => catalogNarrowSearchFiltersHTML(),
      'rendered_narrow_search_sortbar' => catalogSearchResultsSortbarHTML(),
      'rendered_narrow_search_actionbar' => catalogSearchResultsActionbarHTML(),
      'rendered_tab_content_main'=>getCourseClassTabContentMainHTML(),
      'rendered_main_div' => getAdminModules('administration','learning'),
    );
    echo drupal_json_encode($catalogDetailsResponse);
  } catch (Exception $ex) {
    watchdog_exception('getCatalogTabContent', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getCatalogDetailsHTML() - Renders an program details as HTML to be displayed in jqGrid.
 */
function getCatalogDetailsHTML($catalog) {
  try {
      $detailsList = array();
      $actionList = array();
      $viewClasslist = array();
      $viewCourselist = array();
      $promoteAction =array();
      expDebug::dPrint('$catalog---->'.print_r($catalog,true),5);
     /* if (!empty($catalog->object_type)) {
        $detailsList[] = array('detail' => $catalog->object_type, 'tooltip' => t('LBL036'));
      }*/
      if (!empty($catalog->code)) {
        //$detailsList[] = array('detail' => sanitize_data($catalog->code), 'tooltip' => t('LBL096'));
        $detailsList[] = array('detail' => sanitize_data($catalog->code), 'tooltip' => t('LBL096'));
      }
      if (!empty($catalog->lang)) {
        //$langchar=substr($catalog->lang, 0, 3); commented for new theme change
        $detailsList[] = array('detail' => t($catalog->lang), 'tooltip' => t('LBL038').': '.t($catalog->lang));
      }
      if (!empty($catalog->location_name)) {
        $detailsList[] = array('detail' => substr($catalog->location_name, 0, 3), 'tooltip' => t('Location').': '.$catalog->location_name); //Added by ganeshbabuv on March 15th 2017  for  Location Filter #66404
      }
      /*if (!empty($catalog->status)) {
        $detailsList[] = array('detail' => $catalog->status, 'tooltip' => t('LBL102'));
      } */
      if (!empty($catalog->delivery_type)) {
        $detailsList[] = array('detail' => t($catalog->delivery_type), 'tooltip' => t('LBL084').': '.t($catalog->delivery_type));
      }
      if($catalog->object_type == 'Class'){
        $catalogEntityId = $catalog->class_id;
      } else {
        $catalogEntityId = $catalog->id;
      }

      $catalog->object_type_code = $catalog->object_type == 'Class' ? 'cre_sys_obt_cls' : 'cre_sys_obt_crs';

      // View URL for course and class
      //$viewURL = 'administration/learning/course-class/nojs/addedit/'.$catalog->id.'/'.$catalog->class_id.'/view';
      $viewURL = '';

      $actionList[] = array('action_link_text' => t('LBL063'),
                          'action_page_path' => 'administration/learning/course-class/nojs/addedit',
                          'action_button_params' => core_encrypt($catalog->id),
                          'action_button_params_child' => core_encrypt($catalog->class_id),
                          'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-scroll-wrapper',
                          'row_id' => $catalog->row,
                          'manageby' => $catalog->manageby,
                          'object_type' => $catalog->object_type,
                          'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                           ),
                          'tooltip' => t('LBL063')
                          );

    /*  if(empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {     // This promote/demote will be appear only if Featured Training Enabled
        if($catalog->object_type == 'Course'){
            $actionList[] = array('action_link_text' => (((empty($catalog->promote) || $catalog->promote == 0)) ? t('LBL236') : t('LBL237')),
                                  //'action_link_img' => (((empty($catalog->promote) || $catalog->promote == 0)) ? 'promote-img-class' : 'demote-img-class'),
                              'action_page_path' => 'administration/catalog-promote/nojs',
                              'action_button_params' => $catalogEntityId.'/'.$catalog->object_type.'/'.urlencode(filterSpecialCharacters($catalog->title)),
                                'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
                                  'js_object_info' => array (
                                                        'name' => 'narrowsearch', // provide the java script object name
                                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                                 ),
                                'tooltip' => (((empty($catalog->promote) || $catalog->promote == 0)) ? t('LBL236') : t('LBL237')), //$catalog->promote
                                'action_id' => $catalogEntityId
                              );

        }
        }*/
        $actionList[] = array('action_link_text' => t('LBL286'),
          'action_page_path' => '',
          'action_button_params' => core_encrypt($catalogEntityId),
          'action_button_class_id' => core_encrypt($catalog->class_id),
              'action_button_course_id' => core_encrypt($catalog->id),
              'action_button_param_text' => $catalog->object_type,
              'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
              'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                     ),
                    'tooltip' => t('LBL286')
      );
      /* $actionList[] = array('action_link_text' => t('View'),
            'action_page_path' => '',
            'action_button_params' => $catalogEntityId,
            'action_button_class_id' => $catalog->class_id,
            'action_button_course_id' => $catalog->id,
            'action_button_param_text' => $catalog->object_type,
            'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
            'js_object_info' => array (
                    'name' => 'narrowsearch', // provide the java script object name
                    'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => t('LBL286')
      ); */

      $viewClasslist[] = array('action_link_text' => $catalog->title,
            'action_page_path' => 'administration/view-class/nojs/addedit/'.core_encrypt($catalog->class_id).'',
            //'course_action_page_path' => 'administration/view-class/nojs/addedit/'.$catalog->id.'',
            'action_button_params' =>  core_encrypt($catalog->id),
            'action_button_params_child' => core_encrypt($catalog->class_id),
            'action_button_course_id' => core_encrypt($catalog->id),
            'object_type' => $catalog->object_type,
            'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
            'js_object_info' => array (
                    'name' => 'narrowsearch', // provide the java script object name
                    'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => $catalog->title
      );
      $viewCourselist[] = array('action_link_text' => $catalog->title,
            'action_page_path' => 'administration/view-course/nojs/addedit/'.core_encrypt($catalog->id).'',
            'action_button_params' =>  core_encrypt($catalog->id),
            'action_button_params_child' => core_encrypt($catalog->class_id),
            'action_button_course_id' => core_encrypt($catalog->id),
            'object_type' => $catalog->object_type,
            'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
            'js_object_info' => array (
                    'name' => 'narrowsearch', // provide the java script object name
                    'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
            ),
            'tooltip' => $catalog->title
      );
      if($catalog->object_type == 'Class'){

    /*      $actionList[] = array('action_link_text' => t('Delete'),
                              'action_page_path' => '',
                              'action_button_params' => $catalogEntityId,
                              'entity_type'=>'cre_sys_obt_cls',
                              'ctools_style' => '',
                              'js_object_info' => array (
                                                      'name' => 'narrowsearch', // provide the java script object name
                                                      'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                               ),
                              'tooltip' => t('Delete')
                              );
       }
          $actionList[] = array('action_link_text' => t('LBL238'),
                              'action_page_path' => 'administration/catalog-access/nojs',
                              'action_button_params' => $catalogEntityId.'/'.$catalog->object_type_code.'/'.urlencode(filterSpecialCharacters($catalog->title)),
                              'ctools_style' => 'ctools-modal-ctools-admin-mro-catalog-style',
                              'js_object_info' => array (
                                                      'name' => 'narrowsearch', // provide the java script object name
                                                      'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                               ),
                              'tooltip' => t('LBL238')
                              );

          $actionList[] = array('action_link_text' => t('LBL239'),
                              'action_page_path' => 'administration/mr-role-access/nojs',
                              'action_button_params' => $catalogEntityId.'/'.$catalog->object_type_code.'/'.urlencode(filterSpecialCharacters($catalog->title)),
                              'ctools_style' => 'ctools-modal-ctools-admin-mro-catalog-style',
                              'js_object_info' => array (
                                                      'name' => 'narrowsearch', // provide the java script object name
                                                      'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                               ),
                              'tooltip' => t('LBL239')
                              );


      */
    /*  if($catalog->object_type == 'Course'){

          $promoteAction = array('action_link_text' => '&nbsp;',//(((empty($catalog->promote) || $catalog->promote == 0)) ? t('LBL236') : t('LBL237')),
                              'action_link_img' => (((empty($catalog->promote) || $catalog->promote == 0)) ? 'promote-img-class' : 'demote-img-class'),
                              'action_page_path' => 'administration/catalog-promote/nojs',
                              'action_button_params' => $catalogEntityId.'/'.$catalog->object_type.'/'.trim($catalog->title),
                              'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
                              'js_object_info' => array (
                                                      'name' => 'narrowsearch', // provide the java script object name
                                                      'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                               ),
                              'tooltip' => (((empty($catalog->promote) || $catalog->promote == 0)) ? t('LBL236') : t('LBL237')), //$catalog->promote
                              'action_id' => $catalogEntityId
                              );
      }*/
    }
     $multiActionValues = new StdClass();
     $multiActionValues->entityId = ($catalog->object_type == 'Class') ? $catalog->class_id : $catalog->id;
     $multiActionValues->entityType = ($catalog->object_type == 'Class') ? 'cre_sys_obt_cls' : 'cre_sys_obt_crs';
     $multiActionValues->entityTypeName = $catalog->object_type;
     $multiActionValues->courseId  = $catalog->id;
     if (!empty($catalog->delivery_type)) {
       $multiActionValues->deliveryType  =  $catalog->delivery_type_code;
     }
     if($catalog->delivery_type_code == "lrn_cls_dty_ilt" || $catalog->delivery_type_code == "lrn_cls_dty_vcl"){
        $session_details = getSessionDetailsAdmin($catalog->id,$catalog->class_id,$catalog->delivery_type_code);
     }
     else{
        $session_details = array();
     }

     expDebug::dPrint(' $detailsList---> ' . print_r($detailsList, true) , 4);
      return theme('narrow-search-results-item-details',
                     array(
                            'title' => sanitize_data($catalog->title),
                            //'title' => filterSpecialCharacters($catalog->title),
                           'details_list' => $detailsList,
                           'delete_info' => t('MSG357').' '.$catalog->object_type,
                           //'description' => trim(str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""),$catalog->short_desc)),
                           'description' => $catalog->short_desc,
                                    //'obj_type' => $catalog->object_type,
                           'action_list' => $actionList,
                           'view_class'  => $viewClasslist,
                           'view_list' =>$viewCourselist,
                           'promote_action' => $promoteAction,
                           'view_url' => $viewURL,
                           'entity_multi_action' => $multiActionValues,
                           'session_details' => $session_details,
                           'delivery_type_code' => $catalog->delivery_type_code,
                           'more_type' => (!empty($catalog->delivery_type_code)) ? $catalog->delivery_type_code : $catalog->object_type_code,
                           'sumedit' => $catalog->sumedit,
                           'sumdelete' => $catalog->sumdelete,
                          )
                    );
  } catch (Exception $ex) {
    watchdog_exception('getCatalogDetailsHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCatalogActionsHTML() - Renders an program's actions as HTML to be displayed in jqGrid.
 */
function getCatalogActionsHTML($catalog) {
  try {
      /*return theme('narrow-search-results-item-actions',
                       array('edit_page_path' => base_path().'administration/learning/course-class/nojs/addedit',//?q=administration/learning/course-class/nojs/addedit
                             'edit_button_action_params' => array($catalog->id)
                            )
                  );*/
      return theme('narrow-search-results-item-actions',
                       array('catalog_details' => $catalog)
                  );
  } catch (Exception $ex) {
    watchdog_exception('getCatalogActionsHTML', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * fetchCatalogLanguagesList() - Fetches all program language list.
 */

function fetchCatalogLanguagesList($type) {
  try {
      /*  // Select slt_course_class table
      $select = db_select('slt_course_class', 'cls');

      // Left join the cod and name to get the language name
      $select->leftJoin('slt_profile_list_items', 'pli', 'cls.lang_code = pli.code AND pli.lang_code = \'cre_sys_lng_eng\'');

      // Select fields/expression values to be fetched from the database.
      $select->addField('cls', 'lang_code', 'code');
      $select->addField('pli', 'name', 'name');

      // Order results by name
      $select->orderBy('name');

      expDebug::dPrint('exp_sp_administration_catalog.inc : fetchCatalogLanguagesList() : $select object = ' . print_r($select, true));
      expDebug::dPrint('exp_sp_administration_catalog.inc : fetchCatalogLanguagesList() : $select SQL = ' . print_r($select->__toString(), true));

      $catLangList = $select->execute()->fetchAllKeyed();
      expDebug::dPrint('exp_sp_administration_catalog.inc : fetchCatalogLanguagesList() : $catLangList = ' . print_r($catLangList, true));

      return $catLangList;*/
      if($type == 'Class'){
        $query = "select distinct lang.language_code code, lang.language name from (
                select slpt_li.name as language, slpt_li.code as language_code
                from slt_course_class cls
                left outer join slt_profile_list_items slpt_li on slpt_li.code=cls.lang_code
                where slpt_li.is_active = 'Y' and cls.status != 'lrn_cls_sts_del'
                group by cls.lang_code) lang;";
      } else {
        $query = "select distinct lang.language_code code, lang.language name from (
                select slpt_li.name as language, slpt_li.code as language_code
                from slt_course_template crs
                left outer join slt_profile_list_items slpt_li on slpt_li.code=crs.lang_code
                where slpt_li.is_active = 'Y' and crs.status != 'lrn_crs_sts_del'
                group by crs.lang_code) lang;";
      }
      $select = db_query($query);

      $catLangList = $select->fetchAllKeyed();

      return $catLangList;
  } catch (Exception $ex) {
    watchdog_exception('fetchCatalogLanguagesList', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchCatalogLanguagesList() - Fetches all program language list.
 */

function fetchCatalogCountryList() {
  try {
      // Select slt_course_class table
      $select = db_select('slt_course_class_session', 'ses');

      // Left join to get the location
      $select->leftJoin('slt_facility', 'fac', 'ses.facility_id= fac.id');

      // Left join to get the country name and code
      $select->leftJoin('slt_country', 'sc', 'sc.country_code= fac.contact_country');

      // Select fields/expression values to be fetched from the database.
      $select->addField('sc', 'country_code', 'code');
      $select->addField('sc', 'country_name', 'name');

      $select->where('sc.country_name IS NOT NULL');

      // Order results by name
      $select->orderBy('name');

      expDebug::dPrintDBAPI(' $select object = ' , $select);

      $catContryList = $select->execute()->fetchAllKeyed();
      expDebug::dPrint(' $$catContryList = ' . print_r($catContryList, true) , 3);

      return $catContryList;
  } catch (Exception $ex) {
    watchdog_exception('fetchCatalogCountryList', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchCatalogTypes() - Fetches all delivery type list.
 */

function fetchCatalogTypes() {
  try {
      // Select slt_profile_list_items table
      $select = db_select('slt_profile_list_items', 'splt');

      // Select fields/expression values to be fetched from the database.
      //$select->addField('splt','code', 'code');
      $select->addField('splt','name', 'code');
      $select->addField('splt','name', 'name');


      $select->condition(db_or()->
          condition('splt.code', 'cre_sys_obt_crs' , '=')->
          condition('splt.code', 'cre_sys_obt_cls' , '='));

      // Order results by name
      $select->orderBy('name', 'DESC');

      expDebug::dPrintDBAPI(' $select object = ' , $select);

      $overallDeliveryList = $select->execute()->fetchAllKeyed();
      expDebug::dPrint('  $overallDeliveryList = ' . print_r( $overallDeliveryList, true) , 3);

      return $overallDeliveryList;
  } catch (Exception $ex) {
    watchdog_exception('fetchCatalogTypes', $ex);
    expertusErrorThrow($ex);
  }
}




/**
 * setCatalogOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function setCatalogOrderByClause($selectStmt, $tableAlias) {
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
    watchdog_exception('setCatalogOrderByClause', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * sendCatalogListToJqGrid() - Prepares JSON response to JqGrid's request for program
 */
function sendCatalogListToJqGrid() {
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
            $total_pages = 0;
      
      // SOLR check starts
        $solrSource = getConfigValue('solr_search');
            if($solrSource == 0){
      /*$numCatalog = fetchCatalogAll('COUNT');
      expDebug::dPrint(' $numCatalog = ' . print_r($numCatalog, true) , 4);

      $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numCatalog,true);
      expDebug::dPrint(' $start = ' . print_r($start, true) , 4); */
      $start = calculate_offset($page, $limit);

      $catList = fetchCatalogAll('LIST', $start, $limit);
      expDebug::dPrint(' $prgList = ' . print_r($catList, true) , 4);
            }else{
                require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_admin.php';
                try{
                    $solrObj = new AdminCatalogSolrSearch();
                    $solrList = $solrObj->getCourseList();
                    $catList = $solrList['records'];
                    expDebug::dPrint(' $prgList = ' . print_r($catList, true) , 4);
                    $numCatalog = $solrList['recCount'];
                    $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numCatalog);
                }catch(Exception $e){
                    expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
                }
                expDebug::dPrint("Solr search result - Final ".print_r($qry_search_results,1),4);
            }
      // Prepare and send the response
      $response=new stdClass();
//       $response->page = $page;
//       $response->total = $total_pages;
//       $response->records = $numCatalog;

      //$jsObjectInfo = programJSObjectInfo();
      //$response->js_object_info = $jsObjectInfo;

      $response->initial_sort_type_html_id = 'cat-sort-new';

      $i=0;
      $catList1=$catList[0];

      // Mananged By behavior related code
      $userData = new stdClass();
      $catalogType = isset($_GET['catalogtype']) ? $_GET['catalogtype'] : 'Course';
      if ($catalogType == 'Course') {
        $userData->adminPage = 'course';
        $userData->managedBy = $catList[1];
      }
      else {
        $userData->adminPage = 'class';
        $userData->managedBy = $catList[2];
      }
      $response->userdata = $userData;

      foreach ($catList1 as $catalog) {
        expDebug::dPrint('$catalog = '. print_r($catalog, true) , 4);

        $catalog->row =  $i;
        $response->rows[$i]['id']   = $catalog->id;
        if(!empty($catalogType) && $catalogType == 'Class'){
            $response->rows[$i]['jqgrid-rowid']   = $catalog->class_id;
        }
        $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $catalog)),
                                             'details' => getCatalogDetailsHTML($catalog),
                                             'action' => getCatalogActionsHTML($catalog)
                                             );

        $i++;
      }

      expDebug::dPrint('$response record = ' . print_r($response, true), 4);

      echo drupal_json_encode($response);
  }
  catch (Exception $ex) {
    watchdog_exception('sendCatalogListToJqGrid', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getCatalogAutoComplete() - Returns the matched Program's title
 */

function getCatalogAutoComplete()
{
  try {
        $solrSource = getConfigValue('solr_search');
        
        if($solrSource == 0){
      
        $searchCurText =  $_GET['z'];

      // Select the table
      $select = db_select('slt_master_search', 'mas');
      //$select->leftjoin('slt_tag_entity', 'te','((mas.parent_pk=te.entity_id AND te.entity_type=\'Course\') OR (mas.pk_id= te.entity_id AND te.entity_type=mas.type))');
      //$select->leftjoin('slt_tagdefn', 'td','te.tagid= td.id');

      $select->leftJoin('slt_tag_entity', 'te', '(mas.pk_id = te.entity_id AND te.entity_type = mas.type)');
      $select->leftJoin('slt_tagdefn', 'td', 'te.tagid = td.id');
      $select->leftJoin('slt_tag_entity', 'te2', '(mas.parent_pk = te2.entity_id AND te2.entity_type = \'Course\')');
      $select->leftJoin('slt_tagdefn', 'td2', 'te2.tagid = td2.id');

      // Add fields to fetch
      $select->addField('mas', 'name', 'title');
      $select->addField('mas', 'type', 'type');
     // $select->addField('td','tagname', 'tagname');

      // Add condition
      $select->condition(db_or()
                              ->condition('mas.name', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('mas.code', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('mas.description', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('td.tagname', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('td2.tagname', '%' . db_like($searchCurText) . '%', 'LIKE')
                        );
      $select->condition('mas.type',array('Class','Course'),'IN');
      $select->condition('mas.status',array('lrn_crs_sts_itv','lrn_crs_sts_atv','lrn_cls_sts_atv','lrn_cls_sts_itv'),'IN');
     // $select->condition('mas.status',array('lrn_crs_sts_del','lrn_cls_sts_del'),'NOT IN');
      $select->groupBy('mas.pk_id');

      filterByUserAccessPrivileges($select,'','',1,'','Search','mas');

      // Order in dictionary sort order
      $select->orderBy('mas.name');

      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      $matchingCatalogNames = $select->execute()->fetchAll();
      expDebug::dPrint(' $matchingCatalogNames = '. print_r($matchingCatalogNames, true) , 3);
        }
        else {
            expDebug::dPrint ( 'entered solr search' );
            require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_admin.php';
            try {
                $solrObj = new AdminCatalogSolrSearch ();
                $solrList = $solrObj->getCatalogAutoComplete ();
                expDebug::dPrint ( ' $prgList123 = ' . print_r ( $solrList, true ), 4 );
                $matchingCatalogNames = $solrList ['records'] [0];
                expDebug::dPrint ( ' $prgList123 = ' . print_r ( $matchingCatalogNames, true ), 4 );
            } catch ( Exception $e ) {
                expDebug::dPrint ( "Error in Solr Search for Users - " . print_r ( $e->getMessage (), true ), 1 );
            }
        }

      foreach ( $matchingCatalogNames as $catName) {
        print rawurlencode($catName->title) ." - <b>".rawurlencode($catName->type). "</b>\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getCatalogAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

function getAdminCatalogAutoCompleteAPI()
{
    try {
        $searchCurText =  $_REQUEST['searchtxt'];

        // Select the table
        $select = db_select('slt_master_search', 'mas');

        // Add fields to fetch
        $select->addField('mas', 'name', 'title');
        $select->addField('mas', 'type', 'type');
        $select->addField('mas', 'code', 'code');

        // Add condition
        $select->condition(db_or()
                ->condition('mas.name', '%' . db_like($searchCurText) . '%', 'LIKE')
                ->condition('mas.code', '%' . db_like($searchCurText) . '%', 'LIKE')
                ->condition('mas.description', '%' . db_like($searchCurText) . '%', 'LIKE')
        );
        $select->condition('mas.type',array('Class','Course'),'IN');
        $select->condition('mas.status',array('lrn_crs_sts_itv','lrn_crs_sts_atv','lrn_cls_sts_atv','lrn_cls_sts_itv'),'IN');

        // Order in dictionary sort order
        $select->orderBy('name');

        expDebug::dPrintDBAPI(' $select obj = ' , $select);

        $matchingCatalogNames = $select->execute()->fetchAll();
        return $matchingCatalogNames;
    } catch (Exception $ex) {
        watchdog_exception('getAdminCatalogAutoCompleteAPI', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * getSessLocationAutoComplete() - Returns the matched location's name
 */
function getSessLocationAutoComplete() {
  try {
      $searchText =  $_GET['z'];

      // Select the table
      $select = db_select('slt_location', 'loc');

      // Add fields to fetch
      $select->addField('loc', 'name', 'name');

      // Add condition
      $select->condition(db_or()
                              ->condition('loc.name', '%' . db_like($searchText) . '%', 'LIKE')
                              ->condition('loc.city', '%' . db_like($searchText) . '%', 'LIKE')
                              ->condition('loc.state', '%' . db_like($searchText) . '%', 'LIKE')
                        );


      // Order in dictionary sort order
      $select->orderBy('name');

      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      $matchingLocationNames = $select->execute()->fetchAll();
      expDebug::dPrint(' $matchingLocationNames = '. print_r($matchingLocationNames, true) , 3);

      foreach ($matchingLocationNames as $locationName) {
        print $locationName->name . "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getSessLocationAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchCatalogAll() - Fetches program count / program list for jqgrid after applying the narrow search filters
 */
function fetchCatalogAll($op, $start='', $limit='',$callFrom='') {
  try {
    //$userId = getSltpersonUserId();
      $catalogType          = isset($_GET['catalogtype']) ? $_GET['catalogtype'] : (isset($_POST['catalogtype']) ? $_POST['catalogtype'] : '');
      $catalogStatusAPI     = isset($_REQUEST['catalogcoursestatus']) ? $_REQUEST['catalogcoursestatus'] : (isset($_REQUEST['catalogclassstatus']) ? $_REQUEST['catalogclassstatus'] : '');
      if(!empty($catalogType) && $catalogType=='Class'){
            /* $classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = crs.id AND group_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=crs.id
            AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=crs.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';
        $select = db_select('slt_course_class', 'crs');
        $select->leftJoin('slt_group_mapping','group_map','('.$classsrQuery.') and group_map.group_type = 1');
        $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
        $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_crs\'');       */
            $select = db_select('slt_course_class', 'crs');
            //join slt_tag_entity and slt_tagdefn tables only when cloud tag search or top search happens
            if((isset($_REQUEST['textfilter']) && ! empty($_REQUEST['textfilter'])) || (isset($_GET['catalogclasstag']) && !empty($_GET['catalogclasstag']))) {
                $select->leftJoin('slt_tag_entity', 'te', '(crs.id = te.entity_id and te.entity_type  = \'Class\')');
                $select->leftJoin('slt_tagdefn', 'td', 'te.tagid = td.id');
                $select->leftJoin('slt_tag_entity', 'te2', '(te2.entity_id = crs.course_id and te2.entity_type = \'Course\')');
                $select->leftJoin('slt_tagdefn', 'td2', 'te2.tagid = td2.id');
            }
          if ($op == "COUNT") {
            // Get count of active program
            $select->addExpression('COUNT(DISTINCT(crs.id))', 'count');
            // $select->leftJoin('slt_tag_entity','te','((crs.id=te.entity_id and te.entity_type=\'Class\') OR (crs.course_id=te.entity_id and te.entity_type=\'Course\'))');
            // $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
          }
          else { // LIST
            $select->leftJoin('slt_profile_list_items', 'plistatus', 'crs.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
            $select->leftJoin('slt_profile_list_items', 'plilang', 'crs.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
            $select->leftJoin('slt_profile_list_items', 'plidelivery', 'crs.delivery_type = plidelivery.code AND plidelivery.lang_code = \'cre_sys_lng_eng\'');
            $select->leftJoin('slt_profile_list_items', 'plicurr', 'crs.currency_type = plicurr.attr1 AND plicurr.lang_code = \'cre_sys_lng_eng\'');
            // $select->leftJoin('slt_tag_entity','te','((crs.id=te.entity_id and te.entity_type=\'Class\') OR (crs.course_id=te.entity_id and te.entity_type=\'Course\'))');
            // $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
            //$select->leftJoin('slt_promoted_highly_rated', 'promo', 'promo.activity_id= crs.id and promo.entity_type = \'Class\'');

            // Select fields/expression values to be fetched from the database.
            //$select->addExpression('count(1)', 'priv_count');
           // $select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
           // $select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
            $select->addField('crs', 'course_id', 'id');
            $select->addField('crs', 'is_compliance', 'isCompliance');
            $select->addField('crs', 'title', 'title');
            $select->addField('crs', 'code', 'code');
            $select->addField('crs', 'short_description', 'short_desc');
            $select->addField('crs', 'status', 'status_code');
            $select->addField('crs', 'additional_info', 'additional_info');
            $select->addField('crs', 'addn_catalog_show', 'shown_in_catalog');
            $select->addField('crs', 'addn_notification_show', 'shown_in_notification');
            $select->addField('plistatus', 'name', 'status');
            $select->addField('plilang', 'name', 'lang');
            //$select->addField('promo', 'status', 'promote');
            $select->addField("plidelivery",'name', 'delivery_type');
            $select->addField("crs",'id', 'class_id');
            $select->addField('crs',"delivery_type", 'delivery_type_code');
            //$select->addField('td','tagname', 'tagname');
            $select->addExpression("''", 'facility_name');
            //$select->addExpression("''", 'location_name');
            //46579: PS- List of Classes Doesnot Return Location Name for ILT Class.
            if ($callFrom == 'API')
            {
                $select->addExpression("(SELECT distinct name FROM slt_location as loc left join slt_course_class_session as sccs on sccs.location_id = loc.id where sccs.class_id = crs.id)", 'location_name');
                $select->addField('crs', 'currency_type', 'currency');
                $select->addField('plicurr', 'code', 'currency_code');
                $select->addField('plicurr', 'attr2', 'currency_symbol');
            }
            else
                $select->addExpression("''", 'location_name');

            $select->addField('crs', 'price','price');
            $select->addExpression("'Class'", 'object_type');
            $select->groupBy('crs.id');

            if ($callFrom == 'API') {
                $select->addField('crs', 'scheduled_duration', 'duration');
                $select->addField('crs', 'author_vendor', 'author_vendor');
                $select->addField('crs', 'is_compliance', 'is_compliance');

            }
          }
         /*  $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = crs.id AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=crs.id
          AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=crs.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))'; */
          filterByUserAccessPrivileges($select, 'crs','',1,'','Class','crs');

      }else{
        $select = db_select('slt_course_template', 'crs');
        $select->leftJoin('slt_course_class', 'crscls', 'crscls.course_id = crs.id');
        /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\' and group_map.group_type = 1');
        $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
        $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_crs\''); */
        
        if((isset($_REQUEST['textfilter']) && ! empty($_REQUEST['textfilter'])) || (isset($_GET['catalogcoursetag']) && !empty($_GET['catalogcoursetag']))) {
            $select->leftJoin('slt_tag_entity', 'te', '(crs.id = te.entity_id and te.entity_type = \'Course\')');
            $select->leftJoin('slt_tagdefn', 'td', 'te.tagid= td.id');
        }
          if ($op == "COUNT") {
            // Get count of active program
            $select->addExpression('COUNT(DISTINCT(crs.id))', 'count');
            // $select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
            // $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
          }
          else { // LIST
            $select->leftJoin('slt_profile_list_items', 'plistatus', 'crs.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
            $select->leftJoin('slt_profile_list_items', 'plilang', 'crs.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
            // $select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
            // $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
            //$select->leftJoin('slt_promoted_highly_rated', 'promo', 'promo.activity_id= crs.id and promo.entity_type = \'Course\'');

            // Select fields/expression values to be fetched from the database.
            //$sumEdit = if(group_map.id is not null,sum(ifnull(priv.edit,0)),1);
            //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
            //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
            $select->addField('crs', 'id', 'id');
            $select->addField('crs', 'title', 'title');
            $select->addField('crs', 'code', 'code');
            $select->addField('crs', 'short_description', 'short_desc');
            $select->addField('crs', 'status', 'status_code');
            $select->addField('plistatus', 'name', 'status');
            $select->addField('plilang', 'name', 'lang');
        //  $select->addField('td','tagname', 'tagname');
            //$select->addField('promo', 'status', 'promote');
            $select->addExpression("''", 'delivery_type');
            $select->addExpression("''", 'class_id');
            $select->addExpression("''", 'delivery_type_code');
            $select->addExpression("''", 'facility_name');
            //$select->addExpression("''", 'location_name');
            //46579: PS- List of Classes Doesnot Return Location Name for ILT Class.
            if ($callFrom == 'API')
                $select->addExpression("(SELECT distinct name FROM slt_location as loc left join slt_course_class_session as sccs on sccs.location_id = loc.id where sccs.class_id = crs.id)", 'location_name');
            else
                $select->addExpression("''", 'location_name');
                $select->addExpression("'Course'", 'object_type');
                $select->addField('crs', 'price', 'price');
                $select->addField('crs', 'currency_type', 'currency_type');


            $select->groupBy('crs.id');
          }
          filterByUserAccessPrivileges($select, 'crs','',1,'cre_sys_obt_crs','CourseList','crs');
        //  $classsr_Query = 'gmap.entity_id=crs.id AND gmap.entity_type = \'cre_sys_obt_crs\'';
      }

     /*  if($GLOBALS["user"]->uid != '1'){
        $select->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=1 limit 1),1=1))');
      } */


      if($catalogType == 'Class') {
        $defaultCatalogStatus = isset($_GET['catalogclassstatus']) ? $_GET['catalogclassstatus'] : (isset($_POST['catalogclassstatus']) ? $_POST['catalogclassstatus'] : '');
        selectAddNarrowSearchINCondition($select, 'crs', 'delivery_type', 'deliverytype');
        selectAddNarrowSearchINCondition($select, 'crs', 'lang_code', 'classlangtype');
        selectAddNarrowSearchINCondition($select, 'crs', 'status', 'catalogstatus',$defaultCatalogStatus);

        $defaultClassCurrency = isset($_GET['classcurrency']) ? $_GET['classcurrency'] : (isset($_POST['classcurrency']) ? $_POST['classcurrency'] : '');
        selectAddNarrowSearchINCondition($select, 'crs', 'currency_type', 'classcurrency',$defaultClassCurrency);

        if(isset($_GET['catalogclasstag']) && !empty($_GET['catalogclasstag'])){
         /* $tagSubquery = db_select('slt_tag_entity', 'tagentity');
          $tagSubquery->leftJoin(slt_tagdefn, 'tagdefn', 'tagentity.tagid = tagdefn.id');
          $tagSubquery->addExpression('tagentity.id', 'teid');*/
          /* selectAddNarrowSearchByTextConditionExactMatch($select, 'td', 'tagname', 'catalogclasstag', t('LBL193')); // search by attached tagName
          $select->condition(db_or()->where('crs.id = te.entity_id AND te.entity_type = \'Class\'')
                                         ->where('crs.course_id = te.entity_id AND te.entity_type = \'Course\''));
          $select->range(0,1); */
            
            $textFilterStr = getRawURLParam('catalogclasstag');
            if(!empty($textFilterStr) && $textFilterStr != t('LBL193')){
                $select->condition(db_or()
                        ->condition('td.tagname', $textFilterStr, '=')
                        ->condition('td2.tagname', $textFilterStr, '='));
            }
           // $select->condition('', $tagSubquery, 'EXISTS');
        }
        //selectAddNarrowSearchINCondition($select, 'fac', 'contact_country', 'countrytype'); // search by country
        if(isset($_REQUEST['classLocation']) && !empty($_REQUEST['classLocation'])){
            $classloc = $_REQUEST['classLocation'];
            $select->leftjoin('slt_location', 'loc', 'loc.id = crs.location_id');
            //$select->leftjoin('slt_state', 'st','loc.state = st.state_code and st.state_name like \'%'.db_like($classloc).'%\'');

            /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
             
            $select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');  
            $select->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
            selectAddNarrowSearchByLocationCondition($select, 'classLocation', t('LBL1321')); // search by class location   
        }

        selectAddNarrowSearchSliderInCatalog($select, 'crs', 'price', 'classPrice',$classmax);
        if(isset($_GET['classdaterange']) && !empty($_GET['classdaterange'])){
            $select->leftJoin('slt_course_class_session', 'ses', 'crs.id=ses.class_id AND ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=ses.class_id ORDER BY start_date LIMIT 1)');
            $select->leftJoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id = ses.id');
            selectAddNarrowSearchByDateRangeCondition($select, 'ses', 'start_date', 'classdaterange');
        }
       if(isset($_GET['classContent']) && !empty($_GET['classContent'])){
        $select->leftJoin('slt_course_content_mapper', 'maps1', 'maps1.class_id = crs.id');
        $select->leftJoin('slt_content_master', 'tmplt1', 'tmplt1.id = maps1.content_id');
        selectAddNarrowSearchByTextCondition($select, 'tmplt1', 'code', 'classContent', t('LBL756'));
        }
       if(isset($_GET['classSurvey']) && !empty($_GET['classSurvey'])){
        $select->leftJoin('slt_survey_mapping', 'maps2', 'maps2.object_id = crs.id AND maps2.object_type=\'cre_sys_obt_cls\'');
        $select->leftJoin('slt_survey', 'tmplt2', 'tmplt2.id = maps2.survey_id');
        //selectAddNarrowSearchByTextCondition($select, 'tmplt2', 'title', 'classSurvey', t('LBL545'));
        selectAddNarrowSearchByTextMultipleORCondition($select, 'tmplt2', array('title', 'code'),'classSurvey',t('LBL545'));
        }
      if(isset($_GET['classAssessment']) && !empty($_GET['classAssessment'])){
        $select->leftJoin('slt_survey_mapping', 'maps3', 'maps3.object_id = crs.id AND maps3.object_type=\'cre_sys_obt_cls\'');
        $select->leftJoin('slt_survey', 'tmplt3', 'tmplt3.id = maps3.survey_id');
        //selectAddNarrowSearchByTextCondition($select, 'tmplt3', 'title', 'classAssessment', t('LBL545'));
        selectAddNarrowSearchByTextMultipleORCondition($select, 'tmplt3', array('title', 'code'),'classAssessment',t('LBL545'));
        }
        selectAddNarrowSearchByInstructorTextCondition($select, 'ses_ins', 'classInstructor', t('LBL259')); // search by instructor username

         if ($callFrom == 'API' && $catalogStatusAPI) {
            $select->condition('crs.status', $catalogStatusAPI );
         } else {
            $select->condition('crs.status', 'lrn_cls_sts_del', '!=');
         }
        // Check added by Vincent on Dec 2, 2013
        // Manage by fillter is not require for API calls
        $manageClsfilter = '';
        if ($callFrom != 'API') {
            $manageClsfilter = selectAddNarrowSearchManageByCondition('class', $select, 'crs', 'classmanage');
        }
      } else {
        $defaultCatalogStatus = isset($_GET['catalogcoursestatus']) ? $_GET['catalogcoursestatus'] : $_GET['catalogcoursestatus'];
        selectAddNarrowSearchINCondition($select, 'crs', 'lang_code', 'courselangtype');
        selectAddNarrowSearchINCondition($select, 'crs', 'status', 'catalogstatus',$defaultCatalogStatus);
        // Added by Gayathri for 0070892
        $defaultCourseCurrency = isset($_GET['coursecurrency']) ? $_GET['coursecurrency'] : (isset($_POST['coursecurrency']) ? $_POST['coursecurrency'] : '');
        //expDebug::dPrint('$defaultCourseCurrency'.$defaultCourseCurrency,5);
        selectAddNarrowSearchINCondition($select, 'crs', 'currency_type', 'coursecurrency',$defaultCourseCurrency);
        //expDebug::dPrintDBAPI('$defaultCourseCurrency query :::: ',$select,5);
        
        selectAddNarrowSearchSliderInCatalog($select, 'crs', 'price', 'coursePrice',$classmax);
        
        if(isset($_GET['catalogcoursetag']) && !empty($_GET['catalogcoursetag'])){
          //  $select->leftjoin('slt_tag_entity', 'te','(crs.id=te.entity_id AND te.entity_type=\'Course\')');
          //    $select->leftjoin('slt_tagdefn', 'td','te.tagid= td.id');
            selectAddNarrowSearchByTextConditionExactMatch($select, 'td', 'tagname', 'catalogcoursetag', t('LBL193')); // search by attached tagName
        }

        if ($callFrom == 'API' && $catalogStatusAPI) {
            $select->condition('crs.status', $catalogStatusAPI );
        } else {
            $select->condition('crs.status', 'lrn_crs_sts_del', '!=');
        }

        // Check added by Vincent on Dec 2, 2013
        // Manage by fillter is not require for API calls
        $manageCrsfilter = '';
        if ($callFrom != 'API') {
            $manageCrsfilter = selectAddNarrowSearchManageByCondition('course', $select, 'crs', 'coursemanage');
        }
      }
      if($callFrom == 'API') { //Added by Rajeshwar
        $con = ($catalogType=='Class') ? ' ((crs.id=ent.entity_id and ent.entity_type=\'Class\') OR (crs.course_id=ent.entity_id and ent.entity_type=\'Course\'))':'(crs.id=ent.entity_id and ent.entity_type=\'Course\')';
        $select->addExpression("(select GROUP_CONCAT(def.tagname) FROM slt_tag_entity ent 
                                    LEFT OUTER JOIN slt_tagdefn def on def.id = ent.tagid
                                    where $con)","tagname");
        $tag=trim($_REQUEST['tagname']);
        if(isset($tag) && !empty($tag))
            $select->condition('td.tagname',$tag,'=');
        
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Adding Custom Attribute Fields to Display Columns
            $ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){ 
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc'); 
            if($catalogType=='Class')
            {
            $attrValues = getCustomAttributeValues('Class','mandatory'); 
            }
            else {
            $attrValues = getCustomAttributeValues('Course','mandatory');
            }
            foreach($attrValues as $ckey => $cvalue){
            	$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0` 
            			WHERE FIND_IN_SET(`opt0`.`opt_code`,`crs`.`$cvalue->attributerefercolumn`)),`crs`.`$cvalue->attributerefercolumn`),`crs`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));
            }
                        expDebug::dPrint(' $_POST' . print_r($_POST, true) , 5);
        
            foreach($_POST as $key=>$value) {
                expDebug::dPrint(' $value ' . print_r($value, true) , 5);
                
                if(strpos($key,'c_')===0 && $value !=''){
                    expDebug::dPrint(' $key ' . print_r($key, true) , 5);
        
                    //$entityMappingColumn = getCustomAttributeMappingColumn($key);
                    if($catalogType=='Class')
                    $entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_sys_obt_cls');
                    else if($catalogType=='Course')
                    $entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_sys_obt_crs');
                    $per_column = 'crs.'.$entityMappingColumn['crefercolumn'];
                    expDebug::dPrint(' $per_column' . print_r($per_column, true) , 5);                                       
        
                       		$custom_attr_option_code=$per_column." LIKE '%".$value."%'";
		                   	
		                   	//$customattr_opt_o_pos = strpos($value, ','); //for multiple option
		                   	expDebug::dPrint(' $customattr_opt_o_pos' . print_r($customattr_opt_o_pos, true) , 5);
		                   		include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
		                   		$cus_attr_opt_code_arr=explode(",",$value);
		                   		if(count($cus_attr_opt_code_arr)>0){
		           		     			foreach($cus_attr_opt_code_arr as $o_key => $o_val){
		                   				expDebug::dPrint(' $$o_val' . print_r($o_val, true) , 5);
		                   				if($o_val!='')
		                   				{
		                   				$custom_attr_option_code.=" OR ".$per_column." LIKE '%".getCustomAttributeCode($o_val,'o')."%'";
		                   				}
		                   			}
		                   		}
		                   
		                   	$select->where($custom_attr_option_code); 
                                       
                
                }
            }
        } //#custom_attribute_0078975 - End Check module status
        
      }
        if(isset($_REQUEST['textfilter']) && ! empty($_REQUEST['textfilter'])) {
            $searchText = $_REQUEST['textfilter'];
            $tableAliasFields = array('crs.description', 'crs.code', 'crs.title', 'td.tagname');
            if($catalogType=='Class') {
                $tableAliasFields[] = 'td2.tagname';
            }
            SearchByFilterForMultipleTableORCondition($select, $searchText, 'Search', $tableAliasFields);
        }
      //selectAddNarrowSearchINCondition($select, 'mas', 'type', 'catalogtype');
      expDebug::dPrint(' $mutipleTextFilterStr = $$callFrom' . print_r($callFrom, true) , 4);
      
     /* if ($callFrom == 'API') {
        selectAddNarrowSearchByTextMultipleORCondition($select, 'crs', array('title', 'code', 'short_description','description'),'textfilter','',1); // search by title in the header
      } else{   
        selectAddNarrowSearchByTextMultipleORCondition($select, 'crs', array('title', 'code', 'short_description','description')); // search by title in the header
      }*/
      if ($op != "COUNT") {
        // Order the records
        setCatalogOrderByClause($select, 'crs', "AZ");
        // Group by mas.id to eliminate duplicate program records
        //$select->groupBy('crs.id');
      }

      expDebug::dPrintDBAPI(' $select SQL = ' , $select);

      if($op=="COUNT"){
        // Execute the query
          $newResult = $select->execute()->fetchField();
          expDebug::dPrint(' COUNT $result = ' . print_r($newResult, true) , 3);
      }
      elseif($op == "CSV"){
      if ($catalogType == 'Class') {
      $colHeadersMap = array(t('LBL083') => 'title',
                             t('LBL036') => 'object_type',
                             t('LBL096') => 'code',
                             t('LBL038') => 'lang',
                             t('LBL102') => 'status',
                             //t('LBL040') => 'price',
                             t('LBL084') => 'delivery_type',
                       );
      // Applied the CSV filter option
      $filterArray = array();
      $filterArray[] = array(t('Class').':'.textboxFiltersetValueToPrintableStr('textfilter'));
      $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('catalogclassstatus'));
      $filterArray[] = array(t('LBL084').':'.checkboxFiltersetSelectionsToPrintableStr('deliverytype'));
      if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {
         $filterArray[] = array(t('LBL040').':'.sliderFiltersetValueToPrintableStr('classPrice'));
         $filterArray[] = array(t('LBL040').':'.sliderFiltersetValueToPrintableStr('coursePrice')); // Added by Gayathri for 0070892
          
      }
      $filterArray[] = array(t('LBL101').':'.checkboxFiltersetSelectionsToPrintableStr('classcurrency'));
      $filterArray[] = array(t('LBL101').':'.checkboxFiltersetSelectionsToPrintableStr('coursecurrency')); // Added by Gayathri for 0070892
      $filterArray[] = array(t('LBL042'). ' (' . t('LBL043') . ')'.':'.daterangeFiltersetValueToPrintableStr('classdaterange'));
      $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('classlangtype'));
      $filterArray[] = array(t('Content').':'.textboxFiltersetValueToPrintableStr('classContent'));
      $filterArray[] = array(t('Survey').':'.textboxFiltersetValueToPrintableStr('classSurvey'));
      $filterArray[] = array(t('Assessment').':'.textboxFiltersetValueToPrintableStr('classAssessment'));
      $filterArray[] = array(t('LBL191').':'.textboxFiltersetValueToPrintableStr('catalogclasstag'));
      $filterArray[] = array(t('Location').':'.textboxFiltersetValueToPrintableStr('classLocation')); //Added by ganeshbabuv on March 15th 2017  for  Location Filter #66404
      $filterArray[] = array(t('Instructor').':'.textboxFiltersetValueToPrintableStr('classInstructor'));
      $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
      // manage By in sort option
      $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('classmanage'));
      }
      else {
      $colHeadersMap = array(t('LBL083') => 'title',
                             t('LBL036') => 'object_type',
                             t('LBL096') => 'code',
                             t('LBL038') => 'lang',
                             t('LBL102') => 'status',
                             // t('LBL040') => 'price',
                             //t('LBL084') => 'delivery_type',
                        );
     // Applied the CSV filter option
     $filterArray[] = array(t('Course').':'.textboxFiltersetValueToPrintableStr('textfilter'));
     $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('catalogcoursestatus'));
     $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('courselangtype'));
     $filterArray[] = array(t('LBL191').':'.textboxFiltersetValueToPrintableStr('catalogcoursetag'));
     // Array of ucfirst is not showing sort by to CSV File in admin Course page
     // $filterArray[] = ucfirst(array(ucfirst(t('LBL011'))).':'.getNarrowSearchSortTitle());
     $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
     // manage By in sort option
     $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('coursemanage'));

      }
      SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap,'export',$filterArray);

      }
    elseif ($op == "PDF"){
      if ($catalogType == 'Class') {
        // % width of the columns must total to 100%
        $colHeadersMap = array(t('LBL083') => array('title',         22),
                               t('LBL036') => array('object_type',   20),
                               t('LBL096') => array('code',          15),
                               t('LBL038') => array('lang',          13),
                               t('LBL102') => array('status',        12),
                               t('LBL084') => array('delivery_type', 18),
                               //t('LBL040') => array('price',10), Adjust % column widths of all columns when uncommenting
                         );

        $appliedFilters = array();
        $appliedFilters[t('Class')] = textboxFiltersetValueToPrintableStr('textfilter');
        $appliedFilters[t('LBL102')] = checkboxFiltersetSelectionsToPrintableStr('catalogclassstatus');
        $appliedFilters[t('LBL084')] = checkboxFiltersetSelectionsToPrintableStr('deliverytype');
        if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {
          $appliedFilters[t('LBL040')] = "".sliderFiltersetValueToPrintableStr('classPrice');
          $appliedFilters[t('LBL040')] = "".sliderFiltersetValueToPrintableStr('coursePrice'); // Added by Gayathri for 0070892
        }
        $appliedFilters[t('LBL101')] = checkboxFiltersetSelectionsToPrintableStr('classcurrency');
        $appliedFilters[t('LBL101')] = checkboxFiltersetSelectionsToPrintableStr('coursecurrency'); // Added by Gayathri for 0070892
        $appliedFilters[t('LBL042') . ' (' . t('LBL043') . ')'] = daterangeFiltersetValueToPrintableStr('classdaterange');
        $appliedFilters[t('LBL038')] = checkboxFiltersetSelectionsToPrintableStr('classlangtype');
        $appliedFilters[t('Content')] = textboxFiltersetValueToPrintableStr('classContent');
        $appliedFilters[t('Survey')] = textboxFiltersetValueToPrintableStr('classSurvey');
        $appliedFilters[t('Assessment')] = textboxFiltersetValueToPrintableStr('classAssessment');
        $appliedFilters[t('LBL191')] = textboxFiltersetValueToPrintableStr('catalogclasstag');
        $appliedFilters[t('Location')] = textboxFiltersetValueToPrintableStr('classLocation'); //Added by ganeshbabuv on March 15th 2017  for  Location Filter #66404
        $appliedFilters[t('Instructor')] = textboxFiltersetValueToPrintableStr('classInstructor');
        $appliedFilters[ucwords(t('LBL011'))] = ucwords(getNarrowSearchSortTitle());
        // manage By in sort option
        $appliedFilters[t('Managed By')] = checkboxFiltersetSelectionsToPrintableStr('classmanage');
        SendNarrowSearchResultsAsPDFFile($select, t('Class'), $appliedFilters, $colHeadersMap);
      }
      else {
        // % width of the columns must total to 100%
        $colHeadersMap = array(t('LBL083') => array('title',         25),
                               t('LBL036') => array('object_type',   25),
                               t('LBL096') => array('code',          20),
                               t('LBL038') => array('lang',          15),
                               t('LBL102') => array('status',        15),
                               //t('LBL040') => array('price',10), Adjust % column widths of all columns when uncommenting
                         );
        $appliedFilters = array(t('Course') => textboxFiltersetValueToPrintableStr('textfilter'),
                                 t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('catalogcoursestatus'),
                                 t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('courselangtype'),
                                 t('LBL191') => textboxFiltersetValueToPrintableStr('catalogcoursetag'),
                                 ucwords(t('LBL011')) => ucwords(getNarrowSearchSortTitle()),
                                 t('Managed By') => checkboxFiltersetSelectionsToPrintableStr('coursemanage')
                               );

        SendNarrowSearchResultsAsPDFFile($select, t('Course'), $appliedFilters, $colHeadersMap);
      }

      }
      else{ // LIST
          // Execute the query and fetch all the records
          // Limit to $limit records

          //$cntResults=$select->countQuery()->execute()->fetchField();
//            $cntResults=$select->execute()->fetchField();
          if (is_numeric($limit) && is_numeric($start)) {
            $select->range($start, $limit);
          }
          add_row_id_condition($select, 'crs');
          //print $select;
          expDebug::dPrintDBAPI(' with range LIST $select = ' , $select);
          $result = $select->execute()->fetchAll();
          $newResult[0]=$result;
          $newResult[1]=$manageCrsfilter;
          $newResult[2]=$manageClsfilter;
          //$newResult[3]=$cntResults;
          $newResult[3]=count($result);
          expDebug::dPrint('  LIST $result = ' . print_r($result, true) , 3);

      }
     //print $select;
     return $newResult;
  } catch (Exception $ex) {
    watchdog_exception('fetchCatalogAll', $ex);
    expertusErrorThrow($ex);
  }
}

function getMaxClassPrice(){
  try {
    $userDefaultCurrencyArr = getUserDefaultCurrency();
    $userPrefCurrCode = $userDefaultCurrencyArr['currency_code'];

     $select = db_select('slt_course_class', 'cls');
     $select->addExpression('slf_convert_price(cls.price, cls.currency_type,\''.$userPrefCurrCode.'\' )', 'maximium');
     $select->orderBy('cls.price', 'DESC');
     $select->range(0, 1);

    // Get Maximum of class price
    expDebug::dPrintDBAPI(' $select obj max price = ' , $select);

    // Execute the query
    $clsMaximum = $select->execute()->fetchField();

    return ceil($clsMaximum);
    } catch (Exception $ex) {
    watchdog_exception('getMaxClassPrice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchSliderInCatalog
 */

function selectAddNarrowSearchSliderInCatalog($selectStmt, $tableAlias, $tableField, $filtersetCode,$classmax){

try {
    $UserDefaultCurrencyArr = getUserDefaultCurrency();
    $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];

    $valuefilters= getURLParam($filtersetCode);
    expDebug::dPrint(' $valuefilters = ' . print_r($valuefilters, true) , 4);
    if (!empty($valuefilters)){
      $valueRange = explode("|", $valuefilters);
      $valuefrom = (int) $valueRange[0];
      $valueto = (int) $valueRange[1];
      if($valuefrom == 0){
      $orQuery = db_or();
      $orQuery->isNull($tableAlias . '.' . $tableField);
        if (!empty($valueto)) {
          if($tableAlias == 'cls' && $tableField == 'price' && ($valueto < $classmax)){
          $selectStmt->condition('mas.type', 'Class', '=');
        }
        $orQuery->where('slf_convert_price('.$tableAlias.'.'.$tableField.','.$tableAlias.'.currency_type,\''.$UserPrefCurrCode.'\') <= '.$valueto);
      }
      $selectStmt->condition($orQuery);
      }
      else{
      if (!empty($valuefrom)) {
        /* Changed by Gayathri for #0073578  */
        //$selectStmt->where('slf_convert_price('.$tableAlias.'.'.$tableField.','.$tableAlias.'.currency_type,\''.$UserPrefCurrCode.'\') >= '.$valuefrom);
        $selectStmt->condition($tableAlias . '.' . $tableField, $valuefrom, '>=');
      }
      if (!empty($valueto)) {
        /* Changed by Gayathri for #0073578  */
        //$selectStmt->where('slf_convert_price('.$tableAlias.'.'.$tableField.','.$tableAlias.'.currency_type,\''.$UserPrefCurrCode.'\') <= '.$valueto);
        $selectStmt->condition($tableAlias . '.' . $tableField, $valueto, '<=');
      }
      }
    }
  } catch (Exception $ex) {
    watchdog_exception('selectAddNarrowSearchSliderInCatalog', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByInstructorTextCondition() - To the $selectStmt add Search by Instructor username when set by the user
 */
function selectAddNarrowSearchByInstructorTextCondition($selectStmt, $clsTableAlias, $filtersetCode, $defaultText = '') {
  try {
    $textFilterStr = getURLParam($filtersetCode);
    expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);

    if (!empty($textFilterStr) && $textFilterStr != $defaultText){
     // $selectStmt->leftJoin('slt_object_role_mapping', 'orm', $clsTableAlias . '.id = orm.object_id AND orm.object_type = \'cre_sys_obt_cls\'');
      $selectStmt->leftJoin('slt_person', 'per', 'ses_ins.instructor_id = per.id');
      $selectStmt->condition('per.full_name', '%' . db_like($textFilterStr) . '%', 'LIKE');
    }
  } catch (Exception $ex) {
    watchdog_exception('selectAddNarrowSearchByInstructorTextCondition', $ex);
    expertusErrorThrow($ex);
  }
}

function getStatusCatalogFilter($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  try {
      $select = db_query('SELECT code, name FROM slt_profile_list_items WHERE code like \''.$codePrefix.'%\' AND is_active = \'Y\' AND code NOT IN (\'lrn_crs_sts_del\', \'lrn_cls_sts_del\') ORDER BY FIELD(CODE,\'lrn_crs_sts_atv\',\'lrn_crs_sts_itv\',\'lrn_crs_sts_dft\')');
      $result = $select->fetchAllKeyed();

      // Return results
      return $result;
   } catch (Exception $ex) {
    watchdog_exception('getStatusCatalogFilter', $ex);
    expertusErrorThrow($ex);
  }
}

function publishOrUnpublishCourseClass($catalogId,$catalogType,$actionStatus){
  include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_class.inc');
  global $user;
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    expDebug::dPrint('Ilayaraja : publish/unpublish = ' . $catalogType , 4);
    $status  = getCatalogStatus($catalogId,$catalogType);
    if($catalogType=='Class'){
      $updateStmt = db_update('slt_course_class');
      $updatedstatus  = ($status=='lrn_cls_sts_atv')?"lrn_cls_sts_itv":"lrn_cls_sts_atv";
      if($actionStatus == 'lrn_cls_sts_dld'){
        $updatedstatus = $actionStatus;
      }
      // Update the node logic
      /* Node For Class */
      $nodeSelect = db_select('slt_node_learning_activity', 'entity');
      $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
      $nodeSelect->addField('n','nid','nid');
      $nodeSelect->addField('n','vid','vid');
      $nodeSelect->condition('entity.entity_id', $catalogId);
      $nodeSelect->condition('entity.entity_type', 'cre_sys_obt_cls');
      $nodeResult =  $nodeSelect->execute()->fetchAll();

      $oNode = new StdClass();
      $oNode->nid= $nodeResult[0]->nid;
      $oNode->vid= $nodeResult[0]->vid;
      $drupalUserId = $user->uid;
      $oNode->uid = $drupalUserId;
      $vcNode->language ='en';
      $oNode->type='product';
      $oNode->created=time();
      $oNode->changed=time();
      $oNode->teaser_include=1;
      $oNode->revision= '';
      $oNode->original='';
      $oNode->format=1;
      $oNode->log='';
      $oNode->name=$drupalUserId;
      $oNode->date='';
      $oNode->status= ($updatedstatus == 'lrn_cls_sts_atv') ? 1 : 0;
      $oNode->promote=0;
      $oNode->sticky=0;
      $oNode->op = "Save";
      $oNode->submit = "Save";
      $oNode->preview = "Preview";
      $oNode->form_id = "product_node_form";

      $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();

      if($vid) {
         $classDetails = getCatalogClassDetails($catalogId);
         $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$classDetails['delivery_type']."'")->fetchField();
        // Begin : Taxonomy Insert
        $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
        expDebug::dPrintDBAPI(' $$delivery_type_all_tid  : ', $delivery_type_all_tid);

        if(empty($delivery_type_all_tid)) {
          $terms               = new stdClass;
          $terms->name         = '--All Delivery types--';
          $terms->vid          = $vid;
          taxonomy_term_save($terms);
          $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
        }
        $oNode->taxonomy_catalog['und'] = array();
        $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
        $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$classDetails['delivery_type']."' and vid = ".$vid)->fetchField();
         expDebug::dPrint(' $delivery_type_tid  : '. $delivery_type_tid);
          $dtTerms = array(
           'name' => $delivery_type_name,
           'vid' => $vid,
           'description' => $classDetails['delivery_type'],
           'parent' => $delivery_type_all_tid,
         );
        if(empty($delivery_type_tid)) {
           $oNode->taxonomy_catalog['und'][] = array('tid' => taxonomyVocabTerms($dtTerms));
        }
        else {
           $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
        }
        $delivery_type_tid_cls = db_query("select tid FROM taxonomy_term_data WHERE description ='".$classDetails['delivery_type']."' and vid = ".$vid)->fetchField();
        $class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$catalogId."' and vid = ".$vid)->fetchField();
        $dtTerms1 = array(
           'name' => $classDetails['title'].' : '.$classDetails['code'],
           'vid' => $vid,
           'description' => $catalogId,
           'parent' => $delivery_type_tid_cls
         );
        if(!empty($class_tid)){
          $dtTerms1['tid'] =  $class_tid;
        }
        $tid = taxonomyVocabTerms($dtTerms1);
        expDebug::dPrint(' Taxonomy id: $tid = ' . $tid);

        $oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
        $oNode->teaser='';
        $oNode->validated=1;

        node_save($oNode);
      }

      /* Node For meeting */
      $nodeVCSelect = db_select('slt_node_learning_activity', 'entity');
      $nodeVCSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
      $nodeVCSelect->addField('n','nid','nid');
      $nodeVCSelect->addField('n','vid','vid');
      $nodeVCSelect->condition('entity.entity_id', $catalogId);
      $nodeVCSelect->condition('entity.entity_type', 'meeting');
      $nodeVCResult =  $nodeVCSelect->execute()->fetchAll();
      if(count($nodeVCResult)){
        $vcNode = new StdClass();
        $vcNode->nid= $nodeVCResult[0]->nid;
        $vcNode->vid= $nodeVCResult[0]->vid;
        $drupalUserId = $user->uid;
        $vcNode->uid = $drupalUserId;
        $vcNode->changed = time();
        $vcNode->teaser_include = 1;
        $vcNode->revision = '';
        $vcNode->original='';
        $vcNode->status = ($updatedstatus == 'lrn_cls_sts_atv') ? 1 : 0;
        $vcNode->promote = 0;
        $vcNode->sticky = 0;
        $vcNode->teaser='';
        $vcNode->validated=1;
        node_save($vcNode);
      }
      //node update logic end
      expDebug::dPrint('course id values form the class id details'.$classDetails['course_id']);
      if($status=='lrn_cls_sts_itv' && $classDetails['is_cloned'] == 1){
        cloneEnrollmentNotifications($classDetails['course_id'],$catalogId,$currUserId);
      }
/* Update tag activity Count */
updateTagActivityCount($catalogId,'Class',$updatedstatus);  
/* Update tag activity Count */

      add_audit_trail_entry($currUserId, $catalogId, 'cre_sys_obt_cls', 'exp_sp_administration_catalog', 'publishOrUnpublishCourseClass', 'Updated class Status',
                                             null, $status, $updatedstatus);
      $returnStatus  = ($updatedstatus=='lrn_cls_sts_atv')?t('LBL575'):t('LBL576');
      if($actionStatus == 'lrn_cls_sts_dld'){
        $returnStatus = t('Completed');
      }
    }else if($catalogType=='Course'){
      $updateStmt = db_update('slt_course_template');
      $updatedstatus  = ($status=='lrn_crs_sts_atv')?"lrn_crs_sts_itv":"lrn_crs_sts_atv";

      // Update the node logic
      /* Node For Course */
      $nodeSelect = db_select('slt_node_learning_activity', 'entity');
      $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
      $nodeSelect->addField('n','nid','nid');
      $nodeSelect->addField('n','vid','vid');
      $nodeSelect->condition('entity.entity_id', $catalogId);
      $nodeSelect->condition('entity.entity_type', 'cre_sys_obt_crs');
      $nodeResult =  $nodeSelect->execute()->fetchAll();

      $oNode = new StdClass();
      $oNode->nid= $nodeResult[0]->nid;
      $oNode->vid= $nodeResult[0]->vid;
      $drupalUserId = $user->uid;
      $oNode->uid = $drupalUserId;
      $oNode->changed = time();
      $oNode->teaser_include = 1;
      $oNode->revision = '';
      $oNode->original='';
      $oNode->status = ($updatedstatus == 'lrn_crs_sts_atv') ? 1 : 0;
      $oNode->promote = 0;
      $oNode->sticky = 0;
      $oNode->teaser='';
      $oNode->validated=1;
      node_save($oNode);
      //node update logic end
      
        /* Update tag activity Count */
        updateTagActivityCount($catalogId,'Course',$updatedstatus); 
        /* Update tag activity Count */

      add_audit_trail_entry($currUserId, $catalogId, 'cre_sys_obt_crs', 'exp_sp_administration_catalog', 'publishOrUnpublishCourseClass', 'Updated Course Status',
                                             null, $status, $updatedstatus);
      $returnStatus  = ($updatedstatus=='lrn_crs_sts_atv')?t('LBL571'):t('LBL570');
    }
    //$updateStmt = db_update('slt_course_template');
    $updateStmt->condition('id', $catalogId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'published_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
    print $updatedstatus.'|'.$returnStatus;

  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('publishOrUnpublishCourseClass', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }
  unset($txn);
  syncSolrData($catalogType);
}

function getCatalogStatus($catalogId,$catalogType){
  try {
      if($catalogType=='Class'){
        $select = db_select('slt_course_class', 'cat');
      }else{
        $select = db_select('slt_course_template', 'cat');
      }
      // Get Maximum of class price
      $select->addField('cat','status', 'status');
      $select->condition('cat.id',$catalogId);
      // Execute the query
      $catStatus = $select->execute()->fetchField();
      return $catStatus;
  } catch (Exception $ex) {
    watchdog_exception('getCatalogStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function listCatalogByRestAPI($start, $limit, $userid="")
{
   try {
        $form=array();
        $form["form_id"]="";
        expDebug::dPrint('received form values'. print_r($_REQUEST, 1), 3);
        $catalogcoursestatus = trim($_REQUEST['catalogcoursestatus']);
        $catalogclasstatus = trim($_REQUEST['catalogclassstatus']); // list of class api support.
        $catalogType = trim($_REQUEST['catalogtype']);
        $start = trim($_REQUEST['start']);
        $limit = trim($_REQUEST['limit']);
        $status_check_course =  array('lrn_crs_sts_atv', 'lrn_crs_sts_itv');
        $status_check_class  =  array('lrn_cls_sts_atv', 'lrn_cls_sts_itv');
        if((!empty($catalogcoursestatus)) && (!in_array($catalogcoursestatus,$status_check_course ))) { // $catalogType == 'Course' &&
            $error -> isValidateError = true;
            $error -> errormsg = 'catalogcoursestatus must be in any one of lrn_crs_sts_atv or lrn_crs_sts_itv';
            return $error;
        }
        if((!empty($catalogclasstatus)) && (!in_array($catalogclasstatus,$status_check_class ))) { // $catalogType == 'Class' &&
            $error -> isValidateError = true;
            $error -> errormsg = 'catalogclassstatus must be in any one of lrn_cls_sts_atv or lrn_cls_sts_itv';
            return $error;
        }
        
        // SOLR check starts
        $solrSource = getConfigValue('solr_search');
            if($solrSource == 0){
        drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
        //$cntResults = fetchCatalogAll('COUNT',$start,$limit,'API');
        $results = fetchCatalogAll('',$start,$limit,'API');
        $results1 = $results[0];
        $cntResults = $results[3]; //fetchCatalogAll('COUNT','','','API');
        //$cntResults = count($results);
        if($cntResults){
            $results1['totalrow'] = $cntResults;
        }
            }else{
                require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_admin.php';
                try{
                    $solrObj = new AdminCatalogSolrSearch();
                    $results1 = $solrObj->getCourseList();
                }catch(Exception $e){
                    expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
                }
            }
        
        return $results1;
   } catch (Exception $ex) {
    watchdog_exception('listCatalogByRestAPI', $ex);
    expertusErrorThrow($ex);
  }
}
function getClassStartDate($classId,$ajax=true){
  try {
      $select = db_select('slt_course_class_session', 'ses');
      if($ajax)
        $select->addExpression("DATE_FORMAT(MAX(ses.start_date),'%Y,%m,%d')", 'ses_start_date');
      else
        $select->addExpression("DATE_FORMAT(MAX(ses.start_date),'%m/%d/%Y')", 'ses_start_date');
      $select->condition('ses.class_id',$classId);
      $sesStartDate = $select->execute()->fetchField();
      expDebug::dPrint(' getClassStartDate = ' .print_r($sesStartDate, true) , 3);
      if($ajax)
        print $sesStartDate;
      else
        return $sesStartDate;

  } catch (Exception $ex) {
    watchdog_exception('getClassStartDate', $ex);
    expertusErrorThrow($ex);
  }
}

function isSessionStart($classId){
    try {

        $timeZone = date_default_timezone(false);
        $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;

        $select = db_select('slt_course_class_session','sess');
        $select->leftJoin('slt_profile_list_items', 'prof', 'sess.timezone = prof.code');
        $select->addExpression("IF(CONCAT(DATE_FORMAT(MIN(sess.start_date),'%Y-%m-%d'),' ', TIME_FORMAT(sess.start_time,'%H:%i')) > CONVERT_TZ(NOW(),'".$defaultTimezone."',prof.attr2), FALSE, TRUE)", 'sessionFlag');
        $select->condition('sess.class_id',$classId);
        $sessionFlag = $select->execute()->fetchField();

        expDebug::dPrintDBAPI(' isSessionStart Velu $select = ' , $select);
        expDebug::dPrint(' isSessionStart Velu $sessionFlag = ' .$sessionFlag, 5);

        return $sessionFlag;

    } catch (Exception $ex) {
        watchdog_exception('isSessionStart', $ex);
        expertusErrorThrow($ex);
    }
}

function getClassContentAutoComplete() {
   try {
      $searchCurText =  $_GET['z'];
      $select = db_select('slt_content_master', 'cont_mast');

      // Add fields to fetch
      $select->addField('cont_mast', 'code', 'code');
      $select->condition('cont_mast.status', array('lrn_cnt_sts_atv','lrn_cnt_sts_itv'), 'IN');
      $select->condition('cont_mast.code', '%' . db_like($searchCurText) . '%', 'LIKE');
      filterByUserAccessPrivileges($select, 'cont_mast','',1,'cre_sys_obt_cnt','','cont_mast');
      //selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array("code", "description"), "z");

      // Order in dictionary sort order
      $select->orderBy('code');

      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      $matchingContentCodesList = $select->execute()->fetchAll();
      expDebug::dPrint(' $matchingContentCodesList = ' .print_r($matchingContentCodesList, true) , 3);

        foreach ($matchingContentCodesList as $matchingContentCode) {
         // print $$matchingContentCode->title ." - <b>".$matchingContentCode->code ->code. "</b>\n";
          print $matchingContentCode->code. "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getClassContentAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

//To Get Start and End Session Dates. Ref Ticket: 0023408
function getSessionStartEndDates($classId, $ajax = true){
  try{
  $select = db_select('slt_course_class_session','sess');
  $select->condition('sess.class_id', $classId,'=');
  $select->addExpression("CONCAT(DATE_FORMAT(MIN(sess.start_date),'%Y,%m,%d'), '#' ,DATE_FORMAT(MAX(sess.start_date),'%Y,%m,%d'))", 'sesStartEndDate');
  $sessDates   = $select->execute()->fetchField();
  expDebug::dPrintDBAPI("getSessionStartEndDates Qry = ",$select);
  $sessDates = (!empty($sessDates)) ? $sessDates.'#ilt_or_vc' : $sessDates;
  if($ajax)
    print $sessDates;
  else
    return $sessDates;
  }catch (Exception $ex) {
    watchdog_exception('getSessionStartEndDates', $ex);
    expertusErrorThrow($ex);
  }
}

function exp_sp_administration_viewclass_addedit_form($js = NULL, $entityId){
    try{
        //0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
        expDebug::dPrint('sabbeeee-->'.$entityId);
        includeFiles();
        $catalog_style = array(
                'ctools-viewscreen-wrapper' => array(
                        'modalSize' => array(
                                'width' => 800,
                                'minHeight' => 700,
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
                    array('id' => 'exp_sp_administration_view_classdetails_addedit_form', 'title' => (t('LBL816'))) //view
            );
            expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
            view_form_display($formIdList, $argsList);
        }
        else {
            throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewclass_addedit_form() : nojs');
        }
    }catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_viewclass_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}


function exp_sp_administration_view_classdetails_addedit_form($form, &$form_state,$renderMode, $classId){
    try {
        include_once(drupal_get_path('module', 'exp_sp_coursedetail') .'/exp_sp_coursedetail.inc');
        expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
        expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
        expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
        expDebug::dPrint('$entityId---->'.$classId);

        $wrapperId = 'viewclass-detail-wrapper';

        $form = newAddEditFormWithCancelAndSave($wrapperId);
        $form['view-scroll-start'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="view-scroll-wrapper">'
        );

        $classdetails = getClassDetailsForView($classId);
        expDebug::dPrint('$classdetails-->'.print_r($classdetails,true),5);

        $form['narrow_classview'] = array(
                '#markup'       => theme('narrow_classview', array('record' => $classdetails, 'type' => 'class')),
        );
        $form['view-scroll-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
        );

        return $form;
    }
    catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_view_classdetails_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}

function exp_sp_administration_viewcourse_addedit_form($js = NULL, $entityId){
    try{
        //0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
        expDebug::dPrint('sabbeeee-->'.$entityId);
        includeFiles();
        $catalog_style = array(
                'ctools-viewscreen-wrapper' => array(
                        'modalSize' => array(
                                'width' => 800,
                                'minHeight' => 700,
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
                    array('id' => 'exp_sp_administration_view_coursedetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
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


function exp_sp_administration_view_coursedetails_addedit_form($form, &$form_state,$renderMode, $courseId){
    try {
        include_once(drupal_get_path('module', 'exp_sp_coursedetail') .'/exp_sp_coursedetail.inc');
        expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
        expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
        expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
        expDebug::dPrint('$$courseId---->'.$courseId);

        $wrapperId = 'viewcourse-detail-wrapper';

        $form = newAddEditFormWithCancelAndSave($wrapperId);
        $form['view-scroll-start'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="view-scroll-wrapper">'
        );
        $coursedetails = getCourse_DetailsInfoView($courseId);

        expDebug::dPrint('$$coursedetails-->'.print_r($coursedetails,true),5);
        $form['narrow_courseview'] = array(
                '#markup'       => theme('narrow_courseview', array('results' => $coursedetails, 'type' => 'course')),
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

function getClassViewResults($classId){
    try {
        expDebug::dPrint('enetetet2ugkuwukd');
       include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_class.inc');
         echo theme("narrow_classview", array('record' => getClassDetailsForView($classId)));
    }catch (Exception $ex) {
        watchdog_Exception('getClassViewResults', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * Get Comlience Class Access Count for popup message
*/
function getCountforComlienceAccessClass($clsID) {
try{
        $select = db_select('slt_group_mapping', 'mapping');
        $select->addExpression('COUNT(mapping.id)','accessCount');
        $select->condition('mapping.entity_id', $clsID, '=');
        $select->condition('mapping.entity_type','cre_sys_obt_cls','=');
        $select->condition('mapping.group_type',0,'=');
        //$select->where('mapping.job_role != \'\' OR mapping.user_type != \'\' OR mapping.org_id != \'\'');
        expDebug::dPrintDBAPI("getCountforComlienceAccessClass", $select);
        $count  = $select->execute()->fetchField();
        print $count;
    }catch (Exception $ex) {
        watchdog_exception('getCountforComlienceAccessClass', $ex);
        expertusErrorThrow($ex);
    }
}

?>