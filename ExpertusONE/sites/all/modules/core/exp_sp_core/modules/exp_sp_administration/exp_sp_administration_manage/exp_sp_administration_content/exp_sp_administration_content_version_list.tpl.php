<?php   global $theme_key;
        expDebug::dPrint("Testing class template :".print_r($detail,true) ,4);
        $versionTitle   = $detail->version_title;
        //exit;
        /* Tilte for the particular version from slt_content_version table */
    	$displayTitle = titleController('EXP-SP-ADMIN-CONTENT-VERSION-VERSIONTITLE', $detail->version_title,30);
      
        $qtipIdInit        	 =  'version_'.$detail->version_id;  
        $numActiveUsers = getActiveUsersForContent($detail->master_id,$detail->version_id);
        
        $OtherVersions = '';
        if($detail->version_status == "lrn_cnt_sts_atv"){
        	$OtherVersions = getOtherVersions($detail->master_id,$detail->version_id);
        }   
        
        /* default version for that content */
		$defaultRadio = ($detail->version_status == "lrn_cnt_sts_atv")? "crs-pub-enrolled-btn plainy-btn" : "crs-publish-btn glossy-btn";
		expDebug::dPrint("default rado id --------> ".$defaultRadio);
		
		/* check if the version is default - for enabling the radio button*/
		$radioOnlick = ($detail->version_status == "lrn_cnt_sts_atv") ? "" : "onclick = \"setVersionDefault('$detail->master_id','$detail->version_id')\"";
		$buttonValue = ($detail->version_status == "lrn_cnt_sts_atv") ? t('Published') : t('LBL570');
		$lessonList = getLessonList($detail->version_id);
		
		$versionCreator = getPersonDetails($detail->version_creator, array('full_name'));
		$className='';
		if($detail->sub_type=='lrn_cnt_typ_knc'){
			if($detail->hosted_type==2){
				$className = 'content-version-icon kc-url-type';
			}else{
				$ext = substr($detail->version_title,strlen($detail->version_title)-4);
				if($ext=='.doc' || $ext=='docx'){
					$className = 'content-version-icon kc-doc-file';
				}else if($ext=='.xls' || $ext=='xlsx'){
					$className = 'content-version-icon kc-xls-file';
				}else if($ext=='.ppt' || $ext=='pptx'){
					$className = 'content-version-icon kc-ppt-file';
				}else if($ext=='.pdf'){
					$className = 'content-version-icon kc-pdf-file';
				}else if($ext=='.jpg' || $ext=='jpeg' || $ext=='.gif' || $ext=='.png'){
					$className = 'content-version-icon kc-img-file';
				}else{
					$className = 'content-version-icon kc-txt-file';
				}
			}
		}else if($detail->sub_type=='lrn_cnt_typ_vod'){
			$className = 'content-version-icon video-file-type';
		}else{
			$className = 'content-version-icon';
		}
		
