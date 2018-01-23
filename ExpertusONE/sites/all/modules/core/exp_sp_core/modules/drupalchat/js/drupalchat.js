(function ($) {

  Drupal.drupalchat = Drupal.drupalchat || {};
  Drupal.drupalchat.chatboxTimeout = Drupal.drupalchat.chatboxTimeout || {};

  Drupal.drupalchat.saveAndSendMessage = function(uid2, msgId, msg) {
    //console.log('In Drupal.drupalchat.saveAndSendMessage() with uid2 = ' + uid2);
    Drupal.drupalchat.msgqueue = Drupal.drupalchat.msgqueue || [];
    var entry;
    var found = false;
    for (entry in Drupal.drupalchat.msgqueue) {
      //console.log('In loop entry = ' + entry);
      //console.log('In loop Drupal.drupalchat.msgqueue[entry].uid2 = ' + Drupal.drupalchat.msgqueue[entry].uid2);
      if (Drupal.drupalchat.msgqueue[entry].uid2 == uid2) {
        found = true;
        break;
      }
    }

    if (found) {
      //console.log('appending to existing entry for uid2 = ' + uid2);
      //console.log('entry = ' + entry);
      Drupal.drupalchat.msgqueue[entry].msgsList.push({id:msgId, msg: msg});
    }
    else {
      //console.log('pushing new entry for uid2 = ' + uid2);
      Drupal.drupalchat.msgqueue.push({uid2: uid2, msgsList: [{id:msgId, msg: msg}]});
    }
    //console.log(Drupal.drupalchat.msgqueue);
    //console.log('Drupal.drupalchat.msgqueue');
    
    if (Drupal.drupalchat.sending) {
      return;
    }
    
    var processMsgQueue = function() {
      Drupal.drupalchat.sending = true;
      var msgObjList = Drupal.drupalchat.msgqueue;
      //console.log('Resetting Drupal.drupalchat.msgqueue');
      Drupal.drupalchat.msgqueue = [];
      //console.log(msgObjList);
      //console.log('msgObjList');
      
      // The below ajax post acts as a session timeout check or session refresh when saveMsgs is turned off (2).
      var data = Drupal.settings.drupalchat.saveMsgs == 2? {} : {
        drupalchat_msgs: msgObjList
      };
      //console.log(data);
      //console.log('data');
      
      $.ajax({
        type: 'POST',
        url: Drupal.settings.drupalchat.saveMsgUrl,
        timeout: Drupal.settings.drupalchat.sessionCheckTimeout,
        data: data,
        
        success: function(data) {
          Drupal.drupalchat.sendMessagesNodejs(Drupal.settings.drupalchat.uid, msgObjList);
          if (Drupal.drupalchat.msgqueue.length > 0) {
            processMsgQueue();
          }
          else {
            delete Drupal.drupalchat.sending;
          }
        },
        
        error: function(data) {
          var i;
          var msgObj;
          var msg;
          for (i = 0; i < msgObjList.length; i++) {
            msgObj = msgObjList[i];
            //console.log('msgObj = ');
            //console.log(msgObj);
            var undeliveredMsgs = {};
            undeliveredMsgs.uid2 = msgObj.uid2;
            undeliveredMsgs.msgIdList = [];
            for (msg in msgObj.msgsList) {
              undeliveredMsgs.msgIdList.push(msgObj.msgsList[msg].id);
            }
            //console.log('undeliveredMsgs = ');
            //console.log(undeliveredMsgs);
            Drupal.drupalchat.processNoReceiverForMsg(undeliveredMsgs);
          }
          
          // Invalidate other messages in the queue as well.
          msgObjList = Drupal.drupalchat.msgqueue;
          //console.log('Resetting Drupal.drupalchat.msgqueue');
          Drupal.drupalchat.msgqueue = [];
          for (i = 0; i < msgObjList.length; i++) {
            msgObj = msgObjList[i];
            //console.log('msgObj other = ');
            //console.log(msgObj);
            var undeliveredMsgs = {};
            undeliveredMsgs.uid2 = msgObj.uid2;
            undeliveredMsgs.msgIdList = [];
            for (msg in msgObj.msgsList) {
              undeliveredMsgs.msgIdList.push(msgObj.msgsList[msg].id);
            }
            //console.log('undeliveredMsgs other = ');
            //console.log(undeliveredMsgs);
            Drupal.drupalchat.processNoReceiverForMsg(undeliveredMsgs);            
          }
          
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
          
          delete Drupal.drupalchat.sending;
        }
    });
    };
    
    processMsgQueue();  
  };
  
  Drupal.drupalchat.checkChatBoxInputKey = function(event, chatboxtextarea, chatboxtitle) {
    //console.log('Event handler for input key in chatboxtextarea');
   
    // Unhighlight chat dialog window title when user starts typing in chatboxtextarea
    if (jQuery(chatboxtextarea).parent().siblings('.subpanel_title').hasClass('newchatmsg')) {
      //console.log('removing newchatmsg class');
      jQuery(chatboxtextarea).parent().siblings('.subpanel_title').removeClass('newchatmsg');
    }
    
    if (event.keyCode == 13 && event.shiftKey == 0)  {
      var message;
      //alert('Drupal.drupalchat.checkChatBoxInputKey() : Processing enter key');
  	  message = jQuery(chatboxtextarea).val();
  	  message = message.replace(/^\s+|\s+$/g,"");
  	  //message = message.substr(0,255); // EXPERTUS: Removed limit on number of characters
  	  message = Drupal.drupalchat.removeStopWords(message); // EXPERTUS added
  	  jQuery(chatboxtextarea).val('');
  	  jQuery(chatboxtextarea).focus();
  	  //Height is not applying based on theme so moved code to css file 
  	 /* if(Drupal.settings.ajaxPageState.theme == "expertusoneV2")
  	  {
  	    jQuery(chatboxtextarea).css('height','40px');
  	  }else
  	  {
  	    jQuery(chatboxtextarea).css('height','44px');
  	  }*/
  	  
  	  if (message != '') {
  	    //console.log('chatboxtitle = ' + chatboxtitle);
        drupalchat.send_current_uid2 = chatboxtitle;
        drupalchat.send_current_message = message;
        drupalchat.send_current_message = drupalchat.send_current_message.replace(/\n/g, "{{drupalchat_newline}}"); // EXPERTUS added
          
        var d = new Date();
        drupalchat.send_current_message_id = 'm_' + Drupal.settings.drupalchat.uid + '_' + drupalchat.send_current_uid2 + '_' + d.getTime();
        Drupal.drupalchat.saveAndSendMessage(drupalchat.send_current_uid2, drupalchat.send_current_message_id, drupalchat.send_current_message);
        
  	    message = message.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\"/g,"&quot;").replace(/\n/g, "<br />");  // EXPERTUS added <br /> for /n
  	    message = Drupal.drupalchat.urlify(message); // ticket #0020848
  	    if (Drupal.settings.drupalchat.enableSimleys == 1) {
          message = emotify(message);
  	    }
        if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .chatboxusername .chatmsgusername:last")
                .attr('title') == drupalchat_htmlspecialchars_decode(Drupal.settings.drupalchat.userfullname, 'ENT_QUOTES') &&
              jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .chatboxusername .chatmsgusername:last")
                .parent()
                  .parent()
                    .nextAll('.conversation-time, .undelivered-errmsg').length <= 0) {
          if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").length > 0) {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").append(
                '<p><span class=\'' + drupalchat.send_current_message_id + '\'>' + message + '</span></p>'
            );
            var chatBoxelement = jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").jScrollPane({});
            var api = chatBoxelement.data('jsp');
            api.scrollToPercentY(100);
          }
          else {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append(
                '<p><span class=\'' + drupalchat.send_current_message_id + '\'>' + message + '</span></p>'
            );            
          }
        }
        else {
          var chatboxContent = '<p>' +
                                 '<span class="chatboxusername">' +
                                   '<span class="chatmsgusername" title="' + Drupal.settings.drupalchat.userfullname + '">' + Drupal.settings.drupalchat.me + ':&nbsp;</span>' +
                                 '</span>' +
                                 '<span class=\'' + drupalchat.send_current_message_id + '\'>' + message + '</span>' +
                               '</p>';
          if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").length > 0) {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").append(chatboxContent);
            var chatBoxelement = jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").jScrollPane({});
            var api = chatBoxelement.data('jsp');
            api.scrollToPercentY(100);
          }
          else {
            jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append(chatboxContent);
          }
        }
        
        if (Drupal.drupalchat.chatboxTimeout.hasOwnProperty(chatboxtitle)) {
          //console.log('checkChatBoxInputKey: clearing timeout for ' + chatboxtitle);
          clearTimeout(Drupal.drupalchat.chatboxTimeout[chatboxtitle].timeout);
        }
        delete Drupal.drupalchat.chatboxTimeout[chatboxtitle];
        
        //console.log('checkChatBoxInputKey: setting timeout for ' + chatboxtitle);
        Drupal.drupalchat.chatboxTimeout[chatboxtitle] = {};
        Drupal.drupalchat.chatboxTimeout[chatboxtitle].timeout = 
          setTimeout('Drupal.drupalchat.appendConversationTime("' + chatboxtitle + '")',
                                                                            Drupal.settings.drupalchat.showChatTimeAfter);
        Drupal.drupalchat.chatboxTimeout[chatboxtitle].msgid = drupalchat.send_current_message_id;
        
        var timestamp = new Date().getTime();
        //console.log('timestamp = ' + timestamp);
        jQuery("#chatbox_" + chatboxtitle).data('age', timestamp);

  		  jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
  	  }
  	  
      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
      
      // EXPERTUS: Tooltip for showing restricted character. 
      vtip();
      
      return false;
	  }

    var adjustedHeight = chatboxtextarea.clientHeight;
    var maxHeight = 94;

    if (maxHeight > adjustedHeight) {
      adjustedHeight = Math.max(chatboxtextarea.scrollHeight, adjustedHeight);
      if (maxHeight)
        adjustedHeight = Math.min(maxHeight, adjustedHeight);
      if (adjustedHeight > chatboxtextarea.clientHeight)
        jQuery(chatboxtextarea).css('height', adjustedHeight + 8 + 'px');
    }
	  else {
      jQuery(chatboxtextarea).css('overflow', 'auto');
    }
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();

    return true;
  };
  
  Drupal.drupalchat.changeStatus = function(id, state) {
    //alert('In Drupal.drupalchat.changeStatus()');
  	if(state == 1) {
  	  jQuery('#' + id + ' .subpanel_title > div > div > div > div').removeClass('status-0').addClass('status-1');
  	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','none');
  	}
  	else if(state == 0) {
  	  jQuery('#' + id + ' .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0');
  	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','block');
  	}
  };
  
  Drupal.drupalchat.removeStopWords = function(message) {
    if (Drupal.settings.drupalchat.useStopWordList != 1) {
      if (typeof Drupal.drupalchat.stopWords === 'undefined' ) {
        Drupal.drupalchat.stopWords = Drupal.settings.drupalchat.stopWordList.split(",");
        // sort the array desc so that superstrings are checked and replaced before substrings
        Drupal.drupalchat.stopWords.sort(function(a, b) {
          a = a.toLowerCase();
          b = b.toLowerCase();
          return a < b? 1 : (a > b ? -1 : 0);
        });
      }
      
      for (var i = 0; i < Drupal.drupalchat.stopWords.length; i++) {
        var re = new RegExp(Drupal.drupalchat.stopWords[i], "ig");
        message = message.replace(re, "");
      }
    }
    
    return message;
  };
  
  // EXPERTUS added function. ref: http://stackoverflow.com/questions/1500260/detect-urls-in-text-with-javascript
  // Makes http:// and https:// prefixed text a hyperlink
  Drupal.drupalchat.urlify = function (text) {
    var urlRegex = /(https?:\/\/[^\s]+)/g;
    return text.replace(urlRegex, function(url) {
        return '<a href="' + url + '" target="_blank">' + url + '</a>';
    });
  };
  
  Drupal.drupalchat.appendConversationTime = function(chatboxtitle) {
    //console.log('Drupal.drupalchat.appendConversationTime');
    //console.log('appendConversationTime: executing timeout for ' + chatboxtitle);
    delete Drupal.drupalchat.chatboxTimeout[chatboxtitle]; // cleanup
    var currentTime = new Date();
    var hours = currentTime.getHours();
    var minutes = currentTime.getMinutes();
    
    var ampm = Drupal.settings.drupalchat.chatTimeSuffixAM;
    if (hours == 12) {
      ampm = Drupal.settings.drupalchat.chatTimeSuffixPM;
    }
    else if (hours > 12) {
      hours = hours - 12;
      ampm = Drupal.settings.drupalchat.chatTimeSuffixPM;
    }
    
    if (hours < 10) {
      hours = "0" + hours;
    }
    if (minutes < 10) {
      minutes = "0" + minutes;
    }
    
    if (jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").length > 0) {
      jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent .jspPane").append(
            '<p class="conversation-time">' + Drupal.settings.drupalchat.chatTimePrefix + '&nbsp;' + hours + ':' + minutes + '&nbsp;' + ampm + '</p>'
      );
      var chatBoxelement = jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").jScrollPane({});
      var api = chatBoxelement.data('jsp');
      api.scrollToPercentY(100);
    }
    else {
      jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent").append(
          '<p class="conversation-time">' + Drupal.settings.drupalchat.chatTimePrefix + '&nbsp;' + hours + ':' + minutes + '&nbsp;' + ampm + '</p>'
      );      
    }
    
    jQuery("#chatbox_"+ chatboxtitle + " .chatboxcontent").scrollTop(jQuery("#chatbox_" + chatboxtitle + " .chatboxcontent")[0].scrollHeight);
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
  };

})(jQuery);

