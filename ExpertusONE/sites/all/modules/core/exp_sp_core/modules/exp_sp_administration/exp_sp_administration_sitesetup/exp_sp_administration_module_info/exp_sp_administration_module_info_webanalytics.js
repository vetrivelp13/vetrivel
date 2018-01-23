(function($) {
 try{	
  //Add javascript behavior to the web analytics admin form
  Drupal.behaviors.expInitWebAnalyticsAdminForm =  {
    attach: function (context, settings) {
    try{	
      // Add behavior to the Web Property ID field
      $('.addedit-edit-googleanalytics_account:not(.exp-webanalytics-initialized)').addClass('exp-webanalytics-initialized').each(function () {
    	try{  
        var defaultText = $('#googleanalytics_account_default_text').data('default-text');
        var accountId = $(this).val();
        if (accountId == '' || accountId == defaultText) {
          $(this).addClass('input-field-grey');
          $(this).val(defaultText);
        }
        else {
          $(this).removeClass('input-field-grey');
        }
    
        // Attach the event handlers
        $(this).blur(defaultText, function(event) {
        try{	
          var defaultText = event.data;
          if($(this).val() == '' || $(this).val() == defaultText) {
            $(this).val(defaultText);
            $(this).addClass('input-field-grey');
          }
          else {
            $(this).removeClass('input-field-grey');
          }
        }catch(e){
    		// To DO
    	}
        });
    
        $(this).focus(defaultText, function(event) {
        try{	
          var defaultText = event.data;
          if($(this).val() == defaultText) {
            $(this).val('');
            $(this).removeClass('input-field-grey');
          }
        }catch(e){
    		// To DO
    	}
        });
    	}catch(e){
    		// To DO
    	}
      });
      
      // Alter Domain Names field based on the value of Domain Tracking field
      $('.addedit-edit-googleanalytics_domain_mode:not(.exp-webanalytics-initialized)').addClass('exp-webanalytics-initialized').each(function () {       
        // Attach the event handlers
        $(this).change(function() {
        try{	
          var domainsField = $('.addedit-edit-googleanalytics_cross_domains');
          var domainMode = $(this).val();
          if (domainMode == 1) {
            var singleDomainName = $(this).data('single-domain-name');
            domainsField.val(singleDomainName);
            domainsField.removeClass('input-field-grey');
            domainsField.attr('readonly', 'readonly');
            domainsField.attr('disabled', 'disabled');
            domainsField.css('resize', 'none');
            domainsField.css('height', '26px');
          }
          else if (domainMode == 2) {
            var domainWithSubdomains = $(this).data('domain-with-subdomains');
            domainsField.val(domainWithSubdomains);
            domainsField.removeClass('input-field-grey');
            domainsField.attr('readonly', 'readonly');
            domainsField.attr('disabled', 'disabled');
            domainsField.css('resize', 'none');
            domainsField.css('height', '26px');          
          }
          else {
            var defaultText = $(this).data('default-text');
            domainsField.addClass('input-field-grey');
            domainsField.val(defaultText);
            domainsField.removeAttr('readonly');
            domainsField.removeAttr('disabled');
            domainsField.css('resize', '');
          }
          Drupal.ajax.prototype.commands.CtoolsModalAdjust();
        }catch(e){
    		// To DO
    	}
        });
      });
      
      // Add behavior to the Domain Names field
      $('.addedit-edit-googleanalytics_cross_domains:not(.exp-webanalytics-initialized)').addClass('exp-webanalytics-initialized').each(function () {
    	try{  
        var defaultText = $('.addedit-edit-googleanalytics_domain_mode').data('default-text');
        var domainMode = $('.addedit-edit-googleanalytics_domain_mode').val();
        if (domainMode == 1 || domainMode == 2) {
          $(this).attr('readonly', 'readonly');
          $(this).attr('disabled', 'disabled');
          $(this).css('resize', 'none');
        }
        else {
          var domains = $(this).val();
          if (domains == '' || domains == defaultText) {
            $(this).addClass('input-field-grey');
            $(this).val(defaultText);
          }
          else {
            $(this).removeClass('input-field-grey');
          }
          $(this).removeAttr('readonly');
          $(this).removeAttr('disabled');
          $(this).css('resize', '');
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
      
      // Add behavior to the Pages field
      $('.addedit-edit-googleanalytics_pages:not(.exp-webanalytics-initialized)').addClass('exp-webanalytics-initialized').each(function () {
    	 try{ 
        var defaultText = $('.addedit-edit-googleanalytics_visibility_pages').data('default-text');
        var pages = $(this).val();
        if (pages == '' || pages == defaultText) {
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
      
      // Add behavior to the multiselect Roles to Track field
      $('.addedit-edit-googleanalytics_roles:not(.exp-webanalytics-initialized)').addClass('exp-webanalytics-initialized').each(function () {
        if ($('.addedit-edit-googleanalytics_roles').hasClass('error')) {
          $('.form-item-googleanalytics-roles').addClass('exp-googleanalytics-roles-error');
        }
      });
    }catch(e){
		// To DO
	}
    }
  };
 }catch(e){
		// To DO
	}
})(jQuery);