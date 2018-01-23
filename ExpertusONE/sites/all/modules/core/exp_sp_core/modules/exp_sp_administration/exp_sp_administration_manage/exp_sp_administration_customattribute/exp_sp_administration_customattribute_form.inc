<?php

/* Form Creation Start */

function exp_sp_administration_customattribute_addedit_form_validate($form, &$form_state){
    try {
        global $secure_entities;
        global $user;
        $form_state['hidden-save-submit'] = false;
        $form_state['esign_show_popup']= false;

        expDebug::dPrint(' exp_sp_administration_customattribute_addedit_form_validate triggering_element:' . print_r($form_state['triggering_element'],true), 5);
        expDebug::dPrint(' form_State:' . print_r($form_state, true) , 5);
        expDebug::dPrint(' form_State:' . print_r($form_state['values'], true) , 5); 
         
        //if(strrpos($form_state['triggering_element']['#name'],'screen')){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_screen.inc');
        //} 

        if (!empty($form_state['storage']['new_entity'])) {
            unset($form_state['storage']['new_entity']); // Unset the variable to remove it from $form_state
            unset($form_state['storage']['form_info_list']); // No more required to be present in $form_state
        }

        if (!empty($form_state['storage']['edited_entity'])) {
            unset($form_state['storage']['edited_entity']); // Unset the variable to remove it from $form_state
        }

        if ($form_state['triggering_element']['#name'] == t('cancelcustomattribute')) {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            unset($form_state['custom_fields']); // Clear any change made to custom fields
            $form_state['rebuild']   = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['step'] = 'view';
            return;
        }
        
        
        if ($form_state['triggering_element']['#name'] == 'attribute') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['substep'] = 'attribute';
            $form_state['storage']['step'] = 'edit';
            return;
        }

        if ($form_state['triggering_element']['#name'] == 'screen') {
                    $form_state['display_class']   = TRUE;
                    $form_state['input']     = array(); // Clear user input - raw
                    $form_state['values']    = array(); // Clear user input - security cleared
                    $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
                    $form_state['storage']['substep'] = 'screen';
                    /*unset($form_state['delete_prerequisite_index']);
                    unset($form_state['delete_prerequisite_fieldset']);
                    $form_state['num_prerequisite'] = 0;*/
                    return;
        }

        if ($form_state['triggering_element']['#name'] == 'Edit Custom Attribute') {
            $form_state['display_class']   = TRUE;
            $form_state['input']     = array(); // Clear user input - raw
            $form_state['values']    = array(); // Clear user input - security cleared
            $form_state['rebuild']         = TRUE; // Rebuild the form. Submit handler will not be called.
            $form_state['storage']['step'] = 'edit';
            return;
        } 

        if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
            $triggerValue  = 'save';
        }else{
            $triggerValue  = 'hidden-save';
        }
        
        $triggerElementByName  = $form_state['triggering_element']['#name'];

        expDebug::dPrint('check prerequisite list trigger element :'.$form_state['triggering_element']['#name'], 5);
        expDebug::dPrint('check prerequisite list storage substep :'. $form_state['storage']['substage'],5);

        if (($form_state['triggering_element']['#name'] == 'save' || $form_state['triggering_element']['#name'] == 'saveandpublish')) { 
            
                        //if ($form_state['triggering_element']['#value'] == $triggerValue) {
                        $form_state['esign_show_popup']= true;//By default esign popup will open,
                        //if any validation error messages throws, it will set as false
                        
                        
                        // Check all mandatory fields exist 
                           
                               addEditCheckMandatoryFields($form, $form_state, array('cattr_name'));
                         
                         //check that max 30'' characters in Custom attribte name
                                
                                if(!empty($form_state['values']['cattr_name']) && trim(strlen($form_state['values']['cattr_name']))>30){                
                                    form_set_error('cattr_name', t('Label'). ' '.  t('ERR350',array('@chars'=>'30')));
                                    $form_state["storage"]["form_validate"] = false;
                                    $form_state['esign_show_popup']= false;
                                }
                             
                             /* /^[a-zA-Z0-9-]+$/u */
                               //Allow only the Alphanumneric with_
                                if(is_numeric(substr($form_state['values']['cattr_name'],0,1))){
                                	form_set_error('cattr_name', t('Label').' '.t('ERR358').'.');
                                	$form_state["storage"]["form_validate"] = false;
                                	$form_state['esign_show_popup']= false;
                                }
                                
                                if(!empty($form_state['values']['cattr_name']) && (!preg_match('/^[A-Za-z0-9 ]+$/', $form_state['values']['cattr_name']))){
                                    //form_set_error('cattr_name',t('Label'). ' '. t('ERR351'));
                                    form_set_error('cattr_name', t('MSG811',array('@fieldname'=>t("Label"),'@character list'=>'a-z A-Z 0-9 ')));
                                    //@fieldname accepts only the following characters: @character list
                                    $form_state["storage"]["form_validate"] = false;
                                    $form_state['esign_show_popup']= false;
                                } 
                    
                                 //Check that whether custom attribute name already exists of not
                                 if(!empty($form_state['values']['cattr_name'])){
                                     $mapped_cus_attr_id=$form_state['values']['mapped_cus_attr_id']; 
                                     $ret_value=checkDuplicateAttributeName($form_state['values']['cattr_name'],$mapped_cus_attr_id);
                                     if($ret_value==1){ //if duplicate exists 
                                     //  form_set_error('cattr_name',t('ERR354'));
                                      // form_set_error('cattr_name',t('ERR354'));
                                        form_set_error('cattr_name', t('ERR354',array('@lbl_name'=>$form_state['values']['cattr_name'])));
                                     // form_set_error('cattr_name', t('LBL2015'). ' '.$form_state['values']['cattr_name'].' '. t('ERR354'));
                                      
                                        $form_state["storage"]["form_validate"] = false;
                                        $form_state['esign_show_popup']= false;
                                     }  
                                 } 
                                
                                
                        
                              if($form_state['values']['cattr_type']=='cattr_type_txtbox' || $form_state['values']['cattr_type']=='cattr_type_txtarea'){ 
                                         
                                        // Check Length
                                         addEditCheckMandatoryFields($form, $form_state, array('cattr_length')); 
                                        
                                        
                                        // Allow only numeric value 
                                        if(!is_numeric($form_state['values']['cattr_length']) && !empty($form_state['values']['cattr_length']) || ($form_state['values']['cattr_length']) < 1 || preg_match ("/[^0-9]/", $form_state['values']['cattr_length'])){
                                            form_set_error('cattr_length', t('ERR097').' '.t('LBL2001').'.');
                                            $form_state["storage"]["form_validate"] = false;
                                            $form_state['esign_show_popup']= false;
                                        }else{
                                            
                                            if($form_state['values']['cattr_type']=='cattr_type_txtbox'){
                                                     //Integer for 11
                                                    if(trim($form_state['values']['cattr_datatype'])=='cattr_dtype_numbers' && $form_state['values']['cattr_length']>11){
                                                         form_set_error('cattr_length', t('LBL2001'). ' '.  t('ERR350',array('@chars'=>'11')));
                                                         $form_state["storage"]["form_validate"] = false;
                                                         $form_state['esign_show_popup']= false;
                                                    }
                                                    //TextBox for 255
                                                    if(trim($form_state['values']['cattr_datatype'])=='cattr_dtype_text' && $form_state['values']['cattr_length']>255){
                                                         form_set_error('cattr_length', t('LBL2001'). ' '.  t('ERR350',array('@chars'=>'255')));
                                                         $form_state["storage"]["form_validate"] = false;
                                                         $form_state['esign_show_popup']= false;
                                                    }
                                                
                                            }else if($form_state['values']['cattr_type']=='cattr_type_txtarea'){
                                                    
                                                    //Textarea for 500
                                                    if($form_state['values']['cattr_length']>500){
                                                         form_set_error('cattr_length', t('LBL2001'). ' '.  t('ERR350',array('@chars'=>'500')));
                                                         $form_state["storage"]["form_validate"] = false;
                                                         $form_state['esign_show_popup']= false;
                                                    }
                                                
                                            }
                                            
                                             
                                            
                                        } 
                                        
                            } 
            
                           // For Checkbox and radio 
                       
                           if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_dropdown' || $form_state['values']['cattr_type']=='cattr_type_radio'){
                                 
                                $emptyChoiceCount=0;
                                $special_char=0;
                                
                                $dup_arr=array();
                                
                                //Store into the storage
                                $form_state['storage']['customoptions']['count']=$form_state['values']['customoptions']['count'];
                                 
                                for ($i = 1; $i <= $form_state['values']['customoptions']['count']; $i++) { 
                                	
                                	$form_state['values']['customoptions']['choices']['choice-text-' . $i] = trim($form_state['values']['customoptions']['choices']['choice-text-' . $i]);                               	 
                                    
                                    //store the label value
                                    $form_state['storage']['customoptions']['options'][$i]=$form_state['values']['customoptions']['choices']['choice-text-' . $i];
                                    
                                    expDebug::dPrint('Storage value for Custom Options='.print_r($form_state['storage']['customoptions']['options'],true),5);
                                    
                                    if (($form_state['values']['customoptions']['choices']['choice-text-' . $i]) == null || empty($form_state['values']['customoptions']['choices']['choice-text-' . $i]) || ($form_state['values']['customoptions']['choices']['choice-text-' . $i]) == '') {
                                          form_set_error('customoptions][choices][choice-text-' . $i, ' '); 
                                          $form_state['esign_show_popup'] = false;
                                          $emptyChoiceCount++;
                                    }
                                        
                                    if($form_state['values']['customoptions']['choices']['choice-text-' . $i]!=''){  //if value for the options exists
                                         if(!preg_match('/^[A-Za-z0-9_ .?@#&\-+$><\/%*]+$/', $form_state['values']['customoptions']['choices']['choice-text-' . $i])){ 
                                                form_set_error('customoptions][choices][choice-text-' . $i, ' '); 
                                                $form_state['esign_show_popup'] = false;
                                                $special_char++; 
                                          }
                                          //store in duplicate array
                                          if(in_array($form_state['values']['customoptions']['choices']['choice-text-' . $i],$dup_arr)){
                                                form_set_error('customoptions][choices][choice-text-' . $i, ' '); 
                                                $form_state['esign_show_popup'] = false;
                                                $dupCount++;
                                          }
                                          $dup_arr[]=$form_state['values']['customoptions']['choices']['choice-text-' . $i]; 
                                    }
            
                                  } // end for loop
                                  
                                  //To check duplicate options
                                  
                                  $err_str='';
                                  if($dupCount>0){
                                    form_set_error('1', t('LBL387') . ' ' . t('ERR355'));
                                  }
                        
                                  if ($emptyChoiceCount == 1) { 
                                     form_set_error('2', t('LBL387') . t('ERR101'));
                                  }else if ($emptyChoiceCount > 1) {
                                       form_set_error('2', t('LBL3097') . t('ERR169'));
                                  }          
            
                                  // To check special characters                      
                                  if($special_char==1){
                                      //form_set_error('3', t('LBL387') . ' ' . t('ERR351'));
                                      form_set_error('3', t('MSG811',array('@fieldname'=>t("LBL387"),'@character list'=>'A-Z a-z 0-9 _ .?@#&-+></%*')));
                                       
                                  }else if($special_char>1){
                                      //form_set_error('3', t('LBL3097') . ' ' . t('ERR351')); 
                                      form_set_error('3', t('MSG811',array('@fieldname'=>t("LBL3097"),'@character list'=>'A-Z a-z 0-9 _ .?@#&-+></%*')));
                                  }  
                            } 
            
                            // For help Text
                            if(!empty($form_state['values']['cattr_help_txt']) && trim(strlen($form_state['values']['cattr_help_txt']))>255){
                                form_set_error('cattr_help_txt', t('LBL2003'). ' '.  t('ERR350',array('@chars'=>'255')));
                                $form_state["storage"]["form_validate"] = false;
                                $form_state['esign_show_popup']= false;
                            } 
                
                           if(!empty($form_state['values']['cattr_help_txt']) && (!preg_match('/^[A-Za-z0-9_ .-]+$/', $form_state['values']['cattr_help_txt']))){
                                //form_set_error('cattr_help_txt',t('LBL2003'). ' '. t('ERR351'));
                                form_set_error('cattr_help_txt', t('MSG811',array('@fieldname'=>t("LBL2003"),'@character list'=>'A-Z a-z 0-9 _.-')));
                                $form_state["storage"]["form_validate"] = false;
                                $form_state['esign_show_popup']= false;
                            }  

                            //Check that whether custom attribute is creating through shortcut. IF create through shortcut , applicable below codes
                            expDebug::dPrint('from_shortcut=='.$form_state['storage']['from_shortcut'],5); 
                            if($form_state['storage']['from_shortcut'] == '1'){
                                      $attribute_limit=10; //Set 10
                                      $ent_attr_map_code=$form_state['storage']['shortcut_entity_code'];
                                       $mapped_cus_attr_id=$form_state['values']['mapped_cus_attr_id'];
                                      $tot_fld_alloted=getTotalAllotedCustomFields($mapped_cus_attr_id,$ent_attr_map_code);  
                                      expDebug::dPrint('$tot_fld_alloted=='.$tot_fld_alloted,5);  
                                      if($tot_fld_alloted>=$attribute_limit){
                                            $not_allow=1; 
                                            $ent_attr_map_lbl=getEntityOptions($ent_attr_map_code);
                                            $err_str=$ent_attr_map_lbl;
                                      }
                                      
                                      if($not_allow>0){  
                                            form_set_error('cattr_name', t('ERR353',array('@count'=>$attribute_limit)) . ' ' . $err_str);
                                             $screen_error='1';
                                             $form_state["storage"]["form_validate"] = false;
                                             $form_state['esign_show_popup']= false;  
                                             $form_state['values']['error_in_screen']=true; 
                                      } 
                                      
                            } 
                            
            

        }else if($triggerElementByName == 'esign-screen' || $triggerElementByName == 'set-screen'){ //For Screen
                            $form_state['esign_show_popup']= true;
                            $form_state['storage']['mode'] = 'edit';
                            $screen_error='';
                            
                            $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
                            $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
                            $entityPlugin = $form_state['triggering_element']['#ajax']['entity_plugin'];
                
                            if (in_array($entityType, $secure_entities)) {
                                $uniqueId = "screen-".core_encrypt($entityId)."-$entityType"; // URL encryption data to retrive
                            } else {
                                $uniqueId = "screen-".$entityId."-$entityType"; // URL encryption data to retrive
                            }
                            $form_state['hidden_idlist_'.$uniqueId] = $form_state['input']['hidden_idlist_'.$uniqueId];
                            
                             if(isset($entityPlugin) && $entityPlugin=='screen') {  
                                
                                //Check for all entity by giving input
                                $mapped_cus_attr_id=$form_state['values']['mapped_cus_attr_id']; 
                                $attribute_limit=10; //Set 10
                                
                                $tmp_ent_view_opt_arr=$form_state['input']['ent_view_opt'];
                                $tmp_ent_edit_opt_arr=$form_state['input']['ent_edit_opt'];
                                
                                if(!isset($tmp_ent_view_opt_arr)){
                                    $tmp_ent_view_opt_arr=array();
                                }
                                if(!isset($tmp_ent_edit_opt_arr)){
                                    $tmp_ent_edit_opt_arr=array();
                                }
                                $tmp_ent_unique_arr=array_unique(array_merge($tmp_ent_view_opt_arr,$tmp_ent_edit_opt_arr), SORT_REGULAR);
                                $tot_ent_map_count=count($tmp_ent_unique_arr);
                                
                                expDebug::dPrint(' $tot_ent_map_count = '.print_r($tot_ent_map_count,true), 5);
                                expDebug::dPrint(' $tmp_ent_view_opt_arr = '.print_r($tmp_ent_view_opt_arr,true), 5);
                                expDebug::dPrint(' $tmp_ent_edit_opt_arr = '.print_r($tmp_ent_edit_opt_arr,true), 5);
                                expDebug::dPrint(' $tmp_ent_unique_arr = '.print_r($tmp_ent_unique_arr,true), 5);
                                
                                $not_allow=0;   
                                
                                $sel_screen_entities_arr=array(); 
                                
                                if(count($tot_ent_map_count)>0){
                                    
                                    $tmp_attribute_limit=$attribute_limit-1;
                                    foreach($tmp_ent_unique_arr as $ent_key => $ent_val){  
                                        $ent_attr_map_code=trim($ent_val);
                                        
                                        $sel_screen_entities_arr[]=$ent_attr_map_code;
                                        
                                        $tot_fld_alloted=getTotalAllotedCustomFields($mapped_cus_attr_id,$ent_attr_map_code); 
                                        if($tot_fld_alloted>$tmp_attribute_limit){
                                            $not_allow++; 
                                            $ent_attr_map_lbl=getEntityOptions($ent_attr_map_code);
                                            $err_str.=','.$ent_attr_map_lbl;
                                        } 
                                        
                                    }
                                    $err_str=ltrim($err_str,","); 
                                } 
                                
                                expDebug::dPrint(' $sel_screen_entities_arr = '.print_r($sel_screen_entities_arr,true), 5); 
                                
                                if($not_allow>0){ 
                                    //form_set_error('cattr_name', $err_str. ' '.  t('ERR353',array('@count'=>$attribute_limit)));
                                    form_set_error('cattr_name', t('ERR353',array('@count'=>$attribute_limit)) . ' ' . $err_str);
                                     $screen_error='1';
                                     $form_state["storage"]["form_validate"] = false;
                                     $form_state['esign_show_popup']= false;  
                                     $form_state['values']['error_in_screen']=true;
                                     
                                }
                            }  
                        
                           if($screen_error!='1'){     //Check that whether to show the alert popup or not if unmapping the entities from Screen page
                                            
                                            $disp_alert=0;
                                            
                                            $existing_map_entities=trim($form_state['values']['existing_map_entities']);
                                            expDebug::dPrint(' $existing_map_entities = '.print_r($existing_map_entities,true), 5);
                                            
                                            if($existing_map_entities!=''){
                                                
                                                   if(empty($sel_screen_entities_arr)){ //if unselect all entities from screen page
                                                      $disp_alert=1;
                                                   }else{  //if selected entity by user
                                                       $existing_map_entities_arr=explode(',',$existing_map_entities);
                                                       foreach($existing_map_entities_arr as $exist_key => $exist_ent_code){
                                                           if(!in_array($exist_ent_code,$sel_screen_entities_arr)){
                                                              $disp_alert++;
                                                           }
                                                       }
                                                   } 
                                            }  
                                            
                                            expDebug::dPrint(' $disp_alert = '.print_r($disp_alert,true), 5);
                                           
                                            if($disp_alert>0){ //if changes is there and un-map the existing one from Screen page then show alert
                                            
                                               //$attrDetails   = getCustomAttributeDetail($mapped_cus_attr_id, array('cattr_name'));
                                               $attrDetails   = getCustomAttributeDetail($mapped_cus_attr_id);
                                               $cattr_name    =  $attrDetails[0]->cattr_name;
                                                      
                                                $commands = array();
                                                $commands[] =  array('command' => 'displayScreenWarningWizard',    
                                                                'name'    => t('MSG1105',array('@cust_attr_name'=>$cattr_name)));  
                                                print ajax_render($commands);
                                                exit; 
                                            }  
                            }           
        } 
        
    //  if ($triggerElementByName == 'hidden-save' || $triggerElementByName == 'set-prerequisite' || $triggerElementByName == 'set-equivalence') {
        if ($triggerElementByName == 'hidden-save' || $triggerElementByName == 'set-screen') {
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
        watchdog_exception('exp_sp_administration_customattribute_addedit_form_validate', $ex);
        expertusErrorThrow($ex);
    }
} 

