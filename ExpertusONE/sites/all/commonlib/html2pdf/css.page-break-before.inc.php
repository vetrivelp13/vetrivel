<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/css.page-break-before.inc.php,v 1.1 2011-01-03 13:10:10 srprabhu Exp $

class CSSPageBreakBefore extends CSSPageBreak {
  function get_property_code() {
    return CSS_PAGE_BREAK_BEFORE;
  }

  function get_property_name() {
    return 'page-break-before';
  }
}

CSS::register_css_property(new CSSPageBreakBefore);

?>