<?php
	global $theme_key;
?>
<div class="top-record-div-left">
	<div>
    	<span>
    		<div class="limit-title-row">
    		<?php
    		  $curTitle = $results->title;
    		  if($results->report_type=='query_builder'){
    		    $reportType='report-query-builder-section';
    		  }else{
    		    $reportType='report-data-source';
    		  }
    		  $typeReport = ucwords(str_replace('_', ' ', $results->report_type));
    		?>
    		<a style="cursor: default;" title="<?php echo htmlentities(empty($results->title)? '[Draft]' : $results->title, ENT_QUOTES, "UTF-8"); ?>" class="limit-title vtip spotlight-item-title" href="#"><?php echo empty($results->title)? '[Draft]' : $curTitle;  ?></a>
    		</div>
    		<?php if(user_access('Create Report Perm')){
    			 if($theme_key == 'expertusoneV2') {
             $editClass = 'enable-edit-icon';
             $deleteClass = 'enable-delete-icon ';
           }
           else{
           	$editClass = '';
           	$deleteClass = '';
           }
    		 ?>
        	<div id="edit_delete">
        		&nbsp;&nbsp;&nbsp;<a href="javascript:void(0);" title="<?php print t('LBL063');?>" onclick="$('body').data('reportsearch').reportAddWizard('<?php echo $reportType;?>',<?php echo $results->id; ?>);$('body').data('reportsearch').loadReportData();" class="vtip <?php print $editClass ?>"><?php if($theme_key != 'expertusoneV2') print t('LBL063');?></a>
        		<span class="pipeline">|</span>
        		<a title="<?php print t('LBL286');?>" class="vtip <?php print $deleteClass ?>" href="javascript:void();" onclick="$('#lnr-reports-search').data('lnrreportssearch').displayDeleteWizard('<?php print t('MSG357').' '.t('Report').' '."&quot".rawurlencode($results->title)."&quot"; ?>',<?php echo $results->id; ?>,'report');"><?php if($theme_key != 'expertusoneV2') print t('LBL286');?></a>
        	</div>
        	<?php } ?>
    		<div class="clearBoth"></div>
    	</span>
	</div>
  	<div id="report_type" >
  		<span class="vtip" title="<?php  print t('LBL036');?>" ><?php print t($typeReport)?></span>
  	</div>
  	<div class="limit-desc-row">
	<div class="record-div find-training-txt">
		<div class="record-div find-training-txt">
			<span class="limit-desc vtip">
		 		<span class="cls-learner-descriptions"> <?php print $results->description;?> </span>
		  </span>
    	</div>
    	</div>
	</div>
	<?php
	   $qtipHolderId = 'create-schedule-' . $results->id;
		 $qtipObj = "{" .
								"  'holderId': '" . $qtipHolderId . "'" .
								", 'id': 'create-schedule-qtip-" . $results->id . "'" .
								", 'placement': 'above-or-below'" .
								"}";
	?>
	<div id="publish_status" class="publish_status">
	<?php $loggedInUserId = getSltpersonUserId();
	if(user_access('Create Report Perm')){ ?>
	    <?php if($results->status == 'cre_rpt_rps_atv') {
		    	     $class = 'unpublish-icon';
				    } else {
				    	 $class = 'publish-icon';
				    }
	    ?>
  		&nbsp;&nbsp;&nbsp;<a class=<?php print $class;?> href="javascript:void(0);" onclick="$('#lnr-reports-search').data('lnrreportssearch').publishUnpublishReport('<?php echo ($results->status == 'cre_rpt_rps_atv') ? 'unpublish' : 'publish'; ?>', <?php echo $results->id; ?>);"><?php echo ($results->status == 'cre_rpt_rps_atv') ? t('LBL571') : t('LBL570'); ?></a>
  	<span id="report-pipeline"class="pipeline">|</span>
  	<?php if($results->status == 'cre_rpt_rps_atv') {?>
  	<span id="schedule_status">
  	<a id="report-schedule-link-<?php echo $results->id; ?>" class="use-ajax report-schedule-region" href="?q=administration/report-search/schedules/<?php echo $results->id; ?>" data-qtip-obj="<?php print $qtipObj;?>"><?php print t('LBL1207');?>
  	</a>
  	<div id="<?php print $qtipHolderId ;?>"></div>
  	</span>
  	<?php } else {?>
  	<span id="schedule_status">
  	<a id="report-disable-status"><?php print t('LBL1207');?>
  	</a>
  	<div id="<?php print $qtipHolderId ;?>"></div>
  	</span>
  	<?php } ?>
  	<?php } else if (is_manager($loggedInUserId) || is_instructor($loggedInUserId)){?>
  		<div id="access_schedule_link">
  		 <a id="report-schedule-link-<?php echo $results->id; ?>" class="use-ajax report-schedule-region" href="?q=administration/report-search/schedules/<?php echo $results->id; ?>" data-qtip-obj="<?php print $qtipObj;?>"><?php print t('LBL1207');?>
  		 </a>
  		 <div id="<?php print $qtipHolderId ;?>"></div>
  		</div>
  	<?php }?>
  	</div>
</div>
<script language="Javascript">vtip();</script>