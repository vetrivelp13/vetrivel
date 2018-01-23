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
 $data= "{'entityId':'".$results->skill_id."','entityType':'enr_crs_usr','url':'learning/ajax/skill-set/add-skill/edit/".$results->skill_id."','popupDispId':'skill-qtipid-".$results->skill_id."','qtipDisplayPosition':'tipfaceTopMiddle','catalogVisibleId':'qtipskillqtip_visible_disp_".$results->skill_id."','entityType':'enr_crs_usr','qtipClass':'display-message-positioning','wBubble':'390','hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
?>
<?php expDebug::dPrint('narrow search sortbar check profile pageee' . print_r($results, true),4);
expDebug::dPrint('getMySkillDetails : obj sandhyaaaa ' . print_r($results, true) , 3); 
expDebug::dPrint('Results array : ' . print_r($results, true),4);

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


if($results->valid_from != "0000-00-00" && $results->valid_to != "0000-00-00"){
    $showPipe = true;
    $skill_valid_from_date  = explode(" ",$results->valid_from);
    $skill_valid_from_dformat = date_format(date_create($skill_valid_from_date[0]),'F d, Y');
    $skill_valid_to_date  = explode(" ",$results->valid_to);
    $skill_valid_to_dformat = date_format(date_create($skill_valid_to_date[0]),'F d, Y');
    $display_format = t('Valid from').' '.$skill_valid_from_dformat.' '.strtolower(t('LBL621')).' '.$skill_valid_to_dformat;
    
    $from_date = date_format(date_create($skill_valid_from_date[0]),'d-m-Y');
    $to_date = date_format(date_create($skill_valid_to_date[0]),'d-m-Y');
    $validity =  $from_date.' '.t('LBL621').' '.$to_date;
}
else if($results->valid_from != "0000-00-00" && $results->valid_to == "0000-00-00"){
    $showPipe = true;
    $skill_valid_from_date  = explode(" ",$results->valid_from);
    $skill_valid_from_dformat = date_format(date_create($skill_valid_from_date[0]),'F d, Y');
    $display_format = t('Valid from').' '.$skill_valid_from_dformat;
    $from_date = date_format(date_create($skill_valid_from_date[0]),'d-m-Y');
    $validity =  t('LBL649').' '.$from_date;
}
else{
    $showPipe = false;
    $display_format = NULL;
    $validity = NULL;
}

$fileName = basename($results->certificate_img);
$fileFormat = pathinfo($fileName, PATHINFO_EXTENSION);

expDebug::dPrint(" display_format ".$display_format,5);
expDebug::dPrint("created on date ".$results->created_on,5);
expDebug::dPrint("created on dateFormat ".$skill_added_on_dformat,5);
expDebug::dPrint("Valid from date ".$results->valid_from,5);
expDebug::dPrint("Valid from dateFormat ".$skill_valid_from_dformat,5);
expDebug::dPrint("Valid to date ".$results->valid_to,5);
expDebug::dPrint("Valid from dateFormat ".$skill_valid_to_dformat,5);


?>
<div id="<?php echo 'skill-qtipid-'.$results->skill_id;?>"></div>	
   				   	
	<?php if($results->colheader == 0){?>
<?php //if($theme_key == "expertusoneV2"){?>
<!--<div class="addSkillHeader">
  			      			
</div>
--><?php if($theme_key != "expertusoneV2"){?>  	
<div>
	<div class="skill-seperate">
	<div class="myskill-table">
		<div class="myskill-first-header">
			<div class="skill-header skillname-set"><?php echo t('LBL833');?></div>
			<div class="skill-header proftitle-set"><?php echo t('LBL835');?></div>
			<?php if($results->getuserid == 'emptyvalue'){?>
			<div class="skill-header dsiplay-other-set"><?php echo t('LBL834'); ?></div>
			<div class="clearBoth"></div>
	    </div>
	</div>
	</div>
</div>
			<?php } ?>
		<?php }?>
		<?php }?>
