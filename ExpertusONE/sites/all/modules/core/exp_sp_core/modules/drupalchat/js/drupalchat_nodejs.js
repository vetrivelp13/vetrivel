(function ($) {
  Drupal.drupalchat = Drupal.drupalchat || {};
  Drupal.drupalchat.chatboxTimeout = Drupal.drupalchat.chatboxTimeout || {};

  Drupal.drupalchat.processChatDataNodejs = function(data) {
      var value = data;
      //console.log('In drupalchat_nodejs.js : processChatDataNodejs() : value = (see console below)');
      //console.log(value);
      if (value.length > 0) {
        // Play new message sound effect      
        var obj = swfobject.getObjectById("drupalchatbeep");
        if (obj && Drupal.settings.drupalchat.notificationSound == 1) {
          try {
            obj.drupalchatbeep(); // e.g. an external interface call
          }
          catch(e) {
          }
        }
      }

      //Add div if required.
      var chatboxtitle = '';
      var chatboxname = '';
      var chatboxfullname = '';
      if (value.displayAs == 'R') { // message is to be shown to recipient
        chatboxtitle = value.fromUserId;
        chatboxname = value.fromUsername;
        chatboxfullname = value.fromUserFullName;
      }
      else { // message is to be shown in other sessions of the sender
        chatboxtitle = value.forUserId;
        chatboxname = value.forUsername;
        chatboxfullname = value.forUserFullName;
      }

      if (jQuery("#chatbox_" + chatboxtitle).length <= 0) {
        createChatBox(chatboxtitle, chatboxfullname, 1);
      }
      else {
        var timestamp = new Date().getTime();
        //console.log('timestamp = ' + timestamp);
        jQuery("#chatbox_" + chatboxtitle).data('age', timestamp);
        
        if (jQuery("#chatbox_" + chatboxtitle).css('display') == 'none') {
          jQuery("#chatbox_" + chatboxtitle).css('display', 'block').removeClass('autohidden');
          jQuery("#chatbox_" + chatboxtitle + " .subpanel_title").click(); //Expand the chat dialog
          limitChatboxesDisplayByViewportWidth();
          jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").focus();
        }
      }
      
      // Announce new chat message to user by highlighting the chat dialog title.
      jQuery('#chatbox_' + chatboxtitle + ' .subpanel_title').addClass("newchatmsg");

      for (var msg in value.msgsList) {
        value.msgsList[msg].msg = value.msgsList[msg].msg.replace(/{{drupalchat_newline}}/g, "<br />");
        value.msgsList[msg].msg = value.msgsList[msg].msg.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\"/g,"&quot;");
        value.msgsList[msg].msg = Drupal.drupalchat.urlify(value.msgsList[msg].msg); // ticket #0020848
        if (Drupal.settings.drupalchat.enableSimleys == 1) {
          value.msgsList[msg].msg = emotify(value.msgsList[msg].msg);
        }
          
        if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .chatboxusername span.chatmsgusername:last")
                .attr('title') == value.fromUserFullName &&
              jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .chatboxusername span.chatmsgusername:last")
                .parent()
                  .parent()
                    .nextAll('.conversation-time').length <= 0) {
          if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").length > 0) {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").append(
                '<p><span>' + value.msgsList[msg].msg + '</span></p>'
            );
            var chatBoxelement = jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").jScrollPane({});
            var api = chatBoxelement.data('jsp');
            api.scrollToPercentY(100);
          }
          else {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append(
              '<p><span>' + value.msgsList[msg].msg + '</span></p>'
            );
          }
        }
        else {
  	      var fromUsername = (value.fromUsername == drupalchat_htmlspecialchars_decode(Drupal.settings.drupalchat.username, 'ENT_QUOTES'))? Drupal.settings.drupalchat.me : value.fromUserFullName;
  	      var chatBoxContentHTML = '<p>' +
  	                                 '<span class="chatboxusername">' +
  	                                   '<span class="chatmsgusername" title="'+ value.fromUserFullName +'">' + titleRestricted(fromUsername, 16) + ':&nbsp;</span>' +
  	                                 '</span>' +
  	                                 '<span>' + value.msgsList[msg].msg + '</span>';
  	                               '</p>';
  	      if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").length > 0) {
  	        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").append(chatBoxContentHTML);
            var chatBoxelement = jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").jScrollPane({});
            var api = chatBoxelement.data('jsp');
            api.scrollToPercentY(100);
  	      }
  	      else {
  	        jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append(chatBoxContentHTML);
  	      }
        }
        
        // It is possible that the sender immediately signs out after sending the message and the receiver may have been temporarily away.
        // In such a scenario, when the receiver receives the message, the sender status in chat dialog window at the receiver end should
        // be shown offline.
        if (value.hasOwnProperty('fromUserStatus')) {
          if (value.fromUserStatus == 1) {
            jQuery('#chatbox_' + value.fromUserId + ' .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0');
          }
          else {
            jQuery('#chatbox_' + value.fromUserId + ' .subpanel_title > div > div > div > div').removeClass('status-0').addClass('status-1');
          }
        }

        if (Drupal.drupalchat.chatboxTimeout.hasOwnProperty(chatboxtitle)) {
          //console.log('processChatDataNodejs: clearing timeout for ' + chatboxtitle);
          clearTimeout(Drupal.drupalchat.chatboxTimeout[chatboxtitle].timeout);
        }
        delete Drupal.drupalchat.chatboxTimeout[chatboxtitle];
        
        //console.log('processChatDataNodejs: setting timeout for ' + chatboxtitle);
        Drupal.drupalchat.chatboxTimeout[chatboxtitle] = {};
        Drupal.drupalchat.chatboxTimeout[chatboxtitle].timeout = setTimeout('Drupal.drupalchat.appendConversationTime("' + chatboxtitle + '")',
                                                                                               Drupal.settings.drupalchat.showChatTimeAfter);
        Drupal.drupalchat.chatboxTimeout[chatboxtitle].msgid = '';
      }
      
      jQuery("#chatbox_"+ chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
      jQuery.titleAlert(Drupal.settings.drupalchat.newMessage, {requireBlur:true, stopOnFocus:true, interval:800});

      
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
      
      // EXPERTUS: Tooltip for showing restricted character. 
      vtip();
      
  };
      
  Drupal.drupalchat.processUserOnlineWithInsertInfo = function(data) {
    //console.log('In Drupal.drupalchat.processUserOnline()');
    //console.log(data);
    jQuery('#chatpanel .subpanel ul li.drupalchatnousers').remove();
    
    var predecessorElement = data.insertInfo.after == null? [] : jQuery('a#drupalchat_user_' + data.insertInfo.after.uid);
    predecessorElement = predecessorElement.length == 0? null : predecessorElement.parent();
    var successorElement = data.insertInfo.before == null? [] : jQuery('a#drupalchat_user_' + data.insertInfo.before.uid);
    successorElement = successorElement.length == 0? null : successorElement.parent();
    
    var numOnlineUsers = parseInt(jQuery('#chatpanel .online-count').html());
    
    if (predecessorElement == null && successorElement == null) {
      //console.log('Both predecessor element and successor elements are null');
      var newOnlineUserName = data.name.toUpperCase();
      var greaterElementFound = false;
      var userAlreadyInList = false;
      var greaterElement = null;
      // The below 'each' loop should never get executed, as this scenario means that there are currently no users in online users list.
      // This loop is added only to confirm and preserve sort order integrity.
      jQuery('#chatpanel .subpanel ul li').each(function() {
        //console.log('in each loop');
        var elementName = $(this).text().toUpperCase();
        if (elementName == newOnlineUserName) {
          userAlreadyInList = true;
          return false; // break out of the loop
        }
        if (elementName > newOnlineUserName) {
          // insert before this element
          greaterElement = $(this);
          greaterElementFound = true;
          return false; // break out of the loop
        }
      });

      //console.log(data.onlineUserHTML);
      if (greaterElementFound) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' before ' + greaterElement.text());
        greaterElement.before(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
      else if (!userAlreadyInList) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' at the end of list.');
        jQuery('#chatpanel .subpanel ul').append(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
    }
    else if (predecessorElement == null) {
      //console.log('Only predecessor element is null');
      var prevElement = successorElement.prev();
      var newOnlineUserName = data.name.toUpperCase();
      var smallerElementFound = false;
      var userAlreadyInList = false;
      // The below while loop condition would always fail and the while loop never executed.
      // It is added to ensure that the sort order integrity is always preserved.
      while (prevElement.length != 0) {
        //console.log('in while loop');
        var elementName = $(prevElement).text().toUpperCase();
        if (elementName == newOnlineUserName) {
          userAlreadyInList = true;
          break; // break out of the loop
        }
        if (elementName < newOnlineUserName) {
          // insert before this element
          smallerElementFound = true;
          break; // break out of the loop
        }
        prevElement = prevElement.prev();
      }
      if (smallerElementFound) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' after ' + prevElement.text());
        prevElement.after(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
      else if (!userAlreadyInList) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' at the start of ul list');
        // Prepend to ul list
        jQuery('#chatpanel .subpanel ul').prepend(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
    }
    else if (successorElement == null) {
      //console.log('Only successor element is null');
      var nextElement = predecessorElement.next();
      var newOnlineUserName = data.name.toUpperCase();
      var greaterElementFound = false;
      var userAlreadyInList = false;
      // The below while loop condition would always fail and the while loop never executed.
      // It is added to ensure that the sort order integrity is always preserved.
      while (nextElement.length != 0) {
        //console.log('in while loop');
        var elementName = $(nextElement).text().toUpperCase();
        if (elementName == newOnlineUserName) {
          userAlreadyInList = true;
          break; // break out of the loop
        }
        if (elementName > newOnlineUserName) {
          // insert before this element
          greaterElementFound = true;
          break; // break out of the loop
        }
        nextElement = nextElement.next();
      }
      
      if (greaterElementFound) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' before ' + nextElement.text());
        nextElement.before(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
      else if (!userAlreadyInList) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' at the end of ul list');
        // Append to the ul list
        jQuery('#chatpanel .subpanel ul').append(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
    }
    else {
      //console.log('Neither predecessor element nor successor elements is null');
      var nextElement = predecessorElement.next();
      var newOnlineUserName = data.name.toUpperCase();
      var greaterElementFound = false;
      var userAlreadyInList = false;
      var successorElementId = $(successorElement).children('a').attr('id');
      // The below while loop condition would always fail and the while loop never executed.
      // It is added to ensure that the sort order integrity is always preserved.
      while (nextElement.length != 0 && $(nextElement).children('a').attr('id') != successorElementId) {
        //console.log('in while loop');
        var elementName = $(nextElement).text().toUpperCase();
        if (elementName == newOnlineUserName) {
          userAlreadyInList = true;
          break; // break out of the loop
        }
        if (elementName > newOnlineUserName) {
          // insert before this element
          greaterElementFound = true;
          break; // break out of the loop
        }
        nextElement = nextElement.next();
        //console.log($(nextElement).children('a').attr('id'));
      }
      
      if (greaterElementFound) {
        //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : g.e.f. inserting ' + data.name + ' before ' + nextElement.text());
        nextElement.before(data.onlineUserHTML);
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
      else if (!userAlreadyInList) {
        if (nextElement.length == 0) {
          //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' at the end of ul list');
          // Append to the ul list
          jQuery('#chatpanel .subpanel ul').append(data.onlineUserHTML);
        }
        else {
          //console.log('Drupal.drupalchat.processUserOnlineWithInsertInfo() : inserting ' + data.name + ' before ' + nextElement.text());
          nextElement.before(data.onlineUserHTML);          
        }
        jQuery('#chatpanel .signin-signout-arrow').show();
        jQuery('#chatpanel .count_class').show();
        jQuery('#chatpanel .online-count').html(numOnlineUsers + 1);
      }
    }

    numOnlineUsers = parseInt(jQuery('#chatpanel .online-count').html()); // get updated count after user insert
    
    if(numOnlineUsers >= 5){
      //jQuery('#chatpanel .subpanel .item-list').css({'height':'200','overflow-y':'auto'});
      jQuery('#chatpanel .subpanel .item-list').css({'height':'200'});
      jQuery('#chatpanel .subpanel .item-list').jScrollPane({showArrows:true});
    } /*else {
      jQuery('#chatpanel .subpanel .item-list').css({'height':'auto','overflow-y':'hidden'});
    }*/
      
    // EXPERTUS : Below code added to make user status appear green in any opened chat window with this user
    //console.log('drupalchat_nodejs.js : Drupal.drupalchat.processUserOnline() : selector = ' + '#chatbox_' + data.uid + ' .subpanel_title > div');
    jQuery('#chatbox_' + data.uid + ' .subpanel_title > div > div > div > div').removeClass('status-0').addClass('status-1');
      
    // EXPERTUS : Ensure user picture is latest in any previously present chatdialog windows
    //            There is a scenario where the 'userPictureChanged' message for the user is missed by this client,
    //            and the retrieved chatdialog from permanent storage still has the old picture.
    jQuery('.chatmsguserpic.uid' + data.uid).attr('src', '/sites/default/files/pictures/' + data.pictureFilename);
    this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(this.currTheme == "expertusoneV2"){ 
	jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'186'});
	}else
	{
	jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'175'});	
	}
    //jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'175'});
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();  // needed as picture filename is being updated above
    
    // EXPERTUS: Tooltip for showing restricted character. 
    vtip();

  };
  
  Drupal.drupalchat.processUsersOnlineList = function(data) {
    //console.log('In Drupal.drupalchat.processUsersOnlineList()');   
    jQuery('#chatpanel .subpanel ul').empty();
    jQuery('#chatpanel .subpanel ul').append(data.onlineUsersListHtml);
    jQuery('#chatpanel .signin-signout-arrow').show();
    jQuery('#chatpanel .count_class').show();
    jQuery('#chatpanel .online-count').html(data.count);
        
    if(jQuery('#chatpanel .subpanel ul > li').length >= 5) {
      //jQuery('#chatpanel .subpanel .item-list').css({'height':'200', 'overflow-y':'auto'});
    	jQuery('#chatpanel .subpanel .item-list').css({'height':'200'});
    	jQuery('#chatpanel .subpanel').css({'height':'200'});
		/* Chat Arrow up and down functionality written by 'VJ' */
		var jspPaneCount = '';
		jQuery('#chatpanel .subpanel .item-list')
			.bind(
				'jsp-initialised',
				function(event, isScrollable) {
		  				if(jQuery('#chatpanel').find('.jspDrag').height() < 30 ) {
		  					jQuery('#chatpanel').find('.jspDrag').css('height','30px');
		  				} 
				}
			)
			.bind(
				'jsp-scroll-y',
				function(event, scrollPositionY, isAtTop, isAtBottom) {
					var scrollBarCount = Math.round(scrollPositionY);
					if(scrollBarCount == 0){
						jspPaneCount = 10;
						return false;
					}
					if(isAtBottom != true) {
						if(scrollBarCount > jspPaneCount) {
							jQuery('#chatpanel').find('.jspPane').css('top',-scrollBarCount-10-parseInt(Drupal.settings.drupalchat.onlineUsersListScrollingSetting)+'px');
							jspPaneCount = scrollBarCount+10+parseInt(Drupal.settings.drupalchat.onlineUsersListScrollingSetting);
							return false;
						}
						if(scrollBarCount < jspPaneCount) {
							jQuery('#chatpanel').find('.jspPane').css('top',-scrollBarCount+10+parseInt(Drupal.settings.drupalchat.onlineUsersListScrollingSetting)+'px');
							jspPaneCount = scrollBarCount-10-parseInt(Drupal.settings.drupalchat.onlineUsersListScrollingSetting);
							return false;
						}
					}
				}
			) 
			.jScrollPane({ 
				showArrows:true,
				autoReinitialise: true
			});
}
	
    
    /*else {
      jQuery('#chatpanel .subpanel .item-list').css({'height':'auto', 'overflow-y':'hidden'});
    }*/
      
    // EXPERTUS : Below code added to make user status appear green in any opened chat window with this user
    jQuery('.chatbox').each(function() {
      var chatboxId = $(this).attr('id').replace('chatbox_', '');
      //console.log(chatboxId);
      //console.log(data.onlineUsersList);
      if (data.onlineUsersList.hasOwnProperty(chatboxId)) {
        jQuery('#chatbox_' + chatboxId + ' .subpanel_title > div > div > div > div').removeClass('status-0').addClass('status-1');
        
        // EXPERTUS : Ensure user picture is latest in any previously present chatdialog windows
        //            There is a scenario where the 'userPictureChanged' message for the user is missed by this client,
        //            and the retrieved chatdialog from permanent storage still has the old picture.
        jQuery('.chatmsguserpic.uid' + chatboxId).attr('src', '/sites/default/files/pictures/' + data.onlineUsersList[chatboxId].pictureFilename);
      }
    });
    this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(this.currTheme == "expertusoneV2"){ 
	jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'186'});
	}else
	{
	jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'175'});	
	}
    //jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'175'});
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage(); // needed as picture filename is being updated above
    
    // EXPERTUS: Tooltip for showing restricted character. 
    vtip();
    
  };

  Drupal.drupalchat.processUserPicture = function(data) {
    //console.log(data);
    //console.log('In Drupal.drupalchat.processUserPicture()');
    if (data.uid == Drupal.settings.drupalchat.uid) {
      // update my picture in Drupal.settings.
      Drupal.drupalchat.userPictureFileName = data.pictureFilename;
    }
    else {
      // Update user's picture in the online users list
      jQuery('.onlineuserpic.uid' + data.uid).attr('src', '/sites/default/files/pictures/' + data.pictureFilename);
    }
    
    // Update user picture in chat boxes
    jQuery('.chatmsguserpic.uid' + data.uid).attr('src', '/sites/default/files/pictures/' + data.pictureFilename);
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
  };
  
  Drupal.drupalchat.processNoReceiverForMsg = function(undeliveredMsgs) {
    //console.log('In Drupal.drupalchat.processNoReceiverForMsg()');
    //console.log(undeliveredMsgs);
    var forUserId = undeliveredMsgs.uid2;
    var msgIdList = undeliveredMsgs.msgIdList;
    for (var msgId in msgIdList) {
      //console.log('Drupal.drupalchat.processNoReceiverForMsg : msgId = ' + msgId + ' ' + msgIdList[msgId]);
      // If a timeout is set for this message to print the sent time, clear it
      //console.log('Drupal.drupalchat.chatboxTimeout');
      //console.log(Drupal.drupalchat.chatboxTimeout);
      if (Drupal.drupalchat.chatboxTimeout.hasOwnProperty(forUserId) && Drupal.drupalchat.chatboxTimeout[forUserId].msgid == msgIdList[msgId]) {
        //console.log('processNoReceiverForMsg : clearing timeout for ' + forUserId);
        clearTimeout(Drupal.drupalchat.chatboxTimeout[forUserId].timeout);
        delete Drupal.drupalchat.chatboxTimeout[forUserId];
      }
      
      // Highlight the undelivered message
      jQuery('.' + msgIdList[msgId]).addClass('undelivered');
      
      // If undelivered error message preceeds this message remove it
      var prevPElement = jQuery('.' + msgIdList[msgId]).parent().prev();
      //console.log('prevPElement');
      //console.log(prevPElement);
      if (prevPElement.length > 0 && prevPElement.hasClass('undelivered-errmsg')) {
        //console.log('removing any preceeding undelivered error message');
        prevPElement.remove();
      }
      
      // If sent at text is present after this message remove it
      var nextPElement = jQuery('.' + msgIdList[msgId]).parent().next();
      //console.log('nextPElement');
      //console.log(nextPElement);
      if (nextPElement.length > 0 && nextPElement.hasClass('conversation-time')) {
        //console.log('removing sent at message');
        nextPElement.remove();
      }
    }
    
    if (msgIdList.length > 0) {
      //console.log(jQuery('#chatbox_' + forUserId + ' .chatboxhead .subpanel_title_text'));
      jQuery('.' + msgIdList[msgId]).parent().after(
        '<p class="undelivered-errmsg">' +
          '<i>' + jQuery('#chatbox_' + forUserId + ' .chatboxhead .subpanel_title_text').text() + '</i>' +
          ' ' + Drupal.settings.drupalchat.undeliveredErrorMsg +
        '</p>'
      );
    }

    // Scroll down to ensure last message is showing
    if (jQuery("#chatbox_" + forUserId + " .chatboxcontent .jspPane").length > 0) {
      var chatBoxelement = jQuery("#chatbox_" + forUserId + " .chatboxcontent").jScrollPane({});
      var api = chatBoxelement.data('jsp');
      api.scrollToPercentY(100);
    }
    jQuery("#chatbox_" + forUserId + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + forUserId + " .chatboxcontent")[0].scrollHeight);

    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
    
    var obj = swfobject.getObjectById("drupalchatbeep");
    if (obj && Drupal.settings.drupalchat.notificationSound == 1) {
      try {
        obj.drupalchatbeep(); // e.g. an external interface call
      }
      catch(e) {
      }
    }
  };

