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
<li id="tab_my_enrollment_customized" class="sortable-item">
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
    <?php 
    $menustatic = array('Enrollmentpart'=>'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att','EnrollCompleted'=>'lrn_crs_cmp_cmp','EnrollCompletedWithAttempts'=>'lrn_crs_cmp_cmp_atl',
	'EnrollCanceled'=>'lrn_crs_reg_can','EnrollReregister'=>'lrn_crs_cmp_cmp_rereg','EnrollPayments'=>'lrn_crs_reg_ppm|lrn_crs_reg_wtl'
	);
drupal_add_js(array('enrollmentstatus' => $menustatic), 'setting');
 $class = ''; 
  $sltPersonUserId = getSltpersonUserId();
  $getmandatory = getmandatory($sltPersonUserId,'cre_sys_obt_cls');
  $getLPmandatory = getLPmandatory($sltPersonUserId,'cre_sys_obt_cls');
  $isCompliance = isCompliance($sltPersonUserId);
  $resMenuFilter = constructMyenrollmentCustomizedTab('mylearning','myenrollment');

  expDebug::dPrint("matched element test ccokiue test tttttttttttt".$_COOKIE['enrollment_selected_menu'],5);

  expDebug::dPrint("Values for menu and filter".print_r($resMenuFilter['menu'],1));
  $class = 'sortype-high-lighter';
?>
  <div class="content">
    <?php print $content; ?>
    <div id="block-take-survey"></div>
  </div>
  <div id='learner-myenrollment'>
  <form name="myenrollment-filters" id="myenrollment-filters">
  	<div class='block-title-left'>
  			<div class='block-title-middle'>
    			<h2 class='block-title'>
    			<div data-highlightClass="tour-common tour-filter" id='myenrollment-learner-menu' class='learner-menu active_block' >
					<span class='sort-by-text icon-filter-hover' id='myenrollment-sort-by-text-icon' data1='drop-menu-fltr_myenrollment'>
						<?php print print_refine_filter_menu(true);?>
					</span>
					<div id='drop-menu-fltr_myenrollment' class='drop-menu-fltr filter-block icon-filter-menu' style='display:none;'>
						<?php print $resMenuFilter['menu'];?>
						<a class='apply-btn' onclick="statusFilterSubmit('myenrollment')" href='javascript:void(0);'><?php print t('LBL725');?></a>
					</div>
    			</div>
		  <span class="mylearnmodule-title"><?php print t('LBL3090'); ?> </span>
    	<div class="block-tilte-filter-right-container">	
    	 	<div data-highlightClass="tour-common tour-sort" class="mylearning_tour tour-filter" id="mylearning_tour">	
    	<div class="text-search-filter-container active_block" >
    			<div id="myenrollment-text-search-filter" class="text-search-filter-icon icon-filter" data1="myenrollment-text-filter-box" ></div>
    			<div id='myenrollment-text-filter-box' class='text-filter-box-class filter-block' style='display:none;'>
    			<?php print $resMenuFilter['textsearch'];?>
			</div>
		</div>
		<div class="learner-filter-container active_block">
    			<div id="myenrollment-learner-filter" class='learning-filter-icon icon-filter' data1='myenrollment-mylearning-filter-by'> </div>
    				<div id='myenrollment-mylearning-filter-by' class='mylearning-filter-by filter-block' style='display:none;'>
    				<?php print $resMenuFilter['filter'];?>
	    				<div class='filter-actions'>
	    				<input type='button' class='close-btn'  value=<?php print t('LBL123');?> name="close">
	    				<input type='button' class='apply-btn' onclick="statusFilterSubmit('myenrollment')" value=<?php print t('LBL725');?> name="apply">
	    				</div>
    				</div>
		 </div>
		  </div>
		  <div data-highlightClass="tour-common tour-sort" class="mylearning_sort tour-sort" id="mylearning_sort">	
		  <div class="learner-sort active_block">
    			<div class="enroll-sort-by-container">
    				  <div class="enroll-sort-bylist ">
    					<div class="sort-by-text icon-filter" id="sort-by-enroll" data1="sortby_enrollment"><?php print t("LBL011"); ?></div>
        					<ul class="sort-by-links enrollmentshow filter-block" id='sortby_enrollment' style='display:none;'>  
        					<?php if($getmandatory > 0 || $isCompliance > 0  || $getLPmandatory > 0){ ?>
        						<li><a class="type7" onclick='statusFilterSubmit("myenrollment","Mandatory","type7");' href='javascript:void(0);'><?php print strtolower(t("Mandatory")); ?></a></li>
        						<?php } ?>      					
        						<li class="first"><a class="type1 sortype-high-lighter" onclick='statusFilterSubmit("myenrollment","AZ","type1");' href='javascript:void(0);'><?php print t("LBL017"); ?></a></li>        						
        						<li><a class="type2" onclick='statusFilterSubmit("myenrollment","ZA","type2");' href='javascript:void(0);'><?php print t("LBL018"); ?></a></li>
        						<li><a class="type4" onclick='statusFilterSubmit("myenrollment","dateNew","type4");' href='javascript:void(0);'><?php print t("LBL020"); ?></a></li>        						
								<li><a class="type5" onclick='statusFilterSubmit("myenrollment","dateOld","type5");' href='javascript:void(0);'><?php print t("LBL019"); ?></a></li> 
								<li><a class="type3" onclick='statusFilterSubmit("myenrollment","type","type3");' href='javascript:void(0);'><?php print strtolower(t("LBL036")); ?></a></li>
							    <?php if($getmandatory == 0 && $isCompliance == 0 && $getLPmandatory == 0){ ?>
        						<li><a class="type7" onclick='$(".enrollmentshow li a").removeClass("sortype-high-lighter");$(this).addClass("sortype-high-lighter");$("#learner-enrollment-tab-inner").data("enrollment").enrollSortForm("Mandatory","type6"); sortTypeToggle("enrollmentshow");' href='javascript:void(0);'><?php print strtolower(t("Mandatory")); ?></a></li>
        						<?php } ?>  
								 <li><a class="type8" onclick='statusFilterSubmit("myenrollment","startdate","type8");' href='javascript:void(0);'><?php print strtolower(t("LBL045")); ?></a></li> 
								 
    				</ul>
    					</div>
    					</div>
		  
		  </div>
		 </div>
		  <div class="delete-widget-disabled flt_right" <?php if($salesforce_type == 'iframe' || $last_left_panel == false) {?> style="display:none" <?php } ?>><a href="javascript:void(0);" class="widget-delete-button inactiveLink">X</a></div>
		  <div class="delete-widget flt_right" <?php if ($salesforce_type == 'iframe' || $last_left_panel) { ?> style="display:none" <?php }?> ><!-- |&nbsp; --><a href='javascript:void(0);' onclick='$("body").data("learningcore").displayDeleteWizardLearner("<?php print t("MSG703").' <span class=\"del_panel\">'.strtoupper(t('LBL3090')) .'</span> '.strtolower(t('Panel'));?>", "tab_my_enrollment_customized", "highlight-list")' class='widget-delete-button'>X</a></div>
		  </div>
    					</h2>	
			</div>
		</div>
  </form>
