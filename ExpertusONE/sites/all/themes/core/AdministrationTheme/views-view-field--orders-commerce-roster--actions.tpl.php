<?php
// $Id: views-view-field.tpl.php,v 1.1 2011-04-14 11:12:50 muthusamys Exp $
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified. 
  */ 
//print_r($row);
?>
<a title="<?php print t("View order"); ?>" href="<?php print url('admin/store/orders/'.$row->order_id); ?>" class="popups-form-reload" id='test-popup'><img alt="<?php print t("View order"); ?>" src="<?php print drupal_get_path('module', 'uc_store'); ?>/images/exp_sp_icon_20x20_View.gif"></a>