function getCustomAttributeCode($cattr_name,$concate_str=''){
    try{
        
        $cattr_name_code='';
        if($cattr_name!=''){
            $cattr_name_arr=explode(' ',$cattr_name);  
            $cattr_name_arr=array_filter(array_map('trim',$cattr_name_arr)); 
            expDebug::dPrint("cattr_name_arr : ".print_r($cattr_name_arr,true), 5);
            
            $cattr_name_code=implode('_',$cattr_name_arr);
            if($concate_str!='') {
                $cattr_name_code=$concate_str."_".$cattr_name_code;
            }  
            
            $cattr_name_code=strtolower($cattr_name_code);
        }
        expDebug::dPrint("cattr_name_code : ".print_r($cattr_name_code,true), 5); 
        return $cattr_name_code;
    }catch (Exception $ex) {
        watchdog_exception('getCustomAttributeCode', $ex);
        expertusErrorThrow($ex);
    }
}

function getSequenceNumber($require_for='',$attr_id=''){
        try{
            expDebug::dPrint('$require_for ='.print_r($require_for,true),5);
            
            if($require_for=='option'){
                $max_seq_no = db_query("select max(opt_seq) as max_seq_no from slt_custom_attr_options where cattr_id='$attr_id';")->fetchField(); 
            }else{
               $max_seq_no = db_query("select max(cattr_seq) as max_seq_no from slt_custom_attr;")->fetchField();    
            } 
            expDebug::dPrint('$max_seq_no ='.print_r($max_seq_no,true),5);
            return $max_seq_no;
        }catch (Exception $ex) {
            watchdog_exception('getSequenceNumber', $ex);
            expertusErrorThrow($ex);
        }
}

function customAttributeFormatInputString($form, &$form_state,$createUserId,$db_action,$default = null) {
    try {
         
         //$inactivereasontext    = empty($form_state['values']['status_inactive_reason']) ? NULL : $form_state['values']['status_inactive_reason'];
         $in_active_on          = $form_state['values']['cattr_status'] == 'lrn_crs_sts_itv' ? now() : NULL; 
         
         //Check whether mandatory or not
          $mand_opt=0;
         if ($form_state['values']['cattr_mandatory'] == 'Y'){
            $mand_opt=1;
         } 
         
         //Check whether Unique or not
         $unique_opt=''; 
         if($form_state['values']['cattr_type']=='cattr_type_txtbox'){
             if($form_state['values']['cattr_unique'] == 'Y'){
                 $unique_opt=1;
             }else if($form_state['values']['cattr_unique'] == 'N'){
               $unique_opt=0;   
             } 
         }  
         
         //Get Active Status or not
         $trigger_element = $form_state['storage']['submit_triggering_element'];
         if($trigger_element == 'saveandpublish'){
            $cattr_status = 'cre_cattr_sts_atv ';
         }else if($trigger_element == 'save'){
            $cattr_status = 'cre_cattr_sts_itv ';
        }else if($form_state['values']['cattr_status']){
            $cattr_status =$form_state['values']['cattr_status'];
        }
         
         //If come through shot-cut , then active the status directly
         if($form_state['storage']['from_shortcut'] == '1'){
              $cattr_status = 'cre_cattr_sts_atv ';
         }
        
        // Get Sequence Number
        $max_seq_no=getSequenceNumber();
        if($max_seq_no!=''){
            $seq_no=$max_seq_no+1;  
        }else{
            $seq_no=1;
        }
        
        //Generate dynaic code according to attribute name 
        $cattr_code=getCustomAttributeCode($form_state['values']['cattr_name'],'c');
        
        
        $fields = array(
                  'cattr_name'              => $form_state['values']['cattr_name'],
                  'cattr_code'              => $cattr_code, 
                  'cattr_type'              => $form_state['values']['cattr_type'], 
                  'cattr_mandatory'         =>  $mand_opt,
                  'cattr_help_txt'          =>  $form_state['values']['cattr_help_txt'],
                  'cattr_seq'               =>  $seq_no,
                  'status'                  =>  $cattr_status 
        );
        
        
        //Dynamic translation - start
        $config=getConfig('exp_sp');
        $oldValArr = array();
        $user = "";
        if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
        {    
        include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
        expDebug::dPrint("user det: user::".$createUserId,5);
        $user = getDrupalUserDataForTranslation($createUserId);
        expDebug::dPrint("user det:::".print_r($user,true),5);
        
        }
        
        
        if($db_action == "update"){
            expDebug::dPrint('test1...'.$default[0]->cattr_name,5);
            expDebug::dPrint('tes2t...'.$default[0]->cattr_help_txt,5);
            
            if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
            {
                
                $oldValArr[0] = $default[0]->cattr_name;
                $oldValArr[1] = $default[0]->cattr_help_txt;
                
                $valuesForTranslation = array();
                $valuesForTranslation[0]=$form_state['values']['cattr_name'];
                $valuesForTranslation[1]=$form_state['values']['cattr_help_txt'];
                
                $fieldNames = array();
                $fieldNames[0]="cattr_name";
                $fieldNames[1]="cattr_help_txt"; 
                $recordCreatedLang =  updateContentLabels($oldValArr,$valuesForTranslation,$user->language,"cre_sys_obt_cattr",array($form_state['values']['cattr_id'],$form_state['values']['cattr_id']),$fieldNames);
               // if($recordCreatedLang)
                {
                    
                $fields = array(
                    'cattr_name'              => $form_state['values']['cattr_name'],
                    'cattr_code'              => $cattr_code,
                    'cattr_type'              => $form_state['values']['cattr_type'],
                    'cattr_mandatory'         =>  $mand_opt,
                    'cattr_help_txt'          =>  $form_state['values']['cattr_help_txt'],
                    'cattr_seq'               =>  $seq_no,
                    'status'                  =>  $cattr_status,
                    'recordCreatedLang'       => $recordCreatedLang
                );
                }
               /* else{
                    $fields = array(
                      //  'cattr_name'              => $form_state['values']['cattr_name'],
                      //  'cattr_code'              => $cattr_code,
                        'cattr_type'              => $form_state['values']['cattr_type'],
                        'cattr_mandatory'         =>  $mand_opt,
                   //     'cattr_help_txt'          =>  $form_state['values']['cattr_help_txt'],
                        'cattr_seq'               =>  $seq_no,
                        'status'                  =>  $cattr_status
                    );
                }*/
           
            
          
            
            }else
            {
                $fields = array(
                      'cattr_name'              => $form_state['values']['cattr_name'],
                       'cattr_code'              => $cattr_code,
                    'cattr_type'              => $form_state['values']['cattr_type'],
                    'cattr_mandatory'         =>  $mand_opt,
                       'cattr_help_txt'          =>  $form_state['values']['cattr_help_txt'],
                    'cattr_seq'               =>  $seq_no,
                    'status'                  =>  $cattr_status
                );
                
            }
            
        }else if($db_action == "insert"){
            
            $fields = array(
                'cattr_name'              => $form_state['values']['cattr_name'],
                'cattr_code'              => $cattr_code,
                'cattr_type'              => $form_state['values']['cattr_type'],
                'cattr_mandatory'         =>  $mand_opt,
                'cattr_help_txt'          =>  $form_state['values']['cattr_help_txt'],
                'cattr_seq'               =>  $seq_no,
                'status'                  =>  $cattr_status
            );
            
            
        }
        
         if($form_state['values']['cattr_type']=='cattr_type_txtbox'){  //If textbox
              $fields['cattr_datatype']=$form_state['values']['cattr_datatype'];
              $fields['cattr_length']=$form_state['values']['cattr_length'];
              $fields['cattr_unique']=$unique_opt; 
         }else if($form_state['values']['cattr_type']=='cattr_type_txtarea'){  //If textarea 
              $fields['cattr_length']=$form_state['values']['cattr_length']; 
         }else if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_radio'){  //If Checkbox  or radio
			  $fields['cattr_display_opt']=$form_state['values']['cattr_display_opt']; 
			
         }else if($form_state['values']['cattr_type']=='cattr_type_dropdown'){  //If dropdown
         
             //Check whether multiple or not
             $multiple_opt='';
             if($form_state['values']['cattr_multiple_opt'] == 'Y'){
                 $multiple_opt=1;
             }else if($form_state['values']['cattr_multiple_opt'] == 'N'){
                $multiple_opt=0;   
             }
              $fields['cattr_multiple_opt']=$multiple_opt;
         }  
        
        
     $fields = trimInputFields($fields); // #42124 - Trim input values

     if ($db_action == 'insert') {
        $fields['created_by'] = $createUserId;
        $fields['created_on'] = now();
        $fields['updated_by'] = $createUserId;
        $fields['updated_on'] = now();
     }
     else {
        $fields['updated_by'] = getIdOfLoggedInUser();
        $fields['updated_on'] = now();
     }


     if($trigger_element == 'unpublish'){
        $cattr_status = 'cre_cattr_sts_itv';
        $fields = array( 'status'  => $cattr_status );
     }
     if($trigger_element == 'saveandpublish'){
        $fields['published_on'] = now();
        $fields['updated_on']   = now();
     }
     return $fields;
    } catch (Exception $ex) {
        watchdog_exception('customAttributeFormatInputString', $ex);
        expertusErrorThrow($ex);
    }
}  
 
