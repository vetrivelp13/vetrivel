<?php
/**
 * @file
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $title: Block title.
 * - $content: Block content.
 * - $block->module: Module that generated the block.
 * - $block->delta: An ID for the block, unique within each module.
 * - $block->region: The block region embedding the current block.
 * - $edit_links: A list of contextual links for the block. It can be
 *   manipulated through the variable $edit_links_array from preprocess
 *   functions.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - block: The current template type, i.e., "theming hook".
 *   - block-[module]: The module generating the block. For example, the user
 *     module is responsible for handling the default user navigation block. In
 *     that case the class would be "block-user".
 *   - first: The first block in the region.
 *   - last: The last block in the region.
 *   - region-count-[x]: The position of the block in the list of blocks in the
 *     current region.
 *   - region-odd: An odd-numbered block of the list of blocks in the current
 *     region.
 *   - region-even: An even-numbered block of the list of blocks in the current
 *     region.
 *   - count-[x]: The position of the block in the list of blocks on the current
 *     page.
 *   - odd: An odd-numbered block of the list of blocks on the current page.
 *   - even: An even-numbered block of the list of blocks on the current page.
 *
 * Helper variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $edit_links_array: An array of contextual links for the block.
 * - $block_zebra: Outputs 'odd' and 'even' dependent on each block region.
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $block_id: Counter dependent on each block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $block_html_id: A valid HTML ID and guaranteed unique.
 *
 * @see template_preprocess()
 * @see zen_preprocess()
 * @see template_preprocess_block()
 * @see zen_preprocess_block()
 * @see zen_process()
 */
  global $theme_key;   
   $fullname= getPersonDetails($results->person_id,array('full_name'));
   $src_image_name='';
  if($results->entity_type== 'cre_sys_obt_cls' || $results->entity_type== 'Class') {  	
      $deliveryType = getClassTypeFromId($results->entity_id);
      $delivery_type_class= getTypeImageClass($deliveryType[0]->delivery_type);
      $classTitle  = getClassTypeFromId($results->entity_id);
      $entityTitle = $classTitle[0]->title; 
      $src_image_name='';
  }elseif($results->entity_type== 'cre_sys_obt_crt' || $results->entity_type== 'cre_sys_obt_trn' || $results->entity_type== 'cre_sys_obt_cur' || $results->entity_type== 'Curricula' || $results->entity_type== 'Learning Plan' || $results->entity_type== 'Certification'){  	
      $delivery_type_class= getTypeImageClass($results->entity_type);
      $tpTitle = getTPTypeFromId($results->entity_id);
      $entityTitle = $tpTitle[0]->title;
  }elseif ($results->entity_type== 'cre_sys_obt_usr' || $results->entity_type== 'User' ){  
  	   $userId = $results->entity_id;
  	   if($results->module_name == 'exp_sp_my_skill'){
  	     $userId = getUserIdFromSkillId($results->entity_id);
  	   }
  	   $entityTitle = $results->logged_user_action;
  	   //$entityTitle = ($results->functionality_name == 'deleteskilldetails') ? $results->old_value : $results->new_value;
       //$fullname= getPersonDetails($userId,array('full_name'));
       //$entityTitle = $fullname[full_name]; 
      if(!empty($results->profile_image_path)) { 
    		$headerProfileImage = file_create_url($results->profile_image_path);
    		$src_image_name = "<img src='$headerProfileImage' width='50' height='50' class='profile-img'>";
         }else{
         	if($theme_key == "expertusoneV2")
         		$src_image_name = "<img src='sites/default/files/pictures/expertusonev2_default_user.png' width='41' height='36' >";
         	else
    			$src_image_name = "<img src='sites/default/files/pictures/default_user.png' width='50' height='50' >";
       }
      
  }   
		?>
	     				   	
<div>  <div class="activity-seperate">
       
			<table cellspacing="2" cellpadding="2" border="0" class="myactivity-table">
			 <?php if($theme_key == 'expertusoneV2') {?>
			<tr>
			<td class="myprofile-image">
			<?php 
			switch($delivery_type_class){
				case 'wbt-icon':
					$delivery_type = t('Web-based');
				break;
				case 'ilt-icon':
					$delivery_type = t('Classroom');
				break;
				case 'vcl-icon':
					$delivery_type = t('Virtual Class');
				break;
				case 'vod-icon':
					$delivery_type = t('Video');
				break;
				case 'curr-icon':
					$delivery_type = t('Curricula');
				break;
				case 'cert-icon':
					$delivery_type = t('Certification');
				break;
				case 'lrn-pln-icon':
					$delivery_type = t('Learning Plan');
				break;
				default :
					$delivery_type = '';
				break;
			}
			?>
			<div class="<?php print $delivery_type_class;?> img-spacing <?php if(!empty($delivery_type)){?>vtip<?php }?>" title="<?php if(!empty($delivery_type)){ print $delivery_type;}?>">
			<?php if($delivery_type_class==""){?>
			<div class="user-list-border-img"><?php print $src_image_name;?></div>
			<?php }else{?>
			<?php print $src_image_name;?>
			<?php }?>
			</div></td>
			<td class="myactivity_title">
			<div class="limit-title-row">
			<?php //if($results->getuserid == 'emptyvalue'){$title_controler=64; }else{$title_controler=30; }?>
			<div class="links-username commanTitleAll"><span class='limit-title vtip' title="<?php print sanitize_data($entityTitle); ?>"><?php print $entityTitle;?></span></div>	</div>
			<div class="action-name"><?php print activityDisplayDetails($results->functionality_name);?></div>
			<div class="hourse-ago"><?php print timeDifferenceLoggedAction($results->created_on)?> <div>
			 </td>
			 <td style="display:none"><?php if($results->getuserid == 'emptyvalue' && $theme_key != "expertusoneV2"){?>
			<a class="activity-remove-link" onclick="$('#wizarad-myprofile-activity').data('myprofileactivity').callDeleteProcess(<?php echo $results->entity_id; ?>,'<?php echo $results->entity_type; ?>','<?php echo $results->functionality_name;?>',<?php echo $results->activityid;?>);"> <?php print t("LBL286");?></a>
			<?php }?></td>
			</tr>			
			
			<?php }else{?>
			<tr><td rowspan="2" class="myprofile-image"><div class="<?php print $delivery_type_class;?> img-spacing"><?php print $src_image_name;?></div></td>
			<td class="action-name"><?php print activityDisplayDetails($results->functionality_name);?> - </td>
			<div class="limit-title-row">
			<?php// if($results->getuserid == 'emptyvalue'){$title_controler=50; }else{$title_controler=30; }?>
			<td class="links-username"><span class='limit-title vtip' title="<?php print sanitize_data($entityTitle); ?>"><?php print $entityTitle;?></span></td></div>	
			<?php if($results->getuserid == 'emptyvalue' && $theme_key != "expertusoneV2"){?>
			<td><a class="activity-remove-link"  onclick="$('#wizarad-myprofile-activity').data('myprofileactivity').callDeleteProcess(<?php echo $results->entity_id; ?>,'<?php echo $results->entity_type; ?>','<?php echo $results->functionality_name;?>',<?php echo $results->activityid;?>);"> <?php print t("LBL286");?></a></td>
			<?php }?>
			</tr><tr>
			<td class="hourse-ago" colspan="2"><?php print timeDifferenceLoggedAction($results->created_on)?> <td>
			</tr>
			<?php }?>
			</table>
			
	 </div>
