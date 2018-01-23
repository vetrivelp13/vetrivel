<?php 
expDebug::dPrint('enter the function sasasas1111'.print_r($record,true),4);
global $theme_key;
?>
<div class="top-record-div-left">
 <div class="para">
  <div class="code-container">
		<span class="detail-code"><?php print t("LBL083"); ?>:</span>
		 <span class="detail-desc limit-title-row"><span class="vtip limit-title" title="<?php print sanitize_data($record[0]->title); ?>"> <?php print $record[0]->title; ?>  </span></span>                              					
		 </span>                      				
	</div>
 </div>
 <div class="left-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL038"); ?>:</span>
			 <span class="detail-desc"> <?php print t($record[0]->language); ?> </span>                                             					
		</div>
	 </div>
 </div>
 <div class="right-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL618"); ?>:</span>
			 <span class="detail-desc">
			 <?php print $record[0]->banner_seq_num; ?>                          					
			 </span>                      				
		</div>
	 </div>
 </div>
 <div class="para">
	<div class="code-container">
		<span class="detail-code"><?php print t("LBL229"); ?>:</span>
		 <span  id="banner-detail-desc" class="detail-desc">
		 <?php print $record[0]->description; ?>                            					
		 </span>                      				
	</div>
 </div>
 <div class="left-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL619"); ?>:</span>
			<?php $headerProfileImage = file_create_url($record[0]->banner_thumbnail);?>
			 <span class="detail-desc"><table border="0" cellpadding="0" cellspacing="0"><tr><td style="padding-top:7px;" valign="middle"><img class="admin-user-load-picture" src= "<?php print $headerProfileImage.'?'.time();?>" width="60" height="35" /></td></tr></table>
			  </span>                                             					
		</div>
	 </div>
 </div>
 <div class="right-side-container">
	 <div class="para">
		<div class="code-container">
			<span class="detail-code"><?php print t("LBL620"); ?>:</span>
			 <span class="detail-desc">
			 <?php !empty($record[0]->date_activate)? print date_format(date_create($record[0]->date_activate),'m-d-Y') : '' ; ?>                          					
			 </span>                      				
		</div>
	 </div>
	 <div class="para">
		<div class="code-container">
			<span  class="detail-code"><?php print t("LBL621"); ?>:</span>
			 <span class="detail-desc">
			 <?php !empty($record[0]->date_deactivate) ? print date_format(date_create($record[0]->date_deactivate),'m-d-Y'): ''; ?>                          					
			 </span>                      				
		</div>
	 </div>
 </div>
   <?php       /*-- #36850 : Code Re-Factoring fix --*/
               $access  = getGroupNamesForView($record[0]->id, 'cbn_anm_typ_ban'); 
			   $accessName  = '';
				if(!empty($access)){
				  $accessName  = implode(", ",$access);
				}
	?>
 <div class="para">
	<div class="code-container">
		<span class="detail-code"><?php print t("LBL642"); ?>:</span>
		<span id="access-detail-desc" class="detail-desc detail-single-line"><span class="single-line-lbl-val vtip" title="<?php print sanitize_data($accessName); ?>"> <?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-BANNER-VIEW-ACCESSNAME', $accessName,65) : print titleController('ADMIN-NARROW-BANNER-VIEW-ACCESSNAME', $accessName,55); ?>  </span></span>                     				
	</div>
 </div>
</div>