
var GLOBALPARAMS_H5P = {};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Merging js: default/files/h5p/libraries/H5P.DragNBar-1.2/scripts/dialog.js begins */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/*global H5P*/
H5P.DragNBarDialog = (function ($, EventDispatcher) {

	/**
	 * Controls the dialog in the interactive video.
	 *
	 * @class
	 * @param {H5P.jQuery} $container for dialog
	 * @param {H5P.jQuery} $videoWrapper needed for positioning of dialog
	 */
	function Dialog($container, $videoWrapper) {
		var self = this;
		var $isEdit = false;
		// Initialize event inheritance
		EventDispatcher.call(self);

		// Create DOM elements for dialog
		var $wrapper = $('<div/>', {
			'class': 'h5p-dialog-wrapper h5p-ie-transparent-background h5p-hidden',
			on: {
				click: function () {
					if (!self.disableOverlay)  {
						self.close();
					}
				}
			}
		});

		var $params = "";
		var $dialog = $('<div/>', {
			'class': 'h5p-dialog h5p-big',
			on: {
				click: function (event) {
					event.stopPropagation();
				}
			}
		}).appendTo($wrapper);

		// Create title bar
		// tip div
		var $e1tip = $('<div/>', {
			'class': 'e1-tip',
			appendTo: $dialog
		});

		var $titleBar = $('<div/>', {
			'class': 'h5p-dialog-titlebar titlebaronlyforpreview',
			appendTo: $dialog
		});


		var $dialogCont = $('<div/>', {
			'class': 'h5p-dialog-popup',
			appendTo: $dialog
		});

		var $title = $('<div/>', {
			'class': 'h5p-dialog-title', 
			appendTo: $titleBar
		});
		var $close = $('<div/>', {
			'class': 'h5p-dialog-close',
			tabindex: 0,
			title: window.parent.Drupal.t('LBL123'),
			on: {
				click: function (event) {
					if (event.which === 1) {
						self.close();
					}
				},
				keypress: function (event) {
					if (event.which === 32) {
						self.close();
					}
				}
			},
			appendTo: $titleBar
		});

		// Used instead of close
		var $customButtons;

		// Create inner DOM elements for dialog
		var $inner = $('<div/>', {
			'class': 'h5p-dialog-inner'
		}).appendTo($dialogCont);
		var $titleBar = $('<div/>', {
			'class': 'h5p-dialog-titlebar',
			appendTo: $dialogCont
		});



		// Add all to DOM
		$wrapper.appendTo($container);



		/**
		 * Reset the dialog's positioning
		 *
		 * @private
		 */
		var resetPosition = function () {
			// Reset positioning
			$dialog.css({
				left: '',
				top: '',
				height: '',
				width: '',
				fontSize: '',
				bottom: ''
			});
			$inner.css({
				width: '',
				height: '',
				overflow: ''
			});
		};

		/**
		 * Display overlay.
		 *
		 * @private
		 * @param {function} next callback
		 */
		var showOverlay = function (next) {
			$wrapper.show();
			$wrapper.removeClass('h5p-hidden');

			//h5pcustomize - removed animation
			/*setTimeout(function () {
        // Remove class on next tick to ensure css animation
        $wrapper.removeClass('h5p-hidden');
        if (next) {
          next();
        }
      }, 0); */
		};

		/**
		 * Close overlay.
		 *
		 * @private
		 * @param {function} next callback
		 */
		var hideOverlay = function (next) {
			$wrapper.addClass('h5p-hidden');
			$wrapper.hide();

			//h5pcustomize - removed animation

			/*    setTimeout(function () {
        // Hide when animation is done
        $wrapper.hide();
        if (next) {
          next();
        }
      }, 200);*/
		};

		/**
		 * Opens a new dialog. Displays the given element.
		 *
		 * @param {H5P.jQuery} $element
		 * @param {string} [title] Label for the dialog
		 * @param {string} [classes] For styling
		 * @param {H5P.jQuery} [$buttons] Use custom buttons for dialog
		 */
		self.open = function ($element, title, classes, $buttons,params,isEdit) {

			if(isEdit == false)
			{
				//	ns.$(".h5p-remove").click();
			}
			$isEdit = isEdit;
			jQObj = "";
			try
			{
				if(ns == undefined || ns == null)
					jQObj = $;
				else
					jQObj = ns.$;
			}catch(e){
				jQObj = $;
			}
			jQObj("#loader").remove();
			if(jQObj(".h5p-standalone").size() >  0  )
				jQObj(".titlebaronlyforpreview").show();
			else
				jQObj(".titlebaronlyforpreview").hide();


			showOverlay();

			$inner.children().detach().end().append($element);
			// Reset positioning
			resetPosition();
			//remove space from title
			if(title != null && title != undefined)
			{
				var title1=title.toLowerCase().replace(/ /g, '-');
				$dialog.attr("class","h5p-dialog h5p-big " +title1 +'-container');
			}
			$title.attr('class', 'h5p-dialog-title' + (classes ? ' ' + classes : ''));
			$params = params;

//			console.log($params);
			$title.html(title);


			// Clean up after previous custom buttons
			if ($customButtons) {
				$customButtons.remove();
				$close.show();
			}

			// Add new custom buttons

			if ($buttons) {
				$customButtons = $buttons;

				// Hide default close button
				$close.hide();
				//alert("1:"+JSON.stringify($buttons));
				// Add custom buttons
				$buttons.appendTo($titleBar);
			}

			self.resize();

			self.trigger('open');

			setTimeout(function () {
				if ($inner.find('input').length) {
					$inner.find('input').get(0).focus();
				}
			}, 100);
		};

		self.resize = function () {
			if (!$dialog.hasClass('h5p-big')) {
				return;
			}

			var fontSize = toNum($inner.css('fontSize'));
			var titleBarHeight = ($titleBar.outerHeight() / fontSize);

			// Same as height
			var maxHeight = $container.height();
			// minus dialog margins
			maxHeight -= Number($dialog.css('top').replace('px', '')) * 2;

			/*
      $inner.css({
              width: '100%',
              maxHeight: ((maxHeight / fontSize) - titleBarHeight) + 'em',
              marginTop: titleBarHeight + 'em'
            });*/

			$dialog.css({
				bottom: 'auto',
				maxHeight: ''
			});





			// var selectedComp1 = $inner.find(".library").find("select").val();
			var selectedComp = "";
			//alert($title.html());	


			if($isEdit == false)
			{
				if($title.html() == "Label")
					selectedComp = "e1-tip e1_video_nil";
				else if ($title.html() == 'Text') 
				{
					selectedComp = "e1-tip e1_video_text";
				}
				else if ($title.html() == 'Table') 
					selectedComp = "e1-tip e1_video_table";
				else if ($title.html() == 'Link') 
					selectedComp = "e1-tip e1_video_link";
				else if ($title.html() == 'Image') 
					selectedComp = "e1-tip e1_video_image";
				else if ($title.html() == 'Statements') 
					selectedComp = "e1-tip e1_video_summary";
				else if ($title.html() == 'Single Choice Set')
					selectedComp = "e1-tip e1_video_single";
				else if ($title.html() == 'Multiple Choice') 
					selectedComp = "e1-tip e1_video_multi";
				else if ($title.html() == 'Mark the Words') 
					selectedComp = "e1-tip e1_video_mark";
				else if ($title.html() == 'Drag Text') 
					selectedComp = "e1-tip e1_video_dragText";
				else if ($title.html() == 'Fill in the Blanks') 
					selectedComp = "e1-tip e1_video_blanks";
				else if ($title.html() == 'Go To Question') 
					selectedComp = "e1-tip e1_video_goTo";
			}  
			else
			{
				if ($title.html() == 'Text') 
				{
					//ns.$(".h5p-text-editor .ckeditor").focus(); //change default textbox to ckeditor
					//h5pcustomize
				}
			}

			if(jQObj(".e1-tip").size() == 0)
				jQObj(jQObj(jQObj(".h5p-dialog").children())[0]).attr("class",selectedComp);
			else
				$dialog.parent().find(".e1-tip").attr("class",selectedComp);
			//jQObj(".h5p-interactive-video-dragnbar").find(".h5p-dragnbar-ul li:nth-child(6)").remove();
			//alert(jQObj(".h5p-interactive-video-dragnbar").find(".h5p-dragnbar-ul li:nth-child(6)").html());
			//jQObj(".h5p-dragnbar-summary-button").closest('.li').remove();
			if ($title.html()  == 'Single Choice Set')	
			{
				launchInteractionComponent("SingleChoiceSet",$params);
				//var html = constructSingleChoiceSet($params);
				//jQObj(".h5p-dialog-inner").html(html);
			}else if($title.html() == "Text")
			{
				launchInteractionComponent($title.html(),$params);

			}else if($title.html() == "Label")
			{
				launchInteractionComponent($title.html(),$params);	 
			}else if($title.html() == "Link")
			{
				launchInteractionComponent($title.html(),$params);	 
			}
			else if($title.html() == "Image")
			{
				launchInteractionComponent($title.html(),$params);	 
			}
			else if($title.html() == "Mark the Words")
			{
				launchInteractionComponent("Mark_the_Words",$params);	 
			}
			else if($title.html() == "Drag Text")
			{
				launchInteractionComponent("Mark_the_Words",$params);	 
			}
			else if($title.html() == "Fill in the Blanks")
			{
				launchInteractionComponent("FillInTheBlanks",$params);
				if($params.action.params.questions != undefined)
				{
					for(var i = 0;i < $params.action.params.questions.length; i++)
					{
						addStatement($params);
					}
				}
				else
					addStatement($params);
				vtip();
			}
			else if($title.html() == "Table")
			{
				launchInteractionComponent("Table",$params);
				vtip();
			}
			else if($title.html() == "Go To Question")
			{
				// For removing the goto container at background		
				if($isEdit == false){
					jQObj(".h5p-interaction.h5p-poster.h5p-gotoquestion-interaction.h5p-dragnbar-element").last().css("display","none");

				}

				launchInteractionComponent("GotoQuestions",$params);

				if($params.action.params.choices != undefined)
				{

					for(var i = 0;i < $params.action.params.choices.length; i++)
					{
						addChoiceStatement('addOld',$params);
					}
				}
				else{

					addChoiceStatement('addOld',$params);
					addChoiceStatement('addOld',$params);
				}
				vtip();
			}

			//  });


			//      ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").html("welcome");  
			// var txt1 = "<p>:</p>"; 
			/*if ($title.html()  != 'Single Choice Set')	
           {
           	window.setTimeout(function(){

           	var txt1 = '<span class="h5peditor-label h5peditor-required"></span>'; 
           	var txt2 = '<img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png" class="vtip info-enr-upload" title="To display interaction in an expanded or collapsed format.">';

           	if(((jQObj(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5peditor-label").find('.vtip').size()) == 0) && ($title.html()  != 'Label')) {
          	jQObj(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5peditor-label").after(txt1).append(txt2);
              jQObj(".h5p-dialog-inner").find(".h5peditor-label").append(":"); 	
           	  jQObj(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find('.h5peditor-required').html(jQObj(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find('.h5peditor-required').html().replace(':', ''));
       	      jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.h5peditor-label').html(jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.h5peditor-label').html().replace(':', ''));


            }
            else if((jQObj(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5peditor-label").find('.vtip').size()) == 0){
        	   if((jQObj('.h5p-dialog-inner').find('.h5peditor-interaction-duration').find('.h5peditor-label').text().indexOf(':')) == -1)
            	   jQObj(".h5p-dialog-inner").find(".h5peditor-label").append(":");

        	    jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.h5peditor-label').html(jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.h5peditor-label').html().replace(':', ''));

           }
          	},10);
           	//vtip();
           	customizePauseOption($params);
          	//alert($params.pause);
          	//alert(parameters.pause);
           //	jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.checkPause').html(jQObj(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find('.checkPause').html().replace(':', ''));

           }*/

			if ($title.html()  == 'Image'){
				window.setTimeout(function(){
					jQObj(".h5p-image-library").find(".file").find(".add").css("background","none");
					jQObj(".h5p-image-library").find(".file").find(".add").css("width","140px");
					jQObj(".h5p-image-library").find(".file").find(".add").css("height","40px");
					jQObj(".h5p-image-library").find(".file").find(".add").css("color","#ddd");
					jQObj(".h5p-image-library").find(".file").find(".add").html("Upload");
				},10);       
			}

			if ($title.html()  == 'Fill in the Blanks'){
				window.setTimeout(function(){
					jQObj(".h5p-blanks-library").find(".h5p-copyright-button").remove();
					jQObj(".h5p-blanks-library").find(".file").find(".add").css("background","none");
					jQObj(".h5p-blanks-library").find(".file").find(".add").css("width","140px");
					jQObj(".h5p-blanks-library").find(".file").find(".add").css("height","40px");
					jQObj(".h5p-blanks-library").find(".file").find(".add").css("color","#ddd");
					//ns.$(".h5p-blanks-library").find(".file").find(".h5p-editor-image-buttons").css("padding-left","100px");
					jQObj(".h5p-blanks-library").find(".file").find(".add").html("Upload");

				},10);       
			}

			//jQObj(".h5p-dragnbar-summary-button").unwrap();
			// jQObj(".h5p-dragnbar-summary-button").closest('.li').remove();
			//jQObj(".h5p-interactive-video-dragnbar").find(".h5p-dragnbar-ul li:nth-child(6)").remove();
			//alert(jQObj(".h5p-interactive-video-dragnbar").find(".h5p-dragnbar-ul li:nth-child(6)").html());


			//vtip();
			//position the dialog to the center of screen
			if($isEdit == true)
			{
				//backdrop overlay
				//h5pcustomize      
				if(jQObj(".h5p_int_overlay").size() == 0)
					jQObj("<div class='h5p_int_overlay'></div>").insertBefore(jQObj(".h5p-dialog"));


				/*var containerWidth =  $container.width(); 
		    var containerHeight = $container.height();
			//$dialog.css({"top":"100px","left":"100px"});
       		var dialogWidth =  $dialog.outerWidth(true);
       		var dialogHeight = $dialog.outerHeight(true);*/
				$dialog.css({
					'position': 'fixed'});
				/*'top': parseInt((containerHeight / 2) - (dialogHeight/ 2), 10),
    					'left': parseInt((containerWidth / 2) - (dialogWidth / 2), 10),
    					//'width':"450px"
						});*/
				jQObj(".h5p-dialog-wrapper").addClass("level-two");
				jQObj(".h5p-dialog-wrapper").css("top","0px");
			}
			else
			{
				jQObj(".h5p-dialog-wrapper").removeClass("level-two");
				jQObj(".h5p-dialog-wrapper").css("top","35px");
				//  	jQObj(".h5p-dialog-wrapper").css("position","fixed");
				jQObj(".h5p-dialog").css("position","");
			}

			//h5pcustomize - remove margin-top
			if(jQObj(".h5p-minimal").size() >  0 || window.parent.$("#iframedata").size() > 0 || window.parent.$("#h5pplayer").size() >0)
			{
				jQObj(".h5p-dialog-wrapper").css("top","0px");
			}
		};

		/**
		 * Adds a name to the dialog for identifying what it contains.
		 *
		 * @param {string} machineName Name of library inside dialog.
		 */
		self.addLibraryClass = function (machineName) {
			$dialog.attr('data-lib', machineName);
		};

		self.isOpen = function () {
			return $wrapper.is(':visible');
		};

		/**
		 * Reposition the currently open dialog relative to the given button.
		 *
		 * @param {H5P.jQuery} $button
		 * @param {Object} [size] Sets a size for the dialog, useful for images.
		 * @param {boolean} [medium=false] Sets a min. size for medium dialogs.
		 */
		self.position = function ($button, size, medium) {
			resetPosition();
			//it is for small and medium devices. however h5p is not supportable for mobile in this release. - h5pcustomization
			medium = false;
			$dialog.removeClass('h5p-big h5p-medium');
			var titleBarHeight = Number($inner[0].style.marginTop.replace('em', ''));

			// Use a fixed size
			if (size) {
				var fontSizeRatio = 16 / toNum($container.css('fontSize'));

				// Fixed width
				if (size.width) {
					size.width = (size.width * fontSizeRatio);
					$dialog.css('width', size.width + 'em');
				}

				// Fixed height
				if (size.height) {
					size.height = (size.height * fontSizeRatio) + titleBarHeight;
					$dialog.css('height', size.height + 'em');

					$inner.css({
						width: 'auto',
						overflow: 'hidden'
					});
				}
			}

			if (medium) {
				$dialog.addClass('h5p-medium');
			}

			var buttonWidth = $button.outerWidth(true);
			var buttonPosition = $button.position();
			var containerWidth =  $container.width(); 
			var containerHeight = $container.height();
			//alert("Left:"+buttonPosition.left+" top going to be set:"+buttonPosition.top);
			// Position dialog horizontally
			var left = buttonPosition.left;
			var top = buttonPosition.top;
			var dialogWidth = $dialog.outerWidth(true);
			if (medium && dialogWidth > containerWidth) {
				// If dialog is too big to fit within the container, display as h5p-big instead.
				// Only medium dialogs can become big
				$dialog.addClass('h5p-big');
				return;
			}

			if (buttonPosition.left > (containerWidth / 2) - (buttonWidth / 2)) {
				// Show on left
				left -= dialogWidth - buttonWidth;
			}

			// Make sure the dialog is within the video on the right.
			if ((left + dialogWidth) > containerWidth) {
				left = containerWidth - dialogWidth;
			}
			var marginLeft = parseInt($videoWrapper.css('marginLeft'));
			if (isNaN(marginLeft)) {
				marginLeft = 0;
			}

			// And finally, make sure we're within bounds on the left hand side too...
			if (left < marginLeft) {
				left = marginLeft;
			}
			//	alert('top set now:'+top);
			// Position dialog vertically
			var marginTop = parseInt($videoWrapper.css('marginTop'));
			if (isNaN(marginTop)) {
				marginTop = 0;
			}

			/*alert('medium'+medium);
      alert('buttonPosition.top'+buttonPosition.top);
      alert('marginTop'+marginTop);
      alert('$dialog.outerHeight(true)'+$dialog.outerHeight(true));
      var top = (medium ? 0 : (buttonPosition.top + marginTop));
      alert('top'+top);*/
			var totalHeight = top + $dialog.outerHeight(true);
			//alert('totalHeight'+totalHeight);
			if (totalHeight > containerHeight) {
				//alert('containerHeight'+containerHeight);
				top -= totalHeight - containerHeight;
				//alert('top'+top);
			}
			//alert('top again:'+top);

			var maxHeight = $container.height() - top + $dialog.height() - $dialog.outerHeight(true);
			var fontSize = toNum($container.css('fontSize'));


			//h5pcustomize
			//for preview and play, show interactions top of the video so that it will be visible all the scenario.
			var dialogHeight = $dialog.height();
			var removeMaxHeightForDialogAndinner  = false;  
			if(window.parent.$("#h5pplayer").size() > 0 || $dialog.find(".titlebaronlyforpreview").size() > 0)
			{
				containerHeight -=42; 
				if(containerHeight < (dialogHeight + top))
				{

					top -=  160;//(dialogHeight + top) - containerHeight;
					removeMaxHeightForDialogAndinner = true;
				}
				// alert("containerWidth:"+containerWidth+"left:"+left+"dialogWidth:"+dialogWidth+"top:"+top+"containerHeight:"+containerHeight+"dialogHeight"+dialogHeight);
				if(containerWidth - (left + dialogWidth) <= 15) {
					left -= 15;
					//	 alert("new left"+left);
				}
				if(top <= 20)
					top = 20;


				if($dialog.attr("data-lib") == "H5P.Image")
				{
					$dialog.css({
						top: (top / (containerHeight / 100)) + '%',
						left: (left / (containerWidth / 100)) + '%',
						width: (window.getComputedStyle($dialog[0]).width / fontSize) + 'em',
						maxHeight: (maxHeight / fontSize) + 'em'
					});

					if($dialog.css("top") == "0px" || $dialog.css("top") == "0")
						$dialog.css("top","20px"); //provide space to display close icon in the top.

					console.log("$dialog.css('top'):"+$dialog.css("top"));
					console.log("$dialog.css('left'):"+$dialog.css("left"));
					console.log("$dialog.css('height'):"+$dialog.css("height"));
					var dialogTopAndHeight = parseInt($dialog.css('height'),10)+parseInt($dialog.css('top'),10);
					console.log("containerHeight:"+containerHeight);
					console.log("dialogTopAndHeight:"+dialogTopAndHeight);
					if(dialogTopAndHeight > containerHeight)
					{
						var oldTop = parseInt($dialog.css("top"),10);
						var newTop = oldTop - ((dialogTopAndHeight - containerHeight)+36); //seekbar height is 36
						console.log("newTop13:"+newTop);
						$dialog.css("top",newTop);
					}
					$dialog.css({
						//  	maxHeight:  ((maxHeight) / fontSize) + 'em',
						height:"auto"
					});

				}
				else
				{
					//alert('left:'+left+'==top:'+top+'containerHeight:'+containerHeight+'==containerWidth:'+containerWidth);
					$dialog.css({
						top: (top / (containerHeight / 100)) + '%',
						left: (left / (containerWidth / 100)) + '%',
						width: (window.getComputedStyle($dialog[0]).width / fontSize) + 'em',
						maxHeight: (maxHeight / fontSize) + 'em'
					});


				}


			}
			else
			{
				// Set dialog size
				$dialog.css({
					top: (top / (containerHeight / 100)) + '%',
					left: (left / (containerWidth / 100)) + '%',
					width: (window.getComputedStyle($dialog[0]).width / fontSize) + 'em',
					maxHeight: (maxHeight / fontSize) + 'em'
				});
			}

			if($dialog.attr("data-lib") != "H5P.Image")
			{
				$inner.css('maxHeight', ((maxHeight - $titleBar.outerHeight(true)) / fontSize) + 'em');

			}
			else
				$dialog.css({'maxHeight':(containerHeight - 40)});
			if(removeMaxHeightForDialogAndinner) //remove MaxHeight when interaction displays bottom of the video
			{
				$dialog.css({'maxHeight':''});
				$inner.css({'maxHeight': ""});
			}

		};

		/**
		 * Find max available space inside dialog when positioning relative to
		 * given button.
		 *
		 * @param {H5P.jQuery} $button
		 * @param {Boolean} fullScreen True if dialog fills whole parent
		 * @returns {Object} Attrs: width, height
		 */
		self.getMaxSize = function ($button, fullScreen) {
			var buttonWidth = $button.outerWidth(true);
			var buttonPosition = $button.position();
			var containerWidth = $container.width();

			var max = {};
			max.height = Number($inner.css('maxHeight').replace('px', ''));
			if (fullScreen) {
				max.width = containerWidth;
			}
			else {
				if (buttonPosition.left > (containerWidth / 2) - (buttonWidth / 2)) {
					// Space to the left of the button minus margin
					max.width = buttonPosition.left;
				}
				else {
					// Space to the right of the button minus margin
					max.width = (containerWidth - buttonPosition.left - buttonWidth);
				}
			}

			// Use em
			var fontSize = toNum($container.css('fontSize'));
			max.width = (max.width / fontSize) * (fontSize / 16);
			max.height = (max.height / fontSize) * (fontSize / 16);

			return max;
		};

		/**
		 * Scroll to given position in current dialog.
		 *
		 * @param {number} to Scroll position
		 * @param {number} ms Time the animation takes.
		 */
		self.scroll = function (to, ms) {
			$inner.stop().animate({
				scrollTop: to
			}, ms);
		};

		/**
		 * Close the currently open dialog.
		 */
		self.close = function () {

			$wrapper.addClass('h5p-hidden');
			$wrapper.hide();		

			//h5pcustomize - removed animation 
			/*setTimeout(function () {
        $wrapper.hide();
        self.disableOverlay = false;
        $close.show();
      }, 201);

      self.trigger('close'); */

			// Let others reach to the hiding of this dialog
			self.trigger('domHidden', {
				'$dom': $wrapper,
				'key': 'dialogClosed'
			}, {'bubbles': true, 'external': true});
		};

		/**
		 * Open overlay only.
		 */
		self.openOverlay = function () {
			self.disableOverlay = true;
			$dialog.hide();
			showOverlay();
		};

		/**
		 * Close overlay only.
		 */
		self.closeOverlay = function () {
			$wrapper.addClass('h5p-hidden');
			hideOverlay(function () {
				$dialog.show();
				self.disableOverlay = false;
			});
		};

		/**
		 * Removes the close button from the current dialog.
		 */
		self.hideCloseButton = function () {
			$close.hide();
		};

		/**
		 * Get width of dialog
		 * @returns {Number} Width of dialog
		 */
		self.getDialogWidth = function () {
			return $dialog.width();
		};

		/**
		 * Reset dialog width
		 */
		self.removeStaticWidth = function () {
			$dialog.css('width', '');
		};
	}

	// Extends the event dispatcher
	Dialog.prototype = Object.create(EventDispatcher.prototype);
	Dialog.prototype.constructor = Dialog;


	/**
	 * Converts css px value to number.
	 *
	 * @private
	 * @param {string} num
	 * @returns {Number}
	 */
	var toNum = function (num) {
		return Number(num.replace('px',''));
	};

	return Dialog;
})(H5P.jQuery, H5P.EventDispatcher);




