<?php

/**
 * Function for determine whether to replicate Class or Training program
 * @param string - $page
 * 		Differentiating the call from Class or TP.
 * @param Integer - $entityId
 *    If it is from Class, then it will contain  Course id, or else it have program id.
 * @param Integer - $classId
 *		If it is from Class, then it will contain Class id, or else is will be empty
 * @param String - $enroll
 */
function adminCommonClone($entityId,$clsId,$page,$enroll){
	try {
		$loggedInUserId = getIdOfLoggedInUser();
		switch($page){
			case 'Class':
				cloneClass($entityId,$clsId,$page,$enroll,$loggedInUserId);
			break;
			case 'Tp':
				cloneTp($entityId,$clsId,$page,$enroll,$loggedInUserId);
			break;
			case 'contentauthor':
				cloneContentAuthor($entityId,$clsId,$page,$enroll,$loggedInUserId);
		}
	}catch (Exception $ex) {
		watchdog_exception('adminCommonClone', $ex);
		expertusErrorThrow($ex);
	}
}

function cloneContentAuthor($entityId,$clsId,$page,$form_state,$loggedInUserId)
{
	include_once(drupal_get_path('module', 'exp_sp_administration_contentauthor') .'/exp_sp_administration_contentauthor.inc');
	$clonedIdObj = adminCloneForContentAuthor($entityId,$clsId,$form_state,$loggedInUserId);
	$clonedId = $clonedIdObj->contentmasterid;
	if($clonedIdObj->contentmasterid){
		if($clonedIdObj->type == "lrn_cnt_typ_knc")
		{
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		$catalog_style = array(
				'ctools-admin-course-class-addedit-scroll-wrapper' => array(
						'modalSize' => array(
								'width' => 860,
								'minHeight' => 490,
						),
						'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
				)
		);

		drupal_add_js($catalog_style, 'setting');
		$argsList = array();
		$argsList[] = "";
		$argsList[] = "";
		$argsList[] = "EDIT";
		$argsList[] = $clonedId;
		//$argsList[] = $clonedId;
		$argsList[] = ($form_state['values']['checkbox-attributes-enrollment'] == 1) ? $clsId : 0;
	//	$argsList[] = $clsId;
		// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
		$formInfoList = array(
				array('id' => 'exp_sp_administration_contentauthor_presentation_addedit_form', 'title' => (t('Presentation'))),
		);
		ctoolsPopupForClone($formInfoList,$argsList,"presentation");/*
		include_once(drupal_get_path('module', 'exp_sp_administration_contentauthor') .'/exp_sp_administration_contentauthor.inc');
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		$catalog_style = array(
				'ctools-admin-content-addedit-style' => array(
						'modalSize' => array(
								'width' => 860,
								'minHeight' => 380,
						),
				)
		);

		drupal_add_js($catalog_style, 'setting');
		$argsList = array();
		$argsList[] = $entityId;
		$argsList[] = $clonedId;
		$argsList[] = ($form_state['values']['checkbox-attributes-enrollment'] == 1) ? $clsId : 0;
	//	$argsList[] = $clsId;
		// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
		$formInfoList = array(
				array('id' => 'exp_sp_administration_contentauthor_addedit_form', 'title' => (t('Video')))
		);
		ctoolsPopupForClone($formInfoList,$argsList);
		*/
		
		
		
		/*include_once(drupal_get_path('module', 'exp_sp_administration_contentauthor') .'/exp_sp_administration_contentauthor.inc');
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
		
		$css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
		global $theme_key;
		if ($theme_key == 'expertusoneV2') {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content_v2.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration_v2.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_learning') . '/exp_sp_administration_data_grid_v2.css', $css_theme_optional);
		} else {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content.css', $css_theme_optional);
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration.css', $css_theme_optional);
		}
		
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content.js', $js_module_optional); 
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') . '/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
		drupal_add_css(drupal_get_path('module', 'exp_sp_administration_contentauthor') . '/exp_sp_administration_contentauthor.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1300));
		if ($theme_key == 'expertusoneV2') {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content_v2.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1300));
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration_learning') .'/exp_sp_administration_data_grid.js', $js_module_optional);
			//includeJqGridJsCss();
		} else {
			drupal_add_css(drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content.css', $css_theme_optional);
		}
		
		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$sample_style = array('ctools-modal-ctools-admin-content-addedit-style' => array('modalSize' => array('width' => 860, 'minHeight' => 380, ), ));
		drupal_add_js($sample_style, 'setting');
		//if ($js)
		 {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();

			$contentTitle = '';
			if (!empty($clonedId)) {
				$argsList[] = $clonedId;
				$contentTitle = 'Content';
			}

			// Prepare list of forms to be displayed in the modal
			$formIdList = array( array('id' => 'exp_sp_administration_contentauthor_addedit_form', 'title' => (t('Video'))),
			//   array('id' => 'exp_sp_administration_content_versionedit_form', 'title' => (t('LBL952'))),
			);
			ctoolsPopupForClone($formIdList,$argsList);
		}*/
		} 
		else //video
		{
			$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
			
			$catalog_style = array(
					'ctools-admin-course-class-addedit-scroll-wrapper' => array(
							'modalSize' => array(
									'width' => 850,
									'minHeight' => 380,
							),
							'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
					)
			);
			
			drupal_add_js($catalog_style, 'setting');
			$argsList = array();
			$argsList[] = $clonedId;
			$argsList[] = ($form_state['values']['checkbox-attributes-enrollment'] == 1) ? $clsId : 0;
			$formInfoList = array(
					array('id' => 'exp_sp_administration_contentauthor_addedit_form', 'title' => (t('Video'))),
			);
			ctoolsPopupForClone($formInfoList,$argsList,"video");
		}
		

	}else{
		throw new Exception('exp_sp_administration_clone.inc: adminCommonClone() : cloned id error for class');
	}
	
}
function cloneClass($entityId,$clsId,$page,$form_state,$loggedInUserId){
	expDebug::dPrint('seeiond details check'.print_r($form_state,true),4);
	//For cloning the Class andd it's associated attributes
	$clonedId = adminCloneForClass($entityId,$clsId,$form_state,$loggedInUserId);
 	if($clonedId){
 		syncSolrData('Class');
		include_once(drupal_get_path('module', 'exp_sp_administration_catalog') .'/exp_sp_administration_catalog_course.inc');
		$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
		drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		$catalog_style = array(
				'ctools-admin-course-class-addedit-scroll-wrapper' => array(
						'modalSize' => array(
								'width' => 860,
								'minHeight' => 380,
						),
						'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
				)
		);

		drupal_add_js($catalog_style, 'setting');
		$argsList = array();
		$argsList[] = $entityId;
		$argsList[] = $clonedId;
		$argsList[] = ($form_state['values']['checkbox-attributes-enrollment'] == 1) ? $clsId : 0;
	//	$argsList[] = $clsId;
		// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
		$formInfoList = array(
				array('id' => 'exp_sp_administration_catalog_course_addedit_form', 'title' => (t('Course'))),
				array('id' => 'exp_sp_administration_catalog_class_title_form', 'title' => t('LBL353'))
		);
		ctoolsPopupForClone($formInfoList,$argsList);


	}else{
		throw new Exception('exp_sp_administration_clone.inc: adminCommonClone() : cloned id error for class');
	}
}


function cloneTp($entityId,$clsId,$page,$form_state,$loggedInUserId){
	//For cloning the TP andd it's associated attributes
	$clonedId = adminCloneForTrainingPlan($clsId,$form_state,$loggedInUserId);

	if($clonedId){
		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		syncSolrData('Program');
		$program_style = array(
				'ctools-admin-programform-addedit-style' => array(
						'modalSize' => array(
								'width' => 870,
								'minHeight' => 380,
						),
				)
		);

		drupal_add_js($program_style, 'setting');

		$argsList = array();
		$argsList[] = $clonedId;

		// Prepare list of forms to be displayed in the modal
		$formInfoList = array(
				array('id' => 'exp_sp_administration_program_addedit_form', 'title' => t('Training Plan')),
				array('id' => 'exp_sp_administration_program_attaching_courses_form', 'title' => t('LBL715'),'type' =>'program_tabs'),
		);
		ctoolsPopupForClone($formInfoList,$argsList);
	}else{
		throw new Exception('exp_sp_administration_clone.inc: adminCommonClone() : cloned id error for tp');
	}
}


