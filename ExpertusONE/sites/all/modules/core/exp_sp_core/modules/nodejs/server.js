/**
 * Provides Node.js - Drupal integration.
 *
 * This code is beta quality.
 */

var request = require('request'),
    url = require('url'),
    fs = require('fs'),
    express = require('express'),
    socket_io = require('socket.io'),
    util = require('util'),
    querystring = require('querystring'),
    vm = require('vm');

var channels = {},
    authenticatedClients = {},
    /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
    onlineUsers = {},
    presenceTimeoutIds = {},
    */
    clientPresenceTimeoutIds = {}, // EXPERTUS added - to cleanup authenticatedClients[]
    /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
    contentChannelTimeoutIds = {},
    tokenChannels = {},
    */
    settingsDefaults = {
      scheme: 'http',
      port: 8080,
      host: 'localhost',
      resource: '/socket.io',
      serviceKey: '',
      debug: false,
      baseAuthPath: '/nodejs/',
      publishUrl: 'publish',
      kickUserUrl: 'user/kick/:uid',
      //logoutUserUrl: 'user/logout/:authtoken/:uid/:username',
      logoutUserUrl: 'user/logout',
      addUserToChannelUrl: 'user/channel/add/:channel/:uid',
      removeUserFromChannelUrl: 'user/channel/remove/:channel/:uid',
      addChannelUrl: 'channel/add/:channel',
      removeChannelUrl: 'channel/remove/:channel',
      setUserPresenceListUrl: 'user/presence-list/:uid/:uidList',
      addAuthTokenToChannelUrl: 'authtoken/channel/add/:channel/:uid',
      removeAuthTokenFromChannelUrl: 'authtoken/channel/remove/:channel/:uid',
      toggleDebugUrl: 'debug/toggle',
      contentTokenUrl: 'content/token',
      publishMessageToContentChannelUrl: 'content/token/message',
      getContentTokenUsersUrl: 'content/token/users',
      extensions: ["drupalchat_nodejs.server.extension"],
      clientsCanWriteToChannels: false,
      clientsCanWriteToClients: true,
      transports: ['websocket', 'flashsocket', 'htmlfile', 'xhr-polling', 'jsonp-polling'],
      jsMinification: true,
      jsEtag: true,
      flashPolicy: {
        port: 10843
      },
      backend: {
        host: 'localhost',
        scheme: 'http',
        port: 80,
        basePath: '',
        messagePath: '/?q=nodejs/message'
      },
      logLevel: 1,
      drupalchatNodejs: {
         showUserPicture: true
      }
    },
    extensions = [];

try {
  var settings = vm.runInThisContext(fs.readFileSync(process.cwd() + '/nodejs.config.js'));
}
catch (exception) {
  // console.log("server.js : Failed to read config file, exiting: " + exception);
  process.exit(1);
}
for (var key in settingsDefaults) {
  if (key != 'backend' && key != 'flashPolicy' && !settings.hasOwnProperty(key)) {
    settings[key] = settingsDefaults[key];
  }
}

if (!settings.hasOwnProperty('backend')) {
  settings.backend = settingsDefaults.backend;
}
else {
  for (var key2 in settingsDefaults.backend) {
    if (!settings.backend.hasOwnProperty(key2)) {
      settings.backend[key2] = settingsDefaults.backend[key2];
    }
  }
}

if (!settings.hasOwnProperty('flashPolicy')) {
  settings.flashPolicy = settingsDefaults.flashPolicy;
}
else {
  for (var key3 in settingsDefaults.flashPolicy) {
    if (!settings.flashPolicy.hasOwnProperty(key3)) {
      settings.flashPolicy[key3] = settingsDefaults.flashPolicy[key3];
    }
  }
}

if (settings.debug) {
  // console.log('server.js : settings = ', settings);
}

// Load server extensions
for (var i in settings.extensions) {
  try {
    // Load JS files for extensions as modules, and collect the returned
    // object for each extension.
    extensions.push(require(__dirname + '/' + settings.extensions[i]));
    if (settings.debug) {
      // console.log('server.js : Extension loaded: ', settings.extensions[i]);
    }
  }
  catch (exception) {
    // console.log("server.js : Failed to load extension: " + settings.extensions[i] + " [" + exception + "]");
    process.exit(1);
  }
}

/**
 * Check if the given channel is client-writable.
 */
var channelIsClientWritable = function (channel) {
  if (channels.hasOwnProperty(channel)) {
    return channels[channel].isClientWritable;
  }
  return false;
}

/**
 * Returns the backend url.
 */
var getBackendUrl = function () {
  return settings.backend.scheme + '://' + settings.backend.host + ':' +
         settings.backend.port + settings.backend.messagePath;
}

/**
 * Send a message to the backend.
 */
var sendMessageToBackend = function (message, callback) {
  //if (settings.debug) {
    //console.log('server.js : sendMessageToBackend() : called with message = ', message);
  //}
  var requestBody = querystring.stringify({
    messageJson: JSON.stringify(message),
    serviceKey: settings.serviceKey
  });

  var options = {
    uri: settings.backend.scheme + '://' + settings.backend.host + ':' + settings.backend.port + settings.backend.messagePath,
    body: requestBody,
    headers: {
      'Content-Length': Buffer.byteLength(requestBody),
      'Content-Type': 'application/x-www-form-urlencoded'
    }
  }

  //if (settings.debug) {
	  //console.log('server.js : sendMessageToBackend() : Sending message to backend as options = ', options);
  //}
  request.post(options, callback);
}

/**
 * Authenticate a client connection based on the message it sent.
 */
var authenticateClient = function (client, message) { // Expertus notes : client is socket object, message.authToken is MD5(sid)
  if (settings.debug) {
    // console.log('server.js : authenticateClient() : called with client.id and message.authToken = ', client.id, message.authToken);
    //console.log('server.js : authenticateClient() : authenticatedClients = ', authenticatedClients);
  }
  // If the authToken is verified, initiate a connection with the client.
  if (authenticatedClients[message.authToken]) {
    if (settings.debug) {
      // console.log('server.js : authenticateClient() : Client\'s Drupal sid is already authenticated by backend. Setting up client connection.');
    }
    setupClientConnection(client.id, authenticatedClients[message.authToken], message.contentTokens); // EXPERTUS notes: shouldn't this be authenticatedClients[message.authToken].contentTokens
  }
  else {
    if (settings.debug) {
      // console.log('server.js : authenticateClient() : Sending authenticate message to backend');
    }   
    message.messageType = 'authenticate';
    message.clientId = client.id;
    sendMessageToBackend(message, authenticateClientCallback);
  }
}

/**
 * Handle authentication call response.
 */
