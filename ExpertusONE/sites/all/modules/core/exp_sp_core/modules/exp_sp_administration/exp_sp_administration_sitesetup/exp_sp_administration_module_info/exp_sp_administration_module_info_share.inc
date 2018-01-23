<?php
include_once $_SERVER["DOCUMENT_ROOT"]. "/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc";
function exp_sp_administration_share_display_addedit_form($js = NULL) {
    try {
            global $theme_key;
            $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
            drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
            drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info_share.js', $js_module_optional);
            $share_style = array(
                'ctools-admin-share-style' => array(
                        'modalSize' => array(
                                      'width' => 400,
                                      'minHeight' => 200,
                                     ),
                                         )
        );
        drupal_add_js($share_style, 'setting');
        if ($js) {
            $argsList = array();
            $formIdList = array(
                    	        array('id' => 'exp_sp_administration_module_info_share_form',
                              'title' => t('Share'))
            );
            exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
        }
        else {
            throw new Exception('exp_sp_administration_share_display_addedit_form');
        }
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_share_display_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}
/*
 * getShareModuleStatus() Get the Drupal module status
*/

function share_changeStatus($notifyStat, $shareModStat='', $profStat=''){
       try{
                        //now update the share notification to enable
                        $updNotfyStatus = db_update('slt_notification_info');
                        $updNotfyStatus->fields(array('status' => $notifyStat));
                        $updNotfyStatus->condition('notification_code',array('share_notification_class_tp','share_notification_catalog'),'IN');
                        $updNotfyStatus->execute();
                       
                       if(!empty($profStat)){        
                        $updStatus = db_update('slt_profile_list_items');
                        $updStatus->fields(array('is_active' => $profStat));
                        $updStatus->condition('code','cre_ste_mod_shr','=');
                        $updStatus->execute();
                        }                         
                        //To disable the email & embed flag when module is disabled
                        if(!empty($shareModStat)){
                        	variable_set('share_email',0);
                            variable_set('share_embed',0);
                        if($shareModStat != '1'){
                                variable_set('share_email',1);
                                variable_set('share_embed',1);
                        }
                        }
                        return;
       }catch(Exception $ex){
               watchdog_exception('changeStatus', $ex);
       }
}
function exp_sp_administration_module_info_share_form($form, &$form_state){
    try{
        global $theme_key;
        $wrapperId = 'share-basic-addedit-form';
        $form = newAddEditFormWithCancelAndSave($wrapperId);
        
        //get the existng value for email and embed to see their status
        $getShareVal = getShareModuleStatus();
        $emailChk = $getShareVal['share_email'];
        $embedChk = $getShareVal['share_embed']; 
   		$groupList[0] = (object) array("id"=>"1","name"=>t('E-MAIL'), "enabled" => "$emailChk","data"=>'Email');
        $groupList[1] = (object) array("id"=>"2","name"=>t('EMBED'), "enabled" => "$embedChk","data"=>'Embed');
        $grplistCnt = count($groupList);
        $i=0;
        foreach($groupList as $key => $val) {
            $uniqueid = $val->id;
            if($val->enabled==1){
                $i++;
            }           
        }   
        $checkedallStatus = ($i==$grplistCnt) ? 'checked' : '';
        $checkedall = ($i==$grplistCnt) ? 'checkbox-selected' : 'checkbox-unselected';
        if($theme_key == 'expertusoneV2'){
             $style = 'padding-left: 15px';
         }else{
             $style = 'padding-left: 0';
         }
		 //added style="margin-top for ticket # 75917
         $form[$formName]['share_fieldset'] = array(
          		'#type' => 'markup',
               	'#prefix' => '<table width="94%" class="share-table-heading" cellpadding="4" cellspacing="0" border="0">                                          
                 <th width="90%" style="'.$style.'" class="share-title-heading share-header heading-background-class-title">'.t('LBL3086').'</th>      
                 <th width="10%" style="padding-left: 0px; text-align:center;" class="share-title-heading share-header heading-background-class">
                 <div class="shr-muliselect '.$checkedall.'" style="margin-left: 4px; margin-top:0px;">
                 <input type="checkbox" id="shr_select" class="multiselect-selectall" onclick="sharecheckedall(this);" value="" '.$checkedallStatus.' /></div></th>',
                 '#suffix' => '',
                                             ); 
                $i=0;
                $j=0;
                foreach($groupList as $key => $val){
                $uniqueid = $val->name;
                expDebug::dPrint('Validate $form_state[values] = ' . print_r($form_state['values'], true), 5);
                expDebug::dPrint('$groupList --> ' . print_r($groupList, true),5);
                expDebug::dPrint('$uniqueid --> ' . $uniqueid,5);
       
              $checkedStatus = ($val->enabled==1) ? 'checked' : '';
              $checkedClass =  ($val->enabled==1) ? 'checkbox-selected' : 'checkbox-unselected';
              $class='even-list-class';
              if($j==1){
                 		 $class='odd-list-class';
                 		 $j=0;
              }else{
                  	$j=1;
              }
			  //added style="margin for ticket # 75917
              $form[$formName]['share_table_row'.$i] = array(
                '#type' => 'markup',
                '#prefix' => '<tr class="'.$class.'"><td class="check-attributes">
                <span class="vtip" title="'.sanitize_data($val->data).'"> '.titleController('admin-config-share-check-attributes-title',t($val->name),45).'</span></td>
                <td class="Checked-column"><div class="multichk '.$checkedClass.'" style="margin: 3px 0 0;"><input type="checkbox"  id = "add_grp_'.$val->data.'" class = "attach-group-list"
                 onclick="checkSelectedUnselectedCommon(this);" name="attach_attributes[]" value="'.$val->data.'" '.$checkedStatus.'/>',
                '#suffix' => '</div></td></tr>',
              );
             $i++;
            }
            $form[$formName]['share_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table>',
     ); 
       
        addEditFormWithShareCancelAndSaveClose($form, $wrapperId);
        return $form;
    }
    catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_module_info_share_form', $ex);
        expertusErrorThrow($ex);
    }
}
/**
 * Adds cancel and save button to exp_sp_administration_module_info_share_form
 */
