<?php
// Each file loads it's own styles because we cant predict which file will be
// loaded.
drupal_add_css(drupal_get_path('module', 'privatemsg') . '/styles/privatemsg-view.base.css');
drupal_add_css(drupal_get_path('module', 'privatemsg') . '/styles/privatemsg-view.theme.css');
?>
<?php
print $anchors; ?>
<div <?php if ( !empty($message_classes)) { ?>
	class="<?php echo implode(' ', $message_classes); ?>" <?php } ?>
	id="privatemsg-mid-<?php print $mid; ?>">
<div class="privatemsg-message-column">
<div class="privatemsg-message-information">
<span class="privatemsg-message-date"><?php print $message_timestamp; ?></span>
<?php if (isset($message_actions)): ?> <?php print $message_actions ?> <?php endif ?>
</div>
<div class="privatemsg-message-body"><?php print $message_body; ?></div>
</div>
<div class="clearfix"></div>
</div>
