<?php
expDebug::dPrint('enter the function sasasas1111'.print_r($record,true),4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
		<span class="detail-code"><?php print t("LBL107"); ?>:</span>
		 <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($record['title']); ?>"> <?php print $record['title']; ?>  </span></span>
		 </span>
	</div>
 </div>
 <div class="para">
  <div class="code-container">
		<span class="detail-code"><?php print t("LBL229"); ?>:</span>
		 <span id="notify-detail-desc" class="detail-desc"><?php print $record['short_description']; ?></span>
		 </span>
	</div>
 </div>
 <?php if($record['send_options'] != 'certificate') { ?>
 <div class="left-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL940"); ?>:</span>
			 <span class="detail-desc">
			 <?php print t($record['notify_type']); ?>
			 </span>
		</div>
	 </div>
	 <?php $recipients =  getProfileItemNamesKeyed()?>
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL918"); ?>:</span>
			 <span class="detail-desc">
			 <?php print t($record['sendmailto']); ?>
			 </span>
		</div>
	 </div>
 </div>
  <div class="right-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL036"); ?>:</span>
			 <span class="detail-desc">
			 <?php print t($record['entity_type']); ?>
			 </span>
		</div>
	 </div>
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL923"); ?>:</span>
			 <span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($record['notify_cc']); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-NOTIFY-VIEW-NOTIFYCC', $record['notify_cc'],30) : print titleController('ADMIN-NARROW-NOTIFY-VIEW-NOTIFYCC', $record['notify_cc'],25) ; ?>  </span></span>
		</div>
	 </div>
 </div>

  <?php } ?>
   <?php if($record['send_options'] == 'certificate')
   			$access  = getGroupNamesForView($record['id'], 'cre_cer');
   		 else
   			$access  = getGroupNamesForView($record['id'], 'cre_ntn');
			   $accessName  = '';
				if(!empty($access)){
				  $accessName  = implode(", ",$access);
				}
	?>
 <div class="para">
	<div class="code-container">
		<span class="detail-code"><?php print t("LBL642"); ?>:</span>
		<span class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"> <?php print titleController('ADMIN-NARROW-NOTIFY-VIEW-ACCESSNAME', $accessName,90); ?>  </span></span>
	</div>
 </div>
  <div class="para">
  <?php $getTemplate= templatefornotify($record['id']);
    expDebug::dPrint('$$getTemplate-->'.print_r($getTemplate,true),4);
     if(empty($getTemplate)) { ?>
  	 	<span class="detail-title"><?php print t("LBL925");?>:</span>
						<div class ="nonlist"><?php print t('MSG403')?></div>
	<?php } else {?>
  <div class="code-container">
	 <span class="detail-code"></span>
		 <span class="detail-desc">
  	  <table cellpadding="0" class="class-session-details" cellspacing="0" width="640" border="0" style="margin-bottom: 5px;">
	  	 <tr>
	   		<th> <?php print t('LBL925');?></th>
	   	 </tr>
				 <?php  $i=0;
			 		foreach($getTemplate as $tempInfo) {
			 		expDebug::dPrint('$contentName--->'.print_r($tempInfo,true),5);
				  if(($i%2) == 0){
	        	$class='template-separation';
     			}?>
	   	 <tr>
			 	<td valign="top"><span class="template-names"><?php print t($tempInfo->lang).' '.t('LBL038').' '.t('LBL437'); ?></span></td>
			 </tr>
			 <tr>
				<td class="template-separation" valign="top" ><span class="edit-class-list-left"><?php print t('LBL928').' : '.$tempInfo->created_by.' ';?></span><span class="edit-class-list-left"><?php print ucfirst(t('LBL945')).' '.date_format(date_create($tempInfo->created_on),'M d, Y')?></span></td>
		   </tr>
				 <?php $i++;
				} ?>
  	</table>
   </span>
	</div>
 <?php } ?>
 </div>
</div>