var authenticateClientCallback = function (error, response, body) {
  if (error) {
    // console.log('server.js : authenticateClientCallback() : Error with authenticate client request: ', error);
    return;
  }
  if (response.statusCode == 404) {
    if (settings.debug) {
      // console.log('server.js : authenticateClientCallback() : Backend authentication url not found, full response info: ', response);
    }
    else {
      // console.log('server.js : authenticateClientCallback() : Backend authentication url not found.');
    }
    return;
  }

  var authData = false;
  try {
    authData = JSON.parse(body);
  }
  catch (exception) {
    // console.log('server.js : authenticateClientCallback() : Failed to parse authentication message:', exception);
    if (settings.debug) {
      // console.log('server.js : authenticateClientCallback() : Failed message string: ' + body);
    }
    return;
  }
  
  //if (settings.debug) {
  //  console.log('server.js : authenticateClientCallback() : authData = ', authData);
  //}
  
  if (authData.error) { // EXPERTUS: added this check
    // console.log('server.js : authenticateClientCallback() : Message rejected by Drupal server nodejs module with error = ', authData.error);
    if (authData.clientId && io.sockets.sockets[authData.clientId]) {
      //process.emit('client-auth-failed', authData.clientId, authData.error); // EXPERTUS added
      io.sockets.sockets[authData.clientId].emit('exp-auth-failed', {reason: authData.error}); // EXPERTUS added
    }
    return;
  }
  
  if (!checkServiceKey(authData.serviceKey)) {
    // console.log('server.js : authenticateClientCallback() : Invalid service key from backend "', authData.serviceKey, '"');
    //process.emit('client-auth-failed', authData.clientId, 'invalid service key from backend'); // EXPERTUS added
    if (authData.clientId && io.sockets.sockets[authData.clientId]) {
      io.sockets.sockets[authData.clientId].emit('exp-auth-failed', {reason: 'invalid service key received from backend'}); // EXPERTUS added
    }
    return;
  }
  if (authData.nodejsValidAuthToken) {
    if (settings.debug) {
      // console.log('server.js : authenticateClientCallback() : Valid login for uid "', authData.uid, '"');
      //console.log('server.js : authenticateClientCallback() : authData = ', authData);
    }
    setupClientConnection(authData.clientId, authData, authData.contentTokens);
    authenticatedClients[authData.authToken] = authData;
  }
  else {
    // console.log('server.js : authenticateClientCallback() : Invalid login for uid "', authData.uid, '"');
    if (authData.clientId && io.sockets.sockets[authData.clientId]) {
      io.sockets.sockets[authData.clientId].emit('exp-auth-failed', {reason: 'invalid login'}); // EXPERTUS added
    }
    delete authenticatedClients[authData.authToken];
  }
  
  //if (settings.debug) {
  //  console.log('server.js : authenticateClientCallback() : authenticatedClients after = ', authenticatedClients);
  //}
}

/**
 * Send a presence notifcation for uid.
 */
var sendPresenceChangeNotification = function (uid, presenceEvent) {
  //if (settings.debug) {
  //  console.log('server.js : sendPresenceChangeNotification() : called with uid and presenceEvent ', uid, presenceEvent);
  //}
  
  /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  if (onlineUsers[uid]) {
    for (var i in onlineUsers[uid]) {
      var sessionIds = getNodejsSessionIdsFromUid(onlineUsers[uid][i]); // Expertus Notes: Nodejs session id is socket id.
                                                                        //              Seems one uid can have multiple sessions
                                                                        //              (see sessionIds[] is returned by getNodejsSessionIdsFromUid())
      if (sessionIds.length > 0 && settings.debug) {
        console.log('server.js : sendPresenceChangeNotification() : Sending presence notification for', uid, 'to', onlineUsers[uid][i]);
      }
      for (var j in sessionIds) {
        io.sockets.socket(sessionIds[j]).json.send({'presenceNotification': {'uid': uid, 'event': presenceEvent}});
      }
    }
  }
  */
}

/**
 * Callback that wraps all requests and checks for a valid service key.
 */
var checkServiceKeyCallback = function (request, response, next) {
  if (checkServiceKey(request.header('NodejsServiceKey', ''))) {
    next();
  }
  else {
    response.send({'error': 'Invalid service key.'});
  }
}

/**
 * Check a service key against the configured service key.
 */
var checkServiceKey = function (serviceKey) {
  if (settings.serviceKey && serviceKey != settings.serviceKey) {
    // console.log('server.js : checkServiceKey() : Invalid service key "' + serviceKey + '", expecting "' + settings.serviceKey + '"');
    return false;
  }
  return true;
}

/**
 * Http callback - return the list of content channel users.
 */
var getContentTokenUsers = function (request, response) {
  var requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var channel = JSON.parse(requestBody);
      response.send({users: getContentTokenChannelUsers(channel.channel)});
    }
    catch (exception) {
      // console.log('server.js : getContentTokensUsers() : Invalid JSON "' + requestBody + '"', exception);
      response.send({error: 'Invalid JSON, error: ' + exception.toString()});
    }
  });
}

/**
 * Http callback - set the debug flag.
 */
var toggleDebug = function (request, response) {
  var requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var toggle = JSON.parse(requestBody);
      settings.debug = toggle.debug;
      response.send({debug: toggle.debug});
    }
    catch (exception) {
      // console.log('server.js : toggleDebug() : Invalid JSON "' + requestBody + '"', exception);
      response.send({error: 'Invalid JSON, error: ' + e.toString()});
    }
  });
}

/**
 * Http callback - read in a JSON message and publish it to interested clients.
 */
var publishMessage = function (request, response) {
  var sentCount = 0, requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var message = JSON.parse(requestBody);
      //if (settings.debug) {
      //  console.log('server.js : publishMessage() : message = ', message);
      //}
    }
    catch (exception) {
      // console.log('server.js : publishMessage() : Invalid JSON "' + requestBody + '"',  exception);
      response.send({error: 'Invalid JSON, error: ' + exception.toString()});
      return;
    }
    if (message.broadcast) {
      if (settings.debug) {
        // console.log('server.js : publishMessage() : Broadcasting message');
      }
      io.sockets.json.send(message);
      sentCount = io.sockets.sockets.length;
    }
    else {
      sentCount = publishMessageToChannel(message);
    }
    process.emit('message-published', message, sentCount);
    response.send({sent: sentCount});
  });
}

/**
 * Publish a message to clients subscribed to a channel.
 */
var publishMessageToChannel = function (message) {
  //if (settings.debug) {
  //  console.log('server.js : publishMessageToChannel() : called with message = ', message);
  //}
  if (!message.hasOwnProperty('channel')) {
    // console.log('server.js : publishMessageToChannel() : An invalid message object was provided.');
    return 0;
  }
  if (message.channel == 'expertus-chat-server') { // Expertus added check, to avoid having to create a channel for each user, and prevent
                                                   // forwarding of message to clients, which they are ignoring (ticket #0022133).
    if (settings.debug) {
      // console.log('server.js : publishMessageToChannel() : Msg for chat server xtn. It will be processed & responded to by chat server xtn');
    }
    return 0;
  }
  if (!channels.hasOwnProperty(message.channel)) {
    // console.log('server.js : publishMessageToChannel() : The channel "' + message.channel + '" doesn\'t exist.');
    return 0;
  }

  var clientCount = 0;
  for (var sessionId in channels[message.channel].sessionIds) {
    if (publishMessageToClient(sessionId, message)) {
      clientCount++;
    }
  }
  if (settings.debug) {
    // console.log('server.js : publishMessageToChannel() : Sent message to ' + clientCount + ' clients in channel "' + message.channel + '"');
  }
  return clientCount;
}

/**
 * Publish a message to clients subscribed to a channel.
 */
var publishMessageToContentChannel = function (request, response) {
  var sentCount = 0, requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var message = JSON.parse(requestBody);
      //if (settings.debug) {
      //  console.log('server.js : publishMessageToContentChannel() : message', message);
      //}
    }
    catch (exception) {
      // console.log('server.js : publishMessageToContentChannel() : Invalid JSON "' + requestBody + '"', exception);
      response.send({error: 'Invalid JSON, error: ' + exception.toString()});
      return;
    }
    if (!message.hasOwnProperty('channel')) {
      // console.log('server.js : publishMessageToContentChannel() : An invalid message object was provided.');
      response.send({error: 'Invalid message'});
      return;
    }
    /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
    if (!tokenChannels.hasOwnProperty(message.channel)) {
      console.log('server.js : publishMessageToContentChannel() : The channel "' + message.channel + '" doesn\'t exist.');
      response.send({error: 'Invalid message'});
      return;
    }

    for (var socketId in tokenChannels[message.channel].sockets) {
      publishMessageToClient(socketId, message);
    }
    */
    response.send({sent: 'sent'});
  });
}

