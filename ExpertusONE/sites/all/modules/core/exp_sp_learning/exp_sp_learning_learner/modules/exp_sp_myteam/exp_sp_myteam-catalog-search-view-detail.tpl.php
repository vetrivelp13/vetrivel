<?php
expDebug::dPrint("my team".print_r($results,true),5);
  	if($results->object_type =='Class') {
  	  $results->mandatory_value = myteamFetchEnrollment($results->crs_id,$results->cls_id,$userId);
  	  $exmpId = getRegisteredEnrollmentExmeptedorNot('class',$userId,$results->crs_id,$results->cls_id);
  	}else{
  	  $results->mandatory_value = myteamFetchProgram($results->cls_id,$userId);
  	  $exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$userId,$results->cls_id);
  	}
  	$user_Id = getSltpersonUserId();
  	$timezone_details = getPersonDetails($user_Id);
  	expDebug::dPrint("my team userid".print_r($user_Id,true),5);
  	?>
<div class="top-record-div-left">
	<div>
    	<div class="list-title">
    		<div class="select-class-container limit-title-row">
    	<?php
    		global $language;
    		//Applying fadeout affect so removed the condition
    		/*if($language->language == 'ru' || $language->language =='de') {
    			$curTitle = titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-TITLE',$results->cls_title,40);
    		}
    		else {
    			$curTitle = titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-TITLE',$results->cls_title,65);
    		} */
    		/*	$recom_course = 0;
    			$man_course = 0;
    			$mrovalue= explode(',',$results->mro_id);
    			if (in_array('cre_sys_inv_rec', $mrovalue)) {
    				$recom_course = 1;
    			}
    			if (in_array('cre_sys_inv_man', $mrovalue)) {
    				$man_course = 1;
    			}
    			if($results->is_compliance) {
    				$curTitle = titleController('MYTEAM-COMPLIANCE-COURSE-TITLE',sanitize_data($results->cls_title),65);
    			}
    			else if($man_course == 1) {
    				$curTitle = titleController('MYTEAM-MANDATORY-COURSE-TITLE',sanitize_data($results->cls_title),72);
    			}
    			else if($recom_course == 1) {
    				$curTitle = titleController('MYTEAM-RECOMMENDED-COURSE-TITLE',sanitize_data($results->cls_title),72);
    			}
    			else {
    				$curTitle = titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-TITLE',sanitize_data($results->cls_title),65);
    			}*/
				
		$curTitle = sanitize_data($results->cls_title);
    		
    	$mro_div = '';
    	if($results->is_compliance){
    		if(!empty($exmpId)) {
    			$mro_div  ='';
    		}else {
    	  $mro_div  = '<div class="catalog-course-compliance-role-bg">
    	  <span class="catalog-course-compliance-bg-left"></span>
    	  <span class="catalog-course-compliance-bg-middle">'.t('Compliance').'</span>
    	  <span class="catalog-course-compliance-bg-right"></span>
    	  </div>';
    		}
    	}
    	/*else if($results->mro_id=='cre_sys_inv_opt'){
    	  $mro_div  = '<div class="catalog-course-role-access-bg">
                      <span class="catalog-course-optional-bg-left"></span>
                      <span class="catalog-course-optional-bg-middle">'.strtolower(t('Optional')).'</span>
                      <span class="catalog-course-optional-bg-right"></span>
                      </div>';
    	}*/
    	else if($results->mro_id=='cre_sys_inv_rec'){
    	  $mro_div  = '<div class="catalog-course-role-access-bg">
                      <span class="catalog-course-recommended-bg-left"></span>
                      <span class="catalog-course-recommended-bg-middle">'.t('Recommended').'</span>
                      <span class="catalog-course-recommended-bg-right"></span>
                      </div>';
    	}
    	else if($results->mro_id=='cre_sys_inv_man' || $results->mandatory_value=="Y" || $results->mandatory_value== 1){
    		if(!empty($exmpId)) {
    			$mro_div  ='';
    		}else {
    	  $mro_div  = '<div class="catalog-course-role-access-bg">
    	  <span class="catalog-course-mandatory-bg-left"></span>
    	  <span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span>
    	  <span class="catalog-course-mandatory-bg-right"></span>
    	  </div>';
    		}
    	}
    	if(empty($mro_div)) {
    		print '<span class="limit-title spotlight-item-title vtip without-mro-container" title="'.sanitize_data($results->cls_title).'">'.$curTitle.'</span></div>';
    	}else{
    		print '<span class="limit-title spotlight-item-title vtip" title="'.sanitize_data($results->cls_title).'">'.$curTitle.'</span></div>';
    	}
    	print $mro_div;
    	print '<div class="clearBoth"></div>';
    	?>
	</div>
	<div class="find-training-list-course sub-attributes">	 
		<div class="line2-attribute">
			<span id='srch-code-title' class="vtip" title="<?php print t('LBL096').": ". sanitize_data($results->cls_code); ?>"><?php print titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-CODE',$results->cls_code,3); ?></span>
		</div>
		<div class="line2-attribute">
			<span class="pipe-line">|</span>
			<span class="vtip" title="<?php print t('LBL038').": ".sanitize_data(t($results->language)); ?>"><?php print titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-LANGUAGE',$results->language,3); ?></span>
		</div>
	<!-- </div>	
	<div class="find-training-list">  -->
    <?php if($results->location != null) { ?>
    <div class="line2-attribute">
			<span class="pipe-line">|</span>
			<span id="srch-location-title" class="vtip" title="<?php print sanitize_data($results->location); ?>"><?php print titleController('EXP-SP-MYTEAM-CATALOG-SEARCH-CLASS-LOCATION',$results->location,5); ?></span>
		</div>
    <?php } ?>
    <?php 
        /*$sessStartDate = date_format(date_create($results->sess_start_date),'M d, Y');
        $sessStartTime = date("g:i", strtotime($results->sess_start_time));
        $sessEndTime = date("g:i", strtotime($results->sess_end_time));*/
    $timezone_details = getPersonDetails($userId);
      if(count($results->session_details) > 0) {
      	/*For ticket 0028936: ILT Timezone Display for My Learning page */
      	expDebug::dPrint("my team sess det".print_r($zone,true),5);
      	$zone = $results->session_details[0]['session_code'];
      	$attr1 = $results->session_details[0]['attr1'];
      	$attr3 = $results->session_details[0]['tz_code'];
      	expDebug::dPrint("my team sess det".print_r($zone,true),5);
      	$sDay = $results->session_details[0]['session_start_date_format'];
		$sTime = $results->session_details[0]['session_start_time_format'];
		$sTimeForm = $results->session_details[0]['session_start_time_form'];
		$iltsDay = $results->session_details[0]['ilt_session_start_date_format'];
		$iltsTime = $results->session_details[0]['ilt_session_start_time_format'];
		$iltsTimeForm = $results->session_details[0]['ilt_session_start_time_form'];
		if(count($results->session_details) > 1) {
				$sessLenEnd = count($results->session_details)-1;
				expDebug::dPrint("my team sess det".print_r($results->session_details,true),5);
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				$eDay = $results->session_details[$sessLenEnd]['session_start_date_format'];
				$eTime = $results->session_details[$sessLenEnd]['session_start_end_format'];
				$eTimeForm =$results->session_details[$sessLenEnd]['session_end_time_form'];
				$ilteDay = $results->session_details[$sessLenEnd]['ilt_session_start_date_format'];
				$ilteTime = $results->session_details[$sessLenEnd]['ilt_session_start_end_format'];
				$ilteTimeForm = $results->session_details[$sessLenEnd]['ilt_session_end_time_form'];
				$sessionDate = $sDay .' '. $sTime .' '. '<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eDay. ' ' .$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
				if($results->delivery_type_code == "lrn_cls_dty_ilt")
				$sessionDate = $sDay .' '. $sTime .' '. '<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eDay. ' ' .$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>'.' '.$timezone_details['attr1'].' '.$timezone_details['attr4'];
				$iltsessionDate = $iltsDay .' '. $iltsTime .' '. '<span class="time-zone-text">'.$iltsTimeForm.'</span>'.' to '.$ilteDay. ' ' .$ilteTime.' <span class="time-zone-text">'.$ilteTimeForm.'</span>'.' '.$zone.' '.$attr3;
				
		}else {					
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				$eTime =$results->session_details[0]['session_start_end_format'];
				$eTimeForm =$results->session_details[0]['session_end_time_form'];
				$ilteTime = $results->session_details[0]['ilt_session_start_end_format'];
				$ilteTimeForm = $results->session_details[0]['ilt_session_end_time_form'];
				$sessionDate = $sDay .' '. $sTime.' '.'<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
				if($results->delivery_type_code == "lrn_cls_dty_ilt")
				$sessionDate = $sDay .' '. $sTime.' '.'<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>'.' '.$timezone_details['attr1'].' '.$timezone_details['attr4'];
				$iltsessionDate = $iltsDay .' '. $iltsTime.' '.'<span class="time-zone-text">'.$iltsTimeForm.'</span>'.' to '.$ilteTime.' <span class="time-zone-text">'.$ilteTimeForm.'</span>'.' '.$zone.' '.$attr3;
				
		}
					
	}    
    
        if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") ) { ?>	
        <div class="line2-attribute">
					<span class="pipe-line">|</span>
					<span class="vtip <?php print $results->delivery_type_code;?>"  style = "display: inline-block;" title='<?php print strip_tags($sessionDate); ?>'><?php print titleController('LNRSEARCH-DATE-TIME',$sessionDate,5);?></span> 

			<?php if($results->session_details[0]['sess_timezone']!=$timezone_details['attr2'] && $results->delivery_type_code == "lrn_cls_dty_ilt" ){
				$multisession = '';
				
				if(count($results->session_details) > 1) {
					
					$multisession = 2;
				}
	        $qtip=qtip_popup_paint('assign-learning-'.$results->cls_id,$iltsessionDate,$multisession);
	
	       echo $qtip;  } ?>	
	       </div>
	<?php } ?>
	</div>
	<div class="record-div find-training-txt">
	 	<div class="refer-course-link">
 		  <?php // print l(t('Refer'), '', array('attributes' => array('onclick' => "$('body').data('refercourse').getReferDetails('$results->cls_id','cre_sys_obt_cls','lnr-catalog-search'); return false;"))); ?>
		</div> 
		 <?php $description = str_replace(array("\n","\r"), array("<br>","&nbsp;"), $results->cls_short_description); 
		  		$deliveryType = ($results->delivery_type_code) ? $results->delivery_type_code : $results->object_type;?>
		  <div class="limit-desc-row <?php echo $deliveryType; ?>" >
		<div class="record-div find-training-txt">
		  <?php //print_r($results);//print descController("CATALOG SEARCH",$results->cls_short_description); ?>
		  	<span class="limit-desc vtip"><span class="cls-learner-descriptions"><?php print $description; ?></span></span>
		</div>
	</div>
</div>
