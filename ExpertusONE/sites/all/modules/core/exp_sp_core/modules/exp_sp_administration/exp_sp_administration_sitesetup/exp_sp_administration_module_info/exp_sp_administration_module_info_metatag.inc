<?php
function exp_sp_administration_metatag_addedit($js = NULL) {

	try {
		/* $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		 drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info_metatag.js',
		 		$js_module_optional); */


		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		
		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$metatag_style = array(
				'ctools-admin-metatag-style' => array(
						'modalSize' => array(
								'width' => 717,
								'minHeight' => 290,
						),
				)
		);

		drupal_add_js($metatag_style, 'setting');

		if ($js) {
			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_metatag_form',
							'title' => (t('LBL1317'))), //Network Access
			);
			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
		}else {
			throw new Exception('exp_sp_administration_module_info_matatag.inc: exp_sp_administration_metatag_display_form() : nojs');
		}
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_metatag_addedit', $ex);
		expertusErrorThrow($ex);
	}
}


function get_meta_values_from_db($from_page=""){
	try{	
			$select=db_select('custom_meta','cm');
			$select->addField('cm','meta_attr_value');
			$select->addField('cm','meta_content');
			$select->addField('cm','page');
			if($from_page!=""){
				$select->condition('cm.page',$from_page, '=');
			} 
			expDebug::dPrintDBAPI('$select Query = ' ,$select,5);
			
			$result = $select->execute()->fetchAll();
			expDebug::dPrint('$Metatag result selectStmt SQL = ' .print_r($result,true),5);
			
			return $result; 
			
		}
		catch (Exception $ex) {
			watchdog_exception('get_meta_values_from_db', $ex);
			expertusErrorThrow($ex);
		} 
}


function addTextFieldToAddEditFormMeta(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 2050, $viewWidthInChars = 60, $description = false,$attrDet = array()) {
	try{
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration.js', $js_module_optional);
		
		// Prepare the additional HTML attributes for the textfield
		$attributes = array();
		$attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

		// Add class to apply common style to all text fields.
		$attributes['class'][] = 'addedit-edit-textfield';

		if ($readOnly) {
			$attributes['class'][] = 'addedit-readonly-textfield';
		}

		if (isset($tabIndex) && $tabIndex > 0) {
			$attributes['tabindex'] = $tabIndex;
		}
		if($attrDet['class']){
			$attributes['class'][] = $attrDet['class'];
		}
		// If the view width is larger than max allowed characters, make view width equal to the max allowed characters
		if ($viewWidthInChars > $maxChars) {
			$viewWidthInChars = $maxChars;
		}
		if($defaultValue==t('MSG447')){
			$attributes['class'][] = 'narrow-search-filterset-daterange-empty';
		}
		$prefix = '';
		$suffix = '';
		if (empty($form['addedit_tmp_storage']['two_column'])) {
			if($id == 'webex_name'){
				$prefix = addEditGetPrefix('FIELD', $title, $mandatory,':',$id);
				$suffix = addEditGetSuffix('FIELD');
			}else{
				$prefix = addEditGetPrefix('FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('FIELD');
			}
		}
		else {
			if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
				$prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
			} else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
				$prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
				$suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
				$form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
			} else {
				throw new Exception('exp_sp_administration.inc : addTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
			}
		}

		//$attributes['onKeyDown'] = 'limitTextareaChars('.$id.', '.$charLimit.'); return false;';
		$attributes['onKeyUp'] = 'limitTextFieldChars("'.$id.'", '.$maxChars.'); return false;';
		
		//$charLimitLeft = $charLimit - strlen($defaultValue);
		$charLimitLeft = ($defaultValue != $defaultText) ? strlen($defaultValue) : 0 ;
		
		// Create the form element
		$form[$id] = array(
				'#type' => 'textfield',
				'#prefix' => $prefix,
				//'#suffix' => $suffix,
				'#suffix' => '<div class="admin-desc-character-count catalog-desc-count"><span id="char_count_'.$id.'">'.$charLimitLeft.'</span>'.' ' .t('LBL419').' '.$maxChars.' '.t('LBL420').'.</div>'.addEditGetSuffix('FIELD'),
				'#title' => $title,
				'#title_display' => 'invisible',
				'#default_value' => $defaultValue,
				'#disabled' => $readOnly,
				'#attributes' => $attributes,
				'#size' => $viewWidthInChars,
				'#maxlength' => $maxChars,
				'#description' => $description
		);

	} catch (Exception $ex) {
		watchdog_exception('addTextFieldToAddEditFormMeta', $ex);
		expertusErrorThrow($ex);
	}
}


