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
//var reloadFlagForPagination=true;
(function($) {
	try{
	$.widget("ui.narrowsearch",{
						// Static variables
						widgetName : 'narrowsearch',
						widgetInitId : 'root_admin',
						widgetSelector : '#root_admin',
						searchBasePath : '',
						firstTime : true,
						filterSets : new Array(), // Each element of this  array is an object, e.g. {type: "checkbox", code: "orgstatus"}
						lastSortType : '',
						lastSortTypeHtmlId : '',
						exportPath : '',
						printPath : '',
						showTopTextFilter: true,
						topTextFilterLabel:'',
						tabContentType:'narrow_search',
						qtipLoadSet : new Array(),
						qtipLenth : 0,

						_init:function(){
							//this.initAdminLinks();
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							var pageUrl = window.location.search;
							if (pageUrl.indexOf("admincalendar") < 0) // search call restricted for admin cal
								this.getSearchContentDisp();

						},

						getUrlVars : function (name) {
							var vars = {};
							var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) {
								vars[key] = value;
							});
							return vars[name];
						},

						getModuleURLs : function(){
							var qPath = this.getUrlVars('q');
							if(qPath == 'administration'){
								//qPath = $('#root-admin-links-holder li:first-child a').attr('href');
								//window.location.href = qPath;
								return false;
							}
							var qPathArray = qPath.split('/');
							var urlArray = new Array();
							urlArray['main_module'] = qPathArray[1];
							urlArray['sub_module'] = qPathArray[2];
							return urlArray;
						},

						getSearchContentDisp : function() {
							var obj = this;
							var curObj = this;

							var moduleURLs = obj.getModuleURLs();
							if (moduleURLs === false) {
							  return false;
							}

							/* Highlight root admin menu */
							if($('#block-system-main-menu a[href="/?q=administration"]').hasClass('active') == false){
								$('#block-system-main-menu a[href="/?q=administration"]').addClass('active-trail active');
								$('#block-system-main-menu a[href="/?q=administration"]').parent().addClass('active-trail');
							}
							
						  /* Highlight selected menu links */
						  $("ul.list-item-administrator li a").removeClass("root-admin-links-selected");
						  $("#"+moduleURLs['main_module']).addClass("root-admin-links-selected");

						  /* THE BELOW LINES WRITTEN FOR NEW THEME BY VJ*/
						  $("ul.list-item-administrator li.admin-left-panel-module-list").removeClass("root-admin-module-selected");
						  $("#"+moduleURLs['main_module']).parent().addClass("root-admin-module-selected");
							curObj.createLoader("root-admin-search-right-col");
 							url = obj.constructUrl("administration/search-filter/"+moduleURLs['sub_module']);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(data){

								   curObj.destroyLoader("root-admin-search-right-col");

								   var detailsObj = jQuery.parseJSON(data);

								   // Extend Drupal.settings, if any settings are present in the data
								   if (detailsObj!=null && detailsObj.drupal_settings) {
								     $.extend(true, Drupal.settings, detailsObj.drupal_settings);
								   }

								   /* Display the various admin tabs, etc. including the tab-content-main div */
                   $("#root-admin-results").html(detailsObj.rendered_main_div);
                   $("#root-admin-results").show();

                   /* Display the content in the main content tab.
                    * For narrow_search, this will render place holder HTML elements for search results jqGrid, sortbar, action bar, etc */
                   $("#tab-content-main").html(detailsObj.rendered_tab_content_main);
                   
                 //Added by vetrivel.P for #0070900
                   if(moduleURLs['sub_module'] == 'user'){
                   var sec = readCookie('user_upload_message');
                   if(sec !== null){
                   		sec = urldecode(sec);
                   		var error = new Array();
                   		error[0] = sec;
                   		$('#show_suspend_message').remove();
                   		 var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
                   		 var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();eraseCookie(\'message\');"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
                   		 $('#tab-content-main').prepend(message);
                   		 $("#tab-content-main #show_suspend_message").append(msg);
                   	eraseCookie('user_upload_message');
                   	}
                   }else if(moduleURLs['sub_module'] == 'catalog'){
                       var sec = readCookie('bulk_enrollment_upload_message');
                   if(sec !== null){
                   		sec = urldecode(sec);
                   		var error = new Array();
                   		error[0] = sec;
                   		$('#show_suspend_message').remove();
                   		 var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
                   		 var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();eraseCookie(\'message\');"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
                   		 $('#tab-content-main').prepend(message);
                   		 $("#tab-content-main #show_suspend_message").append(msg);
                   	eraseCookie('bulk_enrollment_upload_message');
                   	}
                   }
                  
								   if (detailsObj.rendered_tab_content_type == "narrow_search") { /* If narrow search */
									   /* Display left side filters*/
										 $("#narrow-search").html(detailsObj.rendered_narrow_search_filters);
										 $("#narrow-search").show();
										 if ($('#group_list_count').val() > 4) {
											$('#group_filterset').css({'height': '90px', 'overflow': 'hidden', 'width': '160px'});
											$("#group_filterset").jScrollPane();
										 }

						                /* if ($('#rescountry_filterset #adminFilterlist_country').val() > 5) {
							  				$('#rescountry_filterset').css({'height': '150px', 'overflow': 'hidden', 'width': '160px'});
							  		   		$('#rescountry_filterset').jScrollPane();
						  			     }	*/					              
										 // Ticket No:0080037 (Revision No:0275605, Point No: 2.3) #custom_attribute_0078975
										 var tabHeight= $("#sort-bar-V2").css('height');
										 if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
											 tabHeight = parseInt(tabHeight)-2;
										 
										 /* Display top bar */
										 if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
											 //$("#admin-maincontent_tab").html("<img style='width:24px;height:24px;padding-top:5px;cursor:pointer;' onclick='drawCalendar();' src='/sites/all/themes/core/expertusoneV2/images/calendaricon.png'></img>");
											 $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_sortbar);
											 $("#narrow-search-actionbar").remove();
											 // Ticket No:0080037 (Revision No:0275605, Point No: 2.3) #custom_attribute_0078975
											 if(moduleURLs['main_module']=='manage' && (moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute') && $('#carousel_inner').parent('#carousel_container')) {
												 $("#sort-bar-V2").css('height','0').css('overflow','hidden');
											 }
											 $("#sort-bar-V2").append(detailsObj.rendered_narrow_search_actionbar);
										 }
										 else {
											 $("#narrow-search-results-topbar").html(detailsObj.rendered_narrow_search_sortbar);
											 $("#narrow-search-actionbar").remove();
											 $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_actionbar);
										 }
										 $("#narrow-search-results-topbar").show();

									/*	 if (Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
										   var adminSubModelLength = $("ul.AdminsublinktabNavigation li").length;
											 if(adminSubModelLength >= 4) {
											 	 $("ul.AdminsublinktabNavigation li a span span").css('padding','0px 5px 0px 5px');
											 }
										 }*/
										 /* Add class "selected" to this selected tab and raise it up to the front */
										 $( "ul.AdminsublinktabNavigation li").removeClass("selected");
										 $("#admin-tab-"+moduleURLs['sub_module']).parent("li").addClass("selected");
										 resetMainTab(); // Display selected tab if it is in hidden
										 //#custom_attribute_0078975
										//Custom Attribute Changes 
										if(moduleURLs['main_module']=='manage' && $('#carousel_inner').parent('#carousel_container')) {
											var itemcnt = $('#carousel_container #carousel_inner ul li').size();
												// console.log('itemcnt: '+itemcnt);
											if(itemcnt === 6) {
												var sort_width = parseInt($('#sort-bar-V2').width());
													// console.log('load container width: '+sort_width);
												var actionbar_width = parseInt($('#narrow-search-actionbar').width());
													// console.log('load actionbar width: '+actionbar_width);
												var container_width = (sort_width - actionbar_width) - 45;
												var width_calc = '';
												var ulactual_width = $('#carousel_container #carousel_inner ul').width();
													// console.log('ul actual width: '+ulactual_width);
												$('#carousel_container #carousel_inner').css('width', 'auto');
												$('#carousel_container #carousel_inner ul').css('width', 'auto');
												var ul_width = parseInt($('#carousel_container #carousel_inner ul').width());
													// console.log('ul width: '+ul_width);
												var lastli_width = parseInt($('#carousel_container #carousel_inner ul li:last').width());
													// console.log('last li width: '+lastli_width);
												if(container_width > ul_width) {	
													width_calc = container_width;
													$('#carousel_container .last').css('display', 'none');
												} else 
													width_calc = (ul_width - lastli_width) - 4;
												// console.log('carousel_inner width: '+width_calc);
												$('#carousel_container #carousel_inner').css('width', width_calc);
												$('#carousel_container #carousel_inner ul').css('width', ulactual_width);
												
												if(moduleURLs['sub_module']=='customattribute')
													MoveTabNext(itemcnt);
											} else {
												if(moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute')
													MoveTabNext(itemcnt);
											}
										}
										// Ticket No:0080037 (Revision No:0275605, Point No: 2.3)
										if(moduleURLs['main_module']=='manage' && (moduleURLs['sub_module']=='announcement' || moduleURLs['sub_module']=='customattribute') && $('#carousel_inner').parent('#carousel_container')) {
											setTimeout(function(){
												$("#sort-bar-V2").css('height',tabHeight).css('overflow','visible');
											},500);
										}

										 var linkPath = 'administration/'+moduleURLs['main_module'];
										 var sublinkPath = 'administration/'+moduleURLs['main_module']+'/'+moduleURLs['sub_module'];
										 curObj.initPaths(detailsObj.rendered_tab_content_type);
										 curObj.initFilters();
										 curObj.renderSearchResults(linkPath, sublinkPath);

										 if($('#narrow-search-actionbar')){
								  	   $('#narrow-search-actionbar').hide();
										 }
										 if(typeof detailsObj.rendered_script != 'undefined'){
											 eval(detailsObj.rendered_script+"()");
										 }
										 $('.courselangtype-scroll-list-container, .coursecurrency-scroll-list-container, .contentlang-scroll-list-container, .tpcurrency-scroll-list-container, .surveydetailslang-scroll-list-container, .surveyquestionslang-scroll-list-container, .langtype-scroll-list-container, .grplang-scroll-list-container, .rescountry-scroll-list-container, .announcementlang-scroll-list-container, .ordercurrency-scroll-list-container').jScrollPane({});
				   }
								   else if (detailsObj.rendered_tab_content_type == "ajax-form") {
                     Drupal.attachBehaviors();
                   }
								}
							});
						},

						initPaths : function(tabContentType){   /* Initialize path for search, print, export, filter option */
							if(tabContentType!=""){
							  this.tabContentType=tabContentType;
							}
							if(this.tabContentType=="narrow_search"){
								/* Narrow search filter data lists */
								var filterSetData=$("#narrow-search-filters").attr("data");
								this.filterSets=eval(filterSetData); //commented for issue in Drupal translated strings for french language
								//this.filterSets=eval(filterSetData.replace(/\\u0026#39;/g,"'"));


								/*Sort bar data lists */
								var sortBarData=$("#narrow-search-sortbar").attr("data");
								var sortBarObj = jQuery.parseJSON(eval("'"+ sortBarData +"'"));
								this.lastSortType = sortBarObj.last_sort_type;
								this.lastSortTypeHtmlId = sortBarObj.last_sort_type_html_id;

								/* Action bar data lists */
								var actionBarData=$("#narrow-search-actionbar").attr("data");
								var actionBarObj = jQuery.parseJSON(eval("'"+ actionBarData +"'"));
								this.exportPath = actionBarObj.export_path;
								this.printPath = actionBarObj.print_path;

								/* Other - narrow search data lists */
								var narrowSearchOtherData=$("#narrow-search-results").attr("data");
								var narrowSearchOtherObj = jQuery.parseJSON(eval("'"+ narrowSearchOtherData +"'"));

								this.searchBasePath = narrowSearchOtherObj.search_base_path;
								this.text_filter_ac_path = narrowSearchOtherObj.text_filter_ac_path;
								this.showTopTextFilter = narrowSearchOtherObj.show_top_text_filter;
								this.topTextFilterLabel = narrowSearchOtherObj.top_text_filter_label;

								if(this.topTextFilterLabel==null || this.topTextFilterLabel=="undefined" || this.topTextFilterLabel==undefined){
									this.topTextFilterLabel = (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') ? Drupal.t("LBL304") : Drupal.t("LBL304").toUpperCase();
								}

								if(this.showTopTextFilter){

								  this.initTextFilter(this.text_filter_ac_path);

								  $("#narrow-search-text-filter").val(this.topTextFilterLabel);

								  this.initTextDateFilterBlurStyle("#narrow-search-text-filter", this.topTextFilterLabel,"1");

								  $("#narrow-search-text-filter-container").show();

								}else{

								  $("#narrow-search-text-filter-container").hide();

								}
						   }else{ /* Non - NarrowSearch */

						   }

						},

						displayResultWizard : function(title,message){
						    $('#result-msg-wizard').remove();
						    var html = '';
						    html+='<div id="result-msg-wizard" style="display:none; padding: 0px;">';
						    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

						    html+= '<tr><td height="40px">&nbsp;</td></tr>';
							html+= '<tr><td align="center" class="light-gray-color">'+message+'</td></tr>';

						    html+='</table>';
						    html+='</div>';
						    $("body").append(html);
						    var closeButt={};
						    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#result-msg-wizard').remove();};

						    $("#result-msg-wizard").dialog({
						        position:[(getWindowWidth()-400)/2,200],
						        bgiframe: true,
						        width:400,
						        resizable:false,
						        modal: true,
						        title:title,//"Delete",
						        buttons:closeButt,
						        closeOnEscape: false,
						        draggable:false,
						        overlay:
						         {
						           opacity: 0.9,
						           background: "black"
						         }
						    });

						    $('.ui-dialog').wrap("<div id='delete-object-dialog'></div>");

						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
						    $('.ui-dialog-buttonpane .ui-difalog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

						    $("#result-msg-wizard").show();

							$('.ui-dialog-titlebar-close').click(function(){
						        $("#result-msg-wizard").remove();
						    });
							this.currTheme = Drupal.settings.ajaxPageState.theme;
						 	if(this.currTheme == "expertusoneV2"){
						 	 changeDialogPopUI();
							}
						},

						resetUserPassword : function(userEmail, userId, rowObj){
							var obj = this;
							$("#reset-password-loader-"+userId).remove();
							var cRow = $(rowObj).parents("tr").before("<tr><td colspan='3'><div id='reset-password-loader-"+userId+"'></div></td></tr>");
							this.createLoader("reset-password-loader-"+userId);
							url = obj.constructUrl("ajax/administration/people/reset-password" + '/' + userEmail );
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
									result = $.trim(result);
									result = result.split('|');
									$("#root-admin").data("narrowsearch").displayResultWizard(result[0],result[1]);
									$("#root-admin").data("narrowsearch").destroyLoader('reset-password-loader-'+userId);
								}
							});
						},

						publishAndUnpublishCatalog : function(catalogId, catalogType,actionStatus, rowObj, iscompliance) {
							var comp = iscompliance;
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/catalog/show-catalog" + '/' + catalogId);

							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(access){
									this.currTheme = Drupal.settings.ajaxPageState.theme;

									var show =	$.trim($('#publish-unpublish-'+catalogId).html());

									if(iscompliance == 1 && access == 0 && show == 'Show in Catalog'){
										var uniqueClassPopup = '';
										 $('#delete-msg-wizard').remove();
										    var html = '';
										    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
										    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
										    if(this.currTheme == 'expertusoneV2'){
										   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("MSG711")+'</td></tr>';
										    } else {
										     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("MSG711")+'</td></tr>';
										    }
										    html+='</table>';
										    html+='</div>';
										    $("body").append(html);

										    var closeButt={};
										    //closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
										    closeButt[Drupal.t('LBL109')]=function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishCat(catalogId, catalogType,actionStatus, rowObj);
										    };
										    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
										    	var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
										    			'esignFor':'PublishUnpublishCatalog','catalogId':catalogId,'catalogType': catalogType,'actionStatus': actionStatus, 'rowObj': rowObj};
										    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };

										 }else{
											 closeButt[Drupal.t('Yes')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
											 setTimeout(function() {
													$('#access-visibility-'+catalogId).click();
												}, 10);
											 };

   										 }
										    $("#delete-msg-wizard").dialog({
										        position:[(getWindowWidth()-400)/2,200],
										        bgiframe: true,
										        width:300,
										        resizable:false,
										        modal: true,
										        title:Drupal.t('LBL749'),
										        buttons:closeButt,
										        closeOnEscape: false,
										        draggable:false,
										        zIndex : 10005,
										        overlay:
										         {
										           opacity: 0.9,
										           background: "black"
										         }
										    });
										    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
										    $(".removebutton").text(Drupal.t("No"));
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
										    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
										    $("#delete-msg-wizard").show();

											$('.ui-dialog-titlebar-close').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											$('.admin-save-button-middle-bg').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											$('.removebutton').click(function(){
										        $("#delete-msg-wizard").remove();
										    });
											if(this.currTheme == 'expertusoneV2'){
										    	changeDialogPopUI();
										    }




										}
									else{
										var show = $('#publish-unpublish-'+catalogId).html();
										if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
											var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
													'esignFor':'PublishUnpublishCatalog','catalogId':catalogId,'catalogType': catalogType,'actionStatus': actionStatus, 'rowObj': rowObj};
											$.fn.getNewEsignPopup(esignObj); // #0040182 This Function is Missing
										}else{
											$("#root-admin").data("narrowsearch").publishAndUnpublishCat(catalogId, catalogType,actionStatus, rowObj);

										}
									}

								}
							});

						},

						publishAndUnpublishCat : function(catalogId, catalogType,actionStatus, rowObj){
							var obj = this;
							//$("#class-detail-loader-"+catalogId).remove();
							//var cRow = $(rowObj).parents("tr").parents("tr").before("<tr><td colspan='3'><div id='class-detail-loader-"+catalogId+"'></div></td></tr>");
							//this.createLoader("class-detail-loader-"+catalogId);
							url = obj.constructUrl("ajax/administration/learning/catalog/publish-and-unpublish" + '/' + catalogId + '/' + catalogType + '/' + actionStatus);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='lrn_crs_sts_atv' || result[0]=='lrn_cls_sts_atv'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='lrn_crs_sts_itv' || result[0]=='lrn_cls_sts_itv'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-publish-btn");
									}
									else if(result[0]=='lrn_cls_sts_dld'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).removeClass("action-crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-pub-enrolled-btn");
										$("#publish-unpublish-"+catalogId).removeAttr("onclick").unbind("click");
									}else if(result[0]=='lrn_cls_sts_can'){
										$("#publish-unpublish-"+catalogId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+catalogId).removeClass("action-crs-publish-btn");
										$("#publish-unpublish-"+catalogId).addClass("crs-pub-enrolled-btn");
										$("#publish-unpublish-"+catalogId).removeAttr("onclick").unbind("click");
									}
									$("#publish-unpublish-"+catalogId).html(result[1]);
								}
							});
						},

						publishAndUnpublishProgram : function(programId, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'program-publish-unpublis-dialog',
							    			'esignFor':'PublishUnpublishProgram','programId':programId,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishPrg(programId, rowObj);
							 }

						},

						publishAndUnpublishPrg : function(programId, rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/program/publish-and-unpublish" + '/' + programId);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='lrn_lpn_sts_atv'){
										$("#publish-unpublish-"+programId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+programId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='lrn_lpn_sts_itv'){
										$("#publish-unpublish-"+programId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+programId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+programId).html(result[1]);
								}
							});
						},
						publishAndUnpublishSurveyAssessment : function(surveyId, surveyType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'survey-publish-unpublis-dialog',
							    			'esignFor':'publishAndUnpublishSurveyAssessment','surveyId':surveyId,'surveyType':surveyType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishSur(surveyId, surveyType, rowObj);
							 }

						},

						publishAndUnpublishSur : function(surveyId, surveyType, rowObj){
							var obj = this;
							if(surveyType =='Assessment'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment/publish-and-unpublish" + '/' + surveyId + '/' + surveyType);
							}
							else{
								url = obj.constructUrl("ajax/administration/survey/survey-assessment/publish-and-unpublish" + '/' + surveyId + '/' + surveyType);
							}
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='sry_det_sry_atv'){
										$("#publish-unpublish-"+surveyId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+surveyId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='sry_det_sry_itv'){
										$("#publish-unpublish-"+surveyId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+surveyId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+surveyId).html(result[1]);
								}
							});
						},

						publishAndUnpublishSurveyAssessmentQuestions : function(questionId, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'survey-publish-unpublis-dialog',
							    			'esignFor':'publishAndUnpublishSurveyAssessmentQuestions','questionId':questionId,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").publishAndUnpublishQus(questionId, rowObj);
							 }

						},

						publishAndUnpublishQus : function(questionId, rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/survey/survey-assessment-questions/publish-and-unpublish" + '/' + questionId );
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								result = result.split('|');
									if(result[0]=='sry_qtn_sts_atv'){
										$("#publish-unpublish-"+questionId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+questionId).addClass("crs-unpublish-btn");
									}
									else if(result[0]=='sry_qtn_sts_itv'){
										$("#publish-unpublish-"+questionId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+questionId).addClass("crs-publish-btn");
									}
									$("#publish-unpublish-"+questionId).html(result[1]);
								}
							});
						},
			setPositionToCtoolPop : function(path) {
//#custom_attribute_0078975
				// console.log('hello setPositionToCtoolPop');

				if(path.indexOf('group') > 1 || path.indexOf('customattribute') > 1){
					$('#create-dd-list').css('visibility','hidden');
				}
				if(path.indexOf('course-class') > 1 || path.indexOf('program') > 1 || path.indexOf('banner') > 1
						|| path.indexOf('announcement') > 1) {
					var v = $(window).scrollTop();
					if(v < 150) {
						$(window).scrollTop(150);
					}
				}
				// Added by Vincent on 07, Jan 2014 for #0029687: Refresh on Admin Enrollments page
				updatePaginationCookie(1);
				//Custom Attributes for User Registration in Site settings page #custom_attribute_0078975
				setTimeout(function(){
					$('#userregister_scrollbar_container').jScrollPane();
				}, 500);
			},

            publishAndUnpublishContent : function(contentId, rowObj) {
              if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                   var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                       'esignFor':'publishAndUnpublishContent', 'contentId':contentId, 'rowObj': rowObj};
                    $.fn.getNewEsignPopup(esignObj);
              }else{
                $("#root-admin").data("narrowsearch").publishAndUnpublishCnt(contentId, rowObj);
              }

           },

           publishAndUnpublishCnt : function(contentId, rowObj){
             var obj = this;
             url = obj.constructUrl("ajax/administration/manage/content/publish-and-unpublish" + '/' + contentId );
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
               result = $.trim(result);
               result = result.split('|');
                 if(result[0]=='lrn_cnt_sts_atv'){
                   $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                   $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                 }
                 else if(result[0]=='lrn_cnt_sts_itv'){
                   $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                   $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                 }
                 $("#publish-unpublish-"+contentId).html(result[1]);
               }
             });
           },
           
           
           
           publishAndUnpublishVideo : function(contentId, rowObj) {
               if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                    var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                        'esignFor':'publishAndUnpublishVideo', 'contentId':contentId, 'rowObj': rowObj};
                     $.fn.getNewEsignPopup(esignObj);
               }else{
                 $("#root-admin").data("narrowsearch").publishAndUnpublishCntVideo(contentId, rowObj);
               }

            },

            publishAndUnpublishCntVideo : function(contentId, rowObj){
              var obj = this;
              url = obj.constructUrl("ajax/administration/contentauthor/video/publish-and-unpublish" + '/' + contentId );
              $.ajax({
                type: "POST",
                url: url,
                data:  '',
                success: function(result){
                result = $.trim(result);
                result = result.split('|');
                  if(result[0]=='lrn_cnt_sts_atv'){
                    $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                    $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                    $("#share-visibility-"+contentId).removeClass("tab-disable");
                    $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").addClass("share-tab-icon");
                    $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").removeClass("disable-share-tab-icon");
                    $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                    		$("#share-visibility-"+contentId).click(function()
                    		{
                    										callVisibility({'entityId':contentId,
      														 'entityType':'cre_sys_obt_cnt',
      														 'url':'administration/contentauthor/content-share/'+contentId+'/cre_sys_obt_cnt/0',
      														 'popupDispId':'narrow_search_qtip_share_disp__cre_sys_obt_cnt',
      														 'catalogVisibleId':'narrow_search_qtipShareqtip_visible_disp__cre_sys_obt_cnt',
      														 'wid':645,
      														 'heg':'300',
      														 'postype':'middle',
      														 'poslwid':'50',
      														 'linkid':'share-visibility-'+contentId});
      						});
      														 
                  }
                  else if(result[0]=='lrn_cnt_sts_itv'){
                    $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                    $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                    $("#share-visibility-"+contentId).addClass("tab-disable");
                    $("#share-visibility-"+contentId).addClass("tab-disable");
                    $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").addClass("disable-share-tab-icon");
                    $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").removeClass("share-tab-icon");
                    $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                    $("#share-visibility-"+contentId).unbind("click",null);
                  }
                  $("#publish-unpublish-"+contentId).html(result[1]);
                }
              });
            },
            
            
            publishAndUnpublishPresentation : function(contentId, rowObj) {
                if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                     var esignObj = {'popupDiv':'content-publish-unpublis-dialog',
                         'esignFor':'publishAndUnpublishPresentation', 'contentId':contentId, 'rowObj': rowObj};
                      $.fn.getNewEsignPopup(esignObj);
                }else{
                  $("#root-admin").data("narrowsearch").publishAndUnpublishCntPresentation(contentId, rowObj);
                }

             },

            
             
             publishAndUnpublishCntPresentation : function(contentId, rowObj){
                 var obj = this;
                 url = obj.constructUrl("ajax/administration/contentauthor/presentation/publish-and-unpublish" + '/' + contentId );
                 $.ajax({
                   type: "POST",
                   url: url,
                   data:  '',
                   success: function(result){
                   result = $.trim(result);
                   result = result.split('|');
                     if(result[0]=='lrn_cnt_sts_atv'){
                       $("#publish-unpublish-"+contentId).removeClass("crs-publish-btn");
                       $("#publish-unpublish-"+contentId).addClass("crs-unpublish-btn");
                   $("#share-visibility-"+contentId).removeClass("tab-disable");
                      $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").addClass("share-tab-icon");
                      $("#share-visibility-"+contentId).parent().parent().find(".disable-share-tab-icon").removeClass("disable-share-tab-icon");
                      $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                      		$("#share-visibility-"+contentId).click(function()
                      		{
                      										callVisibility({'entityId':contentId,
        														 'entityType':'cre_sys_obt_cnt',
        														 'url':'administration/contentauthor/content-share/'+contentId+'/cre_sys_obt_cnt/0',
        														 'popupDispId':'narrow_search_qtip_share_disp__cre_sys_obt_cnt',
        														 'catalogVisibleId':'narrow_search_qtipShareqtip_visible_disp__cre_sys_obt_cnt',
        														 'wid':645,
        														 'heg':'300',
        														 'postype':'middle',
        														 'poslwid':'50',
        														 'linkid':'share-visibility-'+contentId});
        						});
        								   
                     }
                     else if(result[0]=='lrn_cnt_sts_itv'){
                       $("#publish-unpublish-"+contentId).removeClass("crs-unpublish-btn");
                       $("#publish-unpublish-"+contentId).addClass("crs-publish-btn");
                       $("#share-visibility-"+contentId).addClass("tab-disable");
                       $("#share-visibility-"+contentId).addClass("tab-disable");
                       $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").addClass("disable-share-tab-icon");
                       $("#share-visibility-"+contentId).parent().parent().find(".share-tab-icon").removeClass("share-tab-icon");
                       $("#share-visibility-"+contentId).attr("onclick","javascript:void(0);");
                       $("#share-visibility-"+contentId).unbind("click",null);
                    
                     }
                     $("#publish-unpublish-"+contentId).html(result[1]);
                   }
                 });
               },
              
             
            
            

           publishAndUnpublishTax : function(taxId, rowObj) {
               if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                    var esignObj = {'popupDiv':'tax-publish-unpublis-dialog',
                        'esignFor':'publishAndUnpublishTax', 'taxId':taxId, 'rowObj': rowObj};
                     $.fn.getNewEsignPopup(esignObj);
               }else{
                 $("#root-admin").data("narrowsearch").publishAndUnpublishTaxDialog(taxId, rowObj);
               }

            },

            publishAndUnpublishTaxDialog : function(taxId, rowObj){
              var obj = this;
              url = obj.constructUrl("ajax/administration/commerce/tax/publish-and-unpublish" + '/' + taxId );
              $.ajax({
                type: "POST",
                url: url,
                data:  '',
                success: function(result){
                obj.destroyLoader("paint-narrow-search-results");
                result = $.trim(result);
                result = result.split('|');
                  if(result[0]=='cme_tax_sts_atv'){
                    $("#publish-unpublish-"+taxId).removeClass("crs-publish-btn");
                    $("#publish-unpublish-"+taxId).addClass("crs-unpublish-btn");
                  }
                  else if(result[0]=='cme_tax_sts_itv'){
                    $("#publish-unpublish-"+taxId).removeClass("crs-unpublish-btn");
                    $("#publish-unpublish-"+taxId).addClass("crs-publish-btn");
                  }
                  $("#publish-unpublish-"+taxId).html(result[1]);
                }
              });
            },

            publishAndUnpublishDiscount : function(discountId, rowObj,allowPublishorNot) {
            	allowPublishorNot = (typeof allowPublishorNot == 'undefined') ? 0 : allowPublishorNot;
            	this.currTheme = Drupal.settings.ajaxPageState.theme;

				if(allowPublishorNot == 1){
					var uniqueClassPopup = '';
					 $('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
					    if(this.currTheme == 'expertusoneV2'){
					   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("ERR248")+'</td></tr>';
					    } else {
					     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("ERR248")+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);

					    var closeButt={};
					    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

					    $("#delete-msg-wizard").dialog({
					        position:[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:300,
					        resizable:false,
					        modal: true,
					        title:Drupal.t('LBL749'),
					        buttons:closeButt,
					        closeOnEscape: false,
					        draggable:false,
					        zIndex : 10005,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $("#delete-msg-wizard").show();

						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });

						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
					}else{
						if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		                     var esignObj = {'popupDiv':'discount-publish-unpublis-dialog',
		                         'esignFor':'publishAndUnpublishDiscount', 'discountId':discountId, 'rowObj': rowObj};
		                      $.fn.getNewEsignPopup(esignObj);
		                }else{
		                  $("#root-admin").data("narrowsearch").publishAndUnpublishDiscountDialog(discountId, rowObj);
		                }
					}


             },

             publishAndUnpublishDiscountDialog : function(discountId, rowObj){
               var obj = this;
               url = obj.constructUrl("ajax/administration/commerce/discounts/publish-and-unpublish" + '/' + discountId );
               $.ajax({
                 type: "POST",
                 url: url,
                 data:  '',
                 success: function(result){
                 obj.destroyLoader("paint-narrow-search-results");
                 result = $.trim(result);
                 result = result.split('|');
                   if(result[0] == 1){
                     $("#publish-unpublish-"+discountId).removeClass("crs-publish-btn");
                     $("#publish-unpublish-"+discountId).addClass("crs-unpublish-btn");
                   }
                   else if(result[0] == 0){
                     $("#publish-unpublish-"+discountId).removeClass("crs-unpublish-btn");
                     $("#publish-unpublish-"+discountId).addClass("crs-publish-btn");
                   }
                   $("#publish-unpublish-"+discountId).html(result[1]);
                 }
               });
             },

			 publishAndUnpublishGroupDialog : function(objectId, objectType, rowObj,is_admin,owner_cnt) {
				 	var title = Drupal.t('MSG773');
				 	var suspend = $('#action_icon_tooltip_'+objectId).hasClass( "Suspend" );
				 	if(suspend ){
						var uniqueClassPopup = '';
					    $('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
					   
					    if(this.currTheme == 'expertusoneV2'){
					   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					    } else {
					     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);
	
					    var closeButt={};
					    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	
		                 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		                      var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
								    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
		                      closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
		                 }else{
		                	 closeButt[Drupal.t('Yes')]=function(){ $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj); };
		                 }
	
					    $("#delete-msg-wizard").dialog({
					        position:[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:300,
					        resizable:false,
					        modal: true,
					        title:Drupal.t('Groups'),
					        buttons:closeButt,
					        closeOnEscape: false,
					        draggable:false,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
					    $("#delete-msg-wizard").show();
	
						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });
						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
				    }else{
				    	 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					    	var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
								    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
					    	 $.fn.getNewEsignPopup(esignObj);
						 }else{
							 $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj);
						 }
				    }

           },
             publishAndUnpublishOrder : function(orderId, orderStatus, rowObj) {
					var uniqueClassPopup = '';
				    $('#delete-msg-wizard').remove();
				    var html = '';
				    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
				    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
				    //html+= '<tr><td height="40px">&nbsp;</td></tr>';
				    if(orderStatus == 'payment_received'){
	            		var title = Drupal.t('LBL1163');
	            	}else if(orderStatus == 'canceled'){
	            		var title = Drupal.t('LBL1164');
	            	}
				    if(this.currTheme == 'expertusoneV2'){
				   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
				    } else {
				     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
				    }
				    html+='</table>';
				    html+='</div>';
				    $("body").append(html);

				    var closeButt={};
				    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

	                 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	                      var esignObj = {'popupDiv':'order-publish-unpublis-dialog',
	                          'esignFor':'publishAndUnpublishOrder', 'orderId':orderId, 'orderStatus':orderStatus, 'rowObj': rowObj};
	                      closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
	                 }else{
	                	 closeButt[Drupal.t('Yes')]=function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishOrderDialog(orderId, orderStatus, rowObj); };
	                 }

				    $("#delete-msg-wizard").dialog({
				        position:[(getWindowWidth()-400)/2,200],
				        bgiframe: true,
				        width:300,
				        resizable:false,
				        modal: true,
				        title:Drupal.t('LBL820')+' - '+orderId,
				        buttons:closeButt,
				        closeOnEscape: false,
				        draggable:false,
				        overlay:
				         {
				           opacity: 0.9,
				           background: "black"
				         }
				    });
				    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");
				    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
				    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
				    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
				    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
				    $("#delete-msg-wizard").show();

					$('.ui-dialog-titlebar-close').click(function(){
				        $("#delete-msg-wizard").remove();
				    });
					if(this.currTheme == 'expertusoneV2'){
				    	changeDialogPopUI();
				    }

           },

           publishAndUnpublishOrderDialog : function(orderId, orderStatus, rowObj){
         	$('#delete-msg-wizard').remove();
             var obj = this;
             obj.createLoader("main-wrapper");
             url = obj.constructUrl("administration/commerce/orderupdate/ajax" + '/' + orderId + '/' + orderStatus );
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
             	//obj.destroyLoader("paint-narrow-search-results");
				if (typeof $("#root-admin").data("narrowsearch").refreshLastAccessedRow != 'undefined' && $("#root-admin").data("narrowsearch").refreshLastAccessedRow() == false) {
             	$("#narrow-search-results-holder").trigger("reloadGrid");
               }
               }
             });
           },


            publishAndUnpublishModule : function(moduleName, rowObj) {
            if(moduleName == 'exp_sp_administration_contentauthor' || moduleName == 'exp_sp_administration_customattribute')    // For enabling/disabling content Author
            	$('#delete-msg-wizard').remove();
              if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                   var esignObj = {'popupDiv':'mdoule-publish-unpublis-dialog',
                       'esignFor':'publishAndUnpublishModule', 'moduleName':moduleName, 'rowObj': rowObj};
                    $.fn.getNewEsignPopup(esignObj);
              }else{
                $("#root-admin").data("narrowsearch").publishAndUnpublishModuleDialog(moduleName, rowObj);
              }

           },

           publishAndUnpublishModuleDialog : function(moduleName, rowObj){
             var obj = this;
             this.createLoader("main-wrapper");
             url = obj.constructUrl("ajax/administration/sitesetup/moduleinfo/publish-and-unpublish" + '/' + moduleName );
             var reloadgrid = obj.constructUrl("administration/sitesetup/moduleinfo/search/all");
             $.ajax({
               type: "POST",
               url: url,
               data:  '',
               success: function(result){
               result = $.trim(result);
               result = result.split('|');
                 if(result[0]==1){
                   $("#publish-unpublish-"+moduleName).removeClass("crs-publish-btn");
                   $("#publish-unpublish-"+moduleName).addClass("crs-unpublish-btn");
                   if(moduleName == 'exp_sp_administration_module_info_commerce' || moduleName == 'drupalchat'){
                	   window.location.href=window.location.href;
                   }
                 }
                 else if(result[0]==2){
                   $("#publish-unpublish-"+moduleName).removeClass("crs-unpublish-btn");
                   $("#publish-unpublish-"+moduleName).addClass("crs-publish-btn");
                   if(moduleName == 'exp_sp_administration_module_info_commerce' || moduleName == 'drupalchat'){
                	   window.location.href=window.location.href;
                   }
                 }
                 if(moduleName != 'exp_sp_administration_customattribute')
                 $("#publish-unpublish-"+moduleName).html(result[1]);
               //  $("#root-admin").data("narrowsearch").destroyLoader("main-wrapper");
               //  $("#root-admin").data("narrowsearch").createLoader("paint-narrow-search-results");
                 $('#narrow-search-results-holder').setGridParam({url:reloadgrid});
				 if (typeof $("#root-admin").data("narrowsearch").refreshLastAccessedRow != 'undefined' && $("#root-admin").data("narrowsearch").refreshLastAccessedRow() == false) {
				 	$('#narrow-search-results-holder').trigger("reloadGrid",[{page:$(".ui-pg-input").val()}]);
				 }
               }
             });
           },

           publishAndUnpublishPayment : function(paymentId, rowObj) {
             if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
                  var esignObj = {'popupDiv':'payment-publish-unpublis-dialog',
                      'esignFor':'publishAndUnpublishPayment', 'paymentId':paymentId, 'rowObj': rowObj};
                   $.fn.getNewEsignPopup(esignObj);
             }else{
               $("#root-admin").data("narrowsearch").publishAndUnpublishPaymentDialog(paymentId, rowObj);
             }

          },

          publishAndUnpublishPaymentDialog : function(paymentId, rowObj){
            var obj = this;
            this.createLoader("admin-commerce-payment-method-plan");
            url = obj.constructUrl("administration/commerce/setting/payment/action/"+paymentId);
            $.ajax({
              type: "POST",
              url: url,
              data:  '',
              success: function(result){
              result = $.trim(result);
              result = result.split('|');
                if(result[0]==1){
                  $("#publish-unpublish-"+paymentId).removeClass("crs-publish-btn");
                  $("#publish-unpublish-"+paymentId).addClass("crs-unpublish-btn");
                }

                else if(result[0]==0){
                  $("#publish-unpublish-"+paymentId).removeClass("crs-unpublish-btn");
                  $("#publish-unpublish-"+paymentId).addClass("crs-publish-btn");
                }
                $("#publish-unpublish-"+paymentId).html(result[1]);
                $("#root-admin").data("narrowsearch").destroyLoader("admin-commerce-payment-method-plan");

              }
            });
          },

           publishAndUnpublishBanner : function(bannerId, bannerType, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'banner-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishBanner','bannerId':bannerId,'bannerType':bannerType,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishBan(bannerId, bannerType, rowObj);
				 }

			},
//#custom_attribute_0078975
			publishAndUnpublishCustom : function(CustomId, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'Custom-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishCustom','CustomId':CustomId,'rowObj': rowObj};
				    	// console.log('esignObj'+JSON.stringify(esignObj));
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishCus(CustomId, rowObj);
				 }

			},

			publishAndUnpublishAnnouncement : function(annId, annType, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'announcement-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishAnnouncement','annId':annId,'annType':annType,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishAnn(annId, annType, rowObj);
				 }

			},
			publishAndUnpublishAnn : function(annId, annType, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/announcement/publish-and-unpublish" + '/' + annId + '/' + annType);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cre_sys_obt_not_atv'){
							$("#publish-unpublish-"+annId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+annId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_sys_obt_not_itv'){
							$("#publish-unpublish-"+annId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+annId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+annId).html(result[1]);
					}
				});
			},
			publishAndUnpublishBan : function(bannerId, bannerType, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/banner/publish-and-unpublish" + '/' + bannerId + '/' + bannerType);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cbn_anm_sts_atv'){
							$("#publish-unpublish-"+bannerId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+bannerId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cbn_anm_sts_itv'){
							$("#publish-unpublish-"+bannerId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+bannerId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+bannerId).html(result[1]);
					}
				});
			},
//#custom_attribute_0078975
			publishAndUnpublishCus : function(CustomId, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/customattribute/publish-and-unpublish" + '/' + CustomId );
				

				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');

						if(result[0]=='cre_cattr_sts_atv'){
							

							$("#publish-unpublish-"+CustomId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+CustomId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_cattr_sts_itv'){
							

							$("#publish-unpublish-"+CustomId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+CustomId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+CustomId).html(result[1]);
					}
				});
			},
			publishAndUnpublishLocation : function(locationId, rowObj) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'location-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishLocation','locationId':locationId,'rowObj': rowObj};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishLoc(locationId, rowObj);
				 }

			},

			publishAndUnpublishLoc : function(locationId, rowObj){
				var obj = this;
				url = obj.constructUrl("ajax/administration/manage/location/publish-and-unpublish" + '/' + locationId );
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						result = $.trim(result);
						result = result.split('|');
						if(result[0]=='not_able_do'){
							this.currTheme = Drupal.settings.ajaxPageState.theme;

							var show =	$.trim($('#publish-unpublish-'+locationId).html());
							var uniqueClassPopup = 'unique-delete-class';
							var wSize = (wSize) ? wSize : 300;
						    $('#delete-msg-wizard').remove();
						    var html = '';
						    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
						    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

						   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
						    if(this.currTheme == 'expertusoneV2'){
						   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("The location is associated to class")+'</td></tr>';
						    } else {
						     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("The location is associated to class")+'</td></tr>';
						    }
						    html+='</table>';
						    html+='</div>';
						    $("body").append(html);
						    var closeButt={};
						    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};


						    $("#delete-msg-wizard").dialog({
						        position:[(getWindowWidth()-400)/2,200],
						        bgiframe: true,
						        width:wSize,
						        resizable:false,
						        modal: true,
						        title:result[1],//"title",
						        buttons:closeButt,
						        closeOnEscape: false,
						        draggable:false,
						        overlay:
						         {
						           opacity: 0.9,
						           background: "black"
						         }
						    });
						    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
						    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

						    $("#delete-msg-wizard").show();

							$('.ui-dialog-titlebar-close').click(function(){
						        $("#delete-msg-wizard").remove();
						    });
							if(this.currTheme == 'expertusoneV2'){
						    	changeDialogPopUI();
						    }
							/*-- 37211: Unable to delete a class in FF version 32.0 --*/
						    if($('div.qtip-defaults').length > 0) {
						    	var prevZindex = $('.qtip-defaults').css('z-index');
						    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
						    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
						    }
						}else{
							if(result[0]=='lrn_res_loc_atv'){
								$("#publish-unpublish-"+locationId).removeClass("crs-publish-btn");
								$("#publish-unpublish-"+locationId).addClass("crs-unpublish-btn");
							}
							else if(result[0]=='lrn_res_loc_itv'){
								$("#publish-unpublish-"+locationId).removeClass("crs-unpublish-btn");
								$("#publish-unpublish-"+locationId).addClass("crs-publish-btn");
							}
							$("#publish-unpublish-"+locationId).html(result[1]);
						}
					}
				});
			},


			publishAndUnpublishNotification : function(notificationId, notificationType) {
				 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'notification-publish-unpublis-dialog',
				    			'esignFor':'publishAndUnpublishNotification','notificationId':notificationId,'notificationType':notificationType};
				    	 $.fn.getNewEsignPopup(esignObj);
				 }else{
					 $("#root-admin").data("narrowsearch").publishAndUnpublishNot(notificationId, notificationType);
				 }

			},

			publishAndUnpublishNot : function(notificationId, notificationType){
				var obj = this;
				if(notificationType == 'notification_template'){
				  url = obj.constructUrl("ajax/administration/manage/notification_template/publish-and-unpublish" + '/' + notificationId );
				}else {
				  url = obj.constructUrl("ajax/administration/manage/certificate/publish-and-unpublish" + '/' + notificationId );
				}
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
					result = $.trim(result);
					result = result.split('|');
						if(result[0]=='cre_ntn_sts_atv'){
							$("#publish-unpublish-"+notificationId).removeClass("crs-publish-btn");
							$("#publish-unpublish-"+notificationId).addClass("crs-unpublish-btn");
						}
						else if(result[0]=='cre_ntn_sts_itv'){
							$("#publish-unpublish-"+notificationId).removeClass("crs-unpublish-btn");
							$("#publish-unpublish-"+notificationId).addClass("crs-publish-btn");
						}
						$("#publish-unpublish-"+notificationId).html(result[1]);
					}
				});
			},




						activateAndDeactivateResource : function(resourceId, resourceType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'program-publish-unpublis-dialog',
							    			'esignFor':'ActivateDeactivateResource','resourceId':resourceId,'resourceType':resourceType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").activateAndDeactivateRsc(resourceId, resourceType, rowObj);
							 }

						},

						activateAndDeactivateRsc : function(resourceId, resourceType,rowObj){
							var obj = this;
							url = obj.constructUrl("ajax/administration/learning/resource/activate-and-deactivate" + '/' + resourceId+ '/' + resourceType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
									if(result=='lrn_res_loc_atv' || result=='lrn_res_fac_atv' || result=='lrn_res_rms_atv'){
										$("#publish-unpublish-"+resourceType+"-"+resourceId).removeClass("crs-publish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).addClass("crs-unpublish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).html("Deactivate");
									}
									else if(result=='lrn_res_loc_itv' || result=='lrn_res_fac_itv' || result=='lrn_res_rms_itv'){
										$("#publish-unpublish-"+resourceType+"-"+resourceId).removeClass("crs-unpublish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).addClass("crs-publish-btn");
										$("#publish-unpublish-"+resourceType+"-"+resourceId).html("Activate");
									}
								}
							});
						},

						activateAndDeactivateObject : function(objectId, objectType, rowObj) {
							 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
							    	var esignObj = {'popupDiv':'object-activate-deactivate-dialog',
							    			'esignFor':'ActivateDeactivateObject','objectId':objectId,'objectType':objectType,'rowObj': rowObj};
							    	 $.fn.getNewEsignPopup(esignObj);
							 }else{
								 $("#root-admin").data("narrowsearch").activateAndDeactivateObj(objectId, objectType, rowObj);
							 }

						},

						activateAndDeactivateObj : function(objectId, objectType, rowObj){
							//alert(objectType);
							var obj = this;
							if(objectType=='User'){
								url = obj.constructUrl("ajax/administration/people/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='Organization'){
								url = obj.constructUrl("ajax/administration/people/organization/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='Grp'){
								$("#delete-msg-wizard").remove();
								url = obj.constructUrl("ajax/administration/people/group/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='location'){
								url = obj.constructUrl("ajax/administration/location/activate-and-deactivate" + '/' + objectId);
							}
							else if(objectType=='surveyDetails'){
								url = obj.constructUrl("ajax/administration/survey/survey-assessment/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='assessmentDetails'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='surveyQuestion'){
								url = obj.constructUrl("ajax/administration/survey/survey-assessment-questions/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							else if(objectType=='assessmentQuestion'){
								url = obj.constructUrl("ajax/administration/assessment/survey-assessment-questions/publish-and-unpublish" + '/' + objectId + '/'+objectType);
							}
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								if(objectType=='User' && result=='activeinstr'){
									var error = new Array();
									error[0] = Drupal.t("MSG768");
									$('#show_suspend_message').remove();
							 	    var message = "<div id='show_suspend_message' style='position:absolute;left:0;right:0;'></div></div>";
							 	    var msg='<div id="message-container" style="visibility:visible"><div class="messages error"><ul><li><span>'+error[0]+'</span></li></ul><div class="msg-close-btn" onclick="$(\'#show_suspend_message\').remove();"></div></div></div><img onload="$(\'body\').data(\'learningcore\').showHideMultipleLi();" style="display:none;" src="sites/all/themes/core/expertusone/expertusone-internals/images/close.png" height="0" width="0"/>';
							 	    $('#gview_narrow-search-results-holder').prepend(message);
							 	    $("#gview_narrow-search-results-holder #show_suspend_message").append(msg);
								}else{
									$("#suspend_activate_"+objectId).html(result);
									//alert("objectId" + objectId + "result" + result);
									//44133: German-old-ui-Icon not changing when user is activate and suspend
									if(result==Drupal.t('LBL573')){
										$("#action_icon_tooltip_"+objectId).removeClass('Suspend');
										$("#action_icon_tooltip_"+objectId).addClass('Activate');

									}else{
										$("#action_icon_tooltip_"+objectId).removeClass('Activate');
										$("#action_icon_tooltip_"+objectId).addClass('Suspend');
									}
									if(objectType=='User'){
										$("#action_icon_tooltip_"+objectId).attr('title', result);
									}else{
										$("#suspend_activate_"+objectId).parent('span').attr('title', result);
									}
								}
								}
							});
						},

						initAdminLinks : function() {
							var curObj = this;
							var loadingDiv = "root-admin-search-right-col";

							/* Hide the filter and top search text box,result area */
							$("#narrow-search-text-filter-container").hide();
							$("#narrow-search").hide();
							$("#root-admin-results").hide();
							$("ul.AdminsublinktabNavigation li").removeClass("selected");

							// For Left Navigation Admin Menu Links
							$("ul.list-item-administrator li a").click(function(event,mainMenuId) {

									var linkId;
									if(mainMenuId=="" || mainMenuId=="undefined" || mainMenuId==null){
										 linkId = $(this).attr("id");
									}else{
										 linkId=mainMenuId;
									}
									var ancData = $("#"+linkId).metadata();
									var linkPath = ancData.link_path;

									/* Hides/Show narrow search and text filter */
									$("#narrow-search-text-filter-container").hide();
									$("#narrow-search").hide();
									$("#narrow-search-actionbar-list").hide();
									$("#root-admin-results").hide();

									curObj.createLoader(loadingDiv);

									var url = '?q=ajax/get/modules/'+linkPath;
									$.ajax({
											type : "POST",
											url : url,
											data : '',
											datatype : 'json',
											success : function(data) {
												curObj.destroyLoader(loadingDiv);

												$("#root-admin-results").html(data);
												$("#root-admin-results").show();
												var contentLoadingDiv = 'root-admin-results';

												/* For top tabular submenu in result page.*/
												$("ul.AdminsublinktabNavigation li a").click(function(event,firstSubmenuId){
													if($('#narrow-search-actionbar')){
													  $('#narrow-search-actionbar').hide();
													}

												  $("#narrow-search-text-filter-container").hide();
												  $("#narrow-search").hide();
												  $("#tab-content-main").html('');

													var submenuId;
													if(firstSubmenuId=="" || firstSubmenuId=="undefined" || firstSubmenuId==null){
														 submenuId = $(this).attr("id");
													}else{
														submenuId=firstSubmenuId;
													}

													var subAncData = $("#"+submenuId).metadata();

													/* Add class "selected"  if select  the tab */
													$( "ul.AdminsublinktabNavigation li").removeClass("selected");
													$("#"+ submenuId).parent("li").addClass("selected");

													/* Start - To get the submodules according to tab selection */
													curObj.createLoader(contentLoadingDiv);

													var sublinkPath=subAncData.link_path;
													var sublinkUrl = '?q='+sublinkPath;

													$.ajax({
														type : "POST",
														url : sublinkUrl,
														data : '',
														datatype : 'json',
														success : function(data) {
													     curObj.destroyLoader(contentLoadingDiv);
													     //console.log('ajax success data');
													     //console.log(data);
									   				   var detailsObj = jQuery.parseJSON(data);
									   				   //console.log('ajax success detailsObj');
                               //console.log(detailsObj);
											               // Extend Drupal.settings, if any settings are present in the data
												           if (detailsObj.drupal_settings) {
														     $.extend(true, Drupal.settings, detailsObj.drupal_settings);
														   }
														   /*Display the content for Tab wheather narrow search or non narrow search */
														   $("#tab-content-main").html(detailsObj.rendered_tab_content_main);

														   if(detailsObj.rendered_tab_content_type == "narrow_search"){ /* If narrow search */
															   /* Display left side filters and top bar*/
															   $("#narrow-search").html(detailsObj.rendered_narrow_search_filters);
															   $("#narrow-search").show();

															   // curObj.renderSearchResults();

															   $("#narrow-search-results-topbar").html(detailsObj.rendered_narrow_search_sortbar);
															   $("#narrow-search-actionbar").remove();
															   $("#admin-maincontent_tab").append(detailsObj.rendered_narrow_search_actionbar);
															   $("#narrow-search-results-topbar").show();

															   /* Initialise filters once modules loaded */
															   curObj.initPaths(detailsObj.rendered_tab_content_type);
															   curObj.initFilters();
															   curObj.renderSearchResults(linkPath,sublinkPath);
															   if($('#narrow-search-actionbar')){
													  			 $('#narrow-search-actionbar').hide();
															   }
														   }
														   else if (detailsObj.rendered_tab_content_type == "ajax-form"){
	                               Drupal.attachBehaviors();
														   }
													   } // end success
													}); /* End - To get the submodules according to tab selection */

													/* Show  the text filter */
													$("#narrow-search-actionbar-list").show();
												});

												/* Auto load first sub menu */
												var tmpFirstSubmenuId=$("ul.AdminsublinktabNavigation li a:first").attr("id");
												if(tmpFirstSubmenuId!="" && tmpFirstSubmenuId!="undefined" && tmpFirstSubmenuId!=null){
													 $("#"+tmpFirstSubmenuId).trigger("click",[tmpFirstSubmenuId]);
												}

											  }
										});

									/* Highlight selected menu links */
									$("ul.list-item-administrator li a").removeClass("root-admin-links-selected");
									$("#"+linkId).addClass("root-admin-links-selected");

									/* THE BELOW LINES WRITTEN BY NEW THEME BY VJ*/
									$("ul.list-item-administrator li.admin-left-panel-module-list").removeClass("root-admin-module-selected");
									$("#"+linkId).parent().addClass("root-admin-module-selected");

							 });

							/* Auto load main menu */
							var tmpFirstMainMenuId=$("ul.list-item-administrator li a:first").attr("id");
							//var tmpFirstMainMenuId="collaboration";
							if(tmpFirstMainMenuId!="" && tmpFirstMainMenuId!="undefined" && tmpFirstMainMenuId!=null){
							  $("#"+tmpFirstMainMenuId).trigger("click",[tmpFirstMainMenuId]);
							}

						},

						abstractFunctionAlert : function(functionName) {
							alert('UNDER CONSTRUCTION. Please implement function ' + functionName + ' in admin sub-module\'s js file.');
						},

						missingInitParamAlert : function(paramName) {
							alert('UNDER CONSTRUCTION. Missing init data ' + paramName);
						},

						narrowSearch : function() {
							var curObj=this;
							this.refreshGrid();
						},

						sortSearchResults : function(sortType, sortLinkHtmlId) {
							this.refreshGrid(sortType, sortLinkHtmlId);
						},

						narrowSearchByText : function() {
							this.refreshGrid();
						},

						narrowSearchByAddlText : function(code) {
							this.refreshGrid();
						},

						narrowSearchByDateRange : function(code) {
							this.refreshGrid();
						},

						narrowSearchBySlider : function(code) {
							this.refreshGrid();
						},

						printSearchResults : function() {
							if (this.printPath == undefined || this.printPath == null || this.printPath == '') {
								this.missingInitParamAlert('print_path');
								return; // Impt
							}
							window.location = this.constructUrl(this.printPath + this.getNarrowSearchURLArgs(this.lastSortType,this.lastSortTypeHtmlId));
						},

						exportSearchResults : function() {
							if (this.exportPath == undefined || this.exportPath == null || this.exportPath == '') {
								this.missingInitParamAlert('export_path');
								return; // Impt
							}

							window.location = this.constructUrl(this.exportPath + this.getNarrowSearchURLArgs(this.lastSortType,this.lastSortTypeHtmlId));
						},
//#custom_attribute_0078975						
						downloadSampleUserupload : function() {
							window.location = this.constructUrl("administration/userfeed/download/csv");
                        },

						addItem : function() {
							this.abstractFunctionAlert('addItem');
							return false;
						},

						editItem : function() {
							this.abstractFunctionAlert('editItem');
							return false;
						},

						showHide : function(strOne, strTwo) {
							$('#' + strTwo).toggle();
							var classShowHide = $('#' + strOne).hasClass(
									'cls-show');
							if (classShowHide) {
								$('#' + strOne).removeClass('cls-show');
								$('#' + strOne).addClass('cls-hide');
							} else {
								$('#' + strOne).removeClass('cls-hide');
								$('#' + strOne).addClass('cls-show');
							}
						},

						showHideCourseClass : function(type){
							if (type == 'Class') {
								$('#catalogcoursestatus_container').hide();
								$('#courselangtype_container').hide();
								$('#catalogcoursetag_container').hide();
								$('#catalogcoursemanageby_container').hide();
								$('#classhiddenmanageby_container').hide();
								$('#catalogcoursecurrency_container').hide();
								$('#coursePrice_container').hide();

								$('#catalogclassstatus_container').show();
								$('#classdeliverytype_container').show();
								$('#classPrice_container').show();
								$('#classdaterange_container').show();
								$('#classlangtype_container').show();
								$('#catalogclasstag_container').show();
								/*$('.tagscloud-scrollable:not(.jspScrollable)').each(function () {
									if ($(this).is(':visible')) {
									  var tagsHeight = $(this).height();
									  if (tagsHeight > 175) {
									    $(this).height(175);
										$(this).css('max-height', '175px');
									    $(this).jScrollPane({});
								      }
									}
								});*/

								$('#classLocation_container').show();
								$('#classInstructor_container').show();

								$('#classSurvey_container').show();
								$('#classAssessment_container').show();
								$('#classContent_container').show();
								$('#catalogclassmanageby_container').show();
								$('#catalogclasscurrency_container').show();
								$('#tagentityType').val('tagtipclass');

							} else {
								$('#catalogcoursestatus_container').show();
								$('#courselangtype_container').show();
								$('#catalogcoursetag_container').show();
								$('#catalogcoursemanageby_container').show();
								$('#catalogcoursecurrency_container').show();
								$('#coursePrice_container').show();

								$('#catalogclassstatus_container').hide();
								$('#classdeliverytype_container').hide();
								$('#classPrice_container').hide();
								$('#classdaterange_container').hide();
								$('#classlangtype_container').hide();
								$('#catalogclasstag_container').hide();
								$('#classLocation_container').hide();
								$('#classInstructor_container').hide();
								$('#catalogclassmanageby_container').hide();
								$('#catalogclasscurrency_container').hide();

								$('#classSurvey_container').hide();
								$('#classAssessment_container').hide();
								$('#classContent_container').hide();
								$('#tagentityType').val('tagtip');
							}
						},

						renderSearchResults : function(curModule,subModule) {
						  //console.log (curModule);
						  //console.log (subModule);
							var curObj = this;
							if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
								var gridWidth 		= 720;
								var detailsWidth 	= 520;
								var actionWidth 	= 150;
								var iconsWidth 		= 74;
							}else{
								var detailsWidth 	= 595;
								var actionWidth 	= 130;
								var gridWidth 		= 770;
								var iconsWidth 		= 54;
							}
							//Condition for commerce module showindg search widget in JQgrid table format
							if(curModule == 'administration/learning' || subModule == 'administration/manage/content' || subModule == 'administration/contentauthor/video' || subModule == 'administration/contentauthor/presentation' ||  subModule == 'administration/manage/banner'  || subModule == 'administration/sitesetup/moduleinfo'
								|| subModule == 'administration/commerce/order' || subModule == 'administration/manage/announcement') {
								var tmp_html=$('#narrow-search-results-holder').html();
								if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
									$('#narrow-search-results-holder').html("");
								}
								if($('#narrow-search-results-holder').html().length <= 0) {
									//("renderSearchResults");
									curObj.createLoader("main-wrapper");
								
									var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';
									//admininister page jqgrid declaration here
									$("#narrow-search-results-holder").jqGrid( {
										url : curObj.constructUrl(curObj.searchBasePath),
										datatype : "json",
										mtype : 'GET',
										colNames : [ 'Icons','Details', 'Action' ],
										//colNames : [ 'Details' ],
										colModel : [ {
											name : 'Icons',
											index : 'Icons',
											classes:'rowpaddTB',
											title : false,
											width : iconsWidth,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResultsIcons
										},  {
											name : 'Details',
											index : 'Details',
											classes:'rowpaddTB',
											title : false,
											width : detailsWidth ,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResults
										}, {
											name : 'Action',
											index : 'Action',
											classes:'rowpaddTB',
											title : false,
											width : actionWidth,
											'widgetObj' : curObjStr,
											formatter : curObj.paintSearchResultsAction
										} ],
										rowNum : 10,
										rowList : [ 10, 25, 50 ],
										pager : '#narrow-search-results-pager',
										viewrecords : true,
										emptyrecords : "",
										sortorder : "desc",
										toppager : true,
										height : 'auto',
										width : gridWidth,
										loadtext : "",
										recordtext : "",
										pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
										loadui : false,
										userdata: "userdata",
										loadComplete : curObj.loadComplete
									});
									/* .navGrid('#narrow-search-results-pager', {
										add : false,
										edit : false,
										del : false,
										search : false,
										refreshtitle : true
									}); */
									// curObj.hidePageControls(true); // show in loadComplete()
								}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						} else if((curModule == 'administration/people' && subModule != 'administration/people/organization'&&
						            subModule != 'administration/people/group')  ||
						               subModule == 'administration/commerce/setting'  ||  subModule == 'administration/people/setup' ||  subModule == 'administration/manage/announcement' ||  subModule == 'administration/sitesetup/config') {
              				var iconsWidth = 38;
              				if (subModule == 'administration/commerce/setting' || subModule == 'administration/people/setup' || subModule == 'administration/sitesetup/config') {
                				iconsWidth = 80;
                				detailsWidth = 440;
              				}
              			
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
									var iconsWidth 		= 50;
								}
								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									colNames : [ 'Icons','Details'],
									//colNames : [ 'Details' ],
									colModel : [ {
										name : 'Icons',
										index : 'Icons',
										classes:'rowpaddTB',
										title : false,
										width : iconsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsIcons
									},  {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								}); */
								// curObj.hidePageControls(true); // show in loadComplete()
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
//#custom_attribute_0078975
						else if(curModule == 'administration/survey' || subModule == 'administration/survey/surveydetails' || 
								subModule == 'administration/assessment/assessmentdetails' || subModule == 'administration/assessment/assessmentquestions' || 
								subModule == 'administration/survey/surveyquestions' || subModule == 'administration/contentauthor/video' 
									||subModule == 'administration/contentauthor/presentation' 
										||subModule == 'administration/manage/location'
										|| subModule == 'administration/manage/customattribute' || subModule == 'administration/manage/notification_template' || subModule == 'administration/manage/certificate'
							  || subModule == 'administration/commerce/tax' || subModule == 'administration/commerce/discounts'){
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}

							if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
								var detailsWidth 	= 600;
								var actionWidth 	= 150;
							}else{
								var detailsWidth 	= 655;
								var actionWidth 	= 130;
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									colNames : [ 'Details', 'Action' ],
									colModel : [ {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}, {
										name : 'Action',
										index : 'Action',
										classes:'rowpaddTB',
										title : false,
										width : actionWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsAction
									}],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								});
								curObj.hidePageControls(true); // show in loadComplete() */
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
						else {
							var tmp_html=$('#narrow-search-results-holder').html();
							if(tmp_html=="<TBODY></TBODY>" || tmp_html=="<tbody></tbody>"){
								$('#narrow-search-results-holder').html("");
							}
							if($('#narrow-search-results-holder').html().length <= 0) {
								//("renderSearchResults");
								curObj.createLoader("main-wrapper");
								var curObjStr = '$("#' + curObj.widgetInitId + '").data("' + curObj.widgetName + '")';

								$("#narrow-search-results-holder").jqGrid( {
									url : curObj.constructUrl(curObj.searchBasePath),
									datatype : "json",
									mtype : 'GET',
									//colNames : [ 'Details', 'Action' ],
									colNames : [ 'Details' ],
									colModel : [ {
										name : 'Details',
										index : 'Details',
										classes:'rowpaddTB',
										title : false,
										width : detailsWidth,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResults
									}/*, {
										name : 'Action',
										index : 'Action',
										title : false,
										width : 130,
										'widgetObj' : curObjStr,
										formatter : curObj.paintSearchResultsAction
									}*/ ],
									rowNum : 10,
									rowList : [ 10, 25, 50 ],
									pager : '#narrow-search-results-pager',
									viewrecords : true,
									emptyrecords : "",
									sortorder : "desc",
									toppager : true,
									height : 'auto',
									width : gridWidth,
									loadtext : "",
									recordtext : "",
									pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
									loadui : false,
									loadComplete : curObj.loadComplete
								});
								/* .navGrid('#narrow-search-results-pager', {
									add : false,
									edit : false,
									del : false,
									search : false,
									refreshtitle : true
								});
								curObj.hidePageControls(true); // show in loadComplete() */
							}else{
								curObj.refreshGrid();
								if($('#narrow-search-actionbar')){
								  $('#narrow-search-actionbar').show();
								}
							}
						}
					},

						paintSearchResults : function(cellvalue, options,rowObject) {
							return rowObject['details'];
						},

						paintSearchResultsAction : function(cellvalue, options,rowObject) {
							return rowObject['action'];
						},
						paintSearchResultsIcons : function(cellvalue, options,rowObject) {
							return rowObject['image'];
						},

						/* hidePageControls : function(hideAll) {
						//Ayyappan
						console.log('hide pager heer');
						  var lastDataRow = $('#narrow-search-results-holder tr.ui-widget-content').filter(":last");
						  //console.log(lastDataRow.length);

              // if (hideAll) {
                //$('#narrow-search-results-pager').hide();
                 if(this.currTheme == "expertusoneV2")
    	  			$('#root-admin-results .block-footer-left').show();
      				$('#narrow-search-results-pager').hide();

                //remove bottom dotted border from the last row in result if records are present
                if (lastDataRow.length != 0) {
                  //console.log('hidePageControls() : hideAll - last data row found');
                  lastDataRow.children('td').css('border', '0 none');
                }
              }
              else {
              	$('#narrow-search-results-pager').show();
                //console.log('hidePageControls() : hide only next/prev page control');
                if(this.currTheme == "expertusoneV2")
                $('#root-admin-results .block-footer-left').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #first_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #last_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #next_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #prev_narrow-search-results-pager').hide();
                $('#narrow-search-results-pager #narrow-search-results-pager_center #sp_1_narrow-search-results-pager').parent().hide();
                //if (lastDataRow.length != 0) {
                  //add bottom dotted border from the last row in result if records are present
                  //lastDataRow.children('td').css('border-bottom', '1px dotted #CCCCCC');
                //}
              }
						}, */

	showPageControls : function() {
	  //console.log('showPageControls() : show all control');
	  $('#narrow-search-results-pager').show();
	  if(this.currTheme == "expertusoneV2")
	  $('#root-admin-results .block-footer-left').hide();
	  $('#narrow-search-results-pager #narrow-search-results-pager_center #first_narrow-search-results-pager').show();
	  $('#narrow-search-results-pager #narrow-search-results-pager_center #last_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #next_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #prev_narrow-search-results-pager').show();
      $('#narrow-search-results-pager #narrow-search-results-pager_center #sp_1_narrow-search-results-pager').parent().show();
             // issue In Pagination Width
      if(this.currTheme == "expertusone")
    	  $('#narrow-search-results-pager #narrow-search-results-pager_center').removeAttr("style");
      //add bottom dotted border from the last row in result if records are present
      //var lastDataRow = $('#narrow-search-results-holder tr.ui-widget-content').filter(":last");
      //if (lastDataRow.length != 0) {
        //lastDataRow.children('td').css('border-bottom', '1px dotted #CCCCCC');
      //}
				},

						loadComplete : function(response, postdata, formid, updateShowMore) {
							var curObj=this;
							curObj.widgetInitId="root-admin";
							curObj.widgetName="narrowsearch";

							Drupal.attachBehaviors(); // for initializing ctools.
							$('#narrow-search-results-holder').show();
							//alert("Load Complete");
							if($('#narrow-search-actionbar')){
							  $('#narrow-search-actionbar').show();
							}
							//38455: Managed by filters me is not checked when login as multilanguage
							// Managed By behavior
							var userData = $('#narrow-search-results-holder').jqGrid('getGridParam', 'userData');
				              if (typeof userData.managedBy != 'undefined' && userData.managedBy != '') {
				                var managedByCheckboxId = '#managed_by_1 input';	//change by ayyappans for 0040223: See more link is not working in narrow filter in user grid page
				                if (typeof userData.adminPage != 'undefined' && userData.adminPage == 'class') {
				                  managedByCheckboxId = '#managed_by_1 input';
				                }

				                if (typeof userData.adminPage != 'undefined' && userData.adminPage == 'class') {
				                  $('#checkbox_clspgopened').attr('checked', 'checked');
				                  $('#checkbox_clspgopened').parent().removeClass('narrow-search-filterset-checkbox-unselected');
				                  $('#checkbox_clspgopened').parent().addClass('narrow-search-filterset-checkbox-selected');
				                }

				                if (userData.managedBy == 'me') {
				                  $(managedByCheckboxId).attr('checked', 'checked');
				                  $(managedByCheckboxId).parent().removeClass('narrow-search-filterset-checkbox-unselected');
				                  $(managedByCheckboxId).parent().addClass('narrow-search-filterset-checkbox-selected');
				                  $('#managed_by_1').children('.narrow-search-filterset-item-label-unselected').removeClass('narrow-search-filterset-item-label narrow-search-filterset-item-label-unselected').addClass('narrow-search-filterset-item-label narrow-search-filterset-item-label-selected');
				                }
				              }

							var numRecords = parseInt($("#narrow-search-results-holder").getGridParam("records"), 10);
							//console.log('loadComplete() : numRecords = ' + numRecords);
							if (numRecords == 0) {
								$('#narrow-search-no-records').show();
							} else {
								$('#narrow-search-no-records').hide();
							}

							// Show pagination only when search results span multiple pages
			              var recordsPerPage = parseInt($("#narrow-search-results-holder").getGridParam("reccount"), 10);
			              //console.log('loadComplete() : recordsPerPage = ' + recordsPerPage);

			              var hideAllPageControls = true;
			              if (numRecords > 10) { // 10 is the least view per page option.
			                hideAllPageControls = false;
			                //console.log('loadComplete() : hideAllPageControls set to false');
			                $("#page-container #narrow-search-results-holder tr.ui-widget-content:last-child > td.rowpaddTB").css("padding-bottom","9px");
			              }

			              /* if (numRecords <= recordsPerPage) {
			               //console.log('loadComplete() : numRecords <= recordsPerPage : hide pagination controls');
			                $('#' + curObj.widgetInitId).data(curObj.widgetName).hidePageControls(hideAllPageControls);
			              }
			              else {
			                //console.log('loadComplete() : numRecords <= recordsPerPage : show pagination controls including view per page control');
			                $('#' + curObj.widgetInitId).data(curObj.widgetName).showPageControls();
			              } */
							//$('#' + curObj.widgetInitId).data(curObj.widgetName).initGrid('#' + curObj.widgetInitId,curObj.widgetName);
							if ($('#' + curObj.widgetInitId).data(curObj.widgetName).firstTime) {
								$("#narrow-search-filters").show();
								$("#narrow-search-results-topbar").show();
								$("#" + response.initial_sort_type_html_id).addClass('sortype-high-lighter');
								$('#' + curObj.widgetInitId).data(curObj.widgetName).firstTime = false;
								$('#' + curObj.widgetInitId).data(curObj.widgetName).initGrid('#' + curObj.widgetInitId,curObj.widgetName);
							}
							else{
								if($('#' + curObj.widgetInitId).data(curObj.widgetName).lastSortTypeHtmlId)
								    $("#" + $('#' + curObj.widgetInitId).data(curObj.widgetName).lastSortTypeHtmlId).addClass('sortype-high-lighter');
								else
									$("#" + response.initial_sort_type_html_id).addClass('sortype-high-lighter');
							}
							$('#' + curObj.widgetInitId).data(curObj.widgetName).destroyLoader('paint-narrow-search-results');
							$('#' + curObj.widgetInitId).data(curObj.widgetName).destroyLoader('main-wrapper');
								//Vtip-Display toolt tip in mouse over
//							$('.fade-out-title-container').each(function() {
//								//console.log($(this).width());
//								//console.log($(this).find('.title-lengthy-text').width());
//								if($(this).width() >= $(this).find('.title-lengthy-text').width()) {
//									$(this).find('.fade-out-image').remove();
//								}
//								//title-lengthy-text
//							});
								 vtip();
								 resetFadeOutByClass('#narrow-search-results-holder','narrow-search-results-item-detail','vtip','admin'); /*Viswanathan added for #76811*/
							$('.limit-title').trunk8(trunk8.admin_title);
					 		$('.limit-desc').trunk8(trunk8.admin_desc);
						//	Drupal.attachBehaviors();
							// if ($("#paint-narrow-search-results").data('showmore') === undefined) {
							updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);

							if(updateShowMore) {
								$("#paint-narrow-search-results").showmore({
									showAlways: true,
									'grid': '#narrow-search-results-holder',
									'gridWrapper': '#main-wrapper',
									'showMore': '#admin-narrow-search-show-more'
								});
							}
								//bind onclick event for the ajax edit narrow search result icon
								$('#narrow-search-results-holder tr.jqgrow:not(.click-binded)').bind('mousedown', function (e) {
									try {
										// console.log('click happens');
										$('#narrow-search-results-holder').setGridParam({
											'lastAccessedRow': $(e.target).closest("tr.jqgrow")[0]
										});
									} catch (e) {
										// console.log(e, e.stack);
									}
								})
								.addClass('click-binded');
							// }
							// console.log($("#paint-narrow-search-results"), $("#paint-narrow-search-results").data('showmore'));
						},

						getNarrowSearchURLArgs : function(sortType,sortTypeHtmlId) {

							var searchNarrowParams = '';
							var curObj=this;
							var numfilterSets = curObj.filterSets.length;

							for ( var i = 0; i < numfilterSets; i++) {
								searchNarrowParams += '&' + curObj.filterSets[i].code + '=';
								var appliedFilters = '';
								switch (curObj.filterSets[i].type) {
								case 'radio':
									appliedFilters += $('input[name="'+curObj.filterSets[i].code+'"]:checked').val();
									break;

								case 'checkbox':
									$('#' + curObj.filterSets[i].code + '_filterset .narrow-search-filterset-checkbox').each(function() {
														if ($(this).is(':checked')) {
															appliedFilters += (appliedFilters == '') ? '': '|';
															appliedFilters += $(this).val();
														}
									});
									break;

								case 'addltext':
									var tmpDefaultText;
									var curText = $('#' + curObj.filterSets[i].code + '-addltext-filter').val();
									if (curText == curObj.filterSets[i].defaultText || curText == undefined || curText === null || curText == "") {
										tmpDefaultText = '';
									} else {
										tmpDefaultText = curText;
									}
									appliedFilters += encodeURIComponent(tmpDefaultText);
									break;

								case 'tagscloud':
									var curText = $('#' + curObj.filterSets[i].code + '-addltext-filter').val();
									if(curText == undefined || curText === null || curText == ""){
										curText = '';
									}
									appliedFilters += encodeURIComponent(curText);
									break;

								case 'slider':
									var prefix = $('#'+curObj.filterSets[i].code+'_filterset').data("prefix");
									var suffix = $('#'+curObj.filterSets[i].code+'_filterset').data("suffix");
									var tempprice;
									var Lprice = '';
									var Rprice = '';

									if($( '#value-slide-left-'+curObj.filterSets[i].code+'' ).val() != undefined){
										Lprice = $( '#value-slide-left-'+curObj.filterSets[i].code+'' ).val().replace(prefix+suffix,'');
									}
									if($( '#value-slide-right-'+curObj.filterSets[i].code+'' ).val() != undefined){
										Rprice = $( '#value-slide-right-'+curObj.filterSets[i].code+'' ).val().replace(prefix+suffix,'');
									}
									
									if((Lprice == undefined || Lprice === null || Lprice == "") || (Rprice == undefined || Rprice === null || Rprice == "")) {
										tempprice = '';
									}
									else {
										tempprice = $.trim(Lprice+'|'+Rprice);
									}
//									if(tempprice == 'undefined-undefined' || tempprice == undefined-undefined || tempprice =='|' || tempprice == undefined || tempprice === null || tempprice == "")
//										tempprice = '';

									appliedFilters += tempprice;
									break;

								case 'daterange':
									var clrLinkSelector = '#' + curObj.filterSets[i].code + '-daterange-clr';

									var fromDate = $('#' + curObj.filterSets[i].code + '-daterange-from-date').val();
									var toDate = $('#' + curObj.filterSets[i].code + '-daterange-to-date').val();

									if (fromDate == curObj.filterSets[i].from_default_text || fromDate == undefined || fromDate === null || fromDate == "") {
										fromDate = '';
									} else {
										$(clrLinkSelector).css('display','block');
									}

									if (toDate == curObj.filterSets[i].to_default_text || toDate == undefined || toDate === null || toDate == "") {
										toDate = '';
									} else {
										$(clrLinkSelector).css('display','block');
									}
									appliedFilters += fromDate + '|' + toDate;
									break;
								} // end switch

								searchNarrowParams += appliedFilters;
							} // end for loop

							// Append sortby to the search string
							searchNarrowParams += '&sortby=' + sortType;

							// Append text substring from Search By Text filter
							var textFilter = ($('#narrow-search-text-filter').val() == curObj.topTextFilterLabel) ? '' : $('#narrow-search-text-filter').val();
							 //44552: search not working in survey and assessment page.
							searchNarrowParams += '&textfilter=' + encodeURIComponent(textFilter);

							return searchNarrowParams;
						},

						refreshGrid : function(sortType, sortTypeHtmlId) {
							if($('#narrow-search-actionbar')){
							  //$('#narrow-search-actionbar').hide();
							}
							var curObj=this;

							if (sortType == undefined || sortType == null || sortType == '') {
								sortType = curObj.lastSortType;
							}
							if (sortTypeHtmlId == undefined || sortTypeHtmlId == null || sortType == '') {
								sortTypeHtmlId = curObj.lastSortTypeHtmlId;
							}
							$('#gview_narrow-search-results-holder').css('min-height', '100px');
                         //   if(curObj.searchBasePath == "administration/learning/catalog/search/all/")
                            curObj.createLoader('main-wrapper');
                         //   else
							//curObj.createLoader('paint-narrow-search-results');

							var ajaxUrl = curObj.constructUrl(curObj.searchBasePath + curObj.getNarrowSearchURLArgs(sortType,sortTypeHtmlId));
							//console.log('ajaxUrl', ajaxUrl);
							if(ajaxUrl.indexOf('catalogtype=Class') !== -1) {
								$('#narrow-search-results-holder').setGridParam({jsonReader: {id: "jqgrid-rowid"}});;
							} else {
								$('#narrow-search-results-holder').setGridParam({jsonReader: {id: "id"}});;
							}
							$('#narrow-search-results-holder').setGridParam({url : ajaxUrl});

							$("#narrow-search-results-holder").trigger("reloadGrid", [ {page : 1} ]);

							curObj.updateCheckboxFiltersDisplay();

							// Highlight correct sort type
							$('.narrow-search-sortbar-sortlinks li').each(function() {
										$(this).find('a').removeClass('sortype-high-lighter');
							});

							if (sortTypeHtmlId.length > 0) {
								$('#' + sortTypeHtmlId).addClass('sortype-high-lighter');
								curObj.lastSortType = sortType;
								curObj.lastSortTypeHtmlId = sortTypeHtmlId;
							}
							vtip();
						},

						updateCheckboxFiltersDisplay : function() {
							$('.narrow-search-filtersets-holder').find('input[type=checkbox]').each(function() {
												if ($(this).is(':checked')) {
													$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-unselected');
													$(this).parent().next('label').addClass('narrow-search-filterset-item-label-selected');
													$(this).parent().removeClass('narrow-search-filterset-checkbox-unselected');
													$(this).parent().addClass('narrow-search-filterset-checkbox-selected'); // The checkbox itself is animage
												} else {
													$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-selected');
													$(this).parent().next('label').addClass('narrow-search-filterset-item-label-unselected');
													$(this).parent().removeClass('narrow-search-filterset-checkbox-selected');
													$(this).parent().addClass('narrow-search-filterset-checkbox-unselected'); // The checkbox itself is an image
												}
											});

							$('.narrow-search-filtersets-holder').find('input[type=radio]').each(function() {
								if ($(this).is(':checked')) {
									$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-unselected');
									$(this).parent().next('label').addClass('narrow-search-filterset-item-label-selected');
									$(this).parent().removeClass('narrow-search-filterset-radio-unselected');
									$(this).parent().addClass('narrow-search-filterset-radio-selected'); // The radio itself is animage
									// the data is undefined,added this condition.							
									if($(".classlangtype-scroll-list-container").jScrollPane({}).data('jsp') != undefined)
									{
									$(".classlangtype-scroll-list-container").jScrollPane({}).data('jsp').destroy();
									}
									$(".classlangtype-scroll-list-container").jScrollPane();	//Apply scroll
									if($(".classcurrency-scroll-list-container").jScrollPane({}).data('jsp') != undefined)
									{
									$(".classcurrency-scroll-list-container").jScrollPane({}).data('jsp').destroy();
									}
									$(".classcurrency-scroll-list-container").jScrollPane();	//Apply scroll	
								} else {
									$(this).parent().next('label').removeClass('narrow-search-filterset-item-label-selected');
									$(this).parent().next('label').addClass('narrow-search-filterset-item-label-unselected');
									$(this).parent().removeClass('narrow-search-filterset-radio-selected');
									$(this).parent().addClass('narrow-search-filterset-radio-unselected'); // The checkbox itself is an image

								}
							})
							vtip();
						},

						initGrid : function(widgetSelector, widgetName) {
							$('#first_narrow-search-results-pager').click(function(e) {
												if (!$('#first_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});
							$('#prev_narrow-search-results-pager').click(function(e) {
												if (!$('#prev_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							$('#next_narrow-search-results-pager').click(function(e) {
												if (!$('#next_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});
							$('#last_narrow-search-results-pager').click(function(e) {
												if (!$('#last_narrow-search-results-pager').hasClass('ui-state-disabled')) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							$('.ui-pg-selbox').bind('change',function() {
												$('#narrow-search-results-holder').hide();
												$('#gview_narrow-search-results-holder').css('min-height','100px');
												$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												$(widgetSelector).data(widgetName).hidePageControls(false);
												this.blur();
											});

							$(".ui-pg-input").keyup(function(event) {
												if (event.keyCode == 13) {
													$('#narrow-search-results-holder').hide();
													$('#gview_narrow-search-results-holder').css('min-height','100px');
													$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
												}
											});

							if(this.currTheme == "expertusoneV2"){
								$('#narrow-search-results-pager .page-show-prev').bind('click',function() {
									if(parseInt($("#narrow-search-results-pager .page_count_view").attr('id')) < 0){
										$("#narrow-search-results-pager .page_count_view").attr('id','0');
									}else{
										$('#narrow-search-results-holder').hide();
										$('#gview_narrow-search-results-holder').css('min-height','100px');
										$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
										$(widgetSelector).data(widgetName).hidePageControls(false);
									}
								});

								$('#narrow-search-results-pager .page-show-next').bind('click',function() {
									if(parseInt($("#narrow-search-results-pager .page_count_view").attr('id')) >= parseInt($("#narrow-search-results-pager .page-total-view").attr('id'))){
										$("#narrow-search-results-pager .page_count_view").attr('id',($("#narrow-search-results-pager .page_count_view").attr('id')-1));
									}else{
										$('#narrow-search-results-holder').hide();
										$('#gview_narrow-search-results-holder').css('min-height','100px');
										$(widgetSelector).data(widgetName).createLoader('paint-narrow-search-results');
										$(widgetSelector).data(widgetName).hidePageControls(false);
									}
								});

							}
							vtip();
						},

						initTextFilter : function(textFilterACPath) {

							if ($('#narrow-search-text-filter')) {
								var obj = this;
								// Destroy previous autocomplete setup on this field
								$('#narrow-search-text-filter').removeData();

								$("#narrow-search-text-filter").keyup(function(event) {
											if (event.keyCode == 13) {
												obj.narrowSearchByText();
												$('.ac_results').css('display', 'none');
											}
								});

								$('#narrow-search-text-filter-go').click(function() {
									        /*this.currTheme = Drupal.settings.ajaxPageState.theme;*/
											obj.narrowSearchByText();
											 /*var selector='#narrow-search-text-filter';
                                             $(selector).addClass('header-search-text-filter');
                                             $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
                                             $(selector).addClass('narrow-search-filterset-daterange-empty');
                                             if(this.currTheme == 'expertusoneV2'){
                                            	 $(selector).val(Drupal.t('LBL304'));
                                             }else{
                                            	 $(selector).val((Drupal.t('LBL304')).toUpperCase());
                                             }*/

								});

								if (textFilterACPath != undefined && textFilterACPath != 'undefined' && textFilterACPath != null && textFilterACPath != '') {
									textFilterACPath = "/?q="+ textFilterACPath;
									$('#narrow-search-text-filter').autocomplete(textFilterACPath, {
												minChars : 3,
												max : 50,
												autoFill : true,
												mustMatch : false,
												matchContains : false,
												inputClass : "ac_input",
												loadingClass : "ac_loading"
											});
								}
							}

							this.initTextDateFilterBlurStyle("#narrow-search-text-filter", obj.topTextFilterLabel,"1");
							$("#narrow-search-text-filter").focus(function () {
								try {
									var inputElem = $(this);
									inputElem.val() == '' || addClearIcon(inputElem);
								} catch(e) {
									// window.console.log(e, e.stack);
								}
							})
							.blur(function(e) {
								try {
									var inputElem = $(this);
									removeClearIcon(inputElem)
								} catch(e) {
									// window.console.log(e, e.stack);
								}
							}).keyup(function () {
								var inputElem = $(this);
								if(inputElem.siblings('.eol-search-clearance').length < 1) {
									inputElem.val() == '' || addClearIcon(inputElem);
								} else {
									inputElem.val() == '' && removeClearIcon(inputElem);
								}
							});
							vtip();						
						},


						initFilters : function() {

							// init each filter as appropriate
							for ( var i = 0; i < this.filterSets.length; i++) {
								var filterSet = this.filterSets[i];
								 //alert(filterSet.toSource());
								switch (filterSet.type) {
								case 'addltext':
									this.initAddlTextFilter(filterSet);
									break;

//								case 'tagscloud':
//									if ($('#' + filterSet.code + '_tagscloud').is(':visible')) {
//								      var tagsHeight = $('#' + filterSet.code + '_tagscloud').height();
//								      if (tagsHeight > 175) {
//								        $('#' + filterSet.code + '_tagscloud').height(175);
//									    $('#' + filterSet.code + '_tagscloud').css('max-height', '175px');
//										$('#' + filterSet.code + '_tagscloud').jScrollPane({});
//									  }
//	                                }
//									break;

								case 'daterange':
									this.initDateRangeFilter(filterSet);
									break;

								case 'slider' :
									this.initSlider(filterSet);
									break;

								case 'checkbox':
									if (filterSet.code =='group') {
										if ($('#' + filterSet.code + '_filterset').is(':visible')) {
											($('#' + filterSet.code + '_filterset').removeClass('display-more'));
											var groupHeight = $('#' + filterSet.code + '_filterset').height();
										      if (groupHeight > 10) {
											    $('#' + filterSet.code + '_filterset').css({'width':'160px'});
											    //$('#' + filterSet.code + '_filterset').css({'height':'100px','max-height', '100px'});
											    //$('.jspContainer').css('width','160px');
												//$('#' + filterSet.code + '_filterset').jScrollPane({});
										}
									}
								  }
								} // end switch
							} // end for
							//Removing last filter type border
							var filterCount = $('.narrow-search-filterset').filter(':visible').length-1;
							$('.narrow-search-filterset:eq('+filterCount+')').css({'border-bottom':'none','padding-bottom':'0px'});
						},


					    initTextDateFilterBlurStyle : function(selector,defaultText,fromTopFilterTextSearch) {

							var data = selector + '&' + defaultText + '&' + fromTopFilterTextSearch;
							var curObj=this;

							$(selector).blur(data,function(event) { // Can pass a string data only. but not an object data as objects are passed by reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												var fromTopFilterTextSearch = tokens[2];

												if(fromTopFilterTextSearch=="1"){
													defaultText=curObj.topTextFilterLabel;
												}

												if (fieldValue == '' || fieldValue == defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
													$(selector).addClass('narrow-search-filterset-daterange-empty');
													$(selector).val(defaultText);
												} else if (fieldValue != defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-empty');
													$(selector).addClass('narrow-search-filterset-daterange-nonempty');
												}

											});

							$(selector).focus(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by  reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												//$(selector).val('');
												if (fieldValue == defaultText) {
													$(selector).val('');
												}
												$(selector).removeClass('narrow-search-filterset-daterange-empty');
												$(selector).addClass('narrow-search-filterset-daterange-nonempty');

											});

							$(selector).change(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by reference
												var data = event.data;
												var tokens = data.split("&");
												var selector = tokens[0];
												var fieldValue = $(selector).val();
												var defaultText = tokens[1];
												var fromTopFilterTextSearch = tokens[2];

												if(fromTopFilterTextSearch=="1"){
													defaultText=curObj.topTextFilterLabel;
												}

												if (fieldValue == '' || fieldValue == defaultText) {
													$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
													$(selector).addClass('narrow-search-filterset-daterange-empty');
													$(selector).val(defaultText);
												} else {
													$(selector).removeClass('narrow-search-filterset-daterange-empty');
													$(selector).addClass('narrow-search-filterset-daterange-nonempty');
												}

											});

							var fieldValue = $(selector).val();

							if (fieldValue == defaultText) {
								$(selector).addClass('header-search-text-filter');
								$(selector).removeClass('narrow-search-filterset-daterange-nonempty');
								$(selector).addClass('narrow-search-filterset-daterange-empty');
								$(selector).val(defaultText);
							}

							vtip();
						},


						initAddlTextFilter : function(filterSet) {
							if ($('#' + filterSet.code + '-addltext-filter')) {

								var obj = this;

								$('#' + filterSet.code + '-addltext-filter').keyup(filterSet.code,function(event) {
													if (event.keyCode == 13) {
														obj.narrowSearchByAddlText(event.data);

														//Display clear link in the arrow search filter
														$('#' + filterSet.code + '-addltext-clr').css('display','block');
														$('.ac_results').css('display', 'none');
													}
								});

								$('#' + filterSet.code + '-addltext-filter-go').click(filterSet.code,function(event) {
									if($("#" + filterSet.code + "-addltext-filter").val() != filterSet.defaultText){
										obj.narrowSearchByAddlText(event.data);
									}
								});

								if (filterSet.acpath != undefined && filterSet.acpath != 'undefined' && filterSet.acpath != null && filterSet.acpath != '') {
									var addlTextFilterACPath = "/?q=" + filterSet.acpath;
									var multipleval = '';
									if(filterSet.code == 'grporg' || filterSet.code == 'grpjobrole' || filterSet.code == 'grploc'|| filterSet.code == 'grpempl' || filterSet.code == 'grpdep' || filterSet.code == 'grpusrtyp' || filterSet.code == 'grpcontry'|| filterSet.code == 'grplang')
										multipleval = true;
									else
										multipleval = false;
									$('#' + filterSet.code + '-addltext-filter') .autocomplete(addlTextFilterACPath,{
														minChars : 3,
														max : 50,
														autoFill : true,
														mustMatch : false,
														matchContains : false,
														inputClass : "ac_input",
														loadingClass : "ac_loading",
														multiple: multipleval

								    });
								} // endif !empty(textFilterACPath)
							} // end filterset exists

							var selector = "#" + filterSet.code + '-addltext-filter';
							var defaultText = filterSet.defaultText;
							this.initTextDateFilterBlurStyle(selector,defaultText);
						},

						initDateRangeFilter : function(filterSet) {
							if ($('#' + filterSet.code + '-daterange-from-date')) {
								filterSet.__init = 'FROM';
								this.initDateRangeDateField(filterSet);
							}
							if ($('#' + filterSet.code + '-daterange-to-date')) {
								filterSet.__init = 'TO';
								this.initDateRangeDateField(filterSet);
							}
						},

						initDateRangeDateField : function(filterSet) {
							var selectorSuffix = (filterSet.__init == 'FROM') ? '-daterange-from-date' : '-daterange-to-date';
							var selector = '#' + filterSet.code + selectorSuffix;
							var buttonText = (filterSet.__init == 'FROM') ? filterSet.from_tooltip : filterSet.to_tooltip;
							var calendarIcon = resource.base_url + '/' + themepath + '/expertusone-internals/images/calendar_icon.JPG';
							// alert(calendarIcon);
							$(selector).datepicker({
												duration : '',
												showTime : false,
												constrainInput : false,
												stepMinutes : 5,
												stepHours : 1,
												time24h : true,
												dateFormat : "mm-dd-yy",
												buttonImage : calendarIcon,
												buttonImageOnly : true,
												firstDay : 0,
												showOn : 'both',
												buttonText : buttonText,
												showButtonPanel : true,
												changeMonth : true,
												changeYear : true,
												daterangeDiffDays : 20000,

												beforeShow : function(input) {

													var data = $("#" + input.id).metadata();
													var dateFieldCode = data.dateFieldCode;
													var dateShowOption = data.dateRangeShowOption;

													if (dateShowOption == "" || dateShowOption == null || dateShowOption == "undefined") {
														dateShowOption = "all";
													}
													if (dateFieldCode == "" || dateFieldCode == null || dateFieldCode == "undefined") {
														var tokens = data .split("-");
														var dateFieldCode = tokens[0];
													}

													var dateMin = null;
													var dateMax = null;
													var fromSelectorCode = dateFieldCode + '-daterange-from-date';
													var toSelectorCode = dateFieldCode + '-daterange-to-date';
													var fromSelector = '#' + fromSelectorCode;
													var toSelector = '#' + toSelectorCode;

													if (input.id == fromSelectorCode && $(fromSelector).datepicker("getDate") != null) {
														var daterangeDiffDays = $(fromSelector).datepicker("option","daterangeDiffDays");

														if (dateShowOption == "future") { // To show future and today date for From Date

															dateMin = new Date();
															dateMax = $(toSelector).datepicker("getDate");
															dateMax.setDate(dateMin.getDate()+daterangeDiffDays);

														} else if (dateShowOption == "past") { // Default is to Show Past and Today Date for From Date

															var toDateFull = $(toSelector).datepicker("getDate");
															var fromDateFull = $(fromSelector).datepicker("getDate");
															var fromDate = fromDateFull.getFullYear()+ ","+ fromDateFull.getMonth()+ ","+ fromDateFull.getDate();
															var toDate = toDateFull.getFullYear()+ ","+ toDateFull.getMonth()+ ","+ toDateFull.getDate();
															var dateFromObj = new Date(fromDate);
															var dateToObj = new Date(toDate);
															var diffDates = dateToObj - dateFromObj;

															dateMax = new Date();

															if (diffDates == 0) {
																dateMin = $(fromSelector).datepicker("getDate");
																dateMin.setDate(dateMin.getDate() - daterangeDiffDays);
																dateMax = new Date();
															} else {
																dateMax = $(toSelector).datepicker("getDate");
																dateMin = $(toSelector).datepicker("getDate");
																dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
																dateMax.setDate(dateMax.getDate() - 0);
															}
														}
													} else if (input.id == toSelectorCode) {

														var daterangeDiffDays = $(toSelector).datepicker("option","daterangeDiffDays");

														if (dateShowOption == "future") { // To Show Future and Today Date for To Date
															dateMax = new Date();
															if ($(fromSelector).datepicker("getDate") != null) {
																dateMin = $(fromSelector).datepicker("getDate");
																dateMax = $(fromSelector).datepicker("getDate");
																dateMax.setDate(dateMax.getDate() + daterangeDiffDays);
															}
														} else if (dateShowOption == "past") { // Default is to Show Past and Today Date for To Date
															dateMax = new Date();
															if ($(fromSelector).datepicker("getDate") != null) {
																dateMin = $(fromSelector).datepicker("getDate");
																var minDate = dateMin.getFullYear()+ ","+ dateMin.getMonth()+ ","+ dateMin.getDate();
																var maxDate = dateMax.getFullYear()+ ","+ dateMax.getMonth()+ ","+ dateMax.getDate();
																var dateFromObj = new Date(minDate);
																var dateToObj = new Date(maxDate);
																var diffDates = dateToObj - dateFromObj;

																if (diffDates <= 0) {
																	dateMin.setDate(dateMin.getDate() - daterangeDiffDays);
																} else {
																	dateMin.setDate(dateMin.getDate() - 0);
																}
															}
														}
													}

													return {
														minDate : dateMin,
														maxDate : dateMax
													};

												}
											});

							var defaultText = (filterSet.__init == 'FROM') ? filterSet.from_default_text : filterSet.to_default_text;

							this.initTextDateFilterBlurStyle(selector,defaultText);
						},

						initSlider: function(filterSet){
							obj = this;
							var valueMin = $('#'+filterSet.code+'_filterset').data("startval");
							var valueMax = $('#'+filterSet.code+'_filterset').data("endval");
							var prefix = $('#'+filterSet.code+'_filterset').data("prefix");
							var suffix = $('#'+filterSet.code+'_filterset').data("suffix");
							if(valueMax > valueMin){

								$('#value-slider-range-' + filterSet.code + '').slider({
									range: true,
									min: valueMin,
									max: valueMax,
									values: [ valueMin, valueMax ],
									slide: function( event, ui ) {
										$( '#value-slide-left-'+filterSet.code+'' ).val( prefix + ui.values[ 0 ] + suffix);
										$( '#value-slide-right-'+filterSet.code+'' ).val( prefix + ui.values[ 1 ] + suffix);
										 //+ " - $" + ui.values[ 1 ] );
									},
									change: function(e,ui){
										//$("#prg-admin-search").data("prgsesarch").searchAction();
										 obj.narrowSearchBySlider();
								    }
								});
								$( '#value-slide-left-'+filterSet.code+'').val( prefix + $('#value-slider-range-'+filterSet.code+'' ).slider( 'values', 0 ) + suffix);
								$( '#value-slide-right-'+filterSet.code+'' ).val( prefix + $( '#value-slider-range-'+filterSet.code+'' ).slider( 'values', 1 ) + suffix);
								/*if(this.currTheme == "expertusoneV2"){
									$( '#value-slider-range-'+filterSet.code+' a:last').css('margin-left','-9px');
								}*/
							}

						},

						/*
						 * IsValidDate() - Check that given date is valid or not -
						 * TRUE if date is valid - FALSE if date is valid
						 */
						IsValidDate : function(Day, Mn, Yr) {
							var DateVal = Mn + "/" + Day + "/" + Yr;
							var dt = new Date(DateVal);

							if (dt.getDate() != Day) {
								//alert('Invalid Date');
								return (false);
							} else if (dt.getMonth() != Mn - 1) {
								// this is for the purpose JavaScript starts the
								// month from 0
								//alert('Invalid Date');
								return (false);
							} else if (dt.getFullYear() != Yr) {
								//alert('Invalid Date');
								return (false);
							}

							return (true);
						},

						/*
						 * validateDateRangeDates() - Action for the Go button
						 * in the daterange component - validates dates in the
						 * date range narrow search component. - On error,
						 * displays the error message in the component. - On
						 * success, submits the search request
						 */
						validateDateRangeDates : function(code,ID) {
                            var fromDefaultText = $('#' + ID).data('default-fromtext');
                            var toDefaultText =  $('#' + ID).data('default-totext');
							var fromDateSelector = '#' + code + '-daterange-from-date';

							var toDateSelector = '#' + code + '-daterange-to-date';

							var dateFrom = ($(fromDateSelector).val() == fromDefaultText) ? '' : $(fromDateSelector).val();
							var dateTo = ($(toDateSelector).val() == toDefaultText) ? '' : $(toDateSelector).val();

							if (dateFrom == '' && dateTo == '') {
								this.narrowSearchByDateRange(code);
								return;
							}

							var msg = "";
							var validFromDate = false;
							var validToDate = false;
							if (dateFrom != "") {
								var dateFromSplit = dateFrom.split('-');
								var fromDay = dateFromSplit[1];
								var fromMonth = dateFromSplit[0];
								var fromYear = dateFromSplit[2];
								validFromDate = this.IsValidDate(fromDay, fromMonth, fromYear);
								if (!validFromDate) {
									msg += Drupal.t("LBL1154");
								}
							}
							if (dateTo != "") {
								var dateToSplit = dateTo.split('-');
								var toDay = dateToSplit[1];
								var toMonth = dateToSplit[0];
								var toYear = dateToSplit[2];
								validToDate = this.IsValidDate(toDay, toMonth, toYear);
								if (!validToDate) {
									if (msg != "")
										msg += "<br/>";
									//msg += Drupal.t("LBL1146");
								}
							}
							if (validFromDate == true && validFromDate == true) {
								var dateFromObjStr = fromYear + '/' + fromMonth + '/' + fromDay;
								var dateToObjStr = toYear + '/' + toMonth + '/'+ toDay;
								var dateFromObj = new Date(dateFromObjStr);
								var dateToObj = new Date(dateToObjStr);
								var diffDates = dateToObj - dateFromObj;
								if (diffDates < 0) {
									if (msg != "")
										msg += "<br/>";
									msg += Drupal.t("LBL1155");
								}
							}
							errMsgDivSelector = '#' + code + '-daterange-errmsg';
							if (msg != "") {
								$(errMsgDivSelector).html(Drupal.t(msg));
								$(errMsgDivSelector).css('display','inline-block');
								$(errMsgDivSelector).show();
							} else {
								$(errMsgDivSelector).hide();
								this.narrowSearchByDateRange(code);
							}

						},

						/*
						 * clearDateRangeFields() - Action for the clear link in
						 * the daterange component
						 */
						clearDateRangeFields : function(code, ID) {
							// Prepare the jQuery selectors
							var fromDefaultText = $('#' + ID).data('default-fromtext');
                            var toDefaultText =  $('#' + ID).data('default-totext');
							var fromDateSelector = "#" + code + "-daterange-from-date";
							var toDateSelector = "#" + code + "-daterange-to-date";
							var clearDateRangeSelector = '#' + code + '-daterange-clr';
							var errMsgDivSelector = '#' + code + '-daterange-errmsg';

							// Show the default texts in date fields in default
							// color and font
							$(fromDateSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(fromDateSelector).addClass('narrow-search-filterset-daterange-empty');
							$(fromDateSelector).val(fromDefaultText);

							$(toDateSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(toDateSelector).addClass('narrow-search-filterset-daterange-empty');
							$(toDateSelector).val(toDefaultText);

							// Hide the clear link
							$(clearDateRangeSelector).css('display', 'none');

							// Hide the error message box
							$(errMsgDivSelector).hide();

							// Refresh the datepicker
							$(fromDateSelector).datepicker("refresh");
							$(toDateSelector).datepicker("refresh");

							// Search as per cleared filter settings
							this.narrowSearchByDateRange(code);

						},
						/*
						 * validatetextField() - Action for the clear link to appear near the
						 * relevant text box.
						 */

						validatetextField : function(code, ID) {
							var defaultText = $('#' + ID).data('default-text');
							var defaultTextSelector = "#" + code + "-addltext-filter";
							var showClearLink = "#" + code + "-addltext-clr";
							if($("#" + code + "-addltext-filter").val() != defaultText){
								$(showClearLink).css('display', 'block');
							}

						},
						/*
						 * clearTextBoxFields() - Action for the clear link in
						 * the username, organization and location component
						 */
						clearTextBoxFields : function(code, ID) {
							var defaultText = $('#' + ID).data('default-text');
							var defaultTextSelector = "#" + code + "-addltext-filter";
							var clearTextFieldSelector = '#' + code + '-addltext-clr';
							// Show the default texts in date fields in default
							// color and font
							$(defaultTextSelector).removeClass('narrow-search-filterset-daterange-nonempty');
							$(defaultTextSelector).addClass('narrow-search-filterset-daterange-empty');
							$(defaultTextSelector).val(defaultText);
							// Hide the clear link
							$(clearTextFieldSelector).css('display', 'none');
							this.narrowSearchByAddlText(code);

						},

						clearTagsCloudNarrowFilter : function(code) {
							$('#' + code + '-addltext-filter').val('');
							$('.tagscloud-tag').css('text-decoration', '');
							$('#' + code + '-tagscloud-clr').css('display', 'none');
							$('#root-admin').data('narrowsearch').refreshGrid();
						},

						moreListDisplay : function(recLen,dispId,html_id) {
							if(html_id==''){
				            		for(i=0;i<=recLen;i++){
				            			$('#'+dispId+'_'+i).css("display","block");
				            		}
				            		$('#'+dispId+'_more').css("display","none");
				            		$('#'+dispId+'_short').css("display","block");
							}else{
			            		for(i=0;i<=recLen;i++){
			            			$('#'+html_id+' #'+dispId+'_'+i).css("display","block");
			            		}
			            		$('#'+html_id+' #'+dispId+'_more').css("display","none");
			            		$('#'+html_id+' #'+dispId+'_short').css("display","block");
							}
							vtip();
		            	},

		            	shortListDisplay : function(recLen,dispId,html_id){
		            		if(html_id==''){
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
		            		}else{
			            		for(i=0;i<=recLen;i++){
			            			if(i<=4){
			            				$('#'+html_id+' #'+dispId+'_'+i).css("display","block");
			            				}
			            			else {
			            				$('#'+html_id+' #'+dispId+'_'+i).css("display","none");
			            				}
			            			}
			            		$('#'+html_id+' #'+dispId+'_short').css("display","none");
			            		$('#'+html_id+' #'+dispId+'_more').css("display","block");
		            		}
		            		vtip();
		            	},


						/*	callDeleteObject : function(objectId,objectType){
						 var obj = this;
							url = obj.constructUrl("ajax/admincore/deleteobject/" + objectId+'/'+objectType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
								result = $.trim(result);
								//alert(result)
								   if(result > 0){
									   if(objectType == "Class") {
										   obj.callMessageWindow('Delete','This Class has been Enrolled/Futured. So cannot delete.');
									   }else if(objectType == "Course"){
										   obj.callMessageWindow('Delete','This Course has one of the Enrolled/Futured Class. So cannot delete.');
									   }else{
										   obj.callMessageWindow('Delete','Training Plan');
									   }

								   }else{
									   obj.displayDeleteWizard(objectId,objectType);
								   }
								}
						    });

					},	*/

					callDeleteProcess : function(objectId,objectType){
						$('#delete-msg-wizard').remove();
						var obj = this;
							url = obj.constructUrl("ajax/admincore/deleteprocessobject/" + objectId+'/'+objectType);
							$.ajax({
								type: "POST",
								url: url,
								data:  '',
								success: function(result){
									Drupal.CTools.Modal.dismiss();
									// Admin Calendar - Start - Refresh calendar after delete announcement
									if(window.location.href.indexOf("admincalendar") >0 )
										getCalendarData();
									// Admin Calendar - End
									$("#root-admin").data("narrowsearch").refreshGrid();
								}
						    });

					},

					displayDeleteWizard : function(title,objectId,objectType,wSize){
						var uniqueClassPopup = '';
						if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
							if(objectType == "Class" && document.getElementById('catalog-course-basic-addedit-form')){
								uniqueClassPopup = 'unique-delete-class';
							}
						}
						var wSize = (wSize) ? wSize : 300;
						var closeButAction = '';
						var esignForType = '';
						var settingsList = ['cre_usr_dpt', 'cre_usr_etp', 'cre_usr_jrl', 'cre_usr_jtl', 'cre_usr_ptp'];
						if (settingsList.indexOf(objectType) != -1)  {
							closeButAction = function(){ $('#deleteAddedList-'+objectId).click(); };
							esignForType = 'DeleteAdminObjectSettingsList';
						} else {
							
							if(objectId == "cre_ste_mod_aut" || objectId == "cre_ste_mod_cattr"){   //For enable/Disable of Content Authoring module
								closeButAction = function(){ $("#root-admin").data("narrowsearch").publishAndUnpublishModule(objectType,''); };
							}else{
							closeButAction = function(){ $("#root-admin").data("narrowsearch").callDeleteProcess(objectId,objectType); };
							 }
							esignForType = 'DeleteAdminObject';
						}
							$('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

					   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
					    if(this.currTheme == 'expertusoneV2'){
					    	if(objectType == 'Content')
					    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					    	else
					    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+''+'?'+'</td></tr>';
					    } else {
					    	if(objectType == 'Content')
					    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: left;">'+title+'</td></tr>';
					    	else
					    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+''+'?'+'</td></tr>';
					    }
					    html+='</table>';
					    html+='</div>';
					    $("body").append(html);
					    var closeButt={};
					    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

					    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "" && objectId != "cre_ste_mod_aut" && objectId != "cre_ste_mod_cattr"){
					    	var esignObj = {'popupDiv':'delete-object-dialog','esignFor':esignForType,'objectId':objectId,'objectType':objectType};
					    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);  };
					    }else{
					    	closeButt[Drupal.t('Yes')]= closeButAction;
					    }

					    if(objectType == 'UnlockUser'){
					    	var drupalTitle = Drupal.t("LBL930");
					    }else if(objectType == 'exp_sp_administration_contentauthor'){
					    	var drupalTitle = Drupal.t("LBL749");
					    }else if(objectType == 'exp_sp_administration_customattribute'){
					    	var drupalTitle = Drupal.t("LBL2015");
					    }else{
					    	var drupalTitle = Drupal.t("LBL286");
					    }

					    
					    $("#delete-msg-wizard").dialog({
					        position:[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:wSize,
					        resizable:false,
					        modal: true,
					        title:Drupal.t(drupalTitle),//"title",
					        buttons:closeButt,
					        closeOnEscape: false,
					        draggable:false,
					        zIndex : 10005,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

					    $("#delete-msg-wizard").show();

						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });
						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
						/*-- 37211: Unable to delete a class in FF version 32.0 --*/
					    if($('div.qtip-defaults').length > 0) {
					    	var prevZindex = $('.qtip-defaults').css('z-index');
					    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
					    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
					    }

					},
					
					displayH5PPreview : function(h5pplayer,objectId,objectType,wSize,hSize){
							var uniqueClassPopup = '';
						if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
							if(objectType == "Class" && document.getElementById('catalog-course-basic-addedit-form')){
								uniqueClassPopup = 'unique-delete-class';
							}
						}
						var wSize = (wSize) ? wSize : 300;
						var hSize = (hSize) ? hSize : 800;
						var closeButAction = '';
						var esignForType = '';
						$('#delete-msg-wizard').remove();
					    var html = '';
					    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
					    //html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

					   
					   //html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
					   
					    
					    //html+='</table>';
					    html+='</div>';
					    $("body").append(html);
					    var closeButt={};
					    var drupalTitle = Drupal.t("LBL694");
					    $("#delete-msg-wizard").dialog({
					        position:'center',//[(getWindowWidth()-400)/2,200],
					        bgiframe: true,
					        width:wSize,
					        height:hSize,
					        resizable:false,
					        modal: true,
					        title:Drupal.t(drupalTitle),//"title",
					        buttons:closeButt,
					        closeOnEscape: true,
					        draggable:false,
					        zIndex : 10005,
					        overlay:
					         {
					           opacity: 0.9,
					           background: "black"
					         }
					    });
					    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
					    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

					    $("#delete-msg-wizard").show();
					    //display content
					    if(objectType != "customattr"){
					    window.setTimeout(function(){
					    	createLoaderNew("delete-msg-wizard");
					    	},500);
					    }
					    $("#delete-msg-wizard").html(h5pplayer);
					    $("#delete-msg-wizard").css("overflow","hidden");

						$('.ui-dialog-titlebar-close').click(function(){
					        $("#delete-msg-wizard").remove();
					    });
						if(this.currTheme == 'expertusoneV2'){
					    	changeDialogPopUI();
					    }
						/*-- 37211: Unable to delete a class in FF version 32.0 --*/
					    if($('div.qtip-defaults').length > 0) {
					    	var prevZindex = $('.qtip-defaults').css('z-index');
					    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
					    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
					    }

					},
					

					clearBubblePopupSet : function() {
						//this.qtipLoadSet = array();
					},

					fillCompletionDate : function(uniqueId, uniqueId2){
						var overallDate = $('#overall_completion_date_'+uniqueId2).val();
						$('input[name="hidden_completion_date_'+uniqueId+'"]').val(overallDate);
						//$('#datagrid-completiondate-'+uniqueId).val(overallDate);
					},

					completionDateQTip : function(uniqueId,entityId,sessionStartdate){
					$('.qtip-popup-exempted').html('').hide();
						$('.catalog-pub-add-list').hide();
						$('#completion_date_container_'+uniqueId).show();
						$('#overall_completion_date'+uniqueId).datepicker( "destroy" );

						if(entityId!='' && entityId!=undefined){
							if(sessionStartdate !== undefined) {
								if(this.currTheme == 'expertusoneV2'){
									if($.trim(sessionStartdate)){
										$('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
									    $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}else{
								    if($.trim(sessionStartdate)){
										  $('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
										}
										else{
										  $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
										}
								} 
							}
							else {
								//will work for TP completion where sessionStartdate will be undefined
								url = this.constructUrl("ajax/administration/getClassStartDate/" + entityId );
								$.ajax({
									type: "POST",
									url: url,
									data:  '',
									success: function(result){
										var sesStartDate = result.replace(/\,/g, '/');
										this.currTheme = Drupal.settings.ajaxPageState.theme;
										if(this.currTheme == 'expertusoneV2'){
											if($.trim(sesStartDate)){
												$('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sesStartDate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
											}
											else{
											    $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
											}
										}else{
										    if($.trim(sesStartDate)){
	 										  $('#overall_completion_date_'+uniqueId).datepicker({minDate:new Date(sesStartDate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
	 										}
	 										else{
	 										  $('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
	 										}
										}
									}
								});
							}

						}else{
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							if(this.currTheme =='expertusoneV2'){
								$('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
							}else{
								$('#overall_completion_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
					        }
						}
					},

					fillRegistrationDate : function(uniqueId, uniqueId2){
						var overallDate = $('#overall_registration_date_'+uniqueId2).val();
						$('input[name="hidden_registration_date_'+uniqueId+'"]').val(overallDate);
						$('#registration_date_container_'+uniqueId).hide();
					},

					registrationDateQTip : function(uniqueId,entityId, isPastClass, sessionStartdate){
						$('.catalog-pub-add-list').hide();
						if(entityId!='' && entityId!=undefined){
							if(isPastClass) {
								$('#registration_date_container_'+uniqueId).show();
								$('#overall_registration_date'+uniqueId).datepicker("destroy");
								this.currTheme = Drupal.settings.ajaxPageState.theme;
								if(this.currTheme == 'expertusoneV2'){
									if($.trim(sessionStartdate)){
										$('#overall_registration_date_'+uniqueId).datepicker({maxDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
										$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}else{
									if($.trim(sessionStartdate)){
										$('#overall_registration_date_'+uniqueId).datepicker({maxDate:new Date(sessionStartdate), dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
									else{
										$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
									}
								}
							}
						}else{
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							if(this.currTheme =='expertusoneV2'){
								$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
							}else{
								$('#overall_registration_date_'+uniqueId).datepicker({dateFormat: "mm/dd/yy", showOn: "button", buttonImage: "sites/all/themes/core/expertusone/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
					        }
						}
					},

					getBubblePopup : function(qtipObj) {
						try{						
							closeQtip('');					
							var popupId 	= qtipObj.popupDispId;	
							var qtipLen 	= this.qtipLenth;
							var inArrQtip = $.inArray(popupId, this.qtipLoadSet);
							this.qtipLoadSet[qtipLen] = popupId;
							this.qtipLenth = this.qtipLenth+1;
							var entityType = qtipObj.entityType;
							var courseId = qtipObj.courseId!=undefined?qtipObj.courseId:'';
							var url 			  = resource.base_host+'?q='+qtipObj.url; // resource.base_host+'?q=administration/catalogaccess/33/cre_sys_obt_crs';
							var wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
							var hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
							var setwBubble        = wBubble-20;
							var setBtmLeft        = (setwBubble-104)/2;
							var topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
							var bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
							var tipPos = qtipObj.tipPosition;
							var catalogVisibleId = qtipObj.catalogVisibleId;
							var mLeft = 0;
							var tipPosition;
							var messageStyle = '';
							var bwidth = '';
							var crTheme = Drupal.settings.ajaxPageState.theme;
							switch(qtipObj.tipPosition) {
								case 'bottomRight':
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									setBtmLeft        = (setwBubble-104)/2;
									mLeft			  = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?45:(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?58:(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0)?49:62;
									bwidth            = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?wBubble:Math.round((wBubble/2)/5);
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?'214':(bwidth*6)+12;
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('addclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?'168':(bwidth*6)+12;
									if(crTheme == 'expertusoneV2') {
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('addusers')>0)?'172':(bwidth*6)+12;
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('addclass')>0)?'373':(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?'168':(bwidth*6)+12;
									}
									/*if (navigator.userAgent.indexOf("Chrome")>0){
									  setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?'373':(bwidth*6)+12;
									}else{

									}*/
									//Set buble tool tip arrow position
									tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
								break;
								case 'bottomCorner':
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									if(crTheme != 'expertusoneV2') {
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?7:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?451:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?281:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?650:651):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?175:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}else{
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?6:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?458:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?283:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?535:619):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?178:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}
									}else {
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?7:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?472:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?281:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?650:651):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?185:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}else{
											setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?(wBubble==780?374:344):(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?6:(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?191:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?472:451;
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?283:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?651:619):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?16:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0)?185:175;
											mLeft			  = (qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls')?325:(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not'))?((entityType=='cre_sys_obt_not')?712:712):(qtipObj.catalogVisibleId.indexOf('listvaledit')>0)?59:218;
										}
									}

									//Set buble tool tip arrow position
									//var tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									tipPosition       = 'bottomRight';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmRight+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
								break;
								case 'rightBottom':
									//alert("right bottom");
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									//setRightTop        = (hBubble-104)/2;
									tipPosition       = 'rightBottom';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr-empty"><table cellspacing="0" width="100%" height="260" cellpadding="0"><tbody><tr><td class="bubble-brt" style="height: 169px"></td></tr><tr><td class="bubble-brm"></td></tr><tr><td class="bubble-brb" style="height:30px"></td></tr></tbody></table></td></tr><tr><td class="bubble-bl"></td><td class="bubble-bm"></td><td class="bubble-br"></td></tr></tbody></table>';
								break;
								case 'bottomLeft' :
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									bwidth            = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?wBubble:Math.round((wBubble/2)/5);
									if(crTheme != 'expertusoneV2') {
									setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'17':bwidth+2;
									setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'191':(bwidth*6)+12;
									setBtmMiddle	  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'64':bwidth+2;
									 if (navigator.userAgent.indexOf("Chrome")>0){
										setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'203':(bwidth*6)+12;
									 }
									}
									else{
										setBtmLeft        = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'14':bwidth+2;
										setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'155':(bwidth*6)+12;
										setBtmMiddle	  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'52':bwidth+2;
										if (navigator.userAgent.indexOf("Chrome")>0){
											setBtmRight       = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?'194':(bwidth*6)+12;
										}
									}
									mLeft			  = (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?-63:-110;
									//Set buble tool tip arrow position
									messageStyle      =  (qtipObj.catalogVisibleId.indexOf('listvalues')>0)?"margin-left: -120px;":'';
									tipPosition       = 'bottomLeft';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm" style="width:'+setBtmMiddle+'px"></td><td class="bubble-blr" style="width:'+setBtmRight+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
									break;
								default:
									wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
									hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
									setwBubble        = wBubble-20;
									setBtmLeft        = (setwBubble-104)/2;
									//Set buble tool tip arrow position
									tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
									topElements = '<span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
									bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-b"><table cellspacing="0" width="100%" cellpadding="0"><tbody><tr><td class="bubble-blt" style="width:'+setBtmLeft+'px"></td><td class="bubble-blm"></td><td class="bubble-blr" style="width:'+setBtmLeft+'px"></td></tr></tbody></table></td><td class="bubble-br"></td></tr></tbody></table>';
									$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");

							}
							if(popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_jtl" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_dpt" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_etp"  || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_jrl" || popupId == "qtip_addlistvalues_visible_disp_0_cre_usr_ptp"){
								$('.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues').attr('style','pointer-events: none');								
							}
							try{
								$('#'+popupId).qtip("destroy");
							}catch(e){
								//Nothing to do
							}
							$('#'+qtipObj.catalogVisibleId).closest(".qtip").remove();
							//IMPORTANT !!! qtip-child, qtip-parent -- Do not set or use this class anywhere else -- this is meant to be for qtip issue fixes

							 var qtipClass = (qtipObj.qtipClass) ? qtipObj.qtipClass : 'qtip-child';
							 if(qtipSubClass != null) {
								 var qtipSubClass = (qtipObj.qtipSubClass) ? qtipObj.qtipSubClass : '';
								 qtipClass = (qtipSubClass != null) ? qtipClass+" "+qtipSubClass : qtipClass;
							 }
							 if(!document.getElementById(qtipObj.catalogVisibleId)) {
								if((!document.getElementById("popup_container_"+popupId))) {
									$('#'+popupId).qtip({
										overwrite: false, // Make sure another tooltip can't overwrite this one without it being explicitly destroyed
										content: {
										 	text: "<div style='min-height:75px;' id='"+qtipObj.catalogVisibleId+"'><div id='"+qtipObj.catalogVisibleId+"_disp' style='margin-left:"+mLeft+"px;min-height:75px;'>&nbsp;</div></div>",
										 	title: {
												      text: ' ',
												      button: '<div id="'+qtipObj.catalogVisibleId+'_close" class="admin-bubble-close" onclick="closeQtyp(\''+qtipObj.catalogVisibleId+'\',\''+courseId+'\')"> </div>'
											   	   }
											},
										   api:{
												beforeShow: function(){
													//	Hide qtip till the content load
													var qtupActive = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active");
													$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','hidden');
													$(qtupActive).find('.qtip-tip').css('visibility','hidden');
												},
										 		onShow: function() {
													// Hide qtip till the content load
													var qtupActive = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active");
													$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','hidden');
													$(qtupActive).find('.qtip-tip').css('visibility','hidden');
													$('.qtip').each(function(){
														var tmp = this;
														if($(tmp).find('#'+qtipObj.catalogVisibleId).each(function(){
															if($(tmp).attr('class').indexOf('qtip-active')<0){
																try{
																	$('#'+popupId).qtip("destroy");
																	$(this).remove();
																}catch(e){
																	//Nothing to do
																}
															}
														}));
													});
										 			if(tipPos != 'bottomCorner') {
								 						$("#"+catalogVisibleId).parent().parent().parent().parent().removeClass("qtip-access-control");

								 					}
													if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
														var popTop = ($('#'+popupId).offset());
														var xtop = ($.browser.msie)?110:100;
														popTop = Math.round(popTop.top)-xtop;
														$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').css('top',popTop+"px");
													}
													if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0 && navigator.userAgent.indexOf("Chrome")>=0 ){
														var popTop = ($('#'+popupId).offset());
														var xtop = 150;
														popTop = Math.round(popTop.top)-xtop;
														$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').css('top',popTop+"px");
													}
													$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').addClass(qtipClass);
								 					//IMPORTANT !!! modal-qtips-close Do not set or use this class anywhere else -- this is meant to be for qtip issue fixes
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').addClass('modal-qtips-close');
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').siblings('.qtip-child').hide();
								 					if(qtipClass == "qtip-parent" || qtipClass == 'qtip-parent add-key-word-edit-list')
								 					$("#"+qtipObj.catalogVisibleId).parents('.qtip-active').siblings('.qtip-parent').hide();

													//if(!document.getElementById("paintContent"+popupId)) {
								 					if(1==1){
														EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(qtipObj.catalogVisibleId+"_disp");
														var loadPos=0;
														if(tipPosition == 'rightBottom') {
									 						$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').attr('style','height: 64px');
															$(qtupActive).find('.qtip-tip').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/bubble_arrow_point_right.png") no-repeat -1px 3px');
															$(qtupActive).find('.qtip-tip').css('bottom','79px');
															$(qtupActive).find('.qtip-tip').css('left','638px');
															$(qtupActive).find('.qtip-tip').css('position','absolute');
															$('#'+qtipObj.catalogVisibleId+"_close").css('top','');
														}
														if(tipPosition == 'bottomRight' && mLeft!=0) {
															//$('.qtip-tip').css('right','-35px');
									 						var leftPos=0;
									 						var rightPos="0px";
									 						var bottomPos="0px";
									 						var zIndex;
									 						var crTheme = Drupal.settings.ajaxPageState.theme;
									 						var bnrAccleftPos = (crTheme == 'expertusoneV2') ? 672 : 663;
									 						this.currTheme = Drupal.settings.ajaxPageState.theme;

									 						if(qtipObj.tipPosition == 'bottomCorner'){ // Access control fix
									 							if(qtipObj.catalogVisibleId.indexOf('Accessqtip')>0 && entityType=='cre_sys_obt_cls'){ // Class Access control
									 								loadPos = 520;
									 								leftPos = 308;
									 								rightPos = "-12px";
									 							}else if((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && (entityType=='cre_sys_obt_anm' || entityType=='cre_sys_obt_not')){ // Banner / Announcement Access control
									 								loadPos = 715;
									 								leftPos = (crTheme == 'expertusoneV2') ? ((entityType=='cre_sys_obt_not')?693:692) : 692;;
									 								rightPos = (entityType=='cre_sys_obt_not')?"-31px":"-30px";
									 							}else if((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && entityType!='cre_sys_obt_crs' && crTheme == 'expertusoneV2'){ // TP Access
									 								loadPos = 430;
									 								leftPos = 205;
									 								rightPos = "-6px";
									 							}else {
									 								loadPos = 430;
									 								leftPos = 201;
									 								rightPos = "-12px";
									 							}
									 						}else{ //class qtip position fix
									 							if(qtipObj.catalogVisibleId.indexOf('editclass')>0){
									 								loadPos = 380;
									 								leftPos = 28;
									 								rightPos = "-14px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0){
									 								loadPos = 380;
									 								leftPos = 40;
									 								rightPos = "-27px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0){
									 								loadPos = 380;
									 								leftPos = 30;
									 								rightPos = "-18px";
									 								zIndex ="100";
									 								bottomPos = (this.currTheme == "expertusoneV2") ? "0px" : "-64px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
									 								loadPos = 380;
									 								leftPos = 44;
									 								rightPos = "-31px";
									 								zIndex ="100";
									 								bottomPos = (this.currTheme == "expertusoneV2") ? "0px" : "-64px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
									 								loadPos = 380;
									 								leftPos = 30;
									 								rightPos = "-18px";
									 								zIndex ="100";
									 							}else if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_addusers')>0){
									 								loadPos = 380;
									 								leftPos = 65;
									 								zIndex ="100";
									 								rightPos = "-41px";
									 							}else if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
									 								zIndex ="100";
									 							}else if(qtipObj.catalogVisibleId.indexOf('listvalpeoedit')>0){
									 								zIndex ="100";
									 							}else{
									 								loadPos = 380;
									 								leftPos = 44;
									 								rightPos = "-31px";
									 							}
									 						}

															$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').css('right',rightPos);
															$('#'+qtipObj.catalogVisibleId+"_close").css('left',leftPos);
															if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
																$(qtupActive).find('.qtip-tip').css('bottom',bottomPos);
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
																if(this.currTheme == "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10' || this.currTheme == "expertusoneV2" && isAtLeastIE11==1 ){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
                                                               if(navigator.userAgent.indexOf("Safari")!=-1){
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
															}
															if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																if(this.currTheme == "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																}
																if(navigator.userAgent.indexOf("Safari")!=-1){
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																}
															}
															if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_addusers')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
															}
															if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
																$(qtupActive).find('.qtip-tip').css('z-index',zIndex);
																if(this.currTheme == "expertusoneV2"){
																if($.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','1px');
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																}else if($.browser.msie && parseInt($.browser.version, 10)=='9'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css({"bottom":"41px","right":"20px"});
																	$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-3px"});
																}else{
																	$(qtupActive).find('.qtip-tip').css('right','-31px');
																	$(".qtip-button .admin-bubble-close").css({"left":"44px"});
																}
																}else{
																	if($.browser.msie && parseInt($.browser.version, 10)=='10'|| $.browser.msie && parseInt($.browser.version, 10)=='9'){
																		$(qtupActive).find('.qtip-tip').css({'bottom':'0','right':'-31px'});
																		$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-4px"});
																	}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																		$(qtupActive).find('.qtip-tip').css({"bottom":"40px","right":"19px"});
																		$(".qtip-button .admin-bubble-close").css({"left":"43px","top":"-3px"});
																	}else{
																		$(qtupActive).find('.qtip-tip').css('right','-31px');
																		$(".qtip-button .admin-bubble-close").css({"left":"44px"});
																	}

																}
															}

														}else if(tipPosition == 'bottomLeft' && mLeft!=0){
															var leftPos=0;
									 						var rightPos="0px";
															if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0){
																loadPos = 380;
								 								leftPos = 407;
								 								rightPos = "-14px";
								 								if(this.currTheme != "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='9'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
								 								if(this.currTheme != "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css('bottom','40px');
																	$(qtupActive).find('.qtip-tip').css('margin-bottom','38px');
																}
								 								if(this.currTheme = "expertusoneV2" && $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
								 								$(qtupActive).find('.qtip-tip').css('right',rightPos).css("left","13px");
															}else if(qtipObj.catalogVisibleId.indexOf('listvalues')>0){
																leftPos = "-80px";
																$(qtupActive).find('.qtip-tip').css('left','16px');
																if (navigator.userAgent.indexOf("Chrome")>0){
																	$(qtupActive).find('.qtip-tip').css('left','13px');
																}
																if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																	$(qtupActive).find('.qtip-tip').css('margin-bottom','38px');
																}
																if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10'){
																	$(qtupActive).find('.qtip-tip').css('bottom','0');
																}
															}
															//$(qtupActive).find('.qtip').addClass('qtip-access-control');
															$(qtupActive).find('.qtip-tip').css('right',rightPos);
															$('#'+qtipObj.catalogVisibleId+"_close").css('left',leftPos);
														}
														if(qtipObj.catalogVisibleId.indexOf("Access")>=0 || qtipObj.catalogVisibleId.indexOf("addclass")>=0
																|| qtipObj.catalogVisibleId.indexOf("editclass")>=0){
																$('#loaderdiv'+qtipObj.catalogVisibleId+"_disp").css('padding-left',loadPos);
														}
													   if(qtipObj.catalogVisibleId.indexOf("addclass")>=0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10'){
																$(qtupActive).find('.qtip-tip').css('bottom','0');
																$(".qtip-button .admin-bubble-close").css({"left":"44px","top":"-1px"});
															}else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																$(qtupActive).find('.qtip-tip').css('bottom','40px');
																$(qtupActive).find('.qtip-tip').css('right','19px');
																$(qtupActive).find('.qtip-tip').css("margin-bottom","38px");
															}
													   }
													   if(qtipObj.catalogVisibleId.indexOf('editclass')>0){
															if($.browser.msie && parseInt($.browser.version, 10)=='8'){
																$(qtupActive).find('.qtip-tip').css('margin-bottom','76px');
																$(qtupActive).find('.qtip-tip').css('margin-right','50px');
															}
															if($.browser.msie && parseInt($.browser.version, 10)=='9' || $.browser.msie && parseInt($.browser.version, 10)=='10' ){
																$(qtupActive).find('.qtip-tip').css('bottom','0');
															}
														}
													   if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','26px');
							 								}
								 						}
													   if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme == "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','26px');
							 								}
														   if($.browser.msie && parseInt($.browser.version, 10)=='8' && this.currTheme != "expertusoneV2"){
																$(qtupActive).find('.qtip-tip').css('right','32px');
																$(qtupActive).find('.qtip-tip').css('bottom','32px');
							 								}
														   if($.browser.msie && parseInt($.browser.version, 10)=='10' && this.currTheme != "expertusoneV2"){
															   $(".qtip-button .admin-bubble-close").css("top","-3px");
														   }
														 }


														if(qtipObj.catalogVisibleId.indexOf('listvaledit')>0){
															$(qtupActive).find('.qtip-tip').css('right','-12px');
															$('#'+qtipObj.catalogVisibleId+"_close").css('left','40px');
														}
														var bubbleTop = $("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top');
														bubbleTop = parseInt(bubbleTop.substring(0,bubbleTop.length-2));
														var bubblehb = $("#"+qtipObj.catalogVisibleId).css('height');
														bubblehb = parseInt(bubblehb.substring(0,bubblehb.length-2));
														if(document.getElementById('qtip_position')!=null){
															$('#qtip_position').val(qtipObj.catalogVisibleId+"#"+bubbleTop+"#"+bubblehb);
														}
											 			$.ajax({
											 				 type: "GET",
												   	         url: url,
												   	         data:  '',
												   	         success: function(data){
											 					var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bottomElements;
											 					$.extend(true, Drupal.settings, data.drupal_settings);
											 					$("#"+qtipObj.catalogVisibleId+"_disp").html(paintHtml);
											 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(qtipObj.catalogVisibleId+"_disp");

											 					var bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
											 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																bubbleTop = bubbleTop-(bubbleha-bubblehb);
																$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
																bubblehb = $("#"+qtipObj.catalogVisibleId).css('height');
																bubblehb = parseInt(bubblehb.substring(0,bubblehb.length-2));
																if(document.getElementById('qtip_position')!=null){
																	$('#qtip_position').val(qtipObj.catalogVisibleId+"#"+bubbleTop+"#"+bubblehb);
																}
																$('#'+qtipObj.catalogVisibleId+"_close").css('visibility','visible');
																$(qtupActive).find('.qtip-tip').css('visibility','visible');
																this.currTheme = Drupal.settings.ajaxPageState.theme;
																if ($.browser.msie) {
																	if($.browser.version == 9){
																	  // Conditionally set top to 0 to fix the issue reported in ticket #0032959
																	  if (qtipObj.catalogVisibleId.indexOf('qtipAttachIdqtip') < 0 &&
																	        qtipObj.catalogVisibleId.indexOf('renderTagsId') < 0 &&
																	          qtipObj.catalogVisibleId.indexOf('renderPrintCerId') < 0 &&
																	            qtipObj.catalogVisibleId.indexOf('qtipBusinessqtip') < 0) {
																		  $('#' + qtipObj.catalogVisibleId + "_close").css('top', '0px');
																	  }
																		$(qtupActive).find('.qtip-tip').addClass('qtip-tip-ie9');
																	}
																	if($.browser.version == 10){
																		if(qtipObj.catalogVisibleId.indexOf('addclass')>=0 || qtipObj.catalogVisibleId.indexOf('editclass')>=0){
																			 $('#' + qtipObj.catalogVisibleId + "_close").css('top', '-2px');
																		}
																	}
																	if($.browser.version == 8 && (qtipObj.catalogVisibleId.indexOf('qtip_addusers_visible_disp')>0 ||
																			((qtipObj.catalogVisibleId.indexOf('Accessqtip')>0) && crTheme == 'expertusoneV2'))){
																		$('#'+qtipObj.catalogVisibleId+"_close").css('top','0px');
																	}

																	if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0){
																		//var bot = ($.browser.version==7)?-8:-67;
																		if($.browser.version == 8){
																		bottomPos = (this.currTheme == "expertusoneV2") ?-5:-31;
																		rightPos = "25px";
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																		$(qtupActive).find('.qtip-tip').css('right',rightPos);
																	  }else if($.browser.version == 9 || $.browser.version == 10){
																		bottomPos = (this.currTheme == "expertusoneV2") ?-8:-68;
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																	  }
																	  else if($.browser.version == 10 && this.currTheme != "expertusoneV2"){
																		bottomPos = -68;
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos+'px');
																		$(".qtip-button .admin-bubble-close").css("top","-3px");
																	 }
													        	   }
																	if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																		var bot = ($.browser.version==8)?-5:-8;
																		$(qtupActive).find('.qtip-tip').css('bottom',bot+'px');
																		if($.browser.version==8){
																			$('.qtip-tip').css("margin-right","6px");
																			$('.qtip-tip').css("margin-bottom","37px");
																		}
																	}
																}
																if (navigator.userAgent.indexOf("Chrome")>0) {
																	if(qtipObj.catalogVisibleId.indexOf('renderTagsId')>=0 || qtipObj.catalogVisibleId.indexOf('AttachIdqtip_visible')>0
																			|| qtipObj.catalogVisibleId.indexOf('Businessqtip_visible')>0 || qtipObj.catalogVisibleId.indexOf('renderPrintCerId') >=0){
																		$('#'+qtipObj.catalogVisibleId+"_close").css('top','-10px');
																	}
																	if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																		/*--#41331 - Chorome browser qtip position fix --*/
																		bottomPos = (this.currTheme == "expertusoneV2") ? "-8px" : "-68px";
																		$(qtupActive).find('.qtip-tip').css('bottom',bottomPos);
																	}
																	if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0){
																		$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																	$('.bubble-blr').attr('style','width: 124px');
																}
																if(qtipObj.catalogVisibleId.indexOf('BannerAccessqtip')>0 && (Drupal.settings.ajaxPageState.theme == 'expertusoneV2')){
																	$('#'+qtipObj.catalogVisibleId+"_close").css('left','669px');
																	$(qtupActive).find('.qtip-tip').css('right','-17px');
																	if (navigator.userAgent.indexOf("Chrome")>0 || ($.browser.msie && $.browser.version == 9)) {
																		$('.bubble-blt').attr('style','width: 19px');
																		$('.bubble-blr').attr('style','width: 627px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0 && $.browser.msie && $.browser.version != 7){
																	$('#'+qtipObj.catalogVisibleId+"_close").css('top','0px');
																	if($.browser.version == 8){
																		$('.bubble-blt').css('width','58px');
																	}else if($.browser.version == 10 || $.browser.version == 9){
																		$('.bubble-blt').css('width','58px');
																	    $('.bubble-blm').css('width','58px');
																	    $('.bubble-blr').css('width','350px');
																	}else{
																		$('.bubble-blt').css('width','48px');
																	    $('.bubble-blr').css('width','290px');
																	}
																}
															  if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip')>0){
																  if(navigator.userAgent.indexOf("Chrome")>0 || navigator.userAgent.indexOf("Safari")!=-1){
																	$('.bubble-blt').css('width','61px');
																    $('.bubble-blr').css('width','375px');
																  }
															  }
																if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_listvalues')>0){
																	if(navigator.userAgent.indexOf("Chrome")>0){
																		$('.bubble-blt').attr('style','width: 14px');
																		$('.bubble-blr').attr('style','width: 194px');
																	}else if(navigator.userAgent.indexOf("Safari")!=-1){
																		$('.bubble-blt').attr('style','width: 17px');
																	    $('.bubble-blr').attr('style','width: 191px');
																    }
																}
																if(qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachCertification_addtemplate')>0){
																	if(navigator.userAgent.indexOf("Chrome")>0){
																		$('.bubble-blt').attr('style','width: 131px');
																	}
																	else{
																		$('.bubble-blt').attr('style','width: 164px');
																		$('.bubble-blr').attr('style','width: 362px');
																	}
																	var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
																	if(this.currTheme == "expertusoneV2" && isAtLeastIE11==1 ){
																	$(".qtip-button .admin-bubble-close").css({"top":"-2px"});
																	$(qtupActive).find('.qtip-tip').css('bottom','-8px');
																	}
																}
																if(qtipObj.catalogVisibleId.indexOf('MoveUsersqtip_visible')>0 && navigator.userAgent.indexOf("Chrome")>0){
																	$('.bubble-blt').attr('style','width: 61px');
																	$('.bubble-blr').attr('style','width: 375px');
																}
																
															if(tipPosition == 'bottomRight' && mLeft!=0) {
															if(qtipObj.catalogVisibleId.indexOf('listvalpeoedit')>0){
																leftPos = "44px";
																topPos  = "-2px";
																$(qtupActive).find('.qtip-tip').css({'z-index':zIndex,'right':'-31px'});
																$('#'+qtipObj.catalogVisibleId+"_close").css({'left':leftPos,'top':topPos});
																$('.bubble-blt').css('width','184px');
																$('.bubble-blr').css('width','0');
																$('.bubble-blm').css('width','58px');
																if(navigator.userAgent.indexOf("Chrome")>0){
																$('.bubble-blt').css('width','206px')
																}else if($.browser.version == 10 || $.browser.version == 9){
																$(qtupActive).find('.qtip-tip').css("bottom","0");
																$('.bubble-blt').css('width','185px');
																}
																
															}}
																/*if((qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')>0 || qtipObj.catalogVisibleId.indexOf('AttachNotification_addtemplate')>0) &&
																		$.browser.msie ){
																	$('#edit-subject').focus();
																}*/
																// Fix for 0014517: Issue in Class Admin while adding multiple classes - Added by Vincent July 30, 2012
																if(qtipObj.catalogVisibleId.indexOf('addclass')>=0 || qtipObj.catalogVisibleId.indexOf('editclass')>=0
																		|| (qtipObj.catalogVisibleId.indexOf('AttachNotification_edittemplate')) || (qtipObj.catalogVisibleId.indexOf('AttachCertification_edittemplate'))){
																		
																	var tipTop = $('#'+qtipObj.catalogVisibleId).offset().top;
																	var dScrollTop = $("#modal-content").scrollTop();
																	if(tipTop<=0){
																		var toScroll=dScrollTop-Math.abs(tipTop);
																		$("#modal-content").scrollTop(toScroll);
																		bubbleTop=bubbleTop-tipTop;
																		bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
													 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																		bubbleTop = bubbleTop-(bubbleha-bubblehb);
																		$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
																	}
																}
																Drupal.attachBehaviors();
											 					bubbleha = $("#"+qtipObj.catalogVisibleId).css('height');
											 					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
																bubbleTop = bubbleTop-(bubbleha-bubblehb);
																$("#"+qtipObj.catalogVisibleId).closest(".qtip-active").css('top',bubbleTop);
											 					vtip();
											 					if(data.status_message != '' && data.cloned != 0){
											 						$("#bubble-face-table #show_expertus_message").html(data.status_message);
											 					}
										   	                 }
											   			});
											 			//remove style attr border and background to add/edit qtip
											 			$('.qtip-contentWrapper,.qtip-title').each(function(){
															$(this).removeAttr("style");
															$('.qtip-wrapper,.qtip-content').css("overflow","visible");
															if($.browser.version == 8){
																$('.qtip-content').css("padding-bottom","43px");
																if(qtipObj.catalogVisibleId.indexOf('AttachIdqtip_listvalues')>0)
																$('.qtip-tip').css("margin-bottom","76px");
															}
															$('.qtip').css("padding-bottom","24px");
															$(qtupActive).find('.qtip-tip').css("margin-left","-33px");
										          	    });
										 			}
									         }
									      },
									     show:{
											when:{
													event:'click',
													target:$("#"+popupId)
												},
											ready: true // Needed to make it show on first mouseover event
											//effect:'slide'
										 },
										 hide: {
												when:{
										   			event:'',
										   			target:''
												}
										//effect:'slide'
										},
										position: {
										    corner:{
										    	   	target: 'topMiddle',
										    	   	tooltip: tipPosition
										    	  }
										},
										style: {
											width:wBubble,
											height:hBubble,
											background: 'none',
											color: '#333333',
												tip: {
										         corner: tipPosition
												}
									   }
								 });
							}
						}
					}catch(sp){
						//alert(sp.toString());
					}
				},
				getQtipDiv : function(qtipObj,isVCSession,event) {
					var xCoordinate ='';
					var yCoordinate ='';
					var popupId 	= qtipObj.popupDispId;
					var qtipLen 	= this.qtipLenth;
					var inArrQtip = $.inArray(popupId, this.qtipLoadSet);
					this.qtipLoadSet[qtipLen] = popupId;
					this.qtipLenth = this.qtipLenth+1;
					var entityType = qtipObj.entityType;
					var entityId   = qtipObj.entityId;
					var courseId = qtipObj.courseId!=undefined?qtipObj.courseId:'';
					var url 			  = resource.base_host+'?q='+qtipObj.url; // resource.base_host+'?q=administration/catalogaccess/33/cre_sys_obt_crs';
					var wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
					var hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
					var setwBubble        = wBubble-20;
					var setBtmLeft        = (setwBubble-104)/2;
					var qtipLeftPos       = (wBubble > 700) ? 400 : (setBtmLeft + 20);
					var tipPos = qtipObj.tipPosition;
					var catalogVisibleId = qtipObj.catalogVisibleId;
					var mLeft = 0;
					var tipPosition;
					var qtipTopBottomDisplay;
					var topElements,bottomElements;
					var assignHtml  = qtipObj.assignHtml;
					var mtobj=this;
					var rowVal = qtipObj.rowVal;
					var qtip_tbl_clsName = qtipObj.qtip_tbl_clsName!=undefined ? qtipObj.qtip_tbl_clsName:'';
					var loaderId = (qtipObj.sessionPopupId == null || qtipObj.sessionPopupId == '') ?  popupId :qtipObj.sessionPopupId;
					$('body').live('click', function(e){
			    		xCoordinate = e.pageX;
			    		yCoordinate = e.pageY;
			    	});
					//console.log("qtipTopBottomDisplay001: "+ qtipObj.tipPosition);
					//EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(qtipObj.catalogVisibleId+"_disp");
					// Added by Vincent for #0021894
					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(loaderId);
					if(isVCSession>=0){
						var i=isVCSession;
						var timeZone = $('#time_zone_'+i).val();
						var session_meetingid = $("#session_meetingid_"+i).val();
						var session_attendeepass = $("#session_attendeepass_"+i).val();
						var session_presenterpass = $("#session_presenterpass_"+i).val();
						var session_attendeeurl = $("#session_attendeeurl_"+i).val();
						var session_presenterurl = $("#session_presenterurl_"+i).val();
						var hid_session_details_id = $("#hid_session_details_id_"+i).val();

						var formDetail = '{"timezone":"'+timeZone+
									'","session_meetingid":"'+session_meetingid+
									'","session_attendeepass":"'+session_attendeepass+
									'","session_presenterpass":"'+session_presenterpass+
									'","session_attendeeurl":"'+escape(encodeURIComponent(session_attendeeurl))+
									'","session_presenterurl":"'+escape(encodeURIComponent(session_presenterurl))+
									'","instructor_id":"'+hid_session_details_id+'"}';

						//var tt = encodeURIComponent(formDetail)
						url = url + "/"+formDetail;
					}
					if(catalogVisibleId.indexOf('addpermissions')>=0){
						$('#loaderdiv'+popupId).css('top','-80px');
					}
					if(popupId.indexOf('qtip_visible_disp_addsession_iprange')>=0)
					{
						$('#loaderdiv'+popupId).css('left','180px').css('top','-60px');
					}
					if (popupId.indexOf("exp_meeting_qtip_visible_disp_addpresenter_0_presenter") >= 0) {
						$('#loaderdiv'+popupId).css('left','250px').css('top','-78px');
					}
					$.ajax({
		 				 type: "GET",
			   	         url: url,
			   	         data:  '',
			   	     //    async: false,
			   	         success: function(data){
							//alert('X='+xCoordinate+' Y='+yCoordinate);
			   	        	 
			   	        	if(qtipObj.qtipClass != 'admin-qtip-access-parent lrn_cls_vct_web' && qtipObj.qtipClass != 'admin-qtip-presenter-access-parent') {			   	        	 
			   	        		$(".active-qtip-div").remove();
			   	        	}			   	        	
			   	        	var span_width1 = $('.admin-addanother-session-details-info').width();
			   	        	var span_width2 = $('.chosen-meeting-type').width();
							var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message'></div>"+data.render_content_main+"</div>"+bottomElements;
							var contentDiv = qtipObj.catalogVisibleId+"_disp";
							
														
							if(qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_exp' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_oth' || qtipObj.qtipClass == 'admin-qtip-presenter-access-parent') {
								var repstr = qtipObj.qtipClass;
								var replacedstr = repstr.replace(' ','-');
								$("#"+popupId).append("<div id="+replacedstr+" class='bottom-qtip-tip active-qtip-div set-wbubble-left' ></div>");
							} 
							else {
								$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
							}
							
							if(qtipObj.qtipClass == 'display-message-positioning' || qtipObj.qtipClass == 'pwdstrength-admin-popup' || qtipObj.qtipClass == 'admin-qtip-presenter-access-parent' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_exp' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent lrn_cls_vct_oth') {
								$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div "+qtipObj.qtipClass+"'></div>");
							}else{
								$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
							}
							$("#"+contentDiv).html(paintHtml);
							if(catalogVisibleId.indexOf('addroletousers')>=0){
								$("#paintContent"+popupId).css('min-height','225px');
							}
							$(".bottom-qtip-tip").css('bottom','0px').css('position','absolute').css('z-index','101');
							//$('#upload_browse_and_url').find('#search-list-title-keyword').css('z-index','0');
							//$('#upload_detail_container').find('.content-browse-upload').css('z-index','0');
							if (qtipObj.qtipClass == 'add-key-word-popup') {
								$("#"+contentDiv).css('position','absolute').css('left','-'+parseInt(qtipLeftPos-98)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (qtipObj.qtipClass == 'pwdstrength-admin-popup') {
							  $("#"+contentDiv).css('position','absolute').css('left','-'+parseInt(qtipLeftPos-48)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("1_qtip_visible_disp_addsession_iprange")==0) {
								$("#"+contentDiv).css('position','absolute').css('left',parseInt(qtipLeftPos)-133+'px').css('bottom','48px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("2_qtip_visible_disp_addsession_iprange")==0) {
								$("#"+contentDiv).css('position','absolute').css('left','-6px').css('bottom','30px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("qtip_visible_disp_iprange_1_iprange_disp")==0) {
								$("#"+contentDiv).css('position','absolute').css('left',parseInt(qtipLeftPos-75)+'px').css('bottom','40px').css('z-index','100');
							}
							else if (catalogVisibleId.indexOf("EditSessionIdqtip") < 0) {
								$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('bottom','40px').css('z-index','100');
							}
							var p = $("#"+popupId);
							var position = p.position();
							var divHeight = $("#"+contentDiv).height();
		 					var parentTopPos = position.top + 130;
		 					//alert("top Pos: "+parentTopPos+" | divHeight: "+divHeight);
		 					if($("#"+popupId+" .tab-title").width() > 0 ){
								 var labelWidth = ($("#"+popupId+" .tab-title").width())/2;
								 var bubbleWidth = 32;
								 var setbubblePosition = labelWidth - bubbleWidth;
								 $(".set-wbubble-left").css('left', setbubblePosition +'px');
						    }
		 					if(catalogVisibleId.indexOf("MoreEditSession")>0||catalogVisibleId.indexOf("EditSessionIdqtip")>0){
		 						$('.session-add-list').hide();
		 					}

		 				   var dispop=$('.add_session_popup').css('display');
		 				   var SRecrowCount = $('#admin-data-grid').find('.admin-datagrid-session-with-addanother .ui-jqgrid-bdiv tr').length;
		 				   var sessionScrId=$('#admin-data-grid').parent('div').attr("class");

		 				   if (parentTopPos < divHeight && position.top!=0 || catalogVisibleId.indexOf("AddMoreSessionIdqtip")>0)  {
		 					  //Add another session qtip working code
		 					  if(sessionScrId=="catalog-class-session-basic-addedit-form-container" && SRecrowCount > 5){
		 							qtipTopBottomDisplay = "tipfaceUp";
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0);
									$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('top','42px').css('bottom','0px').css('z-index','100');

		 					  }else{
		 					   if ($.browser.msie && $.browser.version == 7) {
									qtipTopBottomDisplay = "tipfaceUp";
									$(".narrow-search-multi-action-container div").css("position"," ");
									$(".active-qtip-div").remove();
									$("body").append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
									$("body").append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
									$("#"+contentDiv).prev().css('top',+yCoordinate+'px').css('position','absolute').css('left',+(xCoordinate-30)+'px').css('z-index','101');
									$(".bottom-qtip-tip").remove();
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0)
										$("#"+contentDiv).css('top',+(yCoordinate+29)+'px').css('position','absolute').css('left',+(xCoordinate-setBtmLeft)+'px').css('z-index','100');
		 						} else {
		 							qtipTopBottomDisplay = "tipfaceUp";
									$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
									//add script for qtip arrow come twoice in add another session
									 if(dispop=="block") {
			 						 $("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute');
			 						 $("#"+popupId).last('div').prev('.bottom-qtip-tip-up').remove();
			 					        } else	{
										 $("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left','-10px');//add minus 10px for tipfaceup qtip
									 }
									$(".bottom-qtip-tip").remove();
									if(catalogVisibleId.indexOf("EditSessionIdqtip") < 0);
									$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('top','42px').css('bottom','0px').css('z-index','100');
								}
		 					}
		 				  }
		 					if( qtipObj.tipPosition == 'tipTopLeft'){
		 						qtipTopBottomDisplay = "tipfaceUp";
								//Add another session qtip working code
								if(sessionScrId=="catalog-class-session-basic-addedit-form-container" && SRecrowCount > 5){
								    $(".bottom-qtip-tip").remove();
									$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
								    $("#"+popupId+" .bottom-qtip-tip").css('bottom','2px').css('left','55').css('z-index','101').css('position','absolute');
									//$("#"+popupId+" .active-qtip-div").css('bottom','30px');
							    	$("#"+popupId+" .active-qtip-div").css('left','55px'); /*viswanathan changed 76px to 55px for #74374*/
									$("#"+contentDiv).css('position','absolute').css('left','-19px').css('top','').css('bottom','40px').css('z-index','100');
								}
								else{
								    $("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
								    
								    
								    if(qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_web' || qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_oth' || qtipObj.qtipClass == 'admin-qtip-access-parent another-lrn_cls_vct_exp') {
								    var repstr = qtipObj.qtipClass;
								    var replacedstr = repstr.split(' ');
									$("#"+popupId+" .active-qtip-div").addClass(replacedstr[1]);
								    
								    	if ((navigator.userAgent.toLowerCase().indexOf('chrome') > -1) || ($.browser.msie && $.browser.version < 8)) {
								    		$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left',parseInt((span_width1+span_width2)-13)+'px');
								    	} else {
								    		$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute').css('left',parseInt((span_width1+span_width2)-9)+'px');
								    	}
								    } else {
								    	$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('position','absolute');
								    }
								    
								    
								    $(".bottom-qtip-tip").remove();
								    $("#"+contentDiv).css('position','absolute').css('left','-'+19+'px').css('top','42px').css('bottom','0px').css('z-index','100');
								}
						    } else if(qtipObj.tipPosition == 'tipTopRight') {

						    	$("#"+popupId+" .bottom-qtip-tip-up").css('left','-20px');
						    	if(catalogVisibleId.indexOf("EditSessionIdqtip") >0){
						    		if(catalogVisibleId.indexOf("qtip_EditSessionIdqtipvisible_disp_iprange")==0)
				    				{
				    				$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','').css('bottom','-'+12+'px').css('z-index','100');
				    				$("#"+popupId+" .bottom-qtip-tip").css('bottom','').css('left','-26px').css('z-index','101').css('position','absolute');
				    				}
						    		else
						    		{
						    		//edit and more session qtip working code
						    		if(rowVal>4){
						    			$("#"+popupId+" .bottom-qtip-tip").css('bottom','2px').css('left','-26px').css('z-index','101').css('position','absolute');
						    			$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','').css('bottom','40px').css('z-index','100');
						    		}else{
						    			$(".bottom-qtip-tip").remove();
						    			$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
						    			$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('left','-26px').css('position','absolute');
						    			$("#"+contentDiv).css('position','absolute').css('left','').css('right','-'+34+'px').css('top','42px').css('bottom','0px').css('z-index','100');
						    			
						    	    	}
						    		 }
						    	}else{
						    	  $("#"+contentDiv).css('position','absolute').css('left','-'+375+'px').css('top','42px').css('bottom','0px').css('z-index','100');
						    	}
						    }else if(qtipObj.tipPosition == 'tipfaceMiddleRight' && catalogVisibleId.indexOf("AddNewSessionIdqtip") >0 ) {
						       	var getSessTop=$("#"+catalogVisibleId+"_disp").offset();
						       	if(parseInt(getSessTop.top) < "30"){
						    		qtipTopBottomDisplay = "tipfaceUp";
						    		$(".bottom-qtip-tip").remove();
					    			$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
					    			$("#"+popupId+" .bottom-qtip-tip-up").css('top','13px').css('left','300px').css('position','absolute');
					    			$("#"+contentDiv).css('position','absolute').css('left','0').css('right','').css('top','42px').css('bottom','0px').css('z-index','100');
					       			//Drupal.ajax.prototype.commands.CtoolsModalAdjust();
					    			
						    	}else{
						    	qtipTopBottomDisplay = "tipfaceUp";
						    	$("#"+popupId+" .active-qtip-div").css('bottom','32px');
						    	if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						    		$("#"+popupId+" .active-qtip-div").css('left','319px');
						    		$("#"+contentDiv).css('position','absolute').css('left','0').css('bottom','70px').css('z-index','auto');
						    	} else{
						    		$("#"+popupId+" .active-qtip-div").css('left','210px');
						    		$("#"+contentDiv).css('position','absolute').css('left','0').css('bottom','70px').css('z-index','unset');
						    	}
						    	}
						    }
							switch(qtipTopBottomDisplay) {
							case 'tipfaceUp':
								wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
								hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
								setwBubble        = wBubble-20;
								setBtmLeft        = (setwBubble-104)/2;
								mLeft			  = (qtipObj.catalogVisibleId.indexOf('editclass')>0)?45:(qtipObj.catalogVisibleId.indexOf('AttachCrsIdqtip')>0)?58:82;
								//Set buble tool tip arrow position
								tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
								topElements = '<a class="qtip-close-button" onclick="$(\'#root-admin\').data(\'narrowsearch\').removeActiveQtip(\''+catalogVisibleId+'\');"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table" class="'+qtip_tbl_clsName+'"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
								bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
								$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
							break;
							default:
								wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
								hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
								setwBubble        = wBubble-20;
								setBtmLeft        = (setwBubble-104)/2;
								//Set buble tool tip arrow position
								tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
								topElements = '<a class="qtip-close-button" onclick="$(\'#root-admin\').data(\'narrowsearch\').removeActiveQtip(\''+catalogVisibleId+'\');"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table" class="'+qtip_tbl_clsName+'"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
								bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
								$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");
							}
							var paintHtml = topElements +
                              "<div id='paintContent" + popupId + "'>" +
                                "<div id='show_expertus_message'></div>" +
                                data.render_content_main +
                              "</div>" +
                              bottomElements;
							var contentDiv = qtipObj.catalogVisibleId+"_disp";
              				if(data.drupal_settings) {
							   $.extend(true, Drupal.settings, data.drupal_settings);
							}
              				EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(loaderId);
							$("#"+contentDiv).html(paintHtml);
							//EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(qtipObj.catalogVisibleId+"_disp");

							//For getting default meeting details on VC session details page
							if(assignHtml == 'meeting-container'){
								mtobj.getMeetingType();
							}
							Drupal.attachBehaviors();
							if(catalogVisibleId.indexOf('addpermissions')>=0 || catalogVisibleId.indexOf('addlistpermissions')>=0){
								$('#admin-add-scroll').jScrollPane({});
								/*if(catalogVisibleId.indexOf('addpermissions')>=0){
									$('#permission-cancel-save-btn').css('margin-right','-4px')
								}else{
									$('#permission-cancel-save-btn').css('margin-right','18px')
								}*/
							} else if(catalogVisibleId.indexOf('qtipAttachIdqtip_visible_disp')>=0){
								var heigt = $("#scrolldiv").height();
								if(heigt > 80){
									$('#catalog-attachment-disp-container #scrolldiv').css('height','100px');
									$('#catalog-attachment-disp-container #scrolldiv').jScrollPane({});
									}
								}
							if(catalogVisibleId.indexOf('qtipAddSessionIdqtip_visible_disp')>=0){
								$(".bottom-qtip-tip").css('bottom','2px').css('left','0').css('position','absolute').css('z-index','101');
							}else if (catalogVisibleId.indexOf("1_qtip_visible_disp_addsession_iprange")==0) {
								$(".bottom-qtip-tip").css('bottom','20px').css('left','195px').css('position','absolute').css('z-index','101');
							}else if (catalogVisibleId.indexOf("2_qtip_visible_disp_addsession_iprange")==0) {
								$(".bottom-qtip-tip").css('bottom','2px').css('left','0').css('position','absolute').css('z-index','101');
							}
							} //end $.ajax success

						});
                vtip();
				},
				visiblePopup : function(qtipObj) {
					try{
						var url = resource.base_host+'?q='+qtipObj.url;
						var popupId 	= qtipObj.popupDispId;
						//var catalogVisibleId = qtipObj.catalogVisibleId;
						var entId = qtipObj.entityId;
						var qtipScrollId = qtipObj.scrollid;

						$.ajax({
			 				 type: "GET",
				   	         url: url,
				   	         data:  '',
				   	         success: function(data){
			 					//var paintHtml = bpTop+"<div id='paintContent"+popupDispId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bpBot;
			 					$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(data.render_content_main);
			 					$.extend(true, Drupal.settings, data.drupal_settings);
			 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("paintContent"+popupId);

			 					if(qtipScrollId != '' && qtipScrollId != undefined && qtipScrollId != null){
			 						var qtipScrollType = (qtipObj.scrolltype == 'class') ? '.' : '#';
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane('destroy');
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane({});
			 					}
								Drupal.attachBehaviors();
			 					vtip();
			 					
			 					if('[qtipObj.linkid^=visible-sharelink]') {
			 						if (navigator.appVersion.indexOf("Safari")!= -1 && ($(window).height() < 742))
			 							$('#program-tp-basic-addedit-form-container .survey-attach-grid-wrapper .ui-jqgrid .jqgrow #bubble-face-table td.bubble-c').css('height','33px');
			 								 						
									if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr"]) != -1) {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-33px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-26px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-54px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-32px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-42px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') { 
										if ((navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1) || (navigator.appVersion.indexOf("Trident/7.0")!= -1))
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}else {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-28px');
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}
								}
		   	                 }
			   			});

					}catch(e){
						alert(e);
					}

				},
				timepickerLoad : function(){
				try{
					$('input.exp-timepicker').click(function(){
						var attributeId = $(this).attr('id');
						var expDropDownId = 'exp-dropdown-'+attributeId;
						var c = $(this).attr('class');
						var x= c.indexOf('callback')>=0?c.substring(c.indexOf('callback')):'';
						var y = x.substring(x.indexOf('-')+1,(x.indexOf(' ')>0?x.indexOf(' '):x.length));
						var hours, minutes;
						$('.exp-timepicker-selection').css('display', 'none');
						if($(this).val() == Drupal.t('hh:mm')){
							$(this).val('');
						}
						if($('#'+expDropDownId).length >0){
							if($('#'+expDropDownId).is(':visible') == true){
								$('#'+expDropDownId).css('display', 'none');
							} else {
					 			$('#'+expDropDownId).css('display', 'block');
							}
						} else {
							$(this).after('<div id="'+expDropDownId+'" data="'+attributeId+'" class="exp-timepicker-selection"><ul></ul></div>');
							for(var i = 0 ;i <= 1425; i += 15){
								hours = Math.floor(i / 60);
								minutes = i % 60;
								if (hours < 10){
								    hours = '0' + hours;
								}
								if (minutes < 10){
								    minutes = '0' + minutes;
								}
								$('#'+expDropDownId+' ul').append('<li>'+hours + ':' + minutes + '</li>');
						    }
							$('div.exp-timepicker-selection li').click(function(){
								var attributeId = $(this).closest('.exp-timepicker-selection').attr('data');
								var selectedTime = $(this).html();
								$('#'+attributeId).val(selectedTime);
								$(this).closest('.exp-timepicker-selection').css('display', 'none');
								if(y!=null && y!=undefined && y!='undefined' && $.trim(y) !=''){
									eval(y+'('+attributeId+')');
								}
							});
						}
					});
					
  			$('body,#admin-data-grid td').bind("click",function(event){
                  if($(event.target).hasClass('exp-timepicker') != true){
                   $('.exp-timepicker-selection').css('display', 'none');
                  }	
				  if($('#exp-dropdown-start_hours').is(':visible')) {
					var Index = $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex();
				    $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex(100);
				  } else {
					var Index = $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex();
				    $('#exp-dropdown-start_hours').parents('.active-qtip-div').siblings('.bottom-qtip-tip').zIndex(101);
				  }
               });
               
           }catch(e){
			 alert(e);
			}
                  
				},
                /* added function to randomize li tags while switching between meetings during session creation */
				reverseSessionType : function(){
					if($(".session-list-ul li").length == 1 )
						{
							$(".session-list-ul li").addClass("single_meeting");
						}
					$('.session_type_list').click(function(){
						var liId = $(this).attr("id");
					    $('.session-list-ul li:eq(0)').removeClass('admin-save-button-middle-bg');
					    $('.session-list-ul li:eq(0)').text($('.session-list-ul li:eq(0)').text().replace(Drupal.t("LBL287"), ''));
						$('.session-list-ul span.session-list').append($('.session-list-ul li:eq(0)'));
						$('#'+liId).text(Drupal.t('LBL287')+' '+$('#'+liId).text());
						$('#'+liId).addClass('admin-save-button-middle-bg');
						$('.session-list-ul span.admin-save-button-left-bg').after($(this));
						$('.session-list-ul span.session-list').hide();
						if(document.getElementById('lrn_cls_vct_oth') && liId != "lrn_cls_vct_oth"){
							$('.session-list-ul span.session-list').append($('#lrn_cls_vct_oth'));
						}

						if(liId =='lrn_cls_vct_web') {
							$('.admin-empty-text-msg').text(Drupal.t('MSG756'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','95px');
						} else if(liId =='lrn_cls_vct_oth') {
							$('.admin-empty-text-msg').text(Drupal.t('MSG757'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','95px');
						} else {
							$('.admin-empty-text-msg').text(Drupal.t('MSG800'));
							$('.expertus-meeting-button-container div.add_session_button_div div.dropdownadd-dd-list-arrow').css('left','130px');
						}
			
						vtip();
						//$(".session-list-ul li:not(#"+liId+")").randomize();
					});

				},

				removeActiveQtip: function(id) {
				  // Handle cancel policy qtip popup close
				  var cancelPolicyQtip = $('.active-qtip-div').parents('.cancel-policy-popup-wrapper');
				  if (cancelPolicyQtip) {
				    if(id=='qtip_visible_disp_addpresenter_0_presenter') {
						  $('#exp_meeting_qtip_visible_disp_addpresenter_0_presenter').empty();
					 } else {
						  $('.active-qtip-div').remove();
					 }
				    if (cancelPolicyQtip.hasClass('add-another-cancel-policy-popup')) {
  				    //alert('closing cancel policy add another qtip popup');
  				    var defaultLabel = $('#add-another-cancel-policy-label').data('default-label');
  				    $('#add-another-cancel-policy-label').text(defaultLabel);
				    }
				    return;
				  }

				  $('.active-qtip-div').remove();
					$(".narrow-search-multi-action-container div").css("position","relative");
					if(id!=null){
						if(id.indexOf('AddMoreSessionId')>=0){
							$('#meeting-title').html('');
						}else if(id.indexOf('addroletousers')>=0){
							$("#narrow-search-results-holder").trigger("reloadGrid",[{page:1}]);

						}
					}
				},

				getInlineEdit : function(inlineData){
                    if(inlineData){
                    	this.actualInlineData = inlineData;
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                        var html ="";
                        var callBlur = '$("#root-admin").data("narrowsearch")';
                        var widthAnchor = $("#tag-name-div-"+this.inlineEditData.tagEntityId+" a").width();
                        var heightAnchor = $("#tag-name-div-"+this.inlineEditData.tagEntityId+" a").height();
                        //html += '<input type ="text" class="tag-edit-txtbox" id="tag-list-name-id-'+this.inlineEditData.tagEntityId+'" style = "width:'+widthAnchor+'px;height:'+heightAnchor+'px;" value ="'+decodeURIComponent(this.inlineEditData.tagName)+'" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateTagList(this);" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateTagsOnKeyDown(event,this);" />';
                        html += '<input type ="text" maxlength="150" class="tag-edit-txtbox" id="tag-list-name-id-'+this.inlineEditData.tagEntityId+'" style = "width:'+widthAnchor+'px;height:'+heightAnchor+'px;" value ="" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateTagList(this);" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateTagsOnKeyDown(event,this);" />';
                        $("#tag-name-div-"+this.inlineEditData.tagEntityId).html(html);
    					$('#tag-list-name-id-'+this.inlineEditData.tagEntityId).focus();
    					$("#tag-list-name-id-"+this.inlineEditData.tagEntityId).val(decodeURIComponent(this.inlineEditData.tagName));
                    }
                },

                updateTagsOnKeyDown : function(evt,updateTag){

            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	updateTag.blur();
            	        evt.preventDefault();
            	        evt.stopPropagation();
            	        return false;
            	    }
            	},

                updateTagList : function(updatedTag){
            		var obj = this;
            		this.actualInlineData = this.actualInlineData.replace(/"/g,"&quot;");
                    var inlineData = this.inlineEditData;
                    var updatedTagName = $(updatedTag).attr("value");
                    var exist_tag_name = decodeURIComponent(inlineData.tagName);
//                    var allowedChars = /^[a-z A-Z0-9@*.,_\-\'\|]+$/;
//                    var restrictedSet = /[~`!#$%^&()+=\\}\]{\[\":;?\/><]/;	//for 34642: Handling of multi-language tags in cloud based on learner's preferred language
//                    if(restrictedSet.test(updatedTagName)) {
//                    	var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 250px;"><span>' +Drupal.t('LBL191') + ' ' + Drupal.t('MSG639') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
//                    	$("#show_expertus_message").append(error_msg);
//                    	return false;
//                    }
                    if(updatedTagName != '' && $.trim(updatedTagName) != "" && updatedTagName != exist_tag_name){
                    	newupdatedTagName = updatedTagName.replace(/\//g,Drupal.settings.custom.EXP_AC_SEPARATOR);
	                    url = obj.constructUrl("administration/update-tags/"+inlineData.entityId+"/"+inlineData.emptyId+"/"+inlineData.entityType+"/"+encodeURIComponent(newupdatedTagName)+"/"+inlineData.tagEntityId);
	                    $.ajax({
	                        type: "POST",
	                        url: url,
	                        data:  '',
	                        success: function(result){	                        	
	                        	if(result == 'error'){
	                        		var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li><span>' +Drupal.t('LBL191') + ' ' + Drupal.t('LBL271') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
                    				$("#paintContentVisiblePopup #show_expertus_message").append(error_msg);
                    				return false;
	                        	}else{                  		
		                            obj.inlineEditData.tagName = encodeURIComponent(updatedTagName); //'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","tagName":"'+updatedTagName+'","tagEntityId":"'+inlineData.tagEntityId+'","tagId":"'+inlineData.tagId+'"}';
		                            //obj.inlineEditData = obj.inlineEditData.replace('"',"&quot;");
		                            var html = "";
		                            obj.actualInlineData = obj.actualInlineData.replace(/&quot;/g,'"');
		                            obj.actualInlineData = eval(obj.actualInlineData);
		                            obj.actualInlineData.tagName = escape(updatedTagName);
		                            obj.actualInlineData = "data= "+EXPERTUS_SMARTPORTAL_AbstractManager.convertJsonToString(obj.actualInlineData);
		                            obj.actualInlineData = obj.actualInlineData.replace(/"/g,"&quot;");
		                            html += '<a href="javascript:void(0);" id="tag-list-name-id-'+inlineData.tagEntityId+'" name="tag-list-name-id-'+inlineData.tagEntityId+'" class = "tag-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEdit(\''+ obj.actualInlineData +'\');">'+updatedTagName+'</a>';
		                            $("#tag-name-div-"+inlineData.tagEntityId).html(html);
	                           }
	                        }
	                    });
                    }else{
                    	var html = "";
                    	obj.actualInlineData= addslashes(obj.actualInlineData);
                    	obj.actualInlineData.tagName = updatedTagName;
                    	obj.actualInlineData = obj.actualInlineData.replace(/"/g,"&quot;");
                        html += '<a href="javascript:void(0);" id="tag-list-name-id-'+inlineData.tagEntityId+'" name="tag-list-name-id-'+inlineData.tagEntityId+'" class = "tag-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEdit(\''+ obj.actualInlineData+'\');">'+exist_tag_name+'</a>';
                        $("#tag-name-div-"+inlineData.tagEntityId).html(html);
                    }
                },


                getInlineEditAttachedCourse : function(inlineData,e){
                	//e.preventDefault();
                	//console.log("double click trigger ");
                    if(inlineData){
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                    }
                   
                   //0073705:single and double quotes
                    var fullGroupNameData = this.inlineEditData.fullGroupName;
                    fullGroupNameData = fullGroupNameData.replace(/"/g, '&quot;');


          
                    tpTabDoubleClickData = {
                    		'selector':"#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag,
                    		'data':$("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).html(),
                    		'oldName': $("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag + " a span").text()
                    } 
                    var html ="";
                    var callBlur = '$("#root-admin").data("narrowsearch")';
                    var inline_txt = $("#attachedcourse-name-div-"+this.inlineEditData.courseId+"-"+this.inlineEditData.recertifyFlag+" #expires_duration").html();
                    html += '<input maxlength="250" type ="text" class="attachedcourse-edit-txtbox" id="attachedcourse-list-name-id-'+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag+'" value ="'+fullGroupNameData+'" onkeydown="$(\'#root-admin\').data(\'narrowsearch\').updateAttachedCourseGroupKeyDown(event, this);" onblur="$(\'#root-admin\').data(\'narrowsearch\').updateAttachedCourseGroup(this);">';
                    html += '<span id ="expires_duration">'+inline_txt+'</span>';
                    $("#attachedcourse-name-div-"+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).html(html);
                    $('#attachedcourse-list-name-id-'+this.inlineEditData.courseId+'-'+this.inlineEditData.recertifyFlag).focus();
                },

                updateAttachedCourseGroupKeyDown : function(evt, updatedGroup){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedCourseGroup(updatedGroup);
            	        evt.preventDefault();
            	        return false;
            	    }

                },

                updateAttachedCourseGroup : function(updatedGroup){
                    var inlineData = this.inlineEditData;
                    var updatedGroupName = $.trim($(updatedGroup).attr("value"));
                    var oldGroupName = this.inlineEditData.groupName;
                    this.inlineEditData.groupName = updatedGroupName;
                    var obj = this;
                    var fieldname = Drupal.t("LBL1003")+' '+Drupal.t("LBL3060");
                    var characterlist = "a-z A-Z 0-9 @ * ' . _ -";
                    var grpNameExists = 0;
                       /**Change by: ayyappans
                     * #37078: UI Issue in TP when adding any special characters in the group name
                     * Root Cause: If the group name contains special characters " and ' the object string gets broken and causes javascript error.
                     * Fix: validation has been added for Group name to contain alphanumerics, spaces and speacial characters ._-@*
                     * 		maxlength characters property added to textbox attachedcourse-list-name-id-?
                     * */
                   if(Drupal.settings.user.language == 'en-us')
                	   {
                	   var allowedChars = /^[a-z A-Z0-9@*\'\"._-]+$/;
                	   if ( updatedGroupName == '' || !allowedChars.test(updatedGroupName)) {
                           var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('MSG811', {'@fieldname' : fieldname , '@character list' : characterlist }) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                           $("#show_expertus_message").html(error_msg);
                           return false;
                         }
                	   }
                   else {
                	   var allowedChars = /^[a-z A-Z0-9@*\'\"._-]+$/;;
                	   if (updatedGroupName == '' || !allowedChars.test(updatedGroupName)) {
                           var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('MSG811', {'@fieldname' : fieldname , '@character list' : characterlist }) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                           $("#show_expertus_message").html(error_msg);
                           return false;
                       }
                   }
                   if(updatedGroupName.length > 49){
                   		var current_length = updatedGroupName.length;
                   		 var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  Drupal.t('!name cannot be longer than %max characters but is currently %length characters long.', {'!name' : fieldname , '%max' : 50 ,'%length' : current_length}) + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                          $("#show_expertus_message").html(error_msg);
                          return false;
                   }
                   //Group name validation
                   $('#program_attach_tabs ul li a').each(function(i) {
                   		var validationmsg = Drupal.t("LBL1003")+' '+Drupal.t("LBL107")+Drupal.t("LBL271");
				    	if (this.text == updatedGroupName) {
				    	 	grpNameExists = 1;
				    	}
					});
					if(grpNameExists == 1){
						var validationmsg = Drupal.t("LBL1003")+' '+Drupal.t("LBL107")+Drupal.t("LBL271");
						var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +  validationmsg + "</span></li></ul><div onclick=\"$('body').data('learningcore').closeErrorMessage();\" class=\"msg-close-btn\"></div></div></div></div>";
                        $("#show_expertus_message").html(error_msg);
                        return false;
				 	}
					
                   var groupName = decodeURIComponent(updatedGroupName);
                    //37078 : End of code change
                    if (updatedGroupName == "No Group Name" || updatedGroupName == "") {
                        updatedGroupName = oldGroupName;
                        this.inlineEditData.groupName = updatedGroupName;
                        full_grp_name = addslashes(inlineData.fullGroupName);
                        group_name = addslashes(inlineData.groupName);
                        var html = "";
                        var datanew = 'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","groupName":"'+group_name+'","fullGroupName":"'+full_grp_name+'","courseId":"'+inlineData.courseId+'","recertifyFlag":"'+inlineData.recertifyFlag+'"}';
                        datanew = unescape(datanew).replace(/"/g, '&quot;');
                        html += '<a href="javascript:void(0);" id="attachedcourse-list-name-id-'+inlineData.courseId+'-'+inlineData.recertifyFlag+'" name="attachedcourse-list-name-id-'+inlineData.courseId+'-'+inlineData.recertifyFlag+'" class = "attachedcourse-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedCourse(\''+datanew+'\',this);">'+updatedGroupName+'</a>';
                        $("#attachedcourse-name-div-"+inlineData.courseId+'-'+inlineData.recertifyFlag).html(html);
                    } else {
                    	obj.createLoader('attach_course_dt');
                        url = obj.constructUrl("administration/update-attachedcourse-group/"+inlineData.entityId+"/"+inlineData.entityType+"/"+updatedGroupName+"/"+inlineData.courseId+"/"+inlineData.recertifyFlag);
                        $.ajax({
                            type: "POST",
                            url: url,
                            data:  '',
                            success: function(result) {
	                        	tpTabDoubleClick = false;
	                        	if(tpTabDoubleClickData != null){
	                        		var oldData = tpTabDoubleClickData.data;
	                        		oldGroupName = tpTabDoubleClickData.oldName;

						formatOldGroupName=oldGroupName;
						formatOldGroupName = formatOldGroupName.replace(/"/g,"&quot;");
						oldData=oldData.replace(/\\/g, '');
						formatUpdatedGroupName=updatedGroupName;
						formatUpdatedGroupName = formatUpdatedGroupName.replace(/"/g,"\\&quot;");
						oldData = oldData.replace( new RegExp(formatOldGroupName, "gi"), formatUpdatedGroupName);

	                        		var clk = $(oldData + ' a').attr('onclick');
	                        		var dblclk = $(oldData + ' a').attr('ondblclick');
		                        	$(tpTabDoubleClickData.selector).html(oldData);
						//var acText = $(tpTabDoubleClickData.selector +" span:first").text();
						$(tpTabDoubleClickData.selector +" span:first").text(updatedGroupName);
		                        	$("#attachedcourse-name-div-"+inlineData.courseId+'-'+inlineData.recertifyFlag).attr('title',updatedGroupName);
		                        	$( "#page-container-tabs-prg" ).tabs('destroy');
		            				$( "#page-container-tabs-prg" ).tabs({});
		                        	tpTabDoubleClickData = null;
	                        	}
	                        	obj.destroyLoader('attach_course_dt');
                            }
                        });
                    }
                },
                moreSurAssSearchHideShow : function () {
                    $('#select-list-surass-dropdown-list').slideToggle();
                    $('#select-list-surass-dropdown-list li:last').css('border-bottom','0px none');
                	},
                	/* Ticket: 46992 */
                	moreSurAssSearchHideShowTag : function () {
                        $('#select-list-surass-dropdown-list-tag').slideToggle();
                        $('#select-list-surass-dropdown-list-tag li:last').css('border-bottom','0px none');
                    	},
                    	/* Ticket: 46992 */
               moreSurAssSearchTypeText : function(dCode,dText) {
                    $('#select-list-surass-dropdown-list').hide();
                    $('#select-list-surass-dropdown').text(dCode);
                    $('#search_all_surass_type-hidden').val(dText);
                    var displayText;
                    if(dText=='surassqus'){
                 	   displayText = Drupal.t('LBL324');
                    }else{
                 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                    }
                    //var displayText = Drupal.t('LBL036') + ' ' + dCode;
            		$('#surassattchedquestions-autocomplete').val(displayText);
            		$('#surassattchedquestions-autocomplete_hidden').val(displayText);
            		$('#surassattchedquestions-autocomplete').addClass('input-field-grey');

             },
             /* Ticket: 46992 */
             moreSurAssSearchTypeTextTag : function(dCode,dText) {

                 $('#select-list-surass-dropdown-list-tag').hide();
                 $('#select-list-surass-dropdown-tag').text(dCode);
                 $('.admin_add_multi_search_container #tagsearch_all_surass_type-hidden').val(dText);

                 var displayText;
                 if(dText=='surassqus'){
              	   displayText = Drupal.t('LBL324');
                 }else{
              	   displayText =Drupal.t('LBL193');
                 }
                 //var displayText = Drupal.t('LBL036') + ' ' + dCode;

         		$('#surassattachquestion-autocomplete-tag').val(displayText);
         		$('#surassattachquestion-autocomplete-tag_hidden').val(displayText);
         		$('#surassattchedquestions-autocomplete-tag').addClass('input-field-grey');

          },
          /* Ticket: 46992 */
               moreCourseSearchTypeText : function(dCode,dText) {
                    //console.log('moreCourseSearchTypeText() called');
                    $('#select-list-course-dropdown').text(dCode);
                    $('#search_all_course_type-hidden').val(dText);
                    var displayText;
                    if(dText=='crstit'){
                 	   displayText = Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083');
                    }else{
                 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                    }
                    //var displayText = Drupal.t('LBL036') + ' ' + dCode;
            		$('#tpattchedcoursename-autocomplete').val(displayText);
            		$('#tpattchedcoursename-autocomplete_hidden').val(displayText);
            		$('#tpattchedcoursename-autocomplete').addClass('input-field-grey');

             },
             	moreAdduserSearchTypeText : function(dCode,dText) {
                 //console.log('moreCourseSearchTypeText() called');
            	 $('#search-list-class-title-keyword #select-list-class-dropdown-list').hide();
                 $('#select-list-class-dropdown').text(dCode);
                 $('#search_all_user_type-hidden').val(dText);
                 var displayText;
                 if(dText=='usrtit'){
              	   displayText = Drupal.t('LBL181');
                 }else if(dText=='fultit'){
              	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('LBL691');
                 }
                 else{
              	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                 }
                 //var displayText = Drupal.t('LBL036') + ' ' + dCode;
         		$('#username-search-autocomplete').val(displayText);
         		$('#username-search-autocomplete_hidden').val(displayText);
         		$('#username-search-autocomplete').addClass('input-field-grey');

          	},
          	 moreUserSearchTypeText : function(dCode,dText) {
          		$('#search-list-class-title-keyword #select-list-class-dropdown-list').hide();
                $('#select-list-class-dropdown-adduser').text(dCode);
                $('#search_all_user_type-hidden').val(dText);
                var displayText;
                if(dText=='usrtitle'){
             	   displayText = Drupal.t('LBL181');
                }else if(dText=='fultitle'){
             	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('LBL691');
                }else if(dText=='grptitle'){
             	   displayText =Drupal.t('LBL1270');
                }
                else{
             	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                }
                //var displayText = Drupal.t('LBL036') + ' ' + dCode;
        		$('#addusername-search-autocomplete').val(displayText);
        		$('#addusername-search-autocomplete_hidden').val(displayText);
        		$('#addusername-search-autocomplete').addClass('input-field-grey');

          },
                getInlineEditAttachedQuestion : function(inlineData){
                    if(inlineData){
                        var inlineEditData= eval(inlineData);
                        this.inlineEditData = inlineEditData;
                    }
                    var html ="";
                    var callBlur = '$("#root-admin").data("narrowsearch")';
                    html += '<input size="16" type ="text" class="attachedquestion-edit-txtbox" id="attachedquestion-list-name-id-'+this.inlineEditData.questionId+'" value ="'+this.inlineEditData.fullGroupName+'" onkeydown="return $(\'#root-admin\').data(\'narrowsearch\').updateAttachedQuestionGroupKeyDown(event, this);" onblur="return $(\'#root-admin\').data(\'narrowsearch\').updateAttachedQuestionGroup(this);">';
                    $("#attachedquestion-name-div-"+this.inlineEditData.questionId).html(html);
                    $('#attachedquestion-list-name-id-'+this.inlineEditData.questionId).focus();

                },

                updateAttachedQuestionGroupKeyDown : function(evt, updatedGroup){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedQuestionGroup(updatedGroup);
            	        evt.preventDefault();
            	        return false;
            	    }

                },
                searchClassNameFilter : function(courseId,classId){
                	var obj = this;
                	var className    = encodeURIComponent($('#classname-autocomplete').val());
                	var searchType   = $('#search_all_classs_type-hidden').val();
					var pagerId	     = '#admin-course-class-list-'+courseId+'-pagination_toppager';
					var objStr       = '$("#root_admin").data("narrowsearch")';

					obj.createLoader("paint-class-search-results-datagrid");
					$("#admin-course-class-list-"+courseId+"-pagination").setGridParam({url: this.constructUrl("administration/class-pagination/"+courseId+"/"+classId+"/0&class_name="+className+"&search_Type="+searchType)});
					$("#admin-course-class-list-"+courseId+"-pagination").trigger("reloadGrid",[{page:1}]);

					//$('#paintContentResults').setGridParam({url: this.constructUrl('learning/catalog-search/search/all/'+searchStr)});
				    //$("#paintContentResults").trigger("reloadGrid",[{page:1}]);
               },
               moreClassSearchHideShow : function () {
                   $('#select-list-class-dropdown-list').slideToggle();
                   $('#select-list-class-dropdown-list li:last').css('border-bottom','0px none');
               	},

              moreClassSearchTypeText : function(dCode,dText) {
                   $('#select-list-class-dropdown-list').hide();
                   $('#select-list-class-dropdown').text(dCode);
                   $('#search_all_classs_type-hidden').val(dText);
                   $('#search_all_user_type-hidden').val(dText);
                   var displayText;
                   if(dText=='clstit'){
                	   displayText = Drupal.t('LBL766');
                   }else if (dText=='clsstatus') {
                	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('Class')+ ' ' + dCode;
                   }else if(dText=='usrtit' || dText=='fultit' ){
		                $('#username-search-autocomplete').val(displayText);
		                $('#username-search-autocomplete_hidden').val(displayText);
		              	$('#username-search-autocomplete').addClass('input-field-grey');
		              	$('#username-search-autocomplete').focus();
                   }
                   else{
                	   displayText =Drupal.t('LBL036') + ' ' + dCode;
                   }
                   //var displayText = Drupal.t('LBL036') + ' ' + dCode;

           		$('#classname-autocomplete').val(displayText);
           		$('#classname-autocomplete_hidden').val(displayText);
           		$('#classname-autocomplete').addClass('input-field-grey');

            },

            templateDisplayPagination : function(notificationId,templateId,argType){
				var obj = this;
				var pagerId	= '#admin-notification-template-list-'+notificationId+'-pagination_toppager';
				var objStr = '$("#root_admin").data("narrowsearch")';
				if(argType == 'notification_template'){
				  urlType = "administration/manage/notification_template/temp-pagination/"+notificationId+"/"+templateId;
				}else{
				  urlType = "administration/manage/certificate/temp-pagination/"+notificationId+"/"+templateId;
				}
				//CREATE LOADER ICON
				obj.createLoader("paint-template-search-results-datagrid");
				$("#admin-notification-template-list-"+notificationId+"-pagination").jqGrid({
					url: this.constructUrl(urlType),
					datatype: "json",
					mtype: 'GET',
					//colNames:['Detail'],
					colModel:[ {name:'Detail',index:'detail', title: false, width:620, 'widgetObj':objStr, sortable: false, formatter: obj.displayClassListValues  }],
					rowNum:5,
					rowList:[5,10,15],
					pager: pagerId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					toppager: false,
					height: "auto",
					width: 620,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					loadui:false,
					loadComplete:obj.callbackTemplateListDataGrid
					}).navGrid('#pager-datagrid-'+notificationId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$("#admin-notification-template-list-"+notificationId+'-pagination_toppager').hide();
				//}
				},
			callbackTemplateListDataGrid : function(response, postdata, formid) {
				var listRows 	= 	5;
				//var curObj	 	=	this;
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");
				$('#admin-notification-template-list-'+response.notificationId+'-pagination').show();
				//$('#gview_admin-notification-template-list-'+response.notificationId+'-pagination').css();
				var element = document.getElementById('modal-content');
				if(element.clientHeight == element.scrollHeight){
					$("#gbox_admin-notification-template-list-"+response.notificationId+"-pagination").css('width','620px');
					$("#gbox_admin-notification-template-list-"+response.notificationId+"-pagination").css('overflow','hidden');
				}
				// Hide the pagination, if the record count in the view learning table is equal to or less than
				var recordCount = $('#admin-notification-template-list-'+response.notificationId+"-pagination").jqGrid('getGridParam', 'records');

		        if (recordCount == 0) {
		        	$("#admin-notification-template-list-"+response.notificationId+"-pagination"). css('display','block');
		            var html = Drupal.t('MSG381')+'.';
		            $("#admin-notification-template-list-"+response.notificationId+"-pagination").html('<tr><td class="border-style-none" width="620px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
		            $('#admin-notification-template-list-'+response.notificationId+"-pagination").css('text-align','center');
		            $('.border-style-none').css('border','0');
		        } else if(recordCount > listRows){
			        $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager").show();
			        if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			         $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager_center").css('width','229px');
			        }else{
			         $("#admin-notification-template-list-"+response.notificationId+"-pagination_toppager_center").css('width','250px');
			        }

			        $('#add-edit-class-norecords').css('display','none');
				} else {
					$('#add-edit-class-norecords').css('display','none');
				}

				curObjStr.initGridTemplatePagination(response.notificationId);
				curObjStr.destroyLoader('paint-template-search-results-datagrid');

				$('#admin-notification-template-list-pagination-'+response.notificationId+' tr').click(function(event){
					event.stopPropagation();
				});

				$('.edit-class-list-container').last().css('border','0px none');
				var backdrop_height = $(document).height();
				$('#modalBackdrop').css('height',backdrop_height+'px');
				//When Template is added,the save and publish button is enabled
				$('.pub-unpub-only-save-btn').attr('disabled',false);
				$('.save-pub-unpub-sub-menu input').removeClass('save-and-enable-disabled');
				$('#admin-notification-template-list-'+response.notificationId+'-pagination tr td').find('div.edit-class-list').last().css('border-bottom','0px none');

				//Vtip-Display toolt tip in mouse over
				 vtip();
			},
			initGridTemplatePagination : function(notificationId) {
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");

				//NEXT PREVIOUS LOADER SETTING
				$('#first_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#first_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#prev_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#prev_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#next_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#next_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});
				$('#last_admin-notification-template-list-'+notificationId+'-pagination_toppager').click(function(e) {
									if (!$('#last_admin-notification-template-list-'+notificationId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});

				$(".ui-pg-input").keyup(function(event) {
									if (event.keyCode == 13) {
										$('#admin-notification-template-list-'+notificationId+'-pagination').hide();
										$('#gview_admin-notification-template-list-'+notificationId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-template-search-results-datagrid');
									}
								});

			},
		    userDisplayPagination : function(roleId,userId){
				var obj = this;
				var pagerId	= '#admin-course-class-list-'+roleId+'-pagination_toppager';
				var objStr = '$("#root_admin").data("narrowsearch")';

				//CREATE LOADER ICON
				obj.createLoader("paint-user-search-results-datagrid");

				$("#admin-course-class-list-"+roleId+"-pagination").jqGrid({
					url: this.constructUrl("administration/people/group/user-pagination/"+roleId+"/"+userId),
					datatype: "json",
					mtype: 'GET',
					colNames:[  Drupal.t('LBL054'),Drupal.t('LBL691'),Drupal.t('LBL102'), multiselectCheckbox],
					colModel:[ { name:'User Name',index:'UserName', title: false, width:120, 'widgetObj':objStr, formatter: obj.userDisplayGridValues },
					           { name:'Full Name',index:'FullName', title: false, width:200, 'widgetObj':objStr, formatter: obj.userDisplayGridValues },
					           { name:'Status',index:'Status', title: false, width:80, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, hidden: hideStatusOption },
					           { name:'MultiselectCheck',index:'MultiselectCheck', title: false, width:19, 'widgetObj':objStr, formatter: obj.userDisplayGridValues, sortable: false}
					         ],
					rowNum:5,
					rowList:[5,10,15],
					pager: pagerId,
					viewrecords:  true,
					multiselect: false,
					emptyrecords: "",
					toppager: false,
					height: "auto",
					width: 570,
					loadtext: "",
					recordtext: "",
					pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
					loadui:false,
					loadComplete:obj.callbackUserListDataGrid
					}).navGrid('#pager-datagrid-'+roleId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
				$("#admin-course-class-list-"+roleId+'-pagination_toppager').hide();
				setQtipPosition();
			},
			callbackUserListDataGrid : function(response, postdata, formid) {
				var listRows 	= 	5;
				//var curObj	 	=	this;
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");
				$('#admin-course-class-list-'+response.roleId+'-pagination').show();
				var element = document.getElementById('modal-content');
				if(element.clientHeight == element.scrollHeight){
					$("#gbox_admin-course-class-list-"+response.roleId+"-pagination").css('width','570px');
					$("#gbox_admin-course-class-list-"+response.roleId+"-pagination").css('overflow','hidden');
				}
				// Hide the pagination, if the record count in the view learning table is equal to or less than
				var recordCount = $('#admin-course-class-list-'+response.roleId+"-pagination").jqGrid('getGridParam', 'records');

		        if (recordCount == 0) {
		        	$("#admin-course-class-list-"+response.roleId+"-pagination"). css('display','block');
		            var html = Drupal.t('MSG381')+'.';
		            $("#admin-course-class-list-"+response.roleId+"-pagination").html('<tr><td class="border-style-none" width="570px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
		            $('#admin-course-class-list-'+response.roleId+"-pagination").css('text-align','center');
		            $('.border-style-none').css('border','0');
		        } else if(recordCount > listRows){
			        $("#admin-course-class-list-"+response.roleId+"-pagination_toppager").show();
			        $('#add-edit-class-norecords').css('display','none');
				} else {
					$('#add-edit-class-norecords').css('display','none');
				}

				curObjStr.initGridUserPagination(response.roleId);
				curObjStr.destroyLoader('paint-user-search-results-datagrid');

				$('#admin-course-class-list-pagination-'+response.roleId+' tr').click(function(event){
					event.stopPropagation();
				});

				$('.edit-class-list-container').last().css('border','0px none');
				var backdrop_height = $(document).height();
				$('#modalBackdrop').css('height',backdrop_height+'px');
				//Vtip-Display toolt tip in mouse over
				 vtip();
			},
			initGridUserPagination : function(roleId) {
				var curObjStr 	= 	$("#root-admin").data("narrowsearch");

				//NEXT PREVIOUS LOADER SETTING
				$('#prev_admin-course-class-list-'+roleId+'-pagination_toppager').click(function(e) {
									if (!$('#prev_admin-course-class-list-'+roleId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});
				$('#next_admin-course-class-list-'+roleId+'-pagination_toppager').click(function(e) {
									if (!$('#next_admin-course-class-list-'+roleId+'-pagination_toppager').hasClass('ui-state-disabled')) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});

				$(".ui-pg-input").keyup(function(event) {
									if (event.keyCode == 13) {
										$('#admin-course-class-list-'+roleId+'-pagination').hide();
										$('#gview_admin-course-class-list-'+roleId+'-pagination').css('min-height','50px');
										curObjStr.createLoader('paint-class-search-results-datagrid');
									}
								});

			},



				classDisplayPagination : function(courseId,classId,oldClassId){
					var obj = this;
					var pagerId	= '#admin-course-class-list-'+courseId+'-pagination_toppager';
					var objStr = '$("#root_admin").data("narrowsearch")';

					//CREATE LOADER ICON
					obj.createLoader("paint-class-search-results-datagrid");
					if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						var Wgrid = '835';
					} else {
						var Wgrid = '827';
					}

					$("#admin-course-class-list-"+courseId+"-pagination").jqGrid({
						url: this.constructUrl("administration/class-pagination/"+courseId+"/"+classId+"/"+oldClassId),
						datatype: "json",
						mtype: 'GET',
						colNames:['Detail'],
						colModel:[ {name:'Detail',index:'detail', title: false, width:860, 'widgetObj':objStr, sortable: false, formatter: obj.displayClassListValues  }],
						rowNum:5,
						rowList:[5,10,15],
						pager: pagerId,
						viewrecords:  true,
						multiselect: false,
						emptyrecords: "",
						toppager: false,
						height: "auto",
						width: Wgrid,
						loadtext: "",
						recordtext: "",
						pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
						loadui:false,
						loadComplete:obj.callbackClassListDataGrid
						}).navGrid('#pager-datagrid-'+courseId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
					$("#admin-course-class-list-"+courseId+'-pagination_toppager').hide();
					setQtipPosition();
				},
				callbackClassListDataGrid : function(response, postdata, formid) {
				Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
					if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						var Wgrid = '835';
					} else {
						var Wgrid = '827';
					}
					var listRows 	= 	5;
					//var curObj	 	=	this;
					var curObjStr 	= 	$("#root-admin").data("narrowsearch");
					$('#admin-course-class-list-'+response.courseId+'-pagination').show();
					var element = document.getElementById('modal-content');
					if(element.clientHeight == element.scrollHeight){
						$("#gbox_admin-course-class-list-"+response.courseId+"-pagination").css('width',Wgrid+'px');
						$("#gbox_admin-course-class-list-"+response.courseId+"-pagination").css('overflow','hidden');
					}
					// Hide the pagination, if the record count in the view learning table is equal to or less than
					var recordCount = $('#admin-course-class-list-'+response.courseId+"-pagination").jqGrid('getGridParam', 'records');

			        if (recordCount == 0) {
			        	$("#admin-course-class-list-"+response.courseId+"-pagination"). css('display','block');
			            var html = Drupal.t('MSG381')+'.';
			            $("#admin-course-class-list-"+response.courseId+"-pagination").html('<tr><td class="border-style-none" width="'+Wgrid+'px"><div id="add-edit-class-norecords" class="no-records-msg">'+html+'</div></td></tr>');
			            $('#admin-course-class-list-'+response.courseId+"-pagination").css('text-align','center');
			            $('.border-style-none').css('border','0');
			            $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").hide();
			        } else if(recordCount > listRows){
				        $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").show();
				        if ( $.browser.msie && parseInt($.browser.version, 10)=='9'){
				        $("#admin-course-class-list-"+response.courseId+"-pagination_toppager").find('.ui-pg-table').css('width', 'auto');
				        }
				        $('#add-edit-class-norecords').css('display','none');
					} else {
						$('#add-edit-class-norecords').css('display','none');
						$("#admin-course-class-list-"+response.courseId+"-pagination_toppager").hide();
					}

					curObjStr.initGridClassPagination(response.courseId);
					curObjStr.destroyLoader('paint-class-search-results-datagrid');

					$('#admin-course-class-list-pagination-'+response.courseId+' tr').click(function(event){
						event.stopPropagation();
					});

					$('.edit-class-list-container').last().css('border','0px none');
					var backdrop_height = $(document).height();
					$('#modalBackdrop').css('height',backdrop_height+'px');
					//Vtip-Display toolt tip in mouse over
					 vtip();
					 $("#admin-course-class-list-"+response.courseId+"-pagination > tbody > tr:last > td > .edit-class-list"). css('border','0px none');
				},


				initGridClassPagination : function(courseId) {

					var curObjStr 	= 	$("#root-admin").data("narrowsearch");

					//NEXT PREVIOUS LOADER SETTING
					$('#first_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#first_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#prev_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#prev_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#next_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#next_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});
					$('#last_admin-course-class-list-'+courseId+'-pagination_toppager').click(function(e) {
										if (!$('#last_admin-course-class-list-'+courseId+'-pagination_toppager').hasClass('ui-state-disabled')) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});

					$(".ui-pg-input").keyup(function(event) {
										if (event.keyCode == 13) {
											$('#admin-course-class-list-'+courseId+'-pagination').hide();
											$('#gview_admin-course-class-list-'+courseId+'-pagination').css('min-height','50px');
											curObjStr.createLoader('paint-class-search-results-datagrid');
										}
									});

				},
				displayClassListValues : function(cellvalue, options, rowObject) {
					return rowObject['detail'];
				},

				userDisplayGridValues : function(cellvalue, options, rowObject){
				    var type = $('body').data('mulitselectdatagrid').type;
					var mode = $('body').data('mulitselectdatagrid').mode;
					var uniqueId = $('body').data('mulitselectdatagrid').uniqueId;
					var entityId = $('body').data('mulitselectdatagrid').entityId;
					var entityType = $('body').data('mulitselectdatagrid').entityType;
					var groupname  = '';
					var index  = options.colModel.index;
					var rowEditOptionId =  rowObject['id'] + '-' + type + '-' + entityId + '-' + entityType + '-' + index;
					if(index == 'MultiselectCheck'){
						var inputType = 'checkbox';
						var id = rowObject['MultiselectCheck'];
						return '<input type="'+inputType+'" id="multiselect-singlecheck-'+uniqueId+'_'+id+'" value="'+id+'" class="multiselect-singlecheck" />';
					}else if(index == 'UserName'){
							var titleAllowed = type == 'RoleAddusers' ?  13 : 20;
							var userNameRestricted = $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed);
							return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+userNameRestricted+'</span>';

				   }else if(index == 'FullName'){

						var titleAllowed = type == 'RoleAddusers' ?  15 : 35;
						var fullNameRestricted = $('body').data('mulitselectdatagrid').titleRestricted(rowObject[index], titleAllowed);
						return '<span class="vtip" title="'+htmlEntities(rowObject[index])+'">'+fullNameRestricted+'</span>';

					}


				},

                updateAttachedQuestionGroup : function(updatedGroup){

                    var inlineData = this.inlineEditData;
                    var updatedGroupName = escape($(updatedGroup).attr("value"));
                    var oldGroupName	= this.inlineEditData.groupName;
                    this.inlineEditData.groupName = updatedGroupName;
                    var obj = this;
                    /*-- #36135: Isuse in Survey/Assessment group name tool --*/
                    var regGroupName = /^[A-Za-z0-9.,@_\s]{0,100}$/;  // /[^a-zA-Z0-9,.@\'\_ |\/]/
                    var grpString = decodeURIComponent(updatedGroupName);
                    // 36135 : validatin code added
                    if(!regGroupName.test(grpString)) {
                    	var error_msg = '<div><div id="message-container" style="visibility: visible;"><div class="messages error"><ul><li style="width: 200px;"><span>' +Drupal.t('MSG644') + ' ' + Drupal.t('MSG639') + '</span></li></ul><div onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();" class="msg-close-btn"></div></div></div></div>';
                    	$("#show_expertus_message").append(error_msg);
                    	return false;
                    }
                    if(updatedGroupName == "No Group Name" || updatedGroupName == "") {
                    	updatedGroupName = oldGroupName;
                    	this.inlineEditData.groupName = updatedGroupName;
                    	var html = "";
                        html += '<a href="javascript:void(0);" id="attachedquestion-list-name-id-'+inlineData.questionId+'" name="attachedquestion-list-name-id-'+inlineData.questionId+'" class = "attachedcourse-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedQuestion();">'+updatedGroupName+'</a>';
                        $("#attachedquestion-name-div-"+inlineData.questionId).html(html);
                    }else{
                    	obj.createLoader('survey_attach_questions_dt');
                        url = obj.constructUrl("administration/update-attachedquestion-group/"+inlineData.entityId+"/"+updatedGroupName+"/"+inlineData.questionId);
                        $.ajax({
                            type: "POST",
                            url: url,
                            data:  '',
                            success: function(result){
                        	    obj.destroyLoader('survey_attach_questions_dt');
                        	    var result = result.split('|');
                        	    var shortGroupName = result[0];
                        	    var longGroupName = result[1];
                                var html = "";
                                var dataQuesGroup = 'data={"entityId":"'+inlineData.entityId+'","entityType":"'+inlineData.entityType+'","groupName":"'+$.trim(shortGroupName)+'","fullGroupName":"'+$.trim(longGroupName)+'","questionId":"'+inlineData.questionId+'"}';
                                dataQuesGroup = unescape(dataQuesGroup).replace(/"/g, '&quot;');
                                html += '<a href="javascript:void(0);" id="attachedquestion-list-name-id-'+inlineData.questionId+'" name="attachedquestion-list-name-id-'+inlineData.questionId+'" class = "attachedquestion-list-name"  ondblclick ="$(\'#root-admin\').data(\'narrowsearch\').getInlineEditAttachedQuestion(\''+dataQuesGroup+'\');">'+titleRestrictionFadeoutImage(shortGroupName,'survey-grp-name')+'</a>';
                                $("#attachedquestion-name-div-"+inlineData.questionId).html(html);
                                $("#attachedquestion-name-div-"+inlineData.questionId).attr('title', longGroupName);
                            }
                        });
                    }

                },

                /*

                updateAttachedQuestionScore : function(inlineData){
                	if(inlineData){
                          var inlineEditData= eval(inlineData);
                          this.inlineEditData = inlineEditData;
                    }
                	var scoreValue = $('#edit-edit-attachquestion-list-score-'+this.inlineEditData.loopId).val();
	                 if(scoreValue != ''){
	                    var obj = this;
	                    url = obj.constructUrl("administration/update-attachedquestion-score/"+this.inlineEditData.entityId+"/"+this.inlineEditData.entityType+"/"+this.inlineEditData.questionId+"/"+scoreValue);
	                    $.ajax({
	                        type: "POST",
	                        url: url,
	                        data:  '',
	                        success: function(result){

	                    	}
	                    });
	                 }

                },

                updateAttachedQuestionScoreKeyDown : function(evt, inlineData){
            		evt = evt || window.event;
            	    var charCode = evt.keyCode || evt.which;
            	    if (charCode == 13) {
            	    	$("#root-admin").data("narrowsearch").updateAttachedQuestionScore(inlineData);
            	        evt.preventDefault();
            	        return false;
            	    }

                }*/

		updatePreviewContent :function(){
        	/*
				function to redirect callback of content launch
				--> searches for the callback function on close of the content and removes the corresponding record from
				slt_aicc_interaction -- only for aicc contents

			*/

            var url='';
            var post_data='';
            if(scoobj._type == "AICC" || scoobj._type == "AICC Course Structure") {
            	var sid = $('#root-admin').data('narrowsearch').launchAICCId;
            	url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
            	post_data={"session_id":sid,"command":"UPDATEAICCPREVIEW"};

        		var obj = this;
        		$.ajax({
        			type: "POST",
        			url: url,
        			data:  post_data,
        			success: function(result) {

        			}
        	    });
            }
        },

        showExpAdminDropdownMenu : function (dropdownListId) {
          var dropdownListSelector = '#' + dropdownListId;
            if ($(dropdownListSelector).is(":visible")) {
              //console.log('showExpAdminDropdownMenu() : is already displayed ' + dropdownListId);
              // hideExpAdminDropDownMenu() will hide the dropdown menu when it is already displayed.
            }
            else {
              //console.log('showExpAdminDropdownMenu() : showing ' + dropdownListId);
              $(dropdownListSelector).slideDown(); // show menu
              $(dropdownListSelector + ' li:last').css('border-bottom','0px none');

              $('html').bind('mousedown.' + dropdownListId, {dropdownListId : dropdownListId},
                                           $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
            }
         },

         hideOnClickExpAdminDropDownMenu : function (e) {
           var dropdownListId = e.data.dropdownListId;
           var dropdownListSelector = '#' + dropdownListId;
           var el = e.target;
           var dropdown = $('#' + dropdownListId + ':visible')[0];
           if(typeof dropdown == 'undefined') {
             //console.log('hideOnClickExpAdminDropDownMenu() : no dropdown menu open. Returning true');
             $('html').unbind('mousedown.' + dropdownListId, $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
             return true;
           }

           // Hide the dropdown menu
           //console.log('hideOnClickExpAdminDropDownMenu() : hiding ' + dropdownListId);
           $(dropdownListSelector).slideUp(); // hide menu
           $('html').unbind('mousedown.' + dropdownListId, $("#root-admin").data("narrowsearch").hideOnClickExpAdminDropDownMenu);
           return false; // no need to propagate the event further
        },

        meetingUrlCreate : function (meetingName,classId) {
        	if(meetingName == 'lrn_cls_vct_oth'){ // Others
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#attendee_url').css('display','block');
        		$('#presenter_url').css('display','block');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class-others');

	        }else if(meetingName == 'lrn_cls_vct_exp'){ // Expertus Meeting
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#session_presenter_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_attende_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_meeting_id').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','none');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
/*        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','block'); */

        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');

	        }else{
	        	$('#session_presenter_password').attr('disabled',false).removeClass('admin-grey-out-fields');
    			$('#session_attende_password').attr('disabled',false).removeClass('admin-grey-out-fields');
    			$('#session_meeting_id').attr('disabled',false).removeClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','inline-block');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }
        },

		getMeetingType : function(){
        	var meetingtypeid = $('#set-meeting-type').val();
            var class_id = $('#class_id').val();
            if(meetingtypeid == 'lrn_cls_vct_oth'){
            	$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
            	$('#attendee_url').css('display','block');
        		$('#presenter_url').css('display','block');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class-others');
            }else if(meetingtypeid == 'lrn_cls_vct_exp'){ // Expertus Meeting
        		$('#session_presenter_password').val('');
        		$('#session_attende_password').val('');
        		$('#session_meeting_id').val('');
        		$('#session_presenter_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_attende_password').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('#session_meeting_id').attr('disabled',true).addClass('admin-grey-out-fields');
        		$('.session-pass-mandatory').css('display','none');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','none');
        		$('#presenter_password').css('display','none');
        		$('#meeting_id_dt').css('display','none');
        		$('.admin-addedit-session-details-meeting tr:visible').addClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }else{
	        	var getDisAttr = $('#session_meeting_id');
	        	if(!getDisAttr.attr('disabled')){
	        		$('#session_presenter_password').attr('disabled',false);
	        		$('#session_attende_password').attr('disabled',false);
	        		$('#session_meeting_id').attr('disabled',false);
	        	}
    			$('.session-pass-mandatory').css('display','inline-block');
    			$('#session_presenter_password').removeClass('admin-grey-out-fields');
        		$('#session_attende_password').removeClass('admin-grey-out-fields');
        		$('#session_meeting_id').removeClass('admin-grey-out-fields');
        		$('#attendee_url').css('display','none');
        		$('#presenter_url').css('display','none');
        		$('#attendee_password').css('display','block');
        		$('#presenter_password').css('display','block');
        		$('#meeting_id_dt').css('display','');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class');
        		$('.admin-addedit-session-details-meeting tr:visible').removeClass('session-meeting-vc-class-others');
	        }

        },


         /*
    	function to display popup if content has multiple lessons
    	--> rendered on clicking "preview" for each version
         */
        getPreviewContent : function(contentId,VersionId,lessonlist){

    		var closeButt={};
    	    $('#lesson-wizard').remove();
    	  	html="";
    		html+='<div id="lesson-wizard" class="lesson-wizard-content" style="display:none; padding: 0px;">';
    	    html+= this.getContentVersionLessons(contentId,VersionId,lessonlist);
    	    html+='</div>';
    	    $("body").append(html);
    	    Drupal.attachBehaviors();


    	    $("#lesson-wizard").dialog({
    	        position:[(getWindowWidth()-500)/2,100],
    	        bgiframe: true,
    	        width: 450,
    	        zIndex: 10000,
    	        resizable:false,
    	        modal: true,
    	        title: Drupal.t('LBL889'),
    	        buttons:closeButt,
    	        closeOnEscape: false,
    	        draggable:false,
    	        overlay:
    	         {
    	           opacity: 0.9,
    	           background: "black"
    	         }
    	    });

    	    $("#lesson-wizard").show();

    		$('.ui-dialog-titlebar-close').click(function(){
    	        $("#lesson-wizard").remove();
    	    });
			if(this.currTheme == 'expertusoneV2'){
		    	changeDialogPopUI();
			}
    	    //SCrollbar for content dialogbox
    	    this.scrollBarContentDialog();

        },

        /*
    	function to display the list of lessons inside the dialog
    	--> rendered on clicking "preview" for each version
        */
        getContentVersionLessons : function(contentId,VersionId,data){


    	var lesCnt = 0;
    	var ostr = '';
    	ostr += '<td colspan="2"><div class="enroll-accordian-div">';
    	ostr += '<table class="launch-lesson-view" cellpadding="0" cellspacing="0" width="100%" border="0">';


    	$(data).each(function(){
    		$(this).each(function(){
    			  lesCnt++;
    			  var params = "";
    			  var lessontitle = $(this).attr('title');
    			  var launchurl =  $(this).attr('launchurl');
    			  var contentType = $(this).attr('contentype');
    			  var contentTypeCode = $(this).attr('contentypecode');
    			  var contenthostedtype = $(this).attr('contenthostedtype');
    			  var aicc_sid = $(this).attr('AICC_SID');
    			  var callLaunchUrlFn;
    			  var lessonId =  $(this).attr('lessonid');
    			  if(contentTypeCode != "lrn_cnt_typ_vod"){
    				    params = "{'Id':'"+lessonId+"','url1':'"+launchurl+
    			            "','ErrMsg':'"+"error"+"','contentType':'"+contentType+"','AICC_SID':'"+aicc_sid+"'}";
    				    callLaunchUrlFn = "onclick=\"$('#root-admin').data('narrowsearch').launchLesson("+params+")\"";
    			  }else{
    				    lessontitle = encodeURIComponent(lessontitle.replace(/\//g, '()|()'));
    				    callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
    				    callLaunchUrlFn += lessontitle + "/";
    				    callLaunchUrlFn += escape(contenthostedtype) + "/";
    				    callLaunchUrlFn += escape(launchurl.replace(/\//g, '()|()')) + '/';
    				    callLaunchUrlFn += "ME" + "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += lessonId + "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "/";
    				    callLaunchUrlFn += "\"";

    			  }

    		      var pipe = '<span class="enroll-pipeline">|</span>';

    			  ostr += '<tr class="enroll-lesson-section"><td class="enroll-launch-column1">';
    			  ostr += '<div class="enroll-course-title-lesson">';
    			  ostr += '<span id="titleAccEn_'+lessonId+'" class="item-title" ><span class="enroll-course-title vtip" title="'+Drupal.t('LBL854')+' '+lesCnt+' : '+unescape(lessontitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
    			  ostr += 'Lesson '+lesCnt+': ' +titleRestricted(decodeURIComponent(lessontitle),35);
    			  ostr += '</span></span>';
    			  ostr += '</div>';
    			  ostr += '<div class="enroll-class-title-info">';
    			  ostr += '<div class="item-title-code">';

    			  ostr += ' </div>';
    			  ostr += '</div>';
    			  ostr += '</td>';
    			  ostr += '<td class="enroll-launch-column2">';
    			  ostr +=   '<div class="enroll-main-list">';
    			  ostr +=     '<label id="' + lessonId + '_launch_button_lbl"' +
    			                  ' class="'+"enroll-launch-full" + ' launch_button_lbl">';

    			  if(contentTypeCode != "lrn_cnt_typ_vod"){
    				  ostr += '<input type="button" id="' + lessonId + '_launch_button"' +
    			                   ' class="' + 'actionLink' + ' enroll-launch"' +

    			                       ' value="' + Drupal.t('LBL199') + '" '+ callLaunchUrlFn  + '>';
    			  }else{
    				  ostr += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
                      "title='" + Drupal.t("LBL199") + "' " +
                        "id='launch" + lessonId + "' " +
                          callLaunchUrlFn +
                            " >" +
                              Drupal.t("LBL199") + "</a>";
    			  }
    			  ostr +=     '</label>';
    			  ostr +=   '</div>';
    			  ostr += '</td></tr>';

    		});
    	});
    	ostr += '</table></div"></td>';
    	return ostr;

        },

        /*
    	function to initialixe scorm wrapper
    	--> called on clicking "preview" for each version
         */
        launchLesson : function(data){
        	var pmType 		= data.contentType;
	        var contentVersion;
	        var contentType;
		    var stdid		= this.getLearnerId();
		    var stdname 	= this.getUserName();
		    $('#root-admin').data('narrowsearch').launchUserId = stdid;
		    $('#root-admin').data('narrowsearch').launchLessonId = data.Id;
		    $('#root-admin').data('narrowsearch').launchAICCId = data.AICC_SID;
		    if(stdid == "0" || stdid == "")
			{
			    self.location='?q=learning/enrollment-search';
		        return;
			}
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

	        var cdnstatus = data.cdnmodulestatus;
	        if(cdnstatus == 1){
	        	$.ajax({
	        		url: '?q=ajax/getcdnurl/'+data.Id,
					data:  '',
					success: function(result){
						var output = result.split('~~');
						var url1      = output[0];
	        var data1 = {
				    	        url				 : url1,
				    	        callback		 : $('#root-admin').data('narrowsearch').updatePreviewContent,
				    	        learnerId		 : stdid,
				    	        learnerName		 : stdname,
				    	        completionStatus : 'not attempted',
				    	        scoreMax		 : '0',
				    	        scoreMin		 : '0',
				    	        score			 : '0',
				    	        location		 : '',
				    	        courseid 		 : '0',
				    	        classid 		 : '0',
				    	        lessonid 		 : data.Id,
				    	        versionid		 : '0',
				    	        enrollid 		 : '0',
				    	        launch_data		 : '',
				    	        suspend_data     : '',
				    	        exit			 : '',
				    	        aicc_sid		 : data.AICC_SID
				    	    };

				        scoobj.Initialize(data1);
					}
				  });
				} else {

	        var data1 = {
	    	        url				 : decodeURIComponent(data.url1),
	    	        callback		 : $('#root-admin').data('narrowsearch').updatePreviewContent,
	    	        learnerId		 : stdid,
	    	        learnerName		 : stdname,
	    	        completionStatus : 'not attempted',
	    	        scoreMax		 : '0',
	    	        scoreMin		 : '0',
	    	        score			 : '0',
	    	        location		 : '',
	    	        courseid 		 : '0',
	    	        classid 		 : '0',
	    	        lessonid 		 : data.Id,
	    	        versionid		 : '0',
	    	        enrollid 		 : '0',
	    	        launch_data		 : '',
	    	        suspend_data     : '',
	    	        exit			 : '',
	    	        aicc_sid		 : data.AICC_SID
	    	    };

	        scoobj.Initialize(data1);
				}
        },

        scrollBarContentDialog : function(){
	    	var contentDialogHeight = $("#lesson-content-container").height();
	    	if(contentDialogHeight > 550) {
	    		$(".lesson-wizard-content").css('height',550);
	    	} else if(contentDialogHeight <= 550) {
	    		$(".lesson-wizard-content").css('height','auto').css('min-height','70px');
	    	}

        },

        underlineTagAndTriggerSearch : function(tagName, filterCode, currElem) {
      	  try {
      		$('.tagscloud-tag').css('text-decoration', '');
      		$(currElem).css('text-decoration', 'underline');
      		$('#' + filterCode + '-addltext-filter').val(tagName);
									    $('#' + filterCode + '-tagscloud-clr').css('display', 'block');
      	    $('#root-admin').data('narrowsearch').refreshGrid();
      	  }
      	  catch (e) {
      	    // To Do
      	  }
      	},

      underlineTagCloudAndTriggerSearch : function(tagName, filterCode, currElem,tagWeight) {
        	  try {
        		$('.tagscloud-tag').css('text-decoration', '');
        		$(currElem).css('text-decoration', 'underline');
        		$('#' + filterCode + '-addltext-filter').val(tagName);
									    $('#' + filterCode + '-tagscloud-clr').css('display', 'block');
        	    $('#root-admin').data('narrowsearch').refreshGrid();
        	    $tagText = $(currElem).html();
        	    if($('#tagentityType').val() == 'tagtip'){
        	    	 var tagpos = $.inArray($tagText,existtagArrAdmin,0);
             	    if(tagpos == -1){
         				existtagArrAdmin.unshift($tagText);
         				existtagArrAdmin.pop();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:last-child').remove();
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}else if(tagpos >= 0){
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
         				if (tagpos > -1) {
         					existtagArrAdmin.splice(tagpos, 1);
         				}
         				existtagArrAdmin.unshift($tagText);
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}
        	    }else{
        	    	var tagpos = $.inArray($tagText,existtagClsArrAdmin,0);
             	    if(tagpos == -1){
         				existtagClsArrAdmin.unshift($tagText);
         				existtagClsArrAdmin.pop();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:last-child').remove();
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}else if(tagpos >= 0){
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').find('span:nth-child('+(tagpos+1)+')').remove();
         				existtagClsArrAdmin.unshift($tagText);
         				if (tagpos > -1) {
         					existtagClsArrAdmin.splice(tagpos, 1);
         				}
         				$(currElem).html(descController('FADE OUT',$tagText,weightArrAdmin[tagWeight-1]));
         				var selectedTag = $(currElem).parent().html();
         				$('#' + filterCode + '_tagscloud #tagcloudcontainerid').prepend('<span class="tagscloud-term">'+selectedTag+'</span>');
         				$(currElem).html($tagText);
         			}
        	    }


        	  }
        	  catch (e) {
        	    // To Do
        	  }
        	},
        	showConfirmPopup: function(submitHandlerId, action, usersList,Type,isPricedClass) {
				try {
					var learners = $('input[name='+usersList+']').val().split(',');
					var getStatus = new Array();
					for (var i=0; i<learners.length; i++) {
						getStatus[i] = $('input[name=enrolled_status_'+learners[i]+']').val();
					}
					var unique = getStatus.filter(function(itm,i,getStatus){
						return i==getStatus.indexOf(itm);
					});
					var getdefaultStatus = new Array();
					for (var i=0; i<learners.length; i++) {
						getdefaultStatus[i] = $('input[name=enrolled_default_status_'+learners[i]+']').val();
					}
					var defltstat = getdefaultStatus.filter(function(itm,i,getdefaultStatus){
						return i==getdefaultStatus.indexOf(itm);
					});				
					 if(defltstat ==  Drupal.t('Completed') && submitHandlerId == 'commonsave_save-button'){
                     $('#'+submitHandlerId).click();
                     return;
                     }   
					var enrollmentStatus;	
					var getStsLength = unique.length;
                     if((isPricedClass == 1 ) && unique == Drupal.t('Canceled')){
                     	$('#show_expertus_message').html(expertus_error_message([Drupal.t('LBL1246')]));
                     	return;
                     }
                  if(getStsLength == 1 && (unique == Drupal.t('Waived')  || (unique == Drupal.t('Enrolled')  && submitHandlerId == 'commonsave_save-button') || (unique == Drupal.t('In progress')  && submitHandlerId == 'commonsave_save-button'))){
                     $('#'+submitHandlerId).click();
                     return;
                     }
					if($('input[name='+usersList+']').val() == "") {
						$('#show_expertus_message').html(expertus_error_message([Drupal.t('ERR106')]));
						return;
					} else if(learners.length == 1) {
						learners = $('#enroll_user_'+$('input[name='+usersList+']').val()).val();
					} else {
						learners = learners.length +" "+ Drupal.t('LBL3080');
					}			
					if(Type=='Class'){
					 var cancelTitleLT = Drupal.t('Class');
					}else{
					 var cancelTitleLT = Drupal.t('Training Plan');
					}
					switch(action) {
						case 'completedandsave':
							enrollmentStatus = Drupal.t('Completed');
						break;
						case 'incompletedandsave':
							enrollmentStatus = Drupal.t('Incomplete');
						break;
						case 'noshowandsave':
							enrollmentStatus = Drupal.t('No Show');
						break;
						case 'cancelledandsave':
							enrollmentStatus = Drupal.t('Canceled');
						break;
						default: enrollmentStatus = "Common";
						break;
					}
					//show jquery dialog with yes or cancel buttons with its action callbacks
					this.currTheme = Drupal.settings.ajaxPageState.theme;
					$('#complete-confirmation-wizard').remove();
					var html = '';
				    html += '<div id="complete-confirmation-wizard" style="display:none; padding: 13px;">';
				    html += '<table width="99%" class="complete-confirmation-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
				    html += '<tr><td height="30"></td></tr>';
				    html += '<tr>';				     
					if(action == "commonsave"){
						if(getStsLength == 1 && learners.length >= 1){
							if(unique == "Completed"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Completed'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "Incomplete"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Incomplete'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "Canceled"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('Canceled'), '@objtype': cancelTitleLT}) +'</span></td>';
						}else if(unique == "No Show"){
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status':Drupal.t('No Show'), '@objtype': cancelTitleLT}) +'</span></td>';
						}
						}else{
							html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG821', {'@learner_name': learners, '@objtype': cancelTitleLT}) +'</span></td>';
						}	  
					}else{						
						html += '<td align="center" id="complete-confirmation-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status': enrollmentStatus, '@objtype': cancelTitleLT}) +'</span></td>';
					}
					html += '</tr>';
		    	   html +='</table>';
		    	   html +='</div>';
		    	   $("body").append(html);
					var confButton = {};
					confButton[Drupal.t('LBL109')] = function() {
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('#complete-confirmation-wizard').remove();
					};
					confButton[Drupal.t('Yes')] = function(){
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('#complete-confirmation-wizard').remove();
						$('#'+submitHandlerId).click();
					}
					var messageLine;
					if(enrollmentStatus == "Common") {
						if(getStsLength == 1 && learners.length >= 1){
							if(unique == "Completed"){
							var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Completed');
							}
							else if(unique == "Incomplete"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Incomplete');
							}
							else if(unique == "Canceled"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('Canceled');
							}
							else if(unique == "No Show"){
								var messageLine = Drupal.t('LBL3081') + " " +Drupal.t('No Show');
							}
						}else {
							var messageLine = Drupal.t('LBL3082');
						}
					}else{
						var messageLine = Drupal.t('LBL3081') + " " +enrollmentStatus;
					}					
					$("#complete-confirmation-wizard").dialog({
						position:[(getWindowWidth()-500)/2,100],
						autoOpen: false,
						bgiframe: true,
						width: 500,
						resizable: false,
						modal: true,
						title: messageLine,
						buttons: confButton,
						closeOnEscape: false,
						draggable: false,
						zIndex : 10005,
						overlay: {
						   opacity: 0.9,
						   background: "black"
						}
					});
					
					$('.ui-dialog').wrap("<div id='complete-confirmation-wizard-dialog'></div>");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					
					changeDialogPopUI();
					$("#complete-confirmation-wizard").show();
					$("#complete-confirmation-wizard").dialog('open')
				} catch(e) {
				}
			},
			refreshLastAccessedRow: function (gridRow) {
				try {
					//console.log('refreshLastAccessedRow called');
					var rowFound = false;
					// console.log('refreshLastAccessedCatalogRow', console.trace());
					var grid = $("#narrow-search-results-holder");
					if (gridRow === undefined) {
						var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
					}
					//console.log(gridRow);
					if (gridRow !== undefined && gridRow != null && typeof gridRow.id != 'undefined') {
						//console.log(gridRow, gridRow.id);
						var rowData = ((typeof gridRow.id != 'undefined' && gridRow.id) ? gridRow.id.split('-') : null);
						var options = {
							data: {
								jqgrid_row_id: gridRow.id,
								page: 1,
								rows: 1
							}
						};
						grid.jqGrid('updateRowByRowId', options);
						rowFound = true; // return true to stop the grid reload
						grid.jqGrid('setGridParam', {lastAccessedRow: null});
					} else {
						// grid.trigger("reloadGrid",[{page:1}]);
						rowFound = false; // return false so that reload of grid happens as no last accessed grid row found
					}
				} catch (e) {
					//console.log(e, e.stack);
				}
				//console.log('rowFound ', rowFound, 'gridRow', gridRow);
				return rowFound;
			}
		});
	 $.extend($.ui.narrowsearch.prototype, EXPERTUS_SMARTPORTAL_AbstractManager,EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
		 window.paginationEnter = false;
			$('#add_lst_pg_txtfld').live('click onfocus mousedown',function(){
				$(this).keypress(function(event){
					 if (event.keyCode == 13) {
						 window.paginationEnter = true;
		              }
				});
			});
		$('.enable-edit-icon').live('click mousedown',function(){
			window.paginationEnter = false;
		});
		$('form').live('keyup keypress',function (e) {
		    var charCode = e.charCode || e.keyCode || e.which;
		    if (charCode  == 13 && e.target.nodeName != 'TEXTAREA') {
		        return false;
		    }
		});
		vtip();
	}catch(e){
			// To Do
		}
})(jQuery);
$(function() {
	try{
	$("#root-admin").narrowsearch();
	}catch(e){
			// To Do
		}
});
function editClassDetails(courseId, classId){
	try{
	if($(".qtip-active").length > 0)
		$(".qtip-active").remove();
	$('.course-class-edit-qtip').remove();
	var classVisibility = $('#catalog-class-basic-addedit-form-'+classId).is(':visible');
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if(classVisibility == true){
		$('#catalog-class-basic-addedit-form-'+classId).hide();
	} else {
		$('#catalog-class-basic-addedit-form-'+classId).show();
		if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			$('#edit-class-list-'+classId).css('background-color','');
		} else {
			$('#edit-class-list-'+classId).css('background-color','#d7ecf9');
		}
		//$('#edit-class-list-'+classId).find('.admin-add-button-container').css('display','none');
	}
	if($('#edit-catalog-course-basic-cancel')){
		$('#edit-catalog-course-basic-cancel').click();
		$('.qtip').find('.qtip-button > div').each(function(){
		    var x = $(this).attr('id');
			if(x.indexOf('cre_sys_obt_crs')>0 || x.indexOf('Course_close')>0 || x.indexOf('PrintCerId')>0){
			 $(this).closest('.qtip').remove();
			};
		});
	}
	}catch(e){
			// To Do
		}
}

function editClassQTipOpen(clickId,i){
	try{
	//Added by Vincent for issue #0015360: Class screen Qtip alignment is incorrect
	var y = $('#catalog-course-basic-addedit-form').height();
	var z = Math.round(y-433+(80*i));
	$('#modal-content').scrollTop(z);

	if($('#'+clickId).hasClass('loaded-edit-class')){
		$('#'+clickId).click();
	}
	$('#'+clickId).addClass('loaded-edit-class');
	}catch(e){
			// To Do
		}
}

function editCourseDetailsView(){
	try{
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
    if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

/* added by ganesh #custom_attribute_0078975*/
function editCustomAttributeDetailsView(){
	try{
	$('#customattribute-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

function editLocationDetailsView(){
	try{
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}

function editAttributesDetailsView(){
	try{
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	$('.admin-course-edit-button-container').hide();
	if($('#edit-newtheme-cancel-link')){
		$('#edit-newtheme-cancel-link').click();
	}
	}catch(e){
			// To Do
		}
}
function addClassDetails(courseId){
	try{
	var classVisibility = $('#catalog-class-basic-addedit-form-').is(':visible');
	$('.admin-course-edit-button-container').show();
	$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	if(classVisibility == true){
		$('#catalog-class-basic-addedit-form-').hide();
		$('#add-edit-class-norecords').show();
	} else {
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#catalog-class-basic-addedit-form-').show();
		$('#add-edit-class-norecords').hide();
	}
	if($('#edit-catalog-course-basic-cancel')){
		$('#edit-catalog-course-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}


function addFacilityDetails(locationId){
	try{
	var facilityVisibility = $('#resource-facility-basic-addedit-form-').is(':visible');
	$('.admin-course-edit-button-container').show();
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	if(facilityVisibility == true){
		$('#resource-facility-basic-addedit-form-').hide();
		$('#add-edit-facility-norecords').show();
	} else {
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#resource-facility-basic-addedit-form-').show();
		$('#add-edit-facility-norecords').hide();
	}
	if($('#edit-resource-location-basic-cancel')){
		$('#edit-resource-location-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}

function editFacilityDetails(locationId, facilityId){
	try{
	var facilityVisibility = $('#resource-facility-basic-addedit-form-'+facilityId).is(':visible');
	$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
	$('.edit-class-list').css('background-color','#FFFFFF');
	$('.edit-class-list').find('.admin-add-button-container').css('display','block');
	if(facilityVisibility == true){
		$('#resource-facility-basic-addedit-form-'+facilityId).hide();
	} else {
		$('#resource-facility-basic-addedit-form-'+facilityId).show();
		if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
			$('#edit-facility-list-'+facilityId).css('background-color','');
		} else {
			$('#edit-facility-list-'+facilityId).css('background-color','#d7ecf9');
		}
		$('#edit-facility-list-'+facilityId).find('.admin-add-button-container').css('display','none');
	}
	if($('#edit-resource-location-basic-cancel')){
		$('#edit-resource-location-basic-cancel').click();
	}
	}catch(e){
			// To Do
		}
}


function displayResourceActionList() {
	try{
	$('.resource-add-list').toggle();
	}catch(e){
			// To Do
		}
}

function displayPubActionList(butnCls) {
	try{
	butnCls = typeof butnCls !== 'undefined' ? butnCls : '';
	if(butnCls!='')
		$('.'+butnCls).toggle();
	else
		$('.catalog-pub-add-list').toggle();
	}catch(e){
			// To Do
		}
}

function displayNarrowSearchAction(id,obj){
	try{
	$('.pub-action-list-'+id).toggle();
	$(obj).closest("tr").prevAll("tr").find("td").find(".narrowsearch-pub-add-list").hide();
	$(obj).closest("tr").nextAll("tr").find("td").find(".narrowsearch-pub-add-list").hide();
	}catch(e){
			// To Do
		}
}

function displayNotificationActionList(e) {
	try{
	$('.catalog-pub-add-list').toggle();
	/* 	0027406: Issue with Notifications */
	//event.stopPropagation();
	if(!e) var e = window.event;
	e.cancelBubble = true;
	e.returnValue = false;
	if ( e.stopPropagation ) e.stopPropagation();
	if ( e.preventDefault ) e.preventDefault();
       return false;
	}catch(e){
			// To Do
		}
}
function showHide(obj,uniqueId){
	try{
	var fieldsetId = $(obj).val();

	//var fieldsetId = $(obj).val();
    var fieldsetId = $(obj).parents('li').attr('id');
    //$(obj).find('option[value="'+fieldsetId+'"]').remove();
    /*
    if($(obj).find('option').length == 1){
        $(obj).remove();
        $('.showselect').remove();
    }
    */
    displayResourceActionList();
	if(fieldsetId=='attachment_fieldset') {
		$('#attachment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='tag_fieldset') {
		$('#tag-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='prerequisite_fieldset') {
		$('#prerequisite-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='equivalence_fieldset') {
		$('#equivalence-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='survey_fieldset') {
		$('#survey-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='assessment_fieldset') {
		$('#assessment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='session_detail_fieldset') {
		$('#session-details-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='wbt_detail_fieldset') {
		$('#wbt-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='register_fieldset') {
		$('#register-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='class_register_fieldset') {
		$('#classregister-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='custom_fieldset') {
		$('#custom-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='classroom_fieldset') {
		$('#classroom-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}else if(fieldsetId=='equipment_fieldset') {
		$('#equipment-fieldset-wrapper-'+uniqueId).removeClass('disable-class');
	}
	}catch(e){
			// To Do
		}
}

function selecteDropdown() {
	try{
	$(document).ready(function(){
		if (!$.browser.opera) {
			// select element styling
			$('select.select').each(function(){
				var title = $('.select').attr('title');
				if( $('option:selected', this).val() != ''  ) title = $('option:selected',this).text();
				$(this)
					.css({'z-index':10,'opacity':0,'-khtml-appearance':'none'})
					.after('<span class="showselect">' + title + '</span>')
					.change(function(){
						val = $('option:selected','.select').text();
						$('.select').next().text(val);
						});
			});

		};

	});
	}catch(e){
			// To Do
		}
}
function hideMessageInfo(){
	try{
    setTimeout(function() {
    	$(".messages").hide();
    	Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
	}, 10000);
	}catch(e){
			// To Do
		}
}
function increasePopWidth() {
	try{
	/* $("#modalContent > .ctools-modal-content").css('width','925px');
	$("#modal-content").css('width','890px');
	Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null); */
	}catch(e){
			// To Do
		}
}
function textfieldTitleBlur(obj, defaultValue, searchTypeHiddenId,sessionInsId){
	try{
	if(typeof(searchTypeHiddenId)==='undefined') searchTypeHiddenId = '';
	if(typeof(sessionInsId)==='undefined') sessionInsId = '';
	var searchTypeVal = '';
	if(searchTypeHiddenId != ''){
	  searchTypeVal	= $('#'+searchTypeHiddenId).val();
	}
	if(obj.value == '' || obj.value == defaultValue || obj.value == searchTypeVal){
		if(searchTypeVal != ''){
		  $(obj).val(searchTypeVal);
		}else{
		  $(obj).val(defaultValue);
		  if(sessionInsId != ''){
			  $('#hid_instructor_id').val('');
		  }
		}
		$(obj).addClass('input-field-grey');
	} else {
		$(obj).removeClass('input-field-grey');
	}
	}catch(e){
			// To Do
		}
}

function textfieldTitleClick(obj, defaultValue, searchTypeHiddenId){
	try{
	if(typeof(searchTypeHiddenId)==='undefined') searchTypeHiddenId = '';
	var searchTypeVal = '';
	if(searchTypeHiddenId != ''){
	  searchTypeVal	= $('#'+searchTypeHiddenId).val();
	}
	if(obj.value == defaultValue || (searchTypeVal != '' && obj.value == searchTypeVal)){
		$(obj).val('');
		$(obj).removeClass('input-field-grey');
	}
	}catch(e){
			// To Do
		}
}

// Attachment : While do click on to edit, the textbox class will change
function toEditInline(attachId,callEvent) {
	try{
	$("#"+attachId).addClass("edit-attachment-txtbox");
	$("#"+attachId).removeClass("non-editable-txt");
	$("#catalog-attachment-disp-container").find(".attachment-txt-box").each(function(){
	    var gId = $(this).attr("id");
	    if((gId  == attachId) && (callEvent == 'click')) {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).addClass("edit-attachment-txtbox");
	    	$("#"+gId).removeClass("non-editable-txt");
	    } else {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).removeClass("non-editable-txt");
	    	$("#"+gId).addClass("non-editable-txt");
	    }
	});
	}catch(e){
			// To Do
		}
}

//Attachment : While do click on text label, the textbox should be appear with dottet line
function toEditAttachment(id,act) {
	try{
	var callEvent ='blur';
	if(act == 'label') {
		$("#attachment_list_label_"+id).hide();
		$("#attachment_list_edit_"+id).show();
		$("#attachment_list"+id).focus();
		callEvent = 'click';
	} else {
		$("#attachment_list_edit_"+id).hide();
		$("#attachment_list_label_"+id).show();
	}
	var attachId = 'attachment_list'+id;
	$("#"+attachId).addClass("edit-attachment-txtbox");
	$("#"+attachId).removeClass("non-editable-txt");
	$("#catalog-attachment-disp-container").find(".attachment-txt-box").each(function(){
	    var gId = $(this).attr("id");
	    if((gId  == attachId) && (callEvent == 'click')) {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).addClass("edit-attachment-txtbox");
	    	$("#"+gId).removeClass("non-editable-txt");
	    } else {
	    	$("#"+gId).removeClass("edit-attachment-txtbox");
	    	$("#"+gId).removeClass("non-editable-txt");
	    	$("#"+gId).addClass("non-editable-txt");
	    }
	});
	}catch(e){
			// To Do
		}
}

// Attachment: In Edit attachment, to prevent default enter key submit
function toEditAttachmentKeyDown(evt) {
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 13) {
    	$("#attachment_done_btn").click();
    	evt.preventDefault();
        return false;
    }
	}catch(e){
			// To Do
		}
}

function disableEnrterKey(evt){
	try{
	  var eve = evt || window.event;
	  var keycode = eve.keyCode || eve.which || eve.charCode;

	  if (keycode == 13) {
	    eve.cancelBubble = true;
	    eve.returnValue = false;

	    if (eve.stopPropagation) {
	      eve.stopPropagation();
	      eve.preventDefault();
	    }

	    return false;
	  }
	}catch(e){
			// To Do
		}
}

function textfieldTitleChange(obj) {
	try{
	var selectTxt = obj.options[obj.selectedIndex].text;
	if(selectTxt == 'Select'){
		$(obj).removeClass('select-normal-text');
		$(obj).addClass('select-greyed-out-text');
	} else {
		$(obj).removeClass('select-greyed-out-text');
		$(obj).addClass('select-normal-text');
	}
	}catch(e){
			// To Do
		}
}
/* function selectDropdownOnclick(){
	var insList = $('#load_multiselect_session_instructor').val();
	var selectedId = $('#edit-session-presenter :selected').val();
	if(insList != null && insList != undefined  && insList != ''){
			var opt ='';
			var url = "/?q=administration/session/allpresenter/"+insList;
			$.ajax({
				url : url,
				async: false,
				success: function(data) {
					var is_selected = false;
					$.each(data, function(){
						var selected = "";
						if(this.id == selectedId) {
							selected = "selected";
							is_selected = true;
						}
						opt += '<option title="'+this.full_name+'" value="'+this.id+'"  ' +selected+ '>'+this.full_name+'</option>';
					});
					var selCls = (is_selected === false) ? "selected" :"";
					opt = '<option title="Select" '+selCls+'> Select </option>'+opt;
					$('#edit-session-presenter').html(opt);
					//$('#change_instructor').val(1);
				}
			});
	}else{
		$('#edit-session-presenter').html('<option title="Select"  value="selected">Select</option>');
	}
	
} */
function textfieldTitleChangeField(fieldId) {
	try{
	var selectTxt = $('#'+fieldId+' option:selected').text();
	if(selectTxt == 'Select'){
		$('#'+fieldId).removeClass('select-normal-text');
		$('#'+fieldId).addClass('select-greyed-out-text');
	} else {
		$('#'+fieldId).removeClass('select-greyed-out-text');
		$('#'+fieldId).addClass('select-normal-text');
	}
	}catch(e){
			// To Do
		}
}

function remove_messages(msgType){
	try{
	if(msgType == 'class') {
		$('#catalog-class-addedit-form-details .messages').remove();
	} else if(msgType == 'course') {
		$('#catalog-course-basic-addedit-form .messages').remove();
	} else if(msgType == 'location'){
		$('#resource-location-basic-addedit-form .messages').remove();
	} else if(msgType == 'facility'){
		$('#resource-facility-basic-addedit-form .messages').remove();
	}else if(msgType == 'profile'){
		$('#exp-sp-my-profile-form').remove();
	}else if(msgType == 'customattribute') { //#custom_attribute_0078975
		$('#customattribute-basic-addedit-form .messages').remove();
	}
	}catch(e){
			// To Do
		}
}

function opt_sel_attach(id) {
	try{
	if($("#edit-attachment-fieldset-attachment-attachment-radioname-"+id+"-url").attr("checked")) {
		$("#attach_url_control"+id).show();
		$("#attach_browse_control"+id).hide();
	} else {
		$("#attach_url_control"+id).hide();
		$("#attach_browse_control"+id).show();
	}
	}catch(e){
			// To Do
		}
}

function getStatusReason(statusSelectField, uniqueId) {
  try{
	var selectId = $(statusSelectField).attr('id');
	var statusName = $('#' + selectId + ' option:selected').text().toLowerCase();

	if (statusName == 'inactive') {
		$('#admin-status-inactive-part-' + uniqueId).show();
	} else {
		$('#admin-status-inactive-part-' + uniqueId).hide();
	}

	Drupal.ajax.prototype.commands.CtoolsModalAdjust();
  }catch(e){
		// To Do
	}
}
function changeDelTypeInAddClass(delTypeCode){
	try{
	if(delTypeCode != 'lrn_cls_dty_ilt') {
		var locationId = $('.addedit-edit-new_location').attr('id');
		$('#'+locationId).val('');
		$('.addedit-edit-new_location').attr('id');
		$('#'+locationId).val('');
		var maxSeatId = $('.addedit-edit-max_seats').attr('id');
		$('#'+maxSeatId).val('');
	}
	}catch(e){
			// To Do
		}
}

function changeDeliveryType(delTypeCode){
 try{
	//var selectId = $(deliveryType).attr('id');
	//var delTypeCode = $('#'+selectId).val();
	if(delTypeCode == 'lrn_cls_dty_vod' || delTypeCode == 'lrn_cls_dty_wbt'){
		$('#admin-class-delivery-data-part').hide();
		$('#admin-class-delivery-data-part-for-date').hide();//ui-datepicker-trigger
		//$('#admin-class-register-date-container').find('#edit-reg-end-date').attr('readonly','readonly').addClass('admin-grey-out-fields');
		//$('#admin-class-register-date-container').find('.ui-datepicker-trigger').css('visibility','hidden');
		$('.addedit-edit-min_seats').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-max_seats').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-waitlist_count').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('#admin-class-location-container').find('#edit-class-location').attr('readonly','readonly').addClass('admin-grey-out-fields');
		$('.addedit-edit-class_location').attr('disabled',true);
		$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		$('#two-col-row-regdate_deadlinedate .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		$('#two-col-row-delivery_type_price .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');

	}
	else{
		$('#admin-class-delivery-data-part').show();
		$('#admin-class-delivery-data-part-for-date').show();
		//$('#admin-class-register-date-container').find('#edit-reg-end-date').removeAttr('readonly').removeClass('admin-grey-out-fields');
		//$('#admin-class-register-date-container').find('.ui-datepicker-trigger').css('visibility','visible');
		$('.addedit-edit-min_seats').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('.addedit-edit-max_seats').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('.addedit-edit-waitlist_count').removeAttr('readonly').removeClass('admin-grey-out-fields');
		//$('#admin-class-location-container').find('#edit-class-location').removeAttr('readonly').removeClass('admin-grey-out-fields');
		$('#two-col-row-regdate_deadlinedate .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');
		$('#two-col-row-delivery_type_price .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');

		if(delTypeCode == 'lrn_cls_dty_vcl'){
			$('#admin-class-location-container').find('#edit-class-location').attr('readonly','readonly').addClass('admin-grey-out-fields');
			$('.addedit-edit-class_location').attr('disabled',true);
			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','hidden');
			$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		}
		else if(delTypeCode == 'lrn_cls_dty_ilt') {
		  $('#admin-class-location-container').find('#edit-class-location').removeAttr('readonly').removeClass('admin-grey-out-fields');
		  $('.addedit-edit-class_location').attr('disabled',false);
			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','visible');
		  $('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','visible');
		}
		else {
			$('.addedit-edit-class_location').attr('disabled',false);

			//$('#two-col-row-waitlist_count_currency_type').find('.addedit-mandatory').css('visibility','hidden');
			$('#two-col-row-lang_code_status_status_temp .addedit-twocol-secondcol .addedit-mandatory').css('visibility','hidden');
		}
	}
 }catch(e){
		// To Do
	}
}

function displayLongDescription(desc){
	try{
	if(desc == 'short_description') {
		var shortLength = $('.admin-course-class-short-description').find('.addedit-new-field-value').height();
		if(shortLength >= 50) {
				//$('.admin-course-class-short-description').find('.addedit-new-field-value').addClass('add-scroll-button-description');
		}
	}
	else if(desc == 'long_description') {
		var longLength = $('.admin-course-class-long-description').find('.addedit-new-field-value').height();
		if(longLength >= 50) {
				//$('.admin-course-class-long-description').find('.addedit-new-field-value').addClass('add-scroll-button-description');
		}
	}
	}catch(e){
			// To Do
		}
}

function limitTextareaChars(limitField, limitNum){
	try{
	var textValue = $('.textformat-textarea-editor').val();
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		$('.textformat-textarea-editor').val(newTextValue);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}

//Added for Custom Attribute #custom_attribute_0078975
function limitCustomAttributeTextareaChars(limitField, limitNum){ 
	try{
	 //alert('limitCustomAttributeTextareaChars');
	var textValue = $('#'+limitField+'_textarea_full_row .textformat-textarea-editor').val();
	// alert('textValue='+textValue);
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		//$('.textformat-textarea-editor').val(newTextValue);
		$('#'+limitField+'_textarea_full_row .textformat-textarea-editor').val(newTextValue);
		$('#char_count_' + limitField).html(limitNum);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}



//For MetaTag Feature
function limitTextFieldChars(limitField, limitNum){
	try{
	var textValue = $('.addedit-edit-textfield').val();
	var newTextValue = '';
	var limitCount = '';
	if (textValue.length > limitNum) {
		newTextValue = textValue.substring(0, limitNum);
		$('.addedit-edit-textfield').val(newTextValue);
	} else {
		//limitCount = limitNum - textValue.length;
		limitCount = textValue.length;
		$('#char_count_' + limitField).html(limitCount);
	}
	}catch(e){
			// To Do
		}
}

function fillCancelReason(uniqueId){
	try{
	var cancelReason = $('#cancel_reason_'+uniqueId).val();
	$('input[name="hidden_cancel_reason"]').val(cancelReason);
	$('#qtipIdqtip_class_cancel_disp_'+uniqueId).closest('.qtip-active').remove();
	}catch(e){
			// To Do
		}
}

(function($) {
	$.fn.refreshVersionList = function(contentId) {
		try{
		/* refresh version list on save or other action performed */
		//var pageNo = $(".ui-pg-input").val();
		$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
		$("#admin-version-list-"+contentId+"-pagination").trigger("reloadGrid",[{page:1}]);
		}catch(e){
  			// To Do
  		}
	};
	
	$.fn.putCustomLabelForContentAuthor = function(){
		putCustomLabelForContentAuthorWrapper();
	};
	
	$.fn.initializePresentationObjects = function()
	{
		initializePresentationObjectsWrapper();	 
	
	}
	$.fn.hideUploadVideoControlAndDisplayLabel = function()
	{
	
		hideUploadVideoControlAndDisplayLabelWrapper();
	};
})(jQuery);


(function($) {
	$.fn.refreshMoveUsersQtip = function(contentId,versionId) {
		try{
		$('input[name="hidden_idlist_ContentMoveUsers-'+contentId+'-'+versionId+'"]').attr('value',0);
		/* refresh users list in move users qtip on save or other action performed */
		$("#datagrid-container-ContentMoveUsers-"+contentId+"-"+versionId).trigger("reloadGrid",[{page:1}]);
		}catch(e){
  			// To Do
  		}
	};

  $.fn.closeMoveUsersQtip = function() {
	  try{
    $(".qtip-active").remove();
    isPopupOpen=0;
	  }catch(e){
			// To Do
		}
  };
})(jQuery);

(function($) {
	$.fn.attachClose = function(entityId,entityType,type) {
		try{
		if(entityType == 'cre_sys_obt_trp'|| entityType == 'cre_sys_obt_cur' || entityType == 'cre_sys_obt_crt' || entityType == 'cre_sys_obt_trn') {
			var object_id = entityId.split('-');
			entityId = object_id[0];
			$('#qtipAttachCrsIdqtip_visible_disp_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}else if(entityType == 'cre_sec'){
			$('#qtipAttachIdqtip_addusers_visible_disp_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}else if(entityType == 'admin-order'){
			$(".active-qtip-div").remove();
		}else{
			$('#qtip_visible_disp_attach_question_'+entityId+'_'+entityType).closest('.qtip-active').remove();
		}
		if(entityType == 'cre_sys_obt_trp' || entityType == 'cre_sys_obt_cur' || entityType == 'cre_sys_obt_crt' || entityType == 'cre_sys_obt_trn'){
			var modId = object_id[1];
			var pos = ''; 
			if(modId != '' && modId != undefined && modId != 'undefined' && ($("#module-list-"+modId).index()>= 0)){
				pos = $("#module-list-"+modId).index();
				
			}else{
				//pos = ($(".first-arrow").size() > 0) ? 1 : 0;
				pos = $( "#page-container-tabs-prg .visible-main-tab:last" ).index();
			}
				if($( ".first-arrow" ).size() > 0 ){
					pos = pos-1;
				}
			$( "#page-container-tabs-prg" ).tabs('destroy');
			
	    	$( "#page-container-tabs-prg" ).tabs({
	    		selected: pos
	    	});
	    	
	    	/*Diable attach courses*/
	    	var str = $('#program_attach_tabs .ui-state-active').attr('id');
			var currmod = str.split('-');
			moduleTabclick(currmod[2]);
		}
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

(function($) {
	$.fn.validateCustomFields = function(customWrapperId,customErrorFlag) {
		try{
		if(customErrorFlag==true)
			$('#'+customWrapperId).show();
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

(function($) {
	try{
	$.fn.validateCertificate = function(defaultCertifyId) {
		$('input:radio[name="attach_certificate"]').filter('[value="'+defaultCertifyId+'"]').attr('checked', true);
		$('#certificate-display-table').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollPerm = function(permWrapperId) {
		$('#admin-add-scroll').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollTag = function(tagWrapperId) {
		$('#tag-scroll-id').jScrollPane({});
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.scrollView = function(argsList) {
		$("#view-scroll-wrapper").jScrollPane({});
		vtip();
		if($('#viewgroup-detail-wrapper').length) {	//to check group view form is open
			$('#modalContent .ctools-modal-content #ctools-face-table .popup-block-footer-right').css('right', '45px'); //57996
		}
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.cancelClassDetails = function(classId, editable) {
		$('#qtipAttachIdqtip_addlocation_visible_disp_'+classId).closest('.qtip-active').remove();

		if(document.getElementById('qtipAttachIdqtip_addclass_visible_disp_'+classId)){
		  $('#qtipAttachIdqtip_addclass_visible_disp_'+classId).closest('.qtip-active').remove();
		}
		if(document.getElementById('qtipAttachIdqtip_addclass_visible_disp_')){
			  $('#qtipAttachIdqtip_addclass_visible_disp_').closest('.qtip-active').remove();
		}
		if(document.getElementById('qtip_editclass_visible_dispid_'+classId)){
		  $('#qtip_editclass_visible_dispid_'+classId).closest('.qtip-active').remove();
		}
		if (!editable && editable != null && editable != undefined) {
			$("#edit-catalog-course-basic-cancel").click();
		}
		$('#catalog-class-addedit-form-details .addedit-form-wrapper').hide();
		$('.edit-class-list').css('background-color','#FFFFFF');
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#catalog-class-basic-addedit-form-'+classId).hide();
	};

	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	$.fn.cancelFacilityDetails = function(facilityId) {
		try{
		$('#resource-facility-addedit-form-details .addedit-form-wrapper').hide();
		$('.edit-class-list').css('background-color','#FFFFFF');
		$('.edit-class-list').find('.admin-add-button-container').css('display','block');
		$('#resource-facility-basic-addedit-form-'+facilityId).hide();
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);

function clearMessages(){
	try{
	$(".messages").hide();
	}catch(e){
			// To Do
		}
}

/*function attachmentUploadFile() {
  var provideUrlVisibility = $('#provide_url_id').is(':visible');

  if(provideUrlVisibility == true){
	 $("#provide_url_id").hide();
  }
  $("#attachment_upload_file").click();
}*/

function attachmentProvideUrl() {
	try{
	var provideUrlVisibility = $('#provide_url_id').is(':visible');

	if(provideUrlVisibility == true){
		$("#provide_url_id").hide();
	} else {
		$("#provide_url_id").show();
	}
	}catch(e){
			// To Do
		}
}
function bubblePopupClose(popupId) {
	try{
	$("#"+popupId).remove();
	}catch(e){
			// To Do
		}
}

function removebubblePopup(){
	try{
	$('.qtip').remove();
	}catch(e){
			// To Do
		}
}

function attachSubmitUrl() {
	try{
	//alert("Heree calling");
	// attachment_done_btn
	$("#attachment_done_btn").click();
	}catch(e){
			// To Do
		}
}

function validateCustomFieldClass() {
	try{
	$("#userbasic-custom-fields").find('.input-field-grey').each(function(){
	    var gId= $(this).attr("id");
	    var gVal = $("#"+gId).val();
	    if((gVal == 'Enter a Label') || (gVal == 'Enter a Value')) {
	        $("#"+gId).removeClass('input-field-grey');
	    	$("#"+gId).addClass('input-field-grey');
	    } else {
	    	$("#"+gId).removeClass('input-field-grey');
	    }
	});
	}catch(e){
			// To Do
		}
}


function sequenceDragAndDrop(containerId, type){
	try{
	if(type == 'attach_class_content'){
	  $("#"+containerId+' tbody').sortable({
		handle: '.dragndrop-selectable-item',
		cursor: 'crosshair'
	  });
	}
	else{
	  $("#"+containerId).sortable({
		handle: '.dragndrop-selectable-item',
		cursor: 'crosshair'
	  });
	}
	//$("#"+containerId).bind('click.sortable mousedown.sortable',function(ev){ ev.target.focus(); });
	//$("#"+containerId).disableSelection();
	$("#"+containerId).droppable({
		drop: function( event, ui ) {
			var sequenceOrder = '';
			var sequenceOrderArray = new Array();
			var c = 0;
			if(type == 'attach_class_content'){
				var currentId = $('#'+containerId+' tr.ui-sortable-helper').attr('id');
				var prevId = $('#'+containerId+' tr.ui-sortable-placeholder').prev().attr('id');
			}
			else{
				var currentId = $('#'+containerId+' li.ui-sortable-helper').attr('id');
				var prevId = $('#'+containerId+' li.ui-sortable-placeholder').prev().attr('id');
			}
			var recordStored = 0;
			if(prevId == undefined){
				sequenceOrderArray[0] = currentId;
				recordStored = 1;
				c = 1;
			}
			if(type == 'attach_class_content'){
				$('#'+containerId+' tr.ui-widget-content').each(function(){
					if($(this).attr('id') != currentId){
						sequenceOrderArray[c] = $(this).attr('id');
						if($(this).attr('id') == prevId){
							c++;
							sequenceOrderArray[c] = currentId;
						}
					}
					c++;
			   });
			}
			else{
				$('#'+containerId+' li.draggable-list').each(function(){
				if($(this).attr('id') != currentId){
					sequenceOrderArray[c] = $(this).attr('id');
					if($(this).attr('id') == prevId){
						c++;
						sequenceOrderArray[c] = currentId;
					}
				}
				c++;
				});
			}
			sequenceOrder = sequenceOrderArray.join(",");
			var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
			obj.createLoader(containerId);
			var i=1;
			$.ajax({
				url : obj.constructUrl("administration/sequence-drag-drop/"+type+"/"+sequenceOrder),
				async: false,
				success: function(data) {
					obj.destroyLoader(containerId);
					var sequenceArray = sequenceOrderArray.filter(function(v){return v!==''});
					var list_values = ($('#dragndrop li.draggable-list').length)-1;
					var arrLen = sequenceArray.length;
					if(list_values == arrLen ) {
						var i = 0;
						for(i=0; i<arrLen; i++) {
							$("#dragndrop li[id="+ sequenceArray[i] +"]").removeClass('odd-even-row-highlighter');
							if (i%2 == 1) {
								$("#dragndrop li[id="+ sequenceArray[i] +"]").addClass('odd-even-row-highlighter');
							}
						}
					}

				}
			});

		}
	});
	}catch(e){
			// To Do
		}
}
    $('#mandatory-list .man-opt-selection').live('click', function(){
    	try{
		if($(this).next().css('display') == 'block'){
			$(this).next().css('display', 'none');
		} else {
			$('.sub-menu').hide();
			$(this).next().css('display', 'block');
		}
    	}catch(e){
  			// To Do
  		}
	});


	$('#mandatory-list .sub-menu li').live('click', function(){
		try{
		var selectedMO = $(this).html();
		$(this).parent().prev('span').prev('span').html(selectedMO);
		$(this).parent().css('display', 'none');
		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;

		var attachValues = $(this).attr('data');
		//alert(attachValues);
		var manValue = 1;
		obj.createLoader('attach_course_dt');
		$.ajax({
			url : obj.constructUrl("administration/update-attachedcourse-mandatory/"+attachValues),
			async: false,
			success: function(data) {
				obj.destroyLoader('attach_course_dt');
				$("#root-admin").data("narrowsearch").refreshGrid();
			}
		});

		return false;
		}catch(e){
  			// To Do
  		}
	});


    $('#attach-question-mandatory-list .man-opt-selection').live('click', function(){
    	try{
		if($(this).next().css('display') == 'block'){
			$(this).next().css('display', 'none');
		} else {
			$(this).next().css('display', 'block');
		}
    	}catch(e){
  			// To Do
  		}
	});


	$('#attach-question-mandatory-list .sub-menu li').live('click', function(){
		try{
		var selectedMO = $(this).html();
		$(this).parent().prev('span').prev('span').html(selectedMO);
		$(this).parent().css('display', 'none');
		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;

		var attachValues = $(this).attr('data');
		//alert(attachValues);
		var manValue = 1;
		obj.createLoader('survey_attach_questions_dt');
		$.ajax({
			url : obj.constructUrl("administration/update-attachedquestion-mandatory/"+attachValues),
			async: false,
			success: function(data) {
				obj.destroyLoader('survey_attach_questions_dt');
			}
		});

		return false;
		}catch(e){
  			// To Do
  		}
	});


	(function($) {
		$('.addedit-customfields-wrapper .custom-field-label').live('dblclick', function(){
			try{
			var labelId = $(this).attr('id');
			var inputId = labelId.replace('custom_label_custom_value_', 'custom_input_custom_value_');
			$('#'+labelId).hide();
			$('#'+inputId).show();
			$('#'+inputId+' input').focus();
			}catch(e){
	  			// To Do
	  		}
		});
	})(jQuery);

	function updateCustomLabelKeyDown(evt, newValue, labelDetails, id, loaderWrapper){
		try{
		evt = evt || window.event;
	    var charCode = evt.keyCode || evt.which;
	    if (charCode == 13) {
	        updateCustomLabel(newValue, labelDetails, id, loaderWrapper);
	        evt.preventDefault();
	        return false;
	    }
		}catch(e){
  			// To Do
  		}
	}

	function updateCustomLabel(newValue, labelDetails, id, loaderWrapper) {
		try{
		var labelDetailsArray = labelDetails.split('-');
		var customId = labelDetailsArray[0];
		var entityId = labelDetailsArray[1];
		var entityType = labelDetailsArray[2];

		var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
		obj.createLoader(loaderWrapper);
		$.ajax({
			type: "POST",
			async: false,
			url: obj.constructUrl("administration/update-custom-fields/"+newValue+"/"+customId),
			data: '',
			success: function(result){
				$('#custom_label_custom_value_'+id).html(result);
				$('#custom_input_custom_value_'+id).hide();
				$('#custom_label_custom_value_'+id).attr('title', newValue);
				$('#custom_label_custom_value_'+id).show();
				obj.destroyLoader(loaderWrapper);
			},
			failure : function(){
				obj.destroyLoader(wrapper);
			}
	    });
		}catch(e){
  			// To Do
  		}
		    }
	function attachCrsDelete(callId) {
		try{
		if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
			var esignObj = {'esignFor':'attachCrsDelete','objectId':callId};
			$.fn.getNewEsignPopup(esignObj);
		}else{
			$("#"+callId).click();
		    }
		}catch(e){
  			// To Do
  		}
	}

	function setQtipPosition(){
		try{
		if(document.getElementById('qtip_position')!=null){
			if($('#qtip_position').val()!=''){
				try{
					var initVal = $('#qtip_position').val().split('#');
					var bubbleha = $("#"+initVal[0]).css('height');
					var bubbleTop = initVal[1];
					var bubblehb = initVal[2];
					bubbleha = parseInt(bubbleha.substring(0,bubbleha.length-2));
					bubbleTop = bubbleTop-(bubbleha-bubblehb);
					$("#"+initVal[0]).closest(".qtip-active").css('top',bubbleTop);
				}catch(e){}
			}
		}
		}catch(e){
  			// To Do
  		}
	}

	/**
	 * closeQtyp()
	 *
	 * This function has been added by Vincent on July 19, 2012.
	 * This will be called on clicking the qtip close icon form the top right corner of the Qtip
	 *
	 * @param id
	 * @return
	 */
	function closeQtyp(id,courseId){
		try{
		var tmp ;
		//reloadFlagForPagination=true;
		if(id.indexOf('addclass')>=0){
			tmp=0;
			 //Fixed for #0038166 : added to get hidden value before ctool get closed.
	        var emptyId = $('#bubble-face-table #empty_id').val();
			var entityType = $('#bubble-face-table #entity_value').val();
			if(emptyId != null && emptyId != undefined  && emptyId != ''){
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
		}else{
			tmp = id.substring(id.indexOf('dispid_')+7);
		}
		if(id.indexOf('addclass')>=0 || id.indexOf('editclass')>=0){
		  //Drupal.ckeditorSubmitAjaxForm();
			$("#admin-course-class-list-pagination-wrapper").empty().remove();
			$('#paint-class-search-results-datagrid').empty().remove();
			var res = document.getElementById('enrollment_upload_done_btn_view');
			var result = document.getElementById('course_class_cancel_button');
			if(document.getElementById('attachment-addedit-form-html')){
					$('#attachment-addedit-form-html').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('attachment_upload_file')!=undefined && document.getElementById('attachment_upload_file')!=null && document.getElementById('attachment_upload_file')!='')
					$('#attachment_upload_file').remove();
			}
			else if(res !=null && res != undefined && res != '' ){
				var formname = document.getElementsByClassName('addedit-form');
				if (formname.length > 0)
						$('.addedit-form').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('enrollment_upload_file_view')!=undefined && document.getElementById('enrollment_upload_file_view')!=null && document.getElementById('enrollment_upload_file_view')!='')
					$('#enrollment_upload_file_view').remove();
				if(document.getElementById('enrollment_upload_file')!=undefined && document.getElementById('enrollment_upload_file')!=null && document.getElementById('enrollment_upload_file')!='')
					$('#enrollment_upload_file').remove();
			} else if(result !=null && result != undefined && result != '' ){
				var formname = document.getElementsByClassName('addedit-form');
				if (formname.length > 0)
					$('.addedit-form').attr('enctype','application/x-www-form-urlencoded');
				if(document.getElementById('enrollment_upload_file_view')!=undefined && document.getElementById('enrollment_upload_file_view')!=null && document.getElementById('enrollment_upload_file_view')!='')
					$('#enrollment_upload_file_view').remove();
				if(document.getElementById('enrollment_upload_file')!=undefined && document.getElementById('enrollment_upload_file')!=null && document.getElementById('enrollment_upload_file')!='')				
					$('#enrollment_upload_file').remove();
				if(document.getElementById('course_class_cancel_button')!=undefined && document.getElementById('course_class_cancel_button')!=null && document.getElementById('course_class_cancel_button')!='')
		 	        $("#course_class_cancel_button").click();
			}
		  	removeQtip(tmp);
		  	if(window.location.href.indexOf("admincalendar")>0)
			{
				$(".close").click();
				adminCalCloseHandler("classaddedit");
			}
		}
		if(id.indexOf('addtemplate') >=0 || id.indexOf('edittemplate') >=0 ){
			//alert(document.getElementById('notify_template_cancel_button'));
			$("#notify_template_cancel_button").click();
			removeClassQtip();
		}
		if(id.indexOf('MoveUsersqtip') >=0 ){
			var contentId = id.split("_");
			isPopupOpen=0;
			$.fn.refreshVersionList(contentId[3]);
		}
		$('#'+id+"_disp").html('');
		$('#'+id+"_disp").closest(".qtip-active").hide();
		
		if(id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_dpt" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_etp" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_jrl"  || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_jtl" || id == "qtipAttachIdqtip_listvalues_visible_disp_0_cre_usr_ptp"){
			$('.page-administration-people-setup #add_lst_pg_next,.page-administration-people-setup #add_lst_pg_previous,.page-administration-people-setup #add_lst_pg_last,.page-administration-people-setup #add_lst_pg_first,.page-administration-people-setup #add_lst_pg_txtfld,.page-administration-people-setup #listvalues-autocomplete,.page-administration-people-setup #search_listvalues').removeAttr('style');								
		}
		
		
		}catch(e){
  			// To Do
  		}
	}

	/**
	 * setWordCountDiv()
	 *
	 * This function has been added by Vincent on July 24, 2012 to fix ckeditor word count issue(#0014481).
	 * It is drived form /ckeditor/wordcount/plugin.js to create word count div
	 *
	 * @param id
	 * @return
	 */
	function setWordCountDiv(id){
		try{
		setTimeout(function(){
			try{
				var editorRealName;
				var editor = CKEDITOR.instances[id];
				if(editor!=null && editor!=undefined && editor!='undefined'){
					editorRealName = (editor.element.$.name).split('[');
					if(!document.getElementById('cke_wordcount_'+editor.name)) {
			    		$('#char_count_'+editorRealName[0]).html('<div id="cke_wordcount_'+editor.name+'" style="display: inline-block; float: right; text-align: right; margin-top: 0px; cursor:auto; font:10px helvetica neue,helvetica,arial; height:auto; padding:0; text-align:left; text-decoration:none; vertical-align:baseline; white-space:nowrap; width:auto;color:#666666;">'+$('#char_count_'+editorRealName[0]).html()+'</div>');
			    	}
				}
			}catch(e){
				//Do nothing;
			}
		},4000);
		}catch(e){
  			// To Do
  		}
	}

	function removeAllQtip(){
		try{
		$('.qtip').remove();
		}catch(e){
  			// To Do
  		}
	}

	function removeQtip(id){
		try{
		//reloadFlagForPagination = true;
		if(id== -1){
			$('#qtipAttachIdqtip_addlocation_visible_disp__close').closest('.qtip').remove();
		}else if(id==0){
			$("#course_class_cancel_button").click();
			$('#qtipAttachIdqtip_addclass_visible_disp__close').closest('.qtip').remove();
			$('#qtipAttachIdqtip_addlocation_visible_disp__close').closest('.qtip').remove();
		}else{
			$("#course_class_cancel_button").click();
			$('#qtipAttachIdqtip_addlocation_visible_disp_'+id+'_close').closest('.qtip').remove();
		}
		removeClassQtip();
		}catch(e){
  			// To Do
  		}
	}

	function removeClassQtip(){
		try{
		$('.qtip').find('.qtip-button > div').each(function(){
		    var x = $(this).attr('id');
			if(x.indexOf('cre_sys_obt_cls')>0 || x.indexOf('Class_close')>0){
			 $(this).closest('.qtip').remove();
			};
		});
		}catch(e){
  			// To Do
  		}
	}


(function($) {
	$.fn.addCustomGrayConversion = function(customCount) {
		try{
		//alert(customCount);
		/*for(var i=0; i<customCount;i++){
			$('.addedit-edit-custom-label-field-'+i).addClass('input-field-grey');
			$('.addedit-edit-textarea-custom_value_'+i).addClass('input-field-grey');

		}*/
		$(".addedit-customfields-wrapper").find('.input-field-grey').each(function(){
		    var gId= $(this).attr("id");
		    var gVal = $("#"+gId).val();
		    if((gVal == 'Enter a Label') || (gVal == 'Enter a Value')) {
		        $("#"+gId).removeClass('input-field-grey');
		    	$("#"+gId).addClass('input-field-grey');
		    } else {
		    	$("#"+gId).removeClass('input-field-grey');
		    }
		});
		}catch(e){
  			// To Do
  		}
	};

	// Time picker
	/*$('input.exp-timepicker').live('click',function(){
		var attributeId = $(this).attr('id');
		var expDropDownId = 'exp-dropdown-'+attributeId;
		var hours, minutes;
		$('.exp-timepicker-selection').css('display', 'none');
		if($('#'+expDropDownId).length >0){
			if($('#'+expDropDownId).is(':visible') == true){
				$('#'+expDropDownId).css('display', 'none');
			} else {
	 			$('#'+expDropDownId).css('display', 'block');
			}
		} else {
			$(this).after('<div id="'+expDropDownId+'" data="'+attributeId+'" class="exp-timepicker-selection"><ul></ul></div>');
			for(var i = 0 ;i <= 1425; i += 15){
				hours = Math.floor(i / 60);
				minutes = i % 60;
				if (hours < 10){
				    hours = '0' + hours;
				}
				if (minutes < 10){
				    minutes = '0' + minutes;
				}
				$('#'+expDropDownId+' ul').append('<li>'+hours + ':' + minutes + '</li>');
		    }
		}
	});

	$('div.exp-timepicker-selection li').live('click', function(){
		var attributeId = $(this).closest('.exp-timepicker-selection').attr('data');
		var selectedTime = $(this).html();
		$('#'+attributeId).val(selectedTime);
		$(this).closest('.exp-timepicker-selection').css('display', 'none');
	});*/
	$('body').click(function(event){
		try{
		if($(event.target).hasClass('exp-timepicker') != true){
			$('.exp-timepicker-selection').css('display', 'none');
		}
		}catch(e){
  			// To Do
  		}
	});

})(jQuery);

//Hide the drop down menu option
  $('body,#admin-add-course-training-plan .ui-widget-content').bind('click', function(event) {
	  try{
	  if(event.target.id != 'admin-dropdown-arrow'){
		 $('#select-list-course-dropdown-list').hide();
		 $('#select-list-class-dropdown-list').hide();
		 $('#select-list-surass-dropdown-list').hide();
    }
	  }catch(e){
			// To Do
		}
  });
   //$('body').bind('mousedown', function(event) {
  //if((event.target.nodeName == 'SELECT') || (event.target.nodeName == 'INPUT' && ($(event.target).attr('type') == 'text'|| $(event.target).attr('type') == 'submit'))){
  $('body').bind('mousedown', function(event) {
	  try{
	  var nsParts = window.location.href.split("/?q=");
 	   if((event.target.nodeName == 'SELECT') || (event.target.nodeName == 'INPUT')
 			  || (event.target.nodeName == 'TEXTAREA') || (event.target.nodeName == 'A' && nsParts[1] !='administration/order/create')
 		  	  || (event.target.nodeName == 'SPAN' && nsParts[1] !='administration/order/create') || (event.target.nodeName == 'LI')){
		  $('#message-container').remove();
	  }
	  }catch(e){
			// To Do
		}
  });
/*$('#location-list .loc-disp-selection').live('click', function(){
	if($(this).next().css('display') == 'block'){
		$(this).next().css('display', 'none');
		$('.add-location-cls').css('display', 'none');
	} else {
		$(this).next().css('display', 'block');
		$('.add-location-cls').css('display', 'block');
	}
});

$('#location-list .sub-menu-list li').live('click', function(){
	var selectedMO = $(this).html();
	$(this).parent().prev('span').prev('span').html(selectedMO);
	$(this).parent().css('display', 'none');
	$('.add-location-cls').css('display', 'none');

	var attachValues = $(this).attr('data');
	//$('#class_location').val(attachValues);
	$('input[name$="class_location"]').val(attachValues);

	return false;
});*/

//$('#location-list .loc-disp-selection').live('click', function(){
$('#location-list .top-select').live('click', function(){
	try{
	if($('.sub-menu-list').css('display') == 'inline-block'){
		$('.sub-menu-list').css('display', 'none');
		$('.add-location-cls').css('display', 'none');
	} else {
		var actualWidthofUL = $('ul.sub-menu-list').width();
		if ($.browser.msie  && parseInt($.browser.version, 10) === 7) {
			var widthofUL = actualWidthofUL + 37;
		} else {
			var widthofUL = actualWidthofUL + 10;
			}
		$('.sub-menu-list').css('display', 'inline-block');
		$('.add-location-cls').css('width', +widthofUL+'px');
		$('.add-location-cls').css('display', 'block');
	}
	}catch(e){
			// To Do
		}
});

$('#location-list .sub-menu-list li').live('click', function(){
	try{
	var selectedMO = $(this).html();
	$('#location-input-txt').val(selectedMO);
	$(this).parent().css('display', 'none');
	$('.add-location-cls').css('display', 'none');

	var attachValues = $(this).attr('data');
	//$('#class_location').val(attachValues);
	$('input[name$="class_location"]').val(attachValues);

	return false;
	}catch(e){
			// To Do
		}
});

(function($) {
	try{
	$.fn.classCreateLocation = function(locId,locName,classId) {
		$('#newlocationpaint').append("<li data="+locId+" title='"+locName+"'>"+locName+"</li>");
		$('#location-list .sub-menu-list li ').click();
		sortUnorderedList("newlocationpaint");
/*		if(classId){
			$('#qtipAttachIdqtip_addlocation_visible_disp_'+classId).closest('.qtip-active').remove();
		}*/
	};
	}catch(e){
			// To Do
		}
})(jQuery);
function sortUnorderedList(ul, sortDescending) {
	try{
	  if(typeof ul == "string")
	    ul = document.getElementById(ul);
	  // Get the list items and setup an array for sorting
	  var lis = ul.getElementsByTagName("LI");
	  var vals = [];
	  // Populate the array
	  for(var i = 0, l = lis.length; i < l; i++)
	    vals.push(lis[i].innerHTML);
	  // Sort it
	  vals.sort();
	  if(sortDescending)
	    vals.reverse();
	  // Change the list on the page
	  for(var i = 0, l = lis.length; i < l; i++)
	    lis[i].innerHTML = vals[i];
	}catch(e){
			// To Do
		}
}
if(!document.getElementById('location-list')){
	try{
	$('body').click(function(event) {
		if($(event.target).hasClass('add-location-cls') != true){
			$('.sub-menu-list').css('display', 'none');
			$('.add-location-cls').css('display', 'none');
		}
	});
	}catch(e){
			// To Do
		}
}
function handlePostLoadDropdown(toBeHidden,toBeShown){
	try{
	$('#'+toBeHidden).hide();
	$('#'+toBeShown).show();
	}catch(e){
			// To Do
		}
}

function searchClassNameFilter(courseId,classId,ClassName){
	try{
	var obj = this;
	var pagerId	= '#admin-course-class-list-'+courseId+'-pagination_toppager';
	var objStr = '$("#root_admin").data("narrowsearch")';
	}catch(e){
			// To Do
		}
}

function onlyNumbers(evt)
{
	try{
     var charCode = (evt.which) ? evt.which : evt.keyCode;
     if (charCode == 8 || charCode == 9 || charCode == 46 || (charCode >= 37 && charCode <= 40) || (charCode >= 48 && charCode <= 57) || (charCode >= 96 && charCode <= 105))
    	 return true;
     else
     	return false;
     /*else if (charCode > 31 && (charCode < 48 || charCode > 57))
        return false;

     return true;*/
	}catch(e){
			// To Do
		}
}
/* Added by Gayathri for #0073578 */
function onlyNumbersforPrice(evt)
{
	try{
     var charCode = (evt.which) ? evt.which : evt.keyCode;
     if (charCode == 8 || charCode == 9 || charCode == 46 || charCode == 110 || (charCode >= 37 && charCode <= 40) || (charCode >= 48 && charCode <= 57) || (charCode >= 96 && charCode <= 105))
    	 return true;
     else
     	return false;    
	}catch(e){
			// To Do
	}
}
/* Set the focus on location field */
function changeLocationFocus(focusType){
	try{
  if(focusType=='deltype') {
		$('.addedit-edit-delivery_type').focusout();
	}else{
	     $('.addedit-edit-title').focusout();
	}
	}catch(e){
			// To Do
		}
}
/* Set the focus on first field */
function autoFocusFirstField() {
	try{
  $('form').find('select,textarea,input[type=text]').filter(':visible:first').focusout();
	}catch(e){
			// To Do
		}
}

/* Get Meeting details from popup tooltip and assign into form */
function getMeetingDetails(pos) {
	try{
	 /*var timezone = $('#timezone').val();
	 var session_attende_password = $('#session_attende_password').val();
	 var session_presenter_password = $('#session_presenter_password').val();
	 var session_attendee_url = $('#session_attendee_url').val();
	 var session_presenter_url = $('#session_presenter_url').val();
	 var instructorid = $('#hid_instructor_id').val();
	 var session_meeting_id = $('#session_meeting_id').val();

	 $('#time_zone_'+pos).val(timezone);
	 $('#session_attendeepass_'+pos).val(session_attende_password);
	 $('#session_presenterpass_'+pos).val(session_presenter_password);
	 $('#session_attendeeurl_'+pos).val(session_attendee_url);
	 $('#session_presenterurl_'+pos).val(session_presenter_url);
	 $('#hid_session_details_id_'+pos).val(instructorid);
	 $('#session_meetingid_'+pos).val(session_meeting_id);*/

	 $('.active-qtip-div').remove();
	}catch(e){
			// To Do
		}
}

function checkedAll(multiSel,uniqueId){
	try{
		if($(multiSel).attr('checked') == true){
			$('.attach-permission-cls').each(function(){
					$(this).attr('checked', true);
					$(this).parent('td').removeClass('checkbox-unselected').addClass('checkbox-selected');
			});
		} else {
			$('.attach-permission-cls').each(function(){
				$(this).attr('checked', false);
				$(this).parent('td').removeClass('checkbox-selected').addClass('checkbox-unselected');
			});

		}
	}catch(e){
			// To Do
		}
}
function uncheckAllPermission(){
	try{
	// If all checkbox selected - multiselect-all should be checked
	var multiselectVar = true;
	var removeClass = "checkbox-unselected";
	var addClass = "checkbox-selected";
	$('.attach-permission-cls').each(function(){
		if($(this).attr('checked') == false){
			multiselectVar = false;
			removeClass = "checkbox-selected";
			addClass = "checkbox-unselected";
		}
	});
	$('#select-all-checkbox').attr('checked', multiselectVar);
	$('#select-all-checkbox').parent().removeClass(removeClass);
	$('#select-all-checkbox').parent().addClass(addClass);
	}catch(e){
			// To Do
		}
}

function scorllMainTabPrev(){
	try{
	var liCount = $('#sort-bar-V2 .AdminsublinktabNavigation li').size();
	var activeLiLast = $('#sort-bar-V2 .visible-main-tab:first').index();
	if(activeLiLast==1){
		return '';
	}
	//if(liCount>activeLiLast){
		$('#sort-bar-V2 ul li:eq('+(activeLiLast-1)+')').removeClass('hidden-main-tab');
		$('#sort-bar-V2 ul li:eq('+(activeLiLast-1)+')').addClass('visible-main-tab');
		$('#sort-bar-V2 .visible-main-tab:last').removeClass('visible-main-tab').addClass('hidden-main-tab');
	//}
	}catch(e){
			// To Do
		}
}

function scorllMainTabNext(){
	try{
	var liCount = $('#sort-bar-V2 .AdminsublinktabNavigation li').size();
	var activeLiLast = $('#sort-bar-V2 .visible-main-tab:last').index();
	if(activeLiLast==(liCount-2)){
		return '';
	}
	if(liCount>activeLiLast){
		$('#sort-bar-V2 ul li:eq('+(activeLiLast+1)+')').removeClass('hidden-main-tab');
		$('#sort-bar-V2 ul li:eq('+(activeLiLast+1)+')').addClass('visible-main-tab');
		$('#sort-bar-V2 .visible-main-tab:first').removeClass('visible-main-tab').addClass('hidden-main-tab');
	}
	}catch(e){
			// To Do
		}
}

function resetMainTab(){
	try{
	var selected = $('#sort-bar-V2 .selected').index();
	var lastLi = $('#sort-bar-V2 .visible-main-tab:last').index();
	if($('#sort-bar-V2 .selected').css('display')=='none'){
		for(var i=lastLi;i<selected;i++){
			scorllMainTabNext();
		}
	}
	}catch(e){
			// To Do
		}
}

function deleteOrderLineItem(nid,enrid){
	try{
	$("input[name=lineitem_nid]:hidden").val(nid);
	$("input[name=lineitem_enrid]:hidden").val(enrid);
	$("input[name = 'pay_lineitem_canceled']").click();
	}catch(e){
			// To Do
		}
}

function checkboxSelectedUnselectedMultiParent(id){
	try{
	if($(id).is(':checked')){
		$(id).parents().eq(1).removeClass('survey-checkbox-unselected');
		$(id).parents().eq(1).addClass('survey-checkbox-selected');
	}
	else {
		$(id).parents().eq(1).removeClass('survey-checkbox-selected');
		$(id).parents().eq(1).addClass('survey-checkbox-unselected');
	}
	}catch(e){
			// To Do
		}
}

function checkboxSelectedUnselectedAddInf(id){
	try{
	if($(id).is(':checked')){
		$(id).parents().eq(1).removeClass('checkbox-unselected');
		$(id).parents().eq(1).addClass('checkbox-selected');
	}
	else {
		$(id).parents().eq(1).removeClass('checkbox-selected');
		$(id).parents().eq(1).addClass('checkbox-unselected');
	}
	}catch(e){
			// To Do
		}
}
jQuery(function($){
	try{
	var currLang = $('html').attr('xml:lang');
	$.datepicker.regional[currLang] = {
		prevText: Drupal.t('LBL692'),
		nextText: Drupal.t('LBL693')
		};
	$.datepicker.setDefaults($.datepicker.regional[currLang]);
	}catch(e){
			// To Do
		}
});

(function($) {
	try{
	$.fn.getScrollDive = function() {
		var heigt = $("#scrolldiv").height();
		if(heigt > 80){
			$('#catalog-attachment-disp-container #scrolldiv').css('height','100px');
			$('#catalog-attachment-disp-container #scrolldiv').jScrollPane({});
			}
			vtip();
	};
	}catch(e){
			// To Do
		}
})(jQuery);

(function($) {
	try{
	$.fn.cloneScrollPopup = function(classId) {
		var heigt = $('#clonescrolldiv'+classId).height();
		if(heigt > 80){
			//$('#clonescrolldiv').css('height','100px');
			$('#clonescrolldiv'+classId).jScrollPane({});
			}
	};
	}catch(e){
			// To Do
		}
})(jQuery);

//if (Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
 jQuery("#classdaterange-daterange-from-date,#classdaterange-daterange-to-date").live("click",function() {
	 try{
	jQuery("#ui-datepicker-div").parent('div').removeClass('datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parent('div').removeClass('edit-datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parent('div').removeClass('add-datepicker-maincontainer');
	jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	jQuery("#ui-datepicker-div").parent('div').addClass('datepicker-maincontainer');
	 }catch(e){
			// To Do
		}
 });
 jQuery("#edit-reg-end-date,.addedit-edit-reg_end_date").live("click",function() {
	 try{
	 var rdoVal = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
	 var rdoVal1 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();

	 if(rdoVal =="Class")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('edit-datepicker-maincontainer');
	 }
	 else if(rdoVal1 =="Course")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-datepicker-maincontainer');
	 }
	 else
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 }
	 }catch(e){
			// To Do
		}
	});
jQuery("#start_date").live("click",function() {
	try{

	 var rdoVal2 = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
	 var rdoVal3 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();
	 //alert(rdoVal3);
	 if(rdoVal2 =="Class")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
	 $("#ui-datepicker-div").css('z-index','10000');
	 }
	 else if(rdoVal3 =="Course")
	 {
	 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
	 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
	 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
	 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
	 $("#ui-datepicker-div").css('z-index','10000');
	 }
	}catch(e){
			// To Do
		}
});
function datepickerAddEdit(){
	try{

		 var rdoVal2 = $('.narrow-search-filterset-radio-selected').find('#radio_Class').val();
		 var rdoVal3 = $('.narrow-search-filterset-radio-selected').find('#radio_Course').val();
		 if(rdoVal2 =="Class"){
			 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
			 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
			 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
			 $("#ui-datepicker-div").css('z-index','10000');
		 }
		 else if(rdoVal3 =="Course"){
			 jQuery("#ui-datepicker-div").parents('div').removeClass('datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('edit-datepicker-maincontainer');
			 jQuery("#ui-datepicker-div").parents('div').removeClass('add-sessions-date');
			 jQuery("#ui-datepicker-div").wrapAll(document.createElement('div'));
			 jQuery("#ui-datepicker-div").parent('div').addClass('add-sessions-date');
			 $("#ui-datepicker-div").css('z-index','10000');
		 }
		 $('#end_date , #start_date').bind('change',function() {
		  if($('#end_date').val()!= '' && $('#end_date').val() != undefined && $('#start_date').val() != '' && $('#start_date').val()!= undefined  && ($('#start_date').val() != 'mm-dd-yyyy' || $('#end_date').val() != 'mm-dd-yyyy')) {
			  if($('#start_date').val() == $('#end_date').val() ){
				  $(".weekday-checkbox-input").attr("checked", false);
				  $(".weekday-checkbox-input").parent().removeClass('checkbox-selected');
				  $(".weekday-checkbox-input").parent().addClass('checkbox-unselected');
				  $(".weekday-checkbox-input").attr("disabled", true);
				  $(".session_det_eachday").addClass('greyoutcheckbox');
     			  }
			  else{
				  $('.weekday-checkbox-input').removeAttr("disabled");
				  $(".session_det_eachday").removeClass('greyoutcheckbox');
			  	}
			  if($('#start_date').val()!= 'mm-dd-yyyy' && $('#end_date').val() != 'mm-dd-yyyy'){
			    var start = $('#start_date').val();
				var end = $('#end_date').val()
			    var countdays = getCountOf( start,end );
			    // console.log(countdays);
			     for (var key in countdays) {
				  if(countdays[key] ==0){
					 $("#"+key).attr("checked", false);
					  $("."+key).removeClass('checkbox-selected');
					  $("."+key).addClass('checkbox-unselected');
					  $("#"+key).attr("disabled", true);
					  $("#"+key+"-text").addClass('greyoutcheckbox');
				  	}
			     }
			}
		  }
		});	

	}catch(e){
			// To Do
	}
}


function parseDate(input) {
    var parts = input.split('-');
    return new Date(parts[2], parts[0]-1, parts[1]); 
  }
function getCountOf( date1, date2){
  var dateObj1 = parseDate(date1);
  var dateObj2 = parseDate(date2);
  var week = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
  var dayToSearch = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
  	var days_count = [];
  		for(var i = 0; i < dayToSearch.length; i++){
  				var dayIndex = week.indexOf( dayToSearch[i] );
  				var count = 0;
  					while ( dateObj1.getTime() <= dateObj2.getTime()){
  						if (dateObj1.getDay() == dayIndex )
  							{
  							   count++;
  								}       
  						dateObj1.setDate(dateObj1.getDate() + 1);
  					}
  					dateObj1 = parseDate(date1);
  					days_count[dayToSearch[i]] = count;
  		}
  return days_count;
}


//}
 $('body').live('click', function (event) {
	 try{
	 $('.add-session-popup-list').css("display","");
		if(event.target.className!='add-another-arrow-link') {
				$('.add-session-popup-list').hide();
			}
	 }catch(e){
			// To Do
		}
	});
$('.add-more-session-list li').live('click', function () {
	try{
	if(navigator.userAgent.indexOf("Chrome")>=0){
      var meetingTypeW=$('.session-add-another-popup-container').find('span.chosen-meeting-type').css("width");
   	  $(this).css('width',meetingTypeW);
	}
	}catch(e){
			// To Do
		}
 });

function updatePaginationCookie(p){
	try{
	var crPage = EXPERTUS_SMARTPORTAL_AbstractManager.getCookie('current_page');
	if(crPage!=''){
	var level = crPage.split('~');
	(p==1)?level[0] = level[0].replace('#0','#1'):level[1] = level[1].replace('#0','#1');
	var updatePage = level[0]+"~"+level[1]+"~"+level[2];
	document.cookie="current_page="+updatePage+"; path=/";
	}
	}catch(e){
			// To Do
		}
}

function manageDropdown(){
	try{
	showHideDropDown();
	var par = $('.grey-btn-bg-right');
	var position = par.position();
	$("#manage-dd-list" ).css( "left" , position.left-110);
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:first").css('padding-bottom','5px');
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:last").css('padding-top','5px');
	}catch(e){
			// To Do
		}
}
function createDropdown(){
	try{
	showHideDropDowncreate();
	var par = $('.pink-btn-bg-right');
	var position = par.position();
	$("#create-dd-list" ).css( "left" , position.left-75);
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:first").css('padding-bottom','5px');
    //$("#narrow-search-actionbar-list").find("#manage-dd-list").find("li:last").css('padding-top','5px');
	}catch(e){
			// To Do
		}
}

/* Added for Custom Attribute - Ganesh  #custom_attribute_0078975 */
function createCustomAttributeDropdown(entity_type){ 
	try{  
		var ddlist_cls='create-dd-list-for-'+entity_type;
		var ddlist = $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility'); 
		
		if(ddlist == 'hidden'){
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','visible');
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','block');
		}else{
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','hidden');
			$('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','none');
		}   
	}catch(e){
			// To Do
	}
}

//#custom_attribute_0078975
function clickCustomAttributeDropdown(attr_type,entity_type){
	try{   
		if(attr_type!=''){
		  var repl_text = '';
		  var cls_name='';
		  if(attr_type=='cattr_type_checkbox'){
		      repl_text = Drupal.t('LBL2006');
		      cls_name='new-customattribute-wrapper-checkbox';
		  }else if(attr_type=='cattr_type_dropdown'){
		      repl_text = Drupal.t('LBL2007');
		      cls_name='new-customattribute-wrapper-dropdown';
		  }else if(attr_type=='cattr_type_radio'){
		      repl_text = Drupal.t('LBL2008');
		      cls_name='new-customattribute-wrapper-radio';
		  }else if(attr_type=='cattr_type_txtarea'){
		      repl_text = Drupal.t('LBL2010');
		      cls_name='new-customattribute-wrapper-textarea';
		  }else if(attr_type=='cattr_type_txtbox'){
		     repl_text = Drupal.t('LBL2009');
		     cls_name='new-customattribute-wrapper-textbox'; 
		  }  
		  
		  if(repl_text!=''){
		    repl_text = Drupal.t('LBL287')+' '+repl_text;
		    // $('#new-customattribute-link').html(repl_text); 
		     $('.new-customattribute-link-for-'+entity_type).html(repl_text);
		     //$('.addedit-customattributes-wrapper').addClass(cls_name);
		     $('.addedit-customattributes-wrapper .addedit-form-cancel-container-actions').attr('id', cls_name);
		     var ddlist_cls='create-dd-list-for-'+entity_type;
		     $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('display','none');
		     $('.addedit-customattributes-wrapper #create-dd-list'+'.'+ddlist_cls).css('visibility','hidden');
		  } 
		 
		}  
		 
	}catch(e){
			// To Do
	}
}  

// Visibility pop up close  #custom_attribute_0078975
function closeCustomAttributeQtip(refreshOpt){ 
	try{ 
		$('.qtip-popup-visible').html('').hide();  
		if(refreshOpt=='1'){
		  $('.hidden-save-addedit-entityform').click();
		}   
	}catch(e){
		// To Do
	}
}

function orderQtipVisible(qtipObj){
	
	try{
						//alert('Visible Popup');
						var url = resource.base_host+'?q='+qtipObj.url;
						var popupId 	= qtipObj.popupDispId;
						//var catalogVisibleId = qtipObj.catalogVisibleId;
						var entId = qtipObj.entityId;
						var qtipScrollId = qtipObj.scrollid;

						$.ajax({
			 				 type: "GET",
				   	         url: url,
				   	         data:  '',
				   	         success: function(data){
			 					//var paintHtml = bpTop+"<div id='paintContent"+popupDispId+"'><div id='show_expertus_message' style='"+messageStyle+"'></div>"+data.render_content_main+"</div>"+bpBot;
			 					$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(data.render_content_main);
			 					$.extend(true, Drupal.settings, data.drupal_settings);
			 					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("paintContent"+popupId);

			 					if(qtipScrollId != '' && qtipScrollId != undefined && qtipScrollId != null){
			 						var qtipScrollType = (qtipObj.scrolltype == 'class') ? '.' : '#';
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane('destroy');
			 						$('#'+popupId+' '+qtipScrollType+qtipScrollId).jScrollPane({});
			 					}
								Drupal.attachBehaviors();
			 					//vtip();
			 					
			 					/*
			 					if('[qtipObj.linkid^=visible-sharelink]') {
			 						if (navigator.appVersion.indexOf("Safari")!= -1 && ($(window).height() < 742))
			 							$('#program-tp-basic-addedit-form-container .survey-attach-grid-wrapper .ui-jqgrid .jqgrow #bubble-face-table td.bubble-c').css('height','33px');
			 								 						
									if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["pt-pt", "de", "fr"]) != -1) {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-33px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-30px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-15px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-26px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans') {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-54px');
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-32px');
										else
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-42px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') { 
										if (navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1)
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-4px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									} else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') { 
										if ((navigator.userAgent.indexOf("MSIE") != -1 && navigator.appVersion.indexOf("10")!= -1) || (navigator.appVersion.indexOf("Trident/7.0")!= -1))
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-3px');
										else
											$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}else {
										$('#catalog-class-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-28px');
										$('#program-tp-basic-addedit-form-container #survey-sharelink-button #edit-sharelink-button').css('margin-right','-14px');
									}
								}
								*/
								
		   	                 }
			   			});

					}catch(e){
						alert(e);
					}
	
}
/* End for Custom attribute */

function showHideDropDown(){
	try{
	var ddlist = $('#manage-dd-list').css('visibility');
	if(ddlist == 'hidden'){
		$('#manage-dd-list').css('visibility','visible');
		$('#download_template').css('visibility','hidden');
                $('#download_template_enroll').css('visibility','hidden');
	}else{
		$('#manage-dd-list').css('visibility','hidden');
		$('#download_template').css('visibility','hidden');
                $('#download_template_enroll').css('visibility','hidden');
	}
	}catch(e){
			// To Do
		}
}
function showHideDropDowncreate(){
	try{
	var ddlist = $('#create-dd-list').css('visibility');
	if(ddlist == 'hidden'){
		$('#create-dd-list').css('visibility','visible');
	}else{
		$('#create-dd-list').css('visibility','hidden');
	}
	}catch(e){
			// To Do
		}
}
$('body').live('click', function (event) {
	try{
	$('#manage-dd-list').css("display","");
	if(event.target.className == 'info-user-upload vtip') {
		$('#manage-dd-list').css('visibility','visible');
		$('#manage-dd-list').css('display','block');
		$('#manage-dd-list').show();
	}else if(event.target.className == 'info-roster-upload vtip'){
                $('#manage-dd-list').css('visibility','visible');
		$('#manage-dd-list').css('display','block');
		$('#manage-dd-list').show();
	}else if(event.target.className!='grey-btn-bg-right') {
		$('#manage-dd-list').css('visibility','hidden');
		$('#manage-dd-list').hide();
	}
	$('#create-dd-list').css("display","");
	if(event.target.className!='pink-btn-bg-right' && event.target.className!='customattr-btn-bg-right'){ //Added for custom attribute by ganesh - #custom_attribute_0078975
		$("#create-dd-list" ).hide();
	}
	}catch(e){
			// To Do
		}
});


$('body').live('click', function (event) {
	try{
		classList = event.target.className.split(" ");
		if(event.target.className !='form-checkbox' && event.target.className !='tab-title' && event.target.className !='manage-clone-list' && event.target.className !='clone-label-list' && event.target.className != 'clone-list-class' && event.target.className != 'select-all-wrapper'
			&& classList[0] != ('jspDrag') && event.target.className != 'jspTrack') {
			$('.manage-clone-list').hide();
			$('.manage-clone-list').css('visibility','hidden');
		}
	}catch(e){
			// To Do
		}
});
$(".searchtext, #narrow-search-text-filter, #search_leaderboard-users_txt, #search_users_txt, #classname-autocomplete, .narrow-search-ac-text-overlap, .searchtag, .searchlocation, #tpattchedcoursename-autocomplete, #username-search-autocomplete, #surassattchedquestions-autocomplete, #countrylist-autocomplete, #enrolluser-autocomplete, #enrolltpuser-autocomplete,#prerequisite-autocomplete,#equivalence-autocomplete,#survey-autocomplete,#assessment-autocomplete,#srch_criteria_catkeyword,#report_table_autocomplete").live("click",function(){
   try{
	if(Drupal.settings.ajaxPageState.theme == 'expertusone' && ($.browser.msie && $.browser.version > 7) )
	 {
       var loaderEl = this;
       var widgetId=$(this).attr("id");
       var filterSearchClsname=$(this).attr("class");
       var filterSearchCls = filterSearchClsname.split(" ");
       $(loaderEl).val("");
       $(loaderEl).parent("div").css("position","relative");
       //reports
       $(loaderEl).closest("div.report-table-search-container").css("position","relative");
       // Top search;
	   $(loaderEl).next(".loader-hidder").remove();
	   $(".loader-hidder").remove();
	   //widget Search
	   $(loaderEl).next(".widget-loader-hidder").remove();
	   $(".widget-loader-hidder").remove();
	   //narrow filter search
	   $(loaderEl).next(".narrowfilter-loader-hidder").remove();
	   $(".narrowfilter-loader-hidder").remove();
	   //course/class search
	   $(loaderEl).next(".crecls-loader-hidder").remove();
	   $(".crecls-loader-hidder").remove();
	   //Country setting search
	   $(loaderEl).next(".countrysett-loader-hidder").remove();
	   $(".countrysett-loader-hidder").remove();
	   //select class search
	   $(loaderEl).next(".selectCls-loader-hidder").remove();
	   $(".selectCls-loader-hidder").remove();
	   //report design wizard left table search
	   $(loaderEl).next(".reportsLTbl-loader-hidder").remove();
	   $(".reportsLTbl-loader-hidder").remove();

	   $(loaderEl).after( "<div class='loader-hidder'>&nbsp;</div>" );

	   loaderEl.onkeydown = function(evt) {
           evt = evt || window.event;
          // alert("keydown: " + evt.keyCode);
          $(".loader-hidder,.widget-loader-hidder, .crecls-loader-hidder, .narrowfilter-loader-hidder, .selectCls-loader-hidder").css({"background-position":"100% -17px","cursor":"none"});
           if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
           $(".countrysett-loader-hidder").css({"background-position":"99% -16px","cursor":"none"});
           }
           if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
           	$(".narrowfilter-loader-hidder").css({"background-position":"100% -17px","cursor":"none"});
           }
           };
       loaderEl.onkeyup = function(evt) {
           evt = evt || window.event;
           // alert("keyup: " + evt.keyCode);
           $(".loader-hidder, .widget-loader-hidder, .crecls-loader-hidder, .narrowfilter-loader-hidder, .selectCls-loader-hidder").css("background-position","100% 3px");
           if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
             $(".countrysett-loader-hidder").css({"background-position":"99% 5px","cursor":"none"});
           }
           if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
        	 $(".narrowfilter-loader-hidder").css({"background-position":"100% 3px","cursor":"none"});
           }
          };

          if(widgetId=="countrylist-autocomplete" || widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete" || widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
        	//$(".loader-hidder").css({"right":"22px","top":"1px","height":"22px"});
        	$(".loader-hidder").addClass("countrysett-loader-hidder");
      	    $(".countrysett-loader-hidder").removeClass("loader-hidder");
      	    if(widgetId=="enrolluser-autocomplete" || widgetId=="enrolltpuser-autocomplete"){
      	      $(".countrysett-loader-hidder").css({"right":"1px"}) ;
      	    }
      	    if(widgetId=="prerequisite-autocomplete" || widgetId=="equivalence-autocomplete" || widgetId=="survey-autocomplete" || widgetId=="assessment-autocomplete"){
      	      $(".countrysett-loader-hidder").css({"right":"1px"}) ;
      	    }
          }
          if(widgetId=="search_leaderboard-users_txt" || widgetId=="search_users_txt"){
        	  $(".loader-hidder").addClass("widget-loader-hidder");
        	  $(".widget-loader-hidder").removeClass("loader-hidder");
          }

          if(widgetId=="classname-autocomplete" || widgetId=="tpattchedcoursename-autocomplete" || widgetId=="username-search-autocomplete" || widgetId=="surassattchedquestions-autocomplete" ){
        	  $(".loader-hidder").addClass("crecls-loader-hidder");
        	  $(".crecls-loader-hidder").removeClass("loader-hidder");
        	  if(widgetId=="tpattchedcoursename-autocomplete"){
        	  $(".crecls-loader-hidder").css({"right":"1px","top":"-10px"});
        	  }
        	  if(widgetId=="surassattchedquestions-autocomplete"){
              $(".crecls-loader-hidder").css({"right":"56px","top":"-10px"});
              }
        	 /* if(widgetId=="username-search-autocomplete") {
        	  $(".crecls-loader-hidder").css("right","108px");
        	  }*/
        	  if(widgetId=="classname-autocomplete") {
              $(".crecls-loader-hidder").css("right","23px");
              }
          }
         if(widgetId=="srch_criteria_catkeyword"){
          $(".loader-hidder").addClass("selectCls-loader-hidder");
       	  $(".selectCls-loader-hidder").removeClass("loader-hidder");
         }
        if(widgetId=="report_table_autocomplete"){
             $(".loader-hidder").addClass("reportsLTbl-loader-hidder");
          	 $(".reportsLTbl-loader-hidder").removeClass("loader-hidder");
            }

          if(filterSearchCls[1]=="narrow-search-ac-text-overlap" || filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation" ){
        	 $(".loader-hidder").addClass("narrowfilter-loader-hidder");
        	 $(".narrowfilter-loader-hidder").removeClass("loader-hidder");
        	 if(filterSearchCls[1]=="searchtag" || filterSearchCls[1]=="searchlocation"){
        	 $(".narrowfilter-loader-hidder").css({"right":"21px","height":"19px"});
        	 }
          }
        }
   }catch(e){
			// To Do
		}
});
Drupal.ajax.prototype.commands.saveandclosePopup = function(ajax, response, status) {
	   try {
		   $(".active-qtip-div").remove();
		   $("#popup_container_qtip_buisness_disp_"+response.entities).closest(".qtip-active").remove();
		   $("#qtipAccessqtip_visible_disp_"+response.entities).closest(".qtip-active").remove();
	   }
	   catch(e) {
		   //nothing to do
	   }
};
Drupal.ajax.prototype.commands.displaymessagewizard = function(ajax, response, status) {
    try {
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	var dupId = response.grpId;
	    $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

    		var title = Drupal.t('MSG711');

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){  $('input[id="show_catag"]').attr('value',0); $("input[name = 'saveandshow']").click();$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'catalog-publish-unpublis-dialog',
	    			'esignFor':'displaymessagewizard','catalogId':dupId,'catalogType': 'Class','actionStatus': 'lrn_cls_sts_atv', 'rowObj': this};
	    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);$(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
         }else{
        	 
        	 closeButt[Drupal.t('Yes')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
	        	 setTimeout(function() {
	     			$('#visible-class-'+dupId).click()
	     		}, 300);
        	 };
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL749'),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        zIndex : 10005,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $(".removebutton").text(Drupal.t("No"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 /*$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
    }
    catch(e){
      //Nothing to do
    }
  };


function orderEdit(){
	 try {
	  setTimeout(function() {
		var orderRow=$(".payment-billing-details-info .order-review-table tr,.payment-user-details-info .order-review-table tr").length;
		for(var i=0; i < orderRow; i++){
			var billH = $(".payment-billing-details-info .order-review-table tr:eq("+i+")").height();
			var userH = $(".payment-user-details-info .order-review-table tr:eq("+i+")").height();
			var maxH = (billH < userH) ? userH : billH;
			$(".payment-billing-details-info .order-review-table tr:eq("+i+"), .payment-user-details-info .order-review-table tr:eq("+i+")").height(maxH);
		}
	  }, 1000);
	  vtip();
	}catch(e) {
		   //nothing to do
	   }
}

function checkPaginationWidth(){
	try {
	if($("#admin-data-grid .ui-pg-table .ui-pg-table").is(":visible")){
	  var pageConWidth=$("#admin-data-grid .ui-pg-table .ui-pg-table").width();
	   $("#exportcontainer").css("right",pageConWidth+15);
	   if(document.getElementById('enrollment-upload-container') !== undefined) {
		if(Drupal.settings.ajaxPageState.theme != "expertusoneV2" && pageConWidth > 0) {
			$('.admin_ac_input_mainform.addedit-edit-enrolluser-autocomplete').width(195);
	 }
		$("#enrollment-upload-container").css("right",pageConWidth+15);
	   }
	 }
	}catch(e) {
		   //nothing to do
	   }
	};

(function($) {
	$.fn.cloneDestroyloader = function() {
		try {
			$("#root-admin").data("narrowsearch").destroyLoader('paint-narrow-search-results');
		} catch (e) {
			// To Do
		}
	};
})(jQuery);


Drupal.ajax.prototype.commands.showClonePopup = function(ajax, response, status) {

    try {
    	classId = response.class_id;
    	$('#manage-clone-list'+classId).html(response.html);

    	showHideCreateCopyDropDown(classId);
    	var par = $('.grey-btn-bg-right-create-copy');
    	var position = par.position();
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	if(this.currTheme == "expertusoneV2"){
    	$("#ul-clone-list-"+classId ).css( "right" ,'-25px');
    	}else{
    		$("#ul-clone-list-"+classId ).css( "right" ,'-25px');
    		$("#ul-clone-list-"+classId ).css( "top" ,'30px');
    	}
		    	
    	$(".contentauthorclone").parent().parent().parent().parent().css("width","180px");
		$(".contentauthorclone .jspContainer").css("width","180px");
		$(".contentauthorclone .jspContainer .jspPane").css("width","170px;");
		$(".contentauthorclone .jspContainer .jspPane").css("height","66px;");
		$(".contentauthorclone").closest(".manage-clone-list").css("width","170px;");
		$(". contentauthorclone").css("width","200px");


		
    	Drupal.attachBehaviors();
    	vtip();
    }
    catch(e){
      //Nothing to do
    }
};

function showHideCreateCopyDropDown(entityId){
	try{
		$('.manage-clone-list').hide();
		// Qtippopup Disable
		$('#group-control').remove();
		$('.qtip-popup-visible').hide();

		var ddlist = $('#manage-clone-list' + entityId).css('visibility');
		if (ddlist == 'hidden') {
			$('#ul-clone-list-' + entityId).css('visibility', 'visible');
			$('#ul-clone-list-' + entityId).show();
		} else {
			$('#ul-clone-list-' + entityId).css('visibility', 'hidden');
		}
	}catch(e){
			// To Do
		}
}

function checkboxDisableForCloneCA(obj)
{
	$('#clone-Content').prop('checked','checked');
	$('#clone-Content').parent().removeClass('checkbox-unselected');
	$('#clone-Content').parent().addClass('checkbox-selected');

	if($(obj).attr("id") == "clone-Interactions" && $(obj).is(":checked") == true)
	{
		$('#clone-Interactions').prop('checked','checked');
		$('#clone-Interactions').parent().removeClass('checkbox-unselected');
		$('#clone-Interactions').parent().addClass('checkbox-selected');
		
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-Interactions" && $(obj).is(":checked") == false)
	{
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').prop('checked','');
	}
	
	
	if($(obj).attr("id") == "clone-access" && $(obj).is(":checked") == true)
	{
		$('#clone-access').prop('checked','checked');
		$('#clone-access').parent().removeClass('checkbox-unselected');
		$('#clone-access').parent().addClass('checkbox-selected');
		
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-access" && $(obj).is(":checked") == false)
	{
		$('#clone-access').parent().removeClass('checkbox-selected');
		$('#clone-access').parent().addClass('checkbox-unselected');
		$('#clone-access').prop('checked','');
	}
	
	/*if($(obj).attr("id") == "clone-Content" && $(obj).is(":checked") == true)
	{
		$('#clone-Content').prop('checked','checked');
		$('#clone-Content').parent().removeClass('checkbox-unselected');
		$('#clone-Content').parent().addClass('checkbox-selected');
	}else if($(obj).attr("id") == "clone-Content" && $(obj).is(":checked") == false)
	{
		$('#clone-Content').prop('checked','');
		$('#clone-Content').parent().removeClass('checkbox-selected');
		$('#clone-Content').parent().addClass('checkbox-unselected');
		
		$('#clone-Interactions').prop('checked','');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		
		
		
	}*/
	
	//{
	
	//}
/*	if($('#clone-Content').is(':checked')){
		$('#clone-Interactions').attr('disabled','');
		//$('#clone-Interactions').parent().addClass('checkbox-selected');
		//$('#clone-Interactions').parent().removeClass('checkbox-unselected');
		
	}
	else
	{
		//alert($('#clone-Interactions').size());
		
		$('#clone-Interactions').attr('checked','');
		$('#clone-Interactions').parent().addClass('checkbox-unselected');
		$('#clone-Interactions').parent().removeClass('checkbox-selected');
		$('#clone-Interactions').attr('disabled','disabled');
		
		
		
	}*/
	
	$(".jspPane").css("left","0px");
}

function checkboxDisableForClone(type,id){
	if(type == 'class'){
	if(!$('#clone-content_list').is(':checked')){
		$('#clone-enrollment').parent().removeClass('checkbox-selected');
		$('#clone-enrollment').parent().addClass('checkbox-unselected');
		$('#clone-enrollment').attr('checked','');
	}
	}else{
		if(!$('#ul-clone-list-'+id+' #clone-attachedcourses').is(':checked')){
			$('#ul-clone-list-'+id+' #clone-assessment').parent().removeClass('checkbox-selected');
			$('#ul-clone-list-'+id+' #clone-assessment').parent().addClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-assessment').attr('checked','');
			
			$('#ul-clone-list-'+id+' #clone-survey').parent().removeClass('checkbox-selected');
			$('#ul-clone-list-'+id+' #clone-survey').parent().addClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-survey').attr('checked','');
		}
	}
}
function checkboxAutoEchckForContentClone(type,id){
	if(type == 'class'){
	if($('#clone-enrollment').is(':checked')){
		$('#clone-content_list').attr('checked','checked');
		$('#clone-content_list').parent().removeClass('checkbox-unselected');
		$('#clone-content_list').parent().addClass('checkbox-selected');
	}
	}else{
		if($('#ul-clone-list-'+id+' #clone-assessment').is(':checked') || $('#ul-clone-list-'+id+' #clone-survey').is(':checked')){
			$('#ul-clone-list-'+id+' #clone-attachedcourses').attr('checked','checked');
			$('#ul-clone-list-'+id+' #clone-attachedcourses').parent().removeClass('checkbox-unselected');
			$('#ul-clone-list-'+id+' #clone-attachedcourses').parent().addClass('checkbox-selected');
}
	}
}

//Added by ganesh for custom attribute #custom_attribute_0078975
Drupal.ajax.prototype.commands.displayScreenWarningWizard = function(ajax, response, status) {
    try { 
        
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	//var dupId = response.grpId; 
    	var name =  unescape(response.name).replace(/"/g, '&quot;');   
    	
		$('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    	var title = name;

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
 
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
              closeButt[Drupal.t('Yes')]=function(){
            	  var esignObj = {'popupDiv':'displayScreenWarningWizard','esignFor':'addScreenDetails'};
	                      $.fn.getNewEsignPopup(esignObj); $(this).dialog('destroy');$(this).dialog('close');};
         }else{
        	 closeButt[Drupal.t('Yes')]=function(){  
        	   
        	   $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();
     		   $("#delete-msg-wizard").remove();  
			   $("#hidden-screen-button").click(); 
        	};
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL2015"),//"title",
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        zIndex : 10005,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			/* $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    } 
			    
	      
	     
	   
    }
    catch(e){
      //Nothing to do
    }
  };

Drupal.ajax.prototype.commands.displayWarningWizard = function(ajax, response, status) {
    try {
    	this.currTheme = Drupal.settings.ajaxPageState.theme;
    	var dupId = response.grpId;
    	var name =  unescape(response.name).replace(/"/g, '&quot;');

	    $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    	var title = name;

	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

         if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
              closeButt[Drupal.t('Yes')]=function(){
            	  var esignObj = {'popupDiv':'displayWarningWizard','esignFor':'displayWarningWizard', 'dupId':dupId, 'rowObj': this};
	                      $.fn.getNewEsignPopup(esignObj); $(this).dialog('destroy');$(this).dialog('close');};
         }else{
        	 closeButt[Drupal.t('Yes')]=function(){ callInsertProcess(dupId);};
         }

	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("Group"),//"title",
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        zIndex : 10005,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			/* $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');*/
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
    }
    catch(e){
      //Nothing to do
    }
  };
  function callInsertProcess(dupId,esign){
	  esign = typeof esign !== 'undefined' ? esign : '';
	  $('input[id="grp_war"]').attr('value',1);
	  $('input[id="grp_war_grpId"]').attr('value',dupId);
	  if(esign == 'esign'){
		  $("#esign-role-addedit-form").click();
	  }else{
		  $("input[name = 'grp-save']").click();
	  }
	  $('#delete-msg-wizard').remove();
	}

function callVisibility(qtipObj){
	
	if(typeof(qtipObj.dispGrpView) == 'undefined') qtipObj.dispGrpView='';
	if(typeof(qtipObj.catalogVisibleId) == 'undefined') qtipObj.catalogVisibleId='';
	
	if(document.getElementsByClassName('qtip-close-button') && qtipObj.catalogVisibleId.indexOf("AttachIdqtip_add_grp") <= 0){
	$('.qtip-close-button').click();
}	
	//console.log(paginationEnter);
	if(paginationEnter)
		return false;
	// Clone Onclick Disable
	var ddlist = $('#manage-clone-list' + qtipObj.entityId).css('visibility');
	if (ddlist == 'hidden') {
		$('#ul-clone-list-' + qtipObj.entityId).css('visibility', 'visible');
		$('#ul-clone-list-' + qtipObj.entityId).show();
	} else {
		$('#ul-clone-list-' + qtipObj.entityId).css('visibility', 'hidden');
	}
	$('.manage-clone-list').hide();
	
	if(qtipObj.dispGrpView == 'Y'){ // Added for this ticket #0042617 to show the Hidden tab

		var lId = $('#viewgroup-detail-wrapper #view-scroll-wrapper');
		var mypos = $('#' + qtipObj.popupDispId); // Declaring ID
		 // Popup Calculation
		var lheg = lId.height();
		var popoff = lId.offset();
		 // Action link Calculation
		var newlheg = mypos.height();
		var newpopoff = mypos.offset();

		var gridHeight = (lheg/2) + popoff.top;
		var linkHeight = newlheg + newpopoff.top;

		if(gridHeight < linkHeight){
			qtipObj.dispDown = '';
			if((linkHeight - gridHeight) > 100){
				lId.css("overflow","hidden");
				$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","hidden");
			}else{
				lId.css("overflow","visible");
				$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","visible");
			}
		}else{
			lId.css("overflow","visible");
			$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","visible");
		}
	}else {
		$('#viewgroup-detail-wrapper #view-scroll-wrapper').css("overflow","hidden");
		$('#viewgroup-detail-wrapper #view-scroll-wrapper .jspContainer').css("overflow","hidden");
	}
	$('#group-control').remove();
	//console.log(qtipObj.catalogVisibleId);
	//console.log(qtipObj.catalogVisibleId.indexOf("AttachCrsIdqtip_visible_disp"));
	if(qtipObj.catalogVisibleId.indexOf("AttachCrsIdqtip_visible_disp") > 0 || qtipObj.catalogVisibleId.indexOf("AttachIdqtip_add_grp") > 0 ){
		var str = $('#program_attach_tabs .ui-state-active').attr('id');
		var currmod = str.split('-');
		//console.log(" string " + str);
		qtipObj.url = qtipObj.url + '/' + currmod[2] + '/' + $('#attach_course_dt form input[name="form_build_id"]').val(); 
	}
	if(qtipObj.catalogVisibleId.indexOf("AttachIdqtip_history") <= 0){
	$('.qtip-popup-visible').html('').hide();
	}else{
		$('.qtip-popup-history').html('').hide();
	}
	$('#' + qtipObj.popupDispId).qtipPopup({
		wid : qtipObj.wid,
		heg : qtipObj.heg,
		entityId : qtipObj.entityId,
		popupDispId : qtipObj.popupDispId,
		postype : qtipObj.postype,
		poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
		posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
		disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
		linkid	: qtipObj.linkid,
		onClsFn	: qtipObj.onClsFn,
		dispDown : (qtipObj.dispDown == undefined) ? '': qtipObj.dispDown,
	});
	setTimeout(function(){
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("paintContent"+ qtipObj.popupDispId);
		if('[qtipObj.linkid^=visible-sharelink]') {
			$('#sharelink_disp_div .survey-sharelink-main-wrapper .loadercontent .loaderimg').css('margin-top','-42px');
			$('.qtip_survey_sharelink .loadercontent .loaderimg').css('margin-top','-8px');
		}
	},10);
	//#custom_attribute_0078975
	if(qtipObj.entityType=='order'){ //For custom attribute order
		orderQtipVisible(qtipObj); // For Order	
	}else{
	$("#root-admin").data("narrowsearch").visiblePopup(qtipObj);
}
	
	 
}
// Visibility pop up close
function closeQtip(popupId,entId,onClsFn){
	try{
	
	   //#custom_attribute_0078975 Refresh the association when click on close button from show-in-screen Qtip
	   if(popupId.indexOf('qtip_showinscreen_dis')>=0){ 
	     $('#root-admin').data('narrowsearch').refreshLastAccessedRow();
	   }
	   
		if(typeof(onClsFn) == 'undefined') onClsFn='';
		if(popupId == '')
			$('.qtip-popup-visible').html('').hide();
		else
			$('#'+popupId+' #visible-popup-'+entId).html('').hide();

		if(typeof(onClsFn) == 'function') {
			onClsFn();
		}
	}catch(e){

	}
}
function checkCountryDisable(){
  if($('#add_grp_Country').is(':checked')){
	 $('#add_grp_State').attr('disabled', false);
  }else {
	 $('#add_grp_State').parent().removeClass('checkbox-selected');
	 $('#add_grp_State').parent().addClass('checkbox-unselected');
	 $('#add_grp_State').attr('checked','');
	 $('#add_grp_State').attr('disabled',true);
  }
}
function stateDisabledMessage(){
 if(!($('#add_grp_Country').is(':checked'))){
	 var error = new Array();
	 error[0] = Drupal.t('select a country');
	 var message_call = expertus_error_message(error,'error');
	 $('#show_expertus_message').show();
	 $('#show_expertus_message').html(message_call);
  }
}

function addslashes(string) {
    return string.replace(/'/g, '\\\'');
}

function removeCurrencyPopup(codeval,symbol){
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	var uniqueClassPopup = '';
	 $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    if(this.currTheme == 'expertusoneV2'){
	   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("MSG747")+' "'+codeval +' <span class="currency-override-bold">'+ symbol+'</span>"?'+'</td></tr>';
	    } else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("MSG747")+' "'+ codeval +' <span class="currency-override-bold">'+ symbol +'</span>"?'+'</td></tr>';
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

		 closeButt[Drupal.t('Yes')]=function(){
			 $("#root-admin").data("narrowsearch").createLoader("currency-detail-wrapper");
			 removeaddedcurrency(codeval);
		 };
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL749'),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        zIndex : 10005,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog').wrap("<div id='catalog-publish-unpublis-dialog' class='"+uniqueClassPopup+"'></div>");
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $(".removebutton").text(Drupal.t("No"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		$('.admin-save-button-middle-bg').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
}

function removeaddedcurrency(val) {
      $.ajax({
       	type: "GET",
       	url: "/?q=administration/sitesetup/config/currency/remove/"+val,
       	success: function(data) {
       			if(data=='success'){
       				$('.row-'+val).remove();
       				$(".currency-table-heading tbody tr").removeClass( "odd-list-class" );
       				$(".currency-table-heading tbody tr").removeClass( "even-list-class" );
       				$(".currency-table-heading tbody tr.rowtbody:nth-child(odd)").addClass("even-list-class");
       			    $(".currency-table-heading tbody tr.rowtbody:nth-child(even)").addClass("odd-list-class");
       			    dynamicWidthHeight('delete');
       			    $("#root-admin").data("narrowsearch").destroyLoader("currency-detail-wrapper");
       			}
       	}
       	});
}
function checkboxSelectedUnselectedCurrency(id){
	try{
		if($(id).attr('type')!='radio'){
			if($(id).is(':checked')){
				$(id).parent().removeClass('checkbox-unselected');
				$(id).parent().addClass('checkbox-selected');
			}
			else {
				$(id).parent().removeClass('checkbox-selected');
				$(id).parent().addClass('checkbox-unselected');
			}
			
		}
		var totalcheckbox = $('#currency-td-table .multichk').length;
    	var cheklen = $('#currency-td-table .checkbox-selected').length;
    	if(totalcheckbox==cheklen){
    		$('#currency_select').attr('checked', true);
    		$('.currency-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
    	} else {
    		$('#currency_select').attr('checked', false);
    		$('.currency-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
    	}	
	}catch(e){
		// to do
	}
}
function Currencycheckedall(){
	try{
		    $("#currency_select").change(function(){
		    	$(".attach-group-list").prop('checked', $(this).prop("checked"));			    	
		    	if($('#currency_select').attr('checked')) {
		    		$(this).attr('checked', true);
		    		$('.currency-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    	} else{
		    		$(this).attr('checked', false);
		    		$('.currency-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    		$('.multichk').removeClass('checkbox-selected').addClass('checkbox-unselected');		    		
		    	}	
		    	return false;
		    });
	}

	catch(e){

	}
}
function dynamicWidthHeight(callfrom){
	  var scrol_height = '';
	  var table_height = $('#multi_currency_setup_div .currency-table-heading').height();
	  var table_width = $('#multi_currency_setup_div .currency-table-heading').width();
	  //console.log(table_height);
	  //console.log(table_width+'here');
	  if(table_height > 310 && table_width > 800){
		  var new_width = 825;
		  var new_height = 378;
		  $('#modal-content').css({"width":new_width,'height':new_height});
		  $('#tableholder').css({'height': '307px', 'width': '790px'});
		  $('#multi_currency_setup_div').css({'height': '307px', 'width': '790px', 'overflow': 'hidden'});
		  $("#multi_currency_setup_div").jScrollPane();
	  }else if(table_height <150 && table_width <165){
		  $('#modal-content').css({'height': '150px', 'width': '295px'});
		  $('.addedit-form-cancel-and-save-actions-row').css({'width':'90%','margin-left': '40px'});
		  $('#tableholder').css({'height': '60px', 'width': '240px'});
		  $('#multi_currency_setup_div').css({'height': '60px', 'width': '240px'});
		  $('.currency-table-heading').css({'width': '245px'});
		  if($('.rowtbody').length<=0){
			  $('.currency-table-heading').remove();
			  $('#multi_currency_setup_div').append("<div width=\"130px\" height=\"100px\" class=\"currency-table-heading admin-empty-text-msg\" style=\"border:0px;\">"+Drupal.t('MSG746')+"</div>" );
		  }

	  } else{
		  if($('.rowtbody').length<=0){
			  $('.currency-table-heading').remove();
			  $('#multi_currency_setup_div').append("<div width=\"130px\" height=\"100px\" class=\"currency-table-heading admin-empty-text-msg\" style=\"border:0px;\">"+Drupal.t('MSG746')+"</div>" );
		  }
		  //console.log('here');
		  var new_width = table_width +50;
		  var new_height = table_height+72;
		  table_width = table_width +15;
		  $('#modal-content').css({"width":new_width,'height':new_height});
		  $('#tableholder').css({'height': table_height+'px', 'width': table_width+'px'});
		  $('#multi_currency_setup_div').css({'height': table_height+'px', 'width': table_width+'px'});
		  var element = $('#multi_currency_setup_div').jScrollPane({});
		  var api = element.data('jsp');
		  api.destroy();
	  }
	  $('#modalContent').removeAttr( 'style' );
	  $('#modalContent').css({"z-index":"1001"});
	  $('#modalContent').css("position","absolute");
	  $('#modalContent').css("top", Math.max(0, (($(window).height() - $('#tableholder').outerHeight()) / 2) +  $(window).scrollTop()) + "px");
	  //console.log(Math.max(0, (($(window).width() - $('#tableholder').outerWidth()) / 2) +  $(window).scrollLeft()) + "px");
	  $('#modalContent').css("left", Math.max(0, (($(window).width() - $('#tableholder').outerWidth()) / 2) +  $(window).scrollLeft()) + "px");
}

(function($) {
	$.fn.dynamicWidthHeightAdd = function(){
	try{
		dynamicWidthHeight(false);
	}catch(e){
			// To Do
		}
	};

})(jQuery);


function dynamicstylechange(callfrom){
	var rowCount = $('.sf-table-heading tr').length;
	var consumer_sec_key = $( "input[name='consumer_secret_key']" ).val();
	var consumer_key = $( "input[name='consumer_key']" ).val();
	var sf_id = $( "input[name='expertusone_sf_id']" ).val();
	if(consumer_sec_key == ''){
		$( "input[name='consumer_secret_key']" ).addClass( "sfrow-error-check" );
	} if(consumer_key == ''){
		$( "input[name='consumer_key']" ).addClass( "sfrow-error-check" );
	} if(sf_id == ''){
		$( "input[name='expertusone_sf_id']" ).addClass( "sfrow-error-check" );
	}
	for(var i=0;i<rowCount-1;i++){
		//alert('#system_user_'+i);
		var system_user = $('#system_user_'+i).val();
		var system_pasword = $('#system_user_pwd_'+i).val();
		if(system_user == ''){
			$('#system_user_'+i).addClass( "sf-table-heading check-attributes > input sfrow-error-check" );
		}
		if(system_pasword == ''){
			$('#system_user_pwd_'+i).addClass( "sf-table-heading check-attributes > input sfrow-error-check" );
		}
	}
}

(function($) {
	$.fn.dynamicstylechangeAdd = function(){
	try{
		dynamicstylechange(false);
	}catch(e){
			// To Do
		}
	};

})(jQuery);

//0053694: SF-Survey | Survey code fadeout
(function($) {
	
	$.fn.callVtip = function() {
		try{
			vtip();
		}catch(e){
  			// To Do
  		}
	};
	/*Viswanathan added for #0074786*/
	$('.fivestar-widget a').live('click', function(e) {
		e.preventDefault();
   });
   // calendar icon - right click disable
   $('#calendar-view-icon').live('contextmenu', function() {
	   return false;
   });
   $('#calendar-view-icon').live('click', function(e) {
	   e.preventDefault();
      	drawCalendar();
   });
})(jQuery);

/* //0072940: Zoho# 18142- Instructor search box is not working
(function($) {
	$('#multiautocomplete').live("focus",  function(e) {
	    $(this).select();
    });	
})(jQuery);
 */
function changeDiscountType(){
	 try{
		 if($('#two-col-row-discount_type_row').find('.addedit-edit-dis_type option:selected').val() == 3){
			 $('#currency-list-discount').addClass('discount_currency_list_visible');
			 $('#currency-list-discount').removeClass('discount_currency_list_invisible');
			 $('.addedit-edit-dis_typeval').addClass('fixed_dusccount_type_applied');
		 } else{
			 $('#currency-list-discount').removeClass('discount_currency_list_visible');
			 $('#currency-list-discount').addClass('discount_currency_list_invisible');
			 $('.addedit-edit-dis_typeval').removeClass('fixed_dusccount_type_applied');
			 $('.discount_currency_list').val('');
		 }
	 }catch(e){
		 // to do
	 }
}
function drawCalendar()
{
	document.cookie="uri="+window.location.search;
	window.location.href="?q=admincalendar";
}
//Added by vetrivel.P for #0070900
function readCookie(name) {
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}
function urldecode(url) {
	  return decodeURIComponent(url.replace(/\+/g, ' '));
}
function eraseCookie(name) {
	createCookie(name,"",-1);
}
function createCookie(name,value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function entityvalidation(entity_type){
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	obj.createLoader('tab-content-main');
    document.getElementById(entity_type).click();
}

function downloadLink(btn_type){
    if(btn_type == 'user_upload_done_btn'){
	$('#download_template').css('visibility','visible');
	$('#download_template').css('display','block');
    }else if(btn_type == 'enrollment_upload_file'){
            $('#download_template_enroll').css('visibility','visible');
            $('#download_template_enroll').css('display','block');
   }else if(btn_type == 'enrollment_upload_class'){
            $('#download_template_class').css('visibility','visible');
            $('#download_template_class').css('display','block');

        }else{
            //do nothing
        }
}

function downloadSample(btn_type){
	
	var url = resource.base_host;
        if(btn_type == 'user_upload_done_btn'){
	var file = url+"/User_feed.csv";
        }else if(btn_type == 'enrollment_upload_file'){
            var file = url+"/Enrollment_feed.csv";
        }else{
            var file = url; // nothing to do here .. for future use
        }
	window.open(file);
	$('#manage-dd-list').css('visibility','hidden');
	$('#manage-dd-list').css('display','none');
	$('#manage-dd-list').hide();
	if(btn_type == 'user_upload_done_btn'){
	$('#download_template').css('visibility','hidden');
	$('#download_template').css('display','none');
	$('#download_template').hide();
        }else if(btn_type == 'enrollment_upload_file'){
            $('#download_template_enroll').css('visibility','hidden');
            $('#download_template_enroll').css('display','none');
            $('#download_template_enroll').hide();
        }else{
            // do nothing
        }
}
//#custom_attribute_0078975
function MoveTabPrev(itemcount) {
	//console.log('MoveTabPrev: '+itemcount);
	var div_width = $('#carousel_inner').width();
	var ul_width = $('#carousel_inner ul').width();
	var increment = '';
	var item_width = '';
	var left_indent = '';
	var current_position = '';
	var isChromium = window.chrome;
	$('#carousel_inner').css('width','auto');
	$('#carousel_inner ul').css('width','auto');
	var ul_actualwidth = $('#carousel_inner ul').width();
	$('#carousel_inner').css('width',div_width);
	$('#carousel_inner ul').css('width',ul_width);
	var lastli_width = parseInt($('#carousel_container #carousel_inner ul li:last').css('width'));
	// console.log('last li width: '+lastli_width);
	
	if(itemcount === 7) {
	
	if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'de'){
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 22;
		else
			increment = 27;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 93;
		else
			increment = 101;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt') {
		if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
			increment = 164;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 156;
		else
			increment = 152;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru') {
		if(isChromium)
		{
		$('#carousel_container #carousel_inner').css('width', '410px');
		increment = 42;
		}
		else if(navigator.appVersion.indexOf("Safari")!= -1)
		{
		increment = 44;
		}
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 39;
		else
			increment = 32;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 99;
		else 
			increment = 104;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'fr') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			increment = 23;
			$('#carousel_container #carousel_inner').css('width', '449px');
			}
		else
			increment = 30;
	}
	else
		increment = 30;
	
	if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') {
		if(navigator.userAgent.indexOf("Firefox")!= -1)
			{
			item_width = (ul_actualwidth - div_width) + 3;
		 console.log('item_width1: '+item_width+'  '+'ul_actualwidth1:'+ul_actualwidth+'  '+'div_width1:'+div_width+'  '+'ul_width1:'+ul_width);
			}
		else
			{
			//alert('hev'+navigator.userAgent.indexOf("chrome")+'navigator.userAgent'+navigator.userAgent);

			item_width = (ul_actualwidth - div_width) + 7;
		// console.log('navigator.userAgent.indexOf'+navigator.userAgent+'  '+'item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width);
			}

	}
	else
		{
		item_width = (ul_width - div_width) - increment;
	
	
	 //console.log('item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width+' '+'increment'+increment);
		}
	var left_indent = parseInt($('#carousel_inner ul').css('left')) + item_width;
	var current_position = parseInt($('#carousel_inner ul').css('left'));
	$('#carousel_container .first a').css('margin-right','0');
	if(current_position <= 0) {
		$('#carousel_container .first').css('display', 'none');
		$('#carousel_container').css('padding-left', '9px');
		$('#carousel_container .last').css('display', 'block');
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '377px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '383px');
			else
				$('#carousel_container #carousel_inner').css('width', '373px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '428px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '433px');
			else
				$('#carousel_container #carousel_inner').css('width', '428px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '400px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '405px');
			else
				$('#carousel_container #carousel_inner').css('width', '410px');
		}
	}
	
	}
	
	if(itemcount === 6) {
		// console.log('Hi dude came into movetabprev 6');
		
		//item_width = (navigator.appVersion.indexOf("Trident/7.0")!= -1) ? (parseInt((ul_width - div_width) / 2) + 6) : parseInt((ul_width - div_width) / 2);
				
		var width_calc = (ul_actualwidth - lastli_width) - 3;
		$('#carousel_container #carousel_inner').css('width', width_calc);
		
		left_indent = parseInt($('#carousel_inner ul').css('left')) - parseInt($('#carousel_inner ul').css('left'));
		current_position = parseInt($('#carousel_inner ul').css('left'));
		
		if(current_position <= 0) {
			$('#carousel_container .first').css('display', 'none');
			$('#carousel_container').css('padding-left', '9px');
			$('#carousel_container .last').css('display', 'block');
		}
	}
	
	$('#carousel_inner ul').animate({'left' : left_indent},{queue:true, duration:500},function(){  
    });
}
function MoveTabNext(itemcount) {
	 //console.log('MoveTabNext: '+itemcount);
	var div_width = $('#carousel_inner').width();
	var ul_width = $('#carousel_inner ul').width();
	var increment = '';
	var item_width = '';
	var left_indent = '';
	var current_position = '';
	var isChromium = window.chrome;
	$('#carousel_inner').css('width','auto');
	$('#carousel_inner ul').css('width','auto');
	var ul_actualwidth = $('#carousel_inner ul').width();
	$('#carousel_inner').css('width',div_width);
	$('#carousel_inner ul').css('width',ul_width);
	
	if(itemcount === 7) {
	
	if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'de'){
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 17;
			increment = 22;
		else
			increment = 27;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 123;
			increment = 128;
		else
			increment = 129;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt') {
		if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
			increment = 228;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			//increment = 223;
			increment = 219;
		else
			increment = 217;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru'){
		//if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
		if(isChromium)
			{
			$('#carousel_container #carousel_inner').css('width', '410px');
			increment = 42;
			}
		else if(navigator.appVersion.indexOf("Safari")!= -1)
			increment = 44;
		else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			increment = 39;
		else
			increment = 32;
	} 
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			//increment = 99;
			increment = 102;
			$('#carousel_container #carousel_inner').css('width', '449px');
			}
		else 
			increment = 104;
	}
	else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'fr') {
		if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
			{
			//increment = 23;
			increment = 26;
		$('#carousel_container #carousel_inner').css('width', '449px');
			}

		else
			increment = 30;
	}
	else
		increment = 30;
	
	if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us') {
		if(navigator.userAgent.indexOf("Firefox")!= -1)
			{
			item_width = (ul_actualwidth - div_width) - 28;
			 //console.log('item_width1: '+item_width+'  '+'ul_actualwidth1:'+ul_actualwidth+'  '+'div_width1:'+div_width+'  '+'ul_width1:'+ul_width);
			}
		else if(navigator.appVersion.indexOf("Trident/7.0")!= -1)
		{
		item_width = (ul_actualwidth - div_width) - 34;
	 //console.log('navigator.userAgent.indexOf'+navigator.userAgent+'  '+'item_widthnext: '+item_width+'  '+'ul_actualwidthnext:'+ul_actualwidth+'  '+'div_widthnext:'+div_width+'  '+'ul_widthnext:'+ul_width);
		}
		else
			{
			item_width = (ul_actualwidth - div_width) - 24;
			 //console.log('item_width3: '+item_width+'  '+'ul_actualwidth3:'+ul_actualwidth+'  '+'div_width3:'+div_width+'  '+'ul_width3:'+ul_width);
			}
	}
	else
		{
		item_width = (ul_width - div_width) - increment;
	
	
	 //console.log('item_widthnext: '+item_width+'  '+'ul_actualwidthnext:'+ul_actualwidth+'  '+'div_widthnext:'+div_width+'  '+'ul_widthnext:'+ul_width+'  '+'increment'+increment);
		}
	left_indent = parseInt($('#carousel_inner ul').css('left')) - item_width;
	current_position = parseInt($('#carousel_inner ul').css('left'));
	$('#carousel_container .first a').css('margin-right','5px');
	if(current_position >= -162) {
		$('#carousel_container').css('padding-left', '0');
		$('#carousel_container .last').css('display', 'none');
		$('#carousel_container .first').css('display', 'block');
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'en-us'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '408px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '414px');
			else
				$('#carousel_container #carousel_inner').css('width', '404px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '456px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '463px');
			else
				$('#carousel_container #carousel_inner').css('width', '456px');
		}
		if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt'){
			if(isChromium || navigator.appVersion.indexOf("Safari")!= -1)
				$('#carousel_container #carousel_inner').css('width', '464px');
			else if (navigator.appVersion.indexOf("Trident/7.0")!= -1)
				$('#carousel_container #carousel_inner').css('width', '472px');
			else {
				$('#carousel_container #carousel_inner').css('width', '475px');
			}
		}
	}
		
	}
	
	if(itemcount === 6) {
		// console.log('Hi dude came into movetabnext 6');
		
		//item_width = (navigator.appVersion.indexOf("Trident/7.0")!= -1) ? (parseInt((ul_width - div_width) / 2) + 6) : parseInt((ul_width - div_width) / 2);
				
		var sort_width = parseInt($('#sort-bar-V2').width());
			// console.log('container width: '+sort_width);
		var actionbar_width = parseInt($('#narrow-search-actionbar').width());
			// console.log('actionbar width: '+actionbar_width);
		var carousel_inner_width = (sort_width - actionbar_width) - 45;
			// console.log('carousel inner width: '+carousel_inner_width);
		var firstli_width = parseInt($('#carousel_container #carousel_inner ul li:first').css('width'));
			// console.log('first li width: '+firstli_width);
		var secondli_width = parseInt($('#carousel_container #carousel_inner ul li:nth-child(2)').css('width'));
			// console.log('second li width: '+secondli_width);
		var thirdli_width = parseInt($('#carousel_container #carousel_inner ul li:nth-child(3)').css('width'));
			// console.log('third li width: '+thirdli_width);
		if(carousel_inner_width > (parseInt(ul_actualwidth) - firstli_width))
			item_width = firstli_width + 6;
		else if(carousel_inner_width > (parseInt(ul_actualwidth) - (firstli_width + secondli_width)))
			item_width = (firstli_width + secondli_width) + 11;
		else
			item_width = (firstli_width + secondli_width + thirdli_width) + 16;
			// console.log('item_width: '+item_width);
		$('#carousel_container #carousel_inner').css('width', carousel_inner_width);	
		
		left_indent = parseInt($('#carousel_inner ul').css('left')) - item_width;
		current_position = parseInt($('#carousel_inner ul').css('left'));
			
		if(current_position >= -162) {
			$('#carousel_container').css('padding-left', '0');
			$('#carousel_container .last').css('display', 'none');
			$('#carousel_container .first').css('display', 'block');
		}
	}

	$('#carousel_inner ul').animate({'left' : left_indent},{queue:true, duration:500},function(){  
    });
}
$('body').click(function(event){
	try{
	if(event.target.className != 'info-classroster-upload vtip'){
       $('#download_template_class_wrapper').hide();
        }
	}catch(e){
		// to do
	}
    });;
( function($) {
	
  function initAddEditDateField(selector, defaultText) {
    try{
    // Show default text instead of ''
    var fieldValue = $(selector).val();
    if (fieldValue == '' || fieldValue == defaultText) {
      $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
      $(selector).addClass('narrow-search-filterset-daterange-empty');
      $(selector).val(defaultText);
    }

    // Set the on blur, on focus and on change behaviours
    var data = selector + '&' + defaultText;
    var curObj=this;
    $(selector).blur(data, function(event) { // Can pass a string data only. but not an object data as objects are passed by reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == '' || fieldValue == defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
              $(selector).addClass('narrow-search-filterset-daterange-empty');
              $(selector).val(defaultText);
            } else if (fieldValue != defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-empty');
              $(selector).addClass('narrow-search-filterset-daterange-nonempty');
            }
          });

    $(selector).focus(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by  reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == defaultText) {
              $(selector).val('');
            }
            $(selector).removeClass('narrow-search-filterset-daterange-empty');
            $(selector).addClass('narrow-search-filterset-daterange-nonempty');

          });

  $(selector).change(data,function(event) { // Can pass a string data only.but not an object data as objects are passed by reference
            var data = event.data;
            var tokens = data.split("&");
            var selector = tokens[0];
            var fieldValue = $(selector).val();
            var defaultText = tokens[1];

            if (fieldValue == '' || fieldValue == defaultText) {
              $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
              $(selector).addClass('narrow-search-filterset-daterange-empty');
              $(selector).val(defaultText);
            } else {
              $(selector).removeClass('narrow-search-filterset-daterange-empty');
              $(selector).addClass('narrow-search-filterset-daterange-nonempty');
            }
          });
    }catch(e){
		// To Do
	}
}
  Drupal.behaviors.fadeeffect =  {

		    attach: function (context, settings) {
		    	if($('.admin-course-class-long-description .trunk8-fade').length>0){
		    	$('.trunk8-fade').trunk8(trunk8.admin_short_desc);
		    	}
		    }
  };
  Drupal.behaviors.addDatePickerToAddEditDateField =  {

	    attach: function (context, settings) {
	    	try{
		  if($("#multi_currency_setup_div").length > 0 && $("#edit-addedit-currency-save-button").length == 0)
	      {
			  dynamicWidthHeight(false);
	      }
	      $('.addedit-edit-datefield:not(.addedit-datepicker-added)').addClass('addedit-datepicker-added').each(function () {
	    	var pickerId = $(this).attr('id');
	        var datePickerTooltip = $('#'+pickerId).data('datePickerTooltip');
	        var datePickerShowTime = false;
	       // alert($('#'+pickerId).data('datePickerShowTime')+ 'ji' + pickerId + 'attr id ' + $(this).attr('id'))
	        if($('#'+pickerId).data('datePickerShowTime') == true){
	        	datePickerShowTime = true;
	        }
	        var calendarIcon = resource.base_url + '/' + themepath + '/expertusone-internals/images/calendar_icon.JPG';
	        try{
	        $('#'+pickerId).datepicker('destroy');
	        }catch(x){  }
	        try{
		        $('#'+pickerId).datetimepicker('destroy');
	        }catch(x){ }
	        try{
	        	$('#'+pickerId).timepicker('destroy');
	        }catch(x){ }

	        if(datePickerShowTime == false){
		        $('#'+pickerId).datepicker({
		          duration : '',
		          showTime : false,
		          constrainInput : false,
		          stepMinutes : 5,
		          stepHours : 1,
		          time24h : true,
		          dateFormat : 'mm-dd-yy',
		          buttonImage : calendarIcon,
		          buttonImageOnly : true,
		          firstDay : 0,
		          showOn : 'both',
		          buttonText : datePickerTooltip,
		          showButtonPanel : true,
		          changeMonth : true,
		          changeYear : true,
		          daterangeDiffDays : 20000,
		          beforeShow : function(input, inst) {
		            var dateFieldId = '#' + input.id;
		            //alert('dateFieldId = ' + dateFieldId);
		            //alert('input = ' + JSON.stringify(input));
		            //alert('inst = ' + JSON.stringify(inst));
		            var datesDisplayOption = $(dateFieldId).data('datePickerDatesDisplayOption');
		            if (datesDisplayOption == '' || datesDisplayOption == null || datesDisplayOption == 'undefined') {
		              datesDisplayOption = 'all';
		            }
		            setTimeout(function(){$('.ui-datepicker').css('z-index', 10000);}, 0);
		            var dateMin = null;
		            var dateMax = null;
	    		            var yearRange = '-10:+10';
	    		            if (datesDisplayOption != 'all' && datesDisplayOption != 'hiredate') {
		              var daterangeDiffDays = $(dateFieldId).datepicker('option', 'daterangeDiffDays');
		              var dateMin = new Date();
		              var dateMax = new Date();
		              switch (datesDisplayOption) {
		              case 'future': // Show future dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMax.setDate(dateMin.getDate() + daterangeDiffDays);
		                break;
		              case 'past': // Show past dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
		                break;
		              } // end switch
	    		            }else if(datesDisplayOption == 'hiredate') {
	    		            	yearRange ='-70:+5';
		            } // end if (datesDisplayOption != "all")
		            if(pickerId=='end_date'){
                    	var start_date_value = $('#start_date').val();                            	
                    	return {
                    		defaultDate:start_date_value,
                    		minDate : start_date_value                            		
	    		            };
	    		    }else if(pickerId =='hire_end_ste_con_hdt_btw'){
	    		            	var start_date_value = $('#hidden_hire_start_ste_con_hdt_btw').val();                            	
                            	return {
                            		yearRange:'-70:+5',
                            		defaultDate: start_date_value,
                            		minDate : start_date_value                            		
      	    		 			};
		            }else{
		            return {
	    		            	
	    		            		yearRange : yearRange,
		              minDate : dateMin,
		              maxDate : dateMax
		            };
		           }
		          } // end beforeShow
		        }); // end datepicker
	        }
	        else{
	          if($('#'+pickerId).data('datePickerShowTimeOnly') == true){
	        	 $('#'+pickerId).timePicker({
	        		  //startTime: "00.00", // Using string. Can take string or Date object.
	        		  //endTime: new Date(0, 0, 0, 15, 30, 0), // Using Date object here.
	        		  show24Hours: true,
	        		  separator: ':',
	        		  step: 5});
	          }
	          else{
	        	  $('#'+pickerId).datetimepicker({
		          duration : '',
		          showTime : true,
		          //timeOnly: false,
		          constrainInput : true,
		          stepMinutes : 5,
		          stepHours : 1,
		          time24h : true,
		         // dateFormat : 'mm-dd-yyyy',
		          buttonImage : calendarIcon,
		          buttonImageOnly : true,
		          firstDay : 1,
		          showOn : 'both',
		          buttonText : datePickerTooltip,
		          showButtonPanel : true,
		          changeMonth : true,
		          changeYear : true,
		          daterangeDiffDays : 20000,
		          beforeShow : function(input, inst) {
		            var dateFieldId = '#' + input.id;
		            var datesDisplayOption = $(dateFieldId).data('datePickerDatesDisplayOption');
		            if (datesDisplayOption == '' || datesDisplayOption == null || datesDisplayOption == 'undefined') {
		              datesDisplayOption = 'all';
		            }

		            var dateMin = null;
		            var dateMax = null;
		            if (datesDisplayOption != 'all') {
		              var daterangeDiffDays = $(dateFieldId).datepicker('option', 'daterangeDiffDays');
		              var dateMin = new Date();
		              var dateMax = new Date();
		              switch (datesDisplayOption) {
		              case 'future': // Show future dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMax.setDate(dateMin.getDate() + daterangeDiffDays);
		                break;
		              case 'past': // Show past dates including today's date as the selectable date options in the datepicker. Disable all other dates.
		                dateMin.setDate(dateMax.getDate() - daterangeDiffDays);
		                break;
		              } // end switch
		            } // end if (datesDisplayOption != "all")
		            return {
		              minDate : dateMin,
		              maxDate : dateMax
		            };
		          } // end beforeShow
		        });
	        }
	      }

	        // Add display effects for empty value in field on blur, on focus and on change
	        var emptyDateFieldText = $('#'+pickerId).data('emptyDateFieldText');
	        if (emptyDateFieldText == '' || emptyDateFieldText == null || emptyDateFieldText == 'undefined') {
	          emptyDateFieldText = Drupal.t('LBL112');
	        }
	        initAddEditDateField('#' + pickerId, emptyDateFieldText);

	      });
	      vtip();
	    }
	    	catch(e){
				// To Do
			}
	 }
	  };

  Drupal.behaviors.resizeModalBackdropOnTextareaResize =  {
      attach: function (context, settings) {
    	  try{
        //console.log('In resizeModalBackdropOnTextareaResize');
        $('textarea.addedit-edit-textarea:not(.adjust-modalbackdrop-onresize)').addClass('adjust-modalbackdrop-onresize').each(function () {

          // Save the height and width as data on the textarea field itself
          var height = $(this).height();
          var width = $(this).width();
          $(this).data({'height': height, 'width': width});
          //console.log($(this).data());

          // Set the mouseup event handler
          $(this).mouseup(function(event) {
            if (typeof(Drupal.ajax.prototype.commands.CtoolsModalAdjust) !== 'undefined') {
              // Fetch the previous dimensions
              var prevHeight = $(this).data('height');
              var prevWidth = $(this).data('width');
              // Get the new dimensions
              var newHeight = $(this).height();
              var newWidth = $(this).width();
              //var dimensions = {'prevHeight': prevHeight, 'prevWidth' : prevWidth, 'newHeight': newHeight, 'newWidth': newWidth};
              //console.log(dimensions);
              if (newHeight != prevHeight || newWidth != prevWidth) {
                //console.log('resizeModalBackdropOnTextareaResize() calling CtoolsModalAdjust()');
                Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
              }
              // Save the new dimensions
              $(this).data({'height': newHeight, 'width': newWidth});

              // Below lines for debugging
              //var finalHeight = $(this).data('height');
              //var finalWidth = $(this).data('width');
              //dimensions = {'finalHeight': finalHeight, 'finalWidth' : finalWidth};
              //console.log(dimensions);
            } // end if
          }); // end mouseup
        }); // end each
    	  }catch(e){
  			// To Do
  		}
     } // end attach
  };

  Drupal.behaviors.resizeModalBackdropOnCKEditorResize =  {
      attach: function (context, settings) {
    	 try{
        //console.log('In resizeModalBackdropOnCKEditorResize');
        $('.cke_resizer:not(.adjust-modalbackdrop-onresize)').addClass('adjust-modalbackdrop-onresize').each(function () {
          // Set the mouseup event handler
          $(this).bind('mousedown.expckeresizer',function(event) {
            //console.log(event);
            //console.log('resizeModalBackdropOnCKEditorResize calling mousedown()');
            $(document).unbind('mouseup.expckeresizer'); // needed, as mouseup on iframe does not invoke mouseup on document
              $(document).one('mouseup.expckeresizer', function() {
                //console.log('resizeModalBackdropOnCKEditorResize calling mouseup() after mousedown()');
                if (typeof(Drupal.ajax.prototype.commands.CtoolsModalAdjust) !== 'undefined') {
                  //console.log('resizeModalBackdropOnCKEditorResize calling CtoolsModalAdjust()');
                     Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
                } // end if
              });
          }); // end mouseup
        }); // end each
    	 }catch(e){
   			// To Do
   		}
     } // end attach cke_resizer
  };

  // Command to refresh narrow search results displayed in jqGrid on form submit
  Drupal.ajax.prototype.commands.refreshnarrowsearchresults = function(ajax, response, status) {
	  try{
  	if(response.refreshMode == 'immediate') {
		if (response.refreshGrid || (typeof $(response.js_object_selector).data(response.js_object).refreshLastAccessedRow != 'undefined' && $(response.js_object_selector).data(response.js_object).refreshLastAccessedRow() == false)) {
			$(response.js_object_selector).data(response.js_object).refreshGrid();
		}
  	}
  	else {
  	  if (typeof Drupal.CTools.Modal.closeCommand.refreshNarrowSearchResults === 'undefined') {
  	    Drupal.CTools.Modal.closeCommand.refreshNarrowSearchResults =
  	      (function (jsObjectSelector, jsObject, refreshGrid) {
			//console.log('jsObjectSelector', jsObjectSelector, 'jsObject', jsObject, 'refreshGrid', refreshGrid);
			try {
  	        return {
                     cmd: function () {
                	          //console.log('jsObjectSelector = ', jsObjectSelector, ',\n jsObject = ', jsObject);
                	          //console.log('refresh grid func', $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow);
                	          if (refreshGrid || (typeof $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow != 'undefined' && $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow() == false)) {
                	          	$(jsObjectSelector).data(jsObject).refreshGrid();
                	          }
                	          // $(jsObjectSelector).data('narrowsearch').refreshLastAccessedRow();
                          }
                   };
			} catch(e) {
				//console.log(e, e.stack);
			}
  	        })(response.js_object_selector, response.js_object, response.refreshGrid);
  	   }
    }
	  }catch(e){
			// To Do
			//console.log(e, e.stack);
		}
  };

  // Command to refresh narrow search results displayed in jqGrid on form submit
  Drupal.ajax.prototype.commands.refresnarrowsearchonsubmit = function(ajax, response, status) {
	  try{
  	if(response.click_object) {
  		$(response.click_object).click();
  	}
  	if(response.refreshMode == 'immediate') {
  		$(response.js_object_selector).data(response.js_object).refreshGrid();
  	}
	  }catch(e){
			// To Do
		}
  };

  // Command to destroy CKEditors on ctools modal close (if present)
  Drupal.ajax.prototype.commands.destroyCKEditorsOnCtoolsModalClose = function(ajax, response, status) {
	  try{
    //console.log('In destroyCKEditorsOnCtoolsModalClose()');
    if (typeof Drupal.CTools.Modal.closeCommand.destroyCKEditors === 'undefined') {
      Drupal.CTools.Modal.closeCommand.destroyCKEditors = {
          cmd: function () {
             if ((typeof(CKEDITOR) != 'undefined') && typeof(CKEDITOR.instances) != 'undefined') {
               //console.log('Before');
               //console.log(CKEDITOR.instances);
               $.each(CKEDITOR.instances, function () {
                 //console.log('Destroying editor');
                 //console.log(this.name);
                 this.destroy(true);
               }); // end $.each
               //console.log('After');
               //console.log(CKEDITOR.instances);
             }; // end if
           } // end function
         };
    } // end if
	    //62832 - close calendar loader in admin calendar
	    if ($("#calendarprimaryview").size())
	    	EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('calendar');
	    //Refresh tabs once the tp is created
	    if($( "#page-container-tabs-prg" ).size()){
	    //console.log('test');
	    	// Always the last tab should be in active while loading
	    	var pos = $( "#page-container-tabs-prg .visible-main-tab:last" ).index();
	    	if($( ".first-arrow" ).size() > 0 ){
	    		pos = pos - 1;
	    	}
	    	$( "#page-container-tabs-prg" ).tabs({
	    		selected: pos
	    	});
	    	//Diable attach courses
	    	var str = $('#program_attach_tabs .ui-state-active').attr('id');
			var currmod = str.split('-');
			moduleTabclick(currmod[2]);
	    	onloadsearch();
	    }
	  }catch(e){
			// To Do
		}
  };

  // Command to hide drupal messages after a period
  Drupal.ajax.prototype.commands.addEditHideMessages = function(ajax, response, status) {
	  try{
    //console.log('exp_sp_administration_addedit_behaviours.js : expHideDrupalMessages() : response');
    //console.log(response);
    var msgContext;
    //var errContext;
    if (typeof response.form_wrapper_id != 'undefined' && response.wrapper_id != '') {
      msgContext = $("#" + response.wrapper_id + " .messages");
      //errContext = $("#" + response.wrapper_id + " .error");
    }
    else {
      msgContext = $(".messages");
      //errContext = $(".error");
    }

    var height_errMsg = $(".messages").height();
    if ($(".admin-class-tp-enrollment").length > 0) {
    	$("#"+response.wrapper_id).css("height",Math.round(425+height_errMsg)+"px");
    }

    var delay = 10000;
    if (typeof response.delay != 'undefined') {
      if (typeof response.delay == "number" && response.delay > 0) {
        delay = response.delay;
      }
      else if (typeof response.delay == "string" && response.delay != '' && parseInt(response.delay) > 0) {
        delay = parseInt(response.delay);
      }
    }

    setTimeout(function() {
      msgContext.hide();
      if ($(".admin-class-tp-enrollment").length > 0) {
    	  $("#"+response.wrapper_id).css("height","403px");
      }
      //errContext.removeClass('error');
      Drupal.ajax.prototype.commands.CtoolsModalAdjust();
  }, delay);
	  }catch(e){
			// To Do
		}
  };

  // Command to scroll when we add new class in the admin screen
  Drupal.ajax.prototype.commands.addEditPopupDialogScroll = function(ajax, response, status) {
	  try{
    var dialogHeight;

    //The model dialog fixed height is 582px
    var normalHeight = 582;
    dialogHeight = $('.admin-popup-container').height();
    if(dialogHeight > normalHeight){
    	dialogHeight = dialogHeight-850;
    	$('#modal-content').scrollTop(dialogHeight);
    }
    else {
    	$('#modal-content').scrollTop(0);
    }
	  }catch(e){
			// To Do
		}
  };


  //#custom_attribute_0078975
  Drupal.behaviors.removeInputGreyfromCustomAttribute =  {
    attach: function (context, settings) {
    try{ 

		 
	  //Scroll issue in ClassAddEDITFORM #88022
		var count_class_custom_attr=$('.page-administration-learning-catalog #catalog-class-basic-addedit-form-scroll-container #custom_attr_container').length; 
    	if(count_class_custom_attr>0){  
    							//$('.catalog-course-basic-addedit-form-container').css('min-height','450px');
    							$('.catalog-course-basic-addedit-form-container').addClass('parent_custom_attr_scroll_class_container');
				    			var cls_page_height=$('#catalog-class-basic-addedit-form-scroll-container').height();
								$('#catalog-class-basic-addedit-form-scroll-container').css('height',cls_page_height);
								$('#catalog-class-basic-addedit-form-scroll-container').jScrollPane({  
									autoReinitialise: true,
									showArrows: false,
									verticalGutter:0,
									horizontalGutter:0
								});		 
	    } 

		
	    
    	//To place the scrollbar  when vertically displays options - added by ganesh for custom attribute
    	var count_vertical_opt=$('#custom_attr_container #customattribute-display-vertical').length; 
    	if(count_vertical_opt>0){
    		
    		 $('#custom_attr_container #customattribute-display-vertical').each(function() {   
			        var inp_count=$(this).find('input').length;		    	 
		    		if(inp_count>0 && inp_count>3){ //Yes there are options under verticatl div   
							$(this).addClass('cus_attr_jscroll_set');  
							$(this).jScrollPane({ 
								verticalGutter:0,
								horizontalGutter:0
							}); 
		    		}
			 }); 
    	}  
    	
    	//Remove the grey class from custom attribute container if not matched the help text - Added by ganesh for Custom Attribute
    	var cattr_page=$('.page-administration-manage-customattribute #create-dd-list li').length;
    	if(cattr_page>0){
    		$('.page-administration-manage-customattribute #create-dd-list li a span').click(function(){
    			var tmp_clicked_show_in_screen=$('.page-administration-manage-customattribute  .narrow-search-multi-action-container #bubble-face-table').length;
    			if(tmp_clicked_show_in_screen>0){
    				closeCustomAttributeQtip(0);
    			}
    		});
    	}
    	
    	 
    	 //Get Grey for HelpText    		
		var tmp_textarea=$('#custom_attr_container .addedit-edit-textarea').length;
		var tmp_textbox=$('#custom_attr_container .addedit-edit-textfield').length;
		
    	if(tmp_textarea>0 || tmp_textbox>0){
    		$('#custom_attr_container .addedit-edit-textarea').each(function() { 
    			if($(this).data('default-text')!=undefined){
    				 var defaultText = $(this).data('default-text');
    				 if(isNaN(defaultText)){ 
				      var defaultText1 = $(this).data('default-text').toLowerCase();;
				     }else{ 
				     	var defaultText1 = $(this).data('default-text');
				     }
				      
				     if($(this).val().toLowerCase() == defaultText1) { 
				        $(this).val(defaultText); 
				        $(this).addClass('input-field-grey');
				      }else { 
				        $(this).removeClass('input-field-grey');
				      }
				      
				        // Attach the event handlers
				      $(this).blur(defaultText, function(event) {
				        var defaultText = event.data;
				        if($(this).val() == '' || $(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val(defaultText);
				          $(this).addClass('input-field-grey');
				        }
				        else{
				          $(this).removeClass('input-field-grey');
				        }
				      });
			
				      $(this).focus(defaultText, function(event) {
				        var defaultText = event.data;	        
				        if($(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val('');
				          $(this).removeClass('input-field-grey');
				        }
				     });
    			}
    		}); 
    		
    		$('#custom_attr_container .addedit-edit-textfield').each(function() { 
    			if($(this).data('default-text')!=undefined){
    				 var defaultText = $(this).data('default-text');
    				 if(isNaN(defaultText)){ 
				      var defaultText1 = $(this).data('default-text').toLowerCase();;
				     }else{ 
				     	var defaultText1 = $(this).data('default-text');
				     }
				      
				     if($(this).val().toLowerCase() == defaultText1) { 
				        $(this).val(defaultText); 
				        $(this).addClass('input-field-grey');
				      }else { 
				        $(this).removeClass('input-field-grey');
				      }
				      
				        // Attach the event handlers
				      $(this).blur(defaultText, function(event) {
				        var defaultText = event.data;
				        if($(this).val() == '' || $(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val(defaultText);
				          $(this).addClass('input-field-grey');
				        }
				        else{
				          $(this).removeClass('input-field-grey');
				        }
				      });
			
				      $(this).focus(defaultText, function(event) {
				        var defaultText = event.data;	        
				        if($(this).val().toLowerCase() == defaultText.toLowerCase()) {
				          $(this).val('');
				          $(this).removeClass('input-field-grey');
				        }
				     });
    			}
    		});     		    		   	
    	}    	  
    }
    catch(e){
		// To Do
	}
    }
  };
 
  // Override multiselect behaviour and jQuery extensions so that
  // a. on Save, to select only from the current form
  // b. and to handle the 'none_selected' option.
  // Original code in multiselect.js of multiselect module.
  Drupal.behaviors.multiselect = {
    attach: function(context) {
    	try{
      //console.log('In exp_sp_administration multiselect attach()');
      // Remove the items that haven't been selected from the select box.
      $('select.multiselect_unsel:not(.multiselect-processed)', context).addClass('multiselect-processed').each(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).removeContentsFrom($(selclass));
      });

      // - This code will affect esignature form as well as script error in all the forms
      /*
      // Note: Doesn't matter what sort of submit button it is really (preview or submit)
      // Selects all the items in the selected box (so they are actually selected) when submitted
      // Expertus: If data-wrapperid is defined on the button, items are selected for multiselect existing inside the wrapperid div
      $('input.form-submit:not(.multiselect-processed)', context).addClass('multiselect-processed').click(function() {
        //console.log('In exp_sp_administration multiselect submit click()');
        var formWrapperId = $(this).data('wrapperid');
        //console.log('exp_sp_administration multiselect submit click() : formWrapperId = ' + formWrapperId);
        if (typeof(formWrapperId) !== 'undefined' && typeof(formWrapperId) === 'string' && formWrapperId != '') {
          $('#' + formWrapperId + ' select.multiselect_sel').selectAll();
        }
        else {
          $('select.multiselect_sel').selectAll();
        }
      });
      */

      // Moves selection if it's double clicked to selected box
      $('select.multiselect_unsel:not(.multiselect-unsel-processed)', context).addClass('multiselect-unsel-processed').dblclick(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).moveSelectionTo($(selclass));
      });

      // Moves selection if it's double clicked to unselected box
      $('select.multiselect_sel:not(.multiselect-sel-processed)', context).addClass('multiselect-sel-processed').dblclick(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(selclass).moveSelectionTo($(unselclass));
      });

      // Moves selection if add is clicked to selected box
      $('li.multiselect_add:not(.multiselect-add-processed)', context).addClass('multiselect-add-processed').click(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(unselclass).moveSelectionTo($(selclass));
      });

      // Moves selection if remove is clicked to selected box
      $('li.multiselect_remove:not(.multiselect-remove-processed)', context).addClass('multiselect-remove-processed').click(function() {
        unselclass = '.' + this.id + '_unsel';
        selclass = '.' + this.id + '_sel';
        $(selclass).moveSelectionTo($(unselclass));
      });
    	}catch(e){
  			// To Do
  		}
    }
  };

  //Removes the content of this select box from the target
  //usage $('nameofselectbox').removeContentsFrom(target_selectbox)
  jQuery.fn.removeContentsFrom = function() {
   //alert('removeContentsFrom');
	  try{
   var dest = arguments[0];
   this.each(function() {
     var lastOptionIdx = this.options.length - 1;
     for (var x = lastOptionIdx; x >= 0; x--) {
       dest.removeOption(this.options[x].value);
     }
   });
	  }catch(e){
			// To Do
		}
  }

  //Moves the selection to the select box specified
  //usage $('nameofselectbox').moveSelectionTo(destination_selectbox)
  jQuery.fn.moveSelectionTo = function() {
    //alert('moveSelectionTo');
 try{
   var dest = arguments[0];
   this.each(function() {
     var optionsLength = this.options.length;
     for (var x = 0; x < optionsLength; x++) {
       //alert('optionsLength before loop = ' + optionsLength);
       //alert('x before loop = ' + x);
       var option = this.options[x];
       //alert('option');
       if (option.selected && option.value != 'none_selected') {
         //alert('Calling addOption');
         dest.addOption(option);
         //alert('Returned from addOption');
         this.remove(x);
         //alert('Removed moved option');
         //$(this).triggerHandler('option-removed', option);
         x--; // The next option has taken deleted option's place. The number of options are reduced. We need to move back up.
         //alert('x end of loop = ' + x);
         optionsLength = this.options.length; // Reset length to the new length
         //alert('optionsLength end of loop = ' + optionsLength);
       }
     }
   });
 	}catch(e){
		// To Do
	}
  };

  //Adds an option to a select box
  //usage $('nameofselectbox').addOption(optiontoadd);
  jQuery.fn.addOption = function() {
	try{
    //alert('addOption');
   var option = arguments[0];
   //alert(option.value + ' => ' + option.text);
   this.each(function() {
     //alert('addOption(): Inside this.each: ' + option.value + ' => ' + option.text);
     //had to alter code to this to make it work in IE
     var anOption = document.createElement('option');
     anOption.text = option.text;
     anOption.value = option.value;
     var numOptions = this.options.length;
     if (numOptions <= 0) {
       this.options[numOptions] = anOption;
       //alert('option added at end as the options list is empty.');
     }
     else {
       var lastOption = this.options[numOptions - 1];
       //alert('lastOption.value = ' + lastOption.value);
       if (lastOption.value == 'none_selected') {
         this.options[numOptions - 1] = anOption;
         this.options[numOptions] = lastOption;
         //alert('option added before non_selected.');
       }
       else {
         this.options[numOptions] = anOption;
         //alert('option added at end.');
       }
     }
     //$(this).triggerHandler('option-added', anOption);
     return false;
   });
	}catch(e){
			// To Do
		}
  };

  //Removes an option from a select box
  //usage $('nameofselectbox').removeOption(valueOfOptionToRemove);
  jQuery.fn.removeOption = function() {
	  try{
    //alert('removeOption');
   var targOption = arguments[0];
   this.each(function() {
     var lastOptionIdx = this.options.length - 1;
     for (var x = lastOptionIdx; x >= 0; x--) {
       var option = this.options[x];
       if (option.value == targOption && option.value != 'none_selected') {
         //alert('removing option ' +  option.value + ' => ' + option.text);
         this.remove(x);
         //$(this).triggerHandler('option-removed', option);
       }
     }
   });
	  }catch(e){
			// To Do
		}
  };

})(jQuery);

// Editor Empty validation highlight color
Drupal.behaviors.highlightEditorOnValidationError =  {
	    attach: function (context, settings) {
	    	try{
	    	  $('span.cke_skin_kama:not(.exp-highlighted)').addClass('exp-highlighted').each(function () {
	           var geteditorid=$(this).attr("id");
	           var textAreaId=geteditorid.substring(4);
	           //alert(textAreaId);
	           if ($('#' + textAreaId).hasClass('error')) {
	        	 //  alert('sss');
	        	   $(this).css('border', '2px solid red');

	           }

	      });
	    	}catch(e){
	  			// To Do
	  		}
	    }
};

/* Behaviour of autofocus first field */
/*Drupal.behaviors.autoFoucusFirstsFields = {
			  attach: function(content, settings) {
				  try{
	        // Instead of use the class name we have conflict for Course and class. So we hanlde that in inc file. Other Pages using the form id.
	        $('#program-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#user-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#organization-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#catalog-survey-assesment-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#catalog-question-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#content-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#location-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        $('#banner-basic-addedit-form:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
	        //content-addedit-form location-basic-addedit-form
	        // For pre-requisite,equivalence,survey and assessment for data grid
	        $('.admin_add_multi_search_container:not(.auto-foucused)').addClass('auto-foucused').each(function () {
	        	$('form').find('select,textarea,input[type=text]').filter(':visible:first').focus();
			    });
				  }catch(e){
			  			// To Do
			  		}
	      } // end attach
};  */

//Initialize and set behaviour for custom label and value fields.
Drupal.behaviors.customField =  {
  attach: function (context, settings) {
	  try{
    var filter = '.addedit-edit-custom-label-field:not(.custom-field-initialized), .addedit-edit-custom-value-field:not(.custom-field-initialized)';
    $(filter).addClass('custom-field-initialized').each(function () {
      var defaultText = $(this).data('default-text');
      //console.log('default text = ' + defaultText);
      // Initialize the field
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
			// To Do
		}
  }
};

//Initialize and set behaviour for custom label and value fields.
Drupal.behaviors.expAddAutocompleteToField =  {
  attach: function (context, settings) {
	  try{
    $('.exp-init-acfield:not(.exp-acfield-initialized)').addClass('exp-acfield-initialized').each(function () {
      var url = $(this).data('exp-acfield-url');
      //console.log('In expAddAutocompleteToField() : url = ' + url);
      var optionsStr = $(this).data('exp-acfield-options');
      //console.log('In expAddAutocompleteToField() : optionsStr = ' + optionsStr);
      var options = eval("(" + optionsStr + ")");
      //console.log(options);

      var preText = $(this).data('exp-acfield-pretext');
      //console.log('In expAddAutocompleteToField() : preText = ' + preText);
      if (preText != '') {
        if ($(this).val() == '' || $(this).val() == preText) {
          $(this).val(preText);
          $(this).addClass('input-field-grey');
        }
        else {
          $(this).removeClass('input-field-grey');
        }

        // Attach the event handlers
        $(this).blur(preText, function(event) {
          var preText = event.data;
          //console.log('blurred. preText = ' + preText);
          if($(this).val() == '' || $(this).val() == preText) {
            $(this).val(preText);
            $(this).addClass('input-field-grey');
          }
          else {
            $(this).removeClass('input-field-grey');
          }
        });

        $(this).focus(preText, function(event) {
          var preText = event.data;
          //console.log('gained focus. preText = ' + preText);
          if($(this).val() == preText) {
            $(this).val('');
            $(this).removeClass('input-field-grey');
          }
       });
      }

      $(this).autocomplete('?q=' + url, options);
   });
	  }catch(e){
			// To Do
		}
  }
};

//Initialize and set behaviour for time picker field
Drupal.behaviors.timePicker = {
  attach: function (context, settings) {
	  try{
    $('.exp-timepicker-for-addedit-form:not(.exp-timepicker-inited)').addClass('exp-timepicker-inited').each(function () {
      // Build the timepicker dropdown
      var timePickerFieldId = $(this).attr('id');
      var expDropDownId = 'exp-dropdown-' + timePickerFieldId;
	  var hours, minutes;
	  var timePickerListHolderHTML =
	    '<div id="' + expDropDownId + '" data="' + timePickerFieldId + '" class="exp-timepicker-selection" style="display:none;">' +
	      '<ul></ul>' +
	    '</div>';

	  $(this).after(timePickerListHolderHTML);

	  for (var i = 0; i <= 1425; i += 15) {
	    hours = Math.floor(i / 60);
	    minutes = i % 60;
		if (hours < 10) {
		  hours = '0' + hours;
		}
		if (minutes < 10) {
	      minutes = '0' + minutes;
		}
		var timePickerListItemHTML = '<li>' + hours + ':' + minutes + '</li>';
		$('#' + expDropDownId + ' ul').append(timePickerListItemHTML);
	  } //end for

	  // Add click handler for freq time textfield
      $(this).click(function() {
  	    var timePickerFieldId = $(this).attr('id');
  		var expDropDownId = 'exp-dropdown-' + timePickerFieldId;
  		$('#' + expDropDownId).toggle();
  	  });

      // Add click handler for date picker dropdown list items
	  $('#' + expDropDownId + ' ul li').click(function() {
		  // Get the selected option value
		  var selectedTime = $(this).html();

		  // Show the selected value in the freq time textfield
		  var timePickerFieldId = $(this).closest('.exp-timepicker-selection').attr('data');
		  $('#' + timePickerFieldId).val(selectedTime);

		  // Close the time picker drop-down
		  $(this).closest('.exp-timepicker-selection').css('display', 'none');
	  });
    });
	  }catch(e){
			// To Do
		}
  }
};
/**Change by: ayyappans
 * #36300: Issue in Tool tip incorrectly showing in Peoples settings page
 * Root Cause: exp_sp_administration_setup_addedit_form is loaded via ajax call.
 * So vtip() method doesn't apply required changes to the title attribute of the target elements
 * Fix: explicit call has been made to vtip() funtion once the ajax call gets complete
 */
//Initialize and set behaviour to initialize vtip on ajax call of exp_sp_administration_setup_addedit_form
Drupal.behaviors.vtipInitialize = {
  attach: function (context, settings) {
	  try{
		  $('.exp_sp_administration_setup_addedit_form').ajaxComplete(function(event, xhr, settings) {
			  vtip();
		  });
	  }catch(e){
			// To Do
		}
  }
};
//23872: Unable to make a class to be Shown in Catalog, when no content is associated
function showContentNeedMessage(){
	var error = new Array();
	 error[0] = Drupal.t('MSG498');
	 var message_call = expertus_error_message(error,'error');
	 $('#bubble-face-table #show_expertus_message').show();
	 $('#bubble-face-table #show_expertus_message').html(message_call);
	 return false;
}
//35264: Issue with Cancellation at Class Level
function showUserEnrolledMessage(){
	var error = new Array();
	 error[0] = Drupal.t('LBL1246');
	 var message_call = expertus_error_message(error,'error');
	 $('#bubble-face-table #show_expertus_message').show();
	 $('#bubble-face-table #show_expertus_message').html(message_call);
	 return false;
}
function checkIfContentAttemptsSet($uniqueId,type) {
	//atempts validation
	if(type != "recertify"){
	$('#hidden_idlist_content-'.$uniqueId).val();
//	var contentList = $('input[type=hidden][name=hidden_idlist_'+$uniqueId+']').val().split(',');
	var contentList = document.getElementById('hidden_idlist_'+$uniqueId) !== null ? document.getElementById('hidden_idlist_'+$uniqueId).value.split(',') : '';
	var attemptsNotSet = false;
	if(contentList != "" && contentList.length) {
		$.each(contentList, function(){
			var attempt = document.getElementById('content-maxattempt-'+this+'-hidden') !== null ? document.getElementById('content-maxattempt-'+this+'-hidden').value : '';
			if($.trim(attempt) == "") {
				attemptsNotSet = true;
				return false;
			}
	});
	}
		var msg = Drupal.t('MSG727')+' '+Drupal.t('MSG728');
		var drupalTitle = Drupal.t("LBL857");
	}else{
		var attemptsNotSet = true;
		var selected = $('input[name="selected_enroll_path_name"]').val();
		var msg = Drupal.t("MSG810")+' '+selected+' .'+Drupal.t("MSG728");
		var drupalTitle = Drupal.t("LBL429");
	}
	
	if(attemptsNotSet) {
		var uniqueClassPopup = 'unique-delete-class';
		var theme = Drupal.settings.ajaxPageState.theme;
		var wSize = (wSize) ? wSize : 300;
		var yesButAction = '';
		$('#content-attempt-message').remove();
		var html = '';
		html+='<div id="content-attempt-message" style="display:none; padding: 0px;">';
		html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';

		if(theme == 'expertusoneV2'){
			html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+msg+'</td></tr>';
		} else {
			html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: left;">'+msg+'</td></tr>';
		}
		html+='</table>';
		html+='</div>';
		$("body").append(html);
		var closeButt={};
		closeButt[Drupal.t('No')]=function(){
			$(this).dialog('destroy');
			$(this).dialog('close');
			$('#content-attempt-message').remove();
		};
		yesButAction = function(){
			$(this).dialog('destroy');
			$(this).dialog('close');
			if(type == "recertify"){
				$('#submit-enrolltp-'+$uniqueId).click();
			}else{
			$('#submit-content-'+$uniqueId).click();
			}
			$('#content-attempt-message').remove();
		};
		closeButt[Drupal.t('Yes')]= yesButAction;

		$("#content-attempt-message").dialog({
			position:[(getWindowWidth()-400)/2,200],
			bgiframe: true,
			width:wSize,
			resizable:false,
			modal: true,
			title:Drupal.t(drupalTitle),
			buttons:closeButt,
			closeOnEscape: false,
			draggable:false,
			overlay:
			{
				opacity: 0.9,
				background: "black"
			}
		});
		$('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

		$("#content-attempt-message").show();

		$('.ui-dialog-titlebar-close').click(function(){
			$("#content-attempt-message").remove();
		});
		if(theme == 'expertusoneV2'){
			changeDialogPopUI();
		}
		if($('div.qtip-defaults').length > 0) {
			var prevZindex = $('.qtip-defaults').css('z-index');
			$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
			$('.ui-widget-overlay').css('z-index', prevZindex+1);
		}else if($('div#modalBackdrop').length > 0){
			var prevZindex = $('#modalBackdrop').css('z-index');
			$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
			$('.ui-widget-overlay').css('z-index', prevZindex+1);
		}
	}
	else {
		$('#submit-content-'+$uniqueId).click();
	}
    return false;
}

function webex_config(uniqueid){
	$('.form-item-webex-radio').addClass(uniqueid);
	
	if(uniqueid == 'webex_User'){
		$('#webex_org_wrapper').css('display','none');
		} else if(uniqueid == 'webex_Organization'){
		$('#webex_org_wrapper').css('display','block');
	}	
}

	function webex_onload() {
	   var webex_radio_val = $("input[name='webex_radio']:checked").val();
       if(webex_radio_val == 'webex_Organization') {
			$('#webex_org_wrapper').css('display','block');	   
       } else if(webex_radio_val == 'webex_User') { 
    	   $('#webex_org_wrapper').css('display','none');	
       } else {
    	   $('#webex_org_wrapper').css('display','block');	
       }
	}
	
		
	function webex_presenter_validate(presenterid,user_lang) {
		
		var otherSessionWidth	= '';
		
		if (user_lang == 'ru') {
			otherSessionWidth = 333;
		} else if (user_lang == 'de'){
			otherSessionWidth = 310;
		} else if (user_lang == 'es'){
			otherSessionWidth = 335;
		} else if (user_lang == 'fr'){
			otherSessionWidth = 328;
		} else if (user_lang == 'it'){
			otherSessionWidth = 296;
		} else if (user_lang == 'pt-pt'){
			otherSessionWidth = 278;
		} else if (user_lang == 'ja'){
			otherSessionWidth = 300;
		} else if (user_lang == 'ko'){
			otherSessionWidth = 290;
		} else if (user_lang == 'zh-hans'){
			otherSessionWidth = 253;
		} else{
			otherSessionWidth = 279;
		}
		
		var popupentityId = 0;
		var popupAddSessionentityType = 'presenter';
		var popupAddSessionIdInit = popupentityId+'_'+popupAddSessionentityType;
		var popupAddpresentervisibPopupId  = 'qtip_visible_disp_addpresenter_'+popupAddSessionIdInit;
				
		var popupAddSessionVC = jQuery.parseJSON( '{"entityId":"","entityType":"'+popupAddSessionentityType+'","url":"ajax/class-presenter-check","popupDispId":"exp_meeting_'+popupAddpresentervisibPopupId+'","catalogVisibleId":"'+popupAddpresentervisibPopupId+'","wBubble":'+otherSessionWidth+',"hBubble":"auto","tipPosition":"tipfaceMiddleRight","qtipClass":"admin-qtip-presenter-access-parent"}' );
							
		$.ajax({
			 type: "POST",
		         url: '?q=ajax/presenter/webex-credentials/'+presenterid,
		         data:'',
		         success: function(result){
	        	
		        	if(result == 1) {
		        		$('#root-admin').data('narrowsearch').getQtipDiv(popupAddSessionVC);
		        		$('#admin-qtip-access-parent-lrn_cls_vct_web').css('bottom','32px');
		        		if (user_lang == 'fr'){
		       			$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','485px').css('bottom','-115px').css('top','-82px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','489px').css('top','-46px');
		        			$('label[for*="edit-presenter-"]').css('width','135px');
		        		} else if (user_lang == 'de'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','116px');
		        		} else if (user_lang == 'it'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','102px');
		        		} else if (user_lang == 'ja'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','87px');
		        		} else if (user_lang == 'ko'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','80px');
		        		} else if (user_lang == 'ru'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','473px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','477px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','139px');
		        		} else if (user_lang == 'zh-hans'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','58px');
		        		} else if (user_lang == 'es'){
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left','499px').css('bottom','-115px').css('top','-90px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','503px').css('top','-56px');
		        		$('label[for*="edit-presenter-"]').css('width','141px');
		        		} else {
		        		$('.admin-qtip-presenter-access-parent').css('position','absolute').css('left',+parseInt(-208+675)+'px').css('bottom','-115px').css('top','-86px').css('z-index','100');
		        		$('#admin-qtip-presenter-access-parent').css('left','471px').css('top','-50px');
		        		$('label[for*="edit-presenter-"]').css('width','79px');
		        		}
		        	} 
		        	if(result == 0) {
		        		$('#exp_meeting_qtip_visible_disp_addpresenter_0_presenter').empty();
		        	}
		        }
		});
	}

function metatag_error_validate () {
	if($('#metatag-basic-addedit-form-container .addedit-edit-description').hasClass('error')) {
		$('.form-textarea-wrapper').addClass('error');
	}
}
;
/* jshint forin:true, noarg:true, noempty:true, eqeqeq:true, boss:true, undef:true, curly:true, browser:true, jquery:true */
/*
 * jQuery MultiSelect UI Widget Filtering Plugin 1.4
 * Copyright (c) 2011 Eric Hynds
 *
 * http://www.erichynds.com/jquery/jquery-ui-multiselect-widget/
 *
 * Depends:
 *   - jQuery UI MultiSelect widget
 *
 * Dual licensed under the MIT and GPL licenses:
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 *
*/
(function($){
	var rEscape = /[\-\[\]{}()*+?.,\\\^$|#\s]/g;
	
	$.widget("ech.multiselectfilter", {
		
		options: {
			label: "Filter:",
			width: null, /* override default width set in css file (px). null will inherit */
			placeholder: "Enter keywords",
			autoReset: false
		},
		
		_create: function(){
			var self = this,
				opts = this.options,
				instance = (this.instance = $(this.element).data("multiselect")),
				
				// store header; add filter class so the close/check all/uncheck all links can be positioned correctly
				header = (this.header = instance.menu.find(".ui-multiselect-header").addClass("ui-multiselect-hasfilter")),
				
				// wrapper elem
				wrapper = (this.wrapper = $('<div class="ui-multiselect-filter">'+(opts.label.length ? opts.label : '')+'<input placeholder="'+opts.placeholder+'" type="search"' + (/\d/.test(opts.width) ? 'style="width:'+opts.width+'px"' : '') + ' /></div>').prependTo( this.header ));

			// reference to the actual inputs
			this.inputs = instance.menu.find('input[type="checkbox"], input[type="radio"]');
			
			// build the input box
			this.input = wrapper
			.find("input")
			.bind({
				keydown: function( e ){
					// prevent the enter key from submitting the form / closing the widget
					if( e.which === 13 ){
						e.preventDefault();
					}
				},
				keyup: $.proxy(self._handler, self),
				click: $.proxy(self._handler, self)
			});
			
			// cache input values for searching
			this.updateCache();
			
			// rewrite internal _toggleChecked fn so that when checkAll/uncheckAll is fired,
			// only the currently filtered elements are checked
			instance._toggleChecked = function(flag, group){
				var $inputs = (group && group.length) ?
						group :
						this.labels.find('input'),
					
					_self = this,

					// do not include hidden elems if the menu isn't open.
					selector = self.instance._isOpen ?
						":disabled, :hidden" :
						":disabled";

				$inputs = $inputs.not( selector ).each(this._toggleState('checked', flag));
				
				// update text
				this.update();
				
				// figure out which option tags need to be selected
				var values = $inputs.map(function(){
					return this.value;
				}).get();
				
				// select option tags
				this.element
					.find('option')
					.filter(function(){
						if( !this.disabled && $.inArray(this.value, values) > -1 ){
							_self._toggleState('selected', flag).call( this );
						}
					});
			};
			
			// rebuild cache when multiselect is updated
			var doc = $(document).bind("multiselectrefresh", function(){
				self.updateCache();
				self._handler();
			});

			// automatically reset the widget on close?
			if(this.options.autoReset) {
				doc.bind("multiselectclose", $.proxy(this._reset, this));
			}
		},
		
		// thx for the logic here ben alman
		_handler: function( e ){
			var term = $.trim( this.input[0].value.toLowerCase() ),
			
				// speed up lookups
				rows = this.rows, inputs = this.inputs, cache = this.cache;
			
			if( !term ){
				rows.show();
			} else {
				rows.hide();
				
				var regex = new RegExp(term.replace(rEscape, "\\$&"), 'gi');
				
				this._trigger( "filter", e, $.map(cache, function(v, i){
					if( v.search(regex) !== -1 ){
						rows.eq(i).show();
						return inputs.get(i);
					}
					
					return null;
				}));
			}

			// show/hide optgroups
			this.instance.menu.find(".ui-multiselect-optgroup-label").each(function(){
				var $this = $(this);
				var isVisible = $this.nextUntil('.ui-multiselect-optgroup-label').filter(function(){
				  return $.css(this, "display") !== 'none';
				}).length;
				
				$this[ isVisible ? 'show' : 'hide' ]();
			});
		},

		_reset: function() {
			this.input.val('').trigger('keyup');
		},
		
		updateCache: function(){
			// each list item
			this.rows = this.instance.menu.find(".ui-multiselect-checkboxes li:not(.ui-multiselect-optgroup-label)");
			
			// cache
			this.cache = this.element.children().map(function(){
				var self = $(this);
				
				// account for optgroups
				if( this.tagName.toLowerCase() === "optgroup" ){
					self = self.children();
				}
				
				return self.map(function(){
					return this.innerHTML.toLowerCase();
				}).get();
			}).get();
		},
		
		widget: function(){
			return this.wrapper;
		},
		
		destroy: function(){
			$.Widget.prototype.destroy.call( this );
			this.input.val('').trigger("keyup");
			this.wrapper.remove();
		}
	});
})(jQuery);
;
Drupal.locale = { 'strings': {"Content":"Content","Location":"Location","Group":"Group","ERR047":"You have already registered for this class","ERR054":"There are no courses under this program.","ERR055":"Login again since your session has expired","ERR057":"System error has occurred. Contact Support.","MSG247":"CartAdded","MSG248":"Registration completed.","Learning Plan":"Learning Plan","Certification":"Certification","Curricula":"Curricula","English":"English","Register":"Register","Sign In":"Sign In","Registered":"Registered","Completed":"Completed","Enrolled":"Enrolled","Canceled":"Canceled","Pending":"Pending","Expired":"Expired","Survey":"Survey","ERR061":"There are no classes under the course.","ERR066":"Select a class in each course.","LBL001":"Sign In","LBL012":"Enrolled","LBL013":"Completed","LBL014":"Incomplete","LBL015":"Canceled","LBL017":"title a-z","LBL018":"title z-a","LBL019":"date new to old","LBL020":"date old to new","LBL021":"type","LBL022":"mandatory","LBL024":"Expired","LBL025":"Enrolled by","LBL026":"Canceled On","LBL027":"Completed On","LBL028":"Expired On","LBL032":"Registered","LBL034":"Completed","LBL036":"Type","LBL038":"Language","LBL041":"Location","LBL042":"Date","LBL044":"newly listed","LBL045":"start date","LBL046":"Full","LBL049":"Added to cart","LBL050":"Add to cart","LBL052":"Seat left","LBL053":"Seats left","LBL054":"Username","LBL073":"Job Title","LBL074":"Timezone","LBL082":"Remove","LBL083":"Title","LBL086":"Instructor","German":"German","Spanish":"Spanish","French":"French","Italian":"Italian","Japanese":"Japanese","Korean":"Korean","Russian":"Russian","MSG251":"You have successfully shared the  ","LBL087":"Course","LBL088":"Enter a course","LBL096":"Code","LBL102":"Status","LBL106":"Seats","LBL107":"Name","LBL108":"Action","LBL109":"Cancel","LBL112":"mm-dd-yyyy","LBL113":"End: mm-dd-yyyy","LBL116":"Email From","LBL117":"Email To","LBL121":"optional","LBL123":"Close","LBL126":"Waitlist seat left","LBL127":"Waitlist seats left","LBL133":"Job Role","LBL134":"Manager Name","LBL137":"Organization Name","LBL173":"User Type","LBL181":"Type a Username","MSG262":"No search results found.","MSG263":"Do you want to cancel the enrollment?","MSG264":"The entire amount paid will be refunded back to your account","MSG266":"Do you still want to cancel the class registration?","LBL186":"enrolled","LBL190":"Waitlisted","LBL191":"Tags","LBL193":"Type a tag","Organization":"Organization","MSG268":"No courses have been associated","LBL199":"Launch","LBL202":"Attempts Left","LBL203":"Survey","LBL205":"Certificate","LBL218":"Select a date","LBL229":"Description","LBL231":"Attachments","LBL232":"(click to download)","LBL234":"Complete By","LBL247":"Course Equivalence","LBL251":"Start","LBL263":"Class Code","LBL271":" already exist.","LBL274":"Assessment","LBL275":"Enrollments","LBL277":"Sessions","LBL286":"Delete","LBL287":"Add","LBL295":"Session Name","Yes":"Yes","No":"No","LBL304":"Search","LBL316":"Classroom","LBL324":"Type a question","LBL325":"Question","ERR101":" is required.","ERR106":"Select one or more.","LBL342":"Yes","LBL343":"No","MSG311":"Users can register multiple times to the class.","ERR119":"Select one or more Users.","LBL416":"Loading...","LBL426":"Mandatory","LBL427":"Recommended","LBL428":"Any","LBL429":"Recertify","LBL432":"System","LBL435":"Class","LBL545":"Type title or code","CATALOG":"CATALOG","REPORTS":"Reports","Assessment":"Assessment","LBL569":"Done","LBL573":"Activate","LBL604":"Validity","LBL605":"days","LBL621":"To","Course":"Course","Class":"Class","Classroom":"Classroom","Video":"Video","Virtual Class":"Virtual Class","Web-based":"Web-based","LBL646":"Selected","LBL647":"AND","LBL648":"Starts on","LBL649":"From","LBL661":"View Option","LBL662":"Horizontal","LBL663":"Vertical","MSG379":"No classes exist under one of the associated courses. Registration failed. Contact Support Team.","MSG380":"No seats available for this program","Training Plan":"Training Plan","MSG381":"Your search did not return any results","LBL651":"Re-Certify","Mandatory":"Mandatory","Recommended":"Recommended","Optional":"Optional","LBL674":"Select","LBL668":"Score","LBL670":"Session Details","LBL675":"Select Start Date","LBL676":"Select End Date","MSG395":"Select atleast one option.","MSG399":"Paid training cannot be rescheduled. Contact Support.","MSG400":"Cancel the Course registration and Register again.","MSG401":"will be deducted if you cancel the class.","MSG402":"An amount of","MSG403":"There are no records in the system","LBL677":"Days Remaining","LBL680":"Canceled by","LBL681":"Completed by","LBL685":"completed","LBL686":"incomplete","LBL687":"expired","LBL688":"canceled","LBL689":"pending","LBL691":"Full Name","LBL692":"Previous","LBL693":"Next","LBL694":"Preview","LBL698":"Incomplete","LBL699":"Relevant to status","LBL702":"Type Course Title","LBL704":"Enrolled On","LBL706":"Completion Date","LBL726":"Not Registered","ERR149":"Select a class in each course.","LBL721":"Registration Status","LBL716":"Select Class","ERR152":"Invalid email address in","MSG419":"The session is completed.","MSG420":"The session has not started, try again at","MSG424":"Cancellation in progress.....","LBL713":"More","MSG430":"You are already enrolled in this program","LBL735":"Expires On","LBL737":"Web-based","LBL738":"Virtual Class","LBL739":"Video","LBL740":"Certification","LBL741":"Curricula","LBL742":"Learning Plan","LBL743":"C","LBL746":"R","LBL747":"Marked Complete","LBL749":"Confirmation","LBL756":"Type a Content name","LBL763":"Starts At","Not Registered":"Not Registered","Instructor":"Instructor","ERR154":"System error, Contact Support Team.","LBL766":"Type a Class Title or Code","MSG498":"There is no content attached to the class.","Compliance":"Compliance","ERR169":" are required.","MSG511":"Not Started","LBL816":"View","LBL820":"Order Id","LBL846":"Format Report Body","LBL847":"Format Report Header","LBL848":"Select options","MSG522":"Are you sure you want to delete?","LBL852":"User Profile","LBL854":"Lesson","LBL857":"Attempts","LBL868":"Add Comment","LBL871":"Post","MSG535":"You have already registered for the class","MSG536":"Do you want to register again?","LBL880":"Join","LBL889":"Lessons","LBL893":"Minimize","LBL894":"maximize","LBL910":"Day","Simplified Chinese":"Simplified Chinese","LBL930":"Unlock","LBL941":"RegFrom","LBL945":"on","LBL951":"Type Class Title","In progress":"In progress","Incomplete":"Incomplete","No Show":"No Show","Attended":"Attended","Waitlist":"Waitlist","Manager":"Manager","MSG588":"per page","MSG605":"There are no users to be transferred.","LBL943":"Change","LBL929":"Message","LBL981":"of","LBL983":"Keyword","LBL989":"Page","LBL1003":"Module","LBL1039":"All","MSG639":"character types are lower case, upper case, digit or punctuation","MSG644":"The group name","LBL1064":"Points","Check":"Check","LBL1123":"Version","LBL1145":"Insert","LBL1146":"Select valid To Date","LBL1154":"Select valid From Date","LBL1155":"To date cannot be less than the From date","MSG687":"Select an option.","LBL1163":"Do you want to confirm the order?","LBL1164":"Do you want to cancel the order?","LBL1165":"Refund","MSG692":"Cannot register since there is price associated to the Class.Contact Support.","LBL1193":"Incomplete on","LBL1207":"Schedule","LBL1224":"Last Run","LBL1230":"Type name or username","LBL1239":"Next Run","LBL1235":"Enter message. If empty SMS will not be sent to users.","LBL1246":"Priced training cannot be canceled. Cancellation can be done from the order screen.","LBL1253":"Pre","ERR245":"Attempts should not be less than 1","LBL1267":"You have already registered for another class of the same course","LBL1268":"Do you want to register to another class?","MSG711":"Access is not set. All users will be registered to the class if access is not set. Do you want to set Access?","Groups":"Groups","by":"by","Share":"Share","LBL1270":"Type a group name","Search for refine":"Search to refine","The location is associated to class":"The location is associated to class","You need to complete":"You need to complete","LBL1272":"Uncheck","E-MAIL":"E-MAIL","EMBED":"EMBED","LBL1275":"Widget Size","LBL1276":"Show thumbnail image","LBL1277":"Show description","LBL1278":"Show Register button","LBL1279":"Show Separator line","LBL1280":"Pass URL","ERR248":"Select the required details and activate the discount.","Reregister":"Reregister","!name cannot be longer than %max characters but is currently %length characters long.":"!name cannot be longer than %max characters but is currently %length characters long.","Panel":"Panel","MSG729":"Unchecking the manager role will remove all direct and indirect reports for this user. Do you want to continue?","MSG727":"Attempts not set for one or more content.","MSG728":"Do you want to proceed?","LBL1283":"Not Set","LBL1284":"Success Status","failed":"Failed","passed":"Passed","unknown":"Unknown","MSG730":"User enrollment is in progress. An email will be sent to you after it is done.","MSG746":"There are no currency to be added.","MSG747":"Are you sure you want to delete the currency","Waived":"Waived","MSG754":"Content is playing in a window. Close the window before launching another content."} };;
/* jshint forin:true, noarg:true, noempty:true, eqeqeq:true, boss:true, undef:true, curly:true, browser:true, jquery:true */
/*
 * jQuery MultiSelect UI Widget 1.12
 * Copyright (c) 2011 Eric Hynds
 *
 * http://www.erichynds.com/jquery/jquery-ui-multiselect-widget/
 *
 * Depends:
 *   - jQuery 1.4.2+
 *   - jQuery UI 1.8 widget factory
 *
 * Optional:
 *   - jQuery UI effects
 *   - jQuery UI position utility
 *
 * Dual licensed under the MIT and GPL licenses:
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 *
*/
(function($, undefined){

var multiselectID = 0;

$.widget("ech.multiselect", {
	
	// default options
	
	options: {
		header: true,
		height: 175,
		minWidth: 225,
		classes: '',
		checkAllText: 'Check all',
		uncheckAllText: 'Uncheck all',
		noneSelectedText: Drupal.t("LBL848"),
		selectedText: "#",
		selectedList: 0,
		show: '',
		hide: '',
		autoOpen: false,
		multiple: true,
		pagefrom:'',//#custom_attribute_0078975
		entityname:'',
		position: {},
		  appendTo: "body"
	},

	_create: function() {
	    		//this.options.noneSelectedText = Drupal.t('LBL848');
	    		//#custom_attribute_0078975
		if(this.options['pagefrom']=='customattr')
			{
		this.options['noneSelectedText'] = Drupal.t('LBL674');
			}
		else
			{
		this.options['noneSelectedText'] = Drupal.t('LBL848');
            }
		var el = this.element.hide(),
			o = this.options;

	      this.speed = $.fx.speeds._default; // default speed for effects
	      this._isOpen = false; // assume no

	      // create a unique namespace for events that the widget
	      // factory cannot unbind automatically. Use eventNamespace if on
	      // jQuery UI 1.9+, and otherwise fallback to a custom string.
	    

	      var button = (this.button = $('<button type="button"><span class=" ui-icon ui-icon-triangle-1-s"></span></button>'))
	        .addClass('ui-multiselect ui-widget ui-state-default ui-corner-all')
	        .addClass(o.classes)
	        .attr({ 'title':el.attr('title'), 'aria-haspopup':true, 'tabIndex':el.attr('tabIndex') })
	        .insertAfter(el),

	        buttonlabel = (this.buttonlabel = $('<span />'))
	          .html(o.noneSelectedText)
	          .appendTo(button),

	        menu = (this.menu = $('<div />'))
	          .addClass('ui-multiselect-menu ui-widget ui-widget-content ui-corner-all')
	          .addClass(o.classes)
	          .appendTo($(o.appendTo)),

	        header = (this.header = $('<div />'))
	          .addClass('ui-widget-header ui-corner-all ui-multiselect-header ui-helper-clearfix')
	          .appendTo(menu),

	        headerLinkContainer = (this.headerLinkContainer = $('<ul />'))
	          .addClass('ui-helper-reset')
	          .html(function() {
	            if(o.header === true) {
	              return '<li><a class="ui-multiselect-all" href="#"><span class="ui-icon ui-icon-check"></span><span>' + o.checkAllText + '</span></a></li><li><a class="ui-multiselect-none" href="#"><span class="ui-icon ui-icon-closethick"></span><span>' + o.uncheckAllText + '</span></a></li>';
	            } else if(typeof o.header === "string") {
	              return '<li>' + o.header + '</li>';
	            } else {
	              return '';
	            }
	          })
	          .append('<li class="ui-multiselect-close"><a href="#" class="ui-multiselect-close"><span class="ui-icon ui-icon-circle-close"></span></a></li>')
	          .appendTo(header),

	        checkboxContainer = (this.checkboxContainer = $('<ul />'))
	          .addClass('ui-multiselect-checkboxes ui-helper-reset')
	          .appendTo(menu);

	        // perform event bindings
	        this._bindEvents();

	        // build menu
	        this.refresh(true);

	        // some addl. logic for single selects
	        if(!o.multiple) {
	          menu.addClass('ui-multiselect-single');
	        }

	        // bump unique ID
	        multiselectID++;
	    },
	
	_init: function(){
		if( this.options.header === false ){
			this.header.hide();
		}
		if( !this.options.multiple ){
			this.headerLinkContainer.find('.ui-multiselect-all, .ui-multiselect-none').hide();
		}
		if( this.options.autoOpen ){
			this.open();
		}
		if( this.element.is(':disabled') ){
			this.disable();
		}
	},
	
	refresh: function(init) {
	      var el = this.element;
	      var o = this.options;
	      var menu = this.menu;
	      var checkboxContainer = this.checkboxContainer;
	      var optgroups = [];
	      var html = "";
	      var id = el.attr('id') || multiselectID++; // unique ID for the label & option tags

	      // build items
	      el.find('option').each(function(i) {
	        var $this = $(this);
	        var parent = this.parentNode;
	        var title = this.innerHTML;
			var description = this.title;
	        var value = this.value;
	        var inputID = 'ui-multiselect-' + (this.id || id + '-option-' + i);
	        var isDisabled = this.disabled;
	        var isSelected = this.selected;
	        var labelClasses = [ 'ui-corner-all' ];
	        var liClasses = (isDisabled ? 'ui-multiselect-disabled ' : ' ') + this.className;
	        var optLabel;

	        // is this an optgroup?
	        if(parent.tagName === 'OPTGROUP') {
	          optLabel = parent.getAttribute('label');

	          // has this optgroup been added already?
	          if($.inArray(optLabel, optgroups) === -1) {
	            html += '<li class="ui-multiselect-optgroup-label ' + parent.className + '"><a href="#">' + optLabel + '</a></li>';
	            optgroups.push(optLabel);
	          }
	        }

	        if(isDisabled) {
	          labelClasses.push('ui-state-disabled');
	        }

	        // browsers automatically select the first option
	        // by default with single selects
	        if(isSelected && !o.multiple) {
	          labelClasses.push('ui-state-active');
	        }

	        html += '<li class="' + liClasses + '">';

	        // create the label
	        html += '<label for="' + inputID + '" title="' + addslashesForReport(title) + '" class="' + labelClasses.join(' ') + '">';
	        html += '<input id="' + inputID + '" name="multiselect_' + id + '" type="' + (o.multiple ? "checkbox" : "radio") + '" value="' + value + '" title="' + addslashesForReport(title) + '"';

	        // pre-selected?
	        if(isSelected) {
	          html += ' checked="checked"';
	          html += ' aria-selected="true"';
	        }

	        // disabled?
	        if(isDisabled) {
	          html += ' disabled="disabled"';
	          html += ' aria-disabled="true"';
	        }

	        // add the title and close everything off
	        html += ' /><span>' + titleRestrictionFadeoutImage(description,'rpt-grp-name') + '</span></label></li>';
	      });

	      // insert into the DOM
	      checkboxContainer.html(html);

	      // cache some moar useful elements
	      this.labels = menu.find('label');
	      this.inputs = this.labels.children('input');

	      // set widths
	      this._setButtonWidth();
	      this._setMenuWidth();

	      // remember default value
	      this.button[0].defaultValue = this.update();

	      // broadcast refresh event; useful for widgets
	      if(!init) {
	        this._trigger('refresh');
	      }	
	    },
	
	 // updates the button text. call refresh() to rebuild
	    update: function() {
	      var o = this.options;
	      var $inputs = this.inputs;
	      var $checked = $inputs.filter(':checked');
	      var numChecked = $checked.length;
	      var value;

	      if(numChecked === 0) {
	        value = o.noneSelectedText;
	      } else {
	        if($.isFunction(o.selectedText)) {
	        	
	          value = o.selectedText.call(this, numChecked, $inputs.length, $checked.get());
	        } else if(/\d/.test(o.selectedList) && o.selectedList > 0 && numChecked <= o.selectedList) {
	        	//alert(o.selectedText);
	          value = $checked.map(function() { return $(this).next().html(); }).get().join(', ');
	        } else {
	          value = o.selectedText.replace('#', numChecked).replace('#', $inputs.length)+' '+ Drupal.t("LBL646");
	        }
	      }

	      this._setButtonValue(value);

	      return value;
	    },
	    // can easily override it.
	    _setButtonValue: function(value) {
	      this.buttonlabel.text(value);
	    },
	// binds events
	_bindEvents: function(){
		var self = this, button = this.button;
		
		function clickHandler(){
			self[ self._isOpen ? 'close' : 'open' ]();
			return false;
		}
		
		// webkit doesn't like it when you click on the span :(
		button
			.find('span')
			.bind('click.multiselect', clickHandler);
		
		// button events
		button.bind({
			click: clickHandler,
			keypress: function( e ){
				switch(e.which){
					case 27: // esc
					case 38: // up
					case 37: // left
						self.close();
						break;
					case 39: // right
					case 40: // down
						self.open();
						break;
				}
			},
			mouseenter: function(){
				if( !button.hasClass('ui-state-disabled') ){
					$(this).addClass('ui-state-hover');
				}
			},
			mouseleave: function(){
				$(this).removeClass('ui-state-hover');
			},
			focus: function(){
				if( !button.hasClass('ui-state-disabled') ){
					$(this).addClass('ui-state-focus');
				}
			},
			blur: function(){
				$(this).removeClass('ui-state-focus');
			}
		});

		// header links
		this.header
			.delegate('a', 'click.multiselect', function( e ){
				// close link
				if( $(this).hasClass('ui-multiselect-close') ){
					self.close();
			
				// check all / uncheck all
				} else {
					self[ $(this).hasClass('ui-multiselect-all') ? 'checkAll' : 'uncheckAll' ]();
				}
			
				e.preventDefault();
			});
		
		// optgroup label toggle support
		this.menu
			.delegate('li.ui-multiselect-optgroup-label a', 'click.multiselect', function( e ){
				e.preventDefault();
				
				var $this = $(this),
					$inputs = $this.parent().nextUntil('li.ui-multiselect-optgroup-label').find('input:visible:not(:disabled)'),
				    nodes = $inputs.get(),
				    label = $this.parent().text();
				
				// trigger event and bail if the return is false
				if( self._trigger('beforeoptgrouptoggle', e, { inputs:nodes, label:label }) === false ){
					return;
				}
				
				// toggle inputs
				self._toggleChecked(
					$inputs.filter('[checked]').length !== $inputs.length,
					$inputs
				);

				self._trigger('optgrouptoggle', e, {
				    inputs: nodes,
				    label: label,
				    checked: nodes[0].checked
				});
			})
			.delegate('label', 'mouseenter.multiselect', function(){
				if( !$(this).hasClass('ui-state-disabled') ){
					self.labels.removeClass('ui-state-hover');
					$(this).addClass('ui-state-hover').find('input').focus();
				}
			})
			.delegate('label', 'keydown.multiselect', function( e ){
				e.preventDefault();
				
				switch(e.which){
					case 9: // tab
					case 27: // esc
						self.close();
						break;
					case 38: // up
					case 40: // down
					case 37: // left
					case 39: // right
						self._traverse(e.which, this);
						break;
					case 13: // enter
						$(this).find('input')[0].click();
						break;
				}
			})
			.delegate('input[type="checkbox"], input[type="radio"]', 'click.multiselect', function( e ){
				var $this = $(this),
					val = this.value,
					checked = this.checked,
					tags = self.element.find('option');
				
				// bail if this input is disabled or the event is cancelled
				if( this.disabled || self._trigger('click', e, { value: val, text: this.title, checked: checked }) === false ){
					e.preventDefault();
					return;
				}

				// make sure the input has focus. otherwise, the esc key
				// won't close the menu after clicking an item.
				$this.focus();
				
				// toggle aria state
				$this.attr('aria-selected', checked);
				
				// change state on the original option tags
				tags.each(function(){
					if( this.value === val ){
						this.selected = checked;
					} else if( !self.options.multiple ){
						this.selected = false;
					}
				});
				
				// some additional single select-specific logic
				if( !self.options.multiple ){
					self.labels.removeClass('ui-state-active');
					$this.closest('label').toggleClass('ui-state-active', checked );
					
					// close menu
					self.close();
				}

				// fire change on the select box
				self.element.trigger("change");
				
				// setTimeout is to fix multiselect issue #14 and #47. caused by jQuery issue #3827
				// http://bugs.jquery.com/ticket/3827 
				setTimeout($.proxy(self.update, self), 10);
			});

		// close each widget when clicking on any other element/anywhere else on the page
		$(document).bind('mousedown.multiselect', function( e ){
			if(self._isOpen && !$.contains(self.menu[0], e.target) && !$.contains(self.button[0], e.target) && e.target !== self.button[0]){
				self.close();
			}
		});

		// deal with form resets.  the problem here is that buttons aren't
		// restored to their defaultValue prop on form reset, and the reset
		// handler fires before the form is actually reset.  delaying it a bit
		// gives the form inputs time to clear.
		$(this.element[0].form).bind('reset.multiselect', function(){
			setTimeout($.proxy(self.refresh, self), 10);
		});
	},

	// set button width
	_setButtonWidth: function(){
		var width = this.element.outerWidth(),
			o = this.options;
		if((/\d/.test(o.minWidth) && width < o.minWidth) || (((o.entityname == 'cre_sys_obt_loc') || (o.entityname == 'cre_sys_obt_cnt') || (o.entityname == 'cre_org') || (o.entityname == 'cre_sys_obt_trp') || (o.entityname == 'cre_sys_obt_cls')) && o.pagefrom == 'customattr')){
			width = o.minWidth;
		}
		// set widths
		this.button.width( width );
	},
	
	// set menu width
	_setMenuWidth: function(){
		var m = this.menu,
			width = this.button.outerWidth()-
				parseInt(m.css('padding-left'),10)-
				parseInt(m.css('padding-right'),10)-
				parseInt(m.css('border-right-width'),10)-
				parseInt(m.css('border-left-width'),10);
				
		m.width( width || this.button.outerWidth() );
	},
	
	// move up or down within the menu
	_traverse: function( which, start ){
		
		var $start = $(start),
			moveToLast = which === 38 || which === 37,
			
			// select the first li that isn't an optgroup label / disabled
			$next = $start.parent()[moveToLast ? 'prevAll' : 'nextAll']('li:not(.ui-multiselect-disabled, .ui-multiselect-optgroup-label)')[ moveToLast ? 'last' : 'first']();
		
		// if at the first/last element
		if( !$next.length ){
			var $container = this.menu.find('ul').last();
			
			// move to the first/last
			this.menu.find('label')[ moveToLast ? 'last' : 'first' ]().trigger('mouseover');
			
			// set scroll position
			$container.scrollTop( moveToLast ? $container.height() : 0 );
			
		} else {
			$next.find('label').trigger('mouseover');
		}
	},

	// This is an internal function to toggle the checked property and
	// other related attributes of a checkbox.
	//
	// The context of this function should be a checkbox; do not proxy it.
	_toggleState: function( prop, flag ){
		return function(){
			if( !this.disabled ) {
				this[ prop ] = flag;
			}

			if( flag ){
				this.setAttribute('aria-selected', true);
			} else {
				this.removeAttribute('aria-selected');
			}
		};
	},

	_toggleChecked: function( flag, group ){
		var $inputs = (group && group.length) ?
			group :
			this.labels.find('input'),

			self = this;

		// toggle state on inputs
		$inputs.each(this._toggleState('checked', flag));

		// give the first input focus
		$inputs.eq(0).focus();
		
		// update button text
		this.update();
		
		// gather an array of the values that actually changed
		var values = $inputs.map(function(){
			return this.value;
		}).get();

		// toggle state on original option tags
		this.element
			.find('option')
			.each(function(){
				if( !this.disabled && $.inArray(this.value, values) > -1 ){
					self._toggleState('selected', flag).call( this );
				}
			});

		// trigger the change event on the select
		if( $inputs.length ) {
			this.element.trigger("change");
		}
	},

	_toggleDisabled: function( flag ){
		this.button
			.attr({ 'disabled':flag, 'aria-disabled':flag })[ flag ? 'addClass' : 'removeClass' ]('ui-state-disabled');
		
		this.menu
			.find('input')
			.attr({ 'disabled':flag, 'aria-disabled':flag })
			.parent()[ flag ? 'addClass' : 'removeClass' ]('ui-state-disabled');
		
		this.element
			.attr({ 'disabled':flag, 'aria-disabled':flag });
	},
	
	// open the menu
	open: function( e ){
		var self = this,
			button = this.button,
			menu = this.menu,
			speed = this.speed,
			o = this.options;
		
		// bail if the multiselectopen event returns false, this widget is disabled, or is already open 
		if( this._trigger('beforeopen') === false || button.hasClass('ui-state-disabled') || this._isOpen ){
			return;
		}
		
		var $container = menu.find('ul').last(),
			effect = o.show,
			pos = button.offset();
		
		// figure out opening effects/speeds
		if( $.isArray(o.show) ){
			effect = o.show[0];
			speed = o.show[1] || self.speed;
		}
		
		// set the scroll of the checkbox container
		$container.scrollTop(0).height(o.height);
		
		// position and show menu
		if( $.ui.position && !$.isEmptyObject(o.position) ){
			o.position.of = o.position.of || button;
			
			menu
				.show()
				.position( o.position )
				.hide()
				.show( effect, speed );
		
		// if position utility is not available...
		} else {
			menu.css({ 
				top: pos.top + button.outerHeight(),
				left: pos.left
			}).show( effect, speed );
		}
		
		// select the first option
		// triggering both mouseover and mouseover because 1.4.2+ has a bug where triggering mouseover
		// will actually trigger mouseenter.  the mouseenter trigger is there for when it's eventually fixed
		this.labels.eq(0).trigger('mouseover').trigger('mouseenter').find('input').trigger('focus');
		
		button.addClass('ui-state-active');
		this._isOpen = true;
		this._trigger('open');
	},
	
	// close the menu
	close: function(){
		if(this._trigger('beforeclose') === false){
			return;
		}
	
		var o = this.options, effect = o.hide, speed = this.speed;
		
		// figure out opening effects/speeds
		if( $.isArray(o.hide) ){
			effect = o.hide[0];
			speed = o.hide[1] || this.speed;
		}
	
		this.menu.hide(effect, speed);
		this.button.removeClass('ui-state-active').trigger('blur').trigger('mouseleave');
		this._isOpen = false;
		this._trigger('close');
	},

	enable: function(){
		this._toggleDisabled(false);
	},
	
	disable: function(){
		this._toggleDisabled(true);
	},
	
	checkAll: function( e ){
		this._toggleChecked(true);
		this._trigger('checkAll');
	},
	
	uncheckAll: function(){
		this._toggleChecked(false);
		this._trigger('uncheckAll');
	},
	
	getChecked: function(){
		return this.menu.find('input').filter('[checked]');
	},
	
	destroy: function(){
		// remove classes + data
		$.Widget.prototype.destroy.call( this );
		
		this.button.remove();
		this.menu.remove();
		this.element.show();
		
		return this;
	},
	
	isOpen: function(){
		return this._isOpen;
	},
	
	widget: function(){
		return this.menu;
	},
	
	// react to option changes after initialization
	_setOption: function( key, value ){
		var menu = this.menu;
		
		switch(key){
			case 'header':
				menu.find('div.ui-multiselect-header')[ value ? 'show' : 'hide' ]();
				break;
			case 'checkAllText':
				menu.find('a.ui-multiselect-all span').eq(-1).text(value);
				break;
			case 'uncheckAllText':
				menu.find('a.ui-multiselect-none span').eq(-1).text(value);
				break;
			case 'height':
				menu.find('ul').last().height( parseInt(value,10) );
				break;
			case 'minWidth':
				this.options[ key ] = parseInt(value,10);
				this._setButtonWidth();
				this._setMenuWidth();
				break;
			case 'selectedText':
			case 'selectedList':
			case 'noneSelectedText':
				this.options[key] = value; // these all needs to update immediately for the update() call
				this.update();
				break;
			case 'classes':
				menu.add(this.button).removeClass(this.options.classes).addClass(value);
				break;
		}
		
		$.Widget.prototype._setOption.apply( this, arguments );
	}
});

})(jQuery);;
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
