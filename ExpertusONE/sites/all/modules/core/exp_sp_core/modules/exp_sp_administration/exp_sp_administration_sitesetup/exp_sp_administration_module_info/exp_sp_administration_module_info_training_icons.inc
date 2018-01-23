<?php
function training_icons_AddEdit($js = NULL){
	try {
		
		global $theme_key, $user;
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		
    
		$ti_style = array(
				'ctools-admin-ticon-style' => array(
						'modalSize' => array(
								'width' => 413,
                                'minHeight' => 180,
						),
				)
		);


		drupal_add_js($ti_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();
				
			// Prepare list of forms to be displayed in the modal
			$formId = array(
					array('id' => 'exp_sp_administration_module_info_training_icons_form',
							'title' => t('Theme Customization')),
			);

			exp_ctools_modal_multi_ajaxonly_forms_display($formId, $argsList);
			
			
		}
		else {
			throw new Exception('training_icons_AddEdit');
		}
	}catch (Exception $ex) {
		watchdog_exception('training_icons_AddEdit', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_module_info_training_icons_form($form, &$form_state){
	try {
			
			include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
			 //$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			 //drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') .'/exp_sp_administration_module_info_training_icons.js',$js_module_optional);
			$form = newAddEditFormWithCancelAndSave('ticon-basic-addedit-form');
			$form['ticon-basic-edit-form-start'] = array(
				'#type'   => 'markup',
				'#markup' => '<div id="ticon-basic-addedit-form-div" class="ticon-edit-rows">',
			);  
			
			$list = array('Course' => 'Course','Classroom' => 'Classroom','Virtualclass'=>'Virtual Class','Video'=>'Video','Web-based'=>'Web-based','Certification'=>'Certification','Curriculam'=>'Curriculum','Learning-plan'=>'Learning Plan','Recertification'=>'Recertification');
			$is_file_uploaded='';
			foreach ($list as $key=> $value){
				
				addTwoColumnRowToAddEditForm($form, $key.'_region');
				$is_file_uploaded = variable_get($key.'_icon');
				
				$default_icon = base_path().'sites/default/files/images/defaulticon/'.$key.'.png'.'?'.time();
				$filename = 'sites/default/files/images/'.$key.'.png';
				
				$fileCheck = file_exists('sites/default/files/images/new_'.$key.'.png');
				//$fileCheckNew = file_exists('sites/default/files/images/new_'.$key.'.png');
	                   
					   
	            $defVal = !empty($form_state['values'][$key]) ? $form_state['values'][$key] : FALSE;
				//$current_image_name = variable_get($key.'_icon',''); // variable_get($key.'_icon','');
				$checkedIcon='';
				expDebug::dPrint('  form state values of yuvan= ' . print_r($form_state, true) , 4);
				if(empty($is_file_uploaded))
				{
					expDebug::dPrint('I am 1 ' . print_r($form_state, true) , 4);
					 $form_state['input'][$key]  = $checkedIcon = 'oldicon';
				}
				else if(!empty($is_file_uploaded) && ($is_file_uploaded == 'new_'.$key.'.png') && $form_state['input']['_triggering_element_name'] == 'hidden-save' && $form_state['values']['save'] != 'Save'){
					expDebug::dPrint('I am 2 ' . print_r($form_state, true) , 4);
					$form_state['input'][$key]  = $checkedIcon = 'newicon';
				}	
				else if(!empty($is_file_uploaded) && ($is_file_uploaded == 'new_'.$key.'.png')){
					expDebug::dPrint('I am 3 ' . print_r($form_state, true) , 4);
					$form_state['input'][$key]  = $checkedIcon = 'oldicon';
				}										
				else if(isset($form_state['input']['_triggering_element_name']) && $form_state['input']['_triggering_element_name'] == 'image_del'.$key)		
				{
					expDebug::dPrint('I am 5 ' . print_r($form_state, true) , 4);
					$form_state['input'][$key]  = $checkedIcon = 'oldicon';
				}
				else if(!empty($is_file_uploaded) && ($is_file_uploaded == $key.'.png')){
					expDebug::dPrint('I am '.$key.'=='.$is_file_uploaded.'== 4 ' . print_r($form_state, true) , 4);
					$form_state['input'][$key]  = $checkedIcon = 'newicon';
				}			
				

	// expDebug::dPrint('  form state values= ' . print_r($form_state, true) , 4);
	

	            
	            if(file_exists('sites/default/files/images/new_'.$key.'.png') || file_exists('sites/default/files/images/old_'.$key.'.png')) {
						
					$uploaded_icon = base_path().'sites/default/files/images/'.$is_file_uploaded.'?'.time();
					addRadioSetToAddEditForm($form, $key, $value, array('oldicon'=>'<img src = '.$default_icon.'>','newicon'=>'<img src = '.$uploaded_icon.' >'),$checkedIcon,'1');
					$attributes['class'] = array('delivery_icon_del');
								
					$form[$key.'_del'] = array(
		  				'#type' => 'image_button', '#src' => base_path().'sites/all/themes/core/expertusone/expertusone-internals/images/close.png',
		  				'#name' => 'image_del'.$key, // Based on this name, delete elemment updates the proper files.
		  				'#submit' => array('exp_sp_administration_module_info_training_icons_delete_form_submit'),
		  				'#attributes' => $attributes,
		  				'#ajax' => array(	
	                     	'callback' => 'addEditRepainttiForm',
		  					'wrapper' => 'ticon-basic-addedit-form',
		  					'banner_id' => $key,
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
				else{
					addRadioSetToAddEditForm($form, $key, $value, array('oldicon'=>'<img src = '.$default_icon.' id="ti-old-icons" >','newicon'=>'New Icon'),$checkedIcon,'1');
					imageFileUploadControlToAddEdittiForm($form, $key.'_image_upload_file', '','', 5,false,false,7); //LBL619
					
					 $form[$key.'-training-icons-text'] = array(
	                 	'#type'    => 'markup',
	                    '#markup'  => '<div class="helptext '.$key.'-training-icons-text">'.t('(50 X 50 px png file)').'</div>'  
	                 );		
										
					$form[$key.'_image_upload_file']['#attributes']['onchange']="$('#hidden-save').click(); $('.form-item-files--".$key."-image-upload-file').addClass(' selected');";
					
					
				}		
				
				endAddEditTwoColumnRow($form);
			}
			tiAddEditFormWithCancelAndSaveClose($form, 'ticon-basic-addedit-form', 'ticon-save-button', 'ticon-cancel-button', 7);
			
			
			$form['ticon-basic-edit-form-end'] = array(
				'#type'   => 'markup',
				'#markup' => '</div>',
			);	
				
		return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_module_info_training_icons_form', $ex);
		expertusErrorThrow($ex);
	}
	
}
function exp_sp_administration_module_info_training_icons_form_validate($form, &$form_state){
    expDebug::dPrint(' zipped success = ' . print_r($form_state['values']['save'], true) , 4);
    try{
    
	
	   	$imageName = array('Course' => 'Course','Classroom' => 'Classroom','Virtualclass'=>'Virtualclass','Video'=>'Video','Web-based'=>'Web-based','Certification'=>'Certification','Curriculam'=>'Curriculam','Learning-plan'=>'Learning-plan','Recertification'=>'Recertification');
     	foreach ($imageName as $key=> $value)	{
	     	$get_upload_loc = $key;
	     	$is_file_uploaded = variable_get($key.'_icon');
			 
			 	   if ($form_state['values'][$value] == 'newicon' && $form_state['values']['save'] == 'Save' && (empty($_FILES['files']['name'][$get_upload_loc.'_image_upload_file'])) &&(empty($is_file_uploaded)) )
		{
			
		form_set_error('announcement_banner', t('Please upload an image'));
          $form_state["storage"]["form_validate"] = false; // not used
          $form_state['esign_show_popup']= false;
			
			
		}
			 
			  $filename = $_FILES['files']['name'][$get_upload_loc.'_image_upload_file'];
     if (!empty($_FILES['files']['name'][$get_upload_loc.'_image_upload_file'])) {
        $filename = strtolower($filename) ;
        $exts = split("[/\\.]", $filename) ;
        $n = count($exts)-1;
        $exts = $exts[$n];
        expDebug::dPrint('Testting extension' . $exts);
        if( $exts!='png'){
          form_set_error('announcement_banner', t('Upload 50 X 50 pixels png files only'));
          $form_state["storage"]["form_validate"] = false; // not used
          $form_state['esign_show_popup']= false;

	     	
		     	
	    	}
		if( $exts =='png'){
		$size = getimagesize($_FILES['files']['tmp_name'][$get_upload_loc.'_image_upload_file']);
		     	$image_width = $size[0];				
		     	$image_height = $size[1];
				
		     	if($image_width != '50' || $image_height != '50') {
		     		form_set_error($get_upload_loc.'_image_upload_file', $form[$get_upload_loc.'_image_upload_file']['#title'] . t('Upload 50 X 50 pixels png files only'));
		     		$form_state["storage"]["form_validate"] = false; // not used
		     		$form_state['esign_show_popup']= false;
		     	}
	 	}}
	 	}
}	
		
		//}
    
    catch (Exception $ex) {
    	watchdog_exception('exp_sp_administration_module_info_training_icons_form', $ex);
		expertusErrorThrow($ex);
    }
}


function tiAddEditFormWithCancelAndSaveClose(&$form, $wrapperId, $saveButtonId, $cancelButtonId, $tabIndex) {
	try {
			$esignHiddenButt = 'esign-'.$wrapperId;
			global $theme_key;
			$prefixBgLeft='';$suffixBgRight='';
			if($theme_key == 'expertusoneV2')	{
				$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
				$suffixBgRight = '<div class="white-btn-bg-right"></div>';
			}
			// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
			// each time the form is re-built.
			$cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
	
			$cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
			$cancelAttributes['tabindex'][] = $tabIndex + 1;
	
			// A common class for all addedit form cancel button, for styling to appear as a link.
			//$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
			$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss(); return false;';
	
			$form[$cancelButtonId] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
				'<div class="addedit-form-cancel-container-actions">'.$prefixBgLeft,
				'#suffix' => $suffixBgRight,
				// Suffix to close the two divs is added in #suffix property of Save button
				'#value' => t('LBL123'), //Close
				'#attributes' => $cancelAttributes,
			);
	
			// Add the Save button
			$ajaxCallbackSave = 'addEditRepainttiForm';
			//$ajaxCallbackHiddenSave = 'addEditRepainttiForm';
	
			// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
			// each time the form is re-built.
			$saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);
	
			$saveAttributes['class'][] = 'admin-save-button-middle-bg';
			$saveAttributes['tabindex'][] = $tabIndex;
			// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
			$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
			// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
			$saveAttributes['data-wrapperid'] = array($wrapperId);
			//$saveAttributes['onclick'][] = '$("#' . $esignHiddenButt . '").click(); return false;';
			$eSignatureAvailable = true;
	
			$form[$saveButtonId] = array(
				'#type' => 'submit',
				'#value' => t('LBL141'), // Save
				'#name' => 'save',
				'#prefix' => '<div class="admin-save-button-container" >' .
				'<div class="admin-save-button-left-bg"></div>',
				'#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
				//'</div>' .
				'<div class="clearBoth"></div>' .
				'</div>' . // see #prefix of close button above
				'<div class="clearBoth"></div>',
				//'</div>', // see #prefix of close button above
				'#id' => $saveButtonId,					
				'#submit' => array('exp_sp_administration_module_info_training_icons_rename_form_submit'),		
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
			
			$form['hidden-save'] = array(
				'#type' => 'submit',
				'#value' => t('LBL141'), // Save
				'#name' => 'hidden-save',
				'#prefix' => '<div  class="admin-save-button-container" style= "display:none">' .
				'<div class="admin-save-button-left-bg"></div>',
				'#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
				'</div>' .
				'<div class="clearBoth"></div>' .
				'</div>' . // see #prefix of close button above
				'<div class="clearBoth"></div>' .
				'</div>', // see #prefix of close button above
				'#id' => 'hidden-save',
				'#submit' => array('exp_sp_administration_module_info_training_icons_form_submit'),
	            '#ajax' => array(
					'callback' => $ajaxCallbackSave,
					'wrapper' => 'ticon-basic-addedit-form',
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
		watchdog_exception('tiAddEditFormWithCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

function upload_file($form, $form_state){
	
	include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
	$sitepath      ="sites/default/files/images/";
expDebug::dPrint(' zipped success = ' . print_r($form_state, true) , 4);
	$get_upload_file = $_FILES['files']['name'];
	expDebug::dPrint(' $get_upload_file = ' . print_r($get_upload_file, true) , 4);
	if(count($get_upload_file)>0){
		foreach($get_upload_file as $key => $values){
			$get_upload_loc = strstr($key, '_', true);                      
					  expDebug::dPrint('$get_upload_loc = ' . print_r($get_upload_loc, true) , 4);
		   /*$validators  = array(
		     	'file_validate_extensions' => array('png'), // Validate extensions.
		     );*/
			  
			  
			$imageUpload = file_save_upload($get_upload_loc.'_image_upload_file', $validators,$destination = false);
   			
   						  expDebug::dPrint('$imageUpload = ' . print_r($imageUpload, true) , 4);
	
			if(!empty($imageUpload)) 	{
            	if (!$imageUpload->status) { // i.e. if picture is a temporary file
      				// Prepare the pictures directory.
      				$filedirectory = file_default_scheme() . '://images';
					expDebug::dPrint(' zipped success of yuvanesh dieeeee = ' . print_r($filedirectory, true) , 4);
					
      				file_prepare_directory($filedirectory, FILE_CREATE_DIRECTORY);

				    // Prepare the unique file name to place the picture under the $filedirectory.
				    $destination = file_stream_wrapper_uri_normalize($filedirectory . '/'  .$imageUpload->filename);
					expDebug::dPrint(' zipped success of yuvanesh dieeeee = ' . print_r($destination, true) , 4);
					if ($imageMove = file_move($imageUpload, $destination, FILE_EXISTS_REPLACE)) {
			
						$save_image = file_save($imageMove);
						$form_state['values']['iconame'] = $get_upload_loc.'_icon';
						$newImageName =$sitepath.$save_image->filename;
						rename($newImageName,$sitepath.'new_'.$get_upload_loc.'.png');
						variable_set($get_upload_loc.'_icon','new_'.$get_upload_loc.'.png');
				        $get_name = variable_get($values);
						chmod($newImageName, 0777);
						//expertus_set_message(t('Uploaded Successfully'));
						expDebug::dPrint(' zipped successfully = ' . print_r($form_state, true) , 4);
						make_audit_trail_entries('', getIdOfLoggedInUser() , null ,$get_name, 'uploaded the image '.$get_name,'cme_mod',
		  											'exp_sp_administration_module_info','upload_file ',array('id'));		
                        //$form_state['storage']['image_set']=$get_upload_loc;
                        
												
						
				    }
				}
			}
				
		}
	}

	
}

function addEditRepainttiForm($form, &$form_state){   
                        $builtForm  = drupal_build_form('exp_sp_administration_module_info_training_icons_form', $form_state);
						$commands[] = ajax_command_replace(NULL, drupal_render($builtForm));
                        $statusMsgHTML = theme('status_messages');
			
						if (!empty($statusMsgHTML)) {
						$commands[] = ajax_command_html('#modalContent #show_expertus_message', $statusMsgHTML);
			}
                   		return array('#type' => 'ajax', '#commands' => $commands);
}


function exp_sp_administration_module_info_training_icons_form_submit($form, &$form_state){
	try {
		$uploadFile = upload_file($form, $form_state);
	    if($uploadFile){	    	
	       	expertus_set_message(t('Uploaded Successfully'));
	       	$form_state['rebuild'] = TRUE;
	    }else{
	        expertus_set_message(t('Uploaded Successfully'));
	        $form_state['rebuild'] = TRUE;
	    }
			//$form['Course']['#attributes']['class'][]= 'unchecked'; 
			
	}catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_module_info_training_icons_submit', $ex);
			expertusErrorThrow($ex);
	}
}


function exp_sp_administration_module_info_training_icons_rename_form_submit($form, &$form_state)
{
try {
		$uploadFile = renameImageName($form, $form_state);
		
	    if($uploadFile){	    	
	       	expertus_set_message(t('Saved Successfully'));
	       	$form_state['rebuild'] = TRUE;
		
	    }
		
	}catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_module_info_training_icons_rename_form_submit', $ex);
			expertusErrorThrow($ex);
	}
}	
	
	
function exp_sp_administration_module_info_training_icons_delete_form_submit($form, &$form_state){
	try {
		$deleteFile = delete_image($form, $form_state);
	    if($deleteFile){	    	
			expertus_set_message(t('Deleted Successfully'));
			$form_state['rebuild'] = TRUE;
	    }else{
	        expertus_set_message(t('Deleted Unsuccessfully'));
	        $form_state['rebuild'] = TRUE;
	    }
			
	}catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_module_info_training_icons_submit', $ex);
			expertusErrorThrow($ex);
	}
}


function delete_image ($form, &$form_state)
{
	$imageId = $form_state['triggering_element']['#ajax']['banner_id'];
	$sitepath      ="sites/default/files/images";
	variable_get($bannerId.'_icon');
	$image_name =variable_get($imageId.'_icon');
	$getImage_path =  $sitepath . '/' . $image_name ; 
	chmod( $getImage_path, 0777);		
	unlink ($getImage_path);		
	variable_del($imageId.'_icon');
	
	make_audit_trail_entries($image_name, getIdOfLoggedInUser() , null ,$image_name, 'deleted the image '.$image_name,'cme_mod','exp_sp_administration_module_info','delete_image',array('id'));		
    $form_state['storage']['image_set']=$get_upload_loc;
	
	if(file_exists('sites/default/files/images/old_'.$imageId.'.png'))
	{
		rename('sites/default/files/images/old_'.$imageId.'.png','sites/default/files/images/'.$imageId.'.png');
		$list = array('Course','Classroom','Virtualclass','Video','Web-based','Certification','Curriculam','Learning-plan','Recertification');
		foreach($list as $values){
			$zip = new ZipArchive;
			if ($zip->open(getcwd().'/sites/default/files/images/mobile.zip') === TRUE) {
				$zip->addFile(getcwd().'/sites/default/files/images/'.$values.'.png', $values.'.png');
			    $zip->close();
			} 
	 	}
	}
	include_once "./sites/all/services/GlobalUtil.php";
	$oConfig  =  new GlobalUtil();
	$aConfig = $oConfig->getConfig();
	$memServer = $aConfig['memcache_server1'];
	$memSrvDt = explode(':',$memServer);
	$memcache = new Memcache;
	$connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
	$memcache->flush();
	$memcache->close();
	return true;
}

function imageFileUploadControlToAddEdittiForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99999, $viewWidthInChars = 61) {
	try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add class to apply common style to all file upload fields
  $attributes['class'][] = 'addedit-edit-file-upload';

  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

 $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
  	
    $prefix = addEditGetPrefix('DATEFIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('DATEFIELD');
  }
  else {
  	
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
    	$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
        $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_DATEFIELD', $title, $mandatory);
        $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_DATEFIELD');
        $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_administration.inc : addDateFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }


  $form[$id] = array(
    '#type' => 'file',
   // '#prefix' => addEditGetPrefix('FIELD', $title, $mandatory),
    //'#suffix' => addEditGetSuffix('FIELD'),
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars
  );
  } catch (Exception $ex) {
  	watchdog_exception('imageFileUploadControlToAddEdittiForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function renameImageName($form, $form_state)
{
	include_once $_SERVER["DOCUMENT_ROOT"]."/includes/file.inc";
	expDebug::dPrint(' form state check for old icon = ' . print_r($form_state, true) , 4);
	$sitepath      ="sites/default/files/images/";
	
	$list = array('Course','Classroom','Virtualclass','Video','Web-based','Certification','Curriculam','Learning-plan','Recertification');
	
	foreach($list as $values){
				
		if ($form_state['values'][$values] == 'newicon') {		
			
			$filename = 'sites/default/files/images/'.'new_'.$values.'.png';
			$fileCheck = file_exists($filename);
			if ($fileCheck)	{
				$get_name = variable_get($values.'_icon');//new image name
				
				$swap_image = $sitepath.'new_'.$values.'.png';// path with ranmed image
				
				rename ($sitepath.$values.'.png',$sitepath.'old_'.$values.'.png');
				rename ($swap_image,$sitepath.$values.'.png');
				variable_set(	$values.'_icon',$values.'.png');
				$fields=$get_name;
				make_audit_trail_entries($fields, getIdOfLoggedInUser() , null ,$get_name, 'selected the image '.$get_name,'cme_mod',
		  											'exp_sp_administration_module_info','upload_file ',array('id'));			
			}
		}
		else if ($form_state['values'][$values] == 'oldicon') {
			expDebug::dPrint(' form state check for old icon = ' . print_r($form_state, true) , 4);
			$filename = 'sites/default/files/images/'.'old_'.$values.'.png';
			if (file_exists($filename))	{
				$get_name = variable_get($values.'_icon');//new image name
				$swap_image = $sitepath.'old_'.$values.'.png' ;// path with ranmed image
				rename ($sitepath.$values.'.png',$sitepath.'new_'.$values.'.png');
				rename ($swap_image,$sitepath.$values.'.png');
				variable_set($values.'_icon','new_'.$values.'.png');
				expDebug::dPrint(' form state double dhamaka = ' . print_r($form_state, true) , 4);
				$fields=$get_name;
				make_audit_trail_entries('', getIdOfLoggedInUser(), null ,$get_name, 'selected the image '.$get_name,'cme_mod',
		  											'exp_sp_administration_module_info','upload_file ',array('id'));		
			}
		}

		$zip = new ZipArchive;
		if ($zip->open(getcwd().'/sites/default/files/images/mobile.zip') === TRUE) {
		    $zip->addFile(getcwd().'/sites/default/files/images/'.$values.'.png', $values.'.png');
		    $zip->close();
		    expDebug::dPrint(' zipped success = ' . print_r($form_state, true) , 4);
		} 
	}
	return true;
	
}


?>