function addEditFormWithShareCancelAndSaveClose(&$form, $wrapperId,&$form_state) {
    try {
        // Button ids
        $saveButtonId = 'share-save-button';
        $cancelButtonId = 'share-cancel-button';
        $esignHiddenButt = 'esign-' . $wrapperId;
        global $theme_key;
        $prefixBgLeft='';$suffixBgRight='';
        if($theme_key == 'expertusoneV2'){
            $prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
            $suffixBgRight = '<div class="white-btn-bg-right"></div>';
        }
        // Add the Cancel button
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
        // Using this class to applying bg color to the save button
        $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $cancelAttributes['data-wrapperid'] = array($wrapperId);
        // A common class for all addedit form cancel button, for styling to appear as a link.
        $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
        $cancelAttributes['onclick'][] = 'Drupal.CTools.Modal.dismiss(); return false;';
        //$cancelAttributes['tabindex'][] = $tabIndex + 1;
        $form[$cancelButtonId] = array(
                '#type' => 'button',
                '#prefix' => '<div style="clear:both;"></div>' .
                '<div class="addedit-form-cancel-and-save-actions-row">' .
                '<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
                // Suffix to close the two divs is added in #suffix property of Save button
                '#suffix' => $suffixBgRight,
                '#value' => t('LBL123'), //Close
                '#name' => 'cancel',
                '#attributes' => $cancelAttributes
        );
        // Add the Save button
        $ajaxCallbackSave = 'addEditRepaintForm';
        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);
        // Using this class to applying bg color to the save button
        $saveAttributes['class'][] = 'admin-save-button-middle-bg';
        //$saveAttributes['tabindex'][] = $tabIndex;
        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid'] = array($wrapperId);
        $eSignatureAvailable = true;
        if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
            $form[$saveButtonId] = array(
                    '#type' => 'submit',
                    '#value' => t('LBL141'), // Save
                    '#name' => 'save',
                    '#prefix' => '<div class="admin-save-button-container">' .
                    '<div class="admin-save-button-left-bg"></div>',
                    '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                    '</div>' .
                    '<div class="clearBoth"></div>' .
                    '</div>' . // see #prefix of close button above
                    '<div class="clearBoth"></div>' .
                    '</div>', // see #prefix of close button above
                    '#id' => $saveButtonId,
                    '#attributes' => $saveAttributes,
                    '#ajax' => array(
                            'wrapper' => $wrapperId,
                            'callback' => $ajaxCallbackSave,
                            'method' => 'replace',
                            'effect' => 'none', // 'fade',
                            'event'=> 'click',
                            'drupalUserId' => 'drupal_uid',
                            'customButtonId'=> $esignHiddenButt,
                            'keypress' => true,
                            'progress' => array(
                                    'type' => 'throbber',
                                    'message' => ''
                            ),
                    )
            );
        }
        else {
            $saveAttributes['onclick'] = array();
            $saveAttributes['onclick'][] = '$("#' . $esignHiddenButt . '").click(); return false;';
            $form[$saveButtonId] = array(
                    '#type' => 'button',
                    '#prefix' => '<div class="admin-save-button-container">' .
                    '<div class="admin-save-button-left-bg"></div>',
                    '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
                    '</div>' .
                    '<div class="clearBoth"></div>' .
                    '</div>' . // see #prefix of close button above
                    '<div class="clearBoth"></div>' .
                    '</div>', // see #prefix of close button above
                    '#value' => t('LBL141'), // Save
                    '#name' => 'save',
                    '#attributes' => $saveAttributes,
            );
        }
        $saveAttributes['onclick'] = array();
        $form[$esignHiddenButt] = array(
                '#type' => 'submit',
                '#value' => t('hidden-save'),
                '#name' => 'hidden-save',
                '#prefix' => '<span id = "esign-admin-div" style="display:none">',
                '#suffix' => '</span>',
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
    }
    catch (Exception $ex) {
        watchdog_exception('addEditFormWithShareCancelAndSaveClose', $ex);
        expertusErrorThrow($ex);
    }
}
/**
 * Default validate handler for exp_sp_administration_module_info_share_form_validate
 */
