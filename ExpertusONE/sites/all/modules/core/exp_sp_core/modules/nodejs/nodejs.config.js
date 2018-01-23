/*
 * For a more fully commented example see the file nodejs.config.js.example in the root of this module
 */
settings = {
  "scheme" : "http"
  //"scheme" : "https"
  , "port" : 8080
  , "host" : "localhost"
  //, "sslKeyPath" : "RSA private key file path (.key)"
  //, "sslCertPath" : "SSL certificate file path (.crt)"
  //, "passPhrase" : "the pass phrase used when generating the RSA private key"
  , "serviceKey" : "DUMMY-SERVICE-KEY"
  , "flashPolicy" : {
      "port" : 10843
  }
  , "backend" : {
      "port" : 80
      //"port" : 443
      , "scheme" : "http"
      //, "scheme" : "https"
      , "host" : "localhost"
  }
  , "debug" : true
  , "eraseDrupalAuthAfter" : 120000 // Wait time (milli-seconds) before re-confirming and invalidating a drupal authentication record for a user session
                                    // after loosing all sockets. (default 2 min)
  , "publishQueuedMsgsBatchSize" : 5 // Number of queued messages that would be published together to the client for shared acknowledgement (default: 5)
  , "msgAckTimeoutAfter" : 30000 // Time within which client has to acknowledge the published batch of messages (default: 30 sec)
  , "publishNextMsgsBatchAfter" : 200 // Time after which the next batch of messages from queue would be sent by server to the client after
                                      // client has acknowledged the previous batch (default: 200ms) 
  , "drupalchatNodejs" : {
      "showUserPicture" : true
    , "eraseUserAfter" : 45000 // Wait time (milli-seconds) before re-confirming and invalidating chat user details and announce the user as having gone
                               // offline on detecting no socket connection for that that user. (default: 45 sec)
    , "currTheme" : "expertusoneV2" // ExpertusONE product theme. Available theme options are "expertusoneV2" and "expertusone". (default: "expertusoneV2")
    //, "currTheme" : "expertusone"
  }
};
