var myBookmark;
function changeNotificationFrequencyType(){
 try{	
  themeKey = Drupal.settings.ajaxPageState.theme;
  if($('#two-col-row-freq_type').find('.addedit-edit-notify_type option:selected').val() == 'Scheduled'){
	  if(themeKey == 'expertusoneV2'){
		  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').css('width','115px');
		  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-bg').css('width','100px'); 
		  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-icon').css('width','100px'); 
		  $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','126px');
		  $('#two-col-row-freq_type').find('.notification-scheduled-days').css('width','30px');
	  }else{
		  $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','116px');
	  }
		$('#admin-frequency-data-part').show();
		$('#admin-frequency-data-part-expires').show();
	}
	else{
		if(themeKey == 'expertusoneV2'){
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').css('width','174px');
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-bg').css('width','186px'); 
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-icon').css('width','186px');
		    $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','210px');
		}else{
			$('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','170px');
		}
		$('#admin-frequency-data-part').hide();
	}
 }catch(e){
	 // to do
 }
}

function changeNotificationFrequencyTypenew(){
	 try{	
	  themeKey = Drupal.settings.ajaxPageState.theme;
	  if($('#two-col-row-freq_type').find('.addedit-edit-notify_type option:selected').val() == 'Scheduled'){
		  //alert('checking');
		  /*if(themeKey == 'expertusoneV2'){
			  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').css('width','115px');
			  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-bg').css('width','100px'); 
			  $('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-icon').css('width','100px'); 
			  $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','116px');
			  $('#two-col-row-freq_type').find('.notification-scheduled-days').css('width','30px');
		  }else{
			  $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','116px');
		  }
			$('#admin-frequency-data-part').show();
			$('#admin-frequency-data-part-expires').show();*/
		  $('#two-col-row-not_sched_days').show();
		}
		else{
			/*if(themeKey == 'expertusoneV2'){
				$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').css('width','174px');
				$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-bg').css('width','186px'); 
				$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-icon').css('width','186px');
			    $('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','202px');
			}else{
				$('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','170px');
			}
			$('#admin-frequency-data-part').hide();*/
			$('#two-col-row-not_sched_days').hide();
		}
	 }catch(e){
		 // to do
	 }
	}

(function($) {
	try{
	$.fn.validateNotificationFrequencyType = function(notifyType,freqDays,notifyCode) {
		if($('#two-col-row-freq_type').find('.addedit-edit-notify_type option:selected').val() == 'Scheduled'){
			if(notifyCode != 'compliance_class_remind_session' && notifyCode != 'mandatory_class_remind_session' && notifyCode != 'compliance_expiry_remainder'){
			$('#two-col-row-freq_type').find('.addedit-edit-notify_type').css('width','126px');
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').css('width','115px');
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-bg').css('width','100px'); 
			$('#two-col-row-freq_type').find('.addedit-firstcol-field-value').find('.expertus-dropdown-icon').css('width','100px');
			$('#admin-frequency-data-part').show();
			$('#admin-frequency-data-part-expires').show();
	}
	}
	//return false;
};
	}catch(e){
		 // to do
	 }
})(jQuery);

function inbutBoxOnClick(idTypeValue){
	try {
	var selectId = $(idTypeValue).attr('id');
	$("#cursor_id").val(selectId);
	}catch(e){
		 // to do
	 }
}

(function($) {
	try {
  $.fn.templateCloseCancel = function(templateId,entityType) {
    
	if(document.getElementById('qtipAttachNotification_addtemplate_visible_disp_'+templateId)){
	  
	  $('#qtipAttachNotification_addtemplate_visible_disp_'+templateId+'_disp').closest('.qtip-active').remove();
	  
	}
	if(document.getElementById('qtipAttachNotification_addtemplate_visible_disp__')){

	  
	  $('#qtipAttachNotification_addtemplate_visible_disp__disp').closest('.qtip-active').remove();
	}
	if(document.getElementById('qtipAttachNotification_edittemplate_visible_disp_'+templateId)){

	 
	  $('#qtipAttachNotification_edittemplate_visible_disp_'+templateId+'_disp').closest('.qtip-active').remove();
	  
	}	
		
  };
	}catch(e){
		 // to do
	 }
})(jQuery);

function editTemplateDetails(notificationId, templateId){
	try {
		if($(".qtip-active").length > 0)
			$(".qtip-active").remove();
	var classVisibility = $('#template-basic-addedit-form-'+templateId).is(':visible');
	$('#catalog-class-addedit-form-details .template-basic-addedit-form-'+templateId).hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if(classVisibility == true){
		$('#template-basic-addedit-form-'+templateId).hide();
	} else {
		$('#template-basic-addedit-form-'+templateId).show();
		$('#edit-class-list-'+templateId).css('background-color','#d7ecf9');
	}
	}catch(e){
		 // to do
	 }
}
$('body').click(function (event) {
	try {
	if(event.target.id!='pub-unpub-action-btn-notify') {
		$('.catalog-pub-not-add-list').hide();
	}
	}catch(e){
		 // to do
	 }
});

Drupal.behaviors.notificationAdminCcField =  {
  attach: function (context, settings) {
	try {  
    $('.addedit-edit-cc_mail:not(.exp-noti-inited)').addClass('exp-noti-inited').each(function () {
      var emptyText = $(this).data('empty-text');
      var fieldValue = $(this).val();
      if (fieldValue == '' || fieldValue == emptyText) {
        $(this).val(emptyText);
        $(this).removeClass('cc-mail-notempty');
        $(this).addClass('cc-mail-empty');
      }
      else {
        $(this).removeClass('cc-mail-empty');
        $(this).addClass('cc-mail-notempty');
      }
  
      $(this).blur(function() {
        var emptyText = $(this).data('empty-text');
        var fieldValue = $(this).val();
        if (fieldValue == '' || fieldValue == emptyText) {
          $(this).val(emptyText);
          $(this).removeClass('cc-mail-notempty');
          $(this).addClass('cc-mail-empty');
        }
      });
      
      $(this).focus(function() {
        var emptyText = $(this).data('empty-text');
        var fieldValue = $(this).val();
        if (fieldValue == emptyText) {
          $(this).val('');
          $(this).removeClass('cc-mail-empty');
          $(this).addClass('cc-mail-notempty');
        }
      });
    });
    $('.addedit-edit-sms_text:not(.exp-noti-inited)').addClass('exp-noti-inited').each(function () {
	      var emptyText = $(this).data('empty-text');
	      var fieldValue = $(this).val();
	      if (fieldValue == '' || fieldValue == emptyText) {
	        $(this).val(emptyText);
	        $(this).removeClass('cc-mail-notempty');
	        $(this).addClass('cc-mail-empty');
	      }
	      else {
	        $(this).removeClass('cc-mail-empty');
	        $(this).addClass('cc-mail-notempty');
	      }
	  
	      $(this).blur(function() {
	        var emptyText = $(this).data('empty-text');
	        var fieldValue = $(this).val();
	        if (fieldValue == '' || fieldValue == emptyText) {
	          $(this).val(emptyText);
	          $(this).removeClass('cc-mail-notempty');
	          $(this).addClass('cc-mail-empty');
	        }
	      });
	      
	      $(this).focus(function() {
	        var emptyText = $(this).data('empty-text');
	        var fieldValue = $(this).val();
	        if (fieldValue == emptyText) {
	          $(this).val('');
	          $(this).removeClass('cc-mail-empty');
	          $(this).addClass('cc-mail-notempty');
	        }
	      });
	    });
	}catch(e){
		 // to do
	 }
  }
};
