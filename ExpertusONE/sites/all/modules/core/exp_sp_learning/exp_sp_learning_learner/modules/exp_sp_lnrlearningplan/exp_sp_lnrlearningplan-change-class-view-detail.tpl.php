<div class="top-record-div-left">
	<div>
    	<span>
    	<?php $curTitle = $results->cls_title;	
   /*  	print '<span class="spotlight-item-title vtip" title="'.sanitize_data($results->cls_title).'">'.$curTitle.'</span>'; */
    	$mro_div = '';
    	if($results->is_compliance){ 
    	  $mro_div  = '<div class="catalog-course-compliance-role-bg">
                      <span class="catalog-course-compliance-bg-left"></span>
                      <span class="catalog-course-compliance-bg-middle">'.t('Compliance').'</span>
                      <span class="catalog-course-compliance-bg-right"></span>
                      </div></div>';
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
                      </div></div>';
    	}
    	else if($results->mro_id=='cre_sys_inv_man'){
    	  $mro_div  = '<div class="catalog-course-role-access-bg">
                      <span class="catalog-course-mandatory-bg-left"></span>
                      <span class="catalog-course-mandatory-bg-middle">'.t('Mandatory').'</span>
                      <span class="catalog-course-mandatory-bg-right"></span>
                      </div></div>';
    	}
    	if(empty($mro_div)) {
      		print '<div class="mylearn-select-class-container limit-title-row"><span class="limit-title limit-title-cls spotlight-item-title without-mro-container vtip" title="'.sanitize_data($results->cls_title).'">'.$curTitle.'</span>';
    	}else{
    		print '<div class="mylearn-select-class-container limit-title-row"><span class="limit-title limit-title-cls spotlight-item-title vtip" title="'.sanitize_data($results->cls_title).'">'.$curTitle.'</span>';
    	}
    	print $mro_div;
    	print '<div class="clearBoth"></div>';
    	?>
    	</span>
	</div>
	<div class="find-training-list-course">	 
		<div class="line-item-container float-left"><span id='srch-code-title' class="vtip" title="<?php print t('LBL096').": ". sanitize_data($results->cls_code); ?>"><?php print titleController('LNRLEARNING-PLAN-CHANGE-CLASS-VIEW-DETAIL-CLASS-CODE',$results->cls_code,10); ?></span></div>
		<div class="line-item-container float-left"><span class="pipe-line">|</span>
		<span class="vtip" title="<?php print t('LBL038').": ".sanitize_data(t($results->language)); ?>"><?php print t(subStringController(t($results->language),3)); ?></span></div>
	<!-- </div>	
	<div class="find-training-list">  -->
    <?php if($results->location != null) { ?>
    <div class="line-item-container float-left">
			<span class="pipe-line">|</span>
			<span id="srch-location-title" class="vtip" title="<?php print sanitize_data($results->location); ?>"><?php print titleController('CHANGE-CLASS-LOCATION',$results->location,15); ?></span></div>
    <?php } ?>
    <?php 
        /*$sessStartDate = date_format(date_create($results->sess_start_date),'M d, Y');
        $sessStartTime = date("g:i", strtotime($results->sess_start_time));
        $sessEndTime = date("g:i", strtotime($results->sess_end_time));*/
    if(count($results->session_details) > 0) {
    	/*For ticket 0028936: ILT Timezone Display for My Learning page */
		$sDay =  $results->session_details[0]['session_start_date_format'];
		$sTime = $results->session_details[0]['session_start_time_format'];
		$sTimeForm = $results->session_details[0]['session_start_time_form'];
		$usDay =$results->session_details[0]['ilt_session_start_date_format'];
		$usTime = $results->session_details[0]['ilt_session_start_time_format'];
		$usTimeForm =$results->session_details[0]['ilt_session_start_time_form'];
		$user_tz = $results->session_details[0]['user_tz'];
		$user_tzcode = $results->session_details[0]['user_tzcode'];
		$loc_tz= $results->session_details[0]['session_code'];
		$loc_tzcode = $results->session_details[0]['tz_code'];
		$det = '';
		if($results->delivery_type_code == "lrn_cls_dty_ilt" && $user_tz!=$loc_tz){
		$det = $user_tz.' '.$user_tzcode;
		expDebug::dPrint('printing time detai ' . print_r($user_tz, true), 5);
		expDebug::dPrint('printing time detai22 ' . print_r($det, true), 5);
		}
		if(count($results->session_details) > 1) {
				$sessLenEnd = count($results->session_details)-1;
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				$eDay =$results->session_details[$sessLenEnd]['session_start_date_format'];
				$eTime = $results->session_details[$sessLenEnd]['session_start_end_format'];
				$eTimeForm = $results->session_details[$sessLenEnd]['session_end_time_form'];
				$ueDay = $results->session_details[$sessLenEnd]['ilt_session_start_date_format'];
				$ueTime =$results->session_details[$sessLenEnd]['ilt_session_start_end_format'];
				$ueTimeForm =$results->session_details[$sessLenEnd]['ilt_session_end_time_form'];
				$sessionDate = $sDay .' '. $sTime .' '. '<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eDay. ' ' .$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>'.$det.'';
				expDebug::dPrint('printing time detai22 ' . print_r($sessionDate, true), 5);
				$locsessionDate = $usDay .' '. $usTime .' '. '<span class="time-zone-text">'.$usTimeForm.'</span>'.' to '.$ueDay. ' ' .$ueTime.' <span class="time-zone-text">'.$ueTimeForm.'</span>'.$loc_tz.' '.$loc_tzcode.'';
				
		}else {					
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				$eTime = $results->session_details[0]['session_start_end_format'];
				$eTimeForm = $results->session_details[0]['session_end_time_form'];
				$ueTime = $results->session_details[0]['ilt_session_start_end_format'];
				$ueTimeForm = $results->session_details[0]['ilt_session_end_time_form'];
				$sessionDate = $sDay .' '. $sTime.' '.'<span class="time-zone-text">'.$sTimeForm.'</span>'.' to '.$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>'.$det.'';
				$locsessionDate = $usDay .' '. $usTime.' '.'<span class="time-zone-text">'.$usTimeForm.'</span>'.' to '.$ueTime.' <span class="time-zone-text">'.$ueTimeForm.'</span>'.$loc_tz.' '.$loc_tzcode.'';
				
		}
					
	}    
    
        if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") ) { ?>	
			<div class="line-item-container float-left">
			<span class="pipe-line">|</span>
			
			<span class="vtip" title="<?php print strip_tags($sessionDate)?>"><?php print titleController('CHANGE-CLASS-SESSIONDATETIME',$sessionDate,100);?></span>
			<?php
			if($user_tz!=$loc_tz && $results->delivery_type_code == "lrn_cls_dty_ilt" ){?>
	<?php 
	$sessionqtipId = 'changeclass-'.$results->session_details[0]['session_id'];
	$multisession = '';
	if(count($results->session_details) > 1)
		$multisession= 1;
	$qtip=qtip_popup_paint($sessionqtipId,$locsessionDate,$multisession,1);
	
	 echo $qtip; } ?>
			</div>
	<?php } ?>
	</div>
	<div class="record-div find-training-txt">
	 	<div class="refer-course-link">
 		  <?php // print l(t('Refer'), '', array('attributes' => array('onclick' => "$('body').data('refercourse').getReferDetails('$results->cls_id','cre_sys_obt_cls','lnr-catalog-search'); return false;"))); ?>
		</div> 
		<div class="record-div find-training-txt">
		  <?php //print_r($results);//print descController("CATALOG SEARCH",$results->cls_short_description); ?>
		  <?php $description = str_replace(array("\n","\r"), array("<br>","&nbsp;"), $results->cls_short_description); ?>
		 <div class="limit-desc-row"><span class="limit-desc limit-desc-cls vtip"><span class="cls-learner-descriptions"> <?php print $description;?></span></span></div>
		</div>
	</div>
</div>