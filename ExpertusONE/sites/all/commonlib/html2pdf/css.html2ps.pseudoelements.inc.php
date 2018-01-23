<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.html2ps.pseudoelements.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

define('CSS_HTML2PS_PSEUDOELEMENTS_NONE'  ,0);
define('CSS_HTML2PS_PSEUDOELEMENTS_BEFORE',1);
define('CSS_HTML2PS_PSEUDOELEMENTS_AFTER' ,2);

class CSSHTML2PSPseudoelements extends CSSPropertyHandler {
  function CSSHTML2PSPseudoelements() { 
    $this->CSSPropertyHandler(false, false); 
  }

  function default_value() { 
    return CSS_HTML2PS_PSEUDOELEMENTS_NONE; 
  }

  function parse($value) {
    return $value;
  }

  function get_property_code() {
    return CSS_HTML2PS_PSEUDOELEMENTS;
  }

  function get_property_name() {
    return '-html2ps-pseudoelements';
  }
}

CSS::register_css_property(new CSSHTML2PSPseudoelements);

?>