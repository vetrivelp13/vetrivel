<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_learning/exp_sp_administration_notification_insert.inc";
include_once(drupal_get_path('module', 'exp_sp_administration_location') .'/exp_sp_administration_location.inc');

if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
    include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
} //#custom_attribute_0078975 - End Check module status

function exp_sp_administration_catalog_class_title_form($form, &$form_state,$renderMode, $courseId, $classId,$oldClassid=0){
    try{
    $form['tip-position'] = array(
      '#type' => 'markup',
      '#markup' => '<input type="hidden" id="qtip_position" name="qtip_position" value="">',
      );
               if (!empty($form_state['triggering_element']) &&
              ($form_state['triggering_element']['#value'] == 'Add Class' || // Add Class clicked after Save of new record without entering session/content details
                  (!empty($form_state['storage']['new']['class_id']) && // Another class Edit button clicked after Save of new record without entering session/content details
                         !empty($form_state['triggering_element']['#ajax']['class_id']) &&
                                  $form_state['storage']['new']['class_id'] != $form_state['triggering_element']['#ajax']['class_id']))) {
        expDebug::dPrint(' unset($form_state[storage][new_entity]) called');
        unset($form_state['storage']['new_entity']);
      }

      if(empty($courseId) && !empty($form_state['storage']['new']['course_id'])) {
          $courseId = $form_state['storage']['new']['course_id'];
          //$classId  = '';//$form_state['storage']['new']['class_id'];
          $classId  = $form_state['storage']['new']['class_id'];
      }

      $displayMode = '';
      if(in_array('view', $form_state['build_info']['args'])){
        $form_state['storage']['step']         = 'view';
        $displayMode = 'view';
      }

       $form['edit-class-details-start'] = array(
          '#prefix' => "<div id='catalog-class-addedit-form-details'>",
        );
      if(!empty($courseId)) {
        $form_state['storage']['courseId'] = $courseId;
      }
      else{
         $courseId = $form_state['course_id'];
         $form_state['storage']['courseId'] = $courseId;
      }
      $clsList       = getAdministrationClassDetails($courseId);
      $courseId     = core_encrypt($courseId); // URL Encryption
      if(isset($addAttributes))
        unset($addAttributes['onclick']);
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      // $addAttributes['class'][] = 'addedit-form-expertusone-throbber narrow-search-actionbar-orange-btnBG';
       $addAttributes['class'][] = 'narrow-search-actionbar-orange-btnBG';
      //$addAttributes['onclick'][] = '$(document).ready(function(){ addClassDetails('.$courseId.') }); remove_messages("class");';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $addAttributes['data-wrapperid'] = array('catalog-class-addedit-form-details');
      $form['add-class-list'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="add-class-list-wrapper">'
      );

     // Classs Search auto-complete start

     $paintMultiAction  = "<div id='search-list-class-title-keyword' class='search-list-keyword' style='display:block;'>";
     $paintMultiAction .= "<span id='search-dropdwn-list-class'>";
     $paintMultiAction .= "<input type='hidden' id='search_all_classs_type-hidden' value='clstit' />";
     $paintMultiAction .= "<input type='hidden' id='classname-autocomplete_hidden' value='".t('LBL766')."' />";
     $paintMultiAction .= "<span id='select-list-class-dropdown' class='select-list-dropdown-class'>".t('LBL083')."</span>";
     $paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-class-dropdown-link' onclick='$(\"#root-admin\").data(\"narrowsearch\").moreClassSearchHideShow();'> </a>";
     $paintMultiAction .= "<ul id='select-list-class-dropdown-list'>";
     $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreClassSearchTypeText('".t('LBL083')."','clstit');\">".t('LBL083')."</li>";
     //$paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('".t('LBL102')."','status');\">".t('LBL102')."</li>";
     $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreClassSearchTypeText('".t('LBL084')."','deltype');\">".t('LBL084')."</li>";
     $paintMultiAction .= "<li onclick=\"$('#root-admin').data('narrowsearch').moreClassSearchTypeText('".t('LBL102')."','clsstatus');\">".t('LBL102')."</li>";
     //$paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('".t('LBL173')."','usertype');\">".t('LBL173')."</li>";
    // $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('".t('LBL180')."','manager');\">".t('LBL180')."</li>";
     $paintMultiAction .= "</ul>";
     $paintMultiAction .= "</span></div>";

     $form['class_multi_search_container'] = array(
     '#type' => 'markup',
     '#markup' => '<div class="admin_class_multi_search_container">',
     );
     $form['more_add_enrolluser_search'] = array(
     '#type' => 'markup',
     '#markup' => $paintMultiAction,
     );
     $form['class_auto-search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin-class-auto-search-container">',
      );
      // onload="$(\'#root-admin\').data(\'narrowsearch\').classDisplayPagination('.$courseId.','.$classId.');"
      $searchAjaxEvent = 'click';
     // $searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "enrolltpuser", $(\'#enrolltpuser-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "0"); return false;';
      $searchAttributes['onClick'][] = "$('#root-admin').data('narrowsearch').searchClassNameFilter('$courseId','0'); return false";

      $acMenuPath = "administration/learning/catalog/class-autocomplete/".$courseId;

      /*$form['add_enrolltpuser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
      );*/

      $autoCompleteAttributes['class'][] = 'admin_class_ac_input_mainform';
     // $autoCompleteAttributes['class'][] = 'admin_ac_class_txtbox';
      $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
      addACFieldForMultiSelect($form, 'classname-autocomplete', $acMenuPath, '', '', '', array(),
                                  array('search_type' => "$('#search_all_classs_type-hidden').val()"), false, false,
                                      t('LBL766'), $autoCompleteAttributes);

      $form['class_auto-search-end'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
      );

     // $searchAttributes['Class'][] = 'admin-pagination-new-search-go';
       $searchAttributes['Class'][] = 'admin-pagination-search-go narrow-text-search';

       $form['search_classuser'] = array(
        '#type' => 'submit',
        '#title' => t('LBL304'),
        '#name' => 'Search autosearch',
        '#value' => ' ',//0018602: issue in training plan
        //'#submit' => array('search_enrolltpuser'),
        '#attributes' => $searchAttributes,
        /*'#ajax' => array(
          'event' => $searchAjaxEvent,
        ),*/
      );
      $form['class_multi_search_container-end'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div></div>',
      );



      if($displayMode != 'view'){
        $entityId            =  '';
        $entityType          = 'cre_sys_obt_cls';
        $qtipIdInit          = $entityId.'_'.$entityType;
        $qtipOptClassObj     = "{'url':'administration/class-addedit/".$courseId."//".$entityType."/".$oldClassid."','popupDispId':'qtip_addclass_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachIdqtip_addclass_visible_disp_".$entityId."','wBubble':870,'hBubble':'auto','tipPosition':'bottomRight','qtipClass':'qtip-parent','courseId':'".$courseId."'}";

        //Using the below veriable we can find current loaded theme name
        global $theme_key;

        $form['add-class-button'] = array(
          '#type' => 'markup',
          '#prefix' => "<div id='qtip_addclass_visible_disp_".$qtipIdInit."' onmousedown =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptClassObj."); return false;\" onmouseup=\"editClassDetails('.$courseId.','.$classId.'); return false;\" class='admin-save-button-container add-class-button'><div class='narrow-search-actionbar-orange-btnLeft'></div><div class='narrow-search-actionbar-orange-btnBG'>",
          '#suffix' => t("LBL260").'</div><div class="narrow-search-actionbar-orange-btnRight"></div></div>',
          '#value' => ($theme_key == 'expertusoneV2')? '': t("LBL260"),
          '#name' => "Add Class",
          '#limit_validation_errors' => array(),
          '#attributes' => $addAttributes,
        );
      }



      if($clsList == 0){
        // Ref to TICKET 0021973: Remove </div> from $form['edit-class-details-end'] and added to $form['no-class-msg'] markup end.
         $form['no-class-msg'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="no-records-msg" id="add-edit-class-norecords">'.t('ERR061').'</div></div>'
        );

      }else{
             $form['edit-class-details-end'] = array(
              '#suffix' => '<div id="admin-course-class-list-pagination-wrapper"><div id="paint-class-search-results-datagrid">',
            );
                    $form['class_list_grid_table'] = array(
                        '#type' => 'markup',
                        '#markup' => '<table id="admin-course-class-list-'.$courseId.'-pagination"></table><div class="ui-jqgrid-toppager" id="admin-course-class-list-'.$courseId.'-pagination_toppager"></div>',
                        '#suffix' => '</div></div>',
                    );
      }

      // Create the form element
      $form['hidden_textformat_field'] = array(
        '#type' => 'text_format',
        '#name' => 'test-desc',
        '#prefix' => "<div style='display:none'>",
        '#suffix' => "</div>",
      );
      $form['add-class'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="add-class-wrapper"></div></div>'
      );

      if(isset($form_state['storage']['fromrebuild']))
            $classId=0;
        else
            $classId=empty($classId)?0:$classId;
        if($clsList != 0){
                $form['class_bubble_popup_trigger8526'] = array (
                    '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="$(\'#root-admin\').data(\'narrowsearch\').classDisplayPagination(\''.$courseId.'\',\''.$classId.'\',\''.$oldClassid.'\');" />',
                );
        }
        return $form;
        } catch (Exception $ex) {
            watchdog_exception('exp_sp_administration_catalog_class_title_form', $ex);
            expertusErrorThrow($ex);
        }
}


