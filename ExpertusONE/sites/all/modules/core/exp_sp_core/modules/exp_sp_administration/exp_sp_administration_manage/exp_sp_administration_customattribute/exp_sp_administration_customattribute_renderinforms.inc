<?php
/*
 * validateCustomAttributesInAddEditForm() - To validate the custom attributes according to mandatory, Unique values and given length, DataType
 */ 

function validateCustomAttributesInAddEditForm(&$form, &$form_state,$entity_type){
    try{
        
        expDebug::dprint('$form  = '.print_r($form,true),5);
        expDebug::dprint('$form_state[values]  = '.print_r($form_state['values'],true),5);
        expDebug::dprint('$form_state[input]  = '.print_r($form_state['input'],true),5); 
        expDebug::dprint('$entity_type = '.$entity_type,5);   
       
         
        $attr_det_arr=getCustomAttributeDetails($entity_type);
        if(count($attr_det_arr)>0){ //If custom attributes available for that entity
            foreach($attr_det_arr as $attr_det_key => $attr_det_obj){

            	
                     if($entity_type=='cre_sys_obt_ord'){ //if order entity , then take from billing pane to update in form state values
                         $form_state['values'][$attr_det_obj->cattr_code]=$form_state['values']['panes']['billing'][$attr_det_obj->cattr_code];
                         expDebug::dprint('$form_state[values]  for order = '.print_r($form_state['values'],true),5);
                     }
                  
                        
                 $error='0';  
                  
                //validation for text box
                if($attr_det_obj->cattr_type=='cattr_type_txtbox'){  
                    $error=validateTextBoxCustomAttribute($form, $form_state,$entity_type,$attr_det_obj);    
                }else if($attr_det_obj->cattr_type=='cattr_type_txtarea'){ // if textarea
                    $error=validateTextAreaCustomAttribute($form, $form_state,$attr_det_obj,$entity_type);  
                }else if($attr_det_obj->cattr_type=='cattr_type_checkbox' || $attr_det_obj->cattr_type=='cattr_type_radio' || $attr_det_obj->cattr_type=='cattr_type_dropdown'){ // if Checkbox /Radio/Drop Down button 
                    $error=validateCheckBoxRadioDropDownCustomAttribute($form, $form_state,$attr_det_obj,$entity_type); 
                }else if($attr_det_obj->cattr_type=='cattr_type_date'){ // if date
                    $error=validateDateCustomAttribute($form, $form_state,$attr_det_obj);  
                }  
                
                if($error=='1'){
                    $form_state["storage"]["form_validate"] = false;
                    $form_state['esign_show_popup']= false;     
                }   
                
                
            }
        }
          
    }catch (Exception $ex) {
        watchdog_exception('validateCustomAttributesInAddEditForm', $ex);
        expertusErrorThrow($ex);
   }
}
 

/*
 *  validateCheckBoxRadioDDCustomAttribute() - To Validate the Checkbox /Radio /DropDown attribute
 */

function validateCheckBoxRadioDropDownCustomAttribute(&$form, &$form_state,$attr_det_obj,$entity_type){
    try{
        
        $fieldName=$attr_det_obj->cattr_code;
        $tmp_fieldName=$attr_det_obj->cattr_code;
        $label_name=$attr_det_obj->cattr_name;
        //$fieldValue = trim($form_state['values'][$fieldName]); 
        $error='0';     
        
        
          if($entity_type=='cre_sys_obt_ord'){ //If order then need to change the id according to billing pane
             $fieldName='panes][billing]['.$attr_det_obj->cattr_code;
             $form[$fieldName]['#title']=$label_name;
          }  
                
          if($attr_det_obj->cattr_mandatory==1){ //if it is mandatory 
                 
                 
                 if(($attr_det_obj->cattr_type=='cattr_type_dropdown' && $attr_det_obj->cattr_multiple_opt!='1')){ //if single dropdown
                       $fieldValue = trim($form_state['values'][$tmp_fieldName]);  
                         if(empty($fieldValue)) {
                              expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' is missing.' , 5);
                              form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101'));
                              $error='1';    
                        }   
                      
                 }else if($attr_det_obj->cattr_type=='cattr_type_radio'){
                      $fieldValue = trim($form_state['values'][$tmp_fieldName]);  
                         if(empty($fieldValue)) {
                              expDebug::dPrint(' ' . $label_name . ' is missing.' , 5);
                              form_set_error($fieldName, $label_name. t('ERR101'));
                              $error='1';    
                        }   
                 }else{  
                    
                    // expDebug::dPrint('Input values ='.print_r($form_state['input'][$fieldName],true) , 5);
                    // expDebug::dPrint('Values values ='.print_r($form_state['values'][$fieldName],true) , 5);
                     
                     $truncated_empty_arr = array_filter($form_state['values'][$tmp_fieldName]);
                     expDebug::dPrint('$truncated_empty_arr ='.print_r($truncated_empty_arr,true) , 5);
                     
                     if(empty($truncated_empty_arr)){
                             expDebug::dPrint(' ' . $label_name . ' is missing.' , 5);
                            form_set_error($fieldName, $label_name . t('ERR101'));
                           $error='1';  
                     }
                 } 
                
            }  
          
           return $error;  
        
    }catch (Exception $ex) {
        watchdog_exception('validateCheckBoxRadioCustomAttribute', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 *  validateDateCustomAttribute() - To Validate the Date Custom attribute 
 */

function validateDateCustomAttribute(&$form, &$form_state,$attr_det_obj){
    try{
         
        $fieldName=$attr_det_obj->cattr_code;
        $fieldValue = trim($form_state['values'][$fieldName]); 
        $error='0';     
                
          if($attr_det_obj->cattr_mandatory==1){ //if it is mandatory 
                 
                if(empty($fieldValue) ||  strtolower(trim($fieldValue)=='mm-dd-yyyy')) {
                      expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' is missing.' , 5);
                      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101'));
                      $error='1';    
                } 
                
            }  
            
             return $error; 
        
    }catch (Exception $ex) {
        watchdog_exception('validateDateCustomAttribute', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 *  validateTextAreaCustomAttribute() - To Validate the TextArea attribute 
 */

function validateTextAreaCustomAttribute(&$form, &$form_state,$attr_det_obj,$entity_type){
    try{
        
        $fieldName=$attr_det_obj->cattr_code;
        $label_name=$attr_det_obj->cattr_name;
        
        $fieldValue = trim($form_state['values'][$fieldName]); 
        $error='0';     
                
            if($entity_type=='cre_sys_obt_ord'){ //If order
             $fieldName='panes][billing]['.$attr_det_obj->cattr_code;
             $form[$fieldName]['#title']=$label_name;
          }  
                 
          if($attr_det_obj->cattr_mandatory==1){ //if it is mandatory 
                 
                if(empty($fieldValue) ||  strtolower(trim($attr_det_obj->cattr_help_txt))==strtolower(trim($fieldValue))) {
                      expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' is missing.' , 5);
                      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101'));
                      $error='1';    
                } 
                
            } 
          
          if(!empty($fieldValue) && strlen($fieldValue)>$attr_det_obj->cattr_length && strtolower(trim($attr_det_obj->cattr_help_txt))!=strtolower(trim($fieldValue))){ //Check no of characters 
                 form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR350',array('@chars'=>$attr_det_obj->cattr_length)));
                  $error='1'; 
          }  
           
          return $error; 
        
        
    }catch (Exception $ex) {
        watchdog_exception('validateTextAreaCustomAttribute', $ex);
        expertusErrorThrow($ex);
    }
}


/*
 *  validateTextBoxCustomAttribute() - Validate Text Box attribute
 */

function validateTextBoxCustomAttribute(&$form, &$form_state,$entity_type,$attr_det_obj){
    try{
        
        $fieldName=$attr_det_obj->cattr_code;
        $label_name=$attr_det_obj->cattr_name;
        
        $fieldValue = trim($form_state['values'][$fieldName]);  
        
        $error='0';   
        
         if($entity_type=='cre_sys_obt_ord'){ //If order
             $fieldName='panes][billing]['.$attr_det_obj->cattr_code;
             $form[$fieldName]['#title']=$label_name;
          }  
                
          if($attr_det_obj->cattr_mandatory==1){ //if it is mandatory 
                 
                if(empty($fieldValue) ||  strtolower(trim($attr_det_obj->cattr_help_txt))==strtolower(trim($fieldValue))) {
                      expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' is missing.' , 5);
                      form_set_error($fieldName, $form[$fieldName]['#title'] . t('ERR101'));
                     // form_set_error('panes][billing][c_order_textbox', $form[$fieldName]['#title'] . t('ERR101')); 
                      $error='1';    
                } 
                
            }
            
            if($attr_det_obj->cattr_unique==1){ //if it is unique value 
            	
            	$upd_id=$form_state['values']['id']; 
            	
            	if($entity_type=='cre_sys_obt_loc'){
            		$upd_id=$form_state['values']['loc_id'];
            	}else if($entity_type=='cre_sys_obt_crs'){
            		$upd_id=$form_state['values']['crs_id'];
            	}
            	
                 
                if(!empty($fieldValue) && strtolower(trim($attr_det_obj->cattr_help_txt))!=strtolower(trim($fieldValue))) {
                        if (checkUniqueOrNot($fieldValue,$attr_det_obj->entity_ref_tbl_col,$entity_type,$upd_id,'1')) {
                                  expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' already exists.' , 5);
                                  form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR355'));
                                  $error='1';    
                        }
                } 
                
           }
            
           if($attr_det_obj->cattr_datatype=='cattr_dtype_numbers'){ //if data type is numbers
              
               if(!empty($fieldValue) && strtolower(trim($attr_det_obj->cattr_help_txt))!=strtolower(trim($fieldValue))) { 
                   
                   
                   $last_char=substr($fieldValue, -1); //if last character is . then raise the error 
                   
                   if(!is_numeric($fieldValue) || $last_char=='.'){
                                  expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' has a non-numeric value.' , 5);
                                  //form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR201'));
                                  form_set_error($fieldName, t('ERR097') .' '. $form[$fieldName]['#title']);
                                  $error='1';    
                                  
                   }else{ //if it is numeric  
                                
                                /*$last_char=substr($fieldValue, -1); //if last character is . then raise the error
                                if($last_char=='.'){
                                     expDebug::dPrint(' ' . $form[$fieldName]['#title'] . ' has a non-numeric value.' , 5);
                                     form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR201'));
                                     $error='1';     
                                }*/
                                
                                $tmp_field_value=str_replace('.','',$fieldValue);  //Check no of characters except .dot
                                if(strlen($tmp_field_value)>$attr_det_obj->cattr_length){ 
                                    
                                         form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR357',array('@count'=>$attr_det_obj->cattr_length))); 
                                         $error='1'; 
                                } 
                       
                   }
                   
                   
               }
               
               
           }else{ //if data type is 
           
                if(!empty($fieldValue) && strtolower(trim($attr_det_obj->cattr_help_txt))!=strtolower(trim($fieldValue))) {
                    
                    if(strlen($fieldValue)>$attr_det_obj->cattr_length){ //Check no of characters 
                         form_set_error($fieldName, $form[$fieldName]['#title'] .' '. t('ERR350',array('@chars'=>$attr_det_obj->cattr_length)));
                          $error='1'; 
                    }
                    
                } 
            
           }
           
            return $error; 
        
    }catch (Exception $ex) {
        watchdog_exception('validateTextBoxCustomAttribute', $ex);
        expertusErrorThrow($ex);
    }
}

/*
 *  checkUniqueOrNot() - To check that whether custom attribute data unique or not. if it is unique then return true else false
 */
 
