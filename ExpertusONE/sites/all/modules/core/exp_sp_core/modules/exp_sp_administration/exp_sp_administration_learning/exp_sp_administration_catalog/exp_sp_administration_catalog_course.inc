<?php
/**
 * Catalog Admin Course inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 06-Feb-2012
 */

/**
 * exp_sp_administration_catalog_course_addedit_form() - Build the addedit form for Course.
 */
function exp_sp_administration_catalog_course_addedit_form($form, &$form_state, $renderMode, $courseId, $classId){
    try {
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_prerequisite.inc');
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_equivalence.inc');
        if (empty($form_state['values'])) {
            $currentUserId = getIdOfLoggedInUser();
            $num_deleted = db_delete('slt_entity_reading_mapper')->condition('entity_id', '0')->condition('entity_type', 'cre_sys_obt_crs')->condition('created_by', $currentUserId)->execute();
        }
        $showCancelSave = 1;
        // To set the display mode as view
        $readOnlyForView = false;
        if(in_array('view', $form_state['build_info']['args'])){
            $form_state['storage']['step']         = 'view';
            $form_state['storage']['display_mode'] = 'view';
            $readOnlyForView = true;
        }

        if (empty($form_state['storage'])) {
            if(!empty($classId) && empty($form_state['storage'])) {
                $form_state['storage']['step'] = 'view';
            } elseif(empty($courseId) && empty($classId) && empty($form_state['storage'])) {
                $form_state['storage']['step'] = 'add';
            }
        }

        $CourseDetails     = array();
        $enroll_compliance = 0;
        if (!empty($courseId)) {
            $CourseDetails   = getCourseDetail($courseId, array('id', 'title', 'code', 'lang_code','short_description', 'description','status','price','currency_type','currency_symbol','author_vendor','is_compliance','complete_days','complete_date','validity_days','validity_date', 'compliance_completed')); // Changed by Gayathri for 0070892
            $enroll_compliance = getEnrolledComplianceCourse($courseId);
        }
        $form              = newAddEditFormWithCancelAndSave('catalog-course-basic-addedit-form');
        $form['#attributes']['class'][] = 'catalog_course_addedit_form_common';
        $fieldList         = array('crs_id', 'crs_title', 'crs_code', 'crs_language', 'crs_short_description', 'crs_description','crs_language_name','crs_status_name','crs_price','crs_currency_type','crs_currency_symbol','crs_author_vendor','is_compliance','complete_days','completed_hire_days','complete_date','validity_days','validity_date', 'compliance_completed'); // Changed by Gayathri for 0070892
        $defaultValue      = getDefaultCrsAddEditFormFieldValue($fieldList, $form_state, $CourseDetails[0]);
        $emptyId = '';
        $entityId = 0;
        if(empty($courseId)){
            $entityId       = $form_state['values']['id'];
            $emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
        } else {
            $entityId       = $courseId;
        }
        $form['mapped_course_id'] = array(
      '#type' => 'hidden',
      '#name' => 'mapped_course_id',
      '#value' => $entityId
        );
        $entityType        = 'cre_sys_obt_crs';
        $uniqueId          = $entityId.'-'.$entityType;

        addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
        addHiddenFieldToAddEditForm($form, 'entity_value', $entityType);
        $form['course_detail_disp'] = array(
      '#type'       => 'markup',
      '#prefix'     => '<div id="catalog-course-basic-addedit-form-disp">',
        //'#suffix'     => '</div>',
        //'#attributes' => $editAttributes,
        );

        list_forms($form, $form_state, $defaultValue);
        $form['course_detail_wrapper'] = array(
      '#type'       => 'markup',
        //'#prefix'     => '<div class="catalog-course-basic-addedit-form-container" id="catalog-course-basic-addedit-form-container">',
      '#prefix'     => '<div class="addedit-form-main-container">
                        <div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
                        <div class="catalog-course-basic-addedit-form-container" id="catalog-course-basic-addedit-form-container">',
        );

        if($form_state['storage']['step'] == 'view') {
            edit_course_details($form, $form_state, $defaultValue);
            if($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep'])) {
                edit_course_view($form, $form_state, $defaultValue,$entityId);
                $form_state['storage']['substep'] = 'details';
            }
            elseif($form_state['storage']['substep'] == 'Attachment') {
                attachmentDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            elseif($form_state['storage']['substep'] == 'tag') {
                tagDetailsView($form, $form_state, $entityId, 'Course', $uniqueId);
            }
            if($form_state['storage']['substep'] == 'prerequisite') {
                // $showCancelSave = 0;
                $form['Prerequisite-Details-view-start'] = array(
          '#type'   => 'markup',
          '#markup' => '<div>',
                );
                displayPrerequisiteDetails($form, $form_state, $entityId, $entityType, $uniqueId, 'complete_view');
            }

            if($form_state['storage']['substep'] == 'equivalence') {
                $showCancelSave = 0;
                $form['Equivalence-Details-view-start'] = array(
           '#type'   => 'markup',
           '#markup' => '<div>',
                );
                displayEquivalenceDetails($form, $form_state, $entityId, $entityType, $uniqueId, 'complete_view');
            }

            /*
             elseif($form_state['storage']['substep'] == 'prerequisite') {
             preRequisiteDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
             }

             elseif($form_state['storage']['substep'] == 'survey') {
             // To display the survey UI
             surveyDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
             }
             elseif($form_state['storage']['substep'] == 'assessment') {
             assessmentDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
             }
             */
            elseif($form_state['storage']['substep'] == 'custom' && (!module_exists('exp_sp_administration_customattribute'))) { //#custom_attribute_0078975
                customFieldsDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            elseif($form_state['storage']['substep'] == 'certificate') {
                displayCertificateToAddEditForm($form, $form_state, $entityId, $entityType, $uniqueId, true);
            }

            // To display the survey UI
            //displaySurveyDetails($form, $form_state, $entityId, $entityType, $uniqueId);

            // To display the assessment UI
            //displayAssessmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);

            unset($form['addedit_tmp_storage']);
            $form['addedit-left-col-end1'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
            );

        } else {
            if($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep'])) {
                addHiddenValueToAddEditForm($form, 'crs_id', $defaultValue['crs_id']);
                $form['addedit-remove-space-title-start'] = array(
          '#type'   => 'markup',
          '#markup' => '<div style="width:100%;">',
                );
                addTextFieldToAddEditForm($form, 'crs_title', t('LBL083'), $defaultValue['crs_title'], 1, true,false,300);
                $form['addedit-remove-space-title-end'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
                );

                addTwoColumnRowToAddEditForm($form, 'lang_status');
                addTextFieldToAddEditForm($form, 'crs_code', t('LBL096'), $defaultValue['crs_code'], 2, true,false,100);

                $defaultValue['crs_language'] = !empty($defaultValue['crs_language']) ? $defaultValue['crs_language'] : 'cre_sys_lng_eng';
                addDropDownToAddEditForm($form, 'crs_language', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue['crs_language'], 3, true);

                endAddEditTwoColumnRow($form);

                // addTextAreaFieldForShortDescription($form, 'crs_short_description', t('LBL097'), $defaultValue['crs_short_description'], 4, true);

                //addTextAreaFieldToAddEditForm($form, 'crs_description', t('LBL098'), $defaultValue['crs_description'], 4);
                addHiddenFieldToAddEditForm($form, 'ckeditor_word_count', '5000');
                $form['addedit-long-description-container'] = array(
          '#type'   => 'markup',
          '#markup' => '<div class="addedit-long-description-container">',
                );
                addTextFormatFieldToAddEditForm($form, 'crs_short_description', t('LBL229'), $defaultValue['crs_short_description'], 'full_html', 4, true, false, 5000);
                $form['addedit-long-description-container1'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
                );


                /*addTwoColumnRowToAddEditForm($form, 'lang_status');

                $defaultValue['crs_language'] = !empty($defaultValue['crs_language']) ? $defaultValue['crs_language'] : 'cre_sys_lng_eng';
                addDropDownToAddEditForm($form, 'crs_language', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue['crs_language'], 9, true);
                */
                /*$statusUniqueCode = 'Course';
        $attributes['onchange'][] = 'getStatusReason(this,\''.$statusUniqueCode.'\');';
        $defaultValue['crs_status'] = !empty($defaultValue['crs_status']) ? $defaultValue['crs_status'] : 'lrn_crs_sts_atv';
        addDropDownToAddEditForm($form, 'crs_status', t('LBL102'), getProfileItemNamesKeyed('lrn_crs_sts_'), $defaultValue['crs_status'], 9, true, false,$attributes);
        */
                $defaultValue['crs_status'] = !empty($defaultValue['crs_status']) ? $defaultValue['crs_status'] : 'lrn_crs_sts_itv';
                $form['crs_status_hid'] = array(
          '#type' => 'hidden',
          '#name' => 'crs_status_hid',
          '#value' => $defaultValue['crs_status'],
                );
                //endAddEditTwoColumnRow($form);

                /* -------Inactive Reason text Area Start----------*/
                /* if($defaultValue['crs_status'] == 'lrn_crs_sts_itv') {
                 $displayPage = "display:block";
                 }else{
                 $displayPage = "display:none";
                 }  */

                /*        $form['admin_status_inactive_disp_start'] = array(
                 '#type' => 'markup',
                 '#markup' => '<div id="admin-status-inactive-part-'.$statusUniqueCode.'" style="'.$displayPage.'">'
                 );

                 addTextAreaFieldToAddEditForm($form, 'status_inactive_reason', t('Inactive Reason'), $defaultValue['status_inactive_reason'], 8, false);

                 $form['admin_status_inactive_disp_end'] = array(
                 '#type' => 'markup',
                 '#markup' => '</div>'
                 );*/
                /* -------Inactive Reason text Area End----------*/
                // Commented by Gayathri for 0070892
                //addTwoColumnRowToAddEditForm($form, 'author_vendor');

                //addTextFieldToAddEditForm($form, 'crs_author_vendor', t('LBL269'), $defaultValue['crs_author_vendor'], 5, false, false, 50);

          //Using the below veriable we can find current loaded theme name
          global $theme_key;

          if($theme_key == 'expertusoneV2') {
              $newuiPrefix = '<div class="expertus-dropdown-bg crs-compliance-dropdown"><div class="expertus-dropdown-icon">';
              $newuiSuffix = '</div></div>';
          } else {
              $newuiPrefix = '';
              $newuiSuffix = '';
          }
        // Added by Gayathri for 0070892 - start
        addTwoColumnRowToAddEditForm($form, 'course_price');

        $default = $defaultValue['crs_price']; 
        if(strlen($defaultValue) >= 11){
            $defaultValue=(int)$defaultValue;
        }
        $priceAttributes = array();
        $priceAttributes['class'][] = 'addedit-edit-price';
        $priceAttributes['class'][] = 'addedit-edit-textfield';
        $priceAttributes['tabindex'] = 108;
        //Added by Gayathri for 0073578
        $priceAttributes['onkeydown'] = 'return onlyNumbersforPrice(event)';
        
        if(strlen($default) >= 11){
            $default=(int)$default;
        }
        
        $form['price'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="addedit-twocol-firstcol"><div class="addedit-new-field-class-fee class-fee-textfield-label">',
        '#suffix' => '</div>',
        //'#required' => TRUE,
        '#size' => 4,
        '#title' => t('LBL040').':',
        '#default_value' => $default,
        '#attributes' => $priceAttributes,
        '#maxlength'=> 10, // added This Max lenght For this Ticket #0036013
        );

        $default = !empty($defaultValue['crs_currency_type']) ? $defaultValue['crs_currency_type'] : 'USD';     
        $finalOptions = getProfileItemNamesKeyed('cre_sys_crn_');
          $form['currency_type'] = array(
          '#type' => 'select',
          '#title' => t('LBL101').' '.t('LBL036'),
          '#prefix' => '<div class="addedit-new-field-currency-type">'.$newuiPrefix,
          '#suffix' => $newuiSuffix.'</div></div>',
          '#title_display' => 'invisible',
          '#options'=> $finalOptions,
          '#default_value' => $default,
          //'#disabled' => $readOnly,
          '#attributes' => array('tabindex' => '109','Class' => 'addedit-new-field-currency-type-select'),
        );
        // 0070892 - End

          $dispWrapperId                   = 'catalog-course-basic-addedit-form';
          $options                         = array('0' => t('No'), '1' => t('Yes'));
          $attributes                      = array();
          $attributes['class']             = array('addedit-edit-compliance_course');
          $attributes['class'][]           = 'addedit-form-expertusone-throbber';
          $attributes['data-wrapperid']    = array($dispWrapperId);
          $attributes['tabindex']          = 6;
          $finalOptions                    = $options;
          $defaultOption                   = !empty($defaultValue['is_compliance']) ? $defaultValue['is_compliance'] : 0;
          $readOnlyCompliance              = (($enroll_compliance>0) ? TRUE : FALSE);

          $form['addedit_tmp_storage']['two_column']['current_column'] = 2;
          $form['course_compliance'] = array(
          '#type'        => 'select',
          '#name'        => 'course_compliance',
          '#id'          => 'course_compliance',
          '#prefix'        => addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', t('Compliance'), FALSE) . $newuiPrefix,
          '#suffix'        => $newuiSuffix . addEditGetSuffix('TWOCOL_SECONDCOL_FIELD'),
          '#title'         => t('LBL036'),
          '#title_display' => 'invisible',
          '#options'       => $finalOptions,
          '#default_value' => $defaultOption,
          '#disabled'      => $readOnlyCompliance,
          '#attributes'    => $attributes,
          '#ajax' => array(
                        'wrapper'        => $dispWrapperId,
                        'callback'       => 'catalog_course_repaint_callback',
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


                          endAddEditTwoColumnRow($form);
                          $isCompliance = isset($form_state['values']["course_compliance"]) ? $form_state['values']["course_compliance"] : $defaultValue['is_compliance'];
                          //

                            addTwoColumnRowToAddEditForm($form, 'author_vendor_complete');
                            $options = array('days' => ucfirst(t('LBL605')), 'date' => t('LBL042'));
                            $completed_options = array(
                                        'date' => t('LBL042'),
                                        //'days' => t('LBL3057').' '. t('LBL3083'), 
                                        'days' => '<span class="vtip" title="'.t('LBL3057').' '. t('LBL3083').'" >'.titleController('COMPLETED-BY', sanitize_data(t('LBL3057').' '. t('LBL3083')),20).'</span>',
                                        //'hire_days' => t('LBL3057').' '. t('LBL3084'), 
                                        'hire_days' => '<span title="'.t('LBL3057').' '. t('LBL3084').'" class="vtip" >'.titleController('COMPLETED-BY', sanitize_data(t('LBL3057').' '. t('LBL3084')),40).'</span>',
                            );
                            $dispWrapperId                   = 'catalog-course-basic-addedit-form';
                            $attributes                      = array();
                            $attributes['class']             = array('addedit-edit-compliance_course');
                            $attributes['class'][]           = 'addedit-form-expertusone-throbber';
                            $attributes['data-wrapperid']    = array($dispWrapperId);
                            $attributes['tabindex']          = 7;

                            $compliance_completed = isset($form_state['values']["compliance_completed"]) ? $form_state['values']["compliance_completed"] : (!empty($defaultValue['compliance_completed']) ? $defaultValue['compliance_completed'] : 'date');

                            $form['compliance_completed'] = array (
            '#type' => 'radios',
            '#prefix' => addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', t('LBL234'), FALSE),
                            //'#suffix' => addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD'),
            '#title' => t('LBL234'),
            '#title_display' => 'invisible',
            '#default_value' =>  $compliance_completed,
                                '#options' => $completed_options,
            '#disabled' => $readOnlyCompliance,
                            //'#attributes' => $attributes
            '#ajax' => array(
                          'wrapper'        => $dispWrapperId,
                          'callback'       => 'catalog_course_repaint_callback',
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

                            if($compliance_completed == 'days') {

                                $comp_attributes             = array();
                                $comp_attributes['tabindex'] = 8;
                                $comp_attributes['class'][] = 'admin-course-class-compliance-days-input';
                                $comp_attributes['onkeydown'] = 'return onlyNumbers(event)';

                                $form['completed_days'] = array(
                  '#type' => 'textfield',
                  '#prefix' => '<div class="admin-course-class-compliance-date">',
                  '#suffix' => '</div>',
                  '#title' => t('LBL731'),
                  '#title_display' => 'invisible',
                  '#default_value' => $defaultValue['complete_days'],
                                //'#disabled' => $readOnly,
                  '#attributes' => $comp_attributes,
                  '#size' => 17,//$viewWidthInChars,
                  '#disabled'      => $readOnlyCompliance,
                  '#maxlength' => 5,
                                //'#description' => $description
                                );
                            } else if ($compliance_completed == 'hire_days') {
                                
                                $comp_attributes             = array();
                                $comp_attributes['tabindex'] = 8;
                                $comp_attributes['class'][] = 'admin-course-class-compliance-hire-days-input';
                                $comp_attributes['onkeydown'] = 'return onlyNumbers(event)';
                                 
                                $form['completed_hire_days'] = array(
                                        '#type' => 'textfield',
                                        '#prefix' => '<div class="admin-course-class-compliance-date">',
                                        '#suffix' => '</div>',
                                        '#title' => t('LBL731'),
                                        '#title_display' => 'invisible',
                                        '#default_value' => $defaultValue['completed_hire_days'],
                                        //'#disabled' => $readOnly,
                                        '#attributes' => $comp_attributes,
                                        '#size' => 17,//$viewWidthInChars,
                                        '#disabled'      => $readOnlyCompliance,
                                        '#maxlength' => 5,
                                        //'#description' => $description
                                );
                            } else if($compliance_completed == 'date') {
                                $defaultCompDate =  $defaultValue['complete_date'];
                                if($defaultCompDate!='') $defaultCompDate = date('m-d-Y', strtotime($defaultCompDate));
                                $attributes = array();
                                $attributes['autocomplete'] = 'off';
                                $attributes['class'] = array('addedit-edit-completed_date'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

                                $attributes['data-datePickerDatesDisplayOption'] = "future";
                                $attributes['data-emptyDateFieldText'] = t('LBL112');
                                $attributes['tabindex'] = 8;
                                //42982: system throw error message when we set complete by date in numbers
                                $attributes['readonly'] = 'readonly';
                                $attributes['data-datePickerTooltip'] = '';
                                $attributes['class'][] = (($readOnlyCompliance) ? 'addedit-edit-disabled-datefield' : 'addedit-edit-datefield');
                                $attributes['class'][] = 'addedit-edit-ac-textfield';

                                $form['completed_date'] = array(
               '#id'   => 'completed_date',
               '#type' => 'textfield',
               '#prefix' => '<div class="admin-course-class-compliance-date">',
               '#suffix' => '</div>',
               '#title' => t('LBL732'),
               '#title_display' => 'invisible',
               '#default_value' => $defaultCompDate,
               '#disabled'      => $readOnlyCompliance,
                                //'#disabled' => $readOnly,
               '#size' => 8,
               '#attributes' => $attributes,
                                );
                            }
                            
                         if($isCompliance == 1) { 
                            $form['compliance_completedby_end'] = array(
            '#type' => 'markup',
            '#markup' => addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD'),
                            );
                            $compliance_validity = isset($form_state['values']["compliance_validity"]) ? $form_state['values']["compliance_validity"] : (!empty($defaultValue['validity_date']) ? 'date' : 'days');
                            //addRadioSetToAddEditForm($form, 'compliance_validity', t('Validity'), $options, $defaultValue, 8,false, false);
                            $validity_attributes             = array();
                            $validity_attributes['tabindex'] = 8;
                            $form['compliance_validity'] = array (
            '#type' => 'radios',
                            //'#prefix' => addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', t('Compliance Course'), TRUE),
                            //'#suffix' => addEditGetSuffix('TWOCOL_SECONDCOL_FIELD'),
            '#prefix' => addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', t('LBL604'), FALSE),
                            // '#suffix' => addEditGetSuffix('TWOCOL_SECONDCOL_FIELD'),
            '#title' => t('LBL604'),
            '#title_display' => 'invisible',
            '#default_value' =>  $compliance_validity,
            '#options' => $options,
            '#disabled' => $readOnlyCompliance,
            '#attributes' => $validity_attributes,
            '#ajax' => array(
                          'wrapper'        => $dispWrapperId,
                          'callback'       => 'catalog_course_repaint_callback',
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





                            if($compliance_validity == 'days') {
                                $validity_days_attributes             = array();
                                $validity_days_attributes['tabindex'] = 9;
                                $validity_days_attributes['onkeydown'] = 'return onlyNumbers(event)';

                                $form['validity_days'] = array(
                  '#type' => 'textfield',
                                //'#prefix' => $prefix,
                                //'#suffix' => $suffix,
                  '#title' => t('LBL667'),
                  '#title_display' => 'invisible',
                  '#default_value' => $defaultValue['validity_days'],
                                //'#disabled' => $readOnly,
                  '#attributes' => $validity_days_attributes,
                  '#size' => 8,//$viewWidthInChars,
                  '#disabled'      => $readOnlyCompliance,
                  '#maxlength' => 5,
                                //'#description' => $description
                                );
                            } else if($compliance_validity == 'date') {
                                $defaultValidityDate =  $defaultValue['validity_date'];
                                if($defaultValidityDate!='') $defaultValidityDate = date('m-d-Y', strtotime($defaultValidityDate));
                                $attributes = array();
                                $attributes['autocomplete'] = 'off';
                                $attributes['class'] = array('addedit-edit-validity_date'); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

                                $attributes['data-datePickerDatesDisplayOption'] = "future";
                                $attributes['data-emptyDateFieldText'] = t('LBL112');
                                $attributes['tabindex'] = 9;
                                //42982: system throw error message when we set complete by date in numbers
                                $attributes['readonly'] = 'readonly';
                                $attributes['data-datePickerTooltip'] = '';
                                $attributes['class'][] = 'addedit-edit-datefield';
                                $attributes['class'][] = (($readOnlyCompliance) ? 'addedit-edit-disabled-datefield' : 'addedit-edit-datefield');
                                $attributes['class'][] = 'addedit-edit-ac-textfield';


                                $form['validity_date'] = array(
               '#id'   => 'validity_date',
               '#type' => 'textfield',
               '#prefix' => '<div class="compliance-validity">',
               '#suffix' => '</div>',
               '#title' => t('LBL733'),
               '#title_display' => 'invisible',
               '#default_value' => $defaultValidityDate,
               '#disabled'      => $readOnlyCompliance,
                                //'#disabled' => $readOnly,
               '#size' => 8,
               '#attributes' => $attributes,
                                );
                            }
                         }
                            $form['compliance_validityby_end'] = array(
            '#type' => 'markup',
            '#markup' => addEditGetSuffix('TWOCOL_SECONDCOL_FIELD'),
                            );

                            endAddEditTwoColumnRow($form);

                          //}

                          $form['course_loading_default_focus'] = array (
                    '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="autoFocusFirstField();" />',
                          );

            // Changed by Gayathri for 0070892
            $form['course-authorvendor-container-start'] = array(
                    '#type' => 'markup',
                    '#markup' => '<div class="admin-course-authorvendor-container">'
              );                
               // addTextFieldToAddEditForm($form, 'author_vendor', t('LBL269'), $defaultValue, 110, false,false,50);
               addTextFieldToAddEditForm($form, 'crs_author_vendor', t('LBL269'), $defaultValue['crs_author_vendor'], 5, false, false, 50);            
               
            
            $form['course-authorvendor-container-end'] = array(
                    '#type' => 'markup',
                    '#markup' => '</div>'
                );                  
                
                if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/To display the Custom attributes UI
                    include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                    renderCustomAttributesInAddEditForm($form, $form_state,'cre_sys_obt_crs',$entityId);
                    addCustomAttributesBlockToAddEditForm($form, $form_state, $defaultValue['crs_id'], 'course', 'catalog-custom-attributes', 'catalog-course-basic-addedit-form');  
                }else{    //To display custom fields
                          addCustomFieldsBlockToAddEditForm($form, $form_state, $defaultValue['crs_id'], 'course', 'course-custom-fields'.$defaultValue['crs_id'], 'catalog-course-basic-addedit-form', 10);
                } //#custom_attribute_0078975 - End Check module status
                 
                          //addTextAreaFieldToAddEditForm($form, 'crs_additional_info', t('LBL156'), $defaultValue['crs_additional_info'], 8, false);
            }

            /*if(!empty($entityId)){
             $form['list_type_'.$uniqueId] = array(
             '#type'     => 'select',
             '#prefix'   => '<div class="admin-course-add-attachment admin-course-add-attachment-select">',
             '#suffix'   => '</div>',
             '#options'  => array(
             ''                        => t('Add'),
             'attachment_fieldset'     => t('Attachment'),
             'tag_fieldset'            => t('Tags'),
             'prerequisite_fieldset'   => t('Pre-requisite'),
             'equivalence_fieldset'    => t('Equivalence'),
             'survey_fieldset'         => t('LBL203'),
             'assessment_fieldset'     => t('Assessment'),
             'custom_fieldset'         => t('Custom Fields'),
             ),
             '#attributes' => array('onchange' => 'showHide(this, "'.$uniqueId.'");selecteDropdown();','tabindex' => '19','Class' => 'select'),
             );
             }*/
            $form_state['esign_from_plugin']= false;
            //$formViewMode = ($form_state['storage']['step'] == 'view') ? TRUE : FALSE;
            if($form_state['storage']['substep'] == 'Attachment') {
                $form_state['esign_from_plugin']= true;
                // To display the Attachment UI
                displayAttachmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            if($form_state['storage']['substep'] == 'tag') {
                $form_state['esign_from_plugin']= true;
                // To display the Tag UI
                displayTagDetails($form, $form_state, $entityId, 'Course', $uniqueId);
            }

            // To display the Pre-requisite UI
            if($form_state['storage']['substep'] == 'prerequisite') {
                $form_state['esign_from_plugin']= true;
                $showCancelSave = 0;
                displayPrerequisiteDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            }

            /*

            // To display the assessment UI
            if($form_state['storage']['substep'] == 'assessment') {
            displayAssessmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            */
            // To display the Custom Fields UI
            if($form_state['storage']['substep'] == 'custom') {
                $form_state['esign_from_plugin']= true;
                displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            /*
             if(!empty($entityId)){
             if($form_state['storage']['substep'] == 'notifications') {
             addNotificationsToAddEditForm($form, $entityId, $entityType);
             }
             }
             */
            if($form_state['storage']['substep'] == 'equivalence') {
                $form_state['esign_from_plugin']= true;
                $showCancelSave = 0;
                displayEquivalenceDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            }
            /*
             if($form_state['storage']['substep'] == 'survey') {
             $showCancelSave = 0;
             displaySurveyDetails($form, $form_state, $entityId, $entityType, $uniqueId);
             }

             if($form_state['storage']['substep'] == 'assessment') {
             $showCancelSave = 0;
             displayAssessmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);
             }
             */
            if($form_state['storage']['substep'] == 'certificate') {
                $form_state['esign_from_plugin']= true;
                displayCertificateToAddEditForm($form, $form_state, $entityId, $entityType, $uniqueId,false);
            }

            // Checking to show the cancel and save button
            if($showCancelSave && $form_state['storage']['display_mode'] != 'view') {
                $subStage = $form_state['storage']['substep'];
                addEditFormWithCourseCancelAndSaveClose($form, 'catalog-course-basic-addedit-form', 'catalog-course-basic-save', 'catalog-course-basic-cancel', $entityId, $subStage, $emptyId);
            }
        }
     $form['course_detail_disp_end'] = array(
      '#type'       => 'markup',
     //'#prefix'     => '<div id="catalog-course-basic-addedit-form-disp">',
     //'#suffix'     => '<div class="clearBoth"></div></div>',
      '#suffix'     => '<div class="clearBoth"></div>
                        <div class="round-corner-footer-left"><div class="round-corner-footer-right">
                        <div class="round-corner-footer-middle"></div></div></div></div></div><div class="clearBoth"></div>',
     //'#attributes' => $editAttributes,
     );

     if($showCancelSave && $form_state['storage']['display_mode'] != 'view'){
        $form['course_detail_wrapper_end'] = array(
        '#type'       => 'markup',
        '#suffix'     => '</div>',
        );
     } else {
        $form['course_detail_wrapper_end'] = array(
        '#type'       => 'markup',
        '#suffix'     => '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
        );
     }
     return $form;

    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_catalog_course_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}

function catalog_course_repaint_callback($form, &$form_state){
    try {
        return $form;
    } catch (Exception $ex) {
        watchdog_exception('catalog_course_repaint_callback', $ex);
        expertusErrorThrow($ex);
    }
}

function list_forms(&$form, &$form_state, $defaultValue){
    try {
        $form['course_actions_disp'] = array(
          '#type'       => 'markup',
          '#prefix'     => '<div class="catalog-course-basic-addedit-action-disp" id="catalog-course-basic-addedit-action-disp">',
        //'#suffix'     => '</div>',
        //'#attributes' => $editAttributes,
        );

        if(!isset($form_state['storage']['substep'])){
            $form_state['storage']['substep'] = '';
        }
        $isSelected = ($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep'])) ? 'selected' : '';
        $form['edit_btn_details'] = array(
          '#type'       => 'submit',
          '#value'      => t('LBL272'),
          '#name'       => 'details',
        //'#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container"><div class="admin-save-button-left-bg"></div>',
        //'#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
          '#prefix'     => '<ul><li>',
          '#suffix'     => '</li>',
          '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
            '#ajax' => array(
            'callback' => 'addEditRepaintForm',
            'wrapper'  => 'catalog-course-basic-addedit-form',
            'event'    => 'click'
            ),
            );
            $entityId       = $form['mapped_course_id']['#value'];
            //if($entityId && ($form_state['storage']['step'] == 'edit' || empty($form_state['storage']['step']))){
            if($entityId){
                $isSelected = ($form_state['storage']['substep'] == 'Attachment') ? 'highlighted' : '';
                /*$form['edit_btn_att'] = array(
                 '#type'       => 'submit',
                 '#value'      => t('LBL273'),
                 '#name'       => 'Attachment',
                 '#prefix'     => '<li>',
                 '#suffix'     => '</li>',
                 //'#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container"><div class="admin-save-button-left-bg"></div>',
                 //'#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
                 '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
                 '#ajax' => array(
                 'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );*/
                /*
                 $isSelected = ($form_state['storage']['substep'] == 'survey') ? 'highlighted' : '';
                 $form['edit_btn_survey'] = array(
                 '#type'       => 'submit',
                 '#value'      => t('LBL203'),
                 '#name'       => 'survey',
                 '#prefix'     => '<li>',
                 '#suffix'     => '</li>',
                 '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
                 '#ajax' => array(
                 'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );

              $isSelected = ($form_state['storage']['substep'] == 'assessment') ? 'highlighted' : '';
              $form['edit_btn_assessment'] = array(
              '#type'       => 'submit',
              '#value'      => t('Assessment'),
              '#name'       => 'assessment',
              '#prefix'     => '<li>',
              '#suffix'     => '</li>',
              '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
              '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );
              */
                /*$isSelected = ($form_state['storage']['substep'] == 'tag') ? 'highlighted' : '';
                 $form['edit_btn_tag'] = array(
                 '#type'       => 'submit',
                 '#value'      => t('LBL191'),
                 '#name'       => 'tag',
                 '#prefix'     => '<li>',
                 '#suffix'     => '</li>',
                 '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
                 '#ajax' => array(
                 'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );*/
                /*
                 $isSelected = ($form_state['storage']['substep'] == 'notifications') ? 'highlighted' : '';
                 $form['edit_btn_notifications'] = array(
              '#type'       => 'submit',
              '#value'      => t('Notification'),
              '#name'       => 'notifications',
              '#prefix'     => '<li>',
              '#suffix'     => '</li>',
              '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
              '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );
              */
                $isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';
                $form['edit_btn_prerequest'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL230'),
            '#name'       => 'prerequisite',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );
              $isSelected = ($form_state['storage']['substep'] == 'equivalence') ? 'highlighted' : '';
              $form['edit_btn_equivalence'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL279'),
            '#name'       => 'equivalence',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-course-basic-addedit-form',
              'event'      => 'click'
              ),
              );
              /* $isSelected = ($form_state['storage']['substep'] == 'certificate') ? 'highlighted' : '';
               $form['edit_btn_certificate'] = array(
               '#type'       => 'submit',
               '#value'      => t('LBL205'),
               '#name'       => 'certificate',
               '#prefix'     => '<li>',
               '#suffix'     => '</li>',
               '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
               '#ajax' => array(
               'callback' => 'addEditRepaintForm',
               'wrapper'  => 'catalog-course-basic-addedit-form',
               'event'     => 'click'
               ),
               );

               $isSelected = ($form_state['storage']['substep'] == 'custom') ? 'highlighted' : '';
               $form['edit_btn_custom'] = array(
               '#type'       => 'submit',
               '#value'      => t('LBL276'),
               '#name'       => 'custom',
               '#prefix'     => '<li>',
               '#suffix'     => '</li></ul>',
               '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber remove-border',$isSelected), 'data-wrapperid' => 'catalog-course-basic-addedit-form'),
               '#ajax' => array(
               'callback' => 'addEditRepaintForm',
               'wrapper'  => 'catalog-course-basic-addedit-form',
               'event'     => 'click'
               ),
               );
               */
            }
            else{
                //$isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';
                $form['edit_btn_prerequest_disabled'] = array(
            '#type'       => 'item',
            '#title'      => t('LBL230'),
            '#prefix'     => '<li><span class="deactive-links">',
            '#suffix'     => '</span></li>'
            );
            $form['edit_btn_equivalence_disabled'] = array(
            '#type'       => 'item',
            '#title'      => t('LBL279'),
            '#prefix'     => '<li><span class="deactive-links">',
            '#suffix'     => '</span></li>',
            );

            $form['course_actions_ul_container_disp_end'] = array(
           '#type'       => 'markup',
           '#suffix'     => '</ul>',
            );
            }
            $form['course_actions_disp_end'] = array(
         '#type'       => 'markup',
         '#suffix'     => '</div>',
            );
    } catch (Exception $ex) {
        watchdog_exception('list_forms', $ex);
        expertusErrorThrow($ex);
    }
}

function add_course_forms(&$form, &$form_state, $defaultValue){
    try{

    } catch (Exception $ex) {
        watchdog_exception('add_course_forms', $ex);
        expertusErrorThrow($ex);
    }
}

/*function exp_sp_administration_course_form_callback($form, &$form_state) {
 $form_state['rebuild'] = TRUE;
 return $form;
 }

 function form_add_edit_ajax_submit(&$form, &$form_state) {
 $form_state['storage']['step'] = 'edit';
 $form_state['display_class']   = TRUE;
 $form_state['rebuild']         = TRUE;
 }*/
function edit_course_details(&$form, &$form_state, $defaultValue){
    try {
        global $theme_key;
        unset($editAttributes['onclick']);
        $editAttributes['class'][]         = 'addedit-form-expertusone-throbber';
        $editAttributes['data-wrapperid']  = array('catalog-course-basic-addedit-form');
        //$editAttributes['class'][]         = 'addedit-form-class-button';
        $editAttributes['class'][]         = 'admin-add-edit-button';
        $editAttributes['onclick'][]       = '$(document).ready(function(){ editCourseDetailsView(); }); remove_messages("course");';
        if($form_state['storage']['display_mode'] != 'view' && ($form_state['storage']['substep'] == 'details' || empty($form_state['storage']['substep']))){
            $entityType = 'cre_sys_obt_crs';
            $entityId = array($defaultValue['crs_id']);
            $userId = getIdOfLoggedInUser();
            $check = objectLevelAccessCheckInGroupMap($entityType,$entityId);
            $editableMode= true;
            if(!empty($check)){
                $courseEditCheck = apiUseraccesscheck($userId,$entityType,$defaultValue['crs_id']);
                if(!empty($courseEditCheck)){
                    $editableMode = true;
                }
                else {
                    $editableMode = false;
                }
            }
            if($editableMode == true) {
                $form['edit_btn'] = array(
                //'#type' => 'image_button',
                //'#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/edit.png',
              '#type' => 'submit',
              '#value'      => ($theme_key == 'expertusoneV2')? ' ': t('LBL063'),
              '#name'       => 'Edit Course',
              '#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container">',
              '#suffix'     => '</div></div>',
              '#attributes' => $editAttributes,
              '#ajax' => array(
                'callback' => 'addEditRepaintForm',
                'wrapper'  => 'catalog-course-basic-addedit-form',
                'event'    => 'click'
                ),
                );
            }
        }
    } catch (Exception $ex) {
        watchdog_exception('edit_course_details', $ex);
        expertusErrorThrow($ex);
    }
}
function edit_course_view(&$form, &$form_state, $defaultValue, $entityId){

    try {
        expDebug::dPrint('Default values from the form::  ' . print_r($defaultValue,true),5);
        /*  unset($editAttributes['onclick']);
         $editAttributes['class'][]         = 'addedit-form-expertusone-throbber';
         $editAttributes['data-wrapperid']  = array('catalog-course-basic-addedit-form');
         $editAttributes['class'][]         = 'admin-save-button-middle-bg';
         $editAttributes['onclick'][]       = '$(document).ready(function(){ editCourseDetailsView(); }); remove_messages("course");';

         $form['edit_btn'] = array(
         '#type'       => 'submit',
      '#value'      => t('Edit'),
      '#name'       => 'Edit Course',
      '#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container"><div class="admin-save-button-left-bg"></div>',
      '#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
      '#attributes' => $editAttributes,
      '#ajax' => array(
      'callback' => 'addEditRepaintForm',
      'wrapper'  => 'catalog-course-basic-addedit-form',
      'event'      => 'click'
      ),
      );*/
        
        $form['crs_title'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL083').':',
        '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt genere-title vtip" title="'.sanitize_data($defaultValue['crs_title']).'">'.titleController('ADDEDIT-COURSE-FIELD-TITLE', sanitize_data($defaultValue['crs_title']),50).'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
        );


        $form['crs_code'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL096').':',
        '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view course-view-mode">',
        '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.sanitize_data($defaultValue['crs_code']).'" class="vtip">'.titleController('ADDEDIT-COURSE-FIELD-CODE', sanitize_data($defaultValue['crs_code']),20).'</span></div>',
        '#suffix'   => '</div>',
        );
        $form['crs_language'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL038').':',
        '#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.t($defaultValue['crs_language_name']).'</div>',
        '#suffix'   => '</div><div class="clearBoth"></div></div>',
        );
        /* $form['crs_short_description'] = array(
         '#type'     => 'item',
         '#title'    => t('LBL282'),
         '#prefix'   => '<div class="admin-course-class-short-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
         '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['crs_short_description'].'</div><div class="clearBoth"></div>',
         '#suffix'   => '</div></div>',
         );
         $form['hidden_marku_short'] = array(
         '#type'     => 'markup',
         '#prefix'   => '<img style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" onload="displayLongDescription(\'short_description\')" />',
         );*/
        $form['crs_short_description'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL229').':',
        '#prefix'   => '<div class="admin-course-class-long-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt limit-desc cre_sys_obt_crs trunk8-fade">'.$defaultValue[str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""),'crs_short_description')].'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div></div>',
        );

        $form['hidden_markup_long'] = array(
        '#type'     => 'markup',
        '#prefix'   => '<img style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" onload="displayLongDescription(\'long_description\')" />',
        );


        /*   $form['crs_status'] = array(
         '#type'     => 'item',
         '#title'    => t('LBL285'),
         '#prefix'   => '<div class="second-col-container-view">',
         '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.$defaultValue['crs_status_name'].'</div>',
         '#suffix'   => '</div></div>',
         );*/

        /*    if(!empty($defaultValue['status_inactive_reason'])) {
         $form['status_inactive_reason'] = array(
         '#type'     => 'item',
         '#title'    => t('Inactive Reason:'),
         '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
         '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['status_inactive_reason'].'</div><div class="clearBoth"></div>',
         '#suffix'   => '</div>',
         );
         }*/

        $form['price'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL040').':',
        '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.$defaultValue['crs_price'].'" class="vtip">'.$defaultValue['crs_currency_symbol'].' '.$defaultValue['crs_price'] .'</span></div>',
        '#suffix'   => '</div>',
        );
        $form['crs_is_compliance'] = array(
        '#type'     => 'item',
        '#title'    => t('Compliance').':',
        '#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.(($defaultValue['is_compliance'] == 1) ? t('Yes') : t('No')).'</div>',
        '#suffix'   => '</div><div class="clearBoth"></div></div>',
        );

    //  if($defaultValue['is_compliance'] == 1) {

            $defaultCompDate =  $defaultValue['complete_date'];
            if($defaultCompDate)
            $defaultCompDate = date('m-d-Y', strtotime($defaultCompDate));
            else
            $defaultCompDate = '';
            
            //0073744: Label statement incorrect in course view form
            $compliance_type = '';
            if($defaultValue['compliance_completed'] == "days"){
                $compliance_type = t('LBL3083');
                $days = $defaultValue['complete_days'];
            }elseif($defaultValue['compliance_completed'] == "hire_days"){
                $compliance_type = t('LBL3084');
                $days = $defaultValue['completed_hire_days'];
            }
            
            $form['compliance_complete'] = array(
            '#type'     => 'item',
            '#title'    => t('LBL234').':',
            '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
            '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt">'.(!empty($days) ? ($days.' '.t('LBL605')).' '.$compliance_type: $defaultCompDate).'</div>',
            '#suffix'   => '</div>',
            );
            expDebug::dPrint('validity_date for the course is ' . $defaultValue['complete_days'],4);
            list($validity_date, $validityTime) = explode(" ",$defaultValue['validity_date']);
            if(!empty($validityTime)){
                $defaultValidityDate = date('m-d-Y', strtotime($defaultValue['validity_date']));
            }
            else{
                $defaultValidityDate = $validity_date;
            }
            //43134: Help text are showing in view mode
            if($defaultValidityDate == t('LBL112')){
                $defaultValidityDate ='';
            }
         if(($defaultValue['is_compliance'] == 1)){
            $form['compliance_validate'] = array(
            '#type'     => 'item',
            '#title'    => t('LBL604').':',
            '#prefix'   => '<div class="second-col-container-view">',
            '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.(!empty($defaultValue['validity_days']) ? ($defaultValue['validity_days'].' '.t('LBL605')) : $defaultValidityDate).'</div>',
            '#suffix'   => '</div><div class="clearBoth"></div></div>',
            );
            $crs_author_vendor_prefix = '<div class="two-column-container-view"><div class="first-col-container-view">';
         }else{
            $crs_author_vendor_prefix = '</div><div class="two-column-container-view"><div class="first-col-container-view">';
         }
            $form['crs_author_vendor'] = array(
            '#type'     => 'item',
            '#title'    => t('LBL269').':',
            '#prefix'   => $crs_author_vendor_prefix,
            '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.$defaultValue['crs_author_vendor'].'" class="vtip">'.titleController('ADDEDIT-COURSE-FIELD-AUTHOR-VENDOR', $defaultValue['crs_author_vendor'],20).'</span></div>',
            '#suffix'   => '</div></div>',
            );

    //  }
        $form['sortable-load'] = array(
            '#type' => 'markup',
            '#prefix' => '<img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0" onload="vtip(); " />',
        );

        //Temporary hide
        // Don't want to show buttons while in view mode
        /*$form['addedit-left-col-view-extend-func'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="crs-tab-titles-container-view"><div class="attachment-tab-icon"></div><div class="tab-title">Attachments</div><div class="tab-seperator"></div><div class="tags-tab-icon"></div><div class="tab-title">Tags</div><div class="tab-seperator"></div><div class="certificate-tab-icon"></div><div class="tab-title">Certificate</div><div class="tab-seperator"></div>
        <div class="access-tab-icon"></div>'.
        "<div id='".$visibPopupId."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptObj.");\" class=\"tab-title\">Access</div>".
        '</div>',
        );*/
        /*$form['crs_additional_info'] = array(
         '#type'     => 'item',
         '#title'    => t('LBL156'),
         '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
         '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['crs_additional_info'].'</div><div class="clearBoth"></div>',
         '#suffix'   => '</div>',
         );   */
        
        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Get Custom Attributes for Course View
         
                
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                $customAttrValues = getCustomAttributesForView($entityId, 'cre_sys_obt_crs','edit-view');
        
                $k=1;
                $reached_2_col=0;
                
                foreach($customAttrValues as $cuskey=>$cusvalue){
                    if($k%2==1){
                        $form['crs_custom_attr_left_'.$k] = array(
                            '#type'     => 'item',
                            '#title'    => t($cuskey).':',
                            '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
                            '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.sanitize_data($cusvalue).'" class="vtip">'.titleController('ADDEDIT-COURSE-CUSTOMATTRIBUTE-LEFT-COL', sanitize_data($cusvalue),20).'</span></div>',
                            '#suffix'   => '</div>',
                        );
                        $reached_2_col=0;
                    }
                    if($k%2==0){
                        $form['crs_custom_attr_right_'.$k] = array(
                            '#type'     => 'item',
                            '#title'    => t($cuskey).':',
                            '#prefix'   => '<div class="second-col-container-view">',
                            '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt"><span title="'.sanitize_data($cusvalue).'" class="vtip">'.titleController('ADDEDIT-COURSE-CUSTOMATTRIBUTE-RIGHT-COL', sanitize_data($cusvalue),20).'</span></div>',
                            '#suffix'   => '</div><div class="clearBoth"></div></div>',
                        );
                        $reached_2_col=1;
                    }
                    $k++;
                }
                if($reached_2_col==0){
                    $form['crs_custom_attr_finish'.$k-1] = array(
                            '#type'     => 'item',
                            '#markup'   => '<div class="clearBoth"></div></div>',
                        );
                }
        }else{ //#custom_attribute_0078975 - End Check module status 
        $customDetails = getCustomFieldsDetails($entityId, 'course');
        for ($i = 0; $i < count($customDetails); $i++) {
            $form['crs_custom_'.$i] = array(
        '#type'     => 'item',
        '#title'    => '<div class="surv-ass-customLbl vtip" title="'.sanitize_data(t($customDetails[$i]->label_txt)).'">'.$customDetails[$i]->label_txt.':</div>',
        '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$customDetails[$i]->value_txt.'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
            );
        }
        }//#custom_attribute_0078975 - End Check module status
        
    } catch (Exception $ex) {
        watchdog_exception('edit_course_view', $ex);
        expertusErrorThrow($ex);
    }
}

function getDefaultCrsAddEditFormFieldValue($fields, $form_state, $crsDetails) {
    try {
        $defaultValue   = '';
        $crsDefaultValue    = array();
        for($i=0;$i<count($fields);$i++) {
            $field                   = $fields[$i];
            if ($field == 'crs_short_description') {
                $crsDefaultValue[$field] = empty($form_state['values'][$field]['value']) ||  $form_state['values'][$field]['value']==''? (empty($crsDetails->$field) || $crsDetails->$field==''? '' : $crsDetails->$field) : $form_state['values'][$field]['value'];
            }elseif($field == 'complete_days' || $field == 'completed_hire_days' ){             
                if(!empty($crsDetails->complete_days) && $crsDetails->compliance_completed == "hire_days" ){
                    $crsDetails->completed_hire_days = $crsDetails->complete_days;
                    $crsDetails->complete_days = '';
                }
                $crsDefaultValue[$field] = empty($form_state['values'][$field]) ? (empty($crsDetails->$field) || $crsDetails->$field==''? '' : $crsDetails->$field) : $form_state['values'][$field];
                
            }else{
                $crsDefaultValue[$field] = empty($form_state['values'][$field]) ? (empty($crsDetails->$field) || $crsDetails->$field==''? '' : $crsDetails->$field) : $form_state['values'][$field];
            }
        }
        return $crsDefaultValue;
    } catch (Exception $ex) {
        watchdog_exception('getDefaultCrsAddEditFormFieldValue', $ex);
        expertusErrorThrow($ex);
    }
}

function getCourseDetail($courseId,$fields) {
    try {
        $select = db_select("slt_course_template","crs");
        $select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = crs.lang_code');
        $select->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = crs.status');
        $select->addField('crs','id','crs_id');
        $select->addField('crs','title','crs_title');
        $select->addField('crs','code','crs_code');
        $select->addField('crs','lang_code','crs_language');
        $select->addField('crs','status','crs_status');
        // Changed by Gayathri for 0070892
        $select->addField('crs','price','crs_price');
        $select->addField('crs','currency_type','crs_currency_type');
        $select->addExpression('(Select attr2 from slt_profile_list_items where attr1 = crs.currency_type)','crs_currency_symbol');     
        $select->addField('prof1','name','crs_language_name');
        $select->addField('prof2','name','crs_status_name');
        $select->addField('crs','short_description','crs_short_description');
        $select->addField('crs','description','crs_description');
        //$select->addField('crs','additional_info','crs_additional_info');
        $select->addField('crs','author_vendor','crs_author_vendor');
        $select->addField('crs','is_compliance','is_compliance');
        $select->addField('crs','complete_days','complete_days');
        $select->addField('crs','complete_date','complete_date');
        $select->addField('crs','validity_days','validity_days');
        $select->addField('crs','validity_date','validity_date');
        $select->addField('crs','compliance_completed','compliance_completed');
        $select->addField('crs','in_active_reason','status_inactive_reason');
        $select->condition('crs.id',$courseId,'=');
        expdebug::dPrintDBAPI('$select', $select);
        $crsList = $select->execute()->fetchAll();
        expdebug::dPrint('$crsList' . print_r($crsList, true),3);
        return $crsList;
    } catch (Exception $ex) {
        watchdog_exception('getCourseDetail', $ex);
        expertusErrorThrow($ex);
    }
}

function exp_sp_administration_catalog_course_addedit_form_validate($form, &$form_state){
    try {
        global $secure_entities;
        $form_state['hidden-save-submit'] = false;
        $form_state['esign_show_popup']= false;
        expDebug::dPrint(' exp_sp_administration_catalog_course_addedit_form_validate triggering_element:' . print_r($form_state['triggering_element'],true), 5);
        expDebug::dPrint(' form_State:' . print_r($form_state, true) , 5);
        //die($form_state['triggering_element']['#value']."TEst");
        if(strrpos($form_state['triggering_element']['#name'],'prerequisite')){
            include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_prerequisite.inc');
        }
        if(strrpos($form_state['triggering_element']['#name'],'equivalence')){
            include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_equivalence.inc');
        }
        if($form_state['triggering_element']['#name'] == 'course_compliance') {
            return ;
        }

        if (!empty($form_state['storage']['new_entity'])) {
            unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
            unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
        }

        if (!empty($form_state['storage']['edited_entity'])) {
            unset($form_state['storage']['edited_entity']); // Unset the variable to remove it from $form_state
        }

        if ($form_state['triggering_element']['#name'] == t('cancelcourse')) {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            unset($form_state['custom_fields']); // Clear any change made to custom fields
            $form_state['rebuild']   = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['step'] = 'view';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'Edit Course') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['step'] = 'edit';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'details') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'details';
            //$form_state['storage']['step'] = 'edit';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'Attachment') {
            $form_state['display_class']   = TRUE;
            $form_state['show_add_another'] = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'Attachment';
            unset($form_state['delete_attachment_index']);
            unset($form_state['delete_attachment_fieldset']);
            $form_state['num_attachment'] = 0;
            return;
        }
        if ($form_state['triggering_element']['#name'] == 'tag') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'tag';
            unset($form_state['delete_tag_index']);
            unset($form_state['delete_tag_fieldset']);
            $form_state['num_tag'] = 0;
            return;
        }
        if ($form_state['triggering_element']['#name'] == 'prerequisite') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'prerequisite';
            unset($form_state['delete_prerequisite_index']);
            unset($form_state['delete_prerequisite_fieldset']);
            $form_state['num_prerequisite'] = 0;
            return;
        }

        /*
         if ($form_state['triggering_element']['#name'] == 'survey') {
         //$form_state['display_class']   = TRUE;
         //$form_state['input']     = array(); // Clear user input - raw
         //$form_state['values']    = array(); // Clear user input - security cleared
         $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
         $form_state['storage']['substep'] = 'survey';
         //unset($form_state['delete_survey_index']);
         //unset($form_state['delete_survey_fieldset']);
         //$form_state['num_survey'] = 0;
         return;
         }
         */
        if ($form_state['triggering_element']['#name'] == 'equivalence') {
            //$form_state['display_class']   = TRUE;
            //$form_state['input']     = array(); // Clear user input - raw
            //$form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'equivalence';
            return;
        }
        /*
         // LBL274
         if ($form_state['triggering_element']['#name'] == 'assessment') {
         //$form_state['display_class']   = TRUE;
         //$form_state['input']     = array(); // Clear user input - raw
         //$form_state['values']    = array(); // Clear user input - security cleared
         $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
         $form_state['storage']['substep'] = 'assessment';
         //unset($form_state['delete_assessment_index']);
         //unset($form_state['delete_assessment_fieldset']);
         //$form_state['num_assessment'] = 0;
         return;
         }
         */
        if ($form_state['triggering_element']['#name'] == 'custom') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'custom';
            unset($form_state['delete_custom_index']);
            unset($form_state['delete_custom_fieldset']);
            $form_state['num_custom'] = 0;
            return;
        }
        if ($form_state['triggering_element']['#name'] == 'notifications') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'notifications';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'equivalence') {
            //$form_state['display_class']   = TRUE;
            //$form_state['input']     = array(); // Clear user input - raw
            //$form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'equivalence';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'certificate') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'certificate';
            return;
        }

        if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
            $triggerValue  = 'save';
        }else{
            $triggerValue  = 'hidden-save';
        }
        $triggerElementByName  = $form_state['triggering_element']['#name'];

        expDebug::dPrint('check prerequisite list Ilayaraja trigger element :'.$form_state['triggering_element']['#name'], 5);
        expDebug::dPrint('check prerequisite list Ilayaraja storage substep :'. $form_state['storage']['substage'],5);

        if (($form_state['triggering_element']['#name'] == 'save' || $form_state['triggering_element']['#name'] == 'saveandpublish')) {
            //if ($form_state['triggering_element']['#value'] == $triggerValue) {
            $form_state['esign_show_popup']= true;//By default esign popup will open,
            //if any validation error messages throws, it will set as false
            // Check all mandatory fields exist
            addEditCheckMandatoryFields($form, $form_state, array('crs_title', 'crs_code', 'crs_language'));

            if(($form_state['values']['compliance_completed'] == 'days') && !empty($form_state['values']['completed_days']) && (is_numeric($form_state['values']['completed_days']) == false)) {
                form_set_error('completed_days', t('MSG458'));
                $form_state["storage"]["form_validate"] = false;
            } else if(($form_state['values']['compliance_completed'] == 'days') && !empty($form_state['values']['completed_days']) && ($form_state['values']['completed_days']<0)) {
                form_set_error('completed_days', t('MSG452'));
                $form_state["storage"]["form_validate"] = false;
            }
            if(($form_state['values']['compliance_completed'] == 'hire_days') && !empty($form_state['values']['completed_hire_days']) && ($form_state['values']['completed_hire_days']<0)) {
                form_set_error('completed_days', t('MSG452'));
                $form_state["storage"]["form_validate"] = false;
            }
            else if(($form_state['values']['compliance_completed'] == 'hire_days') && !empty($form_state['values']['completed_hire_days']) && (is_numeric($form_state['values']['completed_hire_days']) == false)) {
                form_set_error('completed_hire_days', t('MSG458')); /*Viswanathan changed hire_days to Completed_hire_days for #0075072 */
                $form_state["storage"]["form_validate"] = false;
            }
            if(($form_state['values']['compliance_completed'] == 'date') || ($form_state['values']['compliance_validity'] == 'date')) {
                $now_time      = strtotime("now");
                list($cMonth,$cDate,$cYear) = split("-",$form_state['values']['completed_date']);
                list($vMonth,$vDate,$vYear) = split("-",$form_state['values']['validity_date']);
                $completed_date  = mktime(0,0,0,$cMonth,$cDate,$cYear); //strtotime($form_state['values']['completed_date']);
                $validity_date   = mktime(0,0,0,$vMonth,$vDate,$vYear); //strtotime($form_state['values']['validity_date']);
                expDebug::dPrint('Compliance strtotime : now : '.$now_time.' | complete date '.$completed_date.' | validate : '.$validity_date." - ".$form_state['values']['validity_date'], 5);
                if(($form_state['values']['compliance_completed'] == 'date') && ($form_state['values']['compliance_validity'] == 'date') && ($completed_date >= $validity_date)&& ($form_state['values']['completed_date'] != t('LBL112'))&& ($form_state['values']['validity_date'] != t('LBL112'))) {
                    form_set_error('validity_date', t('MSG457'));
                    $form_state["storage"]["form_validate"] = false;
                }
                else if(($form_state['values']['compliance_completed'] == 'days') && ($form_state['values']['compliance_validity'] == 'date')&& ($form_state['values']['validity_date'] != t('LBL112'))){
                    $completedDays = ($form_state['values']['completed_days']) ? $form_state['values']['completed_days'] : 0;
                    $add_cdays     = $completedDays-1;
                    $completed_days = mktime(0,0,0,date("m"),date("d")+$add_cdays,date("Y"));
                    if($completed_days >= $validity_date) {
                        form_set_error('validity_date', t('MSG565'));
                        $form_state["storage"]["form_validate"] = false;
                    }
                }
            }

            if (empty($form_state['values']['crs_short_description']['value'])) {
                form_set_error('crs_short_description', $form['crs_short_description']['#title'] . t('ERR101'));
                $form_state['esign_show_popup']= false;
            }
            // Added by Gayathri for 0070892
            if(!is_numeric($form_state['values']['price']) && !empty($form_state['values']['price'])){
                form_set_error('price', t('ERR097').' '.t('LBL040').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
            }
            if(trim($form_state['values']['price']) < 0){
                form_set_error('price', t('ERR097').' '.t('LBL040').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
            }           
            addEditCodeValidate($form_state,$form_state['values']["crs_id"]);

            if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Validate Custom Attributes
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                validateCustomAttributesInAddEditForm($form, $form_state,'cre_sys_obt_crs');
            }else{ // Validate custom fields
            validateCustomFieldsInAddEditForm($form, $form_state);
            } //#custom_attribute_0078975 - End Check module status  

        }else if($form_state['triggering_element']['#name'] == $triggerValue && $form_state['esign_from_plugin']==true){
            $form_state['esign_show_popup']= true;
            if(isset($form_state['input']['attach_certificate'])){
                expDebug::dPrint('displayCertificateToAddEditForm default check'.$form_state['input']['attach_certificate'], 5);
                $form_state['default_certificate_id']=$form_state['input']['attach_certificate'];
            }
        }else if($triggerElementByName == 'esign-prerequisite' || $triggerElementByName == 'set-prerequisite'){
            $form_state['esign_show_popup']= true;
            $form_state['storage']['mode'] = 'edit';
            $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
            $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
            $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];

            if (in_array($entityType, $secure_entities)) {
                $uniqueId = "prerequisite-".core_encrypt($entityId)."-$entityType"; // URL encryption data to retrive
            } else {
                $uniqueId = "prerequisite-".$entityId."-$entityType"; // URL encryption data to retrive
            }
            $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
            if(isset($entityPlugin) && $entityPlugin=='prerequisite') {
                if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
                    drupal_set_message(t('ERR106'),'error');
                    $form_state['esign_show_popup']= false;
                }
            }
        }else if($triggerElementByName == 'esign-equivalence' || $triggerElementByName == 'set-equivalence'){
            $form_state['esign_show_popup']= true;
            $form_state['storage']['mode'] = 'edit';
            $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
            $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
            $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];

            if (in_array($entityType, $secure_entities)) {
                $uniqueId = "equivalence-".core_encrypt($entityId)."-".$entityType; // URL encryption data to retrive
            } else {
                $uniqueId = "equivalence-".$entityId."-".$entityType; // URL encryption data to retrive
            }
            $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
            if(isset($entityPlugin) && $entityPlugin=='equivalence') {
                if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
                    drupal_set_message(t('ERR106'),'error');
                    $form_state['esign_show_popup']= false;
                }
            }
        }

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
        watchdog_exception('exp_sp_administration_catalog_course_addedit_form_validate', $ex);
        expertusErrorThrow($ex);
    }
}

