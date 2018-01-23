<?php

/**
 * Program Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";
/*--- issue fix : 36122 - add a need files for class regiser ---*/
include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_tp_register.inc');

/*
 * getProgramTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 							  - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */
function programJSObjectInfo() {
  try {
    $jsObjectInfo = array();
    $jsObjectInfo['name'] = 'narrowsearch'; // javascript object name (from exp_sp_administration_program.js)
    $jsObjectInfo['init_id'] = 'root-admin'; //
  } catch (Exception $ex) {
    watchdog_exception('programJSObjectInfo', $ex);
    expertusErrorThrow($ex);
  }
}

function getProgramTabContentType(){
  try {
    $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
    return $jsObjectInfo;
  } catch (Exception $ex) {
    watchdog_exception('getProgramTabContentType', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * programNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function programNarrowSearchFiltersHTML() {
  try {
  		// Added for performance improvement
  		$prgFilters = cache_get('program_filters')->data;
  		if(!empty($prgFilters)){
  			$renderfiltersets = $prgFilters['renderfiltersets'];
  			//Course tags
	  		$weightedCourseTags = getWeightedTags(array('Curricula', 'Certification', 'Learning Plan'));
	      if (!empty($weightedCourseTags)) {
	        $renderfiltersets[$prgFilters['prgTagPos']]=theme('narrow_search_filterset_tags',
	      		       array('code' => 'tagName',
	      								 'title' => t('LBL191'), //Tags
	      								 'tags_for_cloud' => $weightedCourseTags,
	      		)
	         );
	      }

  			$filterSetforJS = $prgFilters['filterSetforJS'];
  			return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
  		}

  		$prgFilters = array();
  		$prgTagPos = 0;
      $checkedStatus = array();
      //$checkedStatus["lrn_lpn_sts_atv"] = TRUE;
      $allStatus = getProfileItemNamesKeyedAdmin('lrn_lpn_sts_');
      expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

      $allLangTypes = fetchProgramLanguagesList();
      expDebug::dPrint(' $allLangTypes = ' . print_r($allLangTypes, true) , 4);

      $overAllDeliveryType = fetchProgramDeliveryTypes();
      expDebug::dPrint(' $overAllDeliveryType = ' . print_r($overAllDeliveryType, true) , 4);
      foreach ($overAllDeliveryType as $key => $value) {
      	$overAllDeliveryType[$key] = t($value);
      }
      expDebug::dPrint('translated $overAllDeliveryType = ' . print_r($overAllDeliveryType, true), 4);

      $managedByOptions = getManagedByFilters();

      $currencyOptions = getCurrencyFilters("TP");

      $renderfiltersets=array();
      if(count($overAllDeliveryType) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'deliverytype',
                                                                     'title' => strtolower(t('LBL036')),
                                                               		 'divid' => 'type',
                                                                     'checkboxes_list' => $overAllDeliveryType)
                                                              );
      	$prgTagPos++;
      }
      if(count($allStatus) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'prgstatus',
                                                                     'title' => t('LBL102'),
                                                               		 'divid' => 'status',
                                                                     'checkboxes_list' => $allStatus,
                                                                     'checked_list' => $checkedStatus)
                                                              );
      	$prgTagPos++;
      }
     if(count($allLangTypes) > 1){
        $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                               array('code' => 'langtype',
                                                                     'title' => t('LBL038'),
                                                               		 'divid' => 'language',
                                                                     'checkboxes_list' => $allLangTypes)
                                                              );
     		$prgTagPos++;
     }
      $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
													      		array('code' => 'tpmanage',
													      				'html_id' => 'tpmanageby_container',
													      				'title' => t('Managed By'),
													      				'divid' => 'managed_by',
													      				'checkboxes_list' => $managedByOptions)
													      );
      $prgTagPos++;

      if(module_exists('exp_sp_commerce') && count($currencyOptions) > 1)
      {
	      $renderfiltersets[] = theme('narrow_search_filterset_checkboxes',
	      		array('code' => 'tpcurrency',
	      				'html_id' => 'catalogtpcurrency_container',
	      				'title' => t('LBL101'),
	      				'divid' => 'currency',
	      				'checkboxes_list' => $currencyOptions)
	      );
	      $prgTagPos++;
      }


      if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) ) {
        //$programmax=getMaxProgramPrice();
      	$programmax = getConfigValue('maxprice');
	        if(($programmax > 0)){
	        $renderfiltersets[]=theme('narrow_search_filterset_slider',
                                    array('code' => 'progPrice',
                                          'title' => t('LBL040'),
                                          'startval' =>'0' ,
                                          'endval' => $programmax,
                                          'prefix' => ''
                                         )
                                    );
	        	$prgTagPos++;
	        }
        }


      $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'courseName',
                                                                     'title' => t('Course'),
                                                                     'default_text'=> t('LBL088')
                                                                     )
                                                              ) ;
      $prgTagPos++;
      $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'surveyName',
                                                                     'title' => t('Survey'),
                                                                     'default_text'=> t('LBL545')
                                                                     )
                                                              ) ;
      $prgTagPos++;
      $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'assesmentName',
                                                                     'title' => t('Assessment'),
                                                                     'default_text'=> t('LBL545')
                                                                     )
                                                              );
      $prgTagPos++;
      $weightedTags = getWeightedTags(array('Curricula', 'Certification', 'Learning Plan'));
      if (!empty($weightedTags)) {
	      $renderfiltersets[]=theme('narrow_search_filterset_tags',
	      		array('code' => 'tagName',
	      				'title' => t('LBL191'), //Tags
	      				'tags_for_cloud' => $weightedTags,
	      		)
	      );
      }

      $narrowfilters =  array();
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"deliverytype");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"prgstatus");
      $narrowfilters[] = array("type"=>"checkbox", "code"=>"tpcurrency");
      $narrowfilters[] = array("type"=> "checkbox", "code"=>"langtype");
      $narrowfilters[] = array("type" => "checkbox", 'code' => "tpmanage");
      $narrowfilters[] = array("type"=> "addltext", "code"=>"courseName", "acpath" =>"administration/learning/program/course-autocomplete/search","defaultText"=>t('LBL088'));
      if (!empty($weightedTags)) {
        $narrowfilters[] = array("type"=> "tagscloud", "code"=>"tagName");
      }
      $narrowfilters[] = array("type"=> "addltext", "code"=>"surveyName", "acpath" =>"administration/learning/program/survey/survey-assessment-autocomplete","defaultText"=>t('LBL545'));
      $narrowfilters[] = array("type"=> "addltext", "code"=>"assesmentName", "acpath" =>"administration/learning/program/assessment/survey-assessment-autocomplete","defaultText"=>t('LBL545'));

      if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {
        	$narrowfilters[] = array("type"=>"slider",  "code"=>"progPrice");
      }

      $filterSetforJS=$narrowfilters;
      $prgFilters = array('renderfiltersets' => $renderfiltersets, 'filterSetforJS' => $filterSetforJS, 'prgTagPos' => $prgTagPos);
      cache_set('program_filters',$prgFilters);
     return theme('narrow_search_filters_holder',
                   array('rendered_filtersets_list' => $renderfiltersets,
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
                         ));
  } catch (Exception $ex) {
    watchdog_exception('programNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * programSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function programSearchResultsSortbarHTML() {
  try {
    $sortbarLinksList = array();
    $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'prg-sort-az');
    $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'prg-sort-za');
    $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'prg-sort-new');
    $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
    					           'last_sort_type_html_id'=>'prg-sort-new'
                                );
    return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                                'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
    																						'title' => t('LBL011'),
                                                )
                );
  } catch (Exception $ex) {
    watchdog_exception('programSearchResultsSortbarHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * programSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function programSearchResultsActionbarHTML() {
  try {
    $actionbarLinksListforJS=array('export_path'=>'administration/learning/program/export/',
    					             'print_path'=>'administration/learning/program/print/'
                                );
    return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL254'),
  												 'add_page_path' => base_path() . 'administration/learning/program/nojs/addedit',
  												 'show_print_button'=>true,
  												 'show_export_button'=>true,
  												 'show_add_button'=>true,
                           'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
                           'ctools_style'=>'ctools-modal-ctools-admin-programform-addedit-style',
    											 'page_code' => 'cre_sys_obt_trp',
                                             )
                );
  } catch (Exception $ex) {
    watchdog_exception('programSearchResultsActionbarHTML', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getProgramTabContentMainHTML - Returns the html for tab content main
 */
function getProgramTabContentMainHTML() {
  try {
      $tabContentTypeArr =  getProgramTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];
      if($tabContentType=="narrow_search"){
        $narrowsearchTabContentListforJS=array(
                                                'search_base_path'=>'administration/learning/program/search/all/',
                                                'show_top_text_filter' => true,
                                                'text_filter_ac_path' => 'administration/learning/program/textfilter-autocomplete'
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
    watchdog_exception('getProgramTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getProgramTabContent - Returns the html for narrow search block on program search page
 */
function getProgramTabContent() {
  try {
      $tabContentTypeArr =  getProgramTabContentType();
      $tabContentType=$tabContentTypeArr['tab_content_type'];
      $programDetailsResponse=array(
      'rendered_tab_content_type' => $tabContentType,
      'rendered_narrow_search_filters' => programNarrowSearchFiltersHTML(),
      'rendered_narrow_search_sortbar' => programSearchResultsSortbarHTML(),
      'rendered_narrow_search_actionbar' => programSearchResultsActionbarHTML(),
      'rendered_tab_content_main'=>getProgramTabContentMainHTML(),
      'rendered_main_div' => getAdminModules('administration','learning'),
    );
     echo drupal_json_encode($programDetailsResponse);
   } catch (Exception $ex) {
    watchdog_exception('getProgramTabContent', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getProgramDetailsHTML() - Renders an program details as HTML to be displayed in jqGrid.
 */
function getProgramDetailsHTML($prg) {
  try {
      $detailsList = array();
      $actionList = array();
      $promoteAction =array();
      if (!empty($prg->object_type)) {
        $detailsList[] = array('detail' => t($prg->object_type), 'tooltip' => t('LBL036').': '.$prg->object_type);
      }
      if (!empty($prg->code)) {
        $detailsList[] = array('detail' => sanitize_data(t($prg->code)), 'tooltip' => t('LBL096'), 'charres' => 8);
      }
      if (!empty($prg->lang)) {
        //$langchar = substr($prg->lang, 0, 3);commented for new theme change
       // $detailsList[] = array('detail' => titleController('',$prg->lang,12), 'tooltip' => t('LBL038').': '.sanitize_data($prg->lang));
      	$detailsList[] = array('detail' => t($prg->lang), 'tooltip' => t('LBL038').': '.sanitize_data($prg->lang));
      }
      /*if (!empty($prg->status)) {
        $detailsList[] = array('detail' => $prg->status, 'tooltip' => t('LBL102'));
      } */
      $get_tp_type  =  getObjectypeFromProgramID($prg->id);
      if($get_tp_type != 'cre_sys_obt_crt'){
      $mandatoryCourseCount = getprogramMandatoryCoursesCount($prg->id);
      $detailsList[] = array('detail' => t('LBL091').':'.' '.$mandatoryCourseCount, 'tooltip' => t('LBL091'));
	  }
	  if($get_tp_type != 'cre_sys_obt_crt'){
      $optionalCourseCount = getprogramCoursesCount($prg->id) - $mandatoryCourseCount;
      $detailsList[] = array('detail' => t('LBL093').':'.' '.$optionalCourseCount, 'tooltip' => t('LBL093'));
      }
      expDebug::dPrint('Program entity type: '.print_r($get_tp_type, true) , 4);
      $actionList[] = array('action_link_text' => t('LBL063'),
    					  'action_page_path' => 'administration/learning/program/nojs/addedit',
    					  'action_button_params' => core_encrypt($prg->id),
      					  'ctools_style' => 'ctools-modal-ctools-admin-programform-addedit-style',
				      	  'row_id' => $prg->row,
				      	  'manageby' => $prg->manageby,
      					  'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                           ),
                          'tooltip' => t('LBL063')
    					  );
        $actionList[] = array('action_link_text' => t('LBL286'),
          'action_page_path' => '',
          'action_button_params' => core_encrypt($prg->id),
          'action_button_program_id' => $prg->id,
          'action_button_param_text' => 'TP',
          'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
          'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                     ),
          'tooltip' => t('LBL286')
      );
        $viewTplist[] = array('action_link_text' => "trainingplan",
        		'action_page_path' => 'administration/view-trainingplan/nojs/addedit/'.core_encrypt($prg->id).'',
        		'action_button_params' =>  $prg->id,
        		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
        		'js_object_info' => array (
        				'name' => 'narrowsearch', // provide the java script object name
        				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
        		),
        		'tooltip' => $catalog->title
        );
        expDebug::dPrint('$actionList--->'.print_r($actionList,true),4);

    /*  $actionList[] = array('action_link_text' => t('LBL275'),
    				  'action_page_path' => '',
    				  'action_button_params' => $prg->id,
    				  'entity_type'=> $get_tp_type,
      				  'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
    					  'js_object_info' => array (
                                                'name' => 'narrowsearch', // provide the java script object name
                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                         ),
                        'tooltip' => t('LBL275')
    				  );
    */
      if(empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {     // This promote/demote will be appear only if Featured Training Enabled

    /*    $actionList[] = array('action_link_text' => (((empty($prg->promote) || $prg->promote == 0)) ? t('LBL236') : t('LBL237')),
        					  'action_page_path' => 'administration/catalog-promote/nojs',
        					  'action_button_params' => $prg->id.'/'.$prg->object_type.'/'.trim($prg->title),
                              'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
          					  'js_object_info' => array (
                                                      'name' => 'narrowsearch', // provide the java script object name
                                                      'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                               ),
                              'tooltip' => (((empty($prg->promote) || $prg->promote == 0)) ? t('LBL236') : t('LBL237'))
        					  );
      */
      }

    /*  $actionList[] = array('action_link_text' => t('LBL238'),
    					  'action_page_path' => 'administration/catalog-access/nojs',
    					  'action_button_params' => $prg->id.'/'.$get_tp_type.'/'.trim($prg->title),
                          'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
      					  'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                           ),
                          'tooltip' => t('LBL238')
    					  );

     $actionList[] = array('action_link_text' => t('LBL200'),
    					  'action_page_path' => '',
    					  'action_button_params' => $prg->id,
                          'ctools_style' => '',
                          'entity_type'=>$get_tp_type,
      					  'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                           ),
                          'tooltip' => t('LBL200'),

    					  );
      $actionList[] = array('action_link_text' => t('LBL239'),
    					  'action_page_path' => 'administration/mr-role-access/nojs',
    					  'action_button_params' => $prg->id.'/'.$get_tp_type.'/'.trim($prg->title),
                          'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
      					  'js_object_info' => array (
                                                  'name' => 'narrowsearch', // provide the java script object name
                                                  'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                           ),
                          'tooltip' => t('LBL239')
    					  );
    */

    /*  $promoteAction = array('action_link_text' => ' ',//(((empty($prg->promote) || $prg->promote == 0)) ? t('LBL236') : t('LBL237')),
      					  'action_link_img' => (((empty($prg->promote) || $prg->promote == 0)) ? 'promote-img-class' : 'demote-img-class'),
      					  'action_page_path' => 'administration/catalog-promote/nojs',
      					  'action_button_params' => $prg->id.'/'.$prg->object_type.'/'.trim($prg->title),
                            'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
        					  'js_object_info' => array (
                                                    'name' => 'narrowsearch', // provide the java script object name
                                                    'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                             ),
                            'tooltip' => (((empty($prg->promote) || $prg->promote == 0)) ? t('LBL236') : t('LBL237'))
      					  );*/

      //$prg->delivery_type_code;
     $tagsentityType    = $prg->object_type;
     $tagsentityType = str_replace(' ','-',$tagsentityType);

     $multiActionValues = new StdClass();
     $multiActionValues->entityId = $prg->id;
     $multiActionValues->entityType = 'cre_sys_obt_trp';
     $multiActionValues->objectType = $prg->delivery_type_code; //Used for Access
     $multiActionValues->entityTypeName = $tagsentityType; //Used for Tags\



      return theme('narrow-search-results-item-details',
                     array('title' => filterSpecialCharacters($prg->title),
                           'details_list'=> $detailsList,
                           'delete_info' => t('MSG357').' '.t($prg->object_type),
                           'description'=> trim(str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""),$prg->short_desc)),
                     	   'action_list'=> $actionList,
                           'promote_action' => $promoteAction,
                           'entity_multi_action' => $multiActionValues,
                          'more_type' => $prg->delivery_type_code,
                     		'sumedit' => $prg->sumedit,
                     		'sumdelete' => $prg->sumdelete,
                     		'view_list' => $viewTplist
                          )
                    );
  } catch (Exception $ex) {
    watchdog_exception('getProgramDetailsHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getProgramActionsHTML() - Renders an program's actions as HTML to be displayed in jqGrid.
 */
function getProgramActionsHTML($prg) {
  try {
    /*return theme('narrow-search-results-item-actions',
                     array('edit_page_path' => base_path().'administration/learning/program/nojs/addedit',
                           'edit_button_action_params' => array($prg->id),
                           'ctools_style'=>'ctools-modal-ctools-admin-programform-addedit-style',
                          )
                );*/
    return theme('narrow-search-results-item-actions',
                     array('catalog_details' => $prg)
                );
  } catch (Exception $ex) {
    watchdog_exception('getProgramActionsHTML', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * fetchProgramLanguagesList() - Fetches all program language list.
 */

function fetchProgramLanguagesList() {
  try {
    // Select slt_program table
    $select = db_select('slt_program', 'prg');
    // Left join the cod and name to get the language name
    $select->leftJoin('slt_profile_list_items', 'pli', 'prg.lang_code = pli.code AND pli.lang_code = \'cre_sys_lng_eng\'');
    // Select fields/expression values to be fetched from the database.
    $select->addField('prg', 'lang_code', 'code');
    $select->addField('pli', 'name', 'name');
    $select->condition(db_and()->condition('prg.status', 'lrn_lpn_sts_del', '!='));
    // Order results by name
    $select->orderBy('name');
    expDebug::dPrintDBAPI(' $select object = ' , $select);
    $prgLangList = $select->execute()->fetchAllKeyed();
    expDebug::dPrint(' $prgLangList = ' . print_r($prgLangList, true) , 3);

    return $prgLangList;
  } catch (Exception $ex) {
    watchdog_exception('fetchProgramLanguagesList', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchProgramDeliveryTypes() - Fetches all delivery type list.
 */

function fetchProgramDeliveryTypes() {
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
    watchdog_exception('fetchProgramDeliveryTypes', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * programStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function programStmtAddOrderByClause($selectStmt, $tableAlias) {
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
    watchdog_exception('programStmtAddOrderByClause', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getProgramMandatoryCoursesCount() - Function to fetch the mandatory course for program
 * SELECT COUNT(id)  FROM slt_module_crs_mapping WHERE program_id='1'  AND is_required = 'Y'
 */

function getProgramMandatoryCoursesCount($cid) {
  try {
      // Select slt_plogram table
      $select = db_select('slt_module_crs_mapping', 'prg');
      // Get count of active program
      $select->addExpression('COUNT(prg.id)', 'count');
      // Set the conditions
      $select->condition('prg.program_id', $cid, '=');
      $select->condition('is_required ', 'Y', '=');
      expDebug::dPrintDBAPI(' $select obj = ' , $select);
      // Execute the query
      $manPrgCount = $select->execute()->fetchField();
      expDebug::dPrint(' $manPrgCount = ' . print_r($manPrgCount, true) , 3);

      return $manPrgCount;
   } catch (Exception $ex) {
    watchdog_exception('getProgramMandatoryCoursesCount', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getProgramCoursesCount() - Function to fetch the toatal courses count for program
 * SELECT COUNT(id)  FROM slt_module_crs_mapping WHERE program_id='1'  AND is_required = 'Y'
 */

function getProgramCoursesCount($pid) {
  try {
  	// Select slt_module_crs_mapping table
    $select = db_select('slt_module_crs_mapping', 'prg');

    // Get count of active program
    $select->addExpression('COUNT(prg.id)', 'count');

    // Set the conditions
    $select->condition('prg.program_id', $pid, '=');

    expDebug::dPrintDBAPI(' $select obj = ' , $select);

    // Execute the query
    $prgCount = $select->execute()->fetchField();
    expDebug::dPrint(' $prgCount = ' . print_r($prgCount.$prg, true) , 3);

    return $prgCount;
  } catch (Exception $ex) {
    watchdog_exception('getProgramCoursesCount', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * sendProgramListToJqGrid() - Prepares JSON response to JqGrid's request for program
 */
function sendProgramListToJqGrid() {
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
      
      //Solr check
      $solrSource = getConfigValue('solr_search');
      if($solrSource == 0){
      	/*$numPrg = fetchProgramAll('COUNT');
      expDebug::dPrint(' $numPrg = ' . print_r($numPrg, true) , 4);
      $total_pages = 0;
      $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numPrg); */
      $start = calculate_offset($page, $limit);
      expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
      $prgList = fetchProgramAll('LIST', $start, $limit);
      expDebug::dPrint(' $prgList = ' . print_r($prgList, true) , 4);
      }else{
      	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_tp_admin.php';
      	try{
      		expDebug::dPrint("Solr search is triggered");
      		$solrObj = new AdminCatalogTPSolrSearch();
      		expDebug::dPrint("Solr search is triggered");
      		$solrList = $solrObj->getTPList();
      		$prgList = $solrList['records'];
      		expDebug::dPrint(' $prgList = ' . print_r($prgList, true) , 4);
      		$numPrg = $solrList['recCount'];
      		$start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numPrg);
      	}catch(Exception $e){
      		expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
      	}
      	expDebug::dPrint("Solr search result - Final ".print_r($prgList,1),4);
      }
      
      // Prepare and send the response
      $response->page = $page;
      $response->total = $total_pages;
      $response->records = $numPrg;
     // $jsObjectInfo = programJSObjectInfo();
      //$response->js_object_info = $jsObjectInfo;
      $response->initial_sort_type_html_id = 'prg-sort-new';
      $i=0;
      $prgList1=$prgList[0];

      // Mananged By behavior related code
      $userData = new stdClass();
      $userData->managedBy = $prgList[1];
      $response->userdata = $userData;

      foreach ($prgList1 as $prg) {
        expDebug::dPrint(' $prg = '. print_r($prg, true) , 4);

        $prg->row =  $i;
        $response->rows[$i]['id']   = $prg->id;
        $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $prg)),
                                             'details' => getProgramDetailsHTML($prg),
                                             'action' => getProgramActionsHTML($prg)
                                             );

        $i++;
      }

      expDebug::dPrint(' $response record = '. print_r($response, true), 4);

      echo drupal_json_encode($response);
  } catch (Exception $ex) {
    watchdog_exception('sendProgramListToJqGrid', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getProgramAutoComplete() - Returns the matched Program's title
 */

function getProgramAutoComplete()
{
  try {
  	  $solrSource = getConfigValue('solr_search');
  	  
  	  if($solrSource == 0){
      	$searchCurText =  $_GET['z'];

      // Select the table
      $select = db_select('slt_program', 'plg');
	  $select->leftJoin('slt_tag_entity','te','((plg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
      $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');

      // Add fields to fetch
      $select->addField('plg','title', 'title');
	  //$select->addField('td','tagname', 'tagname');

      // Add condition
      $select->condition(db_or()
                              ->condition('plg.title', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('plg.code', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('plg.description', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('plg.short_desc', '%' . db_like($searchCurText) . '%', 'LIKE')
                              ->condition('td.tagname', '%' . db_like($searchCurText) . '%', 'LIKE') 
                        );
      $select->condition('plg.status',array('lrn_lpn_sts_itv','lrn_lpn_sts_atv'),'IN');
      filterByUserAccessPrivileges($select,'plg','',1,'','autocomplete','plg');     
       
      //groupby id for duplicate entries. #71622
      $select->groupBy('plg.id');
      
      // Order in dictionary sort order
      $select->orderBy('title');

      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      $matchingPrgNames = $select->execute()->fetchAll();
      expDebug::dPrint(' $$matchingPrgNames = '. print_r($matchingPrgNames, true) , 3);
      }
      
      else {
      expDebug::dPrint ( 'entered solr search' );
			require_once $_SERVER ['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_tp_admin.php';
			try {
      					$solrObj = new AdminCatalogTPSolrSearch ();
      					$solrList = $solrObj->getCatalogTPAutoComplete ();
      					expDebug::dPrint ( ' $prgList123 = ' . print_r ( $solrList, true ), 4 );
      					$matchingPrgNames = $solrList ['records'] [0];
      							expDebug::dPrint ( ' $prgList123 = ' . print_r ( $matchingPrgNames, true ), 4 );
      							$numCatalog = $solrList ['recCount'];
      									expDebug::dPrint ( ' Total matiching records ' . print_r ( $numCatalog, true ), 4 );
      					} catch ( Exception $e ) {
      						expDebug::dPrint ( "Error in Solr Search for Users - " . print_r ( $e->getMessage (), true ), 1 );
      					}
      }

      foreach ( $matchingPrgNames as $prgName) {
        print rawurlencode($prgName->title) . "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getProgramAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchProgramAll() - Fetches program count / program list for jqgrid after applying the narrow search filters
 */
function fetchProgramAll($op, $start=NULL, $limit=NULL, $callFrom='', $tpTitleLike = '') {
  try {
  		$userId = getSltpersonUserId();
      // Select slt_program table
      $select = db_select('slt_program', 'prg');
    /*  if($tpTitleLike) {
      	$select->condition('prg.title','%'.$tpTitleLike.'%','LIKE');
      }*/
      /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=prg.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 1');
      $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
      $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_sys_obt_trp\''); */

      // Join required tables
      $select->leftJoin('slt_profile_list_items', 'plitype', 'prg.object_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plistatus', 'prg.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plilang', 'prg.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plicurr', 'prg.currency_type = plicurr.attr1 AND plicurr.lang_code = \'cre_sys_lng_eng\'');
	  $select->leftJoin('slt_tag_entity','te','((prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
      $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');

    //  $select->leftJoin('slt_promoted_highly_rated', 'promo', 'promo.activity_id= prg.id and promo.entity_type = plitype.name');


      if ($op == "COUNT") {
        // Get count of active program
        $select->addExpression('COUNT(DISTINCT(prg.id))', 'count');
      }
      else { // LIST
        // Select fields/expression values to be fetched from the database.
      //	$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
      	//$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
        $select->addField('prg', 'id', 'id');
        $select->addField('prg', 'price', 'price');
        $select->addField('prg', 'title', 'title');
        $select->addField('prg', 'code', 'code');
        $select->addField('prg', 'description', 'description');
        $select->addField('prg', 'short_desc', 'short_desc');
		$select->addField('prg', 'additional_info', 'additional_info');
        $select->addField('prg', 'object_type', 'delivery_type_code');
        $select->addField('plitype', 'name', 'object_type');
        $select->addField('plistatus', 'name', 'status');
        $select->addField('plilang', 'name', 'lang');
	//	$select->addField('td','tagname', 'tagname');
     //   $select->addField('promo', 'status', 'promote');
        $select->addField('prg', 'status', 'status_code');
        if ($callFrom == 'API')
        {
		    $select->addField('prg', 'addn_catalog_show', 'shown_in_catalog');
			$select->addField('prg', 'addn_notification_show', 'shown_in_notification');
            $select->addField('prg', 'currency_type', 'currency');
        	$select->addField('plicurr', 'code', 'currency_code');
        	$select->addField('plicurr', 'attr2', 'currency_symbol');
        	
        	$ret_tmp = getcustomattributemodulestatus();
        	if($ret_tmp == true){
        		include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
        		$attrValues = getCustomAttributeValues('Training Plan','mandatory');
        		foreach($attrValues as $ckey => $cvalue){
        			//$select->addField('per', $cvalue->attributerefercolumn, strtolower(str_replace(' ','_',$cvalue->attributename)));
        			$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0`
        					WHERE FIND_IN_SET(`opt0`.`opt_code`,`prg`.`$cvalue->attributerefercolumn`)),`prg`.`$cvalue->attributerefercolumn`),`prg`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));
        			 
        		}
        	} //#custom_attribute_0078975 - End Check module status
        }
        $select->groupBy('prg.id');
      }

      // Delete entries should not display
      $select->condition('prg.status', array('lrn_lpn_sts_del'), 'NOT IN');
      filterByUserAccessPrivileges($select, '','',1,'','tp','prg');

      // Set the conditions

      $defaultPrgStatus = isset($_GET['prgstatus']) ? '' : '';
      if(isset($_GET['deliverytype']) && !empty($_GET['deliverytype'])){
      	selectAddNarrowSearchINCondition($select, 'prg', 'object_type', 'deliverytype');
      }
      if(isset($_GET['prgstatus']) && !empty($_GET['prgstatus'])){
      	selectAddNarrowSearchINCondition($select, 'prg', 'status', 'prgstatus',$defaultPrgStatus);
      }
      if(isset($_GET['tpcurrency']) && !empty($_GET['tpcurrency'])){
      	selectAddNarrowSearchINCondition($select, 'prg', 'currency_type', 'tpcurrency');
      }
      if(isset($_GET['langtype']) && !empty($_GET['langtype'])){
      	selectAddNarrowSearchINCondition($select, 'prg', 'lang_code', 'langtype');
      }
      if($callFrom == 'API') { //Added by Rajeshwar
        $select->addExpression("(select GROUP_CONCAT(def.tagname) FROM slt_tag_entity ent LEFT OUTER JOIN slt_tagdefn def on def.id = ent.tagid where (prg.id= ent.entity_id AND (ent.entity_type='Certification' OR ent.entity_type='Curricula' OR ent.entity_type='Learning Plan') ))","tagname");
        $tag=trim($_REQUEST['tagname']);
      if(isset($tag) && !empty($tag))
        $select->condition('td.tagname', $tag,'=');
      }
      if(isset($_REQUEST['textfilter']) && !empty($_REQUEST['textfilter'])){
      	    $searchText = $_REQUEST['textfilter'];
      //	selectAddNarrowSearchByTextMultipleORCondition($select, 'prg', array('title', 'code', 'description', 'short_desc')); // search by title in the header
      		$tableAliasFields = array('prg.title','prg.code','prg.description','prg.short_desc','td.tagname');
        	SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);
      }
      if(isset($_GET['courseName']) && !empty($_GET['courseName'])){
      	$select->leftJoin('slt_module_crs_mapping', 'maps', 'maps.program_id = prg.id');
        $select->leftJoin('slt_course_template', 'tmplt', 'tmplt.id = maps.course_id');
      	selectAddNarrowSearchByTextCondition($select, 'tmplt', 'title', 'courseName', t('LBL088')); // search by attached courseName
      }

      if(isset($_GET['surveyName']) && !empty($_GET['surveyName'])){
      	$select->leftJoin('slt_survey_mapping', 'survmaps', '((survmaps.object_id = prg.id AND (survmaps.object_type=\'cre_sys_obt_crt\' OR survmaps.object_type=\'cre_sys_obt_cur\' OR survmaps.object_type=\'cre_sys_obt_trn\')))');
        $select->leftJoin('slt_survey', 'surv', 'surv.id = survmaps.survey_id');
      	selectAddNarrowSearchByTextMultipleORCondition($select, 'surv', array('title', 'code'),'surveyName',t('LBL545')); // search by attached surveyName

      }

      if(isset($_GET['assesmentName']) && !empty($_GET['assesmentName'])){
      	$select->leftJoin('slt_survey_mapping', 'assessmaps', '((assessmaps.object_id = prg.id AND (assessmaps.object_type=\'cre_sys_obt_crt\' OR assessmaps.object_type=\'cre_sys_obt_cur\' OR assessmaps.object_type=\'cre_sys_obt_trn\')))');
        $select->leftJoin('slt_survey', 'assess', 'assess.id = assessmaps.survey_id');
      	selectAddNarrowSearchByTextMultipleORCondition($select, 'assess', array('title', 'code'),'assesmentName',t('LBL545')); // search by attached assesmentName
      }

      if(isset($_GET['tagName']) && !empty($_GET['tagName'])){
      	//$select->leftjoin('slt_tag_entity', 'te','((prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\')))');
     		//$select->leftjoin('slt_tagdefn', 'td','te.tagid= td.id');
      	selectAddNarrowSearchByTextConditionExactMatch($select, 'td', 'tagname', 'tagName', t('LBL193')); // search by attached tagName
      }
      if(isset($_GET['progPrice']) && !empty($_GET['progPrice'])){
      	selectAddNarrowSearchSliderCondition($select, 'prg', 'price', 'progPrice');
      }
      // Check added by Vincent on Dec 2, 2013
      // Manage by fillter is not require for API calls
      $managefilter = '';
      if ($callFrom != 'API') {
	      $managefilter = selectAddNarrowSearchManageByCondition('tp', $select, 'prg', 'tpmanage');
      }
      //Adding Custom Attrbiutes Condition #custom_attribute_0078975
      //if($callFrom && module_exists('exp_sp_administration_customattribute')) {      
      if($callFrom)
      {
      	$ret_tmp = getcustomattributemodulestatus();      	   	
      	if($ret_tmp == true){
      	expDebug::dPrint(' $_POST' . print_r($_POST, true) , 5);

      	foreach($_POST as $key=>$value) {
      		expDebug::dPrint(' $value ' . print_r($value, true) , 5);
      
      		if(strpos($key,'c_')===0 && $value !=''){
      			expDebug::dPrint(' $key ' . print_r($key, true) , 5);
      
      			//$entityMappingColumn = getCustomAttributeMappingColumn($key);
      			$entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_sys_obt_trp');
      			$per_column = 'prg.'.$entityMappingColumn['crefercolumn'];
      			expDebug::dPrint(' $per_column' . print_r($per_column, true) , 5);
      
      			/*$select->condition($per_column, '%' . db_like($value) . '%', 'LIKE');*/
      		
      		$custom_attr_option_code=$per_column." LIKE '%".$value."%'";
      		
      		
      			include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
      			$cus_attr_opt_code_arr=explode(",",$value);
      			expDebug::dPrint(' $$cus_attr_opt_code_arr ' . print_r($cus_attr_opt_code_arr, true) , 5);
      			expDebug::dPrint(' $$cus_attr_opt_code_arr sckjasxdcn' . print_r(count($cus_attr_opt_code_arr), true) , 5);
      			
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
      
      //expDebug::dPrintDBAPI(' $select obj = ' , $select);
      if ($op != "COUNT" && $callFrom != 'API') {
        // Order the records
        programStmtAddOrderByClause($select, 'prg', "AZ");
        // Group by prg.id to eliminate duplicate program records
        $select->groupBy('prg.id');
      }
      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      if($op=="COUNT"){
      	// Execute the query
    	  $newResult = $select->execute()->fetchField();
    	  expDebug::dPrint('COUNT $newResult = ' . print_r($newResult, true) , 3);
    	  return $newResult;
      }
      elseif($op == "CSV"){
      $colHeadersMap = array(t('LBL083') => 'title',
                             t('LBL036') => 'object_type',
                             t('LBL096') => 'code',
      						           //t('LBL229') => 'description',
                             t('LBL038') => 'lang',
                             t('LBL102') => 'status');
        // Applied the CSV filter option
        $filterArray[] = array(t('Training Plan').':'.textboxFiltersetValueToPrintableStr('textfilter'));
        $filterArray[] = array(t('LBL036'). ':'.checkboxFiltersetSelectionsToPrintableStr('deliverytype'));
      	$filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('prgstatus'));
      	$filterArray[] = array(t('LBL101').':'.checkboxFiltersetSelectionsToPrintableStr('tpcurrency'));
        if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {
          $filterArray[] = array(t('LBL040').':'.sliderFiltersetValueToPrintableStr('progPrice'));
        }
        $filterArray[] = array(t('Course').':'.textboxFiltersetValueToPrintableStr('courseName'));
        $filterArray[] = array(t('Survey').':'.textboxFiltersetValueToPrintableStr('surveyName'));
        $filterArray[] = array(t('Assessment').':'.textboxFiltersetValueToPrintableStr('assesmentName'));
        $filterArray[] = array(t('LBL191').':'.textboxFiltersetValueToPrintableStr('tagName'));
        $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
        $filterArray[] = array(t('Managed By').':'.checkboxFiltersetSelectionsToPrintableStr('tpmanage'));

        SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap,'export', $filterArray, '', 'tp_admin_search');
      }
      elseif($op == "PDF"){
        // % width of the columns must total to 100%
      	$colHeadersMap = array(t('LBL083') => array('title',          38),
                            	 t('LBL036') => array('object_type',    23),
                            	 t('LBL096') => array('code',           10),
                            	 t('LBL038') => array('lang',           14),
                               t('LBL102') => array('status',         15),
      	                    );

      	$detailsList[] = array('detail' => $prg->object_type, 'tooltip' => $prg->object_type); // @TODO: why is this line here?

        $appliedFilters = array();
        $appliedFilters[t('Training Plan')] = textboxFiltersetValueToPrintableStr('textfilter');
        $appliedFilters[t('LBL036')] = checkboxFiltersetSelectionsToPrintableStr('deliverytype');
      	$appliedFilters[t('LBL102')] = checkboxFiltersetSelectionsToPrintableStr('prgstatus');
      	$appliedFilters[t('LBL101')] = checkboxFiltersetSelectionsToPrintableStr('tpcurrency');
        if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {
          $appliedFilters[t('LBL040')] = sliderFiltersetValueToPrintableStr('progPrice');
        }
        $appliedFilters[t('Course')] = textboxFiltersetValueToPrintableStr('courseName');
        $appliedFilters[t('Survey')] = textboxFiltersetValueToPrintableStr('surveyName');
        $appliedFilters[t('Assessment')] = textboxFiltersetValueToPrintableStr('assesmentName');
        $appliedFilters[t('LBL191')] = textboxFiltersetValueToPrintableStr('tagName');
        $appliedFilters[ucfirst(t('LBL011'))] = getNarrowSearchSortTitle();
        $appliedFilters[t('Managed By')] = checkboxFiltersetSelectionsToPrintableStr('tpmanage');

        SendNarrowSearchResultsAsPDFFile($select, t('Training Plan'), $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'tp_admin_search');
      }
      else{ // LIST
    	  // Execute the query and fetch all the records
    	  // Limit to $limit records
      	  if($callFrom == 'API') {
			$totalRecords = $select->countQuery()->execute()->fetchField();
		  }
		  add_row_id_condition($select, 'prg');
      	  if (is_numeric($limit) && is_numeric($start)) {
    	  	$select->range($start, $limit);
    	  }
		expDebug::dPrintDBAPI('select programs ', $select);
    	  $result = $select->execute()->fetchAll();
    	  $newResult = array();
    	  $newResult[0] = $result;
	      $newResult[1] = $managefilter;
	      if($callFrom == 'API') {
			$newResult[2] = $totalRecords;
		  }
	      
	      expDebug::dPrint('LIST $newResult = ' . print_r($newResult, true) , 3);
	      return $newResult;
      }
  } catch (Exception $ex) {
    watchdog_exception('fetchProgramAll', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_column_headers_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function tp_admin_search_csv_column_headers_alter($columnHeaderMap) {
  try {
    $columnHeaderMap[0][t('LBL091')] = 'mandcourse';
    $columnHeaderMap[0][t('LBL093')] = 'optionalcourse';
  }
  catch (Exception $ex) {
    watchdog_exception('tp_admin_search_csv_column_headers_alter', $ex);
    expertusErrorThrow($ex);
  }

}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function tp_admin_search_csv_row_data_alter($rowData) {
  try {
    //appending the Child program count and active learners count to the result.
    $rowData[0]->mandcourse = getProgramMandatoryCoursesCount($rowData[0]->id);
    $rowData[0]->optionalcourse = getProgramCoursesCount($rowData[0]->id) - $rowData[0]->mandcourse;
  }
  catch (Exception $ex) {
    watchdog_exception('tp_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_column_headers_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function tp_admin_search_pdf_column_headers_alter($columnHeaderMap) {
  try {
        // % width of the columns must total to 100%
        $columnHeaderMap[0][t('LBL083')] = array('title', 18);
        $columnHeaderMap[0][t('LBL036')] = array('object_type', 15);
        $columnHeaderMap[0][t('LBL091')] = array('mandcourse', 14);
        $columnHeaderMap[0][t('LBL093')] = array('optionalcourse', 14);
  }
  catch (Exception $ex) {
    watchdog_exception('tp_admin_search_pdf_column_headers_alter', $ex);
    expertusErrorThrow($ex);
  }

}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function tp_admin_search_pdf_row_data_alter($rowData) {
  try {
    //appending the Child program count and active learners count to the result.
    $rowData[0]->mandcourse = getProgramMandatoryCoursesCount($rowData[0]->id);
    $rowData[0]->optionalcourse = getProgramCoursesCount($rowData[0]->id) - $rowData[0]->mandcourse;
  }
  catch (Exception $ex) {
    watchdog_exception('tp_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getMaxProgramPrice() - Get the Maximim Program price
 */
function getMaxProgramPrice(){
  try {
     $select = db_select('slt_program', 'prg');

    // Get Maximum of program price
    $select->addExpression('MAX(price)', 'maximium');


    expDebug::dPrintDBAPI(' $select obj = ' , $select);

    // Execute the query
    $prgMaximum = $select->execute()->fetchField();
    expDebug::dPrint(' $prgMaximum = ' . print_r($prgCount.$prg, true) , 4);

    return $prgMaximum;
  } catch (Exception $ex) {
    watchdog_exception('getMaxProgramPrice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getDefaultPrgAddEditFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultPrgAddEditFormFieldValue($field, $form_state, $prgDetails) {
  try {
      $defaultValue = '';
      switch ($field) {
      	case 'short_desc':
          $defaultValue = (empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='')?
                                        ((empty($prgDetails[$field]) || $prgDetails[$field]=='')? '' : $prgDetails[$field]) :
                                        $form_state['values'][$field]['value'];
      case 'additional_info':
          $defaultValue = (empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='')?
                                        ((empty($prgDetails[$field]) || $prgDetails[$field]=='')? '' : $prgDetails[$field]) :
                                        $form_state['values'][$field]['value'];
      	  break;
        default:   // For all other fields
          $defaultValue = (empty($form_state['values'][$field]) && $form_state['values'][$field]=='')?
                                        ((empty($prgDetails[$field]) && $prgDetails[$field]=='')? '' : $prgDetails[$field]) :
                                        $form_state['values'][$field];
          break;
      } // end switch

      if ($field == 'is_required'){
      	$defaultValue = (empty($defaultValue) || $defaultValue == 'N')? 0 : 1;
      }
      expDebug::dPrint(' $defaultValue for field ' .$field . ' = ' . print_r($defaultValue, true) , 4);

      return $defaultValue;
  } catch (Exception $ex) {
    watchdog_exception('getDefaultPrgAddEditFormFieldValue', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * uniquePrgCode() - Get the unique code count for the program.
 */
function uniquePrgCode($form_state) {
  try {
      // Select slt_program table
      $select = db_select('slt_program', 'prg');
      $select->addExpression('COUNT(prg.id)', 'count');
      // Set the conditions
      $select->condition('prg.code', $form_state['values']['code'], '=');
      $select->condition('prg.lang_code', $form_state['values']['lang_code'], '=');
      if (!empty($form_state['values']['id'])) { // An update operation, do not compare with self's title
        $select->condition('prg.id', $form_state['values']['id'], '!=');
      }

      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      // Execute the query
      $numPrg = $select->execute()->fetchField();
      expDebug::dPrint(' $numPrg = ' . print_r($numPrg, true) , 3);

      if ($numPrg > 0) {
        return false; // Not unique
      }
      return true; // a valid new name
  } catch (Exception $ex) {
    watchdog_exception('uniquePrgCode', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * checkLearningProgramType() Checking the Program Type
 */

function checkLearningProgramType($form, $form_state){
	try {
  expDebug::dPrint(' $form_state[values][object_type] = ' . $form_state['values']['object_type'] , 5);
  return($form_state['values']['object_type']);
  } catch (Exception $ex) {
  	watchdog_exception('checkLearningProgramType', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addNewProgram() - Save a new program details in the database
 */
function addNewProgram($form, &$form_state, $userid,$callFromAPI =0) {
  try {
     expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
     expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);

     $txn = db_transaction();

      try {
        // Select the table to insert the record in
        $insertStmt = db_insert('slt_program');

        $tp_object_type = checkLearningProgramType($form, $form_state);

        if ($tp_object_type != 'cre_sys_obt_crt') {  // All other checks are done in form validation function.
          $form_state['values']['expires_in_value'] = NULL;
          $form_state['values']['expires_in_unit'] = NULL;
         }
        if($callFromAPI)
			{
			$additional_info   = empty($form_state['values']['additional_info']) ? null : trimInput($form_state['values']['additional_info']);
			}
		else{
			$additional_info   = empty($form_state['values']['additional_info']['value']) ? null : trimInput($form_state['values']['additional_info']['value']);
            }
			$is_shown  = empty($form_state['values']['addn_catalog_show']) ? 0 : $form_state['values']['addn_catalog_show'];
	        $is_noti  = empty($form_state['values']['addn_notification_show']) ? 0 : $form_state['values']['addn_notification_show'];
        $end_date = null;
        if ($tp_object_type == 'cre_sys_obt_trn') { // All other checks are done in form validation function.
          $old_dateformat = str_replace("-", "/", $form_state['values']['end_date']);
          $end_date = date_format(date_create($old_dateformat), 'Y-m-d H:i');
        }
        //$price=null;
        //$currencytype=null;

       /*if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)) {

          if($form_state['values']['price']==""){$price=null;}else{$price= $form_state['values']['price'];};
          if($form_state['values']['currency_type']==0 ){$currencytype=null;}else{$currencytype=$form_state['values']['currency_type'];}
        }
        */
        $price = $form_state['values']['price'];
        $currency_type = $form_state['values']['currency_type'];
       /*  if($callFromAPI == 1)
        {
        	if($price > 0 && is_numeric($price))
        	{
        		if(empty($currency_type))
        		{
        			return array((object)array('id'=>'currecny_null','object_type'=>''));
        		}
        		else {
        			$selectQuery = db_select('slt_profile_list_items', 'spli');
        			$selectQuery->addField('spli','attr1');
        			$selectQuery->condition('spli.is_active', 'Y', '=');
        			$selectQuery->condition('spli.attr3', 'Y', '=');
        			$selectQuery->condition('spli.code', $currency_type, '=');
        			expDebug::dPrintDBAPI('$selectQuery', $selectQuery, 1);
        			$currency_type = $selectQuery->execute()->fetchField();
        			if(empty($currency_type))
        			{
        				return array((object)array('id'=>'invalid_currecny','object_type'=>""));
        			}
        		}
        	}
        	else {
        		$selectQuery = db_select('slt_profile_list_items', 'spli');
        		$selectQuery->addField('spli','attr1');
        		$selectQuery->condition('spli.is_active', 'Y', '=');
        		$selectQuery->condition('spli.attr3', 'Y', '=');
        		$selectQuery->condition('spli.code', $currency_type, '=');

        		$currency_type = $selectQuery->execute()->fetchField();
        		if(empty($currency_type))
        		{
        			$currency_type = "USD";
        		}
        	}
        } */
       if($callFromAPI)
        {
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
        	$ret_tmp = getcustomattributemodulestatus();
        	if($ret_tmp == true){
        	include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
        	$custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_trp');
        	expDebug::dPrint('create training plan API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
        	if($custom_attr_error != '') {
        		$customErrmsg=new stdClass();
        		$customErrmsg->isValidateError = 1;
        		$customErrmsg->errormsg = $custom_attr_error;
        		$customErrmsg->id = $custom_attr_error;
        		return array($customErrmsg);
        	}
        } //#custom_attribute_0078975 - End Check module status
        }

        $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
        $in_active_on          = $form_state['values']['status'] == 'lrn_lpn_sts_itv' ? now() : NULL;
        $in_active_reason      = $form_state['values']['status'] == 'lrn_lpn_sts_itv' ? $inactivereasontext : NULL;

        $trigger_element = $form_state['storage']['submit_triggering_element'];
        if($trigger_element == 'saveandpublish'){
         $tp_status = 'lrn_lpn_sts_atv';
        }
        else if($trigger_element == 'save'){
         $tp_status = 'lrn_lpn_sts_itv';
        }
        else if($form_state['values']['status']){
        $tp_status = $form_state['values']['status'];
        }

        $fields = array(
                    'title'                 => $form_state['values']['title'],
                    'type'                  => 'TP',
                    'object_type'           => $tp_object_type,
                    'code'					=> $form_state['values']['code'],
                    'lang_code'             => $form_state['values']['lang_code'],
                    'short_desc'            => $form_state['values']['short_desc']['value'],
                    'price'                 => ($price!=null && $price!='')?$price:0,//$price,
                    'currency_type'         => $currency_type,//'cre_sys_crn_usd',
                    'status'                => $tp_status,//'lrn_lpn_sts_itv',//$form_state['values']['status'],
                    'expires_in_value'      => ($form_state['values']['expires_in_value'] != null && $form_state['values']['expires_in_value'] != '')?$form_state['values']['expires_in_value']:null,
                    'expires_in_unit'       => $form_state['values']['expires_in_unit'],
        			'enforce_sequence'      => $form_state['values']['enforce_sequence'],
                    'created_by'            => $userid,
                    'created_on'            => now(),
        			'updated_by'            => $userid,
        			'updated_on'            => now(),
                    'end_date'              => $end_date,
                    'in_active_on'          => $in_active_on,
                	'in_active_reason'      => $in_active_reason,
                    'author_vendor'			=> $form_state['values']['author_vendor'],
                    'additional_info'       => $additional_info,
                    'addn_catalog_show' =>  $is_shown,
                    'addn_notification_show'  => $is_noti,
 				  );

        // Select the fields and the values to be inserted
      $fields = trimInputFields($fields); // #42124: - Trim form values
      $insertStmt->fields($fields);
      $form_state['values']['object_type'] = $tp_object_type;
       $form_state['values']['short_desc'] = $form_state['values']['short_desc']['value'];
	  $form_state['values']['additional_info'] = $additional_info;
      expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);
        // Execute the insert statement. Fetch the new prg id.
      $prgId = $insertStmt->execute();
      if($callFromAPI) {
      	$tpCreateUser = $userid;
      }
      else {
      	$tpCreateUser = getIdOfLoggedInUser();
      }
      addnewModuleForTP($prgId,$userid,$tp_object_type);
      updateGroupMappingDetails($prgId,$tp_object_type, $tpCreateUser,'emptyadd',0);
      make_audit_trail_entries($form_state['values'], $tpCreateUser , null ,$prgId, 'Inserted',$tp_object_type,'exp_sp_administration_program','Add new Training Plan',
                                             array('title', 'code', 'short_desc','additional_info','addn_catalog_show','addn_notification_show','object_type', 'price','lang_code', 'status','currency_type',
                                                   'expires_in_value', 'expires_in_unit','end_date','author_vendor'));
      expDebug::dPrint(' $prgId = ' . print_r($prgId, true) , 3);
      
      //if($callFromAPI && module_exists('exp_sp_administration_customattribute')){     //#custom_attribute_0078975 - Check module status/Save Custom Attributes
      if($callFromAPI)
      {
      	$ret_tmp = getcustomattributemodulestatus();
      	if($ret_tmp == true){
      	include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
      	saveCustomAttributesInAddEditForm($form, $form_state, $prgId, 'cre_sys_obt_trp', $userid,'Inserted',1);
      }
      }
      // Save the new program's id in the hidden id form field.
      $form_state['values']['id'] = $prgId;

      return array((object)array('id'=>$prgId,'object_type'=>$tp_object_type));
      }
  	catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('prgadmin', $ex);
        if($callFromAPI){
        	$errobj=new stdClass();
        	$errobj->isValidateError = 1;
        	$errobj->errcode = 'L_012';
        	$errobj->errormsg = "Error in creating new program.";
        	return $errobj;
		}
        throw $ex;

      }
  } catch (Exception $ex) {
    watchdog_exception('addNewProgram', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * updatePorgramDetails - Updates an existing program details when the user clicks save
 */
function updateProgramDetails($form, &$form_state, $userid,$callFromAPI =0) {
	try {
		expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
		expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
		$txn = db_transaction();
		try {

			$selectStmt = db_select('slt_program', 'prgm');
			$selectStmt->condition('id', $form_state['values']['id']);
			$selectStmt->addField('prgm', 'title');
			$selectStmt->addField('prgm', 'code');
			$selectStmt->addField('prgm', 'short_desc');
			$selectStmt->addField('prgm', 'lang_code');
			$selectStmt->addField('prgm', 'status');
			$selectStmt->addField('prgm','additional_info');
			$selectStmt->addField('prgm', 'price');
			$selectStmt->addField('prgm', 'currency_type');
			$selectStmt->addField('prgm', 'object_type');
			$selectStmt->addField('prgm', 'expires_in_value');
			$selectStmt->addField('prgm', 'expires_in_unit');
			$selectStmt->addField('prgm', 'end_date');
			$selectStmt->addField('prgm', 'author_vendor');
			$selectStmt->addField('prgm', 'addn_catalog_show');
			$selectStmt->addField('prgm', 'addn_notification_show');

			expDebug::dPrintDBAPI('$selectStmt', $selectStmt);

			$default = $selectStmt->execute()->fetchALL();
			expDebug::dPrint('$default = ' . print_r($default, true));
			$price=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
			$currency_type = $form_state['values']['currency_type'];
			if($callFromAPI)
				{
				$additional_info   = empty($form_state['values']['additional_info']) ? null : trimInput($form_state['values']['additional_info']);
				}
			else{
				$additional_info   = empty($form_state['values']['additional_info']['value']) ? null : trimInput($form_state['values']['additional_info']['value']);
                }
			if($callFromAPI){
               $is_shown  = ($form_state['values']['addn_catalog_show'] == '') ? $default['0']->addn_catalog_show : $form_state['values']['addn_catalog_show'];
	           $is_noti  = ($form_state['values']['addn_notification_show'] == '') ? $default['0']->addn_notification_show : $form_state['values']['addn_notification_show'];	            
			}
			else{
				$is_shown  = empty($form_state['values']['addn_catalog_show']) ? 0 : $form_state['values']['addn_catalog_show'];
				$is_noti  = empty($form_state['values']['addn_notification_show']) ? 0 : $form_state['values']['addn_notification_show'];
				}
			
			if(!empty($callFromAPI)){ // Only For API
				if(!empty($default)){
					$default['0']->object_type = strtolower(trim($default['0']->object_type)); // Trim and Convert the type into lower
				}
				if(empty($default)){ // Id Exist Validate
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Id does not exist.";
					return $errobj;
				}
				if($default['0']->object_type != $form_state['values']['object_type']){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Object Type is mismatching.";
					return $errobj;
				}
				if($default['0']->object_type == 'cre_sys_obt_crt'){
					$cerExpInTypes = array('years','months','days');
					if(!in_array(strtolower(trim($form_state['values']['expires_in_unit'])) , $cerExpInTypes)){ // Only this types are Allowed
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Certification Type incorrect.";
						return $errobj;
					}
				}
				if($default['0']->object_type == 'cre_sys_obt_trn'){
					if($form_state['values']['end_date']=='' || $form_state['values']['end_date'] == t('LBL112')){
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Complete By is required.";
						return $errobj;
					}
					$old_dateformat=str_replace("-","/",$form_state['values']['end_date']);
					$end_date= date_format(date_create( $old_dateformat),'Y-m-d H:i');
					if(strtotime($end_date) <= time()){
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->errcode = 'L_012';
						$errobj->errormsg = "Complete By Should be a future date.";
						return $errobj;
					}
				}
				$totalEnrollment = getTotalTPEnroll($form_state['values']['id']);
				expDebug::dPrint('$totalEnrollment : '.$totalEnrollment, 4);
				if($totalEnrollment > 0){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Active Enrollments Available.";
					return $errobj;
				}
				/* if($price > 0 && is_numeric($price))
				{
					if(empty($currency_type))
					{
						return array((object)array('id'=>'currecny_null','object_type'=>''));
					}
					else {
						$selectQuery = db_select('slt_profile_list_items', 'spli');
						$selectQuery->addField('spli','attr1');
						$selectQuery->condition('spli.is_active', 'Y', '=');
						$selectQuery->condition('spli.attr3', 'Y', '=');
						$selectQuery->condition('spli.code', $currency_type, '=');
						$currency_type = $selectQuery->execute()->fetchField();
						if(empty($currency_type))
						{
							return array((object)array('id'=>'invalid_currecny','object_type'=>""));
						}
					}
				}
				else {
					$selectQuery = db_select('slt_profile_list_items', 'spli');
					$selectQuery->addField('spli','attr1');
					$selectQuery->condition('spli.is_active', 'Y', '=');
					$selectQuery->condition('spli.attr3', 'Y', '=');
					$selectQuery->condition('spli.code', $currency_type, '=');

					$currency_type = $selectQuery->execute()->fetchField();
					if(empty($currency_type))
					{
						$currency_type = "USD";
					}
				} */
              
                	//if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
                $ret_tmp = getcustomattributemodulestatus();
                if($ret_tmp == true){
                		include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
                		$custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_trp');
                		expDebug::dPrint('Update training plan API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
                		if($custom_attr_error != '') {
                			$customErrmsg=new stdClass();
                			$customErrmsg->isValidateError = 1;
                			$customErrmsg->errormsg = $custom_attr_error;
        					$customErrmsg->id = $custom_attr_error;
        					return array($customErrmsg);
			}
                	} //#custom_attribute_0078975 - End Check module status
                }            
			$trigger_element = $form_state['storage']['submit_triggering_element'];
			/* Update tag activity Count */
			if($callFromAPI){
				$trigger_element =  $form_state['values']['status'];
			}
			updateTagActivityCount($form_state['values']['id'],getProgramEntityType($form_state['values']['id']),$trigger_element,$callFromAPI);
			/* Update tag activity Count */	
			
			// Select table to update
			$updateStmt = db_update('slt_program');

			$updateStmt->condition('id', $form_state['values']['id']);

			$tp_object_type = checkLearningProgramType($form, $form_state);

			if ($tp_object_type != 'cre_sys_obt_crt' || $default['0']->object_type != 'cre_sys_obt_crt') { // all other checks are done in form validation function
				$form_state['values']['expires_in_value'] = NULL;
				$form_state['values']['expires_in_unit'] = NULL;
			}

			$end_date = null;
			if ($tp_object_type == 'cre_sys_obt_trn' || $default['0']->object_type == 'cre_sys_obt_trn') { // all other checks are done in form validation function
				$old_dateformat = str_replace("-", "/", $form_state['values']['end_date']);
				$end_date = date_format(date_create($old_dateformat), 'Y-m-d H:i');
			}

			$inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
			$in_active_on          = $form_state['values']['status'] == 'lrn_lpn_sts_itv' ? now() : NULL;
			$in_active_reason      = $form_state['values']['status'] == 'lrn_lpn_sts_itv' ? $inactivereasontext : NULL;

			
			if($trigger_element == 'saveandpublish'){
				$tp_status = 'lrn_lpn_sts_atv';
			}
			else if($trigger_element == 'save'){
				$tp_status = 'lrn_lpn_sts_itv';
			}
			else if($form_state['values']['status']){
				$tp_status =$form_state['values']['status'];
			}

			if($trigger_element == 'unpublish'){
				$crs_status = 'lrn_lpn_sts_itv';
				$fields = array( 'status'  => $crs_status );
			}else{
				
				$lang_code =$form_state['values']['lang_code'];
				if(($callFromAPI)&& $tp_status =='lrn_lpn_sts_del' ){
					$lang_code= ($form_state['values']['lang_code'].'_deleted_'.$form_state['values']['id']) ;
				}
				
				$fields = array(
						'title'                 => $form_state['values']['title'],
						'type'                  => 'TP',
						'code'				    => $form_state['values']['code'],
						'lang_code'             => $lang_code,
						'short_desc'            => $form_state['values']['short_desc']['value'],
						'price'                 => $price,
						'currency_type'         => $currency_type,//'cre_sys_crn_usd',
						'status'                => $tp_status,//'lrn_lpn_sts_itv',//$form_state['values']['status']
						'additional_info'       => $additional_info,
						'expires_in_value'      => ($form_state['values']['expires_in_value'] != null && $form_state['values']['expires_in_value'] != '')?$form_state['values']['expires_in_value']:null,
						'expires_in_unit'       => $form_state['values']['expires_in_unit'],
						'enforce_sequence'      => $form_state['values']['enforce_sequence'],
						'updated_on'            => now(),
						'updated_by'            => $userid,
						'end_date'              => $end_date,
						'in_active_on'          => $in_active_on,
						'in_active_reason'      => $in_active_reason,
						'author_vendor'			=> $form_state['values']['author_vendor'],
						'addn_catalog_show'     =>  $is_shown,
                        'addn_notification_show' =>  $is_noti,
				);
			}
			if($trigger_element == 'saveandpublish'){
				$fields['published_on'] = now();
				$fields['updated_on']   = now();
			}
			
			// Select the fields and the values to be updated
			$fields = trimInputFields($fields); // #42124: - Trim form values
			$updateStmt->fields($fields);
			expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
			// Execute query
			$numUpdated = $updateStmt->execute();
			$form_state['values']['short_desc'] = $form_state['values']['short_desc']['value'];
			$object_type=getObjectypeFromProgramID($form_state['values']['id']);
			if($callFromAPI) {
				$tpUpdateUser = $userid;
			}
			else {
				$tpUpdateUser = getIdOfLoggedInUser();
			}
			make_audit_trail_entries($fields, $tpUpdateUser , $default ,$form_state['values']['id'], 'Updated',$object_type,'exp_sp_administration_program','Update Training Plan',
					array('title', 'code', 'short_desc', 'additional_info','addn_catalog_show','addn_notification_show','lang_code', 'status','price','currency_type','expires_in_value','expires_in_unit','end_date','author_vendor'));


			expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);


			// adminCreateLMSProductTpNode($form, $form_state,$operation);
			return array((object)array('id'=>$form_state['values']['id']));

		}
		catch (Exception $ex) {
			$txn->rollback();
			watchdog_exception('prgadmin', $ex);
			if($callFromAPI){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Unable to update a program.";
				return $errobj;
			}
			else {
				return array((object)array('id'=>'Failiure'));
			}
			throw $ex;
			// return array((object)array('id'=>"failure"));

		}

	} catch (Exception $ex) {
		watchdog_exception('updateProgramDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getTrainingPirceAttribute - Getting the  Price Attribute columan name
 */
function getTrainingPirceAttribute($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  try {
      // Select the table
      $select = db_select('slt_profile_list_items','spli');
      // Select the fields to fetch
      $select->addField('spli','code', 'code');
      $select->addField('spli','attr1', 'attr1');
     // $select->addField('spli','name', 'attr1');
      // Order results by name
      $select->orderBy('name');
      // Set the conditions
      $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
      $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
      if (!$alsoFetchInactiveItems) {
      	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
      }
      expDebug::dPrintDBAPI(' $select object = ' , $select);

      $result = $select->execute()->fetchAllKeyed();
      expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

      // Return results
      return $result;
  } catch (Exception $ex) {
    watchdog_exception('getTrainingPirceAttribute', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * adminCreateLMSProductTpNode - Insert the product id in the drupal node tables and other releated stuff
 */
function adminCreateLMSProductTpNode($form, &$form_state,$operation,$paramuserid){
  try {
      $type =checkLearningProgramType($form, $form_state);
      $currency_type     = $form_state['values']['currency_type'];
      //$vActivityId=$inObj->InsValue;
      $vActivityId=$form_state['values']['id'];
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
      $oNode = new StdClass();
      if($operation=='update'){
    			$results =adminLearningActivity($vActivityId, $type);
    			expDebug::dPrint("node results");
    			expDebug::dPrint($results , 4);
    			$oNode->nid=$results[0]->nid;
                $oNode->vid=$results[0]->vid;
      }
      $oNode->uid=$userid;
      $oNode->language='en';
      $oNode->title= str_replace("''","'",$form_state['values']['title']);
      $oNode->body= $form_state['values']['short_desc'];
      $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$type."'")->fetchField();
      $oNode->model=$delivery_type_name.'-Code-'.$form_state['values']['code'];
      $oNode->sell_price= ($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
      $oNode->cost=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
      $oNode->list_price=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
      $currency_type = ($form_state['values']['currency_type']!=null && $form_state['values']['currency_type']!='')?$form_state['values']['currency_type']:'';
      if($currency_type != '')
      	  $oNode->currency_type=db_query("select attr1 FROM slt_profile_list_items WHERE attr1 ='".$currency_type."' AND code LIKE 'cre_sys_crn_%' AND is_active='Y' AND attr3='Y'")->fetchField();
      else
     {
      	  $UserDefaultCurrencyArr = getUserDefaultCurrency(1);
      	  $oNode->currency_type = $UserDefaultCurrencyArr['currency_code'];
      	  if($oNode->currency_type == "")
      	  	  $oNode->currency_type = "USD";
      }
      $oNode->type='product';
      $oNode->created=time();
      $oNode->changed=time();
      $oNode->teaser_include=1;
      //$oNode->currency_type = $currency_type;
      expDebug::dPrint("operation-123456 ". $oNode->currency_type , 5);
      $oNode->revision= ($operation == 'update')? '' : 1;
      //$oNode->revision=  1;
     // $node->teaser=$form_state['values']['short_desc']['value'];

      $oNode->format=1;
      $oNode->log='';
      $oNode->name=$userid;
      $oNode->date='';
      $prgStatus = (($form_state['values']['status'] == 'lrn_lpn_sts_atv') || ($form_state['storage']['submit_triggering_element'] == 'saveandpublish')) ? 1 : 0;
      $oNode->status= $prgStatus;
      $oNode->promote=0;
      $oNode->sticky=0;

      $oNode->op="Save";
      $oNode->submit="Save";
      $oNode->preview="Preview";
      $oNode->form_id="product_node_form";


// Implemention of Forum intergration : Start

    $forumNode = new StdClass();

    if($operation=='update'){
      $forumNodeSelect = db_select('slt_node_learning_activity', 'entity');
      $forumNodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
      $forumNodeSelect->addField('n','nid','nid');
      $forumNodeSelect->addField('n','vid','vid');
      $forumNodeSelect->condition('entity.entity_id', $vActivityId);
      $forumNodeSelect->condition('entity.entity_type', 'TP');
      $forumNodeSelect->condition('entity.custom0', 'forum');
      $forumNodeResult =  $forumNodeSelect->execute()->fetchAll();

      expDebug::dPrintDBAPI("BALAG slt_node_learning_activity-----  : ",$forumNodeSelect);

      $forumNode->nid = $forumNodeResult[0]->nid;
      $forumNode->vid = $forumNodeResult[0]->vid;
    }

    $forumNode->uid=$userid;
    $forumNode->language='en';
    $forumNode->title= str_replace("''","'",$form_state['values']['title']);
    $forumNode->body= $form_state['values']['short_desc'];
    $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$type."'")->fetchField();
    $forumNode->model=$delivery_type_name.'-Code-'.$form_state['values']['code'];
    $forumNode->sell_price= ($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
    $forumNode->cost=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
    $forumNode->list_price=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;

    $forumNode->type='forum';
    $forumNode->created=time();
    $forumNode->changed=time();
    $forumNode->teaser_include=1;
    expDebug::dPrint("operation- ". $operation , 4);
    $forumNode->revision= ($operation == 'update')? '' : 1;
    $forumNode->currency_type=$currency_type;

    $forumNode->format=1;
    $forumNode->log='';
    $forumNode->name=$userid;
    $forumNode->date='';
    $prgStatus = (($form_state['values']['status'] == 'lrn_lpn_sts_atv') || ($form_state['storage']['submit_triggering_element'] == 'saveandpublish')) ? 1 : 0;
    $forumNode->status= $prgStatus;
    $forumNode->promote=0;
    $forumNode->sticky=0;

    $forumNode->op="Save";
    $forumNode->submit="Save";
    $forumNode->preview="Preview";
    $forumNode->form_id="product_node_form";

    // Implemention of Forum intergration : End

       expDebug::dPrint("Drupal UserId  : ".drupal_user_node_id." - ".$oNode->form_id , 4);

      $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
      expDebug::dPrintDBAPI("Delivery_type id : v_id - ", $vid);

      if($vid){
          $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
            if(empty($delivery_type_all_tid)) {
              $terms               = new stdClass;
              $terms->name         = '--All Tp--';
              $terms->vid          = $vid;
              //$terms->description  = '';
              //$terms->parent       = '';
              taxonomy_term_save($terms);
              $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
            }
      $oNode->taxonomy_catalog['und'] = array();
      $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
      $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$type."' and vid = ".$vid)->fetchField();

      expDebug::dPrintDBAPI("Delivery_type id :  - " , $delivery_type_tid);
      expDebug::dPrint("Training Plan Id :  - ".$form_state['values']['id'] , 5);
      $dtTerms = array(
         'name' => $delivery_type_name,
         'vid' => $vid,
         'description' => $type,
         'parent' => $delivery_type_all_tid,
      );

      if(empty($delivery_type_tid)) {
         $oNode->taxonomy_catalog['und'][] = array('tid' => admincreateVocabTerms($dtTerms));
      }
      else {
         $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
      }
      $delivery_type_prg_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$type."' and vid = ".$vid)->fetchField();
      $class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$vActivityId."' and vid = ".$vid)->fetchField();
      $dtTerms1 = array(
         'name' => $form_state['values']['title'].' : '.$form_state['values']['code'],
         'vid' => $vid,
         'description' => $vActivityId,
         'parent' => $delivery_type_prg_tid
       );
      if(!empty($class_tid)){
        $dtTerms1['tid'] =  $class_tid;
      }

      $tid = admincreateVocabTerms($dtTerms1);
      expDebug::dPrint(' Taxonomy id: $tid = ' . print_r($tid, true) , 4);

      //$oNode->taxonomy_catalog['und'] = array();
      $oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
      $oNode->teaser='';
      $oNode->validated=1;
      expDebug::dPrint("Tp Node :");
      expDebug::dPrint($oNode ,4);
      $oNode = trimInputFields($oNode); // #42124: - Trim form values
      node_save($oNode);
        if($operation=='insert') {
          $vNodeId=  $oNode->nid;
          $lastinsertid=adminInsertLearningActivity($vNodeId, $vActivityId, $type, $userid);
         }

// Node creation for Forum integration : Start
     $forumNode = trimInputFields($forumNode); // #42124: - Trim form values
     if($operation=='insert') {
         // Getting forum  vocabulary id
        $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();

        $forumTerms               = new stdClass;
        $forumTerms->name         = str_replace("''","'",trimInput($form_state['values']['title']));
        $forumTerms->vid          = $forumvid;
        $forumTerms->description  = "TP";//$form_state['values']['short_desc'];
        $forumTerms->format       = $vActivityId;

        taxonomy_term_save($forumTerms);

        node_save($forumNode);

        $forumNodeId = $forumNode->nid;

        $nodeInsertForum = db_insert('slt_node_learning_activity');
        $nodeFieldsForum = array(
                    'node_id'      => $forumNodeId,
                    'entity_id'    => $vActivityId,  // class id
    	            	'entity_type'  => 'TP',//$type,
    		       			'createdby'    => $userid,
                    'createdon'    => now(),
        						'updatedby'    => $userid,
        						'updatedon'    => now(),
      							'custom0'      => 'forum'
              );
        $nodeInsertForum->fields($nodeFieldsForum);
        $nodeInsertForum->execute();

      }else{

        $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
        $forum_tid = db_query("select tid FROM taxonomy_term_data WHERE description = 'TP' AND format ='".$vActivityId."' AND vid = ".$forumvid)->fetchField();

        node_save($forumNode);

        $forumTerms = array(
           'name'        => trimInput($form_state['values']['title']),
           'vid'         => $forumvid,
           'description' => "TP",//$description,
           'format'      => $vActivityId
         );

        if(!empty($forum_tid)){
          $forumTerms['tid'] =  $forum_tid;
        }

        expDebug::dPrint(' Forum Update = ' .$vActivityId.'--===-'. $forum_tid. '--===-'. $forumTerms['tid']);

        //$tid = taxonomyVocabTerms($forumTerms);

        $tid = admincreateVocabTerms($forumTerms);

        node_save($forumNode);

      }

      // Node creation for Forum integration : End


      }
      // Clears the Program filter data.
    cache_set('program_filters','');
  } catch (Exception $ex) {
    watchdog_exception('adminCreateLMSProductTpNode', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * admincreateVocabTerms - Insert the product id in the drupal taxonomy tables and other releated stuff
 */
function admincreateVocabTerms($termsTxt) {
  try {
      $terms               = new stdClass;
      $terms->name         = $termsTxt["name"];
      $terms->vid          = $termsTxt["vid"];
      if(!empty($termsTxt["description"])) {
        $terms->description  = $termsTxt["description"];
      }
      if(!empty($termsTxt["parent"])) {
        $terms->parent       = $termsTxt["parent"];
      }
      if(!empty($termsTxt["tid"])) {
        $terms->tid       = $termsTxt["tid"];
      }
      taxonomy_term_save($terms);
      return $terms->tid;
   } catch (Exception $ex) {
    watchdog_exception('admincreateVocabTerms', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * adminLearningActivity - Select the learnining activity details
 */
  function adminLearningActivity($entity_id, $type){

  try {
      $select = db_select('slt_node_learning_activity', 'entity');

      $select->leftJoin('node', 'nodetype', 'entity.node_id = nodetype.nid');

      $select->addField('nodetype', 'nid', 'nid');
      $select->addField('nodetype', 'vid', 'vid');

      $select->condition('entity.entity_id', $entity_id, '=');
      $select->condition('entity.entity_type ', $type, '=');

      $result = $select->execute()->fetchAll();
      expDebug::dPrintDBAPI(' $select object = ' , $select);
      expDebug::dPrint('  LIST $result = ' . print_r($result, true) , 34);

       return  $result;

  } catch (Exception $ex) {
    watchdog_exception('adminLearningActivity', $ex);
    expertusErrorThrow($ex);
  }
 }

/*
 * adminInsertLearningActivity - Inserting node learnining activity details
 */
function adminInsertLearningActivity($vNodeId, $vActivityId, $type, $userid){

   $txn = db_transaction();
  try {

    $insertStmt = db_insert('slt_node_learning_activity');
    $fields = array(
                'node_id'     => $vNodeId,
                'entity_id'   => $vActivityId,
                'entity_type' => $type,
                'createdby'   => $userid,
                'createdon'   => now(),
    						'updatedby'   => $userid,
    						'updatedon'   => now(),

              );

    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);

    $lastinserid = $insertStmt->execute();
    expDebug::dPrint(' $lastinserid = ' . print_r($lastinserid, true) , 3);
   return $lastinserid;
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('adminInsertLearningActivity', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
   // return array((object)array('id'=>"failure"));

  }
  unset($txn);
 }

 function AttachModuleToTPAPI(&$form_state,$createuserid="")
 {
 	try {
 		//include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
 		$form=array();
 		$form["form_id"]="";
 		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
 		$program_id   =    $form_state['values']['program_id'];
 		$modulename     =    $form_state['values']['module_name'];
 		$errobj = new stdClass();
		
 		//Program id validation check
 		$prgDetails = getTrainingProgramDetails($program_id, array('id','object_type','expires_in_value','expires_in_unit'));
 		if(empty($prgDetails['id'])) {
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Invalid program id.";
 			return $errobj;
 		}
 		
 		//Add module is allowed for program or not.
 		$module_list = getModuleIdFromObjectId($program_id);
 		
 		$lastModId = getModuleIdFromObjectId($program_id,count($module_list));
	 	expDebug::dPrint("end of the functions -->> ".print_r($lastModId,true) ,5 );
	 	$crs_cnt = getGrpCourseCnt($program_id,$lastModId[0]->module_id);
	 	expDebug::dPrint("course count for last module -->> ".$crs_cnt ,5 );
	 	
		if($prgDetails['object_type'] != "cre_sys_obt_crt" || $crs_cnt == 0 || empty($prgDetails['expires_in_value'])){
			if($prgDetails['object_type'] != "cre_sys_obt_crt")
				$msg = "Next Module level is applicable only for Certification.";
			else if(empty($prgDetails['expires_in_value']))
				$msg = "Certification doesn't have expiry value, so new module cannot be created.";
			else
				$msg = "Courses not yet added for previous module.";
				
			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = $msg;
 			return $errobj;
		}
 		
 		// Module Name already Exists
 		$select = db_select("slt_module", "module");
 		$select->addField("module", "id"); 			
 		$select->condition("module.program_id", $program_id);
 		$select->condition("module.title", $modulename);
 		$moduleId = $select->execute()->fetchField();
 		if(!empty($moduleId)) {
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Module Name already exists.";
 			return $errobj;
 		}
 		// Module Name length validation
 		if(strlen($modulename)> 50){
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Module Name cannot be longer than 50 characters.";
 			return $errobj;
 		}
 		if(preg_match('/[^a-z A-Z0-9@*._-\']+/i', $modulename)) {
 			$errobj->field = 'module_name';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Module name contains invalid characters';
 			return $errobj;
 		}
 		$new_module_id = addnewModuleForTP($program_id,$createuserid,$prgDetails['object_type'],$modulename);
 		if($new_module_id && $new_module_id != 'Failiure'){
 			return array((object)array('id'=>$new_module_id));
 		}
 		else{
 			return array((object)array('id'=>'Failure'));
 		}
 		
 	} catch (Exception $ex) {
 		watchdog_exception('trainingProgram', $ex);
 		expertusErrorThrow($ex);
 	}

 }
 function UpdateModuleToTPAPI(&$form_state,$createuserid="")
 {
 	try {
 		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
 		$form=array();
 		$form["form_id"]="";
 		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
 		$program_id   =    $form_state['values']['program_id'];
 		$module_id   =    $form_state['values']['module_id'];
 		$modulename     =    $form_state['values']['module_name'];
 		$errobj = new stdClass();
		
 		//Program id validation check
 		$prgDetails = getTrainingProgramDetails($program_id, array('id','object_type','expires_in_value','expires_in_unit'));
 		if(count($prgDetails) == 0) {
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Invalid program id.";
 			return $errobj;
 		}
 		
 		// Module id validate
 		$select = db_select("slt_module", "module");
 		$select->addField("module", "id"); 			
 		$select->condition("module.program_id", $program_id);
 		$select->condition("module.id", $module_id);
 		$moduleId = $select->execute()->fetchField();
 		if(empty($moduleId)) {
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Invalid module id.";
 			return $errobj;
 			
 		}
 		// Module Name already Exists
 		$select = db_select("slt_module", "module");
 		$select->addField("module", "id"); 			
 		$select->condition("module.program_id", $program_id);
 		$select->condition("module.id", $module_id, '=');
 		$select->condition("module.title", $modulename);
 		$moduleId_check = $select->execute()->fetchField();
 		if(!empty($moduleId_check)) {
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Module Name already exists.";
 			return $errobj;
 		}
 		// Module Name length validation
 		if(strlen($modulename)> 50){
 			$errobj=new stdClass();
 			$errobj->isValidateError = 1;
 			$errobj->errcode = 'L_013';
 			$errobj->errormsg = "Module Name cannot be longer than 50 characters.";
 			return $errobj;
 		}
 		if(preg_match('/[^a-z A-Z0-9@*._-\']+/i', $modulename)) {
 			$errobj->field = 'module_name';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Module name contains invalid characters';
 			return $errobj;
 		}
 		$result_obj = attachedcourse_group_update($program_id,"module",$modulename,$moduleId,'','api');
 		if($result_obj == 1){
 			return array((object)array('id'=>$moduleId));
 		}
 		
 	} catch (Exception $ex) {
 		watchdog_exception('trainingProgram', $ex);
 		expertusErrorThrow($ex);
 	}

 }
 
 
/*
 * AttachCourseToTPAPI - Inserting the Course Attachements details for the Training Plan from API
 */

 function AttachCourseToTPAPI( &$form_state,$createuserid="")
 {
 	try {
 		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
 		$form=array();
 		$form["form_id"]="";
 		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
 		$program_id   =    $form_state['values']['program_id'];
 		$courseid     =    $form_state['values']['course_id'];
 		$groupName    =    $form_state['values']['groupname'];
 		$modulename    =    $form_state['values']['modulename'];
 		$isRequired   =	   $form_state['values']['isrequired'];
 		$errobj=new stdClass();
 		
 		if(empty($modulename)) {
 			$select = db_select("slt_program", "prg");
 			$select->addField("prg", "id");
 			$select->condition("prg.id", $program_id, "=");
 			$select->condition("prg.object_type", "cre_sys_obt_crt",'=');
 			$result_set = $select->execute();
 					
 			if($result_set->rowCount() > 0){
 			$modulename = getModulename($program_id);
 			}		
 		} 	
 		
 		$select = db_select("slt_module", "module");
 		$select->addField("module", "id"); 			
 		$select->condition("module.program_id", $program_id);
 		$select->condition("module.title", $modulename);
 		$moduleId = $select->execute()->fetchField();
  		
 		if($program_id > 0 && is_numeric($program_id))
 		{
 			$select = db_select("slt_program", "sp");
 			$select->addField("sp", "id");
 			$select->condition("sp.status", 'lrn_lpn_sts_del', "!=");
 			$select->condition("sp.id", $program_id, "=");
 			$result_set = $select->execute();
 			if(!($result_set->rowCount() > 0))
 			{
 				$errobj->field = 'program_id';
 				$errobj->isValidateError = 1;
 				$errobj->errormsg = 'Program id is invalid or doesn\'t exist';
 				return $errobj;
 			}
 			$checkemptygroup = objectLevelAccessCheckInGroupMap('cre_sys_obt_crs',array($courseid));
 			expDebug::dPrint(' check11 '.print_r($checkemptygroup, true),4);
 			if(!empty($checkemptygroup)){
 				$check=apiUseraccesscheck($createuserid,'cre_sys_obt_crs',$courseid);
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
 		}
 		else
 		{
 			$errobj->field = 'program_id';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Invalid program id';
 			return $errobj;
 		}
 		if(getTotalTPEnroll($program_id) > 0) {
 			$errobj->field = 'program_id';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'The given training program has active enrollments.';
 			return $errobj;
 		}
 		if($courseid > 0 && is_numeric($courseid))
 		{
 			$select = db_select("slt_course_template", "sct");
 			$select->addField("sct", "id");
 			$select->condition("sct.status", 'lrn_crs_sts_atv', "=");
 			$select->condition("sct.id", $courseid, "=");
 			$result_set = $select->execute();
       if(!$result_set->rowCount() > 0)
 			{
 				$errobj->field = 'course_id';
 				$errobj->isValidateError = 1;
 				$errobj->errormsg = 'Course id is invalid or doesn\'t exist';
 				return $errobj;
 			}
 		}
 		else
 		{
 			$errobj->field = 'course_id';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Invalid course id';
 			return $errobj;
 		}

 		$select = db_select("slt_module_crs_mapping", "smcm");
 		$select->addField("smcm", "id");
 		$select->condition("smcm.program_id", $program_id, "=");
 		$select->condition("smcm.course_id", $courseid, "=");
 		$select->condition("smcm.module_id", $moduleId,'=');
 		$select->condition("smcm.object_type", "cre_sys_obt_crt",'!='); 		
 		$result_set = $select->execute();
 		if($result_set->rowCount() > 0)
 		{
 			$errobj->field = 'course_id';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'The given course has already been attached to the given training plan';
 			return $errobj;
 		}
 		
 		$select = db_select("slt_module_crs_mapping", "smcm");
 		$select->addField("smcm", "id");
 		$select->condition("smcm.program_id", $program_id, "=");
 		$select->condition("smcm.course_id", $courseid, "=");
 		$select->condition("smcm.module_id", $moduleId,'=');
 		$select->condition("smcm.object_type", "cre_sys_obt_crt",'=');
 		$result_set_crt = $select->execute();
 		if($result_set_crt->rowCount() > 0)
 		{
 			$errobj->field = 'course_id';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'The given course has already been attached to the given training plan in the Module : '.$modulename;
 			return $errobj;
 		} 
 		if(preg_match('/[^a-z A-Z0-9@*._-]+/i', $groupName)) {
 			$errobj->field = 'groupname';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Group name contains invalid characters';
 			return $errobj;
 		}
 		if($isRequired != 0 && $isRequired != 1){
 			$errobj->field = 'groupname';
 			$errobj->isValidateError = 1;
 			$errobj->errormsg = 'Is required should be 0 or 1';
 			return $errobj;
 		}

 		if(!empty($modulename)) { 
 			
 			$select = db_select("slt_program", "prg");
 			$select->innerJoin('slt_module', 'module', 'module.program_id=prg.id');
 			$select->addExpression('count(module.id)', 'cnt'); 			
 			$select->condition("prg.id", $program_id);
 			$select->condition("prg.object_type", "cre_sys_obt_crt",'!=');			
 			$select->condition("module.title", $modulename,'!=');
 			expDebug::dPrintDBAPI('fetch module iddsasaasAS : ',$select);
  			$ExistsInModule = $select->execute()->fetchField();
  			expDebug::dPrint('result '.print_r($ExistsInModule,true), 4);
  			
  			$select = db_select("slt_program", "prg");
  			$select->innerJoin('slt_module', 'module', 'module.program_id=prg.id');
  			$select->addExpression('count(module.id)', 'cnt');
  			$select->condition("prg.id", $program_id);
  			$select->condition("prg.object_type", "cre_sys_obt_crt",'=');
  			$select->condition("module.title", $modulename,'=');
  			$ExistsInModuleAPI = $select->execute()->fetchField();
  			if(empty($ExistsInModuleAPI))
  			{
  				$errobj->field = 'modulename';
  				$errobj->isValidateError = 1;
  				$errobj->errormsg = 'Module name doesn\'t exist';
  				return $errobj;
  			}
  			
 			if(!empty($ExistsInModule))
 			{
 				$errobj->field = 'modulename';
 				$errobj->isValidateError = 1;
 				$errobj->errormsg = 'Next Module level is applicable only for Certification, Please check ModuleName';
 				return $errobj;
 			}
 			
 		} 
 		
 		//$tpId =  customattachcourseToTP($form,$form_state,1);
 		$tpId =  courseAssignToTrainingProgram($form,$form_state,1,1,$moduleId);

 		expDebug::dPrint('addNewTPByRestAPI created tp:' . print_r($tpId,true) ,4);
 		if($tpId[0]->id && $tpId[0]->id != 'Failiure'){
 			$form_state['values']['id'] = $tpId[0]->id;
 			return $tpId;
 		}
 		else{
 			return array((object)array('id'=>'Failure'));
 		}
 	} catch (Exception $ex) {
 		watchdog_exception('trainingProgram', $ex);
 		expertusErrorThrow($ex);
 	}

 }

 /*
  * courseAssignToTrainingProgram - Inserting the Course Attachements details for the Training Plan
 */

 function courseAssignToTrainingProgram($form, $form_state, $userid, $callFromAPI = 0,$module_id){
 	try {
 		expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
 		expDebug::dPrint(' $form_state = ' . print_r($form_state , true) , 5);
 		expDebug::dPrint(' $form_state values= ' . print_r($form_state['values'], true) , 5);

 		if($callFromAPI == 1)
 		{
 			$program_id   =    $form_state['values']['program_id'];
 			$courseid     =    $form_state['values']['course_id'];
 			$groupName 	  =    $form_state['values']['groupname'];
 			$modulename   =    $form_state['values']['modulename'];
 			$isRequired=($form_state['values']['isrequired'] == 1)?'Y' : 'N';
 		}
 		else {
	 		$program_id   =    $form_state['values']['editprogram_id'];
	 		//$courseid=$form_state['input']['courseset_fieldset']['courseset']['hid_courseset_id_0'];
	 		$courseid     =    $form_state['input']['hid_courseset_id_0'];
	 		$groupName = $form_state['values']['tpattachinggroupname'];
	 		expDebug::dPrint("Tp Node :". $program_id.'--'.$userid.$courseid.'-'.$courseid1.'new'.$course_new.'course is'. $form_state['values']['set_new_course_id'] , 5);
	 		$isRequired = 'Y';
 		}
 		if(empty($modulename)) { 		

 			$modulename = getModulename($program_id);
 		}
 		if(empty($module_id)) {
 			$module_id = FetchModuleId($program_id);
 		}
 		if(!empty($courseid)){
 			$random_num=rand(1,99999);
 			//$auto_code="modco_".$random_num;
 			$autogroup_code="grpco_".$random_num;
 			
 			//$uniquecode=checkUniqueGroupCode($auto_code);
 			//Checking existing Group name
 			$txn = db_transaction();expDebug::dPrint("Tp Node :". $groupName , 5);
 			try {
 				$checkModuleName  =   checkingTPModuleGroupName($modulename,$program_id,''); 				
 				$checkGroupName   =   checkGrpNameExistsForModule($module_id,$program_id,$groupName); 				
 						
 				if(empty($checkModuleName)){
 					
 					$object_type=getObjectypeFromProgramID($program_id);
 					$select = db_select('slt_module', 'sltmodule');
 					$select->addExpression('COUNT(sltmodule.program_id)', 'countnewval');
 					$select->condition('sltmodule.program_id', $program_id, '=');
 					$tpmoduleCount = $select->execute()->fetchField();
 					expDebug::dPrintDBAPI(' fetch_learning_courses_list() count:  = ', $select);
 					expDebug::dPrint(' $tpmoduleCount = ' . print_r($tpmoduleCount, true) , 3);

 					
 					$insertStmt = db_insert('slt_module');
 					$fields = array(
 							'program_id'  		   => $program_id,
 							'title'              => $modulename,
 							'code'               => '',
 							'created_by'         => $userid,
 							'sequence'			 => $tpmoduleCount+1,
 							'created_on'         => now(),
 							'updated_by'         => $userid,
 							'updated_on'         => now(),
 							'status'             => 'spp_tpm_mdl_atv',
 							'is_required'        => '1',
 							'parent_id'			     => '0',

 					);

 					// Select the fields and the values to be inserted
 					$insertStmt->fields($fields);
 					expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);
 					$module_id = $insertStmt->execute();

					//update module code 
 					$auto_code="modco_".$module_id;
 					$updateStmt = db_update('slt_module');
 					$updateStmt->condition('id', $module_id, '='); 				
 					$updateStmt->fields(array(
 							'code'     =>  $auto_code
 					));
 					expDebug::dPrintDBAPI(' update module code  = ', $updateStmt);
 					$numUpdated = $updateStmt->execute(); 


 					make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$module_id, 'Added ','Module','exp_sp_administration_program','new module',
 							array('program_id', 'title', 'code','sequence','is_required'));
 				}else{
 					$module_id=$checkModuleName;
 				}
 				
 				if(empty($checkGroupName)){		
//  					$exisgrpList = getModulegrpList($module_id);
//  					$updateUserlist = db_update('slt_module');
//  					$updateUserlist->condition('id', $module_id, '='); 					
//  					$updateUserlist->fields(array(
//  							'group_list'     =>  ((empty($exisgrpList)) ? '' : $exisgrpList . ',') .$groupName,
//  							'updated_on'    => now(),
//  							'updated_by'    => $loggedUserId,
//  					));
 					
//  					$updateUserlist->execute(); 

 					$random_num=rand(1,99999);
 					$grp_code="grpco_".$random_num;
 					
 					$insertStmt = db_insert('slt_module_groups');
 					$fields = array(
 							'program_id'  		 => $program_id,
 							'title'              => $groupName,
 							'code'               => $grp_code,
 							'module_id'   		 => $module_id,
 							'created_by'         => $userid,
 							'sequence'			 => 9999,
 							'created_on'         => now(),
 							'updated_by'         => $userid,
 							'updated_on'         => now(),
 					);
 					$insertStmt->fields($fields);
 					$newgrpid = $insertStmt->execute();
 					expDebug::dPrint("Tp Node456 :". $checkModuleName , 5);
 					expDebug::dPrintDBAPI(' firstyt insert ', $insertStmt);
 				} else{
 					$newgrpid = $checkGroupName;
 				} 
 				

 				$object_type=getObjectypeFromProgramID($program_id);


 				//$isRequired = $form_state['values']['is_required_'.$programId.'_'.$courseId];
 				//$isRequired = empty($isRequired) ? 'Y' : $isRequired;
 				// Checking for Re-certify
 				$recertify=($form_state['values']['recertify'] == 0)?'N' : 'Y';
 				expDebug::dPrint('recertify ----'.$recertify.'object_type'.$object_type.$form_state['values']['recertify'] , 5);
 				/*
 				if($form_state['values']['recertify']=='1' && $object_type=='cre_sys_obt_crt'){

 					$checkRecertify=checkingTpAssingnedRecertify($courseid, $program_id);
 					if($checkRecertify==0){
 						$insertRecertify = db_insert('slt_re_certify');
 						$fields = array(
 								'program_id'  => $program_id,
 								'course_id'   => $courseid,
 								'module_id'   => $module_id,
 								'created_by'  => $userid,
 								'created_on'  => now(),
 								'updated_by'  => $userid,
 								'updated_on'  => now(),
 								'is_required' => $isRequired,

 						);
 						$insertRecertify->fields($fields);
 						$insertRecertify->execute();
 						$auditMessage = "new course recertify";


 					}
 				}else{ */
 			
 				
 				
 				
 					$checkTp = checkingTpAssingnedCourses($courseid, $program_id,$module_id);
 					$seq = getGroupCodeFromGrpTitle($module_id,$grpId);
 					$lastSeqNo = !empty($seq) ? $seq+1 : 1;
// 					$select = db_select('slt_module_crs_mapping', 'sltmapping');
//  					$select->addExpression('COUNT(sltmapping.program_id)', 'countnewval');
//  					$select->condition('sltmapping.program_id', $program_id, '=');
//  					$select->condition('sltmapping.group_title', $groupName, '='); 					 				
//  					$tpmoduleCount = $select->execute()->fetchField();

//  					//select group_code from  slt_module_crs_mapping
//  					$select = db_select('slt_module_crs_mapping', 'grps'); 			 
//  					$select->addField('grps','group_code', 'group_code');
//  					$select->condition('grps.program_id', $program_id, '=');
//  					$select->condition('grps.module_id', $module_id, '=');
//  					expDebug::dPrintDBAPI('autogroup code SQL = ' , $select);
//  					$autogroup_result = $select->execute()->fetchField();
//  					expDebug::dPrint('$autogroup_result = ' .$autogroup_result,5);
 					
 					if(!empty($autogroup_result))  $autogroup_code = $autogroup_result;
 					
 					else $autogroup_code = $autogroup_code;  

 					if($checkTp == 0) {
 						$insertCoursMapping = db_insert('slt_module_crs_mapping');
 						$fields = array(
 								'program_id'   => $program_id,
 								'module_id'    => $module_id,
 								'course_id'    => $courseid,
 								'object_type'  => $object_type,
 								'group_id' => $newgrpid,
//  								'group_title'  => $groupName,
//  								'group_code'   => $autogroup_code,
 								'created_by'   => $userid,
 								'created_on'   => now(),
 								'updated_by'   => $userid,
 								'updated_on'   => now(),
 								'is_required'  => $isRequired,
 								'sequence'     => $lastSeqNo

 						);
 						$insertCoursMapping->fields($fields);
 						expDebug::dPrintDBAPI(' new module crs mapping insert:  = ', $insertCoursMapping);
 						$lastinserid = $insertCoursMapping->execute();
 						$auditMessage = "new course";

 					// }

 					make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$program_id, 'Added ',$object_type,'exp_sp_administration_program',$auditMessage,
 							array('module_id', 'course_id', 'is_required'));
 					$lastSeqNo++;
 				}
 				if($callFromAPI == 1)
 					return array((object)array('id'=>$lastinserid));
 			}catch (Exception $ex) {
 				$txn->rollback();
 				watchdog_exception('Assigncourses', $ex);
 				throw $ex;
 				if($callFromAPI == 1)
 					return array((object)array('id'=>"failure"));

 			}
 			unset($txn);

 		}
 	} catch (Exception $ex) {
 		watchdog_exception('courseAssignToTrainingProgram', $ex);
 		expertusErrorThrow($ex);
 	}
 }




/*
 * checkingTpAssingnedCourses - Checking assigned course count for the training program
 */
function checkingTpAssingnedCourses($courseid, $programid,$moduleid = ''){
  try {
      $select = db_select('slt_module_crs_mapping', 'mapping');
      $select->addExpression('COUNT(mapping.id)', 'count');
      // Set the conditions
      $select->condition('mapping.course_id', $courseid, '=');
      $select->condition('mapping.program_id', $programid, '=');
      if(!empty($moduleid))
      	$select->condition('mapping.module_id', $moduleid, '=');
      // Execute the query
      expDebug::dPrintDBAPI(' $select checkingTpAssingnedCourses = ' , $select);
      $courseCount = $select->execute()->fetchField();
      expDebug::dPrint(' $uniquecount = ' . print_r($courseCount, true) , 3);
      return $courseCount; // a valid new name
  } catch (Exception $ex) {
    watchdog_exception('checkingTpAssingnedCourses', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * getObjectypeFromProgramID - Getting object type from program id.
 */
function getObjectypeFromProgramID($pid){
  try {
     $select = db_select('slt_program', 'prgs');

     $select->addField('prgs', 'object_type', 'object_type');
    // Set the conditions
    $select->condition('prgs.id', $pid, '=');
     $object_type = $select->execute()->fetchField();
     expDebug::dPrint(' $insertStmt obj = ' . print_r($object_type, true) , 3);
     return $object_type;
  } catch (Exception $ex) {
    watchdog_exception('getObjectypeFromProgramID', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 * getProfileItemNamesKeyedAdmin() - Function to fetch status list in a particular order
 * @TODO: This funciton needs to be rewritten as a generic function for all modules in exp_sp_administration.inc
 */
function getProfileItemNamesKeyedAdmin($codePrefix) {
  try {
      // Select the table
      $select = db_query('SELECT code, name FROM slt_profile_list_items WHERE code like \''.$codePrefix.'%\' AND is_active = \'Y\' ORDER BY FIELD(CODE,\'lrn_lpn_sts_atv\',\'lrn_lpn_sts_itv\',\'lrn_lpn_sts_arc\',\'lrn_lpn_sts_dft\')');
      $result = $select->fetchAllKeyed();
      expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

      // Return results
      return $result;
  } catch (Exception $ex) {
    watchdog_exception('getProfileItemNamesKeyedAdmin', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getAttachedTpCourses - get the all attached courses for a particular training program
 */
function getAttachedTpCourses($programid,$start = 0,$end = 0, $courseName, $courseSearchType,$module_id,$group_code =''){
  try {

      $select = db_select('slt_module', 'mdl');
      $select->leftJoin('slt_module_groups', 'grps', 'mdl.id = grps.module_id');
      //$select->leftJoin('slt_module_crs_mapping', 'maps', '(maps.group_id = grps.id OR (maps.group_id = 0 AND mdl.id = maps.module_id))');
      $select->leftJoin('slt_module_crs_mapping', 'maps', '((maps.group_id = grps.id AND mdl.id = maps.module_id) OR (maps.group_id = 0 AND mdl.id = maps.module_id))');
      $select->leftJoin('slt_course_template', 'tmplt', 'tmplt.id = maps.course_id');
      $select->leftJoin('slt_profile_list_items', 'plistatus', 'tmplt.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plilang', 'tmplt.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_module_crs_mapping', 'maps', 'maps.course_id = tmplt.id');
      $select->leftJoin('slt_module', 'modules', 'modules.id = maps.module_id');
      $select->leftJoin('slt_tag_entity','te','(tmplt.id=te.entity_id and te.entity_type=\'Course\')');
      $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
      $select->distinct();
      $select->addField('tmplt', 'code', 'code');
      $select->addField('mdl', 'id', 'module_id');
      $select->addField('grps', 'id', 'group_id');
      $select->addField('grps', 'title', 'group_title');
      $select->addField('grps', 'code', 'group_code');
      $select->addField('grps', 'sequence', 'group_seq');
      $select->addField('tmplt', 'title', 'title');
      $select->addField('plistatus', 'name', 'status');
      $select->addField('maps', 'id', 'mapping_id');
      $select->addField('maps', 'sequence', 'sequence_ordering');
      $select->addField('maps', 'course_id', 'course_id');
      $select->addField('maps', 'module_id', 'module_id');
      $select->addField('maps', 'is_required', 'is_required');
      $select->addField('modules', 'title', 'groupname');
      //$select->addExpression('group_concat(DISTINCT(td.tagname))', 'tagname');
      $select->addExpression('0', 'is_re_certify');
      $select->condition('mdl.program_id', $programid, '=');
      $select->condition('mdl.id', $module_id, '=');
      //$select->where('grp.id != maps.group_id');
      expDebug::dPrintDBAPI(' $selectStmt obj =' , $select);
      $select->orderby('group_seq');
      $select->orderby('sequence_ordering');
  	  if(($start > 0) || ($end > 0)) {
          $select->range($start,$end);
        }

	  //search type value 
      if(!empty($courseName)&& $courseName!=t('LBL088').' '.t('LBL083') && $courseSearchType =='crstit'){
     // 	$select->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like');
        $select->condition(db_or()->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like')
  	                              ->condition('td.tagname', '%' . db_like($courseName) . '%', 'like'));
      }
      if(!empty($courseName)&& $courseName!=t('LBL036').' '.t('LBL096') && $courseSearchType =='crscode'){
        $select->condition(db_or()->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like')
  	                             ->condition('tmplt.code', '%' . db_like($courseName) . '%' ,'like')
        		 				 ->condition('td.tagname', '%' . db_like($courseName) . '%', 'like')
        		 				 );
      }
      if(!empty($courseName)&& $courseName!=t('LBL036'). ' '.t('Group') && $courseSearchType=='grpnametype' ){
       $select->condition('grps.title', '%' . db_like($courseName) . '%', 'LIKE');
      }
     
      $select1 = db_select('slt_course_template', 'tmplt');

      //select->leftJoin('slt_profile_list_items', 'plitype', 'tmplt.object_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_profile_list_items', 'plistatus', 'tmplt.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_profile_list_items', 'plilang', 'tmplt.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_re_certify', 'recertify', 'recertify.course_id = tmplt.id');
      $select1->leftJoin('slt_module', 'modules', 'modules.id = recertify.module_id');
      $select1->leftJoin('slt_tag_entity','te','(tmplt.id=te.entity_id and te.entity_type=\'Course\')');
      $select1->leftJoin('slt_tagdefn','td','te.tagid= td.id');
      $select1->distinct();
      $select1->addField('tmplt', 'code', 'code');
      $select1->addField('tmplt', 'title', 'title');
      $select1->addField('plistatus', 'name', 'status');
      $select1->addField('recertify', 'id', 'mapping_id');
      $select1->addField('recertify', 'sequence', 'sequence_ordering');
      $select1->addField('recertify', 'course_id', 'course_id');
      $select1->addField('recertify', 'module_id', 'module_id');
      $select1->addField('recertify', 'is_required', 'is_required');
      $select1->addField('modules', 'title', 'groupname');
    //  $select1->addExpression('group_concat(DISTINCT(td.tagname))', 'tagname');
      $select1->addExpression('1', 'is_re_certify');
      //$select->condition('tmplt.id', $courseid, '=');
      $select1->condition('recertify.program_id', $programid, '=');

      if(!empty($courseName)&& $courseName!=t('LBL088').' '.t('LBL083') && $courseSearchType =='crstit'){
   //   $select1->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like');
      $select1->condition(db_or()->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like')
  	                             ->condition('td.tagname', '%' . db_like($courseName) . '%', 'like'));
      }
      if(!empty($courseName)&& $courseName!=t('LBL036').' '.t('LBL096') && $courseSearchType =='crscode'){
      	$select->condition(db_or()->condition('tmplt.title', '%' . db_like($courseName) . '%' ,'like')
  	                             ->condition('tmplt.code', '%' . db_like($courseName) . '%' ,'like')
        		 				 ->condition('td.tagname', '%' . db_like($courseName) . '%', 'like')
        		 				);
      }
      if(!empty($courseName)&& $courseName!=t('LBL036'). ' '.t('Group') && $courseSearchType=='grpnametype' ){
       $select->condition('grps.title', '%' . db_like($courseName) . '%', 'LIKE');
      }

      $result = $select->execute()->fetchAll();
      expDebug::dPrintDBAPI(' $select SQL getAttachedTpCourses = ' , $select);
      expDebug::dPrint(' $result getAttachedTpCourses = ' . print_r($result, true) ,3);
      return $result;
  } catch (Exception $ex) {
    watchdog_exception('getAttachedTpCourses', $ex);
    expertusErrorThrow($ex);
  }

}



function getAttachedTpCourses_OLD($programid){
  try {
      $select = db_select('slt_course_template', 'tmplt');

      //select->leftJoin('slt_profile_list_items', 'plitype', 'tmplt.object_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plistatus', 'tmplt.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plilang', 'tmplt.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_module_crs_mapping', 'maps', 'maps.course_id = tmplt.id');
      $select->leftJoin('slt_module', 'modules', 'modules.id = maps.module_id');

      $select->addField('tmplt', 'title', 'title');
      $select->addField('tmplt', 'code', 'code');
      $select->addField('plistatus', 'name', 'status');
      $select->addField('maps', 'id', 'mapping_id');
      $select->addField('maps', 'sequence', 'sequence_ordering');
      $select->addField('maps', 'course_id', 'course_id');
      $select->addField('maps', 'module_id', 'module_id');
      $select->addField('maps', 'is_required', 'is_required');
      $select->addField('modules', 'title', 'groupname');
      $select->addExpression('0', 'is_re_certify');

      //$select->condition('tmplt.id', $courseid, '=');
      $select->condition('maps.program_id', $programid, '=');

      $select1 = db_select('slt_course_template', 'tmplt');

      //select->leftJoin('slt_profile_list_items', 'plitype', 'tmplt.object_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_profile_list_items', 'plistatus', 'tmplt.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_profile_list_items', 'plilang', 'tmplt.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select1->leftJoin('slt_re_certify', 'recertify', 'recertify.course_id = tmplt.id');
      $select1->leftJoin('slt_module', 'modules', 'modules.id = recertify.module_id');

      $select1->addField('tmplt', 'title', 'title');
      $select1->addField('tmplt', 'code', 'code');
      $select1->addField('plistatus', 'name', 'status');
      $select1->addField('recertify', 'id', 'mapping_id');
      $select1->addField('recertify', 'sequence', 'sequence_ordering');
      $select1->addField('recertify', 'course_id', 'course_id');
      $select1->addField('recertify', 'module_id', 'module_id');
      $select1->addField('recertify', 'is_required', 'is_required');
      $select1->addField('modules', 'title', 'groupname');
      $select1->addExpression('1', 'is_re_certify');
      //$select->condition('tmplt.id', $courseid, '=');
      $select1->condition('recertify.program_id', $programid, '=');
      $select1->orderby('sequence_ordering');
      $select->union($select1,'UNION ALL');
      //expDebug::dPrintDBAPI(' $select SQL = ' , $select);

      $result = $select->execute()->fetchAll();
      expDebug::dPrintDBAPI(' $select SQL = ' , $select);
      expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
      return $result;
      /*
      $select = db_select('slt_course_template', 'tmplt');
      $select->leftJoin('slt_profile_list_items', 'plistatus', 'tmplt.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_profile_list_items', 'plilang', 'tmplt.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
      $select->leftJoin('slt_module_crs_mapping', 'maps', 'maps.course_id = tmplt.id');
      //$select->leftJoin('slt_module', 'modules', 'modules.id = maps.module_id');

      $select->addField('tmplt', 'title', 'title');
      $select->addField('tmplt', 'code', 'code');
      $select->addField('plistatus', 'name', 'status');
      $select->addField('maps', 'course_id', 'course_id');
      $select->addField('maps', 'module_id', 'module_id');
    // $select->addField('modules', 'title', 'groupname');

      $select->condition('maps.program_id', $programid, '=');
       expDebug::dPrint('exp_sp_administration_program.inc : exp_sp_administration_program.inc() : $BALAJIIIIII = ' . print_r($select, true));
      $result = $select->execute()->fetchAll();
      return $result;*/
  } catch (Exception $ex) {
    watchdog_exception('getAttachedTpCourses_OLD', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getGroupDetails - get the group name for a particular training program
 */
function getGroupDetails($programid,$moduleid,$courseid){
  try {
    $select = db_select('slt_module_crs_mapping', 'groupsname');
    $select->leftJoin('slt_module', 'gpname', 'gpname.id = groupsname.module_id');
    $select->addField('groupsname', 'sequence', 'sequence');
    $select->addField('groupsname', 'is_required', 'is_required');
    $select->addField('gpname', 'title', 'groupname');

    $select->condition(db_and()
                            -> condition('groupsname.program_id', $programid , '=')
                            ->condition('groupsname.module_id',$moduleid, '=')
                            ->condition('groupsname.course_id', $courseid , '=')
                      );

    $result = $select->execute()->fetchAll();
    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    return $result;
  } catch (Exception $ex) {
    watchdog_exception('getGroupDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTrainingMappingDetails - Fetches specified details ($fields) of an program from slt_module_crs_mapping
 */
function getTrainingProgramGroupDetails($programId, $courseId, $fields) {
  try {
      if (empty($fields)) {
        // Get all fields
        $fields = array('id', 'program_id', 'course_id', 'module_id', 'sequence', 'is_required', 'created_by', 'created_on', 'updated_by', 'updated_on',
    					'custom0', 'custom1', 'custom2', 'custom3', 'custom4',);
      }
      // Select table
      $selectStmt = db_select('slt_module_crs_mapping', 'grp');
      // Set conditions
      $selectStmt->condition('grp.program_id', $programId);
      $selectStmt->condition('grp.course_id', $courseId);
      // Select fields and/or add expressions
      foreach($fields as $fieldName) {
        $selectStmt->addField('grp', $fieldName, $fieldName);
      }
      $result = $selectStmt->execute();
      expDebug::dPrint(' $selectStmt = ' . print_r($result, true) , 3);
      return $result->fetchAssoc();
  } catch (Exception $ex) {
    watchdog_exception('getTrainingProgramGroupDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTrainingProgramReCertifyGroupDetails - Fetches specified details ($fields) of an program from slt_module_crs_mapping
 */
function getTrainingProgramReCertifyGroupDetails($programId, $courseId, $fields) {
  try {
      if (empty($fields)) {
        // Get all fields
        $fields = array('id', 'program_id', 'course_id', 'module_id', 'sequence', 'is_required', 'created_by', 'created_on', 'updated_by', 'updated_on',
    					'custom0', 'custom1', 'custom2', 'custom3', 'custom4',);
      }
      // Select table
      $selectStmt = db_select('slt_re_certify', 'grp');
      // Set conditions
      $selectStmt->condition('grp.program_id', $programId);
      $selectStmt->condition('grp.course_id', $courseId);
      // Select fields and/or add expressions
      foreach($fields as $fieldName) {
        $selectStmt->addField('grp', $fieldName, $fieldName);
      }
      $result = $selectStmt->execute();
      expDebug::dPrint(' getTrainingProgramReCertifyGroupDetails = ' . print_r($result, true) , 3);
      return $result->fetchAssoc();
  } catch (Exception $ex) {
    watchdog_exception('getTrainingProgramReCertifyGroupDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTrainingProgramModuleDetails() - Fetches specified details ($fields) of an program from slt_module
 */
function getTrainingProgramModuleDetails($moduleid, $fields) {
  try {
      if (empty($fields)) {
        // Get all fields
        $fields = array('id', 'title', 'created_by', 'created_on', 'updated_by', 'updated_on','sequence',
    					'custom0', 'custom1', 'custom2', 'custom3', 'custom4',);
      }
      // Select table
      $selectStmt = db_select('slt_module', 'modls');
      $selectStmt->condition('modls.id', $moduleid);
      // Select fields and/or add expressions
      foreach($fields as $fieldName) {
        $selectStmt->addField('modls', $fieldName, $fieldName);
      }
      expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
      // Execute query
      $result = $selectStmt->execute();
      return $result->fetchAssoc();
  } catch (Exception $ex) {
    watchdog_exception('getTrainingProgramModuleDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * groupSequenceUpdate() - Update the sequemce details for a particular Traininig program
 */
function groupSequenceUpdate($form, &$form_state, $userid){

  expDebug::dPrint(' $selectStmt = ' . print_r($form_state['values']['module_id'].'AA'.$form_state['values']['course_id'], true) , 5);
  expDebug::dPrint(' recertify id hid  = ' . print_r($form_state['values'], true) ,5);
  $txn = db_transaction();
  try {
  $programId  = $form_state['values']['program_id'];
  $courseId   = $form_state['values']['course_id'];
  $reCertifyFlag   = $form_state['re_certify_flag'];

  $uniqueId = $programId.'-'.$courseId.'-'.$reCertifyFlag;
  $moduleDet= db_query("SELECT title,sequence from slt_module where id=:mid",array(":mid"=> $form_state['values']['module_id']))->fetchAll();
  $updateStmt = db_update('slt_module');
  $attachgroupname= empty($form_state['values']['attachgroupname_'.$uniqueId]) ? null : $form_state['values']['attachgroupname_'.$uniqueId];
  $updateStmt->condition('id', $form_state['values']['module_id']);
  $fields = array(
              'title' => $form_state['values']['attachgroupname_'.$uniqueId],
              'sequence' => empty($form_state['values']['groupsequence']) ? null : $form_state['values']['groupsequence_'.$uniqueId],
              'updated_on' => now(),
              'updated_by' => $userid,
              'end_date' =>  $end_date,
              );


  // Select the fields and the values to be updated
  $updateStmt->fields($fields);
  expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
  // Execute query
  $numUpdated = $updateStmt->execute();

  make_audit_trail_entries($fields, getIdOfLoggedInUser() , $moduleDet ,$form_state['values']['module_id'], 'Updated','Module','exp_sp_administration_program','old module',
                                          array('title', 'sequence'));
  // Update for Sequence


  if($form_state['values']['re_certify_flag']==1 && $object_type=='cre_sys_obt_crt'){
  	$default= db_query("SELECT is_required,sequence from slt_re_certify where module_id=:mid and course_id=:cid",array(":mid"=> $form_state['values']['module_id'],":cid"=> $form_state['values']['course_id']))->fetchAll();
  	$updateSequence=db_update('slt_re_certify');
	  $updateSequence->condition(db_and()
	                        ->condition('module_id',$form_state['values']['module_id'], '=')
	                        ->condition('course_id', $form_state['values']['course_id'] , '=')
	                  );

	  $sequenceno= empty($form_state['values']['sequenceno_'.$uniqueId]) ? null : $form_state['values']['sequenceno_'.$uniqueId];
	  $sequencefields = array(
	              'is_required' => ($form_state['values']['is_required_'.$uniqueId] == 0)? 'N' : 'Y',
	              'sequence' => $sequenceno,
	              'updated_on' => now(),
	              'updated_by' => $userid,
	            );

	  // Select the fields and the values to be updated
	  $updateSequence->fields($sequencefields);
	  $seqUpdated = $updateSequence->execute();

      make_audit_trail_entries($sequencefields, getIdOfLoggedInUser() , $default ,$programId,
      							 'Updated for module id '.$form_state['values']['module_id'].' and course id '.$form_state['values']['course_id'],
      							$object_type,'exp_sp_administration_program','old course recertify',
                                          array('is_required', 'sequence'));
  }else{

  	$default= db_query("SELECT is_required,sequence from slt_module_crs_mapping where module_id=:mid and course_id=:cid",array(":mid"=> $form_state['values']['module_id'],":cid"=> $form_state['values']['course_id']))->fetchAll();

  	$updateSequence=db_update('slt_module_crs_mapping');
	  $updateSequence->condition(db_and()
	                        ->condition('module_id',$form_state['values']['module_id'], '=')
	                        ->condition('course_id', $form_state['values']['course_id'] , '=')
	                  );

	  $sequenceno= empty($form_state['values']['sequenceno_'.$uniqueId]) ? null : $form_state['values']['sequenceno_'.$uniqueId];
	  $sequencefields = array(
	              'is_required' => ($form_state['values']['is_required_'.$uniqueId] == 0)? 'N' : 'Y',
	              'sequence' => $sequenceno,
	              'updated_on' => now(),
	              'updated_by' => $userid,
	            );

	  // Select the fields and the values to be updated
	  $updateSequence->fields($sequencefields);
	  $seqUpdated = $updateSequence->execute();


      make_audit_trail_entries($sequencefields, getIdOfLoggedInUser() , $default ,$programId,
      							 'Updated for module id '.$form_state['values']['module_id'].' and course id '.$form_state['values']['course_id'],
      							$object_type,'exp_sp_administration_program','old course',
                                          array('is_required', 'sequence'));
  }

  expDebug::dPrintDBAPI(' $$seqUpdated obj = ' , $updateSequence);

  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('groupSequenceUpdate', $ex);
    expertusErrorThrow($ex);
  //throw $ex;
  // return array((object)array('id'=>"failure"));

  }
  unset($txn);
 }

/*
 * checkLearningProgramTypeString() - Getting the object type string
 */
function checkLearningProgramTypeString($objectType){
  try {
      // checking for curricula, certificate or leaning program
      if($objectType=='cre_sys_obt_crt') {
        $object_type='Certification';
      }
      elseif($objectType=='cre_sys_obt_trn'){
        $object_type='Learning Plan';
      }
      else{
        $object_type='Curricula';
      }
      return $object_type;
  } catch (Exception $ex) {
    watchdog_exception('checkLearningProgramTypeString', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * deleteTpAttachedCourse() - Delete the attached courses for the Training program
 */
function deleteTpAttachedCourse($programId, $courseId,$moduleId,$grpId){
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
  $txn = db_transaction();
  try {
     
    if(!empty($programId) && !empty($courseId) ){
        if($form_state['values']['create_user_id'])
            $loggedInUserId = $form_state['values']['create_user_id'];
        else
            $loggedInUserId  = getIdOfLoggedInUser();
         
        $delete_id = db_select('slt_module_crs_mapping', 'mcm');
        $delete_id->addField('mcm', 'id', 'id');
        $delete_id->addField('mcm','group_id','group_id');
        $delete_id->condition('program_id', $programId,'=');
        $delete_id->condition('course_id', $courseId,'=');
        $delete_id->condition('module_id', $moduleId,'=');
        expDebug::dPrintDBAPI("delete oject in attached course132154235" ,$delete_id);
        $del=$delete_id->execute()->fetchAll();
        expDebug::dPrint('$delete_id'.print_r($del,true),1);
      
      $deleteStmt = db_delete('slt_module_crs_mapping');
      $deleteStmt->condition('program_id', $programId,'=');
      $deleteStmt->condition('course_id', $courseId,'=');
      $deleteStmt->condition('module_id', $moduleId,'=');
     // $deleteStmt->condition('group_code', $grpCode,'=');
      expDebug::dPrintDBAPI("delete oject in attached course" ,$deleteStmt);
      $numUpdated = $deleteStmt->execute();

      //Make delete log entries
      foreach ($del as $val){
          $fields = array(
                  'table_name' => 'slt_module_crs_mapping',
                  'entity_id' => $val->id,
                  'entity_type' => 'module_course_mapping',
                  'module_name' => 'exp_sp_administration_program',
                  'functionality_name' => 'deleteTpAttachedCourse',
                  'api_name' => null,
                  'deleted_on' => now(),
                  'deleted_by' => $loggedInUserId,
                  'parent1_entity_id' => $programId,
                  'parent1_entity_type' => 'program_id',
                  'parent2_entity_id' => $moduleId,
                  'parent2_entity_type' => 'module_id',
                  'parent3_entity_id' => $courseId,
                  'parent3_entity_type' => 'course_id',
                  'parent4_entity_id' => $val->group_id,
                  'parent4_entity_type' => 'group_id',
          );
          deleted_log_entry($fields);
      }expDebug::dPrint('slt_module_crs_mapping table log');

      // Resequence attached courses and their groups
      expDebug::dPrint(' Re-sequencing attached courses after delete');
      //$sequenceOrder = getTPAttachedCoursesSequenceOrder($programId,$moduleId,$grpId);
     //$usedModuleIds = sequenceAttachCourse($sequenceOrder);

      // Delete any group that is now unused
     // deleteUnusedTPGroups($programId, $usedModuleIds);
      updateTPStatusOnDelete($programId);
      $deleteFormState = array();
      $deleteFormState['id'] = $programId;
      $deletedEntity = "deleteCourseDetailsList";
      make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$courseId, 'Deleted', 'cre_sys_obt_cls', 'exp_sp_administration_program',$deletedEntity, array('id'));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('deleteTpAttachedCourse', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }
  unset($txn);
}


function deleteTPAttachedGroups($programId,$moduleId,$grpId ){
      if($form_state['values']['create_user_id'])
         $loggedInUserId = $form_state['values']['create_user_id'];
      else
        $loggedInUserId  = getIdOfLoggedInUser();
    
	  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
	  expDebug::dPrint("delete tp attached groups ");
	   
	  $deleteStmt = db_delete('slt_module_groups');
      $deleteStmt->condition('program_id', $programId,'=');
      $deleteStmt->condition('id', $grpId,'=');
      $deleteStmt->condition('module_id', $moduleId,'=');
     // $deleteStmt->condition('group_code', $grpCode,'=');
      expDebug::dPrintDBAPI("delete oject in deleteTPAttachedGroups" ,$deleteStmt);
      $numUpdated = $deleteStmt->execute();
      $deleteStmt = db_delete('slt_module_crs_mapping');
      $deleteStmt->condition('program_id', $programId,'=');
      $deleteStmt->condition('group_id', $grpId,'=');
      $deleteStmt->condition('module_id', $moduleId,'=');
      expDebug::dPrintDBAPI("delete oject in attached course" ,$deleteStmt);
      $numUpdated = $deleteStmt->execute();
      //make delete log entries - change
      $fields = array(
              'table_name' => 'slt_module_groups', 
              'entity_id' => $grpId,
              'entity_type' => 'module_groups',
              'module_name' => 'exp_sp_administration_program',
              'functionality_name' => 'deleteTPAttachedGroups',
              'api_name' => null,
              'deleted_on' => now(),
              'deleted_by' => $loggedInUserId,
              'parent1_entity_id' =>$programId,
              'parent1_entity_type' => 'program_id',
              'parent2_entity_id' => $moduleId,
              'parent2_entity_type' => 'module_id'
      );
      deleted_log_entry($fields);
      expDebug::dPrint('slt_module_groups table log - 2');
      
      //Re-order sequence
	  $selectQry1 = db_select('slt_module_groups', 'modcrsmap');
      $selectQry1->addField('modcrsmap', 'sequence', 'sequence');
      $selectQry1->addField('modcrsmap', 'id', 'id');
      $selectQry1->condition('modcrsmap.program_id', $programId);
      $selectQry1->condition('modcrsmap.module_id', $moduleId);
      $selectQry1->condition('modcrsmap.sequence', 9999, '!=');

      $seqOrderRecList = $selectQry1->execute()->fetchAll();
      expDebug::dPrintDBAPI('get sequence of attached groups to TP' , $selectQry1);
      expDebug::dPrint(' $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
      usort($seqOrderRecList, "tpAttachedCoursesSeqOrderRecCompare");
      expDebug::dPrint(' after usort, $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
      $sequenceOrder = '';
      $first = true;
    
     foreach ($seqOrderRecList as $seqOrderRec) {
      if ($first) {
        $sequenceOrder .= $seqOrderRec->id;
        $first = false;
       }
      else {
        $sequenceOrder .= ',' . $seqOrderRec->id;
       }
     }
     
     expDebug::dPrint("sequence order >>> ". $sequenceOrder ,5);
     $seqArry = explode(',',$sequenceOrder);
     expDebug::dPrint("sequence order aray >> ".print_r($seqArry,true),5);
     $grpseq = 1;
     foreach($seqArry as $seq){
    	$update = db_update("slt_module_groups");
		$update->fields(array('sequence'=>$grpseq));
		$update->condition('id', $seq);
		expDebug::dPrintDBAPI('update module grps sequence order ' , $update);
		$update->execute();
		$grpseq++;
    }
}

/*
 * deleteTpAttachedReCertifyCourse() - Delete the attached courses for the Training program
 */
/*function deleteTpAttachedReCertifyCourse($programId, $courseId){
	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
  $txn = db_transaction();
  try {
    if (!empty($programId) && !empty($courseId) ) {
      $deleteStmt = db_delete('slt_re_certify');
      $deleteStmt->condition('program_id', $programId,'=');
      $deleteStmt->condition('course_id', $courseId,'=');
      $numUpdated = $deleteStmt->execute();

      // Resequence attached courses and their groups
      expDebug::dPrint(' Re-sequencing attached courses after delete',4);
      $sequenceOrder = getTPAttachedCoursesSequenceOrder($programId);
      $usedModuleIds = sequenceAttachCourse($sequenceOrder);

      // Delete any group that is now unused
      deleteUnusedTPGroups($programId, $usedModuleIds);
      updateTPStatusOnDelete($programId);
      $deleteFormreState = array();
      $deleteFormreState['id'] = $programId;
      $deletedreEntity = "deleterecertifycrs";
      make_audit_trail_entries($deleteFormreState, getIdOfLoggedInUser() , null ,$courseId, 'Deleted', 'cre_sys_obt_cls', 'exp_sp_administration_program',
            	                                 $deletedreEntity, array('id'));
    }
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('deleteTpAttachedReCertifyCourse', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }
  unset($txn);
}*/
/*
 * updateTPStatusOnDelete() -  Use this function to update the TP status
*/
function updateTPStatusOnDelete($programId){
	try{
		$select = db_select('slt_module_crs_mapping', 'courses');
		$select->addExpression('COUNT(1)','total_courses');
		$select->condition('courses.program_id', $programId, '=');
		$cntMapping = $select->execute()->fetchField();
		// Commented For the Certiffication all main Course are Deleted Issue #0039876
		/*  $select = db_select('slt_re_certify', 'rec');
		$select->addExpression('COUNT(1)','total_courses');
		$select->condition('rec.program_id', $programId, '=');
		$cntRecertify = $select->execute()->fetchField(); */

		//  $attachCount = $cntMapping + 	$cntRecertify;
		$attachCount = $cntMapping;
		if($attachCount == 0){
			$selectprogramStatus = db_select('slt_program', 'program');
			$selectprogramStatus->addField('program','status');
			$selectprogramStatus->condition('program.id',$programId);
			expDebug::dPrintDBAPI(' $select obj = ' , $selectprogramStatus);
			$Statusprogram = $selectprogramStatus->execute()->fetchField();

			if($Statusprogram == 'lrn_lpn_sts_atv'){
				$updatedUserId = getSltpersonUserId();
				$updatestmt = db_update('slt_program');
				$updatestmt->fields(array('status' => 'lrn_lpn_sts_itv',
						'updated_on' => now(),
						'updated_by' => $updatedUserId
				));
				$updatestmt->condition('id', $programId, '=');
				$result = $updatestmt->execute();
			}
		}else{
			//Don't delete this update this is report flat table related update.
			$updatedUserId = getSltpersonUserId();
			$updatestmt = db_update('slt_program');
			$updatestmt->fields(array(
					'updated_on' => now(),
					'updated_by' => $updatedUserId
			));
			$updatestmt->condition('id', $programId, '=');
			$result = $updatestmt->execute();
		}
	} catch (Exception $ex) {
		watchdog_exception('updateTPStatusOnDelete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * addTwoColumnTextFieldToAddEditForm() -  Use this function to add a text field to the addedit form
 */
function addTwoColumnTextFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 60) {
  try {
      // Prepare the additional HTML attributes for the textfield
      $attributes = array();
      $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $attributes['class'] = array('addedit-edit-' . $id);

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

      // Create the form element
      $form[$id] = array(
        '#type' => 'textfield',
        '#prefix' => addEditGetPrefix('DOUBLEFIELD', $title, $mandatory),
         '#suffix' => addEditGetSuffix('DOUBLEFIELDCLOSE'),
        '#title' => $title,
        '#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#disabled' => $readOnly,
        '#attributes' => $attributes,
        '#size' => $viewWidthInChars,
        '#maxlength' => '28',
      );
  } catch (Exception $ex) {
    watchdog_exception('addTwoColumnTextFieldToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * addTwoColumnDateFieldToAddEditForm() -  Use this function to add a date field to the addedit form
 */
function addTwoColumnDateFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $description = false, $mandatory = false, $readOnly = false, $size = 19) {
  try {
      // Prepare the additional HTML attributes for the textfield
      $attributes = array();
      $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $attributes['class'] = array('addedit-edit-datepicker-' . $id);
      $attributes['class'][] = 'addedit-datepicker';

      if ($readOnly) {
        $attributes['class'][] = 'addedit-readonly-textfield';

      }
      if (isset($tabIndex) && $tabIndex > 0) {
        $attributes['tabindex'] = $tabIndex;
      }
      // Create the form element
      $form[$id] = array(
        '#type' => 'textfield',
        //'#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
       // '#suffix' => addEditGetSuffix('FIELD'),
        '#prefix' => addEditGetPrefix('STATUSFIELD', $title, $mandatory),
        '#suffix' => addEditGetSuffix('DOUBLEFIELD'),
        '#title' => $title,
        '#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#disabled' => $readOnly,
        '#attributes' => $attributes,
        '#size' => $size,
        '#description'=> $description

      );
   } catch (Exception $ex) {
    watchdog_exception('addTwoColumnDateFieldToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addDropDownTwoColumnToAddEditForm - Use this function to add a dropdown (select) to the addedit form
 */
function addDropDownTwoColumnToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false, $prefixname='DOUBLEDROPDOWN', $suffixname='DOUBLEFIELD') {
  try {
      $attributes = array();
      $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $attributes['class'] = array('addedit-edit-' . $id);

      if ($readOnly) {
        $attributes['class'][] = 'addedit-readonly-dropdown';
      }

      if (isset($tabIndex) && $tabIndex > 0) {
        $attributes['tabindex'] = $tabIndex;
      }

      if ($mandatory) {
        $nullOption[''] = 'Select';
      } else {
        $nullOption[''] = 'None Selected';
      }
      $finalOptions = array_merge($nullOption, $options);

      $form[$id] = array(
        '#type' => 'select',
        '#prefix' => addEditGetPrefix($prefixname, $title, $mandatory),
        '#suffix' => addEditGetSuffix($suffixname),
        '#title' => $title,
        '#title_display' => 'invisible',
        '#options'=> $finalOptions,
        '#default_value' => $defaultOption,
        '#disabled' => $readOnly,
        '#attributes' => $attributes
      );
  } catch (Exception $ex) {
    watchdog_exception('addDropDownTwoColumnToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTotalTPEnroll - Use this function to get a total enrollment
 */
function getTotalTPEnroll($prgId){
  try {
      $select = db_select('slt_master_enrollment', 'master_enroll');
      $select->addExpression('COUNT(1)','total_enroll');
      $select->condition('master_enroll.program_id', $prgId, '=');
      $select->condition('master_enroll.overall_status',array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl'), 'IN');
      $total_enroll = $select->execute()->fetchField();
      return $total_enroll;
  } catch (Exception $ex) {
    watchdog_exception('getTotalTPEnroll', $ex);
    expertusErrorThrow($ex);
  }
}
function checkBackgroundJobs($prgId){
	try {
		$select = db_select('slt_temp_background_jobs', 'temp');
		$select->addExpression('COUNT(1)','cnt');
		$select->condition('temp.program_id', $prgId, '=');
		$select->condition('temp.status',array('inprogress'), 'IN');
		$select->condition('temp.job_name','AR-%', 'LIKE');
		expDebug::dPrintDBAPI("CHECK INPROGRESS REC COUNT",$select);
		$recCnt = $select->execute()->fetchField();
		return $recCnt;
	}catch (Exception $ex) {
		watchdog_exception('checkBackgroundJobs', $ex);
		expertusErrorThrow($ex);
	}
}
function addNewTPByRestAPI( &$form_state,$createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once(drupal_get_path('module', 'exp_sp_administration_banner') .'/exp_sp_administration_banner.module');
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		expDebug::dPrint('Formstate values : '.print_r($form_state['values'] , true), 4);
		$error = new stdClass();
		$addinfo = $form_state['values']['additional_info'];
	    $addcatinfo = $form_state['values']['addn_catalog_show'];
	    $addnotinfo = $form_state['values']['addn_notification_show'];
	       if(strlen($addinfo) > 500) {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';						
				$errobj->errormsg = "Enter maximum of 500 characters in additional information";
				return $errobj;
			}
           $resultaddn_catalog_show = array(0,1);
           if ((!in_array(trimInput($addcatinfo), $resultaddn_catalog_show)) || ((!empty($addcatinfo)) && !is_numeric($addcatinfo)))
           {
           $errobj=new stdClass();
           $errobj->isValidateError = 1;
           $errobj->errcode = 'L_012';
           $errobj->errormsg = "Enter only 0 or 1 for addn_catalog_show";
            return $errobj;
             }
		   $resultaddn_notification_show = array(0,1);
           if ((!in_array(trimInput($addnotinfo), $resultaddn_notification_show) || ((!empty($addnotinfo)) && !is_numeric($addnotinfo))))
           {
           $errobj=new stdClass();
           $errobj->isValidateError = 1;
           $errobj->errcode = 'L_012';
           $errobj->errormsg = "Enter only 0 or 1 for addn_notification_show";
            return $errobj;
             }
		   
		$statusTypes = array('lrn_lpn_sts_itv');
		if(!in_array(strtolower($form_state['values']['status']), $statusTypes)){ // Status validation
			$error -> isValidateError = true;
			$error -> errormsg = t('Please Enter the correct Status');
			return $error;
		}
		$trpTypes = array('cre_sys_obt_trn','cre_sys_obt_crt','cre_sys_obt_cur');
		if(!in_array(strtolower($form_state['values']['object_type'] ), $trpTypes)){ // Object Type Validation
			$error -> isValidateError = true;
			$error -> errormsg = t('object Type incorrect.');
			return $error;
		}
		if($form_state['values']['object_type'] == 'cre_sys_obt_trn'){
			if (!empty($form_state['values']['end_date']) && !isValidDate($form_state['values']['end_date'])) { // days validate
				$error -> isValidateError = true;
				$error -> errormsg =t('LBL234').' '.t('ERR243');
				return $error;
			}
		}
		if($form_state['values']['object_type'] == 'cre_sys_obt_crt'){
			$cerExpInTypes = array('years','months','days');
			if(!in_array(strtolower(trim($form_state['values']['expires_in_unit'])) , $cerExpInTypes)){ // Only this types are Allowed
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Certification Type incorrect.";
				return $errobj;
			}
		}
		
		$price = $form_state['values']['price'];
    $currency_type = $form_state['values']['currency_type'];
		
		if($currency_type){
			$short_code = currency_isValidOrNot($currency_type);
 			if(empty($short_code)){
 				$errobj=new stdClass();
 				$errobj->isValidateError = 1;
 				$errobj->errcode = 'L_012';
 				$errobj->field = 'currency_type';
 				$errobj->errormsg = "The given Currency type is invalid or inactive status";
 				return $errobj;
 			}
		}
		if($price > 0 && is_numeric($price))
	    {
	   		if(empty($currency_type)){
	        $error->isValidateError = 1;
					$error->errcode = 'L_012';
					$error->field = 'currency_type';
					$error->errormsg = "Currency type should not empty";
					return $error;
	      }
	    }else{
	    	if(empty($currency_type))
					$currency_type = "USD";
	    }
		$form_state['values']['currency_type'] = ($short_code) ? $short_code : $currency_type;
		
		if(strtolower($form_state['values']['status']) == 'lrn_lpn_sts_atv'){
			$form_state['triggering_element']['#name'] = 'saveandpublish';
		}else if(strtolower($form_state['values']['status']) == 'lrn_lpn_sts_itv'){
			$form_state['triggering_element']['#name'] = 'save';
		}else{
			$form_state['triggering_element']['#name'] = 'delete';
		}
        
        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
            exp_sp_administration_program_addedit_form_validate($form, $form_state,1);//#custom_attribute_0078975
        }else{
		exp_sp_administration_program_addedit_form_validate($form, $form_state);
        } //#custom_attribute_0078975 - End Check module status
        
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		$tpId =  addNewProgram($form,$form_state,$createuserid,1);
		expDebug::dPrint(' addNewTPByRestAPI created tp:' . print_r($tpId,true) ,4);
		$operation='insert';
		$errobj=new stdClass();
		/*if($tpId[0]->id == 'currecny_null')
		{
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->field = 'currency_type';
			$errobj->errormsg = "Currency type should not empty";
			return $errobj;
		}
		if($tpId[0]->id == 'invalid_currecny')
		{
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->field = 'currency_type';
			$errobj->errormsg = "The given Currency type is invalid or inactive status";
			return $errobj;
		}*/
		if($tpId[0]->id && $tpId[0]->id != 'Failiure'){
			$form_state['values']['id'] = $tpId[0]->id;
			adminCreateLMSProductTpNode($form, $form_state, $operation,$createuserid);
			//To sync solr
			syncSolrData('Program');
			return $tpId;
		}
		else{
			//return array((object)array('id'=>'Failure'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "The Program ID given is not valid";
			return $errobj;
		}

	} catch (Exception $ex) {
		watchdog_exception('addNewTPByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateTPByRestAPI( &$form_state,$createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once(drupal_get_path('module', 'exp_sp_administration_banner') .'/exp_sp_administration_banner.module');
		expDebug::dPrint('$form_state values = ' . print_r($form_state['values'], true) , 5);
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		/* Validation Starts Here */
		$error = new stdClass();

		if(!is_numeric($form_state['values']['id'])){ // invalid Id must be Numeric validate
			$error -> isValidateError = true;
			$error -> errormsg = 'Invalid ID';
			return $error;
		}
		$addinfo = $form_state['values']['additional_info'];
	    $addcatinfo = $form_state['values']['addn_catalog_show'];
	    $addnotinfo = $form_state['values']['addn_notification_show'];
	       if(strlen($addinfo) > 500) {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';						
				$errobj->errormsg = "Enter maximum of 500 characters in additional information";
				return $errobj;
			}
           $resultaddn_catalog_show = array(0,1);
           if ((!in_array(trimInput($addcatinfo), $resultaddn_catalog_show)) || ((!empty($addcatinfo)) && !is_numeric($addcatinfo)))
           {
           $errobj=new stdClass();
           $errobj->isValidateError = 1;
           $errobj->errcode = 'L_012';
           $errobj->errormsg = "Enter only 0 or 1 for addn_catalog_show";
            return $errobj;
             }
		   $resultaddn_notification_show = array(0,1);
           if ((!in_array(trimInput($addnotinfo), $resultaddn_notification_show) || ((!empty($addnotinfo)) && !is_numeric($addnotinfo))))
           {
           $errobj=new stdClass();
           $errobj->isValidateError = 1;
           $errobj->errcode = 'L_012';
           $errobj->errormsg = "Enter only 0 or 1 for addn_notification_show";
            return $errobj;
             }
		   
		$statusTypes = array('lrn_lpn_sts_atv','lrn_lpn_sts_itv','lrn_lpn_sts_del');
		if(!in_array(strtolower($form_state['values']['status']), $statusTypes)){ // Status validation
			$error -> isValidateError = true;
			$error -> errormsg = t('Please Enter the correct Status');
			return $error;
		}
				
		$program_valid=getIfProgramValid($form_state['values']['id']);
		if((empty($program_valid[0]->sesavailable) || $program_valid[0]->sesavailable == 0) && $form_state['values']['status'] == 'lrn_lpn_sts_atv'){
			$error -> isValidateError = true;
			$error -> errormsg =t('Invalid status.No course associated to TP.');
			return $error;
		}
		
		$trpTypes = array('cre_sys_obt_trn','cre_sys_obt_crt','cre_sys_obt_cur');
		if(!in_array(strtolower($form_state['values']['object_type'] ), $trpTypes)){ // Object Type Validation
			$error -> isValidateError = true;
			$error -> errormsg = t('object Type incorrect.');
			return $error;
		}
		if (!empty($form_state['values']['end_date']) && !isValidDate($form_state['values']['end_date'])) { // days validate
			$error -> isValidateError = true;
			$error -> errormsg ='Complete By Invalid date.';
			return $error;
		}
		if(strlen(trim($form_state['values']['price'])) > 10){
			$error -> isValidateError = true;
			$error -> errormsg ='Price must be Minimum 10 Character Length';
			return $error;
		}
		$price=($form_state['values']['price']!=null && $form_state['values']['price']!='')?$form_state['values']['price']:0;
		$currency_type = $form_state['values']['currency_type'];
		
		if($currency_type){
			$short_code = currency_isValidOrNot($currency_type);
 			if(empty($short_code)){
 				$errobj=new stdClass();
 				$errobj->isValidateError = 1;
 				$errobj->errcode = 'L_012';
 				$errobj->errormsg = "Currency Type is not Valid";
 				return $errobj;
 			}
		}
		if($price > 0 && is_numeric($price))
    {
   		if(empty($currency_type)){
        $error->isValidateError = 1;
				$error->errcode = 'L_012';
				$error->field = 'currency_type';
				$error->errormsg = "Currency type should not empty";
				return $error;
      }
    }
    else{
	    	if(empty($currency_type))
					$currency_type = "USD";
	    }
		$form_state['values']['currency_type'] = ($short_code) ? $short_code : $currency_type;
		
		if(strtolower($form_state['values']['status']) == 'lrn_lpn_sts_atv'){
			$form_state['triggering_element']['#name'] = 'saveandpublish';
		}else if(strtolower($form_state['values']['status']) == 'lrn_lpn_sts_itv'){
			$form_state['triggering_element']['#name'] = 'save';
		}else{
			$form_state['triggering_element']['#name'] = 'delete';
 		}
        
        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
            exp_sp_administration_program_addedit_form_validate($form, $form_state,1);//#custom_attribute_0078975
        }else{
		exp_sp_administration_program_addedit_form_validate($form, $form_state);
        } //#custom_attribute_0078975 - End Check module status
        
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		 
		
		if($form_state['values']['object_type']=="cre_sys_obt_crt") {
			$expiresdays = expiredDaysCheck($form_state['values']['id'],$form_state['values']['expires_in_value'],$form_state['values']['expires_in_unit']);
			expDebug::dPrint("expiresdays = ".$expiresdays,5);
		
			if($expiresdays == 1) {
				$error -> isValidateError = true;
				$error -> errormsg = t('Already has expires_in_value for the entered Certification');
				return $error;
			}
		}
		
		
		/* Validation Ends */
		$tpId =  updateProgramDetails($form,$form_state,$createuserid,1);
		expDebug::dPrint('updateTPByRestAPI update tp:' . print_r($tpId,true) , 4);
        
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
        
        	$ret_tmp = getcustomattributemodulestatus();
        	if($ret_tmp == true){
        	//Save Custom Attributes #custom_attribute_0078975
        	include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        	saveCustomAttributesInAddEditForm($form, $form_state, $tpId[0]->id, 'cre_sys_obt_trp', $createuserid,'Updated',1);
        } //#custom_attribute_0078975 - End Check module status
        
		$operation='update';
		$errobj=new stdClass();
		/*if($tpId[0]->id == 'currecny_null')
		{
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->field = 'currency_type';
			$errobj->errormsg = "Currency type should not empty";
			return $errobj;
		}
		if($tpId[0]->id == 'invalid_currecny')
		{
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->field = 'currency_type';
			$errobj->errormsg = "The given Currency type is invalid or inactive status";
			return $errobj;
		}*/
		if($tpId[0]->id && $tpId[0]->id != 'Failiure'){
			adminCreateLMSProductTpNode($form, $form_state, $operation,$createuserid);
			//To sync solr
			syncSolrData('Program');
			return $tpId;
		}
		else{
			//return array((object)array('id'=>'Failure'));
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid Training Program ID";
		}
		return $errobj;
	} catch (Exception $ex) {
		watchdog_exception('updateTPByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function listTPByRestAPI($start, $limit, $userid="")
{
  try {
	$form=array();
	$form["form_id"]="";
	
	$default_limit = 10;
	$start = (empty($start) || ($start < 0)) ?  0 : $start;
	$limit = (empty($limit) || ($limit < 0)) ?  $default_limit : $limit;
	if(!is_numeric($start) || !is_numeric($limit)) {
		$error -> isValidateError = true;
		$error -> errormsg =t('start and limit must be in Numeric values only');
		return $error;
	}
	if ($start < 0) $start = 0;
	$textfilter = addslashes(trim($_REQUEST['textfilter'])); // #46931 - Back slash support added for the filter text.
	expDebug::dPrint('updateTPByRestAPI update tp $textfilter:' . print_r($textfilter,true) , 4);
	
  	
  	//Solr check
      $solrSource = getConfigValue('solr_search');
      if($solrSource == 0){
      	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$result = fetchProgramAll('',$start,$limit,'API',$textfilter);
	//$result = $result[0];
  	$cntResults = $result[2];
  	if($cntResults){
    	$result[0]['totalrow'] = $cntResults;
  	}
      }else{
      	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog_tp_admin.php';
      	try{
      		$solrObj = new AdminCatalogTPSolrSearch();
      		$result[0] = $solrObj->getTPList();
      	}catch(Exception $e){
      		expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
      	}
      }
	return $result[0];
  } catch (Exception $ex) {
    watchdog_exception('listTPByRestAPI', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * programEnrollmentsConflictCheck() - Conflict check before assigning course.
 * Existing course->class sessions should not have any conflicts with the new course->class sessions
 */
function _programEnrollmentsConflictCheck($programId, $courseId) {
  try {
      expDebug::dPrint('programEnrollmentsConflictCheck - Program ID : '.$programId , 4);
      expDebug::dPrint('programEnrollmentsConflictCheck - Course ID : '.$courseId , 4);

      $existingAttachedSessionDetails = _attachedCourseClassDetails($programId);
      $newSessionDetails = _associateClassesforCourses($courseId);

      expDebug::dPrint('programEnrollmentsConflictCheck - Existing Sessiondetails : '.print_r($existingAttachedSessionDetails, true) , 4);
      expDebug::dPrint('programEnrollmentsConflictCheck - New Sessiondetails : '.print_r($newSessionDetails, true) , 4);

      $countExistingSessions = count($existingAttachedSessionDetails);
      $countNewSessions = count($newSessionDetails);

      // Atleast one session should be there in existing attached course->class and new course->class getting attached to check the session conflict
      if($countExistingSessions != 0 && $countNewSessions != 0){

        for($i = 0; $i < $countExistingSessions; $i++){

          for($j = 0; $j < $countNewSessions; $j++){

            $existStartTime = strtotime($existingAttachedSessionDetails[$i]->start_session_time);
            $existEndTime   = strtotime($existingAttachedSessionDetails[$i]->end_session_time);

            $currStartTime = strtotime($newSessionDetails[$j]->start_session_time);
            $currEndTime   = strtotime($newSessionDetails[$j]->end_session_time);

            if( ($currStartTime >= $existStartTime && $currStartTime <= $existEndTime) || ($currEndTime >= $existStartTime && $currEndTime <= $existEndTime) ||
                      ($existStartTime >= $currStartTime && $existStartTime <= $currEndTime) || ($existEndTime >= $currStartTime && $existEndTime <= $currEndTime) ){
              return 1; // Conflict
            }

          }

        }

      }

      return 0; // No Conflict
  } catch (Exception $ex) {
    watchdog_exception('_programEnrollmentsConflictCheck', $ex);
    expertusErrorThrow($ex);
  }

}


/*
 * _attachedCourseClassDetails() - To get the complete session details of all the course->class associated with the program id
 */
function _attachedCourseClassDetails($programId){
  try {
    $query = db_select('slt_module_crs_mapping', 'crs_map');
    $query->leftjoin('slt_course_class', 'cls', 'crs_map.course_id = cls.course_id');
    $query->innerjoin('slt_course_class_session', 'sess', 'sess.course_id = cls.course_id and sess.course_id = crs_map.course_id');
    $query->addExpression("CONCAT(DATE_FORMAT(sess.start_date, '%Y-%m-%d '), CONCAT(sess.start_time,':00'))",'start_session_time');
    $query->addExpression("CONCAT(DATE_FORMAT(sess.start_date, '%Y-%m-%d '), CONCAT(sess.end_time,':00'))",'end_session_time');
    $query->condition('crs_map.program_id', $programId);
    $query->condition('cls.delivery_type', array('lrn_cls_dty_ilt', 'lrn_cls_dty_vcl'), 'IN');
    return $query->execute()->fetchAll();
  } catch (Exception $ex) {
    watchdog_exception('_attachedCourseClassDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * _associateClassesforCourses() - To get the complete session details of all the course->class associated with the course id
 */
function _associateClassesforCourses($courseId){
  try {
    $query = db_select('slt_course_class', 'cls');
    $query->innerjoin('slt_course_class_session', 'sess', 'sess.course_id = cls.course_id');
    $query->addExpression("CONCAT(DATE_FORMAT(sess.start_date, '%Y-%m-%d '), CONCAT(sess.start_time,':00'))",'start_session_time');
    $query->addExpression("CONCAT(DATE_FORMAT(sess.start_date, '%Y-%m-%d '), CONCAT(sess.end_time,':00'))",'end_session_time');
    $query->condition('cls.course_id', $courseId);
    $query->condition('cls.delivery_type', array('lrn_cls_dty_ilt', 'lrn_cls_dty_vcl'), 'IN');
    return $query->execute()->fetchAll();
  } catch (Exception $ex) {
    watchdog_exception('_associateClassesforCourses', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * function to publish and unpublish the program
 */
function publishOrUnpublishProgram($programId){
  global $user;
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    $status  = getProgramStatus($programId);
    $updateStmt = db_update('slt_program');
    $updatedstatus  = ($status=='lrn_lpn_sts_atv')?"lrn_lpn_sts_itv":"lrn_lpn_sts_atv";
    $returnStatus   =  ($updatedstatus=='lrn_lpn_sts_atv')?t('LBL571'):t('LBL570');
    $updateStmt->condition('id', $programId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
    					'published_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
	/* Update tag activity Count */	
	updateTagActivityCount($programId,getProgramEntityType($programId),$updatedstatus);	
	/* Update tag activity Count */
    /* Node For Program */
    $prgDetails = getProgramObjDetails($programId);
	
    $prgType = $prgDetails[0]->object_type;
    $nodeSelect = db_select('slt_node_learning_activity', 'entity');
    $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
    $nodeSelect->addField('n','nid','nid');
    $nodeSelect->addField('n','vid','vid');
    $nodeSelect->condition('entity.entity_id', $programId);
    $nodeSelect->condition('entity.entity_type', $prgType);
    $nodeResult =  $nodeSelect->execute()->fetchAll();
    if(count($nodeResult)){
      $oNode = new StdClass();
      $oNode->nid= $nodeResult[0]->nid;
      $oNode->vid= $nodeResult[0]->vid;
      $drupalUserId = $user->uid;
      $oNode->uid = $drupalUserId;
      $oNode->language='en';
      $oNode->type='product';
      $oNode->created=time();
      $oNode->changed=time();
      $oNode->teaser_include=1;
      $oNode->revision= '';
      $oNode->format=1;
      $oNode->log='';
      $oNode->name=$drupalUserId;
      $oNode->date='';
      $oNode->status= ($updatedstatus == 'lrn_lpn_sts_atv') ? 1 : 0;
      $oNode->promote=0;
      $oNode->sticky=0;

      $oNode->op="Save";
      $oNode->submit="Save";
      $oNode->preview="Preview";
      $oNode->form_id="product_node_form";

      $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
      expDebug::dPrint("Delivery_type id : v_id - ". $vid,4);

      if($vid){
          $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
            if(empty($delivery_type_all_tid)) {
              $terms               = new stdClass;
              $terms->name         = '--All Tp--';
              $terms->vid          = $vid;
              //$terms->description  = '';
              //$terms->parent       = '';
              taxonomy_term_save($terms);
              $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
            }
      $oNode->taxonomy_catalog['und'] = array();
      $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
      $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$prgType."' and vid = ".$vid)->fetchField();

      expDebug::dPrint("Delivery_type id :  - " . $delivery_type_tid,4);
      $dtTerms = array(
         'name' => $delivery_type_name,
         'vid' => $vid,
         'description' => $prgType,
         'parent' => $delivery_type_all_tid,
      );

      if(empty($delivery_type_tid)) {
         $oNode->taxonomy_catalog['und'][] = array('tid' => admincreateVocabTerms($dtTerms));
      }
      else {
         $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
      }
      $delivery_type_prg_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$prgType."' and vid = ".$vid)->fetchField();
      $class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$programId."' and vid = ".$vid)->fetchField();
      $dtTerms1 = array(
         'name' => $prgDetails[0]->title.' : '.$prgDetails[0]->code,
         'vid' => $vid,
         'description' => $programId,
         'parent' => $delivery_type_prg_tid
       );
      if(!empty($class_tid)){
        $dtTerms1['tid'] =  $class_tid;
      }
      $tid = admincreateVocabTerms($dtTerms1);
      expDebug::dPrint(' Taxonomy id: $tid = ' . print_r($tid, true) , 4);

      $oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
      $oNode->teaser='';
      $oNode->validated=1;
      expDebug::dPrint("Tp Node :");
      expDebug::dPrint($oNode ,4);
      node_save($oNode);

      // Node creation for Forum integration : Start

      /*$forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
      $forum_tid = db_query("select tid FROM taxonomy_term_data WHERE description = 'TP' AND format ='".$programId."' AND vid = ".$forumvid)->fetchField();

      $forumTerms = array(
         'name'        => $prgDetails[0]->title,
         'vid'         => $forumvid,
         'description' => "TP",//$description,
         'format'      => $programId
       );

      if(!empty($forum_tid)){
        $forumTerms['tid'] =  $forum_tid;
      }

      expDebug::dPrint(' Forum Update = ' .$programId.'--===-'. $forum_tid. '--===-'. $forumTerms['tid']);
      $tid = admincreateVocabTerms($forumTerms);
      node_save($forumNode);*/

      // Node Updation for Forum integration : End
      }
    }
    //node update logic end

    add_audit_trail_entry($currUserId, $programId, 'cre_sys_obt_trp', 'exp_sp_administration_program', 'publishOrUnpublishProgram', 'Updated program Status',
		                                     null, $status, $updatedstatus);
   
    print $updatedstatus.'|'.$returnStatus;
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishProgram', $ex);
    throw $ex;
  }
  
  unset($txn);
  syncSolrData('Program'); //To sync solr
}
/*
 * fucntion to retrive the current status of the program
 */
function getProgramStatus($programId){
  try {
    $select = db_select('slt_program', 'prg');
    $select->addField('prg','status', 'status');
    $select->condition('prg.id',$programId);
    // Execute the query
    $prgStatus = $select->execute()->fetchField();
    return $prgStatus;
  } catch (Exception $ex) {
    watchdog_exception('getProgramStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function attachCourseDetails($entityId,$entityType,$module_id,$crsfrom_build_id){
  try {
  		
  	  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'module_id'	  => $module_id,
       'entity_type'  => $entityType,
       'crsfrom_build_id' => $crsfrom_build_id
      );

      $formId                 = 'addcrs_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();
      $attachCourseResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($attachCourseResponse);
      exit;

  } catch (Exception $ex) {
    watchdog_exception('attachCourseDetails', $ex);
    expertusErrorThrow($ex);
  }

}

function addcrs_addedit_form_html($form,&$form_state) {
  try {
  	
    return displayAttachCourseDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type']);
  } catch (Exception $ex) {
    watchdog_exception('addcrs_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }

}
function addcrs_addedit_form_html_validate($form,&$form_state) {
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attach_course.inc');
		$entityId = $form_state['entity_id'];
		$entityType = $form_state['entity_type'];
		$uniqueId = 'TPAttachCourse-'.$entityId.'-'.$entityType;
		expDebug::dPrint("triggering element >> ". print_r($form_state,true),5);
		if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
			form_set_error('',t('ERR106'));
			$form_state['esign_show_popup']= false;
		}
	} catch (Exception $ex) {
		watchdog_exception('addcrs_addedit_form_html_validate', $ex);
		expertusErrorThrow($ex);
	}
}

//Assessment popup
function attachAssessmentDetails($entityId,$entityType,$module_id){
  try {
  	expDebug::dPrint("inside attach assessment ");
  	//$entityType = 'cre_sys_obt_crt';
  	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'module_id'	  => $module_id,
       'entity_type'  => $entityType
      );
	  
      $formId                 = 'addassessment_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      expDebug::dPrint("built form ".print_r($builtForm,true),5);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();
      $attachCourseResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );
		
      echo drupal_json_output($attachCourseResponse);
      exit;

  } catch (Exception $ex) {
    watchdog_exception('attachCourseDetails', $ex);
    expertusErrorThrow($ex);
  }

}
function addassessment_addedit_form_html($form,&$form_state) {
  try {
  	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
  	$uniqueId     = $form_state['entity_id'].'-'.$form_state['entity_type'];
  	$form = array(
	      '#type' => 'markup',
	   		// '#prefix' => (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '' ),
	      '#prefix' =>  '</form>',
	    );
  	 //$form = drupal_get_form('assessment_example_form');
     displayAssessmentDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type'],$uniqueId,'',$form_state['module_id']);
     return $form;
  } catch (Exception $ex) {
    watchdog_exception('addassessment_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }

}
function addassessment_addedit_form_html_validate($form,&$form_state) {
	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
	$form_state['hidden-save-submit']=true;
}
function attachSurveyDetails($entityId,$entityType,$module_id){
  try {
  	expDebug::dPrint("inside attach survey ");
  	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
       $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'entity_id'    => $entityId,
       'module_id'	  => $module_id,
       'entity_type'  => $entityType
      );
	  
      $formId                 = 'addsurvey_addedit_form_html';
      $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();
      $attachCourseResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );

      echo drupal_json_output($attachCourseResponse);
      exit;

  } catch (Exception $ex) {
    watchdog_exception('attachCourseDetails', $ex);
    expertusErrorThrow($ex);
  }

}

function addsurvey_addedit_form_html($form,&$form_state) {
  try {
  	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
  	$uniqueId     = $form_state['entity_id'].'-'.$form_state['entity_type'];
  	$form = array(
	      '#type' => 'markup',
	   		// '#prefix' => (( stripos($_SERVER['HTTP_USER_AGENT'], 'Chrome')  || stripos($_SERVER['HTTP_USER_AGENT'], 'Firefox'))? '</form>' : '' ),
	      '#prefix' =>  '</form>',
	    );
  	 //$form = drupal_get_form('assessment_example_form');
     displaySurveyDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type'],$uniqueId,'',$form_state['module_id']);
     return $form;
  } catch (Exception $ex) {
    watchdog_exception('addassessment_addedit_form_html', $ex);
    expertusErrorThrow($ex);
  }
}

function addsurvey_addedit_form_html_validate($form,&$form_state) {
	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
	$form_state['hidden-save-submit']=true;
}
function getCrsAutocomplete($programId,$moduleId){
	expDebug::dPrint("include one ");
	expDebug::dPrint("program id >>> ".$programId."module id>>" .$moduleId);
    $searchCurText =  $_GET['z'];
    $select = db_select('slt_module_crs_mapping', 'crsmapping');
    $select->leftjoin('slt_program', 'prg', 'prg.id = crsmapping.program_id');
    $select->leftjoin('slt_course_template', 'crs', 'crs.id = crsmapping.course_id');
    $select->addField('crs', 'title', 'title');
    $select->condition('crsmapping.program_id', $programId, '=');
    $select->condition('crsmapping.module_id', $moduleId, '=');
    $select->condition('crs.title', '%' . db_like($searchCurText) . '%' ,'like');
    $matchingCoursesList = $select->execute()->fetchAll();
}

/*
 * Get the Attached course Name /Group name for a Particular TP
 */
function getProgramCourseAutoComplete($programId,$moduleId) {

  try {
  	expDebug::dPrint("program id >>> ".$programId."module id>>" .$moduleId);
       $searchCurText =  $_GET['z'];
       $searchType    =  $_GET['search_type'];


       if($searchType =='crstit'){
         $select = db_select('slt_module_crs_mapping', 'crsmapping');
         $select->leftjoin('slt_program', 'prg', 'prg.id = crsmapping.program_id');
         $select->leftjoin('slt_course_template', 'crs', 'crs.id = crsmapping.course_id');
         $select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
         $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
         $select->addField('crs', 'title', 'title');
         $select->addExpression('group_concat(DISTINCT(td.tagname))', 'tagname');
         $select->condition('crsmapping.program_id', $programId, '=');
		 		 $select->condition('crsmapping.module_id', $moduleId, '=');
      	 $select->condition(db_and()->
      				condition(db_or()
      				   -> condition('crs.title', '%' . db_like($searchCurText) . '%' ,'like')
   				       -> condition('td.tagname', '%' . db_like($searchCurText) . '%', 'like')
      			));
      	 $select->groupBy('crs.title');
      }
      elseif($searchType == 'grpnametype'){
         $select = db_select('slt_module_groups', 'modules');
         $select->addField('modules', 'title', 'title');
         $select->condition('modules.program_id', $programId, '=');
         $select->condition('modules.module_id', $moduleId, '=');
         $select->condition('modules.title', db_like($searchCurText) . '%', 'LIKE');
	   }
	    elseif($searchType == 'crscode'){
	      $select = db_select('slt_module_crs_mapping', 'crsmapping');
        $select->leftjoin('slt_program', 'prg', 'prg.id = crsmapping.program_id');
        $select->leftjoin('slt_course_template', 'crs', 'crs.id = crsmapping.course_id');
        $select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
        $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
        $select->addField('crs', 'code', 'title');
     //   $select->addField('td','tagname', 'tagname');
        $select->condition('crsmapping.program_id', $programId, '=');
        $select->condition('crsmapping.module_id', $moduleId, '=');
        $select->condition(db_and()->
        		condition(db_or()
        				-> condition('crs.code', '%' . db_like($searchCurText) . '%' ,'like')
        				-> condition('td.tagname', '%' . db_like($searchCurText) . '%', 'like')
        		));
	   }

       $matchingCoursesList = $select->execute()->fetchAll();
       expDebug::dPrint('Testing pgram id'.$programId,4);
       expDebug::dPrint(' $select obj11 = ' . print_r($select->__toString(), true), 3);
       expDebug::dPrintDBAPI('$select with auto_complete', $select);
       expDebug::dPrint("matching courses list ".print_r($matchingCoursesList,true),5);
      foreach ($matchingCoursesList as $matchingCourse) {
          print $matchingCourse->title. "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getProgramCourseAutoComplete', $ex);
    expertusErrorThrow($ex);
  }

}

function getProgramCourseAutoCompleteSearch() {
  try {
       $searchCurText =  $_GET['z'];
    
         $select = db_select('slt_module_crs_mapping', 'crsmapping');
         $select->leftjoin('slt_program', 'prg', 'prg.id = crsmapping.program_id');
         $select->leftjoin('slt_course_template', 'crs', 'crs.id = crsmapping.course_id');
         $select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
         $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
         $select->addField('crs', 'title', 'title');
         $select->addExpression('group_concat(DISTINCT(td.tagname))', 'tagname');
      	 $select->condition(db_and()->
      				condition(db_or()
      				   -> condition('crs.title', '%' . db_like($searchCurText) . '%' ,'like')
      				   -> condition('crs.code', '%' . db_like($searchCurText) . '%' ,'like')
   				       -> condition('td.tagname', '%' . db_like($searchCurText) . '%', 'like')
      	 ));    
	 
       $matchingCoursesList = $select->execute()->fetchAll();
       expDebug::dPrint('Testing pgram id'.$programId,4);
       expDebug::dPrint(' $select obj11 = ' . print_r($select->__toString(), true), 3);
       expDebug::dPrintDBAPI('$select with auto_complete', $select);
       expDebug::dPrint("matching courses list ".print_r($matchingCoursesList,true),5);
      foreach ($matchingCoursesList as $matchingCourse) {
          print $matchingCourse->title. "\n";
      }
  } catch (Exception $ex) {
    watchdog_exception('getProgramCourseAutoCompleteSearch', $ex);
    expertusErrorThrow($ex);
  }

}



/*

function addcrs_addedit_form_html_validate($form,&$form_state) {

  form_set_error('error', 'Error');


    $courseId     =    $form_state['input']['hid_courseset_id_0'];
  $programId    =    $form_state['values']['editprogram_id'];

  if ($form_state['triggering_element']['#value'] == t('LBL287')) {
    if($form_state['values']['courses_name_'] ==""){
      form_set_error('courses_name_', t('ERR089'));
    }
    if($form_state['values']['tpattachinggroupname'] == "Enter Group Name" || $form_state['values']['tpattachinggroupname'] == ''){
      form_set_error('tpattachinggroupname', t('ERR090'));
    }else{
          $recertify=($form_state['values']['recertify'] == 0)?'N' : 'Y';

          $checkTp = checkingTpAssingnedCourses($courseId, $programId);
          $checkRecertify = checkingTpAssingnedRecertify($courseId, $programId);
          $conflictStatus = _programEnrollmentsConflictCheck($programId, $courseId);

          if($recertify == 'Y') {
            if($checkRecertify > 0){
              form_set_error('recertify', t('ERR122')); //re-certify exits
            }else{
              if($checkTp == 0 && $conflictStatus == 1){
                  form_set_error('courses_name_', t('ERR124')); //session conflict
              }
            }
          }else{
            if($checkTp > 0){
              form_set_error('courses_name_', t('ERR123')); //course exits
            } else {
                if($checkRecertify == 0 && $conflictStatus == 1){
                  form_set_error('courses_name_', t('ERR124'));  //session conflict
                }
            }
          }

   }

    $form_state['storage']['attacherror'] = 'tperror';
 }
  //$form_state['values']['tp_courset_id']=!empty($form_state['input']['hid_courseset_id_0']) ? $form_state['input']['hid_courseset_id_0'] : 'nooo';
  return;

}
*/
/*
function loadDataGridListAttachedCourse($type, $entityId, $entityType){

  $page      = $_GET['page'];
  $limit     = $_GET['rows'];
  $sidx      = $_GET['sidx'];
  $sord      = $_GET['sord'];
  $tab       = $_GET['tab'];
  $uniqueId  =  "$type-$entityId-$entityType";

  $queryRequired = 1;

  switch($type){

    case 'ListAttachedCourse':
        $listquery = loadAttachCourseForTP($searchKeyword,1);
    break;

  }

  $count = 0;
  $count = count($listquery->execute()->fetchAll());


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

  $srch_result_response_attachcourse->total   = $total_pages;
  $srch_result_response_attachcourse->records = $count;
  $srch_result_response_attachcourse->page    = $page;
  $srch_result_response_attachcourse->unique_id    = $uniqueId;

  $_SESSION['jq_count'] = array();
  $i=0;


  switch($type){

    case 'ListAttachedCourse':

      foreach ($queryResult as $rec_search_results)
      {
        $srch_result_response_attachcourse->rows[$i]['id']   = $rec_search_results->id;
        $srch_result_response_attachcourse->rows[$i]['cell'] = array(
        	 'id' 	              => $rec_search_results->id,
             'CourseTitle' 	      => $rec_search_results->title,
             'CourseCode' 	      => $rec_search_results->code,
          );;
        $i++;
      }
      break;


   }

  echo drupal_json_encode($srch_result_response_attachcourse);

}*/
function training_plan_enroll_recertify_history($entityId,$entityType,$enrollId){
	try {
	  $form_state = array(
	   'no_redirect'  => TRUE,
	   'build_info'   => array('args' => array('ajax')),
	   'enroll_entity_id'    => $entityId,
	   'enroll_entity_type'  => $entityType,
	   'enroll_id'  => $enrollId,
	   'call_from'  => 'training_plan_enroll_display',
	  );

	  $formId                 = 'training_plan_enroll_recertify_form';
	  //$builtForm              = drupal_build_form($formId, $form_state, $entityId);
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
	    watchdog_exception('training_plan_enroll_recertify_history', $ex);
	    expertusErrorThrow($ex);
	  }
}

function training_plan_enroll_recertify_form($form, &$form_state) {
  //include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program.inc');
  try {
	//expDebug::dPrint("testing 1111 >>> ");
  $entityId = $form_state['enroll_entity_id'];
  $entityType = $form_state['enroll_entity_type'];
  $enrollId = $form_state['enroll_id'];
  $uniqueId = $entityId.'-'.$entityType;

  //$form_state['values']['id'] = $entityId;
  $form_state['esign_from_plugin']= true;
  $form['mapped_program_id'] = array(
    '#type' => 'hidden',
    '#name' => 'mapped_program_id',
    '#value' => $entityId
  );
  $form['mapped_program_type'] = array(
    '#type' => 'hidden',
    '#name' => 'mapped_program_type',
    '#value' => $entityType
  );
  $form['mapped_program_type'] = array(
    '#type' => 'hidden',
    '#name' => 'mapped_enroll_id',
    '#value' => $enrollId
  );
  $form['mapped_module_id'] = array(
          '#type' => 'hidden',
          '#name' => 'selected_enroll_path',
          '#value' => ''
   );
  $form['mapped_module_name'] = array(
          '#type' => 'hidden',
          '#name' => 'selected_enroll_path_name',
          '#value' => ''
  );
  $form['addedit-enroll_launch_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="program-tprecertify-basic-addedit-form-container" class="enrollTPrecertifyParentDiv">',
  );

  displayTPRecertifyDetails($form, $form_state, $entityId, $entityType ,$enrollId, $uniqueId);

  $form['addedit-enroll_launch_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  expDebug::dPrint("testing 333 >>> ".print_r($form,true),5);
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('training_plan_enroll_form_html', $ex);
  	expertusErrorThrow($ex);
  }
}


function training_plan_enroll_display($entityId,$entityType) {
  unset($_SESSION['exempted-tp-'.$entityId]);
  try {
  $form_state = array(
   'no_redirect'  => TRUE,
   'build_info'   => array('args' => array('ajax')),
   'enroll_entity_id'    => $entityId,
   'enroll_entity_type'  => $entityType,
   'call_from'  => 'training_plan_enroll_display',
  );

  $formId                 = 'training_plan_enroll_form_html';
  //$builtForm              = drupal_build_form($formId, $form_state, $entityId);
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
    watchdog_exception('training_plan_enroll_display', $ex);
    expertusErrorThrow($ex);
  }

}

function training_plan_enroll_form_html($form, &$form_state) {
  //include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program.inc');
  try {

  $entityId = $form_state['enroll_entity_id'];
  $entityType = $form_state['enroll_entity_type'];
  $uniqueId = $entityId.'-'.$entityType;

  //$form_state['values']['id'] = $entityId;
  $form_state['esign_from_plugin']= true;
  $form['mapped_program_id'] = array(
    '#type' => 'hidden',
    '#name' => 'mapped_program_id',
    '#value' => $entityId
  );
  $form['mapped_program_type'] = array(
    '#type' => 'hidden',
    '#name' => 'mapped_program_type',
    '#value' => $entityType
  );
  $form['mapped_module_id'] = array(
          '#type' => 'hidden',
          '#name' => 'selected_enroll_path',
          '#value' => ''
   );
  $form['mapped_module_name'] = array(
          '#type' => 'hidden',
          '#name' => 'selected_enroll_path_name',
          '#value' => ''
   );
  $form['addedit-enroll_launch_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="program-tp-basic-addedit-form-container" class="catalog-course-basic-addedit-form-container enrollTPuserParentDiv">',
  );

  displayTPRegisterDetails($form, $form_state, $entityId, $entityType ,$uniqueId);

  $form['addedit-enroll_launch_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('training_plan_enroll_form_html', $ex);
  	expertusErrorThrow($ex);
  }
}

function training_plan_enroll_form_html_validate($form, &$form_state) {
 try {
 	
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_tp_register.inc');
  $form_state['esign_show_popup']= true;
  $form_state['storage']['mode'] = 'edit';
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];
  $uniqueId = "enrolltpuser-$entityId-$entityType";
  $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];

  $triggerElementByName  = $form_state['triggering_element']['#name'];

  $prgDetails = getProgramObjDetails($entityId);
  $isPricedTP = $prgDetails[0]->price;
  $pricedErr = 0;
  /*-- #36602 - Issue fix --*/
  $isCommerceEnabled =  module_exists('exp_sp_commerce');

  if($isPricedTP > 0 && $triggerElementByName == 'commonsave'){
  	$selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolltpuser-'.$entityId.'-'.$entityType];
  	$selectedIdsArray = explode(',', $selectedEnrolledIds);
  	foreach($selectedIdsArray as $enrollId){
  		$status = $form_state['input']['enrolled_status_'.$enrollId];
  		if($status == t('Enrolled')){
  			$pricedErr = 1;
  		}
  	}
  }
  if(($isPricedTP > 0 && $isCommerceEnabled)  && ($entityPlugin =='enrolltpuser' || $triggerElementByName=='enrolledandsave' || $pricedErr)){
  	form_set_error('',t('MSG716'));
  }

  if($triggerElementByName=='commonsave' || $triggerElementByName =='incompletedandsave' || $triggerElementByName=='completedandsave'|| $triggerElementByName=='enrolledandsave'||$triggerElementByName=='cancelledandsave'||$triggerElementByName=='noshowandsave') {
    if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
      expDebug::dPrint('enrolltpuser form_set_error1');
      form_set_error('', t('ERR106'));
      $form_state['esign_show_popup']= false;
    }
  }else if(isset($entityPlugin) && $entityPlugin=='enrolltpuser') {
    if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
      expDebug::dPrint('enrolltpuser form_set_error2');
      form_set_error('', t('ERR106'));
      $form_state['esign_show_popup']= false;
    }
  }

  if($triggerElementByName == 'hidden-save'  || $triggerElementByName == 'set-enrolltpuser' || $triggerElementByName == 'set-enroll-tp-user-mandatory') {
    $form_state['hidden-save-submit']=true;
    $form_state['esign_show_popup']= false;
  }
  } catch (Exception $ex) {
  	watchdog_exception('training_plan_enroll_form_html_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function training_plan_enroll_form_html_submit($form, &$form_state) {
	try {
		$entityType = $form_state['enroll_entity_type'];
		$entityId = $form_state['enroll_entity_id'];
		$uniqueId = $entityId.'-'.$entityType;
		// E-sign Pop-up Check For Enroll Tp User --- Catalog => Enrollment.  For This Ticket #0040313
		$chkValidation = 1;
		expDebug::dPrint('submit : '.$form_state['hidden-save-submit'] .' && esign_validate : '.$_SESSION['esign_validate'], 4);
		if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
			$chkValidation = 0;
			if($form_state['hidden-save-submit']==true && $_SESSION['esign_validate'] ==  1){
				$chkValidation = 1;
				$form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
			}else{
				$form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
			}
		}
		if(isset($form_state['values']['attr_action']) && isset($entityId) && $chkValidation == 1 ) {
			if($form_state['values']['attr_action'] == 'enrolltpuser') {
				addNewEnrollTPUserDetails($form, $form_state, $entityId, $entityType);

				// Clear enrolltpuser-autocomplete form textfield as switching from enroll user view to roster view - fix for ticket #0022120
				clearTPRosterEnrollUserAutocomplete($form_state);
			}
			$form_state['rebuild']  = TRUE;
			unset($_SESSION['esign_validate']);
		}
	} catch (Exception $ex) {
		watchdog_exception('training_plan_enroll_form_html_submit', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_viewtp_addedit_form($js = NULL, $entityId){
	try{
		//0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
		expDebug::dPrint('sabbeeee-->'.$entityId);
		includeFilesForProgram();
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
					array('id' => 'exp_sp_administration_view_tpdetails_addedit_form', 'title' => (t('LBL816'))) //view
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewclass_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewtp_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_tpdetails_addedit_form($form, &$form_state,$renderMode, $prgId){
	try {
		include_once(drupal_get_path('module', 'exp_sp_learning_plan_detail') .'/exp_sp_learning_plan_detail.inc');
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$entityId---->'.$prgId);

		$wrapperId = 'viewprogram-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);

		$programdetails = getTrainingplanDetails($prgId);
		expDebug::dPrint('$classdetails-->'.print_r($programdetails,true),5);

		$form['narrow_tpview'] = array(
				'#markup'       => theme('narrow_tpview', array('record' => $programdetails,'type' => 'cre_sys_obt_trp')),
		);
		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_tpdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}
function getSelectPathDisplay($type,$entityId,$entityType){
	try {
		$form_state = array(
			'no_redirect'  => TRUE,
			'build_info'   => array('args' => array('ajax')),
			'entity_id'    => $entityId,
			'entity_type'  => $entityType,
			'type'  => $type
		);
		$formId                 = 'select_module_path_html';
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
		watchdog_exception('getAddModuleDisplay', $ex);
		expertusErrorThrow($ex);
	}
}

function select_module_path_html($form, &$form_state){
	try{
		$entityId = $form_state["entity_id"];
		$entityType = $form_state["entity_type"];
		$type = $form_state["type"];
		expDebug::dPrint("testing Entity Id ". $entityId . "Entity Type ".$entityType . "type ". $type, 5);
		$donelbl = t('LBL569');
		$closelbl = t('LBL123');
		$registrationdatelbl = t('LBL674').' '.t('LBL3060');
		$availablePath = getModuleIdFromObjectId($entityId);
		$formName = "select-module-path";
		$wrapperId = 'tp-select-module'.$entityId;
	
		if(empty($form)){
			$form = array(
					'#type' => 'markup',
					'#prefix' =>  '</form>',
			);
		}
		
		$form[$formName]['select_path_module_start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="select_path_module" class="module-scroll-pane">',
		);
		addHiddenFieldToAddEditForm($form, 'module_sel', '');
		$form[$formName]['module_td_start'] = array(
				'#type' => 'markup',
				'#prefix' =>'<table width="100%" cellpadding="4" cellspacing="0" border="0" id="add-module-table">',
				'#suffix' => '',
		);
		
		$select = '<div class="certifcatepath-custom-select"> <select id = "certifcatepath">';
		foreach($availablePath as $path){
			$select .= '<option value="'.$path->module_id.'">'.$path->module_title.'</option>';
		}
		$select .= '</select><div class="certifcatepath-custom-select-arrow"></div></div>';
		$markup = '<div class="certifcatepathlbl">'.$registrationdatelbl.': </div>'.$select.'<div id="survey-tp-path-popup" class="addedit-form-cancel-container-actions"></div>';
		$form[$formName]['module_td_end'] = array(
				'#type' => 'markup',
				'#prefix' => $markup,
				'#suffix' => '</table>',
		);
		$form[$formName]['select_path_module_end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>',
		);
		$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
		$cancelAttributes['onclick'][]         = "closeQtip('','$cancelButtonId'); return false;";
		$cancelAttributes['data-wrapperid'] = array($wrapperId);
		
		$form[$formName]['cancel_addmodule_edit_mode'] = array(
				'#type' => 'button',
				'#value' => t('LBL109'),
				'#title' => t('LBL109'),
				'#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom-setsurvey"><div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn"><div class="white-btn-bg-left"></div>',
				'#suffix' => '<div class="white-btn-bg-right"></div>',
				'#name' => 'Cancel attachcourse',
				'#attributes' => $cancelAttributes,
		);
		if($type == 'roster')
			$uniqueId = $entityId.$entityType;
		else 
			$uniqueId = $type."-".$entityId."-$entityType";
			
		
		$setAttributes['class'][] = 'admin-save-button-middle-bg attach-list-view-attach-btn';
		$setAttributes['data-wrapperid'] = 'attach_course_dt';
		$setAttributes['onclick'][] = "fillSurveyPath('".$uniqueId."','".$type."'); return false;";
		$form[$formName]['set_attachmodule_edit_mode'] = array(
				'#type' => 'button',
				'#prefix' => '<div class="admin-save-button-container-path"><div class="admin-save-button-left-bg"></div>',
				'#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
				'#title' => t('LBL569'), //Done
				'#value' => t('LBL569'), //Done
				'#name' => 'add_module',
				'#attributes' => $setAttributes,
		);
		 
		expDebug::DPrint("final form result getSelectPath >> ". print_r($form,true),5);
		return $form;
		
		
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_tpdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function getTrainingplanDetails($programId){
	try{
		//expDebug::dPrint('enter the function'.$programId);
		$vLMSUserId = getSltpersonUserId();
		// Select table
		$selStmt = db_select('slt_program', 'lp');
		//  $selStmt->leftJoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id');
		//  $selStmt->leftJoin('slt_course_template', 'crs', 'map.course_id = crs.id');
		$selStmt->leftJoin('slt_profile_list_items', 'pro', 'lp.object_type = pro.code');
		$selStmt->leftJoin('slt_profile_list_items', 'prof', 'lp.lang_code = prof.code');
		$selStmt->leftJoin('slt_profile_list_items', 'curr_prof', 'curr_prof.attr1 = lp.currency_type');
		//$selStmt->leftJoin('slt_profile_list_items', 'spcur', 'spcur.attr1 = lp.currency_type');
	//	$selStmt->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=lp.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
		//$selStmt->leftJoin('slt_node_learning_activity','nl','lp.id = nl.entity_id and nl.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
		//$selStmt->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=lp.id AND catacs.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
	//	$selStmt->leftJoin('slt_group_mapping', 'group_map', 'group_map.entity_id=lp.id AND group_map.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')');
	//	$selStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');


		//filterByUserAccessPrivileges($selStmt, 'catacs',$vLMSUserId,'group_map');

		// Set conditions
		$selStmt->condition('lp.id', $programId);

		// Select fields
		$selStmt->addField('lp','id', 'id');
		$selStmt->addField('lp','title','title');
		$selStmt->addField('lp','short_desc','description');
		$selStmt->addField('lp','short_desc','short_desc');
		$selStmt->addField('lp','code',	'code');
		$selStmt->addField('lp','end_date',	'end_date');
		$selStmt->addField('lp','expires_in_value','expires_in_value');
		$selStmt->addField('lp','expires_in_unit', 'expires_in_unit');
		$selStmt->addField('lp','custom0', 'view_count');
		$selStmt->addField('lp','object_type','object_type');
		$selStmt->addField('lp', 'price', 'price');
		$selStmt->addField('curr_prof', 'attr2', 'currency_symbol');
		$selStmt->addField('lp', 'enforce_sequence','sequence');
		$selStmt->addField('pro',	'code',	'pro_code');
		$selStmt->addField('prof', 'name',	'lang');
		$selStmt->addField('pro',	'name',	'pro_type');
		$selStmt->addField('lp',	'author_vendor','author');
		$selStmt->addField('lp',	'additional_info','additional_info');
		$selStmt->addField('lp',	'addn_catalog_show','addn_catalog_show');
		$selStmt->addField('lp',	'addn_notification_show','addn_notification_show');
		//$selStmt->addField('mro','mro_flag', 'mro_id');
	//	$selStmt->addField('nl','node_id', 'node_id');
		expDebug::dPrintDBAPI('$selStmt', $selStmt);

		// Execute query
		$learningDetails = $selStmt->execute()->fetchAll();
		expDebug::dPrint('$learningDetails = ' . print_r($learningDetails, true));

		if (count($learningDetails) > 0) {
			$module_list = getModuleIdFromObjectId($programId);
			$i=0; 
			foreach($module_list as $mod){
			// Call the new function
				$learningDetails['module'][$i]->module_title = $mod->module_title;
				$learningDetails['module'][$i]->module_id = $mod->module_id;
				$learningDetails['module'][$i]->courselist = fetch_learning_courses_list($programId,$mod->module_id);
				$i++;
			}
			$learningDetails[0]->description = trim($learningDetails[0]->description);
			// Prepare the expires in value string that is to be displayed
			if ($learningDetails[0]->object_type == 'cre_sys_obt_crt') {
				$learningDetails[0]->expires_in_value_str =
				prepareCrtExpiresInDisplayString($learningDetails[0]->expires_in_value, $learningDetails[0]->expires_in_unit);
			}
		}


		$prereqequv = getPrerequisiteTPDetails($programId);
		$prereqArr = Array();
		foreach ($prereqequv as $row){
			$prereqArr[] = $row;
		}
		$learningDetails[0]->prerequisites = $prereqArr;

		expDebug::dPrint('$learningDetails in view page = ' . print_r($learningDetails, true), 4);
		return $learningDetails;

	}
	catch (Exception $ex) {
		watchdog_exception('getTrainingplanDetails', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * check training plan have enrollment status retrun true;
 */
function sequenceAttachCourseCheck($programId) {
  $txn = db_transaction();
  $enrollment = 'lrn_tpm_ovr_enr';
  $status_enrollment = false;
  try {
    $select = db_select('slt_master_enrollment', 'masenrollment');
    $select->addExpression('COUNT(masenrollment.id)', 'count');
    $select->condition('masenrollment.program_id', $programId, '=');
    $select->condition('masenrollment.overall_status', $enrollment, '=');
    $uniquecount = $select->execute()->fetchField();

    if($uniquecount >=1) {
      $status_enrollment = true;
    }

    expDebug::dPrint(' enrollment = ' .print_r($status_enrollment, true) , 4);
    return $status_enrollment;
  }
  catch (Exception $ex) {
    watchdog_exception('sequenceAttachCourse', $ex);
  }
}
/*
 * fucntion to retrive the Entity Type of the program
 */
function getProgramEntityType($programId){
  try {
    $select = db_select('slt_program', 'prg');
    $select->addField('prg','object_type', 'status');
    $select->condition('prg.id',$programId);
    // Execute the query
    $object_type = $select->execute()->fetchField();
	if($object_type == 'cre_sys_obt_trn'){
		$entity_type='Learning Plan';
	}elseif($object_type == 'cre_sys_obt_crt'){
		$entity_type='Certification';
	}elseif($object_type == 'cre_sys_obt_cur'){
		$entity_type='Curricula';
	}elseif($object_type == 'cre_sys_obt_trn'){
		$entity_type='';
	}
    return $entity_type;
  } catch (Exception $ex) {
    watchdog_exception('getProgramEntityType', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * ADD new module for TP
 * */
function addnewModuleForTP($program_id,$userid,$object_type,$moduleName = ''){
  try {
	$select = db_select('slt_module', 'sltmodule');
 	$select->addExpression('COUNT(sltmodule.program_id)', 'countnewval');
 	$select->condition('sltmodule.program_id', $program_id, '=');
 	expDebug::dPrintDBAPI(' $select ', $select);
 	$tpcourseCount = $select->execute()->fetchField();
 	
 	expDebug::dPrintDBAPI(' fetch_learning_courses_list() count:  = ', $select);
 	expDebug::dPrint(' $tpcourseCount = ' . print_r($tpcourseCount, true) , 3);
	if(!empty($moduleName))
		$groupName = $moduleName;
	else
 		$groupName = ($object_type == 'cre_sys_obt_crt') ? "Module Path - ".($tpcourseCount+1) : "Courses";
 	$insertStmt = db_insert('slt_module');
 	$fields = array(
 			'program_id'  		 => $program_id,
 			'title'              => $groupName,
 			'code'               => '',
 			'created_by'         => $userid,
 			'sequence'			 => $tpcourseCount+1,
 			'created_on'         => now(),
 			'updated_by'         => $userid,
 			'updated_on'         => now(),
 			'status'             => 'spp_tpm_mdl_atv',
 			'is_required'        => '1',
 			'parent_id'			     => '0',

 	);
 	// Select the fields and the values to be inserted
 	$insertStmt->fields($fields);
 	expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);
 	$module_id = $insertStmt->execute();
 	
 	//update module code 
 	$auto_code="modco_".$module_id;
 	$updateStmt = db_update('slt_module');
 	$updateStmt->condition('id', $module_id, '='); 				
 	$updateStmt->fields(array(
 			'code'     =>  $auto_code
 	));
 	expDebug::dPrintDBAPI(' update module code  = ', $updateStmt);
 	$numUpdated = $updateStmt->execute(); 
 	
 	make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$program_id, 'Inserted', $object_type, 'exp_sp_administration_program',
 			'addnewModule', array('program_id','title','code'));
 	return $module_id;
  } catch (Exception $ex) {
    watchdog_exception('getProgramEntityType', $ex);
    expertusErrorThrow($ex);
  }
}

function groupEditSaveTP($program_id,$module_id,$group_name){
  try {
		expDebug::dPrint("program Id >> ". $program_id . "module Id >>" .$module_id ." group name >> ".$group_name , 4);
		$userid = getIdOfLoggedInUser();
		$random_num=rand(1,99999);
 		$grp_code="grpco_".$random_num;
		//Insert into module groups table
		$insertStmt = db_insert('slt_module_groups');
    	$fields = array(
                'code'     => $grp_code,
                'title'   => $group_name,
                'program_id' => $program_id,
                'module_id'   => $module_id,
                'sequence'   => 9999,
    			'created_by' => $userid,
    			'created_on' => now(),
    			'updated_by'   => $userid,
    			'updated_on'   => now(),
    	);

    	// Select the fields and the values to be inserted
    	$insertStmt->fields($fields);
		expDebug::dPrintDBAPI(' Insert module groups  = ', $insertStmt);
    	$lastinserid = $insertStmt->execute();
    	expDebug::dPrint(' last group inserid = ' . print_r($lastinserid, true) , 3);
   		$updateStmt = db_update('slt_module_crs_mapping');
 		$updateStmt->condition('module_id', $module_id, '='); 	
 		$updateStmt->condition('group_id', 0, '='); 				
 		$updateStmt->fields(array(
 				'group_id'     =>  $lastinserid
 		));
 		expDebug::dPrintDBAPI(' update group id  = ', $updateStmt);
 		$numUpdated = $updateStmt->execute(); 
 		expDebug::dPrint("number update ".print_r($numUpdated,true),5);
 		if(!empty($numUpdated)){
 			// get last sequence number
			$select = db_select('slt_module_groups', 'sltmodule');
		 	$select->addExpression('COUNT(sltmodule.id)', 'countnewval');
		 	$select->condition('sltmodule.program_id', $program_id, '=');
		 	$select->condition('sltmodule.module_id', $module_id, '=');
		 	$select->condition('sltmodule.sequence', 9999, '!=');
		 	expDebug::dPrintDBAPI(' fetch group list all  = ', $select);
		 	$tpmoduleCount = $select->execute()->fetchField();
			expDebug::dPrint(' tp module count = ' . $tpmoduleCount , 3);
	      	
			$update = db_update("slt_module_groups");
			$update->fields(array('sequence'=>$tpmoduleCount+1));
			$update->condition('id', $lastinserid);
			$update->condition('sequence', 9999 , '=');
			expDebug::dPrintDBAPI('update module grp list ' , $update);
			$update->execute();
 		}
 	} catch (Exception $ex) {
    	watchdog_exception('getProgramEntityType', $ex);
    	expertusErrorThrow($ex);
  	}
}

/*function listOfGrpsProgram($module_id){
  try {
	  $selectStmt = db_select('slt_module_groups', 'groups');
	  $selectStmt->addField('mdl','group_list','group_list');
	  $selectStmt->condition('mdl.id',$module_id);
	  expDebug::dPrintDBAPI('listOfGrpsProgram',$selectStmt);
	  $result = $selectStmt->execute()->fetchField();
	  expDebug::dPrint('listOfGrpsProgram result'.print_r($result,true), 4);
	  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getProgramEntityType', $ex);
    expertusErrorThrow($ex);
  }
}*/
function getGroupCodeFromGrpTitle($module_id,$grpId){
  try {
	  $selectStmt = db_select('slt_module_crs_mapping', 'mcrs');
	  //$selectStmt->addField('mcrs','group_code','group_code');
	  $selectStmt->addField('mcrs','sequence','sequence');
	  $selectStmt->condition('mcrs.module_id',$module_id);
	  $selectStmt->condition('mcrs.group_id',$grpId);
	  $selectStmt->orderBy('id','DESC');
	  $selectStmt->range(0,1);
	  expDebug::dPrintDBAPI('getGroupCodeFromGrpTitle',$selectStmt);
	  $result = $selectStmt->execute()->fetchField();
	  expDebug::dPrint('getGroupCodeFromGrpTitle result'.print_r($result,true), 4);
	  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getProgramEntityType', $ex);
    expertusErrorThrow($ex);
  }	
}

function listOfgrpsUnderModule($module_id){
try {
	
	  $selectStmt = db_select('slt_module_groups', 'grp');
	  $selectStmt->addExpression('COUNT(1)','count');
	  $selectStmt->condition('grp.module_id',$module_id);
	  expDebug::dPrintDBAPI('listOfgrpsUnderModule',$selectStmt);
	  $result = $selectStmt->execute()->fetchField();
	  expDebug::dPrint('listOfgrpsUnderModule result'.print_r($result,true), 4);
	  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getProgramEntityType', $ex);
    expertusErrorThrow($ex);
  }
}
function getDefaultModuleId($prg_Id,$sequence = ''){
	$select = db_select('slt_module', 'sltmodule');
 	$select->addField('sltmodule','title','module_title');
 	$select->addField('sltmodule','id','module_id');
 	$select->condition('sltmodule.program_id', $prg_Id, '=');
 	if(!empty($sequence))
 		$select->condition('sltmodule.sequence', $sequence, '=');
 	expDebug::dPrintDBAPI('getDefaultModuleId',$select);
 	$select->range(0,1);
 	$tpcurrMod = $select->execute()->fetchAll();
 	return $tpcurrMod;
}
/*
 * function to delete modules and courses under the module
 * */
function deleteModuleTP($prg_Id,$module_id,$entity_type){
	try{
	    expDebug::dPrint('hello deleteModuleTP');
	    if($form_state['values']['create_user_id'])
	        $loggedInUserId = $form_state['values']['create_user_id'];
	    else
	        $loggedInUserId  = getIdOfLoggedInUser();
	
	//Remove courses under the module
	$select = db_select('slt_module_crs_mapping', 'crs');
 	$select->addField('crs','course_id','course_id');;
 	$select->condition('crs.program_id', $prg_Id, '=');
 	$select->condition('crs.module_id', $module_id, '=');
 	expDebug::dPrintDBAPI('get courses under the module',$select);
 	$crsIds = $select->execute()->fetchAll();
 	
 	foreach($crsIds as $crs){
 	    if($form_state['values']['create_user_id'])
 	        $loggedInUserId = $form_state['values']['create_user_id'];
 	    else
 	        $loggedInUserId  = getIdOfLoggedInUser();
 	     
 	    $delete_id = db_select('slt_module_crs_mapping', 'mcm');
 	    $delete_id->addField('mcm', 'id', 'id');
 	    $delete_id->addField('mcm', 'group_id', 'group_id');
 	    $delete_id->condition('program_id', $prg_Id,'=');
 	    $delete_id->condition('course_id', $crs->course_id,'=');
 	    $delete_id->condition('module_id', $module_id,'=');
 	    $del=$delete_id->execute()->fetchAll();
 	    expDebug::dPrintDBAPI('$delete_id', $del);
 	    
 	    		
		$deleteStmt = db_delete('slt_module_crs_mapping');
	    $deleteStmt->condition('program_id', $prg_Id,'=');
	    $deleteStmt->condition('module_id', $module_id,'=');
	    $deleteStmt->condition('course_id', $crs->course_id,'=');
	    expDebug::dPrintDBAPI("delete courses under the module" ,$deleteStmt);
	    $crsupd = $deleteStmt->execute();
	    
	    //Make delete log entries
	    foreach ($del as $val){
	        $fields = array(
	                'table_name' => 'slt_module_crs_mapping',
	                'entity_id' => $val->id,
	                'entity_type' => 'module_course_mapping',
	                'module_name' => 'exp_sp_administration_program',
	                'functionality_name' => 'deleteModuleTP',
	                'api_name' => null,
	                'deleted_on' => now(),
	                'deleted_by' => $loggedInUserId,
	                'parent1_entity_id' => $prg_Id,
	                'parent1_entity_type' => 'program_id',
	                'parent2_entity_id' => $module_id,
	                'parent2_entity_type' => 'module_id',
	                'parent3_entity_id' => $crs->course_id,
	                'parent3_entity_type' => 'course_id',
	                'parent4_entity_id' => $val->group_id,
	                'parent4_entity_type' => 'group_id',
	        );
	        deleted_log_entry($fields);
	    } expDebug::dPrint('slt_module_crs_mapping table log');
	      	    
		$deleteFormState = array('id');
		$deleteFormState['id'] = $crs;
	    make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$crs->course_id, 'Deleted',$entity_type, 'exp_sp_administration_program','Course deleted', array('id'));
 	}

 	//Remove survey/assessment associated to the module
 	$select = db_select('slt_survey_mapping', 'map');
 	$select->addField('map','id','id');
 	$select->addField('map','survey_id','survey_id');
 	$select->addField('map','object_id','object_id');
 	$select->addField('map','object_type','object_type');
 	$select->addField('map','module_id','module_id');
 	$select->condition('map.object_id', $prg_Id, '=');
 	$select->condition('map.module_id', $module_id, '=');
 	$select->condition('map.object_type', $entity_type, '=');
 	expDebug::dPrintDBAPI('get survey/assessment associated to the module',$select);
 	$sryIds = $select->execute()->fetchAll();
 	
 	$loggeduserId = getSltpersonUserId();
 	
 	foreach($sryIds as $sry){
 	    
		$deleteStmt = db_delete('slt_survey_mapping');
	    $deleteStmt->condition('object_id', $prg_Id,'=');
	    $deleteStmt->condition('object_type', $entity_type,'=');
	    $deleteStmt->condition('module_id', $module_id,'=');
	    $deleteStmt->condition('survey_id', $sry->survey_id,'=');
	    expDebug::dPrintDBAPI("delete survey/assessment associated to the module" ,$deleteStmt);
	    $sryupd = $deleteStmt->execute();
	    
	    if($sry->module_id == null){
	        $mod_id = 0;
	        $type = 'class';
	    }
	    else{
	        $mod_id = $sry->module_id;
	        $type = 'module_id';
	    }
	    // Make deleted log entry
	    $fields = array(
	            'table_name' => 'slt_survey_mapping',
	            'entity_id' => $sry->id,
	            'entity_type' => 'module_survey/assessment_mapping',
	            'parent1_entity_id' => $sry->survey_id,
	            'parent1_entity_type' => 'survey_id',
	            'parent2_entity_id' => $sry->object_id,
	            'parent2_entity_type' => $sry->object_type,
	            'parent3_entity_id' => $mod_id,
	            'parent3_entity_type' => $type,
	            'module_name' => 'exp_sp_administration_program',
	            'functionality_name' => 'deleteModuleTP',
	            'api_name' => $_REQUEST['apiname'],
	            'deleted_on' => now(),
	            'deleted_by' => $loggeduserId
	    );
	    deleted_log_entry($fields);
	    
		$deleteFormState = array('id');
		$deleteFormState['id'] = $sry;
	    make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$sry->survey_id, 'Deleted',$entity_type, 'exp_sp_administration_program','surveymappingdeleted', array('id'));
 	}
 	
 	//Delete groups associated to modules
 	if($form_state['values']['create_user_id'])
 	    $loggedInUserId = $form_state['values']['create_user_id'];
 	else
 	    $loggedInUserId  = getIdOfLoggedInUser();
 	
 	$delete_id = db_select('slt_module_groups', 'mg');
 	$delete_id->addField('mg', 'id', 'id');
 	$delete_id->condition('program_id', $prg_Id,'=');
 	$delete_id->condition('module_id', $module_id,'=');
 	$del=$delete_id->execute()->fetchAll();
 	expDebug::dPrintDBAPI('$delete_id', $del->id);
 	
 	
 	
 	$deleteStmt = db_delete('slt_module_groups');
    $deleteStmt->condition('program_id', $prg_Id,'=');
    $deleteStmt->condition('module_id', $module_id,'=');
    expDebug::dPrintDBAPI("delete Module from groups table" ,$deleteStmt);
    $modupd = $deleteStmt->execute();
 	
    //make delete log entries - change
    foreach ($del as $val){
        $fields = array(
                'table_name' => 'slt_module_groups', 
                'entity_id' => $val->id,
                'entity_type' => 'module_groups',
                'module_name' => 'exp_sp_administration_program',
                'functionality_name' => 'deleteModuleTP',
                'api_name' => null,
                'deleted_on' => now(),
                'deleted_by' => $loggedInUserId,
                'parent1_entity_id' => $prg_Id,
                'parent1_entity_type' => 'program_id',
                'parent2_entity_id' => $module_id,
                'parent2_entity_type' => 'module_id'
        );
    deleted_log_entry($fields);
    }expDebug::dPrint('slt_module_groups table log');
 	
    
	//Remove Module from module table
	$deleteStmt = db_delete('slt_module');
    $deleteStmt->condition('program_id', $prg_Id,'=');
    $deleteStmt->condition('id', $module_id,'=');
    expDebug::dPrintDBAPI("delete Module from module table" ,$deleteStmt);
    $modupd = $deleteStmt->execute();
    
    //Make delete log entries
    $fields = array(
            'table_name' => 'slt_module',
            'entity_id' => $module_id,
            'entity_type' => 'module',
            'module_name' => 'exp_sp_administration_program',
            'functionality_name' => 'deleteModuleTP',
            'api_name' => null,
            'deleted_on' => now(),
            'deleted_by' => $loggedInUserId,
            'parent1_entity_id' => $prg_Id,
            'parent1_entity_type' => 'program_id',
    );
    deleted_log_entry($fields);
    
    expDebug::dPrint('slt_module table log');
    //Don't delete this update this is report flat table related update.
    $update = db_update('slt_program');
    $update->condition('id',$prg_Id, '=');
    $update->fields(array(
    		'updated_on'     =>  now(),
    		'updated_by'	 => $loggedInUserId
    ));
    $update->execute();
    
    
    //Re-sequncing the modules in program
    $select = db_select('slt_module', 'module');
 	$select->addField('module','id');
 	$select->condition('module.program_id', $prg_Id, '=');
 	expDebug::dPrintDBAPI('get remaining module list associated to the Program',$select);
 	$select->orderBy('id','asc');
 	$modIds = $select->execute()->fetchAll();
 	expDebug::dPrint("module id list " .print_r($modIds,true),5);
 	$i = 1;
 	foreach($modIds as $id){
 		expDebug::dPrint(' ididid = '. print_r($id, true) , 4);
 		$update = db_update('slt_module');
 		$update->condition('id', $id->id, '='); 					
 		$update->fields(array(
 				'sequence'     =>  $i
 		));
 		$update->execute(); 
 		expDebug::dPrintDBAPI('update program sequence',$update);
 		$i++;
 	} 
    
	
	//Audit trail entry
	$deleteFormState = array('id');
	$deleteFormState['id'] = $modupd;
	make_audit_trail_entries($deleteFormState, getIdOfLoggedInUser() , null ,$module_id, 'Deleted',$entity_type, 'exp_sp_administration_program','moduledeleted', array('id'));
		
	 }catch (Exception $ex) {
	    watchdog_exception('deleteModuleTP', $ex);
	    expertusErrorThrow($ex);
  	}

}
/*
 * get enrollment cunt for each module
 * */
function getEnrollmentCnt($prgId,$moduleId='',$from = ''){
	try{
		$select = db_select('slt_master_enrollment', 'menr');
		$select->leftJoin('slt_module', 'mdl', 'menr.program_id = mdl.program_id AND mdl.sequence = menr.recertify_path');
		$select->addExpression('COUNT(1)','count');
		$select->condition('mdl.program_id', $prgId, '=');
		if(!empty($moduleId))
			$select->condition('mdl.id', $moduleId, '=');
		$select->condition('menr.overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_exp'), 'IN');
		expDebug::dPrintDBAPI("enrollment count for module" ,$select);
		$enrcnt = $select->execute()->fetchField();
		if($from == 'ajax'){
			echo drupal_json_output($enrcnt);
		}else{
			return $enrcnt;
		}
	}catch (Exception $ex) {
	    watchdog_exception('deleteModuleTP', $ex);
	    expertusErrorThrow($ex);
  	}
}
/*
 * survey mapped to module already or not
 * */
function getSurveyMappedOrNot($entityId,$entityType,$moduleId,$surveyId){
	try{
	    $select = db_select('slt_survey_mapping', 'map');
	    $select->innerJoin('slt_survey', 'sur', 'map.survey_id = sur.id');
		$select->addField('sur','title','survey_title');
		$select->condition('map.object_id', $entityId, '=');
		$select->condition('map.object_type', $entityType, '=');
		$select->condition('map.module_id', $moduleId, '=');
		$select->condition('map.survey_id', $surveyId, '=');
		expDebug::dPrintDBAPI("getSurveyMappedOrNot" ,$select);
		$survey = $select->execute()->fetchField();
		return $survey;
	}catch (Exception $ex) {
	    watchdog_exception('getSurveyMappedOrNot', $ex);
	    expertusErrorThrow($ex);
  	}
}
?>