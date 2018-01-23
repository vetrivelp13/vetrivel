<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.border.collapse.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('BORDER_COLLAPSE', 1);
define('BORDER_SEPARATE', 2);

class CSSBorderCollapse extends CSSPropertyStringSet {
  function CSSBorderCollapse() { 
    $this->CSSPropertyStringSet(true, 
                                true,
                                array('inherit'  => CSS_PROPERTY_INHERIT,
                                      'collapse' => BORDER_COLLAPSE,
                                      'separate' => BORDER_SEPARATE)); 
  }

  function default_value() { 
    return BORDER_SEPARATE; 
  }

  function get_property_code() {
    return CSS_BORDER_COLLAPSE;
  }

  function get_property_name() {
    return 'border-collapse';
  }
}

CSS::register_css_property(new CSSBorderCollapse);

?>