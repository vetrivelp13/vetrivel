<?php
	

/**
 * Survey Details Admin inc file
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
 * getSurveyDetailsTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								   - If tab content type is narrow search then filters and text filters , Jggrid will be initialise or else it consider as a open page.
 */
 
function getSurveyDetailsTabContentType(){
	try{
  $jsObjectInfo['tab_content_type']='narrow_search'; /* Modules is wheather contain the narrow search result  or some other result like commerce */
   expDebug::dPrint(' $getSurveyDetailsTabContentType = ' . print_r($jsObjectInfo, true) , 4);
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
} 

/**
 * getSurveyDetailsLanguage() - Function to fetch all profile list item names keyed by code whose code is prefixed as $codePrefix and
 * 								Fetch all related survey details language.The result is ordered by name.
 */
function getSurveyDetailsLanguage($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
  try{
  // Select the table
  $select = db_select('slt_survey','surveydetails');  
  
  //Distinct the Resultset
  $select->distinct();
  
  // Select the fields to fetch
  $select->addField('spli','code', 'code');
  $select->addField('spli','name', 'name');
 
  // Left join the slt_profile_list_items to get language names
  $select->leftJoin('slt_profile_list_items', 'spli', 'surveydetails.lang_code = spli.code');   
  
  // Set the conditions
  $select->condition('spli.code', db_like($codePrefix) . '%', 'LIKE');
  
  $select->condition(db_and()->condition('spli.lang_code', $langCode, '='));
  $select->condition(db_and()->condition('surveydetails.status', 'sry_det_sry_del', '!='));
  
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
  }catch (Exception $ex) {
    watchdog_exception('getSurveyDetailsLanguage', $ex);
    expertusErrorThrow($ex);
  }  
}

/*
 * surveyDetailsNarrowSearchFiltersHTML - Returns the html for narrow search filters 
 */
function surveyDetailsNarrowSearchFiltersHTML($argumentype) {
  try{
 // $jsObjectInfo = surveyDetailsJSObjectInfo();
   if($argumentype=='assessment'){
     
     $titleName=strtoupper(t('Assessment'));
   }else{
     $titleName=strtoupper(t('Survey'));
   }
  $checkedStatus = array();  
  
 // $checkedStatus["sry_det_sry_atv"] = TRUE;
  
 // $allStatus = getProfileItemNamesKeyed('sry_det_sry_');
  $allStatus = getStatusSurveyAssessmentFilter('sry_det_sry_');
  
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4); 

  $allLang = getSurveyDetailsLanguage('cre_sys_lng_');
  expDebug::dPrint(' $allLang = ' . print_r($allLang, true) , 4); 
  
  $allType =  getProfileItemNamesKeyed('sry_det_typ_');
  expDebug::dPrint(' $allType = ' . print_r($allType, true) , 4);
  
  $renderfiltersets=array();
 // Comment the Type filter search box 
 /* $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveytype',
                                                                 'title' => t('LBL036'),
                                                                 'checkboxes_list' => $allType)
                                                          );
  */
  $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveydetailsstatus',
                                                                 'title' => t('LBL102'),
                                                           		 'divid' => 'status',
                                                                 'checkboxes_list' => $allStatus,
                                                            	'checked_list' => $checkedStatus)
                                                          );
 
  
  if(count($allLang) > 1){
   $renderfiltersets[]= theme('narrow_search_filterset_checkboxes',
                                                           array('code' => 'surveydetailslang',
                                                                 'title' => t('LBL038'),
                                                           		 'divid' => 'language',
                                                                 'checkboxes_list' => $allLang)
                                                          );
  }
  
  $renderfiltersets[]= theme('narrow_search_filterset_addltext',
                                                           array('code' => 'surveyquestion',
                                                                 'title' =>t('LBL325'),//$titleName,                                                           
                                                           		 'default_text'=>t('LBL324'))
                                                          );

  
  if($argumentype=='assessment'){ 
  	
    $acPath = 'administration/assessment/assessmentdetails/assessment-question-autocomplete';
  }
  else{  	
    $acPath = 'administration/survey/surveydetails/survey-question-autocomplete';
  }                                                         
  $filterSetforJS=array(array('type'=>'checkbox','code'=>'surveydetailsstatus'),
  									array('type'=>'checkbox','code'=>'surveytype'),
                                    array('type'=>'checkbox','code'=>'surveydetailslang'),
                                    array('type'=>'addltext','code'=>'surveyquestion','acpath'=>$acPath,'defaultText'=>t('LBL324')),
                       );
                                                        
  return theme('narrow_search_filters_holder',
               array('rendered_filtersets_list' => $renderfiltersets,
                      'filter_set_list' => drupal_json_encode($filterSetforJS), 
                     ));  
  }catch (Exception $ex) {
    watchdog_exception('surveyDetailsNarrowSearchFiltersHTML', $ex);
    expertusErrorThrow($ex);
  }
  
}