function edit_class($form, &$form_state) {
  try {
    $classId = $form_state['triggering_element']['#ajax']['class_id'];
    $courseId = $form_state['triggering_element']['#ajax']['course_id'];
    $displayMode = $form_state['triggering_element']['#ajax']['display_mode'];

    $form_state['class_id'] = $classId;
    $form_state['course_id'] = $courseId;
    $form_state['display_mode'] = $displayMode;

    $form_state['rebuild'] = TRUE;
  }catch (Exception $ex) {
    watchdog_exception('edit_class', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * exp_sp_administration_courseclass_addedit_form() - Multi-Form implementation for course / class
 */

function exp_sp_administration_catalog_class_callback($js = NULL, &$form_state){
  expDebug::dPrint(' $form_state = ' .
                                                 print_r($form_state, true) , 5);
  include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_course.inc');
  try {
    $newEntity = !empty($form_state['storage']['new_entity']); // to open session/catalog entry tab in class form after a new class is added
    $displayMode = $form_state['display_mode'];

    $min_form_state = array(
     'ajax' => TRUE,
     're_render' => FALSE,
     'no_redirect' => TRUE,
     /*'course_id' => $form_state['course_id'],
     'class_id' => $form_state['class_id'],*/
     'course_id' => $form_state['triggering_element']['#ajax']['course_id'],
     'class_id' => $form_state['triggering_element']['#ajax']['class_id'],
     'build_info' => array('args' => array('ajax'))
    );

    $form_state = $min_form_state;
    //print_r($form_state);

    if ($newEntity) { // to open session/catalog entry tab in class form after a new class is added
        $form_state['storage'] = array('new_entity' => TRUE);
    }

    $form_state['input'] = array(); // Clear user input - raw
    $form_state['values'] = array(); // Clear user input - security cleared

    $form_state['storage']['display_mode'] = '';
    if($displayMode == 'view'){
      $form_state['storage']['display_mode'] = 'view';
    }

    $formId = 'exp_sp_administration_catalog_class_addedit_form';
    $builtForm = drupal_build_form($formId, $form_state);

    $commands = array();
    $commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
    $commands[] = exp_ctools_modal_adjust($form_state);
    $commands[] = addedit_popup_dialog_scroll($form_state['triggering_element']['#ajax']['wrapper']);
    return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_catalog_class_callback', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getDefaultClassDescriptionAddEditFormFieldValue() - The function determines the default value for a class addedit form field
 */
function getDefaultClassDescriptionAddEditFormFieldValue($field, $form_state, $dbDetail) {

  try {
    /*$defaultValue = empty($form_state['values'][$field])? (empty($dbDetail[$field])? '' : $dbDetail[$field]) :
                                                          $form_state['values'][$field]['value'];*/
    if ($field == 'short_description') {
      $defaultValue = empty($form_state['values'][$field]['value']) ||  $form_state['values'][$field]['value']==''? (empty($dbDetail[$field]) || $dbDetail[$field]==''? '' : $dbDetail[$field]) : $form_state['values'][$field]['value'];
    }else if ($field == 'additional_info') {
      $defaultValue = empty($form_state['values'][$field]['value']) ||  $form_state['values'][$field]['value']==''? (empty($dbDetail[$field]) || $dbDetail[$field]==''? '' : $dbDetail[$field]) : $form_state['values'][$field]['value'];
    }
    else {
      $defaultValue = empty($form_state['values'][$field]) ? (empty($dbDetail[$field]) || $dbDetail[$field]==''? '' : $dbDetail[$field]) : $form_state['values'][$field];
    }
    return $defaultValue;
  }catch (Exception $ex) {
    watchdog_exception('getDefaultClassDescriptionAddEditFormFieldValue', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getNewClassDefaultValues() - for a new class, various course values are class default values
 */
function getNewClassDefaultValues($courseId) {
  try {
    $select = db_select("slt_course_template", "crs");
    $select->addField('crs', 'title', 'title');
    $select->addField('crs', 'code', 'code');
    $select->addField('crs', 'short_description', 'short_description');
    $select->addField('crs', 'description', 'description');
    // Changed by Gayathri for 0070892
    $select->addField('crs', 'price', 'crs_price');
    $select->addField('crs', 'currency_type', 'crs_currency_type');
    $select->condition('crs.id', $courseId, '=');
    expDebug::dPrintDBAPI("Class default values",$select);
    $result = $select->execute()->fetch();
    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

    return $result;
  }catch (Exception $ex) {
    watchdog_exception('getNewClassDefaultValues', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * exp_sp_administration_catalog_class_addedit_form() - Build the addedit form for Class.
 */

function exp_sp_administration_catalog_class_addedit_form($form, &$form_state, $renderMode){
  try {
    includeFiles();
      include_once('exp_sp_administration_catalog.inc');
      // To set the display mode as view
      $readOnlyForView = false;
      if($form_state['storage']['display_mode'] == 'view' ){
        $form_state['storage']['step'] = 'view';
        $readOnlyForView = true;
      }

      $showCancelSave = 1;

      $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
        global $theme_key;
        if($theme_key == 'expertusoneV2') {
            //NEWUI THEME STYLE SHEET
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
        } else {
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);
        }
      //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);

      $classDetails = array();

      $courseId = $form_state['course_id'];
      $classId = $form_state['class_id'];
      $oldClassId= $form_state['oldClassId'];

      expDebug::dPrint(' $form_state = ' . print_r($form_state['triggering_element']['#name'], true) , 5);

      $classDetails['course_id'] = $courseId;
      if (!empty($classId)) {
        $classDetails['class_id'] = $classId;
        // Fetch class details to be displayed in the form
        $classDetails = getCatalogClassDetails($classId);
        // Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
        updatePaginationCookie();
      }
      else if (empty($form_state['values'])) {
        $courseDetails   = getNewClassDefaultValues($courseId);
        $currentUserId = getIdOfLoggedInUser();
        $num_deleted = db_delete('slt_entity_reading_mapper')->condition('entity_id', '0')->condition('entity_type', 'cre_sys_obt_cls')->condition('created_by', $currentUserId)->execute();
        $classDetails['title'] = $courseDetails->title;
        $classDetails['code'] = $courseDetails->code;
        $classDetails['short_description'] = $courseDetails->short_description;
        $classDetails['description'] = $courseDetails->description;
        // Changed by Gayathri for 0070892
        $classDetails['price'] = $courseDetails->crs_price;
        $classDetails['currency_type'] = $courseDetails->crs_currency_type;
      }
      //42964: when i select groups from left side,if groups are getting selected and listed in right side,but selectenot showing in left side
      $emptyId = (empty($form_state['values']['empty_id']) && empty($classId)) ? microtime(false) : $form_state['values']['empty_id'];
      expDebug::dPrint(' $classDetails = ' . print_r($classDetails, true) , 4);

      if(!empty($emptyId) && empty($form_state['rebuild'])){
        insertCourseAttachedGroups($emptyId,$courseId);
        insertCourseAttachedUsers($emptyId,$courseId);
      }

      // Create a new add/edit form
      $form = newAddEditFormWithCancelAndSave('catalog-class-basic-addedit-form-'.$classId);
      $entityType = 'cre_sys_obt_cls';
      addHiddenValueToAddEditForm($form, 'id', $classId);
      addHiddenValueToAddEditForm($form, 'course_id', $courseId);
      addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
      addHiddenFieldToAddEditForm($form, 'entity_value', $entityType);
      addHiddenFieldToAddEditForm($form, 'hidden_cancel_reason', '');
      /*if(empty($classId)){
          $form['new-class-title-box'] = array(
          '#type' => 'markup',
          '#markup' => '<div id="add-class-title-box">'.t('LBL261').'</div>'
          );
      }

      $form['emptyspace'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-class-title-arrow-icon"></div>'
      ); */

      $form['class_detail_disp'] = array(
        '#type'       => 'markup',
        '#prefix'     => '<div class="catalog-class-basic-addedit-form-disp" id="catalog-class-basic-addedit-form-disp-'.$classId.'">',
      );

      //Hide button for close qTip
      $hidewrapperId         = 'add-class-list-wrapper';
      $hideCloseButton = 'course_class_cancel_button';
      $hideCloseAttributes['class'] = array('addedit-edit-' . $hideCloseButton);
      $hideCloseAttributes['class'][] = 'addedit-form-expertusone-throbber';
      $hideCloseAttributes['data-wrapperid'] = array($hidewrapperId);
      //$hideCloseAttributes['onclick'][] = 'remove_messages("class"); return false;';
      $classIdQtip = !empty($classId)?$classId:0;
      $hideCloseAttributes['onclick'][] = 'remove_messages("class");';

      $form[$hideCloseButton] = array(
        '#type' => 'submit',
        '#id' => $hideCloseButton,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
        '#value' => '',
        '#name' => 'hiddencancel',
        '#attributes' => $hideCloseAttributes,
        '#ajax' => array(
          'wrapper' => $hidewrapperId,
          'callback' => 'addEditCatalogCancelRepaintForm',
          'method' => 'replace',
          'effect' => 'none',
          'event'=>'click',
          'keypress' => true,
          'progress' => array(
            'type' => 'throbber',
            'message' => ''
          ),
        )
      );

      list_class_attributes($form, $form_state,$classId,$classDetails);
      $clsDeliveryType =  getDefaultAddEditFormFieldValue('delivery_type', $form_state, $classDetails);
      $class_detail_wrapper_class = ($form_state['storage']['substage'] == 'sessiondetail' || ($form_state['storage']['new_entity'] )) ? 'catalog-class-session-basic-addedit-form-container' : 'catalog-course-basic-addedit-form-container';
      $form['class_detail_wrapper'] = array(
        '#type'       => 'markup',
        '#prefix'     => '<div class="addedit-form-main-container">
                            <div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
                            <div class="'.$class_detail_wrapper_class.'" id="catalog-class-basic-addedit-form-container">',
      );
         
        //Scroll issue in ClassAddEDITFORM #88022
      if(module_exists('exp_sp_administration_customattribute')){          
         expDebug::dPrint('Triggering Element ='.print_r($form_state['triggering_element']['#name'],true),5);
         if($form_state['triggering_element']['#name']=='details'){
              $form_state['storage']['class_saved_first_time']='0';
          }
      	 $saved_first_time=$form_state['storage']['class_saved_first_time'];
         expDebug::dPrint('$saved_first_time='.$saved_first_time,5);
           //if(in_array($form_state['triggering_element']['#name'],array('details','','save','saveandshow'))){ //if details page only
          if($saved_first_time!='1'){
           if(!in_array($form_state['triggering_element']['#name'],array('Content Details','Session Details','survey','assessment','enrolluser'))){
               $form['class_detail_scroll_wrapper_start'] = array(
                 '#type'       => 'markup',
                 '#prefix'     => '<div id="catalog-class-basic-addedit-form-scroll-container">',
                ); 
           }
          } 
      }
      
      if($form_state['storage']['step'] == 'view') { // Start - View / Edit mode

        if(($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) && empty($form_state['storage']['new_entity'])) {

          $fieldList         = array('id', 'title', 'code', 'short_description', 'description','status', 'status_name', 'status_inactive_reason', 'delivery_type', 'delivery_type_name', 'language_name', 'price', 'currency_type', 'currency_name', 'min_seats','max_seats','waitlist_count','class_location', 'class_location_name', 'author_vendor','additional_info','export_compliance','reg_end_date','scheduled_duration','addn_catalog_show','addn_notification_show');
          $defaultValue      = getDefaultClassAddEditFormFieldValue($fieldList, $form_state, $classDetails);
          viewClassDetails($form, $form_state, $defaultValue);
        }

      } else {

      if(($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) && empty($form_state['storage']['new_entity'])) {
        $form_state['storage']['substage'] = 'details';
        $form['addedit-remove-space-title-start'] = array(
          '#type'   => 'markup',
          '#markup' => '<div style="width:100%;">',
        );
        $defaultValue =  getDefaultAddEditFormFieldValue('title', $form_state, $classDetails);
        addTextFieldToAddEditForm($form, 'title', t('LBL083'), $defaultValue, 101, true,false,300);
        $form['addedit-remove-space-title-end'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
        );

        addTwoColumnRowToAddEditForm($form, 'classcode_langcode');
        $defaultValue =  getDefaultAddEditFormFieldValue('code', $form_state, $classDetails);
        addTextFieldToAddEditForm($form, 'code', t('LBL096'), $defaultValue, 102, true,false,100);
              //cre_sys_lng_eng
        $defaultCrsLng = defaultCourseLanguageForClass($courseId);
        expDebug::dPrint('values of new created clas lang'.print_r($defaultCrsLng,true));
        $classDetails['lang_code'] = !empty($classDetails['lang_code']) ? $classDetails['lang_code'] : $defaultCrsLng;
        $defaultValue =  getDefaultAddEditFormFieldValue('lang_code', $form_state, $classDetails);
        addDropDownToAddEditForm($form, 'lang_code', t('LBL038'), getProfileItemNamesKeyed('cre_sys_lng_'), $defaultValue, 103, true);
        endAddEditTwoColumnRow($form);

       /* $defaultValue =  getDefaultAddEditFormFieldValue('short_description', $form_state, $classDetails);
        addTextAreaFieldForShortDescription($form, 'short_description', t('LBL097'), $defaultValue, 3, true);*/
        //addHiddenFieldToAddEditForm($form, 'ckeditor_word_count', '5000');  // commented for 0076233
        $form['addedit-long-description-container'] = array(
              '#type'   => 'markup',
              '#markup' => '<div class="addedit-long-description-container">',
        );
        $defaultValue =  getDefaultClassDescriptionAddEditFormFieldValue('short_description', $form_state, $classDetails);
        //addTextAreaFieldToAddEditForm($form, 'description', t('LBL098'), $defaultValue, 4, false);
        addTextFormatFieldToAddEditForm($form, 'short_description', t('LBL229'), $defaultValue, 'full_html', 104, true,false,5000);
        $form['addedit-long-description-container1'] = array(
          '#type'   => 'markup',
          '#markup' => '</div>',
        );
        // Begin : Two column - lang_code, status
       // addTwoColumnRowToAddEditForm($form, 'lang_code_status_status_temp');

        /*$statusUniqueCode = 'Class';
        $attributes['onchange'][] = 'getStatusReason(this,\''.$statusUniqueCode.'\');';
        $classDetails['status'] = !empty($classDetails['status']) ? $classDetails['status'] : 'lrn_cls_sts_atv';
        $defaultValue =  getDefaultAddEditFormFieldValue('status', $form_state, $classDetails);
        addDropDownToAddEditForm($form, 'status', t('LBL102'), getProfileItemNamesKeyed('lrn_cls_sts_'), $defaultValue, 5, true, false, $attributes);
        */
        // Begin : Two column - delivery_type_location
        addTwoColumnRowToAddEditForm($form, 'delivery_type_location');
        $defaultValue =  getDefaultAddEditFormFieldValue('delivery_type', $form_state, $classDetails);
        $cls_delivery_type = $defaultValue;
        $readonly = empty($classId) ? false : true;
        if(isset($_SERVER['HTTP_REFERER']) && !empty($_SERVER['HTTP_REFERER'])){
            if(isset($_COOKIE['delivery_type']) && ($_COOKIE['delivery_type']== 'lrn_cls_dty_ilt' || $_COOKIE['delivery_type']== 'lrn_cls_dty_vcl') && (strrpos($_SERVER['HTTP_REFERER'],'admincalendar') > 0)){
                $readonly = true;
                $defaultValue = $_COOKIE['delivery_type'];
            }
        }
        if($classId){
        $form['class_onload'] = array(
                '#type' => 'markup',
                '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="changeDeliveryType(\''.$cls_delivery_type.'\');" />',
            );
        }
        //addDropDownToAddEditForm($form, 'delivery_type', t('LBL036'), getProfileItemNamesKeyed('lrn_cls_dty_'), $defaultValue, 105, true, $readonly,$attributes);

        $dispWrapperId = 'catalog-class-basic-addedit-form-'.$classId;
          //addDropDownToAddEditForm($form, 'content_type', t('LBL608'), getProfileItemNamesKeyed('lrn_cnt_typ_'), $defaultValue, 1, true,$readonly,$attributes);
          $options                         = getProfileItemNamesKeyed('lrn_cls_dty_');
          $attributes                      = array();

          //Using the below veriable we can find current loaded theme name
          global $theme_key;
          if($theme_key == 'expertusoneV2') {
              $newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
              $newuiSuffix = '</div></div>';
          } else {
              $newuiPrefix = '';
              $newuiSuffix = '';
          }

          if(!$classId){
           $attributes['onchange'][] = 'changeDelTypeInAddClass(this.value);';
          }
          $attributes['class']             = array('addedit-edit-delivery_type');
          $attributes['class'][]           = 'addedit-form-expertusone-throbber';
          $attributes['data-wrapperid']    = array($dispWrapperId);
          $attributes['tabindex']          = 105;
          $nullOption[]                    = t('LBL674');
          $finalOptions                    = $nullOption + $options;
          foreach($finalOptions as $key => $value){
            $finalOptions[$key] = t($value);
          }
          $defaultOption                   = $defaultValue;
          
//        if(isset($_COOKIE["delivery_type"]))
//          $defaultOption = $_COOKIE["delivery_type"];
          $form['addedit_tmp_storage']['two_column']['current_column'] = 1;
          $form['delivery_type'] = array(
            '#type'          => 'select',
            '#name'          => 'delivery_type',
            '#id'            => 'delivery_type',
            '#prefix'        => addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', t('LBL036'), TRUE).$newuiPrefix,
            '#suffix'        => addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD').$newuiSuffix,
            '#title'         => t('LBL036'),
            '#title_display' => 'invisible',
            '#options'       => $finalOptions,
            '#default_value' => $defaultOption,
            '#disabled'      => $readonly,
            '#attributes'    => $attributes,
            '#ajax' => array(
                          'wrapper'        => $dispWrapperId,//'add-class-list-wrapper',
                          'callback'       => 'catalog_class_repaint_callback',
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

       // if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){
         /*$form['class-location-container'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="admin-class-location-container">'
          );*/
         // $defaultValue =  getDefaultAddEditFormFieldValue('class_location', $form_state, $classDetails);
         // addDropDownToAddEditForm($form, 'class_location', t('LBL041'), getClassLocation(), $defaultValue, 106, $mandatory,$readOnly);



     /*-----------Start Custom Select Box------*/
          /*$classCreateLocationIdQtip = !empty($classId) ? $classId : 0;
          $qtipLocationIdInit     = $classCreateLocationIdQtip.'_'.'cre_sys_obt_loc';
          $qtipOptLocationObj     = "{'url':'administration/location-add/".$classCreateLocationIdQtip."','popupDispId':'qtip_addlocation_visible_disp_".$qtipLocationIdInit."','catalogVisibleId':'qtipAttachIdqtip_addlocation_visible_disp_".$classCreateLocationIdQtip."','wBubble':636,'hBubble':'auto','tipPosition':'bottomRight','qtipClass':'admin-qtip-access-parent'}";
          //$qtipOptLocationObj     = "{'url':'administration/location-add/".$classCreateLocationIdQtip."','popupDispId':'qtip_addlocation_visible_disp_".$qtipLocationIdInit."','catalogVisibleId':'qtipAttachIdqtip_addlocation_visible_disp_".$classCreateLocationIdQtip."','wBubble':635,'hBubble':'300','tipPosition':'rightBottom'}";

          $locDetail = getClassLocationAdd();
          $html = '';

          foreach($locDetail as $locDetails){
            $html .= '<li data="'.$locDetails->id.'" title="'.$locDetails->name.'">'.$locDetails->name.'</li>';
          }

          addHiddenValueToAddEditFormLoc($form, 'class_location', $defaultValue);

          if(!empty($defaultValue)){
            $dt = getClassLocationAdd($defaultValue);
            $dTxt = $dt[0]->name;
          }else{
            if($cls_delivery_type == 'lrn_cls_dty_wbt' || $cls_delivery_type == 'lrn_cls_dty_vod' || $cls_delivery_type=='lrn_cls_dty_vcl'){
              $dTxt = t('LBL679');
            }else{
              $dTxt = t('LBL674');
            }
          }

          if($cls_delivery_type=='lrn_cls_dty_wbt' || $cls_delivery_type=='lrn_cls_dty_vod' || $cls_delivery_type=='lrn_cls_dty_vcl'){
            $classClick = 'top-select-nonclick';
            $inputClass = 'input-selected-loc-disp-nonclick';
            $mandatoryClass='';
          }else{
            $classClick = 'top-select';
            $inputClass = 'input-selected-loc-disp';
            $mandatoryClass='<span class="addedit-mandatory" style="visibility: visible;">*</span>';
          }

          $form['new_location'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="location-list-title">'.t('LBL041').':'.$mandatoryClass.'</div>
                        <div id="select-location-list">
                           <div id="location-list">
                            <ul class="loc-disp">
                              <li>
                                <div class="'.$classClick.'"  title="'.$dTxt.'"><input readonly value="'.$dTxt.'" name="location-input-txt" id="location-input-txt" size="8" class="'.$inputClass.'">&nbsp;&nbsp;<span class="loc-disp-selection"></span></div>
                                <div class="middle-select"><ul class="sub-menu-list" id="newlocationpaint" style="display:none;">
                                    '.$html.'
                                </ul>
                                <div id="qtip_addlocation_visible_disp_'.$qtipLocationIdInit.'" class="add-location-cls" style="display:none;">'.t('LBL603').'</div></div>
                              </li>
                            </ul>
                           </div>
                        </div>',
        );*/

     /*-----------End Custom Select Box------*/
        /* ------------------ Vincent changed for preformance issue START ------------*/
         // addHiddenFieldToAddEditForm($form, 'class_location', $defaultValue);

          //$locPreText   = ($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt') ? t('LBL114') : '';

         /* if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt')
              $locReadonly=false;
          else
              $locReadonly=true;
          if(($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt') && !empty($defaultValue)){
              $dt = getClassLocationAdd($defaultValue);
              $dTxt = $dt[0]->name;
              $locPreText = '';
          }else{*/
              /*if($cls_delivery_type == 'lrn_cls_dty_wbt' || $cls_delivery_type == 'lrn_cls_dty_vod' || $cls_delivery_type=='lrn_cls_dty_vcl'){
                $dTxt = '';
                $locPreText='';
              }else{*/
                $dTxt = t('LBL114');
                $locPreText=t('LBL114');
              //}
          //}
           if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl')
              $seatReadonly=false;
          else
              $seatReadonly=true;

          $defaultValue =  getDefaultAddEditFormFieldValue('min_seats', $form_state, $classDetails);
          // setting Max length into 9 for Int(11) => this issue fixed for this ticket #0035886
          addTextFieldToAddEditForm($form, 'min_seats', t('LBL566'), $defaultValue, 111, false,$seatReadonly,'9');
         // addACFieldToAddEditFormWithId($form, 'new_location', 'administration/learning/catalog/class-location-autocomplete', t('LBL041'), $dTxt, 106, array(), array('id' => "'class_location'"),true,$locReadonly,$locPreText);
          //addTextFieldToAddEditForm($form, 'new_location', t('LBL041'), $dTxt, 106, true, $seatReadonly);
        /* ------------------ Vincent changed for preformance issue END ------------*/

        /* $form['class-location-container-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
          );*/
       // }

        endAddEditTwoColumnRow($form);
        // End : Two column

        $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';
        $form['new_location_onload'] = array(
        '#type' => 'markup',
        '#prefix' => '<div style="display:none;"><img id= "edit-course-class-img" src="'.$pngPath.'" onload = "$(\'#root-admin\').data(\'narrowsearch\').getBubblePopup('.$qtipOptLocationObj.');"></div>',
        );

       if($form_state['triggering_element']['#name']=='delivery_type'){
         $form['class_loading_default_focus'] = array (
                    '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="changeLocationFocus(\'deltype\');" />',
                );
       }else{
         $form['class_loading_default_focus'] = array (
                    '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" style="display:none;" onload="changeLocationFocus(\'othertype\');" />',
                );
       }
        //if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){
          addTwoColumnRowToAddEditForm($form, 'regdate_deadlinedate');
         /* $defaultValue =  getDefaultAddEditFormFieldValue('reg_end_date', $form_state, $classDetails);
          if($defaultValue!='') $defaultValue = date('m-d-Y', strtotime($defaultValue));
          $form['class-register-date-container'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="class-register-date-container" id="admin-class-register-date-container">'
          );

          addDateFieldToAddEditForm($form, 'reg_end_date', t('LBL565'), $defaultValue, 'mm-dd-yyyy', 'all', t('LBL218'), 107);
         */
       // if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){
          $form['class-location-container'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="admin-class-location-container">'
          );
          $defaultValue =  getDefaultAddEditFormFieldValue('class_location', $form_state, $classDetails);

          addHiddenFieldToAddEditForm($form, 'class_location', $defaultValue);

          //$locPreText   = ($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt') ? t('LBL114') : '';

          if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt'){
            $locReadonly=false;
            $locMandatory = true;
          } else {
            $locReadonly=true;
            $locMandatory = false;
          }
          if(($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt') && !empty($defaultValue)){
              /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 and #0071329 */
              $dTxt = getClassLocationAdd($defaultValue);   
              $locPreText = '';
          }else{
              if($cls_delivery_type == 'lrn_cls_dty_wbt' || $cls_delivery_type == 'lrn_cls_dty_vod' || $cls_delivery_type=='lrn_cls_dty_vcl'){
                $dTxt = '';
                $locPreText='';
              }else{
                $dTxt = t('LBL114');
                $locPreText=t('LBL114');
              }
          }
        if (!empty($classId)) { // fixed for this ticket  #0035952
             $totalEnrollment= getTotalClassEnroll($classId); // get the Total Number of users Enrolled If users Enrolled means we wont make admin to change a location
             if($totalEnrollment > 0){
                $locReadonly=true;
             }
             expDebug::dPrint('$totalEnrollment : '.print_r($totalEnrollment,true), 4);
        }
          addACFieldToAddEditFormWithId($form, 'new_location', 'administration/learning/catalog/class-location-autocomplete', t('Location'), $dTxt, 106, array(), array('id' => "'class_location'"),$locMandatory,$locReadonly,$locPreText, true);
          /* Added/changed by ganeshbabuv on Feb 13th 2016 for 0071329: The Location filter search enhancement 66404 has to be implemented for Course/class creation screens - 1 */
          $form['new_location']['#maxlength']=500;
          //addTextFieldToAddEditForm($form, 'new_location', t('LBL041'), $dTxt, 106, true, $seatReadonly);
        /* ------------------ Vincent changed for preformance issue END ------------*/

          $form['class-location-container-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
          );
       // }

          /*$form['class-register-date-container-1'] = array(
            '#type' => 'markup',
            '#markup' => '</div>'
          );*/

           $form['class-max-seats'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="addedit-twocol-secondcol"><div id="admin-class-max-seats-container">'
            );
          $defaultValue =  getDefaultAddEditFormFieldValue('max_seats', $form_state, $classDetails);
          // setting Max length into 9 for Int(11) => this issue fixed for this ticket #0035886
          if($cls_delivery_type == 'lrn_cls_dty_wbt' || $cls_delivery_type == 'lrn_cls_dty_vod')
            $maxMandatory = false;
          else 
            $maxMandatory = true;
          addTextFieldToAddEditForm($form, 'max_seats', t('LBL567'), $defaultValue, 112, $maxMandatory, $seatReadonly,'9');
          $form['class-max-seats-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div></div>'
            );
          endAddEditTwoColumnRow($form);
      // }


        // Begin : Two column - delivery type, price
        addTwoColumnRowToAddEditForm($form, 'waitlist_count_currency_type');

         $defaultValue =  getDefaultAddEditFormFieldValue('reg_end_date', $form_state, $classDetails);
          if($defaultValue!='') $defaultValue = date('m-d-Y', strtotime($defaultValue));
         /* $form['class-register-date-container'] = array(
            '#type' => 'markup',
            '#markup' => '<div class="class-register-date-container" id="admin-class-register-date-container">'
          );*/

          addDateFieldToAddEditForm($form, 'reg_end_date', t('LBL565'), $defaultValue, t('LBL112'), 'all', t('LBL218'), 107);

       /*  $defaultValue =  getDefaultAddEditFormFieldValue('price', $form_state, $classDetails);
        addTextFieldToAddEditForm($form, 'price', t('Class Fee'), $defaultValue, 8, true); */

      /* $defaultValue =  getDefaultAddEditFormFieldValue('price', $form_state, $classDetails);

        $priceAttributes = array();
        $priceAttributes['class'][] = 'addedit-edit-price';
        $priceAttributes['class'][] = 'addedit-edit-textfield';
        $priceAttributes['tabindex'] = 109;

        $form['price'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="addedit-twocol-firstcol"><div class="addedit-new-field-class-fee class-fee-textfield-label">',
        '#suffix' => '</div>',
        //'#required' => TRUE,
        '#size' => 4,
        '#title' => t('LBL040').':',
        '#default_value' => $defaultValue,
        '#attributes' => $priceAttributes,
        );*/

      /*  $defaultValue =  getDefaultAddEditFormFieldValue('currency_type', $form_state, $classDetails);
        addDropDownToAddEditForm($form, 'currency_type', t('Currency Type'), getProfileItemNamesKeyed('cre_sys_crn_'), $defaultValue, 9, true); */

        /*$classDetails['currency_type'] = !empty($classDetails['currency_type']) ? $classDetails['currency_type'] : 'cre_sys_crn_usd';
        $defaultValue =  getDefaultAddEditFormFieldValue('currency_type', $form_state, $classDetails);

        $finalOptions = getProfileItemNamesKeyed('cre_sys_crn_');
          $form['currency_type'] = array(
          '#type' => 'select',
          '#title' => t('LBL265'),
          '#prefix' => '<div class="addedit-new-field-currency-type">',
          '#suffix' => '</div></div>',
          '#title_display' => 'invisible',
          '#options'=> $finalOptions,
          '#default_value' => $defaultValue,
          //'#disabled' => $readOnly,
          '#attributes' => array('tabindex' => '110','Class' => 'addedit-new-field-currency-type-select'),
        );
        */
        //if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){

        //}

          /* $form['class-waitlist-seats'] = array(
                '#type' => 'markup',
                '#markup' => '<div id="admin-class-waitlist-seats-container">'
            );*/
          $defaultValue =  getDefaultAddEditFormFieldValue('waitlist_count', $form_state, $classDetails);
          addTextFieldToAddEditForm($form, 'waitlist_count', t('Waitlist'), $defaultValue, 113, false,$seatReadonly);
          /* $form['class-waitlist-seats-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
            );
            $form['class-register-date-container-1'] = array(
            '#type' => 'markup',
            '#markup' => '</div>'
          );*/
        endAddEditTwoColumnRow($form);
        // End : Two column
          // Begin : Two column - waitlist_count,currency_type
        addTwoColumnRowToAddEditForm($form, 'delivery_type_price');

        $defaultValue =  getDefaultAddEditFormFieldValue('price', $form_state, $classDetails);
        expDebug::dPrint('exp_sp_administration_catalog_access.inc '.strlen($defaultValue), 5);
        // added This For Length default this Ticket #0036013
        if(strlen($defaultValue) >= 11){
            $defaultValue=(int)$defaultValue;
        }

        $priceAttributes = array();
        $priceAttributes['class'][] = 'addedit-edit-price';
        $priceAttributes['class'][] = 'addedit-edit-textfield';
        $priceAttributes['tabindex'] = 108;
        $priceAttributes['onkeydown'] = 'return onlyNumbersforPrice(event)';

        $form['price'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div class="addedit-twocol-firstcol"><div class="addedit-new-field-class-fee class-fee-textfield-label">',
        '#suffix' => '</div>',
        //'#required' => TRUE,
        '#size' => 4,
        '#title' => t('LBL040').':',
        '#default_value' => $defaultValue,
        '#attributes' => $priceAttributes,
        '#maxlength'=> 10, // added This Max lenght For this Ticket #0036013
        );

        $classDetails['currency_type'] = !empty($classDetails['currency_type']) ? $classDetails['currency_type'] : 'USD';
        $defaultValue =  getDefaultAddEditFormFieldValue('currency_type', $form_state, $classDetails);

        $finalOptions = getProfileItemNamesKeyed('cre_sys_crn_');
          $form['currency_type'] = array(
          '#type' => 'select',
          '#title' => t('LBL101').' '.t('LBL036'),
          '#prefix' => '<div class="addedit-new-field-currency-type">'.$newuiPrefix,
          '#suffix' => $newuiSuffix.'</div></div>',
          '#title_display' => 'invisible',
          '#options'=> $finalOptions,
          '#default_value' => $defaultValue,
          //'#disabled' => $readOnly,
          '#attributes' => array('tabindex' => '109','Class' => 'addedit-new-field-currency-type-select'),
        );

        //if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){

        //}
        $form['class-max-seats-new-start'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="addedit-twocol-secondcol"><div id="admin-class-max-seats-container">'
            );

        if($cls_delivery_type == 'lrn_cls_dty_wbt' || $cls_delivery_type == 'lrn_cls_dty_vod'){
              $durReadonly=false;
         }else{
              $durReadonly=true;
         }

        $defaultValue =  getDefaultAddEditFormFieldValue('scheduled_duration', $form_state, $classDetails);
       addTextFieldToAddEditForm($form, 'scheduled_duration', t('LBL248'), $defaultValue, 114, false,$durReadonly,50);

         $form['class-max-seats-new-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div></div>'
            );

        endAddEditTwoColumnRow($form);
          // End : Two column
        $form['class-authorvendor-container-start'] = array(
                '#type' => 'markup',
                '#markup' => '<div class="admin-class-authorvendor-container">'
          );
        $defaultValue =  getDefaultAddEditFormFieldValue('author_vendor', $form_state, $classDetails);
        addTextFieldToAddEditForm($form, 'author_vendor', t('LBL269'), $defaultValue, 110, false,false,50);

         $form['class-authorvendor-container-end'] = array(
                '#type' => 'markup',
                '#markup' => '</div>'
            );
    // added for #0046671 
       // addHiddenFieldToAddEditForm($form, 'ckeditor_word_count1', '5000' ); // commented for 0076233
                        $form['addedit-edit-additionalinfo-container'] = array(
                        '#type'   => 'markup',
                        '#markup' => '
                        <div class="addedit-long-description-container">
                        ',
                        );
        $defaultValue =  getDefaultClassDescriptionAddEditFormFieldValue('additional_info', $form_state, $classDetails);
        addTextFormatFieldToAddEditForm($form, 'additional_info', t('LBL3068'), $defaultValue, 'full_html', 104, false,false,500);
                        $form['addedit-edit-additionalinfo-container1'] = array(
                        '#type'   => 'markup',
                        '#markup' => '
                        </div>',
                        );
        addTwoColumnRowToAddEditForm($form, 'is_shown_is_noti' );
        $defaultValue =  getDefaultAddEditFormFieldValue('addn_catalog_show', $form_state, $classDetails);
        $path = $base_url.'/sites/all/themes/core/expertusoneV2/expertusone-internals/images';
        $choiceCheckboxAttributes['onclick'][] = 'checkboxSelectedUnselectedAddInf(this);';
        $className = ($form_state['input']['addn_catalog_show']==1 || $defaultValue==1)? "checkbox-selected" : "checkbox-unselected";
                            
                            
        $form['addn_catalog_show'] = array(
         '#type' => 'checkbox',
         '#prefix' => '<div class="addedit-twocol-firstcol"><div class="catalog-block"><label class="show-label-catalog">'.t("LBL576").':</label><img src="'.$path.'/help.png" class="vtip info-enr-upload test-help-iconcatalog" title="'.t('MSG818').'" onload="vtip();" />
         <div class="'.$className.'">',         
         '#suffix' => '</div></div>',
         '#attributes' => $choiceCheckboxAttributes,
         '#default_value' => $defaultValue,
          );
        

        $defaultValue =  getDefaultAddEditFormFieldValue('addn_notification_show', $form_state, $classDetails);
        $className = ($form_state['input']['addn_notification_show']==1 || $defaultValue==1)? "checkbox-selected" : "checkbox-unselected";
                            
                            
        $form['addn_notification_show'] = array(
        '#type' => 'checkbox',
        '#prefix' => '<div class="notifi-block"><label class="show-label-notifications">'.t("LBL3069").':</label><img src="'.$path.'/help.png" class="vtip info-enr-upload test-help-iconnotification" title="'.t('MSG819').'" onload="vtip();" />
        <div class="'.$className.'">',         
        '#suffix' => '</div></div></div></div>',
        '#attributes' => $choiceCheckboxAttributes,
        '#default_value' => $defaultValue,
        );
        
        if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status / Add the custom attrubutes
            renderCustomAttributesInAddEditForm($form, $form_state,'cre_sys_obt_cls',$classId);
            
            $saved_first_time=$form_state['storage']['class_saved_first_time'];
            expDebug::dPrint('Before Unset $saved_first_time = '.$saved_first_time,5);
            if($saved_first_time!='1'){
            	if(!in_array($form_state['triggering_element']['#name'],array('Content Details','Session Details','survey','assessment','enrolluser'))){
            		//Scroll issue in ClassAddEDITFORM #88022
            		$form['class_detail_scroll_wrapper_end'] = array(
            				'#type'       => 'markup',
            				'#prefix'     => '</div>',
            		);
            	}
            }
            if($saved_first_time=='1'){ 
            	$form_state['storage']['class_saved_first_time']='0';
            }
            expDebug::dPrint('After Unset $saved_first_time ='.$form_state['storage']['class_saved_first_time'],5);
         
            addCustomAttributesBlockToAddEditForm($form, $form_state, $form_state['class_id'], 'class', 'class-custom-attributes', 'catalog-class-basic-addedit-form-'.$form_state['class_id']);
        }else{  //To display custom fields  
            addCustomFieldsBlockToAddEditForm($form, $form_state, $form_state['class_id'], 'class', 'class-custom-fields'.$form_state['class_id'], 'catalog-class-basic-addedit-form-'.$form_state['class_id'], 118); 
        } //#custom_attribute_0078975 - End Check module status
      

      }

      } // End View / Edit mode
      if(empty($form_state['class_id'])){
        $entityId = $form_state['values']['id'];
      }else{
        $entityId = $form_state['class_id'];
      }

      $entityType = 'cre_sys_obt_cls';

      $uniqueId = $entityId.'-'.$entityType;

      if(!empty($entityId)){
       /* $form['list_type_'.$uniqueId] = array(
          '#type' => 'select',
          //'#multiple' =>TRUE,
          '#prefix' => '<div class="admin-course-add-attachment admin-course-add-attachment-select">',
          '#suffix' => '</div>',
          '#options' => array(
            ''   => t('Add'),
            'attachment_fieldset'     => t('Attachment'),
            'tag_fieldset'            => t('Tags'),
            'survey_fieldset'         => t('Survey'),
            'assessment_fieldset'     => t('Assessment'),
            'custom_fieldset'         => t('Custom Fields'),
            'class_register_fieldset' => t('Enroll User'),
          ),
          '#attributes' => array('onchange' => 'showHide(this, "'.$uniqueId.'");selecteDropdown();','tabindex' => '19','Class' => 'select'),
        );  */
          if($form_state['storage']['substage'] == 'Attachment') {
          // To display the Attachment UI
          if($form_state['storage']['step'] == 'view'){
            attachmentDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
          } else {
            displayAttachmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);
          }
        }
        if($form_state['storage']['substage'] == 'tag') {
          // To display the Tag UI
          if($form_state['storage']['step'] == 'view'){
            tagDetailsView($form, $form_state, $entityId, 'Class', $uniqueId);
          } else {
            displayTagDetails($form, $form_state, $entityId, 'Class', $uniqueId);
          }
        }
        if($form_state['storage']['substage'] == 'survey') {
          $showCancelSave = 0;
          // To display the survey UI
          displaySurveyDetails($form, $form_state, $entityId, $entityType, $uniqueId);
        }

        if($form_state['storage']['substage'] == 'assessment') {
          $showCancelSave = 0;
          // To display the assessment UI
          displayAssessmentDetails($form, $form_state, $entityId, $entityType, $uniqueId);
        }

        if($form_state['storage']['substage'] == 'enrolluser') {
          $showCancelSave = 0;
          // User Registration
          displayClassRegisterDetails($form, $form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType);
        }

        if($form_state['storage']['substage'] == 'custom') {
          // To display the Custom Fields UI
          if($form_state['storage']['step'] == 'view'){
            customFieldsDetailsView($form, $form_state, $entityId, $entityType, $uniqueId);
          } else {
            displayCustomFieldsDetails($form, $form_state, $entityId, $entityType, $uniqueId);
          }
        }

        if($form_state['storage']['substage'] == 'business_rules') {
           displayBusinessRulesToAddEditForm($form, $form_state, $entityId, $entityType, $uniqueid, $clsDeliveryType, true);
           if($clsDeliveryType != 'lrn_cls_dty_vod' && $clsDeliveryType != 'lrn_cls_dty_wbt'){
             $showCancelSave = 0;
           }
        }

        if(!empty($entityId) && ( $clsDeliveryType == 'lrn_cls_dty_wbt' || $clsDeliveryType == 'lrn_cls_dty_vod') ){
          //$form['list_type_'.$uniqueId]['#options']['wbt_detail_fieldset'] = t('Add Content');
          // To display the WBT Details UI
          if($form_state['storage']['substage'] == 'content' ||  ($form_state['storage']['new_entity'])) {
            $form_state['storage']['substage'] = 'content';
            $showCancelSave = 0;
            displayContentDetails($form, $form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType, $readOnlyForView);
          }
        } elseif(!empty($entityId) && ($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl')){
          //$form['list_type_'.$uniqueId]['#options']['session_details_fieldset'] = t('Add Session Details');
          // To display the Session Details UI
          if($form_state['storage']['substage'] == 'sessiondetail' || ($form_state['storage']['new_entity'])) {
            $form_state['storage']['substage'] = 'sessiondetail';
            $showCancelSave = 0;
            unset($form['session_details_fieldset']);
            expDebug::dPrint('checking the argument list with details of ids'.print_r($form_state,true).'entity id'.$entityId);
            displaySessionDetails($form, $form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType, $readOnlyForView, $oldClassId, $form_state['storage']['new_entity_class']);
          }
        }

        if(!empty($entityId)){
          if($form_state['storage']['substage'] == 'notifications') {
            $form_state['storage']['substage'] = 'notifications';
            addNotificationsToAddEditForm($form, $entityId, $entityType, $readOnlyForView);
          }
        }

      }

       if($showCancelSave && $form_state['storage']['display_mode'] != 'view') {
          //addEditFormWithCancelAndSaveClose($form, 'catalog-class-basic-addedit-form-'.$classId, 'newtheme-save-button', 'newtheme-cancel-link');
          $subStage = $form_state['storage']['substage'];
          addEditFormWithCatalogCancelAndSaveClose($form, 'catalog-class-basic-addedit-form-'.$classId, 'newtheme-save-button-'.$classId, 'newtheme-cancel-link-'.$classId, $subStage,$classId,$courseId,$emptyId);
       } else {
         /*
         $form['class_end_markup'] = array(
         '#type'   => 'markup',
         '#prefix' => '<div class="admin-save-button-container">',
         '#suffix' => '</div>' .
                      '<div class="clearBoth"></div>' .
                      '<div class="admin-class-course-separater"></div>',
         );
         */
       }


      $form['class_detail_disp_end'] = array(
        '#type'       => 'markup',
        '#suffix'     => '<div class="clearBoth"></div>
                          <div class="round-corner-footer-left"><div class="round-corner-footer-right">
                          <div class="round-corner-footer-middle"></div></div></div></div></div>',
      );
      if($showCancelSave && $form_state['storage']['display_mode'] != 'view'){
        $form['class_detail_wrapper_end'] = array(
          '#type'       => 'markup',
          '#suffix'     => '</div>',
        );
      } else {
        $form['class_detail_wrapper_end'] = array(
          '#type'       => 'markup',
          '#suffix'     => '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
        );
      }

      addHiddenFieldToAddEditForm($form, 'show_catag', 1);
      return $form;
  }catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_catalog_class_addedit_form', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * @param $fields
 * @param $form_state
 * @param $classDetails
 * @return Ambigous <string, unknown, multitype:>
 */
function getDefaultClassAddEditFormFieldValue($fields, $form_state, $classDetails) {
  try {
    $defaultValue   = '';
    $classDefaultValue  = array();
    for($i=0;$i<count($fields);$i++) {
      $field                   = $fields[$i];
      if ($field == 'description') {
        $classDefaultValue[$field] = empty($form_state['values'][$field]) ? (!empty($classDetails[$field]) ? $classDetails[$field] : '') : $form_state['values'][$field]['value'];
      } else {
        $classDefaultValue[$field] = empty($form_state['values'][$field]) ? (!empty($classDetails[$field]) ? $classDetails[$field] : '') : $form_state['values'][$field];
      }
    }
    //print_r($classDefaultValue);
    return $classDefaultValue;
  }catch (Exception $ex) {
    watchdog_exception('getDefaultClassAddEditFormFieldValue', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * @param $form
 * @param $form_state
 * @param $defaultValue
 */
function viewClassDetails(&$form, &$form_state, $defaultValue){

    try {

        $clsDeliveryType =  getDefaultAddEditFormFieldValue('delivery_type', $form_state, $classDetails);

        $form_state['storage']['substage'] = 'details';

        $form['cls_title'] = array(
          '#type'     => 'markup',
          '#markup'   => '<div class="addedit-new-field"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL262').' : </div><div class="addedit-new-field-value"><div class="form-item form-type-textfield form-item-code">'.
                         '<label for="edit-code--2" class="element-invisible">'.t('LBL262').'</label>'.
                           '<div class="class-view-mode-fields-value">'.$defaultValue['title'].'</div>'.
                       '</div>'.
                         '</div><div class="clearBoth"></div></div>',
        );

        $form['cls_code'] = array(
          '#type'     => 'markup',
          '#markup'   => '<div class="addedit-new-field"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL263').' : </div><div class="addedit-new-field-value"><div class="form-item form-type-textfield form-item-code">'.
                         '<label for="edit-code--2" class="element-invisible">'.t('LBL263').'</label>'.
                           '<div class="class-view-mode-fields-value">'.$defaultValue['code'].'</div>'.
                       '</div>'.
                         '</div><div class="clearBoth"></div></div>',
        );


       $form['cls_description'] = array(
          '#type'     => 'item',
          '#title'    => t('LBL098').':',
        '#prefix'   => '<div><div class="addedit-course-field-title class-view-mode-fields">',
          '#markup'   => '</div><div class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">'.$defaultValue['description'].'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
       );

        $form['cls_status_code'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol "><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL102').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL102').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['status_name'].'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div><div class="addedit-twocol-secondcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL038').':</div><div class="addedit-secondcol-field-value"><div class="form-item form-type-select form-item-lang-code">
                            <label for="edit-lang-code" class="element-invisible">'.t('LBL038').' </label>
                            <div class="addedit-edit-lang_code addedit-edit-dropdown form-select  class-view-mode-fields-value">
                            '.t($defaultValue['language_name']).'
                            </div>
                            </div>
                            </div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>',);

        if(!empty($defaultValue['status_inactive_reason'])) {
          $form['status_inactive_reason'] = array(
            '#type'     => 'item',
            '#title'    => t('LBL412').':',
            '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title class-view-mode-fields">',
            '#markup'   => '</div><div class="addedit-edit-lang_code addedit-edit-dropdown form-select  class-view-mode-fields-value">'.$defaultValue['status_inactive_reason'].'</div><div class="clearBoth"></div>',
            '#suffix'   => '</div>',
          );
         }

        $form['cls_delivery_currency'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL084').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL084').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['delivery_type_name'].'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div><div class="addedit-twocol-secondcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL264').':'.'</div><div class="addedit-secondcol-field-value"><div class="form-item form-type-select form-item-lang-code">
                            <label for="edit-lang-code" class="element-invisible">'.t('LBL264').':'.' </label>
                            <div class="addedit-edit-lang_code addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['price'].' '.current(explode(' ',$defaultValue['currency_name'])).'
                            </div>
                            </div>
                            </div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>',
                                    );


        if(empty($classId)){
          $form['delivery_data_disp_start'] = array(
            '#type' => 'markup',
            '#markup' => '<div id="admin-class-delivery-data-part">'
          );
        }
        if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){

        $form['cls_min_max'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL266').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL266').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['min_seats'].'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div><div class="addedit-twocol-secondcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL267').':</div><div class="addedit-secondcol-field-value"><div class="form-item form-type-select form-item-lang-code">
                            <label for="edit-lang-code" class="element-invisible">'.t('LBL267').' </label>
                            <div class="addedit-edit-lang_code addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['max_seats'].'
                            </div>
                            </div>
                            </div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>',
                            );

        $form['cls_waitlist_location'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL268').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL268').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['waitlist_count'].'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div><div class="addedit-twocol-secondcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('Location').':</div><div class="addedit-secondcol-field-value"><div class="form-item form-type-select form-item-lang-code">
                            <label for="edit-lang-code" class="element-invisible">'.t('Location').' </label>
                            <div class="addedit-edit-lang_code addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['class_location_name'].'
                            </div>
                            </div>
                            </div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>',
                            );

        }
        if(empty($classId)){
          $form['delivery_data_disp_end'] = array(
          '#type' => 'markup',
          '#markup' => '</div>'
          );
        }

        $form['cls_waitlist_location'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL269').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL269').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$defaultValue['author_vendor'].'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div><div class="addedit-twocol-secondcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL270').':'.'</div><div class="addedit-secondcol-field-value"><div class="form-item form-type-select form-item-lang-code">
                            <label for="edit-lang-code" class="element-invisible">'.t('LBL270').':'.' </label>
                            <div class="addedit-edit-lang_code addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.(($defaultValue['export_compliance'] == 'Y') ? 'Yes' : 'No').'
                            </div>
                            </div>
                            </div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>',
                            );



        if($cls_delivery_type == '' || $cls_delivery_type == 'lrn_cls_dty_ilt' || $cls_delivery_type == 'lrn_cls_dty_vcl'){
        $regEndDate = '';
        if(!empty($defaultValue['reg_end_date'])){
          $regEndDate = date('Y-m-d', strtotime($defaultValue['reg_end_date']));
        }
        $form['cls_reg_end_date'] = array(
              '#type'   => 'markup',
              '#markup' => '<div id="two-col-row-lang_code_status_status_temp" class="addedit-new-field addedit-new-field-two-col"><div class="addedit-twocol-firstcol"><div class="addedit-new-field-title class-view-mode-fields">'.t('LBL336').':</div><div class="addedit-firstcol-field-value"><div class="form-item form-type-select form-item-status">
                                <label for="edit-status" class="element-invisible">'.t('LBL336').' </label>
                            <div  class="addedit-edit-status addedit-edit-dropdown form-select class-view-mode-fields-value">
                            '.$regEndDate.'
                            </div >
                            </div>
                            </div><div class="clearBoth"></div></div></div>',
                            );
        }

        /*
       $form['cls_additional_info'] = array(
          '#type'     => 'item',
          '#title'    => t('LBL156').':',
        '#prefix'   => '<div class="admin-course-class-long-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
          '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['additional_info'].'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div></div>',
       );
       */

        if(empty($classId)){
          $form['delivery_data_disp_for_date_start'] = array(
            '#type' => 'markup',
            '#markup' => '<div id="admin-class-delivery-data-part-for-date">'
          );
        }
        if(empty($classId)){
          $form['delivery_data_disp_for_date_end'] = array(
          '#type' => 'markup',
          '#markup' => '</div>'
          );
        }
   } catch (Exception $ex) {
    watchdog_exception('viewClassDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * exp_sp_administration_catalog_class_addedit_form_validate() - Class form validation.
 */
function exp_sp_administration_catalog_class_addedit_form_validate($form, &$form_state){
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' test content ' .$form_state['triggering_element']['#name'],5);
 try {
    global $secure_entities;
    /* (module_exists('exp_sp_esignature') && isActiveEsignRegion())   Is Fixed E-sign Enable issue */
    if(strrpos($form_state['triggering_element']['#name'],'content') || (module_exists('exp_sp_esignature') && isActiveEsignRegion())){
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_content_details.inc');
    }
    if(strrpos($form_state['triggering_element']['#name'],'survey') || (module_exists('exp_sp_esignature') && isActiveEsignRegion())){
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_survey.inc');
    }
    //if(strrpos($form_state['triggering_element']['#name'],'enrolluser')){
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_class_register.inc');
    //}
    if(strrpos($form_state['triggering_element']['#name'],'assessment') || (module_exists('exp_sp_esignature') && isActiveEsignRegion())){
        include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_assessment.inc');
    }
      if($form_state['triggering_element']['#name'] == 'delivery_type') {
        return ;
      }
        
        $form_state['hidden-save-submit'] = false;
        $form_state['esign_show_popup']= false;

        if (!empty($form_state['storage']['new_entity'])) {
          unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
          unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
        }

        if (!empty($form_state['storage']['edited_entity'])) {
            unset($form_state['storage']['edited_entity']); // Unset the variable to remove it from $form_state
        }

        if ($form_state['values']['op'] == t('LBL109') || $form_state['triggering_element']['#name'] == 'cancel' || $form_state['triggering_element']['#name'] == 'hiddencancel') {
            $form_state['input'] = array(); // Clear user input - raw
            $form_state['values'] = array(); // Clear user input - security cleared
          unset($form_state['custom_fields']); // Clear any change made to custom fields
            $form_state['rebuild'] = TRUE; // Rebuild the form. Submit handler will not be called.
          return;
        }
      if ($form_state['triggering_element']['#name'] == 'details') {
        $form_state['display_class']   = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'details';
        return;
      }

      /*if ($form_state['triggering_element']['#name'] == 'Attachment') {
        $form_state['display_class']   = TRUE;
        $form_state['show_add_another'] = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'Attachment';
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
        $form_state['storage']['substage'] = 'tag';
        unset($form_state['delete_tag_index']);
        unset($form_state['delete_tag_fieldset']);
        $form_state['num_tag'] = 0;
        return;
      } */
      if ($form_state['triggering_element']['#name'] == 'survey') {
        //$form_state['display_class']   = TRUE;
        //$form_state['input']     = array(); // Clear user input - raw
        //$form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'survey';
        //unset($form_state['delete_survey_index']);
        //unset($form_state['delete_survey_fieldset']);
        //$form_state['num_survey'] = 0;
        return;
      }
      if ($form_state['triggering_element']['#name'] == 'assessment') {
        //$form_state['display_class']   = TRUE;
        //$form_state['input']     = array(); // Clear user input - raw
        //$form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'assessment';
        //unset($form_state['delete_assessment_index']);
        //unset($form_state['delete_assessment_fieldset']);
        //$form_state['num_assessment'] = 0;
        return;
      }
      if ($form_state['triggering_element']['#name'] == 'enrolluser') {
       // $form_state['display_class']   = TRUE;
       // $form_state['input']     = array(); // Clear user input - raw
       // $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'enrolluser';
        return;
      }
      /*if ($form_state['triggering_element']['#name'] == 'custom') {
        $form_state['display_class']   = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'custom';
        unset($form_state['delete_custom_index']);
        unset($form_state['delete_custom_fieldset']);
        $form_state['num_custom'] = 0;
        return;
      }*/
      if ($form_state['triggering_element']['#name'] == 'notifications') {
        $form_state['display_class']   = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'notifications';
        return;
      }
      if ($form_state['triggering_element']['#name'] == 'Session Details') {
        $form_state['display_class']   = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'sessiondetail';
        unset($form_state['delete_session_index']);
        unset($form_state['delete_session_details_fieldset']);
        unset($form['session_details_fieldset']);
        $form_state['num_session'] = 0;
        return;
      }
      if ($form_state['triggering_element']['#name'] == 'Content Details') {
       // $form_state['display_class']   = TRUE;
       // $form_state['input']     = array(); // Clear user input - raw
       // $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'content';
      //  unset($form_state['delete_wbt_index']);
      //  unset($form_state['delete_wbt_fieldset']);
     //   $form_state['num_wbt'] = 0;
        return;
      }

      if ($form_state['triggering_element']['#name'] == 'business_rules') {
        $form_state['display_class']   = TRUE;
        $form_state['input']     = array(); // Clear user input - raw
        $form_state['values']    = array(); // Clear user input - security cleared
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'business_rules';
        return;
      }


      if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
        $triggerValue  = 'save';
      }else{
        $triggerValue  = 'hidden-save';
      }
      $triggerElementByName  = $form_state['triggering_element']['#name'];      
      if ($form_state['triggering_element']['#name'] == 'save' || $form_state['triggering_element']['#name'] == 'saveandshow' || $form_state['triggering_element']['#name'] == 'hidden-save'){
        $form_state["storage"]["form_validate"] = true;
        $form_state['esign_show_popup']= true;
        if($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) {
          // Code duplicate check
          $classCode = $form_state['values']['code'];
          $classId = $form_state['values']['id'];
          expDebug::dPrint('Class ID:'.$classId , 5);
          $classResult = classCodeDupeCheck($classCode, $classId,$form_state['values']['lang_code']);

          $deliveryType  =  $form_state['values']['delivery_type'];
          expDebug::dPrint('$deliveryType ID:'.$deliveryType , 4);
          if($deliveryType == "0" || $deliveryType == 'lrn_cls_dty_vcl') {
            addEditCheckMandatoryFields($form, $form_state, array('title', 'code', ));
            if (empty($form_state['values']['short_description']['value'])){
                form_set_error('short_description', $form['short_description']['#title'] . t('ERR101'));
                $form_state['esign_show_popup']= false;
            }
            if($deliveryType == "0")
                addEditCheckMandatoryFields($form, $form_state, array('lang_code', 'delivery_type', 'currency_type'));
            else
                addEditCheckMandatoryFields($form, $form_state, array('lang_code', 'delivery_type', 'currency_type', 'max_seats'));
          }else if ($deliveryType == 'lrn_cls_dty_ilt') {
            addEditCheckMandatoryFields($form, $form_state, array('title', 'code', ));
            if (empty($form_state['values']['short_description']['value'])){
                form_set_error('short_description', $form['short_description']['#title'] . t('ERR101'));
                $form_state['esign_show_popup']= false;
            }
            addEditCheckMandatoryFields($form, $form_state, array( 'lang_code', 'delivery_type', 'currency_type', 'max_seats'));
            if (empty($form_state['values']['new_location']) || $form_state['values']['new_location']==t('LBL114')){
                form_set_error('new_location', $form['new_location']['#title'] . t('ERR101'));
                $form_state['esign_show_popup']= false;
            }
            elseif(empty($form_state['values']['class_location']) || !empty($form_state['values']['new_location'])){
              
              /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 and #0071329  */
                         
              $new_location=trim($form_state['values']['new_location']);
              $locaion_id=getClassLocationID($new_location);
              expDebug::dPrint('$location_id === '.$locaion_id,5);
              
              if ((empty($locaion_id)) && !empty($form_state['values']['id'])) {
              $class_id = $form_state['values']['id'];
                  if($class_id){
              $default= db_query("SELECT location_id from slt_course_class where id=$class_id")->fetchField();
              $def_loc = getLocationDetails($default, array('name'));
              }
              }
              if($locaion_id || ( $form_state['values']['new_location'] == $def_loc['name'])){
                $form_state['values']['class_location'] = !empty($locaion_id) ? $locaion_id : $default;
              }
              else{
                form_set_error('new_location', t('ERR171') .' '.$form['new_location']['#title']);
              }
            }

            /*-- #35708: Session duplication issue while updating location --*/
            $select = db_select('slt_course_class_session', 'session');
            $select->addField('session', 'id');
            $select->addField('session', 'title', 'session_title');
            $select->addExpression("DATE_FORMAT(session.start_date, '%Y-%m-%d')", "start_date");
            $select->addField('session', 'start_time');
            $select->addField('session', 'end_time');
            $select->condition('course_id', $form_state['course_id']);
            $select->condition('class_id', $form_state['class_id']);
            $results = $select->execute()->fetchAll();
            // Check for location conflict if session already avilable
            if (count($results) > 0){
              foreach($results as $result) {
                 $sessId = $result->id;
                 $locationId = $form_state['values']['class_location'];
                 $time_zone = getLocationTimezone($locationId);
                 if(isset($time_zone)){
                  $tzArray   = getProfileListItemByCode($time_zone);
                  $time_zone = $tzArray[0]->attr2;
                 }else{
                  $time_zone = date_default_timezone(FALSE);
                 }
                 $vSesTimeZone = $time_zone;
                 $stDate = $result->start_date. " " .$result->start_time;
                 $enDate = $result->start_date. " " .$result->end_time;
                 $vSysPHPTimezonename = date_default_timezone(FALSE);
                 $ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
                 $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
                 $cStartDT = strtotime($ses_start_date->format('Y-m-d H:i:s'));
                 $cEndDT = strtotime($ses_end_date->format('Y-m-d H:i:s'));
                 //expDebug::dPrint('current session time:'.$sessId.'======>'. $vSesTimeZone. '|'.$cStartDT .'|||'. $cEndDT  , 4);
                 $locationScheduled = getLocationScheduledSession($locationId,$sessId);

                  foreach($locationScheduled as $id => $row){
                     $roomSessId    = $row->sessionid;
                     $vSesTimeZone = $row->session_timezone;
                     $stDate = $row->ses_start_date. " " .$row->start_time;
                     $enDate = $row->ses_start_date. " " .$row->end_time;
                     $vSysPHPTimezonename = date_default_timezone(FALSE);
                     $ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
                     $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
                     $roomSesStartTime = strtotime($ses_start_date->format('Y-m-d H:i:s'));
                     $roomSesEndTime = strtotime($ses_end_date->format('Y-m-d H:i:s'));
                     //expDebug::dPrint('$session time:'.$roomSessId.'======>'. $vSesTimeZone. '|'.$roomSesStartTime .'|||'. $roomSesEndTime  , 4);
                     if(($roomSesStartTime < $cStartDT && $roomSesEndTime <= $cStartDT) || ($roomSesStartTime >= $cEndDT && $roomSesEndTime > $roomSesStartTime) ) {
                     }
                     else{
                      form_set_error('', $result->session_title . ' - ' . t('ERR146'));
                      $form_state['esign_show_popup']= false;
                      break;
                     }
                  }
                }
            }
          }else {
            addEditCheckMandatoryFields($form, $form_state, array('title', 'code', ));
            if (empty($form_state['values']['short_description']['value'])){
                form_set_error('short_description', $form['short_description']['#title'] . t('ERR101'));
                $form_state['esign_show_popup']= false;
            }
            // Check all mandatory fields exist
            addEditCheckMandatoryFields($form, $form_state, array('lang_code', 'delivery_type', 'currency_type',));
          }

          //session seat validation
          $min_seats = $form_state['values']['min_seats'];
          $max_seats = $form_state['values']['max_seats'];
          $waitlist_count = $form_state['values']['waitlist_count'];

          if(count($classResult) > 0){
            form_set_error('code', $form['code']['#title'] . t('LBL271'));
            $form_state['esign_show_popup']= false;
            $form_state["storage"]["form_validate"] = false;
          }
        
        // Validation for ticket 22364//
          if(!empty($max_seats)){
             if(is_numeric($max_seats)){
                 $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
               $enrollSeatCount = getClassEnrollmentCountForRegistrationStatuses(null, $classId, $regStatusesList);
               expDebug::dPrint('Max seat values = '.$max_seats,4);
              //  error message for floating value #0034729
               if(strpos($max_seats,'.') == true){
                    form_set_error('max_capacity_in_value', t('LBL567').' '.strtolower(t('LBL106')).' '.t('ERR185').'.'); // Max Capacity In cannot have decimal Value
                    $form_state["storage"]["form_validate"] = false;
                    $form_state['esign_show_popup']= false;
               }else if($max_seats < $enrollSeatCount){
                        expDebug::dPrint('enrolled-->'.$enrollSeatCount.'--max--->'.$max_seats,4);
                        form_set_error('max_seats', t('ERR180'));
                        $form_state["storage"]["form_validate"] = false;
                        $form_state['esign_show_popup']= false;
                }
            }
          }

         if(!empty($waitlist_count) && !empty($max_seats)){
             if(is_numeric($waitlist_count) && is_numeric($max_seats)){
                 $regStatusesList = array('lrn_crs_reg_wtl');
               $waitlistSeatCount =  getClassEnrollmentCountForRegistrationStatuses(null, $classId, $regStatusesList);
                 if($waitlist_count < $waitlistSeatCount){
                    expDebug::dPrint('wait-->'. $waitlistSeatCount .'--countwait--->'.$waitlist_count,4);
                        form_set_error('waitlist_count', t('ERR183'));
                        $form_state["storage"]["form_validate"] = false;
                        $form_state['esign_show_popup']= false;
                }
            }
          }
          if(!empty($min_seats) && !empty($max_seats)){
            if(is_numeric($min_seats) && is_numeric($max_seats)){
                if($min_seats > $max_seats){
                  form_set_error('max_seats', t('LBL267').' '.t('ERR087').' '.t('LBL266').'.');
                  $form_state["storage"]["form_validate"] = false;
                  $form_state['esign_show_popup']= false;
                }else if(strpos($min_seats,'.') == true){
                    form_set_error('min_capacity_in_value', t('LBL1071').' '.strtolower(t('LBL106')).' '.t('ERR185').'.'); // Min Capacity In cannot have decimal Value and fixed for this ticket #0035886
                    $form_state["storage"]["form_validate"] = false;
                    $form_state['esign_show_popup']= false;
              }
            }
          }
         $location_id = $form_state['values']['class_location'];
         if($location_id){
           $maxSeatCount = getLocationMaxSeatCount($location_id);
           if(!empty($max_seats) && !empty($maxSeatCount)){
                if($max_seats > $maxSeatCount){
                drupal_set_message(t('ERR181'));
                $form_state['storage']['maxseat_warning'] = true;
               }
           }
         }
          // comment waitlist validation
         /* if(!empty($waitlist_count) && !empty($max_seats)){
            $form_state["storage"]["form_validate"] = TRUE;
            if(is_numeric($waitlist_count) && is_numeric($max_seats)){
                if($waitlist_count > $max_seats){
                  form_set_error('max_seats', t('ERR088'));
                  $form_state["storage"]["form_validate"] = false;
                }
            }
          }*/
          if(!is_numeric($min_seats) && !empty($min_seats)){
                //$form_state["storage"]["form_validate"] = TRUE;
                form_set_error('min_seats', t('ERR097').' '.t('LBL266').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
          }
          if(!is_numeric($max_seats) && !empty($max_seats)){
                //$form_state["storage"]["form_validate"] = TRUE;
                form_set_error('max_seats', t('ERR097').' '.t('LBL267').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
          }
          if(!is_numeric($waitlist_count) && !empty($waitlist_count)){
                //$form_state["storage"]["form_validate"] = TRUE;
                form_set_error('waitlist_count', t('ERR097').' '.t('LBL268').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
          }
          if(!is_numeric($form_state['values']['price']) && !empty($form_state['values']['price'])){
                //$form_state["storage"]["form_validate"] = TRUE;
                form_set_error('price', t('ERR097').' '.t('LBL040').'.');
                $form_state["storage"]["form_validate"] = false;
                $form_state['esign_show_popup']= false;
          }
          if(trim($form_state['values']['price']) < 0){
            form_set_error('price', t('ERR097').' '.t('LBL040').'.');
            $form_state["storage"]["form_validate"] = false;
            $form_state['esign_show_popup']= false;
          }
          $regEndDate='';
          if(!empty($form_state['values']['reg_end_date']) && ($form_state['values']['reg_end_date']!= t('LBL112'))){
            if(!validateDateFormat(trim($form_state['values']['reg_end_date']), 'm-d-Y'))
                form_set_error('reg_end_date', t('ERR242'));
            $regEndDate  = $form_state['values']['reg_end_date'];
          }

          $cancelDeadlineDate='';
          if(!empty($form_state['values']['cancel_deadline_date']) && ($form_state['values']['cancel_deadline_date']!= t('LBL112'))){
            $cancelDeadlineDate = $form_state['values']['cancel_deadline_date'];
          }
          if(!empty($regEndDate) && !empty($cancelDeadlineDate)){
            if($cancelDeadlineDate<$regEndDate){
              form_set_error('cancel_deadline_date', t('ERR100'));
              $form_state['esign_show_popup']= false;
            }
          }

          if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Validate Custom Attributes
              validateCustomAttributesInAddEditForm($form, $form_state,'cre_sys_obt_cls');
          }else{ // Validate custom fields
          validateCustomFieldsInAddEditForm($form, $form_state);
          } //#custom_attribute_0078975 - End Check module status
          
          
        }
        /*elseif($form_state['storage']['substage'] == 'sessiondetail') {
          // Check Session validation
          $classId = $form_state['values']['id'];
          addEditSessionDetails_validate($form, $form_state,$classId);
        }*/
        else if($form_state['storage']['substage'] == 'custom'){
          $form_state['esign_show_popup']= true;
          //customfieldsValidation($form, $form_state);
        }
        //return false;
        //$form_state['rebuild'] = TRUE;
      }else if($triggerElementByName == 'esign-survey' || $triggerElementByName == 'set-survey'){
          $form_state['esign_show_popup']= true;
          $form_state['storage']['mode'] = 'edit';
          $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
          $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
          $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];
          if (in_array($entityType, $secure_entities)) {
            $uniqueId = "survey-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive the data
          } else {
            $uniqueId = "survey-".$entityId."-$entityType"; // URL encryption to retrive the data
          }
          $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
          if(isset($entityPlugin) && $entityPlugin=='survey') {
            if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
              drupal_set_message(t('ERR106'),'error');
              $form_state['esign_show_popup']= false;
            }
          }
      }else if($triggerElementByName == 'esign-assessment' || $triggerElementByName == 'set-assessment'){
          $form_state['esign_show_popup']= true;
          $form_state['storage']['mode'] = 'edit';
          $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
          $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
          $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];

          if (in_array($entityType, $secure_entities)) {
            $uniqueId = "assessment-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive the data
          } else {
            $uniqueId = "assessment-".$entityId."-$entityType"; // URL encryption to retrive the data
          }
          $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
          $radioValueString = $form_state['input']['hidden_idlist_'.$uniqueId];
          $radioarray = explode('-',$radioValueString);
          $radioValue = $radioarray[0];
          expDebug::dPrint('$$$RadioValue = ' . print_r($radioValue, true), 4);
          $radioValueString = $form_state['input']['hidden_idlist_'.$uniqueId];
          $radioarray = explode('-',$radioValueString);
          $radioValue = $radioarray[0];
          expDebug::dPrint('$RadioValue = ' . print_r($radioValue, true), 4);
          if(isset($entityPlugin) && $entityPlugin=='assessment') {
            if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
              drupal_set_message(t('ERR106'),'error');
              $form_state['esign_show_popup']= false;
            }
             /*if(empty($form_state['input']['assessment-maxattempt-'.$radioValue]) || $form_state['input']['assessment-maxattempt-'.$radioValue] == 0 || $form_state['input']['assessment-maxattempt-'.$radioValue] == null){
                expDebug::dPrint('$assessment $form_state unique id= ' . print_r($form_state['input']['assessment-maxattempt-'.$radioValue], true), 4);
                drupal_set_message(t('Atempt field should not be empty!'),'error');
                $form_state['esign_show_popup']= false;
            }*/
           else if($form_state['input']['assessment-maxattempt-'.$radioValue] < 1){
                 $form_state['input']['assessment-maxattempt-'.$radioValue] = 1;
                  //form_set_error('assessment-maxattempt-'.$radioValue,t('Attempt field should not be less than 1'),NULL);
            }
          }


      }else if($triggerElementByName == 'esign-enrolluser' || $triggerElementByName == 'set-enrolluser' || $form_state['storage']['substage'] == 'enrolluser'){
        enrollUserValidateCheck($form_state);
      }else if($triggerElementByName == 'esign-content' || $triggerElementByName == 'set-content'){
        $form_state['esign_show_popup']= true;
        $form_state['storage']['mode'] = 'edit';
        $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
        $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];

        if (in_array($entityType, $secure_entities)) {
            $uniqueId = "content-".core_encrypt($entityId)."-".$entityType; // URL encryption to retrive the data
        } else {
            $uniqueId = "content-".$entityId."-".$entityType; // URL encryption to retrive the data
        }
        $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
        if(isset($entityPlugin) && $entityPlugin=='content') {
          if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
            drupal_set_message(t('ERR120'),'error');
            $form_state['esign_show_popup']= false;
          }else{
            $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
            $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
            $courseId = $form_state['triggering_element']['#ajax']['course_id'];
            $contentDetails = fetchContentList($courseId,$entityId,$entityType,'');
            if(count($contentDetails) > 0){
                foreach($contentDetails as $details){
                    $contentIdArr[] = $details->content_id;
                }
            }
            $excludedContentId = implode(',', $contentIdArr);
            $selectStmt = db_select('slt_course_class', 'cls');
          $selectStmt->condition('cls.id', $entityId);
          $selectStmt->addField('cls', 'delivery_type', 'delivery_type');
          expDebug::dPrintDBAPI('$selectStmt', $selectStmt);
          $deliveryType = $selectStmt->execute()->fetchField();
          expDebug::dPrint('$deliveryType = ' . print_r($deliveryType, true), 4);
            $listquery = loadCtoolDataGridForContent('', $deliveryType, '', $excludedContentId);
            if(count($listquery) > 0){
                foreach($listquery as $qdetails){
                    $content_id[] = $qdetails->id;
                    $total_lesson[] = $qdetails->total_lesson;
                    $content_name[] = $qdetails->code;
                }
            }

            $chkcontentId = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
            $err = '';
            foreach($chkcontentId as $chkCntId){
            $key = array_search($chkCntId, $content_id);
            $attempt = $form_state['input']['content-maxattempt-'.$chkCntId];
            if($attempt < 1 && $attempt != '') { /*-- #50000: System allow to set attempts left 0 --*/
                $err = t('ERR245') . "<br />";
                $form_state['esign_show_popup']= false;
            }
            if(!empty($attempt)) {
                if($total_lesson[$key] > $attempt && isset($attempt)){ /*-- #49977: System doesn't display the proper alert message --*/
                    $err .= " $content_name[$key] - Attempts should be $total_lesson[$key] or greater than $total_lesson[$key].<br>";
                    $form_state['esign_show_popup']= false;
                }
            }
          }
              if($err){
                form_set_error('',t($err));
              }
          }
         }

      }
      else if($triggerElementByName == 'esign-session' || $triggerElementByName == 'set-session'){
        $form_state['esign_show_popup']= true;
        $form_state['storage']['mode'] = 'edit';
        $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
        $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];

        if (in_array($entityType, $secure_entities)) {
            $uniqueId = "session-".core_encrypt($entityId)."-".$entityType; // URL encryption
        } else {
            $uniqueId = "session-".$entityId."-".$entityType; // URL encryption
        }
        $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
        if(isset($entityPlugin) && $entityPlugin=='session') {
          addEditSessionDetails_validate($form, $form_state,$entityId);
        }
      }
      if($triggerElementByName == 'hidden-save' || $triggerElementByName == 'set-survey' || $triggerElementByName == 'set-assessment' || $triggerElementByName == 'set-enrolluser' || $triggerElementByName == 'set-enrolluser-mandatory' || $triggerElementByName == 'set-content' || $triggerElementByName == 'set-session') {
        $form_state['hidden-save-submit']=true;
        $form_state['esign_show_popup']= false;
      }
      if($triggerElementByName == 'delete' ||  $triggerElementByName == 'hidecatalog' ||  $triggerElementByName == 'classcancel' ||  $triggerElementByName == 'classcomplete'){
        $form_state['esign_show_popup']= true;
      }
      $form_state['esign_validation_status']= false;
      if(!($form_state['hidden-save-submit'])){
        unset($_SESSION['esign_validate']);
      }

      $classDetails = getCatalogClassDetails($form_state['class_id']);
      expDebug::dPrint("Class Details :  ".print_r($classDetails,true),4);
      $getAccess = getAccessCount($form_state['class_id']);

      if(($classDetails['is_compliance'] == 1) && (empty($getAccess) || $getAccess == 0) && ($classDetails['status'] == 'lrn_cls_sts_itv') &&  $form_state['values']['show_catag'] == 1 && $form_state['input']['_triggering_element_name'] == 'saveandshow'){
        $commands = array();
        $commands[] = displaymessagewizard($form_state['class_id']);
        print ajax_render($commands);
        exit;
      }


  } catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_catalog_class_addedit_form_validate', $ex);
    expertusErrorThrow($ex);
  }
}
function validateDate($date, $format = 'Y-m-d H:i:s')
{
    $d = DateTime::createFromFormat($format, $date);
    return $d && $d->format($format) == $date;
}

function displaymessagewizard($recConut){
    try{
        return array(
                'command' => 'displaymessagewizard',
                'grpId' => core_encrypt($recConut),
        );

    }catch (Exception $ex) {
        watchdog_exception('callCourseDetailWidget', $ex);
        expertusErrorThrow($ex);
    }
}
function enrollUserValidateCheck(&$form_state){
    try{
        global $secure_entities;
        expDebug::dPrint('camin in enrollUserValidateCheck');
        $form_state['esign_show_popup']= true;
        $form_state['storage']['mode'] = 'edit';
        $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
        $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
        $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];
        $eventName = $form_state['triggering_element']['#name'];


        if (in_array($entityType, $secure_entities)) {
            $uniqueId = 'enrolluser-'.core_encrypt($entityId).'-'.$entityType; // URL encryption to retrive the data
        } else {
            $uniqueId = 'enrolluser-'.$entityId.'-'.$entityType; // URL encryption to retrive the data
        }
        $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
        $isPricedClass = getSltClassPrice($entityId);
        $pricedErr = 0;
        /*-- #36602 - Issue fix --*/
        $isCommerceEnabled =  module_exists('exp_sp_commerce');

        if($isPricedClass > 0 && $eventName == 'commonsave'){
            $selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolluser-'.$entityId.'-cre_sys_obt_cls'];
            $selectedIdsArray = explode(',', $selectedEnrolledIds);
            foreach($selectedIdsArray as $enrollId){
                $status = $form_state['input']['enrolled_status_'.$enrollId];
                if($status == t('Enrolled')){
                    $pricedErr = 1;
                }
            }
        }
        if(($isPricedClass > 0 && $isCommerceEnabled) && ($entityPlugin =='enrolluser' || $eventName=='enrolledandsave' || $pricedErr)){
            form_set_error('',t('MSG716'));
        }

        if($eventName=='commonsave' || $eventName =='incompletedandsave' || $eventName=='completedandsave'|| $eventName=='enrolledandsave'||$eventName=='cancelledandsave'||$eventName=='noshowandsave'){
            if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
                form_set_error('',t('ERR106'));
                $form_state['esign_show_popup']= false;
            }
        }
        elseif(isset($entityPlugin) && $entityPlugin=='enrolluser') {
            if(empty($form_state['input']['hidden_idlist_'.$uniqueId])){
                form_set_error('',t('ERR106'));
                $form_state['esign_show_popup']= false;
            }
        }
        if(($form_state['input']['hidden_delivery_type_'.$uniqueId] == 'lrn_cls_dty_ilt' || $form_state['input']['hidden_delivery_type_'.$uniqueId] == 'lrn_cls_dty_vcl')){
            if($entityPlugin=='enrolluser'){
                $cntSelectedUser = 0;
                if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){
                    $selectedUser = explode(',',$form_state['input']['hidden_idlist_'.$uniqueId]);
                    $cntSelectedUser = count($selectedUser);
                }
                $cls_seat_info     = _getClassInfo($entityId);
                $max_seats         = $cls_seat_info[0]->max_seats;
                $waitlist_count    = ($cls_seat_info[0]->waitlist_count) ? $cls_seat_info[0]->waitlist_count : 0;
                $totAvailableSeats = $max_seats + $waitlist_count;
                $totEnroll         = getTotalClassEnroll($entityId);
                $availableSeats    = $max_seats - $totEnroll;
                if($availableSeats < 0 ){
                    $availableWaitlistSeat = $waitlist_count + $availableSeats;
                    $availableSeats  = 0;
                }
                else{
                    $availableWaitlistSeat = $waitlist_count;
                }
                $cntAvailable = $availableSeats + $availableWaitlistSeat;
                expDebug::dPrint('$cntSelectedUser : '.$cntSelectedUser.' $cntAvailable : '.$cntAvailable, 4);
                if($cntSelectedUser > $cntAvailable){
                    form_set_error('enroll_seats', $cntAvailable . ' '. t('MSG501'));
                    if($cntAvailable !=0){
                        form_set_error('enroll_seats_avail', t('MSG502'). ' ' .$cntAvailable . ' '.t('MSG503'));
                    }
                    $form_state['esign_show_popup']= false;
                }
            }
            if($form_state['input']['hidden_registration_date_required'] == 'required') {
                $regDate = trim($form_state['input']['hidden_registration_date_'.$uniqueId]);
                if(empty($regDate)) {
                    form_set_error('form', t('LBL674').' '.t('LBL1067').' '.t('LBL042'));
                }
                else {
                    list($month, $day, $year) = split('[/-]', $regDate);
                    if(!checkdate($month, $day, $year)) {
                        form_set_error('form', t('ERR171').' '.t('LBL1067').' '.t('LBL042'));
                    }
                        expDebug::dPrint ( '$validateRegDate ' . $regDate, 5 );
                    $validateRegDate = validateRegistrationDateOfILT ( $entityId, $regDate, $form_state['input']['hidden_delivery_type_'.$uniqueId] ); // 47622: Not able to mark a past class with a past date...
                    if (! $validateRegDate) {
                        form_set_error ( 'form', t ( 'MSG726' ) );
                    }
                }
            }


        }
        expDebug::dPrint('catalog class set enroll triggered.',4);
    } catch (Exception $ex) {
        watchdog_exception('enrollUserValidateCheck', $ex);
        expertusErrorThrow($ex);
    }
}
function list_class_attributes(&$form, &$form_state,$classId,$classDetails){
  try {
        expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
      $form['class_actions_disp'] = array(
          '#type'       => 'markup',
          '#prefix'     => '<div class="catalog-course-basic-addedit-action-disp" id="catalog-class-basic-addedit-form-action-disp-'.core_encrypt($classId).'">',
          //'#suffix'     => '</div>',
          //'#attributes' => $editAttributes,
      );
      $isSelected = (($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) && empty($form_state['storage']['new_entity'])) ? 'selected' : '';
      expDebug::dPrint(' details tab $isSelected = ' . $isSelected ,4);
      $form['class_btn_details'] = array(
          '#type'       => 'submit',
          '#value'      => t('LBL272'),
          '#name'       => 'details',
          //'#prefix'     => '<div id="wizard-course-form-wrapper"><div class="admin-course-edit-button-container"><div class="admin-save-button-left-bg"></div>',
          //'#suffix'     => '<div class="admin-save-button-right-bg"></div></div>',
          '#prefix'     => '<ul><li>',
          '#suffix'     => '</li>',
          '#attributes' => array('onclick'=> 'removeQtip(-1)', 'Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
          '#ajax' => array(
            'callback' => 'addEditRepaintForm',
            'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
            'event'    => 'click'
          ),
        );
        if($classId){
          $deliveryType = $classDetails['delivery_type'];
          if($deliveryType == 'lrn_cls_dty_ilt' || $deliveryType == 'lrn_cls_dty_vcl'){
            $isSelected = ($form_state['storage']['substage'] == 'sessiondetail' || (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == true)) ? 'highlighted' : '';
            expDebug::dPrint(' session details tab $isSelected = ' . $isSelected , 4);
            $form['class_btn_sessiondetail'] = array(
              '#type'       => 'submit',
              '#value'      => t('LBL277'),
              '#name'       => 'Session Details',
              '#prefix'     => '<li>',
              '#suffix'     => '</li>',
              '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
              '#ajax' => array(
                'method' => 'replace',
                'callback' => 'addEditRepaintForm',
                'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
                'event'    => 'click'
              ),
            );
          }
          elseif($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod'){
            $isSelected = ($form_state['storage']['substage'] == 'content'  || (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == true)) ? 'highlighted' : '';
            expDebug::dPrint(' content details tab $isSelected = ' . $isSelected , 4);

            $form['class_btn_content'] = array(
              '#type'       => 'submit',
              '#value'      => t('Content'),
              '#name'       => 'Content Details',
              '#prefix'     => '<li>',
              '#suffix'     => '</li>',
              '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
              '#ajax' => array(
                'callback' => 'addEditRepaintForm',
                'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
                'event'    => 'click'
              ),
            );
          }
    /*      $isSelected = ($form_state['storage']['substage'] == 'Attachment') ? 'highlighted' : '';
          $form['class_btn_att'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL231'),
            '#name'       => 'Attachment',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );*/
          $isSelected = ($form_state['storage']['substage'] == 'survey') ? 'highlighted' : '';
          $form['class_btn_survey'] = array(
            '#type'       => 'submit',
            '#value'      => t('Survey'),
            '#name'       => 'survey',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );
          $isSelected = ($form_state['storage']['substage'] == 'assessment') ? 'highlighted' : '';
          $form['class_btn_assessment'] = array(
            '#type'       => 'submit',
            '#value'      => t('Assessment'),
            '#name'       => 'assessment',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );
          $isSelected = ($form_state['storage']['substage'] == 'enrolluser') ? 'highlighted' : '';
          $form['class_btn_enrolluser'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL275'),
            '#name'       => 'enrolluser',
            '#prefix'     => '<li>',
            '#suffix'     => '</li></ul>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );
    /*      $isSelected = ($form_state['storage']['substage'] == 'tag') ? 'highlighted' : '';
          $form['class_btn_tag'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL191'),
            '#name'       => 'tag',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );*/
          /*$isSelected = ($form_state['storage']['substage'] == 'notifications') ? 'highlighted' : '';
          $form['class_btn_notifications'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL204'),
            '#name'       => 'notifications',
            '#prefix'     => '<li>',
            '#suffix'     => '</li>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );*/
          /*$isSelected = ($form_state['storage']['substage'] == 'business_rules') ? 'highlighted' : '';
          $form['class_btn_business_rules'] = array(
            '#type'       => 'submit',
            '#value'      => 'Business Rules',
            '#name'       => 'business_rules',
            '#prefix'     => '<li>',
            '#suffix'     => '</li></ul>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );*/
          /*$isSelected = ($form_state['storage']['substage'] == 'custom') ? 'highlighted' : '';
          $form['class_btn_custom'] = array(
            '#type'       => 'submit',
            '#value'      => t('LBL276'),
            '#name'       => 'custom',
            '#prefix'     => '<li>',
            '#suffix'     => '</li></ul>',
            '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber remove-border',$isSelected), 'data-wrapperid' => 'catalog-class-basic-addedit-form-disp-'.$classId),
            '#ajax' => array(
              'callback' => 'addEditRepaintForm',
              'wrapper'  => 'catalog-class-basic-addedit-form-'.$classId,
              'event'      => 'click'
            ),
          );*/
        }
        else{
            $form['class_btn_sessiondetail_disabled'] = array(
              '#type'       => 'item',
              '#title'      => t('LBL277'),
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
            );
            $form['class_btn_survey_disabled'] = array(
            '#type'       => 'item',
                '#title'      => t('Survey'),
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
            );
            $form['class_btn_assessment_disabled'] = array(
            '#type'       => 'item',
                '#title'      => t('Assessment'),
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
            );
            $form['class_btn_enrolluser_disabled'] = array(
            '#type'       => 'item',
                '#title'      => t('LBL275'),
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
             );

            /*$form['class_btn_notifications_disabled'] = array(
            '#type'       => 'item',
                '#title'      => t('LBL204'),
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
            );*/
            /*$form['class_btn_business_rules_disabled'] = array(
            '#type'       => 'item',
                '#title'      => 'Business Rules',
              '#prefix'     => '<li><span class="deactive-links">',
              '#suffix'     => '</span></li>'
            );*/

            $form['class_actions_ul_container_disp_end'] = array(
                '#type'       => 'markup',
                '#suffix'     => '</ul>',
            );
        }

        $form['class_actions_disp_end'] = array(
          '#type'       => 'markup',
          '#suffix'     => '</div>',
        );
   } catch (Exception $ex) {
    watchdog_exception('list_class_attributes', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * exp_sp_administration_catalog_class_addedit_form_submit() - Class form submit.
 */
function exp_sp_administration_catalog_class_addedit_form_submit($form, &$form_state){

  try {

      expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
      include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_tags.inc');
      include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_attachment.inc');
       $chkesign = 0;
       $chkValidation = 1;
       $triggerName = $form_state['triggering_element']['#name'];
       if($triggerName == 'details' || $triggerName == 'Add custom' || $triggerName == 'clone_enrollment'){
         $chkValidation = 0;
       }
        expDebug::dPrint('exp_sp_administration_catalog_class_addedit_form_submit esign validate trigger ' .$form_state['triggering_element']['#name'] , 5);

       if($triggerName == 'esign-survey' || $triggerName == 'Set survey' || $triggerName == 'esign-assessment' || $triggerName == 'set-assessment' || $triggerName == 'esign-content' || $triggerName == 'set-content'){
         $chkesign = 1;
       }
        //Hidden for Remove E-sign in class Qtip screen
     if($chkesign) {
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
       }
       $form_state['storage']['submit_triggering_element'] = $form_state['triggering_element']['#name'];
       if($chkValidation){
         $form_state['esign_validation_status'] = true;
        expDebug::dPrint('cancel called inside hidden submit');
        expDebug::dPrint(' Save op');
        include_once('exp_sp_administration_catalog.inc');
        $loggedInUserId = getIdOfLoggedInUser();
        // Class price saving as Null is Rectified.
        if(is_null($form_state['values']['price']) || empty($form_state['values']['price'])){
            $form_state['values']['price'] = '0.00';
        }
        // Check whether add or edit
        if (empty($form_state['values']['id']) && $form_state['values']['id']=='') { // Add
          $classId = addNewClass($form, $form_state, $loggedInUserId);
        
          if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
            saveCustomAttributesInAddEditForm($form, $form_state, $classId, 'cre_sys_obt_cls', $loggedInUserId, 'Inserted');
          }else{
          saveCustomFieldsInAddEditForm($form, $form_state, $classId, 'class', 1);
          } //#custom_attribute_0078975 - End Check module status
          
          expertus_set_message(t('LBL272').' '.t('MSG600').'.');

          $form_state['build_info']['args'][] = $classId;
          if(!($form_state['storage']['maxseat_warning'])){
            $form_state['storage']['new_entity'] = TRUE;
            $form_state['storage']['new_entity_class'] = TRUE;
          }
          $form_state['storage']['display_mode'] = 'Edit';
          $form_state['storage']['class_saved_first_time'] = '1'; //for custom attribte
          $entityType = 'cre_sys_obt_cls';
          $emptyId = $form_state['values']['empty_id'];
          $update_attach_id = is_int($emptyId) ? $emptyId : 0;
          updateAttachmentInfoOnAddForm($classId,$update_attach_id);
          _updateAccessEntity($classId, $entityType, $emptyId);
          _updateTagsEntity($classId, "Class", $emptyId);

        }
        else { //Edit
          $classId = $form_state['values']['id'];
          unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
          unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
          if($form_state['storage']['substage'] == 'details' || empty($form_state['storage']['substage'])) {
            // RAM : Enrollment having Class => Price Changing Shows a Message Changing the price will impact the order and cancellation. Here #0036049
            $totalEnrollment = getTotalClassEnroll($classId,'Y');
            expDebug::dPrint('$totalEnrollment : '.$totalEnrollment , 4);
            if(!empty($totalEnrollment)){
                $classOldPrice = getSltClassPrice($classId);
                $classNewPrice =    $form_state['values']['price'];
                // empty to Null value conflict issue is fixed
                if(is_null($classOldPrice) || empty($classOldPrice)){
                    $classOldPrice = '0.00';
                }
                if(is_null($classNewPrice) || empty($classNewPrice)){
                    $classNewPrice = '0.00';
                }

                expDebug::dPrint('$classOldPrice : '.$classOldPrice.' $classNewPrice : '.$classNewPrice , 4);

            }// ends Price Changing Shows a Message.
            updateClassDetails($form, $form_state, $loggedInUserId);
            
            
            if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attribute Values
                 saveCustomAttributesInAddEditForm($form, $form_state, $classId, 'cre_sys_obt_cls', $loggedInUserId, 'Updated');
            }else{
            saveCustomFieldsInAddEditForm($form, $form_state, $classId, 'class', 1);
            } //#custom_attribute_0078975 - End Check module status
            
            if($classNewPrice != $classOldPrice){
                    expertus_set_message(t('MSG713'));
            }
            expertus_set_message(t('LBL272').' '.t('MSG601').'.');
            $form_state['storage']['substage'] = 'details';
          }
          $form_state['storage']['edited_entity'] = TRUE;
        }

        $form_state['display_class'] = TRUE;
        $form_state['show_add_another'] = TRUE;

        if(empty($form_state['class_id'])){
          $entityId = $form_state['values']['id'];
        }else{
          $entityId = $form_state['class_id'];
        }

        $form_state['storage']['class_list']['courseId'] = $form_state['values']['course_id'];
        $form_state['storage']['class_list']['classId']  = $classId;
        $form_state['courseId']                    = $form_state['values']['course_id'];
        $form_state['class_id']                    = $entityId;

        $entityType = 'cre_sys_obt_cls';

        $uniqueId = $entityId.'-'.$entityType;
        if(isset($form_state['values']['attr_action']) && isset($entityId)) {
          /*if($form_state['values']['attr_action'] == 'attachment') {
            attachmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
          }
          elseif($form_state['values']['attr_action'] == 'tag') {
            tagOperations($form, $form_state, $entityId, 'Class', $uniqueId);
          } */

          if($form_state['values']['attr_action'] == 'survey') {
            //surveyOperations($form, $form_state, $entityId, $entityType, $uniqueId);
            addNewSurveyDetails($form, $form_state, $entityId, $entityType);
          }
          elseif($form_state['values']['attr_action'] == 'assessment') {
            //assessmentOperations($form, $form_state, $entityId, $entityType, $uniqueId);
            addNewAssessmentDetails($form, $form_state, $entityId, $entityType);
          }
          elseif($form_state['values']['attr_action'] == 'sessiondetail') {
            //sessionDetailOperations($form, $form_state, $form_state['course_id'], $entityId, $entityType, $uniqueId);
            sessionDetailOperations($form, $form_state, $entityId, $entityType);
          }
          elseif($form_state['values']['attr_action'] == 'content') {
            //sessionWBTOperations($form, $form_state, $form_state['course_id'], $entityId, $entityType, $uniqueId);
            addNewContentDetails($form, $form_state);
          }
          elseif($form_state['values']['attr_action'] == 'enrolluser') {
            //classRegisterOperations($form, $form_state, $form_state['course_id'], $entityId, $entityType, $uniqueId);
            addNewEnrollUserDetails($form, $form_state, $entityId, $entityType);

            // Clear enrolluser-autocomplete form textfield as switching from enroll user form to roster form - fix for ticket #0022120
            clearClassRosterEnrollUserAutocomplete($form_state);
          }
          /*elseif($form_state['values']['attr_action'] == 'custom') {
            customfieldsOperations($form, $form_state, $entityId, $entityType, $uniqueId);
          }*/
          elseif($form_state['values']['attr_action'] == 'notifications') {
            updateNotificationList($entityId,$entityType,$form_state['values']);
          } elseif($form_state['values']['attr_action'] == 'business_rules') {
            updateBusinessRules($entityId, $entityType, $uniqueid, $form_state);
             // Update Business rules
          }
          //drupal_set_message('Class details updated successfully.');
        }

        //$form_state['input']    = array();
       // $form_state['values']   = array();
        $form_state['rebuild']  = TRUE;
        unset($_SESSION['esign_validate']);
      }
      if($triggerName == 'clone_enrollment'){
        addNewEnrollUserDetails($form, $form_state,$form_state['class_id'],$entityType);
        $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
        $form_state['storage']['substage'] = 'enrolluser';
      }
      expDebug::dPrint('cancel called outside hidden submit');
      //To sync solr
      //syncSolrData('Class');
  } catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_catalog_class_addedit_form_submit', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * addNewClass() - New Class Entry
 */
function addNewClass($form, &$form_state, $createUserId,$callFromAPI =0){

  $txn = db_transaction();
  $form_state['values'] = trimInputFields($form_state['values']);
  try {
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_course_class');

    $course_id         = $form_state['values']['course_id'];
    $title             = $form_state['values']['title'];
    $code              = $form_state['values']['code'];
   // $short_description = $form_state['values']['short_description'];
    $description       = empty($form_state['values']['short_description']['value']) ? null : trimInput($form_state['values']['short_description']['value']);
    $lang_code         = $form_state['values']['lang_code'];
    //$registration_end_on        = $form_state['values']['registration_end_on'];
    //$cancellation_deadline_date = $form_state['values']['cancellation_deadline_date'];
    $delivery_type     = $form_state['values']['delivery_type'];
    $currency_type     = $form_state['values']['currency_type'];
    $price             = $form_state['values']['price'] == '' ? null : $form_state['values']['price'];
   // $status            = $form_state['values']['status'];
    $author_vendor     = $form_state['values']['author_vendor'];
    if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt' || $form_state['values']['delivery_type'] == 'lrn_cls_dty_vcl'){
    $min_seats         = empty($form_state['values']['min_seats']) ? null : $form_state['values']['min_seats'];
    $max_seats         = empty($form_state['values']['max_seats']) ? null : $form_state['values']['max_seats'];
    $waitlist_count    = empty($form_state['values']['waitlist_count']) ? null : $form_state['values']['waitlist_count'];
    }
    else {  
    $min_seats         =  null;
    $max_seats         =  null;
    $waitlist_count    =  null;
    }
    if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt')
        $location_id       = empty($form_state['values']['class_location']) ? null : $form_state['values']['class_location'];
    else 
        $location_id =  null ;
    $duration          = empty($form_state['values']['scheduled_duration']) ? null : $form_state['values']['scheduled_duration'];
    //$export_compliance = empty($form_state['values']['export_compliance']) || $form_state['values']['export_compliance'] == 0 ? 'N' : $form_state['values']['export_compliance'];
    if($callFromAPI)
    {
    $additional_info   = empty($form_state['values']['additional_info']) ? null : trimInput($form_state['values']['additional_info']);
    }
    else{
    $additional_info   = empty($form_state['values']['additional_info']['value']) ? null : trimInput($form_state['values']['additional_info']['value']);
    }
    // $description       = empty($form_state['values']['short_description']['value']) ? null : trimInput($form_state['values']['short_description']['value']);
    $is_shown  = empty($form_state['values']['addn_catalog_show']) ? 0 : $form_state['values']['addn_catalog_show'];
    $is_noti  = empty($form_state['values']['addn_notification_show']) ? 0 : $form_state['values']['addn_notification_show'];
    //$inactivereasontext= empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
    //$in_active_on      = $form_state['values']['status'] == 'lrn_cls_sts_itv' ? now() : NULL;
    //$in_active_reason  = $form_state['values']['status'] == 'lrn_cls_sts_itv' ? $inactivereasontext : NULL;
    $regEndDate=null;
    if(!empty($form_state['values']['reg_end_date']) && ($form_state['values']['reg_end_date']!= t('LBL112'))){
      $oldRegEndDatet=str_replace("-","/",$form_state['values']['reg_end_date']);
      $regEndDate= date_format(date_create( $oldRegEndDatet),'Y-m-d H:i');
    }
    //Get is_compliance value from course table
    $selectIsComp = db_select('slt_course_template', 'course');
    $selectIsComp->addField('course','is_compliance');
    // Changed by Gayathri for 0070892
    $selectIsComp->addField('course','price');
    $selectIsComp->addField('course','currency_type');
    $selectIsComp->condition('course.id', $course_id);
    //expDebug::dPrintDBAPI('crs query ',$selectIsComp,5);
    $selectIsCompRes = $selectIsComp->execute()->fetchAll();
    $isCompliance = $selectIsCompRes[0]->is_compliance;
    $crsprice = $selectIsCompRes[0]->price;
    $crscurrencytype = $selectIsCompRes[0]->currency_type;
    
    //changed for #0074017
    if($form_state['values']['price'] == '' && $form_state['values']['currency_type'] == 'USD' ) { 
        $price = $crsprice;
        $currency_type = $crscurrencytype;
    } else {
        $price             = $form_state['values']['price'];                
        $currency_type     = $form_state['values']['currency_type'];        
    } 

    /*$cancelDeadlineDate=null;
    if(!empty($form_state['values']['cancel_deadline_date']) && ($form_state['values']['cancel_deadline_date']!='mm-dd-yyyy')){
      $oldCancelDeadlineDate=str_replace("-","/",$form_state['values']['cancel_deadline_date']);
      $cancelDeadlineDate= date_format(date_create( $oldCancelDeadlineDate),'Y-m-d H:i');
    }*/
    $trigger_element = $form_state['storage']['submit_triggering_element'];
    if($trigger_element == 'saveandshow'){
       $status = 'lrn_cls_sts_atv';
    }
    else if($trigger_element == 'save'){
      $status = 'lrn_cls_sts_itv';
    }
    else if($form_state['values']['status']){
      $status = $form_state['values']['status'];
    }
    // Added for #74017
    if($crsprice != $price || $crscurrencytype != $currency_type) {         
        $priceinherit = 1;
    }  
    $fields = array(
                    'course_id' => $course_id,
                    'title' => $title,
                    'code' => $code,
                    'short_description' => $description,
                    //'description' => $description,
                    'lang_code' => $lang_code,
                    'delivery_type' => $delivery_type,
                    'currency_type' => $currency_type,
                    'price' => $price,
                    'status' => $status,
                    'author_vendor' => $author_vendor,
                    'min_seats' => $min_seats,
                    'max_seats' => $max_seats,
                    'waitlist_count' => $waitlist_count,
                    'location_id' => $location_id,
                    'additional_info' => $additional_info,
                    'custom4' => $export_compliance,
                    'created_by' => $createUserId,
                    'created_on' => now(),
                    'updated_by' => $createUserId,
                    'updated_on' => now(),
                    'registration_end_on' => $regEndDate,
                    'is_compliance'  =>  $isCompliance,
                    'scheduled_duration' =>  $duration,
                    'addn_catalog_show' =>  $is_shown,
                    'addn_notification_show'  => $is_noti,
                    'price_inherit' => $priceinherit
                    // 'cancellation_deadline_date' => $cancelDeadlineDate,
                    //'in_active_on' => $in_active_on,
                    //'in_active_reason' => $in_active_reason
              );

    // Select the fields and the values to be inserted
    $insertStmt->fields($fields);

    expDebug::dPrintDBAPI(' $insertStmt SQL = ' , $insertStmt);

    // Execute the insert statement. Fetch the new class id in slt_course_class table.
    $classId = $insertStmt->execute();
    if($callFromAPI) {
        $classCreateUser = $createUserId;
    }
    else {
        $classCreateUser = getIdOfLoggedInUser();
    }
    updateGroupMappingDetails($classId, 'cre_sys_obt_cls', $classCreateUser,'emptyadd',0);
    expDebug::dPrint(' $classId = ' . print_r($classId, true) , 3);
    make_audit_trail_entries($fields, $classCreateUser , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_administration_catalog_class','addNewClass',
                                          array('course_id', 'title', 'code', 'short_description', 'description', 'lang_code','delivery_type','currency_type','price','status','author_vendor','min_seats','max_seats','waitlist_count','location_id','registration_end_on','additional_info','addn_catalog_show','addn_notification_show'));


    // Begin : Node and Taxonomy Entry
    if($callFromAPI){
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    }
    // Getting drupal userid
    $userSelect = db_query('SELECT uid uid, name uname FROM users WHERE LOWER(name) = (select lower(user_name) from slt_person where id='.$createUserId.')');
    $userResult = $userSelect->fetchAll();
    $drupalUserId = $userResult[0]->uid;

    // Getting delivery type name
    $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$delivery_type."'")->fetchField();

    // Getting vocabulary id
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();

    $vActivityId = $classId;

    $oNode = new StdClass();
    $node= new StdClass();
    $price = empty($price)? 0 : $price;
    $oNode->uid = $drupalUserId;
    $oNode->language ='en';
    $oNode->title = $title;
    $oNode->body = $description;
    $oNode->model = $delivery_type_name.'-Code-'.$code;
    $oNode->sell_price = $price;
    $oNode->cost = $price;
    $oNode->list_price = $price;
    $oNode->currency_type = $currency_type;
   
    $oNode->type = 'product';
    $oNode->created = time();
    $oNode->changed = time();
    $oNode->teaser_include = 1;
    $oNode->revision = 1;
    $node->teaser = $description;

    $oNode->format = 1;
    $oNode->log = '';
    $oNode->name = $drupalUserId;
    $oNode->date ='';
    $oNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $oNode->promote = 0;
    $oNode->sticky = 0;

    $oNode->op = "Save";
    $oNode->submit = "Save";
    $oNode->preview = "Preview";
    $oNode->form_id = "product_node_form";


    // Implemention of Expertus Meeting intergration : Start
    $vcNode = new StdClass();

    $vcNode->uid = $drupalUserId;
    $vcNode->language ='en';
    $vcNode->title = $title;
    $vcNode->body = $description;
    $vcNode->model = $delivery_type_name.'-Code-'.$code;
    $vcNode->sell_price = $price;
    $vcNode->cost = $price;
    $vcNode->list_price = $price;

    $vcNode->type = 'meeting';
    $vcNode->created = time();
    $vcNode->changed = time();
    $vcNode->teaser_include = 1;
    $vcNode->revision = 1;
    $vcNode->teaser = $description;

    $vcNode->format = 1;
    $vcNode->log = '';
    $vcNode->name = $drupalUserId;
    $vcNode->date ='';
    $vcNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $vcNode->promote = 0;
    $vcNode->sticky = 0;

    $vcNode->op = "Save";
    $vcNode->submit = "Save";
    $vcNode->preview = "Preview";
    $vcNode->form_id = "product_node_form";

    // Implemention of Expertus Meeting intergration : End

// Implemention of Forum intergration : Start

    /*$forumNode = new StdClass();

    $forumNode->uid = $drupalUserId;
    $forumNode->language ='en';
    $forumNode->title = $title;
    $forumNode->body = $description;
    $forumNode->model = $delivery_type_name.'-Code-'.$code;
    $forumNode->sell_price = $price;
    $forumNode->cost = $price;
    $forumNode->list_price = $price;

    $forumNode->type = 'forum';
    $forumNode->created = time();
    $forumNode->changed = time();
    $forumNode->teaser_include = 1;
    $forumNode->revision = 1;
    $forumNode->teaser = $description;

    $forumNode->format = 1;
    $forumNode->log = '';
    $forumNode->name = $drupalUserId;
    $forumNode->date ='';
    $forumNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $forumNode->promote = 0;
    $forumNode->sticky = 0;

    $forumNode->op = "Save";
    $forumNode->submit = "Save";
    $forumNode->preview = "Preview";
    $forumNode->form_id = "product_node_form";*/

    // Implemention of Forum intergration : End

    if($vid) {

      $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
      if(empty($delivery_type_all_tid)) {
        $terms               = new stdClass;
        $terms->name         = '--All Delivery types--';
        $terms->vid          = $vid;
        taxonomy_term_save($terms);
        $delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
      }
      $oNode->taxonomy_catalog['und'] = array();
      $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
      $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$delivery_type."' and vid = ".$vid)->fetchField();
      $dtTerms = array(
         'name' => $delivery_type_name,
         'vid' => $vid,
         'description' => $delivery_type,
         'parent' => $delivery_type_all_tid,
       );
      if(empty($delivery_type_tid)) {
         $oNode->taxonomy_catalog['und'][] = array('tid' => taxonomyVocabTerms($dtTerms));
      }
      else {
         $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
      }
      $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$delivery_type."' and vid = ".$vid)->fetchField();
      $class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$vActivityId."' and vid = ".$vid)->fetchField();
      $dtTerms1 = array(
         'name' => $title.' : '.$code,
         'vid' => $vid,
         'description' => $vActivityId,
         'parent' => $delivery_type_tid
       );
      if(!empty($class_tid)){
        $dtTerms1['tid'] =  $class_tid;
      }
      $tid = taxonomyVocabTerms($dtTerms1);
      expDebug::dPrint(' Taxonomy id: $tid = ' . print_r($tid, true) , 4);

      $oNode->taxonomy_catalog['und'] = array();
      $oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
      $oNode->teaser='';
      $oNode->validated=1;

      node_save($oNode);

      $vNodeId =  $oNode->nid;

      expDebug::dPrint(' $vNodeId - $vActivityId - $createUserId = ' . $vNodeId .' - '. $vActivityId.' - '.$createUserId , 4);

      $nodeinsertStmt = db_insert('slt_node_learning_activity');
      $nodefields = array(
                    'node_id' => $vNodeId,
                    'entity_id' => $vActivityId,
                                    'entity_type' => 'cre_sys_obt_cls',
                                    'createdby' => $createUserId,
                    'createdon' => now(),
                                'updatedby' => $createUserId,
                                'updatedon' => now()
              );
      $nodeinsertStmt->fields($nodefields);
      $nodeinsertStmt->execute();


     // Node creation for meeting integration : Start

      if($delivery_type=='lrn_cls_dty_vcl'){
        node_save($vcNode);

        $vcnid = $vcNode->nid;

        $nodeinsertStmt1 = db_insert('slt_node_learning_activity');
        $nodefields1 = array(
                      'node_id' => $vcnid,
                      'entity_id' => $vActivityId,
                                    'entity_type' => 'meeting',
                                    'createdby' => $createUserId,
                      'createdon' => now(),
                                    'updatedby' => $createUserId,
                                    'updatedon' => now()
                );
        $nodeinsertStmt1->fields($nodefields1);
        $nodeinsertStmt1->execute();
      }

      // Node creation for meeting integration : End


       // Node creation for Forum integration : Start

       // Getting forum  vocabulary id
/*      $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();

      $forumTerms               = new stdClass;
      $forumTerms->name         = $title;
      $forumTerms->vid          = $forumvid;
      $forumTerms->description  = "Class";//$description;
      $forumTerms->format       = $vActivityId;
      taxonomy_term_save($forumTerms);

      node_save($forumNode);
      $forumNodeId = $forumNode->nid;

      expDebug::dPrint('Forum Node ID = ' . $forumNodeId .' - '. $vActivityId.' - '.$createUserId , 4);

      $nodeInsertForum = db_insert('slt_node_learning_activity');
      $nodeFieldsForum = array(
                    'node_id' => $forumNodeId,
                    'entity_id' => $vActivityId,  // class id
                    'entity_type' => 'Class',
                    'createdby' => $createUserId,
                    'createdon' => now(),
                    'custom0' => 'forum'
              );
      $nodeInsertForum->fields($nodeFieldsForum);
      $nodeInsertForum->execute();*/

      // Node creation for Forum integration : End


    }
    // End: Node and Taxonomy Entry

    // Save the new user's slt_person table id in the hidden id form field.
    $form_state['values']['id'] = $classId;
    // Clears the catalog filter data.
    //cache_set('catalog_filters','');
    //cache_clear_all($cid = NULL, $bin = NULL, $wildcard = FALSE)
    cache_clear_all('catalog_filters_', 'cache', TRUE);

  } catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('addNewClass', $ex);
    expertusErrorThrow($ex);
    if($callFromAPI){
      //return array((object)array('id'=>'Failure'));
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "The ID entered is not valid";
        return $errobj;
    }
    else {
        return array((object)array('id'=>'Failure'));
    }
    //throw $ex;

  }

  // Commit the transaction
  unset($txn);
  syncSolrData('Class');
  return $classId;
}

