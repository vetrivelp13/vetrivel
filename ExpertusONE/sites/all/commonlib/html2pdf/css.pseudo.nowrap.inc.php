<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.pseudo.nowrap.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

define('NOWRAP_NORMAL',0);
define('NOWRAP_NOWRAP',1);

class CSSPseudoNoWrap extends CSSPropertyHandler {
  function CSSPseudoNoWrap() { $this->CSSPropertyHandler(false, false); }
  function default_value() { return NOWRAP_NORMAL; }

  function get_property_code() {
    return CSS_HTML2PS_NOWRAP;
  }

  function get_property_name() {
    return '-html2ps-nowrap';
  }
}

CSS::register_css_property(new CSSPseudoNoWrap);
  
?>