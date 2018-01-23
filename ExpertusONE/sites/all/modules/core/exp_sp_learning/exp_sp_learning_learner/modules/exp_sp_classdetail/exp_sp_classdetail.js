(function($) {

$.widget("ui.lnrclassdetails", {
	
	_init: function() {
	 try{	
		var self = this;
		this.populateClassDetails();
	 }catch(e){
		 // to do
	 }
	},
	
	populateClassDetails: function() {
		try{
		var objStr = '$("#class_detail_content").data("lnrclassdetails")';
		$('#class_details_display_area').show();	
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();
		var user_id = this.getLearnerId();
		if(user_id == "" || user_id == "0"){
			
		    $('.clsdet-node .fivestar-click').removeClass('fivestar-click');	    
	        $('.clsdet-node .fivestar-widget').unbind('mouseover');
	        $('.clsdet-node .fivestar-widget .star a').unbind('click');
	        $('.clsdet-node .fivestar-widget .star a').css('cursor','auto');
		}
		//after login auto register or add to cart related work start
		//console.log(user_id);
		if(user_id != 0 && user_id !='' && user_id != null && user_id!=undefined){
			setTimeout(function(){
				 var user_selected_class_id = $.cookie("user_selected_class_id");
				 if(user_selected_class_id != null  && user_selected_class_id !=undefined){
					 var onclickprop = $('#'+user_selected_class_id).attr("onclick");
					 if(onclickprop != null &&  onclickprop !=''  && onclickprop!=undefined)
						 $('#'+user_selected_class_id).click();
					 else
						 $("body").data("learningcore").callMessageWindow(Drupal.t('LBL721'),Drupal.t('ERR047'));
					 $.cookie("user_selected_class_id",'',{expires: -300});
					 $.cookie("user_selected_url", '',{ expires: -300 });
				 }
				 var user_selected_page_number = $.cookie("user_selected_page_number");
				 if(user_selected_page_number != null && user_selected_page_number !=undefined)
					 $.cookie("user_selected_page_number",'',{expires: -300});
				 var user_selected_row_number = $.cookie("user_selected_row_number");
				 if(user_selected_row_number != null && user_selected_row_number !=undefined)
					$.cookie("user_selected_row_number",'',{expires: -300});
			 }, 1000);
		}
		else {
			$('#signin').click();
		}
		//after login auto register or add to cart related work end
		if(availableFunctionalities.exp_sp_forum){
			/* call to function in forum.js to render the forum topics */
			var contId = $("#tdataClsParentId").val();
			var frm = "Class";
			var newObj = {id:contId,src:frm};
	    	$('#forum-topic-list-display').data('forumlistdisplay').renderForumTopicResults(newObj);
	    }
		}catch(e){
			 // to do
		 }
	},
	

	openAttachment : function(url){
		try{
			url = prepareFileURL(url);
		var woption = "width=800,height=900,toolbar=no,location=yes,status=yes,menubar=no,scrollbars=yes,resizable=1";
		window.open(url, "_blank", woption);
		}catch(e){
			 // to do
		 }
	}

});
//this code widget as well as core will change but no problem for core also.
var tablewidth = $('#class_details_display_area .detail-item-row').width()-10;
$('#class_detail_content .code-container table.class-session-details').css('width',tablewidth+'px');
//this code widget as well as core will change but no problem for core also.
$.extend($.ui.lnrclassdetails.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
	$( "#class_detail_content" ).lnrclassdetails();
	$('.lmt-cls-title').trunk8(trunk8.class_detail_title);
	$('.lmt-cls-desc').trunk8(trunk8.class_detail_desc);
	$('.lmt-cls-desc-add').trunk8(trunk8.class_detail_add_desc);
	
	resetInstructorFadeout('.left-section','class');
    if(Drupal.settings.widget.widgetCallback==true){
    	resetFadeOutByClass('#class_detail_content','content-detail-code','line-item-container','class_details');
    	resetFadeOutByClass('#class_detail_content','content-prereq-container','prerequisite-block','class_details');
        resetFadeOutByClass('#class_detail_content','course-block-container','course-code-row','class_details'); 
	 	   $('.detail-item-row-left').find('.detail-desc').each(function(){
	 		   if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
	 	    		$(this).find('.fade-out-image').remove();
	 	    	}
	 			 
	 		});
	    }
	}catch(e){
		 // to do
	 }
});