Drupal.drupalchat.processUserOffline = function(data) {
  //console.log('In Drupal.drupalchat.processUserOffline()');
  if (data != Drupal.settings.drupalchat.uid) {
    if (jQuery("#drupalchat_user_" + data).length > 0) {
      jQuery("#drupalchat_user_" + data).parent().remove();
      var numOnlineUsers = parseInt(jQuery('#chatpanel .online-count').html()) - 1;
      jQuery('#chatpanel .signin-signout-arrow').show();
      jQuery('#chatpanel .count_class').show();
      jQuery('#chatpanel .online-count').html(numOnlineUsers);
      if (numOnlineUsers == 0) {
        jQuery('#chatpanel .subpanel > .item-list').remove();
        if (Drupal.settings.drupalchat.signInState == 2) {
          jQuery('#chatpanel .subpanel').append(Drupal.settings.drupalchat.noUsers);
        }
        else {
          jQuery('#chatpanel .subpanel').append(Drupal.settings.drupalchat.signedOut);
        }
      }
    }
    
    // EXPERTUS : Below code added to make user status appear grey in any opened chat window with this user
    //console.log('drupalchat_nodejs.js : Drupal.drupalchat.processUserOnline() : selector = ' + '#chatbox_' + data.uid + ' .subpanel_title > div');
    jQuery('#chatbox_' + data + ' .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0');
    if(numOnlineUsers <= 5){
	    var element = $('#chatpanel .subpanel .item-list').jScrollPane({showArrows:true});
	    var api = element.data('jsp');
	    api.destroy();
	    jQuery('#chatpanel .subpanel .item-list').css({'height':'auto'});
    }
  }
  
  // EXPERTUS: Save details to persistent storage.
  expSaveToPersistentStorage();
};

