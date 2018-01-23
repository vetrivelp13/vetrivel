<?php
expDebug::dPrint('exp_sp_learniasdsang-common-lp-register-html.tpl.php: $results = ' . print_r($results, true), 5);
global $theme_key;
global $catalog_reg;
$delivettype = getObjectdeliveryType($results->cls_id,$userId);
$getcount = getCourseClassCountForLP($results->cls_id,$userId);
if ((in_array("lrn_cls_dty_ilt", $delivettype)) || (in_array("lrn_cls_dty_vcl", $delivettype)))
{	$match = true;}else{	$match = false;}
if($match == true){$results->cls_count = count($getcount);}else{$results->cls_count = 0;}

$entityColor = ($theme_key == "expertusoneV2") ? 'learning-type-'.array_pop(explode('_',$results->object_type)).' currency-override-bold' : '' ;
if($results->object_type == 'Course' || $results->object_type == 'Class'){
		$condCount = (($catalog_reg == 'Course' && $results->cls_count == 1 && $results->widgetId == 'lnr-catalog-search') || ($catalog_reg == 'Course' && $results->widgetId != 'lnr-catalog-search')) ? 1 : 0;
}else{
	$condCount = ((($catalog_reg == 'Course' || $catalog_reg == 'Class') && $results->cls_count == 1 && $results->widgetId == 'lnr-catalog-search' || $results->widgetId =='learning-plan-details-data') || ($catalog_reg == 'Course' && $results->widgetId != 'lnr-catalog-search')) ? 1 : 0;
}
$userId = getSltpersonUserId();
$module_seq = getModuleId($userId,$results->cls_id);

if(count($module_seq)>0) {
$module_path = $module_seq[0]->recertify_path;
}

$resultsMasterEnroll = getObjectRegisteredOrNot($results->cls_id,'',$module_path);
$allow_recertify = 0; 
if($resultsMasterEnroll->object_type == 'cre_sys_obt_crt' && $resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_cmp') 
	$allow_recertify = getRecertifyBeforeCertifyExpired($resultsMasterEnroll->comp_date, $resultsMasterEnroll->expires_in_value ,$resultsMasterEnroll->expires_in_unit);

$results->is_cart_added = isCartAdded($results->node_id);
$passData = "data={'NodeId' :'".$results->node_id."','TpId':'".$results->cls_id."'}";
$preRequisite = checkCatalogPrerequisite($results->cls_id,'cre_sys_obt_trp');
if($match == true){
	$results->prm_available_seats = getObjectAvailableSeats($results->cls_id,$userId);
	
	$results->waitlist_status=-1;
	if($results->prm_available_seats<=0)
		$results->waitlist_status = getProgramWaitlistStatus($results->cls_id,$userId);
}
$hideButton = 'show';
if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_button']==false){
	$hideButton = 'hide';
}
expDebug::dPrint('widget Certificate: '.print_r($_SESSION['widget'], true).'$hideButton : '.$hideButton, 4);
if($condCount == 1){
	if(($results->waitlist_status > 0) && ($resultsMasterEnroll->master_enrolled_id == '' || $resultsMasterEnroll->master_enrolled_id == null || $resultsMasterEnroll->master_enrolled_id == 0)) {
	  $waitlist_register  = $results->waitlist_status;
	  $waitlist           = 1;
	} else {
	  $waitlist_register = 0;
	  $waitlist          = 0;
	}
}
if($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_wtl') {
	$waitlist_st = 1;
} else {
	$waitlist_st = 0;
}




	 $startdate = format_date(time(), 'custom', 'Y-m-d');
	 if($condCount == 1){
	if($results->registration_end_on !='' || $results->registration_end_on != null) {
	    if (strtotime($results->registration_end_on) < strtotime($startdate)) {
	       $registerEndDateCheck = 1;
	    }
	}
	 }
	$referer_url = $_SERVER['HTTP_REFERER'];
