<div class="top-record-div-left">
	<div>
    	<span>
    	<?php
    	require_once(drupal_get_path('module', 'exp_sp_myteam').'/exp_sp_myteam-catalog-search.inc'); 
    	global $catalog_reg;
    	$condClass = ($catalog_reg=='Course' && $results->object_type == 'Class' );
    	$tags  = getCatalogTags($results->cls_id,$results->object_type);
    	$tagSlice='';
    	$moreTag='';
    	$nodeId = '';
    	if(!empty($tags)){
    	  $tagSlice  = implode(", ",array_slice($tags,0,4));
    	  $moreTag  = implode(", ",$tags);
    	  $tagSlice = titleController('LNR-SEARCH-VIEW-DETAIL-TAG',sanitize_data($tagSlice),22);
    	}
    	$userId = getSltpersonUserId();
    	//42055: Provide an option in the admin share widget screen to pass site url
    	$referer_url = $_SERVER['HTTP_REFERER'];
    	if(isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)){
    		$curTitle = widgetTitleController($results->cls_title, 'catalog_page',$results->is_compliance);
    	}else	if((isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE)){
    		$curTitle = widgetTitleController($results->cls_title, 'catalog_page',$results->is_compliance);
    	}
    	else{
    		$recom_course = 0;
    		$man_course = 0;
    		$mrovalue= explode(',',$results->mro_id);
    		if (in_array('cre_sys_inv_rec', $mrovalue)){
    			$recom_course = 1;
    		}
    		if (in_array('cre_sys_inv_man', $mrovalue)){
    			$man_course = 1;
    		}
    	/*	if($results->is_compliance) {
    			$curTitle = titleController('LNR-SEARCH-VIEW-DETAIL-COMPLIANCE-COURSE-TITLE',$results->cls_title,72);
    		}
    		else if($man_course == 1)
    		{
    			$curTitle = titleController('LNR-SEARCH-VIEW-DETAIL-MANDATORY-COURSE-TITLE',$results->cls_title,72);
    		}
    		else if($recom_course == 1){
    			 $curTitle = titleController('LNR-SEARCH-VIEW-DETAIL-RECOMMENDED-COURSE-TITLE',$results->cls_title,72);
    		}
    		else {
    			$curTitle = titleController('LNR-SEARCH-VIEW-DETAIL-TITLE',$results->cls_title,72);
    		}*/
    		//$curTitle = titleController('LNR-SEARCH-VIEW-DETAIL-TITLE',$results->cls_title,72);
    		$curTitle = $results->cls_title;
    	} 
      if($man_course == 0){
    	if($results->object_type =='Class') {
    	  $results->mandatory_value = myteamFetchEnrollment($results->crs_id,$results->cls_id,$userId);
	    	}else if($results->object_type =='Course') {
    	  $results->mandatory_value = myteamFetchEnrollment($results->cls_id,$results->crs_id,$userId);
    	}else{
    	  $results->mandatory_value = myteamFetchProgram($results->cls_id,$userId);
    	}
      }else{
      	$results->mandatory_value = 'Y';
      }
    	if($catalog_reg == 'Course'){
    	  //$nodeIdFiveStar = getNodeActivityMapping($results->cls_id);
    	  $crs_class_id = getCompletedClassId($results->cls_id);
    	} 
    	
    	?>	
    	<?php 
    	$mroImageClassArr['cre_sys_inv_com'] =  '<div class="catalog-course-compliance-role-bg"><span class="catalog-course-compliance-bg-left"></span><span class="catalog-course-compliance-bg-middle">'.t('Compliance').'</span><span class="catalog-course-compliance-bg-right"></span></div></div>';
    	$mroImageClassArr['cre_sys_inv_man'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-mandatory-bg-left"></span><span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span><span class="catalog-course-mandatory-bg-right"></span></div></div>';
        //$mroImageClassArr['cre_sys_inv_opt'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-optional-bg-left"></span><span class="catalog-course-optional-bg-middle">'.strtolower(t('Optional')).'</span><span class="catalog-course-optional-bg-right"></span></div>';
        $mroImageClassArr['cre_sys_inv_rec'] =  '<div class="catalog-course-role-access-bg"><span class="catalog-course-recommended-bg-left"></span><span class="catalog-course-recommended-bg-middle">'.t('Recommended').'</span><span class="catalog-course-recommended-bg-right"></span></div></div>';
        $mrovalue= explode(',',$results->mro_id);
        if (in_array('cre_sys_inv_man', $mrovalue)){
        	$results->mro_id= 'cre_sys_inv_man';
        }else if (in_array('cre_sys_inv_rec', $mrovalue)){
        	$results->mro_id = 'cre_sys_inv_rec';
        }
        $mroImageClass = empty($results->mro_id) ? '' : $mroImageClassArr[$results->mro_id];
        if($results->mandatory_value=="Y"){
          $mroImageClass = $mroImageClassArr['cre_sys_inv_man'];
        }
        if($results->is_compliance){
          $mroImageClass = $mroImageClassArr['cre_sys_inv_com'];
        }
      //  $mroImageClass .= '<div class="clearBoth"></div>';
        // Change the link based on the url call //42055: Provide an option in the admin share widget screen to pass site url
        if(isset($_SESSION['widgetCallback']) && ($_SESSION['widgetCallback'] == true && (isset($_SESSION['widget']['catalog_display_parameters']) &&  ($_SESSION['widget']['pass_url']!=TRUE || empty($_SESSION['widget']['pass_url']))))) {
         $primaryPath = 'widget';
        } else {
         $primaryPath = 'learning';
        }
		?>
		<div class="content-title catalog-select-class-container  limit-title-row"><?php
        //42055: Provide an option in the admin share widget screen to pass site url
        if(isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false))
        	if(empty($mroImageClass))
        	$attributes_arr = array('attributes' => array('class' => 'limit-title spotlight-item-title without-mro-container vtip','title' => trim($results->cls_title),'target'=>'_blank'));
        else
            $attributes_arr = array('attributes' => array('class' => 'limit-title spotlight-item-title vtip','title' => trim($results->cls_title),'target'=>'_blank'));
        	
        else
        	if(empty($mroImageClass))
        	$attributes_arr = array('attributes' => array('class' => 'limit-title spotlight-item-title without-mro-container vtip','title' => trim($results->cls_title)));
        	else
        	$attributes_arr = array('attributes' => array('class' => 'limit-title spotlight-item-title vtip','title' => trim($results->cls_title)));?>
       <?php        	
       	if($results->object_type == 'Class') {    	
    		if($catalog_reg == 'Course'){
    			if(empty($mroImageClass)){
    			?>
    		<span title="<?php print sanitize_data(trim($results->cls_title)); ?>" class="limit-title spotlight-item-title without-mro-container vtip" ><?php print $curTitle; ?></span>
    		<?php }else{?>
    		<span title="<?php print sanitize_data(trim($results->cls_title)); ?>" class="limit-title spotlight-item-title vtip" ><?php print $curTitle; ?></span>
    		<?php } }else{
    			$attributes_arr['html'] = TRUE;
    			print l($curTitle, $primaryPath . '/class-details/'.$results->cls_id.'/'.$results->session_id,$attributes_arr);
    		}
    		
    	//	$exmpId = getRegisteredEnrollmentExmeptedorNot('class',$userId,$results->crs_id,$results->cls_id);
    	//	if(empty($exmpId))
    			print $mroImageClass;
    	}else if($results->object_type == 'Course') { 
    	  $attributes_arr['html'] = TRUE;
    	  print l($curTitle, $primaryPath .'/course-details/'.$results->course_node_id,$attributes_arr);
    	  expDebug::dPrint("VAlu e for class and course id".print_r($results,1));
    	 /* $class_array = explode(',',$results->crs_id);
    	  $waived_flag = false;
    	  foreach($class_array  as $class_id) {
    	  	$exmpId = getRegisteredEnrollmentExmeptedorNot('course',$userId,$results->cls_id,$class_id);
    	  	if(empty($exmpId)){
    	  		$waived_flag = true;
    	  		break;
    	  	}
    	  } 
    	 if($waived_flag == true) */
    	  	print $mroImageClass; 	
    	} else { 
    	 $attributes_arr['html'] = TRUE;
    	 print l($curTitle, $primaryPath .'/learning-plan-details/'.$results->cls_id.'/catalog',$attributes_arr);   
    	 
    	 //$exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$userId,$results->cls_id);
    	// if(empty($exmpId))
    	 	print $mroImageClass; 		
    	}
    	?>	
    	</span>
	</div>
	<div class="find-training-list-course">	 
		<?php $classname = ($results->object_type != 'Class' && $results->object_type!= 'Course' && $results->object_type!= 'cre_sys_obt_crt' && $results->course_to_complete == 0) ? 'LNR-SEARCH-VIEW-DETAIL-CODE-TP':'LNR-SEARCH-VIEW-DETAIL-CODE' ?>
		<div class="line-item-container float-left">
			<span id='srch-code-title' class='vtip' title="<?php print t('LBL096').": ".sanitize_data($results->cls_code);?>"><?php print titleController($classname,$results->cls_code,10); ?></span>
		</div>
		<?php if(($catalog_reg=='Course' && $results->cls_count != 0) || $condClass) {?>
		<div class="line-item-container float-left" >
			<span class="pipe-line">|</span>
			<span class='vtip' title="<?php print t('LBL038').": ".sanitize_data(t($results->language)); ?>">
		<?php //$langchar = subStringController($results->language,3);
		   if($condClass) {
		     print t(subStringController(t($results->language),3));
		   }else {
		   	 print titleController('LNR-SEARCH-VIEW-DETAIL-LANG',t($results->language),10);
		   }
		?>
		</span>
		</div>
		<?php  } ?>
	<!-- </div>	
	<div class="find-training-list">  -->
    <?php if($results->location != null && $results->object_type == 'Class') { ?>
    <div class="line-item-container float-left" >
			<span class="pipe-line">|</span>
			<span id="srch-location-title" class='vtip' title="<?php print t('Location').": ". sanitize_data($results->location);?>"><?php print titleController('LNR-SEARCH-VIEW-DETAIL-LOCATION-NAME',$results->location);/*subStringController($results->location,15);*/ ?></span>
		</div>
    <?php }else if($results->object_type == 'Course' && $results->cls_count == 1 && ($results->delivery_type_code == "lrn_cls_dty_ilt")) { ?>
    <div class="line-item-container float-left" >
     	<span class="pipe-line">|</span>
			<span id="srch-location-title" class='vtip' title="<?php print t('Location').": ". sanitize_data($results->session_details->loc_name);?>"><?php print titleController('LNR-SEARCH-VIEW-DETAIL-SESSION-NAME',$results->session_details->loc_name,15); ?></span>
		</div>
	<?php } 
	//Added by Vincent on Oct 28, 2013 for #0028593
	 if(empty($userId)){
	 	$defaultTimezone = date_default_timezone(false);
	 	$select = db_select('slt_profile_list_items','spli');
	 	$select->addField('spli','attr1');
	 	$select->addField('spli','attr4');
	 	$select->condition('spli.attr2',$defaultTimezone);
	 	$timezone_details = $select->execute()->fetchAssoc();
	 }
	 else{
	 $timezone_details = getPersonDetails($userId);
	 }
     $sessionDate='';
	if($results->delivery_type_code == "lrn_cls_dty_vcl" || $results->delivery_type_code == "lrn_cls_dty_ilt" ){
		$sDay_user = $results->session_details->vc_session_start_date_format;
		$sTime_user = $results->session_details->vc_session_start_time_format;
		$sTimeForm_user = $results->session_details->vc_session_start_time_form;
		$timedet ='';
		if( $results->delivery_type_code == "lrn_cls_dty_ilt"){
		$timedet =$timezone_details['attr1'].' '.$timezone_details['attr4'];
		}
		if($results->session_details->is_multi_session == 1){
			$eDay_user = $results->session_details->vc_session_start_end_format;
			$eTime_user = $results->session_details->vc_session_end_time_format;
			$eTimeForm_user = $results->session_details->vc_session_end_time_form;
			$sessionDate_user = $sDay_user .' '. $sTime_user .' '. '<span class="meridian-time time-zone-text">'.$sTimeForm_user.'</span>'.' to '.$eDay_user. ' ' .$eTime_user.' <span class="meridian-time time-zone-text">'.$eTimeForm_user.'</span>'.' '.' <span class="time-zone-offset">'.$timedet.'</span>';
		}else{
			$eTime_user = $results->session_details->vc_session_end_time_format;
			$eTimeForm_user = $results->session_details->vc_session_end_time_form;
			$sessionDate_user = $sDay_user .' '. $sTime_user.' '.'<span class="meridian-time time-zone-text">'.$sTimeForm_user.'</span>'.' to '.$eTime_user.' <span class="meridian-time time-zone-text">'.$eTimeForm_user.'</span>'.' '.' <span class="time-zone-offset">'.$timedet.'</span>';
		}
	}
	 if($results->delivery_type_code == "lrn_cls_dty_ilt"){
	 	$tzcode = $results->session_details->attr4;
	 	$zone = $results->session_details->attr1;
		$sDay = $results->session_details->ilt_session_start_date_format;
		$sTime = $results->session_details->ilt_session_start_time_format;
		$sTimeForm = $results->session_details->ilt_session_start_time_form;
		if($results->session_details->is_multi_session == 1){
			$eDay = $results->session_details->ilt_session_start_end_format;
			$eTime = $results->session_details->ilt_session_end_time_format;
			$eTimeForm = $results->session_details->ilt_session_end_time_form;
			$sessionDate = $sDay .' '. $sTime .' '. '<span class="meridian-time time-zone-text">'.$sTimeForm.'</span>'.' to '.$eDay. ' ' .$eTime.' <span class="meridian-time time-zone-text">'.$eTimeForm.'</span>'.' '.$zone.' '.$tzcode;
		}else{
			$eTime = $results->session_details->ilt_session_end_time_format;
			$eTimeForm = $results->session_details->ilt_session_end_time_form;
			$sessionDate = $sDay .' '. $sTime.' '.'<span class="meridian-time time-zone-text">'.$sTimeForm.'</span>'.' to '.$eTime.' <span class="meridian-time time-zone-text">'.$eTimeForm.'</span>'.' '.$zone.' '.$tzcode;
		}
	}
	if($results->object_type == 'Course' && $results->cls_count >1 ){
		$sessionDate='';
		$sessionDate_user='';
	}
	?>
		<?php if(!empty($sessionDate_user) && ($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")) { ?>
		<div class="line-item-container float-left" >
		<span class="pipe-line">|</span>
		<span class="vtip" title='<?php print strip_tags($sessionDate_user); ?>'>
		<?php
       $session_timezone = $results->session_details->session_timezone;
	 	if(($session_timezone!=$timezone_details['attr2'] && $results->delivery_type_code == "lrn_cls_dty_ilt") || $_SESSION['widgetCallback']===TRUE ){
	 	print titleController('LNRSEARCH-DATE-TIME',$sessionDate_user,5);?></span>
		<?php }elseif($results->session_details->is_multi_session == 1){ print titleController('LNRSEARCH-DATE-TIME-MULTISESSION',$sessionDate_user,5);?></span>
		<?php }else{ print $sessionDate_user;?></span><?php }?>
		<?php $session_timezone = $results->session_details->session_timezone;
		if($session_timezone!=$timezone_details['attr2'] && $results->delivery_type_code == "lrn_cls_dty_ilt" ){
			if((isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']===TRUE))
		$qtip=qtip_popup_paint($results->session_details->session_id ,$sessionDate,$results->session_details->is_multi_session);
			else
			$qtip=qtip_popup_paint($results->session_details->session_id ,$sessionDate,$results->session_details->is_multi_session,1);
	?>
	
	<?php echo $qtip;}?>
	</div>
	<?php } ?>
	
	<?php if($results->object_type != 'Class' && $results->object_type == 'cre_sys_obt_crt' && !empty($results->expires_in_value)) { ?>
	<?php   $expiresInValueStr = prepareCrtExpiresInDisplayString($results->expires_in_value, $results->expires_in_unit)?>
		<div class="line-item-container float-left" >
			<span class="pipe-line">|</span>
			<span class="vtip srch-expires-in" title="<?php print t('LBL233') . '&nbsp;:&nbsp;' . $expiresInValueStr . '&nbsp;' . t('MSG568'); ?>">
			  <?php print titleController('LNRSEARCH-CRT-EXPIRE',t('LBL233') . '&nbsp;:&nbsp;' . $expiresInValueStr . '&nbsp;' . t('MSG568')); ?>
			</span>
		</div>
	<?php } ?>
	<?php if($results->object_type != 'Class' && $results->object_type != 'Course' && $results->course_to_complete > 0){ 
		$course_count = getCourseCntEnrollment($userId, $results->cls_id,1);
		?>
		<div class="line-item-container float-left" >
		<span class="pipe-line">|</span>
		<span class="vtip srch-courses-to-complete" title="<?php print t('LBL606'); ?>">  <?php print t('LBL606') ?> : <?php print $course_count;?></span>
		</div>
    <?php }else if($results->object_type == 'Course' && $results->cls_count >1 ){?>
    <div class="line-item-container float-left" >
      <span class="pipe-line">|</span>
      <span class="vtip" id="srch-class-count" title="<?php print t('LBL353'); ?>">  <?php print t('LBL353') ?> : <?php print $results->cls_count;?></span>
    </div>
    <?php }?>			
		
	</div>
	
	<div class="record-div find-training-txt">
	 	<div class="refer-course-link">
 		  <?php  expDebug::dPrint('slt_widget Session ' . print_r($_SESSION['widget']['catalog_display_parameters'],true), 4); //Embed widget related work (check the display parameter) // print l(t('Refer'), '', array('attributes' => array('onclick' => "$('body').data('refercourse').getReferDetails('$results->cls_id','cre_sys_obt_cls','lnr-catalog-search'); return false;"))); ?>
		</div> 
		<?php if(!isset($_SESSION['widget']['catalog_display_parameters']) || (isset($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']==TRUE)){?>
		<!--<div class="record-div find-training-txt">-->
		  <?php
		  $description = str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), $results->cls_short_description); 
		 // $catalogDes=preg_replace(array("'/<([^<\/>]*)>([\s]*?|(?R))<\/\1>/imsU'"),array(""), $description);
		  // return preg_replace('#<p>\s*+(<br\s*/*>)?\s*</p>#i', '', $description);
		  $crsMoreType= ($results->object_type == 'Course') ? 'cre_sys_obt_crs' : (($results->delivery_type_code) ? $results->delivery_type_code : $results->object_type);?>
		  <div class="content-description limit-desc-row <?php echo $crsMoreType; ?>" ><span class="limit-desc vtip"><span class="cls-learner-descriptions"><?php $desctxt = $description; print $desctxt; ?></span></span></div>
		  
		<!--</div>-->
		<div class="record-div find-training-txt">
		<input type="hidden" id = "lnrsearch-node" value="<?php print $results->node_id?>"></input>
		  <?php 
		  /*if($results->object_type == 'Class'){
		  	 $getRegister = getRegisteredOrNot($results->crs_id,$results->cls_id);
		  }elseif($results->object_type == 'Course'){
		  	 $getRegister = getRegisteredOrNot($results->cls_id,$results->crs_id,'','catalog-search');
		  }else{
		  	$getRegister = getObjectRegisteredOrNot($results->cls_id);
		  }*/
		 $getRegister = $results->getRegister;
		?>
		</div>
		<?php 
		if($catalog_reg == 'Course' && $results->object_type == 'Course') {
		  $settings = array(
		                  'content_type' => 'Class',
		                  'content_id' => $results->nodeIdFiveStar,
		                  'stars' => 5,
		                  'autosubmit' => TRUE,
		                  'allow_clear' => FALSE,
		                  'required' => FALSE,
		                  'tag' => 'vote',
		                  'style' => 'average',
		                  'text' => 'average',
		                  'startype' => 'lrn_cls_dty_ilt',
		                  'catalog_crs' => $results->object_type,
		                );
		}else {
		  $settings = array(
						  'content_type' => $results->object_type,
						  'content_id' => $results->node_id,
						  'stars' => 5,
						  'autosubmit' => TRUE,
						  'allow_clear' => FALSE,
						  'required' => FALSE,
						  'tag' => 'vote',
              'style' => 'average',
      				'text' => 'average',
		  				'startype' => ($results->delivery_type_code) ? $results->delivery_type_code : $results->object_type,
 						);
		}

		  print "<div class='content-rate-tags'><div id='cls-node-".$results->node_id."' class='float-left'>".drupal_render(drupal_get_form('fivestar_rating_widget', '', $settings))."</div>";
  	if ($moreTag!=''){
		  	$class = '';
		  	if(module_exists('exp_sp_fivestar') and ($getRegister->comp_status == "lrn_crs_cmp_cmp" || $getRegister->master_enrolled_status == "lrn_tpm_ovr_cmp")){
		  		$class='class="tagInfoWithRating"';} 
  	?>
  					<span id="tagInfoDiv" <?php echo $class;?> >
  				  		<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {?>
  				  			<span class='pipe-line'>|</span>
  				  		<?php }?>
  				  		<span title="<?php print t('LBL191');?>" class="tagTitle vtip"><?php print t('LBL191');?> : </span> 
  				  		<span class="tagContent vtip" title="<?php print sanitize_data($moreTag);?>"><?php  /*$tagSlice = titleController('LNR-SEARCH-VIEW-DETAIL-TAG',$tagSlice,18);*/ print $tagSlice;?></span>
  				  </span>
  		<?php } 
  			print '</div>';
		  ?>
		<?php 			global $user;
		//if(!isset($_GET['callfrom']) && $_GET['callfrom']!='widget') 58017-share is not being shown in catalog page
		if(!isset($_SESSION['widgetCallback']) || ($_SESSION['widgetCallback'] != true)){
		      if ($user->uid) {
		        $pastDateCount = 0;
		        if ($results->object_type == 'Class') {
              $pastDateCount = checkPastDateOrNot($results->cls_id, $results->delivery_type_code);
            }
            if ($pastDateCount <= 0) {
            	if($catalog_reg != 'Course' || ($catalog_reg == 'Course' && $results->object_type != 'Class')) {
            		 
		?>
		<div id="shareoption">
			<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) { ?>
        	<span class="pipe-line">|</span>
        	<?php }?>
        	
              <?php  if($results->object_type == 'Class'){
              	$getObjectType='cre_sys_obt_cls';
              	$delType='Class';
              } else if ($results->object_type == 'Course'){
              	$getObjectType='cre_sys_obt_crs';
              	$delType='Course';
              }else {
               	$getObjectType = $results->object_type; 
               	$delType = getTypeTitle($results); 
              }?>  
              <?php
              $qtipIdInit        = $results->cls_id.'_'.$getObjectType;
              $qtipOptAccessObj      = "{'entityId':".$results->cls_id.",
              'entityType':'".$getObjectType."',
              'popupDispId':'widget_share_qtip_visible_disp_$qtipIdInit',
              'wid':440,
              'heg':'350',
              'postype':'middle',
              'linkid':'widget-share-$results->cls_id'}";
              ?>
        	<div id="widget_share_qtip_visible_disp_<?php  print $qtipIdInit ?>" class="clsShareOption">
        	<span title="<?php print t('Share');?>" class="share-tab-icon vtip"></span>
        	<span title="<?php print t('Share');?>" class="vtip"><?php print '<a id="widget-share-'.$results->cls_id.'" class="single-share-link" onclick = "$(\'body\').data(\'refercourse\').getEmbedReferDetails(\'lnr-catalog-search\', \''.$delType.'\', '.$qtipOptAccessObj.',\'Popup\');">'. t('Share') .'</a>'; ?></span>
        	<span style=" position:absolute; left:0px; top:0px;" class="qtip-popup-visible" id="visible-popup-<?php print $results->cls_id;?>"></span>
        	</div>
        	<span id="visible-popup-<?php print $results->cls_id;?>" class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
      </div>
      <?php  if ($results->object_type == 'Course') { ?>
      <div id="request-class-option" class="request-class">
        	<span class="pipe-line">|</span>
        	<span title="<?php print t('Request Class'); ?>" class="request-class-icon vtip"></span>
        	<span>
        <?php if(!empty($tags)){ ?>	
        	<a class="request-option vtip" title="<?php print t('Request Class'); ?>" onclick="$('body').data('requestclass').openRequestClassDialog('<?php print $results->cls_id;?>', 'lnr-catalog-search');"><?php print titleController('LNRSEARCH-REQUEST-CLS',t('Request Class')); //Request Class ?></a>
        <?php }else{?>	
        	<a class="request-option" onclick="$('body').data('requestclass').openRequestClassDialog('<?php print $results->cls_id;?>', 'lnr-catalog-search');"><?php print t('Request Class'); //Request Class ?></a>
        <?php }?>
        	</span>
    	</div>
    	<?php }
    	     }
		      } 
         }
       }?>
	<?php }?>       
	</div>	
</div>


<?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {?>
		<?php if($getRegister->comp_status == "lrn_crs_cmp_cmp" || $getRegister->master_enrolled_status == "lrn_tpm_ovr_cmp"){ ?>
		<script type=text/javascript>
			$("#lnr-catalog-search").data("lnrcatalogsearch").fiveStarCheckCompStatus('<?php print $results->node_id;?>','<?php ($results->object_type == 'Course') ? print 'Class': print $results->object_type;?>','<?php ($results->object_type == 'Course') ? print $crs_class_id : print $results->node_id; ?>');
		</script>
		<?php } else { ?>
		<script type=text/javascript>
			$("body").data("learningcore").fiveStarCheck('<?php print $results->node_id;?>','<?php print 'cls-node-'?>');
		</script>
		<?php } ?>
		
		<?php if(!empty($results->vote_value)) {
		$rating_values = getRatingDetails($results->node_id); 
		$passParams 	= "data=$rating_values";
		// print $passParams;
		?>
		<script type=text/javascript>
		var data="<?php print $passParams; ?>";
		$('#cls-node-<?php print $results->node_id; ?>').find('.total-votes').attr('id','rating-popup-<?php print $results->node_id; ?>');
		$('#cls-node-<?php print $results->node_id; ?>').find('.total-votes').attr('data',data);
		$('#cls-node-<?php print $results->node_id; ?>').find('.total-votes').mouseover(function() {
			$('body').data('learningcore').getRatingDetails(this,'<?php print $results->node_id; ?>','<?php print $results->vote_value;?>');
		});
		</script>
		<?php } ?>
<?php } ?>