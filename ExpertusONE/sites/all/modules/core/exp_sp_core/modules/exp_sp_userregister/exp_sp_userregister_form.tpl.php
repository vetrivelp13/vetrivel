<?php 
global $theme_key;
if(arg(2) == "success") {
	
} else {
?>
<div class="create-lnr-container">
  <div class="create-lnr-content user-profile-box" id="reg-new-div">
  <div id="message-container"><ul><li><?php //print t('ERR016') ?></li></ul></div>
  <?php if($theme_key == 'expertusoneV2') {?>
    <table width="113%" cellpadding="0" cellspacing="0">
  <?php }else{ ?>
    <table width="105%" cellpadding="0" cellspacing="0">
  <?php }?>
    <tbody>
      <tr>
      	<td width="20%" class="reg-field-title"><?php print t('LBL056'); ?>: <span class="require-text">*</span></td>
      	<td width="80%" colspan="2">
          <?php print drupal_render($form['fname']); ?>
        	
      	</td>
      </tr>
      <!-- 
      <tr>
      	<td class="reg-field-title"><?php //print t('LBL057'); ?>:</td>
      	<td>
      	  <?php //print drupal_render($form['mname']); ?>
      	</td>
      </tr>
       -->
      <tr>
	  	<td class="reg-field-title"><?php print t('LBL058'); ?>: <span class="require-text">*</span> </td>
	  	<td  colspan="2">
    		<?php print drupal_render($form['lname']); ?>
    		
    	</td>
	</tr>
	<tr>
      	<td class="reg-field-title"><?php print t('LBL061'); ?>: <span class="require-text">*</span></td>
      	<td colspan="2">
        <?php print drupal_render($form['email']); ?>
    		
    	</td>
    </tr>
    <tr>
  		<td class="reg-field-title"><?php print t('LBL054'); ?>: <span class="require-text">*</span> </td>
  		<td colspan="2">
          <?php print drupal_render($form['name']); ?>
  		</td>
  	</tr>
  	<tr>
		<td  class="reg-field-title"><?php print t('LBL060'); ?>: <span class="require-text">*</span> </td>
		<td width="30%">
		<?php print drupal_render($form['pass']); ?>
		</td>
		<td width="50%" rowspan="6" valign="top">
		<?php print drupal_render($form['password_restriction']); ?>			
		</td>
	</tr>
	<tr>
		<td class="reg-field-title"><?php print t('LBL075'); ?>: <span class="require-text">*</span> </td>
      	<td colspan="2">
        <?php print drupal_render($form['cpass']); ?>
      	</td>
     </tr>
         <tr>
    	<td class="reg-field-title"><?php print t('LBL077'); ?>: <span class="require-text">*</span> </td>
    	<td colspan="2">
    		<div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
    				<?php print drupal_render($form['tzone']); ?>
    			</div>
    		</div>
    	</td>
   </tr>
      <?php if (variable_get('registration_Language') == '1') { ?>
   <tr>
    	<td class="reg-field-title"><?php print t('LBL038'); ?>: </td>
    	<td colspan="2">
    		<div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
    				<?php print drupal_render($form['lang']); ?>
    			</div>
    		</div>
    	</td>
   </tr><?php }?>
<?php if (variable_get('registration_Currency') == '1') { ?>
   <tr>
    	<td class="reg-field-title"><?php print t('LBL101'); ?>: </td>
    	<td colspan="2">
    		<div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
    				<?php print drupal_render($form['currency']); ?>
    			</div>
    		</div>
    	</td>
   </tr><?php }?>
     <?php if (variable_get('registration_Phone') == '1') { ?>
     <tr>
        <td class="reg-field-title"><?php print t('LBL070'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['phoneno']); ?>
    	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_Mobile') == '1') { ?>
      <tr>
        <td class="reg-field-title"><?php print t('Mobile'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['mobile']); ?>
    	</td>
    </tr> <?php }?>
    <?php if (variable_get('registration_Address 1') == '1') { ?>
     <tr>
        <td class="reg-field-title"><?php print t('LBL064'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['addr1']); ?>
    	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_Address 2') == '1') { ?>
     <tr>
        <td class="reg-field-title"><?php print t('LBL065'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['addr2']); ?>
    	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_City') == '1') { ?>
     <tr>
        <td class="reg-field-title"><?php print t('LBL066'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['city']); ?>
    	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_Zip Code') == '1') { ?>
    <tr>
     <tr>
        <td class="reg-field-title"><?php print t('LBL562'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['zipcode']); ?>
    	</td>
    </tr><?php }?>
    <?php 	if (variable_get('registration_State') == '1') { ?>
     <tr>
        <td class="reg-field-title"><?php print t('LBL152'); ?>: </td>
        <td colspan="2">
        <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['state']); ?>
				</div>
			</div>
    	</td>
    </tr><?php } ?>
     <?php if (variable_get('registration_Country') == '1') { ?>
    <tr>
        <td class="reg-field-title"><?php print t('LBL039'); ?>: </td>
        <td colspan="2">
        <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['country']); ?>
				</div>
			</div>
    	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_Organization') == '1') { ?>
        <tr>
        <td class="reg-field-title"><?php print t('Organization'); ?>: </td>
        <td colspan="2">
        <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['org']); ?>
				</div>
			</div>
    	</td>
    </tr><?php } ?>
 <?php if (variable_get('registration_Employment Type') == '1') {
 	?>   
            <tr>
        <td class="reg-field-title"><?php print t('LBL174'); ?>: </td>
        <td colspan="2">
         <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['emptype']); ?>
				</div>
			</div>
    	 	</td>
    </tr><?php } ?>
    
    <?php if (variable_get('registration_Department') == '1') { ?>
        <tr>
        <td class="reg-field-title"><?php print t('LBL179'); ?>: </td>
        <td colspan="2">
        <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['dept']); ?>
				</div>
			</div>
			</td>
    </tr><?php } ?>
    
    <?php  if (variable_get('registration_Employee ID') == '1') { ?>
           <tr>
        <td class="reg-field-title"><?php print t('LBL294'); ?>: </td>
        <td colspan="2">
    	<?php print drupal_render($form['empid']); ?>
    	</td>
    </tr><?php }?>

    <?php if (variable_get('registration_User Type') == '1') { ?>
         <tr>
        <td class="reg-field-title"><?php print t('LBL173'); ?>: </td>
        <td colspan="2">
        <div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['usrtype']); ?>
				</div>
			</div>
    	</td>
    </tr><?php }?>
    <!--<tr>
      	<td width="14%" class="reg-field-title"><?php print t('Location'); ?> : </td>
      	<td  >
          <?php print drupal_render($form['prefloc']); ?>
      	</td>
    </tr>-->
    <?php if (variable_get('registration_Job Title') == '1') { ?>
    <tr>
      	<td class="reg-field-title"><?php print t('LBL073'); ?>:
      	<td colspan="2">
			<div class="expertus-dropdown-bg">
				<div class="expertus-dropdown-icon ">
					<?php print drupal_render($form['jtitle']); ?>
				</div>
			</div>
        	
      	</td>
    </tr><?php }?>
    <?php if (variable_get('registration_Job Role') == '1') { ?>
        		 
    <tr>
    	<td class="reg-field-title"><?php print t('LBL133'); ?>:</td>
    	<td colspan="2">
    	<?php print drupal_render($form['load_multiselect_jobrole']); ?>
    		</td>
   </tr><?php }?>
    <?php  print drupal_render($form['captcha_test']); ?>
      <tr>
		<td>&nbsp;</td>
        <td class="reg-field-title" colspan="2">
          <div class="receive-news-txt">
          	<?php print drupal_render($form['sub_check']); ?>
          	<div class="checkbox-space-new-theme"><?php print t('LBL150'); ?></div>
          </div>
        </td>
	  </tr> 
      <tr>
      	<td>&nbsp;</td>
      	<td colspan="2">
      	  <span class="reg-link-back" style="float:left;"> 
      	  <?php if($theme_key == 'expertusoneV2') {?>
      	  	<div class="white-btn-bg-container">  
	      	  	<div class="white-btn-bg-left"></div>
				<a class="white-btn-bg-middle" href="javascript:void(0);" onclick="history.go(-1);">&nbsp;<?php print '&nbsp;&nbsp;'.t('LBL109').'&nbsp;&nbsp;';?>&nbsp;</a>
				<div class="white-btn-bg-right"></div>
			</div>
          <?php }else{?>
			<a href="javascript:void(0);" onclick="history.go(-1);">&nbsp;<?php print '[&nbsp;&nbsp;'.t('LBL109').'&nbsp;&nbsp;]';?>&nbsp;</a>
          <?php }?>
          </span>
          <span class="register-user-btn-cont">		
            <?php print drupal_render($form['register']); ?>
          </span>
      	</td>
      </tr>
    </tbody>
    </table>
    <?php print drupal_render_children($form); ?>
	</div>
</div>
<?php } ?>