/**
 * Publish a message to a specific client.
 */
var publishMessageToClient = function (sessionId, message) {
  if (io.sockets.sockets[sessionId]) {
    io.sockets.socket(sessionId).json.send(message);
    //if (settings.debug) {
    //  console.log('server.js : publishMessageToClient() : Sent message to client ' + sessionId);
    //}
    return true;
  }
  else {
    // console.log('server.js : publishMessageToClient() : Failed to find client ' + sessionId);
  }
};

/**
 * Queue a message for a specific client sessionId.
 * Queued messages are published to the client sessionId only after receipt of acknowlegement for the previously sent queued message.
 * When the previous message is already acknowledged, and there are no queued messages, the message is immediately published to the client sessionid.
 * However, the publish op is done by the originating source nodejsExtension to allow pre-rendering of the message.
 */
var queueAndPublishMessageToClient = function (sessionId, message, nodejsExtension) {
  if (typeof io.sockets.sockets[sessionId].msgAckTimeoutId != 'undefined' ||
        typeof io.sockets.sockets[sessionId].publishMsgBatchTimeoutId != 'undefined' ||
          typeof io.sockets.sockets[sessionId].msgsQueue != 'undefined') {
    // message cannot be immediately published, it needs to be queued.
    if (typeof io.sockets.sockets[sessionId].msgsQueue == 'undefined') {
      io.sockets.sockets[sessionId].msgsQueue = [];
    }
    io.sockets.sockets[sessionId].msgsQueue.push({'msgsource-nodejs-xtn': nodejsExtension, 'message': message});
    return;
  }

  // message can be immediately published to the client.
  // To process.emit message so that the originating source nodejsExtension can pre-render the message.
  //if (settings.debug) {
  //  console.log('server.js : queueAndPublishMessageToClient() : message = ', message);
  //}
  process.emit('render-message', sessionId, message, nodejsExtension);
  // Since javascript passes objects by reference, message object should be modified after process.emmit returns
  //if (settings.debug) {
  //  console.log('server.js : queueAndPublishMessageToClient() : message after pre-render = ', message);
  //}
  message.ack = true;
  //if (settings.debug) {
  //  console.log('server.js : queueAndPublishMessageToClient() : message being published to client = ', message);
  //}
  publishMessageToClient(sessionId, message);
  if (settings.debug) {
    // console.log('server.js : queueAndPublishMessageToClient() : setting msgAckTimeout for sessionId = ', sessionId);
  }
  io.sockets.sockets[sessionId].msgAckTimeoutId = setTimeout(msgNotAcknowledged, settings.msgAckTimeoutAfter, sessionId);
};

/**
 * Callback for 'message-processed' msg emitted by client to acknowledge processing of a message sent by server
 */
var msgAcknowledged = function(sessionId) {
  //if (settings.debug) {
  //  console.log('server.js : msgAcknowledged() : called with sessionId = ', sessionId);
  //}
  // Cleanup timeout
  if (typeof io.sockets.sockets[sessionId].msgAckTimeoutId != 'undefined') {
    // console.log('server.js : msgAcknowledged() : clearing msgAckTimeoutId');
    clearTimeout(io.sockets.sockets[sessionId].msgAckTimeoutId);
  }
  delete io.sockets.sockets[sessionId].msgAckTimeoutId;
  
  // if there are queued messages, publish the next batch of messages after pre-render by the server xtn.
  if (typeof io.sockets.sockets[sessionId].msgsQueue != 'undefined') {
    if (settings.debug) {
      // console.log('server.js : msgAcknowledged() : setting publishMsgBatchTimeoutId for sessionId = ', sessionId);
    }
    io.sockets.sockets[sessionId].publishMsgBatchTimeoutId = setTimeout(publishNextMsgBatchToClient, settings.publishNextMsgsBatchAfter, sessionId);
  }
  
}

/**
 * Publish next batch of messages to the server
 */
var publishNextMsgBatchToClient = function(sessionId) {
  
  // Cleanup
  delete io.sockets.sockets[sessionId].publishMsgBatchTimeoutId;
  
  // Publish next batch of messages
  if (typeof io.sockets.sockets[sessionId].msgsQueue != 'undefined') {
    var publishMsgCount = io.sockets.sockets[sessionId].msgsQueue.length < settings.publishQueuedMsgsBatchSize?
                                 io.sockets.sockets[sessionId].msgsQueue.length : settings.publishQueuedMsgsBatchSize;
    for (var i = 0; i < publishMsgCount; i++) {
      var nodejsExtension = io.sockets.sockets[sessionId].msgsQueue[i]['msgsource-nodejs-xtn'];
      var message = io.sockets.sockets[sessionId].msgsQueue[i]['message'];
      // To process.emit message so that the originating source nodejsExtension can pre-render the message.
      //if (settings.debug) {
      //  console.log('server.js : publishNextMsgBatchToClient() : message = ', message);
      //}
      process.emit('render-message', sessionId, message, nodejsExtension);
      // Since javascript passes objects by reference, message object should be modified after process.emit returns
      //if (settings.debug) {
      //  console.log('server.js : publishNextMsgBatchToClient() : message after pre-render = ', message);
      //}
      if (i == (publishMsgCount - 1)) {
        message.ack = true; // As the messages would be processed by the client in order received,
                            // only the last message needs to be acknowledged by the client
      }
      //if (settings.debug) {
      //  console.log('server.js : publishNextMsgBatchToClient() : message being published to client = ', message);
      //}
      publishMessageToClient(sessionId, message);
    } // end while
    
    // Remove the published elements from the queue
    if (publishMsgCount == io.sockets.sockets[sessionId].msgsQueue.length) {
      // There are no more messages in queue
      delete io.sockets.sockets[sessionId].msgsQueue;
    }
    else {
      io.sockets.sockets[sessionId].msgsQueue.splice(0, publishMsgCount);
    }

    if (publishMsgCount > 0) {
      if (settings.debug) {
        // console.log('server.js : publishNextMsgBatchToClient() : setting msgAckTimeout for sessionId = ', sessionId);
      }
      io.sockets.sockets[sessionId].msgAckTimeoutId = setTimeout(msgNotAcknowledged, settings.msgAckTimeoutAfter, sessionId);
    }
  }
};

/**
 * Timeout handler when 'message-processed' msg is not emitted by client within timeout time settings.msgAckTimeoutAfter
 */
var msgNotAcknowledged = function(sessionId) {
  // Disconnect socket connection with client
  if (settings.debug) {
    // console.log('server.js : msgNotAcknowledged() : disconnecting socket connection for sessionId = ' + sessionId);
  }
  
  // Queue cleanup is done in the socket.on 'client-disconnect' handler
  io.sockets.sockets[sessionId].disconnect();
};

/*
 * Drops message queue for a socket session
 */
var dropMsgQueue = function(sessionId) {
  // Cleanup msgAckTimeout
  if (typeof io.sockets.sockets[sessionId].msgAckTimeoutId != 'undefined') {
    // console.log('server.js : dropMsgQueue() : clearing msgAckTimeoutId as set for sessionId = ', sessionId);
    clearTimeout(io.sockets.sockets[sessionId].msgAckTimeoutId);
  }
  delete io.sockets.sockets[sessionId].msgAckTimeoutId;
  
  // Cleanup publishMsgBatchTimeout
  if (typeof io.sockets.sockets[sessionId].publishMsgBatchTimeoutId != 'undefined') {
    // console.log('server.js : dropMsgQueue() : clearing publishMsgBatchTimeoutId as set for sessionId = ', sessionId);
    clearTimeout(io.sockets.sockets[sessionId].publishMsgBatchTimeoutId);    
  }
  delete io.sockets.sockets[sessionId].publishMsgBatchTimeoutId;
  
  // Delete the socket msg queue
  if (settings.debug) {
    // console.log('server.js : dropMsgQueue() : deleting msgsQueue for sessionId = ', sessionId);
  }
  delete io.sockets.sockets[sessionId].msgsQueue;
};

