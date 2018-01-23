<?php
 
 function displayScreenDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId, $mode = ''){
 try {
	 	global $secure_entities;
	 	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
	 	include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
	    //$screenResult = fetchPrerequisiteList($entityId, $entityType);
	    $screenResult = array();//tmp 
	   if (in_array($entityType, $secure_entities)) {
	   	 $entityId_encrypted = core_encrypt($entityId); // URL encryption
	   } else {
	  	 $entityId_encrypted = $entityId;
	   }
	   $uniqueId = "screen-$entityId_encrypted-$entityType";
	  // $count = count($screenResult); 
	     
  
      // Get no.of screens associated with Entity      
      $associated_ent_arr=getAssociatedEntityCount();  
       
      //Get getScreenDetailsfromDB
      
      $attr_map_details_res=getScreenDetailsfromDB($entityId); 
      expDebug::dPrint('$attr_map_details_res = '.print_r($attr_map_details_res,true),5); 
	  $tmp_mapped_ent_view_arr=array();
	  $tmp_mapped_ent_edit_arr=array();
	  $already_mapped_entities=array();
	  
	  if(count($attr_map_details_res)>0){
			foreach($attr_map_details_res as $map_key => $map_obj){
				$tmp_mapped_ent_view_arr[$map_obj->entity_type]=$map_obj->entity_screen_opt;
				$tmp_mapped_ent_edit_arr[$map_obj->entity_type]=$map_obj->entity_api_opt;
				$already_mapped_entities[]=$map_obj->entity_type; 
			}
	  }
	  
	  expDebug::dPrint(' $already_mapped_entities - '.print_r($already_mapped_entities,true), 5);   
	  
	  if(isset($already_mapped_entities) && is_array($already_mapped_entities) && count($already_mapped_entities)>0){
	  	  $existing_map_entities=implode(',', $already_mapped_entities);
	  }
	  
	  expDebug::dPrint(' $existing_map_entities - '.print_r($existing_map_entities,true), 5); 
      	  
  		
	  $wrapperId = 'screen-grid-wrapper-'.$uniqueId;
	  $formName = 'screen_form_'.$uniqueId;
  	  $form_state['form_unique_name'] = $formName;
  
  
  	  $form['existing_map_entities'] = array(
		      '#type' => 'hidden',
		      '#name' => 'existing_map_entities',
		      '#id' => 'existing_map_entities',
		      '#value' => $existing_map_entities
	  ); 
	  
   	  $form['attr_action'] = array(
		    '#type' => 'hidden',
		    '#name' => 'attr_action',
		    '#value' => 'screen',
	   ); 
		 
	  
	  $form[$formName] = array(
	    '#type' => 'markup',
	    //'#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'" class="admin-datagrid-pagination prereq-grid-wrapper '.$setminHight.'">',
	    '#prefix' => '<div id="admin-data-grid"><div id="'.$wrapperId.'">',
	    '#suffix' => '</div></div>',
	  );  
	
	   $form[$formName]['hidden_idlist'] = array(
	    '#type' => 'hidden',
	    '#name' => 'hidden_idlist_'.$uniqueId,
	    '#id' => 'datagrid-idlist-'.$uniqueId,
	  );     
	   
	   $path = base_path() . drupal_get_path('module', 'uc_credit'); 
	   
	   
	   $form[$formName]['mapped_cus_attr_id'] = array(
		      '#type' => 'hidden',
		      '#name' => 'mapped_cus_attr_id',
		      '#value' => $entityId
		);
		 
		
		/*$form['from_tab'] = array(
		      '#type' => 'hidden',
		      '#name' => 'from_tab',
		      '#value' => 'screen'
		);*/
		
		
		 
	   
	   /*if($theme_key == 'expertusoneV2'){
	 		$style_label = 'padding: 0 4px;';
	 		$style_th="padding: 0 0 0 3px";
	 		$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
	 		$suffixBgRight = '<div class="white-btn-bg-right"></div>';
	 	}else{
	 		$style = 'padding-left: 0';
	 	}
		*/
	 
	   $vtipheader = '<span>&nbsp;<img src="'.$path.'/images/info.png" class="vtip info-enr-upload" title="'.t('MSG751').'" onload="$(\'#multi_sf_org_setup_div\').jScrollPane({});"></span>';
	   
		 
				$vtipheader=''; 
			
			
		$tablelist='<div id="screen_tableholder"><div id="multi_ent_setup_div"><table class="screen-table-heading" cellpadding="3" cellspacing="0" border="0">
		<th id="screen_heading_field" class="heading-background-class-title">'.t('LBL107').'</th>
		<th id="view_heading_field" class="heading-background-class-title">'.t('Screen').'</th>
		<th id="edit_heading_field" class="heading-background-class-title">'.t('LBL2020').' '.$vtipheader.'</th>';
		
		
		$form[$formName]['screen_fieldset'] = array(
        	'#type' => 'markup',
        	'#prefix' => $tablelist,
        	'#suffix' => '</div></div>',
        );  
				
      	 $entity_arr=getEntityOptions(); 
      	 
      	$j=0;
		$tablebodyrow='';
		$rows = array();    
        
       $attribute_limit=10;
       
       
       $previlage_add_attr= adminVisibilityForGlobalAdd('cre_sys_obt_cattr','','','1');
       expDebug::dPrint('$previlage_add_attr  >>= '.print_r($previlage_add_attr,true),5);
        
        $readonly_str='';
        if($previlage_add_attr[0]->addvisible =='0'){
            $readonly_str='-readonly'; 
        }
       
       
		    
		foreach($entity_arr as $ent_code => $ent_name){ 
					 
					  /*  $check_enr_arr=array(); 
					   if($row_value->enr_syncup=='1'){
					        $check_enr_arr="value =" .$row_value->enr_syncup. " checked";
					   }else{
					   	  	$check_enr_arr= ($row_value->enr_syncup=='') ? 0 : "value =" .$row_value->enr_syncup;
					     }*/
					   
			   
			/*  $check_option="value =" .$ent_code. " checked";
			  $check_view_option="value =" .$ent_code. " checked";*/
			  
			  $tmp_view_opt=$tmp_mapped_ent_view_arr[$ent_code];
			  $view_checked='';
			  if($tmp_view_opt==1){
			  	$view_checked=' checked';
			  	$view_class='checkbox-selected'.$readonly_str;
			  } else {
			  	$view_class='checkbox-unselected'.$readonly_str;
			  }
			  
			  $tmp_edit_opt=$tmp_mapped_ent_edit_arr[$ent_code];
			  $edit_checked='';
			  if($tmp_edit_opt==1){
			  	$edit_checked=' checked';
			  	$edit_class='checkbox-selected'.$readonly_str;
			  }else {
			  	$edit_class='checkbox-unselected'.$readonly_str;
			  }
			  
			  $check_option="value =" .$ent_code.$edit_checked;
			  $check_view_option="value =" .$ent_code.$view_checked;
              
              $disable_chk_box='';
              $disable_class='';
              $onclick_callback_screen='';
              $onclick_callback_api='';
              if($tmp_edit_opt!='1' && $tmp_view_opt!='1'){ //if both checkbox are not selected then apply the disable if exists limit 10
                    if($associated_ent_arr[$ent_code]>=$attribute_limit){ //if exits above 10
                       // $disable_chk_box=' disabled';
                        $disable_class=' greyout_ent_lbl'; 
                        $msg_str=escape_string(t('ERR353',array('@count'=>$attribute_limit))). ' ' . $ent_name;
                        $onclick_callback_screen='displayCustomAttributeDisableMessage(\''.$msg_str.'\',\'screen\',this);'; 
                        $onclick_callback_api='displayCustomAttributeDisableMessage(\''.$msg_str.'\',\'api\',this);'; 
                    }
              }
			  
			   $class='odd-list-class';
    		   if($j==1){
    			 $class='even-list-class';
    			 $j=0;
    		   }else{
    			 $j=1;
    		   }
		    		   
				$tablebodyrow.='<tr class="rowtbody row-'.$ent_name.' '.$class.'">
								<td class="check-attributes"><span class="vtip'.$disable_class.'" title="'.sanitize_data(t($ent_name)).'"> '.sanitize_data(t($ent_name)).'</span></td>';
				
				$tablebodyrow.='<td class="check-attributes view_option">
									<div class="publish_checkbox '.$view_class.'">
									 <input type="checkbox"'.$disable_chk_box.' onclick="checkboxSelectedUnselectedCommon(this);'.$onclick_callback_screen.'" name="ent_view_opt[]" id="view_opt_'.$ent_code.'" '.$check_view_option.' >
									 </div>
								</td>';
								
				$tablebodyrow.='<td class="check-attributes edit_option">
									<div class="publish_checkbox '.$edit_class.'">
									 <input type="checkbox"'.$disable_chk_box.' onclick="checkboxSelectedUnselectedCommon(this);'.$onclick_callback_api.'" name="ent_edit_opt[]" id="edit_opt_'.$ent_code.'" '.$check_option.' >
									 </div>
								</td>';
				
		
		    	$tablebodyrow.='</tr>';
				
				$form[$formName]['sf_table_row'] = array(
		    		'#type' => 'markup',
		    		'#prefix' => $tablebodyrow,
		    	);
		}

		/*if($count_details<=0){ //if there is no organization result set then row value is 0 else increase it.
		         $row=0;
		      }else{
		         $row++;
		      }
		  */
		 
		
    	$form[$formName]['screen_table_end'] = array(
       		 '#type' => 'markup',
       		 '#suffix' => '</table></div></div>',
		);  
		 
		
		 
		//screenAddEditFormWithCancelAndSaveClose($form, 'screen-basic-addedit-form', 'screen-save-button', 'screen-cancel-button', 8,$formName);
		screenAddEditFormWithCancelAndSaveClose($form, $wrapperId, 'set_screen_edit_mode', 'cancel_screen_edit_mode', 8,$formName,$entityId,$entityType);
		//expDebug::dPrint('$form formvalue= ' . print_r($form, true), 5);
		
		return $form; 
		
   
  }catch (Exception $ex) {
  	watchdog_exception('displayScreenDetails', $ex);
  	expertusErrorThrow($ex);
  }
}
  
