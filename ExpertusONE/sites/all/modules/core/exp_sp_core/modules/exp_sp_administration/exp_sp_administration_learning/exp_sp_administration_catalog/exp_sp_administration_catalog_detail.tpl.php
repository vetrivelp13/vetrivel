<?php
//Using the below veriable we can find current loaded theme name
global $theme_key, $user;


        expDebug::dPrint("Testing class template :".print_r($detail,true) ,4);
        $classTitle   = $detail->cls_title;
        //exit;
    	$displayTitle = titleController('NARROW-SEARCH-RESULTS-ITEM-CLASS-TITLE', sanitize_data($detail->cls_title),96);
    	if(is_data_admin($user)){
    	 	$getprivedit = 1;
    	}else{
    		$getprivedit = getprivDetails($detail->cls_id);
    	}
    	$entityId          	 =  core_encrypt($detail->cls_id);
        $entityType        	 = 'cre_sys_obt_cls';
        $qtipIdInit        	 = $entityId.'_'.$entityType;  
        $qtipSessionEnforce  = ($oldClassId)? 'enforce':'';
        $qtipOptClassObj     = "{'url':'administration/class-addedit/".core_encrypt($courseId)."/".$entityId."/".$entityType."/".$oldClassId."','popupDispId':'qtip_editclass_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtip_editclass_visible_dispid_".$entityId."','wBubble':870,'hBubble':'auto','tipPosition':'bottomRight','qtipClass':'qtip-parent course-class-edit-qtip','courseId':$courseId,qtipSessionEnforce:'".$qtipSessionEnforce."'}";
        $createdBy			 = checkClassCreatedBy($detail->cls_id, $user->uid);
        if ($createdBy) {$getprivedit = true;}
?>
	<div id="<?php print 'edit-class-list-'.$detail->cls_id; ?>" class='edit-class-list'>
		<table cellpadding='5' class='edit-class-list-container' cellspacing='5' border='0'>
			<tr>
				<td><div class='narrow-search-results-item-title-class'><span class="vtip" title="<?php print sanitize_data($detail->cls_title); ?>"><?php print $displayTitle;?></span></div></td>        
<?php 
		if($detail->facility_name!='')
          $facilityName  = '<span class="narrow-search-results-item-detail-pipe-line">|</span><span title="'.t('LBL312').'">'.$detail->facility_name.'</span>';
        else
          $facilityName  = '';