function addEditCodeValidate(&$form_state,$course_id) {
    try {
        //42771: when we creating course system throws a error message
        $select = db_select("slt_course_template","crs");
        $select->addField('crs','id','crs_id');
        //       $select->addField('crs','title','crs_title');
        //       $select->addField('crs','code','crs_code');
        //       $select->addField('crs','lang_code','lang_code');
        if(!empty($course_id)) {
            $select->condition('crs.id',array($course_id),'NOT IN');
        }
        $select->condition('crs.code',$form_state['values']['crs_code'],'=');
        $select->condition('crs.lang_code',$form_state['values']['crs_language'],'=');
        expDebug::dPrintDBAPI('course code ', $select);
        $crsListCount = $select->execute()->rowCount();
        expDebug::dPrint('Total Count : '.$crsListCount, 4);

        $crs_code_flag   = false;
        if($crsListCount>0){
            $crs_code_flag = true;
        }

        //       foreach($crsList as $crs_list) {
        //        if(strtolower($crs_list->crs_code) == strtolower($crs_code) && $crs_list->lang_code == $form_state['values']['crs_language']) {
        //           $crs_code_flag = true;
        //         }
        //       }

        if($crs_code_flag) {
            form_set_error('crs_code', t('ERR131'));
            $form_state["storage"]["form_validate"] = false;
            $form_state['esign_show_popup']= false;
        }
    } catch (Exception $ex) {
        watchdog_exception('addEditCodeValidate', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 * exp_sp_administration_catalog_course_addedit_form_submit() - Save the data entered by the user in the exp_sp_administration_catalog_course_addedit_form_submit form
 */
function exp_sp_administration_catalog_course_addedit_form_submit($form, &$form_state) {
    global $user;
    try {
        includeFiles();
        $userId = $user->uid;
        $entityType = 'cre_sys_obt_crs';

        $formIdList = array(
        array('id' => 'exp_sp_administration_catalog_course_addedit_form', 'title' => t('Course')),
        array('id' => 'exp_sp_administration_catalog_class_title_form', 'title' => t('LBL353'))
        );
        expDebug::dPrint('exp_sp_administration_catalog_course_addedit_form_submit esign validate ' . $_SESSION['esign_validate'], 4);
        $chkValidation = 1;
        expDebug::dPrint('exp_sp_administration_catalog_course_addedit_form_submit esign validate trigger ' .$form_state['triggering_element']['#name'], 5);
        if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
            $chkValidation = 0;
            if($form_state['hidden-save-submit']==true && $_SESSION['esign_validate'] ==  1){
                $chkValidation = 1;
                $form_state['storage']['submit_triggering_element'] = $form_state['storage']['triggering_button_element'];
            }
            else{
                $form_state['storage']['triggering_button_element'] = $form_state['triggering_element']['#name'];
            }
        }
        else{
            $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
        }
        if($form_state['triggering_element']['#name'] == 'Add custom'){
            $chkValidation = 0;
        }
        if($chkValidation){
            $form_state['esign_validation_status'] = true;
            expDebug::dPrint('exp_sp_administration_catalog_course_addedit_form_submit success validation ' . $form_state['storage']['submit_triggering_element'], 5);
            $trigger =  $form_state['storage']['submit_triggering_element'];
            //  Check whether add or edit
            if(isset($form_state['values']['crs_title'])) {
                if(empty($form_state['values']["crs_id"])) {
                    $loggedInUserId = getIdOfLoggedInUser();
                    // Added by Gayathri for 0070892
                      if(is_null($form_state['values']['price']) || empty($form_state['values']['price'])){
                      $form_state['values']['price'] = '0.00';
                     }                  
                    $returnArr = addNewCourseDetails($form, $form_state,$loggedInUserId);
                    expDebug::dPrint(' $returnArr = ' . print_r($returnArr, true), 5);
                    $newCourseId = $returnArr[0]->id;
                    

                    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
                        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                        saveCustomAttributesInAddEditForm($form, $form_state, $newCourseId, 'cre_sys_obt_crs', $userId, 'Inserted');
                    }else{
                    saveCustomFieldsInAddEditForm($form, $form_state, $newCourseId, 'course', $userId);
                    } //#custom_attribute_0078975 - End Check module status
                    
                    if($form_state['triggering_element']['#name'] == 'saveandpublish'){
                        expertus_set_message(t('MSG601').'.');
                    } else {
                        expertus_set_message(t('LBL272').' '.t('MSG600').'.');
                    }
                    $entityId                            = $form_state['values']['id'];
                    $form_state['storage']['step']       = 'view';
                    $form_state['build_info']['args'][]  = $entityId;
                    $form_state['storage']['new_entity'] = TRUE;

                    $emptyId = $form_state['values']['empty_id'];
                    updateAttachmentInfoOnAddForm($newCourseId,$emptyId);
                    _updateAccessEntity($entityId, $entityType, $emptyId);
                    _updateTagsEntity($entityId, "Course", $emptyId,$trigger);
                    _updateCertificateEntity($entityId, $entityType, $emptyId);

                } else {

                    unset($form_state['storage']['new_entity']);
                    unset($form_state['storage']['form_info_list']);
                    // Added by Gayathri for 0070892 - start
                    $queryStr = "SELECT price,currency_type from slt_course_template where id=:cid";
                    $args = array(':cid' => $form_state['values']['crs_id']);
                    $query = db_query($queryStr, $args);
                    //expDebug::dPrintDBAPI('$query', $query, $args);
                    $defaultcrs = $query->fetchAll();                    
                    if(count($default > 0)) {
                        $defaultcrsprice = $defaultcrs[0]->price;
                        $defaultcrscurrency = $defaultcrs[0]->currency_type;
                    } 
                    // 0070892 - end
                    updateCourseDetails($form, $form_state);
                    // Added by Gayathri for 0070892
                    $classList = getCourseClassIds($form_state['values']['crs_id']);                                
                    
                    
                    if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
                        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                        saveCustomAttributesInAddEditForm($form, $form_state, $form_state['values']['crs_id'], 'cre_sys_obt_crs', $userId, 'Updated');
                    }else{
                    saveCustomFieldsInAddEditForm($form, $form_state, $form_state['values']["crs_id"], 'course', $userId);
                    } //#custom_attribute_0078975 - End Check module status
                    
                    if($form_state['triggering_element']['#name'] == 'saveandpublish' && (count($classList) == 0)){
                        expertus_set_message(t('MSG601').'.');
                    } else if((module_exists('exp_sp_commerce')  == 1) && (count($classList) > 0) && ($defaultcrsprice!=$form_state['values']['price'] || $defaultcrscurrency!=$form_state['values']['currency_type'])){  // changed by Gayathri for 0070892
                        expertus_set_message(t('MSG823').'<br>'.t('LBL272').' '.t('MSG601').'.');
                    } else if($form_state['triggering_element']['#name'] == 'unpublish'){
                        expertus_set_message(t('MSG601').'.');
                    } else {
                        expertus_set_message(t('LBL272').' '.t('MSG601').'.');
                    }

                    $form_state['storage']['step']          = 'edit';
                    $entityId                               = $form_state['values']['crs_id'];
                    $form_state['storage']['edited_entity'] = TRUE;
                }
            }
            else{
                $entityId = $form_state['values']['mapped_course_id'];
            }

            $form_state['build_info']['args'][]       = $entityId;
            $form_state['storage']['form_info_list']  = $formIdList;
            $form_state['display_class']              = TRUE;
            $form_state['show_add_another'] = TRUE;

            $uniqueId = $entityId.'-'.$entityType;

            if(isset($form_state['values']['attr_action']) && isset($entityId)) {
                if($form_state['values']['attr_action'] == 'attachment') {
                    expDebug::dPrint(' Ilayaraja' . $form_state['hidden-save-submit'], 5);
                    attachmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
                }
                elseif($form_state['values']['attr_action'] == 'tag') {
                    tagOperations($form, $form_state, $entityId,'Course', $uniqueId);
                }
                elseif($form_state['values']['attr_action'] == 'prerequisite') {
                    //prerequisiteOperations($form, $form_state, $entityId, $entityType, $uniqueId);
                    addNewPrerequisiteDetails($form, $form_state, $entityId, $entityType);
                }
                elseif($form_state['values']['attr_action'] == 'equivalence') {
                    //prerequisiteOperations($form, $form_state, $entityId, $entityType, $uniqueId);
                    addNewEquivalenceDetails($form, $form_state, $entityId, $entityType);
                }
                elseif($form_state['values']['attr_action'] == 'assessment') {
                    //assessmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
                }
                elseif($form_state['values']['attr_action'] == 'custom' && (!module_exists('exp_sp_administration_customattribute'))) { //#custom_attribute_0078975
                    customfieldsOperations($form, $form_state, $entityId, $entityType, $uniqueId);
                }
                elseif($form_state['values']['attr_action'] == 'notification') {
                    // updateNotificationList($entityId, $entityType, $form_state['values']);
                }
                elseif($form_state['values']['attr_action'] == 'certificate') {
                    updateCertificationList($entityId, $entityType, $form_state['values'],$form_state);
                }
                //drupal_set_message('Course details updated successfully.');
            }
            
            $form_state['rebuild'] = TRUE;
        }
        // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
        //To sync solr
        syncSolrData('Course');
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_catalog_course_addedit_form_submit', $ex);
        expertusErrorThrow($ex);
    }
}

