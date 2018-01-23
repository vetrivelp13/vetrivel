<?php
expDebug::dPrint('exp_sp_learning-common-register-html.tpl.php: $results = ' . print_r($results, true), 4);

require_once(drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_launch.inc');
global $theme_key;
global $catalog_reg;
expDebug::dPrint('catalog registration level '.$catalog_reg, 5);
$condCount = (($catalog_reg == 'Course' && $results->cls_count == 1 && $results->widgetId == 'lnr-catalog-search') 
							|| ($catalog_reg == 'Course' && $results->widgetId != 'lnr-catalog-search') || $catalog_reg != 'Course') ? 1 : 0;
$userId                       = getSltpersonUserId();

if($catalog_reg == 'Course' && $results->widget == 'catalog-search'){
	$courseID 	   = $results->cls_id;
	$classID 	   = $results->crs_id;
	$classID 	   = str_replace(',', '-', $classID);
	$entityColor   = ($theme_key == "expertusoneV2" && $results->cls_count == 1) ? 'learning-type-crs currency-override-bold' : '' ;
	$pastDateCount = ($results->cls_count == 1) ? checkPastDateOrNot($classID,$results->delivery_type_code) : 0;
}else {
	$courseID 		= $results->crs_id;
	$classID		= $results->cls_id;
	$entityColor 	= ($theme_key == "expertusoneV2") ? 'learning-type-'.array_pop(explode('_',$results->delivery_type_code)).' currency-override-bold' : '' ;
	$pastDateCount  = checkPastDateOrNot($classID,$results->delivery_type_code);
}
$passData     = "data={'NodeId':'".$results->node_id."','ClassId':'".$classID."','CourseId':'".$courseID."'}";
$preRequisite = checkCatalogPrerequisite($courseID,'cre_sys_obt_crs');
if($catalog_reg == 'Course') {
	$getRegister  = getRegisteredOrNotForCourseLevel($courseID,$classID,'',$results->widget,$results->is_compliance,'','lnr-Catalog',NULL);
} else {
	$getRegister  = getRegisteredOrNot($courseID,$classID,'',$results->widget,$results->is_compliance,'','lnr-Catalog',NULL);
}
expDebug::dPrint('get regeiser not value: ' . print_r($getRegister, 1), 4);
if ($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") {
	require_once(drupal_get_path('module', 'exp_sp_administration').'/exp_sp_administration.inc');
	$classSessions = checkClassValidity($classID, $results->delivery_type_code);
	$sessionInProgress = (($classSessions->futureSessions + $classSessions->inProgressSessions) > 0 && ($classSessions->pastSessions + $classSessions->inProgressSessions) > 0 ) ? true : false; // Any one of future or in-progress session is available.
}
//$clsId = ($results->cls_count > 1) ? explode('-',$classID) : $classID;
if($results->cls_count > 1)
	$clsId = explode('-',$classID);
else
	$clsId[] = $classID;
$getEnrollCnt = getEnrolledCount($courseID,$userId,$clsId, $catalog_reg);
	//complianceExpiryandRegisterCount($courseID,'',$userId);
expDebug::dPrint('$catalog_reg : '.$catalog_reg.' $results->widgetId : '.$results->widgetId.' $courseID : '.$courseID.' $classID : '.$classID,4);
$registerEndDateCheck = 0;

$hideButton = 'show';
if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_button']==false){
	$hideButton = 'hide';
} expDebug::dPrint('widget class and Course : '.print_r($_SESSION['widget'], true).'$hideButton : '.$hideButton, 4);