/*
 * surveyDetailsSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function surveyDetailsSearchResultsSortbarHTML() {
  try{
  //$jsObjectInfo = surveyDetailsJSObjectInfo();
  
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => t('LBL017'), 'sort_type' => 'AZ', 'html_id' => 'surveydetails-sort-az');
  $sortbarLinksList[] = array('title' => t('LBL018'), 'sort_type' => 'ZA', 'html_id' => 'surveydetails-sort-za'); 
  $sortbarLinksList[] = array('title' => t('LBL044'), 'sort_type' => 'NewlyListed', 'html_id' => 'surveydetails-sort-new');
    
  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'surveydetails-sort-new'
                              );   
  
  return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
                                              )
              );
  }catch (Exception $ex) {
    watchdog_exception('surveyDetailsSearchResultsSortbarHTML', $ex);
    expertusErrorThrow($ex);
  }
} 

/*
 * surveyDetailsSearchResultsActionbarHTML - Returns the html for search results action bar 
 */
function surveyDetailsSearchResultsActionbarHTML($argumentype) { 
  try{ 
  if($argumentype=='assessment'){
    $actionbarLinksListforJS=array('export_path'=>'administration/assessment/assessmentdetails/export',
  					              'print_path'=>'administration/assessment/assessmentdetails/print'
                              );   
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL601'),
												 'add_page_path' => 'administration/assessment/assessmentdetails/nojs/addedit',
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
                         'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
                         'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  											 'page_code' => 'sry_det_typ_ass',
               )
         ); 
    
  }
  else{
  $actionbarLinksListforJS=array('export_path'=>'administration/survey/surveydetails/export/',
  					             'print_path'=>'administration/survey/surveydetails/print/'
                              );   
  return theme('narrow_search_actionbar', array( 'add_button_title' => t('LBL600'),
												 'add_page_path' => 'administration/survey/surveydetails/nojs/addedit',
												 'show_print_button'=>true,
												 'show_export_button'=>true,
												 'show_add_button'=>true,
                         'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
                         'actionbar_list'=> drupal_json_encode($actionbarLinksListforJS),
  		                   'page_code' => 'sry_det_typ_sry',
               )
         ); 
         
  }
        /* $actionbarLinksListforJS=array('export_path'=>'administration/learning/catalog/export/',
  					             'print_path'=>'administration/learning/catalog/print/'
                              );  */
  }catch (Exception $ex) {
    watchdog_exception('surveyDetailsSearchResultsActionbarHTML', $ex);
    expertusErrorThrow($ex);
  }
         
}

/*
 * getSurveyDetailsTabContentMainHTML - Returns the html for tab content main 
 */
