<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.bottom.color.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderBottomColor extends CSSSubProperty {
  function CSSBorderBottomColor(&$owner) {
    $this->CSSSubProperty($owner);
  }

  function set_value(&$owner_value, &$value) {
    $owner_value->bottom->setColor($value);
  }

  function get_value(&$owner_value) {
    $value = $owner_value->bottom->color->copy();
    return $value;
  }

  function get_property_code() {
    return CSS_BORDER_BOTTOM_COLOR;
  }

  function get_property_name() {
    return 'border-bottom-color';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }

    return parse_color_declaration($value);
  }
}

?>