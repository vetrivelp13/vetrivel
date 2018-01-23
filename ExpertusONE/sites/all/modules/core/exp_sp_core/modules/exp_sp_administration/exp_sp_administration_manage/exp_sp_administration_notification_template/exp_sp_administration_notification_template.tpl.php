<?php
        expDebug::dPrint("Testing class template :".print_r($detail,true),4);
        $templateTitle   = t($detail->lang).' '.t('LBL038').' '.t('LBL437');
        //exit;
    	//$displayTitle = titleController('ADMIN_COURSE_CLASS', $detail->cls_title);
        $createdOn =  new DateTime($detail->created_on);
        $tempCreatedOn = $createdOn->format('M d, Y');
    	$entityId          	 =  $detail->id;
        $entityType        	 = 'cre_ntn';
        $qtipIdInit        	 = $entityId.'_'.$entityType;  
        if($argType == 'notification_template'){
          $qtipOptClassObj     = "{'url':'administration/manage/notification_template/temp-addedit/".$notificationId."/".$entityId."/".$entityType."','popupDispId':'qtip_edittemplate_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachNotification_edittemplate_visible_disp_".$entityId."','wBubble':650,'hBubble':400,'tipPosition':'bottomRight','qtipClass':'qtip-parent','qtipSubClass':'add-key-word-edit-list','courseId':$notificationId}";
        }else{
          $qtipOptClassObj     = "{'url':'administration/manage/certificate/temp-addedit/".$notificationId."/".$entityId."/".$entityType."','popupDispId':'qtip_edittemplate_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAttachCertification_edittemplate_visible_disp_".$entityId."','wBubble':650,'hBubble':400,'tipPosition':'bottomRight','qtipClass':'qtip-parent','qtipSubClass':'add-key-word-edit-list','courseId':$notificationId}";
        }
?>
	<div id="<?php print 'edit-template-list-'.$detail->id; ?>" class='edit-class-list'>
		<table cellpadding='5' class='edit-class-list-container' cellspacing='5' border='0'>
			<tr>
				<td><div class='narrow-search-results-item-title-class'><span title="<?php print t($detail->lang); ?>"><?php print $templateTitle;?></span></div></td>             		
				<td class="edit-class-list-right" rowspan="2">
					<div class='admin-add-button-container'>
					<input type="hidden" id="qtip_position" value="">
						<div id='qtip_edittemplate_visible_disp_<?php print $qtipIdInit ?>'>
							<?php
							$editLink = t('LBL063');
							global $theme_key;
							if($theme_key == 'expertusoneV2') {
							  $editLink = '';
							}
								if($detail->id==$templateId){
							?> 
							<input id="<?php print 'edit-template-list-button-'.$detail->id; ?>"  class="form-submit admin-add-edit-button addedit-form-class-button" type="button" name="<?php print 'edit-template-'.$notificationId.'-'.$detail->id; ?> " value="<?php print $editLink; ?>" />
							<?php 
								}else{
							?>
							<input id="<?php print 'edit-template-list-button-'.$detail->id; ?>"  onmousedown="$('#root-admin').data('narrowsearch').getBubblePopup(<?php print $qtipOptClassObj;?>);" class="form-submit admin-add-edit-button addedit-form-class-button" onclick="$(document).ready(function(){  editTemplateDetails('<?php print $notificationId;?>','<?php print $detail->id; ?>') }); remove_messages('class'); return false;" type="button" name="<?php print 'edit-template-'.$notificationId.'-'.$detail->id; ?> " value="<?php print $editLink; ?>" />
							<?php 
								}
							?>
						</div>
					</div>
				</td>
			</tr>
			<tr>
				<td class="edit-class-list-left">
					<div class="narrow-search-results-item-detail"> <!--  added by ganeshbabuv on june 26th 2015 9:30 PM for security fix -->
						<span title="<?php print t('LBL928'); ?> - <?php print sanitize_data($detail->created_by); ?>"><?php print t('LBL928').': '.sanitize_data($detail->created_by); ?></span>
						<span title="<?php print ucfirst(t('LBL945')); ?>"><?php print ucfirst(t('LBL945')).' '.$tempCreatedOn; ?></span>
						
				</div>
	
					</td>	
				</tr>
		</table>
		<div id="<?php print 'edit-template-wrapper-'.$detail->id; ?>"></div>
	</div>