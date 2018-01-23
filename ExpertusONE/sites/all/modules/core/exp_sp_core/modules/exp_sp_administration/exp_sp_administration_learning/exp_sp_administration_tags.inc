<?php

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */


function getTagDetailsPop($entityId,$entityType, $emptyId) {
	try {
    if($entityType == 'Class'){
      $courseId = _getCourseId($entityId);
    }
	$entityType = ereg_replace('-',' ',$entityType);
	$select = db_select('slt_tag_entity','tagentity');
	$select->Join('slt_tagdefn', 'tagdef', 'tagentity.tagid=tagdef.id');
	$select->addField('tagentity','id', 'tagentity_id');
	$select->addField('tagentity','entity_type', 'entity_type');
	$select->addField('tagdef','id', 'tag_id');
	$select->addField('tagdef','tagname', 'tag_name');
	
	if($entityId == 0){
		$select->condition('tagentity.custom0', $emptyId,'=');
	} else {
		if($entityType == 'Class'){
		  //$select->condition('tagentity.entity_id', array($entityId, $courseId), 'IN');
		    $select_and1 = db_and();
			$select_and1->condition('tagentity.entity_id', $entityId, '=');
			$select_and1->condition('tagentity.entity_type', $entityType, '=');
			if($courseId > 0 && is_numeric($courseId))
			{
				$select_and2 = db_and();
				$select_and2->condition('tagentity.entity_id', $courseId, '=');
				$select_and2->condition('tagentity.entity_type', "Course", '=');
				
				$select_or = db_or();
				$select_or->condition($select_and1);
				$select_or->condition($select_and2);
				$select->condition($select_or);
			}
			else
				$select->condition($select_and1);
			expDebug::dPrintDBAPI('--Tag getTagDetailsPop------------' , $select);    
			
		} else {
		  $select->condition('tagentity.entity_id', $entityId,'=');
        }
	}
	if($entityType == 'Class'){
		if($entityId == 0)
			$select->condition('tagentity.entity_type', array($entityType), 'IN');
	} else {
	  $select->condition('tagentity.entity_type', $entityType,'=');
	}
	$select->orderBy('tagentity_id');
	$result = $select->execute()->fetchAll();	
    expDebug::dPrintDBAPI('--Tag getTagDetailsPop------------' , $select);    
	return $result;
	} catch (Exception $ex) {
		watchdog_exception('getTagDetailsPop', $ex);
		expertusErrorThrow($ex);
	}
}

function _getCourseId($classId){
	try {
  $query = db_select('slt_course_class', 'class');
  $query->addField('class', 'course_id');
  $query->condition('class.id', $classId);
  return $query->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('_getCourseId', $ex);
  	expertusErrorThrow($ex);
  }
}