function constructSingleChoiceSet(interaction)
{
	if(interaction.label == undefined)
		interaction.label = "";

	if(interaction.duration.from.toString().indexOf(":") < 0)
	{
		interaction.duration.from = convertMinutes(interaction.duration.from);
		interaction.duration.to = convertMinutes(interaction.duration.to.toString());
	}

	var html = "";//<div class='h5p-dialog-inner'>";
	html += '<div class="single-choice-container">';
	html += '<div class="field duration h5peditor-interaction-duration">';
	html += '  <label class="h5peditor-label-wrapper"><span class="h5peditor-label h5peditor-required">Display time:</span><input id="singlechoicesetfrom" class="h5peditor-text" type="text" value="'+interaction.duration.from+'" placeholder="From" maxlength="15"> - <input id="singlechoicesetto" class="h5peditor-text" type="text" value="'+interaction.duration.to+'" placeholder="To" maxlength="15"></label>';
	html += '      <div class="h5p-errors"></div>';
	html += '   </div>';
	html += '   <div class="field boolean h5peditor-interaction-pause">';

	var chk ="checked";
	if(interaction.pause == false){
		chk = "";
		//alert(interaction.pause);
	}
	html += '      <label class="h5peditor-label"><input id="singlechoicesetpausevideo" '+chk+' type="checkbox">Pause video</label>';
	html += '      <div class="h5p-errors"></div>';
	html += '	</div>';
	html += '   <div class="field text h5p-image-radio-button-group">';
	// html += '  <div class="h5peditor-label"><span class="h5peditor-label h5peditor-required">Display as:</span></div>';
	html += '  <div class="h5peditor-label"><span class="h5peditor-label h5peditor-required">Display as:</span><img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/help.png" class="vtip info-enr-upload"  title="To display interaction in an expanded or collapsed format."></div>';

	html += '  <div class="h5p-image-radio-button-container">';
	html += '     <div class="h5p-image-radio-button button">';

	var chkButton ='',chkPoster = '';

	if((interaction.displayType == "button")){
		chkButton = 'checked = "checked" '; 

	}
	else{
		chkPoster ='checked = "checked" ';

	}

	html += '        <input type="radio" name="displayType" value="button" onclick="showLabel()"   id="h5p-image-radio-button-0" '+chkButton+'>';
	html += '        <label for="h5p-image-radio-button-0">';
	html += '           <div class="image-container" alt="Button"></div>';
	html += '           <span>Button</span>';
	html += '        </label>';
	html += '     </div>';
	html += '     <div class="h5p-image-radio-button poster">';
	html += '        <input type="radio" name="displayType" value="poster" onclick="hideLabel()"  id="h5p-image-radio-button-1" '+chkPoster+'>';
	html += '        <label for="h5p-image-radio-button-1">';
	html += '           <div class="image-container" alt="Poster"></div>';
	html += '           <span>Poster</span>';
	html += '        </label>';
	html += '     </div>';
	html += '  </div>';
	html += '  <div class="h5peditor-field-description"><b>Button</b> is a collapsed interaction the user must press to open. <b>Poster</b> is an expanded interaction displayed directly on top of the video</div>';
	html += '</div>';

	html += '<div class="field text h5peditor-interaction-label OptionalLabel">';
	html += '      <label class="h5peditor-label">Label:</label>';
	html += '      <div class="ckeditor" tabindex="0" id="singlechoicesetlabel"  contenteditable="true">'+interaction.label+'</div>';
	html += '      <div class="h5p-errors"></div>';
	html += '      <div class="h5peditor-field-description">Label displayed next to interaction icon.</div>';
	html += '</div>'; 	

	html += "<div class='questionandanswers'>"; 

	if(interaction.action.params.choices != null)
	{
		for( var i = 0 ; i < interaction.action.params.choices.length;i++)
		{
			html += displayQuestionSet(interaction.action.params.choices[i],i+1);
		}
	}
	else
	{
		html += displayQuestionSet();
	}


	html += "</div>"; //questionandanswer



	html += '</div>';
	//  	html += '</div>'; 

	// vtip(); 
	return html;
}


