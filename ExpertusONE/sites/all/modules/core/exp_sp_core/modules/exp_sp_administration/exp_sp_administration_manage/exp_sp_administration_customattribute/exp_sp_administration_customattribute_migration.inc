<?php

function migrateCustomAttributeFromCustomFields(){
   try {  
       //Create Custom Attribute()
        $cfield_res=getDistinctCustomFieldsForCustomAttribute();     
           
        expDebug::dPrint('Started Migration Process for Custom Attribute',1);  
        
        if(count($cfield_res)>0){
               
               expDebug::dPrint('Started to create Custom Attribute for Migration',1);
                 
               foreach($cfield_res as $cfield_key => $cfield_val){
                   $custom_field_label=$cfield_val->custom_field_label;
                   $max_val_char_count=$cfield_val->max_val_char_count; //Max value for each custom field
                   $mapped_ent=$cfield_val->mapped_ent;
                   $custom0=$cfield_val->custom0;  
                   $mapped_ent_arr=explode(',',$mapped_ent); 
                   
                   //Create Custom Attribute for textbox / textarea according to length
                   createMigCustomAttribute($custom_field_label,$max_val_char_count,$mapped_ent_arr,$custom0);
                   
               }
                expDebug::dPrint('Ending to create Custom Attribute for Migration',1);  
                
               //Load Custom Field Values into Custom Attribute, which are created through migration
               expDebug::dPrint('Started to loading Custom Attribute for Migration',1);  
               loadCustomFieldDataIntoCustomAttribute();
               expDebug::dPrint('Ending to loading Custom Attribute for Migration',1);  
                
               
               
        }else{
            expDebug::dPrint('No custom fields are avaiable to migrate',1);  
        }  
        
        expDebug::dPrint('Ending Migration Process for Custom Attribute',1); 
        
   }catch (Exception $ex) {
    watchdog_exception('migrateCustomAttributeFromCustomFields', $ex);
    throw $ex;
  }   
} 

/*
 * loadCustomFieldData() - To load the data from custom field to custom attribute 
 */

function loadCustomFieldDataIntoCustomAttribute(){ 
     try {
            $selectStmt = db_select('slt_custom_fields', 'scf'); 
            
            $selectStmt->addField('scf', 'label','custom_field_label'); 
            $selectStmt->addField('scf', 'entity_id','entity_id');            
            $selectStmt->addField('scf', 'entity_type','entity_type');   
            //group_concat(scf.custom1,'="',replace(scf.value,'"','\"'),'"')
            $srch_str='"';
            $replace_str='~dquote~';
            $fld_set_str="GROUP_CONCAT(scf.custom1,'=\"',REPLACE(scf.value,'".$srch_str."','".$replace_str."'),'\"')";
           // $selectStmt->addExpression('GROUP_CONCAT(scf.custom1,\'="\',REPLACE(scf.value,\'"\',\'\"\'),\'"\')','field_set');
            $selectStmt->addExpression($fld_set_str,'field_set');   
            
            $selectStmt->WHERE('scf.custom0!=\'\' and scf.custom1!=\'\' and (scf.custom2 is null or scf.custom2=\'\')');
            $selectStmt->groupBy('scf.entity_type,scf.entity_id');
            $selectStmt->orderBy('scf.entity_type', 'ASC');     
                        
            expDebug::dPrintDBAPI('$selectStmt -  ', $selectStmt);
            $cfield_load_res = $selectStmt->execute()->fetchAll();
            expDebug::dPrint('Custom Fields Load Data - $cfield_load__res ' . print_r($cfield_load_res, 1), 5); 
            
            $load_count=count($cfield_load_res);
            expDebug::dPrint('$load_count  ' . print_r($load_count, 1), 5); 
             
            if($load_count>0){
                
                 $tbl_ent_arr = array('course'=>'slt_course_template', //Set array for Custom Field Code = Custom Attribute Code 
                                                 'class'=>'slt_course_class',
                                                 'cre_sys_obt_trp'=>'slt_program',
                                                 'user'=>'slt_person',
                                                 'organization'=>'slt_organization', 
                                                 'location'=>'slt_location');
                    
                $idle_count=0;                             
                foreach($cfield_load_res as $load_key => $load_val){
                    
                    $idle_count++;
                    
                    if($idle_count==1000){
                      expDebug::dPrint('Idle start',5);
                      usleep(500000); //Wait for half a second
                      expDebug::dPrint('Idle end',5);
                      $idle_count=0; //Reset the idle count    
                    }
                    
                   $entity_id=$load_val->entity_id;
                   $entity_type=$load_val->entity_type;
                   $field_set=$load_val->field_set;
                   $tbl_name=$tbl_ent_arr[$entity_type];
                   
                   
                   if($field_set!='' && $entity_id!='' && $tbl_name!=''){ //if not set
                        //Update the entity table 
                       $replace_custom_field_value=str_replace('~dquote~','\"',$field_set); //Replce the double quote 
                       $load_up_query="UPDATE ".$tbl_name." set ".$replace_custom_field_value." WHERE id = ".$entity_id;
                       expDebug::dPrint('$load_up_query='.$load_up_query,5);
                       db_query($load_up_query);
                       
                        //Update the entity table 
                       $cfield_up_query="UPDATE slt_custom_fields set custom2=".$entity_id." WHERE custom0!='' and custom1!='' and (custom2 is null or custom2='') and entity_id = ".$entity_id." and entity_type='".$entity_type."'";
                       expDebug::dPrint('$cfield_up_query='.$cfield_up_query,5);
                       db_query($cfield_up_query);
                       
                   }else{
                       expDebug::dPrint('No update process happen',1);  
                   }
                    
                }
                
                
            }else{
                expDebug::dPrint('No custom data avaiable to migrate',1);  
            }
            
              
      
   }catch (Exception $ex) {
    watchdog_exception('loadCustomFieldDataIntoCustomAttribute', $ex);
    throw $ex;
  }   
} 
/*
 * createMigCustomAttribute() - To create textbox and textarea custom attribute according to their label's char length
 *  Set 1 in Custom 0 if attr created 
 */

