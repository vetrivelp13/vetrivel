(function($) {

$.widget("ui.printcertificate", {
	_init: function() {
		try{
				var self = this;
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				this.referObj = '$("body").data("printcertificate")';
			}catch(e){
			// to do
		}
	},

	getPrintcertificateDetails :function(enrollid,classid,userid,certifyfrom,loaderDiv,popup_width,popup_height){
		try{
			
			//Added by ganesh
			/*var sf_exp_ses_value = $('.salesforce-widget #widget').attr('data-exp-sess-id'); 
			var exp_ses_var="";
			if (typeof sf_exp_ses_value !== typeof undefined && sf_exp_ses_value !== false) {
				exp_ses_var="&exp_sess_id="+sf_exp_ses_value;
			}*/
			
			this.createLoader(loaderDiv);
			//var url = "printcertificate.php?enrollid="+enrollid+"&classid="+classid+"&userid="+userid+"&certifyfrom="+certifyfrom+"&from_salesforce=1"+exp_ses_var;
			var url = "printcertificate.php?enrollid="+enrollid+"&classid="+classid+"&userid="+userid+"&certifyfrom="+certifyfrom+"&from_salesforce=1";
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader(loaderDiv);
					$('#print_certificate_container').css('overflow','normal');
					//var result="Test Print Certificate";
					obj.paintprintcertificate(result,popup_width,popup_height);
					$('span.closetext').hide();
					$('span.closetext').css('ckear','both');
				}
		    });
		}catch(e){
			// to do
		}
	},

	paintprintcertificate: function(result,popup_width,popup_height){
	 try{
		res = result;
		var obj = this;

		var rhtml = '';

		rhtml = res;

		var dlgDiv = '<div id="print_certificate_container" class="print_certificate_container"></div>';
		$('#print_certificate_container').remove();
		$("#print_certificate_holder").remove();

		$('body').append(dlgDiv);

		$('#print_certificate_container').html(rhtml);

		var closeButt={};

		$("#print_certificate_container").dialog({
			bgiframe: true,
			width:popup_width,
			height:popup_height,
			resizable:false,
			draggable:false,
			closeOnEscape: false,
			modal: true,
			//title: Drupal.t('Print Certificate'),
			title: Drupal.t('SFLBL001'),
			buttons: closeButt,
			close: function(){
				$("#print_certificate_container").remove();
				$("#print_certificate_holder").remove();
			},
			overlay:
			{
			   opacity: 0.4,
			   background: '#000000'
			 }
		});

		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
		}

		$('.ui-dialog').wrap("<div id='print_certificate_holder' class='print_certificate_holder'></div>");
		$('#print_certificate_holder a.ui-dialog-titlebar-close').html('X');

		if(this.currTheme == "expertusoneV2"){
			$('#print_certificate_container').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
		}

		$('#print_certificate_container').css('height','653px');
	 }catch(e){
			// to do
	 }
	}

});

$.extend($.ui.printcertificate.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
	$("body").printcertificate();
	
	}catch(e){
		// to do
	}
});
