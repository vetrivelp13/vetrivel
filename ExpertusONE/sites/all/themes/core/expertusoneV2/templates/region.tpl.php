<?php
/**
 * @file
 * Default theme implementation to display a region.
 *
 * Available variables:
 * - $content: The content for this region, typically blocks.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - region: The current template type, i.e., "theming hook".
 *   - region-[name]: The name of the region with underscores replaced with
 *     dashes. For example, the page_top region would have a region-page-top class.
 * - $region: The name of the region variable as defined in the theme's .info file.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see zen_preprocess_region()
 * @see zen_process()
 */
if (arg(0) == "learning" && arg(1) == "enrollment-search" && !arg(2) && ($region == 'highlight' || $region == 'sidebar_second')) {
	$widgetSortable = TRUE;
}
?>
<?php if($widgetSortable): ?>
<div class="<?php print $classes; ?>">
	<ul id="<?php print $region; ?>-list" class="sortable-list">
 	 	<?php print $content; ?>
  	</ul>
</div><!-- /.region -->
<?php else: ?>
<div class="<?php print $classes; ?>">
  <?php print $content; ?>
</div><!-- /.region -->
<?php endif; ?>


