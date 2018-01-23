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
global $salesforce_type, $last_left_panel;
?>
<li id="tab_my_enrollment" class="sortable-item">
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
    <?php 
 $class = ''; 
  $sltPersonUserId = getSltpersonUserId();
  $getmandatory = getmandatory($sltPersonUserId,'cre_sys_obt_cls');
  $getLPmandatory = getLPmandatory($sltPersonUserId,'cre_sys_obt_cls');
  $isCompliance = isCompliance($sltPersonUserId);
  $class = 'sortype-high-lighter';

  ?>
  <div class="content">
    <?php print $content; ?>
    <div id="block-take-survey"></div>
  </div>
  <div id='learner-enrollment'>
  	<div class='block-title-left'>
  		<div class='block-title-right'>
  			<div class='block-title-middle'>
    			<h2 class='block-title'><?php print t('LBL010'); ?>
    			<div class="delete-widget-disabled flt_right" <?php if($salesforce_type == 'iframe' || $last_left_panel == false) {?> style="display:none" <?php } ?>>|&nbsp;<a href="javascript:void(0);" class="widget-delete-button inactiveLink">X</a></div>
    			<div class="delete-widget flt_right" <?php if ($salesforce_type == 'iframe' || $last_left_panel) { ?> style="display:none" <?php }?> >|&nbsp;<a href='javascript:void(0);' onclick='$("body").data("learningcore").displayDeleteWizardLearner("<?php print t("MSG703").' <span class=\"del_panel\">'.strtoupper(t('LBL010')) .'</span> '.strtolower(t('Panel'));?>", "tab_my_enrollment", "highlight-list")' class='widget-delete-button'>X</a></div>
    			<div class="enroll-sort-by-container">
    				  <div class="enroll-sort-bylist">
    					<div class="sort-by-text" id="sort-by-enroll" onclick = 'sortTypeToggle("enrollmentshow");$(".learningplanshow").hide();$(".learnerinstructorshow").hide();'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></div>
        					<ul class="sort-by-links enrollmentshow" style="display:none;">
        					<?php if($getmandatory > 0 || $isCompliance > 0  || $getLPmandatory > 0){ ?>
        						<li><a class="type6 <?php print $class; ?>" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("Mandatory","type6"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print strtolower(t("Mandatory")); ?></a></li>
        						<?php } ?>
        						<?php if($getmandatory == 0 && $isCompliance == 0 && $getLPmandatory == 0){ ?>       						
        						<li><a class="type3 <?php print $class; ?>" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("dateOld","type3"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print t("LBL019"); ?></a></li>
        						<?php } else{?>
												<li><a class="type3 " onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("dateOld","type3"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print t("LBL019"); ?></a></li>        						
        						<?php }?>
        						<li class="first"><a class="type1" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("AZ","type1"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print t("LBL017"); ?></a></li>        						
        						<li><a class="type2" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("ZA","type2"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print t("LBL018"); ?></a></li>
        						<li><a class="type4" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("dateNew","type4"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print t("LBL020"); ?></a></li>        						
        						<li><a class="type5" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("type","type5"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print strtolower(t("LBL036")); ?></a></li>
        						<?php if($getmandatory == 0 && $isCompliance == 0 && $getLPmandatory == 0){ ?>
        						<li><a class="type6" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("Mandatory","type6"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print strtolower(t("Mandatory")); ?></a></li>
        						<?php } ?>
        						<li><a class="type7" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("startdate","type7"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print strtolower(t("LBL045")); ?></a></li>
        						
        						
    				</ul>
    					</div>
    					</div>
    					</h2>	
			</div>
		</div>
	</div></div>
    <div class='clearBoth'></div>
    
  <div id="enroll-result-container"  <?php global $mylearning_right; if($mylearning_right===false) print 'class="clsenroll-result-container"'; ?>>
  <div id='learner-maincontent_tab3'>
    		<ul class='EnrolltabNavigation'>
    			<li class='selected'><a id='Enrollmentpart' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart")'><span><span><?php print t("Enrolled"); ?></span></span></a></li>
    			<li><a id='EnrollCompleted' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_cmp","EnrollCompleted");'><span><span><?php print t("Completed"); ?></span></span></a></li>
    			<li><a id='EnrollInCompleted' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_inc|lrn_crs_cmp_nsw","EnrollInCompleted")'><span><span><?php print t("Incomplete"); ?></span></span></a></li>
    			<li><a id='EnrollExpired' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_exp","EnrollExpired");'><span><span><?php print t("Expired"); ?></span></span></a></li>
    			<li><a id='EnrollCanceled' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_can","EnrollCanceled")'><span><span><?php print t("Canceled"); ?></span></span></a></li>
    			<li class='removeMrg'><a id='EnrollPayments' href='javascript:void(0);' onclick='$(".EnrolltabNavigation li").removeClass("selected");$(this).parents("li").addClass("selected");$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_ppm|lrn_crs_reg_wtl","EnrollPayments")'><span><span><?php print t("Pending"); ?></span></span></a></li>
    		</ul>
	</div>
    
   
    
     <table id ="paintEnrollmentResults"></table>    
   <!--  <div id = "cp_seemorecontent" class='cp_display-less' onClick='$("#learner-enrollment-tab-inner").data("contentPlayer").playContentMylearning();'><?php print t("LBL543"); ?></div>-->
    <div id = "cp_content_display" class='cp_display_more' style="display:none;">
   </div>
    <div id="enroll-noresult-msg"><?php print t('MSG261') ?> <span id="enrollment-state"><?php print strtolower(t('Enrolled')); ?></span> <?php print strtolower(t('LBL152')); ?>.</div>
  </div>
  <div id="pager" style="display:none;"></div>
  
  <div class="enroll-bottom-curve <?php if($mylearning_right===false) print ' clsRightNoData'; ?>"> 
	<div class="block-footer-left">
		<div class="block-footer-right">
			<div class="block-footer-middle">&nbsp;</div>
		</div>
	 </div>
	</div>
</div><!-- /.block -->
</li>