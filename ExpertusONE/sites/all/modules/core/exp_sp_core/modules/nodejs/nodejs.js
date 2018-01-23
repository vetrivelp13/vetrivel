
(function ($) {

Drupal.Nodejs = Drupal.Nodejs || {
  'contentChannelNotificationCallbacks': {},
  'presenceCallbacks': {},
  'callbacks': {},
  'socket': false,
  'connectionSetupHandlers': {}
};

Drupal.behaviors.nodejs = {
  attach: function (context, settings) {
    if (!Drupal.Nodejs.socket) {
      if (Drupal.Nodejs.connect()) { // Expertus Notes: Drupal.Nodejs.connect() does not return anything!
        Drupal.Nodejs.sendAuthMessage(); // Expertus Notes: Never executed
      }
    }
  }
};

Drupal.Nodejs.runCallbacks = function (message) {
  //console.log('In Drupal.Nodejs.runCallbacks().');
  // It's possible that this message originated from an ajax request from the
  // client associated with this socket.
  if (message.clientSocketId == Drupal.Nodejs.socket.socket.sessionid) {
    return;
  }
  if (message.callback) {
    //console.log('In Drupal.Nodejs.runCallbacks() : message.callback code.');
    if (typeof message.callback == 'string') {
      message.callback = [message.callback];
    }
    $.each(message.callback, function () {
      var callback = this;
      if (Drupal.Nodejs.callbacks[callback] && $.isFunction(Drupal.Nodejs.callbacks[callback].callback)) {
        try {
          Drupal.Nodejs.callbacks[callback].callback(message);
        }
        catch (exception) {}
      }
    });
  }
  else if (message.presenceNotification != undefined) {
    //console.log('In Drupal.Nodejs.runCallbacks() : message.presenceNotification code.');
    $.each(Drupal.Nodejs.presenceCallbacks, function () {
      if ($.isFunction(this.callback)) {
        try {
          this.callback(message);
        }
        catch (exception) {}
      }
    });
  }
  else if (message.contentChannelNotification != undefined) {
    //console.log('In Drupal.Nodejs.runCallbacks() : message.contentChannelNotification code.');
    $.each(Drupal.Nodejs.contentChannelNotificationCallbacks, function () {
      if ($.isFunction(this.callback)) {
        try {
          this.callback(message);
        }
        catch (exception) {}
      }
    });
  }
  else {
    //console.log('In Drupal.Nodejs.runCallbacks() : default callbacks code.');
    $.each(Drupal.Nodejs.callbacks, function () {
      if ($.isFunction(this.callback)) {
        try {
          this.callback(message);
        }
        catch (exception) {}
      }
    });
  }
};

Drupal.Nodejs.runSetupHandlers = function (type) {
  $.each(Drupal.Nodejs.connectionSetupHandlers, function () {
    //console.log('Drupal.Nodejs.runSetupHandlers foreach this[type]' + this[type]);
    if ($.isFunction(this[type])) {
      try {
        this[type]();
      }
      catch (exception) {}
    }
  });
};

Drupal.Nodejs.connect = function (cleanupBeforeConnect) {
  //console.log('nodejs : In Drupal.Nodejs.connect(). Attempt number = ' + Drupal.Nodejs.reconnectCount);
  if (typeof cleanupBeforeConnect === 'undefined') cleanupBeforeConnect = false; // EXPERTUS added
  var scheme = Drupal.settings.nodejs.secure ? 'https' : 'http',
      url = scheme + '://' + Drupal.settings.nodejs.host + ':' + Drupal.settings.nodejs.port;
  Drupal.settings.nodejs.connectTimeout = Drupal.settings.nodejs.connectTimeout || 10000; // EXPERTUS increased from 5 sec. to 10 sec.
  //Drupal.settings.nodejs.reconnectDelay = Drupal.settings.nodejs.reconnectDelay || 500;
  Drupal.settings.nodejs.tryMultipleTransport = Drupal.settings.nodejs.tryMultipleTransport || false; // EXPERTUS added
  if (typeof io === 'undefined') {
     //console.log('nodejs : Drupal.Nodejs.connect() : io is undefined');
     Drupal.Nodejs.runSetupHandlers('connectionFatalFailure');
     return false;
  }
  
  //console.log('nodejs : Drupal.Nodejs.connect() : creating connection');
 
  // Drupal.Nodejs.expCheckConnection is added by EXPERTUS to manage the setTimeout handler for checking connection
  if (Drupal.Nodejs.expCheckConnection) {
    clearTimeout(Drupal.Nodejs.expCheckConnection);
  }
  delete Drupal.Nodejs.expCheckConnection;

  //console.log(io);
  //console.log('io before io.connect()');
  if (cleanupBeforeConnect) {
    Drupal.Nodejs.socket.removeAllListeners();
    if (Drupal.Nodejs.socket.socket.connected) {
      //alert('Error : Reconnecting when a socket connection was open');
      Drupal.Nodejs.socket.disconnect();
    }
    delete io.sockets[url];
    io.j = [];
  }
  //console.log('nodejs.js : Drupal.Nodejs.connect() : Drupal.settings.nodejs.flashPolicyPort = ' + Drupal.settings.nodejs.flashPolicyPort);
  Drupal.Nodejs.socket = io.connect(url, {'connect timeout': Drupal.settings.nodejs.connectTimeout
                                          , 'reconnect': false
                                          , 'secure': Drupal.settings.nodejs.secure? true : false
                                          , 'try multiple transports' : Drupal.settings.nodejs.tryMultipleTransport
                                          , 'flash policy port' : Drupal.settings.nodejs.flashPolicyPort
                                          //, 'max reconnection attempts' : Drupal.settings.nodejs.reconnectAttempts
                                          //, 'reconnection delay': Drupal.settings.nodejs.reconnectDelay
                                          //, 'reconnection limit': Drupal.settings.nodejs.reconnectLimit

                                          });
  //console.log(io);
  //console.log('io after io.connect()');

  // EXPERTUS : setTimout handler for checking connection moved up to before defining socket.on connect.
  //console.log('nodejs.js : Drupal.Nodejs.connect() : calling Drupal.Nodejs.checkConnection() in ' + (Drupal.settings.nodejs.connectTimeout + 250));  
  Drupal.Nodejs.expCheckConnection = setTimeout("Drupal.Nodejs.checkConnection()", Drupal.settings.nodejs.connectTimeout + 250);
  
  Drupal.Nodejs.socket.on('connect', function() {
    //console.log('nodejs.js : Drupal.Nodejs.connect() : socket.io "connect" event handler. Connection made using ' +
    //                                                                                   Drupal.Nodejs.socket.socket.transport.name);
    //console.log(io);
    //console.log('io after connect is completed');

    // EXPERTUS added : clear setTimeout handler for checking connection, as connection is now established
    if (Drupal.Nodejs.expCheckConnection) {
      //console.log('nodejs.js : Drupal.Nodejs.connect() : cancelled setTimeout call to Drupal.Nodejs.checkConnection() as connection is established');
      clearTimeout(Drupal.Nodejs.expCheckConnection);
    }
    delete Drupal.Nodejs.expCheckConnection;

    Drupal.Nodejs.sendAuthMessage();
    if (Drupal.Nodejs.authTimoutId) {
      clearTimeout(Drupal.Nodejs.authTimoutId);
    }
    //console.log('nodejs.js : Drupal.Nodejs.connect() : calling Drupal.Nodejs.reconnectOnAuthNoResponse() in ' + (Drupal.settings.nodejs.authTimeout)); 
    Drupal.Nodejs.authTimoutId = setTimeout("Drupal.Nodejs.reconnectOnAuthNoResponse()", Drupal.settings.nodejs.authTimeout);
    Drupal.Nodejs.runSetupHandlers('connect');
    // EXPERTUS : moved message event handler initialization out of here (see below), as multiple instances of the same event handler were getting set
    //            on the socket when the node server was stopped and restarted in the backend.
    //Drupal.Nodejs.socket.on('message', Drupal.Nodejs.runCallbacks);
    /* EXPERTUS - unused code, use unclear - commented to fix issue in mantis ticket #0020614
    if (Drupal.ajax != undefined) {
      // Monkey-patch Drupal.ajax.prototype.beforeSerialize to auto-magically
      // send sessionId for AJAX requests so we can exclude the current browser
      // window from resulting notifications. We do this so that modules can hook
      // in to other modules ajax requests without having to patch them.
      Drupal.Nodejs.originalBeforeSerialize = Drupal.ajax.prototype.beforeSerialize;
      Drupal.ajax.prototype.beforeSerialize = function(element_settings, options) {
        options.data['nodejs_client_socket_id'] = Drupal.Nodejs.socket.socket.sessionid;
        return Drupal.Nodejs.originalBeforeSerialize(element_settings, options);
      };
    }
    */
  });
  
  // Expertus : moved this line out of above connect event handler, as multiple instances of the same event handler were getting set
  //            on the socket when node server was stopped and restarted in the backend.
  //console.log('Initializing Drupal.Nodejs.socket.on message event handler');
  Drupal.Nodejs.socket.on('message', Drupal.Nodejs.runCallbacks);
  
  Drupal.Nodejs.socket.on('disconnect', function() {
    //console.log('nodejs.js : Drupal.Nodejs.socket on "disconnect" event handler');
    /* EXPERTUS - unused code, use unclear - commented to fix issue in mantis ticket #0020614
    if (Drupal.ajax != undefined) {
      Drupal.ajax.prototype.beforeSerialize = Drupal.Nodejs.originalBeforeSerialize;
    }
    */
    
    Drupal.Nodejs.runSetupHandlers('disconnect');
    
    // If disconnect is received while we are waiting for auth, there is no more a need for calling Drupal.Nodejs.reconnectOnAuthNoResponse()
    if (Drupal.Nodejs.authTimoutId) {
      //console.log('nodejs.js : Drupal.Nodejs.connect() : cancelled setTimeout to Drupal.Nodejs.reconnectOnAuthNoResponse() as socket disconnected');
      clearTimeout(Drupal.Nodejs.authTimoutId);
    }
    delete Drupal.Nodejs.authTimoutId;
    
    if (typeof Drupal.Nodejs.codeTriggeredDisconnectOnAuthNoResponse === 'undefined') {
      //console.log('nodejs.js : Drupal.Nodejs.socket.on disconnect : calling Drupal.Nodejs.reconnectWhenNotUnload() in ' +
      //                                                                                           Drupal.settings.nodejs.pageUnloadDelay);
      setTimeout("Drupal.Nodejs.reconnectWhenNotUnload()", Drupal.settings.nodejs.pageUnloadDelay);
    }
    else {
      //console.log('nodejs.js : Drupal.Nodejs.socket.on disconnect : deleting Drupal.Nodejs.codeTriggeredDisconnectOnAuthNoResponse');
      delete Drupal.Nodejs.codeTriggeredDisconnectOnAuthNoResponse;
      Drupal.Nodejs.reconnectWhenNotUnload(); // no need to wait to invoke reconnect logic when it is code triggered disconnect
    }
  });
  
  Drupal.Nodejs.socket.on('connecting', function () {  // EXPERTUS added
    //console.log('nodejs.js : Drupal.Nodejs.connect : socket.io "connecting" event handler');
    Drupal.Nodejs.runSetupHandlers('connecting');
  });
  
  Drupal.Nodejs.socket.on('exp-authenticated', function(data) {
    //console.log('nodejs.js : Drupal.Nodejs.connect : custom socket.io "exp-authenticated" event handler: ' + data.status);
    
    // Since response is received, there is no more a need to call Drupal.Nodejs.reconnectOnAuthNoResponse()
    if (Drupal.Nodejs.authTimoutId) {
      //console.log('nodejs.js : Drupal.Nodejs.connect() : cancelled setTimeout to Drupal.Nodejs.reconnectOnAuthNoResponse() as auth response received');
      clearTimeout(Drupal.Nodejs.authTimoutId);
    }
    delete Drupal.Nodejs.authTimoutId;
    
    // Delete reconnect counters, etc. as connection is now established and authenticated
    Drupal.Nodejs.cleanupAfterReconnectSuccessful();  // EXPERTUS added

    // Run any registered setup handlers for 'expAuthenticated'
    Drupal.Nodejs.runSetupHandlers('expAuthenticated');
  });
  
  Drupal.Nodejs.socket.on('exp-auth-failed', function(data) {
    //console.log('nodejs.js : Drupal.Nodejs.connect : custom socket.io "exp-auth-failed" event handler: ' + data.reason);
    
    // Since response is received, there is no more a need to call Drupal.Nodejs.reconnectOnAuthNoResponse()
    if (Drupal.Nodejs.authTimoutId) {
      //console.log('nodejs.js : Drupal.Nodejs.connect() : cancelled setTimeout to Drupal.Nodejs.reconnectOnAuthNoResponse() as authfail response received');
      clearTimeout(Drupal.Nodejs.authTimoutId);
    }
    delete Drupal.Nodejs.authTimoutId;

    // Permanently close the socket connection
    Drupal.Nodejs.socket.removeAllListeners();
    Drupal.Nodejs.socket.disconnect();
    
    // Run any registered setup handlers for 'expAuthFailed'
    Drupal.Nodejs.runSetupHandlers('expAuthFailed');
  });

};

Drupal.Nodejs.checkConnection = function () {
  //console.log('nodejs.js : In Drupal.Nodejs.checkConnection()');
  delete Drupal.Nodejs.expCheckConnection; // EXPERTUS - delete setTimeout global holder
  
  if (!Drupal.Nodejs.socket.socket.connected) {
    //console.log('nodejs.js : Drupal.Nodejs.checkConnection() : Executing connectionFailure handlers');
    Drupal.Nodejs.runSetupHandlers('connectionFailure');
    
    // Attempt a re-connect  (EXPERTUS added)
    Drupal.Nodejs.reconnect();
  }
};

Drupal.Nodejs.reconnect = function () { // EXPERTUS added
  //console.log('nodejs.js : In Drupal.Nodejs.reconnect()');
  if (Drupal.Nodejs.socket.socket.connected) {
    //alert('Error: Reconnecting when socket connection is still open');
  }
  
  // Attempt reconnection
  if (typeof Drupal.Nodejs.reconnectCount === 'undefined') {
    Drupal.Nodejs.reconnectCount = 0;
    Drupal.Nodejs.currentReconnectDelay =
                  Math.floor((Math.random() * (Drupal.settings.nodejs.maxInitialReconnectDelay - Drupal.settings.nodejs.minInitialReconnectDelay))
                                                  + Drupal.settings.nodejs.minInitialReconnectDelay);
     //console.log('nodejs.js : Drupal.Nodejs.reconnect() : Drupal.Nodejs.currentReconnectDelay = ' + Drupal.Nodejs.currentReconnectDelay);
  }

  if (Drupal.Nodejs.reconnectCount < Drupal.settings.nodejs.reconnectAttempts) {
    if (Drupal.Nodejs.reconnectCount > 0) {
      Drupal.Nodejs.currentReconnectDelay *= 2;
      if (Drupal.Nodejs.currentReconnectDelay > Drupal.settings.nodejs.reconnectLimit) {
        Drupal.Nodejs.currentReconnectDelay =
          Math.floor((Math.random() * (Drupal.settings.nodejs.maxInitialReconnectDelay - Drupal.settings.nodejs.minInitialReconnectDelay))
              + Drupal.settings.nodejs.minInitialReconnectDelay); // start all over again
      }
      //console.log('nodejs.js : Drupal.Nodejs.reconnect() : Drupal.Nodejs.currentReconnectDelay = ' + Drupal.Nodejs.currentReconnectDelay);      
    }
    
    //console.log('nodejs.js : Drupal.Nodejs.reconnect() : calling Drupal.Nodejs.connect(true) in ' + Drupal.Nodejs.currentReconnectDelay);
    setTimeout("Drupal.Nodejs.connect(true)", Drupal.Nodejs.currentReconnectDelay);
    Drupal.Nodejs.reconnectCount += 1;
  }
  else {
    // Permanently close the socket connection
    Drupal.Nodejs.socket.removeAllListeners();

    //console.log('nodejs.js : Drupal.Nodejs.reconnect() : Executing connectionFatalFailure handlers');
    Drupal.Nodejs.runSetupHandlers('connectionFatalFailure');
  }
}

Drupal.Nodejs.cleanupAfterReconnectSuccessful = function() {  // EXPERTUS added
  //console.log('nodejs.js : In Drupal.Nodejs.cleanupAfterReconnectSuccessful()');
  delete Drupal.Nodejs.reconnectCount;
  delete Drupal.Nodejs.currentReconnectDelay;
}

Drupal.Nodejs.sendAuthMessage = function () {
  //console.log('nodejs.js : In Drupal.Nodejs.sendAuthMessage()');
  var authMessage = {
    authToken: Drupal.settings.nodejs.authToken,
    contentTokens: Drupal.settings.nodejs.contentTokens
  };
  Drupal.Nodejs.socket.emit('authenticate', authMessage);
};

Drupal.Nodejs.reconnectOnAuthNoResponse = function () {
  //console.log('nodejs.js : In Drupal.Nodejs.reconnectOnAuthNoResponse()');
  delete Drupal.Nodejs.authTimoutId;

  //console.log('nodejs.js : In Drupal.Nodejs.reconnectOnAuthNoResponse() : setting Drupal.Nodejs.codeTriggeredDisconnectOnAuthNoResponse');
  Drupal.Nodejs.codeTriggeredDisconnectOnAuthNoResponse = true;
  
  //console.log('nodejs.js : In Drupal.Nodejs.reconnectOnAuthNoResponse() : triggering a reconnect by disconnecting the socket');
  Drupal.Nodejs.socket.disconnect();
}

})(jQuery);

Drupal.Nodejs.reconnectWhenNotUnload = function() {
  //console.log('nodejs.js : Drupal.Nodejs.reconnectWhenNotUnload() : Executing connectionFailure handlers');
  Drupal.Nodejs.runSetupHandlers('connectionFailure');
  
  Drupal.Nodejs.reconnect();
};

/*
 * In IE (uses flash socket), for some reason socket connection is not disconnecting on the first destroy of tab after logging in. This resulted in
 * any messages for this user not getting queued and not getting delivered when the user came back again. So, manually triggering the disconnect on
 * window unload.
 */
jQuery(window).unload(function(){
  //console.log('nodejs.js : window unload callback');
  if (typeof Drupal.Nodejs.socket != 'undefined'
    && typeof Drupal.Nodejs.socket.socket != 'undefined'
        && typeof Drupal.Nodejs.socket.socket.connected != 'undefined') {
    Drupal.Nodejs.socket.removeAllListeners();
    if (Drupal.Nodejs.socket.socket.connected) {
      Drupal.Nodejs.socket.disconnect();
    }
  }
});

// vi:ai:expandtab:sw=2 ts=2