if($condCount == 1){
	$timeZone = date_default_timezone(false);
	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	$defaultDateTimezone = ($results->session_details->session_timezone) ? $results->session_details->session_timezone : $defaultTimezone;
	$sessionTZ = new DateTimeZone($defaultDateTimezone);
	$startdate = new DateTime(null,$sessionTZ) ;
	
	//Changed for #0064685
	//$startdate = $startdate->format('Y-m-d');
	$startdate = format_date(time(), 'custom', 'Y-m-d');
	
	if($results->registration_end_on !='' || $results->registration_end_on != null) {
	    if (strtotime($results->registration_end_on) < strtotime($startdate)) {
	      $registerEndDateCheck = 1;
	    }
	}
}
if($catalog_reg == 'Course') {
	if($results->cls_count == 1 || $results->view_from == 'details') {
		$results->enrolled_id         = $getRegister[0]->enrolled_id;
		$results->comp_status         = $getRegister[0]->comp_status;
		$results->enrolled_status     = $getRegister[0]->enrolled_status;
		$results->waitlist_flag       = $getRegister[0]->waitlist_flag;
		$results->waitlist_priority   = $getRegister[0]->waitlist_priority;
		$results->master_enrollment_id    = $getRegister[0]->master_enrollment_id;
	} else {
		$results->enrolled_id         = '';
		$results->comp_status         = '';
		$results->enrolled_status     = '';
		$results->waitlist_flag       = '';
		$results->waitlist_priority   = '';
		$results->master_enrollment_id    = '';
	}
} else {
	$results->enrolled_id         = $getRegister->enrolled_id;
	$results->comp_status         = $getRegister->comp_status;
	$results->enrolled_status     = $getRegister->enrolled_status;
	$results->waitlist_flag       = $getRegister->waitlist_flag;
	$results->waitlist_priority   = $getRegister->waitlist_priority;
	$results->master_enrollment_id    = $getRegister->master_enrollment_id;
}

