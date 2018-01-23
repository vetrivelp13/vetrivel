(function($) {
$.widget("ui.mobileapp", {
		_init: function() {	
			try{
				
			}catch(e){
				// to do
			}
},

mobileAppSendLink: function(type){
	try{
	var obj = this;
	var loaderObj = 'mobile-app-download';	
	var email = $('#mobile-app-download-email').val();
	var errorMessages = new Array();
	if(email.length == 0){
		errorMessages[0] = Drupal.t('ERR002');
		var message_call = expertus_error_message(errorMessages,'error');
		$('#show_expertus_message').html(message_call);
		$('#show_expertus_message').show();
	}
	else{
		this.createLoader(loaderObj);
		url = obj.constructUrl("ajax/send-mobile-app-link/"+ type + '/' + email);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				obj.destroyLoader(loaderObj);
				if(result.notify_id){
					var html = Drupal.t('MSG613')+" "+ email + '.';
					html += '<br/> <br/>';
					html += Drupal.t('MSG614')+" " + type +" "+Drupal.t('MSG615');
					$('#learning-mobile-app-send-link').html(html);
				}
				else{
					errorMessages[0] = Drupal.t('ERR002');
					var message_call = expertus_error_message(errorMessages,'error');
					$('#show_expertus_message').html(message_call);
					$('#show_expertus_message').show();
				}
			}
		});
	}
	}catch(e){
		// to do
	}
},

mobileAppStore: function(type){
	try{
	if(type == 'android'){
		window.open(decodeURIComponent(resource.android_download_url),'_blank',"location=1,status=1,resizable =1,scrollbars=1");
	}else{
		window.open(decodeURIComponent(resource.iphone_download_url),'_blank',"location=1,status=1,resizable =1,scrollbars=1");
	}
	}catch(e){
		// to do
	}
}

});
$.extend($.ui.mobileapp.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});
})(jQuery);
$(function() {
	try{
	$("body").mobileapp();
	}catch(e){
		// to do
	}
});	
