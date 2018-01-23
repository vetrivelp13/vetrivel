<div class="content-top-record-div top-record-div">
	<div class="content-search-register-btn detail-register-btn"><?php print $result->htmlout;?></div>
</div>
<?php if(contentPlayerIsActive()){?>
	<?php if($callFrom=='lrnplan_details' || $result->catalog_tp_progress){?>
		<div class="progress prm_progress_<?php echo $result->program_id?>" id="prm_progress_<?php echo $result->master_enrolled_id?>" <?php if($result->showprogress===false){?> style="display:none;"<?php }?>></div>
	<?php }else if($callFrom=='lrnplan_course_class_list'){
		?>
		<div class="progress progress_<?php echo $result->class_id?>" id="progress_<?php echo $result->enrolled_id?>" <?php if($result->showTpprogress===true){?> style="display:block;"<?php }else{?> style="display:none;" <?php } ?> ></div>
		<?php 
	}else{?>
		<div class="progress progress_<?php echo $result->class_id?>" id="progress_<?php echo $result->enrolled_id?>" <?php if($result->showprogress===false){?> style="display:none;"<?php }?>></div>
	<?php }
}else{
	if($callFrom=='lrnplan_details'){?>
		<div class="progress prm_progress_<?php echo $result->program_id?>" id="prm_progress_<?php echo $result->master_enrolled_id?>" <?php if($result->showprogress===false){?> style="display:none;"<?php }?>></div>
	<?php }
}?>
<?php if($result->attempt_left_block===true): ?>
<div class="course-attempt-left">
		<span class="<?php echo $result->attempt_left_class?>" id="<?php echo $result->attempt_left_id?>"><?php echo $result->attempt_left_content?></span>
</div>
<?php if($result->validity_content!='' || $result->video_progress_content!=''):?>
<div class="course-content-validity">
		
		<?php if($result->attempt_left_content!='' && $result->validity_content!=''){?>
			<span id="<?php echo $result->validity_label_id?>" class="<?php echo $result->validity_label_class?>"><?php echo $result->validity_content_label?></span>
		<?php }?>
		
		<span id="<?php echo $result->validity_id?>" class="<?php echo $result->validity_class?>"><?php echo $result->validity_content?></span>
		
		<?php if($result->video_progress_content!='' && ($result->attempt_left_content!='' || $result->validity_content!='')):?>
			<span id="<?php echo $result->validity_label_id?>" class="<?php echo $result->validity_label_class?>"><?php echo $result->validity_content_label?></span>
		<?php endif; ?>
		
		<span id="<?php echo $result->video_progress_id?>" class="<?php echo $result->video_progress_class?>"><?php echo $result->video_progress_content?></span>
</div>
<?php endif; ?>
<?php endif; ?>
<?php if($result->seat_block===true){?>
<div class="content-available-seats" id="content-available-seats_<?php echo $result->class_id?>">
	<span class="<?php echo $result->seat_class?>" id="<?php echo $result->seat_id?>"><?php echo $result->seat_content?></span>
</div>

<?php

}
expDebug::dPrint('content action ' . var_export($result, 1), 5);
if(contentPlayerIsActive() && $result->showprogress === true) {
	if($callFrom=='lrnplan_details')
		$progress_initialize = "$(function() { progressBarRound('" . $result->master_enrolled_id . "', '" . $result->progress . "', 'enr_progress','prm_progress_',$(\"#lnr-catalog-search\").data(\"contentPlayer\"));});";
//		$progress_initialize = "$(function() { $(\"#lnr-catalog-search\").data(\"contentPlayer\").progressbardetail('" . $result->master_enrolled_id . "', '" . $result->progress . "', 'enr_progress','prm_progress_');});";
	else if($callFrom=='lrnplan_course_class_list' && $result->showTpprogress === true)
		$progress_initialize = "$(function() { progressBarRound('" . $result->enrolled_id . "', '" . $result->progress . "', 'enr_progress','progress_',$(\"#lnr-catalog-search\").data(\"contentPlayer\"));});";
		//$progress_initialize = "$(function() { $(\"#lnr-catalog-search\").data(\"contentPlayer\").progressbardetail('" . $result->enrolled_id . "', '" . $result->progress . "', 'enr_progress','progress_');});";
	else {
		if($result->catalog_tp_progress) {
			$progress_initialize = "$(function() { progressBarRound('" . $result->master_enrolled_id . "', '" . $result->progress . "', 'enr_progress','prm_progress_',$(\"#lnr-catalog-search\").data(\"contentPlayer\"));});";
		} else {
			$progress_initialize = "$(function() { progressBarRound('" . $result->enrolled_id . "', '" . $result->progress . "', 'enr_progress','progress_',$(\"#lnr-catalog-search\").data(\"contentPlayer\"));});";
			//$progress_initialize = "$(function() { $(\"#lnr-catalog-search\").data(\"contentPlayer\").progressbardetail('" . $result->enrolled_id . "', '" . $result->progress . "', 'enr_progress','progress_');});";
		}
	}
		
	if($callFrom == 'catalog') {
		print "<script type=text/javascript>
				$progress_initialize
				</script>";
	} else {
		drupal_add_js("$progress_initialize", 'inline');
	}
}else{
	if($callFrom=='lrnplan_details'){
		$progress_initialize = "$(function() { progressBarRound('" . $result->master_enrolled_id . "', '" . $result->progress . "', 'enr_progress','prm_progress_',$(\"#lnr-catalog-search\").data(\"contentPlayer\"));});";
		//$progress_initialize = "$(function() { progressbarlpdetail('" . $result->master_enrolled_id . "', '" . $result->progress . "', 'enr_progress','prm_progress_');});";
		drupal_add_js("$progress_initialize", 'inline');
	}
}
?>