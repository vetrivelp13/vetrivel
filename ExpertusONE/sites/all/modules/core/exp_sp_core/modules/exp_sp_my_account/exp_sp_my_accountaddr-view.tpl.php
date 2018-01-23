<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
 
	global $theme_key;
	$num_rows = count($results);
	if($num_rows > 0){
		$userInfo     = $results[0][0];
		$profilePath  = $results[1];
	}
?>

<!-- <div class="<?php print $classes; ?>">
	<div class="region-sidebar-widget-bg">
		<div class="content"> -->
		     <table cellspacing="2"  cellpadding="2" id="no-edit-addr" class="my-account-box myAccount">
		     	<tbody>
		     		
                                <?php //Get Address Details
                                    $addr1 = $userInfo->person_addr1;
                                    $addr2 = $userInfo->person_addr2;
                                    $city = $userInfo->person_city;
                                    $zip = $userInfo->person_zip;
                                    $countryName = getCountryName($userInfo->person_country);
                                    $stateName = getStateName($userInfo->person_state,$userInfo->person_country);
                                ?>
                                <tr>
		     			<td class="field-title"><?php print t("LBL727")?>: </td>
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
                                <td class="field-title"></td>
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
                                    <td class="field-title"></td>
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
                                    <td class="field-title"></td>
                                <?php
                                if(!empty($addr1) && !empty($addr2)  && (!empty($city) || !empty($stateName) || !empty($zip))) { ?>
		     			<td class="field-value"><?php print titleController('MY-ACCOUNT-COUNTRY',$countryName); ?></td>
                                </tr>
                               <?php } ?>
		     	     		
		     	</tbody>
		     </table>		     
		 <!--   </div>
	</div>	
</div>-->
<!--  <script>
//document.getElementById("header-profile-img").src = "<?php // print $headerProfileImage; ?>"
</script> -->
<!-- /.block -->
