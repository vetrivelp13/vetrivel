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
/*$profileitems = getGradeProfileItems();
foreach ($profileitems as $profile_records) {
  $grade .= "<option value=\"$profile_records->code\">$profile_records->name</option>"; 
}*/
global $salesforce_type, $last_left_panel;
?>
<li id="tab_instructor_desk" class="sortable-item">
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">

  <div class="content">
    <?php print $content; ?>
    <div id="block-take-survey"></div>
  </div>
  <div id='learner-instructor'>
  	<div class='block-title-left'>
  		<div class='block-title-right'>
  			<div class='block-title-middle'>
    			<h2 class='block-title'><?php print t('LBL103'); ?>
    				<div class="delete-widget-disabled flt_right" <?php if($salesforce_type == 'iframe' || $last_left_panel == false) {?> style="display:none" <?php } ?>>|&nbsp;<a href="javascript:void(0);" class="widget-delete-button inactiveLink">X</a></div>
    				<div class="delete-widget flt_right" <?php if ($salesforce_type == 'iframe' || $last_left_panel) { ?> style="display:none" <?php }?> >|&nbsp;<a href='javascript:void(0);' onclick='$("body").data("learningcore").displayDeleteWizardLearner("<?php print t("MSG703").' <span class=\"del_panel\">'.strtoupper(t('LBL103')).'</span> '. strtolower(t('Panel'));?>", "tab_instructor_desk", "highlight-list")' class='widget-delete-button'>X</a></div>
    				<div class="instructor-sort-by-container">
    				 <div class="enroll-sort-bylist">
    					<div class="sort-by-text" id="sort-by-instructor" onclick = 'sortTypeToggle("learnerinstructorshow");$(".enrollmentshow").hide();$(".learningplanshow").hide();'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></div>
        				<ul class="sort-by-links learnerinstructorshow" style="display:none;">
        					<li class="first"><a class="type1 sortype-high-lighter" onclick='$(".learnerinstructorshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#instructor-tab-inner").data("instructordesk").instructorSortForm("AZ","type1"); sortTypeToggle("sort-by-links");' href='javascript:void(0);'><?php print t("LBL017"); ?></a></li>
        					<li><a class="type2" onclick='$(".learnerinstructorshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#instructor-tab-inner").data("instructordesk").instructorSortForm("ZA","type2"); sortTypeToggle("sort-by-links");' href='javascript:void(0);'><?php print t("LBL018"); ?></a></li>
        					<li><a class="type3" onclick='$(".learnerinstructorshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#instructor-tab-inner").data("instructordesk").instructorSortForm("dateOld","type3"); sortTypeToggle("sort-by-links");' href='javascript:void(0);'><?php print t("LBL019"); ?></a></li>
        					<li><a class="type4" onclick='$(".learnerinstructorshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#instructor-tab-inner").data("instructordesk").instructorSortForm("dateNew","type4"); sortTypeToggle("sort-by-links");' href='javascript:void(0);'><?php print t("LBL020"); ?></a></li>
        					<li><a class="type5" onclick='$(".learnerinstructorshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#instructor-tab-inner").data("instructordesk").instructorSortForm("type","type5"); sortTypeToggle("sort-by-links");' href='javascript:void(0);'><?php print strtolower(t("LBL036")); ?></a></li>
        				</ul>
    					<!--</span>-->
    					</div>
    					</div>
    			</h2>		
			</div>
		</div>
	</div></div>
    <div class='clearBoth'></div>
    
  <div id="instructor-result-container" <?php global $mylearning_right; if($mylearning_right===false) print 'class="clsinstructor-result-container"'; ?>>
  <div id='instructor-maincontent_tab'>
    		<ul class='EnrolltabNavigation-Ins'>
    			<li class='selected'><a id='scheduled_tab' href='javascript:void(0);' onclick='$(".EnrolltabNavigation-Ins li").removeClass("selected");$(this).parents("li").addClass("selected");$("#instructor-tab-inner").data("instructordesk").callInstructor("scheduled")'><span><span><?php print t("Scheduled"); ?></span></span></a></li>
    			<li><a id='delivered_tab' href='javascript:void(0);' onclick='$(".EnrolltabNavigation-Ins li").removeClass("selected");$(this).parents("li").addClass("selected");$("#instructor-tab-inner").data("instructordesk").callInstructor("delivered")'><span><span><?php print t("Completed"); ?></span></span></a></li>
    		</ul>
	</div>
    <table id ="paintInstructorResults"></table>
    <div id="instructor-noresult-msg"><?php print t('ERR073') ?>.</div>
  </div>
  <div id="pager-ins" style="display:none;" ></div>
  <!--<div style="display:none;" id="mark-complete-grade"><?php echo $grade; ?></div>-->
  <div style="display:none;" id="mark-complete-date"><?php echo date('m-d-Y'); ?></div>
  <div class="enroll-bottom-curve <?php if($mylearning_right===false) print ' clsRightNoData'; ?>"> 
	<div class="block-footer-left">
		<div class="block-footer-right">
			<div class="block-footer-middle">&nbsp;</div>
		</div>
	 </div>
	</div>
</div><!-- /.block -->
</li>