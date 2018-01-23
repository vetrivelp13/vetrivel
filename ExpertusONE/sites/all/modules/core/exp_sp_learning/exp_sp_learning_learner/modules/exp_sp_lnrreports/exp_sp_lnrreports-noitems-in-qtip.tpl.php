<?php
  expDebug::dPrint('exp_sp_lnrreports-noschedules.tpl.php: $popup_id 1234555---> = ' . $popup_id, 4);
  expDebug::dPrint('exp_sp_lnrreports-noschedules.tpl.php: $$menu_path 1234555---> = ' . $menu_path, 4);
  expDebug::dPrint('exp_sp_lnrreports-noschedules.tpl.php: $$button_label 1234555---> = ' . $button_label, 4);
  expDebug::dPrint('exp_sp_lnrreports-noschedules.tpl.php: $$help_text 1234555---> = ' . $help_text, 4);
?>

      <div class="noitems-container <?php print $class;?>">
        <a id="create-item-link-<?php print $popup_id; ?>" class="addedit-form-expertusone-throbber use-ajax" 
           data-wrapperid="<?php print $popup_id;?>" href="?q=<?php print $menu_path;?>"> 
          <div class="admin-save-button-left-bg"></div>
          <div class="admin-save-button-middle-bg"><?php print $button_label;?></div>
          <div class="admin-save-button-right-bg"></div>
        </a>
          <div id="noitems-msg" class="admin-empty-text-msg">
          <?php print $help_text; ?>
        </div>
     </div>
         