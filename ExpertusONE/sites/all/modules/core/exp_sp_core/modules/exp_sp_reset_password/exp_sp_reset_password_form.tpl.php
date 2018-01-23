<?php
global $theme_key;
$form['buttons']['next']['#value'] = t('LBL124');
if($theme_key == 'expertusoneV2') {
	$form['buttons']['next']['#attributes']['class'][] = 'admin-save-button-middle-bg';
}

?>
<table width="100%" cellpadding="0" cellspacing="0" border="0" class="reset-pass-table" >
	<tbody>
      <tr>
        <td class="reset-pass-table-col1"><?php print t('LBL754').' '.t('LBL060'); ?> :<div class="require-text">*</div></td>
        <td class="reset-pass-table-col3"><?php print drupal_render($form['new_password']); ?></td>
      	<!--  <td class="reset-pass-table-col4"><div class="require-image">&nbsp;</div></td> -->
      </tr>
      <tr>
      	<td class="reset-pass-table-col1"><?php print t('LBL075');?> :<div class="require-text">*</div></td>
        <td class="reset-pass-table-col3"><?php print drupal_render($form['confirm_password']); ?></td>
      	<!--  <td class="reset-pass-table-col4"><div class="require-image">&nbsp;</div></td>  -->
      </tr>
      <tr>
        <td class="reset-pass-table-col1">&nbsp;</td>        
        <td class="reset-pass-table-col3 reset-pass-btn-cont"><div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div><?php print drupal_render_children($form); ?><div class="admin-save-button-right-bg"></div></div></td>
      	<td class="reset-pass-table-col4">&nbsp;</td>
      </tr>            
	</tbody>
</table>	