<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.word-spacing.inc.php,v 1.1 2011-01-03 13:10:11 srprabhu Exp $

class CSSWordSpacing extends CSSPropertyHandler {
  var $_default_value;

  function CSSWordSpacing() { 
    $this->CSSPropertyHandler(false, true); 

    $this->_default_value = Value::fromString("0");
  }

  function default_value() { 
    return $this->_default_value;
  }

  function parse($value) {
    $value = trim($value);

    if ($value === 'inherit') {
      return CSS_PROPERTY_INHERIT;
    };

    if ($value === 'normal') { 
      return $this->_default_value; 
    };

    return Value::fromString($value);
  }

  function get_property_code() {
    return CSS_WORD_SPACING;
  }

  function get_property_name() {
    return 'word-spacing';
  }
}

CSS::register_css_property(new CSSWordSpacing);

?>