function addUpdateCustomAttributeOptionDetails($form, &$form_state, $dbAction, $loggedInUserId,$recordCreatedLang = true,$callFromAPI = 0){
    try{ 
             
              expDebug::dPrint(' $cus_attr_id = ' . $cus_attr_id, 5);
              expDebug::dPrint(' $loggedInUserId = ' . $loggedInUserId, 5);
               
              // If call update process
              $config=getConfig('exp_sp');
              
              if($dbAction=='update'){
                 
                 $cus_attr_id=$form_state['values']['cattr_id'];
                 
                 $oldRecordsOfOptions = array();
                 if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
                 {
                     $oldRecordsOfOptions = db_query("select opt_name,cattr_id,id from slt_custom_attr_options  where cattr_id=".$cus_attr_id)->fetchAll();
                 }
                     
                 //Get Old Options
                 $old_options = db_query("select replace(group_concat(opt_name),',','##') as old_options_value from slt_custom_attr_options  where cattr_id=".$cus_attr_id)->fetchField();
                 
                 //if($recordCreatedLang)  // if record created language and updating language is same then we do update the actual values otherwise it will be updated only in translation strings
                 {
                 //delete the existing options mapping from slt_cust_attr_options 
                 $deleteExistingQuery = db_delete('slt_custom_attr_options')->condition('cattr_id', $cus_attr_id,'=');
                 expDebug::dPrintDBAPI('$deleteQuery for existion options  = ', $deleteExistingQuery);
                 $deleteExistingQuery->execute();
                 add_audit_trail_entry($loggedInUserId, $cus_attr_id, 'cre_sys_obt_cattr' , 'exp_sp_administration_customattribute', 'addUpdateCustomAttributeOptionDetails', 'Deleting existing attribute options', null, null , null);
                 }
                    
         
              }else{ //if insert
                   $cus_attr_id=$form_state['values']['id'];
              }
               
              //Get Count for custom options 
              $opt_count=trim($form_state['values']['customoptions']['count']);
              $opt_code_arr=array();
              
              if($opt_count>0){ //if there is options then place the value in slt_cust_attr_options 
                    
                    // Get Sequence Number
                     $max_seq_no=getSequenceNumber('option',$form_state['values']['id']); 
                     if($max_seq_no!=''){
                            $seq_no=$max_seq_no+1;  
                        }else{
                            $seq_no=1;
                        }
                        expDebug::dPrint("dynamic trans options::::");
                        
                            
                        //Dynamic translations
                        $translationStrings = array();
                        $translationStringscustomAttributeOptionId = array();
                   for ($i =1;$i <= $form_state['values']['customoptions']['count']; $i++) {
                       
                        $opt_name=trim($form_state['values']['customoptions']['choices']['choice-text-' . $i]); 
                        $val=preg_replace('/[^a-zA-Z0-9\_ ]/si' , '' , $opt_name); 
                        expDebug::dPrint('$val = ' . print_r($val, true), 5); 
                        $opt_code=getCustomAttributeCode($val,'o');     
                       
                        $fields = array();  
                        $fields['cattr_id'] = $cus_attr_id; 
                        $fields['opt_name'] = $opt_name; 
                        $fields['opt_code'] = $opt_code.'_'.$seq_no; 
                        $fields['opt_seq'] = $seq_no; 
                        
                        $insertOptStmt     = db_insert('slt_custom_attr_options'); 
                        expDebug::dPrint('$fields = ' . print_r($fields, true), 4); 
                        $insertOptStmt->fields($fields);
                        expDebug::dPrintDBAPI('$insertOptStmt', $insertOptStmt);  
                        
                        $customAttributeOptionId = $insertOptStmt->execute();
                        expDebug::dPrint(' $customAttributeOptionId = ' . $customAttributeOptionId, 5);
        
                        if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
                        {
                            $translationStrings[]=$opt_name;
                            $translationStringscustomAttributeOptionId[] =$customAttributeOptionId;
                         
                        }
                         $audit_opt_arr=array();
                         $audit_opt_arr=array('cattr_id','opt_name','opt_code','opt_seq');  
                         
                         if($dbAction=='insert'){
                             $action_str='Inserted';
                         }else{
                              $action_str='Updated';
                         }
                         make_audit_trail_entries($fields, $loggedInUserId , null ,$cus_attr_id, $action_str,'cre_sys_obt_cattr','exp_sp_administration_customattribute','addUpdateCustomAttributeOptionDetails',$audit_opt_arr);  
                          
                        $seq_no++;
                        
                   }
                   //Dynamic translations
                   if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
                   {
                       expDebug::dPrint("inside dynamic trans options::::".$callFromAPI);
                       include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
                       
                       if($callFromAPI) {
                           $user = getDrupalUserDataForTranslation($loggedInUserId);
                       }
                       else {
                       global $user;
                       }
                       storeContentLabels($translationStrings,$user->language,"custom_attribute_options",$translationStringscustomAttributeOptionId,"opt_name",$oldRecordsOfOptions);
                       //$translatedLabels = getTranslationsForArrayStrings($translationStrings,$user->language);
                       
                   }
                   
                   if($dbAction=='update'){
                        $new_options = db_query("select replace(group_concat(opt_name),',','##') as new_options_value from slt_custom_attr_options  where cattr_id=".$cus_attr_id)->fetchField();
                        add_audit_trail_entry($loggedInUserId, $cus_attr_id, 'cre_sys_obt_cattr' , 'exp_sp_administration_customattribute', 'addUpdateCustomAttributeOptionDetails', 'Updating attribute options', null, $old_options,$new_options);
                   }
                 
              }else{ 
                    expDebug::dPrint(' No options available', 5);
              }  
           
             return $fields;
                    
    }catch (Exception $ex) {
        watchdog_exception('addUpdateCustomAttributeOptionDetails', $ex);
        expertusErrorThrow($ex);
    }
}

