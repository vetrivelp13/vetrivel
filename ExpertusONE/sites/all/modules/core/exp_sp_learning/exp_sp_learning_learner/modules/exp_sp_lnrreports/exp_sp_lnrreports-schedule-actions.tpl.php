<?php
  expDebug::dPrint('exp_sp_lnrreports-schedule-actions.tpl.php: $id 1234555---> = ' 	.$schedule_id, 4);
  expDebug::dPrint('exp_sp_lnrreports-schedule-actions.tpl.php: $id 1234555---> = ' 	.$report_id, 4);

?>

      <div class="schedule-actions-link" <?php print $class;?>>
       <span class="vtip" title="<?php print t("LBL063"); ?>" > <!-- Edit -->
	        <a id="click-to-editschedule-<?php print $schedule_id;?>" class="edit-schedule addedit-form-expertusone-throbber use-ajax"
	        data-wrapperid="schedules-list-wrapper-<?php print $schedule_id;?>" href="?q=ajax/administration/report-search/schedule-form/<?php print $report_id;?>/<?php print $schedule_id;?>">
	        <?php print t("LBL063"); ?>
	        </a>
	        <span class="schedule-pipeline">|</span>
        </span>        
         <?php if ($status == 'cre_rpt_rss_atv') { ?>
        <span class="vtip" title="<?php print t("LBL1226"); ?>" > <!-- Run -->
	        <a id="click-to-runschedule-<?php print $schedule_id;?>" class="run-schedule   addedit-form-expertusone-throbber use-ajax"
	        data-wrapperid="schedules-list-wrapper-<?php print $schedule_id;?>" href="?q=ajax/administration/report-search/schedule/run/<?php print $schedule_id;?>">
	        <?php print t('LBL1226'); ?>
	        </a>
	        <span class="schedule-pipeline">|</span>
        </span>
         
        <?php } else {?>
        	<a id="click-to-runschedule-<?php print $schedule_id;?>" class="inactive-run-schedule ">
        	</a>
        	<span class="schedule-pipeline">|</span>
        </span>	
         <?php }
   
	         if ($status == 'cre_rpt_rss_atv') {
	          $statusFlag = 0;
	          $statusLabel = t('LBL572');
	          $class = 'suspend-schedule';
	        }
	        else {
	        	$statusFlag = 1;
	        	$statusLabel = t('LBL573');
	        	$class = 'activate-schedule';
	        }?>
	      <span class="vtip" title="<?php print $statusLabel; ?>" >
	        <a id="click-to-activateschedule-<?php print $schedule_id;?>" class="<?php print $class;?>  addedit-form-expertusone-throbber use-ajax"
	        data-wrapperid="schedules-list-wrapper-<?php print $schedule_id;?>" href="?q=ajax/administration/report-search/schedule/status/<?php print $report_id;?>/<?php print $schedule_id;?>/<?php print $statusFlag; ?>">
	        <?php print $statusLabel; ?>
	        </a>
	        <span class="schedule-pipeline">|</span>
        </span>
        <span class="vtip" title="<?php print t("LBL286"); ?>" > <!-- Delete -->
	        <a id="click-to-deleteschedule-<?php print $schedule_id;?>" class="delete-schedule  addedit-form-expertusone-throbber use-ajax"
	        data-wrapperid="schedules-list-wrapper-<?php print $schedule_id;?>" href="?q=administration/report-schedule/datagrid/delete/<?php print $report_id;?>/<?php print $schedule_id;?>">
	        <?php print t("LBL286"); ?>
	        </a>
        </span>
      </div>