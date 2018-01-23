<?php global $theme_key; ?>
<div class='narrow-search-text-filter-container' id='narrow-search-text-filter-container'>
  <ul class="eol-search">
  	
  	<!--Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 -->
 <?php
  $tmp_max_width='300';
  $mng_loc_cls_name='';
  if(arg(0) == 'administration' && arg(1) =='manage' &&  arg(2) =='location'){
  	$tmp_max_width='500';
	$mng_loc_cls_name='manage_location_top_txt_filter'; //Added by ganeshbabuv on March 15th 2017 for Location UI filter #0073054 Search auto-suggestion UI needs to be improvised for Location filter
  } 
 if($theme_key == 'expertusoneV2') { ?>
  	<li class='eol-search-go'><a id='narrow-search-text-filter-go' href='javascript:void(0);' >&nbsp;</a></li>
    <li class='eol-search-input'>
      <span>      	 
        <input type='text' id='narrow-search-text-filter' ondrop="return false" class='ac_input <?php print $mng_loc_cls_name; ?>' value='<?php print t('LBL304'); ?>' name='searchword' size='25' maxlength='<?php print $tmp_max_width;?>' alt='search...' autocomplete='on'/>
      </span>
  <!-- <div class="eol-search-clearance"><span id="searchclear" ></span></div>-->
    </li>
 <?php } else { ?>
    <li class='eol-search-input'>
      <span>
        <input type='text' id='narrow-search-text-filter' ondrop="return false" class='ac_input <?php print $mng_loc_cls_name; ?>' value='<?php print mb_strtoupper(t('LBL304')); ?>' name='searchword' size='25' maxlength='<?php print $tmp_max_width;?>' alt='search...' autocomplete='on'/>
      </span>
    <!--  <div class="eol-search-clearance"><span id="searchclear" ></span></div>-->
    </li>
    <li class='eol-search-go'><a id='narrow-search-text-filter-go' href='javascript:void(0);' >&nbsp;</a></li>
<?php } ?>
  </ul>
</div> 
