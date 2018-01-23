<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.bottom.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSBorderBottom extends CSSSubFieldProperty {
  function get_property_code() {
    return CSS_BORDER_BOTTOM;
  }

  function get_property_name() {
    return 'border-bottom';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    };

    $border = CSSBorder::parse($value);
    return $border->bottom;
  }
}

?>