(function($) {
	  Drupal.ajax.prototype.commands.showReportSchedulesPopup = function(ajax, response, status) {
	     
	    try {
	    	this.currTheme = Drupal.settings.ajaxPageState.theme;
	      // Remove any open qtip popup from DOM
	      $(".active-qtip-div").remove();
	      
	      //Initialize variables
	
	      var qtipObjJSON = $('#' + ajax.element.id).data('qtip-obj');
	     

	      var qtipObj = eval('(' + qtipObjJSON + ')');
	      var popupHolderId = qtipObj.holderId;
	      
	      var popupId = qtipObj.id;
	      var renderedHeight = (typeof qtipObj.renderedHeight == 'undefined')? (this.currTheme == "expertusoneV2")? 380 : 360 : qtipObj.renderedHeight; //setting some rendered height when not set
	      // Add the qtip popup to DOM
	      $('#' + popupHolderId).html(response.html);
	      
	      
	      // Determine whether to place the qtip above or below the clicked element
	      var qtipPlacement = 'above';
	      var triggerPos = $('#' + ajax.element.id).offset();
	      var availableSpaceAbove = triggerPos.top - $(window).scrollTop();
	      //alert(availableSpaceAbove);
	      if (availableSpaceAbove < 470) {
	        qtipPlacement = 'below';
	      }
	      
	      // Detemine position of qtip arrow and qtip box
	      //Initialize
	      var qtipArrowPos = {};
	      var qtipBoxPos = {};
	      var triggerWidth = $('#' + ajax.element.id).width();
	      var triggerHeight = $('#' + ajax.element.id).height();

	      if (qtipPlacement == 'above') {
	        // Add the bottom pointing arrow image (bottom-qtip-tip)
	        $('#' + popupHolderId).append('<div id="arrow-' + popupId + '" class="bottom-qtip-tip active-qtip-div qtip-arrow" style="display:none"></div>');

	        // Calculate the qtip arrow left position
	        var downArrowImageWidth = $('#' + popupHolderId + ' .bottom-qtip-tip').width();
	        var downArrowLeftAdjust = (downArrowImageWidth - triggerWidth)/2;
	        qtipArrowPos.left = triggerPos.left - downArrowLeftAdjust;
	       
	        // Calculate the qtip arrow top position
	        var downArrowImageHeight = $('#' + popupHolderId + ' .bottom-qtip-tip').height();
	        qtipArrowPos.top = triggerPos.top - (downArrowImageHeight - 15); // the image needs to overlap the triggering element a bit
	                                                                         // as arrow tip is not at bottom of image.
	        // Calculate the qtip box left position
	        	if (typeof qtipObj.launchedFrom == 'undefined') {
		          qtipBoxPos.left = triggerPos.left - downArrowLeftAdjust - 60;
		        }
		        else if(qtipObj.launchedFrom == 'admincalendar') {
		           qtipBoxPos.left = triggerPos.left - downArrowLeftAdjust - 525;
		        } else{
		        	qtipBoxPos.left = triggerPos.left - downArrowLeftAdjust - 60;
		        }
	       // alert(qtipArrowPos.top+"\n"+ renderedHeight);
	        // Calculate the qtip box top position
	        qtipBoxPos.top = qtipArrowPos.top - renderedHeight + 12; //the box slightly overlaps the arrow
	       
	      }
	      else { // qtip placement is below the clicked link
	        // Add the up pointing arrow image (bottom-qtip-tip-up)
	        $('#' + popupHolderId).append('<div id="arrow-' + popupId + '"class="bottom-qtip-tip-up active-qtip-div qtip-arrow" style="display:none"></div>');
	        
	        // Calculate the qtip arrow left position
	        var upArrowImageWidth = $('#' + popupHolderId + ' .bottom-qtip-tip-up').width();
	        var upArrowLeftAdjust = ((upArrowImageWidth - triggerWidth)/2);
	        qtipArrowPos.left = triggerPos.left - upArrowLeftAdjust;
	        
	        // Calculate the qtip arrow top position
	        var upArrowImageHeight = $('#' + popupHolderId + ' .bottom-qtip-tip-up').height();
	        qtipArrowPos.top = triggerPos.top + triggerHeight - 5; // the image needs to overlap the triggering element a bit
	                                                               // as arrow tip is not at top of image.
	     // Calculate the qtip box left position
	        if (typeof qtipObj.launchedFrom == 'undefined') {
		          qtipBoxPos.left = triggerPos.left - upArrowLeftAdjust - 60;
		        }
		        else {
		          qtipBoxPos.left = triggerPos.left - upArrowLeftAdjust - 537;
		        }

	        // Calculate the qtip box top position
	        qtipBoxPos.top = qtipArrowPos.top + upArrowImageHeight - 24; //the box slightly overlaps the arrow
	      }

	      $('#arrow-' + popupId).css({'position': 'absolute', 'z-index' : '1003'})
	                            .show() //jQuery offset() does not work correctly on a hidden div
	                            .offset(qtipArrowPos);
	      $('#' + popupId).css({'position': 'absolute', 'z-index' : '1002'})
	                      .show() //jQuery offset() does not work correctly on a hidden div
	                      .offset(qtipBoxPos);
	      $('#' + popupId).css('max-height',renderedHeight);
	      if (this.currTheme == "expertusoneV2") {
	    	  $('#' + popupId + " .datagrid-items-list").css('height','352px');
	      } else {
	    	  $('#' + popupId + " .datagrid-items-list").css('height','306px');
	      }
	      $('#' + popupId + " .datagrid-items-list").css('overflow','hidden');
	      if (qtipPlacement == 'above') {
	    	   var x = document.getElementById('arrow-' +popupId);
		       var xTop = x.style.top;
		       //get data Grid container view/not and set to the display qtip bottom pos
		       var containerId ='#bubble-content-' + popupId + ' .noitems-container';
		        if (this.currTheme == "expertusoneV2") {
		           xTop = parseInt(xTop.substring(0,(xTop.length-2))) - (renderedHeight + 27); //the box slightly overlaps the arrow
			      }else{
		    	   if($(containerId).is(":visible")) {
				     xTop = parseInt(xTop.substring(0,(xTop.length-2))) - (renderedHeight + 32); //the box slightly overlaps the arrow
				     }else{
				     xTop = parseInt(xTop.substring(0,(xTop.length-2))) - (renderedHeight + 34);	
				     }   
		        }
		        $('#' + popupId).css('top',xTop);
	      }
	      Drupal.attachBehaviors();  
	    }
	    catch(e){
	      //Nothing to do
	    }
	  
	  };
	  
    Drupal.ajax.prototype.commands.replaceContentInReportSchedulesPopup = function(ajax, response, status) {
	    
	    try {
	    	this.currTheme = Drupal.settings.ajaxPageState.theme;
	    	var containerId ='#bubble-content-' + response.popup_id;
	    	var lastChild =$(containerId).children().last();
	    	lastChild.replaceWith(response.html);	
	    	if (this.currTheme == "expertusoneV2") {
	    	  $(containerId+ " .datagrid-items-list").css('height','352px');
	    	} else {
	    	  $(containerId+ " .datagrid-items-list").css('height','306px');
	    	}
	    	$(containerId+ " .datagrid-items-list").css('overflow','hidden');
	    	Drupal.attachBehaviors();  	
	    	vtip();	
	    }
	    catch(e){
	      //Nothing to do
	    }
	  };
	  
   Drupal.ajax.prototype.commands.initializeVtip = function(ajax, response, status) {
	    try {
	      vtip();
	    }
	    catch(e){
		      //Nothing to do
		}
		   
  };
	
})(jQuery);

