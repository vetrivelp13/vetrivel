<?php

function displayTPRegisterDetails(&$form, &$form_state, $entityId, $entityType, $uniqueId){
	try {
		global $theme_key, $secure_entities;	
		if (in_array($entityType, $secure_entities)) {
  			$entityId_encrypted = core_encrypt($entityId); // URL encryption
  		} else {
  			$entityId_encrypted = $entityId;
  		}
		$uniqueId = "enrolltpuser-".$entityId_encrypted."-$entityType";
		$customHiddenButtonId = 'enroll-user-esign';
		$msg = t('MSG308');
		$excludeIdRecertify = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? 'R' : 0;
		$wrapperId = 'enrolltpuser-grid-wrapper-'.$uniqueId;

		$formName = 'enrolltpuser_form_'.$uniqueId;
		$form_state['form_unique_name'] = $formName;
		$form_state['form_program_id'] = $entityId;
		$form_state['form_program_type'] = $entityType;

		$form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'enrolltpuser',
		);
	
		$setminHight = ($entityType == 'cre_sys_obt_crt' || $entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur') ? 'admin-training-program-tab-datagrid-wrapper' : '';

		$form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="admin_list_values_container" class="enrolled-display-controller"><div id="'.$wrapperId.'" class="admin-datagrid-pagination enrolltp-grid-wrapper-list-page enrolltp-grid-wrapper '.$setminHight.'">',
    '#suffix' => '</div></div></div></div>',
		);

		$form[$formName]['hidden_idlist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_idlist_'.$uniqueId,
    '#id' => 'datagrid-idlist-'.$uniqueId,
		);

		$form[$formName]['hidden_valuelist'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_valuelist_'.$uniqueId,
    '#id' => 'hidden-valuelist-'.$uniqueId,
		);

		$form[$formName]['hidden_completion_date'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_completion_date_'.$uniqueId,
    '#id' => 'datagrid-completiondate-'.$uniqueId,
		);

		$form[$formName]['hidden_certificate_path'] = array(
		    '#type' => 'hidden',
		    '#name' => 'hidden_certificate_path_'.$uniqueId,
		    '#id' => 'datagrid-certificate_path-'.$uniqueId,
		);

		$displayMode = $form_state['storage']['mode'];

		$enrolluserDetails = fetchEnrollTPUserList($entityId, $entityType,'', '', '','','',$displayMode,1);
		$listrecCount = fetchEnrollTPUserList($entityId, $entityType,'', '', '','','R',$displayMode,1);
		$listrecCount = $listrecCount[0]->cnt ;
		$count = $enrolluserDetails[0]->cnt;
		$listqueryCount = loadCtoolDataGridForEnrollTPUser('', 0, $entityId, 1);

		$tpStatus = _checkTPEnabled($entityId);

		if($count == 0 && empty($displayMode)){

			$addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
			$addAttributes['data-wrapperid'] = array($wrapperId);

			$programValid = getIfProgramValid($entityId);

			if($listqueryCount == 0 || $programValid[0]->sesavailable == 0 || $tpStatus == false){

				$msg = t('MSG448');

				$form[$formName]['add_enrolltpuser'] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
        '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL640').'</div><div class="admin-save-button-right-greyout-bg"></div></span></div>',
				);

				$form[$formName]['add_enrolltpuser_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
				);

			} else {
				$setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'admin-save-button-left-bg';
				$form[$formName]['add_enrolltpuser'] = array(
        '#type' => 'submit',
        '#prefix' => '<div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="'.$setclass.'"></div>',
        '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
        '#title' => t('LBL367'),
        '#name' => 'Add enrolltpuser',
        '#value' => t('LBL640'),
        '#submit' => array('add_enrolltpuser'),
        '#attributes' => $addAttributes,
        '#ajax' => array(
          'callback' => 'enrollTPUserGrid',
          'wrapper' => $wrapperId,
				),
				);

				$form[$formName]['add_enrolltpuser_text'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div>',
				);

			}

		} else {


			$paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
			$paintMultiAction .= "<span id='search-dropdwn-list'>";
			$paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
			$paintMultiAction .= "<input type='hidden' id='enrolltpuser-autocomplete_hidden' value='".t('LBL181')."' />";
			$paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>".t('LBL054')."</span>";
			$paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
			$paintMultiAction .= "<ul id='select-list-dropdown-list'>";
			$paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','TP');\">".t('LBL054')."</li>";
			$paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','TP');\">".t('LBL107')."</li>";

			if($displayMode != 'edit'){
				$paintMultiAction .= "<li id='status-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('status','TP');\">".t('LBL102')."</li>";
				// Added by Vincent for #0072720
				$paintMultiAction .= "<li id='path-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('path','TP');\">".t('LBL3060')."</li>";
			}

			$paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','TP');\">".t('Organization')."</li>";
			$paintMultiAction .= "<li id='jobrole-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('jobrole','TP');\">".t('LBL133')."</li>";
			$paintMultiAction .= "<li id='usertype-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('usertype','TP');\">".t('LBL173')."</li>";
			$paintMultiAction .= "<li id='manager-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('manager','TP');\">".t('Manager')."</li>";
			$paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','TP');\">".t('Group')."</li>";
			$paintMultiAction .= "</ul>";
			$paintMultiAction .= "</span></div>";


			if($displayMode == 'edit'){

				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
				);

				$form[$formName]['more_add_enrolltpuser_search'] = array(
      '#type' => 'markup',
      '#markup' => $paintMultiAction,
				);

				$searchAjaxEvent = 'click';
				$searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "enrolltpuser", $(\'#enrolltpuser-autocomplete\').val(), "'.$entityId_encrypted.'", "'.$entityType.'", "'.$excludeIdRecertify.'",$(\'.skip_tp_access\').attr(\'checked\')); return false;';

				$acMenuPath = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? 'administration/learning/catalog-class/notenrolledtpuser-autocomplete/R' :'administration/learning/catalog-class/notenrolledtpuser-autocomplete/E';

				$form[$formName]['add_enrolltpuser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
				);

				$autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
				$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
				addACFieldForMultiSelect($form[$formName], 'enrolltpuser-autocomplete', $acMenuPath, '', '', '', array(),
				array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'",'search_type' => "$('#search_all_enroll_type-hidden').val()"), false, false,
				t('LBL181'), $autoCompleteAttributes);

				$form[$formName]['add_enrolltpuser_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
				);

				$searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolltpuser\", $(\"#enrolltpuser-autocomplete\").val(), $entityId,\"$entityType\",\"$excludeIdRecertify\",$(\".skip_tp_access\").attr(\"checked\"));'></a>";
				$form[$formName]['search_enrolltpuser'] = array(
      '#type' => 'markup',
      '#markup' => $searchlink,
				);
				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>',
				);


			} else {

				$displayMode = 'view';

				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
				);

				$form[$formName]['more_add_enrolltpuser_search'] = array(
      '#type' => 'markup',
      '#markup' => $paintMultiAction,
				);

				$acMenuPath = 'administration/learning/catalog-class/enrolledtpuser-autocomplete';

				$form[$formName]['add_enrolluser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search tp-list-admin_add_multi_auto_search">',
				);

				$autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
				$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
				addACFieldForMultiSelect($form[$formName], 'enrolltpuser-autocomplete', $acMenuPath, '', '', '', array(),
				array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'", 'id_required' => 0,'search_type' => "$('#search_all_enroll_type-hidden').val()"), false, false,
				t('LBL181'), $autoCompleteAttributes);

				$form[$formName]['add_enrolluser_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
				);

				$searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolltpuser\", $(\"#enrolltpuser-autocomplete\").val(), $entityId,\"$entityType\", 0);'></a>";
				$form[$formName]['search_enrolltpuser'] = array(
      '#type' => 'markup',
      '#markup' => $searchlink,
				);
				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>',
				);

				$addClassName = ($form_state['call_from'] == 'training_plan_enroll_display') ? '' : 'alter-margin-top';
				$form[$formName]['export_link'] = array(
      '#type' => 'markup',
      '#markup' => "<div id='exportcontainer' class='exportcontainer ".$addClassName."'><a onclick=\"$('body').data('mulitselectdatagrid').callEnrollExportProcess('','".$entityId."','".$entityType."','TP');\" class='enrollments-exports-icon' title=".t('LBL309')."></a></div>",
				);

			}

			$form[$formName]['enrolltpuser_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
			);
			// alert($(\'#datagrid-div-'.$uniqueId.'\').html());


			$form[$formName]['statistics_count'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="enrollment-statistics-count tp-enroll-list-statistics-count" id="statistics-count-'.$uniqueId.'"></div>',
			);

			$form[$formName]['enrolltpuser_view_grid_load'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayMode.'\', \'enrolltpuser\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$excludeIdRecertify.'\');" width="100" height="132" /></div>',
			);

			if($displayMode == 'view'){

				$addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
				$addAnotherAttributes['data-wrapperid'] = array($wrapperId);

				$programValid = getIfProgramValid($entityId);

				if($listqueryCount != 0){
					/*
					 $form[$formName]['add_enrolltpuser_view_mode'] = array(
					 '#type' => 'submit',
					 '#title' => t('LBL367'),
					 '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'" style="display:block;" class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
					 '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
					 '#value' => t('LBL183'),
					 '#name' => 'Add enrolltpuser',
					 '#submit' => array('add_enrolltpuser'),
					 '#attributes' => $addAnotherAttributes,
					 '#ajax' => array(
					 'callback' => 'enrollTPUserGrid',
					 'wrapper' => $wrapperId,
					 ),
					 );
					 */
				}

				addEditSaveTPFormForEnrollUser($form, $formName, $wrapperId, 'save-button', 'cancel-button', '', $entityId, $entityType, $uniqueId, $listqueryCount,$programValid, $tpStatus,$listrecCount);
				/*
				 $form[$formName]['add_enrolltpuser_view_mode_btn'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL367'),
				 '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
				 '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div></div>',
				 '#value' => t('LBL183'),
				 '#name' => 'Add enrolltpuser',
				 '#submit' => array('add_enrolltpuser'),
				 '#attributes' => $addAnotherAttributes,
				 '#ajax' => array(
				 'callback' => 'enrollTPUserGrid',
				 'wrapper' => $wrapperId,
				 ),
				 );


				 $form[$formName]['enrolltpuser_view_grid_no_msg'] = array(
				 '#type' => 'markup',
				 '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div>',
				 );
				 */
					

			} else if($displayMode == 'edit'){
			
				

				$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle  grid-refresh';
				$cancelAttributes['data-wrapperid'] = array($wrapperId);
				$form[$formName]['display_unassigned_user'] = array(
						'#type' => 'markup',
						'#markup' => "<div class=\"admin-datagrid-save-cancel-button-align-bottom\"><div class='checkbox-label-wrapper'><div class=\"checkbox checkbox-unselected\"><input onclick=\"checkboxSelectedUnselectedCommon(this);$('body').data('mulitselectdatagrid').searchDataGrid('$displayMode', 'enrolltpuser', $('#enrolltpuser-autocomplete').val(), '$entityId_encrypted','$entityType', '$excludeIdRecertify', this.checked);\" id=\"skip_access\" class=\"skip_tp_access\" type=\"checkbox\"></div><label class=\"option\" for=\"skip_access\">".t('Show only users who do not have access')."</label></div>",
						'#id'=>'skip_access',
				);
				$form[$formName]['cancel_enrolltpuser_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),// Cancel',
          '#title' => t('LBL109'),
          '#prefix' => '<div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2 enroll-tp-user-save-button-contaniner"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel enrolltpuser',
          '#submit' => array('cancelEnrollTPUserDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'enrollTPUserGrid',
            'wrapper' => $wrapperId,
             'method' => 'replace',
  	        'effect' => 'none', // 'fade',
  	      	'event'=>'click',
				),
				);

				$condCount = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? $listrecCount : $listqueryCount;
				if($condCount != 0){

					if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
						$buttonName  = 'esign-enrolltpuser';
					}else{
						$buttonName  = 'set-enrolltpuser';
					}
					//$recertifyValue = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? 'R' : '';
					
					$recertifyValue = '';
					//Get recertify sequence
					if($form_state['triggering_element']['#name'] == 'Add_recertifyuser'){
						$qry = db_select('slt_module','module');
						$qry->addField('module','sequence');
						$qry->condition('module.id',$form_state['input']['selected_enroll_path'],'=');
						$recertify_seq  = $qry->execute()->fetchField();
						$recertifyValue = ($recertify_seq).'-R';
					}
					$maxCount = getCourseClassCountForLP($entityId,'','Y',$recertifyValue);

					if($maxCount[0]->cnt == '-1' || $maxCount[0]->cnt == 0){
						$registerStatus = "noclasses";     	// no classes under any one of the courses
					} else if($maxCount[0]->cnt == 1) {
						$registerStatus = "singleclass";
					} else {
						$registerStatus = "MultiRegister";
					}

					$btnName = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? t('LBL429').' '.t('User') : t('LBL640') ;
					$multiClassContainer = "<div id='multi-class-container' style='display:none;'>" .
            						'<table width="750px" height="auto" cellspacing="0" cellpadding="0" class="tp-bubble-popup-multi-register" id="bubble-face-table">' .
            							'<tbody>' .
            								'<tr>' .
            							      	'<td class="bubble-tl"></td>' . 
      											'<td class="bubble-t"></td>' .
      											'<td class="bubble-tr"></td>' .
    										'</tr>' .
        							        '<tr>' .
											    '<td class="bubble-cl"></td>' .
											    '<td valign="top" class="bubble-c"><div id="show_expertus_tp_message"></div><div id="multi-select-class-wrapper" class="multi-select-class-wrapper">' .
											    '<a onclick="$(\'#multi-class-container\').hide();$(\'.bottom-qtip-tip-up\').css(\'z-index\',\'100\');$(\'.bottom-qtip-tip-up-visible\').css(\'z-index\',\'101\');var x=$(\'.scroll-pane\').data(\'jsp\');x.destroy();$(\'.grid-refresh\').click();" class="bubble-qtip-button-close"></a>' .
											    '<div id="multi-container-pop-up"  class="multi-class-container-cls scroll-pane"></div></div>' .
											    '<div class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span class="admin-action-button-middle-bg white-btn-bg-middle" onclick="$(\'#multi-class-container\').hide();$(\'.bottom-qtip-tip-up\').css(\'z-index\',\'100\');$(\'.bottom-qtip-tip-up-visible\').css(\'z-index\',\'101\');var x=$(\'.scroll-pane\').data(\'jsp\');x.destroy();$(\'.grid-refresh\').click();">'.t('LBL123').'</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg"></div><input type="button" value = "'.t("LBL569").'"class="admin-save-button-middle-bg" onClick="$(\'body\').data(\'adminlearningcore\').registerTPClassDetailsAdminSide(\''.$entityId.'\',\'newadmin\',\''.$recertifyValue.'\'); return false;"><div class="admin-save-button-right-bg"></div></div>' .

											    '</td>' .
											    '<td class="bubble-cr"></td>' .
										    '</tr>' .
										    '<tr>' .
										      	'<td class="bubble-bl"></td>' .
										      	'<td class="bubble-b">' .
										        	'<table width="100%" cellspacing="0" cellpadding="0">' .
										          		'<tbody>' .
										            		'<tr>' .
												              '<td class="bubble-blt" style="width:325px;"></td>' .
												              '<td class="bubble-blr" style="width:325px;"></td>' .
												              '<td class="bubble-blm"><div rel="bottomRight" dir="ltr" class="qtip-tip"></div></td>' .
										            		'</tr>' .
										          		'</tbody>' .
										        	'</table>' .
										      	'</td>' .
										      '<td class="bubble-br"></td>' .
										    '</tr>' .
										    
            							'</tbody>' .
									'</table></div>';
					$setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'admin-save-button-left-bg';
					// $setclass = 'admin-save-button-left-bg';
					if(strtolower($registerStatus) == 'multiregister'){
						$displayStyle = "display:none;";
						$setAttributes['class'][] = 'addedit-form-expertusone-throbber';
						$setAttributes['data-wrapperid'] = array($wrapperId);
			$form[$formName]['multi-class-button'] = array(
  	              '#type' => 'markup',
  	              '#prefix' => $multiClassContainer.'<div class="admin-save-button-container">',
  	    	      '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'cls-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
                  '#markup' => '<span class="admin-save-button-middle-bg" onClick="$(\'body\').data(\'adminlearningcore\').multiClassQTip(\''.$entityId.'\',\''.$uniqueId.'\',\''.$recertifyValue.'\'); return false;">'.$btnName.'</span>',
						);
			
			
	//Multi register mandatory enrollment 		
			$form[$formName]['addedit-button-group-ul-start'] = array(
					'#type' => 'markup',
					'#markup' => '<ul class="catalog-pub-add-list cls-pub-save-btn">',
			); //  catalog-save-btn
				
			$saveButtonId = 'enrolltp-user-man';
	    $enrollMandatoryAttributes['class'][] = 'addedit-form-expertusone-throbber enrolltp-user-multiregister';
			$enrollMandatoryAttributes['data-wrapperid'] = array($wrapperId);
			$enrollMandatoryAttributes['onclick'][] = 'clearMessages();$(\'body\').data(\'adminlearningcore\').multiClassQTip(\''.$entityId.'\',\''.$uniqueId.'\',\''.$recertifyValue.'\'); return false;';
			$form[$formName][$saveButtonId] = array(
										'#type' => 'submit',
					'#prefix' => '<li class="save-pub-unpub-sub-menu">',
					'#suffix' => '</li>',
					'#value' => t('Enroll user and set training as mandatory'),
					'#name' => 'set-enroll-tp-user-mandatory',
					'#attributes' => $enrollMandatoryAttributes	
			);		
			$form[$formName]['addedit-button-group-ul-end'] = array(
					'#type' => 'markup',
					'#markup' => '</ul><div class="clearBoth"></div></div></div>'
			
			);			
			$form[$formName]['hidden-item'] = array(
					'#type' => 'hidden',
					'#attributes' => array('id' => 'multi_register_mandatory'),
					'#default_value' => ''
			);	
					
					}else{
						$displayStyle = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? "display:none;" : "display:block;";
						$butTitle = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? 'recertify' : t('LBL305');
						$buttonValue = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? t('LBL429').' '.t('User') : t('LBL640');
						//$setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
						
						if($form_state['triggering_element']['#name'] == 'Add_recertifyuser'){
							 	$setAttributes = array();
				            	$setAttributes['class'][] = 'addedit-form-validate-attempts admin-save-button-middle-bg';
				            	$setAttributes['data-wrapperid'] = array($wrapperId);
				            	$setAttributes['onclick'][] = 'return checkIfContentAttemptsSet(\''.$uniqueId.'\',\'recertify\');';
				            	$form[$formName]['set_recertify_user_edit_mode_validate'] = array(
									'#type' => 'button',
									'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
									'#suffix' => '<div class="admin-save-button-right-bg"></div></div></div></div>',
									'#title' => $butTitle,
									'#value' => $buttonValue,
									'#name' => $buttonName,
									'#attributes' => $setAttributes,
				            );
						}
						
// 						$setclass = 'admin-save-button-left-bg';
						$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
						$setAttributes['data-wrapperid'] = array($wrapperId);
						$form[$formName]['set_enrolltpuser_edit_mode'] = array(
	              '#type' => 'submit',
	              '#prefix' => '<div class="admin-save-button-container" style="'.$displayStyle.'">',
	    	      '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'cls-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
	              '#title' => $butTitle,
	              '#value' => $buttonValue,
	              '#name' => $buttonName,
	              '#attributes' => $setAttributes,
							 	'#id' => 'submit-enrolltp-'.$uniqueId,
	              '#ajax' => array(
	                'callback' => 'addEditRepaintEnrolltoTrainingForm',
	                'wrapper' => $wrapperId,
	                'entity_id' => $entityId, 
	                'entity_type' => $entityType,
            		'entity_plugin'=>'enrolltpuser',
	            	'customButtonId'=>$customHiddenButtonId,
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
						
						




						$form[$formName]['addedit-button-group-ul-start'] = array(
								'#type' => 'markup',
								'#markup' => '<ul class="catalog-pub-add-list cls-pub-save-btn">',
						); //  catalog-save-btn
							
						$saveButtonId = 'enroll-user-man';
						$enrollMandatoryAttributes['data-wrapperid'] = array($wrapperId);
						$enrollMandatoryAttributes['class'][] = 'addedit-form-expertusone-throbber';
						$enrollMandatoryAttributes['onclick'][] = 'clearMessages();';
							
							
						$form[$formName][$saveButtonId] = array(
								'#type' => 'submit',
								'#prefix' => '<li class="save-pub-unpub-sub-menu">',
								'#suffix' => '</li>',
								'#value' => t('Enroll user and set training as mandatory'),
								'#name' => 'set-enroll-tp-user-mandatory',
								'#attributes' => $enrollMandatoryAttributes,
								'#ajax' => array(
										'callback' => 'addEditRepaintEnrolltoTrainingForm',
										'wrapper' => $wrapperId,
										'entity_id' => $entityId,
										'entity_type' => $entityType,										
										'entity_plugin'=>'enrolltpuser',
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
							
							
						$form[$formName]['addedit-button-group-ul-end'] = array(
								'#type' => 'markup',
								'#markup' => '</ul><div class="clearBoth"></div></div></div>'
						
						);
						
						
          		        $setAttributes = array();
          		        $form[$formName][$customHiddenButtonId] = array(
			    '#type' => 'submit',
			    '#value' => t('hidden-save'),
			    '#name'	=> 'hidden-save',
			    '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
			    '#suffix' => '</span>',
			    '#id' =>$customHiddenButtonId,
			  	'#attributes' => $setAttributes,
			    '#ajax' => array(
			      'wrapper' => $wrapperId,
			      'callback' => 'addEditRepaintEnrolltoTrainingForm',
                  'entity_id' => $entityId, 
                  'entity_type' => $entityType,
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
				//skip access
			/* 	$form[$formName]['display_unassigned_user'] = array(
						'#type' => 'checkbox',
						'#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom">',
						'#id'=>'skip_access',
						'#title' => t('Show only users who do not have access'),
						'#attributes' => array('onclick' => "$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolltpuser\", $(\"#enrolltpuser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", \"$excludeIdRecertify\", this.checked);"),
						//'#default_value' => ($defaultValue['display_unassigned_user'] == 1) ? True : FALSE
				); */
				
					
				
				/* 			$form[$formName]['group_accsss_list'] = array(
				 '#type' => 'radios',
				 //'#title' => t('Annotations will be deleted'),
				 //'#description' => t('Select a method for deleting annotations.'),
				 '#attributes' => array('onclick' => "$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolluser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0,this.value);"),
				 '#options' => array(
				 'open'=>t('show only users who do not have access '),
				 'group'=>t('show only users who have access'),
				 )); */
				
				
				//skip access
			}

			$form_state['storage']['mode'] = '';

		}
	} catch (Exception $ex) {
		watchdog_exception('displayTPRegisterDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function displayTPRecertifyDetails(&$form, &$form_state, $entityId, $entityType ,$enrollId, $uniqueId){

try {
		global $theme_key, $secure_entities;	
		if (in_array($entityType, $secure_entities)) {
  			$entityId_encrypted = core_encrypt($entityId); // URL encryption
  		} else {
  			$entityId_encrypted = $entityId;
  		}
		$uniqueId = "enrolltprecertify-".$entityId_encrypted."-$entityType";
		$customHiddenButtonId = 'enroll-user-esign';
		$msg = t('MSG308');
		$excludeIdRecertify = ($form_state['triggering_element']['#name'] == 'Add_recertifyuser') ? 'R' : 0;
		$wrapperId = 'enrolltpuser-grid-wrapper-'.$uniqueId;

		$formName = 'enrolltprecertify_form_'.$uniqueId;
		$form_state['form_unique_name'] = $formName;
		$form_state['form_program_id'] = $entityId;
		$form_state['form_program_type'] = $entityType;

		$form['attr_action'] = array(
		    '#type' => 'hidden',
		    '#name' => 'attr_action',
		    '#value' => 'enrolltpuser',
		);
	

		$form[$formName] = array(
		    '#type' => 'markup',
		    '#prefix' => '<div id="admin-data-grid"><div id="admin_list_values_container" class="enrolled-display-controller"><div id="'.$wrapperId.'" class="admin-datagrid-pagination enrolltp-grid-wrapper-list-page enrolltp-grid-wrapper '.$setminHight.'">',
		    '#suffix' => '</div></div></div></div>',
		);

		$form[$formName]['hidden_idlist'] = array(
		    '#type' => 'hidden',
		    '#name' => 'hidden_idlist_'.$uniqueId,
		    '#id' => 'datagrid-idlist-'.$uniqueId,
		);

		$form[$formName]['hidden_valuelist'] = array(
		    '#type' => 'hidden',
		    '#name' => 'hidden_valuelist_'.$uniqueId,
		    '#id' => 'hidden-valuelist-'.$uniqueId,
		);

		$displayMode = 'view';

		//$listqueryCount = fetchEnrollmentHistory($entityId, $entityType ,$enrollId);
		
		/*$paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
		$paintMultiAction .= "<span id='search-dropdwn-list'>";
		$paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
		$paintMultiAction .= "<input type='hidden' id='enrolltpuser-autocomplete_hidden' value='".t('LBL181')."' />";
		$paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>".t('LBL054')."</span>";
		$paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
		$paintMultiAction .= "<ul id='select-list-dropdown-list'>";
		$paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','TP');\">".t('LBL054')."</li>";
		$paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','TP');\">".t('LBL107')."</li>";

		//if($displayMode != 'edit'){
			$paintMultiAction .= "<li id='status-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('status','TP');\">".t('LBL102')."</li>";
		//}

		$paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','TP');\">".t('Organization')."</li>";
		$paintMultiAction .= "<li id='jobrole-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('jobrole','TP');\">".t('LBL133')."</li>";
		$paintMultiAction .= "<li id='usertype-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('usertype','TP');\">".t('LBL173')."</li>";
		$paintMultiAction .= "<li id='manager-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('manager','TP');\">".t('Manager')."</li>";
		$paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','TP');\">".t('Group')."</li>";
		$paintMultiAction .= "</ul>";
		$paintMultiAction .= "</span></div>";*/


		/*$form[$formName][] = array(
      		'#type' => 'markup',
      		'#markup' => '<div class="admin_add_multi_search_container">',
		);

		$form[$formName]['more_add_enrolltprecertify_search'] = array(
      		'#type' => 'markup',
      		'#markup' => $paintMultiAction,
		);

		$acMenuPath = 'administration/learning/catalog-class/enrolledtpuser-autocomplete';

		$form[$formName]['add_enrolltprecertify_search'] = array(
              '#type' => 'markup',
                '#markup' => '<div class="admin_add_multi_auto_search tp-list-admin_add_multi_auto_search">',
			);

			$autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
			$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
			addACFieldForMultiSelect($form[$formName], 'enrolltprecertify-autocomplete', $acMenuPath, '', '', '', array(),
			array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'", 'id_required' => 0,'search_type' => "$('#search_all_enroll_type-hidden').val()"), false, false,
			t('LBL181'), $autoCompleteAttributes);

			$form[$formName]['add_enrolltprecertify_search1'] = array(
                    '#type' => 'markup',
      			'#markup' => '</div>',
			);

			$searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolltpuser\", $(\"#enrolltpuser-autocomplete\").val(), $entityId,\"$entityType\", 0);'></a>";
			$form[$formName]['search_enrolltprecertify'] = array(
                   '#type' => 'markup',
                    '#markup' => $searchlink,
			);
			$form[$formName][] = array(
                     '#type' => 'markup',
                     '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>',
			);*/

			$form[$formName]['export_link'] = array(
			      '#type' => 'markup',
			      '#markup' => "<div id='exportcontainer' class='exportcontainer ".$addClassName."'><a onclick=\"$('body').data('mulitselectdatagrid').callRecertifyExportProcess('".$entityId."','".$entityType."','".$enrollId."');\" class='enrollments-exports-icon' title=".t('LBL309')."></a></div>",
				);


			$form[$formName]['enrolltprecertify_view_grid_markup'] = array(
		      '#type' => 'markup',
		      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
			);


			$form[$formName]['statistics_count'] = array(
      				'#type' => 'markup',
      				'#markup' => '<div class="enrollment-statistics-count tp-enroll-list-statistics-count" id="statistics-count-'.$uniqueId.'"></div>',
			);

			$form[$formName]['enrolltprecertify_view_grid_load'] = array(
			      '#type' => 'markup',
			      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayMode.'\', \'enrolltprecertify\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \''.$enrollId.'\');" width="100" height="132" /></div>',
			);
			
			//$cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);
			$cancelButtonId = "recertify-close";
			$cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
			$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';
			$cancelAttributes['onclick'][] = "closeQtip('enrolled-history-".$enrollId."','".$enrollId."'); ";
		    $cancelAttributes['tabindex'][] = '117';
		     
		    $prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
    		$suffixPgRight = '<div class="white-btn-bg-right"></div>';
		      
		    $form[$cancelButtonId] = array(
		      '#type' => 'submit',
		      '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
		                       '<div class="addedit-form-cancel-container-actions">'.$prfixPgLeft,
		       '#suffix' =>  $suffixPgRight,
		      '#value' => t('LBL123'),
		      '#name' => 'cancel',
		      '#attributes' => $cancelAttributes
		      );

			$addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
			$addAnotherAttributes['data-wrapperid'] = array($wrapperId);

			$form_state['storage']['mode'] = '';
	} catch (Exception $ex) {
		watchdog_exception('displayTPRegisterDetails', $ex);
		expertusErrorThrow($ex);
	}
	
}


function addEditSaveTPFormForEnrollUser(&$form, $formName, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $subStage = '', $entityId, $entityType, $uniqueId, $listqueryCount,$programValid, $tpEnabled,$listrecCount) {
	try {
		global $theme_key,$user;
		expDebug::dPrint(' addnewcatalog formstatevaluesUpdate 11111 : '.print_r($entityId,true), 5);
		// Remove the temporary storage for API's to exchange information during form creation
		$esignHiddenButt = 'esign-'.$wrapperId;
		// Add the Cancel button
		$ajaxCallbackCancel = 'addEditCancelRepaintForm';
		//$ajaxCallbackCancel = 'enrollUserGrid';

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

		// Remove the error messages or success messages on clicking cancel
		$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();'."closeQtip('','$entityId');  return false; ";

		$uniqueId2 = $entityId.$entityType;

		$form[$formName][$cancelButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom tp-enroll-save-cancel-button-align-bottom"><div class="addedit-form-cancel-and-save-actions-row">' .
                     '<div class="addedit-form-cancel-container-actions enrolluser-save-cancel-btn-container resize-save_btn4"><div class="white-btn-bg-left"></div>',
      '#suffix' =>   '<div class="white-btn-bg-right"></div>',
      '#value' => t('LBL123'),
      '#name' => 'cancel',
      '#attributes' => $cancelAttributes,
      '#ajax' => array(
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
          )
          );

          // Add the Save button

          $ajaxCallbackSave = 'enrollTPUserGridCallback';
          //$wrapperId        = 'add-class-list-wrapper';

          // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
          // each time the form is re-built.
          $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
          $saveOtherAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

          // For common styling of all addedit save buttons
          //$saveAttributes['class'][] = 'addedit-edit-save';

          // Using this class to applying bg color to the save button
          //$saveAttributes['class'][] = 'admin-save-button-middle-bg';
          $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
           
          // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
          $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
          $saveOtherAttributes['class'][] = 'addedit-form-expertusone-throbber';

          // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
          $saveAttributes['data-wrapperid'] = array($wrapperId);
          $saveOtherAttributes['data-wrapperid'] = array($wrapperId);

          //Increase the width of the popup while clicking on save button
          $saveAttributes['onclick'][] = 'increasePopWidth();';

          // Remove the error messages or success messages on clicking submit

          $eSignatureAvailable = ($subStage == 'Attachment') ? false : true;

          $completionId = '#completion_date_container_'.$uniqueId2;
          $date = date('m/d/Y');
          // EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader($wrapperId);
          $completiondatelbl = t('LBL706');
          $closelbl = t('LBL123');
          $donelbl = t('LBL569');
          $completionContainer = <<<EOD
<div class="completion_container_roster" id="completion_date_container_$uniqueId2">
<a style="float: right; position: relative; cursor: pointer;" class="qtip-button"  onClick="$('$completionId').hide();"><div class="admin-bubble-close"> </div></a>
          $completiondatelbl:  <input size="8" type="text" name="overall_completion_date_$uniqueId2" id="overall_completion_date_$uniqueId2" class="enrollement-date-picker" value="$date"/>
	<div id="enrolltp-user-completion-date-popup"<div class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span onClick="$('$completionId').hide();" class="admin-action-button-middle-bg white-btn-bg-middle">$closelbl</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg completion-button-space"></div><input type="button" class="admin-save-button-middle-bg"  value="$donelbl" onClick="$('#root-admin').data('narrowsearch').fillCompletionDate('$uniqueId', '$uniqueId2');$('#root-admin').data('narrowsearch').showConfirmPopup('completedandsave_$uniqueId2', 'completedandsave', 'hidden_idlist_$uniqueId','TP','.$isPricedd.'); return false;"/><div class="admin-save-button-right-bg"></div></div>
</div>
EOD;

          
          $qtipOptWaivedObj  = "{'entityId' : '".$entityId."'".
          		", 'entityType' : '".entityType ."'" .
          		", 'url' : 'administration/enrollment/exempted/all/" . $entityId . "/0/tp/0'" .
          		", 'popupDispId' : 'enrolled-all-exempted-".$entityId . "'".
          		", 'catalogVisibleId' : 'enrolled-all-exempted-disp'" .
          		", 'wid' : 400".", 'heg' : 200" .", 'postype' : 'topright'" .",'poslwid' : 30 ".", 'qdis' : 'ctool'" .
          		", 'linkid' : 'visible-all-exempted-". $entityId."','enrId' : '".entityId ."'".", 'waivedfrom' : 'all'}";
          
          
          //Recertify user
		  if(in_array($entityType, $secure_entities)){
	  			$entityId_encrypted = core_encrypt($entityId); // URL encryption
	  	  }else{
	  			$entityId_encrypted = $entityId;
	  	  }
          $uniqueId = "enrolltpuser-".$entityId_encrypted."-$entityType";
          $RecertifyId = '#recertify_user_container_'.$uniqueId2;
         
          //select path popup
          $qtipIdInit        	 = $entityId.'_'.$entityType;
          if($user->language =='es' || $user->language =='ru' || $user->language =='fr' || $user->language =='it')
          	$popupWidth = '320';
          else if($user->language =='zh-hans')
          	$popupWidth = '280';
          else if($user->language =='ko' || $user->language =='ja')
          	$popupWidth = '250';
          else if($user->language =='en-us')
          	$popupWidth = '260';
          else
          	$popupWidth = '300';
          $qtipOptSelectObj      = "{'entityId':'".$entityId."','entityType':'".$entityType."',
																	'url':'administration/selectpath/roster/".$entityId."/".$entityType."','popupDispId':'qtip_visible_disp_".$qtipIdInit."',
																	'wid':'".$popupWidth."','heg':'120','postype':'topright',posrwid :'180',
																	'catalogVisibleId':'qtipSelectModulepathqtip_visible_disp_".$qtipIdInit."',
																	'popupDispId':'qtip_visible_disp_select_roster_path_".$qtipIdInit."',
																	'linkid':'visible-roster-path-".$entityId."'}";
          
          $recertifyPrefix = "<div id='qtip_visible_disp_select_roster_path_".$qtipIdInit."' class='set-survey-container'><a id='visible-roster-path-".$entityId."' onclick = \"exemptedVisibility(".$qtipOptSelectObj.");\" class = 'survey-tp-path'></a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>";
          	
          
          
          
         $isPricedTP = getProgramObjDetails($entityId);
		 $isPricedd = (($isPricedTP[0]->price) > 0) ? 1 : 0;
          $isSaveParent = 1;
          $setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
		  $form[$formName][$saveButtonId.'-markup'] = array(
          		'#type' => 'markup',
          		'#prefix' => '<div class="admin-save-pub-unpub-button-container"><span id="setwaived-visible-container"><span id="enrolled-all-exempted-' . $entityId . '" class="enrolled-exempted-all-status"><a id="visible-all-exempted-'.$entityId.'" class="enrolled-all-exempted" onclick = "exemptedVisibility('.$qtipOptWaivedObj.');"></a><span id="visible-popup-' . $entityId . '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span></span></span><div class="'.$setclass.'">'.$recertifyPrefix.'</div>',
          		'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'commonsave_'.$saveButtonId.'\', \'commonsave\',\'hidden_idlist_'.$uniqueId.'\',\'TP\','.$isPricedd.'); return false;" type="button" name="commonsave" value="'.t('LBL141').'">',
          );
          $saveAttributes['onclick'] = array();
          $saveAttributes['onclick'][] = 'clearMessages();';
          $form[$formName][$saveButtonId] = array(
      '#type' => 'submit',
     // '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="'.$setclass.'"></div>',
      '#prefix' => '<div style="display: none">',
		      '#suffix' => '</div><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div></div>'.$completionContainer.'<ul class="catalog-pub-add-list">',
      	'#value' => t('LBL141'),
      	'#name' => 'commonsave',
        '#id' => 'commonsave_'.$saveButtonId,
      	'#attributes' => $saveAttributes,
      		'#validate' => array('validateTPRosterEnrollmentDetails'),
          '#submit' => array('updateTPRosterEnrollmentDetails'),
          '#ajax' => array(
  	      'wrapper' => $wrapperId,
  	      'callback' => $ajaxCallbackSave,
          'entity_id' => $entityId, 
          'entity_type' => $entityType,
          // 'customButtonId'=>$esignHiddenButt,
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


  	        /*
  	         $form[$formName]['completionContainer'] = array(
  	         '#type' => 'textfield',
  	         '#prefix' => $completionContainerPrefix,
  	         '#suffix' => $completionContainerSuffix,
  	         '#name' => 'overall_completion_date_'.$uniqueId2,
  	         '#id' => 'overall_completion_date_'.$uniqueId2,
  	         '#title' => t('Completion Date:'),
  	         '#size' => 6,
  	         '#attributes' => array('disabled' => 'disabled'),
  	         );
  	         */
//  	        if(isSessionStartForTP($entityId) == 1){
  	        	$saveButtonId = 'catalog-enrolluser-complete-save';
  	        	$attributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages(); return false;';
  	        	$form[$formName][$saveButtonId] = array(
	      '#type' => 'markup',
	      '#prefix' => '<li class="save-pub-unpub-sub-menu completedandsave" onClick="$(\'#root-admin\').data(\'narrowsearch\').completionDateQTip(\''.$uniqueId2.'\'); return false;">',
	      '#suffix' => '</li>',
	      '#markup' => '<input id="completedandsave_trigger" class="form-submit ajax-processed" value="'.str_replace("&quot;","", htmlspecialchars(t('LBL695'),ENT_QUOTES,'UTF-8')).'"/>',
  	        	);
  	        	 
  	        	$qtipId = 'completedandsave_'.$uniqueId2;
  	        	$saveButtonId = 'catalog-enrolluser-complete-save-hidden';
  	        	$saveAttributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages();';
  	        	$form[$formName][$saveButtonId] = array(
	      '#type' => 'submit',
	      '#prefix' => '<div style="display:none;">',
	      '#suffix' => '</div>',
	      '#value' => t('LBL695'),
	      '#name' => 'completedandsave',
	      '#id' => $qtipId,
	      '#attributes' => $saveAttributes,
	      '#validate' => array('validateTPRosterEnrollmentDetails'),
	      '#submit' => array('updateTPRosterEnrollmentDetails'),
	      '#ajax' => array(
	          'wrapper' => $wrapperId,
	          'callback' => $ajaxCallbackSave,
  	        	// 'customButtonId'=>$esignHiddenButt,
	          'entity_id' => $entityId, 
	          'entity_type' => $entityType,
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
  	        	$form[$formName][$saveButtonId.'-markup'] = array(
  	        			'#type' => 'markup',
  	        			'#prefix' => '<li class="save-pub-unpub-sub-menu incompletedandsave">',
  	        			'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'incompletedandsave_'.$uniqueId2.'\', \'incompletedandsave\', \'hidden_idlist_'.$uniqueId.'\', \'TP\','.$isPricedd.'); return false;" type="button" name="incompletedandsave" value="'.t('LBL815').'">',
  	        	);
	          $qtipId = 'incompletedandsave_'.$uniqueId2;
	          $saveButtonId = 'catalog-enrolluser-incomplete-save-hidden';
	          $saveAttributes['onclick'] = array();
	          $saveAttributes['onclick'][] = 'clearMessages();';
	          $form[$formName][$saveButtonId] = array(
	      		'#type' => 'submit',
	      		'#prefix' => '<li class="save-pub-unpub-sub-menu incompletedandsave"><div style="display: none;">',
	      		'#suffix' => '</div></li>',
	      		'#value' => t('LBL815'),
	      		'#name' => 'incompletedandsave',
	      		'#id' => $qtipId,
	      		'#attributes' => $saveAttributes,
	      		'#submit' => array('updateTPRosterEnrollmentDetails'),
	      		'#ajax' => array(
	      				'wrapper' => $wrapperId,
	      				'callback' => $ajaxCallbackSave,
	      				'entity_id' => $entityId,
	      				'entity_type' => $entityType,
	      				// 'customButtonId'=>$esignHiddenButt,
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
  	//        }
			
  	        
  	        $qtipId = 'waivedandsave_'.$entityId.'tp';
  	        $saveButtonId = 'catalog-enrolluser-waived-save-hidden';
  	        $saveAttributes['onclick'] = array();
  	        $saveAttributes['onclick'][] = 'clearMessages();';
  	        $form[$formName][$saveButtonId] = array(
  	        		'#type' => 'submit',
  	        		'#prefix' => '<div style="display:none;">',
  	        		'#suffix' => '</div>',
  	        		'#value' => t('LBL695'),
  	        		'#name' => 'waivedandsave',
  	        		'#id' => $qtipId,
  	        		'#attributes' => $saveAttributes,
  	        		'#validate' => array('validateTPRosterEnrollmentDetails'),
  	        		'#submit' => array('updateTPRosterEnrollmentDetails'),
  	        		'#ajax' => array(
  	        				'wrapper' => $wrapperId,
  	        				'callback' => $ajaxCallbackSave,
  	        				'entity_id' => $entityId,
  	        				'entity_type' => $entityType,
  	        				// 'customButtonId'=>$esignHiddenButt,
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
  	  
  	        $mcCnt = getCountofMandatoryCompliance($entityId, 'tp');
  	        
  	        if($mcCnt){
  	        	$saveButtonId = 'catalog-enrolluser-waived-save';
  	        	$attributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages(); return false;';
  	        	$form[$formName][$saveButtonId] = array(
  	        			'#type' => 'markup',
  	        			'#prefix' => '<li class="save-pub-unpub-sub-menu waivedandsave" onClick="$(\'#visible-all-exempted-'.$entityId.'\').click(); return false;">',
  	        			'#suffix' => '</li>',
  	        			'#markup' => "<input id='waivedandsave_trigger' class='form-submit ajax-processed' value='".t('Set Waived and Save')."'/>",
  	        	);
  	        }
  	        
  	        $count = getClassExmeptedorNot('tp',$entityId);
  	        
  	        if($count > 0){
  	        	$saveButtonId = 'hidden-unwaived-id';
  	        	$form[$formName][$saveButtonId] = array(
  	        			'#type' => 'markup',
  	        			'#markup' => '<input type="hidden" value="" id="hidden-unwaived-id" />',
  	        	);
  	        	
  	        	
  	        	$saveButtonId = 'catalog-un-waived-save';
  	        	$attributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages(); return false;';
  	        	$form[$formName][$saveButtonId] = array(
  	        			'#type' => 'markup',
  	        			'#prefix' => '<li class="save-pub-unpub-sub-menu unwaivedandsave" onClick="$(\'#visible-all-exempted-'.$entityId.'\').click(); $(\'#hidden-unwaived-id\').val(\'unwaived\'); return false;">',
  	        			'#suffix' => '</li>',
  	        			'#markup' => "<input id='unwaivedandsave_trigger' class='form-submit ajax-processed' value='".t('Set UnWaived and Save')."'/>",
  	        	);  	        	
    	        		
  	        	$qtipId = 'unwaivedandsave_'.$entityId.'tp';
  	        	$saveButtonId = 'catalog-un-waived-save-hidden';
  	        	$saveAttributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages();';
  	        	$form[$formName][$saveButtonId] = array(
  	        			'#type' => 'submit',
  	        			'#prefix' => '<div style="display:none;">',
  	        			'#suffix' => '</div>',
  	        			'#value' => t('LBL695'),
  	        			'#name' => 'unwaivedandsave',
  	        			'#id' => $qtipId,
  	        			'#attributes' => $saveAttributes,
  	        			'#validate' => array('validateTPRosterEnrollmentDetails'),
  	        			'#submit' => array('updateTPRosterEnrollmentDetails'),
  	        			'#ajax' => array(
  	        					'wrapper' => $wrapperId,
  	        					'callback' => $ajaxCallbackSave,
  	        					'entity_id' => $entityId,
  	        					'entity_type' => $entityType,
  	        					// 'customButtonId'=>$esignHiddenButt,
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
  	        
  	        
  	        $saveButtonId = 'catalog-enrolluser-enroll-save';
  	        $saveUnPublishAttributes['onclick'] = array();
  	        $saveOtherAttributes['onclick'][] = 'clearMessages();';
  	        $form[$formName][$saveButtonId] = array(
      '#type' => 'submit',
      '#prefix' => '<li class="save-pub-unpub-sub-menu enrolledandsave">',
      '#suffix' => '</li>',
      '#value' => t('LBL696'),
      '#name' => 'enrolledandsave',
      '#attributes' => $saveOtherAttributes,
      '#submit' => array('updateTPRosterEnrollmentDetails'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'entity_id' => $entityId, 
          'entity_type' => $entityType,
  	        //'customButtonId'=>$esignHiddenButt,
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
  	        $form[$formName][$saveButtonId.'-markup'] = array(
  	        		'#type' => 'markup',
  	        		'#prefix' => '<li class="save-pub-unpub-sub-menu cancelledandsave">',
  	        		'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'cancelledandsave_'.$uniqueId2.'\', \'cancelledandsave\', \'hidden_idlist_'.$uniqueId.'\',\'TP\','.$isPricedd.'); return false;" type="button" name="cancelledandsave" value="'.t('LBL659').'">',
  	        );
  	        $qtipId = 'cancelledandsave_'.$uniqueId2;
          $saveButtonId = 'catalog-enrolluser-cancel-save';
          $saveAttributes['onclick'] = array();
          $saveAttributes['onclick'][] = 'clearMessages();';
          $form[$formName][$saveButtonId] = array(
      '#type' => 'submit',
  '#prefix' => '<div style="display: none">',
                '#suffix' => '</div></li>',
      '#value' => t('LBL659'),
      '#name' => 'cancelledandsave',
          '#id' => $qtipId,
       	'#attributes' => $attributes,
      '#submit' => array('updateTPRosterEnrollmentDetails'),
      '#ajax' => array(
          'wrapper' => $wrapperId,
          'callback' => $ajaxCallbackSave,
          'entity_id' => $entityId, 
          'entity_type' => $entityType,
          //  'customButtonId'=>$esignHiddenButt,
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
           $prgDetails = getProgramObjDetails($entityId);
          /**0081560 starts**/
          $isPricedTP =0;
          $select = db_select('system', 'sys');
          $select->addField('sys', 'status', 'status');
          $select->condition('name', 'exp_sp_commerce');
          $commerceEnabled =  $select->execute()->fetchField();
          if($commerceEnabled == 1){
          $isPricedTP = $prgDetails[0]->price;
          }
          /**0081560 ends**/
          expDebug::dPrint(' $m = '. print_r($isPricedTP, true) , 3);
          expDebug::dPrint(' $c = '. print_r($commerceEnabled, true) , 3);
          if($listqueryCount > 0 && $programValid[0]->sesavailable != 0 && $tpEnabled && $isPricedTP == 0){
          	$saveOtherAttributes['onclick'] = array();
          	$saveButtonId = 'catalog-enrolluser-newuser';
          	$form[$formName][$saveButtonId]['add_enrolluser_view_mode'] = array(
          '#type' => 'submit',
          '#title' => t('LBL361'),
          '#prefix' => '<li class="save-pub-unpub-sub-menu">',
          '#suffix' => '</li>',
          '#value' => t('LBL640'),
          '#name' => 'Add enrolltpuser',
          '#submit' => array('add_enrolltpuser'),
          '#attributes' => $saveOtherAttributes,
        	'#ajax' => array(
            'wrapper' => $wrapperId,
            'callback' => 'enrollTPUserGrid',//$ajaxCallbackSave,
          	//  'customButtonId'=>$esignHiddenButt,
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
           $module_count = db_query("select count(*) from slt_module where program_id = '.$entityId.'")->fetchField();
          if($listrecCount > 0 && $programValid[0]->sesavailable != 0 && $tpEnabled && $module_count>1){
         
			$saveButtonId = 'catalog-recertify-user';
  	        
  	        $saveAttributes['onclick'][] = 'clearMessages(); return false;';
  	        $form[$formName][$saveButtonId] = array(
		      '#type' => 'markup',
			      '#prefix' => '<li class="save-pub-unpub-sub-menu completedandsave" onClick="certificatePathQTip(\''.$entityId.'\',\'roster\'); return false;">',
		      '#suffix' => '</li>',
		      '#markup' => '<input id="completedandsave_trigger" class="form-submit ajax-processed" value="'.str_replace("&quot;","", htmlspecialchars(t('LBL429').' '.t('User'),ENT_QUOTES,'UTF-8')).'"/>',
  	        );
  	        $qtipId = 'recertifyandsave_'.$uniqueId2;	 
          	
          	$saveButtonId = 'catalog-recertify-newuser';
          	$saveAttributes['onclick'] = array();
          	$saveAttributes['onclick'][] = 'clearMessages();';
          	$form[$formName][$saveButtonId]['add_enrolluser_view_mode'] = array(
	          '#type' => 'submit',
          	  '#id' => $qtipId,
	          '#title' => t('Recertify User'),
	          '#prefix' => '<div style="display:none;">',
		      '#suffix' => '</div>',
	          '#value' => t('LBL429').' '.t('User'),
	          '#name' => 'Add_recertifyuser',
	          '#submit' => array('add_enrolltpuser'),
	          '#attributes' => $saveAttributes,
	        	'#ajax' => array(
	            'wrapper' => $wrapperId,
	            'callback' => 'enrollTPUserGrid',//$ajaxCallbackSave,
          	//  'customButtonId'=>$esignHiddenButt,
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
          $form[$formName]['addedit-button-group-ul-end'] = array(
        '#type' => 'markup',
        '#markup' => '</ul>',
          );
          /*
           $saveAttributes['onclick'] = array();
           $form[$formName][$esignHiddenButt] = array(
           '#type' => 'submit',
           '#prefix' => '<span id="esign-admin-div" style="display:none;">',
           '#suffix' => '</span>',
           '#value' => t('hidden-save'),
           '#name'	=> 'hidden-save',
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
  	        */
          $form[$formName]['addedit-left-col-end'] = array(
      '#type' => 'markup',
      '#markup' => '</div></div>',
          );

          $form[$formName]['load-scripts'.$uniqueId] = array(
          //'#prefix' => '<script language="Javascript" type="text/javascript">selecteDropdown()</script>',
          );
          /*
           $form[$formName][''] = array(
           '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="$(\'#root-admin\').data(\'narrowsearch\').completionDateQTip(\''.$qtipId.'\');" />',
           );
           */
	} catch (Exception $ex) {
		watchdog_exception('addEditSaveTPFormForEnrollUser', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getEnrolledTPUserAutoComplete() - Returns the matched person names enrolled to a tp
 */
function getEnrolledTPUserAutoComplete(){
	try{
		$nameSubstr = $_GET['z'];
		$entityId   = $_GET['entity_id'];
		$entityType = $_GET['entity_type'];
		$searchType = $_GET['search_type'];
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0){
		// Select tables
		$select = db_select('slt_master_enrollment','mse');
		$select->join('slt_person','per','per.id = mse.user_id');
			
		if($searchType == 'user'  || $searchType ==t('LBL107') || $searchType == ''){
	  // Add fields to fetch
			$select->addExpression('DISTINCT(per.user_name)', 'name');
		}else if($searchType == 'fullname') {
			$select->addExpression('DISTINCT(per.full_name)', 'name');
		}
		// Add conditions
		$select->condition('mse.program_id', $entityId, '=');
		if($searchType != 'status'){
			$select->condition('mse.overall_status', array('lrn_tpm_ovr_rsc'), 'NOT IN');
		}
		//Modified for the ticket no:18743
		if($searchType == 'user' || $searchType ==t('LBL107') || $searchType == ''){
			if(!empty($nameSubstr)){
				$select->condition('per.user_name', '%' . db_like($nameSubstr) . '%' , 'LIKE');
			}

			$select->orderBy('name');
		}else if($searchType == 'fullname'){
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%' , 'LIKE');
			$select->orderBy('name');
		}
		elseif($searchType == 'status'){
			if(mb_stripos(t('Enrolled'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Enrolled';
			} else if(mb_stripos(t('Completed'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Completed';
			} else if(mb_stripos(t('Waitlist'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Waitlist';
			} else if(mb_stripos(t('Canceled'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Canceled';
			} else if(mb_stripos(t('In progress'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'In progress';
			} else if(mb_stripos(t('Incomplete'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Incomplete';
			} else if(mb_stripos(t('Expired'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Expired';
			} else if(mb_stripos(t('Pending'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Pending';
			} else if(mb_stripos(t('Reserved'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Reserved';
			} else if(mb_stripos(t('Pending Approval'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Pending Approval';
			} else if(mb_stripos(t('Attended'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Attended';
			}

			$waivedcnt = 0;
			if(mb_stripos(t('Waived'),$nameSubstr,null,'UTF-8') !== false) {
				$qry = db_select('slt_master_enrollment','enr');
				$qry->innerJoin("slt_enrollment_exempted",'exmp',"exmp.enrollment_id = enr.id and exmp.enroll_type='tp'");
				$qry->addExpression("COUNT(1)",'waivedcnt');
				$qry->condition("exmp.exempted_status",1,'=');
				$qry->condition("enr.mandatory","","IS NULL");
				$qry->condition('enr.program_id', $entityId, '=');
				$qry->condition("enr.overall_status",array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'),'IN');
				$qry->groupBy('exmp.enrollment_id');
				expDebug::dPrintDBAPI("ADFSF AGT EQTQEQET",$qry);
				$waivedcnt = $qry->execute()->fetchField();
				expDebug::dPrint("DSAFDSATFGE".print_r($waivedcnt,1));
			}
			
			$select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = mse.overall_status');
			$select->addExpression('DISTINCT(spli.name)', 'name');
			$select->condition('spli.is_active', 'Y');
			$select->condition('spli.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->condition(db_or()->condition('spli.code', '%' . db_like('lrn_tpm_ovr_') . '%' ,'like'));
		}elseif($searchType == 'path'){ // Added by Vincent for #0072720
			$select->leftjoin('slt_module', 'module', 'module.program_id = mse.program_id AND module.sequence = mse.recertify_path');
			$select->addExpression('DISTINCT(module.title)', 'name');
			$select->condition('module.title', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->orderBy('module.title');
		}elseif($searchType == 'org'){
			$select->leftjoin('slt_organization', 'org', 'org.id = per.org_id');
			$select->addExpression('DISTINCT(org.name)', 'name');
			$select->condition('org.status', array('cre_org_sts_itv','cre_org_sts_act'), 'IN');
			$select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->orderBy('org.name');
		}
		elseif($searchType == 'jobrole'){
			$jobRoleCode = 'cre_usr_jrl_';
			$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
			$select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = spjm.job_role');
			$select->addExpression('DISTINCT(spli.name)', 'name');
			$select->condition('spli.is_active', 'Y');
			$select->condition('spli.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->condition('spli.code', db_like($jobRoleCode) . '%', 'LIKE');
			$select->orderBy('spli.name');
		}
		elseif($searchType == 'usertype'){
			$userTypeCode = 'cre_usr_ptp_';
			$select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = per.user_type');
			$select->addExpression('DISTINCT(spli.name)', 'name');
			$select->condition('spli.is_active', 'Y');
			$select->condition('spli.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->condition('spli.code',  db_like($userTypeCode) . '%', 'LIKE');
			$select->orderBy('spli.name');
		}
		elseif($searchType == 'manager'){
			$select->leftjoin('slt_person','pers1','pers1.id = per.manager_id');
			$select->condition(db_or()->condition('pers1.user_name', '%' . db_like($nameSubstr) . '%', 'LIKE')
			->condition('pers1.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE'));
			$select->addExpression('DISTINCT(pers1.full_name)', 'name');
			$select->orderBy('name');
		}
		elseif($searchType == 'group'){
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.entity_id = mse.program_id and grpmap.entity_type IN(\'cre_sys_obt_cur\',\'cre_sys_obt_crt\',\'cre_sys_obt_trn\')');
			$select->leftjoin('slt_groups', 'grps', 'grps.id = grpmap.group_id');
			$select->addExpression('DISTINCT(grps.name)', 'name');
			$select->condition('grps.is_admin',0,'=');
			$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
			//Added for #0056922
			$matchingQuery1 = $select->execute()->fetchAll();
			expDebug::dPrint('The $matchingQuery value is here '.print_r($matchingQuery1[0]->name,1),2);
			if($matchingQuery1[0]->name == ''){
				
				$select = db_select('slt_master_enrollment','mse');
				$select->join('slt_person','per','per.id = mse.user_id');
				$select->leftjoin('slt_groups', 'grps', '1=1');
				$select->addExpression('DISTINCT(grps.name)', 'name');
				$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
				$select->condition('grps.is_admin',0,'=');
			}
			$select->condition('grps.status','cre_sec_sts_atv','=');
			$select->orderBy('name');
			expDebug::dPrintDBAPI(' $select SQL = enrollment ' , $select);
		}
			

		expDebug::dPrintDBAPI(' $select SQL = ' , $select);
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per');
		$matchingPersons = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingPersons = '. print_r($matchingPersons, true) , 3);
		}
		else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			$solrObj = new UserEnrollments();
			try{
				$waivedcnt = 0;
				$matchingPersonsRes = $solrObj->TPEnrollments($entityId,$entityType,$nameSubstr,'autocomplete',$enrType='tp');
				$matchingPersonsResults = $matchingPersonsRes['records'];
				if($searchType == 'status'){
					if((mb_stripos(t('Waived'),$nameSubstr,null,'UTF-8') !== false) &&  ($searchType == 'status')){
						$waivedcnt = 1;
					}
				}
				$stsArray=array();
				foreach ($matchingPersonsResults as $result) {
				$groupNameArray = explode(",", $result->name);
        			foreach($groupNameArray as $GroupName){
        				$pos = strpos($GroupName, $nameSubstr);
        				if($pos !== false){
        					array_push($stsArray, str_replace('`','',$GroupName));
        				} 
        				
        			}
				}
				$stsArray = array_unique($stsArray);
				
				$matchingPersons = $stsArray;
			}catch(Exception $e){
				expDebug::dPrint("Error in tp enrollment autocomplete -- ".print_r($e->getMessage(),true),1);
			}
		}
		
		if($searchType == 'status' && $waivedcnt)
			print 'Waived'."\n";
		foreach ($matchingPersons as $person) {
			if($srcSource == 0){
				if($searchType == 'status'){
					print t($person->name) . "\n";
				}else {
					print $person->name . "\n";
				}
			} else {
				expDebug::dPrint("Error in tp enrollment autocomplete  666666--".$person,5);
				if($searchType == 'status'){
					if(empty($waivedcnt))
						print t($person) . "\n";
				}else {
					print $person . "\n";
				}
			}
		}
	}
	catch (Exception $ex) {
		watchdog_exception('getEnrolledTPUserAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getNotEnrolledTPUserAutoComplete() - Returns the matched person names that can be enrolled to a tp and are not already enrolled
 */
function getNotEnrolledTPUserAutoComplete($reg_flag) {
	try{
		$entityId   = $_GET['entity_id'];
		$entityType = $_GET['entity_type'];
		$nameSubstr = $_GET['z'];
		$searchType = $_GET['search_type'];
			
		$runQuery = 1;
		$srcSource = getConfigValue('solr_search');
		// Solr implementation
		if($srcSource == 0){
		if($searchType == 'user' || $searchType ==t('LBL107') || $searchType == ''){
			// Select the table
			$select = db_select('slt_person', 'per');

			// Add fields to fetch
			$select->addExpression('DISTINCT(per.user_name)', 'name');

			// Add condition
			//Modified for the ticket no:18743
			$select->condition('per.user_name', '%' . db_like($nameSubstr) . '%', 'LIKE');

			$select1 = db_select('slt_master_enrollment','enr');
			$select1->addField('enr','user_id');
			$select1->condition('enr.program_id',$entityId,'=');
			$select1->condition('enr.overall_status',array('lrn_tpm_ovr_inc','lrn_tpm_ovr_cln'),'NOT IN');
			$select1->where('enr.user_id = per.id');

			if($reg_flag == 'R'){
				$select->Exists($select1);
			}else{
				$select->notExists($select1);
			}


			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			$select->orderBy('name');
		}else if($searchType == 'fullname'){
			// Select the table
			$select = db_select('slt_person', 'per');

			// Add fields to fetch
			$select->addExpression('DISTINCT(per.full_name)', 'name');

			// Add condition
			//Modified for the ticket no:18743
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%', 'LIKE');

			if (!empty($excludedUserId)) {
				if($reg_flag == 'R'){
					$select->condition('per.id', $excludedUserId, 'IN');
				}else{
					$select->condition('per.id', $excludedUserId, 'NOT IN');
				}
			}

			$select->condition('per.full_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			$select->orderBy('name');
		}
		elseif($searchType == 'org'){
			$select = db_select('slt_organization', 'org');
			$select->leftJoin('slt_person','per','per.org_id = org.id');
			$select->addExpression('DISTINCT(org.name)', 'name');
			$select->condition('org.status', array('cre_org_sts_itv','cre_org_sts_act'), 'IN');
			$select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->orderBy('name');
		}
		elseif($searchType == 'jobrole'){
			$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_jrl_');
			foreach ($itemsList as $code => $name) {
				print $name . "\n";
			}
			$runQuery = 0;
		}
		elseif($searchType == 'usertype'){
			$itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_ptp_');
			foreach ($itemsList as $code => $name) {
				print $name . "\n";
			}
			$runQuery = 0;
		}
		elseif($searchType == 'manager'){
			$select = db_select('slt_person', 'per');
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition(db_or()->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like')
			->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like'));
			if (!empty($excludedUserId)) {
				if($reg_flag == 'R'){
					$select->condition('per.id', $excludedUserId, 'IN');
				}else{
					$select->condition('per.id', $excludedUserId, 'NOT IN');
				}
			}
			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
    		$select->condition('per.is_manager', 'Y');
			$select->orderBy('name');
		}
		elseif($searchType == 'group'){
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grps.id = grpmap.group_id');
			$select->addExpression('DISTINCT(grps.name)', 'name');
			$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
			$select->condition('grpmap.entity_type',array('cre_sys_obt_cur','cre_sys_obt_crt','cre_sys_obt_trn'),'IN');
			$select->condition('grpmap.entity_id',$entityId,'=');
			$select->condition('grps.is_admin',0,'=');
			$select->orderBy('name');
			$matchingQuery1 = $select->execute()->fetchAll();
			if($matchingQuery1[0]->name == ''){
				
				$select = db_select('slt_groups', 'grps');
				$select->addExpression('DISTINCT(grps.name)', 'name');
				$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
				$select->condition('grps.is_admin',0,'=');
				$select->orderBy('name');
				$matchingQuery2 = $select->execute()->fetchAll();
				foreach ($matchingQuery2 as $result) {
					print $result->name . "\n";
				}
			}
			else
			{
				foreach ($matchingQuery1 as $result) {
					print $result->name . "\n";
				}
			}
			$runQuery = 0;
		}
		// Changed by Vincent on 19 Jan, 2017. Fix for Organization, Usertype, Jobrole autocomplete are not working after Data Segmentaion.
		if($runQuery){
			if($searchType != 'group'){
				$userId = getIdOfLoggedInUser();
				//if($userId != 1)
				//	filterByGroupAccessPrivileges($select,$entityId,'TP');
			}
			
			expDebug::dPrintDBAPI(' $select SQL = ' , $select);

			$matchingQuery = $select->execute()->fetchAll();
			expDebug::dPrint(' $matchingPersons = '.print_r($matchingPersons, true) , 3);
			foreach ($matchingQuery as $result) {
				print $result->name . "\n";
			}
		}
	}
		else {
		    try{
		    	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
		        $resultvalues = array();
		        $solrObj = new UserSolrSearch();
		        $search_results = $solrObj->getSolrSearchResultEnrollUserAutoComplete($searchType,$nameSubstr,$entityId,$enrPage='tp');
		        $matchingQuery = $search_results['records'];
		        foreach($matchingQuery as $key => $val){
		        $groupNameArray = explode(",", $val -> name);
			        	foreach($groupNameArray as $GroupName){
			        		$pos = strpos($GroupName, $nameSubstr);
			        		if($pos !== false){
			        			array_push($resultvalues, str_replace('`','',$GroupName));
			        		}
			            
			        	}
		        }
		        $matchingQuery23 = array_unique($resultvalues);
		        $count = $search_results['recCount'];	
		    }catch(Exception $e){
		        expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
		    }
		    foreach ($matchingQuery23 as $result) {
		        print $result . "\n";
		    }
		}
	}catch (Exception $ex) {
		watchdog_exception('getNotEnrolledTPUserAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

function loadCtoolDataGridForEnrollTPUser($searchKeyword, $queryRequired = '', $entityId = '', $countRequired = '',$recertify_flag = '',$skipAccess=''){
	try {
		$searchType = $_GET['searhType'];
		$select = db_select('slt_person', 'per');
		if($countRequired){
			$select->addExpression('count(DISTINCT(per.id))', 'count');
		} else {
			$select->distinct();
			$select->addField('per', 'id', 'id');
			$select->addField('per', 'user_name', 'user_name');
			$select->addField('per', 'full_name', 'full_name');
			$select->addExpression('(select mgr.full_name from slt_person mgr where mgr.id = per.manager_id)', 'manager_name');
			$select->addExpression('(select name from slt_organization where id = per.org_id)', 'organization_name');
		}
		$select->condition('per.status', 'cre_usr_sts_atv');
		$select->condition('per.user_name', array('admin','guest'),'NOT IN');


		$searchTypeVal = t('LBL054');
		if($searchType == 'user'){
			$searchTypeVal = t('LBL181');
		}else if($searchType == 'fullname'){
			$searchTypeVal = t('LBL036') .' '.t('LBL107');
		}
		elseif($searchType == 'org'){
			$searchTypeVal = t('Organization');
		}
		elseif($searchType == 'jobrole'){
			$searchTypeVal = t('LBL133');
		}
		elseif($searchType == 'usertype'){
			$searchTypeVal = t('LBL173');
		}
		elseif($searchType == 'manager'){
			// $searchTypeVal = t('LBL180');
			$searchTypeVal = t('LBL134');
		}

		//if(!empty($searchKeyword) && $searchKeyword != t('LBL690') . ' ' . t('LBL644') . ' ' . t('LBL107')){
		if($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
			if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal && $searchKeyword != t('LBL690') ) {
				$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			}
		}elseif($searchType == 'fullname'){
			$select->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
		}
		elseif($searchType == 'org' && $searchKeyword != t('LBL036') .' '.t('Organization')){
			$select->leftjoin('slt_organization','org','org.id = per.org_id');
			$select->condition('org.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
		}
		elseif($searchType == 'jobrole' && $searchKeyword != t('LBL036') .' '.t('LBL133')){
			$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
			$select->leftjoin('slt_profile_list_items','splt','splt.code = spjm.job_role');
			$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$select->condition('splt.code',  db_like('cre_usr_jrl_') . '%', 'LIKE');
		}
		elseif($searchType == 'usertype' && $searchKeyword != t('LBL036') .' '.t('LBL173')){
			$select->leftjoin('slt_profile_list_items','splt','splt.code = per.user_type');
			$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$select->condition('splt.code',  db_like('cre_usr_ptp_') . '%', 'LIKE');
		}
		elseif($searchType == 'manager' && $searchKeyword != t('LBL036') .' '.t('LBL134')){
			$select->leftjoin('slt_person','pers1','pers1.id = per.manager_id');
			$select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
		}
		elseif($searchType == 'group' && $searchKeyword != t('LBL1270')){ // Type a user name
			$select->join('slt_groups', 'grp','grp.status=\'cre_sec_sts_atv\'');
			$select->leftjoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
			$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
			$select->join('slt_profile_list_items','pli','per.status=pli.code');
			$select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		   			AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		   			AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		   			AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		   			AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		   			AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
						AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role IS NULL OR FIND_IN_SET(ifnull(spjm.job_role,''),ifnull(grp.job_role,''))>0))
		   			AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		   			AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
		   			AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
			$select->condition('grp.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$select->condition('grp.is_admin',0,'=');
		}
		//filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
		expDebug::dPrint("asdsads".print_r(arg(),true),5);
		//filterByGroupAccessPrivileges($select,$entityId,'TP');		
		//Added by Vincent on 20 Jan, 2017 for #0071187
		//if($openUser !=1){
		//filterByGroupAccessPrivilegesUsingFlatTable($select,$entityId,'TP');
		//}else{
		if($skipAccess=='true'){
			$select->where('per.id not in (select gumap.user_id from slt_group_mapping map
			join slt_group_user_mapping gumap on gumap.group_id =map.group_id and map.group_type = gumap.group_type and gumap.user_type != \'R\'
			where map.group_type =0 and map.entity_id ='.$entityId.' and map.entity_type IN ( "cre_sys_obt_crt","cre_sys_obt_cur","cre_sys_obt_trn","cre_sys_obt_trp" )
			UNION ALL
			select map.user_id from slt_user_access_mapping map where map.entity_id ='.$entityId.' and map.entity_type IN ( "cre_sys_obt_crt","cre_sys_obt_cur","cre_sys_obt_trn","cre_sys_obt_trp" )
			) ');
		/* 	select user_id from slt_group_mapping map
			join slt_group_user_mapping gumap on gumap.group_id =map.group_id and map.group_type = gumap.group_type
			where map.group_type =0 and map.entity_id =244 and map.entity_type IN ( "cre_sys_obt_cls" ) */
		}
		$select1 = db_select('slt_master_enrollment','enr');
		$select1->addField('enr','user_id');
		$select1->condition('enr.program_id',$entityId,'=');
		$select1->condition('enr.overall_status',array('lrn_tpm_ovr_inc','lrn_tpm_ovr_cln','lrn_tpm_ovr_rsv','lrn_tpm_ovr_rsc'),'NOT IN');
		$select1->where('enr.user_id = per.id');
		if(strpos($recertify_flag ,'R')> 0){
			$certify_arr = explode("-",$recertify_flag);
			$select1->leftjoin('slt_module', 'module', 'module.sequence = enr.recertify_path');
			$select1->condition('module.id',$certify_arr[0],'=');
			$select1->condition('enr.is_current','Y','=');
			$select1->condition('enr.overall_status','lrn_tpm_ovr_exp','=');
			$select->Exists($select1);
		}else{
			$select->notExists($select1);
		}
		//filterByUserAccessPrivileges($select,'per','',1,'cre_usr','adminroster','per');
		//Added by Vincent on 20 Jan, 2017 for #0071187
		filterByUserAccessPrivilegesUsingFlatTable($select,'per','',1,'cre_usr','adminroster','per');
		if(empty($queryRequired)){
			if($countRequired){
				return $select->execute()->fetchField();
			} else {
				return $select->execute()->fetchAll();
			}
		} else {
			return $select;
		}
	} catch (Exception $ex) {
		watchdog_exception('loadCtoolDataGridForEnrollTPUser', $ex);
		expertusErrorThrow($ex);
	}
}

function fetchEnrollTPUserList($entityId, $entityType, $queryRequiredOrExportSearchType = '', $searchKeyword = '', $eliminateCancel = '',  $exportOption ='',$recertify_flag = '',$mode='', $isCountOnly = 0){
	try {
		if($entityType=='cre_sys_obt_trn' || $entityType=='cre_sys_obt_crt' || $entityType=='cre_sys_obt_cur'){
			if($exportOption == 'CSV') {
				// There seems a limit to number of page arguments that can be in hook_menu from where this function is called for export to csv.
				// Keeping code cosistent with that for class roster export to csv
				$searchType = $queryRequiredOrExportSearchType;
				$queryRequired = 'no';
			}
			else {
				$searchType = $_GET['searhType'];
				$queryRequired = $queryRequiredOrExportSearchType;
			}

			$select = db_select('slt_master_enrollment','mse');
			$select->join('slt_person','per','per.id = mse.user_id');
			$select->innerjoin('slt_program','prg','prg.id = mse.program_id');
			$select->innerjoin('slt_module','module','prg.id = module.program_id and module.sequence = mse.recertify_path');
			if($isCountOnly==0){
				$select->addField('mse','id','id');
				$select->addField('mse','user_id','registered_user_id');
				$select->addField('mse','overall_status','overall_status');
				$select->addField('mse','reg_date','reg_date');
				$select->addField('mse','cancel_date','cancel_date');
				$select->addField('mse','comp_date','comp_date');
				$select->addField('mse','updated_on','updated_on');
				$select->addExpression('(select name from slt_profile_list_items where code = mse.overall_status)','overall_status_name');
				$select->addField('per','user_name','user_name');
				$select->addField('per','full_name','full_name');
				$select->addField('mse','mandatory');
				$select->addField('mse','recertify_path','certifypath');
				$select->addField('mse','program_id','program_id');
				$select->addField('module','title','module_title');
				$select->addExpression("if(mse.overall_status = 'lrn_tpm_ovr_enr', mse.reg_date,
 											if(mse.overall_status = 'lrn_tpm_ovr_cln',mse.cancel_date,
 											if(mse.overall_status = 'lrn_tpm_ovr_cmp',mse.comp_date,mse.updated_on)))",'date');
				$select->addExpression('(SELECT CONCAT(exmp.exempted_status,\'|\',exmp.id) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = mse.id AND exmp.enroll_type = \'tp\' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
				$select->addExpression('IFNULL(if(mse.overall_status NOT IN (\'lrn_tpm_ovr_enr\',\'lrn_tpm_ovr_inp\'),(select name from slt_profile_list_items where code = mse.overall_status),(SELECT if(exmp.exempted_status,\'Waived\',NULL) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = mse.id AND exmp.enroll_type = \'tp\' ORDER BY id DESC LIMIT 1)),(select name from slt_profile_list_items where code = mse.overall_status))', 'exempted_overallstatus');
			}else{
				$select->addExpression('count(1)','cnt');
			}
			$select->condition('mse.overall_status', array('lrn_tpm_ovr_rsc'), 'NOT IN');
			$select->condition('mse.is_current','Y','=');
			if(($eliminateCancel) && $recertify_flag == ''){
				$select->condition('mse.overall_status', 'lrn_tpm_ovr_cln', '!=');
			}
			if($recertify_flag){
				$select->condition('mse.overall_status', 'lrn_tpm_ovr_exp', '=');
			}
			// Below condition modified by Vincent on 10 Dec, 2013 for #0026273
			if($mode=='edit'){
				$select->condition('mse.overall_status', 'lrn_tpm_ovr_inc', '!=');
			}
			$select->condition('mse.program_id',$entityId,'=');

			//$select->condition('mse.overall_status',array('lrn_tpm_ovr_enr','lrn_tpm_ovr_wtl'),'IN');
			//if(!empty($searchKeyword) && $searchKeyword != t('LBL690') . ' ' . t('LBL644') . ' ' . t('LBL107')) {

			$searchTypeVal = t('LBL054');
			if($searchType == 'user'){
				$searchTypeVal = t('LBL054');
			}else if($searchType == 'fullname'){
				$searchTypeVal = t('LBL036') .' '.t('LBL107');
			}
			elseif($searchType == 'org'){
				$searchTypeVal = t('Organization');
			}
			elseif($searchType == 'jobrole'){
				$searchTypeVal = t('LBL133');
			}
			elseif($searchType == 'usertype'){
				$searchTypeVal = t('LBL173');
			}
			elseif($searchType == 'manager'){
				//$searchTypeVal = t('LBL180');
				$searchTypeVal = t('LBL036') . ' ' .t('LBL134');
			}
			elseif($searchType == 'status'){
				$searchTypeVal = t('LBL102');
			}elseif($searchType == 'path'){ // Added by Vincent for #0072720
				$searchTypeVal = t('LBL3060');
			}

			if($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == t('LBL036') . ' '.$searchTypeVal){
				if(!empty($searchKeyword) && $searchKeyword != t('LBL036') . ' '.$searchTypeVal && stripslashes($searchKeyword) != t('LBL181') ){
					$select->condition(db_or()->condition('per.full_name', '%' . db_like($searchKeyword) . '%' , 'LIKE')
					->condition('per.user_name', '%' . db_like($searchKeyword) . '%' , 'LIKE'));
					$filterArray[] = array(t('LBL054').':'.$searchKeyword);
				}
			}else if($searchType == 'fullname' && $searchKeyword != t('LBL036') .' '.t('LBL107')){
				$select->condition('per.full_name', '%' . db_like($searchKeyword) . '%' , 'LIKE');
				$filterArray[] = array(t('LBL107').':'.$searchKeyword);
			}
			elseif ($searchType == 'status') {
				$waivedCnt = 0;
				$allStatus = getProfileItemNamesKeyed('lrn_tpm_ovr_');
				expDebug::dPrint('$allStatus = ' . print_r($allStatus, true), 4);
				$searchStatusList = array();
				if(mb_stripos(t('Waived'),$searchKeyword,null,'UTF-8') !== false) {
					$waivedCnt = 1;
				}else{
					foreach ($allStatus as $code => $name) {
						if (mb_stripos($name, $searchKeyword, null, 'UTF-8') !== false) {
							$searchStatusList[] = $code;
						}
					}
				}
				
				expDebug::dPrint('$searchStatusList = ' . print_r($searchStatusList, true), 4);
				if(empty($waivedCnt)){
					if (empty($searchStatusList)) {
						$select->where('1=0');
					}
					else {
						$select->condition('mse.overall_status', $searchStatusList, 'IN');
					}
				}else{
					$select->where('if((SELECT exm.exempted_status as exmsts from slt_enrollment_exempted exm
							where exm.enrollment_id = mse.id
							AND mse.overall_status IN (\'lrn_tpm_ovr_inp\',\'lrn_tpm_ovr_enr\') AND
							exm.enroll_type = \'tp\' order by exm.id desc limit 1) = 1 ,1=1,1=0)');
				}
				

				$filterArray[] = array(t('LBL102').':'.$searchKeyword);
			}elseif($searchType == 'path'){ // Added by Vincent for #0072720
				$select->condition('module.title','%' . db_like($searchKeyword) . '%','LIKE');
			}
			elseif($searchType == 'org'){
				$select->leftjoin('slt_organization','org','org.id = per.org_id');
				$select->condition('org.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$filterArray[] = array(t('Organization').':'.$searchKeyword);
			}
			elseif($searchType == 'jobrole'){
				$jobRoleCode = 'cre_usr_jrl_';
				$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
				$select->leftjoin('slt_profile_list_items','splt','splt.code = spjm.job_role');
				$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$select->condition('splt.code', db_like($jobRoleCode) . '%', 'LIKE');
				$filterArray[] = array(t('LBL133').':'.$searchKeyword);
			}
			elseif($searchType == 'usertype'){
				$select->leftjoin('slt_profile_list_items','splt','splt.code = per.user_type');
				$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$select->condition('splt.code',  db_like('cre_usr_ptp_') . '%', 'LIKE');
				$filterArray[] = array(t('LBL173').':'.$searchKeyword);
			}
			elseif($searchType == 'manager' && $searchKeyword != t('LBL036') . ' ' .t('LBL134')){
				$select->leftjoin('slt_person','pers1','pers1.id = per.manager_id');
				$select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')
				->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
				$filterArray[] = array(t('Manager').':'.$searchKeyword);
			}
			elseif($searchType == 'group' && $searchKeyword != t('LBL1270')){ // Type a user name
				$select->join('slt_groups', 'grp');
				$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
				$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
				$select->join('slt_profile_list_items','pli','per.status=pli.code');
				$select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		   			AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		   			AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		   			AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		   			AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		   			AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
						AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id=per.id)>0,grp.job_role IS NULL OR FIND_IN_SET(ifnull(spjm.job_role,''),ifnull(grp.job_role,''))>0))
		   			AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		   			AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N') 
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y') 
                                                    THEN 
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))	
		   			AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
				$select->condition('grp.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$select->condition('grp.is_admin',0,'=');
			}
			expDebug::dPrintDBAPI('$QUWER U for tp enrolllment = ' ,$select);
			if($_REQUEST['apiname'] == "ListTPRosterAPI")
			{
				if(!empty($_REQUEST['date_from'])) {
					$date_from = trim($_REQUEST['date_from']);
					$from_timestamp = strtotime($date_from);
					$from_format = date("Y-m-d H:i:s", $from_timestamp);
				}
				if(!empty($_REQUEST['date_to'])) {
					$date_to = trim($_REQUEST['date_to']);
					$to_timestamp = strtotime($date_to);
					$to_format = date("Y-m-d 23:59:59", $to_timestamp);
				}
				if (($date_from == "") && (!empty($date_to))) {
				  $select->condition('mse.reg_date',$to_format,'<=');
				}
				else if(($date_to==  "") && (!empty($date_from))) {
				  $select->condition('mse.reg_date',$from_format,'>=' );
				}
				else if(!empty($date_from) && !empty($date_to)) {
				 	if($from_timestamp <= $to_timestamp) {
				    	$select->condition('mse.reg_date',array($from_format,$to_format),'BETWEEN' );
					}
					else {
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->field = "date_to";
						$errobj->errormsg = 'To date should be greater than or equal to from date';
						return $errobj;
					}
				}
			}
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','adminroster','per');
			
			if(empty($queryRequired)){
				return $select->execute()->fetchAll();
			} else {
				if($exportOption=='CSV'){
					$colHeadersMap = array(t('LBL054') => 'user_name',
					t('LBL107') => 'full_name',
					t('LBL3060') => 'module_title',
					t('LBL102') => 'exempted_overallstatus'
					);

					SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap,'export', $filterArray, '', 'tp_roster');
				} else {
					return $select;
				}
			}
		}
	} catch (Exception $ex) {
		watchdog_exception('fetchEnrollTPUserList', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Custom hook_csv_column_headers_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function tp_roster_csv_column_headers_alter($columnHeaderMap) {
	try {
		$columnHeaderMap[0][t('LBL042')] = 'datedefault';
	}
	catch (Exception $ex) {
		watchdog_exception('tp_roster_csv_column_headers_alter', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function tp_roster_csv_row_data_alter($rowData) {
	try {
		if ($rowData[0]->overall_status == 'lrn_tpm_ovr_cmp' || $rowData[0]->overall_status =='lrn_tpm_ovr_inc' ||
		$rowData[0]->overall_status =='lrn_tpm_ovr_exp') {
			if(($rowData[0]->comp_date != NULL) || ($rowData[0]->comp_date != ''))
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->comp_date));
			else
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->date));
		}
		else if ($rowData[0]->overall_status == 'lrn_tpm_ovr_cln') {
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->cancel_date));
		}
		else {
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->reg_date));
		}
	}
	catch (Exception $ex) {
		watchdog_exception('tp_roster_csv_row_data_alter', $ex);
		expertusErrorThrow($ex);
	}
}

function cancelEnrollTPUserDetails($form, &$form_state){
	try {
		// Clear enrolltpuser-autocomplete form textfield as switching from enroll user view to roster view - fix for ticket #0022120
		clearTPRosterEnrollUserAutocomplete($form_state);

		$form_state['storage']['mode'] = '';
		$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('cancelEnrollTPUserDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function search_enrolltpuser () {
	try {

	} catch (Exception $ex) {
		watchdog_exception('search_enrolltpuser', $ex);
		expertusErrorThrow($ex);
	}

}
function enrollTPUserGrid($form, &$form_state) {
	try {
		$formName = $form_state['form_unique_name'];
		return $form[$formName];
	} catch (Exception $ex) {
		watchdog_exception('enrollTPUserGrid', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollTPUserGridCallback($form, &$form_state) {
	try {
		$formName = $form_state['form_unique_name'];
		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
		$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
		$uniqueId = "enrolltpuser-$entityId-$entityType";
		unset($_SESSION['exempted-tp-'.$entityId]);
		$commands = array();
		if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])) {
			$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
		}

		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true),4);
		if (!empty($statusMsgHTML)) {
			// Show the messages
			$commands[] = ajax_command_html('#show_expertus_message', $statusMsgHTML);
			// Set the trigger to remove the messages after a while
			if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
				expDebug::dPrint(' ajax cmd addedit_hide_messages() called for messages within ' . $form_state['triggering_element']['#ajax']['wrapper'],5);
				$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
			}
			else {
				expDebug::dPrint(' ajax cmd addedit_hide_messages() called for all messages in doc');
				$commands[] = addedit_hide_messages();
			}
		}
		$commands[] = exp_ctools_modal_adjust();
		//63622: Training plan doesn't moves to completed tab --start
		$min_form_state = array(
				'ajax' => TRUE,
				're_render' => TRUE,
				'no_redirect' => TRUE,
				'program_id' => $entityId,
				'build_info' => array('args' => $form_state['build_info']['args'])
		);
		
		$formId = 'exp_sp_administration_program_attaching_courses_form';
		$builtForm = drupal_build_form($formId, $min_form_state);
		$commands[] = ajax_command_replace('#attach_course_dt', drupal_render($builtForm));
		$commands[] = ajax_command_invoke(NULL, "attachClose", array($entityId, $entityType));
		//63622: Training plan doesn't moves to completed tab --end
		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('enrollTPUserGridCallback', $ex);
		expertusErrorThrow($ex);
	}
}


function add_enrolltpuser($form, &$form_state) {
	try {
		// Clear enrolltpuser-autocomplete form textfield as switching from roster view to enroll user view - fix for ticket #0022120
		clearTPRosterEnrollUserAutocomplete($form_state);

		$form_state['storage']['mode'] = 'edit';
		$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('add_enrolltpuser', $ex);
		expertusErrorThrow($ex);
	}
}

function validateTPRosterEnrollmentDetails ($form, &$form_state, $callFromAPI = 0) {
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program_roster.inc');
		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');

		$submitTypeArray = array(
				'cancelledandsave' => 'Canceled',
				'enrolledandsave' => 'Enrolled',
				'completedandsave' => 'Completed',
				'recertifyandsave' => 'recertify',
		);

		$submitType = $form_state['triggering_element']['#name'];

		$entityId = $form_state['input']['mapped_program_id'];
		$entityType = $form_state['input']['mapped_program_type'];
		$date = now();
		$completionDate = '';
		$selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolltpuser-'.$entityId.'-'.$entityType];
		if(empty($selectedEnrolledIds)  && empty($callFromAPI)){
			form_set_error('',t('ERR106'));
			$form_state['esign_show_popup']= false;
			return '';
		}
		expDebug::dPrint('$selectedEnrolledIds values = '.print_r($selectedEnrolledIds,true),5);
		$selectedIdsArray = explode(',', $selectedEnrolledIds);
		$overallCompletionDate = $form_state['input']['hidden_completion_date_enrolltpuser-'.$entityId.'-'.$entityType];

		$rosterDetails = array();
		$overallDetails = $form_state['input']['hidden_valuelist_enrolltpuser-'.$entityId.'-'.$entityType];
		$overallDetailsArray = explode(',', $overallDetails);
		foreach($overallDetailsArray as $overallValue){
			list($tmpEnrollId, $tmpCompletionDate) = explode('##', $overallValue);
			$rosterDetails[$tmpEnrollId] = array( 'completion_date' => $tmpCompletionDate );
		}

		$pricedErr = 0;
		$prgDetails = getProgramObjDetails($entityId);
		$isPricedTP = $prgDetails[0]->price;
		/*-- #36602 - Issue fix --*/
		$isCommerceEnabled =  module_exists('exp_sp_commerce');

		foreach($selectedIdsArray as $enrollId){			
			$completionDate = $form_state['input']['completion_sel_date_'.$enrollId];
			 
			$status = $form_state['input']['enrolled_status_'.$enrollId];
			if($status == t('Enrolled')){
				$pricedErr = 1;
			}
			// Added by velu for #0023621
			if( ($isPricedTP > 0 && $isCommerceEnabled) && ($submitType=='commonsave' || $submitType=='cancelledandsave' || $pricedErr) && $form_state['input']['enrolled_status_'.$enrollId] == t('Canceled') ){
				form_set_error('',t('LBL1246'));
			}
			
			$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollId,'tp');
			if($submitType == 'commonsave' || $submitType == 'waivedandsave'){
				expDebug::dPrint("Waived status check".print_r($_SESSION['exempted-tp-'.$entityId][0],1)."---status".$status);
				if($submitType == 'commonsave' && ($status == 'Waived' && (!checkWaivedReasonEmpty($entityId))) 
						|| ($previous_exempted_status[0]->exempted_status == 1 && ($status == 'Enrolled' || $status == t('In progress') || $status == 'In progress'))) {
					expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
					if(empty($_SESSION['exempted-tp-'.$entityId][$enrollId])){
						expDebug::dPrint('check for waived enrolledIds'.print_r($entityId,1),4);
						form_set_error('', t('waived reason should not be empty'));
						return '';
					}
				}
				if($submitType == 'waivedandsave' && $status != t('Waived') && $status != t('Enrolled') && $status != t('In progress')){
					expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
					form_set_error('', t('Cannot mark Waived status'));
					return '';
				}
				if($submitType == 'waivedandsave' && empty($_SESSION['exempted-tp-'.$entityId][0])){
					expDebug::dPrint('check for waived enrolledIds sss'.print_r($entityId,1),4);
					form_set_error('', t('waived reason should not be empty'));
					return '';
				} else if($submitType == 'waivedandsave'){
					$mandorComp = checkComplianceAndMandatory($enrollId,'tp');
					expDebug::dPrint("return value from funviton".print_r($mandorComp,1));
					if($mandorComp[0]->mandatory != 1){
						form_set_error('', t('Enrollments should be mandatory or compliance to mark as Waived.'));
						return '';
					}
				}
			}
			
			
			 
			if($submitType == 'commonsave'){
				if(empty($status)){
					$status = $rosterDetails[$enrollId]['status'];
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
					$score = $rosterDetails[$enrollId]['score'];
				}
			} else {
				$status = $submitTypeArray[$submitType];
				if(empty($completionDate) && $callFromAPI == 0){
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
					$score = empty($score) ? $rosterDetails[$enrollId]['score'] : $score;
				}
			}

			if(strtolower($status) == 'completed'){
				$completionDate = empty($overallCompletionDate) ? $completionDate : $overallCompletionDate;

				if(!empty($completionDate)){
					$completionDateArray = explode('/', $completionDate);
					$completionDate = $completionDateArray[0].'/'.$completionDateArray[1].'/'.$completionDateArray[2];
				}
				if($completionDateArray[0] > 12){
					form_set_error('completion_sel_date_'.$enrollId , t('ERR242'));
				}
			}

		}

	}catch (Exception $ex) {
		watchdog_exception('validateTPRosterEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function updateTPRosterEnrollmentDetails($form, &$form_state, $callFromAPI = 0){
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_program') .'/exp_sp_administration_program_roster.inc');

		$submitTypeArray = array(
				'cancelledandsave' 	 => 'Canceled',
				'enrolledandsave'  	 => 'Enrolled',
				'completedandsave' 	 => 'Completed',
				'recertifyandsave' 	 => 'recertify',
				'incompletedandsave' => 'Incomplete',
		);

		$submitType = $form_state['triggering_element']['#name'];
		//getMasterEnrollmentDetails($enrollmentid, $fieldList = array())
		$entityId = $form_state['input']['mapped_program_id'];
		$entityType = $form_state['input']['mapped_program_type'];

		$selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolltpuser-'.$entityId.'-'.$entityType];		
		// completed and save error message is setting
		if(empty($selectedEnrolledIds) && ($submitType =='completedandsave' || $submitType == 'commonsave' || $submitType == 'unwaivedandsave') && empty($callFromAPI)){
			form_set_error('',t('ERR106'));
			$form_state['esign_show_popup']= false;
			return '';
		}
		$date = now();
		$grade = '';
		$score = '';
		$contentValidFrom  = '';
		$contentValidTo = '';
		$registrationStatusDate = '';
		$completionDate = '';

		$code = array(
					'Enrolled' 	 => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_enr' ),
					'Canceled'   => array( 'lrn_crs_reg_can', '' ),
					'Completed'  => array( '', 'lrn_crs_cmp_cmp' ),
					'Incomplete' => array( '', 'lrn_crs_cmp_inc' ),
				);

		if($callFromAPI){
			$selectedEnrolledIds = $form_state['MasterEnrollId'];
		}
		$selectedIdsArray = explode(',', $selectedEnrolledIds);
		$rosterResponse = '';
		$overallCompletionDate = $form_state['input']['hidden_completion_date_enrolltpuser-'.$entityId.'-'.$entityType];

		$rosterDetails = array();
		$overallDetails = $form_state['input']['hidden_valuelist_enrolltpuser-'.$entityId.'-'.$entityType];
		$overallDetailsArray = explode(',', $overallDetails);
		foreach($overallDetailsArray as $overallValue){
			list($tmpEnrollId, $tmpStatus, $tmpCompletionDate, $tmpCompletionScore) = explode('##', $overallValue);
			$rosterDetails[$tmpEnrollId] = array('status' => $tmpStatus,
					'completion_date' => $tmpCompletionDate,
					'score' => $tmpCompletionScore);
		}
		$rosterResponseApi = array();
		$tpErrors = array();
		$tpStr = '';
		foreach($selectedIdsArray as $enrollId){
			
			$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollId,'tp',$entityId);
			 
			if((($submitType =='enrolledandsave' || $submitType =='incompletedandsave') && $form_state['input']['enrolled_status_'.$enrollId] == 'Waived')
					|| ($submitType =='enrolledandsave' && strtolower($form_state['input']['enrolled_status_'.$enrollId]) == 'enrolled' && $previous_exempted_status[0]->exempted_status == 1)){
				form_set_error('',t('Cannot mark waived status'));
				$form_state['esign_show_popup']= false;
				return '';
			}
			
			$tpEnrollDetail 	= getMasterEnrollmentDetails($enrollmentid, array('id','overall_status','user_id'));
			$code = array(
					'Enrolled' 	 => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_enr' ),
					'Canceled'   => array( 'lrn_crs_reg_can', '' ),
					'Completed'  => array( '', 'lrn_crs_cmp_cmp' ),
					'Incomplete' => array( '', 'lrn_crs_cmp_inc' ),
					'Waived' 	 => array( 'lrn_crs_reg_cnf', $tpEnrollDetail->overall_status ),
					'UnWaived' => array('lrn_crs_reg_cnf', 'lrn_crs_cmp_enr')
				);

			$score          = $form_state['input']['score_'.$enrollId];
			$completionDate = $form_state['input']['completion_sel_date_'.$enrollId];
			
			if($submitType == 'commonsave'){
				$status = $form_state['input']['enrolled_status_'.$enrollId];
				if(empty($status)){
					$status = $rosterDetails[$enrollId]['status'];
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
				}
			}else if($submitType == 'waivedandsave'){
				$status = $form_state['input']['enrolled_status_'.$enrollId];
				expDebug::dPrint("Value from update roster as status---->>>>>".print_r($status,1));
			}else if($submitType == 'unwaivedandsave'){
                $status = $form_state['input']['enrolled_status_'.$enrollId];
                expDebug::dPrint("Value from update roster as status---->>>>>".print_r($status,1));
            }
			 else {
				$status = $submitTypeArray[$submitType];
				if(empty($completionDate) && $callFromAPI ==0){
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
				}
				if($callFromAPI){
					$status = $form_state['action'];
				}
			}
			$registrationStatusDate = $date;
			if($callFromAPI){
				$completionDate = str_replace('-', '/', $form_state['CompletionDate']);
				$registrationStatusDate = $form_state['RegStatusDate'];
			}


			if(strtolower($status) == 'completed'){
				$completionDate = empty($overallCompletionDate) ? $completionDate : $overallCompletionDate;
				//date('Y-m-d H:i',strtotime($registrationStatusDate)
				if(!empty($completionDate)){
					$completionDateArray = explode('/', $completionDate);
					$completionDate = $completionDateArray[0].'/'.$completionDateArray[1].'/'.$completionDateArray[2];
				}
			}			
			if(((strtolower($status) == 'enrolled' || $submitType == 'waivedandsave'  || $submitType == 'unwaivedandsave') && ($previous_exempted_status[0]->exempted_status == 1))){
				$_SESSION['enrollment_id_mark_complete_revert_'.enrollId]= $enrollId;
				revertExemptedStatus($previous_exempted_status,$enrollId,'tp',$entityId,$submitType,'');
			}else if((strtolower($status) == 'waived' || $submitType == 'waivedandsave') && empty($previous_exempted_status[0]->exempted_status)){
				$_SESSION['enrollment_id_mark_complete_'.$enrollId] = $enrollId;
				enrollmentExemptedDetailsUpdate($enrollId,'tp',$entityId,$submitType,'');
			}
			
			$tpUsrName 	= getTPEnrollmentPersonName($enrollId);
			// for 43061: Through simulator update Roster API, should not allow if the completion date is less than the enrollment date.
			$enrollDetail 	= getMasterEnrollmentDetails($enrollId, array('reg_date'));
			$dateComp		= date_format(date_create($completionDate), 'y-m-d');
			$dateRegStatus	= date_format(date_create($registrationStatusDate), 'y-m-d');
			$dateReg		= date_format(date_create($enrollDetail->reg_date), 'y-m-d');
			$flag = 0;
			
			if($dateComp < $dateReg && $submitType != 'waivedandsave') {
				//Completion Date must be greater than the Registration Date
				$tpErrors[$tpUsrName] = t('LBL706').' '.t('ERR087').' '.t('LBL1067').' '.t('LBL042');
				$form_state['esign_show_popup'] = false;
    			$flag = 1;
			}
			if($dateRegStatus < $dateReg && $submitType != 'waivedandsave') {
				
				//Registration Status Date must be greater than the Registration Date
				$tpErrors[$tpUsrName] = t('LBL721').' '.t('LBL042').' '.t('ERR087').' '.t('LBL1067').' '.t('LBL042');
				$form_state['esign_show_popup'] = false;
    			$flag = 1;
			}
			$registrationStatus = $code[$status][0];
			$completionStatus = $code[$status][1];
			
			
			$paramArray = array();
			$paramArray[] = $entityId;
			$paramArray[] = $entityType;
			$paramArray[] = $enrollId;
			$paramArray[] = $registrationStatus;
			$paramArray[] = $registrationStatusDate;
			$paramArray[] = $completionStatus;
			$paramArray[] = $completionDate = empty($completionDate) ? $date : $completionDate;
			$paramArray[] = $grade;
			$paramArray[] = $score;
			$paramArray[] = $contentValidFrom;
			$paramArray[] = $contentValidTo;

			$paramList = implode('$$$', $paramArray);
			if($flag == 0) {
			  if($callFromAPI){
				$rosterResponseApi[] = updateProgramRoster($paramList, 1, $callFromAPI);
			  }
			  else{
				$rosterResponse .= updateProgramRoster($paramList, 1, $callFromAPI);
			  }
			}

			// list($programId, $entityType, $selectedEnrolledIds, $registrationStatus, $registrationStatusDate, $completionStatus, $completionDate, $grade, $score, $contentValidFrom, $contentValidTo) = explode('$$$', $params);
		}
		if(count($tpErrors) > 0) {
			$combinedTP = array();
			foreach($tpErrors as $key=>$val) {
				$combinedTP[$val][] = $key;
			}
			foreach($combinedTP as $key_tp_combined=>$val_tp_combined) {
				$tpStr_key = $key_tp_combined;
				if(is_array($val_tp_combined)) {
					foreach($val_tp_combined as $tp) {
						$tpStr .= $tp. ", ";
					}
				}
			}
			$tpStr = rtrim($tpStr,", ");
			if($callFromAPI){
				form_set_error('', t($tpStr_key));
			}
			else {
				$error_msg = $tpStr_key. "=".$tpStr;
				form_set_error('', t($error_msg));
			}
		}
		if($callFromAPI){
			return $rosterResponseApi;
		}
		if(!empty($rosterResponse) && !empty($selectedEnrolledIds)){
			$message = '<div><div class="registered-msg-enrolluser"><table>'.$rosterResponse.'</table></div></div>';
			drupal_set_message($message, 'error');
			$form_state['storage']['mode'] = 'view';
			$form_state['rebuild'] = TRUE;
		}

		expDebug::dPrint(' $submitType = ' . print_r($submitType, true) , 5);
		expDebug::dPrint(' $code = ' . print_r($code, true) , 4);
		expDebug::dPrint(' $$paramArray = ' . print_r($paramArray, true) ,4);
		expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
		expDebug::dPrint(' $rosterResponse = ' . $rosterResponse , 4);

		// Clear enrolltpuser-autocomplete form textfield as roster enrollment details are updated - fix for ticket #0022120
		clearTPRosterEnrollUserAutocomplete($form_state);
		
		// To sync solr data
		syncSolrData('MasterEnrollment');
	} catch (Exception $ex) {
		watchdog_exception('updateTPRosterEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getTPEnrollmentPersonName($eid) {
	try {
		$sql = "select p.user_name from slt_master_enrollment e left join slt_person p on e.user_id=p.id where e.id=$eid";
		$qry = db_query($sql);
		$res = $qry->fetch();
		return $res->user_name;
	} catch (Exception $ex) {
		watchdog_exception('getTPEnrollmentPersonName', $ex);
		expertusErrorThrow($ex);
	}
}
function addNewEnrollTPUserDetails($form, &$form_state, $entityId, $entityType) {
	global $secure_entities;
	//  include_once(drupal_get_path('module', 'exp_sp_learning') .'/exp_sp_learning.inc');
	//$txn = db_transaction();
	$recertify_flag = ($form_state['triggering_element']['#title'] == 'recertify') ? 'R' : '';
	try {
		//$entityId = $form_state['triggering_element']['#ajax']['entity_id'];  //classId
		//$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
		if (in_array($entityType, $secure_entities)) {
			$uniqueId = "enrolltpuser-".core_encrypt($entityId)."-$entityType"; // URL encryption to retirve data
		} else {
			$uniqueId = "enrolltpuser-".$entityId."-$entityType"; // URL encryption to retirve data
		}

		if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId])){
			if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])){
				$userIdList = $form_state['input']['hidden_idlist_'.$uniqueId];
				$associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
			}else{
				$userIdList = $form_state['hidden_idlist_'.$uniqueId];
				$associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
			}

			$associateIdListArray = array_unique($associateIdListArray);

			$userIdList = implode(',', $associateIdListArray);

			$waitlist = 1;
			$isAdminSide = 'Y';
			$regDate = '';
			$page = 'newadmin-N';

			/*
			 * 0024925: Enforce equivalence in training plans
			 * for this ticket we created equivalence option for training plan
			 * don't delete it. Its may need for feature enhancement
			 * When uncommanding it the previous function getAllTpAutoComplete() dont need
			 * /
			 *

			 foreach ($associateIdListArray as $userId) {
			 $checkEquivalenceTP = checkEquivalenceTP($userId, $entityId,$adminEnrollCall=true);
			 if(count($checkEquivalenceTP) > 0){
			 $tp_names = array();
			 foreach($checkEquivalenceTP as $result)
			 {
			 $tp_names[] = $result->equv_title;
			 }
			 $coursenames = implode(',',$tp_names);

			 $user_detail=db_query("select full_name from slt_person WHERE id = '".$userId."'");
			 $userDetail = $user_detail->fetchAll();
			 $fullName = $userDetail[0]->full_name;
			 $valuesEquWarning[] = $fullName." - is already registered to an equivalent Training Plan: ".$coursenames;
			 }
			 }
			 if(count($valuesEquWarning)>0){
			 $returnMsg = implode('', $valuesEquWarning);
			 $resultcnt=count($valuesEquWarning);
			 for($i=0;$i<$resultcnt;$i++){
			 $displayWarTxt = $valuesEquWarning[$i];
			 expertus_set_message($displayWarTxt, 'error');
			 }
			 }


			 expDebug::dPrint(' $checkEquivalenceTP= ' . print_r($checkEquivalenceTP, true) , 4);
			 expDebug::dPrint(' unique ID from TrainingPlan= ' . print_r($uniqueId, true) , 4);
			 // $equivalence = checkEquivalenceTP($userId, $courseId, $classId,$adminEnrollCall=false){
			 */
			
			$isMandatory='';
			if($form_state['triggering_element']['#name']=='set-enroll-tp-user-mandatory'){
				$isMandatory=1;
			}
			$registerStatus  = lp_singleclass_or_multiclass_cnt_for_course($entityId, '', '', $isAdminSide, $userIdList, $page, false, true,$regDate,$recertify_flag,$isMandatory);
			/* Changes for ticket 16522 -- not able to enroll users for a training plan*/
			$tpRegMode = '';
			expDebug::dPrint('$registerStatus : '.print_r($registerStatus , true), 4);
			$statususername= array();
			$valuesSuccess = array();
			$recertify_msg = array();
			$regstatus = '';
			if(!empty($registerStatus) && is_array($registerStatus)){

				foreach($registerStatus as $key=>$status){
					if(isset($status->status)){
						if($status->status != 'Registered'){
							$statususername[] = $status->username.', '. $status->status;
							expDebug::dPrint(' return 112 99= ' . print_r($registerStatus, true) , 4);
						}else{
							$tpRegMode = 'view';
						}
					}else{
						if(strpos($registerStatus[$key], t('MSG809')) !== false){
							$regstatus .='Recertified';
							$recertify_msg[]= $registerStatus[$key];
						}else if(!strstr($registerStatus[$key], t('Registered'))){
							$valuesSuccess[] = $registerStatus[$key];
							$regstatus .='default';
						}
						else{
							$regstatus .='Registered';
						}
					}
				}
				expDebug::dPrint('$registerStatus : '.print_r($regstatus , true), 4);
				if(count($statususername)>0){
					$returnMsg = implode('', $statususername);
					drupal_set_message($returnMsg,'error');
				}
				if(count($valuesSuccess)>0){
					$returnMsg = implode('', $valuesSuccess);
					$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('MSG714').'</div><div class="registered-msg-enrolluser-list">'.$returnMsg.'</div>';
					drupal_set_message($displayTxt1);
				}
				if(strstr($regstatus, 'Recertified')){
					$returnMsg1 = implode('', $recertify_msg);
					$displayTxt2 = '<div><div class="registered-msg-enrolluser">'.t('MSG714').'</div><div class="registered-msg-enrolluser-list">'.$returnMsg1.'</div>';
					drupal_set_message($displayTxt2);
					$tpRegMode = 'view';
				}
				else if(!strstr($regstatus, 'default')){
					$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('Registered').'</div>';
					drupal_set_message($displayTxt1);
					$tpRegMode = 'view';
				}else if(strstr($regstatus, 'Registered')){
					$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('Others').' '.t('Registered').'</div>';
					drupal_set_message($displayTxt1);
					$tpRegMode = 'view';
				}
				/* Changes for ticket 16522 -- not able to enroll users for a training plan*/
				if($tpRegMode == 'view'){
					$form_state['storage']['mode'] = 'view';
					$form_state['rebuild'] = true;
				}

			} else if(!empty($registerStatus)) {

				if($registerStatus == 'nocourses'){
					drupal_set_message(t('MSG299'), 'error');
				}else if($registerStatus == 'noclasses') {
					drupal_set_message(t('ERR108'), 'error');
				}else if($registerStatus == 'seatsfull'){
					drupal_set_message(t('ERR022'), 'error');
				}else if(strtolower($registerStatus) == 'multiregister'){
					drupal_set_message(t('ERR117'), 'error');
				}else{
					drupal_set_message(t($registerStatus), 'error');
				}

			}
		}
		/*else {

		drupal_set_message(t('ERR106'),'error');
		$form_state['storage']['mode'] = 'edit';
		$form_state['rebuild'] = true;
		//$form_state['storage']['refresh'] = 'norefresh';

		}*/

	}   catch (Exception $ex) {
		//$txn->rollback();
		watchdog_exception('addNewEnrollTPUserDetails', $ex);
		throw $ex;
	}

	//unset($txn);

}

/*
 * addEditRepaintEnrolltoTrainingForm() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *
 */
function addEditRepaintEnrolltoTrainingForm($form, &$form_state) {
	try {
		global $secure_entities;
		$form_state['storage']['mode'] = 'edit';

		$formName = $form_state['form_unique_name'];
		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
		$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
		
		if (in_array($entityType, $secure_entities)) {
			$uniqueId = "enrolltpuser-".core_encrypt($entityId)."-$entityType"; // URL encryption to retirve data
		} else {
			$uniqueId = "enrolltpuser-".$entityId."-$entityType"; // URL encryption to retirve data
		}

		$commands = array();
		// Comment Removed For This Ticket #0040313 , WHen E-sign is Enabled Unable To Enroll User By Admin.
		if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
			expDebug::dPrint('repainting formName');
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
			if($form_state['esign_show_popup']==true)
			$commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
		}
		expDebug::dPrint('repainting formName');
		//63622: Training plan doesn't moves to completed tab --start
		$programId = $form_state['form_program_id'];
		$entityType = $form_state['form_program_type'];
		$min_form_state = array(
				'ajax' => TRUE,
				're_render' => TRUE,
				'no_redirect' => TRUE,
				'program_id' => $programId,
				'build_info' => array('args' => $form_state['build_info']['args'])
		);
		
		$form_state = $min_form_state;
		$formId = 'exp_sp_administration_program_attaching_courses_form';
		$builtForm = drupal_build_form($formId, $form_state);
		$commands[] = ajax_command_replace('#attach_course_dt', drupal_render($builtForm));
		$commands[] = ajax_command_invoke(NULL, "attachClose", array($programId, $entityType));
		//63622: Training plan doesn't moves to completed tab --end
		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('addEditRepaintEnrolltoTrainingForm', $ex);
		expertusErrorThrow($ex);
	}
}

function listTPRosterByAPI($paramsObj, $userId){
	try {
		$programId = $paramsObj['ProgramId'];
		$userName = $paramsObj['UserName'];
		$date_from = trim($paramsObj['date_from']);
		$date_to = trim($paramsObj['date_to']);
		if(!empty($date_from)){
			if(!validateDateFormat($date_from))
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = "date_from";
				$errobj->errormsg = 'From date should be in yyyy-mm-dd format';
				return $errobj;
			}
		}
		if(!empty($date_to)){
			if(!validateDateFormat($date_to))
			{
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = "date_to";
				$errobj->errormsg = 'To date should be in yyyy-mm-dd format';
				return $errobj;
			}
		}		
		unset($_REQUEST['usid']);
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0){
			$result = fetchEnrollTPUserList($programId, 'cre_sys_obt_crt', '', $userName, '');
			$count = fetchEnrollTPUserList($programId,  'cre_sys_obt_crt', '',  $userName, '', '', '', '', 1);
			expdebug::dPrintDBAPI('$count :'. print_r($count, true),3);
			$result['totalrow'] = $count[0]->cnt;
		} else {

			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try{
				$rtnType = $_REQUEST['returntype'];
				$_REQUEST['returntype'] = 'json';
				$solrObj = new UserEnrollments();
				$getlpQryRes = $solrObj->TPEnrollments($programId,$entityType,$searchKeyword,'enrollments');
				$result = $getlpQryRes['result'];
				$result['totalrow'] = $getlpQryRes['count'];
				$_REQUEST['returntype'] = $rtnType;
				unset($_REQUEST['SolrImpl']);
			}catch(Exception $e){
				expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
			}
				
		}
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('listTPRosterByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function UpdateTPRosterByAPI(&$form_state,$createuserid=""){
	try {
		expDebug::dPrint(' UpdateTPRosterByAPI called');
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form_state = stripslashes_deep(trimInputFields($form_state)); // #42124 - Trim input values
		$form=array();
		$form["form_id"]="";
		$form_state['triggering_element']['#name'] = $form_state['action'].'andsave';
		$form_state['input']['mapped_program_id'] = $form_state['ProgramId'];
		$form_state['input']['mapped_program_type'] = 'cre_sys_obt_crt';
		expDebug::dPrint('$form_state: '.print_r($form_state , true), 4);
		  if(isset($form_state['CompletionDate']) && !validateDateFormat($form_state['CompletionDate'] , 'Y-m-d')){
			$error -> errormsg = 'Completion Date should be in yyyy-mm-dd format';
			$error -> isValidateError = true;
			return $error;
		}
		$form_state['RegStatusDate'] = str_replace('/', '-', $form_state['RegStatusDate']);
		expDebug::dPrint('Reg status Date : '.$form_state['RegStatusDate'], 4);
		   if(!empty($form_state['RegStatusDate']) && !validateDateFormat($form_state['RegStatusDate'] , 'Y-m-d')){
			$error -> errormsg = 'RegStatus Date should be in yyyy-mm-dd format';
			$error -> isValidateError = true;
			return $error;
		} 
		
	if(!empty($form_state['action']) && ($form_state['action']) == 'Canceled'){
		
	    $prgDetails = getProgramObjDetails($form_state['ProgramId']);
		expDebug::dPrint("dropTPclassunderManagerAPI $prgDetails::".print_r($prgDetails,TRUE) , 4);
		$isPricedTP =0;
		$select = db_select('system', 'sys');
		$select->addField('sys', 'status', 'status');
		$select->condition('name', 'exp_sp_commerce');
		$commerceEnabled =  $select->execute()->fetchField();
		expDebug::dPrint("dropTPclassunderManagerAPI $commerceEnabled::".print_r($commerceEnabled,TRUE) , 4);
		if($commerceEnabled == 1){
		    $isPricedTP = $prgDetails[0]->price;
		}
		if ($isPricedTP > 0) {
		    $errobj=new stdClass();
		    $errobj->isValidateError = 1;
		    $errobj->errcode = 'L_012';
		    $errobj->errormsg = "Priced training cannot be cancelled. Contact support team.";
		    //return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
		    return $errobj;
		}
	}
		
		$action = array('Canceled','Completed','Incomplete');
		
		if(!in_array($form_state['action'],$action)){
			$error -> errormsg = 'action Type is invalid';
			$error -> isValidateError = true;
			return $error;
		}
		unset($_REQUEST['uid']);
		$result = updateTPRosterEnrollmentDetails($form,$form_state,1);
		$validate = form_get_errors();
		if(count($validate)){
			return array((object)array('Id'=>implode(',' ,$validate)));
		}
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('UpdateTPRosterByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * _checkTPEnabled()
 */
function _checkTPEnabled($programId){
	try {
		$query = db_select('slt_program', 'program');
		$query->addField('program', 'id');
		$query->condition('program.status', 'lrn_lpn_sts_atv');
		$query->condition('program.id', $programId);
		$result = $query->execute()->fetchAll();
		return (count($result)) ? true : false;
	} catch (Exception $ex) {
		watchdog_exception('_checkTPEnabled', $ex);
		expertusErrorThrow($ex);
	}
}


function fetchEnrollTPUserListStatusCount($entityId){
	try {
		$statisticsArray = array();
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0) {
		$select = db_select('slt_master_enrollment','mse');
		$select->join('slt_person','per','per.id = mse.user_id');
		$select->addField('mse','overall_status','overall_status');
		$select->condition('mse.overall_status', array('lrn_tpm_ovr_rsc'), 'NOT IN');
		$select->condition('mse.program_id',$entityId,'=');
		$select->condition('mse.is_current','Y','='); // Added by Vincent for 0072624
		$select->addExpression('count(mse.overall_status)', 'overall_status_count');
		$select->groupby('mse.overall_status');
		$select->range();
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI('tp query',$select);

		$resultSet = $select->execute()->fetchAll();
		} else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try{
				$solrObj = new UserEnrollments();
				$getlpQryRes = $solrObj->TPEnrollments($entityId);
				$statusCount = $getlpQryRes['statusCount'];				
			}catch(Exception $e){
				expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
			}
		}
		if($srcSource == 1) {
			if($statusCount['Enrolled'] != 0)
				$statisticsArray[0] = t('Enrolled').': '.$statusCount['Enrolled'];
			if($statusCount['Completed'] != 0)
				$statisticsArray[1] = t('Completed').': '.$statusCount['Completed'];
			if($statusCount['Canceled'] != 0)
				$statisticsArray[2] = t('Canceled').': '.$statusCount['Canceled'];
			if($statusCount['In progress'] != 0)
				$statisticsArray[3] = t('In progress').': '.$statusCount['In progress'];
			if($statusCount['Incomplete'] != 0)
				$statisticsArray[4] = t('Incomplete').': '.$statusCount['Incomplete'];
			if($statusCount['Waitlist'] != 0)
				$statisticsArray[5] = t('Waitlist').': '.$statusCount['Waitlist'];
			if($statusCount['Pending'] != 0)
				$statisticsArray[6] = t('Pending').': '.$statusCount['Pending'];
			if($statusCount['Reserved'] != 0)
				$statisticsArray[7] = t('LBL942').': '.$statusCount['Reserved'];
			if($statusCount['Expired'] != 0)
				$statisticsArray[8] = t('Expired').': '.$statusCount['Expired'];
		} else {
			foreach($resultSet as $rec_search_results){
				$ovrStatusCount = empty($rec_search_results->overall_status_count) ? 0 : $rec_search_results->overall_status_count;
	
				//lrn_tpm_ovr_enr,lrn_tpm_ovr_cmp,lrn_tpm_ovr_cln,lrn_tpm_ovr_exp,lrn_tpm_ovr_inc,lrn_tpm_ovr_inp,lrn_tpm_ovr_ppm
				//lrn_tpm_ovr_ppv,lrn_tpm_ovr_rsc,lrn_tpm_ovr_rsv,lrn_tpm_ovr_wtl
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_enr'){
					$statisticsArray[0] = t('Enrolled').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_cmp'){
					$statisticsArray[1] = t('Completed').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_cln'){
					$statisticsArray[2] = t('Canceled').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_inp'){
					$statisticsArray[3] = t('In progress').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_inc'){
					$statisticsArray[4] = t('Incomplete').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_wtl'){
					$statisticsArray[5] = t('Waitlist').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_ppm'){
					$statisticsArray[6] = t('Pending').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_rsv'){
					$statisticsArray[7] = t('LBL942').': '.$ovrStatusCount;
				}
				if($rec_search_results->overall_status == 'lrn_tpm_ovr_exp'){
					$statisticsArray[8] = t('Expired').': '.$ovrStatusCount;
				}
			}
		}
		ksort($statisticsArray);
		foreach ($statisticsArray as $key=>$value) {
			$wrappedstatisticsArray[] = '<span>'.$value.'</span>';
		}
		
		$countResult = implode('<span class="narrow-search-results-item-detail-pipe-line">|</span>',$wrappedstatisticsArray);
		expDebug::dPrint('countttt111111 : '.count($wrappedstatisticsArray), 4);
		if(count($wrappedstatisticsArray)>1){
			$countResult .= '<div class="cls-status-count"><span>'.t('MSG771').'</span></div>';
		}else{
			$countResult .= '<div><span>'.t('MSG771').'</span></div>';
		}
		return $countResult;
	} catch (Exception $ex) {
		watchdog_exception('fetchEnrollTPUserListStatusCount', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 *  clearTPRosterEnrollUserAutocomplete() - Clears enrolltpuser-autocomplete form textfield as switching from enroll user view to roster view
 *                                             fix for ticket #0022120
 */
function clearTPRosterEnrollUserAutocomplete(&$form_state) {
	try {
		expDebug::dPrint('$form_state = ' . print_r($form_state, true), 5);

		if (!empty($form_state['complete form'][$form_state['form_unique_name']]['enrolltpuser-autocomplete']['#value'])) {
			$form_state['complete form'][$form_state['form_unique_name']]['enrolltpuser-autocomplete']['#value'] = '';
		}

		if (!empty($form_state['values']['enrolltpuser-autocomplete'])) {
			$form_state['values']['enrolltpuser-autocomplete'] = '';
		}
		if (!empty($form_state['input']['enrolltpuser-autocomplete'])) {
			$form_state['input']['enrolltpuser-autocomplete'] = t('LBL181');
		}

		if (!empty($form_state['input'][$form_state['form_unique_name']]['enrolltpuser-autocomplete'])) {
			$form_state['input'][$form_state['form_unique_name']]['enrolltpuser-autocomplete'] =  t('LBL181');
		}

		if (!empty($form_state['values'][$form_state['form_unique_name']]['enrolltpuser-autocomplete'])) {
			$form_state['values'][$form_state['form_unique_name']]['enrolltpuser-autocomplete'] = '';
		}
	} catch (Exception $ex) {
		watchdog_exception('clearTPRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}


function fetchEnrollmentHistory($entityId, $entityType,$enrId,$exportOption =''){
	
	$master_details = getMasterEnrollmentDetails($enrId, array('user_id'));
	expDebug::dPrint("master enrollment details >> ".print_r($master_details,true),5);
	$select = db_select('slt_master_enrollment','mse');
	$select->join('slt_module','module','module.program_id = mse.program_id and module.sequence = mse.recertify_path');
	$select->join('slt_profile_list_items','prof','mse.overall_status = prof.code');
	$select->addField('prof','name','overall_status');
	$select->addField('mse','id','id');
	$select->addField('mse','reg_date','reg_date');
	$select->addField('mse','comp_date','comp_date');
	$select->addField('module','title','path');
	expDebug::dPrintDBAPI('tp fetchEnrollmentHistory ',$select);
	$select->condition('mse.user_id', $master_details->user_id, '=');
	$select->condition('mse.program_id',$entityId,'=');

	expDebug::dPrintDBAPI('tp fetchEnrollmentHistory ',$select);

	if($exportOption =='CSV'){
		// Added by Vincent for #0072740
		$select->join('slt_person','per','per.id = mse.user_id');
		$select->addField('per','user_name','username');
		$select->addField('per','full_name','fullname');
		$select->orderBy('path','desc');
		$colHeadersMap = array(
			t('LBL054') => 'username',
			t('LBL691') => 'fullname',
			t('LBL3060') => 'path',
			t('Enrollment date') => 'reg_date',
			t('LBL706') => 'comp_date',
			t('LBL102') => 'overall_status'
		);
	    SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap,'export');
	}else{
		return $select;
	}
}

?>