function updateCourseDetails($form, &$form_state, $createUserId,$callFromAPI =0) {

    $txn = db_transaction();

    try {

        if($callFromAPI){
            drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
        }
        $queryStr = "SELECT title,code,short_description,description,lang_code,status,price,currency_type,author_vendor,is_compliance,complete_days,complete_date,validity_days,validity_date from slt_course_template where id=:cid"; // changed by Gayathri for 0070892
        $args = array(':cid' => $form_state['values']['crs_id']);
        $query = db_query($queryStr, $args);
        expDebug::dPrintDBAPI('$query', $query, $args);
        $default = $query->fetchAll();
        expDebug::dPrint('$default : ' . print_r($default, true),4);
        
        
        $trigger_element = $form_state['storage']['submit_triggering_element'];
            /* Update tag activity Count */
            if($callFromAPI){
                $trigger_element = $form_state['values']['crs_status'];
            }
            updateTagActivityCount($form_state['values']['crs_id'],'Course',$trigger_element,$callFromAPI);     
            /* Update tag activity Count */
            
        if($callFromAPI) {
            $updateUserId = $createUserId;
        } else {
            $updateUserId = getIdOfLoggedInUser();
        }
        // Added by Gayathri for 0070892    
        $updateStmt = db_update('slt_course_template');
        $fields     = courseFormatInputString($form, $form_state,$createUserId,'update',$callFromAPI);

        $updateStmt->fields($fields);
        $updateStmt->condition('id', $form_state['values']['crs_id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt);

        // Execute the update statement.
        $numUpdated = $updateStmt->execute();

        // Added by Gayathri for 0070892 - start
        //audit trail for course price
        if($default[0]->price != $form_state['values']['price']) { 
            make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['crs_id'], 'Updated','cre_sys_obt_crs','exp_sp_administration_catalog','updateCourseDetails',
            array('price'));
        }
        if($default[0]->currency_type != $form_state['values']['currency_type']) { 
            make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['crs_id'], 'Updated','cre_sys_obt_crs','exp_sp_administration_catalog','updateCourseDetails',
            array('currency_type'));
        }   
      $classListtoInherit = array();
       $classListcrspriceinherit = getClassIdsforCoursePriceInherit($form_state['values']['crs_id']);
            foreach ($classListcrspriceinherit as $cls) {
                if($default[0]->price==$cls->class_price && $default[0]->currency_type==$cls->class_currency_type) {
                   $classListtoInherit['class_id'][] = $cls->class_id;
                   $classListtoInherit['nid'][] = $cls->nid;
                    make_audit_trail_entries($fields, $updateUserId , $default ,$cls->class_id, 'Updated','cre_sys_obt_cls','exp_sp_administration_catalog','InheritCourseLevelPrice',
                    array('price','currency_type'));
                }
            }
             
        if(count($classListtoInherit) > 0){
            $updatePriceStmt = db_update('slt_course_class');
            $classfields     = array('price' => $form_state['values']['price'],'currency_type' => $form_state['values']['currency_type'],'updated_on'=>now());
            $updatePriceStmt->fields($classfields);
            $updatePriceStmt->condition('id',$classListtoInherit['class_id'],'IN');         
            $updatePriceStmt->execute();    
            
            //Added for #0074022 : update class price to uc_products            
            $ucprodupd = db_update('uc_products');
            $ucprodupdfields     = array('list_price' => $form_state['values']['price'],'cost' =>  $form_state['values']['price'],'sell_price' =>  $form_state['values']['price'], 'currency_type' => $form_state['values']['currency_type']);
            $ucprodupd->fields($ucprodupdfields);
            $ucprodupd->condition('nid',$classListtoInherit['nid'],'IN');   
            expDebug::dPrintDBAPI('$ucprodupd ::::::: ', $ucprodupd,5);     
            $ucprodupd->execute();                      
        } 
        // 0070892 - end
        //Update is_compliance in Class table
        $is_compliance = empty($form_state['values']['course_compliance']) ? null :$form_state['values']['course_compliance'];
        $updateClsStmt = db_update('slt_course_class');
        $clsfields     = array('is_compliance' => $is_compliance,'updated_on'=>now());
        $updateClsStmt->fields($clsfields);
        $updateClsStmt->condition('course_id', $form_state['values']['crs_id']);
        expDebug::dPrintDBAPI('$ucprodupd ::::::: ', $updateClsStmt,5);
        $updateClsStmt->execute();

        /*-- #38557: System allow to set a class Mandatory and compliance issue fix--*/
        $non_compliance = '';
        // remove group mapping entries when course changed to compliance.
        if ($is_compliance) {
            $updateGrpAccessFields = array('mro' => null,'updated_on'=>now());
            $upd_qry = db_query("update slt_enrollment set mandatory=null, is_compliance=1 where course_id=:course_id", array(":course_id"=>$form_state['values']['crs_id']));
        }
        else{
            
            $updateGrpAccessFields = array('mro' => 'cre_sys_inv_opt','updated_on'=>now());
            $non_compliance = 1;
            $upd_qry = db_query("update slt_enrollment set is_compliance=null where course_id=:course_id", array(":course_id"=>$form_state['values']['crs_id']));
        }
        
        //Common update for compliance and non-compliance
        // remove access at course level
        $updateGrpAccess = db_update('slt_group_mapping');
        $updateGrpAccess->fields($updateGrpAccessFields);
        $updateGrpAccess->condition('entity_id', $form_state['values']['crs_id']);
        $updateGrpAccess->condition('entity_type', 'cre_sys_obt_crs');
        if($non_compliance)
            $updateGrpAccess->condition('mro', '','IS NULL');
        //expDebug::dPrintDBAPI('$$updateGrpAccess', $updateGrpAccess);
        $updateGrpAccess->execute();
            
        $updateGrpAccess = db_update('slt_user_access_mapping');
        $updateGrpAccess->fields($updateGrpAccessFields);
        $updateGrpAccess->condition('entity_id', $form_state['values']['crs_id']);
        $updateGrpAccess->condition('entity_type', 'cre_sys_obt_crs');
        if($non_compliance)
            $updateGrpAccess->condition('mro', '','IS NULL');
        //expDebug::dPrintDBAPI('$$updateGrpAccess', $updateGrpAccess);
        $updateGrpAccess->execute();
        
        $classList = getCourseClassIds( $form_state['values']['crs_id']);
        
        
        // remove access at class level
        foreach ($classList as $class) {
            if($non_compliance)
                $updateGrpAccessClsFields = array('mro' => 'cre_sys_inv_opt','updated_on'=>now());
            else
                $updateGrpAccessClsFields = array('mro' => null,'updated_on'=>now());
            
            $updateGrpAccessCls = db_update('slt_group_mapping');
            $updateGrpAccessCls->fields($updateGrpAccessClsFields);
            $updateGrpAccessCls->condition('entity_id', $class->class_id);
            $updateGrpAccessCls->condition('entity_type', 'cre_sys_obt_cls');
            if($non_compliance)
                $updateGrpAccessCls->condition('mro', '','IS NULL');
            expDebug::dPrintDBAPI('$$updateGrpAccess', $updateGrpAccessCls);
            $updateGrpAccessCls->execute();
        
            $updateGrpAccessCls = db_update('slt_user_access_mapping');
            $updateGrpAccessCls->fields($updateGrpAccessClsFields);
            $updateGrpAccessCls->condition('entity_id', $class->class_id);
            $updateGrpAccessCls->condition('entity_type', 'cre_sys_obt_cls');
            if($non_compliance)
                $updateGrpAccessCls->condition('mro', '','IS NULL');
            expDebug::dPrintDBAPI('$$updateGrpAccess', $updateGrpAccessCls);
            $updateGrpAccessCls->execute();
        
        }
        
        
        if($default[0]->is_compliance && $is_compliance == null){
            $updateClsStmt = db_update('slt_enrollment');
            $enrfields     = array('is_compliance' => null);
            $updateClsStmt->fields($enrfields);
            $updateClsStmt->condition('is_compliance', 1,'=');
            $updateClsStmt->condition('course_id', $form_state['values']['crs_id']);
            expDebug::dPrintDBAPI('update query for set is_compliance as null', $updateClsStmt);
            $updateClsStmt->execute();
        }else if($default[0]->is_compliance == null && $is_compliance){
            $updateClsStmt = db_update('slt_enrollment');
            $enrfields     = array('is_compliance' => 1);
            $updateClsStmt->fields($enrfields);
            $updateClsStmt->condition('is_compliance', '','IS NULL');
            $updateClsStmt->condition('course_id', $form_state['values']['crs_id']);
            expDebug::dPrintDBAPI('update query for set is_compliance as 1', $updateClsStmt);
            $updateClsStmt->execute();
        }

        if($trigger_element == 'unpublish'){
            make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['crs_id'], 'Updated','cre_sys_obt_crs','exp_sp_administration_catalog','updateCourseDetails',
            array('status'));

        }
        else{
            make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['crs_id'], 'Updated','cre_sys_obt_crs','exp_sp_administration_catalog','updateCourseDetails',
            array('title', 'code', 'short_description', 'description', 'lang_code','status','author_vendor','is_compliance','complete_days', 'complete_date', 'validity_days', 'validity_date'));
        }

        $fields["id"]           = $form_state['values']['crs_id'];
        $fields["entity_type"]  = 'cre_sys_obt_crs';
        $operation              = 'update';
        drupalNodeTaxonomySave($fields,$operation);
        if($callFromAPI){
            return array((object)array('id'=>$form_state['values']['crs_id']));
        }
    }
    catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('updateCourseDetails', $ex);
        if($callFromAPI){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Unable to update a course.";
            return $errobj;
        }
        else {
            return array((object)array('id'=>'Failiure'));
        }
        unset($txn);
        expertusErrorThrow($ex);
        // throw $ex;
    }
    syncSolrData('Enrollment');
    // Commit the transaction
    unset($txn);

}