$results->is_cart_added       = isCartAdded($results->node_id);
$showContentAttempts		  = false;
if ($results->enrolled_id && ($results->delivery_type_code == 'lrn_cls_dty_wbt' || $results->delivery_type_code == 'lrn_cls_dty_vod' || $results->delivery_type_code =='lrn_cls_dty_wbt,lrn_cls_dty_vod')) {
	$enrolled_class_id		  = getClassEnrollmentId($courseID, $classID, $userId);
	if ($enrolled_class_id > 0 ) {
		$results->enrolled_id	  = $enrolled_class_id;
	} else {
		$results->enrolled_id	  = getMasterEnrollmentStatus($results->master_enrollment_id, $results->enrolled_id);
	}  	
	$results->launch_info 	  = getLaunchDetail($results->enrolled_id);
	$single_content_class 	  = (count($results->launch_info) == 1 ) ? 1 : 0;
	if ($single_content_class) {
		//$showContentAttempts		  = true;
		$IsLaunchable	= $results->launch_info['0']['IsLaunchable'];
		$attemptLeft 	= $results->launch_info['0']['AttemptLeft'];
		$validDays		= $results->launch_info['0']['daysLeft'];
// 		$validDays 		= $results->launch_info['0']['ValidityDays'] - $results->launch_info['0']['remDays'];
		$validDayString = ($validDays == 1) ? t('LBL910') : t('LBL605');
		$crsClassId		= 	$courseID . '_' .  $classID;
	}
}
// expDebug::dPrint("Validity DAYS --- >> ".$validDays);
// expDebug::dPrint("Validity DAYS --- >> ".print_r($results->launch_info,true),4);
//$results->waitlist_seats = getWaitlistCatalogInfo($results->cls_id);
$waitlist   = 0;
$waitlist_register = 0;
$waitlisted = (($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl') ) ? 1 : 0;
if($condCount == 1){
  if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && (($results->available_seats == 0) || ($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl'))) {
    $waitlist_register  = $results->waitlist_seats;
    $waitlist           = 1;
  }
}
expDebug::dPrint('checking the dprint values to be printed in catalog common action 22'.print_r($getRegister,true));
//42055: Provide an option in the admin share widget screen to pass site url
$referer_url = $_SERVER['HTTP_REFERER'];
?>
<?php if($hideButton != 'hide'){?>
<div class="top-record-div">
	<table class="search-register-btn" border="0" cellpadding="0" cellspacing="0">
		<tr>
	 <?php
	 	expDebug::dPrint('results->comp_status----->'.print_r($results->comp_status,true),4);
	 	expDebug::dPrint('results->delivery_type_code----->'.print_r($results,true),4);
	 	$ifexpired = 0;
	if ($results->multi_register == 1) {
		if (!empty(GetIfExpired($courseID,$userId)) && ($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0)){
			$ifexpired = 1;
			$msg = t('Registered');
		}else if(!empty(GetIfExpired($courseID,$userId)) && ($results->enrolled_id == '' || $results->enrolled_id == null || $results->enrolled_id == 0)){
			$ifexpired = 1;
			$msg = t('Register');	
		}
	} else{
		if (!empty(GetIfExpired($courseID,$userId)) && ($results->enrolled_id == '' || $results->enrolled_id == null || $results->enrolled_id == 0)){
			$ifexpired = 1;
			$msg = t('Register');
		}
	 	}
	 		if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 0 && $results->comp_status != 'lrn_crs_cmp_exp' && $ifexpired !=1) {
			$regMsg       = (($results->comp_status == 'lrn_crs_cmp_cmp') ? t('Completed') :
			       ((($results->enrolled_status == 'lrn_crs_reg_wtl') ||
			                 ($results->waitlist_flag == 'lrn_crs_reg_wtl')) ? t('LBL190') : t('Registered')));//Waitlisted
			$waitlist_register  = ($regMsg !== t('LBL190')) ? 0 : $waitlist_register; //Waitlisted
			$seatDisplay        = ($regMsg == t('LBL190')) ? true : false;  //Waitlisted
		  ?>
		  <?php
		  $course_com_cnt = getCourseCompletedCount($courseID,$userId);
		  if($catalog_reg == 'Course'  && $results->cls_count > 1  ){
		  	
		  	$courseCnt = ($results->comp_status == 'lrn_crs_cmp_cmp') ? 'Y' : '';
		   ?>
		   <td
		   <?php if(($catalog_reg == 'Course' && $results->cls_count <= 1) || ($catalog_reg != 'Course' && $getEnrollCnt != $results->cls_count)) {?>
		   id="registerCls_<?php print $classID; ?>" class="<?php print ($getEnrollCnt == $results->cls_count) ? 'action-btn-disable' : 'action-btn'; ?>"  onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $courseID; ?>',
		   '<?php print $classID; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>','fromCourselvl','<?php print $courseCnt;?>');"
		   <?php echo '>'.t($regMsg);}
			else if(($results->delivery_type_code == 'lrn_cls_dty_wbt' || $results->delivery_type_code == 'lrn_cls_dty_vod' || $results->delivery_type_code =='lrn_cls_dty_wbt,lrn_cls_dty_vod') && ( $results->comp_status =='lrn_crs_cmp_enr' || $results->comp_status =='lrn_crs_cmp_inp' ||($course_com_cnt < $getEnrollCnt))){
			?>
			id="registerCls_<?php print $classID; ?>" class="action-btn" onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').showSelectClass('<?php print $userId; ?>','<?php print $courseID; ?>');">
			<?php  print t('LBL199');
			}else{ echo 'class="action-btn-disable" >';
				print t($regMsg);
			}
			?>
		</td>

		<?php } else { ?>
		<td
			<?php
					expDebug::dPrint('$results->comp_status---->'.print_r($results->comp_status,true),4);
					if(($results->delivery_type_code == 'lrn_cls_dty_wbt' || $results->delivery_type_code == 'lrn_cls_dty_vod') && ( $results->comp_status =='lrn_crs_cmp_enr' || $results->comp_status =='lrn_crs_cmp_inp')){
						$res = registeredToLaunch($userId,$classID,$courseID,$results->enrolled_id,$results->delivery_type_code,'',$regMsg);
						expDebug::dPrint('OVERE ALL RESULTs---->'.print_r($res,true),4);
						print $res[0];
						$showContentAttempts = $res[1];
					}
					else{
						?>
						id="registerCls_<?php print $classID; ?>"  class="action-btn-disable" >
						<?php
						print t($regMsg);
					}
			?>
		</td>
		<?php } ?>
	<?php } else if($ifexpired ==1) { ?>
		<td class="action-btn-disable" id="registerCls_<?php  print $classID; ?>">
			<?php print $msg;?>
		</td>
	<?php } else if((($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && $results->available_seats == 0 && $results->waitlist_seats == 0) && ($condCount == 1)) { ?>
		<td class="action-btn-disable" id="registerCls_<?php  print $classID; ?>">
			<?php print t('LBL046'); //Full ?>
		</td>
	<?php } else if((($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && $sessionInProgress) && ($condCount == 1)) { ?>
		<td class="action-btn-disable" id="registerCls_<?php print $classID; ?>">
			<?php print t('In progress'); //In progress ?>
		</td>			
	<?php } else if((($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && $pastDateCount > 0) && ($condCount == 1)) { ?>
		<td class="action-btn-disable" id="registerCls_<?php print $classID; ?>">
			<?php print t('LBL105'); //Delivered ?>
		</td>
	<?php }	else if(($registerEndDateCheck <> 0) && ($condCount == 1)) { ?>
		<td class="action-btn-disable" id="registerCls_<?php print $classID; ?>">
			<?php print t('LBL047'); //Closed ?>
		</td>
	<?php } else if($catalog_reg == 'Course' && $results->cls_count == 0 && $results->widgetId == 'lnr-catalog-search') { ?>
		<td class="action-btn-disable" id="registerCls_<?php print $classID; ?>">
			<?php print t('LBL636'); //No Classes ?>
		</td>
	<?php } else  {
		 		$seatDisplay = true;
				if(count($preRequisite)>0) { ?>
						<td class="action-btn" id="prereqCourse<?php print $courseID; ?>"
						  <?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
						  	onclick="passUrlCommonCall(this.id);">
						  <?php }else{ ?>
								<?php if($userId!="" && $userId>0){ ?>
								onclick="$('body').data('prerequisite').getTpPrerequisites(<?php print $courseID; ?>,'<?php  //print $results->widgetId;?>','course','l1');"
								<?php }else{ ?>
								onclick="$('body').data('learningcore').callSigninWidget(this.id);"
								<?php }?> >
							<?php }?>
							<div><?php print t('LBL230'); //Prerequisite ?></div>
							<?php // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //?>
							<?php  if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && $results->price > 0)  { ?>
							<div class="<?php print $entityColor; ?>"><?php if($condCount == 1) print $results->currency_type ." ".$results->price;?></div>
							<?php } ?>
							</td>
				<?php } else if($condCount == 1 && !empty($_SESSION['availableFunctionalities']->exp_sp_commerce))  {
				          //if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)){

							 if($results->is_cart_added){  ?>
								<td class="action-btn-disable" id="addToCartList_<?php print $classID; ?>">
									<?php print t('LBL049'); //Added to Cart ?>
								</td>
							<?php } else if(($results->price != '0.00') || ($results->price == '0.00' && $results->export_compliance == 'Y')){  ?>

										<td data="<?php print $passData;?>" class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
										id="addToCartList_<?php print $classID; ?>"
										<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
									  	onclick="passUrlCommonCall(this.id);">
									  <?php }else{ ?>
											<?php if($userId!="" && $userId>0){ ?>
											onclick="$('body').data('learningcore').callAddToCart('<?php print $results->widgetId; ?>','1','<?php print $classID; ?>','','',<?php print $waitlist;  ?>);"
											<?php }else{ ?>
											onclick="$('body').data('learningcore').callSigninWidget(this.id);"
											<?php }?>>
										<?php }?>

											<div><?php print ($waitlist_register > 0) ? t("Waitlist") : t('LBL050'); //Add to cart ?></div>
											<?php // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //?>
										  <?php  print (!empty($_SESSION['availableFunctionalities']->exp_sp_commerce)  && $results->price > 0) ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>


										</td>

							<?php } else if(($results->price == '0.00' && $results->export_compliance == 'N')  || ($waitlist_register > 0)){ ?>
										<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
											id="registerCls_<?php  print $classID;?>"
											  <?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
											  onclick="passUrlCommonCall(this.id);">
											  <?php }else{ ?>
													<?php if($userId!="" && $userId>0){ ?>

													onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $courseID; ?>','<?php print $classID; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>');"
													<?php }else{ ?>
													onclick="$('body').data('learningcore').callSigninWidget(this.id);"
													<?php }?>>
												<?php }?>

											<div><?php print ($waitlist_register > 0) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf' && $results->comp_status != 'lrn_crs_cmp_exp') ? t('Registered') : t('Register'));  ?></div>
											<?php print ''; // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //print (!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && $results->enrolled_status != 'lrn_crs_reg_cnf') ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>


										</td>
						     <?php	} ?>
						<?php //}
				}else {
				  if($catalog_reg == 'Course' && $results->cls_count > 1 && $results->widgetId == 'lnr-catalog-search'){ ?>
				  <?php if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && ($results->is_cart_added)) {?>

				      <td class="action-btn-disable" id="addToCartList_<?php print $classID; ?>">
  									<?php print t('LBL049'); //Added to Cart ?>
  					  </td>

                  <?php } else if (($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 1 )  { ?>
                                      <td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
                                      id="registerCls_<?php print $courseID; ?>"
                                      			<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
																					  	onclick="passUrlCommonCall(this.id);">
																					  <?php }else{ ?>
                                      					  <?php if($userId!="" && $userId>0){ ?>
                                      					  onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $courseID; ?>','<?php print $results->registered_class; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>','<?php print 'fromCourselvl'; ?>');"
                                      					  <?php }else{ ?>
                                      					  onclick="$('body').data('learningcore').callSigninWidget(this.id);"
                                      					  <?php }?>>
                                      			<?php } ?>
                                      					  <?php  print ($waitlist_register > 0 ) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf' && $results->comp_status != 'lrn_crs_cmp_exp') ? t('Registered') : t('Register')); ?>
                                      					  </td>
                                    <?php }
                   else { ?>
         <td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
					  id="registerCls_<?php print ($catalog_reg == 'Course' && $results->widgetId == 'lnr-catalog-search') ? $classID : $courseID; ?>"
					  <?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
						  	onclick="passUrlCommonCall(this.id);">
						<?php }else{ ?>
								  <?php if($userId!="" && $userId>0){
								  	if(($catalog_reg == 'Course' && $results->cls_count <= 1) || $results->view_from == 'details' || ($catalog_reg != 'Course' && $getEnrollCnt)){ ?>
								  	onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $courseID; ?>','<?php print $classID; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>','<?php print 'fromCourselvl'; ?>');"
								  	<?php }else{ ?>
								  onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').showSelectClass('<?php print $userId; ?>','<?php print $courseID; ?>');"
								  <?php }
			                   }else{ ?>
								  onclick="$('body').data('learningcore').callSigninWidget(this.id);"
								  <?php }?>>
						<?php }?>
					  <?php  print ($waitlist_register > 0 ) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf' && $results->comp_status != 'lrn_crs_cmp_exp') ? t('Registered') : t('Register')); ?>
					  </td>
					  <?php }
				  }  else { ?>
						<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : (($results->multi_register == 0 && $getEnrollCnt == $results->cls_count && isset($results->cls_count)) ? 'action-btn-disable' : 'action-btn'); ?>"
						id="registerCls_<?php /* if($catalog_reg == 'Course' && $results->widgetId == 'lnr-catalog-search') print $courseID; else */ print $classID; ?>"
						<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
					  		onclick="passUrlCommonCall(this.id);">
					  	<?php }else{ ?>
								<?php if($userId!="" && $userId>0){
										if($getEnrollCnt != $results->cls_count || $results->view_from == 'details' || $results->multi_register == 1) {?>
											onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $courseID; ?>','<?php print $classID; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>');"
										<?php }
									}else{ ?>
										onclick="$('body').data('learningcore').callSigninWidget(this.id);"
								<?php }?>>
						<?php }?>
						<?php  print ($waitlist_register > 0 ) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf' && $results->comp_status != 'lrn_crs_cmp_exp') ? t('Registered') : t('Register')); ?>
						</td>
				<?php }
				 } ?>
			<?php }?>
	</tr>