function addNewCustomAttributeDetails($form, &$form_state, $createUserId,$callFromAPI =0) {
    
    expDebug::dPrint('$form_state='.print_r($form_state,true),5);
    
    $txn = db_transaction(); 

    try {
            if($callFromAPI){
                drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
                $form_state['values']['cattr_name'] = $form_state['values']['name'];
                $form_state['values']['cattr_type'] = $form_state['values']['type'];
                $form_state['values']['cattr_datatype'] = $form_state['values']['datatype'];
                $form_state['values']['cattr_length'] = $form_state['values']['datalength'];
                $form_state['values']['cattr_mandatory'] = ($form_state['values']['mandatory'] == 1) ? 'Y' : 'N';
                $form_state['values']['cattr_unique'] = ($form_state['values']['attr_unique'] == 1) ? 'Y' : 'N';
                $form_state['values']['cattr_help_txt'] = $form_state['values']['help_txt'];
                $form_state['values']['cattr_status'] = $form_state['values']['status'];
                $form_state['values']['cattr_multiple_opt'] = ($form_state['values']['multiple_option'] == 1) ? 'Y' : 'N';
                $form_state['values']['cattr_display_opt'] = ($form_state['values']['display_option'] == V) ? 'V' : 'H';
                
            }
            
            $insertStmt     = db_insert('slt_custom_attr');
            $fields         = customAttributeFormatInputString($form, $form_state,$createUserId,'insert');     
            $insertStmt->fields($fields);
            expDebug::dPrintDBAPI('$insertStmt', $insertStmt);
    
            $customAttributeId          = $insertStmt->execute();
            expDebug::dPrint(' $customAttributeId = ' . $customAttributeId, 3);
            if($callFromAPI) {
                $createUser = $createUserId;
                $user =  getDrupalUserDataForTranslation($createUser);
            }
            else {
                $createUser = getIdOfLoggedInUser();
                global $user;
            } 
            $config=getConfig('exp_sp');
            if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
            {
                
                $valuesForTranslation = array();
                $valuesForTranslation[0]=$form_state['values']['cattr_name'];
                $valuesForTranslation[1]=$form_state['values']['cattr_help_txt'];
                
                $field_names_arr = array();
                $field_names_arr[0]='cattr_name';
                $field_names_arr[1]='cattr_help_txt';
                
                storeContentLabels($valuesForTranslation,$user->language,'cre_sys_obt_cattr',$customAttributeId,$field_names_arr);
            }
             
            $audit_arr=array();
            $audit_arr[]='cattr_name';
            $audit_arr[]='cattr_type';
            $audit_arr[]='cattr_code';
            $audit_arr[]='cattr_mandatory';
            $audit_arr[]='cattr_help_txt';
            $audit_arr[]='status';
            
             if($form_state['values']['cattr_type']=='cattr_type_txtbox'){  //If textbox
                  $audit_arr[]='cattr_datatype';
                  $audit_arr[]='cattr_length';
                  $audit_arr[]='cattr_unique'; 
             }else if($form_state['values']['cattr_type']=='cattr_type_txtarea'){  //If textarea 
                  $audit_arr[]='cattr_length'; 
             }else if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_radio'){  //If Checkbox or Radio
                  $audit_arr[]='cattr_display_opt';  
             }else if($form_state['values']['cattr_type']=='cattr_type_dropdown'){  //If Checkbox 
                  $audit_arr[]='cattr_multiple_opt';  
             } 
            
            if($form_state['values']['from_migration']=='1'){ //if come from migration
                $module_name='exp_sp_administration_customattribute_migration';
            }else{
                $module_name='exp_sp_administration_customattribute';
                
            }
            //Add in audit trail
            make_audit_trail_entries($fields, $createUser , null ,$customAttributeId, 'Inserted','cre_sys_obt_cattr',$module_name,'addNewCustomAttributeDetails',$audit_arr);  
            
            // Save the new organization's id in the hidden id form field. 
            $form_state['values']['id']     = $customAttributeId;
            $form_state['values']["cus_attr_id"] = $customAttributeId;
            $fields["id"]                   = $customAttributeId;
            $fields["entity_type"]          = 'cre_sys_obt_cattr';
            $operation                      = 'insert'; 
            
             // If checkbox, radio, dropdown
             if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_radio' || $form_state['values']['cattr_type']=='cattr_type_dropdown'){
                 addUpdateCustomAttributeOptionDetails($form, $form_state, $operation, $createUserId,true,$callFromAPI);      
             }  
            return array((object)array('id'=>$customAttributeId));
            
    } catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('addNewCustomAttributeDetails', $ex);
        if($callFromAPI){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Unable to create custom attribute id.";
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

function updateCustomAttributeDetails($form, &$form_state, $createUserId,$callFromAPI =0) {

    $txn = db_transaction();

    try {

        if($callFromAPI){
            drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
            $form_state['values']['cattr_id'] = $form_state['values']['id'];
            $form_state['values']['cattr_name'] = $form_state['values']['name'];
            $form_state['values']['cattr_datatype'] = $form_state['values']['datatype'];
            $form_state['values']['cattr_length'] = $form_state['values']['datalength'];
            $form_state['values']['cattr_mandatory'] = ($form_state['values']['mandatory'] == 1) ? 'Y' : 'N';
            $form_state['values']['cattr_unique'] = ($form_state['values']['attr_unique'] == 1) ? 'Y' : 'N';
            $form_state['values']['cattr_help_txt'] = $form_state['values']['help_txt'];
            $form_state['values']['cattr_status'] = $form_state['values']['status'];
            $form_state['values']['cattr_multiple_opt'] = ($form_state['values']['multiple_option'] == 1) ? 'Y' : 'N';
            $form_state['values']['cattr_display_opt'] = ($form_state['values']['display_option'] == V) ? 'V' : 'H';
                   
        }
        $queryStr = "SELECT cattr_name,cattr_type,cattr_code,cattr_datatype,cattr_length,cattr_mandatory,cattr_unique,cattr_help_txt,cattr_seq,status,cattr_multiple_opt,cattr_display_opt from slt_custom_attr where id=:cid"; 
        $args = array(':cid' => $form_state['values']['cattr_id']);
        
        $query = db_query($queryStr, $args);
        expDebug::dPrintDBAPI('$query', $query, $args);
        $default = $query->fetchAll();
        expDebug::dPrint('$default : ' . print_r($default, true),4);
        
        if($callFromAPI)
            $form_state['values']['cattr_type'] = $default[0]->cattr_type;
        
        $trigger_element = $form_state['storage']['submit_triggering_element'];
             
            if($callFromAPI){
                $trigger_element = $form_state['values']['cattr_status'];
            } 
            
        if($callFromAPI) {
            $updateUserId = $createUserId;
        } else {
            $updateUserId = getIdOfLoggedInUser();
        }
         
        $updateStmt = db_update('slt_custom_attr');
        $fields     = customAttributeFormatInputString($form, $form_state,$updateUserId,'update',$default); 
        //Dynamic translation
        $recordCreatedLang = true;
        if(isset($fields["recordCreatedLang"])){
            $recordCreatedLang = $fields["recordCreatedLang"];
            unset($fields["recordCreatedLang"]);
        }
        $updateStmt->fields($fields);
        $updateStmt->condition('id', $form_state['values']['cattr_id']);
        expDebug::dPrintDBAPI('$updateStmt', $updateStmt); 

        // Execute the update statement.
        $numUpdated = $updateStmt->execute(); 
        
        if($trigger_element == 'unpublish'){
            make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['cattr_id'], 'Updated','cre_sys_obt_cattr','exp_sp_administration_customattribute','updateCustomAttributeDetails',
            array('status'));

        }
        else{
             
                $audit_arr=array();
                $audit_arr[]='cattr_name';
                $audit_arr[]='cattr_type';
                $audit_arr[]='cattr_code';
                $audit_arr[]='cattr_mandatory';
                $audit_arr[]='cattr_help_txt';
                $audit_arr[]='status';
                
                 if($form_state['values']['cattr_type']=='cattr_type_txtbox'){  //If textbox
                      $audit_arr[]='cattr_datatype';
                      $audit_arr[]='cattr_length';
                      $audit_arr[]='cattr_unique'; 
                 }else if($form_state['values']['cattr_type']=='cattr_type_txtarea'){  //If textarea 
                      $audit_arr[]='cattr_length'; 
                 }else if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_radio'){  //If Checkbox or Radio
                      $audit_arr[]='cattr_display_opt';  
                 }else if($form_state['values']['cattr_type']=='cattr_type_dropdown'){  //If Checkbox 
                      $audit_arr[]='cattr_multiple_opt';  
                 } 
            
             // Add audit trail
             make_audit_trail_entries($fields, $updateUserId , $default ,$form_state['values']['cattr_id'], 'Updated','cre_sys_obt_cattr','exp_sp_administration_customattribute','updateCustomAttributeDetails',$audit_arr);
        }

        $fields["id"]           = $form_state['values']['cattr_id'];
        $fields["entity_type"]  = 'cre_sys_obt_cattr';
        $operation              = 'update';
        
         if($form_state['values']['cattr_type']=='cattr_type_checkbox' || $form_state['values']['cattr_type']=='cattr_type_radio' || $form_state['values']['cattr_type']=='cattr_type_dropdown'){
             addUpdateCustomAttributeOptionDetails($form, $form_state, $operation, $updateUserId,$recordCreatedLang,$callFromAPI );      
         }  
         
        if($callFromAPI){
            return array((object)array('id'=>$form_state['values']['cattr_id']));
        }
    }
    catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('updateCustomAttributeDetails', $ex);
        if($callFromAPI){
            $errobj=new stdClass();
            $errobj->isValidateError = 1;
            $errobj->errcode = 'L_012';
            $errobj->errormsg = "Unable to update a custom attribute.";
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

/*
 * exp_sp_administration_customattribute_addedit_form_submit() - Save the data entered by the user in the exp_sp_administration_catalog_course_addedit_form_submit form
 */
function exp_sp_administration_customattribute_addedit_form_submit($form, &$form_state) {
    global $user;
    try {
        includeFiles();
        $userId = $user->uid;
        $entityType = 'cre_sys_obt_cattr'; 
        
        if($form_state['values']['cattr_type'] == 'cattr_type_txtbox'){
            $from_page = 'textbox';
        }elseif($form_state['values']['cattr_type'] == 'cattr_type_txtarea'){
            $from_page = 'textarea';
        }elseif($form_state['values']['cattr_type'] == 'cattr_type_checkbox'){
            $from_page = 'checkbox';
        }elseif($form_state['values']['cattr_type'] == 'cattr_type_dropdown'){
            $from_page = 'dropdown';
        }elseif($form_state['values']['cattr_type'] == 'cattr_type_radio'){
            $from_page = 'radio';
        }elseif($form_state['values']['cattr_type'] == 'cattr_type_date'){
            $from_page = 'date';
        }
         
        expDebug::dPrint('from page submit - ' .$from_page, 5);
        
        // Prepare list of forms to be displayed in the modal
        if(!empty($from_page)){ 
            $formIdList = array(
                    array('id' => 'exp_sp_administration_customattribute_addedit_form', 'title' => (t('LBL2015')),'from_page'=>$from_page) 
            );
                
        }else{ 
            $formIdList = array(
                    array('id' => 'exp_sp_administration_customattribute_addedit_form', 'title' => (t('LBL2015'))) 
            ); 
        }
         
        
        expDebug::dPrint('exp_sp_administration_customattribute_addedit_form_submit esign validate - ' . $_SESSION['esign_validate'], 4);
        $chkValidation = 1;
        expDebug::dPrint('exp_sp_administration_customattribute_addedit_form_submit esign validate trigger - ' .$form_state['triggering_element']['#name'], 5);
        
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
        
        /*if($form_state['triggering_element']['#name'] == 'Add custom'){
            $chkValidation = 0;
        }*/
        
        if($chkValidation){
            $form_state['esign_validation_status'] = true;
            expDebug::dPrint('exp_sp_administration_customattribute_addedit_form_submit success validation ' . $form_state['storage']['submit_triggering_element'], 5);
            $trigger =  $form_state['storage']['submit_triggering_element'];
            //  Check whether add or edit
            if(isset($form_state['values']['cattr_name'])) {
                if(empty($form_state['values']["cattr_id"])) { //if custom attribute set
                     $loggedInUserId = getIdOfLoggedInUser();
                    // Added by Gayathri for 0070892
                      /*if(is_null($form_state['values']['price']) || empty($form_state['values']['price'])){
                      $form_state['values']['price'] = '0.00';
                     }*/
                                        
                    //$returnArr = addNewCourseDetails($form, $form_state,$loggedInUserId);
                    
                    $returnArr = addNewCustomAttributeDetails($form, $form_state,$loggedInUserId); 
                    
                    expDebug::dPrint(' $returnArr = ' . print_r($returnArr, true), 5);
                    //$newCourseId = $returnArr[0]->id;
                    $newCustomAttributeId = $returnArr[0]->id;
                    
                    //saveCustomFieldsInAddEditForm($form, $form_state, $newCourseId, 'course', $userId);
                    
                    //Adding the screen details if custom attribute creation happens through shortcut in Entity Page 
                    //Check that whether custom attribute is creating through shortcut. IF create through shortcut , applicable below codes
                     expDebug::dPrint('from_shortcut=='.$form_state['storage']['from_shortcut'],5); 
                     if($form_state['storage']['from_shortcut'] == '1'){
                              $ent_attr_map_code=$form_state['storage']['shortcut_entity_code'];
                             addNewScreenDetailsForShotCut($form,$form_state,$form_state['values']['id'],$ent_attr_map_code);  
                             $form_state['storage']['shortcut_attr_form_saved']='1';   //Saved the custom attribute shortcut form
                     }
                     
                    
                    if($form_state['triggering_element']['#name'] == 'saveandpublish'){
                        //expertus_set_message(t('MSG601').'.');
                        expertus_set_message(t('LBL2014').' '.t('MSG600').'.');
                    } else {
                        expertus_set_message(t('LBL2014').' '.t('MSG600').'.');
                    }
                    
                    $entityId                            = $form_state['values']['id'];
                    $form_state['storage']['step']       = 'edit';
                    $form_state['build_info']['args'][]  = $entityId;
                    $form_state['storage']['new_entity'] = TRUE; 
                    $emptyId = $form_state['values']['empty_id']; 
                } else {

                    unset($form_state['storage']['new_entity']);
                    unset($form_state['storage']['form_info_list']); 
                     
                    updateCustomAttributeDetails($form, $form_state); 
                    
                    if($form_state['triggering_element']['#name'] == 'saveandpublish'){
                        //expertus_set_message(t('MSG601').'.');
                        expertus_set_message(t('LBL2014').' '.t('MSG600').'.');
                    } else if($form_state['triggering_element']['#name'] == 'unpublish'){
                        //expertus_set_message(t('MSG601').'.');
                        expertus_set_message(t('LBL2014').' '.t('MSG600').'.');
                    } else {
                        expertus_set_message(t('LBL2014').' '.t('MSG601').'.');
                    }

                    $form_state['storage']['step']          = 'edit';
                    $entityId                               = $form_state['values']['cattr_id'];
                    $form_state['storage']['edited_entity'] = TRUE;
                }
            }
            else{
                $entityId = $form_state['values']['mapped_cus_attr_id'];
            }

            $form_state['build_info']['args'][]       = $entityId;
            $form_state['storage']['form_info_list']  = $formIdList;
            $form_state['display_class']              = TRUE;
            $form_state['show_add_another'] = TRUE;
            
            
            if(isset($form_state['values']['attr_action']) && isset($entityId)) {
                  if($form_state['values']['attr_action'] == 'screen') { 
                    addNewScreenDetails($form, $form_state, $entityId, $entityType); 
                }
            }  
             
            $form_state['rebuild'] = TRUE;//second time will not get saved
        }
        // Setting $form_state['rebuild'] = TRUE causes the form to be rebuilt again.
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_customattribute_addedit_form_submit', $ex);
        expertusErrorThrow($ex);
    }
}

function getDataTypeOptions($code=''){
   try{
        $result['cattr_dtype_numbers'] = ucfirst(t('LBL161')); // Numbers
       // $result['cattr_dtype_date'] = t('LBL042'); //Date 
        $result['cattr_dtype_text'] = t('LBL3032'); // Alphanumeric 
        if($code!=''){
            return $result[$code]; //Send label value for corresponding code
        }
        return $result;
   }catch (Exception $ex) {
    watchdog_exception('getDataTypeOptions', $ex);
    expertusErrorThrow($ex);
   }
}

function getEntityOptions($code='',$order=''){ 
   try{
       /* $result['cattr_ent_crs'] = t('Course'); // Course
        $result['cattr_ent_cls'] = t('Class'); //Class
        $result['cattr_ent_tp'] = t('Training Plan'); // Training Plan 
        $result['cre_usr'] = t('User'); // User 
        $result['cattr_ent_org'] = t('Organization'); // Organization
        $result['cattr_ent_grp'] = t('Group'); // Group 
        $result['cattr_ent_cont'] = t('Content'); // Content
        $result['cattr_ent_loc'] = t('Location'); // Location
        $result['cattr_ent_ord'] = t('LBL1025'); // Order*/
        
   	    $result['cre_sys_obt_crs'] = t('Course'); // Course  	
        $result['cre_sys_obt_cls'] = t('Class'); //Class
        $result['cre_sys_obt_trp'] = t('Training Plan'); // Training Plan        
        $result['cre_sys_obt_cnt'] = t('Content'); // Content
        $result['cre_sys_obt_loc'] = t('Location'); // Location
        $result['cre_usr'] = t('User'); // User  
        $result['cre_org'] = t('Organization'); // Organization       
        $result['cre_sys_obt_ord'] = t('LBL1025'); // Order***
        
        
        if($order=='ASC'){
        $result1['cre_sys_obt_cls'] = t('Class'); //Class
        $result1['cre_sys_obt_cnt'] = t('Content'); // Content
   	    $result1['cre_sys_obt_crs'] = t('Course'); // Course
        $result1['cre_sys_obt_loc'] = t('Location'); // Location
        $result1['cre_sys_obt_ord'] = t('LBL1025'); // Order***
        $result1['cre_org'] = t('Organization'); // Organization
        $result1['cre_sys_obt_trp'] = t('Training Plan'); // Training Plan
        $result1['cre_usr'] = t('User'); // User        
        }               
        if($code!=''){
            return $result[$code]; //Send label value for corresponding code
        }
        if($order=='ASC'){
        	return $result1; //Send label value for corresponding code
        }
        return $result;
        
   }catch (Exception $ex) {
    watchdog_exception('getDataTypeOptions', $ex);
    expertusErrorThrow($ex);
   }
}

function edit_customattribute_details(&$form, &$form_state, $defaultValue){
    try {
        global $theme_key;
        unset($editAttributes['onclick']);
        
        $editAttributes['class'][]         = 'addedit-form-expertusone-throbber';
        $editAttributes['data-wrapperid']  = array('customattribute-basic-addedit-form');
        //$editAttributes['class'][]         = 'addedit-form-class-button';
        $editAttributes['class'][]         = 'admin-add-edit-button';
        $editAttributes['onclick'][]       = '$(document).ready(function(){ editCustomAttributeDetailsView(); }); remove_messages("customattribute");';
        
        expDebug::dPrint('display_mode='.print_r($form_state['storage']['display_mode'],true),5);
        expDebug::dPrint('substep='.print_r($form_state['storage']['substep'],true),5);
        
        
        if($form_state['storage']['display_mode'] != 'view' && ($form_state['storage']['substep'] == 'attribute' || empty($form_state['storage']['substep']))){
            $entityType = 'cre_sys_obt_cattr';
            $entityId = array($defaultValue['cattr_id']);
            $userId = getIdOfLoggedInUser();
            $check = objectLevelAccessCheckInGroupMap($entityType,$entityId); //Tmp Need to check group level
            $editableMode= true;
            if(!empty($check)){
                $customAttributeEditCheck = apiUseraccesscheck($userId,$entityType,$defaultValue['cattr_id']); 
                if(!empty($customAttributeEditCheck)){
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
                      '#name'       => 'Edit Custom Attribute',
                      '#prefix'     => '<div id="wizard-customattribute-form-wrapper"><div class="admin-customattribute-edit-button-container">',
                      '#suffix'     => '</div></div>',
                      '#attributes' => $editAttributes,
                      '#ajax' => array(
                        'callback' => 'addEditRepaintForm',
                        'wrapper'  => 'customattribute-basic-addedit-form',
                        'event'    => 'click'
                        ),
                        );
            }
        }
    } catch (Exception $ex) {
        watchdog_exception('edit_customattribute_details', $ex);
        expertusErrorThrow($ex);
    }
}

function edit_customattribute_view(&$form, &$form_state, $defaultValue, $entityId){

    try { 

        $form['cattr_name'] = array(
        '#type'     => 'item',
        '#title'    => t('Label').':',
        '#prefix'   => '<div class="addedit-new-field"><div class="addedit-course-field-title">',
        '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt genere-title vtip" title="'.sanitize_data($defaultValue['cattr_name']).'">'.titleController('ADDEDIT-COURSE-FIELD-TITLE', sanitize_data($defaultValue['cattr_name']),50).'</div><div class="clearBoth"></div>',
        '#suffix'   => '</div>',
        );


        $dt_lbl=getDataTypeOptions($defaultValue['cattr_datatype']);

        $form['cattr_datatype'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL2000').':',
        '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view course-view-mode">',
        '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.sanitize_data($dt_lbl).'" class="vtip">'.titleController('ADDEDIT-COURSE-FIELD-CODE', sanitize_data($dt_lbl),20).'</span></div>',
        '#suffix'   => '</div>',
        );
        $form['cattr_length'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL2001').':',
        '#prefix'   => '<div class="second-col-container-view">',
        '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.t($defaultValue['cattr_length']).'</div>',
        '#suffix'   => '</div><div class="clearBoth"></div></div>',
        ); 
        
         $mand_opt=(trim($defaultValue['cattr_mandatory'])=='' ? '' : trim($defaultValue['cattr_mandatory'])=='1'? 'Yes' : 'No' );
        $mand_opt=t($mand_opt);
        $form['cattr_mandatory'] = array(
        '#type'     => 'item',
        '#title'    => t('Mandatory').':',
        '#prefix'   => '<div class="two-column-container-view"><div class="first-col-container-view">',
        '#markup'   => '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.$mand_opt.'" class="vtip">'.$mand_opt.'</span></div>',
        '#suffix'   => '</div>',
        );
        
        $unique_opt=(trim($defaultValue['cattr_unique'])=='' ? '' : trim($defaultValue['cattr_unique'])=='1'? 'Yes' : 'No' );
        $unique_opt=t($unique_opt);
        $form['cattr_unique'] = array(
        '#type'     => 'item',
        '#title'    => t('LBL2002').':',
        '#prefix'   => '<div class="second-col-container-view">',
        //'#markup'   => '<div class="second-col-value-view view-mode-field-value-txt">'.(($defaultValue['is_compliance'] == 1) ? t('Yes') : t('No')).'</div>',
        '#markup'   => '<div class="second-col-value-view view-mode-field-value-txt"><span title="'.$unique_opt.'" class="vtip">'.$unique_opt.'</span></div>',
        '#suffix'   => '</div><div class="clearBoth"></div></div>',
        );


        $form['cattr_help_txt'] = array(
         '#type'     => 'item',
         '#title'    => t('LBL2003'),
         '#prefix'   => '<div class="admin-course-class-short-description"><div class="addedit-new-field"><div class="addedit-course-field-title">',
         '#markup'   => '</div><div class="addedit-new-field-value view-mode-field-value-txt">'.$defaultValue['cattr_help_txt'].'</div><div class="clearBoth"></div>',
         '#suffix'   => '</div></div>',
         ); 
         
        $form['sortable-load'] = array(
            '#type' => 'markup',
            '#prefix' => '<img src="'.base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/drag.png" width="0" height="0" onload="vtip(); " />',
        ); 
         
    } catch (Exception $ex) {
        watchdog_exception('edit_customattribute_view', $ex);
        expertusErrorThrow($ex);
    }
}


function getCustomAttributeDetail($cattr_id){
    try {
        $select = db_select("slt_custom_attr","cattr");
        //$select->innerJoin('slt_profile_list_items', 'prof1', 'prof1.code = crs.lang_code');
        $select->innerJoin('slt_profile_list_items', 'prof2', 'prof2.code = cattr.status');
        $select->addField('cattr','id','cattr_id');
        $select->addField('cattr','cattr_name','cattr_name');
        $select->addField('cattr','cattr_code','cattr_code');
        $select->addField('cattr','cattr_datatype','cattr_datatype');
        $select->addField('cattr','cattr_length','cattr_length');
        $select->addField('cattr','cattr_mandatory','cattr_mandatory');
        $select->addField('cattr','cattr_unique','cattr_unique');
        $select->addField('cattr','cattr_display_opt','cattr_display_opt');
        $select->addField('cattr','cattr_multiple_opt','cattr_multiple_opt');
        $select->addField('cattr','status','cattr_status');
        $select->addField('prof2','name','cattr_status_name');
        $select->addField('cattr','cattr_help_txt','cattr_help_txt');
        $select->condition('cattr.id',$cattr_id,'=');
        expdebug::dPrintDBAPI('$select', $select);
        $cattrList = $select->execute()->fetchAll();
        expdebug::dPrint('$cattrList' . print_r($cattrList, true),3);
        
        //Dynamic translation
       /* $config=getConfig('exp_sp');
        if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
        {
            
            include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
            global $user;
            
            $translationStrings = array();
            $translationIds = array();
            foreach ($cattrList as $Custom) {
                $title    =   $Custom->cattr_name;
                $helptext =   $Custom->cattr_help_txt;
                
                $translationStrings[]=$title;
                $translationStrings[]=$helptext;
                $translationIds[]=$Custom->cattr_id;
            }
            
            expDebug::dPrint(' $translatedLabels before = ' . print_r($translationStrings, true) , 3);
            
            $translatedLabels = getTranslationsForArrayStrings($translationStrings,$user->language,$translationIds,"cre_sys_obt_cattr");
            expDebug::dPrint(' $translatedLabels  after = ' . print_r($translationStrings, true) , 3);
            
            foreach ($cattrList as $Custom) {
                $Custom->cattr_name          = strval($translatedLabels[$Custom->cattr_name]);
                $Custom->cattr_help_txt      = strval($translatedLabels[$Custom->cattr_help_txt]);
            }
        }*/

        return $cattrList;
    } catch (Exception $ex) {
        watchdog_exception('getCustomAttributeDetail', $ex);
        expertusErrorThrow($ex);
    }
} 

function getCustomAttributeOptionDetail($cattr_id){
    try {
        $select = db_select("slt_custom_attr_options","cattr"); 
        $select->addField('cattr','id','cattr_id');
        $select->addField('cattr','opt_name','opt_name');
        $select->addField('cattr','opt_code','opt_code'); 
        $select->condition('cattr.cattr_id',$cattr_id,'=');
        expdebug::dPrintDBAPI('$select', $select);
        $cattrList = $select->execute()->fetchAll();
        expdebug::dPrint('Custom Attribute Option List' . print_r($cattrList, true),3);
        return $cattrList;
    }catch (Exception $ex) {
        watchdog_exception('getCustomAttributeDetail', $ex);
        expertusErrorThrow($ex);
    }
}

 
function getDefaultCustomAttributeAddEditFormFieldValue($fields, $form_state, $customAttributeDetails) {
    try {
        $defaultValue   = '';
        $customAttributeDefaultValue    = array();
        for($i=0;$i<count($fields);$i++) {
            $field  = $fields[$i]; 
            $customAttributeDefaultValue[$field] = empty($form_state['values'][$field]) ? (empty($customAttributeDetails->$field) || $customAttributeDetails->$field==''? '' : $customAttributeDetails->$field) : $form_state['values'][$field];
        }
       
         //Get Custom Attribute Options value  
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderoptions.inc');  
        $customAttributeDefaultValue=getDefaultCustomAttributeOptions($form_state,$customAttributeDefaultValue,$customAttributeDetails);      
       
        return $customAttributeDefaultValue;
        
    } catch (Exception $ex) {
        watchdog_exception('getDefaultCustomAttributeAddEditFormFieldValue', $ex);
        expertusErrorThrow($ex);
    }
}

/**
 * exp_sp_administration_customattribute_addedit_form() - Build the addedit form for Custom Attribute. 
 */
function exp_sp_administration_customattribute_addedit_form($form, &$form_state, $renderMode,$attr, $cus_attr_id){
    try {  
        
        expDebug::dPrint('substep ==='.$form_state['storage']['substep'],5); 
        expDebug::dPrint('step ==='.$form_state['storage']['step'],5);  
        expDebug::dPrint('$renderMode ==='.print_r($renderMode,true),5);  
        expDebug::dPrint('$cus_attr_id ==='.$cus_attr_id,5);  
        expDebug::dPrint('$attr ==='.$attr,5);  
        expDebug::dPrint('$form_state[storage][form_info_list] ==='.print_r($form_state['storage']['form_info_list'][0]['from_page'],1),5);  
         expDebug::dPrint('from_shortcut ==='.$form_state['storage']['from_shortcut'],5);  
         
    
         //Show the screen page directly when click on Show in Preview 
         $from_showinscreen=$form_state['storage']['form_info_list'][0]['from_showinscreen'];
          
        if($from_showinscreen=='from_show_in_screen' && empty($form_state['storage']['substep']) && empty($form_state['storage']['step'])){
            $form_state['storage']['substep'] = 'screen'; 
            expDebug::dPrint('I am screen preview');
        }
        
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_screen.inc');
        
        if($form_state['storage']['substep'] == 'screen')
            $container_id = 'form-screen';
        else
            $container_id = 'form-attribute';
        
        
        $showCancelSave = 1;
        
        // To set the display mode as view
        $readOnlyForView = false;
        if(in_array('view', $form_state['build_info']['args'])){
            $form_state['storage']['step']         = 'view';
            $form_state['storage']['display_mode'] = 'view';
            $readOnlyForView = true;
        }

        if (empty($form_state['storage'])) {
            if(!empty($cus_attr_id) && empty($form_state['storage'])) {
                $form_state['storage']['step'] = 'view';
            } elseif(empty($cus_attr_id) && empty($form_state['storage'])) {
                $form_state['storage']['step'] = 'add';
            }
        }  
        
        $cus_att_details     = array(); 
        if (!empty($cus_attr_id)) {
            //$cus_att_details   = getCourseDetail($courseId, array('id', 'title', 'code', 'lang_code','short_description', 'description','status','price','currency_type','currency_symbol','author_vendor','is_compliance','complete_days','complete_date','validity_days','validity_date', 'compliance_completed')); // Changed by Gayathri for 0070892
            //$cus_att_details   = getCustomAttributeDetail($cus_attr_id, array('id', 'cattr_name', 'cattr_type', 'cattr_datatype','cattr_length', 'cattr_mandatory','cattr_unique','cattr_help_txt','status'));
            $cus_att_details   = getCustomAttributeDetail($cus_attr_id); 
        } 
        
        $form              = newAddEditFormWithCancelAndSave('customattribute-basic-addedit-form');
        $form['#attributes']['class'][] = 'customattribute_addedit_form_common';
        //$fieldList         = array('crs_id', 'crs_title', 'crs_code', 'crs_language', 'crs_short_description', 'crs_description','crs_language_name','crs_status_name','crs_price','crs_currency_type','crs_currency_symbol','crs_author_vendor','is_compliance','complete_days','completed_hire_days','complete_date','validity_days','validity_date', 'compliance_completed'); // Changed by Gayathri for 0070892
        $fieldList         = array('cattr_id', 'cattr_name', 'cattr_type', 'cattr_datatype','cattr_length', 'cattr_mandatory','cattr_unique','cattr_help_txt','cattr_display_opt','cattr_multiple_opt');
     
        $defaultValue      = getDefaultCustomAttributeAddEditFormFieldValue($fieldList, $form_state, $cus_att_details[0]);    
        
        $emptyId = '';
        $entityId = 0;
        if(empty($cus_attr_id)){
            $entityId       = $form_state['values']['id'];
            $emptyId        = empty($form_state['values']['empty_id']) ? microtime(false) : $form_state['values']['empty_id'];
        } else {
            $entityId       = $cus_attr_id;
        }
        $form['mapped_cus_attr_id'] = array(
              '#type' => 'hidden',
              '#name' => 'mapped_cus_attr_id',
              '#value' => $entityId
        ); 
        
        $entityType        = 'cre_sys_obt_cattr';
        $uniqueId          = $entityId.'-'.$entityType;

        addHiddenFieldToAddEditForm($form, 'empty_id', $emptyId);
        addHiddenFieldToAddEditForm($form, 'entity_value', $entityType);
     
        $form['customattribute_detail_disp'] = array(
            '#type'       => 'markup',
            '#prefix'     => '<div id="customattribute-basic-addedit-form-disp">', 
        );

        list_custom_attribute_forms($form, $form_state, $defaultValue); 
        
        $tmp_attr_cls='';
        if($attr!=''){
            $tmp_attr_cls=' custom_attr_'.$attr;
        }
        
        $tmp_attr_screen_cls='';
        if($container_id=='form-screen'){
            $tmp_attr_screen_cls=' '.$tmp_attr_cls.'_screen';
        }
        
        $form['customattribute_detail_wrapper'] = array(
            '#type'       => 'markup',
            '#prefix'     => '<div id='.$container_id.' class="addedit-form-main-container'.$tmp_attr_cls.$tmp_attr_screen_cls.'">
                                <div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
                                <div class="customattribute-basic-addedit-form-container" id="customattribute-basic-addedit-form-container">',
        );

        if($form_state['storage']['step'] == 'view') { 
             
                    edit_customattribute_details($form, $form_state, $defaultValue); 
              
                    if($form_state['storage']['substep'] == 'attribute' || empty($form_state['storage']['substep'])) {
                        edit_customattribute_view($form, $form_state, $defaultValue,$entityId); 
                        $form_state['storage']['substep'] = 'attribute';
                    }
                     
                    if($form_state['storage']['substep'] == 'screen') { 
                        $showCancelSave = 0;
                        $form['screen-Details-view-start'] = array(
                                                              '#type'   => 'markup',
                                                              '#markup' => '<div>',
                        ); 
                        displayScreenDetails($form, $form_state, $entityId, $entityType, $uniqueId, 'complete_view');
                    } 
                     
                    unset($form['addedit_tmp_storage']);
                    
                    $form['addedit-left-col-end1'] = array(
                        '#type'   => 'markup',
                        '#markup' => '</div>',
                    );
             
        } else { // For Adding  
             if($form_state['storage']['substep'] == 'attribute' || empty($form_state['storage']['substep'])) { 
                      
                     //Whether to show madatory/unique/DataType/length readonly or not
                      $read_only_status=false;
                      if($defaultValue['cattr_id']!=''){
                          $ent_map_screen_results=getScreenDetailsfromDB($defaultValue['cattr_id']); 
                          expDebug::dPrint('$ent_map_screen_results = '.print_r($ent_map_screen_results,true),5);
                          $count_mapped_enties=count($ent_map_screen_results);
                          expDebug::dPrint('$count_mapped_enties = '.print_r($count_mapped_enties,true),5);
                          if($count_mapped_enties>0){
                            $read_only_status=true;
                          }
                      } 
                      
                      if($form_state['storage']['from_shortcut']=='1'){
                          $read_only_status=false;
                      }
                       //To show the help icon
                        $path = base_path() . drupal_get_path('module', 'uc_credit').'/images/info.png';
        
                        $info_symbol_arr['span_id']='vtip_custom_attr';
                        $info_symbol_arr['image_path']=$path;
                        //$info_symbol_arr['class_name']='vtip cus_attr_vtip'; 
                        $info_symbol_arr['class_name']='vtip info-enr-upload';  

                      if($attr=='textbox'){
                        getTextBoxCustomAttributeFromFields($form,$form_state,$defaultValue,$read_only_status,$info_symbol_arr); 
                      }else if($attr=='textarea' || $attr=='date' ){
                        getTextAreaCustomAttributeFromFields($form,$form_state,$defaultValue,$read_only_status,$info_symbol_arr,$attr); 
                      }else if($attr=='dropdown' || $attr=='checkbox' || $attr=='radio'){ 
                        getCheckBoxCustomAttributeFromFields($form,$form_state,$defaultValue,$read_only_status,$info_symbol_arr,$attr);  
                      } 
            }
            
            $form_state['esign_from_plugin']= false; 

            // To display the Pre-requisite UI
            if($form_state['storage']['substep'] == 'screen') {
                $form_state['esign_from_plugin']= true;
                $showCancelSave = 0; 
                displayScreenDetails($form, $form_state, $entityId, $entityType, $uniqueId);
            } 

            // Checking to show the cancel and save button
            if($showCancelSave && $form_state['storage']['display_mode'] != 'view') {
                $subStage = $form_state['storage']['substep'];
                addEditFormWithCustomAttributeCancelAndSaveClose($form,$form_state,'customattribute-basic-addedit-form', 'customattribute-basic-save', 'customattribute-basic-cancel', $entityId, $subStage, $emptyId);
                 
            } 
        }


     $form['customattribute_detail_disp_end'] = array(
          '#type'       => 'markup', 
          '#suffix'     => '<div class="clearBoth"></div>
                            <div class="round-corner-footer-left"><div class="round-corner-footer-right">
                            <div class="round-corner-footer-middle"></div></div></div></div></div><div class="clearBoth"></div>',
     );

     if($showCancelSave && $form_state['storage']['display_mode'] != 'view'){
            $form['customattribute_detail_wrapper_end'] = array(
            '#type'       => 'markup',
            '#suffix'     => '</div>',
            );
     } else {
        $form['customattribute_detail_wrapper_end'] = array(
            '#type'       => 'markup',
            '#suffix'     => '<div class="clearBoth"></div><div class="admin-class-course-separater"></div></div>',
        );
     }
     
     endAddEditTwoColumnRow($form);
     return $form;
     

    } catch (Exception $ex) {
        watchdog_exception('exp_sp_administration_customattribute_addedit_form', $ex);
        expertusErrorThrow($ex);
    }
}  
 
function getCheckBoxCustomAttributeFromFields(&$form,&$form_state,$defaultValue,$read_only_status,$info_symbol_arr,$attr){
     try{
          expDebug::dPrint('attr='.$attr,5);
          
          if($attr=='checkbox'){
              $entity_type='cattr_type_checkbox';
          }else if($attr=='dropdown'){
              $entity_type='cattr_type_dropdown';
          }else if($attr=='radio'){
              $entity_type='cattr_type_radio';
          }
                 
        addHiddenValueToAddEditForm($form, 'cattr_id', $defaultValue['cattr_id']);  
        addHiddenValueToAddEditForm($form, 'cattr_type', $entity_type); //if cattr_type_checkbox hardcoded  
            
       
         $tab_index=1;
         if($form_state['storage']['from_shortcut']=='1'){
            $tab_index=$tab_index+100; 
         }
        
       
         $info_symbol_arr['msg']=t('MSG1100'); // For Title 
        //label
        $form['addedit-remove-space-title-start'] = array(
        '#type'   => 'markup',
        '#markup' => '<div style="width:100%;">',
        );
        
        //addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,20,'','','',$info_symbol_arr);
        addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,30,'','','',$info_symbol_arr);
        $form['addedit-remove-space-title-end'] = array(
        '#type'   => 'markup',
        '#markup' => '</div>',
        );
        
        // Render Options
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderoptions.inc');
        addCustomAttributeOptionsBlock($form,  $defaultValue,$read_only_status);
        //addCustomAttributeOptionsBlock($form,  $defaultValue);
        
        $yes_or_no_options = array('N' => t('No'), 'Y' => t('Yes'));
        addTwoColumnRowToAddEditForm($form, $entity_type.'_length');    
                
        $defaultValue['cattr_mandatory']=trim($defaultValue['cattr_mandatory']);
        
        if($defaultValue['cattr_mandatory']!='' && ($defaultValue['cattr_mandatory']=='Y' || $defaultValue['cattr_mandatory']=='1')){ 
            $def_mand='Y'; 
        }else{
            $def_mand='N';
        } 
            
        $info_symbol_arr['msg']=t('MSG1103'); // For Mand or not
            
        addDropDownToAddEditForm($form, 'cattr_mandatory', t('Mandatory'), $yes_or_no_options, $def_mand,$tab_index+10, false,$read_only_status,'','customattribute',$info_symbol_arr); 
        
       
        
        if($attr=='checkbox' || $attr=='radio'){  // For Display Options
            
            $display_options = array('H' => t('LBL662'), 'V' => t('LBL663')); 
            $defaultValue['cattr_display_opt']=trim($defaultValue['cattr_display_opt']);
            if($defaultValue['cattr_display_opt']!=''){
                $def_display_opt=$defaultValue['cattr_display_opt'];
            }else{
                $def_display_opt='N';
            }       
            
            $info_symbol_arr['msg']=t('MSG1106'); // For display option
            addDropDownToAddEditForm($form, 'cattr_display_opt', t('LBL2013'), $display_options, $def_display_opt,$tab_index+11, false,$read_only_status,'','customattribute',$info_symbol_arr);
          
         
        }else if($attr=='dropdown'){ //Multiple Options 
             
            $defaultValue['cattr_multiple_opt']=trim($defaultValue['cattr_multiple_opt']);
            if($defaultValue['cattr_multiple_opt']!='' && ($defaultValue['cattr_multiple_opt']=='Y'|| $defaultValue['cattr_multiple_opt']==1)){
                $def_multiple_opt='Y';
            }else{
                $def_multiple_opt='N';
            }   
            
            $info_symbol_arr['msg']=t('MSG1107'); // For Multiple option 
            addDropDownToAddEditForm($form, 'cattr_multiple_opt', t('LBL2011'), $yes_or_no_options, $def_multiple_opt, $tab_index+11, false,$read_only_status,'','customattribute',$info_symbol_arr); 
            
       }  
       
       $form[$entity_type.'_length_end'] = array(
            '#type'   => 'markup',
            '#markup' => '</div>',
       );
       
         // help text
         $form['addedit-add-start-customattr-options-block'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="cust_attr_option_block">',
        );    
          
         $form['addedit-add-end-customattr-options-block'] = array(
                '#type'   => 'markup',
                '#markup' => '</div>',
        );
          
          
        // help text
        /* $form['addedit-add-space'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="space_before_textareatext">',
        );
        // help text
        addTextAreaFieldToAddEditForm($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+12);
        
        $form['addedit-end-space'] = array(
                '#type'   => 'markup',
                '#markup' => '</div>',
        );*/
        
        $form['addedit-add-space'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="space_before_textareatext"><div id="cattr_help_txt_textarea_full_row">', 
          );
                
             
        // help text
        //addTextAreaFieldToAddEditForm($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+5);  
        addTextAreaFieldForShortDescription($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+12,false,false, 255,'',array(),array(),'customattr');
   
        $form['addedit-end-space'] = array(
                '#type'   => 'markup',
                '#markup' => '</div></div>',
        );
        
         
     }catch (Exception $ex) {
        watchdog_exception('getCheckBoxCustomAttributeFromFields', $ex);
        expertusErrorThrow($ex);
    }
}

