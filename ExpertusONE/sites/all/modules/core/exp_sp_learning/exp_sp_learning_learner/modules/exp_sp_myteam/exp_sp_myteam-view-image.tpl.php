<?php 
 global $theme_key; 
 if($theme_key == 'expertusoneV2') {
 $user_picture = ($results->user_picture!='')?file_create_url($results->user_picture):'sites/default/files/pictures/expertusonev2_default_user.png';
 }
 else
 {
  $user_picture = ($results->user_picture!='')?file_create_url($results->user_picture):'sites/default/files/pictures/default_user.png';
 }
 ?>
<?php if($theme_key == 'expertusoneV2') {?>
<div class="myteam-profile-image user-list-border-img">
<img width="41" height="36" src="<?php print $user_picture;?>" class="vtip" title="<?php print $results->full_name; ?>" />
</div>
<?php }else{?>
<div class="myteam-profile-image">
<img width="32" height="32" src="<?php print $user_picture;?>" class="vtip" title="<?php print $results->full_name; ?>" />
</div>
<?php }?>