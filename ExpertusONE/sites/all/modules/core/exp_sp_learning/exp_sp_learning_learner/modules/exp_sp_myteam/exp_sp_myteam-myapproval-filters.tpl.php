<?php expDebug::dPrint(' $lnrSrch aaaaa= ' . print_r($location, true) , 5);
expDebug::dPrint(' $lnrSrch rrrrrr= ' . print_r($reporttype, true) , 5);
expDebug::dPrint(' $lnrSrch nnnn= ' . print_r($startdate, true) , 5);
expDebug::dPrint(' $lnrSrch mmm= ' . print_r($enddate, true) , 5);
$name = !empty($location)? $location : t('LBL036').' '.t('LBL107');
if($reporttype=="verified" || $reporttype=="verified|rejected"){
	$verified_class = "checkbox-selected";
	$verified_input = 'checked="checked"';
}else{
	$verified_class = "checkbox-unselected";
	$verified_input = '';
}
if($reporttype=="rejected"|| $reporttype=="verified|rejected"){
	$rejected_class = "checkbox-selected";
	$rejected_input = 'checked="checked"';
}else{
	$rejected_class = "checkbox-unselected";
	$rejected_input = '';
}
$startdateval = (!empty($startdate) && $startdate!="mm-dd-yyyy") ? $startdate :t('LBL251').':'.t('LBL112');
$enddateval = (!empty($enddate) && $enddate!="mm-dd-yyyy") ? $enddate :t('LBL252').':'.t('LBL112') ;
?>
<table class="content region-sidebar-widget-bg">
	<tbody>
		<tr>
			<td>
				<div id="LnrSearchService_searchOptDiv">
					<div class="search-list" id="LnrSearchService_searchoptions">
						<div class="search-opts" id="searchopts-content">   
						<?php global $theme_key,$language;					
						if($theme_key != 'expertusoneV2') {?> 
						<div>
							<span class='srch-filter-heading'><?php print t("Narrow Results"); ?></span>
						</div>
						<?php }?>
								
						<div id='search_report_type' class='search-opt-box'>
							<ul class='find-list-items'>
								<li>
									<span id='reportTypeHideShow' class='cls-show ' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").showHide("reportTypeHideShow","paintReportType");'>
										<a class='search-heading'><?php print ucfirst(strtolower(t('LBL102'))); ?></a>
									</span>
								</li>
							</ul>
							<div id='paintReportType'>
								<div id='report_hideshow_direct' class='srch-checkbox-container-cls' style='display:block'>
									<label for='lrn_srch_verified' class='<?php echo $verified_class; ?>' ><input id='lrn_srch_verified' <?php echo $verified_input; ?> class='report-others srch-checkbox-cls' type='checkbox' value='verified' onClick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction();'/></label>
									<label for='lrn_srch_verified' class='srch-label-cls highlight-light-blue'><?php print t("Verified"); ?></label>
								</div>
								<div id='report_hideshow_virtual' class='srch-checkbox-container-cls' style='display:block'>
									<label for='lrn_srch_rejected' class='<?php echo $rejected_class; ?>' ><input id='lrn_srch_rejected' <?php echo $rejected_input; ?> class='report-others srch-checkbox-cls' type='checkbox' value='rejected' onClick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction();'/></label>
									<label for='lrn_srch_rejected' class='srch-label-cls highlight-light-blue'><?php print ucfirst(strtolower(t("rejected"))); ?></label>
								</div>
							</div>
						</div>														
												
					  <!-- Hide the user name filter in the narrow search ticket no: 28622 
					  <div id='search_username' class='search-opt-box' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='usernameHideShow' class='cls-show ' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showHide("usernameHideShow","paintUsername");'>
										<a class='search-heading'><?php print t("LBL054"); ?></a>
									</span>
									<span id='username-clr' class='clr-txt' style='display:none' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").clearField("Username");'><?php print t('LBL307'); ?></span>
								</li>
							</ul>
							<div id='paintUsername'>
								<div id='username_hideshow' class='srch-checkbox-container-cls' style='display:block'>
							    	<div class="filter-search-start-date-left-bg"></div>
							    	<input type='text' tabindex="1" name='srch_criteria_username' class='ac_input searchusername filter-search-start-date-middle-bg' size='15' maxlength='15'
									       id='srch_criteria_username'
									       onkeypress='$("#lnr-myteam-search").data("lnrmyteamsearch").usernameEnterKey();this.style.fontSize="13px";this.style.fontStyle="normal";'
									       data-default-text="<?php print t('LBL181'); ?>"
									       value="<?php print t('LBL181'); ?>"
									       onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_username")'
									       onfocus='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_username")'
									       onchange='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_username")'>
									<a class='username-search filter-search-start-date-search-bg' tabindex="2" title='<?php print t("LBL304"); ?>' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'>&nbsp;</a>
								<div class="filter-search-start-date-right-bg"></div>
								</div>
							</div>
						</div>
					 -->
					  <div id='search_location' class='search-opt-box <?php if(is_array($fullSearchObj->country_name_distinct) && count($fullSearchObj->country_name_distinct)<=1) {?> search-opt-box-lastitem <?php }?>' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='locationHideShow' class='cls-show  vtip' title="<?php print sanitize_data(t('LBL107')) ?>" onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").showHide("locationHideShow","paintLocation");'>
									<?php $displayTitle = titleController('EXP-TEAM-PAGE-NARROW-SEARCH-LOCATION', t("LBL107"),100); ?>
										<a class='search-heading'><?php print $displayTitle; ?></a>
									</span>
									<?php if(empty($name) || $name == t('LBL036').' '.t('LBL107') ){?>
									<span id='location-clr' class='clr-txt' style='display:none' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").clearField("Location");'><?php print t("LBL307"); ?></span>
								<?php }else{?>
								    	<span id='location-clr' class='clr-txt' style='display:block' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").clearField("Location");'><?php print t("LBL307"); ?></span>
								    
								<?php }?>
								</li>
							</ul>
							<div id='paintLocation'>
							 <div id='location_hideshow' class='srch-checkbox-container-cls' style='display:block'>
									<div class="filter-search-start-date-left-bg"></div>
									<input type='text' name='srch_criteria_location' class='ac_input searchlocation filter-search-start-date-middle-bg' size='23' maxlength='23'
									       id='srch_criteria_location'
									       onkeypress='$("#lnr-myteam-approval").data("lnrmyteamapproval").locationEnterKey();this.style.fontSize="13px";this.style.fontStyle="normal";'
										     data-default-text="<?php print $name; ?>"
									       value="<?php print $name; ?>"
									       onblur='$("#lnr-myteam-approval").data("lnrmyteamapproval").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'
									       onfocus='$("#lnr-myteam-approval").data("lnrmyteamapproval").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'
									       onchange='$("#lnr-myteam-approval").data("lnrmyteamapproval").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'>
									<a class='location-search filter-search-start-date-search-bg' title='<?php print t("LBL304"); ?>' onClick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction();'>&nbsp;</a>
								<div class="filter-search-start-date-right-bg"></div>
								</div>
							</div>
						</div>
						 <div id='search_startdate' class='search-opt-box' style='display:block'>
							<ul class='find-list-items'>
								<li>
									<span id='startdateHideShow' class='cls-show ' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").showHide("startdateHideShow","paintStartDate");'>
										<a class='search-heading'><?php print t("LBL042"); ?> <span class="date-range-heading"> (<?php print t("LBL043"); ?>)</span> </a>
									</span>
									<span id='date-clr' class='clr-txt' style='display:none' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").clearField("Date");'><?php print t("LBL307"); ?></span>
								</li>
							</ul>
							<div id='paintStartDate' class="catalog-criteria-filter-set">
								  <div style="display: block;  font-size: 12px;" id="showDateRange">
    								  <div class="catalog-start-date">
    								  	<div class="filter-search-start-date-left-bg"></div>
    									  <input type="text" value="<?php echo $startdateval;?>" class="catalog-start-date-field filter-search-start-date-middle-bg "  onblur="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate1', '<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')" onfocus="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate1','<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')"onchange="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate1','<?php print addslashes(t('LBL251').':'.t("LBL112")); ?>')" name="approval_startdate1" id="approval_startdate1" readonly>
    									  <div class="filter-search-start-date-right-bg"></div>
    									  <?php if($enddate[0]!=''){?>
    									  	<script type="text/javascript">$('#approval_startdate1').css('color','#222222').css('fontSize','13px').css('fontStyle','normal');</script>
    									  <?php } ?>
    								  </div>    								 
    								  <div class="catalog-start-date">
    								  	<div class="filter-search-start-date-left-bg"></div>
    									   <input type="text" value="<?php echo $enddateval;?>" class="catalog-start-date-field filter-search-start-date-middle-bg " onblur="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate2','<?php print addslashes(t('LBL252').':'.t('LBL112')); ?>')"onfocus="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate2','<?php print addslashes(t('LBL252').':'.t("LBL112")); ?>')"onchange="$('#lnr-myteam-approval').data('lnrmyteamapproval').hightlightedText(event,'approval_startdate2','<?php print addslashes(t('LBL252').':'.t("LBL112")); ?>')" name="approval_startdate2" id="approval_startdate2" readonly>
    									 <div class="filter-search-start-date-right-bg"></div>
    									  <?php if($enddate[0]!=''){?>
    									  	<script type="text/javascript">$('#approval_startdate2').css('color','#222222').css('fontSize','13px').css('fontStyle','normal');$('#date-clr').css('display','block');</script>
    									  <?php } ?>
    									  
    								  </div>
    								  <div class="catalog-date-format">
    								  	  <div class="curved-blue-button-left"></div> 
    									  <span class="date-refersh-img" onClick='$("#lnr-myteam-approval").data("lnrmyteamapproval").dateValidationCheck();' data-filter-label='<?php print t("LBL042"); ?>' data-filter-name='date-range' data-filter-id='date-range'><?php print t("LBL669"); ?></span>
    									  <div class="curved-blue-button-right"></div>
    								  </div> 
								  </div>
							</div>
						</div>
					<?php if(is_array($fullSearchObj->country_name_distinct) && count($fullSearchObj->country_name_distinct)>1){ ?>
					<div id='search_country' class='search-opt-box search-opt-box-lastitem'>
							<ul class='find-list-items'>
								<li>
									<span id='countryHideShow' class='cls-show ' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").showHide("countryHideShow","paintCountry");'>
										<a class='search-heading'><?php print t("LBL039"); ?></a>
									</span>
									<!-- <span class="reset-img" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").resetCheckbox("Country");'>Reset</span> -->
								</li>
							</ul>
							<div id='paintCountry'>
								  <!--<div class='srch-checkbox-container-cls'>
      								  <label for='lrn_srch_country_all' class='checkbox-selected' ><input id='lrn_srch_country_all' class='country-all srch-checkbox-cls' type='checkbox'  value='All' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'/></label>
      								  <label for='lrn_srch_country_all' class='srch-label-cls'>All</label>
								  </div>-->
								<?php
								 $count = 0;
								 foreach ($fullSearchObj->country_name_distinct as $id => $value) { 
								 if(!empty($id)) {
								   ?>
									<?php $count = $count+1; ?>
									<div id='country_hideshow_<?php print $count; ?>' class='srch-checkbox-container-cls' >
											<label for='lrn_srch_country_<?php print $id; ?>' class='checkbox-unselected'><input id='lrn_srch_country_<?php print $id; ?>'  class='country-others srch-checkbox-cls' type='checkbox'  value='<?php print $id; ?>' onClick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction();'/></label>
											<label for='lrn_srch_country_<?php print $id; ?>' class='srch-label-cls highlight-light-blue vtip' title='<?php print t($value);?>'><?php print titleController('TEAM-COUNTRY-NAME',t($value),20); ?></label>
									</div>
								<?php }
								  } ?>
								

							</div>
						</div>
					<?php }?>
					
						</div>
					</div>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<script>
$("#lnr-myteam-approval").data("lnrmyteamapproval").paintAfterReady();
$("#lnr-myteam-approval").data("lnrmyteamapproval").paintUsrNameAutocomplete();
//$("#lnr-myteam-search").data("lnrmyteamsearch").paintUsernameAutocomplete();
</script>