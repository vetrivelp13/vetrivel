<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.pseudo.localalign.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

define('LA_LEFT',0);
define('LA_CENTER',1);
define('LA_RIGHT',2);

class CSSLocalAlign extends CSSPropertyHandler {
  function CSSLocalAlign() { $this->CSSPropertyHandler(false, false); }

  function default_value() { return LA_LEFT; }

  function parse($value) { return $value; }

  function get_property_code() {
    return CSS_HTML2PS_LOCALALIGN;
  }

  function get_property_name() {
    return '-html2ps-localalign';
  }
}

CSS::register_css_property(new CSSLocalAlign);

?>