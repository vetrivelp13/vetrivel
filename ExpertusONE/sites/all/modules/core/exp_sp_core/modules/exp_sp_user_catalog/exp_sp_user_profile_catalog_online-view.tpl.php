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
<?php
global $user;
?>
<div id='loadDiv' class='UserProfile_box'>
  <table border='0' width='100%' cellpadding='0' cellspacing='0'>
    <tr>
      <td>
        <div class='UserCatalogInfo' id="qtip-catalog-user-view">
          <div class="profile-online-picture">
            <img width='50px' height='50px' id='avImg' src="<?php echo $result->filename; ?>" />
          </div>
          <div class="profile-catalog-user-info">
            <div class="profile-catalog-user-session">
              <div class="user-profile-text"><?php echo $result->full_name; ?></div>
              <div class="profile-catalog-list-items">
                <div class='user-description'><?php print t("Email");?>:</div>
                <div class='user-details'><?php echo $result->email; ?></div>
                <div class="clearBoth"></div>
              </div>
              <div class="profile-catalog-list-items">
                <div class='user-description'><?php print t("Organization");?>:</div>
                <div class='user-details'><?php echo (!empty($result->name)?$result->name:'-');?></div>
                <div class="clearBoth"></div>
              </div>
              <div class="profile-catalog-list-items">
                <div class='user-description'><?php print t("Location");?>:</div>
                <div class='user-details'><?php echo (!empty($result->location_name)?$result->location_name:'-');?></div>
                <div class="clearBoth"></div>
              </div>
              <div class="profile-catalog-list-items">
                <div class='user-description'><?php print t("Timezone");?>:</div>
                <div class='user-details'><?php echo (!empty($result->timezonename)?$result->timezonename:'-');?></div>
                <div class="clearBoth"></div>
              </div>
            </div>
          </div>
        </div>
      </td>
    </tr>
  </table>
</div>