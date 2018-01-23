<?php
expDebug::dPrint('exp_sp_administraton_commerce_payment_cc_settings_screen.tpl.php : $rendered_tabs = ' . print_r($rendered_tabs, true), 5);
expDebug::dPrint('exp_sp_administraton_commerce_payment_cc_settings_screen.tpl.php : $rendered_settings_form = ' . print_r($rendered_settings_form, true), 5);
?>

<div id='commerce-cc-settings-screen-wrapper' class='commerce-cc-settings-screen-wrapper'>
  <div id='commerce-cc-settings-tabs' class='commerce-cc-settings-tabs catalog-course-basic-addedit-action-disp' >
    <?php print $rendered_tabs?>
  </div>
  <div id='commerce-cc-settings-form-wrapper' class='commerce-cc-settings-form-wrapper'>
    <div class="round-corner-left">
      <div class="round-corner-right">
        <div class="round-corner-middle"></div>
      </div>
    </div>
    <div style='clear:both'></div>
    <div class='commerce-cc-settings-form'>
      <?php print $rendered_settings_form?>
    </div>
    <div style='clear:both'></div>
    <div class="round-corner-footer-left">
      <div class="round-corner-footer-right">
        <div class="round-corner-footer-middle"></div>
      </div>
    </div>
  </div>
</div>