function checkUniqueOrNot($field_val,$field_ref_col,$entity_type,$entity_id='',$lower_case_check='1'){
    try{ 
         
            $tbl_arr=getEntityTableInformation($entity_type); 
            
            $tbl_name=$tbl_arr['table_name'];
            $del_status=$tbl_arr['del_status'];
        
            $select = db_select($tbl_name, 'tbl');
            $select->addExpression('COUNT(*)', 'count');
            if($lower_case_check=='1'){
                $tmp_fld_str='LOWER(tbl.'.$field_ref_col.')=\''.strtolower(addslashes($field_val)).'\'';
                $select->where($tmp_fld_str);   
            }else{
               $select->condition('tbl.'.$field_ref_col, $field_val, '=');  
            }
            
            if($entity_type!='cre_sys_obt_ord'){ //if it is not the order entity
               $select->condition('tbl.'.'status',$del_status,'!=');
            }
            
            if (!empty($entity_id) && $entity_id > 0 ) {
              $select->condition('tbl.'.'id', $entity_id, '!=');
            }

            expDebug::dPrintDBAPI('$select -  ', $select);

            $result = $select->execute()->fetchField();
            
            expDebug::dPrint('$result='.print_r($result,true),5);
                 
            return (($result > 0)? true : false);  
        
    }catch (Exception $ex) {
        watchdog_exception('checkUniqueOrNot', $ex);
        expertusErrorThrow($ex);
  }
}
/*
 * addCustomCheckBoxRadioFieldToAddEditForm() - It paints the checkbox and radio button control in Form  
*/   
function addCustomCheckBoxRadioFieldToAddEditForm($button_type='checkbox',&$form,$cattr_code,$cattr_name,$cattr_options,$defaultValue,$tab_index, $cattr_mand,$read_only=false,$display_position='H',$sep_row='0',$col_position='0',$entity_type='',$info_symbol_arr=array()){
    try{
        
           expDebug::dPrint('$entity_type = '.$entity_type,5);
           expDebug::dPrint('$button_type='.$button_type,5);    
           expDebug::dPrint('$defaultValue='.print_r($defaultValue,true),5);            
           $defaultValueArr=!empty($defaultValue) ? explode(',',$defaultValue) : ''; 
           expDebug::dPrint('$defaultValueArr='.print_r($defaultValueArr,true),5);
          
          $checkRadiobuttonlAttributes=array();
          
          $check_options=$cattr_options;  
          
          $check_box_code=$cattr_code;
        //  $check_box_code_arr=$check_box_code.'[]';
          $attr_name=$cattr_name; 
          
          //$option_count=count($check_options);
         // $check_input_str='';
          
          $display_cls='';
          if($display_position=='V'){
            $no_of_columns='1'; 
           // $checkRadiobuttonlAttributes['class'] = array('customattribute-display-vertical');
            $display_cls='customattribute-display-vertical';
          }else{
            
            //$checkRadiobuttonlAttributes['class'] = array('customattribute-display-horizontal');
              $display_cls='customattribute-display-horizontal';
            if($sep_row=='1'){ //If need separate row to render set no . of option here manually 
                 if($entity_type=='cre_usr'){
                      $no_of_columns='100000';  
                 }
                
            }else{
              $no_of_columns='2';   
            }
            
          }  
                      
           if($button_type=='checkbox'){
              $checkRadiobuttonlAttributes['onclick'] = 'checkboxSelectedUnselectedCommon(this);';   
           }         
           
           //Information details
          $infoSymbol='';
          if(isset($info_symbol_arr) && !empty($info_symbol_arr)){ 
                 $infoSymbol='<span id="'.$info_symbol_arr['span_id'].'">
                              <img src="'.$info_symbol_arr['image_path'].'" class="'.$info_symbol_arr['class_name'].'" title="'.$info_symbol_arr['msg'].'" onload="vtip();">
                            </span>';
          }
                 
          //Mandatory Symbol
          $mandSymbol='';
          if(isset($cattr_mand) && trim($cattr_mand)!='' && $cattr_mand==1){ 
            $mandSymbol='<span class="addedit-mandatory">*</span>';
          }
          
          //$total_width_per='98'; //98%
          //$per_col_width=(int)$total_width_per/$no_of_columns;
          
          /*$table_style="display: table; border-collapse: separate; border-spacing: 1px;width:$total_width_per%;";
          $row_style="display:table-row; width:100%;";
          $col_style="display:table-cell;padding-left:0px;background-color: none;width:$per_col_width%;";
           */
            
          /*  
          $table_style="width:$total_width_per%;";
          $row_style="";
          $col_style="display:inline;padding-right:10px;background-color: none;"; 
          
              if($option_count!='' && $option_count>0){   
                         $check_input_str.="<div class='table' style='$table_style'><div class='row' style='$row_style'>";
                         $label_str="custom_label_".$button_type;
                         
                          $i=1;
                         foreach($check_options as $check_key=>$check_val){
                             
                             
                            
                            
                             
                              
                            
                            
                            $check_input_str.=" <div class='cell' style='$col_style'>";
                            $check_val='<span id=\''.$label_str.'\'>'.$check_val.'</span>'; 
                            
                            $check_str='';
                            $check_sel_cls='checkbox-unselected';
                            if($defaultValueArr!=''){
                                if(in_array($check_key,$defaultValueArr)){
                                    $check_str=' checked';
                                    $check_sel_cls='checkbox-selected';
                                }
                                
                            } 
                            
                            if($button_type=='checkbox'){ //Checkbox
                               $check_input_str.="<div id='entity-wrapper'> <span class='".$check_sel_cls."'><input type='checkbox' name='".$check_box_code_arr."' onclick='checkboxSelectedUnselectedCommon(this);' value='".$check_key."' id='".$check_key."' $check_str></span>".$check_val."</div>";
                            }else{ //or Radio button
                               $check_input_str.=" <input type='radio' name='".$check_box_code."' value='".$check_key."' id='".$check_key."' $check_str>".$check_val;   
                            }
                            
                            
                            $check_input_str.="</div>"; 
                            
                            if($i%$no_of_columns == 0){
                                $check_input_str.="</div><div class='row' style='$row_style'>";
                            }
                            $i++;
                            
                            
                            
                         }
                        
                         $check_input_str.="</div></div>";
              }*/ 
                    
                
                    
            if($sep_row!='1'){ //If double column in a Row 
                        
                            if($col_position=='2'){ // If place the second coloumn
                $div_field_lable = 'customcheckboxradio';
                
                $form [$check_box_code . '_chk_sec_col_start'] = array (
                        '#type' => 'markup',
                        '#markup' => '<div class="addedit-twocol-secondcol" id="' . $div_field_lable . '_checkbox_second_col">
                                                                                <div class="addedit-new-field-title test"><span class="addedit-label">' . $attr_name . ':</span>' . $mandSymbol . $infoSymbol . '</div>
                                                                                    <div class="addedit-secondcol-field-value">' 
                );
            } else if ($col_position == '1') { // If place the first column
                
                $div_field_lable = 'customcheckboxradio';
                
                $form [$check_box_code . '_chk_sec_col_start'] = array (
                        '#type' => 'markup',
                        '#markup' => '<div class="addedit-twocol-firstcol" id="' . $div_field_lable . '_checkbox_first_col">
                                                                                <div class="addedit-new-field-title test"><span class="addedit-label">' . $attr_name . ':</span>' . $mandSymbol . $infoSymbol . '</div>
                                                                                    <div class="addedit-firstcol-field-value">' 
                );
            } 
                
                    
                          /*$form[$check_box_code] = array(
                              '#title' => t(''), 
                              '#type' => 'checkboxes',
                              '#description' => t(''),
                              '#options' => $check_options,
                              '#default_value' => $defaultValueArr,
                              '#attributes' => $checkRadiobuttonlAttributes,  
                              '#theme_wrappers' => array('exp_sp_administration_customattribute_form_element'), 
                            );*/
                            
                               if($button_type=='checkbox'){
                                   $form[$check_box_code] = array( 
                                     '#prefix' => '<div id="'.$display_cls.'">' ,
                                      '#suffix' => '</div>',
                                      '#type' => 'checkboxes', 
                                      '#options' => $check_options,
                                      '#default_value' => $defaultValueArr,
                                      '#attributes' => $checkRadiobuttonlAttributes, 
                                      '#theme_wrappers' => array('exp_sp_administration_customattribute_form_element'), 
                                    );  
                               }else if($button_type=='radio'){ 
                                   $form[$check_box_code] = array( 
                                      '#prefix' => '<div id="'.$display_cls.'">' ,
                                      '#suffix' => '</div>',
                                      '#type' => 'radios', 
                                      '#options' => $check_options,
                                      '#default_value' => $defaultValue,
                                      '#attributes' => $checkRadiobuttonlAttributes,
                                    );  
                                   
                               }
                            
                            
                            
                
                            /* $form[$check_box_code] = array( 
                                                '#type' => 'markup',
                                                '#prefix' => $check_input_str,
                                            ); */
                                            
                             $form[$check_box_code.'_chk_sec_col_end']  = array(
                                                                '#type' => 'markup',
                                                                '#markup' => '</div>
                                                                               <div class="clearBoth"></div>
                                                                                </div>'
                                                                );  
                
                
            }else{ // Render control in Single row  
                         $field_width='width:98%'; 
                        if($entity_type=='cre_usr'){
                                $field_width='width:98%'; 
                                $field_inner_width='width:78%'; 
                                $div_field_lable='user';
                       }else if($entity_type=='cre_sys_obt_crs'){                              
                                $field_inner_width='width:74%'; 
                                $div_field_lable='course';
                       }else if($entity_type=='cre_sys_obt_cls'){                              
                                $field_inner_width='width:68%'; 
                                $div_field_lable='class';
                       }else if($entity_type=='cre_sys_obt_trp'){                              
                                $field_inner_width='width:68%'; 
                                $div_field_lable='tp';
                       }else if($entity_type=='cre_org'){                              
                                //$field_inner_width='width:74%'; 
                                $div_field_lable='org';
                       }else if($entity_type=='cre_sys_obt_cnt'){                              
                                $field_inner_width='width:68%'; 
                                $div_field_lable='content';
                       }else if($entity_type=='cre_sys_obt_loc'){                              
                                //$field_inner_width='width:75%'; 
                                $div_field_lable='location';
                       }                        
                                                                       
                        
                        $form[$check_box_code.'_chk_full_row_start'] = array(
                        		'#type' => 'markup',
                        		'#markup' => '<div class="addedit-new-field" id="'.$div_field_lable.'_checkbox_full_row" style="'.$field_width.'">
                                                                                <div class="addedit-new-field-title test">'.$attr_name.':'.$mandSymbol.$infoSymbol.'</div>
                                                                                    <div style="'.$field_inner_width.'" class="addedit-new-field-value">'
                        );
                                                      
                         /*$form[$check_box_code] = array(
                                                '#type' => 'markup',
                                                //'#prefix' => $check_input_str,
                                            );
                                            
                                             
                      */
                          
                        
                         
                         
                              //$checkboxPrefix = ($smsChk == 1) ? '<div class="checkbox-selected">' : '<div class="checkbox-unselected">';
                              //$checkboxSuffix = '</div>'; 
                             //  $checkboxOnClick = 'checkboxSelectedUnselectedCommon(this);';
              
                         
                              if($button_type=='checkbox'){
                                   $form[$check_box_code] = array( 
                                      '#prefix' => '<div id="'.$display_cls.'">' ,
                                      '#suffix' => '</div>',
                                      '#type' => 'checkboxes', 
                                      '#options' => $check_options,
                                      '#default_value' => $defaultValueArr,
                                      '#attributes' => $checkRadiobuttonlAttributes, 
                                      '#theme_wrappers' => array('exp_sp_administration_customattribute_form_element'), 
                                    );  
                               }else if($button_type=='radio'){ 
                                   $form[$check_box_code] = array( 
                                      '#prefix' => '<div id="'.$display_cls.'">' ,
                                      '#suffix' => '</div>',
                                      '#type' => 'radios', 
                                      '#options' => $check_options,
                                      '#default_value' => $defaultValue,
                                      '#attributes' => $checkRadiobuttonlAttributes, 
                                    );  
                                   
                               }
                        
                                                        
                                            
                         $form[$check_box_code.'_chk_full_row_end']  = array(
                                                '#type' => 'markup',
                                                '#markup' => '</div>
                                                               <div class="clearBoth"></div>
                                                                </div>'
                                                );   
                                                
            }

            
           

 
        
    }catch (Exception $ex) {
        watchdog_exception('addCustomCheckBoxFieldToAddEditForm', $ex);
        expertusErrorThrow($ex);
  }
}

