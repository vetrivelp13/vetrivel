<?php
  expDebug::dPrint('exp_sp_administration_drupalajax_qtip.tpl.php: $popup_id = ' . $popup_id, 4);
  expDebug::dPrint('exp_sp_administration_drupalajax_qtip.tpl.php: $popup_width = ' . $popup_width, 4);
  expDebug::dPrint('exp_sp_administration_drupalajax_qtip.tpl.php: $class = ' . $class, 4);
  expDebug::dPrint('exp_sp_administration_drupalajax_qtip.tpl.php: $display_html = ' . $display_html, 5); 
?>

<div id="<?php print $popup_id;?>" class="qtip-box active-qtip-div <?php print $class;?>" style="display:none">
  <a class="qtip-close-button" onclick="$('#root-admin').data('narrowsearch').removeActiveQtip();"></a>
  <span id="bubble-wrapper-<?php print $popup_id;?>">
    <table cellspacing="0" cellpadding="0" width="<?php print $popup_width;?>" height="auto" id="bubble-face-table">
      <tbody>
        <tr>
          <td class="bubble-tl"></td>
          <td class="bubble-t"></td>
          <td class="bubble-tr"></td>
        </tr>
        <tr>
          <td class="bubble-cl"></td>
          <td valign="top" class="bubble-c">   
          <div id="bubble-content-<?php print $popup_id;?>">
            <div id="show_expertus_message"></div>
            <?php print $display_html;?>
          </div>
          </td>
          <td class="bubble-cr"></td>
        </tr>
        <tr>
          <td class="bubble-bl"></td>
          <td class="bubble-blt"></td>
          <td class="bubble-br"></td>
        </tr>
      </tbody>
    </table>
  </span>
</div>