// To get total alloted custom fields to user 
function getTotalAllotedCustomFields($attr_id='',$attr_type=''){ 
	try {
	  	$select = db_select('slt_custom_attr', 'attr');
		$select->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id'); 
		$select->addExpression('COUNT(distinct attr_map.cattr_id)','tot_fld_alloted');  
		$select->condition('attr.status', 'cre_cattr_sts_del','!=');	
		 
		if($attr_id!=''){
		  $select->condition('attr.id', $attr_id,'!=');	
		}
		if($attr_type!=''){
		  $select->condition('attr_map.entity_type', $attr_type,'=');	
		}
		$select->where('attr_map.entity_screen_opt=1 or attr_map.entity_api_opt=1');  
	       
	    expDebug::dPrintDBAPI('Select -->',$select);
		
		$result = $select->execute()->fetchField();  
		
		expDebug::dPrint(' $result - '.print_r($result,true), 5); 
		
	    return $result;
	
  } catch (Exception $ex) {
   		 watchdog_exception('getTotalAllotedCustomFields', $ex);
    	expertusErrorThrow($ex);
  }
}
 
// getMappedEntityAllotedField - To get the count, which is associated with custom attribute   
function getMappedEntityAllotedField($attr_id='',$attr_type=''){ 
    try {
        $select = db_select('slt_custom_attr', 'attr');
        $select->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id');  
        $select->addField('attr_map', 'entity_ref_tbl_col','entity_ref_tbl_col'); 
        $select->condition('attr.status', 'cre_cattr_sts_del','!=');    
        $select->condition('attr.id', $attr_id,'=');
        $select->condition('attr_map.entity_type', $attr_type,'=');   
        
        $select->where('attr_map.entity_screen_opt=1 or attr_map.entity_api_opt=1');  
           
        expDebug::dPrintDBAPI('Select -->',$select);
        
        $result = $select->execute()->fetchField();  
        
        expDebug::dPrint(' $result - '.print_r($result,true), 5);  
        
        return $result;
    
  } catch (Exception $ex) {
         watchdog_exception('getMappedEntityCount', $ex);
        expertusErrorThrow($ex);
  }
} 


