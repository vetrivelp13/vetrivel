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
	<div class="forum-content">
		<div id='forum-list-display'>
			   <div id="frm-paint-content">
				<!-- header -->
				<div class="block-title-left">
    				<div class="block-title-right">
      					<div class="block-title-middle">
                			<h2 class="block-title"><?php print t('DISCUSSIONS');?></h2>
                			<div class="frm-list-sort" id='discuss-frm-sort-display' style="">	
                			<div class="frm-list-sortby">
                        
                        	<span class="frm-list-sort-text"onclick = 'sortTypeToggle("sort-by-links");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></span>

                        	<ul class="sort-by-links">
                            	<li class="first sortype-high-lighter"><a class='type1' onclick='$("#forum-list-display").data("forumlistdisplay").searchAction("AZ","type1","forum"); sortTypeToggle("sort-by-links");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='type2' onclick='$("#forum-list-display").data("forumlistdisplay").searchAction("ZA","type2","forum"); sortTypeToggle("sort-by-links");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='type3' onclick='$("#forum-list-display").data("forumlistdisplay").searchAction("created","type3","forum"); sortTypeToggle("sort-by-links");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='type4 sortype-last' onclick='$("#forum-list-display").data("forumlistdisplay").searchAction("posttime","type4","forum"); sortTypeToggle("sort-by-links");'><?php print t("LBL873"); ?></a></li>
                            	<!-- <li><a class='type5' onclick='$("#forum-list-display").data("forumlistdisplay").searchAction("registration","type5","forum");'><?php //print t("LBL874"); ?></a></li> -->
 							</ul>
 							<!-- POC code - HTML editor in Qtip - hyperlink added for testing -->
 							<!-- <a style="display:none" id="editorinqtip1" class="use-ajax" title="EditorInQtip" href="/?q=ajax/editorinqtip">EditorInQtip</a> -->
 							<div class="clearBoth"></div>
                        </div>
                        </div>
                			
                	</div>
             </div>
          </div>
          <!--  <div style="clear: both;" class="block-findtraining-left">
      				<div class="block-findtraining-right">
      					<div class="block-findtraining-middle">&nbsp;</div>
      				</div>
  				</div>-->    			     			
  				
				<div class="region-sidebar-widget-bg">
					<div class="frm-list-sort" id='frm-list-sort-display'>						
                        
					</div>
					<div class="clearBoth"></div>					
					<div id="no-records" style="display: none"></div>
					<div id="forumListResultsPaint" class="forumListResultsPaintInfo search-record-paint"><table id="forumListContentResults"></table></div> <!-- Datatable Results -->
					<div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
				</div> <!-- End #region-sidebar-widget-bg -->
				
				<!-- footer -->
				<div style="clear: both;" class="block-footer-left">
					<div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					</div>
				</div>

			</div><!-- End #frm-paint-content -->

			<div style="width:100%;clear:both;">  </div>

		</div><!-- End #forum-list-display -->

	</div>	 <!-- End div Class - 'Content' -->
</div><!-- /.block -->