<?php
$location  = '';
if(!empty($results->user_city)){
  $location  = $results->user_city;
}
if(!empty($location) && !empty($results->user_state)){
  $location .= ', '.$results->user_state;
}else{
  $location .= $results->user_state;
}

$name = $results->skill_name;
$dataProfile="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/".core_encrypt($results->user_id)."','popupDispId':'profile-qtipid-".$results->user_id."','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_".$results->user_id."','dataloaderid':'lnr-myteam-search','dataIntId':'lnrmyteamsearch','wBubble':'350','hBubble':'auto'}";


expDebug::dPrint('MyApproval Results array : ' . print_r($results, true),4);

$showPipe = false;
$skill_created_on_date  = explode(" ",$results->created_on);
$skill_added_on_dformat = date_format(date_create($skill_created_on_date[0]),'F d, Y');
$cert_verify_status = $results->verification_status;
$cert_verify_status_lc = strtolower($results->verification_status);
if($cert_verify_status_lc == "not verified" || $cert_verify_status_lc == "verification pending"){
    $status = explode(" ", $cert_verify_status_lc);
    $cert_verify_status_format = $status[0].'-'.$status[1];
}
else{
    $cert_verify_status_format = $cert_verify_status_lc;
}
expDebug::dPrint("verification_status ".$cert_verify_status,5);
expDebug::dPrint("verification_status format ".$cert_verify_status_format,5);

$fileName = basename($results->certificate_img);
$fileFormat = pathinfo($fileName, PATHINFO_EXTENSION);


if($results->valid_from != "0000-00-00" && $results->valid_to != "0000-00-00"){
    $showPipe = true;
    $skill_valid_from_date  = explode(" ",$results->valid_from);
    $skill_valid_from_dformat = date_format(date_create($skill_valid_from_date[0]),'F d, Y');
    $skill_valid_to_date  = explode(" ",$results->valid_to);
    $skill_valid_to_dformat = date_format(date_create($skill_valid_to_date[0]),'F d, Y');
    $display_format = t('Valid from').' '.$skill_valid_from_dformat.' '.strtolower(t('LBL621')).' '.$skill_valid_to_dformat;
    $validity =  $results->start_date.' '.t('LBL621').' '.$results->end_date;
}
else if($results->valid_from != "0000-00-00" && $results->valid_to == "0000-00-00"){
    $showPipe = true;
    $skill_valid_from_date  = explode(" ",$results->valid_from);
    $skill_valid_from_dformat = date_format(date_create($skill_valid_from_date[0]),'F d, Y');
    $display_format = t('Valid from').' '.$skill_valid_from_dformat;
    $validity =  t('LBL649').' '.$results->start_date;
}
else{
    $showPipe = false;
    $display_format = NULL;
    $validity = NULL;
}
$usr_nme = ucwords($results->full_name);
expDebug::dPrint("Full Name ".$usr_nme,5);
expDebug::dPrint(" display_format ".$display_format,5);
expDebug::dPrint("created on date ".$results->created_on,5);
expDebug::dPrint("created on dateFormat ".$skill_added_on_dformat,5);
expDebug::dPrint("Valid from date ".$results->valid_from,5);
expDebug::dPrint("Valid from dateFormat ".$skill_valid_from_dformat,5);
expDebug::dPrint("Valid to date ".$results->valid_to,5);
expDebug::dPrint("Valid from dateFormat ".$skill_valid_to_dformat,5);

$skillTitle = sanitize_data($results->skill_name);

?>
<div id='<?php echo "qtipprofileqtip_visible_disp_".$results->user_id."_disp";?>'></div>
<div class="top-record-div-left">
	<div class="myteam-title-Div limit-title-row">
	<!-- Change the user profile list onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").showUserProfile("<?php print $results->user_id;?>","<?php print $name;?>"); -->
    	<span class="myteam-user-title limit-title vtip" title ='<?php echo addslashes($skillTitle); ?>' onclick = '$("#lnr-myteam-approval").data("lnrmyteamapproval").ShowCertificateDet("<?php print addslashes($results->skill_name);?>","<?php print addslashes($results->company);?>","<?php print addslashes($results->certificate_number);?>","<?php if(!empty($validity)){print $validity;}else {print '';}?>","<?php if(!empty($fileName)){print addslashes($fileName);}?>");' title="<?php print addslashes($name);?>">
    	<?php //print $results->last_name.', '.$results->first_name; ?>
    	<?php print $name; ?>
    	</span>
	</div>
	<div class="narrow-search-results-item-detail content-detail-code view-certificate-grid PadBWithDesc">
	       <span class="vtip" title="<?php print t($usr_nme);?>">
		      <div class="add-certificate-date line-item-container float-left">
		          <?php 
		          print titleController('MY-APPROVAL-UNAME',t($usr_nme));
		         // print t($usr_nme);?>
		      </div>
		  </span>
	       
		  <span class="vtip" title="<?php print t('Added on').' '.$skill_added_on_dformat;?>">
		      <div class="add-certificate-date line-item-container float-left">
		          <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
		          <?php 
		          print titleController('MY-APPROVAL-ADDED-ON-DATE',(t('Added on').' '.$skill_added_on_dformat));
		          //print t('Added on '.$skill_added_on_dformat);?>
		      </div>
		  </span>
		  <?php if($showPipe){?>
		     <!--  <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span> -->
		 <?php }?>
		  <span class="vtip" title="<?php print t($display_format);?>">
		      <div class="add-certificate-expiredate line-item-container float-left">
		         <?php if(!empty($display_format)){?>
		             <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
		             <?php 
		             print titleController('MY-APPROVAL-EXPIREDATE',t($display_format));
		         }?>
		      </div>
		  </span>
		</div>
		<?php if(empty($results->certificate_img)){?>
		<div class="cls-crt-rowcontainer float-left"><div class="access-tab-icon certificate-icon-disable float-left"></div><div class="view_certificate certificate-view-disable float-left"><?php print t('LBL205');?></div></div>
		<?php }
		else{?>
		<div class="cls-crt-rowcontainer float-left"><div class="access-tab-icon float-left"></div><div class="view_certificate vtip float-left" onclick = '$("#lnr-myteam-approval").data("lnrmyteamapproval").viewCertificate("<?php print addslashes($results->skill_name);?>","<?php print addslashes($results->certificate_img);?>","<?php print addslashes($fileFormat);?>")';  title="<?php print t('LBL205');?>"><?php print t('LBL205');?></div></div>
		<?php }?>
	
	<div style="display:none;" class="myteam-class-loaderimg profile-loader" id="user-detail-loader-<?php print $results->drup_user_id;?>"><span class="ui-accordion-header"></span></div>
	<div style="display:none;" class="myteam-class-loaderimg" id="user-detail-<?php print $results->user_id;?>"><span class="ui-accordion-header"></span></div>
</div>