?>
	<div id="<?php print 'edit-class-list-'.$detail->version_id; ?>" class='edit-class-list'>
		<table cellpadding='0' class='edit-content-version-list-container' cellspacing='0' border='0' width="99%">
			<tr>
				<td rowspan="2" valign="top">
					<div class="<?php print $className; ?>"></div>
					<div title="<?php print $detail->version_file_size; ?>"><?php print $detail->version_file_size; ?></div>
				</td>
				<td valign="top">
					<div class='narrow-search-results-item-title-class'>
						<span title="<?php print sanitize_data($detail->version_title); ?>" class="content-version-title vtip"><?php print $displayTitle;?></span>
						<?php if(count($lessonList) > 1){								
									$onclick = "onclick = '$(\"#root-admin\").data(\"narrowsearch\").getPreviewContent(".$detail->master_id.",".$detail->version_id.",".json_encode($lessonList).")'";
						?>
							<!-- 	<span><a class="narrow-search-results-item-action-list-btn vtip" title="<?php print t('LBL694');?>" id = "preview_<?php print $detail->master_id;?>_<?php print $detail->version_id?>" <?php print $onclick; ?>><?php print t('LBL694');?></></a></span> -->
									<span class="vtip" title="<?php print t('LBL694');?>"><a class="narrow-search-results-item-action-list-btn preview-content-version"  id="preview_<?php print $detail->master_id;?>_<?php print $detail->version_id?>" <?php print $onclick;?>><?php print ($theme_key == 'expertusoneV2')? '' : t('LBL694');?></a></span>
						<?php }else if(count($lessonList) == 1){
							    $popupHolderId = 'vlist-video-preview-popup-container-' . $detail->version_id;
									$onclick = getOnclick($lessonList, $detail->version_id, $popupHolderId);
									if($lessonList[0]->contentypecode != "lrn_cnt_typ_vod"){
										$previewCls = "preview-content-version";
									}else{
										$previewCls = "preview-content-version actionLink enroll-launch use-ajax";
									}									
						?>
						      <div style="float:left;" class="video-preview-popup-container" id="<?php print $popupHolderId; ?>"></div>
									<span class="vtip" title="<?php print t('LBL694');?>">
									  <a class="narrow-search-results-item-action-list-btn <?php print $previewCls;?>"  id="vlist-preview-<?php print $detail->master_id;?>-<?php print $detail->version_id?>" <?php print $onclick;?>>
									    <?php print ($theme_key == 'expertusoneV2')? '' : t('LBL694');?>
									  </a>
									</span>
						<?php }?>
						<span class="narrow-search-results-item-detail-pipe-line">|</span>
						<span id='qtip_editclass_visible_disp_<?php print $qtipIdInit ?>'>
						<?php if($detail->version_status != "lrn_cnt_sts_atv" && $numActiveUsers == 0){?>
							<a href="javascript:void(0)" class="narrow-search-results-item-action-list-btn enable-delete-icon  vtip" onclick="deleteContentVersion('<?php print $detail->master_id ; ?>','<?php print $detail->version_id ; ?>','<?php print htmlspecialchars($displayTitle);?>')" title="<?php print t('LBL286');?>"><?php print ($theme_key == 'expertusoneV2')? '' : t("LBL286"); ?></a>
						<?php } else { ?>
							<a class="narrow-search-results-item-action-list-btn enable-delete-icon disable-delete-link  vtip" title="<?php print t('LBL286');?>"><?php print ($theme_key == 'expertusoneV2')? '' : t("LBL286"); ?></a>
						<?php } ?>
						</span>
					</div>
					<div class="clearBoth"></div>
					<?php 
						$versionCreated = date_format(date_create($detail->version_created),'M d, Y');
						$versionUpdated = ($detail->version_updated != null) ? date_format(date_create($detail->version_updated),'M d, Y') : '';
					?>
					<div class="edit-content-version-list-left">
						<div class="narrow-search-results-item-detail">
							<span class ="vtip" title="<?php print t('LBL944'); ?>"><?php print t('LBL944');?></span>: <span class ="vtip" title="<?php print sanitize_data($versionCreator['full_name']) ?>"><?php print $versionCreator['full_name']?></span>
							<span class = "vtip" title="<?php print t('LBL945'); ?>"><?php print t('LBL945'); ?></span>&nbsp;<span class="vtip" title="<?php print sanitize_data($versionCreated); ?>"><?php print $versionCreated; ?></span>
							
													   
						   <?php if(!empty($versionUpdated)){?>
							    <span class="narrow-search-results-item-detail-pipe-line">|</span>
							   	<span class="vtip" title="<?php print t('LBL946'); ?>"><?php print t('LBL946'); ?></span> : <span class="vtip" title="<?php print sanitize_data($versionUpdated); ?>"><?php print $versionUpdated; ?></span>
						   <?php }?>
						   
						</div>	
					</div>
					
						   
						   
				</td>        
				<td class="edit-content-version-list-right" rowspan="2" valign="top">
					<div class='admin-add-button-container'>
						<div id='qtip_editclass_visible_disp_<?php print $qtipIdInit ?>'>
							<a id="versionradio_<?php print $detail->version_id ;?>" name="versionradio_<?php print $detail->version_id ;?>" class="<?php print $defaultRadio;?>" <?php print $radioOnlick; ?>><?php print $buttonValue; ?></a>
													
						</div>
					</div>
				</td>
			</tr>
			<tr>
			<?php 			
			/* display Transfer users only for active version */					
        	?>
			<td colspan="3" class="version-content-transfer-user">
				<?php if(!empty($OtherVersions) && $detail->version_status == "lrn_cnt_sts_atv" && $detail->content_transfer == 0){?>
				<div class="version-content-transfer-user-icon"></div>
				<div class="version-content-transfer-user-txt">			  					
	        			<span class="inprogress-active" id = "inprogress_<?php print $detail->master_id;?>_<?php print $detail->version_id;?>"><?php print t('LBL978') ;?></span>
	        	</div>
	        	<a id="transfer-dd-arrow" class="transfer-dd-arrow-link" onclick="transUserList();">&nbsp;</a>
	        	<script type="text/javascript"> 
	        		var tuid='inprogress_<?php print $detail->master_id;?>_<?php print $detail->version_id;?>';
	        		var actionLink = "<?php print t('LBL978') ;?>";
	        	</script>
	        	<ul id="transfer-dd-list">
	        	 <div class="transfer-dd-list-arrow"></div>
	        	<?php foreach($OtherVersions as $OtherVersion){
	        		$otherPublished = date_format(date_create($OtherVersion->other_version_publishedon),'M d, Y');
	        		$popupVersionId     = $OtherVersion->other_version_id;
				    $popupContentId    = $OtherVersion->other_content_id;
				    $popupMoveUsersIdInit        	 = $popupContentId.'_'.$popupVersionId;  
				    $popupMoveUsersvisibPopupId  = 'qtip_visible_disp_moveusers_'.$popupMoveUsersIdInit;
				    $popupMoveUsers    = "{'entityId':".$popupVersionId.",'entityType':'".$popupContentId."','url':'administration/manage/content/moveusers/".$popupContentId."/".$popupVersionId."','popupDispId':'".$popupMoveUsersvisibPopupId."','catalogVisibleId':'qtipMoveUsersqtip_visible_disp_".$popupMoveUsersIdInit."','wBubble':'560','hBubble':'auto','tipPosition':'bottomLeft','qtipClass':'content-tip-display-top'}";
				    					    
	        	?>
	        		<input type="hidden" id="qtip_position" value="">
			    	<span id="<?php print $popupMoveUsersvisibPopupId; ?>">	
		        	<li class="vtip" title = "<?php print sanitize_data($otherPublished); ?>" onclick="setPosition(this,tuid,<?php print "'".addslashes($OtherVersion->other_version_title)."'";?>); isPopupOpen=1; $('#root-admin').data('narrowsearch').getBubblePopup(<?php print $popupMoveUsers; ?>);"><?php print $OtherVersion->other_version_title;?></li>
		        	</span>		   
		        <?php }?>	     				            
		        </ul>			        
		        <?php }elseif(empty($OtherVersions) && $detail->version_status == "lrn_cnt_sts_atv"){?>
		        		<div class="version-content-transfer-user-icon"></div>
		        		<div class="version-content-transfer-user-txt">	
		        			<span class="inprogress-deactive" id = "inprogress_<?php print $detail->master_id;?>_<?php print $detail->version_id;?>"><?php print t('LBL978') ;?></span>
		        	    </div>
				<?php }else{?>
						&nbsp;
			<?php }?>
			</td>
			</tr>
		</table>
		<?php if($detail->row == 'tr_0' && $detail->sub_type=='lrn_cnt_typ_vod') {?>
		<script>convertVideoForMobileAccess();</script>
		<?php } ?>
		<div id="<?php print 'edit-class-wrapper-'.$detail->version_id; ?>"></div>
		<script>
				Drupal.attachBehaviors();	
		</script>
	</div>
	