<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.visibility.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

define('VISIBILITY_VISIBLE',0);
define('VISIBILITY_HIDDEN',1);
define('VISIBILITY_COLLAPSE',2); // TODO: currently treated is hidden

class CSSVisibility extends CSSPropertyStringSet {
  function CSSVisibility() { 
    $this->CSSPropertyStringSet(false, 
                                false,
                                array('inherit'  => CSS_PROPERTY_INHERIT,
                                      'visible'  => VISIBILITY_VISIBLE,
                                      'hidden'   => VISIBILITY_HIDDEN,
                                      'collapse' => VISIBILITY_COLLAPSE)); 
  }

  function default_value() { 
    return VISIBILITY_VISIBLE; 
  }

  function get_property_code() {
    return CSS_VISIBILITY;
  }

  function get_property_name() {
    return 'visibility';
  }
}

CSS::register_css_property(new CSSVisibility);

?>