function getSurveyDetailsTabContentMainHTML($argumentype) {
	try{
  expDebug::dPrint(' getSurveyDetailsTabContent argument type= ' . $argumentype , 4);
       
  $tabContentTypeArr =  getSurveyDetailsTabContentType();  
  $tabContentType=$tabContentTypeArr['tab_content_type'];   
  if($tabContentType=="narrow_search"){
    if($argumentype=='assessment'){
      
     $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/assessment/assessmentdetails/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/assessment/assessmentdetails/textfilter-autocomplete'                                            
                                          );  
    }else{
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/survey/suryveydetails/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/survey/surveydetails/textfilter-autocomplete'                                            
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
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}
 
/*
 * getSurveyDetailsTabContent		  - Returns the html for narrow search block on survey details search page
 * 									  - Prepares JSON response for Survey Details.
 * 									  -	This response contains the Filters , Auto Complete Text Box, JQgrid Results,Print and Expert Settings.
 */
function getSurveyDetailsTabContent($argumentype) { 
	try {
  expDebug::dPrint(' getSurveyDetailsTabContent argument type= ' . $argumentype , 4);
  $tabContentTypeArr =  getSurveyDetailsTabContentType();  
  $tabContentType=$tabContentTypeArr['tab_content_type'];  
  //$argumentypeMenu = ($argumentype == 'survey') ? 'surveydetails' : 'assessment';
  $surveyDetailsResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => surveyDetailsNarrowSearchFiltersHTML($argumentype),
    'rendered_narrow_search_sortbar' => surveyDetailsSearchResultsSortbarHTML(), 
    'rendered_narrow_search_actionbar' => surveyDetailsSearchResultsActionbarHTML($argumentype),
    'rendered_tab_content_main'=>getSurveyDetailsTabContentMainHTML($argumentype), 
    'rendered_main_div' => getAdminModules('administration',$argumentype),
  ); 
  echo drupal_json_encode($surveyDetailsResponse);
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getChildSurveyDetailsQuestionCount() - Function to get the count of the questions(active only) under Survey Details
 * SELECT * FROM `slt_survey_groups_questions`  AS sgq LEFT JOIN  `slt_survey_questions` AS sq ON sgq.question_id=sq.id WHERE sq.status='sry_qtn_sts_atv' AND sgq.survey_id=4;
 */
function getChildSurveyDetailsQuestionCount($surveydetailsId) {
	try {
  // Select slt_survey_groups_questions table
  $select = db_select('slt_survey_groups_questions', 'surveygroup');
  
  // Get count of active Questions
  $select->addExpression('COUNT(surveygroup.id)', 'count');
  // Uncommented for #0045977
  $select->leftJoin('slt_survey_questions', 'surveyquestion', 'surveygroup.question_id=surveyquestion.id');
   
  // Set the conditions
  $select->condition('surveygroup.survey_id', $surveydetailsId, '=');
  // Uncommented for #0045977
  $select->condition('surveyquestion.status', 'sry_qtn_sts_atv', '=');

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
 * getSurveyDetailsHTML() - Renders a survey detail as HTML to be displayed in jqGrid.  
 */
function getSurveyDetailsHTML($surveydetails, $argumentType) { 
  try {
  include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
  $detailsList = array();
  $actionList = array();
  $newActionList = array();
  expDebug::dPrint('chekcing the survey details');
  /*if (!empty($surveydetails->type)) {
    $detailsList[] = array('detail' => $surveydetails->type, 'tooltip' => t('LBL036'));
  } */ 	
  if (!empty($surveydetails->code)) {
    $detailsList[] = array('detail' => sanitize_data($surveydetails->code), 'tooltip' => t('LBL096').': '.sanitize_data($surveydetails->code));
  }  
  if (!empty($surveydetails->language)) {
    //$langchar = substr($surveydetails->language, 0, 3);
    $detailsList[] = array('detail' => t($surveydetails->language), 'tooltip' => t('LBL038').': '.t($surveydetails->language));
  }	
  /*if (!empty($surveydetails->status)) {
    $detailsList[] = array('detail' => $surveydetails->status, 'tooltip' => t('LBL102'));
  }	*/
  $surveyDetailsQuestions=getChildSurveyDetailsQuestionCount($surveydetails->id);

    $detailsList[] = array('detail' => t('LBL241').': '." ".$surveyDetailsQuestions, 'tooltip' => t('LBL240'));

  if($argumentType=='assessment'){
    
     $actionList[] = array('action_link_text' => t('LBL063'),
					  //'action_page_path' => 'administration/learning/course-class/nojs/addedit', 
  					  'action_page_path' => 'administration/assessment/assessmentdetails/nojs/addedit',
					  'action_button_params' => $surveydetails->id,
  					  //'action_button_params_child' => $catalog->class_id,
  					  'action_button_params_module' => $argumentType,
  					  'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  }else{
  $actionList[] = array('action_link_text' => t('LBL063'),
					  //'action_page_path' => 'administration/learning/course-class/nojs/addedit', 
  					  'action_page_path' => 'administration/survey/surveydetails/nojs/addedit',
					  'action_button_params' => $surveydetails->id,
  					  //'action_button_params_child' => $catalog->class_id,
  					  'action_button_params_module' => $argumentType,
  					  'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL063')
					  );
  }					    
  $actionList[] = array('action_link_text' => t('LBL286'),
  					  'action_page_path' => '',
					  'action_button_params' => $surveydetails->id,
  					  'action_button_surveyassement_id' => $surveydetails->id,
                      'action_button_param_text' => 'SurAss',
  					  'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  					  'js_object_info' => array (
                                              'name' => 'narrowsearch', // provide the java script object name
                                              'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
                                       ),
                      'tooltip' => t('LBL286')
					  );    
 /*$unpublishStatus = getSurveyAssessmentDeleteAccess($surveydetails->id);
   if(!$unpublishStatus) {
	   if(strtolower($surveydetails->status_code) == 'sry_det_sry_atv') {
			$newActionList[] = array('action_link_text' => t('LBL571'),
	    				  		'action_page_path' => '', 
	    				  		'action_button_params' => $surveydetails->id,
	        					'action_button_params_text' => 'deactivate',
								'action_button_params_module' => $argumentType,
	    					    'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
	    					    'js_object_info' => array (
	                                                'name' => 'narrowsearch', // provide the java script object name
	                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	                                         ),
	                        	'tooltip' => t('LBL571')
	    				  );	
	 	} elseif(strtolower($surveydetails->status_code) == 'sry_det_sry_itv') {
	 		$newActionList[] = array('action_link_text' => t('LBL570'),
	    				  		'action_page_path' => '', 
	    				  		'action_button_params' => $surveydetails->id,
	        					'action_button_params_text' => 'activate',
	 							'action_button_params_module' => $argumentType,
	    					    'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
	    					    'js_object_info' => array (
	                                                'name' => 'narrowsearch', // provide the java script object name
	                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	                                         ),
	                        	'tooltip' => t('LBL570')
	    				  );	
	 		
	 	} 
	 	
   }
	else {
	 		$newActionList[] = array('action_link_text' => t('LBL577'),
	    				  		'action_page_path' => '', 
	    				  		'action_button_params' => $surveydetails->id,
	        					'action_button_params_text' => 'published',
	 		                    'action_button_param_text' => 'banner',
	    					    'ctools_style' => 'ctools-modal-ctools-admin-survey-addedit-style',
	    					    'js_object_info' => array (
	                                                'name' => 'narrowsearch', // provide the java script object name
	                                                'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	                                         ),
	                        	'tooltip' => t('LBL577')
	    				  );	
	 		}*/
  
 
  if($argumentType == 'assessment'){
  	$surveytype='sry_det_typ_ass';
    $objectType = 'assessmentDetails';
  }
  else{
  	$surveytype='sry_det_typ_sry';
    $objectType = 'surveyDetails';
  }
  $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => $surveydetails->id,
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'userId'=> $user->id,
  		'entityId' => $surveydetails->id,
  		'entityType' => $surveytype,
  		'icon_style_class'=>'access-tab-icon',
  		'postype' => 'topleft',
  		'poslwid' => 45,
  );
  if($argumentType=='assessment'){
	  $viewsSurAssessList[] = array('action_link_text' => 'Surass',
	  		'action_page_path' => 'administration/assessment/assessmentdetails/view-assess/nojs/addedit/'.$surveydetails->id.'/'.$surveytype.'',
	  		'action_button_params' => $surveydetails->id,
	  		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
	  		'js_object_info' => array (
	  				'name' => 'narrowsearch', // provide the java script object name
	  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
	  		),
	  );
  }else{
  	$viewsSurAssessList[] = array('action_link_text' => 'Surass',
  			'action_page_path' => 'administration/survey/surveydetails/view-survey/nojs/addedit/'.$surveydetails->id.'/'.$surveytype.'',
  			'action_button_params' => $surveydetails->id,
  			'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  			'js_object_info' => array (
  					'name' => 'narrowsearch', // provide the java script object name
  					'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  			),
  	);
  }				 
  return theme('narrow-search-results-item-details',
                 array('title' => filterSpecialCharacters($surveydetails->title),                        
					             'delete_info' => t('MSG357').' '.$argumentType,                       
					             'details_list' => $detailsList,
                       'action_list'=> $actionList,
                       'new_action_list'=>$newActionList,
                       'description'=> trim($surveydetails->short_description),
                       'object_type'=> $objectType,
                 		   'sumedit' => $surveydetails->sumedit,
                 		   'sumdelete' => $surveydetails->sumdelete,
                 		   'view_list'=> $viewsSurAssessList
                      )
                );
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getSurveyDetailsActionsHTML() - Renders a survey detail's actions as HTML to be displayed in jqGrid.  
 */
function getSurveyDetailsActionsHTML($surveydetails) {
 /* return theme('narrow-search-results-item-actions',
                   array('edit_page_path' => '',
                         'edit_button_action_params' => array($surveydetails->id)
                        )
              );*/
	try{
  return theme('narrow-search-results-item-actions',
                   array('catalog_details' => $surveydetails)
              );  
} catch (Exception $ex) {
	watchdog_exception('getSurveyDetailsActionsHTML', $ex);
	expertusErrorThrow($ex);
}
}
 
/**
 * surveyDetailsSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function surveyDetailsSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try{
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
  	watchdog_exception('surveyDetailsSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendSurveyDetailsListToJqGrid() - Prepares JSON response to JqGrid's request for Survey Details
 */
function sendSurveyDetailsListToJqGrid($argumentType) {
  try {
  expDebug::dPrint(' fetchSurveyDetailsAll() : ' .
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
  											'argumentType = "' . $argumentType . '" ,' .
                        'npage = "' . $_GET['npage'] , 4
                        
  );
  
  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];

/*   $numSurveyDetails = fetchSurveyDetailsAll($argumentType, 'COUNT');
   
  expDebug::dPrint(' $numSurveyDetails = ' . print_r($numSurveyDetails, true) , 4);
  
  $total_pages = 0;
  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numSurveyDetails); */
  $start = calculate_offset($page, $limit);
  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
  
  
  $surveyDetailsList = fetchSurveyDetailsAll($argumentType, 'LISTS', $start, $limit);
  expDebug::dPrint(' $surveyDetailsList = ' . print_r($surveyDetailsList, true) , 4);  

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numSurveyDetails;
  
 // $jsObjectInfo = surveyDetailsJSObjectInfo();
 // $response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
                                             // have to do this.
  $response->initial_sort_type_html_id = 'surveydetails-sort-new'; 
    
  $i=0;
  foreach ($surveyDetailsList as $surveydetails) {
    expDebug::dPrint(' $surveydetails = '. print_r($surveydetails, true) , 4);

    $surveydetails->survey_type = $argumentType;
    
    $surveydetails->row =  $i;
    $response->rows[$i]['id']   = $surveydetails->id;
    
    $response->rows[$i]['cell'] = array( 'image' => theme('narrow-search-results-item-image', array('results' => $surveydetails)),
                                         'details' => getSurveyDetailsHTML($surveydetails, $argumentType),
                                         'action' => getSurveyDetailsActionsHTML($surveydetails)
                                         );

    $i++;    
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);
  
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('sendSurveyDetailsListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getSurveyDetailsTextFilterAutoComplete() - Returns the matched survey details title
 */
function getSurveyDetailsTextFilterAutoComplete($argumentType) {
  try{
  if($argumentType=='assessment'){
     $surveytype='sry_det_typ_ass';
  }else {
     $surveytype='sry_det_typ_sry';
  }
 
  $searchText =  $_GET['z'];

  // Select the table
  $select = db_select('slt_survey', 'surveydetails');

  // Add fields to fetch
  $select->addExpression('DISTINCT(surveydetails.title)', 'title');
  $select->condition('surveydetails.type', $surveytype, '=');
  $select->condition('surveydetails.status','sry_det_sry_del', '!=');
  					
  selectAddNarrowSearchByTextMultipleORCondition($select, "surveydetails", array("title", "code", "description", "short_description"), "z");

  filterByUserAccessPrivileges($select,'surveydetails','',1,$surveytype,'attach','surveydetails'); // #46933 - group access visiblity check
  // Order in dictionary sort order
  $select->orderBy('title'); 

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // print_r($select->__toString());
  $matchingSurveyDetailsNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingSurveyDetailsNames = ' . print_r($matchingSurveyDetailsNames, true) , 3);

  foreach ($matchingSurveyDetailsNames as $surveyDetailsName) {
    print rawurlencode($surveyDetailsName->title) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getSurveyDetailsQuestionTextFilterAutoComplete() - Returns the matched survey questions title
 */
function getSurveyDetailsQuestionTextFilterAutoComplete($argumentType) {
	try {
  $searchText =  $_GET['z'];
  if($argumentType=='assessment'){
     $surveytype='sry_det_typ_ass';
  }else {
     $surveytype='sry_det_typ_sry';
  }
  // Select the table
  $select = db_select('slt_survey_questions', 'surveyquestion');

  // Add fields to fetch
  $select->addField('surveyquestion', 'question_txt', 'question');
  
  // Add condition
  //$select->condition('surveyquestion.question_txt', '%' . db_like($searchText) . '%', 'LIKE');
  //$select->condition('surveyquestion.survey_type', $surveytype, '=');
  
  selectAddNarrowSearchByTextMultipleORCondition($select, "surveyquestion", array("question_txt", "code"), "z");

  $select->condition('surveyquestion.status',array('sry_qtn_sts_atv','sry_qtn_sts_itv'),'IN');
  $entType = (arg(2) == 'assessmentdetails') ? 'sry_det_typ_ass_qus' : 'sry_det_typ_sry_qus';
	if($entType == 'sry_det_typ_sry_qus') {
		$select->condition('surveyquestion.survey_type','sry_det_typ_sry');
	} else if($entType == 'sry_det_typ_ass_qus') {
		$select->condition('surveyquestion.survey_type','sry_det_typ_ass');
	}
    filterByUserAccessPrivileges($select,'surveyquestion','',1,$entType,'attach','surveyquestion');

  // Order in dictionary sort order
  $select->orderBy('question');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  $matchingSurveyDetailsQuestionNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingSurveyDetailsQuestionNames = '. print_r($matchingSurveyDetailsQuestionNames, true) , 3);

  foreach ($matchingSurveyDetailsQuestionNames as $surveyDetailsQuestionName) {
    print $surveyDetailsQuestionName->question . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyDetailsQuestionTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}
 
/*
 * fetchSurveyDetailsAll() - Fetches survey details count / listing for jqgrid / listing for FILE after applying the filters.
 */
function fetchSurveyDetailsAll($argumentType=NULL, $op=NULL, $start=NULL, $limit=NULL) { 
 try { 
  expDebug::dPrint('Argument type = ' . print_r($argumentType, true) , 4);
  if($argumentType=='assessment'){
     $surveytype='sry_det_typ_ass';
     $pdfTitleText=t('Assessment');
  }else {
     $surveytype='sry_det_typ_sry';
     $pdfTitleText=t('Survey');
  }
  // Select table
  $select = db_select('slt_survey', 'surveydetails');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=surveydetails.id AND group_map.entity_type =\''.$surveytype.'\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \''.$surveytype.'\''); */
  // Left join the users, slt_survey_groups_questions and slt_survey_questions table to get the survey details
  $select->leftJoin('slt_survey_groups_questions', 'surveygroup', 'surveydetails.id = surveygroup.survey_id');
  $select->leftJoin('slt_survey_questions', 'surveyquestion', 'surveygroup.question_id = surveyquestion.id');
  $select->leftJoin('slt_profile_list_items', 'pliType', 'surveydetails.type = pliType.code AND pliType.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plilang', 'surveydetails.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_profile_list_items', 'plistatus', 'surveydetails.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\' ');
  $select->condition('surveydetails.type', $surveytype, '=');
    // Delete entries should not display
  $select->condition('surveydetails.status','sry_det_sry_del', '!=');
 /// $select->condition('pliType.is_active', 'Y');
  if($op=='COUNT'){
     // Get count of active survey
     $select->addExpression('COUNT(DISTINCT(surveydetails.id))', 'COUNT');
  }
  else{
  	$select->distinct(); 
    // Select fields/expression values to be fetched from the database.
  //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
  //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
	$select->addField('surveydetails', 'id', 'id');
	$select->addField('surveydetails', 'title', 'title');
	$select->addField('surveydetails', 'short_description', 'short_description');
	//$select->addField('surveydetails', 'status', 'status');
	$select->addField('pliType', 'name', 'type');	
	$select->addField('surveydetails', 'code', 'code'); 
	$select->addField('plilang', 'name', 'language');
	$select->addField('plistatus', 'name', 'status');    
	$select->addField('surveydetails', 'status', 'status_code');
	$select->groupBy('surveydetails.id');
  } 
  filterByUserAccessPrivileges($select, 'surveydetails','',1,$surveytype,'','surveydetails');
  // Set the conditions
  //$defaultSurveyStatus = isset($_GET['surveydetailsstatus']) ? '' : 'sry_det_sry_atv';
  
  selectAddNarrowSearchINCondition($select, 'surveydetails', 'status', 'surveydetailsstatus'); 
  selectAddNarrowSearchINCondition($select, 'surveydetails', 'lang_code', 'surveydetailslang');
  selectAddNarrowSearchINCondition($select, 'surveydetails', 'type', 'surveytype');
  selectAddNarrowSearchByTextMultipleORCondition($select, "surveydetails", array("title", "code", "description", "short_description"), "textfilter");
  
  selectAddNarrowSearchByTextCondition($select, 'surveyquestion', 'question_txt', 'surveyquestion', t("LBL324"));
  
  if($op != "COUNT"){
    // Order the records
    surveyDetailsSearchSelectStmtAddOrderByClause($select, 'surveydetails', "AZ"); 
  }
  
  expDebug::dPrintDBAPI(' $select obj = ' , $select);  
    
  if($op == "COUNT"){  
  	$result = $select->execute()->fetchField();  
	expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);
	return $result;
  }
  elseif ($op == "CSV") {
	  $colHeadersMap = array(t('LBL083') => 'title',
                           t('LBL036') => 'type',
						   t('LBL096') => 'code',
                           t('LBL038') => 'language',
                           t('LBL102') => 'status'
   					   );
	  
    $filterArray[] = array($pdfTitleText.':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('surveydetailsstatus'));
    $filterArray[] = array(t('LBL038').':'.checkboxFiltersetSelectionsToPrintableStr('surveydetailslang'));                            
    $filterArray[] = array(t('LBL325').':'.textboxFiltersetValueToPrintableStr('surveyquestion', t('LBL324')));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
    
  	SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'survery_details_admin_search');
  }
  else if ($op == "PDF") {
    // % width of the columns must total to 100%
    $colHeadersMap = array(t('LBL083') => array('title',           48),
    					   t('LBL036') => array('type',            15),
                           t('LBL096') => array('code',            10),
                           t('LBL038') => array('language',        15),
    					   t('LBL102') => array('status',          12)
                       );

    $appliedFilters = array($pdfTitleText => textboxFiltersetValueToPrintableStr('textfilter'),
                            t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('surveydetailsstatus'),
    						t('LBL038') => checkboxFiltersetSelectionsToPrintableStr('surveydetailslang'),                            
                            t('LBL325') => textboxFiltersetValueToPrintableStr('surveyquestion', t('LBL324')),
                            ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    );

    SendNarrowSearchResultsAsPDFFile($select, $pdfTitleText, $appliedFilters, $colHeadersMap, 'export', 11, 'P', 'survery_details_admin_search');
  }
  else{
  	// Limit to $limit records
  	if(is_numeric($start) && is_numeric($limit))
  		$select->range($start, $limit);
  	// Execure the query and fetch all the records
	add_row_id_condition($select, 'surveydetails');
  	$result = $select->execute()->fetchAll();
  	expDebug::dPrintDBAPI(' $select sest = ' , $select);
  	expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 3);
  	return $result;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchSurveyDetailsAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_column_headers_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function survery_details_admin_search_csv_column_headers_alter($columnHeaderMap) {
  try {
    $columnHeaderMap[0][t('LBL240')] = 'surveyQuestions';
  }
  catch (Exception $ex) {
    watchdog_exception('survery_details_admin_search_csv_column_headers_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function survery_details_admin_search_csv_row_data_alter($rowData) {
  try {
  	// Appending the Active Question Counts Under Survey Details
    $rowData[0]->surveyQuestions = getChildSurveyDetailsQuestionCount($rowData[0]->id);
  }
  catch (Exception $ex) {
    watchdog_exception('survery_details_admin_search_csv_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_column_headers_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function survery_details_admin_search_pdf_column_headers_alter($columnHeaderMap) {
  try {
    // % width of the columns must total to 100%
    $columnHeaderMap[0][t('LBL083')] = array('title', 33);
    $columnHeaderMap[0][t('LBL240')] = array('surveyQuestions', 15);
  }
  catch (Exception $ex) {
    watchdog_exception('survery_details_admin_search_pdf_column_headers_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Custom hook_pdf_row_data_alter function
 * Called in SendNarrowSearchResultsAsPDFFile()
 */
function survery_details_admin_search_pdf_row_data_alter($rowData) {
  try {
    // Appending the Active Question Counts Under Survey Details
    $rowData[0]->surveyQuestions = getChildSurveyDetailsQuestionCount($rowData[0]->id);
  }
  catch (Exception $ex) {
    watchdog_exception('survery_details_admin_search_pdf_row_data_alter', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * function to publish and unpublish the survey assessment
 */
function publishOrUnpublishSurveyAssessment($surveyId, $surveyType ){
  $currUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    $status  = getSurveyAssesmentStatus($surveyId);

    $updateStmt = db_update('slt_survey');
    $updatedstatus  = ($status=='sry_det_sry_atv')?"sry_det_sry_itv":"sry_det_sry_atv";  
    $updateStmt->condition('id', $surveyId);
    $updateStmt->fields(array(
                        'status'      => $updatedstatus,                                                   
                        'updated_on'    => now(),
                        'updated_by'    => $currUserId
                        ));
    $updateStmt->execute();
   
    add_audit_trail_entry($currUserId, $surveyId, 'cre_sys_obt_sur', 'exp_sp_administration_survey', 'publishOrUnpublishSurveyAssesment', 'Updated Survey/Assessment Status',
		                                     null, $status, $updatedstatus);
	
	$returnStatus  = ($updatedstatus=='sry_det_sry_atv')?t('LBL571'):t('LBL570');

	print $updatedstatus.'|'.$returnStatus;
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('publishOrUnpublishSurveyAssessment', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of the program
 */
function getSurveyAssesmentStatus($surveyId){
	try{
  $select = db_select('slt_survey', 'surv');
  $select->addField('surv','status', 'status');
  $select->condition('surv.id',$surveyId);
  // Execute the query
  $survStatus = $select->execute()->fetchField();
  return $survStatus; 
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyAssesmentStatus', $ex);
  	expertusErrorThrow($ex);
  } 
}

function getStatusSurveyAssessmentFilter($codePrefix, $langCode = 'cre_sys_lng_eng', $alsoFetchInactiveItems = false) {
	try{
  $select = db_query('SELECT code, name FROM slt_profile_list_items WHERE code like \''.$codePrefix.'%\' AND is_active = \'Y\'  AND code NOT IN (\'sry_det_sry_del\')ORDER BY FIELD(CODE,\'sry_det_sry_atv\',\'sry_det_sry_itv\')');
  $result = $select->fetchAllKeyed();
  
  // Return results
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getStatusSurveyAssessmentFilter', $ex);
  	expertusErrorThrow($ex);
  }
}

function attachQuestionDetails($entityId,$entityType){
try{
   $form_state = array(
   'no_redirect'  => TRUE, // To confirm whether necessary
   'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
   'entity_id'    => $entityId,
   'entity_type'  => $entityType
  );      
  
  $formId                 = 'addquestions_addedit_form_html';
  $builtForm              = drupal_build_form($formId, $form_state,$entityId,$entityType);
  $renderedTabContentHTML = drupal_render($builtForm);   
  $drupalSettings         = getDrupalSettings();

  $commerceResponse = array(
    'drupal_settings'     => $drupalSettings,
    'render_content_main' => $renderedTabContentHTML,
  ); 
 
  echo drupal_json_output($commerceResponse);
  exit; 
  } catch (Exception $ex) {
  	watchdog_exception('attachQuestionDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addquestions_addedit_form_html($form,&$form_state) {
	try{
	  include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
      return displayAttachQuestionsDetails($form,$form_state,$form_state['entity_id'], $form_state['entity_type']);
  } catch (Exception $ex) {
  	watchdog_exception('addquestions_addedit_form_html', $ex);
  	expertusErrorThrow($ex);
  }
}

function addquestions_addedit_form_html_validate($form, &$form_state) {
	try {
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment.inc');
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_surveydetails.inc');
	$entityId = $form_state['entity_id'];
  $entityType = $form_state['entity_type'];
  $uniqueId = "SurAssAttachQuestion-$entityId-$entityType";
  if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
  	form_set_error('',t('ERR106'));
    $form_state['esign_show_popup']= false;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addquestions_addedit_form_html_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function listSurveyByRestAPI( $start, $limit, $userid="") 
{
	try {
	$form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$result = fetchSurveyDetailsAll('','',$start,$limit);
	$result['totalrow'] = fetchSurveyDetailsAll('', 'COUNT');
	return $result;
	} catch (Exception $ex) {
		watchdog_exception('listSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function listAssessmentByRestAPI( $start, $limit, $userid="") 
{
	try {
	$form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$result = fetchSurveyDetailsAll('assessment','',$start,$limit);
	$result['totalrow'] = fetchSurveyDetailsAll('assessment', 'COUNT');
	return $result;
	} catch (Exception $ex) {
		watchdog_exception('listAssessmentByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_viewsurassess_addedit_form($js = NULL,$mode ,$entityId, $type){
	try{
		includeFilesForsurvey();
		expDebug::dPrint('sabbeeee-->'.$entityId);
		expDebug::dPrint('2333333-->'.$type);
		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 680,
								'minHeight' =>300,
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
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_view_surassessdetails_addedit_form', 'title' => (t('LBL816'))) //LBL816
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
			//exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewcourse_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewsurassess_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_surassessdetails_addedit_form($form, &$form_state,$renderMode, $entityId, $type){
	try {
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$$courseId---->'.$type);

		$wrapperId = 'viewsurass-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);
		$form['view-scroll-start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="view-scroll-wrapper">'
		);
		$surAssDetails = getSurAssDetailsForView($entityId, $type);

		expDebug::dPrint('$$$surAssDetails-->'.print_r($surAssDetails,true),5);
		$form['narrow_surveyassess_view'] = array(
				'#markup'       => theme('narrow_surveyassess_view', array('results' => $surAssDetails,
																																	  'type' => $type)),
		);
		$form['view-scroll-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		);


		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_surassessdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getSurAssDetailsForView($entityId, $type) {
	try {
		expDebug::dPrint('$$courseId111---->'.$type);
		$selectStmt = db_select('slt_survey', 'sur');
		$selectStmt->leftJoin('slt_profile_list_items', 'prof', 'sur.lang_code = prof.code');
		$selectStmt->leftjoin('slt_profile_list_items', 'proflist', 'sur.random = proflist.code');
		$selectStmt->addField('sur','id','id');
		$selectStmt->addField('sur','title','title');
		$selectStmt->addField('sur','code','code');
		$selectStmt->addField('prof','name','lang');
		$selectStmt->addField('sur','short_description','descrip');
		$selectStmt->addField('sur','question_per_page','perpage');
		$selectStmt->addField('sur','min_mark','min');
		$selectStmt->addField('sur','max_mark','max');
		$selectStmt->addField('proflist','name','random');
		$selectStmt->condition('sur.id', $entityId, '=');
		$selectStmt->condition('sur.type', $type, '=');
		expDebug::dPrintDBAPI('$selectStmt$selectStmt->',$selectStmt);
		$result = $selectStmt->execute()->fetchAll();
		expDebug::dPrint('getSurveyName--->'.print_r($result,true), 4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getSurAssDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}

function getQuestionsDetails($entityId){
	try{
		$select = db_select("slt_survey_groups_questions","sgq");
		$select->innerJoin('slt_survey', 'sy', 'sy.id = sgq.survey_id');
		$select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
		$select->leftjoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
		$select->addField('qst','question_txt','question_txt');
		$select->addField('qst','id','survey_question_id');
		$select->addField('sgq','id','survey_group_question_id');
		$select->addField('sgq','sequence','survey_group_sequence');
		$select->addField('sgq','mandatoryOption','mandatory_option');
		$select->addField('sgq','score','survey_score');
		$select->addField('sg','id','survey_group_id');
		$select->addField('sg','title','survey_group_title');
		$select->condition('sgq.survey_id',$entityId,'=');
		$select->condition('qst.status','sry_qtn_sts_atv','=');
		$select->orderBy('sgq.sequence');
		$select->orderBy('qst.id');
		$surveyGroupList = $select->execute()->fetchAll();
		expDebug::dPrintDBAPI(' $select object = ' , $select);
		
		return $surveyGroupList;
		
	}catch (Exception $ex) {
		watchdog_exception('getQuestionsDetails', $ex);
		expertusErrorThrow($ex);
	}
}

?>