<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.pseudo.listcounter.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

class CSSPseudoListCounter extends CSSPropertyHandler {
  function CSSPseudoListCounter() { 
    $this->CSSPropertyHandler(true, false); 
  }

  function default_value() { 
    return 0; 
  }

  function get_property_code() {
    return CSS_HTML2PS_LIST_COUNTER;
  }

  function get_property_name() {
    return '-html2ps-list-counter';
  }

  function parse($value) {
    return (int)$value;
  }
}

CSS::register_css_property(new CSSPseudoListCounter);

?>