function removeEntity(domobj,from)
{

	//ns.$(domobj).parent().parent().remove();

	if(ns.$(domobj).hasClass("ansBlock"))
	{
		ns.$(domobj).parent().parent().parent().remove();	
		//reshuffleOptionCount();
		var opt = ns.$(".questionset"+ns.$(domobj).attr("quesNumber")).find(".answercontainerwrapper").find(".answercontainer:nth-child(3)").find(".ans-set .option-set .h5peditor-label").html();

		//if(opt == "Option 4:")
		if(opt == window.parent.Drupal.t('LBL387')+" 4:")
		{
			ns.$(".questionset"+ns.$(domobj).attr("quesNumber")).find(".answercontainerwrapper").find(".answercontainer:nth-child(3)").find(".ans-set .option-set .h5peditor-label").html(window.parent.Drupal.t('LBL387')+" 3:");
		}

	}
	else if(ns.$(domobj).hasClass("quesBlock"))
	{
		ns.$(domobj).parent().parent().remove();
		reshuffleQuestionCount();
	}
}


function convertMinutes(time)
{
	if(time == undefined || time == null)
	{
		time = "0:00";
		return time;
	}
	if(time!= null && new String(time).indexOf(":")>0)
		return time;
	var minutes = Math.floor(time / 60);
	var seconds = ""+(time - minutes * 60);

	if(seconds.length == 1)
		seconds = "0"+seconds;
	return minutes+":"+seconds;
}
/*
function convertToSeconds(input) {
	alert(input);
    var parts = input.split(':'),
        minutes = +parts[0],
        seconds = +parts[1];
    return (minutes * 60 + seconds).toFixed(0);
}
 */
