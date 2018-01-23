<?php
$location  = '';
if(!empty($results->user_city)){
  $location  = $results->user_city;
}
if(!empty($location) && !empty($results->user_state)){
  $location .= ', '.$results->user_state;
}else{
  $location .= $results->user_state;
}

$name = $results->last_name.', '.$results->first_name;
$dataProfile="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/".core_encrypt($results->user_id)."','popupDispId':'profile-qtipid-".$results->user_id."','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_".$results->user_id."','dataloaderid':'lnr-myteam-search','dataIntId':'lnrmyteamsearch','wBubble':'350','hBubble':'auto'}";

?>
<div id='<?php echo "qtipprofileqtip_visible_disp_".$results->user_id."_disp";?>'></div>
<div class="top-record-div-left">
	<div class="myteam-title-Div limit-title-row">
	<!-- Change the user profile list onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showUserProfile("<?php print $results->user_id;?>","<?php print $name;?>"); -->
    	<span class="myteam-user-title limit-title vtip" onclick="$('body').data('learningcore').paintOtherUserProfile(<?php print $dataProfile;?>)" title="<?php print $name;?>">
    	<?php //print $results->last_name.', '.$results->first_name; ?>
    	<?php print $name; ?>
    	</span>
	</div>
	<div class="find-training-list-course">
		<?php if(!empty($results->job_title)){?>	 
		<span id='srch-job-title' class="vtip" title="<?php print t('LBL073').': '.sanitize_data($results->job_title); ?>"><?php print titleController('EXP-SP-MYTEAM-VIEW-JOBTITLE',$results->job_title,20); ?></span>
		<?php }
		if(!empty($results->job_title) && !empty($location)){?>
		<span class="pipe-line">|</span>
		<?php }
		if(!empty($location)){?>	 
		<span id='srch-user-location' class="vtip" title="<?php print t('Location').': '.sanitize_data($location); ?>"><?php print titleController('EXP-SP-MYTEAM-VIEW-LOCATION',$location,20); ?></span>
		<?php }
		if(!empty($results->username) && (!empty($location) || !empty($results->job_title))){?>
		<span class="pipe-line">|</span>
		<?php }
		if(!empty($results->username)){?>	 
		<!--<span id='srch-username' class="vtip" title="<?php print t('LBL054'); ?>"><?php print $results->username; ?></span> -->
		<input type="hidden" id="<?php print "complete-confirmation-wizard_".$results->user_id; ?>" class="complete-confirmation-wizard" value=<?php print $results->username; ?>>	 
		<span id='srch-username' class='vtip' title="<?php print t('LBL054').": ". sanitize_data($results->username);?>"><?php print titleController('EXP-SP-MYTEAM-VIEW-USERNAME',$results->username); ?> </span>
		<?php } ?>
		<span class="pipe-line">|</span>
		<span id='srch-userposition' class="vtip" title="<?php print t('Report'); ?>"><?php print $results->dotted_manager==1?t('Virtual Report'):ucwords(t('SFMSG017')); ?></span>
	</div> 
	<div style="display:none;" class="myteam-class-loaderimg profile-loader" id="user-detail-loader-<?php print $results->drup_user_id;?>"><span class="ui-accordion-header"></span></div>
	<div style="display:none;" class="myteam-class-loaderimg" id="user-detail-<?php print $results->user_id;?>"><span class="ui-accordion-header"></span></div>
</div>