/**
 * Sends a 404 message.
 */
var send404 = function (request, response) {
  response.send('Not Found.', 404);
};

/**
 * Kicks the given logged in user from the server.
 */
var kickUser = function (request, response) {
  if (request.params.uid) {
    // Delete the user from the authenticatedClients hash.
    for (var authToken in authenticatedClients) {
      if (authenticatedClients[authToken].uid == request.params.uid) {
        delete authenticatedClients[authToken];
      }
    }
    // Destroy any socket connections associated with this uid.
    for (var clientId in io.sockets.sockets) {
      if (io.sockets.sockets[clientId].uid == request.params.uid) {
        delete io.sockets.sockets[clientId];
        if (settings.debug) {
          // console.log('server.js : kickUser() : deleted socket "' + clientId + '" for uid "' + request.params.uid + '"');
        }
        // Delete any channel entries for this clientId.
        for (var channel in channels) {
          delete channels[channel].sessionIds[clientId];
        }
      }
    }
    response.send({'status': 'success'});
    return;
  }
  // console.log('server.js : kickUser() : Failed to kick user, no uid supplied');
  response.send({'status': 'failed', 'error': 'missing uid'});
};

/**
 * Logout the given user from the server.
 */
var logoutUser = function (request, response) {
  var requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var userInfo = JSON.parse(requestBody);
    }
    catch (exception) {
      // console.log('server.js : logoutUser() : Invalid JSON "' + requestBody + '"', exception);
      response.send({error: 'Invalid JSON, error: ' + exception.toString()});
      return;
    }
    
    if (!(userInfo.hasOwnProperty('authToken') && userInfo.hasOwnProperty('uid') && userInfo.hasOwnProperty('name') &&
          typeof userInfo.authToken == 'string' && userInfo.authToken.length > 0 &&
            userInfo.uid > 0 &&
              typeof userInfo.name == 'string' && userInfo.name.length > 0)) {
      // console.log('server.js : logoutUser() : An invalid userInfo object was provided ', userInfo);
      response.send({error: 'Invalid or incomplete userInfo'});
      return;
    }

    //if (settings.debug) {
    //  console.log('server.js : logoutUser() : userInfo = ', userInfo);
    //}
    
    var authToken = userInfo.authToken;
    var uid = userInfo.uid;
    var username = userInfo.name;

    //if (settings.debug) {
      //console.log('server.js : logoutUser() : Logging out http session ', authToken);
      //console.log('server.js : logoutUser(): authenticatedClients = ', authenticatedClients);
    //}
    
    // EXPERTUS added: Code start : Remove any setTimeout() set for checking client presence to keep authenticatedClients[] trimmed
    if (clientPresenceTimeoutIds[authToken]) {
      //if (settings.debug) {
        //console.log('server.js : logoutUser() : clearing clientPresenceTimeout for authToken = ', authToken);
      //}
      clearTimeout(clientPresenceTimeoutIds[authToken]);
    }
    delete clientPresenceTimeoutIds[authToken];
    // EXPERTUS added: Code end : Remove any setTimeout() set for checking client presence to keep authenticatedClients[] trimmed

    // Prepare list of socket connections associated with this authToken to emit to nodejs server extensions
    var authTokenSocketIds = [];
    for (var clientId in io.sockets.sockets) {
      if (typeof io.sockets.sockets[clientId].authToken != 'undefined' && io.sockets.sockets[clientId].authToken == authToken) {
        authTokenSocketIds.push(clientId);
      }
    }
    
    var hasAnotherAuthToken = false;
    for (var nextAuthToken in authenticatedClients) {
      if (authenticatedClients[nextAuthToken].username == username && nextAuthToken != authToken) {
        hasAnotherAuthToken = true;
      }
    }
    
    
    // EXPERTUS added: Code start : Drop socket msg queue for sockets associated with this authToken as these connections will be destroyed
    for (var i = 0; i < authTokenSocketIds.length; i++) {
      if (settings.debug) {
        // console.log('server.js : logoutUser() : dropping msg queue for socket.id = ', authTokenSocketIds[i]);
      }
      dropMsgQueue(authTokenSocketIds[i]);
    }
    // EXPERTUS added: Code end : Drop socket msg queue for sockets associated with this authToken as these connections will be destroyed
    

    if (settings.debug) {
      // console.log('server.js : logoutUser() : emmitting user-logged-out with args ', uid, username, authToken, authTokenSocketIds, hasAnotherAuthToken);
    }
    process.emit('user-logged-out', uid, username, authToken, authTokenSocketIds, hasAnotherAuthToken);

    // Cleanup

    // Delete the user authToken from the authenticatedClients hash.
    delete authenticatedClients[authToken];

    //if (settings.debug) {
    //  console.log('server.js : logoutUser(): authenticatedClients after delete = ', authenticatedClients);
    //  console.log('server.js : logoutUser() : io.sockets.sockets = ', io.sockets.sockets);
    //  console.log('server.js : logoutUser() : channels = ', channels);
    //}
    
    // Destroy the socket connections associated with this authToken.
    for (var i = 0; i < authTokenSocketIds.length; i++) {
      delete io.sockets.sockets[authTokenSocketIds[i]];
      
      // Delete any channel entries for this clientId.
      for (var channel in channels) {
        delete channels[channel].sessionIds[authTokenSocketIds[i]];
        // EXPERTUS added - Trim channels if no more sessionIds in channel - ticket #20389
        if (Object.getOwnPropertyNames(channels[channel].sessionIds).length == 0) {
          delete channels[channel];
        }
      }
    }
    
    //if (settings.debug) {
    //  console.log('server.js : logoutUser() : channels after delete = ', channels);
    //  console.log('server.js : logoutUser() : io.sockets.sockets after delete = ', io.sockets.sockets);
    //}

    response.send({'status': 'success'});
  });
};

/**
 * Get the list of backend uids and authTokens connected to a content token channel.
 */
var getContentTokenChannelUsers = function (channel) {
  var users = {uids: [], authTokens: []};
  /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  for (var sessionId in tokenChannels[channel].sockets) {
    if (io.sockets.sockets[sessionId].uid) {
      users.uids.push(io.sockets.sockets[sessionId].uid);
    }
    else {
      users.authTokens.push(io.sockets.sockets[sessionId].authToken);
    }
  }
  */
  return users;
}

/**
 * Get the list of Node.js sessionIds for a given uid.
 */
var getNodejsSessionIdsFromUid = function (uid) {
  var sessionIds = [];
  for (var sessionId in io.sockets.sockets) {
    if (io.sockets.sockets[sessionId].uid == uid) {
      sessionIds.push(sessionId);
    }
  }
  //if (settings.debug) {
  //  console.log('server.js : getNodejsSessionIdsFromUid() : uid and sessionIds for the uid = ', {uid: uid, sessionIds: sessionIds});
  //}
  return sessionIds;
}

/**
 * Get the list of Node.js sessionIds for a given authToken.
 */
var getNodejsSessionIdsFromAuthToken = function (authToken) {
  var sessionIds = [];
  for (var sessionId in io.sockets.sockets) {
    if (io.sockets.sockets[sessionId].authToken == authToken) {
      sessionIds.push(sessionId);
    }
  }
  //if (settings.debug) {
  //  console.log('server.js : getNodejsSessionIdsFromAuthToken() : ', {authToken: authToken, sessionIds: sessionIds});
  //}
  return sessionIds;
}

/**
 * Add a user to a channel.
 */
