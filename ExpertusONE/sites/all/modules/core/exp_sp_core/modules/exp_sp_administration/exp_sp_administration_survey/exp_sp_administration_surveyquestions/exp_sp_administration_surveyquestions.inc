<?php


/**
 * Survey Questions Admin inc file
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
 * getSurveyQuestionsTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */

function getSurveyQuestionsTabContentType(){
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
  return $jsObjectInfo;
}

/**
 * getSurveyQuestionsLanguage() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix and
 * 								Fetch all related survey questions language.The result is ordered by name.
 */
function getSurveyQuestionsLanguage($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  try{
  // Select the table
  $select = db_select('slt_survey_questions','surveyquestions');

  //Distinct the Resultset
  $select->distinct();

  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');

  // Left join the slt_profile_list_items to get language names
  $select->leftJoin('slt_profile_list_items', 'spli', 'surveyquestions.lang_code = spli.code');

  // Set the conditions
  $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');

  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  $select->condition(db_and()->condition('surveyquestions.status', 'sry_qtn_sts_del', '!='));

  if (!$alsoFetchInactiveItems) {
  	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }

  // Order results by name
  $select->orderBy('name');

  expDebug::dPrintDBAPI('$select', $select);

  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);

  // Return results
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionsLanguage', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * surveyQuestionsNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function surveyQuestionsNarrowSearchFiltersHTML($argumenType) {
  try{
  $checkedStatus = array();
  //$checkedStatus["sry_qtn_sts_atv"] = TRUE;

  $allStatus = getProfileItemNamesKeyed('sry_qtn_sts_');

  expDebug::dPrint('$allStatus = ' . print_r($allStatus, true), 4);

  $allLang = getSurveyQuestionsLanguage('cre_sys_lng_');
  expDebug::dPrint('$allLang = ' . print_r($allLang, true), 4);

  $allQuestionType = getFilterQuestionType('sry_qtn_typ_','cre_sys_lng_eng',false,$argumenType);
  expDebug::dPrint('$allQuestionType = ' . print_r($allQuestionType, true), 4);

  $renderfiltersets=array();

  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveyquestionsstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus,
                                                           		 'checked_list' => $checkedStatus)
                                                          );


  if(count($allLang) > 1){
   $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveyquestionslang',
                                                                 'title' => t('LBL038'),
                                                           		 'divid' => 'language',
                                                                 'checkboxes_list' => $allLang)
                                                          );
  }

  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveyquestionstype',
                                                                 'title' => t('LBL327'),
                                                                 'divid' => 'questions_type',
                                                                'checkboxes_list' => $allQuestionType)
                                                          );
 if($argumenType=='assessment'){
 $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'attachedassessmentquestions',
                                                               		 'title' => t('Assessment'),
                                                                     'default_text'=> t("LBL545")
                                                                     )
                                                              );
     $narrowfilters =  array();
     $narrowfilters[]  = array("type"=> "addltext", "code"=>"attachedassessmentquestions",
                                    "acpath" => "administration/learning/admin/assessment/autocomplete", "defaultText" => t("LBL545"));
     /* Ticket: 46992 */
     $weightedSurveyTags = getWeightedTags(array('sry_det_typ_ass_qus'));
     if (!empty($weightedSurveyTags)) {
     	$renderfiltersets[]=theme('narrow_search_filterset_tags',
     			array('code' => 'surveyquestionstag',
     					'html_id' => 'assessmentquestionstag_container',
     					'title' => t('LBL191'), //Tags
     					'tags_for_cloud' => $weightedSurveyTags
     			)
     	);
     }
     if (!empty($weightedSurveyTags)) {
     	$narrowfilters[] = array("type" => "tagscloud", "code" => "surveyquestionstag");
     }/* Ticket: 46992 */

 }else{
   $renderfiltersets[]=theme('narrow_search_filterset_addltext',
                                                               array('code' => 'attachedsurveyquestions',
                                                               		 'title' => t('Survey'),
                                                                     'default_text'=> t("LBL545")
                                                                     )
                                                              );
   $narrowfilters[]  = array("type"=> "addltext", "code"=>"attachedsurveyquestions",
                                    "acpath" => "administration/learning/admin/survey/autocomplete", "defaultText" => t("LBL545"));
/* Ticket: 46992 */
   $weightedSurveyTags = getWeightedTags(array('sry_det_typ_sry_qus'));
   if (!empty($weightedSurveyTags)) {
   	$renderfiltersets[]=theme('narrow_search_filterset_tags',
   			array('code' => 'surveyquestionstag',
   					'html_id' => 'surveyquestionstag_container',
   					'title' => t('LBL191'), //Tags
   					'tags_for_cloud' => $weightedSurveyTags
   			)
   	);
   }

   if (!empty($weightedSurveyTags)) {
   	$narrowfilters[] = array("type" => "tagscloud", "code" => "surveyquestionstag");
   }/* Ticket: 46992 */
 }


      $narrowfilters[] = array('type'=>'checkbox','code'=>'surveyquestionsstatus');
      $narrowfilters[] = array('type'=>'checkbox','code'=>'surveyquestionslang');
      $narrowfilters[] = array('type'=>'checkbox','code'=>'surveyquestionstype');



  /*$filterSetforJS=array(array('type'=>'checkbox','code'=>'surveyquestionsstatus'),
                        array('type'=>'checkbox','code'=>'surveyquestionslang'),
                        array('type'=>'checkbox','code'=>'surveyquestionstype'),
                        array("type"=> "addltext", "code"=>"attachedassessmentquestions",
                                    "acpath" => "administration/learning/admin/assessment/autocomplete", "defaultText" => t("Type a assessment name")),
                       array("type"=> "addltext", "code"=>"attachedsurveyquestions",
                                    "acpath" => "administration/learning/admin/survey/autocomplete", "defaultText" => t("Type a survey name")),
                       );*/

    $filterSetforJS =  $narrowfilters;

  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS),
                     ));
  }catch (Exception $ex) {
    watchdog_exception('surveyQuestionsNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * surveyQuestionsSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function surveyQuestionsSearchResultsSortbarHTML() {
  try{
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'surveyquestions-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'surveyquestions-sort-za');
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'surveyquestions-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'surveyquestions-sort-new'
                              );

  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  }catch (Exception $ex) {
    watchdog_exception('surveyQuestionsSearchResultsSortbarHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * surveyQuestionsSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function surveyQuestionsSearchResultsActionbarHTML($argumentType) {
  try{
  $type = $argumentType == 'assessmentquestions' ? 'assessment' : 'survey';
  if($argumentType == 'assessmentquestions'){
    $actionbarLinksListforJS=array('export_path'=>'administration/assessment/assessmentquestions/export/',
  					             'print_path'=>'administration/assessment/assessmentquestions/print/'
                              );
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL388'),
												 'add_page_path' => 'administration/assessment/assessmentquestions/nojs/addedit',
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
  											 'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
                         'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  											 'page_code' => 'sry_det_typ_ass_qus',
               )
         );
  }else{
  $actionbarLinksListforJS=array('export_path'=>'administration/survey/surveyquestions/export/',
  					             'print_path'=>'administration/survey/surveyquestions/print/'
                              );
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL388'),
												 'add_page_path' => 'administration/survey/surveyquestions/nojs/addedit',
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
  											 'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
                         'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  											 'page_code' => 'sry_det_typ_sry_qus',
               )
         );
  }

  }catch (Exception $ex) {
    watchdog_exception('surveyQuestionsSearchResultsActionbarHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getSurveyQuestionsTabContentMainHTML - Returns the html for tab content main
 */
function getSurveyQuestionsTabContentMainHTML($argumentType) {
  try{
  $tabContentTypeArr =  getSurveyQuestionsTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    if($argumentType=='assessmentquestions'){

      $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/assessment/assessmentquestions/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/assessment/assessmentquestions/textfilter-autocomplete'
                                          );
    }else{
      $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/survey/surveyquestions/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/survey/surveyquestions/textfilter-autocomplete'
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
    watchdog_exception('getSurveyQuestionsTabContentMainHTML', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getSurveyQuestionsTabContent - Returns the html for narrow search block on survey questions search page
 * 									  - Prepares JSON response for Survey Questions.
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Expert Settings.
 */
function getSurveyQuestionsTabContent($argumentType) {
  try{
  $tabContentTypeArr =  getSurveyQuestionsTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  $argumentypeMenu = ($argumentType == 'surveyquestions') ? 'survey' : 'assessment';
  $surveyQuestionsResponse=array(
     'rendered_tab_content_type' => $tabContentType,
     'rendered_narrow_search_filters' => surveyQuestionsNarrowSearchFiltersHTML($argumentypeMenu),
     'rendered_narrow_search_sortbar' => surveyQuestionsSearchResultsSortbarHTML(),
     'rendered_narrow_search_actionbar' => surveyQuestionsSearchResultsActionbarHTML($argumentType),
     'rendered_tab_content_main'=>getSurveyQuestionsTabContentMainHTML($argumentType),
     'rendered_main_div' => getAdminModules('administration', $argumentypeMenu),
  );
  echo drupal_json_encode($surveyQuestionsResponse);
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionsTabContent', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getSurveyQuestionsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.
 */
function getSurveyQuestionsHTML($surveyquestions, $argumentType) {
  try{
  $questionList = array();
 /* if (!empty($surveyquestions->code)) {
    $questionList[] = array('detail' => $surveyquestions->code, 'tooltip' => t('LBL096'));
  }*/
  if (!empty($surveyquestions->language)) {
    //$langchar = substr($surveyquestions->language, 0, 3);
    $questionList[] = array('detail' => t($surveyquestions->language), 'tooltip' => t('LBL038'));
  }
 /* if (!empty($surveyquestions->status)) {
    $questionList[] = array('detail' => $surveyquestions->status, 'tooltip' => t('LBL102'));
  }*/
 if (!empty($surveyquestions->question_type)) {

    $questionList[] = array('detail' => t($surveyquestions->question_type), 'tooltip' => t('LBL242'));
  }

  $type = $argumentType == 'assessmentquestions' ? 'assessment' : 'survey';

   $actionList[] = array('action_link_text' => t('LBL063'),
  					  'action_page_path' => 'administration/'.$type.'/'.$argumentType.'/nojs/addedit',
					  'action_button_params' => $surveyquestions->id,
  					  'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );

   $actionList[] = array('action_link_text' => t('LBL286'),
  					  'action_page_path' => '',
					  'action_button_params' => $surveyquestions->id,
                      'action_button_param_text' => 'SurAssQues',
  					  'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );
	   if(strtolower($surveyquestions->status_code) == 'sry_qtn_sts_atv') {
			$newActionList[] = array('action_link_text' => t('LBL571'),
	    				  		'action_page_path' => '',
	    				  		'action_button_params' => $surveyquestions->id,
	        					'action_button_params_text' => 'deactivate',
	    					    'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
	    					    'js_object_info' => array (
	                                                'name' => 'narrowsearch', // provide the java script object name
	                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	                                         ),
	                        	'tooltip' => t('LBL571')
	    				  );
	 	} else {
	 		$newActionList[] = array('action_link_text' => t('LBL570'),
	    				  		'action_page_path' => '',
	    				  		'action_button_params' => $surveyquestions->id,
	        					'action_button_params_text' => 'activate',
	    					    'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
	    					    'js_object_info' => array (
	                                                'name' => 'narrowsearch', // provide the java script object name
	                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	                                         ),
	                        	'tooltip' => t('LBL570')
	    				  );

	 	}
	 	if($type == 'survey'){
	 		$surveytype = 'sry_det_typ_sry_qus';
          $surveyDeleteInfo = t('MSG357').' '.strtolower(t('LBL323'));
          $objectType = 'surveyQuestion';
	 	} else {
	 		$surveytype = 'sry_det_typ_ass_qus';
	 	  $surveyDeleteInfo = t('MSG357').' '.strtolower(t('LBL626'));
	 	  $objectType = 'assessmentQuestion';
	 	}

	 	$newActionList1[] = array('action_link_text' => t('LBL642'),
	 			'action_page_path' => '',
	 			'action_button_params' =>  $surveyquestions->id,
	 			'action_button_params_text' => 'access',
	 			'ctools_style' => 'ctools-modal-ctools-admin-questions-addedit-style',
	 			'js_object_info' => array (
	 					'name' => 'narrowsearch', // provide the java script object name
	 					'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	 			),
	 			'tooltip' => t('LBL642'),
	 			'action_org_users_count'=> $surveyquestions->id,
	 			'entityId' =>  $surveyquestions->id,
	 			'entityType' => $surveytype,
	 			'icon_style_class'=>'access-tab-icon',
	 			'postype' => 'topleft',
	 			'poslwid' => 40,
	 	);
	 	if($type == 'survey'){
	 		$surveytype = 'sry_det_typ_sry';
	 		$questionType = 'sry_det_typ_sry_qus';
	 	} else {
	 		$surveytype = 'sry_det_typ_ass';
	 		$questionType = 'sry_det_typ_ass_qus';
	 	}

	 	$viewQuestionList[] = array('action_link_text' => 'SurAssQues',
	 			'action_page_path' => 'administration/'.$type.'/'.$argumentType.'/view-question/nojs/addedit/'.$surveyquestions->id.'/'.$surveytype.'/'.$questionType.'',
	 			'action_button_params' => $surveyquestions->id,
	 			'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
	 			'js_object_info' => array (
	 					'name' => 'narrowsearch', // provide the java script object name
	 					'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	 			),
	 	);
	 	expDebug::dPrint('$actionList-->'.print_r($actionList,true),5);

  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($surveyquestions->question_txt),
                       'delete_info' => $surveyDeleteInfo,
                       'details_list' => $questionList,
                       'new_action_list'=>$newActionList1,
                 	     'action_list'=> $actionList,
                 	     'object_type'=> $objectType,
                 		   'sumedit' => $surveyquestions->sumedit,
                 		   'sumdelete' => $surveyquestions->sumdelete,
                 		   'view_list'=>$viewQuestionList
                      )
                );
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionsHTML', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getSurveyQuestionsActionsHTML() - Renders a survey detail's actions as HTML to be displayed in jqGrid.
 */
function getSurveyQuestionsActionsHTML($surveydetails) {
 /* return theme('narrow-search-results-item-actions',
                   array('edit_page_path' => '',
                         'edit_button_action_params' => array($surveydetails->id)
                        )
              );*/

   return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $surveydetails)
              );
}

/**
 * surveyQuestionsSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function surveyQuestionsSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
  try{
  switch (getURLParam('sortby')) {
    case "AZ" :
                $field = $tableAlias . ".question_txt";
                $direction = "ASC";
                break;

    case "ZA" :
                $field = $tableAlias . ".question_txt";
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
    watchdog_exception('surveyQuestionsSearchSelectStmtAddOrderByClause', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * sendSurveyQuestionsListToJqGrid() - Prepares JSON response to JqGrid's request for Survey Details
 */
function sendSurveyQuestionsListToJqGrid($argumentType) {
  try{
  expDebug::dPrint( 'page = "' . $_GET['page'] . '" ,' .
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
                    'npage = "' . $_GET['npage'], 4);

  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];

  /* $numSurveyQuestions = fetchSurveyQuestionsAll('COUNT', '', '', $argumentType);
  expDebug::dPrint('$numSurveyQuestions = ' . print_r($numSurveyQuestions, true), 4);

  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numSurveyQuestions); */
  
  $start = calculate_offset($page, $limit);
  expDebug::dPrint('$start = ' . print_r($start, true), 4);

  $surveyQuestionsList = fetchSurveyQuestionsAll('LISTS', $start, $limit, $argumentType);

  expDebug::dPrint('$surveyQuestionsList = ' . print_r($surveyQuestionsList, true), 4);

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numSurveyQuestions;

 // $jsObjectInfo = surveyQuestionsJSObjectInfo();
 // $response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
                                             // have to do this.
  $response->initial_sort_type_html_id = 'surveyquestions-sort-new';

  $i=0;
  foreach ($surveyQuestionsList as $surveyquestions) {
    expDebug::dPrint('$surveyquestions = '. print_r($surveyquestions, true), 4);
    //53640: Survey/Assessment questions list page showing - empty row at the last
    if(is_object($surveyquestions)){
    $surveyquestions->row =  $i;
    $response->rows[$i]['id']   = $surveyquestions->id;

    $response->rows[$i]['cell'] = array('image' => theme('narrow-search-results-item-image', array('results' => $surveyquestions)),
                                         'details' => getSurveyQuestionsHTML($surveyquestions, $argumentType),
                                         'action' => getSurveyQuestionsActionsHTML($surveyquestions)
                                         );

    $i++;
  }
  }

  expDebug::dPrint('$response = '. print_r($response, true), 4);

  echo drupal_json_encode($response);
  }catch (Exception $ex) {
    watchdog_exception('sendSurveyQuestionsListToJqGrid', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getSurveyQuestionsTextFilterAutoComplete() - Returns the matched survey details title
 */
function getSurveyQuestionsTextFilterAutoComplete() {
  try{
  $searchText =  $_GET['z'];
  $entType = (arg(2) == 'assessmentquestions') ? 'sry_det_typ_ass_qus' : 'sry_det_typ_sry_qus';

  // Select the table
  $select = db_select('slt_survey_questions', 'surveyquestions');
  $select->leftjoin('slt_tag_entity', 'te', "surveyquestions.id=te.entity_id and te.entity_type= '$entType'");
  $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');

  // Add fields to fetch
   $select->addField('surveyquestions', 'question_txt', 'question_txt');
  // $select->addField('td', 'tagname', 'tagname');

 // selectAddNarrowSearchByTextMultipleORCondition($select, "surveyquestions", array("question_txt", "code"), "z");
   $tableAliasFields = array('surveyquestions.question_txt','surveyquestions.code','td.tagname');
   SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);

   $select->condition('surveyquestions.status',array('sry_qtn_sts_atv','sry_qtn_sts_itv'),'IN');
   if($entType == 'sry_det_typ_sry_qus') {
 	$select->condition('surveyquestions.survey_type','sry_det_typ_sry');
   } else if($entType == 'sry_det_typ_ass_qus') {
 	$select->condition('surveyquestions.survey_type','sry_det_typ_ass');
   }
    filterByUserAccessPrivileges($select,'surveyquestions','',1,$entType,'attach','surveyquestions'); // #46933 - group access visiblity check
    
   //groupby id for duplicate entries. #71622
  $select->groupBy('surveyquestions.id');
  // Order in dictionary sort order
  $select->orderBy('question_txt');
  

  expDebug::dPrintDBAPI('$select', $select);

  // print_r($select->__toString());
  $matchingSurveyQuestionsNames = $select->execute()->fetchAll();
  expDebug::dPrint('$matchingSurveyQuestionsNames = ' . print_r($matchingSurveyQuestionsNames, true) , 3);

  foreach ($matchingSurveyQuestionsNames as $surveyQuestionsName) {
    print rawurlencode($surveyQuestionsName->question_txt) . "\n";
  }
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionsTextFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getSurveyQuestionsQuestionTextFilterAutoComplete() - Returns the matched survey questions title
 */
function getSurveyQuestionsQuestionTextFilterAutoComplete() {
  try{
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_survey_questions', 'surveyquestion');

  // Add fields to fetch
  $select->addField('surveyquestion', 'question_txt', 'question');

  // Add condition
  $select->condition('surveyquestion.question_txt', '%' . db_like($searchText) . '%', 'LIKE');


  // Order in dictionary sort order
  $select->orderBy('question');

  expDebug::dPrintDBAPI('$select', $select);

  $matchingSurveyDetailsQuestionNames = $select->execute()->fetchAll();
  expDebug::dPrint('$matchingSurveyDetailsQuestionNames = '. print_r($matchingSurveyDetailsQuestionNames, true) , 3);

  foreach ($matchingSurveyDetailsQuestionNames as $surveyDetailsQuestionName) {
    print $surveyDetailsQuestionName->question . "\n";
  }
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionsQuestionTextFilterAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * fetchSurveyQuestionsAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchSurveyQuestionsAll($op, $start, $limit, $argumentType = '') {
  try{
  // Select table
  $select = db_select('slt_survey_questions', 'surveyquestions');
  $entType = ($argumentType == 'assessmentquestions') ? 'sry_det_typ_ass_qus' : 'sry_det_typ_sry_qus';
  // Left join the users, slt_survey_groups_questions and slt_survey_questions table to get the survey details
  //$select->leftJoin('slt_survey_groups_questions', 'surveygroup', 'surveydetails.id = surveygroup.survey_id');
 // $select->leftJoin('slt_survey_questions', 'surveyquestion', 'surveygroup.question_id = surveyquestion.id');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=surveyquestions.id AND group_map.entity_type = \''.$entType.'\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$entType.'\'');   */
  $select->leftJoin('slt_profile_list_items', 'plilang', 'surveyquestions.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'surveyquestions.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'pliquestions', 'surveyquestions.question_type = pliquestions.code');
  $select->leftjoin('slt_tag_entity', 'te', "surveyquestions.id=te.entity_id and te.entity_type= '$entType'");
  $select->leftjoin('slt_tagdefn', 'td', 'td.id=te.tagid');
  $select->condition('surveyquestions.status', array('sry_qtn_sts_del','sry_qtn_sts_dft'), 'NOT IN');
  if($op=='COUNT'){
     // Get count of active survey
     $select->addExpression('COUNT(DISTINCT(surveyquestions.id))', 'COUNT');
  }
  else{
  	$select->distinct();
    // Select fields/expression values to be fetched from the database.
  //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
  //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
	$select->addField('surveyquestions', 'id', 'id');
	$select->addField('surveyquestions', 'question_txt', 'question_txt');
	$select->addField('surveyquestions', 'code', 'code');
	$select->addField('surveyquestions', 'status', 'status_code');
	$select->addField('plilang', 'name', 'language');
	$select->addField('plistatus', 'name', 'status');
	$select->addField('pliquestions', 'name', 'question_type');
	//$select->addField('td', 'tagname', 'tagname');
	$select->addExpression('\'SurveyAssessmentQuestions\'', 'object_type');
	$select->groupBy('surveyquestions.id');
  }
  if($_REQUEST['apiname']=='ListSurveyQuestionAPI' || $_REQUEST['apiname']=='ListAssessmentQuestionAPI') {
      $select->addExpression("(select GROUP_CONCAT(def.tagname) FROM slt_tag_entity ent LEFT OUTER JOIN slt_tagdefn def on def.id = ent.tagid where (surveyquestions.id=ent.entity_id and ent.entity_type='$entType'))","tagname");
      $tag=trim($_REQUEST['tagname']);
      if(isset($tag) && !empty($tag)){
        $select->condition('td.tagname', $tag,'=');
      }
  }
  filterByUserAccessPrivileges($select, 'surveyquestions','',1,$entType,'','surveyquestions');

  if(!empty($argumentType)){
    $type = $argumentType == 'assessmentquestions' ? 'sry_det_typ_ass' : 'sry_det_typ_sry';
    $select->condition('surveyquestions.survey_type', $type);
  }
/* Ticket: 46992 */
  if(!empty($_GET['surveyquestionstag'])){
  	$tagsearch = $_GET['surveyquestionstag'];
  //	$select->innerJoin('slt_tag_entity', 'ste', 'ste.entity_id = surveyquestions.id');
  //	$select->innerJoin('slt_tagdefn', 'stf', 'stf.id = ste.tagid');
  	$select->condition('td.tagname',rawurldecode($tagsearch),'=');
  	$select->condition('te.entity_type', $entType, '=');
  }
  /* Ticket: 46992 */
    $pdfTitleText = t('Assessment');
    $surveyTypeText = t('Assessment');
    $surveySearchURLParam = 'attachedassessmentquestions';
    if ($argumentType == 'surveyquestions') {
    	$pdfTitleText = t('Survey');
    	$surveyTypeText = t('Survey');
    	$surveySearchURLParam = 'attachedsurveyquestions';
    }
    $pdfTitleText = $pdfTitleText .  ' ' . t('LBL240');
  // Set the conditions
   //$defaultQuestionStatus = isset($_GET['surveydetailsstatus']) ? '' : 'sry_qtn_sts_atv';
   if(isset($_GET['attachedassessmentquestions']) && !empty($_GET['attachedassessmentquestions'])){
    $select->leftJoin('slt_survey_groups_questions', 'maps', 'maps.question_id = surveyquestions.id');
    $select->leftJoin('slt_survey', 'surv', 'surv.id = maps.survey_id AND surv.status!=\'sry_det_sry_del\'');
  	selectAddNarrowSearchByTextMultipleORCondition($select, 'surv', array("title", "code"), 'attachedassessmentquestions', t('LBL545'));
    }
   if(isset($_GET['attachedsurveyquestions']) && !empty($_GET['attachedsurveyquestions'])){
    $select->leftJoin('slt_survey_groups_questions', 'maps', 'maps.question_id = surveyquestions.id');
    $select->leftJoin('slt_survey', 'sur', 'sur.id = maps.survey_id AND sur.status!=\'sry_det_sry_del\'');
  	selectAddNarrowSearchByTextMultipleORCondition($select, 'sur', array("title", "code"), 'attachedsurveyquestions', t('LBL545'));
    }
   selectAddNarrowSearchINCondition($select, 'surveyquestions', 'status', 'surveyquestionsstatus');
   selectAddNarrowSearchINCondition($select, 'surveyquestions', 'lang_code', 'surveyquestionslang');
   selectAddNarrowSearchINCondition($select, 'surveyquestions', 'question_type', 'surveyquestionstype');
    if(isset($_GET['textfilter']) && !empty($_GET['textfilter'])){
      	    $searchText = $_GET['textfilter'];
        //  selectAddNarrowSearchByTextMultipleORCondition($select, "surveyquestions", array("question_txt", "code"), "textfilter"); search by title in the header
      		$tableAliasFields = array('surveyquestions.question_txt','surveyquestions.code','td.tagname');
  			SearchByFilterForMultipleTableORCondition($select, $searchText,'Search',$tableAliasFields);
      }


  if($op != "COUNT"){
    // Order the records
    surveyQuestionsSearchSelectStmtAddOrderByClause($select, 'surveyquestions', "AZ");
  }

  expDebug::dPrintDBAPI('$select first', $select);

  if($op == "COUNT"){
  	$result = $select->execute()->fetchField();
	  expDebug::dPrint('COUNT $result = ' . print_r($result, true) , 3);
	return $result;
  }

  elseif($op == "CSV"){
	  $colHeadersMap = array(t('LBL325') => 'question_txt',
	                         t('LBL036') => 'question_type',
                             t('LBL038') => 'language',
                             t('LBL102') => 'status'
  					   );
	$filterArray[] = array(t('LBL325').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('surveyquestionsstatus'));
    $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('surveyquestionslang'));
    $filterArray[] = array(t('LBL036').':'.checkboxFiltersetSelectionsToPrintableStr('surveyquestionstype'));
    $filterArray[] = array($surveyTypeText .':'.textboxFiltersetValueToPrintableStr($surveySearchURLParam, t('LBL545')));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());

  	SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray);
  } else if($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL325') => array('question_txt',  35),
    					   t('LBL036') => array('question_type', 15),
                           t('LBL038') => array('language',      20),
    					   t('LBL102') => array('status',        15)
                      );

    $appliedFilters = array(t('LBL325') 		 => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') 		 => checkboxFiltersetSelectionsToPrintableStr('surveyquestionsstatus'),
    						t('LBL038') 		 => checkboxFiltersetSelectionsToPrintableStr('surveyquestionslang'),
    						t('LBL036') 		 => checkboxFiltersetSelectionsToPrintableStr('surveyquestionstype'),
                            $surveyTypeText 	 => textboxFiltersetValueToPrintableStr($surveySearchURLParam, t('LBL545')),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
                        );
    SendNarrowSearchResultsAsPDFFile($select, $pdfTitleText, $appliedFilters, $colHeadersMap);

  } else{

        if(!empty($argumentType)){
          $type = $argumentType == 'assessmentquestions' ? 'sry_det_typ_ass' : 'sry_det_typ_sry';
          $select->condition('surveyquestions.survey_type', $type);
        }
        $totalRecords = $select->countQuery()->execute()->fetchField();
        add_row_id_condition($select, 'surveyquestions');
        
	     // Limit to $limit records
	     if(is_numeric($start) && is_numeric($limit))
	     	$select->range($start, $limit);
	     expDebug::dPrintDBAPI('$select with range', $select);
	    // Execure the query and fetch all the records
	    $result = $select->execute()->fetchAll();
	    $result['totalrow'] = $totalRecords;
	    expDebug::dPrint('LIST $result = ' . print_r($result, true) , 3);

	    return $result;
  }
  }catch (Exception $ex) {
    watchdog_exception('fetchSurveyQuestionsAll', $ex);
    expertusErrorThrow($ex);
  }
}

function getFilterQuestionType($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false, $argumenType = 'survey') {
  try{
  // Select the table
  $select = db_select('slt_profile_list_items','spli');

  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');

  // Order results by name
  $select->orderBy('name');

  $select->condition('spli.code', array('sry_qtn_typ_img'), 'NOT IN');

  // Set the conditions
  $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  if (!$alsoFetchInactiveItems) {
  	$select->condition(db_and()->condition('spli.is_active', 'Y', '='));
  }

  if($argumenType != 'survey') {
    $select->condition(db_and()->condition('spli.code', array('sry_qtn_typ_dpn', 'sry_qtn_typ_mch', 'sry_qtn_typ_trf', 'sry_qtn_typ_yno'), 'IN'));
  }
  expDebug::dPrintDBAPI('$select', $select);

  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);

  // Return results
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getFilterQuestionType', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the survey assessment
 */
function publishOrUnpublishSurveyAssessmentQuestions($questionId ){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    $status  = getSurveyAssesmentQuestionStatus($questionId);
    $updateStmt = db_update('slt_survey_questions');
    $updatedstatus  = ($status=='sry_qtn_sts_atv')?"sry_qtn_sts_itv":"sry_qtn_sts_atv";

    $updateStmt->condition('id', $questionId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $updateStmt->execute();
	/* Update tag activity Count */
	$activity_count_etype = (getSurveyAssesmentQuestionSurveyType($questionId)=='sry_det_typ_ass')?"sry_det_typ_ass_qus":"sry_det_typ_sry_qus";
	updateTagActivityCount($questionId,$activity_count_etype,$updatedstatus);		
	/* Update tag activity Count */
    add_audit_trail_entry($currUserId, $questionId, 'cre_sys_obt_sur', 'exp_sp_administration_survey', 'publishOrUnpublishSurveyAssessmentQuestions', 'Updated Survey/Assessment Status',
		                                     null, $status, $updatedstatus);

	$returnStatus  = ($updatedstatus=='sry_qtn_sts_atv')?t('LBL571'):t('LBL570');
	//print $returnStatus;
	print $updatedstatus.'|'.$returnStatus;
  }
  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('publishOrUnpublishSurveyAssessmentQuestions', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of the program
 */
function getSurveyAssesmentQuestionStatus($questionId){
  try{
  $select = db_select('slt_survey_questions', 'qus');
  $select->addField('qus','status', 'status');
  $select->condition('qus.id',$questionId);
  expDebug::dPrintDBAPI('$select', $select);
  // Execute the query
  $qusStatus = $select->execute()->fetchField();
  expDebug::dPrint('$qusStatus = ' . $qusStatus , 3);
  return $qusStatus;
  }catch (Exception $ex) {
    watchdog_exception('getSurveyAssesmentQuestionStatus', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * fucntion to retrive the question type.
 */
function getSurveyAssesmentQuestionSurveyType($questionId){
  try{
  $select = db_select('slt_survey_questions', 'qus');
  $select->addField('qus','survey_type', 'survey_type');
  $select->condition('qus.id',$questionId);
  expDebug::dPrintDBAPI('$select', $select);
  // Execute the query
  $qusStatus = $select->execute()->fetchField();
  return $qusStatus;
  }catch (Exception $ex) {
    watchdog_exception('getSurveyAssesmentQuestionSurveyType', $ex);
    expertusErrorThrow($ex);
  }
}
function listSurveyQuestionsApi( $start, $limit, $userid="")
{
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
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$totalQuestions = fetchSurveyQuestionsAll('COUNT', '', '', 'sry_det_typ_sry');
	$surveyQuestions = fetchSurveyQuestionsAll('',$start,$limit,'sry_det_typ_sry');
	$surveyQuestions['totalrow'] = $totalQuestions;
	return $surveyQuestions;
}

function listAssessmentQuestionsApi( $start, $limit, $userid="")
{
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
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	return fetchSurveyQuestionsAll('',$start,$limit,'assessmentquestions');
}

function exp_sp_administration_viewquestion_addedit_form($js = NULL,$mode ,$entityId, $type, $questionType){
	try{
		global $theme_key;
		$css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
		if($theme_key == 'expertusoneV2') {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning_v2.css', $css_theme_optional);
		}else{
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.css', $css_theme_optional);
		}
		//CUSTOM SCROLLBAR "JSscrollbar"
		drupal_add_js(drupal_get_path('module', 'exp_sp_lnrreports') .'/jscrollbar/jquery.jscrollpane.js', $js_module_optional);

		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 630,
								'minHeight' =>200,
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
			$argsList[] = $type;
			$argsList[] = $questionType;
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_view_questiondetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewquestion_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_questiondetails_addedit_form($form, &$form_state,$renderMode, $entityId, $type, $questionType){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$courseId---->'.$type);
		expDebug::dPrint('$$$questionType---->'.$questionType);

		$wrapperId = 'viewquestion-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		 $form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
	  $quesDetails = getQuestionDetailsForView($entityId, $type);

		expDebug::dPrint('$$coursedetails-->'.print_r($quesDetails,true),5);
		$form['narrow_questionview'] = array(
				'#markup'       => theme('narrow_questionview', array('results' => $quesDetails,
																															 'type' => $type,
																																'questiontype' => $questionType)),
		);
		 $form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);


		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_questiondetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getQuestionDetailsForView($entityId, $type){
	try{
		expDebug::dPrint('$$courseId111---->'.$type);
		$selectStmt = db_select('slt_survey_questions', 'ques');
		$selectStmt->leftJoin('slt_profile_list_items', 'prof', 'ques.lang_code = prof.code');
		$selectStmt->leftJoin('slt_profile_list_items', 'spli', 'ques.question_type = spli.code');
		$selectStmt->addField('ques','question_txt', 'title');
		$selectStmt->addField('ques','id','id');
		$selectStmt->addField('prof','name','lang');
		$selectStmt->addField('spli','name', 'type');
		$selectStmt->addField('ques','answer_choice_txt', 'answer');
		$selectStmt->addField('ques','right_answer', 'correct');
		$selectStmt->condition('ques.id', $entityId, '=');
		$selectStmt->condition('ques.survey_type', $type, '=');
		expDebug::dPrintDBAPI('$selectStmt$selectStmt->',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('getSurveyName--->'.print_r($result,true), 4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getQuestionDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}
?>