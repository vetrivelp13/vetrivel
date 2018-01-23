<?php
/*
 * This template used only to render the online users list. User chat dialog panels are rendered in drupalchat.js
 */
?>
<div class="<?php print $subpanel['name']; ?> subpanel_toggle">
  <div class="block-title-left">
    <div class="block-title-right">
      <div class="block-title-middle">
        <span class="subpanel_title_text">
          <?php if ($subpanel['icon']):?>
            <?php print $subpanel['icon']; ?>
          <?php endif; ?>
          <?php if ($subpanel['text']):?>
            <?php print $subpanel['text']; ?>
          <?php endif; ?> 
          <span class="signin-signout-arrow" id="signin-signout-arrow" title="<?php print $subpanel['signinbuttontext']; ?>"></span>
			          	<div class="signin-signout-container <?php print ($subpanel['signinbuttontext'] == t('MSG526')) ? 'chat-signin-bg-setting' : ''; ?>" title="<?php print $subpanel['signinbuttontext']; ?>">          
	  					  	<a class="qtip-close-button"></a>
				            <span class="change-signin-state">
				              <?php print $subpanel['signinbuttontext']; ?>
				            </span>
						</div>
          <span class="min chatMinMaxBtn titlemin" title="chatMaximizeText"></span>
        </span>
      </div>
    </div>
  </div>
</div>


<div class="subpanel" style="<?php print ($subpanel['signinstatus'] == 'TRUE') ? 'display:block': '';?>">
  <?php if ($subpanel['contents']):?>
    <?php print $subpanel['contents']; ?>
  <?php endif; ?> 
</div>