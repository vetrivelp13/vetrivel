<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.direction.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('DIRECTION_LTR', 1);
define('DIRECTION_RTF', 1);

class CSSDirection extends CSSPropertyStringSet {
  function CSSDirection() { 
    $this->CSSPropertyStringSet(true, 
                                true,
                                array('lrt' => DIRECTION_LTR,
                                      'rtl' => DIRECTION_RTF)); 
  }

  function default_value() { 
    return DIRECTION_LTR; 
  }

  function get_property_code() {
    return CSS_DIRECTION;
  }

  function get_property_name() {
    return 'direction';
  }
}

CSS::register_css_property(new CSSDirection);

?>