function createMigCustomAttribute($attr_name,$max_val_char_count,$mapped_ent_arr=array(),$custom0=''){
     try {
         
         expDebug::dPrint('$attr_name ='.print_r($attr_name,true),5);
         expDebug::dPrint('$max_val_char_count ='.print_r($max_val_char_count,true),5);
         expDebug::dPrint('$mapped_ent_arr ='.print_r($mapped_ent_arr,true),5); 
         expDebug::dPrint('$custom0 ='.print_r($custom0,true),5); 
         
         $form_state['values']['from_migration'] = '1';   // Set migration flag
           
         if($custom0>0){ //if already created through migration, not requirred to process
              $newCustomAttributeId=$custom0;
              expDebug::dPrint(' Custom Attribute ' .$attr_name. ' already exists.$newCustomAttributeId - '.$newCustomAttributeId, 5);
              
         }else{ 
             
              $replaced_attr_name=preg_replace('/[^A-Za-z0-9 ]/', '', trim($attr_name)); // Removes all spcial chars except special chars.
              expDebug::dPrint('$replaced_attr_name ='.print_r($replaced_attr_name,true),5);  
                  
              $form_state['values']['cattr_name'] = trim($replaced_attr_name);  
              $form_state['values']['cattr_mandatory'] = 'N';
              $form_state['values']['cattr_unique'] = 'N';
              $form_state['values']['cattr_help_txt']='';
              $form_state['values']['cattr_status'] = 'cre_cattr_sts_atv';   
              
              if($max_val_char_count<=255){ //if textbox
                   $form_state['values']['cattr_type'] = 'cattr_type_txtbox';
                   $form_state['values']['cattr_datatype'] = 'cattr_dtype_text';
                   $form_state['values']['cattr_length'] = 255;
              }else{ //if textarea
                   $form_state['values']['cattr_type'] = 'cattr_type_txtarea'; 
                   $form_state['values']['cattr_length'] = 500;
              }
              
              $loggedInUserId = getIdOfLoggedInUser();
              
              $returnArr = addNewCustomAttributeDetails($form, $form_state,$loggedInUserId); 
              expDebug::dPrint(' $returnArr = ' . print_r($returnArr, true), 5); 
              $newCustomAttributeId = $returnArr[0]->id;   
             
             //Update attribute flag
              $custom0Updated = db_update('slt_custom_fields')
                        ->fields(array('custom0' => $newCustomAttributeId))
                        ->condition('label',$attr_name,'=')
                        ->execute();
              expDebug::dPrint('$custom0Updated = '.$custom0Updated,5); 
             
         } 
          
         if($newCustomAttributeId>0){ //if create new custom attributed then updated the custom attribute id for custom 0 value  
            
            // Map entity with custom attribute, which is created newly.  
             expDebug::dPrint('count($mapped_ent_arr) = '.count($mapped_ent_arr),5); 
             if(count($mapped_ent_arr)>0){ 
                 $cfield_attr_ent_arr = array('course'=>'cre_sys_obt_crs', //Set array for Custom Field Code = Custom Attribute Code 
                                                 'class'=>'cre_sys_obt_cls',
                                                 'cre_sys_obt_trp'=>'cre_sys_obt_trp',
                                                 'user'=>'cre_usr',
                                                 'organization'=>'cre_org', 
                                                 'location'=>'cre_sys_obt_loc');
             
             
                  foreach($mapped_ent_arr as $map_ent_key=>$map_ent_code_val){ 
                                         
                      $attr_ent_code=$cfield_attr_ent_arr[$map_ent_code_val]; 
                      $ent_cnt_arr=getAssociatedEntityCount($attr_ent_code);  //get associated entities for this item   
                      $no_of_assoc_ent=$ent_cnt_arr[$attr_ent_code];   
                      expDebug::dPrint('$no_of_assoc_ent = '.$no_of_assoc_ent,5);    
                      
                      //Check that whether already mapped or not for this custom attribute
                      $alloted_field=getMappedEntityAllotedField($newCustomAttributeId,$attr_ent_code);   
                      expDebug::dPrint('$alloted_field = '. print_r($alloted_field,true),5);  
                         
                      
                      if($no_of_assoc_ent < 10){ //eligible to attach and not mapped this entity to this custom attribute
                           
                           if($alloted_field!=''){ //if already field is there update the field
                               $alloting_field=$alloted_field;
                           }else{ //if no field is  available
                                expDebug::dPrint('Mapping custom attribute '.$attr_name.'('.$newCustomAttributeId.')'.' with this entity '.$attr_ent_code,5); 
                               $alloting_field=addNewScreenDetailsForShotCut($form, $form_state,$newCustomAttributeId,$attr_ent_code);    
                               expDebug::dPrint('Alloting field for an entity - $alloting_field - '.$alloting_field,5);
                           } 
                              //Once mapped the custom attribute with entity then updating the flag alloted field for custom 1 in slt_custom_fields
                               $custom1Updated = db_update('slt_custom_fields')
                                        ->fields(array('custom1' => $alloting_field))
                                        ->condition('label',$attr_name,'=')
                                        ->condition('entity_type',$map_ent_code_val,'=')
                                        ->execute();
                               expDebug::dPrint('$custom1Updated = '.$custom1Updated,5);  
                           
                      }else{
                           expDebug::dPrint('Unable to map this custom attribute '.$attr_name.'('.$newCustomAttributeId.')'.' with this entity '.$attr_ent_code,5);
                      }
                  }
                  
             }else{
                  expDebug::dPrint('No mapping available for this attribute -' . $attr_name, 1); 
             } 
                             
          }  
            
   }catch (Exception $ex) {
        watchdog_exception('createMigCustomAttribute', $ex);
        throw $ex;
  }              
} 