//Metatag Form
function exp_sp_administration_metatag_form($form, &$form_state)
{
	try{
		expDebug::dPrint('$form_state ' .print_r($form_state, true),4);
		
		$triggered_from=trim($form_state['triggering_element']['#name']); 
		
		if($triggered_from==""){
			$triggered_from="home";
		}
		
		expDebug::dPrint('$values form_from_page in FORM = ' .print_r($form_state['values']['form_from_page'], true),4);
		
		if($triggered_from=="hidden-save" && $form_state['values']['form_from_page']!="" && $form_state['values']['form_from_page']=="catalog"){
			$triggered_from="catalog";
		}else if($triggered_from=="hidden-save" && $form_state['values']['form_from_page']!="" && $form_state['values']['form_from_page']=="home"){
			$triggered_from="home";
		}  
		
		expDebug::dPrint('$triggered_from in FORM = ' .print_r($triggered_from, true),4);
		
		//To check whether 'Catalog Learner' permission is set for Anonymous User.
		$is_anonymous_user_catalog = is_anonymous_user_catalog();
		expDebug::dPrint("is_anonymous_user_catalog function returns - ".$is_anonymous_user_catalog,5);
		
		$result=get_meta_values_from_db(); 
 
		$meta_title="";
		$meta_desc="";
		 
	if($triggered_from=="catalog"){
		
			   if( (trim($result[0]->meta_attr_value)!="" && trim($result[0]->meta_content)!="") && ($result[0]->page == "catalog") ){
				   $meta_title=$result[0]->meta_attr_value;
				   $meta_desc=$result[0]->meta_content;
			   } else {
			   	   $meta_title=$result[1]->meta_attr_value;
			       $meta_desc=$result[1]->meta_content;
			   }
				
			
				$form = array();
				
				$form = newAddEditFormWithCancelAndSave('metatag-basic-addedit-catalog-form');
				
				$form['metatag_catalog_disp'] = array(
						'#type'       => 'markup',
						'#markup'     => '<div id="meta-form-container"><div id="metatag-basic-addedit-catalog-form-disp">',
				);
				
				$form['metatag-basic-edit-form-start'] = array(
						'#type'   => 'markup',
						'#markup' => '<div id="metatag-basic-addedit"><div id="metatag-basic-addedit-catalog-form" class="metatag-edit-rows">',
				);
				
				$form['tabs-list-start'] = array(
						'#markup' => '<ul>'
				); 
				 
				$form['metatag-settings-home'] = array(
						'#type' => 'submit',
						'#value' => t('HOME'), //Home
						'#name'  => 'home',
						'#prefix' => '<li>' .
						'<span class="'.'notfirst'.'">',
						'#suffix' =>   '</span>' .
						'</li>',
						'#attributes' => array(
								'class' => array('form-submit', 'metatag-settings-home', 'addedit-form-expertusone-throbber'),
								'data-wrapperid' => 'meta-form-container'
						),
						'#ajax' => array(
								'callback' => 'addHomeCatalogRepaintForm',
								'wrapper'  => 'meta-form-container',
								'event'    => 'click'
						),
				);
				
		if(!empty($is_anonymous_user_catalog) && $is_anonymous_user_catalog == '1') {
				$form['metatag-settings-catalog'] = array(
						'#type' => 'submit',
						'#value' => t('CATALOG'), //Catalog
						'#name'  => 'catalog',
						'#prefix' => '<li>' .
						'<span class="'.'selected'.'">',
						'#suffix' =>   '</span>' .
						'</li>',
						'#attributes' => array(
								'class' => array('form-submit','metatag-settings-catalog', 'addedit-form-expertusone-throbber','selected'),
								'data-wrapperid' => 'meta-form-container'
						),
						'#ajax' => array(
								'callback' => 'addHomeCatalogRepaintForm',
								'wrapper'  => 'meta-form-container',
								'event'    => 'click'
						),
				);
		}		
				
				$form['tabs-list-end'] = array(
						'#type'       => 'markup',
						'#suffix'     => '</ul>',
				);
				
				$form['metatag-basic-edit-form-end'] = array(
						'#type'   => 'markup',
						'#markup' => '</div></div>',
				);
				
				
				$form['metatag_detail_wrapper'] = array(
						'#type'       => 'markup',
						'#prefix'     => '<div class="addedit-form-main-container">
						<div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
						<div class="metatag-basic-addedit-form-container" id="metatag-basic-addedit-form-container">
						<div id="metatag-settings-screen-catalog-wrapper">
						',
				);
					
				addHiddenFieldToAddEditForm($form, 'form_from_page', 'catalog');
				
				$form['addedit-remove-space-title-start'] = array(
						'#type'   => 'markup',
						'#markup' => '<div style="width:100%;">',
				); 
				
				addTextFieldToAddEditFormMeta($form, 'title', t('LBL083'), $meta_title, 1, true,false,70);
				
				$form['addedit-remove-space-title-end'] = array(
						'#type'   => 'markup',
						'#markup' => '</div>',
				);
				
				$form['addedit-long-description-container'] = array(
						'#type'   => 'markup',
						'#markup' => '<div class="addedit-long-description-container">',
				);
				
			
				addTextAreaFieldForShortDescription($form, 'description', t('LBL229'), $meta_desc, 4, true, false, 155);
				
								
				$form['addedit-long-description-container1'] = array(
						'#type'   => 'markup',
						'#markup' => '</div>',
				);
				
				addEditFormWithmetatagCancelAndSaveClose($form, 'metatag-basic-addedit-catalog-form', 15);
				
				
				$form['metatag_detail_disp_end'] = array(
						'#type'       => 'markup',
						'#suffix'     => '<div class="clearBoth"></div>
						<div class="round-corner-footer-left"><div class="round-corner-footer-right">
						<div class="round-corner-footer-middle"></div></div></div></div></div><div class="clearBoth"></div>',
				);
				$form['metatag_detail_wrapper_end'] = array(
						'#type'       => 'markup',
						'#suffix'     => '</div></div></div>',
				);
				
				$form['hidden'] = array(
						'#markup' => '<img style="display:none;" src="'.$base_url.'/sites/all/themes/core/expertusoneV2/logo.png" id="Img" onload="metatag_error_validate(); return false;"/>',
				
				);
				
				return $form;
		} else {
			
		if( (trim($result[0]->meta_attr_value)!="" && trim($result[0]->meta_content)!="") && ($result[0]->page == "home") ){
				$meta_title=$result[0]->meta_attr_value;
				$meta_desc=$result[0]->meta_content;
			} else {
				$meta_title=$result[1]->meta_attr_value;
				$meta_desc=$result[1]->meta_content;
			}
			 
			$form = array();
			
			$form = newAddEditFormWithCancelAndSave('metatag-basic-addedit-home-form');
			
			$form['metatag_home_disp'] = array(
					'#type'       => 'markup',
					'#markup'     => '<div id="meta-form-container"><div id="metatag-basic-addedit-home-form-disp">',
			);
			
			$form['metatag-basic-edit-form-start'] = array(
					'#type'   => 'markup',
					'#markup' => '<div id="metatag-basic-addedit"><div id="metatag-basic-addedit-home-form" class="metatag-edit-rows">',
			);
			
			$form['tabs-list-start'] = array(
					'#markup' => '<ul>'
			);
			
			$form['metatag-settings-home'] = array(
					'#type' => 'submit',
					'#value' => t('HOME'), //Home
					'#name'  => 'home',
					'#prefix' => '<li>' .
					'<span class="'.'selected'.'">',
					'#suffix' =>   '</span>' .
					'</li>',
					'#attributes' => array(
							'class' => array('form-submit', 'metatag-settings-home', 'addedit-form-expertusone-throbber','selected'),
							'data-wrapperid' => 'meta-form-container'
					),
					'#ajax' => array(
							'callback' => 'addHomeCatalogRepaintForm',
							'wrapper'  => 'meta-form-container',
							'event'    => 'click'
					),
			);
			
			$form['hidden'] = array(
					'#markup' => '<img style="display:none;" src="'.$base_url.'/sites/all/themes/core/expertusoneV2/logo.png" id="Img" onload="metatag_error_validate(); return false;"/>',
					 
			);

	if(!empty($is_anonymous_user_catalog) && $is_anonymous_user_catalog == '1') {
			$form['metatag-settings-catalog'] = array(
					'#type' => 'submit',
					'#value' => t('CATALOG'), //Catalog
					'#name'  => 'catalog',
					'#prefix' => '<li>' .
					'<span class="'.'notfirst'.'">',
					'#suffix' =>   '</span>' .
					'</li>',
					'#attributes' => array(
							'class' => array('form-submit','metatag-settings-catalog', 'addedit-form-expertusone-throbber'),
							'data-wrapperid' => 'meta-form-container'
					),
					'#ajax' => array(
							'callback' => 'addHomeCatalogRepaintForm',
							'wrapper'  => 'meta-form-container',
							'event'    => 'click'
					),
			);
	}
			
			$form['tabs-list-end'] = array(
					'#type'       => 'markup',
					'#suffix'     => '</ul>',
			);
			
			$form['metatag-basic-edit-form-end'] = array(
					'#type'   => 'markup',
					'#markup' => '</div></div>',
			);
			
			
			$form['metatag_detail_wrapper'] = array(
					'#type'       => 'markup',
					'#prefix'     => '<div class="addedit-form-main-container">
					<div class="round-corner-left"><div class="round-corner-right"><div class="round-corner-middle"></div></div></div>
					<div class="metatag-basic-addedit-form-container" id="metatag-basic-addedit-form-container">
					<div id="metatag-settings-screen-home-wrapper">
					',
			);
				
			addHiddenFieldToAddEditForm($form, 'form_from_page', 'home');
			
			$form['addedit-remove-space-title-start'] = array(
					'#type'   => 'markup',
					'#markup' => '<div style="width:100%;">',
			);
			
			addTextFieldToAddEditFormMeta($form, 'title', t('LBL083'), $meta_title, 1, true,false,70);
			
			$form['addedit-remove-space-title-end'] = array(
					'#type'   => 'markup',
					'#markup' => '</div>',
			);
			
			$form['addedit-long-description-container'] = array(
					'#type'   => 'markup',
					'#markup' => '<div class="addedit-long-description-container">',
			);
			
			addTextAreaFieldForShortDescription($form, 'description', t('LBL229'), $meta_desc, 3, true, false, 155);
			
			$form['addedit-long-description-container1'] = array(
					'#type'   => 'markup',
					'#markup' => '</div>',
			);
			
			addEditFormWithmetatagCancelAndSaveClose($form, 'metatag-basic-addedit-home-form', 15);
			
			
			$form['metatag_detail_disp_end'] = array(
					'#type'       => 'markup',
					'#suffix'     => '<div class="clearBoth"></div>
					<div class="round-corner-footer-left"><div class="round-corner-footer-right">
					<div class="round-corner-footer-middle"></div></div></div></div></div><div class="clearBoth"></div>',
			);
			$form['metatag_detail_wrapper_end'] = array(
					'#type'       => 'markup',
					'#suffix'     => '</div></div></div>',
			);
			return $form;
		} 
	 
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_metatag_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_metatag_form_validate($form, &$form_state){
	
	expDebug::dPrint(' Metatag Validate $form_state = ' . print_r($form_state['values'], true) , 5);
	try{
		
		$form_state['hidden-save-submit'] = false;
		$form_state['esign_show_popup'] = false;
	
		// Determine which button triggers validation
		if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
			$validationTrigger  = 'save';
		}
		else{
			$validationTrigger  = 'hidden-save';
		}
		
		if ($form_state['triggering_element']['#name'] == $validationTrigger) {
				$form_state['esign_show_popup']= true;
				
		if(empty($form_state['values']['title']) || trim($form_state['values']['title']) == '') {
						form_set_error('title', t('LBL083') ." " .t('ERR101'));
						$form_state['esign_show_popup']=false;
					}
					
		if(empty($form_state['values']['description']) || trim($form_state['values']['description']) == '') {
			form_set_error('description', t('LBL229') ." " .t('ERR101'));
						$form_state['esign_show_popup']=false;
					}
				}
					
		if ($form_state['triggering_element']['#name'] == 'hidden-save' || $form_state['triggering_element']['#ajax']['triggerEvent'] == 'hidden-save') {
				$form_state['hidden-save-submit'] = true;
			}
			
			$form_state['esign_validation_status'] = false; // checked to be false in exp_sp_administration.inc:addEditRepaintForm() as a precondition to
			// showing popup.
		if (!($form_state['hidden-save-submit'])) {
				unset($_SESSION['esign_validate']); // for every button other than hidden-save, unset $_SESSION['esign_validate'].
			}
		}catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_metatag_form_validate', $ex);
			expertusErrorThrow($ex);
		} 
}

