<?php
function addCustomAttributeOptionsBlock(&$form,  $defaultValue,$readOnly) {
    try {
        
        expDebug::dPrint(' $form  : '.print_r($form,1),5);
        expDebug::dPrint(' $defaultValue - '.print_r($defaultValue,1),5); 
        
        // Add the answerchoices block based on question type
        $form['customoptions']['prefix'] = array (
                '#type'   => 'markup',
                '#markup' => '<div id="answer-choices-block" class="answer-choices-block">'
        ); 

        $options = $defaultValue['customoptions']['options'];
        expDebug::dPrint(' customoptions Before >>'.print_r($options,1),5);
        
        if (empty($options)) {
            $options = array(1 => '');
        }
        expDebug::dPrint(' customoptions After >>'.print_r($options,1),5);
        
        
        $attributes['tabindex'] = 4;
   
        $attributes['readOnly']=$readOnly;        
        $form['customoptions']['choices'] = array (
                //'#type' => 'survey_question_fields',
                '#type' => 'customattribute_options',
                '#custom_question_options' => $options, // Cannot use #options as it triggers validation in
                // form.inc:_form_validate() assuming a specific implementation.
                //'#attributes' => array('tabindex' => 4),
        		'#attributes' => $attributes,
        		
        );
        
        $form['customoptions']['suffix'] = array (
                '#type'   => 'markup',
                '#markup' =>   '<div style="clear:both"></div>' .
                '</div>'
        ); 
        
        // Save count of options in a value field
        $form['customoptions']['count'] = array (
                '#type' => 'value',
                '#value' => count($options)
        );
        }
        catch (Exception $ex) {
            watchdog_exception('addCustomAttributeOptionsBlock', $ex);
            expertusErrorThrow($ex);
        }
}

// repaintcustomAttributeOptionsForm() - Repaint the Custom Attribute Option Form 
function repaintcustomAttributeOptionsForm($form, $form_state) {
    try {             
        expDebug::dPrint('repaintcustomAttributeOptionsForm called', 4);
        expDebug::dPrint(' $form  : '.print_r($form,1),5);
        expDebug::dPrint(' $form_state  : '.print_r($form_state,1),5); 
    
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
        watchdog_exception('repaintcustomAttributeOptionsForm', $ex);
        expertusErrorThrow($ex);
    }
}