?>
<?php if($hideButton != 'hide'){?>
<div class="top-record-div">
<table class="search-register-btn" border="0" cellpadding="0" cellspacing="0">
	<tr>

	<?php if($resultsMasterEnroll->master_enrolled_id != '' || $resultsMasterEnroll->master_enrolled_id != null || $resultsMasterEnroll->master_enrolled_id != 0) { ?>

			<?php if($results->object_type == 'cre_sys_obt_crt' && (($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_exp') || $allow_recertify == 1)) { ?>
				<td class="action-btn" id="object-registerCls_<?php print $results->cls_id; ?>" onclick="$('body').data('learningcore').callPopupOrNot('enroll-lp-result-container','<?php print $results->cls_id; ?>',null,'N',null,'<?php print $resultsMasterEnroll->recertify_path.'-R'; ?>');">
					<?php print t('LBL429'); ?>
				</td>
			<?php } else {
					$seatDisplay = true;
				?>
				<?php
				$tpcompleted  = t('Completed');
				$tpwaitlist   = t('LBL190');
				$tpregistered = t('Registered');

				$regMsg = ($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_cmp') ? $tpcompleted : (($waitlist_st > 0) ? $tpwaitlist :$tpregistered); ?>
				<td class="action-btn-disable" id="object-registerCls_<?php print $results->cls_id; ?>">
					<?php print t($regMsg); ?>
				</td>
			<?php } ?>

	<?php } else if(($match == true) && ($results->prm_available_seats <= 0 && $results->waitlist_status == 0) && ($condCount == 1)) { ?>
		<td class="action-btn-disable" id="object-registerCls_<?php print $results->cls_id; ?>">
			<?php print t('LBL046'); ?>
		</td>
	<?php } else {
				$seatDisplay = true;
				if((count($preRequisite)>0)) { ?>
						<td class="action-btn"
							id="prereqCourse<?php print $results->cls_id; ?>"
							<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false))){ ?>
						  	onclick="passUrlCommonCall(this.id);">
						  <?php }else{ ?>
									<?php if($userId!="" && $userId>0){ ?>
									onclick="$('body').data('prerequisite').getTpPrerequisites(<?php print $results->cls_id; ?>,'<?php  print $results->widgetId;?>','initPrereq');"
									<?php }else{ ?>
									onclick="$('body').data('learningcore').callSigninWidget(this.id);"
									<?php }?>>
							<?php }?>
							<div><?php print t('LBL230'); ?></div>
							<?php // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //?>
							<?php  if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && $results->prm_price > 0)  { ?>
							<div class="<?php print $entityColor; ?>"><?php if($condCount == 1) print $results->prm_currency_type ." ".$results->prm_price;?></div>
							<?php } ?>
						</td>
				<?php  }
				else if(($registerEndDateCheck <> 0) && ($condCount == 1)){	?>
						<td class="action-btn-disable" id="registerCls_<?php print $results->cls_id; ?>">
			<?php print t('LBL047'); //Closed ?>
		</td>
					<?php
				}
				else  if(/*$condCount == 1 &&*/ !empty($_SESSION['availableFunctionalities']->exp_sp_commerce))  { ?>
							<?php if($results->is_cart_added){  ?>
								<td class="action-btn-disable" id="object-addToCartList_<?php print $results->cls_id; ?>">
									<?php print t('LBL049'); ?>
								</td>
							<?php } else if($results->prm_price != '0.00'){  ?>
								<td data="<?php print $passData;?>" class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
									id="object-addToCartList_<?php print $results->cls_id; ?>"
									<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
								  	onclick="passUrlCommonCall(this.id);">
								  <?php }else{ ?>
											<?php if($userId!="" && $userId>0){ ?>
											onclick="$('body').data('learningcore').callPopupOrNot('<?php print $results->widgetId; ?>','<?php print $results->cls_id; ?>','Cart','N','');"
											<?php }else{ ?>
											onclick="$('body').data('learningcore').callSigninWidget(this.id);"
											<?php }?>>
									<?php }?>
								<div><?php print ($waitlist_register > 0) ? t("Waitlist") : t('LBL050'); ?></div>
								<?php print !empty($_SESSION['availableFunctionalities']->exp_sp_commerce) ? ("<div class='".$entityColor."'>".$results->prm_currency_type ." ".$results->prm_price."</div>") : '' ;?>
								</td>
							<?php } else { ?>
								<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
									id="object-registerCls_<?php print $results->cls_id; ?>"
									<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
								  	onclick="passUrlCommonCall(this.id);">
								  <?php }else{ ?>
											<?php if($userId!="" && $userId>0){ ?>
											onclick="$('body').data('learningcore').callPopupOrNot('<?php print $results->widgetId; ?>','<?php print $results->cls_id; ?>','','N','');"
											<?php }else{ ?>
											onclick="$('body').data('learningcore').callSigninWidget(this.id);"
											<?php }?>>
									<?php }?>
								<div><?php print ($waitlist_register > 0) ? t("Waitlist") : t('Register'); ?></div>
								<?php  // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //?>
								<?php print (!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && $results->prm_price > 0) ? ("<div class='".$entityColor."'>".$results->prm_currency_type ." ".$results->prm_price."</div>") : '';?>
								</td>
						<?php  } ?>

				<?php } else {  ?>
						<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>" id="object-registerCls_<?php print $results->cls_id; ?>"
						<?php if((isset($_SESSION['widget']['pass_url']) &&  ($_SESSION['widget']['pass_url']===TRUE || $_SESSION['widget']['pass_url']==1) && (strpos($referer_url,'page_number') === false && strpos($referer_url,'click_id') === false && strpos($referer_url,'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')){ ?>
					  	onclick="passUrlCommonCall(this.id);">
					  <?php }else{ ?>
								<?php if($userId!="" && $userId>0){ ?>
								onclick="$('body').data('learningcore').callPopupOrNot('<?php print $results->widgetId; ?>','<?php print $results->cls_id; ?>','','N','');"
								<?php }else{ ?>
								onclick="$('body').data('learningcore').callSigninWidget(this.id);"
								<?php }?>>
						<?php }?>
								<?php print  ($waitlist_register > 0) ? t("Waitlist") : t('Register'); ?>
						</td>
				<?php } ?>
				<?php } ?>




	</tr>
</table>
</div>
<?php }?>
<!--<?php //if($seatDisplay == true) { ?>
	<div class="course-available-seats">
	<?php //if($results->prm_available_seats != -1){ // -1 is to identify only if wbt class under training plan. ?>
		<?php //if($results->prm_available_seats == 1){ ?>
			<span id="object-seats_available_<?php //print $results->cls_id; ?>">
				<?php //print $results->prm_available_seats; ?>&nbsp;<?php //print t('Seat left'); ?>
			</span>
		<?php //}else { ?>
			<span id="object-seats_available_<?php //print $results->cls_id; ?>">
				<?php //print $results->prm_available_seats; ?>&nbsp;<?php //print t('Seats left'); ?>
			</span>
		<?php //} ?>
	<?php //} ?>
	</div>
<?php //} ?>

-->