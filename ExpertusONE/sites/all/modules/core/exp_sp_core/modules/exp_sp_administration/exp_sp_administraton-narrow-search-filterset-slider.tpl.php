<div id='<?php print $html_id; ?>' class='narrow-search-filterset'>
<ul>
	<li>
	    <span id='<?php print $code; ?>_filterset_title' class='cls-show ' onclick='$("<?php print '#' . $js_object_info['init_id']; ?>").data("<?php print $js_object_info['name']; ?>").showHide("<?php print $code; ?>_filterset_title", "<?php print $code; ?>_filterset");'>
	    
			<a class="narrow-search-filterset-heading"><?php print $title;?></a>
		</span>
	</li>
</ul>
<div id='<?php print $code; ?>_filterset' data-prefix="<?php print $prefix;?>" data-suffix="<?php print $suffix;?>" data-startval="<?php print $startval;?>" data-endval="<?php print $endval;?>">
		<div id="narrowsearchslider" class="narrow-text-container-cls" style="display: block;">
  <div id="narrowsearchslidervalue">
	 <div id='price_hideshow' class='narrowsearchslider-container-cls' style='display:block'>
	 <div id="value-slider-range-<?php print $code;?>" style="margin-left:8px;width: 145px;"></div>
	 <input  readonly type="text" id="value-slide-left-<?php print $code;?>"  class="narrowsearch-value-slide-left">
	 <input  readonly type="text" id="value-slide-right-<?php print $code;?>" class="narrowsearch-value-slide-right"/>
        </div>
      </div>
  </div>
  </div>
</div>
