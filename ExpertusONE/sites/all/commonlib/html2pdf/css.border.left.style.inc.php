<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.left.style.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderLeftStyle extends CSSSubProperty {
  function CSSBorderLeftStyle(&$owner) {
    $this->CSSSubProperty($owner);
  }

  function set_value(&$owner_value, &$value) {
    $owner_value->left->style = $value;
  }

  function get_value(&$owner_value) {
    return $owner_value->left->style;
  }

  function get_property_code() {
    return CSS_BORDER_LEFT_STYLE;
  }

  function get_property_name() {
    return 'border-left-style';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }

    return CSSBorderStyle::parse_style($value);
  }
}

?>