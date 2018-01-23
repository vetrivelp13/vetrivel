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
 $del_type = array("lrn_cls_dty_ilt" => "ILT", "lrn_cls_dty_wbt" => "WBT", "lrn_cls_dty_vod" => "VOD", "lrn_cls_dty_vcl" => "VC");
 global $user,$language;
 if(arg(1) != 'my-profile'){
   $tooltip = t('LBL009');
   $class = "block-title vtip";
 }else{
   $tooltip = "";
   $class = "block-title";
 }

?>
<?php if (arg(1) != 'my-profile'): ?><li id="mytranscript" class="sortable-item"><?php endif; ?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div id='div_my_transcript' style='width:100%'>
    <div class='block-title-left'>
 	  <div class='block-title-right'>
 	    <div class='block-title-middle'>
 		  <?php if ($title): ?>
 		    <h2 class="<?php  print $class;?>" title="<?php  print $tooltip;?>"><?php print titleController('FADEOUT-MY-TRANSCRIPT', $title); ?>
 		    <?php if (arg(1) != 'my-profile'): ?><div class="delete-widget flt_right"><a href='javascript:void(0);' onclick='$("body").data("learningcore").displayDeleteWizardLearner("<?php print t("MSG703").' <span class=\"del_panel\">'.strtoupper($title).'</span> '. strtolower(t('Panel'));?>", "mytranscript", "sidebar_second-list")' class='widget-delete-button'>X</a></div><?php endif; ?>
 		    </h2>
 		  <?php endif; ?>
 		</div>
 	  </div>
 	</div>
		<div class="content">
	  	<div class="region-sidebar-widget-bg">
	    <?php print $content; ?>
	    </div>
	  </div>
	<div class='block-footer-left'>
	  <div class='block-footer-right'>
		<div class='block-footer-middle'>&nbsp;</div>
	  </div>
	</div>
  </div>
</div>
<?php if (arg(1) != 'my-profile'): ?></li><?php endif; ?>