function addNewCourseDetails($form, &$form_state, $createUserId,$callFromAPI =0) {

    $txn = db_transaction();

    try {
        if($callFromAPI){
            drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
        }
        $insertStmt     = db_insert('slt_course_template');
        $fields         = courseFormatInputString($form, $form_state,$createUserId,'insert');
        $insertStmt->fields($fields);
        expDebug::dPrintDBAPI('$insertStmt', $insertStmt);

        $crsId          = $insertStmt->execute();
        expDebug::dPrint(' $crsId = ' . $crsId, 3);
        if($callFromAPI) {
            $createUser = $createUserId;
        }
        else {
            $createUser = getIdOfLoggedInUser();
        }
        updateGroupMappingDetails($crsId,'cre_sys_obt_crs', $createUser,'emptyadd',0);
        make_audit_trail_entries($fields, $createUser , null ,$crsId, 'Inserted','cre_sys_obt_crs','exp_sp_administration_catalog','addNewCourseDetails',
        array('title', 'code', 'short_description', 'description', 'lang_code','status','price','currency_type','author_vendor','is_compliance','complete_days', 'complete_date', 'validity_days', 'validity_date'));
                        // Save the new organization's id in the hidden id form field.
        $form_state['values']['id']     = $crsId;
        $form_state['values']["crs_id"] = $crsId;
        $fields["id"]                   = $crsId;
        $fields["entity_type"]          = 'cre_sys_obt_crs';
        $operation                      = 'insert';

        drupalNodeTaxonomySave($fields,$operation);

        return array((object)array('id'=>$crsId));
    } catch (Exception $ex) {
        expDebug::dPrint('exception in addNewCourseDetails'.print_r($ex->getMessage(), 1), 1);
        $txn->rollback();
        watchdog_exception('addNewCourseDetails', $ex);
        if($callFromAPI){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Unable to create course id.";
            return $errobj;
        }
        else {
            return array((object)array('id'=>'Failiure'));
        }
        unset($txn);
        expertusErrorThrow($ex);
        // throw $ex;
    }

    // Commit the transaction
    unset($txn);
}

