<?php expDebug::dPrint('narrow search filters holder tpl file : $rendered_filtersets_list = ' . print_r($rendered_filtersets_list, true),4);?>
<?php
  if($enable_option == true){    
  $renderedRoundedHeader = theme('root_admin_rounded_header');
  $renderedRoundedFooter = theme('root_admin_rounded_footer');
?> 
	    <?php print $renderedRoundedHeader; ?> 
	    <div id="narrow-search-filters" class="narrow-search-filters" data='<?php print "{".$filter_set_list."}";?>'>   
	    <table id = '<?php print $html_id; ?>' class="region-sidebar-widget-bg narrow-search-filters-holder">
		  <tbody>
			    <tr>
			      <td style="padding:0px;">
			            <div class="narrow-search-filtersets-holder" id="narrow-search-filtersets-holder">    
			              <div class='srch-filter-heading'><?php print t($title); ?></div>
			              <?php $lastIndex = count($rendered_filtersets_list) - 1; $currIndex = 0; ?>
			              <?php foreach ($rendered_filtersets_list as $renderedFilterset): ?>
			                <?php if ($currIndex == $lastIndex): ?>
			                  <div class = 'narrow-search-filterset-lastitem'>
			                <?php else : ?>
			                  <div>
			                <?php endif; ?>
			                      <?php print $renderedFilterset ?>
			                  </div>
			                <?php $currIndex++; ?>
			              <?php endforeach; ?>
			            </div>
				 </td>
			   </tr>
		 </tbody>
		</table>
	    </div>
	    <?php print $renderedRoundedFooter ?>		
<?php  } else {?>
			<div id="narrow-search-filters" class="narrow-search-filters" data='<?php print "{".$filter_set_list."}";?>'>   
	    </div>
<?php }?>	    
	   
	 
