<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.color.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSColor extends CSSPropertyHandler {
  function CSSColor() { 
    $this->CSSPropertyHandler(true, true); 
  }

  function default_value() { 
    return new Color(array(0,0,0),false); 
  }

  function parse($value) {
    if ($value === 'inherit') {
      return CSS_PROPERTY_INHERIT;
    };

    return parse_color_declaration($value);
  }

  function get_property_code() {
    return CSS_COLOR;
  }

  function get_property_name() {
    return 'color';
  }
}

CSS::register_css_property(new CSSColor);

?>