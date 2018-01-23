<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.letter-spacing.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSLetterSpacing extends CSSPropertyHandler {
  var $_default_value;

  function CSSLetterSpacing() { 
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
    return CSS_LETTER_SPACING;
  }

  function get_property_name() {
    return 'letter-spacing';
  }
}

CSS::register_css_property(new CSSLetterSpacing);

?>