function getTextAreaCustomAttributeFromFields(&$form,&$form_state,$defaultValue,$read_only_status,$info_symbol_arr,$attr){
     try{
        
        expDebug::dPrint('$attr='.$attr,5);
        
         if($attr=='textarea'){
              $entity_type='cattr_type_txtarea';
          }else if($attr=='date'){
              $entity_type='cattr_type_date';
          }  
          
        addHiddenValueToAddEditForm($form, 'cattr_id', $defaultValue['cattr_id']);  
        addHiddenValueToAddEditForm($form, 'cattr_type', $entity_type); //if textbox hardcoded
        
       
         $tab_index=1;
         if($form_state['storage']['from_shortcut']=='1'){
            $tab_index=$tab_index+100; 
         }
                 
         $info_symbol_arr['msg']=t('MSG1100'); // For Title 
        //label
        $form['addedit-remove-space-title-start'] = array(
        '#type'   => 'markup',
        '#markup' => '<div style="width:100%;">',
        );
        
        //addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,20,'','','',$info_symbol_arr);
        addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,30,'','','',$info_symbol_arr);
        $form['addedit-remove-space-title-end'] = array(
        '#type'   => 'markup',
        '#markup' => '</div>',
        );
        
        //mandatory
        
        $yes_or_no_options = array('N' => t('No'), 'Y' => t('Yes')); 
        
        addTwoColumnRowToAddEditForm($form, 'custom-attr-txtarea-length');
        
        // length 
        $info_symbol_arr['msg']= t('MSG1108',array('@count'=>'500')); // For Length
        
        addTextFieldToAddEditForm($form, 'cattr_length', t('LBL2001'), $defaultValue['cattr_length'],$tab_index+1, true,$read_only_status,3,'','','',$info_symbol_arr);
         
        
        $defaultValue['cattr_mandatory']=trim($defaultValue['cattr_mandatory']);
        
        
        //if($defaultValue['cattr_mandatory']!='' && $defaultValue['cattr_mandatory']==1){
        if($defaultValue['cattr_mandatory']!='' && ($defaultValue['cattr_mandatory']=='Y' || $defaultValue['cattr_mandatory']=='1')){
        $def_mand='Y';
        }else{
            $def_mand='N';
        } 
        
        $info_symbol_arr['msg']=t('MSG1103'); // For Mand or not
            
        addDropDownToAddEditForm($form, 'cattr_mandatory', t('Mandatory'), $yes_or_no_options, $def_mand,$tab_index+2, false,$read_only_status,'','customattribute',$info_symbol_arr);
        
        // length
        //$info_symbol_arr['msg']=t('MSG1102'); // For Length
        
        //addTextFieldToAddEditForm($form, 'cattr_length', t('LBL2001'), $defaultValue['cattr_length'], 3, true,$read_only_status,3,'','','',$info_symbol_arr);
        
        $form['custom-attr-txtarea-length-end'] = array(
                '#type'   => 'markup',
                '#markup' => '</div>',
        );
        
        // help text
       /* $form['addedit-add-space'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="space_before_textareatext">',
        );
        // help text
        addTextAreaFieldToAddEditForm($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+3); 
        
        $form['addedit-end-space'] = array(
                '#type'   => 'markup',
                '#markup' => '</div>',
        );
        */
        
          $form['addedit-add-space'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="space_before_textareatext"><div id="cattr_help_txt_textarea_full_row">', 
          );
                
             
        // help text
        //addTextAreaFieldToAddEditForm($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+5);  
        addTextAreaFieldForShortDescription($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+3,false,false, 255,'',array(),array(),'customattr');
   
        $form['addedit-end-space'] = array(
                '#type'   => 'markup',
                '#markup' => '</div></div>',
        );
        
        
        
        expDebug::dPrint(' gettextareacustomformfields >>'.$defaultValue,5);
         
         
     } catch (Exception $ex) {
        watchdog_exception('getTextAreaCustomAttributeFromFields', $ex);
        expertusErrorThrow($ex);
    }
}

