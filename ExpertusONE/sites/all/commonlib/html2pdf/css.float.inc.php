<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.float.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('FLOAT_NONE',0);
define('FLOAT_LEFT',1);
define('FLOAT_RIGHT',2);

class CSSFloat extends CSSPropertyStringSet {
  function CSSFloat() { 
    $this->CSSPropertyStringSet(false, 
                                false,
                                array('left'  => FLOAT_LEFT,
                                      'right' => FLOAT_RIGHT,
                                      'none'  => FLOAT_NONE)); 
  }

  function default_value() { 
    return FLOAT_NONE; 
  }

  function get_property_code() {
    return CSS_FLOAT;
  }

  function get_property_name() {
    return 'float';
  }
}

CSS::register_css_property(new CSSFloat);

?>