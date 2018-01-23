<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.right.width.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderRightWidth extends CSSSubProperty {
  function CSSBorderRightWidth(&$owner) {
    $this->CSSSubProperty($owner);
  }

  function set_value(&$owner_value, &$value) {
    if ($value != CSS_PROPERTY_INHERIT) {
      $owner_value->right->width = $value->copy();
    } else {
      $owner_value->right->width = $value;
    };
  }

  function get_value(&$owner_value) {
    return $owner_value->right->width;
  }

  function get_property_code() {
    return CSS_BORDER_RIGHT_WIDTH;
  }

  function get_property_name() {
    return 'border-right-width';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }

    $width_handler = CSS::get_handler(CSS_BORDER_WIDTH);
    $width = $width_handler->parse_value($value);
    return $width;
  }
}

?>