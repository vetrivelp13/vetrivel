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
global $user;
$result = $block->results['details'];
expDebug::dPrint('result info: ' . print_r($block->results, 1));
$stats_block = $block->results['details']->statistics;
$class_content = $block->results['details'];
$loggedInUserID = getIdOfLoggedInUser ();

if($result->data_code){
?>

<div id="<?php print $block_html_id; ?>" 	class="<?php print $classes; ?>">
	<div id="course_detail_content" class="course-wrapper detail-wrapper ">
		<div id="lnr-catalog-search"></div>
		<div id="block-take-survey"></div>
		<div id="course-details-display-content" class="course-header-wrapper">
			<?php print theme("content_header", array("result"=>$result, 'context' => ['callfrom' => 'course_details'])); ?>
			<div class="left-section">
				<div class="course-content-wrapper border-box-crs">
						  <div class="sub-section-title-out  padbt5"><?php print t("LBL353"); ?></div>
						  <input id='courseId' type=hidden value="<?php print $block->results['details']->course_id ;?>" />
        				  <input id='courseNodeId' type=hidden value="<?php print arg(2); ?>" />
            			  <input id='userId' type=hidden value="<?php global $user; print $user->uid; ?>" />
	        				
	    				  <!--  Place holder table and div for jqgrid of classes list and pager -->
	    				  <div id="class-list-loader"></div>
	        			  <div id="class-details-displayloader"></div>
	        			  <div id="searchCourseClassListPaint" class="search-course-class-list-paint"><table id ="paint-classes-list"></table></div>
	        			  <div class="grid-show-more show-more-wrapper" style="text-align: center; border: none;">
								<span id="paintcrsclslist-show_more" class="grid-show-more-wrapper  show-more-handler"><?php print t("SHOW MORE"); ?></span>
						  </div>
				</div>
				<?php print theme('content_misc', array("result"=>$block->results['miscContent'], 'context' => ['callfrom' => 'course_details']));  ?>
			<?php if (!empty($_SESSION ['availableFunctionalities']->exp_sp_forum) && !empty( $loggedInUserID )) { ?>	
				<div class="course-dicussion-wrapper border-box">
					<?php print theme("content_disc", array("result"=>$block->results['discussions'], 'context' => ['callfrom' => 'course_details'])); ?>
				</div>
			<?php } ?>
			</div>
			<div class="right-section">
				<div class="course-stats-wrapper stats-wrapper border-box">
					<?php print theme('content_stats', array( 'result' => $stats_block, 'context' => ['callfrom' => 'course_details'])); ?>
				</div>
			</div>
		</div>
	</div>
</div>
<?php 
} else {
	print catalogNoAccessHtml($block_html_id,$classes);
//	drupal_add_js('jQuery(document).ready(function () { jQuery("#signin").click(); });', 'inline'); // #56618 - Issue Fix
}
?>