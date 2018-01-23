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

<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?> mylearning-calendar-customization">
 	  <div class="content">
	  	<div class="region-sidebar-widget-bg">
	    <?php print $content; ?>
	    </div>
	  </div>
</div><!-- /.block class = "mylearning-calendar -->

<?php
$xml1 = '';
$xml1 = getClassList(1);
$xmlr = rawurlencode($xml1);
//expDebug::dPrint('xml testtest'.print_r($xml,true),5);
?>
<div id= "calendar_custom" class="mylearning-calendar-tab" style="display:none;">
<div id= "calendar_new" class="mylearning-calendar-tab" >  
<input type='hidden' id='catalog-admin-cal-xml' value='<?php print $xmlr;?>'></input>
<div id='catalog-admin-style' class='cal-widget-height'>
<div id='catalog-admin-cal'></div>
<div class='sp_rowseparator'></div>
</div><div class='splitter' style='height: 0px;'>&nbsp;</div>
<div id='catalog-admin-cal-legend'>
<div class='first-row'><div class='first-row-cell'>
<table cellspacing=0 cellpadding=0 border=0>
<tr class='cal-legent-tr-height'>
<td class='cal-legent-td'><span class='eventdot todaydot'>&nbsp;</span><span class='mycalendar-class-types'>Today</span></td>
<td class='cal-legent-td'><span class='eventdot updot'>&nbsp;</span><span class='mycalendar-class-types'>Upcoming</span></td>
<?php
$isInstructor=is_instructor(getSltpersonUserId());
if ((isset($_SESSION['availableFunctionalities']->exp_sp_instructor_desk))&& $isInstructor==1 ) {
?>
<td class='cal-legent-td'><span class='eventdot instructordot'>&nbsp;</span><span class='mycalendar-class-types'>Instructor</span></td> </tr>
<tr class='cal-legent-tr-height'>
<td class='cal-legent-td'><span class='eventdot regdot'>&nbsp;</span><span class='mycalendar-class-types'>Registered</span></td>
<td class='cal-legent-td'><span class='eventdot compdot'>&nbsp;</span><span class='mycalendar-class-types'>Completed</span></td>
<td class='cal-legent-td'>&nbsp;</td>
</tr>
<?php } else {?> 
<td class='cal-legent-td'><span class='eventdot regdot'>&nbsp;</span><span class='mycalendar-class-types'>Registered</span></td> </tr>
<tr class='cal-legent-tr-height'>
<td class='cal-legent-td'><span class='eventdot compdot'>&nbsp;</span><span class='mycalendar-class-types'>Completed</span></td>
<td class='cal-legent-td'>&nbsp;</td>
</tr>
<?php }?>
</table>
</div></div>
</div>
<div id='catalog-admin-cal-session-details' class='session-details-list'> </div>
</div>
</div>