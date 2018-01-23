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
$DeliveryTypeCode = $results['DeliveryTypeCode']; 
$userId = getSltpersonUserId();
$timezone_details = getPersonDetails($userId); //Added by Vincent on Oct 28, 2013 for #0028593
$hideDesc= 'show';
$results['Code']  = sanitize_data($results['Code']);
if( !empty($_SESSION['widget']['catalog_display_parameters']) &&  $_SESSION['widget']['catalog_display_parameters']['show_description']== false){
	$hideDesc = 'hide';
}expDebug::dPrint('widget Course desc: '.print_r($_SESSION['widget'], true).'$hideDesc : '.$hideDesc, 4);
?>

<div id="learning-class-details-displayarea">
    <div class="<?php print $results['Code'];?> lnp-cls-det">
	<div class="class-detail-item-row">
			<div class="para">
	  			<div class="class-code-container code">
	  				<span class="detail-code"><?php print t("LBL263"); ?>:&nbsp;</span>
	  				<span class="detail-desc"><?php print $results['Code'];?></span>
	  			</div>
  			</div>
  			<?php if($hideDesc != 'hide'){ // Hide the Description?>
  			 <?php if(!empty($results['Description'])) { ?>
			<div class="para">  			 
	  			<div class="class-code-container cdescription">
	  				<span class="detail-code"><?php print t("LBL229"); ?>:&nbsp;</span>
	  			    <span class="detail-desc"><?php print $results['Description']; ?></span>
				</div>
			</div>
			<?php } } ?>
		    <?php if($results['addn_catalog_show'] == 1){?>
            <div class="para">
	         <div class="code-container additionalnotes">
	           <span class="detail-code"><?php print t("LBL3068"); ?>:&nbsp;</span>
	           <span class="detail-desc"><?php print $results['Additional notes'];?></span>
	         </div>
	       </div>
	       <?php }  ?>
			<div class="para">			
  				<div class="class-code-container language">
  					<span class="detail-code"><?php print t("LBL038"); ?>:&nbsp;</span>
  					<span class="detail-desc"><?php print t($results['Language']);?></span>
  				</div>
  			</div>  
  		<?php if(count($SessionDetails)>0) { ?>	
			<div class="para">  		
				<div class="class-code-container session">	
    				<span class="detail-code" style="padding-bottom:10px;"><?php print t("LBL670"); ?>:</span>
    				<table border="0" cellpadding="0" class="course-session-timing" cellspacing="0">
			  			<tr>
			              <th><?php print t("LBL250");  ?></th>
			              <th><?php print t("LBL042");  ?></th>
			              <th><?php print t("LBL251");  ?></th>
			              <th><?php print t("LBL252");  ?></th>
			  			</tr>
			  			<?php expDebug::dPrint(" classAttachmentsResult=ss ". print_r($SessionDetails,true), 3); ?>
			    			<?php foreach($SessionDetails as $sessDetails) { 
			        		  if(!empty($sessDetails["session_start_format"])) {
			        		  	
			        		  	//Added by Vincent on Oct 28, 2013 for #0028593
			        		  	if($DeliveryTypeCode=='lrn_cls_dty_ilt'){
			        		  		$session_code = $sessDetails["session_code"];
			        		  		$tz_code = $sessDetails["tz_code"];
			        		  		$session_start_format_loc = $sessDetails["ilt_session_start_format"];
			        		  		$session_start_time_format_loc = $sessDetails["ilt_session_start_time_format"];
			        		  		$session_start_end_format_loc = $sessDetails["ilt_session_start_end_format"];
			        		  		$session_start_time_form_loc = $sessDetails["ilt_session_start_time_form"];
			        		  		$session_end_time_form_loc = $sessDetails["ilt_session_end_time_form"];
			        		  		$location_details = $session_start_format_loc .' '. $session_start_time_format_loc .' '. '<span class="time-zone-text">'.$session_start_time_form_loc.'</span>'.' to '.$session_start_end_format_loc.' <span class="time-zone-text">'.$session_end_time_form_loc.'</span> '.' '.$session_code.' '.$tz_code.' ';
			        		  	}
			        		  		$session_start_format = $sessDetails["session_start_format"];
			        		  		$session_start_time_format = $sessDetails["session_start_time_format"];
			        		  		$session_start_end_format = $sessDetails["session_start_end_format"];
			        		  		$session_start_time_form = $sessDetails["session_start_time_form"];
			        		  		$session_end_time_form = $sessDetails["session_end_time_form"];
			        		  if($DeliveryTypeCode=='lrn_cls_dty_ilt' && $sessDetails["sess_timezone"]!= $timezone_details['attr2'] ){
                                          $qtip=qtip_popup_paint($sessDetails["session_id"],$location_details);
                                          $session_end_time_form = '<span class="course-timing-format">'.$sessDetails["session_start_time_form"].'</span>'.' '.$qtip;
                                      }
			        		  	?>
			          			<tr>
			        			  <td valign="top" class="learning-class-session-table"><?php print $sessDetails["session_title"];  ?><span>&nbsp;</span></td>
			        			  <td valign="top" width="15%"><?php print $session_start_format;  ?></td>
			        			  <td valign="top" width="15%"><?php print $session_start_time_format;  ?> <span class="course-timing-format"><?php print $session_start_time_form;  ?></span></td>
			        			  <td valign="top" width="15%" class="learning_td"><?php print $session_start_end_format;  ?><span class="course-timing-format course-timing-format-endtime"><?php print $session_end_time_form;  ?></span></td>	
			          			</tr>
			        		<?php
			        		  }
			             	} ?>	
			    	</table>
    			</div>
    		</div>	

			<div class="para">    		
    			<div class="class-code-container location">	
		    		<?php 
		    		$sessinc = 1;
		    		foreach($SessionDetails as $sessDetails) {
		    			if(!empty($sessDetails["session_start_format"])) {
		    				if($sessinc==1){
		    					$sessinc++;
		    					if(!empty($results["LocationName"])) {
		    						$sessAddDet = $results["LocationName"];
		    					}/*if(!empty($sessDetails["session_name"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_name"];
		    					}*/if(!empty($sessDetails["session_address1"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_address1"];
		    					}if(!empty($sessDetails["session_address2"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_address2"];
		    					}if(!empty($sessDetails["session_city"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_city"];
		    					}if(!empty($sessDetails["session_state"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_state"];
		    					}if(!empty($sessDetails["session_country"])){
		    						$sessAddDet .= ",<br>".$sessDetails["session_country"];
		    					}if(!empty($sessDetails["session_zipcode"])){
		    						$sessAddDet .= ", ".$sessDetails["session_zipcode"];
		    					}
		    					print "<span class='detail-code'>".t("Location").": </span><span class='detail-desc'>".$sessAddDet."</span>";
		    				}
		    			}
		        	}			
		             ?>
		             <?php
		        			     /* if(!empty($results["LocationName"])) {
		        			      $locDet = $results["LocationName"].",<br>";
		        			      }if(!empty($results["LocationAddr1"])){
			                      $locDet .= $results["LocationAddr1"].",<br>";
			                      }if(!empty($results["LocationAddr2"])){
			                      $locDet .= $results["LocationAddr2"].",<br>";
			                      }if(!empty($results["LocationCity"])){
			                      $locDet .= $results["LocationCity"].",<br>";
			                      }if(!empty($results["LocationState"])){
			                      $locDet .= $results["LocationState"].", ";
			                      }if(!empty($results["LocationCountry"])){
			                      $locDet .= $results["LocationCountry"]."&nbsp;";
			                      }if(!empty($results["LocationZip"])){
								  $locDet .= $results["LocationZip"];
			                      }
		                    
		                      print "<b>Location:</b><br>";
		                      print $locDet;  */
		                      ?>
					</div>
    			</div>
  		<?php } ?>
        <?php if(!empty($results['Duration']) && ($results['Duration'] != '-')) { ?>
			<div class="para">        
		        <div class="class-code-container duration">
		        	<span class="detail-code"><?php print t("LBL248"); ?>:&nbsp;</span>
		        	<span class="detail-desc"><?php print $results['Duration'];?></span>
		        </div>
			</div>
      	<?php } ?>
      	<?php if ($NumAttachments > 0) { ?>
			<div class="para">      	
	    		<div class="class-code-container attachment">
    				<span class="detail-code">
    					<span class="lnp-head"><?php print t("LBL231"); ?>:&nbsp;</span>
    					<span class="click-info"><?php print t("LBL232"); ?></span>
    				</span>
    				<span class="detail-desc class-attachment">
		    			<ul class='class-details-attachmentdetails'>
		        		<?php for ($i = 0; $i < $NumAttachments; $i++) { ?>
							<li class='attachment-link'><?php print ($i == 0)?'':'<span class="divider-pipeline">|</span>';?> <a class="attach vtip" title="<?php $AttachedRecord[$i]->readingtitle; ?>" onclick="openAttachment('<?php echo addslashes($AttachedRecord[$i]->readingcontent); ?>')"><?php echo titleController(' ', sanitize_data($AttachedRecord[$i]->readingtitle),100); ?></a></li>  
		        		<?php  	} ?>
		      			</ul>
    				</span>
	  			</div>
			</div>
      	<?php }  ?>
    	</div>       
        <div class="clearBoth"></div>
	</div>
</div>
