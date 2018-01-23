(function($) {

$.widget('ui.requestclass', {
	_init: function() {
		try{
		var self = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		this.referObj = '$("body").data("requestclass")';
		}catch(e){
			// to do
		}
	},
	
	openRequestClassDialog: function(courseId, loaderDiv) {
		try{
		closeQtip('');	//to close all open qtips. for 042549: Share pop up is not automatically closing...
		this.createLoader(loaderDiv);
		var url = this.constructUrl('ajax/request-class-details/' + courseId);
		var obj = this;
		$.ajax({
			type: 'POST',
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result) {
				obj.destroyLoader(loaderDiv);
				$('#request_class_container').css('overflow', 'normal');
				obj.paintRequestClassDialog(result);
			}
	  });
		}catch(e){
			// to do
		}
	},

	paintRequestClassDialog: function(result) {
		try{
		var rhtml =
		  '<form id="form-refer" method="post">' +
		    '<div id="show_expertus_message" style="display:none;"></div>' +
		      '<table cellpadding="0" cellspacing="0" border="0" class="refer-class-table" width="100%">' +
            '<tr>' +
              '<td class="refer-class-col1">' + result['labelmsg']['subject'] + ': </td>' +
              '<td class="refer-class-col2"></td>' +
              '<td class="refer-class-col3">' +
                '<input type="text" id="rc-subject" class="input-text" readonly tabindex="1" value="' + result['code'] + ' - ' + result['title'] + '">' +
              '</td>' +
            '</tr>' +
            '<tr>' +
              '<td class="refer-class-col1 req-class-msg-title">' + result['labelmsg']['message'] + ': </td>' +
              '<td class="refer-class-col2"></td>' +
              '<td class="refer-class-col3 req-class-msg-textarea">' +
                '<textarea class="input-textarea" rows="5" cols="40" name="description" id="edit-description" class="input-textarea" tabindex="2"></textarea>' +
              '</td>' +  
		        '</tr>' +
            '<tr class="last">' +
              '<td class="refer-class-col1">&nbsp;</td>' +
              '<td class="refer-class-col2">&nbsp;</td>';
        if (this.currTheme == "expertusoneV2") {
        rhtml +=
              '<td class="refer-class-col3">' +
                '<div class="checkbox-unselected">' +
                  '<input type="checkbox" id="rc-ccopy" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="4">' +
                '<span class="check-label">' + 
                  result['labelmsg']['ccme'] +
                '</span>' +
                '</div>' +
              '</td>';
        }
        else {
        rhtml +=
              '<td class="refer-class-col3">' +
                //'<div>' +
                '<input type="checkbox" id="rc-ccopy" tabindex="4">' +
                  '<span class="check-label">' +
                    result['labelmsg']['ccme'] +
                  '</span>' +
                //'</div>' +
              '</td>';
        }
        rhtml +=
            '</tr>' +
          '</table>' +
        '</form>';
		
		var noteTxt = '<div class="refer-note">' +
		              '</div>';
		var dlgDiv = '<div id="request_class_container" class="refer-class-container"></div>';
		$('#request_class_container').remove();
		$("#refer_course_holder").remove();
		$('body').append(dlgDiv);
		
		$('#request_class_container').html(rhtml);

		var closeButt = {};
		closeButt[result['labelmsg']['close']] = function() {
		                                           $(this).dialog('destroy');
		                                           $(this).dialog('close');
		                                           $('#request_class_container').remove();
		                                         };
		closeButt[result['labelmsg']['submit']] = function() {
			                                          $('body').data('requestclass').callRequestClassSubmit(result['courseId']);
		                                          };

		$('#request_class_container').dialog({
			bgiframe: true,
			width: 376,
			resizable: false,
			draggable: false,
			closeOnEscape: false,
			modal: true, 
			title: result['labelmsg']['dialog-title'],
			buttons: closeButt,
			close: function() {
				$('#request_class_container').remove();
				$('#request_class_holder').remove();
			},
			overlay: {
			   opacity: 0.4,
			   background: '#000000'
			}	
		});
		if (($.browser.msie && this.currTheme == 'expertusoneV2' &&
		      (parseInt($.browser.version, 10) == '9' || parseInt($.browser.version, 10) == '8'))) {
		  //$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
		}
		
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass('admin-save-button-middle-bg');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('removebutton').end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr('tabindex', 6);
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('tabindex', 7);
		
		if (this.currTheme == 'expertusoneV2') {
		  changeDialogPopUI();
		}
		
		$('.ui-dialog').wrap('<div id="request_class_holder" class="refer-course-holder"></div>');
		$('#request_class_holder a.ui-dialog-titlebar-close').html('X');
		$('.ui-dialog-buttonset').before(noteTxt);
		$('.request-class-container').css('width', '93.9%');
		}catch(e){
			// to do
		}
	},

	callRequestClassSubmit: function(courseId) {
	 try{
	  var obj = this;
	  var inc = 0; 
	  var eCount = 0;
	  var errMsg = new Array();
	  errMsg[0] = '';
	  
	  var taComment = $('#edit-description').val();
	  var chkCcopy = $('#rc-ccopy').attr('checked');
	  
	  if ($('#request_class_container')) {
	    this.createLoader('request_class_container');
	    $('#request_class_container').css('overflow', 'hidden');
	  }
	  
	  var url = this.constructUrl('ajax/request-class-submit/' + courseId + '/' + escape(taComment) + '/' + chkCcopy);
	  $.ajax({
	    type: "POST",
	    url: url,
	   // data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
	    success: function(result) {
	      obj.destroyLoader('request_class_container');
	      $('#request_class_container').html('<div class="rc-success-message">' + result['labelmsg']['successmsg'] + '</div>');
	      $('.ui-dialog-buttonset').css('margin-right', '0px');	        
	      $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('shareclosetext').end();
	      $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').css('display', 'none');
	      $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').prev('.admin-save-button-left-bg').css('display', 'none');
	      $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').next('.admin-save-button-right-bg').css('display', 'none');
	      $('.refer-note').remove();
	    }
	  });
	 }catch(e){
			// to do
		}
	}
});

$.extend($.ui.requestclass.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
 try{	
  $('body').requestclass();
 }catch(e){
		// to do
}
});