var addUserToChannel = function (request, response) {
  var uid = request.params.uid || '';
  var channel = request.params.channel || '';
  if (uid && channel) {
    if (!/^\d+$/.test(uid)) {
      // console.log('server.js : addUserToChannel() : Invalid uid: ' + uid);
      response.send({'status': 'failed', 'error': 'Invalid uid.'});
      return;
    }
    if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log('server.js : addUserToChannel() : Invalid channel: ' + channel);
      response.send({'status': 'failed', 'error': 'Invalid channel name.'});
      return;
    }
    channels[channel] = channels[channel] || {'sessionIds': {}};
    var sessionIds = getNodejsSessionIdsFromUid(uid);
    if (sessionIds.length > 0) {
      for (var i in sessionIds) {
        channels[channel].sessionIds[sessionIds[i]] = sessionIds[i];
      }
      if (settings.debug) {
        // console.log("server.js : addUserToChannel() : Added channel '" + channel + "' to sessionIds " + sessionIds.join());
      }
      response.send({'status': 'success'});
    }
    else {
      // console.log('server.js : addUserToChannel() : No active sessions for uid: ' + uid);
      response.send({'status': 'failed', 'error': 'No active sessions for uid.'});
    }
    for (var authToken in authenticatedClients) {
      if (authenticatedClients[authToken].uid == uid) {
        if (authenticatedClients[authToken].channels.indexOf(channel) == -1) {
          authenticatedClients[authToken].channels.push(channel);
          if (settings.debug) {
            // console.log("server.js : addUserToChannel() : Added channel '" + channel + "' authenticatedClients");
          }
        }
      }
    }
  }
  else {
    // console.log('server.js : addUserToChannel() : Missing uid or channel');
    response.send({'status': 'failed', 'error': 'Missing uid or channel'});
  }
};

/**
 * Add an authToken to a channel.
 */
var addAuthTokenToChannel = function (request, response) {
  var authToken = request.params.authToken || '';
  var channel = request.params.channel || '';
  if (!authToken || !channel) {
    // console.log("server.js : addAuthTokenToChannel() : Missing authToken or channel");
    response.send({'status': 'failed', 'error': 'Missing authToken or channel'});
    return;
  }

  if (!/^[a-z0-9_]+$/i.test(channel)) {
    // console.log("server.js : addAuthTokenToChannel() : Invalid channel: " + channel);
    response.send({'status': 'failed', 'error': 'Invalid channel name.'});
    return;
  }
  if (!authenticatedClients[authToken]) {
    // console.log("server.js : addAuthTokenToChannel() : Unknown authToken : " + authToken);
    response.send({'status': 'failed', 'error': 'Invalid authToken.'});
    return;
  }
  channels[channel] = channels[channel] || {'sessionIds': {}};
  var sessionIds = getNodejsSessionIdsFromAuthtoken(authToken);
  if (sessionIds.length > 0) {
    for (var i in sessionIds) {
      channels[channel].sessionIds[sessionIds[i]] = sessionIds[i];
    }
    if (settings.debug) {
      // console.log("server.js : addAuthTokenToChannel() : Added sessionIds '" + sessionIds.join() + "' to channel '" + channel + "'");
    }
    response.send({'status': 'success'});
  }
  else {
    // console.log("server.js : addAuthTokenToChannel() : No active sessions for authToken: " + authToken);
    response.send({'status': 'failed', 'error': 'No active sessions for uid.'});
  }
  if (authenticatedClients[authToken].channels.indexOf(channel) == -1) {
    authenticatedClients[authToken].channels.push(channel);
    if (settings.debug) {
      // console.log("server.js : addAuthTokenToChannel() : Added channel '" + channel + "' to authenticatedClients");
    }
  }
};

/**
 * Add a client (specified by nodejs session ID) to a channel.
 */
var addClientToChannel = function (sessionId, channel) { // Expertus Notes: sessionId here is socket id.
  if (sessionId && channel) {
    if (!/^[0-9]+$/.test(sessionId) || !io.sockets.sockets.hasOwnProperty(sessionId)) {
      // console.log("server.js : addClientToChannel() : Invalid sessionId: " + sessionId);
    }
    else if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log("server.js : addClientToChannel() : Invalid channel: " + channel);
    }
    else {
      channels[channel] = channels[channel] || {'sessionIds': {}};
      channels[channel].sessionIds[sessionId] = sessionId;
      if (settings.debug) {
        // console.log("server.js : addClientToChannel() : Added channel '" + channel + "' to sessionId " + sessionId);
      }
      return true;
    }
  }
  else {
    // console.log("server.js : addClientToChannel() : Missing sessionId or channel name");
  }
  
  return false;
};

/**
 * Remove a channel.
 */
var removeChannel = function (request, response) {
  var channel = request.params.channel || '';
  if (channel) {
    if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log('server.js : removeChannel() : Invalid channel: ' + channel);
      response.send({'status': 'failed', 'error': 'Invalid channel name.'});
      return;
    }
    if (channels[channel]) {
      delete channels[channel];
      if (settings.debug) {
        // console.log("server.js : removeChannel() : Successfully removed channel '" + channel + "'");
      }
      response.send({'status': 'success'});
    }
    else {
      // console.log("server.js : removeChannel() : Non-existent channel name '" + channel + "'");
      response.send({'status': 'failed', 'error': 'Non-existent channel name.'});
      return;
    }
  }
  else {
    // console.log("server.js : removeChannel() : Missing channel");
    response.send({'status': 'failed', 'error': 'Invalid data: missing channel'});
  }
}

/**
 * Add a channel.
 */
var addChannel = function (request, response) {
  var channel = request.params.channel || '';
  if (channel) {
    if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log('server.js : addChannel() : Invalid channel: ' + channel);
      response.send({'status': 'failed', 'error': 'Invalid channel name.'});
      return;
    }
    if (channels[channel]) {
      // console.log("server.js : addChannel() : Channel name '" + channel + "' already exists.");
      response.send({'status': 'failed', 'error': "Channel name '" + channel + "' already exists."});
      return;
    }
    channels[channel] = {'sessionIds': {}};
    if (settings.debug) {
      // console.log("server.js : addChannel() : Successfully added channel '" + channel + "'");
    }
    response.send({'status': 'success'});
  }
  else {
    // console.log("server.js : addChannel() : Missing channel");
    response.send({'status': 'failed', 'error': 'Invalid data: missing channel'});
  }
}

/**
 * Remove a user from a channel.
 */
var removeUserFromChannel = function (request, response) {
  var uid = request.params.uid || '';
  var channel = request.params.channel || '';
  if (uid && channel) {
    if (!/^\d+$/.test(uid)) {
      // console.log('server.js : removeUserFromChannel() : Invalid uid: ' + uid);
      response.send({'status': 'failed', 'error': 'Invalid uid.'});
      return;
    }
    if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log('server.js : removeUserFromChannel() : Invalid channel: ' + channel);
      response.send({'status': 'failed', 'error': 'Invalid channel name.'});
      return;
    }
    if (channels[channel]) {
      var sessionIds = getNodejsSessionIdsFromUid(uid);
      for (var i in sessionIds) {
        if (channels[channel].sessionIds[sessionIds[i]]) {
          delete channels[channel].sessionIds[sessionIds[i]];
        }
      }
      for (var authToken in authenticatedClients) {
        if (authenticatedClients[authToken].uid == uid) {
          var index = authenticatedClients[authToken].channels.indexOf(channel);
          if (index != -1) {
            delete authenticatedClients[authToken].channels[index];
          }
        }
      }
      if (settings.debug) {
        // console.log("server.js : removeUserFromChannel() : Successfully removed uid '" + uid + "' from channel '" + channel + "'");
      }
      response.send({'status': 'success'});
    }
    else {
      // console.log("server.js : removeUserFromChannel() : Non-existent channel name '" + channel + "'");
      response.send({'status': 'failed', 'error': 'Non-existent channel name.'});
      return;
    }
  }
  else {
    // console.log("server.js : removeUserFromChannel() : Missing uid or channel");
    response.send({'status': 'failed', 'error': 'Invalid data'});
  }
}

