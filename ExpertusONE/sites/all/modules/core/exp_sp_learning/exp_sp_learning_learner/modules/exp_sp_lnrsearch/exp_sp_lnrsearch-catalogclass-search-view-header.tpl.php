				<!-- header --><?php expDebug::dPrint('values for testing header');?>
					  <div id='search-cat-title-keyword' class='search-cat-keyword' style='display:block'>
        					<span id='search_delivery_type'>
                                <input type="hidden" id='myteam-cat-delivery-type-hidden' value="<?php print t('LBL428');?>" />
        						<span id='myteam-cat-delivery-type' class='myteam-cat-delivery-type'><?php print t("LBL428"); ?></span>
                                <a id="admin-dropdown-arrow" class="myteam-cat-delivery-type-link" onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").deliverySelectClsHideShow();'>&nbsp;</a>
        							<ul id="myteam-cat-delivery-type-list">
                                    <li onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").deliveryTypeSelectClsText("<?php print str_replace('\'','\u0027',t('LBL428'));?>","<?php print str_replace('\'','\u0027',t('LBL428')); ?>","<?php print $userId;?>","<?php print $courseId;?>");' value="<?php print t('LBL428');?>"><?php print t('LBL428');?></li>
                                    <?php $delivery_types = getProfileItemNamesKeyed('lrn_cls_dty_');
        							foreach ($delivery_types as $key => $val) {
          						  ?>
        							<li onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").deliveryTypeSelectClsText("<?php print $key; ?>","<?php print t($val); ?>","<?php print $userId;?>","<?php print $courseId;?>");' class='<?php print $key; ?>'><?php print t($val); ?></li>
        						  <?php } ?>
        						</ul>
        					</span>
					  		<span id="myteam-keyword-search">
     							<span id='paintCatKeyword'>
     							<span id='catkeyword_hideshow' class='srch-checkbox-container-cls' style='display:block'>
     							 <?php global $theme_key;
				                       if($theme_key == 'expertusoneV2') {?>
				                    <div class="assign-learn-search-left"></div>
				                    <div class="select-class-search-common">
    									<input type='text' name='srch_criteria_catkeyword' class='ac_input searchcatkeyword' size='70' maxlength='70' id='srch_criteria_catkeyword' onkeypress='$("#lnr-catalog-search").data("lnrcatalogsearch").catkeywordSelectClassEnterKey("<?php print $userId;?>","<?php print $courseId;?>");this.style.fontSize="13px";this.style.fontStyle="normal";' value="<?php print t('LBL545');?>" onblur="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')" onfocus="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')" onchange="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')">
    									<a class='catkeyword-search' title='<?php print t('LBL304') ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("","","<?php print $userId;?>","<?php print $courseId;?>");'>&nbsp;</a>
    								</div>
    								    <div class="assign-learn-search-right"></div>	
    								<?php }else{?>
    								<input type='text' name='srch_criteria_catkeyword' class='ac_input searchcatkeyword' size='70' maxlength='70' id='srch_criteria_catkeyword' onkeypress='$("#lnr-catalog-search").data("lnrcatalogsearch").catkeywordSelectClassEnterKey("<?php print $userId;?>","<?php print $courseId;?>");this.style.fontSize="13px";this.style.fontStyle="normal";' value="<?php print t('LBL545');?>" onblur="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')" onfocus="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')" onchange="$('#lnr-catalog-search').data('lnrcatalogsearch').highlightedSelectClassText('srch_criteria_catkeyword','<?php print t('LBL545');?>')">
    								<a class='catkeyword-search' title='<?php print t('LBL304') ?>' onClick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("","","<?php print $userId;?>","<?php print $courseId;?>");'>&nbsp;</a>
    								<?php }?>
    								</span>
    							</span>
							</span>
							<div class="clearBoth"></div>
						</div>
				<?php 
				if($theme_key == 'expertusoneV2') {?>
				  <div class="change-class-find-trng-sort" id='myteam-find-trng-sort-display'>						
                        <div class="find-trng-sortby">
                        	<a class="find-trng-sort-text" onclick = 'sortTypeToggle("sort-by-change-links");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></a>
                        	<ul class="sort-by-change-links" style="display:none;">
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("AZ","catalog-type1","<?php print $userId;?>","<?php print $courseId;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("ZA","catalog-type2","<?php print $userId;?>","<?php print $courseId;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("Time","catalog-type3","<?php print $userId;?>","<?php print $courseId;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("ClassStartDate","catalog-type4","<?php print $userId;?>","<?php print $courseId;?>");sortTypeToggle("sort-by-change-links");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php } else { ?>
				  <div class="change-class-find-trng-sort" id='myteam-find-trng-sort-display' style="display:none;">						
                        <div class="find-trng-sortby">
                        	<span class="find-trng-sort-text"><?php print t("LBL011"); ?>:</span>
                        	<ul class="sort-by-links">
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("AZ","catalog-type1","<?php print $userId;?>","<?php print $courseId;?>");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("ZA","catalog-type2","<?php print $userId;?>","<?php print $courseId;?>");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("Time","catalog-type3","<?php print $userId;?>","<?php print $courseId;?>");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("ClassStartDate","catalog-type4","<?php print $userId;?>","<?php print $courseId;?>");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php } ?>
<script>
$("#lnr-catalog-search").data("lnrcatalogsearch").paintCatSltClskeywordAutocomplete(<?php print $userId;?>,<?php print $courseId;?>);
</script>