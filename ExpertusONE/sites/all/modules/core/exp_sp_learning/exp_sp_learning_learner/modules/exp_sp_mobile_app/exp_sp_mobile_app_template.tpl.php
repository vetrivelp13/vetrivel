<?php global $theme_key;?>
<div class="learner-mobile-app-dialog learner-mobile-app-theme-<?php print $theme_key; ?>" id='mobile-app-download'>
	<div class="learner-mobile-app-<?php print ($type == 'android') ? 'android' : 'iphone' ?>"></div>
    <div id='learning-mobile-app-send-link'>
	<div class="learner-mobile-app-message">
	<?php $setLblMsg = str_replace('\'','\u0027',t('LBL966')); ?>
		<?php if($type == 'android'){ ?>
		<span><?php echo t('LBL963');?></span>
			 <input type="text" id='mobile-app-download-email' class="android-input-field" onblur="if(this.value=='') { this.value='<?php echo $setLblMsg;?>'; this.style.color='#999999'; this.style.fontStyle='italic';} else {this.style.color='#333333'; this.style.fontStyle='normal'; }" onfocus="if(this.value=='<?php echo $setLblMsg;?>') this.value=''; this.style.color='#333333'; this.style.fontStyle='normal';" value="<?php echo t('LBL966');?>" />
			 <?php  if($theme_key == 'expertusoneV2') {?>
			 <div class="newtheme-mobile-button"><div class="admin-save-button-left-bg"></div><input onclick="$('body').data('mobileapp').mobileAppSendLink('Android');" type="button" class="admin-save-button-middle-bg" value="<?php echo t('LBL965');?>"/><div class="admin-save-button-right-bg"></div><div class="clearBoth"></div></div>
	    <?php }else{?>
	    <div class="mobile-button"><div class="mobile-btn-bg-left"></div><input onclick="$('body').data('mobileapp').mobileAppSendLink('Android');" type="button" class="mobile-btn-bg-middle" value="<?php echo t('LBL965');?>"/><div class="mobile-btn-bg-right"></div><div class="clearBoth"></div></div> 
	    <?php }?>
			 <?php } else { ?>
				<span><?php echo t('LBL964');?></span>
			 <input type="text" id='mobile-app-download-email' class="iphone-input-field" onblur="if(this.value=='') { this.value='<?php echo $setLblMsg;?>'; this.style.color='#999999'; this.style.fontStyle='italic';} else {this.style.color='#333333'; this.style.fontStyle='normal'; }" onfocus="if(this.value=='<?php echo $setLblMsg;?>') this.value=''; this.style.color='#333333'; this.style.fontStyle='normal';" value="<?php echo t('LBL966');?>"/>
			 <?php  if($theme_key == 'expertusoneV2') {?>
			 <div class="newtheme-mobile-button"><div class="admin-save-button-left-bg"></div><input onclick="$('body').data('mobileapp').mobileAppSendLink('iPhone');" type="button" class="admin-save-button-middle-bg" value="<?php echo t('LBL965');?>"/><div class="admin-save-button-right-bg"></div><div class="clearBoth"></div></div>
	   <?php }else{?>
	   <div class="mobile-button"><div class="mobile-btn-bg-left"></div><input onclick="$('body').data('mobileapp').mobileAppSendLink('iPhone');" type="button" class="mobile-btn-bg-middle" value="<?php echo t('LBL965');?>"/><div class="mobile-btn-bg-right"></div><div class="clearBoth"></div></div>
			<?php }?>	
			 <?php } ?>		 	
	</div>
	<div class="learner-mobileapp-body-seperator">
		<div class="learner-mobileapp-body-seperator-line"></div>
		<div class="learner-mobileapp-body-seperator-text"><?php echo strtoupper(t('LBL644'));?></div>
		<div class="learner-mobileapp-body-seperator-line"></div>
	</div>
		<div onclick="$('body').data('mobileapp').mobileAppStore('<?php print $type;?>');" class="learner-mobile-app-getit-<?php print ($type == 'android') ? 'android' : 'iphone' ?>"></div>
	<div class="clearBoth"></div>
	</div>
	<div class="learner-mobile-app-note-info">
	</div>
</div>
