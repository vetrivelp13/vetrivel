<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.left.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

require_once(HTML2PS_DIR.'value.left.php');

class CSSLeft extends CSSPropertyHandler {
  function CSSLeft() { 
    $this->CSSPropertyHandler(false, false); 
    $this->_autoValue = ValueLeft::fromString('auto');
  }

  function _getAutoValue() {
    return $this->_autoValue->copy();
  }

  function default_value() { 
    return $this->_getAutoValue();
  }

  function parse($value) { 
    return ValueLeft::fromString($value);
  }

  function get_property_code() {
    return CSS_LEFT;
  }

  function get_property_name() {
    return 'left';
  }
}

CSS::register_css_property(new CSSLeft);

?>