Drupal.drupalchat.processSignedIn = function(data) {
  //console.log(data);
  //console.log('In Drupal.drupalchat.processSignedIn()');
    Drupal.settings.drupalchat.signInState = 2;
    jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "online.png");
    $('#drupalchat').find('.chat-signin-bg-setting').removeClass('chat-signin-bg-setting');
    $('#drupalchat').closest('#drupalchat-wrapper').removeClass('offline');
    jQuery('#chatpanel .count_class').show();
    jQuery('#chatpanel .signin-signout-arrow').show();
    $('#drupalchat .change-signin-state').html(Drupal.settings.drupalchat.signOutOfChat);
    $('#drupalchat .change-signin-state').attr('title', Drupal.settings.drupalchat.signOutOfChat);
    $('#drupalchat .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signOutOfChat);
    if (data.count == 0) {
      jQuery('#chatpanel .online-count').html(0);
      jQuery('#chatpanel .subpanel > .item-list').remove();
      jQuery('#chatpanel .subpanel').append(Drupal.settings.drupalchat.noUsers);
      
      // To confirm code - copied from processUserOffline() above.
      var element = $('#chatpanel .subpanel .item-list').jScrollPane({showArrows:true});
      var api = element.data('jsp');
      api.destroy();
      jQuery('#chatpanel .subpanel .item-list').css({'height':'auto'});
      
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
    }
    else {
      Drupal.drupalchat.processUsersOnlineList(data);
    }
};

