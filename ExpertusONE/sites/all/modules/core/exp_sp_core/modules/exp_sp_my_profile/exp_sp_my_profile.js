function profileSubmitUrl() {
	try{
	$("#myprofile_done_btn").click();
	}catch(e){
		// to do
	}
}

function remove_statusmessages(){
	try{
	$('#wizard-form-wrapper .messages').remove();
	}catch(e){
		// to do
	}
}

(function($) {
	try{
	$.fn.addSkillGrayConversion = function(skillCount) {
		$(".addedit-skillfields-wrapper").find('.input-field-grey').each(function(){
		    var gId= $(this).attr("id");
		    var gVal = $("#"+gId).val();
		    if(gVal == 'Type a skill')  {
		        $("#"+gId).removeClass('input-field-grey');
		    	$("#"+gId).addClass('input-field-grey');
		    } else {
		    	$("#"+gId).removeClass('input-field-grey');
		    }
		});
	};
	}catch(e){
		// to do
	}
})(jQuery);

//Initialize and set behaviour for custom label and value fields.
Drupal.behaviors.skillAddField =  {
  attach: function (context, settings) {
	try{  
    var filter = '.skill-text-box:not(.skill-field-initialized)';
    $(filter).addClass('skill-field-initialized').each(function () {
      var defaultText = $(this).data('default-text');
      if ($(this).val() == '' || $(this).val() == defaultText) {
        $(this).val(defaultText);
        $(this).addClass('input-field-grey');
      }
      else {
        $(this).removeClass('input-field-grey');
      }
      // Attach the event handlers
      $(this).blur(defaultText, function(event) {
        var defaultText = event.data;
        //console.log('blurred. default text = ' + defaultText);
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
        //console.log('gained focus. default text = ' + defaultText);
        if($(this).val() == defaultText) {
          $(this).val('');
          $(this).removeClass('input-field-grey');
        }
     });
   });
	}catch(e){
		// to do
	}
  }
};

function toggleProfileDesc(id,opt){
	try{
	$('#learner-myprofile-details .short-desc-toggle').hide();
	$('#learner-myprofile-details .long-desc-toggle').hide();
	if(opt == 2){
		$('#learner-myprofile-details .short-desc-toggle').show();
	}else{
	  $('#learner-myprofile-details .long-desc-toggle').show();
	}	
	}catch(e){
		// to do
	}
}
(function($) {
	$.fn.uploadProfilePicture = function() {
		try{
		var tag='';
			if(document.getElementById('del-pic-anchor') != null){
				tag = document.getElementById('del-pic-anchor').tagName;
			}else if(document.getElementById('add-pic-anchor') != null){
				tag = document.getElementById('add-pic-anchor').tagName;
			}
			if((navigator.userAgent).indexOf('MSIE')>0 && (tag != '')){
				if(tag.toLowerCase()=='span'){
					var x = document.getElementById('user-profile-add-picture-id');
					$('#myprofile_upload_file').css('left',x.offsetLeft);
				}
			}
		}catch(e){
			// to do
		}
  };
  Drupal.ajax.prototype.commands.initializeVtipMyProfile = function(ajax, response, status) {
	    try {
	      vtip();
	    }
	    catch(e){
		      //Nothing to do
		}
		   
};
})(jQuery);

$(document).ready(function() {
	try{
	$.fn.uploadProfilePicture();
	var width = $(".exp-sp-myprofile-job-title-fadeout-container").width();
	if(width < 220) {
	  var remaing_width = 210 - width ;
	 var max_width =  Math.max.apply(Math, $('.exp-sp-myprofile-org-name-fadeout-container').map(function(){ return $(this).width(); }).get());
	  var updated_with = max_width+remaing_width;
	  $('.exp-sp-myprofile-org-name-fadeout-container').css('max-width',updated_with);
	}
	
$("#edit-edit-userprofile-detail").click(function () {
	$(document).bind('keydown keyup', function(e) {
    if(e.which === 116) {       
       return false;
    }
    if(e.which === 82 && e.ctrlKey) {      
       return false;
    }
});
});

	
	}catch(e){
		// to do
	}
});

function showAddmenu(){
	try{
		//alert(2);
		
		
			$('.profile-add-menu').toggle();
		}catch(e){
			// to do
		}
}
