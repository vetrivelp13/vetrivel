(function($) {
  // Add javascript behaviors to the password policy admin and password strength admin forms
	try {
  Drupal.behaviors.expInitPasswordPolicyAdminForm =  {
    attach: function (context, settings) {
    	try{
      // Add behavior to the Expires After field
      $('.addedit-edit-pwdpolicy-expiration:not(.exp-pwdpolicy-initialized)').addClass('exp-pwdpolicy-initialized').each(function () {
    	try{  
        var defaultText = $(this).data('default-text');
        var fieldValue = $(this).val();
        if (fieldValue == '' || fieldValue == defaultText) {
          $(this).addClass('input-field-grey');
          $(this).val(defaultText);
        }
        else {
          $(this).removeClass('input-field-grey');
        }
  
        // Attach the event handlers
        $(this).blur(defaultText, function(event) {
          var defaultText = event.data;
          if($(this).val() == '' || $(this).val() == defaultText) {
            $(this).val(defaultText);
            $(this).addClass('input-field-grey');
          }
          else {
            $(this).removeClass('input-field-grey');
          }
        });
  
        $(this).focus(defaultText, function(event) {
          var defaultText = event.data;
          if($(this).val() == defaultText) {
            $(this).val('');
            $(this).removeClass('input-field-grey');
          }
        });
    	}catch(e){
    		// To DO
    	}
      });

      // Add behavior to the Warning Before field
      $('.addedit-edit-pwdpolicy-warning:not(.exp-pwdpolicy-initialized)').addClass('exp-pwdpolicy-initialized').each(function () {
    	try{
        var defaultText = $(this).data('default-text');
        var fieldValue = $(this).val();
        if (fieldValue == '' || fieldValue == defaultText) {
          $(this).addClass('input-field-grey');
          $(this).val(defaultText);
        }
        else {
          $(this).removeClass('input-field-grey');
        }

        // Attach the event handlers
        $(this).blur(defaultText, function(event) {
          var defaultText = event.data;
          if($(this).val() == '' || $(this).val() == defaultText) {
            $(this).val(defaultText);
            $(this).addClass('input-field-grey');
          }
          else {
            $(this).removeClass('input-field-grey');
          }
        });

        $(this).focus(defaultText, function(event) {
          var defaultText = event.data;
          if($(this).val() == defaultText) {
            $(this).val('');
            $(this).removeClass('input-field-grey');
          }
        });
    	}catch(e){
    		// To DO
    	}
      });
      
      // Add behavior to the multiselect Roles field
      $('.addedit-edit-pwdpolicy-roles:not(.exp-pwdpolicy-initialized)').addClass('exp-pwdpolicy-initialized').each(function () {
    	  try{
        if ($('.addedit-edit-pwdpolicy-roles').hasClass('error')) {
          $('.form-item-pwdpolicy-roles').addClass('exp-pwdpolicy-roles-error');
        }
    	  }catch(e){
    			// To DO
    		}
      });
      
      // Add scrollpane in password strength form
      $('#pwdstrength-admin-form-fields:not(.exp-pwdstrength-initialized)').addClass('exp-pwdstrength-initialized').each(function () {
    	try{  
        $(this).jScrollPane({enableKeyboardNavigation: false}); // enableKeyboardNavigation set to true is throwing js exception when typing in textfield
    	}catch(e){
    		// To DO
    	}
      });
      
      // Initialize Password Strength link in password policy form to launch qtip when policy id is not empty
      // Else show disabled Password Strength link
      $('#pwdpolicy-admin-form:not(.exp-pwdstrength-initialized)').addClass('exp-pwdstrength-initialized').each(function () {
    	try{  
        var pwdPolicyId = $('#pwdpolicy-pid').val();
        var pwdStrengthPopupLabel = $('#pwdpolicy-pwdstrength-label').val();
        var pwdStrengthLinkHTML = '';
        if (pwdPolicyId !== '') {
          var popupDispId = 'qtip-pwdstrength-' + pwdPolicyId;
          var lang = Drupal.settings.user.language; // language fix for the #0043857
          var defaultwidth = 480;
          // langa
          if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){ // new theme
        	  if(lang == 'fr') {
        		  defaultwidth = 650;}
        	  else if(lang == 'de') {
        		  defaultwidth = 600;}
        	  else if(lang == 'it') {
        		  defaultwidth = 630;}
        	  else if(lang == 'pt-pt') {
        		  defaultwidth = 650;}
        	  else if(lang == 'ru') {
        		  defaultwidth = 720;}
        	  else if(lang == 'es') {
        		  defaultwidth = 600;}        	  
          }else{
        	  if(lang == 'fr'){
        		  defaultwidth = 640;}
        	  else if(lang == 'de') {
        		  defaultwidth = 560;}
        	  else if(lang == 'it') {
        		  defaultwidth = 520;}
        	  else if(lang == 'ja') {
        		  defaultwidth = 520;}
        	  else if(lang == 'pt-pt') {
        		  defaultwidth = 560;}
        	  else if(lang == 'ru') {
        		  defaultwidth = 600;}
        	  else if(lang == 'es') {
        		  defaultwidth = 520;}
          }
          var qtipOptAttachmentObj  = "{'entityId' : " + pwdPolicyId +
                                     ", 'entityType' : 'pwd_stength'" +
                                     ", 'url' : 'administration/sitesetup/moduleinfo/pwdstrength/" + pwdPolicyId + "'" +
                                     ", 'popupDispId' : '" + popupDispId + "'" +
                                     ", 'catalogVisibleId' : 'qtipAttachIdqtip_pwdstrength_visible_disp_" + pwdPolicyId + "'" +
                                     ", 'wid' : '" + defaultwidth + "'"+", 'heg' : 270" +", 'postype' : 'middle'" +", 'qdis' : 'ctool'" +
          							 ", 'linkid' : 'visible-password-policy'}";	
                                     
          pwdStrengthLinkHTML = 
                '<div class="crs-tab-titles-container">' +
                  '<div class="add-pwdstrength-tab-icon"></div>' +
                  '<div id="' + popupDispId + '">' +
                    '<a id="visible-password-policy"  class="tab-title" onclick = "callVisibility(' + qtipOptAttachmentObj + ');">' +
                      pwdStrengthPopupLabel +
                    '</a>' +
                    '<span id="visible-popup-' + pwdPolicyId + '" class="qtip-popup-visible" style="display:none; position:absolute; left:0px; top:0px;" ></span>'+
                  '</div>' +
                '</div>';
          }
          else {
            pwdStrengthLinkHTML = 
                '<div class="crs-tab-titles-container">' +
                  '<div class="add-pwdstrength-tab-icon"></div>' +
                  '<div id="' + popupDispId + '">' +
                     '<a class="tab-title-disabled" onClick="return false;">' +
                       pwdStrengthPopupLabel +
                    '</a>' +
                  '</div>' +
                '</div>';
          }
          $(this).append(pwdStrengthLinkHTML);
    	}catch(e){
    		// To DO
    	}
      });
    	}catch(e){
    		// To DO
    	}
    } // end attach function
  }; // end behavior
	}catch(e){
		// To DO
	}
})(jQuery);