/*
 *  renderCustomAttributeFormControls() - To render / paint the form controls like TextBox,Text Area,Checkbox single,multiple, Radiobutton etc.. according to entity type
 *
*/
function renderCustomAttributeFormControls(&$form,&$form_state,$cattr_obj,$tab_index='',$entity_type='',$column_count='0',$attr_default_form_values_arr){
      try{
        
        expDebug::dPrint(' $cattr_obj ->  ' . print_r($cattr_obj, true) ,5);        
        
        $cattr_name=trim($cattr_obj->cattr_name);
        $cattr_code=trim($cattr_obj->cattr_code);
        $cattr_mand=(trim($cattr_obj->cattr_mandatory)=='1')? true : false;
        $cattr_help_txt=trim($cattr_obj->cattr_help_txt); 
        $cattr_length=trim($cattr_obj->cattr_length); 
        $info_symbol_arr=array();
        
        if($cattr_help_txt!=''){
              $path = base_path() . drupal_get_path('module', 'uc_credit').'/images/info.png';  
              $info_symbol_arr['span_id']='vtip_custom_attr';
              $info_symbol_arr['image_path']=$path;
              //$info_symbol_arr['class_name']='vtip cus_attr_vtip';
              $info_symbol_arr['class_name']='vtip info-enr-upload';
              $info_symbol_arr['msg']=$cattr_help_txt; // For Title
        }         

        //Load Default values
        $defaultValue =  getDefaultAddEditFormFieldValue($cattr_code, $form_state, $attr_default_form_values_arr);   
        
        if($cattr_obj->cattr_type=='cattr_type_txtbox'){ // For Textbox control
            
            // Set default place holder text     
            if($cattr_help_txt!=''){
              $attrDet['data-default-text']=$cattr_help_txt;  
            }else{
              $attrDet['data-default-text']=''; 
            }
            
            if($cattr_length==''){
                $cattr_length=255;
            } 
             
            addTextFieldToAddEditForm($form, $cattr_code, $cattr_name, $defaultValue, $tab_index,$cattr_mand,false,$cattr_length,'60','',$attrDet,$info_symbol_arr);
            
            $form[$cattr_code]['#from_customattribute']='1';
            
        }else if($cattr_obj->cattr_type=='cattr_type_dropdown'){  // For DropDown Control
              
              $cattr_opts=trim($cattr_obj->cattr_opts);
              $cattr_opt_codes=trim($cattr_obj->cattr_opt_codes);
              $cattr_options_name_arr=explode(",",$cattr_opts);
              $cattr_options_code_arr=explode(",",$cattr_opt_codes);
              $dd_opt_arr=array();
              
              
              
              $config=getConfig('exp_sp');
              if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
              {
                  include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
                  global $user;
                  $translatedLabels = getTranslationsForArrayStrings($cattr_options_name_arr,$user->language,"","custom_attribute_options");
                  expDebug::dPrint("ttttt after::".print_r($translatedLabels,true),3);
                  $cattr_options_name_arr_translated = array();
                  $translation_ids = array();
                  for($i = 0 ; $i < count($cattr_options_name_arr); $i++){
                      $cattr_options_name_arr_translated[]=$translatedLabels[$cattr_options_name_arr[$i]];
                  }
                  $cattr_options_name_arr = $cattr_options_name_arr_translated; 
              }
              
              if($cattr_options_name_arr>0){ 
                  $dd_opt_arr=array_combine($cattr_options_code_arr,$cattr_options_name_arr); 
              }  
              expDebug::dPrint(' $dd_opt_arr ->  ' . print_r($dd_opt_arr, true) ,5);   
              $defaultValue=!empty($defaultValue) ? explode(',',$defaultValue) : '';  
              expDebug::dPrint('Option Selected =  ' . print_r($defaultValue, true) ,5);  
              
              //$cattr_obj->cattr_multiple_opt='0';
              if($cattr_obj->cattr_multiple_opt=='1'){  //Multiple option selection
                    addMultiselectDropDownToAddEditForm($form, $cattr_code, $cattr_name, $dd_opt_arr, $defaultValue, $tab_index,$cattr_mand,false,'','',0,false,'',false,$info_symbol_arr,'customattr',$entity_type);
              }else{ //Single option selection
                    addDropDownToAddEditForm($form, $cattr_code, $cattr_name, $dd_opt_arr, $defaultValue, $tab_index,$cattr_mand,false,'','',$info_symbol_arr);  //Single Dropdown
              } 
        }else if($cattr_obj->cattr_type=='cattr_type_date'){ // For Date Control
             
             $form['date-start'] = array(
                                                                          '#type'   => 'markup',
                                                                          '#markup' => '<div class="date-container" style="display:inline;">',
                                                                      );
                                                                      
              addDateFieldToAddEditForm($form, $cattr_code, $cattr_name, $defaultValue, t('LBL112'), 'all', t('LBL218'), $tab_index,$cattr_mand,false,'10','40',$info_symbol_arr);
             $form['date-end'] = array(
                                                                          '#type'   => 'markup',
                                                                          '#markup' => '</div>',
                                                                           );
            
        }else if($cattr_obj->cattr_type=='cattr_type_txtarea'){ //For Text Area Control
                 
                $cattr_length=trim($cattr_obj->cattr_length);
                if($cattr_length==''){
                    $cattr_length=500;
                }
                
                // Set default place holder text     
                if($cattr_help_txt!=''){
                  $attrDet['data-default-text']=$cattr_help_txt;  
                }else{
                  $attrDet['data-default-text']='';  
                } 
             
             
               $form[$cattr_code.'_chk_full_row_start'] = array(
                                                            '#type' => 'markup',
                                                            '#markup' => '<div id="'.$cattr_code.'_textarea_full_row">' 
                                                        );  
                                                        
                $form['addedit-long-description-container-start'.'-'.$cattr_code] = array(
                                                                          '#type'   => 'markup',
                                                                          '#markup' => '<div class="addedit-long-description-container">',
                                                                      );
              //addTextFormatFieldToAddEditForm($form, $cattr_code, $cattr_name, $defaultValue, 'full_html', $tab_index, $cattr_mand,false,$cattr_length); 
             // addTextAreaFieldToAddEditForm($form, $cattr_code,$cattr_name, $defaultValue, $tab_index,$cattr_mand,false,$info_symbol_arr);
             
                addHiddenFieldToAddEditForm($form, 'ckeditor_word_count', $cattr_length);
                //addTextAreaFieldForShortDescription(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $charLimit = 1000, $defaultText = '') {
                addTextAreaFieldForShortDescription($form, $cattr_code, $cattr_name, $defaultValue, $tab_index, $cattr_mand, false, $cattr_length,$cattr_help_txt,$attrDet,$info_symbol_arr,'customattr');   
                //$form[$cattr_code]['#attributes']['data-empty-text'] = $cattr_help_txt; 
        
               $form[$cattr_code]['#from_customattribute']='1';
                
                $form['addedit-long-description-container-end'.'-'.$cattr_code] = array(
                                                                              '#type'   => 'markup',
                                                                              '#markup' => '</div>',
                                                                            ); 
        
        
                $form[$cattr_code.'_chk_full_row_end'] = array(
                                                            '#type' => 'markup',
                                                            '#markup' => '</div>'
                                                        );  
                                                        
                                                        
                                                        
        }else if($cattr_obj->cattr_type=='cattr_type_checkbox' || $cattr_obj->cattr_type=='cattr_type_radio'){ // For CheckBox and Radio Control
        
         
              $cattr_opt_count=trim($cattr_obj->cattr_opt_count);
              $cattr_display_opt=trim($cattr_obj->cattr_display_opt);
           
              $cattr_opts=trim($cattr_obj->cattr_opts);
              $cattr_opt_codes=trim($cattr_obj->cattr_opt_codes);
              $cattr_options_name_arr=explode(",",$cattr_opts);
              $cattr_options_code_arr=explode(",",$cattr_opt_codes);
              $dd_opt_arr=array();
              
              $config=getConfig('exp_sp');
              if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
              {
                  include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
                  global $user;
                  $translatedLabels = getTranslationsForArrayStrings($cattr_options_name_arr,$user->language,"","custom_attribute_options");
                  expDebug::dPrint("ttttt after::".print_r($translatedLabels,true),3);
                  $cattr_options_name_arr_translated = array();
                  for($i = 0; $i < count($cattr_options_name_arr);$i++){
                      $cattr_options_name_arr_translated[]=$translatedLabels[$cattr_options_name_arr[$i]];
                  }
                  $cattr_options_name_arr = $cattr_options_name_arr_translated;
              }
              
              if($cattr_options_name_arr>0){ 
                  $check_opt_arr=array_combine($cattr_options_code_arr,$cattr_options_name_arr); 
              }  
              expDebug::dPrint(' $check_opt_arr ->  ' . print_r($check_opt_arr, true) ,5);
              
              if($cattr_obj->cattr_type=='cattr_type_checkbox'){
                 $button_type='checkbox';
                  
                  //  $defaultValue='o_option_3,o_option_5'; 
                /* $tmp_values_arr=$form_state['input'][$cattr_code]; // For retain the value when form validation
                 if(!empty($tmp_values_arr)){
                       $defaultValue=implode(',',$tmp_values_arr);
                 }  */
              }else{ //if radio
                  $button_type='radio';
                   // $defaultValue='o_option_3';
                /*  if($form_state['input'][$cattr_code]!=''){
                    $defaultValue=$form_state['input'][$cattr_code]; // For retain the value when form validation
                    
                  
                  }*/
              } 
           
            if($cattr_display_opt=='' || is_null($cattr_display_opt)){ 
                 $cattr_display_opt='H'; // As default to place the options in Horizontal Positions 
            }   
            
          
           if($cattr_opt_count>2){ //Please place the checkbox control in  single row
           
                 $field_position_render='0';
                 addCustomCheckBoxRadioFieldToAddEditForm($button_type,$form,$cattr_code,$cattr_name,$check_opt_arr,$defaultValue,$tab_index, $cattr_mand,false,$cattr_display_opt,'1',$field_position_render,$entity_type,$info_symbol_arr);
           }else{ // //Please place the checkbox control in  first/second column row
                
               //Check column poistion whether need to render on first column or second column 
               if($column_count!='0' && $column_count>0){
                  $col_position=$column_count%2;
                  if($col_position==1){
                    $field_position_render='1'; // Render attribute in first field
                  }else{
                     $field_position_render='2'; // Render attribute in second field
                  }   
                  addCustomCheckBoxRadioFieldToAddEditForm($button_type,$form,$cattr_code,$cattr_name,$check_opt_arr,$defaultValue,$tab_index, $cattr_mand,false,$cattr_display_opt,'0',$field_position_render,$entity_type,$info_symbol_arr);
               } 
           }
        
        } 
        
      }catch (Exception $ex) {
            watchdog_exception('renderCustomAttributeFormControls', $ex);
            expertusErrorThrow($ex);
    }
    
}

