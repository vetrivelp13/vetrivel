(function($) {
  //Add javascript behavior to the credit card settings admin form
  Drupal.behaviors.expCcSettingsAdminForm =  {
    attach: function (context, settings) {
     try{	
      // Add behavior to the Web Property ID field
      $('.addedit-edit-uc_credit_encryption_path:not(.exp-cc-settings-initialized)').addClass('exp-cc-settings-initialized').each(function () {
        try{
    	var defaultText = $(this).data('default-text');
        var ucDefaultText = $(this).data('uc-default-text');
        var encrKeyPath = $(this).val();
        if (encrKeyPath == '' || encrKeyPath == defaultText || encrKeyPath == ucDefaultText) {
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
            //Nothing to do
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
            //Nothing to do
          }
        });
        }catch(e){
            //Nothing to do
          }
      });
      
      // Add behavior to the multiselect card types field
      $('.addedit-edit-exp-card-types:not(.exp-cc-settings-initialized)').addClass('exp-cc-settings-initialized').each(function () {
    	try{  
        if ($('.addedit-edit-exp-card-types').hasClass('error')) {
          $('.form-item-exp-card-types').addClass('exp-card-types-error');
        }
    	}catch(e){
    	       //Nothing to do
    	 }
      });
     }catch(e){
         //Nothing to do
      }
    }
  };
})(jQuery);