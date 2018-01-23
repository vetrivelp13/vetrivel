<?php
/*
 * exp_sp_administration_question_addedit_form() - survey/assessment question form builder function
 */
function exp_sp_administration_question_addedit_form($form, &$form_state, $renderMode, $questionId, $surveyType) {
  try {
	  expDebug::dPrint('$questionId = ' . $questionId, 4);
	  expDebug::dPrint('$surveyType = ' . $surveyType, 4);
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);
	  expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);
	  expDebug::dPrint('$form_state[build_info] = ' . print_r($form_state['build_info']['other_args'], true), 5);

	  // Determine survey type, whether an assessment question or a survey question
    $surveyType = 'sry_det_typ_ass';
    if (in_array('surveyquestions', $form_state['build_info']['other_args'])) {
      $surveyType = 'sry_det_typ_sry';
    }

    $questionDetails    = array();

    // Question type drop-down is readonly when question is published (i.e. active).
    // Note: Whenever the user changes the question type, all existing options should be deleted.
    if (!empty($questionId) && empty($form_state['values'])) {
      $questionDetails   = getSurveyQuestionDetails($questionId);
      expDebug::dPrint('$questionDetails = ' . print_r($questionDetails, true), 4);
      if($questionDetails->question_status == 'sry_qtn_sts_atv') { // Question is published, question type cannot be modified
        $form_state['storage']['readonly_question'] = true;
      }
      else {
      	$form_state['storage']['readonly_question'] = false;
      }
    }

    $defaultValue = getSurveyQuestionDefaultValues($surveyType, $form_state, $questionDetails);
    expDebug::dPrint('$defaultValue = ' . print_r($defaultValue, true), 4);

    $form = newAddEditFormWithCancelAndSave('survey-question-basic-addedit-form');
    // Hidden question id field
    addHiddenValueToAddEditForm($form, 'id', $questionId);
    // Add hidden survey type field
    addHiddenValueToAddEditForm($form, 'survey_type', $surveyType);
    $emptyId = '';
    if(empty($questionId)){
    	$emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
    	$entityType = ($surveyType == 'sry_det_typ_ass') ? 'sry_det_typ_ass_qus' : 'sry_det_typ_sry_qus';
    }
    addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
    addHiddenFieldToAddEditForm($form, 'entity_value', $entityType);
    // Question text field
    expDebug::dPrint('$addHiddenFieldToAddEditForm  empty_id = ' . $emptyId);
    addTextAreaFieldToAddEditForm($form, 'question_txt', t('LBL325'), $defaultValue['question_txt'], 1, true); // "Question"
    // Question type and language drop-downs in two column format
    addTwoColumnRowToAddEditForm($form, 'question_type_lang_code');
    // Question type drop-down (field id = 'question_type', title = "Question type", type: mandatory, ajax enabled)
    // On change, form is rebuilt and the answer choices block is repainted.
    addSurveyQuestionTypeDropDownToAddEditForm($surveyType, $form, $defaultValue['question_type'], 2,  $form_state['storage']['readonly_question']);
    // Language drop-down
    addDropDownToAddEditForm($form, 'lang_code', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue['lang_code'], 3, true);
    addAnswerChoicesBlock($form, $surveyType, $defaultValue);
    if($surveyType == 'sry_det_typ_ass' && $defaultValue['question_type'] == 'sry_qtn_typ_mch' ) {
    	// Add answer block for multi choice assessment question type
    	addMultiChoiceQuestionBlock($form, $surveyType, $defaultValue);
    }
    endAddEditTwoColumnRow($form);
    sureveyQuestionAddEditFormWithCancelAndSaveClose($surveyType, $form, $form_state, 'survey-question-basic-addedit-form',
                                                       'survey-question-basic-save', 'survey-question-basic-cancel', false, '', $questionId, true,$emptyId);

    return $form;
  }catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_question_addedit_form', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getSurveyQuestionDetails() - Fetches an existing question's details from DB
 */
