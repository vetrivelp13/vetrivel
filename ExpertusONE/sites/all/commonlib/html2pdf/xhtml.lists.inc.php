<?php 
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/xhtml.lists.inc.php,v 1.1 2011-01-03 13:10:13 srprabhu Exp $

function process_li(&$sample_html, $offset) {
  return autoclose_tag($sample_html, $offset, "(ul|ol|li|/li|/ul|/ol)", 
                       array("ul" => "process_ul",
                             "ol" => "process_ol"),
                       "/li");
};

function process_ol(&$sample_html, $offset) {
  return autoclose_tag($sample_html, $offset, "(li|/ol)",
                       array("li" => "process_li"),
                       "/ol");
};

function process_ul(&$sample_html, $offset) {
  return autoclose_tag($sample_html, $offset, "(li|/ul)",
                       array("li" => "process_li"),
                       "/ul");
};

function process_lists(&$sample_html, $offset) {
  return autoclose_tag($sample_html, $offset, "(ul|ol)",
                       array("ul" => "process_ul",
                             "ol" => "process_ol"),
                       "");
};

?>