<?php
if($block->region=="left" || $block->region=="content" || $block->region=="right"){?>
<div class="<?php print "block block-$block->module" ?>" id="<?php print "block-$block->module-$block->delta"; ?>">					
 	<div class="block-title-left">
 		<div class='block-title-right block-header'>
 			<div class='block-title-middle'>
 				<h2 style='width:auto'><?php print ucfirst(t($title))?></h2>
 			</div>
 		</div>
 	</div>
	<div class='front-block-content-utils'>
		<p>
			<a class='collapse-block open-block'>
				<span class='hide'>Show / Hide Block</span>
			</a>
		</p>
	</div>
	<div class="region-sidebar-widget-bg content"><div style='margin:0px;padding:10px;'><?php print t($content); ?></div></div>
</div>

<div class="block-footer-left">
	<div class="block-footer-right">
		<div class="block-footer-middle">&nbsp;</div>
	</div>
</div>
<?php }else {
 print $content;
}?>