// process_customattribute_options() - To process custom attribute options
function process_customattribute_options($element) { 
    try {
        expDebug::dPrint('$element = ' . print_r($element, true), 5);
        $value = is_array($element['#value']) ? $element['#value'] : array();
        $element['#tree'] = TRUE;

        $weight = 0;
        $tabIndex =  empty($element['#attributes']['tabindex'])? 0 : $element['#attributes']['tabindex'];
        $readOnly = empty($element['#attributes']['readOnly'])? 0 : $element['#attributes']['readOnly'];
        
        foreach ($element['#custom_question_options'] as $key => $choice) {
            expDebug::dPrint('$element custom_question_options  = ' . print_r($element['#custom_question_options'], true), 5);
            expDebug::dPrint('$element key  = ' . print_r($key, true), 5);
            expDebug::dPrint('$element choice  = ' . print_r($choice, true), 5);
                
                 
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
            $choiceTextareaAttributes['class'][] = 'addedit-edit-customattributetextfield';
            $choiceTextareaAttributes['class'][] = 'custom-answer-choice-customattributetextfield';
           
            if ($tabIndex > 0) {
                $choiceTextareaAttributes['tabindex'] = $tabIndex;
                $tabIndex++;
            }
            $tmp_readonly=0;
 
            if ($readOnly > 0) {
            	 if($choice!=''){   
	            		$tmp_readonly=1;
	            		$choiceTextareaAttributes['readonly']='readonly';
	            		$choiceDeleteAttributes['readonly'] = 'readonly';
	            		$choiceTextareaAttributes['class'][] = 'addedit-readonly-textfield';  
	            	//form_error($element,$element['#title'].' '.t('ERR351'));
	            }
            }
           
            $element['choice-text-' . $key] += array(
                    '#type' => 'textfield',
                    '#default_value' => isset($value['choice-text-' . $key]) ? $value['choice-text-' . $key] : '',
                    '#maxlength' => '25', 
                    '#size' => '32', 
                    '#weight' => $weight,
                    '#attributes' => $choiceTextareaAttributes,
            		//'#disabled' => $tmp_readonly,
            );

            $weight += 0.001;
            $element += array('choice-delete-' . $key => array());
            $choiceDeleteAttributes = array();
            $choiceDeleteAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $choiceDeleteAttributes['data-wrapperid'] = array('customattribute-basic-addedit-form');
           /* if ($readOnly) {
            	 $choiceDeleteAttributes['class'][] = 'addedit-readonly-dropdown';
            }*/
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
                    '#prefix' =>'<span class="customattribute-ques-delete ">',
                    '#suffix' => '</span>',
                    '#weight'     => $weight,
                    '#validate'   => array('customattribute_delete_choice'),
            		'#disabled' => $tmp_readonly,
                    '#ajax'       => array(
                            'wrapper'         => 'customattribute-basic-addedit-form',
                            'callback'        => 'repaintcustomAttributeOptionsForm',
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
        $choiceAddAttributes['data-wrapperid'] = array('customattribute-basic-addedit-form');
        if ($tabIndex > 0) {
            $choiceAddAttributes['tabindex'] = $tabIndex;
            $tabIndex++;
        }
       
       
        $element['choice-add'] += array(
                '#type'       => 'submit',
                '#name'       => 'choice-add',
                '#value'      => t('LBL2012'),  // IMPT: #value needed to paint the button label.
        		//'#disabled' => $readOnly,
                '#attributes' => $choiceAddAttributes,
                '#weight'     => $weight,
                '#validate'   => array('customattribute_new_choice'),
                '#ajax'       => array(
                        'wrapper'         => 'customattribute-basic-addedit-form',
                        'callback'        => 'repaintcustomAttributeOptionsForm',
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
        watchdog_exception('process_customattribute_options', $ex);
        expertusErrorThrow($ex);
    }
}
function customattribute_delete_choice($form, &$form_state) {
    try {
        expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
        expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

        $deleteChoiceId = $form_state['triggering_element']['#choice-id'];
        $newChoices = array(); // initialize
        $j = 1; // index for the updated choices list
        for ($i = 1; $i <= $form_state['values']['customoptions']['count']; $i++) {
            if ($i != $deleteChoiceId) {
                $newChoices[$j] = $form_state['values']['customoptions']['choices']['choice-text-' . $i];
                $j++;
            }
        } // end for loop

        $form_state['storage']['customoptions']['count'] = count($newChoices);
        $form_state['storage']['customoptions']['options'] = $newChoices;

        unset($form_state['values']['customoptions']);
        unset($form_state['input']['customoptions']);

        expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
        $form_state['rebuild'] = TRUE;
    }
    catch (Exception $ex) {
        watchdog_exception('customattribute_delete_choice', $ex);
        expertusErrorThrow($ex);
    }
}
function customattribute_new_choice($form, &$form_state) {
    try {
        expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state[triggering_element], true), 5);
        expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);

        $newCorrectAnswers = array(); // initialize
        $newChoices = array(); // initialize
        for ($i = 1; $i <= $form_state['values']['customoptions']['count']; $i++) {
            $newChoices[$i] = $form_state['values']['customoptions']['choices']['choice-text-' . $i];
        } // end for loop
        $newChoices[$i] = ''; // Add new blank choice

        $form_state['storage']['customoptions']['count'] = count($newChoices);
        $form_state['storage']['customoptions']['options'] = $newChoices;

        unset($form_state['values']['customoptions']);
        unset($form_state['input']['customoptions']);

        expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 4);
        $form_state['rebuild'] = TRUE;
    }
    catch (Exception $ex) {
        watchdog_exception('customattribute_new_choice', $ex);
        expertusErrorThrow($ex);
    }
}
 