/*
 * getCustomAttributeDetails() - get custom attributes details from DB according to entity type
 */
function getCustomAttributeDetails($entity_type,$shortcut='',$fromapi='') {
    try {
    	
    	expDebug::dPrint('$fromapi ' . print_r($fromapi, 1), 5);
    	expDebug::dPrint('$shortcut ' . print_r($shortcut, 1), 5);
        
            $selectStmt = db_select('slt_custom_attr', 'attr'); 
            $selectStmt->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id');
            $selectStmt->leftJoin('slt_custom_attr_options', 'attr_opt','attr_opt.cattr_id=attr.id'); 
            
            $selectStmt->addField('attr', 'id','cattr_id');
            $selectStmt->addField('attr', 'cattr_name','cattr_name');
            $selectStmt->addField('attr', 'cattr_code','cattr_code');
            $selectStmt->addField('attr', 'cattr_type','cattr_type');
            $selectStmt->addField('attr', 'cattr_datatype','cattr_datatype');
            $selectStmt->addField('attr', 'cattr_length','cattr_length');
            $selectStmt->addField('attr', 'cattr_mandatory','cattr_mandatory');
            $selectStmt->addField('attr', 'cattr_unique','cattr_unique');
            $selectStmt->addField('attr', 'cattr_help_txt','cattr_help_txt');
            $selectStmt->addField('attr', 'cattr_multiple_opt','cattr_multiple_opt');
            $selectStmt->addField('attr', 'cattr_display_opt','cattr_display_opt');
            $selectStmt->addField('attr_map', 'entity_ref_tbl_col','entity_ref_tbl_col');
            $selectStmt->addField('attr_map', 'entity_screen_opt','entity_screen_opt');
            $selectStmt->addField('attr_map', 'entity_api_opt','entity_api_opt'); 
            
            $selectStmt->addExpression('GROUP_CONCAT(attr_opt.opt_name)','cattr_opts');
            $selectStmt->addExpression('GROUP_CONCAT(attr_opt.opt_code)','cattr_opt_codes'); 
            //$selectStmt->addExpression('if(attr.cattr_type=\'cattr_type_txtarea\',100,if(attr.cattr_type=\'cattr_type_dropdown\',0,count(attr_opt.opt_code)))','cattr_opt_count'); 
            $selectStmt->addExpression('if(attr.cattr_type=\'cattr_type_txtarea\',100,if(attr.cattr_type=\'cattr_type_dropdown\',0,(if(attr.cattr_type in (\'cattr_type_checkbox\',\'cattr_type_radio\') and attr.cattr_display_opt=\'V\',0,count(attr_opt.opt_code)))))','cattr_opt_count');
            
            $selectStmt->condition('attr_map.entity_type', $entity_type, '=');
            
            if($shortcut!='' && $shortcut=='1' || $fromapi=='1'){
                 $selectStmt->condition('attr.status', 'cre_cattr_sts_del', '!='); //For short cut, need to be take active and inactive 
                 $selectStmt->where('attr_map.entity_screen_opt=1  OR attr_map.entity_api_opt=1');  
            }else{
                 $selectStmt->condition('attr.status', 'cre_cattr_sts_atv', '=');
                 $selectStmt->condition('attr_map.entity_screen_opt', '1', '=');
            } 
            
            //$selectStmt->condition('attr.cattr_type', 'cattr_type_txtbox', '='); //tmp
            
            $selectStmt->groupBy('attr.id');
            $selectStmt->orderBy("cattr_opt_count,field(attr.cattr_type,'cattr_type_radio','cattr_type_checkbox','cattr_type_txtarea')", 'ASC');        
                        
            expDebug::dPrintDBAPI('$selectStmt -  ', $selectStmt);
            $cattr_res = $selectStmt->execute()->fetchAll(); 
            
            expDebug::dPrint('Custom Attribute Results - $cattr_res ' . print_r($cattr_res, 1), 5);
             
            return $cattr_res;   
     
  } catch (Exception $ex) {
    watchdog_exception('getCustomAttributeDetails', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getCustomAttributeValuesFromEntityTable() - To get the custom attribute field values from respective entity table
 * For eg:  if user entity then function relates to slt_person table
 */
function getCustomAttributeValuesFromEntityTable($table_name,$fieldsList=array(),$entity_id,$format='object') {
    try {
            
            expDebug::dPrint(' $fieldsList = ' . print_r($fieldsList, true),5); 
             
            if(empty($fieldsList)){ //if empty then take all custom fields
                $fieldsList=array('e1_cattr0','e1_cattr1','e1_cattr2','e1_cattr3','e1_cattr4','e1_cattr5','e1_cattr6','e1_cattr7','e1_cattr8','e1_cattr9');
                expDebug::dPrint(' Empty $fieldsList = ' . print_r($fieldsList, true),5);  
            }
            
            $selectStmt = db_select($table_name, 'tbl');    
            
            $selectStmt->condition('tbl.id', $entity_id, '='); 
            
            foreach ($fieldsList as $key => $field) {
                $selectStmt->addField('tbl', $field, $field);
            } 

            expDebug::dPrintDBAPI(' $selectStmt obj = ' ,$selectStmt);  
            
            $result = $selectStmt->execute()->fetchAll(); //make_audit_trail_entries() expects an array of objects for prev values
            expDebug::dPrint(' $result = ' . print_r($result, true),3); 
            
            if($format=='array'){
                $result = (array) $result[0];
            } 
            
            return $result; 

    } catch (Exception $ex) {
    watchdog_exception('getCustomAttributeValuesFromEntityTable', $ex);
    expertusErrorThrow($ex); 
  }
  }

/*
 * getEntityTableInformation() - To get entity source table according to entity type
 * if $entity_type='cre_usr' then slt_person, if cattr_ent_class then slt_course_class
 */

 function getEntityTableInformation($entity_type='cre_usr'){
    try{
     
        expDebug::dPrint(' $entity_type = ' . print_r($entity_type, true),5); 
        
        if($entity_type == 'cre_usr'){
            //For user entity
            $entity_type_arr['cre_usr']['table_name']= 'slt_person';
            $entity_type_arr['cre_usr']['del_status']= 'cre_usr_sts_del'; 
        } else if($entity_type == 'cre_sys_obt_crs') {
            //For Course entity
            $entity_type_arr['cre_sys_obt_crs']['table_name']= 'slt_course_template';
            $entity_type_arr['cre_sys_obt_crs']['del_status']= 'lrn_crs_sts_del';           
        } else if($entity_type == 'cre_sys_obt_cls') {
            //For Class entity
            $entity_type_arr['cre_sys_obt_cls']['table_name']= 'slt_course_class';
            $entity_type_arr['cre_sys_obt_cls']['del_status']= 'lrn_cls_sts_del';           
        } else if($entity_type == 'cre_sys_obt_trp') {
            //For TP entity
            $entity_type_arr['cre_sys_obt_trp']['table_name']= 'slt_program';
            $entity_type_arr['cre_sys_obt_trp']['del_status']= 'lrn_lpn_sts_del';           
        } else if($entity_type == 'cre_org') {
            //For Organisation entity
            $entity_type_arr['cre_org']['table_name']= 'slt_organization';
            $entity_type_arr['cre_org']['del_status']= 'cre_org_sts_del';           
        } else if($entity_type == 'cre_sys_obt_loc') {
            //For Location entity
            $entity_type_arr['cre_sys_obt_loc']['table_name']= 'slt_location';
            $entity_type_arr['cre_sys_obt_loc']['del_status']= 'lrn_res_loc_del';           
        } else if($entity_type == 'cre_sys_obt_cnt') {
            //For Content entity
            $entity_type_arr['cre_sys_obt_cnt']['table_name']= 'slt_content_master';
            $entity_type_arr['cre_sys_obt_cnt']['del_status']= 'lrn_cnt_sts_del';           
        } else if($entity_type == 'cre_sys_obt_ord') {
            //For Content entity
            $entity_type_arr['cre_sys_obt_ord']['table_name']= 'slt_order';
            $entity_type_arr['cre_sys_obt_ord']['del_status']= 'lrn_crs_reg_can';           
        }
        
        expDebug::dPrint(' $entity_type_arr[$entity_type] = ' . print_r($entity_type_arr[$entity_type], true),5); 
        
        return $entity_type_arr[$entity_type];
    
    } catch (Exception $ex) {
    watchdog_exception('getEntityTableInformation', $ex);
    expertusErrorThrow($ex); 
    }
    
 }
 
/*
 * updateCustomAttributeFieldInEntityTable() - Update custom attribute data in related entity table like slt_person for User Entity
 */
function updateCustomAttributeFieldInEntityTable($entity_id, $entity_type, $db_field_val_arr,$login_userId='',$action='Updated',$callFromAPI='') {
  
      expDebug::dPrint(' $entity_id = ' . print_r($entity_id, true),3); 
      expDebug::dPrint(' $entity_id = ' . print_r($entity_type, true),3); 
      expDebug::dPrint(' $db_field_val_arr = ' . print_r($db_field_val_arr, true),3);  
      
      
       $tbl_arr=getEntityTableInformation($entity_type);  
       $source_tbl_name=$tbl_arr['table_name'];
        
      if($entity_type=='cre_usr'){
        $entity_type_str='User';
        $module_name='exp_sp_administration_user';
      } else if($entity_type=='cre_sys_obt_crs'){
        $entity_type_str='Course';
        $module_name='exp_sp_administration_catalog';
      } else if($entity_type=='cre_sys_obt_cls'){
        $entity_type_str='Class';
        $module_name='exp_sp_administration_catalog_class';
      } else if($entity_type=='cre_sys_obt_trp'){
        $entity_type_str='Training Plan';
        $module_name='exp_sp_administration_program';
      } else if($entity_type=='cre_org'){
        $entity_type_str='Organization';
        $module_name='exp_sp_administration_organization';
      } else if($entity_type=='cre_sys_obt_loc'){
        $entity_type_str='Location';
        $module_name='exp_sp_administration_location';
      } else if($entity_type=='cre_sys_obt_cnt'){
        $entity_type_str='Content';
        $module_name='exp_sp_administration_content';
      } else if($entity_type=='cre_sys_obt_ord'){
        $entity_type_str='Order';
        $module_name='exp_sp_administration_order';
      }
      
      
      //Get form field values, which are updating through form
      if(count($db_field_val_arr)>0){
        foreach($db_field_val_arr as $key => $val){
            $form_field_list[$key]=$key;
        } 
      } 
       expDebug::dPrint(' $form_field_list = ' . print_r($form_field_list, true),3);  
      
      //Get previous value to store in audit trail
      $prevValues = getCustomAttributeValuesFromEntityTable($source_tbl_name,$form_field_list,$entity_id);
      
      $txn = db_transaction();
    
      try {
        $updateStmt = db_update($source_tbl_name);
       /* $fields = array(
                      'value'              => trim($customFieldValue),
                      'updated_by'         => $userId,
                      'updated_on'         => now(),
                    );*/
                    
         $fields = $db_field_val_arr;          
        
        $updateStmt->fields($fields);    
        $updateStmt->condition('id', $entity_id);
        $numUpdated = $updateStmt->execute();
        expDebug::dPrintDBAPI('$updateStmt = ', $updateStmt);
        expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true),3);      
      }
      catch (Exception $ex) {
        $txn->rollback();
        watchdog_exception('updateCustomAttributeFieldInEntityTable', $ex);
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
        throw $ex;
      }
    
      unset($txn); // Commit the transaction 
      
      $updatedFields = array();
      
      $updatedFields=$db_field_val_arr;
       
      make_audit_trail_entries($updatedFields,  $login_userId, $prevValues, $entity_id, $action, $entity_type_str, $module_name, 'updateCustomAttributeFieldInEntityTable',
                                              $form_field_list); 
      if($callFromAPI){
        return array((object)array('id'=>$entity_id));
      } 
       
}
    

/*
 *  saveCustomAttributesInAddEditForm() - To save the custom attribute values from Form input
 */ 
   
function saveCustomAttributesInAddEditForm(&$form, &$form_state,$entity_id,$entity_type,$loggedInUserId,$action='',$fromapi='') { 
    try {
        expDebug::dprint('$form_state[values]  = '.print_r($form_state['values'],true),5);
        expDebug::dprint('$form_state[input]  = '.print_r($form_state['input'],true),5); 
         
         /*if($entity_type=='cre_sys_obt_ord'){ //if order entity
             $form_state['values']=$form_state['values']['panes']['billing'];
             expDebug::dprint('$form_state[values]  for order = '.print_r($form_state['values'],true),5);
         }*/
         
         
         
        //Get DB and Custom Attribute Mapping 
        expDebug::dprint('$form_state  $fromapi  = '.print_r($fromapi,true),5);
        if($fromapi)
        {
        $attr_det_arr=getCustomAttributeDetails($entity_type,'',$fromapi);
        }
        else {
        $attr_det_arr=getCustomAttributeDetails($entity_type);
        }
        if(count($attr_det_arr)>0){
            foreach($attr_det_arr as $attr_det_key => $attr_det_obj){
                
                if($entity_type=='cre_sys_obt_ord'){ //if order entity , then take from billing pane to update in form state values
                     $form_state['values'][$attr_det_obj->cattr_code]=$form_state['values']['panes']['billing'][$attr_det_obj->cattr_code];
                     expDebug::dprint('$form_state[values]  for order = '.print_r($form_state['values'],true),5);
                 }
                
                
                $cattr_fld_arr[$attr_det_obj->cattr_code]=$attr_det_obj->entity_ref_tbl_col; //get mapping column 
                
                //Check Default Value in Place Holder for TextBox
                
                if($attr_det_obj->cattr_type=='cattr_type_txtbox' || $attr_det_obj->cattr_type=='cattr_type_txtarea'){
                    if(strtolower(trim($attr_det_obj->cattr_help_txt))==strtolower(trim($form_state['values'][$attr_det_obj->cattr_code]))){
                        $form_state['values'][$attr_det_obj->cattr_code]=''; //reset empty if value is place holder
                    }
                } 
                
                if($attr_det_obj->cattr_type=='cattr_type_date'){
                    if($form_state['values'][$attr_det_obj->cattr_code]==t('LBL112')){
                        $form_state['values'][$attr_det_obj->cattr_code]='';  //reset empty if value is place holder
                    }
                } 
                 
                
                //For Text Box, Text Area, Single Drop Down,Multiple DropDown,Date,Radio button,Checkbox
                if(array_key_exists($attr_det_obj->cattr_code,$form_state['values'])){ //Get value only for Text Box and Text Area, Checkbox and date
                    
                    if($fromapi && in_array($attr_det_obj->cattr_type,array('cattr_type_dropdown','cattr_type_checkbox','cattr_type_radio'))){
                        
                        expDebug::dPrint('Before Set Values ='.print_r($form_state['values'][$attr_det_obj->cattr_code],true),5);
                        
                        $api_opt_name_arr=explode(",",$attr_det_obj->cattr_opts);
                        $api_opt_code_arr=explode(",",$attr_det_obj->cattr_opt_codes);
                        $api_opt_arr = array_combine($api_opt_name_arr, $api_opt_code_arr);
                        
                        $tmp_api_opt_arr= explode(',',$form_state['values'][$attr_det_obj->cattr_code]);
                        $tmp_api_opt_arr = array_filter(array_map('trim', $tmp_api_opt_arr));
                        $api_opt_to_store='';
                        $tmp_opt_arr=array();
                        if(count($tmp_api_opt_arr)>0){
                            foreach($tmp_api_opt_arr as $tmp_opt_key => $tmp_opt_val){
                                $tmp_code=$api_opt_arr[$tmp_opt_val];
                                $tmp_opt_arr[$tmp_code]=$tmp_code;
                            }
                            //$api_opt_to_store=implode(",",$tmp_opt_arr);
                        } 
                        expDebug::dPrint('$$tmp api_opt_to_store ='.print_r($tmp_opt_arr,true),5);  
                        $form_state['values'][$attr_det_obj->cattr_code]=$tmp_opt_arr;
                        expDebug::dPrint('Set Values ='.print_r($form_state['values'][$attr_det_obj->cattr_code],true),5);
                    }
                    
                    
                    if($attr_det_obj->cattr_type=='cattr_type_dropdown'){
                        if(!is_array($form_state['values'][$attr_det_obj->cattr_code])){ //if single drop down array
                            
                            /*if($fromapi){ 
                                
                                
                                expDebug::dPrint('cusdetails >>'.print_r($opt_arr,1), 5);
                                $makeOptionsArray = explode(',',$form_state['values'][$attr_det_obj->cattr_code]); 
                                
                                $makeOptionsArray = array_filter(array_map('trim', $makeOptionsArray));
                                $makeOptionsArray = preg_filter('/^/', 'o_', $makeOptionsArray);
                                    expDebug::dPrint('before makeOptionsArray ='.print_r($makeOptionsArray,true),5);
                                $makeOptionsArray = implode(',',$makeOptionsArray);
                                    expDebug::dPrint('after makeOptionsArray ='.print_r($makeOptionsArray,true),5);
                                $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=strtolower($makeOptionsArray);
                                */                              
                                
                                
                                
                            //}else
                          
                            
                            if($fromapi){
                                $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=implode(',',$form_state['values'][$attr_det_obj->cattr_code]);
                            }else{
                                $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=$form_state['values'][$attr_det_obj->cattr_code];
                            }
                            
                            
                        }else{
                          $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=implode(',',$form_state['values'][$attr_det_obj->cattr_code]);    
                        }
                        
                    }else if($attr_det_obj->cattr_type=='cattr_type_checkbox'){
                        /*if($fromapi){
                            $makeCheckboxOptionsArray = explode(',',$form_state['values'][$attr_det_obj->cattr_code]);
                            $makeCheckboxOptionsArray = array_filter(array_map('trim', $makeCheckboxOptionsArray));
                            $makeCheckboxOptionsArray = preg_filter('/^/', 'o_', $makeCheckboxOptionsArray);
                                expDebug::dPrint('before makeOptionsArray ='.print_r($makeCheckboxOptionsArray,true),5);
                            $makeCheckboxOptionsArray = implode(',',$makeCheckboxOptionsArray);
                                expDebug::dPrint('after makeCheckboxOptionsArray ='.print_r($makeCheckboxOptionsArray,true),5);
                            $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=strtolower($makeCheckboxOptionsArray);
                        } else {*/
                            expDebug::dPrint('Before ='.print_r($form_state['values'][$attr_det_obj->cattr_code],true),5);
                           $trunc_arr_values = array_filter($form_state['values'][$attr_det_obj->cattr_code]);
                           expDebug::dPrint('After ='.print_r($trunc_arr_values,true),5);
                           $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=implode(',',$trunc_arr_values);
                       // } 
                    } 
                    else{
                        if($fromapi && $attr_det_obj->cattr_type=='cattr_type_radio'){
                            $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=implode(',',$form_state['values'][$attr_det_obj->cattr_code]);
                        } else                      
                           $tbl_arr[$attr_det_obj->entity_ref_tbl_col]=$form_state['values'][$attr_det_obj->cattr_code];
                    }  
                    
                }
                 
                
            }
        }
        
        expDebug::dprint('Active Attributes - $cattr_fld_arr[]  = '.print_r($cattr_fld_arr,true),5); 
        expDebug::dprint('Form Attributes and Mapping Columns - $tbl_arr[]  = '.print_r($tbl_arr,true),5); 
        
        if(count($tbl_arr)>0 ){ //If there is custom fields then update in related table 
             updateCustomAttributeFieldInEntityTable($entity_id, $entity_type, $tbl_arr,$loggedInUserId,$action); 
        }
        
        
    }catch (Exception $ex) {
            watchdog_exception('saveCustomAttributesInAddEditForm', $ex);
            expertusErrorThrow($ex);
    }
} 


/*
 * getCustomAttributeDefaultFormValues() - To get default form values when open the form. 
 * - Values are fetching from related entity table like slt_person and put in the array
 */

function getCustomAttributeDefaultFormValues($custom_attr_result,$entity_type,$entity_id){
    try { 
            
            $tbl_arr=getEntityTableInformation($entity_type); 
            $table_name=$tbl_arr['table_name']; 
             
             $attr_default_form_values_arr=array();
             
            if($entity_id!=''){
                
                 if(count($custom_attr_result)>0){ //if custom attribute exists then take the default value from DB 
                    
                     $tmp_attr_details_arr=getCustomAttributeValuesFromEntityTable($table_name,'',$entity_id,'array');  
                     expDebug::dPrint('$tmp_attr_details_arr ' . print_r($tmp_attr_details_arr, 1), 5);  
                
                     foreach($custom_attr_result as $cattr_obj){
                        $attr_default_form_values_arr[$cattr_obj->cattr_code]=$tmp_attr_details_arr[$cattr_obj->entity_ref_tbl_col];
                     } 
                     
                      expDebug::dPrint('$attr_default_form_values_arr ' . print_r($attr_default_form_values_arr, 1), 5);  
                    
                 }
                
            }else{
                expDebug::dprint('No custom attribute details available in respective table '.$table_name.' for '.$entity_type,5); 
            }
         
            return $attr_default_form_values_arr;
        
    }catch (Exception $ex) {
            watchdog_exception('getCustomAttributeDefaultFormValues', $ex);
            expertusErrorThrow($ex);
    }
}

/*
 * getOptionNamesforAttribute() - to get custom label and value for View screen
 */
function getOptionNamesforAttribute($attr_id, $opt_codes_arr){
    try{
    
        expDebug::dprint('$opt_codes_arr = ' . print_r($opt_codes_arr, 1), 5);  
        
        $select = db_select('slt_custom_attr_options','opt');
        $select->addExpression('GROUP_CONCAT(opt.opt_name)','opt_name_list');
        $select->condition('opt.cattr_id',$attr_id, '='); 
        $select->condition('opt.opt_code', $opt_codes_arr, 'IN');
        $select->groupBy('opt.cattr_id'); 
        $select->range(0,1); 
        expDebug::dPrintDBAPI('$select  = ' , $select);
        $result = $select->execute()->fetchAll();
        
        expDebug::dprint('$result = ' . print_r($result, 1), 5);  
        
        return $result[0]->opt_name_list;
    }catch (Exception $ex) {
        watchdog_exception('getOptionNamesforAttribute', $ex);
        expertusErrorThrow($ex);
    }
}
 
/*
 * getCustomAttributesForView() - to get custom attribute and value from DB for View screen 
 */
function getCustomAttributesForView($entity_id, $entity_type, $viewtype='view'){ 
    try{ 
        
        $cattr_res=getCustomAttributeDetails($entity_type);
        
        if(count($cattr_res)>0){
            
            $tbl_arr=getEntityTableInformation($entity_type); 
            $table_name=$tbl_arr['table_name'];
            
            $tmp_attr_details_arr=getCustomAttributeValuesFromEntityTable($table_name,'',$entity_id,'array');  
            expDebug::dPrint('$tmp_attr_details_arr ' . print_r($tmp_attr_details_arr, 1), 5);  
            
            $i=1;
            
            $theme_render_str='';
            if($entity_type=='cre_usr'){
                $theme_render_str='<div class="para">
                               <div class="user-container">
                                        <div class="user-separator"></div>
                              </div>
                             </div>';
            }
            
            $opt_ent_arr=array('cattr_type_checkbox','cattr_type_radio','cattr_type_dropdown');
            
            foreach($cattr_res as $cattr_obj){
                
                 $label_str=$cattr_obj->cattr_name;
                 $value_str=$tmp_attr_details_arr[$cattr_obj->entity_ref_tbl_col]; 
                
                  if(in_array($cattr_obj->cattr_type,$opt_ent_arr)){ 
                      $opt_codes_arr=explode(',',$value_str);
                      $value_str=getOptionNamesforAttribute($cattr_obj->cattr_id,$opt_codes_arr); 
                  } 
                  
                  if($viewtype == 'edit-view') {
                    $theme_render_str[$label_str] = $value_str;
                  } else { 
                      
                       if($i%2==1){ //first column
                         if($entity_type=='cre_sys_obt_ord'){
                              $ord_first_col=1;
                              $theme_render_str.=' <div class="clear two-column-container-view">';
                         }
                         $theme_render_str.=getCustomAttributeThemesForView($label_str,$value_str,$entity_type,'first_col');
                       }  
                       
                       if($i%2==0){ //Second column
                          $theme_render_str.=getCustomAttributeThemesForView($label_str,$value_str,$entity_type,'second_col');
                           if($entity_type=='cre_sys_obt_ord'){
                                $ord_first_col=0;
                              $theme_render_str.='<div class="clearBoth"></div></div>';
                           }
                       }
                  }
                   
                   $i++;    
                
            }  

                   if($entity_type=='cre_sys_obt_ord' && $ord_first_col=='1'){
                        $ord_first_col=0;
                        $theme_render_str.='<div class="clearBoth"></div></div>'; 
                   }
                   
                   if($entity_type=='cre_sys_obt_ord' &&  $i>0){
                        $theme_render_str='<div id="customattribute-order-detail-container">'.$theme_render_str.'</div>
                                          <div class="ord-separator"></div>
                        ';
                   } 
            
            expDebug::dPrint('$theme_render_str = '.print_r($theme_render_str,true), 5);  
              
        }else{
            expDebug::dPrint('No custom attributes available for this entity ', 5);  
        } 

        return $theme_render_str;
         
    }catch (Exception $ex) {
        watchdog_exception('getCustomAttributesForView', $ex);
        expertusErrorThrow($ex);
    }
}  


/*
 * getCustomAttributeThemesForView() - To render the theme according to each entity type 
 */
function getCustomAttributeThemesForView($label_str,$value_str,$entity_type,$position){
    try{
         $tmp_str='';
         
         if($entity_type=='cre_usr'){ //if user
         
              if($position=='first_col'){ // For rendering in first column
              
                   $tmp_str.='<div class="left-side-container">
                           <div class="para">
                           <div class="user-container">
                               <span class="userview-label">'.$label_str.':</span>';
                               
                    $tmp_str.='<span class="user-desc">
                               <span class="vtip" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'. 
                               titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).
                               '</span></span>
                               </div>
                              </div>
                             </div>';
              }
 
            
            if($position=='second_col'){ // For rendering in Second column
              
                   $tmp_str.='<div class="right-side-container">
                              <div class="para">
                               <div class="user-container">
                                   <span class="userview-label">'.$label_str.':</span>';
                               
                    $tmp_str.='<span class="user-desc">
                               <span class="vtip" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'.
                               titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-RIGHT-COL', str_replace(',', ', ', $value_str),20).
                               '</span></span>
                               </div>
                              </div>
                             </div>';
              } 
              
         }else if($entity_type=='cre_sys_obt_ord'){ //if order 
         
              if($position=='first_col'){ // For rendering in first column
              
                 /*  $tmp_str.='<div class="left-side-container">
                           <div class="para">
                           <div class="user-container">
                               <span class="userview-label">'.$label_str.':</span>';
                               
                    $tmp_str.='<span class="user-desc">
                               <span class="vtip" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'. 
                               titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).
                               '</span></span>
                               </div>
                              </div>
                             </div>';*/
                             
                    /*    $tmp_str.= '<div class="clear two-column-container-view"><div class="first-col-container-view course-view-mode">';
                        $tmp_str.= '<div class="first-col-value-view view-mode-field-value-txt" ><span title="'.sanitize_data(str_replace(',', ', ', $value_str)).'" class="vtip">'.titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).'</span></div>';
                        $tmp_str.= '</div>'; */
                        
                        $tmp_str=' <div class="first-col-container-view course-view-mode">
                                     <div id="edit-order-detail-order-id" class="form-item form-type-item">
                                         <label for="edit-order-detail-order-id">'.$label_str.': </label>
                                               <div class="first-col-value-view view-mode-field-value-txt"><span title="'.sanitize_data(str_replace(',', ', ', $value_str)).'" class="vtip">'.titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).'</span></div>
                                    </div>
                                    </div>';
                                 
              }

            
            if($position=='second_col'){ // For rendering in Second column
              
                  /* $tmp_str.='<div class="right-side-container">
                              <div class="para">
                               <div class="user-container">
                                   <span class="userview-label">'.$label_str.':</span>';
                               
                    $tmp_str.='<span class="user-desc">
                               <span class="vtip" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'.
                               titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-RIGHT-COL', str_replace(',', ', ', $value_str),20).
                               '</span></span>
                               </div>
                              </div>
                             </div>';*/
                             
                      /*  $tmp_str.= '<div class="second-col-container-view">';
                        $tmp_str.= '<div class="second-col-value-view view-mode-field-value-txt"><span title="'.sanitize_data(str_replace(',', ', ', $value_str)).'" class="vtip">'.titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).'</span></div>';
                        $tmp_str.= '</div><div class="clearBoth"></div></div>';*/
                        
                        $tmp_str.=' <div class="second-col-container-view">
                                        <div id="edit-order-detail-order-date" class="form-item form-type-item">
                                           <label for="edit-order-detail-order-date">'.$label_str.': </label>
                                                <div class="second-col-value-view view-mode-field-value-txt"><span title="'.sanitize_data(str_replace(',', ', ', $value_str)).'" class="vtip">'.titleController('ADMIN-NARROW-USERVIEW-CUSTOMATTRIBUTE-LEFT-COL', str_replace(',', ', ', $value_str),24).'</span></div>
                                        </div>
                                       </div>';
                        
                              
              } 
              
         } else if($entity_type=='cre_sys_obt_crs' || $entity_type=='cre_sys_obt_cls' || $entity_type=='cre_sys_obt_trp' || $entity_type == 'cre_org' || $entity_type == 'cre_sys_obt_cnt'){
            if($position=='first_col'){ // For rendering in first column
                $tmp_str.='<div class="left-side-container">
                           <div class="para">
                           <div id="custom-attribute-view-left" class="code-container">
                               <span class="detail-code">'.$label_str.':</span>';
                
                $tmp_str.='<span class="detail-desc detail-single-line">
                           <span class="vtip single-line-lbl-val" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'.titleController('ADMIN-NARROW-CUSTOMATTRIBUTE-LEFT-COL', $value_str,35).'</span>
                           </span>
                           </div>
                           </div>
                           </div>';
            }
            if($position=='second_col'){ // For rendering in Second column
                $divid = ($entity_type == 'cre_sys_obt_trp') ? 'id="tp-right-view"' : ($entity_type == 'cre_org') ? 'id="org-right-detail-code"' : '';
                $tmp_str.='<div class="right-side-container">
                           <div class="para">
                           <div id="custom-attribute-view-right" class="code-container">
                               <span '.$divid.' class="detail-code">'.$label_str.':</span>';
                 
                $tmp_str.='<span class="detail-desc detail-single-line">
                           <span class="vtip single-line-lbl-val" title="'.sanitize_data(str_replace(',', ', ', $value_str)).'">'.titleController('ADMIN-NARROW-CUSTOMATTRIBUTE-RIGHT-COL', $value_str,35).'</span>
                           </span>
                           </div>
                           </div>
                           </div>';
            }   
         }
         
       
       expDebug::dPrint('$tmp_str = '.print_r($tmp_str,true), 5);  
       
        return $tmp_str;
         
    }catch (Exception $ex) {
        watchdog_exception('getCustomAttributeThemesForView', $ex);
        expertusErrorThrow($ex);
    }
}  

  
/*
 *  renderCustomAttributesInAddEditForm() - To render the custom attributes according to Entity Type like User, Course, Class 
 */
 function renderCustomAttributesInAddEditForm(&$form, &$form_state,$entity_type,$entity_id='',$req_entity_arr=array()) {  
    try {
            expDebug::dPrint('$form ---'.print_r($form,true),5);
            expDebug::dPrint('$form_state ---'.print_r($form_state,true),5);
               
            $selectStmt = db_select('slt_custom_attr', 'attr'); 
            $selectStmt->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id');
            $selectStmt->leftJoin('slt_custom_attr_options', 'attr_opt','attr_opt.cattr_id=attr.id');
            
            $selectStmt->addField('attr', 'id','cattr_id');
            $selectStmt->addField('attr', 'cattr_name','cattr_name');
            $selectStmt->addField('attr', 'cattr_code','cattr_code');
            $selectStmt->addField('attr', 'cattr_type','cattr_type');
            $selectStmt->addField('attr', 'cattr_datatype','cattr_datatype');
            $selectStmt->addField('attr', 'cattr_length','cattr_length');
            $selectStmt->addField('attr', 'cattr_mandatory','cattr_mandatory');
            $selectStmt->addField('attr', 'cattr_unique','cattr_unique');
            $selectStmt->addField('attr', 'cattr_help_txt','cattr_help_txt');
            $selectStmt->addField('attr', 'cattr_multiple_opt','cattr_multiple_opt');
            $selectStmt->addField('attr', 'cattr_display_opt','cattr_display_opt');
            $selectStmt->addField('attr_map', 'entity_ref_tbl_col','entity_ref_tbl_col');
            $selectStmt->addField('attr_map', 'entity_screen_opt','entity_screen_opt');
            $selectStmt->addField('attr_map', 'entity_api_opt','entity_api_opt'); 
            
            $selectStmt->addExpression('GROUP_CONCAT(attr_opt.opt_name)','cattr_opts');
            $selectStmt->addExpression('GROUP_CONCAT(attr_opt.opt_code)','cattr_opt_codes'); 
            //$selectStmt->addExpression('if(attr.cattr_type=\'cattr_type_txtarea\',100,if(attr.cattr_type=\'cattr_type_dropdown\',0,count(attr_opt.opt_code)))','cattr_opt_count'); 
            $selectStmt->addExpression('if(attr.cattr_type=\'cattr_type_txtarea\',100,if(attr.cattr_type=\'cattr_type_dropdown\',0,(if(attr.cattr_type in (\'cattr_type_checkbox\',\'cattr_type_radio\') and attr.cattr_display_opt=\'V\',0,count(attr_opt.opt_code)))))','cattr_opt_count');
            
             
            $selectStmt->condition('attr_map.entity_type', $entity_type, '=');
            $selectStmt->condition('attr.status', 'cre_cattr_sts_atv', '=');
            $selectStmt->condition('attr_map.entity_screen_opt', '1', '=');
            
            
            if(!empty($req_entity_arr)){
                 $selectStmt->condition('attr.cattr_type', $req_entity_arr, 'IN'); //type 
            }

            //$selectStmt->condition('attr.cattr_type', 'cattr_type_txtbox', '='); //type
                        
             
            $selectStmt->groupBy('attr.id');
            $selectStmt->orderBy("cattr_opt_count,field(attr.cattr_type,'cattr_type_radio','cattr_type_checkbox','cattr_type_txtarea')", 'ASC');        
            $selectStmt->range(0,10);
            
            expDebug::dPrintDBAPI('$selectStmt -  ', $selectStmt);
            $cattr_res = $selectStmt->execute()->fetchAll();
            
            //Dynamic translation - start 
            $config=getConfig('exp_sp');
            if(isset($config["dynamic_translation"]) &&  $config["dynamic_translation"] == 1 || $config["dynamic_translation"] == "1")
            {
                include_once($_SERVER["DOCUMENT_ROOT"]. '/translatetool/ExpertusONETranslation.inc');
                global $user;
                
                $translationStrings = array();
                $translationIds = array();
                foreach ($cattr_res as $Custom) {
                    $title    =   $Custom->cattr_name;
                    $helptext =   $Custom->cattr_help_txt;
                    
                    $translationStrings[]=$title;
                    $translationStrings[]=$helptext;
                    $translationIds[] = $Custom->cattr_id;
                }
                expDebug::dPrint(' $translatedLabels before = ' . print_r($translationStrings, true) , 3);
                
                $translatedLabels = getTranslationsForArrayStrings($translationStrings,$user->language,$translationIds,"cre_sys_obt_cattr");
                expDebug::dPrint(' $translatedLabels  after = ' . print_r($translationStrings, true) , 3);
                
                foreach ($cattr_res as $Custom) {
                    $Custom->cattr_name          = strval($translatedLabels[$Custom->cattr_name]);
                    $Custom->cattr_help_txt      = strval($translatedLabels[$Custom->cattr_help_txt]);
                    
                }
            }
            
            $cattr_count=count($cattr_res);
            
            expDebug::dPrint('Custom Attribute Results - $cattr_res ' . print_r($cattr_res, 1), 5);
            expDebug::dPrint('$cattr_count ' . print_r($cattr_count, 1), 5);
            
           // $form_state['storage']['shortcut_custom_attr_count']=$cattr_count;
           
           $short_cut_attr_res=getCustomAttributeDetails($entity_type,'1');
           expDebug::dPrint('$short_cut_attr_res = ' . print_r($short_cut_attr_res, 1), 5);
           $form_state['storage']['shortcut_custom_attr_count']=count($short_cut_attr_res);
           
            
            if ($cattr_count>0){
                        
                          $attr_default_form_values_arr=getCustomAttributeDefaultFormValues($cattr_res,$entity_type,$entity_id);  
                          
                          $k=1;
                          $hidden_attr_str='';  
						  						  
                          foreach($cattr_res as $cattr_entity){
                              //For order javascript validation
                              if($cattr_entity->cattr_mandatory == '1' && $entity_type=='cre_sys_obt_ord'){ //if mandatory 
                                   $tmp_cattr_code=$cattr_entity->cattr_code;
                                   if($cattr_entity->cattr_type=='cattr_type_txtbox' || $cattr_entity->cattr_type=='cattr_type_dropdown' || $cattr_entity->cattr_type=='cattr_type_txtarea'){ //if attibute type is text/textarea/dropdown
                                       $tmp_cattr_code=str_replace('_','-',$cattr_entity->cattr_code);
                                   }
                                  $hidden_attr_str.='~~'.$cattr_entity->cattr_type.'|'.$tmp_cattr_code.'|'.$cattr_entity->cattr_help_txt; 
                              } 
                              
                            if($k % 2 == 0){
                                if($cattr_entity->cattr_mandatory == 1 && !empty($cattr_entity->cattr_help_txt)){
                                    $customAttrWrapperId[] .= 'customattr-both';
                                } else if($cattr_entity->cattr_mandatory == 1 && empty($cattr_entity->cattr_help_txt)){
                                    $customAttrWrapperId[] .= 'customattr-mandatory';
                                } else if($cattr_entity->cattr_mandatory == 0 && !empty($cattr_entity->cattr_help_txt)){
                                    $customAttrWrapperId[] .= 'customattr-info';
                                } else {
                                    $customAttrWrapperId[] .= 'customattr-plain';
                                }
                            }
                            $k++;
                          }
                          expDebug::dPrint('$customAttrWrapperId ==> '.print_r($customAttrWrapperId,1),4);
						   
						  if($hidden_attr_str!='' && $entity_type=='cre_sys_obt_ord'){ //if order
                                $hidden_attr_str=ltrim($hidden_attr_str,'~~');
                          
                                $form['cust_attr_mandatory_hiddens'] = array(
                                  '#type' => 'markup',
                                   '#prefix' => '<div style=\'display:none;\' id=\'hid_custom_attr_order_mandatroy\' data-hval=\''.$hidden_attr_str.'\'></div>'
                                 );
                                 
                                 expDebug::dPrint('$hidden_attr_str ==> '.print_r($hidden_attr_str,1),4);  
                          }
                            
                          if(in_array('customattr-both',$customAttrWrapperId)){
                            $customAttrWrapperId = 'customattr-both';
                          } else if(in_array('customattr-info',$customAttrWrapperId)){
                            $customAttrWrapperId = 'customattr-info';
                          } else if(in_array('customattr-mandatory',$customAttrWrapperId)){
                            $customAttrWrapperId = 'customattr-mandatory';
                          } else{
                            $customAttrWrapperId = 'customattr-plain';
                          }
               
                           $row_id=1;
                           $i=1;
                           $endrow_flag='0';
                           $opt_count=2;
                           $tab_index=100;
                           $column_count=0;
                           $processed_double_col=0;
                           
                           $form['cust_attr_containter_start'] = array(
                                '#type' => 'markup',
                                '#prefix' => '<div class="separator"></div><div id="custom_attr_container" class="'.$customAttrWrapperId.'">'
                           );
                    
                           foreach ($cattr_res as $cattr_obj){
                               
                                      $form_state['storage']['custom_attribute_form_key'][$cattr_obj->cattr_code]=$cattr_obj->cattr_code; 
                            
                                     if($cattr_obj->cattr_opt_count>$opt_count){ //if option count > 2 then make all the upcoming attributes as single row
                                         
                                           if($endrow_flag=='0' &&  $processed_double_col>0){
                                              endAddEditTwoColumnRow($form); 
                                              $endrow_flag='1';
                                           }
                                            
                                           //Paint / Render the Custom Attribute Form Contols.
                                           renderCustomAttributeFormControls($form,$form_state,$cattr_obj,$tab_index,$entity_type,$column_count,$attr_default_form_values_arr);
                                          
                                     }else{ //if option count <=2 
                                             
                                           $processed_double_col++;   
                                           $column_count++;
                                           
                                            if($i%2==1){  //Check start 2 col field
                                               $custom_attr_row_id='custom_attr_doublecolrow_id_'.$row_id;  
                                               addTwoColumnRowToAddEditForm($form, $custom_attr_row_id);
                                               $endrow_flag='0';
                                               $row_id++;
                                          }  
                                          
                                           //Paint / Render the Custom Attribute Form Contols.
                                          renderCustomAttributeFormControls($form,$form_state,$cattr_obj,$tab_index,$entity_type,$column_count,$attr_default_form_values_arr);
                                          
                                          if($i%2==0){ //Check end 2 col field
                                                 endAddEditTwoColumnRow($form);
                                                 $endrow_flag='1';
                                          }
                                     } 
                                     
                                      $i++;
                                      $tab_index++;
                               
                           } //End foreach
                           
                           expDebug::dPrint('$endrow_flag = ' . print_r($endrow_flag, 1), 5);
                           
                           if($endrow_flag=='0' && $processed_double_col>0){
                              endAddEditTwoColumnRow($form);
                           }
             
       
                        $form['cust_attr_containter_end'] = array(
                            '#type' => 'markup',
                            '#suffix' => '</div>'
                        );
                         
                          return $form;
               
                 
            }else{
                expDebug::dPrint('No ustom Attribute Available for this entity ', 5);
            } 
    }catch (Exception $ex) {
        watchdog_exception('renderCustomAttributesInAddEditForm', $ex);
        expertusErrorThrow($ex);
  }
}