/**
 * updateClassDetails() - Update Class Details
 */
function updateClassDetails($form, &$form_state, $createUserId, $callFromAPI = 0){
  $txn = db_transaction();

  expDebug::dPrint(' updateClassDetails() '.print_r($form_state, true) , 5);

  try {
    $form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
    $course_id = $form_state['values']['course_id'];
    $class_id = $form_state['values']['id'];

    $seatQuery = db_select('slt_course_class', 'cls');
    $seatQuery->addField('cls', 'max_seats');
    $seatQuery->condition('cls.id', $class_id);
    $seatDetails = $seatQuery->execute()->fetchAssoc();
    $existingMaxSeats = $seatDetails['max_seats'];
    if(($form_state['values']['currency_type'] != $default[0]['currency_type']) || ($form_state['values']['lang_code'] != $default[0]['lang_code'])){
            //cache_set('catalog_filters','');
            cache_clear_all('catalog_filters_', 'cache', TRUE);
        }
    $lang_code = ($form_state['values']['lang_code']);
        
    if(($callFromAPI)&& $form_state['values']['status'] == 'lrn_cls_sts_del' ){
            $lang_code= ($form_state['values']['lang_code'].'_deleted_'.$class_id) ;
        }

        $trigger_element = $form_state['storage']['submit_triggering_element'];
        /* Update tag activity Count */
        if($callFromAPI){
            $trigger_element = $form_state['values']['status'];
        }
        updateTagActivityCount($class_id,'Class',$trigger_element,$callFromAPI);    
        /* Update tag activity Count */
        
        // Get Course Price and currency_type   
        $selectcrs = db_select('slt_course_template', 'course');
        $selectcrs->addField('course','price');
        $selectcrs->addField('course','currency_type');
        $selectcrs->condition('course.id', $course_id);
        $selectCrsRes = $selectcrs->execute()->fetchAll();      
        $crsprice = $selectCrsRes[0]->price;
        $crscurrencytype = $selectCrsRes[0]->currency_type;  
        
    // Select the table to insert the record in
    $updateStmt = db_update('slt_course_class');
    $default= db_query("SELECT id,title,code,short_description,description,lang_code,delivery_type,currency_type,price,status,author_vendor,min_seats,max_seats,waitlist_count,location_id,registration_end_on,in_active_on,in_active_reason,additional_info,addn_catalog_show,addn_notification_show,price_inherit from slt_course_class where id=:cid",array(":cid"=>$class_id))->fetchAll();
    // Changed by Gayathri for 0070892


    if($form_state['values']['price'] == '' && $form_state['values']['currency_type'] == 'USD' ) {
        $price = $default[0]->price;
        $currency_type  = $default[0]->currency_type;
    }  else {
        $price             = $form_state['values']['price'];                
        $currency_type     = $form_state['values']['currency_type'];
    } 
    // Added for #74017
    if(empty($default[0]->price_inherit) && ($crsprice != $price || $crscurrencytype != $currency_type)) {      
        $priceinherit = 1;
    }   else {      
        $priceinherit = $default[0]->price_inherit;
    }   
      
    $title             = $form_state['values']['title'];
    $code              = $form_state['values']['code'];
    $short_description = $form_state['values']['short_description'];
    $description       = empty($form_state['values']['short_description']['value']) ? null : trimInput($form_state['values']['short_description']['value']);
    $lang_code         = $lang_code;
    //$registration_end_on        = $form_state['values']['registration_end_on'];
    //$cancellation_deadline_date = $form_state['values']['cancellation_deadline_date'];
    $delivery_type     = $form_state['values']['delivery_type'];
    //$status            = $form_state['values']['status'];
    $author_vendor     = $form_state['values']['author_vendor'];
  if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt' || $form_state['values']['delivery_type'] == 'lrn_cls_dty_vcl'){
    $min_seats         = empty($form_state['values']['min_seats']) ? null : $form_state['values']['min_seats'];
    $max_seats         = empty($form_state['values']['max_seats']) ? null : $form_state['values']['max_seats'];
    $waitlist_count    = empty($form_state['values']['waitlist_count']) ? null : $form_state['values']['waitlist_count'];
    }
    else {  
    $min_seats         =  null;
    $max_seats         =  null;
    $waitlist_count    =  null;
    }
    if( $delivery_type == "lrn_cls_dty_ilt") {
      $location_id       = empty($form_state['values']['class_location']) ? null : $form_state['values']['class_location'];
    }
    $duration          = empty($form_state['values']['scheduled_duration']) ? null : $form_state['values']['scheduled_duration'];
   // $export_compliance = empty($form_state['values']['export_compliance']) || $form_state['values']['export_compliance'] == 0 ? 'N' : $form_state['values']['export_compliance'];
    if($callFromAPI)
    {
    $additional_info   = empty($form_state['values']['additional_info']) ? null : trimInput($form_state['values']['additional_info']);
    }
    else{
    $additional_info       = empty($form_state['values']['additional_info']['value']) ? null : trimInput($form_state['values']['additional_info']['value']);
    }
    if($callFromAPI){
    $is_shown  = ($form_state['values']['addn_catalog_show'] == '') ? $default['0']->addn_catalog_show : $form_state['values']['addn_catalog_show'];
    $is_noti  = ($form_state['values']['addn_notification_show'] == '') ? $default['0']->addn_notification_show : $form_state['values']['addn_notification_show'];              
    }
    else
    {
    $is_shown  = empty($form_state['values']['addn_catalog_show']) ? 0 : $form_state['values']['addn_catalog_show'];
    $is_noti  = empty($form_state['values']['addn_notification_show']) ? 0 : $form_state['values']['addn_notification_show'];
    }
    //$inactivereasontext= empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
   // $in_active_on      = $form_state['values']['status'] == 'lrn_cls_sts_itv' ? now() : NULL;
    //$in_active_reason  = $form_state['values']['status'] == 'lrn_cls_sts_itv' ? $inactivereasontext : NULL;
    
    $regEndDate=null;
    if(!empty($form_state['values']['reg_end_date']) && ($form_state['values']['reg_end_date']!= t('LBL112'))){
      $oldRegEndDatet=str_replace("-","/",$form_state['values']['reg_end_date']);
      $regEndDate= date_format(date_create( $oldRegEndDatet),'Y-m-d H:i');
    }

    $cancelDeadlineDate=null;
    if(!empty($form_state['values']['cancel_deadline_date']) && ($form_state['values']['cancel_deadline_date']!= t('LBL112'))){
      $oldCancelDeadlineDate=str_replace("-","/",$form_state['values']['cancel_deadline_date']);
      $cancelDeadlineDate= date_format(date_create( $oldCancelDeadlineDate),'Y-m-d H:i');
    }

    
    if($trigger_element == 'saveandshow'){
        expDebug::dPrint(' class $default = ' . print_r($default, true) , 3);
        if(($default[0]->delivery_type == 'lrn_cls_dty_ilt' || $default[0]->delivery_type == 'lrn_cls_dty_vcl') && $default[0]->status == 'lrn_cls_sts_can'){
            $updateSessStmt  = db_update('slt_course_class_session');
            $fields      = array(
                    'updated_on'          => now(),
                    'updated_by'          => $createUserId
            );

            $updateSessStmt->fields($fields);
            $updateSessStmt->condition('class_id', $class_id,'=');
            $updateSessStmt->condition('start_date', now(),'>');
            $numUpdated = $updateSessStmt->execute();
        }
       $status = 'lrn_cls_sts_atv';
    }
    else if($trigger_element == 'save'){
      $status = 'lrn_cls_sts_itv';
    }
    else if($form_state['values']['status']){
      $status = $form_state['values']['status'];
    }

    $fields = array(
                    'title' => $title,
                    'code' => $code,
                                    'short_description' => $description,
                                    //'description' => $description,
                    'lang_code' => $lang_code,
                    'delivery_type' => $delivery_type,
                    'currency_type' => $currency_type,
                    'price' => $price,
                    'status' => $status,
                    'author_vendor' => $author_vendor,
                    'min_seats' => $min_seats,
                    'max_seats' => $max_seats,
                    'waitlist_count' => $waitlist_count,
                                    'location_id' => $location_id,
                                    'additional_info' => $additional_info,
                    'custom4' => $export_compliance,
                                    'updated_by' => $createUserId,
                    'updated_on' => now(),
                                        'addn_catalog_show' =>  $is_shown,
                                        'addn_notification_show' =>  $is_noti,
                    'registration_end_on' => $regEndDate,
                    'scheduled_duration' =>  $duration,
                    //'cancellation_deadline_date' => $cancelDeadlineDate,
                                    //'in_active_on' => $in_active_on,
                                    //'in_active_reason' => $in_active_reason
                                    'cancel_remarks' => NULL,
                    'cancel_date'   => NULL,
                     'price_inherit' => $priceinherit
                         );

   if($trigger_element == 'hidecatalog'){
     $status = 'lrn_cls_sts_itv';
     $fields = array( 'status'  => $status,
            'updated_on' => now()
     );
   }
   if($trigger_element == 'classcomplete'){
     $status = 'lrn_cls_sts_dld';
     $fields = array( 'status'  => $status,
            'updated_on' => now()
     );
   }
   if($trigger_element == 'classcancel'){
     $status = 'lrn_cls_sts_can';
     $fields = array(
               'status'  => $status,
               'cancel_remarks' => $form_state['values']['hidden_cancel_reason'],
               'cancel_date'  => now(),
               'updated_on' => now()
               );
   }
   if($trigger_element == 'saveandshow'){
        $fields['published_on'] = now();
        $fields['updated_on']   = now();
   }
    $updateStmt->condition('id', $class_id);
    // Select the fields and the values to be inserted
    $updateStmt->fields($fields);

    expDebug::dPrintDBAPI(' $insertStmt SQL = ', $updateStmt);

    // Execute the update statement
    $updateStmt->execute();
    if($callFromAPI) {
        $updateClassUID = $createUserId;
    } else {
        $updateClassUID = getIdOfLoggedInUser();
    }
    make_audit_trail_entries($fields, $updateClassUID , $default ,$class_id, 'Updated','cre_sys_obt_cls','exp_sp_administration_catalog_class','updateClassDetails',
                                          array('course_id', 'title', 'code', 'short_description', 'description', 'lang_code','delivery_type','currency_type','price','status','author_vendor','min_seats','max_seats','waitlist_count','location_id','registration_end_on','in_active_on','in_active_reason','additional_info','addn_catalog_show','addn_notification_show'));

    expDebug::dPrint(' $classId = ' . $class_id .' code '. $code, 4);

    //Update the inactive class sent the notififcation
    //lrn_cls_sts_itv
    if($status=='lrn_cls_sts_itv' || $status=='lrn_cls_sts_can'){
      $inactiveNotification = db_select('slt_enrollment', 'enroll');
      $inactiveNotification->addField('enroll','id');
      $inactiveNotification->addField('enroll','user_id');
      $inactiveNotification->condition('enroll.course_id', $course_id);
      $inactiveNotification->condition('enroll.class_id', $class_id);
      $inactiveNotification->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
      $inactiveNotification->condition('enroll.comp_status', 'lrn_crs_cmp_cmp' ,'<>');
      $inactiveNotificationDetails = $inactiveNotification->execute()->fetchAll();
      if($status=='lrn_cls_sts_itv'){
        $notificationId = 'ClassInActive';
      }
      if($status=='lrn_cls_sts_can'){
        $notificationId = 'Cancel';
        if($delivery_type=='lrn_cls_dty_ilt' || $delivery_type=='lrn_cls_dty_vcl') {
//           $classSessQuery = db_select('slt_course_class_session', 'sessions');
//           //$classSessQuery->addExpression('DISTINCT(sessions.instructor_id)', 'instructor_id');
//           $classSessQuery->addField('sessions', 'instructor_id' ,'instructor_id');
//           $classSessQuery->condition('sessions.class_id', $class_id);
//           $classSessQuery->condition('start_date', now(),'>');
//           $classSessResult = $classSessQuery->execute()->fetchAll();
//           expDebug::dPrintDBAPI('instrucotr select statement= ' , $classSessQuery);
//           $countins        = count($classSessResult);
//           for($i=0; $i<=$countins; $i++){
//               notificationInsert('CancelClassInstructor', $course_id, $class_id, $classSessResult[$i]->instructor_id, $createUserId);
//          }


            // Select table
            $selectStmt = db_select('slt_course_class_session', 'ses');
            // Join other tables.
            $selectStmt->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=ses.id');
            $selectStmt->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
            $selectStmt->leftJoin('slt_location', 'loc', 'ses.location_id = loc.id');
            $selectStmt->leftJoin('slt_country', 'country', 'loc.country = country.country_code');
            $selectStmt->leftJoin('slt_state', 'state', 'country.country_code = state.country_code &&  loc.state = state.state_code');

            // Select fields and/or add expressions
            $selectStmt->addField('ses', 'id', 'id');
            $selectStmt->addField('ses', 'title', 'title');
            $selectStmt->addField('ses', 'course_id', 'course_id');
            $selectStmt->addField('ses', 'class_id', 'class_id');
            //$selectStmt->addField('ses', 'instructor_id', 'instructor_id');
            $selectStmt->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
            $selectStmt->addField('loc', 'name', 'location_name');
            $selectStmt->addField('loc', 'addr1', 'addr1');
            $selectStmt->addField('loc', 'addr2', 'addr2');
            $selectStmt->addField('loc', 'city', 'city');
            $selectStmt->addField('state', 'state_name', 'state');
            $selectStmt->addField('country', 'country_name', 'country');
            $selectStmt->addField('loc', 'zipcode', 'zipcode');
            $selectStmt->addField('ses', 'start_time', 'start_time');
            $selectStmt->addField('ses', 'end_time', 'end_time');
            $selectStmt->addField('ses', 'start_date', 'start_date');
            $selectStmt->addField('ses', 'end_date', 'end_date');
            $selectStmt->addField('ses', 'ical_sequence', 'seq');
            $selectStmt->addField('ses', 'timezone', 'timezone');
            $selectStmt->addField('ses', 'capacity_max', 'capacity_max');
            $selectStmt->addField('ses', 'custom4', 'previous_instructor_id');
            $selectStmt->addField('splt_timezone','attr2', 'session_timezone');
            $selectStmt->addField('splt_timezone','name', 'session_timezone_name');
            // Set conditions
            $selectStmt->condition('ses.class_id', $class_id);
            // Limit to 1 record (the first record)
            //$selectStmt->range(0, 1);
            expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
            // Execute query
            $result = $selectStmt->execute()->fetchAll();
            $new_result_arr = array();
            $i = 0;
            foreach ($result as $val){
                //if(!empty($val->instructor_id) && empty($val->previous_instructor_id))
                if(!empty($val->instructor_id))
                    $new_result_arr[$val->instructor_id][] = $result[$i];
                $i++;
            }
            expDebug::dPrint(' $new_result_arr = ' . print_r($new_result_arr, true) , 4);
            //Send Notification if newly created or modified.
            if(count($new_result_arr)){
                $classInfo = array();
                $classInfo['title'] = $default[0]->title;
                $classInfo['class_id'] = $default[0]->id;
                $classInfo['code'] = $default[0]->code;
                $classInfo['lang_code'] = $default[0]->lang_code;
                $classInfo['delivery_type'] = $default[0]->delivery_type;
                expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 3);
                foreach ($new_result_arr as $instructor_id => $notificationVal){
                    expDebug::dPrint(' $instructor_id = ' . print_r($instructor_id, true) , 4);
                    if(!empty($instructor_id)){
                        $instructor_ids_arr = explode(',',$instructor_id);
                        foreach ($instructor_ids_arr as $ins_id){
                            $classInstructorInfo  = getUserInformations($ins_id);
                            expDebug::dPrint(' $classInstructorInfo = ' . print_r($classInstructorInfo, true) , 3);
                            $iCalArr = array();
                            $iCalArr['notificationVal'] = $notificationVal;
                            $iCalArr['classInfo'] = $classInfo;
                            $iCalArr['classInstructorInfo'] = $classInstructorInfo;
                            $classSessionInfo = generateNotificationInfoForInstructor($notificationVal);
                            $iCal = getCancelICal($class_id, 'Class',null,$iCalArr,'cancel');
                            expDebug::dPrint(' $icalllll = ' . print_r($iCal, true) , 3);
                            expDebug::dPrint(' $$classSessionInfo = ' . print_r($classSessionInfo, true) , 3);
                            notificationInsert('CancelClassInstructor', $course_id, $class_id, $ins_id, $createUserId,$iCal);
                        }
                    }
                }
            }
        }else {
          $notificationId = 'WBTCancel';
        }
      //}
        $util=new GlobalUtil();
        $config=$util->getConfig();
        expDebug::dPrint(' $inactiveNotificationDetails string = ' . print_r(count($inactiveNotificationDetails), true) , 4);
        expDebug::dPrint(' $inactiveNotificationDetails string = ' . print_r($config['class_cancel_normal_limit'], true) , 4);
      if(count($inactiveNotificationDetails)>$config['class_cancel_normal_limit']){
             //Cancel the class from enrollment table.
            $update_query = db_update('slt_enrollment');
            $fields = array(
                        'reg_status'  => 'lrn_crs_reg_can',
                        'updated_by' => $createUserId,
                        'updated_on' =>now(),
                        'browser_session' =>'bulk_cancel_notification_pending',
                        'comp_status'  => NULL
                    );
            $update_query->condition('class_id', $class_id);
            $update_query->condition('course_id', $course_id);
            $update_query->condition('reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
            $update_query->condition('comp_status', 'lrn_crs_cmp_cmp' ,'<>');
            $update_query->fields($fields);
            expDebug::dPrintDBAPI(' $$update_query SQL = ', $update_query);
            $update_query->execute();
            
            // after above function ality cron will insert notification
            // Update monitoring jobs table to pick the process in next run
            $update = db_update('slt_monitoring_jobs');
            $update->fields(array(
                    'custom0'=>1
            ));
            $update->condition('job_name','bulk_class_cancel_notification','=');
            $update->execute();
        }else{
      foreach($inactiveNotificationDetails as $inactiveSeperate){
          $enrollId          = $inactiveSeperate->id;
          $enrollUserId      = $inactiveSeperate->user_id;
        $cancelStatusQuery = db_update('slt_enrollment');
        $cancelStatusFields['reg_status'] = 'lrn_crs_reg_can';
        $cancelStatusFields['comp_status'] = NULL;
        $cancelStatusFields['updated_by'] = $createUserId;
        $cancelStatusFields['updated_on'] = now();
        $cancelStatusQuery->fields($cancelStatusFields);
        $cancelStatusQuery->condition('id', $enrollId);
        $cancelStatusQuery->execute();

       //insertNotification();
       if ($notificationId == 'Cancel') { // ILT or VC
        $iCal = getCancelICal($enrollId, 'Class');
       }
       insertNotification($notificationId, $enrollId, $class_id, $enrollUserId, $createUserId, $iCal);
       }
      }
    }
    }

    // Update seat details in session table
    $sessUpdateQuery = db_update('slt_course_class_session');
    $sessUpdateQueryFields['capacity_min'] = $min_seats;
    $sessUpdateQueryFields['capacity_max'] = $max_seats;
    $sessUpdateQueryFields['waitlist_count'] = $waitlist_count;
    if($default[0]->location_id != $location_id){
      $sessUpdateQueryFields['location_id'] = $location_id;
      /*-- #35708: Session duplication issue while updating location --*/
      $time_zone = getLocationTimezone($location_id);
      $sessUpdateQueryFields['timezone'] = $time_zone;
    }
    $sessUpdateQuery->fields($sessUpdateQueryFields);
    $sessUpdateQuery->condition('class_id', $class_id);
    $sessUpdateQuery->condition('course_id', $course_id);
    $sessUpdateQuery->execute();


    // Begin - Waitlist promotion or confirmation  - slp_class_session_upd.sql

    $userId = getSltpersonUserId();

    $inc = 0;
    $pos = 1;

    expDebug::dPrint('Existing max seats - '. $max_seats.' '.$existingMaxSeats , 4);

    if($max_seats > $existingMaxSeats){

      $increasedCapacity = $max_seats - $existingMaxSeats;

      // Waitlist enrollment details
      $waitlistEnrollQuery = db_select('slt_enrollment', 'enroll');
      $waitlistEnrollQuery->addField('enroll','id');
      $waitlistEnrollQuery->addField('enroll','user_id');
      $waitlistEnrollQuery->addField('enroll','reg_status');
      $waitlistEnrollQuery->addField('enroll','master_enrollment_id');
      $waitlistEnrollQuery->condition('enroll.course_id', $course_id);
      $waitlistEnrollQuery->condition('enroll.class_id', $class_id);
      $waitlistEnrollQuery->condition('enroll.reg_status', 'lrn_crs_reg_wtl');
      $waitlistEnrollQuery->orderby('enroll.waitlist_priority');
      $waitlistEnrollDetails = $waitlistEnrollQuery->execute()->fetchAll();
        expDebug::dPrintDBAPI('Fetch all enrollment in waitlist status : ',$waitlistEnrollQuery);
      foreach($waitlistEnrollDetails as $waitlistEnroll){
        $wtlEnrollId     = $waitlistEnroll->id;
        $wtlUserId       = $waitlistEnroll->user_id;
        $wtlRegStatus    = $waitlistEnroll->reg_status;
        $masterEnrollId  = $waitlistEnroll->master_enrollment_id;

        if($inc < $increasedCapacity){

          if($wtlRegStatus == 'lrn_crs_reg_ppm' || $wtlRegStatus == 'lrn_crs_reg_ppv'){

            db_query("UPDATE slt_enrollment SET waitlist_flag=NULL, waitlist_priority= NULL, updated_on=now(), updated_by=1 WHERE id= '$wtlEnrollId'");

          } else if($wtlRegStatus == 'lrn_crs_reg_wtl'){

            db_query("UPDATE slt_enrollment SET reg_status= 'lrn_crs_reg_cnf', waitlist_flag=NULL, comp_status='lrn_crs_cmp_enr', waitlist_priority=NULL, updated_on=now(), updated_by=1 WHERE id= '$wtlEnrollId'");

            expDebug::dPrint('Master enrollment update - '. $masterEnrollId , 4);

            // Master enrollment update
            if(!empty($masterEnrollId)){
              updateMasterWaitlistStatus($masterEnrollId);
            }

          }
          //CALL slp_notification_ins('WaitlistPromotion',xcourseid,xclassid,xenroluserid,'Admin');
          $iCal = getPublishICal($wtlEnrollId, 'Class');
          notificationInsert('WaitlistPromotion', $course_id, $class_id, $wtlUserId, $userId, $iCal);
          $inc += 1;

        } else {

          db_query("UPDATE slt_enrollment SET reg_status= '$wtlRegStatus', waitlist_flag='lrn_crs_reg_wtl', comp_status=null, waitlist_priority='$pos', updated_on=now(), updated_by=1 WHERE id= '$wtlEnrollId'");
          $pos += 1;
          $inc += 1;

        }
      }

    }
    // End - Waitlist promotion or confirmation

    // Begin : Node and Taxonomy Entry
    if($callFromAPI){
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    }
    // Getting drupal userid
    $userSelect = db_query('SELECT uid uid, name uname FROM users WHERE LOWER(name) = (select lower(user_name) from slt_person where id = '.$createUserId.')');
    $userResult = $userSelect->fetchAll();
    $drupalUserId = $userResult[0]->uid;
    expDebug::dPrint('Drupal uid ' . $drupalUserId , 4);
    // Getting delivery type name
    $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$delivery_type."'")->fetchField();
    expDebug::dPrintDBAPI('exp_sp_administration_catalog_class.inc delivery_type_name' ,  $delivery_type_name);
    // Getting vocabulary id

    $vActivityId= $class_id;

