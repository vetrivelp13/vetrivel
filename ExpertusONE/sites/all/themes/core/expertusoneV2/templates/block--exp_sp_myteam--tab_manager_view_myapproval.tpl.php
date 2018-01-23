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
	<div id="paintCriteriaLinkResults" class="searchlinkcriteria-div">
    			<!-- Header-->
    			<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
                			<h2 class="block-title"><?php print t('TEAM');?></h2>
              			</div>
    				</div>
  				</div>
    			<div id="search-link-content" style="
    border-left: 1px solid #e7e7e7;
    border-right: 1px solid #e7e7e7;
"></div>			
    			<div style="clear: both;" class="block-footer-left">
    				<div class="block-footer-right">
    					<div class="block-footer-middle">&nbsp;</div>
    				</div>
    			</div>
    			
    			<!-- Header-->
    			<div class="block">
    			<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
                			<h2 class="block-title"><?php print t('REFINE');?></h2>
              			</div>
    				</div>
  				</div>
    			<div id="search-filter-content"></div>			
    			<div style="clear: both;" class="block-footer-left">
    				<div class="block-footer-right">
    					<div class="block-footer-middle">&nbsp;</div>
    				</div>
    			</div>
    			</div>
    			
			</div>
	<div class="content">
		<div id='lnr-myteam-approval'>

			<div id="paintContent" style="width:77.3%;float:right;">
				<!-- header -->
  				<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
                			<h2 class="block-title"><?php print t('MyApproval');?>
                				<div class="find-trng-sort" id="find-trng-sort-display" style="display:none;" >							
                        			<div class="find-trng-sortby">
                        			<span class="find-trng-sort-text" onclick = 'sortTypeToggle("sort-by-links");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></span>
                        	<!--<span class="find-trng-sort-dropdown">
                        	<select id='sortByOption' onChange='$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction()'>
                        	<option value="AZ"><?php print t("Title: A to Z"); ?></option>
                        	<option value="ZA"><?php print t("Title: Z to A"); ?></option>
                        	<option value="Time"><?php print t("Newly Listed"); ?></option>
                        	</select>
                        	</span>-->
                        			<ul class="sort-by-links">
                            			<li class="first"><a class='type1' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("AZ","type1"); sortTypeToggle("sort-by-links");'><?php print t("LBL017"); ?></a></li>
                            			<li><a class='type2' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("ZA","type2"); sortTypeToggle("sort-by-links");'><?php print t("LBL018"); ?></a></li>
                            			<li><a class='type3' onclick='$("#lnr-myteam-approval").data("lnrmyteamapproval").searchAction("Time","type3"); sortTypeToggle("sort-by-links");'><?php print t("LBL044"); ?></a></li>
 									</ul>
 									<div class="clearBoth"></div>
                     				</div>
								</div>
                			</h2>
              			</div>
    				</div>
  				</div>
				<div class="region-sidebar-widget-bg">
					<div class="clearBoth"></div>					
					<div id="searchRecordsPaint" class="search-record-paint"><div id="no-records" style="display: none"></div><table id="paintContentResults"></table><table id="paintClassContentResults"></table></div> <!-- Datatable Results -->
					<div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
				</div> <!-- End #region-sidebar-widget-bg -->
				<!-- footer -->
				<div style="clear: both;" class="block-footer-left">
					<div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					</div>
				</div>

			</div><!-- End #paintContent -->


		</div><!-- End #lnr-myteam-search -->

	</div>	 <!-- End div Class - 'Content' -->
</div><!-- /.block -->