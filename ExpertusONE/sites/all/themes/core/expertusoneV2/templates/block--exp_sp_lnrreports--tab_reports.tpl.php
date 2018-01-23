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
	<div class="content">
	<?php
	  $userId = getSltpersonUserId(); 
	  //die('userid'.$userId);
	  expDebug::dPrint('checking the content of the new reports permi'.print_r(user_access('New Report Perm'),true));
	  if(user_access('New Report Perm') || is_manager($userId) || is_instructor($userId)){ ?>
		<div id='lnr-reports-search'>
	<?php }else{ ?>
		<div id='lnr-reports-search' style='display:none;'>
	<?php } ?>
			<div id="paintLearnerCriteriaResults" class="searchcriteria-div">
    			<!-- Header-->
    			<div style="clear: both;" class="block-findtraining-left">
    				<div class="block-findtraining-right">
    					<div class="block-findtraining-middle srch-filter-heading"><?php print t('REFINE'); ?></div>
    				</div>
    			</div>
    			<div id="search-filter-content"></div>			
    			<div style="clear: both;" class="block-footer-left">
    				<div class="block-footer-right">
    					<div class="block-footer-middle">&nbsp;</div>
    				</div>
    			</div>
    			
			</div>

			<div id="paintReportsContent" class="paint-report-content">
			 <div id="admin-sub-links">
				<!-- header -->
            	<div class="block-title-left">
            		<div class="block-title-right">
              			<div class="block-title-middle">
                				<h2 class="block-title"><?php print t('LBL304');?></h2>
                				<div id="admin-maincontent_tab">                					
                					<div class="narrow-search-sortbar" id='find-trng-sort-display' style="display:block;">						
                        		<div class="narrow-search-sortbar-options">                        
                        			<span class="narrow-search-sortbar-sorttext" onclick = 'sortTypeToggle("narrow-search-sortbar-sortlinks");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></span>
                        			<ul class="narrow-search-sortbar-sortlinks">
                            		<li class="first"><a class='type1' onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction("AZ","type1"); sortTypeToggle("narrow-search-sortbar-sortlinks");'><?php print t("LBL017"); ?></a></li>
                            		<li><a class='type2' onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction("ZA","type2"); sortTypeToggle("narrow-search-sortbar-sortlinks");'><?php print t("LBL018"); ?></a></li>
                            		<li><a class='type3' onclick='$("#lnr-reports-search").data("lnrreportssearch").searchAction("Time","type3"); sortTypeToggle("narrow-search-sortbar-sortlinks");'><?php print t("LBL044"); ?></a></li>
 															</ul>
 															<div class="clearBoth"></div>
                        			</div>
														</div> 
													</div>               				
            			</div>
            		</div>
            	</div>
            </div>
            	<!-- 
  				<div style="clear: both;" class="block-findtraining-left">
      				<div class="block-findtraining-right">
      					<div class="block-findtraining-middle">&nbsp;</div>
      				</div>
  				</div>
  				 -->				 
  			 
  			<div id="sort-bar-V2">
  			<ul class="AdminsublinktabNavigation"></ul>
  					 <div id="narrow-search-actionbar" class="narrow-search-actionbar"  data=''>  
                <ul class="narrow-search-actionbar-list" id="narrow-search-actionbar-list">
                	<div class="grey-btn-bg-left"></div>
                	<div class="grey-btn-bg-middle manage-dd"><span><?php print t('Manage');?></span></div>
                	<div class="manage-btn">
                	    <a id="manage-dd-arrow" class="grey-btn-bg-right" "href="javascript:void(0);" onclick="manageDropdown();">&nbsp;&nbsp;</a>
                	    <span class="dd-arrow-bg">&nbsp;</span>
                	</div>
						<ul id="manage-dd-list">
	        	 			<div class="manage-dd-list-arrow"></div>
                			<li>
                  			<a onclick='$("#lnr-reports-search").data("lnrreportssearch").exportSearchResults();'><span class="narrow-search-actionbar-export"> </span> <span class="dropdown-title"><?php print t('Export to CSV'); ?></span></a>
                  			</li>
                  			<div class="clearBoth"></div>
                  			<li>
                  			<a onclick='$("#lnr-reports-search").data("lnrreportssearch").printSearchResults();'><span class="narrow-search-actionbar-print" > </span> <span class="dropdown-title"><?php print t('Print as PDF'); ?></span></a>
                  			</li>
                  			<div class="clearBoth"></div>
                  		</ul>
                  <?php if(user_access('Create Report Perm')){ ?>
                  <li>
                  	<span class="narrow-search-separater"></span>
                  		<a onclick="$('body').data('reportsearch').reportSelectWizard();" class="narrow-search-actionbar-textbtn ">
									   	 <span class="narrow-search-actionbar-orange-btnLeft"></span>
       								 <span title="<?php print t('Create Report'); ?>" class="narrow-search-actionbar-orange-btnBG"><?php print t('Create Report');?></span>
       								 <span class="narrow-search-actionbar-orange-btnRight"></span>
      								</a>
                  </li>
                  <?php } ?>
            		</ul><!-- End .narrow-search-sortbar-options -->
              </div><!-- End .narrow-search-sortbar -->
              <div class="clearBoth"></div>	
           </div>					
	  
				<!-- 
				<div class="region-sidebar-widget-bg">
					<div id="reports-no-records" style="display: none"></div>
					<div id="searchReportsRecordsPaint" class="search-record-paint"><table id="paintLearnerReportsResults"></table></div> 
					<div id="pager" style="display:none;"></div>
				</div>
				-->
				<div class="clearBoth"></div> 
		   <div id="tab-content-main" class='tab-content-main'>
					<div class="narrow-search-results"> 	 	 	 
 						<div id="narrow-search-results-topbar" class="narrow-search-results-topbar" style="display: none;"><!-- Start-narrow search results topbar -->
 						</div><!-- End-narrow search results topbar -->
  					<div class="clearBoth"></div>
  					<div id="reports-no-records" class="narrow-search-no-records" style="display: none;"></div>
  					<div id="paint-narrow-search-results" class="paint-narrow-search-results">    					
    					  <div id="searchReportsRecordsPaint">
    							<table id="paintLearnerReportsResults"></table><!-- jqGrid displays the results here -->
    						</div>
  					</div>
  					<div class="show-more-wrapper">
						<span id="admin-reports-show-more" class="show-more-handler"><?php print t("SHOW MORE"); ?></span>
					</div>
  					<!--
  					<div id="pager" style="display: none"></div>
  					-->
  					<!-- jqGrid Pagination -->
					</div> 
				</div>
				<!-- footer -->
				<div id ='report-search-footer' style="clear: both;" class="block-footer-left">
					<div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					</div>
				</div>

			</div><!-- End #paintReportsContent -->

			<div style="width:100%;clear:both;">  </div>
			
		</div><!-- End report-search -->

	</div>	 <!-- End div Class - 'Content' -->
</div><!-- /.block -->