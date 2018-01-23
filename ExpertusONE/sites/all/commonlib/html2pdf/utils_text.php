<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/utils_text.php,v 1.1 2011-01-03 13:10:13 srprabhu Exp $

function squeeze($string) {
  return preg_replace("![ \n\t]+!"," ",trim($string));
}

?>