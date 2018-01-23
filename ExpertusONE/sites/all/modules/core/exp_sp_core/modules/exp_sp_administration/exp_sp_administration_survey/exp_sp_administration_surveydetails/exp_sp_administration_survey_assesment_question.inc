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

 /**
 * exp_sp_administration_survey_assessment_addedit_form() - Build the addedit form for Survey/Assesment Question.
 */
function exp_sp_administration_survey_assesment_question_form($form, &$form_state, $renderMode, $surveyId){
	try{
	global $theme_key;
  expDebug::dPrint('Initial form Statee : '.print_r($form_state,true) ,5);
  expDebug::dPrint('Initial form Statee Storage : '.print_r($form_state['storage'],true) , 5);
  expDebug::dPrint('Initial form Statee render mode : '.print_r($renderMode ,true).$surveyId.'geting value'.$form_state['build_info']['args'][2] , 5);
  include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
  $css_theme_optional   = array('type' => 'file', 'group' => CSS_THEME);
  if($theme_key == 'expertusoneV2') {
  	drupal_add_css(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_surveydetails_v2.css', $css_theme_optional);
  }
  else{
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_surveydetails.css', $css_theme_optional);  	
  }
  
  $js_module_optional   = array('type' => 'file', 'group' => JS_DEFAULT);
  drupal_add_js(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_surveydetails.js', $js_module_optional);
  
  if(!empty($form_state['survey_id'])) {
    $formTypeMode = $form_state['survey_id'];
  }else{
    $formTypeMode  = $form_state['build_info']['args'][2];
  }
 if(!empty($form_state['storage']['search_attach_surass_question'])){
    $questionSearchName        = $form_state['storage']['search_attach_surass_question'];
    $searchQuestionGroupType   = $form_state['storage']['search_question_gptype'];
  }else{
    $questionSearchName='';
    $searchQuestionGroupType='surassqus';
  }  
  $entityId             = $formTypeMode;
  $entityType           = getSurveyInfo($entityId);
  $fetchQstionLst       = SurvAssAttachedQuestions($entityId, 0, 0, 0, $questionSearchName, $searchQuestionGroupType);
  //$count                = count($fetchQstionLst);  
  $type                 = 'attachQuestion';
  $uniqueId             = "$type-$entityId-$entityType";
  $wrapperId            = 'attach-question-grid-wrapper-'.$uniqueId;  
  $displayMode          = $form_state['storage']['mode'];
 // $getAttachQstList     = getAttachQuestion($entityId,'',0);
  $formName             = 'attach_question_form_'.$uniqueId;
  $questionArr          = array();
  $questionGroupArr     = array();
  $excludedQuestionId   = '""';
  $questionSearchType   = $form_state['build_info']['args'][1];
  
  
  // Survey Assessment Question search start
      addHiddenFieldToAddEditForm($form, 'hiddensurassgptype','');
       $paintMultiAction  = "<div id='search-list-surass-title-keyword' class='search-list-keyword' style='display:block;'>";
       $paintMultiAction .= "<span id='search-dropdwn-list-surass'>";
       $paintMultiAction .= "<input type='hidden' id='search_all_surass_type-hidden' value='surassqus' />";
       $paintMultiAction .= "<input type='hidden' id='surasssgroup_typehidden' value='' />";
       $paintMultiAction .= "<input type='hidden' id='surassattchedquestions-autocomplete_hidden' value=' ' />";
       $paintMultiAction .= "<span id='select-list-surass-dropdown' class='select-list-dropdown-surass'>".t('LBL325')."</span>";
       $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-surass-dropdown-link dropdown-link-font ' onclick='$(\"#root-admin\").data(\"narrowsearch\").moreSurAssSearchHideShow();'> </a>";                                                                
       $paintMultiAction .= "<ul id='select-list-surass-dropdown-list'>";
       $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreSurAssSearchTypeText('".t('LBL325')."','surassqus');\">".t('LBL325')."</li>";
       $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreSurAssSearchTypeText('".t('Group')."','surassgrpname');\">".t('Group')."</li>";
       $paintMultiAction .= "</ul>";
       $paintMultiAction .= "</span></div>";
   
      $form['surass_auto-search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin-surass-auto-search-container">',
      );
      
      $form['more_add_surasstype_search'] = array(
     '#type' => 'markup',
     '#markup' => $paintMultiAction,
     );
      $form['surassautocomplete_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_surass_auto_search">',
      );
      
     $acMenuPath = "administration/learning/survey/survey-assesment-questions-search-autocomplete/".$entityId;
     $autoCompleteAttributes['class'][] = 'admin_ac_surr_ass_input_mainform';
     $autoCompleteAttributes['class'][] = 'admin_ac_tp_txt_mainform';
     addACFieldForMultiSelect($form, 'surassattchedquestions-autocomplete', $acMenuPath, '', '', '', array(),
                                  array('search_type' => "$('#search_all_surass_type-hidden').val()"), false, false,
                                      t('LBL324'), $autoCompleteAttributes);
    
     $form['tpautocomplete_search-end'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );
      $searchAttributes['class'][] = 'admin-pagination-surass-search-go';
      $searchAttributes['class'][] = 'addedit-form-expertusone-throbber';
      $searchAttributes['data-wrapperid'][] = 'survey_attach_questions_dt';
      $searchAttributes['onClick'][] = "getSurAssSearchType(); ";   
       $form['search_attached-course'] = array(
        '#type' => 'submit',
        '#id'   => 'surass_attach_search_question',
        '#title' => t('LBL304'),
        '#value' => ' ',
        '#name' => 'Search Survey assessment attached questions',
        //'#submit' => array('search_enrolltpuser')attach_survey_assessment_questions_search_callback,
        '#attributes' => $searchAttributes,
        '#ajax' => array(
                  'wrapper' => 'survey_attach_questions_dt',
                  'callback' => 'attach_survey_assessment_questions_search_callback',
                  'method' => 'replace',
                  'effect' => 'none',
                  'event'=>'click',
   				  'question_pgn_page' => 1,
   				  'entity_id' =>$entityId,
                  'entity_type' =>$entityType,
                  'keypress' => true,
                  'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                  ),
                )
        /*'#ajax' => array(
                  'wrapper' => 'attach_course_dt',
                  'callback' => 'attach_tp_course_search_callback',
                  'method' => 'replace',
                  'effect' => 'none',
                  'event'=>'click',
   				  'question_pgn_page' => 1,
   				  'program_id' =>$program_id,
                  'get_attached_course_name'=> $form_state['values']['tpattchedcoursename-autocomplete'],    
                  'keypress' => true,
                  'progress' => array(
                    'type' => 'throbber',
                    'message' => ''
                  ),
                )*/
      );
       
      $form['greyout-load'] = array(
                '#type' => 'markup',
                '#prefix' => '<img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0" onload="onLoadChangeSurAssGreyout();" />',
              ); 
			$filterSuffix = '';
      if($theme_key == 'expertusoneV2') {
      	$filterSuffix = '<div class="filter-search-start-date-right-bg"></div>';
      }
       $form['surass_auto-search-end'] = array(
      '#type' => 'markup',
      '#markup' => $filterSuffix . '</div>',
      );   
  
  // Survey Assessment Question search end 
  unset($addAttributes);    
  $addAttributes['class'][]         = 'addedit-form-expertusone-throbber narrow-search-actionbar-orange-btnBG';
  $addAttributes['data-wrapperid']  = array($wrapperId);
  $addAttributes['onclick'][]       = '$(document).ready(function(){ editSurveyListDetails('.$surveyId.',0); });';
  
  $popupentityId      = $entityId;
  $popupentityType    = $entityType;
  $callUrl = ($entityType =='sry_det_typ_ass')? 'administration/assessment/assessmentdetails/attachquestion' : 'administration/survey/surveydetails/attachquestion'; 
  $popupAttachquestionIdInit        	 = $popupentityId.'_'.$popupentityType;  
  $popupattachquestionvisibPopupId  = 'popup_qtip_visible_disp_attach_question_'.$popupAttachquestionIdInit; //".$popupentityId."/".$popupentityType."
  $popupAttachQuestion    = "{'entityId':".$popupentityId.",'entityType':'".$popupentityType."',
														  'url':'".$callUrl."/".$popupentityId."/".$popupentityType."',
														  'popupDispId':'Question-".$popupattachquestionvisibPopupId."',
														  'catalogVisibleId':'qtip_visible_disp_attach_question_".$popupAttachquestionIdInit."',
														  'wid':515,'heg':'260','postype':'topright','poslwid':'','qdis':'ctool',
															'linkid':'visible-attachQuestion-".$popupentityId."'}";
    
    $addAttributes['class'][] = 'narrow-search-actionbar-orange-btnBG';    
    $form['add-attachquestion-button'] = array(
      '#type' => 'markup',
      '#prefix' => "<input type='hidden' id='qtip_position' value=''>
    		<div id='Question-".$popupattachquestionvisibPopupId."'>
    		<div id='".$popupattachquestionvisibPopupId."' class='add-attachquestion-popup-container'>
    		<a id='visible-attachQuestion-".$popupentityId."'class='admin-save-button-container' onclick =\"callVisibility(".$popupAttachQuestion.");\">
    		<div class='narrow-search-actionbar-orange-btnLeft'></div><div class='narrow-search-actionbar-orange-btnBG'>",
      '#suffix' => t("LBL418").'</div><div class="narrow-search-actionbar-orange-btnRight"></div></a>'.
    		"<span id='visible-popup-".$popupentityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div></div>",
      '#value' => t("Attach Question"),
      '#name' => "Attach Question",
      '#attributes' => $addAttributes,
    		
    );   
    
  $form['#prefix'] = '<div id="survey_attach_questions_dt">';
  $form['#suffix'] = '</div>';    
  
  $surveyId=$popupentityId;
  
/* ------------------------------------Start Pagination----------------------------*/  

  $pg_page  = 1;
  $start    = 0;
  $limit    = 5;
  
  $surAssQuestListCount =  SurvAssAttachedQuestions($popupentityId, 0, 0, 0, $questionSearchName,$searchQuestionGroupType);
  $surAssQuestListCount = count($surAssQuestListCount);   
 
  if( $surAssQuestListCount > 0 ){ 
    $total_pages = ceil($surAssQuestListCount/$limit); 
  }else{ 
    $total_pages = 0; 
  }	  
  if(!empty($form_state['storage']['page'])) {
    $pg_pagenew  = $form_state['storage']['page'];
    if($pg_pagenew > $total_pages){
       $pg_page  = $pg_pagenew- 1;
       $form_state['storage']['page'] = $pg_page;
    }else{
      $pg_page  = $form_state['storage']['page'];//$form_state['question_pgn_page'];
    }  
  }
  if($pg_page > $total_pages) {
  	$pg_page = $total_pages;
  }
  
  $limit = $pg_page * 5;

  $surAssQuestList =  SurvAssAttachedQuestions($popupentityId, 0, $start, $limit, $questionSearchName,$searchQuestionGroupType);

/* ------------------------------------End Pagination----------------------------*/

  
  
          
    $form['edit-survey-attachquestion-details-start'] = array(
      '#prefix' => "<div id='view-survey-attachquestion-list-wrapper'><div id='survey-attach-question-addedit-form-details'>",
    );
    
 
  if(count($fetchQstionLst) == 0){
  	$message = t('MSG368');
     $form['no-questions-list'] = array(
      '#type' => 'item',    
      '#prefix' => '<div class="clearBoth"></div><div class="nonlist" id="no-questions-list">'. $message ,
      '#suffix' => '</div>',  
    ); 
  }else{

        $i = 0;
        $valign ='';
        if($theme_key == 'expertusoneV2') {
          $valign = "valign=top";
        }
        if($entityType == 'sry_det_typ_ass'){
    	  $scoreTxt = "<td  class='attachquestion-title-heading attach-question-score' .$valign. >".t('LBL668')." <span class='addedit-mandatory'>*</span></td>";
    	  $validtext= "<div class='attach-question-valid-txt'>".t('MSG470')."</div>";
        }else{
    	  $scoreTxt = '';
    	  $validtext = '';
    	}
    	
    	
    	if($entityType == 'sry_det_typ_sry') {
    	  $manTxt = "<td class='attachquestion-title-heading attach-question-mandatory'>&nbsp;</td>";
    	}else{
    	  $manTxt = '';
    	}
    	$vAlign = 'valign=top';
    	$width = "width='580'";
    	$gHeight = '30px';
    	if($theme_key == 'expertusoneV2') {
    		//$vAlign = 'style="padding-left:10px; width:250px;"';
    		$width = "";
    		$gHeight = '26px';
    	
    	}
    	
    	if($surAssQuestList[$i]->random_type == 'sry_det_typ_ass_ran_ans') {  
    	  $classNameNewTheme = 'attach-question-move-icon';
    	  $vAlign = 'style="width:250px;"';
    	  $gAlign = 'style="padding-left:22px;"';
    	}else{
    	  if($theme_key == 'expertusoneV2'){ 
    	  	$vAlign = 'style="padding-left:10px;"';
    	  $classNameNewTheme = 'attach-question-move-icon-empty';
    	  }else{
    	    $classNameNewTheme = 'attach-question-move-icon';
    	  }
    	} 
    	$form['heading-attached-question'] = array(
            '#type' => 'markup',
            '#prefix' => "".$validtext."<div class='edit-attachquestion-list'><table class='attachquestion-table-heading' cellpadding='0' cellspacing='0' border='0' $width><tr height='".$gHeight."'>",
            '#suffix' =>  "
            			   <td class='attachquestion-title-heading attach-question-title' $vAlign>".t('LBL325')."</td>
            			   <td class='attachquestion-title-heading attach-question-group-name' $gAlign>".t('LBL379')."<span class='addedit-mandatory'>*</span><div class='attach-question-grp-title-note'>(".t('LBL630').")</div></td>
            			   ".$scoreTxt."
            			   ".$manTxt."
            			   <td class='attachquestion-title-heading attach-question-delete-btn'>&nbsp;</td>
            			   <td class='attachquestion-title-heading $classNameNewTheme'>&nbsp;</td>
            			   </tr></table>", 
          );
          
          
          $form['drag-and-drop-attached-question-start'] = array(
            '#type' => 'markup',
            '#prefix' => "<ul id='dragndrop'>",
            '#suffix' =>  "", 
          );
  		$gStyle = '';
    	if($theme_key == 'expertusoneV2') {
    		$gStyle = 'style="padding-left:10px;"';
    	}    
      for($i=0;$i<count($surAssQuestList);$i++) {

          $uniqueId = $entityId.'-'.$surAssQuestList[$i]->survey_question_id;
          $dynamic_cos_id      =    $surAssQuestList[$i]->survey_question_id;
          $dynamic_div         =	"delete_all_".$uniqueId;

		  
		  $title = $surAssQuestList[$i]->question_txt;
		  $displayTitle = titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-TITLE', $title,35);
		  $code  = $surAssQuestList[$i]->question_code;
		  $displayCode = titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-CODE', $code,8); 

          //$dragId = $surAssQuestList[$i]->mapping_id.'-'.$surAssQuestList[$i]->is_re_certify;
          //<span class='vtip' title='".$surAssQuestList[$i]->question_code."'>".$displayCode."</span></td>
          $dragId = $surAssQuestList[$i]->mapping_id;
          $alterNateRow = ($i%2 == 1) ? 'odd-even-row-highlighter' : '';
          $form['edit-attachquestion-list-'.$i] = array(
            '#type' => 'markup',
            '#prefix' => "<li id='".$dragId."' class='draggable-list $alterNateRow'><table cellpadding='0' cellspacing='0' border='0' class='addedit-survey-question-list' $width><tr>",
            '#suffix' =>  "", 
          );
          
          
          $form['edit-attachquestion-list-code-'.$i] = array(
            '#type' => 'markup',
            '#prefix' => "<td class='attach-question-title' $gStyle>",
            '#suffix' =>  "<span class='vtip' title='".htmlentities($surAssQuestList[$i]->question_txt,ENT_QUOTES,'UTF-8')."'>".$displayTitle."</span></td>", 
          );
          
          
            if(!empty($surAssQuestList[$i]->survey_group_id)) {
              $moduleDetails = getSurveyModuleDetails($surAssQuestList[$i]->survey_group_id, array('title'));
              $fullModTitle = $moduleDetails['title'];
              $clsName = "attachedquestion-list-name";
            }else{
              $fullModTitle = 'No Group Name';
              $clsName = "attachedquestion-list-name-italic";
            }
            $surveyAssessTitle = $fullModTitle;
            $modTitle     =  titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-QUESTION-FULLMODTITLE', $fullModTitle,15);
           
            $data = 'data={"entityId":"'.$surveyId.'","entityType":"'.$entityType.'","groupName":"'.$surveyAssessTitle.'","fullGroupName":"'.$fullModTitle.'","questionId":"'.$surAssQuestList[$i]->survey_question_id.'"}';
  		    $data = str_replace('"',"&quot;", $data);
            $form['edit-attachquestion-list-group-'.$i] = array(
            '#type' => 'markup',
            '#prefix' => '<td class="attach-question-group-name"><div class="addedit-attachedquestion_list"><div id="attachedquestion-name-div-'.$surAssQuestList[$i]->survey_question_id.'" class="vtip" title="'.$fullModTitle.'"><a href="javascript:void(0);" id="attachedquestion-list-name-id-'.$surAssQuestList[$i]->survey_question_id.'" name="attachedquestion-list-name-id-'.$surAssQuestList[$i]->survey_question_id.'" class = "'.$clsName.'"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedQuestion(\''.$data.'\');">'.$modTitle.'</a></div></div>',
            '#suffix' => '</td>',
            '#size' => 15,
            '#maxlength' => 100,
          );

          
          if($entityType == 'sry_det_typ_ass') {   
            
            $attributes['class'][] = 'addedit-form-expertusone-throbber';
            $attributes['data-wrapperid'] = 'survey_attach_questions_dt';
            $attributes['class'][] = 'attached-question-score';
           
            $form['edit-attachquestion-list-score-'.$i.'-'.$surAssQuestList[$i]->survey_question_id] = array(
              '#type' => 'textfield',
              '#prefix' => "<td class='attach-question-score'>",
              '#suffix' =>  "<span></span></td>", 
              '#default_value' =>(double) $surAssQuestList[$i]->survey_score,// Float Value Is Fixed For This Issue Only Three Char are Allowed #35870
              '#attributes' => $attributes,
              '#size' => 5,
              '#maxlength' => 5,
              '#ajax' => array(
                     'callback' => 'attach_questions_score_callback',
                   	 'wrapper' =>'survey_attach_questions_dt',
                     'loop_id' =>$i,
                 	 'question_id' =>$surAssQuestList[$i]->survey_question_id,
                     'entity_id' =>$surveyId,
                     'score_value' =>$surAssQuestList[$i]->survey_score,
                	 'entity_type' =>$entityType,    
                     'event' => 'blur', 
                  ),  

            );
            
          }
          
         if($entityType == 'sry_det_typ_sry') {            
          
           $data1 = $surveyId.'-'.$entityType.'-'.$surAssQuestList[$i]->survey_question_id.'-Y';
           $data2 = $surveyId.'-'.$entityType.'-'.$surAssQuestList[$i]->survey_question_id.'-N';
            
           if($surAssQuestList[$i]->mandatory_option == 'Y') {
             $manValue = 'Mandatory';
           }else{
             $manValue = 'Optional';
           }
            $form['edit-attachquestion-list-man-'.$i] = array(
              '#type' => 'markup',
              '#prefix' => "<td class='attach-question-mandatory'>",
              '#suffix' => '<div id="attach-question-mandatory-list">
              <ul class="man-opt">
                <li>
                	<span class="selected-man-opt">'.t($manValue).'</span><span class="man-opt-selection "></span>
                  <ul class="sub-menu">
                    <li data="'.$data1.'">'.t(Mandatory).'</li>
                    <li data="'.$data2.'">'.t(Optional).'</li>
                  </ul>
                </li>
              </ul>
              </div></td>', 
           //  '#attributes' => $manAttributes,
            );          
         
         } 
          //if(!$total_enroll){  

            $deleteAttributes['class'][]   = 'addedit-form-expertusone-throbber';
            $deleteAttributes['class'][]   = 'admin-delete-img';
            $deleteAttributes['class'][]   = 'attach-question-delete';

            $wrapperId                     =	'survey_attach_questions_dt';
            $deleteAttributes['data-wrapperid'] = array($wrapperId); 
            
            //For Hidding delete bin icon if any one of the user is taken the Survey/Assessment. ref TICKET: 0023022
            expDebug::dPrint('$entityId VS '.$entityId , 3);
            $deleteAccess = getSurveyAssessmentDeleteAccess($entityId);
            if($deleteAccess > 0){
              $deleteQ = 'display:none;';
              $icon = '';
            }else{
              $deleteQ = 'display:block;';
              $icon = '|';
            }
            
             $form['delete-group-list-button-start-'.$i] = array(
            	'#type' => 'markup',
                '#prefix' => '<td class="attach-question-delete-btn"><div class="attach-question-delete "  style='.$deleteQ.' onclick="attachQuestionDelete(\'deleteAttachedQuestion-'.$i.'\')">&nbsp;</div><span style="display:none;">',
             );
              
                $form['delete-group-list-button-'.$i] = array( 
                '#type' => 'checkbox',
                '#name' => 'deleteAttachedQuestion-'.$i,
                '#id' => 'deleteAttachedQuestion-'.$i,
                //'#prefix' => '',
                '#suffix' => '</span></td>',    
                '#attributes' => $deleteAttributes,
                '#ajax' => array(
                     'callback' => 'attach_questions_delete_callback',
                   	 'wrapper' =>'survey_attach_questions_dt',
                     'loop_id' =>$i,
                 	 'question_id' =>$surAssQuestList[$i]->survey_question_id,
                     'entity_id' =>$surveyId,
                     'group_id' =>$surAssQuestList[$i]->survey_group_id,
                	 'entity_type' =>$entityType,    
                     'event' => 'click', 
                  ),  
                );
              
              
        //  }
                 
         if($surAssQuestList[$i]->random_type == 'sry_det_typ_ass_ran_ans' || $entityType == 'sry_det_typ_sry') {                
          $form['edit-group-list-button-'.$i] = array( 
            '#type' => 'markup',
            '#prefix' => '<td width="4%"><div class="infinite-scrolling-separator">'.$icon.'</div></td><td width="2%" class="attach-question-move-icon" align="center"><div class="dragndrop-selectable-item"><img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" /></div>',
            '#suffix' => '</td></tr></table></li>',
            );       
         }else{
           if($theme_key == 'expertusoneV2') {
    		$className = 'attach-question-move-icon-empty';
    	   }else{
    	     $className = 'attach-question-move-icon';
    	   } 
           $form['edit-group-list-button-'.$i] = array( 
            '#type' => 'markup',
            '#prefix' => '<td width="4%"><div class="infinite-scrolling-separator">'.$icon.'</div></td><td width="2%" class="'.$className.'" align="center"><div class="dragndrop-selectable-item"><img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" /></div>',
            '#suffix' => '</td></tr></table></li>',
            );
         }      
       }       
                 
          $form['drag-and-drop-attached-question-end'] = array(
            '#type' => 'markup',
            '#prefix' => "",
            '#suffix' =>  "</ul>", 
          );
          
          $form['sortable-load'] = array(
            '#type' => 'markup',
            '#prefix' => '<img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0" onload="sequenceDragAndDrop(\'dragndrop\', \'attach_question\'); vtip(); " />',
          );  
          
          // For Show more records #0022926
          if($surAssQuestListCount > 5){
          	$attrShowMore = array();
          	$attrShowMore['class'][] = 'addedit-form-expertusone-throbber';
          	$attrShowMore['class'][] = 'infinite-scrolling';
          	$attrShowMore['data-wrapperid'] = 'survey_attach_questions_dt';
          	 
          	expDebug::dPrint(' Records Count $surAssQuestListCount = '.$surAssQuestListCount);
          	expDebug::dPrint(' Records Count $limit = '.$limit);
          	 
          	$ShowMore = ($limit >= $surAssQuestListCount) ? 0 : 1 ;
          	 
          	if($ShowMore == 1){
          		$form['infinite-scrolling'] = array(
          				'#type' => 'button',
          				'#id' => 'attach_qstion_pg_next',
          				'#prefix' => '<div class="infinite-loader-container">',
          				'#suffix' => '</div>',
          				'#value' => t('LBL1251'),
          				'#name' => 'attach_questions_pagination_next',
          				'#attributes' => $attrShowMore,
          				'#ajax' => array(
          						'wrapper' => 'survey_attach_questions_dt',
          						'callback' => 'attach_questions_pagination_callback',
          						'method' => 'replace',
          						'effect' => 'none',
          						'event'=>'click',
          						'question_pgn_page' => ($pg_page+1),
          						'entity_id' =>$surveyId,
          						'entity_type' =>$entityType,
          						'keypress' => true,
          						'progress' => array(
          								'type' => 'throbber',
          								'message' => ''
          						),
          				)
          		);
          	}else{
          		$form['infinite-scrolling'] = array(
          				'#markup' => '<div class="infinite-loader-container infinite-loader-no-records">'.t('LBL1252').'</div>',
          				'#attributes' => $attrShowMore,
          		);
          	}
          	 
          }
          
          $form['edit-survey-attachquestion-details-end'] = array(
          		'#suffix' => "<div class='clearBoth'></div></div></div></div>",
          );
  }
  
  //Added for 0018617: Assessment search option (
  //2.After Tp saved when open up in the Edit mode below page nation some text is showing in it.)
	if(count($fetchQstionLst) == 0){
  	$form['attachquestion-pagination-markup-hidden'] = array(
  			'#type' => 'markup',
  			'#prefix' => '<img style="visibility:hidden;" src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0" onload="$(\'#attach_qstion_pg_next\').val(\'\');" />',
  	);
  }
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_survey_assesment_question_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function attach_questions_pagination_callback($form, &$form_state){
  try{
  
  $entityId             = $form_state['triggering_element']['#ajax']['entity_id']; 
  $question_pgn_page    = $form_state['triggering_element']['#ajax']['question_pgn_page'];
  
 $form_state = array();
 $form_state['rebuild'] = TRUE;
  
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'survey_id' => $entityId,
   'question_pgn_page' => $question_pgn_page,
   'build_info' => array('args' => array('ajax'),'question_pgn_page' => $question_pgn_page) 
  );
  
  $form_state = $min_form_state;
  
  expDebug::dPrint('attach_questions_pagination_callback calllll : '.print_r($form_state,true) , 5);
  
  $formId = 'exp_sp_administration_survey_assesment_question_form';
  $builtForm = drupal_build_form($formId, $form_state);

  $commands = array();  
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  //$commands[] = ajax_command_refresh_narrow_search_results('onclose');

  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
  } catch (Exception $ex) {
  	watchdog_exception('attach_questions_pagination_callback', $ex);
  	expertusErrorThrow($ex);
  }
  
}


function attach_questions_score_callback($form, &$form_state){
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_attach_question.inc');
		$questionId = $form_state['triggering_element']['#ajax']['question_id'];
		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];

		$loopId = $form_state['triggering_element']['#ajax']['loop_id'];

		$scoreValue = $form_state['values']['edit-attachquestion-list-score-'.$loopId.'-'. $questionId];
		$scoreValue = empty($scoreValue) ? 0 : $scoreValue;		
		if(is_numeric($scoreValue)){		
			expDebug::dPrint('$scoreValue Before Float Conv : '.$scoreValue, 4);
			$scoreValue = is_float($scoreValue) ? $scoreValue : floatval($scoreValue);
			expDebug::dPrint('$scoreValue after Float Conv : '.$scoreValue, 4);
			attachedquestion_score_update($entityId,$questionId,$scoreValue);
		}

		$min_form_state = array(
				'ajax' => TRUE,
				're_render' => TRUE,
				'no_redirect' => TRUE,
				'survey_id' => $entityId,
				'question_id' =>  $questionId,
				'build_info' => array('args' => array('ajax'))
		);

		$form_state = $min_form_state;

		expDebug::dPrint('attach_questions_delete_callback : '.print_r($form_state,true) , 5);

		$formId = 'exp_sp_administration_survey_assesment_question_form';
		$builtForm = drupal_build_form($formId, $form_state);

		$commands = array();
		$commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
		$commands[] = ajax_command_refresh_narrow_search_results('onclose');

		return array(
				'#type' => 'ajax',
				'#commands' => $commands
		);

	} catch (Exception $ex) {
  	watchdog_exception('attach_questions_score_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

function attach_questions_delete_callback($form, &$form_state){  
 try {  
  $questionId = $form_state['triggering_element']['#ajax']['question_id'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];

  $delete=deleteSurveyAttachedQuestion($entityId, $questionId);   
  
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'survey_id' => $entityId,
   'question_id' =>  $questionId,
   'build_info' => array('args' => array('ajax')) 
  );
  
  $form_state = $min_form_state;
  
  expDebug::dPrint('attach_questions_delete_callback : '.print_r($form_state,true) , 5);
  
  $formId = 'exp_sp_administration_survey_assesment_question_form';
  $builtForm = drupal_build_form($formId, $form_state);

  $commands = array();  
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  $commands[] = ajax_command_refresh_narrow_search_results('onclose');

  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
  } catch (Exception $ex) {
  	watchdog_exception('attach_questions_delete_callback', $ex);
  	expertusErrorThrow($ex);
  }
}
function attach_survey_assessment_questions_search_callback($form, &$form_state){
 try{ 
  
  $entityId                   = $form_state['triggering_element']['#ajax']['entity_id']; 
  $question_pgn_page          = $form_state['triggering_element']['#ajax']['question_pgn_page'];
  $get_question_search_name   = $form_state['values']['surassattchedquestions-autocomplete'];
  
 $form_state = array();
 $form_state['rebuild'] = TRUE;
  
  $min_form_state = array(
   'ajax' => TRUE, 
   're_render' => TRUE, 
   'no_redirect' => TRUE, 
   'survey_id' => $entityId,
   'question_pgn_page' => $question_pgn_page,
   'get_question_search_name' => $get_question_search_name,
   'build_info' => array('args' => array('ajax'),'question_pgn_page' => $question_pgn_page,'get_question_search_name'=>$get_question_search_name) 
  );
  
  $form_state = $min_form_state;
  
  expDebug::dPrint('attach_survey_assessment_questions_search_callback calllll : '.print_r($form_state,true) , 5);
  
  $formId = 'exp_sp_administration_survey_assesment_question_form';
  $builtForm = drupal_build_form($formId, $form_state);

  $commands = array();  
  $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
  //$commands[] = ajax_command_refresh_narrow_search_results('onclose');

  return array(
        '#type' => 'ajax',
        '#commands' => $commands
  );
  
  } catch (Exception $ex) {
  	watchdog_exception('attach_survey_assessment_questions_search_callback', $ex);
  	expertusErrorThrow($ex);
  }
}
function delete_qst_group($form, &$form_state) {  
	try{ 
  $form_state['rebuild'] = TRUE;  
  } catch (Exception $ex) {
  	watchdog_exception('delete_qst_group', $ex);
  	expertusErrorThrow($ex);
  }
}


function attachQuestionGrid($form, &$form_state) {
	try{
  $formName = $form_state['form_unique_name'];
  return $form[$formName];
  } catch (Exception $ex) {
  	watchdog_exception('attachQuestionGrid', $ex);
  	expertusErrorThrow($ex);
  }
}


function add_question_entity($form, &$form_state) {
	try {
  $form_state['storage']['mode'] = 'edit';
  $form_state['storage']['act_mode'] = 'add';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('add_question_entity', $ex);
  	expertusErrorThrow($ex);
  }
}

function getPrePostValues($form, &$form_state) {
  try {
  $entityId   = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  $uniqueId   = "attachQuestion-$entityId-$entityType";

  /*
     hidden_entity_id_
    hidden_entity_type_
    hidden_question_id_
    hidden_group_name_
    hidden_sequence_
    hidden_score_
    */
  $questionIdStack  = array();  
  $groupNameStack   = array();
  $sequenceStack    = array();
  $scoreStack       = array();  
  
  expDebug::dPrint('Form Inputttt storage Errorr question idssss '.print_r($form_state['input'],true) , 5);
  expDebug::dPrint('Form Inputttt storage Errorr question idssss-1 '.$uniqueId , 4);
  
  $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
  $questionIdLst = $form_state['input']['hidden_idlist_'.$uniqueId];
  expDebug::dPrint('Form Inputttt storage Errorr question idssss-2 '.print_r($associateIdListArray,true) , 4);
    
    if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){      

        $associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
        
        foreach($associateIdListArray as $keyId=>$associatedEntityId){
  
            $qstEntityId         = $associatedEntityId;            
            $groupNameInputVal   = $form_state['input']['attachQuestion-groupName-'.$qstEntityId];
            $sequenceInputVal    = $form_state['input']['attachQuestion-sequenceNo-'.$qstEntityId];
            $scoreInputVal       = $form_state['input']['attachQuestion-score-'.$qstEntityId];
            $mandatoryInputVal   = $form_state['input']['attachQuestion-mandatoryOptional-'.$qstEntityId];
            
            $questionIdStack[] = $associatedEntityId;
            $groupNameStack[]  = $groupNameInputVal;
            $sequenceStack[]   = $sequenceInputVal;
            $scoreStack[]      = $scoreInputVal;
            $mandOptStack[]    = $mandatoryInputVal;
        }
    }
    expDebug::dPrint('Form Inputttt storage Errorr '.print_r($form_state["storage"]["error"],true) , 5);
    expDebug::dPrint('Form Inputttt valueeeeesss '.print_r($form_state['input'],true) , 5);
    /*
    
    $form_state["storage"]["error"]["hidden_question"]    = $hidden_question_id;         
    $form_state["storage"]["error"]["hidden_sequance"]    = $hidden_sequence;
    $form_state["storage"]["error"]["hidden_score"]       = $hidden_score;         
    $form_state["storage"]["error"]["hidden_entity_id"]   = $entityId;
    $form_state["storage"]["error"]["hidden_entity_type"] = $entityType; 
    */
    
    $sequenceStackImplode   = implode(",",$sequenceStack);
    $scoreStackImplode      = implode(",",$scoreStack);
    $groupNameImplode       = implode(",",$groupNameStack);
    $mandatoryOptionImplode = implode(",",$mandOptStack);
    
    //if(!empty($form_state["storage"]["error"]["hidden_question"]) && !empty($form_state["storage"]["error"]["hidden_question_err"])){
    if(!empty($form_state["storage"]["error"]["hidden_question"])){
      
            $entityId            =  $form_state["storage"]["error"]["hidden_entity_id"]; 
            $entityType          =  $form_state["storage"]["error"]["hidden_entity_type"];
            $questionIdStack     =  explode(",",$form_state["storage"]["error"]["hidden_question"]);
            $groupNameStack      =  explode(",",$form_state['input']['hidden_group_name_'.$uniqueId]);
            $sequenceStack       =  explode(",",$form_state["storage"]["error"]["hidden_sequance"]);
            $scoreStack          =  explode(",",$form_state["storage"]["error"]["hidden_score"]);
            $mandatoryStack      =  explode(",",$form_state["storage"]["error"]["hidden_mandatory_opt"]);

           /* $sequenceStackImplode   = $form_state["storage"]["error"]["hidden_sequance"];
            $scoreStackImplode      = $form_state["storage"]["error"]["hidden_score"];
            $groupNameImplode       = $form_state['input']['hidden_group_name_'.$uniqueId];
            */
    }
    
    $return_array = array('entity_id' => $entityId, 'entity_type' => $entityType, 'question_id' => $questionIdLst,'group_name' => $groupNameImplode,
                          'sequence' => $sequenceStackImplode, 'score' => $scoreStackImplode, 'mandatory_option' => $mandatoryOptionImplode
                          );
                          
    expDebug::dPrint('return pre-submit values '.print_r($return_array,true) , 4);                          
    return $return_array;
    } catch (Exception $ex) {
    	watchdog_exception('getPrePostValues', $ex);
    	expertusErrorThrow($ex);
    }
}

function cancelQuestionDetails($form, &$form_state){
	try {
  $form_state['storage']['mode'] = '';
  $form_state['rebuild'] = TRUE;
  } catch (Exception $ex) {
  	watchdog_exception('cancelQuestionDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewQuestionList($form, &$form_state) {
  try {
  expDebug::dPrint('addNewQuestionList submit state valueesss '.print_r($form_state['values'],true) , 5);
  expDebug::dPrint('addNewQuestionList submit state valueesss inputs : '.print_r($form_state['input'],true) , 5);
  
  expDebug::dPrint('Form Submit Checkk : '.$form_state["storage"]["form_validate"]." | ".$form_state['triggering_element']['#name'] , 5);
  // // $submit_action = ($form_state['storage']['mode'] == "addmore") ? "save" : "attachquestion" ;
 // if ($form_state['triggering_element']['#value'] == t('LBL374')) {
   
  if(($form_state["storage"]["form_validate"]) && ($form_state['triggering_element']['#name'] == 'save')) {
   expDebug::dPrint('ready to submit : ');
      $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
      $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
      $uniqueId = "attachQuestion-$entityId-$entityType";
      
      $hidden_question_id = $form_state['values']['hidden_question_id_'.$uniqueId];
      $hidden_sequence    = $form_state['values']['hidden_sequence_'.$uniqueId];
      $hidden_score       = $form_state['values']['hidden_score_'.$uniqueId];
      $entityId           = $form_state['values']['hidden_entity_id_'.$uniqueId];
      $hidden_mand_opt    = $form_state['values']['hidden_mandatory_option_'.$uniqueId];
      
      expDebug::dPrint('qst1111  mandatoryOption hidden :'.$hidden_mand_opt , 5);
      
      $survey_id          = $entityId;
      $question_ids       = explode(",",$hidden_question_id);
      $sequence_ids       = explode(",",$hidden_sequence);
      $score_ids          = explode(",",$hidden_score);  
      $mandatoryOption    = explode(",",$hidden_mand_opt);
      
      $surveyGroupTxt     = $form_state['input']['attachQuestion-groupName-700'];//.$question_ids[0]];
      
      expDebug::dPrint('qst1111  $question_ids :'.print_r($question_ids,true) , 4);
      expDebug::dPrint('qst1111  $sequence_ids :'.print_r($sequence_ids,true) , 4);
      expDebug::dPrint('qst1111  $score_ids :'.print_r($score_ids,true) , 4);
      expDebug::dPrint('qst1111  $entityId :'.print_r($entityId,true) , 4);  
      expDebug::dPrint('qst1111  surveyGroupTxt :'.print_r($surveyGroupTxt,true) , 4);
      expDebug::dPrint('qst1111  mandatoryOption :'.print_r($mandatoryOption,true) , 4);
      
      for($i=0;$i<count($question_ids);$i++) {
        
            $surveyQuestionId    = $question_ids[$i];
            $survey_score        = $score_ids[$i];
            $sequence            = $sequence_ids[$i]; 
            $mandatoryOptionVal  = $mandatoryOption[$i];    
            $mandatoryOptionVal  = ($entityType == 'sry_det_typ_ass') ? 'Y' : $mandatoryOptionVal;   
            //$surveyQuestionId    = $qstEntityId;
            
            expDebug::dPrint('qst1111  mandatoryOption :'.print_r($mandatoryOption,true) , 4);
            
            //$surveyGroupTxt      = $form_state['input']['attachQuestion-groupName-'.$qstEntityId];
            //$group_question_id   = $form_state['values']['group_question_id'];        
                       
            $checkGroupNameExist = checkGroupNameExist($surveyGroupTxt);
              
            if(!empty($checkGroupNameExist["id"])) {          
              $survey_group_id   = $checkGroupNameExist["id"];              
            } else {
              $groupInfo         = array('survey_id' => $survey_id,'survey_group' => $surveyGroupTxt);
              $survey_group_id   = _addNewSurveyGroup($groupInfo);
            }            
            // $surveyGroupInfo['mandatoryOption']
            $surveyGroupQuestion = array('survey_id' => $survey_id, 'survey_group_id' => $survey_group_id, 'survey_question_id' => $surveyQuestionId, 'survey_score' => (!empty($survey_score) ? $survey_score : ''), 'sequence' => $sequence, 'mandatoryOption' => $mandatoryOptionVal);
            expDebug::dPrint('Survey Qst :'.print_r($surveyGroupQuestion,true) , 4);
            
            _addNewSurveyQuestionGroup($surveyGroupQuestion); 
            $form_state['storage']['new_entity'] = TRUE;          
      }
      
      drupal_set_message(t('MSG601').'.');
      
      $form_state['storage']['mode']       = 'view';
      $form_state['storage']['act_mode']   = '';
      $form_state['rebuild']               = TRUE;
  }
  } catch (Exception $ex) {
  	watchdog_exception('addNewQuestionList', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateQuestionList($form, &$form_state) {
	try {
  expDebug::dPrint('updateQuestionList on submit ');
  
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  
  if($form_state["storage"]["form_validate"]) {
       expDebug::dPrint('updateQuestionList submit triggerr '.print_r($form_state['triggering_element']['#ajax'],true) , 5);
       expDebug::dPrint('updateQuestionList submit '.print_r($form_state['input'],true) , 5);
       expDebug::dPrint('updateQuestionList submit values '.print_r($form_state['values'],true) , 5); 

       //entity_type
       
       expDebug::dPrint('hidden_qustion_list_id '.$form_state['input']['hidden_qustion_list_id'] , 5);
       
       //$qustion_list_id  =  ereg_replace('"',$form_state['input']['hidden_qustion_list_id']);
       $qustion_list_id  =  $form_state['input']['hidden_qustion_list_id'];
       $qustion_list_id  = ereg_replace("'","",$qustion_list_id);
       
       expDebug::dPrint('hidden_qustion_list_id strr '.$qustion_list_id , 5);
       
       $question_ids     = explode(",",$qustion_list_id);
    
       expDebug::dPrint('hidden_qustion_list_id explode '.print_r($question_ids,true) , 4);
       
       $question_group_updated   = false;
       
       foreach($question_ids as $question_id){
         
          $surveyQuestionTxt   = $form_state['input']['attachQuestion-groupName-'.$question_id];
          $survey_id           = $form_state['input']['entity_id'];
          $group_question_id   = $question_id;//$form_state['input']['group_question_id'];
          $sequence            = $form_state['input']['attachQuestion-sequenceNo-'.$question_id];
          $survey_score        = $form_state['input']['attachQuestion-score-'.$question_id];
          $mandatoryInput      = $form_state['input']['attachQuestion-mandatoryOptional-'.$question_id];
          
          $mandatoryInput      = ($entityType == 'sry_det_typ_ass') ? 'Y' : $mandatoryInput;
          
          $survey_group_id     = '';
          
          if(!empty($surveyQuestionTxt)) {
            
              $checkGroupNameExist = checkGroupNameExist($surveyQuestionTxt);
            
              if(!empty($checkGroupNameExist["id"])) {
                $survey_group_id   = $checkGroupNameExist["id"];    
              } else {
                $groupInfo         = array('survey_id' => $survey_id,'survey_group' => $surveyQuestionTxt);
                $survey_group_id   = _addNewSurveyGroup($groupInfo);
              }
            
          }
          
          expDebug::dPrint($question_id.' | Group question to update  : $surveyQuestionTxt : '.$surveyQuestionTxt.' | $$sequence : '.$sequence.' | $$survey_score : '.$survey_score , 4);
          
          if(!empty($surveyQuestionTxt) || !empty($sequence) || !empty($survey_score)) { 
            
            $surveyGroupQuestion['survey_id']         = $survey_id;
            $surveyGroupQuestion['survey_group_id']   = $survey_group_id;
            $surveyGroupQuestion['sequence']          = $sequence;
            $surveyGroupQuestion['group_question_id'] = $group_question_id;
            $surveyGroupQuestion['survey_score']      = $survey_score;
            $surveyGroupQuestion['mandatoryOption']   = $mandatoryInput;
            
            
            //$surveyGroupQuestion = array('survey_id' => $survey_id, 'survey_group_id' => $survey_group_id, 'sequence' => $sequence, 'group_question_id' => $group_question_id, 'survey_score' => $survey_score);
            $question_group_updated   = true;
             expDebug::dPrint('Survey Qst up :'.print_r($surveyGroupQuestion,true) , 4);
            _updateSurveyQuestionGroup($surveyGroupQuestion);
            
            $form_state['storage']['edited_entity'] = TRUE;
            
            
          }
         
       }
       
       if($question_group_updated) {
          drupal_set_message(t('MSG601').'.');
          $form_state['rebuild']               = TRUE;
       }
   
   }
   
   // hidden_qustion_list_id
   } catch (Exception $ex) {
   	watchdog_exception('updateQuestionList', $ex);
   	expertusErrorThrow($ex);
   }
}

function add_question($form, &$form_state) {
	try {
		$form_state['storage']['mode'] = 'edit';
		$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('add_question', $ex);
		expertusErrorThrow($ex);
	}
}

function attach_question_group_form_submit($form, &$form_state){
	try {
  expDebug::dPrint("Validate herrrrr : submit");
  if ($form_state['triggering_element']['#value'] == t('Add') && $form_state["storage"]["form_validate"]) {  
    $surveyQuestionTxt   = $form_state['values']['survey_question_name_survey_question_fld'];
    $surveyGroupTxt      = $form_state['values']['survey_group'];
    $surveyQuestionId    = getSurveyQuestionId($surveyQuestionTxt);  
    $groupInfo           = array('survey_id' => $form_state['values']['survey_id'],'survey_group' => $form_state['values']['survey_id']);
    //$surveyGroupId       = _addNewSurveyGroup($groupInfo);    
    $checkGroupNameExist = checkGroupNameExist($surveyGroupTxt);
    $survey_id           = $form_state['values']['survey_id'];
    expDebug::dPrint('surveyGroupTxt :'.$surveyGroupTxt." || ".print_r($checkGroupNameExist,true) , 4);
      
      if(!empty($checkGroupNameExist["id"])) {
        $survey_group_id   = $checkGroupNameExist["id"];    
      } else {
        $groupInfo         = array('survey_id' => $survey_id,'survey_group' => $surveyGroupTxt);
        $survey_group_id   = _addNewSurveyGroup($groupInfo);
      }
    
      expDebug::dPrint('surveyGroupTxt after :'.$surveyGroupTxt." || ".print_r($checkGroupNameExist,true) , 4);
    
    //$survey_group_id     = $surveyGroupId; 
    $survey_score        = ($form_state['values']['survey_score'] != 'Score') ? $form_state['values']['survey_score'] : '';  
    $surveyGroupQuestion = array('survey_id' => $survey_id, 'survey_group_id' => $survey_group_id, 'survey_question_id' => $surveyQuestionId, 'survey_score' => (!empty($survey_score) ? $survey_score : ''));
    expDebug::dPrint('Survey Qst :'.print_r($surveyGroupQuestion,true) , 4);
    _addNewSurveyQuestionGroup($surveyGroupQuestion);  
    drupal_set_message(t('MSG601').'.');  
  }
  } catch (Exception $ex) {
  	watchdog_exception('attach_question_group_form_submit', $ex);
  	expertusErrorThrow($ex);
  }
}

function _addNewSurveyGroup($groupInfo) {
	$txn = db_transaction();
	try {
		$insertStmt  	 = db_insert('slt_survey_groups');
		if($groupInfo['userid'])
			$loggedInUserId = $groupInfo['userid'];
		else
			$loggedInUserId  = getIdOfLoggedInUser();
		$field_change1   = 'created_by';
		$field_change2   = 'created_on';
		$custom = NULL;
		$fields          = array(
								'survey_id'          => $groupInfo['survey_id'],
								'title'              => $groupInfo['survey_group'],
								'code'		     	 => $groupInfo['survey_group'],
								'status'             => 'sry_det_grp_atv',
								$field_change1       => $loggedInUserId,
								$field_change2       => now(),
								'updated_by'         => $loggedInUserId,
								'updated_on'         => now(),
								'custom0'            => $custom,
								'custom1'            => $custom,
								'custom2'            => $custom,
								'custom3'            => $custom,
								'custom4'            => $custom,
							);

		$insertStmt->fields($fields);
		$surveyGroupId	= $insertStmt->execute();

		return $surveyGroupId;
	} catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('_addNewSurveyGroup', $ex);
		if($groupInfo['userid']){
			return 0;
		}
		throw $ex;
	}
	// Commit the transaction
	unset($txn);
}

 
function _addNewSurveyQuestionGroup($surveyGroupInfo) {
	$txn = db_transaction();
	try {
		$insertStmt  	  = db_insert('slt_survey_groups_questions');
		if($surveyGroupInfo['userid'])
			$loggedInUserId = $surveyGroupInfo['userid'];
		else
			$loggedInUserId  = getIdOfLoggedInUser();
		
		$field_change1    = 'created_by';
		$field_change2    = 'created_on';
		$fields           = array(
								'survey_id'          => $surveyGroupInfo['survey_id'],
								'survey_group_id'    => $surveyGroupInfo['survey_group_id'],
								'question_id'		 => $surveyGroupInfo['survey_question_id'],
								'sequence'           => $surveyGroupInfo['sequence'],
								'mandatoryOption'	 => $surveyGroupInfo['mandatoryOption'],
								'score'		       	 => (!empty($surveyGroupInfo['survey_score']) ? $surveyGroupInfo['survey_score'] : null),
								$field_change1       => $loggedInUserId,
								$field_change2       => now(),
								'updated_by'         => $loggedInUserId,
								'updated_on'         => now(),
								'custom0'            => $custom,
								'custom1'            => $custom,
								'custom2'            => $custom,
								'custom3'            => $custom,
								'custom4'            => $custom,
							);

		$insertStmt->fields($fields);
		$surveyGroupId		= $insertStmt->execute();
		expDebug::dPrint('exp_sp_administration_survey_assesment$surveyGroupId' . print_r($fields,true) ,4);
		make_audit_trail_entries($fields, getIdOfLoggedInUser() , null, $surveyGroupInfo['survey_question_id'] ,'Inserted','cre_sys_obt_sur','exp_sp_administration_survey','_addNewSurveyQuestionGroup',
				array('question_id', 'sequence','mandatoryOption', 'score'));

		return $surveyGroupId;

	} catch (Exception $ex) {
		$txn->rollback();
		watchdog_exception('_addNewSurveyQuestionGroup', $ex);
		if($surveyGroupInfo['userid']){
			return 0;
		}
		throw $ex;
	}

	// Commit the transaction
	unset($txn);
}


function _updateSurveyQuestionGroup($surveyGroupInfo) {

  $txn = db_transaction();

  try {

    $updateStmt       = db_update('slt_survey_groups_questions');    
    $default= db_query("SELECT id,survey_id,survey_group_id,sequence,mandatoryOption from slt_survey_groups_questions where id=:uid",array(":uid"=> $surveyGroupInfo['group_question_id']))->fetchAll();
    if($surveyGroupInfo['userid'])
      $loggedInUserId = $surveyGroupInfo['userid'];
    else
      $loggedInUserId  = getIdOfLoggedInUser();
    $field_change1    = 'updated_by';
    $field_change2    = 'updated_on';
    
    $fields           = array();
    if(!empty($surveyGroupInfo['survey_group_id'])) {
      $fields['survey_group_id'] = $surveyGroupInfo['survey_group_id'];
    }
    
    if(!empty($surveyGroupInfo['sequence'])) {
      $fields['sequence'] = $surveyGroupInfo['sequence'];
    }
    
    if(!empty($surveyGroupInfo['survey_score'])) {
      $fields['score'] = $surveyGroupInfo['survey_score'];
    }
    if(!empty($surveyGroupInfo['mandatoryOption'])) {
      $fields['mandatoryOption'] = $surveyGroupInfo['mandatoryOption'];
    }

    $fields['updated_by'] = $loggedInUserId;
    $fields['updated_on'] = now();
    
    /*
    $fields           = array(
                'survey_id'          => $surveyGroupInfo['survey_id'],                  
                'survey_group_id'    => $surveyGroupInfo['survey_group_id'],
    			'sequence'           => $surveyGroupInfo['sequence'],
                //'question_id'		 => $surveyGroupInfo['survey_question_id'], 
                'score'              => ($surveyGroupInfo['survey_score'] != 'Score') ? $surveyGroupInfo['survey_score'] : null,               
                $field_change1       => $loggedInUserId,
                $field_change2       => now(),
                'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom,             
              );
              */
    $updateStmt->condition('id', $surveyGroupInfo['group_question_id']);
    $updateStmt->fields($fields);  
    $updateStmt->execute();
    if($surveyGroupInfo['userid']){
      return $surveyGroupInfo['group_question_id'];
    }  
    $fieldsupdate = $fields;
    $fieldsupdate[mandatoryoption] = $fields[mandatoryOption];
    expDebug::dPrint('exp_sp_administration_survey_update$surveyGroupId12' . print_r($fieldsupdate,true) ,4);
    expDebug::dPrintDBAPI('exp_sp_administration_survey_updatedefaoulst$surveyGroupId' , $default );
    make_audit_trail_entries($fieldsupdate, getIdOfLoggedInUser() , $default, $surveyGroupInfo['group_question_id'] ,'Updated','cre_sys_obt_sur','exp_sp_administration_survey','_updateSurveyQuestionGroup',
                                          array('sequence','mandatoryoption'));
    
    
   } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('_updateSurveyQuestionGroup', $ex);
    if($surveyGroupInfo['userid']){
      return 0; 
    }
    throw $ex;
  }
  
  unset($txn);
}

function exp_sp_administration_group_question_repaint($form, &$form_state) {
  try{
  // $form_state['triggering_element']['#value'] == 'Cancel'
  
  $triggerElement     = $form_state['triggering_element']['#value'];  
  $survey_id          = $form_state['triggering_element']['#ajax']['survey_id'];
  $storage_step       = $form_state['storage']['step'];  
  $form_validate      = $form_state["storage"]["form_validate"];  
  $form_validate      = ($triggerElement == 'Cancel') ? false : $form_validate; 

          
  $min_form_state     = array(
   		'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
   		're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
   		'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
   		'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
        );
        
  $form_state                                 = $min_form_state;  
  $form_state['storage']['new']['survey_id']  = $survey_id;
  $form_state['storage']['step']              = $storage_step;
  $classListrenderedForm                      = exp_ctools_modal_multi_ajaxonly_forms_renderform('exp_sp_administration_catalog_class_title_form', $form_state);
  $builtForm                                  = drupal_build_form('exp_sp_administration_survey_assesment_question_form', $form_state);
  $commands                                   = array();
  if($form_validate) {
    $commands[]                                 = ajax_command_replace(NULL, drupal_render($builtForm));
  }
  $commands[]                                 = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[]                                 = exp_ctools_modal_adjust();
  $commands[]                                 = ajax_command_refresh_narrow_search_results('onclose');  
  $form_state['input']                        = array();
  $form_state['values']                       = array();
  
  return array('#type' => 'ajax', '#commands' => $commands); 
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_group_question_repaint', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSurveyAssessmentGroupAutoComplete(){
try{
  $searchText   =  $_GET['z'];
  
  $select  = db_select('slt_survey_groups', 'groups');
  $select->addField('groups', 'id');
  $select->addField('groups', 'survey_id');
  $select->addField('groups', 'title');
  $select->condition('groups.title', '%' . db_like($searchText) . '%', 'LIKE');

  $groupNameResult = $select->execute()->fetchAll();
  
  print '';
  foreach ($groupNameResult as $group) {
      print $group->title . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyAssessmentGroupAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}


function getSurveyQuestionAutoComplete() {
 try{ 
  $searchText   =  $_GET['z'];
  $searchType   =  $_GET['search_type'];
  expDebug::dPrintDBAPI('----$searchType----' , $searchType );
  $excluded_question_id = $_GET["excluded_question_id"];
  $idRequired = isset($_GET['id_required']) ? $_GET['id_required'] : 1;
  $entityType = isset($_GET['entity_type']) ? $_GET['entity_type'] : '';
  $select  = db_select('slt_survey_questions', 'surveyQstion');
  
  $select->condition('surveyQstion.status', 'sry_qtn_sts_atv', '=');
  if($entityType == 'sry_det_typ_ass') {  	
    //sry_qtn_typ_cmt, sry_qtn_typ_dpn, sry_qtn_typ_img, sry_qtn_typ_mch, sry_qtn_typ_mch, sry_qtn_typ_rtg, sry_qtn_typ_trf, sry_qtn_typ_yno
    $assesArr = array('sry_qtn_typ_mch', 'sry_qtn_typ_trf', 'sry_qtn_typ_yno','sry_qtn_typ_dpn'); // Ram : sry_qtn_typ_dpn Id Not showing In Autocomplete Fixed
    $select->condition('surveyQstion.question_type', $assesArr , 'IN');
    $select->condition('surveyQstion.survey_type', 'sry_det_typ_ass' , '='); 
    $entityCode = 'sry_det_typ_ass_qus';    
  } else {
    $select->condition('surveyQstion.survey_type', 'sry_det_typ_sry' , '=');
    $entityCode = 'sry_det_typ_sry_qus';
  }
  /* Ticket: 46992 */
  if($searchType=='surasstagname'){
  	$select->AddExpression('distinct stf.tagname', 'tagname');
  	$select->innerJoin('slt_tag_entity', 'st', 'st.entity_id = surveyQstion.id');
  	$select->innerJoin('slt_tagdefn', 'stf', 'stf.id = st.tagid');
  	$select->condition('stf.tagname', '%' . db_like($searchText) . '%', 'LIKE');
  	//$select->condition('st.entity_type', $entityCode , '=');
  	$select->orderBy('tagname');
  }else{
  	$select->addField('surveyQstion', 'id', 'id');
  	$select->addField('surveyQstion', 'code', 'code');
  	$select->addField('surveyQstion', 'question_txt', 'question_txt');
  	$select->condition('surveyQstion.question_txt', '%' . db_like($searchText) . '%', 'LIKE');
  	$select->orderBy('question_txt');
  } /* Ticket: 46992 */
 if(!empty($excluded_question_id)) {
    $select->condition('surveyQstion.id', explode(",",$excluded_question_id) , 'NOT IN');    
  }
  filterByUserAccessPrivileges($select,'surveyQstion','',1,$entityCode,'attach','surveyQstion'); // #46933 - group access visiblity check
  
  expDebug::dPrintDBAPI('----$searchType select----' , $select );
  $matchingQuestionNames = $select->execute()->fetchAll();
  
  /*foreach ($matchingQuestionNames as $qstName) {    
    print $qstName->id."|".$qstName->question_txt . "\n";
  }*/
  expDebug::dPrintDBAPI('$select', $select);
  foreach ($matchingQuestionNames as $qstName) {
    if($idRequired == 0){
    	if($searchType=='surasstagname'){
      		print $qstName->tagname . "\n";
    	}else{
    		print $qstName->question_txt . "\n";
    	}
    } else {
      print $qstName->id."|".$qstName->question_txt .  ' (' . $qstName->code . ')'  . "\n";
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyQuestionAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}


function getSurveyQuestionId($searchText) {
 try{
  $select = db_select('slt_survey_questions', 'surveyQstion');
  $select->addField('surveyQstion', 'id', 'id');
  $select->addField('surveyQstion', 'question_txt', 'question_txt');
  //$select->condition('surveyQstion.question_txt', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('surveyQstion.question_txt', db_like($searchText), 'LIKE');
  $select->condition('surveyQstion.status', 'sry_qtn_sts_atv', '=');
  
  $select->orderBy('question_txt');

  $matchingQuestionNames = $select->execute()->fetchAll();
  
  return $matchingQuestionNames[0]->id;  
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyQuestionId', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSurveyModuleDetails($moduleid, $fields) {
 try{ 
  if (empty($fields)) {

    $fields = array('id','survey_id', 'title', 'code');
  } 
  // Select table
  $selectStmt = db_select('slt_survey_groups', 'modls');
  $selectStmt->condition('modls.id', $moduleid);

  foreach($fields as $fieldName) {
    $selectStmt->addField('modls', $fieldName, $fieldName);
  }
  expDebug::dPrintDBAPI('surveyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy = ' , $selectStmt);

  $result = $selectStmt->execute();
  return $result->fetchAssoc();
  } catch (Exception $ex) {
  	watchdog_exception('getSurveyModuleDetails', $ex);
  	expertusErrorThrow($ex);
  }
}


function checkGroupNameExist($searchText) {
  try{
  $select = db_select('slt_survey_groups', 'surveyGroup');
  $select->addField('surveyGroup', 'id', 'id');
  $select->addField('surveyGroup', 'title', 'group_title');
  //$select->condition('surveyGroup.title', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('surveyGroup.title', db_like($searchText), 'LIKE');
  $groupQuestion = $select->execute()->fetchAll();
  
  return array( 'id' => $groupQuestion[0]->id , 'group_title' => $groupQuestion[0]->group_title);  
  } catch (Exception $ex) {
  	watchdog_exception('checkGroupNameExist', $ex);
  	expertusErrorThrow($ex);
  }
}


function _getSurveyGroupAttachQuestions($survey_id,$group_question_id,$queryRequired = 0) {
  try{
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('sgq','id','survey_group_question_id');  
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','score','survey_score');
  $select->addField('sg','title','survey_group_title');
  $select->addField('qst','code','question_code');   
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
  $select->condition('qst.status','sry_qtn_sts_atv','=');
  
  if($group_question_id>0) {
    $select->condition('sgq.id',$group_question_id,'=');
  }   
  $select->condition('sgq.survey_id',$survey_id,'=');
  	
  if($queryRequired > 0) {
    $surveyGroupList = $select;
  } else {
    $surveyGroupList = $select->execute()->fetchAll();
  }
  $surveyGroupList = $select->execute()->fetchAll();
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true) , 4);
  
  return $surveyGroupList;
  } catch (Exception $ex) {
  	watchdog_exception('_getSurveyGroupAttachQuestions', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
function _getSurveyGroupAttachQuestions($survey_id,$group_question_id,$queryRequired = 0) {
  
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('sgq','id','survey_group_question_id');  
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','score','survey_score');
  $select->addField('sg','title','survey_group_title');
  $select->addField('qst','code','question_code');   
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
  if($group_question_id>0) {
    $select->condition('sgq.id',$group_question_id,'=');
  }   
  $select->condition('sgq.survey_id',$survey_id,'=');
  	
  if($queryRequired > 0) {
    $surveyGroupList = $select;
  } else {
    $surveyGroupList = $select->execute()->fetchAll();
  }
  $surveyGroupList = $select->execute()->fetchAll();
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true));
  
  return $surveyGroupList;
  
}*/
function getAttachQuestion($survey_id,$group_question_id,$queryRequired = 0,$callfromapi = 0) {
  try{
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('sgq','id','survey_group_question_id');  
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','score','survey_score');
  $select->addField('sg','title','survey_group_title');
  $select->addField('qst','code','question_code');   
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
  $select->addField('sgq','mandatoryOption','mandatory_option');
  $select->addField('sgq','id','id');  
  //$select->addField('qst','id','id');
  $select->addField('qst','code','question_code');  
  //$select->addField('prof','name','question_status'); 
  //$select->addField('prof1','name','question_type');
  $select->condition('qst.status','sry_qtn_sts_atv','=');
  $select->condition('sgq.survey_id',$survey_id,'=');
  if($callfromapi){
  	$select->condition('sgq.question_id',$group_question_id,'=');
  }
  $select->orderBy('sgq.sequence');
  
  if($queryRequired > 0) {
    $surveyGroupList = $select;
  } else {
    $surveyGroupList = $select->execute()->fetchAll();
  }
  //$surveyGroupList = $select->execute()->fetchAll();
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true) , 4);
  
  return $surveyGroupList;
  } catch (Exception $ex) {
  	watchdog_exception('getAttachQuestion', $ex);
  	expertusErrorThrow($ex);
  }
}

//function loadCtoolDataGridForQuestionList($survey_id,$group_question_id,$queryRequired = 0,$excludedId) {
  
function loadCtoolDataGridForQuestionList($searchKeyword, $queryRequired = '', $excludedId = '',$entityType) {
  try {
  $select = db_select("slt_survey_questions","qst");
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  
  $select->addField('qst','question_txt','question_txt');
  $select->addField('qst','id','id');
  $select->addField('qst','code','question_code');  
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
    
  $select->condition('qst.status','sry_qtn_sts_atv','=');
  $excludedId = ereg_replace('"','',$excludedId);
  if(!empty($searchKeyword) && ($searchKeyword != 'Type a question')) {
    $select->condition('qst.question_txt', '%' . db_like($searchKeyword) . '%', 'LIKE');
  }
  
  if(!empty($excludedId)){
    $select->condition('qst.id', explode(',', $excludedId) , 'NOT IN');
  }
  
  if($entityType == 'sry_det_typ_ass') {
    //sry_qtn_typ_cmt, sry_qtn_typ_dpn, sry_qtn_typ_img, sry_qtn_typ_mch, sry_qtn_typ_mch, sry_qtn_typ_rtg, sry_qtn_typ_trf, sry_qtn_typ_yno
    $assesArr = array('sry_qtn_typ_mch', 'sry_qtn_typ_trf', 'sry_qtn_typ_yno');
    $select->condition('qst.question_type', $assesArr , 'IN');
  } 
  expDebug::dPrint(' Listttttttttttttttttttttt : '.print_r($excludedId,true) , 4);
  
  if($queryRequired > 0) {
    $surveyGroupList = $select;
  } else {
    $surveyGroupList = $select->execute()->fetchAll();
  }
  
  return $surveyGroupList;  
  } catch (Exception $ex) {
  	watchdog_exception('loadCtoolDataGridForQuestionList', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSurveyInfo($surveyId) {
	try{
		$select = db_select('slt_survey', 'survey');
		$select->addField('survey', 'id', 'id');
		$select->addField('survey', 'type', 'type');
		$select->condition('survey.id',$surveyId);
		expDebug::dPrintDBAPI('select survey', $select, 5);
		$survetInfo = $select->execute()->fetchAll();
		return $survetInfo[0]->type;
	} catch (Exception $ex) {
  	watchdog_exception('getSurveyInfo', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_delete_question_repaint($form, &$form_state) {
	try{
  expDebug::dPrint(' repaintttttttttt');
  if ($form_state['triggering_element']['#value'] == 'delete') {
      $groupQuestionId  = $form_state['triggering_element']['#ajax']['group_question_id'];
      $surveyId         = $form_state['triggering_element']['#ajax']['survey_id'];
      $dynamic_div      = "delete_all_group_question_".$surveyId."_".$groupQuestionId;      
      $delete           = deleteSurveyAttachedQuestion($surveyId, $groupQuestionId);
      drupal_set_message('Question deleted successfully.');    
      $form_state['rebuild'] = TRUE; 
  }
  
  $triggerElement     = $form_state['triggering_element']['#value'];  
  $survey_id          = $form_state['triggering_element']['#ajax']['survey_id'];
  $storage_step       = $form_state['storage']['step'];  
  $form_validate      = $form_state["storage"]["form_validate"];  
  $form_validate      = ($triggerElement == 'Cancel') ? false : $form_validate; 

          
  $min_form_state     = array(
   		'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
   		're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
   		'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
   		'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
        );
        
  $form_state                                 = $min_form_state;  
  $form_state['storage']['new']['survey_id']  = $survey_id;
  $form_state['storage']['step']              = $storage_step;
  $classListrenderedForm                      = exp_ctools_modal_multi_ajaxonly_forms_renderform('exp_sp_administration_catalog_class_title_form', $form_state);
  $builtForm                                  = drupal_build_form('exp_sp_administration_survey_assesment_question_form', $form_state);
  $commands                                   = array();
  $commands[]                                 = ajax_command_replace(NULL, drupal_render($builtForm));
  $commands[]                                 = ajax_command_prepend(NULL, theme('status_messages'));
  $commands[]                                 = exp_ctools_modal_adjust();
  $commands[]                                 = ajax_command_refresh_narrow_search_results('onclose');  
  $form_state['input']                        = array();
  $form_state['values']                       = array();
  
  return array('#type' => 'ajax', '#commands' => $commands); 
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_delete_question_repaint', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * deleteSurveyAttachedQuestion() - Delete the attached question for the Survey/Assessment
 */ 
function deleteSurveyAttachedQuestion($surveyId, $questionId){
  $updatedUserId = getSltpersonUserId();
  $txn = db_transaction();
  try {
    if(!empty($surveyId) && !empty($questionId)){
        
        $sel = db_select('slt_survey_groups_questions','sur');
        $sel->addField('sur','id','id');
        $sel->addField('sur','survey_group_id','survey_group_id');
        $sel->condition('survey_id', $surveyId,'=');
        $sel->condition('question_id', $questionId,'=');
        $sel1 = $sel->execute()->fetchAll();
        
	    $deleteStmt = db_delete('slt_survey_groups_questions');
	    $deleteStmt->condition('survey_id', $surveyId,'=');
	    $deleteStmt->condition('question_id', $questionId,'=');
	    $numUpdated = $deleteStmt->execute();
	   
	    foreach($sel1 as $val){
	        $fields = array(
	                'table_name' => 'slt_survey_groups_questions',
	                'entity_id' => $val->id,
	                'entity_type' => 'question_mapping',
	                'module_name' => 'exp_sp_administration_survey_assesment_question',
	                'functionality_name' => 'deleteSurveyAttachedQuestion',
	                'api_name' => null,
	                'deleted_on' => now(),
	                'deleted_by' => getIdOfLoggedInUser(),
	                'parent1_entity_id' => $surveyId,
	                'parent1_entity_type' => 'survey_id',
	                'parent2_entity_id' => $val->survey_group_id,
	                'parent2_entity_type' => 'survey_group_id',
	                'parent3_entity_id' => $questionId,
	                'parent3_entity_type' => 'question_id',
	        );
	        deleted_log_entry($fields);
	    }
	    
	    add_audit_trail_entry($updatedUserId, $surveyId, 'sry_det_typ_sry', 'exp_sp_administration_survey_assessment', 'deleteSurveyAttachedQuestion', 'Question Deleted',null,$questionId);
	    // Resequence attached courses and their groups
        expDebug::dPrint(' Re-sequencing attached courses after delete');
        $sequenceOrder = getSurveyAttachedQuestionSequenceOrder($surveyId);
        $usedModuleIds = sequenceAttachQuestion($sequenceOrder);
	    
	    $select = db_select('slt_survey_groups_questions', 'group_question');
	   	$select->addExpression('COUNT(1)','total_group_enroll');
	   	$select->condition('group_question.survey_id', $surveyId, '=');
	  	$surAssQuestListCount = $select->execute()->fetchField();
	
	    if($surAssQuestListCount == 0){
	        $selectSurveyStatus = db_select('slt_survey', 'survey');
	        $selectSurveyStatus->addField('survey','status');
	        $selectSurveyStatus->condition('survey.id',$surveyId);
	        expDebug::dPrintDBAPI(' $select obj = ' , $selectSurveyStatus);
	        $surveyStatus = $selectSurveyStatus->execute()->fetchField();
	        
	        if($surveyStatus == 'sry_det_sry_atv'){
	        	$updatestmt = db_update('slt_survey');
	        	$updatestmt->fields(array('status' => 'sry_det_sry_itv',
	        					     'updated_on' => now(),     
	              				    'updated_by' => $updatedUserId
	                          ));
	            $updatestmt->condition('id', $surveyId, '=');
	            $result = $updatestmt->execute();        	
	        }
	    }
    }
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteSurveyAttachedQuestion', $ex);
    throw $ex;
  }
  unset($txn); 
}

function _fetchQuestionList($survey_id,$group_question_id) {
  try{
  $select = db_select("slt_survey_groups_questions","sgq");
  $select->innerJoin('slt_survey_groups', 'sg', 'sg.id = sgq.survey_group_id');
  $select->innerJoin('slt_survey_questions', 'qst', 'qst.id = sgq.question_id');
  $select->innerJoin('slt_profile_list_items', 'prof', 'prof.code = qst.status');
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = qst.question_type');
  $select->addField('qst','question_txt','question_txt');
  $select->addField('sgq','id','survey_group_question_id');
  $select->addField('qst','id','survey_question_id');  
  $select->addField('sgq','sequence','survey_group_sequence');
  $select->addField('sgq','score','survey_score');
  $select->addField('sg','title','survey_group_title');
  $select->addField('qst','code','question_code');   
  $select->addField('prof','name','question_status'); 
  $select->addField('prof1','name','question_type');
  if($group_question_id>0) {
    $select->condition('sgq.id',$group_question_id,'=');
  }   
  $select->condition('sgq.survey_id',$survey_id,'=');
  $select->condition('qst.status','sry_qtn_sts_atv','=');	
  $totalRecords = $select->countQuery()->execute()->fetchField();
  $surveyGroupList = $select->execute()->fetchAll();
  $surveyGroupList['totalrow'] = $totalRecords;
  expDebug::dPrintDBAPI($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :', $select);
  
  expDebug::dPrint($survey_id.' == group_question_id :'.$group_question_id.' || surveyGorupList 123434444 :'.print_r($surveyGroupList,true) , 3);
  
  return $surveyGroupList;
  } catch (Exception $ex) {
  	watchdog_exception('_fetchQuestionList', $ex);
  	expertusErrorThrow($ex);
  }
}

function deleteSurvAssesQuestionDetails($entityId, $entityType, $attacgQustionIdList) {
  
  $attacgQustionIdArr = explode('|', $attacgQustionIdList);
  expDebug::dPrint('Delete question id list '.print_r($attacgQustionIdArr,true) , 4);
  $txn = db_transaction();
  try {
   
    if(count($attacgQustionIdArr) > 0){
        
           // deleted_log_entry('slt_survey_groups_questions',$val,'question','exp_sp_administration_survey_assesment_question','deleteSurvAssesQuestionDetails',null,getIdOfLoggedInUser());
        $sel = db_select('slt_survey_groups_questions','sur');
        $sel->addField('sur','id','id');
        $sel->addField('sur','survey_id','survey_id');
        $sel->addField('sur','survey_group_id','survey_group_id');
        $sel->addField('sur','question_id','question_id');
        $sel->condition('id', $attacgQustionIdArr,'IN');
        $sel->condition('question_id', $questionId,'=');
        $sel1 = $sel->execute()->fetchAll();
        
        $deleteStmt = db_delete('slt_survey_groups_questions');
        $deleteStmt->condition('id', $attacgQustionIdArr,'IN');
        expDebug::dPrintDBAPI(' $deleteStmt123 ' , $deleteStmt);
        $numDeleted = $deleteStmt->execute();
        
        foreach($sel1 as $val){
            $fields = array(
                    'table_name' => 'slt_survey_groups_questions',
                    'entity_id' => $val->id,
                    'entity_type' => 'question',
                    'module_name' => 'exp_sp_administration_survey_assesment_question',
                    'functionality_name' => 'deleteSurvAssesQuestionDetails',
                    'api_name' => null,
                    'deleted_on' => now(),
                    'deleted_by' => getIdOfLoggedInUser(),
                    'parent1_entity_id' => $val->survey_id,
                    'parent1_entity_type' => 'survey_id',
                    'parent2_entity_id' => $val->survey_group_id,
                    'parent2_entity_type' => 'survey_group_id',
                    'parent3_entity_id' => $val->question_id,
                    'parent3_entity_type' => 'question_id',
            );
            deleted_log_entry($fields);}
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteSurvAssesQuestionDetails', $ex);
    throw $ex;
  }

  unset($txn);
  
}

function getMaxScoreForAssesment($entityId) {
try{
  $select = db_select('slt_survey', 'sur');  
  $select->addExpression('sur.max_mark', 'max_score_value');  
  $select->condition('sur.id', $entityId,'=');  
  $select->condition('sur.type','sry_det_typ_ass', '=');
    
  $result = $select->execute();
  
  if($result->rowCount()>0){
    $max_score_results = $result->fetchAll();
    return $max_score_results[0]->max_score_value;
  } else {
    return 0;
  }
  } catch (Exception $ex) {
  	watchdog_exception('getMaxScoreForAssesment', $ex);
  	expertusErrorThrow($ex);
  }
}

function maxScoreValidate($entityId) {
try {
  $select = db_select('slt_survey', 'sur');//, 'sur.id = sur_map.survey_id');  
  $select->join('slt_survey_groups_questions', 'sur_grp_ques', 'sur.id = sur_grp_ques.survey_id');
  $select->join('slt_survey_groups', 'sur_grp', 'sur_grp.id = sur_grp_ques.survey_group_id');
  $select->addExpression('sur.max_mark', 'max_score_value');
  $select->addExpression('SUM(sur_grp_ques.score)', 'total_score');
  $select->condition('sur.id', $entityId,'=');  
  $select->condition('sur.type','sry_det_typ_ass', '=');
  $select->condition('sur_grp.status','sry_det_grp_atv', '=');
  
  $result = $select->execute();
  if($result->rowCount()>0){
    $max_score_results = $result->fetchAll();
    foreach($max_score_results as $value){
        if($value->max_score_value == $value->total_score) {
          return 0; 
        } else { 
          return $value->total_score == 0 ? 1 : $value->total_score; 
        }
    }
  }	else {
    return 1;
  }
  } catch (Exception $ex) {
  	watchdog_exception('maxScoreValidate', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewSurveygroupByRestAPI($groupInfo,$createuserid=""){
	try{
		$groupInfo['userid'] = $createuserid;
		expDebug::dPrint('addNewSurveygroupByRestAPI group info : ' . print_r($groupInfo,true) , 4);
		$survey = getSurveyInfo($groupInfo['survey_id']);
		if(empty($survey)) {
			$error -> errormsg = t('Invalid survey Id');
			$error -> isValidateError = true;
			return $error;
		}
		$pattern = "/^[A-Za-z0-9.,@_\s]{0,100}$/";
	    if(!preg_match($pattern, $groupInfo['survey_group'])){
            $error -> errormsg = t('MSG644').' '.t('MSG639');
            $error -> isValidateError = true;
            return $error;
	    }
		$surveyGroupId =  _addNewSurveyGroup($groupInfo);
		expDebug::dPrint('addNewSurveygroupByRestAPI created Survey group : ' . print_r($surveyGroupId,true) , 4);
		if($surveyGroupId){
	  return array((object)array('Id'=>$surveyGroupId));
		}
		else{
	  //return array((object)array('Id'=>'Failure'));
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "invalid survey id";
			return $errobj;
		}
	} catch (Exception $ex) {
		watchdog_exception('addNewSurveygroupByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function attachQuestionToSurveyByRestAPI($groupInfo,$createuserid=""){
	try{
		$error = new stdClass();
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$groupInfo['userid'] = $createuserid;
		expDebug::dPrint('attachQuestionToSurveyByRestAPI group info : ' . print_r($groupInfo,true) , 4);
		$check_surveyId = getSurveyInfo($groupInfo['survey_id']);
		$check_surveyGrpId = getSurveyGroupInfo($groupInfo['survey_group_id']);
		$check_surveyQuesId = getSurveyQuestionInfo($groupInfo['survey_question_id']);
		expDebug::dPrint('$check_surveyId : ' . print_r($check_surveyId,true) , 4);
		
		$apiname = (isset($_GET['apiname']) ? $_GET['apiname'] : $_POST['apiname']);
		$checkType = ($apiname == 'AttachQuestionsToSurveyAPI' ? 'sry_det_typ_sry' : 'sry_det_typ_ass'); // Type Assement or Survey
		
		if(empty($check_surveyId) || $check_surveyId != $checkType){ // Survey id Validation
			$error -> errormsg = 'Invalid Survey Id';
			$error -> isValidateError = true;
			return $error;
		}
		if(empty($check_surveyGrpId)) { // Survey Group id Validation
			$error -> errormsg = 'Invalid Survey Group Id';
			$error -> isValidateError = true;
			return $error;
		}
		if($check_surveyGrpId !='sry_det_grp_atv'){
			$error -> errormsg = 'Survey Group in Unpublished status';
			$error -> isValidateError = true;
			return $error;
		}
		if(empty($check_surveyQuesId)) { // Survey Question id Validation
			$error -> errormsg = 'Invalid Survey Question Id';
			$error -> isValidateError = true;
			return $error;
		}
		if($check_surveyQuesId !='sry_qtn_sts_atv'){
			$error -> errormsg = 'Survey Question in Unpublished status';
			$error -> isValidateError = true;
			return $error;
		}
		$mandOption = array('Y','N');
		if(!in_array($groupInfo['mandatoryOption'],$mandOption)){ // Survey Mandatory option validation
			$error -> errormsg = 'Mandatory Option must be Y or N';
			$error -> isValidateError = true;
			return $error;
		}
		if($check_surveyId == 'sry_det_typ_ass'){
			$surveyAttachedId = getAttachQuestion($groupInfo['survey_id'],$groupInfo['survey_question_id'],$queryRequired = 0,1);
			if(!empty($surveyAttachedId)){
				$error -> errormsg = 'Survey Group question id already attached to this Assessment';
				$error -> isValidateError = true;
				return $error;
			}
			if(empty($groupInfo['sequence']) || !is_numeric($groupInfo['sequence']) ){
				$error -> errormsg = 'Survey sequence must be greater than 0 or Numeric';
				$error -> isValidateError = true;
				return $error;
			} 
			$seqOrderRecList = getSurveyAttachedQuestionSequenceOrder($groupInfo['survey_id'] , 1);
			$checkSequence = false;
			foreach ($seqOrderRecList as $seqOrderRec) {
				if($seqOrderRec->sequence == $groupInfo['sequence']){
					$checkSequence = true;
					break;
				}
			}
			if($checkSequence){
				$error -> errormsg = 'Sequence Already used.';
				$error -> isValidateError = true;
				return $error;
			}
			$maxsequence = count($seqOrderRecList);
			if($groupInfo['sequence']!=($maxsequence+1)){
				$error -> errormsg = 'incorrect sequence';
				$error -> isValidateError = true;
				return $error;
			}
			expDebug::dPrint('$check : '.print_r($check , true), 4);
			
		}
		$attachQuestionId =  _addNewSurveyQuestionGroup($groupInfo);
		expDebug::dPrint('attachQuestionToSurveyByRestAPI attached Survey question : ' . print_r($attachQuestionId,true) , 4);
		if($attachQuestionId){
	  		return array((object)array('Id'=>$attachQuestionId));
		}
		else{
	  		$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Error in attaching question to survey";
				return $errobj;
		  	//return array((object)array('Id'=>'Failure'));
		}
	} catch (Exception $ex) {
		watchdog_exception('attachQuestionToSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateAttachedQuestionsInSurveyByRestAPI($groupInfo,$createuserid=""){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$groupInfo['userid'] = $createuserid;
		expDebug::dPrint('updateAttachedQuestionsInSurveyByRestAPI group info : ' . print_r($groupInfo,true) ,4);
		/* Validation starts */
		$check_surveyGrpQuesId  = getSurveyGrpQuestionInfo($groupInfo['group_question_id']);
		$check_surveyGrpId 		= getSurveyGroupInfo($groupInfo['survey_group_id']);
		
        if($_REQUEST['apiname'] == 'UpdateAttachedQuestionsInSurveyAPI'){
        	$entity_type = 'sry_det_typ_sry';
        }
        if($_REQUEST['apiname' ] == 'UpdateAttachedQuestionsInAssessmentAPI'){
        	$entity_type = 'sry_det_typ_ass';
        }
		if(empty($check_surveyGrpQuesId)) { // Survey Question id Validation
			$error -> errormsg = 'Invalid Survey Group Question Id';
			$error -> isValidateError = true;
			return $error;
		}
		if(empty($check_surveyGrpId)) { // Survey Group id Validation
			$error -> errormsg = 'Invalid Survey Group Id';
			$error -> isValidateError = true;
			return $error;
		}
		$Accesscheckingrpmap = objectLevelAccessCheckInGroupMap($entity_type,array($check_surveyGrpQuesId[0]->survey_id));
		if(!empty($Accesscheckingrpmap)){
		$privcheck = apiUseraccesscheck($groupInfo['userid'],$entity_type,$check_surveyGrpQuesId[0]->survey_id);
		if(empty($privcheck)){
			$error -> errormsg = 'User dont have this privilege';
			$error -> isValidateError = true;
			return $error;
		}
		
		}
		
		if($check_surveyGrpId !='sry_det_grp_atv'){
			$error -> errormsg = 'Survey Group in Unpublished status';
			$error -> isValidateError = true;
			return $error;
		}
		$mandOption = array('Y','N');
		if(!in_array($groupInfo['mandatoryOption'],$mandOption)){ // Survey Mandatory option validation
			$error -> errormsg = 'Mandatory Option must be Y or N';
			$error -> isValidateError = true;
			return $error;
		}
		/* validation Ends */
		$updatedQuestionId =  _updateSurveyQuestionGroup($groupInfo);
		expDebug::dPrint(' updateAttachedQuestionsInSurveyByRestAPI updated Survey question:' . print_r($updatedQuestionId,true) , 4);
		if($updatedQuestionId){
			return array((object)array('Id'=>$updatedQuestionId));
		}
		else{
			return array((object)array('Id'=>'Failure'));
		}
	} catch (Exception $ex) {
		watchdog_exception('updateAttachedQuestionsInSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function listAttachedQuestionInSurveyByRestAPI($surveyId,$createuserid=""){
	try{
		$error = new stdClass();
		expDebug::dPrint('listAttachedQuestionInSurveyByRestAPI survey id : ' . $surveyId , 4);
		$getinfo = getSurveyInfo($surveyId);
		
		$apiname = (isset($_GET['apiname']) ? $_GET['apiname'] : $_POST['apiname']);
		$checkType = ($apiname == 'ListAttachedQuestionsInSurveyAPI' ? 'sry_det_typ_sry' : 'sry_det_typ_ass'); // Type Assement or Survey
		if(empty($getinfo) || $getinfo != $checkType){ // Validate for invalid Survey id
			$error -> isValidateError = true;
			$error -> errormsg = 'Invalid Survey id';
			return $error;
		}
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		$attachQuestions =  _fetchQuestionList($surveyId , 0);
		return $attachQuestions;
	} catch (Exception $ex) {
		watchdog_exception('listAttachedQuestionInSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * Get Survey Assessment - Question/Group Autocomplete filter
 */
function getSurveyAssessmentQuestionTextFilterAutoComplete($entityId) {
	try{
		$searchText    =  $_GET['z'];
		$searchType    =  $_GET['search_type'];

		if($questionType =='assessment') {
			$question_code  = 'sry_det_typ_ass';
		}else{
			$question_code = 'sry_det_typ_sry';
		}

		if($searchType =='surassqus'){
			$select = db_select('slt_survey_groups_questions', 'surveygroupques');
			$select->leftjoin('slt_survey_groups', 'sg', 'sg.id = surveygroupques.survey_group_id');
			$select->leftjoin('slt_survey_questions', 'sq', 'sq.id = surveygroupques.question_id');
			$select->addField('sq', 'question_txt', 'question');
			$select->condition('sq.question_txt', '%' . db_like($searchText) . '%', 'LIKE');
			//$select->condition('sq.survey_type', $question_code,'=');
			$select->condition('surveygroupques.survey_id', $entityId,'=');
			$select->orderBy('question');
			$select->groupBy('question');
		}else{
			$select = db_select('slt_survey_groups_questions', 'surveygroupques');
			$select->leftjoin('slt_survey_groups', 'sg', 'sg.id = surveygroupques.survey_group_id');
			$select->addField('sg', 'title', 'question');
			$select->condition('sg.title', '%' . db_like($searchText) . '%', 'LIKE');
			$select->condition('sg.status',array('sry_det_grp_atv'),'IN');
			$select->condition('surveygroupques.survey_id', $entityId,'=');
			$select->orderBy('question');
			$select->groupBy('question');
		}
		expDebug::dPrintDBAPI(' $select obj = ' , $select);
		expDebug::dPrintDBAPI('$select with auto_complete', $select);
		$matchingSurveyDetailsQuestionNames = $select->execute()->fetchAll();

		foreach ($matchingSurveyDetailsQuestionNames as $surveyDetailsQuestionName) {
			print $surveyDetailsQuestionName->question . "\n";
		}
	} catch (Exception $ex) {
		watchdog_exception('getSurveyAssessmentQuestionTextFilterAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

function getSurveyAttachedQuestionSequenceOrder($surveyId,$callfromapi=0) {
 try {
    $selectQry1 = db_select('slt_survey_groups_questions', 'sgs');
    $selectQry1->addField('sgs', 'sequence', 'sequence');
    $selectQry1->addField('sgs', 'id', 'id');
    $selectQry1->condition('sgs.survey_id', $surveyId);
	expDebug::dPrintDBAPI(' SQL = ' , $selectQry1);
    $seqOrderRecList = $selectQry1->execute()->fetchAll();
    expDebug::dPrint(' $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
    usort($seqOrderRecList, "surveyAttachedQuestionSeqOrderRecCompare");
    expDebug::dPrint(' after usort, $seqOrderRecList = ' .print_r($seqOrderRecList, true) , 3);
    if($callfromapi){
    	return $seqOrderRecList;
    }
    $sequenceOrder = '';
    $first = true;
    foreach ($seqOrderRecList as $seqOrderRec) {
      if ($first) {
       $sequenceOrder .= $seqOrderRec->id ;
       $first = false;
      }
      else {
      	$sequenceOrder .= ',' . $seqOrderRec->id ;
      }
    } // end foreach
    expDebug::dPrint(' $sequenceOrder = ' . print_r($sequenceOrder, true) , 4);
    
    return $sequenceOrder;
  }
  catch (Exception $ex) {
    watchdog_exception('getSurveyAttachedQuestionSequenceOrder', $ex);
    throw $ex;    
  }
}

/*
 * surveyAttachedQuestionSeqOrderRecCompare() - Function called by usort in surveyAttachedQuestionSeqOrderRecCompare() for sorting records by sequence number
 */
function surveyAttachedQuestionSeqOrderRecCompare($seqOrderRecA, $seqOrderRecB) {
	try{
		return ($seqOrderRecA->sequence - $seqOrderRecB->sequence);
	} catch (Exception $ex) {
		watchdog_exception('surveyAttachedQuestionSeqOrderRecCompare', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * @param id $surveyGrpId
 * @return id
 * @desc Check the id is present in database.
 */
function getSurveyGroupInfo($surveyGrpId) {
	try{
		$select = db_select('slt_survey_groups', 'surgroup');
		$select->addField('surgroup', 'status', 'status');
		$select->condition('surgroup.id',$surveyGrpId);
		expDebug::dPrintDBAPI('$select : ',$select);
		$survetInfo = $select->execute()->fetchField();
		return $survetInfo;
	} catch (Exception $ex) {
		watchdog_exception('getSurveyGroupInfo', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * @param id $surveyGrpId
 * @return id
 * @desc Check the id is present in database.
 */
function getSurveyQuestionInfo($surveyQueId) {
	try{
		$select = db_select('slt_survey_questions', 'surque');
		$select->addField('surque', 'status', 'status');
		$select->condition('surque.id',$surveyQueId);

		$survetInfo = $select->execute()->fetchField();
		return $survetInfo;
	} catch (Exception $ex) {
		watchdog_exception('getSurveyQuestionInfo', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * @param id $surveyGrpId
 * @return id
 * @desc Check the id is present in database.
 */
function getSurveyGrpQuestionInfo($surveyQueId) {
	try{
		$select = db_select('slt_survey_groups_questions', 'surgrpque');
		$select->addField('surgrpque', 'id', 'id');
		$select->addField('surgrpque', 'survey_id', 'survey_id');
		$select->condition('surgrpque.id',$surveyQueId);

		$survetInfo = $select->execute()->fetchAll();
		return $survetInfo;
	} catch (Exception $ex) {
		watchdog_exception('getSurveyQuestionInfo', $ex);
		expertusErrorThrow($ex);
	}
}
?>