?>        		
				<td class="edit-class-list-right" rowspan="2">
					<div class='admin-add-button-container'>
						<div id='qtip_editclass_visible_disp_<?php print $qtipIdInit ?>'>
							<?php 
								if($detail->cls_id==$classId){
							?> 
							<input id="<?php print 'edit-class-list-button-'.core_encrypt($detail->cls_id); ?>"  class="form-submit admin-add-edit-button addedit-form-class-button" type="button" name="<?php print 'edit-class-'.core_encrypt($courseId).'-'.core_encrypt($detail->cls_id); ?> " value="<?php print t('LBL063'); ?>" />
							<?php 
								}else if($getprivedit == 0 && $getprivedit != '' && $GLOBALS["user"]->uid != 1){
									?>
									<input id="<?php print 'edit-class-list-button-'.core_encrypt($detail->cls_id); ?>"  class="form-submit admin-add-edit-button addedit-form-class-button disable-button"  type="button" name="<?php print 'edit-class-'.core_encrypt($courseId).'-'.core_encrypt($detail->cls_id); ?> " value="<?php print ($theme_key == 'expertusoneV2')? '': t('LBL063'); ?>" />
									<?php 
								}else{
							?>
							<input id="<?php print 'edit-class-list-button-'.core_encrypt($detail->cls_id); ?>"  onmousedown="$('#root-admin').data('narrowsearch').getBubblePopup(<?php print $qtipOptClassObj;?>);" class="form-submit admin-add-edit-button addedit-form-class-button" onclick="$(document).ready(function(){  editClassDetails('<?php print core_encrypt($courseId);?>','<?php print core_encrypt($detail->cls_id); ?>') }); remove_messages('class'); return false;" type="button" name="<?php print 'edit-class-'.core_encrypt($courseId).'-'.core_encrypt($detail->cls_id); ?> " value="<?php print ($theme_key == 'expertusoneV2')? '': t('LBL063'); ?>" />
							<?php 
								}
							?>
						</div>
					</div>
				</td>
			</tr>
			<tr>
				<td class="edit-class-list-left">
					<div class="narrow-search-results-item-detail vtip">
						<span class="vtip" title="<?php print t('LBL096'); ?> - <?php print sanitize_data($detail->cls_code); ?>"><?php print titleController('NARROW-SEARCH-RESULTS-ITEM-DETAIL-CLASS-CODE', sanitize_data($detail->cls_code),15); ?></span>
						<span class="narrow-search-results-item-detail-pipe-line">|</span>
						<span title="<?php print t('LBL038'); ?>- <?php print t($detail->cls_language); ?>"><?php print titleController('NARROW-SEARCH-RESULTS-ITEM-DETAIL-CLASS-LANGUAGE',t($detail->cls_language),12); ?></span>
						<span class="narrow-search-results-item-detail-pipe-line">|</span>
						<span title="<?php print t('LBL102'); ?>"><?php print t($detail->cls_status); ?></span>
						<span class="narrow-search-results-item-detail-pipe-line">|</span>
						<span title="<?php print t('LBL084'); ?>"><?php print t($detail->cls_delivery_type); ?></span><?php print $facilityName; ?>
						
						
						<!--  Session Details Start  -->
                        <?php 
                        	if(count($detail->session_details) > 0) {
                        		$sDay = $detail->session_details[0]['session_start_date_format'];
                        		$sTime = $detail->session_details[0]['session_start_time_format'];
                        		$sTimeForm = $detail->session_details[0]['session_start_time_form'];
                        		if(count($detail->session_details) > 1) {
                        				$sessLenEnd = count($detail->session_details)-1;							
                        				$eDay = $detail->session_details[$sessLenEnd]['session_start_date_format'];
                        				$eTime = $detail->session_details[$sessLenEnd]['session_start_end_format'];
                        				$eTimeForm = $detail->session_details[$sessLenEnd]['session_end_time_form'];
                        				$sessionDate = $sDay .' '. $sTime .' '. '<span class="time-zone-text">'.$sTimeForm.'</span>'.' '.strtolower(t('LBL621')).' '.$eDay. ' ' .$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
                        			}else {					
                        				$eTime = $detail->session_details[0]['session_start_end_format'];
                        				$eTimeForm = $detail->session_details[0]['session_end_time_form'];
                        				$sessionDate = $sDay .' '. $sTime.' '.'<span class="time-zone-text">'.$sTimeForm.'</span>'.' '.strtolower(t('LBL621')).' '.$eTime.' <span class="time-zone-text">'.$eTimeForm.'</span>';
                       			}
                        	}
                        ?>
    	
                        <?php if($detail->cls_delivery_type_code == "lrn_cls_dty_ilt" || $detail->cls_delivery_type_code == "lrn_cls_dty_vcl") { ?>
                    	<span class="narrow-search-results-item-detail-pipe-line">|</span>
                    	<span><?php print $sessionDate;?></span>
                    	<?php } ?>                    	
                    	<!--  Session Details End  -->

				</div>
	
					</td>
				</tr>
		</table>
		<div id="<?php print 'edit-class-wrapper-'.$detail->cls_id; ?>"></div>
	</div>
	<?php 
		if($detail->cls_id==$classId){
			?> 
			<script language="Javascript" type="text/javascript">
				$('#root-admin').data('narrowsearch').getBubblePopup(<?php print $qtipOptClassObj;?>); 
				$('#edit-class-list-button-<?php print core_encrypt($classId);?>').click();
			</script>
			<?php 
		}
	
	?>