<div class="add-skill-recordlist"> 
    <?php // For getting updated TP title. ticket: 0023049 //
    if($results->getuserid == 'emptyvalue'){ $title_controler = 58;} else {$title_controler = 26; } 
    if(empty($results->programid)){
     //$skillTitle = $results->skill_name;
      $skillTitle = sanitize_data($results->skill_name);
      
    }else{
    //$skillTitle = $results->title;
     $skillTitle = sanitize_data($results->title);
      
    }
    ?>
	<div class="skill-seperate">
	<div class="myskill-table">
		<div class="myskill-first-row-list">
		<span>
			<div class="skillname-set">
				<div class="limit-title-row titlePadBot">
				<?php if($results->type == 'certificate'){?>
        				<span class="limit-title skill-columns-certificate vtip" title ='<?php echo addslashes($skillTitle); ?>' onclick = '$("#wizarad-myprofile-skills").data("myprofileskills").ShowCertificateDet("<?php print addslashes($results->skill_name);?>","<?php print addslashes($results->company);?>","<?php print addslashes($results->certificate_number);?>","<?php if(!empty($validity)){print $validity;}else {print '';}?>","<?php if(!empty($fileName)){print addslashes($fileName);}?>");' title="<?php print addslashes($name);?> ">
        				<?php echo $skillTitle; ?>
        				</span>
        	   <?php } else{?>
				        <span class="limit-title skill-columns vtip" title ='<?php echo addslashes($skillTitle); ?>'>
        				<?php echo $skillTitle; ?>
        				</span>
        		<?php }?>
			</div>
			</div>
		<!--  	<div class="skill-columns proftitle-set">
				<span class="rating-static rating_<?php echo $results->proficiency;?> vtip" title="<?php print t('Rating'); ?>"></span><?php //echo $results->experience;?>
			</div> -->
			<?php if($results->getuserid == 'emptyvalue'){?>
			
			<?php if($theme_key == "expertusoneV2"){?>
			
			
				<div class="edit-delete-set">
				     <?php
				 expDebug::dPrint('narrow search sortbar check profile pageee' . print_r($results->type, true),4);
				      if($results->type == "certificate"){?>
				<span>
				<div class="skills-edit-delete skill-edit-div vtip" title="<?php print t('LBL063'); ?>" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').openCertificateDialog('<?php echo $results->skill_id;?>','edit');">
				     	<?php } else{?>
				<div class="skills-edit-delete skill-edit-div vtip" title="<?php print t('LBL063'); ?>" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').openSkillDialog('edit',<?php echo $results->skill_id;?>);">
					
					
				<?php } ?>
					<div class="profileskill-edit-icon"> &nbsp; </div>
				</div></span>
				
				
				<?php $skillTitle = '\"'.$skillTitle.'\"';  ?>
                <?php if($results->type == "skill"){?>
                   <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
                   <?php if(empty($results->programid)){?>
                   <span><div class="skills-edit-delete skill-delete-div vtip"  title="<?php print t('LBL286'); ?>" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').displaySkillDeleteWizard('<?php echo t('MSG357').' '.strtolower(t(LBL876)).' '.check_plain(str_replace("\\\\", "", escape_string($skillTitle))).'?';?>',<?php echo $results->skill_id; ?>);"><div class="profileskill-delete-icon">&nbsp; </div></div></span>
                   <?php }else{?>
                   <span><div class="skills-edit-delete skill-delete-div vtip" title="<?php print t('LBL286'); ?>"><div class="profileskill-delete-icon disable">&nbsp; </div></div></span>
                <?php } }else if($results->type == "certificate" ){  ?>
                <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
                <span><div class="skills-edit-delete skill-delete-div vtip"  title="<?php print t('LBL286'); ?>" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').displaySkillDeleteWizard('<?php echo t('MSG357').' '.strtolower(t(LBL205)).' '.check_plain(str_replace("\\\\", "", escape_string($skillTitle))).'?';?>',<?php echo $results->skill_id; ?>);"><div class="profileskill-delete-icon">&nbsp; </div></div></span>
                <?php }else if($results->type == "certificate"){ ?>
                <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
                <span><div class="skills-edit-delete skill-delete-div vtip"  title="<?php print t('LBL286'); ?>"><div class="profileskill-delete-icon">&nbsp; </div></div></span>
                <?php } ?>
			
		 	</div></span>
		 	<?php }else{ ?>
		 	<div class="skill-columns dsiplay-other-set"><?php  echo t($results->display_to_others); ?></div>
		 	<div class="edit-delete-set">
				<div class="skills-edit-delete" onclick="$('body').data('learningcore').getLeanerQtipDiv(<?php echo $data;?>,this);">
					<?php echo t('LBL063');?>
				</div>
				<?php if(empty($results->programid)){?>
				<div class="skill-tab-seperator" >|</div>
				<div class="skills-edit-delete" onclick="$('#wizarad-myprofile-skills').data('myprofileskills').displaySkillDeleteWizard('<?php print t('MSG522');?>',<?php echo $results->skill_id; ?>);"> <?php print t("LBL286");?></div>
				<?php } ?>
		 	</div>
		 	<?php }?>		 	
		 	<div class="clearBoth"></div>
			<?php } ?>
		</div></span></div>
		
		
        		
        		
		<?php if($results->type == "certificate"){?>
		<div class="narrow-search-results-item-detail content-detail-code attrPadBWithDesc">
		  <span class="vtip" title="<?php print t('Added on').' '.$skill_added_on_dformat;?>">
		      <div class="add-certificate-date line-item-container float-left">
		          <?php 
		          if($showPipe){
		              print titleController('PROFILE-SKILL-ADDED-ON-DATE',(t('Added on').' '.$skill_added_on_dformat));
		          }
		          else{
		              print (t('Added on').' '.$skill_added_on_dformat);
		          }
		          //print t('Added on '.$skill_added_on_dformat);?>
		      </div>
		  </span>
		  <?php if($showPipe){?>
		      
		 <?php }?>
		  <span class="vtip" title="<?php print t($display_format);?>">
		      <div class="add-certificate-expiredate line-item-container float-left">
		         <?php if(!empty($display_format)){?>
		         <span class="narrow-search-results-item-detail-pipe-line skill-pipe-lines">|</span>
		         <?php 
		             print titleController('PROFILE-SKILL-EXPIREDATE',t($display_format));
		             //print t($display_format);
		         }?>
		      </div>
		  </span>
		</div>
      
	</div>
	
	<?php 
	/*if($cert_verify_status == 'Not verified'){
	    $cert_status_label = t('Not verified');
	}
    if($cert_verify_status == 'Verified'){
        $cert_status_label = t('Verified');
    }*/
	if($cert_verify_status == 'Verification pending'){
	    $cert_status_label = t('Verification').' '.strtolower(t('Pending'));
	}
    if($cert_verify_status == 'Rejected'){
         $cert_status_label = ucfirst(t('rejected'));
    }
	?>
	
	<div class="certificate-icon-container">
	<?php if(!empty($results->verification_status) && !empty($results->certificate_img)){?>
		  <div class="certificate-icon <?php print $cert_verify_status_format;?>" title="<?php print t($cert_verify_status);?>"></div>
		  <?php if(($cert_verify_status == 'Verification pending') || ($cert_verify_status == 'Rejected')){?>
		  <div class="certificate-verify-status <?php print t($cert_verify_status_format);?>"><?php print $cert_status_label;?></div>
		  <?php } else {?>
		  <div class="certificate-verify-status <?php print t($cert_verify_status_format);?>"><span class="certificate-view-label vtip" title="<?php print t('LBL816');?>" onclick = '$("#wizarad-myprofile-skills").data("myprofileskills").viewCertificateskill("<?php print addslashes($results->skill_name);?>","<?php print addslashes($results->certificate_img);?>","<?php print addslashes($fileFormat);?>")'><?php print t('LBL816');?></span></div>
		  <?php } }?>
	</div>
	<?php }else{ ?>
	<div class="cirBlock">
			<div>
			 <span class="topNo"><?php print t($results->proficiency);?></span>
			 <span class="bottomNo">10</span>
			 </div>
	  </div>
	<?php }?>
	
	
</div>
