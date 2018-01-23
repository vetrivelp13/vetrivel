				<!-- header -->
					  <div id='search-cat-title-keyword' class='search-cat-keyword' style='display:block'>
        					<span id='search_delivery_type'>
                                <input type="hidden" id='myteam-cat-delivery-type-hidden' value="<?php print t('LBL428');?>" />
        						<span id='myteam-cat-delivery-type' class='myteam-cat-delivery-type'><?php print t("LBL428"); ?></span>
                                <a id="admin-dropdown-arrow" class="myteam-cat-delivery-type-link" onclick='$("#learningplan-tab-inner").data("learningplan").deliveryHideShow();'>&nbsp;</a>
        							<ul id="myteam-cat-delivery-type-list">
                                    <li onclick='$("#learningplan-tab-inner").data("learningplan").deliveryTypeText("<?php print str_replace('\'','\u0027',t('LBL428'));?>","<?php print str_replace('\'','\u0027',t('LBL428')); ?>","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");' value="<?php print t('LBL428');?>"><?php print t('LBL428');?></li>
                                    <?php $delivery_types = getProfileItemNamesKeyed('lrn_cls_dty_');
        							foreach ($delivery_types as $key => $val) {
          						  ?>
        							<li onclick='$("#learningplan-tab-inner").data("learningplan").deliveryTypeText("<?php print $key; ?>","<?php print t($val); ?>","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");' class='<?php print $key; ?>'><?php print t($val); ?></li>
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
    									<input style="font-size: 11px;" type='text' name='srch_criteria_catkeyword' class='ac_input searchcatkeyword' size='70' maxlength='70' id='srch_criteria_catkeyword'onkeypress='$("#learningplan-tab-inner").data("learningplan").catkeywordEnterKey("<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");this.style.fontSize="13px";this.style.fontStyle="normal";' value="<?php print t('LBL545');?>"onblur="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print addslashes(t('LBL545'));?>')"onfocus="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print addslashes(t('LBL545'));?>')"onchange="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print addslashes(t('LBL545'));?>')">
    								 <a class='catkeyword-search' title='<?php print t('LBL304') ?>' onClick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("","","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'>&nbsp;</a>
    								</div>
    								 <div class="assign-learn-search-right"></div>	
    									
    								<?php }else{?>
    								<input type='text' style="font-size: 11px;" name='srch_criteria_catkeyword' class='ac_input searchcatkeyword' size='70' maxlength='70' id='srch_criteria_catkeyword' onkeypress='$("#learningplan-tab-inner").data("learningplan").catkeywordEnterKey("<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");this.style.fontSize="13px";this.style.fontStyle="normal";' value="<?php print t('LBL545');?>"onblur="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print t('LBL545');?>')"onfocus="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print t('LBL545');?>')"onchange="$('#learningplan-tab-inner').data('learningplan').highlightedText(event, 'srch_criteria_catkeyword','<?php print t('LBL545');?>')">
    								<a class='catkeyword-search' title='<?php print t('LBL304') ?>' onClick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("","","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'>&nbsp;</a>
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
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("AZ","catalog-type1","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("ZA","catalog-type2","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("Time","catalog-type3","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>"); sortTypeToggle("sort-by-change-links");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("ClassStartDate","catalog-type4","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");sortTypeToggle("sort-by-change-links");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php } else { ?>
				  <div class="change-class-find-trng-sort" id='myteam-find-trng-sort-display' style="display:none;">						
                        <div class="find-trng-sortby">
                        	<span class="find-trng-sort-text"><?php print t("LBL011"); ?>:</span>
                        	<ul class="sort-by-links">
                            	<li class="first"><a class='catalog-type1 sortype-high-lighter' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("AZ","catalog-type1","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='catalog-type2' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("ZA","catalog-type2","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='catalog-type3' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("Time","catalog-type3","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='catalog-type4' onclick='$("#learningplan-tab-inner").data("learningplan").catalogSearchAction("ClassStartDate","catalog-type4","<?php print $userId;?>","<?php print $changeClassId;?>","<?php print $courseId;?>","<?php print $myEnrollChangeCls;?>");'><?php print t("LBL045"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>
					</div>
				<?php } ?>
<script>
$("#learningplan-tab-inner").data("learningplan").paintCatkeywordAutocomplete(<?php print $userId;?>,<?php print $changeClassId;?>,<?php print $courseId;?>,"<?php print $myEnrollChangeCls;?>"); 
</script>