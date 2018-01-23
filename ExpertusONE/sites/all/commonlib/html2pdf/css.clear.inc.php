<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.clear.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('CLEAR_NONE',0);
define('CLEAR_LEFT',1);
define('CLEAR_RIGHT',2);
define('CLEAR_BOTH',3);

class CSSClear extends CSSPropertyStringSet {
  function CSSClear() { 
    $this->CSSPropertyStringSet(false, 
                                false,
                                array('inherit' => CSS_PROPERTY_INHERIT,
                                      'left'    => CLEAR_LEFT,
                                      'right'   => CLEAR_RIGHT,
                                      'both'    => CLEAR_BOTH,
                                      'none'    => CLEAR_NONE)); 
  }

  function default_value() { 
    return CLEAR_NONE; 
  }

  function get_property_code() {
    return CSS_CLEAR;
  }

  function get_property_name() {
    return 'clear';
  }
}

CSS::register_css_property(new CSSClear);

?>