Drupal.drupalchat.processSignedOut = function(data) {
  //console.log(data);
  //console.log('In Drupal.drupalchat.signedOut()');
  
  Drupal.settings.drupalchat.signInState = 1;
  jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "offline.png");
  $('#drupalchat').find('.signin-signout-container').addClass('chat-signin-bg-setting');
  $('#drupalchat').closest('#drupalchat-wrapper').addClass('offline');
  $('#drupalchat .change-signin-state').html(Drupal.settings.drupalchat.signIntoChat);
  $('#drupalchat .change-signin-state').attr('title', Drupal.settings.drupalchat.signIntoChat);
  $('#drupalchat .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signIntoChat);
  
  jQuery('#chatpanel .count_class').hide();
  jQuery('#chatpanel .signin-signout-arrow').hide();
  //jQuery('#chatpanel .online-count').html(0);
  jQuery('#chatpanel .subpanel > .item-list').remove();
  jQuery('#chatpanel .subpanel').append(Drupal.settings.drupalchat.signedOut);
  
  jQuery('.chatbox .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0');

  // To confirm code - copied from processUserOffline() above.
  var element = $('#chatpanel .subpanel .item-list').jScrollPane({showArrows:true});
  var api = element.data('jsp');
  api.destroy();
  jQuery('#chatpanel .subpanel').css({'height':'auto'});
  jQuery('#chatpanel .subpanel .item-list').css({'height':'auto'});
  
  // EXPERTUS: Save details to persistent storage.
  expSaveToPersistentStorage();
  
};

