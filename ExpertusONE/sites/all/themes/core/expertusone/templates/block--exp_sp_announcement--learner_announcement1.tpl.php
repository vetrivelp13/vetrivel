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
  <div class="content announcement-maincontainer">
    <div class="cCorner10" id="announcement_type1">
      <div style="width: auto;" id="announcement_details">
<?php foreach ($block->results as $id => $row): ?>
        <span>
          <div class="roundbox-ml" style="<?php print ($id==0)? 'display: block;' :'display: none;'?>" id="announce_<?php print ($id+1); ?>">
            <div class="announce_description">
              <div style="height:394px;position:relative;" id="<?php print $id; ?>_announcement_img_div" class="announcement_img_div"><img height="404" width="700" src="<?php print file_create_url($row->banner_thumbnail); ?>">
                <div class="announce-desc announce-desc-mxhi" id="<?php print $id; ?>_announce-desc">
                  <div class="announce-desc-heading vtip" title="<?php print sanitize_data($row->title); ?>">
                  <?php $curTitle = titleController('ANNOUNCEMENT-LEARNER-TITLE',$row->title,25); ?>
                    <a class="announce-desc-title"><?php print $curTitle; ?></a>
                  </div>
                  <?php $shortTitle = titleController('ANNOUNCEMENT_SHORTTITLE',$row->shortdesc); 
                        $shortTitle = str_ireplace("<a","<a target='_blank'",htmlspecialchars_decode($shortTitle, ENT_NOQUOTES));   
                  ?>
                  	<table class="announce-short-desc-text" border="0" width="100%" cellpadding="0" cellspacing="0">
                  		<tr>
                  			<td valign="top" align="left" width="81%">
			                    <div class="announcement-short-desc">
			                    	<div id="<?php print $id; ?>_short-desc-toggle" class="short-desc-toggle">
    			                      <?php print $shortTitle; ?>
    			                      <?php if(substr($shortTitle, strlen($shortTitle)-3, 3)=='...'){ ?><span class="announcement-popup more-text" onClick="toggleAnnouncementDesc(<?php print $id; ?>, 1);return false;" ><a class="show-short-text"></a></span>
    			                      <?php } ?>
			                    	</div>
			                    	<div id="<?php print $id; ?>_long-desc-toggle" class="long-desc-toggle">
    			                      <?php 
    			                         $annoDesc = $row->shortdesc;
    			                         $annoDesc = str_ireplace("<a","<a target='_blank'",htmlspecialchars_decode($annoDesc, ENT_NOQUOTES));   
    			                         print $annoDesc;
    			                       ?>
    			                      <span class="announcement-popup more-text" onClick="toggleAnnouncementDesc(<?php print $id; ?>, 2);return false;" ><a class="show-full-text"></a></span>
			                    	</div>
			                    </div>
            			        <!-- <a onclick="EXPERTUS.SMARTPORTAL.LnrAnnouncementWidget.moreAnnouncementDesc('0');" title="Click here to view more description" class="more_desc" id="more_desc" href="#">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</a> -->
                  			</td>
                  			<td align="right" valign="bottom" width="19%">
			                    <?php if (count($block->results) > 1): ?>
                                    <div class="announcement_scroll_links">
    		                                  <?php foreach ($block->results as $id1 => $row1): ?>
            	                          			<div class="announce_scroll">
                	                        			<div class="<?php print ($id==$id1)? 'announce_page_enable' :'announce_page_disable'?>" id="circle_<?php print ($id1+1); ?>">
                    	                      				<a id="<?php print ($id1+1); ?>" class="button" href="javascript:void(0);"><?php print ($id1+1); ?></a>
                                        				</div>
                                      				</div>
                                              <?php endforeach; ?>
                                    </div>
		                        <?php endif; ?>
                  			</td>
                  		</tr>
                  	</table>
                </div>
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
              <a class="button" href="javascript:void(0);"><?php print ($id+1); ?></a>
            </div>
          </div>
  <?php endforeach; ?>
        </div>
<?php endif; ?>
      </div>
    </div>
  </div>  
</div><!-- /.block -->
