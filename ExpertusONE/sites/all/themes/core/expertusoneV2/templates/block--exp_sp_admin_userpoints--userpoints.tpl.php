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
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div id='user-points' class="most-active-user" style='width:100%'>
  <div id="catalog-user-points-search-list">
    <div class='block-title-left'>
 	  <div class='block-title-right'>
 	    <div class='block-title-middle'>
 		  <?php if ($title): ?>
 		    <h2 class='block-title'><?php print $title; ?></h2>
 		  <?php endif; ?>
 		</div>
 	  </div>
 	  </div>
 	</div>
	<div id='catalog-userpoints-loader'>
     <?php $userId = getSltpersonUserId();
     $resultTotal =  getActionPointsForUser($userId);
     $regPoints = ($resultTotal['register_class']) ? $resultTotal['register_class'] : 0;
     $compPoints = ($resultTotal['complete_class_training']) ? $resultTotal['complete_class_training'] : 0;
     $topicCommentPoints = ($resultTotal['add_topic_comment_reply']) ? $resultTotal['add_topic_comment_reply'] : 0;
     $ratePoints = ($resultTotal['rating_class']) ? $resultTotal['rating_class'] : 0;
    // $votePoints = ($resultTotal['voting_comment_reply']) ? $resultTotal['voting_comment_reply'] : 0;
     $sharePoints = ($resultTotal['sharing_class']) ? $resultTotal['sharing_class'] : 0;
	 $sharemodule=getShareModuleStatus('profile'); 	
     ?>
      <div id='users-points-view' class="user-points-result-widget">
      		<div id='users-points-view-row1' class="points-row1">
    			<div class="points-row1-column-1"><div class="column-action-points"><?php print $regPoints; ?></div><div class="column-action-text"><?php print t('LBL1067');?></div></div>
    			<div class="points-row1-column-2"><div class="column-action-points"><?php print $compPoints; ?></div><div class="column-action-text"><?php print t('LBL1068');?></div></div>
    		</div>
    		<div id='users-points-view-row2' class="points-row2">
 			<?php if(!$sharemodule){?>
         <div class="points-row2-column-1-sharedisable"><div class="column-action-points"><?php print $ratePoints;?></div><div class="column-action-text"><?php print ucfirst(t('LBL149'));?></div></div>
          <div class="points-row2-column-3-sharedisable"><div class="column-action-points"><?php print $topicCommentPoints;?></div><div class="column-action-text"><?php print t('LBL1140');?></div></div>
               <?php } else {?>
                <div class="points-row2-column-1"><div class="column-action-points"><?php print $ratePoints;?></div><div class="column-action-text"><?php print ucfirst(t('LBL149'));?></div></div>
                <div class="points-row2-column-1"><div class="column-action-points"><?php print $sharePoints;?></div><div class="column-action-text"><?php  print t('LBL1139');?></div></div>
                <div class="points-row2-column-3"><div class="column-action-points"><?php print $topicCommentPoints;?></div><div class="column-action-text"><?php print t('LBL1140');?></div></div>
             <?php }?>
             </div>
    		<div id='users-points-view-row3' style="text-align:center;">
    			<div id="badges-view">
    		    <?php $badgesDetails = array();
    		      $regBadge = getUserPointActionBadgeCode($regPoints,'cre_sys_upt_bg1');
    		      $shareBadge = getUserPointActionBadgeCode($sharePoints,'cre_sys_upt_bg2');
    		      $commentBadge = getUserPointActionBadgeCode($topicCommentPoints,'cre_sys_upt_bg3');
    		      $compBadge = getUserPointActionBadgeCode($compPoints,'cre_sys_upt_bg4');
    		      $rateBadge = getUserPointActionBadgeCode(($votePoints+$ratePoints),'cre_sys_upt_bg5');
    		      if($regBadge) {?>
    		      	<span class="registration badge-common-image vtip" title="<?php print t('Registration Master')?>"></span>
           		  <?php } 
           		  if($compBadge) {?>
           		  	<span class="finisher badge-common-image vtip" title="<?php print t('Perfect Finisher')?>"></span>
           		  <?php } 
           		  if($rateBadge) {?>
           		  	<span class="judge badge-common-image vtip" title="<?php print t('Super Judge')?>"></span>
           		  <?php } 
           		  if($shareBadge ==true && $sharemodule) { 
				    ?>
           		  	<span class="expert badge-common-image vtip" title="<?php print t('Share Expert')?>"></span>
				<?php } 
           		  if($commentBadge) {?>
           		  	<span class="blogger badge-common-image vtip" title="<?php print t('Super Blogger')?>"></span>
           		  <?php } ?>
            	</div>
            	<?php if($regBadge || $compBadge || $rateBadge || $shareBadge || $commentBadge){?>
        		<div class="column-action-text"><?php print t('LBL1066');?></div>
        		<?php } else {?>
        		<div class="no-badges-text"><?php print t('MSG673');?></div>
        		<?php }?>
            </div>
            
      </div>
	</div>
	<div class='block-footer-left'>
	  <div class='block-footer-right'>
		<div class='block-footer-middle'>&nbsp;</div>
	  </div>
	</div>
  </div>
</div>