Drupal.drupalchat.processLoggedOut = function(data) {
  //console.log('logged out');
  if (typeof resource.base_url != undefined) {
    window.location.replace(resource.base_url);
  }
};

Drupal.drupalchat.sendMessagesNodejs = function(uid1, msgObjList) {
  //console.log('In Drupal.drupalchat.sendMessagesNodejs');
  Drupal.Nodejs.socket.emit('message', {type:    'newMessage',
                                        uid1: uid1,
                                        msgObjList : msgObjList});
};

Drupal.Nodejs.callbacks.drupalchatNodejsMessageHandler = {
  callback: function (message) {
    //console.log(message);
    //console.log('Drupal.Nodejs.callbacks.drupalchatNodejsMessageHandler callback() message = (see console) ');
    if (typeof Drupal.settings.drupalchat === 'undefined') {
      //console.log('Drupal.settings.drupalchat is undefined');
      return;
    }
    
    switch (message.type) {
      case 'newMessage':
        //Drupal.drupalchat.processChatDataNodejs(jQuery.parseJSON(message.data));
        Drupal.drupalchat.processChatDataNodejs(message.data);
        break;

      case 'noReceiverForMsg':
        Drupal.drupalchat.processNoReceiverForMsg(message.data);
        break;
        
      case 'userOnline':
        Drupal.drupalchat.processUserOnlineWithInsertInfo(message.data);
        break;
        
      case 'onlineUsersList':
        Drupal.drupalchat.processUsersOnlineList(message.data);
        break;

      case 'userPictureChanged':
        Drupal.drupalchat.processUserPicture(message.data);
        break;

      case 'userOffline':
        Drupal.drupalchat.processUserOffline(message.data);
        break;
        
      case 'signedIn':
        Drupal.drupalchat.processSignedIn(message.data);
        break;
        
      case 'signedOut':
        Drupal.drupalchat.processSignedOut(message.data);
        break;

      case 'loggedOut':
        Drupal.drupalchat.processLoggedOut(message.data);
        break;
    }
    
    if (message.hasOwnProperty('ack') && message.ack === true) {
      Drupal.Nodejs.socket.emit('message-processed', {});
    }
  }
};

