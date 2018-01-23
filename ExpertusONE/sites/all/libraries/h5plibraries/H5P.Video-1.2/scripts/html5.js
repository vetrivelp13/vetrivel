/** @namespace H5P */
H5P.VideoHtml5 = (function ($) {

  /**
   * HTML5 video player for H5P.
   *
   * @class
   * @param {Array} sources Video files to use
   * @param {Object} options Settings for the player
   * @param {Object} l10n Localization strings
   */
  function Html5(sources, options, l10n) {
    var self = this;

    /**
     * Displayed when the video is buffering
     * @private
     */
    var $throbber = $('<div/>', {
      'class': 'h5p-video-loading'
    });

    /**
     * Used to display error messages
     * @private
     */
    var $error = $('<div/>', {
      'class': 'h5p-video-error'
    });

    /**
     * Keep track of current state when changing quality.
     * @private
     */
    var stateBeforeChangingQuality;
    var currentTimeBeforeChangingQuality;

    /**
     * Avoids firing the same event twice.
     * @private
     */
    var lastState;

    // Create player
    var video = document.createElement('video');

    // Sort sources into qualities
    var qualities = getQualities(sources, video);

    // Select quality and source
    var currentQuality = getPreferredQuality();
    if (currentQuality === undefined || qualities[currentQuality] === undefined) {
      // No preferred quality, pick the first.
      for (currentQuality in qualities) {
        if (qualities.hasOwnProperty(currentQuality)) {
          break;
        }
      }
    }
    video.src = qualities[currentQuality].source.path;

    // Set options
    video.controls = (options.controls ? true : false);
    video.autoplay = (options.autoplay ? true : false);
    video.loop = (options.loop ? true : false);
    video.className = 'h5p-video';
    
    video.style.display = 'none';
    video.style.position="absolute"; //for safari avoid sliding the video editor. 
    video.style.top="0";
    video.style.left="0";
    
    if (options.fit) {
      // Style is used since attributes with relative sizes aren't supported by IE9.
      video.style.width = '100%';
      video.style.height = '100%';
    }
    // Add poster if provided
    if (options.poster) {
      video.poster = options.poster;
    }

    /**
     * Register track to video
     *
     * @param {Object} trackData Track object
     * @param {string} trackData.kind Kind of track
     * @param {Object} trackData.track Source path
     * @param {string} [trackData.label] Label of track
     * @param {string} [trackData.srcLang] Language code
     */
    var addTrack = function (trackData) {
      // Skip invalid tracks
      if (!trackData.kind || !trackData.track.path) {
        return;
      }

      var track = document.createElement('track');
      track.kind = trackData.kind;
      track.src = trackData.track.path;
      if (trackData.label) {
        track.label = trackData.label;
      }

      if (trackData.srcLang) {
        track.srcLang = trackData.srcLang;
      }

      return track;
    };

    // Register tracks
    options.tracks.forEach(function (track, i) {
      var trackElement = addTrack(track);
      if (i === 0) {
        trackElement.default = true;
      }
      if (trackElement) {
        video.appendChild(trackElement);
      }
    });

    /**
     * Helps registering events.
     *
     * @private
     * @param {String} native Event name
     * @param {String} h5p Event name
     * @param {String} [arg] Optional argument
     */
    var mapEvent = function (native, h5p, arg) {
      video.addEventListener(native, function () {
        switch (h5p) {
          case 'stateChange':
            if (lastState === arg) {
              return; // Avoid firing event twice.
            }

            var validStartTime = options.startAt && options.startAt > 0;
            if (arg === H5P.Video.PLAYING && validStartTime) {
              video.currentTime = options.startAt;
              delete options.startAt;
            }
            break;

          case 'loaded':
            if (stateBeforeChangingQuality !== undefined) {
              return; // Avoid loaded event when changing quality.
            }

            // Remove any errors
            if ($error.is(':visible')) {
              $error.remove();
            }

            if (OLD_ANDROID_FIX) {
              var andLoaded = function () {
                video.removeEventListener('durationchange', andLoaded, false);
                // On Android seeking isn't ready until after play.
                self.trigger(h5p);
              };
              video.addEventListener('durationchange', andLoaded, false);
              return;
            }
            break;

          case 'error':
            // Handle error and get message.
            arg = error(arguments[0], arguments[1]);
            break;
        }
        self.trigger(h5p, arg);
      }, false);
    };

    /**
     * Handle errors from the video player.
     *
     * @private
     * @param {Object} code Error
     * @param {String} [message]
     * @returns {String} Human readable error message.
     */
    var error = function (code, message) {
      if (code instanceof Event) {

        // No error code
        if (!code.target.error) {
          return '';
        }

        switch (code.target.error.code) {
          case MediaError.MEDIA_ERR_ABORTED:
            message = l10n.aborted;
            break;
          case MediaError.MEDIA_ERR_NETWORK:
            message = l10n.networkFailure;
            break;
          case MediaError.MEDIA_ERR_DECODE:
            message = l10n.cannotDecode;
            break;
          case MediaError.MEDIA_ERR_SRC_NOT_SUPPORTED:
            message = l10n.formatNotSupported;
            break;
          case MediaError.MEDIA_ERR_ENCRYPTED:
            message = l10n.mediaEncrypted;
            break;
        }
      }
      if (!message) {
        message = l10n.unknownError;
      }

      // Hide throbber
      $throbber.remove();

      // Display error message to user
      $error.text(message).insertAfter(video);

      // Pass message to our error event
      return message;
    };

    /**
     * Appends the video player to the DOM.
     *
     * @public
     * @param {jQuery} $container
     */
    self.appendTo = function ($container) {
    	  //$(".h5p-editor-iframe").contents().find(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide()
  	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide(); //Interactive editor title
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-tabs").hide(); //tabs

    	  $container.append(video);
      H5P.jQuery("video.h5p-video").css("display","block");
      H5P.jQuery("video.h5p-video").css("position","relative"); 
      
    };
    

    self.attachVideoJS = function($container){
     	$wrapper.appendTo($container);
     	window.parent.$(".loadercontent").remove();
        var h = window.parent.$("#iframedata").height()-38;
     	var w = window.parent.$("#iframedata").width();
     	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/video.js");
     	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/Vimeo.js");
     	loadScriptsForTinyMCE("/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/vimeoplayer.js");
     	
     	videojs.options.flash.swf = "/sites/all/modules/core/exp_sp_core/js/videojs/video-js.swf";
    	
    	H5P.jQuery(".h5p-interactive-video").css("height","inherit");
    	H5P.jQuery(".h5p-video-wrapper").css("height","inherit");
    	window.parent.$(".loadercontent").remove();	
    	
    	if(sources[0].path.indexOf("vimeo") > 0)
    	{
    		// get vimeo id from url
    		var r = /(videos|video|channels|\.com)\/([\d]+)/;
    		var vimeoId=sources[0].path.match(r)[2];
    		

    		html = '<iframe style="height:180%;width:100%;left:0;position:absolute;top:-40%" src="https://player.vimeo.com/video/'+vimeoId+'?player_id=my_video_2" id="my_video_2" class="Fullsc video-js" width="100%" height="100%" frameborder="0" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>';
    		// html = '<iframe style="width:100%;"
    		// src="https://player.vimeo.com/video/76979871?api=1&player_id=my_video_2"
    		// id="my_video_2" class="Fullsc video-js" width="100%" height="100%"
    		// frameborder="0" webkitallowfullscreen mozallowfullscreen
    		// allowfullscreen></iframe>';
    		H5P.jQuery(".h5p-video-flash").append(html);
    		var iframe = document.getElementById('my_video_2');
    	    fp = new Vimeo.Player(iframe);
    	    console.log("fp:",fp);
    	    console.log("size::",H5P.jQuery(".h5p-video-flash").size());
    	    
    	    window.setTimeout(function(){
    	    	    
    	    	fp.on("loaded", function () {
    	    	console.log("fp.addEvent ready1....");
    	   	fp.play();
    	    	window.parent.$("#iframe_editor_wrapper").css("visibility","visible");

    	    fp.getDuration().then(function(duration) {
    	    		vimeoDuration = duration;
    	    }).catch(function(error) {
    	        // an error occurred
    	    });
    	    
    	    
    	    

    		
    		fp.getVideoWidth().then(function(width) {
    			vimeoWidth = width;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    		fp.getVideoHeight().then(function(height) {
    			vimeoHeight = height;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    		fp.on('timeupdate',function(evt){
        		vimeoCurrentTime = evt.seconds;
    		})
    		fp.on('progress',function(evt){
        			vimeoBuffered = evt.percent*100;
    		})

    	
    	    
    	    
    	    
    	    
    	    fp.on('play', function() {
    	    		self.trigger('stateChange', H5P.Video.PLAYING);
    	        console.log('played the video!');
    	    });

    	    fp.on('pause', function() {
    	    		self.trigger('stateChange', H5P.Video.PAUSED);
    	        console.log('pause the video!');
    	    });
    	    fp.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
    	    });
    	    },1000); // timeout

    	}
    	else
    	{
    		html =   "<video id='my_video_2' class='Fullsc video-js' autoplay=true controls='false' preload='auto' width='"+w+"' height='"+h+"' data-setup=\"{}\">";
    	// html += "<source
    	// src='rtmp://192.241.173.210/oflaDemo/Saravanan_4/256/Saravanan.mp4'
    	// type='rtmp/mp4' />"; //what about webm?
    	    html += "<source src='"+sources[0].path+"' type='rtmp/mp4' />";
    		html +="</video>";

    	H5P.jQuery(".h5p-video-flash").append(html);

    	console.log("1115:"+sources[0].path);
    	var playing = false,firstTime = true;
    	fp = videojs("my_video_2",{controls: false}).ready(function(){
      	var player = this;
      	// this.play();
        player.on( 'loadedmetadata', 	function( e ) { 
      																				console.log('load meta...');
      																				self.trigger('ready');
      																				self.trigger('loaded');
      																				self.trigger("addControls");
      																				window.setTimeout(function(){
      																				playing = true;
    																				firstTime = false;
      																				
      																				self.trigger('stateChange', H5P.Video.PLAYING);
      																				},50);
      														
      												});
    	// player.on( 'timeupdate', function( e ) { window.console.log( 'videoJS:
    	// ended111' );self.trigger('stateChange', H5P.Video.ENDED);});

    	player.on( 'ended', 			function( e ) { window.console.log( 'videoJS: ended111' );self.trigger('stateChange', H5P.Video.ENDED);});
    	player.on( 'pause', 			function( e ) { window.console.log( 'videoJS: pause' );
    				firstTime = false;
    				self.trigger('stateChange', H5P.Video.PAUSED); 
    				});
    	
    	
    	player.on( 'play', 				function( e ) {
    													if(firstTime == false)
    														self.trigger('stateChange', H5P.Video.PLAYING);
    												 });
    	
    	
    	player.on( 'seeking', 			function( e ) { console.log( 'videoJS: seeking' );
    	
    			self.trigger('stateChange', H5P.Video.BUFFERING); 
    			});
    	// play/pause
    	var obt = H5P.jQuery(".vjs-text-track-display");  	
      	obt.css({ "pointer-events": "auto" });  	
      	obt.on('click', function() {  		
      		if (player.paused()) { 
      			player.play();
      	  	  }else if(player.play()){
      	  		player.pause();
      	  	  }
      	});
      	
    	/*
    	 * player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
    	 * waiting' ); //self.trigger('stateChange', H5P.Video.BUFFERING); });
    	 */


    	// player.on( 'loadstart', function( e ) { window.console.log( 'videoJS:
    	// loadstart' ); self.trigger('stateChange', H5P.Video.PLAYING);});
    	
    	
      	// player.on( 'durationchange', function( e ) { window.console.log(
    	// 'videoJS: durationchange' ); self.trigger('stateChange',
    	// H5P.Video.PLAYING); });
    	
    	// player.on( 'error', function( e ) { window.console.log( 'videoJS: error'
    	// ); self.trigger('error', message);});
    	
    	// player.on( 'loadedalldata', function( e ) { window.console.log( 'videoJS:
    	// loadedalldata' );self.trigger('stateChange', H5P.Video.PLAYING); });
    	
    	
    	
    	
    	
    	// player.on( 'seeked', function( e ) { alert( 'videoJS: seeked' ); });
    	// player.on( 'seeking', function( e ) { alert( 'videoJS: seeking'
    	// );self.trigger('stateChange', InteractiveVideo.SEEKING); });
    	// player.on( 'waiting', function( e ) { window.console.log( 'videoJS:
    	// waiting' );self.trigger('stateChange', H5P.Video.BUFFERING); });
    // player.on( 'contentplayback', function( e ) { window.console.log( 'videoJS:
    // contentplayback' );self.trigger('stateChange', H5P.Video.PLAYING); });
    	});
    	}
    	
    	H5P.jQuery(".h5p-video").css("margin-top","0px");
    	
     	};
     	

    /**
     * Get list of available qualities. Not available until after play.
     *
     * @public
     * @returns {Array}
     */
    self.getQualities = function () {
      // Create reverse list
      var options = [];
      for (var q in qualities) {
        if (qualities.hasOwnProperty(q)) {
          options.splice(0, 0, {
            name: q,
            label: qualities[q].label
          });
        }
      }

      if (options.length < 2) {
        // Do not return if only one quality.
        return;
      }

      return options;
    };

    /**
     * Get current playback quality. Not available until after play.
     *
     * @public
     * @returns {String}
     */
    self.getQuality = function () {
      return currentQuality;
    };

    /**
     * Set current playback quality. Not available until after play.
     * Listen to event "qualityChange" to check if successful.
     *
     * @public
     * @params {String} [quality]
     */
    self.setQuality = function (quality) {
      if (qualities[quality] === undefined || quality === currentQuality) {
        return; // Invalid quality
      }

      // Keep track of last choice
      setPreferredQuality(quality);

      // Avoid multiple loaded events if changing quality multiple times.
      if (!stateBeforeChangingQuality) {
        // Keep track of last state
        stateBeforeChangingQuality = lastState;

        // Keep track of current time
        currentTimeBeforeChangingQuality = video.currentTime;

        // Seek and start video again after loading.
        var loaded = function () {
          video.removeEventListener('loadedmetadata', loaded, false);
          if (OLD_ANDROID_FIX) {
            var andLoaded = function () {
              video.removeEventListener('durationchange', andLoaded, false);
              // On Android seeking isn't ready until after play.
              self.seek(currentTimeBeforeChangingQuality);
            };
            video.addEventListener('durationchange', andLoaded, false);
          }
          else {
            // Seek to current time.
            self.seek(currentTimeBeforeChangingQuality);
          }

          // Always play to get image.
          video.play();

          if (stateBeforeChangingQuality !== H5P.Video.PLAYING) {
            // Do not resume playing
            video.pause();
          }

          // Done changing quality
          stateBeforeChangingQuality = undefined;

          // Remove any errors
          if ($error.is(':visible')) {
            $error.remove();
          }
        };
        video.addEventListener('loadedmetadata', loaded, false);
      }

      // Keep track of current quality
      currentQuality = quality;
      self.trigger('qualityChange', currentQuality);

      // Display throbber
      self.trigger('stateChange', H5P.Video.BUFFERING);

      // Change source
      video.src = qualities[quality].source.path; // (iPad does not support #t=).
    };

    /**
     * Starts the video.
     *
     * @public
     */
    self.play = function () {
      if ($error.is(':visible')) {
        return;
      }

      video.play();
    };

    /**
     * Pauses the video.
     *
     * @public
     */
    self.pause = function () {
      video.pause();
    };

    /**
     * Seek video to given time.
     *
     * @public
     * @param {Number} time
     */
    self.seek = function (time) {
      if (lastState === undefined) {
        // Make sure we always play before we seek to get an image.
        // If not iOS devices will reset currentTime when pressing play.
        video.play();
        video.pause();
      }

      video.currentTime = time;
    };

    /**
     * Get elapsed time since video beginning.
     *
     * @public
     * @returns {Number}
     */
    self.getCurrentTime = function () {
      return video.currentTime;
    };

    /**
     * Get total video duration time.
     *
     * @public
     * @returns {Number}
     */
    self.getDuration = function () {
      if (isNaN(video.duration)) {
        return;
      }

      return  video.duration;
    };

    /**
     * Get percentage of video that is buffered.
     *
     * @public
     * @returns {Number} Between 0 and 100
     */
    self.getBuffered = function () {
      // Find buffer currently playing from
      var buffered = 0;
      for (var i = 0; i < video.buffered.length; i++) {
        var from = video.buffered.start(i);
        var to = video.buffered.end(i);

        if (video.currentTime > from && video.currentTime < to) {
          buffered = to;
          break;
        }
      }

      // To percentage
      return buffered ? (buffered / video.duration) * 100 : 0;
    };

    /**
     * Turn off video sound.
     *
     * @public
     */
    self.mute = function () {
      video.muted = true;
    };

    /**
     * Turn on video sound.
     *
     * @public
     */
    self.unMute = function () {
      video.muted = false;
    };

    /**
     * Check if video sound is turned on or off.
     *
     * @public
     * @returns {Boolean}
     */
    self.isMuted = function () {
      return video.muted;
    };

    /**
     * Returns the video sound level.
     *
     * @public
     * @returns {Number} Between 0 and 100.
     */
    self.getVolume = function () {
      return video.volume * 100;
    };

    /**
     * Set video sound level.
     *
     * @public
     * @param {Number} level Between 0 and 100.
     */
    self.setVolume = function (level) {
      video.volume = level / 100;
    };

    // Register event listeners
    mapEvent('ended', 'stateChange', H5P.Video.ENDED);
    mapEvent('playing', 'stateChange', H5P.Video.PLAYING);
    mapEvent('pause', 'stateChange', H5P.Video.PAUSED);
    mapEvent('waiting', 'stateChange', H5P.Video.BUFFERING);
    mapEvent('loadedmetadata', 'loaded');
    mapEvent('error', 'error');

    if (!video.controls) {
      // Disable context menu(right click) to prevent controls.
      video.addEventListener('contextmenu', function (event) {
        event.preventDefault();
      }, false);
    }

    // Display throbber when buffering/loading video.
    self.on('stateChange', function (event) {
      var state = event.data;
      lastState = state;
      if (state === H5P.Video.BUFFERING) {
        $throbber.insertAfter(video);
      }
      else {
        $throbber.remove();
      }
    });

    // Video controls are ready
    setTimeout(function () {
      self.trigger('ready');
      
      H5P.jQuery(window).keypress(function(e) {
    	      var keyCode = e.keyCode || e.charCode;
    	      
          if (keyCode == 32) { //space bar
        	  e.preventDefault();
        	  if(e.target.tagName.toLowerCase() == "body" || e.target.tagName.toLowerCase() == "video") 
        		  H5P.jQuery("video.h5p-video").get(0).paused?H5P.jQuery("video.h5p-video").get(0).play() :H5P.jQuery("video.h5p-video").get(0).pause();
          }
      });
      
      
      
    }, 0);
  }

  /**
   * Check to see if we can play any of the given sources.
   *
   * @public
   * @static
   * @param {Array} sources
   * @returns {Boolean}
   */
  Html5.canPlay = function (sources) {
    var video = document.createElement('video');
    if (video.canPlayType === undefined) {
      return false; // Not supported
    }
    
    // Cycle through sources
    for (var i = 0; i < sources.length; i++) {
      //if it is an editor, use h5p video handlers. Do not return false to attach videojs	
    	
    	 	var playerType = "flowplayer";
 	    if(sources[i].path.indexOf("rtmp://")==0)
 	    		playerType = "videojs";
 	    else if(sources[i].path.indexOf("vimeo")>0 )
 	    		playerType = "vimeo";
 	   console.log("playerType:",playerType); 
 	   
      if(sources[i].path.indexOf("rtmp:") >=0  && (window.parent.$("#iframe_editor_wrapper_container").size() == 0 && window.parent.$("#contentauthor-presentation-addedit-form").size() == 0))
      	return false;
      var type = getType(sources[i]);
      if(playerType == "vimeo")
    	    return false;
      if (type && video.canPlayType(type) !== '') {
        // We should be able to play this
        return true;
      }
    }

    return false;
  };

  /**
   * Find source type.
   *
   * @private
   * @param {Object} source
   * @returns {String}
   */
  var getType = function (source) {
    var type = source.mime;
    if (!type) {
      // Try to get type from URL
      var matches = source.path.match(/\.(\w+)$/);
      if (matches && matches[1]) {
        type = 'video/' + matches[1];
      }
    }

    if (type && source.codecs) {
      // Add codecs
      type += '; codecs="' + source.codecs + '"';
    }

    return type;
  };

  /**
   * Sort sources into qualities.
   *
   * @private
   * @static
   * @param {Array} sources
   * @param {Object} video
   * @returns {Object} Quality mapping
   */
  var getQualities = function (sources, video) {
    var qualities = {};
    var qualityIndex = 1;
    var lastQuality;

    // Cycle through sources
    for (var i = 0; i < sources.length; i++) {
      var source = sources[i];

      // Find and update type.
      var type = source.type = getType(source);

      // Check if we support this type
      if (!type || video.canPlayType(type) === '') {
        continue; // We cannot play this source
      }

      if (source.quality === undefined) {
        /* No quality metadata. Create a dummy tag to seperate multiple
        sources of the same type, e.g. if two mp4 files have been uploaded. */

        if (lastQuality === undefined || qualities[lastQuality].source.type === type) {
          // Create a new quality tag
          source.quality = {
            name: 'q' + qualityIndex,
            label: 'Quality ' + qualityIndex // TODO: l10n
          };
          qualityIndex++;
        }
        else {
          // Tag as the same quality as the last source
          source.quality = qualities[lastQuality].source.quality;
        }
      }

      // Log last quality
      lastQuality = source.quality.name;

      // Look to see if quality exists
      var quality = qualities[lastQuality];
      if (quality) {
        // We have a source with this quality. Check if we have a better format.
        if (source.mime.split('/')[1] === PREFERRED_FORMAT) {
          quality.source = source;
        }
      }
      else {
        // Add new source with quality.
        qualities[source.quality.name] = {
          label: source.quality.label,
          source: source
        };
      }
    }

    return qualities;
  };

  /**
   * Set preferred video quality.
   *
   * @private
   * @static
   * @param {String} quality Index of preferred quality
   */
  var setPreferredQuality = function (quality) {
    var settings = document.cookie.split(';');
    for (var i = 0; i < settings.length; i++) {
      var setting = settings[i].split('=');
      if (setting[0] === 'H5PVideoQuality') {
        setting[1] = quality;
        settings[i] = setting.join('=');
        document.cookie = settings.join(';');
        return;
      }
    }

    document.cookie = 'H5PVideoQuality=' + quality + '; ' + document.cookie;
  };

  /**
   * Set preferred video quality.
   *
   * @private
   * @static
   * @returns {String} Index of preferred quality
   */
  var getPreferredQuality = function () {
    var quality, settings = document.cookie.split(';');
    for (var i = 0; i < settings.length; i++) {
      var setting = settings[i].split('=');
      if (setting[0] === 'H5PVideoQuality') {
        quality = setting[1];
        break;
      }
    }

    return quality;
  };

  /** @constant {Boolean} */
  var OLD_ANDROID_FIX = false;

  /** @constant {Boolean} */
  var PREFERRED_FORMAT = 'mp4';

  if (navigator.userAgent.indexOf('Android') !== -1) {
    // We have Android, check version.
    var version = navigator.userAgent.match(/AppleWebKit\/(\d+\.?\d*)/);
    if (version && version[1] && Number(version[1]) <= 534.30) {
      // Include fix for devices running the native Android browser.
      // (We don't know when video was fixed, so the number is just the lastest
      // native android browser we found.)
      OLD_ANDROID_FIX = true;
    }
  }
  else {
    if (navigator.userAgent.indexOf('Chrome') !== -1) {
      // If we're using chrome on a device that isn't Android, prefer the webm
      // format. This is because Chrome has trouble with some mp4 codecs.
      PREFERRED_FORMAT = 'webm';
    }
  }

  return Html5;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoHtml5);
