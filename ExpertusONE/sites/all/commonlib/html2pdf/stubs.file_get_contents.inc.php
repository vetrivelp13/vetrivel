<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/stubs.file_get_contents.inc.php,v 1.1 2011-01-03 13:10:12 srprabhu Exp $

function file_get_contents($file) {
  $lines = file($file);
  if ($lines) {
    return implode('',$lines);
  } else {
    return "";
  };
}
?>