/**
 * Remove an authToken from a channel.
 */
var removeAuthTokenFromChannel = function (request, response) {
  var authToken = request.params.authToken || '';
  var channel = request.params.channel || '';
  if (authToken && channel) {
    if (!authenticatedClients[authToken]) {
      // console.log('server.js : removeAuthTokenFromChannel() : Invalid authToken: ' + uid);
      response.send({'status': 'failed', 'error': 'Invalid authToken.'});
      return;
    }
    if (!/^[a-z0-9_]+$/i.test(channel)) {
      // console.log('server.js : removeAuthTokenFromChannel() : Invalid channel: ' + channel);
      response.send({'status': 'failed', 'error': 'Invalid channel name.'});
      return;
    }
    if (channels[channel]) {
      var sessionIds = getNodejsSessionIdsFromAuthToken(authToken);
      for (var i in sessionIds) {
        if (channels[channel].sessionIds[sessionIds[i]]) {
          delete channels[channel].sessionIds[sessionIds[i]];
        }
      }
      if (authenticatedClients[authToken]) {
        var index = authenticatedClients[authToken].channels.indexOf(channel);
        if (index != -1) {
          delete authenticatedClients[authToken].channels[index];
        }
      }
      if (settings.debug) {
        // console.log("server.js : removeAuthTokenFromChannel() : Successfully removed authToken '" + authToken + "' from channel '" + channel + "'.");
      }
      response.send({'status': 'success'});
    }
    else {
      // console.log("server.js : removeAuthTokenFromChannel() : Non-existent channel name '" + channel + "'");
      response.send({'status': 'failed', 'error': 'Non-existent channel name.'});
      return;
    }
  }
  else {
    // console.log("server.js : removeAuthTokenFromChannel() : Missing authToken or channel");
    response.send({'status': 'failed', 'error': 'Invalid data'});
  }
}

/**
 * Remove a client (specified by session ID) from a channel.
 */
var removeClientFromChannel = function (sessionId, channel) {
  if (sessionId && channel) {
    if (!/^[0-9]+$/.test(sessionId) || !io.sockets.sockets.hasOwnProperty(sessionId)) {
      // console.log("server.js : removeClientFromChannel() : Invalid sessionId: " + sessionId);
    }
    else if (!/^[a-z0-9_]+$/i.test(channel) || !channels.hasOwnProperty(channel)) {
      // console.log("server.js : removeClientFromChannel() : Invalid channel: " + channel);
    }
    else if (channels[channel].sessionIds[sessionId]) {
      delete channels[channels].sessionIds[sessionId];
      if (settings.debug) {
        // console.log("server.js : removeClientFromChannel() : Removed sessionId '" + sessionId + "' from channel '" + channel + "'");
      }
      return true;
    }
  }
  else {
    // console.log("server.js : removeClientFromChannel() : Missing sessionId or channel name");
  }
  return false;
};

/**
 * Set the list of users a uid can see presence info about.
 */
var setUserPresenceList = function (uid, uids) {
  var uid = request.params.uid || '';
  var uidlist = request.params.uidlist.split(',') || [];
  if (uid && uidlist) {
    if (!/^\d+$/.test(uid)) {
      // console.log("server.js : setUserPresenceList() : Invalid uid: " + uid);
      response.send({'status': 'failed', 'error': 'Invalid uid.'});
      return;
    }
    if (uidlist.length == 0) {
      // console.log("server.js : setUserPresenceList() : Empty uidlist");
      response.send({'status': 'failed', 'error': 'Empty uid list.'});
      return;
    }
    for (var i in uidlist) {
      if (!/^\d+$/.test(uidlist[i])) {
        // console.log("server.js : setUserPresenceList() : Invalid uid: " + uid);
        response.send({'status': 'failed', 'error': 'Invalid uid.'});
        return;
      }
    }
    /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
    onlineUsers[uid] = uidlist;
    */
    response.send({'status': 'success'});
  }
  else {
    response.send({'status': 'failed', 'error': 'Invalid parameters.'});
  }
}

/**
 * Cleanup after a socket has disconnected.
 */
var cleanupSocket = function (socket) {
  //if (settings.debug) {
  //  console.log("server.js : cleanupSocket() : Cleaning up after socket id", socket.id, 'uid', socket.uid);
  //  console.log('server.js : cleanupSocket() : channels = ', channels);
  //}
  for (var channel in channels) {
    delete channels[channel].sessionIds[socket.id];
    // EXPERTUS added - trim channels if no more sessionIds in channel - ticket #20389
    if (Object.getOwnPropertyNames(channels[channel].sessionIds).length == 0) {
      delete channels[channel];
    }
  }
  
  //if (settings.debug) {
  //  console.log('server.js : cleanupSocket() : channels after delete = ', channels);
  //}
  
  var uid = socket.uid;
  
  /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  if (uid != 0) {
    if (presenceTimeoutIds[uid]) {
      clearTimeout(presenceTimeoutIds[uid]);
    }
    presenceTimeoutIds[uid] = setTimeout(checkOnlineStatus, 2000, uid);
  }
  */

  /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  for (var tokenChannel in tokenChannels) { // Expertus Notes: Use of tokenChannels is unclear
    //if (settings.debug) {
    //  console.log("server.js : cleanupSocket() : checking tokenChannel", tokenChannel, socket.id);
    //}
    if (tokenChannels[tokenChannel].sockets[socket.id]) {
      //if (settings.debug) {
      //  console.log("server.js : cleanupSocket() : found socket.id for tokenChannel", tokenChannel, tokenChannels[tokenChannel].sockets[socket.id]);
      //}
      if (tokenChannels[tokenChannel].sockets[socket.id].notifyOnDisconnect) {
        if (contentChannelTimeoutIds[tokenChannel + '_' + uid]) {
          clearTimeout(contentChannelTimeoutIds[tokenChannel + '_' + uid]);
        }
        contentChannelTimeoutIds[tokenChannel + '_' + uid] = setTimeout(checkTokenChannelStatus, 2000, tokenChannel, socket); // only place where socket is used
      }
      delete tokenChannels[tokenChannel].sockets[socket.id];
    }
  }
  */

  var authToken = io.sockets.sockets[socket.id].authToken? io.sockets.sockets[socket.id].authToken : false; // EXPERTUS added - see below comment
  delete io.sockets.sockets[socket.id];
 
  // EXPERTUS added code to prevent authenticatedClients to keep on growing if user closes browser window instead of logging out.
  //          Next time the user logs in, a new sid is created for the user. So, the authToken (MD5(sid)) entry in authenticatedClients never
  //          cleans up.
  if (authToken !== false) {
    var socketPresent = false;
    for (var socketId in io.sockets.sockets) {
      if (typeof io.sockets.sockets[socketId].authToken !== 'undefined' && io.sockets.sockets[socketId].authToken == authToken) {
        socketPresent = true;
        break;
      }
    }
    if (clientPresenceTimeoutIds[authToken]) {
      clearTimeout(clientPresenceTimeoutIds[authToken]);
    }
    delete clientPresenceTimeoutIds[authToken];
    
    if (!socketPresent) { // Do not immediately delete authenticatedClients[authData] as is could be a page refresh or page switch
      if (settings.debug) {
        // console.log('server.js : cleanupSocket() : settingTimeout to check for client with authToken = ', authToken, settings.eraseDrupalAuthAfter);
      }
      clientPresenceTimeoutIds[authToken] = setTimeout(checkAuthenticatedClientStatus, settings.eraseDrupalAuthAfter, authToken);
    }
    else {
      if (settings.debug) {
        // console.log('server.js : cleanupSocket() : no need for settingTimeout to check for client with authToken = ', authToken);
      }
    }
  }
}

