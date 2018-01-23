<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/ps.utils.inc.php,v 1.1 2011-01-03 13:10:12 srprabhu Exp $

function trim_ps_comments($data) {
  $data = preg_replace("/(?<!\\\\)%.*/","",$data);
  return preg_replace("/ +$/","",$data);
}

function format_ps_color($color) {
  return sprintf("%.3f %.3f %.3f",$color[0]/255,$color[1]/255,$color[2]/255);
}
?>