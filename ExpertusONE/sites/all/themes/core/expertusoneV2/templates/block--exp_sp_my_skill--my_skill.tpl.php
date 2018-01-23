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
  $data= "{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/skill-set/add-skill/add/emptyvalue','popupDispId':'skill-qtipid','qtipDisplayPosition':'tipfaceTopMiddle','catalogVisibleId':'qtipskillqtip_visible_disp','entityType':'enr_crs_usr','qtipClass':'display-message-positioning','wBubble':'390','hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
  $headerTitle = t('LBL833').' '. t('LBL647').' '.t('LBL205');
  $title = t('LBL832');
  $title2 = t('LBL287').' '. t('LBL205');
  $title3 = t('LBL309');
  $config=getConfig("exp_sp");
  $profile_cert = $config['profile_certificate'];
 
  ?>

<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>">
  <div id='learner-myskill-details'>
   	<div class='block-title-left'>
   		<div class='block-title-right'>
   			<div class='block-title-middle'>
   				<?php if ($title): ?>
   					<h2 class='block-title'><?php print $headerTitle; ?></h2>  					
   				 <?php endif; ?>
   				 <div id='menuaddskill'><span style="display:block" class="clsenableattachcourse addskillcerti fa-stack pull-down-circle-menu" onclick="showAddmenu();">
                     <i class="fa fa-circle"></i>
                     <i class="fa fa-circle"></i>
                     <i class="fa fa-circle"></i>
                     </span>
                     <ul class="profile-add-menu" style = "display:none">
                     <li><div id="qtip_addcert_div"><a id="add-cert-profile" class="tab-title" onclick = "$('#wizarad-myprofile-skills').data('myprofileskills').openSkillDialog('add','emptyvalue');"><span class="limit-title"><?php print $title ?></span></a><span id="visible-popup" class="qtip-popup-visible" style="display:none;">
        						 </span></div></li>
        						 <?php if($profile_cert == 1) { ?> 
        						   <li><div id="qtip_addcert_div"><a class="addedit-certificate" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').openCertificateDialog('emptyvalue','add');"><?php print t('LBL287').' '. t('LBL205'); //Request Class ?></a></a><span id="visible-popup" class="qtip-popup-visible" style="display:none;">
        						 </span></div></li> 
        						  <?php } ?>
        						 <li><div id="qtip_addcert_div"><a id="add-cert-profile" class="tab-title" onclick = "$('#wizarad-myprofile-skills').data('myprofileskills').exportSearchResults();"><?php print $title3 ?></a><span id="visible-popup" class="qtip-popup-visible" style="display:none;">
        						 </span></div></li>
                     </ul>
                   	 </div>
   			
   		</div>
   		</div>
   	</div>
 
   
  	 <div class="content">	    
  	   <div class="region-sidebar-widget-bg">
  	    <div id="wizarad-myprofile-skills" class="my-skill-wrapper">
  	      <?php print $content; ?>
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