</table>
</div>
<?php expDebug::dPrint("Valid Days -- > ".$validDays, 5);?>
		<?php if($showContentAttempts && $results->comp_status !='lrn_crs_cmp_cmp' &&  !contentPlayerIsActive()){ // && $results->cls_count == 1?>
			<div class="course-attempt-left"><span id="attempt_left_<?php print $crsClassId; ?>" class="attempts_left"> <?php if ($attemptLeft != 'notset' && !empty($attemptLeft)) {  print $attemptLeft . ' ' . t('LBL202'); } ?></span></div>
			<div class="course-content-validity">
				<span id="content_concat_str_<?php print $crsClassId; ?>" class="content_concat_string"><?php if ($attemptLeft != 'notset' && !empty($validDays)) { print t('LBL647'); }?></span>
				<span id="content_validity_<?php print $crsClassId; ?>" class="content_validity"><?php if ($validDays > 0) { print $validDays . ' ' .$validDayString . ' ' .t('LBL604'); }?> </span></div>
			<?php if($results->delivery_type_code == 'lrn_cls_dty_vod') {
				// expDebug::dPrint('launch info from tpl'.print_r($results->launch_info, 1), 5);
				$progress = '';
				$progress_percent = 0;
				foreach($results->launch_info as $launch){
					
					$suspend_data = isset($launch['SuspendData']) ? json_decode(rawurldecode($launch['SuspendData'])) : null;
					$progress = 0;
					// expDebug::dPrint('launch info SuspendData'.print_r($suspend_data, 1), 5);
					if($suspend_data != null) {
						$progress = $suspend_data->progress;
						// expDebug::dPrint('progress of each vod'.print_r($progress, 1), 5);
					}
					$progress_percent += floatval($progress);
				}
				$progress_percent = $progress_percent / count($results->launch_info);
				$progress_percent_string = round($progress_percent) . '% ' . t('Completed');
				//to show progress percentage for single content vod classes
				// expDebug::dPrint('launch info from tpl'.print_r($progress_percent, 1), 5);
			?>
				<div class="course-content-validity">
					<span class="content_concat_string">
						<?php if (($attemptLeft != 'notset' || !empty($validDays)) && count($results->launch_info) == 1) {
							print t('LBL647');
						}?>
					</span>
					<?php if (count($results->launch_info) == 1) { ?>
					<span id="content_progress_<?php print $crsClassId; ?>" class="content_validity">
						<?php print $progress_percent_string; ?>
					</span> <?php } ?>
				</div>
			<?php } ?>
		<?php } ?>
