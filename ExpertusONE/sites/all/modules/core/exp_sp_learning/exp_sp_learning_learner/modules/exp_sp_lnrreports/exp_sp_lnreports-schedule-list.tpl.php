<?php 
expDebug::dPrint('commands in the $id = ' . print_r($id, true), 4);
expDebug::dPrint('commands in the = ' . print_r($report_id, true), 4);
?>
        <div id="schedules-list-wrapper-<?php print $report_id; ?>" class="schedule-list-wrapper">
         <div id="datagrid-div-ReportSchedulesList-<?php print $report_id;?>-cre_rpt_rss" class="datagrid-items-list" data-item-id="<?php print $report_id;?>">
           <table id="datagrid-container-ReportSchedulesList-<?php print $report_id; ?>-cre_rpt_rss" class="datagrid-container-common"></table>
           <div id="pager-datagrid-ReportSchedulesList-<?php print $report_id; ?>-cre_rpt_rss" class="pager-datagrid-common"></div>
         </div>
         
         <div class="addedit-form-cancel-container-actions resize-save_btn">
          <div class="admin-cancel-button-container">
            <div class="white-btn-bg-left"></div>
		       <a id="cancel-scheduling" class="addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle"
		              data-wrapperid="datagrid-div-<?php print $report_id;?>" onclick="$('.active-qtip-div').remove(); calRefreshAftClose();">
		              <?php print t('LBL123'); ?>
		       </a>
            <div class="white-btn-bg-right"></div>
          </div>  
              <div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>
		            <a id="create-schedule-fromgrid" class="addedit-form-expertusone-throbber admin-save-button-middle-bg use-ajax"
		             data-wrapperid="datagrid-div-<?php print $report_id;?>" href="?q=ajax/administration/report-search/schedule-form/<?php print $report_id;?>/0">
		             <?php print t('LBL1207');?>
		            </a>
            <div class="admin-save-button-right-bg"></div>
          </div>
         </div>
          <span class='vtip' title="<?php print t('LBL309');?>">
          <div id='schedule-exportcontainer' class='schedule-exports-icon schedule-exportcontainer'  data-item-id="<?php print $report_id;?>">
          </span> 
          </div>
          
        </div>
   
   