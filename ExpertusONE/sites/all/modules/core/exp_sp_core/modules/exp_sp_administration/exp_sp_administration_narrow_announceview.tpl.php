<?php 
expDebug::dPrint('enter the function sasasas1111'.print_r($record,true),4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
		<span class="announce-code"><?php print t("LBL350"); ?>:</span>
		 <span  id="announce-detail-desc" class="detail-desc">
		 <?php print $record[0]->description; ?>                            					
		 </span>                             					
		 </span>                      				
	</div>
 </div>
  <div class="left-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="announce-code"><?php print t("LBL620"); ?>:</span>
			 <span class="detail-desc">
			 <?php print date_format(date_create($record[0]->from_date),'m-d-Y') ; ?>                          					
			 </span> 
			 <span class="time-detail-desc">
			 <?php print ($record[0]->from_time == "hh:mm") ? '' : $record[0]->from_time; ?>                          					
			 </span>                    				
		</div>
	 </div>
	 <div class="para">
		<div class="code-container">
			<span class="announce-code"><?php print t("LBL621"); ?>:</span>
			 <span class="detail-desc">
			 <?php print date_format(date_create($record[0]->to_date),'m-d-Y'); ?>                          					
			 </span>   
			 <span class="time-detail-desc">
			 <?php print ($record[0]->to_time == "hh:mm") ? '' : $record[0]->to_time; ?>                          					
			 </span>                   				
		</div>
	 </div>
 </div>
 <div class="right-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="announce-code"><?php print t("LBL038"); ?>:</span>
			 <span class="detail-desc"> <?php print t($record[0]->language); ?> </span>                                             					
		</div>
	 </div>
 <div class="para">
		<div class="code-container">
			<span class="announce-code"><?php print t("LBL1240"); ?>:</span>
			 <span class="detail-desc"> <?php print ($record[0]->priority == 1) ? t('LBL1244') : t('LBL1243'); ?> </span>                                             					
		</div>
	 </div>
 </div>
</div>
 <?php $access  = getGroupNamesForView($record[0]->id, 'cre_sys_obt_not');
 		 $accessName  = '';
	if(!empty($access)){
	  $accessName  = implode(", ",$access);
	}
	?>
 <div class="para">
	<div class="code-container">
		<span class="announce-code"><?php print t("LBL642"); ?>:</span>
			<span id= "access-detail-desc detail-single-line" class="detail-desc"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2')? print titleController('NARROW-ANNOUNCE-VIEW-ACCESSNAME', $accessName,65) : print titleController('NARROW-ANNOUNCE-VIEW-ACCESSNAME', $accessName,60); ?>  </span></span>                   					
	</div>
 </div>
</div> 