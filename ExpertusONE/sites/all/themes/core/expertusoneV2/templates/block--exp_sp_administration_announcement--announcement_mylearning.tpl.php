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
 $msg = t("MSG267"); /*Viswanathan added for #78112 */
 
 expDebug::dPrint('New Announcement count== ' . print_r($count, true) , 5);
 //$count = 1;
 $select = db_select('slt_site_notice', 'notice');
 $select->leftJoin('slt_person', 'per', 'notice.user_id = per.id');
 $select->leftJoin('slt_profile_list_items', 'plilang', 'notice.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
 $select->leftJoin('slt_profile_list_items', 'plistatus', 'notice.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
 $select->condition('notice.status', 'cre_sys_obt_not_atv', '=');
 $select->distinct();
 $select->addField('notice', 'description', 'description');
 filterByUserAccessPrivileges($select, 'notice','',1,'cre_sys_obt_not','','notice');
 $result = $select->execute()->fetchAll();
 expDebug::dPrint(' LIST $result = ' . print_r($result, true) , 5);
 foreach($result as $desc){
     $description = $desc->description;
 }
?>

<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> mylearning-announcement-customization">
<div id = "no_announcement_msg" class = "no_announcement_msg" >
<?php print  $msg;?></div>
 <div class="content">
   <div class="region-sidebar-widget-bg">
        <?php print $content; ?>
</div>
</div>

<div id= "announcement_custom" class="mylearning-announcement-tab" style="display:none;">
<div id="announcement_loader" class="announcement-content"></div>
<div id='announcement-msg'  class="my-transcript-msg"style='display: none;'>
<?php print $msg;?>
</div>

  <div id="announcement_new" class="jcarousel-skin-tango" style="z-index: 1001;">

    <ul class='announce-list' style='left: none;'>
 
    </ul>
<div class="jcarousel-control-wrapper" style="text-align: center;">
    <div class="jcarousel-control-newer"><?php echo t('LBL1132')?></div>
    <div class="jcarousel-control">
        <?php
    // mylearning_newtheme();
    $total = fetchAnnouncementUserWidget('count');
    $page_count = ceil($total / 3);
    expDebug::dPrint('width for jcarousel control a :'.$page_count);
    $maxDots = 58; // Maximum number of dots(pagination) to be displayed in Announcement carousel
    $maxWidth = 820; // Width of Carousel control Pagination dots
    $padding = 9; // Total of left and right padding
   if($page_count > $maxDots){
        $page_count = $maxDots;
    }
    expDebug::dPrint('width for jcarousel control a :'.$page_count);
    $width = ($maxWidth/$page_count)-$padding;

    expDebug::dPrint('width for jcarousel control a :'.$width);
 ?>
  <a href="#1" class = 'page1 active' onclick ='addClass(this)' style = "width:<?php print $width;?>px;"><?php print 1; ?></a>   
  <?php 
   for($i=2; $i <= $page_count; $i++) {
   ?>
     	<a href="#" class ='page<?php print $i;?>' onclick ='addClass(this)' style = "width:<?php print $width;?>px;"><?php print $i; ?></a>
   <?php }?>
</div>
    <div class="jcarousel-control-older"><?php echo t('LBL1134')?></div>
    <div class = 'jcarousel-next'style="display: block;" onclick ='addClassToNext()'> </div>
    <div class = 'jcarousel-prev'style="display: block;" onclick ='addClassToPrev()'> </div>
</div>
</div>
</div>
</div>