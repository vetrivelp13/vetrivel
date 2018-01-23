<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.left.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderLeft extends CSSSubFieldProperty {
  function get_property_code() {
    return CSS_BORDER_LEFT;
  }

  function get_property_name() {
    return 'border-left';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    };

    $border = CSSBorder::parse($value);
    return $border->left;
  }
}

?>