// EXPERTUS: Added drupalchatNodejsConnectionSetupHandlers
Drupal.Nodejs.connectionSetupHandlers.drupalchatNodejsConnectionSetupHandlers = {
    disconnect: function () {
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }

      //console.log('drupalchat_nodejs.js : In drupalchatNodejsConnectionSetupHandlers : disconnect handler');

      // Update UI status
      if (Drupal.settings.drupalchat.signInState == 2) {
        $('#chatpanel .subpanel .item-list').replaceWith(Drupal.settings.drupalchat.noUsers);
      }
      else {
        $('#chatpanel .subpanel .item-list').replaceWith(Drupal.settings.drupalchat.signedOut);
      }
      jQuery('#chatpanel .online-count').html(0); // Set online user count to 0
      
      // Make user status to be grey in all opened chat windows
      jQuery('.chatbox .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0');
      
      jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "loading.gif");

      // NOTE: Do not save to permanent storage now. Saved later in connectionFailure if not a page reload.
    },
    
    connecting : function () {
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }
      
      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : connecting hander');
      jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "loading.gif");
    },
    
    connect : function() {
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }
      
      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : connect hander');
    },
    
    connectionFailure : function() {      
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }

      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : connectionFailure handler');
      $('#drupalchat').closest('#drupalchat-wrapper').addClass('offline');
      jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "offline.png");
      jQuery('#chatpanel .count_class').hide();
      jQuery('#chatpanel .signin-signout-arrow').hide();
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
    },
    
    connectionFatalFailure : function() {
      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : connectionFatalFailure handler');
      $('#drupalchat').closest('#drupalchat-wrapper').addClass('offline');
      jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "offline.png");
      jQuery('#chatpanel .count_class').hide();
      jQuery('#chatpanel .signin-signout-arrow').hide();
      //@TODO: "#chatpanel .icon" can be changed here to indicate no more reconnect attempts will be made and the user will have to refresh page.
      
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
    },
    
    expAuthenticated : function(data) {
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }
      
      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : expAuthenticated handler');
      if (Drupal.settings.drupalchat.signInState == 2) {
        jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "online.png");
        jQuery('#drupalchat .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signOutOfChat);
      }
      else {
    	$('#drupalchat').closest('#drupalchat-wrapper').addClass('offline');
        jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "offline.png");
        jQuery('#chatpanel .count_class').hide();
        jQuery('#chatpanel .signin-signout-arrow').hide();
        jQuery('#drupalchat .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signIntoChat);
      }
      
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
    },
    
    expAuthFailed : function(data) {
      if (typeof Drupal.settings.drupalchat === 'undefined') {
        //console.log('Drupal.settings.drupalchat is undefined');
        return;
      }
      
      //console.log('drupalchat_nodejs.js : drupalchatNodejsConnectionSetupHandlers : expAuthFailed handler');
      $('#drupalchat').closest('#drupalchat-wrapper').addClass('offline');
      jQuery('#chatpanel .count_class').hide();
      jQuery('#chatpanel .signin-signout-arrow').hide();
      jQuery("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "offline.png");
      
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
    }
  };

})(jQuery);