// Set custom attribute form set values in Session - Ganesh
function storeCustomAttributeFormValuesinSession(&$form, &$form_state){
    try {
            
            
        //GeCustom Attribute in the Storage - Ganesh 
       // expDebug::dPrint('Form Storage in Validate Pre = ' . print_r($form_state['storage'], true), 5);
     expDebug::dPrint('Billing Pane Store = ' . print_r($form_state['values']['panes']['billing'], true), 5); 
       
        unset($_SESSION['ses_default_cust_attr_arr']);
        $order_cust_attr_count=count($form_state['storage']['custom_attribute_form_key']);
        expDebug::dPrint('$order_cust_attr_count= ' . print_r($order_cust_attr_count, true), 5);
        if($order_cust_attr_count>0){
           foreach($form_state['storage']['custom_attribute_form_key'] as $form_key => $val){
               // $form_state['storage']['custom_attribute_form_key_val'][$form_key]=$form_state['values']['panes']['billing'][$form_key];
               if(is_array($form_state['values']['panes']['billing'][$form_key])){
                   $tmp_arr=array_filter($form_state['values']['panes']['billing'][$form_key]);
               }else{
                    $tmp_arr=$form_state['values']['panes']['billing'][$form_key];
               }
                $tmp_values['custom_attribute_form_key_val'][$form_key]=$tmp_arr;
            }    
        }  
        
        $_SESSION['ses_default_cust_attr_arr']=$tmp_values; //Set in Sessoion
        
        expDebug::dPrint('ses_default_cust_attr_arr = ' . print_r($_SESSION['ses_default_cust_attr_arr'], true), 5);
        
    }catch (Exception $ex) {
        watchdog_exception('storeCustomAttributeFormValuesinSession', $ex);
        expertusErrorThrow($ex);
    }
}

