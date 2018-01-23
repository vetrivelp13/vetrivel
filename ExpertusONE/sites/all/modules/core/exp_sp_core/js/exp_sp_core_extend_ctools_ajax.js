( function($) {
  // ++++ Begin override of Drupal.CTools.Modal functions
  if (Drupal.CTools.Modal) {
    
    // Expertus : Object to hold the commands to execute on modal close.
    Drupal.CTools.Modal.closeCommand = Drupal.CTools.Modal.closeCommand || {};
    
    // Override ctools modal.js function Drupal.CTools.show()
    Drupal.CTools.Modal.show = function(choice, expModalTitle, expModalContent, fullScreen) {
    try{
      var opts = {};

      if (choice && typeof choice == 'string' && Drupal.settings[choice]) {
        // This notation guarantees we are actually copying it.
        $.extend(true, opts, Drupal.settings[choice]);
      } else if (choice) {
        $.extend(true, opts, choice);
      }

      var defaults = {
              modalTheme: 'ExpertusCToolsModalTheme' //was CToolsModalDialog
            , throbberTheme: 'CToolsModalThrobber'
            , animation: 'fadeIn' // was 'show'
            , animationSpeed: 'fast'
            , modalSize: {
                  type: 'fixed' // was 'scale'
                , width: .8
                , height: 'auto' // was .8
                , display: 'table'
                , addWidth: 20 // was 0
                , addHeight:20 // was 0
                // How much to remove from the inner content to make space for the theming.
                , contentRight :25
                , contentBottom :45
              }
            , modalOptions : {
                  opacity: .5
                , background: '#fff'
                , 'background-color': '#000' //new
                , height:'auto' //new
              }
          };

      var settings = {};
      $.extend(true, settings, defaults, Drupal.settings.CToolsModal, opts);

      if (Drupal.CTools.Modal.currentSettings && Drupal.CTools.Modal.currentSettings != settings) {
        //Drupal.CTools.Modal.modal is a jQuery object initialized below with modal dialog's content HTML
        Drupal.CTools.Modal.modal.remove();
        Drupal.CTools.Modal.modal = null;
      }

      Drupal.CTools.Modal.currentSettings = settings;

      var resize = function(e) {
        // When creating the modal, it actually exists only in a theoretical place that is not in the DOM. But once the
        // modal exists, it is in the DOM so the context must be set appropriately.
        var context = e ? document : Drupal.CTools.Modal.modal;

        if (Drupal.CTools.Modal.currentSettings.modalSize.type == 'scale') {
          var width = $(window).width() * Drupal.CTools.Modal.currentSettings.modalSize.width;
          var height = $(window).height() * Drupal.CTools.Modal.currentSettings.modalSize.height;
        } else {
          var width = Drupal.CTools.Modal.currentSettings.modalSize.width;
          var height = Drupal.CTools.Modal.currentSettings.modalSize.height;

          // Expertus: Code added to specify max-height, min-height, max-width, min-width for fixed size modals - start
          var maxHeight = 'none';
          if (Drupal.CTools.Modal.currentSettings.modalSize.maxHeight
              && Drupal.CTools.Modal.currentSettings.modalSize.maxHeight > 0
              && (Drupal.CTools.Modal.currentSettings.modalSize.maxHeight - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) > 0) {
            maxHeight = (Drupal.CTools.Modal.currentSettings.modalSize.maxHeight - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + 'px';
          }

          var minHeight = 'none';
          if (Drupal.CTools.Modal.currentSettings.modalSize.minHeight
              && Drupal.CTools.Modal.currentSettings.modalSize.minHeight > 0
              && (Drupal.CTools.Modal.currentSettings.modalSize.minHeight - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) > 0) {
            minHeight = (Drupal.CTools.Modal.currentSettings.modalSize.minHeight - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + 'px';
          }
          
          var maxWidth = 'none';
          if (Drupal.CTools.Modal.currentSettings.modalSize.maxWidth
              && Drupal.CTools.Modal.currentSettings.modalSize.maxWidth > 0
              && (Drupal.CTools.Modal.currentSettings.modalSize.maxWidth - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) > 0) {
            maxWidth = (Drupal.CTools.Modal.currentSettings.modalSize.maxWidth - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + 'px';
          }

          var minWidth = 'none';
          if (Drupal.CTools.Modal.currentSettings.modalSize.minWidth
              && Drupal.CTools.Modal.currentSettings.modalSize.minWidth > 0
              && (Drupal.CTools.Modal.currentSettings.modalSize.minWidth - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) > 0) {
            minWidth = (Drupal.CTools.Modal.currentSettings.modalSize.minWidth - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + 'px';
          }
          // Expertus: Code added to specify max-height, min-height, max-width, min-width for fixed size modals - end
        }

        //Expertus: Code modified to correctly handle auto value for height and width - start.
        var modalHeight = (height == 'auto')? 'auto' : height + Drupal.CTools.Modal.currentSettings.modalSize.addHeight + 'px';
        var modalWidth = (width == 'auto')? 'auto' : width + Drupal.CTools.Modal.currentSettings.modalSize.addWidth + 'px';
          
        // Use the additionol pixels for creating the width and height.
        $('div.ctools-modal-content', context).css( {
          'width' : modalWidth,
          'height' : modalHeight
        });

        var modalContentHeight = (height == 'auto')? 'auto' : (height - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + 'px';
        var modalContentWidth = (width == 'auto')? 'auto' : (width - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + 'px';
        
        $('div.ctools-modal-content .modal-content', context).css( {
          'width' : modalContentWidth,
          'height' : modalContentHeight,
          'max-height' : maxHeight, // Expertus: Code added to specify max-height and min-height for fixed size modals
          'min-height' : minHeight, // Expertus: Code modified to specify max-height and min-height for fixed size modals
          'max-width' : maxWidth, // Expertus: Code added to specify max-width and min-width for fixed size modals
          'min-width' : minWidth // Expertus: Code modified to specify max-width and min-width for fixed size modals
        });
      }
      //Expertus: Code modified to correctly handle auto value for height and width - end.

      if (!Drupal.CTools.Modal.modal) {
        // Expertus: Now the modal title and modal content are pre-rendered by Drupal.theme.
        //           This enables getting accurate modal height (when height is set to auto)
        //           The modal height is needed to determine the placement of modal in view window
        //           and the most appropriate modal backdrop height.
        Drupal.CTools.Modal.modal = $(Drupal.theme(settings.modalTheme, expModalTitle, expModalContent, fullScreen));
		// console.log(settings.modalTheme);
		// console.log(expModalTitle);
		// console.log(expModalContent);
		// console.log(Drupal.CTools.Modal.modal);
        if (settings.modalSize.type == 'scale') {
          $(window).bind('resize', resize);
        }
      }

      resize();

      Drupal.CTools.Modal.modalContent(Drupal.CTools.Modal.modal, settings.modalOptions, settings.animation, settings.animationSpeed);

      if (choice && typeof choice == 'string' && choice == 'ctools-video-style') {
        $('#modalContent').addClass('ctool-video-modal');
      } else if (choice && typeof choice == 'string' && choice == 'ctools-learning-request-style') {
        $('#modalContent').addClass('ctool-learning-request-modal');
        var height = parseInt($("#modalContent").css('height'));
        var availheight = parseInt(document.documentElement.clientHeight);
        $('#modalContent').css( {
          'top' :((availheight - height) / 2) + 'px'
        });
      } else if (choice && typeof choice == 'string' && choice == 'ctools-admin-addedit-style') {
        $('#modalContent').addClass('ctools-admin-addedit-modal');        
      } else if (choice && typeof choice == 'string' && choice == 'ctools-admin-webanalytics-style') {
        $('#modalContent').addClass('ctools-admin-webanalytics-modal');
      } else if (choice && typeof choice == 'string' && choice == 'ctools-admin-pwdpolicy-style') {
        $('#modalContent').addClass('ctools-admin-pwdpolicy-modal');
      } 
      else if (choice && typeof choice == 'string' && choice == 'ctools-admin-ldap-setting-addedit-scroll-wrapper') {
          $('#modalContent').addClass('ctools-admin-ldap-modal');
        } 
      else if (choice && typeof choice == 'string' && choice == 'ctools-admin-user-points-addedit-wrapper') {
          $('#modalContent').addClass('ctools-admin-user-points-modal');
        } 
      else if (choice && typeof choice == 'string' && choice == 'ctools-mobile-app-style') {
          $('#modalContent').addClass('ctools-mobile-app-popup');
      }else if (choice && typeof choice == 'string' && choice == 'ctools-admin-userreg-style') {
          $('#modalContent').addClass('ctools-admin-userreg-model');
      } else if (choice && typeof choice == 'string' && choice == 'ctools-admin-saml-style') {
          $('#modalContent').addClass('ctools-admin-saml-model');
      } else if (choice && typeof choice == 'string' && choice.indexOf('ctools-content-launch-style') !== -1) {
          $('#modalContent').addClass('ctools-content-launch-style');
		  if(choice != 'ctools-content-launch-style') {
			// add one more class to embedded widget style
			$('#modalContent').addClass('ctools-content-launch-style-widget');
		  }
      } else if (choice && typeof choice == 'string' && choice == 'ctools-admin-iprange-style') {
          $('#modalContent').addClass('ctools-admin-iprange-modal');
        } 
      else {
        $('#modalContent').addClass('ctool-login-modal');
      }
      
      if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2' && choice == 'ctools-video-style'){
    	  $('.popup-block-footer-lert, .popup-block-footer-middle, .popup-block-footer-right').css('background','none');
      }
    }catch(e){
		// to do
	}
    };

    /**
     * Override ctools modal.js function Drupal.CTools.Modal.modal_display()
     */
    Drupal.CTools.Modal.modal_display = function(ajax, response, status) {
    	try{
      // alert('exp_sp_formatter_user.js : In Drupal.CTools.Modal.modal_display()');
      if ($('#modalContent').length == 0) {
        var settings = Drupal.CTools.Modal.getSettings(ajax.element);
        // Expertus: Now the theme to render the modal is expecting the title and content
        Drupal.CTools.Modal.show(settings, response.title, response.output);
      } else {
        // Expertus: Paint the title and content in modal only when the modal already exists
        $('#modal-title').html(response.title);
        $('#modal-content').html(response.output);
       /* if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2' && settings=='ctools-video-style'){
        $('.block-footer-left, .block-footer-middle, .block-footer-right').css('background','none');
        $('#modal-content').css('width','640px');
        }*/
        Drupal.ajax.prototype.commands.CtoolsModalAdjust(); // Expertus: Adjust modal top and modal backdrop as size of modal may have changed.
      }
      Drupal.attachBehaviors();
    	}catch(e){
    		// to do
    	}
    };

    /*
     *
     */
    Drupal.CTools.Modal.getTopLeftOfWindow = function() {
    	try{
      // position code lifted from http://www.quirksmode.org/viewport/compatibility.html
      var wt = 0;      
      if (self.pageYOffset) { // all except Explorer
        wt = self.pageYOffset;
      }
      else if (document.documentElement && document.documentElement.scrollTop) { // Explorer 6 Strict
        wt = document.documentElement.scrollTop;
      }
      else if (document.body) { // all other Explorers
        wt = document.body.scrollTop;
      }

      var wl = 0;
      if (self.pageXOffset) { // all except Explorer
        wl = self.pageXOffset;
      }
      else if (document.documentElement && document.documentElement.scrollLeft) { // Explorer 6 Strict
        wl = document.documentElement.scrollLeft;
      }
      else if (document.body) { // all other Explorers
        wl = document.body.scrollLeft;
      }

      return { 'top' : wt, 'left' : wl };
    	}catch(e){
    		// to do
    	}
    }
    /**
     * Override ctools modal.js function Drupal.CTools.Modal.modalContent().
     */
    Drupal.CTools.Modal.modalContent = function(content, css, animation, speed) {
    	try{
      //alert('exp_sp_core_extend_ctools_ajax.js : In Drupal.CTools.Modal.modalContent()');
      
      // If our animation isn't set, make it just show/pop
      if (!animation) {
        animation = 'show';
      }
      else {
        // If our animation isn't "fadeIn" or "slideDown" then it always is show
        if (animation != 'fadeIn' && animation != 'slideDown') {
          animation = 'show';
        }
      }

      if (!speed) {
        speed = 'fast';
      }

      // Build our base attributes and allow them to be overriden
      css = jQuery.extend({
        position: 'absolute',
        left: '0px',
        margin: '0px',
        background: '#000',
        opacity: '.55'
      }, css);

      // Add opacity handling for IE.
      css.filter = 'alpha(opacity=' + (100 * css.opacity) + ')';
      content.hide();
 
      // if we already have a modalContent, remove it
      if ( $('#modalBackdrop')) $('#modalBackdrop').remove();
      if ( $('#modalContent')) $('#modalContent').remove();
    
      var topLeft = Drupal.CTools.Modal.getTopLeftOfWindow();
      var wt = topLeft.top;
      var wl = topLeft.left;
      //alert ('wt = ' + wt + ',\n wl = ' + wl);

      // Get our dimensions

      // Get the docHeight and (ugly hack) add 50 pixels to make sure we dont have a *visible* border below our div
      var docHeight = $(document).height(); // Expertus: Removed the ugly hack of +50 from here in original code - Sunil 
      var docWidth = $(document).width();
      //console.log('modalContent() doc height = ' + docHeight);
      //console.log('modalContent() : doc width = ' + docWidth);
      // console.log('modalContent html = ' + $(content).html());

      // Create our divs
      // Expertus: Added three hidden fields modalContentOuterHeight, modalBackdropHeight and modalTop
      //           Needed for adjusting the modal top and its backdrop when the modal size is changed.
      $('body').append('<div id="modalBackdrop" style="z-index: 1000; display: none;"></div>' +
                       '<div id="modalContent" style="z-index: 1010; position: absolute;">' +
                         $(content).html() +
                         '<input type="hidden" id="modalContentOuterHeight" value="0" />' +
                         '<input type="hidden" id="modalContentOuterWidth" value="0" />' +
                         '<input type="hidden" id="modalBackdropHeight" value="0" />' +
                         '<input type="hidden" id="modalBackdropWidth" value="0" />' +
                         '<input type="hidden" id="modalTop" value="0" />' +
                         '<input type="hidden" id="modalLeft" value="0" />' +
                         '<input type="hidden" id="docHeight" value="' + docHeight + '" />' +
                         '<input type="hidden" id="docWidth" value="' + docWidth + '" />' +
                       '</div>');

      var winHeight = $(window).height();
      var winWidth = $(window).width();
      //alert ('winHeight = ' + winHeight + ',\n winWidth = ' + winWidth);
      if( docHeight < winHeight ) docHeight = winHeight;
      if( docWidth < winWidth ) docWidth = winWidth;

      // Keyboard and focus event handler ensures focus stays on modal elements only
      modalEventHandler = function( event ) {
        target = null;
        if ( event ) { //Mozilla
          target = event.target;
        } else { //IE
          event = window.event;
          target = event.srcElement;
        }

        var parents = $(target).parents().get();
        for (var i in $(target).parents().get()) {
          var position = $(parents[i]).css('position');
          if (position == 'absolute' || position == 'fixed') {
            return true;
          }
        }
        if( $(target).filter('*:visible').parents('#modalContent').size()) {
          // allow the event only if target is a visible child node of #modalContent
          return true;
        }
        if ( $('#modalContent')) $('#modalContent').get(0).focus();
        return false;
      };
      $('body').bind( 'focus', modalEventHandler );
      $('body').bind( 'keypress', modalEventHandler );

      // Create our content div, get the dimensions, and hide it
      // Expertus: Increased hidden top from -1000px to -3000px
      var modalContent = $('#modalContent').css('top','-3000px');
      var modalContentOuterHeight = modalContent.outerHeight();
      var modalContentOuterWidth = modalContent.outerWidth();
      $('#modalContentOuterHeight').val(modalContentOuterHeight); // Expertus: Save the modal height for later resizing of modalBackdrop when modal size changes
      $('#modalContentOuterWidth').val(modalContentOuterWidth); // Expertus: Save the modal width for later resizing of modalBackdrop when modal size changes
      //alert(modalContentOuterHeight);
      // Expertus: docHeight here is the modal backdrop height and width. Adjusting that here to the full length of the modal.
      if (docHeight < wt + modalContentOuterHeight) {
        docHeight = wt + modalContentOuterHeight;
      }
      if (docWidth < wl + modalContentOuterWidth) {
        docWidth = wl + modalContentOuterWidth;
      }
      $('#modalBackdropHeight').val(docHeight);
      $('#modalBackdropWidth').val(docWidth);
      
      var mdcTop = wt + ( winHeight / 2 ) - (  modalContentOuterHeight / 2);
      var mdcLeft = wl + ( winWidth / 2 ) - (  modalContentOuterWidth / 2);
      
      // Expertus: Code added to prevent top from becoming less than wt and left from becoming less than wl - start
      if (mdcTop < wt) {
          mdcTop = wt;
      }
      if (mdcLeft < wl) {
        mdcLeft = wl;
      }
      
      //alert ('mdcTop = ' + mdcTop + ',\n mdcLeft = ' + mdcLeft);
      
      $('#modalTop').val(mdcTop); // Save the modal top  for later resizing of modalBackdrop when modal size changes
      $('#modalLeft').val(mdcLeft); // Save the modal left  for later resizing of modalBackdrop when modal size changes
      // Expertus: Code added to prevent top from becoming less than wt and left from becoming less than wl - end

      //console.log('modalContent() : backdrop height = ' + docHeight);
      //console.log('modalContent() : backdrop width = ' + docWidth);
      $('#modalBackdrop').css(css).css('top', 0).css('height', docHeight + 'px').css('width', docWidth + 'px').show();
      modalContent.css({top: mdcTop + 'px', left: mdcLeft + 'px'}).hide()[animation](speed);

      // Bind a click for closing the modalContent
      modalContentClose = function(){close(); return false;};
      $('.close').bind('click', modalContentClose);

      // Close the open modal content and backdrop
      function close() {
        //alert('exp_sp_core_extend_ctools_ajax.js : In Drupal.CTools.Modal.modalContent.close()');
        // Unbind the events
        $(window).unbind('resize',  modalContentResize);
        $('body').unbind( 'focus', modalEventHandler);
        $('body').unbind( 'keypress', modalEventHandler );
        $('.close').unbind('click', modalContentClose);
        $(document).trigger('CToolsDetachBehaviors', $('#modalContent'));

        // Set our animation parameters and use them
        if ( animation == 'fadeIn' ) animation = 'fadeOut';
        if ( animation == 'slideDown' ) animation = 'slideUp';
        if ( animation == 'show' ) animation = 'hide';

        // Close the content
        modalContent.hide()[animation](speed);
        //Fixed for #0038166 : added to get hidden value before ctool get closed.
        var emptyId = $('#empty_id').val();
		var entityType = $('#entity_value').val();
		
        // Remove the content
        $('#modalContent').remove();
        $('#modalBackdrop').remove();
        	
    
        //Expertus: Function added to run any custom jquery on ctools modal close (e.g. refresh narrow search results, destroy ckeditors if present, etc.)
        Drupal.CTools.Modal.executeCloseCommands(emptyId,entityType);

      };

      // Move and resize the modalBackdrop and modalContent on resize of the window
      modalContentResize = function(){
        
        // Expertus: Code added to keep modal top withing the view window when the window is resized.       
        var topLeft = Drupal.CTools.Modal.getTopLeftOfWindow();
        var wt = topLeft.top;
        var wl = topLeft.left;

        // Get our heights and widths
        var docHeight = $('#docHeight').val();
        var docWidth = $('#docWidth').val();

        var winHeight = $(window).height();
        var winWidth = $(window).width();

        if( docHeight < winHeight ) docHeight = winHeight;
        if( docWidth < winWidth ) docWidth = winWidth;
        
        var modalContent = $('#modalContent');

        // Expertus: Code added to adjust the backdrop height and width when modal gets bigger than the document.
        var modalContentOuterHeight = modalContent.outerHeight();
        var modalContentOuterWidth = modalContent.outerWidth();
        $('#modalContentOuterHeight').val(modalContentOuterHeight); // Save the modal height for later resizing of modalBackdrop when modal size changes
        $('#modalContentOuterWidth').val(modalContentOuterWidth); // Save the modal width for later resizing of modalBackdrop when modal size changes
        if (docHeight < wt + modalContentOuterHeight) {
          docHeight = wt + modalContentOuterHeight;
        }
        if (docWidth < wl + modalContentOuterWidth) {
          docWidth = wl + modalContentOuterWidth;
        }
        $('#modalBackdropHeight').val(docHeight);
        $('#modalBackdropWidth').val(docWidth);
  
        var mdcTop = wt + ( winHeight / 2 ) - (  modalContentOuterHeight / 2);
        var mdcLeft = wl + ( winWidth / 2 ) - (  modalContentOuterWidth / 2);
        
        // Expertus: Code added to prevent top from becoming less than wt and left from becoming less than wl - start
        if (mdcTop < wt) {
            mdcTop = wt;
        }
        if (mdcLeft < wl) {
          mdcLeft = wl;
        }
        $('#modalTop').val(mdcTop); // Save the modal top  for later resizing of modalBackdrop when modal size changes
        $('#modalLeft').val(mdcLeft); // Save the modal left  for later resizing of modalBackdrop when modal size changes
        // Expertus: Code added to prevent top from becoming less than wt and left from becoming less than wl - end        

        //console.log('modalContentResize() : recalc backdrop height = ' + docHeight);
        //console.log('modalContentResize() : recalc backdrop width = ' + docWidth);

        // Apply the changes
        $('#modalBackdrop').css('height', docHeight + 'px').css('width', docWidth + 'px').show();
        modalContent.css('top', mdcTop + 'px').css('left', mdcLeft + 'px').show();
      };
      
      $(window).bind('resize', modalContentResize);

      $('#modalContent').focus();
      $(".limit-title").trunk8(trunk8.admin_title);
    	}catch(e){
    		// to do
			// console.log(e);
			// console.log(e.stack);
			
    	}
    };
  } // end if (Drupal.CTools.Modal)

  /**
   * Hide the modal
   */
  Drupal.CTools.Modal.dismiss = function() {
	  try{
    //console.log('In exp_sp_core_extend_ctools_ajax.js : Drupal.CTools.Modal.dismiss()');
		//Fixed for #0038166 : added to get hidden value before ctool get closed.
		  var emptyId = $('#empty_id').val();
		  var entityType = $('#entity_value').val();
		    if (Drupal.CTools.Modal.modal) {
		      if (Drupal.CTools.Modal.currentSettings) {
		        var animation = Drupal.CTools.Modal.currentSettings.animation;
		        var speed = Drupal.CTools.Modal.currentSettings.animationSpeed;
		        Drupal.CTools.Modal.unmodalContent(Drupal.CTools.Modal.modal, animation, speed);
		      }
		      else {
		        Drupal.CTools.Modal.unmodalContent(Drupal.CTools.Modal.modal);       
		      }
		    }
		    
	    //Expertus: Function added to run any custom jquery on ctools modal close (e.g. refresh narrow search results, destroy ckeditors if present, etc.)
	    Drupal.CTools.Modal.executeCloseCommands(emptyId,entityType);     
	  }catch(e){
			// to do
		}
  };

  /**
   * unmodalContent
   * @param content (The jQuery object to remove)
   * @param animation (fadeOut, slideUp, show)
   * @param speed (valid animation speeds slow, medium, fast or # in ms)
   */
  Drupal.CTools.Modal.unmodalContent = function(content, animation, speed)
  {
	  try{
    //console.log('In exp_sp_core_extend_ctools_ajax.js : Drupal.CTools.Modal.unmodalContent()');
    // If our animation isn't set, make it just show/pop
    if (!animation) { var animation = 'show'; } else {
      // If our animation isn't "fade" then it always is show
      if (( animation != 'fadeOut' ) && ( animation != 'slideUp')) animation = 'show';
    }
    // Set a speed if we dont have one
    if ( !speed ) var speed = 'fast';

    // Unbind the events we bound
    $(window).unbind('resize', modalContentResize);
    $('body').unbind('focus', modalEventHandler);
    $('body').unbind('keypress', modalEventHandler);
    $('.close').unbind('click', modalContentClose);
    $(document).trigger('CToolsDetachBehaviors', $('#modalContent'));

    // jQuery magic loop through the instances and run the animations or removal.
    content.each(function(){
      if ( animation == 'fadeOut' ) {
        $('#modalContent').fadeOut(speed,function(){$('#modalBackdrop').fadeOut(speed, function(){$(this).remove();});$(this).remove();});
      } else {
        if ( animation == 'slideUp' ) {
          $('#modalContent').slideUp(speed,function(){$('#modalBackdrop').slideUp(speed, function(){$(this).remove();});$(this).remove();});
        } else {
   		  	$('#modalContent').remove();$('#modalBackdrop').remove();
        }
      }
    });
	  }catch(e){
			// to do
		}
  };

  //Expertus: Function added to run any custom jquery on ctools modal close (e.g. refresh narrow search results, destroy ckeditors if present, etc.)
  Drupal.CTools.Modal.executeCloseCommands = function(emptyId,entityType) {
	  try{
	//IMPORTANT !!! modal-qtips-close -- Do not set or use this class anywhere else -- this is meant to be for qtip issue fixes
	$('.modal-qtips-close').remove();
	// Added by Vincent on 07, Jan 2014 for #0029687: Refresh on Admin Enrollments page
	if (typeof Drupal.CTools.Modal.closeCommand.refreshNarrowSearchResults === 'undefined') {
    	var allPage= EXPERTUS_SMARTPORTAL_AbstractManager.getCookie('current_page');
    	crPage = allPage.split('~');
	    crPage[0] = crPage[0].replace('#1','#0');
	    var updatePage = crPage[0]+"~"+crPage[1]+"~"+crPage[2];
		document.cookie="current_page="+updatePage+"; path=/";
    }
    $.each(Drupal.CTools.Modal.closeCommand, function () {
      if ($.isFunction(this.cmd)) {
        this.cmd();
      }
    });
    // Reset closeCommand object (closeCommands are executed only once).
    Drupal.CTools.Modal.closeCommand = {};
    
    // Refresh calendar events
    if ($("#calendarprimaryview").size())
    	calRefreshAftClose();
    
    //Added by Vincent for #0028157 **it is not language specific issue it is a common one
    if($.browser.msie && document.getElementById('narrow-search-text-filter')!=null){
		$('#narrow-search-text-filter').focus();
	}
    
    //Fixed for #0038166 : added to delete access entry if the ctool get closed without adding any entity.
  	if(emptyId != null && emptyId != undefined  && emptyId != ''){
  		entityType = ( entityType == null || entityType == undefined  || entityType == '') ? 0 : entityType;
    	var url = "?q=administration/catalogaccess/delete/"+emptyId+"/"+entityType;
    	$.ajax({
 		   type: "POST",
 		   url: url,
 		   data:  '',
 		   success: function(respText){
 			  res = respText;
 	  		}
 		 });
  	}
	  }catch(e){
			// to do
		}
  };
  
  /**
  * Provide the HTML to create the ctools modal dialog in Expertusone
  */
  Drupal.theme.prototype.ExpertusCToolsModalTheme = function (expModalTitle, expModalContent, fullScreenEnabled) {
    //alert('Using ExpertusCToolsModalTheme');
	try{  
	var rclass = (expModalTitle == Drupal.t('Sign In'))?'popup-block-footer-right-login':'';
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(fullScreenEnabled !== undefined) {
		expModalContent = '<div id="content-to-maximize" class="iframe-wrapper">' + expModalContent + '</div>';
	}
    if(this.currTheme == "expertusoneV2"){
    var html = '';
    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div id="ctools-modal-content" class="ctools-modal-content ctools-sample-modal-content'+(fullScreenEnabled !== undefined ? ' ctools-modal-content-fullscreen-enabled' : '')+'">';
    html += '    <table cellpadding="0" cellspacing="0" class="theme-ctool-popup" id="ctools-face-table">';
    html += '      <tr>';
    html += '        <td class="popups" colspan="3" valign="top">';
    html += '          <div class="popups-container-newui"><div class="block-title-left"><div class="block-title-right">';                                      
    html += '            <div class="modal-header popups-title block-title-middle">';
    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
    html += '              <span class="popups-close"><a class="close" href="#" title= '+Drupal.t("LBL123")+' >' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '              <div class="clear-block"></div>';
	if(fullScreenEnabled !== undefined) {
		html += '<span class="popups-maximize"><a class="maximize" href="#" title = ' + Drupal.t('LBL894') + '></a></span>';
		html += '<div class="clear-block"></div>';
		html += '<span class="popups-minimize" style="display: none;"><a class="minimize" href="#" title = ' + Drupal.t('LBL893') + '></a></span>';
		html += '<div class="clear-block"></div>';
	}
    html += '            </div></div></div>';
    html += '            <div id="show_expertus_message"></div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body">' + expModalContent + '</div></div>';
    html += '        </td>';
    html += '      </tr><tr><td>';
    /*html += '      <tr><td class="block-footer-left"></td>';
    html +='       <td class="block-footer-middle"></td>';
    html +='       <td class="block-footer-right"></td>';	*/
    html += '      <div class="block-footer-left popup-block-footer-lert"></div>';
    html +='       <div class="block-footer-middle popup-block-footer-middle"></div>';
    html +='       <div class="block-footer-right popup-block-footer-right '+rclass+'"></div>';
    html += '      </td></tr>';	
    html += '    </table>';
    html += '  </div>';
    html += '</div>';
   }else{
    var html = '';
    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
    html += '    <table cellpadding="0" cellspacing="0" id="ctools-face-table">';
    html += '      <tr>';
    html += '        <td class="popups-tl popups-border"></td>';
    html += '        <td class="popups-t popups-border"></td>';
    html += '        <td class="popups-tr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-cl popups-border"></td>';
    html += '        <td class="popups-c" valign="top">';
    html += '          <div class="popups-container">';
    html += '            <div class="modal-header popups-title">';
    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
    html += '              <span class="popups-close"><a class="close" href="#" title= '+Drupal.t("LBL123")+' >' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '              <div class="clear-block"></div>';
    html += '            </div>';
    html += '            <div id="show_expertus_message"></div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body">' + expModalContent + '</div></div>';
    html += '            <div class="popups-buttons"></div>'; //Maybe someday add the option for some specific buttons.
    html += '            <div class="popups-footer"></div>'; //Maybe someday add some footer.
    html += '          </div>';
    html += '        </td>';
    html += '        <td class="popups-cr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-bl popups-border"></td>';
    html += '        <td class="popups-b popups-border"></td>';
    html += '        <td class="popups-br popups-border"></td>';
    html += '      </tr>';
    html += '    </table>'; 
    html += '  </div>';
    html += '</div>';
   } 
    return html;
	}catch(e){
		// to do
		// console.log(e);
		// console.log(e.stack);
	}
  };
  
  // Override CToolsSampleModal theme as the logic now passes modal header and modal content to the theme
  // instead of painting it later. This has helped in determining the modal size which is required to
  // calculate modal backdrop height.
  Drupal.theme.prototype.CToolsSampleModal = Drupal.theme.prototype.ExpertusCToolsModalTheme;
  
  
/**
  * Provide the HTML to create the ctools modal dialog in Expertusone
  */
  Drupal.theme.prototype.ExpertusCToolsAdminModalTheme = function (expModalTitle, expModalContent) {
    //alert('Using ExpertusCToolsModalTheme');
	try{  
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(this.currTheme == "expertusoneV2"){
    var html = '';
    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
    html += '    <table cellpadding="0" cellspacing="0" class="theme-ctool-popup" id="ctools-face-table">';
    html += '      <tr>';
    html += '        <td class="popups" colspan="3" valign="top">';
    html += '          <div class="popups-container-newui"><div class="block-title-left"><div class="block-title-right">';                                      
    html += '            <div class="modal-header popups-title block-title-middle">';
    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
    html += '              <span class="popups-close"><a class="close" href="#" title= '+Drupal.t("LBL123")+' >' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '              <div class="clear-block"></div>';
    html += '            </div></div></div>';
    html += '            <div id="show_expertus_message"></div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body">' + expModalContent + '</div></div>';
    html += '        </td>';
    html += '      </tr><tr><td>';
    /*html += '      <tr><td class="block-footer-left"></td>';
    html +='       <td class="block-footer-middle"></td>';
    html +='       <td class="block-footer-right"></td>';	*/
    html += '      <div class="block-footer-left popup-block-footer-lert"></div>';
    html +='       <div class="block-footer-middle popup-block-footer-middle"></div>';
    html +='       <div class="block-footer-right popup-block-footer-right"></div>';
    html += '      </td></tr>';	
    html += '    </table>'; 
    html += '  </div>';
    html += '</div>';
	}else{
    var html = '';
    html += '<div id="ctools-modal" class="popups-box" >';
    html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
    html += '    <table cellpadding="0" cellspacing="0" id="ctools-face-table">';
    html += '      <tr>';
    html += '        <td class="popups-tl popups-border"></td>';
    html += '        <td class="popups-t popups-border"></td>';
    html += '        <td class="popups-tr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-cl popups-border"></td>';
    html += '        <td class="popups-c" valign="top">';
    html += '          <div class="popups-container">';
    html += '            <div class="modal-header popups-title">';
    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
    html += '              <span class="popups-close"><a class="close" href="#" title='+Drupal.t("LBL123")+' onclick="removeAllQtip(); return false;">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '              <div class="clear-block"></div>';
    html += '            </div>';
    html += '            <div id="show_expertus_message"></div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body admin-popup-with-scroll"><div class="admin-popup-container">' + expModalContent + '</div></div></div>';
    html += '            <div class="popups-buttons"></div>'; //Maybe someday add the option for some specific buttons.
    html += '            <div class="popups-footer"></div>'; //Maybe someday add some footer.
    html += '          </div>';
    html += '        </td>';
    html += '        <td class="popups-cr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-bl popups-border"></td>';
    html += '        <td class="popups-b popups-border"></td>';
    html += '        <td class="popups-br popups-border"></td>';
    html += '      </tr>';
    html += '    </table>'; 
    html += '  </div>';
    html += '</div>';
    }
   return html;
	}catch(e){
		// to do
	}
  };
  
  // Override CToolsSampleModal theme as the logic now passes modal header and modal content to the theme
  // instead of painting it later. This has helped in determining the modal size which is required to
  // calculate modal backdrop height.
  Drupal.theme.prototype.CToolsSampleModal = Drupal.theme.prototype.ExpertusCToolsAdminModalTheme;
  
  // ++++ End override of Drupal.CTools.Modal functions


/**
  * Provide the HTML to create the ctools modal dialog in for Course/class widget
  */
  Drupal.theme.prototype.ExpertusCToolsAdminCourseModalTheme = function (expModalTitle, expModalContent) {
	try{
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(this.currTheme == "expertusoneV2"){
		 html += '<div id="ctools-modal" class="popups-box">';
		    html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
		    html += '    <table cellpadding="0" cellspacing="0" class="theme-ctool-popup" id="ctools-face-table">';
		    html += '      <tr>';
		    html += '        <td class="popups" colspan="3" valign="top">';
		    html += '          <div class="popups-container-newui"><div class="block-title-left"><div class="block-title-right">';                                      
		    html += '            <div class="modal-header popups-title block-title-middle">';
		    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
		    html += '              <span class="popups-close"><a class="close" href="#" title= '+Drupal.t("LBL123")+' >' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
		    html += '              <div class="clear-block"></div>';
		    html += '            </div></div></div>';
		    html += '            <div id="show_expertus_message"></div>';
		    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body">' + expModalContent + '</div></div>';
		    html += '        </td>';
		    html += '       </tr><tr><td>';
		    /*html += '      <tr><td class="block-footer-left"></td>';
		    html +='       <td class="block-footer-middle"></td>';
		    html +='       <td class="block-footer-right"></td>';	*/
		    html += '      <div class="block-footer-left popup-block-footer-lert"></div>';
		    html +='       <div class="block-footer-middle popup-block-footer-middle"></div>';
		    html +='       <div class="block-footer-right popup-block-footer-right"></div>';
		    html += '      </td></tr>';	
		    html += '    </table>'; 
		    html += '  </div>';
		    html += '</div>';
	        }else{
			var html = '';
		    html += '<div id="ctools-modal" class="popups-box" >';
		    html += '  <div class="ctools-modal-content ctools-sample-modal-content">';
		    html += '    <table cellpadding="0" cellspacing="0" id="ctools-face-table">';
		    html += '      <tr>';
		    html += '        <td class="popups-tl popups-border"></td>';
		    html += '        <td class="popups-t popups-border"></td>';
		    html += '        <td class="popups-tr popups-border"></td>';
		    html += '      </tr>';
		    html += '      <tr>';
		    html += '        <td class="popups-cl popups-border"></td>';
		    html += '        <td class="popups-c" valign="top">';
		    html += '          <div class="popups-container">';
		    html += '            <div class="modal-header popups-title">';
		    html += '              <span id="modal-title" class="modal-title">' + expModalTitle  + '</span>';
		    html += '              <span class="popups-close"><a class="close" href="#" title='+Drupal.t("LBL123")+' onclick="removeAllQtip(); return false;">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
		    html += '              <div class="clear-block"></div>';
		    html += '            </div>';
		    html += '            <div id="show_expertus_message"></div>';
		    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body admin-popup-with-scroll admin-popup-course-class-with-scroll"><div class="admin-popup-container">' + expModalContent + '</div></div></div>';
		    html += '            <div class="popups-buttons"></div>'; //Maybe someday add the option for some specific buttons.
		    html += '            <div class="popups-footer"></div>'; //Maybe someday add some footer.
		    html += '          </div>';
		    html += '        </td>';
		    html += '        <td class="popups-cr popups-border"></td>';
		    html += '      </tr>';
		    html += '      <tr>';
		    html += '        <td class="popups-bl popups-border"></td>';
		    html += '        <td class="popups-b popups-border"></td>';
		    html += '        <td class="popups-br popups-border"></td>';
		    html += '      </tr>';
		    html += '    </table>'; 
		    html += '  </div>';
		    html += '</div>';
		}
       return html;
	}catch(e){
		// to do
	}
  };
  
  // Override CToolsSampleModal theme as the logic now passes modal header and modal content to the theme
  // instead of painting it later. This has helped in determining the modal size which is required to
  // calculate modal backdrop height.
  Drupal.theme.prototype.CToolsSampleModal = Drupal.theme.prototype.ExpertusCToolsAdminCourseModalTheme;
  



  // ++++ Begin override for ajax.js methods to show custom throbber when ajax element has class
  // addedit-form-expertusone-throbber set ++++
  if (Drupal.ajax) {
  try{	  
    /**
     * Prepare the Ajax request before it is sent.
     */
    abstractDetailWidget = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
    
    /*
     * Create the request object; Microsoft failed to properly
     * implement the XMLHttpRequest in IE7 (can't request local files),
     * so we use the ActiveXObject when it is available
     * Additionally XMLHttpRequest can be disabled in IE7/IE8 so
     * we need a fallback.
     */
    if (window.ActiveXObject) {
  	  jQuery.ajaxSetup( {xhr:function(){
  	 		try{ 
  	 			return new window.ActiveXObject("Microsoft.XMLHTTP");
  	 		} catch(e){}  	 		
  	 	 	}  
  		 });
  	}

    Drupal.ajax.prototype.beforeSend = function(xmlhttprequest, options) {
      // For forms without file inputs, the jQuery Form plugin serializes the form values, and then calls jQuery's
      // $.ajax() function, which invokes this handler. In this circumstance, options.extraData is never used. For forms
      // with file inputs, the jQuery Form plugin uses the browser's normal form submission mechanism, but captures the
      // response in a hidden IFRAME. In this circumstance, it calls this handler first, and then appends hidden fields
      // to the form to submit the values in options.extraData. There is no simple way to know which submission
      // mechanism will be used, so we add to extraData regardless, and allow it to be ignored in the former case.
      if (this.form) {
        options.extraData = options.extraData || {};

        // Let the server know when the IFRAME submission mechanism is used. The server can use this information to wrap
        // the JSON response in a TEXTAREA, as per http://jquery.malsup.com/form/#file-upload.
        options.extraData.ajax_iframe_upload = '1';

        // The triggering element is about to be disabled (see below), but if it contains a value (e.g., a checkbox,
        // textfield, select, etc.), ensure that value is included in the submission. As per above, submissions that use
        // $.ajax() are already serialized prior to the element being disabled, so this is only needed for IFRAME
        // submissions.
        var v = $.fieldValue(this.element);
        if (v !== null) {
          options.extraData[this.element.name] = v;
        }
      }
    	
      // Disable the element that received the change to prevent user interface interaction while the Ajax request is in
      // progress. ajax.ajaxing prevents the element from triggering a new request, but does not prevent the user from
      // changing its value.
      $(this.element).addClass('progress-disabled').attr('disabled', true);
      
      // Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments page
      if(options.data.indexOf('Save')>0 || options.data.indexOf('order_product_creation')>0){
	      var allPage= EXPERTUS_SMARTPORTAL_AbstractManager.getCookie('current_page');
	      if(allPage!=''){
	      crPage = allPage.split('~');
	      crPage[2] = crPage[2].replace('#0','#1');
	      crPage[2] = crPage[2]=='0#1'?'1#1':crPage[2]; // fix for #0033200
	      var updatePage = crPage[0]+"~"+crPage[1]+"~"+crPage[2];
		  document.cookie="current_page="+updatePage+"; path=/";
	      }
      }
      // Insert progressbar or throbber.
      if (this.progress.type == 'bar') {
        var progressBar = new Drupal.progressBar('ajax-progress-' + this.element.id,
            eval(this.progress.update_callback), this.progress.method, eval(this.progress.error_callback));
        if (this.progress.message) {
          progressBar.setProgress(-1, this.progress.message);
        }
        if (this.progress.url) {
          progressBar.startMonitoring(this.progress.url, this.progress.interval || 1500);
        }
        this.progress.element = $(progressBar.element).addClass('ajax-progress ajax-progress-bar');
        this.progress.object = progressBar;
        $(this.element).after(this.progress.element);
      } else if (this.progress.type == 'throbber') {
        this.progress.element = $('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
        if (this.progress.message) {
          $('.throbber', this.progress.element).after('<div class="message">' + this.progress.message + '</div>');
        }

        if ($(this.element).hasClass('addedit-form-expertusone-throbber')) { // Expertus: new behaviour for addedit save button
          var wrapperId = $(this.element).data('wrapperid');
          abstractDetailWidget.createLoader(wrapperId);
        } else {
          $(this.element).after(this.progress.element); // original behaviour
        }
      }
    };

    /**
     * Handler for the form redirection completion.
     */
    Drupal.ajax.prototype.success = function(response, status) {
      // Remove the progress element.
      if (this.progress.element) {
        if ($(this.element).hasClass('addedit-form-expertusone-throbber')) { // Expertus: new behaviour for addedit save button
          var wrapperId = $(this.element).data('wrapperid');
          abstractDetailWidget.destroyLoader(wrapperId);
        } else {
          $(this.progress.element).remove(); // original behaviour
        }
      }
      if (this.progress.object) {
        this.progress.object.stopMonitoring();
      }
      $(this.element).removeClass('progress-disabled').removeAttr('disabled');

      Drupal.freezeHeight();

      for ( var i in response) {
        if (response[i]['command'] && this.commands[response[i]['command']]) {
          this.commands[response[i]['command']](this, response[i], status);
        }
      }

      // Reattach behaviors, if they were detached in beforeSerialize(). The attachBehaviors() called on the new content
      // from processing the response commands is not sufficient, because behaviors from the entire form need to be
      // reattached.
      // Expertus: this step is not needed if the complete form is replaced in insert:replaceWith AJAX command
      //           this.form is orphaned in this case
      if (this.form && this.form.parents('html').length > 0) {
        var settings = this.settings || Drupal.settings;
        Drupal.attachBehaviors(this.form, settings);
      }

      Drupal.unfreezeHeight();

      // Remove any response-specific settings so they don't get used on the next call by mistake.
      this.settings = null;
    };

    /**
     * Handler for the form redirection error.
     */
    Drupal.ajax.prototype.error = function(response, uri) {
      
        if($('#modal-content #my-login-container') != 'undefined'){
      	  var errorcontent = new Array();
      	  errorcontent[0] = Drupal.t('ERR057'); 
      	  //var messageDiv = '<div class="messages"><div class="messages error">'+errorcontent+'</div></div>';
      	  /*if($('#modal-content .messages').html() == null) {
  	    	  $('#modal-content').prepend(messageDiv);
      	  }*/ 
    			var message_call = expertus_error_message(errorcontent,'error');
  			$('#show_expertus_message').html(message_call);
  			$('#show_expertus_message').show();
      		  //$('#modal-content .messages .error').html(errorcontent);
      	  
        } else {
      	  alert(Drupal.ajaxError(response, uri));
        }
      // Remove the progress element.
      if (this.progress.element) {
        if ($(this.element).hasClass('addedit-form-expertusone-throbber')) {  // Expertus: new behaviour for addedit save button
          var wrapperId = $(this.element).data('wrapperid');
          abstractDetailWidget.destroyLoader(wrapperId);

        } else {
          $(this.progress.element).remove(); // original behaviour
        }
      }
      if (this.progress.object) {
        this.progress.object.stopMonitoring();
      }
      // Undo hide.
      $(this.wrapper).show();
      // Re-enable the element.
      $(this.element).removeClass('progress-disabled').removeAttr('disabled');
      // Reattach behaviors, if they were detached in beforeSerialize().
      if (this.form) {
        var settings = response.settings || this.settings || Drupal.settings;
        Drupal.attachBehaviors(this.form, settings);
      }
    };
  }catch(e){
		// to do
  }
  } // end if (Drupal.ajax)
  // ++++ End override for ajax.js functions to show custom throbber ++++++

  // ++++ Begin extend of Drupal.ajax.prototype.commands in ajax.js +++++++
  if (Drupal.ajax.prototype.commands) {
    /*
     * Drupal.ajax.prototype.commands.CtoolsModalAdjust() - Command to resize ctools modal backdrop when ctools modal size has increased.
     *                                                      This also adjusts ctools modal top to the top of the view area when the ctools
     *                                                      modal size stretches beyond the view area.
     */
    Drupal.ajax.prototype.commands.CtoolsModalAdjust = function(ajax, response, status) {
    	try{
      //debugger;
      //alert('In Drupal.ajax.prototype.commands.CtoolsModalAdjustBackdrop');
      if ($('#modalContent').length > 0) {
        var prevModalContentOuterHeight = $('#modalContentOuterHeight').val();
        //alert('prevModalContentOuterHeight = ' + prevModalContentOuterHeight);
        var prevModalContentOuterWidth = $('#modalContentOuterWidth').val();
        //alert('prevModalContentOuterWidth = ' + prevModalContentOuterWidth);
        var newModalContentOuterHeight = $('#modalContent').outerHeight();
        //alert('newModalContentOuterHeight = ' + newModalContentOuterHeight);
        var newModalContentOuterWidth = $('#modalContent').outerWidth();
        //alert('newModalContentOuterWidth = ' + newModalContentOuterWidth);

        $('#modalContentOuterHeight').val(newModalContentOuterHeight); // Update modal height in the hidden input field
        $('#modalContentOuterWidth').val(newModalContentOuterWidth); // Update modal width in the hidden input field
        
        var prevModalTop = $('#modalTop').val();
        //alert('prevModalTop = ' + prevModalTop);
        var prevModalLeft = $('#modalLeft').val();
        //alert('prevModalLeft = ' + prevModalLeft);
        
        var newModalTop = prevModalTop;
        var newModalLeft = prevModalLeft;
        var winHeight = $(window).height();
        var winWidth = $(window).width();
        var topLeft = Drupal.CTools.Modal.getTopLeftOfWindow();
        var wt = topLeft.top;
        var wl = topLeft.left;       
        if (newModalContentOuterHeight != prevModalContentOuterHeight) {
          if (prevModalContentOuterHeight < winHeight && newModalContentOuterHeight > winHeight && prevModalTop > wt) {   
            newModalTop = wt;
          }
          
          if (newModalContentOuterHeight < winHeight && prevModalTop >= wt) { // centre modal only when prev modal top was in view
            // We need to height wise re-center the modal in the view window
            newModalTop = wt + ( winHeight / 2 ) - (  newModalContentOuterHeight / 2);
          }

          $('#modalContent').css('top', newModalTop + 'px');
          $('#modalTop').val(newModalTop);
        }
        //alert('newModalTop = ' + newModalTop);

        if (newModalContentOuterWidth != prevModalContentOuterWidth) {         
          if (prevModalContentOuterWidth < winWidth && newModalContentOuterWidth > winWidth && prevModalLeft > wl) {   
            newModalLeft = wl;
          }
          
          if (newModalContentOuterWidth < winWidth && prevModalLeft >= wl) { // centre modal only when prev modal top was in view
            // We need to width wise re-center the modal in the view window
            newModalLeft = wl + ( winWidth / 2 ) - (  newModalContentOuterWidth / 2);
          }
          
          $('#modalContent').css('left', newModalLeft + 'px');
          $('#modalLeft').val(newModalLeft);
        }
        //alert('newModalLeft = ' + newModalLeft);
        
        var docHeight = $("#docHeight").val();
        var docWidth = $("#docWidth").val();
        //console.log('CtoolsModalAdjust() : saved doc height = ' + docHeight);
        //console.log('CtoolsModalAdjust() : saved doc width = ' + docWidth);
        
        if( docHeight < winHeight ) docHeight = winHeight;
        if( docWidth < winWidth ) docWidth = winWidth;
        
        var lastSavedModalBackdropHeight = $('#modalBackdropHeight').val();
        //alert('lastSavedModalBackdropHeight = ' + lastSavedModalBackdropHeight);
        var lastSavedModalBackdropWidth = $('#modalBackdropWidth').val();
        //alert('lastSavedModalBackdropWidth = ' + lastSavedModalBackdropWidth);
        
        var modalBackdropHeight = parseInt($("#modalBackdrop").css('height'));
        //alert('current modalBackdropHeight = ' + modalBackdropHeight);
        var modalBackdropWidth = parseInt($("#modalBackdrop").css('width'));
        //alert('current modalBackdropWidth = ' + modalBackdropWidth);
        
        var newModalBackdropHeight = parseInt(newModalTop) + parseInt(newModalContentOuterHeight);
        //alert('newModalBackdropHeight = ' + newModalBackdropHeight);
        var newModalBackdropWidth = parseInt(newModalLeft) + parseInt(newModalContentOuterWidth);
        //alert('newModalBackdropWidth = ' + newModalBackdropWidth);
               
        if (newModalBackdropHeight < docHeight) {
          newModalBackdropHeight = docHeight;
        }

        if (newModalBackdropWidth < docWidth) {
          newModalBackdropWidth = docWidth;
        }
        //console.log('CtoolsModalAdjust() : recalc backdrop height = ' + newModalBackdropHeight);
        //console.log('CtoolsModalAdjust() : recalc backdrop width = ' + newModalBackdropWidth);

        // Increase the backdrop height and/or width if needed
        if (newModalBackdropHeight != modalBackdropHeight) {
            $('#modalBackdrop').css('height', newModalBackdropHeight + 'px');
            $('#modalBackdropHeight').val(newModalBackdropHeight); // Update modal backdrop height in the hidden input field
            //console.log('CtoolsModalAdjust() : New modalBackdrop height is set to ' + newModalBackdropHeight);
        }
        if (newModalBackdropWidth != modalBackdropWidth) {
            $('#modalBackdrop').css('width', newModalBackdropWidth + 'px');
            $('#modalBackdropWidth').val(newModalBackdropWidth); // Update modal backdrop width in the hidden input field
            //console.log('CtoolsModalAdjust() : New modalBackdrop width is set to ' + newModalBackdropWidth);
        }    	
      }
    	}catch(e){
    		// to do
    	}
    };

    // MRO - role based access related coding added
    Drupal.ajax.prototype.commands.refreshvalueresults = function(ajax, response, status) {
    	try{
        // Refresh the narrow search results only when addedit form is shown on same page as search results via ctools
        // modal
      $('.fldroles_sel').selectAll();
        $('.fldroles_sel').moveSelectionTo($('.fldroles_unsel'));
        var xx = $('.fldroles_unsel').attr('options');
        $(response.entity_id).each(function(){
          for(var i=0;i<xx.length;i++){
            if(xx[i].value==this){
              xx[i].selected=true;
            }
          }
        });
        $('.fldroles_unsel').moveSelectionTo($('.fldroles_sel'));
    	}catch(e){
    		// to do
    	}
    };
  } // end if (Drupal.ajax.prototype.commands)
  // ++++ End extend of Drupal.ajax.prototype.commands in ajax.js +++++++

})(jQuery);
function launchFullscreen(element) {
  return false;
  if(element.requestFullscreen) {
    element.requestFullscreen();
  }/*  else if(element.mozRequestFullScreen) {
    element.mozRequestFullScreen();
  }  */else if(element.webkitRequestFullscreen) {
    element.webkitRequestFullscreen();
  } else if(element.msRequestFullscreen) {
    element.msRequestFullscreen();
  } else {
	return false;
  }
  return true;
}
function exitFullscreen() {
  if(document.exitFullscreen) {
    document.exitFullscreen();
  }/*  else if(document.mozCancelFullScreen) {
    document.mozCancelFullScreen();
  } */ else if(document.webkitExitFullscreen) {
    document.webkitExitFullscreen();
  } else if(document.msExitFullscreen) {
    document.msExitFullscreen();
  } else {
	return false;
  }
  return true;
 } 