function getTextBoxCustomAttributeFromFields(&$form,&$form_state,$defaultValue,$read_only_status,$info_symbol_arr){
    try{
        
                 addHiddenValueToAddEditForm($form, 'cattr_id', $defaultValue['cattr_id']); 
                 addHiddenValueToAddEditForm($form, 'cattr_type', 'cattr_type_txtbox'); //if textbox hardcoded  
                    
                 $tab_index=1;
                 if($form_state['storage']['from_shortcut']=='1'){
                    $tab_index=$tab_index+100; 
                 }
        
                   
                 $info_symbol_arr['msg']=t('MSG1100'); // For Title 
                  
                // Start Title
                $form['addedit-remove-space-title-start'] = array(
                                                                  '#type'   => 'markup',
                                                                  '#markup' => '<div style="width:100%;">',
                                                                 ); 
                
                //addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,20,'','','',$info_symbol_arr);
                addTextFieldToAddEditForm($form, 'cattr_name', t('Label'), $defaultValue['cattr_name'], $tab_index, true,false,30,'','','',$info_symbol_arr);
                $form['addedit-remove-space-title-end'] = array(
                                                                  '#type'   => 'markup',
                                                                  '#markup' => '</div>', 
                                                                );

                
                
                addTwoColumnRowToAddEditForm($form, 'custom-attr-txt-length');
                
                $info_symbol_arr['msg']=t('MSG1102'); // For Length
                
                addTextFieldToAddEditForm($form, 'cattr_length', t('LBL2001'), $defaultValue['cattr_length'], $tab_index+1, true,$read_only_status,3,'','','',$info_symbol_arr);
                
                //addTwoColumnRowToAddEditForm($form, 'cattr_datatype');                  
                $defaultValue['cattr_datatype'] = !empty($defaultValue['cattr_datatype']) ? $defaultValue['cattr_datatype'] : 'cattr_dtype_text';
                 
                //addDropDownToAddEditForm(&$form, $id, $title = '', $options = array(), $defaultOption = '', $tabIndex = null, $mandatory = false, $readOnly = false,$attributes = array(),$pagefrom ='',$info_symbol_arr=array()) {
                 $info_symbol_arr['msg']=t('MSG1101'); // For DataType  
                addDropDownToAddEditForm($form, 'cattr_datatype', t('LBL2000'), getDataTypeOptions(), $defaultValue['cattr_datatype'], $tab_index+2, false,$read_only_status,'','customattribute',$info_symbol_arr);
                
                //$info_symbol_arr['msg']=t('MSG1102'); // For Length
                
               // addTextFieldToAddEditForm($form, 'cattr_length', t('LBL2001'), $defaultValue['cattr_length'], 3, true,$read_only_status,3,'','','',$info_symbol_arr);
                //endAddEditTwoColumnRow($form); 
                 
                $form['custom-attr-txt-length-end'] = array(
                        '#type'   => 'markup',
                        '#markup' => '</div>',
                );
                
                $yes_or_no_options = array('N' => t('No'), 'Y' => t('Yes'));
                    
                addTwoColumnRowToAddEditForm($form, 'custom-attr-txt-mandatory');                 
                //$defaultValue['cattr_mandatory'] = !empty($defaultValue['cattr_mandatory']) ? $defaultValue['cattr_mandatory'] : '';
                
                $defaultValue['cattr_mandatory']=trim($defaultValue['cattr_mandatory']);
                
                //if($defaultValue['cattr_mandatory']!='' && $defaultValue['cattr_mandatory']==1){
                if($defaultValue['cattr_mandatory']!='' && ($defaultValue['cattr_mandatory']=='Y' || $defaultValue['cattr_mandatory']=='1')){                
                    $def_mand='Y'; 
                }else{
                    $def_mand='N';
                } 
                    
                $info_symbol_arr['msg']=t('MSG1103'); // For Mand or not
                    
                addDropDownToAddEditForm($form, 'cattr_mandatory', t('Mandatory'), $yes_or_no_options, $def_mand,$tab_index+3, false,$read_only_status,'','customattribute',$info_symbol_arr); 
                 
                
                $defaultValue['cattr_unique']=trim($defaultValue['cattr_unique']);
                
                if($defaultValue['cattr_unique']!='' && ($defaultValue['cattr_unique']==1 || $defaultValue['cattr_unique']=='Y')){
                    $def_unique='Y';
                }else{
                    $def_unique='N';
                }
              
                 $info_symbol_arr['msg']=t('MSG1104'); // For Unique or not
                addDropDownToAddEditForm($form, 'cattr_unique', t('LBL2002'), $yes_or_no_options, $def_unique, $tab_index+4, false,$read_only_status,'','customattribute',$info_symbol_arr);
                 
                $form['custom-attr-txt-mandatory-end'] = array(
                    '#type'   => 'markup',
                    '#markup' => '</div>',
                );
                
                // Start Help Text                   
                                                            
                 $form['addedit-add-space'] = array(
                '#type'   => 'markup',
                '#markup' => '<div id="space_before_textareatext"><div id="cattr_help_txt_textarea_full_row">', 
                );
                
             
                // help text
                //addTextAreaFieldToAddEditForm($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+5);  
                addTextAreaFieldForShortDescription($form, 'cattr_help_txt', t('LBL2003'), $defaultValue['cattr_help_txt'], $tab_index+5,false,false, 255,'',array(),array(),'customattr');
           
                $form['addedit-end-space'] = array(
                        '#type'   => 'markup',
                        '#markup' => '</div></div>',
                );
        
                
     } catch (Exception $ex) {
        watchdog_exception('getTextBoxCustomAttributeFromFields', $ex);
        expertusErrorThrow($ex);
      }
    
}