function exp_sp_administration_metatag_form_submit($form, &$form_state) {
	try {
		$result=get_meta_values_from_db();
		expDebug::dPrint('$form_state values: ' .print_r($form_state['values'], true),4); 

		$triggered_from=trim($form_state['triggering_element']['#name']);		
		
		expDebug::dPrint('$triggered_from = ' .print_r($triggered_from, true),4);
		
		if($triggered_from=="catalog"){
			expDebug::dPrint('Clicked on catalog.No action',4); //Render the catalog form here. 
		}else if($triggered_from=="home") {
			expDebug::dPrint('Clicked on Home. No action',4);
		}
		
		if($triggered_from=="hidden-save"){
			
				$from_page =  $form_state['values']['form_from_page'];
				expDebug::dPrint('From Page: ' .print_r($from_page, true),4);
				
				$meta_attr = "name";
				$meta_attr_value = $form_state['values']['title'];
				$meta_content =  $form_state['values']['description'];
				
				$meta_result=get_meta_values_from_db($from_page);
				expDebug::dPrint("print metatag db result in submit action ".print_r($meta_result,true),5);
				
				$meta_count = count($meta_result);
				
				expDebug::dPrint('$meta_count for DB insert/update = '.$meta_count,5);
				
				//To get the ID of logged in user
				$userId = getIdOfLoggedInUser();
				expDebug::dPrint('Logged in User Id = ' .$userId, 4);
					
				//To get the Metatag Module ID
				$entityId = db_select('slt_profile_list_items','spli');
				$entityId->addField('spli','id');
				$entityId->condition('spli.code','cre_ste_mod_mtg','=');
				$entityId = $entityId->execute()->fetchField();
				expDebug::dPrint('Module entityId = ' .print_r($entityId,true), 4);
				
				//Insert or Update the values given in the Home form to the DB
				if($from_page == 'home' || $from_page == 'catalog'){
					if($meta_count == 0){
						$insertStmt = db_insert('custom_meta');
						$insertStmt->fields(array(
								'meta_attr' => $meta_attr,
								'meta_attr_value' => $meta_attr_value,
								'meta_content' => $meta_content,
								'page' => $from_page
						));
							
						expDebug::dPrintDBAPI("Insert metatags for $from_page = " ,$insertStmt,5);
						$insert_result = $insertStmt->execute();
						expDebug::dPrint("Insert metatags result for $from_page = " .print_r($insert_result,true),5);
			
			//Insert Audit log	
				add_audit_trail_entry($userId,$entityId,'custom_meta', 'exp_sp_administration_module_info', 'Add Metatags', 'Add Metatag '.$from_page.' title',null,null,$meta_attr_value);
				add_audit_trail_entry($userId,$entityId,'custom_meta', 'exp_sp_administration_module_info', 'Add Metatags', 'Add Metatag '.$from_page.' description',null,null,$meta_content);
								
			   } else{
						if(($result[0]->page == "home") && ($from_page == "home")){
							$meta_audit_title=$result[0]->meta_attr_value;
							$meta_audit_desc=$result[0]->meta_content;
						}elseif (($result[0]->page == "catalog")  && ($from_page == "catalog")) {
							$meta_audit_title=$result[0]->meta_attr_value;
							$meta_audit_desc=$result[0]->meta_content;
						}else {
							$meta_audit_title=$result[1]->meta_attr_value;
							$meta_audit_desc=$result[1]->meta_content;
						}
						
						$updateStmt = db_update('custom_meta');
						$updateStmt->fields(array(
								'meta_attr' => $meta_attr,
								'meta_attr_value' => $meta_attr_value,
								'meta_content' => $meta_content,
								'page' => $from_page
						));
							
						$updateStmt->condition('page',$from_page, '=');
						expDebug::dPrintDBAPI("Update metatags for $from_page = " ,$updateStmt);
						$upd_result = $updateStmt->execute();
						expDebug::dPrintDBAPI("Update metatags result for $from_page = " ,print_r($upd_result,true),5);
           
           //Update Audit log
           
		if($meta_attr_value != $meta_audit_title){
 		      add_audit_trail_entry($userId,$entityId, 'custom_meta', 'exp_sp_administration_module_info', 'Update Metatags', 'Update '.$from_page.' metatag title',null,$meta_audit_title,$meta_attr_value);
		}
		if($meta_content != $meta_audit_desc){
 		      add_audit_trail_entry($userId,$entityId, 'custom_meta', 'exp_sp_administration_module_info', 'Update Metatags', 'Update '.$from_page.' metatag description',null,$meta_audit_desc,$meta_content);
		}
		
				  }
		      }
				expertus_set_message(t('MSG689'));
		}
		
		drupal_flush_all_caches();
		$form_state['rebuild'] = TRUE;
		
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_metatag_form_submit', $ex);
		expertusErrorThrow($ex);
	}
}