var drupalchat = {
	username: null,
	uid: null,
	send_current_message: null,
	send_current_message_id: null,
	last_timestamp: 0,
	send_current_uid2: 0,
	attach_messages_in_queue: 0,
	running: 0,
	online_users: 0,
  smilies: { /*
    smiley     image_url          title_text              alt_smilies           */
    "O:-)":  [ "25.gif",          "angel",                "O:)"                 ],
    ":))":   [ "21.gif",          "laughing",             ":-))"                ],
    ":)":    [ "1.gif",           "happy",                ":-)"                 ],
    ";)":    [ "3.gif",           "winking",              ";-)"                 ],
    ":D":    [ "4.gif",           "big grin",             ":-D"                 ],
    ";;)":   [ "5.gif",           "batting eyelashes"                           ],
    //">:D<":  [ "6.gif",           "big hug"                                     ],
    ":-/":   [ "7.gif",           "confused",             ":/"                  ],
    ":x":    [ "8.gif",           "love struck",          ":X"                  ],
    //":\">":  [ "9.gif",           "blushing"                                    ],
    ":P":    [ "10.gif",          "tongue",               ":p", ":-p", ":-P"    ],
    //":-*":   [ "11.gif",          "kiss",                 ":*"                  ],
    "=((":   [ "12.gif",          "broken heart"                                ],
    ":-O":   [ "13.gif",          "surprise",             ":O"                  ],
    "X(":    [ "14.gif",          "angry"                                       ],
    //":>":    [ "15.gif",          "smug"                                        ],
    "B-)":   [ "16.gif",          "cool"                                        ],
    ":-S":   [ "17.gif",          "worried"                                     ],
    "#:-S":  [ "18.gif",          "whew!",                "#:-s"                ],
    //">:)":   [ "19.gif",          "devil",                ">:-)"                ],
    ":((":   [ "20.gif",          "crying",               ":-((", ":'(", ":'-(" ],
    ":(":    [ "2.gif",           "sad",                  ":-("                 ],
    ":|":    [ "22.gif",          "straight face",        ":-|"                 ],
    "/:)":   [ "23.gif",          "raised eyebrow",       "/:-)"                ],
    "=))":   [ "24.gif",          "rolling on the floor"                        ],
    ":-B":   [ "26.gif",          "nerd"                                        ],
    "=;":    [ "27.gif",          "talk to the hand"                            ],
    "I-)":   [ "28.gif",          "sleepy"                                      ],
    "8-|":   [ "29.gif",          "rolling eyes"                                ],
    "L-)":   [ "30.gif",          "loser"                                       ],
    ":-&":   [ "31.gif",          "sick"                                        ],
    ":-$":   [ "32.gif",          "don't tell anyone"                           ],
    "[-(":   [ "33.gif",          "not talking"                                 ],
    ":O)":   [ "34.gif",          "clown"                                       ],
    "8-}":   [ "35.gif",          "silly"                                       ],
    //"<:-P":  [ "36.gif",          "party",                "<:-p"                ],
    "(:|":   [ "37.gif",          "yawn"                                        ],
    //"=P~":   [ "38.gif",          "drooling"                                    ],
    ":-?":   [ "39.gif",          "thinking"                                    ],
    "#-o":   [ "40.gif",          "d'oh",                 "#-O"                 ],
    //"=D>":   [ "41.gif",          "applause"                                    ],
    ":-SS":  [ "42.gif",          "nailbiting",           ":-ss"                ],
    //"@-)":   [ "43.gif",          "hypnotized"                                  ],
    //":^o":   [ "44.gif",          "liar"                                        ],
    ":-w":   [ "45.gif",          "waiting",              ":-W"                 ],
    /*":-<":   [ "46.gif",          "sigh"                                        ],
    ">:P":   [ "47.gif",          "phbbbbt",              ">:p"                 ],
    "<):)":  [ "48.gif",          "cowboy"                                      ],
    ":@)":   [ "49.gif",          "pig"                                         ],
    "3:-O":  [ "50.gif",          "cow",                  "3:-o"                ],
    ":(|)":  [ "51.gif",          "monkey"                                      ],
    "~:>":   [ "52.gif",          "chicken"                                     ],
    "@};-":  [ "53.gif",          "rose"                                        ],
    "%%-":   [ "54.gif",          "good luck"                                   ],
    "**==":  [ "55.gif",          "flag"                                        ],
    "(~~)":  [ "56.gif",          "pumpkin"                                     ],
    "~O)":   [ "57.gif",          "coffee"                                      ],
    "*-:)":  [ "58.gif",          "idea"                                        ],
    "8-X":   [ "59.gif",          "skull"                                       ],
    "=:)":   [ "60.gif",          "bug"                                         ],
    ">-)":   [ "61.gif",          "alien"                                       ],
    ":-L":   [ "62.gif",          "frustrated",           ":L"                  ],
    "[-O<":  [ "63.gif",          "praying"                                     ],
    "$-)":   [ "64.gif",          "money eyes"                                  ],
    ":-\"":  [ "65.gif",          "whistling"                                   ],
    "b-(":   [ "66.gif",          "feeling beat up"                             ],
    ":)>-":  [ "67.gif",          "peace sign"                                  ],
    "[-X":   [ "68.gif",          "shame on you"                                ],
    "\\:D/": [ "69.gif",          "dancing"                                     ],
    ">:/":   [ "70.gif",          "bring it on"                                 ],
    ";))":   [ "71.gif",          "hee hee"                                     ],
    "o->":   [ "72.gif",          "hiro"                                        ],
    "o=>":   [ "73.gif",          "billy"                                       ],
    "o-+":   [ "74.gif",          "april"                                       ],
    "(%)":   [ "75.gif",          "yin yang"                                    ],
    ":-@":   [ "76.gif",          "chatterbox"                                  ],
    "^:)^":  [ "77.gif",          "not worthy"                                  ],
    ":-j":   [ "78.gif",          "oh go on"                                    ],
    "(*)":   [ "79.gif",          "star"                                        ],*/
    ":)]":   [ "100.gif",         "on the phone"                                ],
    ":-c":   [ "101.gif",         "call me"                                     ],
    //"~X(":   [ "102.gif",         "at wits' end"                                ],
    ":-h":   [ "103.gif",         "wave"                                        ],
    ":-t":   [ "104.gif",         "time out"                                    ],
    /*"8->":   [ "105.gif",         "daydreaming"                                 ],
    ":-??":  [ "106.gif",         "I don't know"                                ],
    "%-(":   [ "107.gif",         "not listening"                               ],
    ":o3":   [ "108.gif",         "puppy dog eyes"                              ],
    "X_X":   [ "109.gif",         "I don't want to see",  "x_x"                 ],*/
    ":!!":   [ "110.gif",         "hurry up!"                                   ],
    //"\\m/":  [ "111.gif",         "rock on!"                                    ],
    ":-q":   [ "112.gif",         "thumbs down"                                 ],
    ":-bd":  [ "113.gif",         "thumbs up"                                   ],
    "^#(^":  [ "114.gif",         "it wasn't me"                                ]
    /*":bz":   [ "115.gif",         "bee"                                         ],
    ":ar!":  [ "pirate.gif",      "pirate"                                      ],
    "[..]":  [ "transformer.gif", "transformer"                                 ]*/
  }
};
//(function ($) {

