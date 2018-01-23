<?php expDebug::dPrint('narrow search filterset date range tpl file : $code = ' . print_r($code, true),4);?>
<?php global $theme_key; ?>
<div id='<?php print $html_id; ?>' class='narrow-search-filterset'>
  <ul>
     <li>
       <span id='<?php print $code; ?>_filterset_title' class='cls-show ' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
         <a class='narrow-search-filterset-heading'><?php print $title; ?></a>
        <!--   <span> &nbsp;&nbsp;(<?php /*print $title_description;*/// print t("LBL043"); ?>)</span> -->
       </span>
       <span id='<?php print $code; ?>-daterange-clr' class='clr-txt' style='display:none' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").clearDateRangeFields("<?php print $code; ?>", "<?php print $code; ?>-narrow-search-filterset-daterange");'><?php print t("LBL307"); ?></span>
     </li>
  </ul>
  <div id='<?php print $code; ?>_filterset' >
    <div class='narrow-search-filterset-daterange-container'>
    <?php if($theme_key == 'expertusoneV2') { ?>
      <div class='narrow-search-filterset-daterange-date-container'>
      	<div class="filter-search-start-date-left-bg"></div>
        <input type='text' class='narrow-search-filterset-daterange-date filter-search-start-date-middle-bg'
                 id='<?php print $code;?>-daterange-from-date' ondrop="return false" name='<?php print $code; ?>-daterange-from-date'
                   value='<?php /*print $from_default_text;*/ print t("LBL220"); ?>' data="{dateFieldCode:'<?php print $code; ?>',dateRangeShowOption:'<?php print $dateRangeShowOption;?>'}">
      <div class="filter-search-start-date-right-bg"></div>
      </div>                     
      <div class='narrow-search-filterset-daterange-date-container'>
        <div class="filter-search-start-date-left-bg"></div>
        <input type='text' class='narrow-search-filterset-daterange-date filter-search-start-date-middle-bg' 
                 id='<?php print $code; ?>-daterange-to-date' ondrop="return false" name='<?php print $code; ?>-daterange-to-date'
                   value='<?php /*print $to_default_text;*/ print t("LBL221"); ?>'  data="{dateFieldCode:'<?php print $code; ?>',dateRangeShowOption:'<?php print $dateRangeShowOption;?>'}" >
       <div class="filter-search-start-date-right-bg"></div>
      </div>
      <div id= '<?php print $code; ?>-daterange-errmsg' class='narrow-search-filterset-daterange-errmsg narrow-search-filterset-daterange-errmsg-hide'></div>
      <div class='narrow-search-filterset-daterange-go-button-container'>
       <div class="curved-blue-button-left"></div>
        <span class='narrow-search-filterset-daterange-go-button-img' id="<?php print $code; ?>-narrow-search-filterset-daterange" data-default-fromtext="<?php print $from_default_text;?>"  
          data-default-totext="<?php print $to_default_text;?>"  
          onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").validateDateRangeDates("<?php print $code; ?>", "<?php print $from_default_text; ?>", "<?php print $to_default_text; ?>");'><?php print t("LBL669"); ?></span>        
        <div class="curved-blue-button-right"> </div>
      </div>
       <?php } else { ?>
       	<div class='narrow-search-filterset-daterange-date-container'>
        <input type='text' class='narrow-search-filterset-daterange-date'
                 id='<?php print $code; ?>-daterange-from-date' ondrop="return false" name='<?php print $code; ?>-daterange-from-date'
                   value="<?php /*print $from_default_text;*/ print t("LBL220"); ?>" data="{dateFieldCode:'<?php print $code; ?>',dateRangeShowOption:'<?php print $dateRangeShowOption; ?>'}">
      </div>                     
      <div class='narrow-search-filterset-daterange-date-container'>
        <input type='text' class='narrow-search-filterset-daterange-date' 
                 id='<?php print $code; ?>-daterange-to-date' ondrop="return false" name='<?php print $code; ?>-daterange-to-date'
                   value="<?php /*print $to_default_text;*/ print t("LBL221"); ?>"  data="{dateFieldCode:'<?php print $code; ?>',dateRangeShowOption:'<?php print $dateRangeShowOption;?>'}" >
      </div>
      <div id= '<?php print $code; ?>-daterange-errmsg' class='narrow-search-filterset-daterange-errmsg narrow-search-filterset-daterange-errmsg-hide'></div>
      <div class='narrow-search-filterset-daterange-go-button-container'>
       <div class="curved-blue-button-left"></div>
        <span class='narrow-search-filterset-daterange-go-button-img' id="<?php print $code; ?>-narrow-search-filterset-daterange" 
          data-default-fromtext="<?php print $from_default_text;?>"  
          data-default-totext="<?php print $to_default_text;?>"            
          onClick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").validateDateRangeDates("<?php print $code; ?>", "<?php print $from_default_text; ?>", "<?php print $to_default_text; ?>");'><?php print t("LBL669"); ?></span>        
       <div class="curved-blue-button-right"></div>
      </div>
      <?php } ?> 	
    </div>
  </div>
</div>