</div>
    <div class='clearBoth'></div>
    
  <div id="enroll-result-container"  <?php global $mylearning_right; if($mylearning_right===false) print 'class="clsenroll-result-container"'; ?>>
  	 <div id="myenrollment-searchFilter" class="searchFilter">  </div> 
     <table id ="paintEnrollmentResults"></table>
   <!--  <div id = "cp_seemorecontent" class='cp_display-less' onClick='$("#learner-enrollment-tab-inner").data("contentPlayer").playContentMylearning();'><?php print t("LBL543"); ?></div>-->
  <div id = "cp_content_display" class='cp_display_more' style="display:none;"> </div>
    <div id="enroll-noresult-msg" class="mylearn-no-result-msg"><?php print t('MSG867') ?> <!-- <span id="enrollment-state"><?php// print strtolower(t('Enrolled')); ?></span> <?php //print strtolower(t('LBL152')); ?>--></div>
     <div id="enroll-nosearchresult-msg" class="mylearn-no-result-msg"><?php print t('MSG381') ?> </div>
  </div>
  <!--  <div id="pager" style="display:none;"></div>-->
  
  <div class="enroll-bottom-curve <?php if($mylearning_right===false) print ' clsRightNoData'; ?>">
    <div class="show-more-wrapper show-more">
		<span id="paintEnrollmentResults-show_more" class="show-more-handler"><?php print t("SHOW MORE"); ?></span>
	</div>
	<div class="block-footer-left">
		<div class="block-footer-right">
			<div class="block-footer-middle">&nbsp;</div>
		</div>
	 </div>
	</div>
</div><!-- /.block -->
</li>



