<?php
global $theme_key;
$title_len = 0;
if($theme_key == 'expertusoneV2'){
	$title_len = 45;
}else{
	$title_len = 20;
}
if($class_title){
  $curTitle = titleController('EXP-SP-MYTEAM-MYCLASS-VIEW-CLASSTITLE',sanitize_data($class_title->class_title),$title_len,40);
  print '<span class="vtip" title="'.sanitize_data($class_title->class_title).'">'.$curTitle.'</span>';
}
if($enrolled_on){
  print date_format(date_create($enrolled_on->enrolled_on),'M d, Y');
} 
if($completed_on){
  /*  changes for ticket 16260 - changes to performed on date showing default date */
  if($completed_on->comp_status=='lrn_crs_cmp_cmp' || $completed_on->comp_status=='lrn_tpm_ovr_cmp'){
  	if($completed_on->completed_on != "")
    $performedDate = date_format(date_create($completed_on->completed_on),'M d, Y');
  }else if($completed_on->comp_status=='lrn_crs_cmp_enr' || $completed_on->comp_status=='lrn_tpm_ovr_enr'){
    if($completed_on->enrolled_on != "")
    $performedDate = date_format(date_create($completed_on->enrolled_on),'M d, Y');
  }else if($completed_on->reg_status=='lrn_crs_reg_can' || $completed_on->reg_status=='lrn_tpm_ovr_cln' || $completed_on->comp_status=='lrn_crs_cmp_nsw'){
  	if($completed_on->updated_on != "")
  	$performedDate = date_format(date_create($completed_on->updated_on),'M d, Y');
  }else if($completed_on->comp_status=='lrn_crs_cmp_cln'||comp_status=='lrn_tpm_ovr_cln'){
  	if($completed_on->canceled_on != "")
    $performedDate = date_format(date_create($completed_on->canceled_on),'M d, Y');
  }else if($completed_on->comp_status=='lrn_crs_cmp_inc'){
    if($completed_on->completed_on != "")
    $performedDate = date_format(date_create($completed_on->completed_on),'M d, Y');
  }else if($completed_on->comp_status=='lrn_tpm_ovr_inc'){
    if($completed_on->updated_on != "")
    $performedDate = date_format(date_create($completed_on->updated_on),'M d, Y');
  }
  print '<span class="myteam-learning-performed-on">'.$performedDate.'</span>';
} 
if($type){
  print $type->type;
} 
if($status){
  $regStat = $status->reg_status;
  $compStat= $status->comp_status;
  if($compStat=='lrn_crs_cmp_cmp' || $compStat=='lrn_tpm_ovr_cmp')
    $printStatus = t('Completed');
  else if($compStat=='lrn_crs_cmp_enr' || $compStat=='lrn_tpm_ovr_enr')
    $printStatus = t('Enrolled');
  else if($compStat=='lrn_crs_cmp_nsw')
  	$printStatus = t('No Show');
  else if($regStat=='lrn_crs_reg_can' || $compStat=='lrn_tpm_ovr_cln')
  	$printStatus = t('Canceled');
  else if($regStat=='lrn_crs_reg_ppm' || $compStat=='lrn_tpm_ovr_ppv' || $compStat=='lrn_tpm_ovr_ppm')
  	$printStatus = t('Pending');
  else if($regStat=='lrn_crs_reg_wtl' || $regStat=='lrn_tpm_ovr_wtl')
    $printStatus = t('LBL190'); //Waitlisted
  else if($compStat=='lrn_crs_cmp_inc' || $compStat=='lrn_tpm_ovr_inc')
    $printStatus = t('Incomplete');
  else if($compStat=='lrn_crs_cmp_inp' || $compStat=='lrn_tpm_ovr_inp')
    $printStatus = t('In progress');	
  else if($compStat=='lrn_crs_cmp_att')
    $printStatus = t('Attended');	
    $comTitle = titleController('MYTEAM_STATUS',$printStatus);
  	print '<span class="vtip myteam-learning-status" title="'.$printStatus.'">'.$comTitle.'</span>';
} 
if($action && $action->comp_status!='lrn_crs_cmp_cmp' && $action->reg_status!='lrn_crs_reg_can' && $action->comp_status!='lrn_tpm_ovr_cmp' && $action->comp_status!='lrn_tpm_ovr_cln' 
		&& $action->comp_status!='lrn_crs_cmp_nsw' && $action->comp_status!='lrn_tpm_ovr_inc' && $action->comp_status!='lrn_crs_cmp_inc'){ // Ram : #0036253 Incompleted Tp and Class Can be Mark Complete.
	$markcomplete = getManagerMarkComplete();
	expDebug::dPrint('$action->comp_status  1 : '.$action->comp_status , 4);
	if($markcomplete=='Y'){
?>

<div id="sessStartEndDates_<?php print $action->class_id;?>" style="display:none;"><?php print getSessionDates($action->class_id); ?></div>

 <div class="myteam-learning-course-btn">
 <?php if($action->comp_status=='lrn_crs_cmp_inp'){
 	if($action->reg_status!='lrn_crs_reg_ppm' && $action->comp_status!='lrn_tpm_ovr_ppm' && $action->SessionCompleted=='1' && $action->reg_status!='lrn_crs_reg_wtl') { ?>
 		<span class="view-btn-left"></span><span class="view-btn-middle vtip" title="<?php print t("LBL059") ?>" style="margin-left: 0px;" href="javascript:void(0)"  name="Mark Complete" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").markCompleteWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print rawurlencode($action->class_title);?>",this,"<?php print $action->learning_type; ?>","<?php print $action->class_id; ?>","<?php print $action->row ?>");'><?php print titleController('EXP-SP-MYTEAM-COMPLETED-STATUS',t("LBL059"),15);?></span><span class="view-btn-right"></span>  
 		<?php }
 }else{?>
 <?php if($action->reg_status!='lrn_crs_reg_ppm' && $action->comp_status!='lrn_tpm_ovr_ppm' && $action->SessionCompleted=='1' && $action->reg_status!='lrn_crs_reg_wtl') {?>
	 <span class="view-btn-left"></span><span class="view-btn-mid" class="manager-view-learning-btn" id="viewLearningCls_<?php print $results->user_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").rejectWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print  rawurlencode($action->class_title);?>",this,"<?php print $action->base_type; ?>","<?php print $action->class_id; ?>", "<?php print $action->row ?>");'><?php print t('LBL069'); ?></span> 
	<a href="javascript:void(0);" title=<?php print t('LBL713'); ?> class="myteam-learning-arrow"  onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showMoreAction(this)' onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").hideMoreAction(this)'></a>
    <?php  }else {?> 
        <span  class="full-button-without-action" id="viewLearningCls_<?php print $results->user_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").rejectWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print rawurlencode($action->class_title);?>",this,"<?php print $action->base_type; ?>","<?php print $action->class_id; ?>", "<?php print $action->row ?>");'><?php print t('LBL069'); ?></span>
	<?php }if($action->reg_status!='lrn_crs_reg_ppm' && $action->comp_status!='lrn_tpm_ovr_ppm' && $action->SessionCompleted=='1' && $action->reg_status!='lrn_crs_reg_wtl') { ?>
	 <div class="myteam-more-action"><ul class="myteam-more-drop-down-list"><li class="action-enable"><a href="javascript:void(0)" class="actionLink" name="Mark Complete" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").markCompleteWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print rawurlencode($action->class_title);?>",this,"<?php print $action->learning_type; ?>","<?php print $action->class_id; ?>","<?php print $action->row ?>");'><?php print t("LBL059");?></a></li></ul></div>  
		<?php }} ?>
  </div>
<?php 
	}else{
		?>
  <div class="myteam-learning-course-btn">
    <span  class="full-button-without-action" id="viewLearningCls_<?php print $results->user_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").rejectWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print rawurlencode($action->class_title);?>",this,"<?php print $action->base_type; ?>","<?php print $action->class_id; ?>", "<?php print $action->row ?>");'><?php print t('LBL069'); ?></span>
  </div>
<?php 
	}
} 
?>