<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.top.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

require_once(HTML2PS_DIR.'value.top.php');

class CSSTop extends CSSPropertyHandler {
  function CSSTop() { 
    $this->CSSPropertyHandler(false, false); 
    $this->_autoValue = ValueTop::fromString('auto');
  }

  function _getAutoValue() {
    return $this->_autoValue->copy();
  }

  function default_value() { 
    return $this->_getAutoValue();
  }

  function get_property_code() {
    return CSS_TOP;
  }

  function get_property_name() {
    return 'top';
  }

  function parse($value) { 
    return ValueTop::fromString($value);
  }
}

CSS::register_css_property(new CSSTop);

?>