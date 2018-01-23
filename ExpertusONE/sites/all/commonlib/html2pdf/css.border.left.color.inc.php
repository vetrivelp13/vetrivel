<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.left.color.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderLeftColor extends CSSSubProperty {
  function CSSBorderLeftColor(&$owner) {
    $this->CSSSubProperty($owner);
  }

  function set_value(&$owner_value, &$value) {
    $owner_value->left->setColor($value);
  }

  function get_value(&$owner_value) {
    return $owner_value->left->color->copy();
  }

  function get_property_code() {
    return CSS_BORDER_LEFT_COLOR;
  }

  function get_property_name() {
    return 'border-left-color';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }

    return parse_color_declaration($value);
  }
}

?>