<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.top.color.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderTopColor extends CSSSubProperty {
  function CSSBorderTopColor(&$owner) {
    $this->CSSSubProperty($owner);
  }

  function set_value(&$owner_value, &$value) {
    $owner_value->top->setColor($value);
  }

  function get_value(&$owner_value) {
    return $owner_value->top->color->copy();
  }

  function get_property_code() {
    return CSS_BORDER_TOP_COLOR;
  }

  function get_property_name() {
    return 'border-top-color';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }

    return parse_color_declaration($value);
  }
}

?>