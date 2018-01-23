<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.table-layout.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

define('TABLE_LAYOUT_AUTO',   1);
define('TABLE_LAYOUT_FIXED',  2);

class CSSTableLayout extends CSSPropertyStringSet {
  function CSSTableLayout() { 
    $this->CSSPropertyStringSet(false, 
                                false,
                                array('auto'  => TABLE_LAYOUT_AUTO,
                                      'fixed' => TABLE_LAYOUT_FIXED)); 
  }

  function default_value() { 
    return TABLE_LAYOUT_AUTO; 
  }

  function get_property_code() {
    return CSS_TABLE_LAYOUT;
  }

  function get_property_name() {
    return 'table-layout';
  }
}

CSS::register_css_property(new CSSTableLayout());
  
?>