jQuery(document).ready(function() {
  //console.log('DRUPALCHAT_NEWLOGIN cookie = ' + drupalchat_getCookie('DRUPALCHAT_NEWLOGIN'));
	if (drupalchat_getCookie('DRUPALCHAT_NEWLOGIN') != 1) { // Expertus Notes: Whenever this cookie is set, the only possible value for it is 1,
	                                                        // which is set in hook_user_login().
	                                                        // Otherwise the cookie is not present, and its value therefore is undefined.
	  if(jQuery.drupalchatjStorage.get('username') != null) {
	    drupalchat.username = jQuery.drupalchatjStorage.get('username');
	  }
	  
	  if(jQuery.drupalchatjStorage.get('uid') != null) {
	    drupalchat.uid = jQuery.drupalchatjStorage.get('uid');
	  }
	  
    if(jQuery.drupalchatjStorage.get('send_current_message') != null) {
      drupalchat.send_current_message = jQuery.drupalchatjStorage.get('send_current_message');
    }	
    
    if(jQuery.drupalchatjStorage.get('last_timestamp') != null) {
      drupalchat.last_timestamp = jQuery.drupalchatjStorage.get('last_timestamp');
    }
    
    if(jQuery.drupalchatjStorage.get('send_current_uid2') != null) {
      drupalchat.send_current_uid2 = jQuery.drupalchatjStorage.get('send_current_uid2');
    }
    
    if(jQuery.drupalchatjStorage.get('attach_messages_in_queue') != null) {
      drupalchat.attach_messages_in_queue = jQuery.drupalchatjStorage.get('attach_messages_in_queue');
    }
    
    if(jQuery.drupalchatjStorage.get('running') != null) {
      drupalchat.running = jQuery.drupalchatjStorage.get('running');
    }

    if(jQuery.drupalchatjStorage.get('drupalchat') != null) {
      if(jQuery.drupalchatjStorage.get('drupalchat').length > 4) {
        
        var connStatusIcon = jQuery("#chatpanel .icon").attr("src"); // Save the connection status icon which was sent by server to present to user
        jQuery('#drupalchat').html(jQuery.drupalchatjStorage.get('drupalchat'));
        jQuery('#chatpanel .subpanel ul').empty();
        jQuery('#chatpanel .online-count').html(0); // online-count to 0
        if (Drupal.settings.drupalchat.signInState == 2) {
          $('#chatpanel .subpanel .item-list').replaceWith(Drupal.settings.drupalchat.noUsers);
        }
        else {
          $('#chatpanel .subpanel .item-list').replaceWith(Drupal.settings.drupalchat.signedOut);
        }
        $('#drupalchat .chatbox .subpanel_title > div > div > div > div').removeClass('status-1').addClass('status-0'); // EXPERTUS added
        jQuery("#chatpanel .icon").attr("src", connStatusIcon); // Present the saved connection status icon above to the user
        
        jQuery('#chatpanel .chattitle').html(Drupal.settings.drupalchat.chatTitle); // paint title in current language selection
     
        $('span.chatMinMaxBtn').each(function() {
        	if ($(this).hasClass('titlemin')) {
        	   $(this).attr('title', Drupal.settings.drupalchat.chatMinimizeText);
        	}
        	else {
        	   $(this).attr('title', Drupal.settings.drupalchat.chatMaximizeText);
        	}
        	});
        
        if (Drupal.settings.drupalchat.signInState == 2) {
          jQuery('#chatpanel .change-signin-state').html(Drupal.settings.drupalchat.signOutOfChat);
          jQuery('#chatpanel .change-signin-state').attr('title', Drupal.settings.drupalchat.signOutOfChat);
          jQuery('#chatpanel .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signOutOfChat);
        }
        else {
          jQuery('#chatpanel .change-signin-state').html(Drupal.settings.drupalchat.signIntoChat);
          jQuery('#chatpanel .change-signin-state').attr('title', Drupal.settings.drupalchat.signIntoChat);
          jQuery('#chatpanel .signin-signout-arrow').attr('title', Drupal.settings.drupalchat.signIntoChat);
        }
      }
    }
    
	  if((drupalchat.send_current_uid2 != null) && (jQuery("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").length > 0)) {
	    jQuery("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").scrollTop(
	                            jQuery("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent")[0].scrollHeight);
		}
	}
	else {
	  drupalchat_setCookie('DRUPALCHAT_NEWLOGIN', 2, 0); // Expertus Notes: Delete the cookie by setting number of days to 0
	}
	
  //console.log(drupalchat);
  //console.log('On document ready : After init from storage drupalchat object is (see above)');
  
	//Load smileys.
	if (Drupal.settings.drupalchat.enableSimleys == 1) {
	  emotify.emoticons( Drupal.settings.drupalchat.smileyURL, true, drupalchat.smilies ); // EXPERTUS: true added to ensure smileys are limited to the
	                                                                                       //           set specified in drupalchat.smilies
	}
	
	//Adjust panel height
	jQuery.fn.adjustPanel = function() {
	    // Expertus Notes - 'this' is #chatpanel which is online users list
	    jQuery(this).find("ul, .subpanel").css({ 'height' : 'auto'}); //Reset sub-panel and ul height
	
	    var windowHeight = jQuery(window).height(); //Get the height of the browser viewport
	    //console.log('windowHeight = ' + windowHeight);
	    var panelsub = jQuery(this).find(".subpanel").height(); //Get the height of sub-panel
	    //console.log('panelsub = ' + panelsub);
	    var panelAdjust = windowHeight - 100; //Viewport height - 100px (Sets max height of sub-panel)
	    //console.log('panelAdjust = ' + panelAdjust);
	    var ulAdjust =  panelAdjust - 25; //Calculate ul size after adjusting sub-panel
	    //console.log('ulAdjust = ' + ulAdjust);
	
	    if (panelsub > panelAdjust) {	 //If sub-panel is taller than max height...
	        jQuery(this).find(".subpanel").css({ 'height' : panelAdjust}); //Adjust sub-panel to max height
	        jQuery(this).find("ul").css({ 'height' : (panelAdjust - 48)}); ////Adjust subpanel ul to new size
	    }
	    else { //If sub-panel is smaller than max height...
	    	jQuery(this).find("ul").css({ 'height' : 'auto'}); //Set sub-panel ul to auto (default size)
	    }
	    
	    // Expertus - these two lines readjust the chatbox height
	    //var winHeight = $('#drupalchat-wrapper').height(); // is same as windowHeight
	    //console.log('winHeight = ' + winHeight);
	    jQuery('.chatbox').css('height', windowHeight);
	    
	    limitChatboxesDisplayByViewportWidth();
	};
	
	//Execute function on load
	jQuery("#chatpanel").adjustPanel(); //Run the adjustPanel function on #chatpanel
	
	//Each time the viewport is adjusted/resized, execute the function
	jQuery(window).resize(function () {
	    //console.log('window resize event handler');
	    jQuery("#chatpanel").adjustPanel();

      // EXPERTUS: Save details to persistent storage.
      expSaveToPersistentStorage();
	});
	
	//Add sound effect SWF file to document
	var drupalChatBeepHTML = '<div style="width: 0px; height: 0px; overflow: hidden;">' +
	                           '<object id="drupalchatbeep" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="1" height="1">' +
	                             '<param name="movie" value="' + Drupal.settings.drupalchat.sound + '"/>' +
	                             '<!--[if !IE]>-->' +
	                               '<object type="application/x-shockwave-flash" data="' + Drupal.settings.drupalchat.sound + '" width="1" height="1">' +
	                               '</object>' +
	                             '<!--<![endif]-->' +
	                           '</object>' +
	                         '</div>';
	jQuery(drupalChatBeepHTML).appendTo(jQuery("body"));
	swfobject.registerObject("drupalchatbeep", "9");
	
	jQuery("#drupalchat .subpanel .chatboxcontent").each(function() {
	  jQuery(this).scrollTop(jQuery(this)[0].scrollHeight);
	});

	//Click event on subpanels	
	jQuery("#mainpanel li div.subpanel_toggle").live('click', function() { // Click on online users list title and...
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		//console.log('Event handler for mouse click on online users list title');
	  if (jQuery(this).parent('#chatpanel').children(".subpanel").is(':visible')) {
	    //console.log('Hiding online users list');
		  jQuery(this).parent('#chatpanel').children(".subpanel").hide();
		  if(this.currTheme == "expertusoneV2")
		  {
		  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat -203px -390px');
	 	  }else{
		    jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -121px -1571px');
	      }
		  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').attr('title',Drupal.settings.drupalchat.chatMaximizeText);
		  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').addClass('titlemin');
		  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').removeClass('titlemin');
		  
	  }
	  else {		    
	    //console.log('Showing online users list');
	    jQuery(this).parent('#chatpanel').children(".subpanel").show();
	    if(this.currTheme == "expertusoneV2")
		  {
		    jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat -203px -386px');
	      }else{
	        jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -121px -1566px');	
	      }
	    jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').attr('title',Drupal.settings.drupalchat.chatMinimizeText);
	    jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').addClass('titlemin');
		jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').removeClass('titlemin');
	    if((jQuery('.subpanel div.item-list ul li').size()) > 5) {
		    if(!(jQuery('.subpanel div.item-list').hasClass('jspScrollable'))) {
				//jQuery('#chatpanel .subpanel .item-list').jScrollPane({showArrows:true});				

				/* Chat Arrow up and down functionality written by 'VJ' */
		    	jQuery('#chatpanel .subpanel .item-list').css({'height':'200'});
		    	jQuery('#chatpanel .subpanel').css({'height':'200'});
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
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				if(this.currTheme == "expertusoneV2"){ 
		    	jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'186'});
				}else
				{
				jQuery('#drupalchat .subpanel .item-list ul li a').css({'width':'175'});	
				}
		    }
	    }
	  }

		/* jQuery('#drupalchat-chat-options').hide(); */

    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
    
		return false; //Prevent browser jump to link anchor
	});

	jQuery('.signin-signout-arrow').live('click', function(e) {
		  e.stopPropagation();
		  if ($('#chatpanel .subpanel').is(':visible')) {
			  jQuery(".signin-signout-container").toggle();
		  } else {
			  jQuery("#chatpanel .subpanel").show();
			  if(Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			  {
			  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat -203px -384px');	    
			  }else
			  {
				  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -121px -1566px');	    	  
			  }
			  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').attr('title',Drupal.settings.drupalchat.chatMinimizeText);
			  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').addClass('titlemin');
			  jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').removeClass('titlemin');
			  jQuery(".signin-signout-container").toggle();
		  }
	});
	
	jQuery('.signin-signout-container .qtip-close-button').live('click', function(e) {
		  e.stopPropagation();
		  jQuery(".signin-signout-container").hide();
	});

	$('body').bind('click', function(event) {
		  if(event.target.id != 'signin-signout-arrow'){
		   $('.signin-signout-container').css('display','none');
		  }
		});
	
	//jQuery('#mainpanel li a.subpanel_toggle .change-signin-state').live('click', function(event) {
	jQuery('#mainpanel .change-signin-state').live('click', function(event) {
    //console.log('Event handler for  mouse click on change-signin-state');
    
    //console.log('Drupal.Nodejs.socket');
    //console.log(Drupal.Nodejs.socket);
    if (!Drupal.Nodejs.socket || Drupal.Nodejs.socket.socket.connected === false) {
      alert(Drupal.settings.drupalchat.noChatServer);
      event.stopPropagation();
      return;
    }
    
    $.ajax({
      type: 'GET',
      url: Drupal.settings.drupalchat.changeSignInStateUrl + '/' + Drupal.settings.drupalchat.signInState,
      timeout: 30000, //30 sec
      data: {},
      dataType: 'json',
      success: function(data) {
        //console.log(data);
        //Drupal.settings.drupalchat.signInState = newSignInState;
        // Backend would initimate to nodejs server of user's new signin state
        // which will in-turn inform all sessions of this user to update their display appropriately.
        // for different state of connections with nodejs server, what to do?
        //? may have to settimeout to kill connection with nodejs server, if no message from the latter?//
      },
      error: function(data) {
        //?unclear?
      //? may have to kill connection to nodejs ?//
      }
    });
    event.stopPropagation();
	});
	
  $('body').bind('click', function(event) {
    if(event.target.id != 'exp-chat-options'){
     $('#exp-chat-options').css('display','none');
    }
  });
  
	jQuery('.subpanel .subpanel_title').live('click', function() { // Click on chat dialog box title and...
	  //console.log('Event handler for  mouse click on chat dialog box title');
    // Clicking on chat dialog title also unhiglights the title
	  if (jQuery(this).hasClass('newchatmsg')) {
	    //console.log('removing class newchatmsg');
	    jQuery(this).removeClass('newchatmsg');
	  }
	  
		if(jQuery(this).siblings('.chatBoxContentWrapper').is(':visible')) {
		  //console.log('minimizing chat dialog window');
			jQuery(this).siblings('.chatBoxContentWrapper').hide();
			jQuery(this).siblings('.chatboxinput').hide();
			var parentLiId = jQuery(this).closest('li').attr('id');
			 if(Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			  {
			jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat -203px -390px');
			  }else
			  {
				  jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -121px -1571px');	  
			  }
			jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').attr('title',Drupal.settings.drupalchat.chatMaximizeText);
			jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').addClass('titlemin');
			jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').removeClass('titlemin');
		} else {
		  //console.log('expanding chat dialog window');
			jQuery(this).siblings('.chatBoxContentWrapper').show();
			jQuery(this).siblings('.chatboxinput').show();
			var parentLiId = jQuery(this).closest('li').attr('id');
			 if(Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			 {
			jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons_v2.png") no-repeat -203px -384px');
			 }else
			 {
				 jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -121px -1566px');	 
			 }
			jQuery('#'+parentLiId+' .subpanel .subpanel_title .chatMinMaxBtn').attr('title',Drupal.settings.drupalchat.chatMinimizeText);
			jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').addClass('titlemin');
			jQuery('#mainpanel .subpanel_title_text .chatMinMaxBtn').removeClass('titlemin');
      var isTextarea = jQuery(this).siblings('.chatboxinput').children(".chatboxtextarea");
      if (isTextarea.length > 0) {
        //console.log('setting keyboard focus to textarea');
        isTextarea[0].focus();
        jQuery(this).siblings('.chatBoxContentWrapper').scrollTop(jQuery(this).siblings('.chatBoxContentWrapper')[0].scrollHeight);
      }
		}

    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
	});

	jQuery("#chatpanel .subpanel li:not(.link) a").live('click', function() {
    //console.log('Handling mouse click to open chat dialog with online user');
		chatWith(jQuery(this).attr("class"), jQuery(this).text());
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
    
	  return false;
	});
	
	jQuery(".chatbox .subpanel_title span:not(.min)").live('click', function (eventObj) {
	  //console.log('Handling mouse click on X in chat dialog window to close the chat dialog');
		
		closeChatBox(jQuery(this).attr('class'), eventObj);
		
	  // EXPERTUS: Save details to persistent storage.
	  expSaveToPersistentStorage();
	});
	
  jQuery('#drupalchat .subpanel .chatboxcontent').live('mouseenter', function() {

	
    //jQuery(this).css("overflow-y", "auto");
	//document.body.style.overflow='hidden';
	jQuery(this).jScrollPane();
  });
  
  jQuery('#drupalchat .subpanel .chatboxcontent').live('mouseleave', function() {
    jQuery(this).css("overflow-y", "hidden");
    var chatBoxelement = jQuery(this).jScrollPane({});
    var api = chatBoxelement.data('jsp');
    api.destroy();
	  //document.body.style.overflow='auto';
  });
  
  // Expertus added - Give keyboard focus to chatboxtextarea when chatboxcontent is clicked
  jQuery('.chatbox .chatboxcontent').click(function() {
    if (jQuery(this).siblings('.chatboxinput').children('.chatboxtextarea').is(':visible')) {
      jQuery(this).siblings('.chatboxinput').children('.chatboxtextarea').focus();
    }
    
    // EXPERTUS: Save details to persistent storage.
    expSaveToPersistentStorage();
  });
  
  // EXPERTUS: Save details to persistent storage after drupalchat is loaded and initialized
  expSaveToPersistentStorage();
});


function chatWith(chatboxtitle, chatboxname) {
  createChatBox(chatboxtitle, chatboxname);
  jQuery("#chatbox_" + chatboxtitle + " .subpanel_title").click(); // Toggle the subpanel to make active
  jQuery("#chatbox_" + chatboxtitle + " .chatboxtextarea").focus();
}


function createChatBox(chatboxtitle, chatboxname, chatboxblink) {

  var timestamp = new Date().getTime();
  //console.log('timestamp = ' + timestamp);
  
  if (jQuery("#chatbox_" + chatboxtitle).length > 0) {
    jQuery("#chatbox_" + chatboxtitle).data('age', timestamp); // update age timestamp to make it most recently used
    if (jQuery("#chatbox_" + chatboxtitle).css('display') == 'none') {
      jQuery("#chatbox_" + chatboxtitle).css('display', 'block').removeClass('autohidden');
      limitChatboxesDisplayByViewportWidth();
    }

    return;
  }
  
  if(chatboxname.length >= 20) {
    restrictTitle = chatboxname.substring(0,20);
  }
  else {
    restrictTitle = chatboxname;
  }
  
  //console.log("restrictTitle : " + restrictTitle);
  this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(this.currTheme == "expertusoneV2"){ 
	var chatTxtArea= '<textarea class="chatboxtextarea"' +
    ' onkeydown="return Drupal.drupalchat.checkChatBoxInputKey(event, this, \'' + chatboxtitle + '\');">' +
    '</textarea>';	
	}else
	{
		var chatTxtArea= '<textarea class="chatboxtextarea"' +
	    ' onkeydown="return Drupal.drupalchat.checkChatBoxInputKey(event, this, \'' + chatboxtitle + '\');">' +
	    '</textarea>';		
	}
  var chatboxHtml = '<a href="#" class="chatboxhead">' +
                        '<div class="block-title-left">' +
                          '<div class="block-title-right">' +
                            '<div class="block-title-middle">' +
                              '<span class="subpanel_title_text">' + chatboxname + '</span>' + // This is used in drupalchat_nodejs.js:Drupal.drupalchat.processNoReceiverForMsg()
                            '</div>' +
                          '</div>' +
                        '</div>' +
                      '</a>' +
                      '<div class="subpanel">' +
                        '<div class="subpanel_title">' +
                          '<div class="block-title-left">' +
                            '<div class="block-title-right">' +
                              '<div class="block-title-middle">' +
                                '<span class="'+chatboxtitle+'" id="chatCloseBtn" title="'+Drupal.settings.drupalchat.chatClose+'"></span>' +
                                '<span title = "'+Drupal.settings.drupalchat.chatMinimizeText+'" class="min chatMinMaxBtn titlemin"></span>' +
                                '<div class="status-1"></div>' +
                                restrictTitle +
                              '</div>' +
                            '</div>' +
                          '</div>' +
                        '</div>' +
                        '<div class="chatBoxContentWrapper" style="display:none;">' +
                          '<div class="chatboxcontent"></div>' +
                        '</div>' +
                        '<div class="drupalchat_userOffline">' + chatboxname + ' is currently offline.</div>' +
                        '<div class="chatboxinput" style="display:none;">' +
                        chatTxtArea +
                        '</div>' +
                      '</div>';
  jQuery("<li />").attr("id", "chatbox_" + chatboxtitle)
                  .addClass("chatbox")
                  .attr('data-age', timestamp)
                  .html(chatboxHtml)
                  .prependTo(jQuery("#mainpanel"));
  
  jQuery('.chatbox').css('height', $(window).height());
	
  if (chatboxblink == 1) {
    jQuery('#chatbox_' + chatboxtitle + ' .subpanel_title').addClass('newchatmsg');
  }

  jQuery("#chatbox_" + chatboxtitle).show();
  limitChatboxesDisplayByViewportWidth();
}

function limitChatboxesDisplayByViewportWidth() {
  var windowWidth = jQuery(window).width(); //Get the height of the browser viewport
  //console.log('windowWidth = ' + windowWidth);
  var onlineUsersListWidth = 203;
  //console.log('onlineUsersListWidth = ' + onlineUsersListWidth);
  var availableWidth = windowWidth - onlineUsersListWidth;
  //console.log('availableWidth = ' + availableWidth);
  var chatboxWidth = 225;
  //console.log('chatboxWidth = ' + chatboxWidth);
  var numChatboxesWhichFit = Math.floor(availableWidth / chatboxWidth);
  numChatboxesWhichFit = numChatboxesWhichFit < 1? 1 : numChatboxesWhichFit;
  //console.log('numChatboxesWhichFit = ' + numChatboxesWhichFit);

  var visibleChatboxes = {};
  var visibleChatboxesAges = [];
  var autoHiddenChatboxes = {};
  var autoHiddenChatboxesAges = [];
  jQuery('.chatbox:visible').each(function () {
    var id = $(this).attr('id');
    var age = $(this).data('age');
    visibleChatboxes[age] = id;
    visibleChatboxesAges.push(age);
  });
  //console.log('visibleChatboxesAges');
  //console.log(visibleChatboxesAges);
  //console.log('visibleChatboxes');
  //console.log(visibleChatboxes);
  var numVisibleChatboxes = visibleChatboxesAges.length;
  //console.log('numVisibleChatboxes = ' + numVisibleChatboxes);
  
  jQuery('.chatbox.autohidden').each(function () {
    var id = $(this).attr('id');
    var age = $(this).data('age');
    autoHiddenChatboxes[age] = id;
    autoHiddenChatboxesAges.push(age);
  });
  //console.log('autoHiddenChatboxesAges');
  //console.log(autoHiddenChatboxesAges);
  //console.log('autoHiddenChatboxes');
  //console.log(autoHiddenChatboxes);
  var numAutoHiddenChatboxes = autoHiddenChatboxesAges.length;
  //console.log('numAutoHiddenChatboxes = ' + numAutoHiddenChatboxes);
  
  if (numVisibleChatboxes < numChatboxesWhichFit) {
    autoHiddenChatboxesAges.sort(function(a,b){return b - a;});
    //console.log('autoHiddenChatboxesAges sorted desc');
    //console.log(autoHiddenChatboxesAges);
    var numChatboxestoShow = numChatboxesWhichFit - numVisibleChatboxes
    for (var i = 0; i < numChatboxestoShow; i++) {
      jQuery('#' + autoHiddenChatboxes[autoHiddenChatboxesAges[i]]).removeClass('autohidden').show();
      jQuery('#' + autoHiddenChatboxes[autoHiddenChatboxesAges[i]] + ' .subpanel_title').click();
    }
  }
  else if (numVisibleChatboxes > numChatboxesWhichFit) {
    visibleChatboxesAges.sort(function(a,b){return a - b;});
    //console.log('visibleChatboxesAges sorted asc');
    //console.log(visibleChatboxesAges);
    var numChatboxesToHide = numVisibleChatboxes - numChatboxesWhichFit;
    for (var i = 0; i < numChatboxesToHide; i++) {
      jQuery('#' + visibleChatboxes[visibleChatboxesAges[i]]).addClass('autohidden').hide();
      jQuery('#' + visibleChatboxes[visibleChatboxesAges[i]]).find('.chatBoxContentWrapper').hide();
      jQuery('#' + visibleChatboxes[visibleChatboxesAges[i]]).find('.chatboxinput').hide();
    }    
  }
}

function closeChatBox(chatboxtitle,event) {
	jQuery('#chatbox_'+ chatboxtitle).css('display', 'none');
	jQuery('#chatbox_'+ chatboxtitle).find('.chatBoxContentWrapper').hide();
	jQuery('#chatbox_'+ chatboxtitle).find('.chatboxinput').hide();
	limitChatboxesDisplayByViewportWidth();
	event.stopPropagation();
}

// EXPERTUS defined function
function expSaveToPersistentStorage() {
  // Expertus: These lines were in jQuery(window).unload handler function below
  jQuery.drupalchatjStorage.set('username', drupalchat.username);
  jQuery.drupalchatjStorage.set('uid', drupalchat.uid);
  jQuery.drupalchatjStorage.set('send_current_message', drupalchat.send_current_message);
  jQuery.drupalchatjStorage.set('last_timestamp', drupalchat.last_timestamp);
  jQuery.drupalchatjStorage.set('send_current_uid2', drupalchat.send_current_uid2);
  jQuery.drupalchatjStorage.set('attach_messages_in_queue', drupalchat.attach_messages_in_queue);
  jQuery.drupalchatjStorage.set('running', drupalchat.running);
  
  // Remove 'chatboxblink' class from chatboxhead before saving #drupalchat html.
  var drupalChatDOM = jQuery('<div>' + jQuery('#drupalchat').html() + '</div>');
  //console.log('drupalChatDOM HTML = ' + drupalChatDOM.html());
  jQuery('.chatbox .subpanel_title', drupalChatDOM).removeClass('newchatmsg'); // ticket #0021023
  jQuery('#drupalchat-chat-options', drupalChatDOM).hide();
  
  // EXPERTUS: Don't save list of online users for drupalchat_nodejs as the list is rebuilt on page refresh/load - mantis ticket #0020507
  jQuery('#chatpanel .subpanel ul', drupalChatDOM).empty();
  jQuery('#chatpanel .online-count', drupalChatDOM).html(0); // online-count to 0
  if (Drupal.settings.drupalchat.signInState == 2) {
    $('#chatpanel .subpanel .item-list', drupalChatDOM).replaceWith(Drupal.settings.drupalchat.noUsers);
  }
  else {
    $('#chatpanel .subpanel .item-list', drupalChatDOM).replaceWith(Drupal.settings.drupalchat.signedOut);
  }
  //console.log('drupalChatDOM HTML after = ' + drupalChatDOM.html());
  
  jQuery(".subpanel .chatboxcontent .jspPane", drupalChatDOM).each(function() {
    var content = $(this).html();
    $(this).parent().parent().parent().html('<div class="chatboxcontent">'+content+'</div>');
  });
  
  jQuery.drupalchatjStorage.set('drupalchat', drupalChatDOM.html());
 
  
  /* @TODO: Expertus: Optimise saving and retrieving a. save/retrieve only the used drupalchat object fields  b. re-save only the changed items.
   
      drupalchat object fields
      ------------------------

      Used drupalchat object fields in code
      -------------------------------------
      send_current_uid2
      attach_messages_in_queue
      send_current_message
      send_current_message_id (used, not saved in storage)
      last_timestamp
      
      
      drupalchat object fields saved in storage
      -----------------------------------------
      send_current_message
      last_timestamp
      send_current_uid2
      attach_message_in_queue
      username(unused)
      uid (unused)
      running (unused)
      
   */
}
	

/* EXPERTUS: save to persistent storage is now done immediately when drupalchat UI changes when nodejs is chat backend.
 *           Therefore there is no need to do it on unload.
 jQuery(window).unload(function(){
  //alert('In drupalchat unload');
  jQuery('.chatbox .chatboxhead').removeClass("chatboxblink");
	//YUI().use('gallery-storage-lite', function (Y) {
	//    Y.StorageLite.on('storage-lite:ready', function () {
	//    	Y.StorageLite.setItem('username', drupalchat.username);
	//    	Y.StorageLite.setItem('uid', drupalchat.uid);
	//    	Y.StorageLite.setItem('send_current_message', drupalchat.send_current_message);
	//    	Y.StorageLite.setItem('last_timestamp', drupalchat.last_timestamp);
	//    	Y.StorageLite.setItem('send_current_uid2', drupalchat.send_current_uid2);
	//    	Y.StorageLite.setItem('attach_messages_in_queue', drupalchat.attach_messages_in_queue);
	//    	Y.StorageLite.setItem('running', drupalchat.running);
	//    	//alert(jQuery('#drupalchat').html());
	//    	Y.StorageLite.setItem('drupalchat', jQuery('#drupalchat').html());
	//        });
	//    });

  // EXPERTUS: These lines moved into new function saveToPersistentStorage()
  //jQuery.drupalchatjStorage.set('username', drupalchat.username);
  //jQuery.drupalchatjStorage.set('uid', drupalchat.uid);
  //jQuery.drupalchatjStorage.set('send_current_message', drupalchat.send_current_message);
  //jQuery.drupalchatjStorage.set('last_timestamp', drupalchat.last_timestamp);
  //jQuery.drupalchatjStorage.set('send_current_uid2', drupalchat.send_current_uid2);
  //jQuery.drupalchatjStorage.set('attach_messages_in_queue', drupalchat.attach_messages_in_queue);
  //jQuery.drupalchatjStorage.set('running', drupalchat.running);
  //jQuery.drupalchatjStorage.set('drupalchat', jQuery('#drupalchat').html());
  
  expSaveToPersistentStorage();

});*/

function drupalchat_getCookie(c_name)
{
  var i, x, y, ARRcookies = document.cookie.split(";");
  for (i=0; i < ARRcookies.length; i++) {
    x = ARRcookies[i].substr(0, ARRcookies[i].indexOf("="));
    y = ARRcookies[i].substr(ARRcookies[i].indexOf("=") + 1);
    x = x.replace(/^\s+|\s+$/g,"");
    if (x == c_name) {
      return unescape(y);
    }
  }
}
function drupalchat_setCookie(c_name, value, exdays)
{
  var exdate = new Date();
  exdate.setDate(exdate.getDate() + exdays);
  var c_value = escape(value) + ((exdays == null) ? "" : "; expires=" + exdate.toUTCString());
  document.cookie = c_name + "=" + c_value;
}

/*
 * Source: http://phpjs.org/functions/htmlspecialchars_decode/
 *         https://github.com/kvz/phpjs/blob/master/functions/strings/htmlspecialchars_decode.js
 */
function drupalchat_htmlspecialchars_decode (string, quote_style) {
  // http://kevin.vanzonneveld.net
  // +   original by: Mirek Slugen
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   bugfixed by: Mateusz "loonquawl" Zalega
  // +      input by: ReverseSyntax
  // +      input by: Slawomir Kaniecki
  // +      input by: Scott Cariss
  // +      input by: Francois
  // +   bugfixed by: Onno Marsman
  // +    revised by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
  // +      input by: Ratheous
  // +      input by: Mailfaker (http://www.weedem.fr/)
  // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
  // +    bugfixed by: Brett Zamir (http://brett-zamir.me)
  // *     example 1: htmlspecialchars_decode("<p>this -&gt; &quot;</p>", 'ENT_NOQUOTES');
  // *     returns 1: '<p>this -> &quot;</p>'
  // *     example 2: htmlspecialchars_decode("&amp;quot;");
  // *     returns 2: '&quot;'
  var optTemp = 0,
    i = 0,
    noquotes = false;
  if (typeof quote_style === 'undefined') {
    quote_style = 2;
  }
  string = string.toString().replace(/&lt;/g, '<').replace(/&gt;/g, '>');
  var OPTS = {
    'ENT_NOQUOTES': 0,
    'ENT_HTML_QUOTE_SINGLE': 1,
    'ENT_HTML_QUOTE_DOUBLE': 2,
    'ENT_COMPAT': 2,
    'ENT_QUOTES': 3,
    'ENT_IGNORE': 4
  };
  if (quote_style === 0) {
    noquotes = true;
  }
  if (typeof quote_style !== 'number') { // Allow for a single string or an array of string flags
    quote_style = [].concat(quote_style);
    for (i = 0; i < quote_style.length; i++) {
      // Resolve string input to bitwise e.g. 'PATHINFO_EXTENSION' becomes 4
      if (OPTS[quote_style[i]] === 0) {
        noquotes = true;
      } else if (OPTS[quote_style[i]]) {
        optTemp = optTemp | OPTS[quote_style[i]];
      }
    }
    quote_style = optTemp;
  }
  if (quote_style & OPTS.ENT_HTML_QUOTE_SINGLE) {
    string = string.replace(/&#0*39;/g, "'"); // PHP doesn't currently escape if more than one 0, but it should
    // string = string.replace(/&apos;|&#x0*27;/g, "'"); // This would also be useful here, but not a part of PHP
  }
  if (!noquotes) {
    string = string.replace(/&quot;/g, '"');
  }
  // Put this in last place to avoid escape being double-decoded
  string = string.replace(/&amp;/g, '&');

  return string;
}
//})(jQuery);
