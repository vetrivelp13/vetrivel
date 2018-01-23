H5P.InteractiveVideoInteraction = (function ($, EventDispatcher) {

  /**
   * Keeps control of interactions in the interactive video.
   *
   * @class H5P.InteractiveVideoInteraction
   * @extends H5P.EventDispatcher
   * @param {Object} parameters describes action behavior
   * @param {H5P.InteractiveVideo} player instance
   * @param {Object} previousState
   */
  function Interaction(parameters, player, previousState) {
    var self = this;

    // Initialize event inheritance
    EventDispatcher.call(self);

    var $interaction, $label, $inner, $outer;
    var action = parameters.action;
    if (previousState) {
      action.userDatas = {
        state: previousState
      };
    }

	  //if from and to not converted already as pure numbers
	  if(parameters.duration.from != undefined)
	  {
      	if(new String(parameters.duration.from).indexOf(":")>0)
      	{
      		parameters.duration.from = parseInt(convertToSeconds(parameters.duration.from),10);
			parameters.duration.to   = parseInt(convertToSeconds(parameters.duration.to),10);
	  	}
	  }	
	  
    // Find library name and title
    var library = action.library.split(' ')[0];
    var title = (action.params.contentName !== undefined ? action.params.contentName : player.l10n.interaction);

    // Detect custom html class for interaction.
    var classes = parameters.className;
    if (classes === undefined) {
      var classParts = action.library.split(' ')[0].toLowerCase().split('.');
      classes = classParts[0] + '-' + classParts[1] + '-interaction';
    }

    // Keep track of content instance
    var instance;

    // Keep track of DragNBarElement and related dialog/form
    var dnbElement;

    // Keep track of interaction element state
    var isShownAsButton = false;

    // Keep track of tooltip state
    var isHovered = false;

    // Changes if interaction has moved from original position
    var isRepositioned = false;

    /**
     * Display the current interaction as a button on top of the video.
     *
     * @private
     */
    var createButton = function () {
      var hiddenClass = isShownAsButton ? '' : ' h5p-hidden';
      $interaction = $('<div/>', {
        tabIndex: 0,
        role: 'button',
        'class': 'h5p-interaction ' + classes + hiddenClass,
        css: {
          left: parameters.x + '%',
          top: parameters.y + '%',
          width: '',
          height: ''
        },
        on: {
          click: function () {
            if (!self.dialogDisabled && library !== 'H5P.Nil') {
              openDialog();
            }
          },
          keypress: function (event) {
            if ((event.charCode || event.keyCode) === 32) { // Space
              if (!self.dialogDisabled && library !== 'H5P.Nil') {
                openDialog();
              }
            }
          }
        }
      });

      // Touch area for button
      $('<div/>', {
        'class': 'h5p-touch-area'
      }).appendTo($interaction);

      $('<div/>', {
        'class': 'h5p-interaction-button',
        'aria-label': title
      }).appendTo($interaction);

      // Show label in editor on hover
      if (player.editor) {
        $interaction.hover(function () {
          if ((!$interaction.is('.focused') && !$interaction.is(':focus')) &&
              (!player.dnb || (player.dnb && !player.dnb.newElement))) {
            player.editor.showInteractionTitle(title, $interaction);
            isHovered = true;
          } else {

            // Hide if interaction is focused, because of coordinates picker
            player.editor.hideInteractionTitle();
            isHovered = false;
          }
        }, function () {

          // Hide on hover out
          player.editor.hideInteractionTitle();
          isHovered = false;
        }).focus(function () {

          // Hide on focus, because of coord picker
          player.editor.hideInteractionTitle();
          isHovered = false;
        });
      }

      // Check to see if we should add label
      if (library === 'H5P.Nil' || (parameters.label && $converter.html(parameters.label).text().length)) {
      
      	//h5pcustomize
      	var lengthytitle = '';
      	if($(".h5p-interactivevideo-editor").size() > 0)
      	{
			if(parameters.label.length > 30)
				lengthytitle = parameters.label; //.substring(0,20)+"...";//'<div class="fade-out-title-container  video-file-title-fadeout-container"><span class="title-lengthy-text">'+parameters.label.substring(0,12)+'</span><span class="fade-out-image"></span></div>';
			else
				lengthytitle = parameters.label;
      	}
      	else
      		lengthytitle = parameters.label;
      		
        $label = $('<div/>', {
          'class': 'h5p-interaction-label',
          html: '<div class="h5p-interaction-label-text">' + lengthytitle + '</div>'
        }).appendTo($interaction); 
        
       
          /*$label = $('<div/>', {
          'class': 'h5p-interaction-label',
          html: '<div class="h5p-interaction-label-text">' + lengthytitle + '</div>'
        }).appendTo($interaction); */ 
        
      }
      
		
      self.trigger('display', $interaction);
      setTimeout(function () {
        if ($interaction) {
          // Transition in
          $interaction.removeClass('h5p-hidden');
        }
      }, 0);
    };

    /**
     * Opens button dialog.
     *
     * @private
     */
    var openDialog = function () {
      if (typeof instance.setActivityStarted === 'function' && typeof instance.getScore === 'function') {
        instance.setActivityStarted();
      }

      // Create wrapper for dialog content
      var $dialogContent = $('<div/>', {
        'class': 'h5p-dialog-interaction h5p-frame',
        css: {                           //h5pcustomization
           overflow: 'visible',
          }
      });

      // Attach instance to dialog and open
      instance.attach($dialogContent);

      // Open dialog
      player.dnb.dialog.open($dialogContent);
      player.dnb.dialog.addLibraryClass(library);
    
      /**
       * Handle dialog closing once.
       * @private
       */
      var dialogCloseHandler = function () {
        this.off('close', dialogCloseHandler); // Avoid running more than once

        // Try to pause any media when closing dialog
        try {
          if (instance.pause !== undefined &&
            (instance.pause instanceof Function ||
            typeof instance.pause === 'function')) {
            instance.pause();
          }
        }
        catch (err) {
          // Prevent crashing, log error.
          H5P.error(err);
        }
      };
      player.dnb.dialog.on('close', dialogCloseHandler);

      /**
       * Set dialog width of interaction and unregister dialog close listener
       */
      var setDialogWidth = function () {
        self.dialogWidth = player.dnb.dialog.getDialogWidth();
        player.dnb.dialog.off('close', setDialogWidth);
      };

      // Keep dialog width when dialog closes.
      player.dnb.dialog.on('close', setDialogWidth);
      if (library === 'H5P.Image') {
        // Special case for fitting images
        var max = player.dnb.dialog.getMaxSize($interaction);
		//h5pcustomize
		if(action.params.file != null)
		{
			if(action.params.file.path.split("^^^") >0)
				action.params.file.path =action.params.file.path.split("^^^")[1];
			
		}
        var $img = $dialogContent.find('img');
        if (action.params.file.width && action.params.file.height) {
          // Use the image size info that is stored
          resizeImage($img, max, {
            width: action.params.file.width,
            height: action.params.file.height
          }, !player.isMobileView);
        }
        else {
          // Wait for image to load
          $img.load(function () {
            if ($img.is(':visible')) {
              resizeImage($img, max, {
                width: this.width,
                height: this.height
              }, !player.isMobileView);
            }
          });
          player.dnb.dialog.position($interaction);
        }
      }
      else {
        // Position dialog. Use medium dialog for all interactive dialogs.
        if (!player.isMobileView) {
          // Set size of dialog
          player.dnb.dialog.position($interaction, {width: self.dialogWidth / 16}, !(library === 'H5P.Text' || library === 'H5P.Table'));
        }
      }

      if (library === 'H5P.Summary') {
        // Scroll summary to bottom if the task changes size
        var lastHeight = 0;
        H5P.on(instance, 'resize', function () {
          var height = $dialogContent.height();
          if (lastHeight > height + 10 || lastHeight < height - 10)  {
            setTimeout(function () {
              player.dnb.dialog.scroll(height, 300);
            }, 500);
          }
          lastHeight = height;
        });
      }
		
		
      setTimeout(function () {
        H5P.trigger(instance, 'resize');
      }, 0);
    };

    /**
     * Resize the image so that it fits the available dialog space.
     *
     * @private
     * @param {H5P.jQuery} $img
     * @param {Object} max width,height in em
     * @param {Object} size width,height in px
     * @param {Boolean} positionDialog position dialog if true
     */
    var resizeImage = function ($img, max, size, positionDialog) {
    
      var fontSize = 16;
      size.width /= fontSize;
      size.height /= fontSize;

      if (size.height > max.height) {
        size.width = size.width * max.height / size.height;
        size.height = max.height;
      }
      if (size.width > max.width) {
        size.height = size.height * max.width / size.width;
        size.width = max.width;
      }

      var fontSizeRatio = 16 / Number($img.css('fontSize').replace('px',''));
      $img.css({
        width: (size.width * fontSizeRatio) + 'em',
        height: (size.height * fontSizeRatio) + 'em'
      });

      if (positionDialog) {
        // Set dialog size and position
        player.dnb.dialog.position($interaction, size);
      }
      
      
      
    };

    /**
     * Display the current interaction as a poster on top of the video.
     *
     * @private
     */
    var createPoster = function () {
     console.log("widthPX:"+JSON.stringify(parameters)); 
  //  if(parameters.widthPX == undefined && player.editor != undefined)
    if( player.editor != undefined)
    {
      $interaction = $('<div/>', {
        'class': 'h5p-interaction h5p-poster ' + classes,
        css: {
          left: parameters.x + '%',
          top: parameters.y + '%',
          width: (parameters.width ? parameters.width : 10) + 'em',
          height: (parameters.height ? parameters.height : 10) + 'em'
        }
      });
	
	 }
	 else if (parameters.widthPX != undefined )
	 {
	   $interaction = $('<div/>', {
        'class': 'h5p-interaction h5p-poster ' + classes,
        css: {
          left: parameters.x + '%',
          top: parameters.y + '%',
          width: (parameters.width ? parameters.width : 10) + 'em',
          height: (parameters.heightPX ? parameters.heightPX : 10) + 'px'
        }
      });
	
    }
	
	
	
      // Reset link interaction dimensions
      if (library === 'H5P.Link') {
        $interaction.css('height', 'auto');
        $interaction.css('width', 'auto');

        // Set link functionality on whole button
        if (player.editor === undefined) {
          $interaction.click(function () {
            window.open(instance.getUrl());
            player.pause();
            return false;
          });
        }
      }

      $outer = $('<div>', {
        'class': 'h5p-interaction-outer'
      }).appendTo($interaction);

      $inner = $('<div/>', {
        'class': 'h5p-interaction-inner h5p-frame'
      }).appendTo($outer);

      if (player.editor !== undefined && instance.disableAutoPlay) {
        instance.disableAutoPlay();
      }

      instance.attach($inner);

	   
      // Trigger event listeners
      self.trigger('display', $interaction);

      setTimeout(function () {
        H5P.trigger(instance, 'resize');
         
         
      }, 0);

	  
      // Register that this interaction has started if it is a question
      if (typeof instance.setActivityStarted === 'function' && typeof instance.getScore === 'function') {
        instance.setActivityStarted();
      }
	
	  //isRepositioned = false;
		
    if(player.editor === undefined)
    {
      var containerWidth =parseInt($(".h5p-interactive-video").css("width"),10);
      var containerHeight =parseInt($(".h5p-interactive-video").css("height"),10);
      var containerHeightOnlyVideoSpaceWithoutSeekbar =  parseInt(containerHeight,10) - 36; //36 seekar height
	  var interactionLeft = parseInt($interaction.css("left"),10);
	  var interactionTop = parseInt($interaction.css("top"),10);
	  var interactionHeight = parseInt($interaction.css("height"),10);
	  var interactionWidth =  parseInt($interaction.css("width"),10);
      
      if(parameters.widthPX == undefined)
         {
         	parameters.widthPX = interactionWidth+"px";
         	parameters.heightPX = interactionHeight+"px";
         }
      //Customize h5p poster to avoid bottom hide issue
      var interactionArea = interactionHeight + interactionTop;
	  if (interactionArea > containerHeightOnlyVideoSpaceWithoutSeekbar) {
	  	$interaction.css("top",(interactionTop- (interactionArea - containerHeightOnlyVideoSpaceWithoutSeekbar)));
		isRepositioned = false;
	  }
	  
	  //avoid left hidden issue
	  
	  if(interactionLeft <= 0)
	  {
	  	$interaction.css("left","10px");
	  	isRepositioned = false;
	  }
	  
	  //avoid right hidden issue
	  var interactionArea = interactionWidth + interactionLeft;
	  console.log("interactionWidth:"+interactionWidth+"::interactionLeft:"+interactionLeft+":interactionTop:"+interactionTop+":interactionHeight:"+interactionHeight+":containerWidth::"+containerWidth+":containerHeight:"+containerHeight);
	  if(interactionArea > containerWidth){
	  	var newLeft = interactionLeft - (interactionArea - containerWidth); //10 for padding
	  	$interaction.css("left",(newLeft - 10)); //minus 10 for padding
		isRepositioned = false;
	  }
	  
	  //avoid top hidden issue
	    if(interactionTop <= 0)
	    {
	  	$interaction.css("top","10px");
	  	isRepositioned = false;
	    }
	    
		//To avoid additional spaces in poster for mark the words,drag text
		$interaction.css('height', 'auto');
		if(library == "H5P.Image")
		{
		 	if(parameters.widthPX != undefined)
			{
				$interaction.css('height', parameters.heightPX);
				$interaction.css('width', parameters.widthPX);
				
			}
		}
		else if(parameters.heightPX != undefined)
		{
			if(library == "H5P.Link"){
				$interaction.css('height', "auto");
			}else if(parameters.height == 10 && library != "H5P.Link")
				$interaction.css('height', '10em');
			else
			$interaction.css('height', parameters.heightPX);
		//	$interaction.css('width', parameters.widthPX);
		}

    }
		
    };

    /**
     * Adds adaptivity or continue button to exercies.
     *
     * @private
     * @param {H5P.jQuery} $target
     */
    var adaptivity = function ($target) {

      var adaptivity, fullScore;
      if (parameters.adaptivity) {
        fullScore = self.score >= self.maxScore;

        // Determine adaptivity
        adaptivity = (fullScore ? parameters.adaptivity.correct : parameters.adaptivity.wrong);
      }

      if (!adaptivity || adaptivity.seekTo === undefined) {
        // Add continue button if no adaptivity
        if (instance.hasButton !== undefined) {
          if (!instance.hasButton('iv-continue')) {
            // Register continue button
            instance.addButton('iv-continue', player.l10n.defaultAdaptivitySeekLabel, function () {
              if (self.isButton()) {
                // Close dialog
                player.dnb.dialog.close();
              }
              else {
                if (player.isMobileView) {
                  player.dnb.dialog.close();
                }
                // Remove interaction posters
                $interaction.detach();
              }

              // Do not play if player is at the end, state 0 = ENDED
              if (player.currentState !== 0) {
                player.play();
              }
            });
          }
          else {
            instance.showButton('iv-continue');
          }
        }

        return;
      }

      // Stop playback
      player.pause();

      if (!adaptivity.allowOptOut && $interaction) {
        // Make sure only the interaction is useable.
        if (self.isButton()) {
          player.dnb.dialog.disableOverlay = true;
          player.dnb.dialog.hideCloseButton();
        }
        else {
          $interaction.css('zIndex', 52);
          player.$videoWrapper.addClass('h5p-disable-opt-out');
          player.dnb.dialog.openOverlay();
        }
      }

      var adaptivityId = (fullScore ? 'correct' : 'wrong');
      var adaptivityLabel = adaptivity.seekLabel ? adaptivity.seekLabel : player.l10n.defaultAdaptivitySeekLabel;

      // add and show adaptivity button, hide continue button
      instance.hideButton('iv-continue')
        .addButton('iv-adaptivity-' + adaptivityId, adaptivityLabel, function () {
          if (self.isButton() || player.isMobileView) {
            player.dnb.dialog.close();
          }
          if (!adaptivity.allowOptOut) {
            if (!self.isButton()) {
              player.dnb.dialog.closeOverlay();
              $interaction.css('zIndex', '');
              player.$videoWrapper.removeClass('h5p-disable-opt-out');
            }
          }

          // Reset interaction
          if (!fullScore && instance.resetTask) {
            instance.resetTask();
            instance.hideButton('iv-adaptivity-' + adaptivityId);
          }

          // Remove interaction
          self.remove();
          player.pause();
          player.seek(adaptivity.seekTo);
          player.play();
        }
      ).showButton('iv-adaptivity-' + adaptivityId, 1)
        .hideButton('check-answer', 1)
        .hideButton('show-solution', 1)
        .hideButton('try-again', 1);

      // Disable any input
      if (instance.disableInput !== undefined &&
          (instance.disableInput instanceof Function ||
           typeof instance.disableInput === 'function')) {
        instance.disableInput();
      }

      // Wait for any modifications Question does to feedback and buttons
      setTimeout(function () {
        // Set adaptivity message and hide interaction flow controls, strip adaptivity message of p tags
        instance.updateFeedbackContent(adaptivity.message.replace('<p>', '').replace('</p>', ''), true);
      }, 0);
    };

    /**
     * Extract the current state of interactivity for serialization.
     *
     * @returns {Object}
     */
    self.getCurrentState = function () {
      if (instance && (instance.getCurrentState instanceof Function ||
                       typeof instance.getCurrentState === 'function')) {
        return instance.getCurrentState();
      }
    };

    /**
     * Checks to see if the interaction should pause the video.
     *
     * @returns {boolean}
     */
    self.pause = function () {
      return parameters.pause;
    };

    /**
     * Check to see if interaction should be displayed as button.
     *
     * @returns {boolean}
     */
    self.isButton = function () {
      return parameters.displayType === 'button' || library === 'H5P.Nil' || player.isMobileView;
    };

    /**
     * Checks if this is the end summary.
     *
     * @returns {boolean}
     */
    self.isMainSummary = function() {
      return parameters.mainSummary === true;
    };

    /**
     * Create dot for displaying above the video timeline.
     * Append to given container.
     *
     * @param {H5P.jQuery} $container
     */
    self.addDot = function ($container) {
    
     //h5pcustomize - commented to display slider for label
     /* if (library === 'H5P.Nil') {
        return; // Skip "sub titles"
      }*/

      // One could also set width using ((parameters.duration.to - parameters.duration.from + 1) * player.oneSecondInPercentage)
      $('<div/>', {
        'class': 'h5p-seekbar-interaction ' + classes,
        title: title,
        css: {
          left: (parameters.duration.from * player.oneSecondInPercentage) + '%'
        }
      }).appendTo($container);
    };

    /**
     * Display or remove the interaction depending on the video time.
     *
     * @param {number} second video time
     * @returns {H5P.jQuery} interaction button or container
     */
    self.toggle = function (second) {
      second = Math.floor(second);
      if (second < parameters.duration.from || second > parameters.duration.to) {
        if ($interaction) {
          // Remove interaction from display
          if (dnbElement) {
            dnbElement.hideContextMenu();
            if (dnbElement === player.dnb.focusedElement) {
              dnbElement.blur();
              delete player.dnb.focusedElement;
            }
          }
          if (player.editor && isHovered) {
            player.editor.hideInteractionTitle();
            isHovered = false;
          }
          self.remove();
        }
        return;
      }

      if ($interaction) {
        return; // Interaction already on display
      }

      if (self.isButton() || player.isMobileView) {
        createButton();
        isShownAsButton = true;
      }
      else {
      
        createPoster();
        isShownAsButton = false;
      }
      if (player.editor === undefined) {
        dnbElement = player.dnb.add($interaction, undefined, {dnbElement: dnbElement, disableContextMenu: true});
      }
      else {

        if (self.fit) {
          // Make sure player is inside video container
          player.editor.fit($interaction, parameters);
          self.fit = false;
        }

        // Pause video when interaction is focused
        $interaction.focus(function () {
          player.pause();
        });
      }

      return $interaction;
    };

    self.setTitle = function (customTitle) {
      if ($interaction) {
        $interaction.attr('aria-label', customTitle);
      }
      title = customTitle;
    };

    /**
     * Recreate interactions. Useful when an interaction or view has changed.
     */
    self.reCreateInteraction = function () {
      // Only recreate existing interactions
      if ($interaction) {
        $interaction.detach();

        if (self.isButton() || player.isMobileView) {
          createButton();
          isShownAsButton = true;
        } else {
          createPoster();
          isShownAsButton = false;
        }
      }
    };

    self.resizeInteraction = function () {
      if (library !== 'H5P.Nil') {
        H5P.trigger(instance, 'resize');
      }
    };

    /**
     * Position label to the left or right of the action button.
     *
     * @param {number} width Size of the container
     */
    self.positionLabel = function (width) {
      //h5pcustomize
      if(library === 'H5P.Nil' && $label != undefined && $label.text() != "")
      {
			$label.attr("title",$label.text());
			if($(".h5p-interactivevideo-editor").size() > 0) //if editor show icon
				ns.$(".h5p-interaction-button").css("display","block");
			
    /*  //	if($(".h5p-interactivevideo-editor").size() > 0)
      	{
      		
      		//if label is big
      		try
      		{
			//console.log("positionLabel::::new	....:"+library+"====$interaction.css('left'):"+$interaction.css('left')+"label width:"+$label.outerWidth()+"::width:"+width);
      		//if bigger label
      		if(parseInt($interaction.css('left'),10)+$label.outerWidth() > width)
      		{
      			var excessLabelWidth = parseInt($interaction.css('left'),10)+$label.outerWidth() - width;
      			$label.css("width",$label.outerWidth() -excessLabelWidth);
      			if($label.find(".fade-desc").size() == 0){
      				$label.attr("title",$label.text());
      				//$label.html($label.html()+"<span class='fade-desc'></span>");
      			}
      			console.log($label.html());
      		}
      		else
      		{
      			var increaseLabelWidth = width -  parseInt($interaction.css('left'),10)+$label.outerWidth();
      			
			//	$label.css("width",$label.outerWidth() +increaseLabelWidth);
				if($label.find(".fade-desc").size() == 0){
					$label.attr("title",$label.text());	
      				//$label.html($label.html()+"<span class='fade-desc'></span>");
      			} 
      			
      		}
      		}catch(e){}
      		
      	}*/
     
      	
      }
      if (!$interaction || !self.isButton() || !$label || library === 'H5P.Nil') {
        return;
      }

      $label.removeClass('h5p-left-label');
      if (parseInt($interaction.css('left')) + $label.position().left + $label.outerWidth() > width) {
        $label.addClass('h5p-left-label');
      }
    };

    /**
     * Update element position.
     *
     * @param {number} x left
     * @param {number} y top
     */
    self.setPosition = function (x, y) {
      parameters.x = x;
      parameters.y = y;
      $interaction.css({
        'left': x + '%',
        'top': y + '%'
      });
    };


	//h5pcustomize
	/**
     * Update element position in pixel.
     *
     * @param {number} x left
     * @param {number} y top
     */
    self.setPositionPX = function (x, y) 
    {
      try
      {
      var editorWidth = parseFloat(H5P.jQuery(".h5p-video-wrapper").width());
      var editorHeight= parseFloat(H5P.jQuery(".h5p-video-wrapper").height());
      parameters.xPX = (x/100)*editorWidth;
      parameters.yPX = (y/100)*editorHeight;
      //alert($interaction.css("width"));
      parameters.widthPX = $interaction.css("width");
      parameters.heightPX = $interaction.css("height");
      var bottomSpaceInPx = editorHeight - (parseFloat(parameters.heightPX)+parseFloat($interaction.css("top")));
      parameters.bottomSpaceInPx = bottomSpaceInPx;
      
      var left = parseFloat($interaction.css("left"));
      var availableSpaceInRight = editorWidth - (left + parseFloat($interaction.css("width")));
      parameters.availableSpaceInRight = availableSpaceInRight;
      
      }catch(e){}
    };


    /**
     * Update element size. This function is needed by the IV editor
     *
     * @param {number} width in ems
     * @param {number} height in ems
     */
    self.setSize = function (width, height) {
      if (width) {
        parameters.width = width;
      }
      if (height) {
        parameters.height = height;
      }

      H5P.trigger(instance, 'resize');
      parameters.widthPX = $interaction.css("width");
      parameters.heightPX = $interaction.css("height");
    };

    /**
     * Removes interaction from display.
     *
     * @param {boolean} [updateSize] Used when re-creating element
     */
    self.remove = function (updateSize) {
      if ($interaction) {
        // Let others reach to the hiding of this interaction
        self.trigger('domHidden', {
          '$dom': $interaction,
          'key': 'videoProgressedPast'
        }, {'bubbles': true, 'external': true});
        $interaction.detach();
        $interaction = undefined;
      }
    };

    /**
     * Create a new instance of the interaction.
     * Useful if the input parameters have changes.
     */
    self.reCreate = function () {
      if (library !== 'H5P.Nil') {
        instance = H5P.newRunnable(action, player.contentId, undefined, undefined, {parent: player});

        // Set adaptivity if question is finished on attach
        if (instance.on) {

          // Handle question/task finished
          instance.on('xAPI', function (event) {
            if ((event.getMaxScore() && event.getScore() !== null) &&
                event.getVerb() === 'completed' ||
                event.getVerb() === 'answered') {
              self.score = event.getScore();
              self.maxScore = event.getMaxScore();
              adaptivity();
            }
            self.trigger(event);
          });
          instance.on('question-finished', function () {
            adaptivity();
          });

          instance.on('resize', function () {
            // Forget the static dialog width on resize
            delete self.dialogWidth;
            if (player && player.dnb) {
              player.dnb.dialog.removeStaticWidth();
            }
          });

          if (library === 'H5P.GoToQuestion') {
            instance.on('chosen', function (event) {
              if (self.isButton()) {
                // Close dialog
                player.dnb.dialog.close();
              }
              if (player.currentState === H5P.Video.PAUSED ||
                  player.currentState === H5P.Video.ENDED) {
                // Start playing again
                player.play();
              }

              // Jump to chosen timecode
              player.seek(event.data);
            });
          }
        }
      }
    };

    /**
     * Set dnb element for interaction, connecting it to a dialog/form
     *
     * @param {H5P.DragNBarElement} newDnbElement
     * @returns {Boolean} True if a new DragNBarElement was set.
     */
    self.setDnbElement = function (newDnbElement) {
      if (dnbElement === newDnbElement) {
        return false;
      }
      dnbElement = newDnbElement;
      return true;
    };

    /**
     * Gets the name of the library used in the interaction.
     *
     * @returns {string}
     */
    self.getLibraryName = function () {
      return library;
    };

    /**
     * Returns the human readable label for the interaction.
     *
     * @returns {string}
     */
    self.getTitle = function () {
      return title;
    };

    /**
     * Get HTML class name
     *
     * @returns {string}
     */
    self.getClass = function () {
      return classes;
    };

    /**
     * Collect copyright information for the interaction.
     *
     * @returns {H5P.ContentCopyrights}
     */
    self.getCopyrights = function () {
      if (library === 'H5P.Nil') {
        return undefined;
      }

      var instance = H5P.newRunnable(action, player.contentId);

      var interactionCopyrights;
      if (instance !== undefined && instance.getCopyrights !== undefined) {
        interactionCopyrights = instance.getCopyrights();
      }
      else if (instance !== undefined) {
        interactionCopyrights = H5P.getCopyrights(instance, parameters, player.contentId);
      }
      if (interactionCopyrights !== undefined) {
        interactionCopyrights.setLabel(title + ' ' + H5P.InteractiveVideo.humanizeTime(parameters.duration.from) + ' - ' + H5P.InteractiveVideo.humanizeTime(parameters.duration.to));
        return interactionCopyrights;
      }
      return undefined;
    };

    /**
     * Returns unique content id
     * @returns {String} Sub content Id
     */
    self.getSubcontentId = function () {
      return action.subContentId;
    };

    /**
     * Returns interaction element
     * @returns {*}
     */
    self.getElement = function () {
      return $interaction;
    };

    /**
     * Focus interaction element
     */
    self.focus = function () {
      if ($interaction) {
        $interaction.focus();
      }
    };

    /**
     * Create clipboard data object.
     * @returns {object}
     */
    self.getClipboardData = function () {
      return H5P.DragNBar.clipboardify(H5PEditor.InteractiveVideo.clipboardKey, parameters, 'action');
    };

    /**
     * Resize to fit wrapper so icon does not overflow
     * @param {H5P.jQuery} $wrapper
     */
    self.repositionToWrapper = function ($wrapper) {
        

      if ($interaction) {
        // Reset positions
        if (isRepositioned) {
          $interaction.css({
            'top': parameters.y + '%',
            'left': parameters.x + '%'
          });
		
          // Reset dimensions
          var height = '';
          var width = '';

          // Posters reset to standard dimensions
          if (!self.isButton()) {
            if(parameters.heightPX ==undefined || player.editor != undefined)
            	height = (parameters.height ? parameters.height : 10) + 'em';
            else
            	height = (parameters.heightPX ? parameters.heightPX : 10) + 'px';
            width = (parameters.width ? parameters.width : 10) + 'em';
          }

          $interaction.css({
            'height': height,
            'width': width
          });

          isRepositioned = false;
        }
     
        // Check if button overflows parent
      //  console.log("$interaction.position().top1:"+$interaction.position().top+"=$interaction.height()=="+$interaction.height()+"====$wrapper.height():"+$wrapper.height());
       // console.log("diff1:"+($interaction.position().top + $interaction.height() - $wrapper.height()));
        var interactionHiddenBottomValue = ($interaction.position().top + $interaction.height() - $wrapper.height());
        if(interactionHiddenBottomValue > 5){
       // if ($interaction.position().top + $interaction.height() > $wrapper.height()) {
          var newTop = (($wrapper.height() - $interaction.height()) / $wrapper.height()) * 100;
          // We must reduce interaction height
          if (newTop < 0) {
            newTop = 0;
            var newHeight = $wrapper.height() / parseFloat($interaction.css('font-size'));
            $interaction.css('height', newHeight + 'em');
          }
          $interaction.css('top', newTop + '%');
          isRepositioned = true;
        }

        if ($interaction.position().left + $interaction.width() > $wrapper.width()) {
          var newLeft = (($wrapper.width() - $interaction.width()) / $wrapper.width()) * 100;

          // We must reduce interaction width
          if (newLeft < 0) {
            newLeft = 0;
            var newWidth = $wrapper.width() / parseFloat($interaction.css('font-size'));
            $interaction.css('width', newWidth + 'em');
          }

          $interaction.css('left', newLeft + '%');
          isRepositioned = true;
        }
        
        
        //h5pcustomization - keep the components relavant to the boundaries if it is designed in bottom.
		if(H5P.jQuery(".h5p-interactivevideo-editor").size() == 0)
		{
		
		if(parameters.bottomSpaceInPx != undefined && parseInt(parameters.bottomSpaceInPx,10) <100)
		{
			isRepositioned = false;
			var newBottomInPx = $wrapper.height() - ($interaction.height()+$interaction.position().top);
			var topNeedTobeAdjustedValue = newBottomInPx - parseInt(parameters.bottomSpaceInPx,10);
			var interactionTop = $interaction.position().top + topNeedTobeAdjustedValue;
			var diff = 0;
			//handle bottom hide
			if($wrapper.height() <= (interactionTop+$interaction.height()))
			{
			   	diff = (interactionTop+$interaction.height()+5) - $wrapper.height() ; //5 for buffer
			   	if(diff < 15)
			   		diff = 15;
			}
			interactionTop = interactionTop - diff;
			$interaction.css('top', interactionTop);
		}
		if(parameters.availableSpaceInRight != undefined && parseInt(parameters.availableSpaceInRight,10) < 100)
		{
			isRepositioned = false;
			var spaceInRight = $wrapper.width() - ($interaction.width()+$interaction.position().left);
			var newRightValuePX = spaceInRight - parameters.availableSpaceInRight;
			$interaction.css("left",parseFloat($interaction.css("left"))+newRightValuePX);
			
			var spaceInRight = $wrapper.width() - ($interaction.width()+$interaction.position().left);
			if(spaceInRight <= 3)
				$interaction.css("left",parseFloat($interaction.css("left"))-15);	

			
		}

		
		}
		
      }
    };

    /**
     * Reset task.
     */
    self.resetTask = function () {
      if (action.userDatas !== undefined && action.userDatas.state !== undefined) {
        delete action.userDatas.state;
      }
      delete self.score;
      delete self.maxScore;

      self.reCreate();
    };

    // Create instance of content
    self.reCreate();
  }

  // Extends the event dispatcher
  Interaction.prototype = Object.create(EventDispatcher.prototype);
  Interaction.prototype.constructor = Interaction;

  /**
   * Tool for converting.
   *
   * @private
   * @type {H5P.jQuery}
   */
  var $converter = $('<div/>');

  return Interaction;
})(H5P.jQuery, H5P.EventDispatcher);


function replaceAll(find, replace, str) {
	  return str.replace(new RegExp(find, 'g'), replace);
}

