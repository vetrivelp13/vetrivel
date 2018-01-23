<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.min-height.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

require_once(HTML2PS_DIR.'value.min-height.php');

class CSSMinHeight extends CSSPropertyHandler {
  var $_defaultValue;

  function CSSMinHeight() { 
    $this->CSSPropertyHandler(true, false); 
    $this->_defaultValue = ValueMinHeight::fromString("0px");
  }

  /**
   * 'height' CSS property should be inherited by table cells from table rows
   * (as, obviously, )
   */
  function inherit($old_state, &$new_state) { 
    $parent_display = $old_state[CSS_DISPLAY];
    if ($parent_display === "table-row") {
      $new_state[CSS_MIN_HEIGHT] = $old_state[CSS_MIN_HEIGHT];
      return;
    }

    $new_state[CSS_MIN_HEIGHT] = 
      is_inline_element($parent_display) ? 
      $this->get($old_state) : 
      $this->default_value();
  }

  function _getAutoValue() {
    return $this->default_value();
  }

  function default_value() { 
    return $this->_defaultValue->copy();
  }

  function parse($value) { 
    return ValueMinHeight::fromString($value);
  }

  function get_property_code() {
    return CSS_MIN_HEIGHT;
  }

  function get_property_name() {
    return 'min-height';
  }
}
 
CSS::register_css_property(new CSSMinHeight);

?>