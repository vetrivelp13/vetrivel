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
 * Implementation of hook_init().
 */
function exp_sp_administration_survey_assessment_init() {
	try{
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_custom_fields.inc');
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_survey_assessment_init', $ex);
  	expertusErrorThrow($ex);
  }
}

 /**
 * exp_sp_administration_survey_assessment_addedit_form() - Build the addedit form for Survey/Assessment.
 */
function exp_sp_administration_survey_assessment_addedit_form($form, &$form_state, $argumentType, $renderMode, $surveyId){
  try{
  	includeFilesForsurvey();
   expDebug::dPrint('Params value'.print_r($form_state['complete form'],true) , 5);
   expDebug::dPrint('Params valuesssss all values '.print_r($form_state['values'],true) , 5);
   expDebug::dPrint('Params valuesssss122 ARGUMENTTYPE:'.$argumentType.' , RENDERMODE:'.$renderMode.' , SURVID:'.$surveyId , 5);

   if($form_state['storage']['step']=='edit'){
     $formTypeMode  = $form_state['build_info']['args'][0];
   $form_state['storage']['form_type']=$formTypeMode;
   }else{
   $formTypeMode  = $form_state['build_info']['args'][1];
   $form_state['storage']['form_type']=$formTypeMode;
   }
   expDebug::dPrint('Params value $argumentType'.print_r($argumentType,true) , 4);
   expDebug::dPrint('Params value $renderMode '.print_r($renderMode,true) , 4);
   $js_module_optional   = array('type' => 'file', 'group' => JS_DEFAULT);
   drupal_add_js(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_surveydetails.js', $js_module_optional);


    if (empty($form_state['storage'])) {
       /* if(!empty($surveyId) && empty($form_state['storage'])) {
            $form_state['storage']['step'] = 'view';
        } else */
      if(empty($surveyId) && empty($form_state['storage'])) {
            $form_state['storage']['step'] = 'add';
        }
    }

    $surveyDetails     = array();
    if(empty($surveyId)){
      $surveyId=$form_state['values']['survey_id'];
    }
    if(empty($surveyId) && !empty($form_state['build_info']['args']["entity_id"])){
      $surveyId=$form_state['build_info']['args']["entity_id"];
    }
    $emptyId = '' ;
    if (!empty($surveyId)) {
      $surveyDetails   = _getSurveyAssessmentDetails($surveyId);
      $form_state['storage']['form_type']  = ($surveyDetails[0]->survey_type == 'sry_det_typ_ass') ? 'assessment' : 'survey';
    }else{
    	$emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
    	$enityType = ($renderMode == 'assessment') ? 'sry_det_typ_ass' : 'sry_det_typ_sry' ;
    }


    $form 		       = newAddEditFormWithCancelAndSave('catalog-survey-assesment-basic-addedit-form');
    $fieldList         = array('survey_id','survey_title', 'survey_code', 'survey_lang_code','survey_description', 'survey_language', 'survey_status', 'survey_qust_per_page', 'survey_type', 'min_marks', 'max_marks','survey_language_name', 'survey_status_name', 'survey_type_name','status_inactive_reason','random','random_name');
    $defaultValue      = getDefaultSurveyAssesmentFormFieldValue($fieldList, $form_state, $surveyDetails[0]);
   expDebug::dPrint('Survey details parama'.print_r($defaultValue,true) , 4);
   expDebug::dPrint('Survey details parama123456 : '.print_r($form_state['values'],true) , 5);
    if($form_state['storage']['step'] == 'view') {
        edit_survey_view($form, $form_state, $defaultValue);
    } else {
    		addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
    		addHiddenFieldToAddEditForm($form, 'entity_value', $enityType);
        addHiddenValueToAddEditForm($form, 'id', $surveyId);
        addHiddenValueToAddEditForm($form, 'survey_id', $defaultValue['survey_id']);
        addTextFieldToAddEditForm($form, 'survey_title', t('LBL083'), $defaultValue['survey_title'], 1, true);
        if($form_state['storage']['form_type'] == 'assessment'){
          $tabIndex1 = 3;
          $tabIndex2 = 8;
        }else{
          $tabIndex1 = 4;
          $tabIndex2 = 9;
        }
        addTwoColumnRowToAddEditForm($form, 'lang_status');
        addTextFieldToAddEditForm($form, 'survey_code', t('LBL096'), $defaultValue['survey_code'], 2, true,false,100);
        addTextFieldToAddEditForm($form, 'survey_qust_per_page', t('LBL328'), $defaultValue['survey_qust_per_page'], $tabIndex1, true);
        endAddEditTwoColumnRow($form);

        addTwoColumnRowToAddEditForm($form, 'min_marks_disp_id');
        $defaultValue['survey_lang_code'] = !empty($defaultValue['survey_lang_code']) ? $defaultValue['survey_lang_code'] : 'cre_sys_lng_eng';
        addDropDownToAddEditForm($form, 'survey_language', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue['survey_lang_code'], 4, true);

        if($form_state['storage']['form_type'] == 'assessment'){
          addTextFieldToAddEditForm($form, 'min_marks', t('LBL329'), $defaultValue['min_marks'], 5, true);
        }
        endAddEditTwoColumnRow($form);

        if($form_state['storage']['form_type'] == 'assessment'){
            addTwoColumnRowToAddEditForm($form, 'min_marks_disp');
            addDropDownToAddEditForm($form, 'assess_random', t('LBL955'),getProfileItemNamesKeyed('sry_det_typ_ass_ran_', 'cre_sys_lng_eng', false, '', '' ,''), $defaultValue['random'], 6, false);
            addTextFieldToAddEditForm($form, 'max_marks', t('LBL330'), $defaultValue['max_marks'], 7, true);
            endAddEditTwoColumnRow($form);
        }
        addHiddenFieldToAddEditForm($form, 'ckeditor_word_count', '5000');
        $form['addedit-long-description-container'] = array(
          '#type'   => 'markup',
          '#markup' => '<div class="addedit-long-description-container">',
        );
        addTextFormatFieldToAddEditForm($form, 'survey_description', t('LBL229'), $defaultValue['survey_description'], 'full_html', $tabIndex2, true, false, 5000);
        $form['addedit-long-description-container1'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
        );

        //addTextAreaFieldToAddEditForm($form, 'survey_description', t('LBL229'), $defaultValue['survey_description'], 3, false);

        /*addTwoColumnRowToAddEditForm($form, 'lang_status');
        $defaultValue['survey_lang_code'] = !empty($defaultValue['survey_lang_code']) ? $defaultValue['survey_lang_code'] : 'cre_sys_lng_eng';
	    addDropDownToAddEditForm($form, 'survey_language', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue['survey_lang_code'], 4, true);

        $statusUniqueCode = 'SurveyDetail';
	    $attributes['onchange'][] = 'getStatusReason(this,\''.$statusUniqueCode.'\');';
	    $defaultValue['survey_status']   = !empty($defaultValue['survey_status']) ? $defaultValue['survey_status'] : 'sry_det_sry_atv';
	    addDropDownToAddEditForm($form, 'survey_status', t('LBL102'), getProfileItemNamesKeyed('sry_det_sry_'), $defaultValue['survey_status'], 5, true, false, $attributes);
        endAddEditTwoColumnRow($form);
        */
        /* -------Inactive Reason text Area Start----------*/
        if($defaultValue['survey_status'] == 'sry_det_sry_itv') {
          $displayPage = "display:none";
        }else{
          $displayPage = "display:none";
        }

        $form['admin_status_inactive_disp_start'] = array(
      		'#type' => 'markup',
      		'#markup' => '<div id="admin-status-inactive-part-'.$statusUniqueCode.'" style="'.$displayPage.'">'
        );

    	addTextAreaFieldToAddEditForm($form, 'status_inactive_reason', t('Inactive Reason'), $defaultValue['status_inactive_reason'], 8, false);

    	$form['admin_status_inactive_disp_end'] = array(
      		'#type' => 'markup',
      		'#markup' => '</div>'
      	);
      	/* -------Inactive Reason text Area End----------*/


       // addTwoColumnRowToAddEditForm($form, 'question_per_page');

        $defaultValue['survey_type']     = !empty($defaultValue['survey_type']) ? $defaultValue['survey_type'] : '';
	    //addDropDownToAddEditForm($form, 'survey_type', t('Type'), getProfileItemNamesKeyed('sry_det_typ_'), $defaultValue['survey_type'], 7, true);

      // sry_det_typ_ass , sry_det_typ_sry
	 /* $readOnly                       = !empty($defaultValue['survey_type']) ? (($defaultValue['survey_type'] == 'sry_det_typ_sry') ? true : false) : false;
      $attributes                     = array();
      $attributes['autocomplete']     = 'off';
      $attributes['class']            = array('addedit-edit-survey_type');
      $attributes['class'][]          = 'addedit-edit-dropdown';

      if ($readOnly) {
        $attributes['class'][]        = 'addedit-readonly-dropdown';
      }

      $attributes['class'][]           = 'addedit-form-expertusone-throbber';
      $attributes['data-wrapperid']    = array('catalog-survey-assesment-basic-addedit-form');
      $attributes['onChange']         = 'showSurvType();';
      //$attributes['onChange'][]         = '$(document).ready(function(){ editSurveyAssesment(); });';
      $attributes['tabindex']         = 6;
      $nullOption[]                   = 'Select';
      $options                        = getProfileItemNamesKeyed('sry_det_typ_');
      $finalOptions                   = $nullOption + $options;
      $defaultOption                  = $defaultValue['survey_type'];

	    $form['addedit_tmp_storage']['two_column']['current_column'] = 1;
	    $form['survey_type'] = array(
	      '#id' => 'survey_type',
          '#type' => 'select',
          '#prefix' => addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', t('LBL036'), true),
          '#suffix' => addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD'),
          '#title' => t('LBL036'),
          '#title_display' => 'invisible',
          '#options'=> $finalOptions,
          '#default_value' => $defaultOption,
          //'#disabled' => $readOnly,
          '#attributes' => $attributes,
  	      '#ajax' => array(
                      'wrapper'        => 'min_max_mark_container_load',
                      'callback'       => 'question_type_callback',
                      'method'         => 'replace',
                      'effect'         => 'none',
                      'event'          => 'change',
                      'keypress'       => true,
                      'progress' => array(
                        'type'     => 'throbber',
                        'message'  => ''
                      ),
                  )
        );
        */
       $form['addedit-common-fields-after-editor-start'] = array(
          '#type'   => 'markup',
          '#markup' => '<div id="quesper_min_max">',
        );
      if($form_state['storage']['form_type']=='assessment'){
          //$form['survey_type']='sry_det_typ_asss';
          addHiddenValueToAddEditForm($form, 'survey_type', 'sry_det_typ_ass');
            $defaultValue['survey_type'] ='sry_det_typ_ass';
            $readOnlyMarkFld = !empty($defaultValue['survey_type']) ? (($defaultValue['survey_type'] == 'sry_det_typ_ass') ? true : false) : true;
            $readOnlyMarkFld = ($surveyId>0) ? $readOnlyMarkFld : false;

            /*$form['surv_min_max_container_start'] = array(
              '#markup'   => '<div id="min_max_mark_container"><div id="min_max_mark_container_load" '.(($defaultValue['survey_type'] == 'sry_det_typ_ass') ? ('style="display:block;"') : ('style="display:none;"')).'>',
            );



           // $readOnlyMarkFld = false;
            //$readOnlyMarkFld = empty($readOnlyMarkFld) ? false : $readOnlyMarkFld;
            addTwoColumnRowToAddEditForm($form, 'min_marks_disp');
            //addTextFieldToAddEditForm($form, 'min_marks', t('LBL329'), $defaultValue['min_marks'], 5, true);
            //addTextFieldToAddEditForm($form, 'max_marks', t('LBL330'), $defaultValue['max_marks'], 6, true);
            endAddEditTwoColumnRow($form);
            $form['surv_min_max_container_end'] = array(
              '#markup'   => '</div></div>',
            );*/
        }else{

         // $form['survey_type']='sry_det_typ_sry';
          addHiddenValueToAddEditForm($form, 'survey_type', 'sry_det_typ_sry');
        }
         if(empty($surveyId)){
            $entityId = $form_state['values']['survey_id'];
        }else{
            $entityId = $surveyId;
        }
	    //$form['addedit_tmp_storage']['two_column']['current_column'] = 2;


       // endAddEditTwoColumnRow($form);


        // if($form_state['storage']['form_type']=='assessment'){ sry_det_typ_ass sry_det_typ_sry
        //$entityType = 'survey-assessment';
        $entityType = ($form_state['storage']['form_type']=='assessment') ? 'sry_det_typ_ass' : 'sry_det_typ_sry';
        $uniqueId = $entityId.'-'.$entityType;
        //displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId, TRUE);
        //To display custom fields
        //addCustomFieldsBlockToAddEditForm($form, $form_state, $defaultValue['crs_id'], 'course', 'course-custom-fields'.$defaultValue['crs_id'], 'catalog-course-basic-addedit-form', 6);
        
        if(!module_exists('exp_sp_administration_customattribute')){ //#custom_attribute_0078975 - Check module status 
        addCustomFieldsBlockToAddEditForm($form, $form_state, $entityId, $entityType, $uniqueId, 'catalog-survey-assesment-basic-addedit-form', 11);
        } //#custom_attribute_0078975 - End Check module status 
        
        
       $form['addedit-common-fields-after-editor-end'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
        );
        if(!empty($entityId)){
          /*$form['list_type_'.$uniqueId] = array(
              '#type' => 'select',
              '#prefix' => '<div class="admin-course-add-attachment admin-course-add-attachment-select custom-attr-dropdown-cls">',
              '#suffix' => '</div>',
              '#options' => array(
                ''   => t('Add'),
                'custom_fieldset'         => t('LBL276'),
              ),
              '#attributes' => array('onchange' => 'showHide(this, "'.$uniqueId.'");','tabindex' => '10','Class' => 'select'),
          );*/
        /*$form['list_type_'.$uniqueId] = array(
	        '#type' => 'markup',
	        '#prefix' => '<div class="admin-survey-add-attachment admin-course-add-attachment-select"><span onclick="displayResourceActionList()" class="resource-add-action-wrapper">'.t('LBL287').'</span> <ul class="resource-add-list">',
    	);
	    $form['list_type_custom_'.$uniqueId] = array(
	        '#type' => 'markup',
	        '#prefix' => '<li id="custom_fieldset"><a href="javascript:void(0);" onclick="showHide(this, \''.$uniqueId.'\')">'.t('LBL276').'</a></li>',
	    );
	    $form['list_type_end_'.$uniqueId] = array(
	        '#type' => 'markup',
	        '#suffix' => '</ul></div>',
	    );*/
        }


        //displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId, TRUE);
        //To display custom fields
        //addCustomFieldsBlockToAddEditForm($form, $form_state, $defaultValue['crs_id'], 'course', 'course-custom-fields'.$defaultValue['crs_id'], 'catalog-course-basic-addedit-form', 6);
        //addCustomFieldsBlockToAddEditForm($form, $form_state, $entityId, $entityType, $uniqueId, 'catalog-survey-assesment-basic-addedit-form', 6);
        $form_state['cutom_fields_wrapper_id']= 'custom-fieldset-wrapper-'.$uniqueId;//this is needs for custom attribute validation
        //addEditFormWithCancelAndSaveClose($form, 'catalog-survey-assesment-basic-addedit-form', 'catalog-survey-assesment-basic-save', 'catalog-survey-assesment-basic-cancel', false, '', $surveyId,false);
        surveyAssessmentaddEditFormWithCancelAndSaveClose($form, 'catalog-survey-assesment-basic-addedit-form', 'catalog-survey-assesment-basic-save', 'catalog-survey-assesment-basic-cancel', false, '', $surveyId,true,$emptyId);
    }

  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_survey_assessment_addedit_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function question_type_callback($form, &$form_state) {
  //return ($form_state['values']["survey_type"] == 'sry_det_typ_ass') ? array($form['min_marks'],$form['max_marks']) : '';
  try{
	$minMaxMarksFormFields = array($form['min_marks'], $form['max_marks']);
	$commands = array();
	$commands[] = ajax_command_replace(NULL, drupal_render($minMaxMarksFormFields));
  $commands[] = exp_ctools_modal_adjust();

  return array('#type' => 'ajax', '#commands' => $commands);
  //return array($form['min_marks'],$form['max_marks']);
  } catch (Exception $ex) {
  	watchdog_exception('question_type_callback', $ex);
  	expertusErrorThrow($ex);
  }
}

function edit_survey_view(&$form, &$form_state, $defaultValue){
	try{
  global $theme_key;
   expDebug::dPrint('Survey $defaultValue'.print_r($defaultValue,true) , 4);
   expDebug::dPrint('Survey $defaultValue formstateee '.print_r($form_state["values"],true) , 5);
  $editAttributes['class'][]           = 'addedit-form-expertusone-throbber';
  $editAttributes['data-wrapperid']    = array('catalog-survey-assesment-basic-addedit-form');
  $editAttributes['class'][]           = 'addedit-form-class-button';
  $editAttributes['class'][]           = 'admin-add-edit-button';
  $editAttributes['onclick'][]         = '$(document).ready(function(){ editSurveyAssesment(); });';
  $defaultValue['random_name'] = ($defaultValue['random_name'] == 'Questions') ? 'LBL240' : $defaultValue['random_name'];

  $form['edit_btn'] = array(
  	  '#type' => 'submit',
	  '#value'      => ($theme_key == 'expertusoneV2')? '': t('LBL063'),
      '#name'       => 'EditAssessment',
      '#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container">',
      '#suffix'     => '</div>',
      '#attributes' => $editAttributes,
      '#ajax' => array(
  		'callback' => 'surveyAddEditRepaintForm',
  		'wrapper'  => 'catalog-survey-assesment-basic-addedit-form',
        'event'	   => 'click'
      ),
    );

   $form['sur_title'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL083').':',
    	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black genere-title vtip" title="'.sanitize_data(t($defaultValue['survey_title'])).'">'.titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-SURVEYTITLE',$defaultValue['survey_title'],55).'</div><div class="clearBoth"></div>',
    	'#suffix'   => '</div>',
     );

   /*$form['sur_code'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL281'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_code'].'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div>',
     );
     */
/*   $form['sur_description'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL331'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_description'].'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
     );*/

  $form['sur_code'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL096').':',
     	'#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view addedit-secondcol-field-value-black vtip" title="'.sanitize_data(t($defaultValue['survey_code'])).'">'.titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-SURVEYCODE',$defaultValue['survey_code'],17).'</div>',
     	'#suffix'   => '</div></div>',
     );


  $form['sur_noof_qst'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL332').': ',
     	'#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view addedit-secondcol-field-value-black">'.$defaultValue['survey_qust_per_page'].'</div>',
     	'#suffix'   => '</div>',
    );

  $form['sur_language'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL038').':',
     	'#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view addedit-secondcol-field-value-black">'.t($defaultValue['survey_language_name']).'</div>',
     	'#suffix'   => '</div></div>',
     );
  if($defaultValue['survey_type'] == 'sry_det_typ_ass') {

    $form['sur_min_mark'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL329').':',
     	'#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="first-col-value-view addedit-secondcol-field-value-black">'.$defaultValue['min_marks'].'</div>',
     	'#suffix'   => '</div></div>',
     );

  }

   if($defaultValue['survey_type'] == 'sry_det_typ_ass') {

   $form['assess_random'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL955').':',
     	'#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view addedit-secondcol-field-value-black">'.t($defaultValue['random_name']).'</div>',
     	'#suffix'   => '</div>',
   );

   $form['sur_max_mark'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL330').':',
     	'#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view addedit-secondcol-field-value-black">'.$defaultValue['max_marks'].'</div>',
     	'#suffix'   => '</div></div>',
     );

  }



   $form['sur_description'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL229').":",
     	'#prefix'   => '<div class="admin-course-class-long-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value addedit-secondcol-field-value-black">'.descController('ADMIN_CATALOG_DESC',$defaultValue['survey_description']).'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div></div>',
     );

/*   $form['sur_language'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL284'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_language_name'].'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div>',
     );

   $form['sur_status'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL285'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_status_name'].'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div>',
     );*/



   /*$form['sur_status'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL285'),
     	'#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view addedit-secondcol-field-value-black">'.$defaultValue['survey_status_name'].'</div>',
     	'#suffix'   => '</div>',
     );*/

  if(!empty($defaultValue['status_inactive_reason'])) {
     $form['sur_inactive_reason'] = array(
          '#type'     => 'item',
          '#title'    => t('LBL412'),
       	'#prefix'   => '<div class="admin-course-class-long-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
          '#markup'   => '</div><div class="addedit-new-field-value addedit-secondcol-field-value-black">'.$defaultValue['status_inactive_reason'].'</div><div class="clearBoth"></div>',
       	'#suffix'   => '</div></div>',
       );
  }


/*  $form['sur_noof_qst'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL332'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_qust_per_page'].'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div>',
     );

  $form['sur_type'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL333'),
     	'#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-secondcol-field-value-black">'.$defaultValue['survey_type_name'].'</div><div class="clearBoth"></div>',
     	'#suffix'   => '</div>',
     );*/



  /* $form['sur_type'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL333'),
     	'#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view addedit-secondcol-field-value-black">'.$defaultValue['survey_type_name'].'</div>',
     	'#suffix'   => '</div></div>',
     );*/



  $customDetails = getCustomFieldsDetails($defaultValue['survey_id'], $defaultValue['survey_type']);
     for ($i = 0; $i < count($customDetails); $i++) {
        $form['crs_custom_'.$i] = array(
        '#type'     => 'item',
        '#title'    =>'<div class="surv-ass-customLbl vtip" title="'.sanitize_data(t($customDetails[$i]->label_txt)).'">'.$customDetails[$i]->label_txt.':</div>',
        '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt vtip" title="'.t($customDetails[$i]->value_txt).'">'.titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-LABEL-VALUE',$customDetails[$i]->value_txt,20).'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
        );
     }
     } catch (Exception $ex) {
     	watchdog_exception('edit_survey_view', $ex);
     	expertusErrorThrow($ex);
     }
}

function exp_sp_administration_survey_assessment_addedit_form_validate($form, &$form_state){
	try {
  $form_state['hidden-save-submit'] = false;
  $form_state['esign_show_popup']= false;

 if (!empty($form_state['storage']['new_entity'])) {
    unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
    unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
  }

  if ($form_state['triggering_element']['#name'] == 'cancel'){
    $form_state['display_class']   = TRUE;
    $form_state['input']           = array();
    $form_state['values']          = array();
    unset($form_state['custom_fields']); // Clear any change made to custom fields
    $form_state['rebuild']         = TRUE;
    $form_state['storage']['step'] = 'view';
    return;
  }
  if ($form_state['triggering_element']['#name'] == 'EditAssessment') {
    $form_state['display_class']   = TRUE;
    $form_state['input']           = array();
    //$form_state['values']          = array();
    $form_state['rebuild']         = TRUE;
    $form_state['storage']['step'] = 'edit';
    return;
  }

  if(module_exists('exp_sp_esignature') && isActiveEsignRegion() ){
    $triggerValue  = 'save';
  }else{
    $triggerValue  = 'hidden-save';
  }

  $triggerElementByName  = $form_state['triggering_element']['#name'];
  expDebug::dPrint('$triggerElementByName : '.$triggerElementByName , 5);
  expDebug::dPrint('$form_state[triggering_element][#ajax][triggerEvent] : '. $form_state['triggering_element']['#ajax']['triggerEvent'] , 5);
  if ((($form_state['triggering_element']['#name'] == 'save') ) ||($form_state['triggering_element']['#name'] == 'saveandpublish')){
    $form_state['esign_show_popup']= true;
    expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
    addEditCheckMandatoryFields($form, $form_state, array('survey_title', 'survey_code', 'survey_language','survey_qust_per_page'));

    if (empty($form_state['values']['survey_description']['value'])){
        form_set_error('survey_description', $form['survey_description']['#title'] . t('ERR101'));
        $form_state['esign_show_popup']= false;
    }
    if(!empty($form_state['values']['survey_qust_per_page'])) {
      if (!is_numeric($form_state['values']['survey_qust_per_page'])) {
    	form_set_error('survey_qust_per_page', t('ERR097').' '.t('LBL328'));
    	$form_state['esign_show_popup']= false;
      }
    }
    if($form_state['values']['survey_type'] == 'sry_det_typ_ass'){
       addEditCheckMandatoryFields($form, $form_state, array('min_marks','max_marks'));
       //@TODO - to do below checks as appropriate for each field only when non-empty.

       if (!is_numeric($form_state['values']['min_marks'])) {
    		form_set_error('min_marks', t('ERR097').' '.t('LBL329'));
    		$form_state['esign_show_popup']= false;
    	 }
       if (!is_numeric($form_state['values']['max_marks'])) {
    		form_set_error('max_marks', t('ERR097').' '.t('LBL330'));
    		$form_state['esign_show_popup']= false;
    	 }

    	 if (is_numeric($form_state['values']['max_marks'])) {
    	 	 $maxMarksHighlighed = false;
	       if (is_numeric($form_state['values']['min_marks']) && ($form_state['values']['min_marks'] > $form_state['values']['max_marks'])) {
	    		 form_set_error('max_marks', t('LBL330').' '.t('ERR087').' '.t('LBL329').'.');
	    		 form_set_error('min_marks', ' '); //highlight min marks as well
	    		 $maxMarksHighlighed = true;
	    		 $form_state['esign_show_popup'] = false;
	    	 }

    	   // When assessment has questions, max marks entered by admin must equal the questions' score total
         $isValid = validateMaxScore($form_state['values']['survey_id'], $form_state['values']['max_marks']);
         if(!$isValid) {
           if ($maxMarksHighlighed) {
             form_set_error('', t('ERR184'));
           }
           else {
             form_set_error('max_marks', t('ERR184'));
           }
           $form_state['esign_show_popup'] = false;
         }
    	 }
    }
    //validateTitleCode($validate_text,$validate_value,$entity_type);
    if(!empty($form_state['values']['survey_type'])) {

       // $title_validate  = validateTitleCode('title',$form_state['values']['survey_title'],$form_state['values']['survey_type']);
        $code_validate   = validateTitleCode('code',$form_state['values']['survey_code'],$form_state['values']['survey_type'],$form_state['values']['survey_id']);

        /*if(($title_validate>0) && !empty($form_state['values']['survey_title'])) {
          form_set_error('survey_title', t('Title is already exist.'));
          $form_state["storage"]["form_validate"] = false;
          $form_state['esign_show_popup']= false;
        }*/
       /* if (!is_numeric($form_state['values']['survey_qust_per_page'])) {
        	form_set_error('survey_qust_per_page', t('ERR097').' '.t('LBL328'));
        	$form_state['esign_show_popup']= false;
        }*/

       if(($code_validate>0) && !empty($form_state['values']['survey_code'])) {
          form_set_error('survey_code', t('ERR131'));
          $form_state["storage"]["form_validate"] = false;
          $form_state['esign_show_popup']= false;
        }
   // }

    // Validate custom fields
    if(!module_exists('exp_sp_administration_customattribute')){  //#custom_attribute_0078975 - Check module status  
    validateCustomFieldsInAddEditForm($form, $form_state);
    }  //#custom_attribute_0078975 - End Check module status 

  }
 }
  /*if ($triggerElementByName == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent']=='hidden-save') {
    expDebug::dPrint('ilayaraja hidden-save called');
    $form_state['hidden-save-submit']=true;
    $form_state['esign_show_popup']= false;
  }
  */

 if ($triggerElementByName == 'hidden-save' || $triggerElementByName == 'set-prerequisite' || $triggerElementByName == 'set-equivalence') {
    $form_state['hidden-save-submit']=true;
    $form_state['esign_show_popup']= false;
  }

  if($form_state['triggering_element']['#name'] == 'delete' ||  $form_state['triggering_element']['#name'] == 'unpublish'){
    $form_state['esign_show_popup']= true;
  }

  $form_state['esign_validation_status']= false;
  if(!($form_state['hidden-save-submit'])){
    unset($_SESSION['esign_validate']);
  }
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_survey_assessment_addedit_form_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function validateMaxScore($entityId, $newMaxScore) {
	try {
	  $select = db_select('slt_survey_groups_questions', 'sur_grp_ques');
	  // $select->leftJoin('slt_survey_groups', 'sur_grp', 'sur_grp_ques.survey_group_id = sur_grp.id'); // Issue fix : #35887
	  $select->leftJoin('slt_survey', 'sur', 'sur_grp_ques.survey_id = sur.id');
	  $select->addExpression('SUM(sur_grp_ques.score)', 'total_score');
	  $select->addExpression('COUNT(sur_grp_ques.id)', 'ques_cnt');
	  $select->condition('sur_grp_ques.survey_id', $entityId, '=');
	  $select->condition('sur.type', 'sry_det_typ_ass', '=');
	  //$select->condition('sur.status', 'sry_det_sry_atv', '='); // Issue fix : #35887
	  expDebug::dPrintDBAPI('Validate max score select = ', $select);
	  $questionsTotalScore = $select->execute()->fetch();
	  expDebug::dPrint('$questionsTotalScore = ' . print_r($questionsTotalScore, true));

	  $isValid = true;
	  if ($questionsTotalScore->ques_cnt > 0) {
	  	expDebug::dPrint("Comparing questions total score $questionsTotalScore->total_score with new max score $newMaxScore", 4);
	  	if ($questionsTotalScore->total_score != $newMaxScore) {
	  		$isValid = false;
	    }
	  }

	  expDebug::dPrint('$isValid = ' . ($isValid? 'TRUE' : 'FALSE'), 4);
	  return $isValid;
	}
  catch (Exception $ex) {
    watchdog_exception('validateMaxScore', $ex);
    expertusErrorThrow($ex);
  }
}

function validateTitleCode($validate_text,$validate_value,$entity_type,$survey_id) {
  try{
  $select = db_select("slt_survey","survey");
  $select->addField('survey','id','survey_id');
  $select->addField('survey','title','survey_title');
  $select->addField('survey','code','survey_code');

  if(!empty($validate_text) && !empty($validate_value) && !empty($entity_type)) {
    $select->condition('survey.'.$validate_text,$validate_value,'=');
    $select->condition('survey.type',$entity_type,'=');
  }
  if(!empty($survey_id)) {
    $select->condition('survey.id',$survey_id,'!=');
  }

  $result = $select->execute();

  return $result->rowCount();
  } catch (Exception $ex) {
  	watchdog_exception('validateTitleCode', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_survey_assessment_addedit_form_submit($form, &$form_state){
	try {
  global $user;
  $userId = $user->uid;
  /*$formIdList = array(
                        array('id' => 'exp_sp_administration_survey_assessment_addedit_form', 'title' => t('Survey/Assessment')),
                        array('id' => 'exp_sp_administration_survey_assesment_question_form', 'title' => t('Question'))
                        );*/

    //if ($form_state['triggering_element']['#value'] == 'Save'){
     expDebug::dPrint('Tersting Submit save : ');

     expDebug::dPrint('Tersting Submit valuee : '.print_r($form_state["values"],true) , 5);

		$chkValidation = 1;
    expDebug::dPrint('exp_sp_administration_catalog_course_addedit_form_submit esign validate trigger ' .$form_state['triggering_element']['#name'] , 5);

   if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
     $chkValidation = 0;
     if($form_state['hidden-save-submit']==true && $_SESSION['esign_validate'] ==  1){
       $chkValidation = 1;
       $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
       expDebug::dPrint('Tersting Submit save 1 : '.print_r($form_state,true) , 5);
     }
     else{
       $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
       expDebug::dPrint('Tersting Submit save 2 : '.print_r($form_state,true) , 5);
     }
   }
   else{
     $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
     expDebug::dPrint('Tersting Submit save 3 : '.print_r($form_state,true) , 5);
   }
   expDebug::dPrint('Tersting validation erroe save : '.$chkValidation , 4);
      if($chkValidation){
        expDebug::dPrint('Correct check : '.$chkValidation , 4);
   $form_state['esign_validation_status'] = true;
   // if($form_state['hidden-save-submit']==true){
        $loggedInUserId = getIdOfLoggedInUser();
 expDebug::dPrint('Tersting Submit save 4 : '.print_r($form_state["values"],true) , 5);
        if (empty($form_state['values']['id']) && $form_state['values']['id']=='') {

          expDebug::dPrint('Tersting Submit save 4 : '.print_r($form_state["values"],true) , 5);

          $surveyId                               = addNewSurveyAssessment($form, $form_state, $loggedInUserId);
          if(!module_exists('exp_sp_administration_customattribute')){  //#custom_attribute_0078975 - Check module status
          saveCustomFieldsInAddEditForm($form, $form_state, $surveyId, $form_state['values']['survey_type'], $userId);
          }  //#custom_attribute_0078975 - End Check module status
          
          $emptyId = $form_state['values']['empty_id'];
          _updateAccessEntity($surveyId, $form_state['values']['survey_type'], $emptyId);
          if($form_state['values']['survey_type']=='sry_det_typ_ass'){
            expertus_set_message(t('LBL272').' '.t('MSG600').'.');
          }else{
          	expertus_set_message(t('LBL272').' '.t('MSG600').'.');
          }

          expDebug::dPrint('after inserttttt surv iddd :'.$surveyId , 4);

          $form_state['values']['id']             = $surveyId;
          $form_state['values']['survey_id']      = $surveyId;
          $form_state['survey_id']      = $surveyId;
          $form_state['storage']['step']          = 'view';
          $form_state['build_info']['args'][2]     = $surveyId;
          $form_state['build_info']['args']['entity_id']     = $surveyId;
          $form_state['storage']['new_entity']    = TRUE;
          $form_state['rebuild']                  = TRUE;

          expDebug::dPrint('after inserttttt surv iddd :'.print_r($form_state['values'],true) , 5);

        } else {
          $classId                                = $form_state['values']['id'];
          unset($form_state['storage']['new_entity']);
          unset($form_state['storage']['form_info_list']);

          updateSurveyAssessment($form, $form_state, $loggedInUserId);
          if(!module_exists('exp_sp_administration_customattribute')){  //#custom_attribute_0078975 - Check module status
          saveCustomFieldsInAddEditForm($form, $form_state, $form_state['values']['survey_id'], $form_state['values']['survey_type'], $userId);
          }  //#custom_attribute_0078975 - End Check module status
          
          //customfieldsOperationsForDropdown($form, $form_state,$form_state['values']['survey_id'],$form_state['values']['survey_type']);
        if($form_state['values']['survey_type']=='sry_det_typ_ass'){
            expertus_set_message(t('LBL272').' '.t('MSG601').'.');
          }else{
          	expertus_set_message(t('LBL272').' '.t('MSG601').'.');
          }
         // drupal_set_message(t('MSG298'));
          $form_state['storage']['step']          = 'view';
          $form_state['storage']['edited_entity'] = TRUE;
          $form_state['storage']['new_entity']    = FALSE;
          $form_state['rebuild']                  = TRUE;

        }
        $form_state['display_class'] = TRUE;
    }
    /*$form_state['build_info']['args'][]       = $surveyId;
    $form_state['storage']['form_info_list']  = $formIdList;
    $form_state['display_class']              = TRUE;*/
    } catch (Exception $ex) {
    	watchdog_exception('exp_sp_administration_survey_assessment_addedit_form_submit', $ex);
    	expertusErrorThrow($ex);
    }
}

function addNewSurveyAssessment($form, &$form_state, $loggedInUserId) {

  expDebug::dPrint('Tersting Submit save : '.print_r($form_state,true) , 5);
  $txn = db_transaction();

  try {
    $insertStmt  	                    = db_insert('slt_survey');
    $fields       	                    = surveyAssessmentFormatInputString($form, $form_state, $loggedInUserId,'insert',$callFromAPI);
    $insertStmt->fields($fields);
    $surveyId		                    = $insertStmt->execute();
    $form_state['values']['id']         = $surveyId;
    $form_state['values']['survey_id']  = $surveyId;
    expDebug::dPrint(' addNewAssesformstatevalues' . print_r($form_state['values'],true) , 5);
    $insertfields = $form_state['values'];
    $insertfields['description'] = $fields['description'];
    $insertfields['status'] = $fields['status'];
    expDebug::dPrintDBAPI(' $insertStmt SQL for survey = ' , $insertStmt);
    if($callFromAPI) {
    	$classCreateUser = $loggedInUserId;
    }
    else {
    	$classCreateUser = getIdOfLoggedInUser();
    }
    updateGroupMappingDetails($surveyId, $form_state['values']['survey_type'], $classCreateUser,'emptyadd',0);
    make_audit_trail_entries($insertfields, getIdOfLoggedInUser() , null, $surveyId ,'Inserted',$insertfields['survey_type'],'exp_sp_administration_survey','addNewSurveyAssessment',
                                          array('survey_id', 'survey_title', 'survey_code', 'description','survey_language', 'survey_type','survey_qust_per_page','status','min_marks','max_marks'));

    return $surveyId;
  } catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewSurveyAssessment', $ex);
    throw $ex;
  }

  unset($txn);
}


function updateSurveyAssessment($form, &$form_state, $loggedInUserId, $callFromAPI =0) {

  $txn = db_transaction();

  try {
    $updateStmt       = db_update('slt_survey');
    $default= db_query("SELECT id,title as survey_title,code as survey_code,description,lang_code as survey_language,status,type as survey_type,question_per_page as survey_qust_per_page,min_mark as min_marks,max_mark as max_marks from slt_survey where id=:uid",array(":uid"=>$form_state['values']['survey_id']))->fetchAll();
    $fields           = surveyAssessmentFormatInputString($form, $form_state, $loggedInUserId, 'update');
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['survey_id']);
    $numUpdated       = $updateStmt->execute();
    $updatefields = $form_state['values'];
    $updatefields['description'] = $fields['description'];
    $updatefields['status'] = $fields['status'];
    make_audit_trail_entries($updatefields, getIdOfLoggedInUser() , $default, $form_state['values']['survey_id'] ,'Updated',$updatefields['survey_type'],'exp_sp_administration_survey','addNewSurveyAssessment',
                                          array('survey_title', 'survey_code', 'description','survey_language', 'status', 'survey_type', 'survey_qust_per_page','min_marks','max_marks'));

      if($callFromAPI){
      return array((object)array('Id'=>$form_state['values']['survey_id']));
    }
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateSurveyAssessment', $ex);
    if($callFromAPI){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Error in Update SurveyAssessment";
    	return $errobj;
     // return array((object)array('Id'=>'Failiure'));
    }
    throw $ex;
  }

  unset($txn);
}

function surveyAssessmentFormatInputString($form, &$form_state, $userid, $db_action,$callFromApi = 0) {
   try{
   $custom          = null;


    $inactivereasontext= empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
    $in_active_on      = $form_state['values']['survey_status'] == 'sry_det_sry_itv' ? now() : NULL;
    $in_active_reason  = $form_state['values']['survey_status'] == 'sry_det_sry_itv' ? $inactivereasontext : NULL;
   $trigger_element = $form_state['storage']['submit_triggering_element'];
  if($trigger_element == 'saveandpublish'){
     $survey_status = 'sry_det_sry_atv';
   }
   else if($trigger_element == 'save'){
     $survey_status = 'sry_det_sry_itv';
   }
   else if($trigger_element == 'unpublish'){
     $survey_status = 'sry_det_sry_itv';
   }
   else if($form_state['values']['survey_status']){
	$survey_status =$form_state['values']['survey_status'];
   }
   $code =$form_state['values']['survey_code'];
   if( ($callFromApi) && $survey_status =='sry_det_sry_del' ){
   	$code= ($form_state['values']['survey_code'].'_deleted_'.$form_state['values']['survey_id']) ;
   }

   $fields = array(
                'title'              => $form_state['values']['survey_title'],
                'code'               => $code,
                'lang_code'     	   => $form_state['values']['survey_language'],
                'locale_code'        => '',
                'profile_code'       => '',
                'description'        => $form_state['values']['survey_description']['value'],
                'short_description'  => $form_state['values']['survey_description']['value'],
                'question_per_page'  => $form_state['values']['survey_qust_per_page'],
                'status'             => $survey_status, //$form_state['values']['survey_status']
   							'type'               => $form_state['values']['survey_type'],
                'max_mark'           => !empty($form_state['values']['max_marks']) ? $form_state['values']['max_marks'] : null,
   							'min_mark'           => !empty($form_state['values']['min_marks']) ? $form_state['values']['min_marks'] : null,
   							'in_active_on'       => $in_active_on,
    						'in_active_reason'   => $in_active_reason,
                'random'             => $form_state['values']['assess_random'],
                'custom0'            => $custom,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom,
              );
  if ($db_action == 'insert') {
   	$fields['created_by'] = $userid;
   	$fields['created_on'] = now();
   	$fields['updated_by'] = $userid;
   	$fields['updated_on'] = now();
   }
   else {
   	$fields['updated_by'] = $userid;
   	$fields['updated_on'] = now();
   }

    return $fields;
    } catch (Exception $ex) {
    	watchdog_exception('surveyAssessmentFormatInputString', $ex);
    	expertusErrorThrow($ex);
    }
}

function getDefaultSurveyAssesmentFormFieldValue($fields, $form_state, $crsDetails) {
	try{
  $defaultValue 	      = '';
  $crsDefaultValue 	      = array();
  for($i=0;$i<count($fields);$i++) {
  $field                   = $fields[$i];
  if ($field == 'survey_description') {
    	$crsDefaultValue[$field] = empty($form_state['values'][$field]['value']) || $form_state['values'][$field]['value']=='' ? (empty($crsDetails->$field) || $crsDetails->$field=='' ? '' : $crsDetails->$field) : $form_state['values'][$field]['value'];
    }else{
    $crsDefaultValue[$field] = empty($form_state['values'][$field]) ? (!empty($crsDetails->$field) ? $crsDetails->$field : '') : $form_state['values'][$field];
    }
  }

  return $crsDefaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultSurveyAssesmentFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

function _getSurveyAssessmentDetails($surveyId) {
  try{
  $select = db_select("slt_survey","survey");
  $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = survey.lang_code');
  $select->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = survey.status');
  $select->innerJoin('slt_profile_list_items', 'prof3', 'prof3.code = survey.type');
  $select->leftJoin('slt_profile_list_items', 'prof4', 'prof4.code = survey.random');
  $select->addField('survey','id','survey_id');
  $select->addField('survey','title','survey_title');
  $select->addField('survey','code','survey_code');
  $select->addField('survey','lang_code','survey_lang_code');
  $select->addField('survey','short_description','survey_description');
  $select->addField('survey','question_per_page','survey_qust_per_page');
  $select->addField('survey','type','survey_type');
  $select->addField('survey','status','survey_status');
  $select->addField('survey','random','random');
  $select->addField('prof1','name','survey_language_name');
  $select->addField('prof2','name','survey_status_name');
  $select->addField('prof3','name','survey_type_name');
  $select->addField('prof4','name','random_name');
  $select->addField('survey','min_mark','min_marks');
  $select->addField('survey','max_mark','max_marks');
  $select->addField('survey','in_active_reason','status_inactive_reason');
  $select->condition('survey.id',$surveyId,'=');

  expdebug::dPrintDBAPI('Survey List', $select);

  $surveyList = $select->execute()->fetchAll();

  return $surveyList;
  } catch (Exception $ex) {
  	watchdog_exception('_getSurveyAssessmentDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function surveyAddEditRepaintForm($form, &$form_state){
  // If a new entity was added, build additional forms
  try{
  expDebug::dPrint('args list in the repaint form valllluee: '.print_r($form_state["values"],true) , 5);
  if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {
    // Create the additional now editable forms if any present
    $formInfoList = $form_state['storage']['form_info_list'];
    $argsList = $form_state['build_info']['args'];
    expDebug::dPrint('args list in the repaint form: '.print_r($argsList,true) , 5);
    array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList
     expDebug::dPrint('args list in the repaint after: '.print_r($argsList,true) , 5);
     expDebug::dPrint('args list in the form details after: '.print_r($form_state,true) , 5);
     $entity_id = $form_state['build_info']['args']['entity_id'];
     $argsList['values']['survey_id'] = !empty($entity_id) ? $entity_id :  $form_state["values"]['survey_id'];
    $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
  }
  $commands = array();
  $commands[] = ajax_command_replace(NULL, drupal_render($form));
  $commands[] = ajax_command_prepend(NULL, theme('status_messages'));

  if (!empty($renderedAddlForms) &&  ($form_state['storage']['step']=='view')) {
    $commands[] = ajax_command_after(NULL, $renderedAddlForms);
  }
  $commands[] = exp_ctools_modal_adjust();
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
    $commands[] = ajax_command_refresh_narrow_search_results('onclose');
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('surveyAddEditRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function addNewSurveyByRestAPI( &$form_state,$createuserid="") {
	try{
		$form=array();
		$form["form_id"]="";
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state['values']['survey_type'] = 'sry_det_typ_sry';//sry_det_typ_ass

		//validateTitleCode($validate_text,$validate_value,$entity_type);
		if(!empty($form_state['values']['survey_type'])) {
			$code_validate   = validateTitleCode('code',$form_state['values']['survey_code'],$form_state['values']['survey_type'],$form_state['values']['survey_id']);
			if(($code_validate>0) && !empty($form_state['values']['survey_code'])) {
				$error -> errormsg = t('ERR131');
				$error -> isValidateError = true;
				return $error;
			}
		}
		$default_status = $form_state['values']['survey_status'];
		$form_state['values']['survey_status'] = 'sry_det_sry_itv';
		$surveyId =  addNewSurveyAssessment($form,$form_state,$createuserid);
		expDebug::dPrint(' addNewSurveyByRestAPI created Survey:' . print_r($surveyId,true) , 4);
		if($surveyId){
		  if($default_status == 'sry_det_sry_atv'){
		    return array((object)array('Id'=>$surveyId,'msg'=>'Survey is created with inactive status, since no questions found'));
		  }else{
		    return array((object)array('Id'=>$surveyId));
		  }
		}
		else{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Survey is not created ";
			return $errobj;
	 // return array((object)array('Id'=>'Failure'));
		}
	} catch (Exception $ex) {
		watchdog_exception('addNewSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateSurveyByRestAPI( &$form_state,$createuserid="")
{
	try{
	$form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form_state['values']['survey_type'] = 'sry_det_typ_sry';
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
	$survey = getSurveyInfo($form_state['values']['survey_id']);
	if(empty($survey)) {
		$error -> errormsg = t('Invalid survey Id');
		$error -> isValidateError = true;
		return $error;
	}
	if(!empty($form_state['values']['survey_type'])) {
		$code_validate   = validateTitleCode('code',$form_state['values']['survey_code'],$form_state['values']['survey_type'],$form_state['values']['survey_id']);
		if(($code_validate>0) && !empty($form_state['values']['survey_code'])) {
			$error -> errormsg = t('ERR131');
			$error -> isValidateError = true;
			return $error;
		}
	}
	$delete_per = 0;
	if($form_state['values']['survey_status'] != 'sry_det_sry_atv'){
		$delete_per = getSurveyAssessmentDeleteAccess($form_state['values']['survey_id']);
		if($delete_per){
			$error -> errormsg = t('Sorry unable to unpulish this survey, Survey attached some class');
			$error -> isValidateError = true;
			return $error;
		}
	}
	else{
		$question_attached = getIfSurveyAssessmentValid($form_state['values']['survey_id']);
		if($question_attached == 0){
			$error -> errormsg = t('Sorry unable to publish this survey, no questions attached');
			$error -> isValidateError = true;
			return $error;
		}
	}
	$surveyId =  updateSurveyAssessment($form,$form_state,$createuserid,1);
	expDebug::dPrint(' updateSurveyByRestAPI updated Survey:' . print_r($surveyId,true) , 4);
	return $surveyId;
	} catch (Exception $ex) {
		watchdog_exception('updateSurveyByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function addNewAssessmentByRestAPI( &$form_state,$createuserid="")
{
	try{
	$form=array();
	$form["form_id"]="";
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	$form_state['values']['survey_type'] = 'sry_det_typ_ass';
	//46332: PS-API: Issues in 'Create Assessment API'.
	$form_state['triggering_element']['#name'] = 'saveandpublish';
	exp_sp_administration_survey_assessment_addedit_form_validate($form,$form_state);
	$validate = form_get_errors();
	$error=new stdClass();
	if(count($validate)){
		$output = implode(', ', array_map(function ($v, $k) {	return $k . ' ' . $v;		}, $validate, array_keys($validate)));
		$error -> errormsg = $output;
		$error -> isValidateError = true;
		return $error;
	}
	$default_status = $form_state['values']['survey_status'];
	$form_state['values']['survey_status'] = 'sry_det_sry_itv';
	$assId =  addNewSurveyAssessment($form,$form_state,$createuserid);
	expDebug::dPrint(' addNewAssessmentByRestAPI created assessment:' . print_r($assId,true) , 4);
	if($assId){
	  if($default_status == 'sry_det_sry_atv'){
	    return array((object)array('Id'=>$assId,'msg'=>'Assesment is created with inactive status, since no questions found'));
	  }else{
	  return array((object)array('Id'=>$assId));
	}
	}
	else{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->errormsg = "Assessment is not Created";
		return $errobj;
	  //return array((object)array('Id'=>'Failure'));
	}
	} catch (Exception $ex) {
		watchdog_exception('addNewAssessmentByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateAssessmentByRestAPI( &$form_state,$createuserid="")
{
	try {
	$form=array();
	$form["form_id"]="";
	$form_state['values']['survey_type'] = 'sry_det_typ_ass';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	//46332: PS-API: Issues in 'Create Assessment API'.
	$form_state['triggering_element']['#name'] = 'saveandpublish';
	/* 0046817: PS - API : Issue faced in the Update Assessment API */
	include_once(drupal_get_path('module', 'exp_sp_administration_surveydetails') .'/exp_sp_administration_survey_assesment_question.inc');
	$survey = getSurveyInfo($form_state['values']['survey_id']);
	$error=(object)array();
	if(empty($survey)) {
		$error -> errormsg = t('Invalid Assessment Id');
		$error -> isValidateError = true;
		return $error;
	}
	exp_sp_administration_survey_assessment_addedit_form_validate($form,$form_state);
	$validate = form_get_errors();
	expDebug::dPrint(' updateAssessmentByRestAPI updated Assessment:$validate' . print_r($validate,true),5 );
	if(count($validate)){
		$output = implode(', ', array_map(function ($v, $k) {
			return $k . ' ' . $v;
		}, $validate, array_keys($validate)));
		$error -> errormsg = $output;
		$error -> isValidateError = true;
		return $error;
	}
	$assId =  updateSurveyAssessment($form,$form_state,$createuserid,1);
	expDebug::dPrint(' updateAssessmentByRestAPI updated Assessment:' . print_r($assId,true) );
	return $assId;
	} catch (Exception $ex) {
		watchdog_exception('updateAssessmentByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function surveyAssessmentaddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $expandPopup = FALSE, $saveTabIndex = '', $entityId = '',$esignature=false,$emptyId) {
	try {
	global $theme_key;
  $customHiddenButtonId = 'esign-button-'.$wrapperId;

   $customHiddenButtonId = 'last-button-account';
  //$customHiddenButtonId = 'last-button-account';

  $esignHiddenButt = 'esign-'.$wrapperId;
  expDebug::dPrint('exp_sp_administration.inc : addEditFormWithCancelAndSaveClose() called.');

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);

  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }

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
  // Also used by multiselect to show input as selected for only the current form
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
  $cancelAttributes['tabindex'][] = '10';

  if(empty($entityId)){
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
    $cancelLabel  = t('LBL123'); // Close
  }else{
    $cancelLabel  = t('LBL109'); // Cancel
  }
  $prfixPgLeft = '';
  $suffixPgRight = '';
  if($theme_key == 'expertusoneV2'){
  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  }else{
  	$setClass = (empty($entityId))?'resize-save_btn':'';
  }
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions '.$setClass.'">'. $prfixPgLeft,
  	'#suffix' => $suffixPgRight,
    '#value' => $cancelLabel,
    '#name' => 'cancel',
    '#attributes' => $cancelAttributes
  );

  if (!empty($entityId)) { // Cancel button is ajax enabled, as it takes to the readonly form view screen.
  	$form[$cancelButtonId]['#ajax'] = array (
      'wrapper' => $wrapperId,
      'callback' => $ajaxCallbackCancel,
      'method' => 'replace',
      'effect' => 'none', // 'fade',
      'event'=>'click',
      'keypress' => true,
      'progress' => array (
        'type' => 'throbber',
        'message' => ''
      ),
    );
  }

  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';

  // Using this class to applying bg color to the save button
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';

  //Increase the width of the popup while clicking on save button

  $saveAttributes['onclick'][] = 'increasePopWidth();';


  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  // Also used by multiselect to show input as selected for only the current form
  $saveAttributes['data-wrapperid'] = array($wrapperId);

 if (!empty($entityId)) {
		  $surveyDetails      = _getSurveyAssessmentDetails($entityId);
		  $fieldList          = array('survey_id','survey_title', 'survey_code', 'survey_lang_code','survey_description', 'survey_language', 'survey_status', 'survey_qust_per_page', 'survey_type', 'min_marks', 'max_marks','survey_language_name', 'survey_status_name', 'survey_type_name','status_inactive_reason','random','random_name');
		  $surveyDetails      = getDefaultSurveyAssesmentFormFieldValue($fieldList, $form_state, $surveyDetails[0]);
      $surStatus    =  $surveyDetails['survey_status'];
    }

   $resultCnt = 0;
   if(!empty($entityId)){
      $resultCnt = delete_object_record($entityId,'SurAss');
      $ifSurAssessValid = getIfSurveyAssessmentValid($entityId);
	  $maxAssessmentScore = maxScoreValidate($entityId); // Issue Fix: 54647
   }
   $deleteCheck = adminVisibilityForAddEditDelete($entityId,$surveyDetails['survey_type'],$surveyDetails['survey_type'],'slt_survey');
 //  if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
   	$displayPubActionList = '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>';
  // }else{
   //	$displayPubActionList = '<div class="admin-save-button-right-bg"></div>';
 //  }

   $isSaveParent = 0;
   if(empty($entityId) || $surStatus != 'sry_det_sry_atv'){
      $isSaveParent = 1;
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = 'increasePopWidth();';
      $saveAttributes['class'][] = ((($resultCnt==0) && empty($entityId)) ? '' : 'admin-save-button-dropdown-middle-bg-wrapper');
      $saveAttributes['tabindex'][] = '9';
      $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' => ((($resultCnt==0) && empty($entityId)) ? ('<div class="admin-save-button-right-bg"></div></div>') :  ($displayPubActionList.'</div>')),
	    '#value' => t('LBL141'),
      	'#name' => 'save',
	    '#attributes' => $saveAttributes,
        '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'customButtonId'=>$esignHiddenButt,
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
      );
      $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
      $savePublishSuffix = '</li>';
    }else{
      $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
      $savePublishSuffix = ((($resultCnt==0) && ($surStatus == 'sry_det_sry_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';
	  $savePublishAttributes['class'][] = ((($resultCnt==0) && ($surStatus == 'sry_det_sry_atv')) ? 'admin-save-button-dropdown-middle-bg-wrapper' : '');
    }

    if($ifSurAssessValid != 0 && $maxAssessmentScore == 0){ // Issue Fix: 54647
	  $saveButtonId = 'catalog-course-save-publish';
	  $savePublishAttributes['onclick'] = array();
	  $saveAttributes['tabindex'][] = '9';
      $savePublishAttributes['onclick'][] = 'increasePopWidth();';
      $buttonLabel = ($surStatus == 'sry_det_sry_atv') ? t('LBL141') : t('LBL614');
      $savePublishAttributes['class'][] = 'admin-save-button-middle-bg';

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
          'customButtonId'=>$esignHiddenButt,
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


   if(!$isSaveParent){
      $form['addedit-button-group-ul-start'] = array(
        '#type' => 'markup',
        '#markup' => '<ul class="catalog-pub-add-list">',
      );
    }

	if(($entityId) && $surStatus == 'sry_det_sry_atv' ){
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
              'customButtonId'=>$esignHiddenButt,
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


	expDebug::dPrint("detail ".print_r($surveyDetails,true),4);
	//38374: Drop down is not working properly
	if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
    if(($entityId)){
      if($resultCnt == 0){
        $displayTitle = titleController('EXP-SP-ADMIN-SURVEY-ASSESSMENT-STITLE', $surveyDetails['survey_title'],34);
  	   $saveButtonId = 'catalog-course-delete';

  	   $deleteAttributes['onclick'] = array();
        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.
                                               strtolower($surveyDetails['survey_type_name']).' '.escape_string($surveyDetails['survey_title']) . '","'.$entityId.
                                                  '","SurAss"); return false;';
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
      '#markup' => '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div></ul>',
    );
  unset($saveAttributes['tabindex']);
  $saveAttributes['onclick'] = array();
  $saveAttributes['onclick'][] = 'hideMessageInfo();';
  $form[$esignHiddenButt] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',
	      '#id' => $esignHiddenButt,
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
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



  if(!empty($emptyId)){
  	$entityId = 0;
  }else {
  	$emptyId = 0;
  }
  expDebug::dPrint("formInfoList build_info".print_r($form['survey_type']['#value'],true),4);


	$entityType  = $form['survey_type']['#value'];

  $qtipIdInit        	 = $entityId.'_'.$entityType;
  $qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'$entityType','url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."','wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool','linkid':'visible-surass-".$entityId."'}";
  $form['addedit-left-col-extend-func'] = array(
  		'#type' => 'markup',
  		'#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-surass-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
  		'</div>',
  );


  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('surveyAssessmentaddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}
?>