function convertToSeconds(input) {
	//adding code for more than an hour video support
	var matchesCount = input.split(":").length - 1;
	var hours = 0;
	var minutes = 0;
	var seconds = 0;
	var parts = input.split(':');
	if(matchesCount == 2)
	{
		hours = +parts[0];
		minutes = +parts[1];
		seconds = +parts[2];
	}
	else{
		minutes = +parts[0];
		seconds = +parts[1];
	} 
	return ((hours * 60 * 60) + minutes * 60 + seconds).toFixed(0);
}

function CheckFunc(obj)
{
	//alert($);
	ns.$(obj).find('.btn-question').show();
//	$('.questionandanswers .addedit-form-cancel-container-actions .admin-save-pub-unpub-button-container.btn-question #addquestion_btn').show();

}


/*

function displaySingleChoiceSetAction(){
	var op="";
	op +='	<div class="admin-save-pub-unpub-button-container">';
		op +='		<div class="admin-save-button-left-bg"></div>';
		op+='		<input class="addedit-edit-contentauthor-basic-save edit-catalog-course-save-publish addedit-form-expertusone-throbber form-submit ajax-processed"  data-wrapperid="contentauthor-addedit-form" tabindex="4" type="submit" id="content_save_btn" name="save" value="Save">';
		op+='			<span id="pub-unpub-action-btn" onclick="displayPubActionListSCL();" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">hello</span>';
		op+='		</div>';

	op +='<ul class="catalog-pub-add-list" style="display:none;">';
			op +='	<li class="save-pub-unpub-sub-menu">';
					op +='<input data-wrapperid="contentauthor-addedit-form" onclick="displayPubActionListSCL();" tabindex="5" class="addedit-edit-catalog-course-save-publish addedit-form-expertusone-throbber admin-save-button-middle-bg form-submit ajax-processed" type="submit" id="content_save_pub_btn" name="saveandpublish" value="Save and Publish">';
					op +='</li>';
					op +='</ul>';
					return op;
}

function displayPubActionListSCL(){
	//console.log("well");
	ns.$(".catalog-pub-add-list").show();
}

 */


