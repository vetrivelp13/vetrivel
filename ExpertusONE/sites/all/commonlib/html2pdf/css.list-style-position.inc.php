<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.list-style-position.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('LSP_OUTSIDE',0);
define('LSP_INSIDE',1);

class CSSListStylePosition extends CSSSubFieldProperty {
  // CSS 2.1: default value for list-style-position is 'outside'
  function default_value() { return LSP_OUTSIDE; }

  function parse($value) {
    if (preg_match('/\binside\b/',$value)) {
      return LSP_INSIDE; 
    };

    if (preg_match('/\boutside\b/',$value)) { 
      return LSP_OUTSIDE; 
    };

    return null;
  }

  function get_property_code() {
    return CSS_LIST_STYLE_POSITION;
  }

  function get_property_name() {
    return 'list-style-position';
  }
}

?>