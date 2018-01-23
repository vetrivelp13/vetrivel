				<!-- header -->
					  <div id='search-cat-title-keyword' class='search-cat-keyword' style='display:block'>
        					<span id='search_delivery_type'>
                                <input type="hidden" id='myteam-cat-delivery-type-hidden' value="<?php print t('LBL428');?>" />
        						<span id='myteam-cat-delivery-type' class='myteam-cat-delivery-type'><?php print t("LBL428"); ?></span>
                                <a id="admin-dropdown-arrow" class="myteam-cat-delivery-type-link" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").deliveryHideShow();'>&nbsp;</a>
        							<ul id="myteam-cat-delivery-type-list">
                                    <li onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").deliveryTypeText("<?php print str_replace('\'','\u0027',t('LBL428'));?>","<?php print str_replace('\'','\u0027',t('LBL428')); ?>","<?php print $userId;?>");' value="<?php print t('LBL428');?>"><?php print t('LBL428');?></li>
                                    <?php $delivery_types = getMyteamDeliverTypes(); 
        							foreach ($delivery_types as $id => $row) {
          						  ?>
        							<li onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").deliveryTypeText("<?php print $row->c_code; ?>","<?php print str_replace('\'','\u0027',t($row->c_name)); ?>","<?php print $userId;?>");' class='<?php print $row->c_code; ?>'><?php print t($row->c_name); ?></li>
        						  <?php } ?>
        						</ul>
        					</span>
					  		<span id="myteam-keyword-search">
    							<!--  <ul class='find-list-items'>
    								<li>
    									<span id='cat-title-clr' class='clr-txt' style='display:none' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").clearCatalogField("<?php print $userId;?>");'><?php print t("LBL307"); ?></span>
    								</li>
    							</ul>-->
    							<span id='paintCatKeyword'>
    								<span id='catkeyword_hideshow' class='srch-checkbox-container-cls' style='display:block'>
     								  <?php global $theme_key;
				                       if($theme_key == 'expertusoneV2') {?>
                                        <div class="assign-learn-search-left"></div>
                                        <div class="select-class-search-common">
    									<input type='text' name='srch_criteria_catkeyword' class='ac_input searchcatkeyword'
    									       size='15'
    									       id='srch_criteria_catkeyword'
    									       onkeypress='$("#lnr-myteam-search").data("lnrmyteamsearch").catkeywordEnterKey("<?php print $userId;?>");this.style.fontSize="12px";this.style.fontStyle="normal";'
    									       data-default-text="<?php print t('LBL970') . ' ' . t('LBL983');?>"
    									       value="<?php print t('LBL970') . ' ' . t('LBL983');?>"
    									       onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'
    									       onfocus='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'
    									       onchange='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'>
    									<a class='catkeyword-search' title='search' onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("","","<?php print $userId;?>");'>&nbsp;</a>
    									</div>
    									<div class="assign-learn-search-right"></div>
    									
    									<?php }else{?>
    									 <input type='text' name='srch_criteria_catkeyword' class='ac_input searchcatkeyword'
    									       size='15'
    									       id='srch_criteria_catkeyword'
    									       onkeypress='$("#lnr-myteam-search").data("lnrmyteamsearch").catkeywordEnterKey("<?php print $userId;?>");this.style.fontSize="13px";this.style.fontStyle="normal";'
    									       data-default-text="<?php print t('LBL545');?>"
    									       value="<?php print t('LBL545');?>"
    									       onblur='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'
    									       onfocus='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'
    									       onchange='$("#lnr-myteam-search").data("lnrmyteamsearch").highlightedText("srch_criteria_catkeyword")'>
    									<a class='catkeyword-search' title=<?php print t('LBL304');?> onClick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("","","<?php print $userId;?>");'>&nbsp;</a>
    								<?php }?>
    								</span>
    							</span>
							</span>
							<div class="clearBoth"></div>
						</div>
				<?php global $theme_key;
				    if($theme_key == 'expertusoneV2') {?>
					<div class="myteam-catalog-find-trng-sort" id='myteam-find-trng-sort-display' style="display:none;">						
                        <div class="find-trng-sortby">
                        	<span class="find-trng-sort-text" onclick = 'sortTypeToggle("teamSortShow");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></span>
                        	<ul class="sort-by-links teamSortShow" style="display:none;">
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("AZ","catalog-type1","<?php print $userId;?>"); sortTypeToggle("sort-by-links");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("ZA","catalog-type2","<?php print $userId;?>"); sortTypeToggle("sort-by-links");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("Time","catalog-type3","<?php print $userId;?>"); sortTypeToggle("sort-by-links");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("ClassStartDate","catalog-type4","<?php print $userId;?>"); sortTypeToggle("sort-by-links");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php }else{?>
					<div class="myteam-catalog-find-trng-sort" id='myteam-find-trng-sort-display' style="display:none;">						
                        <div class="find-trng-sortby">
                        	<span class="find-trng-sort-text"><?php print t("LBL011"); ?>:</span>
                        	<ul class="sort-by-links">
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("AZ","catalog-type1","<?php print $userId;?>");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("ZA","catalog-type2","<?php print $userId;?>");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("Time","catalog-type3","<?php print $userId;?>");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("ClassStartDate","catalog-type4","<?php print $userId;?>");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php }?>
<script>
$("#lnr-myteam-search").data("lnrmyteamsearch").paintCatkeywordAutocomplete(<?php print $userId;?>);
</script>