function theme_customattribute_options($variables) {
    try {
             
        expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
        
        $mandatorySymbol = "<span class='addedit-mandatory'>*</span>";
         
        $element = $variables['element'];

        $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field">' .
                '<div class="addedit-new-field-title">' .
                t('LBL3097').':'.$mandatorySymbol.
                '</div>' .
                '<div class="addedit-new-field-value">&nbsp;</div>' .
                '<div class="clearBoth"></div>' .
                '</div>';


         $element['#children'] .='<div id="custom_attr_option_block">';
        // Prepare html for each option
        $i = 1;
        foreach ($element['#custom_question_options'] as $key => $value) {
            $element['#children'] .=  '<div class="addedit-new-field custom_attr_option_row">' . // Each checkbox is a new row
                    '<table>' .
                    '<tr>' .
                    '<td class="div-align-top">' .
                    '<div class="addedit-new-field-title-choice">' .
                    t(LBL387) . ' ' . $i . ':' .$mandatorySymbol.
                    '</div>' .
                    '</td>' .
                    '<td>' .
                    '<div class="custom-question-choice-text" id="custom-question-choice-text-'. $i.'">' .
                    drupal_render($element['choice-text-' . $key]) .
                    '</div>' . // choice textfield - floats to the left
                    '</td>' .
                    '<td><div class="custom-question-choice-delete-button">' ;// delete button container
                        //if($i!='1'){ 
                            $element['#children'] .= drupal_render($element['choice-delete-' . $key]); 
                        //}else{
                            //$element['#children'].='<div id=\'custom-question-choice-first-delete-button\'><div>';
                        //}
                    
                    $element['#children'] .='</div></td>' .
                    '</tr>' .
                    '</table>' .
                    '<div style="clear:both"></div>' .
                    '</div>';
            $i++;
        } // end foreach
        
        $element['#children'] .='</div>'; //Block End
        
        $j = $i-1;
        //if($element['#attributes']['readOnly']!=1)
        //{
	       if($j<10)// for tenth one options will not come
	       {
	        $element['#children'] .= '<div class="addedit-new-field  addedit-custom-field custom_attr_option_row">' . // Add choice button row
	                '<div class="addedit-new-field-title">&nbsp;</div>' .
	                '<div class="addedit-new-field-value">' .
	                '<div class="addedit-form-cancel-container-actions">' .
	                drupal_render($element['choice-add']) .
	                '</div>' .
	                '</div>' .
	                '<div class="clearBoth"></div>' .
	                '</div>';
	        }
        //}else{
        	//if($j<10){
        	 // $element['#children'] .= '<div class="" style="padding-bottom:5px;"></div>';
        	//}   
        //}
        
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
        watchdog_exception('theme_customattribute_options', $ex);
        expertusErrorThrow($ex);
    }
}

function customattribute_options_element_validate($element, &$form_state) {
    try {
            /*
        expDebug::dPrint('$element = ' . print_r($element, true), 5);
        expDebug::dPrint('$element[#value] = ' . print_r($element['#value'], true), 4);
        foreach ($element['#value'] as $key => $value) {
            if (substr($key, 0, strlen('choice-text-')) === 'choice-text-') {
                $idx = substr($key, strlen('choice-text-')) ;
            }

            if (empty($idx) || !isset($element['#custom_question_options'][$idx])) {
                form_error($element, t('An illegal choice has been detected. Please contact the site administrator.'));
                watchdog('form', 'Illegal choice %choice for text field or correct answer selection detected in !name element.',
                array('%choice' => empty($idx)? 0 : $idx,
                '!name' => empty($element['#title'])? $element['#parents'][0] : $element['#title']), WATCHDOG_ERROR);
            }
            
            if(!preg_match('/^[A-Za-z0-9_ .?@#&-+$><\/%*]+$/', $value)){
               form_error($element, empty($element['#title']) ? $element['#parents'][0] : $element['#title'].' '.t('ERR351'));
            } 
        }*/
    }
    catch (Exception $ex) {
        watchdog_exception('customattribute_options_element_validate', $ex);
        expertusErrorThrow($ex);
    }
}
  
function getDefaultCustomAttributeOptions(&$form_state, $customAttributeDefaultValue,$customAttributeDetails) {   
    try {
        
          expDebug::dPrint('$form_state= ' . print_r($form_state, true), 5);
          expDebug::dPrint('$customAttributeDefaultValue= ' . print_r($customAttributeDefaultValue, true), 5);
          expDebug::dPrint('$customAttributeDetails = ' . print_r($customAttributeDetails, true), 5);
          expDebug::dPrint('values - customoptions  = ' . print_r($form_state['values']['customoptions'], true), 5);
          expDebug::dPrint('storage - customoptions = ' . print_r($form_state['storage']['customoptions'], true), 5);
        
        if(isset($form_state['values']['customoptions'])) {
            expDebug::dPrint('Building answerchoices from $form_state[values]', 4);
           // $form_state['storage']['customoptions'] = $customAttributeDefaultValue['customoptions'];
            $customAttributeDefaultValue['customoptions'] = prepareDefaultCustomAttributeOptionsFromFormState($form_state);
        }
        else if (isset($form_state['storage']['customoptions'])) {
            expDebug::dPrint('Building answerchoices from storage', 4);
            expDebug::dPrint('Building answerchoices from storage >>>>'.print_r($form_state['storage']['customoptions'],1),4);
            
            $customAttributeDefaultValue['customoptions'] = $form_state['storage']['customoptions'];
            
            unset($form_state['storage']['customoptions']); // Remove $form_state['storage']['customoptions'] once used
        }
        else if (!empty($customAttributeDetails)) {
            expDebug::dPrint('Building answerchoices from DB data', 4);
            $customAttributeDefaultValue['customoptions'] = prepareDefaultCustomAttributeOptionsFromDB($customAttributeDetails->cattr_id);  
        }
        expDebug::dPrint('$customAttributeDefaultValue='.print_r($customAttributeDefaultValue,true),5);
       
        return $customAttributeDefaultValue;
       
    }catch (Exception $ex) {
        watchdog_exception('getDefaultCustomAttributeOptions', $ex);
        expertusErrorThrow($ex);
    }
}

