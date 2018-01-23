<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.min-width.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSMinWidth extends CSSSubFieldProperty {
  function CSSMinWidth(&$owner, $field) {
    $this->CSSSubFieldProperty($owner, $field);
  }

  function get_property_code() {
    return CSS_MIN_WIDTH;
  }

  function get_property_name() {
    return 'min-width';
  }

  function parse($value) {
    if ($value == 'inherit') {
      return CSS_PROPERTY_INHERIT;
    }
    
    return Value::fromString($value);
  }
}

?>