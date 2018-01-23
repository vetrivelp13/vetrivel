<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.page-break-inside.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSPageBreakInside extends CSSPageBreak {
  function get_property_code() {
    return CSS_PAGE_BREAK_INSIDE;
  }

  function get_property_name() {
    return 'page-break-inside';
  }
}

CSS::register_css_property(new CSSPageBreakInside);

?>