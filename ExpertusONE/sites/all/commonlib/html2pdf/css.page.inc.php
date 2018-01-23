<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.page.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSPage extends CSSPropertyHandler {
  function CSSPage() { 
    $this->CSSPropertyHandler(true, true); 
  }

  function default_value() { 
    return 'auto'; 
  }

  function parse($value) {
    return $value;
  }

  function get_property_code() {
    return CSS_PAGE;
  }

  function get_property_name() {
    return 'page';
  }
}

CSS::register_css_property(new CSSPage());

?>