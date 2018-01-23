<?php expDebug::dPrint('In root admin block tpl file',4);?>
<?php 
  $renderedRoundedHeader = theme('root_admin_rounded_header'); 
  $renderedRoundedFooter = theme('root_admin_rounded_footer'); 
  $rendered_admin_links = theme('root_admin_links'); 
  
?>
<div id='root-admin' class='root-admin-block'>
  <div class="root-admin-left-col"><!--  Start - container for Admin links and Filters -->
		  
		  <div class="root-admin-links-block">
		    <?php print $renderedRoundedHeader; ?>
		    <div id="root-admin-links" class="root-admin-links" style="display:block;"><?php print $rendered_admin_links;?></div>
		    <?php print $renderedRoundedFooter ?>
		  </div> 
		  <div class="root-admin-links_div_break"></div>
		  
		  <div class="narrow-search" id="narrow-search" style="display: none;"></div> 
  
  </div><!--  End - container for Admin links and Filters -->
  
  <div class="root-admin-right-col" id="root-admin-search-right-col"><div id="root-admin-results" style="display:none;"></div></div>  
</div> 