function isTagExits($tagname,$entityId='',$entityType='',$emptyId='') {
 try {
	 	$select = db_select('slt_tagdefn','tagdef');
 		$select->addField('tagdef','id', 'tag_id');
 		if(!empty($entityId)&&!empty($entityType))
 		{
  			$select->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid');
  			$select->condition('tagdefent.entity_id', $entityId,'=');
  			$select->condition('tagdefent.entity_type', $entityType,'=');
 		}
		elseif (!empty($emptyId) && !empty($entityType)){
 		  	$select->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid');
          	$select->condition('tagdefent.custom0', $emptyId,'=');
 		}
 		$select->condition('tagdef.entity_type', $entityType,'=');
 		$select->condition('tagdef.tagname', $tagname,'=');
		expDebug::dPrintDBAPI("tag def --->>> " ,$select);
 		$result = $select->execute()->fetchAll();
 		return $result[0]->tag_id;
	} catch (Exception $ex) {
		watchdog_exception('isTagExits', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * tags_addedit_form() - Renders and returns JSON for tags form with drupal settings
 */
function tags_addedit_form($renderMode, $entityId, $entityType, $emptyId, $entityTitle) {
	try {
  $min_form_state       = array(
    'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
    're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
    'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
    'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
  );
  $form_state                                = $min_form_state;
	
  $form_state['build_info']['args'][] = $entityId;
  $form_state['build_info']['args'][] = $entityType;
  $form_state['build_info']['args'][] = $emptyId;

  $formId = 'exp_sp_administration_tags_addedit_form_html';
  $builtForm = drupal_build_form($formId, $form_state);
  $renderedForm = drupal_render($builtForm);

  $drupalSettings = getDrupalSettings();
  $response = array(
    'drupal_settings' => $drupalSettings,
    'render_content_main' => $renderedForm,

  ); 
  echo drupal_json_output($response);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('tags_addedit_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function exp_sp_administration_tags_addedit_form_html($form, &$form_state, $renderMode, $entityId, $entityType, $emptyId) {
	try {
  $uniqueId          = $entityId.'-'.$entityType;
  expDebug::dPrint('UNIQUE ID TAGS   '.$uniqueId , 4);
  expDebug::dPrint('emptyId   '.$emptyId , 4);
  
  $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
  global $theme_key;
  	if($theme_key == 'expertusoneV2') {
  		//NEWUI THEME STYLE SHEET
  		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_v2.css', $css_theme_optional);
  	} else {
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  		
  	}
  //drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.css', $css_theme_optional);  

  $form['tag_fieldset'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="tag-fieldset-wrapper-'.$uniqueId.'" class="admin-tab-list-row tag-fieldset-wrapper">',
    '#suffix' => '</div>',
  );  

  $wrapper = 'tag-fieldset-wrapper-'.$uniqueId;
  if (!empty($entityId) || $entityId == 0) { 
  		$tagDetails = getTagDetailsPop($entityId, $entityType, $emptyId);
  }
  $tagCount = count($tagDetails);
  
  	$noTags = t('MSG349');
  	$tagsMessage = t('MSG350');
  
  if($tagCount == 0){
  	$form['tag_fieldset']['tag_list_message'] = array(
	          '#type' => 'item',   
	          '#markup' => $noTags,
  			  '#prefix' => '<div class="tag-list-message">',
  			  '#suffix' => '</div>',
	          '#name' => 'Tag Message',        
	  ); 
  }else{
	  $form['tag_fieldset']['tag_list_message'] = array(
	          '#type' => 'item',   
	          '#markup' => $tagsMessage,
	  		  '#prefix' => '<div class="tag-list-message noempty">',
  			  '#suffix' => '</div>',
	          '#name' => 'Tag Message',        
	  ); 
  }
  $form['tag_fieldset']['tag']['list-begin'] = array(
	          '#markup' => '<div id="tag-scroll-id" class="addedit-tag_list"><ul id="tag-inline-show">',
	  ); 
  expDebug::dPrint('$tagDetails  >> '.$tagCount , 4);
  expDebug::dPrint('$form_state["delete_tag_fieldset"] >> '.print_r($form_state["delete_tag_fieldset"],true) , 5);
  expDebug::dPrint('$form_state["delete_tag_index"] >> '.print_r($form_state["delete_tag_index"],true) , 5);
  
  $tagsArray = array();
  for ($i = 0,$j=0; $i < $tagCount; $i++) {
  	  
  	if($entityType == 'Class' && $tagDetails[$j]->entity_type == 'Course' && $tagDetails[$j]->entity_type == 'Class'){
        $readonlyTag = 1;
      } else {
        $readonlyTag = 0;
      }

  	  $tagsArray[]=$tagDetails[$j]->tag_id;
  	  $tag_name = rawurlencode($tagDetails[$j]->tag_name);
  	  //$tag_name = str_replace('"',"&quot;", $tag_name);
  	  //$tag_name = str_replace("'","&apos;", $tag_name);
  	   $data = 'data={"emptyId":"'.$emptyId.'","entityId":"'.$entityId.'","entityType":"'.$entityType.'","tagName":"'.$tag_name.'","tagEntityId":"'.$tagDetails[$j]->tagentity_id.'","tagId":"'.$tagDetails[$j]->tag_id.'"}';
  	   expDebug::dPrint('DATA ----------------->'.$data);
  		$data = str_replace('"',"&quot;", $data);

      if($entityType == 'Class' && $tagDetails[$j]->entity_type == 'Course' && $tagDetails[$j]->entity_type == 'Class'){
        $tagInfo = '<a href="javascript:void(0);" id="tag-list-name-id-'.$tagDetails[$j]->tagentity_id.'" class="tag-list-name-readonly" >'.sanitize_data($tagDetails[$j]->tag_name).'</a>';
      } else {
            if($entityType == 'Class' && $tagDetails[$j]->entity_type == 'Course')
            {
              $msg[0]="Tag is attached to course. It can only be edited from course.";
              $tagInfo = '<a href="javascript:void(0);" id="tag-list-name-id-'.$tagDetails[$j]->tagentity_id.'" class="tag-list-name" onclick="$(\'#show_expertus_message\').html(expertus_error_message(a=[\''.$msg[0].'\'],\'error\'));"  data="'.$data.'"">'.sanitize_data($tagDetails[$j]->tag_name).'</a>';
            }
            else
              $tagInfo = '<a href="javascript:void(0);" id="tag-list-name-id-'.$tagDetails[$j]->tagentity_id.'" class="tag-list-name"  data="'.$data.'" ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEdit(\''.$data.'\');">'.sanitize_data($tagDetails[$j]->tag_name).'</a>';
          }
  		
  		$form['tag_fieldset']['tag']['list_tag_'.$i] = array(
          '#type' => 'item',
          '#name' => 'Tag List'.$i,
          '#prefix' => '<li><span id="tag-name-div-'.$tagDetails[$j]->tagentity_id.'">'.$tagInfo.'</span>',
        );
  		if($readonlyTag){
  		
  			$form['tag_fieldset']['tag']['tag_delete_'.$i] = array(
  					'#type' => 'markup',
  					'#markup' => '</li></div>'
  			);
  			 
  		} else {
                        if(!($entityType == 'Class' && $tagDetails[$j]->entity_type == 'Course'))
                        {
                            $form['tag_fieldset']['tag']['tag_delete_'.$i] = array(
  					'#type' => 'submit',
  					//'#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
  					'#value' => 'X',
  					'#title' => t('LBL286'),
  					'#name' => 'Delete Tag'.$i.$uniqueId,
  					'#prefix' => '&nbsp;&nbsp;',
  					'#suffix' => '',
  					'#submit' => array('deleteNewTagDetails'),
  					'#attributes' => array('Class' => 'tag-delete-btn'),
  					'#ajax' => array(
  							'wrapper' => 'tag-fieldset-wrapper-'.$uniqueId,//$wrapperId,
  							'callback' => 'addEditRepaintForm',
  							'param' => $tagDetails[$j]->tag_id,
  							'entity_id' => $entityId,
  							'entity_type' => $entityType,
  							'empty_id' => $emptyId,
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
  		}
  		if($j!=($tagCount-1)) {
  			$assignClass = $readonlyTag ? 'tag-seperator-readonly' : 'tag-seperator';
  			$form['tag_fieldset']['tag']['tag_separator_'.$i] = array(
  					'#type' => 'item',
  					'#name' => 'Tag Separator'.$i,
  					'#prefix' => '<span class="'.$assignClass.'">|',
  					'#suffix' => '</span></li>'
  			);
  		}
        
    
       
     /*  $form['tag_fieldset']['tag']['tag_delete_'.$i] = array(
      		'#type' => 'submit',
      		'#suffix' => '</div>'
      		); */
      		
      addHiddenValueToAddEditForm($form, 'tagentity_id_'.$i, $tagDetails[$j]->tagentity_id);
        $tagId = $tagDetails[$j]->tag_id!=''?$tagDetails[$j]->tag_id:$form_state['input']['tag_fieldset']['tag']['hid_tag_id_'.$i];
        addHiddenFieldToTagsFieldSet($form, $i, 'tag', $tagId);
        
        $j++;
     
  }
  $form['tag_fieldset']['tag']['list-ends'] = array(
  		'#markup' => '</ul></div>',
  );
  $excludedTagsId = implode(',', $tagsArray);
  $excludedTagsId = empty($excludedTagsId) ? 0 : core_encrypt($excludedTagsId);
  
  $acMenuPath = 'administration/learning/tags/tags-autocomplete';
  $preTextValue = t('LBL578');
  $additionalClass = array();
  $additionalClass['class'][] = 'admin-addedit-tags-ac-input';
  addACFieldSetToAddEditForm($form, 'tag',$i.$uniqueId , $acMenuPath, '', '', 4001, array(), array('id' => '"tag_id"', 'exclude_tag_ids'=>"'$excludedTagsId'",'entityType'=>"'$entityType'"), false, false,$preTextValue, $additionalClass);
  $form['tag_fieldset']['tag_hidden_delete'] = array(
    '#type' => 'value',
    '#value' => implode(",",$form_state['delete_tag_fieldset'])    
  );
  $popuprenderTagsId = "paintContentqtip_visible_disp_tags_".$entityId.'_'.$entityType;
  $dispId = "renderTagsId".$entityId.'_'.$entityType;
  $cancelButtonId                      = 'tagsCloseButton';
  $ajaxCallbackCancel                  = 'addEditCancelRepaintForm';  
  $cancelAttributes['class']           = array('addedit-edit-' . $cancelButtonId);  
  $cancelAttributes['class'][]         = 'admin-action-button-middle-bg white-btn-bg-middle';
  $cancelAttributes['class'][]         = 'addedit-form-expertusone-throbber';
  $cancelAttributes['class'][]         = 'tags-close-button';
  $cancelAttributes['data-wrapperid']  = array($wrapper);  
  $cancelAttributes['class'][]         = 'exp-addedit-form-cancel-button';
 // $cancelAttributes['onclick']         = '$("#'.$popuprenderTagsId.'").remove();return false;';
  $cancelAttributes['onclick']         = "closeQtip('','$entityId');  return false; ";
  $cancelAttributes['tabindex'][] = 4003;
  
  
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<table cellpadding="0" cellspacing="0" border="0"><tr><td><div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions"><div class="white-btn-bg-left"></div>',
    '#suffix' =>   '<div class="white-btn-bg-right"></div></div></div></td>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );
 
  $addButtonId                      = 'tagsSubmitButton';
  $addTagAttributes['class'][] = 'addedit-form-expertusone-throbber';
  $addTagAttributes['class'][] = 'admin-save-button-middle-bg';
  $addTagAttributes['class'][] = $addButtonId.$uniqueId;
  $addTagAttributes['tabindex'][] = 4002;
  
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $addTagAttributes['data-wrapperid'] = array('tag-fieldset-wrapper-'.$uniqueId);
  $form[$addButtonId] = array(
      '#type' => 'submit',  
      '#prefix' => '<td><div class="admin-save-button-left-bg"></div>',
      '#suffix' => '<div class="admin-save-button-right-bg"></div></td></tr></table>',
  	  '#attributes' => $addTagAttributes,
  	  '#submit' => array('addCourseTagsDetails'),
      '#value' => t('LBL287'),
      '#ajax' => array(
		      'wrapper' => $wrapper,//$wrapperId,
		      'callback' => 'addEditTagsRepaintForm',
  			  'element_no' => $i.$uniqueId,
  			  'entity_id' => $entityId,
        	  'entity_type' => $entityType,
          'empty_id' => $emptyId,
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
    
    
  addHiddenValueToAddEditForm($form, 'entity_id', $entity_id);
  addHiddenValueToAddEditForm($form, 'entity_type', $entityType);
  //jQuery(\'input[name="tag_name_'.$i.$uniqueId.'"]\').focus();
  $form['tag_fieldset']['tag_cursor'] = array(
     '#type' => 'markup',
   	 '#markup' => '<script type="text/javascript">(function ($) { 
			$(\'input[name="tag_name_'.$i.$uniqueId.'"]\').live(\'keydown\',function(event){
			    if(event.keyCode == 13){
			        $(".ac_results").css("display","none");
			        event.preventDefault();
        			event.stopPropagation();
			        $(".'.$addButtonId.$uniqueId.'").click();	
			        
			}
			});})(jQuery);</script>',
  );  
 
 	syncSolrData('Program');
	syncSolrData('Course');
 	syncSolrData('Class');
 
  return $form;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_tags_addedit_form_html', $ex);
  	expertusErrorThrow($ex);
  }
}

function exp_sp_administration_tags_addedit_form_html_validate($form, &$form_state) {
 expDebug::dPrint('exp_sp_administration_tags_addedit_form_html_validate' , 4);
 $i = $form_state['triggering_element']['#ajax']['element_no'];
 // Commented for -- 0053651: Unable to enter tags in french language.
 if(!empty($form_state['values']['tag_name_'.$i]) && $form_state['values']['tag_name_'.$i]!= t('LBL578') && trim($form_state['values']['tag_name_'.$i]) != ''){ 
 //   if((preg_match('#[\~\`\!\#\^\&\*\(\)\-\+\=\{\}\[\]\|\:\;\<\>\.\?\@\"\/\\\\]+#', $form_state['values']['tag_name_'.$i]))) {
 //     expDebug::dPrint("Vincent test",1);   
 //    	$err_msg = t('LBL191').' '.t('MSG639');
 //        form_set_error('content_data_url', $err_msg);
 //        return false;
 //     }
 	/* 0050012: Tags creation in course and class level shows error message when using some special characters	 */
//  	if (!preg_match("/^[a-zA-Z0-9,.@$\'\- |\/\\\\]*$/",$form_state['values']['tag_name_'.$i])) {
//      	$err_msg = t('LBL191').' '.t('MSG639');
//      	form_set_error('content_data_url', $err_msg);
//      	return false;
//      }
   }
   if($form_state['values']['tag_name_'.$i] == t('LBL578')) 
   {    
       $err_msg = t('LBL752').' '.t('LBL578');
       form_set_error('content_data_url', $err_msg);
       return false;
     }
     } 
function _getEntityType($tagId){
	try {
  $query = db_select('slt_tag_entity', 'tag');
  $query->addField('tag','entity_type');
  $query->condition('', $tagId);
  $query->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('_getEntityType', $ex);
  	expertusErrorThrow($ex);
  }
}

function deleteNewTagDetails($form, &$form_state){
  $txn = db_transaction();

  try {
		 if($_REQUEST['apiname']=='DeleteTagByAPI'){
			$tagId = $form_state['tag_id'];
  			$entityId = $form_state['entity_id'];
  			$entityType = $form_state['entity_type'];
  			$emptyId = 0;
			$userid = $_REQUEST['userid'];
		}
		else{
		$tagId = $form_state['triggering_element']['#ajax']['param'];
  		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
  		$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
  		$entityType = ereg_replace('-',' ',$entityType);
  		$emptyId = $form_state['triggering_element']['#ajax']['empty_id'];
		$userid=getIdOfLoggedInUser();
		}
		
		if($entityType == 'Course'){
			$updst = db_update('slt_course_class');
			$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
			$updst->fields($fields);
			$updst->condition('course_id',$entityId,'=');
			$updst->execute();
			$upd = db_update('slt_course_template');
		}
		else if($entityType == 'Class')
			$upd = db_update('slt_course_class');
		else
			$upd = db_update('slt_program');
		$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
		$upd->fields($fields);
		$upd->condition('id',$entityId,'=');
		expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
		$upd->execute();
		
		// Get the fields before deleting
		$loggeduserId = getSltpersonUserId();
		$select = db_select('slt_tag_entity', 'tagent');
		$select->addField('tagent','id','id');
		$select->addField('tagent','tagid','tagid');
		$select->addField('tagent','entity_id','entity_id');
		$select->addField('tagent','entity_type','entity_type');
		$select->condition('entity_type', $entityType,'=');
  		if($entityId == 0){
		    $select->condition('custom0', $emptyId,'=');
		} else {
		    $select->condition('entity_id', $entityId,'=');
		}        
        $select->condition('tagid', $tagId,'=');
		$tagEntInfo =$select->execute()->fetchObject();
		
		$deleteStmt = db_delete('slt_tag_entity');  		
        $deleteStmt->condition('entity_type', $entityType,'=');
  		if($entityId == 0){
		    $deleteStmt->condition('custom0', $emptyId,'=');
		} else {
		    $deleteStmt->condition('entity_id', $entityId,'=');
		}        
        $deleteStmt->condition('tagid', $tagId,'=');
        expDebug::dPrintDBAPI('$deleteStmt', $deleteStmt);
        $numDeleted = $deleteStmt->execute();
        
        // Make deleted log entry
        $fields = array(
                'table_name' => 'slt_tag_entity',
                'entity_id' => $tagEntInfo->id,
                'entity_type' => 'tag_entity',
                'parent1_entity_id' => $tagEntInfo->entity_id,
                'parent1_entity_type' => $tagEntInfo->entity_type,
                'parent2_entity_id' => $tagEntInfo->tagid,
                'parent2_entity_type' => 'tagid',
                'module_name' => 'exp_sp_administration_tags',
                'functionality_name' => 'deleteNewTagDetails',
                'api_name' => $_REQUEST['apiname'],
                'deleted_on' => now(),
                'deleted_by' => $loggeduserId
        );
        deleted_log_entry($fields);
        
        //foreach($tagIdArr as $record){    
        	$deleteFormState = array();
        	$deleteFormState['id'] =  $tagId;  
        	make_audit_trail_entries($deleteFormState,$userid , null ,$entityId, 'Deleted',$entityType,'exp_sp_administration_tags','old tag',
                                          array('id'));    	
        //}  
        

        $selectStmt = db_select('slt_tag_entity');
      	$selectStmt->addExpression('count(id)','actcnt');
      	$selectStmt->condition('tagid',$tagId,'=');
      	expDebug::dPrintDBAPI('$selectStmt', $selectStmt);
      	$count = $selectStmt->execute()->fetchField();
      	expDebug::dPrint('$count = ' . $count);
        
      	expDebug::dPrint(' Delete IDDDDDDDDDDDDDDDDDDDDDDDD' . $count.'-'.$tagId , 3);
      	
      	
        $updateStmt = db_update('slt_tagdefn');
        $custom  = NULL;  
    	
        $fields = array(
                      'activity_count'     => $count,
                      'updated_by'         => $userid,
                      'updated_on'         => now(),
                      'custom0'            => $emptyId,
                      'custom1'            => $custom,
                      'custom2'            => $custom,
                      'custom3'            => $custom,
                      'custom4'            => $custom   
                    );

        $updateStmt->fields($fields);    
        $updateStmt->condition('id', $tagId,'=');
        $numUpdated = $updateStmt->execute();
    
        expDebug::dPrint(' $$numDeleted = ' . print_r($numDeleted, true) , 3);
		if($_REQUEST['apiname']=='DeleteTagByAPI'){
           $res=new stdClass();
           $res->status='Deleted Successfully';
           return $res;
	       }
    	   else{
    	             $form_state['rebuild'] = TRUE; 
    	   } 
    syncSolrData('Program');
	syncSolrData('Course');
 	syncSolrData('Class');
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('deleteNewTagDetails', $ex);
    throw $ex;    
  }
  
  unset($txn);  

	
}


function addCourseTagsDetails($form, &$form_state) {
	try {
  expDebug::dPrint('$form_staterrrrrrrrr'.print_r($form_state,true),5);
  if($_REQUEST['apiname']=='CreateTagByAPI')
  { //Added by Rajeshwar
    $entityId =$form_state['entity_id']; // $form_state['triggering_element']['#ajax']['entity_id'];
    $entityType = $form_state['entity_type']; //$form_state['triggering_element']['#ajax']['entity_type'];
    $emptyId = 0;
    $tagname=$form_state['tagname'];
    $userid = $_REQUEST['userid'];
  }
  else
  {
    $i = $form_state['triggering_element']['#ajax']['element_no'];
    $entityId = $form_state['triggering_element']['#ajax']['entity_id'];
    $entityType = $form_state['triggering_element']['#ajax']['entity_type'];
    $entityType = ereg_replace('-',' ',$entityType);
    $emptyId = $form_state['triggering_element']['#ajax']['empty_id'];
    $tagname=$form_state['values']['tag_name_'.$i];
    $userid = getIdOfLoggedInUser();
  }
  expDebug::dPrint('$emptyId try >>>>>>>>>.'.print_r($emptyId,1),5);
  if($entityId != 0){
  	$emptyId=NULL;
  }
  $txn = db_transaction();
	
  try {
	if(!empty($tagname) && $tagname!= t('LBL578') && $tagname != ''){
			$tagEntId = isTagExits($tagname,$entityId,$entityType,$emptyId);
			expDebug::dPrint('$tagEntId: '.print_r($tagEntId,true), 4);
			if(!empty($tagEntId) || $tagEntId<>'') {
                            $err_msg = t('LBL191').' '.t('LBL271');
                            expDebug::dPrint('$_REQUEST[apiname]: '.print_r($_REQUEST['apiname'],true), 4);
                            if($_REQUEST['apiname']=='CreateTagByAPI')
                            {
                                expDebug::dPrint('$_REQUEST[apiname]: '.print_r($_REQUEST['apiname'],true), 4);
                                return APIerror(1,'L_013','Tags already exist.');
                            }
                            else
                            {				
				form_set_error('content_data_url', $err_msg);
				return false;
                            }
			}
			$tagDefId = isTagExits($tagname,NULL,$entityType);
			$activity_count_details = checkCourseClassTpStatus($entityId,$entityType);
      if(empty($tagDefId) || $tagDefId=='') {
          $insertStmt = db_insert('slt_tagdefn');
          $custom  = NULL;
		  $activity_count = ($activity_count_details == 1 ) ? 1 :0;
          /* Ticket: 46992 - module_type 'module_type'        => $entityType,*/
          $fields = array(
                    'tagname'            => trim($tagname),                  
                    'activity_count'     => $activity_count,
                    'entity_type'        => $entityType,
                    'created_by'         => $userid,
                    'created_on'         => now(),
                    'updated_by'         => $userid,
                    'updated_on'         => now(),
                    'custom0'            => $emptyId,
                    'custom1'            => $custom,
                    'custom2'            => $custom,
                    'custom3'            => $custom,
                    'custom4'            => $custom   
                  );
          /* Ticket: 46992 - module_type */
          $insertStmt->fields($fields);			            
           expDebug::dPrintDBAPI("Insert sql >>>>>>>>>." , $insertStmt);
          $tagDefId = $insertStmt->execute();
      }else{
        if($_REQUEST['apiname']=='CreateTagByAPI'){
                    $tagname = db_query('select tagname from slt_tagdefn where id='. $tagDefId)->fetchCol()[0];
        }
      	$selectStmt = db_select('slt_tag_entity');
      	$selectStmt->addExpression('count(id)','actcnt');
      	$selectStmt->condition('tagid',$tagDefId,'=');
      	$count = $selectStmt->execute()->fetchField();
      	$updStmt = db_update('slt_tagdefn');
          $custom  = NULL;
        	if($activity_count_details=='1'){
			$updStmt->expression('activity_count', 'activity_count + 1');
		}
          $fields = array(
                    'updated_by'         => $userid,
                    'updated_on'         => now(),
                    'custom0'            => $emptyId,
                    'custom1'            => $custom,
                    'custom2'            => $custom,
                    'custom3'            => $custom,
                    'custom4'            => $custom   
                  );
          $updStmt->fields($fields);	
          $updStmt->condition('id',$tagDefId,'=');
		  if($status=='lrn_crs_sts_itv'){
		  $updStmt->condition('activity_count','0','>');
		  }		            
          //$updStmt->condition('module_type',$entityType,'=');
          expDebug::dPrintDBAPI("Update sql >>>>>>>>>." , $updStmt);
          $tagDefIdUpd = $updStmt->execute();
      }
      
      
      $insertStmt = db_insert('slt_tag_entity');
      $custom  = NULL;
    	
      $fields = array(
                'entity_type'        => $entityType,                  
                'entity_id'          => $entityId,
                'tagid'              => $tagDefId,
                'created_by'         => $userid,
                'created_on'         => now(),
                'updated_by'         => $userid,
                'updated_on'         => now(),
                'custom0'            => $emptyId,
                'custom1'            => $custom,
                'custom2'            => $custom,
                'custom3'            => $custom,
                'custom4'            => $custom   
              );
      $insertStmt->fields($fields);		
       expDebug::dPrintDBAPI("Insert sql detail >>>>>>>>>. " , $insertStmt);
      $tagId = $insertStmt->execute();
      
      if($entityType == 'Course'){
      	$updst = db_update('slt_course_class');
      	$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
      	$updst->fields($fields);
      	$updst->condition('course_id',$entityId,'=');
      	$updst->execute();
      		$upd = db_update('slt_course_template');
      }
      else if($entityType == 'Class')
      	$upd = db_update('slt_course_class');
      else 
      	$upd = db_update('slt_program');
      $fields = array('updated_on'=> now() , 'updated_by'=> $userid);
      $upd->fields($fields);
      $upd->condition('id',$entityId,'=');
      expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
      $upd->execute();
      
      
      $attFormState = array();
      $attFormState['id'] = $tagDefId;
      $attFormState['tagname'] =  $tagname;
      make_audit_trail_entries($attFormState, $userid , null ,$entityId, 'Inserted',$entityType,'exp_sp_administration_tag','mapped tag',
      array('id', 'tagname'));
      expDebug::dPrint(' $$tagId = ' . print_r($tagId, true) , 3);   
       if($_REQUEST['apiname']=='CreateTagByAPI'){
           $res=new stdClass();
           $res->id=$tagDefId;
           $res->tagname= $tagname;
           return $res;
       }
       else{
                 $form_state['rebuild'] = TRUE; 
       }
  	}
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addtag', $ex);
    throw $ex;    
  }
  
  unset($txn);
  syncSolrData('Program');
  syncSolrData('Course');
  syncSolrData('Class');
  } catch (Exception $ex) {
  	watchdog_exception('addCourseTagsDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function checkCourseClassTpStatus($entityId,$entityType)
{
  try {
  	 if($entityType == 'Course'){
  	 	 $query = db_select('slt_course_template', 'slt_db');
		 $status_type = 'lrn_crs_sts_atv';
  	 }elseif($entityType == 'Learning Plan'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Training Plan'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Certification'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Curricula'){
  	 	 $query = db_select('slt_program', 'slt_db');
		 $status_type = 'lrn_lpn_sts_atv';
  	 }elseif($entityType == 'Class'){
  	 	 $query = db_select('slt_course_class', 'slt_db');
		 $status_type = 'lrn_cls_sts_atv';
  	 }elseif($entityType == 'sry_det_typ_sry_qus'){
  	 	 $query = db_select('slt_survey_questions', 'slt_db');
		 $status_type = 'sry_qtn_sts_atv';
  	 }elseif($entityType == 'sry_det_typ_ass_qus'){
  	 	 $query = db_select('slt_survey_questions', 'slt_db');
		 $status_type = 'sry_qtn_sts_atv';
  	 }elseif($entityType == 'Content'){
  	 	 $query = db_select('slt_content_master', 'slt_db');
		 $status_type = 'lrn_cnt_sts_atv';
  	 }	 
	 
	  $query->addExpression('count(id)','recdcnt');
	  $query->condition('slt_db.id', $entityId);
	  $query->condition('slt_db.status', $status_type);
	  expDebug::dPrintDBAPI("checkCourseClassTpStatus sql >>>>>>>>>." , $query);
	  return $query->execute()->fetchField();
  } catch (Exception $ex) {
  	watchdog_exception('checkCourseClassTpStatus', $ex);
  	expertusErrorThrow($ex);
  }
}

function tags_update($entityId,$emptyId,$entityType,$newTag,$tagEntityId){	
	$entityType = ereg_replace('-',' ',$entityType);
	$txn = db_transaction();
	try {
		if($entityType == 'Course'){
			$updst = db_update('slt_course_class');
			$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
			$updst->fields($fields);
			$updst->condition('course_id',$entityId,'=');
			$updst->execute();
			$upd = db_update('slt_course_template');
		}
		else if($entityType == 'Class')
			$upd = db_update('slt_course_class');
		else
			$upd = db_update('slt_program');
		$fields = array('updated_on'=> now() , 'updated_by'=> $userid);
		$upd->fields($fields);
		$upd->condition('id',$entityId,'=');
		expDebug::dPrintDBAPI("Update entity sql >>>>>>>>>." , $upd);
		$upd->execute();
		if($_REQUEST['apiname']=='UpdateTagByAPI'){
		$currentUserId = $_REQUEST['userid'];
		}
		else{
		$newTag = rawurldecode($newTag);
		$newTag = str_replace(EXP_AC_SEPARATOR, '/', $newTag);
		$currentUserId = getIdOfLoggedInUser();
                }
		$old_entityType = $entityType;
		$old_entityId = $entityId;
		if($newTag != '' && !empty($newTag)){
			if($entityType=="Class"){
				$chkclasstagentselect = db_select('slt_tag_entity','tagentity');
				$chkclasstagentselect->addField('tagentity','entity_id', 'entity_id');
				$chkclasstagentselect->addField('tagentity','entity_type', 'entity_type');
				$chkclasstagentselect->condition('tagentity.id', $tagEntityId,'=');
				expDebug::dPrintDBAPI("tags_chkexisttag_entity >>>>>>>>>." , $chkclasstagentselect.'----'.$entityType,5);	
				$chkclassexisttag_entity = $chkclasstagentselect->execute()->fetchAll();
				if($chkclassexisttag_entity[0]->entity_type == "Course"){
					$entityType = "Course";
					$entityId = $chkclassexisttag_entity[0]->entity_id;
				}else{
					$entityType=$entityType;
				}
			}
			
			$tagEntId = isTagExits($newTag,$entityId,$entityType,$emptyId);
			if(!empty($tagEntId) || $tagEntId<>'') {
				if($_REQUEST['apiname']=='UpdateTagByAPI'){
					$err_msg = t('LBL191').' '.t('LBL271');
					return APIerror(1,'L_013',$err_msg);
				}
				else{
					echo "error";
					exit;
				}
			}
			$tagDefId = isTagExits($newTag,NULL,$entityType);
			$activity_count_details = checkCourseClassTpStatus($entityId,$entityType);
			db_query("update slt_tagdefn set activity_count = activity_count-1 where id IN (select tagid from slt_tag_entity WHERE id=$tagEntityId) and activity_count > 0 AND entity_type='$entityType'");
			if(empty($tagDefId) || $tagDefId == ""){
				   $insertStmt = db_insert('slt_tagdefn');
          		   $custom  = NULL;
		  		   $activity_count = ($activity_count_details == 1 ) ? 1 :0;
          $fields = array(
                    'tagname'            => trim($newTag),
                    'activity_count'     => $activity_count,
                    'entity_type'        => $entityType,
                    'created_by'         => $currentUserId,
                    'created_on'         => now(),
                    'updated_by'         => $currentUserId,
                    'updated_on'         => now(),
                    'custom0'            => $emptyId,
                    'custom1'            => $custom,
                    'custom2'            => $custom,
                    'custom3'            => $custom,
                    'custom4'            => $custom   
                  );

          		$insertStmt->fields($fields);		
		    	expDebug::dPrintDBAPI("tags_update sql >>>>>>>>>." , $insertStmt);	            
	          	$newTagId = $insertStmt->execute();        	
	 			$tagIdUpdate =  $newTagId;
			}else{								
											
				$select = db_select('slt_tag_entity','tagentity');
				$select->addField('tagentity','id', 'tagentityid');
				$select->condition('tagentity.tagid', $tagDefId,'=');
				$select->condition('tagentity.entity_type', $entityType,'=');
				$select->condition('tagentity.entity_id', $entityId,'=');
				if($entityId == 0){
					$select->condition('tagentity.custom0', $emptyId,'='); 
				}	
				$result = $select->execute()->fetchAll();
			    $alreadyAssoc = $result[0]->tagentityid;
			    if(empty($alreadyAssoc) || $alreadyAssoc == ""){
					$selectStmt = db_select('slt_tag_entity');
			      	$selectStmt->addExpression('count(id)','actcnt');
			      	$selectStmt->condition('tagid',$tagDefId,'=');
			      	$count = $selectStmt->execute()->fetchField();
			      	$custom  = NULL;					
					$updStmt = db_update('slt_tagdefn');
					if($activity_count_details=='1'){
						$updStmt->expression('activity_count', 'activity_count + 1');
					}
			          $fields = array(
			                    'updated_by'         => $currentUserId,
			                    'updated_on'         => now(),
			                    'custom0'            => $emptyId,
			                    'custom1'            => $custom,
			                    'custom2'            => $custom,
			                    'custom3'            => $custom,
			                    'custom4'            => $custom   
			                  );
          			$updStmt->fields($fields);
          			$updStmt->condition('id',$tagDefId,'=');
          			expDebug::dPrintDBAPI("Update sql >>>>>>>>>." , $updStmt);
          			$tagDefIdUpd = $updStmt->execute();
					$tagIdUpdate =  $tagDefId;
			    }
			}
			
				$chktagentselect = db_select('slt_tag_entity','tagentity');
				$chktagentselect->addExpression('COUNT(id)', 'count');
				$chktagentselect->condition('tagentity.entity_type', $entityType,'=');
				$chktagentselect->condition('tagentity.entity_id', $entityId,'=');				
				if($entityId == 0){
					$chktagentselect->condition('tagentity.custom0', $emptyId,'='); 
				}
				if($tagEntityId != '' && !empty($tagEntityId)){
					$chktagentselect->condition('tagentity.id', $tagEntityId,'='); 
				}
				$chkexisttag_entity = $chktagentselect->execute()->fetchField();	
			if($chkexisttag_entity > 0){			
				$updStmt = db_update('slt_tag_entity');
			    $custom  = NULL;	        	
			    $fields = array(
			         'tagid' => $tagIdUpdate,
			    	 'custom0' => $emptyId
			    );
		        $updStmt->fields($fields);	
		        $updStmt->condition('id',$tagEntityId,'=');	
		        $tagDefIdUpd = $updStmt->execute();
	        }else{
	        	 $insertStmt = db_insert('slt_tag_entity');
      			 $custom  = NULL;
    	
			      $fields = array(
			                'entity_type'        => $entityType,                  
			                'entity_id'          => $entityId,
			                'tagid'              => $tagIdUpdate,
			                'created_by'         => $currentUserId,
			                'created_on'         => now(),
			                'updated_by'         => $currentUserId,
			                'updated_on'         => now(),
			                'custom0'            => $emptyId,
			                'custom1'            => $custom,
			                'custom2'            => $custom,
			                'custom3'            => $custom,
			                'custom4'            => $custom   
			              );
      			$insertStmt->fields($fields);		
       			expDebug::dPrintDBAPI("Insert sql detailssss >>>>>>>>>. " , $insertStmt,5);
      			$tagId = $insertStmt->execute();
	        }
                if($_REQUEST['apiname']=='UpdateTagByAPI'){
                    $newTag = db_query('select tagname from slt_tagdefn where id = '. $tagIdUpdate)->fetch()->tagname;
                   // expDebug::dPrintDBAPI("tags_update sql >>>>>>>>>." , $newTag);
                }
	        $attFormState = array();
	      	$attFormState['id'] = $tagIdUpdate;
	      	$attFormState['tagname'] =  $newTag;
	      	make_audit_trail_entries($attFormState, $currentUserId , null ,$entityId, 'Inserted',$entityType,'exp_sp_administration_tag','mapped tag',
	                                          array('id', 'tagname'));
		if($_REQUEST['apiname']=='UpdateTagByAPI'){
                        $res=new stdClass();
                        $res->id=$tagIdUpdate;
                        $res->tagname=$newTag;
                return $res;
            }				
			
		}
		syncSolrData('Program');
		syncSolrData('Course');
		syncSolrData('Class');
	}
	catch (Exception $ex) {
	    $txn->rollback();
	    watchdog_exception('tags_update', $ex);
	    throw $ex;    
  	}
  
  unset($txn);
}

/**
 * _updateTagsEntity()
 * @param Entity Id
 * @param Entity Type
 * @param Empty ID 
 */
function _updateTagsEntity($entityId, $entityType, $emptyId,$trigger=''){
  try {
  $fields = array();
  $fields['custom0'] = NULL;
  $fields['entity_type'] = $entityType;
  
  // Update catalog access entry 
  $catalogUpdate = db_update('slt_tagdefn');
  if($trigger == 'saveandpublish'){
  	$catalogUpdate->expression('activity_count', 'activity_count + 1');
  }
  $catalogUpdate->fields($fields);
  $catalogUpdate->condition('custom0', $emptyId);
  $catalogUpdate->execute();
  
  $fields = array();
  $fields['custom0'] = NULL;
  $fields['entity_id'] = $entityId;
  $fields['entity_type'] = $entityType;
  // Update MRO entry
  $mroUpdate = db_update('slt_tag_entity');
  $mroUpdate->fields($fields);
  $mroUpdate->condition('custom0', $emptyId);
  expDebug::dPrint(' $$$mroUpdate = ' . $mroUpdate);
  $mroUpdate->execute();
  } catch (Exception $ex) {
  	watchdog_exception('_updateTagsEntity', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addHiddenFieldToTagsFieldSet - 
 */
function addHiddenFieldToTagsFieldSet(&$form, $id, $fieldsetName, $defaultValue = '') {
	try {
		 $form[$fieldsetName.'_fieldset'][$fieldsetName]['hid_'.$fieldsetName.'_id_'.$id] = array (
		   '#type' => 'hidden',
		   '#default_value' => $defaultValue,
		   '#attributes' => array('id'=>'hid_'.$fieldsetName.'_id_'.$id)
		 );
 } catch (Exception $ex) {
 	watchdog_exception('addHiddenFieldToTagsFieldSet', $ex);
 	expertusErrorThrow($ex);
 }
}

/*
 * addEditTagsRepaintForm() - After the form has been validated and saved, this function returns the form
 *                            to the client for re-rendering.
 */
function addEditTagsRepaintForm($form, &$form_state) {
	try{
  expDebug::dPrint(' addEditRepaintForm() called.', 5);
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint('  $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
  expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);
  
  // If a new entity was added, build additional forms
  if (!empty($form_state['storage']['new_entity']) && $form_state['storage']['new_entity'] == TRUE) {    
    // Create the additional now editable forms if any present
    $formInfoList = $form_state['storage']['form_info_list'];
    $argsList = $form_state['build_info']['args'];
    array_shift($argsList); // The first arg is 'ajax', which we need to drop from $argsList
    
    $renderedAddlForms = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, 'ADD-TO-EDIT');
  }
  
  $commands = array();
  
  // Build HTML for the custom fields block if triggering element is Add custom or Delete custom button
  // We paint only the custom fields block in this case, and not the full form.
  if (!empty($form_state['triggering_element']['#name']) &&
               ($form_state['triggering_element']['#name'] == 'Add custom' || substr($form_state['triggering_element']['#name'], 0, 13) == 'Delete custom')) {
    $customFieldsWrapperId = $form_state['triggering_element']['#ajax']['custom_fields_wrapper_id'];
    expDebug::dPrint(' $customFieldsWrapperId = ' . print_r($customFieldsWrapperId, true) , 5);

    $customFieldsFormElements = array();
    //$customFieldsFormElementsKeysForDebugging = array();
    $startCapture = false;
    foreach ($form as $elemKey => $elemAttrib) {
      expDebug::dPrint(' $elemKey = ' . print_r($elemKey, true) , 4);
      expDebug::dPrint(' $elemAttrib = ' . print_r($elemAttrib, true), 4);
    
      if (!$startCapture && $elemKey != 'customfields_wrapper_start') {
        continue;
      }
    
      $startCapture = true;
      $customFieldsFormElements[] = $form[$elemKey];
      //$customFieldsFormElementsKeysForDebugging[] = $elemKey;
    
      if ($elemKey == 'customfields_wrapper_end') {
        break; // break out of form; we have got all the fields that we want to repaint
      }
    }
    expDebug::dPrint(' $customFieldsFormElementsKeysForDebugging = ' . print_r($customFieldsFormElementsKeysForDebugging, true) , 4);
  
    $renderHTML = drupal_render($customFieldsFormElements);
    expDebug::dPrint(' $renderHTML = ' . print_r($renderHTML, true)  , 4);
    $commands[] = ajax_command_replace('#' . $customFieldsWrapperId, $renderHTML);
  }
  else { // repaint the full form
  	$elementNo = $form_state['triggering_element']['#ajax']['element_no'];
  	expDebug::dPrint('check element no= '. print_r($elementNo, true), 4);
  	expDebug::dPrint(' Before class attribute = ' . print_r($form['tag_fieldset']['tag']['tag_name_'.$elementNo]['#attributes']['class'][4], true) , 4);
  	if(!empty($form_state['values']['tag_name_'.$elementNo]) && $form_state['values']['tag_name_'.$elementNo]!= t('LBL578') && trim($form_state['values']['tag_name_'.$elementNo]) != '') {
  	  unset($form['tag_fieldset']['tag']['tag_name_'.$elementNo]['#attributes']['class'][4]); 
  	}
    $commands[] = ajax_command_replace(NULL, drupal_render($form));
  }
  
  // Render additional forms, if moving from add to edit
  if (!empty($renderedAddlForms)) {
    $commands[] = ajax_command_after(NULL, $renderedAddlForms);
  }

  // Show the status messages, if any.
  // Ensure the status messages are auto removed after a brief while
  $statusMsgHTML = theme('status_messages');
  expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
  if (!empty($statusMsgHTML)) {
  	// Show the messages
    //$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
    $commands[] = ajax_command_html('#bubble-face-table #paintContentVisiblePopup  #show_expertus_message', $statusMsgHTML);
  }
  
  // Resize the modal backdrop, adjust the model top (and left) to optimal
  $commands[] = exp_ctools_modal_adjust();

  // If a new entity was added or an existing entity was edited, inform ctools to refresh narrow search results no modal close.
  if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
    $commands[] = ajax_command_refresh_narrow_search_results('onclose');
  }
  if (module_exists('exp_sp_esignature') && isActiveEsignRegion() && (!$form_state['esign_validation_status'])){
    $custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];   
    $drupalUserId  = $form_state['triggering_element']['#ajax']['drupalUserId']; 
    $drupalUserId  = $form_state['values'][$drupalUserId];
    $esignObj  = '{"popupDiv":"modalContent","esignFor":"AddAdmin","drupalUserId":"'.$drupalUserId.'","esignButtId":"'.$custButtonId.'"}';
    if($form_state['esign_show_popup'] == true){
      $commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
      $form_state['esign_show_popup']=false; // this is ignored, as form state is not saved after the ajax callback is executed
    }
  }
  //calling scroll function in tags
  $var = $form_state['triggering_element']['#ajax']['wrapper']; 
  if(strpos($var,'tag-fieldset-wrapper')!== false){
  	$commands[] = ajax_command_invoke(NULL, "scrollTag", array($var));
  }
   
  
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

  return array('#type' => 'ajax', '#commands' => $commands);
  } catch (Exception $ex) {
  	watchdog_exception('addEditTagsRepaintForm', $ex);
  	expertusErrorThrow($ex);
  }
}
function deleteTagByAPI()
{
    $entity_id = trim($_REQUEST['entity_id']);
    $entity_type = trim($_REQUEST['entity_type']);
    $tagname=trim($_REQUEST['tag_name']);
    if(!is_numeric($entity_id) || $entity_id <= 0)
    {
        return APIerror(1,'L_013','entity_id accepts only numeric values which are greater than \'0\'');
    }
    if($entity_type!='Class' && $entity_type!='Course' && $entity_type!='Training Plan' && $entity_type!='Assessment Question' && $entity_type!='Survey Question' && $entity_type!='Content')
    {
        return APIerror(1,'L_013','entity_type can contain only these values - "Class", "Course", "Training Plan", "Assessment Question", "Survey Question" or "Content"');
    }
    if(!entityIsExists($entity_id,$entity_type)){
        return APIerror(1,'L_013','Invalid Entity ID');
    }
    expDebug::dPrint('$form_state '. print_r($form_state, true), 4);
    if($entity_type=='Survey Question')
        $entity_type='sry_det_typ_sry_qus';
    if($entity_type=='Assessment Question')
        $entity_type='sry_det_typ_ass_qus';
    if($entity_type=='Training Plan')
    {
        $select= db_select('slt_program','prg');
        $select->innerJoin('slt_profile_list_items','plist','plist.code=prg.object_type');
        $select->addField('plist', 'name', 'entity_type');
        $select->condition('prg.id', $entity_id);
        $type= $select->execute()->fetch();
        $entity_type=$type->entity_type;
    }
    $select1 = db_select('slt_tagdefn','tagdef');
    $select1->addField('tagdef','id', 'tag_id');
    $select1->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid');
    $select1->condition('tagdefent.entity_id', $entity_id,'=');
    $select1->condition('tagdefent.entity_type', $entity_type,'=');
    $select1->condition('tagdef.entity_type', $entity_type,'=');
    $select1->condition('tagdef.tagname', $tagname,'=');
    expDebug::dPrintDBAPI("tag def --->>> " ,$select1);
    $emptyId= 0;
    $tag_id = $select1->execute()->fetch()->tag_id;
    if(!empty($tag_id)) {
     $form_state=array(
        'entity_id'=>$entity_id,
        'entity_type'=>$entity_type,
        'tag_id'=>$tag_id
            );
    expDebug::dPrint('$form_state '. print_r($form_state, true), 4);
    $res=deleteNewTagDetails('',$form_state);
    expDebug::dPrint('$res '. print_r($res, true), 4);
    return $res;
    }
    else
    {
        if($entity_type == 'Class')
        {
            $select2 = db_select('slt_tagdefn','tagdef');
            $select2->addField('tagdef','id', 'tag_id');
            $select2->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid and tagdefent.entity_type = \'Course\'');
            $select2->leftjoin('slt_course_class', 'cls','tagdefent.entity_id = cls.course_id and cls.id='.$entity_id);
            $select2->condition('tagdef.entity_type', 'Course','=');
            $select2->condition('tagdef.tagname', $tagname,'=');
            expDebug::dPrintDBAPI("tag def --->>> " ,$select2);
            $tag_id = $select2->execute()->fetch()->tag_id;
            if(!empty($tag_id)) {
                return APIerror(1,'L_013','Entered tag is not attached to Class, it is attached to course. It can only be deleted from course.'); 
            }
        }
        return APIerror(1,'L_013','Invalid Tag'); 
    }
}
function updateTagByAPI() {
try{
    $entity_id = trim($_REQUEST['entity_id']);
    $entity_type = trim($_REQUEST['entity_type']);
    $oldtagname=trim($_REQUEST['old_tag_name']);
    $newtagname=trim($_REQUEST['new_tag_name']);
    if(!is_numeric($entity_id) || $entity_id <= 0)
    {
        return APIerror(1,'L_013','entity_id accepts only numeric values which are greater than \'0\'');
    }
    if($entity_type!='Class' && $entity_type!='Course' && $entity_type!='Training Plan' && $entity_type!='Assessment Question' && $entity_type!='Survey Question' && $entity_type!='Content')
    {
        return APIerror(1,'L_013','entity_type can contain only these values - "Class", "Course", "Training Plan", "Assessment Question", "Survey Question" or "Content"');
    }
    if(strlen($newtagname) >150){
        return APIerror(1,'L_013','Tagname length must be lesser than 150');
    }
    if(!entityIsExists($entity_id,$entity_type)){
        return APIerror(1,'L_013','Invalid Entity ID');
    }
    if($entity_type=='Survey Question')
        $entity_type='sry_det_typ_sry_qus';
    if($entity_type=='Assessment Question')
        $entity_type='sry_det_typ_ass_qus';
    if($entity_type=='Training Plan')
    {
        $select= db_select('slt_program','prg');
        $select->innerJoin('slt_profile_list_items','plist','plist.code=prg.object_type');
        $select->addField('plist', 'name', 'entity_type');
        $select->condition('prg.id', $entity_id);
        $type= $select->execute()->fetch();
        $entity_type=$type->entity_type;
    }
    $select1 = db_select('slt_tagdefn','tagdef');
    $select1->addField('tagdefent','id', 'tag_id');
    $select1->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid');
    $select1->condition('tagdefent.entity_id', $entity_id,'=');
    $select1->condition('tagdefent.entity_type', $entity_type,'=');
    $select1->condition('tagdef.entity_type', $entity_type,'=');
    $select1->condition('tagdef.tagname', $oldtagname,'=');
    expDebug::dPrintDBAPI("tag def --->>> " ,$select1);
    $emptyId= 0;
    $old_tag_entityid = $select1->execute()->fetch()->tag_id;
    expDebug::dPrint('Hi raj old tag ID'.print_r($old_tag_entityid,1),4);
    if(!empty($old_tag_entityid)) {
        $res=tags_update($entity_id,$emptyId,$entity_type,$newtagname,$old_tag_entityid);
        return $res;
    }
 else {
        if($entity_type == 'Class')
        {
            $select2 = db_select('slt_tagdefn','tagdef');
            $select2->addField('tagdef','id', 'tag_id');
            $select2->leftjoin('slt_tag_entity', 'tagdefent','tagdef.id=tagdefent.tagid and tagdefent.entity_type = \'Course\'');
            $select2->leftjoin('slt_course_class', 'cls','tagdefent.entity_id = cls.course_id and cls.id='.$entity_id);
            $select2->condition('tagdef.entity_type', 'Course','=');
            $select2->condition('tagdef.tagname', $tagname,'=');
            expDebug::dPrintDBAPI("tag def --->>> " ,$select2);
            $tag_id = $select2->execute()->fetch()->tag_id;
            if(!empty($tag_id)) {
                return APIerror(1,'L_013','Entered tag is not attached to Class, it is attached to course. It can only be edited from course.'); 
            }
        }
        return APIerror(1,'L_013','Invalid Tag'); 
    }
    
    expDebug::dPrint('Hi $res old tag ID'.print_r($res,1),4);

} catch (Exception $ex) {
watchdog_exception('updateTagByAPI', $ex);
  	expertusErrorThrow($ex);
}
}
function addCourseTagsDetailsByAPI()
{
    try{        
    $entity_id = trim($_REQUEST['entity_id']);
    $entity_type = trim($_REQUEST['entity_type']);
    $tagname=trim($_REQUEST['tag_name']);
    if(!is_numeric($entity_id) || $entity_id <= 0)
    {
        return APIerror(1,'L_013','entity_id accepts only numeric values which are greater than \'0\'');
    }
    if($entity_type!='Class' && $entity_type!='Course' && $entity_type!='Training Plan' && $entity_type!='Assessment Question' && $entity_type!='Survey Question' && $entity_type!='Content')
    {
        return APIerror(1,'L_013','entity_type can contain only these values - "Class", "Course", "Training Plan", "Assessment Question", "Survey Question" or "Content"');
    }
    if(strlen($tagname) >150){ 
                return APIerror(1,'L_013','Tagname length must be lesser than 150');
    }
    if(!entityIsExists($entity_id,$entity_type)){
        return APIerror(1,'L_013','Invalid Entity ID');
    }
    expDebug::dPrint('$form_state '. print_r($form_state, true), 4);
    if($entity_type=='Survey Question')
        $entity_type='sry_det_typ_sry_qus';
    if($entity_type=='Assessment Question')
        $entity_type='sry_det_typ_ass_qus';
    if($entity_type=='Training Plan')
    {
        $select= db_select('slt_program','prg');
        $select->innerJoin('slt_profile_list_items','plist','plist.code=prg.object_type');
        $select->addField('plist', 'name', 'entity_type');
        $select->condition('prg.id', $entity_id);
        $type= $select->execute()->fetch();
        $entity_type=$type->entity_type;
    }
    expDebug::dPrint('$entity_id '. print_r($entity_id, true), 4);
    $form_state=array(
        'entity_id'=>$entity_id,
        'entity_type'=>$entity_type,
        'tagname'=>$tagname
            );
    expDebug::dPrint('$form_state '. print_r($form_state, true), 4);
    $res=addCourseTagsDetails('',$form_state);
    expDebug::dPrint('$res '. print_r($res, true), 4);
    return $res;
        
    } catch (Exception $ex) {
        watchdog_exception('addCourseTagsDetailsByAPI', $ex);
  	expertusErrorThrow($ex);
    }
   
}
function APIerror($isValidateError,$errcode,$errormsg)
{
    try{
     $errobj=new stdClass();
     $errobj->isValidateError = $isValidateError;
     $errobj->errcode = $errcode;
     $errobj->errormsg = $errormsg;
     return $errobj;        
    } catch (Exception $ex) {
        watchdog_exception('APIerror', $ex);
  	expertusErrorThrow($ex);
    }
   
}
function entityIsExists($entity_id,$entity_type)
{
    
    switch($entity_type) {
        case 'Class':
            $tableName = 'slt_course_class';
            $status='lrn_cls_sts_del';
            break;
        case 'Course':
            $tableName = 'slt_course_template';
            $status='lrn_crs_sts_del';
            break;
        case 'Training Plan':
            $tableName = 'slt_program';
            $status='lrn_lpn_sts_del';
            break;
        case 'Content':
            $tableName = 'slt_content_master';
            $status='lrn_cnt_sts_del';
            break;
        case 'Survey Question':
            $tableName = 'slt_survey_questions';
            $status='sry_qtn_sts_del';
            $type= 'sry_det_typ_sry';
            break;
        case 'Assessment Question':
            $tableName = 'slt_survey_questions';
            $status='sry_qtn_sts_del';
            $type= 'sry_det_typ_ass';
            break;
        }
        $select= db_select($tableName,'test');
        $select->addField('test', 'id', 'entityid');
        $select->condition('test.id', $entity_id);
        $select->condition('test.status', $status,'!=');
        if($entity_type == 'Survey Question' || $entity_type == 'Assessment Question')
            $select->condition('test.survey_type',$type,'=');
        $res= $select->execute()->fetch();
        $entity_id=$res->entityid;
        expDebug::dPrintDBAPI("Test Raj sql " , $select);
        if(is_numeric($entity_id) && $entity_id > 0)
        {
            return 1;
        }
    return 0;
}
?>