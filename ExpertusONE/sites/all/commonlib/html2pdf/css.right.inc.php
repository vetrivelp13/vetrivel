<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.right.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

require_once(HTML2PS_DIR.'value.right.php');

class CSSRight extends CSSPropertyHandler {
  function CSSRight() { 
    $this->CSSPropertyHandler(false, false); 
    $this->_autoValue = ValueRight::fromString('auto');
  }

  function _getAutoValue() {
    return $this->_autoValue->copy();
  }

  function default_value() { 
    return $this->_getAutoValue();
  }

  function parse($value) { 
    return ValueRight::fromString($value);
  }

  function get_property_code() {
    return CSS_RIGHT;
  }

  function get_property_name() {
    return 'right';
  }
}

CSS::register_css_property(new CSSRight);

?>