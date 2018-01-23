/**
 * DrupalChat server extension for nodejs
 *
 * Add this extension name to the "extensions" in node.config.js
 */

var publishMessageToClient;
var queueAndPublishMessageToClient;
var dropMsgQueue;
var settings = {};
var authenticatedClients = {};

var drupalchat_ordered_names = []; //sorted array of usernames
var drupalchat_users = {}; //property is username, value is an object whose properties are sessionIds.
var drupalchat_uids = {};  //property is username, value is uid
var drupalchat_names = {}; //property is uid, value is username
var drupalchat_fullnames = {}; //property is uid, value is username //Expertus added
var drupalchat_pictureFilenames = {}; // propery is username, value is picture file name.
var drupalchat_signInState = {}; // propery is username, value is 1 for signed out and 2 for signed in.
var userPresenceTimeoutIds = {}; // property is username, value is setTimeout() id.
var drupalchat_msgs_queue = {}; // property is username, value is [{senderSessionId : sessionId, msgForClient : msgForClientObj}, ...]
var drupal_visible_users = {};

exports.setup = function (config) {
  publishMessageToClient = config.publishMessageToClient;
  queueAndPublishMessageToClient = config.queueAndPublishMessageToClient;
  dropMsgQueue = config.dropMsgQueue;
  settings = config.settings;
  authenticatedClients = config.authenticatedClients;

  var insertNameInOrderedList = function(name) {
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : called with name = ', name);
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : drupalchat_ordered_names = ', drupalchat_ordered_names);
    //}
    if (drupalchat_ordered_names.length <= 0) { // array empty
      drupalchat_ordered_names[0] = name;
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : empty drupalchat_ordered_names ins = ',
      //                                                             drupalchat_ordered_names);
      //}
      return;
    }

    // Binary search and insert
    var startIndex = 0;
    var stopIndex = drupalchat_ordered_names.length - 1;
    var middle = Math.floor((stopIndex + startIndex) / 2);
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : startIndex = ' + startIndex);
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : stopIndex = ' + stopIndex);
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : middle = ' + middle);
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : drupalchat_ordered_names[middle] = ' + drupalchat_ordered_names[middle]);
    //}
    while (startIndex < stopIndex && drupalchat_ordered_names[middle] != name) {
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : next drupalchat_ordered_names[middle] = ' + drupalchat_ordered_names[middle]);
      //}

      if (name < drupalchat_ordered_names[middle]) {
        stopIndex = middle - 1;
        //if (settings.debug) {
        //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : new stopIndex = ' + stopIndex);
        //}
      }
      else if (name > drupalchat_ordered_names[middle]) {
        startIndex = middle + 1;
        //if (settings.debug) {
        //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : new startIndex = ' + startIndex);
        //}
      }
      // recalculate middle
      middle = Math.floor((stopIndex + startIndex) / 2);
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : new middle = ' + middle);
      //}
    } // end while

    if (middle < 0) {
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : middle is < 0 = ' + middle);
      //}
      middle = 0;
    }
    else if (middle > drupalchat_ordered_names.length - 1) {
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : middle is > last index = ' + middle);
      //}
      middle = drupalchat_ordered_names.length - 1;
    }
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : middle after loop = ' + middle);
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : drupalchat_ordered_names[middle] after loop = ' +
    //                                                                                                      drupalchat_ordered_names[middle]);
    //}
    if (name < drupalchat_ordered_names[middle]) { // insert name before middle
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : inserting name before ' + drupalchat_ordered_names[middle]);
      //}
      drupalchat_ordered_names.splice(middle, 0, name);
    }
    else if (name > drupalchat_ordered_names[middle]) { // insert name after middle
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : inserting name after ' + drupalchat_ordered_names[middle]);
      //}
      drupalchat_ordered_names.splice(middle+1, 0, name);
    }
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : insertNameInOrderedList() : after insert drupalchat_ordered_names = ',
    //                                                             drupalchat_ordered_names);
    //}
  };

  var getUserIndexInSortedList = function(name) {
    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : called with name = ', name);
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : drupalchat_ordered_names = ', drupalchat_ordered_names);
    //}
    if (drupalchat_ordered_names.length <= 0) { // array empty
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : empty drupalchat_ordered_names');
      //}
      return -1;
    }

    // Binary search and insert
    var startIndex = 0;
    var stopIndex = drupalchat_ordered_names.length - 1;
    var middle = Math.floor((stopIndex + startIndex) / 2);
    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : startIndex = ' + startIndex);
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : stopIndex = ' + stopIndex);
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : middle = ' + middle);
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : drupalchat_ordered_names[middle] = ' + drupalchat_ordered_names[middle]);
    //}
    while (startIndex < stopIndex && drupalchat_ordered_names[middle] != name) {
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : next drupalchat_ordered_names[middle] = ' + drupalchat_ordered_names[middle]);
      //}

      if (name < drupalchat_ordered_names[middle]) {
        stopIndex = middle - 1;
        //if (settings.debug) {
          //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : new stopIndex = ' + stopIndex);
        //}
      }
      else if (name > drupalchat_ordered_names[middle]) {
        startIndex = middle + 1;
        //if (settings.debug) {
          //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : new startIndex = ' + startIndex);
        //}
      }
      // recalculate middle
      middle = Math.floor((stopIndex + startIndex) / 2);
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : new middle = ' + middle);
      //}
    } // end while

    if (middle < 0) {
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : middle is < 0 = ' + middle);
      //}
      middle = 0;
    }
    else if (middle > drupalchat_ordered_names.length - 1) {
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : middle is > last index = ' + middle);
      //}
      middle = drupalchat_ordered_names.length - 1;
    }
    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : middle after loop = ' + middle);
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : drupalchat_ordered_names[middle] after loop = ' +
      //                                                                                                    drupalchat_ordered_names[middle]);
    //}
    if (name != drupalchat_ordered_names[middle]) { // insert name before middle
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : name not found');
      //}
      return -1;
    }

    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getUserIndexInSortedList() : name is found at idx = ', middle);
    //}

    return middle;
  };

  var getClientInsertInfoForUser = function(clientUser, newOnlineUserIndexInSortedList) {
    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getClientInsertInfoForUser() : called for clientUser and newUser = ',
      //                                                clientUser, drupalchat_ordered_names[newOnlineUserIndexInSortedList]);
      //console.log('drupalchat_nodejs.server.extension.js : getClientInsertInfoForUser() : drupalchat_ordered_names = ', drupalchat_ordered_names);
    //}
    var retObj = {after: null, before: null};
    var i;
    for (i = newOnlineUserIndexInSortedList - 1;
             i >= 0 && (drupalchat_ordered_names[i] == clientUser || drupalchat_signInState[drupalchat_ordered_names[i]] != 2);
                 i--);
    if (i >= 0) {
      retObj.after = {name: drupalchat_ordered_names[i], uid: drupalchat_uids[drupalchat_ordered_names[i]]};
    }

    for (i = newOnlineUserIndexInSortedList + 1;
             i < drupalchat_ordered_names.length && (drupalchat_ordered_names[i] == clientUser || drupalchat_signInState[drupalchat_ordered_names[i]] != 2);
                 i++);
    if (i < drupalchat_ordered_names.length) {
      retObj.before = {name: drupalchat_ordered_names[i], uid: drupalchat_uids[drupalchat_ordered_names[i]]};
    }

    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : getClientInsertInfoForUser() : retObj = ', retObj);
    //}

    return retObj;
  };

  var titleRestricted = function(title, chara) {
    var restrictTitle;
    if (title.length >= chara) {
      restrictTitle = title.substring(0, chara);
      restrictTitle = restrictTitle + '...';
    }
    else {
      restrictTitle =  title;
    }
    return restrictTitle;
  };

  var generateOnlineUserHTML = function(name, uid, pictureFilename) {
    if (typeof drupalchat_users[name] != 'undefined') {
      uid = drupalchat_uids[name];
      pictureFilename = drupalchat_pictureFilenames[name];
    }
    var currTheme= settings.drupalchatNodejs.currTheme;
    var fullname = drupalchat_fullnames[uid];
    if(currTheme == "expertusoneV2")
	 {
     var onlineuserList='<span class="user-list-border-img"><img class="onlineuserpic uid' + uid + '" width="32px" height="32px" alt="" ' +
     'src="/sites/default/files/pictures/' + pictureFilename +
     '" style="vertical-align:middle;"></span>';
     var onlineUserName= '<span class="chatUserTitle">' + titleRestricted(fullname, 13) + '</span>';
	 }else
	 {
		 var onlineuserList='<img class="onlineuserpic uid' + uid + '" width="32px" height="32px" alt="" ' +
	     'src="/sites/default/files/pictures/' + pictureFilename +
	     '" style="vertical-align:middle;">';
		 var onlineUserName=   titleRestricted(fullname, 16);
	 }

    var onlineUserHTML = '<li class="status-1' + ((settings.drupalchatNodejs.showUserPicture)? '' : ' nopic') + '">' +
                           '<a title="' + fullname + '" class="' + uid +
                                '" href="javascript:void(0)" id="drupalchat_user_' + uid + '">' +

                       		    ((settings.drupalchatNodejs.showUserPicture)?
                       		    onlineuserList : '') +
                       		 onlineUserName  +

                            '</a>' +
                          '</li>';
    return onlineUserHTML;
  };

  var getOnlineUsersListForMsg = function(excludedUsername) {
    var onlineUsersListHTML = '';
    var onlineUsersList = {};
    var onlineUsersCount = 0;
    var i;
    for (i=0; i < drupalchat_ordered_names.length; i++) {
    	if (drupalchat_ordered_names[i] != excludedUsername && drupalchat_signInState[drupalchat_ordered_names[i]] == 2 && (drupal_visible_users[excludedUsername].indexOf(drupalchat_uids[drupalchat_ordered_names[i]]) >= 0)) { 
        //if (settings.debug) {
        //  console.log('drupalchat_nodejs.server.extension.js : preparedOrderedOnlineUsersList() : preparing html for user = ',
        //                                                                                                      drupalchat_ordered_names[i]);
        //}
        onlineUsersListHTML += generateOnlineUserHTML(drupalchat_ordered_names[i]);
        onlineUsersList[drupalchat_uids[drupalchat_ordered_names[i]]] = {
            pictureFilename: drupalchat_pictureFilenames[drupalchat_ordered_names[i]]
        };
        onlineUsersCount++;
      }
    } // end for loop

    return {count: onlineUsersCount, onlineUsersListHtml: onlineUsersListHTML, onlineUsersList: onlineUsersList};
  };

  //http://stackoverflow.com/questions/679915/how-do-i-test-for-an-empty-javascript-object-from-json
  var isEmptyJSObject = function(obj) {
    for(var prop in obj) {
      if(obj.hasOwnProperty(prop))
          return false;
    }

    return true;
  };

  var userHasSessions = function(username) {
    return (!isEmptyJSObject(drupalchat_users[username]));
    /*var hasSessions = false;
    for(var socketId in drupalchat_users[username]) {
      hasSessions = true;
      break;
    }

    return hasSessions;*/
  };

  var eraseUserPresence = function(rusername) {

    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : userPresenceTimeoutIds = ', userPresenceTimeoutIds);
    //}
    delete userPresenceTimeoutIds[rusername]; // Cleanup
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : userPresenceTimeoutIds after delete = ', userPresenceTimeoutIds);
    //}

    if (typeof drupalchat_users[rusername] == 'undefined') { // confirm
      return; // should never happen
    }

    if (userHasSessions(rusername)) {
      if (typeof drupalchat_msgs_queue[rusername] != 'undefined') {
        delete drupalchat_msgs_queue[rusername];
      }
      return; // no action is required
    }

    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_users = ', drupalchat_users);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_names = ', drupalchat_names);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_pictureFilenames = ', drupalchat_pictureFilenames);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_uids = ', drupalchat_uids);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_ordered_names = ', drupalchat_ordered_names);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_signInState = ', drupalchat_signInState);
      //console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_msgs_queue = ', drupalchat_msgs_queue);
    //}

    eraseUserMessagesQueue(rusername);

    var ruid = drupalchat_uids[rusername];
    var rSignInState = drupalchat_signInState[rusername];
    delete drupalchat_users[rusername];
    delete drupalchat_names[ruid];
    delete drupalchat_fullnames[ruid]; //Expertus added
    delete drupalchat_uids[rusername];
    delete drupalchat_pictureFilenames[rusername];
    delete drupalchat_signInState[rusername];
    delete drupal_visible_users[rusername]; //Expertus added
    var index = drupalchat_ordered_names.indexOf(rusername);
    drupalchat_ordered_names.splice(index, 1);

    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_users after delete  = ', drupalchat_users);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_names after delete = ', drupalchat_names);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_pictureFilenames after delete = ', drupalchat_pictureFilenames);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_uids after delete = ', drupalchat_uids);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_ordered_names after delete = ', drupalchat_ordered_names);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_signInState after delete = ', drupalchat_signInState);
    //  console.log('drupalchat_nodejs.server.extension.js : eraseUserPresence : drupalchat_msgs_queue after delete = ', drupalchat_msgs_queue);
    //}

    // Nobody needs to be informed if the user was not signed into chat
    if (rSignInState != 2) {
      return;
    }

    // User was signed into chat
    // Inform all sessions of other signed in users of this user going offline
    for (var username in drupalchat_users) {
      if (drupalchat_signInState[username] == 2) {
        for (var socketId in drupalchat_users[username]) {
          if (settings.debug) {
          }
          queueAndPublishMessageToClient(socketId, {type: 'userOffline', data: ruid, callback: 'drupalchatNodejsMessageHandler'}, 'drupalchat');
        } // end inner for loop
      }
    } // end outer for loop
  };

  var flushUserMessagesQueue = function(username) {
    if (typeof drupalchat_msgs_queue[username] == 'undefined') {
      return;
    }

    // Send any queued messages to the user
    for (var i = 0; i < drupalchat_msgs_queue[username].length; i++) {
      var senderSessionId = drupalchat_msgs_queue[username][i].senderSessionId;
      var msgForClient = drupalchat_msgs_queue[username][i].msgForClient;

      // Drop the message if the sender is no more a drupalchat user.
      if (typeof drupalchat_users[msgForClient.fromUsername] == 'undefined') {
        continue;
      }

      // Make sure that the pictures are the latest
      msgForClient.fromUserPictureFilename = drupalchat_pictureFilenames[msgForClient.fromUsername];
      msgForClient.forUserPictureFilename = drupalchat_pictureFilenames[username];

      // Inform the recipient of the status to paint the correct status icon
      msgForClient.fromUserStatus = drupalchat_signInState[msgForClient.fromUsername];

      // Forward the chat message
      if (settings.debug) {
      }
      forwardChatMessage(senderSessionId, msgForClient);
    } //end for loop

    delete drupalchat_msgs_queue[username]; // Cleanup
  };

  var eraseUserMessagesQueue = function(username) {
    if (typeof drupalchat_msgs_queue[username] == 'undefined') {
      return;
    }

    // Inform sender of undelivered messages.
    for (var i = 0; i < drupalchat_msgs_queue[username].length; i++) {
      var senderSessionId = drupalchat_msgs_queue[username][i].senderSessionId;
      var msgForClient = drupalchat_msgs_queue[username][i].msgForClient;

      if (typeof drupalchat_users[msgForClient.fromUsername] != 'undefined' &&
                     drupalchat_users[msgForClient.fromUsername].hasOwnProperty(senderSessionId)) {
        if (settings.debug) {
        }
        informSenderOfUndeliveredMessages(senderSessionId, msgForClient.msgsList, msgForClient.forUserId);
      }
      else {
      }
    } // end for loop

    delete drupalchat_msgs_queue[username]; // Cleanup
  };

  var informSenderOfUndeliveredMessages = function(sessionId, msgsList, forUserId) {

    var msgIdList = [];
    for (msg in msgsList) {
      msgIdList.push(msgsList[msg].id);
    }

    var undeliveredMsgs = {msgIdList: msgIdList, uid2: forUserId};

    if (settings.debug) {
    }
    publishMessageToClient(sessionId, {type: 'noReceiverForMsg', data: undeliveredMsgs, callback: 'drupalchatNodejsMessageHandler'});
  };

  var forwardChatMessage = function(senderSocketId, msgForClient) {
    // First update other sessions of the sending client with the message
    msgForClient.displayAs = 'S'; // Sender

    // Publish message to all nodejs sessions of the source user other than the sender session
    for (var socketId in drupalchat_users[msgForClient.fromUsername]) {
      if (socketId != senderSocketId) {
        if (settings.debug) {
        }
        publishMessageToClient(socketId, {type: 'newMessage', data: msgForClient, callback: 'drupalchatNodejsMessageHandler'});
      }
    }

    //Publish message to the target client
    msgForClient.displayAs = 'R'; // Recipient

    // Publish message to all nodejs sessions of the target user
    for (var socketId in drupalchat_users[msgForClient.forUsername]) {
      if (settings.debug) {
      }
      publishMessageToClient(socketId, {type: 'newMessage', data: msgForClient, callback: 'drupalchatNodejsMessageHandler'});
    }
  };

  var queueChatMessage = function(senderSessionId, msgForClient) {
    // Create/Initialize the queue if not present already
    if (typeof drupalchat_msgs_queue[msgForClient.forUsername] == 'undefined') {
      drupalchat_msgs_queue[msgForClient.forUsername] = [];
    }

    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : queueChatMessage : drupalchat_msgs_queue[msgForClient.forUsername] = ',
    //      msgForClient.forUsername, drupalchat_msgs_queue[msgForClient.forUsername]);
    //}

    drupalchat_msgs_queue[msgForClient.forUsername].push({senderSessionId : senderSessionId, msgForClient: msgForClient});

    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : queueChatMessage : drupalchat_msgs_queue[msgForClient.forUsername] after add = ',
    //      msgForClient.forUsername, drupalchat_msgs_queue[msgForClient.forUsername]);
    //}
  };

  process.on('client-authenticated', function (sessionId, authData) { // SessionId is nodejs sessionId, which is socket id.
    //if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : Processing this event with session id and authdata.uid = ',
      //                                                                                                                   sessionId, authData.uid);
      //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : authData = ', authData);
    //}
	  var currTheme= settings.drupalchatNodejs.currTheme;
		 if(currTheme == "expertusoneV2")
		  {
			 var chatuserIcon='expertusonev2_default_user.png';
		  }
		 else
		 {
			 var chatuserIcon='default_user.png';
		 }

    if(authData.uid > 0 && typeof authData.name != 'undefined' && authData.name != '') {

      //add new user
      if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_users = ', drupalchat_users);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_uids = ', drupalchat_uids);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_names = ', drupalchat_names);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_pictureFilenames = ', drupalchat_pictureFilenames);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_ordered_names = ', drupalchat_ordered_names);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_signInState = ', drupalchat_signInState);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : userPresenceTimeoutIds = ', userPresenceTimeoutIds);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_msgs_queue = ', drupalchat_msgs_queue);
      }

      var authDataPictureFilename = authData.picture && authData.picture != null? authData.picture.filename : chatuserIcon;

      var newUser = false;
      if (typeof drupalchat_users[authData.name] == 'undefined') {
        insertNameInOrderedList(authData.name);
        drupalchat_users[authData.name] = {};
        drupalchat_uids[authData.name] = authData.uid;
        drupalchat_names[authData.uid] = authData.name;
        drupalchat_fullnames[authData.uid] = authData.fullname;
        drupalchat_pictureFilenames[authData.name] = authDataPictureFilename;
        drupalchat_signInState[authData.name] = authData.signInState;

        newUser = true;
      }
      drupal_visible_users[authData.name] = authData.visibleUserID;

      // If user picture is changed, we need to update user picture in all pre-existing user sessions
      if (authDataPictureFilename != drupalchat_pictureFilenames[authData.name]) {
        process.emit('message-published',
             {type: 'sendPictureFilename',
              data: '{"uid":"' + authData.uid + '","name":"' + authData.name + '","pictureFilename":"' + authDataPictureFilename + '"}'}, 1);
        //drupalchat_pictureFilenames[authData.name] = authDataPictureFilename;
      }

      // If user sign in state is changed, we need to update user presence in all pre-existing user sessions
      if (authData.signInState != drupalchat_signInState[authData.name]) {
        process.emit('message-published',
            {type: 'signInStateChanged',
             data: '{"uid":"' + authData.uid + '","name":"' + authData.name + '","signInState":' + authData.signInState + '}'}, 1);
        //drupalchat_signInState[authData.name] = authData.signInState;
      }

      // Save user session in drupalchat_users
      drupalchat_users[authData.name][sessionId] = sessionId;

      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_users after = ', drupalchat_users);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_uids after = ', drupalchat_uids);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_names after = ', drupalchat_names);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_pictureFilenames after = ', drupalchat_pictureFilenames);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_ordered_names after = ', drupalchat_ordered_names);
        //console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_signInState after = ', drupalchat_signInState);
      //}

      // If user presence timeout on this user was set, clear the timeout
      if (userPresenceTimeoutIds[authData.name]) {
        clearTimeout(userPresenceTimeoutIds[authData.name]);
      }
      delete userPresenceTimeoutIds[authData.name];
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : client-authenticated : userPresenceTimeoutIds after delete = ', userPresenceTimeoutIds);
      //}

      if (authData.signInState != 2) { // 2 is for signed in
        // The user is not signed into chat
        if (settings.debug) {
        }

        // Delete any messages in the message queue of this user after informing senders of the undelivered messages
        eraseUserMessagesQueue(authData.name);
        //if (settings.debug) {
        //  console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_msgs_queue = ', drupalchat_msgs_queue);
        //}

        return;
      }

      if (settings.debug) {
      }
      flushUserMessagesQueue(authData.name);
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : client-authenticated : drupalchat_msgs_queue = ', drupalchat_msgs_queue);
      //}

      // Prepare ordered online users list HTML to be sent to the new client.
      var onlineUsersListForMsg = getOnlineUsersListForMsg(authData.name);

      if (onlineUsersListForMsg.count == 0) {
        // This is the only user currently online, nobody is there to be informed
        // Nor this user needs to be sent an online user's list
        if (settings.debug) {
        }
        return;
      }

      if (settings.debug) {
       // console.log('drupalchat_nodejs.server.extension.js : client-authenticated : Emitting to new user onlineUsersList having length ',
          //  onlineUsersListForMsg.count, authData.name, authData.uid, sessionId);
      }

      publishMessageToClient(sessionId, {type: 'onlineUsersList', data: onlineUsersListForMsg, callback: 'drupalchatNodejsMessageHandler'});

      // If the user is new user, inform other signed in users of this new user
      if (newUser) {
        var userIndexInSortedList = getUserIndexInSortedList(authData.name);
        for (var username in drupalchat_users) {
        	 if (username != authData.name && drupalchat_signInState[username] == 2 && (drupal_visible_users[authData.name].indexOf(drupalchat_uids[authData.name]) >= 0)) {
            var insertInfo = getClientInsertInfoForUser(username, userIndexInSortedList);
            for (var socketId in drupalchat_users[username]) {
              if (settings.debug) {
              //  console.log('drupalchat_nodejs.server.extension.js : client-authenticated : Emitting userOnline informing existing online user of the new online user ',
                                                                             //     username, drupalchat_uids[username], socketId);
              }
              queueAndPublishMessageToClient(socketId,
                                     {type: 'userOnline',
                                      data: {uid: authData.uid,
                                             name: authData.name,
                                             insertInfo: insertInfo
                                            },
                                      callback: 'drupalchatNodejsMessageHandler'
                                     }, 'drupalchat');
              /*//TEST CODE - start
              for (var i = 0; i < 1000; i++) {
                var testInsertInfo = {after:{}, before:null};
                var suffix = (i < 10? '000' + i : (i < 100? '00' + i : (i < 1000? '0' + i : i)));
                var uid = 'test' + suffix;
                var name = 'test.user.' + suffix;
                if (i == 0) {
                  testInsertInfo.after.uid = authData.uid;
                  testInsertInfo.after.name = authData.name;
                }
                else {
                  testInsertInfo.after.uid = uid;
                  testInsertInfo.after.name = name;
                }
                queueAndPublishMessageToClient(socketId,
                  {type: 'userOnline',
                   data: {uid: uid,
                          name: name,
                          insertInfo: testInsertInfo
                         },
                   callback: 'drupalchatNodejsMessageHandler'
                  }, 'drupalchat');
              }
              //TEST CODE - end*/
            } // end for each session of user
          }
        } // end for each user
      }
    }
  })
  .on('client-message', function (sessionId, message) { // SessionId is socket id from which this message was received
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : client-message : Processing this event with sessionId and message = ', sessionId, message);
    //}

    if(message.type == 'newMessage') {
      //if (settings.debug) {
        //console.log('drupalchat_nodejs.server.extension.js : client-message : newMessage : message ', message);
      //}

      if (message.uid1 == 0 || typeof drupalchat_names[message.uid1] == 'undefined') {
        // sender of message is an unauthenticated user
        return;
      }

      for (var i in message.msgObjList) {
        var msgObj = message.msgObjList[i];
        //if (settings.debug) {
        //  console.log('drupalchat_nodejs.server.extension.js : client-message : newMessage : msgObj ', msgObj);
        //}
        // If there is no message receiver, then inform the same to the sender of the message
        if (drupalchat_signInState[drupalchat_names[message.uid1]] != 2 ||
               typeof drupalchat_names[msgObj.uid2] == 'undefined' ||
                 drupalchat_signInState[drupalchat_names[msgObj.uid2]] != 2) {
          if (settings.debug) {
          }
          informSenderOfUndeliveredMessages(sessionId, msgObj.msgsList, msgObj.uid2);
          continue;
        }

        // Message to publish
        var msgForClient = {
              length: msgObj.msgsList? msgObj.msgsList.length : 0,
              msgsList: msgObj.msgsList || [],
              forUserId: msgObj.uid2,
              forUsername: drupalchat_names[msgObj.uid2],
	      forUserFullName: drupalchat_fullnames[msgObj.uid2],
              forUserPictureFilename : drupalchat_pictureFilenames[drupalchat_names[msgObj.uid2]],
              fromUserId: message.uid1,
              fromUsername: drupalchat_names[message.uid1],
	      fromUserFullName: drupalchat_fullnames[message.uid1],
              fromUserPictureFilename: drupalchat_pictureFilenames[drupalchat_names[message.uid1]]
        };

        if (userHasSessions(msgForClient.forUsername)) {
          if (settings.debug) {
          }
          forwardChatMessage(sessionId, msgForClient);
        }
        else {
          if (settings.debug) {
          }
          queueChatMessage(sessionId, msgForClient);
          //if (settings.debug) {
          //  console.log('drupalchat_nodejs.server.extension.js : client-authenticated : after queuing drupalchat_msgs_queue = ', drupalchat_msgs_queue);
          //}
        }
      } // end for loop - messages
    } // end if
  })
  .on('message-published', function (message, i) {
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : message-published : Received this event with message and i = ', message, i);
    //}

    if (message.type == 'sendPictureFilename') {
      var data = JSON.parse(message.data);

      // update picture file name in authenticatedClients
      for (var authToken in authenticatedClients) {
        if (authenticatedClients[authToken].name == data.name) {
          if (typeof authenticatedClients[authToken].picture == 'undefined' || authenticatedClients[authToken].picture == null) {
            authenticatedClients[authToken].picture = {};
          }
          authenticatedClients[authToken].picture.filename = data.pictureFilename;
        }
      }

      if (typeof drupalchat_pictureFilenames[data.name] != 'undefined') {
        drupalchat_pictureFilenames[data.name] = data.pictureFilename;
      }

      // Regardless of whether the picture is of an existing drupalchat user or not, we inform all users of the picture change
      // since there may be a currently displaying chat dialog window with this user.
      for (var username in drupalchat_users) {
        for (var socketId in drupalchat_users[username]) {
          if (settings.debug) {
          }
          publishMessageToClient(socketId,
                {type: 'userPictureChanged', data: {uid: data.uid, pictureFilename: data.pictureFilename}, callback: 'drupalchatNodejsMessageHandler'});
        }
      }
    }

    if (message.type == 'signInStateChanged') {
      var data = JSON.parse(message.data);

      // Update signInState in authenticatedClients
      for (var authToken in authenticatedClients) {
        if (authenticatedClients[authToken].name == data.name) {
          authenticatedClients[authToken].signInState = data.signInState;
        }
      }

      if (typeof drupalchat_users[data.name] === 'undefined') {
        // The user is no more a drupalchat user
        return;
      }

      // Confirm change in signIn state before doing anything.
      if (data.signInState == drupalchat_signInState[data.name]) {
        return;
      }

      // Update user signIn state in drupalchat_signInState
      drupalchat_signInState[data.name] = data.signInState;

      if (data.signInState == 2) { // User signed in
        // Prepare and send online user's list html to all sessions of this user.
        // Send userOnline message to all sessions of all other users

        // Prepare ordered online users list HTML to be sent to the new client.
        var onlineUsersListForMsg = getOnlineUsersListForMsg(data.name);
        var userIndexInSortedList = getUserIndexInSortedList(data.name);

        for (var username in drupalchat_users) {
          for (var socketId in drupalchat_users[username]) {
            if (username == data.name) {
              if (settings.debug) {
              }
              publishMessageToClient(socketId, {type: 'signedIn', data: onlineUsersListForMsg, callback: 'drupalchatNodejsMessageHandler'});
            }
            else if (drupalchat_signInState[username] == 2) { // only when user is signed into chat
              var insertInfo = getClientInsertInfoForUser(username, userIndexInSortedList);
              queueAndPublishMessageToClient(socketId,
                  {type: 'userOnline',
                   data: {uid: data.uid,
                          name: data.name,
                          insertInfo: insertInfo
                         },
                   callback: 'drupalchatNodejsMessageHandler'
                  }, 'drupalchat');
            }
          } // end inner for loop
        } // end outer for loop
      }
      else { // User signed out
        // Inform all sessions of this user of the same.
        // Send userOffline message to all sessions of all other users
        for (var username in drupalchat_users) {
          for (var socketId in drupalchat_users[username]) {
            if (username == data.name) {
              if (settings.debug) {
              }
              dropMsgQueue(socketId);
              if (settings.debug) {
              }
              publishMessageToClient(socketId, {type: 'signedOut', data: {}, callback: 'drupalchatNodejsMessageHandler'});
            }
            else if (drupalchat_signInState[username] == 2) { // only when user is signed into chat
              if (settings.debug) {
              }
              queueAndPublishMessageToClient(socketId, {type: 'userOffline', data: data.uid, callback: 'drupalchatNodejsMessageHandler'}, 'drupalchat');
            }
          } // end inner for loop
        } // end outer for loop
      }
    }
  })
  .on('client-disconnect', function (sessionId) { // sessionId is nodejs session id which is socket id of the disconnecting client
    if (settings.debug) {
    }

    // Find the username which corresponds to this session id
    var rusername = '';
    for (var username in drupalchat_users) {
        for (var socketId in drupalchat_users[username]) {
          if (socketId == sessionId) {
            rusername = username;
            break;
          }
        }

        if (rusername == '') {
          continue;
        }
    }

    if(rusername != '') {
      if (settings.debug) {
      }

      delete drupalchat_users[rusername][sessionId];

      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : client-disconnect : drupalchat_users after deleting session = ', drupalchat_users);
      //}

      if (userHasSessions(rusername)) {
        return; // no action is required
      }

      // User does not have any more sessions.

      if (userPresenceTimeoutIds[rusername]) {
        clearTimeout(userPresenceTimeoutIds[rusername]);
      }
      delete userPresenceTimeoutIds[rusername];

      if (settings.debug) {
      }
      userPresenceTimeoutIds[rusername] = setTimeout(eraseUserPresence, settings.drupalchatNodejs.eraseUserAfter, rusername);
      //if (settings.debug) {
      //  console.log('drupalchat_nodejs.server.extension.js : client-disconnect : userPresenceTimeoutIds = ', userPresenceTimeoutIds);
      //}
    }
  })
  .on('user-logged-out', function (ruid, rusername, authToken, authTokenSocketIds, hasAnotherAuthToken) {
    if (settings.debug) {
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_users = ', drupalchat_users);
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_names = ', drupalchat_names);
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_pictureFilenames = ', drupalchat_pictureFilenames);
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_uids = ', drupalchat_uids);
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_ordered_names = ', drupalchat_ordered_names);
      //console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_signInState = ', drupalchat_signInState);
    }

    // Remove any user presence timeout set for the user in client-disconnect, if that was called first
    if (userPresenceTimeoutIds[rusername]) { // Should be present only if this was the last session for the user and client-disconnect was called first.
      clearTimeout(userPresenceTimeoutIds[rusername]);
    }
    delete userPresenceTimeoutIds[rusername];

    // Check if drupalchat_users[rusername] is still present
    if (!drupalchat_users.hasOwnProperty(rusername)) {
      if (authTokenSocketIds.length > 0) {
      }
      if (settings.debug) {
      }
      return;
    }

    for (var i = 0; i < authTokenSocketIds.length; i++) {
      var sessionId = authTokenSocketIds[i];
      if (drupalchat_users[rusername].hasOwnProperty(sessionId)) {
        if (settings.debug) {
        }
        publishMessageToClient(sessionId, {type: 'loggedOut', data: {}, callback: 'drupalchatNodejsMessageHandler'});

        // Remove the user nodejs sessionId from drupalchat_users.
        delete drupalchat_users[rusername][sessionId];
      }
      else {
      }
    } // end for loop
    //if (settings.debug) {
    //  console.log('drupalchat_nodejs.server.extension.js : user-logged-out : drupalchat_users after deleting authTokenSocketIds = ', drupalchat_users);
    //}

    if (userHasSessions(rusername) || hasAnotherAuthToken) {
      return; // no action is required
    }

    eraseUserPresence(rusername);
  })
  .on('render-message', function (sessionId, message, nodejsExtension) {
    /*if (settings.debug) {
      console.log('drupalchat_nodejs.server.extension.js : render-message : invoked with sessionId, message, nodejsExtension = ',
                                 sessionId, message, nodejsExtension);
    }*/
    if (nodejsExtension != 'drupalchat' || typeof message.type == 'undefined') {
      return;
    }
    var currTheme= settings.drupalchatNodejs.currTheme;
	 if(currTheme == "expertusoneV2")
	  {
		 var chatuserIcon='expertusonev2_default_user.png';
	  }
	 else
	 {
		 var chatuserIcon='default_user.png';
	 }
    switch (message.type) {
    case 'userOnline':

      /*// TEST CODE - start
      var prefix = message.data.uid.substring(0,4);
      if (prefix == 'test') {
        message.data.pictureFilename = 'default_user.png';
        message.data.onlineUserHTML = generateOnlineUserHTML(message.data.name, message.data.uid, message.data.pictureFilename);
        break;
      }
      // TEST CODE - end*/
      //if ((drupal_visible_users[message.data.name].indexOf(drupalchat_uids[message.data.name]) >= 0)) {
      message.data.pictureFilename = typeof drupalchat_users[message.data.name] == 'undefined'? chatuserIcon :
                                                                                                drupalchat_pictureFilenames[message.data.name];
      message.data.onlineUserHTML = generateOnlineUserHTML(message.data.name, message.data.uid, message.data.pictureFilename);
      //}
      break;
    case 'userOffline':
      // Nothing to be done
      break;
    }

    /*if (settings.debug) {
      console.log('drupalchat_nodejs.server.extension.js : render-message : after modification message is ', message);
    }*/
  });



};

