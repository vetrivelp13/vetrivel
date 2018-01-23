<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.page-break-after.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSPageBreakAfter extends CSSPageBreak {
  function get_property_code() {
    return CSS_PAGE_BREAK_AFTER;
  }

  function get_property_name() {
    return 'page-break-after';
  }
}

CSS::register_css_property(new CSSPageBreakAfter);

?>