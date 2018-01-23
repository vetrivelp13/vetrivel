<?php 
	global $theme_key;
?>
<table class="content region-sidebar-widget-bg">
	<tbody>
		<tr>
			<td>
				<div id="LnrSearchService_searchOptDiv">
					<div class="search-list" id="LnrSearchService_searchoptions">
						<div class="search-opts" id="searchopts-content">   
						<?php if($theme_key != 'expertusoneV2') {?>
						<div>
							<span class="srch-filter-heading"><?php print t('Narrow Results');?></span>
						</div>									
            <?php } ?>
					    <?php if(user_access('Create Report Perm')){ ?>
						<div id="publish_unpublish" class="search-opt-box">
							<ul class="find-list-items">
								<li>
									<span id="publishUnpublishHideShow" class="cls-show " onclick='$("#lnr-reports-search").data("lnrreportssearch").showHide("publishUnpublishHideShow","paintpublishUnpublish");'>
										<a class="search-heading"><?php print t('LBL102');?></a>
									</span>
								</li>
							</ul>
							<div id="paintpublishUnpublish">
								<div id="hideshow_publish" class="srch-checkbox-container-cls" style="display: block;">
										<label for="lrn_srch_publish" class="checkbox-unselected"><input id="lrn_srch_publish" class="reports-status srch-checkbox-cls" value="cre_rpt_rps_atv" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"></label>
										<label for="lrn_srch_publish" class="srch-label-cls highlight-light-blue"><?php print titleController('NARROW-SEARCH-FILTERSET-CHECKBOX',  t('Published'), 15);?></label>
								</div>

								<div id="hideshow_unpublish" class="srch-checkbox-container-cls" style="display: block;">
										<label for="lrn_srch_unpublish" class="checkbox-unselected"><input id="lrn_srch_unpublish" class="reports-status srch-checkbox-cls" value="cre_rpt_rps_itv" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"></label>
										<label for="lrn_srch_unpublish" class="srch-label-cls highlight-light-blue"><?php print titleController('NARROW-SEARCH-FILTERSET-CHECKBOX',t('Unpublished'), 15);?></label>
								</div>
							</div>
						</div>
						<?php } ?>
						<?php if(count($roles['default_role']) > 0){?>
					    <?php //if(count($access) > 1){ ?>
						<div id="search_delivery_type" class="search-opt-box">
							<ul class="find-list-items">
								<li>
									<span id="groupHideShow" class="cls-show " onclick='$("#lnr-reports-search").data("lnrreportssearch").showHide("groupHideShow","paintGroupContent");'>
										<a class="search-heading"><?php print t('Group');?></a>
									</span>
								</li>
							</ul>
						<div id="paintGroupContent">
							<div id="paintDeliveryType" class ="paintDeliveryType">
							<?php $langCount = 0;	?>
    							    <?php foreach($roles['default_role'] as $type){
    							    	$langCount = $langCount+1;
    							    	$type->value=sanitize_data($type->value); /* Added fix for avoid security issue - By Ganeshbabuv, June 26th 2015 8:40PM */
    							            //if(in_array($type->code, $access)){?>
  									 <div id="delivery_hideshow_<?php print $langCount; ?>" class="srch-checkbox-container-cls " <?php //if ($langCount>=5) { print 'style="display:none"'; } else { print 'style="display:block"'; } ?>>  
												<label for="lrn_srch_<?php print $type->code;?>" class="checkbox-unselected"><input id="lrn_srch_<?php print $type->code;?>" class="reports-roles srch-checkbox-cls" value="<?php print $type->code;?>" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"></label> 									
  											<label for="lrn_srch_<?php print $type->code;?>" title="<?php print t($type->value); ?>" class="srch-label-cls highlight-light-blue vtip"><?php print titleController('NARROW-SEARCH-FILTERSET-CHECKBOX', t($type->value), 15);?></label>
  									 </div> 
    								<?php  }// } ?>
    									<?php  //if ($langCount>4) {?>
    									
							<!--	<div id='delivery_hideshow_more' class='display-more' onClick='$("#lnr-reports-search").data("lnrreportssearch").moreListDisplay(<?php //print $langCount; ?>,"delivery_hideshow");'><?php //print t("LBL543"); ?></div>
								<div id='delivery_hideshow_short' style='display:none;' class='display-short' onClick='$("#lnr-reports-search").data("lnrreportssearch").shortListDisplay(<?php //print $langCount; ?>,"delivery_hideshow");'><?php //print t("LBL544"); ?></div> -->
								<?php  //}  ?>
								</div>
							</div>
						</div>
						<input id= 'group_list' type ="hidden" value = "<?php print $langCount ?>"/>
						<?php } ?>

						<div id="report_types" class="search-opt-box">
							<ul class="find-list-items">
								<li>
									<span id="typeHideShow" class="cls-show " onclick='$("#lnr-reports-search").data("lnrreportssearch").showHide("typeHideShow","paintType");'>
										<a class="search-heading"><?php print strtolower(t('LBL036'));?></a>
									</span>
								</li>
							</ul>
							<?php 
							foreach($types as $key=>$ar){
							if($ar->code == 'cre_rpt_rpc_oth'){							   		
								$unset  = $key;
								break;
								}
							}							
							$val    = $types[$unset];
							unset($types[$unset]);
							$types[]  = $val;							
							?>
							<div id="paintType">
							    <?php foreach($types as $type){?>
								<div id="hideshow_<?php print $type->code;?>" class="srch-checkbox-container-cls" style="display: block;">
    									<label for="lrn_srch_<?php print $type->code;?>" class="checkbox-unselected"><input id="lrn_srch_<?php print $type->code;?>" class="reports-types srch-checkbox-cls" value="<?php print $type->code;?>" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"></label>
    									<label for="lrn_srch_<?php print $type->code;?>" class="srch-label-cls highlight-light-blue"><?php print titleController('NARROW-SEARCH-FILTERSET-CHECKBOX',t($type->value), 15);?></label>
								</div>
								<?php } ?>
							</div>
						</div>

						<div id="reports_managedby" class="search-opt-box">
							<ul class="find-list-items">
								<li>
									<span id="managedbyHideShow" class="cls-show " onclick='$("#lnr-reports-search").data("lnrreportssearch").showHide("managedbyHideShow","paintmanagedby");'>
										<a class="search-heading"><?php print strtolower(t('Managed By'));?></a>
									</span>
								</li>
							</ul>
							<div id="paintmanagedby">
							    <?php foreach($managedby as $key => $val){?>
								<div id="hideshow_<?php print $key;?>" class="srch-checkbox-container-cls" style="display: block;">
										<?php if($key == 'cre_sys_fop_me'){?>
										<?php expDebug::dPrint('$filter_By : '.print_r($filter_By , true), 4); ?>
										<?php  $checkboxSelectedClass =  ($filter_By == 'me')? 'checkbox-selected': 'checkbox-unselected';?>
										 <?php $checkedStatus =  ($filter_By == 'me')? 'checked': ''; ?>
											<label for="lrn_srch_<?php print $key;?>" class="<?php print $checkboxSelectedClass;?>"> 
												<input id="lrn_srch_<?php print $key;?>" class="reports-manage srch-checkbox-cls" value="<?php print $key;?>" 
												onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"  
												<?php print $checkedStatus; ?>>
											</label>
    								<?php }else{?>	
    									<label for="lrn_srch_<?php print $key;?>" class="checkbox-unselected"><input id="lrn_srch_<?php print $key;?>" class="reports-manage srch-checkbox-cls" value="<?php print $key;?>" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();' type="checkbox"></label>
    								<?php }?>	
    									<label for="lrn_srch_<?php print $key;?>" class="srch-label-cls highlight-light-blue"><?php  print titleController('NARROW-SEARCH-FILTERSET-CHECKBOX',t($val), 15);?></label>
								</div>
								<?php } ?>
							</div>
						</div>
						<!-- 
					    <div id="search_report_name" class="search-opt-box" style="display: block;">
							<ul class="find-list-items">
								<li>
									<span id="locationHideShow" class="cls-show" onclick='$("#lnr-reports-search").data("lnrreportssearch").showHide("locationHideShow","paintReportName");'>
										<a class="search-heading">REPORT NAME</a>
									</span>
									<span id="location-clr" class="clr-txt" style="display: none;" onclick='$("#lnr-reports-search").data("lnrreportssearch").clearField();'>clear</span>
								</li>
							</ul>
							<div id="paintReportName">
								<div id="location_hideshow" class="srch-checkbox-container-cls" style="display: block;">
									<input name="srch_criteria_report_title" class="ac_input searchlocation" size="15" maxlength="15" id="srch_criteria_report_title" autocomplete="off"  onkeypress='$("#lnr-reports-search").data("lnrreportssearch").reportEnterKey();this.style.fontSize="13px";this.style.fontStyle="normal";' onchange="$('#lnr-reports-search').data('lnrreportssearch').hightlightedText('srch_criteria_report_title','Type a report name')" value="Type a report name" onblur="$('#lnr-reports-search').data('lnrreportssearch').hightlightedText('srch_criteria_report_title','Type a report name')"  onfocus="$('#lnr-reports-search').data('lnrreportssearch').hightlightedText('srch_criteria_report_title','Type a report name')" onchange="$('#lnr-reports-search').data('lnrreportssearch').hightlightedText('srch_criteria_report_title','Type a report name')" type="text">
									<a class="location-search" title="search" onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction();'>&nbsp;</a>
								</div>
							</div>
					 </div>
					  -->
				</div>
			</td>
		</tr>
	</tbody>
</table>
<script language="Javascript" type="text/javascript">
$("#lnr-reports-search").data("lnrreportssearch").paintReportAutocomplete();
</script>