function exp_sp_administration_module_info_share_form_validate($form, &$form_state){
    try {
        expDebug::dPrint('Validate $form_state[values] = ' . print_r($form_state['values'], true), 5);
        expDebug::dPrint('$form_state[triggering_element] = ' . print_r($form_state['input'], true), 5);
        $form_state['hidden-Save-submit'] = false;
        $form_state['esign_show_popup']= false;
        $form_state['storage']= true;
        if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
            $validationTrigger  = 'save';
        }
        else{
            $validationTrigger  = 'hidden-Save';
        }
        if ($form_state['triggering_element']['#name'] == 'hidden-Save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-Save') {
            $form_state['hidden-Save-submit'] = true;
        }
        $form_state['esign_validation_status'] = false;
   
        if (!($form_state['hidden-Save-submit'])) {
            unset($_SESSION['esign_validate']);
        }
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_share_addedit_form_validate', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * Submit handler for exp_sp_administration_module_info_share_form_submit
 */
function exp_sp_administration_module_info_share_form_submit($form, &$form_state){
    try{
        $getcheckedattr  = $form_state['input']['attach_attributes'];
	    $attrcode = implode(',',$form_state['input']['attach_attributes']);
		$getShareVal = getShareModuleStatus();
        $preshareemail = $getShareVal['share_email'];
        $preshareembed = $getShareVal['share_embed'];
		
		$prefields[0] = new stdClass();
		$prefields[0]->share_email = $preshareemail;
		$prefields[0]->share_embed = $preshareembed;
		
	       	  variable_set('share_email',0);
              variable_set('share_embed',0);
            if(in_array('Email',$getcheckedattr)){
                variable_set('share_email',1);
            }
            if(in_array('Embed',$getcheckedattr)){
                variable_set('share_embed',1);
            }
  			$userId = getIdOfLoggedInUser();			
  			$shareemail = variable_get('share_email',0);
		    $shareembed = variable_get('share_embed',0);
			$fields = array('share_email' => $shareemail, 'share_embed' => $shareembed);
			$entityId = db_select('slt_profile_list_items','spli');
		    $entityId->addField('spli','id');
		    $entityId->condition('spli.code','cre_ste_mod_shr','=');
		    $entityId = $entityId->execute()->fetchField();			
 			make_audit_trail_entries($fields, $userId, $prefields ,$entityId, 'Settings Updated','cre_ste_mod_shr','exp_sp_administration_sitesetup','shareModule Configuration',
		    array('share_email', 'share_embed'));
		  	$sharemodule=getShareModuleStatus();
            $status=$sharemodule['status'];
            $email=$sharemodule['share_email'];
            $embed=$sharemodule['share_embed'];
			if($status == 1 && $email == 0 && $embed == 0){
            	share_changeStatus('cre_ntn_sts_itv','','N');
            }else if($status == 1 && (($email == 0 && $embed == 0) || ($email == 0 && $embed == 1))){
                share_changeStatus('cre_ntn_sts_itv');
            }      
            else{
                share_changeStatus('cre_ntn_sts_atv');
	
    } 
      expertus_set_message(t('MSG689'));
      $form_state['rebuild'] = TRUE;

    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_share_addedit_form_submit', $ex);
        expertusErrorThrow($ex);
    }
}
?>