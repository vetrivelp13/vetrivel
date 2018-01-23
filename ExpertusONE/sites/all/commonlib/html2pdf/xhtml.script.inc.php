<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/xhtml.script.inc.php,v 1.1 2011-01-03 13:10:13 srprabhu Exp $

function process_script($sample_html) {
  return preg_replace("#<script.*?</script>#is","",$sample_html);
}

?>