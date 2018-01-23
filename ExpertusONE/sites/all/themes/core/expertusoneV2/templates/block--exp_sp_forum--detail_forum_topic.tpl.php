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


$urlString = explode('/',$_GET['q']);
$frmDetails = getFrmDetails($urlString[2]);
$num_topics = getCountTopic($urlString[2]);
$num_post = $num_topics + getCountPost($urlString[2]);

$loggedInUserId = getIdOfLoggedInUser();
//$instructor = is_instructor($loggedInUserId);
$forumAdminAccess = forumAdminAccess();
$listList = chechIfInsHasAccess($frmDetails[0]->object_id,$frmDetails[0]->object_type);
$insListArray = explode(',',$listList[0]->ins_list);
if (in_array($loggedInUserId, $insListArray)) {
  $instructorAccess = 1;
}else{
  $instructorAccess = 0;
}

$popupentityId      = $urlString[2];
$popupAddTopicentityType    = 'forum';
$popupAddTopicIdInit        	 = $popupentityId.'_'.$popupAddTopicentityType;  
$popupAddTopicvisibPopupId  = 'qtip_visible_disp_addtopic_'.$popupAddTopicIdInit;
$anchorId = 'frm-'.$popupAddTopicvisibPopupId;
$popupAddTopic    = "data={'entityId':".$popupentityId.",'entityType':'".$popupAddTopicentityType."','url':'ajax/forum-add-topic/frm-tp-list/add/".$popupentityId."/0','popupDispId':'".$popupAddTopicvisibPopupId."','catalogVisibleId':'qtipAddTopicIdqtip_visible_disp_".$popupAddTopicIdInit."','wBubble':475,'hBubble':'auto','tipPosition':'tipfaceTopRight'}";

if($instructorAccess == 1 || $loggedInUserId == 1 || $forumAdminAccess == 1){
  $paintMultiAction = '<div class="add_topic_popup" id="'.$popupAddTopicvisibPopupId.'" ></div><a id='.$anchorId.' data='.$popupAddTopic.' class="use-ajax" href="/?q=ajax/forum-add-topic/frm-tp-list/add/'.$popupentityId.'/0"><span class="narrow-search-actionbar-orange-btnLeft"></span><span class="narrow-search-actionbar-orange-btnBG">'.t('LBL875').'</span><span class="narrow-search-actionbar-orange-btnRight"></span><div class="clearBoth"></div></a>';
}else{
  $paintMultiAction = '<div class="add_topic_popup" id="'.$popupAddTopicvisibPopupId.'" ></div><span class="narrow-search-actionbar-disable-btnLeft"></span><span class="narrow-search-actionbar-disable-btnBG">'.t('LBL875').'</span><span class="narrow-search-actionbar-disable-btnRight"></span><div class="clearBoth"></div>';
}
    
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">

	 	<div class='block-title-left'>
	 		<div class='block-title-right'>
	 			<div class='block-title-middle'>
	 			<?php if (!empty($frmDetails[0]->name)): ?>
			    	 <?php $displayTitle = titleController('FORUM-DETAIL-FORUM-TOPIC-NAME', check_plain($frmDetails[0]->name),80); ?>
	 					<h2 class='block-title course-delivery-title vtip' title='<?php print check_plain($frmDetails[0]->name); ?>'><?php print $displayTitle; ?></h2>
	 					<div class="frm-topic-list-sort" id="frm-topic-list-sort-display">						
                        <div class="frm-topic-list-sortby">
                        	<span class="frm-topic-list-sort-text" onclick = 'sortTypeToggle("sort-by-links");'><?php print t("LBL011"); ?><span class="find-trng-sort-arrow-icon"></span></span>
                        	<ul class="sort-by-links">
                            	<li class="first"><a class='type1' onclick='$("#forum-topic-list-display").data("forumlistdisplay").searchAction("AZ","type1","topic");sortTypeToggle("sort-by-links");'><?php print t("LBL017"); ?></a></li>
                            	<li><a class='type2' onclick='$("#forum-topic-list-display").data("forumlistdisplay").searchAction("ZA","type2","topic");sortTypeToggle("sort-by-links");'><?php print t("LBL018"); ?></a></li>
                            	<li><a class='type3' onclick='$("#forum-topic-list-display").data("forumlistdisplay").searchAction("created","type3","topic");sortTypeToggle("sort-by-links");'><?php print t("LBL044"); ?></a></li>
                            	<li><a class='type4' onclick='$("#forum-topic-list-display").data("forumlistdisplay").searchAction("posttime","type4","topic");sortTypeToggle("sort-by-links");'><?php print t("LBL873"); ?></a></li>
 							</ul>
 							<div class="clearBoth"></div>
                        </div>                        
					
					</div>
	 				<?php endif; ?>
	 			
			    	
	 			</div>
	 		</div>
	 	</div>
	 	
	<div class="forum-topic-content"> 
		<div id='forum-topic-list-display'>
			   <div id="frm-topic-paint-content">
				<!-- header -->
  				<!--<div style="clear: both;" class="block-findtraining-left">
      				<div class="block-findtraining-right">
      					<div class="block-findtraining-middle">&nbsp;</div>
      				</div>
  				</div>
				--><div class="region-sidebar-widget-bg"><div class="region-sidebar-widget-container">
	 					<div class="admin-forum-topic-detail-action-btn discuss-background"><div class="addedit-form-cancel-container-actions"><a class="admin-action-button-middle-bg form-submit" href="?q=learning/forum-list">[&nbsp;<?php print t('LBL212');?>&nbsp;]</a> <div class="narrow-search-separater"></div> <div class="admin-forum-add-topic"> <?php print $paintMultiAction; ?></div></div><div class="clearBoth"></div></div>
	 					<div class="clearBoth"></div>


<!-- Forum topic detail information HTML tag start here -->
<div style="display:none;" class="forumListResultsPaintInfo admin-forum-topic-details-container">
	<div class="learner-forum-delivery-type-img">
		<div class="crs-icon"></div>
	</div>
	<div class="learner-forum-desc-container">
    	<div>
    		<span class="learner-forum-title"><?php print $frmDetails[0]->name; ?></span>
    	</div>
		<div class="learner-forum-list-sub-txt">	 
			<span title="<?php print t('LBL870')?>" class='vtip'><?php print t('LBL870')?>: <?php print $num_topics; ?></span>
			<span class="pipe-line">|</span>
			<span title="<?php print t('LBL867')?>" class='vtip'><?php print t('LBL867')?>: <?php print $num_post; ?></span>
		</div>
		<div class="learner-forum-topic-description">
			<div class="learner-forum-topic-content">
				<?php print $frmDetails[0]->description; ?>
			</div>  
		</div>
	</div>
	<div class="clearBoth"></div>
</div>
				
										
					<div id="no-records" style="display: none"></div>
					<div id="forumTopicListResultsPaint"><table id="forumTopicListContentResults" class="forumListResultsPaintInfo"></table></div> <!-- Datatable Results -->
					<div id="pager" style="display:none;"></div> <!-- Datatable Pagination -->
				</div>
			</div> <!-- End #region-sidebar-widget-bg -->
				
				<!-- footer -->
				<div style="clear: both;" class="block-footer-left">
					<div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					</div>
				</div>

			</div><!-- End #frm-topic-paint-content -->

			<div style="width:100%;clear:both;">  </div>

		</div><!-- End #forum-list-display -->

	</div>	 <!-- End div Class - 'Content' -->
</div><!-- /.block -->