function hideLabel(){

	ns.$('.OptionalLabel').hide();

}

function showLabel(){

	ns.$('.OptionalLabel').show();

}


function closeInteraction(){

	ns.$('#delete-object-dialog').hide();
	//ns.$('.h5p-big').hide();
	ns.$('.h5p-big').hide();
	ns.$('.e1-tip').hide();

}
function NotCloseInteraction(){

	ns.$('#delete-object-dialog').hide();
	//ns.$('#delete-object-dialog').dnb.dialog.close();

}

function customizePauseOption(interactions) {

	/*if(interactions.pause == false) {
	ns.$(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find(".h5peditor-label").addClass("uncheckPause");
	ns.$(".uncheckPause").bind("click",function() {
		ns.$(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find(".h5peditor-label").toggleClass("uncheckPause");
	   	ns.$(".h5p-dialog-inner").find(".h5peditor-interaction-pause").find(".h5peditor-label").toggleClass("checkPause");
	  });
		ns.$(".checkPause").bind("click",function() {
	  });
	} 
	if(interactions.displayType == "button") {
	    ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").addClass("buttonIsChecked");
        ns.$(".buttonIsChecked").bind("click",function() {
 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").removeClass("posterIsChecked");
 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").addClass("buttonIsChecked");
 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").addClass("posterIsUnchecked");
 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").removeClass( "buttonIsUnchecked" );
   	   	  });
	 	}
	 else{
		 ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").addClass("posterIsChecked");
		 ns.$(".posterIsChecked").bind("click",function() {
			 ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").addClass("posterIsChecked");
	 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").removeClass("buttonIsChecked");
	 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").removeClass("posterIsUnchecked");
	 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").addClass( "buttonIsUnchecked" );

           });
	    }
	ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").addClass("posterIsUnchecked");
	ns.$(".posterIsUnchecked").bind("click",function() {
		ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").addClass("posterIsChecked");
		ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").removeClass("buttonIsChecked");
		ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".poster").removeClass("posterIsUnchecked");
	 	     ns.$(".h5p-dialog-inner").find(".h5p-image-radio-button-group").find(".h5p-image-radio-button-container").find(".button").addClass( "buttonIsUnchecked" );
	});*/
}