// To get total alloted custom fields to user 
function getFreeCustomFields($attr_type){ 
	try {
		  
		$ent_total_fiels_arr=array('e1_cattr0','e1_cattr1','e1_cattr2','e1_cattr3','e1_cattr4','e1_cattr5','e1_cattr6','e1_cattr7','e1_cattr8','e1_cattr9');
		$ent_alloted_fields_arr=explode(",",getAllotedCustomFields($attr_type));
		
		
		$ent_arr_fields[$attr_type]['total_fields']=$ent_total_fiels_arr;
		$ent_arr_fields[$attr_type]['alloted_fields']=$ent_alloted_fields_arr;
		$ent_arr_fields[$attr_type]['free_fields']=array_diff($ent_total_fiels_arr,$ent_alloted_fields_arr);
		
		expDebug::dPrint(' $ent_arr_fields - '.print_r($ent_arr_fields,true), 5); 
	 
	    return $ent_arr_fields; 
	
  } catch (Exception $ex) {
   		 watchdog_exception('getFreeCustomFields', $ex);
    	expertusErrorThrow($ex);
  }
}  


// To get total alloted custom fields to user 
function getAllotedCustomFields($attr_type=''){ 
	try { 
	 
	  	$select = db_select('slt_custom_attr', 'attr');
		$select->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id'); 
		$select->addExpression('group_concat(distinct attr_map.entity_ref_tbl_col)','alloted_fields');  
		$select->condition('attr.status', 'cre_cattr_sts_del','!=');	 
		 
		if($attr_type!=''){
		  $select->condition('attr_map.entity_type', $attr_type,'=');	
		}
		$select->where('attr_map.entity_screen_opt=1 or attr_map.entity_api_opt=1');  
	    $select->groupBy('attr_map.entity_type');  
		   
		   
	    expDebug::dPrintDBAPI('Select -->',$select);
		
		$result = $select->execute()->fetchField();  
		
		expDebug::dPrint(' $result - '.print_r($result,true), 5); 
		
	    return $result; 
	
  } catch (Exception $ex) {
   		 watchdog_exception('getAllotedCustomFields', $ex);
    	expertusErrorThrow($ex);
  }
}  
 
