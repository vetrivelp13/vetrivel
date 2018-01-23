<?php expDebug::dPrint('narrow search sortbar tpl file : $links_list = ' . print_r($links_list, true),4); ?>
<?php expDebug::dPrint('narrow search sortbar tpl file : $sortbar_list = ' . print_r($sortbar_list, true),4); ?>
<?php expDebug::dPrint('narrow search sortbar tpl file : $links_list = ' . print_r($title, true),4); ?>
<?php expDebug::dPrint('narrow search sortbar tpl file : $links_list = ' . print_r($sortbar_enable, true),4); ?>
<?php global $theme_key;
if($sortbar_enable == true){ ?> 
  	<!-- <img style='width:24px;height:24px;padding-top:5px;cursor:pointer;' onclick='drawCalendar();' src='/sites/all/themes/core/expertusoneV2/images/calendaricon.png'></img>-->
  	<?php
  		$calendar_view_arr = array('catalog','user','location','announcement','order');
  		if(module_exists('exp_sp_admincalendar') && in_array(arg(2), $calendar_view_arr)){  		  
  	?>
	<a href="javascript:void(0)" onclick="drawCalendar();" id="calendar-view-icon"><span class="calendar-view-icon vtip" title="<?php print t('ACLBL0026')?>"> </span></a>
	<?php }?>
	
<div id="<?php print $html_id; ?>" class="narrow-search-sortbar" data='<?php print "".$sortbar_list."";?>' >
  <div class="narrow-search-sortbar-options">
  	
    <span class="narrow-search-sortbar-sorttext" <?php if($theme_key == 'expertusoneV2'): ?> onclick = 'sortTypeToggle("narrow-search-sortbar-sortlinks");' <?php endif;?>><?php print $title; ?> <?php if($theme_key != 'expertusoneV2'): ?>:<?php endif;?> <span class="find-trng-sort-arrow-icon"></span></span>
    <ul class="narrow-search-sortbar-sortlinks">
      <?php $firstLink = true; ?>
      <?php foreach ($links_list as $link): ?>
        <?php if ($firstLink): ?>
	        <li class="first">
	      <?php else: ?>
	        <li>
	      <?php endif; 
	      if($theme_key == 'expertusoneV2') {?>
	        <a id='<?php print $link['html_id']; ?>' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").sortSearchResults("<?php print $link['sort_type']; ?>", "<?php print $link['html_id']; ?>"); sortTypeToggle("narrow-search-sortbar-sortlinks");'>
	      <?php }else{?>
	        <a id='<?php print $link['html_id']; ?>' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").sortSearchResults("<?php print $link['sort_type']; ?>", "<?php print $link['html_id']; ?>");'>
	      <?php }?>
		      <?php print $link['title']; ?>
		    </a>
		  </li>
		<?php $firstLink = false; ?> 
	  <?php endforeach; ?>
    </ul>
  </div>
</div>
<?php }else{?>
           <div id="<?php print $html_id; ?>" data='<?php print "".$sortbar_list."";?>' >
           </div>
<?php }?>
