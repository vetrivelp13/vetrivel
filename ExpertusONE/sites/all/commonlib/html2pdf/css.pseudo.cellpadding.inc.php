<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.pseudo.cellpadding.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

class CSSCellPadding extends CSSPropertyHandler {
  function CSSCellPadding() { 
    $this->CSSPropertyHandler(true, false); 
  }

  function default_value() { 
    return Value::fromData(1, UNIT_PX);
  }

  function parse($value) { 
    return Value::fromString($value);
  }

  function get_property_code() {
    return CSS_HTML2PS_CELLPADDING;
  }

  function get_property_name() {
    return '-html2ps-cellpadding';
  }
}

CSS::register_css_property(new CSSCellPadding);

?>