/*    $nodeSelect = db_query("SELECT n.nid AS nid,n.vid AS vid FROM slt_node_learning_activity entity INNER JOIN node n ON (entity.node_id=n.nid) WHERE  entity_id='".$classId."' AND entity_type='cre_sys_obt_cls'");
    $nodeResult = $nodeSelect->fetchAll();*/

    $nodeSelect = db_select('slt_node_learning_activity', 'entity');
    $nodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
    $nodeSelect->addField('n','nid','nid');
    $nodeSelect->addField('n','vid','vid');
    $nodeSelect->condition('entity.entity_id', $class_id);
    $nodeSelect->condition('entity.entity_type', 'cre_sys_obt_cls');
    $nodeResult =  $nodeSelect->execute()->fetchAll();

    $oNode = new StdClass();
    $node= new StdClass();
    $price = empty($price)? 0 : $price;
    $oNode->nid= $nodeResult[0]->nid;
    $oNode->vid= $nodeResult[0]->vid;
    $oNode->uid = $drupalUserId;
    $oNode->language ='en';
    $oNode->title = $title;
    $oNode->body = $description;
    $oNode->model = $delivery_type_name.'-Code-'.$code;
    $oNode->sell_price = $price;
    $oNode->currency_type = $currency_type;
    $oNode->cost = $price;
    $oNode->list_price = $price;

    $oNode->type = 'product';
    $oNode->created = time();
    $oNode->changed = time();
    $oNode->teaser_include = 1;
    $oNode->revision = '';
    $node->teaser = $description;

    $oNode->format = 1;
    $oNode->log = '';
    $oNode->name = $drupalUserId;
    $oNode->date ='';
    $oNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $oNode->promote = 0;
    $oNode->sticky = 0;

    $oNode->op = "Save";
    $oNode->submit = "Save";
    $oNode->preview = "Preview";
    $oNode->form_id = "product_node_form";

 // Implemention of Expertus Meeting intergration : Start

    $nodeSelect1 = db_select('slt_node_learning_activity', 'entity');
    $nodeSelect1->innerjoin('node', 'n', 'entity.node_id=n.nid');
    $nodeSelect1->addField('n','nid','nid');
    $nodeSelect1->addField('n','vid','vid');
    $nodeSelect1->condition('entity.entity_id', $class_id);
    $nodeSelect1->condition('entity.entity_type', 'meeting');
    $nodeResult1 =  $nodeSelect1->execute()->fetchAll();

    $vcNode = new StdClass();
    $price = empty($price)? 0 : $price;
    $vcNode->nid= $nodeResult1[0]->nid;
    $vcNode->vid= $nodeResult1[0]->vid;
    $vcNode->uid = $drupalUserId;
    $vcNode->language ='en';
    $vcNode->title = $title;
    $vcNode->body = $description;
    $vcNode->model = $delivery_type_name.'-Code-'.$code;
    $vcNode->sell_price = $price;
    $vcNode->cost = $price;
    $vcNode->list_price = $price;

    $vcNode->type = 'meeting';
    $vcNode->created = time();
    $vcNode->changed = time();
    $vcNode->teaser_include = 1;
    $vcNode->revision = '';
    $vcNode->teaser = $description;

    $vcNode->format = 1;
    $vcNode->log = '';
    $vcNode->name = $drupalUserId;
    $vcNode->date ='';
    $vcNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $vcNode->promote = 0;
    $vcNode->sticky = 0;

    $vcNode->op = "Save";
    $vcNode->submit = "Save";
    $vcNode->preview = "Preview";
    $vcNode->form_id = "product_node_form";

    // Implemention of Expertus Meeting intergration : End

    // Implemention of Forum intergration : Start

    /*$forumNodeSelect = db_select('slt_node_learning_activity', 'entity');
    $forumNodeSelect->innerjoin('node', 'n', 'entity.node_id=n.nid');
    $forumNodeSelect->addField('n','nid','nid');
    $forumNodeSelect->addField('n','vid','vid');
    $forumNodeSelect->condition('entity.entity_id', $vActivityId);
    $forumNodeSelect->condition('entity.entity_type', 'Class');
    $forumNodeSelect->condition('entity.custom0', 'forum');
    $forumNodeResult =  $forumNodeSelect->execute()->fetchAll();

    $forumNode = new StdClass();
    $price = empty($price)? 0 : $price;
    $forumNode->nid= $forumNodeResult[0]->nid;
    $forumNode->vid= $forumNodeResult[0]->vid;
    $forumNode->uid = $drupalUserId;
    $forumNode->language ='en';
    $forumNode->title = $title;
    $forumNode->body = $description;
    $forumNode->model = $delivery_type_name.'-Code-'.$code;
    $forumNode->sell_price = $price;
    $forumNode->cost = $price;
    $forumNode->list_price = $price;

    $forumNode->type = 'forum';
    $forumNode->created = time();
    $forumNode->changed = time();
    $forumNode->teaser_include = 1;
    $forumNode->revision = '';
    $forumNode->teaser = $description;

    $forumNode->format = 1;
    $forumNode->log = '';
    $forumNode->name = $drupalUserId;
    $forumNode->date ='';
    $forumNode->status = ($status == 'lrn_cls_sts_atv') ? 1 : 0;
    $forumNode->promote = 0;
    $forumNode->sticky = 0;

    $forumNode->op = "Save";
    $forumNode->submit = "Save";
    $forumNode->preview = "Preview";
    $forumNode->form_id = "product_node_form";*/

    // Implemention of Forum intergration : End

    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();

    expDebug::dPrintDBAPI('exp_sp_administration_catalog_class.inc outside node vid' , $vid);
    if($vid) {
       expDebug::dPrint('exp_sp_administration_catalog_class.inc inside node');

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
      $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$delivery_type."' and vid = ".$vid)->fetchField();
       expDebug::dPrint(' $delivery_type_tid  : '. $delivery_type_tid);
      $dtTerms = array(
         'name' => $delivery_type_name,
         'vid' => $vid,
         'description' => $delivery_type,
         'parent' => $delivery_type_all_tid,
       );
      if(empty($delivery_type_tid)) {
         $oNode->taxonomy_catalog['und'][] = array('tid' => taxonomyVocabTerms($dtTerms));
      }
      else {
         $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
      }
      $delivery_type_tid_cls = db_query("select tid FROM taxonomy_term_data WHERE description ='".$delivery_type."' and vid = ".$vid)->fetchField();
      $class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$vActivityId."' and vid = ".$vid)->fetchField();
      $dtTerms1 = array(
         'name' => $title.' : '.$code,
         'vid' => $vid,
         'description' => $vActivityId,
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
    // End: Node and Taxonomy Entry
    if($delivery_type=='lrn_cls_dty_vcl'){
        node_save($vcNode);
    }
    // Node update for Forum : Start
        // Getting forum  vocabulary id
        /*$forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
        $forum_tid = db_query("select tid FROM taxonomy_term_data WHERE description = 'Class' AND format ='".$vActivityId."' AND vid = ".$forumvid)->fetchField();

        $forumTerms = array(
           'name'        => $title,
           'vid'         => $forumvid,
           'description' => "Class",//$description,
           'format'      => $vActivityId
         );

        if(!empty($forum_tid)){
          $forumTerms['tid'] =  $forum_tid;
        }

        expDebug::dPrint(' Forum Update = ' .$vActivityId.'--===-'. $forum_tid. '--===-'. $forumTerms['tid']);

        $tid = taxonomyVocabTerms($forumTerms);

        node_save($forumNode);*/
      // Node update for Forum : End

    //$form_state['values']['id'] = $class_id;
  }  catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateClassDetails', $ex);
    expertusErrorThrow($ex);
    //throw $ex;
  }

  // Commit the transaction
  unset($txn);
  syncSolrData('Class');
  syncSolrData('Enrollment');
  syncSolrData('MasterEnrollment');  
  return $class_id;
}

