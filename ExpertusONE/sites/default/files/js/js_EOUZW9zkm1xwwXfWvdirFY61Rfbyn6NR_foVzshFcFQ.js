function sleep(milliseconds) {
  var start = new Date().getTime();
  for (var i = 0; i < 1e7; i++) {
    if ((new Date().getTime() - start) > milliseconds){
      break;
    }
  }
}

(function ($) {

  Drupal.behaviors.exp_sp_session_timeout = {
    attach: function(context, settings) {

      if (context != document) {
        return;
      }

      var paddingTimer;
      var t;
      var theDialog;
      var localSettings;

      // Activity is a boolean used to detect a user has
      // interacted with the page.
      var activity;

      // Timer to keep track of activity resets.
      var activityResetTimer;

      // Prevent settings being overriden by ajax callbacks by cloning the settings.
      localSettings = jQuery.extend(true, {}, settings.exp_sp_session_timeout);

      if (localSettings.refresh_only) {
        // On pages that cannot be logged out of don't start the logout countdown.
        t = setTimeout(keepAlive, localSettings.timeout);
      }
      else {
        // Set no activity to start with.
        activity = false;

        // Bind formUpdated events to preventAutoLogout event.
        $('body').bind('formUpdated', function(event) {
          $(event.target).trigger('preventAutologout');
        });

        // Support for CKEditor.
        if (typeof CKEDITOR !== 'undefined') {
          CKEDITOR.on('instanceCreated', function(e) {
            e.editor.on('contentDom', function() {
              e.editor.document.on('keyup', function(event) {
                // Keyup event in ckeditor should prevent exp_sp_session_timeout.
                $(e.editor.element.$).trigger('preventAutologout');
              });
            });
          });
        }

        $('body').bind('preventAutologout', function(event) {
          // When the preventAutologout event fires
          // we set activity to true.
          activity = true;

          // Clear timer if one exists.
          clearTimeout(activityResetTimer);

          // Set a timer that goes off and resets this activity indicator
          // after a minute, otherwise sessions never timeout.
          activityResetTimer = setTimeout(function () {
            activity = false;
          }, 60000);
        });

        // On pages where the user can be logged out, set the timer to popup
        // and log them out.
        t = setTimeout(init, localSettings.timeout);
      }

      function init() {
        var noDialog = Drupal.settings.exp_sp_session_timeout.no_dialog;

        if (activity) {
          // The user has been active on the page.
          activity = false;
          refresh();
        }
        else {

          // The user has not been active, ask them if they want to stay logged in
          // and start the logout timer.
          paddingTimer = setTimeout(confirmLogout, localSettings.timeout_padding);

          // While the countdown timer is going, lookup the remaining time. If there
          // is more time remaining (i.e. a user is navigating in another tab), then
          // reset the timer for opening the dialog.
          Drupal.ajax['exp_sp_session_timeout.getTimeLeft'].exp_sp_session_timeoutGetTimeLeft(function(time) {
              if (time > 0) {
                clearTimeout(paddingTimer);
                t = setTimeout(init, time);
              }
              else {
                // Logout user right away without displaying a confirmation dialog.
                if (noDialog) {
                  logout();
                  return;
                }
                theDialog = dialog();
              }
          });
        }
      }

      function dialog() {
        var buttons = {};
        buttons[Drupal.t('Yes')] = function() {
          $(this).dialog("destroy");
          clearTimeout(paddingTimer);
          refresh();
        };

        buttons[Drupal.t('No')] = function() {
          $(this).dialog("destroy");
          logout();
        };
	   	
	   	
//        return $('<div id="exp_sp_session_timeout-confirm"> ' +  localSettings.message + '</div>').dialog({
//            modal: true,
//                 closeOnEscape: false,
//                 width: "auto",
//                 dialogClass: 'exp_sp_session_timeout-dialog',
//                 title: localSettings.title,
//                 buttons: buttons,
//                 close: function(event, ui) {
//                   logout();
//                 }
//          });
        
        return $('<div id="exp_sp_session_timeout-confirm"> ' +  localSettings.message + '</div>').dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL286"),//"title",
	        buttons:buttons,
	        dialogClass: 'exp_sp_session_timeout-dialog',
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         },
	         close: function(event, ui) {
               logout();
             }
	    });
        
      }

      // A user could have used the reset button on the tab/window they're actively
      // using, so we need to double check before actually logging out.
      function confirmLogout() {
        $(theDialog).dialog('destroy');

        Drupal.ajax['exp_sp_session_timeout.getTimeLeft'].exp_sp_session_timeoutGetTimeLeft(function(time) {
          if (time > 0) {
            t = setTimeout(init, time);
          }
          else {
            logout();
          }
        });
      }

      function logout() {
        if (localSettings.use_alt_logout_method) {
          window.location = Drupal.settings.basePath + "?q=exp_sp_session_timeout_ahah_logout/alt";
        }
        else {// reset the session for salesforce always. dont let the system to throw the user out of session
        	var fromSafesforce = localSettings.redirect_url.indexOf("canvas");
        	//var sfLoginCatalog = localSettings.redirect_url.indexOf("canvas/catalog");
        	//var sfLoginMyLearning = localSettings.redirect_url.indexOf("canvas/mylearning");
        	var sfWidget = localSettings.redirect_url.indexOf("salesforce/widget");
    	  if((sfWidget != -1) || (fromSafesforce != -1)){
    		  clearTimeout(paddingTimer);
              refresh();
    	  }else {
          $.ajax({
            url: Drupal.settings.basePath + "?q=exp_sp_session_timeout_ahah_logout",
            type: "POST",
            success: function() {
              window.location = localSettings.redirect_url;
            },
            error: function(XMLHttpRequest, textStatus) {
              if (XMLHttpRequest.status == 403 || XMLHttpRequest.status == 404) {
                window.location = localSettings.redirect_url;
              }
            }
          });
         }
        }
      }

      /**
       * Use the Drupal ajax library to handle get time remaining events
       * because if using the JS Timer, the return will update it.
       *
       * @param function callback(time)
       *   The function to run when ajax is successful. The time parameter
       *   is the time remaining for the current user in ms.
       */
      Drupal.ajax.prototype.exp_sp_session_timeoutGetTimeLeft = function(callback) {
        var ajax = this;

        if (ajax.ajaxing) {
          return false;
        }

        ajax.options.success = function (response, status) {
          if (typeof response == 'string') {
            response = $.parseJSON(response);
          }

          if (typeof response[1].command === 'string' && response[1].command == 'alert') {
            // In the event of an error, we can assume
            // the user has been logged out.
            window.location = localSettings.redirect_url;
          }
		  if(response[2].settings != null) {
            callback(response[2].settings.time);
		  } else {
		    callback(response[4].settings.time);
		  }

          // Let Drupal.ajax handle the JSON response.
          return ajax.success(response, status);
        };

        try {
          ajax.beforeSerialize(ajax.element, ajax.options);
          $.ajax(ajax.options);
          $('body.progress-disabled').removeClass('progress-disabled');
        }
        catch (e) {
          ajax.ajaxing = false;
        }
      };

      Drupal.ajax['exp_sp_session_timeout.getTimeLeft'] = new Drupal.ajax(null, $(document.body), {
        url: Drupal.settings.basePath  + '?q=exp_sp_session_timeout_ajax_get_time_left',
        event: 'exp_sp_session_timeout.getTimeLeft',
        error: function(XMLHttpRequest, textStatus) {
          // Disable error reporting to the screen.
        }
      });

      /**
       * Use the Drupal ajax library to handle refresh events
       * because if using the JS Timer, the return will update
       * it.
       *
       * @param function timerFunction
       *   The function to tell the timer to run after its been
       *   restarted.
       */
      Drupal.ajax.prototype.exp_sp_session_timeoutRefresh = function(timerfunction) {
        var ajax = this;
        if (ajax.ajaxing) {
          return false;
        }

        ajax.options.success = function (response, status) {
          if (typeof response == 'string') {
            response = $.parseJSON(response);
          }

          if (typeof response[1].command === 'string' && response[1].command == 'alert') {
            // In the event of an error, we can assume
            // the user has been logged out.
            window.location = localSettings.redirect_url;
          }

          t = setTimeout(timerfunction, localSettings.timeout);
          activity = false;

          // Let Drupal.ajax handle the JSON response.
          return ajax.success(response, status);
        };

        try {
          ajax.beforeSerialize(ajax.element, ajax.options);
          $.ajax(ajax.options);
          $('body.progress-disabled').removeClass('progress-disabled'); 
        }
        catch (e) {
          ajax.ajaxing = false;
        }
      };

      Drupal.ajax['exp_sp_session_timeout.refresh'] = new Drupal.ajax(null, $(document.body), {
        url: Drupal.settings.basePath  + '?q=exp_sp_session_timeout_ahah_set_last',
        event: 'exp_sp_session_timeout.refresh',
        error: function(XMLHttpRequest, textStatus) {
          // Disable error reporting to the screen.
        }
      });

      function keepAlive() {
        Drupal.ajax['exp_sp_session_timeout.refresh'].exp_sp_session_timeoutRefresh(keepAlive);
      }

      function refresh() {
        Drupal.ajax['exp_sp_session_timeout.refresh'].exp_sp_session_timeoutRefresh(init);
      }

      // Check if the page was loaded via a back button click.
      var $dirty_bit = $('#exp_sp_session_timeout-cache-check-bit');
      if ($dirty_bit.length !== 0) {

    	  // Additional condition added by Vincent on 13 Apr, 2017 for #0073228
        if ($dirty_bit.val() == '1' && $('#timer').size() > 0) {
          // Page was loaded via a back button click, we should
          // refresh the timer.
          refresh();
        }

        $dirty_bit.val('1');
      }
    }
  };
})(jQuery);
;
/**
 * Intro.js v2.5.0
 * https://github.com/usablica/intro.js
 *
 * Copyright (C) 2016 Afshin Mehrabani (@afshinmeh)
 */

(function (root, factory) {
  if (typeof exports === 'object') {
    // CommonJS
    factory(exports);
  } else if (typeof define === 'function' && define.amd) {
    // AMD. Register as an anonymous module.
    define(['exports'], factory);
  } else {
    // Browser globals
    factory(root);
  }
} (this, function (exports) {
  //Default config/variables
  var VERSION = '2.5.0';

  /**
   * IntroJs main class
   *
   * @class IntroJs
   */
  function IntroJs(obj) {
    this._targetElement = obj;
    this._introItems = [];

    this._options = {
      /* Next button label in tooltip box */
      nextLabel: 'Next &rarr;',
      /* Previous button label in tooltip box */
      prevLabel: '&larr; Back',
      /* Skip button label in tooltip box */
      skipLabel: 'Skip',
      /* Done button label in tooltip box */
      doneLabel: 'Done',
      /* Hide previous button in the first step? Otherwise, it will be disabled button. */
      hidePrev: false,
      /* Hide next button in the last step? Otherwise, it will be disabled button. */
      hideNext: false,
      /* Default tooltip box position */
      tooltipPosition: 'bottom',
      /* Next CSS class for tooltip boxes */
      tooltipClass: '',
      /* CSS class that is added to the helperLayer */
      highlightClass: '',
      /* Close introduction when pressing Escape button? */
      exitOnEsc: true,
      /* Close introduction when clicking on overlay layer? */
      exitOnOverlayClick: true,
      /* Show step numbers in introduction? */
      showStepNumbers: true,
      /* Let user use keyboard to navigate the tour? */
      keyboardNavigation: true,
      /* Show tour control buttons? */
      showButtons: true,
      /* Show tour bullets? */
      showBullets: true,
      /* Show tour progress? */
      showProgress: false,
      /* Scroll to highlighted element? */
      scrollToElement: true,
      /* Set the overlay opacity */
      overlayOpacity: 0.8,
      /* Padding to add after scrolling when element is not in the viewport (in pixels) */
      scrollPadding: 30,
      /* Precedence of positions, when auto is enabled */
      positionPrecedence: ["bottom", "top", "right", "left"],
      /* Disable an interaction with element? */
      disableInteraction: false,
      /* Default hint position */
      hintPosition: 'top-middle',
      /* Hint button label */
      hintButtonLabel: 'Got it',
      /* Adding animation to hints? */
      hintAnimation: true
    };
  }

  /**
   * Initiate a new introduction/guide from an element in the page
   *
   * @api private
   * @method _introForElement
   * @param {Object} targetElm
   * @returns {Boolean} Success or not?
   */
  function _introForElement(targetElm) {
    var introItems = [],
        self = this;

    if (this._options.steps) {
      //use steps passed programmatically
      for (var i = 0, stepsLength = this._options.steps.length; i < stepsLength; i++) {
        var currentItem = _cloneObject(this._options.steps[i]);
        //set the step
        currentItem.step = introItems.length + 1;
        //use querySelector function only when developer used CSS selector
        if (typeof(currentItem.element) === 'string') {
          //grab the element with given selector from the page
          currentItem.element = document.querySelector(currentItem.element);
        }

        //intro without element
        if (typeof(currentItem.element) === 'undefined' || currentItem.element == null) {
          var floatingElementQuery = document.querySelector(".introjsFloatingElement");

          if (floatingElementQuery == null) {
            floatingElementQuery = document.createElement('div');
            floatingElementQuery.className = 'introjsFloatingElement';

            document.body.appendChild(floatingElementQuery);
          }

          currentItem.element  = floatingElementQuery;
          currentItem.position = 'floating';
        }

        if (currentItem.element != null) {
          introItems.push(currentItem);
        }
      }

    } else {
      //use steps from data-* annotations
      var allIntroSteps = targetElm.querySelectorAll('*[data-intro]');
      //if there's no element to intro
      if (allIntroSteps.length < 1) {
        return false;
      }

      //first add intro items with data-step
      for (var i = 0, elmsLength = allIntroSteps.length; i < elmsLength; i++) {
        var currentElement = allIntroSteps[i];

        // skip hidden elements
        if (currentElement.style.display == 'none') {
          continue;
        }

        var step = parseInt(currentElement.getAttribute('data-step'), 10);

        if (step > 0) {
          introItems[step - 1] = {
            element: currentElement,
            intro: currentElement.getAttribute('data-intro'),
            step: parseInt(currentElement.getAttribute('data-step'), 10),
            tooltipClass: currentElement.getAttribute('data-tooltipClass'),
            highlightClass: currentElement.getAttribute('data-highlightClass'),
            position: currentElement.getAttribute('data-position') || this._options.tooltipPosition
          };
        }
      }

      //next add intro items without data-step
      //todo: we need a cleanup here, two loops are redundant
      var nextStep = 0;
      for (var i = 0, elmsLength = allIntroSteps.length; i < elmsLength; i++) {
        var currentElement = allIntroSteps[i];

        if (currentElement.getAttribute('data-step') == null) {

          while (true) {
            if (typeof introItems[nextStep] == 'undefined') {
              break;
            } else {
              nextStep++;
            }
          }

          introItems[nextStep] = {
            element: currentElement,
            intro: currentElement.getAttribute('data-intro'),
            step: nextStep + 1,
            tooltipClass: currentElement.getAttribute('data-tooltipClass'),
            highlightClass: currentElement.getAttribute('data-highlightClass'),
            position: currentElement.getAttribute('data-position') || this._options.tooltipPosition
          };
        }
      }
    }

    //removing undefined/null elements
    var tempIntroItems = [];
    for (var z = 0; z < introItems.length; z++) {
      introItems[z] && tempIntroItems.push(introItems[z]);  // copy non-empty values to the end of the array
    }

    introItems = tempIntroItems;

    //Ok, sort all items with given steps
    introItems.sort(function (a, b) {
      return a.step - b.step;
    });

    //set it to the introJs object
    self._introItems = introItems;

    //add overlay layer to the page
    if(_addOverlayLayer.call(self, targetElm)) {
      //then, start the show
      _nextStep.call(self);

      var skipButton     = targetElm.querySelector('.introjs-skipbutton'),
          nextStepButton = targetElm.querySelector('.introjs-nextbutton');

      self._onKeyDown = function(e) {
        if (e.keyCode === 27 && self._options.exitOnEsc == true) {
          //escape key pressed, exit the intro
          //check if exit callback is defined
          _exitIntro.call(self, targetElm);
        } else if(e.keyCode === 37) {
          //left arrow
          _previousStep.call(self);
        } else if (e.keyCode === 39) {
          //right arrow
          _nextStep.call(self);
        } else if (e.keyCode === 13) {
          //srcElement === ie
          var target = e.target || e.srcElement;
          if (target && target.className.indexOf('introjs-prevbutton') > 0) {
            //user hit enter while focusing on previous button
            _previousStep.call(self);
          } else if (target && target.className.indexOf('introjs-skipbutton') > 0) {
            //user hit enter while focusing on skip button
            if (self._introItems.length - 1 == self._currentStep && typeof (self._introCompleteCallback) === 'function') {
                self._introCompleteCallback.call(self);
            }

            _exitIntro.call(self, targetElm);
          } else {
            //default behavior for responding to enter
            _nextStep.call(self);
          }

          //prevent default behaviour on hitting Enter, to prevent steps being skipped in some browsers
          if(e.preventDefault) {
            e.preventDefault();
          } else {
            e.returnValue = false;
          }
        }
      };

      self._onResize = function(e) {
        _setHelperLayerPosition.call(self, document.querySelector('.introjs-helperLayer'));
        _setHelperLayerPosition.call(self, document.querySelector('.introjs-tooltipReferenceLayer'));
      };

      if (window.addEventListener) {
        if (this._options.keyboardNavigation) {
          window.addEventListener('keydown', self._onKeyDown, true);
        }
        //for window resize
        window.addEventListener('resize', self._onResize, true);
      } else if (document.attachEvent) { //IE
        if (this._options.keyboardNavigation) {
          document.attachEvent('onkeydown', self._onKeyDown);
        }
        //for window resize
        document.attachEvent('onresize', self._onResize);
      }
    }
    return false;
  }

 /*
   * makes a copy of the object
   * @api private
   * @method _cloneObject
  */
  function _cloneObject(object) {
      if (object == null || typeof (object) != 'object' || typeof (object.nodeType) != 'undefined') {
        return object;
      }
      var temp = {};
      for (var key in object) {
        if (typeof (jQuery) != 'undefined' && object[key] instanceof jQuery) {
          temp[key] = object[key];
        } else {
          temp[key] = _cloneObject(object[key]);
        }
      }
      return temp;
  }
  /**
   * Go to specific step of introduction
   *
   * @api private
   * @method _goToStep
   */
  function _goToStep(step) {
    //because steps starts with zero
    this._currentStep = step - 2;
    if (typeof (this._introItems) !== 'undefined') {
      _nextStep.call(this);
    }
  }

  /**
   * Go to the specific step of introduction with the explicit [data-step] number
   *
   * @api private
   * @method _goToStepNumber
   */
  function _goToStepNumber(step) {
    this._currentStepNumber = step;
    if (typeof (this._introItems) !== 'undefined') {
      _nextStep.call(this);
    }
  }

  /**
   * Go to next step on intro
   *
   * @api private
   * @method _nextStep
   */
  function _nextStep() {
    this._direction = 'forward';

    if (typeof (this._currentStepNumber) !== 'undefined') {
        for( var i = 0, len = this._introItems.length; i < len; i++ ) {
            var item = this._introItems[i];
            if( item.step === this._currentStepNumber ) {
                this._currentStep = i - 1;
                this._currentStepNumber = undefined;
            }
        }
    }

    if (typeof (this._currentStep) === 'undefined') {
      this._currentStep = 0;
    } else {
      ++this._currentStep;
    }

    if ((this._introItems.length) <= this._currentStep) {
      //end of the intro
      //check if any callback is defined
      if (typeof (this._introCompleteCallback) === 'function') {
        this._introCompleteCallback.call(this);
      }
      _exitIntro.call(this, this._targetElement);
      return;
    }

    var nextStep = this._introItems[this._currentStep];
    if (typeof (this._introBeforeChangeCallback) !== 'undefined') {
      this._introBeforeChangeCallback.call(this, nextStep.element);
    }

    _showElement.call(this, nextStep);
  }

  /**
   * Go to previous step on intro
   *
   * @api private
   * @method _previousStep
   */
  function _previousStep() {
    this._direction = 'backward';

    if (this._currentStep === 0) {
      return false;
    }

    var nextStep = this._introItems[--this._currentStep];
    if (typeof (this._introBeforeChangeCallback) !== 'undefined') {
      this._introBeforeChangeCallback.call(this, nextStep.element);
    }

    _showElement.call(this, nextStep);
  }

  /**
   * Exit from intro
   *
   * @api private
   * @method _exitIntro
   * @param {Object} targetElement
   */
  function _exitIntro(targetElement) {
    //remove overlay layers from the page
    var overlayLayers = targetElement.querySelectorAll('.introjs-overlay');

    if (overlayLayers && overlayLayers.length > 0) {
      for (var i = overlayLayers.length - 1; i >= 0; i--) {
        //for fade-out animation
        var overlayLayer = overlayLayers[i];
        overlayLayer.style.opacity = 0;
        setTimeout(function () {
          if (this.parentNode) {
            this.parentNode.removeChild(this);
          }
        }.bind(overlayLayer), 500);
      };
    }

    //remove all helper layers
    var helperLayer = targetElement.querySelector('.introjs-helperLayer');
    if (helperLayer) {
      helperLayer.parentNode.removeChild(helperLayer);
    }

    var referenceLayer = targetElement.querySelector('.introjs-tooltipReferenceLayer');
    if (referenceLayer) {
      referenceLayer.parentNode.removeChild(referenceLayer);
    }
    //remove disableInteractionLayer
    var disableInteractionLayer = targetElement.querySelector('.introjs-disableInteraction');
    if (disableInteractionLayer) {
      disableInteractionLayer.parentNode.removeChild(disableInteractionLayer);
    }

    //remove intro floating element
    var floatingElement = document.querySelector('.introjsFloatingElement');
    if (floatingElement) {
      floatingElement.parentNode.removeChild(floatingElement);
    }

    _removeShowElement();

    //remove `introjs-fixParent` class from the elements
    var fixParents = document.querySelectorAll('.introjs-fixParent');
    if (fixParents && fixParents.length > 0) {
      for (var i = fixParents.length - 1; i >= 0; i--) {
        fixParents[i].className = fixParents[i].className.replace(/introjs-fixParent/g, '').replace(/^\s+|\s+$/g, '');
      }
    }

    //clean listeners
    if (window.removeEventListener) {
      window.removeEventListener('keydown', this._onKeyDown, true);
    } else if (document.detachEvent) { //IE
      document.detachEvent('onkeydown', this._onKeyDown);
    }

    //check if any callback is defined
    if (this._introExitCallback != undefined) {
      this._introExitCallback.call(self);
    }

    //set the step to zero
    this._currentStep = undefined;
  }

  /**
   * Render tooltip box in the page
   *
   * @api private
   * @method _placeTooltip
   * @param {HTMLElement} targetElement
   * @param {HTMLElement} tooltipLayer
   * @param {HTMLElement} arrowLayer
   * @param {HTMLElement} helperNumberLayer
   * @param {Boolean} hintMode
   */
  function _placeTooltip(targetElement, tooltipLayer, arrowLayer, helperNumberLayer, hintMode) {
    var tooltipCssClass = '',
        currentStepObj,
        tooltipOffset,
        targetOffset,
        windowSize,
        currentTooltipPosition;

    hintMode = hintMode || false;

    //reset the old style
    tooltipLayer.style.top        = null;
    tooltipLayer.style.right      = null;
    tooltipLayer.style.bottom     = null;
    tooltipLayer.style.left       = null;
    tooltipLayer.style.marginLeft = null;
    tooltipLayer.style.marginTop  = null;

    arrowLayer.style.display = 'inherit';

    if (typeof(helperNumberLayer) != 'undefined' && helperNumberLayer != null) {
      helperNumberLayer.style.top  = null;
      helperNumberLayer.style.left = null;
    }

    //prevent error when `this._currentStep` is undefined
    if (!this._introItems[this._currentStep]) return;

    //if we have a custom css class for each step
    currentStepObj = this._introItems[this._currentStep];
    if (typeof (currentStepObj.tooltipClass) === 'string') {
      tooltipCssClass = currentStepObj.tooltipClass;
    } else {
      tooltipCssClass = this._options.tooltipClass;
    }

    tooltipLayer.className = ('introjs-tooltip ' + tooltipCssClass).replace(/^\s+|\s+$/g, '');

    currentTooltipPosition = this._introItems[this._currentStep].position;
    if ((currentTooltipPosition == "auto" || this._options.tooltipPosition == "auto")) {
      if (currentTooltipPosition != "floating") { // Floating is always valid, no point in calculating
        currentTooltipPosition = _determineAutoPosition.call(this, targetElement, tooltipLayer, currentTooltipPosition);
      }
    }
    targetOffset  = _getOffset(targetElement);
    tooltipOffset = _getOffset(tooltipLayer);
    windowSize    = _getWinSize();

    switch (currentTooltipPosition) {
      case 'top':
        arrowLayer.className = 'introjs-arrow bottom';

        if (hintMode) {
          var tooltipLayerStyleLeft = 0;
        } else {
          var tooltipLayerStyleLeft = 15;
        }

        _checkRight(targetOffset, tooltipLayerStyleLeft, tooltipOffset, windowSize, tooltipLayer);
        tooltipLayer.style.bottom = (targetOffset.height +  20) + 'px';
        break;
      case 'right':
        tooltipLayer.style.left = (targetOffset.width + 20) + 'px';
        if (targetOffset.top + tooltipOffset.height > windowSize.height) {
          // In this case, right would have fallen below the bottom of the screen.
          // Modify so that the bottom of the tooltip connects with the target
          arrowLayer.className = "introjs-arrow left-bottom";
          tooltipLayer.style.top = "-" + (tooltipOffset.height - targetOffset.height - 20) + "px";
        } else {
          arrowLayer.className = 'introjs-arrow left';
        }
        break;
      case 'left':
        if (!hintMode && this._options.showStepNumbers == true) {
          tooltipLayer.style.top = '15px';
        }

        if (targetOffset.top + tooltipOffset.height > windowSize.height) {
          // In this case, left would have fallen below the bottom of the screen.
          // Modify so that the bottom of the tooltip connects with the target
          tooltipLayer.style.top = "-" + (tooltipOffset.height - targetOffset.height - 20) + "px";
          arrowLayer.className = 'introjs-arrow right-bottom';
        } else {
          arrowLayer.className = 'introjs-arrow right';
        }
        tooltipLayer.style.right = (targetOffset.width + 20) + 'px';

        break;
      case 'floating':
        arrowLayer.style.display = 'none';

        //we have to adjust the top and left of layer manually for intro items without element
        tooltipLayer.style.left   = '50%';
        tooltipLayer.style.top    = '50%';
        tooltipLayer.style.marginLeft = '-' + (tooltipOffset.width / 2)  + 'px';
        tooltipLayer.style.marginTop  = '-' + (tooltipOffset.height / 2) + 'px';

        if (typeof(helperNumberLayer) != 'undefined' && helperNumberLayer != null) {
          helperNumberLayer.style.left = '-' + ((tooltipOffset.width / 2) + 18) + 'px';
          helperNumberLayer.style.top  = '-' + ((tooltipOffset.height / 2) + 18) + 'px';
        }

        break;
      case 'bottom-right-aligned':
        arrowLayer.className      = 'introjs-arrow top-right';

        var tooltipLayerStyleRight = 0;
        _checkLeft(targetOffset, tooltipLayerStyleRight, tooltipOffset, tooltipLayer);
        tooltipLayer.style.top    = (targetOffset.height +  20) + 'px';
        break;

      case 'bottom-middle-aligned':
        arrowLayer.className      = 'introjs-arrow top-middle';

        var tooltipLayerStyleLeftRight = targetOffset.width / 2 - tooltipOffset.width / 2;

        // a fix for middle aligned hints
        if (hintMode) {
          tooltipLayerStyleLeftRight += 5;
        }

        if (_checkLeft(targetOffset, tooltipLayerStyleLeftRight, tooltipOffset, tooltipLayer)) {
          tooltipLayer.style.right = null;
          _checkRight(targetOffset, tooltipLayerStyleLeftRight, tooltipOffset, windowSize, tooltipLayer);
        }
        tooltipLayer.style.top = (targetOffset.height + 20) + 'px';
        break;

      case 'bottom-left-aligned':
      // Bottom-left-aligned is the same as the default bottom
      case 'bottom':
      // Bottom going to follow the default behavior
      default:
        arrowLayer.className = 'introjs-arrow top';

        var tooltipLayerStyleLeft = 0;
        _checkRight(targetOffset, tooltipLayerStyleLeft, tooltipOffset, windowSize, tooltipLayer);
        tooltipLayer.style.top    = (targetOffset.height +  20) + 'px';
        break;
    }
  }

  /**
   * Set tooltip left so it doesn't go off the right side of the window
   *
   * @return boolean true, if tooltipLayerStyleLeft is ok.  false, otherwise.
   */
  function _checkRight(targetOffset, tooltipLayerStyleLeft, tooltipOffset, windowSize, tooltipLayer) {
    if (targetOffset.left + tooltipLayerStyleLeft + tooltipOffset.width > windowSize.width) {
      // off the right side of the window
      tooltipLayer.style.left = (windowSize.width - tooltipOffset.width - targetOffset.left) + 'px';
      return false;
    }
    tooltipLayer.style.left = tooltipLayerStyleLeft + 'px';
    return true;
  }

  /**
   * Set tooltip right so it doesn't go off the left side of the window
   *
   * @return boolean true, if tooltipLayerStyleRight is ok.  false, otherwise.
   */
  function _checkLeft(targetOffset, tooltipLayerStyleRight, tooltipOffset, tooltipLayer) {
    if (targetOffset.left + targetOffset.width - tooltipLayerStyleRight - tooltipOffset.width < 0) {
      // off the left side of the window
      tooltipLayer.style.left = (-targetOffset.left) + 'px';
      return false;
    }
    tooltipLayer.style.right = tooltipLayerStyleRight + 'px';
    return true;
  }

  /**
   * Determines the position of the tooltip based on the position precedence and availability
   * of screen space.
   *
   * @param {Object} targetElement
   * @param {Object} tooltipLayer
   * @param {Object} desiredTooltipPosition
   *
   */
  function _determineAutoPosition(targetElement, tooltipLayer, desiredTooltipPosition) {

    // Take a clone of position precedence. These will be the available
    var possiblePositions = this._options.positionPrecedence.slice();

    var windowSize = _getWinSize();
    var tooltipHeight = _getOffset(tooltipLayer).height + 10;
    var tooltipWidth = _getOffset(tooltipLayer).width + 20;
    var targetOffset = _getOffset(targetElement);

    // If we check all the possible areas, and there are no valid places for the tooltip, the element
    // must take up most of the screen real estate. Show the tooltip floating in the middle of the screen.
    var calculatedPosition = "floating";

    // Check if the width of the tooltip + the starting point would spill off the right side of the screen
    // If no, neither bottom or top are valid
    if (targetOffset.left + tooltipWidth > windowSize.width || ((targetOffset.left + (targetOffset.width / 2)) - tooltipWidth) < 0) {
      _removeEntry(possiblePositions, "bottom");
      _removeEntry(possiblePositions, "top");
    } else {
      // Check for space below
      if ((targetOffset.height + targetOffset.top + tooltipHeight) > windowSize.height) {
        _removeEntry(possiblePositions, "bottom");
      }

      // Check for space above
      if (targetOffset.top - tooltipHeight < 0) {
        _removeEntry(possiblePositions, "top");
      }
    }

    // Check for space to the right
    if (targetOffset.width + targetOffset.left + tooltipWidth > windowSize.width) {
      _removeEntry(possiblePositions, "right");
    }

    // Check for space to the left
    if (targetOffset.left - tooltipWidth < 0) {
      _removeEntry(possiblePositions, "left");
    }

    // At this point, our array only has positions that are valid. Pick the first one, as it remains in order
    if (possiblePositions.length > 0) {
      calculatedPosition = possiblePositions[0];
    }

    // If the requested position is in the list, replace our calculated choice with that
    if (desiredTooltipPosition && desiredTooltipPosition != "auto") {
      if (possiblePositions.indexOf(desiredTooltipPosition) > -1) {
        calculatedPosition = desiredTooltipPosition;
      }
    }

    return calculatedPosition;
  }

  /**
   * Remove an entry from a string array if it's there, does nothing if it isn't there.
   *
   * @param {Array} stringArray
   * @param {String} stringToRemove
   */
  function _removeEntry(stringArray, stringToRemove) {
    if (stringArray.indexOf(stringToRemove) > -1) {
      stringArray.splice(stringArray.indexOf(stringToRemove), 1);
    }
  }

  /**
   * Update the position of the helper layer on the screen
   *
   * @api private
   * @method _setHelperLayerPosition
   * @param {Object} helperLayer
   */
  function _setHelperLayerPosition(helperLayer) {
    if (helperLayer) {
      //prevent error when `this._currentStep` in undefined
      if (!this._introItems[this._currentStep]) return;

      var currentElement  = this._introItems[this._currentStep],
          elementPosition = _getOffset(currentElement.element),
          widthHeightPadding = 10;

      // If the target element is fixed, the tooltip should be fixed as well.
      // Otherwise, remove a fixed class that may be left over from the previous
      // step.
      if (_isFixed(currentElement.element)) {
        helperLayer.className += ' introjs-fixedTooltip';
      } else {
        helperLayer.className = helperLayer.className.replace(' introjs-fixedTooltip', '');
      }

      if (currentElement.position == 'floating') {
        widthHeightPadding = 0;
      }

      //set new position to helper layer
      helperLayer.setAttribute('style', 'width: ' + (elementPosition.width  + widthHeightPadding)  + 'px; ' +
                                        'height:' + (elementPosition.height + widthHeightPadding)  + 'px; ' +
                                        'top:'    + (elementPosition.top    - 5)   + 'px;' +
                                        'left: '  + (elementPosition.left   - 5)   + 'px;');

    }
  }

  /**
   * Add disableinteraction layer and adjust the size and position of the layer
   *
   * @api private
   * @method _disableInteraction
   */
  function _disableInteraction() {
    var disableInteractionLayer = document.querySelector('.introjs-disableInteraction');
    if (disableInteractionLayer === null) {
      disableInteractionLayer = document.createElement('div');
      disableInteractionLayer.className = 'introjs-disableInteraction';
      this._targetElement.appendChild(disableInteractionLayer);
    }

    _setHelperLayerPosition.call(this, disableInteractionLayer);
  }

  /**
   * Setting anchors to behave like buttons
   *
   * @api private
   * @method _setAnchorAsButton
   */
  function _setAnchorAsButton(anchor){
    anchor.setAttribute('role', 'button');
    anchor.tabIndex = 0;
  }

  /**
   * Show an element on the page
   *
   * @api private
   * @method _showElement
   * @param {Object} targetElement
   */
  function _showElement(targetElement) {
    if (typeof (this._introChangeCallback) !== 'undefined') {
      this._introChangeCallback.call(this, targetElement.element);
    }

    var self = this,
        oldHelperLayer = document.querySelector('.introjs-helperLayer'),
        oldReferenceLayer = document.querySelector('.introjs-tooltipReferenceLayer'),
        highlightClass = 'introjs-helperLayer',
        elementPosition = _getOffset(targetElement.element);

    //check for a current step highlight class
    if (typeof (targetElement.highlightClass) === 'string') {
      highlightClass += (' ' + targetElement.highlightClass);
    }
    //check for options highlight class
    if (typeof (this._options.highlightClass) === 'string') {
      highlightClass += (' ' + this._options.highlightClass);
    }

    if (oldHelperLayer != null) {
      var oldHelperNumberLayer = oldReferenceLayer.querySelector('.introjs-helperNumberLayer'),
          oldtooltipLayer      = oldReferenceLayer.querySelector('.introjs-tooltiptext'),
          oldArrowLayer        = oldReferenceLayer.querySelector('.introjs-arrow'),
          oldtooltipContainer  = oldReferenceLayer.querySelector('.introjs-tooltip'),
          skipTooltipButton    = oldReferenceLayer.querySelector('.introjs-skipbutton'),
          prevTooltipButton    = oldReferenceLayer.querySelector('.introjs-prevbutton'),
          nextTooltipButton    = oldReferenceLayer.querySelector('.introjs-nextbutton');

      //update or reset the helper highlight class
      oldHelperLayer.className = highlightClass;
      //hide the tooltip
      oldtooltipContainer.style.opacity = 0;
      oldtooltipContainer.style.display = "none";

      if (oldHelperNumberLayer != null) {
        var lastIntroItem = this._introItems[(targetElement.step - 2 >= 0 ? targetElement.step - 2 : 0)];

        if (lastIntroItem != null && (this._direction == 'forward' && lastIntroItem.position == 'floating') || (this._direction == 'backward' && targetElement.position == 'floating')) {
          oldHelperNumberLayer.style.opacity = 0;
        }
      }

      //set new position to helper layer
      _setHelperLayerPosition.call(self, oldHelperLayer);
      _setHelperLayerPosition.call(self, oldReferenceLayer);

      //remove `introjs-fixParent` class from the elements
      var fixParents = document.querySelectorAll('.introjs-fixParent');
      if (fixParents && fixParents.length > 0) {
        for (var i = fixParents.length - 1; i >= 0; i--) {
          fixParents[i].className = fixParents[i].className.replace(/introjs-fixParent/g, '').replace(/^\s+|\s+$/g, '');
        };
      }

      //remove old classes if the element still exist
      _removeShowElement();

      //we should wait until the CSS3 transition is competed (it's 0.3 sec) to prevent incorrect `height` and `width` calculation
      if (self._lastShowElementTimer) {
        clearTimeout(self._lastShowElementTimer);
      }
      self._lastShowElementTimer = setTimeout(function() {
        //set current step to the label
        if (oldHelperNumberLayer != null) {
          oldHelperNumberLayer.innerHTML = targetElement.step;
        }
        //set current tooltip text
        oldtooltipLayer.innerHTML = targetElement.intro;
        //set the tooltip position
        oldtooltipContainer.style.display = "block";
        _placeTooltip.call(self, targetElement.element, oldtooltipContainer, oldArrowLayer, oldHelperNumberLayer);

        //change active bullet
        if (self._options.showBullets) {
            oldReferenceLayer.querySelector('.introjs-bullets li > a.active').className = '';
            oldReferenceLayer.querySelector('.introjs-bullets li > a[data-stepnumber="' + targetElement.step + '"]').className = 'active';
        }
        oldReferenceLayer.querySelector('.introjs-progress .introjs-progressbar').setAttribute('style', 'width:' + _getProgress.call(self) + '%;');

        //show the tooltip
        oldtooltipContainer.style.opacity = 1;
        if (oldHelperNumberLayer) oldHelperNumberLayer.style.opacity = 1;

        //reset button focus
        if (nextTooltipButton.tabIndex === -1) {
          //tabindex of -1 means we are at the end of the tour - focus on skip / done
          skipTooltipButton.focus();
        } else {
          //still in the tour, focus on next
          nextTooltipButton.focus();
        }
      }, 350);

      // end of old element if-else condition
    } else {
      var helperLayer       = document.createElement('div'),
          referenceLayer    = document.createElement('div'),
          arrowLayer        = document.createElement('div'),
          tooltipLayer      = document.createElement('div'),
          tooltipTextLayer  = document.createElement('div'),
          bulletsLayer      = document.createElement('div'),
          progressLayer     = document.createElement('div'),
          buttonsLayer      = document.createElement('div');

      helperLayer.className = highlightClass;
      referenceLayer.className = 'introjs-tooltipReferenceLayer';

      //set new position to helper layer
      _setHelperLayerPosition.call(self, helperLayer);
      _setHelperLayerPosition.call(self, referenceLayer);

      //add helper layer to target element
      this._targetElement.appendChild(helperLayer);
      this._targetElement.appendChild(referenceLayer);

      arrowLayer.className = 'introjs-arrow';

      tooltipTextLayer.className = 'introjs-tooltiptext';
      tooltipTextLayer.innerHTML = targetElement.intro;

      bulletsLayer.className = 'introjs-bullets';

      if (this._options.showBullets === false) {
        bulletsLayer.style.display = 'none';
      }

      var ulContainer = document.createElement('ul');

      for (var i = 0, stepsLength = this._introItems.length; i < stepsLength; i++) {
        var innerLi    = document.createElement('li');
        var anchorLink = document.createElement('a');

        anchorLink.onclick = function() {
          self.goToStep(this.getAttribute('data-stepnumber'));
        };

        if (i === (targetElement.step-1)) anchorLink.className = 'active';

        _setAnchorAsButton(anchorLink);
        anchorLink.innerHTML = "&nbsp;";
        anchorLink.setAttribute('data-stepnumber', this._introItems[i].step);

        innerLi.appendChild(anchorLink);
        ulContainer.appendChild(innerLi);
      }

      bulletsLayer.appendChild(ulContainer);

      progressLayer.className = 'introjs-progress';

      if (this._options.showProgress === false) {
        progressLayer.style.display = 'none';
      }
      var progressBar = document.createElement('div');
      progressBar.className = 'introjs-progressbar';
      progressBar.setAttribute('style', 'width:' + _getProgress.call(this) + '%;');

      progressLayer.appendChild(progressBar);

      buttonsLayer.className = 'introjs-tooltipbuttons';
      if (this._options.showButtons === false) {
        buttonsLayer.style.display = 'none';
      }

      tooltipLayer.className = 'introjs-tooltip';
      tooltipLayer.appendChild(tooltipTextLayer);
      tooltipLayer.appendChild(bulletsLayer);
      tooltipLayer.appendChild(progressLayer);

      //add helper layer number
      if (this._options.showStepNumbers == true) {
        var helperNumberLayer = document.createElement('span');
        helperNumberLayer.className = 'introjs-helperNumberLayer';
        helperNumberLayer.innerHTML = targetElement.step;
        referenceLayer.appendChild(helperNumberLayer);
      }

      tooltipLayer.appendChild(arrowLayer);
      referenceLayer.appendChild(tooltipLayer);

      //next button
      var nextTooltipButton = document.createElement('a');

      nextTooltipButton.onclick = function() {
        if (self._introItems.length - 1 != self._currentStep) {
          _nextStep.call(self);
        }
      };

      _setAnchorAsButton(nextTooltipButton);
      nextTooltipButton.innerHTML = this._options.nextLabel;

      //previous button
      var prevTooltipButton = document.createElement('a');

      prevTooltipButton.onclick = function() {
        if (self._currentStep != 0) {
          _previousStep.call(self);
        }
      };

      _setAnchorAsButton(prevTooltipButton);
      prevTooltipButton.innerHTML = this._options.prevLabel;

      //skip button
      var skipTooltipButton = document.createElement('a');
      skipTooltipButton.className = 'introjs-button introjs-skipbutton';
      _setAnchorAsButton(skipTooltipButton);
      skipTooltipButton.innerHTML = this._options.skipLabel;

      skipTooltipButton.onclick = function() {
        if (self._introItems.length - 1 == self._currentStep && typeof (self._introCompleteCallback) === 'function') {
          self._introCompleteCallback.call(self);
        }

        _exitIntro.call(self, self._targetElement);
      };

      buttonsLayer.appendChild(skipTooltipButton);

      //in order to prevent displaying next/previous button always
      if (this._introItems.length > 1) {
        buttonsLayer.appendChild(prevTooltipButton);
        buttonsLayer.appendChild(nextTooltipButton);
      }

      tooltipLayer.appendChild(buttonsLayer);

      //set proper position
      _placeTooltip.call(self, targetElement.element, tooltipLayer, arrowLayer, helperNumberLayer);

      //end of new element if-else condition
    }

    //disable interaction
    if (this._options.disableInteraction === true) {
      _disableInteraction.call(self);
    }

    prevTooltipButton.removeAttribute('tabIndex');
    nextTooltipButton.removeAttribute('tabIndex');

    // when it's the first step of tour
    if (this._currentStep == 0 && this._introItems.length > 1) {
      skipTooltipButton.className = 'introjs-button introjs-skipbutton';
      nextTooltipButton.className = 'introjs-button introjs-nextbutton';

      if (this._options.hidePrev == true) {
        prevTooltipButton.className = 'introjs-button introjs-prevbutton introjs-hidden';
        nextTooltipButton.className += ' introjs-fullbutton';
      } else {
        prevTooltipButton.className = 'introjs-button introjs-prevbutton introjs-disabled';
      }

      prevTooltipButton.tabIndex = '-1';
      skipTooltipButton.innerHTML = this._options.skipLabel;
    } else if (this._introItems.length - 1 == this._currentStep || this._introItems.length == 1) {
      // last step of tour
      skipTooltipButton.innerHTML = this._options.doneLabel;
      // adding donebutton class in addition to skipbutton
      skipTooltipButton.className += ' introjs-donebutton';
      prevTooltipButton.className = 'introjs-button introjs-prevbutton';

      if (this._options.hideNext == true) {
        nextTooltipButton.className = 'introjs-button introjs-nextbutton introjs-hidden';
        prevTooltipButton.className += ' introjs-fullbutton';
      } else {
        nextTooltipButton.className = 'introjs-button introjs-nextbutton introjs-disabled';
      }

      nextTooltipButton.tabIndex = '-1';
    } else {
      // steps between start and end
      skipTooltipButton.className = 'introjs-button introjs-skipbutton';
      prevTooltipButton.className = 'introjs-button introjs-prevbutton';
      nextTooltipButton.className = 'introjs-button introjs-nextbutton';
      skipTooltipButton.innerHTML = this._options.skipLabel;
    }

    //Set focus on "next" button, so that hitting Enter always moves you onto the next step
    nextTooltipButton.focus();

    _setShowElement(targetElement);

    if (!_elementInViewport(targetElement.element) && this._options.scrollToElement === true) {
      var rect = targetElement.element.getBoundingClientRect(),
        winHeight = _getWinSize().height,
        top = rect.bottom - (rect.bottom - rect.top),
        bottom = rect.bottom - winHeight;

      //Scroll up
      if (top < 0 || targetElement.element.clientHeight > winHeight) {
        window.scrollBy(0, top - this._options.scrollPadding); // 30px padding from edge to look nice

      //Scroll down
      } else {
        window.scrollBy(0, bottom + 70 + this._options.scrollPadding); // 70px + 30px padding from edge to look nice
      }
    }

    if (typeof (this._introAfterChangeCallback) !== 'undefined') {
      this._introAfterChangeCallback.call(this, targetElement.element);
    }
  }

  /**
   * To remove all show element(s)
   *
   * @api private
   * @method _removeShowElement
   */
  function _removeShowElement() {
    var elms = document.querySelectorAll('.introjs-showElement');

    for (var i = 0, l = elms.length; i < l; i++) {
      var elm = elms[i];
      _removeClass(elm, /introjs-[a-zA-Z]+/g);
    }
  }

  /**
   * To set the show element
   * This function set a relative (in most cases) position and changes the z-index
   *
   * @api private
   * @method _setShowElement
   * @param {Object} targetElement
   */
  function _setShowElement(targetElement) {
    // we need to add this show element class to the parent of SVG elements
    // because the SVG elements can't have independent z-index
    if (targetElement.element instanceof SVGElement) {
      var parentElm = targetElement.element.parentNode;

      while (targetElement.element.parentNode != null) {
        if (!parentElm.tagName || parentElm.tagName.toLowerCase() === 'body') break;

        if (parentElm.tagName.toLowerCase() === 'svg') {
          _setClass(parentElm, 'introjs-showElement introjs-relativePosition');
        }

        parentElm = parentElm.parentNode;
      }
    }

    _setClass(targetElement.element, 'introjs-showElement');

    var currentElementPosition = _getPropValue(targetElement.element, 'position');
    if (currentElementPosition !== 'absolute' &&
        currentElementPosition !== 'relative' &&
        currentElementPosition !== 'fixed') {
      //change to new intro item
      //targetElement.element.className += ' introjs-relativePosition';
      _setClass(targetElement.element, 'introjs-relativePosition')
    }

    var parentElm = targetElement.element.parentNode;
    while (parentElm != null) {
      if (!parentElm.tagName || parentElm.tagName.toLowerCase() === 'body') break;

      //fix The Stacking Context problem.
      //More detail: https://developer.mozilla.org/en-US/docs/Web/Guide/CSS/Understanding_z_index/The_stacking_context
      var zIndex = _getPropValue(parentElm, 'z-index');
      var opacity = parseFloat(_getPropValue(parentElm, 'opacity'));
      var transform = _getPropValue(parentElm, 'transform') || _getPropValue(parentElm, '-webkit-transform') || _getPropValue(parentElm, '-moz-transform') || _getPropValue(parentElm, '-ms-transform') || _getPropValue(parentElm, '-o-transform');
      if (/[0-9]+/.test(zIndex) || opacity < 1 || (transform !== 'none' && transform !== undefined)) {
        parentElm.className += ' introjs-fixParent';
      }

      parentElm = parentElm.parentNode;
    }
  }

  function _setClass(element, className) {
    if (element instanceof SVGElement) {
      var pre = element.getAttribute('class') || '';

      element.setAttribute('class', pre + ' ' + className);
    } else {
      element.className += ' ' + className;
    }
  }

  function _removeClass(element, classNameRegex) {
    if (element instanceof SVGElement) {
      var pre = element.getAttribute('class') || '';

      element.setAttribute('class', pre.replace(classNameRegex, '').replace(/^\s+|\s+$/g, ''));
    } else {
      element.className = element.className.replace(classNameRegex, '').replace(/^\s+|\s+$/g, '');
    }
  }

  /**
   * Get an element CSS property on the page
   * Thanks to JavaScript Kit: http://www.javascriptkit.com/dhtmltutors/dhtmlcascade4.shtml
   *
   * @api private
   * @method _getPropValue
   * @param {Object} element
   * @param {String} propName
   * @returns Element's property value
   */
  function _getPropValue (element, propName) {
    var propValue = '';
    if (element.currentStyle) { //IE
      propValue = element.currentStyle[propName];
    } else if (document.defaultView && document.defaultView.getComputedStyle) { //Others
      propValue = document.defaultView.getComputedStyle(element, null).getPropertyValue(propName);
    }

    //Prevent exception in IE
    if (propValue && propValue.toLowerCase) {
      return propValue.toLowerCase();
    } else {
      return propValue;
    }
  }

  /**
   * Checks to see if target element (or parents) position is fixed or not
   *
   * @api private
   * @method _isFixed
   * @param {Object} element
   * @returns Boolean
   */
  function _isFixed (element) {
    var p = element.parentNode;

    if (!p || p.nodeName === 'HTML') {
      return false;
    }

    if (_getPropValue(element, 'position') == 'fixed') {
      return true;
    }

    return _isFixed(p);
  }

  /**
   * Provides a cross-browser way to get the screen dimensions
   * via: http://stackoverflow.com/questions/5864467/internet-explorer-innerheight
   *
   * @api private
   * @method _getWinSize
   * @returns {Object} width and height attributes
   */
  function _getWinSize() {
    if (window.innerWidth != undefined) {
      return { width: window.innerWidth, height: window.innerHeight };
    } else {
      var D = document.documentElement;
      return { width: D.clientWidth, height: D.clientHeight };
    }
  }

  /**
   * Check to see if the element is in the viewport or not
   * http://stackoverflow.com/questions/123999/how-to-tell-if-a-dom-element-is-visible-in-the-current-viewport
   *
   * @api private
   * @method _elementInViewport
   * @param {Object} el
   */
  function _elementInViewport(el) {
    var rect = el.getBoundingClientRect();

    return (
      rect.top >= 0 &&
      rect.left >= 0 &&
      (rect.bottom+80) <= window.innerHeight && // add 80 to get the text right
      rect.right <= window.innerWidth
    );
  }

  /**
   * Add overlay layer to the page
   *
   * @api private
   * @method _addOverlayLayer
   * @param {Object} targetElm
   */
  function _addOverlayLayer(targetElm) {
    var overlayLayer = document.createElement('div'),
        styleText = '',
        self = this;

    //set css class name
    overlayLayer.className = 'introjs-overlay';

    //check if the target element is body, we should calculate the size of overlay layer in a better way
    if (!targetElm.tagName || targetElm.tagName.toLowerCase() === 'body') {
      styleText += 'top: 0;bottom: 0; left: 0;right: 0;position: fixed;';
      overlayLayer.setAttribute('style', styleText);
    } else {
      //set overlay layer position
      var elementPosition = _getOffset(targetElm);
      if (elementPosition) {
        styleText += 'width: ' + elementPosition.width + 'px; height:' + elementPosition.height + 'px; top:' + elementPosition.top + 'px;left: ' + elementPosition.left + 'px;';
        overlayLayer.setAttribute('style', styleText);
      }
    }

    targetElm.appendChild(overlayLayer);

    overlayLayer.onclick = function() {
      if (self._options.exitOnOverlayClick == true) {
        _exitIntro.call(self, targetElm);
      }
    };

    setTimeout(function() {
      styleText += 'opacity: ' + self._options.overlayOpacity.toString() + ';';
      overlayLayer.setAttribute('style', styleText);
    }, 10);

    return true;
  }

  /**
   * Removes open hint (tooltip hint)
   *
   * @api private
   * @method _removeHintTooltip
   */
  function _removeHintTooltip() {
    var tooltip = this._targetElement.querySelector('.introjs-hintReference');

    if (tooltip) {
      var step = tooltip.getAttribute('data-step');
      tooltip.parentNode.removeChild(tooltip);
      return step;
    }
  }

  /**
   * Start parsing hint items
   *
   * @api private
   * @param {Object} targetElm
   * @method _startHint
   */
  function _populateHints(targetElm) {
    var self = this;
    this._introItems = [];

    if (this._options.hints) {
      for (var i = 0, l = this._options.hints.length; i < l; i++) {
        var currentItem = _cloneObject(this._options.hints[i]);

        if (typeof(currentItem.element) === 'string') {
          //grab the element with given selector from the page
          currentItem.element = document.querySelector(currentItem.element);
        }

        currentItem.hintPosition = currentItem.hintPosition || this._options.hintPosition;
        currentItem.hintAnimation = currentItem.hintAnimation || this._options.hintAnimation;

        if (currentItem.element != null) {
          this._introItems.push(currentItem);
        }
      }
    } else {
      var hints = targetElm.querySelectorAll('*[data-hint]');

      if (hints.length < 1) {
        return false;
      }

      //first add intro items with data-step
      for (var i = 0, l = hints.length; i < l; i++) {
        var currentElement = hints[i];

        // hint animation
        var hintAnimation = currentElement.getAttribute('data-hintAnimation');

        if (hintAnimation) {
          hintAnimation = (hintAnimation == 'true');
        } else {
          hintAnimation = this._options.hintAnimation;
        }

        this._introItems.push({
          element: currentElement,
          hint: currentElement.getAttribute('data-hint'),
          hintPosition: currentElement.getAttribute('data-hintPosition') || this._options.hintPosition,
          hintAnimation: hintAnimation,
          tooltipClass: currentElement.getAttribute('data-tooltipClass'),
          position: currentElement.getAttribute('data-position') || this._options.tooltipPosition
        });
      }
    }

    _addHints.call(this);

    if (document.addEventListener) {
      document.addEventListener('click', _removeHintTooltip.bind(this), false);
      //for window resize
      window.addEventListener('resize', _reAlignHints.bind(this), true);
    } else if (document.attachEvent) { //IE
      //for window resize
      document.attachEvent('onclick', _removeHintTooltip.bind(this));
      document.attachEvent('onresize', _reAlignHints.bind(this));
    }
  }

  /**
   * Re-aligns all hint elements
   *
   * @api private
   * @method _reAlignHints
   */
  function _reAlignHints() {
    for (var i = 0, l = this._introItems.length; i < l; i++) {
      var item = this._introItems[i];

      if (typeof (item.targetElement) == 'undefined') continue;

      _alignHintPosition.call(this, item.hintPosition, item.element, item.targetElement)
    }
  }

  /**
   * Hide a hint
   *
   * @api private
   * @method _hideHint
   */
  function _hideHint(stepId) {
    _removeHintTooltip.call(this);
    var hint = this._targetElement.querySelector('.introjs-hint[data-step="' + stepId + '"]');

    if (hint) {
      hint.className += ' introjs-hidehint';
    }

    // call the callback function (if any)
    if (typeof (this._hintCloseCallback) !== 'undefined') {
      this._hintCloseCallback.call(this, stepId);
    }
  }

  /**
   * Hide all hints
   *
   * @api private
   * @method _hideHints
   */
  function _hideHints() {
    var hints = this._targetElement.querySelectorAll('.introjs-hint');

    if (hints && hints.length > 0) {
      for (var i = 0; i < hints.length; i++) {
        _hideHint.call(this, hints[i].getAttribute('data-step'));
      }
    }
  }

  /**
   * Show all hints
   *
   * @api private
   * @method _showHints
   */
  function _showHints() {
    var hints = this._targetElement.querySelectorAll('.introjs-hint');

    if (hints && hints.length > 0) {
      for (var i = 0; i < hints.length; i++) {
        _showHint.call(this, hints[i].getAttribute('data-step'));
      }
    } else {
      _populateHints.call(this, this._targetElement);
    }
  };

  /**
   * Show a hint
   *
   * @api private
   * @method _showHint
   */
  function _showHint(stepId) {
    var hint = this._targetElement.querySelector('.introjs-hint[data-step="' + stepId + '"]');

    if (hint) {
      hint.className = hint.className.replace(/introjs\-hidehint/g, '');
    }
  };

  /**
   * Removes all hint elements on the page
   * Useful when you want to destroy the elements and add them again (e.g. a modal or popup)
   *
   * @api private
   * @method _removeHints
   */
  function _removeHints() {
    var hints = this._targetElement.querySelectorAll('.introjs-hint');

    if (hints && hints.length > 0) {
      for (var i = 0; i < hints.length; i++) {
        _removeHint.call(this, hints[i].getAttribute('data-step'));
      }
    }
  };

  /**
   * Remove one single hint element from the page
   * Useful when you want to destroy the element and add them again (e.g. a modal or popup)
   * Use removeHints if you want to remove all elements.
   *
   * @api private
   * @method _removeHint
   */
  function _removeHint(stepId) {
    var hint = this._targetElement.querySelector('.introjs-hint[data-step="' + stepId + '"]');

    if (hint) {
      hint.parentNode.removeChild(hint);
    }
  };

  /**
   * Add all available hints to the page
   *
   * @api private
   * @method _addHints
   */
  function _addHints() {
    var self = this;

    var oldHintsWrapper = document.querySelector('.introjs-hints');

    if (oldHintsWrapper != null) {
      hintsWrapper = oldHintsWrapper;
    } else {
      var hintsWrapper = document.createElement('div');
      hintsWrapper.className = 'introjs-hints';
    }

    for (var i = 0, l = this._introItems.length; i < l; i++) {
      var item = this._introItems[i];

      // avoid append a hint twice
      if (document.querySelector('.introjs-hint[data-step="' + i + '"]'))
        continue;

      var hint = document.createElement('a');
      _setAnchorAsButton(hint);

      (function (hint, item, i) {
        // when user clicks on the hint element
        hint.onclick = function(e) {
          var evt = e ? e : window.event;
          if (evt.stopPropagation)    evt.stopPropagation();
          if (evt.cancelBubble != null) evt.cancelBubble = true;

          _hintClick.call(self, hint, item, i);
        };
      }(hint, item, i));

      hint.className = 'introjs-hint';

      if (!item.hintAnimation) {
        hint.className += ' introjs-hint-no-anim';
      }

      // hint's position should be fixed if the target element's position is fixed
      if (_isFixed(item.element)) {
        hint.className += ' introjs-fixedhint';
      }

      var hintDot = document.createElement('div');
      hintDot.className = 'introjs-hint-dot';
      var hintPulse = document.createElement('div');
      hintPulse.className = 'introjs-hint-pulse';

      hint.appendChild(hintDot);
      hint.appendChild(hintPulse);
      hint.setAttribute('data-step', i);

      // we swap the hint element with target element
      // because _setHelperLayerPosition uses `element` property
      item.targetElement = item.element;
      item.element = hint;

      // align the hint position
      _alignHintPosition.call(this, item.hintPosition, hint, item.targetElement);

      hintsWrapper.appendChild(hint);
    }

    // adding the hints wrapper
    document.body.appendChild(hintsWrapper);

    // call the callback function (if any)
    if (typeof (this._hintsAddedCallback) !== 'undefined') {
      this._hintsAddedCallback.call(this);
    }
  }

  /**
   * Aligns hint position
   *
   * @api private
   * @method _alignHintPosition
   * @param {String} position
   * @param {Object} hint
   * @param {Object} element
   */
  function _alignHintPosition(position, hint, element) {
    // get/calculate offset of target element
    var offset = _getOffset.call(this, element);
    var iconWidth = 20;
    var iconHeight = 20;

    // align the hint element
    switch (position) {
      default:
      case 'top-left':
        hint.style.left = offset.left + 'px';
        hint.style.top = offset.top + 'px';
        break;
      case 'top-right':
        hint.style.left = (offset.left + offset.width - iconWidth) + 'px';
        hint.style.top = offset.top + 'px';
        break;
      case 'bottom-left':
        hint.style.left = offset.left + 'px';
        hint.style.top = (offset.top + offset.height - iconHeight) + 'px';
        break;
      case 'bottom-right':
        hint.style.left = (offset.left + offset.width - iconWidth) + 'px';
        hint.style.top = (offset.top + offset.height - iconHeight) + 'px';
        break;
      case 'middle-left':
        hint.style.left = offset.left + 'px';
        hint.style.top = (offset.top + (offset.height - iconHeight) / 2) + 'px';
        break;
      case 'middle-right':
        hint.style.left = (offset.left + offset.width - iconWidth) + 'px';
        hint.style.top = (offset.top + (offset.height - iconHeight) / 2) + 'px';
        break;
      case 'middle-middle':
        hint.style.left = (offset.left + (offset.width - iconWidth) / 2) + 'px';
        hint.style.top = (offset.top + (offset.height - iconHeight) / 2) + 'px';
        break;
      case 'bottom-middle':
        hint.style.left = (offset.left + (offset.width - iconWidth) / 2) + 'px';
        hint.style.top = (offset.top + offset.height - iconHeight) + 'px';
        break;
      case 'top-middle':
        hint.style.left = (offset.left + (offset.width - iconWidth) / 2) + 'px';
        hint.style.top = offset.top + 'px';
        break;
    }
  }

  /**
   * Triggers when user clicks on the hint element
   *
   * @api private
   * @method _hintClick
   * @param {Object} hintElement
   * @param {Object} item
   * @param {Number} stepId
   */
  function _hintClick(hintElement, item, stepId) {
    // call the callback function (if any)
    if (typeof (this._hintClickCallback) !== 'undefined') {
      this._hintClickCallback.call(this, hintElement, item, stepId);
    }

    // remove all open tooltips
    var removedStep = _removeHintTooltip.call(this);

    // to toggle the tooltip
    if (parseInt(removedStep, 10) == stepId) {
      return;
    }

    var tooltipLayer = document.createElement('div');
    var tooltipTextLayer = document.createElement('div');
    var arrowLayer = document.createElement('div');
    var referenceLayer = document.createElement('div');

    tooltipLayer.className = 'introjs-tooltip';

    tooltipLayer.onclick = function (e) {
      //IE9 & Other Browsers
      if (e.stopPropagation) {
        e.stopPropagation();
      }
      //IE8 and Lower
      else {
        e.cancelBubble = true;
      }
    };

    tooltipTextLayer.className = 'introjs-tooltiptext';

    var tooltipWrapper = document.createElement('p');
    tooltipWrapper.innerHTML = item.hint;

    var closeButton = document.createElement('a');
    closeButton.className = 'introjs-button';
    closeButton.innerHTML = this._options.hintButtonLabel;
    closeButton.onclick = _hideHint.bind(this, stepId);

    tooltipTextLayer.appendChild(tooltipWrapper);
    tooltipTextLayer.appendChild(closeButton);

    arrowLayer.className = 'introjs-arrow';
    tooltipLayer.appendChild(arrowLayer);

    tooltipLayer.appendChild(tooltipTextLayer);

    // set current step for _placeTooltip function
    this._currentStep = hintElement.getAttribute('data-step');

    // align reference layer position
    referenceLayer.className = 'introjs-tooltipReferenceLayer introjs-hintReference';
    referenceLayer.setAttribute('data-step', hintElement.getAttribute('data-step'));
    _setHelperLayerPosition.call(this, referenceLayer);

    referenceLayer.appendChild(tooltipLayer);
    document.body.appendChild(referenceLayer);

    //set proper position
    _placeTooltip.call(this, hintElement, tooltipLayer, arrowLayer, null, true);
  }

  /**
   * Get an element position on the page
   * Thanks to `meouw`: http://stackoverflow.com/a/442474/375966
   *
   * @api private
   * @method _getOffset
   * @param {Object} element
   * @returns Element's position info
   */
  function _getOffset(element) {
    var elementPosition = {};

    var body = document.body;
    var docEl = document.documentElement;

    var scrollTop = window.pageYOffset || docEl.scrollTop || body.scrollTop;
    var scrollLeft = window.pageXOffset || docEl.scrollLeft || body.scrollLeft;

    if (element instanceof SVGElement) {
      var x = element.getBoundingClientRect()
      elementPosition.top = x.top + scrollTop;
      elementPosition.width = x.width;
      elementPosition.height = x.height;
      elementPosition.left = x.left + scrollLeft;
    } else {
      //set width
      elementPosition.width = element.offsetWidth;

      //set height
      elementPosition.height = element.offsetHeight;

      //calculate element top and left
      var _x = 0;
      var _y = 0;
      while (element && !isNaN(element.offsetLeft) && !isNaN(element.offsetTop)) {
        _x += element.offsetLeft;
        _y += element.offsetTop;
        element = element.offsetParent;
      }
      //set top
      elementPosition.top = _y;
      //set left
      elementPosition.left = _x;
    }

    return elementPosition;
  }

  /**
   * Gets the current progress percentage
   *
   * @api private
   * @method _getProgress
   * @returns current progress percentage
   */
  function _getProgress() {
    // Steps are 0 indexed
    var currentStep = parseInt((this._currentStep + 1), 10);
    return ((currentStep / this._introItems.length) * 100);
  }

  /**
   * Overwrites obj1's values with obj2's and adds obj2's if non existent in obj1
   * via: http://stackoverflow.com/questions/171251/how-can-i-merge-properties-of-two-javascript-objects-dynamically
   *
   * @param obj1
   * @param obj2
   * @returns obj3 a new object based on obj1 and obj2
   */
  function _mergeOptions(obj1,obj2) {
    var obj3 = {};
    for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
    for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
    return obj3;
  }

  var introJs = function (targetElm) {
    if (typeof (targetElm) === 'object') {
      //Ok, create a new instance
      return new IntroJs(targetElm);

    } else if (typeof (targetElm) === 'string') {
      //select the target element with query selector
      var targetElement = document.querySelector(targetElm);

      if (targetElement) {
        return new IntroJs(targetElement);
      } else {
        throw new Error('There is no element with given selector.');
      }
    } else {
      return new IntroJs(document.body);
    }
  };

  /**
   * Current IntroJs version
   *
   * @property version
   * @type String
   */
  introJs.version = VERSION;

  //Prototype
  introJs.fn = IntroJs.prototype = {
    clone: function () {
      return new IntroJs(this);
    },
    setOption: function(option, value) {
      this._options[option] = value;
      return this;
    },
    setOptions: function(options) {
      this._options = _mergeOptions(this._options, options);
      return this;
    },
    start: function () {
      _introForElement.call(this, this._targetElement);
      return this;
    },
    goToStep: function(step) {
      _goToStep.call(this, step);
      return this;
    },
    addStep: function(options) {
      if (!this._options.steps) {
        this._options.steps = [];
      }

      this._options.steps.push(options);

      return this;
    },
    addSteps: function(steps) {
      if (!steps.length) return;

      for(var index = 0; index < steps.length; index++) {
        this.addStep(steps[index]);
      }

      return this;
    },
    goToStepNumber: function(step) {
      _goToStepNumber.call(this, step);

      return this;
    },
    nextStep: function() {
      _nextStep.call(this);
      return this;
    },
    previousStep: function() {
      _previousStep.call(this);
      return this;
    },
    exit: function() {
      _exitIntro.call(this, this._targetElement);
      return this;
    },
    refresh: function() {
      // re-align intros
      _setHelperLayerPosition.call(this, document.querySelector('.introjs-helperLayer'));
      _setHelperLayerPosition.call(this, document.querySelector('.introjs-tooltipReferenceLayer'));

      //re-align hints
      _reAlignHints.call(this);
      return this;
    },
    onbeforechange: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._introBeforeChangeCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onbeforechange was not a function');
      }
      return this;
    },
    onchange: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._introChangeCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onchange was not a function.');
      }
      return this;
    },
    onafterchange: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._introAfterChangeCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onafterchange was not a function');
      }
      return this;
    },
    oncomplete: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._introCompleteCallback = providedCallback;
      } else {
        throw new Error('Provided callback for oncomplete was not a function.');
      }
      return this;
    },
    onhintsadded: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._hintsAddedCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onhintsadded was not a function.');
      }
      return this;
    },
    onhintclick: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._hintClickCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onhintclick was not a function.');
      }
      return this;
    },
    onhintclose: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._hintCloseCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onhintclose was not a function.');
      }
      return this;
    },
    onexit: function(providedCallback) {
      if (typeof (providedCallback) === 'function') {
        this._introExitCallback = providedCallback;
      } else {
        throw new Error('Provided callback for onexit was not a function.');
      }
      return this;
    },
    addHints: function() {
      _populateHints.call(this, this._targetElement);
      return this;
    },
    hideHint: function (stepId) {
      _hideHint.call(this, stepId);
      return this;
    },
    hideHints: function () {
      _hideHints.call(this);
      return this;
    },
    showHint: function (stepId) {
      _showHint.call(this, stepId);
      return this;
    },
    showHints: function () {
      _showHints.call(this);
      return this;
    },
    removeHints: function () {
      _removeHints.call(this);
      return this;
    },
    removeHint: function (stepId) {
      _removeHint.call(this, stepId);
      return this;
    }
  };

  exports.introJs = introJs;
  return introJs;
}));
;
(function($) {
$.widget("ui.exp_sp_tour", {
	getCurrentUrl: function(page) {
	    try{
			var url = window.location.href.split("/?q=");
			return url[1];
		}catch(e){
		// to do
			// console.log(e, e.stack);
	    }
	},
	getSteps: function() {
	    try{
	    	var steps = [];
			if(Drupal.settings.exp_sp_tour !== undefined) {
				steps = JSON.parse(Drupal.settings.exp_sp_tour.tourconfig);
			}
			return steps;
	    }catch(e){
			// to do
			// console.log(e, e.stack);
	    }
	},

	_init: function() {
	    try{
	    			    	
	    	var isTourDivExist = ($('#tour_mylearn').length > 0 ? true : false);
			var steps = this.getSteps();
			// console.log('steps', steps);
			// console.log(this.rearrangeStep(steps));
			//append tour div if not exists and steps for current page is available
			if(!isTourDivExist && ((steps !== undefined && steps.length > 0) || $('[data-intro]').length > 0)) {
				$('<div class = "tour-outer-wrapper"><div id = "tour_mylearn" onclick="$(\'body\').data(\'exp_sp_tour\').startTour();"><p>'+Drupal.t('TOUR')+'</p></div></div>').prependTo('body');	
			}
			if(Drupal.settings.exp_sp_tour.auto_start == 1){
	    		setTimeout(function(){	    			
	    			$("#tour_mylearn").click()}, 500);	    		
	    	}
	    }catch(e){
			// to do
			// console.log(e, e.stack);
	    }
	},

	startTour: function() {
		try {
			var introguide = introJs();
			var steps = this.getSteps();
			var obj = this;
			// console.log('steps', steps);
			var options = {
				showStepNumbers: false,
				overlayOpacity: 0.5,
				exitOnOverlayClick: false,
				nextLabel: Drupal.t('LBL693'),
				prevLabel: Drupal.t('LBL692'),
				skipLabel: Drupal.t('LBL3213'),
				doneLabel: Drupal.t('LBL569'),
				// steps: steps
			};
			if(steps !== undefined) {
				options.steps = obj.rearrangeStep(steps).filter(function(item) {
					// console.log($(item.element).length);
					// console.log(item.welcomeText);
					var displayAlways = ['#paintCriteriaResults', '#refine-filter-pin-icon.pin-icon-white'];
					return (($(item.element).length > 0 && $(item.element).is(':visible')) || item.welcomeText || displayAlways.indexOf(item.element) != -1);
				});
				// options.steps = obj.rearrangeStep(options.steps)
				// console.log(options.steps);
			}
			introguide.setOptions(options)
			.onbeforechange(function (targetElement) {
			try {
				// console.log($(targetElement));
				var refineHidden = $('#paintCriteriaResults').hasClass('searchcriteria-div-unpinned');
				if(refineHidden) {
					if($(targetElement).attr('id') == 'paintCriteriaResults') {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
					} else if($(targetElement).is('#refine-filter-pin-icon.pin-icon-white')) {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
					}
				} else {
					if($(targetElement).is('#refine-filter-pin-icon.pin-icon-black')) {
						$('#lnr-catalog-search').data('lnrcatalogsearch').pinUnpinFilterCriteria();
						$('.catalog-criteria-refine-icon').trigger('mouseenter');
					}
				}
				//delete exiting mask div if any
				$('.introjs-helperLayer-Mask').remove();
			} catch(e) {
				// console.log(e, e.stack);
			}
            })
			.onafterchange(function(targetElement){
				//add a mask div to restrict actions on target element if any
				$('.introjs-helperLayer').clone().appendTo('body')
					.addClass('introjs-helperLayer-Mask');
				$('.tour-common').css("height","41px");
				$('.tour-filter').css("width","63px");
				$('.tour-filter').css("margin-left","-7px");
				$('.tour-common').css("margin-top","3px");
			})
			.oncomplete(function() {
				$('.introjs-helperLayer-Mask').remove();
			})
			.onexit(function() {
				$('.introjs-helperLayer-Mask').remove();
			})
			.start();
			// console.log(options.steps);
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	rearrangeStep: function(steps) {
		if(this.getCurrentUrl() == 'learning/enrollment-search') {
			var elementsArr = ['#block-exp-sp-lnrlearningplan-tab-my-learningplan-customized', '#block-exp-sp-lnrenrollment-tab-my-enrollment-customized', '#block-exp-sp-instructor-desk-tab-instructor-desk-customized'];
			var compare = function (a, b) {
				// console.log(a, b);
				// console.log($.inArray(a.element, elementsArr), $.inArray(b.element, elementsArr));
				if($.inArray(a.element, elementsArr) != -1 && $.inArray(b.element, elementsArr) != -1) {
					var selectorArr = $(a.element+', '+b.element);
					// console.log(selectorArr);
					var aIndex = 0;
					var bIndex = 0;
					$(a.element+', '+b.element).each(function(index, item) {
						// console.log(index, item);
						if($(item).is(a.element)) {
							aIndex = index;
						} else if($(item).is(b.element)) {
							bIndex = index;
						}
					});
					// console.log('aIndex', aIndex, 'bIndex', bIndex);
					if (aIndex < bIndex)
						return -1;
					if (aIndex > bIndex)
						return 1;
				}
				return 0;
			}
			return steps.sort(compare);
		}
		return steps;
	}
});

$.extend($.ui.exp_sp_tour.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});

})(jQuery);

$(function() {
	try{
	$('body').exp_sp_tour();
	}catch(e){
		// to do
	}
});;
// MSDropDown - jquery.dd.js
// author: Marghoob Suleman - Search me on google
// Date: 12th Aug, 2009
// Version: 2.38.4
// Revision: 38
// web: www.giftlelo.com | www.marghoobsuleman.com
/*
// msDropDown is free jQuery Plugin: you can redistribute it and/or modify
// it under the terms of the either the MIT License or the Gnu General Public License (GPL) Version 2
*/
;eval(function(p,a,c,k,e,r){e=function(c){return(c<a?'':e(parseInt(c/a)))+((c=c%a)>35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)r[e(c)]=k[c]||e(c);k=[function(e){return r[e]}];e=function(){return'\\w+'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}(';(6($){3 1N="";3 3B=6(s,u){3 v=s;3 x=1b;3 u=$.3C({1c:4s,2w:7,2S:23,1O:9,1P:4t,3D:\'2g\',1J:10,3E:\'4u\',2T:\'\',2U:9,1k:\'\'},u);1b.1Z=21 3F();3 y="";3 z={};z.2V=9;z.2x=10;z.2y=1r;3 A=10;3 B={2W:\'4v\',1Q:\'4w\',1K:\'4x\',2h:\'4y\',1g:\'4z\',2X:\'4A\',2Y:\'4B\',4C:\'4D\',2z:\'4E\',3G:\'4F\'};3 C={2g:u.3D,2Z:\'2Z\',31:\'31\',32:\'32\',1w:\'1w\',1l:.30,1R:\'1R\',2A:\'2A\',2B:\'2B\',15:\'15\'};3 D={3H:"2C,33,34,1S,2D,2E,1s,1A,2F,1T,4G,22,35",1a:"1B,1x,1l,4H"};1b.1U=21 3F();3 E=$(v).1a("1d");5(1e(E)=="14"||E.18<=0){E="4I"+$.1V.3I++;$(v).2i("1d",E)};3 F=$(v).1a("1k");u.1k+=(F==14)?"":F;3 G=$(v).3J();A=($(v).1a("1B")>1||$(v).1a("1x")==9)?9:10;5(A){u.2w=$(v).1a("1B")};3 H={};3 I=0;3 J=10;3 K;3 L=10;3 M={};3 N="";3 O=6(a){5(1e(M[a])=="14"){M[a]=1o.4J(a)}11 M[a]};3 P=6(a){11 E+B[a]};3 Q=6(a){3 b=a;3 c=$(b).1a("1k");11(1e c=="14")?"":c.4K};3 R=6(a){3 b=$("#"+E+" 36:15");5(b.18>1){1C(3 i=0;i<b.18;i++){5(a==b[i].1j){11 9}}}19 5(b.18==1){5(b[0].1j==a){11 9}};11 10};3 S=6(a,b,c,d){3 e="";3 f=(d=="3a")?P("2Y"):P("2X");3 g=(d=="3a")?f+"3b"+(b)+"3b"+(c):f+"3b"+(b);3 h="";3 t="";3 i="";3 j="";5(u.1J!=10){i=\' \'+u.1J+\' \'+a.3K}19{h=$(a).1a("1p");3 k=21 3L(/^\\{.*\\}$/);3 l=k.3M(h);5(u.2U==9&&l==9){5(h.18!=0){3 m=24("["+h+"]");1W=(1e m[0].2j=="14")?"":m[0].2j;t=(1e m[0].1p=="14")?"":m[0].1p;j=(1e m[0].3N=="14")?"":m[0].3N;h=(1W.18==0)?"":\'<1W 2G="\'+1W+\'" 2H="2I" /> \'}}19{h=(h.18==0)?"":\'<1W 2G="\'+h+\'" 2H="2I" /> \'}};3 n=$(a).1t();3 o=$(a).4L();3 p=($(a).1a("1l")==9)?"1l":"3c";H[g]={1L:h+n,2k:o,1t:n,1j:a.1j,1d:g,1p:t};3 q=Q(a);5(R(a.1j)==9){e+=\'<a 3O="3P:3Q(0);" 1u="\'+C.15+\' \'+p+i+\'"\'}19{e+=\'<a  3O="3P:3Q(0);" 1u="\'+p+i+\'"\'};5(q!==10&&q!==14&&q.18!=0){e+=" 1k=\'"+q+"\'"};5(t!==""){e+=" 1p=\'"+t+"\'"};e+=\' 1d="\'+g+\'">\';e+=h+\'<1y 1u="\'+C.1w+\'">\'+n+\'</1y>\';5(j!==""){e+=j};e+=\'</a>\';11 e};3 T=6(t){3 b=t.3d();5(b.18==0)11-1;3 a="";1C(3 i 2l H){3 c=H[i].1t.3d();5(c.3R(0,b.18)==b){a+="#"+H[i].1d+", "}};11(a=="")?-1:a};3 U=6(){3 f=G;5(f.18==0)11"";3 g="";3 h=P("2X");3 i=P("2Y");f.3e(6(c){3 d=f[c];5(d.4M.4N().3d()=="4O"){g+="<1z 1u=\'4P\'>";g+="<1y 1k=\'3S-4Q:4R;3S-1k:4S;4T:4U;\'>"+$(d).1a("4V")+"</1y>";3 e=$(d).3J();e.3e(6(a){3 b=e[a];g+=S(b,c,a,"3a")});g+="</1z>"}19{g+=S(d,c,"","")}});11 g};3 V=6(){3 a=P("1Q");3 b=P("1g");3 c=u.1k;25="";25+=\'<1z 1d="\'+b+\'" 1u="\'+C.32+\'"\';5(!A){25+=(c!="")?\' 1k="\'+c+\'"\':\'\'}19{25+=(c!="")?\' 1k="2J-1D:4W 4X #4Y;1q:2m;1m:2K;\'+c+\'"\':\'\'};25+=\'>\';11 25};3 W=6(){3 a=P("1K");3 b=P("2z");3 c=P("2h");3 d=P("3G");3 e="";3 f="";5(O(E).1E.18>0){e=$("#"+E+" 36:15").1t();f=$("#"+E+" 36:15").1a("1p")};3 g="";3 t="";3 h=21 3L(/^\\{.*\\}$/);3 i=h.3M(f);5(u.2U==9&&i==9){5(f.18!=0){3 j=24("["+f+"]");g=(1e j[0].2j=="14")?"":j[0].2j;t=(1e j[0].1p=="14")?"":j[0].1p;f=(g.18==0||u.1O==10||u.1J!=10)?"":\'<1W 2G="\'+g+\'" 2H="2I" /> \'}}19{f=(f.18==0||f==14||u.1O==10||u.1J!=10)?"":\'<1W 2G="\'+f+\'" 2H="2I" /> \'};3 k=\'<1z 1d="\'+a+\'" 1u="\'+C.2Z+\'"\';k+=\'>\';k+=\'<1y 1d="\'+b+\'" 1u="\'+C.31+\'"></1y><1y 1u="\'+C.1w+\'" 1d="\'+c+\'">\'+f+\'<1y 1u="\'+C.1w+\'">\'+e+\'</1y></1y></1z>\';11 k};3 X=6(){3 c=P("1g");$("#"+c+" a.3c").1F("1S");$("#"+c+" a.3c").1f("1S",6(a){a.26();3f(1b);28();5(!A){$("#"+c).1F("1A");29(10);3 b=(u.1O==10)?$(1b).1t():$(1b).1L();1X(b);x.2n()}})};3 Y=6(){3 d=10;3 e=P("1Q");3 f=P("1K");3 g=P("2h");3 h=P("1g");3 i=P("2z");3 j=$("#"+E).4Z();3 k=u.1k;5($("#"+e).18>0){$("#"+e).2L();d=9};3 l=\'<1z 1d="\'+e+\'" 1u="\'+C.2g+\'"\';l+=(k!="")?\' 1k="\'+k+\'"\':\'\';l+=\'>\';l+=W();l+=V();l+=U();l+="</1z>";l+="</1z>";5(d==9){3 m=P("2W");$("#"+m).3g(l)}19{$("#"+E).3g(l)};5(A){3 f=P("1K");$("#"+f).2o()};$("#"+e).12("3T",j+"1v");$("#"+h).12("3T",(j-2)+"1v");5(G.18>u.2w){3 n=2p($("#"+h+" a:3h").12("2q-3U"))+2p($("#"+h+" a:3h").12("2q-1D"));3 o=((u.2S)*u.2w)-n;$("#"+h).12("1c",o+"1v")}19 5(A){3 o=$("#"+E).1c();$("#"+h).12("1c",o+"1v")};5(d==10){3V();3W(E)};5($("#"+E).1a("1l")==9){$("#"+e).12("2M",C.1l)};3X();$("#"+f).1f("1A",6(a){3i(1)});$("#"+f).1f("1T",6(a){3i(0)});X();$("#"+h+" a.1l").12("2M",C.1l);5(A){$("#"+h).1f("1A",6(c){5(!z.2x){z.2x=9;$(1o).1f("22",6(a){3 b=a.3Y;z.2y=b;5(b==39||b==40){a.26();a.2r();3j();28()};5(b==37||b==38){a.26();a.2r();3k();28()}})}})};$("#"+h).1f("1T",6(a){29(10);$(1o).1F("22",2N);z.2x=10;z.2y=1r});$("#"+f).1f("1S",6(b){29(10);5($("#"+h+":2a").18==1){$("#"+h).1F("1A")}19{$("#"+h).1f("1A",6(a){29(9)});x.3Z()}});$("#"+f).1f("1T",6(a){29(10)});5(u.1O&&u.1J!=10){2s()}};3 Z=6(a){1C(3 i 2l H){5(H[i].1j==a){11 H[i]}};11-1};3 3f=6(a){3 b=P("1g");5($("#"+b+" a."+C.15).18==1){y=$("#"+b+" a."+C.15).1t()};5(!A){$("#"+b+" a."+C.15).1M(C.15)};3 c=$("#"+b+" a."+C.15).1a("1d");5(c!=14){3 d=(z.2b==14||z.2b==1r)?H[c].1j:z.2b};5(a&&!A){$(a).1G(C.15)};5(A){3 e=z.2y;5($("#"+E).1a("1x")==9){5(e==17){z.2b=H[$(a).1a("1d")].1j;$(a).50(C.15)}19 5(e==16){$("#"+b+" a."+C.15).1M(C.15);$(a).1G(C.15);3 f=$(a).1a("1d");3 g=H[f].1j;1C(3 i=3l.51(d,g);i<=3l.52(d,g);i++){$("#"+Z(i).1d).1G(C.15)}}19{$("#"+b+" a."+C.15).1M(C.15);$(a).1G(C.15);z.2b=H[$(a).1a("1d")].1j}}19{$("#"+b+" a."+C.15).1M(C.15);$(a).1G(C.15);z.2b=H[$(a).1a("1d")].1j}}};3 3W=6(a){3 b=a;O(b).53=6(e){$("#"+b).1V(u)}};3 29=6(a){z.2V=a};3 41=6(){11 z.2V};3 3X=6(){3 b=P("1Q");3 c=D.3H.54(",");1C(3 d=0;d<c.18;d++){3 e=c[d];3 f=2c(e);5(f==9){2O(e){1h"2C":$("#"+b).1f("55",6(a){O(E).2C()});1i;1h"1S":$("#"+b).1f("1S",6(a){$("#"+E).1H("1S")});1i;1h"2D":$("#"+b).1f("2D",6(a){$("#"+E).1H("2D")});1i;1h"2E":$("#"+b).1f("2E",6(a){$("#"+E).1H("2E")});1i;1h"1s":$("#"+b).1f("1s",6(a){$("#"+E).1H("1s")});1i;1h"1A":$("#"+b).1f("1A",6(a){$("#"+E).1H("1A")});1i;1h"2F":$("#"+b).1f("2F",6(a){$("#"+E).1H("2F")});1i;1h"1T":$("#"+b).1f("1T",6(a){$("#"+E).1H("1T")});1i}}}};3 3V=6(){3 a=P("2W");$("#"+E).3g("<1z 1u=\'"+C.1R+"\' 1k=\'1c:3m;42:43;1m:2P;\' 1d=\'"+a+"\'></1z>");$("#"+E).56($("#"+a))};3 1X=6(a){3 b=P("2h");$("#"+b).1L(a)};3 3n=6(w){3 a=w;3 b=P("1g");3 c=$("#"+b+" a:2a");3 d=c.18;3 e=$("#"+b+" a:2a").1j($("#"+b+" a.15:2a"));3 f;2O(a){1h"3o":5(e<d-1){e++;f=c[e]};1i;1h"44":5(e<d&&e>0){e--;f=c[e]};1i};5(1e(f)=="14"){11 10};$("#"+b+" a."+C.15).1M(C.15);$(f).1G(C.15);3 g=f.1d;5(!A){3 h=(u.1O==10)?H[g].1t:$("#"+g).1L();1X(h);2s(H[g].1j)};5(a=="3o"){5(2p(($("#"+g).1m().1D+$("#"+g).1c()))>=2p($("#"+b).1c())){$("#"+b).2t(($("#"+b).2t())+$("#"+g).1c()+$("#"+g).1c())}}19{5(2p(($("#"+g).1m().1D+$("#"+g).1c()))<=0){$("#"+b).2t(($("#"+b).2t()-$("#"+b).1c())-$("#"+g).1c())}}};3 3j=6(){3n("3o")};3 3k=6(){3n("44")};3 2s=6(i){5(u.1J!=10){3 a=P("2h");3 b=(1e(i)=="14")?O(E).1n:i;3 c=O(E).1E[b].3K;5(c.18>0){3 d=P("1g");3 e=$("#"+d+" a."+c).1a("1d");3 f=$("#"+e).12("1Y-2j");3 g=$("#"+e).12("1Y-1m");5(g==14){g=$("#"+e).12("1Y-1m-x")+" "+$("#"+e).12("1Y-1m-y")};3 h=$("#"+e).12("2q-45");5(f!=14){$("#"+a).2u("."+C.1w).2i(\'1k\',"1Y:"+f)};5(g!=14){$("#"+a).2u("."+C.1w).12(\'1Y-1m\',g)};5(h!=14){$("#"+a).2u("."+C.1w).12(\'2q-45\',h)};$("#"+a).2u("."+C.1w).12(\'1Y-47\',\'57-47\');$("#"+a).2u("."+C.1w).12(\'2q-3U\',\'58\')}}};3 28=6(){3 a=P("1g");3 b=$("#"+a+" a."+C.15);5(b.18==1){3 c=$("#"+a+" a."+C.15).1t();3 d=$("#"+a+" a."+C.15).1a("1d");5(d!=14){3 e=H[d].2k;O(E).1n=H[d].1j};5(u.1O&&u.1J!=10)2s()}19 5(b.18>1){1C(3 i=0;i<b.18;i++){3 d=$(b[i]).1a("1d");3 f=H[d].1j;O(E).1E[f].15="15"}};3 g=O(E).1n;x.1Z["1n"]=g};3 2c=6(a){5($("#"+E).1a("59"+a)!=14){11 9};3 b=$("#"+E).3p("5a");5(b&&b[a]){11 9};11 10};3 3q=6(a){$("#"+E).2C();$("#"+E)[0].33();28();$(1o).1F("1s",2Q);$(1o).1F("1s",3q)};3 48=6(){3 a=P("1g");5(2c(\'34\')==9){3 b=H[$("#"+a+" a.15").1a("1d")].1t;5($.49(y)!==$.49(b)&&y!==""){$("#"+E).1H("34")}};5(2c(\'1s\')==9){$("#"+E).1H("1s")};5(2c(\'33\')==9){$(1o).1f("1s",3q)};11 10};3 3i=6(a){3 b=P("2z");5(a==1)$("#"+b).12({4a:\'0 5b%\'});19 $("#"+b).12({4a:\'0 0\'})};3 4b=6(){1C(3 i 2l O(E)){5(1e(O(E)[i])!==\'6\'&&1e(O(E)[i])!=="14"&&1e(O(E)[i])!=="1r"){x.1I(i,O(E)[i],9)}}};3 4c=6(a,b){5(Z(b)!=-1){O(E)[a]=b;3 c=P("1g");$("#"+c+" a."+C.15).1M(C.15);$("#"+Z(b).1d).1G(C.15);3 d=Z(O(E).1n).1L;1X(d)}};3 4d=6(i,a){5(a==\'d\'){1C(3 b 2l H){5(H[b].1j==i){5c H[b];1i}}};3 c=0;1C(3 b 2l H){H[b].1j=c;c++}};3 2R=6(){3 a=P("1g");3 b=P("1Q");3 c=$("#"+b).5d();3 d=$("#"+b).1c();3 e=$(4e).1c();3 f=$(4e).2t();3 g=$("#"+a).1c();3 h={1P:u.1P,1D:(d)+"1v",1q:"2d"};3 i=u.3E;3 j=10;3 k=C.2B;$("#"+a).1M(C.2B);$("#"+a).1M(C.2A);5((e+f)<3l.5e(g+d+c.1D)){3 l=g;h={1P:u.1P,1D:"-"+l+"1v",1q:"2d"};i="2e";j=9;k=C.2A};11{3r:j,4f:i,12:h,2J:k}};3 3s=6(){5(x.1U["4g"]!=1r){24(x.1U["4g"])(x)}};3 3t=6(){48();5(x.1U["4h"]!=1r){24(x.1U["4h"])(x)}};3 2N=6(a){3 b=P("1g");3 c=a.3Y;5(c==8){a.26();a.2r();N=(N.18==0)?"":N.3R(0,N.18-1)};2O(c){1h 39:1h 40:a.26();a.2r();3j();1i;1h 37:1h 38:a.26();a.2r();3k();1i;1h 27:1h 13:x.2n();28();1i;4i:5(c>46){N+=5f.5g(c)};3 d=T(N);5(d!=-1){$("#"+b).12({1c:\'5h\'});$("#"+b+" a").2o();$(d).2e();3 e=2R();$("#"+b).12(e.12);$("#"+b).12({1q:\'2m\'})}19{$("#"+b+" a").2e();$("#"+b).12({1c:K+\'1v\'})};1i};5(2c("22")==9){O(E).5i()};11 10};3 2Q=6(a){5(41()==10){x.2n()};11 10};3 3u=6(a){5($("#"+E).1a("4j")!=14){O(E).4j()};11 10};1b.3Z=6(){5((x.2f("1l",9)==9)||(x.2f("1E",9).18==0))11;3 a=P("1g");5(1N!=""&&a!=1N){$("#"+1N).4k("3v");$("#"+1N).12({1P:\'0\'})};5($("#"+a).12("1q")=="2d"){y=H[$("#"+a+" a.15").1a("1d")].1t;N="";K=$("#"+a).1c();$("#"+a+" a").2e();$(1o).1f("22",2N);$(1o).1f("35",3u);$(1o).1f("1s",2Q);3 b=2R();$("#"+a).12(b.12);5(b.3r==9){$("#"+a).12({1q:\'2m\'});$("#"+a).1G(b.2J);3s()}19{$("#"+a)[b.4f]("3v",6(){$("#"+a).1G(b.2J);3s()})};5(a!=1N){1N=a}}};1b.2n=6(){3 b=P("1g");5(!$("#"+b).4l(":2a")||L)11;L=9;5($("#"+b).12("1q")=="2d"){11 10};3 c=$("#"+P("1K")).1m().1D;3 d=2R();J=10;5(d.3r==9){$("#"+b).5j({1c:0,1D:c},6(){$("#"+b).12({1c:K+\'1v\',1q:\'2d\'});3t();L=10})}19{$("#"+b).4k("3v",6(a){3t();$("#"+b).12({1P:\'0\'});$("#"+b).12({1c:K+\'1v\'});L=10})};2s();$(1o).1F("22",2N);$(1o).1F("35",3u);$(1o).1F("1s",2Q)};1b.1n=6(i){5(1e(i)=="14"){11 x.2f("1n")}19{x.1I("1n",i)}};1b.4m=6(a){5(1e(a)=="14"||a==9){$("."+C.1R).5k("1k")}19{$("."+C.1R).2i("1k","1c:3m;42:43;1m:2P")}};1b.1I=6(a,b,c){5(1e a=="14"||1e b=="14")11 10;x.1Z[a]=b;5(c!=9){2O(a){1h"1n":4c(a,b);1i;1h"1l":x.1l(b,9);1i;1h"1x":O(E)[a]=b;A=($(v).1a("1B")>0||$(v).1a("1x")==9)?9:10;5(A){3 d=$("#"+E).1c();3 f=P("1g");$("#"+f).12("1c",d+"1v");3 g=P("1K");$("#"+g).2o();3 f=P("1g");$("#"+f).12({1q:\'2m\',1m:\'2K\'});X()};1i;1h"1B":O(E)[a]=b;5(b==0){O(E).1x=10};A=($(v).1a("1B")>0||$(v).1a("1x")==9)?9:10;5(b==0){3 g=P("1K");$("#"+g).2e();3 f=P("1g");$("#"+f).12({1q:\'2d\',1m:\'2P\'});3 h="";5(O(E).1n>=0){3 i=Z(O(E).1n);h=i.1L;3f($("#"+i.1d))};1X(h)}19{3 g=P("1K");$("#"+g).2o();3 f=P("1g");$("#"+f).12({1q:\'2m\',1m:\'2K\'})};1i;4i:4n{O(E)[a]=b}4o(e){};1i}}};1b.2f=6(a,b){5(a==14&&b==14){11 x.1Z};5(a!=14&&b==14){11(x.1Z[a]!=14)?x.1Z[a]:1r};5(a!=14&&b!=14){11 O(E)[a]}};1b.2a=6(a){3 b=P("1Q");5(a==9){$("#"+b).2e()}19 5(a==10){$("#"+b).2o()}19{11 $("#"+b).12("1q")}};1b.5l=6(a,b){3 c=a;3 d=c.1t;3 e=(c.2k==14||c.2k==1r)?d:c.2k;3 f=(c["1p"]==14||c["1p"]==1r)?\'\':c["1p"];3 i=(b==14||b==1r)?O(E).1E.18:b;O(E).1E[i]=21 5m(d,e);5(f!=\'\')O(E).1E[i]["1p"]=f;3 g=Z(i);5(g!=-1){3 h=S(O(E).1E[i],i,"","");$("#"+g.1d).1L(h)}19{3 h=S(O(E).1E[i],i,"","");3 j=P("1g");$("#"+j).5n(h);X()}};1b.2L=6(i){O(E).2L(i);5((Z(i))!=-1){$("#"+Z(i).1d).2L();4d(i,\'d\')};5(O(E).18==0){1X("")}19{3 a=Z(O(E).1n).1L;1X(a)};x.1I("1n",O(E).1n)};1b.1l=6(a,b){O(E).1l=a;3 c=P("1Q");5(a==9){$("#"+c).12("2M",C.1l);x.2n()}19 5(a==10){$("#"+c).12("2M",1)};5(b!=9){x.1I("1l",a)}};1b.3w=6(){11(O(E).3w==14)?1r:O(E).3w};1b.3x=6(){5(2v.18==1){11 O(E).3x(2v[0])}19 5(2v.18==2){11 O(E).3x(2v[0],2v[1])}19{5o{5p:"5q 1j 4l 5r!"}}};1b.4p=6(a){11 O(E).4p(a)};1b.1x=6(a){5(1e(a)=="14"){11 x.2f("1x")}19{x.1I("1x",a)}};1b.1B=6(a){5(1e(a)=="14"){11 x.2f("1B")}19{x.1I("1B",a)}};1b.5s=6(a,b){x.1U[a]=b};1b.5t=6(a){24(x.1U[a])(x)};1b.5u=6(r){5(1e r=="14"||r==0){11 10};3 a=P("1g");3 b=$("#"+a+" a:3h").1c();3 c=(b==0)?u.2S:b;3 d=r*c;$("#"+a).12("1c",d+"1v")};3 4q=6(){x.1I("3y",$.1V.3y);x.1I("3z",$.1V.3z)};3 4r=6(){Y();4b();4q();5(u.2T!=\'\'){24(u.2T)(x)}};4r()};$.1V={3y:\'2.38.4\',3z:"5v 5w",3I:20,4m:6(v){5(v==9){$(".1R").12({1c:\'5x\',1m:\'2K\'})}19{$(".1R").12({1c:\'3m\',1m:\'2P\'})}},5y:6(a,b){11 $(a).1V(b).3p("2g")}};$.3A.3C({1V:6(b){11 1b.3e(6(){3 a=21 3B(1b,b);$(1b).3p(\'2g\',a)})}});5(1e($.3A.1a)==\'14\'){$.3A.1a=6(w,v){5(1e v=="14"){11 $(1b).2i(w)};4n{$(1b).2i(w,v)}4o(e){}}}})(5z);',62,346,'|||var||if|function|||true|||||||||||||||||||||||||||||||||||||||||||||||||||||false|return|css||undefined|selected|||length|else|prop|this|height|id|typeof|bind|postChildID|case|break|index|style|disabled|position|selectedIndex|document|title|display|null|mouseup|text|class|px|ddTitleText|multiple|span|div|mouseover|size|for|top|options|unbind|addClass|trigger|set|useSprite|postTitleID|html|removeClass|bB|showIcon|zIndex|postID|ddOutOfVision|click|mouseout|onActions|msDropDown|img|bJ|background|ddProp||new|keydown||eval|sDiv|preventDefault||bO|bF|visible|oldIndex|bP|none|show|get|dd|postTitleTextID|attr|image|value|in|block|close|hide|parseInt|padding|stopPropagation|bN|scrollTop|find|arguments|visibleRows|keyboardAction|currentKey|postArrowID|borderTop|noBorderTop|focus|dblclick|mousedown|mousemove|src|align|absmiddle|border|relative|remove|opacity|bZ|switch|absolute|ca|bW|rowHeight|onInit|jsonTitle|insideWindow|postElementHolder|postAID|postOPTAID|ddTitle||arrow|ddChild|blur|change|keyup|option||||opt|_|enabled|toLowerCase|each|bD|after|first|bS|bL|bM|Math|0px|bK|next|data|bQ|opp|bX|bY|cb|fast|form|item|version|author|fn|bC|extend|mainCSS|animStyle|Object|postInputhidden|actions|counter|children|className|RegExp|test|postHTML|href|javascript|void|substr|font|width|bottom|bI|bE|bH|keyCode|open||bG|overflow|hidden|previous|left||repeat|bR|trim|backgroundPositions|bT|bU|bV|window|ani|onOpen|onClose|default|onkeyup|slideUp|is|debug|try|catch|namedItem|cc|cd|120|9999|slideDown|_msddHolder|_msdd|_title|_titletext|_child|_msa|_msopta|postInputID|_msinput|_arrow|_inp|keypress|tabindex|msdrpdd|getElementById|cssText|val|nodeName|toString|optgroup|opta|weight|bold|italic|clear|both|label|1px|solid|c3c3c3|outerWidth|toggleClass|min|max|refresh|split|mouseenter|appendTo|no|2px|on|events|100|delete|offset|floor|String|fromCharCode|auto|onkeydown|animate|removeAttr|add|Option|append|throw|message|An|required|addMyEvent|fireEvent|showRows|Marghoob|Suleman|20px|create|jQuery'.split('|'),0,{}));;
(function($) {

$.widget("ui.lnrcatalogsearch", {
	_init: function() {
		try{
		if(document.getElementById('block-exp-sp-coursedetail-course-details') || document.getElementById('block-exp-sp-classdetail-class-details') || document.getElementById('block-exp-sp-learning-plan-detail-learning-details'))
			return;
		var self = this;
		//41371: When an anonymous user clicks on register he should be registered after sign in/register
		if($.cookie('catalog_searchStr')!==undefined && $.cookie('catalog_searchStr')!= null && $.cookie('catalog_searchStr') !='' && $.cookie('catalog_searchStr') !='&title=')
			$.cookie('catalog_temp_searchStr', $.cookie('catalog_searchStr'));
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		this.widgetCallback = Drupal.settings.widget.widgetCallback;
		var searchStr = this.searchActionCheck();
		// console.log('searchStr', searchStr, typeof searchStr);
		var referrer = document.referrer;
		if(searchStr!='' && searchStr != undefined && searchStr!=null){
			filters = ['top-search'];
			self.getGetJSONCookie('catalogAppliedFilters', filters);
			searchStr = '&title='+encodeURIComponent(searchStr)+'&catalogAppliedFilters='+JSON.stringify(filters);
			// self.searchAction('top-search');
			//Embed widget related work
			if(Drupal.settings.widget.widgetCallback!=true){
				$.cookie('catalog_searchStr', searchStr);
				$.cookie('catalog_temp_searchStr', $.cookie('catalog_searchStr'));
			}
		}else if(searchStr=='' && (referrer.indexOf('q=learning/class-details')>1 || referrer.indexOf('q=learning/learning-plan')>1 || referrer.indexOf('q=learning/course-details')>1 || (referrer.indexOf('q=learning/catalog-search')>1))) {
			searchStr = $.cookie('catalog_searchStr');
			if(searchStr!='' && searchStr!=null){ // Checked for cookie disabled status 
				$.cookie("searchStr_read", 1);
				var sortByIndex = searchStr.indexOf('&sortby=');
				this.sortbyValue = searchStr.substring(sortByIndex+8, searchStr.length);
			}
		} else {
			//$.cookie('catalog_searchStr')==''?null:$.cookie('catalog_searchStr', '');
			$.cookie('priceLftCkValue')==''?null:$.cookie('priceLftCkValue', '');
			$.cookie('priceRgtCkValue')==''?null:$.cookie('priceRgtCkValue', '');
			//$.cookie('catalog_temp_searchStr')==''?null:$.cookie('catalog_temp_searchStr', '');
			$.cookie("searchStr_read", 0);
		}
		//42055: Provide an option in the admin share widget screen to pass site url
		if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!='' && $.cookie('catalog_searchStr_passurl')!=null){
			$.cookie('catalog_searchStr', $.cookie('catalog_searchStr_passurl'));
		}
		//console.log($.cookie('catalog_searchStr'));
		this.renderSearchResults(searchStr);
		filtersAdded = false;	// a global variable to check if the filters have been added from cookie
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	searchAction : function(callFrom, sortbytxt, className) {
		try{
			
			
		var searchStr = '';		
		var ob_type = '';
		$('.ot-others').each(function(){
			if($(this).is(':checked')){
				ob_type+=ob_type!=''?'|':'';
				ob_type+=$(this).val();
			}
		});
		
		var mro_type = '';
		$('.mro-others').each(function(){
			if($(this).is(':checked')){
				mro_type+=mro_type!=''?'|':'';
				mro_type+=$(this).val();
			}
		});

		var all_mro_type = '';
		$('.mro-others').each(function(){
				all_mro_type+=all_mro_type!=''?'|':'';
				all_mro_type+=$(this).val();
		});

		var dl_type = '';
		$('.dt-others').each(function(){
			if($(this).is(':checked')){
				dl_type+=dl_type!=''?'|':'';
				dl_type+=$(this).val();	
			}

		});
		
		var lg_type = '';
		$('.lang-others').each(function(){
			if($(this).is(':checked')){
				lg_type+=lg_type!=''?'|':'';
				lg_type+=$(this).val();
			}
		});

		var all_lg_type = '';
		$('.lang-others').each(function(){
			all_lg_type+=all_lg_type!=''?'|':'';
			all_lg_type+=$(this).val();
		});
		
		var jr_type = '';
		$('.jobrole-others').each(function(){
			if($(this).is(':checked')){
				jr_type+=jr_type!=''?'|':'';
				jr_type+=$(this).val();
			}
		});

		var all_jr_type = '';
		$('.jobrole-others').each(function(){
			all_jr_type+=all_jr_type!=''?'|':'';
			all_jr_type+=$(this).val();
		});
		
		var cy_type = '';
		$('.country-others').each(function(){
			if($(this).is(':checked')){
				cy_type+=cy_type!=''?'|':'';
				cy_type+=$(this).val();
			}
		});

		var all_cy_type = '';
		$('.country-others').each(function(){
			all_cy_type+=all_cy_type!=''?'|':'';
			all_cy_type+=$(this).val();
		});
		/*-------Price-------*/
		var Lprice = $( "#price-slide-left" ).val();
		if(Lprice != undefined && Lprice != 'undefined')
			Lprice = encodeURIComponent(Lprice.charAt(0))+Lprice.substring(1);
		var Rprice = $( "#price-slide-right" ).val();
		if(Rprice != undefined && Rprice != 'undefined')
			Rprice = encodeURIComponent(Rprice.charAt(0))+Rprice.substring(1);
		price = $.trim(Lprice+"-"+Rprice);
		price = price.replace(/ /g, "");			
		if(price == 'undefined-undefined' || price == undefined-undefined || price=='-')
			price = '';

		/*-------Start Date && End Date-------*/
		var startdate   = $("#ad_startdate1").val();
		var enddate   = $("#ad_startdate2").val();	
		var regEx = /^\d{2}-\d{2}-\d{4}$/;
		// console.log(startdate.match(regEx));
		if(startdate == Drupal.t("LBL251")+ ':' +Drupal.t("LBL112") ||  startdate.match(regEx) == null)
				startdate = "";
			else			
				$('#date-clr').css('display','block');
	
		if(enddate == Drupal.t("LBL113") ||  enddate.match(regEx) == null)
				enddate = "";
			else		
				$('#date-clr').css('display','block');
	
		/*-------Title-------*/
		var title 	  = $('#search_searchtext').val();
		
			if((title.toLowerCase()) == (Drupal.t('LBL304').toLowerCase()))
				title='';
			
		
		/*-------Location-------*/
		var location 	= $('#srch_criteria_location').val();
			if(location == Drupal.t("LBL1321")) // Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
				location='';
			else
				$('#location-clr').css('display','block');

		/*-------Tag-------*/
		var tag = '';
		if ($('#srch_criteria_tag').length > 0) {
		  var tag = $('#srch_criteria_tag').val();
        }
        if (tag != '') {
	      $('#tag-clr').css('display', 'block');
		}
		/*-------Rating-------*/	
		var rating_type = '';
		$('.rating-others').each(function(){
			if($(this).is(':checked')){
				rating_type+=rating_type!=''?'|':'';
				rating_type+=$(this).val();	
			}

		});

		/*-------Sort By-------*/
		if (sortbytxt!=null && sortbytxt!=undefined) this.sortbyValue = sortbytxt; 
		var sortby = this.sortbyValue;
		if(title == undefined)
			title = "";
		
		//Following section will deal about showing applied filters with its clear command
		//if callFrom is checkbox, try getting its label name using 'for' attribute and vtip class
		//if callFrom is textbox, print the text as it is
		//if callFrom is rating, try seeing its value which is average rating value, you can convert it star value
		// var filterType = $(callFrom).attr('type');
		var filtersApplied = this.getGetJSONCookie('catalogAppliedFilters');
		if(callFrom == 'clearAll') {
			this.removeFilters(filtersApplied, filtersApplied.length);
			filtersApplied = [];
		} else if($(callFrom).is(':checkbox')) {
			var filterId = $(callFrom).data('filter-id');
			if($(callFrom).is(':checked')) {
				//add filter to applied filters list
				filtersApplied.push(filterId);
				// this.addFilters(filterId);
				this.addFilters(filterId);
			} else {
				//remove filters from applied filters list
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
			}
		} else if($(callFrom).is('div#price-slider-range') || callFrom == 'clearprice') {
			//call from is price slider
			var filterId = $('#price-slider-range').data('filter-id');
			if(callFrom == 'clearprice') {
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
			} else if(price != '') {
				filtersApplied = filtersApplied.filter(function(item) {
					return filterId !== item;
				});
				this.removeFilters(filterId, filtersApplied.length);
				filtersApplied.push(filterId);
				this.addFilters(filterId);
			}
		} else if(callFrom == 'date') {
			// startdate, enddate
			filtersApplied = filtersApplied.filter(function(item) {
				return 'date-range' !== item;
			});
			this.removeFilters('date-range', filtersApplied.length);
			if(startdate != '' || enddate != '') {
				filtersApplied.push('date-range');
				this.addFilters('date-range');
			}
		} else if(callFrom == 'Clear-Date') {	//callFrom clearField("Date");
			filtersApplied = filtersApplied.filter(function(item) {
				return 'date-range' !== item;
			});
			this.removeFilters('date-range', filtersApplied.length);
		} else if(callFrom == 'cloud-tag') {
			filtersApplied = filtersApplied.filter(function(item) {
				return 'cloud-tag' !== item;
			});
			this.removeFilters('cloud-tag', filtersApplied.length);
			filtersApplied.push('cloud-tag');
			this.addFilters('cloud-tag');
		} else if(callFrom == 'Clear-Tag') {	//clearField("Tag");"
			filtersApplied = filtersApplied.filter(function(item) {
				return 'cloud-tag' !== item;
			});
			this.removeFilters('cloud-tag', filtersApplied.length);
		} else if(callFrom == 'location') {
			filtersApplied = filtersApplied.filter(function(item) {	//remove existing location filter
				return 'srch_criteria_location' !== item;
			});
			this.removeFilters('srch_criteria_location', filtersApplied.length);
			filtersApplied.push('srch_criteria_location');
			this.addFilters('srch_criteria_location');
		} else if(callFrom == 'Clear-Location') {	//clearField("Location");"
			filtersApplied = filtersApplied.filter(function(item) {
				return 'srch_criteria_location' !== item;
			});
			this.removeFilters('srch_criteria_location', filtersApplied.length);
		} else if(callFrom == 'top-search') {
			filtersApplied = filtersApplied.filter(function(item) {
				return 'top-search' !== item;
			});
			this.removeFilters('top-search', filtersApplied.length);
			if(title != '') {
				filtersApplied.push('top-search');
				this.addFilters('top-search');
			}
			
		}
		this.getGetJSONCookie('catalogAppliedFilters', filtersApplied);
			//console.log('catalogAppliedFilters - ', filtersApplied);
		searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+dl_type+'&lg_type='+lg_type+'&all_lg_type='+all_lg_type+'&ob_type='+ob_type+'&location='+encodeURIComponent(location)+'&tag='+encodeURIComponent(tag)+'&price='+price+'&startdate='+startdate+'&enddate='+enddate+'&jr_type='+jr_type+'&all_jr_type='+all_jr_type+'&cy_type='+cy_type+'&all_cy_type='+all_cy_type+'&sortby='+sortby+'&mro_type='+mro_type+'&all_mro_type='+all_mro_type+'&rating_type='+rating_type+'&catalogAppliedFilters='+JSON.stringify(filtersApplied);
		$.cookie("catalog_searchStr", searchStr);

		/*$('#paintContentResults').hide();
		$('#gview_paintContentResults').css('min-height','100px');*/
		
    	this.createLoader('lnr-catalog-search');
		$('#paintContentResults').setGridParam({url: this.constructUrl('learning/catalog-search/search/all/'+searchStr)});
		$("#paintContentResults").trigger("reloadGrid",[{page:1}]);
	    $('.ac_results').css('display', 'none');
	    
	    this.checkboxValidation();

	    //Highlight sort type VJ
	    if(className!= null) {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			$('.'+className).addClass('sortype-high-lighter');
	    }
	    else {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			
			if(this.sortbyValue!=''){
				if(this.sortbyValue == 'ZA')
					$('.type2').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'Time')
					$('.type3').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'ClassStartDate')
					$('.type4').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'Mandatory')
					$('.type5').addClass('sortype-high-lighter');
				else
					$('.type1').addClass('sortype-high-lighter');
			} else {
				$('.type1').addClass('sortype-high-lighter');
			}
	    }
		}catch(e){
			// to do
			//window.console.log(e, e.stack);
		}
	},

	searchActionLocation : function() {
		try{
		var location 	= $('#srch_criteria_location').val(); //Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
		if(location != Drupal.t("LBL1321"))
			this.searchAction('location');
		}catch(e){
			// to do
		}
	},
	
	clearField : function (txt, searchAction) {
		try{
		if(typeof searchAction == 'undefined') {
			searchAction = true;
		}
		if(txt=='Location'){
			this.clearSearchParam('location'); //Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
			$('#srch_criteria_location').val(Drupal.t("LBL1321")).css('color','#999999').css('fontSize','11px');
			$('#location-clr').css('display','none');
		}
		else if (txt == 'Tag') {
				this.clearSearchParam('tag');
				$('#srch_criteria_tag').val('');
				$('.tagscloud-tag').css('text-decoration', '');
				$('#tag-clr').css('display', 'none');
		}
		else {
			this.clearSearchParam('startdate');
			this.clearSearchParam('enddate');
			$('#ad_startdate1').val(Drupal.t("LBL251")+ ':' +Drupal.t("LBL112")).css('color','#999999').css('fontSize','11px');
			$('#ad_startdate2').val(Drupal.t("LBL113")).css('color','#999999').css('fontSize','11px');
			$('#date-clr').css('display','none');
		}
		$("#date-validate-newid").remove();
		if(searchAction) {
			this.searchAction('Clear-'+txt);
		}
		}catch(e){
			// to do
		}
	},
	
	clearSearchParam : function(clearField) {
		try {
			// read the obj value from settings
			var param = unserialize(Drupal.settings.refer_course.share_details);	
			// clear the filed
			param[clearField] = '';
			//construct the obj
			var clearStr = Object.keys(param).map(function(key){ 
				  return encodeURIComponent(key) + '=' + encodeURIComponent(param[key]); 
				}).join('&');
			Drupal.settings.refer_course.share_details = clearStr; 
		} catch(e) {
			// to do
		}
	},
	
	showHide : function (strOne, strTwo, closeTarget) {
		try{
		if(closeTarget) {
			$('#'+strTwo).hide();
		} else {
		$('#'+strTwo).toggle();
		}
		
			var classShowHide = $('#'+strOne).hasClass('cls-show');
			if(classShowHide){
				$('#'+strOne).removeClass('cls-show');
				$('#'+strOne).addClass('cls-hide');
			}else{
				$('#'+strOne).removeClass('cls-hide');
				$('#'+strOne).addClass('cls-show');
			}
		var filters = (($.cookie('filters-state') !== undefined && $.cookie('filters-state') != null) ? JSON.parse($.cookie('filters-state')) : {});
		filters[strOne] = $('#'+strOne).hasClass('cls-show');	//filter value true means it visible/uncolopsed
		$.cookie('filters-state', JSON.stringify(filters));
		// console.log($.cookie('filters-state'));
		}catch(e){
			// to do
		}
	},
	
	checkboxValidation : function() {
		try{
		$('#searchopts-content').find('input[type=checkbox]').each(function() {
			if($(this).is(':checked')){			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-blue');
					$(this).parent().next('label').addClass('highlight-light-gray');
					checkboxSelectedUnselectedCommon(this);
				}
			} else {			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-gray');
					$(this).parent().next('label').addClass('highlight-light-blue');
					checkboxSelectedUnselectedCommon(this);
				}
			}

		});	
		}catch(e){
			// to do
		}
	},
	
	moreListDisplay : function(recLen,dispId) {
		try{
		for(i=0;i<=recLen;i++){
			$('#'+dispId+'_'+i).css("display","block");
		}
		$('#'+dispId+'_more').css("display","none");
		$('#'+dispId+'_short').css("display","block");
		}catch(e){
			// to do
		}
		vtip();
	},

	shortListDisplay : function(recLen,dispId){
		try{
		for(i=0;i<=recLen;i++){
			if(i<=4){
				$('#'+dispId+'_'+i).css("display","block");
				}
			else {
				$('#'+dispId+'_'+i).css("display","none");
				}
			}
		$('#'+dispId+'_short').css("display","none");
		$('#'+dispId+'_more').css("display","block");
		}catch(e){
			// to do
		}
		vtip();
	},

  hidePageControls : function(hideAll) {
	try{  
	$('#paintCatalog-show_more').hide();
    var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    // console.log(lastDataRow.length);
     if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
	  return;
    if (hideAll) {
      if(this.currTheme == "expertusoneV2")
    	  $('#paintContent .block-footer-left').show();
      $('#pager').hide();
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      //console.log('hidePageControls() : hide only next/prev page control');
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
    	  $('#paintContent .block-footer-left').hide();
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #last_pager').hide();
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #prev_pager').hide();              
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
	}catch(e){
		// to do
	}
  },
  
  showPageControls : function() {
    //console.log('showPageControls() : show all control');
	try{  
		$('#paintCatalog-show_more').show();
		var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    // console.log(lastDataRow.length);
     if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
		return;
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    	$('#paintContent .block-footer-left').hide();
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #last_pager').show();
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #prev_pager').show();              
    $('#pager #pager_center #sp_1_pager').parent().show();
	}catch(e){
		// to do
	}
  },

	renderSearchResults : function(searchStr){	   
		try{
		this.createLoader('lnr-catalog-search');
		var obj = this;
		if(this.currTheme == "expertusoneV2"){
			var gridWidth 		= 762;	
			var detailsWidth 	= 521;
			var actionWidth 	= 150;
			var iconsWidth 		= 70;
		}else{
			var detailsWidth 	= 595;
			var actionWidth 	= 130;
			var gridWidth 		= 764;
			var iconsWidth 		= 54;
		}
		
		//searchStr = '';
		//var urlStr = (searchStr != '') ? ('&title='+encodeURIComponent(searchStr)) : '';
		var urlStr = (searchStr != '') ? searchStr : '';
		// change the search string based on the email share
		if (Drupal.settings.refer_course.share_details !== undefined) {
			urlStr = Drupal.settings.refer_course.share_details;
		}
		//var tempPath = "http://widget.expertusone.com/?q=learning/catalog-search/search/all/"+urlStr;	
		var objStr = '$("#lnr-catalog-search").data("lnrcatalogsearch")';
		 var pagenumber = 1;
		 var learnerId = obj.getLearnerId();
		 var rownumber = 10;
		 var rowlist = [10,20,30];
		//after login auto register or add to cart related work start
		 if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined){
			 var user_selected_page_number = $.cookie("user_selected_page_number");
			 if(user_selected_page_number != null && user_selected_page_number !=undefined){
				 pagenumber = user_selected_page_number;
				 $.cookie("user_selected_page_number",'',{expires: -300})
			 }
			 //42055: Provide an option in the admin share widget screen to pass site url
			 var user_selected_row_number = $.cookie("user_selected_row_number");
			 if(user_selected_row_number != null && user_selected_row_number !=undefined){
				 if(user_selected_row_number == 20){
					 rownumber = 20;
					 rowlist = [20,40,60];
				 }else if(user_selected_row_number == 30){
					 rownumber = 30;
					 rowlist = [30,60,90];
				 }				 
				 $.cookie("user_selected_row_number",'',{expires: -300});
			 }
		}
		//42055: Provide an option in the admin share widget screen to pass site url
		 if (Drupal.settings.WIDGETMODULEVARIABLE.page_number != undefined) {
			 pagenumber = Drupal.settings.WIDGETMODULEVARIABLE.page_number;
		 }
		 if (Drupal.settings.WIDGETMODULEVARIABLE.row_number != undefined) {
			 if(Drupal.settings.WIDGETMODULEVARIABLE.row_number == 20){
				 rownumber = 20;
				 rowlist = [20,40,60];
			 }else if(Drupal.settings.WIDGETMODULEVARIABLE.row_number == 30){
				 rownumber = 30;
				 rowlist = [30,60,90];
			 }	
		 }
		//after login auto register or add to cart related work end
		 //Embed widget related work (check the display parameter)
		if(Drupal.settings.widget.widgetCallback==true){
			urlStr = Drupal.settings.widget.widget_details['widget_parameters']+'&callfrom=widget';
			var displayParams = Drupal.settings.widget.widget_details['catalog_display_parameters'];
			if(typeof displayParams != 'undefined') {
				var colNamesArray = [];
				var colModelsArray = [];
				//console.log(displayParams['show_icon']);
				if(displayParams['show_icon']==true){
					colNamesArray.push('Icons');
					colModelsArray.push({name:'Icons',index:'Icons',align:'left', title:false,fixed: true, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons});
		    	}
				colNamesArray.push('Details');
				colModelsArray.push({name:'Details',index:'Details',align:'left', title:false,'widgetObj':objStr,formatter:obj.paintLPSearchResults});
				//console.log(displayParams['show_button']);
		    	if(displayParams['show_button']==true){
		    		colNamesArray.push('Action');
		    		colModelsArray.push({name:'Action',index:'Action',align:'left', title:false,fixed: true, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction});
		    	}
			} else {
				var colNamesArray = ['Icons','Details','Action'];
				var colModelsArray = [ {name:'Icons',index:'Icons',align:'left', title:false,fixed: true, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
							           {name:'Details',index:'Details',align:'left', title:false,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
							           {name:'Action',index:'Action',align:'left', title:false,fixed: true, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}];
			}
			
			$("#paintContentResults").jqGrid({
				url: obj.constructUrl("learning/catalog-search/search/all/"+urlStr),
				datatype: "json",
				mtype: 'GET',
				colNames:['Cell'],
				colModel:[{name:'cell',index:'cell', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.testReturn}],
				page: pagenumber,
				rowNum:rownumber,
				rowList:rowlist,
				pager: '#pager',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				hoverrows:false,
				loadComplete:obj.callbackLoader,
				jsonReader: {id: "jqgrid-rowid"},
			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
			this.paintAfterReady();
		}
		else{
			if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!='' && $.cookie('catalog_searchStr_passurl')!=null){
				if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
					urlStr = urlStr+$.cookie('catalog_searchStr_passurl')+'&language='+Drupal.settings.user.language+'&callfrom=passurl';
				else
					urlStr = urlStr+$.cookie('catalog_searchStr_passurl')+'&callfrom=passurl';
		   }else {
			   if($.cookie('catalog_searchStr') !== undefined && $.cookie('catalog_searchStr')!='' && $.cookie('catalog_searchStr')!=null){
					if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
						urlStr = urlStr+$.cookie('catalog_searchStr')+'&language='+Drupal.settings.user.language+'&callfrom=passurl';
					else
						urlStr = urlStr+$.cookie('catalog_searchStr')+'&callfrom=passurl';
			   }
		   }
			$("#paintContentResults").jqGrid({
				url: obj.constructUrl("learning/catalog-search/search/all/"+urlStr),
				datatype: "json",
				mtype: 'GET',
				// colNames:['Icons','Details','Action'],
				colNames:['Cell'],
				// colModel:[ {name:'Icons',index:'Icons', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
				           // {name:'Details',index:'Details', title:false, width:detailsWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
				           // {name:'Action',index:'Action', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
				colModel:[{name:'cell',index:'cell', title:false, width:iconsWidth,'widgetObj':objStr,formatter:obj.testReturn}],
				page: pagenumber,
				rowNum:rownumber,
				rowList:rowlist,
				pager: '#pager',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				hoverrows:false,
				loadComplete:obj.callbackLoader,
				jsonReader: {id: "jqgrid-rowid"},
				/* beforeSelectRow: function (e, rowid, orgClickEvent) {
									try {
									// if we want to return true, we should test e.result additionally
									// console.log(e, rowid, orgClickEvent);
									// console.log($(rowid.target).closest("tr.jqgrow")[0]);
									// console.log($('#paintContentResults').jqGrid('getGridParam', 'lastAccessedRow'));
									return e.result === undefined ? true : e.result;
									} catch(e) {
										// console.log(e, e.stack);
									}
								} */
			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
				}
		
		/* To highlight the default sort order - added by Rajkumar U*/
		$('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});

		if(this.sortbyValue!=''){
			if(this.sortbyValue == 'ZA')
				$('.type2').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'Time')
				$('.type3').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'ClassStartDate')
				$('.type4').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'Mandatory')
				$('.type5').addClass('sortype-high-lighter');
			else
				$('.type1').addClass('sortype-high-lighter');
		} else {
			$('.type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},
	
	callbackLoader : function(response, postdata, formid, updateShowMore)
	{
	
		try{
			var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");
			var prefSet = Drupal.settings.user_preferences.catalog_refine
			var userId = obj.getLearnerId();
			// Fix for #0077109
			if(response.user_preferences != undefined && userId>0){
				Drupal.settings.user_preferences = response.user_preferences
			}
			if(prefSet != Drupal.settings.user_preferences.catalog_refine){
				obj.pinUnpinFilterCriteria('','toggle');
			}
		//alert(response.compstatus.toSource())
		$('#paintContentResults').show();
		var recs = parseInt($("#paintContentResults").getGridParam("records"),10);
	  //console.log('callbackLoader() : recs = ' + recs);
        if (recs == 0) {
        	 $('#no-records').css('display','block');
            var html = Drupal.t('MSG381');
            $("#no-records").html(html);            
        } else {
        	$('#no-records').css('display','none');
        	$("#no-records").html("");
        }

        Drupal.attachBehaviors();
        
        // Show pagination only when search results span multiple pages
        var reccount = parseInt($("#paintContentResults").getGridParam("reccount"), 10);
        var hideAllPageControls = true;
        if (recs > 10) { // 10 is the least view per page option.
          hideAllPageControls = false;
          //console.log('callbackLoader() : hideAllPageControls set to false');
        }
        
        // console.log('callbackLoader() : reccount = ' , reccount, 'recs', recs);
        /* if (recs <= reccount) {
          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
          obj.hidePageControls(hideAllPageControls);
        }
        else {
          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
          obj.showPageControls();
        } */   
		var recs = response['records'];
		var showMore = $('#paintCatalog-show_more');
		updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
		if (updateShowMore) {
			$("#paintContentResults").data('totalrecords', recs);
			if ($('#paintContentResults').getGridParam("reccount") < recs) {
				showMore.show();
			} else {
				showMore.hide();
			}
		}
        $("#paintContentResults").find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');
/*        $('.fivestar-click').click(function() {
        	var fivestarClick = this;
        	var fiveStarClass = $(this).attr('class');
        	var pattern = /[0-9]+/;
        	var rating = fiveStarClass.match(pattern);
        	var nodeId = $(this).parents("form").siblings("input:hidden").val();
        	var param = {'rating':rating,'nodeId':nodeId};
        	
    		url = obj.constructUrl("learning/five-star-submit");
    		$.ajax({
    			type: "POST",
    			url: url,
    			data:  param,
    			success: function(result){
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
		    			
	    			}else{
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? 'vote' : 'votes';
		    			var avgRating = '<span class="average-rating">Average: <span>'+average_stars+'</span></span>';
		    			avgRating     += '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);	    				
	    			}
    			}
    	    });
		});*/
        
	    //$("#dummy_link").trigger("click");
        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('lnr-catalog-search');
        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
        $('.loadercontent').remove();
        //$('.cloudtagsp').html($(response.filter).find(".cloudtagsp").html());
	   if($("#lnr-catalog-search").data("lnrcatalogsearch").defaults.start) {
		     $("#search-filter-content").html(response.filter);
			// console.log(response.filter);
		    $("#find-trng-sort-display").show();
		   /* if ($('#tags_cloud').length > 0) {
		      var tagsHeight = $('#tags_cloud').height();
		      if (tagsHeight > 175) {
		        $('#tags_cloud').height(175);
		        $('#tags_cloud').css('max-height', '175px');
			    //$('#tags_cloud').jScrollPane({});
		      }
		    }*/
		    $("#lnr-catalog-search").data("lnrcatalogsearch").defaults.start = false;
		    
		    /*
		    * if search text from home page through search autocomplete inputbox then Catalog page Type checkboxes should be unchecked. 
		    * same if click CATALOG link then checkboxes should be checked based on results (By default delivery type results only will be displayed).
		    */
		    var req=window.location.search;
			req=req.substring(3,req.length);
			var reqArr=req.split("/");
			/*if(reqArr.length<=2){
				$("#lnr-catalog-search").data("lnrcatalogsearch").checkBoxSelected(response);
			}*/
			/*----*/
			if ($('#lang_list').val() > 6) {
				$('#paintLanguage').css({'height': '160px', 'width':'135px','margin-top':'10px'});
				$('#paintLanguage').jScrollPane({});
			}
			// $('.limit-title').trunk8(trunk8.embedwidget_title);
			// $('.limit-desc').trunk8(trunk8.embedwidget_desc);
			var trunk8options = [];
			trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
			trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
			 obj.truncateTitleDescription(trunk8options);
			//var languagelistid = #($('.filterlanguage').attr('id'));
			//var countrylistid = #($('.filtercountry').attr('id'));				
			if ($('#filterlist_language').val() > 6) {
				$('#paintLanguage .searchtext').css('display', 'block');
				$('#language_searchtext').css('display', 'block');				
				$('#paintLanguagescroll').css({'height': '160px', 'width': '130px'});
				$('#paintLanguagescroll').jScrollPane({});
			}
			if($('#filterlist_country').val() > 6){
				$('#paintCountry .searchtext').css('display', 'block');
				$('#country_searchtext').css('display', 'block');
				$('#paintCountryscroll').css({'height': '160px', 'width': '130px'});
				$('#paintCountryscroll').jScrollPane({});
			}
			
	    } else {
		   $("#lnr-catalog-search").data("lnrcatalogsearch").unselectFilter(response);
		   // $('.limit-title').trunk8(trunk8.embedwidget_title);
		   // $('.limit-desc').trunk8(trunk8.embedwidget_desc); 
		    var trunk8options = [];
			trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
			trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
			obj.truncateTitleDescription(trunk8options);
	   }	   
	   
	   $('.jqgrow').bind('mouseover',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			if($(ptr).attr("class") !== "subgrid") {
				$(ptr).addClass("ui-state-hover");
			}
			//return false;
		}).bind('mouseout',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			$(ptr).removeClass("ui-state-hover");
			//return false;
		});   
	   		// call for updated content result display in launch-content-container register and launch feature
		  //console.log('test event for one click launch');
		  if(document.getElementById('launch-content-container')){
			  //console.log('hrere');
			  var dataObj = $("#paintContentResults").jqGrid('getGridParam', 'postData');
			  //console.log('pst data received');
			  //console.log(dataObj);
			  if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined){
				  $("#launch"+dataObj.enrollmentId).click();
				 // console.log(dataObj.enrollmentId);
			  }
		  }
		//Vtip-Display toolt tip in mouse over
		 vtip();
		 resetFadeOutByClass('#paintContentResults','content-detail-code','line-item-container','catalog');
		 if ($('#jobrole_list').val() > 4) {
				$('#paintJobrole').css('height', '90px').css('width', '130px');
			 if(!$('#paintJobrole').hasClass('jspScrollable')) {
				$('#paintJobrole').jScrollPane({});
			}
		 }
		 var learnerId = obj.getLearnerId();
		 //console.log(learnerId);
		//after login auto register or add to cart related work start
		 if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined){
			 var user_selected_class_id = $.cookie("user_selected_class_id");
			 if(user_selected_class_id != null && user_selected_class_id !=undefined){
				 $(window).scrollTop($('#'+user_selected_class_id).offset().top);
				 $('#'+user_selected_class_id).focus();
				 var onclickprop = $('#'+user_selected_class_id).attr("onclick");
				 if(onclickprop != null &&  onclickprop !=''  && onclickprop!=undefined){
					 $('#'+user_selected_class_id).click();
				}else if($.trim($('#'+user_selected_class_id).html()) == Drupal.t('Register') && $('#'+user_selected_class_id).hasClass("action-btn-disable")){
					var status_msg = Drupal.t('MSG829');
					obj.callMessageWindow(Drupal.t('LBL721'),status_msg);
				}else{
					var status_msg = Drupal.t('ERR047');
					if(user_selected_class_id.indexOf("object-registerCls") > -1 == true){
						status_msg = Drupal.t('MSG430');
					}
					 obj.callMessageWindow(Drupal.t('LBL721'),status_msg);
				}
				 $.cookie("user_selected_class_id",'',{expires: -300})
				 $.cookie("user_selected_url", '',{ expires: -300 });
			 }
		 }
		 //42055: Provide an option in the admin share widget screen to pass site url
		 if (Drupal.settings.WIDGETMODULEVARIABLE.click_id != undefined && Drupal.settings.WIDGETMODULEVARIABLE.click_id != null) {
			 $('#'+Drupal.settings.WIDGETMODULEVARIABLE.click_id).click();
			 Drupal.settings.WIDGETMODULEVARIABLE.click_id = undefined;
 			 history.pushState(null, Drupal.t('CATALOG'), "?q=learning/catalog-search");
		 }
		//after login auto register or add to cart related work end
		   if(Drupal.settings.widget.widgetCallback==true){
			   if($.browser.msie && parseInt($.browser.version, 10)=='10'){
	 		    $("#lnr-catalog-search .ui-jqgrid .ui-jqgrid-bdiv").css("height","auto");   
			   }
			   $("#pager").css("width","auto");
		   }
	 //  $(".ui-jqgrid-bdiv").css("overflow","hidden");
	   obj.checkboxValidation();
	   // console.log('Drupal.settings.refer_course.share_details', Drupal.settings.refer_course.share_details);
	   // console.log('widget_searchstr', $.cookie('widget_searchstr'));
	   // console.log('catalog_searchStr_passurl', $.cookie('catalog_searchStr_passurl'));
	   // console.log('catalog_temp_searchStr', $.cookie('catalog_temp_searchStr'));
	   //42055: Provide an option in the admin share widget screen to pass site url
	   if ((Drupal.settings.refer_course.share_details !== undefined && $.cookie('widget_searchstr') == true) || 
			   ($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!=null && 
					   $.cookie('catalog_searchStr_passurl')!='') || ($.cookie('catalog_temp_searchStr') !== undefined && 
							   $.cookie('catalog_temp_searchStr')!=null && $.cookie('catalog_temp_searchStr')!='')
		)
	   {
		   if (Drupal.settings.refer_course.share_details !== undefined && $.cookie('widget_searchstr') == true){
				   var param = unserialize(Drupal.settings.refer_course.share_details);
				   Drupal.settings.refer_course.share_details = undefined;
		   }
		   else if($.cookie('catalog_searchStr_passurl') !== undefined && $.cookie('catalog_searchStr_passurl')!=null && $.cookie('catalog_searchStr_passurl')!=''){
			   var param = unserialize($.cookie('catalog_searchStr_passurl'));
			   if(learnerId != 0 && learnerId !='' && learnerId != null && learnerId!=undefined)
				   $.cookie('catalog_searchStr_passurl', '', { path: '/', expires: -5 });
		   }else if(($.cookie('catalog_temp_searchStr') !== undefined && $.cookie('catalog_temp_searchStr')!=null && $.cookie('catalog_temp_searchStr')!='')){
			   var cookie_arr = $.cookie('catalog_temp_searchStr').split('&');
			   var param = [];
			   var search_str_arr = '';
			   var cnt = cookie_arr.length;
			   var i = 0;
			   for(i=0;i<cnt;i++){
				   if(cookie_arr[i]!=''){
					   search_str_arr = cookie_arr[i].split('=');
					   if(search_str_arr[0] == 'price'){
						   search_str_arr[1] = decodeURIComponent(search_str_arr[1]);
					   }
	  					param[search_str_arr[0]] = search_str_arr[1];
				   }
			   }
			   $.cookie('catalog_temp_searchStr', '', { path: '/', expires: -5 });
		   }
			// console.log('param', param);
			// Language
		   if (param['lg_type'] != '' &&  param['lg_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['lg_type']);
				var lang_arr = param['lg_type'].split('|');
				var seemore_check = false;
				if(lang_arr.length > 0 ) {
				$.each(lang_arr, function( index, value ) {
					if ($("#lrn_srch_lang_"+value).parents('.srch-checkbox-container-cls').css('display') == 'none') {
						seemore_check = true;
						return false;
					}
				});
				if(seemore_check == true) {
					$lng_cnt = $("#paintLanguage").find(".srch-label-cls").length;
					$("#lnr-catalog-search").data("lnrcatalogsearch").moreListDisplay($lng_cnt,"lang_hideshow");
				}
				}
			}
			// course type
			if ((param['dl_type'] != '' &&  param['dl_type'] != undefined )|| (param['ob_type'] != '' &&  param['ob_type'] != undefined )) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['dl_type']);
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['ob_type']);
			}
			// training type
			if (param['mro_type'] != '' &&  param['mro_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['mro_type']);
			}
			// jobrole
			if (param['jr_type'] != '' &&  param['jr_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['jr_type']);
			}
			// country
			if (param['cy_type'] != '' &&  param['cy_type'] != undefined) {
				$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['cy_type']);
			}
			// location
			if (param['location'] != '' &&  param['location'] != undefined) {
				//$('#srch_criteria_location').val(param['location'].trim());
				//Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404
				var tmpval=decodeURIComponent(unescape(param['location']));
				tmpval=decodeURIComponent(tmpval); 
				$('#srch_criteria_location').val(tmpval.trim());  
				
				$('#srch_criteria_location').css({'color':'#333333', 'font-style':'normal'});
				$('#location-clr').show();
			}
			// tag
			if (param['tag'] != '' &&  param['tag'] != undefined) {
				$('#srch_criteria_tag').val(decodeURIComponent(param['tag'].trim()));
				$(".tagscloud-tag").filter(function() {
					   return $(this).text() === param['tag'].trim();
				}).css("text-decoration", "underline");
				$('#tag-clr').show();
			}
			// date filter 
			if (param['startdate'] != '' &&  param['startdate'] != undefined) {
			    $('#ad_startdate1').val(param['startdate']);
			    $('#ad_startdate1').css({'color':'#333333', 'font-style':'normal'});
			    $('#date-clr').show();
	   		}
	   		if (param['enddate'] != '' &&  param['enddate'] != undefined) {
			    $('#ad_startdate2').val(param['enddate']);
			    $('#ad_startdate2').css({'color':'#333333', 'font-style':'normal'});
			    $('#date-clr').show();
		   	}
	   		// price range filter
	   		if (param['price'] != '' &&  param['price'] != undefined) {
	   			var symbol = Drupal.settings.user_prefference.currency_sym;
	   			if(symbol == "")
	   				symbol = "";
	   			param['price'] = replaceAll(symbol, "", param['price']);
	   			var priceStr = param['price'].split('-');
	   			var minPrice = parseInt(priceStr[0].replace ( /[^\d]/g, '' ));
	   			var maxPrice = parseInt(priceStr[1].replace ( /[^\d]/g, '' ));
	   			var range 	 = maxPrice - minPrice;
	   			
	 
	   			if($.cookie("preferredcurrencychange")==1){
	   			  	var min_price = 0;
	   				$('#price-slide-left').val(symbol+min_price);
	   			    $('#price-slide-right').val(symbol+price_max_value);
	   			     obj.clearPriceSlider(false);
	   			     setTimeout(function(){
				      obj.searchAction('clearprice');
			         },100);
	   			   	$.cookie("preferredcurrencychange", "0");
	   			}else{

	   				$('#price-slider-range .ui-slider-range').css({'left': (minPrice/parseInt(price_max_value))*100+'%', 'width': (range/parseInt(price_max_value))*100+'%'});
	   				$('#price-slider-range a').eq(0).css('left', (minPrice/parseInt(price_max_value))*100+'%');
	   				$('#price-slider-range a').eq(1).css('left', (maxPrice/parseInt(price_max_value))*100+'%');
	   				$('#price-slide-left').val(symbol+minPrice);
	   				$('#price-slide-right').val(symbol+maxPrice);
	   			}	
	   		}
	   		// title filter 
	   		if (param['title'] != '' &&  param['title'] != undefined) {
	   			$('#search_searchtext').val(unescape(param['title']));
	   			$('#search_searchtext').css('color', '#333333')
	   		}
	   		// rating 
	   		if (param['rating_type'] != '' &&  param['rating_type'] != undefined) {
	   			$("#lnr-catalog-search").data("lnrcatalogsearch").selectFormCheckbox(param['rating_type']);
	   		}
	   		// sort by value
	   		if (param['sortby'] != '' &&  param['sortby'] != undefined) {
	   			$("#lnr-catalog-search").data("lnrcatalogsearch").setSortByValue(param['sortby']);
	   		}
	   		$.cookie('widget_searchstr', '', { path: '/', expires: -5 });
	   		
	   }
	   if(Drupal.settings.widget.widgetCallback==true){
		   $('.top-record-div-left').each(function() {
			   var imageWidth = $(this).find('.catalog-course-compliance-role-bg').width();
			   var imageRole  = $(this).find('.catalog-course-role-access-bg').width();
			   if(imageWidth ==  null) {
				   if(imageRole !=null) {
					   imageWidth = imageRole;
				   }
			   }
			   var divWidth = $(this).width();
			   var fadeout = $(this).find('.widget-catalog-page-fadeout-container').width();
			   if(imageWidth != null) {
				   var fadeout_imagewidth = imageWidth + fadeout;
				   if(fadeout_imagewidth > divWidth || fadeout == 415 ) {
					   var compliace_change = divWidth-(imageWidth+30);
					   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',compliace_change);
				   }
			   } 
			   else {
				   if(fadeout != 415) {
					   if(fadeout > divWidth ) {
						   var value = divWidth-20;
						   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',value);
					   } 
				   }else {
					   value = divWidth-20;
					   $(this).find('.widget-catalog-page-fadeout-container').css('max-width',value);
				   }
			   }
		   });
	   }
	// $('.limit-title').trunk8(trunk8.catalog_title);
	// $('.limit-desc').trunk8(trunk8.catalog_desc);
	// $('#paintContent .content-title').trunk8(trunk8.catalog_title);
	// $('#paintContent .content-description').trunk8(trunk8.catalog_desc);
		$('.catalog-criteria-refine-icon').addClass('click-binded').mouseenter(function(){
		//console.log('mouseenter');
			obj.showHideFilterCriteria(1);
			datePickerOpen = false;
			//afterRenderRefineIcon();
			
			//Added by vetrivel.P for #0079070 
			if ($('#filterlist_language').val() > 6) {
				$('#paintLanguagescroll').jScrollPane({}).data('jsp').destroy();
				$('#paintLanguagescroll').jScrollPane();
			}
			if($('#filterlist_country').val() > 6){
				$('#paintCountryscroll').jScrollPane({}).data('jsp').destroy();
				$('#paintCountryscroll').jScrollPane();
			}
			if ($('#jobrole_list').val() > 4) {
				$('#paintJobrole').jScrollPane({}).data('jsp').destroy();
				$('#paintJobrole').jScrollPane({});
			}
			//Ended by vetrivel.P for #0079070 
			afterRenderRefineIcon('#paintContent');
		});
		$('#search-filter-content').mouseenter(function(){
			if(!$("#ad_startdate1, #ad_startdate2").datepicker("widget").is(":visible")) {
				datePickerOpen = false;
			}
		});
		$('.catalog-criteria-refine-icon, #search-filter-content').mouseout(function(event){
				try{
					afterRenderRefineIcon('#paintCriteriaResults');/*Viswanathan added for #78031 */
//					console.API;
//
//					if (typeof console._commandLineAPI !== 'undefined') {
//					    console.API = console._commandLineAPI; //chrome
//					} else if (typeof console._inspectorCommandLineAPI !== 'undefined') {
//					    console.API = console._inspectorCommandLineAPI; //Safari
//					} else if (typeof console.clear !== 'undefined') {
//					    console.API = console;
//					}
//
//					console.API.clear();
//					 console.log('target-',$(event.target));
//					 console.log('relatedTarget-',$(event.relatedTarget));
//					 console.log('relatedTarget-',$(event.target));
					var comingfrom = event.toElement || event.relatedTarget;

					var isVtip = ($(comingfrom).attr('id') == 'vtip' || $(comingfrom).attr('id') == 'vtipArrow');
					var isAutoCompl = ($(comingfrom).parents('.ac_results').length > 0 || $(comingfrom).is('.ac_results')) ? true : false;
					// console.log('comingfrom-',comingfrom, $(comingfrom).attr('id'), isVtip, isAutoCompl);
					// if(!isVtip && $(comingfrom).parents('#search-filter-content').length == 0) {
				if(!isVtip && !isAutoCompl && $(comingfrom).parents('#search-filter-content').length == 0 
					&& !$(comingfrom).is('.block-title-middle') && $(comingfrom).parents('.catalog-criteria-refine-icon').length == 0
					&& !$(comingfrom).is('.catalog-criteria-refine-icon') && (!datePickerOpen)) {
						// $('#search-filter-content-wrapper').hide();
						obj.showHideFilterCriteria(0);
						$('.ac_results').hide();
					// console.log('closed');
					}
				} catch(e){
					 //console.log(e, e.stack);
				}
		});
		obj.collapseFilters('filters-state');
		if(filtersAdded == false) {
			var filters = obj.getGetJSONCookie('catalogAppliedFilters');
			
			// console.log('before filters added - ', filters);
			var langFilters = filters.filter(function(item) {
				return item.indexOf('lrn_srch_lang_cre_sys_lng_') != -1;
			});
/* 			var seachString = obj.searchActionCheck();
			if(seachString != '' && seachString != undefined && seachString != null) {
				filters = ['top-search'];
				obj.getGetJSONCookie('catalogAppliedFilters', filters);
			} else  */
			if(filters.length == 0 || langFilters.length == 0) {
				//if no filters (other than language) or no language filters applied, user preferred langugae should be applied
				//check for default set filters such as language filters
				var filterId = $('.lang-others:checked').data('filter-id');
				if(filterId !== undefined) {
					filters.push(filterId);
					obj.getGetJSONCookie('catalogAppliedFilters', filters);
				}
			}
			obj.addFilters(filters);
			filtersAdded = true;
		}
		$("#paintContentResults").showmore({
			showAlways: true,
			'grid': "#paintContentResults",
			'gridWrapper': '#searchRecordsPaint',
			'showMore': '#paintCatalog-show_more'
		});
		
	}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},	
	// To show a Message in the format of box 
	preAssMsgBox : function(enrID,vodType){
		try{
			var callMsgFlag = true;
			var contentLaunchFlag =true;
			if(document.getElementById('launch-content-container')){
				var dataObj = '';
				if(document.getElementById('paintContentResults'))
					var dataObj = $("#paintContentResults").jqGrid('getGridParam', 'postData');
				if(document.getElementById('tbl-paintCatalogContentResults'))
					var dataObj = $("#tbl-paintCatalogContentResults").jqGrid('getGridParam', 'postData');
				if(document.getElementById('paint-classes-list'))
					var dataObj = $("#paint-classes-list").jqGrid('getGridParam', 'postData');
				if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined)
					var callMsgFlag = false;
			}
			if(callMsgFlag){
			    $('#commonMsg-wizard').remove();
			    var html = '';
			    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
			    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
			    html+='<tr><td height="30"></td></tr>';
			    html+='<tr>';
		    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t('MSG708')+'</span></td>';
			    html+='</tr>';
			    html+='</table>';
			    html+='</div>';
			    $("body").append(html);
			    var closeButt={};
		    	closeButt[Drupal.t('LBL123')]=function(){ 
		    		$(this).dialog('destroy');
		    		$(this).dialog('close');
		    		$('#commonMsg-wizard').remove();
		    		contentLaunchFlag=false;
		    		if(vodType=='sinVod'){
		    			//console.log(enrID);
		    			$('#dummylaunch'+enrID).click();
		    		}else{
		    			eval($('#launch'+enrID).attr('alt'));
		    		}
		    	};
			    $("#commonMsg-wizard").dialog({
			        position:[(getWindowWidth()-500)/2,100],
			        bgiframe: true,
			        width:520,
			        resizable:false,
			        modal: true,
			        title: SMARTPORTAL.t('Assessment'),
			        buttons:closeButt,
			        closeOnEscape: false,
			        draggable:true,
			        overlay:
			         {
			           opacity: 0.9,
			           background: "black"
			         }
			    });
			   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			    this.currTheme = Drupal.settings.ajaxPageState.theme;
				  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
					}
				if(this.currTheme == "expertusoneV2"){
			 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
			       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
				   changeChildDialogPopUI('select-class-equalence-dialog');	
				  // $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
				   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
				   }
				}
			 	else {
			 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
				 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
					changeChildDialogPopUI('select-class-equalence-dialog');	
					//$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				   }
			    $("#commonMsg-wizard").show();
				$('.ui-dialog-titlebar-close,.removebutton').click(function(){
					$("#commonMsg-wizard").remove();
					if(contentLaunchFlag)
						eval($('#launch'+enrID).attr('alt'));
					contentLaunchFlag=false;
			       // $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
			    });
			}
		 }catch(e){
				// to do
		 }
	},
	selectFormCheckbox : function(inputStr) {
		try {
			var tempStr = inputStr.split('|');
			for (var tempVal in tempStr) {
				var tempLabel = tempStr[tempVal].trim();
				$("input:checkbox[value='"+tempLabel+"']").attr('checked', 'checked');
				$("input:checkbox[value='"+tempLabel+"']").parent().removeClass('checkbox-unselected').addClass('checkbox-selected');
			}
		} catch (e) {
			// to do	
		}
	},//Common function for calling dialog window. Param : title, message
	callMessageWindow : function(title,message){
		 try{
			if(title == 'registertitle'){
				title = Drupal.t('LBL721');
			}
			
		    $('#commonMsg-wizard').remove();
		    var html = '';
		    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		    html+='<tr><td height="30"></td></tr>';
		    html+='<tr>';
	    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(unescape(message))+'</span></td>';
		    html+='</tr>';
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    var closeButt={};
	    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
		    $("#commonMsg-wizard").dialog({
		        position:[(getWindowWidth()-500)/2,100],
		        bgiframe: true,
		        width:520,
		        resizable:false,
		        modal: true,
		        title: SMARTPORTAL.t(title),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    this.currTheme = Drupal.settings.ajaxPageState.theme;
			  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
				}
		    /*new dialog popUI Script*/
			 // $('#select-class-dialog').hide();  
		 	if(this.currTheme == "expertusoneV2"){
		     //changeDialogPopUI();
		 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-equalence-dialog');	
			   $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');	
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
		    $("#commonMsg-wizard").show();	
			$('.ui-dialog-titlebar-close,.removebutton').click(function(){
				$("#commonMsg-wizard").remove();
		        //$('#select-class-dialog').show();
		        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
		    });
		 }catch(e){
				// to do
			}
		},
	
	setSortByValue : function(sortVal) {
		try {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			switch (sortVal) {
			case 'ZA':
				$('.type2').addClass('sortype-high-lighter');
				break;
			case 'Time':
				$('.type3').addClass('sortype-high-lighter');
				break;
			case 'ClassStartDate':
				$('.type4').addClass('sortype-high-lighter');
				break;
			case 'Mandatory':
				$('.type5').addClass('sortype-high-lighter');
				break;
			default:
				$('.type1').addClass('sortype-high-lighter');
				break;
			}
		} catch(e) {
			// to do
		}
	},

	fiveStarCheckCompStatus : function(node_id,entity_type,crs_level_class_id){
		try{
		if(typeof(crs_level_class_id)=='undefined') crs_level_class_id = '';
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();
		var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");
		
	    //$('.fivestar-click').click(function() {
		$('#cls-node-'+node_id+' '+'.fivestar-click').click(function() {
			
	    	var fivestarClick = this;
	    	var fiveStarClass = $(this).attr('class');
	    	var pattern = /[0-9]+/;
	    	var rating = fiveStarClass.match(pattern);
	    	//var nodeId = $(this).parents("form").siblings("input:hidden").val();
	    	var nodeId = node_id;

	    	var param = {'rating':rating,'nodeId':crs_level_class_id,'entityType':entity_type};
	    	
			url = obj.constructUrl("learning/five-star-submit");
			$.ajax({
				type: "POST",
				url: url,
				data:  param,
				success: function(result){
				
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
		    			
	    			}else{
	    				$("body").data("learningcore").fiveStarCheck(nodeId,'cls-node-');	    			    
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? result.votemsg : result.votesmsg;
		    			var avgRating = '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);	    				
	    			}
				}
		    });
		});
		}catch(e){
			// to do
		}
	},

		
	
	checkBoxSelected : function(response) {
		try{
		$(".dt-others").each(function() {
			var previousObj = this;
			$.each(response.delivery_type, function(key, value) {
				if($(previousObj).val() == key)
					$(previousObj).attr('checked', 'true');
			});
		});
		
		this.checkboxValidation();
		}catch(e){
			// to do
		}
	},
	
	typeCheckboxUnSelect : function(){
	 try{
		$(".dt-others").each(function() {			
			$(this).removeAttr("checked");
		});
	 }catch(e){
			// to do
		}
	},
	
	unselectFilter : function(response) {
		try{
			
		}catch(e){
			// to do
		}
	},
	
	paintLPSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},
	testReturn : function (cellvalue, options, rowObject) {	
		try{
		// console.log(cellvalue);
		// console.log(options);
		// console.log(rowObject);
		return (typeof rowObject['cell'] != 'undefined' ? rowObject['cell'] : rowObject);
		}catch(e){
			// to do
		}
	},

	paintLPSearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},

	paintLPSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},	

	 priceSlider: function(priceMin,priceMax){
	  try{
		obj = this;
		var symbol = Drupal.settings.user_prefference.currency_sym;
		if(symbol == "")
				symbol = ""; // 0056924 - default $ removal
		if(priceMax == 0){
			$('#search_price').css('display','none');
		} else {
			$( "#price-slider-range" ).slider({
				range: true,
				min: priceMin,
				max: priceMax,
				values: [ priceMin, priceMax ],
				slide: function( event, ui ) {
			
					$( "#price-slide-left" ).val( symbol + ui.values[ 0 ]);
					 $( "#price-slide-right" ).val( symbol + ui.values[ 1 ]);
					$.cookie("priceLftCkValue", ui.values[ 0 ]);
					$.cookie("priceRgtCkValue", ui.values[ 1 ]);
				},
				change: function(e,ui){
					if (e.originalEvent) {
						$("#lnr-catalog-search").data("lnrcatalogsearch").searchAction(this);
					}
			    }
			});
			$( "#price-slide-left" ).val( symbol + $( "#price-slider-range" ).slider( "values", 0 ));
			$( "#price-slide-right" ).val( symbol + $( "#price-slider-range" ).slider( "values", 1 ));
			
			var priceLft = ''; var priceRgt = '';
			priceLft 	= $.cookie("priceLftCkValue");
			priceRgt 	= $.cookie("priceRgtCkValue");
			priceLft 	= (priceLft=='' || priceLft==null)?'':priceLft.replace("", "");
			priceRgt 	= (priceRgt=='' || priceRgt==null)?'':priceRgt.replace("", "");
			priceLft 	= priceLft==''?priceMin:priceLft;
			priceRgt 	= priceRgt==''?priceMax:priceRgt;
				
			$( "#price-slider-range" ).slider({ values: [priceLft, priceRgt] });
			$( "#price-slide-left" ).val(symbol +priceLft);
			$( "#price-slide-right" ).val(symbol +priceRgt);
	        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('lnr-catalog-search');
	        $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
	        /*if(this.currTheme == "expertusoneV2"){
	          $('#search_price #paintPrice #price-slider-range a:last').css('margin-left','-8px');
	        }*/

		}
	  }catch(e){
			// to do
		}
	},  

	clearPriceSlider: function(searchAction) {
		try{
			if(typeof searchAction == 'undefined') {
				var searchAction = true;
			}
			obj = this;
			var priceMin = $('div#price-slider-range').slider("option", "min");
			var priceMax = $('div#price-slider-range').slider("option", "max");
			//reset cookie values which has from and to price values
			$.cookie("priceLftCkValue", "");
			$.cookie("priceRgtCkValue", "");
			//destroy and reinitialize price slider
			$('div#price-slider-range').slider("destroy");
			obj.priceSlider(priceMin, priceMax);
			if(searchAction) {
				obj.searchAction('clearprice');
			}
		} catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	locationEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_location").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction('location');
			 }
		 });
		}catch(e){
			// to do
		}
	},

	paintLocationAutocomplete : function(){
	  try{
		$('#srch_criteria_location').autocomplete(
			"/?q=learning/location-autocomplete",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},
	paintAutocomplete: function(inputId, ValuesEnabled) {
	    try {
	       $("#"+inputId).autocomplete(
	    		   ValuesEnabled, {
	            	minLength: 2,

	            });
	       $("#"+inputId).focus(function(event) {
	                try {
	                    if ($(this).val() == Drupal.t("LBL304")) {
	                        $(this).css({
	                                'color': '#474747',
	                                'font-size': '12px'
	                            })
	                            .val('');
	                    }
	                } catch (e) {
	                  //  console.log(e, e.stack);
	                }
	            })
	            .blur(function(event) {
	                try {
	                    if ($(this).val() == '') {
	                        $(this).css({
	                                'color': '#999999',
	                                'font-size': '12px'
	                            })
	                            .val(Drupal.t('LBL304'));
	                    }
	                    if ($(this).val() != Drupal.t('LBL304')) {
	                        $(this).css({
	                            'color': '#333333',
	                            'font-size': '12px'
	                        });
	                    }
	                } catch (e) {
	                  //  console.log(e, e.stack);
	                }
	            });
	    } catch (e) {
	        // to do
	      // console.log(e, e.stack);
	    }
	},
	searchActionCheck : function() {
	 try{
		var req=window.location.search;
		req=req.substring(3,req.length);
		var reqArr=req.split("/");
		var searchStr = '';
		if(this.currTheme == "expertusoneV2"){
		  var passSearchValue = Drupal.t('LBL304');
		}else{
		  var passSearchValue = Drupal.t("LBL304").toUpperCase();
		}
		
		if(reqArr.length>2){
			searchStr = (reqArr[2]!=null && reqArr[2]!=undefined && reqArr[2]!='undefined')?reqArr[2]:'';
			reqTitleSearch=searchStr.split("|");
			if(reqTitleSearch!='' && reqTitleSearch!=null && reqTitleSearch!=undefined && reqTitleSearch!='undefined') {
				if(reqTitleSearch.length == 1){
                    reqTitleSearch=searchStr.split("%7C");
				}
				 passSearchTitle = reqTitleSearch[0];
				 passSearchValue = reqTitleSearch[1];
			}
		}
		
		passSearchValue = passSearchValue.replace('@@','/');

		var searchTerm = $('#search_searchtext').val(unescape(passSearchValue)).val();
		if((searchTerm.toLowerCase()) != (Drupal.t("LBL304").toLowerCase())) {
		    $('#search_searchtext').css({
			'color': '#333333',
			'font-size': '13px'
		    });
		    //$(".eol-search-clearance").show();
		}
		
		if((passSearchValue.toLowerCase())==(Drupal.t('LBL304').toLowerCase()))
			return '';
		else
			return unescape(passSearchValue);
	 }catch(e){
			// to do
		}
	},
	
	hightlightedText : function(event, ID,textType) {
	 try{	
			 //		 textType = unescape(textType);
		var crrTheme = Drupal.settings.ajaxPageState.theme;
		var fontStyle = (crrTheme == 'expertusoneV2')?'normal':'italic';
		if(event.type == "blur"){
   			if($("#"+ID).val() != textType) {
        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
    		}
		}
		else if(event.type == "focus"){
		   			if($("#"+ID).val() != textType) {
		   				$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
		    		}
		    		if($("#"+ID).val() == textType) {
		        		$("#"+ID).val('').css('color','#333333').css('fontSize','11px').css('fontStyle',fontStyle);
		    		}
				}
				else if (event.type == "change"){
		   			if($("#"+ID).val() != textType) {
		        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
		    		}
		    		if($("#"+ID).val() == '') {
		        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','11px').css('fontStyle',fontStyle);
		    		}
				}
			/*$("#"+ID).blur(function(){
				console.log('in blur '+$("#"+ID).val());
				console.log('in blur '+textType);
	   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
    		}
		});
		$("#"+ID).focus(function(){
				console.log('in focus '+$("#"+ID).val());
				console.log('in focus '+textType);
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == textType) {
        		$(this).val('').css('color','#333333').css('fontSize','11px').css('fontStyle',fontStyle);
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','11px').css('fontStyle',fontStyle);
    		}
			});*/
	 }catch(e){
			// to do
		}
	},	
	
	dateValidationCheck : function() {
	 try{	
		var v1=$('#ad_startdate1').val();
		var v2=$('#ad_startdate2').val();
				
		if($('#ad_startdate1').val() == 'Start: mm-dd-yyyy')
			v1 = '';

		if($('#ad_startdate2').val() == 'End: mm-dd-yyyy')
			v2 = '';

		var v1Split = v1.split('-');
		var v2Split = v2.split('-');
		var date1 = v1Split[2] + '-' + v1Split[0] + '-' + v1Split[1];
		var date2 = v2Split[2] + '-' + v2Split[0] + '-' + v2Split[1];	
		
		/*
		
		var d1=new Date(date1);
		var d2=new Date(date2);
		var end_date_less_diff= d2.valueOf() - d1.valueOf();		
		var now = new Date();
		var dateString = now.getFullYear() + "-" + now.getMonth() + "-" + now.getDate() ;
		var currdate=new Date(dateString);
		var start_date_less_currdate =  d1.valueOf() - currdate.valueOf();
		var end_date_less_currdate =  d2.valueOf() - currdate.valueOf();
		
		*/
		
		var d1=new Date(date1);
		var d2=new Date(date2);
		
		var currentTime = new Date();
		var month = currentTime.getMonth() + 1;
		var day = currentTime.getDate();
		var year = currentTime.getFullYear();
		
		var date3 = year + '-' + month + '-' + day;
		var today = new Date(date3);
				

		var end_date_less_diff= d2 - d1;		
		var start_date_less_currdate =  d1 - today;
		var end_date_less_currdate =  d2 - today;

		$("#date-validate-newid").remove();

		if((v1!='') && (v2!='')) {			

			if (start_date_less_currdate < 0 ) {
				
				 newDiv=document.createElement('div');
				 newDiv.id= "date-validate-newid";
				 $(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				 $(newDiv).html(SMARTPORTAL.t("Start date cannot be before current date."));		
				 $('.catalog-date-format').before(newDiv);
				 $("#date-validate-newid").show();
				 
			} else if (end_date_less_diff < 0){
				
				 newDiv=document.createElement('div');
				 newDiv.id= "date-validate-newid";
				 $(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				 $(newDiv).html(SMARTPORTAL.t("End date cannot be less than the Start date."));
				 $('.catalog-date-format').before(newDiv);
				 $("#date-validate-newid").show();
				 
			} else {				
				 this.searchAction('date');
			}
		} else if ((v1!='') && (v2=='')) {

			if (start_date_less_currdate < 0 ) {
				
				newDiv=document.createElement('div');
				newDiv.id= "date-validate-newid";
				$(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				$(newDiv).html(SMARTPORTAL.t("Start date cannot be before current date."));		
				$('.catalog-date-format').before(newDiv);
				$("#date-validate-newid").show();
				
			} else {				
				this.searchAction('date');
			}
			
		} else if ((v2!='') && (v1=='')){

			if (end_date_less_currdate < 0 ) {
				
				newDiv=document.createElement('div');
				newDiv.id= "date-validate-newid";
				$(newDiv).css({'color':'red','display':'none','float':'left','font-size':'11px'});
				$(newDiv).html(SMARTPORTAL.t("End date cannot be before current date."));		
				$('.catalog-date-format').before(newDiv);
				$("#date-validate-newid").show();
				
			} else {				
				this.searchAction('date');
			}
			
		} else {
			this.searchAction('date');
		}
	 }catch(e){
			// to do
	 }
	},
	
	customRangeDate : function (input) 
	{ 
		try{
	        var dateMin = null;
	        var dateMax = null;

	        if (input.id == "ad_startdate1" && $("#ad_startdate2").datepicker("getDate") != null)
	        {
	        		dateMin = new Date();
	                dateMax = $("#ad_startdate2").datepicker("getDate");
	                dateMax.setDate(dateMin.getDate() + 20000);
	                                      
	        }
	        else if (input.id == "ad_startdate2")
	        {
	                dateMax = new Date();
	                if ($("#ad_startdate1").datepicker("getDate") != null)
	                {
	                        dateMin = $("#ad_startdate1").datepicker("getDate");
	                        dateMax = $("#ad_startdate1").datepicker("getDate");
	                        dateMax.setDate(dateMax.getDate() + 20000); 
	                }
	        }
	        
	     return {
	    	 minDate: dateMin, 
	    	 maxDate: dateMax
	   	 }; 
		}catch(e){
			// to do
		}
		
	},
	displayTagTip : function(elementid, messagecontent){
		try{
			$('.qtip-contentWrapper').css('border','0px none');
			if(!document.getElementById("tooltip"+elementid)) {
				$('#'+elementid).qtip({
					 content: '<div id="tooltip'+elementid+'" class="tooltiptop"></div><div class="tooltipmid"><div style="width:220px;">'+messagecontent+'</div></div><div class="tooltipbottom"></div>',			
				     show:{
						when:{
							event:'mouseover'
						},
						effect:'slide'
					 },
					 hide: {
						when:{
							event:'mouseout'
						},
						effect:'slide'
					},
					position: { adjust: { x: -75, y: 0 } },
					style: {
						width: 325,
						background: 'none',
						'font-size' : 12,
						color: '#333333'
					}
				});
			}
		}catch(e){
			// to do
		}
	},
	
	paintAfterReady : function() {		
		try{
		var dates = $('#ad_startdate1').datepicker({
			  duration: '',
			  showTime: false,
			  constrainInput: false,
			  stepMinutes: 5,
			  stepHours: 1,
			  time24h: true,
			  dateFormat: "mm-dd-yy",
			  buttonImage: themepath+'/expertusone-internals/images/calendar_icon.JPG',
			  buttonImageOnly: true,
			  firstDay: 0,
			  showOn: 'both',
			  buttonText:Drupal.t('LBL675'),
			  showButtonPanel: true,
			  changeMonth: true,
			  changeYear: true, 
			  beforeShow: $("#lnr-catalog-search").data("lnrcatalogsearch").customRangeDate
		});
		var dates = $('#ad_startdate2').datepicker({
			  duration: '',
			  showTime: false,
			  constrainInput: false,
			  stepMinutes: 5,
			  stepHours: 1,
			  time24h: true,
			  dateFormat: "mm-dd-yy",
			  buttonImage: themepath+'/expertusone-internals/images/calendar_icon.JPG',
			  buttonImageOnly: true,
			  firstDay: 0,
			  showOn: 'both',
			  buttonText:Drupal.t('LBL676'),
			  showButtonPanel: true,
			  changeMonth: true,
			  changeYear: true, 
			  beforeShow: $("#lnr-catalog-search").data("lnrcatalogsearch").customRangeDate
		});
		if(document.getElementById('lnr-catalog-search')) {
			$('#first_pager').click( function(e) {
				
				if(!$('#first_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#prev_pager').click( function(e) {
				
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#next_pager').click( function(e) {
				
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#last_pager').click( function(e) {
		
				if(!$('#last_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			$('.ui-pg-selbox').bind('change',function() {
				$('#paintContentResults').hide();
				$('#gview_paintContentResults').css('min-height','100px');
				$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
				$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
			});			
			$(".ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','100px');
				  $("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
				}
			});
			if(this.currTheme == "expertusoneV2"){				
					$('.page-show-prev').bind('click',function() {
						if(parseInt($("#pg_pager .page_count_view").attr('id')) < 0){
							$("#pg_pager .page_count_view").attr('id','0');
						}else{
							$('#paintContentResults').hide();
							$('#gview_paintContentResults').css('min-height','100px');
							$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
							$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						}
					});
				
					$('.page-show-next').bind('click',function() {	
						if(parseInt($("#pg_pager .page_count_view").attr('id')) >= parseInt($("#pg_pager .page-total-view").attr('id'))){
							$("#pg_pager .page_count_view").attr('id',($("#pg_pager .page_count_view").attr('id')-1));
						}else{
							$('#paintContentResults').hide();
							$('#gview_paintContentResults').css('min-height','100px');
							$("#lnr-catalog-search").data("lnrcatalogsearch").hidePageControls(false);
							$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						}
					});
				
			}
		}
		}catch(e){
			// to do
		}
	},
	showSelectClass : function(userId,courseId){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var chgClassWidth = 688 ;
	 	if(this.currTheme == "expertusoneV2"){
			chgClassWidth = 750 ;	
		}
		$("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart = true;
		//this.changeClsEnrollId = enrollId;
		this.renderSelectClassPopup(userId);
		$('#paintCatalogContentResults'+userId).css('min-height','auto').css('overflow','visible');
		this.createLoader('paintCatalogContentResults'+userId);
		$('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
		
		var nAgt = navigator.userAgent;
		var verOffset ;
		var DetailsW = 465;
		var actionW = 85;
	
		// In Chrome, the true version is after "Chrome" 
		if ((verOffset=nAgt.indexOf("Chrome"))!=-1) {
			 DetailsW = 420;
			 actionW = 100;
		}
		//	alert(10000);
		var obj = this;
		var objStr = '$("#lnr-catalog-search").data("lnrcatalogsearch")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#tbl-paintCatalogContentResults").jqGrid({
				url:this.constructUrl("learning/courselevel-search/catalog/"+userId+ "/" +courseId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Icons','Details','Action'],
				colModel:[ {name:'Icons',index:'Icons', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchIcons},
				           {name:'Details',index:'Details', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResults},
				           {name:'Action',index:'Action', title:false,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResultsAction}],
				rowNum:5,
				rowList:[5,10,15],
				pager: '#pager1',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:false,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				loadComplete:obj.callbackCatalogSelectClassLoader,
				gridComplete:obj.callbackGridSelectClassComplete
			}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
			$("#tbl-paintCatalogContentResults").jqGrid({
				url:this.constructUrl("learning/courselevel-search/catalog/"+userId+ "/" +courseId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Icons','Details','Action'],
				colModel:[ {name:'Icons',index:'Icons', title:false, width:55,'widgetObj':objStr,formatter: obj.paintSelectClsSearchIcons},
				           {name:'Details',index:'Details', title:false, width:DetailsW,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResults},
				           {name:'Action',index:'Action', title:false, width:actionW,'widgetObj':objStr,formatter: obj.paintSelectClsSearchResultsAction}],
				rowNum:5,
				rowList:[5,10,15],
				pager: '#pager1',
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "desc",
				toppager:false,
				height: 'auto',
				width: chgClassWidth,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadui:false,
				loadComplete:obj.callbackCatalogSelectClassLoader,
				gridComplete:obj.callbackGridSelectClassComplete
			}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
			$("#shareoption #bubble-face-table" ).hide();
			$("#shareoption .qtip-popup-visible" ).hide();		
			$("#multishareoption #bubble-face-table" ).hide();
			$("#multishareoption .bottom-qtip-tip-up-visible" ).hide();				
		this.paintCatalogSelectClassAfterReady(userId);
		}catch(e){
			// to do
		}
	},
	
	paintSelectClsSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},

	paintSelectClsSearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},

	paintSelectClsSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},
	
	
	/*
	 * To show the pop-up with the rendered classes
	 */
	renderSelectClassPopup : function(id) {
		try{
		var obj	= this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var chgclassqtipWidth = 750 ;	
		}else{
			var chgclassqtipWidth = 712;
	    }
	 	//Embed widget related work (Create flexible grid)
	 	if(Drupal.settings.widget.widgetCallback==true){
		    var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
		    chgclassqtipWidth = iwidth - 40;
		}
		var html = "<div id='paintCatalogContentResults"+id+"'></div>";
        $("#paintCatalogContentResults"+id).remove();
        $('body').append(html);
        var nHtml = "<div id='show_expertus_message'></div><div id='paintCatalogHeader'></div><div id='catalog-no-records' style='display: none'></div><table id='tbl-paintCatalogContentResults'></table><div id='paintCatalogFooter'><div id='pager1' style='display:none;'></div></div>";
        $('#paintCatalogContentResults'+id).html(nHtml);
        var closeButt={};
        $("#paintCatalogContentResults"+id).dialog({
        	autoResize: true,
        	position:[(getWindowWidth()-chgclassqtipWidth)/2,100],
            bgiframe: true,
            width:chgclassqtipWidth,
            resizable:false,
            draggable:false,
            closeOnEscape: false,
            modal: true,
            title:SMARTPORTAL.t('<span class="myteam-header-text">' + Drupal.t("LBL716") +'</span><div id="search-cat-keyword"></div>'),
            buttons: closeButt,
            close: function(){
                $("#paintCatalogContentResults"+id).remove();
                $("#select-class-dialog").remove();
                if($('.location-session-detail-clone').size()>0){
                	$('.location-session-detail-clone').remove();
                }
                //obj.viewLearning(id,rowObj);
            },
            overlay: {
 	           opacity: 0.5,
	           background: "#000000"
             },
            open: function(){
            	//Embed widget related work (Create flexible grid)
            	if((typeof(Drupal.settings.widget.widgetCallback) == "undefined" && Drupal.settings.widget.widgetCallback == null) || Drupal.settings.widget.widgetCallback == false) {
            		$(".ui-widget-overlay").eq(-1).css("width",getWindowWidth()-1+"px");
            	}
             }
        });
        //$('.ui-dialog').css('border','solid 10px #5F5F5F');
        $('.ui-dialog').wrap("<div id='select-class-dialog'></div>");
        $("#paintCatalogContentResults"+id).css("position","");
        this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
	 		$('#select-class-dialog').find('.ui-dialog-content').addClass('assign-learning-team');	
	 		$('#select-class-dialog').find('.ui-dialog').css("overflow","visible");
	 		changeDialogPopUI();
	 		$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	   }
		}catch(e){
			// to do
		}
    },
	
	/*
	 * Call back after rendering the classes in the pop-up
	 */
    callbackCatalogSelectClassLoader: function(response, postdata, formid){
     try{	
	  $("#tbl-paintCatalogContentResults").show();
	  $('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:first-child').addClass('assign-learn-first-row');
		$('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:nth-child(2)').addClass('assign-learn-second-row');
		$('#select-class-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:last-child').addClass('assign-learn-last-row');
		
	  var recs = parseInt($("#tbl-paintCatalogContentResults").getGridParam("records"),10);
      if (recs == 0) {
        $('#catalog-no-records'). css('display','block');
        var html = Drupal.t("MSG381")+'.';
        $("#catalog-no-records").html(html);            
      } else {
        $('#catalog-no-records'). css('display','none');
        $("#catalog-no-records").html("");
      }    
      var obj = $("#lnr-catalog-search").data("lnrcatalogsearch");

      var reccount = parseInt($("#tbl-paintCatalogContentResults").getGridParam("reccount"), 10);
	  // Show pagination only when search results span multiple pages
	  var hideAllPageControls = true;
	  if (recs > reccount) { // 5 is the least view per page option.
	    hideAllPageControls = false;
	    $('#select-class-dialog .popupBotLeftCorner').hide();
	  }

      
      if (recs <= reccount) {
        obj.hideCatalogSelectClassPageControls(hideAllPageControls);
        $('#select-class-dialog .popupBotLeftCorner').show();
      }
      else {
        obj.showCatalogSelectClassPageControls();
        $('#select-class-dialog .popupBotLeftCorner').hide();
      }

      if($("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart) {
		$("#paintCatalogHeader").html(response.header);
		$("#myteam-find-trng-sort-display").show();
		$("#lnr-catalog-search").data("lnrcatalogsearch").defaults.catStart = false;
      }
      var curHeight = parseInt($(".ui-dialog").eq(-1).css("height")) + parseInt($(".ui-dialog").eq(-1).css("top"));
      var curOlay = parseInt($(".ui-widget-overlay").eq(-1).css("height"));
	  if(curOlay < curHeight) {
	    $(".ui-widget-overlay").eq(-1).css("height", (curHeight + 50) + "px");
	  }
	//Embed widget related work (Create flexible grid)
	 if(Drupal.settings.widget.widgetCallback==true){
		 if(Drupal.settings.ajaxPageState.theme != "expertusoneV2"){
			 var leftval = $('#select-class-dialog .ui-dialog').offset().left;
			 $('#select-class-dialog .ui-dialog').css('left',Math.floor(leftval/2)+'px');
		 }
	 }
	  $("#tbl-paintCatalogContentResults tr.ui-widget-content:last td").css('border','0px none');  
	  $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('paintCatalogContentResults'+response.userId);
	  
	// call for updated content result display in launch-content-container register and launch feature
	  //console.log('test event for one click launch');
	  if(document.getElementById('launch-content-container')){
		  //console.log('hrere');
		  var dataObj = $("#tbl-paintCatalogContentResults").jqGrid('getGridParam', 'postData');
		  //console.log('pst data received');
		  //console.log(data);
		  if(dataObj.enrollmentId!='' && dataObj.enrollmentId!=undefined){
			  $("#launch"+dataObj.enrollmentId).click();
		  }
	  }
	  vtip();	
	  resetFadeOutByClass('#tbl-paintCatalogContentResults','find-training-list-course','line-item-container','catalog');
	  $('.limit-title').trunk8(trunk8.catalogclsmul_title);
	  $('.limit-desc').trunk8(trunk8.catalogclsmul_desc);
	  scrollBar_Refresh('catalog');
     }catch(e){
			// to do
		}
	},
	
	callbackGridSelectClassComplete: function(response, postdata, formid){
		try {
			$('.limit-title').trunk8(trunk8.catalogclsmul_title);
			$('.limit-desc').trunk8(trunk8.catalogclsmul_desc);	
			scrollBar_Refresh('catalog');
		} catch(e) {
			// to do
		}
	},
	
	hideCatalogSelectClassPageControls : function(hideAll) {
	 try{	
	  var lastDataRow = $('#tbl-paintCatalogContentResults tr.ui-widget-content').filter(":last");
	  if (hideAll) {
		$('#pager-lp').hide();
		$('#pager1').hide();
	    if (lastDataRow.length != 0) {
	      lastDataRow.children('td').css('border', '0 none');
	    }
	  }
	  else {
	    $('#pager1').show();
	    $('#pager1 #pager1_center #first_pager1').hide();
	    $('#pager1 #pager1_center #last_pager1').hide();
	    $('#pager1 #pager1_center #next_pager1').hide();
	    $('#pager1 #pager1_center #prev_pager1').hide();              
	    $('#pager1 #pager1_center #sp_1_pager1').parent().hide();
	  }
	 }catch(e){
			// to do
		}
	},
	  
	showCatalogSelectClassPageControls : function() {
	 try{	
	  $('#pager1').show();
	  $('#pager1 #pager1_center #first_pager1').show();
	  $('#pager1 #pager1_center #last_pager1').show();
	  $('#pager1 #pager1_center #next_pager1').show();
	  $('#pager1 #pager1_center #prev_pager1').show();              
	  $('#pager1 #pager1_center #sp_1_pager1').parent().show();
	 }catch(e){
			// to do
		}
	},
	/*
	 * action to be performed while clicking on pagination at Change Classes pop-up
	 */
	paintCatalogSelectClassAfterReady : function(userId) {
		try{
		
		if(document.getElementById('select-class-dialog') != null) {
			$('#first_pager1').click( function(e) {
				if(!$('#first_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#prev_pager1').click( function(e) {
				if(!$('#prev_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#next_pager1').click( function(e) {
				if(!$('#next_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#last_pager1').click( function(e) {
				if(!$('#last_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#pager1 .ui-pg-selbox').bind('change',function() {
				$('#tbl-paintCatalogContentResults').hide();
				$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
				$("#lnr-catalog-search").data("lnrcatalogsearch").hideCatalogSelectClassPageControls(false);
				$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
			});			
			$("#pager1 .ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+userId);
				}
			});
		}
		}catch(e){
			// to do
		}
	},
	
	/*
	 * To list all matched classes on class/code autocomplete on class search at pop-up
	 * 'Change Classes' section
	 */
	paintCatSltClskeywordAutocomplete : function(userId,courseId){
		try{
		var obj	= this;
		var delType	= $('#myteam-cat-delivery-type-hidden').val();
		url = obj.constructUrl("learning/select-class/catalog-autocomplete/" + delType + '/' + userId + '/' + courseId + '/');
		$('#srch_criteria_catkeyword').autocomplete(
			url,{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},
	
	deliverySelectClsHideShow : function () {
		try{
		$('#myteam-cat-delivery-type-list').slideToggle();
		$('#myteam-cat-delivery-type-list li:last').css('border-bottom','0px none');	
		}catch(e){
			// to do
		}
	},
	
	deliveryTypeSelectClsText : function(dCode,dText,userId,courseId) {
		try{
		$('#myteam-cat-delivery-type-list').hide();
		$('#myteam-cat-delivery-type').text(dText);
		$('#myteam-cat-delivery-type-hidden').val(dCode);
		//$("#lnr-catalog-search").data("lnrcatalogsearch").catalogSearchSelectClassAction("","",userId,courseId);
		$("#lnr-catalog-search").data("lnrcatalogsearch").paintCatSltClskeywordAutocomplete(userId,courseId);
		}catch(e){
			// to do
		}
	},
	
	
	 /* To highlight the default text when there is no text, in search filters*/
	 
	highlightedSelectClassText : function(ID,textType) {
		try{
		$("#"+ID).blur(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','12px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).focus(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == textType) {
        		$(this).val('').css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != textType) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(textType).css('color','#999999').css('fontSize','12px').css('fontStyle','normal');
    		}
		});
		}catch(e){
			// to do
		}
	},	
	
	 /* To call class search widget when enter class keywords on pop-up, 'Change Classes' section*/
	 
	catkeywordSelectClassEnterKey : function(userId,courseId){
		try{
		 obj = this;
		 $("#srch_criteria_catkeyword").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.catalogSearchSelectClassAction('','',userId,courseId);
			 }
		 });
		}catch(e){
			// to do
		}
	},
	/*
	 * Action to performed during class search in the pop-up
	 */
	catalogSearchSelectClassAction : function(sortbytxt,className,userId,courseId) {
		try{
		var searchStr = '';
		/*-------Title-------*/
		var title 	  = $('#srch_criteria_catkeyword').val();
		if(title == Drupal.t("LBL545"))
			title='';
		else		
			$('#cat-title-clr').css('display','block');
		
		/*-------Delivery Type-------*/
		var dl_type 	  = $('#myteam-cat-delivery-type-hidden').val();
		if(dl_type == Drupal.t("LBL428"))
			dl_type='';
		else		
			$('#cat-title-clr').css('display','block');	
		
		/*-------Sort By-------*/
		var sortby = sortbytxt;
		if(title == undefined)
			title = "";
		searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type)+'&sortby='+sortby;
		this.searchStrValue = searchStr;
		$("#lnr-catalog-search").data("lnrcatalogsearch").hideCatalogSelectClassPageControls(true);
		$('#tbl-paintCatalogContentResults').hide();		
        this.createLoader('paintCatalogContentResults'+userId);
        $('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
    	
		$('#tbl-paintCatalogContentResults').setGridParam({url: this.constructUrl("learning/courselevel-search/catalog/"+userId+"/" +courseId+ '/'+searchStr)});
	    $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:1}]);
	   //Highlight sort type added by yogaraja 
	    $('.'+className).parents('li').siblings().find('a').removeClass('sortype-high-lighter');
		if(className!=''){
			$('.'+className).addClass('sortype-high-lighter');
		}else{
			$('#paintCatalogHeader .catalog-type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},
	
	underlineTagAndTriggerSearch: function(tagName, currElem) {
		  try {
			$('.tagscloud-tag').css('text-decoration', '');
			$('.hovertag').css('text-decoration', '');
			$(currElem).css('text-decoration', 'underline');
			$('#srch_criteria_tag').val(tagName);
			this.searchAction('cloud-tag');
		  }
		  catch (e) {
		    // To Do
		  }
	},
	
	underlineTagCloudAndTriggerSearch: function(tagName, currElem, tagWeight) {
		  try {
			$('.tagscloud-tag').css('text-decoration', '');
			$('.hovertag').css('text-decoration', '');
			$(currElem).css('text-decoration', 'underline');
			$('#srch_criteria_tag').val(tagName);
			this.searchAction('cloud-tag');
			var tagpos = $.inArray($(currElem).html(),existtagArr,0);
			if(tagpos == -1){
				$tagText = $(currElem).html();
				existtagArr.unshift($tagText);
				existtagArr.pop();
				$('#tagcloudcontainerid').find('span:last-child').remove();
				$(currElem).html(descController('FADE OUT',$tagText,weightArr[tagWeight-1]));
				var selectedTag = $(currElem).parent().html();
				$('#tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
				$(currElem).html($tagText);
			}else if(tagpos >= 0){
				$tagText = $(currElem).html();
				$('#tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
				if (tagpos > -1) {
					existtagArr.splice(tagpos, 1);
 				}
				existtagArr.unshift($tagText);
				$(currElem).html(descController('FADE OUT',$tagText,weightArr[tagWeight-1]));
				var selectedTag = $(currElem).parent().html();
				$('#tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
				$(currElem).html($tagText);
			}
		  }
		  catch (e) {
		    // To Do
		  }
	},
	truncateTitleDescription: function(trunk8options) {
		try{
		// return;
			if(trunk8options === undefined) {
				var trunk8options = [];
				trunk8options.push({'selector': '.content-title .limit-title', 'lines': trunk8.embedwidget_title});
				trunk8options.push({'selector': '.content-description .limit-desc', 'lines': trunk8.embedwidget_desc});
				$(trunk8options).each(function() {
				$(this.selector).trunk8('revert');
				$(this.selector).trunk8();
			});
			} else {
			$(trunk8options).each(function() {
				// $(this.selector).trunk8(this.lines);
				$(this.selector).trunk8(this.lines);
			});
	}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	pinUnpinFilterCriteria: function(options,type) {
		try {
			var _this = this;
			// _this.toggleClass('unpinned');
			$('#paintContent').toggleClass('catalog-extended');
			////////////
			_this.showHideFilterCriteria(!$('#paintContent').hasClass('catalog-extended'));
			
			/* $('#search-filter-content-wrapper').appendTo('#paintContent .catalog-criteria-refine-icon')
			// .addClass('.pinned')
			.width($('#paintCriteriaResults').width())
			// .css({'position': 'absolute', 'z-index':10, 'display':'none'})

			$('#paintContent .catalog-criteria-refine-icon').mouseenter(function(event) {
				// $('#search-filter-content-wrapper').show();
				$('#paintContent .catalog-criteria-refine-icon').addClass('hover');
			});

			$('#paintContent .catalog-criteria-refine-icon').mouseleave(function(event){
				try{
					// console.log('target-',$(event.target));
					// console.log('relatedTarget-',$(event.relatedTarget));
					// console.log('relatedTarget-',$(event.target));
					var comingfrom = event.toElement || event.relatedTarget;

					var isVtip = ($(comingfrom).attr('id') == 'vtip' || $(comingfrom).attr('id') == 'vtipArrow');
					console.log('comingfrom-',comingfrom, $(comingfrom).attr('id'), isVtip);
					if(!isVtip && $(comingfrom).parents('.catalog-criteria-refine-icon').length == 0) {
						// $('#search-filter-content-wrapper').hide();
						$('#paintContent .catalog-criteria-refine-icon').removeClass('hover');
					}
				} catch(e){
					console.log(e, e.stack);
				}
			});
			 */
			
			////////////
			$('#page-container .searchcriteria-div').toggleClass('searchcriteria-div-unpinned');
			// $('.catalog-criteria-refine-icon:not(.click-binded)').addClass('click-binded').bind('click', _this.showHideFilterCriteria);
			// $('.catalog-criteria-refine-icon:not(.click-binded)').addClass('click-binded').bind('mouseenter mouseout', _this.showHideFilterCriteria);
			$("#paintContentResults").jqGrid('setGridWidth', ($('#paintContent').width() - 2));
			_this.truncateTitleDescription();
			var userId = _this.getLearnerId();
			var refineHidden = $('#paintContent').hasClass('catalog-extended');
			if(type != 'toggle') {	//user logged in
				$.ajax({
					type: "POST",
					url: _this.constructUrl("learning/save-preference/1"),
					data: {catalog_refine: (refineHidden ? 1 : 0)},
					success: function(result) {
						// console.log(result);
						if(userId==0)
							Drupal.settings.user_preferences.catalog_refine = refineHidden ? 1 : 0
						else
							Drupal.settings.user_preferences = result;
					}
				});
			}
			$('#paintContentResults .content-detail-code').find('.fadeout-applied').removeClass('fadeout-applied');
			setTimeout(function(){
				resetFadeOutByClass('#paintContentResults','content-detail-code','line-item-container','catalog');
			},100);
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	showHideFilterCriteria: function(show, toggleCompleteHandler) {
		try {
			if(show == true) {
				$('#page-container .searchcriteria-div').addClass('active');
			} else if(show == false) {
				$('#page-container .searchcriteria-div').removeClass('active');
			} else {
				$('#page-container .searchcriteria-div').toggleClass('active');
			}
			$('#paintCriteriaResults').find('.fade-out-title-container:not(.title-processed)').each(function() {
				if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
					$(this).find('.fade-out-image').remove();
				}
			}).addClass('title-processed');
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	collapseFilters: function(cookieName, count) {
		try {
			var obj = this;
			if(cookieName !== undefined && $.cookie(cookieName) !== undefined && $.cookie(cookieName) !== null) {
				//if cookie is given it will collapse/uncollape the given filters with given state
				var filters = JSON.parse($.cookie('filters-state'));
				// console.log('filters set ', filters);
				for(var handlerId in filters){
					// console.log(handlerId, filters[handlerId]);
					// console.log($('#'+handlerId).parents('.find-list-items').siblings('.catalog-criteria-filter-set'));
					if(filters[handlerId]) {	//uncollapse filter section
						$('#'+handlerId).removeClass('cls-hide')
							.addClass('cls-show')
							.parents('.find-list-items').siblings('.catalog-criteria-filter-set').show();
					} else {	//collapse filter section
						$('#'+handlerId).removeClass('cls-show')
							.addClass('cls-hide')
							.parents('.find-list-items').siblings('.catalog-criteria-filter-set').hide();
					}
				}
			} else {
				(count === undefined) && (count = -5);	//default select of last 5 filters if count is not given
				$('#paintCriteriaResults').find(".catalog-criteria-filter-set").slice(count).each(function(){
					var handlerId = $(this).siblings().find('.cls-show').attr('id');
					var targetId = $(this).attr('id');
					obj.showHide(handlerId, targetId, true);
				});
			}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	parseParams: function(paramString) {
		try {
			var result = {};
			// "&title=&dl_type=&lg_type=cre_sys_lng_eng&all_lg_type=cre_sys_lng_eng|cre_sys_lng_gzh&ob_type=&location=&tag=&price=%24,$0-%24,$10000&startdate=&enddate=&jr_type=&all_jr_type=&cy_type=&all_cy_type=&sortby=undefined&mro_type=&all_mro_type=cre_sys_inv_com|cre_sys_inv_man&rating_type="
			paramString.split("&").forEach(function(part) {
				var item = part.split("=");
				result[item[0]] = decodeURIComponent(item[1]);
			});
			return result;
		} catch(e) {
			//window.console.log(e, e.stack);
		}
	},
	addFilters: function(filters) {
		try {
			var filtersList = [];
			if($.isArray(filters)) {
				filtersList = filters;
			} else {
				filtersList.push(filters);
			}
			
			var filterLength = 0;
			$(filtersList).each(function(index, filterId) {
				var selector = '[data-filter-id="'+filterId+'"]';
				var filter = $(selector);
				var filterId = filter.data('filter-id');
				var filterName = filter.data('filter-name');
				var displayName = filter.data('filter-label');
				var menustr = '<span class="cls-filtermenu-wrapper"><span class="cls-filtermenu">';
				var showFilter = false;
				if(filter.is(':checkbox')) {
					if(filter.is(':checked')) {
						var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'" for="'+filterId+'"></label>';
						if(filterName == 'rating') {
							displayName = '<span class="lrn_srch_rating">'+filter.parent().siblings('span.lrn_srch_rating').html()+'</span>';
						}
						menustr += displayName;
						showFilter = true;
						filterLength++;
					}
					
				} else if(filterName == 'price-slider') {
					var Lprice = $("#price-slide-left").val();
					if (Lprice != undefined && Lprice != 'undefined') {
						Lprice = encodeURIComponent(Lprice.charAt(0)) + Lprice.substring(1);
					}
					var Rprice = $("#price-slide-right").val();
					if (Rprice != undefined && Rprice != 'undefined') {
						Rprice = encodeURIComponent(Rprice.charAt(0)) + Rprice.substring(1);
					}
					var price = $.trim(Lprice + "-" + Rprice);
					price = price.replace(/ /g, "");
					if (price == 'undefined-undefined' || price == undefined - undefined || price == '-') {
						price = '';
					}
					var displayName = decodeURIComponent(price.replace(/-/g, " - "));
					var delAction = '<label data-filter-tag-name="' + filterName + '" class="enable-delete-icon" id="filter-delete-handler-' + filterId + '"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearPriceSlider();"></label>';
					menustr += displayName;
					if (($('#price-slider-range').slider("option", "min") != $('#price-slider-range').slider("option", "values")[0])
						 || ($('#price-slider-range').slider("option", "max") != $('#price-slider-range').slider("option", "values")[1])) {
						//this will work when slider action happens
						showFilter = true;
						filterLength++;
					} else {
						//this will work when slider doesn't have proper values, if we don't initialize the slider properly (say catalog share scenario)
						var symbol = Drupal.settings.user_prefference.currency_sym;
						var minPrice = replaceAll(symbol, "", $('#price-slide-left').val());
						var maxPrice = replaceAll(symbol, "", $('#price-slide-right').val());
						minPrice = parseInt(minPrice.replace(/[^\d]/g, ''));
						maxPrice = parseInt(maxPrice.replace(/[^\d]/g, ''));
						if (($('#price-slider-range').slider("option", "min") != minPrice)
							 || ($('#price-slider-range').slider("option", "max") != maxPrice)) {
							//this will work when slider action happens
							showFilter = true;
							filterLength++;
						}
					}
				} else if(filterName == 'date-range') {
					var startdate = $("#ad_startdate1").val();
					var enddate = $("#ad_startdate2").val();	
					if(startdate == Drupal.t("LBL251")+ ':' +Drupal.t("LBL112")) {
						startdate = "";
					}
					if(enddate == Drupal.t("LBL113")) {
						enddate = "";
					}
					if(startdate != '' && enddate != '') {
						var displayName = startdate+'<span class=\'lower-case\'> '+Drupal.t('LBL621')+' </span>'+enddate;
						showFilter = true;
						filterLength++;
						
					} else if(startdate != '' || enddate != '') {
						var displayName = (startdate != '' ? startdate : enddate);
						showFilter = true;
						filterLength++;
					}
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Date\');"></label>';
					menustr += displayName;
				} else if(filterName == 'cloud-tag') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Tag\');"></label>';
					menustr += decodeURIComponent($('#srch_criteria_tag').val());
					if($('#srch_criteria_tag').length > 0 && $('#srch_criteria_tag').val() != '') {
						showFilter = true;
						filterLength++;
					}
				} else if(filterName == 'location') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').clearField(\'Location\');"></label>';
					menustr += decodeURIComponent($('#srch_criteria_location').val());
					if($.trim($('#srch_criteria_location').val()) != Drupal.t("LBL1321")) {
						showFilter = true;
						filterLength++;
					}
				} else if(filterName == 'top-search') {
					var delAction = '<label data-filter-tag-name="'+filterName+'" class="enable-delete-icon" id="filter-delete-handler-'+filterId+'"  onclick="$(\'#search_searchtext\').val(\'\');$(\'#lnr-catalog-search\').data(\'lnrcatalogsearch\').searchAction(\'top-search\');"></label>';
					menustr += $('#search_searchtext').val();
					if($('#search_searchtext').val().toLowerCase() != Drupal.t('LBL304').toLowerCase()) {
						showFilter = true;
						filterLength++;
					}
				}
				menustr += '</span></span>'+delAction;
				// console.log('filterName', filterName, 'displayName', displayName, 'showFilter', showFilter);
				if($('#paintContent .catalog-applied-filters').length == 0 && filterLength != 0) {
					$('<div class="catalog-applied-filters-wrapper"><div id="catalog-applied-filter-grid" class="catalog-applied-filters"></div></div>').insertBefore('#paintContent #no-records');
				}
				if (filterName != undefined && displayName != undefined && showFilter) {
					$('#paintContent #catalog-applied-filter-grid').append("<div class='checkedmenu' id='filter-"+filterId+"'>"+menustr+"</div>");
				}
			});
			$('.checkedmenu').each(function() {
				try {
					if($(this).find('.cls-filtermenu-wrapper').width() < $(this).find('.cls-filtermenu').width()) {
						$(this).find('.cls-filtermenu-wrapper').append('<span class="cls-filtermenu-fade"></span>');
					}
					var displayName = $(this).find('.cls-filtermenu').contents().get(0).nodeValue;
					if(displayName != null && displayName != '') {
						$(this).find('.cls-filtermenu-wrapper').attr('title', displayName).addClass('vtip');
					}
				} catch(e) {
					//window.console.log(e, e.stack);
				}
			});
			vtip('.checkedmenu');
			// add clear all handler
			if($('#catalog-applied-filter-grid').find('.cls-filter-clear').length == 0 && $('#paintContent #catalog-applied-filter-grid').find('.checkedmenu').length > 0) {
				var clearAllStr = Drupal.t('clear all');
				$("<a class=\"cls-filter-clear\" onclick=\"$('#lnr-catalog-search').data('lnrcatalogsearch').clearAllSearch();\">"+clearAllStr+"</a>").appendTo('#catalog-applied-filter-grid');
			} else {
				$('#catalog-applied-filter-grid .cls-filter-clear').appendTo('#catalog-applied-filter-grid');
			}

		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	
	removeFilters: function(filters, filterCounter) {
		try {
			var filtersList = [];
			if($.isArray(filters)) {
				filtersList = filters;
			} else {
				filtersList.push(filters);
			}
			$(filtersList).each(function(index, filterId) {
				$('#filter-'+filterId).remove();
			});
			if($('#catalog-applied-filter-grid').find('.checkedmenu').length == 0) {
				$('#catalog-applied-filter-grid').find('.cls-filter-clear').remove();
				// $(".catalog-applied-filters-wrapper #catalog-applied-filter-grid").remove();
				$(".catalog-applied-filters-wrapper").remove();
			}
			
		} catch(e) {
			//window.console.log(e, e.stack);
		}
	},
	getGetJSONCookie: function(cookieName, value) {
		try {
			//if value is present, given value will be saved. else existing value of the cookie will be returned
			if(value !== undefined) {
				// $.cookie(cookieName, JSON.stringify(value));
				value = value.filter(function(item, i, ar){ return ar.indexOf(item) === i; });

				Drupal.settings['catalog_variables'][cookieName] = value;
				// console.log('cookie value set', Drupal.settings['catalog_variables'][cookieName]);
			} else {
				// return (($.cookie(cookieName) !== undefined && $.cookie(cookieName) != null) ? JSON.parse($.cookie(cookieName)) : []);
				var cookie = [];
				// console.log(typeof Drupal.settings['catalog_variables'][cookieName], Drupal.settings['catalog_variables'][cookieName]);
				if(typeof Drupal.settings['catalog_variables'][cookieName] != 'undefined' && Drupal.settings['catalog_variables'][cookieName] != null) {
					cookie = (Drupal.settings['catalog_variables'][cookieName]);
				}
				// console.log('cookie value get', cookie);
				return cookie;
			}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	clearAllSearch: function(triggerSearch) {
		try {
		// console.log('clearall search');
			var obj = this;
			//clear all checkboxes, textboxes and call if clear methods available for controls and call searchAction
			$('.ot-others, .mro-others, .dt-others, .lang-others, .jobrole-others, .country-others, .rating-others').each(function() {
				$(this).attr('checked', false);
				$(this).parent().removeClass('checkbox-selected').addClass('checkbox-unselected');
			});
			obj.clearPriceSlider(false);
			obj.clearField("Date", false);
			obj.clearField("Location", false);
			obj.clearField("Tag", false);
			$('#search_searchtext').css({
				'color': '#999999',
				'font-size': '12px'
			})
			.val(Drupal.t('LBL304'));
			// console.log('triggerSearch = ', triggerSearch);
			if(triggerSearch === undefined || triggerSearch != false) {
				obj.searchAction('clearAll');
			}
		} catch (e) {
			// window.console.log(e, e.stack);
		}
	},
	refreshLastAccessedCatalogRow: function(gridRow) {
		try {
			var rowFound = false;
			// console.log('refreshLastAccessedCatalogRow', console.trace());
			var grid = $('#paintContentResults');
			if(gridRow === undefined) {
				var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
			}
			if(gridRow !== undefined && gridRow != null) {
				// console.log(gridRow, gridRow.id);
				var rowData = ((typeof gridRow.id != 'undefined' && gridRow.id) ? gridRow.id.split('-') : null);
				var options = {
					data: {
						ent_id: rowData[0],
						'ent_type': rowData[1]
					}
				};
				grid.jqGrid('updateRowByRowId', options);
				rowFound = true;	// return true to stop the grid reload
				grid.jqGrid('setGridParam', {lastAccessedRow: null});
			} else {
				// grid.trigger("reloadGrid",[{page:1}]);
				rowFound = false;	// return false so that reload of grid happens as no last accessed grid row found
			}
		} catch(e) {
			rowFound = false;
			// window.console.log(e, e.stack);
		}
		return rowFound;
	}
	// var objStr = $("#lnr-catalog-search").data("lnrcatalogsearch");
});

$.extend($.ui.lnrcatalogsearch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});


})(jQuery);

$(function() {
	try{
	$( "#lnr-catalog-search" ).lnrcatalogsearch();
	var curUrl	 = window.location.search.substring(1);
	var splitUrl = curUrl.split('/');
	var loadProfile = splitUrl[1];

	$("#search_searchtext").keyup(function(event){
		try{
		  if(event.keyCode == 13){
			  if(loadProfile != 'catalog-search') {
					var title = $('#search_searchtext').val();
					var objecttype = "Search";
					if(title != undefined) title=title.replace('/','@@');
					aUrl=resource.base_url;
					aUrl+='?q=learning/catalog-search/'+escape(objecttype)+"|"+escape(title);
					location.href=aUrl;
			  }else{
	 			   $("#lnr-catalog-search").data("lnrcatalogsearch").searchAction('top-search');
			  }
		   }
		}catch(e){
			// to do
		}
		})
		.attr('data-filter-id', 'top-search')
		.attr('data-filter-name', 'top-search')
		.attr('data-filter-label', Drupal.t('LBL304'));

	$('#searchTxtFrmPages').click(function() {
		try{
		if(loadProfile != 'catalog-search') {
			var title = $('#search_searchtext').val();
			var langtitle = Drupal.t("LBL304").toUpperCase();
			if((title.toLowerCase()) == (langtitle.toLowerCase()))
				title='';				
			var objecttype = "Search";
			if(title != undefined) title=title.replace('/','@@');
			aUrl=resource.base_url;
			aUrl+='?q=learning/catalog-search/'+escape(objecttype)+"|"+escape(title);
			location.href=aUrl;
	  }else{
		  $("#lnr-catalog-search").data("lnrcatalogsearch").clearSearchParam('title');
		  $("#lnr-catalog-search").data("lnrcatalogsearch").searchAction('top-search');
	  }
		}catch(e){
			// to do
		}
	});

	if($('#search_searchtext')) {
		try{
		$('#search_searchtext').autocomplete(
				"/?q=learning/catalog-autocomplete",{
				minChars :3,
				max :50, 
				autoFill :true,
				mustMatch :false,
				matchContains :false,
				inputClass :"ac_input",
				loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	}
/*	$('.find-trng-sortby').each(function() {
		$('.find-trng-sortby-link').addClass('highlight-light-gray');
		$('.find-trng-sortby-link').removeClass('highlight-light-gray');
	});
	$("#search_searchtext").click(function(){
		$(this).val("");
	});*/
		datePickerOpen = false;
		$("#ad_startdate1, #ad_startdate2").live('focus', function(){
			 try {
				datePickerOpen = true;
			 } catch(e) {
				// console.log(e, e.stack);
			 }
		});
	}catch(e){
		// to do
		// console.log(e, e.stack);
	}
});
function preventBackspace(e) {
    var evt = e || window.event;
    if (evt) {
        var keyCode = evt.charCode || evt.keyCode;
        if (keyCode === 8 || keyCode === 13) { // prevent backspace and enter
            if (evt.preventDefault) {
                evt.preventDefault();
            } else {
                evt.returnValue = false;
            }
        }
    }
}
//42055: Provide an option in the admin share widget screen to pass site url
function passUrlCommonCall(click_id){
	//console.log($("#paintContentResults").jqGrid('getGridParam', 'rowNum'))
	var iframe_url = window.location.href;
	var new_url =  iframe_url.substring(0,iframe_url.indexOf('&'));
	var redirect_url = new_url.replace('widget/catalog-search','share/training')
	var pagenumber = 	$('.ui-pg-input').val();
	var rownumber = 	$('.ui-corner-bottom #data-table-page-view .page_count_view').html();
	redirect_url = redirect_url+'&page_number='+pagenumber+'&click_id='+click_id+'&row_number='+rownumber;
	window.open(redirect_url,'_blank');
//	iframe.contentWindow.location.reload();
//	var params = [
//	              'height='+screen.height,
//	              'width='+screen.width,
//	              'fullscreen=yes' // only works in IE, but here for completeness
//	              ].join(',');
//	window.open(redirect_url, 'new window', params);
}
function replaceAll(find, replace, str) {
	  return str.replace(new RegExp(find, 'g'), replace);
}

function filtersearch(val){
	try{
	//	alert("inside filtersearch"+val);
		var srchstr ='';
		var filter ='';
		var id='';
		if (val.id == 'language' || val=='language') {
			 srchstr = $("#search_language input.language").val();
			 filter = $('.search_filter_language');
			 id = $('.languagefilter').attr('id');
		} else if (val.id == 'country' || val=='country') {
			 srchstr = $("#search_country input.country").val();
			 filter = $('.search_filter_country');
			 id = $('.countryfilter').attr('id');
		}
		if (srchstr && srchstr != Drupal.t('LBL304')){
		//	alert(srchstr);
			srchstr = srchstr.replace(/\s+/g, '');
			filter.hide();
			var div = $('#' + id + ' div[id*=' + srchstr + ']');
		  			div.show();
		} else {
			filter.show();
	          }
	}catch(e){
		//console.log(e);
		// to do
	}}
function searchKeyPress(event, obj) {
	try{
		// alert("inside press"+obj.id);
		if(event.keyCode == 13 || srch == 1){
			if (obj.id == 'language_searchtext') {
				filtersearch('language');
			} else if (obj.id == 'country_searchtext') {
				filtersearch('country');
				}
		}
	}catch(e){
		// console.log(e);
		// to do
	}
};
(function($) {

$.widget("ui.prerequisite", {
	_init: function() {
		try{
		var self = this;
		this.referObj = '$("body").data("prerequisite")';
		}catch(e){
			// to do
		}
	},
	
	// To render a popup
	render_popup : function(id) {
		try{
		var html = "<div id='prereqCatalog"+id+"' style='height:250px;width:600px;overflow:visible;'></div>";
		$("#prereqCatalog"+id).remove();
		$('body').append(html);
		
		var nHtml = "<div id='show_expertus_message' style='display:none;'></div><div id='lnr-prerequisite-container'><table id='lnr-prerequisite'></table></div>";		
		$('#prereqCatalog'+id).html(nHtml);
		
		var closeButt = {};
		var prewidth = 780;
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
		    var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
		    var lesswidth = 20;
		    if(iwidth>=800)
		    	lesswidth = 40;
		    prewidth = iwidth - lesswidth;
		}
			
		$("#prereqCatalog"+id).dialog({
			bgiframe: true,
			position :[(getWindowWidth()-750)/2,100],
			width:prewidth,
			resizable:false,
			draggable:false,
			closeOnEscape: false,
			modal: true,
			title:(Drupal.t('LBL716')+" <span id='preReqTitle'></span>"),
			buttons: closeButt,
			close: function() {
				$("#prereqCatalog"+id).remove();
				if($('.location-session-detail-clone').size()>0){
                	$('.location-session-detail-clone').remove();
                }
			},
			overlay: {
			   opacity: 0.5,
			   background: '#000000'
			 }	
		});		
		//$('#prereqCatalog'+id).parent().css('border','4px solid gray');
		}catch(e){
			// to do
		}
	},
	
	// To show a Message in the format of box 
	preMsgBox : function(msgTitle,msgTxt){
		try{
	    $('#preMsgBox-wizard').remove();	    
	    var html = '';
	    
	    html += '<div id="preMsgBox-wizard" style="display:none; padding: 0px;">';
	    html += '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html += '<tr><td height="30"></td></tr>';
	    html += '<tr>';
	    html += '<td align="center"><i>'+msgTxt+'</i></td>';
	    html += '</tr>';
	    html += '</table>';
	    html += '</div>';
	    $("body").append(html);
	    
	    $("#preMsgBox-wizard").dialog({
	        position	:[(getWindowWidth()-500)/2,100],
	        bgiframe	: true,
	        width		: 500,
	        resizable	: false,
	        modal		: true,
	        title		: SMARTPORTAL.t(msgTitle),
	        closeOnEscape : false,
	        draggable	: true,
	        overlay		: {
	           				opacity: 0.9,
	           				background: "black"
	         			  }
	    });	
	  
	    $("#msgTitle-wizard").show();	
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#msgTitle-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},
	
	getPrerequisites : function(id,divId) {
		try{
		var userId = this.getLearnerId();

		if(!userId){
			this.preMsgBox(Drupal.t("LBL721"),SMARTPORTAL.t("MSG027"));
		} else {
			this.render_popup(id);		
			this.createLoader("prereqCatalog"+id);		
			$('#prereqCatalog'+id).css("min-height","120px");
			var obj 	= this;
			var urlStr 	= this.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId);
			var objStr 	= '$("#lnr-prerequisite").data("prerequisite")';
			//Embed widget related work (Create flexible grid)
			if(Drupal.settings.widget.widgetCallback==true){
				$("#lnr-prerequisite").jqGrid({
					url		 : obj.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId),
					datatype : "json",
					mtype	 : 'GET',
					colNames :['Name','Code','Status'],
					colModel :[ {name:'Name',index:'name', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Code',index:'code', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Status',index:'status', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:10,			
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass
				});
			}else{
				$("#lnr-prerequisite").jqGrid({
					url		 : obj.constructUrl("learning/catalog-prerequisite/"+id+"/"+divId),
					datatype : "json",
					mtype	 : 'GET',
					colNames :['Name','Code','Status'],
					colModel :[ {name:'Name',index:'name', title:false, width:600,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Code',index:'code', title:false, width:200,'widgetObj':objStr,formatter:obj.paintPrereqResults},
					           {name:'Status',index:'status', title:false, width:200,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:10,			
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					width: 756,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass
				});
			}
			
			
			$("#lnr-prerequisite_toppager").css("display","none");
			// Removing vertical scroll bar in Google Chrome by VJ
			$('.ui-jqgrid-bdiv').css('overflow','hidden');
			
		}
		}catch(e){
			// to do
		}
	},
	
	callbackLoaderClass : function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;
		$('body').data('prerequisite').destroyLoader("prereqCatalog"+crsId);
		var  len = response.rows.length -1;
		$("#"+response.rows[len].id+":last > td ").css("border-bottom","0px solid");
		$("#gview_lnr-prerequisite").css("border-bottom","0px solid");
		/*-- #40207 - Jscrollpane container has more course/class --*/
		$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
		
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		//Vtip-Display toolt tip in mouse over
		vtip();
		}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	getTpPrerequisites : function(id,divId,preLevel) {
		try{
		var userId 	= this.getLearnerId();
		var initReq = (preLevel == 'course') ? 'initClassPrereq' : 'initPrereq';
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback!=true){
		  	if(this.currTheme == "expertusoneV2"){
				// var dWidth = 744;
				// var dWidth = $('.pre-select-class').width();
				var dWidth = 780;
			}else{
				var dWidth = 756;
		    }
		}
		if(!userId){
			this.preMsgBox(Drupal.t("LBL721"),SMARTPORTAL.t("MSG027"));
		} else {
			
			this.render_popup(id);		
			this.createLoader("prereqCatalog"+id);		
			$('#prereqCatalog'+id).css("min-height","120px");
			var obj = this;
			var urlStr = this.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel);
			var objStr = '$("#lnr-prerequisite").data("prerequisite")';
			//Embed widget related work (Create flexible grid)
			if(Drupal.settings.widget.widgetCallback==true){
				$("#lnr-prerequisite").jqGrid({
					url:obj.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel),
					datatype: "json",
					mtype: 'GET',
					colNames:[''],
					colModel:[ {name:'tName',index:'preReqTpDetails', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum:-1,
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:true,
					height: 'auto',
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass,
					gridComplete:obj.callbackGridPreSelectClassComplete
				});
			}else{
				$("#lnr-prerequisite").jqGrid({
					url:obj.constructUrl("learning/catalog-tpprerequisite/"+id+"/"+initReq+"/"+divId+"/"+preLevel),
					datatype: "json",
					mtype: 'GET',
					colNames:[''],
					colModel:[ {name:'tName',index:'preReqTpDetails', title:false, width:610,'widgetObj':objStr,formatter:obj.paintPrereqResults}],
					rowNum: -1,
					viewrecords:  true,
					emptyrecords: "",
					sortorder: "asc",
					toppager:false,
					height: 'auto',
					width: dWidth,
					loadtext: "",
					recordtext: "",			
					loadui:false,
					loadComplete:obj.callbackLoaderClass,
					gridComplete:obj.callbackGridPreSelectClassComplete
				});
			}
			$("#lnr-prerequisite_toppager").css("display","none");
			$(".ui-jqgrid-hbox").css("display","none");
			$(".ui-jqgrid-hdiv").css("display","none");
			// Removing vertical scroll bar in Google Chrome by VJ
			$('.ui-jqgrid-bdiv').css('overflow','hidden');
			
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				$('.ui-dialog .ui-dialog-content').addClass('pre-select-class');	
				changeDialogPopUI();
			}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	callbackGridPreSelectClassComplete: function(response, postdata, formid){
		try {
			scrollBar_Refresh('prerequisite');
			$('.limit-title').trunk8(trunk8.Prerequisite_title);
			$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
			resetFadeOutByClass('#lnr-prerequisite','content-detail-code','line-item-container','select');	
		} catch(e) {
			// to do
		}
	},
	
	paintPrereqResults : function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		if(index  == 'name') {
			$("#preReqTitle").html(' '+rowObject['parentTitle']);
			return rowObject['details1'];
		} else if(index  == 'code') {
			return rowObject['details2'];
		} else if(index  == 'status') {
			return rowObject['details3'];
		} else if(index  == 'preReqTpDetails') {
			return rowObject['preReqTpDetails'];
		} else {
			return '';
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getPrereqClass : function(currTr,divId) {
		try{
		var crsId = currTr;
		currTr = 'prereq'+currTr; 
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr).after("<tr id='"+currTr+"SubGrid' class='subClassRow'><td colspan='4' id='"+crsId+"preReqclassSubGrid' class='classSubGrid'><table id='"+currTr+"ClassSubGrid' class='subRow'></table></td></tr>");
			$("#"+currTr+"SubGrid").show();			
			$("#preReqTitle"+crsId).removeClass("title_close");
			$("#"+crsId+"prereq").addClass("subClassGridRow");			
			$("#preReqTitle"+crsId).addClass("title_open");
			$("#prereq"+crsId+" > td ").addClass("cls-grid-row-open");			
			$("#prereq"+crsId).removeClass("ui-widget-content");
			$("#"+currTr+".jqgrow").next().children().css("border-bottom","solid 1px #ccc");
			var classPrereq = this.classPrereqFun(crsId,currTr,divId);
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
				$("#preReqTitle"+crsId).removeClass("title_close");
				$("#preReqTitle"+crsId).addClass("title_open");
				$("#prereq"+crsId+" > td ").removeClass("cls-grid-row-open");
				$("#prereq"+crsId+" > td ").addClass("cls-grid-row-close");
				$("#"+crsId).removeClass("ui-widget-content");
				var classPrereq = this.classPrereqFun(crsId,currTr,divId);
				this.destroyLoader(crsId+"preReqclassSubGrid");
    		} else {			
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$("#"+currTr+"SubGrid").slideUp(1000);
				$("#preReqTitle"+crsId).removeClass("title_open");
				$("#preReqTitle"+crsId).addClass("title_close");
				$("#"+crsId).removeClass("ui-widget-content");
				$("#prereq"+crsId+" > td ").removeClass("cls-grid-row-open");
				$("#prereq"+crsId+" > td ").addClass("cls-grid-row-close");
				$("#"+crsId).addClass("ui-widget-content");
				this.destroyLoader(crsId+"preReqclassSubGrid");
    		}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	classPrereqFun : function(crsId,clsDisp,divId) {
		try{
		var html = '';
		this.createLoader(crsId+"preReqclassSubGrid");
		var obj = this;
		var objStr = '$("#lnr-prerequisite").data("prerequisite")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#"+clsDisp+"ClassSubGrid").jqGrid({
				url:obj.constructUrl("learning/class-catalog-prerequisite/" + crsId+"/"+divId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Name','Date','Start','End','Type',''],
				colModel:[ {name:'Name',index:'name', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Date',index:'date', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Start',index:'start', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'End',index:'end', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Type',index:'type', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'action',index:'action', title:false,'widgetObj':objStr,formatter:obj.paintPrereqClsResult}],
				rowNum:10,
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoaderPreReq
			});
		}else{
			$("#"+clsDisp+"ClassSubGrid").jqGrid({
				url:obj.constructUrl("learning/class-catalog-prerequisite/" + crsId+"/"+divId),
				datatype: "json",
				mtype: 'GET',
				colNames:['Name','Date','Start','End','Type',''],
				colModel:[ {name:'Name',index:'name', title:false, width:300,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Date',index:'date', title:false, width:120,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Start',index:'start', title:false, width:90,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'End',index:'end', title:false, width:90,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'Type',index:'type', title:false, width:100,'widgetObj':objStr,formatter:obj.paintPrereqClsResult},
				           {name:'action',index:'action', title:false, width:160,'widgetObj':objStr,formatter:obj.paintPrereqClsResult}],
				rowNum:10,
				viewrecords:  true,
				emptyrecords: "",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				width: 745,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoaderPreReq
			});
		}
		
		$("#prereq"+crsId+"ClassSubGrid_toppager").css("display","none");		
		$(".prereq-enrolled-txt").parent().parent().css('border','none');
		vtip();
		}catch(e){
			// to do
		}
	},
	
	callbackLoaderPreReq : function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;
		$('body').data('prerequisite').destroyLoader(crsId+"preReqclassSubGrid");
		var clsDisp = 'prereq'+crsId;
		$("#"+clsDisp+"ClassSubGrid tr.jqgrow").eq(-1).addClass("last").children("td").css("border-bottom","0px none");
		$("table.search-register-btn td").css("border-bottom","0px none");
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		//Vtip-Display toolt tip in mouse over
		 vtip();	
		}catch(e){
			// to do
		}
	},

	paintPrereqClsResult : function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		if(index  == 'name') {
			return rowObject['details4'];
		} else if(index  == 'date') {
			return rowObject['details5'];
		} else if(index  == 'start') {
			return rowObject['details6'];
		} else if(index  == 'end') {
			return rowObject['details7'];
		}else if(index  == 'type') {
			return rowObject['details8'];
		} else if(index  == 'action') {
			return rowObject['clsaction'];
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getPrereqClassDetail : function(currTr) {
		try{
		var crsId = currTr;
		currTr = 'preReqClassDet'+currTr;
		
		if(!document.getElementById(crsId+"ClassSubGrid")) {
			$("#"+currTr).after("<tr id='"+crsId+"ClassSubGrid'><td colspan='6'><span id='"+crsId+"clsDesc'>Loading . . . </span></td></tr>");
			$("#preReqClsTitle"+crsId).removeClass("title_close");
			$("#preReqClsTitle"+crsId).addClass("title_open");
			if($("#"+currTr).hasClass("last")==false) {
				$("#"+currTr+" td").css("border-bottom","0px none");
				$("#"+currTr+" td").parents("tr.jqgrow").next().children().css("border-bottom","solid 1px #ccc");
			}
			var classPrereq = this.showClassDetails(crsId);
		} else {
			var clickStyle = $("#"+crsId+"ClassSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
				$("#"+crsId+"ClassSubGrid").slideDown(1000);   
				$("#preReqClsTitle"+crsId).removeClass("title_close").addClass("title_open");
				$("#"+crsId).removeClass("ui-widget-content");
				$("#"+currTr+".jqgrow").children().css("border-bottom","0px none");
				var classPrereq = this.showClassDetails(crsId);
    		} else {			
    			$("#"+crsId+"ClassSubGrid").slideUp(1000);  
    			$("#preReqClsTitle"+crsId).removeClass("title_open").addClass("title_close");
				$("#"+crsId).removeClass("ui-widget-content").addClass("ui-widget-content");
				if($("#"+currTr).hasClass("last") == false)
				$("#"+currTr+".jqgrow").children().css("border-bottom","solid 1px #ccc");
    		}
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	getTpPrereqCatalog : function(id,divId,catalogType,preLevel,parentId) {
		try{
		var crsId 		= id;
		//var gridRowId 	= catalogType+'prereq'+id;
		var gridRowId 	= catalogType+id+preLevel;
		var newGridId	= gridRowId+'SubGrid';
		var loaderDivId = gridRowId+'level';
		var jqGridTabId = gridRowId+'jgGrid';
		var jgTitleId   = "preTitle"+gridRowId;
		$.fn.extend({
			toggleText: function(a, b){
				return this.text(this.text() == b ? a : b);
			}
		});
		if(!document.getElementById(newGridId)) {
		//	alert("enter");
			$("#"+jgTitleId).before("<div id='"+newGridId+"' class='subClassRow' 	><div id='"+loaderDivId+"' class='classSubGrid'><div id='gview_lnr-tpprerequisite' class='gview_lnr-tpprerequisite'><table id='"+jqGridTabId+"' class='subRow'></table></div></div></div>");
			$("#"+newGridId).show();			
			
			// this.styleSwap(jgTitleId,'title_open','title_close');
			
			$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
			$("#"+gridRowId+" > td ").addClass("cls-grid-row-open");			
			$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #ccc");
			$("#"+gridRowId+" tr:last > td ").css("border-bottom","solid 0px #ccc");
			$("#lnr-prerequisite tr:last > td ").css("border-bottom","solid 0px #ccc");
				
			var classPrereq = this.getTpPrereqList(id,divId,catalogType,preLevel);
			
			$("#gview_"+jqGridTabId).css("border-bottom","solid 0px #ccc");
			if(this.currTheme == 'expertusoneV2') {
				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
					if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
					}
					else{
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
					}
					
					
				$("#gview_"+jqGridTabId+" .ui-jqgrid-hdiv").css("border-bottom","solid 1px #EDEDED");
			}else
			{
				if(navigator.userAgent.indexOf("Chrome")>0){
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
				}else{
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px"); 
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
				}
				
				$("#gview_"+jqGridTabId+" .ui-jqgrid-hdiv").css("border-bottom","solid 1px #ccc");
			}
			$('body').data('prerequisite').createLoader("gview_"+jqGridTabId);
			
		} else {
			//alert("close");
			var clickStyle = $("#"+newGridId).css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			//alert("close001");
    			$("#"+newGridId).slideDown(1000, function() {
    				/*-- #40207 - Jscrollpane container has more course/class --*/
					$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
    			});
    			// this.styleSwap(jgTitleId,'title_open','title_close');
				$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
    			if(this.currTheme == 'expertusoneV2') {
    				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
    				if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
    				}else{
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
    				}
    					
    			$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #EDEDED");
    			}
    			else
    			{
    				if(navigator.userAgent.indexOf("Chrome")>0){
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
    				}else{
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px"); 
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
    				}
    			
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 0px #ccc");
    			}
    		} else {			
    			//alert("open");
    			$("#"+newGridId).slideUp(1000, function() {
    				/*-- #40207 - Jscrollpane container has more course/class --*/
					$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
    			});
    			// this.styleSwap(jgTitleId,'title_close','title_open');
				$("#"+jgTitleId).toggleText(Drupal.t('LBL713'), Drupal.t('LBL3042'));
    			if(this.currTheme == 'expertusoneV2') {
    				if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")>0 ){
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","25px");
        			if(navigator.userAgent.indexOf("Safari")>0){
					$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","12px");
				      }
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","25px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","25px");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
        			}else{
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","24px"); 
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","42px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","0px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
    				$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","42px");
        			}
        			
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 1px #EDEDED");
    				$("#lnr-prerequisite").find("tr:last-child td ").css("border-bottom","solid 0px #EDEDED");
    			}
    			else
    			{
    				if(navigator.userAgent.indexOf("Chrome")>0){
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","7px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","15px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
        			}else{
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").find(".radio-btn:not(input)").css("margin-right","-6px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find(".top-record-div-left:not(.title_close)").css("margin-right","30px");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr").find("#coursel2").find(".top-record-div-left").css("margin-right","0");
        			$(".title_open").parents(".cls-grid-row-open").parent("tr").next("tr.subClassRow").next("#coursel2").find(".top-record-div-left").css("margin-right","30px");
        			}
        		
    				$("#"+gridRowId+" > td ").css("border-bottom","solid 1px #ccc");
    				$("#lnr-prerequisite").find("tr:last-child td ").css("border-bottom","solid 0px #ccc");
    			}
    		}
		}
		vtip();
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-class-level-container','content-detail-code','line-item-container','select');
		}catch(e){
			// to do
			// window.console.log(e, e.stack);
		}
	},
	
	styleSwap : function(classId,addClass,removeClass) {
		try{
		$("#"+classId).removeClass(removeClass);
		$("#"+classId).addClass(addClass);
		}catch(e){
			// to do
		}
	},
	
	getTpPrereqList : function(id,divId,catalogType,preLevel) {
		try{
		var divId = ((divId == preLevel) ? 'lnr-catalog-search' : divId);
		var dispGridId, constructUrl, colNames , colModel,loadCompleteCall;
		//var dispGridId 		 = catalogType+'prereq'+id+'jgGrid'; //initPrereq
		var dispGridId 		 = catalogType+id+preLevel+'jgGrid'; //initPrereq
		
		var constructUrl 	 = this.constructUrl("learning/catalog-tpprerequisite/" + id+"/"+catalogType+"/"+divId+"/"+preLevel);
		var colNames		 = [''];
		var obj 			 = this;
		var objStr 			 = '$("#lnr-prerequisite").data("prerequisite")';
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			var colModel		 = [ {name:'tName',index:'preReqTpDetails', title:false,'widgetObj':objStr,formatter:obj.paintPrereqResults}];
		}else{
			var colModel		 = [ {name:'tName',index:'preReqTpDetails', title:false, width:600,'widgetObj':objStr,formatter:obj.paintPrereqResults}];
		}
		
		var loadCompleteCall = obj.prereqCallback;
		this.jqGridFun(dispGridId, constructUrl, colNames , colModel,loadCompleteCall);
		vtip();
				}catch(e){
			// to do
		}
	},
	
	getTpPrereqCatalogRegister : function(tpCrsId,objectId,str) {
		try{
		var tpCrsId = tpCrsId.split('-');
		
		var tpCrsIdV, selectedClass,inc = 0,v = 0;
		var checkedClass = new Array();
		var tpCrsIdLen	 = tpCrsId.length;
		var tpAction 	= $('#tpaction-'+objectId).val();
		var tpActionV 	= tpAction.split("-");
		tpActionType 	= tpActionV[0];
		tpNodeId		= tpActionV[1];
		var tpSelectedClass = '';
		
		var params = 'selectedItem={';
		for(var n = 0; n < tpCrsIdLen ; n++) {			
			tpCrsIdV 		= tpCrsId[n];
			inc=inc+1;
			selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();
			
			if(selectedClass != undefined) {
				checkedClass[n]	= selectedClass;
				tpSelectedClass	= selectedClass+tpSelectedClass;
			}
			checkedClass	= checkedClass
			
			if(selectedClass == undefined || selectedClass == 'undefined') {				
				selectedClass='NULL';
				break;
			}
			//if($("#cls-"+tpCrsIdV).attr("disabled") == false) {
				params += '"'+v+'":'+'{';				
				params += '"tpid":"'+objectId+'",';
				params += '"courseid":"'+tpCrsIdV+'",';
				params += '"classid":"'+selectedClass+'"';
				params += '}';
				
				if(inc < tpCrsIdLen) {
					params += ',';							
					tpSelectedClass = ','+tpSelectedClass;
				 }
				v = v+1;
			//}
			
		}
		
		params += '}';
		
		if(tpCrsIdLen != checkedClass.length) {
			var errMsgPre = new Array();
			errMsgPre[0] = str;
			var message_call = expertus_error_message(errMsgPre,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
		} else {/*
			clsaction 		= $('#clsaction-'+courseId).val();
			var clsactionV 	= clsaction.split("-");
			clsActionType 	= clsactionV[0];
			clsNodeId		= clsactionV[1];
			alert("clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
			*/
			
			if(tpActionType == "cart") {
				this.addToCartFun("tp",tpNodeId,objectId,tpSelectedClass);
			} else {
				var isAdminSide = 'N'; 
				var userListIds = ''; 
				var fromPage = 'catalogpopup'; 
				var MasterMandatory = 'N'; 
				var url = this.constructUrl("ajax/trainingplan/class-list-register/"+isAdminSide+ "/" + userListIds+ "/" + fromPage+ "/" + MasterMandatory);		
				var obj = this;
				$.ajax({
					type: "POST",
					url: url,
					data:  params,
					datatype: 'text',
					success: function(result){
						result = $.trim(result);
						var resultErr = new Array();
						if(result == 'Registered') {
							resultErr[0] = Drupal.t("Registered Successfully");
							var message_call = expertus_error_message(resultErr,'error');
							$('#show_expertus_message').html(message_call);
							$('#show_expertus_message').show();
							
					   } else {
						   resultErr[0] = result; 
						   var message_call = expertus_error_message(resultErr,'error');
							$('#show_expertus_message').html(message_call);
							$('#show_expertus_message').show();
					   }
						
					}
			    });
			}
		} 
		}catch(e){
			// to do
		}
	},
	
	//callRegisterClass : function(widgetId,userId, courseId, classId) {
	getPrereqCatalogRegister : function(tpCrsIdv,courseId,str) {
		try{
		var tpCrsId = tpCrsIdv.split('-');
		var tpCrsIdLen = tpCrsId.length;
		var inc = 0;
		var selectedClass,clsaction;
		var clsActionType, clsNodeId;
		
		
		if(tpCrsIdLen == 1) {
			selectedClass 	= $('input[name="cls-'+tpCrsId[0]+'"]:radio:checked').val();
			if(selectedClass != undefined) {
				clsaction 		= $('#clsaction-'+selectedClass).val();
				var clsactionV 	= clsaction.split("-");
				clsActionType 	= clsactionV[0];
				clsNodeId		= clsactionV[1];
				//alert("clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
				//addToCartFun(actionType,nodeId,courseId,classId)
				if(clsActionType == "cart" && availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "") {
				//if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != ""){	
					
					this.addToCartFun("class",clsNodeId,tpCrsId[0],selectedClass);
				} else {
					this.prereqCatalogRegister(tpCrsId[0],selectedClass);
				}
			}
		} else {
			for(var n = 0; n < tpCrsId.length; n++) {			
				tpCrsIdV 		= tpCrsId[n];
				courseId		= tpCrsIdV;
				selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();
				if(selectedClass != undefined) {
					//checkedClass[n]	= selectedClass;
					if($("#cls-"+courseId).attr("disabled") == false) {
						clsaction 		= $('#clsaction-'+courseId).val();
						var clsactionV 	= clsaction.split("-");
						clsActionType 	= clsactionV[0];
						clsNodeId		= clsactionV[1];
						//alert("multiple clsaction : "+clsActionType+" | clsNodeId: "+clsNodeId);
						
						if(clsActionType == "cart") {
							this.addToCartFun("class",clsNodeId,courseId,selectedClass);
						} else {
							this.prereqCatalogRegister(courseId,selectedClass);
						}
						inc=inc+1;
					}
				}
				
			}
		}
		
		if(((tpCrsIdLen == 1) && (selectedClass == undefined)) || ((tpCrsIdLen > 1) && (inc == 0))) {
			var MsgErr = new Array();
			MsgErr[0] = str;
			var message_call = expertus_error_message(MsgErr,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
		} 
			// #43510 - refresh the catalog page for button status update
			var currentPage = $('.ui-pg-input').val();
			if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
				$("#lnr-catalog-search #paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
			}
		}catch(e){
			// to do
		}
	},
	prereqCatalogRegister : function(courseId,classId) {
		try{
		var obj = this;
		 
		var userId = this.getLearnerId();			
		var isAdminSide = 'N';
		var waitlist = 1;
			//this.createLoader(loaderObj);
			url = obj.constructUrl("ajax/learningcore/register/" + userId + '/' + courseId + '/' + classId+'/'+waitlist+'/'+isAdminSide);
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
				result = $.trim(result);
				   /*if(userId!=0)
					   obj.callAvailableSeats(userId, courseId, classId);*/
					var preMsg = new Array();
				   if(result == 'Registered') {
					   preMsg[0] = Drupal.t("Registered Successfully");
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();					   
					   $('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);
					   
					   if(document.getElementById('clsStatus-'+courseId)) {
						   $("#clsStatus-"+courseId).html(SMARTPORTAL.t("Registered"));
						   $("#clsStatus-"+courseId).attr("title",SMARTPORTAL.t("Registered"));
					   }
					   
				   }else if(result == 'Waitlisted'){ // Waitlist Message Ticket no :  0021486
					   preMsg[0] = Drupal.t("Waitlisted Successfully");
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();	
					   $('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);
					   
					   if(document.getElementById('clsStatus-'+courseId)) {
						   $("#clsStatus-"+courseId).html(SMARTPORTAL.t("Waitlisted"));
						   $("#clsStatus-"+courseId).attr("title",SMARTPORTAL.t("Waitlisted"));
					   }
				   } 
				   else{
					   preMsg[0] = result;
					   var message_call = expertus_error_message(preMsg,'error');
					   $('#show_expertus_message').html(message_call);
					   $('#show_expertus_message').show();
					   //obj.callMessageWindow('registertitle',result);
				   }
				  // obj.destroyLoader(loaderObj);
				}
		    });
		}catch(e){
			// to do
		}
	},
	
	//addToCartFun : function(widgetId,act,ObjectId,addFrom,class_ids) {
	addToCartFun : function(actionType,nodeId,courseId,classId) {
		try{
		//var loaderObj = widgetId ;
		var waitlist = 0;
		var programId = (actionType == "tp") ? courseId : 0;
		 /*
		 if(addFrom == 'Cart'){
			var classData 	= eval($('#object-addToCartList_'+ObjectId).attr("data"));
		 }else{
			 var classData 	= eval($('#addToCartList_'+ObjectId).attr("data"));
		 }
			var LMSNodeId 	= classData.NodeId;
			var classId	 	= addFrom == 'Cart' ? classData.TpId : classData.ClassId;
			var courseId	= classData.CourseId;

			var obj = this;
			this.createLoader(loaderObj);
			*/
			//if(addFrom == 'Cart'){
			if(actionType == "tp") {
				//var class_ids = '';
				/*if(class_ids ==''){
					class_ids = 0;
				}*/
				url =  this.constructUrl("ajax/cart/tpproduct/add/" + nodeId + "/" + programId + "/" + 'null' + "/" + classId);
			}
			else{
				url =  this.constructUrl("ajax/cart/product/add/" + nodeId + "/" + classId + "/" + courseId + "/" + waitlist);
			}

			$.ajax({
				type: "POST",
				url: url,
				// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					//alert("result : "+result.toSource());
					
					//obj.destroyLoader(loaderObj);
					var MsgPre = new Array();
					if(result.cart_msg == 'CartAdded'){
						MsgPre [0] = Drupal.t("Added to your Cart");
						var message_call = expertus_error_message(MsgPre,'error');
						$('#show_expertus_message').html(message_call);
						$('#show_expertus_message').show();
					} else {
						MsgPre [0] = result.cart_msg;
						var message_call = expertus_error_message(MsgPre,'error');
						$('#show_expertus_message').html(message_call);
						$('#show_expertus_message').show();					
					}
					if (parseInt(result.total_product) > 0) {
						$("#ShoppingCartItemsId").addClass("filled");
						$("#ShoppingCartItemsId").html(result.total_product);
					} else {
						$("#ShoppingCartItemsId").removeClass("filled");
						$("#ShoppingCartItemsId").html('');
					}
				}
				
		    });
		}catch(e){
			// to do
		}
	},
	
	jqGridFun : function(dispGridId, constructUrl, colNames , colModel,loadCompleteCall) {
		try{
		this.createLoader(dispGridId);
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			$("#"+dispGridId).jqGrid({
				url		 	 : constructUrl,
				datatype 	 : "json",
				mtype	 	 : 'GET',
				colNames 	 : colNames,
				colModel  	 : colModel,
				rowNum	 	 : 10,
				viewrecords  : true,
				emptyrecords : "",
				sortorder	 : "asc",
				toppager	 : true,
				height		 : 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext	 : "",
				recordtext	 : "",
				loadui		 : false,
				loadComplete : loadCompleteCall
			});
		}else{
			$("#"+dispGridId).jqGrid({
				url		 	 : constructUrl,
				datatype 	 : "json",
				mtype	 	 : 'GET',
				colNames 	 : colNames,
				colModel  	 : colModel,
				rowNum	 	 : 10,
				viewrecords  : true,
				emptyrecords : "",
				sortorder	 : "asc",
				toppager	 : true,
				height		 : 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext	 : "",
				recordtext	 : "",
				loadui		 : false,
				loadComplete : loadCompleteCall
			});
		}
		
		$("#"+dispGridId+"_toppager").css("display","none");
		$(".ui-jqgrid-hbox").css("display","none");	
		//$(".ui-jqgrid-hdiv").css("display","none");
		}catch(e){
			// to do
		}
	},
	
	prereqCallback :  function(response, postdata, formid) {
		try{
		var crsId = response.rows[0].cell.crsId;		
		if(document.getElementById('cls-'+crsId)) {
			if($('input[name="cls-'+crsId+'"]:radio').is(":checked") == true)	{
				$('input[name="cls-'+crsId+'"]:radio').attr('disabled',true);
			}
		}
		var len = response.rows.length - 1; 
		var len1 = response.rows.length - 2;
		$("tr #"+response.rows[len].id+":last > td ").css("border-bottom","0px solid");
		// When course has no class, loader icon loading issue fix.
		if (response.rows[len].id == 'coursel3' || response.total==0) {
			var parentId = $('#lnr-prerequisite .loadercontent').parent().attr('id');
		} else {
		var parentId = $("tr #"+response.rows[0].id+"").parent().parent().parent().parent().parent().attr("id");
		}
		
		$('body').data('prerequisite').destroyLoader(parentId);

		/*if(response.rows[len].cell.registerBtn == false) {
			$("tr #"+response.rows[len].id+":last ").css("border-bottom","1px solid #cccccc");
			//$("#"+parentId).css("border-bottom","1px solid #cccccc");
			
		} else if(len1 > 0){
			if($("tr #"+response.rows[len1].id+":last").next().find("table").attr("id") == undefined) {
				$("tr #"+response.rows[len1].id+":last").next().css("display","none");
				$("tr #"+response.rows[len1].id+":last > td ").css("border-bottom","0px solid #cccccc");
			}
		}*/
		//$("#gview_lnr-prerequisite").css("border-bottom","0px solid");		
		//$('body').data('prerequisite').destroyLoader("prereqCatalog"+crsId);
		$('.limit-title').trunk8(trunk8.Prerequisite_title);
		$('.limit-desc').trunk8(trunk8.Prerequisite_desc);
		$('.lmt-des-prereq-title').trunk8(trunk8.Prerequisite_title);
		/*-- #40207 - Jscrollpane container has more course/class --*/
		$("body").data("learningcore").refreshJScrollPane('#lnr-prerequisite-container #gview_lnr-prerequisite');
		//$('.fade-out-title-container.assignlearn-tp-cls-session-title-fadeout-container').css('max-width','500px');
		vtip();
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-class-level-container','content-detail-code','line-item-container','select');
		resetFadeOutByClass('#lnr-prerequisite .tp-prereq-select-course-level-container','content-detail-code','line-item-container','select');
	
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	
	getPreqClassId : function(classId,crsId) {
		try{
		var seatsleft  = $("#hidden-seatleft-cls-"+classId).html();
		    $("#seatleftcount"+crsId).html(seatsleft);
		}catch(e){
			// to do
		}
	},
	showClassDetails : function(clsId) {
		try{
		var oHtml = '';
		var currTr = 'preReqClassDetils'+clsId;
		
		if(document.getElementById(currTr)) {
			var data = eval($("#"+currTr).attr("data"));
			var dataInfo = data;
			var desc = data.description; 
			oHtml += "<table cellpadding='2' cellspacing='2' width='100%'>";
			if(data.cls_code) {
				oHtml += "<tr><td class='class-code-heading'>"+SMARTPORTAL.t("Class Code")+": "+data.cls_code+"</td></tr>";
			}
			oHtml += "<tr><td class='cls-description'>"+desc+"</td></tr><tr><td class='cls-description'>";
			if(dataInfo.sessionDetails.length>0) {
				oHtml += '<div class="enroll-session-details">'+SMARTPORTAL.t("Session Details")+':</div>';
				var inc = 1;
				$(dataInfo.sessionDetails).each(function(){ //sessionDate sessionDay
					if($(this).attr("sessionTitle")) {
						var sesionsH = ($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;"; 
						var sesionsHead = titleRestricted(sesionsH,15);
						
						oHtml += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;")+'">'+sesionsHead+'</div>';
						oHtml += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
						oHtml += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
						inc++;
					}
				});
			}
			if(data.language) {
				oHtml += "<div class='cls-lang'>"+SMARTPORTAL.t("Language")+": "+data.language+"</div>";
			}
			var LocationName 	= dataInfo.locationDetails.locationName; 
			if(LocationName != '') {
				oHtml +='<table border="0" cellpadding="0" class="enroll-loc-details" cellspacing="0"><tr><td class="enroll-location-head" valign="top"><div class="enroll-loc-head">'+SMARTPORTAL.t("Location")+':</div></td><td></td></tr><tr><td>';
				
				oHtml += "<div class='enroll-location-text'>"+LocationName+"</div>";
				if(dataInfo.locationDetails.locationAddr1 !='' && dataInfo.locationDetails.locationAddr1 != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationAddr1+"</div>";
				}
				if(dataInfo.locationDetails.locationAddr2 !='' && dataInfo.locationDetails.locationAddr2 != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationAddr2+"</div>";
				}
				if(dataInfo.locationDetails.locationCity !='' && dataInfo.locationDetails.locationCity != null) {
					oHtml += "<div class='enroll-location-text'>"+dataInfo.locationDetails.locationCity;
					if (dataInfo.locationDetails.locationState == '' && dataInfo.locationDetails.locationState != null) {
						oHtml += "<br />";
					}
				}
				if(dataInfo.locationDetails.locationState !='' && dataInfo.locationDetails.locationState != null) {
					if (dataInfo.locationDetails.locationCity != '' && dataInfo.locationDetails.locationCity != null) {
						oHtml += " , ";
					}
					oHtml += dataInfo.locationDetails.locationState+"</div>";
				}
				if(dataInfo.locationDetails.locationZipcode !='' && dataInfo.locationDetails.locationZipcode != null){
					oHtml += "<div class='enroll-location-text'>zip"+dataInfo.locationDetails.locationZipcode+"</div>";
				}

				oHtml += "</td></tr></table>";
			}
		
			oHtml += "</td></tr></table>"; 
			$("#"+clsId+"clsDesc").html(oHtml);
		}
		vtip();
		}catch(e){
			// to do
		}
	},
	
	
	
	});

$.extend($.ui.prerequisite.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
	$("body").prerequisite();
	}catch(e){
		// to do
	}
});;
/**
 * Cookie plugin
 *
 * Copyright (c) 2006 Klaus Hartl (stilbuero.de)
 * Dual licensed under the MIT and GPL licenses:
 * http://www.opensource.org/licenses/mit-license.php
 * http://www.gnu.org/licenses/gpl.html
 *
 */

/**
 * Create a cookie with the given name and value and other optional parameters.
 *
 * @example $.cookie('the_cookie', 'the_value');
 * @desc Set the value of a cookie.
 * @example $.cookie('the_cookie', 'the_value', { expires: 7, path: '/', domain: 'jquery.com', secure: true });
 * @desc Create a cookie with all available options.
 * @example $.cookie('the_cookie', 'the_value');
 * @desc Create a session cookie.
 * @example $.cookie('the_cookie', null);
 * @desc Delete a cookie by passing null as value. Keep in mind that you have to use the same path and domain
 *       used when the cookie was set.
 *
 * @param String name The name of the cookie.
 * @param String value The value of the cookie.
 * @param Object options An object literal containing key/value pairs to provide optional cookie attributes.
 * @option Number|Date expires Either an integer specifying the expiration date from now on in days or a Date object.
 *                             If a negative value is specified (e.g. a date in the past), the cookie will be deleted.
 *                             If set to null or omitted, the cookie will be a session cookie and will not be retained
 *                             when the the browser exits.
 * @option String path The value of the path atribute of the cookie (default: path of page that created the cookie).
 * @option String domain The value of the domain attribute of the cookie (default: domain of page that created the cookie).
 * @option Boolean secure If true, the secure attribute of the cookie will be set and the cookie transmission will
 *                        require a secure protocol (like HTTPS).
 * @type undefined
 *
 * @name $.cookie
 * @cat Plugins/Cookie
 * @author Klaus Hartl
 */

/**
 * Get the value of a cookie with the given name.
 *
 * @example $.cookie('the_cookie');
 * @desc Get the value of a cookie.
 *
 * @param String name The name of the cookie.
 * @return The value of the cookie.
 * @type String
 *
 * @name $.cookie
 * @cat Plugins/Cookie
 * @author Klaus Hartl
 */
jQuery.cookie = function(name, value, options) {
    if (typeof value != 'undefined') { // name and value given, set cookie
        options = options || {};
        if (value === null) {
            value = '';
            options.expires = -1;
        }
        var expires = '';
        if (options.expires && (typeof options.expires == 'number' || options.expires.toUTCString)) {
            var date;
            if (typeof options.expires == 'number') {
                date = new Date();
                date.setTime(date.getTime() + (options.expires * 24 * 60 * 60 * 1000));
            } else {
                date = options.expires;
            }
            expires = '; expires=' + date.toUTCString(); // use expires attribute, max-age is not supported by IE
        }
        // CAUTION: Needed to parenthesize options.path and options.domain
        // in the following expressions, otherwise they evaluate to undefined
        // in the packed version for some reason...
        var path = options.path ? '; path=' + (options.path) : '';
        var domain = options.domain ? '; domain=' + (options.domain) : '';
        var secure = options.secure ? '; secure' : '';
        document.cookie = [name, '=', encodeURIComponent(value), expires, path, domain, secure].join('');
    } else { // only name given, get cookie
        var cookieValue = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            for (var i = 0; i < cookies.length; i++) {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    }
};;
(function($){
	$.fn.qtipPopup = function(qtipObj){ // convert to json
		try{
			var ptype = qtipObj.postype;
			if(ptype.indexOf('custom') != -1)
				$(this).qtipCustomLeft(qtipObj);
			else{
				var theme = Drupal.settings.ajaxPageState.theme;
				var qwid = qtipObj.wid;	
				var mlwid = qtipObj.poslwid;
				var mrwid = qtipObj.posrwid;
				var qheg = qtipObj.heg;
				var popupDispId = qtipObj.popupDispId;
				var onClsFn = qtipObj.onClsFn;
				var entId = qtipObj.entityId;
				var disp = qtipObj.disp;
				var lId = qtipObj.linkid;
				var dispDown= qtipObj.dispDown;
				var posmin;
			var setTop=qtipObj.top;
				var catalogVisibleId = qtipObj.catalogVisibleId;	
			var beforeShow = qtipObj.beforeShow;
			var afterShow = qtipObj.afterShow;
			var afterPosition = qtipObj.afterPosition;
			
				setTimeout(function(){
					var lheg = $('#'+lId).height();
					var lwid = $('#'+lId).width();
					
					var popoff = $('#'+lId).offset();
					var lpos = $('#'+lId).position();
				var tcls = (((popoff.top - qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? 'bottom-qtip-tip-visible' : 'bottom-qtip-tip-up-visible';
					
				$(this).qtipPopupShow(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj); // pop up
					
					var theg = $('.' + tcls).height();
					var twid = $('.' + tcls).width();
					var llwid = lwid / 2;
					var tpos = lpos.top + (lheg + theg - 12);
					var orgtpos = $('.'+tcls).position();
				if (setTop === undefined || setTop === null) 
					var ttop = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? (lpos.top-lheg) : (lpos.top + lheg);
				else
					var ttop =setTop;
					var z = lpos.left + (parseInt(llwid) - (twid / 2));
					
					$(this).tipPositioning(tcls,ttop,z,popupDispId); 
					if(popupDispId.indexOf("qtip_owner_disp") === 0)
					{
						if($("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").length > 0)
						{
							$("div[id^='qtip_owner_disp']").find("span[class='qtip-popup-visible']").addClass("qtip-popup-visible_for_ie");
						}
					}
					
					if(theme == 'expertusoneV2'){
						var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
						if(catalogVisibleId =='enrolled-all-exempted-disp' || catalogVisibleId == 'qtipAttachIdqtip_exempted_disp_all' || catalogVisibleId == 'qtipAttachIdqtip_exempted_single_disp'){
							posmin = $(this).qtipExemptedPosition(catalogVisibleId);
						}else{
							if($.browser.msie && $.browser.version == 10) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 11) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 9) 
								posmin = 8;
							else if($.browser.msie && $.browser.version == 8)
								posmin = 8;
							else if(navigator.userAgent.indexOf("Chrome")>0) 
								posmin = 8;
							else
								posmin = 8;
						}
					}
					//var posmin = (Drupal.settings.ajaxPageState.theme == 'expertusone') ? 13 : 8;
					
					var lps = (lpos.top == 0 ) ? lpos.top : lpos.top-lheg;
					
				    if(lId.indexOf("widget-share-")==0 )
					  lps = (parseInt(lpos.top) == 0 ) ? lpos.top : lpos.top-lheg; 
			       
			        var ppos = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? (lps+theg-posmin) : tpos;
					var post = (((popoff.top-qheg) >= (qheg/2) || disp=='ctool') && dispDown !='Y') ? 'bottom' : 'top';
					if(ptype == 'middle'){
						try{
							$(this).qtipMiddle(ppos,qwid,orgtpos,mlwid,mrwid,llwid,twid,lpos,popupDispId,entId,post); //Middle
							
						}catch(e){
							alert(e);
						}
					}else if(ptype == 'bottomleft' || ptype == 'topleft'){
						try{
							$(this).qtipLeft(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post); //left
						}catch(e){
							alert(e);
						}
					}else if(ptype == 'bottomright' || ptype == 'topright'){
						try{
							$(this).qtipRight(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post); //right
						}catch(e){
							alert(e);
						}
					}

					if(qtipObj.afterPosition !== undefined) {
						qtipObj.afterPosition.call();
					}
				},10)
			}
		}catch(e){
			//alert(e);
		}

	};
	
	$.fn.qtipPopupShow = function(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj){
		try{
			var bpTop = '<div class="'+tcls+'"></div> <table cellspacing="0" cellpadding="0" style="z-index:100;" height="'+qheg+'px" width="'+qwid+'px" id="bubble-face-table"> <tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"><a id="admin-bubble-close" class="qtip-close-button-visible" onclick="closeQtip(\''+popupDispId+'\',\''+entId+'\','+onClsFn+');"></a></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
			var bpBot = '</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b-visible"></td><td class="bubble-br"></td></tr></tbody></table>';
			var paintHtml = bpTop+"<div id='paintContentVisiblePopup'><div id='show_expertus_message'></div><div id='paintContent"+popupDispId+"'></div></div>"+bpBot;
			$('#'+popupDispId+' #visible-popup-'+entId).html(paintHtml);
			if(qtipObj.beforeShow !== undefined) {
				qtipObj.beforeShow.call();
			}
			$('#'+popupDispId+' #visible-popup-'+entId).show();
			if(qtipObj.afterShow !== undefined) {
				qtipObj.afterShow.call();
			}
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.tipPositioning = function(tcls,ttop,z,popupDispId){
		try{
			$('#'+popupDispId+' .' + tcls).css('position', 'absolute').css('top', ttop+'px').css('left', z+'px');
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.qtipMiddle = function(ppos,qwid,orgtpos,mlwid,mrwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == '' && mrwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft = qpos + parseInt(m) + parseInt(qm) - (llwid - (twid / 2));
			}else{
				if(mlwid !=''){
					tipleft = qpos + parseInt(mlwid);
				}else{
					tipleft = qpos - mrwid;
				}
			}
			var tleft = orgtpos.left - tipleft + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
			
		}catch(e){
			alert(e);
		}
		
	};
	
	$.fn.qtipLeft = function(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
		//alert(ppos);
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft =  (m) + (qm) - (llwid - (twid / 2));
			}else{
				tipleft1 = mlwid;
				if(tipleft1 > 0){
					tipleft = tipleft1;//(llwid + (twid / 2)) - mwid;
				}
			}
			
			var tleft = orgtpos.left - tipleft + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
		}catch(e){
			alert(e);
		}
	};
	
	$.fn.qtipRight = function(ppos,qwid,orgtpos,mlwid,llwid,twid,lpos,popupDispId,entId,post){
		try{
			var tipleft = 0;
			var qpos = (qwid-40)/3;
			if(mlwid == ''){
				var m = qpos/3;
				var qm = m/2;
				tipleft = (qpos*2) + parseInt(m) + parseInt(qm) - (llwid - (twid / 2));
			}else{
				var d = (qpos*2);
				var balpos = qwid - (llwid + (twid / 2) +10);
				tipleft1 = d + parseInt(mlwid);
				if(tipleft1 > balpos){
					tipleft = (llwid + (twid / 2)) + d;
				}else{
					tipleft = tipleft1;
				}
			}
			var tleft = orgtpos.left - tipleft -10 + lpos.left;
			if(post == 'bottom')
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('bottom',ppos+'px').css('left',tleft+'px');
			else
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top',ppos+'px').css('left',tleft+'px');
		}catch(e){
			alert(e);
		}
	};
	
	

	$.fn.qtipExemptedPosition =  function(catalogVisibleId) {
		try {
			var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
			if(catalogVisibleId=='enrolled-all-exempted-disp'){
				if($.browser.msie && $.browser.version == 10) 
					posmin = 32;
				else if(isAtLeastIE11) 
					posmin = 32;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 12;
				else if(navigator.userAgent.indexOf("Chrome")>0) 
					posmin = 32;
				else
					posmin = 8;	
			}else if(catalogVisibleId=='qtipAttachIdqtip_exempted_single_disp'){
				if($.browser.msie && $.browser.version == 10) {
					posmin = 18;
				}
				else if(isAtLeastIE11) 
					posmin = 18;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 18;
				else
					posmin = 15;
			}else if(catalogVisibleId == 'qtipAttachIdqtip_exempted_disp_all'){
				if($.browser.msie && $.browser.version == 10) 
					posmin = 21;
				else if(isAtLeastIE11) 
					posmin = 21;
				else if($.browser.msie && $.browser.version == 9) 
					posmin = 8;
				else
					posmin = 21;
			}
			return posmin;
		}catch(e){
			alert(e);
		}
	};
	$.fn.qtipCustomLeft = function(qtipObj){
		try{
			// Collect qtipObj values
			var theme = Drupal.settings.ajaxPageState.theme;
			var qwid = qtipObj.wid;	
			var mlwid = qtipObj.poslwid;
			var mrwid = qtipObj.posrwid;
			var qheg = qtipObj.heg;
			var popupDispId = qtipObj.popupDispId;
			var onClsFn = qtipObj.onClsFn;
			var entId = qtipObj.entityId;
			var disp = qtipObj.disp;
			var lId = qtipObj.linkid;
			var dispRight= qtipObj.dispDown;
			var posmin;
			var catalogVisibleId = qtipObj.catalogVisibleId;
			var psubtype = qtipObj.possubtype;
			setTimeout(function(){
				psubtype = (psubtype==null || psubtype == undefined )?'middleLeft':psubtype;
				// Get positions of the triggering link
				var lpos = $('#'+lId).position();
				var lheg = $('#'+lId).height();
				var lwid = $('#'+lId).width();
				var popoff = $('#'+lId).offset();
				
				//Class name of the qtip
				var tcls = 'qtip-tip-point-right';
				// Show qtip popup
				$(this).qtipPopupShow(popupDispId,tcls,qwid,qheg,entId,onClsFn,qtipObj);
				
				// Get qtip positions
				var theg = $('.' + tcls).height();
				var twid = $('.' + tcls).width();

				//Calculate tip Position
				var tipTop = parseInt(theg/2) - 20;  //since tip height is 40
				var tipRight = parseInt(twid/2) - twid;
				
				// Position the tip
				$('.' + tcls).css('top',tipTop+'px').css('left',tipRight+'px');
				
				//Calculate popup possisions
				var tlcpos = $('.'+tcls).position();
				var left = tlcpos.left-qwid+20;
				var ppos = (lpos.top == 0) ? lpos.top : theg+(theg/2)-qheg;
				
				if(psubtype == 'middleLeft'){
					ppos = parseInt(qheg/2)-(theg/2)+10;
					left = tlcpos.left-qwid+20;
				}else if(psubtype == 'bottomleft' || psubtype == 'topleft'){
					
				}else if(psubtype == 'bottomright' || psubtype == 'topright'){
					
				}
				$('#'+popupDispId+' #visible-popup-'+entId+' #bubble-face-table').css('position','absolute').css('top','-'+ppos+'px').css('left',left+'px');
			},10)
		}catch(e){
			alert(e);
		}
	};
	
})(jQuery);


;
(function($) {
	var dWidth='';

$.widget("ui.enrollment", {
	_init: function() {
		try{
			var DrupalLocale = {'failed' : Drupal.t('failed'),
					'passed' : Drupal.t('passed'),
					'unknown' : Drupal.t('unknown'),
					'incomplete': Drupal.t('Incomplete'),
					'completed': Drupal.t('Completed')
					};
		var self = this;
		this.enrUserId = this.getLearnerId();
		dWidth = $("#enroll-result-container").width();
		if(dWidth==1000) {
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').addClass('enroll-disable-right-region');
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').removeClass('enroll-enable-right-region');
		}
		else {
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').addClass('enroll-enable-right-region');
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').remove('enroll-enable-right-region');
		}
		this.renderEnrollResults();
		this.prevObj;
		this.prevMoreObj;
		if(document.getElementById('learner-enrollment-tab-inner'))
		this.widgetObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
		else
			this.widgetObj = '$("#lnr-catalog-search").data("enrollment")';
		this.qtipListArr = new Array();
		}catch(e){
			// to do
		}
	},

  hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintEnrollmentResults tr.ui-widget-content').filter(":last");
    if (hideAll) {
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').show();
      $('#pager').hide();
      $('#gview_paintEnrollmentResults').css('padding', '0');

      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').hide();
     // $('#gview_paintEnrollmentResults').css('padding-bottom', '12px');
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #prev_pager').hide();
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #last_pager').hide();
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
	}catch(e){
		// to do
	}
  },

  showPageControls : function() {
	try{
	var lastDataRow = $('#paintEnrollmentResults tr.ui-widget-content').filter(":last");
	lastDataRow.children('td').css('border', '0 none');
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').hide();
    $('#gview_paintEnrollmentResults').css('padding-bottom', '0px');
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #prev_pager').show();
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #last_pager').show();
    $('#pager #pager_center #sp_1_pager').parent().show();
	}catch(e){
		// to do
	}
  },

	renderEnrollResults : function(){
		try{
	   	this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var dWidth = 663;
		}else{
			var dWidth = 667;
	    }

	  	if(document.getElementById('learner-enrollment-tab-inner')){
	  	if (Drupal.settings.last_left_panel==true) {
	  		rowNumValue = 20;
	  		rowListValue = [20,30,40];
	  	} else {
	  		rowNumValue = 10;
	  		rowListValue = [10,15,20];
	  	}
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
		var searchEnrollStr = '';
		var getRegStatus = 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
		var obj = $("#learner-enrollment-tab-inner").data("enrollment");
		var objStr = '$("#learner-enrollment-tab-inner").data("enrollment")';

	  	}else{
	  		$("#lnr-catalog-search").data("enrollment").createLoader('enroll-result-container');
			var searchEnrollStr = '';
			var getRegStatus = 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
			var obj = $("#lnr-catalog-search").data("enrollment");
			var objStr = '$("#lnr-catalog-search").data("enrollment")';
	  	}

		var userId = this.enrUserId;
		var enrStr = Drupal.settings.myenrolmentSearchStr;
		if (typeof enrStr === 'undefined' || enrStr == null || enrStr == undefined){
			searchEnrollStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus+'&sortBy=AZ';
		}
		else {
			searchEnrollStr	= '&UserID='+userId+enrStr
		}
		var url = this.constructUrl("learning/enrollment-search/all/"+searchEnrollStr);
		if(Drupal.settings.mylearning_right===false){
			$("#paintEnrollmentResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sortable':false,formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintEnrollmentTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintActions}

				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager',
				rowTotal: 0,
				viewrecords: true,
				emptyrecords: "No Search Results Found",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				autowidth: true,
				shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackLoader

			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
		$("#paintEnrollmentResults").jqGrid({
			url: url,
			datatype: "json",
			mtype: 'GET',
			colNames:['','',''],
			colModel : [
			            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sortable':false,formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
			            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintEnrollmentTitle},
			     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintActions}

			     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
			pager: '#pager',
			rowTotal: 0,
			viewrecords: true,
			emptyrecords: "No Search Results Found",
			sortorder: "asc",
			toppager:true,
			height: 'auto',
			width: dWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadComplete:obj.callbackLoader

		}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
		obj.hidePageControls(true); // show in loadComplete callbackLoader()
		$('.ui-jqgrid').addClass('myenrollment-table');
		$('.jqgfirstrow td:first-child').addClass('enroll-datatable-column-img');
		$('.jqgfirstrow td:nth-child(2)').addClass('enroll-datatable-column1');
		$('.jqgfirstrow td:last-child').addClass('enroll-datatable-column2');
		$('#paintEnrollmentResults .jqgfirstrow').hide();
		$('.ui-jqgrid-bdiv > div').css('position','static');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		$('#pager').find('#pg_pager .ui-pg-table').css("table-layout","auto");
		$('#pager').find('#pg_pager .ui-pg-table #pager_center').removeAttr("style");
		this.setSortTypeData('lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att');
		/* to highlight the default sort order - added by Rajkumar U*/
		$('#enroll-result-container .sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-result-container .add-high-lighter').addClass('sortype-high-lighter');
		$('.iframe-mylearning #innerWidgetTagEnroll #pager').css("width","948px");
		$('.iframe-mylearning #innerWidgetTagEnroll #pager-lp').css("width","948px");
		if(this.currTheme != "expertusoneV2"){
		  $('.iframe-mylearning #innerWidgetTagEnroll .enroll-action').css("right","321px");
		}

		}catch(e){
			// to do
		}
	},

	enrollSortForm : function(sort,className) {
		try{
		// High light sortype
		$('#enroll-result-container .sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-result-container .'+className).addClass('sortype-high-lighter');

		var getEnrData 		= eval($("#sort-by-enroll").attr("data"));
		var currEnrMode 	= getEnrData.currEnrMode;
		var userId 			= this.enrUserId;
    	var getRegStatus 	= '';
    	var searchStr 		= '';
    	searchStr			= '&UserID='+userId+'&regstatuschk='+currEnrMode+'&sortBy='+sort;
    	var url 			= this.constructUrl('learning/enrollment-search/all/'+searchStr)
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
    	$('#paintEnrollmentResults').setGridParam({url: url});
        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
		}catch(e){
			// to do
		}
	},

	updateMultiContentLaunchDialog : function(enrollmentId, lessonId, lessonInfo, contentQuizStatusConsolidated) { // function added for mantis ticket #0020086
    // Update lessonlocation data in the launch button of the lesson
	try{
	var widgetObj = this;
    lessonInfo.LessonLocation = lessonInfo.LessonLocation == null || lessonInfo.LessonLocation == 'null'? '' : lessonInfo.LessonLocation;
    lessonInfo.LaunchData = lessonInfo.LaunchData == null || lessonInfo.LaunchData == 'null'? '' : lessonInfo.LaunchData;
    lessonInfo.SuspendData = lessonInfo.SuspendData == null || lessonInfo.SuspendData == 'null'? '' : lessonInfo.SuspendData;
    lessonInfo.CmiExit = lessonInfo.CmiExit == null || lessonInfo.CmiExit == 'null'? '' : lessonInfo.CmiExit;
    var relaunchInfo = [{
         lessonlocation: lessonInfo.LessonLocation
         , launchData: lessonInfo.LaunchData
         , suspendData: lessonInfo.SuspendData
         , exit: lessonInfo.CmiExit
    }];
    // Update lesson status
    var status = Drupal.t('MSG511');
    if (lessonInfo.Status != '') {
      status = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.Status)+'" class="vtip">'+titleRestrictionFadeoutImage(Drupal.t('LBL102') +': ' + Drupal.t(lessonInfo.Status),'exp-sp-lnrenrollment-status',200)+'</span>';
    }
    var lesDiv = lessonId + "_" + enrollmentId + "_launch_button";
    if(document.getElementById(lesDiv) != null){
      $("#" + lessonId + "_" + enrollmentId + "_launch_button").data('relaunch', relaunchInfo);
      $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_status").html(status);
    }
    // Update lesson score
    var lesnscore = '';
    var pipe1 = '<span class="enroll-pipeline">|</span>';
    if(lessonInfo.LesScore !=null && lessonInfo.LesScore!=undefined && lessonInfo.LesScore != ''&& lessonInfo.LesScore != '0.00') {
    	lesnscore = '<div class="line-item-container float-left">'+pipe1 + '<span title="'+Drupal.t('LBL668')+': '+lessonInfo.LesScore+'" class="vtip">'+ titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + lessonInfo.LesScore,'exp-sp-lnrenrollment-score',200)+'</span></div>';
    }
    var contentQuizStatus = '';
    if(lessonInfo.contentQuizStatus != null && lessonInfo.contentQuizStatus != undefined && lessonInfo.contentQuizStatus != '') {
    	contentQuizStatus = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus) + '">'+  titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus),'exp-sp-lnrenrollment-sucstatus')+ '</span>';
    }
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonScore").html(lesnscore);
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonQuizStatus").html(contentQuizStatus);
    // Update content status and score
    var contentStatus = Drupal.t("MSG511");
    var Dstatus = Drupal.t("In progress");
	var Dstatus1 = Drupal.t("Incomplete");
	var Dstatus2 = Drupal.t("Completed");

  
    if (lessonInfo.ContentStatus != '') {
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.ContentStatus)+'" class="vtip">'+titleRestrictionFadeoutImage( Drupal.t('LBL102') +' : ' + Drupal.t(lessonInfo.ContentStatus),'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }else{
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.ContentStatus)+'" class="vtip">'+ titleRestrictionFadeoutImage( Drupal.t('LBL102') +' : ' + contentStatus,'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }
    if(lessonInfo.LaunchType == 'VOD' && lessonInfo.Status != ''){
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.Status)+'" class="vtip">'+ titleRestrictionFadeoutImage(Drupal.t('LBL102') +' : ' + Drupal.t(lessonInfo.Status),'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }
    var score = '';
    var pipe = '<span class="enroll-pipeline">|</span>';
    if(lessonInfo.ContScore !=null && lessonInfo.ContScore!=undefined && lessonInfo.ContScore != '' && lessonInfo.ContScore != '0.00') {
      score = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + lessonInfo.ContScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + lessonInfo.ContScore,'exp-sp-lnrenrollment-score',200)+'</span></div>';
    }
    var attemptsLeft = (lessonInfo.AttemptLeft == 'notset')? '' : lessonInfo.AttemptLeft;
    if (attemptsLeft !== '') {
      attemptsLeft = '<div class="line-item-container float-left">'+pipe +'<span class="vtip" title="'+Drupal.t('LBL202')+' : '+' ' + attemptsLeft+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL202')+' : ' +' ' + attemptsLeft,'exp-sp-lnrenrollment-attemptsleft',200)+'</span></div>';
    }
    var versionNo = lessonInfo.VersionNum;
    if (versionNo !== '') {
      versionNo = '<div class="line-item-container float-left">'+pipe +'<span class="vtip" title="'+Drupal.t('LBL1123')+' : '+' ' + versionNo+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL1123')+' : ' +' ' + versionNo,'exp-sp-lnrenrollment-version',200)+'</span></div>';
    }
    var validityDay = (lessonInfo.ValidityDays == '')? '' : lessonInfo.ValidityDays;
    var diffDays =  lessonInfo.remDays;
    if (validityDay !== '') {
//    	var remValidityDays = validityDay - diffDays;
    	var remValidityDays = (lessonInfo.daysLeft !== undefined && lessonInfo.daysLeft != null) ? lessonInfo.daysLeft : '';
		  if(remValidityDays <= 0){
		  var daysLBL = Drupal.t("Expired");//Expired
		  remValidityDays = '';// To avoid result as Validity: 0 Expired
		  }else if(remValidityDays == 1){
		  var daysLBL = Drupal.t("LBL910");//day
		  }else if(remValidityDays > 1){
		  var daysLBL = Drupal.t("LBL605");//days
		  }
    	validityDay = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604')+' : ' + remValidityDays + ' ' + daysLBL+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-lnrenrollment-validitydays',200)+'</span></div>';
    }
    contentQuizStatus = lessonInfo.contentQuizStatus;
    var data = eval($('#launch'+enrollmentId).attr('data'));
    //console.log('checck:: ' +data);
    contentQuizStatus = (data !== undefined ? widgetObj.getConsolidatedQuizStatus(lessonInfo.ContentId, data) : (contentQuizStatusConsolidated !== undefined ? contentQuizStatusConsolidated : ''));
    if(document.getElementById('lnr-catalog-search')){
    	var success_status = lessonInfo.contentQuizStatus;
		if (success_status == "failed" || success_status == 'incomplete' || success_status == '') {
			contentQuizStatus = success_status;
		}
		else if((lessonInfo.contentQuizStatus=='completed' || lessonInfo.contentQuizStatus=='Completed') && (lessonInfo.ContentType == 'Knowledge Content' || lessonInfo.ContentType == 'Tin Can'))
    		contentQuizStatus = Drupal.t('passed');
    }
    if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
		  contentQuizStatus = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
	  }
	var percentCompleted = '';
	if(lessonInfo.LaunchType == 'VOD') {
		var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "" && lessonInfo.SuspendData != undefined) ? JSON.parse(unescape(lessonInfo.SuspendData)) : null;
		var progress = suspend_data != null ? suspend_data['progress'] : 0;
		progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
		percentCompleted = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ progress+'% '+Drupal.t('Completed') + '">'+ titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
	}
    $("#lesson_status_"+lessonInfo.ContentId).html(contentStatus + score + contentQuizStatus + attemptsLeft +validityDay + percentCompleted + versionNo);
    // When no more attempts left, disable the Launch button
    if (lessonInfo.AttemptLeft == 0) {
      // Single lesson content
      $("#lesson-launch-" + lessonInfo.ContentId + " .launch_button_lbl").removeClass('enroll-launch-full').addClass('enroll-launch-empty');
      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeClass('actionLink');
      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeAttr('onclick');

      // More than 1 lesson in content
      $("#" + lessonInfo.ContentId + "LessonSubGrid .launch_button_lbl").removeClass('enroll-launch-full').addClass('enroll-launch-empty');
      $("#" + lessonInfo.ContentId + "LessonSubGrid .enroll-launch").removeClass('actionLink');
      $("#" + lessonInfo.ContentId + "LessonSubGrid .enroll-launch").removeAttr('onclick');

      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() { // for VOD content type
        var launchButtonId = $(this).attr('id');
        if (Drupal.ajax[launchButtonId]) {
          if (Drupal.ajax[launchButtonId].element_settings.event) {
            $("#" + launchButtonId).unbind(Drupal.ajax[launchButtonId].element_settings.event);
          }

          if (Drupal.ajax[launchButtonId].element_settings.keypress) {
            $("#" + launchButtonId).unbind('keypress');
          }

          delete Drupal.ajax[launchButtonId];

          $("#" + launchButtonId).attr('href', '#');
          $("#" + launchButtonId).click(function() {return false;});
          $("#" + launchButtonId).removeClass('use-ajax');
          $("#" + launchButtonId).removeClass('ctools-modal-ctools-video-style');
          $("#" + launchButtonId).removeClass('ajax-processed');
        }
      });
    }
	if(lessonInfo.LaunchType == 'VOD') {
		//update launch button progress
//		console.log($("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch"));
		$("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() {
			var launchButtonId = $(this).attr('id');
			var href = $("#" + launchButtonId).attr('href');
			href_suspend = href.substr(0, href.lastIndexOf("/"));
			href = href_suspend.substr(0, href_suspend.lastIndexOf("/"));
			var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "" && lessonInfo.SuspendData != undefined) ? JSON.parse(unescape(lessonInfo.SuspendData)) : null;
			// var progress = suspend_data != null ? suspend_data['progress'] : 0;
			//console.log(progress);
			if(lessonInfo.AttemptLeft != 0) {	//update href attribute if attempts left to launch
			if(Drupal.ajax[launchButtonId].element_settings.url) {
				var url = Drupal.ajax[launchButtonId].element_settings.url.substr(0, Drupal.ajax[launchButtonId].element_settings.url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].element_settings.url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			if(Drupal.ajax[launchButtonId].url){
				var url = Drupal.ajax[launchButtonId].url.substr(0, Drupal.ajax[launchButtonId].url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			if(Drupal.ajax[launchButtonId].options.url) {
				var url = Drupal.ajax[launchButtonId].options.url.substr(0, Drupal.ajax[launchButtonId].options.url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].options.url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			$("#" + launchButtonId).attr('href', href + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData);
			}
		});
	}
		
	}catch(e){
		// to do
		// console.log(e);
	}
	},

	callbackLoader : function(data, postdata, formid, updateShowMore){
	 try{
	     var default_cookie = '&regstatuschk=lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';
		 if(document.getElementById('learner-enrollment-tab-inner'))
			var obj = $("#learner-enrollment-tab-inner").data("enrollment");
		 else
			 var obj = $("#lnr-catalog-search").data("enrollment");
		obj.destroyLoader('enroll-result-container');
		if(Drupal.settings.mylearning_right===false)
			$('.clsenroll-result-container #pager').width($('.clsenroll-result-container').width()+4);
		// Delete enrollmentId and lessonId from jqGrid postData object when present. Added for mantis ticket #0020086
		var postData = $('#paintEnrollmentResults').getGridParam("postData");
		delete postData.enrollmentId;
		delete postData.lessonId;

		if (document.getElementById('launch-content-container') &&
		      data.triggering_enrollment_id && data.triggering_lesson && data.triggering_lesson_details) { // added for mantis ticket #0020086
		  obj.updateMultiContentLaunchDialog(data.triggering_enrollment_id, data.triggering_lesson, data.triggering_lesson_details, data.triggering_content_quiz_status);
		}

//		var recs = parseInt($("#paintEnrollmentResults").getGridParam("records"),10);
//		$("#paintEnrollmentResults")[0].p.rowTotal = recs;
		$("#paintEnrollmentResults")[0].p.rowTotal = data['records'];
		var recs = data['records'];
		updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
		if (updateShowMore) {
			$("#paintEnrollmentResults").data('totalrecords', recs);
			var showMore = $('#paintEnrollmentResults-show_more');
			if ($('#paintEnrollmentResults').getGridParam("reccount") < recs) {
				showMore.show();
			} else {
				showMore.hide();
			}
		}
		//Added by ganeshbabuv to avoid the issue for not showing the enrollment message if there are no enrollments and comes from salesforce app - Ref:SF Cookieless Option (0054508),Sep 30th 2015
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {		    
			setTimeout(function(){
				if (recs <= 0){ 
					if(Drupal.settings.myenrolmentSearchStr === default_cookie){
						$('#enroll-noresult-msg').show();
					}
					else{
						$('#enroll-nosearchresult-msg').show();
					}
					$('#enroll-result-container .sort-by-text').hide();
					$('#enroll-result-container .sort-by-links').hide();
				} else {
					$('#enroll-result-container .sort-by-text').show();
					$('#enroll-result-container .sort-by-links').show();
				}
			},100);
			
		}else{  
			if (recs <= 0) {
				if(Drupal.settings.myenrolmentSearchStr === default_cookie){
					$('#enroll-noresult-msg').show();
				}
				else{
					$('#enroll-nosearchresult-msg').show();
				}
				$('#enroll-result-container .sort-by-text').hide();
				$('#enroll-result-container .sort-by-links').hide();
			} else {
				$('#enroll-result-container .sort-by-text').show();
				$('#enroll-result-container .sort-by-links').show();
			}
		}	
		 

    // Show pagination only when search results span multiple pages
    var reccount = parseInt($("#paintEnrollmentResults").getGridParam("reccount"), 10);
    var hideAllPageControls = true;
    if (recs > 10) { // 10 is the least view per page option.
       hideAllPageControls = false;
    }

    if (recs <= reccount) { // Covers the case when there are no recs, i.e. recs == 0
      obj.hidePageControls(hideAllPageControls);
    }
    else {
      obj.showPageControls();
    }
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();
        $('.fivestar-click').click(function() {
        	var fivestarClick = this;
        	var fiveStarClass = $(this).attr('class');
        	var pattern = /[0-9]+/;
        	var rating = fiveStarClass.match(pattern);
        	var nodeId = $(this).parents("form").siblings("input:hidden").val();
        	var entity_type = "Class";
        	var param = {'rating':rating,'nodeId':nodeId,'entityType':entity_type};
    		url = obj.constructUrl("learning/five-star-submit");
    		$.ajax({
    			type: "POST",
    			url: url,
    			data:  param,
    			success: function(result){
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
	    			}else{
	    				$("body").data("learningcore").fiveStarCheck(nodeId,'enroll-node-');
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? result.votemsg : result.votesmsg;
		    			var avgRating = '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);
	    			}
    			}
    	    });
		});
		$("#learner-enrollment-tab-inner").data("enrollment").shapeTheButton('paintEnrollmentResults');
		$('#enroll-result-container .ui-jqgrid-hdiv').css('display','none');
		$('#paintEnrollmentResults .jqgfirstrow').css('display','none');
		//Vtip-Display toolt tip in mouse over
		
		
		
		if (Drupal.settings.salesforce.type == "canvas" || Drupal.settings.salesforce.type == "iframe") {			
			if (Drupal.settings.mylearning_right===false) {
				var currentWidth = 942;
				$("#paintEnrollmentResults").jqGrid('setGridWidth', currentWidth);
				// var currentPage = $('.ui-pg-input').val();
				// $("#paintEnrollmentResults").setGridParam({page: currentPage}).trigger('reloadGrid');
			}
		} 
		//alert(navigator.userAgent.indexOf('Edge/'));
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if(typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1" && (navigator.userAgent.indexOf('Edge/')!= -1)) {		
			 RefreshFadeoutForSF('#paintEnrollmentResults','item-title-code','line-item-container','mylearning');
			 Refreshtrunk8('myenrollments','limit-title-enr');
		}else{ 
			resetFadeOutByClass('#paintEnrollmentResults','item-title-code','line-item-container','mylearning');
			$('.limit-title-enr').trunk8(trunk8.myenroll_title);
			$('.limit-desc-enr').trunk8(trunk8.myenroll_desc);
		}
		//$('.cp-menulist .limit-title').trunk8(trunk8.contstrip_title);
		vtip();
		$("#paintEnrollmentResults").showmore({
			'grid': "#paintEnrollmentResults",
			'gridWrapper': '#enroll-result-container',
			'showMore': '#paintEnrollmentResults-show_more'
		});
	 }catch(e){
			// to do		
		}
	},
	



	shapeTheButton : function(parentId){
		try{
		$('#'+parentId).find(".enroll-main-list").each(function() {
			var avlBtn = 0;
			avlBtn += ($(this).children(".enroll-launch-gray").length > 0)? 1: 0;
			avlBtn += ($(this).children(".enroll-launch-more-gray").length > 0)? 2: 0;

			switch(avlBtn) {
				case 2:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch").wrap("<label class='enroll-launch-full'></label>");
				case 3:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch-gray").wrap("<label class='enroll-launch-empty'></label>");
			}
			resetFadeOutByClass('#launch-wizard #paintEnrollmentResults','item-title-code','line-item-container','mylearning');
		});
		}catch(e){
			// to do
		}
	},

	paintLPSearchIcons : function (cellvalue, options, rowObject) {
		try{
		var type = 'cre_sys_obt_cls';
		var deliverytypecode = rowObject['delivery_type_code'];
	    var html		= '';
		var iconType='';
		if(type == "cre_sys_obt_cls"){

			if(deliverytypecode == 'lrn_cls_dty_ilt' ) {
				iconType = "iltIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_wbt' ) {
				iconType = "wbtIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_vcl' ) {
				iconType = "vclIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_vod' ) {
				iconType = "vodIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}
		}else if(type == "cre_sys_obt_crt"){
			iconType = "Cert_Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';
		}else if(type == "cre_sys_obt_cur"){
			iconType = "Curr_Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';

		}else if(type == "cre_sys_obt_trp"){
			iconType = "TP Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';
		}

		return html;
		}catch(e){
			// to do
		}
	},

	setEnrStateInMsg : function(regActive) {
		try{
		switch (regActive) {
		case 'Enrollmentpart' :
			$('#enrollment-state').html((Drupal.t('Enrolled')).toLowerCase());
			break;
		case 'EnrollCompleted':
			$('#enrollment-state').html(Drupal.t('Completed'));
			break;
		case 'EnrollInCompleted':
			$('#enrollment-state').html((Drupal.t('Incomplete')).toLowerCase());
			break;
		case 'EnrollExpired':
			$('#enrollment-state').html((Drupal.t('Expired')).toLowerCase());
			break;
		case 'EnrollCanceled':
			$('#enrollment-state').html(Drupal.t('Canceled'));
			break;
		case 'EnrollPayments' :
			$('#enrollment-state').html((Drupal.t('Pending')).toLowerCase());
			break;
		}
		}catch(e){
			// to do
		}
	},

	callEnrollment : function(regStatus, regActive){
		try{
		$('#enroll-noresult-msg').hide();
		$('#enroll-nosearchresult-msg').hide();
		this.setEnrStateInMsg(regActive);
		var userId = this.enrUserId;
    	var getRegStatus = (regStatus) ? regStatus : 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
    	var searchStr = '';
    	searchStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus;
    	var url = this.constructUrl('learning/enrollment-search/all/'+searchStr);
    	this.setSortTypeData(getRegStatus);
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
    	$('#paintEnrollmentResults').setGridParam({url: url});
        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
       	this.highlightCourseTab(regActive);
       	$('#enroll-result-container .sort-by-links').find('li a').removeClass('sortype-high-lighter');
       	$('#enroll-result-container .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
       	if(this.currTheme == "expertusoneV2"){
           $('#learner-enrollment .sort-by-links').find('li a').removeClass('sortype-high-lighter');
           $('#learner-enrollment .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
        }
		}catch(e){
			// to do
		}
    },

    setSortTypeData : function(getRegStatus) {
    	try{
    	var setdata = "data={'currEnrMode':'"+getRegStatus+"'}";
    	$("#sort-by-enroll").attr("data",setdata);
    	}catch(e){
    		// to do
    	}
    },

	highlightCourseTab : function(highlightId){
		try{
		var curTheme = themepath.split("/");
		var resTheme = curTheme[curTheme.length-1];
		if (resTheme == 'expertusoneV2')
		{
			$('#learner-maincontent_tab3 ul li a#'+highlightId).parent('li').siblings('li').removeClass('selected');
			$('#learner-maincontent_tab3 ul li a#'+highlightId).parent('li').addClass('selected');
		} else
		{
			$("#learner-maincontent_tab3 ul li a").each(function(){
			    $(this).removeClass('orange');
			});
			$('#learner-maincontent_tab3 ul li a#'+highlightId).addClass('orange');
		}
		}catch(e){
			// to do
		}
	},

	paintEnrollmentTitle : function(cellvalue, options, rowObject) {
		try{
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		var htmlVal='';
		var wobj = eval(options.colModel.widgetObj);
		var obj1 = options.colModel.widgetObj;
		var baseType				= rowObject['basetype'];
		var dateValidTo				= rowObject['valid_to'];
		var dataDelType				= rowObject['delivery_type'];
		var dataDelTypeCode			= rowObject['delivery_type_code'];
		var dataRegStatus			= rowObject['reg_status'];
		var dataRegStatusCode		= rowObject['reg_status_code'];
		var RegStatusCode			= rowObject['reg_status_code'];
		var courseComplitionStatus	= rowObject['reg_status'];
		var RegStatusDate          =  rowObject['reg_status_date'];
		var courseScore				= rowObject['score'];
		var quizStatus				= rowObject['quiz_status'];
		var dataId 					= rowObject['id'];
		var courseId 				= rowObject['course_id'];
		var classId 				= rowObject['class_id'];
		var classTitle				= rowObject['cls_title'];
		var classCode				= rowObject['cls_code'];
		var courseTitle				= rowObject['course_title'];
		var courseTempId			= rowObject['courseid'];
		var session_id				= rowObject['session_id'];
		var SessStartDate 			= rowObject['session_start'];
		var sessionStartDay			= rowObject['session_start_day'];
		var SessEndDate 			= rowObject['session_end'];
		var shortDescription 		= encodeURIComponent(rowObject['description']);
		var descriptionFull			= encodeURIComponent(rowObject['descriptionfull']);
		var language 				= rowObject['language'];
		var usertimezonecode = rowObject['usertimezonecode'];
		var session_code = rowObject['session_code'];
		var user_tzcode =  rowObject['user_tzcode'];
		var LocationName 			= rowObject['location_name'];
		var LocationId				= rowObject['location_id'];
		var LocationAddr1 			= rowObject['location_addr1'];
		var LocationAddr2 			= rowObject['location_addr2'];
		var LocationCity 			= rowObject['location_city'];
		var LocationState 			= rowObject['location_state'];
		var LocationCountry 		= rowObject['location_country'];
		var LocationZip 			= rowObject['location_zip'];
		var LocationPhone 			= rowObject['location_phone'];
		var regDate					= rowObject['reg_date'];
		var remDays			        = rowObject['remDays'];
		var compDate				= rowObject['comp_date'];
		var updateDate				= rowObject['updated_on'];
		var startDateFormat			= rowObject['session_start_format'];
		var lnrAttach				= rowObject['show_lnr_attach'];
		var show_events				= rowObject['show_events'];
		var LessonLocation			= rowObject['LessonLocation'];
		var launch_data				= rowObject['LaunchData'];
		var suspend_data			= rowObject['SuspendData'];
		var exit					= rowObject['CmiExit'];
		var labelmsg 				= rowObject['labelmsg'];
		var updated_by_ins_mngr_slf	= rowObject['updated_by_ins_mngr_slf'];
		var created_by_ins_mngr_slf	= rowObject['created_by_ins_mngr_slf'];
		var compliance_compl_days 	= rowObject['compliance_complete_days'];
		var compliance_compl_date	= rowObject['compliance_complete_date'];
		var compliance_val_days 	= rowObject['compliance_validity_days'];
		var compliance_val_date		= rowObject['compliance_validity_date'];
		var compliance_expire		= rowObject['compliance_expire'];
		var usrId					= rowObject['user_id'];
		var compliance_expired_on_validity = rowObject['compliance_expired_on_validity'];

		var daysRemaining = '';
		var dayRemainVal  = '';
		var daysLeft = '';
		var AttemptLeft	  = '';
		var ValidityDays	  = '';
		var contValidateMsg = '';
		var classScore = '';
		var mro						= rowObject['mro'];
		var mro_id						= rowObject['mro_id'];
		var assigned_by				= unescape(rowObject['assigned_by']).replace(/"/g, '&quot;');

		var FullName			= rowObject['full_name'].replace(/'/g, "\\\'");
		var MandatoryOption			= rowObject['mandatory'];
		var IsCompliance			= rowObject['is_compliance'];
		var Managerid             =  rowObject['managerid'];
		var UpdatedBy             =  rowObject['updated_by'];
		var CreatedBy             =  rowObject['created_by'];
		var UpdatedByName         =  rowObject['updated_by_name'].replace(/'/g, "\\\'");
		var waitlist_priority     =  rowObject['waitlist_priority'];
		var isLastRec             =  (typeof rowObject['is_last_rec'] != 'undefined' && rowObject['is_last_rec'] == 'last')? 'last' : '';
		var diffDays = remDays;
		var complianceCompDate	= (compliance_compl_days) ? compliance_compl_days : compliance_compl_date;
		var is_exempted		= rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'].replace(/'/g, "\\\'");;
		var exempted_on = rowObject['exempted_on'];
		var VersionNo = '';
		var mroImageClass = '';
		var mroImageClassArr = new Array();
		mroImageClassArr['cre_sys_inv_com'] =  '<span class="course-compliance-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Compliance')+'">'+Drupal.t('LBL743')+'</span>';
		mroImageClassArr['cre_sys_inv_man'] =  '<span class="course-mandatory-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Mandatory')+'">'+Drupal.t('M')+'</span>';
        mroImageClassArr['cre_sys_inv_opt'] =  '<span id="'+classId+mro_id+'" title="'+Drupal.t('Optional')+'">'+'</span>';
        mroImageClassArr['cre_sys_inv_rec'] =  '<span class="course-recommended-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Recommended')+'">'+Drupal.t('LBL746')+'</span>';
        
		if(IsCompliance == 1){
        	mroImageClass = mroImageClassArr['cre_sys_inv_com'];
        }
        else if(MandatoryOption == 'Y'){
        	mroImageClass = mroImageClassArr['cre_sys_inv_man'];
        } else {
        	mroImageClass = (mro == '' || mro == null) ? '' : mroImageClassArr[mro_id];
        }

		if(session_id == null) {
			session_id ='';
		}
		if (courseScore && courseScore != 0 && courseScore != '0' && dataRegStatusCode=="lrn_crs_cmp_cmp") {
		    classScore = Drupal.t("LBL668") +' : ' + courseScore;
		}
		if (quizStatus && quizStatus != "" && dataRegStatusCode == "lrn_crs_cmp_cmp") {
			quizStatus = Drupal.t("LBL1284") +' : ' + Drupal.t(quizStatus);
		}
		var extendedClass = '';
		switch (baseType) {
			case "WBT":
			case "VOD":
				extendedClass = 'webvod_cls_code';
				break;
			case "ILT":
			case "VC":
				extendedClass = 'iltvc_cls_code';
				break;
			default: 
				extendedClass = ''
				break;	
		}
		if((baseType=="WBT" || baseType=="VOD") && ((dataRegStatusCode=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") || (dataRegStatusCode=="lrn_crs_cmp_inp"))) {
			  var i=0;
				var allCntId = [];
				for(j=0; j < rowObject.launch.length; j++){
				  allCntId[j] = rowObject.launch[j].ContentId;
				}
				uniqueCntId = $.unique(allCntId);
				if(uniqueCntId.length == 1 && rowObject.launch[0].ValidityDays !=''){
					var remValidityDays = (rowObject.launch[0].daysLeft !== undefined && rowObject.launch[0].daysLeft != null) ? rowObject.launch[0].daysLeft : '';
					if(remValidityDays <= 0){
						var daysLBL = Drupal.t("Expired");//Expired
						remValidityDays = '';// To avoid result as Validity: 0 Expired
					}else if(remValidityDays == 1){
						var daysLBL = Drupal.t("LBL910");//day
					}else if(remValidityDays > 1){
						var daysLBL = Drupal.t("LBL605");//days
					}
					ValidityDays = Drupal.t("LBL604")+' : '+remValidityDays+' '+daysLBL;
				}
				if(uniqueCntId.length == 1 && rowObject.launch[0].AttemptLeft !='' && rowObject.launch[0].AttemptLeft !='notset'){
					contValidateMsg			= rowObject.launch[0].contValidateMsg;
					AttemptLeft = Drupal.t("LBL202")+' : '+rowObject.launch[0].AttemptLeft;
				}
				if(uniqueCntId.length == 1)
					VersionNo = Drupal.t("LBL1123")+' : '+rowObject.launch[0].VersionNum;
					
			  	if(dateValidTo!='' && dateValidTo!=null){
				  var daystocount = new Date(dateValidTo);
				  var srvDate = rowObject.launch[0].server_date_time;
				  today=new Date(srvDate);
				  var oneday=1000*60*60*24;
				  daysRemaining = (Math.ceil((daystocount.getTime()-today.getTime())/(oneday)));
				  if(daysRemaining<0){
					  dayRemainVal  = Drupal.t("Expired");
				  }else {
					  dayRemainVal = Drupal.t("LBL677")+': '+(daysRemaining);
				  }
			  	}
			  	daysLeft = (rowObject.launch.length > 0 && rowObject.launch[0].daysLeft != undefined && rowObject.launch[0].daysLeft != '' ) ? rowObject.launch[0].daysLeft : '';
			  //	daysLeft = rowObject.launch[0].daysLeft;
		}
		var obj = options.colModel.widgetObj;
		var LocationNameArg = ((LocationName != null) ? ((unescape(LocationName).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationName);
		var LocationAdd1Arg = ((LocationAddr1!= null) ? ((unescape(LocationAddr1).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr1);
		var LocationAdd2Arg = ((LocationAddr2 != null) ? ((unescape(LocationAddr2).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr2);
		var LocationcityArg = ((LocationCity != null) ? ((unescape(LocationCity).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationCity);
		var data1="data={'RegId':'"+dataId+"','classCode':'"+escape(classCode)+"','BaseType':'"+baseType+"','shortDescription':'"+escape(shortDescription)+"','isTitle':'1','title':'"+escape(classTitle)+"','rowId':'"+dataId+"','CourseId':'"+courseId+"'," +
				"'show_events':'"+show_events+"','coursetempid':'"+courseTempId+"','regstatus':'"+dataRegStatusCode+"','regstatusDate':'"+RegStatusDate+"','regStatusCode':'"+RegStatusCode+"','classId':'"+classId+"','deliverytype':'"+dataDelTypeCode+"','daysRemaining':'"+daysRemaining+"'," +
				"'dataDelType':'"+dataDelType+"','SessStartDay':'"+sessionStartDay+"','startDateFormat':'"+startDateFormat+"','SessStartDate':'"+SessStartDate+"','SessEndDate':'"+SessEndDate+"','courseComplitionStatus':'"+courseComplitionStatus+"','courseScore':'"+courseScore+"','detailTab':'false'," +
				"'regDate':'"+regDate+"','compDate':'"+compDate+"','updateDate':'"+updateDate+"'," +
				"'isCompliance':'"+IsCompliance+"',"+"'remDays':'"+remDays+"',"+
				"'complianceCompleteDays' :'"+compliance_compl_days+"',"+"'complianceCompleteDate' :'"+compliance_compl_date+"',"+
				"'complianceValidityDays' :'"+compliance_val_days+"',"+"'complianceValidityDate' :'"+compliance_val_date+"',"+
				"'complianceExpire' : '"+compliance_expire+"',"+
					"'session_id' : '"+session_id+"',"+
					"'usertimezonecode' : '"+usertimezonecode+"',"+
					 "'user_tzcode' : '"+user_tzcode+"',"+
				"'complianceExpiredOnValidity' : '"+compliance_expired_on_validity+"',"+
				"'contValidateMsg':'"+AttemptLeft+"','dayRemainVal':'"+dayRemainVal+"','daysLeft':'"+daysLeft+"',"+"'LessonLocation':'"+LessonLocation+"',"+
				"'launch_data':'"+launch_data+"','suspend_data':'"+suspend_data+"','exit':'"+exit+"',"+
				"'LocationName':'"+LocationNameArg+"','LocationId':'"+LocationId+"','LocationAddr1':'"+LocationAdd1Arg+"','LocationAddr2':'"+LocationAdd2Arg+"','LocationCity':'"+LocationcityArg+"','LocationState':'"+LocationState+"','LocationCountry':'"+LocationCountry+"','LocationZip':'"+LocationZip+"','LocationPhone':'"+LocationPhone+"'," +
				"'FullName':'"+FullName+"','MandatoryOption':'"+MandatoryOption+"','Managerid':'"+Managerid+"','UpdatedBy':'"+UpdatedBy+"','CreatedBy':'"+CreatedBy+"','UpdatedByName':'"+UpdatedByName+"','mro_type':'"+mro+"','assigned_by':'"+assigned_by+"','updated_by_ins_mngr_slf':'"+updated_by_ins_mngr_slf+"','created_by_ins_mngr_slf':'"+created_by_ins_mngr_slf+"',"+
				"'is_last_rec':'"+isLastRec+"',"+"'courseTitle':'"+escape(courseTitle)+"',"+"'is_exempted':'"+is_exempted+"',"+"'exempted_by':'"+exempted_by+"',"+"'exempted_on':'"+exempted_on+"',"+"'version_no':'"+VersionNo+"'}";

		var html	= '';
		var inc = 0;
		var passParams ='';

		if(lnrAttach.length>0) {
			 $(lnrAttach).each(function(){
				 inc=inc+1;
				 passParams += "{";
				 passParams += "'Id':'"+$(this).attr("attachment_id")+"'";
				 /*--Issue fix for the ticket - 32781 --*/
				 passParams += ",'Title':'"+escape($(this).attr("reading_title"))+"'";
				 passParams += ",'url':'"+encodeURI($(this).attr("reading_content")).replace(/'/g, '%27')+"'";
				 passParams += "}";
				 if(inc<lnrAttach.length) {
					 passParams += ",";
				 }
			 });
		}
		var attachdata = "data=["+passParams+"]";
		var classSwitch = (baseType=="WBT" || baseType=="VOD") ? 'item-title-wbt' : 'item-title';

		//Enrollment Title Restricted to 20 Character
		var enrollFulltitle = unescape(classTitle);
		var isMRO = '';
		if(mroImageClass != '') {
			isMRO = ' enroll-course-title-mro';
		}
		html += '<div class="limit-title-row enroll-course-title'+isMRO+'">';
		html += '<span id="class_attachment_'+dataId+'" data="'+attachdata+'"></span>';
		var style = '';
		var sessLength = rowObject.sessionDetails.length;
			if(sessLength>1) {
				var titleClass = 'exp-sp-lnrenrollment-enrollmulses';
			}else{
				var titleClass = 'exp-sp-lnrenrollment-enrollsinses';
			}
		if(this.currTheme == "expertusoneV2"){
		   style = "style=\"display:none;\"";
		  }
		html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="title_close"  '+style+'  onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>&nbsp;</a>';
		html += '<span id="titleAccEn_'+dataId+'" class="'+classSwitch+' vtip" ><span class="limit-title limit-title-enr enroll-class-title vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
		html += '<span class="'+titleClass+'"/>';
		//if(baseType=="WBT" || baseType=="VOD"){
		//	if(Drupal.settings.mylearning_right===false)
				html += enrollFulltitle;
		//	else
			//	html += enrollFulltitle;
		//}
	/*	else{
			var sessLength = rowObject.sessionDetails.length;
			if(sessLength>1) {
				var titleClass = 'exp-sp-lnrenrollment-enrollmulses';
			}else{
				var titleClass = 'exp-sp-lnrenrollment-enrollsinses';
			}
			if(Drupal.settings.mylearning_right===false)
				html += titleRestrictionFadeoutImage(enrollFulltitle,titleClass,47);
			else
				html += titleRestrictionFadeoutImage(enrollFulltitle,titleClass,15);
		}*/
		if(is_exempted != 1)
			html += '</span>'+mroImageClass+'</span>';
		
		html += '</div>';

		var startTime = rowObject['session_start'];
		var startDate = rowObject['session_start_format'];
		var baseType  = rowObject['basetype'];
		var classId	  = rowObject['class_id'];
	 //click view button open close TP Description
		var viewcount = 0;
		$('#class-accodion-'+dataId).live("click",function(){
			viewcount++;
			curObj = $(this).closest("tr").find(".item-long-desc");
			shortObj = $(this).closest("tr").find(".item-short-desc");
			elipsis = $(this).closest("tr").find(".item-elipsis");
			desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
			var isEven = function(viewcount) {
		    return (viewcount % 2 === 0) ? true : false;
		    };
		    if (isEven(viewcount) === false) {
				$(curObj).show();
				$(elipsis).hide();
				$(shortObj).hide();
				$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
				$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
			}else if (isEven(viewcount) === true) {
				$(curObj).hide();
				$(elipsis).show();
				$(shortObj).show();
				$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
				$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

			}
		});


        
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			var usertimezonecode = rowObject['usertimezonecode'];
			var session_code = rowObject.sessionDetails[0].session_code;
			var user_tzcode = rowObject['user_tzcode'];
			
			if(sessLen>1) {
			
				sessLenEnd = sessLen-1;
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				var sTime =  rowObject.sessionDetails[0].session_start_format;
				var sTimeForm =  rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
				var eTimeForm =rowObject.sessionDetails[sessLenEnd].session_end_time_form;
				var ecode = rowObject.sessionDetails[0].tz_code;
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';

				if(baseType =="ILT"){
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
				}
				var uTime = rowObject.sessionDetails[0].ilt_session_start_format ;
			    var uTimeForm = rowObject.sessionDetails[0].ilt_session_start_time_form;
			    var rTime = rowObject.sessionDetails[sessLenEnd].ilt_session_end_format;
			    var rTimeForm = rowObject.sessionDetails[sessLenEnd].ilt_session_end_time_form;
			    startDateForTitleloc = uTime+' '+'<span class="time-zone-text">'+uTimeForm+'</span>'+' to '+rTime+' <span class="time-zone-text">'+rTimeForm+'</span>'+' '+session_code+' '+ecode;
			} else {
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				var sTime =  rowObject.sessionDetails[0].session_start_format;
				var sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[0].session_start_end_format;
				var eTimeForm = rowObject.sessionDetails[0].session_end_time_form;
				var ecode = rowObject.sessionDetails[0].tz_code;
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';

				if(baseType =="ILT"){
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
				}
				var uTime = rowObject.sessionDetails[0].ilt_session_start_format;
			    var uTimeForm = rowObject.sessionDetails[0].ilt_session_start_time_form;
			    var rTime = rowObject.sessionDetails[0].ilt_session_start_end_format;
			    var rTimeForm = rowObject.sessionDetails[0].ilt_session_end_time_form;
			    startDateForTitleloc = uTime+' '+'<span class="time-zone-text">'+uTimeForm+'</span>'+' to '+rTime+' <span class="time-zone-text">'+rTimeForm+'</span>'+' '+session_code+' '+ecode;
			}
			var inc = 0;
			var passParams = "[";
        
			$(rowObject.sessionDetails).each(function(){
				inc=inc+1;

				var sessionfacName = (($(this).attr("session_name") != null) ? ((unescape($(this).attr('session_name')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_name"));
				var sessionfacAddress1 = (($(this).attr("session_address1") != null) ? ((unescape($(this).attr('session_address1')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_address1"));
				var sessionfacAddress2 = (($(this).attr("session_address2") != null) ? ((unescape($(this).attr('session_address2')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_address2"));
				var sessionfacCity = (($(this).attr("session_city") != null) ? ((unescape($(this).attr('session_city')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_city"));
				var sInsName = (($(this).attr("session_instructor_name") ===null) ? '':((unescape($(this).attr('session_instructor_name')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")));
				 passParams += "{";
				 passParams += "'sessionTitle':'"+(($(this).attr("session_title")) ? escape($(this).attr("session_title")) : '')+"',";

				 /*For ticket 0028936: ILT Timezone Display for My Learning page */
				 passParams += "'sessionDay':'"+$(this).attr("session_start_day")+"',";
				 passParams += "'sessionDate':'"+$(this).attr("session_start_format")+' to '+$(this).attr("session_start_end_format")+"',";
				 passParams += "'sessionSDate':'"+$(this).attr("session_start_format")+"',";
				 passParams +=  "'sessionEDate':'"+$(this).attr("session_start_end_format")+"',";
				 passParams += "'sessionSDayForm':'"+$(this).attr("session_start_time_form")+"',";
				 passParams += "'sessionEDayForm':'"+$(this).attr("session_end_time_form")+"',";
              // if(baseType =="ILT"){
                passParams += "'sessiontimezone':'"+$(this).attr("sess_timezone")+"'," ;
                 passParams += "'usertimezone':'"+$(this).attr("user_timezone")+"'," ;
               passParams += "'sessionId':'"+$(this).attr("session_id")+"'," ;
                passParams += "'sessionDayloc':'"+$(this).attr("ilt_session_start_day")+"'," ;
              passParams += "'sessionDateloc':'"+$(this).attr("ilt_session_start_format")+' to '+$(this).attr("ilt_session_start_end_format")+"'," ;
                 passParams += "'sessionSDateloc':'"+$(this).attr("ilt_session_start_format")+"'," ;
                passParams += "'sessionEDateloc':'"+$(this).attr("ilt_session_start_end_format")+"'," ;
                passParams += "'sessionSDayFormloc':'"+$(this).attr("ilt_session_start_time_form")+"'," ;
                passParams += "'sessionEDayFormloc':'"+$(this).attr("ilt_session_end_time_form")+"'," ;
                passParams += "'usertimezonecode':'"+usertimezonecode+"'," ;
               passParams += "'session_code':'"+$(this).attr("session_code")+"'," ;
               passParams += "'user_tzcode':'"+$(this).attr("user_tzcode")+"'," ;
                passParams += "'tz_code':'"+$(this).attr("tz_code")+"'," ;
               // }
                 
                 
                 
				 //passParams += "'sessionfacName':'"+$(this).attr("session_name")+"',";
				 passParams += "'sInsName':'"+sInsName+"',";
				 passParams += "'sessionfacName':'"+sessionfacName+"',";
				 passParams += "'sessionfacAddress1':'"+sessionfacAddress1+"',";
				 passParams += "'sessionfacAddress2':'"+sessionfacAddress2+"',";
				 passParams += "'sessionfacCountry':'"+escape($(this).attr("session_country"))+"',";
				 passParams += "'sessionfacState':'"+escape($(this).attr("session_state"))+"',";
				 passParams += "'sessionfacCity':'"+sessionfacCity+"',";
				 passParams += "'sessionfacZipcode':'"+$(this).attr("session_zipcode")+"'";
				 passParams += "}";

				 if(inc < sessLen) { 
					 passParams += ",";
				 }
			});
			passParams += "]";
			/*if(baseType =="ILT"){
			html += '<div id=sessdetails><span class="session-time-zone" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDateForTitle,'exp-sp-lnrenrollment-timezone', 30) +'</span>';
       }
			else{
			html += '<div id=sessdetails><span class="session-time-zone" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+startDateForTitle+'</span>';

			}*/// alert(rowObject.sessionDetails[0].sess_timezone);
          //alert(rowObject.sessionDetails[0].user_timezone);
            /*if(baseType =="ILT" && rowObject.sessionDetails[0].sess_timezone!=rowObject.sessionDetails[0].user_timezone){
            	html += qtip_popup_paint(classId,startDateForTitleloc,sessLen); 
            }*/
		//else{
			//html += '<span>&nbsp;</span>';
		//}
		}
		//else{
		//	html += '<span>&nbsp;</span>';
		//}
		html += '<div class="enroll-class-title-info">';
		html += '<div class="item-title-code '+ extendedClass +'">';

		var statusDate = '';
		var statusName = '';

		if(RegStatusCode == 'lrn_crs_cmp_enr') {
		/*	if((complianceCompDate != '' && complianceCompDate != null)){
				statusDate = complianceCompDate;
				statusName = Drupal.t('LBL234');

			}else{*/

				statusDate = regDate;
				statusName = Drupal.t('LBL704');
			//}
		}else if(RegStatusCode == 'lrn_crs_cmp_cmp') {
			statusDate = compDate;
			statusName = Drupal.t('LBL027');
		}else if(RegStatusCode == 'lrn_crs_cmp_inc') {
			statusDate = compDate;
			statusName = Drupal.t('LBL1193');
		}else if(RegStatusCode == 'lrn_crs_reg_can') {
			statusDate = updateDate;
			statusName = Drupal.t('LBL026');
		}else if(RegStatusCode == 'lrn_crs_reg_ppm') {
			statusDate = regDate;
			statusName = Drupal.t('LBL704');
		}else if(RegStatusCode == 'lrn_crs_cmp_nsw') {
		    statusDate = compDate;
		    statusName = Drupal.t('LBL704');
	    }else{
			statusDate = regDate;
			statusName = Drupal.t('LBL704');
		}
		//Pipe Sysmbol use this variable 'pipe'
		var pipe = '<span class="enroll-pipeline">|</span>';
		
		html += '<div class="line-item-container float-left" ><span class="vtip" title="' + statusName + ': ' + statusDate + '">' + titleRestrictionFadeoutImage(statusName + ' : ' + statusDate,'exp-sp-lnrenrollment-status',200)+'</span></div>';
		
		if((RegStatusCode == 'lrn_crs_cmp_enr' && complianceCompDate != '' && complianceCompDate != null)){
                statusDate = complianceCompDate;
                statusName = Drupal.t('LBL234');
                html += '<div class="line-item-container float-left">'+pipe;
                html += '<span class="vtip" title="' + statusName + ': ' + statusDate + '">' + titleRestrictionFadeoutImage(statusName + ' : ' + statusDate,'exp-sp-lnrenrollment-status', 50) + '</span></div>';
            }
//		html += pipe;
//		if(Drupal.settings.mylearning_right===false)
//			html += '<span class="vtip" title="' + Drupal.t('LBL096') + ': ' + unescape(classCode).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(classCode,'exp-sp-lnrenrollment-classCode', 50) + '</span>'; // Code
//		else
//			html += '<span class="vtip" title="' + Drupal.t('LBL096') + ': ' + unescape(classCode).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(classCode,'exp-sp-lnrenrollment-classCode', 5) + '</span>'; // Code
		if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_cmp' || RegStatusCode == 'lrn_crs_cmp_inp') {
		var progress = '';
		/* Viswanathan uncommented the below code to display tracking percentage*/
			var progressPercent = 0;
			if(baseType == 'VOD') {
				 for(j=0; j < rowObject.launch.length; j++){
                     var suspendData = (rowObject.launch[j].SuspendData != undefined && rowObject.launch[j].SuspendData != null && rowObject.launch[j].SuspendData != '') ? JSON.parse(unescape(rowObject.launch[j].SuspendData)) : null;
                     progressPercent = progressPercent + (suspendData != null && !isNaN(parseFloat(suspendData['progress'])) ? parseFloat(suspendData['progress']) : 0);
				 }
				 progress += Math.round(progressPercent/rowObject.launch.length) + '% '+Drupal.t('Completed');
				 html += ((progress != '')) ? '<div class="line-item-container float-left">'+(pipe) : '';
				 html += ((progress != '') ? '<span class="vtip" title="'+progress+'">'+(titleRestrictionFadeoutImage(progress,'exp-sp-lnrenrollment-percentCompletion'))+'</span></div>' : '');
			}/* Viswanathan uncommented the below code to display tracking percentage*/
				 }
		var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
		 var clslocation = 'clsLocationSingle';
		  var clssession = 'clsSessionSingle';
		  if(sessLen>1){
			  clslocation = 'clsLocationMultiple';
			  clssession = 'clsSessionMultiple';
			}
	
		var newdelType;
		if(dataDelTypeCode=='lrn_cls_dty_vcl' && RegStatusCode == 'lrn_crs_cmp_att') {
                  newdelType = Drupal.t('Attended');
                  html += pipe+newdelType;
				}		
		var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
		if(ClassLoc && (RegStatusCode != 'lrn_crs_cmp_cmp') ) {
		
		  if(Drupal.settings.mylearning_right===false){
			 
				html += '<div class="line-item-container float-left" >'+pipe+'<span class="vtip '+clslocation+'" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrenrollment-classloc',20)+'</span></div>';
			    //html += pipe+'<span class="session-time-zone '+clssession+'" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+');\'>'+startDateForTitle+'</span>';
				
			}else
				html += pipe+'<span class="vtip" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrenrollment-classloc',9)+'</span>';
		}
		if(passParams && (baseType =="ILT" || baseType =="VC")&& (RegStatusCode != 'lrn_crs_cmp_cmp') ){
			html += '<div class="line-item-container float-left" >'+pipe+'<span class="session-time-zone '+clssession+'" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDateForTitle,'exp-sp-lnrenrollment-timezone')+'</span></div>';
		if(baseType =="ILT" && rowObject.sessionDetails[0].sess_timezone!=rowObject.sessionDetails[0].user_timezone)
        	html += qtip_popup_paint(classId,startDateForTitleloc,sessLen); 
		}
		//#73112	
		if(RegStatusCode == 'lrn_crs_cmp_cmp'){			
			if(IsCompliance == 1) {	
				if(compliance_val_date != null && compliance_val_date != ''){
					html += '<div class="line-item-container float-left">'+pipe;
					if(compliance_expire == true){
						html += "<span class='comp-label vtip' title='+Drupal.t('LBL028')+' : ' +compliance_val_date+'>"+titleRestrictionFadeoutImage(Drupal.t("LBL028")+" : " +compliance_val_date,'exp-sp-lnrenrollment-validitydays')+"</span>";
					} else if(compliance_expire == false) {
						html += "<span class='comp-label vtip' title='"+Drupal.t('LBL735')+' : ' +compliance_val_date+"'>"+titleRestrictionFadeoutImage(Drupal.t('LBL735')+" : " +compliance_val_date,'exp-sp-lnrenrollment-validitydays')+"</span>";
					}
					html +='</div>';
				}
			}
		}
		//#73112		
		if(dataRegStatusCode=="lrn_crs_reg_wtl" && waitlist_priority !=''){
		  var wtlCount = SMARTPORTAL.t(rowObject['labelmsg']['msg5']) + ' : '+ waitlist_priority;
		  html += '<div class="line-item-container float-left" >'+pipe+'<span>'+titleRestrictionFadeoutImage(wtlCount,'exp-sp-lnrenrollment-waitlist',200)+'</span></div>';
		}
		if(dataRegStatusCode == "lrn_crs_cmp_cmp" || dataRegStatusCode == "lrn_crs_cmp_inp"  || dataRegStatusCode == "lrn_crs_cmp_enr"){
			//html += pipe;
			html += "<span id='compTabDetails"+dataId+"' class='compTabDetails'>";
			html += ((AttemptLeft != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+AttemptLeft+'">'+titleRestrictionFadeoutImage(AttemptLeft,'exp-sp-lnrenrollment-attemptsleft',200))+'</span></div>' : '');
			//html += ((ValidityDays != '' && AttemptLeft != '') ? (pipe) : '');
			html += ((ValidityDays != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+ValidityDays+'">'+titleRestrictionFadeoutImage(ValidityDays,'exp-sp-lnrenrollment-validitydays',200))+'</span></div>' : '');
		//	html += ((( ValidityDays != '' || AttemptLeft != '' )) ? (pipe) : '');
			//html += ((classScore != '') ? (classScore) : '');
		//	html += ((VersionNo != '' && ( ValidityDays != '' || AttemptLeft != '' )) ? (pipe) : '');
			html += ((VersionNo != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+VersionNo+'">'+titleRestrictionFadeoutImage(VersionNo,'exp-sp-lnrenrollment-version',200))+'</span></div>' : '');
			if(dataRegStatusCode == "lrn_crs_cmp_cmp") {
				html += ((classScore != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+classScore+'">'+titleRestrictionFadeoutImage(classScore,'exp-sp-lnrenrollment-score',200))+'</span></div>' : '');
				html += ((quizStatus != '' && ( ValidityDays != '' || AttemptLeft != '' || classScore != '' || progress != '')) ? '<div class="line-item-container float-left">'+(pipe) : '');
				html += ((quizStatus != '' && ( ValidityDays == '' && AttemptLeft == '' && classScore == '' && progress == '')) ? pipe : '');
				html += ((quizStatus != '') ? '<span class="vtip" title="'+quizStatus+'">'+titleRestrictionFadeoutImage(quizStatus,'exp-sp-lnrenrollment-quizstatus')+'</span></div>' : '');
			}
			html += "</span>";
		}else{
			html += ((AttemptLeft != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(AttemptLeft,'exp-sp-lnrenrollment-attemptsleft',200))+'</div>' : '');
			html += ((ValidityDays != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(ValidityDays,'exp-sp-lnrenrollment-validitydays',200))+'</div>' : '');
			html += ((classScore != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(classScore,'exp-sp-lnrenrollment-score',200))+'</div>' : '');
		}
		html += ' </div>';
		//var displayDesc = addExpanColapse(decodeURIComponent(shortDescription),decodeURIComponent(descriptionFull),dataDelTypeCode,'ME',dataId);
		var displayDesc =decodeURIComponent(shortDescription) && decodeURIComponent(descriptionFull);
		var crsMoreType= dataDelTypeCode;
		html += '<div class="limit-desc-row ' +crsMoreType+'">';
		html += '<span class="limit-desc limit-desc-enr vtip" id="classShortDesc_'+dataId+'"><span class="cls-learner-descriptions">'+displayDesc+'</span></span>';
		html += '</div>';
		html += '<div class="cp_seemore" id="seemore_'+dataId+'" onclick="seeMoreMyLearning('+dataId+',\'myenrollmentclass\');">'+ Drupal.t('LBL713') +'</div>';
		html += '</div>';

		return html;
		}catch(e){
			// to do
		}
	},
	displayTip : function(elementid, messagecontent){
		try{
		if(!document.getElementById("tooltip"+elementid)) {
			$('#'+elementid).qtip({
				 content: '<div id="tooltip'+elementid+'" class="tooltiptop"></div><div class="tooltipmid">'+messagecontent+'</div><div class="tooltipbottom"></div>',
			     show:{
					when:{
						event:'mouseover'
					},
					effect:'slide'
				 },
				 hide: {
					when:{
						event:'mouseout'
					},
					effect:'slide'
				},
				position: { adjust: { x: -75, y: 0 } },
				style: {
					width: 325,
					background: 'none',
					'font-size' : 12,
					color: '#333333'
				}
			});
		}
		}catch(e){
			// to do
		}
	},

	getSessionDetails : function(sessData,sessionId,baseType) {
		try{
		var data= eval($(sessData).attr("data"));
		var sessionDet = '';
    
		if(!document.getElementById("session_det"+sessionId)) {
			var sessionDet1 = '';
			$(data).each(function(){
			if(baseType == "ILT")
				sessionDet1 += "<span class='qtip_session enroll-session-time time-zone-ilt'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> "+"  "+$(this).attr("usertimezonecode")+" "+$(this).attr("user_tzcode")+"</span>";
			else
			sessionDet1 += "<span class='qtip_session enroll-session-time'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> </span>";
			
			});
			$('#session_det_popup'+sessionId).qtip({
				// content: '<div id="session_det'+sessionId+'" class="tooltiptop"></div><div class="tooltipmid">'+sessionDet1+'</div><div class="tooltipbottom"></div>',
				 content: '<div id="session_det'+sessionId+'" class="qtip_session_top tooltiptop"></div><div class="qtip_session_mid tooltipmid">'+sessionDet1+'</div><div class="qtip_session_bottom tooltipbottom"></div>',
			     show:{
					when:{
						event:'mouseover'
					},
					effect:'slide'
				 },
				 hide: {
					when:{
						event:'mouseout'
					},
					effect:'slide'
				},
				position: {
				    corner: {
				       target: 'bottomLeft',
				       tooltip: 'topLeft'
				    }
				},
				style: {
					width: 325,
					background: 'none',
					border:'0',
					color: '#333333'
				}
			});
			
		}
		}catch(e){
			// to do
		}
	},

	getClassDetails : function(catdiv,accordionLink,parentObj){
		try{
		var data= eval(accordionLink.attr("data"));
		$('#openclose_enr_'+data.rowId).removeClass('title_open');
		$('#openclose_enr_'+data.rowId).addClass('title_close');
		var ostr = '';
        $('#ClassDetailsMainDiv').remove();
        ostr += parentObj.paintDetailsClassSection(data);
		catdiv.html(ostr);
		catdiv.css('display','block');
		$('.dt-child-row-En > td').attr('colSpan','6');
		$('.enrollment-class-table tr td.enroll-class-schedule').css('padding','0');
		}catch(e){
			// to do
		}
	},

	getLocationDetails : function(data) {
		try{
		var data= eval($(data).attr("data"));
		var enrollId	   = data.enrollId;
		var LocationName   = data.LocationName;
		var LocationId	   = data.LocationId;
		var locAddr1 	   = unescape(data.LocationAddr1);
		var locAddr2 	   = unescape(data.LocationAddr2);
		var locCity		   = unescape(data.LocationCity);
		var locState 	   = unescape(data.LocationState);
		var locCountryName = unescape(data.LocationCountry);
		var locZip 		   = data.LocationZip;
		var locPhone       = data.LocationPhone;

		var inQtip 		= $.inArray(enrollId,this.qtipListArr);
		if((inQtip == -1)) {
			this.qtipListArr.push(enrollId);
			var locValue = '';
			locValue += "<span class='enroll-location-details'>"+LocationName+"</span>";
			if(locAddr1 !='' && locAddr1 != null) {
				locValue += "<span class='enroll-location-details'>"+locAddr1+"</span>";
			}
			if(locAddr2 !='' && locAddr2 != null) {
				locValue += "<span class='enroll-location-details'>"+locAddr2+"</span>";
			}
			if(locCity !='' && locCity != null) {
				locValue += "<span class='enroll-location-details'>"+locCity+"</span>";
				if (locState == '' && locState != null) {
					locValue += "<br />";
				}
			}
			if(locState !='' && locState != null) {
				if (locCity != '' && locCity != null) {
					locValue += ", ";
				}
				locValue += "<span class='enroll-location-details'>"+locState+"</span>";
			}
			if(locZip !='' && locZip != null){
				locValue += "<span class='enroll-location-details'>"+locZip+"</span>";
			}
			if(locCountryName !='' && locCountryName != null){
				locValue += "<span class='enroll-location-details'>"+locCountryName+"</span>";
			}

			$('#paint-location'+enrollId).qtip({
				 content: locValue,
			     show:{
					when:{
						event:'mouseover'
					},
					effect:'slide'
				 },
				 hide: {
					when:{
						event:'mouseout'
					},
					effect:'slide'
				},
				position: {
				    corner: {
				       target: 'bottomRight',
				       tooltip: 'topLeft'
				    }
				},
				style: {
					width: 250,
					background: '#dfe5eb',
					border: {
						radius: 10,
						width: 5,
						color: '#dfe5eb'
					},
					padding:5,
					tip: true, // Give it a speech bubble tip with automatic corner detection
					name: 'cream', // Style it according to the preset 'cream' style
					color: '#333333'
				}
			});
		}
		}catch(e){
			// to do
		}
	},

	paintDetailsClassSection : function(data){
		try{
		var ostr = '';
		var dataInfo = data;
		var shortDescription = unescape(data.shortDescription);
		var classId  = data.classId;
		var courseId = data.courseId;
		var enrollId = data.RegId;
		var clsTitle = data.title;
		var SessStartDate = data.SessStartDate;
		var SessStartDay = data.SessStartDay;
		var SessEndDate = data.SessEndDate;
		var sessionId = data.session_id;
		var user_tzcode = data.user_tzcode;
		var startDateFormat = data.startDateFormat;
		var updated_by_ins_mngr_slf = data.updated_by_ins_mngr_slf;
		var created_by_ins_mngr_slf = data.created_by_ins_mngr_slf;
		var baseType = data.BaseType;
		var regStatusCode = data.regStatusCode;
		var regstatusDate = data.regstatusDate;
		var show_events   = data.show_events;
		var dayRemainVal = data.dayRemainVal;
		var complianceCompleteDays = data.complianceCompleteDays;
		var complianceCompleteDate = data.complianceCompleteDate;
		var complianceValidityDays = data.complianceValidityDays;
		var complianceValidityDate = data.complianceValidityDate;
		var complianceExpire	   = data.complianceExpire;
		var complianceExpiredOnValidity = data.complianceExpiredOnValidity;
		var isCompliance		   = data.isCompliance;
		var fullName			   = data.FullName;
		var courseTitle 		= data.courseTitle;
		var noBorderLastRecClass = ($("#pager").is(":visible") || data.is_last_rec != 'last')? '' : 'noborder';
		var Obj = $("#learner-enrollment-tab-inner").data("enrollment");
		var userId = this.enrUserId;
		var RegStatusCode		= data.regStatusCode;
		var regDate				= data.regDate;
		var compDate			= data.compDate;
		var updateDate			= data.updateDate;
		var dateComnplete    = '';
		var dateEnrolled    = '';
		var isExtempted = data.is_exempted;
		var extemptedBy = data.exempted_by;
		var extemptedOn = data.exempted_on;

		eval(complianceExpire);


		// Class details main div
		ostr += '<td colspan="3"><div id="ClassDetailsMainDiv" class="enroll-accordian-div ' + noBorderLastRecClass + '"> <div class="session-details-course-row-innertbl main-item-container">';

		//Session & Location detail section -- start
		/*if(document.getElementById("session_det_popup"+classId)) {
			var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			ostr += '<div class="enroll-session-details session-row"><span class="session-label">'+Drupal.t("LBL670")+' :</span><div class="session-desc">';
			var inc = 1;
			var LocationName = '';
			LocationName 	= dataInfo.LocationName;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
		if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
		else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				//ostr += '<div class="sessionDet"><div class="sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				ostr += '<div class="sessionDet"><div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				ostr += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
				//ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				ostr += '<div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				inc++;
			});
			ostr += '</div></div>';
			var sesinc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					if(LocationName!='null' && LocationName!=''){
					ostr += '<div class="sessAddDet location-row"><span class="location-label">'+Drupal.t("Location")+' :</span>';
					ostr += '<div class="location-desc"><span class="name-add">'+LocationName+',</span>';
					}
					if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='')
						ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
					if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='')
						ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
					if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
						ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
					if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
						ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
					if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+',</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					if(LocationName!='null' && LocationName!=''){
		ostr += '</div>';
					}
					}

			});
		}*/

		// Coursee Title section -- start
		ostr += '<div class="enroll-session-details course-row line-item-container">';
		//if(Drupal.settings.mylearning_right===false)
			ostr += '<span class="wbt-course-title course-label container">'+ Drupal.t("Course")+' '+Drupal.t("LBL083")+ ' : </span><div class="course-desc"><span class="vtip" title="'+unescape(htmlEntities(courseTitle))+'">'+titleRestrictionFadeoutImage(unescape(htmlEntities(courseTitle)),'exp-sp-lnrenrollment-wbt-course-title',70)+'</span></div>';  // course title
//		else
//			ostr += '<span class="wbt-course-title course-label container">'+ Drupal.t("Course")+' '+Drupal.t("LBL083")+ ' : </span><div class="course-desc"><span class="vtip" title="'+saniztizeJSData(courseTitle)+'">'+titleRestrictionFadeoutImage(saniztizeJSData(courseTitle),'exp-sp-lnrenrollment-wbt-course-title',35)  +'</span></div>';  // course title
		ostr += '</div>';
		// Coursee Title section -- end
		var dataDesc = "data={'ClassId':'"+classId+"','ClassDesc':'"+shortDescription+"'}";
		var data1 = "data={'classId':'"+classId+"','courseId':'"+courseId+"','regStatusCode':'"+regStatusCode+"'}";

		//Enrollment detail section -- start


			if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_inp' || RegStatusCode =='lrn_crs_cmp_att'){
				if(isExtempted == 1){
					ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('Waived') +" " + Drupal.t("by") +" : " +  "</span><div class='enr-status-cls status-desc'>" +  extemptedBy +" "+Drupal.t("LBL945")+" " +extemptedOn+'</div></div>';
				}else{
					if(isCompliance == 1) {
			        	if(dataInfo.CreatedBy == 0) {
			        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025') +" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';

			        	}else {
			        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025') +" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			        	}
			        	//0041143: For this ticket ,this part is important
			        	/*if(complianceExpire == 'true'){
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t("LBL028")+" : " +"</span>"+complianceExpiredOnValidity;
						} else if(complianceExpire == 'false') {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL735')+" : " +"</span>"+complianceExpiredOnValidity;
						} else {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL234')+" : " +"</span>"+complianceExpiredOnValidity;
						}*/

			        }else {
			        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName :dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
				}
			        
			}else if(RegStatusCode == 'lrn_crs_cmp_cmp'){
				if(isCompliance == 1) {
					/*if(complianceValidityDate != null && complianceValidityDate != ''){
						//	0043140: Modified by Priya.
						if(complianceExpire == 'true'){
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t("LBL028")+" : " +"</span>"+complianceValidityDate;
						} else if(complianceExpire == 'false') {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL735')+" : " +"</span>"+complianceValidityDate;
						} /*else {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL234')+" : " +"</span>"+complianceCompleteDays;
						}*/
					//}
					if(dataInfo.CreatedBy == 0) {
						ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';
					} else {
						ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
					//ostr += "<span title='"+updated_by_ins_mngr_slf+"'>";
					ostr += "<div class='status-row line-item-container vtip' title='"+updated_by_ins_mngr_slf+"'><span class='status-label container"+((userId != dataInfo.UpdatedBy) ? ' markcompletestatus' : ' completedby')+"'>"+((userId != dataInfo.UpdatedBy) ? Drupal.t('LBL747')+' '+Drupal.t('by') : Drupal.t('LBL681'))+" : " + "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +compDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
					//ostr += "</span>";
					ostr += "</div>";
				}else{
					ostr += "<div class='status-row line-item-container'><span class='status-label container"+((userId != dataInfo.UpdatedBy) ? ' markcompletestatus' : ' completedby')+"'>"+((userId != dataInfo.UpdatedBy) ? Drupal.t('LBL747')+' '+Drupal.t('by') : Drupal.t('LBL681'))+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +compDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
					ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				}
			}else if(RegStatusCode == 'lrn_crs_cmp_inc' || RegStatusCode == 'lrn_crs_cmp_nsw'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL1193')+" : " +  "</span><span class='enr-status-cls status-desc'>" +compDate+'</span></div>';
				if(isCompliance == 1) {
		        	if(dataInfo.CreatedBy == 0) {
		        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';
		        	}else {
			        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
		        }else {
		        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
		        }
			}else if(RegStatusCode == 'lrn_crs_reg_can'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL680')+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +regstatusDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			}else if(RegStatusCode == 'lrn_crs_reg_ppm' || RegStatusCode == 'lrn_crs_reg_wtl'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			}
		/*if(dayRemainVal != ''){
		ostr += '<div class="days-remain"><span>'+dayRemainVal+'</span></div>';
		}*/

			//Attachment detail section -- start
			/*var attachData = eval($("#class_attachment_"+enrollId).attr("data"));
			if(attachData.length > 0) {
				ostr += '<div class="enroll-attach attach-row"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+' :'+'</span><span class="attach-info">'+ Drupal.t("LBL232")+'</span></div>';
				ostr += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
				var len = attachData.length;
				var inc = 1;
					$(attachData).each(function(){
							--Issue fix for the ticket - 32781 --
							var attachment_url = unescape(saniztizeJSData($(this).attr('Title')));
						//	if(attachment_url.length > 60) attachment_url = attachment_url.substring(0,60)+"...";
						attachment_url = titleRestrictionFadeoutImage(attachment_url,'exp-sp-lnrenrollment-attachment-name');
							ostr += "<li class='vtip' title='"+unescape(saniztizeJSData($(this).attr('Title')))+"'>"+"<a href='javascript:void(0);' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+attachment_url+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
							inc++;
					});
				ostr += "</ul></div></div>";
			}*/
			//Attachment detail section -- end	

		//Session & Location detail section -- start
		if(document.getElementById("session_det_popup"+classId)) {
			var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			var inc = 1;
			var LocationName = '';
			LocationName 	= dataInfo.LocationName;
			/*$(sessionDet).each(function(){ //sessionDate sessionDay
			var usertimezonecodes= $(this).attr("sessionTitle");
				var locationsess = '<div class="sessDate">'+$(this).attr("sessionSDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayFormloc")+"</span>"+' to '+$(this).attr("sessionEDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayFormloc")+'</span>'+$(this).attr("session_code")+' '+$(this).attr("tz_code")+'</div>' ;
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				ostr += '<div class="sessionDet"><div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				ostr += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
				if(baseType == "ILT")
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span>'+' '+$(this).attr("usertimezonecode")+' '+user_tzcode;
				else
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
	
	
				if(baseType == "ILT" && $(this).attr("sessiontimezone")!= $(this).attr("usertimezone") ){
					ostr += qtip_popup_paint($(this).attr("sessionId"),locationsess,'',1); 
              }
          
            ostr += '</div></div>';
				inc++;
			});*/
			
			var sesinc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					if(LocationName!='null' && LocationName!=''){
					ostr += '<div class="sessAddDet location-row line-item-container"><span class="location-label container">'+Drupal.t("Location")+' :</span>';
					LocationName =   LocationName + ",";
					if(LocationName.length > 110)
							loctitle = titleRestrictionFadeoutImage(unescape(LocationName),'exp-sp-myenroll-loc',110);
						else
							loctitle = unescape(LocationName);
					ostr += '<div class="location-desc"><span class="name-add"> <span class="vtip" title="'+unescape(htmlEntities(LocationName))+'">'+loctitle+'</span> </span>';
					}
					if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='')
						ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
					if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='')
						ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
					if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
						ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
					if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
						ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
					if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+',</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					if(LocationName!='null' && LocationName!=''){
					ostr += '</div>';
					}
					}

			});
			
			//var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			ostr += '</div><div class="enroll-session-details session-row main-item-container"><span class="session-label session-container">'+Drupal.t("LBL277")+' :</span><div class="session-desc ">';
			//var inc = 1;
			//var LocationName = '';
			//LocationName 	= dataInfo.LocationName;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				var usertimezonecodes= $(this).attr("sessionTitle");
				var locationsess = '<div class="sessDate">'+$(this).attr("sessionSDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayFormloc")+"</span>"+' to '+$(this).attr("sessionEDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayFormloc")+'</span>'+$(this).attr("session_code")+' '+$(this).attr("tz_code")+'</div>' ;
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				var sesionInsName = ($(this).attr("sInsName") != '') ? unescape($(this).attr("sInsName")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				    var sesInstName = titleRestrictionFadeoutImage(sesionInsName,'exp-sp-lnrenrollment-instructor-names');
				ostr += '<div class="sessionDet">';
				ostr += '<div class="cls-sessionTitle-container line-item-container"><div class="lbl-sessionTitle sessionDetlbl container">'+Drupal.t("LBL107")+' :'+'</div> <div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div></div>';
				ostr += '<div class="cls-sessDate-container line-item-container"><div class="lbl-sessionDate sessionDetlbl container">'+Drupal.t("LBL042")+' :'+'</div> <div class="session-day-date-container"> <div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
			//	ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				if(baseType == "ILT")
					ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span>'+' '+$(this).attr("usertimezonecode")+' '+user_tzcode+'</div>';
					else
					ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div>';
					if(baseType == "ILT" && $(this).attr("sessiontimezone")!= $(this).attr("usertimezone") ){
						ostr += qtip_popup_paint($(this).attr("sessionId"),locationsess,'',1); 
						//ostr += '</div>';
	                }
				ostr += '</div></div>';
				ostr += '<div class="cls-sessionInstructor-container line-item-container"><div class="lbl-sessionInstructor sessionDetlbl container">'+Drupal.t("Instructor")+' :'+'</div> <div class="vtip sessionInstructor-val" title="'+(($(this).attr("sInsName") != '') ? htmlEntities(unescape($(this).attr("sInsName"))) : "")+'">'+sesInstName+'</div></div>';
				ostr += '</div>';
				inc++;
			});
			ostr += '</div></div>';
			
		}

		//Attachment detail section -- start
		var attachData = eval($("#class_attachment_"+enrollId).attr("data"));
		if(attachData.length > 0) {
			ostr += '<div class="enroll-attach attach-row"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+' :'+'</span><div class="attach-info">'+ Drupal.t("LBL232")+'</div></div>';
			ostr += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
			var len = attachData.length;
			var inc = 1;
				$(attachData).each(function(){
						/*--Issue fix for the ticket - 32781 --*/
						var attachment_url = unescape(saniztizeJSData($(this).attr('Title')));
					//	if(attachment_url.length > 60) attachment_url = attachment_url.substring(0,60)+"...";
					attachment_url = titleRestrictionFadeoutImage(attachment_url,'exp-sp-lnrenrollment-attachment-name');
						ostr += "<li class='vtip' title='"+unescape(saniztizeJSData($(this).attr('Title')))+"'>"+"<a href='javascript:void(0);' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+attachment_url+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
						inc++;
				});
			ostr += "</ul></div></div>";
		}
		//Attachment detail section -- end

		//Session & Location detail section -- end

	

		ostr += '</div></td>';
		return ostr;
		}catch(e){
			// to do
		}
	},

	getClassOption : function(enrollId,regStatusCode) {
	 try{
		if($('#class-option-'+enrollId).html() == ''){
		    this.createLoader('ClassDetailsMainDiv');
			var url = this.constructUrl("ajax/class-option/" + enrollId);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader('ClassDetailsMainDiv');
					$("#class-option-"+enrollId).slideDown(1000);
					obj.classOptionPaint(result,enrollId,regStatusCode);
					$('#show-alternative-events_'+enrollId).css('display','none');
					$('#hide-alternative-events_'+enrollId).css('display','block');
				}
		    });
		}
		else{
			$("#class-option-"+enrollId).slideDown(1000);
			$('#show-alternative-events_'+enrollId).css('display','none');
			$('#hide-alternative-events_'+enrollId).css('display','block');
		}
		}catch(e){
			// to do
		}
	},

	getClassOptionHide : function(enrollId) {
	 try{
		$("#class-option-"+enrollId).slideUp(1000);
		$('#hide-alternative-events_'+enrollId).css('display','none');
		$('#show-alternative-events_'+enrollId).css('display','block');
	 }catch(e){
			// to do
		}
	},

	classOptionPaint : function(data,enrollId,regStatusCode) {
	 try{
		var res_length = data.length;
		var rhtml = "";
		var c;
		var courseId;
		var classId;
		var classCode;
		var ScheduledClassId;
		var NodeId;
		var StartDate;
		var EndDate;
		var clsTitle;
		var DeliveryType;

		var clsAddEven;
		if(res_length > 0 ) {
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-moreevent' cellspacing='0' width='100%'>";
			for(c=0; c < data.length ; c++){
				courseId 	= data[c]['courseid'];
				classId 	= data[c]['classid'];
				classCode 	= unescape(data[c]['code']);
				//ScheduledClassId = data[c]['ScheduledClassId'];
				NodeId 		= data[c]['nodeid'];
				StartDate 	= data[c]['startdateformat'];
				StartTime 	= data[c]['starttime'];
				EndTime 	= data[c]['endtime'];
				DeliveryType 	 = data[c]['deliverytype'];
				clsTitle 	= data[c]['name'];

				StartDate = (StartDate != null)? StartDate : '';
				StartTime = (StartTime != null)? StartTime : '-';
				EndTime   = (EndTime != null)? EndTime : '-';
				var curDay ='-';
				if(StartDate != ''){
					var myDays = ["Sun","Mon","Tue","Wed","Thu","Fri","Sat","Sun"];
					var myDate = new Date(eval('"'+StartDate+'"'));
					curDay = myDays[myDate.getDay()];
				}
				else{
					StartDate = '-';
				}
				clsAddEven = (c % 2 == 0)? 'odd' : 'even';
				var isSwitchLink = '';
				if(regStatusCode == 'lrn_crs_cmp_enr' || regStatusCode == 'lrn_crs_cmp_inp'){
					isSwitchLink = "<a href='javascript:void(0);' class='actionLink' name='Select' onclick='"+this.widgetObj+".switchEnroll("+classId+","+enrollId+",\""+clsTitle+"\");' >"+SMARTPORTAL.t("Select")+"</a>";
				}
		 			rhtml += '<tr class="'+clsAddEven+'">';
					rhtml += '<td class="enrollment-class-name">'+clsTitle+'</td>';
					rhtml += '<td class="enrollment-class-day">'+curDay+'</td>';
					rhtml += '<td class="enrollment-class-date">'+StartDate +'</td>';
					rhtml += '<td class="enrollment-class-start">'+StartTime+'</td>';
					rhtml += '<td class="enrollment-class-end">'+EndTime+'</td>';
					rhtml += '<td class="enroll-show-select">'+isSwitchLink+'</td>';
					rhtml += '</tr>';
		  }
		  rhtml += '</table>';
		}
		else{
			rhtml += "No Events";
		}
		$("#class-option-"+enrollId).html(rhtml);
		$('.enroll-show-moreevent tr:last').css('border-bottom','0px');
	 }catch(e){
			// to do
		}
	},

	switchEnroll : function(classId,enrollId,classTitle){
		try{
		if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != ""){
			this.confirmPricedEvent(classId,classTitle);
		}
		else{
			this.createLoader('class-option-'+enrollId);
			var url = this.constructUrl("ajax/switch-enroll/" + classId + "/" + enrollId);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader('class-option-'+enrollId);
					obj.callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
				}
		    });
		}
		}catch(e){
			// to do
		}
	},

	confirmPricedEvent : function(cid,vTitle){
		try{
		 $('#div-confirm-price').remove();
		var message  = "<div id='div-confirm-price' >";
			message += "<div class='dialogInner'>" + Drupal.t('MSG399')+"<br />"+Drupal.t('MSG400')+"</div>";
			message += "</div>";

		$('body').append(message);
		var closeButt={};
		closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#div-confirm-price').remove();};
		$("#div-confirm-price").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:vTitle,
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
		$("#div-confirm-price").show();
		$('.ui-dialog-titlebar-close').click(function(){
			$("#div-confirm-price").remove();
		});
		}catch(e){
			// to do
		}
	},

	getLaunchDetails : function(enrollId) {
		try{
		var passdata = $('#launch'+enrollId).attr('data');
		var data = eval($('#launch'+enrollId).attr('data'));
		var html = '<div id="launch-content-container">';
		html += '<table id="paintEnrollmentResults" cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-launch-table">';
		var lesCnt = 0;
		var obj = this.widgetObj;
		var widgetObj = this;
		var contentArr = new Array();
		$(data).each(function(){
			$(this).each(function(){
				var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				var launchFrom = '';
				if(this.LaunchFrom =='CL')
					var launchFrom = $(this).attr('LaunchFrom');
				var contValidateMsg = $(this).attr('contValidateMsg');
				var contentType = $(this).attr('launchType');
		          var contentId = $(this).attr('ContentId');
		          var VersionId = $(this).attr('VersionId');
				  var lcnt = $(this).attr('Lessoncnt');
				  var pipe = '<span class="enroll-pipeline">|</span>';

				  var AvailableScore = ($(this).attr('ContScore') == '') ? '' : $(this).attr('ContScore');
				  var contentQuizStatus = '';
				  /*if(lcnt > 1) {
					  contentQuizStatus = widgetObj.getConsolidatedQuizStatus(data);
				  }
				  else {
					  contentQuizStatus = ($(this).attr('contentQuizStatus') == '') ? '' : $(this).attr('contentQuizStatus');
				  }*/
				  var launchInfo = data;
				  var contentQuizStatus = widgetObj.getConsolidatedQuizStatus(contentId, launchInfo);
//				  var contentQuizStatus = ($(this).attr('contentQuizStatus') == '') ? '' : $(this).attr('contentQuizStatus');
				  var score ='';
				  if(AvailableScore !=null && AvailableScore !=undefined && AvailableScore != '' && AvailableScore != '0.00') {
					      var score = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + AvailableScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + AvailableScore ,'exp-sp-lnrenrollment-score')+'</span></div>';
				  }
				  if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
					  contentQuizStatus = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-sucstatus') + '</span></div>';
				  }
				  var attemptsLeft = '';
				  var attemptsLeft = ($(this).attr('AttemptLeft') == 'notset')? '' : $(this).attr('AttemptLeft');
				  if (attemptsLeft !== '') {
					  attemptsLeft = '<div class="line-item-container float-left" >'+pipe +'<span class="vtip" title="'+Drupal.t('LBL202')+' : '+attemptsLeft+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL202') +' : '+' ' + attemptsLeft,'exp-sp-lnrenrollment-attemptsleft')+'</span></div>';
				  }

				  var validityDays = ($(this).attr('ValidityDays') == '')? '' : $(this).attr('ValidityDays');
				  var diffDays = $(this).attr('remDays');

				  if (validityDays !== '') {
					  var remValidityDays = ($(this).attr('daysLeft') !== undefined && $(this).attr('daysLeft')  != null && $(this).attr('daysLeft')  !== 'null') ? $(this).attr('daysLeft')  : '';
//					  var remValidityDays = validityDays - diffDays;
					  if(remValidityDays <= 0){
					  var daysLBL = Drupal.t("Expired");//Expired
					  remValidityDays = '';// To avoid result as Validity: 0 Expired
					  }else if(remValidityDays == 1){
					  var daysLBL = Drupal.t("LBL910");//day
					  }else if(remValidityDays > 1){
					  var daysLBL = Drupal.t("LBL605");//days
					  }
					  validityDays = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604')+' : '+remValidityDays+' '+daysLBL+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-lnrenrollment-validitydays')+'</span></div>';
				  }
				
				var percentCompleted = '';
				if(contentType == 'VOD') {
					var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != '') ? JSON.parse(unescape($(this).attr('suspend_data'))) : null;
					var progress = suspend_data != null ? suspend_data['progress'] : 0;
					progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
					//console.log(progress);
					percentCompleted = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+ progress+'% '+Drupal.t('Completed') + '">'+titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
				}
	
				var versionNo = $(this).attr('VersionNum');
				  if (versionNo !== '') {
					  versionNo = '<div class="line-item-container float-left" >'+pipe +'<span class="vtip" title="'+Drupal.t('LBL1123')+' : '+versionNo+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL1123') +' : '+' ' + versionNo,'exp-sp-lnrenrollment-versionNo')+'</span></div>';
				  }
	
				  var Dstatus = Drupal.t('In progress');
				  var Dstatus1 = Drupal.t('Incomplete');
				  var Dstatus2 = Drupal.t('Completed');
				  if ($.inArray(contentId, contentArr) == -1) {
					  contentArr.push(contentId);
					  var lessonId = $(this).attr('Id');
					  var contentStatus = ($(this).attr('ContentStatus') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('ContentStatus'));
				  if(contentType == 'VOD') {
						  contentStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : $(this).attr('Status');
				  }

					  html += '<tr id="lesson-launch-' + contentId + '">';
					  html +=   '<td class="enroll-launch-column1">';
					  html +=     '<div class="enroll-course-title">';
					  html +=       '<span id="lesson_title_' + contentId + '"></span>';
					  if (lcnt > 1) {
					    html +=     '<a id="lesson-accodion-' + contentId + '" href="javascript:void(0);"' +
					                    ' data="' + passdata + '"' +
					                      ' class="title_close"' +
					                        ' onclick=\'' + obj + '.addAccordionInLessonView(this.className, "0 0", "0 -61px", "dt-child-row-En", ' +
					                                                                          obj + '.getLessonViewDetails, this, ' + obj + ', true);\'>' +
					                  '&nbsp;' +
					                '</a>';
					  }
					  
					  if(lcnt > 1){
						  html +=       '<span id="titleAccEn_'+contentId+'" class="item-title multilesswidth" >';
					  }else{
						  html +=       '<span id="titleAccEn_'+contentId+'" class="item-title" >';
					  }
					 
					  html +=         '<span class="enroll-class-title"><span class="vtip"' +
					                       ' title="'+unescape($(this).attr('ContentTitle')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
					  html +=           titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('ContentTitle'))),'exp-sp-lnrenrollment-content-title',45)+'</span></br>';
					  html +=           '<div class="item-title-code individual-content">';
					  html +=             '<span id="lesson_status_'+contentId+'"><div class="line-item-container float-left" ><span title="'+Drupal.t('LBL102')+': '+Drupal.t(contentStatus)+'" class="vtip">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+': '+ Drupal.t(contentStatus),'exp-sp-lnrenrollment-contentstatus',200)+'</span></div>' + score + contentQuizStatus + attemptsLeft + validityDays + percentCompleted +versionNo;
					  html +=           '</span></div>';
					  html +=         '</span>';
					  html +=       '</span>';
					  html +=     '</div>';
					  html +=   '</td>';
					  html +=   '<td class="enroll-launch-column2">';
					  if (lcnt == 1 || contentType == 'VOD') {
			            var IsLaunchable  = eval($(this).attr('IsLaunchable'));
			            var contValidateMsg = $(this).attr('contValidateMsg');
			            var contentType = $(this).attr('launchType');
			            var lessonId = $(this).attr('Id');
			            if (contentType == 'VOD'){
				        	var title = $(this).attr('Title');
						    title = encodeURIComponent(title.replace(/\//g, '()|()'));
							var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != "") ? JSON.parse(unescape($(this).attr('suspend_data'))) : null;
							// var progress = suspend_data != null ? suspend_data['progress'] : 0;
							//console.log(progress);
						    callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
						    callLaunchUrlFn += title + "/";
						    callLaunchUrlFn += escape($(this).attr('contentSubTypeCode')) + "/";
						    callLaunchUrlFn += escape($(this).attr('url1').replace(/\//g, '()|()')) + '/';
						    callLaunchUrlFn += "ME" + "/";
						    callLaunchUrlFn += $(this).attr('courseId') + "/";
						    callLaunchUrlFn += $(this).attr('classId') + "/";
						    callLaunchUrlFn += $(this).attr('Id') + "/";
						    callLaunchUrlFn += $(this).attr('VersionId') + "/";
						    callLaunchUrlFn += $(this).attr('enrollId') + "/";
							callLaunchUrlFn += escape($(this).attr('StatusCode')) + "/";
						    // callLaunchUrlFn += progress + "/";
						    callLaunchUrlFn += $(this).attr('suspend_data')+ "\"";
							//console.log('callLaunchUrlFn');
			            }
			            else{
			            	var params = "";
			            	if(launchFrom == 'CL')
			            		params = "{'ContentId':'"+contentId+"','LaunchFrom':'"+launchFrom+"','Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			                      "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"'," +
			                      "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"' }";
			            	else
			            	params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			                      "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"'," +
			                      "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
			            	if(launchFrom == 'CL')
			            		callLaunchUrlFn = "onclick=\"$('#lnr-catalog-search').data('contentLaunch').launchWBTContent("+params+")\"";
			            	else
			            	callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
			            }
	            var lessonLocation = $(this).attr('LessonLocation');
	            var launchData = $(this).attr('launch_data');
	            var suspendData = $(this).attr('suspend_data');
	            var exit = $(this).attr('exit');
	            lessonLocation = lessonLocation == null || lessonLocation == 'null'? '' : lessonLocation;
	            launchData = launchData == null || launchData == 'null'? '' : launchData;
	            suspendData = suspendData == null || suspendData == 'null'? '' : suspendData;
	            exit = exit == null || exit == 'null'? '' : exit;
	            var relaunchInfo ='';
	            relaunchInfo += "{";
	            relaunchInfo += "'lessonlocation':'"+lessonLocation+"'";
	            relaunchInfo += ",'launchData':'"+launchData+"'";
	            relaunchInfo += ",'suspendData':'"+suspendData+"'";
	            relaunchInfo += ",'exit':'"+exit+"'";
	            relaunchInfo += "}";
	    		var relaunchData = "["+relaunchInfo+"]";
	    		var dataparam = 'data="'+params+'"';
	            var ajaxCss = ((contentType == 'VOD') && (IsLaunchable))? ' use-ajax ctools-modal-ctools-video-style ' : ' ';
					    html +=   '<div class="enroll-main-list">';
					    html +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
	                            ' class="' + ((IsLaunchable)? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
					   	html +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
	                             ' class="' + ((IsLaunchable)? 'actionLink' : '') + ajaxCss + ' enroll-launch"' +
	                             ' data-relaunch="' + relaunchData + '"' +
	                                 ' value="' + Drupal.t('LBL199') + '" '+((IsLaunchable)? callLaunchUrlFn : '') + ((launchFrom == 'CL')? dataparam : '')+' >';
					    html +=     '</label>';
					    html +=   '</div>';
					  }
					  html += '</td></tr>';
				  }
			});
		});

	   html +='</table>';
	   html +='</div>';

	   return html;
		}catch(e){
			// to do
			// console.log(e);
		}
	},

	getLessonViewDetails : function(catdiv,accordionLink,parentObj){
		try{
		return '';
		}catch(e){
			// to do
		}
	},


	addAccordionInLessonView : function(openCloseClass, openpos, closepos, childClass, callback, obj, parentObj, isRemove) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var currTrStr = $(obj).parent().parent().parent().attr("id");
		currTrStr = currTrStr.split('-');
		var currTr = currTrStr[2];
		if(!document.getElementById(currTr + "LessonSubGrid")) {
			$("#lesson-launch-"+currTr).after("<tr id='"+currTr+"LessonSubGrid'></tr>");
			$("#"+currTr+"LessonSubGrid").show();
			$("#lesson-accodion-"+currTr).removeClass("title_close");
			$("#lesson-accodion-"+currTr).addClass("title_open");
			$("#lesson-launch-"+currTr).css('border-bottom','none 0px');
			$("#"+currTr+"LessonSubGrid").slideDown(300);
			var data = eval($("#lesson-accodion-"+currTr).attr("data"));
			var lessonDetSec = this.paintLessonSection(data, currTr);
			$("#"+currTr+"LessonSubGrid").html(lessonDetSec);
		}
		else {
			var clickStyle = $("#"+currTr+"LessonSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"LessonSubGrid").show();
    			$("#"+currTr+"LessonSubGrid").slideDown(300);
    			$("#lesson-accodion-"+currTr).removeClass("title_close");
				$("#lesson-accodion-"+currTr).addClass("title_open");
				$("#lesson-launch-"+currTr).css('border-bottom','none 0px');
    		}
    		else {
    			$("#"+currTr+"LessonSubGrid").hide();
    			$("#"+currTr+"LessonSubGrid").slideUp(300);
    			$("#lesson-accodion-"+currTr).removeClass("title_open");
				$("#lesson-accodion-"+currTr).addClass("title_close");
				$("#lesson-launch-"+currTr).removeClass("ui-widget-content");
				$("#lesson-launch-"+currTr).css('border-bottom','1px solid #cccccc');
    		}
		}
		if(this.currTheme == 'expertusoneV2'){
			$('.enrollment-launch-table tr:last-child').css('border','0px');
		}

		$('.launch-lesson-view').each(function(){
		  $(this).parents("tr").children("td").css({'padding-top' : '0px',"border-top":"0"});
		  $('.enrollment-launch-table').find('tbody tr:visible:last').last().css('border','0px');
		  $('.enrollment-launch-table').find("tbody tr td tr:visible:last-child").css('border','0px');
		});

		//SCrollbar for content dialogbox
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
		else
			$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		resetFadeOutByClass('.launch-lesson-view','item-title-code','line-item-container','MyEnrollments');
		vtip();
		}catch(e){
			// to do
		}
	},

	paintLessonSection : function(data,paramContId){
	 try{
	 	
		var data = eval(data);
		var lesCnt = 0;
		var obj = this.widgetObj;
		var ostr = '';
		ostr += '<td colspan="2"><div class="enroll-accordian-div">';
		ostr += '<table class="launch-lesson-view" cellpadding="0" cellspacing="0" width="100%" border="0">';
		$(data).each(function(){
			$(this).each(function(){
			  var contentId = $(this).attr('ContentId');
			  if(paramContId == contentId){
				  lesCnt++;
				  var params = "";
				  var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				  var launchFrom = '';
				  if(this.LaunchFrom =='CL')
					  var launchFrom = $(this).attr('LaunchFrom');
				  var contValidateMsg = $(this).attr('contValidateMsg');
				  var contentType = $(this).attr('launchType');
				  var VersionId = $(this).attr('VersionId');
				  var lessonId = $(this).attr('Id');
				  if(launchFrom == 'CL')
					  params = "{'LaunchFrom':'"+launchFrom+"','Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			            "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',"+
			            "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
				  else
				  params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
				            "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',"+
				            "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
				  if(launchFrom == 'CL')
	            		callLaunchUrlFn = "onclick=\"$('#lnr-catalog-search').data('contentLaunch').launchWBTContent("+params+")\"";
	            	else
				  callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
				  var lessonLocation = $(this).attr('LessonLocation');
          var launchData = $(this).attr('launch_data');
          var suspendData = $(this).attr('suspend_data');
          var exit = $(this).attr('exit');
          lessonLocation = lessonLocation == null || lessonLocation == 'null'? '' : lessonLocation;
          launchData = launchData == null || launchData == 'null'? '' : launchData;
          suspendData = (suspendData == null || suspendData == 'null' || suspendData == undefined)? '' : suspendData;
          exit = exit == null || exit == 'null'? '' : exit;
          var relaunchInfo ='';
          relaunchInfo += "{";
          relaunchInfo += "'lessonlocation':'"+lessonLocation+"'";
          relaunchInfo += ",'launchData':'"+launchData+"'";
          relaunchInfo += ",'suspendData':'"+suspendData+"'";
          relaunchInfo += ",'exit':'"+exit+"'";
          relaunchInfo += "}";
  		  var relaunchData = "["+relaunchInfo+"]";
  		          var pipe = '<span class="enroll-pipeline">|</span>';
		          var lessonStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('Status'));
		          var contScore = ($(this).attr('LesScore') != '') ? $(this).attr('LesScore') : '';
		          var contentQuizStatus = ($(this).attr('contentQuizStatus') != '') ? $(this).attr('contentQuizStatus') : '';
		          if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
					  contentQuizStatus = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-sucstatus') + '</span></div>';
				  }
		          if(contScore == '') {
		        	  var lessonScore = '';
		          } else {
		        	  var lessonScore = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + contScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') + ': ' + contScore,'exp-sp-lnrenrollment-score') + '</span></div>';
		          }
				  ostr += '<tr class="enroll-lesson-section"><td class="enroll-launch-column1">';
				  ostr += '<div class="enroll-course-title-lesson">';
				  ostr += '<span id="titleAccEn_'+lessonId+'" class="item-title" ><span class="enroll-course-title vtip" title="'+Drupal.t('LBL854')+' '+lesCnt+' : '+unescape($(this).attr('Title')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				  ostr += Drupal.t('LBL854')+' '+lesCnt+': ' +titleRestrictionFadeoutImage(decodeURIComponent($(this).attr('Title')),'exp-sp-lnrenrollment-enroll-course-title',35);
				  ostr += '</span></span>';
				  ostr += '</div>';
				  ostr += '<div class="enroll-class-title-info">';
				  ostr += '<div class="item-title-code individual-lesson">';
				  // fix for mantis ticket #0020086. Added id attribute for status span
				  ostr += '<div class="line-item-container float-left"><span id ="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_status" class="vtip" title="'+Drupal.t('LBL102')+': ' +Drupal.t(lessonStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+' : '+Drupal.t(lessonStatus),'exp-sp-lnrenrollment-status') + '</span></div>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonScore">'+ lessonScore +'</span>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonQuizStatus">'+ contentQuizStatus +'</span>';
				  ostr += ' </span>';
				  ostr += ' </div>';
				  ostr += '</div>';
				  ostr += '</td>';
				  // fix for mantis ticket #0020086. Added id attribute for button
				  ostr += '<td class="enroll-launch-column2">';
				  ostr +=   '<div class="enroll-main-list">';
				  ostr +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
				                  ' class="'+((IsLaunchable) ? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
				  ostr +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
				                   ' class="' + ((IsLaunchable) ? 'actionLink' : '') + ' enroll-launch"' +
                             ' data-relaunch="' + relaunchData + '"' +
				                       ' value="' + Drupal.t('LBL199') + '" '+((IsLaunchable)? callLaunchUrlFn : '') + '>';
				  ostr +=     '</label>';
				  ostr +=   '</div>';
				  ostr += '</td></tr>';
			  }
			});
		});
		ostr += '</table></div"></td>';
		return ostr;
	 }catch(e){
			// to do
		}
	},

	addAccordionToTable1 : function(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableView(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		}catch(e){
			// to do
		}
		vtip();
	},

	addAccordionToTableView : function(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var currTr = $('#class-accodion-'+classId).parent().parent().parent().attr("id");
		var data = eval($("#class-accodion-"+currTr).attr("data"));
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr+" td .enroll-class-title-info").after("<div id='"+currTr+"SubGrid' class='clsDetailsDivNormal'></div>");
			$("#"+currTr+"SubGrid").css("display","block");
			//$("#compTabDetails"+classId).show();// Completed tab css("display","block");
			if(this.currTheme != 'expertusoneV2') {
				$("#class-accodion-"+currTr).removeClass("title_close");
				$("#class-accodion-"+currTr).addClass("title_open");
			}
			//$("#"+currTr).find("td").css('border-bottom','none 0px');
			//$("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','none 0px');
			//$("#class-accodion-"+currTr).parents("tr").children("td").css('border-right','none 0px');
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"SubGrid").css("display","block");
			$("#"+currTr+"SubGrid").slideDown(1000);
			$("#"+currTr+"SubGrid").css("display","block");
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display");
			//$("#compTabDetails"+classId).hide();// Completed tab css("display","none");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			//$("#compTabDetails"+classId).show();// Completed tab css("display","block");
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#class-accodion-"+currTr).removeClass("title_close");
    				$("#class-accodion-"+currTr).addClass("title_open");
    			}
				  $("#"+currTr).removeClass("ui-widget-content");
				  if ($("#pager").is(":visible") || data.is_last_rec != 'last') {
		//		    $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','none 0px');
				  }
				} else {
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$('#seemore_'+currTr).css('display', 'block');
    			$("#"+currTr+"SubGrid").slideUp(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#class-accodion-"+currTr).removeClass("title_open");
    				$("#class-accodion-"+currTr).addClass("title_close");
    			}
				  $("#"+currTr).removeClass("ui-widget-content");
				  $("#"+currTr).addClass("ui-widget-content");
				  if ($("#pager").is(":visible") || data.is_last_rec != 'last') {
					  if(this.currTheme == 'expertusoneV2') {
						  $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','solid 1px #ededed');
					  }else
					  {
						  $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','solid 1px #ccc');
					  }
				  }
    		}
		}
		$("#"+currTr+"SubGrid").find("td").css('border-bottom','none 0px');
		var classDetSec = this.paintDetailsClassSection(data);
		if(document.getElementById("seemore_"+currTr))
			$("#"+currTr+"SubGrid").html('<table><tr>'+classDetSec+'</tr></table>');
		else
		$("#"+currTr+"SubGrid").html(classDetSec);
		if((this.currTheme == 'expertusoneV2') && (data.is_last_rec != 'last')){
			//$("#"+currTr+"SubGrid").find("td").css('border-bottom','1px solid #EDEDED');
			$("#"+currTr+"SubGrid").html('<table><tr>'+classDetSec+'</tr></table>');
		}
		}catch(e){
			// console.log(e);
			// to do
		}
	},

	addAccordionToTable2 : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		this.addAccordionToTable(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		$('.dt-child-row-En > td').attr('colSpan','6');
		Drupal.attachBehaviors();
		}catch(e){
			// to do
		}
	},

	addAccordionToTable_override:function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
        if(openCloseClass!=undefined && childClass!=undefined && callback!=undefined) {
            var mainObj=$(obj).parent().get(0).nodeName.toLowerCase()=="td"?$(obj).parent():$(obj).parent().parent();
            $('.'+openCloseClass).each(function(){
                $(this).css("background-position",closepos);
            });

            $(".ui-state-hover").click(function(){
            	var clickId = $(this).attr("id");
            	if(document.getElementById("child"+clickId)) {
            		var clickStyle = $("#child"+clickId).css("display");
            		if((clickStyle == "none") || (clickStyle == 'undefined') || (clickStyle == 'table-row')) {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","block");
            		} else if(clickStyle == "block") {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","none");
            		}else {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","none");
            		}
            	}
            });

        	var cols = $(obj).parent('td').siblings().length;
        	if(String(mainObj.parent().next().attr("class")).indexOf(childClass+" dt-acc-row")<0) {
                var newObj=mainObj.parent().after("<tr class='"+childClass+" dt-acc-row' id='tempChild'><td colspan='"+cols+"'><div class='"+childClass+"-data' style='display:none;width=100%;'></div></td></tr>").next(0).find("div."+childClass+"-data");
                if(callback!=undefined) {
                    callback(newObj,$(obj),parentObj);
                }
                $(obj).children().css("background-position",openpos);
            }
        } else {
            throw("addAccordionTable : Parameters not specified");
        }
		}catch(e){
			// to do
		}
    },

	paintActions : function(cellval,options,rowObject){
	 try{
		var baseType				= rowObject['basetype'];
		var dateValidTo				= rowObject['valid_to'];
		var dataDelType				= rowObject['delivery_type'];
		var dataDelTypeCode			= rowObject['delivery_type_code'];
		var dataRegStatus			= rowObject['reg_status'];
		var dataRegStatusCode		= rowObject['reg_status_code'];
		var RegStatusCode			= rowObject['reg_status_code'];
		var courseRegStatusCode		= rowObject['reg_status_code'];
		var courseComplitionStatus	= rowObject['reg_status'];
		var courseScore				= rowObject['score'];
		var courseQuizStatus		= rowObject['quiz_status'];
		//var courseGrade				= rowObject['grade'];
		var dataId 					= unescape(rowObject['id']);
		var courseId 				= rowObject['course_id'];
		var classId 				= rowObject['class_id'];
		var classTitle				= rowObject['cls_title'];
		var classStatus				= rowObject['cls_status'];
		var dedClass				= rowObject['dedicated_class_flag'];
		var crsTitle				= rowObject['course_title'];
		var courseTempId			= rowObject['courseid'];
		var surStatus				= rowObject['surveystatus'];
		var assessStatus			= rowObject['assessmentstatus'];
		var preassessStatus			= rowObject['preassessmentstatus'];
		//var attemptsleft			= rowObject['attemptleft'];
		var maxScoreValidationpre	= rowObject['maxscorevaluepre'];
		var maxScoreValidationpost	= rowObject['maxscorevaluepost'];
		var session_id				= rowObject['session_id'];
		var usertimezonecode        = rowObject['usertimezonecode'];
		var launchInfo				= rowObject['launch'];
		var masterEnrollId			= rowObject['master_enrollment_id'];
		var mandatory			    = rowObject['mandatory'];
		var IsCompliance			= rowObject['is_compliance'];
		var recertifyCompliance    	= rowObject['recertify_compliance'];
		var mro						= rowObject['mro'];
		var nodeId 					= rowObject['node_id'];
		var usrId					= rowObject['user_id'];
		var isChangeClass 			= rowObject['switch_events'];
		var dataIdEncrypted			= rowObject['id_encrypted'];
		var classIdEncrypted		= rowObject['class_id_encrypted'];
		//36601: Invalid Error Message in "Change Class" function
		var isPricedClass			= (rowObject['show_price']==0)?'':rowObject['show_price'];
		var ecommerce_enabled_status= Drupal.settings.exp_sp_lnrenrollment.ecommerce_enabled;//36658: Unable to choose another class using "Change Class" function.
		var daysRemaining 			= '';
		var callLaunchUrlFn			= '';
		var contLaunch				= false; // Used to determine whether Launch link opens an accordian for multiple content class (true)
											 // or directly launches content (false)
		var IsLaunchable			= false;
		var remDays			        = rowObject['remDays'];
		var errmsg					= '';
		var contValidateMsg			= '';
		var isMultipleCont			= false;
		var LessonLocation			= '';
		var launch_data				= '';
		var suspend_data			= '';
		var exit					= '';
		var VersionId 				= '';
		var AICC_SID 				= '';
		var AttemptLeft	  = '';
		var dayRemainVal  = '';
		var VersionId = '';
		var launch_data				= rowObject['LaunchData'];
		var starWidgetHtml = rowObject['star_widget'];
		var vcMoreActionFlag		= true;
		var is_exempted = rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'].replace(/'/g, "\\\'");
		var exempted_on = rowObject['exempted_on'];
		var hideShareLink = rowObject['hide_share'];
		if(IsCompliance){
			mandatory = 'Y';
		}

		// Assessment Validation - Max score and questions score total should be equal.
		// At least one assessment should match this condition
		if(assessStatus == 'TRUE' && maxScoreValidationpost == 0)
			assessStatus = false;
		if(preassessStatus == 'TRUE' && maxScoreValidationpre == 0)
			preassessStatus = false;

		var startTime = rowObject['session_start'];
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			var sTime,sTimeForm,eTime,eTimeForm,nTime,stDateFull,enDateFull,srvDate;
			if(sessLen>1) {
				if(baseType =="ILT"){
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					viewButton = 'viewDetails';
					html += "<a type='button' title='"+Drupal.t("Reregister")+"' class='actionLink enroll-launch mypgm-recertify vtip'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+titleRestrictionFadeoutImage(Drupal.t("Reregister"),'exp-sp-mylearning-menulist')+"</a>";

					completeAction = '<li class="action-enable"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';

					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';

					sessLenEnd = sessLen-1;
					sTime = rowObject.sessionDetails[0].session_start_format;
					sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
					eTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
					eTimeForm = rowObject.sessionDetails[sessLenEnd].session_end_time_form;
					nTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
					eDateTime = rowObject.sessionDetails[0].session_end_format;
					stDateFull = rowObject.sessionDetails[0].session_start_time_full;
					enDateFull = rowObject.sessionDetails[sessLenEnd].session_end_time_full;
				}else{
					var ind=0;
					for(var i=0;i<rowObject.sessionDetails.length;i++){
						if(rowObject.sessionDetails[i].session_id == launchInfo[0]['ID'] ){
							ind=i;
							break;
						}
						vcMoreActionFlag=false;
					}
					if(vcMoreActionFlag == false && ind == 0){
						ind = rowObject.sessionDetails.length - 1;
					}
					var endSes = rowObject.sessionDetails.length - 1;
					sTime = rowObject.sessionDetails[ind].session_start_format;
					sTimeForm = rowObject.sessionDetails[ind].session_start_time_form;
					eTime = rowObject.sessionDetails[ind].session_end_format;
					eTimeForm = rowObject.sessionDetails[ind].session_end_time_form;
					nTime = rowObject.sessionDetails[ind].session_end_format;
					eDateTime = rowObject.sessionDetails[ind].session_end_format;
					stDateFull = rowObject.sessionDetails[ind].session_start_time_full;
					enDateFull = rowObject.sessionDetails[ind].session_end_time_full;
					clEnDateFull = rowObject.sessionDetails[endSes].session_end_time_full;
				}
			} else {
				sTime = rowObject.sessionDetails[0].session_start_format;
				sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
				eTime = rowObject.sessionDetails[0].session_start_end_format;
				eDateTime = rowObject.sessionDetails[0].session_end_format;
				eTimeForm = rowObject.sessionDetails[0].session_end_time_form;
				nTime = rowObject.sessionDetails[0].session_end_format;
				stDateFull = rowObject.sessionDetails[0].session_start_time_full;
				enDateFull = rowObject.sessionDetails[0].session_end_time_full;
				clEnDateFull = rowObject.sessionDetails[0].session_end_time_full;
			}
			var srvDate = rowObject.sessionDetails[0].server_date_time;
			var type = rowObject.sessionDetails[0].type;
			var sesStartIlt = new Date(stDateFull);
			var sesEndVC   = new Date(enDateFull);
			var todayDate    = new Date(srvDate);
			var timeDiffIlt    = (sesStartIlt - todayDate);
		}

		if(launchInfo.length) {
			IsLaunchable 			= (launchInfo.length>1) ? true : launchInfo[0]["IsLaunchable"];//alert("IsLaunchable : "+IsLaunchable);
			isMultipleCont			= (launchInfo.length>1) ? true : false;
			errmsg 					= launchInfo[0]["NonLaunchableMessage"];
			contValidateMsg			= launchInfo[0]["contValidateMsg"];
			LessonLocation			= launchInfo[0]['LessonLocation'];
			launch_data				= launchInfo[0]['LaunchData'];
			suspend_data			= launchInfo[0]['SuspendData'];
			exit					= launchInfo[0]['CmiExit'];
			VersionId 				= launchInfo[0]['VersionId'];
			AICC_SID 				= launchInfo[0]['AICC_SID'];
			// If the content is launchable, prepare the launch URL call function string in variable callLaunchUrlFn
			if(IsLaunchable) {
				var id 					= launchInfo[0]["ID"];
				var url1 				= launchInfo[0]["LearnerLaunchURL"];
				var contentTitle       	= launchInfo[0]["Title"];
				var url2 				= launchInfo[0]["PresenterLaunchURL"];
				var status 				= launchInfo[0]["Status"];
				var statusCode			= launchInfo[0]["StatusCode"];
				var launchBaseType		= launchInfo[0]["baseType"];
				var cntType				= launchInfo[0]["ContentType"];
				var cntSubTypeCode 		= launchInfo[0]["ContentSubTypeCode"];
				var MasteryScore 		= launchInfo[0]["masteryscore"];
				var CntStatus			= launchInfo[0]["contentQuizStatus"];
				if (baseType == "VOD")
					var suspendData = (launchInfo[0]["SuspendData"] != null && launchInfo[0]["SuspendData"] != "" && launchInfo[0]["SuspendData"] != undefined) ? JSON.parse(unescape(launchInfo[0]["SuspendData"])) : null;
				var inc = 0;
				if(launchInfo.length > 1) {  // Class has multiple contents, launch URL call function will launch the accordian
					contLaunch = true; // Launch link opens an accordian
					var overrideIsLaunchable = false; // Variable used to determine WBT class launchability from the launchability of each content

					// Prepare content launch data for rendering the content launch links in the accordian
					var passParams = "[";
					$(launchInfo).each(function(){
						 inc=inc+1;
						 passParams += "[{";
						 passParams += "'Id':'"+$(this).attr("ID")+"'";
						 passParams += ",'ContentId':'"+$(this).attr("ContentId")+"'";
						 passParams += ",'VersionId':'"+$(this).attr("VersionId")+"'";
						 passParams += ",'VersionNum':'"+$(this).attr("VersionNum")+"'";
						 passParams += ",'enrollId':'"+dataId+"'";
						 passParams += ",'Title':'"+$(this).attr("Title")+"'";
						 passParams += ",'ContentTitle':'"+escape($(this).attr("Code"))+"'";
						 passParams += ",'url1':'"+$(this).attr("LearnerLaunchURL")+"'";
						 passParams += ",'courseId':'"+courseId+"'";
						 passParams += ",'IsLaunchable':'"+$(this).attr("IsLaunchable")+"'";
						 passParams += ",'AttemptLeft':'"+$(this).attr("AttemptLeft")+"'";
						 passParams += ",'ValidityDays':'"+$(this).attr("ValidityDays")+"'";
						 passParams += ",'remDays':'"+$(this).attr("remDays")+"'";
						 passParams += ",'contValidateMsg':'"+$(this).attr("contValidateMsg")+"'";
						 passParams += ",'daysLeft':'"+$(this).attr("daysLeft")+"'";
						 passParams += ",'classId':'"+classId+"'";
						 passParams += ",'url2':'"+$(this).attr("PresenterLaunchURL")+"'";
						 passParams += ",'ErrMsg':''";
						 passParams += ",'contentType':'"+$(this).attr("ContentType")+"'";
						 passParams += ",'contentSubTypeCode':'"+$(this).attr("ContentSubTypeCode")+"'";
						 passParams += ",'launchType':'"+$(this).attr("LaunchType")+"'";
						 passParams += ",'ClsScore':'"+$(this).attr("ClsScore")+"'";
						 passParams += ",'LesScore':'"+$(this).attr("LesScore")+"'";
						 passParams += ",'ContScore':'"+$(this).attr("ContScore")+"'";
						 passParams += ",'Status':'"+$(this).attr("Status")+"'";
						 passParams += ",'ContentStatus':'"+$(this).attr("ContentStatus")+"'";
						 passParams += ",'Lessoncnt':'"+$(this).attr("Lessoncnt")+"'";
						 passParams += ",'LessonLocation':'"+$(this).attr("LessonLocation")+"'";
						 passParams += ",'launch_data':'"+$(this).attr("LaunchData")+"'";
						 passParams += ",'suspend_data':'"+$(this).attr("SuspendData")+"'";
						 passParams += ",'exit':'"+$(this).attr("CmiExit")+"'";
						 passParams += ",'AICC_SID':'"+$(this).attr("AICC_SID")+"'";
						 passParams += ",'MasteryScore':'"+$(this).attr("masteryscore")+"'";
						 passParams += ",'contentQuizStatus':'"+$(this).attr("contentQuizStatus")+"'";
						 passParams += ",'ContentCompletionStatus':'"+$(this).attr("ContentCompletionStatus")+"'";
						 passParams += ",'StatusCode':'"+$(this).attr("StatusCode")+"'";
						 passParams += "}]";

						 if(inc < launchInfo.length) {
							 passParams += ",";
						 }

						 // Determine launchability of WBT class and VOD class from the launchability of each included content
						 if (baseType == "WBT" || baseType == "VOD") {
							 overrideIsLaunchable = overrideIsLaunchable || $(this).attr("IsLaunchable");
						 }
					});
					passParams += "]";

					// Launchability of a multiple content WBT class or a multiple content VOD class is determined from the launchability of each
					// included content
					if (baseType == "WBT" || baseType == "VOD") {
						IsLaunchable = overrideIsLaunchable;
					}

					var obj = options.colModel.widgetObj;
					callLaunchUrlFn = 'onclick=\''+obj+'.launchMultiContent('+dataId+',this);\'';


				} else { // Class has single content
					contLaunch = false; // Launch link directly launches the content
					if((baseType == "WBT") && (cntType != null) && (cntType != undefined) && (cntType != '')) {
						var params = "{'Id':'"+id+"','enrollId':'"+dataId+"','VersionId':'"+VersionId+"','url1':'"+url1+"','courseId':'"+courseId+"','classId':'"+classId+"','url2':'"+url2+"','ErrMsg':'"+errmsg+"','contentType':'"+cntType+"','Status':'"+status+
						"','LessonLocation':'"+LessonLocation+"','launch_data':'"+launch_data+"','contentQuizStatus':'"+CntStatus+"','suspend_data':'"+suspend_data+"','exit':'"+exit+"','AICC_SID':'"+AICC_SID+"','MasteryScore':'"+MasteryScore+"'}";
						callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
					}
					else if (baseType == "VOD") {
					   //debugger;
						// The title for video is the content title
						var title = contentTitle;
						title = encodeURIComponent(title.replace(/\//g, '()|()'));						
						
						callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
						callLaunchUrlFn += title + "/";
						callLaunchUrlFn += escape(cntSubTypeCode) + "/";
						callLaunchUrlFn += (url1) ? escape(url1.replace(/\//g, '()|()')) + '/' : '' ;
						callLaunchUrlFn += "ME" + "/";
						callLaunchUrlFn += courseId + "/";
						callLaunchUrlFn += classId + "/";
						callLaunchUrlFn += id + "/";
						callLaunchUrlFn += VersionId + "/";
						callLaunchUrlFn += dataId + "/";
						callLaunchUrlFn += escape(statusCode) + "/";
						
						// var progress = suspendData != null ? suspendData['progress'] : 0;
						//console.log('progress--->'+progress);
						// callLaunchUrlFn += progress + "/";
						callLaunchUrlFn += launchInfo[0]["SuspendData"] + "\"";
						//console.log($(this));
					}
					else if(baseType == "WBT" || baseType == "VC") {
						callLaunchUrlFn	= url1;
					}
				}
			}
		}
		// Calculate days remaining for the WBT and VOD content, to disallow cancellation of an expired WBT or VOD class.
		if((baseType=="WBT" || baseType == "VOD") &&
			 ((courseComplitionStatus=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") ||
					 			(dataRegStatusCode=="lrn_crs_cmp_inp"))) {
			var i=0;
			if(dateValidTo!='' && dateValidTo != null){
				  var daystocount = new Date(dateValidTo);
				  //daystocount.setDate(daystocount.getDate()+30)
				  var srvDate = launchInfo[0].server_date_time;
				  today=new Date(srvDate);
				  var oneday=1000*60*60*24;
				  daysRemaining = (Math.ceil((daystocount.getTime()-today.getTime())/(oneday)));
			}
		}
        var vClassId = classId;

		var obj = options.colModel.widgetObj;
		var objEval = eval(options.colModel.widgetObj);
		var callDialogObj = '$("body").data("learningcore")';
		var data1;
		var contdata;
		var multiContentData;
		if(contLaunch) {
			// Multiple contents in class, prepare data for the accordian
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'" + options.rowId + "','CourseId':'" + courseId + "','session_id':'"+session_id+
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode +
							"','regstatus':'" + dataRegStatusCode + "','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode +
							"','daysRemaining':'" + daysRemaining + "','courseComplitionStatus':'" + courseComplitionStatus +
							"','courseScore':'" + courseScore + "','courseQuizStatus':'" + courseQuizStatus + "','detailTab':'true','assMand':'" + mandatory + "','mro':'" 
							+ mro+"','is_exempted':'" + is_exempted + "','exempted_on':'" + exempted_on + "','exempted_by':'" + exempted_by + "'}";
			contdata = "data=["+passParams+"]";
			multiContentData = passParams;
		} else {
			// Prepare data for content launch link
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'" + options.rowId + "','CourseId':'" + courseId +  "','session_id':'"+session_id+
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode + "','regstatus':'" + dataRegStatusCode +
							"','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode + "','daysRemaining':'" + daysRemaining +
							"','courseComplitionStatus':'" + courseComplitionStatus + "','courseScore':'" + courseScore +
							"','courseQuizStatus':'" + courseQuizStatus +
							"','detailTab':'true','assMand':'" + mandatory + "','mro':'" + mro+"','is_exempted':'" + is_exempted + "','exempted_on':'" + exempted_on + "','exempted_by':'" + exempted_by + "'}";
		}

		// Prepare the Action HTML to be returned.
		var html = '';
		var setDropButton = true; // Allow cancellation (drop) of class when setDropButton is set to true.
		if((baseType == 'WBT' || baseType == 'VOD') && daysRemaining < 0){
			setDropButton = false; // Disallow cancellation of class if the WBT class has expired.
		}

		html += "<div class='enroll-main-list' id='enroll-main-action-"+dataId+"'>";
		// Prepare Action button HTML for My Enrollments 'Enrolled' tab
		/**
		 * RegStatusCode lrn_crs_cmp_cmp added to the block as the assessment links are not shown in completed tab
		 * */
		if(RegStatusCode == 'lrn_crs_cmp_enr' ||  RegStatusCode == 'lrn_crs_cmp_inp' || RegStatusCode == 'lrn_crs_cmp_att' || RegStatusCode == 'lrn_crs_cmp_cmp'){ //  RegStatusCode == 'lrn_crs_cmp_cmp' ||
			var isILT = false;
			var isWBT = false;
			var isVC = false;
			var isVOD = false;
			var wbtButtonAction = null; // Variable later used to determined what links to show in the More menu.
			var vodButtonAction = null;
			var iltButtonAction = null;
			var viewButton = null;
			var iltShareButton = null;
			var wbtShareButton = null;
			var iltChangeButton = null;
			var iltCompleteCertificateButton = null;
			var vcjoinButton = null;
			var no_primary_html_button='0';
			var no_drop_down_option='0';

			if (dataDelTypeCode == 'lrn_cls_dty_wbt') { // Show correct button for the WBT class
			     isWBT = true;
				// If is launchable, show the Launch button
				if (IsLaunchable) {
					var launchLabelVal  = Drupal.t("LBL199");//(launchInfo.length > 1) ? Drupal.t('LBL816') : Drupal.t("LBL199");
					wbtButtonAction = 'Launch';
					html += "<input type='button' class='actionLink enroll-launch' " +
									"data=\"" + multiContentData + "\" " +
										"value='" + launchLabelVal + "' " +
											"name='" + SMARTPORTAL.t("Launch") + "' " +
												"id='launch" + dataId + "' " +
													callLaunchUrlFn +
														" >";
				}
				else {
					//wbtButtonAction = 'View';
					var wbtShareButton = 'wbtShareButton';
					if(hideShareLink == 0){
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
							'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
							'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
				   }else{
				   	  no_primary_html_button='1';
				   }
					
					//viewButton = 'viewDetails';
					//html += '<a  id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				}
			} else if (dataDelTypeCode == 'lrn_cls_dty_vod') {
						 isVOD = true;
				        // If is launchable, show the Launch button
				        if (IsLaunchable) {
				          vodButtonAction = 'Launch';
				          if (launchInfo.length > 1) {
				            html += "<input type='button' class='actionLink enroll-launch' " +
				                  "data=\"" + multiContentData + "\" " +  "data-data1=\"" + data1 + "\" " +
				                    "value='" + Drupal.t("LBL199") + "' " +
				                      "name='" + SMARTPORTAL.t("Launch") + "' " +
				                        "id='launch" + dataId + "' " +
				                          callLaunchUrlFn +
				                            " >";
				          } else {
				            html += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
				                          "title='" + Drupal.t("LBL199") + "' " +
				                            "id='launch" + dataId + "' " +
				                              callLaunchUrlFn +
				                                " >" +
				                                  Drupal.t("LBL199") + "</a>";
				          }
				        }
				        else {
				          //viewButton = 'viewDetails';
				          //html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				        	var wbtShareButton = 'wbtShareButton';
				        	if(hideShareLink == 0){
								html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
								'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
								'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
							}else{
							   	  no_primary_html_button='1';
							}
				        }
			// Show enabled/disabled Launch button for the VCL class
			} else if (dataDelTypeCode == 'lrn_cls_dty_vcl') {
					  isVC = true;
					  if(RegStatusCode == 'lrn_crs_cmp_cmp') {
						  	var title = 'Session Details';
							var message = Drupal.t("MSG419");
							html += "<a class='actionLink enroll-launch vtip " +
		        					"' data=\"" + contdata + "\" " +
		        					"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
		        					" id='launch" + dataId + "' href='javascript:void(0);' " +
													"onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
														" >"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
					  }
					  else {
						  	var sesStartVC = new Date(stDateFull);
						  	var sesEndVC   = new Date(enDateFull);
							var todayVC    = new Date(srvDate);
							var timeDiffVC    = (sesStartVC - todayVC);
							var endTimeDiffVC =(sesEndVC - todayVC);
							var allowTime = (resource.allow_meeting_launch) ? (resource.allow_meeting_launch * 60000) : 3600000;
							//Launch appears only in the configured time . If not configured allow_meeting_launch then system take default as 1 hour from session start
							var afterCompleteAllowTime = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete * 60000) : 1800000;
							//Launch appears only in the configured time. If not configured allow_meeting_launch then system take default as half hour from session end
							
							if(timeDiffVC <= allowTime && (endTimeDiffVC+afterCompleteAllowTime) >=0){
									var a = (sesEndVC-todayVC);
									var timer = (afterCompleteAllowTime+a);
									if(RegStatusCode == 'lrn_crs_cmp_att' && type == 'lrn_cls_vct_web'){
													var new_window;
													new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink enroll-launch " +
													"' data=\"" + contdata + "\" " +
															"' name='" + SMARTPORTAL.t("Launch") + "' " +
																" id='launch" + dataId + "' href='javascript:void(0);' " +
																		//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")'>"+ Drupal.t("Re-Join") + "</a>";
																"onclick='"+new_window+";'>"+ Drupal.t("LBL1319") + "</a>";
												refresh_time(timer);
									}else{
												var new_window;
												new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink enroll-launch vtip " +
												"' data=\"" + contdata + "\" " +
														"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
															" id='launch" + dataId + "' href='javascript:void(0);' " +
																	//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")'>"+ Drupal.t("Join") + "</a>";
												"onclick='"+new_window+";refresh_enrolled("+new_window+");'>"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
										}
							}else{
								var title ,message;
								title = Drupal.t('LBL670');
								if(endTimeDiffVC+afterCompleteAllowTime < 0){
									message = Drupal.t("MSG419");
								}
								else{
									message = Drupal.t("MSG420").replace("'", "&#39;")+ ' ' + sTime +' <span class=enroll-session-timefrom>'+sTimeForm+'</span>';
								}
								html += "<a class='actionLink enroll-launch vtip " +
				        		"' data=\"" + contdata + "\" " +
				        				"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
				        					" id='launch" + dataId + "' href='javascript:void(0);' " +
															"onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
																" >"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
							}
					  }
			}else {
				// For ILT classes, the button is always Cancel on Enrolled tab
				isILT = true;
				viewButton = 'viewDetails';
				if(timeDiffIlt >= 0 && hideShareLink == 0){
					iltShareButton = 'iltShareDetails';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
					'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
			    }
				else if(RegStatusCode == 'lrn_crs_cmp_enr' && isPricedClass && ecommerce_enabled_status != '' && ecommerce_enabled_status !=null) {
					iltChangeButton = 'iltChangeButton';
					regMsg = Drupal.t('MSG692');
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Change Class" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class") ,'exp-sp-mylearning-menulist')+ '</a>';
				} else if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw' && isChangeClass && (isPricedClass == null || isPricedClass == '' || isPricedClass == undefined)){
					iltChangeButton = 'iltChangeButton';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Change Class" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
					titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),'exp-sp-mylearning-menulist') + '</a>';
				}
				else{
					if(RegStatusCode == 'lrn_crs_cmp_cmp') {
						iltCompleteCertificateButton = 'iltCompleteCertificateButton';
					   var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
				   	   if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch" data=\"' + data1 +
							'\" name="Certificate"  onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+dataIdEncrypted+'\',\''+classIdEncrypted+'\',\''+objEval.enrUserId+'\',\'CLASS\',\'div_my_transcript\',800,900);" >' + Drupal.t("LBL205") + '</a>';
					   }else{
						   html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip'  data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")' title='"+Drupal.t("LBL205")+"'>" + titleRestrictionFadeoutImage(Drupal.t("LBL205") ,'exp-sp-mylearning-menulist') + "</a>";
					   }
					}else{
						if(hideShareLink == 0){
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch clsDisabledButton vtip" data=\"' + data1 +
							'\" name="Refer" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
							//viewButton = 'viewDetails';
							//html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
						}else{
							no_primary_html_button='1'; 
						}
					}
				}
				//html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
			}
			
			// Prepare the More menu items HTML in variable enrollAction
			var enrollAction = '';		
			// Show Cancel link
		 
			if( RegStatusCode == 'lrn_crs_cmp_enr'){
				if (isWBT || isVOD ) {
					if(setDropButton == true) {
						
						if(no_primary_html_button=='1'){ //make CANCEL button as primary
							
							html+="<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a>";
						    no_primary_html_button='0'; // Reset to 0					
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a></li>";
						} 
											
											
					}
				}else if(isILT){
					if(setDropButton == true) {
					  iltButtonAction = 'Cancel';
					  if(timeDiffIlt >= 0) {
					  	
						  	if(no_primary_html_button=='1'){ //make CANCEL button as primary
						  		html += "<a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" onclick='" + obj + ".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' >" +Drupal.t("LBL109") + "</a>";
							    no_primary_html_button='0'; // Reset to 0				
						  	}else{
						  		enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" onclick='" + obj + ".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' >" +Drupal.t("LBL109") + "</a></li>";
						  	}
						  
								
								
					  }
					}

				}else if(isVC && timeDiffVC >= 0 && vcMoreActionFlag==true){
					if(setDropButton == true) {
						enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a></li>";
					}
				}
			}
			
		 
			// Show Survey link
			var surObj		= '$("#block-take-survey").data("surveylearner")';
			if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) /*&&
					((isWBT && wbtButtonAction == 'Launch') || (isVOD && vodButtonAction == 'Launch') || isILT || isVC )*/) {
						
						
				if(no_primary_html_button=='1'){ //make SURVEY button as primary
					
					html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ",\"NULL\",\"enroll-result-container\");' >" +
											Drupal.t("Survey") + "</a>";
					
					no_primary_html_button='0';
					
				}else{
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ",\"NULL\",\"enroll-result-container\");' >" +
											Drupal.t("Survey") + "</a></li>";
				}		
				
			}

	        //Show Assessment link
			//For ILT and VC the Assessment Link will enable after the Class Session time is complete.

			//if (assessStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
				if(isILT || isVC){
					if(isVC){
						var sessEndDate =clEnDateFull;
						var meeting_complete = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete) : 30;
						var sesEndDateTime = new Date(sessEndDate);
						sesEndDateTime.setMinutes(sesEndDateTime.getMinutes()+parseInt(meeting_complete));
						var sesEndTimeStamp	= sesEndDateTime.getTime();
						var todayTimeStamp	= todayDate.getTime();
					}else{
						var sessEndDate = enDateFull ;
						var sesEndDateTime = new Date(sessEndDate);
						var sesEndTimeStamp	= sesEndDateTime.getTime();
						var todayTimeStamp	= todayDate.getTime();
					}
					if(id==null || id==undefined || id=='undefined')
			        	id='0';
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId;
					if(preassessStatus == 'TRUE'){
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
						Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>"+ "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					if(todayTimeStamp > sesEndTimeStamp){
						enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
								Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
					 }
					}
				}else {
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId+"###"+VersionId;
					
					if(preassessStatus == 'TRUE'){
						
						if(no_primary_html_button=='1'){ //make PRE ASSESSMENT button as primary
							
							html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
											"\" name='takesurvey' onclick='" +
												surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																		escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
													titleRestrictionFadeoutImage(Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>",'exp-sp-mylearning-menulist') + "</a>";
													
							no_primary_html_button='0';
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='takesurvey' onclick='" +
												surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																		escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
													Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
						} 													
												
					}
					
					if(assessStatus == 'TRUE'){
						
						if(no_primary_html_button=='1'){ //make POST ASSESSMENT button as primary
							
							html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
							"\" name='takesurvey' onclick='" +
								surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
														escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
									titleRestrictionFadeoutImage(Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>",'exp-sp-mylearning-menulist') + "</a>";
									
							no_primary_html_button='0';
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
							"\" name='takesurvey' onclick='" +
								surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
														escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
									Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
						} 
							
				     }
			 }
			//}

			//Show refer link
				//change by ayyappans for 41930: Survey & Share link gone once Vod/WBT is Launched
				//Share link should be hidden only when session of VC or ILT classes is invalid
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				/*if(((isWBT && (wbtButtonAction == 'Launch' || wbtButtonAction == 'Survey')) ||
					      (isVOD && (vodButtonAction == 'Launch' || vodButtonAction == 'Survey'))) && classStatus == 'lrn_cls_sts_atv'){*/
				if((isWBT || isVOD) && classStatus == 'lrn_cls_sts_atv' && dedClass != 'Y'&& (hideShareLink == 0) && wbtShareButton==null){  
					enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data="'
												+ data1 + '" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\'' +
												classId + '\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +
												Drupal.t("Share") + '</a></li>';
				}
			}
			else {
				// Show Refer link
				/*if (((isWBT && (wbtButtonAction == 'Launch')) ||
				       (isVOD && (vodButtonAction == 'Launch')) ||
				         ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv') {*/
				if ((isWBT || isVOD || ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv' && iltShareButton == null && dedClass != 'Y' && (hideShareLink == 0) && wbtShareButton==null) {
					enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data=\"' + data1 +
									'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
									'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' + Drupal.t("Share") + '</a></li>';
				}
			}
			
			//36658: Unable to choose another class using "Change Class" function.
			if(RegStatusCode == 'lrn_crs_cmp_enr' && isPricedClass && ecommerce_enabled_status != '' && ecommerce_enabled_status !=null && iltChangeButton == null) {
				regMsg = Drupal.t('MSG692');
				
				if(no_primary_html_button=='1'){ //make CHANGE CLASS button as primary  
					 html += '<a href="javascript:void(0)" class="actionLink enroll-launch vtip" '+
					'name="Change Class" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a>';
						
					no_primary_html_button='0';
					
				}else{ 
					enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" '+
					'name="Change Class" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a></li>';
					
				} 
				
				
			} else if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw' && isChangeClass && (isPricedClass == null || isPricedClass == '' || isPricedClass == undefined)) && iltChangeButton == null){
				
				if(no_primary_html_button=='1'){ //make CHANGE CLASS button as primary				 
					html += '<a href="javascript:void(0)" class="actionLink enroll-launch" '+
						'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
						titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),"exp-sp-mylearning-menulist") + '</a>';
						no_primary_html_button='0';
						no_drop_down_option='1'; 
				}else{
					enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" '+
						'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
						Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a></li>';
				}
				
			}
			
			/**
			 * 39005: In completed tab "certificate" link and rating option not showing
			 * change by: ayyappas
			 * */
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				// Add the Print Certificate More menu item to HTML in completeAction for Completed items


				/*enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/


				  /*
				   * Start # 0041917 -  Qa link is shown in salesforce app
				   * Added By : Ganesh Babu V, Dec 9th 2014 5:00 PM
				   * Description: Check the print certificate whether it triggers from salesforce or expertusone. callback changed according to trigger
				   * Ticket : #0041917 -  Qa link is shown in salesforce app
				   */


				/*enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/

					
				   var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
				   if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {
					   if(iltCompleteCertificateButton==null)
					    enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data=\"' + data1 +
						'\" name="Certificate"  onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+dataIdEncrypted+'\',\''+classIdEncrypted+'\',\''+objEval.enrUserId+'\',\'CLASS\',\'div_my_transcript\',800,900);" >' + Drupal.t("LBL205") + '</a></li>';
				   }else if(iltCompleteCertificateButton==null){
				   	
					   	if(no_primary_html_button=='1'){ //make CERTIFICATE button as primary
					   		
					   		html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + titleRestrictionFadeoutImage(Drupal.t("LBL205"),'exp-sp-mylearning-menulist') + "</a>";
							  
					   		no_primary_html_button='0';
					   		no_drop_down_option='1'; 
					   		
					   	}else{
					   		  enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";
					   	}
	
				   }


				   /* End # 0041917 -  Qa link is shown in salesforce app */


				if(recertifyCompliance > 0){
					if(classStatus == 'lrn_cls_sts_atv') {
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					
					if(no_primary_html_button=='1'){ //make RECERTIFICATE button as primary
						
							html += "<a href='javascript:void(0);' class='actionLink'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+Drupal.t("Reregister")+"</a>";
										
							no_primary_html_button='0';
					   		no_drop_down_option='1'; 
										
					}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink'" +
						"data=\"" + data1 + "\" " +
							"value='" + Drupal.t("Reregister") + "' " +
								"name='" + Drupal.t("Reregister") + "' " +
										"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
											" >"+Drupal.t("Reregister")+"</a></li>";
					}
					
										
										
					}
				}
			}
			
			if(viewButton != 'viewDetails'){
				if(no_primary_html_button=='1'){
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="clsDisabledButton actionLink enroll-launch vtip" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
					no_primary_html_button='0';
				}else{
				  enrollAction += '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';	
				}
				
			}
			// Add html to show the More menu-button/menu (enabled or disabled as appropriate)
			if(enrollAction !='' && ((no_drop_down_option!='1' && RegStatusCode != 'lrn_crs_cmp_inp') || (hideShareLink == 0 && RegStatusCode == 'lrn_crs_cmp_inp'))) { 
					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'></a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + enrollAction + '</ul></div>'; 
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			}
			// add rating option to completed items
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				html += "<div id='enroll-node-"+nodeId+"'>";
				html += "<input type='hidden' id = 'lnrenroll-node' value = '"+nodeId+"'>";
				html += starWidgetHtml;
				html += "<div>";
			}
		}

		// Prepare Action button HTML for My Enrollments 'Completed' tab
		//Commented For Code Refactoring Unwanted Code To Remove in Future
		/*else if(RegStatusCode == 'lrn_crs_cmp_cmp') {
	      var isILT = false;
	      var isWBT = false;
	      var isVC = false;
	      var isVOD = false;
	      var wbtButtonAction = null; // Variables later used to determined what links to show in the More menu.
	      var vodButtonAction = null;

			if (dataDelTypeCode == 'lrn_cls_dty_wbt') { // Show correct button for the WBT class
			  isWBT = true;
				// If is launchable, show the Launch button
				if (IsLaunchable) {
					var launchLabelVal  = Drupal.t("LBL199");//(launchInfo.length > 1) ? Drupal.t('LBL816') : Drupal.t("LBL199");
					wbtButtonAction = 'Launch';
					html += "<input type='button' class='actionLink enroll-launch' " +
					"data=\"" + multiContentData + "\" " +
						"value='" + launchLabelVal + "' " +
						"id='launch" + dataId + "' " +
							"name='" + SMARTPORTAL.t("Launch") + "' " +
									callLaunchUrlFn +
										" >";
				}
				else {
					viewButton = 'viewDetails';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				}
			}
			else if (dataDelTypeCode == 'lrn_cls_dty_vod') {
	       isVOD = true;
	        // If is launchable, show the Launch button
	        if (IsLaunchable) {
	          vodButtonAction = 'Launch';
	          if (launchInfo.length > 1) {
	            html += "<input type='button' class='actionLink enroll-launch' " +
	            "data=\"" + multiContentData + "\" " +  "data-data1=\"" + data1 + "\" " +
	                "value='" + Drupal.t("LBL199") + "' " +
	                "id='launch" + dataId + "' " +
	                  "name='" + Drupal.t("LBL199") + "' " +
	                     callLaunchUrlFn +
	                       " >";
	          } else {
              html += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
                          "title='" + Drupal.t("LBL199") + "' " +
                            callLaunchUrlFn +
                              " >" +
                                Drupal.t("LBL199") +
                                   "</a>";
	          }
	        }
	        // Otherwise show the Survey button if survey is present and surveylearner module button is enabled
	        // Otherwise show the Refer button
	        else {
	          //vodButtonAction = 'Refer';
	        	viewButton = 'viewDetails';
	        	html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
	        }
			}
			else if (dataDelTypeCode == 'lrn_cls_dty_vcl') {
			  isVC = true;
				var title = 'Session Details';
				var message = Drupal.t("MSG419");
				html += "<input type='button' class='actionLink enroll-launch " +
					"' data=\"" + contdata + "\" value='" + Drupal.t("LBL880") +
					"' onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
					"' >";
			} else {
				// For ILT classes, the button is always Refer on Completed tab
				isILT = true;
				viewButton = 'viewDetails';
				html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';

			}

			// Prepare the More menu items HTML in completeAction
			var completeAction = '';
			if(viewButton != 'viewDetails') {
				completeAction += '<li class="action-enable"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
			}
			var surObj		= '$("#block-take-survey").data("surveylearner")';
			if (surStatus == 'TRUE'  && (availableFunctionalities.exp_sp_surveylearner) &&
					((isWBT && wbtButtonAction == 'Launch') || (isVOD && vodButtonAction == 'Launch') || isILT || isVC) ) {

				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" + surObj + ".callTakeSurveyToClass(" +
									classId + ",\"" + escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ");' >" + Drupal.t("Survey") + "</a></li>";
			}

			// Commented - Assessment not available after class completion
			if(((isWBT && (wbtButtonAction == 'Launch' || wbtButtonAction == 'Survey')) ||
				      (isVOD && (vodButtonAction == 'Launch' || vodButtonAction == 'Survey'))) && classStatus == 'lrn_cls_sts_atv'){
					completeAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data="'
											+ data1 + '" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\'' +
											classId + '\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +
											Drupal.t("Share") + '</a></li>';
			}

			if(recertifyCompliance > 0){
				var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink'" +
				"data=\"" + data1 + "\" " +
					"value='" + Drupal.t("Reregister") + "' " +
						"name='" + Drupal.t("Reregister") + "' " +
								"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
									" >"+Drupal.t("Reregister")+"</a></li>";
			}

			//Show Assessment link
			//For ILT and VC the Assessment Link will enable after the Class Session time is complete.

			//if (assessStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
				if(isILT || isVC){
					var sessEndDate = isVC ? clEnDateFull : enDateFull ;
					var sesEndDateTime = new Date(sessEndDate);
					var sesEndTimeStamp	= sesEndDateTime.getTime();
					var todayTimeStamp	= todayDate.getTime();
					if(id==null || id==undefined || id=='undefined')
			        	id='0';
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId;
					if(preassessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1);' >" +
						Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>"+ "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					if(todayTimeStamp > sesEndTimeStamp){
						completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0);' >" +
								Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
					 }
					}
				}else {
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId+"###"+VersionId;
					if(preassessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
										"\" name='takesurvey' onclick='" +
											surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																	escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1);' >" +
												Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0);' >" +
							Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
				}
			}

			// Add the Print Certificate More menu item to HTML in completeAction
			if(RegStatusCode == 'lrn_crs_cmp_cmp'){
				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";
			}

			// Add the Rating More menu item to HTML in completeAction
			//completeAction += "<li><a href='javascript:void(0);' class='actionLink' data=\""+data1+"\" name='Rating' onclick='"+obj+".testAction(this);' >"+SMARTPORTAL.t("Rating")+"</a></li>";

			// Add html to show the more menu (enabled or disabled as appropriate)
			if(completeAction !=''){
				html += "<a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a>";
				html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			}

			html += "<div id='enroll-node-"+nodeId+"'>";
			html += "<input type='hidden' id = 'lnrenroll-node' value = '"+nodeId+"'>";
			html += starWidgetHtml;
			html += "<div>";
		}*/else if( RegStatusCode == 'lrn_crs_cmp_exp' ){
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					viewButton = 'viewDetails';
					if(classStatus == 'lrn_cls_sts_atv' && recertifyCompliance > 0) {
					html += "<label class='enroll-launch-full'><a type='button' title='"+Drupal.t("Reregister")+"' class='actionLink enroll-launch mypgm-recertify vtip'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+titleRestrictionFadeoutImage(Drupal.t("Reregister"),'exp-sp-mylearning-menulist')+"</a></label>";
		
									//	completeAction = '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
		
				//	html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a></div>";
				//	html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
				}
					else {
				html += '<label class="enroll-launch-full">'+'<span style="display:none;" id="class-accodion-'+dataId+'" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'></span><a href="javascript:void(0);"  class="action-link enroll-launch clsDisabledButton" >'+Drupal.t('LBL816')+'</a>'+'</label>';
					}
		}
		// Prepare Action button HTML for My Enrollments 'Pending' tab
		else if (RegStatusCode == 'lrn_crs_reg_ppm' || RegStatusCode == 'lrn_crs_reg_wtl' ) {
				if(setDropButton == true) {
					html += "<label class=\"enroll-launch-full\"><input type='button' class='actionLink enroll-launch' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" onclick='"+obj+".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' ></label>";
					//html += "<div  class='dd-btn'><span class='dd-arrow'></span><a style=\"display:none;\" class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'></a></div>";
					var	pendingAction = '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + pendingAction + '</ul></div>';
				}
				else{
					html += "<input type='button' class='actionLink enroll-launch-gray' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" name='"+SMARTPORTAL.t("Cancel")+"' >";
				}
		}
		if(RegStatusCode == 'lrn_crs_cmp_inc' || RegStatusCode =='lrn_crs_reg_can' || RegStatusCode =='lrn_crs_cmp_nsw') {
				html += '<label class="enroll-launch-full">'+'<span style="display:none;" id="class-accodion-'+dataId+'" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'></span><a href="javascript:void(0);"  class="action-link enroll-launch clsDisabledButton" >'+Drupal.t('LBL816')+'</a>'+'</label>';
		}
		html += "</div>";
		return html;
	 }catch(e){
			// to do
		}
	},

	launchMultiContent : function(enrollId,obj){
		try{
		var closeButt={};
	    $('#launch-wizard').remove();
	  	html="";
		html+='<div id="launch-wizard" class="launch-wizard-content" style="display:none; padding: 0px;">';
	    html+= this.getLaunchDetails(enrollId);
	    html+='</div>';
	    $("body").append(html);
	    Drupal.attachBehaviors();
	    $('.enrollment-launch-table tr:last').css('border-bottom','0px');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		var iframeWidth = 0;
		//var iframeDisWidth = Drupal.settings.widget.widget_details.display_size;
		if(Drupal.settings.widget.widgetCallback == true)
			var iframeWidth = $( document ).width();
		if(iframeWidth < 775 && iframeWidth!=0){
			var cpopwidth = 600;
		}else
			var cpopwidth = 675;

	    $("#launch-wizard").dialog({
	        position:[(getWindowWidth()-700)/2,100],
	        bgiframe: true,
	        width:cpopwidth,
	        resizable:false,
	        modal: true,
	        title: Drupal.t("Content"),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         },
	         zIndex: 999 //0034567: UI issue in while launching Video -> Fix: z-index added to the modal dialog
	    });

	    $("#launch-wizard").show();

		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
			}
	  	if(this.currTheme == "expertusoneV2"){
  		   $('#select-class-onclick-dialog').prevAll().removeAttr('select-class-onclick-dialog');
	 	   $('#launch-wizard').closest('.ui-dialog').wrap("<div id='select-class-onclick-dialog'></div>");
	       $('#select-class-onclick-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		   changeChildDialogPopUI('select-class-onclick-dialog');
		   $('#select-class-onclick-dialog').prev('.ui-widget-overlay').css('display','none');
		   $('#select-class-dialog').prev('.ui-widget-overlay').css('display','none');
		   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
		   }
		   $('#select-class-onclick-dialog #launch-wizard').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
		   if(iframeWidth < 775 && iframeWidth!=0)
			   $('#select-class-onclick-dialog .ui-widget').css('left','10px');
	  	}

		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
	        $("#launch-wizard").remove();
	        $('#select-class-onclick-dialog').next('.ui-widget-overlay').css('display','block');
	        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#select-class-onclick-dialog").remove();
	    });
		resetFadeOutByClass('#launch-wizard','item-title-code','line-item-container','mylearning');
	    //SCrollbar for content dialogbox
		
		
		if(document.getElementById('learner-enrollment-tab-inner'))
	    	$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
	    $('#paintEnrollmentResults tr:first-child td .title_close').trigger("click");
		}catch(e){
			// to do
		}
		vtip();
	},
	
	getLaunchSurveyDetails : function(data) {
		try{
		var data = eval(data);
		var html = '<div id="launch-content-container">';
		html += '<table id="paintEnrollmentResults" cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-launch-table">';
		$(data).each(function(){
			$(this).each(function(){
				var launchFrom = '';
					var surveyId = $(this).attr('survey_id');
					var surveyTitle = $(this).attr('survey_title');
					var enrollId = $(this).attr('enroll_id');
					var enrollDate = decodeURIComponent(unescape($(this).attr('enroll_date')));
					var enrollStatus = $(this).attr('enroll_status');
					var objectId = $(this).attr('object_id');
					var objectTitle = $(this).attr('object_title');
					var objectType = $(this).attr('object_type');
					var surveyCount = $(this).attr('survey_count');
					var launchElligible = ''; 
					var prevCourseTitle = ''; 
					var masterEnrollId = ''; 
					var clickparams = '';
					var buttonValue = '';
					var buttonclass = '';
					var deliverytype = '';
					var programId = '';
					
					if(surveyCount==0) {
						if(Drupal.settings.mylearning.user.survey_multipleTP > 0) {
							launchElligible = $(this).attr('launch_elligible'); 
							prevCourseTitle = $(this).attr('prev_coursetitle'); 
							masterEnrollId = $(this).attr('master_enrollid');
							programId = $(this).attr('program_id');
							
							clickparams = "$('.ui-dialog-titlebar-close').click();";
							if (launchElligible !== '' && launchElligible == 0) {
								
								if(masterEnrollId=='')
									clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-result-container');";
								else {
									if(objectType == 'cre_sys_obt_crt') { clickparams += "$('#mod-accodion-"+programId+"').click();";}
									clickparams += "$('#lp_seemore_prg_"+masterEnrollId+"').click();";
									clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+Drupal.settings.mylearning.user.survey_objectname+"','"+Drupal.settings.mylearning.user.survey_objecttype+"','survey','',"+enrollId+",'NULL','enroll-lp-result-container');";
								}
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
							}
							else {
								clickparams += "$('body').data('learningcore').callMessageWindow('Survey','"+Drupal.t('You need to complete')+" "+decodeURIComponent(unescape(prevCourseTitle))+" "+Drupal.t('Course')+"')";
							}
						} else {
							if(Drupal.settings.mylearning.user.survey_objecttype=='cre_sys_obt_cls'){	
								clickparams = "$('.ui-dialog-titlebar-close').click();";
								clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-result-container');";
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
								clickparams += "setTimeout(function(){$('.survey-main-holder .ui-dialog-buttonset .removebutton').css('display','none');}, 200);";
							} else{
								clickparams = "$('.ui-dialog-titlebar-close').click();";
								clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-lp-result-container');";
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
								clickparams += "setTimeout(function(){$('.survey-main-holder .ui-dialog-buttonset .removebutton').css('display','none');}, 200);";
							}
						}
						buttonValue = Drupal.t('LBL199');
						buttonclass = 'enroll-launch';
					} else {
						buttonValue = Drupal.t('Completed');
						buttonclass = 'enroll-completed';
					}
					
					if(Drupal.settings.mylearning.user.survey_recertify==1) {
						deliverytype = Drupal.t('Certification');
					} else {
						deliverytype = Drupal.t('Class');
					}
				    
				  html += '<tr id="lesson-launch-' + enrollId + '">';
				  html +=   '<td class="enroll-launch-column1">';
				  html +=     '<div class="enroll-course-title">';
				  html +=       '<span id="lesson_title_' + enrollId + '"></span>';
				  html +=       '<span id="titleAccEn_'+enrollId+'" class="item-title" >';
					html +=         '<span class="enroll-class-title"><span class="vtip"' +
					                       ' title="'+unescape($(this).attr('survey_title')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">'+
					                       ' '+titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('survey_title'))),'exp-sp-lnrenrollment-content-title',150);+'';
					  html += '</span></br>';
				  html +=           '<div class="item-title-code individual-content">';
				  if(Drupal.settings.mylearning.user.survey_multipleTP > 0) {
						 html +=   '<span id="lesson_status_'+enrollId+'"><span title="'+Drupal.t('LBL704')+': '+enrollDate+'" class="vtip">'+Drupal.t('LBL704')+': '+ enrollDate+'</span><span class="narrow-search-results-item-detail-pipe-line sharelink-edit-delete-icons">|</span></span>';
						 html +=   '<span class="assoc_object_title" id="lesson_status_'+enrollId+'"><span class="vtip content-subtitle"' +
						  ' title="'+unescape($(this).attr('object_title')).replace(/"/g, '&quot;')+'">'+
						  ' '+Drupal.t('LBL083')+': '+titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('object_title'))),'exp-sp-lnrenrollment-content-title',45);+'';
						  html +=   '</span></span>';
					  } else {
						  html +=  '<span id="lesson_status_'+enrollId+'"><span title="'+deliverytype+' '+Drupal.t('LBL704')+': '+enrollDate+'" class="vtip">'+deliverytype+' '+Drupal.t('LBL704')+': '+ enrollDate+'</span></span>';
				  }
				  html +=           '</div>';
				  html +=         '</span>';
				  html +=       '</span>';
				  html +=     '</div>';
				  html +=   '</td>';
				  html +=   '<td class="enroll-launch-column2">';
				  html +=   '<div class="enroll-main-list">';
					html +=     '<label id="' + enrollId + '_' + surveyId + '_launch_button_lbl"' +
					    ' class="enroll-launch-full launch_button_lbl">';
					html +=       '<input type="button" id="' + enrollId + '_' + surveyId + '_launch_button"' +
					     ' class="actionLink '+buttonclass+'"' +
					     'onclick="'+clickparams+'"'+
					     ' value="' + buttonValue+'"' +
					     ' >';
					html +=     '</label>';
					html +=   '</div>';
					html += '</td></tr>';
				
			});
		});

	   html +='</table>';
	   html +='</div>';

	   return html;
		}catch(e){
			// to do
			// console.log(e);
		}
	},
	
	launchMultiSurvey : function(data){
		try{
		var closeButt={};
	    $('#launch-wizard').remove();
	  	html="";
		html+='<div id="launch-wizard" class="launch-wizard-content" style="display:none; padding: 0px;">';
	    html+= this.getLaunchSurveyDetails(data);
	    html+='</div>';
	    $("body").append(html);
	    Drupal.attachBehaviors();
	    $('.enrollment-launch-table tr:last').css('border-bottom','0px');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		var iframeWidth = 0;
		if(Drupal.settings.widget.widgetCallback == true)
			var iframeWidth = $( document ).width();
		if(iframeWidth < 775 && iframeWidth!=0){
			var cpopwidth = 600;
		}else
			var cpopwidth = 675;

	    $("#launch-wizard").dialog({
	        position:[(getWindowWidth()-700)/2,100],
	        bgiframe: true,
	        width:cpopwidth,
	        resizable:false,
	        modal: true,
	        title: Drupal.t("Survey For")+'&nbsp;'+titleRestrictionFadeoutImage(decodeURIComponent(unescape(Drupal.settings.mylearning.user.survey_objectname)),'exp-sp-lnrenrollment-content-title',45),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         },
	         zIndex: 999 //0034567: UI issue in while launching Video -> Fix: z-index added to the modal dialog
	    });
	    
	    $('.assoc_object_title .fade-out-title-container').css('margin-bottom','-6px');
		$('.assoc_object_title .content-subtitle').css({'overflow':'hidden', 'width': '342px', 'white-space':'nowrap', 'position':'absolute'});	
		$('.assoc_object_title .content-subtitle .fade-out-title-container').css('max-width','312px');
		$('.enroll-class-title .fade-out-title-container .title-lengthy-text').css({'white-space':'normal','line-height':'16px','margin-top':'2px'});
		$('.assoc_object_title .content-subtitle .fade-out-title-container .title-lengthy-text').css({'white-space':'nowrap','margin-bottom':'8px'});
		$('.launch-wizard-content #paintEnrollmentResults .enroll-course-title .enroll-class-title .exp-sp-lnrenrollment-content-title').css('margin-bottom','-12px');
		$('#launch-wizard #paintEnrollmentResults .enroll-course-title .enroll-class-title .item-title-code').css('padding-top','0');
		$('#launch-wizard .enroll-launch-full').css({'width':'auto', 'margin-left':'3px'});
		$('#launch-wizard .enroll-launch-full .enroll-launch').css({'border-radius':'12px', 'height':'24px', 'padding':'2px 0'});
	    $("#launch-wizard").show();
	   
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
			}
	  	if(this.currTheme == "expertusoneV2"){
  		   $('#select-class-onclick-dialog').prevAll().removeAttr('select-class-onclick-dialog');
	 	   $('#launch-wizard').closest('.ui-dialog').wrap("<div id='select-class-onclick-dialog'></div>");
	       $('#select-class-onclick-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		   changeChildDialogPopUI('select-class-onclick-dialog');
		   $('#select-class-onclick-dialog').prev('.ui-widget-overlay').css('display','none');
		   $('#select-class-dialog').prev('.ui-widget-overlay').css('display','none');
		   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
		   }
		   $('#select-class-onclick-dialog #launch-wizard').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
		   if(iframeWidth < 775 && iframeWidth!=0)
			   $('#select-class-onclick-dialog .ui-widget').css('left','10px');
	  	}

		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
	        $("#launch-wizard").remove();
	        $('#select-class-onclick-dialog').next('.ui-widget-overlay').css('display','block');
	        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#select-class-onclick-dialog").remove();
	    });
		
		$('#select-class-onclick-dialog .ui-dialog .ui-dialog-titlebar .fade-out-title-container').css({'float':'right','margin-bottom':'-9px'});
		$('#select-class-onclick-dialog .ui-dialog .ui-dialog-titlebar .fade-out-title-container .fade-out-image').css('background','rgba(0, 0, 0, 0) linear-gradient(to right, transparent, #27245e) repeat scroll 0 0');

	    //SCrollbar for content dialogbox
		if(document.getElementById('learner-enrollment-tab-inner'))
	    	$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		/*if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();*/
	    $('#paintEnrollmentResults tr:first-child td .title_close').trigger("click");
		}catch(e){
			// to do
		}
		vtip();
	},

	//SCROLLBAR IMPLEMENTATION FOR CONTENT SCREEN
	scrollBarContentDialog : function(){
		try{
		var contentDialogHeight = $("#launch-content-container").height();
		if(contentDialogHeight > 550) {
			$(".launch-wizard-content").css('height',550);
			$(".launch-wizard-content").css('overflow','auto');
		} else if(contentDialogHeight <= 550) {
			$(".launch-wizard-content").css('height','auto').css('min-height','70px');
		}
		}catch(e){
			// to do
		}
	},

	showMoreAction : function(obj) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	var position = $(obj).position();
		var posLeft  = Math.round(position.left);
		var posTop   = Math.round(position.top);
		$('.enroll-drop-down-list').toggle();
		$('.enroll-action').hide();
		if(this.currTheme == "expertusoneV2"){			
			if (navigator.userAgent.indexOf("Chrome")>0) {
				$(obj).parent().siblings('.enroll-action').css('position','absolute');
				$(obj).parent().siblings('.enroll-action').css('margin-top',"21px");
			//	$(obj).parent().siblings('.enroll-action').css('right','2px');
				$(obj).parent().siblings('.enroll-action').css('top','2px');
			//	$(obj).parent().siblings('.enroll-action').css('width','90px');
				$(obj).parent().siblings('.enroll-action').css('z-index','-1');
				$(obj).parent().siblings('.enroll-action').slideToggle();
			}else{
				$(obj).parent().siblings('.enroll-action').css('position','');
				$(obj).parent().siblings('.enroll-action').css('margin-top',"21px");
				$(obj).parent().siblings('.enroll-action').slideToggle();
			}
//			if(Drupal.settings.mylearning_right===false){
//
//				$(obj).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
//			      "position":"absolute","z-index":"1000"
//			    });
//				$(obj).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
//				      "position":"absolute","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
//				      "position":"relative","z-index":"0"
//				    });
//				$(obj).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
//				      "position":"relative","z-index":"0"
//				    });
//			}else{
//				$(obj).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
//				      "position":"absolute","margin-right":"0","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
//				      "position":"absolute","margin-right":"0","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
//				      "position":"relative","margin-right":"6px","z-index":"0"
//				    });
//				$(obj).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
//				      "position":"relative","margin-right":"6px","z-index":"0"
//				    });
//			}
			$(obj).parent().siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
			      "z-index":"1000"
			    });
			$(obj).parent().siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
			      "z-index":"1000"
			    });
			$(obj).parent().siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
			      "z-index":"0"
			    });
			$(obj).parent().siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
			      "z-index":"0"
			    });
			}else{
			$(obj).parent().siblings('.enroll-action').css('position','absolute');
			$(obj).parent().siblings('.enroll-action').css('right','16px');
			$(obj).parent().siblings('.enroll-action').css('top',posTop+20);
			$(obj).parent().siblings('.enroll-action').slideToggle("");
			$(obj).parent().siblings('.iframe-mylearning #innerWidgetTagEnroll .enroll-action').css('right','321px'); // salesforce
		}
		}catch(e){
			// to do
		}
	},

	hideMoreAction : function(obj) {
		try{
			this.prevMoreObj = '';
		}catch(e){
			// to do
		}
	},
	getDropPolicy:function(data, loaderDiv)
	{
		try{
			$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			var dataobj= eval($(data).attr("data"));

			var isCommerceEnabled='';
		    var assMand =0;
		    
		    if(dataobj.assMand =='Y' && dataobj.is_exempted != 1){assMand =1;}  
		  
		    var mandByRole	= 0;
		    if(dataobj.mro =='Mandatory' && dataobj.is_exempted != 1){mandByRole =1;}

		    if(!assMand && !mandByRole){
			    if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "")
			    {
			    	isCommerceEnabled = "1";
			    }
		    }
			var closeButt={};
		    $('#dropMsg-wizard').remove();
		  	html="";
			html+='<div id="dropMsg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" class="dropMsg-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
		    html+='<tr><td height="30"></td></tr>';
		    html+='<tr>';
		    html+= '<td align="center" id="dropmsg-content"><span>'+Drupal.t("LBL416")+'</span></td>';
		    html+='</tr>';
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);

		    $("#dropMsg-wizard").dialog({
		        position:[(getWindowWidth()-500)/2,100],
		        autoOpen: false,
		        bgiframe: true,
		        width:500,
		        resizable:false,
		        modal: true,
		        title: Drupal.t("LBL109"),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		    $("#dropMsg-wizard").show();

			$('.ui-dialog-titlebar-close').click(function(){
		        $("#dropMsg-wizard").remove();
		    });
			$("#learner-enrollment-tab-inner").data("enrollment").getDropPolicyCall(dataobj.RegId,dataobj.BaseType,dataobj.classid,isCommerceEnabled,assMand,dataobj.clstitle,mandByRole);
		}catch(e){
			// to do
		}
	},



	getDropPolicyCall : function(enrollId,baseType,classid,isCommerceEnabled,assMand,clstitle,mandByRole)
	{
		try{
		var userId = this.enrUserId;
		var url = this.constructUrl("ajax/get-droppolicy/" + userId +  "/" + baseType + "/" + enrollId+"/"+classid+"/"+isCommerceEnabled+"/"+assMand+"/"+mandByRole);
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				var closeButt={};
				if(drop_policy_info.next_action=="drop")
				{
					$("#dropmsg-content").html('<span>'+Drupal.t("MSG263")+'</span><br /><i>"'+unescape(clstitle)+'"</i>');
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand); $("#dropmsg-content").html(Drupal.t('MSG424'));};
				}else if(drop_policy_info.next_action=="showmsg")
				{
					$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_info.msg)+"</span>");

					var html="";
					html+='<span><b>'+Drupal.t('LBL083')+' : </b>'+unescape(clstitle)+"</span>";
					html+="<br/><br/>";
					html+="<span>"+SMARTPORTAL.t(drop_policy_info.msg)+'</span>';
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
				}else if(drop_policy_info.next_action=="showdroppolicy")
				{
					var html="";
					if(drop_policy_info.deducted_amount == 0 && drop_policy_info.price > 0){
						html+="<span>"+Drupal.t("MSG264")+"<br/> </span>";
						html+="<br/><br/>";
						html+="<span>"+Drupal.t("MSG266")+'</span>';
					}else if (drop_policy_info.price <= 0 && drop_policy_info.refund_amt == 0) {
						html+='<span>'+Drupal.t("MSG263")+'</span><br /><i>"'+unescape(clstitle)+'"</i>';
					}
					else{
						if(drop_policy_info.refund_amt != null && drop_policy_info.refund_amt != undefined && drop_policy_info.refund_amt != "" && drop_policy_info.refund_amt != 0){
							drop_policy_info.refund_amt = drop_policy_info.refund_amt.toFixed(2);
						}
						else
							drop_policy_info.refund_amt = '0.00';
						if(drop_policy_info.deducted_amount != null && drop_policy_info.deducted_amount != undefined && drop_policy_info.deducted_amount != "" && drop_policy_info.deducted_amount != 0){
							drop_policy_info.deducted_amount = drop_policy_info.deducted_amount.toFixed(2);
						}
						else
							drop_policy_info.deducted_amount = '0.00';
					  html+='<span>' + Drupal.t('MSG402')+' ' + drop_policy_info.currency_type +' '+ drop_policy_info.deducted_amount +' '+Drupal.t('MSG401')+"</span>";
					  html+="<br/><br/>";
					  html+='<span class="dropmsgTitle">'+Drupal.t('LBL1165')+': </span>'+drop_policy_info.currency_type+" "+drop_policy_info.refund_amt;
					  html+="<br/><br/>";
					  html+="<span>"+Drupal.t("MSG266")+'</span>';
					}
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"true",isCommerceEnabled,assMand);  $("#dropmsg-content").html(Drupal.t("MSG424")); };
				}
				 $("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
				 $("#dropMsg-wizard").dialog("open");
				 $("#dropMsg-wizard").dialog('option', 'buttons', closeButt);

				 //Append div script
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end()
				 this.currTheme = Drupal.settings.ajaxPageState.theme;
				 if(this.currTheme == "expertusoneV2"){
				 changeDialogPopUI();
				 }
			}
	    });
		}catch(e){
			// to do
		}
	},

	dropEnroll : function(data){
		try{
		this.getDropPolicy(data);
		}catch(e){
			// to do
		}
	},

	dropEnrollCall : function(enrollId,baseType,refundflag,isCommerceEnabled,assMand){  
		try{
		closeButt=new Array();
		$("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
		var userId = this.enrUserId;
		var url = this.constructUrl("ajax/drop-enroll/" + userId +  "/" + baseType + "/" + enrollId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/0");
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				if(drop_policy_result.status=="success")
				{
				 	var dialogObj=$("#dropMsg-wizard").dialog();
				 	dialogObj.dialog('destroy');
				 	dialogObj.dialog('close');
				 	$('#dropMsg-wizard').remove();
				 	var enrStr = Drupal.settings.myenrolmentSearchStr;
					if (typeof enrStr == 'undefined' || enrStr == null || enrStr == undefined){
						$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
					}else {
				    	$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment(enrStr);
				    }
					if(document.getElementById('learningplan-tab-inner')) {
						$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_enr|lrn_tpm_ovr_inp","EnrollmentLP");
					}
					// Reload Instructor view
					if(document.getElementById('instructor-tab-inner')) {
						var insSelTab = ($("#scheduled_tab").parent().hasClass('selected') === true) ? "scheduled" : "delivered";
						$("#instructor-tab-inner").data("instructordesk").callInstructor(insSelTab);
					}
			    	// Reload My-Calendar after enrollment dropped
			    	$("#catalog-admin-cal").data("mycalendar").reloadMyCalendar();
				}
				else {
					$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_result.msg)+"</span>");
				}
			}
	    });
		}catch(e){
			// to do
		}
	},

		showCompliancePopup : function(usrId,classId,courseId,dataId){
		var popupObj = $("#learningplan-tab-inner").data("learningplan");
		var obj = $('body').data('learningcore');
		var url = this.constructUrl("ajax/compliance-class-cnt/"+usrId + "/" + classId +"/" + courseId+"/" + dataId);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				returnMsg = $.trim(result);
				if(returnMsg == 'MultiRegister') {
					popupObj.showSwitchClass(usrId,classId,courseId,dataId,"compliance");
				}else if(returnMsg == 'noclasses' || returnMsg == 'pricedclass'){
					regMsg = (returnMsg == 'noclasses') ? Drupal.t('MSG379') : Drupal.t('Only priced class is available.Register from Catalog page.');
					obj.enrollChangeClassErrorMessage('compliance register', regMsg);//, '', usrId, courseId, classId, '','');
				}else{
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
				}
			}
	    });
	},

	testAction : function(obj){
		try{
		}catch(e){
			// to do
		}

	},
	loadComplete : function(){
	 try{
	    var recs = $("#paintEnrollmentResults").getGridParam("records");
	    if (recs == 0) {
	        $("#paintEnrollmentResults").html(Drupal.t("MSG381")+'.');
	    }
	 }catch(e){
			// to do
		}
	},

	showHide : function (strOne,strTwo) {
		try{
		$('#'+strTwo).toggle();
			var classShowHide = $('#'+strOne).hasClass('clsShow');
			if(classShowHide){
				$('#'+strOne).removeClass('clsShow');
				$('#'+strOne).addClass('clsHide');
			}else{
				$('#'+strOne).removeClass('clsHide');
				$('#'+strOne).addClass('clsShow');
			}
		}catch(e){
			// to do
		}
	},


	getConsolidatedQuizStatus : function(contentId, launchInfo) {
	  //console.log(launchInfo);
		var quizStatus = "";
		var lessonCount = 0;
		$(launchInfo).each(function(i, launch)  {
			var contentInfo = launch[0];
			if(contentInfo.ContentId == contentId) {	//if multiple contents attached to single enrollment, separate status should be considered
				lessonCount ++;
				var successStatus = contentInfo.contentQuizStatus;
				//console.log('hi' +contentInfo);
				if (successStatus == "failed" || successStatus == 'incomplete' || successStatus.trim() == '') {
					quizStatus = successStatus;
					return false;
				}else if( contentInfo.ContentCompletionStatus == 'completed' || (contentInfo.ContentCompletionStatus == '' && contentInfo.contentType!="Tin Can")){			
					quizStatus = "passed";
				} else {
					quizStatus = "";
				}
			}
		});
		return quizStatus;
	},

	capitalizeFirstLetter : function(string) {
	    return string.charAt(0).toUpperCase() + string.slice(1);
	}
	
});

$.extend($.ui.enrollment.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);



$(function() {
	try{
		$( "#learner-enrollment-tab-inner" ).enrollment();
		if(document.getElementById('learner-enrollment-tab-inner')) {
			$('#first_pager').click( function(e) {
				try{
				if(!$('#first_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#prev_pager').click( function(e) {
				try{
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#next_pager').click( function(e) {
				try{
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#last_pager').click( function(e) {
				try{
				if(!$('#last_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#pager .ui-pg-selbox').bind('change',function() {
				try{
				$('#gview_paintEnrollmentResults').css('min-height','auto');
				$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);
				}catch(e){
					// to do
				}
			});

			$("#pager .ui-pg-input").keyup(function(event){
				if(event.keyCode == 13){
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
			});
			//select record count in veiw per page
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				$('#enroll-result-container .page-show-prev').bind('click',function() {
					if(parseInt($("#pg_pager .page_count_view").attr('id')) < 0){
						$("#pg_pager .page_count_view").attr('id','0');
					}else{
						$('#gview_paintEnrollmentResults').css('min-height','100px');
						$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
						$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);

					}
				});
				$('#enroll-result-container .page-show-next').bind('click',function() {
					if(parseInt($("#pg_pager .page_count_view").attr('id')) >= parseInt($("#pg_pager .page-total-view").attr('id'))){
						$("#pg_pager .page_count_view").attr('id',($("#pg_pager .page_count_view").attr('id')-1));
					}else{
						$('#gview_paintEnrollmentResults').css('min-height','100px');
						$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
						$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);

					}
				});

		      }


		}
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").enrollment();
	}catch(e){
		// to do
	}
});

function refresh_enrolled(window,timer){
	if(!window.closed){
	var window = window;
		setTimeout(refresh_enrolled,10000,window);
	}else{
		reload_page();
		refresh_time(timer);
	}
	
}

function refresh_time(timer){
	setTimeout(reload_page,timer);
}

function reload_page(){
	if(document.getElementById('loaderdivenroll-result-container')){
		}else{
		$("#paintEnrollmentResults").setGridParam().trigger("reloadGrid");
}	
}
;
(function($) {

	$.widget("ui.contentLaunch", {
	_init: function() {
	 try{
		var self = this;
		this.obj  = null;
		var scorm_api_init = null; // variable to store the scorm values when initialized 
	 }catch(e){
			// to do
		}
	},
	launchWBTContent : function(data){
	 try{
		if(isLaunched || (winobj !== undefined && !winobj.closed)) {
			$('body').data('learningcore').enrollChangeClassErrorMessage(Drupal.t('Content'), Drupal.t('MSG754'), 1);
			return false;
		}
	  //console.log('In launchWBTContent');
	  //console.log(data);
		var pmType 		= data.contentType;
	    var classid 	= data.classId;
	    var courseid 	= data.courseId;
	    var stdid		= this.getLearnerId();
	    var stdname 	= this.getUserName();
	    var pmPath 		= data.url1;
	    var contentVersion;
	    var contentType;
	    var contentQuizStatus = data.contentQuizStatus;
	    if(stdid == "0" || stdid == "")
	    {
	    	 self.location='?q=learning/enrollment-search';
             return;
	    }
	    if(data.LaunchFrom == 'LP'){
		    $("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
	    	$("#learningplan-tab-inner").data("contentLaunch").updateFrom = 'LP';
	    	if(document.getElementById('learner-enrollment-tab-inner')) {
	    		$("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom = '';
	    	}
	    	disableWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');  // Prevent panel close while launch content
	    }
	    else if(data.LaunchFrom == 'CL'){
	    	$("#lnr-catalog-search").data("contentLaunch").updateFrom ='CL';
	    	//52687: Course after completion is not moving to completed status.Launch button is still retailed in Catalog Page
	    	//commented the bellow line bcz of above issue
//	    	if(document.getElementById('learner-enrollment-tab-inner') || document.getElementById('learningplan-tab-inner')) {
//	    		$("#lnr-catalog-search").data("contentLaunch").updateFrom ='';
//	    	}
	    }
	    else{
		    $("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
	    	$("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom = 'ME';
	    	if(document.getElementById('learningplan-tab-inner')) {
	    		$("#learningplan-tab-inner").data("contentLaunch").updateFrom = '';
	    	}
	    	disableWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment'); // Prevent panel close while launch content
	    }
	    this.launchParam =  {
	        'launchUrl'			: data.url1,
	        'userId' 			: stdid,
	        'username'			: stdname,
	        'courseid' 			: courseid,
	        'classid' 			: classid,
	        'lessonid' 			: data.Id,
	        'versionid'			: data.VersionId,
	        'enrollid' 			: data.enrollId,//this.regId,
	        'prevcontentstatus' : data.Status,
	        'LaunchFrom'		: data.LaunchFrom,
	        'aicc_sid'			: data.AICC_SID
	    };

	    if(pmType.toLowerCase() == 'scorm 1.2' || pmType.toLowerCase() == 'scorm 2004') {
	    	pmType				= pmType;
	    	var pmTypeVer 		= pmType.split(' ');
	        contentType  	= pmTypeVer[0];
	        contentVersion 	= pmTypeVer[1];
	    } else  {
	        contentVersion 	= '';
	        contentType 	= pmType;
	    }

	    var x	= {version: contentVersion,type:contentType};
	    scoobj 	= new SCORM_API_WRAPPER(x);
	    var lessonlocation=data.LessonLocation==null||data.LessonLocation==undefined ||data.LessonLocation=='null'||data.LessonLocation==''?"":data.LessonLocation;
      var launch_data=data.launch_data==null||data.launch_data==undefined ||data.launch_data=='null'||data.launch_data==''?"":data.launch_data;
      var suspend_data=data.suspend_data==null||data.suspend_data==undefined ||data.suspend_data=='null'||data.suspend_data==''?"":unescape(data.suspend_data);
      var exit=data.exit==null||data.exit==undefined ||data.exit==''?"":data.exit;
      var AICC_SID=data.AICC_SID==null||data.AICC_SID==undefined ||data.AICC_SID==''?"":data.AICC_SID;
      suspend_data = decodeURIComponent(suspend_data);
	    // When launched from a Launch button of a multi-content dialog, get lessonlocation, launchData, suspendData, exit from Launch button data-relaunch attribute
	    var launchButton = $('#' + data.Id + '_' + data.enrollId + '_launch_button');
	    if (launchButton.length > 0) {
	      //console.log(launchButton.data('relaunch'));
	      var launchButtonData = eval(launchButton.data("relaunch"));
	      if(launchButtonData.length){
	    	  lessonlocation = launchButtonData[0].lessonlocation;
	    	  launch_data = launchButtonData[0].launchData;
	    	  suspend_data = decodeURIComponent(unescape(launchButtonData[0].suspendData));
	    	  exit = launchButtonData[0].exit;
	      }
	      //var launchButtonData = eval('(' + decodeURIComponent(launchButton.data('relaunch')) + ')');
	      //console.log(launchButtonData);
	      //console.log(typeof launchButtonData);
	    }
	    //console.log('lessonlocation = ' + lessonlocation);
	    //console.log('launch_data = ' + launch_data);
	    //console.log('suspend_data = ' + suspend_data);
	    //console.log('exit = ' + exit);
	    if(checkEncodedState(suspend_data)==false){
        	//suspend_data=encodeURIComponent(suspend_data);
        	//do nothing
        }else{
        	 suspend_data = decodeURIComponent(suspend_data);
        }

		    var newobj = this;
		    $.ajax({
				url: '?q=ajax/getcdnurl/'+data.Id,
				// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					if(document.getElementById('learningplan-tab-inner'))
						$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
					if(document.getElementById('learner-enrollment-tab-inner'))
						$("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
					if(document.getElementById('lnr-catalog-search'))
						$("#lnr-catalog-search").data("enrollment").destroyLoader('enroll-result-container');
					var output = result.split('~~');
					url1      = output[0];
					cdnstatus = output[1];
					
					if(scoobj._type.toLowerCase() == 'scorm') {
						scorm_api_init = new Object();
						scorm_api_init.session_time = '0000:00:00.00';
						scorm_api_init.lesson_location = lessonlocation;
						scorm_api_init.launch_data = launch_data;
						scorm_api_init.suspend_data = suspend_data;
						scorm_api_init.version = scoobj._version;
						if(scoobj._version == '2004') {
							scorm_api_init.completion_status = 'not attempted';
							scorm_api_init.success_status = 'unknown';
						} else {
							scorm_api_init.lesson_status = 'not attempted';
						}
					}
					
					if(cdnstatus == 1){
						var data1 = {
						        url				 : url1,
						        callback		 : newobj.updateScore,
						        learnerId		 : stdid,
						        learnerName		 : stdname,
						        completionStatus : 'not attempted',
						        successStatus	 : 'unknown',
						        scoreMax		 : '0',
						        scoreMin		 : '0',
						        score			 : '0',
						        location		 : lessonlocation,
						        courseid 		 : courseid,
						        classid 		 : classid,
						        lessonid 		 : data.Id,
						        versionid		 : data.VersionId,
						        enrollid 		 : data.enrollId,
						        launch_data		 : launch_data,
						        suspend_data     : suspend_data,
						        exit			 : exit,
						        aicc_sid		 : AICC_SID,
						        MasteryScore	 : data.MasteryScore,
						    };
						    scoobj.Initialize(data1);
					} else {
	    var data1 = {
	        url				 : pmPath,
			callback		 : newobj.updateScore,
	        learnerId		 : stdid,
	        learnerName		 : stdname,
	        contentQuizStatus: data.contentQuizStatus,
	        completionStatus : 'not attempted',
	        successStatus	 : 'unknown',
	        scoreMax		 : '0',
	        scoreMin		 : '0',
	        score			 : '0',
	        location		 : lessonlocation,
	        courseid 		 : courseid,
	        classid 		 : classid,
	        lessonid 		 : data.Id,
	        versionid		 : data.VersionId,
	        enrollid 		 : data.enrollId,
	        launch_data		 : launch_data,
	        suspend_data     : suspend_data,
	        exit			 : exit,
	        aicc_sid		 : AICC_SID,
	        MasteryScore	 : data.MasteryScore,
	    };
	    scoobj.Initialize(data1);
					}
				}
		    });
	 }catch(e){
			// to do
		}
	},



	updateScore : function(callback_param){
	 try{
		 /*
		  * SCORM 1.2 :
	         	result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
	         	result.status = API.LMSGetValue("cmi.core.lesson_status");
		 	SCORM 2004:
     			//SCORM 2004 will have 2 statuses - both should be validated
		         result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
		         result.status = API_1484_11.GetValue("cmi.success_status");				//quiz status
		  */
		var setObj;
		var updateFrom;
		if(document.getElementById('learningplan-tab-inner')) {
			if($("#learningplan-tab-inner").data("contentLaunch").updateFrom == 'LP'){
				setObj = $("#learningplan-tab-inner").data("contentLaunch");
				updateFrom = 'LP';
			}
			activateWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');
		}
		if(document.getElementById('learner-enrollment-tab-inner')) {
			if($("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom == 'ME'){
				setObj = $("#learner-enrollment-tab-inner").data("contentLaunch");
				updateFrom = 'ME';
			}
			activateWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment');
		}
		if(document.getElementById('lnr-catalog-search')){
			if($("#lnr-catalog-search").data("contentLaunch").updateFrom == 'CL'){
				setObj = $("#lnr-catalog-search").data("contentLaunch");
				updateFrom = 'CL';
			}
			if($("#lnr-catalog-search").data("contentLaunch").updateFrom == 'ME'){
				setObj = $("#lnr-catalog-search").data("contentLaunch");
				updateFrom = 'ME';
			}
	    }
        var sestime					= '0';
        var courseComplitionStatus 	= '';//$('#courseComplitionStatus').val();
        var lObj	 				="'"+setObj+"'";
        var launchflag              = 0;
        var crid					= setObj.launchParam.courseid;
        var clid					= setObj.launchParam.classid;
        var lesid					= setObj.launchParam.lessonid;
        var versionid				= setObj.launchParam.versionid;
        var enrollid				= setObj.launchParam.enrollid;
        var stid 					= setObj.launchParam.userId;
        var prevcontentstatus 		= setObj.launchParam.prevcontentstatus;
        var regId					= setObj.launchParam.enrollid;
        var aicc_sid				= setObj.launchParam.aicc_sid;
        var contentType				= "scorm 1.2";
        var contentVersion			= '';
        var result					= (callback_param !== undefined && (scoobj._type.toLowerCase() == 'scorm' || scoobj._type.toLowerCase() == 'knowledge content')) ? callback_param : scoobj.Finish();
        var status 					= '';
        var scmax 					= '';
        var scmin 					= '';
        var score 					= '';
        //var sestimear 				= '';
        var loc 					= '';
        var compstatus				= '';
        var launch_data		 		= '';
        var suspend_data     		= '';
        var exit			 		= '';
        var grade					= '';
        var xstatus					= '';
        if(typeof(result) != undefined && typeof(result) != 'undefined'){
	        status 					= result.status;
	        score 					= result.score;
	        sestime 				= result.sessionTime;
	        loc 					= result.location;
	        compstatus				= result.completionStatus;
	        launch_data		 		= result.launch_data;
	        suspend_data     		= result.suspend_data;
	        exit			 		= result.exit;
	        launchflag				= (callback_param !== undefined) ? result.launchflag : 0;
        }
//       alert('type'+scoobj._type);
//       alert('version'+scoobj._version);
//       alert('quiz'+status);
//       alert('conet'+compstatus);
        //console.log(scoobj._type);
		var is_scorm_api_changed = true;
        if(scoobj._type != "Knowledge Content" && scoobj._type != "Tin Can") {
        	if(prevcontentstatus !='Completed')	{
        		if(scoobj._type.toLowerCase() == 'scorm' && scoobj._version == '2004') {
        			var completion_status = compstatus.toLowerCase();
        			var success_status = status.toLowerCase();

        			if(completion_status == 'completed') {
        				xstatus='lrn_crs_cmp_cmp';
        			}
        			else if(completion_status == 'unknown' && (success_status == 'unknown' || success_status == 'passed')){
        				xstatus='lrn_crs_cmp_cmp';
        			}
        			else {
        				xstatus='lrn_crs_cmp_inp';
        			}
        		}
        		else {
        			if(status.toLowerCase() == "passed" || status.toLowerCase() == "failed" || status.toLowerCase() == "completed" || status.toLowerCase() == "unknown"){
    		        	xstatus='lrn_crs_cmp_cmp';
        			}else{
    		        	xstatus='lrn_crs_cmp_inp';
        			}
        		}

        	}
        	else{
        	xstatus='lrn_crs_cmp_cmp';
        	}
			if(scoobj._type.toLowerCase() == 'scorm') {
//scorm_api_init = new Object();
				if(scoobj._version == '2004') {
					//comparing initialized values with the current scorm result
					if(compstatus == 'not attempted' && scorm_api_init.session_time == sestime && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data && scorm_api_init.success_status == status) {
						is_scorm_api_changed = false;
					}
				} else {
					//comparing initialized values with the current scorm result
					if(status  == 'not attempted' && scorm_api_init.session_time == sestime && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data) {
						is_scorm_api_changed = false;
					}
				}
			}
        }else if(scoobj._type == "Tin Can"){
        	xstatus='lrn_crs_cmp_inp';
        }
		else if(scoobj._type == "Knowledge Content") {
			if(status.toLowerCase()  == "completed") {
				xstatus='lrn_crs_cmp_cmp';
			}
			else {
				xstatus='lrn_crs_cmp_inp';
			}
		}else{
        	xstatus='lrn_crs_cmp_cmp';
        	status="Completed";
        }
        //console.log('xstatus'+xstatus);
//        alert('xstatus'+xstatus);
        //pending values from wrapper completionStatus totalTime
        if(score == undefined || score == '') {
            score='0';
            grade='';
        };
        $('#'+lesid+'_launch_score').html(score);
        $('#'+lesid+'launch_grade').html(grade);

        $('#'+lesid+'_launch_status').html(xstatus);

        $('#surveybutton_'+clid).css("display","block");
        $('#lanchlinks1_'+clid+'').click();
        var type="";
   
        if(setObj.launchParam.launchUrl != null && setObj.launchParam.launchUrl.indexOf("h5p/embed") > 0)
        	{
        	type = "h5p";
        	var mystr = setObj.launchParam.launchUrl;
        	var myarr = mystr.split("/");
        	var h5p_cnt_id= myarr[2];
        	}

        
        var launchData	=	{
        		'regid'		: regId,
                'stid'		: stid,
                'classid'	: clid,
                'courseid'	: crid,
                'lessonid'	: lesid,
                'versionid' : versionid,
                'status'	: xstatus,
                'max'		: scmax,
                'min'		: scmin,
                'score'		: score,
                'grade'		: grade,
                'sestime'	: sestime,
                'location'	: loc,
                'enrollid'	: enrollid,
                'courseComplitionStatus' : courseComplitionStatus,
                'launch_data'	 : launch_data,
    	        'suspend_data'   : encodeURIComponent(suspend_data),
    	        'exit'			 : exit,
    	        'contentstatus'  : status,		//for SCORM 1.2 => lesson_status and for 2004 => success_status
    	        'compstatus'	 : compstatus,	//for SCORM 1.2 => '' and for 2004 => completion_status
				'contenttype'	 : scoobj._type,
				'contentversion' : scoobj._version,
    	        'aicc_sid'		 : aicc_sid,
				'is_scorm_api_changed' : is_scorm_api_changed,
				'launchflag' : launchflag,
				 'type':type,
				'h5p_cnt_id':h5p_cnt_id
				
          };

        var passUrl = 'launch&regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&versionid='+versionid+'&status='+xstatus+'&max='+scmax+'&min='+scmin;
        passUrl = passUrl+'&score='+score+'&grade='+grade+'&sestime='+sestime+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseComplitionStatus;
        passUrl = passUrl+'&contentstatus='+status+'&launch_data='+launch_data+'&suspend_data='+suspend_data+'&exit='+exit+'&aicc_sid='+aicc_sid;

        var url='';
        var post_data='';
        if(scoobj._type == "AICC" || scoobj._type == "AICC Course Structure") {
        	var sid = aicc_sid;
        	passUrl="session_id="+sid+"&command=UPDATELMSDATA";
        	url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
        	post_data={"session_id":callback_param,"command":"UPDATELMSDATA",'launchflag' : launchflag};
        }else if(scoobj._type == "Tin Can"){
        	var sid = aicc_sid;
        	passUrl="Authorization="+sid+"&command=UPDATELMSDATA";
        	url = resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";
        	post_data={"Authorization=":callback_param,"command":"UPDATELMSDATA",'launchflag' : launchflag};
        }else{
        	url = resource.base_url+'/?q='+"ajax/update-launch/launch";
        	post_data = launchData;
        }
        if(launchflag == 0){
	        if(document.getElementById('launch-wizard')) {
	        	$('#launch-wizard').css("overflow-x","hidden");
	        	setObj.createLoader('launch-wizard');
	        }
	        if(document.getElementById('launch-lp-wizard')) {
	        	$('#launch-lp-wizard').css("overflow-x","hidden");
	        	setObj.createLoader('launch-lp-wizard');
	        }
	        if(document.getElementById('learningplan-tab-inner')) {
	        	setObj.createLoader('enroll-lp-result-container');
	        }
	        if(document.getElementById('lnr-catalog-search')){
	          	$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
	          	if(document.getElementById("course-details-display-content"))
	          		$("#course-details-display-content").data("lnrcoursedetails").createLoader('class-list-loader');
	          	if(document.getElementById('learning-plan-details-display-content'))
	                $("#learning-plan-details-display-content").data("lnrplandetails").createLoader('class-container-loader-'+clid);
	            if(document.getElementById("class_detail_content"))
		            $("#class_detail_content").data("lnrclassdetails").createLoader('class_detail_content');
	          	if(document.getElementById("tbl-paintCatalogContentResults"))
	    			$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
	        }
	    }
		var obj = this;
		if(callback_param == null && $('#content-to-maximize').find('iframe').length > 0) {
			$('#content-to-maximize').find('iframe').each(function() {
				$(this).remove();
			});
			Drupal.CTools.Modal.dismiss();
			winobj.closed = true;
			isLaunched = false;
			// window.history.back();
		}
		$.ajax({
			type: "POST",
			url: url,
			data:  post_data,
			success: function(result) {
			if(launchflag == 1){
				return;
			}
			scoobj = new Object();
					//console.log('resr'+result);
					if(document.getElementById('learner-enrollment-tab-inner')) {
					  // extra parameters passed to grid for ticket #0020086, which will update the score, status, etc in client
					  // Only do this when multi content launch dialog is present in DOM
					  if (document.getElementById('launch-content-container')) {
					    //console.log('reloading grid with lesson id and enrollment id');
						  $("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId}});
						  if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
						  	$("#paintEnrollmentResults").trigger("reloadGrid");
						  }
					  }
					  else {
					    //console.log('reloading grid without lesson id and enrollment id as no launch-content-container');
					    if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
					    	$("#paintEnrollmentResults").trigger("reloadGrid");
					    }
					  }
					  setObj.destroyLoader('launch-wizard');
					  $('#launch-wizard').css("overflow","auto");
					}
					if(document.getElementById('learningplan-tab-inner')) {
					  // extra parameters passed to grid for ticket #0020086, which will update the status, etc in client
					  // Only do this when multi content launch dialog is present in DOM
					  if (document.getElementById('lplaunch-content-container')) {
					    //console.log('reloading LP grid with lesson id and enrollment id');
					    $("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId}});
						if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
							$("#paintEnrollmentLPResults").trigger("reloadGrid");
						}
					  }
					  else {
					  	//console.log('reloading LP grid without lesson id and enrollment id as no launch-content-container');
					  	if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
					  		$("#paintEnrollmentLPResults").trigger("reloadGrid");
					  	}
					  }
					  setObj.destroyLoader('launch-lp-wizard');
					  $('#launch-lp-wizard').css("overflow","auto");
					}
					if(document.getElementById('lnr-catalog-search')) {
						if(document.getElementById('launch-wizard'))
							setObj.createLoader('launch-wizard');
		            	$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						url = setObj.constructUrl("ajax/learningcore/register-to-launch/" + stid + '/' + clid + '/' + crid+'/'+regId+'/lrn_cls_dty_wbt/1/'+lesid);
						$.ajax({
							type: "POST",
							url: url,
							//data:  '',//Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
							success: function(result){
								if(result){
									result_arr = result.split('*~*');
									var launched_info = $.parseJSON(result_arr[1]);
									var lessonCnt = result_arr[2];
									$("#lnr-catalog-search").data('enrollment').updateMultiContentLaunchDialog(regId,lesid,launched_info.triggering_lesson_details,launched_info.triggering_content_quiz_status);
									var html_out = '<div '+result_arr[0]+'</div>';
									if(document.getElementById('launch'+regId)){
										$('#launch'+regId).replaceWith(html_out);
										Drupal.attachBehaviors($('#launch'+regId));
									}
									if(document.getElementById('registerCls_'+clid)){
										$('#registerCls_'+clid).replaceWith(html_out);
										Drupal.attachBehaviors($('#registerCls_'+clid));
									}
									paintContentStatus(launched_info, crid, clid, '',lessonCnt,regId);
								}
								$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
								if(document.getElementById('launch-content-container'))
									$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
								if(document.getElementById("tbl-paintCatalogContentResults")){
									if(document.getElementById('launch'+regId)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
									}else if(document.getElementById('registerCls_'+clid)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
									}
				        			//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
									$("#tbl-paintCatalogContentResults").trigger("reloadGrid");
				        		}
								if(document.getElementById('paintContentResults')){
									if(document.getElementById('launch'+regId)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
									}else if(document.getElementById('registerCls_'+clid)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
									}
									//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
									if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
										$("#paintContentResults").trigger("reloadGrid");
									}
								}
								if(document.getElementById('launch-wizard'))
									setObj.destroyLoader('launch-wizard');
								if(document.getElementById("course-details-display-content"))
							    	$("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
							    if(document.getElementById('learning-plan-details-display-content'))
						            $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+clid);
						        if(document.getElementById("class_detail_content"))
							         $("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
							}
					    });

					}
					// Commented status check for Tincan content #0047307 - issue 1
					//if(xstatus == "lrn_crs_cmp_cmp" && document.getElementById("my_transcript_jqgrid")) {
					if(document.getElementById("my_transcript_jqgrid")){
						$("#my_transcript_jqgrid").trigger('reloadGrid');
					}

					for (var line_item in result) {
						if (line_item == 0) {
							if (result[line_item].CompMessage == 'Attempts Over') {
								callLaunchMessageWindow(Drupal.t('LBL929'), Drupal.t('MSG915'));
							}
						}
					}
			}
	    });

	 	}catch(e){
			// to do
	 		//console.log(e)
		}
	  },


	  updateVODScoreOnCtoolsModalClose : function (launchedFrom, courseId, classId, lessonId, versionId, enrollId, prevContentStatus, videoSessionId,type,h5p_cnt_id) {
	  try{
		  //console.log(this);
		  // console.log('updateVODScoreOnCtoolsModalClose');
		  // console.log('expjwPlayerSettings position');
		  // console.log(expjwPlayerSettings.getPosition());
		  // console.log(jwplayer().getPosition());
		  if(winobj !== undefined) {
			winobj.closed = true;
		  }
      var setObj;
      var updateFrom;
      if(launchedFrom == 'LP'){
          setObj = $("#learningplan-tab-inner").data("contentLaunch");
          updateFrom = 'LP';
          $('#launch-lp-wizard').parent().css('display', 'block');
          activateWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');
		   // create loaders
		   if(launchflag == 0){
			  if(document.getElementById('launch-lp-wizard')) { //for multi content launch my-programs tab
				setObj.createLoader('launch-lp-wizard');
				$('#launch-lp-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-lp-result-container')) {
				  setObj.createLoader('enroll-lp-result-container');
			  }
			}
      }
	  else if(launchedFrom == 'ME'){
		  if(document.getElementById('lnr-catalog-search'))
			  setObj = $("#lnr-catalog-search").data("contentLaunch");
		  else
          	setObj = $("#learner-enrollment-tab-inner").data("contentLaunch");
          updateFrom = 'ME';
         $('#class_detail_content #launch-wizard').show();
          $('#launch-wizard').parent().css('display', 'block');
          activateWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment');
          if(launchflag == 0){
			  if(document.getElementById('launch-wizard')) {	//for multi content launch my-enrollments tab
				setObj.createLoader('launch-wizard');
				$('#launch-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('paintEnrollmentResults')) {
				  setObj.createLoader('enroll-result-container');
			  }
		 }
      }

      var crid                  = parseInt(courseId);
      var clid                  = parseInt(classId);
      var lesid                 = parseInt(lessonId);
      var versionid             = parseInt(versionId);
      var enrollid              = parseInt(enrollId);
      var stid                  = this.getLearnerId();
      var regId                 = parseInt(enrollId);

      var status          = '';
      var xstatus         = '';
      if(stid == "0" || stid == "")
	  {
	   	 self.location='?q=learning/enrollment-search';
         return;
	  }
      //0021160: Overriding the existing content status when launch the VOD
      //if(prevContentStatus != 'Completed') {
            status = 'Completed';
            xstatus='lrn_crs_cmp_cmp';
      //}

      $('#'+lesid+'_launch_status').html(xstatus); // No _launch_status in DOM

      $('#surveybutton_'+clid).css("display","block");  // No surveybutton_ in DOM
      $('#lanchlinks1_'+clid+'').click(); // No launchlinks1_ in DOM
	  var video_suspend_data = null;
	  if(typeof videoTrackerProgress != "undefined" && videoTrackerProgress != null) {
		videoTrackerProgress.progress = Math.round((videoTrackerProgress.current_position / videoTrackerProgress.video_duration) * 100);
		videoTrackerProgress.progress = videoTrackerProgress.progress > 100 ? 100 : videoTrackerProgress.progress;
		video_suspend_data = videoTrackerProgress;
	  }
	
	   var launchflag = 0;
	  if(videoSessionId != undefined && videoSessionId == 1)
	  	launchflag = 1;
	
	  var launchData  = {
          'regid'   : regId,
          'stid'    : stid,
          'classid' : clid,
          'courseid'  : crid,
          'lessonid'  : lesid,
          'versionid' : versionid,
          'enrollid'  : enrollid,
          'prev_content_status'	: prevContentStatus,
          'video_suspend_data'	: JSON.stringify(video_suspend_data),
          'type':type,
          'h5p_cnt_id':h5p_cnt_id,
          'launchflag' : launchflag
        };

      url = resource.base_url + '/?q=ajax/update-launch/launch';
      post_data = launchData;
     if(launchflag == 0){
	      if(document.getElementById('lnr-catalog-search')){
	        if(document.getElementById("course-details-display-content"))
		    	$("#course-details-display-content").data("lnrcoursedetails").createLoader('class-list-loader');
		    if(document.getElementById('learning-plan-details-display-content'))
	            $("#learning-plan-details-display-content").data("lnrplandetails").createLoader('class-container-loader-'+classId);
	        if(document.getElementById("class_detail_content"))
		         $("#class_detail_content").data("lnrclassdetails").createLoader('class_detail_content');
		   }
	  }
      var obj = this;
      $.ajax({
        type: "POST",
        url: url,
        data:  post_data,
        success: function(result){
        	if(launchflag == 1)
        		 return;
			disposeVideoJSPlayer('all');
            for(i=0; i < result.length; i++) {
              if(result[i].ID == parseInt(lesid)) {
                $("#" + lesid + "_" + regId + "attempts_left").html(result[i].contValidateMsg);
              }
            }
			// destroy loaders
			if(launchedFrom == 'LP'){
			  if(document.getElementById('launch-lp-wizard')) { //for multi content launch my-programs tab
				setObj.destroyLoader('launch-lp-wizard');
				$('#launch-lp-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('paintEnrollmentLPResults')) {
				  setObj.destroyLoader('paintEnrollmentLPResults');
			  }
			}
			else if(launchedFrom == 'ME'){
			  if(document.getElementById('launch-wizard')) {	//for multi content launch my-enrollments tab
				setObj.destroyLoader('launch-wizard');
				$('#launch-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-result-container')) {
				  setObj.destroyLoader('enroll-result-container');
			  }
			}
            if(document.getElementById('lnr-catalog-search')) {
            	if(document.getElementById('launch-wizard')){
            		$('#launch-wizard').css("overflow","hidden");
            		obj.createLoader('launch-wizard');
            	}
        		$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
        		url = obj.constructUrl("ajax/learningcore/register-to-launch/" + stid + '/' + clid + '/' + crid+'/'+regId+'/lrn_cls_dty_vod/1/'+lesid);
				$.ajax({
					type: "POST",
					url: url,
					//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
					success: function(result){
						if(result){
							result_arr = result.split('*~*');
							var launched_info = $.parseJSON(result_arr[1]);
							var lessonCnt = result_arr[2];
							$("#lnr-catalog-search").data('enrollment').updateMultiContentLaunchDialog(regId,lesid,launched_info.triggering_lesson_details,launched_info.triggering_content_quiz_status);
							var html_out = '<div '+result_arr[0]+'</div>';
							if(document.getElementById('launch'+regId)){
								$('#launch'+regId).replaceWith(html_out);
								Drupal.attachBehaviors($('#launch'+regId));
							}
							if(document.getElementById('registerCls_'+clid)){
								$('#registerCls_'+clid).replaceWith(html_out);
								Drupal.attachBehaviors($('#registerCls_'+clid));
							}
							paintContentStatus(launched_info, crid, clid, '',lessonCnt,regId);
						}
						if(document.getElementById('launch-content-container'))
							$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
						if(document.getElementById("tbl-paintCatalogContentResults")){
							if(document.getElementById('launch'+regId)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
							}else if(document.getElementById('registerCls_'+clid)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
							}
		        			//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
							$("#tbl-paintCatalogContentResults").trigger("reloadGrid");
		        		}
						if(document.getElementById('paintContentResults')){
							if(document.getElementById('launch'+regId)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
							}else if(document.getElementById('registerCls_'+clid)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
							}
							//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
							if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
								$("#paintContentResults").trigger("reloadGrid");
							}
						}
						$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
						if(document.getElementById('launch-wizard'))
							obj.destroyLoader('launch-wizard');
							$('#class_detail_content #launch-wizard').hide();
							if(document.getElementById("course-details-display-content"))
					    	  $("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
					    	if(document.getElementById('learning-plan-details-display-content'))
            				  $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+clid);
            				if(document.getElementById("class_detail_content"))
	                  		  $("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
					        resetFadeOutByClass('#launch-wizard','item-title-code','line-item-container','mylearning');
					}
			    });
            }
            if(document.getElementById('learner-enrollment-tab-inner')) {
            	if (document.getElementById('launch-content-container')) {
				    //console.log('reloading grid with lesson id and enrollment id');
					  $("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId}});
					  if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
					  	$("#paintEnrollmentResults").trigger("reloadGrid");
					  }
				  }
				  else {
				    //console.log('reloading grid without lesson id and enrollment id as no launch-content-container');
				    if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
				    	$("#paintEnrollmentResults").trigger("reloadGrid");
				    }
				  }
            }
            if(document.getElementById('learningplan-tab-inner')) {
            	if (document.getElementById('lplaunch-content-container')) {
				    $("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId}});
					if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
						$("#paintEnrollmentLPResults").trigger("reloadGrid");
					}
				}
				else {
					if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
						$("#paintEnrollmentLPResults").trigger("reloadGrid");
					}
				}
            }

            if(xstatus == "lrn_crs_cmp_cmp" && document.getElementById("my_transcript_jqgrid")) {
              $("#my_transcript_jqgrid").trigger('reloadGrid');
            }
			for (var line_item in result) {
				if (line_item == 0) {
					if (result[line_item].CompMessage == 'Attempts Over') {
						callLaunchMessageWindow(Drupal.t('LBL929'), Drupal.t('MSG915'));
					}
				}
			}
        }
        });
	  }catch(e){
			// to do
		  // console.log(e);
	  }
    }
});

$.extend($.ui.contentLaunch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);


var obj;

$(function() {
	try{
	if(document.getElementById('learner-enrollment-tab-inner')) {
		$("#learner-enrollment-tab-inner" ).contentLaunch();
	}
	if(document.getElementById('learningplan-tab-inner')) {
		$("#learningplan-tab-inner" ).contentLaunch();
	}
	if(document.getElementById('lnr-catalog-search')) {
		$("#lnr-catalog-search" ).contentLaunch();
	}

	}catch(e){
		// to do
	}
});

function callLaunchMessageWindow(title,message){
		 try{
			if(title == 'registertitle'){
				title = Drupal.t('LBL721');
			}
			
		    $('#commonMsg-wizard').remove();
		    var html = '';
		    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		    html+='<tr><td height="30"></td></tr>';
		    html+='<tr>';
	    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(message)+'</span></td>';
		    html+='</tr>';
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    var closeButt={};
	    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
		    $("#commonMsg-wizard").dialog({
		        position:[(getWindowWidth()-500)/2,100],
		        bgiframe: true,
		        width:520,
		        resizable:false,
		        modal: true,
		        title: SMARTPORTAL.t(title),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    this.currTheme = Drupal.settings.ajaxPageState.theme;
			  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
				}
		    //new dialog popUI Script
			 // $('#select-class-dialog').hide();  
		 	if(this.currTheme == "expertusoneV2"){
		     //changeDialogPopUI();
		 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-equalence-dialog');	
			   //$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');	
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
		    $("#commonMsg-wizard").show();	
			$('.ui-dialog-titlebar-close,.removebutton').click(function(){
				$("#commonMsg-wizard").remove();
		        //$('#select-class-dialog').show();
		        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
		    });
		 }catch(e){
				// to do
			}
		}

;
var API_1484_11;
var API;
var winobj;
var _isFinishCalled;
var KC_API = new Object();
var isLaunched = false;
var refresh_timer;
var timeOutVar;
SCORM_API_WRAPPER.prototype._version = null;
SCORM_API_WRAPPER.prototype._type = null;
SCORM_API_WRAPPER.prototype._callback = null;
SCORM_API_WRAPPER.prototype.callback_param = null;
SCORM_API_WRAPPER.prototype.winName = '';

function SCORM_API_WRAPPER(data){
	try{
    this._version = data.version;
    this._type = data.type;
	}catch(e){
		// to do
	}
};

SCORM_API_WRAPPER.prototype.Initialize = function(data){
	try{
		isLaunched = true;
		var dateStr =  new Date().getTime();
		//this.winName = this._type+'_'+ dateStr;
		this.winName = this._type.replace(/\ /g, "_")+'_'+ dateStr;
    if(this._type == "SCORM"){
        this.SCORMInit(data);
    }else if(this._type == "Knowledge Content"){
        this.LaunchKnowledgeContent(data);
    }else if (this._type == "Assessment") { // Code for My Know How integration
    	this.LaunchAssessment(data);
    }else if (this._type == "AICC" || this._type=="AICC Course Structure"){ // AICC integration
    	this.LaunchAICC(data);
    }else if(this._type == "Tin Can"){
    	this.LaunchTincan(data);
    }
	}catch(e){
		// to do
	}
    
};

SCORM_API_WRAPPER.prototype.SCORMInit = function(data){
	try{
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
    switch(this._version){
        case "1.2" :
            API  = new SCORM_API_12();
            API.setCallBack(data.callback);
            if(data.contentQuizStatus == null || data.contentQuizStatus == ""){ data.contentQuizStatus='not attempted'}
            API.resetValue(data.contentQuizStatus);
            API.LMSInitialize("",'LMS');
            API.LMSSetValue("cmi.core.student_id",data.learnerId);
            API.LMSSetValue("cmi.core.student_name",data.learnerName);
            //possible values: browse,normal,resumes
            API.LMSSetValue("cmi.core.lesson_mode","normal");
            //possible values:passed,completed,failed,incomplete,browsed,not attempted
            API.LMSSetValue("cmi.core.lesson_status",data.completionStatus);
            API.LMSSetValue("cmi.core.lesson_time","0000:00:00.00");
            API.LMSSetValue("cmi.core.session_time","0000:00:00.00");
            //possible values: ab-initio,"",resume
            API.LMSSetValue("cmi.core.entry","ab-initio");
            API.LMSSetValue("cmi.core.score.max",data.scoreMax);
            API.LMSSetValue("cmi.core.score.min",data.scoreMin);
            API.LMSSetValue("cmi.core.score.raw",data.score);
            API.LMSSetValue("cmi.student_data.mastery_score",data.MasteryScore);
            API.LMSSetValue("cmi.core.total_time","0000:00:00.00");
            API.LMSSetValue("cmi.core.lesson_location",data.location);
            //"cmi.launch_data" contains any information that is to be provided to a SCO upon the start of any session of that SCO including a learner�s initial visit.
            //This value is initialized by the LMS to be equal to the information in
            //the <adlcp:datafromlms> element in the manifest for the particular SCO.
            API.LMSSetValue("cmi.launch_data",data.launch_data);
            API.LMSSetValue("cmi.suspend_data",data.suspend_data);
            API.LMSSetValue("cmi.core.credit","credit");
            API.LMSSetValue("cmi.core.exit",data.exit);
            break;
        case "2004":
            API_1484_11  = new SCORM_API_2004();
            API_1484_11.setCallBack(data.callback);

            API_1484_11.Initialize("");
            API_1484_11.SetValue("cmi.learner_id",data.learnerId);
            API_1484_11.SetValue("cmi.learner_name",data.learnerName);
            //possible values: browse,normal,resumes
            API_1484_11.SetValue("cmi.mode","normal");
            //possible values:passed,completed,failed,incomplete,browsed,not attempted
            API_1484_11.SetValue("cmi.success_status",data.successStatus);
            API_1484_11.SetValue("cmi.completion_status",data.completionStatus);
            API_1484_11.SetValue("cmi.session_time","0000:00:00.00");
            //possible values: ab-initio,"",resume
            API_1484_11.SetValue("cmi.entry","ab-initio");
            API_1484_11.SetValue("cmi.max",data.scoreMax);
            API_1484_11.SetValue("cmi.min",data.scoreMin);
            API_1484_11.SetValue("cmi.score.raw",data.score);
            API_1484_11.SetValue("cmi.total_time","0000:00:00.00");
            API_1484_11.SetValue("cmi.location",data.location);
            //"cmi.launch_data" contains any information that is to be provided to a SCO upon the start of any session of that SCO including a learner�s initial visit.
            //This value is initialized by the LMS to be equal to the information in
            //the <adlcp:datafromlms> element in the manifest for the particular SCO.
            API_1484_11.SetValue("cmi.launch_data",data.launch_data);
            API_1484_11.SetValue("cmi.suspend_data",data.suspend_data);
            API_1484_11.SetValue("cmi.credit","credit");
            API_1484_11.SetValue("cmi.exit",data.exit);
            break;
    }
	
    winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
    setTimeout("SCORM_API_WRAPPER.prototype.ScormContentWindowCheck()",1000);
    SCORM_API_WRAPPER.prototype.invokeLMSCommit(this._version);
    _isFinishCalled=false
	}catch(e){
		// to do
	}
};


SCORM_API_WRAPPER.prototype.ScormContentWindowCheck=function(){
	try{
    if(!winobj.closed)
        setTimeout("SCORM_API_WRAPPER.prototype.ScormContentWindowCheck()",1000);
    else{
		isLaunched = false;
    	if(_isFinishCalled==false){
    		if(API != undefined)
    			API.LMSFinish('');
    		else
    			API_1484_11.Terminate('');
    	}
    }
	}catch(e){
		// to do
	}
};

SCORM_API_WRAPPER.prototype.Finish = function(){
	try{
	clearTimeout(timeOutVar);
    var result = {};
    _isFinishCalled=true;
    if(this._type=="SCORM"){
        switch(this._version){
            case "1.2" :
                result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
                result.status = API.LMSGetValue("cmi.core.lesson_status");
                result.score = API.LMSGetValue("cmi.core.score.raw");
                result.location = API.LMSGetValue("cmi.core.lesson_location");
                result.totalTime = API.LMSGetValue("cmi.core.total_time");
                result.sessionTime = API.LMSGetValue("cmi.core.session_time");
                result.launch_data = API.LMSGetValue("cmi.launch_data");
                result.suspend_data = API.LMSGetValue("cmi.suspend_data");
                result.exit = API.LMSGetValue("cmi.core.exit");
                break;
            case "2004" :
            	//SCORM 2004 will have 2 statuses - both should be validated 
                result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
                result.status = API_1484_11.GetValue("cmi.success_status");					//quiz status
                result.score = API_1484_11.GetValue("cmi.score.raw");
                result.location = API_1484_11.GetValue("cmi.location");
                result.totalTime = API_1484_11.GetValue("cmi.total_time");
                result.sessionTime = API_1484_11.GetValue("cmi.session_time");
                result.launch_data = API_1484_11.GetValue("cmi.launch_data");
                result.suspend_data = API_1484_11.GetValue("cmi.suspend_data");
                result.exit = API_1484_11.GetValue("cmi.exit");
                break;
        }
    }else if(this._type == "Knowledge Content"){
		var timeSpend = SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime - KC_API.startTime);
		result.completionStatus	= "Completed";
        result.status			= "Completed";
        result.score			= "0";
        result.location			= "0";
        result.totalTime		= "0";
        result.sessionTime		= timeSpend;
        result.launchflag 		= 0;
    }else if(this._type == "AICC Course Structure" || this._type=="AICC"){ 
    	result.completionStatus="";
        result.status="";
        result.score="";
        result.location="";
        result.totalTime="";
        result.sessionTime="";
        result.launchflag = 0;
    }else if(this._type == "Tin Can"){ 
    	result.completionStatus="";
        result.status="";
        result.score="";
        result.location="";
        result.totalTime="";
        result.sessionTime="";
        result.launchflag = 0;
    }else if(this._type == "Assessment"){ // Code start for My Know How integration
    	// Do nothing.
    }
    return result;
	}catch(e){
		// to do
	}
};

/**
 * function used to initiate the LMSCommit at every interval of 5 mins
 * content lessons. 
 * @return
 */
SCORM_API_WRAPPER.prototype.invokeLMSCommit = function(version){
	try{
		var result = {};
		if(!winobj.closed){
			switch(version){
			case "1.2" :
				result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
                result.status = API.LMSGetValue("cmi.core.lesson_status");
                result.score = API.LMSGetValue("cmi.core.score.raw");
                result.location = API.LMSGetValue("cmi.core.lesson_location");
                result.totalTime = API.LMSGetValue("cmi.core.total_time");
                result.sessionTime = API.LMSGetValue("cmi.core.session_time");
                result.launch_data = API.LMSGetValue("cmi.launch_data");
                result.suspend_data = API.LMSGetValue("cmi.suspend_data");
                result.exit = API.LMSGetValue("cmi.core.exit");
                result.launchflag = 1;
                break;
			case "2004" :
				result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
                result.status = API_1484_11.GetValue("cmi.success_status");					//quiz status
                result.score = API_1484_11.GetValue("cmi.score.raw");
                result.location = API_1484_11.GetValue("cmi.location");
                result.totalTime = API_1484_11.GetValue("cmi.total_time");
                result.sessionTime = API_1484_11.GetValue("cmi.session_time");
                result.launch_data = API_1484_11.GetValue("cmi.launch_data");
                result.suspend_data = API_1484_11.GetValue("cmi.suspend_data");
                result.exit = API_1484_11.GetValue("cmi.exit");
                result.launchflag = 1;
                break;
			}
			if(version == "1.2"){
				API._callback(result);				
			}else {
				API_1484_11._callback(result);			
			}
			//setTimeout(SCORM_API_WRAPPER.prototype.invokeLMSCommit,(Drupal.settings.content), version);  // request for once in certain interval configured in exp_sp.ini
			/* refresh_timer = setTimeout(function(){
				SCORM_API_WRAPPER.prototype.invokeLMSCommit(version);
			},Drupal.settings.content); */
			timeOutVar = setTimeout(function(){
				SCORM_API_WRAPPER.prototype.invokeLMSCommit(version);
			},Drupal.settings.content);
	  }
	}catch(e){
		
	}
}

SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent = function(data){
  try{
  	var obj = this;
	obj._type = 'Knowledge Content';
  	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
	SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback = data.callback;
	SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback = data.callback;
	if(data.url != null && data.url.indexOf("?q=h5p/embed") >= 0)
    {
    	var left = (screen.width/2)-(900/2);
  		var top = (screen.height/2)-(520/2); 
  		var newurl = data.url.split("/");
		//to support tincan change url
  		//data.url = "?q=node/"+newurl[2]+"/view";
  		
  		winobj = window.open(data.url+"&enrollId="+data.enrollid,this.winName,"location=1,status=0,resizable =1,scrollbars=1,width=900,height=520,top="+top+",left="+left);
   		//API = window.open(data.url,"kcwindow","location=1,status=1,resizable =1,scrollbars=1,width=900,height=520,top="+top+",left="+left);
    }
   else
    {
   winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
    }
   KC_API.startTime		= new Date().getTime();
   setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
   SCORM_API_WRAPPER.prototype.refreshSession(obj);
  }catch(e){
	  // to do
  }
};

SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck=function(){
	try{
    if(!winobj.closed){
        setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
    }else{
		isLaunched = false;
		KC_API.endTime		= new Date().getTime();
        SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback();
    }
	}catch(e){
		// to do
	}
};

//Code start for My Know How integration

SCORM_API_WRAPPER.prototype.LaunchAssessment = function(data){
	try{
	   //alert("In SCORM_API_WRAPPER.prototype.LaunchAssessment()");
	   SCORM_API_WRAPPER.prototype.LaunchAssessment._callback = data.callback;
	   winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
	   setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
	   SCORM_API_WRAPPER.prototype.refreshSession();
	}catch(e){
		// to do
	} 
	};

SCORM_API_WRAPPER.prototype.AssessmentWindowCheck=function(){
	try{
	//alert("In SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()");
    if(!winobj.closed){
        setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
    }else{
		isLaunched = false;
        SCORM_API_WRAPPER.prototype.LaunchAssessment._callback();
    }
	}catch(e){
		// to do
	}
};
// Code end for My Know How integration

//AICC integration
SCORM_API_WRAPPER.prototype.LaunchAICC=function(data){
	try{
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}  
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(resource.base_url+"/sites/all/commonlib/AICC_Handler.php?CMI=HACP");
	SCORM_API_WRAPPER.prototype.LaunchAICC._callback = data.callback;
	var obj1 = this;
	var obj = SCORM_API_WRAPPER.prototype;
	try{	
		var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
		
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSION", "aicc_id": sid },
				success: function(result){
					if(data.url.indexOf("?") != -1){
						qrystr = "&aicc_sid="+result.session_id+"&Aicc_url="+callBackURL;
					}else{
						qrystr = "?aicc_sid="+result.session_id+"&Aicc_url="+callBackURL;
					}
					obj.callback_param = result.session_id;
					var pmPath = data.url + qrystr ;
					winobj = window.open(pmPath,this.winName,"location=1,status=1,scrollbars=1,resizable=yes,width=900,height=900");
					//winobj.document.write('<iframe width="100%" height="100%" src="'+pmPath+'" frameborder="0" allowfullscreen></iframe>'); //allowfullscreen
				    setTimeout("SCORM_API_WRAPPER.prototype.AICCWindowCheck()",1000);
				    SCORM_API_WRAPPER.prototype.refreshSession(obj1);
				}
	    });
	  }catch(e){
		  // to do
	  }
	}catch(e){
		// to do
	}
};
/**
 * Tincan integeration.
 */
SCORM_API_WRAPPER.prototype.LaunchTincan=function(data){
	try{
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(resource.base_url+"/sites/all/commonlib/TinCan_Handler.php/");
	SCORM_API_WRAPPER.prototype.LaunchTincan._callback = data.callback;
	//alert('call back -->> '+ data.toSource())
	try{	
		var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
		var obj = SCORM_API_WRAPPER.prototype;
		var obj1 = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSION", "content_token": sid },
				success: function(result){
				//alert(result.toSource());
					var uuid = guid();
					if(data.url.indexOf("?") != -1){
						//alert('if-');
						qrystr = "&endpoint="+callBackURL+"&auth="+result.session_id+'&actor={"name":"'+result.name+'","mbox":"'+result.email+'","objectType":"agent"}&activity_id='+sid;
					}else{
						//alert('else-');
						qrystr = "?endpoint="+callBackURL+"&auth="+result.session_id+'&actor={"name":"'+result.name+'","mbox":"'+result.email+'","objectType":"agent"}&activity_id='+sid;
					}
					obj.callback_param = result.session_id;
					var pmPath = data.url + qrystr ;
					//alert(pmPath);
					winobj = window.open(pmPath,this.winName,"location=1,status=1,scrollbars=1,resizable=yes,width=900,height=900");
					//winobj.document.write('<iframe width="100%" height="100%" src="'+pmPath+'" frameborder="0" allowfullscreen></iframe>'); //allowfullscreen
				    setTimeout("SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck()",1000);
				    SCORM_API_WRAPPER.prototype.refreshSession(obj1);
				}
	    });
	  }catch(e){
		  // to do
		  //console.log(e);
	  }
	}catch(e){
		// to do
		//console.log(e);
	}
};
SCORM_API_WRAPPER.prototype.AICCWindowCheck=function(){
	try{
		if(!winobj.closed){
			setTimeout("SCORM_API_WRAPPER.prototype.AICCWindowCheck()",1000);
		}else{
			isLaunched = false;
	    	SCORM_API_WRAPPER.prototype.LaunchAICC._callback(this.callback_param);
		}
	}catch(e){
		// to do
	}
}

SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck=function(){
	try{
		if(!winobj.closed){
			//alert('fgre');
			setTimeout("SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck()",1000);
		}else{
			isLaunched = false;
			//alert('window check');
	    	SCORM_API_WRAPPER.prototype.LaunchTincan._callback(this.callback_param);
		}
	}catch(e){
		// to do
	}
}

/**
 * function used to keep the session alive when the learner reading the 
 * content lessons. 
 * @return
 */
SCORM_API_WRAPPER.prototype.refreshSession = function(launchObj){
	try{
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		var obj = this;
		var contentType = launchObj._type.toLowerCase();
		var url ='';
		var params = {};
		switch(contentType) {
			case "aicc":
			case "aicc course structure":
				//console.log('aicc block');
				params = {
					'command' : 'UPDATELMSDATA',
					'session_id' : launchObj.callback_param,
					'launchflag' : 1
				}
				url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
				SCORM_API_WRAPPER.prototype.updateIdleCallBack(launchObj, url, params);
				break;
			case 'tin can':
				params = {
					'command' : 'UPDATELMSDATA',
					'Authorization' : launchObj.callback_param,
					'launchflag' : 1
				}
				url = resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";
				SCORM_API_WRAPPER.prototype.updateIdleCallBack(launchObj, url, params);
				//console.log('tincan block');
				break;
			/* case 'knowledge content':
				var result = {};
				KC_API.endTime		= new Date().getTime();
				var timeSpend = SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime - KC_API.startTime);
				result.completionStatus	= "Completed";
		        result.status			= "Completed";
		        result.score			= "0";
		        result.location			= "0";
		        result.totalTime		= "0";
		        result.sessionTime		= timeSpend;
		        result.launchflag 		= 1;
		        SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback(result);
				break */
			default:
				//console.log('default block');
				break;
		}
		timeOutVar = setTimeout(function(){SCORM_API_WRAPPER.prototype.refreshSession(launchObj);},Drupal.settings.content);
		/* if(!winobj.closed){
			var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/get/0");
			$.ajax({
	    		   type: "POST",
		           url: url,
		           cache: false,
		           success: function(result){}
		        });
			setTimeout("SCORM_API_WRAPPER.prototype.refreshSession()",(1000*60*15));  // request for once in 15 min
		} */
	}catch(e){
		// to do
	}	
}


//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
SCORM_API_WRAPPER.prototype.updateIdleCallBack = function(launchObj, url, params) {
	try {
		//console.log('params', params);
		$.ajax({
			type: "POST",
			url: url,
			data:  params,
			success: function(result){
				/* var contentPlayerObj;
				if(document.getElementById('learningplan-tab-inner')) {
					if($("#learningplan-tab-inner").data("contentLaunch").updateFrom == 'LP'){
						contentPlayerObj = $("#learningplan-tab-inner").data("contentPlayer");
					}
				}
				if(document.getElementById('learner-enrollment-tab-inner')) {
					if($("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom == 'ME'){
						contentPlayerObj = $("#learner-enrollment-tab-inner").data("contentPlayer");
					}
				}
				if(document.getElementById('lnr-catalog-search')){
					contentPlayerObj = $("#lnr-catalog-search").data("contentPlayer");
			    }
				contentPlayerObj.refreshContentProgressBarLine(contentPlayerObj,result); */
			}
		});
	} catch(e){
		//console.log(e);
	}
}


/**
 * function used to convert milliseconds to SCORM session_time format
 * @return
 */
SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat = function(intTotalMilliseconds){
	var intHours;
	var intMinutes;
	var intSeconds;
	var intMilliseconds;
	var intHundredths;
	var strCMITimeSpan;
	
	//extract time parts
	intMilliseconds = intTotalMilliseconds % 1000;

	intSeconds = ((intTotalMilliseconds - intMilliseconds) / 1000) % 60;

	intMinutes = ((intTotalMilliseconds - intMilliseconds - (intSeconds * 1000)) / 60000) % 60;

	intHours = (intTotalMilliseconds - intMilliseconds - (intSeconds * 1000) - (intMinutes * 60000)) / 3600000;
	
	//put in padding 0's and concatinate to get the proper format
	strCMITimeSpan = SCORM_API_WRAPPER.prototype.ZeroPad(intHours, 2) + ":" + SCORM_API_WRAPPER.prototype.ZeroPad(intMinutes, 2) + ":" + SCORM_API_WRAPPER.prototype.ZeroPad(intSeconds, 2);
	
	return strCMITimeSpan;

}

SCORM_API_WRAPPER.prototype.ZeroPad = function(intNum, intNumDigits){

	var strTemp;
	var intLen;
	var i;
	
	strTemp = new String(intNum);
	intLen = strTemp.length;
	
	if (intLen > intNumDigits){
		strTemp = strTemp.substr(0,intNumDigits);
	}
	else{
		for (i=intLen; i<intNumDigits; i++){
			strTemp = "0" + strTemp;
		}
	}
	
	return strTemp;
}

function guid() {
	  function s4() {
	    return Math.floor((1 + Math.random()) * 0x10000)
	      .toString(16)
	      .substring(1);
	  }
	  return s4() + s4() + '-' + s4() + '-' + s4() + '-' +
	    s4() + '-' + s4() + s4() + s4();
};
/*
 *
 * between LMS & SCO.
 * 
 */
   //LMS SCORM Adaptor API - Starts
    function SCORM_API_12() 
    {
         try{
        	 
         }catch(e){
 			// to do
         }
    }
    SCORM_API_12.prototype._version = "SCORM 1.2 LMSAPI1.0.0";
    SCORM_API_12.prototype._desc = "SCORM 1.2 LMS API Adaptor Version 1.0.0";
    SCORM_API_12.prototype._callback = "";
    SCORM_API_12.prototype._resetvalue = "";
    SCORM_API_12.prototype._isLMSInitialized = "false";
    SCORM_API_12.prototype._cmiBooleanFalse = "";
    SCORM_API_12.prototype._cmiBooleanTrue = "" ;
    //current error code
    SCORM_API_12.prototype._errorCode = 0;
    SCORM_API_12.prototype._valueStr = "";
    SCORM_API_12.prototype._errStr = "";
    SCORM_API_12.prototype._CMIDATA = new Array();
    SCORM_API_12.prototype._ErrorCodes = {"201":"Invalid argument error",
                                          "202":"Element cannot have children","203":"Element not an array. Cannot have count.",
                                          "301":"Not initialized","401":"Not implemented error",
                                          "402":"Invalid set value, element is a keyword","403":"Element is read only.",
                                          "404":"Element is write only","405":"Element is write only",
                                           "0":"No Error.The previous LMS API Function call completed successfully.",
                                           "101":"General Exception.An unspecified, unexpected exception has occured"
                                         };
    

    /*
    * Method: LMSInitialize(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * This function must be called by a SCO before any other API calls are made.
    * It can not be called more than once  consecutively unless LMSFinish is called.
    */
    SCORM_API_12.prototype.LMSInitialize = function(param,callFrom) 
    {
    	try{
        debugLog('inside actual LMSInitialize function');
        result = this._cmiBooleanFalse;  // assume failure
        // Make sure param is empty string "" - as per the API spec
        if  (param !="")  
        {
              this._errorCode = 201;
              this._isLMSInitialized = "false";
        }
        this._isLMSInitialized = "true";
        var rtn = this._isLMSInitialized;
        if(rtn == 'true' && callFrom != 'LMS' && callFrom !=undefined){
         this.LMSSetValue("cmi.core.lesson_status",this._resetvalue);
         };
        return rtn;
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSFinish(String param) 
    * Input: none Output: none Description: Signals completion of communication with LMS
    * Now call a javascript function that should be present in the window this javascript is located in
    * that will change the SCO content frame.
    */
    SCORM_API_12.prototype.LMSFinish = function(param) 
    {
    	try{
        debugLog('inside actual LMSFinish function');
        if (param =="")
        {
            result = this.LMSCommit("");
            //alert(2);
            if (!result)
            {
                //log into debug console
                debugLog("Error Occured in LMSFinish while calling LMSCommit; ErrorCode:" + this._errorCode);
                this._errorCode = 201;
            
                return "false";
            }
            else
            {
                debugLog('inside LMSCommit is succeeded');
                this._isLMSInitialized = "false";
                result = this._cmiBooleanTrue;  // successful completion
                //now call the callback defined by the Caller.;
                //eval(this._callback+'()');
            
                this._callback();
                //return true;
            
                
            }
        }
        else
        {
            
                this._errorCode = 201;
                debugLog("Error Occured in LMSFinish. ErrorCode:" + this._errorCode);
                return "false";
        }   

        return "true";  
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSCommit(String param) 
    * Input: none Output: none Description: Applies the SCO data model elements set using LMSSetValue to LMS data model 
    * elements and saves them (on the server). Call function to save the data in your SmartPortal LMS.
    */
    SCORM_API_12.prototype.LMSCommit = function(param) 
    {
    	try{
            //Invoke LMS Web Service to save the learner's score and other details in database
            debugLog('inside LMSCommit');
            return "true";
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.LMSGetValue = function(param) 
    {
    	try{
            var value = this._CMIDATA[param];
            if (value==null)
            {
                value = " ";
            }
            //return this._CMIDATA[param];
            return value;
    	}catch(e){
			// to do
    	}
    };

    SCORM_API_12.prototype.LMSSetValue = function(param,value) 
    {
    	try{
    
           debugLog("==> param:" + param + " value:" + value);
           this._CMIDATA[param] = value;
           if(param!=null && param!=undefined && param!=''
               && value!=null && value!=undefined && value!='')
               return "true";
           else
               return "false";
           
           //alert('array length:' + this._CMIDATA.length);
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_12.prototype.LMSGetLastError = function() 
    {
    	try{
         return this._errorCode;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.LMSGetLastErrorString = function() 
    {
    	try{
        var errstr = this._ErrorCodes[this._errorCode];
        if (errstr==null)
        {
            errstr = "Undefined Error Code";
        }
         return errstr;
    	}catch(e){
			// to do
    	}
    };
    /*
    * Method: LMSGetDiagnostic(String param) 
    * Input: none 
    * Output: Any additional diagonistic information provided by LMS Vendor.  
    */
    SCORM_API_12.prototype.LMSGetDiagnostic = function() 
    {
    	try{
         return " ";
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.getAllCMIData = function() 
    {
    	try{
         return this._CMIDATA;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.setCallBack = function (callbackfn)
    {
    	try{
        this._callback = callbackfn;
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_12.prototype.resetValue = function (value)
    {
    	try{
        this._resetvalue = value;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_12.prototype.LMSGetErrorString = function(isErrorNumber) 
    {
    	try{
        if(isErrorNumber == null || isErrorNumber == undefined || isErrorNumber == "" || isErrorNumber == "null")
          isErrorNumber = this._errorCode;
          
        var errstr = this._ErrorCodes[isErrorNumber];
        if (errstr==null)
        {
            errstr = "Undefined Error Code";
        }
         return errstr;
    	}catch(e){
			// to do
    	}
    };
    //LMS SCORM Adaptor API - Ends
    var findAPITries = 0;
    function findAPI(win)
    {
    	try{
        // Check to see if the window (win) contains the API
        // if the window (win) does not contain the API and
        // the window (win) has a parent window and the parent window
        // is not the same as the window (win)
        debugLog("inside findAPI");
        while ( (win.API == null) && (win.parent != null) && (win.parent != win) )
        {
               // increment the number of findAPITries
               findAPITries++;
            
               // Note: 7 is an arbitrary number, but should be more than sufficient
               if (findAPITries > 7)
               {
                  debugLog("Error finding API -- too deeply nested.");
                  return null;
               }
            
               // set the variable that represents the window being
               // being searched to be the parent of the current window
               // then search for the API again
               win = win.parent;
        }
        return win.API;
    	}catch(e){
			// to do
    	}
    };

    function getAPI()
    {
    	try{
           debugLog("inside getAPI");
           // start by looking for the API in the current window
           var theAPI = findAPI(window);
        
           // if the API is null (could not be found in the current window)
           // and the current window has an opener window
           if ( (theAPI == null) && (window.opener != null) && (typeof(window.opener) != "undefined") )
           {
              // try to find the API in the current window’s opener
              theAPI = findAPI(window.opener);
           }
           // if the API has not been found
           if (theAPI == null)
           {
              // Alert the user that the API Adapter could not be found
              debugLog("Unable to find an API adapter");
           }
           return theAPI;
    	}catch(e){
			// to do
    	}
    }
    function debugLog(msgstr)
    {
    	try{
        var debugConsoleId = document.getElementById("debugConsole");
        var timestamp = new Date().toString();
        if (debugConsoleId != null)
        {
            var logmessage = document.getElementById("debugConsole").innerHTML;
            document.getElementById("debugConsole").innerHTML = logmessage + "<br>" + timestamp + " " + msgstr ;
        }
    	}catch(e){
			// to do
    	}
    }
;
/*
 *
 * between LMS & SCO.
 * 
 */
 
   //LMS SCORM Adaptor API - Starts
    function SCORM_API_2004() 
    {
    	try{
    		
    	}catch(e){
			// to do
    	}
         
    }
    
    SCORM_API_2004.prototype._version = "SCORM 2004 LMSAPI1.0.0";
    SCORM_API_2004.prototype._desc = "SCORM 2004 LMS API Adaptor Version 1.0.0";
    SCORM_API_2004.prototype._callback = "";
    SCORM_API_2004.prototype._isInitialized = "false";
    SCORM_API_2004.prototype._cmiBooleanFalse = "";
    SCORM_API_2004.prototype._cmiBooleanTrue = "" ;
    //current error code
    SCORM_API_2004.prototype._errorCode = 0;
    SCORM_API_2004.prototype._valueStr = "";
    SCORM_API_2004.prototype._errStr = "";
    SCORM_API_2004.prototype._CMIDATA = new Array();
    SCORM_API_2004.prototype._ErrorCodes = {"201":"Invalid argument error",
                                          "202":"Element cannot have children","203":"Element not an array. Cannot have count.",
                                          "301":"Not initialized","401":"Not implemented error",
                                          "402":"Invalid set value, element is a keyword","403":"Element is read only.",
                                          "404":"Element is write only","405":"Element is write only",
                                           "0":"No Error.The previous LMS API Function call completed successfully.",
                                           "101":"General Exception.An unspecified, unexpected exception has occured"
                                         };
    

    /*
    * Method: Initialize(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * This function must be called by a SCO before any other API calls are made.
    * It can not be called more than once  consecutively unless Finish is called.
    */
    SCORM_API_2004.prototype.Initialize = function(param) 
    {
    	try{
        debugLog('inside actual Initialize function');
        result = this._cmiBooleanFalse;  // assume failure
        // Make sure param is empty string "" - as per the API spec
        if  (param !="")  
        {
              this._errorCode = 201;
              this._isInitialized = "false";
        }else{
            this._isInitialized = "true";
        }
        return this._isInitialized;
    	}catch(e){
			// to do
    	}
    };
    
    /*
    * Method: Finish(String param) 
    * Input: String param - must be null string - reserved for future use 
    * Output: CMIBoolean  "false" if fails, "true" if succeeds
    * Description: Signals completion of communication with LMS
    * Now call a javascript function that should be present in the window this javascript is located in
    * that will change the SCO content frame.
    */
    SCORM_API_2004.prototype.Terminate = function(param){
    	try{
        debugLog('inside actual Terminate function');
        if (param =="")
        {
            result = this.Commit("");
            //alert(2);
            if (!result)
            {
                //log into debug console
                debugLog("Error Occured in Terminate while calling Commit; ErrorCode:" + this._errorCode);
                this._errorCode = 201;
            
                return "false";
            }
            else
            {
                debugLog('inside Commit is succeeded');
                this._isInitialized = "false";
                result = this._cmiBooleanTrue;  // successful completion
                //now call the callback defined by the Caller.;
                //eval(this._callback+'()');
                this._callback();
                //return true;
            
                
            }
        }
        else
        {
            
                this._errorCode = 201;
                debugLog("Error Occured in LMSFinish. ErrorCode:" + this._errorCode);
                return "false";
        }   

        return "true";    
    	}catch(e){
			// to do
    	}
    };
    
    /*
    * Method: Commit(String param) 
    * Input: none Output: none Description: Applies the SCO data model elements set using LMSSetValue to LMS data model 
    * elements and saves them (on the server). Call function to save the data in your SmartPortal LMS.
    */
    SCORM_API_2004.prototype.Commit = function(param) 
    {
    	try{
            //Invoke LMS Web Service to save the learner's score and other details in database
            debugLog('inside LMSCommit');
            if  (param !="")  
            {
                  this._errorCode = 201;
                  this._isInitialized = "false";
            }else{
                this._errorCode = 0;
                this._isInitialized = "true";
            }
            return this._isInitialized;
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_2004.prototype.GetValue = function(param) 
    {
     try{	
        if(param!=""){  
            var value = this._CMIDATA[param];
                if (value==null)
                {
                    value = " ";
                }
                //return this._CMIDATA[param];
        }else{
            this._errorCode = 301;
            value = ""; 
        }
        return value;
     }catch(e){
			// to do
 	 }
    };

    SCORM_API_2004.prototype.SetValue = function(param,value) 
    {
    	try{
             debugLog("==> param:" + param + " value:" + value);
           this._CMIDATA[param] = value;
           if(param!=null && param!=undefined && param!='')
               return "true";
           else{
               this._errorCode = 351;
               return "false";
           }
           
           //alert('array length:' + this._CMIDATA.length);
    	}catch(e){
			// to do
    	}
    };
    
    SCORM_API_2004.prototype.GetLastError = function() 
    {
    	try{
         return this._errorCode;
    	}catch(e){
			// to do
    	}
    };
    SCORM_API_2004.prototype.GetErrorString = function(errcode) 
    {
      try{	
        var errstr = this._ErrorCodes[errcode];
        if (errstr==null)
        {
            errstr = "Undefined Error Code";
        }
         return errstr;
      }catch(e){
			// to do
  	  }
    };
    /*
    * Method: LMSGetDiagnostic(String param) 
    * Input: none 
    * Output: Any additional diagonistic information provided by LMS Vendor.  
    */
    SCORM_API_2004.prototype.GetDiagnostic = function() 
    {
     try{	
         return " ";
     }catch(e){
			// to do
	  }
    };
    SCORM_API_2004.prototype.getAllCMIData = function() 
    {
    	try{
         return this._CMIDATA;
    	}catch(e){
			// to do
  	  	}
    };
    SCORM_API_2004.prototype.setCallBack = function (callbackfn)
    {
    	try{
        this._callback = callbackfn;
    	}catch(e){
			// to do
  	  	}
    };
    //LMS SCORM Adaptor API - Ends
    var findAPITries = 0;
    function findAPI(win)
    {
    	try{
        // Check to see if the window (win) contains the API
        // if the window (win) does not contain the API and
        // the window (win) has a parent window and the parent window
        // is not the same as the window (win)
        debugLog("inside findAPI");
        while ( (win.API == null) && (win.parent != null) && (win.parent != win) )
        {
               // increment the number of findAPITries
               findAPITries++;
            
               // Note: 7 is an arbitrary number, but should be more than sufficient
               if (findAPITries > 7)
               {
                  debugLog("Error finding API -- too deeply nested.");
                  return null;
               }
            
               // set the variable that represents the window being
               // being searched to be the parent of the current window
               // then search for the API again
               win = win.parent;
        }
        return win.API;
    	}catch(e){
			// to do
  	  	}
    };

    function getAPI()
    {
    	try{
           debugLog("inside getAPI");
           // start by looking for the API in the current window
           var theAPI = findAPI(window);
        
           // if the API is null (could not be found in the current window)
           // and the current window has an opener window
           if ( (theAPI == null) && (window.opener != null) && (typeof(window.opener) != "undefined") )
           {
              // try to find the API in the current window’s opener
              theAPI = findAPI(window.opener);
           }
           // if the API has not been found
           if (theAPI == null)
           {
              // Alert the user that the API Adapter could not be found
              debugLog("Unable to find an API adapter");
           }
           return theAPI;
    	}catch(e){
			// to do
  	  	}
    }
    function debugLog(msgstr)
    {
     try{	
        var debugConsoleId = document.getElementById("debugConsole");
        var timestamp = new Date().toString();
        if (debugConsoleId != null)
        {
            var logmessage = document.getElementById("debugConsole").innerHTML;
            document.getElementById("debugConsole").innerHTML = logmessage + "<br>" + timestamp + " " + msgstr ;
        }
     }catch(e){
			// to do
	 }
    }
;
(function($){
Drupal.behaviors.contextReactionBlock = {attach: function(context) {
  $('form.context-editor:not(.context-block-processed)')
    .addClass('context-block-processed')
    .each(function() {
      var id = $(this).attr('id');
      Drupal.contextBlockEditor = Drupal.contextBlockEditor || {};
      $(this).bind('init.pageEditor', function(event) {
        Drupal.contextBlockEditor[id] = new DrupalContextBlockEditor($(this));
      });
      $(this).bind('start.pageEditor', function(event, context) {
        // Fallback to first context if param is empty.
        if (!context) {
          context = $(this).data('defaultContext');
        }
        Drupal.contextBlockEditor[id].editStart($(this), context);
      });
      $(this).bind('end.pageEditor', function(event) {
        Drupal.contextBlockEditor[id].editFinish();
      });
    });

  //
  // Admin Form =======================================================
  //
  // ContextBlockForm: Init.
  $('#context-blockform:not(.processed)').each(function() {
    $(this).addClass('processed');
    Drupal.contextBlockForm = new DrupalContextBlockForm($(this));
    Drupal.contextBlockForm.setState();
  });

  // ContextBlockForm: Attach block removal handlers.
  // Lives in behaviors as it may be required for attachment to new DOM elements.
  $('#context-blockform a.remove:not(.processed)').each(function() {
    $(this).addClass('processed');
    $(this).click(function() {
      $(this).parents('tr').eq(0).remove();
      Drupal.contextBlockForm.setState();
      return false;
    });
  });

  // Conceal Section title, subtitle and class
  $('div.context-block-browser', context).nextAll('.form-item').hide();
}};

/**
 * Context block form. Default form for editing context block reactions.
 */
DrupalContextBlockForm = function(blockForm) {
  this.state = {};

  this.setState = function() {
    $('table.context-blockform-region', blockForm).each(function() {
      var region = $(this).attr('id').split('context-blockform-region-')[1];
      var blocks = [];
      $('tr', $(this)).each(function() {
        var bid = $(this).attr('id');
        var weight = $(this).find('select,input').first().val();
        blocks.push({'bid' : bid, 'weight' : weight});
      });
      Drupal.contextBlockForm.state[region] = blocks;
    });

    // Serialize here and set form element value.
    $('form input.context-blockform-state').val(JSON.stringify(this.state));

    // Hide enabled blocks from selector that are used
    $('table.context-blockform-region tr').each(function() {
      var bid = $(this).attr('id');
      $('div.context-blockform-selector input[value='+bid+']').parents('div.form-item').eq(0).hide();
    });
    // Show blocks in selector that are unused
    $('div.context-blockform-selector input').each(function() {
      var bid = $(this).val();
      if ($('table.context-blockform-region tr#'+bid).size() === 0) {
        $(this).parents('div.form-item').eq(0).show();
      }
    });

  };

  // make sure we update the state right before submits, this takes care of an
  // apparent race condition between saving the state and the weights getting set
  // by tabledrag
  $('#ctools-export-ui-edit-item-form').submit(function() { Drupal.contextBlockForm.setState(); });

  // Tabledrag
  // Add additional handlers to update our blocks.
  $.each(Drupal.settings.tableDrag, function(base) {
    var table = $('#' + base + ':not(.processed)', blockForm);
    if (table && table.is('.context-blockform-region')) {
      table.addClass('processed');
      table.bind('mouseup', function(event) {
        Drupal.contextBlockForm.setState();
        return;
      });
    }
  });

  // Add blocks to a region
  $('td.blocks a', blockForm).each(function() {
    $(this).click(function() {
      var region = $(this).attr('href').split('#')[1];
      var base = "context-blockform-region-"+ region;
      var selected = $("div.context-blockform-selector input:checked");
      if (selected.size() > 0) {
        var weight_warn = false;
        var min_weight_option = -10;
        var max_weight_option = 10;
        var max_observed_weight = min_weight_option - 1;
        $('table#' + base + ' tr').each(function() {
          var weight_input_val = $(this).find('select,input').first().val();
          if (+weight_input_val > +max_observed_weight) {
            max_observed_weight = weight_input_val;
          }
        });

        selected.each(function() {
          // create new block markup
          var block = document.createElement('tr');
          var text = $(this).parents('div.form-item').eq(0).hide().children('label').text();
          var select = '<div class="form-item form-type-select"><select class="tabledrag-hide form-select">';
          var i;
          weight_warn = true;
          var selected_weight = max_weight_option;
          if (max_weight_option >= (1 + +max_observed_weight)) {
            selected_weight = ++max_observed_weight;
            weight_warn = false;
          }

          for (i = min_weight_option; i <= max_weight_option; ++i) {
            select += '<option';
            if (i == selected_weight) {
              select += ' selected=selected';
            }
            select += '>' + i + '</option>';
          }
          select += '</select></div>';
          $(block).attr('id', $(this).attr('value')).addClass('draggable');
          $(block).html("<td>"+ text + "</td><td>" + select + "</td><td><a href='' class='remove'>X</a></td>");

          // add block item to region
          //TODO : Fix it so long blocks don't get stuck when added to top regions and dragged towards bottom regions
          Drupal.tableDrag[base].makeDraggable(block);
          $('table#'+base).append(block);
          if ($.cookie('Drupal.tableDrag.showWeight') == 1) {
            $('table#'+base).find('.tabledrag-hide').css('display', '');
            $('table#'+base).find('.tabledrag-handle').css('display', 'none');
          }
          else {
            $('table#'+base).find('.tabledrag-hide').css('display', 'none');
            $('table#'+base).find('.tabledrag-handle').css('display', '');
          }
          Drupal.attachBehaviors($('table#'+base));

          Drupal.contextBlockForm.setState();
          $(this).removeAttr('checked');
        });
        if (weight_warn) {
          alert(Drupal.t('Desired block weight exceeds available weight options, please check weights for blocks before saving'));
        }
      }
      return false;
    });
  });
};

/**
 * Context block editor. AHAH editor for live block reaction editing.
 */
DrupalContextBlockEditor = function(editor) {
  this.editor = editor;
  this.state = {};
  this.blocks = {};
  this.regions = {};

  return this;
};

DrupalContextBlockEditor.prototype = {
  initBlocks : function(blocks) {
    var self = this;
    this.blocks = blocks;
    blocks.each(function() {
      if($(this).hasClass('context-block-empty')) {
        $(this).removeClass('context-block-hidden');
      }
      $(this).addClass('draggable');
      $(this).prepend($('<a class="context-block-handle"></a>'));
      $(this).prepend($('<a class="context-block-remove"></a>').click(function() {
        $(this).parent ('.block').eq(0).fadeOut('medium', function() {
          $(this).remove();
          self.updateBlocks();
        });
        return false;
      }));
    });
  },
  initRegions : function(regions) {
    this.regions = regions;
    var ref = this;

    $(regions).not('.context-ui-processed')
      .each(function(index, el) {
        $('.context-ui-add-link', el).click(function(e){
          ref.showBlockBrowser($(this).parent());
        }).addClass('context-ui-processed');
      });
    $('.context-block-browser').hide();
  },
  showBlockBrowser : function(region) {
    var toggled = false;
    //figure out the id of the context
    var activeId = $('.context-editing', this.editor).attr('id').replace('-trigger', ''),
    context = $('#' + activeId)[0];

    this.browser = $('.context-block-browser', context).addClass('active');

    //add the filter element to the block browser
    if (!this.browser.has('input.filter').size()) {
      var parent = $('.block-browser-sidebar .filter', this.browser);
      var list = $('.blocks', this.browser);
      new Drupal.Filter (list, false, '.context-block-addable', parent);
    }
    //show a dialog for the blocks list
    this.browser.show().dialog({
      modal : true,
      close : function() {
        $(this).dialog('destroy');
        //reshow all the categories
        $('.category', this).show();
        $(this).hide().appendTo(context).removeClass('active');
      },
      height: (.8 * $(window).height()),
      minHeight:400,
      minWidth:680,
      width:680
    });

    //handle showing / hiding block items when a different category is selected
    $('.context-block-browser-categories', this.browser).change(function(e) {
      //if no category is selected we want to show all the items
      if ($(this).val() == 0) {
        $('.category', self.browser).show();
      } else {
        $('.category', self.browser).hide();
        $('.category-' + $(this).val(), self.browser).show();
      }
    });

    //if we already have the function for a different context, rebind it so we don't get dupes
    if(this.addToRegion) {
      $('.context-block-addable', this.browser).unbind('click.addToRegion')
    }

    //protected function for adding a clicked block to a region
    var self = this;
    this.addToRegion = function(e){
      var ui = {
        'item' : $(this).clone(),
        'sender' : $(region)
      };
      $(this).parents('.context-block-browser.active').dialog('close');
      $(region).after(ui.item);
      self.addBlock(e, ui, this.editor, activeId.replace('context-editable-', ''));
    };

    $('.context-block-addable', this.browser).bind('click.addToRegion', this.addToRegion);
  },
  // Update UI to match the current block states.
  updateBlocks : function() {
    var browser = $('div.context-block-browser');

    // For all enabled blocks, mark corresponding addables as having been added.
    $('.block, .admin-block').each(function() {
      var bid = $(this).attr('id').split('block-')[1]; // Ugh.
    });
    // For all hidden addables with no corresponding blocks, mark as addable.
    $('.context-block-item', browser).each(function() {
      var bid = $(this).attr('id').split('context-block-addable-')[1];
    });

    // Mark empty regions.
    $(this.regions).each(function() {
      if ($('.block:has(a.context-block)', this).size() > 0) {
        $(this).removeClass('context-block-region-empty');
      }
      else {
        $(this).addClass('context-block-region-empty');
      }
    });
  },
  // Live update a region
  updateRegion : function(event, ui, region, op) {
    switch (op) {
      case 'over':
        $(region).removeClass('context-block-region-empty');
        break;
      case 'out':
        if (
          // jQuery UI 1.8
          $('.draggable-placeholder', region).size() === 1 &&
          $('.block:has(a.context-block)', region).size() == 0
        ) {
          $(region).addClass('context-block-region-empty');
        }
        break;
    }
  },
  // Remove script elements while dragging & dropping.
  scriptFix : function(event, ui, editor, context) {
    if ($('script', ui.item)) {
      var placeholder = $(Drupal.settings.contextBlockEditor.scriptPlaceholder);
      var label = $('div.handle label', ui.item).text();
      placeholder.children('strong').html(label);
      $('script', ui.item).parent().empty().append(placeholder);
    }
  },
  // Add a block to a region through an AJAX load of the block contents.
  addBlock : function(event, ui, editor, context) {
    var self = this;
    if (ui.item.is('.context-block-addable')) {
      var bid = ui.item.attr('id').split('context-block-addable-')[1];

      // Construct query params for our AJAX block request.
      var params = Drupal.settings.contextBlockEditor.params;
      params.context_block = bid + ',' + context;
      if (!Drupal.settings.contextBlockEditor.block_tokens || !Drupal.settings.contextBlockEditor.block_tokens[bid]) {
        alert(Drupal.t('An error occurred trying to retrieve block content. Please contact a site administer.'));
        return;
     }
     params.context_token = Drupal.settings.contextBlockEditor.block_tokens[bid];

      // Replace item with loading block.
      //ui.sender.append(ui.item);

      var blockLoading = $('<div class="context-block-item context-block-loading"><span class="icon"></span></div>');
      ui.item.addClass('context-block-added');
      ui.item.after(blockLoading);


      $.getJSON(Drupal.settings.contextBlockEditor.path, params, function(data) {
        if (data.status) {
          var newBlock = $(data.block);
          if ($('script', newBlock)) {
            $('script', newBlock).remove();
          }
          blockLoading.fadeOut(function() {
            $(this).replaceWith(newBlock);
            self.initBlocks(newBlock);
            self.updateBlocks();
            Drupal.attachBehaviors(newBlock);
          });
        }
        else {
          blockLoading.fadeOut(function() { $(this).remove(); });
        }
      });
    }
    else if (ui.item.is(':has(a.context-block)')) {
      self.updateBlocks();
    }
  },
  // Update form hidden field with JSON representation of current block visibility states.
  setState : function() {
    var self = this;

    $(this.regions).each(function() {
      var region = $('.context-block-region', this).attr('id').split('context-block-region-')[1];
      var blocks = [];
      $('a.context-block', $(this)).each(function() {
        if ($(this).attr('class').indexOf('edit-') != -1) {
          var bid = $(this).attr('id').split('context-block-')[1];
          var context = $(this).attr('class').split('edit-')[1].split(' ')[0];
          context = context ? context : 0;
          var block = {'bid': bid, 'context': context};
          blocks.push(block);
        }
      });
      self.state[region] = blocks;
    });
    // Serialize here and set form element value.
    $('input.context-block-editor-state', this.editor).val(JSON.stringify(this.state));
  },
  //Disable text selection.
  disableTextSelect : function() {
    if ($.browser.safari) {
      $('.block:has(a.context-block):not(:has(input,textarea))').css('WebkitUserSelect','none');
    }
    else if ($.browser.mozilla) {
      $('.block:has(a.context-block):not(:has(input,textarea))').css('MozUserSelect','none');
    }
    else if ($.browser.msie) {
      $('.block:has(a.context-block):not(:has(input,textarea))').bind('selectstart.contextBlockEditor', function() { return false; });
    }
    else {
      $(this).bind('mousedown.contextBlockEditor', function() { return false; });
    }
  },
  //Enable text selection.
  enableTextSelect : function() {
    if ($.browser.safari) {
      $('*').css('WebkitUserSelect','');
    }
    else if ($.browser.mozilla) {
      $('*').css('MozUserSelect','');
    }
    else if ($.browser.msie) {
      $('*').unbind('selectstart.contextBlockEditor');
    }
    else {
      $(this).unbind('mousedown.contextBlockEditor');
    }
  },
  // Start editing. Attach handlers, begin draggable/sortables.
  editStart : function(editor, context) {
    var self = this;
    // This is redundant to the start handler found in context_ui.js.
    // However it's necessary that we trigger this class addition before
    // we call .sortable() as the empty regions need to be visible.
    $(document.body).addClass('context-editing');
    this.editor.addClass('context-editing');
    this.disableTextSelect();
    this.initBlocks($('.block:has(a.context-block.edit-'+context+')'));
    this.initRegions($('.context-block-region').parent());
    this.updateBlocks();

    $('a.context_ui_dialog-stop').hide();

    $('.editing-context-label').remove();
    var label = $('#context-editable-trigger-'+context+' .label').text();
    label = Drupal.t('Now Editing: ') + label;
    editor.parent().parent()
      .prepend('<div class="editing-context-label">'+ label + '</div>');

    // First pass, enable sortables on all regions.
    $(this.regions).each(function() {
      var region = $(this);
      var params = {
        revert: true,
        dropOnEmpty: true,
        placeholder: 'draggable-placeholder',
        forcePlaceholderSize: true,
        items: '> .block:has(a.context-block.editable)',
        handle: 'a.context-block-handle',
        start: function(event, ui) { self.scriptFix(event, ui, editor, context); },
        stop: function(event, ui) { self.addBlock(event, ui, editor, context); },
        receive: function(event, ui) { self.addBlock(event, ui, editor, context); },
        over: function(event, ui) { self.updateRegion(event, ui, region, 'over'); },
        out: function(event, ui) { self.updateRegion(event, ui, region, 'out'); },
        cursorAt: {left: 300, top: 0}
      };
      region.sortable(params);
    });

    // Second pass, hook up all regions via connectWith to each other.
    $(this.regions).each(function() {
      $(this).sortable('option', 'connectWith', ['.ui-sortable']);
    });

    // Terrible, terrible workaround for parentoffset issue in Safari.
    // The proper fix for this issue has been committed to jQuery UI, but was
    // not included in the 1.6 release. Therefore, we do a browser agent hack
    // to ensure that Safari users are covered by the offset fix found here:
    // http://dev.jqueryui.com/changeset/2073.
    if ($.ui.version === '1.6' && $.browser.safari) {
      $.browser.mozilla = true;
    }
  },
  // Finish editing. Remove handlers.
  editFinish : function() {
    this.editor.removeClass('context-editing');
    this.enableTextSelect();

    $('.editing-context-label').remove();

    // Remove UI elements.
    $(this.blocks).each(function() {
      $('a.context-block-handle, a.context-block-remove', this).remove();
      if($(this).hasClass('context-block-empty')) {
        $(this).addClass('context-block-hidden');
      }
      $(this).removeClass('draggable');
    });

    $('a.context_ui_dialog-stop').show();

    this.regions.sortable('destroy');

    this.setState();

    // Unhack the user agent.
    if ($.ui.version === '1.6' && $.browser.safari) {
      $.browser.mozilla = false;
    }
  }
}; //End of DrupalContextBlockEditor prototype

})(jQuery);
;
/**
 * This is part of a patch to address a jQueryUI bug.  The bug is responsible
 * for the inability to scroll a page when a modal dialog is active. If the content
 * of the dialog extends beyond the bottom of the viewport, the user is only able
 * to scroll with a mousewheel or up/down keyboard keys.
 *
 * @see http://bugs.jqueryui.com/ticket/4671
 * @see https://bugs.webkit.org/show_bug.cgi?id=19033
 * @see /views_ui.module
 * @see /js/jquery.ui.dialog.min.js
 *
 * This javascript patch overwrites the $.ui.dialog.overlay.events object to remove
 * the mousedown, mouseup and click events from the list of events that are bound
 * in $.ui.dialog.overlay.create
 *
 * The original code for this object:
 * $.ui.dialog.overlay.events: $.map('focus,mousedown,mouseup,keydown,keypress,click'.split(','),
 *  function(event) { return event + '.dialog-overlay'; }).join(' '),
 *
 */

(function ($, undefined) {
  if ($.ui && $.ui.dialog) {
    $.ui.dialog.overlay.events = $.map('focus,keydown,keypress'.split(','),
                                 function(event) { return event + '.dialog-overlay'; }).join(' ');
  }
}(jQuery));
;
Drupal.locale = { 'strings': {"Content":"Content","Location":"Location","Group":"Group","ERR047":"You have already registered for this class","ERR055":"Login again since your session has expired","ERR057":"System error has occurred. Contact Support.","MSG247":"CartAdded","Certification":"Certification","English":"English","Register":"Register","Sign In":"Sign In","Registered":"Registered","Completed":"Completed","Enrolled":"Enrolled","Canceled":"Canceled","Pending":"Pending","Expired":"Expired","Survey":"Survey","ERR066":"Select a class in each course.","LBL001":"Sign In","LBL012":"Enrolled","LBL013":"Completed","LBL014":"Incomplete","LBL015":"Canceled","LBL017":"title a-z","LBL018":"title z-a","LBL019":"date new to old","LBL020":"date old to new","LBL021":"type","LBL022":"mandatory","LBL024":"Expired","LBL025":"Enrolled by","LBL026":"Canceled On","LBL027":"Completed On","LBL028":"Expired On","LBL032":"Registered","LBL034":"Completed","LBL036":"Type","LBL038":"Language","LBL041":"Location","LBL042":"Date","LBL044":"newly listed","LBL045":"start date","LBL046":"Full","LBL049":"Added to cart","LBL052":"Seat left","LBL053":"Seats left","LBL054":"Username","LBL074":"Timezone","LBL082":"Remove","LBL083":"Title","LBL086":"Instructor","German":"German","Spanish":"Spanish","French":"French","Italian":"Italian","Japanese":"Japanese","Korean":"Korean","Russian":"Russian","LBL087":"Course","LBL088":"Enter a course","LBL096":"Code","LBL102":"Status","LBL106":"Seats","LBL107":"Name","LBL109":"Cancel","LBL112":"mm-dd-yyyy","LBL113":"End: mm-dd-yyyy","LBL121":"optional","LBL123":"Close","LBL126":"Waitlist seat left","LBL127":"Waitlist seats left","LBL181":"Type a Username","MSG262":"No search results found.","MSG263":"Do you want to cancel the enrollment?","MSG264":"The entire amount paid will be refunded back to your account","MSG266":"Do you still want to cancel the class registration?","LBL186":"enrolled","LBL190":"Waitlisted","LBL191":"Tags","LBL193":"Type a tag","MSG268":"No courses have been associated","LBL199":"Launch","LBL202":"Attempts Left","LBL203":"Survey","LBL205":"Certificate","LBL231":"Attachments","LBL232":"(click to download)","LBL234":"Complete By","LBL247":"Course Equivalence","LBL251":"Start","LBL263":"Class Code","LBL271":" already exist.","LBL274":"Assessment","LBL277":"Sessions","LBL286":"Delete","LBL287":"Add","Yes":"Yes","No":"No","LBL304":"Search","LBL316":"Classroom","LBL324":"Type a question","ERR101":" is required.","ERR106":"Select one or more.","LBL342":"Yes","LBL343":"No","MSG311":"Users can register multiple times to the class.","ERR119":"Select one or more Users.","LBL416":"Loading...","LBL426":"Mandatory","LBL427":"Recommended","LBL428":"Any","LBL432":"System","LBL435":"Class","LBL545":"Type title or code","CATALOG":"CATALOG","REPORTS":"Reports","Assessment":"Assessment","LBL569":"Done","LBL573":"Activate","LBL604":"Validity","LBL605":"days","LBL621":"To","Course":"Course","Class":"Class","Classroom":"Classroom","Video":"Video","Virtual Class":"Virtual Class","Web-based":"Web-based","LBL646":"Selected","LBL647":"AND","MSG379":"No classes exist under one of the associated courses. Registration failed. Contact Support Team.","MSG380":"No seats available for this program","Training Plan":"Training Plan","MSG381":"Your search did not return any results","LBL651":"Re-Certify","Mandatory":"Mandatory","Recommended":"Recommended","Optional":"Optional","LBL674":"Select","LBL668":"Score","LBL670":"Session Details","LBL675":"Select Start Date","LBL676":"Select End Date","MSG395":"Select atleast one option.","MSG399":"Paid training cannot be rescheduled. Contact Support.","MSG400":"Cancel the Course registration and Register again.","MSG401":"will be deducted if you cancel the class.","MSG402":"An amount of","LBL677":"Days Remaining","LBL680":"Canceled by","LBL681":"Completed by","LBL685":"completed","LBL686":"incomplete","LBL687":"expired","LBL688":"canceled","LBL689":"pending","LBL691":"Full Name","LBL692":"Previous","LBL693":"Next","LBL694":"Preview","LBL698":"Incomplete","LBL704":"Enrolled On","LBL726":"Not Registered","ERR149":"Select a class in each course.","LBL721":"Registration Status","LBL716":"Select Class","MSG419":"The session is completed.","MSG420":"The session has not started, try again at","MSG424":"Cancellation in progress.....","LBL713":"More","MSG430":"You are already enrolled in this program","LBL735":"Expires On","LBL737":"Web-based","LBL738":"Virtual Class","LBL739":"Video","LBL740":"Certification","LBL741":"Curricula","LBL742":"Learning Plan","LBL743":"C","LBL746":"R","LBL747":"Marked Complete","LBL749":"Confirmation","Not Registered":"Not Registered","Instructor":"Instructor","ERR154":"System error, Contact Support Team.","LBL766":"Type a Class Title or Code","Compliance":"Compliance","ERR169":" are required.","MSG511":"Not Started","LBL816":"View","LBL820":"Order Id","LBL846":"Format Report Body","LBL847":"Format Report Header","LBL852":"User Profile","LBL854":"Lesson","LBL868":"Add Comment","LBL871":"Post","MSG535":"You have already registered for the class","MSG536":"Do you want to register again?","LBL880":"Join","LBL889":"Lessons","LBL893":"Minimize","LBL894":"maximize","LBL910":"Day","Simplified Chinese":"Simplified Chinese","LBL930":"Unlock","LBL945":"on","In progress":"In progress","Incomplete":"Incomplete","No Show":"No Show","Attended":"Attended","MSG588":"per page","LBL943":"Change","LBL929":"Message","LBL981":"of","LBL989":"Page","LBL1003":"Module","LBL1039":"All","MSG639":"character types are lower case, upper case, digit or punctuation","MSG644":"The group name","LBL1064":"Points","Check":"Check","LBL1123":"Version","LBL1146":"Select valid To Date","LBL1154":"Select valid From Date","LBL1155":"To date cannot be less than the From date","MSG687":"Select an option.","LBL1163":"Do you want to confirm the order?","LBL1164":"Do you want to cancel the order?","LBL1165":"Refund","MSG692":"Cannot register since there is price associated to the Class.Contact Support.","LBL1193":"Incomplete on","LBL1246":"Priced training cannot be canceled. Cancellation can be done from the order screen.","LBL1253":"Pre","LBL1267":"You have already registered for another class of the same course","LBL1268":"Do you want to register to another class?","MSG711":"Access is not set. All users will be registered to the class if access is not set. Do you want to set Access?","Groups":"Groups","by":"by","Share":"Share","LBL1270":"Type a group name","Search for refine":"Search to refine","The location is associated to class":"The location is associated to class","You need to complete":"You need to complete","LBL1272":"Uncheck","ERR248":"Select the required details and activate the discount.","Reregister":"Reregister","!name cannot be longer than %max characters but is currently %length characters long.":"!name cannot be longer than %max characters but is currently %length characters long.","Panel":"Panel","MSG729":"Unchecking the manager role will remove all direct and indirect reports for this user. Do you want to continue?","LBL1284":"Success Status","failed":"Failed","passed":"Passed","unknown":"Unknown","MSG746":"There are no currency to be added.","MSG747":"Are you sure you want to delete the currency","Waived":"Waived","MSG754":"Content is playing in a window. Close the window before launching another content."} };;
