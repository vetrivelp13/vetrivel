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
global $theme_key, $front_sidebar;
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div class="content">
    <div class="cCorner10" id="announcement_type1">
      <div style="width: auto;" id="announcement_details">
<?php foreach ($block->results as $id => $row): ?>
        <span>
          <div class="roundbox-ml" style="<?php print ($id==0)? 'display: block;' :'display: none;'?>" id="announce_<?php print ($id+1); ?>">
            <div class="announce_description">              
				<?php if ((!$front_sidebar) && ($row->banner_large == true)) :?>
				<div style="height:423px;width:942px;position:relative;" id="<?php print $id; ?>_announcement_img_div" class="announcement_img_div">
					 <img class="announcemnt-img" height="423px" width="942px" src="<?php print file_create_url($row->banner_large);?>" id="<?php print $id; ?>_announcemnt-img" >
				<?php elseif ((!$front_sidebar) && ($row->banner_large == false) && ($row->banner_thumbnail == true)) : ?>
				<div style="height:423px;width:621px;position:relative;" id="<?php print $id; ?>_announcement_img_div" class="announcement_img_div">
					 <img class="announcemnt-img" height="423px" width="621px" src="<?php print file_create_url($row->banner_thumbnail);?>" id="<?php print $id; ?>_announcemnt-img" >
				<?php else: ?>
				<div style="height:423px;width:621px;position:relative;" id="<?php print $id; ?>_announcement_img_div" class="announcement_img_div">
					<img class="announcemnt-img" height="423px" width="621px" src="<?php print file_create_url($row->banner_thumbnail);?>" id="<?php print $id; ?>_announcemnt-img">
				<?php endif; ?>
				<?php ?>
              <?php if($theme_key == "expertusoneV2"){?>
              <?php if ((!$front_sidebar) && ($row->banner_large == true)) :?>
              	<div class="announce-desc announce-desc-mxhi limit-title-row" style = "display:block;width:922px " id="<?php print $id; ?>_announce-desc">
              <?php elseif ((!$front_sidebar) && ($row->banner_large == false) && ($row->banner_thumbnail == true)) : ?>
              	<div class="announce-desc announce-desc-mxhi limit-title-row" style = "display:block;width:601px" id="<?php print $id; ?>_announce-desc">
              <?php else: ?>
                <div class="announce-desc announce-desc-mxhi limit-title-row" style = "display:block;width:601px" id="<?php print $id; ?>_announce-desc">
              <?php endif; ?>
                
                  <div class="announce-desc-heading limit-title homebanner-limit-title vtip" title="<?php print sanitize_data($row->title); ?>">
                  <?php $curTitle = $row->title; ?>
                 <!--<a class="announce-desc-title"><?php //print $curTitle; ?></a>-->
                   <?php print $curTitle; ?>
                  </div>
              
                 <!--  	<table class="announce-short-desc-text" border="0" width="100%" cellpadding="0" cellspacing="0" >
                  		<tr>
                  			<td valign="top" align="left" width="81%"> -->
			                    <div class="announcement-short-desc limit-desc-row" id="<?php print $id; ?>_announce-short-desc">
			                	<div class="short-desc-toggle limit-desc homebanner-limit-desc vtip" id="<?php print $id; ?>_short-desc-toggle" >
    			                 <span class="cls-learner-descriptions">     <?php 
    			                         $annoDesc = $row->shortdesc;
    			                         $annoDesc = str_ireplace("<a","<a target='_blank'",htmlspecialchars_decode($annoDesc, ENT_NOQUOTES));   
    			                         print $annoDesc;
    			                       ?> </span>
			                    	</div>
			                    </div>
            			        <!-- <a onclick="EXPERTUS.SMARTPORTAL.LnrAnnouncementWidget.moreAnnouncementDesc('0');" title="Click here to view more description" class="more_desc" id="more_desc" href="#">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</a> -->
                 <!--  			</td>                  		
                  		</tr>
                  	</table> -->
                </div>
                <?php }
                if($theme_key == "expertusoneV2"){
                	$width = 0;
                	$lstno = count($block->results);
                	if($lstno > 1):                	
                	$width = ($lstno * 23) + 35;
                	endif; 
                	
                ?>
                <div class="announce-desc announce-desc-mxhi announce-desc-pager" id="<?php print $id; ?>_announce-desc-1" style="width:<?php print $width;?>px;">
			                    <?php if (count($block->results) > 1) : ?>
                                    <div class="announcement_scroll_links">
    		                                  <?php foreach ($block->results as $id1 => $row1): ?>
            	                          			<div class="announce_scroll">
                	                        			<div class="<?php print ($id==$id1)? 'announce_page_enable' :'announce_page_disable'?>" id="circle_<?php print ($id1+1); ?>">
                    	                      				<a id="<?php print ($id1+1); ?>" class="button" href="javascript:void(0);">&nbsp</a>
                                        				</div>
                                      				</div>
                                              <?php endforeach; ?>
                                    </div>
		                        <?php endif; ?>
                </div>
                <?php  } ?>
              </div>
            </div>
          </div>
        </span>
<?php endforeach; ?>
<?php if ((count($block->results) > 1) && 0): ?>
        <div class="announcement_scroll_links">
  <?php foreach ($block->results as $id => $row): ?>
          <div class="announce_scroll">
            <div class="<?php print ($id==0)? 'announce_page_enable' :'announce_page_disable'?>" id="circle_<?php print ($id+1); ?>">
              <a <?php print ($id+1); ?> class="button" href="javascript:void(0);"><?php print ($id+1); ?></a>
            </div>
          </div>
  <?php endforeach; ?>
        </div>
<?php endif; ?>
      </div>
    </div>
  </div>  
</div><!-- /.block -->
<?php  drupal_add_js('jQuery(document).ready(function () { $(".homebanner-limit-title").trunk8(trunk8.homebanner_title);$(".homebanner-limit-desc").trunk8(trunk8.homebanner_desc); });', 'inline'); ?>