/**
 * If an authenticatedClient is gone, delete its entry from authenticatedClients[]
 */
var checkAuthenticatedClientStatus = function (authToken) { // EXPERTUS - function added

  if (settings.debug) {
    // console.log('server.js : checkAuthenticatedClientStatus() : Checking presence of client with authToken = ', authToken);
  }
  var socketPresent = false;
  for (var socketId in io.sockets.sockets) {
    if (typeof io.sockets.sockets[socketId].authToken !== 'undefined' && io.sockets.sockets[socketId].authToken == authToken) {
      socketPresent = true;
      break;
    }
  }
  
  if (!socketPresent) {
    if (settings.debug) {
         // console.log('server.js : checkAuthenticatedClientStatus() : removing authToken entry from authenticatedClients as no more sockets associated with authToken = ', authToken);
      // console.log('server.js : checkAuthenticatedClientStatus() : authenticatedClients before delete = ', authenticatedClients);
    }
    delete authenticatedClients[authToken];
    //if (settings.debug) {
    //  console.log('server.js : checkAuthenticatedClientStatus() : authenticatedClients after delete = ', authenticatedClients);
    //}
  }
  
  delete clientPresenceTimeoutIds[authToken]; // Cleanup
}

/**
 * Check for any open sockets associated with the channel and socket pair.
 */
var checkTokenChannelStatus = function (tokenChannel, socket) {
  /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  // If the tokenChannel no longer exists, just bail.
  if (!tokenChannels[tokenChannel]) {
    if (settings.debug) {
      console.log("server.js : checkTokenChannelStatus() : no tokenChannel ", tokenChannel, socket.uid);
    }
    return;
  }

  // If we find a socket for this user in the given tokenChannel, we can just
  // return, as there's nothing we need to do.
  var sessionIds = getNodejsSessionIdsFromUid(socket.uid);
  for (var i = 0; i < sessionIds.length; i++) {
    if (tokenChannels[tokenChannel].sockets[sessionIds[i]]) {
      if (settings.debug) {
        console.log("server.js : checkTokenChannelStatus() : found socket for tokenChannel ", tokenChannel, socket.uid);
      }
      return;
    }
  }

  // We didn't find a socket for this uid, and we have other sockets in this,
  // channel, so send disconnect notification message.
  var message = {
    'channel': tokenChannel,
    'contentChannelNotification': true,
    'data': {
      'uid': socket.uid,
      'type': 'disconnect'
    }
  };
  for (var socketId in tokenChannels[tokenChannel].sockets) {
    publishMessageToClient(socketId, message);
  }
  */
}

/**
 * Check for any open sockets for uid.
 */
var checkOnlineStatus = function (uid) {
  if (getNodejsSessionIdsFromUid(uid).length == 0) {
    if (settings.debug) {
      // console.log("server.js : checkOnlineStatus() : Sending offline notification for ", uid);
    }
    setUserOffline(uid);
  }
  /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  delete presenceTimeoutIds[uid]; // Added by EXPERTUS to prevent memory leak
  */
}

/**
 * Sends offline notification to sockets, the backend and cleans up our list.
 */
var setUserOffline = function (uid) {
  sendPresenceChangeNotification(uid, 'offline');
  /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  delete onlineUsers[uid];
  */
  sendMessageToBackend({uid: uid, messageType: 'userOffline'}, function (response) { });
}

/**
 * Set a content token.
 */
var setContentToken = function (request, response) {
  var requestBody = '';
  request.setEncoding('utf8');
  request.on('data', function (chunk) {
    requestBody += chunk;
  });
  request.on('end', function () {
    try {
      var message = JSON.parse(requestBody);
      if (settings.debug) {
        // console.log('server.js : setContentToken() : message', message);
      }
    }
    catch (exception) {
      // console.log('server.js : setContentToken() : Invalid JSON "' + requestBody + '"',  exception);
      response.send({error: 'Invalid JSON, error: ' + exception.toString()});
      return;
    }
    /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
    tokenChannels[message.channel] = tokenChannels[message.channel] || {'tokens': {}, 'sockets': {}};
    tokenChannels[message.channel].tokens[message.token] = message;
    if (settings.debug) {
      console.log('server.js : setContentToken() : ', message.token, 'for channel', message.channel);
    }
    */
    response.send({status: 'ok'});
  });
}

/**
 * Setup a io.sockets.sockets{}.connection with uid, channels etc.
 */
var setupClientConnection = function (sessionId, authData, contentTokens) { // Expertus Notes: sessionId is ndoejs session id which is socket id
  if (settings.debug) {
    // console.log('server.js : setupClientConnection() : called with sessionId, authData.uid and contentTokens = ', sessionId, authData.uid, contentTokens);
    //console.log('io.sockets.sockets = ', io.sockets.sockets);
  }
  if (!io.sockets.sockets[sessionId]) {
    // console.log("server.js : setupClientConnection() : Client socket '" + sessionId + "' went away.");
    // console.log(authData);
    return;
  }
  
  //if (settings.debug) {
    //console.log('server.js : setupClientConnection() : io.sockets.sockets = ', io.sockets.sockets);
    //console.log('server.js : setupClientConnection() : channels = ', channels);
  //}
  io.sockets.sockets[sessionId].authToken = authData.authToken; // Expertus Notes: This is MD5(sid) now saved as authToken in io.sockets.sockets
  io.sockets.sockets[sessionId].uid = authData.uid; // Expertus Notes: uid also is saved in io.sockets.sockets
  
  // EXPERTUS added: Code start : Remove any setTimeout() set for checking client presence to keep authenticatedClients[] trimmed
  if (clientPresenceTimeoutIds[authData.authToken]) {
    if (settings.debug) {
      // console.log('server.js : setupClientConnection() : clearing clientPresenceTimeout for authToken = ', authData.authToken);
    }
    clearTimeout(clientPresenceTimeoutIds[authData.authToken]);
  }
  delete clientPresenceTimeoutIds[authData.authToken];
  // EXPERTUS added: Code end : Remove any setTimeout() set for checking client presence to keep authenticatedClients[] trimmed
  
  for (var i in authData.channels) {
    channels[authData.channels[i]] = channels[authData.channels[i]] || {'sessionIds': {}};
    channels[authData.channels[i]].sessionIds[sessionId] = sessionId; // Expertus Notes: The sessionId saved is socket id and not MD5(sid)
  }
  //if (settings.debug) {
  //  console.log('server.js : setupClientConnection() : channels after processing authData.channels ', channels);
  //}
  
  /* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  if (authData.uid != 0) {
    //if (settings.debug) {
    //  console.log('server.js : setupClientConnection() : onlineUsers = ', onlineUsers);
    //}
    var sendPresenceChange = !onlineUsers[authData.uid]; // Expertus Notes: Presence change notification sent only when onlineUsers[authData.uid)
                                                         //                 is previously undefined
    onlineUsers[authData.uid] = authData.presenceUids || [];
    if (sendPresenceChange) {
      if (settings.debug) {
        console.log('server.js : setupClientConnection() : sending PresenceChangeNotification <online> to user presence list of user id = ',
                                                                                                                                authData.uid);
      }
      sendPresenceChangeNotification(authData.uid, 'online');
    }
  }
  */
  
  //if (settings.debug) {
  //  console.log('server.js : setupClientConnection() : onlineUsers after = ', onlineUsers);
  //  console.log('server.js : setupClientConnection() : contentTokens = ', contentTokens); // Expetus Notes: Use unclear
  //  console.log('server.js : setupClientConnection() : tokenChannels = ', tokenChannels); // Expertus Notes:
  //}

  /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  var clientToken = '';
  for (var tokenChannel in contentTokens) {
    tokenChannels[tokenChannel] = tokenChannels[tokenChannel] || {'tokens': {}, 'sockets': {}};

    clientToken = contentTokens[tokenChannel];
    if (tokenChannels[tokenChannel].tokens[clientToken]) {
      tokenChannels[tokenChannel].sockets[sessionId] = tokenChannels[tokenChannel].tokens[clientToken];
      if (settings.debug) {
        console.log('server.js : setupClientConnection() : Added token', clientToken, 'for channel', tokenChannel, 'for socket', sessionId);
      }
      delete tokenChannels[tokenChannel].tokens[clientToken];
    }
  }
  */
  
  if (settings.debug) {
    //console.log('server.js : setupClientConnection() : tokenChannels after loop = ', tokenChannels);
    // console.log('server.js : setupClientConnection() : Emitting client-authenticated event with session id = ', sessionId);
    //console.log('and authData = ', authData);
  }
  
  //if (settings.debug) {
    //console.log('server.js : setupClientConnection() : onlineUsers = ', onlineUsers);
  //}
  
  process.emit('client-authenticated', sessionId, authData);
  
  if (sessionId && io.sockets.sockets[sessionId]) {
    io.sockets.sockets[sessionId].emit('exp-authenticated', {status: 'authenticated'}); // EXPERTUS added
  }
};