function pauseVideoEvent(obj)
{
	if(ns.$(obj).is(":checked")){
		ns.$(obj).parent().removeClass("uncheckPause");
		ns.$(obj).parent().addClass("checkPause");
	}
	else{
		ns.$(obj).parent().addClass("uncheckPause");
		ns.$(obj).parent().removeClass("checkPause");

	}
} 

function displayTypeEvent(obj)
{
	if(ns.$(obj).attr("id") == "h5p-image-radio-button-0" &&  ns.$(obj).is(":checked"))
	{
		ns.$("#labelid").parent().parent().show();
		ns.$("#singlechoicesetlabel").parent().show();
		ns.$(obj).parent().addClass("buttonIsChecked");
		ns.$(obj).parent().removeClass("buttonIsUnchecked");
		ns.$("#h5p-image-radio-button-1").parent().addClass("posterIsUnchecked");
		ns.$("#h5p-image-radio-button-1").parent().removeClass("posterIsChecked");
	}
	else
	{
		ns.$("#labelid").parent().parent().hide();
		ns.$("#singlechoicesetlabel").parent().hide();
		ns.$(obj).parent().addClass("posterIsChecked");
		ns.$(obj).parent().removeClass("posterIsUnchecked");
		ns.$("#h5p-image-radio-button-0").parent().removeClass("buttonIschecked");
		ns.$("#h5p-image-radio-button-0").parent().addClass("buttonIsUnchecked");

	}
}

