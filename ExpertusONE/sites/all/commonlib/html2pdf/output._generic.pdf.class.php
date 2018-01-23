<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/output._generic.pdf.class.php,v 1.1 2011-01-03 13:10:12 srprabhu Exp $

class OutputDriverGenericPDF extends OutputDriverGeneric {
  var $pdf_version;

  function OutputDriverGenericPDF() {
    $this->OutputDriverGeneric();
    $this->set_pdf_version("1.3");
  }

  function content_type() { return ContentType::pdf(); }

  function get_pdf_version() { 
    return $this->pdf_version; 
  }

  function reset($media) {
    OutputDriverGeneric::reset($media);
  }

  function set_pdf_version($version) {
    $this->pdf_version = $version;
  }
}
?>