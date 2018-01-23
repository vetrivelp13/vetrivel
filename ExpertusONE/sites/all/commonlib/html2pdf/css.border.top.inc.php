<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.top.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderTop extends CSSSubFieldProperty {
  function get_property_code() {
    return CSS_BORDER_TOP;
  }

  function get_property_name() {
    return 'border-top';
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