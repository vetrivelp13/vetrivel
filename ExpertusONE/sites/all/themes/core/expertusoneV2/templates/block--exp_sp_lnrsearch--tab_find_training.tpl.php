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
 
global $theme_key;
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
	<div class="content">
		<div id='lnr-catalog-search'>
			<div id="block-take-survey"></div>
			<div id="paintCriteriaResults" class="searchcriteria-div <?php print $variables['block']->criteria_class;?>">
    			<!-- Header-->
    			<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
                			<h2 class="block-title"><?php print t('REFINE');?></h2>
                			<span id="refine-filter-pin-icon" class="pin-icon-white
                			" onclick="$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();"></span>
                			<span class="catalog-criteria-refine-icon criteria-refine-icon refine-icon-expanded"><?php print print_refine_filter_menu();?></span>
              			</div>
    				</div>
  				</div>
    			<div id="search-filter-content-wrapper">
    				<div id="search-filter-content"></div>
	    			<div style="clear: both;" class="block-footer-left">
	    				<div class="block-footer-right">
	    					<div class="block-footer-middle">&nbsp;</div>
	    				</div>
	    			</div>
    			</div>
			</div>

			<div id="paintContent" class = "catalog-default <?php print $variables['block']->content_class;?>">
				<!-- header -->
  				<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
							<span class="catalog-criteria-refine-icon criteria-refine-icon refine-icon-collapsed"><?php print print_refine_filter_menu();?></span>
                			<h2 class="block-title block-catalog-search-title"><?php print t('CATALOG');?></h2>
							<div class="find-trng-sort" id='find-trng-sort-display' style="display:none;" >						
								<div class="find-trng-sortby">
									<a class="find-trng-sort-text" onclick = 'sortTypeToggle("sort-by-links");$(this).parents("#find-trng-sort-display").toggleClass("sort-active")'>
										<?php // print t("LBL011"); ?>
										<!--<span class="find-trng-sort-arrow-icon"></span>-->
									</a>
										<!--<span class="find-trng-sort-dropdown">
											<select id='sortByOption' onChange='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction()'>
											<option value="AZ"><?php print t("Title: A to Z"); ?></option>
											<option value="ZA"><?php print t("Title: Z to A"); ?></option>
											<option value="Time"><?php print t("Newly Listed"); ?></option>
											<option value="ClassStartDate"><?php print t("Class Start Date"); ?></option>
											</select>
										</span>-->
									<ul class="sort-by-links">
										<li class="first"><a class='type1' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this,"AZ","type1"); sortTypeToggle("sort-by-links");'><?php print t("LBL017"); ?></a></li>
										<li><a class='type2' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this,"ZA","type2"); sortTypeToggle("sort-by-links");'><?php print t("LBL018"); ?></a></li>
										<li><a class='type3' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this,"Time","type3"); sortTypeToggle("sort-by-links");'><?php print t("LBL044"); ?></a></li>
										<li><a class='type4' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this,"ClassStartDate","type4"); sortTypeToggle("sort-by-links");'><?php print t("LBL045"); ?></a></li>
										<?php
										  if($user->uid){ 
										?>
										<li><a class='type5' onclick='$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this,"Mandatory","type5"); sortTypeToggle("sort-by-links");'><?php print strtolower(t("Mandatory")); ?></a></li>
										<?php } ?>
									</ul>
									<div class="clearBoth"></div>
								</div>
							</div>
							<?php if($user->uid) {
								//changed heg from 350 to 300 to fix for ticket:0076219
									$qtipIdInit = '0_cre_sys_cat_shr';
									$qtipOptAccessObj = "{'entityId':0,
													  'entityType':'cre_sys_cat_shr',
													  'popupDispId':'multishareoption',
													  'wid':470,
													  'heg':300,
													  'postype':'topright',
													  'linkid':'multishare-link',
													  'callFrom': 'catalog-multishare'
													  }"; 													  
							$sharemodule=getShareModuleStatus('catalog');
							if($sharemodule){	  
						 ?>
								<div id="multishareoption">
									<a href="javascript:void(0);" id="multishare-link" class="multishare-link" onclick="$('body').data('refercourse').getEmbedReferDetails('lnr-catalog-search','multiple',<?php print $qtipOptAccessObj?>,'Popup');">
										<span class="share-tab-icon-white vtip" title="<?php print t('Share'); ?>"></span>
									</a>
									<span style=" position:absolute; left:0px; top:0px;" class="qtip-popup-visible catalog-multishare-qtip-popup-visible" id="visible-popup-0"></span>
								</div>
							<?php } } ?>
              		</div>
    				</div>
  				</div>
  				
				<div class="region-sidebar-widget-bg">
					<div class="clearBoth"></div>					
					<div id="no-records" style="display: none"></div>
					<div id="searchRecordsPaint" class="search-record-paint"><table id="paintContentResults"></table></div> <!-- Datatable Results -->
					<div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
				</div> <!-- End #region-sidebar-widget-bg -->
				<!-- footer -->
				<div class="show-more-wrapper">
					<span id="paintCatalog-show_more" class="show-more-handler"><?php print t("SHOW MORE"); ?></span>
				</div>
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