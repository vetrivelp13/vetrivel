(function($) {
  // Extend Drupal.ajax to add command for showing cancel policy form in qtip popup
  Drupal.ajax.prototype.commands.expAdminCPolicyFormInQtip = function(ajax, response, status) {
    try {
      // Destroy loader
      $('#root-admin').data('narrowsearch').destroyLoader('commerce-cancellation-policy-screen-wrapper');
      
      // Get current theme for theme specific adjustments
      var currTheme = Drupal.settings.ajaxPageState.theme;
      //console.log(currTheme);
      
      // Remove any open qtip popup from DOM
      $(".active-qtip-div").remove();
      
      // If add another cancel policy, close add another cancel policy type list popup if add another list item was clicked
      // Also update the Add Another button label to show selected delivery type
      if (ajax.element.id.indexOf('add-another-') === 0) {
        $('.add-another-cancel-policy-type-list-ul').hide();        
        $('.add-another-cancel-policy-openup-arrow').hide();
        var newLabel = $('#' + ajax.element.id).data('label-when-qtip-open');
        $('#add-another-cancel-policy-label').text(newLabel);
      }
      // If add first cancel policy, close add cancel policy type list popup if add another list item was clicked
      // Also update the add pancel policy button label to show selected delivery type      
      else if (ajax.element.id.indexOf('add-') === 0) {

        var newVisibleItem = $('#' + ajax.element.id).parent('.add-cancel-policy-type-list-item');
        var replacedItem = $('.add-cancel-policy-type-list-ul li:eq(0)');
        var moreList = $('.add-cancel-policy-type-list-ul span.add-cancel-policy-more-list');          
        moreList.hide();
        
        // When user has clicked visible list item, nothings needs to be done. More list is already hidden above.
        // Otherwise, make the clicked more list item visible instead of the currently visible item
        if (newVisibleItem[0] !== replacedItem[0]) {
          var newVisibleItemLabel = newVisibleItem.data('link-label-as-button');
          var replacedItemLabel = replacedItem.data('link-label-as-more-item');
          var visibleItemLoc = $('.add-cancel-policy-type-list-ul span.admin-save-button-left-bg');
          replacedItem.removeClass('admin-save-button-middle-bg');
          replacedItem.children('a').eq(0).text(replacedItemLabel);
          moreList.append(replacedItem);
          newVisibleItem.children('a').eq(0).text(newVisibleItemLabel);
          newVisibleItem.addClass('admin-save-button-middle-bg');
          visibleItemLoc.after(newVisibleItem);
        }
      }
            
      // Render and display the Qtip
      
      //Initialize
      var qtipObjJSON = $('#' + ajax.element.id).data('qtip-obj');
      //console.log(qtipObjJSON);
      var qtipObj = eval('(' + qtipObjJSON + ')');
      //console.log(qtipObj);

      var popupHolderId = qtipObj.holderId;
      var popupId = qtipObj.id;
      var showForId = typeof qtipObj.showForId == 'undefined'? ajax.element.id : qtipObj.showForId;
      
      //console.log(response);

      // Add the qtip popup to DOM
      $('#' + popupHolderId).html(response.html);
      var renderedHeight = $('#' + popupId).height();
      var renderedWidth = $('#' + popupId).width();
      //console.log(renderedHeight);
      //console.log(renderedWidth);
      
      if (typeof qtipObj.placement == 'undefined') {
        qtipObj.placement = 'above-or-below'; //default placement is above-or-below
      }
      
      var triggerPos = $('#' + showForId).offset();
      var qtipPlacement = 'above'; // default placement is above the triggering element
      if (qtipObj.placement == 'above-or-below') {
        // Determine whether to place the qtip above or below the clicked element
        var availableSpaceAbove = triggerPos.top - $(window).scrollTop();
        //console.log('availableSpaceAbove = ' + availableSpaceAbove);
        if (availableSpaceAbove < 175) {
          qtipPlacement = 'below';
        }
      }
      else { // left
        var qtipPlacement = 'left';
      }
      
      // Detemine position of qtip arrow and qtip box
      //Initialize
      var qtipArrowPos = {};
      var qtipBoxPos = {};
      var triggerWidth = $('#' + showForId).width();
      var triggerHeight = $('#' + showForId).height();
      //console.log('trigger width=' + triggerWidth + ' height=' + triggerHeight);

      switch (qtipPlacement) {
        case 'above': // qtip placement is above the clicked link
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
          if (popupHolderId == 'add-another-cancel-policy') {
            qtipBoxPos.left = triggerPos.left - 15;
          }
          else { // qtipObj.popupHolderId == 'add-cancel-policy'
            var boxLeftAdjust = ((renderedWidth - triggerWidth)/2);
            qtipBoxPos.left = triggerPos.left - boxLeftAdjust;
          }

          // Calculate the qtip box top position
          qtipBoxPos.top = qtipArrowPos.top - renderedHeight + 12 ; //the box slightly overlaps the arrow
          break;
          
        case 'below': // qtip placement is below the clicked link
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
          if (popupHolderId == 'add-another-cancel-policy') {
            qtipBoxPos.left = triggerPos.left - 15;
          }
          else { // qtipObj.popupHolderId == 'add-cancel-policy'
            var boxLeftAdjust = ((renderedWidth - triggerWidth)/2);
            qtipBoxPos.left = triggerPos.left - boxLeftAdjust;
          }

          // Calculate the qtip box top position
          qtipBoxPos.top = qtipArrowPos.top + upArrowImageHeight - 24; //the box slightly overlaps the arrow
          break;
          
        case 'left': // qtip placement is below the clicked link
          // Add the right pointing arrow image (bottom-qtip-tip-up)
          $('#' + popupHolderId).append('<div id="arrow-' + popupId + '"class="bottom-qtip-right active-qtip-div qtip-arrow" style="display:none"></div>');
        
          // Calculate the qtip arrow top position
          var rightArrowImageHeight = $('#' + popupHolderId + ' .bottom-qtip-right').height();
          //console.log(rightArrowImageHeight);
          var rightArrowTopAdjust = ((rightArrowImageHeight - triggerHeight)/2);
          //console.log(rightArrowTopAdjust);
          qtipArrowPos.top = triggerPos.top - rightArrowTopAdjust;
          //console.log(qtipArrowPos);
        
          // Calculate the qtip arrow left position
          var rightArrowImageWidth = $('#' + popupHolderId + ' .bottom-qtip-right').width();
          //console.log(rightArrowImageWidth);
          qtipArrowPos.left = triggerPos.left - rightArrowImageWidth + 27; // the image needs to overlap the triggering element a bit
          // as arrow tip is not at top of image.
          if (currTheme == 'expertusone') {
            qtipArrowPos.left -= 5;
          }
          //console.log(qtipArrowPos);
          
          // Calculate the qtip box top position
          var boxTopAdjust = ((renderedHeight - triggerHeight)/2);
          qtipBoxPos.top = triggerPos.top - boxTopAdjust;
          //console.log(qtipBoxPos);
          
          // Calculate the qtip box left position
          qtipBoxPos.left = qtipArrowPos.left - renderedWidth + 18; //the box slightly overlaps the arrow
          //console.log(qtipBoxPos);
          break;
      }

      // Show the qtip to the user
      $('#arrow-' + popupId).css({'position': 'absolute', 'z-index' : '1003'})
                            .show() //jQuery offset() does not work correctly on a hidden div
                            .offset(qtipArrowPos);
      $('#' + popupId).css({'position': 'absolute', 'z-index' : '1002'})
                      .show() //jQuery offset() does not work correctly on a hidden div
                      .offset(qtipBoxPos);

      // Attach drupal JS behaviors
      Drupal.attachBehaviors();  
    }
    catch(e){
      //Nothing to do
    }
  };
  
  // Add javascript behaviors to the cancellation policy admin screen
  Drupal.behaviors.expInitCancellationPolicyAdminForm =  {
    attach: function (context, settings) {
     try{
      // Add cancel policy
      
      // Show/hide type list when add another cancel policy button more icon is clicked.
      $('#add-cancel-policy-more-button:not(.exp-cancelpolicy-initialized)').addClass('exp-cancelpolicy-initialized').each(function () {
        $(this).click(function() {
        try{              	
          $('.add-cancel-policy-more-list').toggle();
          $('.add-another-cancel-policy-openup-arrow').toggle();
          if($("#add-cancel-policy-qtip").length > 0){
        	  $( "#add-cancel-policy-qtip" ).remove();
        	  $( "#arrow-add-cancel-policy-qtip" ).remove();
        	  
          }
    	}catch(e){
    	    //Nothing to do
    	 }
        }); //end click handler
      }); //end each
      
      // Add another cancel policy
      
      // Show/hide entity type list when add another cancel policy button more icon is clicked.
      $('#add-another-cancel-policy-label:not(.exp-cancelpolicy-initialized)').addClass('exp-cancelpolicy-initialized').each(function () {
        $(this).click(function() {
        try{
          $('.add-another-cancel-policy-type-list-ul').toggle();
          $('.add-another-cancel-policy-openup-arrow').toggle();
        }catch(e){
    	    //Nothing to do
    	 }
        }); //end click handler
      }); //end each
      
      // Show/hide entity type list when add another cancel policy button is clicked.
      $('.add-another-cancel-policy-arrow:not(.exp-cancelpolicy-initialized)').addClass('exp-cancelpolicy-initialized').each(function () {
        $(this).click(function() {
        try{	
          $('.add-another-cancel-policy-type-list-ul').toggle();
          $('.add-another-cancel-policy-openup-arrow').toggle();
        }catch(e){
    	    //Nothing to do
    	 }
        }); //end click handler    
      }); //end each
     }catch(e){
       //Nothing to do
     }
    } //end attach
  }; //end Drupal.behaviors.expInitCancellationPolicyAdminForm
  
  $('body').click(function (event) {
  try{
    if(event.target.id !='add-another-cancel-policy-label' && event.target.id !='arrow-icon-cancel-policy') {
       $('.add-another-cancel-policy-type-list-ul').hide();        
       $('.add-another-cancel-policy-openup-arrow').hide();
    }
	 }catch(e){
	    //Nothing to do
	 }
  });

})(jQuery);