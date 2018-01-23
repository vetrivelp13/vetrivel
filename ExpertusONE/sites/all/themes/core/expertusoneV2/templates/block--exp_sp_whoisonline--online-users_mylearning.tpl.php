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
$titleTrans = strtoupper(t('LBL197'));
$refreshInterval = variable_get('exp_sp_whoisonline_block_refresh_interval', 60);
$listCount = variable_get('exp_sp_whoisonline_block_max_list_count', 8);
expDebug::dPrint(' $block fgfdhgf sderwwerwrw=');
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> mylearning-whoisonline-customization">
	<div class="content">
	  	<div class="region-sidebar-widget-bg">
	    <?php print $content; ?>
	    </div>
	 </div>
	 <div id="whoisonline-block_new" >
		 <div id = "whoisonline-block_start" class = "whoisonline-block">
			 <div id = "expertus-online-users">
			 <div class="expertus-online-users-loader">
			 <div id="expertus-no-online-users-msg" class="expertus-no-online-users-msg" style="display:none;">
			      <?php print t('MSG253');?>
			 </div>
			 <input id="exp_sp_whoisonline_block_refresh_interval" type="hidden" value="<?php print $refreshInterval;?>"/>
				<input id="exp_sp_whoisonline_block_max_list_count" type="hidden" value="<?php print $listCount;?>" />
			  <div id="expertus-online-users-list">
		      <table id ="expertus-online-users-jqgrid"></table>
		      <div id="expertus-online-datatable-pager" class="expertus-online-datatable" style="display:block;"></div>
		      </div>
			 </div>
			 </div>
		 </div>
	 </div>
</div>
