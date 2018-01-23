<?php
  global $theme_key;
  global $user;
?>
<style type="text/css">
<?php if(isset($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_separator']==0){?>
#page-container #paintContentResults .ui-widget-content td { border-bottom:none } 
<?php }?>
</style>
<div id="page-container" class="embedWigetPageContainer">
	<div id="page-wrapper" <?php print $page_class;?>>
		<div id="page">
			<?php print render($page['content']);
			?>
		</div>
	</div>
</div>