// getAlreadyAllotedCustom1Field - To check and get the custom1 field al
/*function getAlreadyAllotedCustom1Field($attr_id='',$custom_field_type=''){ 
    try {
        $select = db_select('slt_custom_fields', 'scf'); 
        $selectStmt->addField('scf', 'custom1','custom1'); 
        $select->condition('scf.custom0', $attr_id,'=');    
        $select->condition('scf.entity_type', $custom_field_type,'='); 
        
        expDebug::dPrintDBAPI('Select -->',$select);
        
        $result = $select->execute()->fetchField();  
        
        expDebug::dPrint(' $result - '.print_r($result,true), 5); 
        
        return $result;
    
  } catch (Exception $ex) {
         watchdog_exception('checkAlreadyAllotedCustom1Field', $ex);
        expertusErrorThrow($ex);
  }
} */


/*
 * getDistinctCustomFieldsForCustomAttribute() - get distinct custom field  from slt_custom_fields 
 * Assume that 10 fields will be there in existing custom fields. Custom field has priorities according to no.of.mapping and no.of records for each custom field label
 * //label  tot_mapped_ent  mapped_ent  tot_records max_val_char_count 
 *  BulkExport 2   course,class    13978   4
 * Create textbox if char lenght is below 255 or textarea attribute if exceeds above 255
 */
function getDistinctCustomFieldsForCustomAttribute() {
    try {
        
            $selectStmt = db_select('slt_custom_fields', 'scf');  
            
            $selectStmt->addField('scf', 'label','custom_field_label'); 
            $selectStmt->addExpression('COUNT(distinct scf.entity_type)','tot_mapped_ent');
            $selectStmt->addExpression('GROUP_CONCAT(distinct scf.entity_type)','mapped_ent'); 
            $selectStmt->addExpression('COUNT(1)','tot_records');
            $selectStmt->addExpression('MAX(CHAR_LENGTH(scf.value))','max_val_char_count'); 
            $selectStmt->addField('scf', 'custom0','custom0');
            
           /* $selectStmt->condition('attr_map.entity_type', $entity_type, '=');
            $selectStmt->condition('attr.status', 'cre_cattr_sts_atv', '=');
            $selectStmt->condition('attr_map.entity_screen_opt', '1', '=');*/ 
            //$selectStmt->range(0,10); //tmp
            
            $selectStmt->groupBy('scf.label');
            $selectStmt->orderBy('tot_mapped_ent', 'DESC');        
            $selectStmt->orderBy('tot_records', 'DESC');        
                        
            expDebug::dPrintDBAPI('$selectStmt -  ', $selectStmt);
            $cfield_res = $selectStmt->execute()->fetchAll(); 
            
            expDebug::dPrint('Custom Fields Distinct Results - $cfield_res ' . print_r($cfield_res, 1), 5);
             
            return $cfield_res;   
     
  } catch (Exception $ex) {
    watchdog_exception('getCustomAttributeDetails', $ex);
    expertusErrorThrow($ex);
  }
}




?>