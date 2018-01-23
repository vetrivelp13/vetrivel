<?php
/**
 * @file
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $title: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: An ID for the block, unique within each module.
 * - $block->region: The block region embedding the current block.
 * - $edit_links: A list of contextual links for the block. It can be
 *   manipulated through the variable $edit_links_array from preprocess
 *   functions.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - block: The current template type, i.e., "theming hook".
 *   - block-[module]: The module generating the block. For example, the user
 *     module is responsible for handling the default user navigation block. In
 *     that case the class would be "block-user".
 *   - first: The first block in the region.
 *   - last: The last block in the region.
 *   - region-count-[x]: The position of the block in the list of blocks in the
 *     current region.
 *   - region-odd: An odd-numbered block of the list of blocks in the current
 *     region.
 *   - region-even: An even-numbered block of the list of blocks in the current
 *     region.
 *   - count-[x]: The position of the block in the list of blocks on the current
 *     page.
 *   - odd: An odd-numbered block of the list of blocks on the current page.
 *   - even: An even-numbered block of the list of blocks on the current page.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $edit_links_array: An array of contextual links for the block.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $block_html_id: A valid HTML ID and guaranteed unique.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see template_preprocess_block()
 * @see zen_preprocess_block()
 * @see zen_process()
 */
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
	<div class="content">
		<div id='lnr-catalog-search'>

			<div id="paintCriteriaResults" class="searchcriteria-div">
    			<!-- Header-->
    			<div style="clear: both;" class="block-findtraining-left">
    				<div class="block-findtraining-right">
    					<div class="block-findtraining-middle">&nbsp;</div>
    				</div>
    			</div>
    			<div id="search-filter-content"></div>			
    			<div style="clear: both;" class="block-footer-left">
    				<div class="block-footer-right">
    					<div class="block-footer-middle">&nbsp;</div>
    				</div>
    			</div>
			</div>

			<div id="paintContent" style="width:80.5%;float:right;">
				<!-- header -->
  				<div style="clear: both;" class="block-findtraining-left">
      				<div class="block-findtraining-right">
      					<div class="block-findtraining-middle">&nbsp;</div>
      				</div>
  				</div>
				<div class="region-sidebar-widget-bg">
					<?php
					    global $language;
					    $qtipIdInit        = '0_cre_sys_cat_shr';
					    if($language->language == 'ru'){
					    	$qtipOptAccessObj      = "{'entityId':0,
					    	'entityType':'cre_sys_cat_shr',
					    	'popupDispId':'multishareoption',
					    	'wid':470,
					    	'heg':'350',
					    	'postype':'topleft',
					    	'poslwid':'380',
					    	'linkid':'multishare-link'}";
					    }else{
	              $qtipOptAccessObj      = "{'entityId':0,
	              'entityType':'cre_sys_cat_shr',
	              'popupDispId':'multishareoption',
	              'wid':470,
	              'heg':'350',
	              'postype':'topleft',
	              'poslwid':'405',
	              'linkid':'multishare-link'}";
					    }
              ?>
             
					<div class="find-trng-sort" id='find-trng-sort-display' style="display:none;">						
                        <div class="find-trng-sortby">
                        
                        	<span class="find-trng-sort-text"><?php print t("LBL011"); ?>:</span>
                        	<!--<span class="find-trng-sort-dropdown">
                        	<select id='sortByOption' onChange='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction()'>
                        	<option value="AZ"><?php print t("Title: A to Z"); ?></option>
                        	<option value="ZA"><?php print t("Title: Z to A"); ?></option>
                        	<option value="Time"><?php print t("Newly Listed"); ?></option>
                        	<option value="ClassStartDate"><?php print t("Class Start Date"); ?></option>
                        	</select>
                        	</span>-->
                        	<ul class="sort-by-links">
	                           	<li class="first"><a class='type1' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction("AZ","type1");'><?php print t("LBL017"); ?><span class="enroll-pipeline">|</span></a></li>
                            	<li><a class='type2' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction("ZA","type2");'><?php print t("LBL018"); ?><span class="enroll-pipeline">|</span></a></li>
                            	<li><a class='type3' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction("Time","type3");'><?php print t("LBL044"); ?><span class="enroll-pipeline">|</span></a></li>
                            	<li><a class='type4' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction("ClassStartDate","type4");'><?php print t("LBL045"); ?><?php if($user->uid){ ?><span class="enroll-pipeline">|</span><?php } ?></a></li>
                            	<?php
                            	  if($user->uid){ 
                            	?>
                            	  <li><a class='type5' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction("Mandatory","type5");'><?php print strtolower(t("Mandatory")); ?></a></li>
                            	<?php } ?>
 							</ul>
 							<?php global $user; global $theme_key;
                           if($user->uid){ 
                       ?>
                			  <div id="multishareoption">
                						<a href="javascript:void(0);" id="multishare-link" class="multishare-link vtip" title="<?php print t('Share'); ?>" onclick="$('body').data('refercourse').getEmbedReferDetails('lnr-catalog-search','multiple',<?php print $qtipOptAccessObj?>,'Popup');"><?php print t('Share'); ?></a>
                						<span style=" position:absolute; left:0px; top:0px;" class="qtip-popup-visible" id="visible-popup-0"></span>
                				</div>
                				<?php }?>
 							<div class="clearBoth"></div>
                        </div>
					</div>
					<div class="clearBoth"></div>					
					<div id="no-records" style="display: none"></div>
					<div id="searchRecordsPaint" class="search-record-paint"><table id="paintContentResults"></table></div> <!-- Datatable Results -->
					<div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
				</div> <!-- End #region-sidebar-widget-bg -->
				<!-- footer -->
				<div style="clear: both;" class="block-footer-left">
					<div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					</div>
				</div>

			</div><!-- End #paintContent -->

			<div style="width:100%;clear:both;">  </div>

		</div><!-- End #lnr-catalog-search -->

	</div>	 <!-- End div Class - 'Content' -->
</div><!-- /.block -->