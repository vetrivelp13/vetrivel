<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/box.input.text.php,v 1.1 2011-01-03 13:10:09 srprabhu Exp $

/// define('SIZE_SPACE_KOEFF',1.65); (defined in tag.input.inc.php)

class TextInputBox extends InlineControlBox {
  /**
   * @var String contains the default value of this text field
   * @access private
   */
  var $_value;

  function TextInputBox($value, $name) {
    $this->InlineControlBox();

    $this->_value = $value;
    $this->_field_name = $name;
  }

  function &create(&$root, &$pipeline) {
    // Text to be displayed
    if ($root->has_attribute('value')) {
      $text = trim($root->get_attribute('value'));
    } else {
      $text = '';
    };

    /**
     * Input field name
     */
    $name = $root->get_attribute('name');

    $box =& new TextInputBox($root->get_attribute("value"), $name);
    $box->readCSS($pipeline->get_current_css_state());
    $box->setup_content($text, $pipeline);

    return $box;
  }

  function get_height() {
    $normal_height = parent::get_height();

    $hc = $this->get_height_constraint();
    if ($hc->is_null()) {
      return $normal_height;
    } else {
      return $normal_height - $this->_get_vert_extra();
    };
  }

  function show(&$driver) {   
    // Now set the baseline of a button box to align it vertically when flowing isude the 
    // text line

    $this->default_baseline = $this->content[0]->baseline + $this->get_extra_top();
    $this->baseline         = $this->content[0]->baseline + $this->get_extra_top();

    /**
     * If we're rendering the interactive form, the field content should not be rendered
     */
    global $g_config;
    if ($g_config['renderforms']) {
      /**
       * Render background/borders only
       */
      $status = GenericFormattedBox::show($driver);

      /**
       * @todo encoding name?
       * @todo font name?
       * @todo check if font is embedded for PDFLIB
       */
      $driver->field_text($this->get_left_padding(), 
                          $this->get_top_padding(),
                          $this->get_width()  + $this->get_padding_left() + $this->get_padding_right(),
                          $this->get_height(),
                          $this->_value,
                          $this->_field_name);
    } else {
      /**
       * Render everything, including content
       */ 
      $status = GenericContainerBox::show($driver);
    }

    return $status;
  }
}
?>