var server;
if (settings.scheme == 'https') {
  var sslOptions = {
    key: fs.readFileSync(settings.sslKeyPath),
    cert: fs.readFileSync(settings.sslCertPath),
    passphrase: settings.passPhrase
  };
  if (settings.sslCAPath) {
    sslOptions.ca = fs.readFileSync(settings.sslCAPath);
  }
  server = express.createServer(sslOptions);
}
else {
  server = express.createServer();
}

server.all(settings.baseAuthPath + '*', checkServiceKeyCallback);
server.post(settings.baseAuthPath + settings.publishUrl, publishMessage);
server.get(settings.baseAuthPath + settings.kickUserUrl, kickUser);
//server.get(settings.baseAuthPath + settings.logoutUserUrl, logoutUser); // Original line
server.post(settings.baseAuthPath + settings.logoutUserUrl, logoutUser); // Expertus modified
server.get(settings.baseAuthPath + settings.addUserToChannelUrl, addUserToChannel);
server.get(settings.baseAuthPath + settings.removeUserFromChannelUrl, removeUserFromChannel);
server.get(settings.baseAuthPath + settings.addChannelUrl, addChannel);
server.get(settings.baseAuthPath + settings.removeChannelUrl, removeChannel);
server.post(settings.baseAuthPath + settings.toggleDebugUrl, toggleDebug);
/* onlineUsers{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
server.get(settings.baseAuthPath + settings.setUserPresenceListUrl, setUserPresenceList);
*/
/* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
server.post(settings.baseAuthPath + settings.getContentTokenUsersUrl, getContentTokenUsers);
server.post(settings.baseAuthPath + settings.contentTokenUrl, setContentToken);
server.post(settings.baseAuthPath + settings.publishMessageToContentChannelUrl, publishMessageToContentChannel);
*/
server.get('*', send404);
server.listen(settings.port, settings.host);
// console.log('Started ' + settings.scheme + ' server.');

//if (settings.debug) {
//  console.log('server.js : flash policy port = ', settings.flashPolicy.port);
//}
var io = socket_io.listen(server, {port: settings.port, resource: settings.resource, 'flash policy port': settings.flashPolicy.port});
io.configure(function () {
  io.set('transports', settings.transports);
  io.set('log level', settings.logLevel);
  if (settings.jsEtag) {
    io.enable('browser client etag');
  }
  if (settings.jsMinification) {
    io.enable('browser client minification');
  }
});

io.sockets.on('connection', function(socket) {
  process.emit('client-connection', socket.id);

  socket.on('authenticate', function(message) {
    if (settings.debug) {
      //console.log('message = ', message);
      // console.log('server.js : socket.on authenticate : Authenticating client with key "' + message.authToken + '"');
    }
    authenticateClient(socket, message);
  });

  socket.on('message', function(message) {
    //if (settings.debug) {
      //console.log('server.js : io.sockets : message handler : message = ', message);
    //}
    // If the message is from an active client, then process it.
    if (io.sockets.sockets[socket.id] && message.hasOwnProperty('type')) {
      if (settings.debug) {
        // console.log('server.js : socket.on message : Received message from client ' + socket.id);
      }

      // If this message is destined for a channel, check that writing to
      // channels from client sockets is allowed.
      if (message.hasOwnProperty('channel')) {
        if (settings.clientsCanWriteToChannels || channelIsClientWritable(message.channel)) {
          if (settings.debug) {
            // console.log('server.js : socket.on message : Emmitting client-message for message with channel');
          }
          process.emit('client-message', socket.id, message);
        }
        else {
          // console.log('server.js : socket.on message : Received unauthorised message with channel from client: cannot write to channel ' + socket.id);
        }
      }

      // No channel, so this message is destined for one or more clients. Check
      // that this is allowed in the server configuration.
      else if (settings.clientsCanWriteToClients) { // EXPERTUS : 'else' before the 'if' was added, as both client-message were triggering
        if (settings.debug) {
          // console.log('server.js : socket.on message : Emmitting message client-message');
        }
        process.emit('client-message', socket.id, message);
      }
      else {
        // console.log('server.js : socket.on message : Received unauthorised message from client: cannot write to client ' + socket.id);
      }
      return;
    }
  });

  socket.on('disconnect', function () {
    if (settings.debug) {
      // console.log('server.js : socket.on disconnect : dropping msg queue for socket.id', socket.id);
    }
    dropMsgQueue(socket.id);

    if (settings.debug) {
      // console.log('server.js : socket.on disconnect : Emitting client-disconnect event for socket.id = ', socket.id);
    }
    process.emit('client-disconnect', socket.id);
    cleanupSocket(socket);
  });
  
  socket.on('message-processed', function() {
    if (settings.debug) {
      // console.log('server.js : socket.on message-processed : socket.id = ', socket.id);
    //  console.log('server.js : socket.on message-processed : io.sockets.sockets[socket.id] = ', io.sockets.sockets[socket.id]);
    }   
    msgAcknowledged(socket.id);
  });
  
})
.on('error', function(exception) {
  // console.log('server.js : io.socket.on error : Socket error [' + exception + ']');
});

/**
 * Invokes the specified function on all registered server extensions.
 */
var invokeExtensions = function (hook) {
  var args = arguments.length ? Array.prototype.slice.call(arguments, 1) : [];
  for (var i in extensions) {
    if (extensions[i].hasOwnProperty(hook) && extensions[i][hook].apply) {
      extensions[i][hook].apply(this, args);
    }
  }
}

/**
 * Define a configuration object to pass to all server extensions at
 * initialization. The extensions do not have access to this namespace,
 * so we provide them with references.
 */
var extensionsConfig = {
  'publishMessageToChannel': publishMessageToChannel,
  'publishMessageToClient': publishMessageToClient,
  'queueAndPublishMessageToClient': queueAndPublishMessageToClient,
  'dropMsgQueue': dropMsgQueue,
  'addClientToChannel': addClientToChannel,
  'settings': settings,
  'channels': channels,
  'io': io,
  /* tokenChannels{} removed as not being used in ExpertusONE chat - see mantis ticket #0020389
  'tokenChannels': tokenChannels,
  */
  'sendMessageToBackend': sendMessageToBackend,
  'authenticatedClients': authenticatedClients
};
invokeExtensions('setup', extensionsConfig);

// vi:ai:expandtab:sw=2 ts=2

