<?php if($results->object_type =='Class') { ?>  
	<div class="<?php print getTypeImageClass($results->delivery_type_code)?>" title="<?php print t(getTypeTitle($results));?>"></div>
<?php } else { ?>
	<div class="<?php print getTypeImageClass($results->object_type)?>" title="<?php print t(getTypeTitle($results));?>"></div>
<?php }?>