function getSurveyQuestionDetails($questionId) {
  try {
	  // Select table
	  $select = db_select('slt_survey_questions', 'question');

	  // Add fields to fetch
	  $select->addField('question', 'id', 'question_id');
	  $select->addField('question', 'lang_code', 'lang_code');
	  $select->addField('question', 'status', 'question_status');
	  $select->addField('question', 'question_type', 'question_type');
	  $select->addField('question', 'question_txt', 'question_txt');
	  $select->addField('question', 'right_answer', 'right_answer');
	  $select->addField('question', 'answer_choice_txt', 'answer_choice_txt');
	  $select->addField('question', 'answer_type', 'answer_type');
	  $select->addField('question', 'answer_min_criteria', 'answer_min_criteria');
	  // Set condition
	  $select->condition('question.id', $questionId, '=');
	  expDebug::dPrintDBAPI('$select', $select);

	  // Execute query
	  $surveyList = $select->execute()->fetchObject();
	  expDebug::dPrint('$surveyList = ' . print_r($surveyList, true));

	  // Return record
	  return $surveyList;
  }
  catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getSurveyQuestionDefaultValues() - Prepares default values for form elements
 */
function getSurveyQuestionDefaultValues($surveyType, &$form_state, $questionDetails) {
  try{
    $questionDefaultValueList = array();
    if($surveyType == 'sry_det_typ_ass') {
    	$fieldsList = array('question_type', 'question_txt','answer_type','answer_min_criteria');
    } else {
    	$fieldsList = array('question_type', 'question_txt');
    }
    foreach ($fieldsList as $field) {
      $questionDefaultValueList[$field] = empty($form_state['values'][$field])?
                                                      (empty($questionDetails->$field) ? '' : $questionDetails->$field) :
                                                      $form_state['values'][$field];
    }

    $questionDefaultValueList['lang_code'] = empty($form_state['values']['lang_code'])?
                                                  (empty($questionDetails->lang_code) ? 'cre_sys_lng_eng' : $questionDetails->lang_code) :
                                                  $form_state['values']['lang_code'];

    // Prepare answer choices - if $form_state['values'] has answerchoices use them to prepare the default choices,
    // else check if $form_state['storage'] has anwerchoices use them to prepare the default choices
    // else prepare the default choices from DB.
    if (isset($form_state['values']['answerchoices'])) {
    	expDebug::dPrint('Building answerchoices from $form_state[values]', 4);
      $questionDefaultValueList['answerchoices'] = prepareDefaultAnswerChoicesFromFormState($surveyType, $form_state);
    }
    else if (isset($form_state['storage']['answerchoices'])) {
    	expDebug::dPrint('Building answerchoices from storage', 4);
      $questionDefaultValueList['answerchoices'] = $form_state['storage']['answerchoices'];
      unset($form_state['storage']['answerchoices']); // Remove $form_state['storage']['answerchoices'] once used
    }
    else if (!empty($questionDetails)) {
    	expDebug::dPrint('Building answerchoices from DB data', 4);
    	$questionDefaultValueList['answerchoices'] = prepareDefaultAnswerChoicesFromDBData($surveyType, $questionDetails);
    }

    expDebug::dPrint('$questionDefaultValueList = ' . print_r($questionDefaultValueList, true), 5);
    return $questionDefaultValueList;
  }
  catch (Exception $ex) {
    watchdog_exception('getSurveyQuestionDefaultValues', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * prepareDefaultAnswerChoicesFromDBData() - Prepares default answer choices from DB question record
 */
function prepareDefaultAnswerChoicesFromDBData($surveyType, $questionDetails) {
	try {
    $answerChoices = answerChoicesResetValues($surveyType, $questionDetails->question_type);

    switch ($questionDetails->question_type) {
      case 'sry_qtn_typ_yno':
      case 'sry_qtn_typ_trf':
      case 'sry_qtn_typ_cmt':
        // Choices list is prepared in answerChoicesResetValues().
        break;

      case 'sry_qtn_typ_dpn':
      case 'sry_qtn_typ_mch':
      case 'sry_qtn_typ_rtg':
        if (!empty($questionDetails->answer_choice_txt)) {
        	$answerChoices['options'] = explode("##", $questionDetails->answer_choice_txt);
        	// Choices index always start from 1 instead of 0
        	$answerChoices['options'] = array_combine(range(1, count($answerChoices['options'])), $answerChoices['options']);
        }
        break;

      default:
        throw new Exception('Unsupported question type');
        break; // Not reached
    } // end switch

    $answerChoices['count'] = count($answerChoices['options']);

      // Assessment has correct answers as well.
      // Note: $answerChoices['rightanswer'] is inited in answerChoicesResetValues(), which covers
      //       the case when $answerChoices['count'] == 0.
      if ($answerChoices['count'] > 0 && $surveyType == 'sry_det_typ_ass') {
        switch ($questionDetails->question_type) {
          case 'sry_qtn_typ_yno':
          case 'sry_qtn_typ_trf':
          case 'sry_qtn_typ_dpn':
            if (empty($questionDetails->right_answer)) {
              $rightAnswer = NULL;
            }
            else {
              $rightAnswer = array_search($questionDetails->right_answer, $answerChoices['options']);
              if ($rightAnswer === FALSE) {
                $rightAnswer = NULL;
              }
            }
            break;

          case 'sry_qtn_typ_mch':
            $rightAnswersList = empty($questionDetails->right_answer)? array() : explode("##", $questionDetails->right_answer);
            $rightAnswer = array();
            foreach ($rightAnswersList as $answer) {
              $rightAnswerKey = array_search($answer, $answerChoices['options']);
              if ($rightAnswerKey !== FALSE) {
                $rightAnswer[$rightAnswerKey] = $rightAnswerKey;
              }
            }
            break;
        } // end switch

        $answerChoices['rightanswer'] = $rightAnswer;
      }

      expDebug::dPrint('returning $answerChoices = ' . print_r($answerChoices, true), 4);
      return $answerChoices;
	}
  catch (Exception $ex) {
    watchdog_exception('prepareDefaultAnswerChoicesFromDBData', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * prepareDefaultAnswerChoicesFromFormState() -  - Prepares default answer choices from form_state[values]
 */
function prepareDefaultAnswerChoicesFromFormState($surveyType, $form_state) {
  try {
    $answerChoices = answerChoicesResetValues($surveyType, $form_state['values']['question_type']);

    switch ($form_state['values']['question_type']) {
      case 'sry_qtn_typ_yno':
      case 'sry_qtn_typ_trf':
      case 'sry_qtn_typ_cmt':

        // Choices list is prepared in answerChoicesResetValues().

        // For assessment, fetch the right answer from form state
        if ($surveyType == 'sry_det_typ_ass') {
          $answerChoices['rightanswer'] = $form_state['values']['answerchoices']['choices'];
        }
        break;

      case 'sry_qtn_typ_dpn':
        for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
          $answerChoices['options'][$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
        }
        $answerChoices['count'] = $form_state['values']['answerchoices']['count'];

        // For assessment, fetch the right answer from form state
        if ($surveyType == 'sry_det_typ_ass') {
          $answerChoices['rightanswer'] = $form_state['values']['answerchoices']['choices']['choice-radio'];
        }
      	break;

      case 'sry_qtn_typ_mch':
      	for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
          $answerChoices['options'][$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];

          // For assessment, build the right answer from form state
          if ($surveyType == 'sry_det_typ_ass' && !empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
            $answerChoices['rightanswer'][$i] = $i;
          }
        } // end for loop
        $answerChoices['count'] = $form_state['values']['answerchoices']['count'];
      	break;

      case 'sry_qtn_typ_rtg':
        for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
          $answerChoices['options'][$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
        }
        $answerChoices['count'] = $form_state['values']['answerchoices']['count'];
        break;

      default:
        throw new Exception('Unsupported question type');
        break; // Not reached
    } // end switch

    expDebug::dPrint('returning $answerChoices = ' . print_r($answerChoices, true), 4);
    return $answerChoices;
  }
  catch (Exception $ex) {
    watchdog_exception('prepareDefaultAnswerChoicesFromFormState', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * answerChoicesResetValues() - Returns initial data for answer choices default values
 */
function answerChoicesResetValues($surveyType, $questionType) {
  try {
		expDebug::dPrint('$surveyType = ' . $surveyType, 4);
		expDebug::dPrint('$questionType = ' . $questionType, 4);
	  $answerChoicesList = array();
	  switch ($questionType) {
	    case 'sry_qtn_typ_yno':
	      $answerChoicesList = array(1 => 'Yes', 2 => 'No'); // Choice indexes start from 1
	      break;

	    case 'sry_qtn_typ_trf':
	      $answerChoicesList = array(1 => 'True', 2 => 'False'); // Choice indexes start from 1
	      break;

	    case 'sry_qtn_typ_cmt':
	      $answerChoicesList = array();
	      break;

	    case 'sry_qtn_typ_dpn':
	    case 'sry_qtn_typ_mch':
	    case 'sry_qtn_typ_rtg':
	       $answerChoicesList  =  array();
	       break;
	  } // end switch

	  $answerChoices = array();
	  $answerChoices['options'] = $answerChoicesList;
	  $answerChoices['count'] = count($answerChoicesList);

	  // Init correct answers for assessment.
	  if ($surveyType == 'sry_det_typ_ass') {
	    $answerChoices['rightanswer'] = NULL;
	    if ($questionType == 'sry_qtn_typ_mch') {
	      $answerChoices['rightanswer'] = array();
	    }
	  }

	  expDebug::dPrint('$answerChoices = '. print_r($answerChoices, true), 4);
	  return $answerChoices;
  }
  catch (Exception $ex) {
    watchdog_exception('answerChoicesResetValues', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addSurveyQuestionTypeDropDownToAddEditForm - Use this function to add the ajax enabled question type drop-downn
 */
function addSurveyQuestionTypeDropDownToAddEditForm($surveyType, &$form, $defaultOption, $tabIndex, $readOnly) {
	global $theme_key;
  try {
	  $id = 'question_type';
	  $title = t('LBL242'); // "Question type"
	  $mandatory = true;

	  // Prepare options
	  $questionTypeOptions = getProfileItemNamesKeyed('sry_qtn_typ_');
	  // Image type question is cuurently unsupported, remove if present
	  unset($questionTypeOptions['sry_qtn_typ_img']);
	  // Remove question types not supported for assessment.
	  if($surveyType == 'sry_det_typ_ass'){
	    unset($questionTypeOptions['sry_qtn_typ_cmt']); // Comments question type
	    unset($questionTypeOptions['sry_qtn_typ_rtg']); // Rating question type
	  }
	  $nullOption[] = t('LBL674');
	  $finalOptions = $nullOption + $questionTypeOptions;

	  // Prepare element attributes
	  $attributes = array();
		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

	  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	  // each time the form is re-built.
	  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

	  // Add class to apply common style to all drop-downs
	  $attributes['class'][] = 'addedit-edit-dropdown';

	  // Attributes for showing loader on ajax submit
	  $attributes['class'][] = 'addedit-form-expertusone-throbber';
	  $attributes['data-wrapperid'] = array('survey-question-basic-addedit-form');


	  if ($readOnly) {
	    $attributes['class'][] = 'addedit-readonly-dropdown';
	  }

	  if (isset($tabIndex) && $tabIndex > 0) {
	    $attributes['tabindex'] = $tabIndex;
	  }

	  // Prepare prefix and suffix of element
	  $prefix = '';
	  $suffix = '';
	  if (empty($form['addedit_tmp_storage']['two_column'])) {
	    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
	    $suffix = addEditGetSuffix('FIELD');
	  }
	  else {
	    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
	        $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
	        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
	        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
	    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
	        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
	        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
	        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
	    } else {
	      throw new Exception('Attempt to add a third field to a two column row.');
	    }
	  }
	  if($theme_key == 'expertusoneV2') {
		  $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon ">';
		  $newuiSuffix = '</div></div>';
	  } else {
		  $newuiPrefix = '';
		  $newuiSuffix = '';
	  }
	  // Add element to $form
	  $form[$id] = array(
	    '#type'          => 'select',
	    '#title'         => $title,
	    '#title_display' => 'invisible',
	    '#prefix'        => $prefix . $newuiPrefix,
	    '#suffix'        => $newuiSuffix . $suffix,
	    '#options'       => $finalOptions,
	    '#default_value' => $defaultOption,
	    '#disabled'      => $readOnly,
	    '#attributes'    => $attributes,
	    '#validate'      => array('surveyAdminQuestionResetOptions'),
	    '#ajax'          => array(
	      'wrapper'  => 'survey-question-basic-addedit-form',
	      'callback' => 'repaintSurveyQuestionForm',
	      'method'   => 'replace',
	      'effect'   => 'none',
	      'event'    => 'change',
	      'keypress' => true,
	      'progress' => array(
	        'type'     => 'throbber',
	        'message'  => ''
	      ),
	    )
	  );
  }
  catch (Exception $ex) {
    watchdog_exception('addSurveyQuestionTypeDropDownToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * surveyAdminQuestionResetOptions() - Whenever question type is changed, this function clears current answer choices and instructs
 *                                     Drupal to rebuild the form.
 */
function surveyAdminQuestionResetOptions($form, &$form_state) {
try {
		expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
		$form_state['storage']['answerchoices'] = answerChoicesResetValues($form_state['values']['survey_type'], $form_state['values']['question_type']);

		// Ensure the values on form re-build are picked from $form_state['storage']['answerchoices']
		unset($form_state['values']['answerchoices']);
		unset($form_state['input']['answerchoices']);
		expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);

		$form_state['rebuild'] = TRUE;
  }
  catch (Exception $ex) {
    watchdog_exception('surveyAdminQuestionResetOptions', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * addAnswerChoicesBlock() - Adds answer choices block to $form
 */
function addAnswerChoicesBlock(&$form, $surveyType, $defaultValue) {
	try {
    // Add the answerchoices block based on question type
    $form['answerchoices']['prefix'] = array (
      '#type'   => 'markup',
      '#markup' => '<div id="answer-choices-block" class="answer-choices-block">'
    );

    switch ($defaultValue['question_type']) {
      case 'sry_qtn_typ_yno':
      case 'sry_qtn_typ_trf':
        if ($surveyType == 'sry_det_typ_ass') { // We have to present sub-form for admin to mark the correct answer to the question
            $form['answerchoices']['choices'] = array (
              '#type' => 'radios',
              '#title_display' => 'invisible',
              '#default_value' =>  $defaultValue['answerchoices']['rightanswer'],
              '#options' => $defaultValue['answerchoices']['options'],
              '#theme' => 'asmt_yntf_radios'
            );
        }
        break;

      case 'sry_qtn_typ_cmt':
        break;

      case 'sry_qtn_typ_dpn':
        if ($surveyType == 'sry_det_typ_ass') { // We have to present sub-form for admin to mark the correct answer to the question
          $options = $defaultValue['answerchoices']['options'];
          $rightAnswer = $defaultValue['answerchoices']['rightanswer'];
          if (empty($options)) {
            $options = array(1 => '');
            $rightAnswer = NULL;
          }

          $form['answerchoices']['choices'] = array (
            '#type' => 'asmt_dpn_radios',
            '#title_display' => 'invisible',
            '#default_value' =>  $rightAnswer,
            '#asmt_dpn_options' => $options, // Cannot use #options as it triggers validation in
                                             // form.inc:_form_validate() assuming a specific implementation.
            '#attributes' => array('tabindex' => 4),
          );
        }
        else {
          $options = $defaultValue['answerchoices']['options'];
          if (empty($options)) {
            $options = array(1 => '');
          }
          $form['answerchoices']['choices'] = array (
            '#type' => 'survey_question_fields',
            '#survey_question_options' => $options, // Cannot use #options as it triggers validation in
                                                    // form.inc:_form_validate() assuming a specific implementation.
            '#attributes' => array('tabindex' => 4),
          );
        }

        // Save count of options in a value field
        $form['answerchoices']['count'] = array (
          '#type' => 'value',
          '#value' => count($options)
        );

        break;

      case 'sry_qtn_typ_mch':
        if ($surveyType == 'sry_det_typ_ass') { // We have to present sub-form for admin to mark the correct answer to the question
        	$options = $defaultValue['answerchoices']['options'];
        	$rightAnswers = $defaultValue['answerchoices']['rightanswer'];
          if (empty($options)) {
            $options = array(1 => '');
            $rightAnswers = array();
          }        
          $form['answerchoices']['choices'] = array (
            '#type' => 'asmt_mch_checkboxes',
            '#title_display' => 'invisible',
            '#default_value' =>  $rightAnswers,
            '#asmt_mch_options' => $options, // Cannot use #options as it triggers validation in
                                               // form.inc:_form_validate() assuming a specific implementation.
            '#attributes' => array('tabindex' => 4),
          );
        }
        else {
          $options = $defaultValue['answerchoices']['options'];
          if (empty($options)) {
            $options = array(1 => '');
          }
          $form['answerchoices']['choices'] = array (
            '#type' => 'survey_question_fields',
            '#survey_question_options' => $options, // Cannot use #options as it triggers validation in
                                                    // form.inc:_form_validate() assuming a specific implementation.
            '#attributes' => array('tabindex' => 4),
          );
        }

        // Save count of options in a value field
        $form['answerchoices']['count'] = array (
          '#type' => 'value',
          '#value' => count($options)
        );
       break;

      case 'sry_qtn_typ_rtg':
        $options = $defaultValue['answerchoices']['options'];
        if (empty($options)) {
          $options = array(1 => '');
        }
        $form['answerchoices']['choices'] = array (
          '#type' => 'survey_question_fields',
          '#survey_question_options' => $options, // Cannot use #options as it triggers validation in
                                                  // form.inc:_form_validate() assuming a specific implementation.
          '#attributes' => array('tabindex' => 4),
        );

        // Save count of options in a value field
        $form['answerchoices']['count'] = array (
          '#type' => 'value',
          '#value' => count($options)
        );

      break;
    } // end switch
    $form['answerchoices']['suffix'] = array (
      '#type'   => 'markup',
      '#markup' =>   '<div style="clear:both"></div>' .
                   '</div>'
    );
  }
  catch (Exception $ex) {
    watchdog_exception('addAnswerChoicesBlock', $ex);
    expertusErrorThrow($ex);
  }
}
function addMultiChoiceQuestionBlock(&$form, $surveyType, $defaultValue) {
	try {
			// Get the answer types
			$answerTypeOptions = getProfileItemNamesKeyed('sry_ans_typ_');
			$path= $base_url.'/sites/all/themes/core/expertusoneV2/expertusone-internals/images';
			if(empty($defaultValue['answer_type'])) {
				$defValue = 'sry_ans_typ_all';
			} else {
				$defValue = $defaultValue['answer_type'];
			}
			$answer_type = t('LBL3059'); 
			$mandatory = false;
			global $theme_key;
			$prefix = '';
			$suffix = '';			
			$form['assessment-answer-block-container-start'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="assessment-answer-block-container">'
			);
			if (empty($form['addedit_tmp_storage']['two_column'])) {
				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $answer_type, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
			}
			$options                         = $answerTypeOptions;
			$attributes                      = array();
			$attributes['class']             = array('addedit-edit-answer-type');
			$finalOptions                    = $options;
			$form['answer_type'] = array(
					'#type' => 'radios',
					//'#title' => '',
					'#prefix' => '<div class="addedit-new-field-title">'.t('LBL3059').':&nbsp;<span>&nbsp;<img src="'.$path.'/help.png" class="vtip info-answer-logic" title="'.t('MSG808').'" onload="vtip();"></span></div>',
					'#options' => $answerTypeOptions,
					'#default_value' => $defValue,
					'#attributes'    => $attributes,
					'#ajax'          => array(
							'wrapper'  => 'survey-question-basic-addedit-form',
							'callback' => 'repaintSurveyQuestionForm',
							'progress' => array(
									'type'     => 'throbber',
							),
					)
			);
		$answer_min_criteria = array();
		$answer_min_criteria['class'][] = 'addedit-edit-answer-min-criteria';
		$answer_min_criteria['onkeydown'] = 'return onlyNumbers(event)';
		$form['min_criteria_data_disp_start'] = array(
				'#type' => 'markup',
				'#markup' => '<div class="addedit-onecol-firstcol"><div class="admin-answer-min-criteria">'
		);
		$form['answer_min_criteria'] = array(
			'#type' => 'textfield',
			'#disabled'=>($defaultValue['answer_type'] != 'sry_ans_typ_all' && $defaultValue['answer_type'] != '') ? '' : TRUE,
			'#size' => 4,
			'#default_value' => ($defaultValue['answer_type'] != 'sry_ans_typ_all') ? $defaultValue['answer_min_criteria'] :'',
			'#attributes' => $answer_min_criteria,				
		);
		$form['min_criteria_data_disp_end'] = array(
				'#type' => 'markup',
				'#markup' => '</div></div>'
		);
		$form['assessment-answer-block-container-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div>'
		); 
	}
	catch (Exception $ex) {
		watchdog_exception('addMultiChoiceQuestionBlock', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * repaintSurveyQuestionForm() - #ajax callback function to repaint form
 */
function repaintSurveyQuestionForm($form, $form_state) {
	try {
	  expDebug::dPrint('repainting form', 4);
	  $commands = array();
	  $commands[] = ajax_command_replace(NULL, drupal_render($form));
	  //0078379: Assessment - Questions - error message is displayed 2 times
	  $statusMsgHTML = theme('status_messages');
	  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
	  if (!empty($statusMsgHTML)) {
	  	// Show the messages
	  	//$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
	  	$commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
	  }
	  // Resize the modal backdrop, adjust the model top (and left) to optimal
	  $commands[] = exp_ctools_modal_adjust();

	  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 5);
	  return array('#type' => 'ajax', '#commands' => $commands);
  }
  catch (Exception $ex) {
    watchdog_exception('repaintSurveyQuestionForm', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * exp_sp_administration_question_addedit_form_validate() - The validate function called when the question form is saved with the
 *                                                          buttons Save, Save and Publish and Unpublish.
 */
function exp_sp_administration_question_addedit_form_validate($form, &$form_state) {
  try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 5);
	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);
	  expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);

	  $form_state['hidden-save-submit'] = false;
	  $form_state['esign_show_popup']= false;

	  if ((($form_state['triggering_element']['#name'] == 'save') ) ||($form_state['triggering_element']['#name'] == 'saveandpublish')) {
      $form_state['esign_show_popup']= true;

	    // Check the mandatory fields
	    addEditCheckMandatoryFields($form, $form_state, array('question_txt','question_type', 'lang_code'));
	    $surveyType = 'sry_det_typ_ass';
	    if(in_array('surveyquestions', $form_state['build_info']['other_args'])) {
	      $surveyType = 'sry_det_typ_sry';
	    }
	    if($form_state['values']['question_type'] == 'sry_qtn_typ_mch' && $surveyType == 'sry_det_typ_ass') {
	    	addEditCheckMandatoryFields($form, $form_state, array('answer_type'));
	    	$answer_min_criteria = trim($form_state['values']['answer_min_criteria']);
	    	$choiceOptionLabelsList = array();
	    	for ($j = 1; $j <= $form_state['values']['answerchoices']['count']; $j++) {
	    		$choiceOptionLabelsList[] = stripRegStatus(trim($form_state['values']['answerchoices']['choices']['choice-text-' . $j]),'Assesment');
	    	}
	    	$dupCount = call_user_func_array('checkOptionDuplicates',array($choiceOptionLabelsList));
	    	if($dupCount > 0) {
	    		form_set_error('', t('ERR258'));
	    		$form_state['esign_show_popup']= false;
	    	}
	    	if( $form_state['values']['answer_type'] == 'sry_ans_typ_atl') {
	    		if (empty($answer_min_criteria)) {
		    		form_set_error('answer_min_criteria', t('At least').''.t('ERR101'));
		    		$form_state['esign_show_popup']= false;
		    	}
		    	if(!is_numeric($answer_min_criteria)) {
		    		form_set_error('answer_min_criteria',  t('ERR097')." ".t('At least')." ".t('LBL750'));
		    		$form_state['esign_show_popup']= false;
		    	}
		    	$answerChoicesList = array();
		    	$rightAnswersList = array();
		    	for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
		    		$answerChoicesList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
		    		if (!empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
		    			$rightAnswersList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
		    		}
		    	}
		    	if(count($rightAnswersList) < $answer_min_criteria)
		    	{
		    		form_set_error('answer_min_criteria',  t('ERR257'));
		    		$form_state['esign_show_popup']= false;
		    	}
	    	}
	    }
	    $questionType = $form_state['values']['question_type'];
	    if ($surveyType == 'sry_det_typ_ass') {
		    // Check whether all configurable choices have text.
		    // Check whether correct answer is specified.
		    switch ($questionType) {
		      case 'sry_qtn_typ_yno':
		      case 'sry_qtn_typ_trf':
		        if (empty($form_state['values']['answerchoices']['choices'])) {
		          form_set_error('answerchoices][choices', t('MSG507') . t('ERR101'));
		          $form_state['esign_show_popup'] = false;
		        }
		        break;

		      case 'sry_qtn_typ_dpn':
	          $emptyChoiceCount = 0;
	          for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	            if (($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == null || ($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == '') {
	              form_set_error('answerchoices][choices][choice-text-' . $i, ' ');
	              $form_state['esign_show_popup'] = false;
	              $emptyChoiceCount++;
	            }
	          } // end for loop

	          if ($emptyChoiceCount == 1) {
	          	form_set_error(' ', t('LBL387') . t('ERR101'));
	          }
	          else if ($emptyChoiceCount > 1) {
	          	form_set_error(' ', t('LBL814') . t('ERR169'));
	          }

	          if (empty($form_state['values']['answerchoices']['choices']['choice-radio'])) {
	          	expDebug::dPrint('setting error for missing correct answer = ' .  t('MSG507') . t('ERR101'));
	            form_set_error('answerchoices][choices][choice-radio', t('MSG507') . t('ERR101'));
	            $form_state['esign_show_popup'] = false;
	          }
	          break;

		      case 'sry_qtn_typ_mch':
		    	  $emptyChoiceCount = 0;
		        $correctAnswerProvided = false;
		    	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
		    		  if (($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == null || ($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == '') {
		    			  form_set_error('answerchoices][choices][choice-text-' . $i, ' ');
		    			  $form_state['esign_show_popup'] = false;
		    			  $emptyChoiceCount++;
		    		  }

		    		  if (!empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
		    			  $correctAnswerProvided = true;
		    		  }
		    	  } // end for loop

		        if ($emptyChoiceCount == 1) {
              form_set_error(' ', t('LBL387') . t('ERR101'));
            }
            else if ($emptyChoiceCount > 1) {
              form_set_error(' ', t('LBL814') . t('ERR169'));
            }

		    	  if (!$correctAnswerProvided) {
		    		  form_set_error('answerchoices][count', t('ERR130'));
		    		  $form_state['esign_show_popup']= false;
		    	  }
		        break;
		    } // end switch
	    }
	    else { // $surveyType == 'sry_det_typ_sry'
		    switch ($questionType) {
		      case 'sry_qtn_typ_yno':
		      case 'sry_qtn_typ_trf':
		      case 'sry_qtn_typ_cmt':
		        // No validation case
		        break;
		      case 'sry_qtn_typ_dpn':
		      case 'sry_qtn_typ_mch':
		      case 'sry_qtn_typ_rtg':
            $emptyChoiceCount = 0;
            for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
              if (($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == null || ($form_state['values']['answerchoices']['choices']['choice-text-' . $i]) == '') {
                form_set_error('answerchoices][choices][choice-text-' . $i, ' ');
                $form_state['esign_show_popup'] = false;
                $emptyChoiceCount++;
              }
            } // end for loop
            if ($emptyChoiceCount == 1) {
              form_set_error(' ', t('LBL387') . t('ERR101'));
            }
            else if ($emptyChoiceCount > 1) {
              form_set_error(' ', t('LBL814') . t('ERR169'));
            }
		        break;
		    } // end switch
	    }
	  }

	  if ($form_state['triggering_element']['#name'] == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent']=='hidden-save') {
	    $form_state['hidden-save-submit'] = true;
	  }

    if($form_state['triggering_element']['#name'] == 'unpublish') {
	    $form_state['esign_show_popup'] = true;
	  }

	  $form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
	                                                  // showing popup. Seems an unncessary check, as we have $form_state['esign_show_popup'] set to
	                                                  // false when we do not want to show popup?
	  if (!($form_state['hidden-save-submit'])) {
	    unset($_SESSION['esign_validate']); // for every button other than hidden-save, unset $_SESSION['esign_validate']. Why?
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_question_addedit_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * exp_sp_administration_question_addedit_form_submit() - The submit handler for the question form.
 *                                                        Primarily to add/update question form fields to DB.
 */
function exp_sp_administration_question_addedit_form_submit($form, &$form_state) {
  try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['triggering_element'], true), 4);
	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);
	  expDebug::dPrint('$form_state[input] = ' . print_r($form_state['input'], true), 5);

    $eSignValidated = true;
    if (module_exists('exp_sp_esignature') && isActiveEsignRegion()) {
      if($form_state['hidden-save-submit'] == true && $_SESSION['esign_validate'] ==  1) { // Shouldn't hidden-save-submit be clicked only after
     	                                                                                     // eSign has been validated? What is the need to check
     	                                                                                     // $_SESSION['esign_validate'] ==  1?
        $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
      }
      else {
     	  $eSignValidated = false;
        $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
      }
    }
    else {
      $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
    }
    if ($eSignValidated) {
      $form_state['esign_validation_status'] = true;
      $loggedInUserId = getIdOfLoggedInUser();
      if(empty($form_state['values']['id'])) {
        $questionId = addNewSurveyQuestion($form, $form_state, $loggedInUserId);
        $emptyId = $form_state['values']['empty_id'];
        $entityType = ($form_state['values']['survey_type'] == 'sry_det_typ_sry') ? 'sry_det_typ_sry_qus' : 'sry_det_typ_ass_qus' ;
        //expDebug::dPrint('$eSignValidated = = = Values '.$questionId."----".$entityType."----".$emptyId);
        _updateTagsEntity($questionId, $entityType, $emptyId);
        _updateAccessEntity($questionId, $entityType, $emptyId);
      }
      else {
        updateSurveyQuestion($form, $form_state, $loggedInUserId);
      }

      $form_state['rebuild'] = TRUE;
    }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_question_addedit_form_submit', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * addNewSurveyQuestion() - Adds a new survey question record to the DB
 */
function addNewSurveyQuestion($form, &$form_state, $loggedInUserId, $callFromAPI = 0){
  $txn = db_transaction();
  try {
    $insertStmt = db_insert('slt_survey_questions');
  
    $insertFields = prepareDataForQuestionDBRecord($form, $form_state, 'insert', $loggedInUserId, $callFromAPI);
    $insertStmt->fields($insertFields);
    expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
    $questionId	= $insertStmt->execute();
    expDebug::dPrint('$surveyGroupId = ' . $questionId);
   if($callFromAPI) {
    	$entityType = ($form_state['values']['question_object_type'] == 'sry_det_typ_sry') ? 'sry_det_typ_sry_qus' : 'sry_det_typ_ass_qus' ;
    	updateGroupMappingDetails($questionId, $entityType, $loggedInUserId,'emptyadd',0);
    }
    else { 
    updateGroupMappingDetails($questionId, $form_state['values']['entity_value'], $loggedInUserId,'emptyadd',0);
    }
    $auditFieldsList = array('survey_type', 'question_txt', 'lang_code', 'question_type', 'status', 'answer_choice_txt', 'right_answer','answer_type','answer_min_criteria');
    make_audit_trail_entries($insertFields, $loggedInUserId, null, $questionId, 'Inserted',
                                      $insertFields['survey_type'], 'exp_sp_administration_surveyquestions', 'addNewSurveyQuestion', $auditFieldsList);

    if ($callFromAPI) {
      return array((object)array('Id' => $questionId));
    }
    else {
      if ($form_state['storage']['submit_triggering_element'] == 'unpublish') {
         expertus_set_message(t('MSG601').'.'); // "Question unpublished successfully."
      }
      else if ($form_state['storage']['submit_triggering_element'] == 'saveandpublish') {
        expertus_set_message(t('MSG601').'.'); // "Question published successfully."
      }
      else {
        expertus_set_message(t('MSG601').'.'); // "Question saved successfully."
      }

      $form_state['build_info']['args'][]  = $questionId;
      $form_state['storage']['new_entity'] = TRUE;
    }
    return $questionId;
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewSurveyQuestion', $ex);
    if($callFromAPI){
      return array((object)array('Id'=>'Failiure'));
    }
    expertusErrorThrow($ex);
  }
}

/*
 * updateSurveyQuestion() Updates an existing question in DB
 */
function updateSurveyQuestion($form, &$form_state, $loggedInUserId, $callFromAPI = 0){
	$txn = db_transaction();
	try {
		
		/* Update tag activity Count */
	$triggerElement = $form_state['storage']['submit_triggering_element'];
	if ($callFromAPI) {
		$activity_count_etype = ($form_state['values']['question_object_type']=='sry_det_typ_ass')?"sry_det_typ_ass_qus":"sry_det_typ_sry_qus";		
		$triggerElement=$form_state['values']['survey_status'];
	}else{
		$activity_count_etype = ($form_state['values']['survey_type']=='sry_det_typ_ass')?"sry_det_typ_ass_qus":"sry_det_typ_sry_qus";
	}
	updateTagActivityCount($form_state['values']['id'],$activity_count_etype,$triggerElement,$callFromAPI);		
	/* Update tag activity Count */
    $prevValuesQuery = db_select('slt_survey_questions', 'sques');
    $prevValueFieldsList = array('id', 'question_type', 'lang_code', 'status', 'in_active_on', 'in_active_reason',
                                      'question_txt', 'answer_choice_txt', 'right_answer');
    foreach ($prevValueFieldsList as $prevValueField) {
    	$prevValuesQuery->addField('sques', $prevValueField, $prevValueField);
    }
    $prevValuesQuery->condition('sques.id', $form_state['values']['id'], '=');
    $prevValuesQuery->forUpdate();
    expDebug::dPrintDBAPI('$prevValuesQuery', $prevValuesQuery);
    $prevValues = $prevValuesQuery->execute()->fetchAll();
    expDebug::dPrint('$prevValues = ' . print_r($prevValues, true));

    if(empty($prevValues) && $callFromAPI){
    	$error -> isValidateError = true;
    	$error -> errormsg = 'Invalid id';
    	return $error;
    }
    $updateStmt 	= db_update('slt_survey_questions');
    $updateFields   = prepareDataForQuestionDBRecord($form, $form_state, 'update', $loggedInUserId, $callFromAPI);
    $updateStmt->fields($updateFields);
    $updateStmt->condition('id', $form_state['values']['id'], '=');
    expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
    $numUpdated	= $updateStmt->execute();
    expDebug::dPrint('$numUpdated = ' . $numUpdated);

    $auditFieldsList = array('survey_type', 'question_txt', 'lang_code', 'question_type', 'status', 'answer_choice_txt', 'right_answer');
    make_audit_trail_entries($updateFields, getIdOfLoggedInUser(), $prevValues, $form_state['values']['id'], 'Updated', $updateFields['survey_type'],
                                         'exp_sp_administration_surveyquestions','updateSurveyQuestion', $auditFieldsList);

    if ($callFromAPI) {
      return array((object)array('Id'=>$form_state['values']['id']));
    }
    else {
      if($form_state['storage']['submit_triggering_element'] == 'unpublish'){
         expertus_set_message(t('MSG601').'.'); // "Question unpublished successfully."
      }
      else if($form_state['storage']['submit_triggering_element'] == 'saveandpublish'){
        expertus_set_message(t('MSG601').'.'); // "Question published successfully."
      }
      else {
        expertus_set_message(t('MSG601').'.');  // "Question updated successfully."
      }

      $form_state['storage']['edited_entity'] = TRUE;
    }
  }
  catch (Exception $ex) {
  	$txn->rollback();
    watchdog_exception('updateSurveyQuestion', $ex);
    if($callFromAPI){
      return array((object)array('Id'=>'Failiure'));
    }
    expertusErrorThrow($ex);
  }
}

/*
 * prepareDataForQuestionDBRecord() - Prepares data for the survey question record in DB
 */
function prepareDataForQuestionDBRecord($form, &$form_state, $dbAction, $loggedInUserId, $callFromAPI) {
  try {
    $fields = array(); //Array to hold question record fields

    // Prepare survey_type field data
    if ($callFromAPI) {
      $fields['survey_type'] = $form_state['values']['question_object_type'];
    }
    else {
      $fields['survey_type'] = 'sry_det_typ_ass';
      if (in_array('surveyquestions', $form_state['build_info']['other_args'])) {
        $fields['survey_type'] = 'sry_det_typ_sry';
      }
    }

    // Prepare question_txt field data
    $fields['question_txt'] = $form_state['values']['question_txt'];

    // Prepare lang_code field data
    $fields['lang_code'] = $form_state['values']['lang_code'];

    // Prepare question_type data
    $fields['question_type'] = $form_state['values']['question_type'];

    // Prepare status field data
    if ($callFromAPI) {
    	$fields['status'] = $form_state['values']['survey_status'];
    }
    else {
      $triggerElement = $form_state['storage']['submit_triggering_element'];
      expDebug::dPrint('$triggerElement = ' . print_r($triggerElement, true), 5);

      if ($triggerElement == 'saveandpublish') {
        $fields['status'] = 'sry_qtn_sts_atv';
        $form_state['storage']['readonly_question'] = true;
      }
      else if ($triggerElement == 'save' || $triggerElement == 'unpublish') {
        $fields['status'] = 'sry_qtn_sts_itv';
        $form_state['storage']['readonly_question'] = false;
      }
    }

    // Prepare answer_choice_txt and right_answer fields data
    if ($callFromAPI) {
      $fields['right_answer']       = str_replace('@@', '##', $form_state['values']['correct_answer']);
      $fields['answer_choice_txt']  = str_replace('@@', '##', $form_state['values']['answers_list']);
    }
    else {
      $fields['answer_choice_txt'] = ''; // Initialize here. Overwrite in the below switch statement as required.
      $fields['right_answer'] = ''; // Initialize here. Overwrite in the below switch statement as required.
      switch ($fields['question_type']) {
      	// Yes or no
        case 'sry_qtn_typ_yno':
          if ($fields['survey_type'] == 'sry_det_typ_ass') {
            $fields['right_answer'] = $form_state['values']['answerchoices']['choices'] == 1? 'Yes' : 'No'; //LBL342=Yes(1), LBL343=No(2)
            //@TODO - Correct answers to be saved as either 1 and 0 or in the selected language of the question. Currently, answers are saved
            //        in the language of the admin, which is incorrect.
          }
      	  break;
      	// True or false
        case 'sry_qtn_typ_trf':
      	  if ($fields['survey_type'] == 'sry_det_typ_ass') {
      		  $fields['right_answer'] = $form_state['values']['answerchoices']['choices'] == 1? 'True' : 'False'; //LBL384=True(1), LBL385=False(2)
            //@TODO - Correct answers to be saved as either 1 and 0 or in the selected language of the question. Currently, answers are saved
            //        in the language of the admin, which is incorrect.
      	  }
       	  break;
       	// comments
        case 'sry_qtn_typ_cmt':
          // Nothing to be done
          break;
        // dropdown
        case 'sry_qtn_typ_dpn':
      	  $answerChoicesList = array();
      	  $rightAnswer = '';
      	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
      	  	// stripApostrophe and stripRegStatus Is used To Eliminate the Special Characters 0038953
      		  $answerChoicesList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
      		  // Prepare right answer data only for assessment
      		  if ($fields['survey_type'] == 'sry_det_typ_ass' && $form_state['values']['answerchoices']['choices']['choice-radio'] == $i) {
      		  	// stripApostrophe and stripRegStatus Is used To Eliminate the Special Characters 0038953
      			  $rightAnswer = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
      		  }
      	  }
      	  $fields['answer_choice_txt'] = implode('##', $answerChoicesList);
      	  $fields['right_answer'] = $rightAnswer;
      	  break;
      	// multiple choice
        case 'sry_qtn_typ_mch':
          $answerChoicesList = array();
          $rightAnswersList = array();
          for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
          	// stripApostrophe and stripRegStatus Is used To Eliminate the Special Characters 0038953
            $answerChoicesList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');

            // Prepare right answer data only for assessment
            if ($fields['survey_type'] == 'sry_det_typ_ass' && !empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
            	// stripApostrophe and stripRegStatus Is used To Eliminate the Special Characters 0038953
              $rightAnswersList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
            }
          }
          $fields['answer_choice_txt'] = implode('##', $answerChoicesList);
          $fields['right_answer'] = implode('##', $rightAnswersList);
        
      	  break;
      	// rating
        case 'sry_qtn_typ_rtg':
          $answerChoicesList = array();
          for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
          	// stripApostrophe and stripRegStatus Is used To Eliminate the Special Characters 0038953
            $answerChoicesList[] = stripRegStatus($form_state['values']['answerchoices']['choices']['choice-text-' . $i],'Assesment');
          }
          $fields['answer_choice_txt'] = implode('##', $answerChoicesList);
          break;
      } // end switch
    }
    if ($callFromAPI) {
    	if($form_state['values']['question_type'] == 'sry_qtn_typ_mch' && $fields['survey_type'] == 'sry_det_typ_ass') {
	    	$fields['answer_type'] = $form_state['values']['answer_logic'];
	    	if($form_state['values']['answer_logic'] == 'sry_ans_typ_atl') {
		    	$fields['answer_min_criteria'] = $form_state['values']['atleast_number'];
	    	}
    	}
    } else {
    	if($form_state['values']['question_type'] == 'sry_qtn_typ_mch' && $fields['survey_type'] == 'sry_det_typ_ass') {
		    $fields['answer_type'] = $form_state['values']['answer_type'];
		    if($form_state['values']['answer_type'] == 'sry_ans_typ_atl') {
		    	$fields['answer_min_criteria'] = trim($form_state['values']['answer_min_criteria']);
		    }
    	}
    }
    if ($dbAction == 'insert') {
      $fields['created_by'] = $loggedInUserId;
      $fields['created_on'] = now();
      $fields['updated_by'] = $loggedInUserId;
      $fields['updated_on'] = now();
      $fields['entity_type'] = is_array($form_state['build_info']['other_args'])&& in_array('surveyquestions', $form_state['build_info']['other_args']) ? 'sry_det_typ_sry_qus' : 'sry_det_typ_ass_qus';
    }
    else {
    	$fields['updated_by'] = $loggedInUserId;
    	$fields['updated_on'] = now();
    }

    // Though unused, these fields need to be set to null string. They are declared as non-null fields and no default value is set in DB.
    // @TODO: Allow null as value in these fields in DB as they are non-mandatory fields or set their default value to a null string in DB.
    $fields['displayrating'] = '';
    $fields['displaymulti'] = '';
    $fields['position_no'] = '';
    $fields['image_thumbnail'] = '';

    expDebug::dPrint('$fields = ' . print_r($fields, true), 4);
    return $fields;
  }
  catch (Exception $ex) {
    watchdog_exception('prepareDataForQuestionDBRecord', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * sureveyQuestionAddEditFormWithCancelAndSaveClose() - Add the various form submit buttons to the question form.
 */
function sureveyQuestionAddEditFormWithCancelAndSaveClose($surveyType, &$form, &$form_state,
                                                             $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button',
                                                               $cancelButtonId = 'cancel-button',
                                                                  $expandPopup = FALSE, $saveTabIndex = '', $entityId = '', $esignature=false,$emptyId) {
  try {
  	global $theme_key;
	  if ($surveyType == 'sry_det_typ_ass') {
	    $surveyDeleteInfo = t('MSG357').' '.strtolower(t('LBL626')); // MSG357 = Are you sure you want to delete the, LBL626 = Assessment Question
	  }
	  else {
	    $surveyDeleteInfo = t('MSG357').' '.strtolower(t('LBL323')); // MSG357 = Are you sure you want to delete the, LBL323 = Survey Question
	  }

	  expDebug::dPrint('$surveyDeleteInfo = ' . $surveyDeleteInfo, 4);

	  $customHiddenButtonId = 'esign-button-'.$wrapperId;

	   $customHiddenButtonId = 'last-button-account';

	  $esignHiddenButt = 'esign-'.$wrapperId;

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
	  $cancelAttributes['tabindex'][] = '20';

	  $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
	  $prfixPgLeft = '';
	  $suffixPgRight = '';
	  if($theme_key == 'expertusoneV2'){
	  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
	  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
	  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
	  }
	  $form[$cancelButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
	                   '<div class="addedit-form-cancel-container-actions">'. $prfixPgLeft,
	    '#suffix' => $suffixPgRight,
	    '#value' => t('LBL123'), // Close
	    '#name' => 'cancel',
	    '#attributes' => $cancelAttributes
	  );

	  expDebug::dPrint('cancel button added', 4);

	  // Add the Save button
	  $ajaxCallbackSave = 'addEditRepaintForm';

	  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
	  // each time the form is re-built.
	  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

	  // For common styling of all addedit save buttons
	  //$saveAttributes['class'][] = 'addedit-edit-save';

	  // Using this class to applying bg color to the save button
	  $saveAttributes['class'][] = 'admin-save-button-middle-bg survey-question-admin-save-button-middle-bg';

	  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
	  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
	  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
	  // Also used by multiselect to show input as selected for only the current form
	  $saveAttributes['data-wrapperid'] = array($wrapperId);

	   $surStatus    =  $form_state['storage']['readonly_question'];
	   $isSaveParent = 0;
	   if(empty($entityId) || $surStatus == false) { // 'sry_qtn_sts_atv'
	   	  expDebug::dPrint('save button 1 is being added', 4);
	      $isSaveParent = 1;
	      $saveAttributes['onclick'] = array();
	      $saveAttributes['class'][] = 'admin-save-button-dropdown-middle-bg-wrapper';
	      $saveAttributes['onclick'][] = 'increasePopWidth();';
	      $saveAttributes['tabindex'] = '21';
	      $form[$saveButtonId] = array(
		    '#type' => 'submit',
		    '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
		    '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">' .
	                     '&nbsp;</span></div>',
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

	  if($entityId){
	      $resultCnt = delete_object_record($entityId,'SurAssQues');
	      expDebug::dPrint('$resultCnt = ' . $resultCnt, 4);
		}

		expDebug::dPrint('$isSaveParent = ' . $isSaveParent, 4);
	  if($isSaveParent){
	      $form['addedit-button-group-ul-start'] = array(
	        '#type' => 'markup',
	        '#markup' => '<ul class="catalog-pub-add-list">',
	      ); //  catalog-save-btn
	      $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
	      $savePublishSuffix = '</li>';
	      $savePublishAttributes['class'][] = ' ';
	      expDebug::dPrint('$savePublishSuffix = ' . $savePublishSuffix, 4);
	   }else{
	      $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
	      $savePublishSuffix = ((($resultCnt==0) && ($surStatus == 'sry_qtn_sts_atv'))?
	       ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn ">&nbsp;</span>') :
	       '<div class="admin-save-button-right-bg"></div>' ).'</div>';
	      $savePublishAttributes['class'][] = 'admin-save-button-middle-bg survey-question-admin-save-button-middle-bg';
	      $savePublishAttributes['class'][] = ((($resultCnt==0) && ($surStatus == 'sry_qtn_sts_atv'))? ('admin-save-button-dropdown-middle-bg-wrapper') :
	                                                      '' );
	  }

	  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
	  $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
	  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
	  // Also used by multiselect to show input as selected for only the current form
	  $savePublishAttributes['data-wrapperid'] = array($wrapperId);

		$saveButtonId = 'catalog-course-save-publish';
		$savePublishAttributes['tabindex'][] = '9';
		expDebug::dPrint('$savePublishAttributes = ' . print_r($savePublishAttributes, true), 4);
	  $buttonLabel = ($surStatus == 'sry_qtn_sts_atv') ? t('LBL141') : t('LBL614');
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
		expDebug::dPrint('save added 2', 4);


	  if(!$isSaveParent){
	    $form['addedit-button-group-ul-start'] = array(
	      '#type' => 'markup',
	      '#markup' => '<ul class="catalog-pub-add-list">',
	    );
	  }

		if(($entityId) && $surStatus == 'sry_qtn_sts_atv' ){
		  if($resultCnt == 0){
		  	  expDebug::dPrint('unpublish added', 4);
	  	    $saveButtonId = 'catalog-course-save-unpublish';

	       // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
	       $saveUnPublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
	       // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
	       // Also used by multiselect to show input as selected for only the current form
	       $saveUnPublishAttributes['data-wrapperid'] = array($wrapperId);

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
		if($surveyType == 'sry_det_typ_sry'){$questionType = 'sry_det_typ_sry_qus'; }else{$questionType = 'sry_det_typ_ass_qus';}
		$deleteCheck = adminVisibilityForAddEditDelete($entityId,$questionType,$questionType,'slt_survey_questions');
		if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
	    if(($entityId)){
	      if($resultCnt == 0){
	      	expDebug::dPrint('delete added', 4);
	        $displayTitle = titleController('ADMIN-QUESTIONS-DETAILS-TEXT', $questionDetails['question_txt'],34);
	  	   $saveButtonId = 'catalog-course-delete';

	  	   $deleteAttributes['onclick'] = array();
	        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.$surveyDeleteInfo.' '.
	                                                                escape_string($questionDetails['question_txt']) . '","'.$entityId.'","SurAssQues"); return false;';
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


	  expDebug::dPrint('hidden save added', 4);

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

	  $form['sortable-onload'] = array(
	            '#type' => 'markup',
	            '#prefix' => '<img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0"/>',
	          );

	  if(!empty($emptyId)){
	  	$entityId = 0;
	  } else {
	  	$emptyId = 0;
	  }

	  $entityType        	 = $questionType;
	  $qtipIdInit        	 = $entityId.'_'.$entityType;

	  $qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'$entityType','url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."','wid':650,'heg':'270','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-questions-".$entityId."'}";
	  /* Ticket: 46992 */

	 // $emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
	  //$emptyId           = 0;

	  //expDebug::dPrint('hidden save added $emptyId'. $emptyId."----". print_r($form_state['triggering_element']));
	  $tagsvisibPopupId  = 'qtip_visible_disp_tags_'.$qtipIdInit;
	  $qtipTagsOptObj    = "{'entityId':".$entityId.",'entityType':'".$entityType."','url':'administration/catalog-tags/ajax/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'".$tagsvisibPopupId."','catalogVisibleId':'renderTagsId".$qtipIdInit."','wid':400,'heg':'150','postype':'middle','poslwid':'','qdis':'ctool','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";

	  /* Ticket: 46992 */
	  $form['addedit-left-col-extend-func'] = array(
	  		'#type' => 'markup',
	  		'#markup' => '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
	  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-questions-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
	  		/* Ticket: 46992 */ "<div class='tab-seperator'></div><div title='".$tagsLabel."' class='tags-tab-icon vtip'></div>".
	  		"<div id='qtip_visible_disp_tags_".$qtipIdInit."'>".
	  		"<a onclick=\"callVisibility(".$qtipTagsOptObj.");\" class='tab-title' id='visible-tags-".$entityId."'>".t('LBL191')."</a>".
	  		"<span style='display:none; position:absolute; left:0px; top:0px;' class='qtip-popup-visible' id='visible-popup-".$entityId."'></span>".
	  		"</div></div>",/* Ticket: 46992 */
	  );

	  $form['addedit-left-col-end'] = array(
	    '#type' => 'markup',
	    '#markup' => '</div><div class="clearBoth"></div>',
	  );
  } catch (Exception $ex) {
    watchdog_exception('sureveyQuestionAddEditFormWithCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_dpn_radios_element_validate() - Validation function for asmt_dpn_radios custom type field.
 *                                      Derived from the validation being done in form.inc:_form_validate() for radios.
 */
function asmt_dpn_radios_element_validate($element, &$form_state) {
	try {
		expDebug::dPrint('$element = ' . print_r($element, true), 5);
		expDebug::dPrint('$element[#value] = ' . print_r($element['#value'], true), 4);
		if (!is_null($element['#value']['choice-radio']) && !isset($element['#asmt_dpn_options'][$element['#value']['choice-radio']])) {
			form_error($element, t('An illegal choice has been detected. Please contact the site administrator.'));
			watchdog('form', 'Illegal choice %choice for correct answer selection detected in !name element.',
					array('%choice' => $element['#value']['choice-radio'],
							'!name' => empty($element['#title'])? $element['#parents'][0] : $element['#title']), WATCHDOG_ERROR);
		}

		foreach ($element['#value'] as $key => $value) {
			if (substr($key, 0, strlen('choice-text-')) === 'choice-text-') {
				$idx = substr($key, strlen('choice-text-'));
			}

			if (empty($idx) || !isset($element['#asmt_dpn_options'][$idx])) {
				form_error($element, t('An illegal choice has been detected. Please contact the site administrator.'));
				watchdog('form', 'Illegal choice %choice for choice text field detected in !name element.',
						array('%choice' => empty($idx)? 0 : $idx,
								'!name' => empty($element['#title'])? $element['#parents'][0] : $element['#title']), WATCHDOG_ERROR);
			}
		}
		// Validate Special Characters 0038953
		//asmt_element_xss_validate($element);
		//commented for #0065306
	}
	catch (Exception $ex) {
		watchdog_exception('asmt_dpn_radios_element_validate', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * asmt_element_xss_validate() - Special character validation for Assesment Questions.
 * Created this Function For This Ticket #0038953
 */
function asmt_element_xss_validate($form_state) {
	try{
		$terms =array('(?i)<\s*script','(?i)<\s*img','(?i)<\s*b','(?i)<\s*input','(?i)<\s*a','(?i)<\s*body','(?i)<\s*meta','(?i)<\s*\?');
		$result = array();
		expDebug::dPrint('$form_state = ' . print_r($form_state, true), 5);
		expDebug::dPrint('$$form_state[#value] = ' . print_r($form_state['#value'], true), 4);

		foreach($form_state['#value'] as $key=>$data){

			expDebug::dPrint('Value : '.print_r($form_state['#value'][$key], true), 4);
			if(!is_array($form_state['#value'][$key]) && !is_object($form_state['#value'][$key]) ){
				foreach($terms as $term){
					if(!count($result)){
						$result = preg_filter('~' . $term . '~','$0', $form_state['#value'][$key]);
					}else{
						break;
					}
				}
			}
			expDebug::dPrint('count($result) : '.print_r(count($result), true), 4);
			if(count($result)){
				break;
			}
			if(!is_array($form_state['#value'][$key]) && !is_object($form_state['#value'][$key]) ){
				if($form_state['values']['form_id'] != 'page_node_form' && $form_state['#value'][$key] != 'date'){
					expDebug::dPrint('Key  : '.print_r($form_state['#value'][$key], true), 4);
					$form_state['#value'][$key] = xss_clean($form_state['#value'][$key],$key);
				}
			}
		}
		expDebug::dPrint('Total Result : '.print_r(count($result), true), 4);
		if(count($result)){
			form_set_error('',  t('LBL1273'));
			$form_state["storage"]["form_validate"] = false;
			if(module_exists('exp_sp_esignature')){
				$form_state['esign_show_popup']= false;
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('asmt_dpn_radios_element_xss_validate', $ex);
		expertusErrorThrow($ex);
	}

}

/*
 * asmt_dpn_radios_value_callback() - Helper function to determine the value for an asmt_dpn_radios custom type form element.
 *                                         Derived from form.inc:form_type_radios_value()
 */
function asmt_dpn_radios_value_callback($element, $input = FALSE) {
  try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  expDebug::dPrint('$input = ' . print_r($input, true), 5);
	  $elementValue = array();
	  if ($input === FALSE) {
	    $element += array('#default_value' => NULL);
	    $elementValue['choice-radio'] = $element['#default_value'];

	    $element += array('#asmt_dpn_options' => array());
	    foreach ($element['#asmt_dpn_options'] as $key => $value) {
	      $elementValue['choice-text-' . $key] = $value;
	    }
	  }
	  elseif (is_array($input)) {
	    $elementValue = $input;
	  }

	  expDebug::dPrint('$elementValue' . print_r($elementValue, true), 4);
	  return $elementValue;
  }
  catch (Exception $ex) {
    watchdog_exception('asmt_dpn_radios_value_callback', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * process_asmt_dpn_radios() - process function for custom 'theme_asmt_dpn_radios' form element.
 *                                 Derived from form.inc:process_radios()
 */
function process_asmt_dpn_radios($element) {
  try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  $value = is_array($element['#value']) ? $element['#value'] : array();

	  $weight = 0;
	  $tabIndex =  empty($element['#attributes']['tabindex'])? 0 : $element['#attributes']['tabindex'];

	  foreach ($element['#asmt_dpn_options'] as $key => $choice) {
	    // Maintain order of options as defined in #options, in case the element
	    // defines custom option sub-elements, but does not define all option
	    // sub-elements.
	    $weight += 0.001;
	    $element += array('choice-text-' . $key => array());
	    $choiceTextareaAttributes = array();
	    $choiceTextareaAttributes['autocomplete'] = 'off';
	    $choiceTextareaAttributes['class'] = array('assessment-answer-choice-textarea');
	    if ($tabIndex > 0) {
	      $choiceTextareaAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-text-' . $key] += array(
	      '#type' => 'textarea',
	      '#default_value' => isset($value['choice-text-' . $key]) ? $value['choice-text-' . $key] : '',
	      '#maxlength' => '500',
	      '#size' => '15',
	      '#weight' => $weight,
	      '#attributes' => $choiceTextareaAttributes,
	    );

	    $weight += 0.001;
	    $element += array('choice-radio-' . $key => array());
	    $choiceCheckboxAttributes = $element['#attributes'];
	    if ($tabIndex > 0) {
	      $choiceCheckboxAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    // Generate the parents as the autogenerator does, so we will have a
	    // unique id for each radio button.
	    $parents_for_id = array_merge($element['#parents'], array('choice-radio'), array($key));
	    $element['choice-radio-' . $key] += array(
	      '#type' => 'radio',
	      '#title' => $choice,
	      // The key is sanitized in drupal_attributes() during output from the
	      // theme function.
	      '#return_value' => $key,
	      '#default_value' => isset($value['choice-radio']) ? $value['choice-radio']  : NULL,
	      '#attributes' => $choiceCheckboxAttributes,
	      '#parents' => array_merge($element['#parents'], array('choice-radio')), // This gives the radio buttons a common name
	      '#id' => drupal_html_id('edit-' . implode('-', $parents_for_id)),
	      '#ajax' => NULL,
	      '#weight' => $weight,
	    );

	    $weight += 0.001;
	    $element += array('choice-delete-' . $key => array());
	    $choiceDeleteAttributes = array();
	    $choiceDeleteAttributes['class'][] = 'addedit-form-expertusone-throbber';
	    $choiceDeleteAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	    if ($tabIndex > 0) {
	      $choiceDeleteAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-delete-' . $key] += array(
	      '#type'       => 'submit',
	      '#name'       => 'choice-delete-' . $key,  // IMPT: #name has to be unique, otherwise the last del button is triggered. Alternatively,
	      //'#value'      => 'choice-delete-' . $key, //      unique #value can be given if we want to keep the #name of all del buttons same
	      '#choice-id'  => $key,
	      '#attributes' => $choiceDeleteAttributes,
	      '#prefix' =>'<span class="surass-ques-delete ">',
          '#suffix' => '</span>',
	      '#weight'     => $weight,
	      '#validate'   => array('asmt_dpn_radios_delete_choice'),
	      '#ajax'       => array(
	        'wrapper'         => 'survey-question-basic-addedit-form',
	        'callback'        => 'repaintSurveyQuestionForm',
	        'method'          => 'replace',
	        'effect'          => 'none',
	        'event'           => 'click',
	        'keypress'        => true,
	        'progress'        => array (
	          'type'    => 'throbber',
	          'message' => ''
	        ),
	      ),
	    );
	  } // end foreach

	  $weight += 0.001;
	  $element += array('choice-add' => array());
	  $choiceAddAttributes = array();
	  $choiceAddAttributes['class'][] = 'add-custombtn-symbol';
	  $choiceAddAttributes['class'][] = 'admin-action-button-middle-bg';
	  $choiceAddAttributes['class'][] = 'addedit-form-expertusone-throbber';
	  $choiceAddAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	  if ($tabIndex > 0) {
	    $choiceAddAttributes['tabindex'] = $tabIndex;
	    $tabIndex++;
	  }
	  $element['choice-add'] += array(
	    '#type'       => 'submit',
	    '#name'       => 'choice-add',
	    '#value'      => t('LBL386'), // IMPT: #value needed to paint the button label.
	    '#attributes' => $choiceAddAttributes,
	    '#weight'     => $weight,
	    '#validate'   => array('asmt_dpn_radios_new_choice'),
	    '#ajax'       => array(
	      'wrapper'         => 'survey-question-basic-addedit-form',
	      'callback'        => 'repaintSurveyQuestionForm',
	      'method'          => 'replace',
	      'effect'          => 'none',
	      'event'           => 'click',
	      'keypress'        => true,
	      'progress'        => array (
	        'type'    => 'throbber',
	        'message' => ''
	      ),
	    ),
	  );
	  expDebug::dPrint('returning $element = ' . print_r($element, true), 5);
	  return $element;
  }
  catch (Exception $ex) {
    watchdog_exception('process_asmt_dpn_radios', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_dpn_radios_delete_choice() - Removes an answer option from being presented to the user and instructs drupal to rebuild the form
 */
function asmt_dpn_radios_delete_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $prevCorrectAnswer = $form_state['values']['answerchoices']['choices']['choice-radio'];
	  $deleteChoiceId = $form_state['triggering_element']['#choice-id'];
	  $newCorrectAnswer = NULL; // initialize
	  $newChoices = array(); // initialize
	  $j = 1; // index for the updated choices list
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    if ($i != $deleteChoiceId) {
	      $newChoices[$j] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	      if (!empty($prevCorrectAnswer) && $i == $prevCorrectAnswer) {
	        $newCorrectAnswer = $j;
	      }
	      $j++;
	    }
	  } // end for loop

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;
	  $form_state['storage']['answerchoices']['rightanswer'] = $newCorrectAnswer;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
  }
  catch (Exception $ex) {
    watchdog_exception('asmt_dpn_radios_delete_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_dpn_radios_new_choice() - Adds a new answer option field and instructs drupal to rebuild the form
 */
function asmt_dpn_radios_new_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $correctAnswer = $form_state['values']['answerchoices']['choices']['choice-radio'];
	  $newChoices = array(); // initialize
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    $newChoices[$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	  }
	  $newChoices[$i] = ''; // Add new blank choice

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;
	  $form_state['storage']['answerchoices']['rightanswer'] = $correctAnswer;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
  }
  catch (Exception $ex) {
    watchdog_exception('asmt_dpn_radios_new_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * theme_asmt_dpn_radios() - derived from form.inc:theme_radios()
 *                           for dropdown type assessment answer choices
 */
function theme_asmt_dpn_radios($variables) {
	try {
	  expDebug::dPrint('$variables = ' . print_r($variables, true), 5);

	  $element = $variables['element'];

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' .
	                             '<div class="addedit-new-field-title">' .
	                                t('Answers').':' . '<span class="addedit-mandatory">*</span>' .
	                             '</div>' .
	                             '<div class="addedit-new-field-value">&nbsp;</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  // Prepare html for each radio option
	  $i = 1;
	  foreach ($element['#asmt_dpn_options'] as $key => $value) {
	    expDebug::dPrint('$element[choice-radio-' . $key. '] = ' . print_r($element['choice-radio-' . $key], true), 5);
	    $element['#children'] .=  '<div class="addedit-new-field">' . // Each radio is a new row
	                                '<table>' .
	                                  '<tr>' .
	                                    '<td class="div-align-top">' .
	                                      '<div class="addedit-new-field-title-choice">' .
	                                        t(LBL387) . ' ' . $i . ':' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="dpn-answer-choice-text">' .
	                                        drupal_render($element['choice-text-' . $key]) .
	                                      '</div>' . // choice textfield - floats to the left
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="dpn-correct-answer-radio">' . // checkbox container
	                                        theme('radio', array('element' => $element['choice-radio-' . $key])) . '<span>' . t('LBL714') . '</span>' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="dpn-answer-choice-delete-button">' . // delete button container
	                                        drupal_render($element['choice-delete-' . $key]) .
	                                      '</div>' .
	                                    '</td>' .
	                                  '</tr>' .
	                                '</table>' .
	                                '<div style="clear:both"></div>' .
	                              '</div>';
	    $i++;
	  } // end foreach

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' . // Add choice button row
	                             '<div class="addedit-new-field-title">&nbsp;</div>' .
	                             '<div class="addedit-new-field-value">' .
	                               '<div class="addedit-form-cancel-container-actions">' .
	                                 drupal_render($element['choice-add']) .
	                               '</div>' .
	                             '</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  $attributes = array();
	  if (isset($element['#id'])) {
	    $attributes['id'] = $element['#id'];
	  }
	  $attributes['class'] = 'form-radios';
	  if (!empty($element['#attributes']['class'])) {
	    $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
	  }

	  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
  }
  catch (Exception $ex) {
    watchdog_exception('theme_asmt_dpn_radios', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * asmt_mch_checkboxes_element_validate() - Validation function for asmt_mch_checkboxes custom type field.
 *                                          Derived from the validation being done in form.inc:_form_validate() for checkboxes.
 */
function asmt_mch_checkboxes_element_validate($element, &$form_state) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  expDebug::dPrint('$element[#value] = ' . print_r($element['#value'], true), 4);
	  foreach ($element['#value'] as $key => $value) {
	    if (substr($key, 0, strlen('choice-text-')) === 'choice-text-') {
	      $idx = substr($key, strlen('choice-text-')) ;
	    }
	    else if (substr($key, 0, strlen('choice-checkbox-')) === 'choice-checkbox-') {
	      $idx = substr($key, strlen('choice-checkbox-')) ;
	    }

	    if (empty($idx) || !isset($element['#asmt_mch_options'][$idx])) {
	      form_error($element, t('An illegal choice has been detected. Please contact the site administrator.'));
	      watchdog('form', 'Illegal choice %choice for text field or correct answer selection detected in !name element.',
	                  array('%choice' => empty($idx)? 0 : $idx,
	                        '!name' => empty($element['#title'])? $element['#parents'][0] : $element['#title']), WATCHDOG_ERROR);
	    }
	  }
	  // Validate Special Characters In Answers 0038953
	//asmt_element_xss_validate($element);
	//commented for #0065306
  }
  catch (Exception $ex) {
    watchdog_exception('asmt_mch_checkboxes_element_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_mch_checkboxes_value_callback() - Helper function to determine the value for an asmt_mch_checkboxes custom type form element.
 *                                         Derived from form.inc:form_type_checkboxes_value()
 */
function asmt_mch_checkboxes_value_callback($element, $input = FALSE) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  expDebug::dPrint('$input = ' . print_r($input, true), 5);
	  $elementValue = array();
	  if ($input === FALSE) {
	    $element += array('#default_value' => array());
	    foreach ($element['#default_value'] as $key) {
	      $elementValue['choice-checkbox-' . $key] = $key;
	    }

	    $element += array('#asmt_mch_options' => array());
	    foreach ($element['#asmt_mch_options'] as $key => $value) {
	      $elementValue['choice-text-' . $key] = $value;
	    }
	  }
	  elseif (is_array($input)) {
	    $elementValue = $input;
	  }

	  expDebug::dPrint('$elementValue' . print_r($elementValue, true), 4);
	  return $elementValue;
	}
  catch (Exception $ex) {
    watchdog_exception('asmt_mch_checkboxes_value_callback', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * process_asmt_mch_checkboxes() - process function for custom 'asmt_mch_checkboxes' form element.
 *                                 Derived from form.inc:process_checkboxes()
 */
function process_asmt_mch_checkboxes($element) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  $value = is_array($element['#value']) ? $element['#value'] : array();
	  $element['#tree'] = TRUE;

	  if (!isset($element['#default_value']) || $element['#default_value'] == 0) {
	    $element['#default_value'] = array();
	  }

	  $weight = 0;
	  $tabIndex =  empty($element['#attributes']['tabindex'])? 0 : $element['#attributes']['tabindex'];

	  foreach ($element['#asmt_mch_options'] as $key => $choice) {
	    // Integer 0 is not a valid #return_value, so use '0' instead.
	    // @see form_type_checkbox_value().
	    // @todo For Drupal 8, cast all integer keys to strings for consistency
	    //   with form_process_radios().
	    if ($key === 0) {
	      $key = '0';
	    }
	    // Maintain order of options as defined in #asmt_mch_options, in case the element
	    // defines custom option sub-elements, but does not define all option
	    // sub-elements.
	    $weight += 0.001;
	    $element += array('choice-text-' . $key => array());
	    $choiceTextareaAttributes = array();
	    $choiceTextareaAttributes['autocomplete'] = 'off';
	    $choiceTextareaAttributes['class'][] = 'addedit-edit-textarea';
	    $choiceTextareaAttributes['class'][] = 'assessment-answer-choice-textarea';
	    if ($tabIndex > 0) {
	      $choiceTextareaAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-text-' . $key] += array(
	      '#type' => 'textarea',
	      '#default_value' => isset($value['choice-text-' . $key]) ? $value['choice-text-' . $key] : '',
	      '#maxlength' => '500',
	      '#size' => '15',
	      '#weight' => $weight,
	      '#attributes' => $choiceTextareaAttributes,
	    );

	    $weight += 0.001;
	    $element += array('choice-checkbox-' . $key => array());
	    $choiceCheckboxAttributes = $element['#attributes'];
	    $choiceCheckboxAttributes['onclick'][] = 'checkboxSelectedUnselectedMultiParent(this);';
	    if ($tabIndex > 0) {
	      $choiceCheckboxAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $className = isset($value['choice-checkbox-' . $key]) ? "survey-checkbox-selected" : "survey-checkbox-unselected";
	    $element['choice-checkbox-' . $key] += array(
	      '#type' => 'checkbox',
	      '#prefix' => '<div class='.$className.'>',
         '#suffix' => '</div>',
	      '#return_value' => $key,
	      '#default_value' => isset($value['choice-checkbox-' . $key]) ? $key : NULL,
	      '#attributes' => $choiceCheckboxAttributes,
	      '#ajax' => NULL,
	      '#weight' => $weight
	    );

	    $weight += 0.001;
	    $element += array('choice-delete-' . $key => array());
	    $choiceDeleteAttributes = array();
	    $choiceDeleteAttributes['class'][] = 'addedit-form-expertusone-throbber';
	    $choiceDeleteAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	    if ($tabIndex > 0) {
	      $choiceDeleteAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-delete-' . $key] += array(
	      '#type'       => 'submit',
	      '#name'       => 'choice-delete-' . $key,  // IMPT: #name has to be unique, otherwise the last del button is triggered. Alternatively,
	      //'#value'      => 'choice-delete-' . $key, //      unique #value can be given if we want to keep the #name of all del buttons same
	      '#choice-id'  => $key,
	      '#attributes' => $choiceDeleteAttributes,
	      '#prefix' =>'<span class="surass-ques-delete ">',
          '#suffix' => '</span>',
	      '#weight'     => $weight,
	      '#validate'   => array('asmt_mch_checkboxes_delete_choice'),
	      '#ajax'       => array(
	        'wrapper'         => 'survey-question-basic-addedit-form',
	        'callback'        => 'repaintSurveyQuestionForm',
	        'method'          => 'replace',
	        'effect'          => 'none',
	        'event'           => 'click',
	        'keypress'        => true,
	        'progress'        => array (
	          'type'    => 'throbber',
	          'message' => ''
	        ),
	      ),
	    );
	  } // end foreach

	  $weight += 0.001;
	  $element += array('choice-add' => array());
	  $choiceAddAttributes = array();
	  $choiceAddAttributes['class'][] = 'add-custombtn-symbol';
	  $choiceAddAttributes['class'][] = 'admin-action-button-middle-bg';
	  $choiceAddAttributes['class'][] = 'addedit-form-expertusone-throbber';
	  $choiceAddAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	  if ($tabIndex > 0) {
	    $choiceAddAttributes['tabindex'] = $tabIndex;
	    $tabIndex++;
	  }
	  $element['choice-add'] += array(
	    '#type'       => 'submit',
	    '#name'       => 'choice-add',
	    '#value'      => t('LBL386'),  // IMPT: #value needed to paint the button label.
	    '#attributes' => $choiceAddAttributes,
	    '#weight'     => $weight,
	    '#validate'   => array('asmt_mch_checkboxes_new_choice'),
	    '#ajax'       => array(
	      'wrapper'         => 'survey-question-basic-addedit-form',
	      'callback'        => 'repaintSurveyQuestionForm',
	      'method'          => 'replace',
	      'effect'          => 'none',
	      'event'           => 'click',
	      'keypress'        => true,
	      'progress'        => array (
	        'type'    => 'throbber',
	        'message' => ''
	      ),
	    ),
	  );
	  expDebug::dPrint('returning $element = ' . print_r($element, true), 5);
	  return $element;
	}
  catch (Exception $ex) {
    watchdog_exception('process_asmt_mch_checkboxes', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_mch_checkboxes_delete_choice() - Removes an answer option from being presented to the user and instructs drupal to rebuild the form
 */
function asmt_mch_checkboxes_delete_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $deleteChoiceId = $form_state['triggering_element']['#choice-id'];
	  $newCorrectAnswers = array(); // initialize
	  $newChoices = array(); // initialize
	  $j = 1; // index for the updated choices list
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    if ($i != $deleteChoiceId) {
	      $newChoices[$j] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	      if (!empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
	        $newCorrectAnswers[$j] = $j;
	      }
	      $j++;
	    }
	  } // end for loop

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;
	  $form_state['storage']['answerchoices']['rightanswer'] = $newCorrectAnswers;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
  }
  catch (Exception $ex) {
    watchdog_exception('asmt_mch_checkboxes_delete_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * asmt_mch_checkboxes_new_choice() - Adds a new answer option field and instructs drupal to rebuild the form
 */
function asmt_mch_checkboxes_new_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $newCorrectAnswers = array(); // initialize
	  $newChoices = array(); // initialize
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    $newChoices[$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	    if (!empty($form_state['values']['answerchoices']['choices']['choice-checkbox-' . $i])) {
	      $newCorrectAnswers[$i] = $i;
	    }
	  } // end for loop
	  $newChoices[$i] = ''; // Add new blank choice

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;
	  $form_state['storage']['answerchoices']['rightanswer'] = $newCorrectAnswers;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
	}
  catch (Exception $ex) {
    watchdog_exception('asmt_mch_checkboxes_new_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * theme_asmt_mch_checkboxes() - derived from form.inc:theme_checkboxes()
 * for multiple choice type assessment answer choices
 */
function theme_asmt_mch_checkboxes($variables) {
	try {
	  expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
	  $element = $variables['element'];

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' .
	                             '<div class="addedit-new-field-title">' .
	                                t('Answers').':'. '<span class="addedit-mandatory">*</span>' .
	                             '</div>' .
	                             '<div class="addedit-new-field-value">&nbsp;</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  // Prepare html for each option
	  $i = 1;
	  foreach ($element['#asmt_mch_options'] as $key => $value) {
	    expDebug::dPrint('$element[choice-checkbox-' . $key. ']' . print_r($element['choice-checkbox-' . $key], true), 5);
	    $element['#children'] .=  '<div class="addedit-new-field">' . // Each checkbox is a new row
	                                '<table>' .
	                                  '<tr>' .
	                                    '<td class="div-align-top">' .
	                                      '<div class="addedit-new-field-title-choice">' .
	                                        t(LBL387) . ' ' . $i . ':' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="mch-answer-choice-text">' .
	                                        drupal_render($element['choice-text-' . $key]) .
	                                      '</div>' . // choice textfield - floats to the left
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="mch-correct-answer-checkbox">' . // checkbox container
	                                        drupal_render($element['choice-checkbox-' . $key]) . '<span>' . t('LBL714') . '</span>' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="mch-answer-choice-delete-button">' . // delete button container
	                                        drupal_render($element['choice-delete-' . $key]) .
	                                      '</div>' .
	                                    '</td>' .
	                                  '</tr>' .
	                                '</table>' .
	                                '<div style="clear:both"></div>' .
	                              '</div>';
	    $i++;
	  } // end foreach

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' . // Add choice button row
	                             '<div class="addedit-new-field-title">&nbsp;</div>' .
	                             '<div class="addedit-new-field-value">' .
	                               '<div class="addedit-form-cancel-container-actions">' .
	                                 drupal_render($element['choice-add']) .
	                               '</div>' .
	                             '</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  $attributes = array();
	  if (isset($element['#id'])) {
	    $attributes['id'] = $element['#id'];
	  }
	  $attributes['class'][] = 'form-checkboxes';
	  if (!empty($element['#attributes']['class'])) {
	    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
	  }
	  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
	}
  catch (Exception $ex) {
    watchdog_exception('theme_asmt_mch_checkboxes', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * theme_asmt_yntf_radios() - override of form.inc:theme_radios()
 * for yes/no and true/false type assessment quesion's answer choices
 */
function theme_asmt_yntf_radios($variables) {
	try {
	  expDebug::dPrint('$variables = ' . print_r($variables, true), 5);

	  $element = $variables['element'];

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' .
	                             '<div class="addedit-new-field-title">' .
	                                t('Answers').':'. '<span class="addedit-mandatory">*</span>' .
	                             '</div>' .
	                             '<div class="addedit-new-field-value">&nbsp;</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  // Prepare html for each radio option
	  foreach ($element['#options'] as $key => $value) {
	    if($value =='Yes'){
	        $labelValue = t('Yes');
	    }elseif($value =='No'){
	        $labelValue = t('No');
	    }elseif($value =='True'){
	        $labelValue = t('LBL384');
	    }else{
	        $labelValue = t('LBL385');
	    }
	    $element['#children'] .=  '<div class="addedit-new-field">' . // Each checkbox is a new row
	                                '<table>' .
	                                  '<tr>' .
	                                    '<td class="div-align-top">' .
	                                      '<div class="addedit-new-field-title">' .
	                                        '&nbsp;' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="yntf-answer-choice-text">' .
	                                        $labelValue .
	                                      '</div>' . // choice textfield - floats to the left
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="yntf-answer-radio">' . // checkbox container
	                                        theme('radio', array('element' => $element[$key])) . '<span>' . t('LBL714') . '</span>' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="mch-answer-choice-delete-button">' . // delete button container
	                                        '&nbsp;' .
	                                      '</div>' .
	                                    '</td>' .
	                                  '</tr>' .
	                                '</table>' .
	                                '<div style="clear:both"></div>' .
	                              '</div>';

	  } // end foreach

	  $attributes = array();
	  if (isset($element['#id'])) {
	    $attributes['id'] = $element['#id'];
	  }
	  $attributes['class'] = 'form-radios';
	  if (!empty($element['#attributes']['class'])) {
	    $attributes['class'] .= ' ' . implode(' ', $element['#attributes']['class']);
	  }
	  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
	}
  catch (Exception $ex) {
    watchdog_exception('theme_asmt_yntf_radios', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * survey_question_fields_element_validate() - Validation function for survey_question_fields custom type field.
 */
function survey_question_fields_element_validate($element, &$form_state) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  expDebug::dPrint('$element[#value] = ' . print_r($element['#value'], true), 4);
	  foreach ($element['#value'] as $key => $value) {
	    if (substr($key, 0, strlen('choice-text-')) === 'choice-text-') {
	      $idx = substr($key, strlen('choice-text-')) ;
	    }

	    if (empty($idx) || !isset($element['#survey_question_options'][$idx])) {
	      form_error($element, t('An illegal choice has been detected. Please contact the site administrator.'));
	      watchdog('form', 'Illegal choice %choice for text field or correct answer selection detected in !name element.',
	                  array('%choice' => empty($idx)? 0 : $idx,
	                        '!name' => empty($element['#title'])? $element['#parents'][0] : $element['#title']), WATCHDOG_ERROR);
	    }
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('survey_question_fields_element_validate', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * survey_question_fields_value_callback() - Helper function to determine the value for an survey_question_fields custom type form element.
 */
function survey_question_fields_value_callback($element, $input = FALSE) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  expDebug::dPrint('$input = ' . print_r($input, true), 5);
	  $elementValue = array();
	  if ($input === FALSE) {
	    $element += array('#survey_question_options' => array()); // If not present, add.
	    foreach ($element['#survey_question_options'] as $key => $value) {
	      $elementValue['choice-text-' . $key] = $value;
	    }
	  }
	  elseif (is_array($input)) {
	    $elementValue = $input;
	  }

	  expDebug::dPrint('$elementValue' . print_r($elementValue, true), 4);
	  return $elementValue;
  }
  catch (Exception $ex) {
    watchdog_exception('survey_question_fields_value_callback', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * process_survey_question_fields() - process function for custom 'survey_question_fields' form element.
 */
function process_survey_question_fields($element) {
	try {
	  expDebug::dPrint('$element = ' . print_r($element, true), 5);
	  $value = is_array($element['#value']) ? $element['#value'] : array();
	  $element['#tree'] = TRUE;

	  $weight = 0;
	  $tabIndex =  empty($element['#attributes']['tabindex'])? 0 : $element['#attributes']['tabindex'];

	  foreach ($element['#survey_question_options'] as $key => $choice) {
	    // Integer 0 is not a valid #return_value, so use '0' instead.
	    // @see form_type_checkbox_value().
	    // @todo For Drupal 8, cast all integer keys to strings for consistency
	    //   with form_process_radios().
	    if ($key === 0) {
	      $key = '0';
	    }
	    // Maintain order of options as defined in #survey_question_options, in case the element
	    // defines custom option sub-elements, but does not define all option
	    // sub-elements.
	    $weight += 0.001;
	    $element += array('choice-text-' . $key => array());
	    $choiceTextareaAttributes = array();
	    $choiceTextareaAttributes['autocomplete'] = 'off';
	    $choiceTextareaAttributes['class'][] = 'addedit-edit-textarea';
	    $choiceTextareaAttributes['class'][] = 'survey-answer-choice-textarea';
	    if ($tabIndex > 0) {
	      $choiceTextareaAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-text-' . $key] += array(
	      '#type' => 'textarea',
	      '#default_value' => isset($value['choice-text-' . $key]) ? $value['choice-text-' . $key] : '',
	      '#maxlength' => '500',
	      '#size' => '15',
	      '#weight' => $weight,
	      '#attributes' => $choiceTextareaAttributes,
	    );

	    $weight += 0.001;
	    $element += array('choice-delete-' . $key => array());
	    $choiceDeleteAttributes = array();
	    $choiceDeleteAttributes['class'][] = 'addedit-form-expertusone-throbber';
	    $choiceDeleteAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	    if ($tabIndex > 0) {
	      $choiceDeleteAttributes['tabindex'] = $tabIndex;
	      $tabIndex++;
	    }
	    $element['choice-delete-' . $key] += array(
	      '#type'       => 'submit',
	      '#name'       => 'choice-delete-' . $key,  // IMPT: #name has to be unique, otherwise the last del button is triggered. Alternatively,
	      //'#value'      => 'choice-delete-' . $key, //      unique #value can be given if we want to keep the #name of all del buttons same
	      '#choice-id'  => $key,
	      '#attributes' => $choiceDeleteAttributes,
	      '#prefix' =>'<span class="surass-ques-delete ">',
          '#suffix' => '</span>',
	      '#weight'     => $weight,
	      '#validate'   => array('survey_question_fields_delete_choice'),
	      '#ajax'       => array(
	        'wrapper'         => 'survey-question-basic-addedit-form',
	        'callback'        => 'repaintSurveyQuestionForm',
	        'method'          => 'replace',
	        'effect'          => 'none',
	        'event'           => 'click',
	        'keypress'        => true,
	        'progress'        => array (
	          'type'    => 'throbber',
	          'message' => ''
	        ),
	      ),
	    );
	  } // end foreach

	  $weight += 0.001;
	  $element += array('choice-add' => array());
	  $choiceAddAttributes = array();
	  $choiceAddAttributes['class'][] = 'add-custombtn-symbol';
	  $choiceAddAttributes['class'][] = 'admin-action-button-middle-bg';
	  $choiceAddAttributes['class'][] = 'addedit-form-expertusone-throbber';
	  $choiceAddAttributes['data-wrapperid'] = array('survey-question-basic-addedit-form');
	  if ($tabIndex > 0) {
	    $choiceAddAttributes['tabindex'] = $tabIndex;
	    $tabIndex++;
	  }
	  $element['choice-add'] += array(
	    '#type'       => 'submit',
	    '#name'       => 'choice-add',
	    '#value'      => t('LBL386'),  // IMPT: #value needed to paint the button label.
	    '#attributes' => $choiceAddAttributes,
	    '#weight'     => $weight,
	    '#validate'   => array('survey_question_fields_new_choice'),
	    '#ajax'       => array(
	      'wrapper'         => 'survey-question-basic-addedit-form',
	      'callback'        => 'repaintSurveyQuestionForm',
	      'method'          => 'replace',
	      'effect'          => 'none',
	      'event'           => 'click',
	      'keypress'        => true,
	      'progress'        => array (
	        'type'    => 'throbber',
	        'message' => ''
	      ),
	    ),
	  );
	  expDebug::dPrint('returning $element = ' . print_r($element, true), 5);
	  return $element;
	}
  catch (Exception $ex) {
    watchdog_exception('process_survey_question_fields', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * survey_question_fields_delete_choice() - Removes an answer option from being presented to the user and instructs drupal to rebuild the form
 */
function survey_question_fields_delete_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $deleteChoiceId = $form_state['triggering_element']['#choice-id'];
	  $newChoices = array(); // initialize
	  $j = 1; // index for the updated choices list
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    if ($i != $deleteChoiceId) {
	      $newChoices[$j] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	      $j++;
	    }
	  } // end for loop

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
	}
  catch (Exception $ex) {
    watchdog_exception('survey_question_fields_delete_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * survey_question_fields_new_choice() - Adds a new blank answer option field and instructs drupal to rebuild the form
 */
function survey_question_fields_new_choice($form, &$form_state) {
	try {
	  expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
	  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

	  $newCorrectAnswers = array(); // initialize
	  $newChoices = array(); // initialize
	  for ($i = 1; $i <= $form_state['values']['answerchoices']['count']; $i++) {
	    $newChoices[$i] = $form_state['values']['answerchoices']['choices']['choice-text-' . $i];
	  } // end for loop
	  $newChoices[$i] = ''; // Add new blank choice

	  $form_state['storage']['answerchoices']['count'] = count($newChoices);
	  $form_state['storage']['answerchoices']['options'] = $newChoices;

	  unset($form_state['values']['answerchoices']);
	  unset($form_state['input']['answerchoices']);

	  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
	  $form_state['rebuild'] = TRUE;
  }
  catch (Exception $ex) {
    watchdog_exception('survey_question_fields_new_choice', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * theme_survey_question_fields() - for survey_question_fields type
 */
function theme_survey_question_fields($variables) {
	try {
	  expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
	  $element = $variables['element'];

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' .
	                             '<div class="addedit-new-field-title">' .
	                                t('Answers').':'. '<span class="addedit-mandatory">*</span>' .
	                             '</div>' .
	                             '<div class="addedit-new-field-value">&nbsp;</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  // Prepare html for each option
	  $i = 1;
	  foreach ($element['#survey_question_options'] as $key => $value) {
	    $element['#children'] .=  '<div class="addedit-new-field">' . // Each checkbox is a new row
	                                '<table>' .
	                                  '<tr>' .
	                                    '<td class="div-align-top">' .
	                                      '<div class="addedit-new-field-title-choice">' .
	                                        t(LBL387) . ' ' . $i . ':' .
	                                      '</div>' .
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="survey-question-choice-text">' .
	                                        drupal_render($element['choice-text-' . $key]) .
	                                      '</div>' . // choice textfield - floats to the left
	                                    '</td>' .
	                                    '<td>' .
	                                      '<div class="survey-question-choice-delete-button">' . // delete button container
	                                        drupal_render($element['choice-delete-' . $key]) .
	                                      '</div>' .
	                                    '</td>' .
	                                  '</tr>' .
	                                '</table>' .
	                                '<div style="clear:both"></div>' .
	                              '</div>';
	    $i++;
	  } // end foreach

	  $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' . // Add choice button row
	                             '<div class="addedit-new-field-title">&nbsp;</div>' .
	                             '<div class="addedit-new-field-value">' .
	                               '<div class="addedit-form-cancel-container-actions">' .
	                                 drupal_render($element['choice-add']) .
	                               '</div>' .
	                             '</div>' .
	                             '<div class="clearBoth"></div>' .
	                           '</div>';

	  $attributes = array();
	  if (isset($element['#id'])) {
	    $attributes['id'] = $element['#id'];
	  }
	  if (!empty($element['#attributes']['class'])) {
	    $attributes['class'] = array_merge($attributes['class'], $element['#attributes']['class']);
	  }
	  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
  }
  catch (Exception $ex) {
    watchdog_exception('theme_survey_question_fields', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addNewSurveyQuestionByRestAPI() -
 */
function addNewSurveyQuestionByRestAPI(&$form_state,$createuserid="")
{
    $form=array();
  $form["form_id"]="";
  $form_state['values']['question_object_type'] = 'sry_det_typ_sry';
  // 46691- Clear backslash added during cleanup
  $form_state['values']['question_txt'] = stripslashes($form_state['values']['question_txt']);
  $form_state['values']['answers_list'] = stripslashes($form_state['values']['answers_list']);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $questionId =  addNewSurveyQuestion($form,$form_state,$createuserid,1);
  expDebug::dPrint('$questionId = ' . print_r($questionId, true), 4);
  return $questionId;
}

/*
 * updateSurveyQuestionByRestAPI() -
 */
function updateSurveyQuestionByRestAPI(&$form_state,$createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		$form_state['values']['question_object_type'] = 'sry_det_typ_sry';
		// 46691- Clear backslash added during cleanup
		$form_state['values']['question_txt'] = stripslashes($form_state['values']['question_txt']);
		$form_state['values']['answers_list'] = stripslashes($form_state['values']['answers_list']);
		$form_state = stripslashes_deep($form_state); // Strip slashes Function
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$questionId =  updateSurveyQuestion($form,$form_state,$createuserid,1);
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		expDebug::dPrint('$questionId = ' . print_r($questionId, true), 4);
		return $questionId;
	}
	catch (Exception $ex) {
		watchdog_exception('updateSurveyQuestionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * addNewAssessmentQuestionByRestAPI()
 */
function addNewAssessmentQuestionByRestAPI(&$form_state,$createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		$form_state['values']['question_object_type'] = 'sry_det_typ_ass';
		// 46691- Clear backslash added during cleanup
		$form_state['values']['question_txt'] = stripslashes($form_state['values']['question_txt']);
		$form_state['values']['correct_answer'] = stripslashes($form_state['values']['correct_answer']);
		$form_state['values']['answers_list'] = stripslashes($form_state['values']['answers_list']);
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		updateAssessmentQuestionFormValidation($form_state); // add validation for given input values. update and insert both have same functions.
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		$questionId =  addNewSurveyQuestion($form,$form_state,$createuserid,1);
		expDebug::dPrint('$questionId = ' . print_r($questionId, true), 4);
		return $questionId;
	}
	catch (Exception $ex) {
		watchdog_exception('addNewAssessmentQuestionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * updateAssessmentQuestionByRestAPI()
 */
function updateAssessmentQuestionByRestAPI(&$form_state,$createuserid=""){
	try {
		$form=array();
		$form["form_id"]="";
		$form_state['values']['question_object_type'] = 'sry_det_typ_ass';
		// 46691- Clear backslash added during cleanup
		$form_state['values']['question_txt'] = stripslashes($form_state['values']['question_txt']);
		$form_state['values']['correct_answer'] = stripslashes($form_state['values']['correct_answer']);
		$form_state['values']['answers_list'] = stripslashes($form_state['values']['answers_list']);
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		updateAssessmentQuestionFormValidation($form_state);
		$validate = form_get_errors();
		if(count($validate)){
			$error -> isValidateError = true;
			$error -> errormsg = implode(',' ,$validate);
			return $error;
		}
		$questionId =  updateSurveyQuestion($form,$form_state,$createuserid,1);
		expDebug::dPrint('$questionId = ' . print_r($questionId, true), 4);
		return $questionId;
	}
	catch (Exception $ex) {
		watchdog_exception('updateAssessmentQuestionByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function updateAssessmentQuestionFormValidation(&$form_state) {
	try {
		expDebug::dPrint('updateAssessmentQuestionFormValidation given form = ' . print_r($form_state, true), 4);
		// updatation part previous question type check
		$question_type ='';
		$previous_assessment_det = '';
		$question_type = trim($form_state['values']['question_type']);
		if(isset($form_state['values']['id']) && !empty($form_state['values']['id'])) {
			$assessment_id = $form_state['values']['id'];
			$previous_assessment_det = getSurveyQuestionDetails($assessment_id);
			if(empty($previous_assessment_det)) {
				form_set_error('',t('Invalid Id entered'));
			}
			if($question_type  != trim($previous_assessment_det->question_type)) {
				form_set_error('',t('Invalid question_type.'));
			}     // yes or no type and true or false type => answer list must be empty.
		}

		$default_question_type = array('sry_qtn_typ_yno','sry_qtn_typ_dpn','sry_qtn_typ_mch',
				'sry_qtn_typ_trf','sry_qtn_typ_cmt','sry_qtn_typ_rtg');
		if(!in_array($question_type, $default_question_type)) {
			form_set_error('',t('Invalid question_type entered.'));
		}
		$atleast_number = isset($form_state['values']['atleast_number']) ? trim($form_state['values']['atleast_number']):'';
		//$to = isset($form_state['values']['to']) ? trim($form_state['values']['to']):''
		if($question_type == 'sry_qtn_typ_mch') {
			$ans_criteria = getProfileItemNamesKeyed('sry_ans_typ_');
			//$ans_criteria = array('sry_ans_typ_all','sry_ans_typ_atl','sry_ans_typ_arg','sry_ans_typ_xct');
			$answer_list_choice_array = explode('@@',$form_state['values']['answers_list']);
			$dupCount = call_user_func_array('checkOptionDuplicates',array($answer_list_choice_array));
			if($dupCount > 0) {
				form_set_error('', 'Duplicate answers are not allowed in answers_list');
				$form_state['esign_show_popup']= false;
			}			
			$correct_answer = explode('@@', trim($form_state['values']['correct_answer']));
				if(empty($form_state['values']['answer_logic'])) {
					form_set_error('',t('Enter answer field'));
				}
				if(!in_array($form_state['values']['answer_logic'],array_keys($ans_criteria))) {
					form_set_error('',t('Invalid answer_logic entered'));
				}
				if($form_state['values']['answer_logic'] == 'sry_ans_typ_atl') {
					answerCriteriaFromToValidation('atleast_number',$atleast_number,$answer_list_choice_array);
				} 
			/*	if($form_state['values']['answer'] == 'sry_ans_typ_ran') {
					answerCriteriaFromToValidation('to',$to,$answer_list_choice_array);
					if($from > $to) {
						form_set_error('',t('The value in “To” should be greater than “From”'));
					}
				} */
			} 

		
		$answer_list = trim($form_state['values']['answers_list']);
		$yesornotype = array('sry_qtn_typ_yno','sry_qtn_typ_trf');
		if(in_array($question_type, $yesornotype)) {
			if(!empty($form_state['values']['answers_list'])) {
				$form_state['values']['answers_list'] ='';
			}
			if($question_type == 'sry_qtn_typ_trf') {
				if(!in_array(trim($form_state['values']['correct_answer']),array('True','False'))) {
					form_set_error('',t('correct_answer must be in True or False'));
				}
			} else {
				if(!in_array(trim($form_state['values']['correct_answer']),array('Yes','No'))) {
					form_set_error('',t('correct_answer must be in Yes or NO'));
				}
			}

		} else {
			//answer choice checking for dropdown and multiple choice
			$def_dropdwon =  array ('sry_qtn_typ_dpn', 'sry_qtn_typ_mch');
			if(in_array($question_type, $def_dropdwon)) {
				$answer_list_choice_array = explode('@@', $form_state['values']['answers_list']);
				if($question_type == 'sry_qtn_typ_mch'){
    				$correct_answer = explode('@@',trim($form_state['values']['correct_answer']));
    				foreach($correct_answer as $val){
        				if(!in_array($val,$answer_list_choice_array)) {
        					form_set_error('',t('Entered Correct_answer not in answers_list'));
        				}
    				}
    				if(count($correct_answer) < $atleast_number) {
    					form_set_error('',t('atleast_number must be lesser than total number of correct answers'));
    				}
				}else{
				  if(!in_array(trim($form_state['values']['correct_answer']),$answer_list_choice_array)) {
				    form_set_error('',t('Entered Correct_answer not in answers_list'));
				  }
				}
			}
		}


	}catch (Exception $ex) {
		watchdog_exception('updateAssessmentQuestionFormValidation', $ex);
		expertusErrorThrow($ex);
	}
}
function answerCriteriaFromToValidation($field,$field_value,$choice_list) {
	if(empty($field_value)) {
		form_set_error('',$field.' is required');
	}
	if(!is_numeric($field_value) || $field_value < 0){
		form_set_error('',t('Enter only numeric values in atleast_number'));
	}
	if(strpos($field_value,'.') == true){
		form_set_error('',t(''.$field.' cannot have decimal value.'));
	}
}
function checkOptionDuplicates($choiceOptionLabelsList){
try {
	$duplicates = array();
	$counter = 0;
	for($m = 0; $m < count($choiceOptionLabelsList); $m++){
		if (in_array(trim($choiceOptionLabelsList[$m]), $duplicates)) {
			++$counter;
			continue;
		}
		else{
			$duplicates[] = trim($choiceOptionLabelsList[$m]);
		}
	}
	return $counter;
}catch (Exception $ex) {
		watchdog_exception('checkOptionDuplicates', $ex);
		expertusErrorThrow($ex);
	}
}
?>