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
									<span id='reportTypeHideShow' class='cls-show ' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showHide("reportTypeHideShow","paintReportType");'>
										<a class='search-heading'><?php print ucfirst(strtolower(t('REPORTS'))); ?></a>
									</span>
								</li>
							</ul>
							<div id='paintReportType'>
								<div id='report_hideshow_direct' class='srch-checkbox-container-cls' style='display:block'>
									<label for='lrn_srch_report_direct' class='checkbox-unselected' ><input id='lrn_srch_report_direct' class='report-others srch-checkbox-cls' type='checkbox' value='direct' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'/></label>
									<label for='lrn_srch_report_direct' class='srch-label-cls highlight-light-blue'><?php print  ucwords(t("SFMSG017")); ?></label>
								</div>
								<div id='report_hideshow_virtual' class='srch-checkbox-container-cls' style='display:block'>
									<label for='lrn_srch_report_virtual' class='checkbox-unselected' ><input id='lrn_srch_report_virtual' class='report-others srch-checkbox-cls' type='checkbox' value='virtual' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'/></label>
									<label for='lrn_srch_report_virtual' class='srch-label-cls highlight-light-blue'><?php print t("Virtual Report"); ?></label>
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
									<span id='locationHideShow' class='cls-show  vtip' title="<?php print sanitize_data(t('Location')) ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showHide("locationHideShow","paintLocation");'>
									<?php $displayTitle = titleController('EXP-TEAM-PAGE-NARROW-SEARCH-LOCATION', t("Location"),100); ?>
										<a class='search-heading'><?php print $displayTitle; ?></a>
									</span>
									<span id='location-clr' class='clr-txt' style='display:none' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").clearField("Location");'><?php print t("LBL307"); ?></span>
								</li>
							</ul>
							<div id='paintLocation'>
							 <div id='location_hideshow' class='srch-checkbox-container-cls' style='display:block'>
									<div class="filter-search-start-date-left-bg"></div>
									<input type='text' name='srch_criteria_location' class='ac_input searchlocation filter-search-start-date-middle-bg' size='23' maxlength='23'
									       id='srch_criteria_location'
									       onkeypress='$("#lnr-myteam-search").data("lnrmyteamsearch").locationEnterKey();this.style.fontSize="13px";this.style.fontStyle="normal";'
										     data-default-text="<?php print t("LBL114"); ?>"
									       value="<?php print t("LBL114"); ?>"
									       onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'
									       onfocus='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'
									       onchange='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_location","<?php print $language->language; ?>")'>
									<a class='location-search filter-search-start-date-search-bg' title='<?php print t("LBL304"); ?>' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'>&nbsp;</a>
								<div class="filter-search-start-date-right-bg"></div>
								</div>
							</div>
						</div>
					<?php if(is_array($fullSearchObj->country_name_distinct) && count($fullSearchObj->country_name_distinct)>1){ ?>
					<div id='search_country' class='search-opt-box search-opt-box-lastitem'>
							<ul class='find-list-items'>
								<li>
									<span id='countryHideShow' class='cls-show ' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showHide("countryHideShow","paintCountry");'>
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
											<label for='lrn_srch_country_<?php print $id; ?>' class='checkbox-unselected'><input id='lrn_srch_country_<?php print $id; ?>'  class='country-others srch-checkbox-cls' type='checkbox'  value='<?php print $id; ?>' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();'/></label>
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
$("#lnr-myteam-search").data("lnrmyteamsearch").paintAfterReady();
$("#lnr-myteam-search").data("lnrmyteamsearch").paintLocationAutocomplete();
$("#lnr-myteam-search").data("lnrmyteamsearch").paintUsernameAutocomplete();
</script>