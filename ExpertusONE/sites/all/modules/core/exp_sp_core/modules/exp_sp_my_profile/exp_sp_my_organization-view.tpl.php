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

<div class="profile-role-section">    <?php if(!empty($results->job_title)){ 
                  $job_title = getJobTitle($results->job_title);
                } 
             if(!empty($job_title) || !empty($results->orgname)){?>
		    <table cellspacing="2" cellpadding="2" class="my-profile-box myProfile" border="0">
		     	<tbody>
		     	    <tr>
		     		<td class="field-role vtip" title=" <?php print sanitize_data($job_title); ?>"><?php (empty($results->orgname)) ? print titleController('EXP-SP-MYPROFILE-JOB-TITLE-FULL-LENGTH',sanitize_data($job_title)): print titleController('EXP-SP-MYPROFILE-JOB-TITLE',sanitize_data($job_title)) ?></td>
		       	    <?php if(!empty($results->orgname)){?><td class="field-at"><?php print t('LBL809');?></td><td class="field-org vtip" title=" <?php print sanitize_data($results->orgname); ?>"> <?php (empty($job_title)) ? print titleController('EXP-SP-MYPROFILE-ORG-NAME-FULL-LENGTH',sanitize_data($results->orgname)): print titleController('EXP-SP-MYPROFILE-ORG-NAME',sanitize_data($results->orgname)) ;?></td><?php }?>	
		     		</tr>
		     		</tbody></table>
		     <?php  }?>		
		     		<table cellspacing="2" cellpadding="2" class="my-profile-box myProfile1">
		     		<tbody>
		     		<tr>
		     			<?php if(!empty($manager_detail)){ expDebug::dPrint("testing the manager name".$manager_detail,4); $restrictChar = ($theme_key == "expertusoneV2")? 15 : 16; ?> <td class="field-reports"><?php print t("MSG585");?></td><td class="field-mngr vtip" title="<?php print sanitize_data($manager_detail); ?>"> <?php print titleController('EXP-SP-MYPROFILE-MANAGER-NAME', sanitize_data($manager_detail),$restrictChar);?></td><?php }else{?><td class="field-empty"><?php print t("LBL890");?></td><?php }?>
		     		</tr>
		     	</tbody>
		    </table>
 </div>