// Render Custom Attribute form for Admin Order Page
function renderCustomAttributeinBillingPane(&$form,&$form_state,$id='billing',$from_page=''){
    try {
          
         $form['custom_attr_start_for_order'] = array(
            '#type' => 'markup  ',
            '#prefix' => '', 
        );
        
        
        //expDebug::dPrint('Form State Val= '.print_r($form_state['values'],true),5);
       
        // $tmp_storage_arr=$form_state['storage']['custom_attribute_form_key_val'];   

        
        $default_cust_attr_arr=$_SESSION['ses_default_cust_attr_arr']['custom_attribute_form_key_val']; 
        
         expDebug::dPrint('$default_cust_attr_arr= '.print_r($default_cust_attr_arr,true),5);
         
        $ord_id='';
       // include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        renderCustomAttributesInAddEditForm($form, $form_state,'cre_sys_obt_ord',$ord_id);  
        
        // #custom_attribute_0078975   
       // addCustomAttributesBlockToAddEditForm($form, $form_state, $ord_id, 'order', 'order-custom-attributes', 'admin-order-create-form');       
         
        $form['custom_attr_end_for_order'] = array(
            '#type' => 'markup  ',
            '#prefix' => '', 
        ); 
        
        expDebug::dPrint('$form New = ' . print_r($form, true), 5);
        
        $form['panes'][$id]['order_custom_attr_start_div'] = array(
            '#type' => 'markup  ',
            '#prefix' => '<div id="orderpage_custom_attributes">', 
        );
        
        
        $get_cattr_arr_from_form=0;
        foreach($form as $form_key => $form_val){
            expDebug::dPrint('$form_key = ' . print_r($form_key, true), 5);
            expDebug::dPrint('$form_val = ' . print_r($form_val, true), 5);
            if($form_key=='custom_attr_start_for_order'){
                $get_cattr_arr_from_form++;
            }
            
            if($get_cattr_arr_from_form>0){ //Custom Attribute Started 
           
                if (strpos($form_key,'load_multiselect_') !== false) { //if multi select exists 
                   $tmp_prefix=$form[$form_key]['#prefix'];
                   $form[$form_key]['#prefix']=str_replace('edit-c-','edit-panes-billing-c-',$tmp_prefix);
                } 
                  
                 
                //set default value from sessionif exists 
                if(array_key_exists($form_key,$default_cust_attr_arr)){
                     // expDebug::dPrint('Custom Attribute key exists = ' . print_r($form, true), 5);
                      //$form['panes'][$id][$form_key]['#default_value']= $form_state['storage']['custom_attribute_form_key_val'][$form_key];
                     // $form[$form_key]['#default_value']= $form_state['storage']['custom_attribute_form_key_val'][$form_key];
                      $form[$form_key]['#default_value']= $default_cust_attr_arr[$form_key];
                }  
                
                 
                //if key exists and if it is the payment page
                if(array_key_exists($form_key,$form_state['storage']['custom_attribute_form_key']) && $from_page=='admin_payment'){
                    $form[$form_key]['#disabled']= true;  
                }
                 
                 //if key exists in Custom Attribute then change the id for char_count_c to text are
                if(array_key_exists($form_key,$form_state['storage']['custom_attribute_form_key']) &&  $form[$form_key]['#type']=='textarea' && 1==2){
                      $tmp_textare_suffix=$form[$form_key]['#suffix'];
                    
                     if (strpos($tmp_textare_suffix,'char_count_c_') !== false) { //if char count exists for text area
                       $repl_from='char_count_'.$form_key;
                       $repl_to='char_count_edit-panes-billing-'.str_replace('_','-',$form_key);  
                       expDebug::dPrint('$repl_from='.$repl_from,5);
                       expDebug::dPrint('$repl_to='.$repl_to,5);
                       $form[$form_key]['#suffix']=str_replace($repl_from,$repl_to,$tmp_textare_suffix);
                       
                       // Need to change id in callback - // [onKeyUp] => limitTextareaChars("c_order_textarea", 100); return false;
                       
                            $tmp_keyup=$form[$form_key]['#attributes']['onKeyUp']; 
                             expDebug::dPrint('$tmp_keyup='.$tmp_keyup,5); 
                         if (strpos($tmp_keyup,$form_key) !== false) { //if id exists in limitTextareaChars function
                                
                                 $repl_from_keyup=$form_key;
                                 $repl_to_keyup='edit-panes-billing-'.str_replace('_','-',$form_key);  
                                 
                                  expDebug::dPrint('$repl_from_keyup='.$repl_from_keyup,5);
                                  expDebug::dPrint('$repl_to_keyup='.$repl_to_keyup,5);
                                 
                        
                                  $form[$form_key]['#attributes']['onKeyUp']=str_replace($repl_from_keyup,$repl_to_keyup,$tmp_keyup);
                                 
                                  expDebug::dPrint('After Change $tmp_keyup='.$form[$form_key]['onKeyUp'],5); 
                               
                         }
                       
                    } 
                     
                }
                
                
                $form['panes'][$id][$form_key]=$form[$form_key];
                
                //$form_state['storage']['custom_attribute_form_key'][$form_key]=$form_key;  
                
                unset($form[$form_key]);
            } 
            
            if($form_key=='custom_attr_end_for_order'){
                $get_cattr_arr_from_form=0;
            } 
        }
        
        $form['panes'][$id]['order_custom_attr_end_div'] = array(
            '#type' => 'markup  ',
            '#suffix' => '</div>', 
        );
        
        
        expDebug::dPrint('$form New Pane= ' . print_r($form['panes'][$id], true), 5);
        
        
    }catch (Exception $ex) {
        watchdog_exception('renderCustomAttributeinBillingPane', $ex);
        expertusErrorThrow($ex);
    }
}