function updateMasterWaitlistStatus($masterEnrollId){
  try {
      $mQuery = db_select('slt_master_enrollment', 'master');
      $mQuery->addField('master', 'overall_status');
      $mQuery->condition('master.overall_status', 'lrn_tpm_ovr_wtl');
      $mQuery->condition('master.id', $masterEnrollId);
      $mResult = $mQuery->execute()->fetchAll();
      expDebug::dPrintDBAPI('waitlist master enrollment ',$mQuery);
      expDebug::dPrint('count($mResult) - '. count($mResult) , 3);

      if(count($mResult) > 0){

        $enQuery = db_select('slt_enrollment', 'enroll');
        $enQuery->addField('enroll', 'id');
        $enQuery->condition('enroll.reg_status', 'lrn_crs_reg_cnf', '!=');
        $enQuery->condition('enroll.master_enrollment_id', $masterEnrollId);
        $enResult = $enQuery->execute()->fetchAll();
        expDebug::dPrintDBAPI('waitlist enrollment ',$enQuery);
        expDebug::dPrint('count($enResult) - '. count($enResult) , 3);

        if(count($enResult) == 0){

          $masterUpdate = db_update('slt_master_enrollment');
          $masterUpdateFields['overall_status'] = 'lrn_tpm_ovr_enr';
          $masterUpdateFields['updated_on'] = now();
          $masterUpdate->fields($masterUpdateFields);
          $masterUpdate->condition('id', $masterEnrollId);
          $masterUpdate->execute();
          expDebug::dPrintDBAPI('waitlist master enrollment update ',$masterUpdate);
        }

      }
  } catch (Exception $ex) {
    watchdog_exception('updateMasterWaitlistStatus', $ex);
    expertusErrorThrow($ex);
  }
}

