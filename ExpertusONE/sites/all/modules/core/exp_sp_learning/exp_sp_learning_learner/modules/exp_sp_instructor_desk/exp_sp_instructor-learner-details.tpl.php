<?php
if($class_title){
  $curTitle = subStringController($class_title->class_title,40);
  print '<span class="vtip" title="'.$class_title->class_title.'">'.$curTitle.'</span>';
}
if($enrolled_on){
  print date_format(date_create($enrolled_on->enrolled_on),'M d, Y');
} 
if($completed_on){
  if($completed_on->comp_status=='lrn_crs_cmp_cmp')
    $performedDate = date_format(date_create($completed_on->completed_on),'M d, Y');
  else if($completed_on->reg_status=='lrn_crs_reg_can' || $completed_on->comp_status=='lrn_crs_cmp_nsw')
  	$performedDate = date_format(date_create($completed_on->updated_on),'M d, Y');
  else if($completed_on->comp_status=='lrn_tpm_ovr_cmp')
    $performedDate = date_format(date_create($completed_on->completed_on),'M d, Y');
  else if($completed_on->comp_status=='lrn_tpm_ovr_cln')
    $performedDate = date_format(date_create($completed_on->canceled_on),'M d, Y');
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
  else if($compStat=='lrn_crs_cmp_enr' || $compStat=='lrn_crs_cmp_inp' || $compStat=='lrn_crs_cmp_inc' || $compStat=='lrn_tpm_ovr_enr' || $compStat=='lrn_tpm_ovr_inp' || $compStat=='lrn_tpm_ovr_att')
    $printStatus = t('Enrolled');
  else if($regStat=='lrn_crs_reg_can' || $compStat=='lrn_crs_cmp_nsw' || $compStat=='lrn_tpm_ovr_cln')
  	$printStatus = t('Canceled');
  else if($regStat=='lrn_crs_reg_ppm' || $regStat=='lrn_crs_reg_wtl' || $compStat=='lrn_tpm_ovr_ppv' || $compStat=='lrn_tpm_ovr_ppm')
  	$printStatus = t('Pending');
  	print '<span class="myteam-learning-status">'.$printStatus.'</span>';
} 
if($action && $action->comp_status!='lrn_crs_cmp_cmp' && $action->reg_status!='lrn_crs_reg_can' && $action->comp_status!='lrn_tpm_ovr_cmp' && $action->comp_status!='lrn_tpm_ovr_cln'){
?>
  <div class="myteam-learning-course-btn">
    <span class="view-btn-left"></span><span class="view-btn-mid" class="manager-view-learning-btn" id="viewLearningCls_<?php print $results->user_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").rejectWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print $action->class_title;?>",this,"<?php print $action->learning_type; ?>","<?php print $action->class_id; ?>", "<?php print $action->row ?>");'><?php print t('LBL069'); ?></span> 
	<a href="javascript:void(0);" title="More" class="myteam-learning-arrow"  onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showMoreAction(this)' onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").hideMoreAction(this)'></a>
	<div class="myteam-more-action"><ul class="myteam-more-drop-down-list"><li class="action-enable"><a href="javascript:void(0)" class="actionLink" name="Mark Complete" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").markCompleteWizard("<?php print $action->user_id; ?>","<?php print $action->enrolled_id; ?>","<?php print $action->class_title;?>",this,"<?php print $action->learning_type; ?>","<?php print $action->class_id; ?>","<?php print $action->row ?>");'><?php print t("LBL059");?></a></li></ul></div>
  </div>
<?php 
} 
?>