function customattribute_options_value_callback($element, $input = FALSE) {
    try {
        expDebug::dPrint('$element custom= ' . print_r($element, true), 5);
        expDebug::dPrint('$input custom= ' . print_r($input, true), 5);
        $elementValue = array();
        if ($input === FALSE) {
            $element += array('#custom_question_options' => array()); // If not present, add.
            foreach ($element['#custom_question_options'] as $key => $value) {
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
        watchdog_exception('customattribute_options_value_callback', $ex);
        expertusErrorThrow($ex);
    }
}
 
function prepareDefaultCustomAttributeOptionsFromFormState($form_state) {
    try {
            $answerChoices = customoptionsResetValues();
            for ($i = 1; $i <= $form_state['values']['customoptions']['count']; $i++) {
                $answerChoices['options'][$i] = $form_state['values']['customoptions']['choices']['choice-text-' . $i];
            }
            $answerChoices['count'] = $form_state['values']['customoptions']['count'];
           return $answerChoices;
    }catch (Exception $ex) {
            watchdog_exception('prepareDefaultCustomAttributeOptionsFromFormState', $ex);
            expertusErrorThrow($ex);
    }
}

function customoptionsResetValues(){
    try {
        
        $answerChoicesList  =  array();
        $answerChoices = array();
        $answerChoices['options'] = $answerChoicesList;
        $answerChoices['count'] = count($answerChoicesList);
        expDebug::dPrint('$answerChoices = '. print_r($answerChoices, true), 4);
        return $answerChoices;

    }catch (Exception $ex) {
        watchdog_exception('customoptionsResetValues', $ex);
        expertusErrorThrow($ex);
    }  
}

function prepareDefaultCustomAttributeOptionsFromDB($cus_attr_id){
    try {
            $answerChoices = customoptionsResetValues(); 
            
            $option_results_arr=getCustomAttributeOptionDetail($cus_attr_id);
            
            expDebug::dPrint('$option_results_arr'.print_r($option_results_arr,true),5);
            
            if(count($option_results_arr)>0){
                $opt_arr=array();
                foreach($option_results_arr as $opt_key => $opt_obj){
                    $opt_arr[]=$opt_obj->opt_name;
                } 
                
                expDebug::dPrint("ttttt::");
                //Dynamic translation - start
              /*  $config=getConfig('exp_sp');
                if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
                {
                    include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
                    global $user;
                    expDebug::dPrint("ttttt before::".print_r($opt_arr,true),3);
                    $translatedLabels = getTranslationsForArrayStrings($opt_arr,$user->language,"","custom_attribute_options");
                    expDebug::dPrint("ttttt after::".print_r($translatedLabels,true),3);
                    $opt_arr_translated = array();
                    foreach($option_results_arr as $opt_key => $opt_obj){
                        $opt_arr_translated[]=$translatedLabels[$opt_obj->opt_name];
                    } 
                }
                else*/
                    $opt_arr_translated = $opt_arr;
                
                $answerChoices['options'] = $opt_arr_translated;
                $answerChoices['options'] = array_combine(range(1, count($answerChoices['options'])), $answerChoices['options']); 
                $answerChoices['count'] = count($answerChoices['options']);
                
            }else{
                expDebug::dPrint('No options are available in DB', 5);
            }

            return $answerChoices;
            
            /*if (!empty($questionDetails->answer_choice_txt)) {
                $answerChoices['options'] = explode("##", $questionDetails->answer_choice_txt);
                // Choices index always start from 1 instead of 0
                $answerChoices['options'] = array_combine(range(1, count($answerChoices['options'])), $answerChoices['options']);
            }
            $answerChoices['count'] = count($answerChoices['options']);
            if ($answerChoices['count'] > 0) {              
                        if (empty($questionDetails->right_answer)) {
                            $rightAnswer = NULL;
                        }
                        else {
                            $rightAnswer = array_search($questionDetails->right_answer, $answerChoices['options']);
                            if ($rightAnswer === FALSE) {
                                $rightAnswer = NULL;
                            }
                        }
                        
                        $answerChoices['rightanswer'] = $rightAnswer;
                            
            expDebug::dPrint('returning $answerChoices = ' . print_r($answerChoices, true), 4);
          return $answerChoices;
           }*/
    }
    catch (Exception $ex) {
            watchdog_exception('prepareDefaultCustomAttributeOptionsFromDB', $ex);
            expertusErrorThrow($ex);
    }
} 

?>