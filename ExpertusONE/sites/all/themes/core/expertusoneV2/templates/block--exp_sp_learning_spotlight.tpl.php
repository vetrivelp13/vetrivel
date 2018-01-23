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
 global $base_url;
 ?>
<div class="<?php print $classes; ?>" id="<?php print $block_html_id; ?>">
  <div class="block-title-left">
    <div class="block-title-right">
      <div class="block-title-middle">
        <?php if ($title): ?>
        	<h2 class='block-title'><?php print $title; ?></h2>
        <?php endif; ?>
      </div>
    </div>
  </div>
  <div>
    <div class="content">
      <div class="region-sidebar-widget-bg">
        <?php if ($content!=" "): ?>
			<div class="spotlight-message-text"><?php print $content ?></div>
		<?php endif; ?>
        <ul>
          <?php  
             $large_icon_img=($block->blockname!='NEW TRAINING')?"-large":"";
             $num_rows = count($block->results); 
             foreach ($block->results as $id => $row) {               
               $entity_type = $row->entity_type;  
               $short_entity_type=substr($entity_type, -3);               
               $detail_page_url_link=($entity_type=="cre_sys_obt_crs")?"home/learning/course-details/":"home/learning/learning-plan-details/";                                           
               $icon_img_class="spotlight-item-image"."-".$short_entity_type.$large_icon_img;              
              ?>
            <li class='list-spotlight<?php print ($num_rows == ($id+1))?'-last':'';?>'>
              <div class="spotlight-block">
              <span class="spotlight-block spotlight-block-para">
              	<!--	<span class="limit-title">-->
                  <span class="<?php print $icon_img_class;?>">&nbsp;</span>
                  <span class="spotlight-desc-container">
                  <span class="limit-title-row">
                  <?php $curTitle = sanitize_data($row->title); ?>
                  <?php 
                      if($entity_type=="cre_sys_obt_crt" || $entity_type=="cre_sys_obt_cur" || $entity_type=="cre_sys_obt_trn") {
                        $row->node_id =$row->id;
                      }
						print l($curTitle,$detail_page_url_link.$row->node_id, $options = array('attributes' => array('class' => 'limit-title spotlight-limit-title spotlight-item-title vtip', 'title' => $row->title),'html'=>TRUE));
                    ?>
 					</span>
                     <!--</span>-->
                   <span class="limit-desc-row <?php print $entity_type ?>">                 
                  <span class="spotlight-title-breaker">&nbsp</span>
                   <span class ="limit-desc spotlight-limit-desc vtip"> 
                   <span class="cls-learner-descriptions">
                  <?php $description = str_replace(array("\n","\r","<p>&nbsp;</p>"), array("","&nbsp;",""), $row->short_description); 
                   		$desctxt = $description; 
           		 		print $desctxt;
           		 ?> </span>
              </span>
              </span>
              </span>
          </span>
          <div class="clearBoth"></div>
			</div>
			
            </li>
           <?php } ?>
        </ul>
      </div>
    </div>
	<div class="block-footer-left">
      <div class="block-footer-right">
        <div class="block-footer-middle">&nbsp;</div>
      </div>
	</div>
  </div>
</div>
<?php  drupal_add_js('jQuery(document).ready(function () { $(".spotlight-limit-title").trunk8(trunk8.spot_title);$(".spotlight-limit-desc").trunk8(trunk8.spot_desc); });', 'inline'); ?>