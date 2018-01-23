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
$AttachedRecord   = $results['AttachmentsResult'];
$NumAttachments   = $results['Attachments']['NumAttachments'];
$SessionDetails   = $results['SessionDetails'];
$DeliveryTypeCode = $results['DeliveryTypeCode'];  //Added by Vincent on Oct 28, 2013 for #0028593
$hideDesc= 'show';
// sanitize needed data's
$results['Code'] 			= sanitize_data($results['Code']);
$courseInfo->title 		= sanitize_data($courseInfo->title );
$courseInfo->code		= sanitize_data($courseInfo->code);
if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']== false){
	$hideDesc = 'hide';
}expDebug::dPrint('widget Course desc: '.print_r($_SESSION['widget'], true).'$hideDesc : '.$hideDesc, 4);
?>

<div id="class-details-displayarea" class="class-detail-display-area">
	<div class="detail-item-row">
		<div class="para">
  			<div class="code-container code">
  				<span class="detail-code"><?php print t("LBL096"); ?>:&nbsp;</span>
  				<span class="detail-desc vtip" title="<?php print $results['Code']; ?>"><?php print titleController('COURSE-VIEW-CODE',$results['Code'],20);?></span>
  			</div>
  		</div>
  		<?php if($hideDesc != 'hide'){ // Hide the Description?>
  		<div class="para">	
  			<div class="code-container cdescription">
  			  <span class="detail-code"><?php print t("LBL229"); ?>:&nbsp;</span>
  			  <span class="detail-desc"><?php print $results['Description']; ?></span>
			</div>
		</div>
		<?php } ?>
		<?php if($results['addn_catalog_show'] == 1){?>
		<div class="para">
	         <div class="code-container-additionalnotes">
	         	<span class="detail-code"><?php print t("LBL3068"); ?>:&nbsp;</span>
	            <span class="detail-desc"><?php print $results['Additional notes'];?></span>
	          </div>
	    </div>
	     <?php }  ?>
		<?php if(!empty($results['Duration']) && ($results['Duration'] != '-')) { ?>
		<div class="para">
    		<div class="code-container duration">
    		  <span class="detail-code"><?php print t("LBL248"); ?>:&nbsp;</span> 
    		  <span class="detail-desc"><?php print $results['Duration'];?></span>
    		</div>
    	</div>	
  		<?php } ?>
  		<div class="para">
	  		<div class="code-container language">
	  		  <span class="detail-code"><?php print t("LBL038"); ?>:&nbsp;</span> 
	  		  <span class="detail-desc"><?php print t($results['Language']);?></span>
	  		</div>  
	  	</div>
  		<?php if(count($SessionDetails)>0) { ?>		
    		<div class="para"><div class="code-container session-det"><span class="detail-code"><?php print t("LBL670"); ?>:</span>
    		<table border="0" cellpadding="0" class="course-session-timing" cellspacing="0" width="900" style="margin-bottom: 5px;">
  			<tr>
              <th><?php print t("LBL250");  ?></th>
              <th><?php print t("LBL042");  ?></th>
              <th><?php print t("LBL251");  ?></th>
              <th><?php print t("LBL252");  ?></th>
  			</tr>
    		<?php  $i=0; 
    		$userId = getIdOfLoggedInUser();
    		$timezone_details = getPersonDetails($userId);
    		
    		foreach($SessionDetails as $sessDetails) { 
        		  if(!empty($sessDetails["session_start_format"])) {
        		  	$sessDetails["session_title"] = sanitize_data($sessDetails["session_title"]);
        		  	//Added by Vincent on Oct 28, 2013 for #0028593
        		  	if($DeliveryTypeCode=='lrn_cls_dty_ilt'){
        		  		$session_start_format_loc = $sessDetails["ilt_session_start_format"];
        		  		$session_start_time_format_loc = $sessDetails["ilt_session_start_time_format"];
        		  		$session_start_end_format_loc = $sessDetails["ilt_session_start_end_format"];
        		  		$session_start_time_form_loc = $sessDetails["ilt_session_start_time_form"];
        		  		$session_end_time_form_loc = $sessDetails["ilt_session_end_time_form"];
        		  		$tzcode = $sessDetails["session_code"];
        		  		$tcode = $sessDetails["tz_code"];
        		  		$location_details = $session_start_format_loc .' '. $session_start_time_format_loc .' '. '<span class="time-zone-text">'.$session_start_time_form_loc.'</span>'.' to '.$session_start_end_format_loc.' <span class="time-zone-text">'.$session_end_time_form_loc.'</span>'.' '.$tzcode.' '.$tcode;
        		  		
        		  	}
        		  		$session_start_format = $sessDetails["session_start_format"];
        		  		$session_start_time_format = $sessDetails["session_start_time_format"];
        		  		$session_start_end_format = $sessDetails["session_start_end_format"];
        		  		$session_start_time_form = $sessDetails["session_start_time_form"];
        		  		$session_end_time_form = $sessDetails["session_end_time_form"];
        		  	?>
        		  	<?php 
									  if(($i%2) == 0){
						        	$class='course-class-session-detail-even';
						        }else{
						        	$class='course-class-session-detail-odd';
						        }?>
          			<tr class="<?php print $class; ?>">
        			  <td valign="top" class="course-class-session-detail" title="<?php print $sessDetails["session_title"]; ?>"><?php print titleController('SESSION-TITLE',$sessDetails["session_title"],20);?></td>
        			  <td valign="top" width="15%"><?php print $session_start_format;  ?></td>
        			  <td valign="top" width="15%"><?php print $session_start_time_format;  ?> <span class="course-timing-format"><?php print $session_start_time_form;  ?></span></td>
        			  <td valign="top" width="15%"><?php print $session_start_end_format;  ?> <span class="course-timing-format"><?php print $session_end_time_form;  ?></span>
          			
              
        		<?php
        
        		 if($DeliveryTypeCode=='lrn_cls_dty_ilt' && $sessDetails['sess_timezone'] !=$timezone_details['attr2'] ){
        		expDebug::dPrint(" entered");?>
        		              <?php 
        		                $qtip=qtip_popup_paint('course-details-'.$sessDetails["session_id"],$location_details);
        		                echo $qtip; 
        		        	
        		        		  } ?>
        		        		</td></tr>
        		        		<?php
        		  }
            $i++; } ?>	
            
    		</table>
    		</div>
    		</div>
    		
    		<div class="para">
    			<div class="code-container location">
    		<?php 
              if(!empty($results["LocationName"])) {
				  $locDet = $results["LocationName"];
    			  if(!empty($results["LocationAddr1"]))
                      $locDet .= ",<br>".$results["LocationAddr1"]; 
    			  if(!empty($results["LocationAddr2"]))
                      $locDet .= ",&nbsp;".$results["LocationAddr2"]; 
    			  if(!empty($results["LocationCity"]))
                      $locDet .= ",<br>".$results["LocationCity"]; 
    			  if(!empty($results["LocationState"]))
                      $locDet .= ",&nbsp;".$results["LocationState"]; 
    			  if(!empty($results["LocationCountry"]))
                      $locDet .= ",<br>".$results["LocationCountry"]; 
    			  if(!empty($results["LocationZip"]))
                      $locDet .= "&nbsp;-&nbsp;".$results["LocationZip"];
                  print "<span class='detail-code'>".t("Location").":</span><span class='detail-desc'>".$locDet."</span>";  
	          }        		
			
    		/*$sessinc = 1;
    		foreach($SessionDetails as $sessDetails) { 
        		  if(!empty($sessDetails["session_start_format"])) {
        		  	if($sessinc==1){ 
					  $sessinc++;
					  if(!empty($results["LocationName"])) {
    				  $locDet = $results["LocationName"];
					  }if(!empty($sessDetails["session_name"]))
                      $locDet .= ",<br>".$sessDetails["session_name"]; 
                      if(!empty($sessDetails["session_address1"])){
			          $locDet .= ",<br>".$sessDetails["session_address1"];
			          }if(!empty($sessDetails["session_address2"])){
			          $locDet .= ",<br>".$sessDetails["session_address2"];
			          }if(!empty($sessDetails["session_city"])){
			          $locDet .= ",<br>".$sessDetails["session_city"];
			          }if(!empty($sessDetails["session_state"])){
			          $locDet .= ",<br>".$sessDetails["session_state"];
			          }if(!empty($sessDetails["session_country"])){
			          $locDet .= ", ".$sessDetails["session_country"];
			          }if(!empty($sessDetails["session_zipcode"])){
			          $locDet .= "&nbsp".$sessDetails["session_zipcode"];
			          }
			          print "<b>Location:</b><br>".$locDet; 
        		  }
    		}
    		}
    		*/
			          ?>
			</div>
	      </div>          
  		<?php } ?>
  		
		<?php /* if(!empty($results["LocationName"])) { ?>
  		<div class="enroll-loc-details para">
  				<div class="enroll-loc-head"><span class="class-code-heading"><?php print t("Location"); ?>:</span></div>
  				<div class='enroll-location-text'><?php print $results["LocationName"].","; ?></div>
      			 <?php if(!empty($results["LocationAddr1"])) { ?>
      				<div class='enroll-location-text'><?php print $results["LocationAddr1"].","; ?></div>
    			 <?php } if(!empty($results["LocationAddr2"])) { ?>
    				<div class='enroll-location-text'><?php print $results["LocationAddr2"].","; ?></div>			
    			<?php } 
    			$locatCityState = array();
    			if(!empty($results["LocationCity"])) {
    			  $locatCityState[] = $results["LocationCity"].",";
    			}
		        if(!empty($results["LocationState"])) {
    			  $locatCityState[] = $results["LocationState"].",";
    			}
    			
    			$locationCityStateTxt = (count($locatCityState)>1) ? implode(",",$locatCityState) : $locatCityState[0];
    			
    			if(!empty($locationCityStateTxt)) { ?>
    				<div class='enroll-location-text'><?php print $locationCityStateTxt; ?></div>			
    			<?php } if(!empty($results["LocationZip"])) { ?>
    				<div class='enroll-location-text'><?php print $results["LocationZip"].","; ?></div>
    			<?php } if(!empty($results["LocationCountry"])) { ?>
    				<div class='enroll-location-text'><?php print $results["LocationCountryName"]; ?></div>
    			 <?php } ?>
		</div>
		<?php } */?>
  		
  		<div class="para">
  		 	<?php if ($NumAttachments > 0) { ?>
    			<div class="code-container attachments">
    				<span class="detail-code">
    				  <span class="lpn-head"><?php print t("LBL231"); ?>:&nbsp;</span>
    				  <span class="lpn-click-info"><?php print t("LBL232"); ?></span>
    				</span>
    				<span class='detail-desc'>
    					<ul class='class-details-attachmentdetails'>
		        			<?php for ($i = 0; $i < $NumAttachments; $i++) { ?>
		        				<li class='attachment-link'><?php print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <a class="attach vtip" title="<?php echo sanitize_data($AttachedRecord[$i]->readingtitle);?>" onclick="openAttachmentCommon('<?php echo $AttachedRecord[$i]->readingcontent; ?>')"><?php echo sanitize_data($AttachedRecord[$i]->readingtitle);?></a></li> 
		        			<?php } ?>
		      			</ul>
    				</span>
  				</div>
      		 <?php }  ?>
      	</div>
	</div>
</div>

	