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

?>
<?php 
	global $theme_key;
	$num_rows = count($results);
	if($num_rows > 0){
		$userInfo     = $results[0][0];
		$profilePath  = $results[1];
	}
?>
<script> window.onload = "test();"
function test () {
	$('').css('height','auto');
}

</script>

<!-- <div class="<?php print $classes; ?>">
	<div class="region-sidebar-widget-bg">
		<div class="content"> -->
		<div class="account-details-view-container">
		     <table cellspacing="2"  cellpadding="2" class="my-account-box myAccount">
		     	<tbody>
		     		<tr>
		     			<td class="field-title"><?php print t("LBL056")?>: </td>
		     			<td class="field-value"><?php print titleController('MY-ACCOUNT-FIRST-NAME', $userInfo->person_first_name); ?></td>
		     		</tr>
		       <?php /*	<tr>
			     		<td class="field-title"><?php print t("LBL057")?>:</td>
			     		<td class="field-value"><?php print $userInfo->person_middle_name; ?></td>
			     	</tr> */ ?>
		     		<tr>
		     			<td height='1px' class="field-title"><?php print t("LBL058")?>: </td>
		     			<td height='1px' class="field-value"><?php print titleController('MY-ACCOUNT-LAST-NAME',$userInfo->person_last_name); ?></td>
		     		</tr>
		     		<tr>
		     			<td class="field-title"><?php print t("LBL054")?>: </td>
		     			<td class="field-value"><?php print $userInfo->person_user_name; ?></td>
		     		</tr>
		     		<!-- 
						 * To handle Sql Injection without session cookie (anonymous users) 
						 * Condition Added by : Vimal DM
					-->
		     		<?php /* Ram : Commmented Password Code is Remove To This Ticket #0039920 */ ?>
		     		 <tr>
		     			<td class="field-title"><?php print t("LBL060")?>:</td>
		     			<td class="field-value">**********</td>
		     		</tr>
		     		<tr>
		     			<td class="field-title"><?php print t("LBL061")?>: </td>
		     			<td class="field-value"><?php print $userInfo->person_email; ?></td>
		     		</tr>
                                
                                <?php //Get Address Details
                                    $addr1 = $userInfo->person_addr1;
                                    $addr2 = $userInfo->person_addr2;
                                    $city = $userInfo->person_city;
                                    $zip = $userInfo->person_zip;
                                    $countryName = getCountryName($userInfo->person_country);
                                    $stateName = getStateName($userInfo->person_state,$userInfo->person_country);
                                ?>
                                <tr>
		     			<td id="view-addr1" class="field-title"><?php print t("LBL727")?>: </td>
                                <?php if(!empty($addr1)){?>
		     			<td class="field-value"><?php print titleController('MY-ACCOUNT-ADDRESS1',$addr1); ?></td>
		     		<?php } else if(empty($addr1) && !empty($addr2)) { ?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-ADDRESS2',$addr2); ?></td>
                                <?php } else if(empty($addr1) && empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){ ?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-CITY',trim(str_replace(', ,',',',($city.', '.$stateName.', '.$zip)),', ')); ?></td>
		     		<?php } else if(empty($addr1) && empty($addr2) && (empty($city) && empty($stateName) && empty($zip))){ ?>
                                    <td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                <?php } //else if(empty($addr1) && !empty($addr2) && (empty($city) && empty($stateName) && empty($zip)) && empty($countryName)){ ?>
                                </tr>
                                <tr>                                    
                                <td id="view-addr2" class="field-title"></td>
                                <?php if(!empty($addr1) && empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){ ?>
                                    <td class="field-value"><?php print titleController('MY-ACCOUNT-CITY',trim(str_replace(', ,',',',($city.', '.$stateName.', '.$zip)),', ')); ?></td>
                                <?php } else if(!empty($addr1) && empty($addr2) && empty($city) && empty($stateName) && empty($zip) && !empty($countryName)){ ?>
                                    <td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                               <?php } else if(!empty($addr1) && !empty($addr2)){ ?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-ADDRESS2',$addr2); ?></td>
                                <?php } else if(empty($addr1) && empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){ ?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                <?php } else if(empty($addr1) && !empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-CITY',trim(str_replace(', ,',',',($city.', '.$stateName.', '.$zip)),', ')); ?></td>
                                <?php } ?>
                                </tr>
                                <tr>
                                    <td id="view-city-state-zip" class="field-title"></td>
                                <?php if(!empty($addr1) && !empty($addr2) && empty($city) && empty($stateName) && empty($zip)&& !empty($countryName)){ ?>
		     			<td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                <?php } else if(!empty($addr1) && !empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){ ?>
                                   <td class="field-value"><?php print titleController('MY-ACCOUNT-CITY',trim(str_replace(', ,',',',($city.', '.$stateName.', '.$zip)),', ')); ?></td>
                                <?php } else if(empty($addr1) && !empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                <?php } else if(!empty($addr1) && empty($addr2) && (!empty($city) || !empty($stateName) || !empty($zip))){?>
                                <td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                <?php } ?>
                                </tr>
                                <tr>
                                    <td id="view-country" class="field-title"></td>
                                <?php
                                if(!empty($addr1) && !empty($addr2)  && (!empty($city) || !empty($stateName) || !empty($zip))) { ?>
		     			<td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                </tr>
                               <?php } ?>
                                
		     	<?php  $config = getConfig("exp_sp"); 
		     	  if($config['sms_api_flag'] == 1){ ?>	
		     		<tr>
		     			<td class="field-title"><?php print t("Mobile")?>: </td>
		     			<td class="field-value"><?php print $userInfo->person_mobile_no; ?></td>
		     		</tr>
		     		<?php if(!empty($userInfo->person_mobile_no)){?>
		     	    <tr>
                        <td></td>
                        <td class="field-value learner-send-me-training-new">
                        <?php if($theme_key == 'expertusoneV2'){?>
                        	<div <?php if($userInfo->person_sms_alert == 'Y'){?> class="checkbox-selected" <?php }else{?> class="checkbox-unselected" <?php } ?> >
                        	<input class="send-me-training-new" type="checkbox" disabled <?php if($userInfo->person_sms_alert == 'Y'){?> checked = "checked" <?php } ?> />
                        	</div>
                        <?php }else{?>
                            <input class="send-me-training-new" type="checkbox" disabled <?php if($userInfo->person_sms_alert == 'Y'){?> checked = "checked" <?php } ?> /> 
                        <?php }?>
                            <span class="field-yes smsalert-check-yes-view"><?php print t("LBL1234")?></span>
                        </td>
                    </tr>
                    <?php } ?>
              <?php }?>
		     		<!-- <tr>
		     			<td class="field-title"><?php print t("LBL062")?>:</td>
		     			<td class="field-value">
		     				<?php //if(!empty($profilePath)) { 
		     				      //$headerProfileImage = file_create_url($profilePath);
		     				?>
		     				   	<img style="vertical-align:bottom;" src="<?php print file_create_url($profilePath); ?>">
		     				<?php //}else { 
		     				      //$headerProfileImage = "sites/default/files/pictures/default_user.png";
		     				?>
		     					<img style="vertical-align:bottom;" src="sites/default/files/pictures/default_user.png">
		     				<?php //} ?>
		     			</td>
		     		</tr> 
		     		<tr>
                        <td class="field-title"></td>
                        <td class="field-value learner-send-me-training-new"><input DISABLED class="send-me-training-new" type="checkbox" <?php //print $userInfo->subcripe == 'Y' ? 'checked' : '';  ?>/> <span><?php //print t("LBL150") ?></span></td>
                    </tr>	-->     		
		     	</tbody>
		     </table>
		 </div>    		     
		 <!--   </div>
	</div>	
</div>-->
<!--  <script>
//document.getElementById("header-profile-img").src = "<?php // print $headerProfileImage; ?>"
</script> -->
<!-- /.block -->
