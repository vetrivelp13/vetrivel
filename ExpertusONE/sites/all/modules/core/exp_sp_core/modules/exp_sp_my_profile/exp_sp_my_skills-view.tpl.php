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
//echo "<pre>"; print_r($results);
?>
<?php if(empty($results)){ 
 echo  "<div class='norecords-skillset'>".t("MSG520")."</div>";
}else {?>
<div>
  <table border="0" class="title-wrapper addedit-cfield-delbtn-view-mode" id="addedit-cfield-delbtn-wrapper-">
    <tbody>
      <tr>
        <td class="addedit-list-skill-title-skillwrapper">Skills</td>
        <td class="addedit-proftitle-wrapper">Proficiency</td>
         <td class="addedit-exptitle-wrapper">Experience</td>
         <td class="addedit-headertitle-wrapper"></td>
          <td class="addedit-headertitle-wrapper"></td>
		</tr>
		</tbody>
	</table>
   <?php foreach ($results as $result): ?>
	 <table border="0" class="addedit-cfield-delbtn-wrapper" ><tbody>
		<tr class="addedit-cfield-wrapper" >
		 <td width="200">
          <div class="form-item form-type-select list-skill-details">
          <?php print $result->skill_name;?>
         </div>
         </td>
         <td width="155">
        <div class="form-item form-type-select list-skill-details">
          <?php print $result->proficiency_name;?>
        </div>
        </td>
        <td width="150">
        <div class="form-item form-type-select list-skill-details">
         <?php if($result->experience == 11){ print "Greater than 10";}else{print $result->proficiency_name;}?>
         </div>
         </td>
       </tr>
     </tbody>
   </table>
  <?php endforeach; ?>
</div>
<?php }?>

