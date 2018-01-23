<?php
  if ($show_back_to_search_button) {
    $miscActionsList = array(array('title' => $back_to_search_button_title, 'path' => $back_to_search_button_path));
    $miscActionsList = array_merge($miscActionsList, $addl_misc_actions_list);
  }
  else {
    $miscActionsList = $addl_misc_actions_list;
  }
?>

<?php foreach ($miscActionsList as $miscAction): ?>
    <div class="addedit-form-next-misc-action">
      <input type="button" class="addedit-form-misc-button" value="<?php print $miscAction['title']; ?>"
           onclick="javascript:window.location='<?php print $miscAction['path']; ?>';" />
      <div style='clear:both'></div>
    </div>
  <?php endforeach; ?>