function addHomeCatalogRepaintForm($form, &$form_state) {
	try {
		expDebug::dPrint(' addHomeCatalogRepaintForm() called.');
		expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
		expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5); 
		 
		 
		expDebug::dPrint(' $form_state = ' . print_r($form_state['values'], true) , 5);
		expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
		expDebug::dPrint(' $form_state[build_info] = ' . print_r($form_state['build_info'], true) , 5);
		
		
		$triggered_from=trim($form_state['triggering_element']['#name']);  
		
		expDebug::dPrint('$triggered_from = ' .print_r($triggered_from, true),4);
		
		expDebug::dPrint('$values form_from_page = ' .print_r($form_state['values']['form_from_page'], true),4);  
		
		if($triggered_from=="hidden-save" && $form_state['values']['form_from_page']!="" && $form_state['values']['form_from_page']=="catalog"){
			$triggered_from="catalog";
		}else if($triggered_from=="hidden-save" && $form_state['values']['form_from_page']!="" && $form_state['values']['form_from_page']=="home"){
			$triggered_from="home";
		}
		
		$result=get_meta_values_from_db(); 
		 
		$meta_title="";
		$meta_desc="";
		
		if($triggered_from=="catalog"){ 

			if( (trim($result[0]->meta_attr_value)!="" && trim($result[0]->meta_content)!="") && ($result[0]->page == "catalog") ){
				$meta_title=$result[0]->meta_attr_value;
				$meta_desc=$result[0]->meta_content;
			} else {
				$meta_title=$result[1]->meta_attr_value;
				$meta_desc=$result[1]->meta_content;
			}
			
			$form['form_from_page']['#value']='catalog';
			
		}else if($triggered_from=="home"){
		
		if( (trim($result[0]->meta_attr_value)!="" && trim($result[0]->meta_content)!="") && ($result[0]->page == "home") ){
				$meta_title=$result[0]->meta_attr_value;
				$meta_desc=$result[0]->meta_content;
			} else {
				$meta_title=$result[1]->meta_attr_value;
				$meta_desc=$result[1]->meta_content;
			}
			$form['form_from_page']['#value']='home';
		}
		expDebug::dPrint('$triggered_from before check = ' .print_r($form_state['triggering_element']['#name'], true),4);
		if($form_state['triggering_element']['#name'] == 'home' || $form_state['triggering_element']['#name'] == 'catalog') {
			$form['title']['#value']=$meta_title;
			$form['description']['#value']=$meta_desc;
		}		

		$commands = array(); 
		
		$commands[] = ajax_command_replace(NULL, drupal_render($form,$form_state));
		//$commands[] = ajax_command_prepend(NULL, theme('status_messages'));

		if (!empty($renderedAddlForms)) {
			//  $commands[] = ajax_command_after(NULL, $renderedAddlForms);
		}
		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
		if (!empty($statusMsgHTML)) {
			// Show the messages
			$commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
		}

		$commands[] = exp_ctools_modal_adjust();

		if ($form_state['storage']['new_entity'] == TRUE || $form_state['storage']['edited_entity'] == TRUE) {
			$commands[] = ajax_command_refresh_narrow_search_results('onclose');
		}

		if($form_state['triggering_element']['#ajax']['tabaction']=='notification'){
			$commands[] = ajax_command_invoke(NULL, "setNotifyContainerWidth",array());
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

		expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);

		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('addHomeCatalogRepaintForm', $ex);
		expertusErrorThrow($ex);
	}
}




function addEditFormWithmetatagCancelAndSaveClose(&$form, $wrapperId, $tabIndex) {
	try {
		// Button ids
		$saveButtonId = 'metaserver-save-button';
		$cancelButtonId = 'metaserver-cancel-button';
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
		$cancelAttributes['tabindex'][] = $tabIndex + 1;

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
		//$ajaxCallbackSave = 'addCatalogRepaintForm';

		$ajaxCallbackSave = "addHomeCatalogRepaintForm";
		
		// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
		// each time the form is re-built.
		$saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

		// Using this class to applying bg color to the save button
		$saveAttributes['class'][] = 'admin-save-button-middle-bg';
		$saveAttributes['tabindex'][] = $tabIndex;
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
				'#markup' =>   '</div>' .
				'<div class="clearBoth"></div>',
		);
	}
	catch (Exception $ex) {
		watchdog_exception('addEditFormWithmetatagCancelAndSaveClose', $ex);
		expertusErrorThrow($ex);
	}
}

?>
