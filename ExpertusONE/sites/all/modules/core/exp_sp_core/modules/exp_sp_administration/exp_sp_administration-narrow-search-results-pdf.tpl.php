<?php
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: data = ' . print_r($data, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: title = ' . print_r($title, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: applied_filters = ' . print_r($applied_filters, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: header_field_map = ' . print_r($header_field_map, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: font_size = ' . print_r($font_size, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: only_header = ' . print_r($only_header, true), 5);
expDebug::dPrint('exp_sp_administration-narrow-search-results-pdf.tpl.php: only_footer = ' . print_r($only_footer, true), 5);

if (empty($font_size)) {
	$font_size = 11;
}
 global $base_url;
 $currentDefaultThemePath = $base_url.base_path().drupal_get_path('theme',variable_get('theme_default', NULL));
 $logo_path = get_logo_path();
 $logo_image_path = str_replace ($base_url, $_SERVER['DOCUMENT_ROOT'],$logo_path );
 expDebug::dPrint('pdf convert = ' . print_r($logo_image_path, true), 5);
?>
<?php if (empty($only_footer)) { ?>
<html>
<body style="font-family:sans-serif">
     <div style="margin-top:14px; margin-left:14px; text-align:left; height:50px;">
    </div>
    <div style ="text-align:center; margin-bottom: 5px; font-weight:bold; font-size:14pt">
      <?php print $title; ?>
    </div>
    <div style="margin-bottom: 15px;">
	    <?php 
	      foreach ($applied_filters as $filter => $value) {
	    ?>
	    <div style="line-height:12pt; margin-bottom: 5px;">
	      <span style="font-weight:bold; font-size:<?php print $font_size;?>pt;"><?php print $filter; ?>:&nbsp;</span>
	      <span style="font-weight:normal; font-size:<?php print $font_size;?>pt;"><?php print $value; ?></span>
	    </div>
	    <?php
	      }
	    ?>
    </div>
    <table border="1" width="100%" style="font-size:<?php print $font_size;?>pt; border-collapse: collapse;" cellpadding="5">
      <thead style="display:table-header-group;">
        <tr>
      <?php 
        $numcols = 0;
        foreach ($header_field_map as $hlabel => $hdetails) {
        	$numcols += 1;
      ?>
          <th align="left" valign="top" width="<?php print $hdetails[1]?>%" style=""><?php print $hlabel;?></th>     
      <?php
        }
      ?>
        </tr>
      </thead>
      <tbody>
<?php }
      if (empty($only_header) && empty($only_footer)) {
        if (isset($_GET['cron_key']) && empty($data)) { ?>
        	<tr>
        	  <td align="left" valign="top" colspan="<?php print $numcols;?>" align="center"><?php print t('MSG278', array(), array('langcode' => 'en-us')); //There are no records.?></td>
        	</tr>
      <?php
        }
        else {
          foreach ($data as $row) {
      ?>
        <tr>
        <?php
          foreach ($header_field_map as $hlabel => $hdetails) {
        ?>
            <td align="left" valign="top"><?php print $row[$hdetails[0]];?></td>
        <?php
          }
        ?>
        </tr>
			<?php
			    }
        }
      }
if (empty($only_header)) {?>
      </tbody>
    </table>
</body>
</html>
<?php } ?>