function launchInteractionComponent(compName,$params)
{
	GLOBALPARAMS_H5P = null;

	GLOBALPARAMS_H5P = H5P.cloneObject($params);

	console.log("GLOBALPARAMS_H5P:::::sure:",GLOBALPARAMS_H5P);
	GLOBALPARAMS_H5P.duration.fromDisplay = convertMinutes(GLOBALPARAMS_H5P.duration.from);
	GLOBALPARAMS_H5P.duration.toDisplay = convertMinutes(GLOBALPARAMS_H5P.duration.to.toString());
	
	var html = "";
	if(compName == "SingleChoiceSet")
		html ="<div class='single-choice-container'>";
	else
		html ="<div>"; 
	//html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/Header.ejs?ts='+new Date().getTime()}).render($params);
	//html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/'+compName+'.ejs?ts='+new Date().getTime()}).render($params);
	html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/Header.ejs'}).render($params);
	html += new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/'+compName+'.ejs'}).render($params);

	html += "</div>";
	jQObj(".h5p-dialog-inner").html(html);
	jQObj(".h5p-dragnbar-context-menu").show();

	//stripe out html tags
	$("body").on("blur",".striptags",function(e){
		$(this).html(stripTags($(this).html()));
	});





}

function vtip() {
	try{
		console.log('vtip entered..return3.');
		this.xOffset = -10; // x distance from mouse
		this.yOffset = 10; // y distance from mouse
		$ = ns.$;

		$("body").attr("id","page-container");
		$(".vtip").unbind("hover mousemove mousedown").hover(
				function(e) {
					this.pageCon=$("#page-container").offset();
					this.docW=$(document).width();

					this.pageConLpos=this.pageCon.left;
					this.pageConw=$("#page-container").width();
					/* change done tio override browse button's default title behavior (No file Chosen)
			Done for Roster Upload icon in Enrollments popup*/
					if($(this).hasClass('browse-button-vtip')) {
						this.titletemp = this.getAttribute('titletemp');
					}
					this.t = (this.title != '' ? this.title : (this.titletemp !== undefined ? this.titletemp : ''));
					this.title = '';
					if(this.t != '') {

						this.top = (e.pageY + yOffset);
						if(this.t == window.parent.Drupal.t('LBL847') || this.t == window.parent.Drupal.t('LBL846')) {
							this.top = (e.pageY + yOffset)-95;
						} else {
							this.top = (e.pageY + yOffset);
						}
						this.left = (e.pageX + xOffset);
						var pageUrl = window.location.search;
						if(window.location.href.indexOf("admincalendar") >0 ) {
							if(this.t.length > 100) {
								timeLen  = $(this).find(".titletimestyle").html().length + 1;
								this.strLen = this.t.length-timeLen;
							}
							else {
								this.strLen = this.t.length
							}
						} else {
							this.strLen = this.t.length
						}  
						//this.strLen = this.t.length;
						this.curWidth=this.left - this.pageConLpos;

						this.totWidthC=this.pageConw -this.curWidth;
						console.log(this.totWidthC);

						if(this.t == window.parent.Drupal.t('LBL847') || this.t == window.parent.Drupal.t('LBL846')) {
							console.log(10.1);

							$('body').append( '<p id="vtip"><img style="right:5px;" id="vtipArrow" />' + this.t + '</p>' );
						} else if($(this).hasClass('info-enr-upload')) {
							$('body').append( '<p id="vtip"><img style="right:5px;" id="vtipArrow" />' + this.t.replace(/<br>/g, '<br>') + '</p>' );
						} else {
							console.log(10.2);
							$('body').append( '<p id="vtip"><img style="left:5px;" id="vtipArrow" />' + window.parent.htmlEntities(this.t) + '</p>' ); // 46942 - html encode support added
						}

						if(this.t == window.parent.Drupal.t('LBL847') || this.t == window.parent.Drupal.t('LBL846')) {
							console.log(10.3);
							$('body').append( '<p id="vtip"><span style="right:5px;" id="vtipArrow" class="vtip-arrow" > </span>' + this.t + '</p>' );
						} else if($(this).hasClass('info-enr-upload')) {
							console.log(10.4);
							$('body').append( '<p id="vtip"><span style="left:5px;" id="vtipArrow" class="vtip-arrow"> </span>' + this.t.replace(/<br>/g, '<br>') + '</p>' );
						} else {
							console.log(10.5);
							$('body').append( '<p id="vtip"><span style="left:5px;" id="vtipArrow" class="vtip-arrow"> </span>' + this.t.replace(/<br>/g, '<br>') + '</p>' );
							//$('body').append( '<p id="vtip"><span style="left:5px;" id="vtipArrow" class="vtip-arrow"> </span>' + window.parent.htmlEntities(this.t) + '</p>' ); // 46942 - html encode support added

						}
						//$('p#vtip #vtipArrow').attr("src", '/sites/all/themes/core/expertusone/expertusone-internals/images/vtip_arrow.png');
						this.vtipW=$('p#vtip').width();
						console.log(11);
						if(this.strLen>100) {
							var posLeft = this.left;
							if(this.totWidthC < 500){
								$('p#vtip #vtipArrow').css('left','200px');
								posLeft = posLeft -200;
							}
							console.log(12);
							if($(this).hasClass('medium-vtip')) {
								console.log(12.1);
								$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").css('width','330px').fadeIn(100);
								$("p#vtip").css('max-width', '340px');

								console.log(12.2);
							}
							else
								$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").css('width','500px').fadeIn(100);
							console.log(13);
						} else {
							console.log(14);
							var pvtipwidth = $("p#vtip").width();
							var posLeft = this.left;
							var vtiparrow = pvtipwidth -10;
							if(this.totWidthC < pvtipwidth){
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
								posLeft = posLeft -pvtipwidth;
							}
							console.log(15.1);
							$('p#vtip').css("top", this.top+100+"px").css("left", posLeft+"px").fadeIn(100);
							$("p#vtip").css('max-width', '600px');
							console.log(16);
						}
						if($(this).hasClass('short-vtip')) {
							$("p#vtip").css('width', '200px').css('word-break', 'break-all');
						}
						else if($(this).hasClass('info-enr-upload')) {
							$("p#vtip").css('max-width', '230px');
						}

					}
					this.titletemp = this.t;

				},
				function() {
					if($(this).hasClass('browse-button-vtip')) {
						this.title = "";
					} else {
						this.title = this.t;
					}
					$("p#vtip").fadeOut("slow").remove();
				}
		).mousemove(
				function(e) {
					if(this.t != '') {
						this.pageCon=$("#page-container").offset();
						this.docW=$(document).width();
						this.pageConLpos=this.pageCon.left;
						this.pageConw=$("#page-container").width();
						this.curWidth=this.left - this.pageConLpos;
						this.totWidthC=this.pageConw -this.curWidth;
						this.top = (e.pageY + yOffset);
						//this.left = (e.pageX + xOffset);

						if(this.t == window.parent.Drupal.t('LBL847') || this.t == window.parent.Drupal.t('LBL846')) {
							this.left = (e.pageX + xOffset)-95;
						} else {
							this.left = (e.pageX + xOffset);
						}
						if(this.strLen>100) {
							var posLeft = this.left;
							if(this.totWidthC < 500){
								$('p#vtip #vtipArrow').css('left','200px');
								posLeft = posLeft -200;
							}else{
								$('p#vtip #vtipArrow').css('left','5px');
							}

							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").css('width','500px').fadeIn(100);
						} else {
							var pvtipwidth = $("p#vtip").width();
							var posLeft = this.left;
							var vtiparrow = pvtipwidth -10;
							if(this.totWidthC < pvtipwidth){
								posLeft = posLeft -pvtipwidth;
								$('p#vtip #vtipArrow').css('left',vtiparrow+'px');
							}else{
								$('p#vtip #vtipArrow').css('left','5px');
							}
							$('p#vtip').css("top", this.top+"px").css("left", posLeft+"px").fadeIn(100);
							$("p#vtip").css('max-width', '600px');
						}
						if($(this).hasClass('short-vtip')) {
							$("p#vtip").css('width', '200px').css('word-break', 'break-all');
						}
						else if($(this).hasClass('info-enr-upload')) {
							$("p#vtip").css('max-width', '230px');
						}
					}
				}
		).mousedown(
				function(e) {
					if($(this).hasClass('browse-button-vtip')) {
						this.title = "";
					} else {
						this.title = this.t;
					}
					$("p#vtip").fadeOut("slow").remove();
				}
		);
		$('.lnrreports-search-display-table-name').removeClass('fade-out-title-container-unprocessed');
		$('.lnrreports-search-table-accordion-content').removeClass('fade-out-title-container-unprocessed');//fix for reports issue
		$('.fade-out-title-container-unprocessed').each(function() {
			//console.log('unprocessed'+$(this).width());
			//console.log('span'+$(this).find('.title-lengthy-text').width());
			if(($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) || $(this).find('.title-lengthy-text').text() == '') {
				$(this).find('.fade-out-image').remove();
			}
			$(this).removeClass('fade-out-title-container-unprocessed');
		});
		/*0051180: Title Restriction - Space available to display the report name */
		$('#paintLearnerReportsResults').find('.spotlight-item-title').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		/*0051207: In Class page,fadeout effect were applied even it is small text. */
		$('#narrow-search-filtersets-holder').find('.narrow-search-filterset-item-label').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#paintLanguage').find('.srch-label-cls').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#admin-add-permissions').find('.user-list-detail').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$('#group-scroll-wrapper').find('.cls-access-list-select').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
	}catch(e){
		console.log("error:",e);
		// to do
	}
}