function adminCloneForContentAuthor($entityId,$clsId,$form_state,$loggedInUserId,$options = array())
{
	try {
		expDebug::dPrint('form state values from the clone content author function calling:::'.print_r($form_state['input'],true));
		
		
		if($form_state['values']['checkbox-attributes-Content'] == 1)
		{
			expDebug::dPrint('form state values from the clone content author function calling:::'.print_r($form_state['values'],true));
			
			$slt_c_mast_fields = array('code', 'type', 'description', 'lang_code', 'status',
			 'deleted_by', 'deleted_on', 'created_by', 'created_on', 'updated_by', 'updated_on',
			 'active', 'entity_type', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4',
			 'subtype');
			$slt_c_mast = db_select('slt_content_master', 'cls');
			$slt_c_mast->fields('cls',$slt_c_mast_fields);
			$slt_c_mast->addExpression('CONCAT(cls.code,\' - (Copy \',(select COUNT(1) + 1 from slt_content_master where code like CONCAT(cls.code, \' - (Copy%\')), \')\')','code');
			$slt_c_mast->condition('cls.id', $clsId ,'=');		
			expDebug::dPrintDBAPI('Content master clone query:', $slt_c_mast);
			$queryResult = $slt_c_mast->execute()->fetchAll();
			$insertStmt = db_insert('slt_content_master');
			
			$type = $queryResult[0]->type;
			$slt_c_mast_val = array('code'=>$queryResult[0]->code, 'type'=>$queryResult[0]->type, 'description'=>$queryResult[0]->description, 'lang_code'=>$queryResult[0]->lang_code, 'status'=>$queryResult[0]->status,
			 'deleted_by'=>$queryResult[0]->deleted_by, 'deleted_on'=>$queryResult[0]->deleted_on, 'created_by'=>$loggedInUserId, 'created_on'=>now(), 'updated_by'=>$loggedInUserId, 'updated_on'=>now(),
			 'active'=>$queryResult[0]->active, 'entity_type'=>$queryResult[0]->entity_type, 'custom1'=>$queryResult[0]->custom1, 'custom2'=>$queryResult[0]->custom2, 'custom3'=>$queryResult[0]->custom3, 'custom4'=>$queryResult[0]->custom4,
			 'subtype'=>$queryResult[0]->subtype);
			 $insertStmt->fields($slt_c_mast_val);
			 $content_master_id = $insertStmt->execute();

			 
			 make_audit_trail_entries($slt_c_mast_val, $loggedInUserId , null ,$content_master_id, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_clone','cloneContentAuthor',
			 		array('code', 'type', 'description', 'lang_code', 'status','entity_type','subtype'));
			 
			 
			 

			$slt_c_ver_fields = array('id','content_master_id', 'content_sub_type', 'title', 'hosted_type', 'total_lesson',
			 'version', 'overall_mastery_score', 'status', 'published_by', 'published_on', 'file_size',
			 'file_path',  'created_by', 'created_on','updated_by','updated_on','custom1', 'custom2', 'custom3', 'custom4');
			$slt_c_ver = db_select('slt_content_version', 'cls');
			$slt_c_ver->fields('cls',$slt_c_ver_fields);
			$slt_c_ver->addExpression('CONCAT(cls.title,\' - (Copy \',(select COUNT(1) + 1 from slt_content_version where title like CONCAT(cls.title, \' - (Copy%\')), \')\')','title');
			$slt_c_ver->condition('cls.content_master_id', $clsId ,'=');	
			expDebug::dPrintDBAPI('Content version clone query:', $slt_c_ver);	
			$queryResult = $slt_c_ver->execute()->fetchAll();
			$insertStmt = db_insert('slt_content_version');
			$new_content_version_title = $queryResult[0]->title;
			$slt_c_ver_fields_val = array('content_master_id'=>$content_master_id, 'content_sub_type'=>$queryResult[0]->content_sub_type, 'title'=>$queryResult[0]->title, 'hosted_type'=>$queryResult[0]->hosted_type, 'total_lesson'=>$queryResult[0]->total_lesson,
			 'version'=>$queryResult[0]->version, 'overall_mastery_score'=>$queryResult[0]->overall_mastery_score, 'status'=>$queryResult[0]->status, 'published_by'=>$queryResult[0]->published_by, 'published_on'=>$queryResult[0]->published_on, 'file_size'=>$queryResult[0]->file_size,
			 'file_path'=>$queryResult[0]->file_path, 'created_by'=>$loggedInUserId, 'created_on'=>now(),'updated_by'=>$loggedInUserId,'updated_on' =>now(),'custom1'=>$queryResult[0]->custom1, 'custom2'=>$queryResult[0]->custom2, 'custom3'=>$queryResult[0]->custom3, 'custom4'=>$queryResult[0]->custom4,
			 ); 
			 $insertStmt->fields($slt_c_ver_fields_val);
			 $content_version_id = $insertStmt->execute();
			 
			  make_audit_trail_entries($slt_c_ver_fields_val, $loggedInUserId , null ,$content_master_id, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_clone','cloneContentAuthor',
			 		array('content_master_id', 'content_sub_type', 'title', 'hosted_type', 'total_lesson','version','overall_mastery_score','status','file_path'));

	
			$slt_c_l_fields = array('content_version_id', 'title', 'manifesturl', 'launchurl', 'masteryscore',
			 'datafromlms', 'created_by', 'created_on', 'updated_by', 'updated_on', 'custom0',
			 'custom1', 'custom2', 'custom3', 'custom4');
			$slt_c_l = db_select('slt_content_lesson', 'cls');
			$slt_c_l->fields('cls',$slt_c_l_fields);
			$slt_c_l->addExpression('CONCAT(cls.title,\' - (Copy \',(select COUNT(1) + 1 from slt_content_lesson where title like CONCAT(cls.title, \' - (Copy%\')), \')\')','title');
			$slt_c_l->condition('cls.content_version_id', $queryResult[0]->id ,'=');
			expDebug::dPrintDBAPI('lesson version clone query:', $slt_c_l);	
					
			$queryResult = $slt_c_l->execute()->fetchAll();
			$insertStmt = db_insert('slt_content_lesson');
			$slt_c_l_fields_val = array('content_version_id'=>$content_version_id, 'title'=>$queryResult[0]->title, 'manifesturl'=>$queryResult[0]->manifesturl, 'launchurl'=>$queryResult[0]->launchurl, 'masteryscore'=>$queryResult[0]->masteryscore,
			 'datafromlms'=>$queryResult[0]->datafromlms, 'created_by'=>$loggedInUserId, 'created_on'=>now(), 'updated_by'=>$loggedInUserId, 'updated_on'=>now(),
			 'custom0'=>$queryResult[0]->custom0, 'custom1'=>$queryResult[0]->custom1, 'custom2'=>$queryResult[0]->custom2,'custom3'=>$queryResult[0]->custom3,'custom4' =>$queryResult[0]->custom4);
			 $insertStmt->fields($slt_c_l_fields_val);
			 $content_l_id = $insertStmt->execute();
			  
			 
			  make_audit_trail_entries($slt_c_l_fields_val, $loggedInUserId , null ,$content_master_id, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_clone','cloneContentAuthor',
			 		array('content_version_id', 'title', 'manifesturl', 'launchurl', 'masteryscore')); 
			 
			 //move respective files to cloned copy
			 /*$fileId = $queryResult[0]->launchurl;
			 $fileIdArr = explode("/",$fileId);
			 $fileId = $fileIdArr[3];*/
			 $h5p_details = getH5PContentId($clsId);
			 expDebug::dPrint('type:::'.$type);
			 
			 // if($type == "lrn_cnt_typ_knc") //for presentation
			 {
			 	$h5pId = $h5p_details[0]->h5p_id;
			 	$jsoncontent =  $h5p_details[0]->json_content;
			 		
			 	global $user;
			 	$iContentNode = new stdClass();
			 	$iContentNode -> uid = $user->uid;
			 	$iContentNode -> type = 'h5p_content';
			 	$iContentNode -> language = 'en-us';
			 	$iContentNode -> title = $title;
			 	$iContentNode -> status = 1;
			 	$iContentNode -> promote = 0;
			 	$iContentNode -> sticky = 0;
			 	$iContentNode -> comment = 2;
			 	$iContentNode -> disable = 0;
			 	$iContentNode -> content_master_id = $content_master_id;
			 	// reference to slt_content_master
			 	
			 
			 	
			 	/*$jsonData = json_decode($jsoncontent,true);
			 	$interactions_action 	=	 	$jsonData["interactiveVideo"]['assets']['interactions'];
			 	foreach ($interactions_action as $interactionaction){
			 		if(($interactionaction['action']['library']=='H5P.Image 1.0')&& $type == "lrn_cnt_typ_vod")
			 			$iContentNode -> clone_from_original_nid = $h5pId;		 		
			 	}	*/	 	
		
			 	
			 	if($form_state['values']['checkbox-attributes-Interactions'] == 1 ) //if interactions selected then copy entire json and files
			 	{
			 		$iContentNode -> clone_from_original_nid = $h5pId;
			 		$jsondata =$jsoncontent;////json_decode($jsoncontent,true);//'{"presentation":{"slides":[{"elements":[{"x":29.936305732484076,"y":29.769392033542978,"width":40,"height":40,"action":{"library":"H5P.AdvancedText 1.1","params":{"text":"<p></p>\n"},"subContentId":"b2f6e6fa-fb1c-4862-881a-d9c6d2ab7ad2"},"alwaysDisplayComments":false,"backgroundOpacity":0,"displayAsButton":false,"invisible":false,"solution":""}],"keywords":[],"slideBackgroundSelector":{}}],"keywordListEnabled":true,"globalBackgroundSelector":{},"keywordListAlwaysShow":false,"keywordListAutoHide":false,"keywordListOpacity":90},"l10n":{"slide":"Slide","yourScore":"Your Score","maxScore":"Max Score","goodScore":"Congratulations! You got @percent correct!","okScore":"Nice effort! You got @percent correct!","badScore":"You got @percent correct.","Total":"Total","showSolutions":"Show solutions","retry":"Retry","title":"Title","author":"Author","lisence":"License","license":"License","exportAnswers":"Export text","copyright":"Rights of use","hideKeywords":"Hide keywords list","showKeywords":"Show keywords list","fullscreen":"Fullscreen","exitFullscreen":"Exit fullscreen","prevSlide":"Previous slide","nextSlide":"Next slide","currentSlide":"Current slide","lastSlide":"Last slide","solutionModeTitle":"Exit solution mode","solutionModeText":"Solution Mode","summaryMultipleTaskText":"Multiple tasks","scoreMessage":"You achieved:","shareFacebook":"Share on Facebook","shareTwitter":"Share on Twitter","summary":"Summary","solutionsButtonTitle":"Show comments","printTitle":"Print","printIngress":"How would you like to print this presentation?","printAllSlides":"Print all slides","printCurrentSlide":"Print current slide"},"override":{"activeSurface":false,"overrideButtons":false,"overrideShowSolutionButton":false,"overrideRetry":false,"hideSummarySlide":false}}';
			 	}
			 	else
			 	{
			 		//$jsondata = '{"presentation":{"slides":[{"elements":[{"x":29.936305732484076,"y":29.769392033542978,"width":40,"height":40,"action":{"library":"H5P.AdvancedText 1.1","params":{"text":"<p></p>\n"},"subContentId":"b2f6e6fa-fb1c-4862-881a-d9c6d2ab7ad2"},"alwaysDisplayComments":false,"backgroundOpacity":0,"displayAsButton":false,"invisible":false,"solution":""}],"keywords":[],"slideBackgroundSelector":{}}],"keywordListEnabled":true,"globalBackgroundSelector":{},"keywordListAlwaysShow":false,"keywordListAutoHide":false,"keywordListOpacity":90},"l10n":{"slide":"Slide","yourScore":"Your Score","maxScore":"Max Score","goodScore":"Congratulations! You got @percent correct!","okScore":"Nice effort! You got @percent correct!","badScore":"You got @percent correct.","Total":"Total","showSolutions":"Show solutions","retry":"Retry","title":"Title","author":"Author","lisence":"License","license":"License","exportAnswers":"Export text","copyright":"Rights of use","hideKeywords":"Hide keywords list","showKeywords":"Show keywords list","fullscreen":"Fullscreen","exitFullscreen":"Exit fullscreen","prevSlide":"Previous slide","nextSlide":"Next slide","currentSlide":"Current slide","lastSlide":"Last slide","solutionModeTitle":"Exit solution mode","solutionModeText":"Solution Mode","summaryMultipleTaskText":"Multiple tasks","scoreMessage":"You achieved:","shareFacebook":"Share on Facebook","shareTwitter":"Share on Twitter","summary":"Summary","solutionsButtonTitle":"Show comments","printTitle":"Print","printIngress":"How would you like to print this presentation?","printAllSlides":"Print all slides","printCurrentSlide":"Print current slide"},"override":{"activeSurface":false,"overrideButtons":false,"overrideShowSolutionButton":false,"overrideRetry":false,"hideSummarySlide":false}}';
			 		//new one
			 		$jsonObject = json_decode($jsoncontent,true);
			 		if($type == "lrn_cnt_typ_vod") {
			 			expDebug::dPrint("mime:".$jsonObject['interactiveVideo']['video']['files'][0]["mime"]);
			 			if($jsonObject['interactiveVideo']['video']['files'][0]["mime"] != "video/YouTube")
			 				$iContentNode -> clone_from_original_nid = $h5pId;
			 			$jsonObject["interactiveVideo"]['assets']['interactions'] = [];
			 			$jsondata =  json_encode($jsonObject);
			 		}else
			 		{
			 			$showSolution = $jsonObject["override"]["overrideShowSolutionButton"];
			 			$retry = $jsonObject["override"]["overrideRetry"];
			 			if($retry == "" || $retry == null || $retry == "0")
			 				$retry = "0";
			 			else
			 				$retry = "true";
			 			
			 			if($showSolution == "" || $showSolution == null || $showSolution == "0")
			 				$showSolution = "false";
			 			else
			 				$showSolution = "true";
			 			
			 			
			 			$passScore = $jsonObject["presentation"]["passingscore"];
			 			$globalBackgroundSelector = $jsonObject["presentation"]["globalBackgroundSelector"]["fillGlobalBackground"];
			 			$slides = "";
			 			$fillSlideBackground = ""; //Slide specific background
			 			for($slideItr = 0; $slideItr < count($jsonObject["presentation"]["slides"]);$slideItr++)
			 			{
			 			 $fillSlideBackground = $jsonObject["presentation"]["slides"][$slideItr]["slideBackgroundSelector"]["fillSlideBackground"];	

			 				if($slides == "" && $fillSlideBackground == "null")
			 					$slides = '{"elements":[],"keywords":[],"slideBackgroundSelector":{"fillSlideBackground":'.$fillSlideBackground.' } }';
			 				else if	($slides == "")	
			 					$slides = '{"elements":[],"keywords":[],"slideBackgroundSelector":{"fillSlideBackground":"'.$fillSlideBackground.'" } }';
			 		
			 				else if($fillSlideBackground == "null")
			 					$slides = $slides.','.'{"elements":[],"keywords":[],"slideBackgroundSelector":{"fillSlideBackground":'.$fillSlideBackground.'} }';
			 				else 
			 					$slides = $slides.','.'{"elements":[],"keywords":[],"slideBackgroundSelector":{"fillSlideBackground":"'.$fillSlideBackground.'"} }';
			 						
			 				
			 			}
			 			expDebug::dPrint("Slide Count:".$slides);
			 			$jsondata ='{"presentation":{"slides":['.$slides.'],"keywordListEnabled":true,"globalBackgroundSelector":{ "fillGlobalBackground":"'.$globalBackgroundSelector.'"},"keywordListAlwaysShow":false,"keywordListAutoHide":false,"keywordListOpacity":90,"passingscore":"'.$passScore.'"},"l10n":{"slide":"Slide","yourScore":"Your Score","maxScore":"Max Score","goodScore":"Congratulations! You got @percent correct!","okScore":"Nice effort! You got @percent correct!","badScore":"You got @percent correct.","Total":"Total","showSolutions":"Show solutions","retry":"Retry","title":"Title","author":"Author","lisence":"License","license":"License","exportAnswers":"Export text","copyright":"Rights of use","hideKeywords":"Hide keywords list","showKeywords":"Show keywords list","fullscreen":"Fullscreen","exitFullscreen":"Exit fullscreen","prevSlide":"Previous slide","nextSlide":"Next slide","currentSlide":"Current slide","lastSlide":"Last slide","solutionModeTitle":"Exit solution mode","solutionModeText":"Solution Mode","summaryMultipleTaskText":"Multiple tasks","scoreMessage":"You achieved:","shareFacebook":"Share on Facebook","shareTwitter":"Share on Twitter","summary":"Summary","solutionsButtonTitle":"Show comments","printTitle":"Print","printIngress":"How would you like to print this presentation?","printAllSlides":"Print all slides","printCurrentSlide":"Print current slide"},"override":{"activeSurface":false,"overrideButtons":true,"overrideShowSolutionButton":'.$showSolution.',"overrideRetry":'.$retry.',"hideSummarySlide":false}}';
			 		}
			 		expDebug::dPrint("Converted::".$jsondata);
			 
			 	}
			 	$iContentNode -> nid = '';
			 	$iContentNode -> vid = '';
			 	$iContentNode -> changed = '';
			 	$iContentNode -> h5p = '';
			 	$iContentNode -> json_content = $jsondata;
			 	//$iContentNode -> filtered = $jsondata;
			 
			 	if($type == "lrn_cnt_typ_vod")
			 	{
			 		$iContentNode -> main_library_id = 36;
			 		$iContentNode -> h5p_library = 'H5P.InteractiveVideo 1.9';
			 	}else
			 	{
			 		$iContentNode -> main_library_id = 13;
			 		$iContentNode -> h5p_library = 'H5P.CoursePresentation 1.9';
			 
			 	}
			 	$iContentNode -> embed_type = 'div';
			 	$iContentNode -> h5p_type = 'create';
			 	$iContentNode -> created = time();
			 	$iContentNode -> revision = 0;
			 	$iContentNode -> timestamp = time();
			 	$iContentNode -> op = "Save";
			 	$iContentNode -> submit = "Save";
			 	$iContentNode -> preview = "Preview";
			 	$iContentNode -> form_id = "h5p_content_node_form";
			 	expDebug::dPrint("Final form save: " . print_r($iContentNode, 1));
			 
			 	node_save($iContentNode);
			 	$newh5pid = $iContentNode->nid;//h5p_get_content_id($iContentNode);
			 
			 	include_once(drupal_get_path('module', 'exp_sp_administration_content') .'/exp_sp_administration_content.inc');
			 	include_once $_SERVER["DOCUMENT_ROOT"] . '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_contentauthor/exp_sp_administration_contentauthor.inc';
				
			 	$loggedInUserId = getIdOfLoggedInUser();
			 	addNewShareContent($form, $form_state,$content_master_id,'',$loggedInUserId);
			 	
			 	$launchurl = "/?q=h5p/embed/".$newh5pid;
			 	/*$sql = "select nid from h5p_nodes where content_id=" . $newh5pid;
			 			
			 	$launchid = 0;
			 	foreach (db_query($sql) as $result) {
			 	$launchid = $result -> nid;
			 	}
			 	$launchurl = "?q=node/".$launchid."/view";*/
			 
			 
			 	$updateStmt = db_update('slt_content_lesson');
			 	$fields = array('launchurl'=>$launchurl);
			 	$updateStmt -> fields($fields);
			 	$updateStmt -> condition('id', $content_l_id);
			 	expDebug::dPrintDBAPI('id=' . $id . '  $updateStmt:', $updateStmt);
			 	$numUpdated = $updateStmt -> execute();
			 
			 		
			 		
			 	//_h5peditor_create_content($iContentNode);
			 		
			 }
			 /* else if($type == "lrn_cnt_typ_vod") //for video
			  {
			 $h5pId = $h5p_details[0]->h5p_id;
			 $jsoncontent =  $h5p_details[0]->json_content;
			 
			 $jsonData = json_decode($jsoncontent,true);
			 	
			 $path = 	$jsonData["interactiveVideo"]['video']['files'][0]["path"];
			 $passingScore = $jsonData["interactiveVideo"]['video']["passingscore"];
			 $show_solution =$jsonData["override"]["overrideShowSolutionButton"];
			 $retry =	$jsonData["override"]["overrideRetry"];
			 
			 if(strpos($path,"youtube") >0)
			 {
			 $posterPath = 	$jsonData["interactiveVideo"]['video']['poster']["path"];
			 $clonedPathPoster = getClonedVersionIdPath($posterPath,$content_version_id);
			 
			 $posterPathURI = "public://h5peditor/".$posterPath;
			 $clonedPathPosterURI = "public://h5peditor/".$clonedPathPoster;
			 
			 copyh5pfile($posterPathURI,$clonedPathPosterURI);
			 expDebug::dPrint("Interaction===".$form_state['input']['checkbox-attributes-Interactions']);
			 expDebug::dPrint("clonedPathPoster.".$clonedPathPoster);
			 if($form_state['values']['checkbox-attributes-Interactions'] == 1 )
			 {
			 $interactions 	=	 	$jsonData["interactiveVideo"]['assets']['interactions'];
			 $bookmarks 		=	 	$jsonData["interactiveVideo"]['assets']['bookmarks'];
			 $summary = 				$jsonData["interactiveVideo"]['summary'];
			 expDebug::dPrint("interactions::".json_encode($interactions));
			 }
			 else
			 {
			 $bookmarks = array();
			 $interactions =  array();
			 $summary = array();
			 }
			 $newh5p_id = insertH5PRecord($new_content_version_title,$content_master_id,$path,"video/youtube",$clonedPathPoster,$mimePoster,$show_solution,$retry,$interactions,$summary,$bookmarks,$passingScore);
			 $destPosterPath = "public://h5p/content/".$newh5p_id."/".$clonedPathPoster;
			 copyh5pfile($posterPathURI,$destPosterPath);
			 
			 }
			 else
			 {
			 $clonedPath = getClonedVersionIdPath($path,$content_version_id);
			 expDebug::dPrintDBAPI('sureshpath1:'.$path);
			 expDebug::dPrintDBAPI('sureshpath2:'.$clonedPath);
			 
			 
			 $srcPath = "public://h5peditor/".$path;
			 $destPath = "public://h5peditor/".$clonedPath;
			 	
			 	
			 expDebug::dPrint("suresh Src path:".$srcPath);
			 expDebug::dPrint("suresh Dest path:".$destPath);
			 copyh5pfile($srcPath,$destPath);
			 	
			 	
			 
			 $posterPath = 	$jsonData["interactiveVideo"]['video']['poster']["path"];
			 	
			 $clonedPathPoster = getClonedVersionIdPath($posterPath,$content_version_id);
			 	
			 $posterPathURI = "public://h5peditor/".$posterPath;
			 $clonedPathPosterURI = "public://h5peditor/".$clonedPathPoster;
			 	
			 copyh5pfile($posterPathURI,$clonedPathPosterURI);
			 expDebug::dPrint("Interaction===".$form_state['input']['checkbox-attributes-Interactions']);
			 if($form_state['values']['checkbox-attributes-Interactions'] == 1 )
			 {
			 $interactions 	=	 	$jsonData["interactiveVideo"]['assets']['interactions'];
			 $bookmarks 		=	 	$jsonData["interactiveVideo"]['assets']['bookmarks'];
			 $summary = 				$jsonData["interactiveVideo"]['summary'];
			 expDebug::dPrint("interactions::".json_encode($interactions));
			 }
			 else
			 {
			 $bookmarks = array();
			 $interactions =  array();
			 $summary = array();
			 }
			 $mime = "";
			 if(strpos($clonedPath, ".mp4") > 0)
			 {
			 $mime = "video/mp4";
			 }
			 else {
			 $mime = "video/webm";
			 }
			 	
			 $mimePoster = "";
			 if(strpos($clonedPath, ".png") > 0)
			 {
			 $mimePoster = "image/png";
			 }
			 else if(strpos($clonedPath, ".jpg") > 0) {
			 $mimePoster = "image/jpg";
			 }
			 $newh5p_id = insertH5PRecord($new_content_version_title,$content_master_id,$clonedPath,$mime,$clonedPathPoster,$mimePoster,$show_solution,$retry,$interactions,$summary,$bookmarks, $passingScore);
			 
			 $destVideoPath = "public://h5p/content/".$newh5p_id."/".$clonedPath;
			 copyh5pfile($srcPath,$destVideoPath);
			 	
			 $destPosterPath = "public://h5p/content/".$newh5p_id."/".$clonedPathPoster;
			 copyh5pfile($posterPathURI,$destPosterPath);
			 	
			 $launchurl = "?q=h5p/embed/".$newh5p_id;
			 
			 $updateStmt = db_update('slt_content_lesson');
			 $fields = array('launchurl'=>$launchurl);
			 $updateStmt -> fields($fields);
			 $updateStmt -> condition('id', $content_l_id);
			 expDebug::dPrintDBAPI('id=' . $content_l_id . '  $updateStmt:', $updateStmt);
			 $numUpdated = $updateStmt -> execute();
			 }
			 
			 } */
			 
			 if($form_state['values']['checkbox-attributes-access'] == 1){
			     // For Clone catalog_access table
			    /*  $cataccFields = array('entity_type', 'user_type', 'is_manager', 'org_id',
			             'job_role', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
			     	
			     $cataccquery = db_select('slt_catalog_access', 'cat_acc');
			     $cataccquery->fields('cat_acc',$cataccFields);
			     $cataccquery->addExpression($content_master_id,'entity_id');
			     $cataccquery->addExpression($loggedInUserId,'created_by');
			     $cataccquery->addExpression('now()','created_on');
			     $cataccquery->addExpression($loggedInUserId,'updated_by');
			     $cataccquery->addExpression('now()','updated_on');
			     $cataccquery->condition('cat_acc.entity_id', $clsId,'=');
			     if($type == "lrn_cnt_typ_vod")
			       $cataccquery->condition('cat_acc.entity_type', 'cre_sys_obt_cnt_aut','=');
			     else 
			         $cataccquery->condition('cat_acc.entity_type', 'cre_sys_obt_cnt_aut_pre','=');
			     	
			     expDebug::dPrintDBAPI('Catalog access clone query', $cataccquery);
			     	
			     db_insert('slt_catalog_access')
			     ->from($cataccquery)
			     ->execute(); */
			     	
			     // For Clone contentAuthor access
			     $mromapFields = array('entity_type', 'group_id', 'group_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
			     	
			     $mromapquery = db_select('slt_group_mapping', 'mro_map');
			     $mromapquery->fields('mro_map',$mromapFields);
			     $mromapquery->addExpression($content_master_id,'entity_id');
			     $mromapquery->addExpression($loggedInUserId,'created_by');
			     $mromapquery->addExpression('now()','created_on');
			     $mromapquery->addExpression($loggedInUserId,'updated_by');
			     $mromapquery->addExpression('now()','updated_on');
			    /*  $mromapquery->addExpression("concat('".$entityId."-".$content_master_id."')","parent_id");
			     $mromapquery->addExpression("'learning'","parent_type"); */
			     $mromapquery->addExpression("NULL","parent_id");
			     $mromapquery->addExpression("NULL","parent_type");
			     $mromapquery->condition('mro_map.entity_id', $clsId,'=');
			     if($type == "lrn_cnt_typ_vod")
			       $mromapquery->condition('mro_map.entity_type', 'cre_sys_obt_cnt_aut','=');
			     else 
			       $mromapquery->condition('mro_map.entity_type', 'cre_sys_obt_cnt_aut_pre','=');
			     	
			     expDebug::dPrintDBAPI('Catalog mro mapping clone query content contentauthoring', $mromapquery);
			     	
			     db_insert('slt_group_mapping')
			     ->from($mromapquery)
			     ->execute();
			 
			     // For Clone user_mapping table #79414
			    /*  $usrmapFields = array('entity_type', 'user_id', 'access_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
			 
			     $usrmapquery = db_select('slt_user_access_mapping', 'usr_map');
			     $usrmapquery->fields('usr_map',$usrmapFields);
			     $usrmapquery->addExpression($content_master_id,'entity_id');
			     $usrmapquery->addExpression($loggedInUserId,'created_by');
			     $usrmapquery->addExpression('now()','created_on');
			     $usrmapquery->addExpression($loggedInUserId,'updated_by');
			     $usrmapquery->addExpression('now()','updated_on');
			     $usrmapquery->addExpression("concat('".$entityId."-".$content_master_id."')","parent_id");
			     $usrmapquery->addExpression("'learning'","parent_type");
			     $usrmapquery->condition('usr_map.entity_id', $clsId,'=');
			     if($type == "lrn_cnt_typ_vod")
			      $usrmapquery->condition('usr_map.entity_type', 'cre_sys_obt_cnt_aut','=');
			     else 
			        $usrmapquery->condition('usr_map.entity_type', 'cre_sys_obt_cnt_aut_pre','=');
			     
			     expDebug::dPrintDBAPI('User mapping clone query contentauthoring', $usrmapquery);
			 
			     db_insert('slt_user_access_mapping')
			     ->from($usrmapquery)
			     ->execute(); */

			 } 
			 
			 
			 
			 $stdObj = new stdClass();
			 $stdObj->contentmasterid = $content_master_id;
			 $stdObj->type = $type;
			 
		//	 make_audit_trail_entries($iContentNode, $loggedInUserId , null ,$content_master_id, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_clone','cloneContentAuthor',
		//	 		array('$content_master_id', '$type'));
			 
			return $stdObj;
			
	//	make_audit_trail_entries($fields, $loggedInUserId , null ,$content_master_id, 'Inserted','cre_sys_obt_cnt','exp_sp_administration_clone','cloneclass',
	//			array('content_master_id', 'title', 'code', 'short_description', 'description', 'lang_code'));
		//if($form_state['values']['checkbox-attributes-enrollment'] == 1 && ($oldClassDelType == 'lrn_cls_dty_ilt' || $oldClassDelType == 'lrn_cls_dty_vcl'))
			//drupal_set_message(t('MSG722'),'error');
					
		
		}
	  }catch (Exception $ex) {
  	watchdog_exception('adminCommonClone', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Function for duplicating the records of all the attributes which is associated to the cloning class.
 *
 * Duplicating Survey,Assessment, access, tags, business rules etc attached to the class will be cloned by the class id.
 * Enrollment will be cloned based upon the selection made by clicking on with enrollment option by the user.
 *
 * @param $entityId
 * 		- Course Id for duplicating
 * @param $clsId
 * 		- The class id which has to be cloned.
 * @param $loggedInUserId
 *    - Id of the person logged in.
 */

function customattributevalues($entity='')
{
	$selectcustom = db_select('slt_custom_attr', 'cusatt');
	$selectcustom->Join('slt_custom_attr_mapping','cusatt1','cusatt1.cattr_id = cusatt.id');
	$selectcustom->addField('cusatt', 'id', 'id');
	$selectcustom->addField('cusatt', 'cattr_name', 'title');
	$selectcustom->addField('cusatt', 'cattr_type', 'cattr_type');
	$selectcustom->addField('cusatt', 'cattr_unique', 'cattr_unique');
	$selectcustom->addField('cusatt1', 'entity_ref_tbl_col', 'entity_ref_tbl_col');
	$selectcustom->condition('cusatt.status', array('cre_cattr_sts_del'), 'NOT IN');
	if($entity=='cre_sys_obt_trp')
	{
		$selectcustom->condition('cusatt1.entity_type', 'cre_sys_obt_trp', '=');
	}
	else if($entity=='cre_sys_obt_cls')
	{
		$selectcustom->condition('cusatt1.entity_type', 'cre_sys_obt_cls', '=');
	}
	$selectcustom->condition('cusatt.status', 'cre_cattr_sts_atv', '=');
	$selectcustom->condition('cusatt1.entity_screen_opt', '1', '=');
	$selectcustom->condition('cusatt.cattr_unique', '1', '=');
	$selectcustom->condition('cusatt.cattr_type', 'cattr_type_txtbox', '=');
	expDebug::dPrintDBAPI('custom attribute query', $selectcustom);
	$selectcustomresult = $selectcustom->execute()->fetchAll();
	return $selectcustomresult;

	expDebug::dPrint('Training program clone'. print_r($selectcustomresult,true));

}

function adminCloneForClass($entityId,$clsId,$form_state,$loggedInUserId,$options = array()){
	try {
		expDebug::dPrint('form state values from the clone function calling'.print_r($form_state,true));
		
		// To get the custom attribute with the textbox with the unique set as 1
		if(module_exists('exp_sp_administration_customattribute'))
		{
			$selectcustomresultclass = customattributevalues('cre_sys_obt_cls');

		}

		// For Clone Course_Class table
		$classFields = array('course_id', 'description', 'short_description', 'status', 'lang_code',
			 'delivery_type', 'valid_from', 'valid_to', 'min_seats', 'max_seats', 'waitlist_count',
			 'location_id', 'content_url', 'document_url', 'max_attempts', 'cancel_date', 'cancel_remarks', 'reg_count',
			 'ratings', 'smes_list', 'subject_area', 'blended_seqno', 'stats_value1', 'stats_value2',
			 'stats_value3', 'stats_value4', 'stats_value5', 'registration_end_on', 'cancellation_deadline_date', 'price', 'currency_type',
			 'training_units', 'cancellation_fee', 'late_cancellation_fee', 'no_show_fee', 'in_active_on', 'in_active_reason',
			 'author_vendor', 'is_compliance', 'dedicated_class_flag', 'additional_info', 'deleted_by', 'deleted_on',
			 'published_on', 'discussion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4','e1_cattr0','e1_cattr1','e1_cattr2','e1_cattr3','e1_cattr4','e1_cattr5','e1_cattr6','e1_cattr7','e1_cattr8','e1_cattr9');


		$classquery = db_select('slt_course_class', 'cls');
		$classquery->fields('cls',$classFields);
		$classquery->addExpression('CONCAT(cls.title,\' - (Copy \',(select COUNT(1) + 1 from slt_course_class where code like CONCAT(cls.code, \' - (Copy%\')), \')\')','title');
		$classquery->addExpression('CONCAT(cls.code,\' - (Copy \',(select COUNT(1) + 1 from slt_course_class where code like CONCAT(cls.code, \' - (Copy%\')), \')\')','code');
		
		if(module_exists('exp_sp_administration_customattribute'))
		{
		foreach($selectcustomresultclass as $load_key1 => $load_val1){
			$entity_id=$load_val1->entity_ref_tbl_col;
			$classquery->addExpression('CONCAT(cls.'.$entity_id.',\' - (Copy \',(select COUNT(1) + 1 from slt_course_class where code like CONCAT(cls.code, \' - (Copy%\')), \')\')',".$entity_id.'");
		}
		}
		
		$classquery->condition('cls.id', $clsId ,'=');
		$classquery->condition('cls.course_id', $entityId ,'=');
		expDebug::dPrintDBAPI('Class clone query', $classquery);

		$queryResult = $classquery->execute()->fetchAll();

		expDebug::dPrint('Class clone query result'. print_r($queryResult,true));
		$oldClassDelType = $queryResult[0]->delivery_type;

		if($form_state['values']['checkbox-attributes-enrollment'] == 1 && ($oldClassDelType == 'lrn_cls_dty_ilt' || $oldClassDelType == 'lrn_cls_dty_vcl'))
			drupal_set_message(t('MSG722'),'error');

		$insertStmt = db_insert('slt_course_class');
		$fields = array(
				'course_id' => $queryResult[0]->course_id,
				'title' => $queryResult[0]->title,
				'code' => $queryResult[0]->code,
				'short_description' => $queryResult[0]->short_description,
				'description' => $queryResult[0]->description,
				'lang_code' => $queryResult[0]->lang_code,
				'status' => 'lrn_cls_sts_itv',
				'delivery_type' => $queryResult[0]->delivery_type,
				'valid_from' => $queryResult[0]->valid_from,
				'valid_to' => $queryResult[0]->valid_to,
				'min_seats' => $queryResult[0]->min_seats,
				'max_seats' => $queryResult[0]->max_seats,
				'waitlist_count' => $queryResult[0]->waitlist_count,
				'location_id' => $queryResult[0]->location_id,
				'content_url' => $queryResult[0]->content_url,
				'document_url' => $queryResult[0]->document_url,
				'max_attempts' => $queryResult[0]->max_attempts,
				'cancel_date' => $queryResult[0]->cancel_date,
				'cancel_remarks' => $queryResult[0]->cancel_remarks,
				'reg_count' => $queryResult[0]->reg_count,
				'ratings' => $queryResult[0]->ratings,
				'smes_list' => $queryResult[0]->smes_list,
				'subject_area' => $queryResult[0]->subject_area,
				//'scheduled_duration' => $queryResult[0]->scheduled_duration,
				'blended_seqno' => $queryResult[0]->blended_seqno,
				'stats_value1' => $queryResult[0]->stats_value1,
				'stats_value2' => $queryResult[0]->stats_value2,
				'stats_value3' => $queryResult[0]->stats_value3,
				'stats_value4' => $queryResult[0]->stats_value4,
				'stats_value5' => $queryResult[0]->stats_value5,
				'registration_end_on' => $queryResult[0]->registration_end_on,
				'cancellation_deadline_date' => $queryResult[0]->cancellation_deadline_date,
				'currency_type' => $queryResult[0]->currency_type,//
				'price' => $queryResult[0]->price,
				'training_units' =>$queryResult[0]->training_units,
				'cancellation_fee' => $queryResult[0]->cancellation_fee,
				'late_cancellation_fee' => $queryResult[0]->late_cancellation_fee,
				'no_show_fee' => $queryResult[0]->no_show_fee,
				'in_active_on' => $queryResult[0]->in_active_on,
				'in_active_reason' => $queryResult[0]->in_active_reason,
				'author_vendor' => $queryResult[0]->author_vendor,//
				'is_compliance' => $queryResult[0]->is_compliance,
				'dedicated_class_flag' => $queryResult[0]->dedicated_class_flag,
				'clone_from'	=> $clsId,
				'additional_info' => $queryResult[0]->additional_info,
				'deleted_by' => $queryResult[0]->deleted_by,
				'deleted_on'=> $queryResult[0]->deleted_on,
				'created_by' => $loggedInUserId,
				'created_on' => now(),
				'updated_by' => $loggedInUserId,
				'updated_on' => now(),
				'published_on' => $queryResult[0]->published_on,
				'discussion'  =>  $queryResult[0]->discussion,
				'custom0' =>  $queryResult[0]->custom0,
				'custom1' =>  $queryResult[0]->custom1,
				'custom2' => $queryResult[0]->custom2,
				'custom3' => $queryResult[0]->custom3,
				'custom4' => $queryResult[0]->custom4,
				'e1_cattr0' => $queryResult[0]->e1_cattr0,
				'e1_cattr1' => $queryResult[0]->e1_cattr1,
				'e1_cattr2' => $queryResult[0]->e1_cattr2,
				'e1_cattr3' => $queryResult[0]->e1_cattr3,
				'e1_cattr4' => $queryResult[0]->e1_cattr4,
				'e1_cattr5' => $queryResult[0]->e1_cattr5,
				'e1_cattr6' => $queryResult[0]->e1_cattr6,
				'e1_cattr7' => $queryResult[0]->e1_cattr7,
				'e1_cattr8' => $queryResult[0]->e1_cattr8,
				'e1_cattr9' => $queryResult[0]->e1_cattr9,
				);
		

		// Select the fields and the values to be inserted
		$insertStmt->fields($fields);

		expDebug::dPrintDBAPI(' $insertStmt SQL = ' , $insertStmt);

		// Execute the insert statement. Fetch the new class id in slt_course_class table.
		$classId = $insertStmt->execute();
		expDebug::dPrint('Class clone query result'. $resultClsId);
		expDebug::dPrint(' $classId = ' . print_r($classId, true) , 3);
		make_audit_trail_entries($fields, $loggedInUserId , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_administration_clone','cloneclass',
				array('course_id', 'title', 'code', 'short_description', 'description', 'lang_code','delivery_type','currency_type','price','status','author_vendor','min_seats','max_seats','waitlist_count','location_id','registration_end_on'));

		// Getting drupal userid
		$userSelect = db_query('SELECT uid uid, name uname FROM users WHERE LOWER(name) = (select lower(user_name) from slt_person where id='.$loggedInUserId.')');
		$userResult = $userSelect->fetchAll();
		$drupalUserId = $userResult[0]->uid;

		// Getting delivery type name
		$delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$queryResult[0]->delivery_type."'")->fetchField();

		// Getting vocabulary id
		$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();

		$vActivityId = $classId;

		$oNode = new StdClass();
		$price = empty($queryResult[0]->price)? 0 : $queryResult[0]->price;
		$oNode->uid = $drupalUserId;
		$oNode->language ='en';
		$oNode->title = $queryResult[0]->title;
		$oNode->body = $queryResult[0]->description;
		$oNode->model = $delivery_type_name.'-Code-'.$queryResult[0]->code;
		$oNode->sell_price = $price;
		$oNode->cost = $price;
		$oNode->list_price = $price;
		$oNode->currency_type = $queryResult[0]->currency_type;

		$oNode->type = 'product';
		$oNode->created = time();
		$oNode->changed = time();
		$oNode->teaser_include = 1;
		$oNode->revision = 1;
		$node->teaser = $queryResult[0]->description;

		$oNode->format = 1;
		$oNode->log = '';
		$oNode->name = $drupalUserId;
		$oNode->date ='';
		$oNode->status = ($queryResult[0]->status == 'lrn_cls_sts_atv') ? 1 : 0;
		$oNode->promote = 0;
		$oNode->sticky = 0;

		$oNode->op = "Save";
		$oNode->submit = "Save";
		$oNode->preview = "Preview";
		$oNode->form_id = "product_node_form";


		// Implemention of Expertus Meeting intergration : Start
		$vcNode = new StdClass();

		$vcNode->uid = $drupalUserId;
		$vcNode->language ='en';
		$vcNode->title = $queryResult[0]->title;
		$vcNode->body = $queryResult[0]->description;
		$vcNode->model = $delivery_type_name.'-Code-'.$queryResult[0]->code;
		$vcNode->sell_price = $price;
		$vcNode->cost = $price;
		$vcNode->list_price = $price;

		$vcNode->type = 'meeting';
		$vcNode->created = time();
		$vcNode->changed = time();
		$vcNode->teaser_include = 1;
		$vcNode->revision = 1;
		$vcNode->teaser = $queryResult[0]->description;

		$vcNode->format = 1;
		$vcNode->log = '';
		$vcNode->name = $drupalUserId;
		$vcNode->date ='';
		$vcNode->status = ($queryResult[0]->status == 'lrn_cls_sts_atv') ? 1 : 0;
		$vcNode->promote = 0;
		$vcNode->sticky = 0;

		$vcNode->op = "Save";
		$vcNode->submit = "Save";
		$vcNode->preview = "Preview";
		$vcNode->form_id = "product_node_form";

		if($vid) {

			$delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
			if(empty($delivery_type_all_tid)) {
				$terms               = new stdClass;
				$terms->name         = '--All Delivery types--';
				$terms->vid          = $vid;
				taxonomy_term_save($terms);
				$delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Delivery types--', ':vid' => $vid))->fetchField();
			}
			$oNode->taxonomy_catalog['und'] = array();
			$oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
			$delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$queryResult[0]->delivery_type."' and vid = ".$vid)->fetchField();
			$dtTerms = array(
					'name' => $delivery_type_name,
					'vid' => $vid,
					'description' => $queryResult[0]->delivery_type,
					'parent' => $delivery_type_all_tid,
			);
			if(empty($delivery_type_tid)) {
				$oNode->taxonomy_catalog['und'][] = array('tid' => cloneTaxonomyVocabTerms($dtTerms));
			}
			else {
				$oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
			}
			$delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$queryResult[0]->delivery_type."' and vid = ".$vid)->fetchField();
			$class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$vActivityId."' and vid = ".$vid)->fetchField();
			$dtTerms1 = array(
					'name' => $queryResult[0]->title.' : '.$queryResult[0]->code,
					'vid' => $vid,
					'description' => $vActivityId,
					'parent' => $delivery_type_tid
			);
			if(!empty($class_tid)){
				$dtTerms1['tid'] =  $class_tid;
			}
			$tid = cloneTaxonomyVocabTerms($dtTerms1);
			expDebug::dPrint(' Taxonomy id: $tid = ' . print_r($tid, true) , 4);

			$oNode->taxonomy_catalog['und'] = array();
			$oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
			$oNode->teaser='';
			$oNode->validated=1;

			node_save($oNode);

			$vNodeId =  $oNode->nid;

			expDebug::dPrint(' $vNodeId - $vActivityId - $createUserId = ' . $vNodeId .' - '. $vActivityId.' - '.$createUserId , 4);

			$nodeinsertStmt = db_insert('slt_node_learning_activity');
			$nodefields = array(
					'node_id' => $vNodeId,
					'entity_id' => $vActivityId,
					'entity_type' => 'cre_sys_obt_cls',
					'createdby' => $loggedInUserId,
					'createdon' => now(),
					'updatedby' => $loggedInUserId,
					'updatedon' => now()
			);
			$nodeinsertStmt->fields($nodefields);
			$nodeinsertStmt->execute();


			// Node creation for meeting integration : Start

			if($queryResult[0]->delivery_type =='lrn_cls_dty_vcl'){
				node_save($vcNode);

				$vcnid = $vcNode->nid;

				$nodeinsertStmt1 = db_insert('slt_node_learning_activity');
				$nodefields1 = array(
						'node_id' => $vcnid,
						'entity_id' => $vActivityId,
						'entity_type' => 'meeting',
						'createdby' => $loggedInUserId,
						'createdon' => now(),
						'updatedby' => $loggedInUserId,
						'updatedon' => now()
				);
				$nodeinsertStmt1->fields($nodefields1);
				$nodeinsertStmt1->execute();
			}
		}

		if($form_state['values']['checkbox-attributes-content_list'] == 1){
			// For Clone course_content_mapper table
			$contentFields = array('course_id', 'content_id', 'title', 'content_code', 'mastery_score',
					'max_attempts', 'validity_days', 'launch_url', 'sequence_number', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$contentquery = db_select('slt_course_content_mapper', 'content');
			$contentquery->fields('content',$contentFields);
			$contentquery->addExpression($classId,'class_id');
			$contentquery->addExpression($loggedInUserId,'created_by');
			$contentquery->addExpression($loggedInUserId,'updated_by');
			$contentquery->addExpression('now()','created_on');
			$contentquery->addExpression('now()','updated_on');
			$contentquery->condition('content.class_id', $clsId,'=');
			$contentquery->condition('content.course_id', $entityId,'=');
			expDebug::dPrintDBAPI('Content clone query', $contentquery);

			db_insert('slt_course_content_mapper')
			->from($contentquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-survey'] == 1 || $form_state['values']['checkbox-attributes-assessment'] == 1){
			// For Clone survey_mapping table
			$surveyFields = array('object_type', 'survey_id', 'view_option', 'no_of_attempts', 'pre_status',
			 'after_completion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$surveyquery = db_select('slt_survey_mapping', 'sur_map');
			$surveyquery->fields('sur_map',$surveyFields);
			$surveyquery->addExpression($classId,'object_id');
			$surveyquery->addExpression($loggedInUserId,'created_by');
			$surveyquery->addExpression('now()','created_on');
			$surveyquery->addExpression($loggedInUserId,'updated_by');
			$surveyquery->addExpression('now()','updated_on');
			if($form_state['values']['checkbox-attributes-survey'] == 1 && $form_state['values']['checkbox-attributes-assessment'] != 1){
				$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
				$surveyquery->condition('surv.type', 'sry_det_typ_sry','=');
			}else if($form_state['values']['checkbox-attributes-assessment'] == 1 && $form_state['values']['checkbox-attributes-survey'] != 1){
				$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
				$surveyquery->condition('surv.type', 'sry_det_typ_ass','=');
			}
			$surveyquery->condition('sur_map.object_id', $clsId,'=');
			$surveyquery->condition('sur_map.object_type', 'cre_sys_obt_cls','=');

			expDebug::dPrintDBAPI('Survey clone query', $surveyquery);

			db_insert('slt_survey_mapping')
			->from($surveyquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-business_rule'] == 1){
			// For Clone business_rule_mapping table
			$businessFields = array('entity_type', 'business_rule_code',
					'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$busmapquery = db_select('slt_business_rule_mapping', 'bus_map');
			$busmapquery->fields('bus_map',$businessFields);
			$busmapquery->addExpression($classId,'entity_id');
			$busmapquery->addExpression($loggedInUserId,'created_by');
			$busmapquery->addExpression('now()','created_on');
			$busmapquery->addExpression($loggedInUserId,'updated_by');
			$busmapquery->addExpression('now()','updated_on');
			$busmapquery->condition('bus_map.entity_id', $clsId,'=');
			$busmapquery->condition('bus_map.entity_type', 'cre_sys_obt_cls','=');

			expDebug::dPrintDBAPI('Business mapping rule clone query', $busmapquery);

			db_insert('slt_business_rule_mapping')
			->from($busmapquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-access'] == 1){
			// For Clone catalog_access table
			$cataccFields = array('entity_type', 'user_type', 'is_manager', 'org_id',
					'job_role', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$cataccquery = db_select('slt_catalog_access', 'cat_acc');
			$cataccquery->fields('cat_acc',$cataccFields);
			$cataccquery->addExpression($classId,'entity_id');
			$cataccquery->addExpression($loggedInUserId,'created_by');
			$cataccquery->addExpression('now()','created_on');
			$cataccquery->addExpression($loggedInUserId,'updated_by');
			$cataccquery->addExpression('now()','updated_on');
			$cataccquery->condition('cat_acc.entity_id', $clsId,'=');
			$cataccquery->condition('cat_acc.entity_type', 'cre_sys_obt_cls','=');

			expDebug::dPrintDBAPI('Catalog access clone query', $cataccquery);

			db_insert('slt_catalog_access')
			->from($cataccquery)
			->execute();

			// For Clone catalog_mro_mapping table
			$mromapFields = array('entity_type', 'group_id', 'group_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$mromapquery = db_select('slt_group_mapping', 'mro_map');
			$mromapquery->fields('mro_map',$mromapFields);
			$mromapquery->addExpression($classId,'entity_id');
			$mromapquery->addExpression($loggedInUserId,'created_by');
			$mromapquery->addExpression('now()','created_on');
			$mromapquery->addExpression($loggedInUserId,'updated_by');
			$mromapquery->addExpression('now()','updated_on');
			$mromapquery->addExpression("concat('".$entityId."-".$classId."')","parent_id");
			$mromapquery->addExpression("'learning'","parent_type");
			$mromapquery->condition('mro_map.entity_id', $clsId,'=');
			$mromapquery->condition('mro_map.entity_type', 'cre_sys_obt_cls','=');

			expDebug::dPrintDBAPI('Catalog mro mapping clone query', $mromapquery);

			db_insert('slt_group_mapping')
			->from($mromapquery)
			->execute();
			
			// For Clone user_mapping table #79414
			$usrmapFields = array('entity_type', 'user_id', 'access_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
			
			$usrmapquery = db_select('slt_user_access_mapping', 'usr_map');
			$usrmapquery->fields('usr_map',$usrmapFields);
			$usrmapquery->addExpression($classId,'entity_id');
			$usrmapquery->addExpression($loggedInUserId,'created_by');
			$usrmapquery->addExpression('now()','created_on');
			$usrmapquery->addExpression($loggedInUserId,'updated_by');
			$usrmapquery->addExpression('now()','updated_on');
			$usrmapquery->addExpression("concat('".$entityId."-".$classId."')","parent_id");
			$usrmapquery->addExpression("'learning'","parent_type");
			$usrmapquery->condition('usr_map.entity_id', $clsId,'=');
			$usrmapquery->condition('usr_map.entity_type', 'cre_sys_obt_cls','=');
			
			expDebug::dPrintDBAPI('User mapping clone query', $usrmapquery);
			
			db_insert('slt_user_access_mapping')
			->from($usrmapquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-tags'] == 1){
			// For Clone tag in tag_entity table.
			$tagentFields = array('entity_type', 'tagid', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$tagentquery = db_select('slt_tag_entity', 'tagent');
			$tagentquery->fields('tagent',$tagentFields);
			$tagentquery->addExpression($classId,'entity_id');
			$tagentquery->addExpression($loggedInUserId,'created_by');
			$tagentquery->addExpression('now()','created_on');
			$tagentquery->addExpression($loggedInUserId,'updated_by');
			$tagentquery->addExpression('now()','updated_on');
			$tagentquery->condition('tagent.entity_id', $clsId,'=');
			$tagentquery->condition('tagent.entity_type', 'Class','=');
			expDebug::dPrintDBAPI('Tag Entity clone query', $tagentquery);

			$tagId = db_insert('slt_tag_entity')
			->from($tagentquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-attachment'] == 1){
			// For Clone tag in entity_reading_mapper table.
			$entmapFields = array('entity_type', 'reading_id', 'reading_type', 'reading_content', 'showlnr', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$entmapquery = db_select('slt_entity_reading_mapper', 'entitymapping');
			$entmapquery->fields('entitymapping',$entmapFields);
			$entmapquery->addExpression($classId,'entity_id');
			$entmapquery->addExpression($loggedInUserId,'created_by');
			$entmapquery->addExpression('now()','created_on');
			$entmapquery->addExpression($loggedInUserId,'updated_by');
			$entmapquery->addExpression('now()','updated_on');
			$entmapquery->condition('entitymapping.entity_id', $clsId,'=');
			$entmapquery->condition('entitymapping.entity_type', 'cre_sys_obt_cls','=');
			expDebug::dPrintDBAPI('Attachment clone query', $entmapquery);

			db_insert('slt_entity_reading_mapper')
			->from($entmapquery)
			->execute();
		}
		updateGroupMappingDetails($classId, 'cre_sys_obt_cls', $loggedInUserId,'bulkadd',0,array('entity_id'=>$clsId,'entity_type'=>'cre_sys_obt_cls'));

		// For Clone Custom fields attached to TP in custom_fields table.
	 	$cusFields = array('entity_type', 'label', 'value', 'deleted_by', 'deleted_on', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

		$cusquery = db_select('slt_custom_fields', 'customfield');
		$cusquery->fields('customfield',$cusFields);
		$cusquery->addExpression($classId,'entity_id');
		$cusquery->addExpression($loggedInUserId,'created_by');
		$cusquery->addExpression('now()','created_on');
		$cusquery->addExpression($loggedInUserId,'updated_by');
		$cusquery->addExpression('now()','updated_on');
		$cusquery->condition('customfield.entity_id', $clsId,'=');
		$cusquery->condition('customfield.entity_type', 'class','=');
		expDebug::dPrintDBAPI('Custom field attached TP query', $cusquery);

		db_insert('slt_custom_fields')
		->from($cusquery)
		->execute();

		expDebug::dPrint('Enrollment clone query $oldClassDelType111 = '. $queryResult[0]->price, 5);		
		$classprice = ($queryResult[0]->price == '0.00' ) ? '' :$queryResult[0]->price; // Issue fixed 57032
		expDebug::dPrint('Enrollment clone query $$classprice = '. $classprice, 5);
	 	if(($oldClassDelType == 'lrn_cls_dty_wbt' || $oldClassDelType == 'lrn_cls_dty_vod') && empty($classprice)) {
			if($form_state['values']['checkbox-attributes-enrollment'] == 1){
				$busnissRule = 0;
				if($form_state['values']['checkbox-attributes-business_rule'] != 1){
					$busmapquery = db_select('slt_business_rule_mapping', 'bus_map');
					$busmapquery->addExpression('COUNT(1)','id');
					$busmapquery->condition('bus_map.entity_id', $clsId,'=');
					$busmapquery->condition('bus_map.entity_type', 'cre_sys_obt_cls','=');
					$busmapquery->condition('bus_map.business_rule_code', 'cre_sys_brl_rra','=');
					$busnissRule = $busmapquery->execute()->fetchField();
				}

				$enrSelect = db_select('slt_enrollment','enroll');

				if($busnissRule){
					$enrSelect->addExpression('MAX(enroll.id)','id');
					$enrSelect->groupBy('enroll.user_id');
				}else
					$enrSelect->addField('enroll', 'id');

				$enrSelect->addField('enroll', 'order_id');
				$enrSelect->condition('enroll.course_id',$entityId);
				$enrSelect->condition('enroll.class_id',$clsId);
				// Fixed For this ticket #0036809 , Clone only Enrolled and Inprogress  Users for Wbt and Vod.
				$enrSelect->condition('enroll.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
				$enrSelect->condition('enroll.master_enrollment_id', '','IS NULL');
				expDebug::dPrintDBAPI('Enrollment clone query', $enrSelect);

				$enrSelectResult = $enrSelect->execute()->fetchAll();
				expDebug::dPrint('result for cloning the enrollment records'.print_r($enrSelectResult,true));
				foreach($enrSelectResult as $value){
					expDebug::dPrint('result for cloning the enrollment records'.$value->id);
					// For Clone the Order for the class in order table.
					$orderFields = array('uc_order_id', 'user_id', 'order_status', 'order_sub_total', 'order_tax_amt', 'order_total_amt',
							'order_date_time', 'order_remarks', 'order_type', 'order_req_id', 'order_req_token', 'order_reference_number', 'order_notes',
							'order_statuschange_date', 'bill_firstname', 'bill_lastname', 'bill_cardtype', 'bill_cardno', 'bill_expiremonth', 'bill_expireyear',
							'bill_add1', 'bill_add2', 'bill_city', 'bill_state', 'bill_postalcode', 'bill_country', 'bill_phone', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
					$orderquery = db_select('slt_order', 'orders');
					$orderquery->fields('orders',$orderFields);
					$orderquery->addExpression($loggedInUserId,'created_by');
					$orderquery->addExpression('now()','created_on');
					$orderquery->addExpression($loggedInUserId,'updated_by');
					$orderquery->addExpression('now()','updated_on');
					$orderquery->condition('orders.id',$value->order_id,'=');
					expDebug::dPrintDBAPI('Orders table query', $orderquery);

					$newOrderId = db_insert('slt_order')
					->from($orderquery)
					->execute();

					expDebug::dPrint('New module id'.$newOrderId);
					// For Clone the Order items for the class in order_items table.
					 $orderItemFields = array('user_id', 'course_id', 'program_id', 'program_type', 'org_id', 'org_name', 'tax_exempted', 'location_id', 'location_name',
							'country_code','currency_type','convertion_rate', 'state_code', 'city', 'zipcode', 'nexus_val', 'vat_no', 'product_code', 'item_price', 'tax_amount',
							'refund_amount', 'order_status', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

					$orderItemsquery = db_select('slt_order_items', 'orderitem');
					$orderItemsquery->fields('orderitem',$orderItemFields);
					$orderItemsquery->addExpression($newOrderId,'order_id');
					$orderItemsquery->addExpression($classId,'class_id');
					$orderItemsquery->addExpression($loggedInUserId,'created_by');
					$orderItemsquery->addExpression('now()','created_on');
					$orderItemsquery->addExpression($loggedInUserId,'updated_by');
					$orderItemsquery->addExpression('now()','updated_on');
					$orderItemsquery->condition('orderitem.order_id', $value->order_id,'=');
					$orderItemsquery->condition('orderitem.course_id', $entityId,'=');
					$orderItemsquery->condition('orderitem.class_id', $clsId,'=');

					expDebug::dPrintDBAPI('Order Items Query for Clone class', $orderItemsquery);

					db_insert('slt_order_items')
					->from($orderItemsquery)
					->execute();

					// For Clone the enrollments for the class in enrollment table.
					$enrollFields = array('master_enrollment_id', 'mandatory', 'is_compliance', 'user_id', 'course_id', 'reg_status', 'reg_date',
							'reg_status_date', 'recertify_path', 'comp_status', 'comp_date', 'valid_from', 'valid_to', 'score', 'grade', 'waitlist_flag',
							'waitlist_priority', 'ical_sequence', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

					$enrollquery = db_select('slt_enrollment', 'enrollment');
					$enrollquery->fields('enrollment',$enrollFields);
					$enrollquery->addExpression($classId,'class_id');
					$enrollquery->addExpression($newOrderId,'order_id');
					$enrollquery->addExpression($loggedInUserId,'created_by');
					$enrollquery->addExpression('now()','created_on');
					$enrollquery->addExpression($loggedInUserId,'updated_by');
					$enrollquery->addExpression('now()','updated_on');
					$enrollquery->condition('enrollment.id', $value->id,'=');
					$enrollquery->condition('enrollment.course_id', $entityId,'=');
					$enrollquery->condition('enrollment.class_id', $clsId,'=');
					expDebug::dPrintDBAPI('Enrollment clone query', $enrollquery);

					$newEnrollId = db_insert('slt_enrollment')
					->from($enrollquery)
					->execute();

					//Clone the survey results,attendance summary and details table
					adminCloneAssessmentScoreForClass($value->id,$newEnrollId,$entityId,$clsId,$classId,$loggedInUserId);

					if($oldClassDelType == 'lrn_cls_dty_wbt' || $oldClassDelType == 'lrn_cls_dty_vod'){
						// For Clone Enrollment content mapped to a class.
						$enrollConFields = array('user_id', 'course_id', 'content_id', 'version_id', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

						$enrollConquery = db_select('slt_enrollment_content_mapping', 'enrollment');
						$enrollConquery->fields('enrollment',$enrollConFields);
						$enrollConquery->addExpression($newEnrollId,'enroll_id');
						$enrollConquery->addExpression($classId,'class_id');
						$enrollConquery->addExpression($loggedInUserId,'created_by');
						$enrollConquery->addExpression('now()','created_on');
						$enrollConquery->addExpression($loggedInUserId,'updated_by');
						$enrollConquery->addExpression('now()','updated_on');
						$enrollConquery->condition('enrollment.enroll_id', $value->id,'=');
						$enrollConquery->condition('enrollment.course_id', $entityId,'=');
						$enrollConquery->condition('enrollment.class_id', $clsId,'=');
						expDebug::dPrintDBAPI('Enrollment content mapping clone query', $enrollConquery);

						db_insert('slt_enrollment_content_mapping')
						->from($enrollConquery)
						->execute();
					}
				}


			}
		}
		syncSolrData('Enrollment');
		return $classId;
		//return $clsId;
  }catch (Exception $ex) {
  	watchdog_exception('adminCommonClone', $ex);
  	expertusErrorThrow($ex);
  }
}

function adminCloneEnrollmentForClass($form_state,$entityId,$clsId,$classId,$oldClassDelType){

	/* if($form_state['values']['checkbox-attributes-enrollment'] == 1){
		$busnissRule = 0;
		if($form_state['values']['checkbox-attributes-business_rule'] != 1){
			$busmapquery = db_select('slt_business_rule_mapping', 'bus_map');
			$busmapquery->addExpression('COUNT(1)','id');
			$busmapquery->condition('bus_map.entity_id', $clsId,'=');
			$busmapquery->condition('bus_map.entity_type', 'cre_sys_obt_cls','=');
			$busmapquery->condition('bus_map.business_rule_code', 'cre_sys_brl_rra','=');
			$busnissRule = $busmapquery->execute()->fetchField();
		}

		$enrSelect = db_select('slt_enrollment','enroll');

		if($busnissRule){
			$enrSelect->addExpression('MAX(enroll.id)','id');
			$enrSelect->groupBy('enroll.user_id');
		}else
			$enrSelect->addField('enroll', 'id');

		$enrSelect->addField('enroll', 'order_id');
		$enrSelect->condition('enroll.course_id',$entityId);
		$enrSelect->condition('enroll.class_id',$clsId);
		expDebug::dPrintDBAPI('Enrollment clone query', $enrSelect);

		$enrSelectResult = $enrSelect->execute()->fetchAll();
		expDebug::dPrint('result for cloning the enrollment records'.print_r($enrSelectResult,true));
		foreach($enrSelectResult as $value){
			expDebug::dPrint('result for cloning the enrollment records'.$value->id);
			// For Clone the Order for the class in order table.
			$orderFields = array('uc_order_id', 'user_id', 'order_status', 'order_sub_total', 'order_tax_amt', 'order_total_amt',
					'order_date_time', 'order_remarks', 'order_type', 'order_req_id', 'order_req_token', 'order_reference_number', 'order_notes',
					'order_statuschange_date', 'bill_firstname', 'bill_lastname', 'bill_cardtype', 'bill_cardno', 'bill_expiremonth', 'bill_expireyear',
					'bill_add1', 'bill_add2', 'bill_city', 'bill_state', 'bill_postalcode', 'bill_country', 'bill_phone', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
			$orderquery = db_select('slt_order', 'orders');
			$orderquery->fields('orders',$orderFields);
			$orderquery->addExpression($loggedInUserId,'created_by');
			$orderquery->addExpression('now()','created_on');
			$orderquery->addExpression($loggedInUserId,'updated_by');
			$orderquery->addExpression('now()','updated_on');
			$orderquery->condition('orders.id',$value->order_id,'=');
			expDebug::dPrintDBAPI('Orders table query', $orderquery);

			$newOrderId = db_insert('slt_order')
			->from($orderquery)
			->execute();

			expDebug::dPrint('New module id'.$newOrderId);
			// For Clone the Order items for the class in order_items table.
			$orderItemFields = array('user_id', 'course_id', 'program_id', 'program_type', 'org_id', 'org_name', 'tax_exempted', 'location_id', 'location_name',
					'country_code', 'state_code', 'city', 'zipcode', 'nexus_val', 'vat_no', 'product_code', 'item_price', 'tax_amount',
					'refund_amount', 'order_status', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$orderItemsquery = db_select('slt_order_items', 'orderitem');
			$orderItemsquery->fields('orderitem',$orderItemFields);
			$orderItemsquery->addExpression($newOrderId,'order_id');
			$orderItemsquery->addExpression($classId,'class_id');
			$orderItemsquery->addExpression($loggedInUserId,'created_by');
			$orderItemsquery->addExpression('now()','created_on');
			$orderItemsquery->addExpression($loggedInUserId,'updated_by');
			$orderItemsquery->addExpression('now()','updated_on');
			$orderItemsquery->condition('orderitem.order_id', $value->order_id,'=');
			$orderItemsquery->condition('orderitem.course_id', $entityId,'=');
			$orderItemsquery->condition('orderitem.class_id', $clsId,'=');

			expDebug::dPrintDBAPI('Order Items Query for Clone class', $orderItemsquery);

			db_insert('slt_order_items')
			->from($orderItemsquery)
			->execute();

			// For Clone the enrollments for the class in enrollment table.
			$enrollFields = array('master_enrollment_id', 'mandatory', 'is_compliance', 'user_id', 'course_id', 'reg_status', 'reg_date',
					'reg_status_date', 'recertify_path', 'comp_status', 'comp_date', 'valid_from', 'valid_to', 'score', 'grade', 'waitlist_flag',
					'waitlist_priority', 'ical_sequence', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$enrollquery = db_select('slt_enrollment', 'enrollment');
			$enrollquery->fields('enrollment',$enrollFields);
			$enrollquery->addExpression($classId,'class_id');
			$enrollquery->addExpression($newOrderId,'order_id');
			$enrollquery->addExpression($loggedInUserId,'created_by');
			$enrollquery->addExpression('now()','created_on');
			$enrollquery->addExpression($loggedInUserId,'updated_by');
			$enrollquery->addExpression('now()','updated_on');
			$enrollquery->condition('enrollment.id', $value->id,'=');
			$enrollquery->condition('enrollment.course_id', $entityId,'=');
			$enrollquery->condition('enrollment.class_id', $clsId,'=');
			expDebug::dPrintDBAPI('Enrollment clone query', $enrollquery);

			$newEnrollId = db_insert('slt_enrollment')
			->from($enrollquery)
			->execute();

			//Clone the survey results,attendance summary and details table
			adminCloneAssessmentScoreForClass($value->id,$newEnrollId,$entityId,$clsId,$classId);

			if($oldClassDelType == 'lrn_cls_dty_wbt' || $oldClassDelType == 'lrn_cls_dty_vod'){
				// For Clone Enrollment content mapped to a class.
				$enrollConFields = array('user_id', 'course_id', 'content_id', 'version_id', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

				$enrollConquery = db_select('slt_enrollment_content_mapping', 'enrollment');
				$enrollConquery->fields('enrollment',$enrollConFields);
				$enrollConquery->addExpression($newEnrollId,'enroll_id');
				$enrollConquery->addExpression($classId,'class_id');
				$enrollConquery->addExpression($loggedInUserId,'created_by');
				$enrollConquery->addExpression('now()','created_on');
				$enrollConquery->addExpression($loggedInUserId,'updated_by');
				$enrollConquery->addExpression('now()','updated_on');
				$enrollConquery->condition('enrollment.enroll_id', $value->id,'=');
				$enrollConquery->condition('enrollment.course_id', $entityId,'=');
				$enrollConquery->condition('enrollment.class_id', $clsId,'=');
				expDebug::dPrintDBAPI('Enrollment content mapping clone query', $enrollConquery);

				db_insert('slt_enrollment_content_mapping')
				->from($enrollConquery)
				->execute();
			}
		}


	} */
}
/**
 * Function for duplicating the records of all the attributes which is associated to the cloning Training program.
 *
 * Duplicating Survey,Assessment, access, tags, business rules etc attached to the TP will be cloned by the program id.
 * Enrollment will be cloned based upon the selection made by clicking on with enrollment option by the user.
 *
 * @param $entityId
 * 		- Program Id for duplicating
 * @param $enroll
 *
 */

function adminCloneForTrainingPlan($entityId,$form_state,$loggedInUserId){
	try{
		$options['attachedcourses']=1;
		$options['survey']=1;
		$options['certificate']=1;
		$options['access']=1;
		$options['tags']=1;
		$options['attachment']=1;
		$options['prerequisite']=1;
		$options['enrollment']=1;
		
	// To get the custom attribute with the textbox with the unique set as 1
		if(module_exists('exp_sp_administration_customattribute'))
		{
			$selectcustomresulttp = customattributevalues('cre_sys_obt_trp');
			expDebug::dPrint('$selectcustomresult1'. print_r($selectcustomresult1,true));
				
				
		}
		// For Clone training program in Program table
		$programFields = array('type', 'version', 'lang_code', 'locale_code', 'profile_code', 'description', 'short_desc', 'start_date', 'end_date', 'registration_open_date',
				'registration_close_date', 'max_capacity', 'expiry_date', 'price', 'is_sequenced', 'enforce_sequence', 'credit_valid_upto', 'expires_in_value', 'expires_in_unit', 'object_type', 'currency_type',
				'in_active_on', 'in_active_reason', 'author_vendor', 'status', 'additional_info', 'deleted_by', 'deleted_on', 'published_on', 'discussion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4','e1_cattr0',
		'e1_cattr1','e1_cattr2','e1_cattr3','e1_cattr4','e1_cattr5','e1_cattr6','e1_cattr7','e1_cattr8','e1_cattr9');

		
		$tpquery = db_select('slt_program', 'prg');
		$tpquery->fields('prg',$programFields);
		$tpquery->addExpression('CONCAT(prg.title,\' - (Copy \',(select COUNT(1) + 1 from slt_program where code like CONCAT(prg.code, \' - (Copy%\')), \')\')','title');
		$tpquery->addExpression('CONCAT(prg.code,\' - (Copy \',(select COUNT(1) + 1 from slt_program where code like CONCAT(prg.code, \' - (Copy%\')), \')\')','code');
		
		foreach($selectcustomresulttp as $load_key1 => $load_val1){
			
			expDebug::dPrint('load_key'. print_r($load_key1,true));
			expDebug::dPrint('load_val'. print_r($load_val1,true));
			$entity_id=$load_val1->entity_ref_tbl_col;
			
			//$tpquery->addExpression('CONCAT(prg.'.$entity_id.',\' - (Copy \',(select COUNT(1) + 1 from slt_program where code like CONCAT(prg.code, \' - (Copy%\')), \')\')',".$entity_id.'");
			
			$tpquery->addExpression('ifnull(CONCAT(prg.'.$entity_id.',\' - (Copy \',(select COUNT(1) + 1 from slt_program where code like CONCAT(prg.code, \' - (Copy%\')), \')\'),\'\')',".$entity_id.");
			
			//ifnull(concat(per.country,\'-\',per.state)
			//$tpquery->addExpression('CONCAT(ifnull(prg.'.$entity_id.',\' - (Copy \',(select COUNT(1) + 1 from slt_program where code like CONCAT(prg.code, \' - (Copy%\')), \')\'),\'\')',".$entity_id.'");
		}
		
		
		//ifnull(concat(per.country,\'-\',per.state),\'\'
		$tpquery->condition('prg.id', $entityId ,'=');
		expDebug::dPrintDBAPI('Training program clone query', $tpquery);

		$queryResult = $tpquery->execute()->fetchAll();

		expDebug::dPrint('Training program clone query result'. print_r($queryResult,true));

		$insertStmt = db_insert('slt_program');

		$fields = array(
				'type' => $queryResult[0]->type,
				'version' => $queryResult[0]->version,
				'lang_code' => $queryResult[0]->lang_code,
				'locale_code' => $queryResult[0]->locale_code,
				'profile_code' => $queryResult[0]->profile_code,
				'title' => $queryResult[0]->title,
				'code' => $queryResult[0]->code,
				'description' => $queryResult[0]->description,
				'short_desc' => $queryResult[0]->short_desc,
				'start_date' => $queryResult[0]->start_date,
				'end_date' => $queryResult[0]->end_date,
				'registration_open_date' => $queryResult[0]->registration_open_date,
				'registration_close_date' => $queryResult[0]->registration_close_date,
				'max_capacity' => $queryResult[0]->max_capacity,
				'expiry_date' => $queryResult[0]->expiry_date,
				'price' => $queryResult[0]->price,
				'is_sequenced' => $queryResult[0]->is_sequenced,
				'enforce_sequence' => $queryResult[0]->enforce_sequence,
				'credit_valid_upto' => $queryResult[0]->credit_valid_upto,
				'expires_in_value' => $queryResult[0]->expires_in_value,
				'expires_in_unit' => $queryResult[0]->expires_in_unit,
				'object_type' => $queryResult[0]->object_type,
				'currency_type' => $queryResult[0]->currency_type,
				'in_active_on' => $queryResult[0]->in_active_on,
				'in_active_reason' => $queryResult[0]->in_active_reason,
				'author_vendor' => $queryResult[0]->author_vendor,
				'status' => 'lrn_lpn_sts_itv',
				'additional_info' => $queryResult[0]->additional_info,
				'deleted_by' => $queryResult[0]->deleted_by,
				'deleted_on' => $queryResult[0]->deleted_on,
				'created_by' => $loggedInUserId,
				'created_on' => now(),
				'updated_by' => $loggedInUserId,
				'updated_on' => now(),
				'published_on' => $queryResult[0]->published_on,
				'discussion'  =>  $queryResult[0]->discussion,
				'custom0' =>  $queryResult[0]->custom0,
				'custom1' =>  $queryResult[0]->custom1,
				'custom2' => $queryResult[0]->custom2,
				'custom3' => $queryResult[0]->custom3,
				'custom4' => $queryResult[0]->custom4,
				'e1_cattr0' => $queryResult[0]->e1_cattr0,
				'e1_cattr1' => $queryResult[0]->e1_cattr1,
				'e1_cattr2' => $queryResult[0]->e1_cattr2,
				'e1_cattr3' => $queryResult[0]->e1_cattr3,
				'e1_cattr4' => $queryResult[0]->e1_cattr4,
				'e1_cattr5' => $queryResult[0]->e1_cattr5,
				'e1_cattr6' => $queryResult[0]->e1_cattr6,
				'e1_cattr7' => $queryResult[0]->e1_cattr7,
				'e1_cattr8' => $queryResult[0]->e1_cattr8,
				'e1_cattr9' => $queryResult[0]->e1_cattr9
		);

		// Select the fields and the values to be inserted
		$insertStmt->fields($fields);

		expDebug::dPrintDBAPI(' $insertStmt SQL = ' , $insertStmt);

		// Execute the insert statement. Fetch the new class id in slt_course_class table.
		$tpId = $insertStmt->execute();
		expDebug::dPrint('training program clone query result Final'. $resultClsId);
		//create Default Module 
		if($form_state['values']['checkbox-attributes-attachedcourses'] != 1){
			//$groupName = ($queryResult[0]->object_type == 'cre_sys_obt_crt') ? "Module Path - 1" : "Courses";
			//Module Name
			$dbmodquery = db_select('slt_module', 'modcrs');
			$dbmodquery->addField('modcrs','title');
			$dbmodquery->addField('modcrs','id');
			$dbmodquery->condition('modcrs.program_id', $entityId,'=');
			$dbmodquery->condition('modcrs.sequence', 1,'=');
			expDebug::dPrintDBAPI('Course attached to TP in main path clone query', $dbmodquery);
			$result = $dbmodquery->execute()->fetchAll();
			$groupName = $result[0]->title;
			$random_num = rand(1,99999);
 			$auto_code = "modco_".$random_num;
			$insertStmt = db_insert('slt_module');
		 	$fields = array(
		 			'program_id'  		 => $tpId,
		 			'title'              => $groupName,
		 			'code'               => '',
		 			'created_by'         => $loggedInUserId,
		 			'sequence'			 => 1,
		 			'created_on'         => now(),
		 			'updated_by'         => $loggedInUserId,
		 			'updated_on'         => now(),
		 			'status'             => 'spp_tpm_mdl_atv',
		 			'is_required'        => '1',
		 			'parent_id'			 => '0',
		
		 	);
		 	// Select the fields and the values to be inserted
		 	$insertStmt->fields($fields);
		 	expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);
		 	$module_id = $insertStmt->execute();
		 	
		 	//update module code 
 			$auto_code="modco_".$module_id;
 			$updateStmt = db_update('slt_module');
 			$updateStmt->condition('id', $module_id, '='); 				
 			$updateStmt->fields(array(
 				'code'     =>  $auto_code
 			));
 			expDebug::dPrintDBAPI(' update module code  = ', $updateStmt);
 			$numUpdated = $updateStmt->execute(); 
 			//Default Entry for Survey and assessment 
 			
 			if($form_state['values']['checkbox-attributes-survey'] == 1 || $form_state['values']['checkbox-attributes-assessment'] == 1){
	 			
	 			$surveyFields = array('object_type', 'survey_id', 'view_option', 'no_of_attempts', 'pre_status',
				 'after_completion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
	
				$surveyquery = db_select('slt_survey_mapping', 'sur_map');
				$surveyquery->fields('sur_map',$surveyFields);
				$surveyquery->addExpression($tpId,'object_id');
				$surveyquery->addExpression($module_id,'module_id');
				$surveyquery->addExpression($loggedInUserId,'created_by');
				$surveyquery->addExpression('now()','created_on');
				$surveyquery->addExpression($loggedInUserId,'updated_by');
				$surveyquery->addExpression('now()','updated_on');
				if($form_state['values']['checkbox-attributes-survey'] == 1 && $form_state['values']['checkbox-attributes-assessment'] != 1){
					$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
					$surveyquery->condition('surv.type', 'sry_det_typ_sry','=');
				}else if($form_state['values']['checkbox-attributes-assessment'] == 1 && $form_state['values']['checkbox-attributes-survey'] != 1){
					$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
					$surveyquery->condition('surv.type', 'sry_det_typ_ass','=');
				}
				$surveyquery->condition('sur_map.object_id', $entityId,'=');
				$surveyquery->condition('sur_map.module_id', $result[0]->id,'=');
				$surveyquery->condition('sur_map.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');
	
				expDebug::dPrintDBAPI('Survey clone query', $surveyquery);
	
				db_insert('slt_survey_mapping')
				->from($surveyquery)
				->execute();
 			}
 			
		}
		
		expDebug::dPrint(' training program cloned id = ' . print_r($tpId, true) , 3);
		make_audit_trail_entries($fields, $loggedInUserId , null ,$tpId, 'Inserted',$queryResult[0]->object_type,'exp_sp_administration_clone','Clone new Training Plan',
				array('title', 'code', 'short_desc','object_type', 'price','lang_code', 'status','currency_type',
						'expires_in_value', 'expires_in_unit','end_date','author_vendor'));

		$type = $queryResult[0]->object_type;
		//$vActivityId=$inObj->InsValue;
		$vActivityId = $tpId;
		$userid="";
		if($paramuserid=="")
		{
			global $user;
			$userid=$user->uid;
		}
		else
		{
			$userid=$paramuserid;
		}
		$oNode = new StdClass();
		$oNode->uid=$userid;
		$oNode->language='en';
		$oNode->title= str_replace("''","'",$queryResult[0]->title);
		$oNode->body= $queryResult[0]->short_desc;
		$delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$type."'")->fetchField();
		$oNode->model=$delivery_type_name.'-Code-'.$queryResult[0]->code;
		$oNode->sell_price= ($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;
		$oNode->cost=($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;
		$oNode->list_price=($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;
		$oNode->currency_type = $queryResult[0]->currency_type;

		$oNode->type='product';
		$oNode->created=time();
		$oNode->changed=time();
		$oNode->teaser_include=1;
		$oNode->revision= 1;

		$oNode->format=1;
		$oNode->log='';
		$oNode->name=$userid;
		$oNode->date='';
		$prgStatus = ($queryResult[0]->status == 'lrn_lpn_sts_atv') ? 1 : 0;
		$oNode->status= $prgStatus;
		$oNode->promote=0;
		$oNode->sticky=0;

		$oNode->op="Save";
		$oNode->submit="Save";
		$oNode->preview="Preview";
		$oNode->form_id="product_node_form";

		// Implemention of Forum intergration : Start

		$forumNode = new StdClass();

		$forumNode->uid=$userid;
		$forumNode->language='en';
		$forumNode->title= str_replace("''","'",$queryResult[0]->title);
		$forumNode->body= $queryResult[0]->short_desc;
		$delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$type."'")->fetchField();
		$forumNode->model=$delivery_type_name.'-Code-'.$queryResult[0]->code;
		$forumNode->sell_price= ($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;
		$forumNode->cost=($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;
		$forumNode->list_price=($queryResult[0]->price!=null && $queryResult[0]->price!='')?$queryResult[0]->price:0;

		$forumNode->type='forum';
		$forumNode->created=time();
		$forumNode->changed=time();
		$forumNode->teaser_include=1;
		$forumNode->revision= 1;

		$forumNode->format=1;
		$forumNode->log='';
		$forumNode->name=$userid;
		$forumNode->date='';
		$prgStatus = ($queryResult[0]->status == 'lrn_lpn_sts_atv') ? 1 : 0;
		$forumNode->status= $prgStatus;
		$forumNode->promote=0;
		$forumNode->sticky=0;

		$forumNode->op="Save";
		$forumNode->submit="Save";
		$forumNode->preview="Preview";
		$forumNode->form_id="product_node_form";

		// Implemention of Forum intergration : End

		expDebug::dPrint("Drupal form id  : ".$oNode->form_id , 4);

		$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
		expDebug::dPrintDBAPI("Delivery_type id : v_id - ", $vid);

		if($vid){
			$delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
			if(empty($delivery_type_all_tid)) {
				$terms               = new stdClass;
				$terms->name         = '--All Tp--';
				$terms->vid          = $vid;
				//$terms->description  = '';
				//$terms->parent       = '';
				taxonomy_term_save($terms);
				$delivery_type_all_tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name and vid = :vid",array(':name' => '--All Tp--', ':vid' => $vid))->fetchField();
			}
			$oNode->taxonomy_catalog['und'] = array();
			$oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);
			$delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$type."' and vid = ".$vid)->fetchField();

			expDebug::dPrintDBAPI("Delivery_type id :  - " , $delivery_type_tid);
			expDebug::dPrint("Training Plan Id :  - ".$vActivityId , 5);
			$dtTerms = array(
					'name' => $delivery_type_name,
					'vid' => $vid,
					'description' => $type,
					'parent' => $delivery_type_all_tid,
			);

			if(empty($delivery_type_tid)) {
				$oNode->taxonomy_catalog['und'][] = array('tid' => cloneTaxonomyVocabTerms($dtTerms));
			}
			else {
				$oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
			}
			$delivery_type_prg_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$type."' and vid = ".$vid)->fetchField();
			$class_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$vActivityId."' and vid = ".$vid)->fetchField();
			$dtTerms1 = array(
					'name' => $queryResult[0]->title.' : '.$queryResult[0]->code,
					'vid' => $vid,
					'description' => $vActivityId,
					'parent' => $delivery_type_prg_tid
			);
			if(!empty($class_tid)){
				$dtTerms1['tid'] =  $class_tid;
			}

			$tid = cloneTaxonomyVocabTerms($dtTerms1);
			expDebug::dPrint(' Taxonomy id: $tid = ' . print_r($tid, true) , 4);

			//$oNode->taxonomy_catalog['und'] = array();
			$oNode->taxonomy_catalog['und'][] = array('tid' => $tid);
			$oNode->teaser='';
			$oNode->validated=1;
			expDebug::dPrint("Tp Node :");
			expDebug::dPrint($oNode ,4);
			node_save($oNode);
			$vNodeId=  $oNode->nid;

			$insertStmt = db_insert('slt_node_learning_activity');
			$fields = array(
					'node_id'     => $vNodeId,
					'entity_id'   => $vActivityId,
					'entity_type' => $type,
					'createdby'   => $userid,
					'createdon'   => now(),
					'updatedby'   => $userid,
					'updatedon'   => now(),

			);

			// Select the fields and the values to be inserted
			$insertStmt->fields($fields);

			$lastinsertid = $insertStmt->execute();
			expDebug::dPrint(' $lastinserid = ' . print_r($lastinsertid, true) , 3);

			// Node creation for Forum integration : Start
			// Getting forum  vocabulary id
			$forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();

			$forumTerms               = new stdClass;
			$forumTerms->name         = str_replace("''","'",$queryResult[0]->title);
			$forumTerms->vid          = $forumvid;
			$forumTerms->description  = "TP";//$form_state['values']['short_desc'];
			$forumTerms->format       = $vActivityId;

			taxonomy_term_save($forumTerms);

			node_save($forumNode);

			$forumNodeId = $forumNode->nid;

			$nodeInsertForum = db_insert('slt_node_learning_activity');
			$nodeFieldsForum = array(
					'node_id'      => $forumNodeId,
					'entity_id'    => $vActivityId,  // class id
					'entity_type'  => 'TP',//$type,
					'createdby'    => $userid,
					'createdon'    => now(),
					'updatedby'    => $userid,
					'updatedon'    => now(),
					'custom0'      => 'forum'
			);
			$nodeInsertForum->fields($nodeFieldsForum);
			$nodeInsertForum->execute();

			// Node creation for Forum integration : End
		}
		/*if($form_state['values']['checkbox-attributes-survey'] == 1 || $form_state['values']['checkbox-attributes-assessment'] == 1){
			// For Clone survey_mapping table for survey attached to a training plan
			$surveyFields = array('object_type', 'survey_id', 'view_option', 'no_of_attempts', 'pre_status',
			 'after_completion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$surveyquery = db_select('slt_survey_mapping', 'sur_map');
			$surveyquery->fields('sur_map',$surveyFields);
			$surveyquery->addExpression($tpId,'object_id');
			$surveyquery->addExpression($loggedInUserId,'created_by');
			$surveyquery->addExpression('now()','created_on');
			$surveyquery->addExpression($loggedInUserId,'updated_by');
			$surveyquery->addExpression('now()','updated_on');
			if($form_state['values']['checkbox-attributes-survey'] == 1 && $form_state['values']['checkbox-attributes-assessment'] != 1){
				$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
				$surveyquery->condition('surv.type', 'sry_det_typ_sry','=');
			}else if($form_state['values']['checkbox-attributes-assessment'] == 1 && $form_state['values']['checkbox-attributes-survey'] != 1){
				$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
				$surveyquery->condition('surv.type', 'sry_det_typ_ass','=');
			}
			$surveyquery->condition('sur_map.object_id', $entityId,'=');
			$surveyquery->condition('sur_map.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');

			expDebug::dPrintDBAPI('Survey clone query', $surveyquery);

			db_insert('slt_survey_mapping')
			->from($surveyquery)
			->execute();
		}*/
		if($form_state['values']['checkbox-attributes-access'] == 1){
			// For Clone catalog_access table
			$cataccFields = array('entity_type', 'user_type', 'is_manager', 'org_id',
					'job_role', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$cataccquery = db_select('slt_catalog_access', 'cat_acc');
			$cataccquery->fields('cat_acc',$cataccFields);
			$cataccquery->addExpression($tpId,'entity_id');
			$cataccquery->addExpression($loggedInUserId,'created_by');
			$cataccquery->addExpression('now()','created_on');
			$cataccquery->addExpression($loggedInUserId,'updated_by');
			$cataccquery->addExpression('now()','updated_on');
			$cataccquery->condition('cat_acc.entity_id', $entityId,'=');
			$cataccquery->condition('cat_acc.entity_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');

			expDebug::dPrintDBAPI('Catalog access clone query', $cataccquery);

			db_insert('slt_catalog_access')
			->from($cataccquery)
			->execute();

			// For Clone catalog_mro_mapping table
			$mromapFields = array('entity_type', 'group_id', 'group_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$mromapquery = db_select('slt_group_mapping', 'mro_map');
			$mromapquery->fields('mro_map',$mromapFields);
			$mromapquery->addExpression($tpId,'entity_id');
			$mromapquery->addExpression($loggedInUserId,'created_by');
			$mromapquery->addExpression('now()','created_on');
			$mromapquery->addExpression($loggedInUserId,'updated_by');
			$mromapquery->addExpression('now()','updated_on');
			$mromapquery->addExpression("concat('".$tpId."-0')","parent_id");
			$mromapquery->addExpression("'program'","parent_type");
			$mromapquery->condition('mro_map.entity_id', $entityId,'=');
			$mromapquery->condition('mro_map.entity_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');

			expDebug::dPrintDBAPI('Catalog mro mapping clone query', $mromapquery);

			db_insert('slt_group_mapping')
			->from($mromapquery)
			->execute();
			
			// For Clone user_mapping table #79414
			$usrmapFields = array('entity_type', 'user_id', 'access_type', 'mro', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
				
			$usrmapquery = db_select('slt_user_access_mapping', 'usr_map');
			$usrmapquery->fields('usr_map',$usrmapFields);
			$usrmapquery->addExpression($tpId,'entity_id');
			$usrmapquery->addExpression($loggedInUserId,'created_by');
			$usrmapquery->addExpression('now()','created_on');
			$usrmapquery->addExpression($loggedInUserId,'updated_by');
			$usrmapquery->addExpression('now()','updated_on');
			$usrmapquery->addExpression("concat('".$tpId."-0')","parent_id");
			$usrmapquery->addExpression("'program'","parent_type");
			$usrmapquery->condition('usr_map.entity_id', $entityId,'=');
			$usrmapquery->condition('usr_map.entity_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');
				
			expDebug::dPrintDBAPI('User mapping clone query TP', $usrmapquery);
				
			db_insert('slt_user_access_mapping')
			->from($usrmapquery)
			->execute();
		}
		
		if($form_state['values']['checkbox-attributes-tags'] == 1){
			// For Clone tag in tag_entity table.
			$tagentFields = array('entity_type', 'tagid', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$tagentquery = db_select('slt_tag_entity', 'tagent');
			$tagentquery->fields('tagent',$tagentFields);
			$tagentquery->addExpression($tpId,'entity_id');
			$tagentquery->addExpression($loggedInUserId,'created_by');
			$tagentquery->addExpression('now()','created_on');
			$tagentquery->addExpression($loggedInUserId,'updated_by');
			$tagentquery->addExpression('now()','updated_on');
			$tagentquery->condition('tagent.entity_id', $entityId,'=');
			$tagentquery->condition('tagent.entity_type', array('Curricula','Certification','Learning Plan'),'IN');
			expDebug::dPrintDBAPI('Tag Entity clone query', $tagentquery);

			$tagId = db_insert('slt_tag_entity')
			->from($tagentquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-attachment'] == 1){
			// For Clone attachment in entity_reading_mapper table.
			$entmapFields = array('entity_type', 'reading_id', 'reading_type', 'reading_content', 'showlnr', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$entmapquery = db_select('slt_entity_reading_mapper', 'entmap');
			$entmapquery->fields('entmap',$entmapFields);
			$entmapquery->addExpression($tpId,'entity_id');
			$entmapquery->addExpression($loggedInUserId,'created_by');
			$entmapquery->addExpression('now()','created_on');
			$entmapquery->addExpression($loggedInUserId,'updated_by');
			$entmapquery->addExpression('now()','updated_on');
			$entmapquery->condition('entmap.entity_id', $entityId,'=');
			$entmapquery->condition('entmap.entity_type','cre_sys_obt_trp','=');
			expDebug::dPrintDBAPI('Attachment clone query', $entmapquery);

			db_insert('slt_entity_reading_mapper')
			->from($entmapquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-certificate'] == 1){
			// For Clone Certificate attached to the TP in entity_notification_mapping table.
			$certifymapFields = array('entity_type', 'notify_id', 'notify_text', 'notify_frequency', 'notify_frequency_type', 'notify_send_type', 'status', 'custom0');

			$certifymapquery = db_select('slt_entity_notification_mapping', 'entnotify');
			$certifymapquery->fields('entnotify',$certifymapFields);
			$certifymapquery->addExpression($tpId,'entity_id');
			$certifymapquery->condition('entnotify.entity_id', $entityId,'=');
			$certifymapquery->condition('entnotify.entity_type','cre_sys_obt_trp','=');
			expDebug::dPrintDBAPI('Certificate clone query', $certifymapquery);

			db_insert('slt_entity_notification_mapping')
			->from($certifymapquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-prerequisite'] == 1){
			// For Clone pre-requisite attached to TP in common_mapping table.
			$entmapFields = array('id2', 'type', 'object_type', 'prereq_type', 'value_string', 'required_for',
					'is_active', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$entmapquery = db_select('slt_common_mapping', 'commap');
			$entmapquery->fields('commap',$entmapFields);
			$entmapquery->addExpression($tpId,'id1');
			$entmapquery->addExpression($loggedInUserId,'created_by');
			$entmapquery->addExpression('now()','created_on');
			$entmapquery->addExpression($loggedInUserId,'updated_by');
			$entmapquery->addExpression('now()','updated_on');
			$entmapquery->condition('commap.id1', $entityId,'=');
			$entmapquery->condition('commap.object_type','cre_sys_obt_trp','=');
			expDebug::dPrintDBAPI('Pre-requisite clone query', $entmapquery);

			db_insert('slt_common_mapping')
			->from($entmapquery)
			->execute();
		}
		if($form_state['values']['checkbox-attributes-attachedcourses'] == 1){
			$modcrsFields = array('parent_id', 'sequence', 'title', 'description', 'short_desc',
					'start_date', 'end_date', 'is_required', 'status', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$dbmodquery = db_select('slt_module', 'modcrs');
			$dbmodquery->addField('modcrs','id');
			$dbmodquery->condition('modcrs.program_id', $entityId,'=');
			expDebug::dPrintDBAPI('Course attached to TP in main path clone query', $dbmodquery);
			$dbmodquery->orderBy('id','ASC');
			$resultQuery = $dbmodquery->execute()->fetchCol();

			foreach($resultQuery as $key => $values){

				// Module Creation
				$random_num=rand(1,99999);
				$auto_code="modco_".$random_num;

				$modcrsquery = db_select('slt_module', 'sltmod');
				$modcrsquery->fields('sltmod',$modcrsFields);
				$modcrsquery->addExpression($tpId,'program_id');
				$modcrsquery->addExpression("'$auto_code'",'code');
				$modcrsquery->addExpression($loggedInUserId,'created_by');
				$modcrsquery->addExpression('now()','created_on');
				$modcrsquery->addExpression($loggedInUserId,'updated_by');
				$modcrsquery->addExpression('now()','updated_on');
				$modcrsquery->condition('sltmod.program_id',$entityId,'=');
				$modcrsquery->condition('sltmod.id', $values,'=');
				expDebug::dPrintDBAPI('Course attached to TP in module table', $modcrsquery);

				$moduleId = db_insert('slt_module')
				->from($modcrsquery)
				->execute();

				expDebug::dPrint('New module id'.$moduleId.'value of old module id'.$value);


				//Group Id
				$grpModFields = array('code', 'title','sequence','id');
				$grpModquery = db_select('slt_module_groups', 'grp');
				$grpModquery->fields('grp',$grpModFields);
				$grpModquery->condition('grp.program_id', $entityId,'=');
				$grpModquery->condition('grp.module_id', $values,'=');
				expDebug::dPrintDBAPI('Module group List  bulk insert ', $grpModquery);
				$ListOfGrps = $grpModquery->execute()->fetchAll();
				$modcrsmapFields = array('course_id', 'object_type','is_required','sequence',
									'recommended_start_date', 'recommended_end_date', 'status', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
				foreach($ListOfGrps as $grp){
					//$prgName->title
					$InsertGroup = db_insert('slt_module_groups');
			        $GroupFields = array(
			                    'code'      => $grp->code,
			                    'title'     => $grp->title,
			    	            'sequence'  => $grp->sequence,
			    		       	'module_id' => $moduleId,
			                    'program_id' => $tpId,
			                    'created_by' => $loggedInUserId,
			        			'created_on' => now(),
			        			'updated_by' => $loggedInUserId,
			        			'updated_on' => now()
			              );
			        $InsertGroup->fields($GroupFields);
			        expDebug::dPrintDBAPI('Module group List  bulk insert ', $InsertGroup);
			        $Grpid = $InsertGroup->execute();
			        
			  		// Add module course mapping
			  		$modcrsmapquery = db_select('slt_module_crs_mapping', 'modcrsmap');
					$modcrsmapquery->fields('modcrsmap',$modcrsmapFields);
					$modcrsmapquery->addExpression($tpId,'program_id');
					$modcrsmapquery->addExpression($moduleId,'module_id');
					$modcrsmapquery->addExpression($Grpid,'group_id');
					$modcrsmapquery->addExpression($loggedInUserId,'created_by');
					$modcrsmapquery->addExpression('now()','created_on');
					$modcrsmapquery->addExpression($loggedInUserId,'updated_by');
					$modcrsmapquery->addExpression('now()','updated_on');
					$modcrsmapquery->condition('modcrsmap.program_id', $entityId,'=');
					$modcrsmapquery->condition('modcrsmap.module_id', $values,'=');
					$modcrsmapquery->condition('modcrsmap.group_id', $grp->id,'=');
					expDebug::dPrintDBAPI('Module crs mapping  bulk insert ', $modcrsmapquery);
			
					db_insert('slt_module_crs_mapping')
					->from($modcrsmapquery)
					->execute();
				
				}
				$select = db_select('slt_module_crs_mapping', 'map');
				$select->addExpression('COUNT(1)','count');
				$select->condition('map.program_id', $entityId, '=');
				$select->condition('map.module_id', $values, '=');
				expDebug::dPrintDBAPI("course count for module" ,$select);
				$crs_cnt = $select->execute()->fetchField();
				expDebug::dPrint('course count >>> '.$crs_cnt . "<< ". count($ListOfGrps),5);
				if((count($ListOfGrps)== 0) && ($crs_cnt > 0)){
			     	$modcrsmapquery = db_select('slt_module_crs_mapping', 'modcrsmap');
					$modcrsmapquery->fields('modcrsmap',$modcrsmapFields);
					$modcrsmapquery->addExpression($tpId,'program_id');
					$modcrsmapquery->addExpression($moduleId,'module_id');
					$modcrsmapquery->addExpression(0,'group_id');
					$modcrsmapquery->addExpression($loggedInUserId,'created_by');
					$modcrsmapquery->addExpression('now()','created_on');
					$modcrsmapquery->addExpression($loggedInUserId,'updated_by');
					$modcrsmapquery->addExpression('now()','updated_on');
					$modcrsmapquery->condition('modcrsmap.program_id', $entityId,'=');
					$modcrsmapquery->condition('modcrsmap.module_id', $values,'=');
					expDebug::dPrintDBAPI('Module crs mapping  bulk insert ', $modcrsmapquery);
			
					db_insert('slt_module_crs_mapping')
					->from($modcrsmapquery)
					->execute();
		     }
		     
		     //survey and assesment clone
			if($form_state['values']['checkbox-attributes-survey'] == 1 || $form_state['values']['checkbox-attributes-assessment'] == 1){
				// For Clone survey_mapping table for survey attached to a training plan
				$surveyFields = array('object_type', 'survey_id', 'view_option', 'no_of_attempts', 'pre_status',
				 'after_completion', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');
	
				$surveyquery = db_select('slt_survey_mapping', 'sur_map');
				$surveyquery->fields('sur_map',$surveyFields);
				$surveyquery->addExpression($tpId,'object_id');
				$surveyquery->addExpression($moduleId,'module_id');
				$surveyquery->addExpression($loggedInUserId,'created_by');
				$surveyquery->addExpression('now()','created_on');
				$surveyquery->addExpression($loggedInUserId,'updated_by');
				$surveyquery->addExpression('now()','updated_on');
				if($form_state['values']['checkbox-attributes-survey'] == 1 && $form_state['values']['checkbox-attributes-assessment'] != 1){
					$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
					$surveyquery->condition('surv.type', 'sry_det_typ_sry','=');
				}else if($form_state['values']['checkbox-attributes-assessment'] == 1 && $form_state['values']['checkbox-attributes-survey'] != 1){
					$surveyquery->leftJoin('slt_survey','surv','surv.id = sur_map.survey_id ');
					$surveyquery->condition('surv.type', 'sry_det_typ_ass','=');
				}
				$surveyquery->condition('sur_map.object_id', $entityId,'=');
				$surveyquery->condition('sur_map.module_id', $values,'=');
				$surveyquery->condition('sur_map.object_type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'),'IN');
	
				expDebug::dPrintDBAPI('Survey clone query', $surveyquery);
	
				db_insert('slt_survey_mapping')
				->from($surveyquery)
				->execute();
			}
		     
			}
		}
		updateGroupMappingDetails($tpId, $queryResult[0]->object_type, $loggedInUserId,'bulkadd',0,array('entity_id'=>$entityId,'entity_type'=>$queryResult[0]->object_type));
			// For Clone Custom fields attached to TP in custom_fields table.
			$cusFields = array('entity_type', 'label', 'value', 'deleted_by', 'deleted_on', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

			$cusquery = db_select('slt_custom_fields', 'customfield');
			$cusquery->fields('customfield',$cusFields);
			$cusquery->addExpression($tpId,'entity_id');
			$cusquery->addExpression($loggedInUserId,'created_by');
			$cusquery->addExpression('now()','created_on');
			$cusquery->addExpression($loggedInUserId,'updated_by');
			$cusquery->addExpression('now()','updated_on');
			$cusquery->condition('customfield.entity_id', $entityId,'=');
			$cusquery->condition('customfield.entity_type', 'cre_sys_obt_trp','=');
			expDebug::dPrintDBAPI('Custom field attached TP query', $cusquery);

			db_insert('slt_custom_fields')
			->from($cusquery)
			->execute();

		expDebug::dPrint(' $tpId = ' . print_r($tpId, true) , 3);
		return $tpId;
	}catch (Exception $ex) {
		watchdog_exception('adminCommonClone', $ex);
		expertusErrorThrow($ex);
	}
}

function cloneTaxonomyVocabTerms($termsTxt) {
  try {
      $terms               = new stdClass;
      $terms->name         = $termsTxt["name"];
      $terms->vid          = $termsTxt["vid"];
      if(!empty($termsTxt["description"])) {
        $terms->description  = $termsTxt["description"];
      }
      if(!empty($termsTxt["parent"])) {
        $terms->parent       = $termsTxt["parent"];
      }
      if(!empty($termsTxt["tid"])) {
        $terms->tid       = $termsTxt["tid"];
      }
      taxonomy_term_save($terms);
      return $terms->tid;
  }catch (Exception $ex) {
    watchdog_exception('cloneTaxonomyVocabTerms', $ex);
    expertusErrorThrow($ex);
  }
}

function ctoolsPopupForClone($formInfoList,$argsList, $type = ''){
	ctools_include('modal');
	ctools_include('ajax');
	expDebug::dPrint('checkign the arguments list'.print_r($formInfoList,true));
	// Prepare title for the modal dialog
	$title = empty($formInfoList[0]['title']) ? drupal_get_title() : $formInfoList[0]['title'];
	$renderMode = 'EDIT';

	$output = exp_ctools_modal_multi_ajaxonly_forms_render($formInfoList, $argsList, $renderMode, $otherArgsList);
	//expDebug::dPrint('checkign the arguments list'.print_r($argsList,true).'ddddd'.$argsList[1]);
	$commands = array();
	// If the form has not yet been rendered, render it.
	$commands[] = ctools_modal_command_display($title, $output);
	$commands[] = exp_destroy_ckeditors_on_modal_close();
	if($type == "video")
	{
		$commands[] = ajax_command_invoke(NULL, "hideUploadVideoControlAndDisplayLabel" );
		$commands[] = ajax_command_invoke(NULL, "putCustomLabelForContentAuthor" );
	}else if($type == "presentation")
	{
		$commands[] = ajax_command_invoke(NULL, "initializePresentationObjects" );
	}
	
	
	$commands[] = ajax_command_invoke(NULL, "cloneDestroyloader", array());
	
	$commands[] = ajax_command_refresh_narrow_search_results('onclose', true);
	expDebug::dPrint('ctoolsPopupForClone $commands = ' . print_r($commands, true) , 4);
	print ajax_render($commands);
	exit;
}

function getAvailableListofEntities($entityId,$clsId){
	expDebug::dPrint(' $$entityId = ' . $entityId.'$clsId'.$clsId , 4);

	$select = db_select('slt_course_class','cls');
	$select->addField('cls', 'delivery_type');
	$select->addField('cls', 'price');
	$select->condition('cls.course_id',$entityId,'=');
	$select->condition('cls.id',$clsId,'=');
	$result = $select->execute()->fetchAll();
	expDebug::dPrintDBAPI('getAvailableListofEntities query', $select);

	expDebug::dPrint(' values with result of available list= '. print_r($result,true) , 4);
	return $result;
}

function getEnrollmentDetailsForClass($entityId,$clsId){
	try{
		// Select table
		$selectStmt = db_select('slt_enrollment', 'enr');

		// Set conditions
		$selectStmt->condition('enr.course_id', $entityId);
		$selectStmt->condition('enr.class_id', $clsId);
		$selectStmt->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
		$selectStmt->condition('enr.reg_status', 'lrn_crs_reg_rsc', '!=');

		// Select fields and/or add expressions
		$selectStmt->addExpression('count(1)','count');
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result getEnrollmentDetailsForClass ' .$result , 4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getEnrollmentDetailsForClass', $ex);
		expertusErrorThrow($ex);
	}
}
?>