function courseFormatInputString($form, &$form_state,$createUserId,$db_action,$callFromAPI) {
    try {
        $is_blended            = 0;
        $price                 = NULL;
        $currency_type    = NULL;
        $training_units        = NULL;
        $cancellation_fee      = NULL;
        $late_cancellation_fee = NULL;
        $no_show_fee           = NULL;

        //$inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
        $in_active_on          = $form_state['values']['crs_status'] == 'lrn_crs_sts_itv' ? now() : NULL;
        //$in_active_reason      = $form_state['values']['crs_status'] == 'lrn_crs_sts_itv' ? $inactivereasontext : NULL;
        $custom                = NULL;
        $completeDate=null;
        $complianceCompleted = NULL;
        if(!empty($form_state['values']['completed_date']) && ($form_state['values']['completed_date'] != t('LBL112'))){
            $oldCompDatet = str_replace("-","/",$form_state['values']['completed_date']);
            $completeDate = date_format(date_create( $oldCompDatet),'Y-m-d H:i');
            $complianceCompleted = 'date';
        }
        $validityDate=null;
        if(!empty($form_state['values']['validity_date']) && ($form_state['values']['validity_date'] != t('LBL112'))){
            $oldValidityDate = str_replace("-","/",$form_state['values']['validity_date']);
            $validityDate = date_format(date_create( $oldValidityDate),'Y-m-d H:i');
        }
        $trigger_element = $form_state['storage']['submit_triggering_element'];
        if($trigger_element == 'saveandpublish'){
            $crs_status = 'lrn_crs_sts_atv';
        }
        else if($trigger_element == 'save'){
            $crs_status = 'lrn_crs_sts_itv';
        }
        else if($form_state['values']['crs_status']){
            $crs_status =$form_state['values']['crs_status'];
        }
        if ($form_state['values']['compliance_completed'] == 'hire_days') {
            $completeDays = empty($form_state['values']['completed_hire_days']) ? null :$form_state['values']['completed_hire_days'];
            $complianceCompleted = ($completeDays > 0 && $completeDays!= '') ? 'hire_days' : null;
        } elseif($form_state['values']['compliance_completed'] == 'days'){
            $completeDays = empty($form_state['values']['completed_days']) ? null :$form_state['values']['completed_days'];
            $complianceCompleted = ($completeDays > 0 && $completeDays != '') ? 'days' : null;
        }
        $lang_code =$form_state['values']['crs_language'];
        if(($callFromAPI)&& $crs_status=='lrn_crs_sts_del' ){
            $lang_code= ($form_state['values']['crs_language'].'_deleted_'.$form_state['values']['crs_id']) ;
        }
        
        $fields = array(
                  'title'              => $form_state['values']['crs_title'],
                  'code'               => $form_state['values']['crs_code'],
                  'course_type'          => NULL,
                  'lang_code'          => $lang_code,
                  'short_description'  => $form_state['values']['crs_short_description']['value'],
        //'description'        => $form_state['values']['crs_short_description']['value'],
                  'status'             => $crs_status,
                  'is_blended'         => $is_blended,
                  'price'              => $form_state['values']['price'],
                  'currency_type'      => $form_state['values']['currency_type'],
                  'training_units'     => $training_units,
                  'cancellation_fee'   => $cancellation_fee,
                  'late_cancellation_fee' => $late_cancellation_fee,
                  'no_show_fee'        => $no_show_fee,
                  'in_active_on'       => $in_active_on,
        //'in_active_reason'   => $in_active_reason,
                  'author_vendor'      => empty($form_state['values']['crs_author_vendor']) ? null :$form_state['values']['crs_author_vendor'],
        //'additional_info'  => empty($form_state['values']['crs_additional_info']) ? null :$form_state['values']['crs_additional_info'],
                  'is_compliance'      => empty($form_state['values']['course_compliance']) ? null :$form_state['values']['course_compliance'],
                  'complete_days'      => $completeDays,
                  'complete_date'      => $completeDate,
                  'validity_days'      => empty($form_state['values']['validity_days']) ? null :$form_state['values']['validity_days'],
                  'validity_date'      => $validityDate,
                  'compliance_completed'    => $complianceCompleted,    
                  'updated_on'         => now(),
                  'custom0'            => $custom,
                  'custom1'            => $custom,
                  'custom2'            => $custom,
                  'custom3'            => $custom,
                  'custom4'            => $custom,
        );
     $fields = trimInputFields($fields); // #42124 - Trim input values

     if ($db_action == 'insert') {
        $fields['created_by'] = $createUserId;
        $fields['created_on'] = now();
        $fields['updated_by'] = $createUserId;
        $fields['updated_on'] = now();
     }
     else {
        $fields['updated_by'] = $callFromAPI ? $createUserId : getIdOfLoggedInUser();
        $fields['updated_on'] = now();
     }


     if($trigger_element == 'unpublish'){
        $crs_status = 'lrn_crs_sts_itv';
        $fields = array( 'status'  => $crs_status ,'updated_on' => now());
     }
     if($trigger_element == 'saveandpublish'){
        $fields['published_on'] = now();
        $fields['updated_on']   = now();
     }
     return $fields;
    } catch (Exception $ex) {
        watchdog_exception('courseFormatInputString', $ex);
        expertusErrorThrow($ex);
    }
}