<?php if(($condCount == 1) && $seatDisplay == true ) { ?>
	<?php if($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl"){
	    //if($waitlist == 1) {
	    if($waitlisted == 1) {
	        $seatInfo = t('LBL125').' '.getWaitlistPosition($classID,$courseID,$userId);
	    } else if($waitlist_register > 0) {
	      //$seatInfo = $waitlist_register." ".t('Waitlist Seats left');
	      $seatInfo = (($waitlist_register == 1) ? ($waitlist_register." ".t('LBL126')) : //Waitlist Seat left
	                                               ($waitlist_register." ".t('LBL127'))); //Waitlist Seats left
	    } else {
	      $seatInfo = $results->available_seats;
	      $seatInfo = (($results->available_seats == 1) ? ($results->available_seats." ".t('LBL052')) : //Seat left
	                                                      ($results->available_seats." ".t('LBL053'))); //Seats left
	    }
	  ?>
			<div class="course-available-seats">
				<span class="catlog_seats_available" id="seats_available_<?php print $classID; ?>">
				<?php
				   print $seatInfo;
			    ?>
			    </span>
			    <?php
				/*if($results->available_seats == 1){ ?>
						<span id="seats_available_<?php print $results->cls_id; ?>">
							<?php print (($waitlist_register>0) ? ($waitlist_register." Waitlist") : $results->available_seats); ?>&nbsp;<?php print t('Seat left'); ?>
						</span>
				<?php } else { ?>
						<span id="seats_available_<?php print $results->cls_id; ?>">
							<?php print (($waitlist_register>0) ? ($waitlist_register." Waitlist") : $results->available_seats); ?>&nbsp;<?php print t('Seats left'); ?>
						</span>
				<?php }*/ ?>
			</div>
	<?php } ?>
<?php } } ?>