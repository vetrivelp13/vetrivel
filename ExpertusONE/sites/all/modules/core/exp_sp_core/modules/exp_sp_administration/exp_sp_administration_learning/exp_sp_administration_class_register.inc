<?php

function displayClassRegisterDetails(&$form, &$form_state, $courseId, $entityId, $entityType, $uniqueId, $clsDeliveryType = '',$isclose=0){
	try {
		global $secure_entities;
		$srcSource = getConfigValue('solr_search');
		expDebug::dPrint(' $clsDeliveryType = ' . $clsDeliveryType,4);
		$customHiddenButtonId =  'enroll-user-toclass-esign-button';
		if (in_array($entityType, $secure_entities)) {
	  		$entityId_encrypted = core_encrypt($entityId); // URL encryption
	  	} else {
	  		$entityId_encrypted = $entityId;
	  	}
		$uniqueId = "enrolluser-$entityId_encrypted-$entityType";

		$wrapperId = 'enrolluser-grid-wrapper-'.$uniqueId;
		$formName = 'enrolluser_form_'.$uniqueId;
		$form_state['form_unique_name'] = $formName;

		$msg = t('MSG506');

		$form['attr_action'] = array(
    '#type' => 'hidden',
    '#name' => 'attr_action',
    '#value' => 'enrolluser',
		);

		$setminHight = ($entityType=='cre_sys_obt_cls') ? 'admin-course-class-tab-datagrid-wrapper' : '';

		$form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-data-grid"><div id="admin_list_values_container" class="enrolled-display-controller"><div id="'.$wrapperId.'" class="admin-datagrid-pagination enrolluser-class-grid-wrapper '.$setminHight.'">',
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

		$form[$formName]['hidden_delivery_type'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_delivery_type_'.$uniqueId,
    '#id' => 'hidden-delivery-type-'.$uniqueId,
    '#value' => $clsDeliveryType,
		);

		$form[$formName]['class_code'] = array(
    '#type' => 'hidden',
    '#name' => 'class_code_'.$uniqueId,
    '#id' => 'class-code-'.$uniqueId,
    '#value' => isset($form_state['values']) ? $form_state['values']['code'] : $form_state['code'],
		);

		$form[$formName]['hidden_completion_date'] = array(
    '#type' => 'hidden',
    '#name' => 'hidden_completion_date_'.$uniqueId,
    '#id' => 'datagrid-completiondate-'.$uniqueId,
		);

		$form[$formName]['hidden_registration_date'] = array(
				'#type' => 'hidden',
				'#name' => 'hidden_registration_date_'.$uniqueId,
				'#id' => 'hidden-registationdate-'.$uniqueId,
		);
		$pricedErr = 0;
		$isPricedClass = getSltClassPrice($entityId);
		$classHasContent = true;
		if ($entityType == 'cre_sys_obt_cls') {
			$classValidRec = checkClassValidity($entityId, $clsDeliveryType);
			if ($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl' ) {
				$classHasContent = $classValidRec->totalSessions > 0? true : false;
			}
			else {
				$classHasContent = $classValidRec->contentCount > 0? true : false;
			}
		}
		//enrollment date issue
		$uniqueId2 = $entityId.$entityType;
		$registrationId = '#registration_date_container_'.$uniqueId2;
		$checkPastClass = false;
		$isPastClass = false;
		//check the session start date for ILT and VC classes
		if($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl') {
			$checkPastClass = true;
			$currentdate = date('m/d/Y');
			$sessionStartEndDates = getSessionStartEndDates($entityId, false);
			expDebug::dPrint("sessionStartEndDates".print_r($sessionStartEndDates, 1), 5);
			$sessionStartEndDatesArray = explode('#', $sessionStartEndDates);
			$sessionStartdate = str_replace(',', '-', $sessionStartEndDatesArray[0]);
			expDebug::dPrint("sessionStartdate".print_r($sessionStartdate, 1), 5);
			expDebug::dPrint("currentdate".print_r($currentdate, 1), 5);
			$registrationdatelbl = t('LBL1067').' '.t('LBL042');
			$closelbl = t('LBL123');
			$donelbl = t('LBL569');
			$form[$formName]['hidden_session_start_date'] = array(
					'#type' => 'hidden',
					'#name' => 'hidden_session_start_date'.$uniqueId,
					'#id' => 'hidden_session_start_date'.$uniqueId,
					'#value' => $sessionStartdate
			);
		}
		$displayMode = $form_state['storage']['mode'];
		if($srcSource == 0) {
			$enrolluserDetails = fetchEnrollUserList($courseId, $entityId, $entityType,'', '', '', '', '',$displayMode,1);
			$count = $enrolluserDetails[0]->enrollment_count;			
		} else {			
			try {
				require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
				$solrObj = new UserEnrollments();
				$listqueryResult = $solrObj->classEnrollments($entityId,$entityType,'','enrollments');
				$count = $listqueryResult['count'];				
			} catch (Exception $e) {
				expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
			}			
		}
	
		$userIdArr = array();
		$excludedUserId = '""';
		$excludedUserId = implode(',', $userIdArr);
		$excludedUserId = $excludedUserId == "" ? 0 : $excludedUserId;
		$checkIfDedicatedClass = is_dedicated($entityId);
		
		if($srcSource == 0) {
			$listAvailableUserResult = loadCtoolDataGridForEnrollUser('', 0, $entityId, 1);
		} else {
			try{
				require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
		        $solrObj = new UserSolrSearch();
		        $search_results = $solrObj->getSolrSearchResultEnrollUser('','',$entityId,$enrPage='class');
		        $listAvailableUserResult = $search_results['recCount'];
			}catch(Exception $e){
				expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
			}
		}
		if(!empty($checkIfDedicatedClass) && $count == 0  && $classHasContent) {
			$msg = t('MSG766');
		}
		if($count == 0 && empty($displayMode)){

			//$addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
			if($isPricedClass > 0.00 || (!empty($checkIfDedicatedClass) && $count == 0)){
				$addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-greyout-bg';
			}
			else {
				$addAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
			}
			$addAttributes['data-wrapperid'] = array($wrapperId);

			if(isset($form_state['storage']['display_mode']) && $form_state['storage']['display_mode'] == 'view'){

				$form[$formName]['add_prereqiuisite_text'] = array(
        '#type' => 'markup',
      	'#markup' => '<div class="no-records-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.t('There are no users enrolled for this class.').'</div></div>',
				);

			} else {
				if ($listAvailableUserResult == 0 || !$classHasContent) {

					if ($listAvailableUserResult == 0) {
						$msg = t('MSG500');
					}
					else if ($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl') {
						$msg = t('MSG499');
					}
					
					else {
						$msg = t('MSG498');
					}
					//51141: Modified by joolavasavi
					$form[$formName]['enrollment_start'] = array(
							'#type'       => 'markup',
							'#markup'     => '<table id="enrollment-upload-assign" cellpadding="1" cellspacing="0" width="100%" border="0"><tr>',
					);

					$form[$formName]['add_enrolluser'] = array(
          '#type' => 'markup',
          '#prefix' => '<td width="40%" class="clsEnrollUser"><div class="dt-add-btn-stage-one enrollment-upload-container"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-greyout-bg"></div>',
          '#suffix' => '<div class="admin-save-button-middle-greyout-bg">'.t('LBL640').'</div><div class="admin-save-button-right-greyout-bg"></div></span>',
					);

					$form[$formName]['add_enrolluser_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></td>',
					);

					$form[$formName]['upload_attachment_link_url_disp'] = array(
							'#type'       => 'markup',
							'#markup'     => '<td class="attachment-seperate-display-td-cls" width="5%"><span class="attachment-seperate-display"></span><span class="or-seperate-display"> '.t('LBL644').' </span><span class="attachment-seperate-display"></span>
							</td>',
					);


					$form[$formName]['upload_enrolluser'] = array(
							'#type' => 'markup',
							'#markup'=>'<td width="40%" class="clsEnrollUser">
      			<div class="enrollment-upload-container"><span class="attach-btn-group-middle"><div class="admin-save-button-left-greyout-bg"></div><span class="admin-save-button-middle-greyout-bg" >'.t('LBL609').'</span><div class="admin-save-button-right-greyout-bg"></div></span></div>',
					); 

					$form[$formName]['enrollment_end'] = array(
							'#type'       => 'markup',
							'#markup'     => '<div class="admin-empty-text-msg" id="upload-file-id">'.$msg.
							'</div></Enrollments roster pop up is inconsistence td></tr></table><div class="clsusrprivi-msg"><span>'.t('MSG771').'</span></div>'
					);

				} else {
					$form[$formName]['attachment_part_disp_start'] = array(
  	  			'#type'       => 'markup',
     			  '#markup'     => '<table id="enrollment-upload-assign" cellpadding="1" cellspacing="0" width="100%" border="0"><tr>',
    			);

    			$form[$formName]['attachment_upload_file_disp_start'] = array(
  	  			'#type'       => 'markup',
      			'#markup'     => '<td width="40%" class="clsEnrollUser">',
    			);
    			if($isPricedClass > 0.00  || (!empty($checkIfDedicatedClass) && $count == 0)){
    				$prefix = 'admin-save-button-left-greyout-bg';
    				$suffix = 'admin-save-button-right-greyout-bg';
    				$disable = true;
    			} else{
    				$prefix = 'admin-save-button-left-bg';
    				$suffix = 'admin-save-button-right-bg';
    				$disable = false;
    			}
					$form[$formName]['add_enrolluser'] = array(
          '#type' => 'submit',
          '#prefix' => '<div><div class="dt-add-btn-stage-one enrollment-upload-container"><span class="add-button-courseclass-tp"><div class="'.$prefix.'"></div>',
          '#suffix' => '<div class="'.$suffix.'"></div></span></div>',
          '#title' => t('LBL361'),
          '#name' => 'Add enrolluser',
          '#value' => t('LBL640'),
		  '#disabled' => $disable,
          '#submit' => array('add_enrolluser'),
          '#attributes' => $addAttributes,
          '#ajax' => array(
            'callback' => 'enrollUserGrid',
            'wrapper' => $wrapperId,
					),
					);
			$enrollmentUpload = '<span class="attach-btn-group-middle"><div class="admin-save-button-left-bg"></div><span class="admin-save-button-middle-bg" >'.t('LBL609').'</span><div class="admin-save-button-right-bg"></div></span>';
			if($isPricedClass > 0 || (!empty($checkIfDedicatedClass) && $count == 0)) {
				$disableRosterUpload = true;
				$enrollmentUpload = '<span class="attach-btn-group-middle"><div class="admin-save-button-left-greyout-bg"></div><span class="admin-save-button-middle-greyout-bg" >'.t('LBL609').'</span><div class="admin-save-button-right-greyout-bg"></div></span>';
			}
					$form[$formName]['add_enrolluser_text'] = array(
          '#type' => 'markup',
          '#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-'.$uniqueId.'">'.$msg.'</div></div></td>',
					);
					$path = base_path() . drupal_get_path('module', 'uc_credit');
					$form[$formName]['attachment_link_url_disp'] = array(
  	  			'#type'       => 'markup',
      			'#markup'     => '<td class="attachment-seperate-display-td-cls" width="5%"><span class="attachment-seperate-display"></span><span class="or-seperate-display"> '.t('LBL644').' </span><span class="attachment-seperate-display"></span>
      			</td><td width="40%" class="clsEnrollUpload">
      			<div class="enrollment-upload-container">'.$enrollmentUpload.'</div>',
    			);
					$browseAttributes             = array();
   				$browseAttributes['class'][]  = 'attach-browse';
   				$browseAttributes['class'][]  = 'enrol-upload-browse-hid';
   				$browseAttributes['onChange'] = 'document.getElementById(\'enrollment_upload_done_btn\').click()';
    			$form[$formName]['enrollment_upload_file'] = array(
      			'#type'          => 'file',
      			'#id'            => 'enrollment_upload_file',
      			'#title'   	   => t('Choose a file'),
      			'#attributes'    => $browseAttributes,
      			'#size'          => 1,
      			'#theme_wrappers' => array(),
    			);
				if($isPricedClass > 0) {
					$form[$formName]['enrollment_upload_file']['#disabled'] = TRUE;
				}
				if(!empty($checkIfDedicatedClass) && $count == 0) {
						$form[$formName]['enrollment_upload_file']['#disabled'] = TRUE;
						$errorMsg = t('MSG766');
						$toolTipIcon = '';
				} else {
					$errorMsg = t('MSG740');
					$toolTipIcon = '<span id="class-enrollment"><img src="' . $path . '/images/info.png" class="info-classroster-upload vtip"
      												onclick="$(\'#download_template_class_wrapper\').show();downloadLink(\'enrollment_upload_class\');" title="'.t("MSG870").'" style="cursor:pointer"/> 
      					<span id="download_template_class_wrapper" style="display:none"><div id="download_roster_class_arrow" class="download_roster_class_arrow"></div>
    					<div id="download_template_class">
						<div id="clstemplatemsg"><span class="clstemplatemsg">'.t('MSG741').'</br></br>'.t('MSG742').'</br></br>'.t('MSG743').'</br></br>'.t('MSG865').' </span><a id="clsdownload-link" class="download_link" href="javascript:void(0);" onclick="downloadSample(\'enrollment_upload_file\');">'.t('MSG854').'</a>.</div>
    					</div>
    					</span></span>';
				}
				
					$form[$formName]['attachment_upload_file_disp_end'] = array(
  	  			'#type'       => 'markup',
      			'#markup'     => '<div class="admin-enroll-empty-text-msg" id="upload-file-id">'. $errorMsg . "" . $toolTipIcon . '
							 	  </div></td></table>
								  <div class="clsusrprivi-msg"><span>'.t('MSG771').'</span></div>'
    			);
	    		$doneBtnAttributes                      = array();
	    		//$doneBtnAttributes['class'][]           = 'attach-btn-catalog-provide-url';
	    		$doneBtnAttributes['class'][]           = 'addedit-form-expertusone-throbber';
	    		$doneBtnAttributes['data-wrapperid']    = array($wrapperId);
	    		$doneBtnAttributes['onclick'][]         = 'return false;';
	    		$form[$formName]["enrollment_upload_done"] = array(
	  	    	'#type'       => 'submit',
	        	'#id'         => 'enrollment_upload_done_btn',
	  	    	'#prefix'     => '<div style="display:none;">',
	  	    	'#suffix'     => '</div>',
	  	    	'#value'      => t('Done'),
	        	'#name'       => 'done',
	    			'#submit' => array('enrollment_upload'),
	  	    	'#attributes' => $doneBtnAttributes,
	        	'#ajax' => array(
			      	'wrapper'   => $wrapperId,
	    		  	'callback'    => 'enrollUserGrid',
	    				'entity_id' => $entityId,
          		'entity_type' => $entityType,
			      	'method'    => 'replace',
			      	'effect'    => 'none', // 'fade',
			      	'event'     =>'click',
			      	'keypress'  => true,
			      	'progress' => array(
			        	'type'    => 'throbber',
			        	'message' => ''
			      	),
		    		),
	  	   	);
				}
			}

		} else {
			$searchAttributes = array();
			if($displayMode == 'edit'){

				$paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
				$paintMultiAction .= "<span id='search-dropdwn-list'>";
				$paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
				$paintMultiAction .= "<input type='hidden' id='enrolluser-autocomplete_hidden' value='".t('LBL181')."' />";
				$paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>".t('LBL054')."</span>";
				$paintMultiAction .= "<a  id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
				$paintMultiAction .= "<ul id='select-list-dropdown-list'>";
				$paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','Class');\">".t('LBL054')."</li>";
				$paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','Class');\">".t('LBL107')."</li>";
				//$paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('".t('LBL102')."','status','Class');\">".t('LBL102')."</li>";
				$paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','Class');\">".t('Organization')."</li>";
				$paintMultiAction .= "<li id='jobrole-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('jobrole','Class');\">".t('LBL133')."</li>";
				$paintMultiAction .= "<li id='usertype-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('usertype','Class');\">".t('LBL173')."</li>";
				$paintMultiAction .= "<li id='manager-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('manager','Class');\">".t('Manager')."</li>";
				$paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','Class');\">".t('Group')."</li>";
				$paintMultiAction .= "</ul>";
				$paintMultiAction .= "</span></div>";


				$form[$formName]['enroll_multi_search_container'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
				);
				$form[$formName]['more_add_enrolluser_search'] = array(
      '#type' => 'markup',
      '#markup' => $paintMultiAction,
				);
				//$searchAjaxEvent = 'click';
				//$searchAttributes['onClick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "enrolluser", $(\'#enrolluser-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "0"); return false;';

				$acMenuPath = 'administration/learning/catalog-class/notenrolleduser-autocomplete';

				$form[$formName]['add_enrolluser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
				);

				$autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
				$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
				addACFieldForMultiSelect($form[$formName], 'enrolluser-autocomplete', $acMenuPath, '', '', '', array(),
				array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'", 'search_type' => "$('#search_all_enroll_type-hidden').val()"), false, false,
				t('LBL181'), $autoCompleteAttributes);

				$form[$formName]['add_enrolluser_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
				);

				/*$searchAttributes['Class'][] = 'admin-pagination-search-go';
				 $form[$formName]['search_enrolluser'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL304'),
				 '#name' => 'Search enrolluser',
				 '#attributes' => $searchAttributes
				 );*/
				$searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolluser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0,$(\".skip_course_access\").attr(\"checked\"));'></a>";
				$form[$formName]['search_enrolluser'] = array(
      '#type' => 'markup',
      '#markup' => $searchlink,
				);
				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>',
				);

			} else {
				$displayMode = 'view';
				$paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
				$paintMultiAction .= "<span id='search-dropdwn-list'>";
				$paintMultiAction .= "<input type='hidden' id='search_all_enroll_type-hidden' value='user' />";
				$paintMultiAction .= "<input type='hidden' id='enrolluser-autocomplete_hidden' value='".t('LBL181')."' />";
				$paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>".t('LBL054')."</span>";
				$paintMultiAction .= "<a id='admin-dropdown-arrow' class='select-list-dropdown-link dropdown-link-font ' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";
				$paintMultiAction .= "<ul id='select-list-dropdown-list'>";
				$paintMultiAction .= "<li id='user-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('user','Class');\">".t('LBL054')."</li>";
				$paintMultiAction .= "<li id='fullname-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('fullname','Class');\">".t('LBL107')."</li>";
				$paintMultiAction .= "<li id='status-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('status','Class');\">".t('LBL102')."</li>";
				$paintMultiAction .= "<li id='org-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('org','Class');\">".t('Organization')."</li>";
				$paintMultiAction .= "<li id='jobrole-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('jobrole','Class');\">".t('LBL133')."</li>";
				$paintMultiAction .= "<li id='usertype-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('usertype','Class');\">".t('LBL173')."</li>";
				$paintMultiAction .= "<li id='manager-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('manager','Class');\">".t('Manager')."</li>";
				$paintMultiAction .= "<li id='group-enr-search' onclick=\"$('body').data('mulitselectdatagrid').moreEnrollSearchTypeText('group','Class');\">".t('Group')."</li>";
				$paintMultiAction .= "</ul>";
				$paintMultiAction .= "</span></div>";

				$form[$formName]['enroll_multi_search_container'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_search_container">',
				);
				$form[$formName]['more_add_enrolluser_search'] = array(
      '#type' => 'markup',
      '#markup' => $paintMultiAction,
				);

				//$searchAjaxEvent = 'click';
				//$searchAttributes['onclick'][] = '$("body").data("mulitselectdatagrid").searchDataGrid("'.$displayMode.'", "enrolluser", $(\'#enrolluser-autocomplete\').val(), "'.$entityId.'", "'.$entityType.'", "0"); return false;';

				$acMenuPath = 'administration/learning/catalog-class/enrolleduser-autocomplete';

				$form[$formName]['add_enrolluser_search'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="admin_add_multi_auto_search">',
				);


				$autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
				$autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
				addACFieldForMultiSelect($form[$formName], 'enrolluser-autocomplete', $acMenuPath, '', '', '', array(),
				array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'", 'id_required' => 0,'search_type' => "$('#search_all_enroll_type-hidden').val()"),
				false, false, t('LBL181'), $autoCompleteAttributes);

				$form[$formName]['add_enrolluser_search1'] = array(
      '#type' => 'markup',
      '#markup' => '</div>',
				);

				$searchAttributes['Class'][] = 'admin-pagination-search-go';
				/* $form[$formName]['search_enrolluser'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL304'),
				 '#name' => 'Search enrolleduser',
				 '#attributes' => $searchAttributes
				 );*/
				$searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolluser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0);'></a>";

				$form[$formName]['search_enrolluser'] = array(
      '#type' => 'markup',
      '#markup' => $searchlink,
				);
				$form[$formName][] = array(
      '#type' => 'markup',
      '#markup' => '<div class="filter-search-start-date-right-bg"></div><div class="clearBoth"></div></div><div class="clearBoth"></div>',
				);

				/* $form[$formName]['upload_enrollment'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="enrollment-upload-container-view"><a class="enrollments-upload-icon" title="'.t('LBL609').'"></a></div>',
				); */

				// $diableBrowseButton = $isPricedClass > 0 ? true : false;
				//Priced training cannot be canceled. Cancellation can be done from the order screen.
				$enrollment_icon_class = '';
				
				if($isPricedClass > 0 || (!empty($checkIfDedicatedClass) && $count != 0 )) {
					$enrollment_icon_class = ' disabled';
				}
				$addClassName = ($form_state['call_from'] == 'catalog_enroll_display') ? '' : 'alter-margin-top';
				$browseAttributes             = array();
   				$browseAttributes['class'][]  = 'attach-browse';
   				$browseAttributes['class'][]  = 'enrol-upload-browse-hid-view';
   				$browseAttributes['class'][]  = 'vtip';
   				$browseAttributes['class'][]  = 'browse-button-vtip';
   				$browseAttributes['class'][]  = 'info-enr-upload';
   				$browseAttributes['onChange'] = 'document.getElementById(\'enrollment_upload_done_btn_view\').click()';
				$browseAttributes['title'] = '';	//empty text for title - If enabled vtip behaviour will be affected
				$browseAttributes['titletemp'] = $isPricedClass > 0 ? t('LBL1246') : '';
				if(!empty($checkIfDedicatedClass) && $count != 0) {
				  $browseAttributes['disabled'] =  TRUE;
				}
    			$form[$formName]['enrollment_upload_file_view'] = array(
      			'#type'          => 'file',
      			'#id'            => 'enrollment_upload_file_view',
      			'#title'   	   => t('Choose a file'),
      			'#attributes'    => $browseAttributes,
      			'#size'          => 1,
      			'#theme_wrappers' => array(),
				// '#prefix' => '<div id="enrollment-upload-container" class="enrollment-upload-container-view"><a class="enrollments-upload-icon" title="'.t('LBL609').'"></a><span id="page-pipe" class="page-pipeline">|</span>',
				//<div id=uploadexport> </div>
    					'#prefix' => '<div id="exportcontainer" class="exportcontainer '.$addClassName.'">
    					<div id=uploadexport><a class="enrollments-upload-icon'.$enrollment_icon_class.'">',    					
				'#suffix' => '</a><span id=upload>'.t('LBL610').'</span><div id="help_text" class="help-text">
    					<img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png" class="info-classroster-upload vtip" onclick="$(\'#download_template_class_wrapper\').show();downloadLink(\'enrollment_upload_class\');" title="'.t('MSG870').'" />
    					<span id="download_template_class_wrapper" style="display:none"><div id="download_roster_class_arrow" class="download_roster_class_arrow" style="cursor:pointer"></div>
    					<div id="download_template_class">
							<div id="clstemplatemsg"><span class="clstemplatemsg">'.t('MSG741').'</br></br>'.t('MSG742').'</br></br>'.t('MSG743').'</br></br>'.t('MSG865').' </span><a id="clsdownload-link" class="download_link" href="javascript:void(0);" onclick="downloadSample(\'enrollment_upload_file\');">'.t('MSG854').'</a>.</div>
    					</div>
    					</span>
    					</div>
    					</div>
    					<span id="page-pipe-upload" class="page-pipeline">|</span>'
    			);
				if($isPricedClass > 0) {
					$form[$formName]['enrollment_upload_file_view']['#disabled'] = TRUE;
				}
				$form[$formName]['export_link'] = array(
				'#type' => 'markup',
				'#markup' => "<a onclick=\"$('body').data('mulitselectdatagrid').callEnrollExportProcess('".$courseId."','".$entityId."','".$entityType."','Class');\" class='enrollments-exports-icon' title=".t('LBL309')."></a><span id=export>".t('LBL309')."</span></div>",
				);

			}

			$form[$formName]['enrolluser_view_grid_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
			);
		 	if($displayMode=='view'){
				$form[$formName]['statistics_count_action_container_start'] = array(
						'#type' => 'markup',
						'#markup' => '<div class="statistics-count-action-container">',
				);
			}
			$form[$formName]['statistics_count'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="enrollment-statistics-count" id="statistics-count-'.$uniqueId.'"></div>',
			);

			$displayModeDataGrid = $form_state['storage']['display_mode'] == 'view' ? 'view_only' : $displayMode;
			$form[$formName]['content_view_grid_load'] = array(
      '#type' => 'markup',
      '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'enrolluser\', \'\', \''.$entityId_encrypted.'\', \''.$entityType.'\', \'0\');" width="100" height="132" /></div>',
			);

			if($displayMode == 'view'){
				$addAnotherAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
				$addAnotherAttributes['data-wrapperid'] = array($wrapperId);

				/*if($form_state['storage']['display_mode'] != 'view'){
				 $form[$formName]['add_enrolluser_view_mode'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL361'),
				 '#prefix' => '<div id= "datagrid-add-search-button-'.$uniqueId.'"  style="display:block;" class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
				 '#suffix' => '<div class="admin-save-button-right-bg"></div></div>',
				 '#value' => t('LBL640'),
				 '#name' => 'Add enrolluser',
				 '#submit' => array('add_enrolluser'),
				 '#attributes' => $addAnotherAttributes,
				 '#ajax' => array(
				 'callback' => 'enrollUserGrid',
				 'wrapper' => $wrapperId,
				 ),
				 );
				 $form[$formName]['add_enrolluser_view_mode_btn'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL361'),
				 '#prefix' => '<div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
				 '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
				 '#value' => t('LBL640'),
				 '#name' => 'Add enrolluser',
				 '#submit' => array('add_enrolluser'),
				 '#attributes' => $addAnotherAttributes,
				 '#ajax' => array(
				 'callback' => 'enrollUserGrid',
				 'wrapper' => $wrapperId,
				 ),
				 );
				 $form[$formName]['enrolluser_view_grid_no_msg'] = array(
				 '#type' => 'markup',
				 '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div>',//</div></div>
				 );
				 }
				 */
				if($isclose){
				    $btnclose =1;
				}else{
					$btnclose =0;
				}
				addEditSaveFormForEnrollUser($form, $formName, $wrapperId, 'save-button', 'cancel-button', '', $entityId, $entityType, $uniqueId, $listAvailableUserResult, $clsDeliveryType,true, $classHasContent,$btnclose);
				/*    $form[$formName]['add_enrolluser_view_mode_btn'] = array(
				 '#type' => 'submit',
				 '#title' => t('LBL361'),
				 '#prefix' => '
				 <div style="display:none;"  id="datagrid-noresult-msg-'.$uniqueId.'"><div class="admin-datagrid-add-btn-wrapper-container"><div class="dt-add-btn-stage-one"><span class="add-button-courseclass-tp"><div class="admin-save-button-left-bg"></div>',
				 '#suffix' => '<div class="admin-save-button-right-bg"></div></span></div>',
				 '#value' => t('LBL640'),
				 '#name' => 'Add enrolluser',
				 '#submit' => array('add_enrolluser'),
				 '#attributes' => $addAnotherAttributes,
				 '#ajax' => array(
				 'callback' => 'enrollUserGrid',
				 'wrapper' => $wrapperId,
				 ),
				 );
				 $form[$formName]['enrolluser_view_grid_no_msg'] = array(
				 '#type' => 'markup',
				 '#markup' => '<div class="admin-empty-text-msg"> '.$msg.'</div></div>',//</div></div>
				 );

				 $form[$formName]['enrolluser_view_grid_no_msg'] = array(
				 '#type' => 'markup',
				 '#markup' => '</div></div>',//</div></div>
				 );*/
			} else if($displayMode == 'edit'){
				$isCompliance=getIfCourseCompliance($courseId);				
				//skip access
				$form[$formName]['display_unassigned_user'] = array(
						'#type' => 'checkbox',
// 						'#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="checkbox-unselected">',
// 						'#suffix' => '</div>',
						'#id'=>'skip_access',
						'#title' => t('Show only users who do not have access'),
						'#attributes' => array('onclick' => "$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"$displayMode\", \"enrolluser\", $(\"#enrolluser-autocomplete\").val(), \"$entityId_encrypted\",\"$entityType\", 0, this.checked);"),
						//'#default_value' => ($defaultValue['display_unassigned_user'] == 1) ? True : FALSE
				);
				$form[$formName]['display_unassigned_user'] = array(
						'#type' => 'markup',
						'#markup' => "<div class=\"admin-datagrid-save-cancel-button-align-bottom\"><div class='checkbox-label-wrapper'><div class=\"checkbox checkbox-unselected\"><input onclick=\"checkboxSelectedUnselectedCommon(this);$('body').data('mulitselectdatagrid').searchDataGrid('$displayMode', 'enrolluser', $('#enrolluser-autocomplete').val(), '$entityId_encrypted','$entityType', 0, this.checked);\" id=\"skip_access\"  class=\"skip_course_access\" type=\"checkbox\"></div><label class=\"option\" for=\"skip_access\">".t('Show only users who do not have access')."</label></div>",
						'#id'=>'skip_access',
				);
// 				<input onclick="$(&quot;body&quot;).data(&quot;mulitselectdatagrid&quot;).searchDataGrid(&quot;edit&quot;, &quot;enrolluser&quot;, $(&quot;#enrolluser-autocomplete&quot;).val(), &quot;ZZ34ZZe6e7e95f3cbf3697&quot;,&quot;cre_sys_obt_cls&quot;, 0, this.checked);" id="skip_access" name="enrolluser_form_enrolluser-ZZ34ZZe6e7e95f3cbf3697-cre_sys_obt_cls[display_unassigned_user]" value="1" class="form-checkbox" type="checkbox">
				
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
				//$listquery = loadCtoolDataGridForEnrollUser('', '', $excludedUserId);
				
				$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
				$cancelAttributes['data-wrapperid'] = array($wrapperId);

				$form[$formName]['cancel_enrolluser_edit_mode'] = array(
          '#type' => 'submit',
          '#value' => t('LBL109'),
          '#title' => t('LBL109'),
          '#prefix' => '<div class="addedit-form-cancel-container-actions admin-save-button-container resize-save_btn2 enroll-user-save-button-contaniner"><div class="white-btn-bg-left"></div>',
          '#suffix' => '<div class="white-btn-bg-right"></div>',
          '#name' => 'Cancel enrolluser',
          '#submit' => array('cancelEnrollUserDetails'),
          '#attributes' => $cancelAttributes,
          '#ajax' => array(
            'callback' => 'enrollUserGrid',
            'wrapper' => $wrapperId,
				),
				);

				//$listquery = loadCtoolDataGridForEnrollUser('', '', $excludedUserId);
				$form[$formName]['hidden-item'] = array(
						'#type' => 'hidden',
						'#attributes' => array('id' => 'enroll_mandatory_hidden'),
						'#default_value' => '0'
				);
				$enrollFormElementName = 'set_enrolluser_edit_mode';
				$enrollUserPrefix = '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>';
				if((strtotime($sessionStartdate) < strtotime($currentdate)) && $checkPastClass) {
					$isPastClass = true;
					$sessionStartdateForPicker = date('m/d/Y', strtotime($sessionStartdate));
					$registrationContainer = <<<EOD
<div class="registration_container_roster" id="registration_date_container_$uniqueId2">
<a style="float: right; position: relative; cursor: pointer;" class="qtip-button"  onClick="$('$registrationId').hide();"><div onClick="$('#enroll_mandatory_hidden').val(0);" class="admin-bubble-close"> </div></a>
$registrationdatelbl:  <input size="8" type="text" name="overall_registration_date_$uniqueId2" id="overall_registration_date_$uniqueId2" class="enrollement-date-picker" value="$sessionStartdateForPicker"/>
	<div id="enroll-user-registration-date-popup" class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span onClick="$('$registrationId').hide();" class="admin-action-button-middle-bg white-btn-bg-middle">$closelbl</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg registration-button-space"></div><input type="button" class="admin-save-button-middle-bg"  value="$donelbl" onClick="$('#root-admin').data('narrowsearch').fillRegistrationDate('$uniqueId', '$uniqueId2'); 
	 if($('#enroll_mandatory_hidden').val()==0)$('#add_new_user_$uniqueId2').click();else $('input[name=set-enrolluser-mandatory]').click(); return false;"/><div class="admin-save-button-right-bg"></div></div>
</div>
EOD;
					$enrollUserPrefix = $registrationContainer.'<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div><input class="admin-save-button-middle-bg" onClick="$(\'#root-admin\').data(\'narrowsearch\').registrationDateQTip(\''.$uniqueId2.'\',\''.$entityId.'\',\''.$isPastClass.'\',\''.$sessionStartdate.'\'
							); return false;clearMessages();" type="button" value="'.t('LBL640').'" name="set-enrolluser">';
					$enrollFormElementName = 'set_enrolluser_edit_mode_pastclass';
					$form[$formName]['hidden_registration_date_required'] = array(
							'#type' => 'hidden',
							'#name' => 'hidden_registration_date_required',
							'#id' => 'hidden_registration_date_required',
							'#value' => 'required'
					);
					expDebug::dPrint("isPastClass "."hidden field added", 5);
				}
				expDebug::dPrint("isPastClass".print_r($isPastClass, 1), 5);

				if($listAvailableUserResult != 0){
					/*$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
					 $setAttributes['data-wrapperid'] = array($wrapperId);

					 $form[$formName]['set_enrolluser_edit_mode'] = array(
					 '#type' => 'submit',
					 '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
					 '#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
					 '#title' => t('LBL305'),
					 '#value' => t('LBL362'),
					 '#name' => 'Set enrolluser',
					 '#submit' => array('addNewEnrollUserDetails'),
					 '#attributes' => $setAttributes,
					 '#ajax' => array(
					 'callback' => 'enrollUserGrid',
					 'wrapper' => $wrapperId,
					 'entity_id' => $entityId,
					 'entity_type' => $entityType,
					 'course_id' => $courseId,
					 ),
					 );*/
		
					if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && 1 == 0){ // e-Signature option should disabled
						$buttonName  = 'esign-enrolluser';
					}else{
						$buttonName  = 'set-enrolluser';
					}
					$span_publish_button='';
								
					if($isCompliance!=1){
						$span_publish_button='<span id="pub-unpub-action-btn" onclick="displayPubActionList(\'cls-pub-save-btn\')" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span>';
						$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
					}else{
						$setAttributes['class'][] = 'compliance_course_border addedit-form-expertusone-throbber admin-save-button-middle-bg';
					}
									
					$setAttributes['data-wrapperid'] = array($wrapperId);
					$setAttributes['onclick'][] = 'clearMessages();';
					if($isPastClass) {
						$setAttributes['class'][] = 'element-invisible';
					}
					$form[$formName][$enrollFormElementName] = array(
	              '#type' => 'submit',
				  '#id' => 'add_new_user_'.$uniqueId2,
	              '#prefix' => $enrollUserPrefix,
	    	      '#suffix' => '<div class="admin-save-button-right-bg"></div>'.$span_publish_button.'</div>',
	              '#title' => t('LBL305'),
	              '#value' => t('LBL640'),
	              '#name' => $buttonName,
	              '#attributes' => $setAttributes,
	              '#ajax' => array(
	                'callback' => 'addEditRepaintEnrolltoClassForm',
	                'wrapper' => $wrapperId,
	                'entity_id' => $entityId,
	                'entity_type' => $entityType,
            		'course_id' => $courseId,
            		'entity_plugin'=>'enrolluser',
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
					
					
					
					expDebug::dPrint(' $$form_state$form_state$form_state$form_state = ' . print_r($isCompliance,true),5);
					if($isCompliance!=1)
					{
					$form[$formName]['addedit-button-group-ul-start'] = array(
							'#type' => 'markup',
							'#markup' => '<ul class="catalog-pub-add-list cls-pub-save-btn">',
					); //  catalog-save-btn					
					
				 $saveButtonId = 'enroll-user-man';
      	   $enrollMandatoryAttributes['data-wrapperid'] = array($wrapperId);
      	   $enrollMandatoryAttributes['class'][] = 'addedit-form-expertusone-throbber';
          
					//PAST CLASS
           $enrollUserMandatoryAttributes['data-wrapperid'] = array($wrapperId);
           $enrollUserMandatoryAttributes['class'][] = 'addedit-form-expertusone-throbber';
           $enrollUserMandatoryAttributes['onclick'][] = 'clearMessages()';
            $enrollUserMandatoryPrefix = '<li class="save-pub-unpub-sub-menu">';
            $enrollUserMandatorySuffix = '</li>';
           if($isPastClass) {
           	$enrollUserMandatoryAttributes['class'][] = 'element-invisible';
           	$enrollUserMandatoryPrefix .= "<input data-wrapperid=\"$wrapperId\" onclick=\"$('#root-admin').data('narrowsearch').registrationDateQTip('$uniqueId2','$entityId','$isPastClass','$sessionStartdate'); $('#enroll_mandatory_hidden').val(1);clearMessages();return false;\" id=\"edit-enroll-user-past-class-button\" name=\"set-enrolluser-mandatory-button\" value=\"".t('Enroll user and set training as mandatory')."\" type=\"button\" class=\"addedit-form-expertusone-throbber form-submit\">";
//            	$enrollUserMandatorySuffix = '';
           }
						$form[$formName][$saveButtonId] = array(
								'#type' => 'submit',
								'#prefix' => $enrollUserMandatoryPrefix,
								'#suffix' => $enrollUserMandatorySuffix,
								'#value' => t('Enroll user and set training as mandatory'),
								'#name' => 'set-enrolluser-mandatory',
								'#id'=>'enroll-user-mandatory-class',
								'#attributes' => $enrollUserMandatoryAttributes,
								'#ajax' => array(
	                'callback' => 'addEditRepaintEnrolltoClassForm',
	                'wrapper' => $wrapperId,
	                'entity_id' => $entityId,
	                'entity_type' => $entityType,
            		'course_id' => $courseId,
            		'entity_plugin'=>'enrolluser',
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
					}
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
			      'callback' => 'addEditRepaintEnrolltoClassForm',
                  'entity_id' => $entityId,
                  'entity_type' => $entityType,
            	  'course_id' => $courseId,
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
			$form_state['storage']['mode'] = '';
			
			if($displayMode=='view'){
				$form[$formName]['statistics_count_action_container_end'] = array(
						'#type' => 'markup',
						'#markup' => '</div>',
				);
			}
		}
	} catch (Exception $ex) {
		watchdog_exception('displayClassRegisterDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function addEditSaveFormForEnrollUser(&$form, $formName, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $subStage = '', $entityId, $entityType, $uniqueId, $listAvailableUserResult, $clsDeliveryType = '',$courseClassStatus, $classHasContent,$btnclose=0) {
	try {
		global $theme_key;
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
		if($btnclose)
		$cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();$(".active-qtip-div").remove();'."closeQtip('','$entityId');  return false; ";
		else
		$cancelAttributes['onclick'] ='$(".active-qtip-div").remove();'."closeQtyp('qtip_editclass_visible_dispid_".core_encrypt($entityId)."','$entityId');closeQtyp('#qtipAttachIdqtip_addclass_visible_disp_".core_encrypt($entityId)."','$entityId'); return false; ";

		$uniqueId2 = $entityId.$entityType;

		$form[$formName]['attachment_link_url_disp-view'] = array(
				'#type'       => 'markup',
				'#markup'     => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-and-save-actions-row enroll-edit-save-cancel-btn">',
		);
		/* if($displayMode=='view'){
			$form[$formName]['statistics_count_action_container_end'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="clearBoth"></div> </div>',
			);
		} */
		$doneBtnAttributes                      = array();
		//$doneBtnAttributes['class'][]           = 'attach-btn-catalog-provide-url';
		$doneBtnAttributes['class'][]           = 'addedit-form-expertusone-throbber';
		$doneBtnAttributes['data-wrapperid']    = array($wrapperId);
		$doneBtnAttributes['onclick'][]         = 'return false;';
		$form[$formName]["enrollment_upload_done_btn_view"] = array(
				'#type'       => 'submit',
				'#id'         => 'enrollment_upload_done_btn_view',
				'#prefix'     => '<div style="display:none;">',
				'#suffix'     => '</div>',
				'#value'      => 'uploaduser',
				'#name'       => 'done',
				'#submit' => array('enrollment_upload'),
				'#attributes' => $doneBtnAttributes,
				'#ajax' => array(
						'wrapper'   => $wrapperId,
						'callback'    => 'enrollUserGrid',
						'entity_id' => $entityId,
						'entity_type' => $entityType,
						'method'    => 'replace',
						'effect'    => 'none', // 'fade',
						'event'     =>'click',
						'keypress'  => true,
						'progress' => array(
								'type'    => 'throbber',
								'message' => ''
						),
				),
		);

		$form[$formName][$cancelButtonId] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="addedit-form-cancel-container-actions enrolluser-save-cancel-btn-container"><div class="white-btn-bg-left"></div>',
				'#suffix' =>   '<div class="white-btn-bg-right"></div>',
				'#value' => t('LBL123'),
				'#name' => 'cancel',
				'#attributes' => $cancelAttributes,
				'#ajax' => array(
						'wrapper' => $wrapperId,
						'callback' => $ajaxCallbackCancel,
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

		// Add the Save button

          $ajaxCallbackSave = 'enrollUserGridCallback';
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

          $currentdate = date('m/d/Y');
//           $date = getClassStartDate($entityId,false);
	 				if(isset($form[$formName]['hidden_session_start_date'])){
		       $date = date('m/d/Y', strtotime($form[$formName]['hidden_session_start_date']['#value']));
          }else{
         	 $date = '';
          }
          expDebug::dPrint('session date modi'.$date);
          $date = empty($date) ? $currentdate : $date;
          // EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader($wrapperId);
          $completiondatelbl = t('LBL706');
          $closelbl = t('LBL123');
          $donelbl = t('LBL569');
          $completionContainer = <<<EOD
<div class="completion_container_roster" id="completion_date_container_$uniqueId2">
<a style="float: right; position: relative; cursor: pointer;" class="qtip-button"  onClick="$('$completionId').hide();"><div class="admin-bubble-close"> </div></a>
$completiondatelbl:  <input size="8" type="text" name="overall_completion_date_$uniqueId2" id="overall_completion_date_$uniqueId2" class="enrollement-date-picker" value="$date"/>
	<div id="enroll-user-completion-date-popup" class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span onClick="$('$completionId').hide();" class="admin-action-button-middle-bg white-btn-bg-middle">$closelbl</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg completion-button-space"></div><input type="button" class="admin-save-button-middle-bg"  value="$donelbl" onClick="$('#root-admin').data('narrowsearch').fillCompletionDate('$uniqueId', '$uniqueId2');  $('#root-admin').data('narrowsearch').showConfirmPopup('completedandsave_$uniqueId2', 'completedandsave', 'hidden_idlist_$uniqueId','Class','.$isPricedd.'); return false;"/><div class="admin-save-button-right-bg"></div></div>
</div>
EOD;


		/*  $waivedlinkHTML =
		 '<span class="enrolled-exempted-container">' .
		'<span id="enrolled-exempted-' . id . '" class="enrolled-exempted-status">' .
		'<a id="visible-exempted-'.id.'" class="tab-title" onclick = "exemptedVisibility(' . qtipOptWaivedObj . ');">' .
		status .
		'</a>' .
		'<span id="visible-popup-' . id . '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span>'.
		'</span>' .
		'</span>'; */



		$qtipOptWaivedObj  = "{'entityId' : '".$entityId."'".
		", 'entityType' : '".entityType ."'" .
		", 'url' : 'administration/enrollment/exempted/all/" . $entityId . "/0/class/0'" .
		", 'popupDispId' : 'enrolled-all-exempted-".$entityId . "'".
		", 'catalogVisibleId' : 'enrolled-all-exempted-disp'" .
		", 'wid' : 400".", 'heg' : '200'" .", 'postype' : 'topright'" .",'poslwid' :'' ".", 'qdis' : 'ctool'" .
		", 'linkid' : 'visible-all-exempted-". $entityId."','enrId' : '".entityId ."'".", 'waivedfrom' : 'all'}";

		$isPricedd = (getSltClassPrice($entityId) > 0) ? 1 : 0;
		$setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
		$isSaveParent = 1;
		$form[$formName][$saveButtonId.'-markup'] = array(
				'#type' => 'markup',
				'#prefix' => '<div class="admin-save-pub-unpub-button-container"><span id="setwaived-visible-container"><span id="enrolled-all-exempted-' . $entityId . '" class="enrolled-exempted-all-status"><a id="visible-all-exempted-'.$entityId.'" class="enrolled-all-exempted" onclick = "exemptedVisibility('.$qtipOptWaivedObj.');"></a><span id="visible-popup-' . $entityId . '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span></span></span><div class="'.$setclass.'"></div>',
				'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'commonsave_'.$saveButtonId.'\', \'commonsave\',\'hidden_idlist_'.$uniqueId.'\',\'Class\','.$isPricedd.'); return false;" type="button" name="commonsave" value="'.t('LBL141').'">',
		);
		$saveAttributes['onclick'] = array();
		$saveAttributes['onclick'][] = 'clearMessages();';
		$form[$formName][$saveButtonId] = array(
				'#type' => 'submit',
	  			//'#prefix' => '<div class="admin-save-pub-unpub-button-container"><span id="setwaived-visible-container"><span id="enrolled-all-exempted-' . $entityId . '" class="enrolled-exempted-all-status"><a id="visible-all-exempted-'.$entityId.'" class="enrolled-all-exempted" onclick = "exemptedVisibility('.$qtipOptWaivedObj.');"></a><span id="visible-popup-' . $entityId . '" class="qtip-popup-exempted" style="display:none; position:absolute; left:0px; top:0px;" ></span></span></span><div class="'.$setclass.'"></div>',
				'#prefix' => '<div style="display: none">',
      			'#suffix' => '</div><span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div></div>'.$completionContainer.'<ul class="catalog-pub-add-list">',
				'#value' => t('LBL141'),
				'#name' => 'commonsave',
				'#id' => 'commonsave_'.$saveButtonId,
				'#attributes' => $saveAttributes,
				'#validate' => array('validateRosterEnrollmentDetails'),
				'#submit' => array('updateRosterEnrollmentDetails'),
				'#ajax' => array(
						'wrapper' => $wrapperId,
						'callback' => $ajaxCallbackSave,
						'entity_id' => $entityId,
						'entity_type' => $entityType,
						// 'customButtonId'=>$esignHiddenButt,
						'method' => 'replace','effect' => 'none', // 'fade',
						'event' => 'click','keypress' => true,
						'progress' => array(
						'type' => 'throbber',
						'message' => ''
								)
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

  	        // #0016788 - Classroom Session should not be able to mark complete for future sessions - Implemented
  	        $ses_start   = getClassSessionStartDate($entityId);
  	        $tz = $ses_start[0]->timezone_attr;
  	        if(!$tz)
  	        $tz = date_default_timezone(FALSE);
  	        $sessionTZ = new DateTimeZone($tz);
  	        $strtotimeNow = new DateTime(null, $sessionTZ);
  	        $stDate = date_format(date_create($ses_start[0]->ses_start_date. " " .$ses_start[0]->start_time),'Y-m-d H:i:sP');
  	        $ses_start_date = new DateTime($stDate, $sessionTZ);
  	        expDebug::dPrint("SESSION TIME CHECK - Current Date -- ".$strtotimeNow->format('Y-m-d H:i:sP')."  ----- Session Start Date ".$ses_start_date->format('Y-m-d H:i:sP'),4);
  	        if($strtotimeNow >= $ses_start_date) {

  	        	$saveButtonId = 'catalog-enrolluser-complete-save';
  	        	$attributes['onclick'] = array();
  	        	$saveAttributes['onclick'][] = 'clearMessages(); return false;';
  	        	$sessionStartdate = $form[$formName]['hidden_session_start_date']['#value'];
  	        	$form[$formName][$saveButtonId] = array(
          '#type' => 'markup',
          '#prefix' => '<li class="save-pub-unpub-sub-menu completedandsave" onClick="$(\'#root-admin\').data(\'narrowsearch\').completionDateQTip(\''.$uniqueId2.'\',\''.$entityId.'\',\''.$sessionStartdate.'\'); return false;">',
          '#suffix' => '</li>',
          '#markup' => "<input id='completedandsave_trigger' class='form-submit ajax-processed' value='".t('LBL695')."'/>",
  	        	);

  	        	$form[$formName][$saveButtonId.'-markup'] = array(
  	        			'#type' => 'markup',
  	        			'#prefix' => '<li class="save-pub-unpub-sub-menu incompletedandsave">',
  	        			'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'incompletedandsave_'.$uniqueId2.'\', \'incompletedandsave\', \'hidden_idlist_'.$uniqueId.'\',\'Class\','.$isPricedd.'); return false;" type="button" name="incompletedandsave" value="'.t('LBL815').'">',
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
  	      '#validate' => array('validateRosterEnrollmentDetails'),
          '#submit' => array('updateRosterEnrollmentDetails'),
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
      '#validate' => array('validateRosterEnrollmentDetails'),
      '#submit' => array('updateRosterEnrollmentDetails'),
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


		$qtipId = 'waivedandsave_'.$uniqueId2;
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
				'#validate' => array('validateRosterEnrollmentDetails'),
      '#submit' => array('updateRosterEnrollmentDetails'),
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
          		'#validate' => array('validateRosterEnrollmentDetails'),
				'#submit' => array('updateRosterEnrollmentDetails'),
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
          		'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'cancelledandsave_'.$uniqueId2.'\', \'cancelledandsave\', \'hidden_idlist_'.$uniqueId.'\',\'Class\','.$isPricedd.'); return false;" type="button" name="cancelledandsave" value="'.t('LBL659').'">',
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
	  '#validate' => array('validateRosterEnrollmentDetails'),
      '#submit' => array('updateRosterEnrollmentDetails'),
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


		$mcCnt = getCountofMandatoryCompliance($entityId,'class');
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
			
		$count = getClassExmeptedorNot('class',$entityId);
		
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
			
		    $qtipId = 'unwaivedandsave_'.$uniqueId2;
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
					'#validate' => array('validateRosterEnrollmentDetails'),
					'#submit' => array('updateRosterEnrollmentDetails'),
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
		$form[$formName][$saveButtonId.'-markup'] = array(
				'#type' => 'markup',
				'#prefix' => '<li class="save-pub-unpub-sub-menu noshowandsave">',
				'#markup' => '<input class="addedit-edit-save-button edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed" onclick="clearMessages(); $(\'#root-admin\').data(\'narrowsearch\').showConfirmPopup(\'noshowandsave_'.$uniqueId2.'\', \'noshowandsave\', \'hidden_idlist_'.$uniqueId.'\',\'Class\','.$isPricedd.'); return false;" type="button" name="noshowandsave" value="'.t('LBL697').'">',
		);
          if(($strtotimeNow >= $ses_start_date) && ($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl')){
          	$qtipId = 'noshowandsave_'.$uniqueId2;
          	$saveButtonId = 'catalog-enrolluser-noshow-save';
          	$saveOtherAttributes['onclick'] = array();
          	$saveOtherAttributes['onclick'][] = 'clearMessages();';
          	$form[$formName][$saveButtonId] = array(
        '#type' => 'submit',
        '#prefix' => '<div style="display: none">',
        '#suffix' => '</div></li>',
        '#value' => t('LBL697'),
        '#name' => 'noshowandsave',
		'#id' => $qtipId,
        '#attributes' => $saveOtherAttributes,
        '#submit' => array('updateRosterEnrollmentDetails'),
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
          $showEnrollUser = 1;
          if(($clsDeliveryType == 'lrn_cls_dty_ilt' || $clsDeliveryType == 'lrn_cls_dty_vcl')){
          	include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_session_details.inc');
          	$cls_seat_info = _getClassInfo($entityId);
          	$max_seats = $cls_seat_info[0]->max_seats;
          	$waitlist_count = $cls_seat_info[0]->waitlist_count;
          	$totAvailableSeats = $max_seats + $waitlist_count;
          	$totEnroll = getTotalClassEnroll($entityId);
          	$showEnrollUser = ($totAvailableSeats > $totEnroll)? 1 : 0;
          }

          if ($entityType == 'cre_sys_obt_cls') {
          	$classValidRec = checkClassValidity($entityId, $clsDeliveryType);
          }
          expDebug::dPrint(' Calss Status = '.print_r($classValidRec,true),5);
          expDebug::dPrint(' Class Status = '.$classValidRec->classstatus,5);
          $isPricedClass = getSltClassPrice($entityId);
          if($isPricedClass == 0) {
          	expDebug::dPrint('price is zero');
          }
          
          if ($listAvailableUserResult > 0 && $classHasContent && $showEnrollUser && $classValidRec->classstatus != 'lrn_cls_sts_can' && $isPricedClass == 0 ) {
          	$saveOtherAttributes['onclick'] = array();
          	$saveButtonId = 'catalog-enrolluser-newuser';
          	$checkDedicated = is_dedicated($entityId);
          	expDebug::dPrint("soun entity id".$checkDedicated,5);
          	if(empty($checkDedicated)) {
          	$form[$formName][$saveButtonId]['add_enrolluser_view_mode'] = array(
          '#type' => 'submit',
          '#title' => t('LBL361'),
          '#prefix' => '<li class="save-pub-unpub-sub-menu">',
          '#suffix' => '</li>',
          '#value' => t('LBL640'),
          '#name' => 'Add enrolluser',
          '#submit' => array('add_enrolluser'),
          '#attributes' => $saveOtherAttributes,
        	'#ajax' => array(
            'wrapper' => $wrapperId,
            'callback' => 'enrollUserGrid',
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
          }
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
          //	'#prefix' => '<script language="Javascript" type="text/javascript">selecteDropdown()</script>',
          );
          /*
           $form[$formName][''] = array(
           '#prefix' => '<img src=\'themes/seven/images/buttons.png\' width="0" height="0" onload="$(\'#root-admin\').data(\'narrowsearch\').completionDateQTip(\''.$qtipId.'\');" />',
           );
           */
      /*  if($displayMode=='view'){
           $form[$formName]['statistics_count_action_container_end'] = array(
           		'#type' => 'markup',
           		'#markup' => '</div>',
           );
       } */
          
	} catch (Exception $ex) {
		watchdog_exception('addEditSaveFormForEnrollUser', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getEnrolledUserAutoComplete() - Returns the matched person names that are enrolled to a class
 */
function getEnrolledUserAutoComplete(){
	try{
		$nameSubstr = $_GET['z'];

		$entityId   = orgValidate($_GET['entity_id']);
		$entityType = $_GET['entity_type'];
		$searchType = $_GET['search_type'];

		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0){
		$courseDet = db_query("select course_id from slt_course_class WHERE id = '".$entityId."'");
		$courseDet = $courseDet->fetchAll();
		$courseId  = $courseDet[0]->course_id;

		// Select tables
		$select = db_select('slt_enrollment', 'enroll');
		$select->join('slt_person', 'per', 'per.id = enroll.user_id');

		// Set conditions
		$select->condition('enroll.course_id', $courseId, '=');
		$select->condition('enroll.class_id', $entityId, '=');
		if($searchType == 'user' || $searchType == ''){
			if(!empty($nameSubstr)){
				//Modified for the ticket no:18743
				$select->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like');
			}
		}else if($searchType == 'fullname'){
			//Modified for the ticket no:0029263
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like');
		}
		elseif($searchType == 'status'){
			
			$waivedcnt = 0;
			if(mb_stripos(t('Waived'),$nameSubstr,null,'UTF-8') !== false) {
				$qry = db_select('slt_enrollment','enr');
				$qry->innerJoin("slt_enrollment_exempted",'exmp',"exmp.enrollment_id = enr.id and exmp.enroll_type='class'");
				$qry->addExpression("COUNT(1)",'waivedcnt');
				$qry->condition("exmp.exempted_status",1,'=');
				$qry->condition(db_or()->condition("enr.mandatory","","IS NULL")->condition("enr.is_compliance","","IS NULL"));
				$qry->condition('enr.course_id', $courseId, '=');
				$qry->condition('enr.class_id', $entityId, '=');
				$qry->condition("enr.comp_status",array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
				$qry->groupBy('exmp.enrollment_id');
				expDebug::dPrintDBAPI("ADFSF AGT EQTQEQET",$qry);
				$waivedcnt = $qry->execute()->fetchField();
				expDebug::dPrint("DSAFDSATFGE".print_r($waivedcnt,1));
			}
			
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
			} else if(mb_stripos(t('No Show'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'No Show';
			} else if(mb_stripos(t('Pending'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Pending';
			} else if(mb_stripos(t('Confirmed'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Confirmed';
			} else if(mb_stripos(t('Reserved'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Reserved';
			} else if(mb_stripos(t('Pending Approval'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Pending Approval';
			} else if(mb_stripos(t('Attended'),$nameSubstr,null,'UTF-8') !== false) {
				$nameSubstr = 'Attended';
			}
			$select->leftjoin('slt_profile_list_items', 'spli', 'spli.code = enroll.reg_status OR spli.code = enroll.comp_status');
			$select->addExpression('DISTINCT(spli.name)', 'name');
			$select->condition('spli.is_active', 'Y');
			$select->condition('spli.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->condition(db_or()->condition('spli.code', '%' . db_like('lrn_crs_reg_') . '%' ,'like')
			->condition('spli.code', '%' . db_like('lrn_crs_cmp_') . '%' ,'like'));

		}
		elseif($searchType == 'org'){
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
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grpmap.entity_id = enroll.class_id and grpmap.entity_type = \'cre_sys_obt_cls\'');
			$select->leftjoin('slt_groups', 'grps', 'grps.id = grpmap.group_id');
			$select->addExpression('DISTINCT(grps.name)', 'name');
			$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
			$select->condition('grps.is_admin',0,'=');
			
			//Added for #0056922
			$matchingQuery1 = $select->execute()->fetchAll();
			expDebug::dPrint('The $matchingQuery value is here '.print_r($matchingQuery1[0]->name,1),2);
			if($matchingQuery1[0]->name == ''){
				$select = db_select('slt_enrollment', 'enroll');
				$select->join('slt_person', 'per', 'per.id = enroll.user_id');
				$select->condition('enroll.course_id', $courseId, '=');
				$select->condition('enroll.class_id', $entityId, '=');
				$select->leftjoin('slt_groups', 'grps', '1=1');
				$select->addExpression('DISTINCT(grps.name)', 'name');
				$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
				$select->condition('grps.is_admin',0,'=');
			}
			$select->condition('grps.status','cre_sec_sts_atv','=');
			$select->orderBy('name');
			expDebug::dPrintDBAPI(' $select SQL  = enrollment ' , $select);
		}
		$select->condition('enroll.reg_status',
		array('lrn_crs_reg_can', 'lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_wtl', 'lrn_crs_reg_rsv', 'lrn_crs_reg_ppv'), 'IN');
		if($searchType == 'user'  || $searchType ==t('LBL107') || $searchType == ''){
			// Fields to fetch
			$select->addExpression('DISTINCT(per.user_name)', 'name');
			$select->orderBy('name');
		}

		expDebug::dPrintDBAPI(' $select SQL = enrollment ' , $select);
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per');
		$matchingQuery = $select->execute()->fetchAll();
		expDebug::dPrint(' $matchingPersons = '.print_r($matchingQuery, true),3);
		}
		else {
		    require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
		    try{
		        $solrObj = new UserEnrollments();
	        	$waivedcnt = 0;
	        	$matchingQuery1 = $solrObj->classEnrollments($entityId,$entityType,$nameSubstr,'autocomplete',$enrType='class');
	        	$matchingQuery3 = $matchingQuery1['records'];
	        	
	        	expDebug::dPrint("waivd count checccc --".print_r($matchingQuery3 ,1),5);
	        	$stsArray=array();
        		foreach ($matchingQuery3 as $result) {
        			$groupNameArray = explode(",", $result->name);
        			foreach($groupNameArray as $GroupName){
        				$pos = strpos(strtolower($GroupName), $nameSubstr);
        				if($pos !== false){
        				array_push($stsArray, str_replace('`','',$GroupName));
        				}
        			}
        		}
        		$stsArray = array_unique($stsArray);
        		$matchingQuery = $stsArray;
        		if(mb_stripos(t('Waived'),$nameSubstr,null,'UTF-8') !== false) {
        			$waivedcnt = 1;
        		}
		    }catch(Exception $e){
		        expDebug::dPrint("Error in my program Solr Search -- ".print_r($e->getMessage(),true),1);
		    }
		    	
		}
		
		if($searchType == 'status' && $waivedcnt)
			print 'Waived'."\n";
		foreach ($matchingQuery as $result) {
			if($srcSource == 0){
				if($searchType == 'status') {
					print t($result->name) . "\n";
				} else{
					print $result->name . "\n";
				}
			} else {
				
				if($searchType == 'status') {
					if(empty($waivedcnt)) {
						print t($result). "\n";
					}
				}else
					print $result. "\n";
			}
		}
	}
	catch (Exception $ex) {
		watchdog_exception('getEnrolledUserAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getNotEnrolledUserAutoComplete() - Returns the matched person names that can be enrolled to a class and are not already enrolled
 */
function getNotEnrolledUserAutoComplete() {
	try{
		$entityId   = $_GET['entity_id'];
		$entityType = $_GET['entity_type'];
		$searchType = $_GET['search_type'];
		$nameSubstr = $_GET['z'];
		$userId = getIdOfLoggedInUser();

		$runQuery = 1;
		$srcSource = getConfigValue('solr_search');
		// Solr implementation
		if($srcSource == 0){
		if($searchType == 'user' || $searchType == ''){
			// Select the table
			$select = db_select('slt_person', 'per');

			// Add fields to fetch
			$select->addExpression('DISTINCT(per.user_name)', 'name');

			// Add conditions
			if(!empty($nameSubstr)){
				//Modified for the ticket no:18743
				$select->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like');
			}

			$chkEnrollments = true;
			if($searchType == 'user' && $entityType == 'cre_sys_obt_cls')
			{
				$select2 = db_select('slt_business_rule_mapping','busrule');
				$select2->addField('busrule','entity_id');
				$select2->condition('busrule.entity_id',$entityId,'=');
				$select2->condition('busrule.business_rule_code','cre_sys_brl_rra','=');
				$rereg = $select2->execute()->fetchField();
				//$result2 = $select2->execute();
			//	$row_cnt = $result2->rowCount();
				if($row_cnt > 0)
					$chkEnrollments = false;
			}

		//	if($chkEnrollments)
		//	{
				
				$courseDet=db_query("select course_id,is_compliance,delivery_type from slt_course_class WHERE id = '".$entityId."'");
				$courseDet = $courseDet->fetchAll();
				$courseId = $courseDet[0]->course_id;
				
				$is_compliance = ($courseDet[0]->is_compliance == 1) ? $courseDet[0]->is_compliance : 0;
				
				if($is_compliance == 1){
					$select1 = db_select('slt_enrollment','enr');
					$select1->addField('enr','user_id');
					$select1->condition('enr.class_id',$entityId,'=');
					$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
					$select1->condition('enr.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_exp'),'NOT IN');
					$select1->where('enr.user_id = per.id');
					$select1->leftJoin('slt_course_template','crs', 'crs.id=enr.course_id');
					$select1->innerJoin('slt_notification_info','notify', 'notify.notification_code=\'compliance_expiry_remainder\' ');
				if(!empty($rereg) && ($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod')){
				$select1->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');	
			}else{
			$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
			$select1->condition('enr.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_exp'),'NOT IN');
				}
					$select1->where('if(enr.comp_status= \'lrn_crs_cmp_cmp\' AND enr.is_compliance = 1 AND (crs.validity_date IS NOT NULL OR crs.validity_days IS NOT NULL),
  			if(((crs.validity_date IS NOT NULL and (curdate() > crs.validity_date)) OR (crs.validity_days IS NOT NULL and (curdate() > DATE_ADD(enr.comp_date, interval crs.validity_days day)) )),1=2,
  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
  			(DATE_SUB(crs.validity_date, interval notify.notify_before day)))) < 0)),1=1)');
				}else{
					$select1 = db_select('slt_enrollment','enr');
					$select1->addField('enr','user_id');
					$select1->condition('enr.class_id',$entityId,'=');
					$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
				if(!empty($rereg) && ($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod')){
				$select1->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');	
			}else{
			$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
			$select1->condition('enr.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_exp'),'NOT IN');
				}
					$select1->where('enr.user_id = per.id');
				}
				$select->notExists($select1);
		//	}
//
			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			//if($userId != 1)
			//filterByGroupAccessPrivileges($select,$entityId,'class');
			$select->orderBy('name');
		}elseif($searchType == 'fullname'){
			$select = db_select('slt_person', 'per');
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like');
			if (!empty($excludedUserId)) {
				$select->condition('per.id', $excludedUserId, 'NOT IN');
			}

			$select->condition('per.full_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
			//if($userId != 1)
			//filterByGroupAccessPrivileges($select,$entityId,'class');
			$select->orderBy('name');
		}
		elseif($searchType == 'org'){
			$select = db_select('slt_organization', 'org');
			$select->leftjoin('slt_person', 'per', 'per.org_id = org.id');
			$select->addExpression('DISTINCT(org.name)', 'name');
			$select->condition('org.status', array('cre_org_sts_itv','cre_org_sts_act'), 'IN');
			$select->condition('org.name', '%' . db_like($nameSubstr) . '%', 'LIKE');
			$select->orderBy('name');
		}
		elseif($searchType == 'jobrole'){
			$select = getMatchingProfileItemNamesKeyedautocomplete($_GET['z'], 'cre_usr_jrl_');
			$select->leftjoin('slt_person_jobrole_mapping', 'permap', 'permap.job_role = spli.code');
			$select->leftjoin('slt_person', 'per', 'per.id = permap.user_id');
			filterByUserAccessPrivileges($select, 'spli','',1,'cre_usr_jrl','','spli');
		}
		elseif($searchType == 'usertype'){
			$select = getMatchingProfileItemNamesKeyedautocomplete($_GET['z'], 'cre_usr_ptp_');			
			$select->leftjoin('slt_person', 'per', 'per.user_type = spli.code');
			$runQuery = 1;
			}
		elseif($searchType == 'manager'){
			$select = db_select('slt_person', 'per');
			$select->addExpression('DISTINCT(per.full_name)', 'name');
			$select->condition(db_or()->condition('per.user_name', '%' . db_like($nameSubstr) . '%' ,'like')
			->condition('per.full_name', '%' . db_like($nameSubstr) . '%' ,'like'));
			$select->condition('per.user_name', array('admin','guest'),'NOT IN');
			$select->condition('per.status', "cre_usr_sts_atv");
			//$select->condition('per.is_manager', "1");
    		$select->where('per.is_manager=\'Y\'');
			$select->orderBy('name');
			expDebug::dPrintDBAPI('The Jooooooooe--->' , $select);		
		}
		elseif($searchType == 'group'){
			$select = db_select('slt_groups', 'grps');
			$select->leftjoin('slt_group_mapping', 'grpmap', 'grps.id = grpmap.group_id');
			$select->addExpression('DISTINCT(grps.name)', 'name');
			$select->condition('grps.name', '%' . db_like($nameSubstr) . '%' ,'like');
			$select->condition('grpmap.entity_type','cre_sys_obt_cls','=');
			$select->condition('grpmap.entity_id',$entityId,'=');
			$select->condition('grps.is_admin',0,'=');
			$select->orderBy('name');

			$matchingQuery1 = $select->execute()->fetchAll();
			expDebug::dPrint('The $matchingQuery1 value is here for the '.print_r($matchingQuery1,1),2);
			if($matchingQuery1[0]->name == '')
			{
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
	/*	if($searchType != 'group'){
			$userId = getIdOfLoggedInUser();		
			if($userId != 1)
				filterByGroupAccessPrivileges($select,$entityId,'class');
		}*/
			if($searchType == 'user' || $searchType == '' || $searchType == 'fullname'){
				$select->leftjoin('slt_course_class', 'class', "class.id = $entityId");
				$select->leftjoin('slt_course_template', 'course', "course.id = class.course_id AND course.compliance_completed IN('hire_days','date')");
				//$select->where('(IF(course.id IS NOT NULL AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),1=1))');
				$select->where('(IF(course.compliance_completed = "hire_days" AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),IF(course.complete_date IS NOT NULL,course.complete_date >= CURDATE(),1=1)))');
			}
			if($runQuery){
			    expDebug::dPrintDBAPI(' $select SQL = ' , $select);
			
			    $matchingQuery = $select->execute()->fetchAll();
			    expDebug::dPrint(' $matchingPersons$matchingQuery = '. print_r($matchingQuery, true),3);
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
			        $search_results = $solrObj->getSolrSearchResultEnrollUserAutoComplete($searchType,$nameSubstr,$entityId,$enrPage='class');
			        expDebug::dPrint('Result set from solr main result'.print_r($search_results,true),5);
			        $matchingQuery = $search_results['records'];
			        foreach($matchingQuery as $key => $val){
			        	$groupNameArray = explode(",", $val -> name);
			        	foreach($groupNameArray as $GroupName){
			        		
			        		$pos = strpos(strtolower($GroupName), $nameSubstr);
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
		}	
	catch (Exception $ex) {
		watchdog_exception('getNotEnrolledUserAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}


function loadCtoolDataGridForEnrollUser($searchKeyword, $queryRequired = '', $entityId = '', $countRequired = '',$is_compliance='',$deliveryType='',$skipAccess=''){
	try {
		$searchType = $_GET['searhType'];
		
		$sord = $_GET['sord'];
		$select = db_select('slt_person', 'per');
		if($countRequired){
			$select->addExpression('count(DISTINCT(per.id))', 'count');
		} else {		
			$select->addField('per', 'id', 'id');
			$select->addField('per', 'user_name', 'user_name');
			$select->addField('per', 'full_name', 'full_name');
			$select->addExpression('(select mgr.full_name from slt_person mgr where mgr.id = per.manager_id)', 'manager_name');
   			$select->addExpression('(select name from slt_organization where id = per.org_id)', 'organization_name');
		}
		$select->leftjoin('slt_course_class', 'class', "class.id = $entityId");
		$select->leftjoin('slt_course_template', 'course', "course.id = class.course_id AND course.compliance_completed IN ('hire_days','date')");
		//$select->where('(IF(course.id IS NOT NULL AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),1=1))');
		$select->where('(IF(course.compliance_completed = "hire_days" AND per.hire_date IS NOT NULL,(DATE_ADD(DATE_FORMAT(per.hire_date, "%Y-%m-%d"), INTERVAL course.complete_days DAY) >= CURDATE()),IF(course.complete_date IS NOT NULL,course.complete_date >= CURDATE(),1=1)))');
		$select->condition('per.status', 'cre_usr_sts_atv');
		$select->condition('per.user_name',array('admin','guest'),'NOT IN');
		$searchTypeVal = t('LBL181');
		if($searchType == 'user'){
			$searchTypeVal = t('LBL181');
		}else if($searchType == 'fullname'){
			$searchTypeVal = t('LBL036'). ' ' .t('LBL107');
		}
		elseif($searchType == 'org'){
			$searchTypeVal = t('LBL036').' '.t('LBL137');
		}
		elseif($searchType == 'jobrole'){
			$searchTypeVal = t('LBL036') . ' ' .t('LBL133').' '. t('LBL107');//t('LBL133');
		}
		elseif($searchType == 'usertype'){
			$searchTypeVal = t('LBL036').' '.t('LBL173');
		}
		elseif($searchType == 'manager'){
			$searchTypeVal = t('LBL036') . ' ' .t('LBL134') ;//t('LBL180');
		}
		elseif($searchType == 'status'){
			$searchTypeVal = t('LBL036').' '.t('LBL102');
		}

		if($searchType == 'user' || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
			if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal) {
				$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			}
		}elseif($searchType == 'fullname'){
			$select->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
		}
		elseif($searchType == 'org'){
			$select->leftjoin('slt_organization','org','org.id = per.org_id');
			$select->condition('org.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
		}
		elseif($searchType == 'jobrole'){
			$select->leftjoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
			$select->leftjoin('slt_profile_list_items','splt','splt.code = spjm.job_role');
			//$select->leftjoin('slt_profile_list_items','splt','splt.code = per.job_role');
			$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$select->condition('splt.code',  db_like('cre_usr_jrl_') . '%', 'LIKE');
		}
		elseif($searchType == 'usertype'){
			$select->leftjoin('slt_profile_list_items','splt','splt.code = per.user_type');
			$select->condition('splt.name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$select->condition('splt.code',  db_like('cre_usr_ptp_') . '%', 'LIKE');
		}
		elseif($searchType == 'manager'){
			$select->leftjoin('slt_person','pers1','pers1.id = per.manager_id');
			$select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
		}
		elseif($searchType == 'group' && $searchKeyword != t('LBL1270')){ // Type a group name
			$select->join('slt_groups', 'grp','grp.status=\'cre_sec_sts_atv\'');
			$select->leftjoin('slt_group_attributes', 'grpatt','grpatt.group_id = grp.id');
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
		$userId = getIdOfLoggedInUser();
	
  /*radio button based displayed group/open users*/
	 /* if($skipAccess=='group'){	
	  	filterByGroupAccessPrivilegesUsingFlatTable($select,$entityId,'class');
	  }else if($skipAccess == 'open'){
				expDebug::dPrint('open isue true');
			$select->where('per.id not in (select user_id from slt_group_mapping map
			join slt_group_user_mapping gumap on gumap.group_id =map.group_id and map.group_type = gumap.group_type
			where map.group_type =0 and map.entity_id ='.$entityId.' and map.entity_type IN ( "cre_sys_obt_cls","cre_sys_obt_crs" )) ');
		} 
		*/
		if($skipAccess == 'true'){
			expDebug::dPrint('open isue true');
			$select->where('per.id not in (select gumap.user_id from slt_group_mapping map
			join slt_group_user_mapping gumap on gumap.group_id =map.group_id and map.group_type = gumap.group_type and gumap.user_type != \'R\'
			where map.group_type =0 and map.entity_id ='.$entityId.' and map.entity_type IN ( "cre_sys_obt_cls","cre_sys_obt_crs" )
			UNION ALL
			select map.user_id from slt_user_access_mapping map where map.entity_id ='.$entityId.' and map.entity_type IN ( "cre_sys_obt_cls","cre_sys_obt_crs" )) ');
			/* 	select user_id from slt_group_mapping map
			 join slt_group_user_mapping gumap on gumap.group_id =map.group_id and map.group_type = gumap.group_type
			where map.group_type =0 and map.entity_id =244 and map.entity_type IN ( "cre_sys_obt_cls" ) */
		}
		/*$groupids = getGroupidByEntityid($entityId,'cre_sys_obt_cls');
		if($groupids){
				$select->join('slt_groups','grp');
				$select->join('slt_profile_list_items','pli','per.status=pli.code');
				$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
				$select1=array();
				$select1[] = ("if(grp.org_id='All',per.org_id is not null,FIND_IN_SET(per.org_id,grp.org_id)>0)");
				$select1[] = ("if(grp.user_type='All',per.user_type is not null,FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 )");
				$select1[] = ("if(grp.employment_type='All',per.employment_type is not null,FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0)");
				$select1[] = ("if(grp.country='All',per.country is not null,FIND_IN_SET(ifnull(per.country,''),grp.country)>0)");
				$select1[] = ("if(grp.state='All',per.state is not null,FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0)");
				$select1[] = ("if(grp.department='All',per.dept_code is not null,FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0)");
				$select1[] = ("if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)");
				$select1[] = ("if(grp.language='All',per.preferred_language is not null,FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0)");
				$select1[] = ("ifnull(per.is_manager,'N') = grp.is_manager");
				$select1[] = ("ifnull(per.is_instructor,'N') = grp.is_instructor");
				$cond = implode(' AND ',$select1);
				$select->where("((".$cond." AND ((grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))) OR (FIND_IN_SET(per.id,grp.added_users)>0))");
				$groupidArray = explode(",", $groupids);
				expDebug::dPrint("sdfdsfds".print_r($groupidArray,true),5);
				$select->condition('grp.id',$groupidArray,'IN');
				$select->groupby('per.id');
		}
		*/
		
		//filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');		
		//if($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod'){
			$select2 = db_select('slt_business_rule_mapping','busrule');
			$select2->addField('busrule','entity_id');
			$select2->condition('busrule.entity_id',$entityId,'=');
			$select2->condition('busrule.business_rule_code','cre_sys_brl_rra','=');
			$rereg = $select2->execute()->fetchField();
			expDebug::dPrintDBAPI('$select222', $select2);
			expDebug::dPrint('$schdPrevStatuses check status' . print_r($rereg, true));
			//$select1->notExists($select2);
	//	}
		if($is_compliance == 1){
			$select1 = db_select('slt_enrollment','enr');
			$select1->addField('enr','user_id');
			$select1->condition('enr.class_id',$entityId,'=');
			$select1->where('enr.user_id = per.id');
			$select1->leftJoin('slt_course_template','crs', 'crs.id=enr.course_id');
			$select1->innerJoin('slt_notification_info','notify', 'notify.notification_code=\'compliance_expiry_remainder\' ');
			if(!empty($rereg) && ($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod')){
				$select1->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');	
				
			}else{
			$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
			$select1->condition('enr.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_exp'),'NOT IN');
				}
			$select1->where('if(enr.comp_status= \'lrn_crs_cmp_cmp\' AND enr.is_compliance = 1 AND (crs.validity_date IS NOT NULL OR crs.validity_days IS NOT NULL),
  			if(((crs.validity_date IS NOT NULL and (curdate() > crs.validity_date)) OR (crs.validity_days IS NOT NULL and (curdate() > DATE_ADD(enr.comp_date, interval crs.validity_days day)) )),1=2,
  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
  			(DATE_SUB(crs.validity_date, interval notify.notify_before day)))) < 0)),1=1)');	
		}else{
			$select1 = db_select('slt_enrollment','enr');
			$select1->addField('enr','user_id');
			$select1->condition('enr.class_id',$entityId,'=');
			if(!empty($rereg) && ($deliveryType == 'lrn_cls_dty_wbt' || $deliveryType == 'lrn_cls_dty_vod')){
				$select1->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
			}else{
			$select1->condition('enr.reg_status','lrn_crs_reg_can','!=');
			$select1->condition('enr.comp_status',array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_exp'),'NOT IN');
		}
			
			$select1->where('enr.user_id = per.id');
		}
		$select->notExists($select1);
		//filterByUserAccessPrivileges($select,'per','',1,'cre_usr','adminroster','per');
		//Added by Vincent on 20 Jan, 2017 for #0071187
		filterByUserAccessPrivilegesUsingFlatTable($select,'per','',1,'cre_usr','adminroster','per');
		if (!$countRequired) {
			// asc and desc for Enroll User => username In admin Roster #0038272
			if($sord == 'desc'){
				$select->orderBy('per.full_name', 'DESC');
			}else{
				$select->orderBy('per.full_name');
			}
		}
		$select->distinct();

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
		watchdog_exception('loadCtoolDataGridForEnrollUser', $ex);
		expertusErrorThrow($ex);
	}
}


function fetchEnrollUserListStatusCount($selectQry,$maxSeats,$courseId,$entityId,$mode){
	try {
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0) {
		$select = db_select('slt_enrollment','enroll');
		$select->join('slt_person', 'per', 'per.id = enroll.user_id');
		$select->addField('enroll', 'reg_status', 'reg_status');
		$select->addExpression('count(enroll.reg_status)', 'reg_status_count');
		$select->addField('enroll', 'comp_status', 'comp_status');
		$select->addExpression('count(enroll.comp_status)', 'comp_status_count');
		$select->condition('enroll.course_id',$courseId,'=');
		$select->condition('enroll.class_id',$entityId,'=');
		$select->condition('enroll.reg_status',array('lrn_crs_reg_can','lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl','lrn_crs_reg_rsv','lrn_crs_reg_ppv'), 'IN');
		$select->groupby('enroll.reg_status');
		$select->groupby('enroll.comp_status');
		expDebug::dPrintDBAPI('fetchEnrollUserListStatusCount edit select', $select);
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','','per');
		$resultSet = $select->execute()->fetchAll();
		} else {
			require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try {
				$solrObj = new UserEnrollments();
				$listqueryResult = $solrObj->classEnrollments($entityId);
				expDebug::dPrint( "Error in class enrollment count -".print_r($listqueryResult,true), 5);
				$resultSet = $listqueryResult['result'];
				$statusCount = $listqueryResult['statusCount'];
				expDebug::dPrint( "Error in class enrollment count status count-".print_r($statusCount,true), 5);
			} catch (Exception $e) {
				expDebug::dPrint( "Error in class enrollment count - " . print_r($e->getMessage(), true), 1);
			}			
		}
		
		$statisticsArray = array();
		if($srcSource == 1) {
			if($statusCount['Enrolled'] != 0)
				$statisticsArray[0] = t('Enrolled').': '.$statusCount['Enrolled'];
			if($statusCount['Completed'] != 0)
				$statisticsArray[1] = t('Completed').': '.$statusCount['Completed'];
			if($statusCount['In progress'] != 0)
				$statisticsArray[2] = t('In progress').': '.$statusCount['In progress'];
			if($statusCount['Canceled'] != 0)
				$statisticsArray[3] = t('Canceled').': '.$statusCount['Canceled'];
			if($statusCount['Incomplete'] != 0)
				$statisticsArray[4] = t('Incomplete').': '.$statusCount['Incomplete'];
			if($statusCount['Waitlist'] != 0)
				$statisticsArray[5] = t('Waitlist').': '.$statusCount['Waitlist'];
			if($statusCount['Pending'] != 0)
				$statisticsArray[6] = t('Pending').': '.$statusCount['Pending'];
			if($statusCount['No Show'] != 0)
				$statisticsArray[7] = t('No Show').': '.$statusCount['No Show'];
			if($statusCount['Reserved'] != 0)
				$statisticsArray[8] = t('LBL942').': '.$statusCount['Reserved'];;
		} else {
			$regStatusesList = array('lrn_crs_reg_cnf');
			$compStatusesList = array('lrn_crs_cmp_enr','lrn_crs_cmp_att');
			$enrolledSeats = getEnrollListStatusCount($entityId, $regStatusesList,$compStatusesList);
			$waitlistSeats = getEnrollListStatusCount($entityId, array('lrn_crs_reg_wtl'),null);
			
			foreach($resultSet as $rec_search_results){
				$regStatusCount = empty($rec_search_results->reg_status_count) ? 0 : $rec_search_results->reg_status_count;
				$compStatusCount = empty($rec_search_results->comp_status_count) ? 0 : $rec_search_results->comp_status_count;
				if($rec_search_results->comp_status == 'lrn_crs_cmp_enr' || $rec_search_results->comp_status == 'lrn_crs_cmp_att'){
					$statisticsArray[0] = t('Enrolled').': '.$enrolledSeats;
				}
				if($rec_search_results->comp_status == 'lrn_crs_cmp_cmp'){
					$regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm');
					$compStatusesList = array('lrn_crs_cmp_cmp');
					$completedSeats = getEnrollListStatusCount($entityId, $regStatusesList,$compStatusesList);
					$statisticsArray[1] = t('Completed').': '.$completedSeats;
				}
				if($rec_search_results->comp_status == 'lrn_crs_cmp_inp'){
					$regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm');
					$compStatusesList = array('lrn_crs_cmp_inp');
					$inprogressList = getEnrollListStatusCount($entityId, $regStatusesList,$compStatusesList);
					$statisticsArray[2] = t('In progress').': '.$inprogressList;
				}
				if($rec_search_results->reg_status == 'lrn_crs_reg_can'){
					$statisticsArray[3] = t('Canceled').': '.$regStatusCount;
				}
				if($rec_search_results->comp_status == 'lrn_crs_cmp_inc'){
					// Below code commented by Vincent on 10 Dec, 2013 for #0026273
					//$regStatusesList  = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm');
					//$compStatusesList = array('lrn_crs_cmp_inc');
					//$inCompleteList = getEnrollListStatusCount($entityId, $regStatusesList,$compStatusesList);
					$statisticsArray[4] = t('Incomplete').': '.$regStatusCount;
				}
				if($rec_search_results->reg_status == 'lrn_crs_reg_wtl'){
					$statisticsArray[5] = t('Waitlist').': '.$waitlistSeats;
				}
				if($rec_search_results->reg_status == 'lrn_crs_reg_ppm'){
					$statisticsArray[6] = t('Pending').': '.$regStatusCount;
				}
				if($rec_search_results->comp_status == 'lrn_crs_cmp_nsw'){
					// Below code commented by Vincent on 10 Dec, 2013 for #0026273
					//$regStatusesList  = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm');
					//  $compStatusesList = array('lrn_crs_cmp_nsw');
					//$noShowList = getEnrollListStatusCount($entityId, $regStatusesList,$compStatusesList);
					$statisticsArray[7] = t('No Show').': '.$regStatusCount;
				}
				if($rec_search_results->reg_status == 'lrn_crs_reg_rsv'){
					$statisticsArray[8] = t('LBL942').': '.$regStatusCount;
				}
			}
		}
		ksort($statisticsArray);
		//$countResult = 'Enrolled: '.$enrolledCnt.', Completed: '.$completedCnt.', In-Progress: '.$inprogresCnt.', Incomplete: '.$incompletCount.', No Show: '.$showCount.', Waitlist: '.$waitlstCount.', Pending: '.$pendingCount.', Canceled: '.$cancelCount.'';
		$countResult = '';
		if($maxSeats){
			$countResult .= '<span>'.t('LBL106') . ': '. $maxSeats.'</span>'.'<span class="narrow-search-results-item-detail-pipe-line">|</span>';
		}
		/*if(count($statisticsArray) && $maxSeats){
		 $countResult .= '<span class="narrow-search-results-item-detail-pipe-line">|</span>';
		 }*/

		$availableSeats = getAvailableSeats($entityId);
		if($availableSeats){
			$countResult .= '<span>'.t('LBL645') . ' ' .t('LBL106') .': '. $availableSeats.'</span>';
		}
		$countStatistic = count($statisticsArray);
		if($countStatistic  &&  $availableSeats){
			$countResult .= '<span class="narrow-search-results-item-detail-pipe-line">|</span>';
		}

		if($countStatistic){
			foreach ($statisticsArray as $key=>$value) {
				$dup[] = '<span>'.$value.'</span>';
			}
			$countResult .= implode('<span class="narrow-search-results-item-detail-pipe-line">|</span>',$dup);
		}
			$countResult .= '<div class="msg-row"><span>'.t('MSG771').'</span></div>';
		
		return $countResult;
	} catch (Exception $ex) {
		watchdog_exception('fetchEnrollUserListStatusCount', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getEnrollListStatusCount() - returns count of enrollment records for a class from slt_enrollment table
 * matching the registration status in $regStatusList and complition status in $compStatusList
 */
function getEnrollListStatusCount($classId, $regStatusList,$compStatusList) {
	try{
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->join('slt_person', 'per', 'per.id = enr.user_id');
		$selectStmt->condition('enr.class_id', $classId);
		if(count($regStatusList) > 0){
			$selectStmt->condition('enr.reg_status', $regStatusList, 'IN');
		}
		if(count($compStatusList) > 0){
			$selectStmt->condition('enr.comp_status', $compStatusList, 'IN');
		}
		$selectStmt->addField('enr', 'id');
		$selectStmt->groupBy('enr.user_id');
		$selectStmt->groupBy('enr.reg_status');
		$selectStmt->groupBy('enr.comp_status');
		filterByUserAccessPrivileges($selectStmt,'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		$statusCount = $result->rowCount();
		expDebug::dPrint(' $result StatusCount = ' .$statusCount , 4);
		return $statusCount;
	}catch (Exception $ex) {
		watchdog_exception('getEnrollListStatusCount', $ex);
		expertusErrorThrow($ex);
	}
}

function fetchEnrollUserList($courseId, $entityId, $entityType, $queryRequiredOrExportSearchType = '', $searchKeyword = '', $eliminateCancel = '', $exportOption ='', $searchType = '', $mode = '',$isCount = 0,$start,$limit){
	try {
		if($exportOption == 'CSV') {
			// There seems a limit to number of page arguments that can be in hook_menu from where this function is called for export to csv.
			$searchType = $queryRequiredOrExportSearchType;
			$queryRequired = 'no';
		}
		else {
			$searchType = $_GET['searhType'];
			$queryRequired = $queryRequiredOrExportSearchType;
		}

		$select = db_select('slt_enrollment','enroll');
		$select->join('slt_person','per','per.id = enroll.user_id');
		$select->join('slt_course_class','cls','cls.id = enroll.class_id');
		$select->leftjoin('slt_order','sor','enroll.order_id = sor.id');
		$select->leftjoin('slt_profile_list_items','items_reg','items_reg.code = enroll.reg_status');
		$select->leftjoin('slt_profile_list_items','items_comp','items_comp.code = enroll.comp_status');
		$select->leftjoin('slt_profile_list_items','items_ovr','items_ovr.code = if((enroll.reg_status=\'lrn_crs_reg_cnf\'),if((enroll.comp_status=\'lrn_crs_cmp_inp\' OR enroll.comp_status=\'lrn_crs_cmp_att\'),\'lrn_crs_cmp_enr\',enroll.comp_status),enroll.reg_status)');
		if($isCount == 0){
			$select->addField('enroll','id','id');
			$select->addField('enroll','master_enrollment_id','master_enr_id');
			$select->addField('cls', 'delivery_type', 'delivery_type');
			$select->addField('enroll','user_id','registered_user_id');
			$select->addField('per','user_name','user_name');
			$select->addField('per','full_name','full_name');
			$select->addField('enroll','reg_status','reg_status');
			$select->addField('enroll','comp_status','comp_status');
			$select->addField('enroll','score','score');
			$select->addField('items_reg','name','reg_status_name');
			$select->addField('items_comp','name','comp_status_name');
			$select->addField('enroll','created_on','created_on');
			$select->addField('enroll','updated_on','updated_on');
			$select->addField('enroll','reg_status_date','reg_status_date');
			$select->addField('enroll','comp_date','comp_date');
			$select->addField('enroll','waitlist_priority','waitlist_priority');
			$select->addField('items_ovr','name','overall_status');
			$select->addField('sor','uc_order_id');
			$select->addField('enroll','is_compliance');
			$select->addField('enroll','mandatory');
			$select->addExpression('if(enroll.reg_status in (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\') and enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'), enroll.reg_status,if(enroll.reg_status=\'lrn_crs_reg_wtl\',enroll.reg_status,enroll.id))', 'reg_asign_status');
			$select->addExpression('if(enroll.comp_date is null,enroll.reg_date,enroll.comp_date)','date');
			$select->addExpression('if(items_comp.name is null,items_reg.name,items_comp.name) ','status');
			$select->addExpression('(SELECT CONCAT(exmp.exempted_status,\'|\',exmp.id) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = enroll.id AND exmp.enroll_type = \'class\' AND exmp.class_id='.$entityId.' ORDER BY id DESC LIMIT 1)', 'exempted_sts');
			$select->addExpression('IF(enroll.comp_status NOT IN(\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\'),items_ovr.name,ifnull((SELECT if((exmp.exempted_status = 1  and enroll.comp_status!=\'\') ,\'Waived\',NULL) FROM slt_enrollment_exempted exmp WHERE exmp.enrollment_id = enroll.id AND exmp.enroll_type = \'class\' AND exmp.class_id='.$entityId.' ORDER BY id DESC LIMIT 1),items_ovr.name))', 'exempted_overallstatus');		
		}else{
			$select->addField('enroll','user_id','registered_user_id');
			$select->addField('enroll','comp_status','comp_status');
			$select->addExpression('if(enroll.reg_status in (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\') and enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'), enroll.reg_status,if(enroll.reg_status=\'lrn_crs_reg_wtl\',enroll.reg_status,enroll.id))', 'reg_asign_status');	
			$select->addExpression('count(1)', 'enrollment_count');
		}
		//0082385: Canceled user records are not displayed in Class Enrollment Screen.
//$select->addExpression('if(enroll.reg_status in (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\') and enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'), (SELECT max(reg_date) FROM slt_enrollment e WHERE e.user_id = enroll.user_id and e.class_id = enroll.class_id and e.comp_status = enroll.comp_status ),if(enroll.reg_status=\'lrn_crs_reg_wtl\',(SELECT max(reg_date) FROM slt_enrollment e WHERE e.user_id = enroll.user_id and e.class_id = enroll.class_id and e.comp_status = enroll.comp_status ),reg_date))', 'maxreg_date');	
//$select->addExpression('if(((enroll.reg_status in (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\') and enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\')) OR (enroll.reg_status=\'lrn_crs_reg_wtl\')),(SELECT max(reg_date) FROM slt_enrollment e WHERE e.user_id = enroll.user_id and e.class_id = enroll.class_id and (if(enroll.reg_status =\'lrn_crs_reg_wtl\',enroll.reg_status = e.reg_status, e.comp_status = enroll.comp_status))),reg_date)', 'maxreg_date');		
		
		$select->condition('enroll.course_id',$courseId,'=');
		$select->condition('enroll.class_id',$entityId,'=');
		if($eliminateCancel){
			$select->condition('enroll.reg_status','lrn_crs_reg_can','!=');
		}

		if($_REQUEST['apiname'] == "ListRosterAPI")
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
				$select->condition('enroll.reg_date',$to_format,'<=');
			}
			else if(($date_to==  "") && (!empty($date_from))) {
				$select->condition('enroll.reg_date',$from_format,'>=');
			}
			else if(!empty($date_from) && !empty($date_to)) {
				if($from_timestamp <= $to_timestamp) {
					$select->condition('enroll.reg_date',array($from_format,$to_format),'BETWEEN' );
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
		expDebug::dPrint(' $searchKeyword = ' . $searchKeyword,4);
		$searchTypeVal = t('LBL181');
		if($searchType == 'user'){
			$searchTypeVal = t('LBL181');
		}elseif($searchType == 'fullname'){
			$searchTypeVal = t('LBL036') .' '.t('LBL107');
		}
		elseif($searchType == 'org'){
			$searchTypeVal = t('LBL036').' '.t('LBL137');
		}
		elseif($searchType == 'jobrole'){
			$searchTypeVal = t('LBL036') . ' ' .t('LBL133').' '. t('LBL107');//t('LBL133');
		}
		elseif($searchType == 'usertype'){
			$searchTypeVal = t('LBL036').' '.t('LBL173');
		}
		elseif($searchType == 'manager'){
			$searchTypeVal = t('LBL036') . ' ' .t('LBL134') ;//t('LBL180');
		}
		elseif ($searchType == 'status') {
			$searchTypeVal = t('LBL036').' '.t('LBL102');
		}
		if($searchType == 'user'  || $searchType == '' || empty($searchKeyword) || $searchKeyword == $searchTypeVal){
			if(!empty($searchKeyword) && $searchKeyword != $searchTypeVal ){
				$select->condition('per.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
				$filterArray[] = array(t('LBL054').':'.$searchKeyword);
			}
		}elseif($searchType == 'fullname') {
			$select->condition('per.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
			$filterArray[] = array(t('LBL107').':'.$searchKeyword);
		}
		elseif ($searchType == 'status') {
			$waivedCnt = 0;
			$allRegStatus = getProfileItemNamesKeyed('lrn_crs_reg_');
			expDebug::dPrint('$allRegStatus = ' . print_r($allRegStatus, true), 4);
			$searchRegStatusList = array();
			foreach ($allRegStatus as $code => $name) {
				if (mb_stripos($name, $searchKeyword, null, 'UTF-8') !== false) {
					$searchRegStatusList[] = $code;
				}
			}
			expDebug::dPrint('$searchRegStatusList = ' . print_r($searchRegStatusList, true), 4);

			$allCompStatus = getProfileItemNamesKeyed('lrn_crs_cmp_');
			expDebug::dPrint('$allCompStatus = ' . print_r($allCompStatus, true), 4);
			$searchCompStatusList = array();
			if(mb_stripos(t('Waived'),$searchKeyword,null,'UTF-8') !== false) {
				/*$searchCompStatusList[] = '';
				$searchCompStatusList[] = '';*/
				$waivedCnt = 1;
			}else{
			foreach ($allCompStatus as $code => $name) {
				if (mb_stripos($name, $searchKeyword, null, 'UTF-8') !== false) {
					$searchCompStatusList[] = $code;
				}
			}
			}
			
			expDebug::dPrint('$searchCompStatusList = ' . print_r($searchCompStatusList, true), 4);
			if(empty($waivedCnt)){
			if (empty($searchRegStatusList) && empty($searchCompStatusList)) {
				$select->where('1=0');
			}
			elseif (empty($searchCompStatusList)) {
				$select->condition('enroll.reg_status', $searchRegStatusList, 'IN');
			}
			elseif (empty($searchRegStatusList)) {
				$select->condition('enroll.comp_status', $searchCompStatusList, 'IN');
			}
			else {
				$select->condition(db_or()->condition('enroll.reg_status', $searchRegStatusList, 'IN')
				->condition('enroll.comp_status', $searchCompStatusList, 'IN'));
			}
			}else{
				$select->where('if((SELECT exm.exempted_status as exmsts from slt_enrollment_exempted exm
						where exm.enrollment_id = enroll.id
						AND enroll.comp_status IN (\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\') AND
						exm.enroll_type = \'class\' order by exm.id desc limit 1) = 1 ,1=1,1=0)');
			}

			$filterArray[] = array(t('LBL102').':'.$searchKeyword);
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
		elseif($searchType == 'manager'){
			$select->leftjoin('slt_person','pers1','pers1.id = per.manager_id');
			$select->condition(db_or()->condition('pers1.user_name', '%' . db_like($searchKeyword) . '%', 'LIKE')
			->condition('pers1.full_name', '%' . db_like($searchKeyword) . '%', 'LIKE'));
			$filterArray[] = array(t('Manager').':'.$searchKeyword);
		}elseif($searchType == 'group' && $searchKeyword != t('LBL1270')){ // Type a user name
			$select->join('slt_groups', 'grp');
			$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
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
			$filterArray[] = array(t('Group').':'.$searchKeyword);
		}

		$select->condition('enroll.reg_status',array('lrn_crs_reg_can','lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl','lrn_crs_reg_rsv','lrn_crs_reg_ppv'), 'IN');
		// Below condition modified by Vincent on 10 Dec, 2013 for #0026273
		if($mode=='edit'){
			$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
		}
		
	  //0082385: Canceled user records are not displayed in Class Enrollment Screen.
	 /* if($_REQUEST['apiname'] == "ListRosterAPI" && $_REQUEST['AllReg']==1){
				$select->groupBy('enroll.id');
		}else{
		//if($isCount == 0){ //Commented for #70595
			$select->groupBy('enroll.user_id');
			$select->groupBy('reg_asign_status');
			$select->groupBy('enroll.comp_status');
			
			$select->groupBy('enroll.reg_date having enroll.reg_date = maxreg_date ');
			//}
		} */
		if($_REQUEST['apiname'] == "ListRosterAPI"){
			if(!empty($_REQUEST['AllReg'])) {
				//$select->groupBy('enroll.user_id');
				$select->groupBy('enroll.id');
			}
		}
		//if($isCount == 0){ //Commented for #70595
		$select->groupBy('enroll.user_id');
		$select->groupBy('reg_asign_status');
		$select->groupBy('enroll.comp_status');
		
		//}    
		
		//$select->orderby('enroll.master_enrollment_id');
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','adminroster','per');
		expDebug::dPrintDBAPI(' SQL fetch enrolluserlist = ' , $select);

		if(empty($queryRequired)){
			if(is_numeric($start) && is_numeric($limit))
				$select->range($start, $limit);
			
			if ($isCount ==1){
				$select1 = db_select($select);
				$result[0] = new stdclass();
				expDebug::dPrintDBAPI(' $select1 query--> ' ,$select1->countQuery());
				$result[0]->enrollment_count= $select1->countQuery()->execute()->fetchField();
				expDebug::dPrint(' LIST $result1 = ' . print_r($result, true) , 3);
			}else{
			$result = $select->execute()->fetchAll();
			}
			expDebug::dPrintDBAPI('$select query-->= ' , $select);
			expDebug::dPrint(' LIST $result22 = ' . print_r($result, true) , 3);
			return $result;
			//return $select->execute()->fetchAll();
		} else {
			if($exportOption=='CSV'){
				$colHeadersMap = array(t('LBL054') => 'user_name',
				t('LBL107') => 'full_name',
				t('LBL102') => 'exempted_overallstatus',
				t('LBL668') => 'score',
				);


				SendNarrowSearchResultsAsCSVFile($select, $colHeadersMap, 'export', $filterArray, '', 'class_roster');
			} else{
				return $select;
			}
		}
	} catch (Exception $ex) {
		watchdog_exception('fetchEnrollUserList', $ex);
		expertusErrorThrow($ex);
	}

}

/**
 * Custom hook_csv_column_headers_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function class_roster_csv_column_headers_alter($columnHeaderMap) {
	try {
		unset($columnHeaderMap[0][t('LBL668')]); // for reorder of columns
		$columnHeaderMap[0][t('LBL042')] = 'datedefault';
		$columnHeaderMap[0][t('LBL668')] = 'score';
	}
	catch (Exception $ex) {
		watchdog_exception('class_roster_csv_column_headers_alter', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Custom hook_csv_row_data_alter function
 * Called in SendNarrowSearchResultsAsCSVFile()
 */
function class_roster_csv_row_data_alter($rowData) {
	try {
		if ($rowData[0]->comp_status == 'lrn_crs_cmp_cmp' || $rowData[0]->comp_status == 'lrn_crs_cmp_inc' ||
		$rowData[0]->comp_status == 'lrn_crs_cmp_nsw') {
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->comp_date));
			
		}
		else {
			$rowData[0]->datedefault = date('m-d-Y', strtotime($rowData[0]->reg_status_date));
		}
	}
	catch (Exception $ex) {
		watchdog_exception('class_roster_csv_row_data_alter', $ex);
		expertusErrorThrow($ex);
	}
}

function cancelEnrollUserDetails($form, &$form_state){
	try {
		// Clear enrolluser-autocomplete form textfield as switching from enroll user form to roster form - fix for ticket #0022120
		clearClassRosterEnrollUserAutocomplete($form_state);

		$form_state['storage']['mode'] = '';
		$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('cancelEnrollUserDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function search_enrolluser($form, &$form_state){

	try {

	} catch (Exception $ex) {
		watchdog_exception('search_enrolluser', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollUserGrid($form, &$form_state) {
	try {
		$formName = $form_state['form_unique_name'];
		return $form[$formName];
	} catch (Exception $ex) {
		watchdog_exception('enrollUserGrid', $ex);
		expertusErrorThrow($ex);
	}
}

function enrollUserGridCallback($form, &$form_state) {
	try {
		global $secure_entities;
		$formName = $form_state['form_unique_name'];
		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
		$entityType = $form_state['triggering_element']['#ajax']['entity_type'];
		
		unset($_SESSION['exempted-class-'.$entityId]);
		
		if (in_array($entityType, $secure_entities)) {
			$uniqueId = "enrolluser-".core_encrypt($entityId)."-$entityType"; // URL encryption
		} else {
			$uniqueId = "enrolluser-$entityId-$entityType"; // URL encryption
		}
		$commands = array();
		if(!empty($form_state['input']['hidden_idlist_'.$uniqueId])) {
			$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
		}
		// Cleared char length for the Message - fix for ticket #0035601
		// $statusMsgHTML = theme('status_messages',array('char_length' => 100));
		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true),4);
		if (!empty($statusMsgHTML)) {
			// Show the messages
			//$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
			$commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
		}
		$commands[] = exp_ctools_modal_adjust();

		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('enrollUserGridCallback', $ex);
		expertusErrorThrow($ex);
	}
}

function add_enrolluser($form, &$form_state) {
	try {
		// Clear enrolluser-autocomplete form textfield as switching from roster form form to enroll user form - fix for ticket #0022120
		clearClassRosterEnrollUserAutocomplete($form_state);

		$form_state['storage']['mode'] = 'edit';
		$form_state['rebuild'] = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('add_enrolluser', $ex);
		expertusErrorThrow($ex);
	}
}


function validateRosterEnrollmentDetails_impl ($form, &$form_state, $callFromAPI = 0) {
	try {
		include_once(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_learning.inc');
		//expDebug::dPrint('$form_state'.print_r($form_state,1), 5);
		$submitTypeArray = array(
				'cancelledandsave' => 'Canceled',
				'enrolledandsave' => 'Enrolled',
				'completedandsave' => 'Completed',
				'noshowandsave' => 'No Show',
				'incompletedandsave' => 'Incomplete',
		);

		$submitType = $form_state['triggering_element']['#name'];

		$courseId = $form_state['course_id'];
		$classId = $form_state['class_id'];
		$enCRYClassID = core_encrypt($classId);
		$date = now();
		$completionDate = '';
		$rosterDetails = array();
		$overallDetails = $form_state['input']['hidden_valuelist_enrolluser-'.$enCRYClassID.'-cre_sys_obt_cls'];
		$overallDetailsArray = explode(',', $overallDetails);
		foreach($overallDetailsArray as $overallValue){
			list($tmpEnrollId, $tmpCompletionDate) = explode('##', $overallValue);
			$rosterDetails[$tmpEnrollId] = array(
					'completion_date' => $tmpCompletionDate
			);
		}

		$overallCompletionDate = $form_state['input']['hidden_completion_date_enrolluser-'.$enCRYClassID.'-cre_sys_obt_cls'];
		$registrationDate = $form_state['input']['hidden_registration_date_enrolluser-'.$enCRYClassID.'-cre_sys_obt_cls'];
		$selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolluser-'.$enCRYClassID.'-cre_sys_obt_cls'];
		if(empty($selectedEnrolledIds) && empty($callFromAPI)){
			form_set_error('',t('ERR106'));
			$form_state['esign_show_popup']= false;
			return '';
		}
		$selectedIdsArray = explode(',', $selectedEnrolledIds);
		expDebug::dPrint('$selectedIdsArray--->'.print_r($selectedIdsArray,1),4);

		$pricedErr = 0;
		$isPricedClass = getSltClassPrice($classId);
		/*-- #36602 - Issue fix --*/
		$isCommerceEnabled =  module_exists('exp_sp_commerce');

		foreach($selectedIdsArray as $enrollId){
			$completionDate = $form_state['input']['completion_sel_date_'.$enrollId];

			$status = $form_state['input']['enrolled_status_'.$enrollId];
			if($status == t('Enrolled')){
				$pricedErr = 1;
			}
			expDebug::dPrint('$isPricedClass--->'.print_r($isPricedClass,1),4);
			expDebug::dPrint('$$isCommerceEnabled--->'.print_r($isCommerceEnabled,1),4);
			expDebug::dPrint('$$submitType--->'.print_r($submitType,1),4);
			expDebug::dPrint('$$pricedErr--->'.print_r($pricedErr,1),4);
			expDebug::dPrint('$isPricedClass--->'.print_r($form_state['input']['enrolled_status_'.$enrollId],1),4);
			expDebug::dPrint('$$enrollId--->'.print_r($enrollId,1),4);
			// Added by velu for #0023621
			if( ($isPricedClass > 0 && $isCommerceEnabled) && ($submitType=='commonsave' || $submitType=='cancelledandsave' || $pricedErr) && $form_state['input']['enrolled_status_'.$enrollId] == t('Canceled') ){
				form_set_error('',t('LBL1246'));
			}
			if($submitType == 'commonsave' || $submitType == 'waivedandsave'){
				$previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollId,'class');
				expDebug::dPrint("Waived status check".print_r($_SESSION['exempted-class-'.$classId][0],1)."---status".$status);
				if($submitType == 'commonsave' && (($status == 'Waived') 
						|| ($previous_exempted_status[0]->exempted_status == 1 && ($status == 'Enrolled' || $status == t('In progress') || $status == 'In progress')))) {
					expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
					if(empty($_SESSION['exempted-class-'.$classId][$enrollId])){
						expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
						form_set_error('', t('waived reason should not be empty'));
						return '';
					}
				} 
				if($submitType == 'waivedandsave' && $status != t('Waived') && $status != t('Enrolled') && $status != t('In progress')){
					expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
					form_set_error('', t('Cannot mark Waived status'));
					return '';
				}
				if($submitType == 'waivedandsave' && empty($_SESSION['exempted-class-'.$classId][0])){
					expDebug::dPrint('check for waived enrolledIds'.print_r($enrollId,1),4);
					form_set_error('', t('waived reason should not be empty'));
					return '';
				} else if($submitType == 'waivedandsave'){
					$mandorComp = checkComplianceAndMandatory ($enrollId,'class');
					if($mandorComp[0]->is_compliance != 1 && $mandorComp[0]->mandatory != 'Y' ){
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
				expDebug::dPrint('completionDateArray '. print_r($completionDateArray, 1));
				if($completionDateArray[0] > 12){
					form_set_error('completion_sel_date_'.$enrollId , t('ERR242'));
				}
				else {
					if(empty($form_state['class_id'])){
						$entityId = $form_state['values']['id'];
					}else{
						$entityId = $form_state['class_id'];
					}

					$entityType = 'cre_sys_obt_cls';
					$uniqueId = "enrolluser-$entityId-$entityType";
					expDebug::dPrint('session id'.$uniqueId, 5);
					// 				expDebug::dPrint('completionDate '. date_create($completionDate));
					if(isset($form_state['input']['hidden_session_start_date'.$uniqueId]) && !empty($form_state['input']['hidden_session_start_date'.$uniqueId])) {
						expDebug::dPrint('session start date in validation'.$form_state['input']['hidden_session_start_date'.$uniqueId], 5);
						$sessionStartDate 	= $form_state['input']['hidden_session_start_date'.$uniqueId];
						$dateComp			= date_format(date_create($completionDate), 'y-m-d');
						$sessionStartDate	= date_format(date_create($sessionStartDate), 'y-m-d');
						expDebug::dPrint('sessionStartDate '. $sessionStartDate);
						expDebug::dPrint('dateComp '. $dateComp);
						//Completion Date must be greater than or equal to Session Start Date
						if($dateComp < $sessionStartDate ) {
							form_set_error('', t('MSG725'));
							return '';
						}
					}
				}


			}
			if(!empty($registrationDate)){
				// $completionDate = empty($overallCompletionDate) ? $completionDate : $overallCompletionDate;
				$registrationDateArray = explode ( '/', $registrationDate );
				$registrationDate = $registrationDateArray [0] . '/' . $registrationDateArray [1] . '/' . $registrationDateArray [2];
			}
			if($registrationDateArray[0] > 12){
				form_set_error('form' , t('ERR242'));
			}
expDebug::dPrint('$registrationDateArray form state values = '.print_r($registrationDateArray,true),5);
		}
	}catch (Exception $ex) {
		watchdog_exception('validateRosterEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function updateRosterEnrollmentDetails($form, &$form_state,$callFromAPI = 0,$deliverytype=''){
	try {
		//If anybody changes this function Please make sure API is working.
		include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_roster.inc');
		expDebug::dPrint('updateRosterEnrollmentDetails form state values = '.print_r($form_state,true),5);
		$submitTypeArray = array(
                        'cancelledandsave' => 'Canceled',
                        'enrolledandsave' => 'Enrolled',
                        'completedandsave' => 'Completed',
                        'noshowandsave' => 'No Show',
                        'incompletedandsave' => 'Incomplete',
		);

		$submitType = $form_state['triggering_element']['#name'];

		$courseId = $form_state['course_id'];
		$classId = $form_state['class_id'];
		$classId_encrypted = ($callFromAPI == 0 ) ? core_encrypt($form_state['class_id']) : $form_state['class_id']; // URL encryption to retrive data
		$selectedEnrolledIds = $form_state['input']['hidden_idlist_enrolluser-'.$classId_encrypted.'-cre_sys_obt_cls'];
		expDebug::dPrint('$selectedEnrolledIds values = '.print_r($selectedEnrolledIds,true), 5);
		expDebug::dPrint('$submitType = '.$submitType, 4);
		// completed and save error message is setting
        if(empty($selectedEnrolledIds) && ($submitType =='completedandsave' || $submitType == 'commonsave' || $submitType == 'unwaivedandsave') && empty($callFromAPI)){
			form_set_error('',t('ERR106'));
			$form_state['esign_show_popup']= false;
			return '';
		}
		if($callFromAPI)
		$delivery_type = $deliverytype;
		else $delivery_type = $form_state['enroll_delivery_type'];
		$date = now();
		$grade = '';
		$contentValidFrom  = '';
		$contentValidTo = '';
		$completionDate = '';
		$registrationStatusDate = $date;
		$rosterDetails = array();
		$overallDetails = $form_state['input']['hidden_valuelist_enrolluser-'.$classId_encrypted.'-cre_sys_obt_cls'];
		$overallDetailsArray = explode(',', $overallDetails);
		foreach($overallDetailsArray as $overallValue){
			list($tmpEnrollId, $tmpStatus, $tmpCompletionDate, $tmpCompletionScore) = explode('##', $overallValue);
			$rosterDetails[$tmpEnrollId] = array(
    									'status' => $tmpStatus,
    									'completion_date' => $tmpCompletionDate,
                                        'score' => $tmpCompletionScore);
		}

        $overallCompletionDate = $form_state['input']['hidden_completion_date_enrolluser-'.$classId_encrypted.'-cre_sys_obt_cls'];

        if($callFromAPI){
            $selectedEnrolledIds = $form_state['EnrollId'];
        }

        $selectedIdsArray = explode(',', $selectedEnrolledIds);


        $rosterResponse = '';
        $rosterResponseApi = array();
        $errors = array();
        $str = '';
        foreach($selectedIdsArray as $enrollId){
           
            $previous_exempted_status = getComplianceMandatoryStatusFromExempted($enrollId,'class',$classId);

            if((($submitType =='enrolledandsave' || $submitType =='incompletedandsave') && $form_state['input']['enrolled_status_'.$enrollId] == 'Waived')
                    || ($submitType =='enrolledandsave' && strtolower($form_state['input']['enrolled_status_'.$enrollId]) == 'enrolled' && $previous_exempted_status[0]->exempted_status == 1)){
                form_set_error('',t('Cannot mark waived status'));
                $form_state['esign_show_popup']= false;
                return '';
            }
            $enrollDetail     = getEnrollmentDetails($enrollId, array('reg_date','reg_status','comp_status','user_id','id'));
		$code = array(
  			'Enrolled' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_enr' ),
  			'Canceled' => array( 'lrn_crs_reg_can', '' ),
  			'Completed' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_cmp' ),
  			'No Show' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_nsw' ),
  			'Incomplete' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_inc' ),
  			'Attended' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_att' ),
  			'In progress' => array( 'lrn_crs_reg_cnf', 'lrn_crs_cmp_inp' ),
            'Pending' => array( 'lrn_crs_reg_ppm', '' ),
                    'Waived' => array($enrollDetail->reg_status,$enrollDetail->comp_status),
            		'UnWaived' => array('lrn_crs_reg_cnf', 'lrn_crs_cmp_enr')

		);
            $score          = $form_state['input']['score_'.$enrollId];
            $completionDate = $form_state['input']['completion_sel_date_'.$enrollId];



			if($submitType == 'commonsave'){
				$status = $form_state['input']['enrolled_status_'.$enrollId];
				if(empty($status)){
					$status = $rosterDetails[$enrollId]['status'];
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
					$score = $rosterDetails[$enrollId]['score'];
				}
            }else if($submitType == 'waivedandsave'){
                $status = $form_state['input']['enrolled_status_'.$enrollId];
                expDebug::dPrint("Value from update roster as status---->>>>>".print_r($status,1));
            }else if($submitType == 'unwaivedandsave'){
                $status = $form_state['input']['enrolled_status_'.$enrollId];
                expDebug::dPrint("Value from update roster as status---->>>>>".print_r($status,1));
			} else {
				$status = $submitTypeArray[$submitType];
				if(empty($completionDate) && $callFromAPI == 0){
					$completionDate = $rosterDetails[$enrollId]['completion_date'];
					$score = empty($score) ? $rosterDetails[$enrollId]['score'] : $score;
				}
			}

			expDebug::dPrint(' suresh1...callFromAPI='.$callFromAPI,5);

			if($callFromAPI){
				$completionDate = $form_state['CompletionDate'];
				$registrationStatusDate = $form_state['RegStatusDate'];
				$score          = $form_state['Score'];
			}
			if(strtolower($status) == 'completed'){
				$completionDate = empty($overallCompletionDate) ? $completionDate : $overallCompletionDate;
				//date('Y-m-d H:i',strtotime($registrationStatusDate)
				if(!empty($completionDate)){
					$comp_date_str = strtotime($completionDate);
					$newformat = date('Y-m-d',$comp_date_str);
					$completionDateArray = explode('-', $newformat);
					$completionDate = $completionDateArray[0].'/'.$completionDateArray[1].'/'.$completionDateArray[2];
				}
			}
			
            if(((strtolower($status) == 'enrolled' || strtolower($status) == 'in progress' || $submitType == 'waivedandsave' || $submitType == 'unwaivedandsave') && ($previous_exempted_status[0]->exempted_status == 1))){
            		revertExemptedStatus($previous_exempted_status,$enrollId,'class',$classId,$submitType,$courseId,'','',$enrollDetail->user_id);
            }else if((strtolower($status) == 'waived' || $submitType == 'waivedandsave') && empty($previous_exempted_status[0]->exempted_status)){
            		enrollmentExemptedDetailsUpdate($enrollId,'class',$classId,$submitType,$courseId,'','',$enrollDetail->user_id);
					
            }
    // for 43061: Through simulator update Roster API, should not allow if the completion date is less than the enrollment date.

   	$usrName 	= getEnrollmentPersonName($enrollId);
    $dateComp		= date_format(date_create($completionDate), 'y-m-d');
    $dateRegStatus	= date_format(date_create($registrationStatusDate), 'y-m-d');
    $dateReg		= date_format(date_create($enrollDetail->reg_date), 'y-m-d');
	include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog.inc');
	$sessionStartEndDates = getSessionStartEndDates ( $classId, false );
    $sessionStartEndDatesArray = explode ( '#', $sessionStartEndDates );
    expDebug::dPrint ( "sessionStartEndDates" . print_r ( $sessionStartEndDatesArray, 1 ), 5 );
    $sessionStartdate = date_format(date_create(str_replace ( ',', '-', $sessionStartEndDatesArray [0] )), 'Y-m-d');
    expDebug::dPrint ( "sessionStartdate" . print_r ( $sessionStartdate, 1 ), 5 );
    $flag = 0;
    if($status != "" && strtolower($status) != 'canceled' && strtolower($status) != 'enrolled' && strtolower($status) != 'waived' && strtolower($status) != 'unwaived'){
    	if((!$dateComp || (strtotime($dateComp) < strtotime($sessionStartdate))) && ($delivery_type == 'lrn_cls_dty_ilt' || $delivery_type == 'lrn_cls_dty_vcl')) {
    	//Completion Date must be greater than the Registration Date
			$errors[$usrName] = t('MSG725');
    		$flag = 1;
	    }else if(!$dateComp || (strtotime($dateComp) < strtotime($dateReg))){
	    	//Completion Date must be greater than the Registration Date
	    	$errors[$usrName] = t('Incorrect date. Completion date should be the same or greater than the registration date');
	    	$flag = 1;
    }
    }
    if(!$dateRegStatus || ($dateRegStatus < $dateReg)) {
    	//Registration Status Date must be greater than the Registration Date
    	$errors[$usrName] = t('LBL721').' '.t('LBL042').' '.t('ERR087').' '.t('LBL1067').' '.t('LBL042');
    	$flag = 1;
    }
    if($score < 0) {
    	//Score cannot be negative.
    	$errors[$usrName] = t('LBL668').' '.t('ERR213');
    	$flag = 1;
    }
			$registrationStatus = $code[$status][0];
			$completionStatus = $code[$status][1];

			$paramArray = array();
			$paramArray[] = $courseId;
			$paramArray[] = $classId;
			$paramArray[] = $enrollId;
			$paramArray[] = $registrationStatus;
			$paramArray[] = $registrationStatusDate;
			$paramArray[] = $completionStatus;
			$paramArray[] = $completionDate = empty($completionDate) ? $date : $completionDate;
			$paramArray[] = $grade;
			$paramArray[] = ($score == '0') ? '-' : $score;
			$paramArray[] = $contentValidFrom;
			$paramArray[] = $contentValidTo;
			if($flag == 0) {
			if($callFromAPI){


				$rosterResponseApi[] = updateRoster(implode('$$$', $paramArray), 1,$callFromAPI);
			}
			else{
				$rosterResponse .= updateRoster(implode('$$$', $paramArray), 1,$callFromAPI);
			}

		}
			}
		if(count($errors) > 0) {
			$combined = array();
			foreach($errors as $key=>$val) {
			$combined[$val][] = $key;
			}
			foreach($combined as $key_combined=>$val_combined) {
				$str_key = $key_combined;
				if(is_array($val_combined)) {
					foreach($val_combined as $v) {
					  $str .= $v. ", ";
					}
				}
			}
			$str = rtrim($str,", ");
			if($callFromAPI){
			  	form_set_error('', t($str_key));
			}
			else {
				$error_msg = $str_key. "=".$str;
				form_set_error('', t($error_msg));
			}
		}
		if($callFromAPI){
			return $rosterResponseApi;
		}
		if(!empty($rosterResponse) && !empty($selectedEnrolledIds)){
			$message = '<div><table>'.$rosterResponse.'</table></div>';
			drupal_set_message($message, 'error');
			$form_state['storage']['mode'] = 'view';
			$form_state['rebuild'] = TRUE;
		}

		expDebug::dPrint(' $submitType = ' . print_r($submitType, true),5);
		expDebug::dPrint(' $code = ' . print_r($code, true),4);
		expDebug::dPrint(' $$paramArray = ' . print_r($paramArray, true),4);
		expDebug::dPrint(' $form_state = ' . print_r($form_state, true),5);
		expDebug::dPrint(' $rosterResponse = ' . $rosterResponse,4);

		// Clear enrolluser-autocomplete form textfield as roster enrollment details are updated - fix for ticket #0022120
		clearClassRosterEnrollUserAutocomplete($form_state);
		// To sync solr data
		syncSolrData('User');
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');
	} catch (Exception $ex) {
		watchdog_exception('updateRosterEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function getEnrollmentPersonName($eid) {
	try {
	$sql = "select p.user_name from slt_enrollment e left join slt_person p on e.user_id=p.id where e.id=$eid";
	$qry = db_query($sql);
	$res = $qry->fetch();
	return $res->user_name;
	} catch (Exception $ex) {
		watchdog_exception('getEnrollmentPersonName', $ex);
		expertusErrorThrow($ex);
	}
}
function addNewEnrollUserDetails($form, &$form_state,$entityId,$entityType) {
	global $secure_entities;
	// Transaction is commented by Vincent on June 29, 2015.
	// Since the transaction is set on a sub function 'enrollUserToClass'.

	//$txn = db_transaction();

	try {

		//$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
		//$entityType = $form_state['triggering_element']['#ajax']['entity_type'];

		if (in_array($entityType, $secure_entities)) {
			$uniqueId = "enrolluser-".core_encrypt($entityId)."-$entityType"; // URL Encryption to reretive data
		} else {
			$uniqueId = "enrolluser-".$entityId."-$entityType"; // URL Encryption to reretive data
		}
		expDebug::dPrint('checking the enrollment for clone in class register page'.print_r($form_state,true),5);
		$associateIdListArray = array();
		$triggerElementName = $form_state['triggering_element']['#name'];
		if($triggerElementName == 'clone_enrollment' ){ //&& ($form_state["storage"]["clonedEnr"] == 0)){
			//	include_once(drupal_get_path('module', 'exp_sp_administration_clone') .'/exp_sp_administration_clone.inc');
			//$cloneEnrollment = 1;
			$getEnrolledUserId = getEnrollmentuserIds($form_state['course_id'],$form_state['oldClassId']);
			$form_state["cloneEnroll"] = 1;
			expDebug::dPrint('values for the debug for enrollment'.print_r($getEnrolledUserId,true),3);
		}

		if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]) || !empty($getEnrolledUserId)){

			$courseId = ($triggerElementName != 'clone_enrollment') ? $form_state['triggering_element']['#ajax']['course_id'] : $form_state['course_id'];
			if(!empty($form_state['input']['hidden_idlist_'.$uniqueId]))
			$associateIdListArray = explode(',', $form_state['input']['hidden_idlist_'.$uniqueId]);
			else if($triggerElementName == 'clone_enrollment' && !empty($getEnrolledUserId))
			$associateIdListArray = $getEnrolledUserId;
			else
			$associateIdListArray = explode(',', $form_state['hidden_idlist_'.$uniqueId]);
			$valuesSuccess = array();
			$valuesUnSuccess = array();
			$valuesEquWarning = array();
			$valuesRecurReg = array();

			$getClassPrice = getClassPrice($form_state['class_id']);
			$isCommerceEnabled =  module_exists('exp_sp_commerce');
			
			$flag = 0;
			if($getClassPrice > 0 && $isCommerceEnabled){
				$valuesEquWarning[] = t('MSG716');
				$form_state['rebuild']         = TRUE;
				$form_state['storage']['substage'] = 'enrolluser';
				$flag = 1;
			}else{
				$class_enrollment_count = getEnrollmentConfigValue();
				$classEnrollmentCnt = count($associateIdListArray);
				if ($classEnrollmentCnt > $class_enrollment_count) {
					$isMandatoryValue='';
					if($form_state['triggering_element']['#name']=='set-enrolluser-mandatory'){
						$isMandatoryValue=1;
					}
					$classEnrRes = tpEnrollmentRegistration('class_enrollment',$form_state['class_id'],$associateIdListArray,'',$isMandatoryValue);
					if($classEnrRes == 'success') {
						drupal_set_message(t('MSG730'));
					}
				} else {
				
				foreach($associateIdListArray as $associatedUserId){
					$associatedUserId = ($triggerElementName != 'clone_enrollment') ? $associatedUserId : $associatedUserId->user_id;
					expDebug::dPrint('$associatedUserId$associatedUserId--->>> sho'.print_r($associatedUserId,true));
					$user_detail=db_query("select full_name from slt_person WHERE id = '".$associatedUserId."'");
					$userDetail = $user_detail->fetchAll();
					$fullName = $userDetail[0]->full_name;
					$prereqcheck = checkCatalogPrerequisite($courseId,'cre_sys_obt_crs',$associatedUserId);

					if(count($prereqcheck)>0){
						$valuesUnSuccess[] = $fullName." - " .t('MSG577');
					} else{
						expDebug::dPrint('details ----->'.$associatedUserId.'-'.$courseId.'-'.$form_state['class_id']);

						$equivalancCheck = checkEquivalence($associatedUserId,$courseId,$form_state['class_id'],true);
						expDebug::dPrint('$equivalancCheck-->>> sho'.print_r($equivalancCheck,true));
						if(count($equivalancCheck) > 0){
							$cours_names = array();
							foreach($equivalancCheck as $result)
							{
								$cours_names[] = $result->equv_title;
							}
							$coursenames = implode(',',$cours_names);
							$valuesEquWarning[] = $fullName." - is already registered to an equivalent course: ".$coursenames;
						}
						$waitlist = 1;
						$isAdminSide = 'Y';
						$regDate = '';
						if(!empty($form_state['input']['hidden_registration_date_'.$uniqueId])) {
							$regDate = date_format(date_create($form_state['input']['hidden_registration_date_'.$uniqueId]), 'Y-m-d');
						}
						$stDate = date_format(date_create($ses_start[0]->ses_start_date. " " .$ses_start[0]->start_time),'Y-m-d H:i:sP');
						$isMandatory='';
						if($form_state['triggering_element']['#name']=='set-enrolluser-mandatory'){
							$isMandatory='Y';
						}
						$valuesReturn = register_user_to_course_class($associatedUserId, $courseId, $entityId, $waitlist, $isAdminSide, $response = true,$fromAPI=false,$regDate,array(),$associatedUserId->id,$associatedUserId->class_id,$triggerElementName,$isMandatory);

						expDebug::dPrint('checking the status of the enrollment'.$valuesReturn);
						$Regsvalue = '';
						if($valuesReturn == t('LBL190')){
							$valuesSuccess[] = "<ul><li><div><span class='enrolluser-name-list'>".$fullName."</span> - <span>".$valuesReturn."</span></div></li></ul>";
							$Regsvalue .= 'default';
						}else if($valuesReturn == t('Registered')){
							$Regsvalue .= 'Registered';
						}
						else if($valuesReturn == t('RecurringRegistration')) {
							$Regsvalue .= 'default';
							$valuesRecurReg[] = "<ul><li><div><span class='enrolluser-name-list'>".$fullName."</span> - <span>Registered</span></div></li></ul>";
						}else{
							$Regsvalue .= 'default';
							$valuesUnSuccess[] = "<ul><li><div><span class='enrolluser-name-list'>".$fullName."</span> - <span>".$valuesReturn."</span></div></li></ul>";
						}
					}
				}
			}
				$flag = 0;
			}
			if(count($valuesSuccess)>0){
				$returnMsg = implode('', $valuesSuccess);
				$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('MSG714').'</div><div class="registered-msg-enrolluser-list">'.$returnMsg.'</div>';
				drupal_set_message($displayTxt1);
			}
			if(count($valuesEquWarning)>0){
				$returnMsg = implode('', $valuesEquWarning);
				$resultcnt=count($valuesEquWarning);
				for($i=0;$i<$resultcnt;$i++){
					$displayWarTxt = $valuesEquWarning[$i];
					drupal_set_message($displayWarTxt, 'error');
				}
			}
			if(count($valuesUnSuccess)>0){
				$returnMsg = implode('', $valuesUnSuccess);
				$displayTxt2 = t('MSG432'). " " .t('MSG433');
				drupal_set_message($displayTxt2, 'error');
				$resultcnt=count($valuesUnSuccess);
				for($i=0;$i<$resultcnt;$i++){
					$displayTxt2 = $valuesUnSuccess[$i];
					drupal_set_message($displayTxt2, 'error');
				}
				//$displayTxt2 = '<div style="padding-top : 10px;"><div class="non-registered-msg-enrolluser">'.t('MSG432'). " " .'<b>'.t('MSG433').'</b> </div><div class="registered-msg-enrolluser-list">'.$returnMsg.'</div>';
			}
			if(count($valuesRecurReg)>0){
				$returnMsg = implode('', $valuesRecurReg);
				$displayTxt3 = '<div><div class="registered-msg-enrolluser">'.t('MSG432'). " " .'<b>'.t('MSG248').'</b>. <b>'.t('MSG311').'</b></div><div class="registered-msg-enrolluser-list">'.$returnMsg.'</div>';
				drupal_set_message($displayTxt3);
			}
			if($Regsvalue != '') {
				if(!strstr($Regsvalue, 'default')){
					$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('Registered').'</div>';
					drupal_set_message($displayTxt1);
				}else if(strstr($Regsvalue, 'Registered')){
					$displayTxt1 = '<div><div class="registered-msg-enrolluser">'.t('Others').' '.t('Registered').'</div>';
					drupal_set_message($displayTxt1);
				}
			}
			else {
				$flag = 1;
			}
			if($flag == 0){
				$form_state['storage']['mode'] = 'view';
				$form_state['rebuild'] = TRUE;
			}

		}/* else {

		drupal_set_message('Please select one or more User(s).','error');
		$form_state['storage']['mode'] = 'edit';
		$form_state['rebuild'] = true;
		//$form_state['storage']['refresh'] = 'norefresh';

		}*/

	}   catch (Exception $ex) {
		//$txn->rollback();
		watchdog_exception('addNewEnrollUserDetails', $ex);
		throw $ex;
	}

	//unset($txn);

}

/*
 * addEditRepaintEnrolltoClassForm() - callback function for save button to invoke the esign popup if esign module is enabled and validation is passed through
 *
 */
function addEditRepaintEnrolltoClassForm($form, &$form_state) {
	try {
		expDebug::dPrint('addEditRepaintEnrolltoClassForm calleed on');
		global $secure_entities;
		$form_state['storage']['mode'] = 'edit';

		$formName = $form_state['form_unique_name'];
		$entityId = $form_state['triggering_element']['#ajax']['entity_id'];
		$entityType = $form_state['triggering_element']['#ajax']['entity_type'];

		if (in_array($entityType, $secure_entities)) {
			$uniqueId = "enrolluser-".core_encrypt($entityId)."-$entityType"; // URL encryption to retrive data
		} else {
			$uniqueId = "enrolluser-".$entityId."-$entityType"; // URL encryption to retrive data
		}

		$commands = array();
		if($form_state['hidden-save-submit'] && (!empty($form_state['input']['hidden_idlist_'.$uniqueId]) || !empty($form_state['hidden_idlist_'.$uniqueId]))){
			$commands[] = ajax_command_replace(NULL, drupal_render($form[$formName]));
		}
		$commands[] = exp_ctools_modal_adjust();

		$statusMsgHTML = theme('status_messages');
		expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true),4);
		if (!empty($statusMsgHTML)) {
			// Show the messages
			$commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
			//$commands[] = ajax_command_prepend(NULL, $statusMsgHTML);
		}

		if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
			$custButtonId  = $form_state['triggering_element']['#ajax']['customButtonId'];
			$esignObj  = '{"popupDiv":"ctools-face-table","esignFor":"AddAdmin","esignButtId":"'.$custButtonId.'"}';
			if($form_state['esign_show_popup']==true)
			$commands[] = ajax_command_invoke(NULL, 'getNewEsignPopup',array($esignObj));
		}
		return array('#type' => 'ajax', '#commands' => $commands);
	} catch (Exception $ex) {
		watchdog_exception('addEditRepaintEnrolltoClassForm', $ex);
		expertusErrorThrow($ex);
	}
}

function listRosterByAPI($paramsObj, $userId){
	try {
		$courseId = $paramsObj['CourseId'];
		$classId  = $paramsObj['ClassId'];
		$userName = $paramsObj['UserName'];
		$RecurringReg = trim($paramsObj['AllReg']);
		$page	  =  trim($paramsObj['page']);
		$limit	  =  trim($paramsObj['rows']);

		if (($page != '' && $limit == '') || ($page == '' && $limit != '') ) {				
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = 'Either Page/Row is empty';
			return $errobj;			
		}	
		if((($page!='') && (!is_numeric($page))) || $page < 0) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->field = "page";
			$errobj->errormsg = 'Page should be a positive number';
			return $errobj;
		}
		if((($limit!='') && (!is_numeric($limit))) || $limit < 0) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->field = "rows";
			$errobj->errormsg = 'rows should be a positive number';
			return $errobj;
		}
		if(!empty($RecurringReg)){
		if(($RecurringReg != 0 && $RecurringReg != 1) || (!is_numeric($RecurringReg))){
			$errobj->field = 'AllReg';
			$errobj->isValidateError = 1;
			$errobj->errormsg = 'Invalid value in AllReg field, Possible values can be 0 or 1.';
			return $errobj;
		}
		}
		
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
		$start = $limit*$page - $limit;
		
		if($start < 0) {
			$start = 0;
		}
		$srcSource = getConfigValue('solr_search');
		if($srcSource == 0){
			$result = fetchEnrollUserList($courseId, $classId, '', '', $userName,'', '', '', '', '',$start, $limit);
			$count = fetchEnrollUserList($courseId, $classId, '', '',  $userName, '', '', '', '', 1);
			$result['totalrow'] = $count[0]->enrollment_count;
		}
		else{
			
			require_once $_SERVER['DOCUMENT_ROOT'] . '/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user_enrollments.php';
			try {
				$rtnType = $_REQUEST['returntype'];
				$_REQUEST['returntype'] = 'json';
				$solrObj = new UserEnrollments();
				$listqueryResult = $solrObj->classEnrollments($classId,$entityType,$searchKeyword,'enrollments');
				$result = $listqueryResult['result'];
				expDebug::dPrint('Result set from solr'.print_r($listqueryResult,true),5);
				$result['totalrow'] = $listqueryResult['count'];
				$_REQUEST['returntype'] = $rtnType;
				unset($_REQUEST['SolrImpl']);
			} catch (Exception $e) {
				expDebug::dPrint( "Error in Solr Search for Users - " . print_r($e->getMessage(), true), 1);
			}
				
		}
		expDebug::dPrint('Result set from solr dsfsfsf'.print_r($result,true),5);
		if($paramsObj['IncludeProfileData']){
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
			$resultCount = count($result);
			$config           = getConfig("exp_sp");
			$configSiteUrl    = $config["admin_site_url"];
			for($i = 0; $i < $resultCount; $i++){
				$drupalUid = getDrupalIdOfUser($result[$i]->registered_user_id);
				$account = user_load($drupalUid);
				$filepath = $account->picture->filename;
				$path= '';
				if($filepath){
					$path = $configSiteUrl . '/sites/default/files/pictures/'. $filepath;
				}
				$result[$i]->profileimage_path = $path;
			}
		}
		
	//	expdebug::dPrintDBAPI('$count :'. print_r($count, true),3);
		
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('listRosterByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function UpdateRosterByAPI(&$form_state,$createuserid=""){
	try {
		expDebug::dPrint(' UpdateRosterByAPI called');
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		$form_state['action'] = strtolower($form_state['action']);
		$statusArray = array ('cancelled','completed','noshow','incompleted');
		if(!in_array($form_state['action'], $statusArray)) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = 'Action can be any of cancelled,completed, noshow, incompleted';
			return $errobj;
		}	
		//Commented below condition for the ticket #59042.
		/*if($form_state['EnrollId'] > 0)
		{//52362
			$select = db_select("slt_enrollment", "se");
			$select->addField("se", "reg_status", "reg_status");
			$select->condition("se.id", $form_state['EnrollId'], "=");
			$select->condition("se.class_id", $form_state['class_id'], "=");
			$select->condition("se.course_id", $form_state['course_id'], "=");
			$result_set = $select->execute();
			expdebug::dPrintDBAPI('$select :', $select);
			if($result_set->rowCount() > 0)
			{
				$reg_status = $result_set->fetchAll();
				expDebug::dPrint(' $reg_status = '.print_r($reg_status[0]->reg_status, true),3);
				if($reg_status[0]->reg_status == "lrn_crs_reg_can"){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->field = 'action';
					$errobj->errormsg = 'This Enrollment is already Cancelled.';
					return $errobj;
				}
			}
			else {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = 'EnrollId';
				$errobj->errormsg = 'Invalid Enroll ID';
				return $errobj;
			}
		}	*/
		
	//	{//52353
	
		if($form_state['RegStatusDate']){
			if(!validateDateFormat($form_state['RegStatusDate'],'m/d/Y H:i:s')){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = 'RegStatusDate';
				$errobj->errormsg = 'Invalid  date format';
				return $errobj;
			}
		}
		
		if($form_state['CompletionDate']){
				if(!validateDateFormat($form_state['CompletionDate'],'m/d/Y H:i:s')){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = 'CompletionDate';
				$errobj->errormsg = 'Invalid  date format';
				return $errobj;
			}
		}
			if($form_state['class_id'] > 0 && is_numeric($form_state['class_id']))
			{
				$select = db_select("slt_course_class", "scs");
				$select->addField("scs", "delivery_type", "delivery_type");
				$select->condition("scs.id", $form_state['class_id'], "=");
				$result_set = $select->execute();
				expdebug::dPrintDBAPI('$select :', $select);
				if($result_set->rowCount() > 0)
				{
					$delivery_type = $result_set->fetchAll();
					expDebug::dPrint(' $delivery_type = '.print_r($delivery_type[0]->delivery_type, true),3);
					if($form_state['action'] == 'noshow'){
					if($delivery_type[0]->delivery_type == "lrn_cls_dty_wbt" || $delivery_type[0]->delivery_type == "lrn_cls_dty_vod"){
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->field = 'action';
						$errobj->errormsg = 'Web based and Video classes can not be marked as noshow.';
						return $errobj;
					}
					}
				}
				else {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->field = 'class_id';
					$errobj->errormsg = 'Invalid Class ID';
					return $errobj;
				}
			}
			else {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = 'class_id';
				$errobj->errormsg = 'Invalid Class ID';
				return $errobj;
			}
		//}	
		if($form_state['action'] == 'cancelled'){
			    $select = db_select("slt_enrollment", "se");
				$select->addExpression('COUNT(1)','rec_count');
				//$select->addField("se", "master_enrollment_id", "master_enrollment_id");
				$select->condition("se.class_id", $form_state['class_id'], "=");
				$select->condition("se.course_id", $form_state['course_id'], "=");
				$select->condition("se.id", $form_state['EnrollId'], "=");
				$select->condition("se.master_enrollment_id",'','!=');
				$result_set = $select->execute()->fetchAll();
				expdebug::dPrintDBAPI('$select :', $select);
				expDebug::dPrint('Roster status1 = '.print_r($result_set[0]->rec_count, true), 4);
				if($result_set[0]->rec_count > 0){					
						$errobj=new stdClass();
						$errobj->isValidateError = 1;
						$errobj->field = 'action';
						$errobj->errormsg = 'Enrollment cannot be cancelled,Associated to Training Plan';
						return $errobj;
						
				}
				
				$isPriced =0;
				$select = db_select('system', 'sys');
				$select->addField('sys', 'status', 'status');
				$select->condition('name', 'exp_sp_commerce');
				$commerceEnabled =  $select->execute()->fetchField();
				expDebug::dPrint("dropTPclassunderManagerAPI $commerceEnabled::".print_r($commerceEnabled,TRUE) , 4);
				if($commerceEnabled == 1 && $result_set[0]->rec_count <= 0){
				    $isPriced = getClassPrice($form_state['class_id']);
				}
				if ($isPriced > 0) {
				    $errobj=new stdClass();
				    $errobj->isValidateError = 1;
				    $errobj->errcode = 'L_012';
				    $errobj->errormsg = "Priced training cannot be cancelled. Contact support team.";
				    //return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
				    return $errobj;
				}
				
		}
		expDebug::dPrint('Roster status1 = '.print_r($form_state['RegStatusDate'], true), 4);
		
		
		$form_state['triggering_element']['#name'] = $form_state['action'].'andsave';
		$form_state = trimInputFields($form_state);
		unset($_REQUEST['uid']);
		$result = updateRosterEnrollmentDetails($form,$form_state,1,$delivery_type[0]->delivery_type);
		$validate = form_get_errors();
		if(count($validate)){
			$error -> errormsg = implode(',' ,$validate);
			$error -> isValidateError = true;
			return $error;
		}
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('UpdateRosterByAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function UpdateRosterInternalAPI(&$form_state,$createuserid=""){
	try {
		expDebug::dPrint(' UpdateRosterInternalAPI called');
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		$form_state['action'] = strtolower($form_state['action']);
		$statusArray = array ('cancelled','enrolled','completed');
		if(!in_array($form_state['action'], $statusArray)) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errormsg = 'Action can be any of cancelled, enrolled, completed';
			return $errobj;
		}
		if($form_state['EnrollId'] > 0 && is_numeric($form_state['EnrollId']))
		{//52362
			$select = db_select("slt_enrollment", "se");
			$select->addField("se", "reg_status", "reg_status");
			$select->addField("se", "comp_status", "comp_status");
			$select->addField("se", "user_id", "user_id");
			$select->condition("se.id", $form_state['EnrollId'], "=");
			$select->condition("se.class_id", $form_state['class_id'], "=");
			$select->condition("se.course_id", $form_state['course_id'], "=");
			$result_set = $select->execute();
			expdebug::dPrintDBAPI('$select :', $select,5);
			if($result_set->rowCount() > 0)
			{
				$reg_status = $result_set->fetchAll();
				expDebug::dPrint(' $reg_status = '.print_r($reg_status[0]->reg_status, true),3);
				expDebug::dPrint(' $comp_status = '.print_r($reg_status[0]->comp_status, true),3);
					if ($reg_status[0]->reg_status == "lrn_crs_reg_can" && $form_state['action'] != 'enrolled') { 
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->field = 'action';
					$errobj->errormsg = 'The possible action would be enrolled as the given Enrollment is already Cancelled.';
					return $errobj;
				}
				//check for delivery type
				$select = db_select("slt_course_class", "scc");
				$select->addField("scc", "delivery_type", "delivery_type");
				$select->condition("scc.id", $form_state['class_id'], "=");
				$result_set = $select->execute();
				expdebug::dPrintDBAPI('$select delivery type:', $select,5);
				$classSelected = $result_set->fetchAll();
				if($classSelected[0]->delivery_type == 'lrn_cls_dty_ilt' || $classSelected[0]->delivery_type == 'lrn_cls_dty_vcl') {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->field = 'class_id';
					$errobj->errormsg = 'Enrollment cannot be updated as the class delivery type is '.$classSelected[0]->delivery_type;
					return $errobj;
				}
				//check reg_date
				if(empty($form_state['RegStatusDate']) && $form_state['action'] == 'enrolled'){
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->field = 'RegStatusDate';
					$errobj->errormsg = 'RegStatusDate should not be empty';
					return $errobj;
				}
			}
			else {
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->field = 'EnrollId';
				$errobj->errormsg = 'enrollment id, class id and course id do not match';
				return $errobj;
			}
		}
		else {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->field = 'EnrollId';
			$errobj->errormsg = 'Invalid Enroll ID';
			return $errobj;
		}
		
		$form_state['triggering_element']['#name'] = $form_state['action'].'andsave';
		$form_state = trimInputFields($form_state);
		//To archive user's data:
		if($form_state['action'] == 'enrolled' && $reg_status[0]->comp_status == 'lrn_crs_cmp_cmp')
		{
						
			$delete=archiveUserInfo($form,$form_state);
						
}
if($form_state['action'] == 'enrolled' && $reg_status[0]->reg_status == 'lrn_crs_reg_can'){
	
	$enrollId=$form_state['EnrollId'];
	$enrollUserId=$reg_status[0]->user_id;	
	
	insertUserPointsForEachActionsPerformed($enrollUserId,'register_class',$enrollId,'insert','class');
}

		$result = updateRosterEnrollmentDetails($form,$form_state,1);
		$validate = form_get_errors();
		if(count($validate)){
			$error -> errormsg = implode(',' ,$validate);
			$error -> isValidateError = true;
			return $error;
		}
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('UpdateRosterInternalAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function archiveUserInfo($form, &$form_state) {
	try {
	    expDebug::dPrint('Inside archiveUserInfo',5);
		
		$enrollid = $form_state['EnrollId'];
		
		$default = db_query("insert into slt_attendance_summary_log select * from slt_attendance_summary where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		
		if($default) {
			db_query("delete from slt_attendance_summary where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		}
			
		$default = db_query("insert into slt_attendance_details_log select * from slt_attendance_details where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		
		if($default) {
			db_query("delete from slt_attendance_details where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		}
		
		$default = db_query("insert into slt_survey_results_log select * from slt_survey_results where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		
		if($default) {
			db_query("delete from slt_survey_results where enrollment_id = :enrollment_id",array("enrollment_id"=>$enrollid));
		}
		syncSolrData('User');
		syncSolrData('Enrollment');
		syncSolrData('MasterEnrollment');
	} catch (Exception $ex) {
		watchdog_exception('archiveUserInfo', $ex);
		expertusErrorThrow($ex);
	}
}

function getClassSessionStartDate($classId){
	try {
		$select = db_select('slt_course_class_session', 'ses');
		//$select->addExpression("DATE_FORMAT(MAX(ses.start_date),'%d/%m/%Y')", 'ses_start_date');
		$select->addExpression("DATE_FORMAT((ses.start_date),'%Y-%m-%d')", 'ses_start_date');
		$select->join('slt_profile_list_items','prof','prof.code=ses.timezone');
		//$select->addField('ses','start_date','ses_start_date');
		$select->addField('ses','start_time','start_time');
		$select->addField('ses','timezone','timezone');
		$select->addField('prof','name','timezone_name');
		$select->addField('prof','attr2','timezone_attr');
		$select->condition('ses.class_id',$classId);
		$select->orderBy('ses.start_date','ASC');
		$select->range(0,1);
		$sesStartDate = $select->execute()->fetchAll();

		return $sesStartDate;

	} catch (Exception $ex) {
		watchdog_exception('getClassSessionStartDate', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 *  clearClassRosterEnrollUserAutocomplete() - Clears enrolluser-autocomplete form textfield as switching from enroll user form to roster form
 *                                             fix for ticket #0022120
 */
function clearClassRosterEnrollUserAutocomplete(&$form_state) {
	try {
		if (!empty($form_state['complete form'][$form_state['form_unique_name']]['enrolluser-autocomplete']['#value'])) {
			$form_state['complete form'][$form_state['form_unique_name']]['enrolluser-autocomplete']['#value'] = '';
		}

		if (!empty($form_state['values']['enrolluser-autocomplete'])) {
			$form_state['values']['enrolluser-autocomplete'] = '';
		}
		if (!empty($form_state['input']['enrolluser-autocomplete'])) {
			$form_state['input']['enrolluser-autocomplete'] = t('LBL181');
		}

		if (!empty($form_state['input'][$form_state['form_unique_name']]['enrolluser-autocomplete'])) {
			$form_state['input'][$form_state['form_unique_name']]['enrolluser-autocomplete'] =  t('LBL181');
		}

		if (!empty($form_state['values'][$form_state['form_unique_name']]['enrolluser-autocomplete'])) {
			$form_state['values'][$form_state['form_unique_name']]['enrolluser-autocomplete'] = '';
		}
	} catch (Exception $ex) {
		watchdog_exception('clearClassRosterEnrollUserAutocomplete', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * to check the given registration date is lesser than equal to session start date of ILT class by given class id
 * if ILT class has more then one session, registration date will be validated against lowest session start date
 * if class id is not matched with any ILT class, function returns true
 */
function validateRegistrationDateOfILT($classId, $regDate, $deliveryType = '') {
	expDebug::dPrint ( "deliveryType" . print_r ( $deliveryType, 1 ), 5 );
	$deliveryType = $deliveryType == '' ? getClassDeliveryType($classId, 'Short') : $deliveryType;
	expDebug::dPrint ( "deliveryTypea fet" . print_r ( $deliveryType, 1 ), 5 );
	if($deliveryType == 'lrn_cls_dty_ilt' || $deliveryType == 'ilt' || $deliveryType == 'lrn_cls_dty_vcl' || $deliveryType == 'vc') {
		include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog.inc');
		$sessionStartEndDates = getSessionStartEndDates ( $classId, false );
		expDebug::dPrint ( "sessionStartEndDates" . print_r ( $sessionStartEndDates, 1 ), 5 );
		$sessionStartEndDatesArray = explode ( '#', $sessionStartEndDates );
		$sessionStartdate = str_replace ( ',', '-', $sessionStartEndDatesArray [0] );
		expDebug::dPrint ( "sessionStartdate" . print_r ( $sessionStartdate, 1 ), 5 );
		expDebug::dPrint ( "regDate" . print_r ( $regDate, 1 ), 5 );
		if (strtotime ( $sessionStartdate ) < strtotime ( $regDate )) {
			return false;
		}
	}
	return true;
}
/*
 * Added by Subin for ticket #76370
 * Bulk Upload Enrollments
 */
function bulk_enrollment_upload(){
	try {
		// Need to do the validations and perform the enrollment process.
		if(count($_FILES["files"])>0) {
                    $file = $_FILES['files']['tmp_name'];
                    $filename = $_FILES['files']['name'];
                    expDebug::dPrint('File Name '. $filename);
                    expDebug::dPrint('File '. $file);
                    // Checking the File extension is CSV and proceed if it is TRUE.
                    $dataload_path = getConfigValue('dataload_file_path');
                    if($dataload_path == '') {
                            $message = t('ERR057');
                            expDebug::dPrint("dataload_file_path not set in ex_sp.ini", 4);
                            return $message;
                    }else if(uploaded_file_validate($_FILES['files']['name'])) {
				$fileext = pathinfo($filename, PATHINFO_EXTENSION);
                                expDebug::dPrint("File Extension-- >>>" .print_r($fileext,true),5);
                                if($fileext != 'csv'){
                                    $message = t('MSG864');
                                    return $message;
                                }
				$file_name = substr_replace($filename, '_'.microtime(false), strpos($filename,'.'));
			 	$file_path = str_replace(' ', '_', $dataload_path . "/" . $file_name.'.'.$fileext);
				if(!move_uploaded_file($file,$file_path)) {
					expDebug::dPrint("enrollment upload file move failed", 4);
					$message = t('ERR057');
					return $message;
				}
				if (($handle = fopen($file_path, "r")) !== FALSE) {
					$data = fgetcsv($handle, 100, ",");
					$hdr_col_arr = explode("|", $data[0]);
					$hdr_col_arr = array_map('strtolower', $hdr_col_arr);
					$hdr_col_arr = array_map('trim', $hdr_col_arr);
                                        $hdr_arr = array("username", "completion_status", "regdate", "completiondate", "class_code", "course_code", "score","language","mandatory");
					if(count($hdr_col_arr) == 9)
					{
						$result=array_diff($hdr_col_arr, $hdr_arr);
						expDebug::dPrint("csv header data :".print_r($result, 1), 5);
				
						if(count($result) > 0)
						{
							$result = implode(",", $result);
							$message = t('Invalid column name');
							return $message;
						}
					}
					else {
						$message = t('Invalid column name');
						return $message;
					}
					fclose($handle);
				}
				
				//$startDate = new DateTime();
				$startDate = date_format(date_now(date_default_timezone(FALSE)),'Y-m-d H:i');
				$userId = getIdOfLoggedInUser();
				$userDet = getDrupalUserForNotificationInsert($userId);
				$insertFilename = str_replace(' ', '_', $file_name.'.'.$fileext);
				//File size conversion
				$unit = intval(log($_FILES['files']['size'], 1024));
				$units = array('B', 'KB', 'MB', 'GB');
				$fileSize = round($_FILES['files']['size'] / pow(1024, $unit),2). $units[$unit];

                                $fields=array(
                                        'entity_type' => 'cre_dtl_cls_enr',
                                        'file_name' => $insertFilename,
                                        'file_size' => $fileSize,
                                        'process_type' => 'bulk',
                                        'user_id' => $userId,
                                        'mail_to' => '',
                                        //'starts_when' => $startDate->format('Y-m-d H:i'),
                                        'starts_when' => $startDate,
                                        'status' => 'NS',
                                        'custom0' => null,
                                        'custom4' => 'BE'
				);
				// Insert into Dataload Jobs table.
				$insertStmt = db_insert('slt_dataload_jobs');
				$insertStmt->fields($fields);
				expDebug::dPrintDBAPI('$insert Dataload Id ' , $insertStmt);
				$insertId = $insertStmt->execute();
				make_audit_trail_entries($fields, $userId, null, 8,'Uploaded','cre_dtl_cls_enr','exp_sp_administration_class_register','bulk_enrollment_upload',
                                          array('file_name','file_size','process_type','user_id','mail_to','starts_when','status', 'custom0'));
				// Update the Job code and Job id.
				$jobId = 'job'.$insertId;
				$jobcode = str_replace(' ', '_', 'Class Enrollment').'_'.$jobId;
				$updateDtld = db_update('slt_dataload_jobs');
                                $updateDtld->condition('id', $insertId, '=');
                                $updateDtld->fields(array(
                                                      'job_id'       => $jobId ,
                                                      'job_code'    => $jobcode
                                                      ));
                                expDebug::dPrintDBAPI('Update the Job id and code ', $updateDtld);
                                $updateDtld->execute();
                                $updateDt = array();
                                $updateDt[job_id]=$jobId;
                                $updateDt[job_code]=$jobcode;
                                make_audit_trail_entries($updateDt, $userId, null, 8,'Updated','cre_dtl_cls_enr','exp_sp_administration_class_register','bulk_enrollment_upload',
                                                array('job_id', 'job_code'));
                                // Call Enrollment data load process. If the number of records
                                // less than the configured count then load will happen immediately.
                                // Otherwise the load will happen as a job process
                                  include_once DRUPAL_ROOT."/dataload/dataloadDirectExec.php";
                                $data = array(
                                              'document_root'=>DRUPAL_ROOT,
                                              'remote_addr'=>'127.0.0.1',
                                              'id'=>$insertId,
                                              'operation'=>'Instant'
                                      );
                                $obj =  new dataloadDirect();
                                $enRt = $obj->start($data);

			  if($enRt == 'Successfully Completed'){
                              $select = db_select('slt_dataload_jobs','job');
					$select->addField('job','total_records','total_records');
					$select->addField('job','status','status');
					$select->addField('job','failure_records','failure_records');
					$select->condition('job.id',$insertId,'=');
					$result = $select->execute()->fetchAll();
					
					$totalrecord = $result[0]->total_records;
					$failurerecords = $result[0]->failure_records;
					$status = $result[0]->status;
					$sucess = $totalrecord - $failurerecords;
					if($status == 'FL'){
                                            if($totalrecord >1 ){
                                                $message = t('MSG861').' '.$failurerecords.' '.t('records').' '.t('rejected');
                                            }else {
						$message = t('MSG861').' '.$failurerecords.' '.t('record').' '.t('rejected');
                                            }
					}else{
						if($failurerecords == 0 && $sucess != 0){
                                                    if($sucess > 1){
                                                        //3 records successfully uploaded
							$message = $sucess. ' ' .t('records').' '.t('successfully uploaded');
                                                    }else {
                                                        $message = $sucess. ' ' .t('record').' '.t('successfully uploaded');
                                                    }
						}elseif($sucess == 0 && $failurerecords !=0){
                                                    if($failurerecords > 1){
                                                        //Enrollment upload completed. @failurerecords records rejected.
							$message = t('MSG861').' '.$failurerecords.' '.t('records').' '.t('rejected');
                                                    }else{
                                                        $message = t('MSG861').' '.$failurerecords.' '.t('record').' '.t('rejected');
                                                    }
						}else{
                                                    //@success records successfully uploaded and @failurerecords records rejected.
                                                        if($sucess > 1 && $failurerecords > 1){
                                                            $message = $sucess.' '.t('records').' '.t('successfully uploaded'). ' and '.$failurerecords.' '.t('records').' '.t('rejected');
                                                        }else if($sucess > 1 && $failurerecords == 1){
                                                            $message = $sucess.' '.t('records').' '.t('successfully uploaded'). ' and '.$failurerecords.' '.t('record').' '.t('rejected');
                                                        }else if($sucess == 1 && $failurerecords > 1){
                                                            $message = $sucess.' '.t('record').' '.t('successfully uploaded'). ' and '.$failurerecords.' '.t('records').' '.t('rejected');
                                                        }else{
                                                            $message = $sucess.' '.t('record').' '.t('successfully uploaded'). ' and '.$failurerecords.' '.t('record').' '.t('rejected');
                                                        }
						}
					}
                                return $message;
			  }else{
			  	$message = t('MSG730');
                                return $message;
			  }
                    }else{
                        expDebug::dPrint("Invalid File Format provided for bulk enrollments.");
                        $message = t('MSG864');
                        return $message;
                    }
            }
        } catch (Exception $ex) {
		watchdog_exception('bulk_enrollment_upload', $ex);
		expertusErrorThrow($ex);
            }
	}
        
/** Enrollment Upload */
function enrollment_upload($form, &$form_state){
	try {
// 		expDebug::dPrint('$form values in enrollment upload'.print_r($form,true),5);
		expDebug::dPrint('$form_state in enrollment upload'.print_r($form_state['values'],true),5);
		//expDebug::dPrint('class code '.$form_state['values'][$form_state['form_unique_name']]['class_code']);
		if(!empty($form_state['values']['id'])) {
		$entityId = core_encrypt($form_state['values']['id']);
		}
		else {
			$entityId = core_encrypt($form_state['triggering_element']['#ajax']['entity_id']);
		}		
		$entityType = $form_state['values']['entity_value'];
		if(!empty($entityId) && !empty($entityType))
			$uniqueId = 'enrolluser_form_enrolluser-'.$entityId.'-'.$entityType;
		// Need to do the validations and perform the enrollment process.
		
		
		
		if((count($_FILES["files"])>0) && (!empty($_FILES['files']['name']['enrollment_upload_file']) || !empty($_FILES['files']['name']['enrollment_upload_file_view']) || !empty($_FILES['files']['name'][$uniqueId]))) {
			if($form_state['values']['enrollment_upload_done_btn_view'] =='uploaduser')
				$enrbut = 'enrollment_upload_file_view';
			else if(!empty($_FILES['files']['name'][$uniqueId]))
				$enrbut = $uniqueId;
			else
				$enrbut = 'enrollment_upload_file';
			//$enrbut = ($form_state['values']['enrollment_upload_done_btn_view'] =='uploaduser') ? 'enrollment_upload_file_view' : (!empty($_FILES['files']['name']['enrollment_upload_file'])) ? 'enrollment_upload_file' : $uniqueId ;
			$file = $_FILES['files']['tmp_name'][$enrbut];
			$filename = $_FILES['files']['name'][$enrbut];
			//expDebug::dPrint('button name '. $enrbut);
			//expDebug::dPrint('File Name '. $filename);
			// Checking the File extension is CSV and proceed if it is TRUE.
			$dataload_path = getConfigValue('dataload_file_path');
			if($dataload_path == '') {
				drupal_set_message(t('ERR057'));
				expDebug::dPrint("dataload_file_path not set in ex_sp.ini", 4);
			}
			else if (uploaded_file_validate($_FILES['files']['name'][$enrbut])) {
				if(!empty($form_state['class_id']) && validate_priced_class($form_state['class_id'])){
					$form_state['storage']['mode'] = '';
					//$form_state['rebuild'] = TRUE;
				} else {
				$fileext = pathinfo($filename, PATHINFO_EXTENSION);
				$file_name = substr_replace($filename, '_'.microtime(false), strpos($filename,'.'));
			 	$file_path = str_replace(' ', '_', $dataload_path . "/" . $file_name.'.'.$fileext);
				if(!move_uploaded_file($file,$file_path)) {
					expDebug::dPrint("enrollment upload file move failed", 4);
					drupal_set_message(t('ERR057'));
					return;
				}
				
				if (($handle = fopen($file_path, "r")) !== FALSE) {
					$data = fgetcsv($handle, 100, ",");
					$hdr_col_arr = explode("|", $data[0]);
					$hdr_col_arr = array_map('strtolower', $hdr_col_arr);
					$hdr_col_arr = array_map('trim', $hdr_col_arr);
				
					$hdr_arr = array("username", "completion_status", "regdate", "completiondate", "class_code", "course_code", "score","language","mandatory");
					if(count($hdr_col_arr) == 9)
					{
						$result=array_diff($hdr_col_arr, $hdr_arr);
						expDebug::dPrint("csv header data :".print_r($result, 1), 5);
				
						if(count($result) > 0)
						{
							$result = implode(",", $result);
							drupal_set_message(t('Invalid column name'));
							return;
						}
					}
					else {
						drupal_set_message(t('Invalid column name'));
						return;
					}
					fclose($handle);
				}
				
				//$startDate = new DateTime();
				$startDate = date_format(date_now(date_default_timezone(FALSE)),'Y-m-d H:i');
				$userId = getIdOfLoggedInUser();
				$userDet = getDrupalUserForNotificationInsert($userId);
				$insertFilename = str_replace(' ', '_', $file_name.'.'.$fileext);
				//File size conversion
				$unit = intval(log($_FILES['files']['size'][$enrbut], 1024));
				$units = array('B', 'KB', 'MB', 'GB');
				$fileSize = round($_FILES['files']['size'][$enrbut] / pow(1024, $unit),2). $units[$unit];

			  $fields=array(
						'entity_type' => 'cre_dtl_cls_enr',
						'file_name' => $insertFilename,
						'file_size' => $fileSize,
						'process_type' => 'bulk',
						'user_id' => $userId,
						'mail_to' => '',
						//'starts_when' => $startDate->format('Y-m-d H:i'),
			  			'starts_when' => $startDate,
						'status' => 'NS',
			      'custom0' => $form_state['class_id']
				);
				// Insert into Dataload Jobs table.
				$insertStmt = db_insert('slt_dataload_jobs');
				$insertStmt->fields($fields);
				expDebug::dPrintDBAPI('$insert Dataload Id ' , $insertStmt);
				$insertId = $insertStmt->execute();
				make_audit_trail_entries($fields, $userId, null, core_decrypt($entityId),'Uploaded','cre_dtl_cls_enr','exp_sp_administration_class_register','enrollment_upload',
                                          array('file_name','file_size','process_type','user_id','mail_to','starts_when','status', 'custom0'));
				// Update the Job code and Job id.
				$jobId = 'job'.$insertId;
				$jobcode = str_replace(' ', '_', 'Class Enrollment').'_'.$jobId;
				$updateDtld = db_update('slt_dataload_jobs');
			  $updateDtld->condition('id', $insertId, '=');
			  $updateDtld->fields(array(
			                        'job_id'       => $jobId ,
			                        'job_code'    => $jobcode
			                        ));
			  expDebug::dPrintDBAPI('Update the Job id and code ', $updateDtld);
			  $updateDtld->execute();
			  $updateDt = array();
			  $updateDt[job_id]=$jobId;
			  $updateDt[job_code]=$jobcode;
			  make_audit_trail_entries($updateDt, $userId, null, core_decrypt($entityId) ,'Updated','cre_dtl_cls_enr','exp_sp_administration_class_register','enrollment_upload',
                                          array('job_id', 'job_code'));
			  // Call Enrollment data load process. If the number of records
			  // less than the configured count then load will happen immediately.
			  // Otherwise the load will happen as a job process
			    include_once DRUPAL_ROOT."/dataload/dataloadDirectExec.php";
			  $data = array(
			  		'document_root'=>DRUPAL_ROOT,
			  		'remote_addr'=>'127.0.0.1',
			  		'id'=>$insertId,
			  		'operation'=>'Instant'
			  	);
			  $obj =  new dataloadDirect();
			  $enRt = $obj->start($data);
			  // Enrollment process end
			// $result = fetchEnrollUserList($courseId, $classId, '', '', $userName);

			  if($enRt == 'Successfully Completed'){
			  	drupal_set_message(t('MSG744'));
        	$form_state['storage']['mode'] = 'view';
			    $form_state['rebuild'] = TRUE;
			  }else{
			  	drupal_set_message(t('MSG730'));
			  }


			}


      } else{
      	$form_state['storage']['mode'] = '';
				//50886: Page getting freeze
				//$form_state['rebuild'] = TRUE;
      }
		}

		expDebug::dPrint("Form state unique name ".$form_state['form_unique_name'],4);
		expDebug::dPrint("Form state ajax triggering element ".print_r($form_state['triggering_element']['#ajax'],1),4);
		expDebug::dPrint("Form state of input values from upload enrollmet".print_r($form_state['input'],1),4);

	} catch (Exception $ex) {
		watchdog_exception('cancelEnrollUserDetails', $ex);
		expertusErrorThrow($ex);
	}
	}

/** Function for Validating the extension of enrollment file uploaded
 *
 * @param $file_name
 * @return true or false
 */
function uploaded_file_validate($file_name) {
	try {
	  $extensions         = array('.csv',);
	  $extension          = strrchr($file_name, '.');
	  $return             = false;

	  if (in_array(strtolower($extension), $extensions))    {
	      $return = true;
	  }else{
	  	form_set_error('', t('ERR171').' '.t('ERR206').'.'.' '.t('MSG740'));
	  }
	  return $return ;
  } catch (Exception $ex) {
  	watchdog_exception('uploaded_file_validate', $ex);
  	expertusErrorThrow($ex);
  }
}

function validate_priced_class($class_id){
	try{
		$select = db_select('slt_course_class','cls');
		$select->addField('cls','price');
  		$select->condition('cls.id',$class_id);
		expDebug::dPrintDBAPI('Priced class INFO ' , $select);
		$resQry = $select->execute();
		$result = $resQry->fetchField();
		if($result > 0.00){
		form_set_error('',t('Cannot upload file for priced class.'));
		return true;
		} else
			return false;
	}catch(Exception $ex){
		watchdog_exception('validate_priced_class', $ex);
  	expertusErrorThrow($ex);
	}
}

function getCountofMandatoryCompliance($classId,$type) {
	try{
		if($type == 'class') {
			$selQry = db_select('slt_enrollment','enr');
			$selQry->addExpression("COUNT(1)",'mccnt');
			$selQry->condition('enr.class_id',$classId,'=');
			$selQry->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
			$selQry->condition(db_or()->condition("enr.mandatory",'Y','=')->condition("enr.is_compliance",'1','='));
			expDebug::dPrintDBAPI('Priced class INFO ' , $selQry);
			$mcCnt = $selQry->execute()->fetchField();
			return $mcCnt;
		}
		if($type == 'tp') {
			$selQry = db_select('slt_master_enrollment','menr');
			$selQry->addExpression("COUNT(1)",'mccnt');
			$selQry->condition('menr.program_id',$classId,'=');
			$selQry->condition('menr.overall_status',array('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp'),'IN');
			$selQry->condition(db_or()->condition("menr.mandatory",'1','='));
			expDebug::dPrintDBAPI('Priced class INFO ' , $selQry);
			$mcCnt = $selQry->execute()->fetchField();
			return $mcCnt;
		}
	} catch(Exception $ex){
		watchdog_exception('getCountofMandatoryCompliance', $ex);
		expertusErrorThrow($ex);
	}
}
?>