function getIfAttributeeUnpublished($attr_id){
 try {
      $select = db_select('slt_custom_attr', 'attr');
      $select->addExpression('COUNT(1)','total_attr');
      $select->condition('attr.id', $attr_id, '=');
      $select->condition('attr.status', 'cre_cattr_sts_atv', '=');
      expDebug::dPrintDBAPI('$select', $select);
      $total_attr = $select->execute()->fetchField();
      expDebug::dPrint(' $total_attr - '.$total_attr,5);
      if ($total_attr== 0){
         return 0;
      } else {
         return 1;
      }
      } catch (Exception $ex) {
        watchdog_exception('getIfAttributeeUnpublished', $ex);
        expertusErrorThrow($ex);
      }
}

//if there is duplicate, it will sends the 1 
function checkDuplicateAttributeName($attr_name,$attr_id=''){
 try {
        
      expDebug::dPrint(' $attr_name - '.$attr_name.',$attr_id - '.$attr_id,5);
      
      $tmp_attr_name=trim(strtolower($attr_name));
      $select = db_select('slt_custom_attr', 'attr');
      $select->addExpression('COUNT(1)','total_attr');
      if($attr_id!=''){
        $select->condition('attr.id', $attr_id, '!=');  
      }
      $select->condition('attr.status', 'cre_cattr_sts_del', '!=');
      $select->where('LOWER(attr.cattr_name)=\''.$tmp_attr_name.'\''); 
      
      expDebug::dPrintDBAPI('$select', $select);
      $total_attr = $select->execute()->fetchField();
      expDebug::dPrint(' $total_attr - '.$total_attr,5);
      
      if ($total_attr==0){
         return 0;
      } else {
         return 1;
      }
  } catch (Exception $ex) {
    watchdog_exception('checkDuplicateAttributeName', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * addEditFormWithCustomAttributeCancelAndSaveClose() 
 */
function addEditFormWithCustomAttributeCancelAndSaveClose(&$form,&$form_state,$wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $cus_attr_id, $subStage = '', $emptyId = '') {
    global $theme_key;
    try {
        expDebug::dPrint(' addEditFormWithCustomAttributeCancelAndSaveClose() called.');

        $customHiddenButtonId = 'last-button-account-custom-attr';

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

        //if(empty($cus_attr_id)){
            expDebug::dPrint('from_shortcut in Cancel Button =='.$form_state['storage']['from_shortcut'],5);  
            
            if($form_state['storage']['from_shortcut']=='1'){  //Custom attribute creation through Shortcut then call the qtip close action
              
              /* if($form_state['storage']['shortcut_attr_form_saved']=='1'){
                  //$cancelAttributes['onclick'] = 'closeCustomAttributeQtip(\'5\',\'cre_usr\',\'1\');return false;';    //Tmp - Hardcoded - Need to change the Id
                  $cancelAttributes['onclick'] = 'closeCustomAttributeQtip(\'1\');return false;';    //Tmp - Hardcoded - Need to change the Id    
               }else{
                   $cancelAttributes['onclick'] = 'closeCustomAttributeQtip(\'0\');return false;';    //Tmp - Hardcoded - Need to change the Id
               }*/
               
                $cancelAttributes['onclick'] = 'closeCustomAttributeQtip(\'1\');return false;'; 
               
            }else{
              // $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';  
              // $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(\'#root-admin\').data(\'narrowsearch\').refreshGrid();return false;';  
               $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(\'#root-admin\').data(\'narrowsearch\').refreshLastAccessedRow();return false;';
            }    
            
            
            
            $cancelLabel  = t('LBL123'); // Close
        //}else{
            //$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
            //$cancelLabel  = t('LBL109'); // Cancel
            
                    //}

        $cancelAttributes['tabindex'][] = '800';
        $courseStatus     = '';
        $CourseDetails;
        $courseStatus;
        $ifCourseValid;
        $resultCnt;
        $ifCourseUnpublish;
        $isMultiAction=0;
        if($cus_attr_id){
            if($subStage == 'attribute' || empty($subStage)) {
                if (!empty($cus_attr_id)) {
                /*  $CourseDetails   = getCourseDetail($courseId, array('id,status,title'));
                    $courseStatus    =  $CourseDetails[0]->crs_status; */
                }
                /*$ifCourseValid = getIfCourseValid($courseId);
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
                }*/
                
                $isMultiAction=1; //tmp
                
                
         
                if (!empty($cus_attr_id)) {
                    // $attrDetails   = getCustomAttributeDetail($cus_attr_id, array('id,cattr_status,cattr_name'));
                     $attrDetails   = getCustomAttributeDetail($cus_attr_id);
                     $attrStatus    =  $attrDetails[0]->cattr_status;
                }
                 
                $ifAttrUnpublish = getIfAttributeeUnpublished($cus_attr_id); 
                if($attrStatus != 'cre_cattr_sts_atv'){
                    $isMultiAction=1;
                }
                if($ifAttrUnpublish == 0){
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
        '#name' => 'cancelcustomattribute',
        '#attributes' => $cancelAttributes,
        );

        if (!empty($cus_attr_id)) { // Cancel button instead of Close is shown only when $courseId is not empty (see above). Cancel button is ajax enabled.
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
        $saveAttributes['tabindex'][] = '700';
        $eSignatureAvailable = ($subStage == 'Attachment') ? false : true; 

         if($subStage == 'attribute' || empty($subStage)) {
                                /*$courseStatus     = '';
                                 if (!empty($cus_attr_id)) {
                                 $CourseDetails   = getCourseDetail($courseId, array('id,status,title'));
                                 $courseStatus    =  $CourseDetails[0]->crs_status;
                                 }
                                 if($courseId){
                                 $ifCourseValid = getIfCourseValid($courseId);
                                 }*/
                                 
                                 $attrStatus  = '';
                                 if (!empty($cus_attr_id)) {
                                     //$attrDetails   = getCustomAttributeDetail($cus_attr_id, array('id,cattr_status,cattr_name'));
                                     $attrDetails   = getCustomAttributeDetail($cus_attr_id);
                                     $attrStatus    =  $attrDetails[0]->cattr_status;
                                 }
                                /* if($courseId){
                                 $ifCourseValid = getIfCourseValid($courseId);
                                 }*/
                                 
                                $isSaveParent = 0;
                               $click_str=' onclick="displayPubActionList(\'crs-pub-save-btn\')"';
                                $suffix_str='<span id="pub-unpub-action-btn"'.$click_str.' class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>'.'</div>';
                                $disp_class=' class="catalog-pub-add-list crs-pub-save-btn"';
                                if($form_state['storage']['from_shortcut']=='1'){
                                    $click_str='';
                                    $suffix_str='<div class="admin-save-button-right-bg"></div>';
                                    $disp_class=' class="catalog-pub-add-list-disable crs-pub-save-btn-disable" style="display:none;"';
                                } 
                                
                                if(empty($cus_attr_id) || $attrStatus != 'cre_cattr_sts_atv'){ 
                                        $isSaveParent = 1;
                                        $saveAttributes['onclick'] = array();
                                        //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                                        $saveAttributes['onclick'][] = 'increasePopWidth();';
                                        $saveAttributes['class'][] = (($cus_attr_id) ? 'admin-save-button-dropdown-middle-bg-wrapper' : '');
                                        // $saveAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
                                        $form[$saveButtonId] = array(
                                                '#type' => 'submit',
                                                '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>',
                                                    // Commented For Course save and Publish By Default #0036253.
                                                    // '#suffix' => (($courseId) ? '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>' : '<div class="admin-save-button-right-bg"></div>').'</div>',
                                              
                                              
                                                //'#suffix' => ( '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>').'</div>',
                                                '#suffix' =>$suffix_str,
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
                                    '#markup' => '<ul '.$disp_class.'>', 
                                    ); //  catalog-save-btn
                                    $savePublishPrefix = '<li class="save-pub-unpub-sub-menu">';
                                    $savePublishSuffix = '</li>';
                                }else{
                                    //$savePublishAttributes['class'][] = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('admin-save-button-dropdown-middle-bg-wrapper') : '');
                                    $savePublishAttributes['class'][] = 'admin-save-button-dropdown-middle-bg-wrapper';
                                    $savePublishPrefix = '<div class="admin-save-pub-unpub-button-container"><div class="admin-save-button-left-bg"></div>';
                                    //$savePublishSuffix = ((($ifCourseUnpublish == 0) && ($courseStatus == 'lrn_crs_sts_atv')) ? ('<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>') : '<div class="admin-save-button-right-bg"></div>' ).'</div>';
                                    //$savePublishSuffix = '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'crs-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>'.'</div>';
                                    $savePublishSuffix = '<span id="pub-unpub-action-btn"'.$click_str.' class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>'.'</div>';
                    
                                }
                    
                                // The If has been removed. refer #0029450
                                // if($ifCourseValid[0]->sesavailable != 0){
                                $saveButtonId = 'catalog-course-save-publish';
                                $savePublishAttributes['onclick'] = array();
                                $savePublishAttributes['tabindex'][] = '700';
                    
                                // $savePublishAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
                                $savePublishAttributes['onclick'][] = 'increasePopWidth();';
                                $savePublishAttributes['class'][] = 'addedit-form-expertusone-throbber';
                                $savePublishAttributes['data-wrapperid'] = array($wrapperId);
                                // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
                                $buttonLabel = ($attrStatus == 'cre_cattr_sts_atv') ? t('LBL141') : t('LBL924');
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
														                                '#markup' => '<ul '.$disp_class.'>',
                                                );
                                            }
                    
                                            // <span onclick="displayPubActionList()" class="resource-add-action-wrapper">
                    
                    
                                            if(($cus_attr_id) && $attrStatus == 'cre_cattr_sts_atv'){
                                                if($ifCourseUnpublish == 0){
                                                    $saveButtonId = 'customattribute-save-unpublish';
                                                    $saveUnPublishAttributes['onclick'] = array();
                                                    //$saveAttributes['onclick'][] = 'hideMessageInfo();';
                                                    $saveUnPublishAttributes['onclick'][] = 'increasePopWidth();';
                                                    // $savePublishAttributes['onclick'][] = '$("#'.$customHiddenButtonId.'").click();';
                                                    $form[$saveButtonId] = array(
                                                                                '#type' => 'submit',
                                                                                '#prefix' => '<li class="save-pub-unpub-sub-menu">',
                                                                                '#suffix' => '</li>',
                                                                                '#value' => t('LBL920'),
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
                                            
                                            $deleted_eligible=0;
                                
                                            if($cus_attr_id!=''){
                                                $deleted_eligible=getCustomAttributeDeleteAccess($cus_attr_id); 
                                            }
                                            
                                            
                                            $deleteCheck = adminVisibilityForAddEditDelete($cus_attr_id,'cre_sys_obt_cattr','cre_sys_obt_cattr','slt_custom_attr'); 
                                            if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
                                                if(($cus_attr_id)){
                                                    if($resultCnt == 0 && $deleted_eligible!=1){
                                                            $displayTitle = titleController(' ', $attrDetails[0]->cattr_name,34);
                                                            $saveButtonId = 'customattribute-delete';
                        
                                                            $deleteAttributes['onclick'] = array();                                       
                                                            //$deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('LBL2024')).' '.escape_string($attrDetails[0]->cattr_name) . '","'.core_encrypt(cus_attr_id).'","Custom"); return false;';                                                           
                                                            $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('LBL2015')).' '.'\"'. escape_string($attrDetails[0]->cattr_name) .'\"'. '","'.trim($cus_attr_id).'","Custom"); return false;';
                                                            
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
                   

                    //Temporary Hide
                    $form['addedit-left-col-extend-func'] = array(
       
                    );
 

                    $pngPath = base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/spacer.gif'; 
                   

                    $form['addedit-left-col-end'] = array(
                                        '#type' => 'markup',
                                        '#markup' => '</div><div class="clearBoth"></div>', 
                    );
        } catch (Exception $ex) {
            watchdog_exception('addEditFormWithCourseCancelAndSaveClose', $ex);
            expertusErrorThrow($ex);
        }
    }




function list_custom_attribute_forms(&$form, &$form_state, $defaultValue){
    try {
        $form['customattribute_actions_disp'] = array(
          '#type'       => 'markup',
          '#prefix'     => '<div class="customattribute-basic-addedit-action-disp" id="customattribute-basic-addedit-action-disp">',
        );

        if(!isset($form_state['storage']['substep'])){
            $form_state['storage']['substep'] = '';
        }
        
        $isSelected = ($form_state['storage']['substep'] == 'attribute' || empty($form_state['storage']['substep'])) ? 'selected' : '';
        
        $form['edit_btn_details'] = array(
          '#type'       => 'submit',
          '#value'      => t('LBL2014'),
          '#name'       => 'attribute',
          '#prefix'     => '<ul><li>',
          '#suffix'     => '</li>',
          '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'customattribute-basic-addedit-form'),
                 '#ajax' => array(
                     'callback' => 'addEditRepaintForm',
                     'wrapper'  => 'customattribute-basic-addedit-form',
                     'event'       => 'click'
                   ),
           );
           $entityId       = $form['mapped_cus_attr_id']['#value'];
            
           if($entityId){
             
                    $isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';
               
                   /* $form['edit_btn_prerequest'] = array(
                        '#type'       => 'submit',
                        '#value'      => t('LBL230'),
                        '#name'       => 'prerequisite',
                        '#prefix'     => '<li>',
                        '#suffix'     => '</li>',
                        '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'customattribute-basic-addedit-form'),
                        '#ajax' => array(
                          'callback' => 'addEditRepaintForm',
                          'wrapper'  => 'customattribute-basic-addedit-form',
                          'event'      => 'click'
                          ),
                    );*/
                      
                    $isSelected = ($form_state['storage']['substep'] == 'screen') ? 'highlighted' : '';
                    $form['edit_btn_screen'] = array(
                        '#type'       => 'submit',
                        '#value'      => t('LBL2019'),
                        '#name'       => 'screen',
                        '#prefix'     => '<li>',
                        '#suffix'     => '</li>',
                        '#attributes' => array('Class' => array('form-submit addedit-form-expertusone-throbber',$isSelected), 'data-wrapperid' => 'customattribute-basic-addedit-form'),
                        '#ajax' => array(
                          'callback' => 'addEditRepaintForm',
                          'wrapper'  => 'customattribute-basic-addedit-form',
                          'event'      => 'click'
                          ),
                     );
                      
            }
            else{
                    //$isSelected = ($form_state['storage']['substep'] == 'prerequisite') ? 'highlighted' : '';
                 /* $form['edit_btn_prerequest_disabled'] = array(
                            '#type'       => 'item',
                            '#title'      => t('LBL230'),
                            '#prefix'     => '<li><span class="deactive-links">',
                            '#suffix'     => '</span></li>'
                            );
                            */
                            
                  $form['edit_btn_screen'] = array(
                                                '#type'       => 'item',
                                                '#title'      => t('LBL2019'),
                                                '#prefix'     => '<li><span class="deactive-links">',
                                                '#suffix'     => '</span></li>',
                                                );
    
                  $form['customattribute_actions_ul_container_disp_end'] = array(
                                                                           '#type'       => 'markup',
                                                                           '#suffix'     => '</ul>',
                      );
            }
            $form['customattribute_actions_disp_end'] = array(
                                                     '#type'       => 'markup',
                                                     '#suffix'     => '</div>',
                                                        );
    } catch (Exception $ex) {
        watchdog_exception('list_custom_attribute_forms', $ex);
        expertusErrorThrow($ex);
    }
}




/* Form Creation End */
?>
