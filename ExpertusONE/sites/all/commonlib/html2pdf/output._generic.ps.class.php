<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/output._generic.ps.class.php,v 1.1 2011-01-03 13:10:12 srprabhu Exp $

class OutputDriverGenericPS extends OutputDriverGeneric {
  var $language_level;
  var $image_encoder;
 
  function content_type() { 
    return ContentType::ps(); 
  }

  function &get_image_encoder() { 
    return $this->image_encoder; 
  }

  function get_language_level() { 
    return $this->language_level; 
  }

  function OutputDriverGenericPS($image_encoder) {
    $this->OutputDriverGeneric();
        
    $this->set_language_level(2);
    $this->set_image_encoder($image_encoder);
  }

  function reset(&$media) {
    OutputDriverGeneric::reset($media);
  }

  function set_image_encoder(&$encoder) {
    $this->image_encoder = $encoder;
  }

  function set_language_level($version) {
    $this->language_level = $version;
  }
}
?>