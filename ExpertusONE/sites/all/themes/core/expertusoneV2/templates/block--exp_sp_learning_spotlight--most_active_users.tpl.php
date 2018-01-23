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
<div class="<?php print $classes; ?>" id="<?php print $block_html_id; ?>">
	<div class="most-active-list"></div>
 	<div class="block-title-left">
 		<div class="block-title-right">
 			<div class="block-title-middle">
 			  <?php if ($title): ?>
 				<h2 class='block-title'><?php print $title; ?></h2>
 			  <?php endif; ?>
 			</div>
 		</div>
 	</div>
	<div class="content region-sidebar-widget-bg">
        <?php if ($content!=" "): ?>
			<div class="spotlight-message-text"><?php print $content ?></div>
		<?php endif; ?>
		<div class="item-list most-active-user">
            	<div class="spotlight-inner">
            	    <?php $num_rows = count($block->results); ?>
                	<ul>
                	<?php foreach ($block->results as $uid => $row): ?>
                		<li class='list-spotlight<?php print ($num_rows == ($uid+1))?'-last':'';?>'>
                			<div class="profile-item">
                				<a id="<?php print $row->uid; ?>" class="profile-image user-list-border-img" title="<?php print $row->full_name; ?>" class="user-profile" href="javascript:void(0);">
                    				<?php 
                                      print getUserPicture($row->uid);
                                    ?>
                    			</a>
                      		 	<div class="profile-desc">
                        			<span class="user-name vtip" title="<?php print $row->user_name;?>" ><?php print $row->user_name; ?></span>
                        			<span class="job-title vtip" title="<?php print $row->job_title;?>" ><?php print $row->job_title; ?></span>
                  				</div>
                			</div>
                		</li>
                	 <?php endforeach; ?>
                	</ul>
                </div>
        	</div>
	  </div>
	<div class="block-footer-left">
		<div class="block-footer-right">
			<div class="block-footer-middle">&nbsp;</div>
		</div>
	</div>
</div>