function taxonomyVocabTerms($termsTxt) {
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
  }catch (Exception $ex) {
    watchdog_exception('taxonomyVocabTerms', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * getCatalogClassDetails() - Class details
 */
function getCatalogClassDetails($classId){
  try {
    $select = db_select('slt_course_class', 'class');
    $select->addField('class','title','title');
    $select->addField('class','code','code');
    $select->addField('class','short_description','short_description');
    $select->addField('class','description','description');
    $select->addField('class','lang_code','lang_code');
    $select->addField('class','status','status');
    $select->addField('class','course_id','course_id');
    $select->addField('class','delivery_type','delivery_type');
    $select->addExpression('(select name from slt_profile_list_items where code = class.lang_code)','language_name');
    $select->addExpression('(select name from slt_profile_list_items where code = class.status)','status_name');
    $select->addExpression('(select name from slt_profile_list_items where code = class.delivery_type)','delivery_type_name');
    $select->addExpression('(select name from slt_profile_list_items where code = class.currency_type)','currency_name');
    $select->addExpression('(select name from slt_location where id = class.location_id)','class_location_name');
    $select->addField('class','registration_end_on','registration_end_on');
    $select->addField('class','cancellation_deadline_date','cancellation_deadline_date');
    $select->addField('class','valid_from','valid_from');
    $select->addField('class','valid_to','valid_to');
    $select->addField('class','price','price');
    $select->addField('class','is_compliance','is_compliance');
    $select->addField('class','currency_type','currency_type');
    $select->addField('class','author_vendor','author_vendor');
    $select->addField('class','scheduled_duration','scheduled_duration');
    $select->addField('class','additional_info','additional_info');
    $select->addField('class','custom4','export_compliance');
    $select->addField('class','min_seats','min_seats');
    $select->addField('class','max_seats','max_seats');
    $select->addField('class','waitlist_count','waitlist_count');
    $select->addField('class','location_id','class_location');
    $select->addField('class','registration_end_on','reg_end_date');
    $select->addField('class','cancellation_deadline_date','cancel_deadline_date');
    $select->addField('class','in_active_reason','status_inactive_reason');
    $select->addField('class','addn_catalog_show','addn_catalog_show');
    $select->addField('class','addn_notification_show','addn_notification_show');
    $select->condition('class.id', $classId);
    $select->range(0,1);
    $result = $select->execute();
    expDebug::dPrintDBAPI("Catalog class details",$select);
    return $result->fetchAssoc();
  } catch (Exception $ex) {
    watchdog_exception('getCatalogClassDetails', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * classCodeDupeCheck()
 */
function classCodeDupeCheck($code, $classId,$lang_code){
  try {
    $select = db_select("slt_course_class","cls");
    $select->addField('cls','title','cls_title');
    $select->addField('cls','id','cls_id');
    $select->condition('cls.code',$code,'=');
    $select->condition('cls.lang_code',$lang_code,'=');
    if(!empty($classId)){
      $select->condition('cls.id', $classId, '!=');
    }
    $crsList = $select->execute()->fetchAll();

    return $crsList;
  } catch (Exception $ex) {
    watchdog_exception('classCodeDupeCheck', $ex);
    expertusErrorThrow($ex);
  }
}
function getAdministrationClassDetails($courseId) {
  try {
    $select = db_select("slt_course_class","cls");
    $select->innerJoin('slt_profile_list_items','prof','prof.code = cls.status ');
    $select->innerJoin('slt_profile_list_items','prof1','prof1.code = cls.delivery_type ');
    $select->innerJoin('slt_profile_list_items','prof2','prof2.code = cls.lang_code ');
    $select->leftJoin('slt_course_class_session', 'ses', 'cls.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cls.id ORDER BY start_date LIMIT 1)');
    $select->leftJoin('slt_facility', 'fac', 'ses.facility_id= fac.id');

    /*$select->addField('cls','id','cls_id');
    $select->addField('cls','title','cls_title');
    $select->addField('prof','name','cls_status');
    $select->addField('prof1','name','cls_delivery_type');
    $select->addField('cls','code','cls_code');
    $select->addField('prof2', 'name','cls_language');
    $select->addField('fac', 'name','facility_name');
    $select->orderby('cls.id','asc'); */
    $select->addExpression('count(1)','cls_count');

    $select->condition('cls.course_id',$courseId,'=');
    $select->condition('cls.status', 'lrn_cls_sts_del' ,'!=');

    $crsList = $select->execute()->fetchField();

    return $crsList;
  } catch (Exception $ex) {
    watchdog_exception('getAdministrationClassDetails', $ex);
    expertusErrorThrow($ex);
  }
 }


function addEditFormWithCatalogCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $subStage = '',$classId,$courseId,$emptyId = '') {
  // Remove the temporary storage for API's to exchange information during form creation
        global $theme_key;
      try {
      unset($form['addedit_tmp_storage']);
      $esignHiddenButt = 'esign-'.$wrapperId;
      // Add the Cancel button
      //$ajaxCallbackCancel = 'addEditCancelRepaintForm';
      $ajaxCallbackCancel = 'addEditCatalogCancelRepaintForm';
      $wrapperId          = 'add-class-list-wrapper';

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
      $cancelAttributes['data-wrapperid'] = array($wrapperId);

      // A common class for all addedit form cancel button, for styling to appear as a link.
      $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button white-btn-bg-middle';

      $classIdQtip = !empty($classId)?$classId:0;
      // Remove the error messages or success messages on clicking cancel

      $cancelAttributes['onclick'][] = 'closeQtyp("qtip_editclass_visible_dispid_'.core_encrypt($classId).'","'.$courseId.'");closeQtyp("#qtipAttachIdqtip_addclass_visible_disp_'.core_encrypt($classId).'");$("#admin-course-class-list-'.core_encrypt($courseId).'-pagination").html(""); $("#admin-course-class-list-'.core_encrypt($classId).'-pagination_toppager").html("");  remove_messages("class"); removeQtip("'.core_encrypt($classIdQtip).'"); return false;';
      $cancelAttributes['tabindex'][] = '117';

      //$addclass = ($emptyId=='')?'':'resize-save_btn';
      $classDetails;
      $ifClassValid;
      $canBeCanceled;
      $resultCnt;
      $isMultiAction;
      $classStatus;
      if(!empty($classId)){
            if($subStage == 'details' || empty($subStage)) {
                if (!empty($classId)) {
                $classDetails['class_id'] = $classId;
                // Fetch class details to be displayed in the form
                $classDetails = getCatalogClassDetails($classId);
                $ifClassValid = checkClassValidity($classId, $classDetails['delivery_type']);
                $canBeCanceled = ifClassCanBeCanceled($classId);
                }
          $resultCnt = delete_object_record($classId,'Classdelete');
                if($classDetails['status'] != 'lrn_cls_sts_atv'){
                    $isMultiAction=1;
                }
                if($classDetails['status'] == 'lrn_cls_sts_atv'){
                    $isMultiAction=1;
                }
                if($canBeCanceled == 0){
                    $isMultiAction=1;
                }
                if($resultCnt == 0 && $ifSessioncontent == 1 && ($classDetails['status'] != 'lrn_cls_sts_can')){
                    $isMultiAction=1;
                }
            }
      }
      $addclass = ($theme_key=='expertusoneV2')?'':($isMultiAction==1)?'resize-save_btn4':'resize-save_btn';
      $form[$cancelButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row enroll-edit-save-cancel-btn">' .
                       '<div class="addedit-form-cancel-container-actions '.$addclass.'"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
        '#value' => t('LBL123'),
        '#name' => 'cancel',
        '#attributes' => $cancelAttributes,
      );

      // Add the Save button


      $ajaxCallbackSave = 'addEditCatalogRepaintForm'; //addEditCatalogRepaintForm
      $wrapperId        = 'catalog-class-basic-addedit-form-'.$classId;//add-class-list-wrapper';'catalog-class-basic-addedit-form-container';//

      // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
      // each time the form is re-built.
      $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

      // For common styling of all addedit save buttons
      //$saveAttributes['class'][] = 'addedit-edit-save';

      // Using this class to applying bg color to the save button
        $saveAttributes['class'][] = 'admin-save-button-middle-bg';
        //$saveAttributes['class'][] = 'edit-catalog-course-save-publish';
        $saveAttributes['tabindex'][] = '116';
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $saveAttributes['data-wrapperid'] = array($wrapperId);

      // Using this onclick function to hide message div


      //Increase the width of the popup while clicking on save button
      $saveAttributes['onclick'][] = 'increasePopWidth();';

      // Remove the error messages or success messages on clicking submit

      $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;
      if($subStage == 'details' || empty($subStage)){
        /*$classStatus     = '';
        if (!empty($classId)) {
        $classDetails['class_id'] = $classId;
        // Fetch class details to be displayed in the form
        $classDetails = getCatalogClassDetails($classId);
        $ifClassValid = checkClassValidity($classId, $classDetails['delivery_type']);
        $canBeCanceled = ifClassCanBeCanceled($classId);
        }*/
        $isSaveParent = 0;
        if(empty($classId) || $classDetails['status'] != 'lrn_cls_sts_atv'){
          $isSaveParent = 1;
          $saveAttributes['onclick'] = array();
          $saveAttributes['class'][] = 'admin-save-button-dropdown-middle-bg-wrapper';
          $saveAttributes['onclick'][] = 'clearMessages();';
          $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
            '#suffix' => (($classId) ? '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'cls-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>' : '<div class="admin-save-button-right-bg"></div>' ).'</div>',
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

        /*if($classId){
          $resultCnt = delete_object_record($classId,'Class');
        }*/
       if($classId){

        $savePublishAttributes['class'] = array();
        if($isSaveParent){
          $form['addedit-button-group-ul-start'] = array(
            '#type' => 'markup',
            '#markup' => '<ul class="catalog-pub-add-list cls-pub-save-btn">',
          ); //  catalog-save-btn
          $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
          $savePublishSuffix = '</li>';
        }else{
          $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
          // $savePublishSuffix = ((($resultCnt==0) && ($classDetails['status'] == 'lrn_cls_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';
          $savePublishSuffix = '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'cls-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>';
          $savePublishAttributes['class'][] = 'admin-save-button-dropdown-middle-bg-wrapper';
        }
        $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
        $savePublishAttributes['data-wrapperid'] = array($wrapperId);
        if(($ifClassValid->flag != 0  && $ifClassValid->coursestatus == "lrn_crs_sts_atv") || $classDetails['status'] == 'lrn_cls_sts_atv'){
          $saveButtonId = 'catalog-class-save-show-catalog';
          $savePublishAttributes['onclick'] = array();
          $saveAttributes['onclick'][] = 'clearMessages();';
          $buttonLabel = ($classDetails['status'] == 'lrn_cls_sts_atv') ? t('LBL141') : t('LBL660');
          $savePublishAttributes['class'][] = 'admin-save-button-middle-bg'; //(($resultCnt==0) && ($courseStatus == 'lrn_cls_sts_atv')) ? 'edit-catalog-course-save-publish' : 'pub-unpub-only-save-btn';
          $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => $savePublishPrefix,
            '#suffix' => $savePublishSuffix,
            '#value' => $buttonLabel,
            '#name' => 'saveandshow',
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
        //23872: Unable to make a class to be Shown in Catalog, when no content is associated
        //41461: Irrelavant validation message appears
        elseif(($ifClassValid->flag == 0) && ($classDetails['delivery_type']=='lrn_cls_dty_wbt' || $classDetails['delivery_type'] == 'lrn_cls_dty_vod')){
            $saveButtonId = 'catalog-class-save-show-catalog-msg';
            $savePublishAttributes['onclick'] = array();
            $savePublishAttributes['onclick'][] = 'showContentNeedMessage(); return false;';
            $buttonLabel = ($classDetails['status'] == 'lrn_cls_sts_atv') ? t('LBL141') : t('LBL660');
            $savePublishAttributes['class'][] = 'admin-save-button-middle-bg';
            $form[$saveButtonId] = array(
                    '#type' => 'button',
                    '#prefix' => $savePublishPrefix,
                    '#suffix' => $savePublishSuffix,
                    '#value' => $buttonLabel,
                    '#name' => 'saveandshow',
                    '#attributes' => $savePublishAttributes,
            );
        }
       }
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
            '#markup' => '<ul class="catalog-pub-add-list cls-pub-save-btn">',
          );
        }


        // <span onclick="displayPubActionList()" class="resource-add-action-wrapper">
        expDebug::dPrint(' $hideshow = ' . print_r($classDetails['status'], true) , 4);
        expDebug::dPrint(' $classId = ' . print_r($classId, true) , 4);
        expDebug::dPrint(' $canBeCanceled = ' . print_r($canBeCanceled, true) , 4);
        if(($classId) && $classDetails['status'] == 'lrn_cls_sts_atv'){
          if($canBeCanceled == 0){
            $saveButtonId = 'catalog-course-save-hidecatalog';
            $saveUnPublishAttributes['onclick'] = array();
            $saveUnPublishAttributes['tabindex'][] = '115';
            $saveUnPublishAttributes['onclick'][] = 'clearMessages();';
            $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<li class="save-pub-unpub-sub-menu">',
            '#suffix' => '</li>',
            '#value' => t('LBL575'),
            '#name' => 'hidecatalog',
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
        $ifSessioncontent = checkSessionContent($classId);
        if(($classId) && $resultCnt == 0 && $ifSessioncontent == 1 && ($classDetails['status'] != 'lrn_cls_sts_can' && $classDetails['status'] != 'lrn_cls_sts_dld')){
            $saveButtonId = 'catalog-course-complete-catalog';
            $saveCompleteAttributes['onclick'] = array();
            $saveCompleteAttributes['onclick'][] = 'clearMessages();';
            $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<li class="save-pub-unpub-sub-menu">',
            '#suffix' => '</li>',
            '#value' => t('LBL639'),
            '#name' => 'classcomplete',
            '#attributes' => $saveCompleteAttributes,
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
        if(($classId) && ($classDetails['status'] == 'lrn_cls_sts_atv')){

           /*$entityType = 'cre_sys_obt_cls';
           $qtipOptClassObj     = "{'url':'administration/class-cancel-window/".$classId."/".$entityType."','popupDispId':'qtip_classcancel_visible_disp_".$classId."','catalogVisibleId':'qtipIdqtip_class_cancel_disp_".$classId."','wBubble':270,'hBubble':'auto','tipPosition':'bottomRight'}";

           $form['catalog-class-cancel-save-li'] = array(
            '#type' => 'markup',
            //'#markup' => "<div id='qtip_visible_disp_".$qtipIdInit."' onmouseover =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptClassObj.");\">",
            '#prefix' => "<div id='qtip_classcancel_visible_disp_".$classId."'><li class='save-pub-unpub-sub-menu completedandsave' onmousedown =\"$('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptClassObj.");return false;\">",
            '#suffix' => '</li></div>',
            '#markup' => '<input class="form-submit ajax-processed" value="'.t('Cancel').'"/>',
           );*/
            $activeEnrollment=getTotalEnrollForPricePurchasedClass($classId);
            $saveCancelAttributes['onclick'] = array();
            //35264: Issue with Cancellation at Class Level
            if($activeEnrollment >0){
                $saveButtonId = 'catalog-course-cancel-button-catalog';
                $saveCancelAttributes['data-wrapperid'] = array($wrapperId);
                $saveCancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
                $saveCancelAttributes['onclick'] = array();
                $saveCancelAttributes['onclick'][] = 'showUserEnrolledMessage(); return false;';
                $buttonLabel = t('LBL109');
                $form[$saveButtonId] = array(
                        '#type' => 'button',
                        '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                        '#suffix' => '</li>',
                        '#value' => $buttonLabel,
                        '#name' => 'classcancel',
                        '#attributes' => $saveCancelAttributes,
                );
            }else{
           $saveButtonId = 'catalog-course-cancel-catalog';
           $saveCancelAttributes['data-wrapperid'] = array($wrapperId);
           $saveCancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
            $saveCancelAttributes['onclick'] = array();
            $saveCancelAttributes['onclick'][] = 'clearMessages();';
            $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<li class="save-pub-unpub-sub-menu">',
            '#suffix' => '</li>',
            '#value' => t('LBL109'),
            '#name' => 'classcancel',
            '#attributes' => $saveCancelAttributes,
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
      if(!$isSaveParent){
          /*$form['addedit-button-group-ul-start-extend'] = array(
            '#type' => 'markup',
            '#markup' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>',
          );*/
        }

        expDebug::dPrint('Class id '.$classId.'result_count='.$resultCnt , 4);
        $select = db_select('slt_group_mapping','map');
        $select->addExpression('count(1)','mapCount');
        $select->condition('map.entity_id',$classId,'=');
        $resultCount = $select->execute()->fetchField();
        if($resultCount > 0){
            $entityId = $classId;
            $entityType = 'cre_sys_obt_cls';
        }else{
            $entityId = $classDetails['course_id'];
            $entityType = 'cre_sys_obt_crs';
        }
        $deleteCheck = adminVisibilityForAddEditDelete($entityId,$entityType,'cre_sys_obt_crs','slt_course_class');
        expDebug::dPrint("delete check ".print_r($deleteCheck,true),4);
        if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
            expDebug::dPrint("delete check123 ".print_r($deleteCheck,true),4);
            if(($classId)){
             // if($resultCnt == 0){
              if($canBeCanceled == 0){
                $displayTitle = titleController('ADDEIDT-CATALOG-CLASS-CANCEL-SAVE-CLOSE', $classDetails['title'],34);
               $saveButtonId = 'catalog-class-delete-'.$classId;

               $deleteAttributes['onclick'] = array();
               $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('Class')).' '.escape_string($classDetails['title']) . '","'.core_encrypt($classId).'","Class");';
               $deleteAttributes['class'][] = 'admin-delete-button-bg';
               $form[$saveButtonId] = array(
                    '#type' => 'button',
                    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                    '#suffix' => '</li>',
                    '#value' => t('LBL286'),
                    '#name' => 'delete',
                    '#attributes' => $deleteAttributes,
                    '#ajax' => array(
                      'wrapper' => $wrapperId,
                      'callback' => '',
                      'event'=>'click',
                      'progress' => array(
                        'type' => 'throbber',
                        'message' => ''
                        ),
                      )
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
          $saveAttributes['onclick'][] = 'clearMessages();';
          $saveAttributes['class'][] = 'admin-save-button-middle-bg';

          //$saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click();';
          $form[$saveButtonId] = array(
            '#type' => 'submit',
            '#prefix' => '<div class="admin-save-button-container admin-addedit-class-notification"><div class="admin-save-button-left-bg"></div>',
            '#suffix' =>
                       '<div class="admin-save-button-right-bg"></div></div>' .
                       '<div class="clearBoth"></div></div>' .
                     '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
            '#value' => t('LBL141'),
            '#name' => 'save',
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
      }

      unset($saveAttributes['tabindex']);
      $saveAttributes['onclick'] = array();
    /*  $saveAttributes['onclick'][] = 'hideMessageInfo();';
      $saveAttributes['onclick'][] = 'remove_messages("class");';*/
      $form[$esignHiddenButt] = array(
            '#type' => 'submit',
            '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
            '#suffix' => '</span>',
            '#value' => t('hidden-save'),
            '#name' => 'hidden-save',
            '#id' =>$esignHiddenButt,
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
      $entityId              = !empty($classId)?$classId:0;
      $entityId             = core_encrypt($entityId);
      expDebug::dPrint('ramtest :'.$entityId.'$emptyId : '.$emptyId,4);
      if(!empty($emptyId)){
        if(empty($entityId)){ // issue Identifed When a class is saved and still $entityId is in 0 Unable to add a Tag
            $entityId = 0;
        }
      } else {
        $emptyId = 0;
      }

      if($subStage == 'details' || empty($subStage)){
        $tagsentityId      = $entityId;//.',"'.$entityType
        $tagsentityType    = core_encrypt('Class');
        $qtipTagsIdInit    = $tagsentityId.'_'.$tagsentityType;
        $tagsvisibPopupId  = 'qtip_visible_disp_tags_'.$qtipTagsIdInit;

       // $entityId              = $classId;//.',"'.$entityType
        $entityType          = 'cre_sys_obt_cls';
        $qtipIdInit          = $entityId.'_'.$entityType;


        if($theme_key == 'expertusoneV2') {
            $qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'Class',
                                                            'url':'administration/catalog-tags/ajax/".$tagsentityId."/".$tagsentityType."/".$emptyId."',
                                                            'popupDispId':'".$tagsvisibPopupId."','catalogVisibleId':'renderTagsId".$qtipTagsIdInit."',
                                                            'wid':400,'heg':'120','postype':'middle','poslwid':'','qdis':'ctool',
                                                            'linkid':'visible-ctools-tags-".$entityId."','scrollid':'tag-scroll-id'}";

            $qtipOptAccessObj      = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_cls',
                                                                    'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."/".$courseId."','popupDispId':'qtip_visible_disp_".$qtipIdInit."',
                                                                    'catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
                                                                    'wid':685,'heg':'270','postype':'middle','poslwid':'','qdis':'ctool',
                                                                    'linkid':'visible-class-".$entityId."'}";
        } else {
            $qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'Class',
                                                            'url':'administration/catalog-tags/ajax/".$tagsentityId."/".$tagsentityType."/".$emptyId."',
                                                            'popupDispId':'".$tagsvisibPopupId."','catalogVisibleId':'renderTagsId".$qtipTagsIdInit."',
                                                            'wid':400,'heg':'140','postype':'middle','poslwid':'150','qdis':'ctool',
                                                                    'linkid':'visible-ctools-tags-".$entityId."','scrollid':'tag-scroll-id'}";

            $qtipOptAccessObj      = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_cls',
                                                                    'url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."/".$courseId."',
                                                                    'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
                                                                    'wid':685,'heg':'270','postype':'middle','poslwid':'150','qdis':'ctool',
                                                                    'linkid':'visible-class-".$entityId."'}";
        }

        $dummy_entity_id       = empty($entityId) ? ereg_replace(" ", "",$emptyId) : $entityId;
        $qtipOptAttachmentObj  = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_cls',
                                                                'url':'administration/catalog-attachment/".$dummy_entity_id."/".$entityType."',
                                                                'popupDispId':'qtip_attachment_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachIdqtip_visible_disp_".$qtipIdInit."',
                                                                'wid':550,'heg':'160','postype':'bottomleft','poslwid':'','qdis':'ctool',
                                                                  'linkid':'visible-ctools-class-attachment-".$entityId."'}";

        $allDetails    = getCatalogClassDetails($classId);
        $deliveryType = $allDetails['delivery_type'];
        $businessRuleRes = _getBusinessRulesList($entityId,$entityType,$deliveryType);
        expDebug::dPrint('$businessRuleRes'.print_r($businessRuleRes,true),4);
        if(!empty($businessRuleRes)){
         $qtipOptBusinessObj      = "{'entityId':'".$entityId."','entityType':'cre_sys_obt_cls',
                                                                     'url':'administration/catalogbusinessrule/".$entityId."/".$entityType."/".$deliveryType."/".$emptyId."',
                                                                     'popupDispId':'qtip_buisness_disp_".$qtipIdInit."','catalogVisibleId':'qtipBusinessqtip_visible_disp_".$qtipIdInit."',
                                                                     'wid':345,'heg':'100','postype':'bottomleft','poslwid':'','qdis':'ctool',
                                                                     'linkid':'visible-ctools-businessrule-".$entityId."'}";
       //if($deliveryType == 'lrn_cls_dty_vod' || $deliveryType == 'lrn_cls_dty_wbt'){
         $business_rule = "<div class=\"tab-seperator\"></div><div class='businessrule-tab-icon'></div><div id='qtip_buisness_disp_".$qtipIdInit."' class=\"tab-title\">".
                                                "<a id='visible-ctools-businessrule-".$entityId."' onclick = \"callVisibility(".$qtipOptBusinessObj.");\" class=\"tab-title\">".t('LBL719')."</a>".
                                                "<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
                                            "</div>";
         $onload_busines_rule ='$(\'#root-admin\').data(\'narrowsearch\').getBubblePopup('.$qtipOptBusinessObj.');';
          //}else{
          //  $business_rule="";
          //}
        }

        $form['addedit-left-col-extend-func'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="crs-tab-titles-container"><div class="attachment-tab-icon"></div>'.
                        "<div id='qtip_attachment_disp_".$qtipIdInit."'>".
                            "<a id='visible-ctools-class-attachment-".$entityId."' onclick = \"callVisibility(".$qtipOptAttachmentObj.");\" class=\"tab-title\">".t('LBL231').
                            "</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
                        "</div><div class=\"tab-seperator\"></div><div class=\"tags-tab-icon\"></div>".
                        "<div id='".$tagsvisibPopupId."'>".
                                    "<a id='visible-ctools-tags-".$entityId."' onclick = \"callVisibility(".$qtipTagsOptObj.");\" class=\"tab-title\">".t('LBL191').
                                    "</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
                        "</div><div class=\"tab-seperator\"></div>".
                        '<div class="access-tab-icon"></div>'.
                        "<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-class-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".$business_rule.
                      '</div>',
        );

       /*$ajaxEvent = 'load';
       $loadAttributes['onload'][] = "$(document).ready(function(){
                                        $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAttachmentObj.");
                                        $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipTagsOptObj.");
                                        $('#root-admin').data('narrowsearch').getBubblePopup(".$qtipOptAccessObj.");
                                     });";*/

       $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif';

       /*$form['edit-class-list-button-qtipload-'] = array(
          '#type' => 'image_button', '#src' => $pngPath,
          '#name' => 'edit-class-',
          '#prefix' => '<div style="display:none;">',
          '#suffix' => '</div>',
          '#attributes' => $loadAttributes,
          '#ajax' => array(
             'wrapper' => $wrapperId,
             'event' => $ajaxEvent,
           ),
        );*/
      }


      $form['addedit-left-col-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="clearBoth"></div>',
        '#prefix' => '<div style="display:none;"><img id= "edit-course-class-img" src="'.$pngPath.'" ></div>',
      );
      //callVisibility('.$qtipOptAccessObj.');
      $form['load-scripts'.$uniqueId] = array(
        '#prefix' => '<script language="Javascript" type="text/javascript">selecteDropdown()</script>',
      );
  } catch (Exception $ex) {
    watchdog_exception('addEditFormWithCatalogCancelAndSaveClose', $ex);
    expertusErrorThrow($ex);
  }

}

function addEditCatalogRepaintForm($form, &$form_state) {
  try {
      $commands = array();
      $triggerElement = $form_state['triggering_element']['#name'];
      $form_validate  = $form_state["storage"]["form_validate"];
      $newEntity      = !empty($form_state['storage']['new_entity']); // will not be present when trigger element is cancel
      $editedEntity   = !empty($form_state['storage']['edited_entity']);

      if($triggerElement=='hidden-save' && $newEntity){
        $courseId             = $form_state['course_id'];
        $classId              = $form_state['storage']['class_list']['classId'];
        $min_form_state       = array(
          'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
          're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
          'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
          'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
        );
        $form_state                                = $min_form_state;
        $form_state['courseId']                    = $courseId;
        $form_state['class_id']                    = $classId;
        $form_state['storage']['new']['course_id'] = $courseId;
        $form_state['storage']['new']['class_id']  = $classId;
        $form_state['storage']['new_entity'] = TRUE; // we need to pass this to exp_sp_administration_catalog_class_title_form() so that
                                                     // it shows the new session/content tab when it re-opens the class edit form for the newly saved class
        $builtForm            = drupal_build_form('exp_sp_administration_catalog_class_addedit_form', $form_state);
        $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
      }else if($triggerElement=='hidecatalog'){
        $form_state['input']['show_catag'] = 1;
        $builtForm            = drupal_build_form('exp_sp_administration_catalog_class_addedit_form', $form_state);
        $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
      }
      else {
            if($form_state['storage']['substage']=='details')
            {
            $path = $base_url.'/sites/all/themes/core/expertusoneV2/expertusone-internals/images';
            $add_shw_cat = t("LBL576");
            $add_shw_cat_label = t("MSG818");
            $add_shw_not = t("LBL3069");
            $add_shw_not_label = t("MSG819");

            if($form_state['input']['addn_catalog_show'] == 1)
            {
            $form['addn_catalog_show']['#prefix'] = "<div class=addedit-twocol-firstcol><div class=catalog-block><label class=show-label-catalog> $add_shw_cat :</label><img src=$path/help.png class='vtip info-enr-upload test-help-iconcatalog' 
            title='$add_shw_cat_label' onload=vtip() /><div class=checkbox-selected >";
            $form['addn_catalog_show']['#suffix'] =  "</div></div>";
            }
            else
            {
            $form['addn_catalog_show']['#prefix'] = "<div class=addedit-twocol-firstcol><div class=catalog-block><label class=show-label-catalog> $add_shw_cat :</label><img src=$path/help.png class='vtip info-enr-upload test-help-iconcatalog' 
            title='$add_shw_cat_label' onload=vtip() /><div class=checkbox-unselected >";
            $form['addn_catalog_show']['#suffix'] =  "</div></div>";
            }
            if($form_state['input']['addn_notification_show'] == 1)
            {
            $form['addn_notification_show']['#prefix'] = "<div class=notifi-block><label class=show-label-notifications> $add_shw_not :</label><img src=$path/help.png class='vtip info-enr-upload test-help-iconcatalog' 
            title='$add_shw_not_label' onload=vtip() /><div class=checkbox-selected >";
            $form['addn_notification_show']['#suffix'] =  "</div></div></div></div>";
            }
            else
            {
            $form['addn_notification_show']['#prefix'] = "
            <div class=notifi-block>
            <label class=show-label-notifications> $add_shw_not :</label><img src=$path/help.png class='vtip info-enr-upload test-help-iconcatalog' 
            title='$add_shw_not_label' onload=vtip() /><div class=checkbox-unselected >";
            $form['addn_notification_show']['#suffix'] =  "</div></div></div></div>";
            }
            }
            $commands[]           = ajax_command_replace(NULL, drupal_render($form));
            }
      $statusMsgHTML = theme('status_messages');
      expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
      if (!empty($statusMsgHTML)) {
        // Show the messages
        $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
      }

      $commands[]           = exp_ctools_modal_adjust();

      if ($newEntity || $editedEntity) {
        $commands[]   = ajax_command_refresh_narrow_search_results('onclose', $newEntity);
      }
      return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
    watchdog_exception('addEditCatalogRepaintForm', $ex);
    expertusErrorThrow($ex);
  }
}

/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 and #0071329  
 * getClassLocation() - Fetch location list.
 */
function getClassLocation(){
  try {
        $location =  $_GET['z']; 
        expDebug::dPrint('$location ='.$location,5);

        $select = db_select('slt_location', 'sl');
        $select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=sl.country');
        $select->leftJoin('slt_state','loc_stats','loc_stats.state_code = sl.state and loc_stats.country_code=sl.country');

        $select->addExpression('DISTINCT sl.name', 'name');
        $select->addExpression('sl.id', 'loc_id');
        $select->addExpression('sl.capacity','loc_capacity'); 
        $select->addExpression('sl.addr1', 'addr1');
        $select->addExpression('sl.addr2', 'addr2');
        $select->addExpression('sl.city', 'city');
        $select->addExpression('sl.zipcode', 'zipcode');
        $select->addExpression('loc_stats.state_name', 'state_name');
        $select->addExpression('loc_ctry.country_name', 'country_name'); 


        $loc_or_condition = db_or()->condition('sl.name', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('sl.addr1', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('sl.addr2', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('sl.city', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('loc_stats.state_name', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('loc_ctry.country_name', '%' . db_like($location) . '%', 'LIKE')
                                           ->condition('sl.zipcode', '%' . db_like($location) . '%', 'LIKE')
                                           ;
         
        $select->condition($loc_or_condition);   
         
        $select->condition('sl.status', 'lrn_res_loc_atv');
         
        //filterByUserAccessPrivileges($select, 'location','',1,'cre_sys_obt_loc','autocomplete','location'); 
        
        filterByUserAccessPrivileges($select, 'sl','',1,'cre_sys_obt_loc','','sl');
        
        expDebug::dPrintDBAPI("Class Autocomplete --->> " , $select); 
        
        $qry_loc_autocomplete = $select->execute()->fetchAll();
        
        foreach ($qry_loc_autocomplete as $rec_loc_autocomplete) {
            //  print htmlentities($rec_loc_autocomplete->name,ENT_QUOTES)."\n";    //change by ayyappan for 42444: In catalog location filter if the location contains special character
            $loc_str=trim($rec_loc_autocomplete->name);
            
            if(trim($rec_loc_autocomplete->addr1)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->addr1);
            }
            if(trim($rec_loc_autocomplete->addr2)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->addr2);
            }
            if(trim($rec_loc_autocomplete->city)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->city);
            }
            
            if(trim($rec_loc_autocomplete->zipcode)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->zipcode);
            }
            
            if(trim($rec_loc_autocomplete->state_name)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->state_name);
            }
            
            if(trim($rec_loc_autocomplete->country_name)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->country_name);
            } 
              
            print $rec_loc_autocomplete->loc_id."#".$rec_loc_autocomplete->loc_capacity."".EXP_AC_SEPARATOR."".$loc_str . "\n";
        } 
     
  } catch (Exception $ex) {
    watchdog_exception('getClassLocation', $ex);
    expertusErrorThrow($ex);
  }
}



/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 and #0071329
getClassLocationID()  - To get the location id for corresponding location name */

function getClassLocationID($tmp_location){
  try {
        
        expDebug::dPrint('$location_name ='.$tmp_location,5);
        
        $loc_split_arr=array(); 
        
        if(strpos($tmp_location,',')!== false){  
            $loc_split_arr=explode(',',$tmp_location);          
        }  
        expDebug::dPrint('$loc_split_arr='.print_r($loc_split_arr,true),5);
        expDebug::dPrint('$loc_split_arr Count ='.count($loc_split_arr),5);  
                    
        $loc_or_condition=''; 
                    
        if(isset($loc_split_arr) && !empty($loc_split_arr) && count($loc_split_arr)>0){ //Full Location Details    
        
                 $remove_comma_from_loc_str=addslashes(implode('',$loc_split_arr));
                                
                 $tmp_full_loc_cond="replace(concat_ws(' ',sl.name,sl.addr1,sl.addr2,sl.city,sl.zipcode,loc_stats.state_name,loc_ctry.country_name),',','') like \"%$remove_comma_from_loc_str%\"";
                 
                 $loc_or_condition = db_and()->where($tmp_full_loc_cond);   
                                           
        }else{ //Only Location Name
                $loc_or_condition = db_and()->condition('sl.name', trim($tmp_location), '=');
        }
        
        $select = db_select('slt_location', 'sl');
        $select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=sl.country');
        $select->leftJoin('slt_state','loc_stats','loc_stats.state_code = sl.state and loc_stats.country_code=sl.country'); 
        $select->addExpression('sl.id', 'loc_id'); 
        $select->condition($loc_or_condition);   
         
        $select->condition('sl.status', 'lrn_res_loc_atv');
        $select->range(0,1);
         
        //filterByUserAccessPrivileges($select, 'location','',1,'cre_sys_obt_loc','autocomplete','location'); 
        
        expDebug::dPrintDBAPI("Class Location ID Select --->> " , $select); 
        
        $locaion_id = $select->execute()->fetchField();
         
        expDebug::dPrint('$locaion_id ='.$locaion_id,5);
         
        return $locaion_id;
     
 
  } catch (Exception $ex) {
    watchdog_exception('getClassLocationID', $ex);
    expertusErrorThrow($ex);
  }
}


 /* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 and #0071329
 * getClassLocationAdd() - Fetch location list.
 */
function getClassLocationAdd($id=''){
  try {
        $select = db_select('slt_location', 'sl');
        $select->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=sl.country');
        $select->leftJoin('slt_state','loc_stats','loc_stats.state_code = sl.state and loc_stats.country_code=sl.country');
        
    
        $select->addExpression('DISTINCT sl.name', 'name');
        $select->addExpression('sl.id', 'loc_id');
        $select->addExpression('sl.addr1', 'addr1');
        $select->addExpression('sl.addr2', 'addr2');
        $select->addExpression('sl.city', 'city');
        $select->addExpression('sl.zipcode', 'zipcode');
        $select->addExpression('loc_stats.state_name', 'state_name');
        $select->addExpression('loc_ctry.country_name', 'country_name'); 
     
        $select->condition('sl.id', $id, '=');
        
        $select->condition('sl.status', 'lrn_res_loc_atv', '=');
        
        //filterByUserAccessPrivileges($select, 'sl','',1,'cre_sys_obt_loc','','sl');
        
        expDebug::dPrintDBAPI('getlocation Select = ', $select);
        
        $qry_loc_autocomplete = $select->execute()->fetchAll();
        
        expDebug::dPrint('$qry_loc_autocomplete='.print_r($qry_loc_autocomplete,true),5);
        
        $loc_str="";
        
        foreach ($qry_loc_autocomplete as $rec_loc_autocomplete) {
            //  print htmlentities($rec_loc_autocomplete->name,ENT_QUOTES)."\n";    //change by ayyappan for 42444: In catalog location filter if the location contains special character
            $loc_str=trim($rec_loc_autocomplete->name);
            
            if(trim($rec_loc_autocomplete->addr1)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->addr1);
            }
            if(trim($rec_loc_autocomplete->addr2)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->addr2);
            }
            if(trim($rec_loc_autocomplete->city)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->city);
            }
            
            if(trim($rec_loc_autocomplete->zipcode)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->zipcode);
            }
            
            if(trim($rec_loc_autocomplete->state_name)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->state_name);
            }

            if(trim($rec_loc_autocomplete->country_name)!=''){
                $loc_str.=", ".trim($rec_loc_autocomplete->country_name);
            }  
        }
        
        expDebug::dPrint('$loc_str='.print_r($loc_str,true),5);
        
        return $loc_str;
        
      
      
  } catch (Exception $ex) {
    watchdog_exception('getClassLocationAdd', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getTotalClassEnroll - Use this function to get a total enrollment
 */
function getTotalClassEnroll($classId,$nocompletestatus = 'N'){
  try {
    $select = db_select('slt_enrollment', 'enroll');
    //Modified by Priya C for # 0032785
    //$select->addExpression('COUNT(1)','total_enroll');
    $select->addExpression('COUNT(Distinct(enroll.user_id))','total_enroll');
    $select->condition('enroll.class_id', $classId, '=');
    $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
    if($nocompletestatus == 'Y') { // RAM fixed for this ticket enrollment,inprogress and waitlisted only #0036049
            $enrollstatus = db_or();
            $enrollstatus->condition('enroll.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'), 'IN');
            $enrollstatus->condition('enroll.comp_status', '','IS NULL');
            $select->condition($enrollstatus);
    }
    expDebug::dPrintDBAPI('$select', $select);
    $total_enroll = $select->execute()->fetchField();
    return $total_enroll;
  } catch (Exception $ex) {
    watchdog_exception('getTotalClassEnroll', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getTotalEnrollForPricePurchasedClass - Use this function to get a total enrollment
*/
function getTotalEnrollForPricePurchasedClass($classId){
    try {
        $select = db_select('slt_enrollment', 'enroll');
        $select->leftJoin('slt_order','ord','ord.id=enroll.order_id');
        $select->addExpression('COUNT(Distinct(enroll.user_id))','total_enroll');
        $select->condition('enroll.class_id', $classId, '=');
        $select->condition('enroll.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
        $enrollstatus = db_or();
        $enrollstatus->condition('enroll.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'), 'IN');
        $enrollstatus->condition('enroll.comp_status', '','IS NULL');
        $select->condition($enrollstatus);
        $select->condition('ord.order_total_amt', '0', '>');
        expDebug::dPrintDBAPI('$select', $select);
        $total_enroll = $select->execute()->fetchField();
        return $total_enroll;
    } catch (Exception $ex) {
        watchdog_exception('getTotalEnrollForPricePurchasedClass', $ex);
        expertusErrorThrow($ex);
    }
}
/*
 * getTotalClassEnroll - Use this function to get a total enrollment
 */
function getSltClassPrice($classId){
    try{
    $select = db_select('slt_course_class', 'cls');
    $select->addField('cls','price');
    $select->condition('cls.id', $classId);
    $price = $select->execute()->fetchField();
    return $price;
    } catch (Exception $ex) {
        watchdog_exception('getSltClassPrice', $ex);
        expertusErrorThrow($ex);
    }
}

function addNewClassByRestAPI( &$form_state,$createuserid="")
{
  try {
    $form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
    if(!checkValidDeliveryType($form_state['values']['delivery_type'])) { // #45472 - Validate the delivery type
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid/Inactive Delivery Type.";
        return $errobj;
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
           
    if($form_state['values']['status'] != 'lrn_cls_sts_itv'){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Invalid class status.";
        return $errobj;
    }
  if($form_state['values']['currency_type']){
        $curr_valid = currency_isValidOrNot($form_state['values']['currency_type']);
        if(empty($curr_valid)){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Currency Type is not Valid";
            return $errobj;
        }
    }
    if(!empty($form_state['values']['reg_end_date']) && !preg_match( '/^(19|20)\d\d[\-\/.](0[1-9]|1[012])[\-\/.](0[1-9]|[12][0-9]|3[01])$/', $form_state['values']['reg_end_date'])){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Incorrect date format. It should be in YYYY-MM-DD";
        return $errobj;
    }
    /**
     * change by: ayyappans
     * 34523: Issue In API's
     * validation block added for addNewClassByRestAPI() method
     * max_seats is mandatory for ILT and VC delivery types
     * location is mandatory for ILT
     * */

    if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt' || $form_state['values']['delivery_type'] == 'lrn_cls_dty_vcl'){
        if(empty($form_state['values']['max_seats'])){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->field = 'max_seats';
            $errobj->errormsg = "max_seats is required for ILT and VC class delivery types.";
            return $errobj;
        }
        if(!empty($form_state['values']['max_seats']) && !empty($form_state['values']['min_seats'])){
            if($form_state['values']['min_seats'] >= $form_state['values']['max_seats']){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Maximum capacity must be greater than minimum capacity.";
                return $errobj;
            }
        }
    }
    if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt'){
        
        $loc = $form_state['values']['class_location'];
        expDebug::dPrint(' $loc$loc$loc$loc$loc '.print_r($loc, true),4);
        $checkemptygroup = objectLevelAccessCheckInGroupMap('cre_sys_obt_loc',array($loc));
        if(!empty($checkemptygroup)){
            $check=apiUseraccesscheck($createuserid,'cre_sys_obt_loc',$loc);
            if(empty($check)) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_013';
                $errobj->errormsg = "user dont have this privilege";
                return $errobj;
            }
        }
        
        if(empty($form_state['values']['class_location'])) {
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->field = 'class_location';
            $errobj->errormsg = "class_location is required for ilt class delivery type.";
            return $errobj;
        }
        else {
            $checkClassLoc = is_numeric($form_state['values']['class_location']);
            if(!empty($checkClassLoc)) {
            $select = db_select('slt_location', 'location');
            $select->addField('location','id');
            $select->condition('location.status', 'lrn_res_loc_atv');
            $select->condition('location.id', $form_state['values']['class_location']);
            $select->range(0, 1);
            $locaion_id = $select->execute()->fetchField();
            if($locaion_id){
                $form_state['values']['class_location'] = $locaion_id;
            }
            else{
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->field = 'class_location';
                $errobj->errormsg = "The given location id doesn't resolve to a location.";
                return $errobj;

            }
        }
            else {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->field = 'class_location';
                $errobj->errormsg = "The class location id must be a valid numeric.";
                return $errobj;
            }
        }
    }
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
    $classList = classCodeDupeCheck($form_state['values']['code'], '', $form_state['values']['lang_code']);
    if(count($classList)>0) {
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Code already exists.";
        return $errobj;
    }
    $crsIdchk = getCourseTitleDirect($form_state['values']['course_id']);
    if(empty($crsIdchk)){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "Course Id does not exists.";
        return $errobj;
    }
    
    //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
    
    $ret_tmp = getcustomattributemodulestatus();
    if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
            $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_cls');
            expDebug::dPrint('Create Class API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
            if($custom_attr_error != '') {
                $customErrmsg=new stdClass();
                $customErrmsg->isValidateError = 1;
                $customErrmsg->errormsg = $custom_attr_error;
                return $customErrmsg;
            }
    } //#custom_attribute_0078975 - End Check module status
    
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
    $form=array();
    $form["form_id"]="";
    $classId =  addNewClass($form,$form_state,$createuserid,1);

    expDebug::dPrint(' addNewClassByRestAPI created class:' . print_r($courseId,true) , 4);
    if($classId && !(is_array($classId))){
        insertCourseAttachedGroups($classId,$form_state['values']['course_id']);
        insertCourseAttachedUsers($classId,$form_state['values']['course_id']);
        _updateAccessEntity($classId, 'cre_sys_obt_cls', $classId);
        $outdata =  array((object)array('id'=>$classId));
        
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
        //$ret_tmp = getcustomattributemodulestatus();
        if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
            saveCustomAttributesInAddEditForm($form, $form_state, $classId, 'cre_sys_obt_cls', $createuserid,'Inserted',1);
        } //#custom_attribute_0078975 - End Check module status
        
    }else
      $outdata = $classId;
    //To sync solr
    syncSolrData('Class');
    return $outdata;

  } catch (Exception $ex) {
    watchdog_exception('addNewClassByRestAPI', $ex);
    expertusErrorThrow($ex);
  }
}

function updateClassByRestAPI( &$form_state,$createuserid="")
{
        try {
            include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
            $clsStatus = $form_state['values']['status'];
            $classId = $form_state['values']['id'];
            $course_id = $form_state['values']['course_id'];
            $form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values

            if(!is_numeric($classId))
            {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->field = "id";
                $errobj->errormsg = 'Invalid ID';
                return $errobj;
            }
            if(!is_numeric($course_id))
            {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->field = "course_id";
                $errobj->errormsg = 'Invalid Course ID';
                return $errobj;
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
        
            if (!checkValidDeliveryType($form_state['values']['delivery_type'])) { // #45472 - Validate the delivery type
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Invalid/Inactive Delivery Type.";
                return $errobj;
            }
            $classDetails = getCatalogClassDetails($classId);
            if(empty($classDetails)) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Invalid Class id.";
                return $errobj;
            }
        if($form_state['values']['currency_type']){
            $curr_valid = currency_isValidOrNot($form_state['values']['currency_type']);
            if(empty($curr_valid)){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Currency Type is not Valid";
                return $errobj;
            }
        }
            expDebug::dPrint(' updateClassByRestAPI updated class1:' . print_r($classDetails,true) , 4);
            if($classDetails[course_id] != $course_id) {
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Class Id and Course Id does not match.";
                return $errobj;
            }
            $ifClassValid = checkClassValidity($classId, $classDetails['delivery_type']);
            if ($classDetails['delivery_type'] != $form_state['values']['delivery_type']){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Class delivery type is different.";
                return $errobj;
            }
            $canBeCanceled = ifClassCanBeCanceled($classId);
            $invalidStatus = 0;

            if(!in_array($clsStatus, array('lrn_cls_sts_itv','lrn_cls_sts_atv','lrn_cls_sts_can','lrn_cls_sts_dld'))){
                $invalidStatus = 1;
            }
            if(($ifClassValid->flag == 0  || $ifClassValid->coursestatus != "lrn_crs_sts_atv") && $clsStatus == 'lrn_cls_sts_atv'){
                $invalidStatus = 1;
            }
            if($canBeCanceled != 0 && $clsStatus == 'lrn_cls_sts_itv'){
                $invalidStatus = 1;
            }
            if($classDetails['status'] != 'lrn_cls_sts_atv' && $clsStatus == 'lrn_cls_sts_can'){
                $invalidStatus = 1;
            }
            $ifSessioncontent = checkSessionContent($classId);
            if($clsStatus == 'lrn_cls_sts_dld'){
                $resultCnt = delete_object_record($classId,'Class');
                if($resultCnt == 0 && $ifSessioncontent == 1 && ($classDetails['status'] != 'lrn_cls_sts_can')){
                }
                else{
                    $invalidStatus = 1;
                }
            }
            if($canBeCanceled != 0 && $clsStatus == 'lrn_cls_sts_del'){
                $invalidStatus = 1;
            }
            if($invalidStatus){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Invalid class status.";
                return $errobj;
            }
            if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt' || $form_state['values']['delivery_type'] == 'lrn_cls_dty_vcl'){
                if(empty($form_state['values']['max_seats'])){
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->field = 'max_seats';
                    $errobj->errormsg = "max_seats is required for ILT and VC class delivery types.";
                    return $errobj;
                }
                if(!empty($form_state['values']['max_seats']) && !empty($form_state['values']['min_seats'])){
                    if($form_state['values']['min_seats'] >= $form_state['values']['max_seats']){
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->errormsg = "Maximum capacity must be greater than minimum capacity.";
                        return $errobj;
                    }
                }
            }
            if($form_state['values']['delivery_type'] == 'lrn_cls_dty_ilt'){
                if(empty($form_state['values']['class_location'])) {
                    $errobj=new stdClass();
                    $errobj->isValidateError = 1;
                    $errobj->errcode = 'L_012';
                    $errobj->field = 'class_location';
                    $errobj->errormsg = "class_location is required for ilt class delivery type.";
                    return $errobj;
                }
                else {
                    $checkClassLoc = is_numeric($form_state['values']['class_location']);
                    if(!empty($checkClassLoc)) {
                        $select = db_select('slt_location', 'location');
                        $select->addField('location','id');
                        $select->condition('location.status', 'lrn_res_loc_atv');
                        $select->condition('location.id', $form_state['values']['class_location']);
                        $select->range(0, 1);
                        $locaion_id = $select->execute()->fetchField();
                        if($locaion_id){
                            $form_state['values']['class_location'] = $locaion_id;
                        }
                        else{
                            $errobj=new stdClass();
                            $errobj->isValidateError = 1;
                            $errobj->errcode = 'L_012';
                            $errobj->field = 'class_location';
                            $errobj->errormsg = "The given location id doesn't resolve to a location.";
                            return $errobj;
                        }
                    }
                    else {
                        $errobj=new stdClass();
                        $errobj->isValidateError = 1;
                        $errobj->errcode = 'L_012';
                        $errobj->field = 'class_location';
                        $errobj->errormsg = "The class location id must be a valid numeric.";
                        return $errobj;
                    }
                }
            }
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
            if(!empty($form_state['values']['reg_end_date']) && !preg_match( '/^(19|20)\d\d[\-\/.](0[1-9]|1[012])[\-\/.](0[1-9]|[12][0-9]|3[01])$/', $form_state['values']['reg_end_date'])){
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Incorrect date format. It should be in YYYY-MM-DD";
                return $errobj;
            }

            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
            $ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
                $custom_attr_error = validateCustomAttribute($form_state,'cre_sys_obt_cls');
                expDebug::dPrint('Update Class API custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
                if($custom_attr_error != '') {
                    $customErrmsg=new stdClass();
                    $customErrmsg->isValidateError = 1;
                    $customErrmsg->errormsg = $custom_attr_error;
                    return $customErrmsg;
                }
            } //#custom_attribute_0078975 - End Check module status
            
            drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
            $form=array();
            $form["form_id"]="";
            $classId =  updateClassDetails($form,$form_state,$createuserid,1);
            expDebug::dPrint(' updateClassByRestAPI updated class:' . print_r($courseId,true) , 4);
            if($classId && !(is_array($classId))){
                
                //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
                //$ret_tmp = getcustomattributemodulestatus();
                if($ret_tmp == true){
                    include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                    saveCustomAttributesInAddEditForm($form, $form_state, $classId, 'cre_sys_obt_cls', $createuserid,'Updated',1);
                } //#custom_attribute_0078975 - End Check module status
                
                //To sync solr
                syncSolrData('Class');
                return array((object)array('id'=>$classId));
            }else{
                $errobj=new stdClass();
                $errobj->isValidateError = 1;
                $errobj->errcode = 'L_012';
                $errobj->errormsg = "Unable to update a class.";
                return $errobj;
            }
        }
        catch (Exception $ex) {
            watchdog_exception('updateClassByRestAPI', $ex);
            expertusErrorThrow($ex);
        }
}

function checkValidDeliveryType($deliveryType) {
    try {

        $select = db_select('slt_profile_list_items', 'profile');
        $select->addField('profile','code');
        $select->condition('code', '%lrn_cls_dty_%', 'LIKE'); // retrive the class delivery types
        $select->condition('is_active', 'Y', '='); // active status
        $result = $select->execute()->fetchCol();
        if (in_array(trimInput($deliveryType), $result)) {
            return true;
        } else {
            return false;
        }
    } catch (Exception $ex) {
        watchdog_exception('checkValidDeliveryType', $ex);
        expertusErrorThrow($ex);
    }

}
function addEditCatalogCancelRepaintForm($form, &$form_state) {
  try {
    $courseId       = $form_state['course_id'];
    $classId        = $form_state['class_id'];
    expDebug::dPrint('cancel called addeditcatalog callback Ilayaraja :'.$classId , 5);
    $min_form_state       = array(
            'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
            're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
            'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
            'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
          );

    $form_state                                = $min_form_state;
    $form_state['courseId']                    = $courseId;
    $form_state['storage']['new']['course_id'] = $courseId;
    $form_state['storage']['new']['class_id']  = $classId;
    $form_state['storage']['cancelled_call']   = TRUE;
    $form_state['storage']['fromrebuild']            = "fromrebuild";

    $commands = array();
    $statusMsgHTML = theme('status_messages');
    expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
    if (!empty($statusMsgHTML)) {
      // Show the messages
       $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
    }

    $commands[] = exp_ctools_modal_adjust();
    $classId  = core_encrypt($classId); // URL encryption added to close respective qtip
    $entityType = 'cre_sys_obt_crs';
    $entityId = array($courseId);
    $userId = getIdOfLoggedInUser();
    $check = objectLevelAccessCheckInGroupMap($entityType,$entityId);
    $editable= true;
    if(!empty($check)){
        $editCheck = apiUseraccesscheck($userId,$entityType,$courseId);
        if(!empty($editCheck)){
            $editable = true;
        }
        else {
            $editable = false;
        }
    } 
    $commands[] = ajax_command_invoke(NULL, 'cancelClassDetails',array($classId,$editable));
    
    $builtForm            = drupal_build_form('exp_sp_administration_catalog_class_title_form', $form_state);
    $commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
    return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
    watchdog_exception('addEditCatalogCancelRepaintForm', $ex);
    expertusErrorThrow($ex);
  }
}

function exp_sp_administration_cancel_class_form_html($form, &$form_state){
  try {
      $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
       global $theme_key;
        if($theme_key == 'expertusoneV2') {
            //NEWUI THEME STYLE SHEET
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
        } else {
            drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);
        }
      //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);

      $classId = $form_state['cancel_class_id'];
      $form = array();
      $wrapperId = 'cancel_class_html';
      $form['#prefix'] = '<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
      $form['#suffix'] = '</div>';

      // Add a class to the form so that we can apply custom style (margin, padding, etc).
      $form['#attributes'] = array('class' => array('addedit-form'));
      $form['#attributes']['autocomplete'] = 'off';
      $form['#tree'] = TRUE; // Show full path of values (hierarchical values)

      $form['addedit-left-col-start'] = array(
        '#type' => 'markup',
        '#markup' => '<div>',
      );

      $cancelFieldAttributes['class'] = array('addedit-new-field-value');
      $cancelTextId = 'cancel_reason_'.$classId;
      $form[$cancelTextId] = array(
        '#type' => 'textarea',
        '#id'   => $cancelTextId,
        '#prefix' => '<div class="addedit-new-field-title">Cancel Reason:</div><div>',
        '#suffix' => '</div>',
        '#title_display' => 'invisible',
        '#default_value' => '',
        '#attributes' => $cancelFieldAttributes,
        '#rows'=>'2',
        '#cols'=>'60' ,
      );

      $form['cancel-class-form-element'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-addedit-done-action-but"><div class="addedit-form-cancel-container-actions"><span onClick=\'fillCancelReason("'.$classId.'"); return false;\' class="admin-action-button-middle-bg">Close</span>'.
                     '<input type="button" class="pub-unpub-only-save-btn"  value="Done" onClick=\'fillCancelReason("'.$classId.'"); $("#catalog-course-cancel-catalog").click(); return false;\'/>'.
                     '<div class="admin-save-button-right-bg"></div></div></div>'
      );

      $form['addedit-left-col-end'] = array(
        '#type' => 'markup',
        '#markup' => '</div><div class="clearBoth"></div>',
      );
      return $form;
  } catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_cancel_class_form_html', $ex);
    expertusErrorThrow($ex);
  }

}

function class_addedit_display($courseId,$classId,$entityType,$oldClassId='') {
  try {
      $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
      drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
      $display_mode = ($classId) ? 'edit' : '';
      $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'class_id'     => $classId,
       'entity_type'  => $entityType,
       'course_id'   =>  $courseId,
       'display_mode' => $display_mode,
       'oldClassId'   => $oldClassId,
       'cloneEnroll' => ($oldClassId !=0) ? 0 : 1
      );

      $formId                 = 'exp_sp_administration_catalog_class_addedit_form';
      $builtForm              = drupal_build_form($formId, $form_state);
      $renderedTabContentHTML = drupal_render($builtForm)."<script>adminCalSuffix();</script>";
      $drupalSettings         = getDrupalSettings();
      $statusMsgHTML = theme('status_messages');
      expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
      /* if (!empty($statusMsgHTML)) {
        // Show the messages
        $commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML); */
      $commerceResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
        'status_message'        => $statusMsgHTML,
        'cloned'                        =>  $oldClassId
      );
      echo drupal_json_output($commerceResponse);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('class_addedit_display', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * class_pagination_display() - not called anywhere
 */
function class_pagination_display($courseId) {
  try {
      $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
      drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
      $display_mode = ($classId) ? 'edit' : '';
      $form_state = array(
       'no_redirect'  => TRUE, // To confirm whether necessary
       'build_info'   => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'course_id'   =>  $courseId,
      );

      $formId                 = 'exp_sp_administration_catalog_class_title_form';
      $builtForm              = drupal_build_form($formId, $form_state);
      $renderedTabContentHTML = drupal_render($builtForm);
      $drupalSettings         = getDrupalSettings();

      $commerceResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );
      echo drupal_json_output($commerceResponse);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('class_pagination_display', $ex);
    expertusErrorThrow($ex);
  }
}


function class_cancel_display($classId,$entityType) {
  try {
      $form_state = array(
       'no_redirect' => TRUE, // To confirm whether necessary
       'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
       'cancel_class_id' => $classId,
       'entity_type' => $entityType
      );

      $formId = 'exp_sp_administration_cancel_class_form_html';
      $builtForm = drupal_build_form($formId, $form_state);
      $renderedForm = drupal_render($builtForm);
      $renderedTabContentHTML = $renderedForm;
      $drupalSettings = getDrupalSettings();
      $cancelformResponse = array(
        'drupal_settings' => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML,
      );
      echo drupal_json_output($cancelformResponse);
      exit;
  } catch (Exception $ex) {
    watchdog_exception('class_cancel_display', $ex);
    expertusErrorThrow($ex);
  }
}

function getLocationMaxSeatCount($location_id){
  try {
    $select = db_select('slt_location', 'location');
    $select->addField('location','capacity');
    $select->condition('location.id', $location_id);
    return $select->execute()->fetchField();
  } catch (Exception $ex) {
    watchdog_exception('getLocationMaxSeatCount', $ex);
    expertusErrorThrow($ex);
  }
}

function catalog_class_repaint_callback($form, &$form_state){
  try {
     return $form;
  } catch (Exception $ex) {
    watchdog_exception('catalog_class_repaint_callback', $ex);
    expertusErrorThrow($ex);
  }
}

function getprivDetails($classId){
    try {
        $select = db_select("slt_group_mapping","map");
        $select->leftJoin('slt_groups','grp','grp.id=group_id');
        $select->leftJoin('slt_group_privilege','priv','priv.group_id = grp.id');
        //$select->addField('priv','priv_edit','priv_edit');
        $select->addExpression('if(count(map.id) = 0,1,ifnull(max(priv.priv_edit),0))','priv_edit');
        $select->condition('map.entity_id',$classId,'=');
        $select->condition('map.entity_type','cre_sys_obt_cls','=');
        $select->condition('map.group_type',1,'=');
        $select->condition('priv.page_code','cre_sys_obt_crs','=');
        $select->condition('grp.status','cre_sec_sts_atv','='); /*-- #47830: Edit and delete privileges --*/
        expDebug::dPrintDBAPI(" getprivDetails ", $select);
        $result = $select->execute()->fetchField();
        return $result;
    }catch (Exception $ex) {
    watchdog_exception('getprivDetails', $ex);
    expertusErrorThrow($ex);
  }
}

function checkClassCreatedBy( $classId = '', $userId = '') {
    try {
        $userId = getSltpersonUserId($userId);
        $select = db_select('slt_course_class', 'class');
        $select->addField('class', 'created_by');
        $select->condition('id', $classId);
        $select->condition('created_by', $userId);
        expDebug::dPrintDBAPI("checkClassCreatedBy query", $select);
        $result = $select->execute();
        if ($result->rowCount()) {
            return true;
        } else {
            return false;
        }
    } Catch(Exception $ex) {
        watchdog_exception('checkClassCreatedBy', $ex);
        expertusErrorThrow($ex);

    }

}
function getClassDetailsResults($courseId,$classId,$oldClassId){
    try {
  expDebug::dPrint("Catalog class pagiation results called -- ".$classId , 4);
  require_once('exp_sp_administration_catalog_search.inc');
  $lnrSrch = new classSearchPagination();
  $qry_search_results = $lnrSrch -> getSearchResults($courseId,$classId);
  $srch_result_response->total   = $lnrSrch->total_pages;
  $srch_result_response->records = $lnrSrch->qry_search_count;
  $srch_result_response->page    = $lnrSrch->page;
  $srch_result_response->courseId  = core_encrypt($courseId); // URL encryption

  expDebug::dPrint("Catalog class pagiation results called total pages: ".$lnrSrch->total_pages , 4);
  expDebug::dPrint("Catalog class pagiation results called query count: ".$lnrSrch->qry_search_count , 4);
  expDebug::dPrint("Catalog class pagiation results called page : ".$lnrSrch->page , 4);


  $_SESSION['jq_count'] = array();
  $i=0;
  expDebug::dPrint("Catalog class pagiation results:".print_r($qry_search_results,true) , 4);
  foreach ($qry_search_results as $rec_search_results) {

    $rec_search_results->session_details = getSessionDetailsAdmin($courseId,$rec_search_results->cls_id,$rec_search_results->cls_delivery_type_code);

    $rec_search_results->row =  'tr_'.$i;
    $srch_result_response->rows[$i]['id']   = 'tr_'.$i;
    $srch_result_response->rows[$i]['cell'] = array(
            'detail'        => theme('exp_sp_administration_catalog_detail', array('detail' => $rec_search_results,'courseId'=>$courseId,'classId'=>$classId,'oldClassId'=>$oldClassId))
    );
    $_SESSION['jq_modal'][$i] = $rec_search_results->cls_id;
    $i++;
  }
  echo drupal_json_output($srch_result_response);
  } catch (Exception $ex) {
    watchdog_exception('getClassDetailsResults', $ex);
    expertusErrorThrow($ex);
  }
}
function catalog_enroll_display($classId,$courseId,$deliveryType) {
  expDebug::dPrint("catalog_enroll_display: --".$classId , 4);
  try {
    unset($_SESSION['exempted-class-'.$classId]);
  $form_state = array(
   'no_redirect'  => TRUE,
   'build_info'   => array('args' => array('ajax')),
   'enroll_entity_id'    => $classId,
   'enroll_entity_type'  => 'cre_sys_obt_cls',
   'course_id'  => $courseId,
   'class_id'  => $classId,
   'enroll_delivery_type'  => $deliveryType,
   'call_from'  => 'catalog_enroll_display',
  );

  $formId                 = 'catalog_enroll_form_html';
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
    watchdog_exception('catalog_enroll_display', $ex);
    expertusErrorThrow($ex);
  }
}

function catalog_enroll_form_html($form, &$form_state) {
    try {
  include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog.inc');
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_class_register.inc');
  drupal_add_js(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_data_grid.js', $js_module_optional);
  expDebug::dPrint("catalog_enroll_form_html: --", 4);
  $entityId = $form_state['enroll_entity_id'];
  $entityType = $form_state['enroll_entity_type'];
  $uniqueId = $entityId.'-'.$entityType;

  $form['addedit-enroll_launch_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="catalog-class-basic-addedit-form-container" class="catalog-course-basic-addedit-form-container">',
  );
  displayClassRegisterDetails($form, $form_state, $form_state['course_id'], $entityId, $entityType, $uniqueId, $form_state['enroll_delivery_type'],$isclose=1);
  $form['addedit-enroll_launch_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  return $form;
  } catch (Exception $ex) {
    watchdog_exception('catalog_enroll_form_html', $ex);
    expertusErrorThrow($ex);
  }
}

function catalog_enroll_form_html_validate($form, &$form_state) {
    try {
  //exp_sp_administration_catalog_class_addedit_form_validate($form, $form_state);
  include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_class_register.inc');
  enrollUserValidateCheck($form_state);
  $triggerElementByName  = $form_state['triggering_element']['#name'];
  if($triggerElementByName == 'hidden-save'  || $triggerElementByName == 'set-enrolluser-mandatory' ||$triggerElementByName == 'set-enrolluser') {
    $form_state['hidden-save-submit']=true;
    $form_state['esign_show_popup']= false;
  }
  expDebug::dPrint('catalog_enroll_form_html_validate triggered.');
  } catch (Exception $ex) {
    watchdog_exception('catalog_enroll_form_html_validate', $ex);
    expertusErrorThrow($ex);
  }
}

function catalog_enroll_form_html_submit($form, &$form_state) {
    try {
  expDebug::dPrint("Catalog class catalog_enroll_form_html_submit: ".print_r($form_state,true) , 5);
  $entityType = 'cre_sys_obt_cls';
  $entityId = $form_state['enroll_entity_id'];
  $uniqueId = $entityId.'-'.$entityType;
   expDebug::dPrint("Catalog class catalog_enroll_form_html_submit entityType 1: " .$entityType , 4);
  if(isset($form_state['values']['attr_action']) && isset($entityId)) {
     expDebug::dPrint("Catalog class catalog_enroll_form_html_submit entityType 2: " .$entityType , 4);
   if($form_state['values']['attr_action'] == 'enrolluser') {
      expDebug::dPrint("Catalog class catalog_enroll_form_html_submit entityType 3: " .$entityType , 4);
      addNewEnrollUserDetails($form, $form_state, $entityId, $entityType);

      // Clear enrolluser-autocomplete form textfield as switching from enroll user form to roster form - fix for ticket #0022120
      clearClassRosterEnrollUserAutocomplete($form_state);
    }
   $form_state['rebuild']  = TRUE;
   unset($_SESSION['esign_validate']);
  }
  } catch (Exception $ex) {
    watchdog_exception('catalog_enroll_form_html_submit', $ex);
    expertusErrorThrow($ex);
  }
}

function getClassAutoComplete($courseId) {

  try {
      $searchCurText =  $_GET['z'];
      $searchType    =  $_GET['search_type'];


      $select = db_select('slt_course_class', 'cls');
      $select->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
      $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');

      // Add fields to fetch
      //$select->addField('cls', 'code', 'code');

      $select->condition('cls.status', array('lrn_cls_sts_del'), 'NOT IN');
      $select->condition('cls.course_id', $courseId, '=');
      filterByUserAccessPrivileges($select, 'cls','',1,'cre_sys_obt_cls','Courseedit','cls');
      
       if($searchType =='clstit'){
        $classsCondition = db_or();
        $classsCondition->condition('cls.title', '%' . db_like($searchCurText) . '%', 'LIKE');
        $classsCondition->condition('cls.code', '%' . db_like($searchCurText) . '%', 'LIKE');
        $classsCondition->condition('td.tagname', '%' . db_like($searchCurText) . '%', 'LIKE');
        $select->condition($classsCondition);
        $select->addField('cls', 'title', 'title');
       // $select->addField('td','tagname', 'tagname');
        $select->orderBy('title');
       }
       elseif($searchType == 'deltype'){
         if(mb_stripos(t('Classroom'),$searchCurText,null,'UTF-8') !== false) {
            $searchCurText = 'Classroom';
          } else if(mb_stripos(t('Web-based'),$searchCurText,null,'UTF-8') !== false) {
            $searchCurText = 'Web-based';
          } else if(mb_stripos(t('Virtual Class'),$searchCurText,null,'UTF-8') !== false) {
            $searchCurText = 'Virtual Class';
          } else if(mb_stripos(t('Video'),$searchCurText,null,'UTF-8') !== false) {
            $searchCurText = 'Video';
          }
        $delCode = 'lrn_cls_dty_';
        $select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = cls.delivery_type');
        $select->addExpression('DISTINCT(spli.name)', 'title');
        $select->condition('spli.is_active', 'Y');
        $select->condition('spli.name', '%' . db_like($searchCurText) . '%', 'LIKE');
        $select->condition('spli.code', db_like($delCode) . '%', 'LIKE');
        $select->orderBy('spli.name');

      }
       elseif($searchType == 'clsstatus'){
        //$searchStr = '';
        if(mb_stripos(t('Shown in Catalog'),$searchCurText,null,'UTF-8') !== false) {
          $searchCurText = 'Shown in Catalog';
        } else if(mb_stripos(t('Hidden in Catalog'),$searchCurText,null,'UTF-8') !== false) {
          $searchCurText = 'Hidden in Catalog';
        } else if(mb_stripos(t('Completed'),$searchCurText,null,'UTF-8') !== false) {
          $searchCurText = 'Completed';
        } else if(mb_stripos(t('Canceled'),$searchCurText,null,'UTF-8') !== false) {
          $searchCurText = 'Canceled';
        }
        expDebug::dPrint('values of the drop down box'.print_r($searchCurText,true));
        $delCode = 'lrn_cls_sts_';
        $select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = cls.status');
        $select->addExpression('DISTINCT(spli.name)', 'title');
        $select->condition('spli.is_active', 'Y');
        $select->condition('spli.name', '%' . db_like($searchCurText) . '%', 'LIKE');
        $select->condition('spli.code', db_like($delCode) . '%', 'LIKE');
        $select->orderBy('spli.name');

      }

      //selectAddNarrowSearchByTextMultipleORCondition($select, "cont_mast", array("code", "description"), "z");

      // Order in dictionary sort order

       expDebug::dPrint(' Argument id'.$courseId , 4);
      expDebug::dPrintDBAPI(' $select obj = ' , $select);

      $matchingClassList = $select->execute()->fetchAll();
      expDebug::dPrint(' $matchingContentCodesList = ' .
                                                                                                            print_r($matchingClassList, true) , 3);

        foreach ($matchingClassList as $matchingAutoClass) {
         // print $$matchingContentCode->title ." - <b>".$matchingContentCode->code ->code. "</b>\n";
         if($searchType =='clstit') {
          print $matchingAutoClass->title. "\n";
         }
         else {
           print t($matchingAutoClass->title). "\n";
         }
      }
  } catch (Exception $ex) {
    watchdog_exception('getClassAutoComplete', $ex);
    expertusErrorThrow($ex);
  }
}
function getAdministrationCourseDetailsApi($courseId) {
  try {
    $select = db_select("slt_course_class","cls");
    $select->innerJoin('slt_profile_list_items','prof','prof.code = cls.status ');
    $select->innerJoin('slt_profile_list_items','prof1','prof1.code = cls.delivery_type ');
    $select->innerJoin('slt_profile_list_items','prof2','prof2.code = cls.lang_code ');
    $select->leftJoin('slt_course_class_session', 'ses', 'cls.id=ses.class_id and ses.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cls.id ORDER BY start_date LIMIT 1)');
    $select->leftJoin('slt_facility', 'fac', 'ses.facility_id= fac.id');
    $select->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
    $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
    $select->addExpression("group_concat(td.tagname)","tagname");
    $select->addField('cls','id','cls_id');
    $select->addField('cls','title','cls_title');
    $select->addField('prof','name','cls_status');
    $select->addField('prof1','name','cls_delivery_type');
    $select->addField('cls','code','cls_code');
    $select->addField('cls','additional_info','cls_additional_info');
    $select->addField('cls','addn_catalog_show','cls_shown_in_catalog');
    $select->addField('cls','addn_notification_show','cls_shown_in_notification');
    $select->addField('prof2', 'name','cls_language');
    $select->addField('fac', 'name','facility_name');
    $select->groupby('cls.id');
    $select->orderby('cls.id','asc');
    //$select->addExpression('count(1)','cls_count');

    $select->condition('cls.course_id',$courseId,'=');
    $select->condition('cls.status', 'lrn_cls_sts_del' ,'!=');
    filterByUserAccessPrivileges($select, 'cls','',1,'cre_sys_obt_cls','Courseedit','cls');
    
    //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Adding Custom Attribute Fields to Display Columns
    $ret_tmp = getcustomattributemodulestatus();
    if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
        $attrValues = getCustomAttributeValues('Class','mandatory');
        foreach($attrValues as $ckey => $cvalue){
            //$select->addField('cls', $cvalue->attributerefercolumn, strtolower(str_replace(' ','_',$cvalue->attributename)));
        	$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0`
        			WHERE FIND_IN_SET(`opt0`.`opt_code`,`cls`.`$cvalue->attributerefercolumn`)),`cls`.`$cvalue->attributerefercolumn`),`cls`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));
        	
        }        
        	expDebug::dPrint(' $_POST' . print_r($_POST, true) , 5);
        
        	foreach($_POST as $key=>$value) {
        		expDebug::dPrint(' $value ' . print_r($value, true) , 5);
        
        		if(strpos($key,'c_')===0 && $value !=''){
        			expDebug::dPrint(' $key ' . print_r($key, true) , 5);
        
        			//$entityMappingColumn = getCustomAttributeMappingColumn($key);
        			//if($catalogType=='Class')
        				$entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_sys_obt_cls');
        			
        			$per_column = 'cls.'.$entityMappingColumn['crefercolumn'];
        			expDebug::dPrint(' $per_column' . print_r($per_column, true) , 5);
        
        			$custom_attr_option_code=$per_column." LIKE '%".$value."%'";
        
        			//$customattr_opt_o_pos = strpos($value, ','); //for multiple option
        			expDebug::dPrint(' $customattr_opt_o_pos' . print_r($customattr_opt_o_pos, true) , 5);
        			//if ($customattr_opt_o_pos>=0) {
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
        //} //#custom_attribute_0078975 - End Check module status
    } //#custom_attribute_0078975 - End Check module status
    
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $crsList = $select->execute()->fetchAll();
    $entity_type = 'cre_sys_obt_crs';
    $coursemapcheck = objectLevelAccessCheckInGroupMap($entity_type,array($courseId));
    if(!empty($coursemapcheck)){
    $courseaccesscheck = apiUserPrivilegeCheck('Course/Class$$$view',$_REQUEST["userid"],'Course$$$CourseId');
    if(empty($crsList) && empty($courseaccesscheck)){
        $errobj=new stdClass();
        $errobj->isValidateError = 1;
        $errobj->errcode = 'L_012';
        $errobj->errormsg = "user dont have this privelege";
        return $errobj;
    }
    }
        $total_records = count($crsList);
        $crsList['totalrow'] = $total_records;
    return $crsList;
  } catch (Exception $ex) {
    watchdog_exception('getAdministrationCourseDetailsApi', $ex);
    expertusErrorThrow($ex);
  }
 }
/*
 * defaultCourseLanguageForClass() - Get the language applied for course for setting default language to class
 */
function defaultCourseLanguageForClass($courseId){
  try {
    $select = db_select("slt_course_template","cls");
    $select->innerJoin('slt_profile_list_items','spli','spli.code = cls.lang_code');
    $select->addField('cls','lang_code');
    $select->condition('cls.id',$courseId,'=');
    expDebug::dPrintDBAPI(' $select obj = ' , $select);
    $crsList = $select->execute()->fetchField();

    return $crsList;
  } catch (Exception $ex) {
    watchdog_exception('defaultCourseLanguageForClass', $ex);
    expertusErrorThrow($ex);
  }
}
 function fetchClassDetails($classId){
    try{
        expDebug::dPrint('ddddd--->'.$classId);
        $classDetailSel = db_select('slt_course_class','class');
        $classDetailSel->addField('class','id','id');
        $classDetailSel->addField('class','title','Title');
        $classDetailSel->addField('class','code','Code');
        $classDetailSel->addField('class','short_description','Description');
        $classDetailSel->addField('class','lang_code','ClsLang');
        $classDetailSel->addField('class','delivery_type','ClsDeliveryType');
        $classDetailSel->where('class.id=:classid',array(':classid'=>$classId));
        expDebug::dPrintDBAPI('$classDetailSel',$classDetailSel);
        $results = $classDetailSel->execute()->fetchAll();
        return $results;
        //echo drupal_json_output($results);

        //$calltoTPl = exp_sp_administration_view.tpl.php
        //$callto = theme(' exp_sp_administration_view', array());
    }
    catch (Exception $ex) {
        watchdog_exception('fetchClassDetails', $ex);
        expertusErrorThrow($ex);
    }
 }


 function getClassDetailsForView($classId){
    try{
        //include_once(drupal_get_path('module', 'exp_sp_classdetail') .'/exp_sp_classdetail.inc');
        expDebug::dPrint('enter the function11111'.$classId);
        $vLMSUserId = getSltpersonUserId();
        $records = getClassDetailsInfoview($classId,$vLMSUserId);
        expDebug::dPrint('$records-->'.print_r($records,true),5);
        $record = $records[0];
        $record->description = trim($record->description);
        /*//Modified for the Multi currency support
        if($record->currency_type == '' || $record->currency_type == null)
            $record->currency_type = '$';*/

        if(is_null($record->export_compliance)){
            $record->export_compliance = 'N';
        }
        if(is_null($record->price)){
            $record->price = '0.00';
        }

        if ($record->delivery_type_code == 'lrn_cls_dty_ilt' || $record->delivery_type_code == 'lrn_cls_dty_vcl') {
            $sessionArr = array();
            $inc=0;

            $records = getSessionDetailsview($record->crs_id,$record->classid,$record->delivery_type_code);

            foreach($records as $sessDet) {
                $sessionArr[$inc]['title']                =   $sessDet['session_title'];
                $sessionArr[$inc]['facilityid']           =   $sessDet['facility_id'];
                $sessionArr[$inc]['start_date']           =   $sessDet['session_start_format'];
                $sessionArr[$inc]['start_time']           =   $sessDet['session_start_time_format'];
                $sessionArr[$inc]['start_time_format']    =   $sessDet['session_start_time_form'];
                $sessionArr[$inc]['end_time']             =   $sessDet['session_start_end_format'];
                $sessionArr[$inc]['end_time_format']      =   $sessDet['session_end_time_form'];
                $sessionArr[$inc]['session_end_date_format'] =   $sessDet['session_end_date_format'];
                $sessionArr[$inc]['session_name']         =   $sessDet['session_name'];
                $sessionArr[$inc]['session_address1']     =   $sessDet['session_address1'];
                $sessionArr[$inc]['session_address2']     =   $sessDet['session_address2'];
                $sessionArr[$inc]['session_country']      =   $sessDet['session_country'];
                $sessionArr[$inc]['session_state']        =   $sessDet['session_state'];
                $sessionArr[$inc]['session_city']         =   $sessDet['session_city'];
                $sessionArr[$inc]['session_zipcode']      =   $sessDet['session_zipcode'];
                //Added by Vincent on Oct 28, 2013 for #0028593
                $sessionArr[$inc]['ilt_start_date']       =   $sessDet['ilt_session_start_format'];
                $sessionArr[$inc]['ilt_start_time']       =   $sessDet['ilt_session_start_time_format'];
                $sessionArr[$inc]['ilt_start_time_format']=   $sessDet['ilt_session_start_time_form'];
                $sessionArr[$inc]['ilt_end_time']         =   $sessDet['ilt_session_start_end_format'];
                $sessionArr[$inc]['ilt_end_time_format']  =   $sessDet['ilt_session_end_time_form'];
                $sessionArr[$inc]['ilt_session_end_date_format'] =   $sessDet['ilt_session_end_date_format'];
                $inc++;
            }
            expDebug::dPrint('$sessionArr : '.print_r($sessionArr, true) , 4);
            if (empty($record->startdatetime)||empty($record->enddatetime)) {

                $record->sessionDetails = '-';
            }
            else {
                $record->sessionDetails = dateTimeStrToUnivShort($record->startdatetime, 'Y','TRUE',false,false)." - ". dateTimeStrToUnivShort($record->enddatetime, 'Y','TRUE',false,false);
            }
            if (empty($record->locationid)) {
                $record->locationaddr1 = '';
                $record->locationaddr2 = '';
                $record->citystate = '';
                $record->locationzip = '';
                $record->locationcountryname = '';
            }
            else {
                if (empty($record->locationname)) {
                    $record->locationname = '-';
                }
                $locCityState = array();
                if(!empty($record->locationcity)) {
                    $locCityState[] = $record->locationcity;
                }
                if(!empty($record->locationstate)) {
                    $locCityState[] = $record->locationstate;
                }
                $record->citystate = (count($locCityState)>1) ?  implode(", ", $locCityState) : $locCityState[0];
            }
            if(!isset($record->startdatetime))
                $record->startdatetime="";
            if(!isset($record->enddatetime))
                $record->enddatetime="";

            $record->sessionDetails    = dateTimeStrToUnivShort($record->startdatetime,'Y','TRUE',false,false)." - ". dateTimeStrToUnivShort($record->enddatetime,'Y','TRUE',false,false);
            $record->available_seats   = getAvailableSeatsView($classId);
            $record->waitlist_seats    = getWaitlistCatalogInfoView($classId);
            $record->sessionDetailInfo = $sessionArr;

        }
        else {
            //$location = '-';
            $record->sessionDetails   = '-';
            $record->available_seats  = '-';
            $record->waitlist_seats   = '-';
        }
        $record->multi_register = is_multiregistration_allowed($classId);
        expDebug::dPrint($record , 4);
        return $record;
    }catch (Exception $ex) {
        watchdog_exception('getClassDetails', $ex);
        expertusErrorThrow($ex);
    }
 }


 function getClassDetailsInfoview($classId, $vLMSUserId) {
    try{
        //include_once(drupal_get_path('module', 'exp_sp_classdetail') .'/exp_sp_classdetail.inc');
        expDebug::dPrint(" classIdAndSessionId = ". $classId . "-" . $sessionId , 4);
        //$vLMSUserId = getSltpersonUserId();
        $sessionCnt = _getClassSessionCountView($classId);
        //if (!empty($sessionId)) {
        if($sessionCnt) {
            $classDetailSel = db_select('slt_course_class','class');
            $classDetailSel->leftJoin('slt_node_learning_activity','nla','class.id = nla.entity_id AND nla.entity_type = \'cre_sys_obt_cls\'');
            $classDetailSel->leftJoin('slt_course_class_session','ses','class.id = ses.class_id');
            $classDetailSel->leftJoin('slt_location','loc','ses.location_id = loc.id');
            $classDetailSel->leftJoin('slt_state','sname','loc.state = sname.state_code');
            $classDetailSel->leftJoin('slt_country','con','loc.country = con.country_code');
        //  $classDetailSel->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
            // $classDetailSel->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
        //  $classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.id AND group_map.entity_type = \'cre_sys_obt_cls\'');
            //$classDetailSel->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id');



            $classDetailSel->addField('class','title','Title');
            $classDetailSel->addField('class','id','classid');
            $classDetailSel->addField('class','course_id','crs_id');
            $classDetailSel->addField('class','code','Code');
            $classDetailSel->addField('class','short_description','Description');
            $classDetailSel->addField('class','additional_info','additional_info');
            $classDetailSel->addField('class','addn_catalog_show','addn_catalog_show');
            $classDetailSel->addField('class','addn_notification_show','addn_notification_show');
            $classDetailSel->addField('class','scheduled_duration','Duration');
            $classDetailSel->addField('class','course_id','courseid');
            $classDetailSel->addField('class','custom4','export_compliance');
            $classDetailSel->addField('class','lang_code','ClsLang');
            $classDetailSel->addField('class','delivery_type','delivery_type_code');
            $classDetailSel->addField('class','delivery_type','ClsDeliveryType');
            $classDetailSel->addField('class','is_compliance','is_compliance');
            $classDetailSel->addField('ses','start_date','startdatetime');
            $classDetailSel->addField('ses','end_date','enddatetime');
            $classDetailSel->addField('ses','title','sessionTitle');
            $classDetailSel->addField('ses','start_time','sessionStartTime');
            $classDetailSel->addField('ses','end_time','sessionEndTime');
        //  $classDetailSel->addField('group_map','mro', 'mro_id');

            $classDetailSel->addField('loc','id','LocationId');
            $classDetailSel->addField('loc','name','LocationName');
            $classDetailSel->addField('loc','addr1','LocationAddr1');
            $classDetailSel->addField('loc','addr2','LocationAddr2');
            $classDetailSel->addField('loc','city','LocationCity');
            $classDetailSel->addExpression('(SELECT  IF(COUNT(state_name)>0,state_name,loc.state)  stname FROM slt_state WHERE state_code= loc.state limit 1)','LocationState');
            $classDetailSel->addField('loc','country','LocationCountry');
            $classDetailSel->addField('con','country_name','LocationCountryName');
            $classDetailSel->addField('loc','zipcode','LocationZip');
            $classDetailSel->addField('loc','phone','LocationPhone');
            $classDetailSel->addField('loc','latitude','Latitude');
            $classDetailSel->addField('loc','longitude','Longitude');

            $classDetailSel->addField('nla','node_id','node_id');

            $classDetailSel->addField('ses','timezone','TimeZone');

            $classDetailSel->addField('class','price','price');
            //Modified for the Multi currency support
            //$classDetailSel->addField('class','currency_type','currency_type');
            $classDetailSel->addField('class','currency_type','currency_type');

            $classDetailSel->addField('class','custom4','CurrencyPrefixSymbol');
            $classDetailSel->addField('class','custom4','CurrencyPostfixSymbol');
            $classDetailSel->addField('class','max_seats','MaxCapacity');
            $classDetailSel->addField('class','waitlist_count','AvailablibleSeatCount');
            $classDetailSel->addField('class','valid_to','EnrollmentValidity');
            $classDetailSel->addField('class','author_vendor','author');

            $classDetailSel->addField('class','registration_end_on', 'registration_end_on');
            $classDetailSel->groupBy('class.id');
            //filterByUserAccessPrivileges($classDetailSel, 'catacs',$vLMSUserId,'group_map');
            // below line commented for Mantis - SQL Injection #31297
            //$classDetailSel->condition('class.id',$classId);
            $classDetailSel->where('class.id=:classid',array(':classid'=>$classId));
            //expDebug::dPrintDBAPI('$classDetailSel$classDetailSel-->',$classDetailSel);
        }
        else {
            expDebug::dPrint('else part');
            $classDetailSel = db_select('slt_course_class','class');
            //$classDetailSel->join('slt_node_learning_activity','nla','class.id = nla.entity_id AND nla.entity_type = \'cre_sys_obt_cls\'');
            //$classDetailSel->leftJoin('slt_common_mapping','pre','class.id = pre.id2');
            //$classDetailSel->leftJoin('slt_mro_mapping', 'mro', 'mro.entity_id=class.id AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$vLMSUserId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
            //$classDetailSel->leftJoin('slt_catalog_access', 'catacs', 'catacs.entity_id=class.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
            //$classDetailSel->leftJoin('slt_group_mapping','group_map','group_map.entity_id=class.id AND group_map.entity_type = \'cre_sys_obt_cls\'');
            //$classDetailSel->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id');

            $classDetailSel->addField('class','title','Title');
            $classDetailSel->addField('class','code','Code');
            $classDetailSel->addField('class','id','classid');
            $classDetailSel->addField('class','course_id','crs_id');
            $classDetailSel->addField('class','short_description','Description');
            $classDetailSel->addField('class','additional_info','additional_info');
            $classDetailSel->addField('class','addn_catalog_show','addn_catalog_show');
            $classDetailSel->addField('class','addn_notification_show','addn_notification_show');
            $classDetailSel->addField('class','course_id','courseid');
            $classDetailSel->addField('class','custom4','export_compliance');
            $classDetailSel->addField('class','scheduled_duration','Duration');
            $classDetailSel->addField('class','lang_code','ClsLang');
            $classDetailSel->addField('class','delivery_type','delivery_type_code');
            $classDetailSel->addField('class','delivery_type','ClsDeliveryType');
            $classDetailSel->addField('class','is_compliance','is_compliance');
            //$classDetailSel->addField('nla','node_id','node_id');
            $classDetailSel->addField('class','price','price');
            //Modified for the Multi currency support
            //$classDetailSel->addField('class','currency_type','currency_type');
            $classDetailSel->addExpression('(Select attr2 from slt_profile_list_items where attr1 = class.currency_type)','currency_type');
            $classDetailSel->addField('class','custom4','CurrencyPrefixSymbol');
            $classDetailSel->addField('class','custom4','CurrencyPostfixSymbol');
            $classDetailSel->addField('class','max_seats','MaxCapacity');
            $classDetailSel->addField('class','waitlist_count','AvailablibleSeatCount');
            $classDetailSel->addField('class','valid_to','EnrollmentValidity');
            $classDetailSel->addField('class','author_vendor','author');
            $classDetailSel->addField('class','registration_end_on', 'registration_end_on');
            //$classDetailSel->addField('mro','mro_flag', 'mro_id');
            //filterByUserAccessPrivileges($classDetailSel, 'catacs',$vLMSUserId);
            // below line commented for Mantis - SQL Injection #31297
            //$classDetailSel->condition('class.id',$classId);
            $classDetailSel->where('class.id=:classid',array(':classid'=>$classId));
        //  expDebug::dPrintDBAPI('$classDetailSel1111-->',$classDetailSel);
        }
        //filterByUserAccessPrivileges($classDetailSel, 'class','',1,'','Class','class');
        expDebug::dPrintDBAPI('$classDetailSel1111--> query',$classDetailSel);
        $results = $classDetailSel->execute()->fetchAll();
        expDebug::dPrint('getClassDetailsInfo : = ' . print_r($results, true) ,5);
        if (count($results) > 0) {
            // Call the new function
            $prereqequv = getPrerequisiteclassDetailsView($classId);
            $prereqArr = Array();
            $equvArr = Array();
            foreach ($prereqequv as $row){
                if($row->cmp_type == "5"){
                    $prereqArr[] = $row;
                }else{
                    $equvArr[] = $row;
                }
            }
            $results[0]->prerequisites = $prereqArr;
            $results[0]->equivalence = $equvArr;

        }

        return $results;


        //  Return the result

        expDebug::dPrint(' getClassDetailsInfo() SQL:  = '. print_r($results->__toString(), true) , 3);
        expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($results), true) , 3);
    }catch (Exception $ex) {
        watchdog_exception('getClassDetailsInfoview', $ex);
        expertusErrorThrow($ex);
    }
 }

 function _getClassSessionCountView($classId) {
    try{
        // Select table
        expDebug::dPrint('count1111'.$classId);
        $selectStmt = db_select('slt_course_class_session', 'sess');
        // Set conditions
        $selectStmt->condition('sess.class_id', $classId);
        // Select fields
        $selectStmt->distinct();
        $selectStmt->addField('sess', 'id', 'session_id'); // This is course id.
        // Execute query
        $result = $selectStmt->execute()->fetchAll();
        expDebug::dPrint('$$result-->'.print_r($result,true),5);
        $countSession = ((count($result)>0) ? count($result) : 0);
        return $countSession;
    }catch (Exception $ex) {
        watchdog_exception('_getClassSessionCountView', $ex);
        expertusErrorThrow($ex);
    }
 }

 function getPrerequisiteclassDetailsView($classId) {
    try{
        // Select table
        $selectStmt = db_select('slt_course_class', 'cls');

        $selectStmt->rightJoin('slt_common_mapping','cmp','cls.course_id = cmp.id1');
        $selectStmt->leftJoin('slt_course_template','crs','cmp.id2 = crs.id');


        // Set conditions
        $selectStmt->condition('cls.id', $classId);
        $selectStmt->condition('cmp.object_type', 'cre_sys_obt_crs');

        // Select fields
        $selectStmt->distinct();
        $selectStmt->addField('crs', 'id', 'course_id'); // This is course id.
        $selectStmt->addField('cmp', 'type', 'cmp_type');
        $selectStmt->addField('crs', 'title', 'crs_title');
        $selectStmt->addField('crs', 'code', 'crs_code');   // Note that this is the class title and not course title.

        // Execute query
        $result = $selectStmt->execute()->fetchAll();

        return $result;

        //  Return the result
        expDebug::dPrint(' getPrerequisiteclassDetails : = ' . print_r($result, true) , 3);
        expDebug::dPrint(' getPrerequisiteclassDetails() SQL:  = '. print_r($result->__toString(), true) , 3);
        expDebug::dPrint('result_response after json encoding = '.print_r(drupal_json_encode($result), true) , 3);
    }catch (Exception $ex) {
        watchdog_exception('getPrerequisiteclassDetails', $ex);
        expertusErrorThrow($ex);
    }
 }

 function getSessionDetailsview($courseid,$classid,$delType,$timeFor="",$userId="") {
    try{
        $selectSess = db_select('slt_course_class_session', 'sess');
        $selectSess->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=sess.id');
        $selectSess->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
        $selectSess->leftjoin('slt_location', 'splt_loc', 'splt_loc.id = sess.location_id');
        $selectSess->leftjoin('slt_profile_list_items', 'loc_timezone', 'loc_timezone.code=splt_loc.timezone');
        $selectSess->addField('sess','id', 'session_id');
        $selectSess->addField('sess','start_date', 'session_start');
        $selectSess->addField('sess','end_date', 'session_end');
        $selectSess->addField('sess','start_time', 'session_start_time');
        $selectSess->addField('sess','end_time', 'session_end_time');
        $selectSess->addField('sess','title', 'session_title');
        $selectSess->addField('sess','course_id', 'course_id');
        $selectSess->addField('sess','class_id', 'class_id');
        //$selectSess->addField('sess','instructor_id', 'session_instructor_id');
        $selectSess->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'session_instructor_id');
        $selectSess->addField('splt_timezone','attr2', 'session_timezone');
        $selectSess->addField('splt_timezone','name', 'session_fulltimezone');
        $selectSess->addField('loc_timezone','attr2', 'location_timezone');
        $selectSess->addField('loc_timezone','name', 'location_fulltimezone');
        $selectSess->addField('splt_loc','name', 'name');
        $selectSess->addField('splt_loc','addr1', 'contact_addr1');
        $selectSess->addField('splt_loc','addr2', 'contact_addr2');
        $selectSess->addExpression('(select country_name from slt_country where country_code = splt_loc.country)', 'contact_country');
        $selectSess->addExpression('(select state_name from slt_state where state_code = splt_loc.state and country_code =  splt_loc.country)', 'contact_state');
        $selectSess->addField('splt_loc','city', 'contact_city');
        $selectSess->addField('splt_loc','zipcode', 'contact_zipcode');
        $selectSess->addField('splt_loc','latitude', 'contact_latitude');
        $selectSess->addField('splt_loc','longitude', 'contact_longitude');
        $selectSess->condition('sess.class_id', $classid,'=');
        $selectSess->condition('sess.course_id', $courseid,'=');
        $selectSess->groupBy('sess.id');
        $selectSess->orderBy('sess.start_date', 'asc');
        $selectSess->orderBy('sess.start_time', 'asc');
        $sessQryRes = $selectSess->execute()->fetchAll();
        expDebug::dPrintDBAPI("ILT/VC session detail : ",$selectSess);
        $sessionDetails = array();
        if(empty($userId)) {
            $userId = getSltpersonUserId();
        }
        foreach ($sessQryRes as $record) {
            $ses_start_date_form     = explode(" ",$record->session_start);
            $ses_start_date_format   = $ses_start_date_form[0]." ".$record->session_start_time;
            $ses_end_date_form       = explode(" ",$record->session_start);
            $ses_end_date_format     = $ses_end_date_form[0]." ".$record->session_end_time;

            $ilt_ses_start_date_format = $ses_start_date_format;
            $ilt_ses_end_date_format        = $ses_end_date_format;
            $currentDate;
            /* conversion of timezone for vc and ilt classes */
            $vTimeZone  = ($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone;
            if($delType == 'lrn_cls_dty_vcl' || ($delType == 'lrn_cls_dty_ilt' && $vTimeZone)){
                $vStartDateTime = $ses_start_date_format; //date_format(date_create($ses_start_date_format),'Y-m-d H:i');
                $vEndDateTime   = $ses_end_date_format; //date_format(date_create($ses_end_date_format),'Y-m-d H:i');
                if($userId){
                    $UserPHPTimezoneName        = ($delType == 'lrn_cls_dty_vcl')?getUserPHPTimezoneName($userId):date_default_timezone(FALSE);//date_default_timezone(FALSE);
                    $vUserPHPTimezonename       = $UserPHPTimezoneName[0]->zone_name;
                    if(!$UserPHPTimezoneName[0]->zone_name){
                        $vUserPHPTimezonename = date_default_timezone(FALSE);
                    }
                }else{
                    $vUserPHPTimezonename = date_default_timezone(FALSE);
                }
                $oStartDateTime  = timeZoneConvert($vStartDateTime , $vTimeZone,  $vUserPHPTimezonename);
                $ses_start_date_format  = date_format($oStartDateTime,'Y-m-d H:i');
                $oEndDateTime    = timeZoneConvert($vEndDateTime, $vTimeZone,  $vUserPHPTimezonename);
                $ses_end_date_format    = date_format($oEndDateTime,'Y-m-d H:i');
                $start_time = date_format($oStartDateTime,'H:i');
                $end_time = date_format($oEndDateTime,'H:i');
                $session_start = date_format(date_create($ses_start_date_format),'Y-m-d')." 00:00:00";
                $session_day = ($record->session_start != null ) ? date_format(date_create($ses_start_date_format),'D') : '';
                $sessionTZ = new DateTimeZone($vUserPHPTimezonename);
                $currentDate = new DateTime(null,$sessionTZ) ;
            }else{
                $start_time = $record->session_start_time;
                $end_time = $record->session_end_time;
                $session_start = $record->session_start;
                $session_day = ($record->session_start != null ) ? date_format(date_create($record->session_start),'D') : '';
                $vUserPHPTimezonename = date_default_timezone(FALSE);
                $sessionTZ = new DateTimeZone($vUserPHPTimezonename);
                $currentDate = new DateTime(null,$sessionTZ) ;
            }
            $sessionDetails[] = array(
                    'session_id'                 => $record->session_id,
                    'session_instructor_id'      => $record->session_instructor_id,
                    'courseid'                   => $courseid,
                    'classid'                    => $classid,
                    'facility_id'                                => $record->facility_id,
                    'session_start'              => $session_start,
                    'session_start_day'                  => $session_day,
                    'session_end'                => $record->session_end,
                    'session_start_time'         => $start_time,
                    'session_end_time'           => $end_time,
                    'session_title'                          => $record->session_title,
                    'session_start_format'       => ($timeFor == "enroll") ? date_format(date_create($ses_start_date_format),'M d, Y h:i') : date_format(date_create($ses_start_date_format),'M d, Y'),
                    'session_end_format'         => date_format(date_create($ses_end_date_format),'M d, Y h:i'),
                    'session_start_date_format'  => date_format(date_create($ses_start_date_format),'M d, Y'),
                    'session_end_date_format'    => date_format(date_create($ses_end_date_format),'M d, Y'),
                    'session_start_time_format'  => date_format(date_create($ses_start_date_format),'h:i'),
                    'session_start_end_format'   => date_format(date_create($ses_end_date_format),'h:i'),
                    'session_start_time_form'    => date_format(date_create($ses_start_date_format),'A'),
                    'session_end_time_form'      => date_format(date_create($ses_end_date_format),'A'),
                    'session_start_time_full'        => date_format(date_create($ses_start_date_format),'M d, Y H:i'),
                    'session_end_time_full'          => date_format(date_create($ses_end_date_format),'M d, Y H:i'),

                    // To display the ILT session in respective locations timezone
                    'ilt_session_start_time'         => $record->session_start_time,
                    'ilt_session_end_time'           => $record->session_end_time,
                    'ilt_session_start_date_format'  => date_format(date_create($ilt_ses_start_date_format),'M d, Y'),
                    'ilt_session_end_date_format'    => date_format(date_create($ilt_ses_end_date_format),'M d, Y'),
                    'ilt_session_start_time_format'  => date_format(date_create($ilt_ses_start_date_format),'h:i'),
                    'ilt_session_start_end_format'   => date_format(date_create($ilt_ses_end_date_format),'h:i'),
                    'ilt_session_start_time_form'    => date_format(date_create($ilt_ses_start_date_format),'A'),
                    'ilt_session_end_time_form'      => date_format(date_create($ilt_ses_end_date_format),'A'),
                    'ilt_session_start_format'       => ($timeFor == "enroll") ? date_format(date_create($ilt_ses_start_date_format),'M d, Y h:i') : date_format(date_create($ilt_ses_start_date_format),'M d, Y'),
                    'ilt_session_start_day'      => ($record->session_start != null ) ? date_format(date_create($ilt_ses_start_date_format),'D') : '',
                    'ilt_session_end_format'         => date_format(date_create($ilt_ses_end_date_format),'M d, Y h:i'),
                    'ilt_session_start_time_full'        => date_format(date_create($ilt_ses_start_date_format),'M d, Y H:i'),
                    'ilt_session_end_time_full'          => date_format(date_create($ilt_ses_end_date_format),'M d, Y H:i'),
                    'server_date_time'                           => $currentDate->format('M d, Y H:i'),

                    'session_name'                           => $record->name,
                    'session_address1'                   => $record->contact_addr1,
                    'session_address2'                   => $record->contact_addr2,
                    'session_country'                        => $record->contact_country,
                    'session_state'                          => $record->contact_state,
                    'session_city'                           => $record->contact_city,
                    'session_zipcode'                        => $record->contact_zipcode,
                    'sess_timezone'                          => ($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone,
                    'sess_fulltimezone'                  => ($delType == 'lrn_cls_dty_vcl') ? $record->session_fulltimezone : $record->location_fulltimezone,
                    'session_latitude'                   => $record->contact_latitude,
                    'session_longitude'                  => $record->contact_longitude
            );
        }
        return $sessionDetails;
    }catch (Exception $ex) {
        watchdog_exception('getSessionDetailsview', $ex);
        expertusErrorThrow($ex);
    }
 }

 function getAvailableSeatsView($classid){
    try{
        $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
        $no_of_confirmed = getClassEnrollmentCountForRegistrationStatuses(null, $classid, $regStatusesList);

        $select = db_select('slt_course_class_session', 'sc');
        $select->addField('sc','capacity_max', 'capacity_max');
        $select->condition('sc.class_id',$classid, '=');
        $select->range(0, 1);
        $qry_result_session = $select->execute()->fetchObject();
        expDebug::dPrintDBAPI("Class max seats for check seat availability : ",$select);
        if ($qry_result_session->capacity_max != 0) {
            $no_of_available_seats = $qry_result_session->capacity_max - $no_of_confirmed;
        }

        if ($no_of_available_seats < 0 ) {
            $no_of_available_seats = 0;
        }
        expDebug::dPrint("Available Seat count".$no_of_available_seats,5);
        return $no_of_available_seats;
    }catch (Exception $ex) {
        watchdog_exception('getAvailableSeatsView', $ex);
        expertusErrorThrow($ex);
    }
 }

 function getWaitlistCatalogInfoView($classid,$userId = '',$From = '') {
    try{
        $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl');
        $no_of_confirmed = getClassEnrollmentCountForRegistrationStatuses(null, $classid, $regStatusesList,$userId);
        $select = db_select('slt_course_class_session', 'sc');
        $select->addField('sc','capacity_max', 'capacity_max');
        $select->addField('sc','waitlist_count', 'waitlist_count');
        $select->condition('sc.class_id',$classid, '=');
        $select->range(0, 1);
        $qry_result_session = $select->execute()->fetchObject();
        expDebug::dPrintDBAPI("Get max and waitlist seats for class : ",$select);
        $no_of_waitlist_seats = 0;
        if ($qry_result_session->capacity_max != 0) {
            if($qry_result_session->capacity_max <= $no_of_confirmed) {
                $no_of_waitlist_seats = ($qry_result_session->capacity_max + $qry_result_session->waitlist_count) - $no_of_confirmed;
            } else {
                $no_of_waitlist_seats = ($From == 'tp') ? ($qry_result_session->capacity_max + $qry_result_session->waitlist_count) : 0;
            }
        }
        if ($no_of_waitlist_seats < 0 ) {
            $no_of_waitlist_seats = 0;
        }
        expDebug::dPrint("getWaitlistCatalogInfo action :".$classid."  ==> capacity_max: ".$qry_result_session->capacity_max." ,
                waitlist_count: ".$qry_result_session->waitlist_count." , no_of_confirmed :".$no_of_confirmed.", Available seats : ".$no_of_waitlist_seats , 5);
        return $no_of_waitlist_seats;
    }catch (Exception $ex) {
        watchdog_exception('getWaitlistCatalogInfoView', $ex);
        expertusErrorThrow($ex);
    }
 }

 function getAttachmentsInfoView($entityId, $entityType=''){
    try{
        //$entityType = ($entityType == '')? 'cre_sys_obt_cls' : $entityType;
        $AttachmentSel =db_select('slt_entity_reading_mapper','serm');
        $AttachmentSel->addField('serm','id');
        $AttachmentSel->addField('serm','reading_id','sequence');
        $AttachmentSel->addField('serm','reading_type','title');
        $AttachmentSel->addField('serm','reading_content','content');
        $AttachmentSel->condition('serm.entity_id',$entityId,'=');
        $AttachmentSel->condition('serm.entity_type',$entityType,'=');
        $AttachmentSel->condition('serm.showlnr',"Y",'=');
        expDebug::dPrintDBAPI(' $$AttachmentSel = ', $AttachmentSel);
        return $AttachmentSel->execute()->fetchAll();
    }catch (Exception $ex) {
        watchdog_exception('getAttachmentsInfo', $ex);
        expertusErrorThrow($ex);
    }

 }
 
 //Commented for #0064996
 /* function getCountOfLessonView($classid) {
    $txn = db_transaction();
    try{
        $selectStmt = db_select('slt_content_version', 'cont_vers');
        $selectStmt->addExpression('SUM(cont_vers.total_lesson)', 'lesson_cnt');
        $selectStmt->leftJoin('slt_content_master', 'cont_mast', 'cont_vers.content_master_id=cont_mast.id');
        $selectStmt->leftJoin('slt_course_content_mapper', 'cc_map', 'cont_mast.id=cc_map.content_id');
        $selectStmt->leftJoin('slt_profile_list_items', 'slpi', 'cont_mast.type=slpi.code and cont_mast.status=slpi.code');

        $selectStmt->condition('cont_mast.status', 'lrn_cnt_sts_del', '!=');
        $selectStmt->condition('cont_vers.content_sub_type',array('lrn_cnt_typ_vod'), 'NOT IN');
        // below line commented for Mantis - SQL Injection #31297
        // $selectStmt->condition('cc_map.class_id', $classid, '=');
        if(!empty($ccmapid)){
         $selectStmt->condition('cc_map.id', $ccmapid, '=');
        }
        $selectStmt->where('cc_map.class_id=:classid', array(':classid'=>$classid));


        expDebug::dPrintDBAPI('count of lessons', $selectStmt);
        $result = $selectStmt->execute()->fetchField();
        expDebug::dPrint('result of the lesson count'.$result, 3);
        return $result;
    }catch (Exception $ex) {
        $txn->rollback();
        unset($txn);
        watchdog_exception('getCountOfLesson', $ex);
        expertusErrorThrow($ex);
    }
    unset($txn);
 } */

 function getBusinessInfoView($classid){
    try{
        $selectStmt = db_select('slt_business_rule_mapping', 'busi');
        $selectStmt->leftJoin('slt_profile_list_items', 'slpi', 'busi.business_rule_code=slpi.code');
        $selectStmt->addField('slpi','name', 'name');
        $selectStmt->condition('busi.entity_id', $classid, '=');
        expDebug::dPrintDBAPI('classroom', $selectStmt);
        $result = $selectStmt->execute()->fetchField();
        expDebug::dPrint('result of classroom'.$result, 3);
        return $result;
    }catch (Exception $ex) {
        watchdog_exception('getBusinessInfoView', $ex);
        expertusErrorThrow($ex);
    }
 }
 //Added slt_content_version table to get the lesson detail for #0064996
 function getContentTitleForView($classid,$contentId='',$callFromAPI = 0) {
    try {
        $selectStmt = db_select('slt_course_content_mapper', 'cont');
        $selectStmt->distinct();
        $selectStmt->addField('cont','id', 'id');
        $selectStmt->addField('master','code', 'title');
        $selectStmt->addField('cont','max_attempts', 'attempts');
        $selectStmt->addField('cont','validity_days', 'validity');
        $selectStmt->leftJoin('slt_content_master', 'master', 'master.id=cont.content_id');
        $selectStmt->leftJoin('slt_content_version', 'cont_vers', 'cont_vers.content_master_id=cont.content_id');
        $selectStmt->addField('cont_vers','total_lesson', 'lesson');
        $selectStmt->condition('cont.class_id', $classid, '=');
        $selectStmt->condition('cont_vers.status','lrn_cnt_sts_atv','=');
        if($callFromAPI){
            $selectStmt->condition('cont.content_id', $contentId, '=');
            $selectStmt->addField('cont','id', 'id');
        }
        expDebug::dPrintDBAPI('getContentTitleForView', $selectStmt);
        $result = $selectStmt->execute()->fetchAll();
        expDebug::dPrint('getContentTitleForView'.print_r($result,true), 3);
        return $result;
    }catch (Exception $ex) {
        watchdog_exception('getContentTitleForView', $ex);
        expertusErrorThrow($ex);
    }
 }
 function getSurveyName($classid,$type=''){
    try{
        $selectStmt = db_select('slt_survey', 'sur');
        $selectStmt->leftJoin('slt_survey_mapping', 'surMap', 'sur.id=surMap.survey_id');
        $selectStmt->leftJoin('slt_module','module','module.id = surMap.module_id');
        $selectStmt->addField('sur','title', 'title');
        $selectStmt->addField('module','title','path');
        $selectStmt->condition('surMap.object_id', $classid, '=');
        $selectStmt->condition('surMap.object_type', $type, '=');
        $selectStmt->condition('sur.type', 'sry_det_typ_sry', '=');         
        $result = $selectStmt->execute()->fetchAll();
        expDebug::dPrintDBAPI("MY SURVEY QUERYY",$selectStmt);
        expDebug::dPrint('getSurveyName'.print_r($result,true), 4);
        return $result;
    }catch (Exception $ex) {
        watchdog_exception('getSurveyName', $ex);
        expertusErrorThrow($ex);
    }
 }
 function getAssessmentName($classid,$type=''){
    try{
        $selectStmt = db_select('slt_survey', 'sur');
        $selectStmt->leftJoin('slt_survey_mapping', 'surMap', 'sur.id=surMap.survey_id');
        $selectStmt->leftJoin('slt_module','module','module.id = surMap.module_id');
        $selectStmt->addField('sur','title', 'title');
        $selectStmt->addField('surMap','no_of_attempts', 'attempts');
        $selectStmt->addField('module','title','path');
        $selectStmt->condition('surMap.object_id', $classid, '=');
        $selectStmt->condition('surMap.object_type', $type, '=');
        $selectStmt->condition('sur.type', 'sry_det_typ_ass', '=');         
        $result = $selectStmt->execute()->fetchAll();
        expDebug::dPrintDBAPI("MY ASSESS QUERYY",$selectStmt);
        expDebug::dPrint('getContentTitleForView'.print_r($result,true), 3);
        return $result;
    }catch (Exception $ex) {
        watchdog_exception('getAssessmentName', $ex);
        expertusErrorThrow($ex);
    }
 }
?>
