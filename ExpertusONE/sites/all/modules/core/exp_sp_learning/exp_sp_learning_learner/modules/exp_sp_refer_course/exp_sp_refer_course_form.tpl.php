<table width="100%" cellpadding="0" cellspacing="0" border="0" class="refer-course-table" >
	<tbody>
      <tr>
        <td class="ref-course-col1"><?php print t('Email From'); ?></td>
        <td class="ref-course-col2">:</td>
        <td class="ref-course-col3"><?php print drupal_render($form['emailfrom']); ?></td>
      	<td class="ref-course-col4"><div class="require-image">&nbsp;</div></td>
      </tr>
      <tr>
        <td class="ref-course-col1"><?php print t('Email To'); ?></td>
        <td class="ref-course-col2">:</td>
        <td class="ref-course-col3"><?php print drupal_render($form['emailto']); ?>
        <?php print '<span class="note">'.t("Enter emails separated by commas.")."</span>" ?>
        </td>
      	<td class="ref-course-col4"><div class="require-image">&nbsp;</div></td>
      </tr>
      <tr>
        <td class="ref-course-col1"><?php print t('Subject'); ?></td>
        <td class="ref-course-col2">:</td>
        <td class="ref-course-col3"><?php print drupal_render($form['subject']); ?></td>
      	<td class="ref-course-col4">&nbsp;</td>
      </tr>
      <tr>
      	<td class="ref-course-col1">&nbsp;</td>
      	<td class="ref-course-col2">&nbsp;</td>
        <td class="ref-course-col3"><?php print drupal_render($form['ccopy']); ?></td>
      	<td class="ref-course-col4"></td>
      </tr>
      <tr>
      	<td class="ref-course-col1">&nbsp;</td>
      	<td class="ref-course-col2">&nbsp;</td>
        <td class="ref-course-col3"><?php print drupal_render($form['description']); ?></td>
      	<td class="ref-course-col4"></td>
      </tr>
      <tr>
        <td class="ref-course-col1">&nbsp;</td>        
        <td class="ref-course-col2">&nbsp;</td>
        <td class="lnr-req--btn-cont"><?php print drupal_render_children($form); ?></td>
      	<td class="ref-course-col4">&nbsp;</td>
      </tr>            
	</tbody>
</table>	