function getScreenDetailsfromDB($entityId){
  try {
    
  	$select = db_select('slt_custom_attr_mapping', 'attr_map');
	$select->addField('attr_map' , 'entity_type');
    $select->addField('attr_map' , 'entity_screen_opt');
    $select->addField('attr_map' , 'entity_api_opt');  
    $select->condition('cattr_id', $entityId);  
    expDebug::dPrintDBAPI('getScreenDetailsfromDB Select -->',$select);
    return $select->execute()->fetchAll();
	
  } catch (Exception $ex) {
    watchdog_exception('getScreenDetailsfromDB', $ex);
    expertusErrorThrow($ex);
  }
}



// To get total alloted custom fields to user  
function getAssociatedEntityCount($attr_type=''){ 
    try { 
     
        $select = db_select('slt_custom_attr', 'attr');
        $select->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id'); 
        $select->addField('attr_map' , 'entity_type','entity_type');
        $select->addExpression('COUNT(1)','tot_fld_associated');  
        $select->condition('attr.status', 'cre_cattr_sts_del','!=');     
         
        if($attr_type!=''){
           $select->condition('attr_map.entity_type', $attr_type,'=');   
        } 
        $select->where('attr_map.entity_screen_opt=1 or attr_map.entity_api_opt=1');  
        $select->groupBy('attr_map.entity_type');  
           
           
        expDebug::dPrintDBAPI('Select -->',$select);
        
        $result = $select->execute()->fetchAll();  
        
        expDebug::dPrint(' $result - '.print_r($result,true), 5); 
        $ent_cnt_arr=array();
       if(count($result)>0){
           foreach($result as $key=>$res){ 
               $ent_cnt_arr[$res->entity_type]=$res->tot_fld_associated;
           }
       }
        expDebug::dPrint(' $ent_arr res - $ent_cnt_arr - '.print_r($ent_cnt_arr,true), 5);
        
        return $ent_cnt_arr; 
    
  } catch (Exception $ex) {
         watchdog_exception('getAssociatedEntityCount', $ex);
        expertusErrorThrow($ex);
  }
}  

 
 function addEditRepaintScreenForm($form, &$form_state) {
	try {
  global $secure_entities;
  expDebug::dPrint(' addEditRepaintScreenForm() called.');
  expDebug::dPrint(' addEditRepaintScreenForm() $form_state[esign_show_popup].'.$form_state['esign_show_popup'] , 5);
  $form_state['storage']['mode'] = 'edit';
  
  $formName = $form_state['form_unique_name'];
  $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  
  if (in_array($entityType, $secure_entities)) {
  	$uniqueId = "screen-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive data
  } else {
  	$uniqueId = "screen-".$entityId."-$entityType"; // URL encryption to retrive data
  }
  
  $commands = array();
  if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
    $commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
  }
  $commands[] = exp_ctools_modal_adjust();
  
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
    // Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
  }
  
  if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){  
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
    $esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup']==true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj)); 
      $form_state['esign_show_popup']=false;
    }
  }
  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditRepaintScreenForm', $ex);
  	expertusErrorThrow($ex);
  }
}
 
 
 
 function screenAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex,$formName,$entityId,$entityType) {
	try {
		//$esignHiddenButt = 'esign-'.$wrapperId;
		$esignHiddenButt = 'esign-screen-button';
		//$customHiddenButtonId = 'esign-prerequisite-button';
		global $theme_key;
		$prefixBgLeft='';$suffixBgRight='';
		if($theme_key == 'expertusoneV2'){
			$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
			$suffixBgRight = '<div class="white-btn-bg-right"></div>';
		}
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

		$cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
		$cancelAttributes['tabindex'][] = $tabIndex + 1;

		// A common class for all addedit form cancel button, for styling to appear as a link.
		$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
		//$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
		
		//$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();'."closeQtip('','$entityId');  return false; ";
		
		//$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();'."closeQtip('','$entityId'); $('#root-admin').data('narrowsearch').refreshGrid();return false; ";
		$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();'."closeQtip('','$entityId'); $('#root-admin').data('narrowsearch').refreshLastAccessedRow();return false; ";

		$form[$formName][$cancelButtonId] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
				'<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
				'#suffix' => $suffixBgRight,
				// Suffix to close the two divs is added in #suffix property of Save button
				//'#value' => t('LBL123'), //Close
				'#value' => t('LBL123'),
				'#attributes' => $cancelAttributes,
				// '#type' => 'hidden',
		);

		// Add the Save button
		$ajaxCallbackSave = 'addEditRepaintScreenForm'; 

		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

		$previlage_attr1 = adminVisibilityForGlobalAdd('cre_sys_obt_cattr','','','1');
		expDebug::dPrint('$previlage_attr  >>= '.print_r($previlage_attr1,true),5);
		
		if($previlage_attr1[0]->addvisible =='0')
		{
			$saveAttributes['class'][] = 'admin-greyout-bg';
			expDebug::dPrint('$cdcvsdxvcsdcvvcdxv  >>= '.print_r($saveAttributes,true),5);
		}

		$saveAttributes['class'][] = 'admin-save-button-middle-bg';
		$saveAttributes['tabindex'][] = $tabIndex;
		// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
		$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
		// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
		$saveAttributes['data-wrapperid'] = array($wrapperId);

		$eSignatureAvailable = true;
		
		if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
             $buttonName  = 'esign-screen'; 
         }else{
              $buttonName  = 'set-screen';
         }
		
		//For Hidden Save
				
		$HiddenButtNew='hidden-screen-button';
		$form[$HiddenButtNew] = array(
				'#type' => 'submit',
				'#value' => t('hidden-save'),
				'#name' => 'hidden-save',
				'#prefix' => '<span id = "hidden-customattribute-screen-admin-div" style="display:none">',
				'#suffix' => '</span>',
				'#id' =>$HiddenButtNew,
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
			
		
		if($previlage_attr1[0]->addvisible ==0){
			$form[$formName][$saveButtonId] = array(  
					// '#prefix' => '<div class="admin-save-button-container" id="admin-save-screen-button><div class="admin-save-button-left-bg"></div>',
					// '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
					'#prefix' => '<div class="admin-save-button-container" id="admin-save-screen-button">' .
					'<div class="admin-save-button-left-bg"></div>',
			 '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
					'</div>' .
					'<div class="clearBoth"></div>' .
					'</div>' . // see #prefix of close button above
								'<div class="clearBoth"></div>' .
					'</div>', // see #prefix of close button above
									
								'#title' => t('LBL569'),
										'#value' => t('LBL569'),
												'#name' => $buttonName,
												//'#submit' => array('addNewPrerequisiteDetails'),
												'#attributes' => $saveAttributes,
														'#ajax' => array(
																'wrapper' => $wrapperId,
																'callback' => $ajaxCallbackSave,
																'entity_id' => $entityId,
																'entity_type' => $entityType,
																'entity_plugin'=>'screen',
																'customButtonId'=>$esignHiddenButt,
																'method' => 'replace',
																'effect' => 'none', // 'fade',
																'event'=>'click',
																'keypress' => true,
																'progress' => array(
																'type' => 'throbber',
																'message' => ''
														),
														),
														);
			
		}else{
		$form[$formName][$saveButtonId] = array( 
              '#type' => 'submit',
             
			 // '#prefix' => '<div class="admin-save-button-container" id="admin-save-screen-button><div class="admin-save-button-left-bg"></div>',
    	     // '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
    	     '#prefix' => '<div class="admin-save-button-container" id="admin-save-screen-button">' .
					'<div class="admin-save-button-left-bg"></div>',
			 '#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
					'</div>' .
					'<div class="clearBoth"></div>' .
					'</div>' . // see #prefix of close button above
					'<div class="clearBoth"></div>' .
					'</div>', // see #prefix of close button above		
					
              '#title' => t('LBL569'),
              '#value' => t('LBL569'),
              '#name' => $buttonName,
              //'#submit' => array('addNewPrerequisiteDetails'), 
              '#attributes' => $saveAttributes,
              '#ajax' => array(
        		    'wrapper' => $wrapperId,
        		    'callback' => $ajaxCallbackSave,
                    'entity_id' => $entityId, 
                    'entity_type' => $entityType,
                    'entity_plugin'=>'screen',
                    'customButtonId'=>$esignHiddenButt,
        		      'method' => 'replace',
        		      'effect' => 'none', // 'fade',
        		      'event'=>'click',
        		      'keypress' => true,
        		      'progress' => array(
        		        'type' => 'throbber',
        		        'message' => ''
        		      ),
               ),  
            );
		}
		
		
            
			 
			
			 	

		/*if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
			$form[$formName][$saveButtonId] = array(
					'#type' => 'submit',
					'#value' => t('LBL569'), // Save - Done
					'#name' => $buttonName,
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
							'entity_id' => $entityId, 
		                    'entity_type' => $entityType,
		                    'entity_plugin'=>'screen',
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
			$saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
			$form[$formName][$saveButtonId] = array(
					'#type' => 'button',
					'#prefix' => '<div class="admin-save-button-container" id="admin-save-screen-button">' .
					'<div class="admin-save-button-left-bg"></div>',
					'#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
					'</div>' .
					'<div class="clearBoth"></div>' .
					'</div>' . // see #prefix of close button above
					'<div class="clearBoth"></div>' .
					'</div>', // see #prefix of close button above
					'#value' => t('LBL569'), // Save - Done
					'#name' => $buttonName,
					'#attributes' => $saveAttributes,
			);
		} */

		unset($saveAttributes['tabindex']);
		
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

		// Close form div
		$form['addedit-left-col-end'] = array(
				'#type' => 'markup',
				'#markup' => '</div><div class="clearBoth"></div>',
		);
	}
	catch (Exception $ex) {
		watchdog_exception('screenAddEditFormWithCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}  
  
 
/*
 * getEntityMappingDetails() - get custom attribute and related entity fields
 * $entity_id - id of custom attribute
 */
function getEntityMappingDetails($entity_id) {
	try {
		
			$selectStmt = db_select('slt_custom_attr', 'attr'); 
			$selectStmt->innerJoin('slt_custom_attr_mapping', 'attr_map','attr_map.cattr_id=attr.id'); 
			
			$selectStmt->addField('attr', 'id','cattr_id'); 
			$selectStmt->addField('attr_map', 'entity_type','entity_type'); 
			$selectStmt->addField('attr_map', 'entity_ref_tbl_col','entity_ref_tbl_col');
			$selectStmt->addField('attr_map', 'entity_screen_opt','entity_screen_opt');
			$selectStmt->addField('attr_map', 'entity_api_opt','entity_api_opt'); 
			 
			 $selectStmt->condition('attr_map.cattr_id', $entity_id, '=');
			 $selectStmt->groupBy('attr_map.entity_type');
			 
		  	expDebug::dPrintDBAPI('$selectStmt -  ', $selectStmt);
			$cattr_map_res = $selectStmt->execute()->fetchAll();  
			
			$map_arr=array();
			
			if(count($cattr_map_res)>0){
				foreach($cattr_map_res as $map_key => $map_obj){  
					$map_arr[$map_obj->entity_type]=$map_obj->entity_ref_tbl_col;
				}
			}
			expDebug::dPrint('$map_arr for this entity  ' . print_r($map_arr, 1), 5); 
			 
			return $map_arr;	 
	 
  } catch (Exception $ex) {
  	watchdog_exception('getEntityMappingDetails', $ex);
  	expertusErrorThrow($ex);
  }
}
 
 
function addNewScreenDetails($form, &$form_state,$entityId,$entityType,$callFromApi =0) {
	  	 
	 // global $secure_entities;
	  $txn = db_transaction();
	  
	  try { 
	  	
			expDebug::dPrint(' addNewScreenDetails() $form_state[input].'.print_r($form_state['input'],true) , 5);
			expDebug::dPrint(' addNewScreenDetails() $form_state[values].'.print_r($form_state['values'],true) , 5); 
			//expDebug::dPrint(' $secure_entities = '.print_r($secure_entities,true), 5);
			
			$mapped_cus_attr_id=$form_state['values']['mapped_cus_attr_id'];
		    $attr_action=$form_state['values']['attr_action'];
			$userId = getSltpersonUserId();
			
			if($mapped_cus_attr_id!='' && $attr_action=='screen' && ($form_state['input']['_triggering_element_name']=='set-screen' || $form_state['input']['_triggering_element_name']=='hidden-save')){ //if attribute set
				 
				 
				 //Get Previous mapping details of custom attribute 
				 $prev_attr_map_arr=getEntityMappingDetails($mapped_cus_attr_id);
				 expDebug::dPrint(' $prev_attr_map_arr = '.print_r($prev_attr_map_arr,true), 5);
				 
				 //removing already mapped entities
				 $deleteMapQuery = db_delete('slt_custom_attr_mapping')->condition('cattr_id', $mapped_cus_attr_id);
				 expDebug::dPrintDBAPI('$deleteQuery for mapped entities  = ', $deleteMapQuery);
				 $deleteMapQuery->execute();
				 add_audit_trail_entry($userId, $mapped_cus_attr_id, $entityType , 'exp_sp_administration_customattribute', 'Screen', 'Deleting attribute screen mappings', null, null , null);
		
				
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
				
				
				if(count($tot_ent_map_count)>0){
					
					foreach($tmp_ent_unique_arr as $ent_key => $ent_val){
						$view_opt=0;
						$edit_opt=0;
						
						$ent_attr_map_code=trim($ent_val);
						
						if(in_array($ent_attr_map_code, $tmp_ent_view_opt_arr)){
							$view_opt=1;
						}
						if(in_array($ent_attr_map_code, $tmp_ent_edit_opt_arr)){
							$edit_opt=1;
						}
						
						$free_ent_arr=getFreeCustomFields($ent_attr_map_code); 
						 
						 $tmp_arr=array();
						 if(count($free_ent_arr[$ent_attr_map_code]['free_fields'])>0){
						 	foreach($free_ent_arr[$ent_attr_map_code]['free_fields'] as $key => $val){
						 		$tmp_arr[]=$val;
						 	}
						 }

						expDebug::dPrint('$tmp_arr='.print_r($tmp_arr,true),5);
						 
						$alloting_field=$tmp_arr[0];
						
						expDebug::dPrint('$alloting_field='.print_r($alloting_field,true),5);
						
						$insertStmt = db_insert('slt_custom_attr_mapping');
				            $custom  = NULL;
				          	
				            $fields = array(
				                      'cattr_id'                => $mapped_cus_attr_id,
				                      'entity_type'             => $ent_attr_map_code,
				            		  'entity_ref_tbl_col'      => $alloting_field,
				                      'entity_screen_opt'    	    => $view_opt,
				            		  'entity_api_opt'         => $edit_opt
				                       
				                    );
				            $insertStmt->fields($fields);			            
				            $screenId = $insertStmt->execute(); 
				            //add_audit_trail_entry($userId, $mapped_cus_attr_id, $ent_attr_map_code , 'exp_sp_administration_customattribute_screen', 'Screen', 'Inserted $entityType', null, null , $associatedEntityId);
							 
							make_audit_trail_entries($fields, $userId , null ,$mapped_cus_attr_id, 'Inserted','cre_sys_obt_cattr','exp_sp_administration_customattribute','addNewScreenDetails',
							array('cattr_id', 'entity_type', 'entity_ref_tbl_col', 'entity_screen_opt', 'entity_api_opt')); 
							
					}
					
				}
 

				//Update slt_custom_attr table  
				$attrFields['updated_by'] = $userId;
				$attrFields['updated_on'] = now(); 
				
				$update = db_update('slt_custom_attr');
				$update->fields($attrFields); 
				$update->condition('id', $mapped_cus_attr_id,'='); 
				$update->execute();
				
				
				//Get current mapping details of custom attribute after save the new changes
				$cur_attr_map_arr=getEntityMappingDetails($mapped_cus_attr_id);
				expDebug::dPrint(' $cur_attr_map_arr = '.print_r($cur_attr_map_arr,true), 5);
				 
				if(count($prev_attr_map_arr)>0){
					foreach($prev_attr_map_arr as $pre_key => $pre_val){
						
						if(!array_key_exists($pre_key,$cur_attr_map_arr)){
							//Unmap respective coulumn 
							expDebug::dPrint('There is no mapping for this entity ='.$pre_key.'--'.$pre_val, 5);
							emptyEntityTableColumnValue($pre_key,$pre_val,$userId); 
						}else{
							expDebug::dPrint(' Mapping is there= '.$pre_key.'--'.$pre_val, 5);
						}
						
					}
				} 
				
					
				$form_state['storage']['mode'] = 'view';
			    $form_state['rebuild'] = TRUE;
			    //expertus_set_message(t('MSG601').'.'); 
                expertus_set_message(t('LBL2014').' '.t('MSG601').'.');		
			} 
	    
	      
	  }catch (Exception $ex) {
		    $txn->rollback();
		    watchdog_exception('addNewScreenDetails', $ex);
		    if($callFromApi){
		      //return array((object)array('id'=>'Failiure'));
		    	$errobj=new stdClass();
		    	$errobj->isValidateError = 1;
		    	$errobj->errcode = 'L_012';
		    	$errobj->errormsg = "Sceen Id is not valid";
		    	return $errobj;
		    }
		    throw $ex;    
	  }
	  unset($txn);
} 

 
function addNewScreenDetailsForShotCut($form, &$form_state,$entityId,$entityType){ 
      $txn = db_transaction(); 
      try {
          
            $ent_attr_map_code=$entityType;
            $mapped_cus_attr_id=$entityId;
            $view_opt=1;
            $edit_opt=1;
            $userId = getSltpersonUserId();
            
             //removing already mapped entities 
             /*$deleteMapQuery = db_delete('slt_custom_attr_mapping')->condition('cattr_id', $mapped_cus_attr_id);
             expDebug::dPrintDBAPI('$deleteQuery for mapped entities  = ', $deleteMapQuery);
             $deleteMapQuery->execute();
             add_audit_trail_entry($userId, $mapped_cus_attr_id, $entityType , 'exp_sp_administration_customattribute', 'Screen', 'Deleting attribute screen mappings from Shortcut', null, null , null);
             */
              
            $free_ent_arr=getFreeCustomFields($ent_attr_map_code);    
             
             $tmp_arr=array();
             if(count($free_ent_arr[$ent_attr_map_code]['free_fields'])>0){
                foreach($free_ent_arr[$ent_attr_map_code]['free_fields'] as $key => $val){
                    $tmp_arr[]=$val;
                }
             }

            expDebug::dPrint('$tmp_arr='.print_r($tmp_arr,true),5);
             
            $alloting_field=$tmp_arr[0];
            
            expDebug::dPrint('$alloting_field='.print_r($alloting_field,true),5);
            
            $insertStmt = db_insert('slt_custom_attr_mapping');
                $custom  = NULL;
                
                $fields = array(
                          'cattr_id'                => $mapped_cus_attr_id,
                          'entity_type'             => $ent_attr_map_code,
                          'entity_ref_tbl_col'      => $alloting_field,
                          'entity_screen_opt'           => $view_opt,
                          'entity_api_opt'         => $edit_opt
                           
                        );
                $insertStmt->fields($fields);                       
                $screenId = $insertStmt->execute(); 
                //add_audit_trail_entry($userId, $mapped_cus_attr_id, $ent_attr_map_code , 'exp_sp_administration_customattribute_screen', 'Screen', 'Inserted $entityType', null, null , $associatedEntityId);
                 
                 if($form_state['values']['from_migration']=='1'){
                     $module_name='exp_sp_administration_customattribute_migration';
                 }else{
                     $module_name='exp_sp_administration_customattribute';
                 }
                 
                make_audit_trail_entries($fields, $userId , null ,$mapped_cus_attr_id, 'Inserted','cre_sys_obt_cattr',$module_name,'addNewScreenDetailsForShotCut',
                array('cattr_id', 'entity_type', 'entity_ref_tbl_col', 'entity_screen_opt', 'entity_api_opt')); 
                
                
                 if($form_state['values']['from_migration']=='1'){
                   return $alloting_field; //Return the alloting field in entity table for custom attribute. It is applicable only for migration
                 }
                 
                
                //Update slt_custom_attr table  
                /*$attrFields['updated_by'] = $userId;
                $attrFields['updated_on'] = now(); 
                
                $update = db_update('slt_custom_attr');
                $update->fields($attrFields); 
                $update->condition('id', $mapped_cus_attr_id,'='); 
                $update->execute();*/
                
                
      }catch (Exception $ex) {
            $txn->rollback();
            watchdog_exception('addNewScreenDetailsForShotCut', $ex); 
            throw $ex;    
      }
      unset($txn);                       
}

/*
 *  emptyEntityTableColumnValue() - To empty the column for respective entity table
 * 
 */
function emptyEntityTableColumnValue($entity_type,$ref_column,$userId='') {  
	try { 
        
           $entity_type_arr=getEntityTableInformation($entity_type); 
           expDebug::dPrint('$entity_type_arr='.print_r($entity_type_arr,true),5);
           
           $tbl_name=$entity_type_arr['table_name'];   
           expDebug::dPrint('$tbl_name='.$tbl_name,5); 
           
		  /* if($entity_type=='cre_usr'){
		   	  $tbl_name='slt_person';
		   }else if($entity_type=='cattr_ent_cls'){
		   	  $tbl_name='slt_person';
		   }*/
		  
		    $fields[$ref_column]='';
            $fields['updated_on']= now();
            if($userId!=''){
                 $fields['updated_by']= $userId;
            }
		   
		    $upd_qry = db_update($tbl_name);
		    $upd_qry->fields($fields); 
            $where_str=$ref_column.'!=\'\' OR '.$ref_column.' IS NOT NULL';
            $upd_qry->where($where_str);
		   
		    expDebug::dPrintDBAPI('$upd_qry =', $upd_qry);
		    $upd_qry->execute();
	
		
	}catch (Exception $ex) {
  	watchdog_exception('emptyEntityTableColumnValue', $ex);
  	expertusErrorThrow($ex);
  }
}

?>
