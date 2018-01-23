<div id="innerWidgetTagEnroll"> <!--  start innerWidget -->
<?php
//Render catalog Block
$block_rendered_catalog= drupal_render($variables['widgetarray'][0]);
 ?>
 <div style="display:none;" id="search_searchtext"></div>
<div id="main-wrapper"> <!--  start main-wrapper -->
	<div id="main" class="clearfix with-navigation"><!--  start main -->
	  <div id="content" class="column"><!--  start content -->
		<div class="section"><!--  start section -->
		  <div class="region region-highlight"><!--  start region -->
		     <?php print $block_rendered_catalog; ?>
		   </div>  <!--  end region -->
          </div><!--  end section -->
        </div><!--  end content -->
     </div><!--  end main -->
</div><!--  end main-wrapper -->
		  			
  
  </div><!--  end innerWidget -->