/* Start Callbacks for Creating Short Cut to Custom Attribute  #custom_attribute_0078975 */

/*
 * addCustomAttributesBlockToAddEditForm() - To display + add to custom   
 */
function addCustomAttributesBlockToAddEditForm(&$form, &$form_state, $entityId, $entityType, $uniqueId, $formId) {
    try {
        
        //Check privilege for add/Edit
         $previlage_attr = adminVisibilityForGlobalAdd('cre_sys_obt_cattr','','','1');
    	//$previlage_attr = adminVisibilityForcustomattribute('cre_sys_obt_cattr');
        expDebug::dPrint('$previlage_attr = '.print_r($previlage_attr,true),5);
       //$previlage_attr[0]->addvisible;
        
         $previlage_add_attr='0';
        //if($previlage_attr[0]->addvisible=='1' || $GLOBALS["user"]->uid == 1){
         if($previlage_attr[0]->addvisible >='1' || $GLOBALS["user"]->uid == 1){
         	 
            $previlage_add_attr='1';
        } 
        expDebug::dPrint('$previlage_add_attr = '.print_r($previlage_add_attr,true),5);
         
      if($form_state['storage']['shortcut_custom_attr_count']>=10 || $previlage_add_attr=='0'){
            return ;
      }
    
      global $theme_key;
      $loaderWrapper = $uniqueId; 
      
      $customDetails = array();
      expDebug::dPrint(' $form_state[custom_fields] = ' .print_r($form_state['custom_fields'], true),5);
      expDebug::dPrint(' $entityId = ' .print_r($entityId, true),4);         
      
      if(empty($entityId)){
          $entityId='0';
      } 
      
       $form['customattributes_wrapper_start'] = array(
        '#type' => 'markup',
        '#markup' => '<div id="' . $uniqueId . '" class="addedit-customattributes-wrapper">'
      );
      
       $lower_entityType=strtolower($entityType);
                      
      
      $addcustomAttributes['class'][] = 'admin-action-button-middle-bg'; 
      
      //Adding plus symbol in the add another class
      $addcustomAttributes['class'][] = 'add-custombtn-symbol';
      
      // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
      $addcustomAttributes['class'][] = 'addedit-form-expertusone-throbber';  
      // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
      $addcustomAttributes['data-wrapperid'] = array($uniqueId);
    
      //$addcustomAttributes['onclick'][]  = 'addCustomGrayToBlack("'.$form_state['custom_fields']['count'].'");'; 
      
      $addBtnPrefix = '<div class="addedit-new-field addedit-custom-attribute" id="customattribute-add-btn-' . $uniqueId . '">' .
                         '<div class="addedit-new-field-title">&nbsp;</div>' .
                         '<div class="addedit-new-field-value"><div class="addedit-form-cancel-container-actions">';
      $addBtnSuffix =    '</div></div>' .
                         '<div class="clearBoth"></div>' .
                      '</div>'; 
         
       
       include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');  
       $attr_types_arr=getCustomattributeTypeFilters();
       
       expDebug::dPrint('$attr_types_arr='.print_r($attr_types_arr,true),5);
       $attr_li_str='';
       $attr_list='';
       
        
       $onclick_str='';
       $cls_disable='class="disable-custom-attr"';
       
       if(count($attr_types_arr)>0 && $previlage_add_attr=='1'){  //If admin user or
           
       
           $tmp_arr['cattr_type_checkbox']='checkbox';
           $tmp_arr['cattr_type_dropdown']='dropdown';
           $tmp_arr['cattr_type_radio']='radio';
           $tmp_arr['cattr_type_txtarea']='textarea';
           $tmp_arr['cattr_type_txtbox']='textbox'; 
           
           $refresh_opt=1;  
           
           foreach($attr_types_arr as $attr_code => $attr_name){
               
               $attr_short_name=$tmp_arr[$attr_code];
               $tmp_url='administration/manage/customattribute/shortcut/createnew/'.$attr_short_name.'/'.$entityType;
               
               $wid ='405';
               if($attr_short_name=='checkbox' || $attr_short_name=='radio' || $attr_short_name=='dropdown'){
               	$wid = '466';
               }
              
                // Data for Qtip  
                 $qtip_data="{'entityId': '".$entityId."','entityType':'".$entityType."',
                   'url':'".$tmp_url."', 'popupDispId':'new-customattribute-wrapper', 'catalogVisibleId':'new-customattribute-wrapper',
                   'wid':'".$wid."','heg':'120','postype':'topleft','poslwid':'40','qdis':'',
                   'onClsFn':'closeCustomAttributeQtip(".$refresh_opt.")',
                   'linkid':'new-customattribute-link'}";   
            
           
               $attr_li_str.='<li>
                                 <a onclick="clickCustomAttributeDropdown(\''.$attr_code.'\',\''.$lower_entityType.'\');callVisibility('.$qtip_data.') " href="javascript:void(0);"><span class="dropdown-title">'.$attr_name.'</span></a>
                                 </li> 
                                <div class="clearBoth"></div>';
           }
            if($attr_li_str!=''){
                
                $ul_dd_cls='create-dd-list-for-'.$lower_entityType;
                
                $attr_list='<ul id="create-dd-list" class="'.$ul_dd_cls.'" style="visibility:hidden;">
                                <div class="create-dd-list-arrow"></div>'.$attr_li_str.'</ul>';
                                
                $attr_list.='<span id="visible-popup-'.$entityId.'" class="qtip-popup-visible" style="position: absolute; left: 0px; top: 0px; display: block;"></span>
                
                ';                            
            }
            
            
            $onclick_str='onclick="createCustomAttributeDropdown(\''.$lower_entityType.'\');"';  
            $cls_disable='';
       }  
       
       $link_cls='new-customattribute-link-for-'.$lower_entityType;
       
       $form['add_customattribute'.$uniqueId] = array( 
        '#type' => 'markup', 
        '#prefix' => $addBtnPrefix,
        '#suffix' => $addBtnSuffix,
        '#attributes' => $addcustomAttributes,  
        '#markup' => '<div class="new-customattribute"  id="new-customattribute-wrapper"><div id="add-custom-attribute-label" '.$cls_disable.'>'.      
                         ' <a id="new-customattribute-link" class="'.$link_cls.'">'.t('LBL568').'</a>'. 
                          '<span id="create-dd-arrow" class="customattr-btn-bg-right" href="javascript:void(0);" '.$onclick_str.'></span></div>'.
                        $attr_list.'</div>' 
         );
         
          //Hidden Submit for rebuild the form
          
       $addcustomAttributes['class'][] = 'hidden-save-addedit-entityform';
       
       $form['add_customattribute_hidden'.$uniqueId] = array(
              '#type' => 'submit',
              '#value' => t('hidden-save-custom-attr'),
              '#name' => 'hidden-save-custom-attr', 
              '#prefix' => '<span id = "custom-attr-hidden-save-entity-div" style="display:none;">',
              '#suffix' => '</span>',
              '#validate' => array('addNewCustomAttributeOnAddEditFormRebuild'), 
              '#attributes' => $addcustomAttributes,
              '#ajax' => array(
                    'callback' => 'addEditRepaintForm', 
                    'wrapper' => $formId,
                    'event'=>'click',
                    'keypress' => true,
                    'add_custom_gray_field' => false,
                    'custom_attributes_wrapper_id' => $uniqueId,
              ),
        ); 
         
         
      $form['customattributes_wrapper_end'] = array(
        '#type' => 'markup',
        '#markup' => '</div>'
      );
      
      expDebug::dPrint(' $form after build = ' . print_r($form, true),5);
  } catch (Exception $ex) {
    watchdog_exception('addCustomAttributesBlockToAddEditForm', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * addNewCustomAttributeOnAddEditFormRebuild() - 
 */
function addNewCustomAttributeOnAddEditFormRebuild($form, &$form_state) {
    try {
        $form_state['rebuild'] = TRUE; // so that form_submit is not called 
    } catch (Exception $ex) {
        watchdog_exception('addNewCustomAttributeOnAddEditFormRebuild', $ex);
        expertusErrorThrow($ex);
    }
}



/* End Callback for creating shortcust to custom attribute */

?>