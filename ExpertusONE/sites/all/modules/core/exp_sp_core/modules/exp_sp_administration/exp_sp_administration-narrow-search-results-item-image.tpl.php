<?php
//print $results->type;
global $theme_key;
	// #43473 - Multilanguage support added
	if($results->name == 'Department'){
		$displayTitle = t('LBL179');
	}else if($results->name == 'Employment Type'){
		$displayTitle = t('LBL174');
	}else if($results->name == 'Job Role'){
		$displayTitle = t('LBL133');
	}else if($results->name == 'Job Title'){
		$displayTitle = t('LBL073');
	}else if($results->name == 'User Type'){
		$displayTitle = t('LBL173');
	}
if($results->object_type =='People' || $results->object_type =='Order'||  $results->object_type =='announcement') {
  $account = user_load_by_name($results->username);
  $filepath = $account->picture->uri;

  if($filepath){
    $alt = t("@user's picture", array('@user' => format_username($account)));
    //$user_img_path = file_create_url($filepath);
    if($theme_key == 'expertusoneV2'){
    print $userimage ='<div class="admin-narrow-search-user user-list-border-img"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url($filepath).'" width="36px" height="36px" alt=""/></div>';
    }else{
      print $userimage ='<div class="admin-narrow-search-user"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url($filepath).'" width="32px" height="32px" alt=""/></div>';
    }
    }else{
  	if($theme_key == 'expertusoneV2') {
	print '<div class="admin-narrow-search-user user-list-border-img"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url('sites/default/files/pictures/expertusonev2_default_user.png').'" width="36px" height="36px" alt=""/></div>';
  	} else {
  	print '<div class="admin-narrow-search-user"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url('sites/default/files/pictures/default_user.png').'" width="32px" height="32px" alt=""/></div>';
  	}
  }
}else if($results->object_type =='Course' || $results->object_type =='Location' || $results->object_type =='Facility' || $results->object_type =='Classroom' || $results->object_type =='Equipment') { ?>
	<div class="crs-icon " title="<?php print getTypeTitle($results)?>"></div>
<?php
}else if($results->object_type =='Content') { ?>
  <div class="<?php print getContentTypeImageClass($results->content_type)?>" title="<?php print $results->type; ?>"></div>
<?php }else if($results->object_type =='Content Author') { ?>
  <div class="<?php print $results->video_type; ?>" title="<?php print $results->title; ?>"></div>
<?php }else if($results->object_type =='commercesetting') { ?>
  <div class="<?php print $results->imagepath;?>" title="<?php print $results->display_name; ?>"></div>
<?php }else if($results->object_type =='moduleinfo') { ?>
  <div class="<?php print $results->imagepath;?>" title="<?php print $results->display_name; ?>"></div>
<?php }else if($results->object_type =='listvalues') { ?>
  <div class="<?php print $results->imagepath;?>" title="<?php print $displayTitle; ?>"></div>
<?php }else if($results->object_type =='config') { ?>
  <div class="<?php print $results->imagepath;?>" title="<?php print $results->display_name; ?>"></div>
<?php }
 else if($results->object_type =='bannertype' ) {
  $bannerImage = (checkFrontSidebarExist()) ? $results->banner_thumbnail : $results->banner_large; // Check image based on the front sidebar status
  if(!empty($bannerImage) && $bannerImage !=''){
    print $bannerimage ='<div class="admin-narrow-search-user"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url($bannerImage).'" width="50px" height="46px" alt=""/></div>';
   }else{
     print $bannerimage ='<div class="admin-narrow-search-user"><img id="header-profile-img" style="vertical-align:bottom;" src="'.file_create_url('sites/default/files/pictures/default_bannerimage.png').'" width="50px" height="46px" alt=""/></div>';
   }

}else if($results->object_type =='paymentmethod') {
	$iconCls = ($theme_key == 'expertusoneV2') ? 'admin-icon ' .$results->id : 'payment-option ' .$results->id ;
	 ?>
	<div class="<?php echo $iconCls; ?>" ></div>
<?php
}
else if($results->type =='Survey') { ?>
	<div class="sur-icon" ></div>
<?php
}
else if($results->type =='Assessment') { ?>
	<div class="ass-icon" ></div>
<?php
}
else {
	//75499: admin: course/class - should be removed delivery type and add delivery type to icons
	$results->delivery_type_name = $results->delivery_type;
	?>	
	<div class="<?php print getTypeImageClass($results->delivery_type_code); print ' vtip';?>" title="<?php print getTypeTitle($results);?>"></div>
<?php }?>