function drupalNodeTaxonomySave($inObj,$operation) {

    global $user;

    $userid     = $user->uid;
    $txn = db_transaction();
    try {
        $oNode  = new StdClass();

        if($operation == 'update') {
            $select = db_select("slt_node_learning_activity","entity");
            $select->innerJoin('node','n','entity.node_id=n.nid');
            $select->addField('n','nid','nid');
            $select->addField('n','vid','vid');
            $select->condition('entity.entity_id',$inObj["id"],'=');
            $select->condition('entity.entity_type',$inObj["entity_type"],'=');
            $crsList = $select->execute()->fetchAll();
            $oNode->nid=isset($crsList[0]->nid)?$crsList[0]->nid:'';
            $oNode->vid=isset($crsList[0]->vid)?$crsList[0]->vid:'';
        }

        $oNode->uid       =  $userid;
        $oNode->language  = 'en';
        $oNode->title     = str_replace("''","'",$inObj["title"]);
        $oNode->body      = $inObj["description"];
        $oNode->type      = 'cre_sys_obt_crs';
        $oNode->created   = time();
        $oNode->changed   = time();
        $oNode->teaser_include = 1;
        $oNode->revision  = ($operation == 'update')? '' : 1;
        $oNode->teaser     = $inObj["description"];
        $oNode->format    = 1;
        $oNode->log       = '';
        $oNode->name      = $userid ;
        $oNode->date      = '';
        $oNode->status    = ($inObj["status"] == 'lrn_crs_sts_atv') ? 1 : 0;
        $oNode->promote   = 0;
        $oNode->sticky    = 0;
        $oNode->op        = "Save";

        node_save($oNode);

        if($operation == 'insert') {
            $vNodeId               =  $oNode->nid;
            $formInfo["node_id"]   = $vNodeId;
            $formInfo["entity_id"] = $inObj["id"];
            update_node_learning_activity($formInfo);
        }


        /* -----  Forum Integration --------------*/

        $forumNode  = new StdClass();

        if($operation == 'update') {
            $selectNodeForum = db_select("slt_node_learning_activity","entity");
            $selectNodeForum->innerJoin('node','n','entity.node_id=n.nid');
            $selectNodeForum->addField('n','nid','nid');
            $selectNodeForum->addField('n','vid','vid');
            $selectNodeForum->condition('entity.entity_id',$inObj["id"],'=');
            $selectNodeForum->condition('entity.entity_type','Course');
            $selectNodeForum->condition('entity.custom0', 'forum');
            $crsList = $selectNodeForum->execute()->fetchAll();
            $forumNode->nid=isset($crsList[0]->nid)?$crsList[0]->nid:'';
            $forumNode->vid=isset($crsList[0]->vid)?$crsList[0]->vid:'';
        }

        $forumNode->uid       =  $userid;
        $forumNode->language  = 'en';
        $forumNode->title     = str_replace("''","'",$inObj["title"]);
        $forumNode->body      = $inObj["description"];
        $forumNode->type      = 'forum';
        $forumNode->created   = time();
        $forumNode->changed   = time();
        $forumNode->teaser_include = 1;
        $forumNode->revision  = ($operation == 'update')? '' : 1;
        $forumNode->teaser     = $inObj["description"];
        $forumNode->format    = 1;
        $forumNode->log       = '';
        $forumNode->name      = $userid ;
        $forumNode->date      = '';
        $forumNode->status    = ($inObj["status"] == 'lrn_crs_sts_atv') ? 1 : 0;
        $forumNode->promote   = 0;
        $forumNode->sticky    = 0;
        $forumNode->op        = "Save";

        node_save($forumNode);

        // insert/update record into the taxonomy tables
        $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
        $forumTerms               = new stdClass;
        $forumTerms->name         = $inObj["title"];
        $forumTerms->vid          = $forumvid;
        $forumTerms->description  = "Course";//$description;
        $forumTerms->format       = $inObj["id"]; //courseid

        if($operation=='insert') {
            taxonomy_term_save($forumTerms);
            $forumNodeId = $forumNode->nid;

            //to insert record into the slt_node_learning_activity
            $nodeInsertForum = db_insert('slt_node_learning_activity');
            $nodeFieldsForum = array(
                      'node_id' => $forumNodeId,
                      'entity_id' => $inObj["id"],  // courseid
                      'entity_type' => "Course",
                      'createdby' => $userid,
                      'createdon' => now(),
                                    'updatedby' => $userid,
                                    'updatedon' => now(),
                      'custom0' => 'forum'
                      );
                      $nodeInsertForum->fields($nodeFieldsForum);
                      $nodeInsertForum->execute();

        }else{
            $forum_tid = db_query("select tid FROM taxonomy_term_data WHERE description = 'Course' AND format ='".$inObj["id"]."' AND vid = ".$forumvid)->fetchField();
            if(!empty($forum_tid)){
                $forumTerms->tid = $forum_tid;
            }
            taxonomy_term_save($forumTerms);
        }

        /*------- Forum Integration End ---------------------*/

    } catch(Exception $e) {
        //throw new SoapFault("SPLMS",$e->getMessage());
        $txn->rollback();
        unset($txn);
        watchdog_exception('drupalNodeTaxonomySave', $ex);
        expertusErrorThrow($ex);
    }
    unset($txn);
}


function adminCourseVocabTerms($termsTxt) {
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
        watchdog_exception('adminCourseVocabTerms', $ex);
        expertusErrorThrow($ex);
    }
}

function update_node_learning_activity($formInfo) {

    global $user;

    $userid  = $user->uid;
    $txn     = db_transaction();

    try {
        $insertStmt = db_insert('slt_node_learning_activity');
        $fields     = array(
                'node_id'           => $formInfo['node_id'],
                'entity_id'         => $formInfo['entity_id'],
                'entity_type'       => 'cre_sys_obt_crs',
                'createdby'         => $userid,
                'createdon'         => now(),
                            'updatedby'         => $userid,
                            'updatedon'         => now(),
        );

        $insertStmt->fields($fields);
        $crsId    = $insertStmt->execute();
        //return array((object)array('id'=>$crsId));
    } catch (Exception $ex) {

        $txn->rollback();
        watchdog_exception('update_node_learning_activity', $ex);
        unset($txn);
        expertusErrorThrow($ex);
        //throw $ex;
    }

    // Commit the transaction
    unset($txn);
}

/*
 * addEditFormWithCourseCancelAndSaveClose()
 */
function addEditFormWithCourseCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $courseId, $subStage = '', $emptyId = '') {
    global $theme_key;
    try {
        expDebug::dPrint(' addEditFormWithCancelAndSaveClose() called.');

        $customHiddenButtonId = 'last-button-account';

        $esignHiddenButt = 'esign-'.$wrapperId;

        // Remove the temporary storage for API's to exchange information during form creation
        unset($form['addedit_tmp_storage']);

        // Add the Cancel button
        $ajaxCallbackCancel = 'addEditCancelRepaintForm';

        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

        // For common styling of all addedit cancel buttons
        //$cancelAttributes['class'][] = 'addedit-edit-cancel';

        // Using this class to applying bg color to the save button
        $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';

        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $cancelAttributes['data-wrapperid'] = array($wrapperId);

        // A common class for all addedit form cancel button, for styling to appear as a link.
        $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

        if(empty($courseId)){
            $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
            $cancelLabel  = t('LBL123'); // Close
        }else{
            $cancelLabel  = t('LBL109'); // Cancel
        }

        $cancelAttributes['tabindex'][] = '8';
        $courseStatus     = '';
        $CourseDetails;
        $courseStatus;
        $ifCourseValid;
        $resultCnt;
        $ifCourseUnpublish;
        $isMultiAction=0;
        if($courseId){
            if($subStage == 'details' || empty($subStage)) {
                if (!empty($courseId)) {
                    $CourseDetails   = getCourseDetail($courseId, array('id,status,title'));
                    $courseStatus    =  $CourseDetails[0]->crs_status;
                }
                $ifCourseValid = getIfCourseValid($courseId);
                $resultCnt = delete_object_record($courseId,'Course');
                $ifCourseUnpublish = getIfCourseBeUnpublished($courseId);
                if($courseStatus != 'lrn_crs_sts_atv'){
                    $isMultiAction=1;
                }
                if($ifCourseUnpublish == 0){
                    $isMultiAction=1;
                }
                if($resultCnt == 0){
                    $isMultiAction=1;
                }
            }
        }
        $addclass = ($theme_key=='expertusoneV2')?'':($isMultiAction==1)?'resize-save_btn4':'resize-save_btn';
        $form[$cancelButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row enroll-edit-save-cancel-btn">' .
                       '<div class="addedit-form-cancel-container-actions course-addedit-actions '.$addclass.'"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
        '#value' => $cancelLabel,
        '#name' => 'cancelcourse',
        '#attributes' => $cancelAttributes,
        );

        if (!empty($courseId)) { // Cancel button instead of Close is shown only when $courseId is not empty (see above). Cancel button is ajax enabled.
            $form[$cancelButtonId]['#ajax'] = array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackCancel,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=>'click',
          'keypress' => true,
          'progress' => array(
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
        //$saveAttributes['class'][] = 'edit-catalog-course-save-publish';


        //Increase the width of the popup while clicking on save button
        $saveAttributes['onclick'][] = 'increasePopWidth();';

        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid'] = array($wrapperId);
        $saveAttributes['tabindex'][] = '7';
        $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;
        /*
         if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
         $saveAttributes['onclick'] = array();
         //$saveAttributes['onclick'][] = 'hideMessageInfo();';
         $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
         $saveAttributes['onclick'][] = 'increasePopWidth();';
         $form[$saveButtonId] = array(
         '#type' => 'submit',
         '#value' => t('LBL141'),
         '#name' => 'save',
         '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
         '#suffix' =>
         '<div class="admin-save-button-right-bg"></div></div>' .
         '<div class="clearBoth"></div></div>' .
         '<div class="clearBoth"></div></div>',
         '#id' =>$saveButtonId,
         '#attributes' => $saveAttributes,
         '#ajax' => array(
         'wrapper' => $wrapperId,
         'callback' => $ajaxCallbackSave,
         'method' => 'replace',
         'effect' => 'none', // 'fade',
         'event'=>'click',
         'customButtonId'=>$customHiddenButtonId,
         'keypress' => true,
         'progress' => array(
         'type' => 'throbber',
         'message' => ''
         ),
         )
         );

         }
         */

        if($subStage == 'details' || empty($subStage)) {
            /*$courseStatus     = '';
             if (!empty($courseId)) {
             $CourseDetails   = getCourseDetail($courseId, array('id,status,title'));
             $courseStatus    =  $CourseDetails[0]->crs_status;
             }
             if($courseId){
             $ifCourseValid = getIfCourseValid($courseId);
             }*/
            $isSaveParent = 0;
            if(empty($courseId) || $courseStatus != 'lrn_crs_sts_atv'){

                $isSaveParent = 1;
                $saveAttributes['onclick'] = array();
                //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                $saveAttributes['onclick'][] = 'increasePopWidth();';
                $saveAttributes['class'][] = (($courseId) ? 'admin-save-button-dropdown-middle-bg-wrapper' : '');
                // $saveAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
                $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
                // Commented For Course save and Publish By Default #0036253.
                // '#suffix' => (($courseId) ? '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>' : '<div class="admin-save-button-right-bg"></div>').'</div>',
            '#suffix' => ( '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>').'</div>',
            '#value' => t('LBL141'),
            '#name' => 'save',
            '#attributes' => $saveAttributes,
            '#ajax' => array(
              'wrapper' => $wrapperId,
              'callback' => $ajaxCallbackSave,
              'customButtonId'=>$customHiddenButtonId,
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


            /*if($courseId){
             $resultCnt = delete_object_record($courseId,'Course');
             $ifCourseUnpublish = getIfCourseBeUnpublished($courseId);
             }*/
            // if($courseId){ // course Id is Commented For Course save and Publish By Default #0036253.
            expDebug::dPrint('$isSaveParent : '.$isSaveParent , 4);
            if($isSaveParent){
                $form['addedit-button-group-ul-start'] = array(
                '#type' => 'markup',
                '#markup' => '<ul class="catalog-pub-add-list crs-pub-save-btn">',
                ); //  catalog-save-btn
                $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
                $savePublishSuffix = '</li>';
            }else{
                //$savePublishAttributes['class'][] = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('admin-save-button-dropdown-middle-bg-wrapper') : '');
                $savePublishAttributes['class'][] = 'admin-save-button-dropdown-middle-bg-wrapper';
                $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
                //$savePublishSuffix = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';
                $savePublishSuffix = '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>'.'</div>';

            }

            // The If has been removed. refer #0029450
            // if($ifCourseValid[0]->sesavailable != 0){
            $saveButtonId = 'catalog-course-save-publish';
            $savePublishAttributes['onclick'] = array();
            $savePublishAttributes['tabindex'][] = '7';

            // $savePublishAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
            $savePublishAttributes['onclick'][] = 'increasePopWidth();';
            $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $savePublishAttributes['data-wrapperid'] = array($wrapperId);
            // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
            $buttonLabel = ($courseStatus == 'lrn_crs_sts_atv') ? t('LBL141') : t('LBL614');
            //$savePublishAttributes['class'][] = (($resultCnt==0) && ($courseStatus == 'lrn_crs_sts_atv')) ? 'admin-save-button-middle-bg' : 'pub-unpub-only-save-btn';
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
                  'customButtonId'=>$customHiddenButtonId,
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
                    // }
                    //  }
                    // resource-add-action-wrapper
                    //if($isSaveParent){
                    /*
                     $form['addedit-button-group-ul-start-ext'] = array(
                     '#type' => 'markup',
                     //'#markup' => '<span onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper">Exp</span>',
                     '#markup' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">Exp</span>',
                     );
                     */
                        //}

                        // resource-add-action-wrapper

                        if(!$isSaveParent){
                            $form['addedit-button-group-ul-start'] = array(
            '#type' => 'markup',
            '#markup' => '<ul class="catalog-pub-add-list crs-pub-save-btn">',
                            );
                        }

                        // <span onclick="displayPubActionList()" class="resource-add-action-wrapper">


                        if(($courseId) && $courseStatus == 'lrn_crs_sts_atv'){
                            if($ifCourseUnpublish == 0){
                                $saveButtonId = 'catalog-course-save-unpublish';
                                $saveUnPublishAttributes['onclick'] = array();
                                //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                                $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
                                // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
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
                  'customButtonId'=>$customHiddenButtonId,
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

                        if(!$isSaveParent){
                            /*$form['addedit-button-group-ul-start-extend'] = array(
                             '#type' => 'markup',
                             '#markup' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>',
                             );*/
                        }

                        $deleteCheck = adminVisibilityForAddEditDelete($courseId,'cre_sys_obt_crs','cre_sys_obt_crs','slt_course_template');
                        if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
                            if(($courseId)){
                                if($resultCnt == 0){
                                    $displayTitle = titleController(' ', $CourseDetails[0]->crs_title,34);
                                    $saveButtonId = 'catalog-course-delete';

                                    $deleteAttributes['onclick'] = array();
                                    $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('Course')).' '.escape_string($CourseDetails[0]->crs_title) . '","'.core_encrypt($courseId).'","Course"); return false;';
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
          '#markup' => '</ul><div class="clearBoth"></div></div>' .
                     '<div class="clearBoth"></div></div>',
                        );
                    }
                    else{
                        $saveAttributes['onclick'] = array();
                        //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                        $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
                        $saveAttributes['onclick'][] = 'increasePopWidth();';
                        $form[$saveButtonId] = array(
        '#type' => 'submit',
        '#value' => t('LBL141'),
        '#name' => 'save',
        '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
        '#suffix' =>
                 '<div class="admin-save-button-right-bg"></div></div>' .
                 '<div class="clearBoth"></div></div>' .
               '<div class="clearBoth"></div></div>',
        '#id' =>$saveButtonId,
        '#attributes' => $saveAttributes,
        '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'method' => 'replace',
          'effect' => 'none', // 'fade',
          'event'=>'click',
              'customButtonId'=>$customHiddenButtonId,
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
            ),
            )
            );

                    }
                    unset($saveAttributes['tabindex']);
                    $saveAttributes['onclick'] = array();
                    //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                    $saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
                    $saveAttributes['onclick'][] = 'increasePopWidth();';
                    $form[$customHiddenButtonId] = array(
              '#type' => 'submit',
              '#value' => t('hidden-save'),
              '#name'   => 'hidden-save',
              '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
              '#suffix' => '</span>',
              '#id' =>$customHiddenButtonId,
              '#attributes' => $saveAttributes,
              '#ajax' => array(
                  'wrapper' => $wrapperId,
                  'callback' => 'addEditRepaintForm',
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

                    //administration/catalogaccess
                    // $("#root-admin").data("narrowsearch").getCatalogVisibleDetails();

                    /*$form['addedit-left-col-extend-func'] = array(
                     '#type' => 'markup',
                     '#markup' => '<div><div>Attachments</div><div>Tags</div><div>Certificate</div>
                     <div id="catalog_det_visible_popup" onmouseover =\'$("#root-admin").data("narrowsearch").getCatalogVisibleDetails('.$entityId.',"'.$entityType.'")\'>Access</div>
                     </div>',
                     );*/
                    // catalog_det_visible_popup
                    $entityId            = core_encrypt($courseId);//.',"'.$entityType
                    if(!empty($emptyId)){
                        $entityId = 0;
                    } else {
                        $emptyId = 0;
                    }
                    $tagsentityId = $entityId;
                    $tagsentityType    = 'Course';
                    $qtipTagsIdInit          = $entityId.'_'.$tagsentityType;
                    $tagsvisibPopupId  = 'qtip_visible_disp_tags_'.$qtipTagsIdInit;


                    $entityType          = 'cre_sys_obt_crs';
                    $qtipIdInit          = $entityId.'_'.$entityType;

                    global $theme_key;
                    if($theme_key == 'expertusoneV2') {
                        $qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'Course',
                                                          'url':'administration/catalog-tags/ajax/".$entityId."/".$tagsentityType."/".$emptyId."',
                                                          'popupDispId':'".$tagsvisibPopupId."',
                                                          'catalogVisibleId':'renderTagsId".$qtipTagsIdInit."',
                                                          'wid':400,'heg':'170','postype':'middle','poslwid':'','qdis':'ctool','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
                        $qtipOptAccessObj  = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_crs',
                                                                  'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."',
                                                                  'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
                                                                  'wid':685,'heg':'270','postype':'middle','poslwid':'200','qdis':'ctool','linkid':'visible-course-".$entityId."'}";
                    } else {
                        $qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'Course',
                                                          'url':'administration/catalog-tags/ajax/".$entityId."/".$tagsentityType."/".$emptyId."',
                                                          'popupDispId':'".$tagsvisibPopupId."',
                                                          'catalogVisibleId':'renderTagsId".$qtipTagsIdInit."',
                                                           'wid':400,'heg':'130','postype':'middle','poslwid':'','qdis':'ctool','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
                        $qtipOptAccessObj  = "{'entityId':'".$entityId."',
                                                                  'entityType':'cre_sys_obt_crs',
                                                                  'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."',
                                                                  'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
                                                                  'wid':650,'heg':'270','postype':'middle','poslwid':'200','qdis':'ctool','linkid':'visible-course-".$entityId."'}";
                    }

                    $dummy_entity_id       = empty($entityId) ? core_encrypt(ereg_replace(" ", "",$emptyId)) : $entityId;
                   // $dummy_entity_id = core_encrypt($dummy_entity_id); // URL encryption
                    $qtipOptAttachmentObj  = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_crs',
                                                              'url':'administration/catalog-attachment/".$dummy_entity_id."/".$entityType."',
                                                              'popupDispId':'qtip_attachment_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachIdqtip_visible_disp_".$qtipIdInit."',
                                                              'wid':570,'heg':'170','postype':'bottomleft','poslwid':'','qdis':'ctool','linkid':'visible-course-attachment-".$entityId."','scrollid':'scrolldiv'}";

                    $visibCertificatePopupId  = 'qtip_visible_certificatedisp_'.$entityId.'_'.$entityType;
                    $qtipCertOptObj          = "{'entityId':'".$entityId."',
                                                                  'entityType':'cre_sys_obt_crs',
                                                                  'url':'administration/print-certificate/".$entityId."/".$entityType."/".$emptyId."',
                                                                  'popupDispId':'qtip_visible_certificatedisp_".$entityId.'_'.$entityType."','catalogVisibleId':'renderPrintCerId',
                                                                  'wid':376,'heg':'120','postype':'middle','poslwid':'','qdis':'','linkid':'visible-certificate-".$entityId."', 'scrollid': 'certificate-display-table'}";
                    /*
                     $form['addedit-left-col-extend-func'] = array(
                     '#type' => 'markup',
                     '#markup' => '<div class="crs-tab-titles-container"><div class="attachment-tab-icon"></div>'.
                     "<div id='qtip_attachment_disp_".$qtipIdInit."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAttachmentObj.");\"class=\"tab-title\">Attachments</div><div class=\"tab-seperator\"></div><div class=\"tags-tab-icon\"></div>".
                     "<div id='".$tagsvisibPopupId."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipTagsOptObj.");\" class=\"tab-title\">Tags</div><div class=\"tab-seperator\"></div>".
                     "<div class='certificate-tab-icon'></div><div id='".$visibCertificatePopupId."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipCertOptObj.");\" class=\"tab-title\">Certificate</div>".
                     '<div class="tab-seperator"></div><div class="access-tab-icon"></div>'.
                        "<div id='qtip_visible_disp_".$qtipIdInit."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAccessObj.");\" class=\"tab-title\">Access</div>".
                        '</div>',
                        );*/

                    //Temporary Hide
                    $form['addedit-left-col-extend-func'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="crs-tab-titles-container"><div class="attachment-tab-icon"></div>'.
                        "<div id='qtip_attachment_disp_".$qtipIdInit."'><a class=\"tab-title\" id='visible-course-attachment-".$entityId."' onclick = \"callVisibility(".$qtipOptAttachmentObj.");\">".t('LBL231')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div><div class=\"tab-seperator\"></div><div class=\"tags-tab-icon\"></div>".
                        "<div id='".$tagsvisibPopupId."'><a id='visible-tags-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipTagsOptObj."); \">".t('LBL191')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div><div class=\"tab-seperator\"></div>".
                         "<div class='certificate-tab-icon'></div><div id='".$visibCertificatePopupId."' class=multiple-certificate-display><a  id='visible-certificate-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipCertOptObj.");\">".t('LBL205')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
                        '<div class="tab-seperator"></div><div class="access-tab-icon"></div>'.
                        "<div class='access-zindex-change' id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-course-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptAccessObj.");\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
                      '</div>',
                    );

                    /*$ajaxEvent = 'load';
                     $editAttributes['onload'][] = "$(document).ready(function(){
                     $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAttachmentObj.");
                     $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipTagsOptObj.");
                     $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipCertOptObj.");
                     $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAccessObj.");
                     });";*/

                    $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';

                    /*$form['edit-class-list-button-'] = array(
                     '#type' => 'image_button', '#src' => $pngPath,
                     '#name' => 'edit-class-',
                     '#prefix' => '<div style="display:none;">',
                     '#suffix' => '</div>',
                     '#attributes' => $editAttributes,
                     '#ajax' => array(
                     'wrapper' => $wrapperId,
                     'event' => $ajaxEvent,
                     ),
                     );*/


                    $form['addedit-left-col-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="clearBoth"></div>',
                    /*'#prefix' => '<div style="display:none;"><img id= "edit-course-img" src="'.$pngPath.'" onload = "$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$qtipOptAttachmentObj.');
                     $(\'#root-admin\').data(\'narrowsearch\').getBubblePopup('.$qtipTagsOptObj.');
                     $(\'#root-admin\').data(\'narrowsearch\').getBubblePopup('.$qtipCertOptObj.');
                     $(\'#root-admin\').data(\'narrowsearch\').getBubblePopup('.$qtipOptAccessObj.');"></div>',*/
                    );
        } catch (Exception $ex) {
            watchdog_exception('addEditFormWithCourseCancelAndSaveClose', $ex);
            expertusErrorThrow($ex);
        }
    }

    function getEnrolledComplianceCourse($courseId){
        try {
            $select = db_select('slt_enrollment', 'enroll');
            $select->addExpression('COUNT(1)','total_enroll');
            $select->condition('enroll.course_id', $courseId, '=');
            $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
            //$select->condition('enroll.comp_status', 'lrn_crs_cmp_cmp' ,'<>');
            $select->condition('enroll.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_enr'), 'IN');
            //$select->condition('enroll.is_compliance', 1 ,'=');
            $total_enroll = $select->execute()->fetchField();

            return $total_enroll;
        } catch (Exception $ex) {
            watchdog_exception('getEnrolledComplianceCourse', $ex);
            expertusErrorThrow($ex);
        }
    }

    function addNewCourseByRestAPI( &$form_state,$createuserid="")
    {
        try {
            $form=array();
            $error_messages = array();
            $form["form_id"]="";
            if(($form_state['values']['course_compliance']==0)&& (!empty($form_state['values']['validity_days'])||!empty($form_state['values']['validity_date']))){
            	$errobj=new stdClass();
            	$errobj->isValidateError = 1;
            	$errobj->errcode = '2';
            	$errobj->errorfield = 'validity_days';
            	$errobj->errormsg = "Validity cannot be set for Non-Compliance course.";
            	$error_messages[] = $errobj;
            }
            if($form_state['values']['crs_status'] != 'lrn_crs_sts_itv' && $form_state['values']['crs_status'] != 'lrn_crs_sts_atv'){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errorfield = 'crs_status';
                $errobj->errormsg = "Invalid course status.";
                $error_messages[] = $errobj;
            }
            addEditCodeValidate($form_state,$form_state['values']["crs_id"]);
            $form_errors = form_get_errors();
            if(count($form_errors) > 0) {
                foreach ($form_errors as $field=>$form_error) {
                    //expDebug::dPrint("field".$field,5);
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = '2';
                    $errobj->errorfield = $field;
                    $errobj->errormsg = $form_error;
                    //return $errobj;
                    $error_messages[] = $errobj;
                }
            }
        // if($form_state['values']['course_compliance'] == 1){
            $form_state['values']['completed_date'] = (!empty($form_state['values']['completed_date']) && ($form_state['values']['completed_date'] != 'mm-dd-yyyy')) ? $form_state['values']['completed_date'] : '';
            $form_state['values']['validity_date'] = (!empty($form_state['values']['validity_date']) && ($form_state['values']['validity_date'] != 'mm-dd-yyyy')) ? $form_state['values']['validity_date'] : '';

            if((!empty($form_state['values']['completed_days']) && (is_numeric($form_state['values']['completed_days']) == false)) ||
            (!empty($form_state['values']['validity_days']) && (is_numeric($form_state['values']['validity_days']) == false)) ||
                (!empty($form_state['values']['completed_hire_days']) && (is_numeric($form_state['values']['completed_hire_days']) == false))   ) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Days Value should be a number.";
                $error_messages[] = $errobj;
            } else if((!empty($form_state['values']['completed_days']) && ($form_state['values']['completed_days']<0)) ||
            (!empty($form_state['values']['validity_days']) && ($form_state['values']['validity_days']<0)) ||
                (!empty($form_state['values']['completed_hire_days']) && ($form_state['values']['completed_hire_days']<0))  ){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Days cannot be a negative number.";
                $error_messages[] = $errobj;
            }
            else if((!empty($form_state['values']['completed_days']) && (strlen($form_state['values']['completed_days'])>5)) ||
            (!empty($form_state['values']['validity_days']) && (strlen($form_state['values']['validity_days'])>5)) ||
                (!empty($form_state['values']['completed_hire_days']) && (strlen($form_state['values']['completed_hire_days'])>5))  ){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter maximum of 5 character in Days.";
                $error_messages[] = $errobj;
            }
            if((!empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_date'])) ||
            (!empty($form_state['values']['validity_days']) && !empty($form_state['values']['validity_date'])) ||
            (!empty($form_state['values']['completed_hire_days']) && !empty($form_state['values']['completed_date']))) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter either date or day value.";
                $error_messages[] = $errobj;
            }
            if(!empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_hire_days'])){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter either completed_days or completed_hire_days.";
                $error_messages[] = $errobj;
            }
        /*  if((!empty($form_state['values']['completed_date']) && $form_state['values']['completed_date'] != '' && !preg_match("/^(0[1-9]|1[012])[\\/.](0[1-9]|[12][0-9]|3[01])[\\/.](19|20)\d\d$/", $form_state['values']['completed_date']))){
                return array((object)array('id'=>'Date format is incorrect'));
            }*/

            if(($form_state['values']['completed_date']) || ($form_state['values']['validity_date'])) {
                $todayDate = date('m-d-Y');
                list($cMonth,$cDate,$cYear) = split("-",$form_state['values']['completed_date']);
                list($vMonth,$vDate,$vYear) = split("-",$form_state['values']['validity_date']);
                list($tMonth,$tDate,$tYear) = split("-",$todayDate);
                $completed_date  = mktime(0,0,0,$cMonth,$cDate,$cYear); //strtotime($form_state['values']['completed_date']);
                $validity_date   = mktime(0,0,0,$vMonth,$vDate,$vYear); //strtotime($form_state['values']['validity_date']);
                $todayDate   = mktime(0,0,0,$tMonth,$tDate,$tYear);
                /* 50153 ,51600 */
                if(!empty($form_state['values']['completed_date'])){
                    if(!validateDateFormat(trim($form_state['values']['completed_date']), 'm-d-Y'))
                    {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errorfield = "completed_date";
                        $errobj->errormsg = 'Completed date should be in mm-dd-yyyy format';
                        $error_messages[] = $errobj;
                    }               
                    else
                    {
                        if($completed_date < $todayDate){
                            $errobj=new stdClass();
                            $errobj->isValidateError = 1;
                            $errobj->errorfield = "completed_date";
                            $errobj->errormsg = "Completed date should not allow past date..";
                            $error_messages[] = $errobj;
                        }
                    }
                }
                if(!empty($form_state['values']['validity_date'])){
                    if(!validateDateFormat(trim($form_state['values']['validity_date']), 'm-d-Y'))
                    {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errorfield = "validity_date";
                        $errobj->errormsg = 'Validity date should be in mm-dd-yyyy format';
                        $error_messages[] = $errobj;
                    }
                    else
                    {
                        if($validity_date < $todayDate){
                            $errobj=new stdClass();
                            $errobj->isValidateError = 1;
                            $errobj->errorfield = "validity_date";
                            $errobj->errormsg = "Validity date should not allow past date.";
                            $error_messages[] = $errobj;
                        }
                    }
                }
                if(!empty($form_state['values']['completed_date']) && !empty($form_state['values']['validity_date']) && ($completed_date >= $validity_date)){
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->errorfield = "validity_date";
                    $errobj->errormsg = "Validity date should be greater than completed by date.";
                    $error_messages[] = $errobj;
                }else if(!empty($form_state['values']['completed_days']) && !empty($form_state['values']['validity_date'])){
                    $completedDays = ($form_state['values']['completed_days']) ? $form_state['values']['completed_days'] : 0;
                    $completedDays = ($form_state['values']['completed_days']) ? $form_state['values']['completed_days'] : 0;
                    $add_cdays     = $completedDays-1;
                    $completed_days = mktime(0,0,0,date("m"),date("d")+$add_cdays,date("Y"));
                    if($completed_days >= $validity_date) {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errorfield = "validity_date";
                        $errobj->errormsg = "Validity date should be greater than completed by days.";
                        $error_messages[] = $errobj;
                    }
                }
            }
    //   }
         if(!empty($form_state['values']['completed_days'])){
            $form_state['values']["compliance_completed"] = "days";
         }elseif(empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_hire_days'])){
            expDebug::dPrint(' $$narrowSearchSet = sandy' . print_r($selectedFilters, true) , 4);
            $form_state['values']["compliance_completed"] = "hire_days";
         }elseif(!empty($form_state['values']['completed_date'])){
            $form_state['values']["compliance_completed"] = "date";
         }

            // Added by Gayathri for 0070892 - start
            $currency_type     = $form_state['values']['currency_type'];
            $price             = $form_state['values']['price'] == '' ? null : $form_state['values']['price'];
            if(strlen($price) > 10) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';                     
                $errobj->errorfield = "price";
                $errobj->errormsg = "Enter maximum of 10 character in Price";
                return $errobj;
            }
        //if((!empty($price)) && !preg_match('/^[0-9]+.?[0-9]+$/', $price)){
            //if((!empty($price)) && !preg_match('/^[0-9]*$/', $price)){
            if((!empty($price)) && !is_numeric($price)){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errorfield = "price";
                $errobj->errormsg = t('ERR097').' '.t('LBL040');
                return $errobj;
            }
            
            if($currency_type){
            $curr_valid = currency_isValidOrNot($currency_type);
                if(empty($curr_valid)){
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->errormsg = "Currency Type is not Valid";
                    return $errobj;
                }
            }  
            /*Viswanathan added for #78099 */       
            if(!empty($price) && ($price < 0 || !is_numeric($price))){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_023';
                $errobj->field = 'price';
                $errobj->errormsg = "Price should not be negative";
                return $errobj;
            }
            if($price > 0 && is_numeric($price))
            {
                if(empty($currency_type))
                {
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->field = 'currency_type';
                    $errobj->errormsg = "Currency type should not empty";
                    return $errobj;
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
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->field = 'currency_type';
                        $errobj->errormsg = "The given Currency type is invalid or inactive status";
                        return $errobj;
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
            $form_state['values']['currency_type'] = $currency_type;

            // 0070892 - end
            $form_state['values']['crs_title'] = stripslashes($form_state['values']['crs_title']);
            $form_state['values']['crs_code'] = stripslashes($form_state['values']['crs_code']);
            $form_state['values']['crs_short_description']['value'] = stripslashes($form_state['values']['crs_short_description']['value']);
            if(count($error_messages) > 0) {
                $errors = new stdClass();
                $errors->isValidateError = 1;
                $errors->errors = $error_messages;
                return $errors;
            }

            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
            $ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
                $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_crs');
                expDebug::dPrint('Create Course API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
                if($custom_attr_error != '') {
                    $customErrmsg=new stdClass();
                    $customErrmsg->isValidateError = 1;
                    $customErrmsg->errormsg = $custom_attr_error;
                    return $customErrmsg;
                }
            } //#custom_attribute_0078975 - End Check module status

            expDebug::dPrint("Form state values from course creation API".print_r($form_state,1));
            $courseId =  addNewCourseDetails($form,$form_state,$createuserid,1);
            expDebug::dPrint(' addNewCourseByRestAPI created course:' . print_r($courseId,true), 5);

            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
            //$ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                saveCustomAttributesInAddEditForm($form, $form_state, $courseId[0]->id, 'cre_sys_obt_crs', $createuserid,'Inserted',1);
            } //#custom_attribute_0078975 - End Check module status
            

            if($courseId[0]->id  && $courseId[0]->id != 'Failiure') {
                //To sync solr
                syncSolrData('Course');
                return $courseId;
            } else{
              //return array((object)array('id'=>'Failure'));
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Course Id is not valid";
                return $errobj;
          }
        } catch (Exception $ex) {
            watchdog_exception('addNewCourseByRestAPI', $ex);
            expertusErrorThrow($ex);
        }
    }

    function updateCourseByRestAPI( &$form_state,$createuserid="")
    {
        try {
            $form=array();
            $error_messages = array();
            $form["form_id"]="";
            include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
            $crsStatus = $form_state['values']['crs_status'];
            $courseId = $form_state['values']['crs_id'];
            $delStatus = delete_object_record($courseId,'Course');
            $ifCourseUnpublish = getIfCourseBeUnpublished($courseId);
            $ifCourseValid = getIfCourseValid($courseId);
            $invalidStatus = 0;
            if($delStatus != 0 && $crsStatus == 'lrn_crs_sts_del'){
                $invalidStatus = 1;
            }
            if($ifCourseUnpublish != 0 && $crsStatus == 'lrn_crs_sts_itv'){
                $invalidStatus = 1;
            }
            if($ifCourseValid[0]->sesavailable == 0 && $crsStatus == 'lrn_crs_sts_atv'){
                $invalidStatus = 1;
            }
            if($invalidStatus){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errorfield = 'crs_status';
                $errobj->errormsg = "Invalid course status.";
                $error_messages[] = $errobj;
            }
            //to check course code already exists
            addEditCodeValidate($form_state, $courseId);
            $form_errors = form_get_errors();
            if(count($form_errors) > 0) {
                foreach ($form_errors as $field=>$form_error) {
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = '2';
                    $errobj->errorfield = $field;
                    $errobj->errormsg = $form_error;
                    //return $errobj;
                    $error_messages[] = $errobj;
                }

            }
            if($form_state['values']['course_compliance'] == 0 && (!empty($form_state['values']['validity_days']) || !empty($form_state['values']['validity_date']))){
                $errobj = new stdClass ();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Validity cannot be set for Non-Compliance course.";
                $error_messages [] = $errobj;
            }
            $enroll_compliance = getEnrolledComplianceCourse($courseId);
            $courseDetails = getCourseDetailsView($courseId);
            expDebug::dPrint(' Course detailss>>>>  ' . print_r($courseDetails, true) , 4);
            $err = 0;
            if($enroll_compliance >0){
                if (empty($courseDetails->compliance_completed )) {
                    if (! empty($form_state['values']['completed_hire_days']) || ! empty($form_state['values']['completed_days']) || ! empty($form_state['values']['completed_date'])) {
                        $err = 1;
                    }
                } else {
                if ($courseDetails->compliance_completed == 'hire_days') {
                    $complete_days = $courseDetails->complete_days;
                    if ($complete_days != $form_state['values']['completed_hire_days'])
                        $err = 1;
                }
                else if ($courseDetails->compliance_completed == 'days') {
                    $complete_days = $courseDetails->complete_days;
                    if ($complete_days != $form_state['values']['completed_days'])
                        $err = 1;
                }
                else if ($courseDetails->compliance_completed == 'date') {
                    $completion_date = date_format ( date_create ( $courseDetails->complete_date ), 'm-d-Y' );
                    expDebug::dPrint ( ' Course detailss>>>>  ' . print_r ( $completion_date, true ), 4 );
                    if ($completion_date != $form_state['values']['completed_date'])
                        $err = 1;
                }
            }
            if ($err == 1) {
                $errobj = new stdClass ();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Users enrollment is in progres, completion days/date cannot be changed.";
                $error_messages [] = $errobj;
            }
        }
        // if($form_state['values']['course_compliance'] == 1){
            $form_state['values']['completed_date'] = (!empty($form_state['values']['completed_date']) && ($form_state['values']['completed_date'] != 'mm-dd-yyyy')) ? $form_state['values']['completed_date'] : '';
            $form_state['values']['validity_date'] = (!empty($form_state['values']['validity_date']) && ($form_state['values']['validity_date'] != 'mm-dd-yyyy')) ? $form_state['values']['validity_date'] : '';
            if((!empty($form_state['values']['completed_days']) && (is_numeric($form_state['values']['completed_days']) == false)) ||
            (!empty($form_state['values']['validity_days']) && (is_numeric($form_state['values']['validity_days']) == false)) || 
            (!empty($form_state['values']['completed_hire_days']) && (is_numeric($form_state['values']['completed_hire_days']) == false))) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Days Value should be a number.";
                $error_messages[] = $errobj;
            } else if((!empty($form_state['values']['completed_days']) && ($form_state['values']['completed_days']<0)) ||
            (!empty($form_state['values']['validity_days']) && ($form_state['values']['validity_days']<0)) ||
                (!empty($form_state['values']['completed_hire_days']) && ($form_state['values']['completed_hire_days']<0))){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Days cannot be a negative number.";
                $error_messages[] = $errobj;
            }
            else if((!empty($form_state['values']['completed_days']) && (strlen($form_state['values']['completed_days'])>5)) ||
                    (!empty($form_state['values']['validity_days']) && (strlen($form_state['values']['validity_days'])>5)) ||
                    (!empty($form_state['values']['completed_hire_days']) && (strlen($form_state['values']['completed_hire_days'])>5))  ){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter maximum of 5 character in Days.";
                $error_messages[] = $errobj;
            }
            if((!empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_date'])) ||
            (!empty($form_state['values']['validity_days']) && !empty($form_state['values']['validity_date'])) ||
                     (!empty($form_state['values']['completed_hire_days']) && !empty($form_state['values']['completed_date'])) ) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter either date or day value.";
                $error_messages[] = $errobj;
            }
            
            if(!empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_hire_days'])){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Enter either completed_days or completed_hire_days.";
                $error_messages[] = $errobj;
            }
        /*  if((!empty($form_state['values']['completed_date']) && $form_state['values']['completed_date'] != '' && !preg_match("/^(0[1-9]|1[012])[\\/.](0[1-9]|[12][0-9]|3[01])[\\/.](19|20)\d\d$/", $form_state['values']['completed_date']))){
                return array((object)array('id'=>'Date format is incorrect'));
            }*/

            if(($form_state['values']['completed_date']) || ($form_state['values']['validity_date'])) {
                $todayDate = date('m-d-Y');
                list($cMonth,$cDate,$cYear) = split("-",$form_state['values']['completed_date']);
                list($vMonth,$vDate,$vYear) = split("-",$form_state['values']['validity_date']);
                list($tMonth,$tDate,$tYear) = split("-",$todayDate);
                $completed_date  = mktime(0,0,0,$cMonth,$cDate,$cYear); //strtotime($form_state['values']['completed_date']);
                $validity_date   = mktime(0,0,0,$vMonth,$vDate,$vYear); //strtotime($form_state['values']['validity_date']);
                $todayDate   = mktime(0,0,0,$tMonth,$tDate,$tYear);
                /* 50153 ,51600 */
                if(!empty($form_state['values']['completed_date'])){
                    if(!validateDateFormat(trim($form_state['values']['completed_date']), 'm-d-Y'))
                    {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errorfield = "completed_date";
                        $errobj->errormsg = 'Completed date should be in mm-dd-yyyy format';
                        $error_messages[] = $errobj;
                    }
                    else
                    {
                        if($completed_date < $todayDate){
                            $errobj=new stdClass();
                            $errobj->isValidateError = 1;
                            $errobj->errorfield = "completed_date";
                            $errobj->errormsg = "Completed date should not allow past date.";
                            $error_messages[] = $errobj;
                        }
                    }
                }
            if(!empty($form_state['values']['validity_date'])){
                    if(!validateDateFormat(trim($form_state['values']['validity_date']), 'm-d-Y'))
                    {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errorfield = "validity_date";
                        $errobj->errormsg = 'Validity date should be in mm-dd-yyyy format';
                        $error_messages[] = $errobj;
                    }
                    else
                    {
                        if($validity_date < $todayDate){
                            $errobj=new stdClass();
                            $errobj->isValidateError = 1;
                            $errobj->errorfield = "validity_date";
                            $errobj->errormsg = "Validity date should not allow past date.";
                            $error_messages[] = $errobj;
                        }
                    }
                }
                if(!empty($form_state['values']['completed_date']) && !empty($form_state['values']['validity_date']) && ($completed_date >= $validity_date)){
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->errorfield = "validity_date";
                    $errobj->errormsg = "Validity date should be greater than completed by date.";
                    $error_messages[] = $errobj;
                }else if(!empty($form_state['values']['completed_days']) && !empty($form_state['values']['validity_date'])){
                    $completedDays = ($form_state['values']['completed_days']) ? $form_state['values']['completed_days'] : 0;
                    $completedDays = ($form_state['values']['completed_days']) ? $form_state['values']['completed_days'] : 0;
                    $add_cdays     = $completedDays-1;
                    $completed_days = mktime(0,0,0,date("m"),date("d")+$add_cdays,date("Y"));
                    if($completed_days >= $validity_date) {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errorfield = "validity_date";
                        $errobj->errormsg = "Validity date should be greater than completed by days.";
                        $error_messages[] = $errobj;
                    }
                }
            }
        // }
         
         if(!empty($form_state['values']['completed_days'])){
            $form_state['values']["compliance_completed"] = "days";
         }elseif(empty($form_state['values']['completed_days']) && !empty($form_state['values']['completed_hire_days'])){
            $form_state['values']["compliance_completed"] = "hire_days";
         }elseif(!empty($form_state['values']['completed_date'])){
            $form_state['values']["compliance_completed"] = "date";
         }
         // Added by Gayathri for 0070892 - start
        $currency_type     = $form_state['values']['currency_type'];
        $price             = $form_state['values']['price'] == '' ? null : $form_state['values']['price'];
        
        
        if(strlen($price) > 10) {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';                     
            $errobj->errorfield = "price";
            $errobj->errormsg = "Enter maximum of 10 character in Price";
            return $errobj;
        }
        
        //if((!empty($price)) && !preg_match('/^[0-9]+.?[0-9]+$/', $price)){
            //if((!empty($price)) && !preg_match('/^[0-9]*$/', $price)){
            if((!empty($price)) && !is_numeric($price)){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errorfield = "price";
                $errobj->errormsg = t('ERR097').' '.t('LBL040');
                return $errobj;
            }
            /*Viswanathan added for #78099 */
            if(!empty($price) && ($price < 0 || !is_numeric($price))){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->field = 'price';
                $errobj->errormsg = "Price should not be negative";
                return $errobj;
            }
        if($currency_type){
        $curr_valid = currency_isValidOrNot($currency_type);
            if(empty($curr_valid)){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Currency Type is not Valid";
                return $errobj;
            }
        }    
            if($price > 0 && is_numeric($price))
            {
                if(empty($currency_type))
                {
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->field = 'currency_type';
                    $errobj->errormsg = "Currency type should not empty";
                    return $errobj;
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
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->field = 'currency_type';
                        $errobj->errormsg = "The given Currency type is invalid or inactive status";
                        return $errobj;
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
            $form_state['values']['currency_type'] = $currency_type;
         
          // 0070892 - end  
         if(count($error_messages) > 0) {
            $errors = new stdClass();
            $errors->isValidateError = 1;
            $errors->errors = $error_messages;
            return $errors;
         }

           //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
         $ret_tmp = getcustomattributemodulestatus();
         if($ret_tmp == true){
                 include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
                 $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_crs');
                 expDebug::dPrint('Update Course API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
                 if($custom_attr_error != '') {
                    $customErrmsg=new stdClass();
                    $customErrmsg->isValidateError = 1;
                    $customErrmsg->errormsg = $custom_attr_error;
                    return $customErrmsg;
                 }
            } //#custom_attribute_0078975 - End Check module status
         
            $courseId = updateCourseDetails($form,$form_state,$createuserid,1);
            expDebug::dPrint(' addNewCourseByRestAPI created course:' . print_r($courseId,true), 5);

            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
            //$ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                saveCustomAttributesInAddEditForm($form, $form_state, $courseId[0]->id, 'cre_sys_obt_crs', $createuserid,'Updated',1);
            } //#custom_attribute_0078975 - End Check module status

            //To sync solr
            syncSolrData('Course');
            return $courseId;
        } catch (Exception $ex) {
            watchdog_exception('updateCourseByRestAPI', $ex);
            expertusErrorThrow($ex);
        }
    }

    function getCourse_DetailsInfoView($courseId) {
        try{
            expDebug::dPrint('vvvvvvv-->'.$courseId);
            include_once(drupal_get_path('module', 'exp_sp_coursedetail') .'/exp_sp_coursedetail.inc');
            $ret_array = array();
            //  if(!empty($nodeId)) {

            // Get course id from node id
            //$courseId = getEntityIdFromNodeId($nodeId);
            // Get course details
            $ret_array['catalog'] = getCourseDetailsView($courseId);


            // Get course profile
            $ret_array['entityProfRes'] = getCourseProfileAsText($courseId);

            // Get pre-requisites
            $ret_array['preRequiste'] = getPrerequisiteDetails($courseId,5);
            expDebug::dPrint('preRequiste ' . print_r($ret_array['preRequiste'], true) , 4);

            //For getting Equivalence Coursees
            $ret_array['Equivalence'] = getPrerequisiteDetails($courseId,6);
            expDebug::dPrint('Equivalence ' . print_r($ret_array['Equivalence'], true) , 4);

            //}
            expDebug::dPrint(' $ret_array = ' . print_r($ret_array, true) , 4);
            return $ret_array;
        }catch (Exception $ex) {
            watchdog_exception('getCourse_DetailsInfo', $ex);
            expertusErrorThrow($ex);
        }
    }
    function getCourseDetailsView($courseId,$callFromApi = '') {
        try{
            // Select table
            $selectStmt = db_select('slt_course_template', 'crs');
            $selectStmt->leftJoin('slt_profile_list_items','spli','crs.lang_code=spli.code');
            if($callFromApi == ''){
                //$selectStmt->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=crs.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.getIdOfLoggedInUser().'\') AND mro.entity_type = \'cre_sys_obt_crs\'');
                // $selectStmt->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=crs.id AND catacs.entity_type = \'cre_sys_obt_crs\'');
                //$selectStmt->leftJoin('slt_group_mapping', 'group_map', 'group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\'');
                //$selectStmt->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id');

            }


            // Set conditions
            $selectStmt->condition('crs.id',  $courseId);

            // Select fields
            $selectStmt->addField('crs',    'id',                   'id');
            $selectStmt->addField('crs',    'title',                'title');
            $selectStmt->addField('crs',    'short_description',    'description');
            $selectStmt->addField('crs',    'short_description',    'short_description');
            $selectStmt->addField('crs',    'code',                 'code');
            $selectStmt->addField('spli',   'name',                 'lang');
            $selectStmt->addField('crs',  'custom0',              'view_count');
            $selectStmt->addField('crs',    'is_compliance',    'is_compliance');
            $selectStmt->addField('crs',    'complete_days',    'complete_days');
            $selectStmt->addField('crs',    'compliance_completed',     'compliance_completed');
            $selectStmt->addField('crs',    'complete_date',    'complete_date');
            $selectStmt->addField('crs',    'validity_days',    'validity_days');
            $selectStmt->addField('crs',    'validity_date',    'validity_date');
            $selectStmt->addField('crs',    'author_vendor',    'author');
            $selectStmt->addField('crs',    'price',    'price');
            $selectStmt->addField('crs',    'currency_type',  'currency_type');         
            $selectStmt->addExpression('(Select attr2 from slt_profile_list_items where attr1 = crs.currency_type)','currency_symbol');
            
            if($callFromApi == ''){
                //  $selectStmt->addField('mro','mro_flag', 'mro_id');
                //  filterByUserAccessPrivileges($selectStmt, 'catacs');
            }
            expDebug::dPrintDBAPI(' $selectStmt OBJ = ' , $selectStmt);

            // Execute query
            $result = $selectStmt->execute();
            if($callFromApi){
                return $result->fetchObject();
            }
            expDebug::dPrint(' rowCount = ' . $result->rowCount() , 3);
            if ($result->rowCount() <= 0) {
                $courseDetails->code = '-';
                $courseDetails->description = '-';
                $courseDetails->id = '-';
                $courseDetails->view_count = 0;
            }else{
                $courseDetails = $result->fetchObject();
            }
            $courseDetails->description = trim($courseDetails->description);
            expDebug::dPrint(' $courseDetails = ' . print_r($courseDetails, true) , 4);
            $txn = db_transaction();
            try{
                // Select table
                $updateStmt = db_update('slt_course_template');

                // Set conditions
                $updateStmt->condition('id', $courseId);

                // Select fields
                $updateStmt->fields(array('custom0' => ($courseDetails->view_count + 1)));

                expDebug::dPrintDBAPI(' $updateStmt = ' , $updateStmt);

                // Execute query
                $updateStmt->execute();
            }catch (Exception $ex) {
                $txn->rollback();
                unset($txn);
                watchdog_exception('getCourseDetails', $ex);
                expertusErrorThrow($ex);
            }
            // Commit the transaction
            unset($txn);
            expDebug::dPrint("Return getCourseDetails ". print_r($courseDetails,true) , 4);
            // Return course details
            return $courseDetails;
        }catch (Exception $ex) {
            watchdog_exception('getCourseDetailsView', $ex);
            expertusErrorThrow($ex);
        }
    }

    function getCertificateforCourseView($courseId){
        try{
            $selectStmt = db_select('slt_notification_info', 'info');
            $selectStmt->leftJoin('slt_entity_notification_mapping','notify','notify.notify_send_type = info.send_options and notify.notify_id = info.id');
            $selectStmt->leftJoin('slt_course_template','crs','crs.id=notify.entity_id');
            $selectStmt->addField('info','notification_title','certificate');
            $selectStmt->condition('notify.entity_id', $courseId, '=');
            expDebug::dPrintDBAPI('$select sabeena=',$selectStmt);
            $result =  $selectStmt->execute()->fetchField();
            expDebug::dPrint('$result ='.print_r($result,true),5);
            return $result;
        }catch (Exception $ex) {
            watchdog_exception('getCertificateforCourseView', $ex);
            expertusErrorThrow($ex);
        }

    }

    // function used to retrive the class id's for the particular course
    function getCourseClassIds($courseId) {
        try {
            $selectCls = db_select('slt_course_class', 'cls');
            $selectCls->addField('cls', 'id', 'class_id');
            $selectCls->condition('course_id', $courseId, '=');
            //expDebug::dPrintDBAPI('$$selectCls =',$selectCls);
            $result =  $selectCls->execute()->fetchAll();
            return $result;
            //expDebug::dPrint('$result getCourseClassIds='. print_r($result, 1), 4);
        } catch(Exception $ex) {
            watchdog_exception('getCourseClassIds', $ex);
            expertusErrorThrow($ex);
        }
    }
    // Added by Gayathri for 0070892
    function getClassIdsforCoursePriceInherit($courseId) {
        try {
            $selectCls = db_select('slt_course_class', 'cls');
            $selectCls->innerJoin('slt_node_learning_activity','node_activity','node_activity.entity_id = cls.id and node_activity.entity_type="cre_sys_obt_cls"');
            $selectCls->addField('cls', 'id', 'class_id');
            $selectCls->addField('cls', 'price', 'class_price');
            $selectCls->addField('cls', 'currency_type', 'class_currency_type');
            $selectCls->addField('node_activity', 'node_id', 'nid');            
            $selectCls->condition('course_id', $courseId, '=');
            $selectCls->condition('price_inherit', '', 'IS NULL');           
            $selectCls->condition('cls.status', array('lrn_cls_sts_del','lrn_cls_sts_can','lrn_cls_sts_dld') ,'NOT IN');
            $result =  $selectCls->execute()->fetchAll();
            return $result;
        } catch(Exception $ex) {
            watchdog_exception('getClassIdsforCoursePriceInherit', $ex);
            expertusErrorThrow($ex);
        }
    }
    
?>