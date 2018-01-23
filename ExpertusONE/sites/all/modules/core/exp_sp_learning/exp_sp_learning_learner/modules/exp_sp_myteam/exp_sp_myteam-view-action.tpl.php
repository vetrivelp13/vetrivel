<?php 
global $theme_key;
$passData = "data={'userId':'".$results->user_id."'}";
$userName  = addslashes($results->last_name.", ".$results->first_name);
expDebug::dPrint('exp_sp_myteam-view-action-tpl.php : $userName = ' . print_r($userName, true),4);
/*$profileitems = getGradeProfileItems();
foreach ($profileitems as $profile_records) {
  $grade .= "<option value=\"$profile_records->code\">$profile_records->name</option>"; 
}
expDebug::dPrint('exp_sp_myteam-view-action-tpl.php : $grade = ' . print_r($grade, true),4);*/

?>
<div class="myteam-learning-btn">
<!-- 	<span data="<?php //print $passData;?>" class="manager-view-learning-btn" id="viewLearningCls_<?php //print $results->user_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").addAccordionToTableCall("open_close","0 0","0 -61px","dt-child-row-En",$("#lnr-myteam-search").data("lnrmyteamsearch").viewLearning,this,$("#lnr-myteam-search").data("lnrmyteamsearch"),true);'><?php //print t('LBL055'); ?></span> -->
	<?php if($theme_key == 'expertusoneV2') {?>
	<div class="view-btn-left"></div>
	<div class="view-btn-mid">
		<a class="view-learning-txt vtip" id="viewLearningCls_<?php print $results->user_id; ?>" data-is_last_rec="<?php print empty($results->is_last_rec)? '' : 'last';?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").viewLearning(<?php print $results->user_id;?>,this);' title="<?php print t('LBL055'); ?>"><?php print titleController('TEAM-ASSIGN-LEARNING', t('LBL055'),30);?></a>
	
	<div class="dd-btn"><span class="dd-arrow"></span><a class="team-launch-more" title="More" onclick="$('#lnr-myteam-search').data('lnrmyteamsearch').showMoreTeamAction(this,<?php print $results->user_id;?>)" onblur="$('#lnr-myteam-search').data('lnrmyteamsearch').hideMoreTeamAction(this)"></a></div>

<div id="more-<?php print $results->user_id;?>" class="team-more-action" style="display: none;">
	<ul class="team-more-drop-down-list">
		<li class="action-enable">
			<a href="javascript:void(0)" class="team-listLink" name="Assing Learning" onclick="$('#lnr-myteam-search').data('lnrmyteamsearch').showLearningCatalog('<?php print $results->user_id;?>', '<?php print $userName;?>', this);" ><?php print titleController('TEAM-ASSIGN-LEARNING-MORE', t('LBL085'),30);?></a></li>
		</ul></div>
	

	<div class="view-btn-right"></div>
	<?php }else{?>
	<span class="view-btn-left"></span>
	<span class="view-btn-mid manager-view-learning-btn" id="viewLearningCls_<?php print $results->user_id; ?>" data-is_last_rec="<?php print empty($results->is_last_rec)? '' : 'last';?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").viewLearning(<?php print $results->user_id;?>,this);'><?php print t('LBL055'); ?></span> 
	<a href="javascript:void(0);" title=<?php print t('LBL713'); ?> class="myteam-learning-arrow"  onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showMoreAction(this)' onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").hideMoreAction(this)'></a>
	<div class="myteam-more-action"><ul class="myteam-more-drop-down-list"><li class="action-enable"><a href="javascript:void(0)" class="actionLink" name="Assing Learning" onclick="$('#lnr-myteam-search').data('lnrmyteamsearch').showLearningCatalog('<?php print $results->user_id;?>', '<?php print $userName;?>', this);" ><?php print t('LBL085');?></a></li></ul></div>
	<?php }?>
</div>
<!-- <div style="display:none;" id="mark-complete-grade"><?php //echo $grade; ?></div> -->
<div style="display:none;"><input type="hidden" id="mark-complete-date" value="<?php echo date('m-d-Y'); ?>"/></div>