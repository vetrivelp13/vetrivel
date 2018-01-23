<?php 
global $theme_key;
if($results->object_type) {
  $typeImage = getTypeImageClass($results->object_type);
}else{
  $typeImage = "crs-icon";
}
$getTypeTitle=getTypeTitle($results);

$loggedInUserId = getIdOfLoggedInUser();
//$instructor = is_instructor($loggedInUserId);
$forumAdminAccess = forumAdminAccess();

$listList = chechIfInsHasAccess($results->object_id,$results->object_type);
$insListArray = explode(',',$listList[0]->ins_list);
if (in_array($loggedInUserId, $insListArray)) {
  $instructorAccess = 1;
}else{
  $instructorAccess = 0;
}
?>

<!-- Start Left -->
<div class="learner-forum-delivery-type-img"><div title="<?php print t($getTypeTitle);?>" class="<?php print $typeImage;?> vtip"></div></div>
<!-- End Left -->

<!-- Start Center -->
<div class="learner-forum-desc-container">
	<div>
    	<span>
    	<?php if (!empty($results->name)): ?>
    		<div class="limit-title-row"><?php $displayTitle = check_plain($results->name); ?>
    		<a class="limit-title learner-forum-title vtip" title="<?php print sanitize_data($results->name); ?>" href="?q=learning/forum-topic-list/<?php print $results->tid; ?>" ><?php print $displayTitle; ?></a>
    	<?php endif; ?>	</div>
    	</span>
    <?php if($loggedInUserId == 1 || $forumAdminAccess == 1){  ?>
    	<div class="admin-forum-action-link-container">
            <?php  if($theme_key == 'expertusoneV2') {
             /*-- #41381: Confirmation message Fix --*/
             ?>
                <span>                
            <a href="javascript:void(0);" class="admin-forum-action-links admin-forum-action-delicon vtip" title="<?php print t('LBL286');?>" onClick="$('#forum-list-display').data('forumlistdisplay').displayDeleteWizardForum('<?php print t('MSG357') . ' ' .strtoupper(check_plain(rawurlencode($results->name))); ?>','<?php print $results->format; ?>','<?php print $results->object_type; ?>','','','forum')"></a>
           </span>
                    <?php } else {?>
                      <span>
                    <a href="javascript:void(0);" class="admin-forum-action-links" title="<?php print t('LBL286');?>" onClick="$('#forum-list-display').data('forumlistdisplay').displayDeleteWizardForum('<?php print t('MSG357') . ' ' .strtoupper(check_plain(rawurlencode($results->name))); ?>','<?php print $results->format; ?>','<?php print $results->object_type; ?>','','','forum')"><?php print t('LBL286');?></a>
                    <!-- $('#forum-list-display').data('forumlistdisplay').deleteForum('<?php print $results->format; ?>','<?php print $results->object_type; ?>') -->
                    </span>
                    <?php }?>
          </div>
   	<?php }?>
	</div>
	<div class="content-detail-code">
	<div class="learner-forum-list-sub-txt">
	<div class="line-item-container float-left">
		<span title="<?php print t('LBL096').': '.sanitize_data($results->code);?>" class='vtip'>
		<!--  Replaced subStringController into titleController Due to the chinese character is not displaying in discussion form . fix is for This Ticket #0037220  -->
		<?php $code = titleController('EXP-SP-FORUM-VIEW-LIST-CODE',$results->code,10); 
		      print $code;?>
		</span>  </div>
		<div class="line-item-container float-left">
		<span class="pipe-line">|</span>  
		<span title="<?php print t('LBL038').': '.t($results->language_name);?>" class='vtip'>
		<?php $langchar = subStringController(t($results->language_name),3);
		      print t($langchar);?>	
		</span> 
		</div>
         <div class="line-item-container float-left">
		<span class="pipe-line">|</span> 
		<span title="<?php print t('LBL870');?>" class='vtip'><?php print t('LBL870');?>: <?php print $results->num_topics;?></span> </div>
		<div class="line-item-container float-left">
		<span class="pipe-line">|</span>
		<span title="<?php print t('LBL871');?>" class='vtip'><?php print t('LBL871');?>: <?php print $results->num_post;?></span> </div>
		<?php if($results->lct){ ?>
		<div class="line-item-container float-left">
		<span class="pipe-line">|</span>
		<span title="<?php print t('LBL872');?>" class='vtip'><?php print t('LBL872');?>: <?php print $results->lct;?></span> </div>
		<?php } ?>
		<?php //if($results->enrollid) {?>
		<!--<span class="pipe-line">|</span>
		<span title="Submitted By" class='vtip'>Registered</span>
		--><?php //} ?>
	   </div>
	   </div>
		<?php if (!empty($results->description)):  
		$crsMoreType= ($results->object_type == 'Course') ? 'cre_sys_obt_crs' : (($results->delivery_type_code) ? $results->delivery_type_code : $results->object_type);?>
			<div class="learner-forum-topic-description limit-desc-row <?php echo $crsMoreType; ?>">
				<div class="learner-forum-topic-content">
					<span class="limit-desc vtip">
				 	<span class="cls-learner-descriptions"> <?php print $results->description;  ?> </span>
				    </span>
				</div>
			</div>
		<?php endif; ?>
</div>
<!-- End Center -->

<?php 

$popupentityId      = $results->tid;
$popupAddTopicentityType    = 'forum';
$popupAddTopicIdInit        	 = $popupentityId.'_'.$popupAddTopicentityType;  
$popupAddTopicvisibPopupId  = 'qtip_visible_disp_addtopic_'.$popupAddTopicIdInit;
$anchorId = 'frm-'.$popupAddTopicvisibPopupId;
$popupAddTopic    = "data={'entityId':".$popupentityId.",'entityType':'".$popupAddTopicentityType."','url':'ajax/forum-add-topic/frm-list/add/".$popupentityId."/0','popupDispId':'".$popupAddTopicvisibPopupId."','catalogVisibleId':'qtipAddTopicIdqtip_visible_disp_".$popupAddTopicIdInit."','wBubble':475,'hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent'}";

?>
<!-- Start Right -->
<?php
if($instructorAccess == 1 || $loggedInUserId == 1 || $forumAdminAccess == 1){
  $paintMultiAction = '<div class="learner-forum-action-type"><div style="float:right;" class="learner-forum-action-container"><div class="add_topic_popup" id="'.$popupAddTopicvisibPopupId.'" ></div><div class="add-topic-btn"><a id='.$anchorId.' data='.$popupAddTopic.' class="learner-forum-action-link use-ajax" href="/?q=ajax/forum-add-topic/frm-list/add/'.$popupentityId.'/0">'.t('LBL875').'</a></div></div></div><div class="clearBoth"></div>';
}else{
  $paintMultiAction = '<div class="learner-forum-action-type"><div style="float:right;" class="learner-forum-action-container"><div class="add_topic_popup" id="'.$popupAddTopicvisibPopupId.'" ></div><div class="add-topic-btn"><a class="learner-forum-action-link-disabled">'.t('LBL875').'</a></div></div></div><div class="clearBoth"></div>';
}
  print $paintMultiAction; 
?>
<!-- End Right -->

<script language="Javascript" type="text/javascript"> 
  if(document.getElementById('forum-list-display')) {
      $("#forum-list-display").data("forumlistdisplay").paintAfterReady();
  }
</script>