function closeH5PIcon(obj)
{
	ns.$(".h5p-remove").click();
	ns.$(".h5p-dialog-wrapper").removeClass("level-two");
	ns.$(".h5p-dialog-wrapper").css("top","35px");

}
function addStatement($params)
{ 

	var stCount = ns.$(".statements_container .stmt_numcount").size();
	if(stCount == undefined)
		stCount = 1;
	else
		stCount = parseInt(stCount,10)+1;
	GLOBALPARAMS_H5P.stmtcount = stCount;	
	var html = new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/FillInTheBlanksStatement.ejs'}).render(GLOBALPARAMS_H5P);
	ns.$(".statements_container").append(html);   
}

function deleteStatement(obj)
{
	ns.$(obj).parent().parent().remove();
	reshuffleStatementCount();
}

function reshuffleStatementCount()
{
	var count1 = 1;
	ns.$(".statements_container .stmt_numcount").each(function(){
		ns.$(this).html(count1++);
	});
}

function deleteChoiceStatement(obj)
{
	ns.$(obj).parent().parent().remove();
	reshuffleChoiceCount();
}


function reshuffleChoiceCount()
{
	var count1 = 1;
	ns.$(".choices_container .choice_numcount").each(function(){
		ns.$(this).html(count1++);
	});
}

 function reshuffleOptionCount(cnt,from)
{
	
		ns.$(".questionset"+cnt).find(".answercontainerwrapper .answercontainer .ans-set .option-set .ansBlock").each(function(){
			
			ns.$(this).attr("quesnumber",cnt);
			//alert("here : " +ns.$(this).attr("quesnumber"));
			
		});
		
	
}

function reshuffleQuestionCount()
{
	var count1 = 1;
	ns.$(".questionandanswers .questionset .questioncontainer .h5peditor-label .h5peditor-label").each(function(){
		var count2 = count1;
		//ns.$(this).html("Question"+count1++);   
		ns.$(this).html(window.parent.Drupal.t('LBL325')+count1++);
		
		//alert("the classes are :: " +ns.$(this).parent().parent().parent().attr("class"));
		ns.$(this).parent().parent().parent().attr('class', 'questionset');
		ns.$(this).parent().parent().parent().attr('qset', "questionset"+count2);
		ns.$(this).parent().parent().parent().addClass("questionset"+count2);
		reshuffleOptionCount(count2,'fromQues');
	});
	
	
	
}



function addChoiceStatement(from,$params)
{ 
	var stCount = ns.$(".choices_container .choice_numcount").size();
	if(stCount == undefined)
		stCount = 1;
	else
		stCount = parseInt(stCount,10)+1;
	GLOBALPARAMS_H5P.choicecount = stCount;	
	if(from == 'addNew')
	{
		ns.$(".goToChoices").attr("addnew",1);
	}


	var html = new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/h5p/templates/GotoQuestion.ejs'}).render(GLOBALPARAMS_H5P);
	ns.$(".choices_container").append(html);   
}



function hideckebuttons(compName)
{
	ns.$(".cke_button__specialchar_icon").hide();
	ns.$(".cke_button__horizontalrule_icon").hide();
	if(compName != "Table")
		ns.$(".cke_button__table").hide();
	else
		ns.$(".cke_button__table").show();


}