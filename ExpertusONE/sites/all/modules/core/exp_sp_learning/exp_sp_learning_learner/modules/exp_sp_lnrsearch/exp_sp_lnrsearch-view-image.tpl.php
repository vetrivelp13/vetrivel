<?php $getTypeTitle=getTypeTitle($results); if($results->object_type =='Class') { ?>  
	<div class="<?php print getTypeImageClass($results->delivery_type_code)?> vtip" title="<?php print t($getTypeTitle);?>"></div>
<?php } elseif($results->object_type =='Course'){  ?>
    <div class="<?php print getTypeImageClass('cre_sys_obt_crs')?> vtip" title="<?php print t('Course');?>"></div>
<?php }else { ?>
	<div class="<?php print getTypeImageClass($results->object_type)?> vtip" title="<?php print t($getTypeTitle);?>"></div>
<?php }?>