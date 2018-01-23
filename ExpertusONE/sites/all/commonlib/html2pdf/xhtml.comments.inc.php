<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/xhtml.comments.inc.php,v 1.1 2011-01-03 13:10:13 srprabhu Exp $

function remove_comments(&$html) {
  $html = preg_replace("#<!--.*?-->#is","",$html);
  $html = preg_replace("#<!.*?>#is","",$html);
}

?>