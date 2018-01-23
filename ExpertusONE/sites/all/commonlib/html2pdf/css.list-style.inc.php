<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.list-style.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

require_once(HTML2PS_DIR.'value.list-style.class.php');

class CSSListStyle extends CSSPropertyHandler {
  // CSS 2.1: list-style is inherited
  function CSSListStyle() { 
    $this->default_value = new ListStyleValue;
    $this->default_value->image    = CSSListStyleImage::default_value();
    $this->default_value->position = CSSListStylePosition::default_value();
    $this->default_value->type     = CSSListStyleType::default_value();

    $this->CSSPropertyHandler(true, true); 
  }

  function parse($value, &$pipeline) { 
    $style = new ListStyleValue;
    $style->image     = CSSListStyleImage::parse($value, $pipeline);
    $style->position  = CSSListStylePosition::parse($value);
    $style->type      = CSSListStyleType::parse($value);

    return $style;
  }

  function default_value() { return $this->default_value; }

  function get_property_code() {
    return CSS_LIST_STYLE;
  }

  function get_property_name() {
    return 'list-style';
  }
}

$ls = new CSSListStyle;
CSS::register_css_property($ls);
CSS::register_css_property(new CSSListStyleImage($ls,    'image'));
CSS::register_css_property(new CSSListStylePosition($ls, 'position'));
CSS::register_css_property(new CSSListStyleType($ls,     'type'));

?>