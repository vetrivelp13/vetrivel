;/** @namespace H5P */
H5P.VideoYouTube = (function ($) {

  /**
   * YouTube video player for H5P.
   *
   * @class
   * @param {Array} sources Video files to use
   * @param {Object} options Settings for the player
   * @param {Object} l10n Localization strings
   */
  function YouTube(sources, options, l10n) {
    var self = this;

    var player;
    var id = 'h5p-youtube-' + numInstances;
    numInstances++;

    var $wrapper = $('<div/>');
    var $placeholder = $('<div/>', {
      id: id,
      text: l10n.loading
    }).appendTo($wrapper);


    /**
     * Use the YouTube API to create a new player
     *
     * @private
     */
    var create = function () {
      if (!$placeholder.is(':visible') || player !== undefined) {
        return;
      }

      if (window.YT === undefined) {
        // Load API first
        loadAPI(create);
        return;
      }

      var width = $wrapper.width();
      if (width < 200) {
        width = 200;
      }

      player = new YT.Player(id, {
        width: width,
        height: width * (9/16),
        videoId: getId(sources[0].path),
        playerVars: {
          origin: ORIGIN,
          autoplay: options.autoplay ? 1 : 0,
          controls: options.controls ? 1 : 0,
          disablekb: options.controls ? 0 : 1,
          fs: 0,
          loop: options.loop ? 1 : 0,
          rel: 0,
          showinfo: 0,
          iv_load_policy: 3,
          wmode: "opaque",
          start: options.startAt
        },
        events: {
          onReady: function () {
            self.trigger('ready');
            self.trigger('loaded');
          },
          onStateChange: function (state) {
            if (state.data > -1 && state.data < 4) {
              self.trigger('stateChange', state.data);
            }
          },
          onPlaybackQualityChange: function (quality) {
            self.trigger('qualityChange', quality.data);
          },
          onError: function (error) {
            var message;
            switch (error.data) {
              case 2:
                message = l10n.invalidYtId;
                break;

              case 100:
                message = l10n.unknownYtId;
                break;

              case 101:
              case 150:
                message = l10n.restrictedYt;
                break;

              default:
                message = l10n.unknownError + ' ' + error.data;
                break;
            }
            self.trigger('error', message);
          }
        }
      });
    };

    /**
     * Indicates if the video must be clicked for it to start playing.
     * For instance YouTube videos on iPad must be pressed to start playing.
     *
     * @public
     */
    self.pressToPlay = navigator.userAgent.match(/iPad/i) ? true : false;

    /**
    * Appends the video player to the DOM.
    *
    * @public
    * @param {jQuery} $container
    */
    self.appendTo = function ($container) {
      $container.addClass('h5p-youtube').append($wrapper);
      create();
    };

    /**
     * Get list of available qualities. Not available until after play.
     *
     * @public
     * @returns {Array}
     */
    self.getQualities = function () {
      if (!player || !player.getAvailableQualityLevels) {
        return;
      }

      var qualities = player.getAvailableQualityLevels();
      if (!qualities.length) {
        return; // No qualities
      }

      // Add labels
      for (var i = 0; i < qualities.length; i++) {
        var quality = qualities[i];
        var label = (LABELS[quality] !== undefined ? LABELS[quality] : 'Unknown'); // TODO: l10n
        qualities[i] = {
          name: quality,
          label: LABELS[quality]
        };
      }

      return qualities;
    };

    /**
     * Get current playback quality. Not available until after play.
     *
     * @public
     * @returns {String}
     */
    self.getQuality = function () {
      if (!player || !player.getPlaybackQuality) {
        return;
      }

      var quality = player.getPlaybackQuality();
      return quality === 'unknown' ? undefined : quality;
    };

    /**
     * Set current playback quality. Not available until after play.
     * Listen to event "qualityChange" to check if successful.
     *
     * @public
     * @params {String} [quality]
     */
    self.setQuality = function (quality) {
      if (!player || !player.setPlaybackQuality) {
        return;
      }

      player.setPlaybackQuality(quality);
    };

    /**
     * Starts the video.
     *
     * @public
     */
    self.play = function () {
      if (!player || !player.playVideo) {
        self.on('ready', self.play);
        return;
      }

      player.playVideo();
    };

    /**
     * Pauses the video.
     *
     * @public
     */
    self.pause = function () {
      self.off('ready', self.play);
      if (!player || !player.pauseVideo) {
        return;
      }
      player.pauseVideo();
    };

    /**
     * Seek video to given time.
     *
     * @public
     * @param {Number} time
     */
    self.seek = function (time) {
      if (!player || !player.seekTo) {
        return;
      }

      player.seekTo(time, true);
    };

    /**
     * Get elapsed time since video beginning.
     *
     * @public
     * @returns {Number}
     */
    self.getCurrentTime = function () {
      if (!player || !player.getCurrentTime) {
        return;
      }

      return player.getCurrentTime();
    };

    /**
     * Get total video duration time.
     *
     * @public
     * @returns {Number}
     */
    self.getDuration = function () {
      if (!player || !player.getDuration) {
        return;
      }

      return player.getDuration();
    };

    /**
     * Get percentage of video that is buffered.
     *
     * @public
     * @returns {Number} Between 0 and 100
     */
    self.getBuffered = function () {
      if (!player || !player.getVideoLoadedFraction) {
        return;
      }

      return player.getVideoLoadedFraction() * 100;
    };

    /**
     * Turn off video sound.
     *
     * @public
     */
    self.mute = function () {
      if (!player || !player.mute) {
        return;
      }

      player.mute();
    };

    /**
     * Turn on video sound.
     *
     * @public
     */
    self.unMute = function () {
      if (!player || !player.unMute) {
        return;
      }

      player.unMute();
    };

    /**
     * Check if video sound is turned on or off.
     *
     * @public
     * @returns {Boolean}
     */
    self.isMuted = function () {
      if (!player || !player.isMuted) {
        return;
      }

      return player.isMuted();
    };

    /**
     * Returns the video sound level.
     *
     * @public
     * @returns {Number} Between 0 and 100.
     */
    self.getVolume = function () {
      if (!player || !player.getVolume) {
        return;
      }

      return player.getVolume();
    };

    /**
     * Set video sound level.
     *
     * @public
     * @param {Number} level Between 0 and 100.
     */
    self.setVolume = function (level) {
      if (!player || !player.setVolume) {
        return;
      }

      player.setVolume(level);
    };

    // Respond to resize events by setting the YT player size.
    self.on('resize', function () {
      if (!player) {
        // Player isn't created yet. Try again.
        create();
        return;
      }

      // Use as much space as possible
      $wrapper.css({
        width: '100%',
        height: '100%'
      });

      var width = $wrapper[0].clientWidth;
      var height = options.fit ? $wrapper[0].clientHeight : (width * (9/16));

      // Set size
      $wrapper.css({
        width: width + 'px',
        height: height + 'px'
      });

      player.setSize(width, height);
    });
  }

  /**
   * Check to see if we can play any of the given sources.
   *
   * @public
   * @static
   * @param {Array} sources
   * @returns {Boolean}
   */
  YouTube.canPlay = function (sources) {
    return getId(sources[0].path);
  };

  /**
   * Find id of YouTube video from given URL.
   *
   * @private
   * @param {String} url
   * @returns {String} YouTube video identifier
   */
  var getId = function (url) {
    var matches = url.match(/^https?:\/\/(www.youtube.com|youtu.be|y2u.be)\/(.+=)?(\S+)$/i);
    if (matches && matches[3]) {
      return matches[3];
    }
  };

  /**
   * Load the IFrame Player API asynchronously.
   */
  var loadAPI = function (loaded) {
    if (window.onYouTubeIframeAPIReady !== undefined) {
      // Someone else is loading, hook in
      var original = window.onYouTubeIframeAPIReady;
      window.onYouTubeIframeAPIReady = function (id) {
        loaded(id);
        original(id);
      };
    }
    else {
      // Load the API our self
      var tag = document.createElement('script');
      tag.src = "https://www.youtube.com/iframe_api";
      var firstScriptTag = document.getElementsByTagName('script')[0];
      firstScriptTag.parentNode.insertBefore(tag, firstScriptTag);
      window.onYouTubeIframeAPIReady = loaded;
    }
  };

  /** @constant {Object} */
  var LABELS = {
    highres: '2160p',
    hd1440: '1440p',
    hd1080: '1080p',
    hd720: '720p',
    large: '480p',
    medium: '360p',
    small: '240p',
    tiny: '144p',
    auto: 'Auto'
  };

  /** @private */
  var numInstances = 0;

  // Extract the current origin (used for security)
  var ORIGIN = window.location.href.match(/http[s]?:\/\/[^\/]+/)[0];

  return YouTube;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoYouTube);
;/** @namespace H5P */
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
    video.style.display = 'block';
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
      $container.append(video);
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
      var type = getType(sources[i]);
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
;/** @namespace H5P */
H5P.VideoFlash = (function ($) {

  /**
   * Flash video player for H5P.
   *
   * @class
   * @param {Array} sources Video files to use
   * @param {Object} options Settings for the player
   */
  function Flash(sources, options) {
    var self = this;

    // Player wrapper
    var $wrapper = $('<div/>', {
      'class': 'h5p-video-flash',
      css: {
        width: '100%',
        height: '100%'
      }
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

    // Sort sources into qualities
    //var qualities = getQualities(sources);
    var currentQuality;

    // Create player options
    var playerOptions = {
      buffering: true,
      clip: {
        url: sources[0].path, // getPreferredQuality(),
        autoPlay: options.autoplay,
        autoBuffering: true,
        scaling: 'fit',
        onSeek: function () {
          if (stateBeforeChangingQuality) {
            // ????
          }
        },
        onMetaData: function () {
          setTimeout(function () {
            if (stateBeforeChangingQuality !== undefined) {
              fp.seek(currentTimeBeforeChangingQuality);
              if (stateBeforeChangingQuality === H5P.Video.PLAYING) {
                // Resume play
                fp.play();
              }

              // Done changing quality
              stateBeforeChangingQuality = undefined;

              // Remove any errors
              if ($error.is(':visible')) {
                $error.remove();
              }
            }
            else {
              self.trigger('ready');
              self.trigger('loaded');
            }
          }, 0); // Run on next tick
        },
        onBegin: function () {
          self.trigger('stateChange', H5P.Video.PLAYING);
        },
        onResume: function () {
          self.trigger('stateChange', H5P.Video.PLAYING);
        },
        onPause: function () {
          self.trigger('stateChange', H5P.Video.PAUSED);
        },
        onFinish: function () {
          self.trigger('stateChange', H5P.Video.ENDED);
        },
        onError: function (code, message) {
          console.log('ERROR', code, message); // TODO
          self.trigger('error', message);
        }
      },
      plugins: {
        controls: null
      },
      play: null, // Disable overlay controls
      onPlaylistReplace: function () {
        that.playlistReplaced();
      }
    };

    if (options.controls) {
      playerOptions.plugins.controls = {};
      delete playerOptions.play;
    }

    var fp = flowplayer($wrapper[0], {
      src: "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf",
      wmode: "opaque"
    }, playerOptions);

    /**
     * Appends the video player to the DOM.
     *
     * @public
     * @param {jQuery} $container
     */
    self.appendTo = function ($container) {
      $wrapper.appendTo($container);
    };

    /**
     * Get list of available qualities. Not available until after play.
     *
     * @public
     * @returns {Array}
     */
    self.getQualities = function () {
      return;
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
      }

      // Keep track of current quality
      currentQuality = quality;
      self.trigger('qualityChange', currentQuality);

      // Display throbber
      self.trigger('stateChange', H5P.Video.BUFFERING);

      // Change source
      fp.setClip(qualities[quality].source.path);
      fp.startBuffering();
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

      fp.play();
    };

    /**
     * Pauses the video.
     *
     * @public
     */
    self.pause = function () {
      fp.pause();
    };

    /**
     * Seek video to given time.
     *
     * @public
     * @param {Number} time
     */
    self.seek = function (time) {
      fp.seek(time);
    };

    /**
     * Get elapsed time since video beginning.
     *
     * @public
     * @returns {Number}
     */
    self.getCurrentTime = function () {
      return fp.getTime();
    };

    /**
     * Get total video duration time.
     *
     * @public
     * @returns {Number}
     */
    self.getDuration = function () {
      return fp.getClip().metaData.duration;
    };

    /**
     * Get percentage of video that is buffered.
     *
     * @public
     * @returns {Number} Between 0 and 100
     */
    self.getBuffered = function () {
      return fp.getClip().buffer;
    };

    /**
     * Turn off video sound.
     *
     * @public
     */
    self.mute = function () {
      fp.mute();
    };

    /**
     * Turn on video sound.
     *
     * @public
     */
    self.unMute = function () {
      fp.unmute();
    };

    /**
     * Check if video sound is turned on or off.
     *
     * @public
     * @returns {Boolean}
     */
    self.isMuted = function () {
      return fp.muted;
    };

    /**
     * Returns the video sound level.
     *
     * @public
     * @returns {Number} Between 0 and 100.
     */
    self.getVolume = function () {
      return fp.volumeLevel * 100;
    };

    /**
     * Set video sound level.
     *
     * @public
     * @param {Number} volume Between 0 and 100.
     */
    self.setVolume = function (level) {
      fp.volume(level / 100);
    };

    // Handle resize events
    self.on('resize', function () {
      var $object = H5P.jQuery(fp.getParent()).children('object');
      var clip = fp.getClip();

      if (clip !== undefined) {
        $object.css('height', $object.width() * (clip.metaData.height / clip.metaData.width));
      }
    });
  }

  /**
   * Check to see if we can play any of the given sources.
   *
   * @public
   * @static
   * @param {Array} sources
   * @returns {Boolean}
   */
  Flash.canPlay = function (sources) {
    // Cycle through sources
    for (var i = 0; i < sources.length; i++) {
      if (sources[i].mime === 'video/mp4' || /\.mp4$/.test(sources[i].mime)) {
        return true; // We only play mp4
      }
    }
  };

  return Flash;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoFlash);
;/** @namespace H5P */
H5P.Video = (function ($, ContentCopyrights, MediaCopyright, handlers) {

  /**
   * The ultimate H5P video player!
   *
   * @class
   * @param {Object} parameters Options for this library.
   * @param {Object} parameters.visuals Visual options
   * @param {Object} parameters.playback Playback options
   * @param {Object} parameters.a11y Accessibility options
   * @param {Boolean} [parameters.startAt] Start time of video
   * @param {Number} id Content identifier
   */
  function Video(parameters, id) {
    var self = this;

    // Initialize event inheritance
    H5P.EventDispatcher.call(self);

    // Default language localization
    parameters = $.extend(true, parameters, {
      l10n: {
        name: 'Video',
        loading: 'Video player loading...',
        noPlayers: 'Found no video players that supports the given video format.',
        noSources: 'Video is missing sources.',
        aborted: 'Media playback has been aborted.',
        networkFailure: 'Network failure.',
        cannotDecode: 'Unable to decode media.',
        formatNotSupported: 'Video format not supported.',
        mediaEncrypted: 'Media encrypted.',
        unknownError: 'Unknown error.',
        invalidYtId: 'Invalid YouTube ID.',
        unknownYtId: 'Unable to find video with the given YouTube ID.',
        restrictedYt: 'The owner of this video does not allow it to be embedded.'
      }
    });

    parameters.a11y = parameters.a11y || [];
    parameters.playback = parameters.playback || {};
    parameters.visuals = parameters.visuals || {};

    /** @private */
    var sources = [];
    if (parameters.sources) {
      for (var i = 0; i < parameters.sources.length; i++) {
        // Clone to avoid changing of parameters.
        var source = $.extend(true, {}, parameters.sources[i]);

        // Create working URL without html entities.
        source.path = H5P.getPath($cleaner.html(source.path).text(), id);
         //h5pcustomize
	      if(source.path != null && source.path.indexOf("^^^")>0)
    	  {
	  		 var pathArr = source.path.split("^^^");
	  		 var path1withfileextension = pathArr[0];
	  		 var path1withfileextensionarr = path1withfileextension.split("/");
	  		 var actualUrl = "";
	  		 for(var j = 0; j < path1withfileextensionarr.length -1; j++)
	  		 {
	  		 	if(actualUrl == "")
	  		 		actualUrl = path1withfileextensionarr[j];
	  		 	else
	  		 		actualUrl += "/"+path1withfileextensionarr[j];
	  		 	
	  		 }
	  		 
	  		 source.path = actualUrl + "/"+pathArr[1];
	  		}
	  	
        sources.push(source);
      }
    }

    /** @private */
    var tracks = [];
    
	
    parameters.a11y.forEach(function (track) {
      // Clone to avoid changing of parameters.
      var clone = $.extend(true, {}, track);

      // Create working URL without html entities
      if (clone.track && clone.track.path) {
        clone.track.path = H5P.getPath($cleaner.html(clone.track.path).text(), id);
        tracks.push(clone);
      }
    });

    /**
     * Attaches the video handler to the given container.
     * Inserts text if no handler is found.
     *
     * @public
     * @param {jQuery} $container
     */
    self.attach = function ($container) {
      $container.addClass('h5p-video').html('');

      if (self.appendTo !== undefined) {
        self.appendTo($container);
      }
      else {
        if (sources.length) {
          $container.text(parameters.l10n.noPlayers);
        }
        else {
          $container.text(parameters.l10n.noSources);
        }
      }
    };

    /**
     * Gather copyright information for the current video.
     *
     * @public
     * @returns {ContentCopyrights}
     */
    self.getCopyrights = function () {
      if (!sources[0] || !sources[0].copyright) {
        return;
      }

      // Use copyright information from H5P media field
      var info = new ContentCopyrights();
      info.addMedia(new MediaCopyright(sources[0].copyright));

      return info;
    };

    // Resize the video when we know its aspect ratio
    self.on('loaded', function () {
      self.trigger('resize');
    });

    // Find player for video sources
    if (sources.length) {
      for (var i = 0; i < handlers.length; i++) {
        var handler = handlers[i];
        if (handler.canPlay !== undefined && handler.canPlay(sources)) {
          handler.call(self, sources, {
            controls: parameters.visuals.controls,
            autoplay: parameters.playback.autoplay,
            loop: parameters.playback.loop,
            fit: parameters.visuals.fit,
            poster: parameters.visuals.poster === undefined ? undefined : H5P.getPath(parameters.visuals.poster.path, id),
            startAt: parameters.startAt || 0,
            tracks: tracks
          }, parameters.l10n);
          return;
        }
      }
    }
  }

  // Extends the event dispatcher
  Video.prototype = Object.create(H5P.EventDispatcher.prototype);
  Video.prototype.constructor = Video;

  // Player states
  /** @constant {Number} */
  Video.ENDED = 0;
  /** @constant {Number} */
  Video.PLAYING = 1;
  /** @constant {Number} */
  Video.PAUSED = 2;
  /** @constant {Number} */
  Video.BUFFERING = 3;

  // Used to convert between html and text, since URLs have html entities.
  var $cleaner = H5P.jQuery('<div/>');

  return Video;
})(H5P.jQuery, H5P.ContentCopyrights, H5P.MediaCopyright, H5P.videoHandlers || []);
;var H5P = H5P || {};

/**
 * H5P audio module
 *
 * @external {jQuery} $ H5P.jQuery
 */
H5P.Audio = (function ($) {
  /**
  * @param {Object} params Options for this library.
  * @param {Number} id Content identifier
  * @returns {undefined}
  */
  function C(params, id) {
    H5P.EventDispatcher.call(this);
    this.contentId = id;
    this.params = params;

    this.params = $.extend({}, {
      playerMode: 'minimalistic',
      fitToWrapper: false,
      controls: true,
      autoplay: false
    }, params);

    // Use new copyright information if available. Fallback to old.
    if (params.files !== undefined
      && params.files[0] !== undefined
      && params.files[0].copyright !== undefined) {

      this.copyright = params.files[0].copyright;
    }
    else if (params.copyright !== undefined) {
      this.copyright = params.copyright;
    }
    this.on('resize', this.resize, this);
  }

  C.prototype = Object.create(H5P.EventDispatcher.prototype);
  C.prototype.constructor = C;

  /**
   * Adds a minimalistic audio player with only "play" and "pause" functionality.
   *
   * @param {jQuery} $container Container for the player.
   */
  C.prototype.addMinimalAudioPlayer = function ($container) {
    var INNER_CONTAINER = 'h5p-audio-inner';
    var AUDIO_BUTTON = 'h5p-audio-minimal-button';
    var PLAY_BUTTON = 'h5p-audio-minimal-play';
    var PAUSE_BUTTON = 'h5p-audio-minimal-pause';

    var self = this;
    this.$container = $container;

    self.$inner = $('<div/>', {
      'class': INNER_CONTAINER
    }).appendTo($container);

    var audioButton = $('<button/>', {
      'class': AUDIO_BUTTON+" "+PLAY_BUTTON
    }).appendTo(self.$inner)
      .click( function () {
        if (self.audio.paused) {
          self.play();
        } else {
          self.pause();
        }
      });

    //Fit to wrapper
    if (this.params.fitToWrapper) {
      audioButton.css({
        'width': '100%',
        'height': '100%'
      });
    }

    // cpAutoplay is passed from coursepresentation
    if (this.params.autoplay) {
      self.play();
    }

    //Event listeners that change the look of the player depending on events.
    self.audio.addEventListener('ended', function () {
      audioButton.removeClass(PAUSE_BUTTON).addClass(PLAY_BUTTON);
    });

    self.audio.addEventListener('play', function () {
      audioButton.removeClass(PLAY_BUTTON).addClass(PAUSE_BUTTON);
    });

    self.audio.addEventListener('pause', function () {
      audioButton.removeClass(PAUSE_BUTTON).addClass(PLAY_BUTTON);
    });

    this.$audioButton = audioButton;
    //Scale icon to container
    self.resize();
  };

  /**
   * Resizes the audio player icon when the wrapper is resized.
   */
  C.prototype.resize = function () {
    // Find the smallest value of height and width, and use it to choose the font size.
    if (this.params.fitToWrapper && this.$container && this.$container.width()) {
      var w = this.$container.width();
      var h = this.$container.height();
      if (w < h) {
        this.$audioButton.css({'font-size': w / 2 + 'px'});
      }
      else {
        this.$audioButton.css({'font-size': h / 2 + 'px'});
      }
    }
  };


  return C;
})(H5P.jQuery);

/**
 * Wipe out the content of the wrapper and put our HTML in it.
 *
 * @param {jQuery} $wrapper Our poor container.
 */
H5P.Audio.prototype.attach = function ($wrapper) {
  $wrapper.addClass('h5p-audio-wrapper');

  // Check if browser supports audio.
  var audio = document.createElement('audio');
  if (audio.canPlayType === undefined) {
    // Try flash
    this.attachFlash($wrapper);
    return;
  }

  // Add supported source files.
  if (this.params.files !== undefined && this.params.files instanceof Object) {
    for (var i = 0; i < this.params.files.length; i++) {
      var file = this.params.files[i];
      //h5pcustomize
      if(file.path != null && file.path.indexOf("^^^")>0)
      {
	  	var pathArr = file.path.split("^^^");
	  	file.path = pathArr[1];
	  }
      if (audio.canPlayType(file.mime)) {
        var source = document.createElement('source');
        source.src = H5P.getPath(file.path, this.contentId);
        source.type = file.mime;
        audio.appendChild(source);
      }
    }
  }

  if (!audio.children.length) {
    // Try flash
    this.attachFlash($wrapper);
    return;
  }

  if (this.endedCallback !== undefined) {
    audio.addEventListener('ended', this.endedCallback, false);
  }

  audio.className = 'h5p-audio';
  audio.controls = this.params.controls === undefined ? true : this.params.controls;
  audio.preload = 'auto';
  audio.style.display = 'block';

  if (this.params.fitToWrapper === undefined || this.params.fitToWrapper) {
    audio.style.width = '100%';
    audio.style.height = '100%';
  }

  this.audio = audio;

  if (this.params.playerMode === 'minimalistic') {
    audio.controls = false;
    this.addMinimalAudioPlayer($wrapper);
  }
  else {
    audio.autoplay = this.params.autoplay === undefined ? false : this.params.autoplay;
    $wrapper.html(audio);
  }
};

/**
 * Attaches a flash audio player to the wrapper.
 *
 * @param {jQuery} $wrapper Our dear container.
 */
H5P.Audio.prototype.attachFlash = function ($wrapper) {
  if (this.params.files !== undefined && this.params.files instanceof Object) {
    for (var i = 0; i < this.params.files.length; i++) {
      var file = this.params.files[i];
      if (file.mime === 'audio/mpeg' || file.mime === 'audio/mp3') {
        var audioSource = H5P.getPath(file.path, this.contentId);
        break;
      }
    }
  }

  if (audioSource === undefined) {
    $wrapper.text('No supported audio files found.');
    if (this.endedCallback !== undefined) {
      this.endedCallback();
    }
    return;
  }

  var options = {
    buffering: true,
    clip: {
      url: window.location.protocol + '//' + window.location.host + audioSource,
      autoPlay: this.params.autoplay === undefined ? false : this.params.autoplay,
      scaling: 'fit'
    },
    plugins: {
      controls: null
    }
  };

  if (this.params.controls === undefined || this.params.controls) {
    options.plugins.controls = {
      fullscreen: false,
      autoHide: false
    };
  }

  if (this.endedCallback !== undefined) {
    options.clip.onFinish = this.endedCallback;
    options.clip.onError = this.endedCallback;
  }

  this.flowplayer = flowplayer($wrapper[0], {
    src: "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf",
    wmode: "opaque"
  }, options);
};

/**
 * Stop the audio. TODO: Rename to pause?
 *
 * @returns {undefined}
 */
H5P.Audio.prototype.stop = function () {
  if (this.flowplayer !== undefined) {
    this.flowplayer.stop().close().unload();
  }
  if (this.audio !== undefined) {
    this.audio.pause();
  }
};

/**
 * Play
 */
H5P.Audio.prototype.play = function () {
  if (this.flowplayer !== undefined) {
    this.flowplayer.play();
  }
  if (this.audio !== undefined) {
    this.audio.play();
  }
};

/**
 * @public
 * Pauses the audio.
 */
H5P.Audio.prototype.pause = function () {
  if (this.audio !== undefined) {
    this.audio.pause();
  }
};

/**
 * Gather copyright information for the current content.
 *
 * @returns {H5P.ContentCopyrights} Copyright information
 */
H5P.Audio.prototype.getCopyrights = function () {
  if (this.copyright === undefined) {
    return;
  }

  var info = new H5P.ContentCopyrights();
  info.addMedia(new H5P.MediaCopyright(this.copyright));

  return info;
};
;/*	SWFObject v2.2 <http://code.google.com/p/swfobject/> 
	is released under the MIT License <http://www.opensource.org/licenses/mit-license.php> 
*/
if(swfobject === undefined) {
var swfobject=function(){var D="undefined",r="object",S="Shockwave Flash",W="ShockwaveFlash.ShockwaveFlash",q="application/x-shockwave-flash",R="SWFObjectExprInst",x="onreadystatechange",O=window,j=document,t=navigator,T=false,U=[h],o=[],N=[],I=[],l,Q,E,B,J=false,a=false,n,G,m=true,M=function(){var aa=typeof j.getElementById!=D&&typeof j.getElementsByTagName!=D&&typeof j.createElement!=D,ah=t.userAgent.toLowerCase(),Y=t.platform.toLowerCase(),ae=Y?/win/.test(Y):/win/.test(ah),ac=Y?/mac/.test(Y):/mac/.test(ah),af=/webkit/.test(ah)?parseFloat(ah.replace(/^.*webkit\/(\d+(\.\d+)?).*$/,"$1")):false,X=!+"\v1",ag=[0,0,0],ab=null;if(typeof t.plugins!=D&&typeof t.plugins[S]==r){ab=t.plugins[S].description;if(ab&&!(typeof t.mimeTypes!=D&&t.mimeTypes[q]&&!t.mimeTypes[q].enabledPlugin)){T=true;X=false;ab=ab.replace(/^.*\s+(\S+\s+\S+$)/,"$1");ag[0]=parseInt(ab.replace(/^(.*)\..*$/,"$1"),10);ag[1]=parseInt(ab.replace(/^.*\.(.*)\s.*$/,"$1"),10);ag[2]=/[a-zA-Z]/.test(ab)?parseInt(ab.replace(/^.*[a-zA-Z]+(.*)$/,"$1"),10):0}}else{if(typeof O.ActiveXObject!=D){try{var ad=new ActiveXObject(W);if(ad){ab=ad.GetVariable("$version");if(ab){X=true;ab=ab.split(" ")[1].split(",");ag=[parseInt(ab[0],10),parseInt(ab[1],10),parseInt(ab[2],10)]}}}catch(Z){}}}return{w3:aa,pv:ag,wk:af,ie:X,win:ae,mac:ac}}(),k=function(){if(!M.w3){return}if((typeof j.readyState!=D&&j.readyState=="complete")||(typeof j.readyState==D&&(j.getElementsByTagName("body")[0]||j.body))){f()}if(!J){if(typeof j.addEventListener!=D){j.addEventListener("DOMContentLoaded",f,false)}if(M.ie&&M.win){j.attachEvent(x,function(){if(j.readyState=="complete"){j.detachEvent(x,arguments.callee);f()}});if(O==top){(function(){if(J){return}try{j.documentElement.doScroll("left")}catch(X){setTimeout(arguments.callee,0);return}f()})()}}if(M.wk){(function(){if(J){return}if(!/loaded|complete/.test(j.readyState)){setTimeout(arguments.callee,0);return}f()})()}s(f)}}();function f(){if(J){return}try{var Z=j.getElementsByTagName("body")[0].appendChild(C("span"));Z.parentNode.removeChild(Z)}catch(aa){return}J=true;var X=U.length;for(var Y=0;Y<X;Y++){U[Y]()}}function K(X){if(J){X()}else{U[U.length]=X}}function s(Y){if(typeof O.addEventListener!=D){O.addEventListener("load",Y,false)}else{if(typeof j.addEventListener!=D){j.addEventListener("load",Y,false)}else{if(typeof O.attachEvent!=D){i(O,"onload",Y)}else{if(typeof O.onload=="function"){var X=O.onload;O.onload=function(){X();Y()}}else{O.onload=Y}}}}}function h(){if(T){V()}else{H()}}function V(){var X=j.getElementsByTagName("body")[0];var aa=C(r);aa.setAttribute("type",q);var Z=X.appendChild(aa);if(Z){var Y=0;(function(){if(typeof Z.GetVariable!=D){var ab=Z.GetVariable("$version");if(ab){ab=ab.split(" ")[1].split(",");M.pv=[parseInt(ab[0],10),parseInt(ab[1],10),parseInt(ab[2],10)]}}else{if(Y<10){Y++;setTimeout(arguments.callee,10);return}}X.removeChild(aa);Z=null;H()})()}else{H()}}function H(){var ag=o.length;if(ag>0){for(var af=0;af<ag;af++){var Y=o[af].id;var ab=o[af].callbackFn;var aa={success:false,id:Y};if(M.pv[0]>0){var ae=c(Y);if(ae){if(F(o[af].swfVersion)&&!(M.wk&&M.wk<312)){w(Y,true);if(ab){aa.success=true;aa.ref=z(Y);ab(aa)}}else{if(o[af].expressInstall&&A()){var ai={};ai.data=o[af].expressInstall;ai.width=ae.getAttribute("width")||"0";ai.height=ae.getAttribute("height")||"0";if(ae.getAttribute("class")){ai.styleclass=ae.getAttribute("class")}if(ae.getAttribute("align")){ai.align=ae.getAttribute("align")}var ah={};var X=ae.getElementsByTagName("param");var ac=X.length;for(var ad=0;ad<ac;ad++){if(X[ad].getAttribute("name").toLowerCase()!="movie"){ah[X[ad].getAttribute("name")]=X[ad].getAttribute("value")}}P(ai,ah,Y,ab)}else{p(ae);if(ab){ab(aa)}}}}}else{w(Y,true);if(ab){var Z=z(Y);if(Z&&typeof Z.SetVariable!=D){aa.success=true;aa.ref=Z}ab(aa)}}}}}function z(aa){var X=null;var Y=c(aa);if(Y&&Y.nodeName=="OBJECT"){if(typeof Y.SetVariable!=D){X=Y}else{var Z=Y.getElementsByTagName(r)[0];if(Z){X=Z}}}return X}function A(){return !a&&F("6.0.65")&&(M.win||M.mac)&&!(M.wk&&M.wk<312)}function P(aa,ab,X,Z){a=true;E=Z||null;B={success:false,id:X};var ae=c(X);if(ae){if(ae.nodeName=="OBJECT"){l=g(ae);Q=null}else{l=ae;Q=X}aa.id=R;if(typeof aa.width==D||(!/%$/.test(aa.width)&&parseInt(aa.width,10)<310)){aa.width="310"}if(typeof aa.height==D||(!/%$/.test(aa.height)&&parseInt(aa.height,10)<137)){aa.height="137"}j.title=j.title.slice(0,47)+" - Flash Player Installation";var ad=M.ie&&M.win?"ActiveX":"PlugIn",ac="MMredirectURL="+O.location.toString().replace(/&/g,"%26")+"&MMplayerType="+ad+"&MMdoctitle="+j.title;if(typeof ab.flashvars!=D){ab.flashvars+="&"+ac}else{ab.flashvars=ac}if(M.ie&&M.win&&ae.readyState!=4){var Y=C("div");X+="SWFObjectNew";Y.setAttribute("id",X);ae.parentNode.insertBefore(Y,ae);ae.style.display="none";(function(){if(ae.readyState==4){ae.parentNode.removeChild(ae)}else{setTimeout(arguments.callee,10)}})()}u(aa,ab,X)}}function p(Y){if(M.ie&&M.win&&Y.readyState!=4){var X=C("div");Y.parentNode.insertBefore(X,Y);X.parentNode.replaceChild(g(Y),X);Y.style.display="none";(function(){if(Y.readyState==4){Y.parentNode.removeChild(Y)}else{setTimeout(arguments.callee,10)}})()}else{Y.parentNode.replaceChild(g(Y),Y)}}function g(ab){var aa=C("div");if(M.win&&M.ie){aa.innerHTML=ab.innerHTML}else{var Y=ab.getElementsByTagName(r)[0];if(Y){var ad=Y.childNodes;if(ad){var X=ad.length;for(var Z=0;Z<X;Z++){if(!(ad[Z].nodeType==1&&ad[Z].nodeName=="PARAM")&&!(ad[Z].nodeType==8)){aa.appendChild(ad[Z].cloneNode(true))}}}}}return aa}function u(ai,ag,Y){var X,aa=c(Y);if(M.wk&&M.wk<312){return X}if(aa){if(typeof ai.id==D){ai.id=Y}if(M.ie&&M.win){var ah="";for(var ae in ai){if(ai[ae]!=Object.prototype[ae]){if(ae.toLowerCase()=="data"){ag.movie=ai[ae]}else{if(ae.toLowerCase()=="styleclass"){ah+=' class="'+ai[ae]+'"'}else{if(ae.toLowerCase()!="classid"){ah+=" "+ae+'="'+ai[ae]+'"'}}}}}var af="";for(var ad in ag){if(ag[ad]!=Object.prototype[ad]){af+='<param name="'+ad+'" value="'+ag[ad]+'" />'}}aa.outerHTML='<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"'+ah+">"+af+"</object>";N[N.length]=ai.id;X=c(ai.id)}else{var Z=C(r);Z.setAttribute("type",q);for(var ac in ai){if(ai[ac]!=Object.prototype[ac]){if(ac.toLowerCase()=="styleclass"){Z.setAttribute("class",ai[ac])}else{if(ac.toLowerCase()!="classid"){Z.setAttribute(ac,ai[ac])}}}}for(var ab in ag){if(ag[ab]!=Object.prototype[ab]&&ab.toLowerCase()!="movie"){e(Z,ab,ag[ab])}}aa.parentNode.replaceChild(Z,aa);X=Z}}return X}function e(Z,X,Y){var aa=C("param");aa.setAttribute("name",X);aa.setAttribute("value",Y);Z.appendChild(aa)}function y(Y){var X=c(Y);if(X&&X.nodeName=="OBJECT"){if(M.ie&&M.win){X.style.display="none";(function(){if(X.readyState==4){b(Y)}else{setTimeout(arguments.callee,10)}})()}else{X.parentNode.removeChild(X)}}}function b(Z){var Y=c(Z);if(Y){for(var X in Y){if(typeof Y[X]=="function"){Y[X]=null}}Y.parentNode.removeChild(Y)}}function c(Z){var X=null;try{X=j.getElementById(Z)}catch(Y){}return X}function C(X){return j.createElement(X)}function i(Z,X,Y){Z.attachEvent(X,Y);I[I.length]=[Z,X,Y]}function F(Z){var Y=M.pv,X=Z.split(".");X[0]=parseInt(X[0],10);X[1]=parseInt(X[1],10)||0;X[2]=parseInt(X[2],10)||0;return(Y[0]>X[0]||(Y[0]==X[0]&&Y[1]>X[1])||(Y[0]==X[0]&&Y[1]==X[1]&&Y[2]>=X[2]))?true:false}function v(ac,Y,ad,ab){if(M.ie&&M.mac){return}var aa=j.getElementsByTagName("head")[0];if(!aa){return}var X=(ad&&typeof ad=="string")?ad:"screen";if(ab){n=null;G=null}if(!n||G!=X){var Z=C("style");Z.setAttribute("type","text/css");Z.setAttribute("media",X);n=aa.appendChild(Z);if(M.ie&&M.win&&typeof j.styleSheets!=D&&j.styleSheets.length>0){n=j.styleSheets[j.styleSheets.length-1]}G=X}if(M.ie&&M.win){if(n&&typeof n.addRule==r){n.addRule(ac,Y)}}else{if(n&&typeof j.createTextNode!=D){n.appendChild(j.createTextNode(ac+" {"+Y+"}"))}}}function w(Z,X){if(!m){return}var Y=X?"visible":"hidden";if(J&&c(Z)){c(Z).style.visibility=Y}else{v("#"+Z,"visibility:"+Y)}}function L(Y){var Z=/[\\\"<>\.;]/;var X=Z.exec(Y)!=null;return X&&typeof encodeURIComponent!=D?encodeURIComponent(Y):Y}var d=function(){if(M.ie&&M.win){window.attachEvent("onunload",function(){var ac=I.length;for(var ab=0;ab<ac;ab++){I[ab][0].detachEvent(I[ab][1],I[ab][2])}var Z=N.length;for(var aa=0;aa<Z;aa++){y(N[aa])}for(var Y in M){M[Y]=null}M=null;for(var X in swfobject){swfobject[X]=null}swfobject=null})}}();return{registerObject:function(ab,X,aa,Z){if(M.w3&&ab&&X){var Y={};Y.id=ab;Y.swfVersion=X;Y.expressInstall=aa;Y.callbackFn=Z;o[o.length]=Y;w(ab,false)}else{if(Z){Z({success:false,id:ab})}}},getObjectById:function(X){if(M.w3){return z(X)}},embedSWF:function(ab,ah,ae,ag,Y,aa,Z,ad,af,ac){var X={success:false,id:ah};if(M.w3&&!(M.wk&&M.wk<312)&&ab&&ah&&ae&&ag&&Y){w(ah,false);K(function(){ae+="";ag+="";var aj={};if(af&&typeof af===r){for(var al in af){aj[al]=af[al]}}aj.data=ab;aj.width=ae;aj.height=ag;var am={};if(ad&&typeof ad===r){for(var ak in ad){am[ak]=ad[ak]}}if(Z&&typeof Z===r){for(var ai in Z){if(typeof am.flashvars!=D){am.flashvars+="&"+ai+"="+Z[ai]}else{am.flashvars=ai+"="+Z[ai]}}}if(F(Y)){var an=u(aj,am,ah);if(aj.id==ah){w(ah,true)}X.success=true;X.ref=an}else{if(aa&&A()){aj.data=aa;P(aj,am,ah,ac);return}else{w(ah,true)}}if(ac){ac(X)}})}else{if(ac){ac(X)}}},switchOffAutoHideShow:function(){m=false},ua:M,getFlashPlayerVersion:function(){return{major:M.pv[0],minor:M.pv[1],release:M.pv[2]}},hasFlashPlayerVersion:F,createSWF:function(Z,Y,X){if(M.w3){return u(Z,Y,X)}else{return undefined}},showExpressInstall:function(Z,aa,X,Y){if(M.w3&&A()){P(Z,aa,X,Y)}},removeSWF:function(X){if(M.w3){y(X)}},createCSS:function(aa,Z,Y,X){if(M.w3){v(aa,Z,Y,X)}},addDomLoadEvent:K,addLoadEvent:s,getQueryParamValue:function(aa){var Z=j.location.search||j.location.hash;if(Z){if(/\?/.test(Z)){Z=Z.split("?")[1]}if(aa==null){return L(Z)}var Y=Z.split("&");for(var X=0;X<Y.length;X++){if(Y[X].substring(0,Y[X].indexOf("="))==aa){return L(Y[X].substring((Y[X].indexOf("=")+1)))}}}return""},expressInstallCallback:function(){if(a){var X=c(R);if(X&&l){X.parentNode.replaceChild(l,X);if(Q){w(Q,true);if(M.ie&&M.win){l.style.display="block"}}if(E){E(B)}}a=false}}}}();
};/*!
* @license SoundJS
* Visit http://createjs.com/ for documentation, updates and examples.
*
* Copyright (c) 2011-2013 gskinner.com, inc.
*
* Distributed under the terms of the MIT license.
* http://www.opensource.org/licenses/mit-license.html
*
* This notice shall be included in all copies or substantial portions of the Software.
*/

/**!
 * SoundJS FlashAudioPlugin also includes swfobject (http://code.google.com/p/swfobject/)
 */

var old = this.createjs;

this.createjs=this.createjs||{},function(){var a=createjs.SoundJS=createjs.SoundJS||{};a.version="0.6.0",a.buildDate="Thu, 11 Dec 2014 23:32:09 GMT"}(),this.createjs=this.createjs||{},createjs.extend=function(a,b){"use strict";function c(){this.constructor=a}return c.prototype=b.prototype,a.prototype=new c},this.createjs=this.createjs||{},createjs.promote=function(a,b){"use strict";var c=a.prototype,d=Object.getPrototypeOf&&Object.getPrototypeOf(c)||c.__proto__;if(d){c[(b+="_")+"constructor"]=d.constructor;for(var e in d)c.hasOwnProperty(e)&&"function"==typeof d[e]&&(c[b+e]=d[e])}return a},this.createjs=this.createjs||{},createjs.indexOf=function(a,b){"use strict";for(var c=0,d=a.length;d>c;c++)if(b===a[c])return c;return-1},this.createjs=this.createjs||{},function(){"use strict";createjs.proxy=function(a,b){var c=Array.prototype.slice.call(arguments,2);return function(){return a.apply(b,Array.prototype.slice.call(arguments,0).concat(c))}}}(),this.createjs=this.createjs||{},function(){"use strict";var a=Object.defineProperty?!0:!1,b={};try{Object.defineProperty(b,"bar",{get:function(){return this._bar},set:function(a){this._bar=a}})}catch(c){a=!1}createjs.definePropertySupported=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(){throw"BrowserDetect cannot be instantiated"}var b=a.agent=window.navigator.userAgent;a.isWindowPhone=b.indexOf("IEMobile")>-1||b.indexOf("Windows Phone")>-1,a.isFirefox=b.indexOf("Firefox")>-1,a.isOpera=null!=window.opera,a.isChrome=b.indexOf("Chrome")>-1,a.isIOS=(b.indexOf("iPod")>-1||b.indexOf("iPhone")>-1||b.indexOf("iPad")>-1)&&!a.isWindowPhone,a.isAndroid=b.indexOf("Android")>-1&&!a.isWindowPhone,a.isBlackberry=b.indexOf("Blackberry")>-1,createjs.BrowserDetect=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this._listeners=null,this._captureListeners=null}var b=a.prototype;a.initialize=function(a){a.addEventListener=b.addEventListener,a.on=b.on,a.removeEventListener=a.off=b.removeEventListener,a.removeAllEventListeners=b.removeAllEventListeners,a.hasEventListener=b.hasEventListener,a.dispatchEvent=b.dispatchEvent,a._dispatchEvent=b._dispatchEvent,a.willTrigger=b.willTrigger},b.addEventListener=function(a,b,c){var d;d=c?this._captureListeners=this._captureListeners||{}:this._listeners=this._listeners||{};var e=d[a];return e&&this.removeEventListener(a,b,c),e=d[a],e?e.push(b):d[a]=[b],b},b.on=function(a,b,c,d,e,f){return b.handleEvent&&(c=c||b,b=b.handleEvent),c=c||this,this.addEventListener(a,function(a){b.call(c,a,e),d&&a.remove()},f)},b.removeEventListener=function(a,b,c){var d=c?this._captureListeners:this._listeners;if(d){var e=d[a];if(e)for(var f=0,g=e.length;g>f;f++)if(e[f]==b){1==g?delete d[a]:e.splice(f,1);break}}},b.off=b.removeEventListener,b.removeAllEventListeners=function(a){a?(this._listeners&&delete this._listeners[a],this._captureListeners&&delete this._captureListeners[a]):this._listeners=this._captureListeners=null},b.dispatchEvent=function(a){if("string"==typeof a){var b=this._listeners;if(!b||!b[a])return!1;a=new createjs.Event(a)}else a.target&&a.clone&&(a=a.clone());try{a.target=this}catch(c){}if(a.bubbles&&this.parent){for(var d=this,e=[d];d.parent;)e.push(d=d.parent);var f,g=e.length;for(f=g-1;f>=0&&!a.propagationStopped;f--)e[f]._dispatchEvent(a,1+(0==f));for(f=1;g>f&&!a.propagationStopped;f++)e[f]._dispatchEvent(a,3)}else this._dispatchEvent(a,2);return a.defaultPrevented},b.hasEventListener=function(a){var b=this._listeners,c=this._captureListeners;return!!(b&&b[a]||c&&c[a])},b.willTrigger=function(a){for(var b=this;b;){if(b.hasEventListener(a))return!0;b=b.parent}return!1},b.toString=function(){return"[EventDispatcher]"},b._dispatchEvent=function(a,b){var c,d=1==b?this._captureListeners:this._listeners;if(a&&d){var e=d[a.type];if(!e||!(c=e.length))return;try{a.currentTarget=this}catch(f){}try{a.eventPhase=b}catch(f){}a.removed=!1,e=e.slice();for(var g=0;c>g&&!a.immediatePropagationStopped;g++){var h=e[g];h.handleEvent?h.handleEvent(a):h(a),a.removed&&(this.off(a.type,h,1==b),a.removed=!1)}}},createjs.EventDispatcher=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.type=a,this.target=null,this.currentTarget=null,this.eventPhase=0,this.bubbles=!!b,this.cancelable=!!c,this.timeStamp=(new Date).getTime(),this.defaultPrevented=!1,this.propagationStopped=!1,this.immediatePropagationStopped=!1,this.removed=!1}var b=a.prototype;b.preventDefault=function(){this.defaultPrevented=this.cancelable&&!0},b.stopPropagation=function(){this.propagationStopped=!0},b.stopImmediatePropagation=function(){this.immediatePropagationStopped=this.propagationStopped=!0},b.remove=function(){this.removed=!0},b.clone=function(){return new a(this.type,this.bubbles,this.cancelable)},b.set=function(a){for(var b in a)this[b]=a[b];return this},b.toString=function(){return"[Event (type="+this.type+")]"},createjs.Event=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.Event_constructor("error"),this.title=a,this.message=b,this.data=c}var b=createjs.extend(a,createjs.Event);b.clone=function(){return new createjs.ErrorEvent(this.title,this.message,this.data)},createjs.ErrorEvent=createjs.promote(a,"Event")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b){this.Event_constructor("progress"),this.loaded=a,this.total=null==b?1:b,this.progress=0==b?0:this.loaded/this.total}var b=createjs.extend(a,createjs.Event);b.clone=function(){return new createjs.ProgressEvent(this.loaded,this.total)},createjs.ProgressEvent=createjs.promote(a,"Event")}(window),this.createjs=this.createjs||{},function(){"use strict";function a(){this.src=null,this.type=null,this.id=null,this.maintainOrder=!1,this.callback=null,this.data=null,this.method=createjs.LoadItem.GET,this.values=null,this.headers=null,this.withCredentials=!1,this.mimeType=null,this.crossOrigin="Anonymous",this.loadTimeout=8e3}var b=a.prototype={},c=a;c.create=function(b){if("string"==typeof b){var d=new a;return d.src=b,d}if(b instanceof c)return b;if(b instanceof Object)return b;throw new Error("Type not recognized.")},b.set=function(a){for(var b in a)this[b]=a[b];return this},createjs.LoadItem=c}(),function(){var a={};a.ABSOLUTE_PATT=/^(?:\w+:)?\/{2}/i,a.RELATIVE_PATT=/^[./]*?\//i,a.EXTENSION_PATT=/\/?[^/]+\.(\w{1,5})$/i,a.parseURI=function(b){var c={absolute:!1,relative:!1};if(null==b)return c;var d=b.indexOf("?");d>-1&&(b=b.substr(0,d));var e;return a.ABSOLUTE_PATT.test(b)?c.absolute=!0:a.RELATIVE_PATT.test(b)&&(c.relative=!0),(e=b.match(a.EXTENSION_PATT))&&(c.extension=e[1].toLowerCase()),c},a.formatQueryString=function(a,b){if(null==a)throw new Error("You must specify data.");var c=[];for(var d in a)c.push(d+"="+escape(a[d]));return b&&(c=c.concat(b)),c.join("&")},a.buildPath=function(a,b){if(null==b)return a;var c=[],d=a.indexOf("?");if(-1!=d){var e=a.slice(d+1);c=c.concat(e.split("&"))}return-1!=d?a.slice(0,d)+"?"+this._formatQueryString(b,c):a+"?"+this._formatQueryString(b,c)},a.isCrossDomain=function(a){var b=document.createElement("a");b.href=a.src;var c=document.createElement("a");c.href=location.href;var d=""!=b.hostname&&(b.port!=c.port||b.protocol!=c.protocol||b.hostname!=c.hostname);return d},a.isLocal=function(a){var b=document.createElement("a");return b.href=a.src,""==b.hostname&&"file:"==b.protocol},a.isBinary=function(a){switch(a){case createjs.AbstractLoader.IMAGE:case createjs.AbstractLoader.BINARY:return!0;default:return!1}},a.isImageTag=function(a){return a instanceof HTMLImageElement},a.isAudioTag=function(a){return window.HTMLAudioElement?a instanceof HTMLAudioElement:!1},a.isVideoTag=function(a){return window.HTMLVideoElement?a instanceof HTMLVideoElement:void 0},a.isText=function(a){switch(a){case createjs.AbstractLoader.TEXT:case createjs.AbstractLoader.JSON:case createjs.AbstractLoader.MANIFEST:case createjs.AbstractLoader.XML:case createjs.AbstractLoader.CSS:case createjs.AbstractLoader.SVG:case createjs.AbstractLoader.JAVASCRIPT:return!0;default:return!1}},a.getTypeByExtension=function(a){if(null==a)return createjs.AbstractLoader.TEXT;switch(a.toLowerCase()){case"jpeg":case"jpg":case"gif":case"png":case"webp":case"bmp":return createjs.AbstractLoader.IMAGE;case"ogg":case"mp3":case"webm":return createjs.AbstractLoader.SOUND;case"mp4":case"webm":case"ts":return createjs.AbstractLoader.VIDEO;case"json":return createjs.AbstractLoader.JSON;case"xml":return createjs.AbstractLoader.XML;case"css":return createjs.AbstractLoader.CSS;case"js":return createjs.AbstractLoader.JAVASCRIPT;case"svg":return createjs.AbstractLoader.SVG;default:return createjs.AbstractLoader.TEXT}},createjs.RequestUtils=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.EventDispatcher_constructor(),this.loaded=!1,this.canceled=!1,this.progress=0,this.type=c,this.resultFormatter=null,this._item=a?createjs.LoadItem.create(a):null,this._preferXHR=b,this._result=null,this._rawResult=null,this._loadedItems=null,this._tagSrcAttribute=null,this._tag=null}var b=createjs.extend(a,createjs.EventDispatcher),c=a;c.POST="POST",c.GET="GET",c.BINARY="binary",c.CSS="css",c.IMAGE="image",c.JAVASCRIPT="javascript",c.JSON="json",c.JSONP="jsonp",c.MANIFEST="manifest",c.SOUND="sound",c.VIDEO="video",c.SPRITESHEET="spritesheet",c.SVG="svg",c.TEXT="text",c.XML="xml",b.getItem=function(){return this._item},b.getResult=function(a){return a?this._rawResult:this._result},b.getTag=function(){return this._tag},b.setTag=function(a){this._tag=a},b.load=function(){this._createRequest(),this._request.on("complete",this,this),this._request.on("progress",this,this),this._request.on("loadStart",this,this),this._request.on("abort",this,this),this._request.on("timeout",this,this),this._request.on("error",this,this);var a=new createjs.Event("initialize");a.loader=this._request,this.dispatchEvent(a),this._request.load()},b.cancel=function(){this.canceled=!0,this.destroy()},b.destroy=function(){this._request&&(this._request.removeAllEventListeners(),this._request.destroy()),this._request=null,this._item=null,this._rawResult=null,this._result=null,this._loadItems=null,this.removeAllEventListeners()},b.getLoadedItems=function(){return this._loadedItems},b._createRequest=function(){this._request=this._preferXHR?new createjs.XHRRequest(this._item):new createjs.TagRequest(this._item,this._tag||this._createTag(),this._tagSrcAttribute)},b._createTag=function(){return null},b._sendLoadStart=function(){this._isCanceled()||this.dispatchEvent("loadstart")},b._sendProgress=function(a){if(!this._isCanceled()){var b=null;"number"==typeof a?(this.progress=a,b=new createjs.ProgressEvent(this.progress)):(b=a,this.progress=a.loaded/a.total,b.progress=this.progress,(isNaN(this.progress)||1/0==this.progress)&&(this.progress=0)),this.hasEventListener("progress")&&this.dispatchEvent(b)}},b._sendComplete=function(){if(!this._isCanceled()){this.loaded=!0;var a=new createjs.Event("complete");a.rawResult=this._rawResult,null!=this._result&&(a.result=this._result),this.dispatchEvent(a)}},b._sendError=function(a){!this._isCanceled()&&this.hasEventListener("error")&&(null==a&&(a=new createjs.ErrorEvent("PRELOAD_ERROR_EMPTY")),this.dispatchEvent(a))},b._isCanceled=function(){return null==window.createjs||this.canceled?!0:!1},b.resultFormatter=null,b.handleEvent=function(a){switch(a.type){case"complete":this._rawResult=a.target._response;var b=this.resultFormatter&&this.resultFormatter(this),c=this;b instanceof Function?b(function(a){c._result=a,c._sendComplete()}):(this._result=b||this._rawResult,this._sendComplete());break;case"progress":this._sendProgress(a);break;case"error":this._sendError(a);break;case"loadstart":this._sendLoadStart();break;case"abort":case"timeout":this._isCanceled()||this.dispatchEvent(a.type)}},b.buildPath=function(a,b){return createjs.RequestUtils.buildPath(a,b)},b.toString=function(){return"[PreloadJS AbstractLoader]"},createjs.AbstractLoader=createjs.promote(a,"EventDispatcher")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractLoader_constructor(a,b,c),this.resultFormatter=this._formatResult,this._tagSrcAttribute="src"}var b=createjs.extend(a,createjs.AbstractLoader);b.load=function(){this._tag||(this._tag=this._createTag(this._item.src)),this._tag.preload="auto",this._tag.load(),this.AbstractLoader_load()},b._createTag=function(){},b._createRequest=function(){this._request=this._preferXHR?new createjs.XHRRequest(this._item):new createjs.MediaTagRequest(this._item,this._tag||this._createTag(),this._tagSrcAttribute)},b._formatResult=function(a){return this._tag.removeEventListener&&this._tag.removeEventListener("canplaythrough",this._loadedHandler),this._tag.onstalled=null,this._preferXHR&&(a.getTag().src=a.getResult(!0)),a.getTag()},createjs.AbstractMediaLoader=createjs.promote(a,"AbstractLoader")}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(a){this._item=a},b=createjs.extend(a,createjs.EventDispatcher);b.load=function(){},b.destroy=function(){},b.cancel=function(){},createjs.AbstractRequest=createjs.promote(a,"EventDispatcher")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractRequest_constructor(a),this._tag=b,this._tagSrcAttribute=c,this._loadedHandler=createjs.proxy(this._handleTagComplete,this),this._addedToDOM=!1,this._startTagVisibility=null}var b=createjs.extend(a,createjs.AbstractRequest);b.load=function(){null==this._tag.parentNode&&(window.document.body.appendChild(this._tag),this._addedToDOM=!0),this._tag.onload=createjs.proxy(this._handleTagComplete,this),this._tag.onreadystatechange=createjs.proxy(this._handleReadyStateChange,this);var a=new createjs.Event("initialize");a.loader=this._tag,this.dispatchEvent(a),this._hideTag(),this._tag[this._tagSrcAttribute]=this._item.src},b.destroy=function(){this._clean(),this._tag=null,this.AbstractRequest_destroy()},b._handleReadyStateChange=function(){clearTimeout(this._loadTimeout);var a=this._tag;("loaded"==a.readyState||"complete"==a.readyState)&&this._handleTagComplete()},b._handleTagComplete=function(){this._rawResult=this._tag,this._result=this.resultFormatter&&this.resultFormatter(this)||this._rawResult,this._clean(),this._showTag(),this.dispatchEvent("complete")},b._clean=function(){this._tag.onload=null,this._tag.onreadystatechange=null,this._addedToDOM&&null!=this._tag.parentNode&&this._tag.parentNode.removeChild(this._tag)},b._hideTag=function(){this._startTagVisibility=this._tag.style.visibility,this._tag.style.visibility="hidden"},b._showTag=function(){this._tag.style.visibility=this._startTagVisibility},b._handleStalled=function(){},createjs.TagRequest=createjs.promote(a,"AbstractRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c){this.AbstractRequest_constructor(a),this._tag=b,this._tagSrcAttribute=c,this._loadedHandler=createjs.proxy(this._handleTagComplete,this)}var b=createjs.extend(a,createjs.TagRequest);b.load=function(){this._tag.onstalled=createjs.proxy(this._handleStalled,this),this._tag.onprogress=createjs.proxy(this._handleProgress,this),this._tag.addEventListener&&this._tag.addEventListener("canplaythrough",this._loadedHandler,!1),this.TagRequest_load()},b._handleReadyStateChange=function(){clearTimeout(this._loadTimeout);var a=this._tag;("loaded"==a.readyState||"complete"==a.readyState)&&this._handleTagComplete()},b._handleStalled=function(){},b._handleProgress=function(a){if(a&&!(a.loaded>0&&0==a.total)){var b=new createjs.ProgressEvent(a.loaded,a.total);this.dispatchEvent(b)}},b._clean=function(){this._tag.removeEventListener&&this._tag.removeEventListener("canplaythrough",this._loadedHandler),this._tag.onstalled=null,this._tag.onprogress=null,this.TagRequest__clean()},createjs.MediaTagRequest=createjs.promote(a,"TagRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.AbstractRequest_constructor(a),this._request=null,this._loadTimeout=null,this._xhrLevel=1,this._response=null,this._rawResponse=null,this._canceled=!1,this._handleLoadStartProxy=createjs.proxy(this._handleLoadStart,this),this._handleProgressProxy=createjs.proxy(this._handleProgress,this),this._handleAbortProxy=createjs.proxy(this._handleAbort,this),this._handleErrorProxy=createjs.proxy(this._handleError,this),this._handleTimeoutProxy=createjs.proxy(this._handleTimeout,this),this._handleLoadProxy=createjs.proxy(this._handleLoad,this),this._handleReadyStateChangeProxy=createjs.proxy(this._handleReadyStateChange,this),!this._createXHR(a)}var b=createjs.extend(a,createjs.AbstractRequest);a.ACTIVEX_VERSIONS=["Msxml2.XMLHTTP.6.0","Msxml2.XMLHTTP.5.0","Msxml2.XMLHTTP.4.0","MSXML2.XMLHTTP.3.0","MSXML2.XMLHTTP","Microsoft.XMLHTTP"],b.getResult=function(a){return a&&this._rawResponse?this._rawResponse:this._response},b.cancel=function(){this.canceled=!0,this._clean(),this._request.abort()},b.load=function(){if(null==this._request)return void this._handleError();this._request.addEventListener("loadstart",this._handleLoadStartProxy,!1),this._request.addEventListener("progress",this._handleProgressProxy,!1),this._request.addEventListener("abort",this._handleAbortProxy,!1),this._request.addEventListener("error",this._handleErrorProxy,!1),this._request.addEventListener("timeout",this._handleTimeoutProxy,!1),this._request.addEventListener("load",this._handleLoadProxy,!1),this._request.addEventListener("readystatechange",this._handleReadyStateChangeProxy,!1),1==this._xhrLevel&&(this._loadTimeout=setTimeout(createjs.proxy(this._handleTimeout,this),this._item.loadTimeout));try{this._item.values&&this._item.method!=createjs.AbstractLoader.GET?this._item.method==createjs.AbstractLoader.POST&&this._request.send(createjs.RequestUtils.formatQueryString(this._item.values)):this._request.send()}catch(a){this.dispatchEvent(new createjs.ErrorEvent("XHR_SEND",null,a))}},b.setResponseType=function(a){this._request.responseType=a},b.getAllResponseHeaders=function(){return this._request.getAllResponseHeaders instanceof Function?this._request.getAllResponseHeaders():null},b.getResponseHeader=function(a){return this._request.getResponseHeader instanceof Function?this._request.getResponseHeader(a):null},b._handleProgress=function(a){if(a&&!(a.loaded>0&&0==a.total)){var b=new createjs.ProgressEvent(a.loaded,a.total);this.dispatchEvent(b)}},b._handleLoadStart=function(){clearTimeout(this._loadTimeout),this.dispatchEvent("loadstart")},b._handleAbort=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent("XHR_ABORTED",null,a))},b._handleError=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent(a.message))},b._handleReadyStateChange=function(){4==this._request.readyState&&this._handleLoad()},b._handleLoad=function(){if(!this.loaded){this.loaded=!0;var a=this._checkError();if(a)return void this._handleError(a);this._response=this._getResponse(),this._clean(),this.dispatchEvent(new createjs.Event("complete"))}},b._handleTimeout=function(a){this._clean(),this.dispatchEvent(new createjs.ErrorEvent("PRELOAD_TIMEOUT",null,a))},b._checkError=function(){var a=parseInt(this._request.status);switch(a){case 404:case 0:return new Error(a)}return null},b._getResponse=function(){if(null!=this._response)return this._response;if(null!=this._request.response)return this._request.response;try{if(null!=this._request.responseText)return this._request.responseText}catch(a){}try{if(null!=this._request.responseXML)return this._request.responseXML}catch(a){}return null},b._createXHR=function(a){var b=createjs.RequestUtils.isCrossDomain(a),c={},d=null;if(window.XMLHttpRequest)d=new XMLHttpRequest,b&&void 0===d.withCredentials&&window.XDomainRequest&&(d=new XDomainRequest);else{for(var e=0,f=s.ACTIVEX_VERSIONS.length;f>e;e++){{s.ACTIVEX_VERSIONS[e]}try{d=new ActiveXObject(axVersions);break}catch(g){}}if(null==d)return!1}a.mimeType&&d.overrideMimeType&&d.overrideMimeType(a.mimeType),this._xhrLevel="string"==typeof d.responseType?2:1;var h=null;if(h=a.method==createjs.AbstractLoader.GET?createjs.RequestUtils.buildPath(a.src,a.values):a.src,d.open(a.method||createjs.AbstractLoader.GET,h,!0),b&&d instanceof XMLHttpRequest&&1==this._xhrLevel&&(c.Origin=location.origin),a.values&&a.method==createjs.AbstractLoader.POST&&(c["Content-Type"]="application/x-www-form-urlencoded"),b||c["X-Requested-With"]||(c["X-Requested-With"]="XMLHttpRequest"),a.headers)for(var i in a.headers)c[i]=a.headers[i];for(i in c)d.setRequestHeader(i,c[i]);return d instanceof XMLHttpRequest&&void 0!==a.withCredentials&&(d.withCredentials=a.withCredentials),this._request=d,!0},b._clean=function(){clearTimeout(this._loadTimeout),this._request.removeEventListener("loadstart",this._handleLoadStartProxy),this._request.removeEventListener("progress",this._handleProgressProxy),this._request.removeEventListener("abort",this._handleAbortProxy),this._request.removeEventListener("error",this._handleErrorProxy),this._request.removeEventListener("timeout",this._handleTimeoutProxy),this._request.removeEventListener("load",this._handleLoadProxy),this._request.removeEventListener("readystatechange",this._handleReadyStateChangeProxy)},b.toString=function(){return"[PreloadJS XHRRequest]"},createjs.XHRRequest=createjs.promote(a,"AbstractRequest")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b){this.AbstractMediaLoader_constructor(a,b,createjs.AbstractLoader.SOUND),createjs.RequestUtils.isAudioTag(a)?this._tag=a:createjs.RequestUtils.isAudioTag(a.src)?this._tag=a:createjs.RequestUtils.isAudioTag(a.tag)&&(this._tag=createjs.RequestUtils.isAudioTag(a)?a:a.src),null!=this._tag&&(this._preferXHR=!1)}var b=createjs.extend(a,createjs.AbstractMediaLoader),c=a;c.canLoadItem=function(a){return a.type==createjs.AbstractLoader.SOUND},b._createTag=function(a){var b=document.createElement("audio");return b.autoplay=!1,b.preload="none",b.src=a,b},createjs.SoundLoader=createjs.promote(a,"AbstractMediaLoader")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){throw"Sound cannot be instantiated"}function b(a,b){this.init(a,b)}var c=a;c.INTERRUPT_ANY="any",c.INTERRUPT_EARLY="early",c.INTERRUPT_LATE="late",c.INTERRUPT_NONE="none",c.PLAY_INITED="playInited",c.PLAY_SUCCEEDED="playSucceeded",c.PLAY_INTERRUPTED="playInterrupted",c.PLAY_FINISHED="playFinished",c.PLAY_FAILED="playFailed",c.SUPPORTED_EXTENSIONS=["mp3","ogg","mpeg","wav","m4a","mp4","aiff","wma","mid"],c.EXTENSION_MAP={m4a:"mp4"},c.FILE_PATTERN=/^(?:(\w+:)\/{2}(\w+(?:\.\w+)*\/?))?([/.]*?(?:[^?]+)?\/)?((?:[^/?]+)\.(\w+))(?:\?(\S+)?)?$/,c.defaultInterruptBehavior=c.INTERRUPT_NONE,c.alternateExtensions=[],c.activePlugin=null,c._pluginsRegistered=!1,c._lastID=0,c._masterVolume=1,c._masterMute=!1,c._instances=[],c._idHash={},c._preloadHash={},c.addEventListener=null,c.removeEventListener=null,c.removeAllEventListeners=null,c.dispatchEvent=null,c.hasEventListener=null,c._listeners=null,createjs.EventDispatcher.initialize(c),c.getPreloadHandlers=function(){return{callback:createjs.proxy(c.initLoad,c),types:["sound"],extensions:c.SUPPORTED_EXTENSIONS}},c._handleLoadComplete=function(a){var b=a.target.getItem().src;if(c._preloadHash[b])for(var d=0,e=c._preloadHash[b].length;e>d;d++){var f=c._preloadHash[b][d];if(c._preloadHash[b][d]=!0,c.hasEventListener("fileload")){var a=new createjs.Event("fileload");a.src=f.src,a.id=f.id,a.data=f.data,a.sprite=f.sprite,c.dispatchEvent(a)}}},c._handleLoadError=function(a){var b=a.target.getItem().src;if(c._preloadHash[b])for(var d=0,e=c._preloadHash[b].length;e>d;d++){var f=c._preloadHash[b][d];if(c._preloadHash[b][d]=!1,c.hasEventListener("fileerror")){var a=new createjs.Event("fileerror");a.src=f.src,a.id=f.id,a.data=f.data,a.sprite=f.sprite,c.dispatchEvent(a)}}},c._registerPlugin=function(a){return a.isSupported()?(c.activePlugin=new a,!0):!1},c.registerPlugins=function(a){c._pluginsRegistered=!0;for(var b=0,d=a.length;d>b;b++)if(c._registerPlugin(a[b]))return!0;return!1},c.initializeDefaultPlugins=function(){return null!=c.activePlugin?!0:c._pluginsRegistered?!1:c.registerPlugins([createjs.WebAudioPlugin,createjs.HTMLAudioPlugin])?!0:!1},c.isReady=function(){return null!=c.activePlugin},c.getCapabilities=function(){return null==c.activePlugin?null:c.activePlugin._capabilities},c.getCapability=function(a){return null==c.activePlugin?null:c.activePlugin._capabilities[a]},c.initLoad=function(a){return c._registerSound(a)},c._registerSound=function(a){if(!c.initializeDefaultPlugins())return!1;var d=c._parsePath(a.src);if(null==d)return!1;a.src=d.src,a.type="sound";var e=a.data,f=c.activePlugin.defaultNumChannels||null;if(null!=e&&(isNaN(e.channels)?isNaN(e)||(f=parseInt(e)):f=parseInt(e.channels),e.audioSprite))for(var g,h=e.audioSprite.length;h--;)g=e.audioSprite[h],c._idHash[g.id]={src:a.src,startTime:parseInt(g.startTime),duration:parseInt(g.duration)};null!=a.id&&(c._idHash[a.id]={src:a.src});var i=c.activePlugin.register(a,f);return b.create(a.src,f),null!=e&&isNaN(e)?a.data.channels=f||b.maxPerChannel():a.data=f||b.maxPerChannel(),i.type&&(a.type=i.type),i},c.registerSound=function(a,b,d,e){var f={src:a,id:b,data:d};a instanceof Object&&(e=b,f=a),f=createjs.LoadItem.create(f),null!=e&&(f.src=e+a);var g=c._registerSound(f);if(!g)return!1;if(c._preloadHash[f.src]||(c._preloadHash[f.src]=[]),c._preloadHash[f.src].push(f),1==c._preloadHash[f.src].length)g.on("complete",createjs.proxy(this._handleLoadComplete,this)),g.on("error",createjs.proxy(this._handleLoadError,this)),c.activePlugin.preload(g);else if(1==c._preloadHash[f.src][0])return!0;return f},c.registerSounds=function(a,b){var c=[];a.path&&(b?b+=a.path:b=a.path);for(var d=0,e=a.length;e>d;d++)c[d]=createjs.Sound.registerSound(a[d].src,a[d].id,a[d].data,b);return c},c.registerManifest=function(a,b){try{console.log("createjs.Sound.registerManifest is deprecated, please use createjs.Sound.registerSounds.")}catch(c){}return this.registerSounds(a,b)},c.removeSound=function(a,d){if(null==c.activePlugin)return!1;a instanceof Object&&(a=a.src),a=c._getSrcById(a).src,null!=d&&(a=d+a);var e=c._parsePath(a);if(null==e)return!1;a=e.src;for(var f in c._idHash)c._idHash[f].src==a&&delete c._idHash[f];return b.removeSrc(a),delete c._preloadHash[a],c.activePlugin.removeSound(a),!0},c.removeSounds=function(a,b){var c=[];a.path&&(b?b+=a.path:b=a.path);for(var d=0,e=a.length;e>d;d++)c[d]=createjs.Sound.removeSound(a[d].src,b);return c},c.removeManifest=function(a,b){try{console.log("createjs.Sound.removeManifest is deprecated, please use createjs.Sound.removeSounds.")}catch(d){}return c.removeSounds(a,b)},c.removeAllSounds=function(){c._idHash={},c._preloadHash={},b.removeAll(),c.activePlugin&&c.activePlugin.removeAllSounds()},c.loadComplete=function(a){if(!c.isReady())return!1;var b=c._parsePath(a);return a=b?c._getSrcById(b.src).src:c._getSrcById(a).src,1==c._preloadHash[a][0]},c._parsePath=function(a){"string"!=typeof a&&(a=a.toString());var b=a.match(c.FILE_PATTERN);if(null==b)return!1;for(var d=b[4],e=b[5],f=c.getCapabilities(),g=0;!f[e];)if(e=c.alternateExtensions[g++],g>c.alternateExtensions.length)return null;a=a.replace("."+b[5],"."+e);var h={name:d,src:a,extension:e};return h},c.play=function(a,b,d,e,f,g,h,i,j){b instanceof Object&&(d=b.delay,e=b.offset,f=b.loop,g=b.volume,h=b.pan,i=b.startTime,j=b.duration,b=b.interrupt);var k=c.createInstance(a,i,j),l=c._playInstance(k,b,d,e,f,g,h);return l||k._playFailed(),k},c.createInstance=function(a,d,e){if(!c.initializeDefaultPlugins())return new createjs.DefaultSoundInstance(a,d,e);a=c._getSrcById(a);var f=c._parsePath(a.src),g=null;return null!=f&&null!=f.src?(b.create(f.src),null==d&&(d=a.startTime),g=c.activePlugin.create(f.src,d,e||a.duration)):g=new createjs.DefaultSoundInstance(a,d,e),g.uniqueId=c._lastID++,g},c.setVolume=function(a){if(null==Number(a))return!1;if(a=Math.max(0,Math.min(1,a)),c._masterVolume=a,!this.activePlugin||!this.activePlugin.setVolume||!this.activePlugin.setVolume(a))for(var b=this._instances,d=0,e=b.length;e>d;d++)b[d].setMasterVolume(a)},c.getVolume=function(){return c._masterVolume},c.setMute=function(a){if(null==a)return!1;if(this._masterMute=a,!this.activePlugin||!this.activePlugin.setMute||!this.activePlugin.setMute(a))for(var b=this._instances,c=0,d=b.length;d>c;c++)b[c].setMasterMute(a);return!0},c.getMute=function(){return this._masterMute},c.stop=function(){for(var a=this._instances,b=a.length;b--;)a[b].stop()},c._playInstance=function(a,b,d,e,f,g,h){if(b instanceof Object&&(d=b.delay,e=b.offset,f=b.loop,g=b.volume,h=b.pan,b=b.interrupt),b=b||c.defaultInterruptBehavior,null==d&&(d=0),null==e&&(e=a.getPosition()),null==f&&(f=a.loop),null==g&&(g=a.volume),null==h&&(h=a.pan),0==d){var i=c._beginPlaying(a,b,e,f,g,h);if(!i)return!1}else{var j=setTimeout(function(){c._beginPlaying(a,b,e,f,g,h)},d);a.delayTimeoutId=j}return this._instances.push(a),!0},c._beginPlaying=function(a,c,d,e,f,g){if(!b.add(a,c))return!1;var h=a._beginPlaying(d,e,f,g);if(!h){var i=createjs.indexOf(this._instances,a);return i>-1&&this._instances.splice(i,1),!1}return!0},c._getSrcById=function(a){return c._idHash[a]||{src:a}},c._playFinished=function(a){b.remove(a);var c=createjs.indexOf(this._instances,a);c>-1&&this._instances.splice(c,1)},createjs.Sound=a,b.channels={},b.create=function(a,c){var d=b.get(a);return null==d?(b.channels[a]=new b(a,c),!0):!1},b.removeSrc=function(a){var c=b.get(a);return null==c?!1:(c._removeAll(),delete b.channels[a],!0)},b.removeAll=function(){for(var a in b.channels)b.channels[a]._removeAll();b.channels={}},b.add=function(a,c){var d=b.get(a.src);return null==d?!1:d._add(a,c)},b.remove=function(a){var c=b.get(a.src);return null==c?!1:(c._remove(a),!0)},b.maxPerChannel=function(){return d.maxDefault},b.get=function(a){return b.channels[a]};var d=b.prototype;d.constructor=b,d.src=null,d.max=null,d.maxDefault=100,d.length=0,d.init=function(a,b){this.src=a,this.max=b||this.maxDefault,-1==this.max&&(this.max=this.maxDefault),this._instances=[]},d._get=function(a){return this._instances[a]},d._add=function(a,b){return this._getSlot(b,a)?(this._instances.push(a),this.length++,!0):!1},d._remove=function(a){var b=createjs.indexOf(this._instances,a);return-1==b?!1:(this._instances.splice(b,1),this.length--,!0)},d._removeAll=function(){for(var a=this.length-1;a>=0;a--)this._instances[a].stop()},d._getSlot=function(b){var c,d;if(b!=a.INTERRUPT_NONE&&(d=this._get(0),null==d))return!0;for(var e=0,f=this.max;f>e;e++){if(c=this._get(e),null==c)return!0;if(c.playState==a.PLAY_FINISHED||c.playState==a.PLAY_INTERRUPTED||c.playState==a.PLAY_FAILED){d=c;break}b!=a.INTERRUPT_NONE&&(b==a.INTERRUPT_EARLY&&c.getPosition()<d.getPosition()||b==a.INTERRUPT_LATE&&c.getPosition()>d.getPosition())&&(d=c)}return null!=d?(d._interrupt(),this._remove(d),!0):!1},d.toString=function(){return"[Sound SoundChannel]"}}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(a,b,c,d){this.EventDispatcher_constructor(),this.src=a,this.uniqueId=-1,this.playState=null,this.delayTimeoutId=null,this._startTime=Math.max(0,b||0),this._volume=1,createjs.definePropertySupported&&Object.defineProperty(this,"volume",{get:this.getVolume,set:this.setVolume}),this._pan=0,createjs.definePropertySupported&&Object.defineProperty(this,"pan",{get:this.getPan,set:this.setPan}),this._duration=Math.max(0,c||0),createjs.definePropertySupported&&Object.defineProperty(this,"duration",{get:this.getDuration,set:this.setDuration}),this._playbackResource=null,createjs.definePropertySupported&&Object.defineProperty(this,"playbackResource",{get:this.getPlaybackResource,set:this.setPlaybackResource}),d!==!1&&d!==!0&&this.setPlaybackResource(d),this._position=0,createjs.definePropertySupported&&Object.defineProperty(this,"position",{get:this.getPosition,set:this.setPosition}),this._loop=0,createjs.definePropertySupported&&Object.defineProperty(this,"loop",{get:this.getLoop,set:this.setLoop}),this._muted=!1,createjs.definePropertySupported&&Object.defineProperty(this,"muted",{get:this.getMuted,set:this.setMuted}),this._paused=!1,createjs.definePropertySupported&&Object.defineProperty(this,"paused",{get:this.getPaused,set:this.setPaused})
},b=createjs.extend(a,createjs.EventDispatcher);b.play=function(a,b,c,d,e,f){return this.playState==createjs.Sound.PLAY_SUCCEEDED?(a instanceof Object&&(c=a.offset,d=a.loop,e=a.volume,f=a.pan),null!=c&&this.setPosition(c),null!=d&&this.setLoop(d),null!=e&&this.setVolume(e),null!=f&&this.setPan(f),void(this._paused&&this.setPaused(!1))):(this._cleanUp(),createjs.Sound._playInstance(this,a,b,c,d,e,f),this)},b.pause=function(){return this._paused||this.playState!=createjs.Sound.PLAY_SUCCEEDED?!1:(this.setPaused(!0),!0)},b.resume=function(){return this._paused?(this.setPaused(!1),!0):!1},b.stop=function(){return this._position=0,this._paused=!1,this._handleStop(),this._cleanUp(),this.playState=createjs.Sound.PLAY_FINISHED,this},b.destroy=function(){this._cleanUp(),this.src=null,this.playbackResource=null,this.removeAllEventListeners()},b.toString=function(){return"[AbstractSoundInstance]"},b.getPaused=function(){return this._paused},b.setPaused=function(a){return a!==!0&&a!==!1||this._paused==a||1==a&&this.playState!=createjs.Sound.PLAY_SUCCEEDED?void 0:(this._paused=a,a?this._pause():this._resume(),clearTimeout(this.delayTimeoutId),this)},b.setVolume=function(a){return a==this._volume?this:(this._volume=Math.max(0,Math.min(1,a)),this._muted||this._updateVolume(),this)},b.getVolume=function(){return this._volume},b.setMute=function(a){this.setMuted(a)},b.getMute=function(){return this._muted},b.setMuted=function(a){return a===!0||a===!1?(this._muted=a,this._updateVolume(),this):void 0},b.getMuted=function(){return this._muted},b.setPan=function(a){return a==this._pan?this:(this._pan=Math.max(-1,Math.min(1,a)),this._updatePan(),this)},b.getPan=function(){return this._pan},b.getPosition=function(){return this._paused||this.playState!=createjs.Sound.PLAY_SUCCEEDED?this._position:this._calculateCurrentPosition()},b.setPosition=function(a){return this._position=Math.max(0,a),this.playState==createjs.Sound.PLAY_SUCCEEDED&&this._updatePosition(),this},b.getDuration=function(){return this._duration},b.setDuration=function(a){return a==this._duration?this:(this._duration=Math.max(0,a||0),this._updateDuration(),this)},b.setPlaybackResource=function(a){return this._playbackResource=a,0==this._duration&&this._setDurationFromSource(),this},b.getPlaybackResource=function(){return this._playbackResource},b.getLoop=function(){return this._loop},b.setLoop=function(a){null!=this._playbackResource&&(0!=this._loop&&0==a&&this._removeLooping(a),0==this._loop&&0!=a&&this._addLooping(a)),this._loop=a},b._sendEvent=function(a){var b=new createjs.Event(a);this.dispatchEvent(b)},b._cleanUp=function(){clearTimeout(this.delayTimeoutId),this._handleCleanUp(),this._paused=!1,createjs.Sound._playFinished(this)},b._interrupt=function(){this._cleanUp(),this.playState=createjs.Sound.PLAY_INTERRUPTED,this._sendEvent("interrupted")},b._beginPlaying=function(a,b,c,d){return this.setPosition(a),this.setLoop(b),this.setVolume(c),this.setPan(d),null!=this._playbackResource&&this._position<this._duration?(this._paused=!1,this._handleSoundReady(),this.playState=createjs.Sound.PLAY_SUCCEEDED,this._sendEvent("succeeded"),!0):(this._playFailed(),!1)},b._playFailed=function(){this._cleanUp(),this.playState=createjs.Sound.PLAY_FAILED,this._sendEvent("failed")},b._handleSoundComplete=function(){return this._position=0,0!=this._loop?(this._loop--,this._handleLoop(),void this._sendEvent("loop")):(this._cleanUp(),this.playState=createjs.Sound.PLAY_FINISHED,void this._sendEvent("complete"))},b._handleSoundReady=function(){},b._updateVolume=function(){},b._updatePan=function(){},b._updateDuration=function(){},b._setDurationFromSource=function(){},b._calculateCurrentPosition=function(){},b._updatePosition=function(){},b._removeLooping=function(){},b._addLooping=function(){},b._pause=function(){},b._resume=function(){},b._handleStop=function(){},b._handleCleanUp=function(){},b._handleLoop=function(){},createjs.AbstractSoundInstance=createjs.promote(a,"EventDispatcher"),createjs.DefaultSoundInstance=createjs.AbstractSoundInstance}(),this.createjs=this.createjs||{},function(){"use strict";var a=function(){this._capabilities=null,this._loaders={},this._audioSources={},this._soundInstances={},this._loaderClass,this._soundInstanceClass},b=a.prototype;a._capabilities=null,a.isSupported=function(){return!0},b.register=function(a){if(this._audioSources[a.src]=!0,this._soundInstances[a.src]=[],this._loaders[a.src])return this._loaders[a.src];var b=new this._loaderClass(a);return b.on("complete",createjs.proxy(this._handlePreloadComplete,this)),this._loaders[a.src]=b,b},b.preload=function(a){a.on("error",createjs.proxy(this._handlePreloadError,this)),a.load()},b.isPreloadStarted=function(a){return null!=this._audioSources[a]},b.isPreloadComplete=function(a){return!(null==this._audioSources[a]||1==this._audioSources[a])},b.removeSound=function(a){if(this._soundInstances[a]){for(var b=this._soundInstances[a].length;b--;){var c=this._soundInstances[a][b];c.destroy()}delete this._soundInstances[a],delete this._audioSources[a],this._loaders[a]&&this._loaders[a].destroy(),delete this._loaders[a]}},b.removeAllSounds=function(){for(var a in this._audioSources)this.removeSound(a)},b.create=function(a,b,c){this.isPreloadStarted(a)||this.preload(this.register(a));var d=new this._soundInstanceClass(a,b,c,this._audioSources[a]);return this._soundInstances[a].push(d),d},b.setVolume=function(a){return this._volume=a,this._updateVolume(),!0},b.getVolume=function(){return this._volume},b.setMute=function(){return this._updateVolume(),!0},b.toString=function(){return"[AbstractPlugin]"},b._handlePreloadComplete=function(a){var b=a.target.getItem().src;this._audioSources[b]=a.result;for(var c=0,d=this._soundInstances[b].length;d>c;c++){var e=this._soundInstances[b][c];e.setPlaybackResource(this._audioSources[b])}},b._handlePreloadError=function(){},b._updateVolume=function(){},createjs.AbstractPlugin=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.AbstractLoader_constructor(a,!0,createjs.AbstractLoader.SOUND)}var b=createjs.extend(a,createjs.AbstractLoader);a.context=null,b.toString=function(){return"[WebAudioLoader]"},b._createRequest=function(){this._request=new createjs.XHRRequest(this._item,!1),this._request.setResponseType("arraybuffer")},b._sendComplete=function(){a.context.decodeAudioData(this._rawResult,createjs.proxy(this._handleAudioDecoded,this),createjs.proxy(this._handleError,this))},b._handleAudioDecoded=function(a){this._result=a,this.AbstractLoader__sendComplete()},createjs.WebAudioLoader=createjs.promote(a,"AbstractLoader")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,d,e){this.AbstractSoundInstance_constructor(a,b,d,e),this.gainNode=c.context.createGain(),this.panNode=c.context.createPanner(),this.panNode.panningModel=c._panningModel,this.panNode.connect(this.gainNode),this.sourceNode=null,this._soundCompleteTimeout=null,this._sourceNodeNext=null,this._playbackStartTime=0,this._endedHandler=createjs.proxy(this._handleSoundComplete,this)}var b=createjs.extend(a,createjs.AbstractSoundInstance),c=a;c.context=null,c.destinationNode=null,c._panningModel="equalpower",b.destroy=function(){this.AbstractSoundInstance_destroy(),this.panNode.disconnect(0),this.panNode=null,this.gainNode.disconnect(0),this.gainNode=null},b.toString=function(){return"[WebAudioSoundInstance]"},b._updatePan=function(){this.panNode.setPosition(this._pan,0,-.5)},b._removeLooping=function(){this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext)},b._addLooping=function(){this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0))},b._setDurationFromSource=function(){this._duration=1e3*this.playbackResource.duration},b._handleCleanUp=function(){this.sourceNode&&this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext)),0!=this.gainNode.numberOfOutputs&&this.gainNode.disconnect(0),clearTimeout(this._soundCompleteTimeout),this._playbackStartTime=0},b._cleanUpAudioNode=function(a){return a&&(a.stop(0),a.disconnect(0),a=null),a},b._handleSoundReady=function(){this.gainNode.connect(c.destinationNode);var a=.001*this._duration,b=.001*this._position;this.sourceNode=this._createAndPlayAudioNode(c.context.currentTime-a,b),this._playbackStartTime=this.sourceNode.startTime-b,this._soundCompleteTimeout=setTimeout(this._endedHandler,1e3*(a-b)),0!=this._loop&&(this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0))},b._createAndPlayAudioNode=function(a,b){var d=c.context.createBufferSource();d.buffer=this.playbackResource,d.connect(this.panNode);var e=.001*this._duration;return d.startTime=a+e,d.start(d.startTime,b+.001*this._startTime,e-b),d},b._pause=function(){this._position=1e3*(c.context.currentTime-this._playbackStartTime),this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext),0!=this.gainNode.numberOfOutputs&&this.gainNode.disconnect(0),clearTimeout(this._soundCompleteTimeout)},b._resume=function(){this._handleSoundReady()},b._updateVolume=function(){var a=this._muted?0:this._volume;a!=this.gainNode.gain.value&&(this.gainNode.gain.value=a)},b._calculateCurrentPosition=function(){return 1e3*(c.context.currentTime-this._playbackStartTime)},b._updatePosition=function(){this.sourceNode=this._cleanUpAudioNode(this.sourceNode),this._sourceNodeNext=this._cleanUpAudioNode(this._sourceNodeNext),clearTimeout(this._soundCompleteTimeout),this._paused||this._handleSoundReady()},b._handleLoop=function(){this._cleanUpAudioNode(this.sourceNode),this.sourceNode=this._sourceNodeNext,this._playbackStartTime=this.sourceNode.startTime,this._sourceNodeNext=this._createAndPlayAudioNode(this._playbackStartTime,0),this._soundCompleteTimeout=setTimeout(this._endedHandler,this._duration)},b._updateDuration=function(){this._pause(),this._resume()},createjs.WebAudioSoundInstance=createjs.promote(a,"AbstractSoundInstance")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this.AbstractPlugin_constructor(),this._panningModel=c._panningModel,this._volume=1,this.context=c.context,this.dynamicsCompressorNode=this.context.createDynamicsCompressor(),this.dynamicsCompressorNode.connect(this.context.destination),this.gainNode=this.context.createGain(),this.gainNode.connect(this.dynamicsCompressorNode),createjs.WebAudioSoundInstance.destinationNode=this.gainNode,this._capabilities=c._capabilities,this._loaderClass=createjs.WebAudioLoader,this._soundInstanceClass=createjs.WebAudioSoundInstance,this._addPropsToClasses()}var b=createjs.extend(a,createjs.AbstractPlugin),c=a;c._capabilities=null,c._panningModel="equalpower",c.context=null,c.isSupported=function(){var a=createjs.BrowserDetect.isIOS||createjs.BrowserDetect.isAndroid||createjs.BrowserDetect.isBlackberry;return"file:"!=location.protocol||a||this._isFileXHRSupported()?(c._generateCapabilities(),null==c.context?!1:!0):!1},c.playEmptySound=function(){var a=c.context.createBufferSource();a.buffer=c.context.createBuffer(1,1,22050),a.connect(c.context.destination),a.start(0,0,0)},c._isFileXHRSupported=function(){var a=!0,b=new XMLHttpRequest;try{b.open("GET","WebAudioPluginTest.fail",!1)}catch(c){return a=!1}b.onerror=function(){a=!1},b.onload=function(){a=404==this.status||200==this.status||0==this.status&&""!=this.response};try{b.send()}catch(c){a=!1}return a},c._generateCapabilities=function(){if(null==c._capabilities){var a=document.createElement("audio");if(null==a.canPlayType)return null;if(null==c.context)if(window.AudioContext)c.context=new AudioContext;else{if(!window.webkitAudioContext)return null;c.context=new webkitAudioContext}c._compatibilitySetUp(),c.playEmptySound(),c._capabilities={panning:!0,volume:!0,tracks:-1};for(var b=createjs.Sound.SUPPORTED_EXTENSIONS,d=createjs.Sound.EXTENSION_MAP,e=0,f=b.length;f>e;e++){var g=b[e],h=d[g]||g;c._capabilities[g]="no"!=a.canPlayType("audio/"+g)&&""!=a.canPlayType("audio/"+g)||"no"!=a.canPlayType("audio/"+h)&&""!=a.canPlayType("audio/"+h)}c.context.destination.numberOfChannels<2&&(c._capabilities.panning=!1)}},c._compatibilitySetUp=function(){if(c._panningModel="equalpower",!c.context.createGain){c.context.createGain=c.context.createGainNode;var a=c.context.createBufferSource();a.__proto__.start=a.__proto__.noteGrainOn,a.__proto__.stop=a.__proto__.noteOff,c._panningModel=0}},b.toString=function(){return"[WebAudioPlugin]"},b._addPropsToClasses=function(){var a=this._soundInstanceClass;a.context=this.context,a.destinationNode=this.gainNode,a._panningModel=this._panningModel,this._loaderClass.context=this.context},b._updateVolume=function(){var a=createjs.Sound._masterMute?0:this._volume;a!=this.gainNode.gain.value&&(this.gainNode.gain.value=a)},createjs.WebAudioPlugin=createjs.promote(a,"AbstractPlugin")}(),this.createjs=this.createjs||{},function(){"use strict";function a(a){this.src=a,this.length=0,this.available=0,this.tags=[],this.duration=0}var b=a.prototype;b.constructor=a;var c=a;c.tags={},c.get=function(b){var d=c.tags[b];return null==d&&(d=c.tags[b]=new a(b)),d},c.remove=function(a){var b=c.tags[a];return null==b?!1:(b.removeAll(),delete c.tags[a],!0)},c.getInstance=function(a){var b=c.tags[a];return null==b?null:b.get()},c.setInstance=function(a,b){var d=c.tags[a];return null==d?null:d.set(b)},c.getDuration=function(a){var b=c.tags[a];return null==b?0:b.getDuration()},b.add=function(a){this.tags.push(a),this.length++,this.available++},b.removeAll=function(){for(var a;this.length--;)a=this.tags[this.length],a.parentNode&&a.parentNode.removeChild(a),delete this.tags[this.length];this.src=null,this.tags.length=0},b.get=function(){if(0==this.tags.length)return null;this.available=this.tags.length;var a=this.tags.pop();return null==a.parentNode&&document.body.appendChild(a),a},b.set=function(a){var b=createjs.indexOf(this.tags,a);-1==b&&this.tags.push(a),this.available=this.tags.length},b.getDuration=function(){return this.duration||(this.duration=1e3*this.tags[this.tags.length-1].duration),this.duration},b.toString=function(){return"[HTMLAudioTagPool]"},createjs.HTMLAudioTagPool=a}(),this.createjs=this.createjs||{},function(){"use strict";function a(a,b,c,d){this.AbstractSoundInstance_constructor(a,b,c,d),this._audioSpriteStopTime=null,this._delayTimeoutId=null,this._endedHandler=createjs.proxy(this._handleSoundComplete,this),this._readyHandler=createjs.proxy(this._handleTagReady,this),this._stalledHandler=createjs.proxy(this.playFailed,this),this._audioSpriteEndHandler=createjs.proxy(this._handleAudioSpriteLoop,this),this._loopHandler=createjs.proxy(this._handleSoundComplete,this),c?this._audioSpriteStopTime=.001*(b+c):this._duration=createjs.HTMLAudioTagPool.getDuration(this.src)}var b=createjs.extend(a,createjs.AbstractSoundInstance);b.setMasterVolume=function(){this._updateVolume()},b.setMasterMute=function(){this._updateVolume()},b.toString=function(){return"[HTMLAudioSoundInstance]"},b._removeLooping=function(){null!=this._playbackResource&&(this._playbackResource.loop=!1,this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._addLooping=function(){null==this._playbackResource||this._audioSpriteStopTime||(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.loop=!0)},b._handleCleanUp=function(){var a=this._playbackResource;if(null!=a){a.pause(),a.loop=!1,a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),a.removeEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1);try{a.currentTime=this._startTime}catch(b){}createjs.HTMLAudioTagPool.setInstance(this.src,a),this._playbackResource=null}},b._beginPlaying=function(a,b,c,d){return this._playbackResource=createjs.HTMLAudioTagPool.getInstance(this.src),this.AbstractSoundInstance__beginPlaying(a,b,c,d)},b._handleSoundReady=function(){if(4!==this._playbackResource.readyState){var a=this._playbackResource;return a.addEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),a.addEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),a.preload="auto",void a.load()}this._updateVolume(),this._playbackResource.currentTime=.001*(this._startTime+this._position),this._audioSpriteStopTime?this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1):(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),0!=this._loop&&(this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.loop=!0)),this._playbackResource.play()},b._handleTagReady=function(){this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_READY,this._readyHandler,!1),this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_STALLED,this._stalledHandler,!1),this._handleSoundReady()},b._pause=function(){this._playbackResource.pause()},b._resume=function(){this._playbackResource.play()},b._updateVolume=function(){if(null!=this._playbackResource){var a=this._muted||createjs.Sound._masterMute?0:this._volume*createjs.Sound._masterVolume;a!=this._playbackResource.volume&&(this._playbackResource.volume=a)}},b._calculateCurrentPosition=function(){return 1e3*this._playbackResource.currentTime-this._startTime},b._updatePosition=function(){this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._handleSetPositionSeek,!1);try{this._playbackResource.currentTime=.001*(this._position+this._startTime)}catch(a){this._handleSetPositionSeek(null)}},b._handleSetPositionSeek=function(){null!=this._playbackResource&&(this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._handleSetPositionSeek,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._handleAudioSpriteLoop=function(){this._playbackResource.currentTime<=this._audioSpriteStopTime||(this._playbackResource.pause(),0==this._loop?this._handleSoundComplete(null):(this._position=0,this._loop--,this._playbackResource.currentTime=.001*this._startTime,this._paused||this._playbackResource.play(),this._sendEvent("loop")))},b._handleLoop=function(){0==this._loop&&(this._playbackResource.loop=!1,this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_SEEKED,this._loopHandler,!1))},b._updateDuration=function(){this._audioSpriteStopTime=.001*(startTime+duration),this.playState==createjs.Sound.PLAY_SUCCEEDED&&(this._playbackResource.removeEventListener(createjs.HTMLAudioPlugin._AUDIO_ENDED,this._endedHandler,!1),this._playbackResource.addEventListener(createjs.HTMLAudioPlugin._TIME_UPDATE,this._audioSpriteEndHandler,!1))},createjs.HTMLAudioSoundInstance=createjs.promote(a,"AbstractSoundInstance")}(),this.createjs=this.createjs||{},function(){"use strict";function a(){this.AbstractPlugin_constructor(),this.defaultNumChannels=2,this._capabilities=c._capabilities,this._loaderClass=createjs.SoundLoader,this._soundInstanceClass=createjs.HTMLAudioSoundInstance}var b=createjs.extend(a,createjs.AbstractPlugin),c=a;c.MAX_INSTANCES=30,c._AUDIO_READY="canplaythrough",c._AUDIO_ENDED="ended",c._AUDIO_SEEKED="seeked",c._AUDIO_STALLED="stalled",c._TIME_UPDATE="timeupdate",c._capabilities=null,c.enableIOS=!1,c.isSupported=function(){return c._generateCapabilities(),null==c._capabilities?!1:!0},c._generateCapabilities=function(){if(null==c._capabilities){var a=document.createElement("audio");if(null==a.canPlayType)return null;c._capabilities={panning:!0,volume:!0,tracks:-1};for(var b=createjs.Sound.SUPPORTED_EXTENSIONS,d=createjs.Sound.EXTENSION_MAP,e=0,f=b.length;f>e;e++){var g=b[e],h=d[g]||g;c._capabilities[g]="no"!=a.canPlayType("audio/"+g)&&""!=a.canPlayType("audio/"+g)||"no"!=a.canPlayType("audio/"+h)&&""!=a.canPlayType("audio/"+h)}}},b.register=function(a,b){for(var c=createjs.HTMLAudioTagPool.get(a.src),d=null,e=0;b>e;e++)d=this._createTag(a.src),c.add(d);var f=this.AbstractPlugin_register(a,b);return f.setTag(d),f},b.removeSound=function(a){this.AbstractPlugin_removeSound(a),createjs.HTMLAudioTagPool.remove(a)},b.create=function(a,b,c){var d=this.AbstractPlugin_create(a,b,c);return d.setPlaybackResource(null),d},b.toString=function(){return"[HTMLAudioPlugin]"},b.setVolume=b.getVolume=b.setMute=null,b._createTag=function(a){var b=document.createElement("audio");return b.autoplay=!1,b.preload="none",b.src=a,b},createjs.HTMLAudioPlugin=createjs.promote(a,"AbstractPlugin")}();

H5P.SoundJS = this.createjs.Sound;

this.createjs = old || this.createjs;
;H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SoundEffects = (function ($) {
  var isDefined = false;

  SoundEffects = {
    types: [
      'positive-short',
      'negative-short'
    ],
    sounds: [],
    libraryPath: undefined,
    muted: false
  };

  /**
   * Setup defined sounds
   *
   * @return {boolean} True if setup was successfull, otherwise false
   */
  SoundEffects.setup = function () {
    if (isDefined || !H5P.SoundJS.initializeDefaultPlugins()) {
      return false;
    }

    SoundEffects.libraryPath = H5P.getLibraryPath('H5P.SingleChoiceSet-1.3');
    H5P.SoundJS.alternateExtensions = ['mp3'];
    for (var i = 0; i < SoundEffects.types.length; i++) {
      var type = SoundEffects.types[i];
      H5P.SoundJS.registerSound(SoundEffects.libraryPath + '/sounds/' + type + '.ogg', type);
    }
    isDefined = true;

    return true;
  };

  /**
   * Play a sound
   *
   * @param  {string} type  Name of the sound as defined in [SoundEffects.types]{@link H5P.SoundEffects.SoundEffects#types}
   * @param  {number} delay Delay in milliseconds
   */
  SoundEffects.play = function (type, delay) {
    if (SoundEffects.muted === false) {
      H5P.SoundJS.play(type, H5P.SoundJS.INTERRUPT_NONE, (delay || 0));
    }
  };

  /**
   * Mute. Subsequent invocations of SoundEffects.play() will not make any sounds beeing played.
   */
  SoundEffects.mute = function () {
    SoundEffects.muted = true;
  };

  /**
   * Unmute
   */
  SoundEffects.unmute = function () {
    SoundEffects.muted = false;
  };

  return SoundEffects;
})(H5P.jQuery);
;H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SoundEffects = (function ($) {
  var isDefined = false;

  SoundEffects = {
    types: [
      'positive-short',
      'negative-short'
    ],
    sounds: [],
    libraryPath: undefined,
    muted: false
  };

  /**
   * Setup defined sounds
   *
   * @return {boolean} True if setup was successfull, otherwise false
   */
  SoundEffects.setup = function () {
    if (isDefined || !H5P.SoundJS.initializeDefaultPlugins()) {
      return false;
    }

    SoundEffects.libraryPath = H5P.getLibraryPath('H5P.SingleChoiceSet-1.3');
    H5P.SoundJS.alternateExtensions = ['mp3'];
    for (var i = 0; i < SoundEffects.types.length; i++) {
      var type = SoundEffects.types[i];
      H5P.SoundJS.registerSound(SoundEffects.libraryPath + '/sounds/' + type + '.ogg', type);
    }
    isDefined = true;

    return true;
  };

  /**
   * Play a sound
   *
   * @param  {string} type  Name of the sound as defined in [SoundEffects.types]{@link H5P.SoundEffects.SoundEffects#types}
   * @param  {number} delay Delay in milliseconds
   */
  SoundEffects.play = function (type, delay) {
    if (SoundEffects.muted === false) {
      H5P.SoundJS.play(type, H5P.SoundJS.INTERRUPT_NONE, (delay || 0));
    }
  };

  /**
   * Mute. Subsequent invocations of SoundEffects.play() will not make any sounds beeing played.
   */
  SoundEffects.mute = function () {
    SoundEffects.muted = true;
  };

  /**
   * Unmute
   */
  SoundEffects.unmute = function () {
    SoundEffects.muted = false;
  };

  return SoundEffects;
})(H5P.jQuery);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

/**
 * SingleChoiceEventEmitter makes it possible for other classes to
 * trigger and listen to events
 */
H5P.SingleChoiceSet.EventEmitter = (function () {

  /**
   * @constructor
   */
  function EventEmitter() {
    this.listeners = {};
  }

  /**
   * EventEmitter.prototype.on - Register a listener
   *
   * @param  {string} type        The name of the event
   * @param  {function} listener  The function to call when event is triggered
   */
  EventEmitter.prototype.on = function (type, listener, self) {
    if (typeof listener === 'function') {
      if (this.listeners[type] === undefined) {
        this.listeners[type] = [];
      }
      this.listeners[type].push({
        function: listener,
        self: self
      });
    }
  };


  /**
   * EventEmitter.prototype.off - Unregister a listener
   *
   * @param  {string} type        The name of the event
   * @param  {function} listener  The function to unregister
   */
  EventEmitter.prototype.off = function (type, listener) {
    if (this.listeners[type] !== undefined) {
      var removeIndex = listeners[type].indexOf(listener);
      if (removeIndex) {
        listeners[type].splice(removeIndex, 1);
      }
    }
  };

  /**
   * EventEmitter.prototype.trigger - Trigger an event
   *
   * @param  {string} type  The name of the event
   * @param  {object} event Object data
   */
  EventEmitter.prototype.trigger = function (type, event) {
    if (event === null) {
      event = {};
    }

    if (this.listeners[type] !== undefined) {
      for (var i = 0; i < this.listeners[type].length; i++) {
        var listener = this.listeners[type][i];
        listener.function.apply(listener.self, [event]);
      }
    }
  };

  return EventEmitter;
})();
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};
/**
 * SingleChoiceResultSlide - Represents the result slide
 */
H5P.SingleChoiceSet.ResultSlide = (function ($, EventEmitter) {

  /**
  * @constructor
  * @param {number} maxscore Max score
  */
  function ResultSlide(maxscore) {
    EventEmitter.call(this);
    this.maxscore = maxscore;
    var self = this;
    this.$feedbackContainer = $('<div>', {
     'class': 'h5p-sc-feedback-container'
    });

    this.$buttonContainer = $('<div/>', {
      'class': 'h5p-sc-button-container'
    });

    var $resultContainer = $('<div/>', {
      'class': 'h5p-sc-result-container'
    }).append(this.$feedbackContainer)
      .append(this.$buttonContainer);

    this.$resultSlide = $('<div>', {
      'class': 'h5p-sc-slide h5p-sc-set-results',
      'css': {left: (maxscore * 100) + '%'}
    }).append($resultContainer);
  }
  ResultSlide.prototype = Object.create(EventEmitter.prototype);
  ResultSlide.prototype.constructor = ResultSlide;


  /**
   * Append the resultslide to a container
   *
   * @param  {domElement} $container The container
   * @return {domElement}            This dom element
   */
  ResultSlide.prototype.appendTo = function ($container) {
    this.$resultSlide.appendTo($container);
    return this.$resultSlide;
  };

  return ResultSlide;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SolutionView = (function ($) {
  /**
  * Constructor function.
  */
  function SolutionView (choices, l10n){
    var self = this;
    this.choices = choices;

    this.$solutionView = $('<div>', {
      'class': 'h5p-sc-solution-view'
    });

    self.populate();  // Brought it before 
    
    
    // Add header
    this.$header = $('<div>', {
      'class': 'h5p-sc-solution-view-header'
    }).appendTo(this.$solutionView);

    // Close solution view button
    $('<button>', {
      'class': 'h5p-joubelui-button h5p-sc-close-solution-view',
      'click': function () {
        self.hide();
      }
    }).appendTo(this.$header);
    
//    this.$header.append('<div class="h5p-sc-solution-view-title">' + l10n.solutionViewTitle + '</div>'); //not required

//    self.populate();
  }
	
	
	

  /**
   * Will append the solution view to a container DOM
   * @param  {jQuery} $container The DOM object to append to
   */
  SolutionView.prototype.appendTo = function ($container) {
    this.$solutionView.appendTo($container);
  };

  /**
   * Shows the solution view
   */
  SolutionView.prototype.show = function () {
    var self = this;
    self.$solutionView.addClass('visible');

    $(document).on('keyup.solutionview', function (event) {
      if (event.keyCode === 27) { // Escape
        self.hide();
        $(document).off('keyup.solutionview');
      }
    });
  };

  /**
   * Hides the solution view
   */
  SolutionView.prototype.hide = function () {
    this.$solutionView.removeClass('visible');
  };

  /**
   * Populates the solution view
   */
  SolutionView.prototype.populate = function () {
    var self = this;
    self.$choices = $('<div>', {
      'class': 'h5p-sc-solution-choices'
    });
    this.choices.forEach(function (choice) {
      if (choice.question && choice.answers && choice.answers.length !== 0) {
        self.$choices.append($('<div>', {
          'class': 'h5p-sc-solution-question',
          html: choice.question
        }));
        self.$choices.append($('<div>', {
          'class': 'h5p-sc-solution-answer',
          html: choice.answers[0]
        }));
      }
    });
    self.$choices.appendTo(this.$solutionView);
  };

  return SolutionView;
})(H5P.jQuery);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.Alternative = (function ($, EventEmitter) {

  /**
  * @constructor
  *
  * @param {object} options Options for the alternative
  */
  function Alternative(options){
    EventEmitter.call(this);
    var self = this;

    this.options = options;

    var triggerAlternativeSelected = function () {
      self.trigger('alternative-selected', {
        correct: self.options.correct,
        $element: self.$alternative
      });
    };


    this.$alternative = $('<li>', {
      'class': 'h5p-sc-alternative h5p-sc-is-' + (this.options.correct ? 'correct' : 'wrong'),
      tabindex: 1,
      click: triggerAlternativeSelected,
      keypress: function (event) {
        // If enter or space has been pushed
        if(event.which === 13 || event.which === 32) {
          triggerAlternativeSelected();
        }
      }
    });

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-progressbar'
    }));

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-label',
      'html': this.options.text
    }));

    this.$alternative.append($('<div>', {
      'class': 'h5p-sc-status'
    }));

  }
  Alternative.prototype = Object.create(EventEmitter.prototype);
  Alternative.prototype.constructor = Alternative;

  /**
   * Is this alternative the correct one?
   *
   * @return {boolean}  Correct or not?
   */
  Alternative.prototype.isCorrect = function () {
    return this.options.correct;
  };


  /**
   * Append the alternative to a DOM container
   *
   * @param  {domElement} $container The Dom element to append to
   * @return {domElement}            This dom element
   */
  Alternative.prototype.appendTo = function ($container) {
    var self = this;
    $container.append(this.$alternative);
    return this.$alternative;
  };

  return Alternative;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter);
;var H5P = H5P || {};
H5P.SingleChoiceSet = H5P.SingleChoiceSet || {};

H5P.SingleChoiceSet.SingleChoice = (function ($, EventEmitter, Alternative, SoundEffects) {
  /**
   * Constructor function.
   */
  function SingleChoice(options, index) {
    EventEmitter.call(this);
    // Extend defaults with provided options
    this.options = $.extend(true, {}, {
      question: '',
      answers: []
    }, options);
    // Keep provided id.
    this.index = index;

    for (var i = 0; i < this.options.answers.length; i++) {
      this.options.answers[i] = {text: this.options.answers[i], correct: i===0};
    }
    // Randomize alternatives
    this.options.answers = H5P.shuffleArray(this.options.answers);
  }
  SingleChoice.prototype = Object.create(EventEmitter.prototype);
  SingleChoice.prototype.constructor = SingleChoice;

  /**
   * appendTo function invoked to append SingleChoice to container
   *
   * @param {jQuery} $container
   */
   SingleChoice.prototype.appendTo = function ($container, current) {
    var self = this;
    this.$container = $container;

    this.$choice = $('<div>', {
      'class': 'h5p-sc-slide h5p-sc' + (current ? ' h5p-sc-current-slide' : ''),
      css: {'left': (self.index*100) + '%'}
    });

    this.$choice.append($('<div>', {
      'class': 'h5p-sc-question',
      'html': this.options.question
    }));

    var $alternatives = $('<ul>', {
      'class': 'h5p-sc-alternatives'
    });


    /**
     * Handles click on an alternative
     */
    var handleAlternativeSelected = function (data) {
      if (data.$element.parent().hasClass('h5p-sc-selected')) {
        return;
      }
      // Can't play it after the transition end is received, since this is not
      // accepted on iPad. Therefore we are playing it here with a delay instead
      SoundEffects.play(data.correct ? 'positive-short' : 'negative-short', 700);

      H5P.Transition.onTransitionEnd(data.$element.find('.h5p-sc-progressbar'), function () {
        data.$element.addClass('h5p-sc-drummed');
        self.showResult(data.correct);
      }, 700);

      data.$element.addClass('h5p-sc-selected').parent().addClass('h5p-sc-selected');
    };

    for (var i = 0; i < this.options.answers.length; i++) {
      var alternative = new Alternative(this.options.answers[i]);
      alternative.appendTo($alternatives);
      alternative.on('alternative-selected', handleAlternativeSelected);
    }
    this.$choice.append($alternatives);
    $container.append(this.$choice);
    return this.$choice;
  };

  /**
   * Reveals the result for a question
   * @param  {boolean} correct True uf answer was correct, otherwise false
   */
  SingleChoice.prototype.showResult = function (correct) {
    var self = this;

    var $correctAlternative = self.$choice.find('.h5p-sc-is-correct');

    H5P.Transition.onTransitionEnd($correctAlternative, function () {
      self.trigger('finished', {correct: correct});
    }, 600);

    // Reveal corrects and wrong
    self.$choice.find('.h5p-sc-is-wrong').addClass('h5p-sc-reveal-wrong');
    $correctAlternative.addClass('h5p-sc-reveal-correct');
  };

  return SingleChoice;

})(H5P.jQuery, H5P.SingleChoiceSet.EventEmitter, H5P.SingleChoiceSet.Alternative, H5P.SingleChoiceSet.SoundEffects);
;var H5P = H5P || {};

H5P.SingleChoiceSet = (function ($, Question, SingleChoice, SolutionView, ResultSlide, SoundEffects) {
  /**
   * @constructor
   * @extends Question
   * @param {object} options Options for single choice set
   * @param {string} contentId H5P instance id
   * @param {Object} contentData H5P instance data
   */
  function SingleChoiceSet(options, contentId, contentData) {
    var self = this;
	
	//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	/*for(var key in contentData){
		console.log(key+"::"+contentData[key]);
		for (var key1 in contentData[key])
		console.log(key1+"::"+contentData[key][key1]);
	}*/
    // Extend defaults with provided options
    this.contentId = contentId;
    Question.call(this, 'single-choice-set');
    this.options = $.extend(true, {}, {
      choices: [],
      behaviour: {
        timeoutCorrect: 50,
        timeoutWrong: 100,
        soundEffectsEnabled: true,
        enableRetry: retry,
        enableSolutionsButton: showSol
      }
    }, options);
    if (contentData && contentData.previousState !== undefined) {
      this.currentIndex = contentData.previousState.progress;
      this.results = contentData.previousState.answers;
    }
    this.currentIndex = this.currentIndex || 0;
    this.results = this.results || {
      corrects: 0,
      wrongs: 0
    };

    this.l10n = H5P.jQuery.extend({
      resultSlideTitle: 'You got :numcorrect of :maxscore correct',
      showSolutionButtonLabel: 'Show solution',
      retryButtonLabel: 'Retry',
      solutionViewTitle: 'Solution'
    }, options.l10n !== undefined ? options.l10n : {});

    this.$container = $('<div>', {
      'class': 'h5p-sc-set-wrapper'
    });

    this.$slides = [];
    // An array containing the SingleChoice instances
    this.choices = [];

    this.solutionView = new SolutionView(this.options.choices, this.l10n);

    this.$choices = $('<div>', {
      'class': 'h5p-sc-set h5p-sc-animate'
    });
    this.$progressbar = $('<div>', {
      'class': 'h5p-sc-set-progress'
      
    });
    this.$progressCompleted = $('<div>', {
      'class': 'h5p-sc-completed'
    }).appendTo(this.$progressbar);

    // Validate "slides", reverse traversal since we remove entries as we go
    for (var slideIndex = this.options.choices.length - 1; slideIndex >= 0; slideIndex--) {

      // Prune invalid slide
      if (!this.options.choices[slideIndex].answers) {
        this.options.choices.splice(slideIndex, 1);
      }
    }

    for (var i = 0; i < this.options.choices.length; i++) {
      var choice = new SingleChoice(this.options.choices[i], i);
      choice.on('finished', this.handleQuestionFinished, this);
      choice.appendTo(this.$choices, (i === this.currentIndex));
      this.choices.push(choice);
      this.$slides.push(choice.$choice);
    }

    this.resultSlide = new ResultSlide(this.options.choices.length);
    this.resultSlide.appendTo(this.$choices);
    this.resultSlide.on('retry', this.resetTask, this);
    this.resultSlide.on('view-solution', this.handleViewSolution, this);
    this.$slides.push(this.resultSlide.$resultSlide);
    this.on('resize', this.resize, this);

    // Use the correct starting slide
    this.recklessJump(this.currentIndex);
      
    if (this.options.choices.length === this.currentIndex) {
      // Make sure results slide is displayed
      this.resultSlide.$resultSlide.addClass('h5p-sc-current-slide');
      this.setScore(this.results.corrects, true, 0);

      
    }
    setTimeout(function () {
      SoundEffects.setup();
    },1);

    SoundEffects.muted = (this.options.behaviour.soundEffectsEnabled === false);

    var hideButtons = [];

    /**
     * Keeps track of buttons that will be hidden
     * @type {Array}
     */
    self.buttonsToBeHidden = [];

    /**
     * Override Question's hideButton function
     * to be able to hide buttons after delay
     *
     * @override
     * @param {string} id
     */
    this.superHideButton = self.hideButton;
    this.hideButton = (function () {
      return function (id) {

        if (!self.scoreTimeout) {
          return self.superHideButton(id);
        }

        self.buttonsToBeHidden.push(id);
        return this;
      };
    })();
  }

  SingleChoiceSet.prototype = Object.create(Question.prototype);
  SingleChoiceSet.prototype.constructor = SingleChoiceSet;

  /**
   * Handler invoked when question is done
   *
   * @param  {object} data An object containing a single boolean property: "correct".
   */
  SingleChoiceSet.prototype.handleQuestionFinished = function (data) {
    var self = this;
    self.triggerXAPI('interacted');
    if (data.correct) {
      self.results.corrects++;
    }
    else {
      self.results.wrongs++;
    }

    if (self.currentIndex+1 >= self.options.choices.length) {
      self.setScore(self.results.corrects);
    }

    var letsMove = function () {
      // Handle impatient users
      self.$container.off('click.impatient keydown.impatient');
      clearTimeout(timeout);
      self.move(self.currentIndex+1);
    };

    var timeout = setTimeout(function () {
      letsMove();
    }, data.correct ? self.options.behaviour.timeoutCorrect : self.options.behaviour.timeoutWrong);

    self.$container.on('click.impatient', function () {
      letsMove();
    });
    self.$container.on('keydown.impatient', function (event) {
      // If return, space or right arrow
      if(event.which === 13 || event.which === 32 || event.which === 39) {
        letsMove();
      }
    });
  };

  /**
   * Handles buttons that are queued for hiding
   */
  SingleChoiceSet.prototype.handleQueuedButtonChanges = function () {
    var self = this;

    if (self.buttonsToBeHidden.length) {
      self.buttonsToBeHidden.forEach(function (id) {
        self.superHideButton(id);
      });
    }
    self.buttonsToBeHidden = [];
  };

  /**
   * Set score and feedback
   *
   * @params {Number} score Number of correct answers
   */
  SingleChoiceSet.prototype.setScore = function (score, noXAPI, timeout) {
    var self = this;

    // Find last selected alternative, and determine timeout before solution slide shows
    if (!self.choices.length) {
      return;
    }
    var lastSelected = self.choices[self.choices.length - 1]
      .$choice
      .find('.h5p-sc-alternative.h5p-sc-selected');
    var timeout = (timeout !== undefined) ? timeout : (lastSelected.is('.h5p-sc-is-correct') ?
      this.options.behaviour.timeoutCorrect :
      this.options.behaviour.timeoutWrong);

    /**
     * Show feedback and buttons on result slide
     */
    var showFeedback = function () {
        
      self.setFeedback(self.l10n.resultSlideTitle
          .replace(':numcorrect', score)
          .replace(':maxscore', self.options.choices.length),
        score, self.options.choices.length);
      if (score === self.options.choices.length) {
        self.hideButton('try-again');
        self.hideButton('show-solution');
      } else {
        self.showButton('try-again');
        self.showButton('show-solution');
      }
      self.handleQueuedButtonChanges();
      self.scoreTimeout = undefined;
      if (!noXAPI) {
        self.triggerXAPIScored(score, self.options.choices.length, 'completed');
      }

      self.trigger('resize');
      //h5pcustomize
      $(".h5p-question-content").css("height","200px");
      $(".h5p-sc-set-progress").css("display","none");
    };

    /**
     * Wait for result slide animation
     */
    self.scoreTimeout = setTimeout(function () {
      showFeedback();
    }, (timeout));

    /**
     * Listen for impatient clicks.
     * On impatient clicks clear timeout and immediately show feedback.
     */
    self.$container.on('click.impatient', function () {
      clearTimeout(self.scoreTimeout);
      showFeedback();
    });
  };

  /**
   * Handler invoked when view solution is selected
   */
  SingleChoiceSet.prototype.handleViewSolution = function () {
    this.solutionView.show();
  };

  /**
   * Register DOM elements before they are attached.
   * Called from H5P.Question.
   */
  SingleChoiceSet.prototype.registerDomElements = function () {
    // Register task content area.
    this.setContent(this.createQuestion());

    // Register buttons with question.
    this.addButtons();

    // Insert feedback and buttons section on the result slide
    this.insertSectionAtElement('feedback', this.resultSlide.$feedbackContainer);
    this.insertSectionAtElement('buttons', this.resultSlide.$buttonContainer);

    // Question is finished
    if (this.options.choices.length === this.currentIndex) {
      this.trigger('question-finished');
    }
  };

  /**
   * Add Buttons to question.
   */
  SingleChoiceSet.prototype.addButtons = function () {
    var self = this;
    if (this.options.behaviour.enableRetry) {
      this.addButton('try-again', this.l10n.retryButtonLabel, function () {
        //h5pcustomize - display progress  when click on try again
        $(".h5p-sc-set-progress").css("display","block");
        self.resetTask();
        
      }, self.results.corrects !== self.options.choices.length);
    }

    if (this.options.behaviour.enableSolutionsButton) {
      this.addButton('show-solution', this.l10n.showSolutionButtonLabel, function () {
        self.showSolutions();
      }, self.results.corrects !== self.options.choices.length);
    }
  };

  /**
   * Create main content
   */
  SingleChoiceSet.prototype.createQuestion = function () {
    var self = this;

    self.$container.append(self.$choices);
    self.$container.append(self.$progressbar);

    if (self.options.behaviour.soundEffectsEnabled) {
      self.$container.append($('<div>', {
        'class': 'h5p-sc-sound-control',
        'click': function () {
          SoundEffects.muted = !SoundEffects.muted;
          $(this).toggleClass('muted', SoundEffects.muted);
        }
      }));
    }

    // Append solution view - hidden by default:
    self.solutionView.appendTo(self.$container);

    self.resize();

    // Hide all other slides than the current one:
    self.$container.addClass('initialized');

    return self.$container;
  };

  /**
   * Resize if something outside resizes
   */
  SingleChoiceSet.prototype.resize = function () {
    var self = this;
    var maxHeight = 0;
    self.choices.forEach(function (choice) {
      var choiceHeight = choice.$choice.outerHeight();
      maxHeight = choiceHeight > maxHeight ? choiceHeight : maxHeight;
    });

    // Set minimum height for choices
    self.$choices.css({minHeight: maxHeight + 'px'});
  };

  /**
   * Will jump to the given slide without any though to animations,
   * current slide etc.
   *
   * @public
   */
  SingleChoiceSet.prototype.recklessJump = function (index) {
    var tX = 'translateX('+(-index*100)+'%)';
    this.$choices.css({'-webkit-transform':tX,'-moz-transform':tX,'-ms-transform':tX,'transform':tX});
    this.$progressCompleted.css({width:((index+1)/(this.options.choices.length+1))*100 + '%'});
  };

  /**
   * Move to slide n
   * @param  {number} index The slide number    to move to
   */
  SingleChoiceSet.prototype.move = function (index) {
    if (index === this.currentIndex) {
      return;
    }

    var $previousSlide = this.$slides[this.currentIndex];

    H5P.Transition.onTransitionEnd(this.$choices, function () {
      $previousSlide.removeClass('h5p-sc-current-slide');
    }, 600);

    this.$slides[index].addClass('h5p-sc-current-slide');
    this.recklessJump(index);

    this.currentIndex = index;
  };

  /**
   * The following functions implements the CP and IV - Contracts v 1.0 documented here:
   * http://h5p.org/node/1009
   */
  SingleChoiceSet.prototype.getScore = function () {
    return this.results.corrects;
  };
  SingleChoiceSet.prototype.getMaxScore = function () {
   return this.options.choices.length;
  };
  SingleChoiceSet.prototype.getAnswerGiven = function () {
    return (this.results.corrects + this.results.wrongs) > 0;
  };
  SingleChoiceSet.prototype.getTitle = function() {
    return H5P.createTitle(this.options.choices[0].question);
  };
  SingleChoiceSet.prototype.showSolutions = function () {
    this.handleViewSolution();
  };
  /**
   * Reset all answers. This is equal to refreshing the quiz
   */
  SingleChoiceSet.prototype.resetTask = function () {
    var self = this;

    // Close solution view if visible:
    this.solutionView.hide();

    // Reset the user's answers
    var classes = ['h5p-sc-reveal-wrong', 'h5p-sc-reveal-correct', 'h5p-sc-selected', 'h5p-sc-drummed', 'h5p-sc-correct-answer'];
    for (var i = 0; i < classes.length; i++) {
      this.$choices.find('.' + classes[i]).removeClass(classes[i]);
    }
    this.results = {
      corrects: 0,
      wrongs: 0
    };

    this.move(0);

    // Wait for transition, then remove feedback.
    H5P.Transition.onTransitionEnd(this.$choices, function () {
      self.setFeedback();
    }, 600);
  };

  /**
   * Clever comment.
   *
   * @public
   * @returns {object}
   */
  SingleChoiceSet.prototype.getCurrentState = function () {
    return {
      progress: this.currentIndex,
      answers: this.results
    };
  };

  return SingleChoiceSet;

})(H5P.jQuery, H5P.Question, H5P.SingleChoiceSet.SingleChoice, H5P.SingleChoiceSet.SolutionView, H5P.SingleChoiceSet.ResultSlide, H5P.SingleChoiceSet.SoundEffects);
;var H5P = H5P || {};

/**
 * Constructor.
 *
 * @param {Object} params Options for this library.
 * @param {Number} id Content identifier
 * @returns {undefined}
 */
(function ($) {
  H5P.Image = function (params, id) {
    H5P.EventDispatcher.call(this);
    if (params.file === undefined || !(params.file instanceof Object)) {
      this.source = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAARgAAACaCAIAAAD948C8AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH3QQQCBoH57dqzgAADlJJREFUeNrtnVtsHNd5x89lZmdnr7zrSlkKA6Vx3DZuDVRF0rzUKGAEaYMESIIgAfIQJOlrgfSlL3npU4A+NwkKF66LtkGQpoqNtIXbpLFjS45sSRRlWRQlilySu9z7ZXYuO3PO6cOSFEVSFHfI5W3+P62I5XLnDOc78+N3zpkze+g/vP6AhEG9cOr7vzf29+RYMFn8zrX8dwmhBIBQaGE3pNfyf93wJv5k/LtHPQRv5r5/r/pFnApgN7DdbHyv+sXL05eP9PFfnr4Mi8ABi0QIKTvPvTp182HjpSN35A8bL706dbPsPIeTAOweGraPtJFzmTdevPCto3LYb8z+cL75IqofHJaMtMZ888VXp27eKX/9kB/wnfLXX526CYvAIc1Ia2SNB586+zcnU1cP26EWrD/6zcLfNryPoNbBERCpy5D5wR+f+d6J5LXDcJDL7RfeWfxe1XkW9Q2OmEir2Wn2d8d+eHHo3w7q8KarX75V/FbDu4CaBkdYpC46t85l/ucPTv5dOpbbn6NqdcbfL/zVfPNPfZFCHYNjItIaplY6m/n1syP/OGze7kf5FecTH5S/sdD8jBOMomrBsRVpDY05WeP+qdSV8wO/GEtc301RRfv5h/WX8talhjcRSBM1CiIk0mO/AZEx3kzoxXRsPmM8zBhz6dh8Ui/EtXKMW5QKpXhHpNxgpO2fbHXONb1nmt75Vuec7Y91REbt3Qg+AOFzw4H/BoowTwx4YqDmXkR9gCMK/pwDAJEAgEgAQCQAAEQCACIBAJEAgEgAAIgEAEQC4PCiIQTgkCC8YJjJL/zFx9Ze+el/3K1Ixg0NIgHwdKSQn790amQ0seH1rlTlkv2zK3nGD3XrCU07cMB4Deebn5vYbNEaI6OJb35uwms4EAmArbGLrW9/6eNqB3znKx+3iy2IBMBGnEr721/9hBAy8Dc+pFCbH3/5teecSvtwHsvB39gHoolvd772Z+fp43/JKX36Ogav/GJWT8QgEgBESfXnL4wSSQgh613aINJmr5RSlNF/v7pM2eFaOgSjduAA+PRHM/VSu6vKmizdZxsMWe+SUqr75DMXs2/ONCESiDRuzS7P+SvarIlEHxm0zq3NuWytECc+mIBIILqclv5yzu7qQhkl3X+EULbyZL1CW2Ykosj5uF5ARgKRpTVf8wq1bvah9JEnlHb/k3XiPKkXtKKTPdhJnRuESCCKYwytyRxdJwylK0lo7SW640EEVbCSZwcOyagDRAL7h71Qaze9tWTzKB1tlYGeNBT+qIFHSGqxlhgfgkggSulIqdk37m6fcCjtLb3MvnH32W9c6nUriASOMNZCXQTy6ef8zqVQK8Wmxw++p4QpQmCfWHz7ASE7mFQnd/xQSim1+PahmFGAjAQIIaQ5Vy3eXLSLrcRYeuz3z2Se2eOOh9dw3Hpfpm+7dcdrOEbWhEjgIBFecP/1KbtkrTTA8g0r30iMpiY++9we3lFXnsr37xDKU/kznzrgFU3RtIs0gRtMvXJ1zaI17JI19crVwA32ZC9SyNLUUv+OojS1JIWESOBgUFLN/HxSSRXipz3RLvR9Xtw+7AIiga0p3lhwq/Z23Y+qXbyxsAcZ4+Ziv49lH3YBkcDWvf/8b+ee+rb8b+d2eY+39EUzV+v34TRzNekLiAT2m/lfTu/5O7dudC3v0/3h+7ajLcGoXRSxi62dn3bt5VZ3WDzcvip39mmW9ux/39mclLjOWUzTkzEjE48PJuLDSXMwoaVijDGIBHbLwlv3e33/xS98MsSOlFT1B+X9Oagtm3bCF8IXftvb/MEp5kgye24o88xQfCS5e68gUhR7R5vHu5+SwUpWuIuebs0+tHFwym2n3C68nyOEaKY+MDE6/Dtj5nAKIoEdUbq1FG6rs5+e6HkMYL52JGISOH55aqk8tUQISY8Pjn3ybPp0FiKBJ7d/Alm+HWaSQfl2/vSlC0zrrQlU7ud12D7RytVauRohZOCjoyf/cDw+kIBIYCPWUn0322bO9TAHT3SEb3eObqzqM6X6TEkz9dOXLgxdHNv+zRj+jhbFG4v7tu1h7iD11Oqb/+X0jR+8Vbg2J6WESIBIX1j5RviMlG/0dNGztVA7TtErvJeb/NHb+XcfbqkTRIoQu79k2VMJteni8Yvh8vWFyR+9XZpcgkjRpXp3ed9KUEJ6Tfe4RnLxnQc3fvBWa6EOkSKHkqo2U9plIbWZ0g7ng3cs79iH9P7rUzOXJ0UngEgRotPy9rMcp9yOQlStfPPWy1cqd5YhUlSw8vX9LKe1WI9ObHO/vgeRokJturSf5dT3aHdHBYgUlQ7Sbga+H89Ijad2k/LvzgkhIhVhzGyIBHs7w8C3O7GU8cQu+GtTkWrXQaQIsbddf6fc3lIk0Qk+/PH7frsTwQijaReNkYY9TRFbltax3FsvX4mmRRApKuzt3XWbS3Nr9gf/fC3KEUbTLhIjDXveR1JSra2nYpes6Z/eiHiQkZGOP4HT6V+ZTqUNiyBSJPAabp/K9BrO3Z9cR4QhUiRwylY/ygxc/86/vofwoo8UFaw+fJyvlW8W3sshthApSiL14fJoY65KlEJs0bSLCkoq0enDbB1YhIwUIYuO9d11EAn0K/kEju81HLvUspYaR+Uz5SASOHB1iO903KptLdXrDyq7XDYCQKQouaOUb3lWvll/UG7OVREQiAR6IHD8dqFZnS42HlYQDYgEeks+Xt1pzFaWbywc7CpaACIdSX/cql2dLpYmFxENiAR6xms41bvF5euYMQCRQO8IL2jMVhbfedCXK6cAIh17nEp7+Xqufr+MUEAk0HsvSKrmfDX3fzOB6yMaEAn0jPRFdbrY6xKuACKB1Y5QJyh/UMhffYhQQCQQKgsFony7sHRlFqGASCBkX6h2rzj/q3sIBUQCIbGWGjOv3SK4bQcigXD4bW/+V/fWr0IFIBLopS2nVOV2YeE3GJSDSCAsXt2Zee1WZD+zFyKBPaB8O4+rQxAJhCdw/dn/vNNebiIUEAmEpL3cvPezScQBQCQ05wBEOiCUVLk3Z6ofLiMUACKFRPpi5ue37JKFUACIFJLA9e/8y3uiEyAUACKFxLc7t//pXcQBbAaf/Q2LwF5kpP9q/kSQIFCBUEFAAqECSYRQQpBAKimIECpQREkipRKKSEWUIkoppTbNx6SEUkop6T4Yo5wRRgnlVOOEM8o40TjljHBONY1onGoa1TjRNKpzyjWqa0TXqMap1n2iUX31nbpGOSfaalG8+5URxgijlHV32tcWHSwC24k05e7ZGrqKPC7XQUx8TrCUyRImTSVYMs4SJk3EmWlQ02Bxg8Zj1NCpoVNdo3rXYUY5I5RsK6EMxIc/fh/nCohQH8mWli0tQoq9bphimTQfSLNsimUSPJVYVTFG4oX/fegKm3HOJKOK4qQBGGx4IpZsWrKZ3/JnzxPy/KPveKAlrKTZTsRtM+6aMceIeYbeiem+zgPOBGcSPU+IBJ6G0ILWQKM10NjmPUywZCudbKW6vhmuGXMN3dc1X+eCU4m0BpHADpBcbi+b4cRTjUyylUq0k3HbNNy43olpvsYER+sRIoGd4pmuZ7qVk1v05aiimdpAVzOznYg7pu4Zmq+hhwaRQA8oqhpDtcbQFuvtxTwjWx1ccyzmGnonBsEgEuiNjuGVThVKpwobG4quka0MpevZZCtl2omYG+9mMEQMIoFeGopxr3gmXzyzcdBxsDycqQ6kG9mElTScuObrsAsigZ6pjVRqIxtX+Bsqjg5UBlONTMJKGm6cBxpahhAJ9Ex1rFQdK61/xbQTg8WRbHUw1UzHbVP3Yxigh0igZ5yE7ZyfXzo/v/aK3tGHl8cGKkOpRsZsJ6AWRAJh8GN+YXyxMP5o7U3DNUYKJ7pqxW1T83VECSKBnvHi3uL5+cV1WStbHRwujGWrg0krFfOMiKcsiARCsuFiV9wxR5dODpaHU42M4caZYBAJgJ5xTSc3MZubWFm9RvP1Ewunh0oj6XrGcM1j7xVEAn0h0P3FC3OLF+a638Y840Tu9FBppJuvjt+AO3325S+h1kGfzzJFKCFEEaooU4SqpJUcLA+nrFSso1OqhB5IPZC6L7RA6oHQfakHUguE7ktNSC1YfQjFpeSB4lIyqZhUXCiqFFOEKkXV4/ukRFEqKVWUCk4lY5JRwZjQqGAs4CzQVh+c+zoLNOZr3NeZr/FAY90nvsa6Pwo4DzQaaExwFnAqGJOcSkpW52chI4EQVqwqwSRhknBBuSRcUE0Qrfs1IJqg2hM/a8kjpNDnX7PrleLd7/q+6DVEAo9LwhRlkqyIERBNUD0guk9jPo35hGLhNIgEVlSRlMuVjBHrGtKhRodwifBAJLApsXBBNEF1nxo+MTxqetu0tQBEijYr6SWgMZ/GPWp61HTRAINIYLs8QzVB9IAaHRp3acKlBlYHhEjgqalGE8To0LjHEg5NOggJRAI7yzaxDjU9lrJpAtpAJLDDhKMHNO7RpMPSbcIFQgKRwM5yTteclM2yLYwHQCSwY7ig8Q5L2jTbojqGniES2Hni0QNqujTdZtkWogGRQE9RDFjCpRmLZbAeJkQCvY4WxD2WsdhQA8EAEKn3llvKZkN1XAwFEKl3f4wOy1hstIpQAIgUyp+BJhuuIxQAIoVov/l0oMWRfwBECjd+wNJtdqKC2w0ARArbhBuus4EmQgEgUiiFkjY/VaIxH6cCgEghBFIs3eZnCzgDAEQKq1DG4meWUfcAIoWVKGVr55ZQ6wAihVUo5vOzBRr3UOUAIoVtyw3X+VgFlQ0gUvhEpJ1fIBruPwUQKSws3ebjedQxgEi7sGi4xk+gOQcg0i7gYxU2UkPtgv37w30MD2mkBosARNp1vwgDdAAi7a6hKjC6ACDSrrtGJ8qoUXAg/D+bbRxow/jhAQAAAABJRU5ErkJggg==';
    }
    else {
		//h5pcustomize
		
		if($(".h5p-interactivevideo-editor").size() == 0 || $("#h5p-image-radio-button-1:checked").size() > 0)
    		if(params.file != null)
			{
				if(params.file.path.indexOf("^^^") > 0)
					params.file.path =params.file.path.split("^^^")[1];
			}
		
    	
      this.source = H5P.getPath(params.file.path, id);
      this.width = params.file.width;
      this.height = params.file.height;

      // Use new copyright information if available. Fallback to old.
      if (params.file.copyright !== undefined) {
        this.copyright = params.file.copyright;
      }
      else if (params.copyright !== undefined) {
        this.copyright = params.copyright;
      }
    }

    this.alt = params.alt !== undefined ? params.alt : 'New image';

    if (params.title !== undefined) {
      this.title = params.title;
    }
  };

  H5P.Image.prototype = Object.create(H5P.EventDispatcher.prototype);
  H5P.Image.prototype.constructor = H5P.Image;

  /**
   * Wipe out the content of the wrapper and put our HTML in it.
   *
   * @param {jQuery} $wrapper
   * @returns {undefined}
   */
  H5P.Image.prototype.attach = function ($wrapper) {
    var self = this;

    if (self.$img === undefined) {
      self.$img = $('<img>', {
        width: '100%',
        height: '100%',
        src: this.source,
        alt: this.alt,
        title: this.title === undefined ? '' : this.title,
        load: function () {
          self.trigger('loaded');
        }
      });
    }

    $wrapper.addClass('h5p-image').html(self.$img);
  };

  /**
   * Gather copyright information for the current content.
   *
   * @returns {H5P.ContentCopyright}
   */
  H5P.Image.prototype.getCopyrights = function () {
    if (this.copyright === undefined) {
      return;
    }

    var info = new H5P.ContentCopyrights();

    var image = new H5P.MediaCopyright(this.copyright);
    image.setThumbnail(new H5P.Thumbnail(this.source, this.width, this.height));
    info.addMedia(image);

    return info;
  };

  return H5P.Image;
}(H5P.jQuery));
;/*global H5P*/
H5P.Blanks = (function ($, Question) {
  /**
   * @constant
   * @default
   */
  var STATE_ONGOING = 'ongoing';
  var STATE_CHECKING = 'checking';
  var STATE_SHOWING_SOLUTION = 'showing-solution';
  var STATE_FINISHED = 'finished';

  /**
   * Initialize module.
   *
   * @class H5P.Blanks
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {number} id Content identification
   * @param {Object} contentData Task specific content data
   */
  function Blanks(params, id, contentData) {
    var self = this;

    // Inheritance
    Question.call(self, 'blanks');

    // IDs
    this.contentId = id;

//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
    // Set default behavior.
    this.params = $.extend(true, {}, {
      text: "Fill in",
      questions: [
        ""
      ],
      userAnswers: [],
      score: "You got @score of @total points.",
      showSolutions: "Show solutions",
      tryAgain: "Try again",
      checkAnswer: "Check",
      changeAnswer: "Change answer",
      notFilledOut: "Please fill in all blanks",
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol,
        caseSensitive: false,
        showSolutionsRequiresInput: true,
        autoCheck: false,
        separateLines: false,
        disableImageZooming: false
      }
    }, params);

    // Delete empty questions
    for (var i = this.params.questions.length - 1; i >= 0; i--) {
      if (!this.params.questions[i]) {
        this.params.questions.splice(i, 1);
      }
    }

    // Previous state
    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined) {
      this.previousState = this.contentData.previousState;
    }

    // Clozes
    this.clozes = [];

    // Keep track tabbing forward or backwards
    this.shiftPressed = false;

    H5P.$body.keydown(function (event) {
      if (event.keyCode === 16) {
        self.shiftPressed = true;
      }
    }).keyup(function (event) {
      if (event.keyCode === 16) {
        self.shiftPressed = false;
      }
    });
  }

  // Inheritance
  Blanks.prototype = Object.create(Question.prototype);
  Blanks.prototype.constructor = Blanks;

  /**
   * Registers this question type's DOM elements before they are attached.
   * Called from H5P.Question.
   */
  Blanks.prototype.registerDomElements = function () {
    var self = this;

    if (self.params.image) {
      // Register task image
      self.setImage(self.params.image.path, {disableImageZooming: self.params.behaviour.disableImageZooming});
    }

    // Register task introduction text
    self.setIntroduction(self.params.text);

    // Register task content area
    self.setContent(self.createQuestions(), {
      'class': self.params.behaviour.separateLines ? 'h5p-separate-lines' : ''
    });

    // ... and buttons
    self.registerButtons();

    // Restore previous state
    self.setH5PUserState();
  };

  /**
   * Create all the buttons for the task
   */
  Blanks.prototype.registerButtons = function () {
    var self = this;

    if (!self.params.behaviour.autoCheck) {
      // Check answer button
      self.addButton('check-answer', self.params.checkAnswer, function () {
        self.toggleButtonVisibility(STATE_CHECKING);
        self.markResults();
        self.showEvaluation();
        self.triggerAnswered();
      });
    }

    // Check answer button
    self.addButton('show-solution', self.params.showSolutions, function () {
      if (self.allBlanksFilledOut()) {
        self.toggleButtonVisibility(STATE_SHOWING_SOLUTION);
        self.showCorrectAnswers();
      }
    }, self.params.behaviour.enableSolutionsButton);

    // Try again button
    if (self.params.behaviour.enableRetry === true) {
      self.addButton('try-again', self.params.tryAgain, function () {
        self.resetTask();
        self.$questions.filter(':first').find('input:first').focus();
      });
    }
    self.toggleButtonVisibility(STATE_ONGOING);
  };

  /**
   * Find blanks in a string and run a handler on those blanks
   *
   * @param {string} question
   *   Question text containing blanks enclosed in asterisks.
   * @param {function} handler
   *   Replaces the blanks text with an input field.
   * @returns {string}
   *   The question with blanks replaced by the given handler.
   */
  Blanks.prototype.handleBlanks = function (question, handler) {
    // Go through the text and run handler on all asterix
    var clozeEnd, clozeStart = question.indexOf('*');
    var self = this;
    while (clozeStart !== -1 && clozeEnd !== -1) {
      clozeStart++;
      clozeEnd = question.indexOf('*', clozeStart);
      if (clozeEnd === -1) {
        continue; // No end
      }
      var replacer = handler(self.parseSolution(question.substring(clozeStart, clozeEnd)));
      clozeEnd++;
      question = question.slice(0, clozeStart - 1) + replacer + question.slice(clozeEnd);
      clozeEnd -= clozeEnd - clozeStart - replacer.length;

      // Find the next cloze
      clozeStart = question.indexOf('*', clozeEnd);
    }
    return question;
  };

  /**
   * Create questitons html for DOM
   */
  Blanks.prototype.createQuestions = function () {
    var self = this;

    var html = '';
    for (var i = 0; i < self.params.questions.length; i++) {
      var question = self.params.questions[i];

      // Go through the question text and replace all the asterisks with input fields
      question = self.handleBlanks(question, function(solution) {
        // Create new cloze
        var defaultUserAnswer = (self.params.userAnswers.length > self.clozes.length ? self.params.userAnswers[self.clozes.length] : null);
        var cloze = new Blanks.Cloze(solution, self.params.behaviour, defaultUserAnswer);

        self.clozes.push(cloze);
        return cloze;
      });

      html += '<div>' + question + '</div>';
    }

    this.$questions = $(html);

    // Set input fields.
    this.$questions.find('input').each(function (i) {
      var afterCheck;
      if (self.params.behaviour.autoCheck) {
        afterCheck = function () {
          if (self.done || self.getAnswerGiven()) {
            // All answers has been given. Show solutions button.
            self.toggleButtonVisibility(STATE_CHECKING);
            self.showEvaluation();
            self.done = true;
            self.triggerAnswered();
          }
        };
      }
      self.clozes[i].setInput($(this), afterCheck, function () {
        self.toggleButtonVisibility(STATE_ONGOING);
        if (!self.params.behaviour.autoCheck) {
          self.hideEvaluation();
        }
      });
    }).keydown(function (event) {
      self.autoGrowTextField($(this));

      if (event.keyCode === 13) {
        return false; // Prevent form submission on enter key
      }

      // Refocus buttons after they have been toggled if last input
      if (event.keyCode === 9 && self.params.behaviour.autoCheck) {
        var $inputs = self.$questions.find('.h5p-input-wrapper:not(.h5p-correct) .h5p-text-input');
        var $lastInput = $inputs[$inputs.length - 1];
        if ($(this).is($lastInput) && !self.shiftPressed) {
          setTimeout(function () {
            self.focusButton();
          }, 10);
        }
      }
    }).on('change', function () {
      self.triggerXAPI('interacted');
    });

    self.on('resize', function () {
      self.resetGrowTextField();
    });

    return this.$questions;
  };

  /**
   *
   */
  Blanks.prototype.autoGrowTextField = function ($input) {
    // Do not set text field size when separate lines is enabled
    if (this.params.behaviour.separateLines) {
      return;
    }

    var self = this;
    var fontSize = parseInt($input.css('font-size'), 10);
    var minEm = 3;
    var minPx = fontSize * minEm;
    var rightPadEm = 3.25;
    var rightPadPx = fontSize * rightPadEm;
    var static_min_pad = 0.5 * fontSize;

    setTimeout(function(){
      var tmp = $('<div>', {
        'html': $input.val()
      });
      tmp.css({
        'position': 'absolute',
        'white-space': 'nowrap',
        'font-size': $input.css('font-size'),
        'font-family': $input.css('font-family'),
        'padding': $input.css('padding'),
        'width': 'initial'
      });
      $input.parent().append(tmp);
      var width = tmp.width();
      var parentWidth = self.$questions.width();
      tmp.remove();
  /*    if (width <= minPx) {       // commented for removing the inline width for textbox
        // Apply min width
        $input.width(minPx + static_min_pad);
      } else if (width + rightPadPx >= parentWidth) {

        // Apply max width of parent
        $input.width(parentWidth - rightPadPx);
      } else {

        // Apply width that wraps input
        $input.width(width + static_min_pad);
      } */

    }, 1);
  };

  /**
   * Resize all text field growth to current size.
   */
  Blanks.prototype.resetGrowTextField = function () {
    var self = this;

    this.$questions.find('input').each(function () {
      self.autoGrowTextField($(this));
    });
  };

  /**
   * Toggle buttons dependent of state.
   *
   * Using CSS-rules to conditionally show/hide using the data-attribute [data-state]
   */
  Blanks.prototype.toggleButtonVisibility = function (state) {
    // The show solutions button is hidden if all answers are correct
    var allCorrect = (this.getScore() === this.getMaxScore());
    if (this.params.behaviour.autoCheck && allCorrect) {
      // We are viewing the solutions
      state = STATE_FINISHED;
    }

    if (this.params.behaviour.enableSolutionsButton) {
      if (state === STATE_CHECKING && !allCorrect) {
        this.showButton('show-solution');
      }
      else {
        this.hideButton('show-solution');
      }
    }

    if (this.params.behaviour.enableRetry) {
      if ((state === STATE_CHECKING && !allCorrect) || state === STATE_SHOWING_SOLUTION) {
        this.showButton('try-again');
      }
      else {
        this.hideButton('try-again');
      }
    }

    if (state === STATE_ONGOING) {
      this.showButton('check-answer');
    }
    else {
      this.hideButton('check-answer');
    }

    this.trigger('resize');
  };

  /**
   * Check if all blanks are filled out. Warn user if not
   */
  Blanks.prototype.allBlanksFilledOut = function () {
    var self = this;

    if (!self.getAnswerGiven()) {
      this.updateFeedbackContent(self.params.notFilledOut);
      return false;
    }

    return true;
  };

  /**
   * Mark which answers are correct and which are wrong and disable fields if retry is off.
   */
  Blanks.prototype.markResults = function () {
    var self = this;
    for (var i = 0; i < self.clozes.length; i++) {
      self.clozes[i].checkAnswer();
      if (!self.params.behaviour.enableRetry) {
        self.clozes[i].disableInput();
      }
    }
    this.trigger('resize');
  };

  /**
   * Removed marked results
   */
  Blanks.prototype.removeMarkedResults = function () {
    this.$questions.find('.h5p-input-wrapper').removeClass('h5p-correct h5p-wrong');
    this.$questions.find('.h5p-input-wrapper > input').attr('disabled', false);
    this.trigger('resize');
  };


  /**
   * Displays the correct answers
   */
  Blanks.prototype.showCorrectAnswers = function () {
    var self = this;
    this.hideSolutions();

    for (var i = 0; i < self.clozes.length; i++) {
      self.clozes[i].showSolution();
    }
    this.trigger('resize');
  };

  /**
   * Display the correct solution for the input boxes.
   *
   * This is invoked from CP - be carefull!
   */
  Blanks.prototype.showSolutions = function () {
    this.params.behaviour.enableSolutionsButton = true;
    this.toggleButtonVisibility(STATE_FINISHED);
    this.markResults();
    this.showCorrectAnswers();
    this.showEvaluation();
    //Hides all buttons in "show solution" mode.
    this.hideButtons();
  };

  /**
   * Resets the complete task.
   * Used in contracts.
   * @public
   */
  Blanks.prototype.resetTask = function () {
    this.hideEvaluation();
    this.hideSolutions();
    this.clearAnswers();
    this.removeMarkedResults();
    this.toggleButtonVisibility(STATE_ONGOING);
    this.resetGrowTextField();
    this.done = false;
  };

  /**
   * Hides all buttons.
   * @public
   */
  Blanks.prototype.hideButtons = function () {
    this.toggleButtonVisibility(STATE_FINISHED);
  };

  /**
   * Trigger xAPI answered event
   */
  Blanks.prototype.triggerAnswered = function() {
    var xAPIEvent = this.createXAPIEventTemplate('answered');
    this.addQuestionToXAPI(xAPIEvent);
    this.addResponseToXAPI(xAPIEvent);
    this.trigger(xAPIEvent);
  };

  /**
   * Add the question itselt to the definition part of an xAPIEvent
   */
  Blanks.prototype.addQuestionToXAPI = function(xAPIEvent) {
    var definition = xAPIEvent.getVerifiedStatementValue(['object', 'definition']);
    definition.description = {
      'en-US': this.params.text
    };
    definition.type = 'http://adlnet.gov/expapi/activities/cmi.interaction';
    definition.interactionType = 'fill-in';
    definition.correctResponsesPattern = ['{case_matters=' + this.params.behaviour.caseSensitive + '}'];
    var firstCorrectResponse = true;
    // xAPI forces us to create solution patterns for all possible solution combinations
    for (var i = 0; i < this.params.questions.length; i++) {
      var question = this.handleBlanks(this.params.questions[i], function(solution) {
        // Store new patterns for each extra alternative answer
        var newPatterns = [];
        for (var j = 0; j < definition.correctResponsesPattern.length; j++) {
          if (!firstCorrectResponse) {
            definition.correctResponsesPattern[j] += '[,]';
          }
          var prefix = definition.correctResponsesPattern[j];
          for (var k = 0; k < solution.solutions.length; k++) {
            if (k === 0) {
              // This is the first possible answr, just add it to the pattern
              definition.correctResponsesPattern[j] += solution.solutions[k];
            }
            else {
              // This is an alternative possible answer, we need to create a new permutation
              newPatterns.push(prefix + solution.solutions[k])
            }
          }
        }
        // Add any new permutations to the list of response patterns
        definition.correctResponsesPattern = definition.correctResponsesPattern.concat(newPatterns);
        
        firstCorrectResponse = false;
        
        // We replace the solutions in the question with a "blank"
        return '__________';
      });
      definition.description['en-US'] += question;
    }
  };
  
  /**
   * Parse the solution text (text between the asterix)
   * 
   * @param {string} solutionText
   * @returns {object} with the following properties
   *  - tip: the tip text for this solution, undefined if no tip
   *  - solutions: array of solution words
   */
  Blanks.prototype.parseSolution = function (solutionText) {
  
    //h5pcustomize - #73817
    this.params.behaviour.caseSensitive = false;
    var solutions = [];
    var tip;

    var solutionsAndTip = solutionText.split(':');
    
    if (solutionsAndTip.length > 0) {
      solutions = solutionsAndTip[0].split('/');
    }
    
    if (solutionsAndTip.length === 2) {
      tip = solutionsAndTip[1];
    }
    // Trim solutions
    for (var i = 0; i < solutions.length; i++) {
      solutions[i] = H5P.trim(solutions[i]);
      if (this.params.behaviour.caseSensitive !== true) {
        solutions[i] = solutions[i].toLowerCase();
      }
    }
    
    return {
      tip: tip,
      solutions: solutions
    };
  }

  /**
   * Add the response part to an xAPI event
   *
   * @param {H5P.XAPIEvent} xAPIEvent
   *  The xAPI event we will add a response to
   */
  Blanks.prototype.addResponseToXAPI = function (xAPIEvent) {
    xAPIEvent.setScoredResult(this.getScore(), this.getMaxScore(), this);
    var usersAnswers = this.getCurrentState();

    xAPIEvent.data.statement.result.response = usersAnswers.join('[,]');
  };

  /**
   * Show evaluation widget, i.e: 'You got x of y blanks correct'
   */
  Blanks.prototype.showEvaluation = function () {
    var maxScore = this.getMaxScore();
    var score = this.getScore();
    var scoreText = this.params.score.replace('@score', score).replace('@total', maxScore);
    this.setFeedback(scoreText, score, maxScore);

    if (score === maxScore) {
      this.toggleButtonVisibility(STATE_FINISHED);
    }
  };

  /**
   * Hide the evaluation widget
   */
  Blanks.prototype.hideEvaluation = function () {
    // Clear evaluation section.
    this.setFeedback();
  };

  /**
   * Hide solutions. (/try again)
   */
  Blanks.prototype.hideSolutions = function () {
    // Clean solution from quiz
    this.$questions.find('.h5p-correct-answer').remove();
  };

  /**
   * Get maximum number of correct answers.
   *
   * @returns {Number} Max points
   */
  Blanks.prototype.getMaxScore = function () {
    var self = this;
    return self.clozes.length;
  };

  /**
   * Count the number of correct answers.
   *
   * @returns {Number} Points
   */
  Blanks.prototype.getScore = function () {
    var self = this;
    var correct = 0;
    for (var i = 0; i < self.clozes.length; i++) {
      if (self.clozes[i].correct()) {
        correct++;
      }
      self.params.userAnswers[i] = self.clozes[i].getUserAnswer();
    }

    return correct;
  };

  Blanks.prototype.getTitle = function() {
    return H5P.createTitle(this.params.text);
  };

  /**
   * Clear the user's answers
   */
  Blanks.prototype.clearAnswers = function () {
    this.$questions.find('.h5p-text-input').val('');
  };

  /**
   * Checks if all has been answered.
   *
   * @returns {Boolean}
   */
  Blanks.prototype.getAnswerGiven = function () {
    var self = this;

    if (this.params.behaviour.showSolutionsRequiresInput === true) {
      for (var i = 0; i < self.clozes.length; i++) {
        if (!self.clozes[i].filledOut()) {
          return false;
        }
      }
    }

    return true;
  };

  /**
   * Helps set focus the given input field.
   * @param {jQuery} $input
   */
  Blanks.setFocus = function ($input) {
    setTimeout(function () {
      $input.focus();
    }, 1);
  };

  /**
   * Returns an object containing content of each cloze
   *
   * @returns {object} object containing content for each cloze
   */
  Blanks.prototype.getCurrentState = function () {
    var clozesContent = [];

    // Get user input for every cloze
    this.clozes.forEach(function (cloze) {
      clozesContent.push(cloze.getUserInput());
    });
    return clozesContent;
  };

  /**
   * Sets answers to current user state
   */
  Blanks.prototype.setH5PUserState = function () {
    var self = this;
    var isValidState = (this.previousState !== undefined &&
                        this.previousState.length &&
                        this.previousState.length === this.clozes.length);

    // Check that stored user state is valid
    if (!isValidState) {
      return;
    }

    // Set input from user state
    var hasAllClozesFilled = true;
    this.previousState.forEach(function (clozeContent, ccIndex) {
      var cloze = self.clozes[ccIndex];
      cloze.setUserInput(clozeContent);

      // Handle instant feedback
      if (self.params.behaviour.autoCheck) {
        if (cloze.filledOut()) {
          cloze.checkAnswer();
        } else {
          hasAllClozesFilled = false;
        }
      }
    });

    if (self.params.behaviour.autoCheck && hasAllClozesFilled) {
      self.showEvaluation();
      self.toggleButtonVisibility(STATE_CHECKING);
    }
    //h5pcustomize
    if(self.params.behaviour.enableRetry == false)
    {
     		self.toggleButtonVisibility(STATE_CHECKING);
        	self.markResults();
        	self.showEvaluation();
        	self.triggerAnswered();
    }
        
  };

  /**
   * Disables any active input. Useful for freezing the task and dis-allowing
   * modification of wrong answers.
   */
  Blanks.prototype.disableInput = function () {
    this.$questions.find('input').attr('disabled', true);
  };

  return Blanks;
})(H5P.jQuery, H5P.Question);
;(function ($, Blanks) {

  /**
   * Simple private class for keeping track of clozes.
   *
   * @class H5P.Blanks.Cloze
   * @param {string} answer
   * @param {Object} behaviour Behaviour for the task
   * @param {string} defaultUserAnswer
   */
  Blanks.Cloze = function (solution, behaviour, defaultUserAnswer) {
    var self = this;
    var $input, $wrapper;
    var answers = solution.solutions;
    var answer = answers.join('/');
    var tip = solution.tip;

    /**
     * Check if the answer is correct.
     *
     * @private
     * @param {string} answered
     */
    var correct = function (answered) {
      if (behaviour.caseSensitive !== true) {
        answered = answered.toLowerCase();
      }

      for (var i = 0; i < answers.length; i++) {
        if (answered === answers[i]) {
          return true;
        }
      }
      return false;
    };

    /**
     * Check if filled out.
     *
     * @param {boolean}
     */
    this.filledOut = function () {
      var answered = this.getUserAnswer();
      // Blank can be correct and is interpreted as filled out.
      return (answered !== '' || correct(answered));
    };

    /**
     * Check the cloze and mark it as wrong or correct.
     */
    this.checkAnswer = function () {
      var isCorrect = correct(this.getUserAnswer());
      if (isCorrect) {
        $wrapper.addClass('h5p-correct');
        $input.attr('disabled', true);
      }
      else {
        $wrapper.addClass('h5p-wrong');
      }
    };

    /**
     * Disables further input.
     */
    this.disableInput = function () {
      $input.attr('disabled', true);
    };

    /**
     * Show the correct solution.
     */
    this.showSolution = function () {
      if (correct(this.getUserAnswer())) {
        return; // Only for the wrong ones
      }

      $('<span class="h5p-correct-answer"> ' + answer + '</span>').insertAfter($wrapper);
      $input.attr('disabled', true);
    };

    /**
     * @returns {boolean}
     */
    this.correct = function () {
      return correct(this.getUserAnswer());
    };

    /**
     * Set input element.
     *
     * @param {H5P.jQuery} $element
     * @param {function} afterCheck
     * @param {function} afterFocus
     */
    this.setInput = function ($element, afterCheck, afterFocus) {
      $input = $element;
      $wrapper = $element.parent();

      // Add tip if tip is set
      if(tip !== undefined && tip.trim().length > 0) {
        $wrapper.addClass('has-tip').append(H5P.JoubelUI.createTip(tip, $wrapper.parent()));
      }

      if (afterCheck !== undefined) {
        $input.blur(function () {
          if (self.filledOut()) {
            // Check answers
            if (!behaviour.enableRetry) {
              self.disableInput();
            }
            self.checkAnswer();
            afterCheck();
          }
        });
      }
      $input.focus(function () {
        $wrapper.removeClass('h5p-wrong');
        if (afterFocus !== undefined) {
          afterFocus();
        }
      });
    };

    /**
     * @returns {string} Cloze html
     */
    this.toString = function () {
      var extra = defaultUserAnswer ? ' value="' + defaultUserAnswer + '"' : '';
      return '<span class="h5p-input-wrapper"><input type="text" class="h5p-text-input" autocapitalize="off"' + extra + '></span>';
    };

    /**
     * @returns {string} Trimmed answer
     */
    this.getUserAnswer = function () {
      return H5P.trim($input.val());
    };

    /**
     * @returns {string} Answer
     */
    this.getUserInput = function () {
      return $input.val();
    };

    /**
     * @param {string} text New input text
     */
    this.setUserInput = function (text) {
      $input.val(text);
    };
  };

})(H5P.jQuery, H5P.Blanks);
;/*global H5P*/
var H5PEditor = H5PEditor || {};

/**
 * Interactive Video editor widget module
 * TODO: Rewrite to use H5P.DragQuestion for previewing?

 * @param {jQuery} $
 */
H5PEditor.widgets.dragQuestion = H5PEditor.DragQuestion = (function ($, DragNBar) {
  /**
   * Must be changed if the semantics for the elements changes.
   * @πvate
   * @type {string}
   */
  var clipboardKey = 'H5PEditor.DragQuestion';

  /**
   * Initialize interactive video editor.
   *
   * @param {Object} parent
   * @param {Object} field
   * @param {Object} params
   * @param {function} setValue
   */
  function C(parent, field, params, setValue) {
    var that = this;

    // Set params
    this.params = $.extend({
      elements: [],
      dropZones: []
    }, params);
    setValue(field, this.params);

    // Get updates for fields
    H5PEditor.followField(parent, 'settings/background', function (params) {
      that.setBackground(params);
    });
    H5PEditor.followField(parent, 'settings/size', function (params) {
      that.setSize(params);
    });

    // Need the override background opacity
    this.backgroundOpacity = parent.parent.params.backgroundOpacity;
    this.backgroundOpacity = (this.backgroundOpacity === undefined || this.backgroundOpacity.trim() === '') ? undefined : this.backgroundOpacity;

    // Update opacity for all dropzones/draggables when global background opacity is changed
    parent.ready(function () {
      H5PEditor.findField('../backgroundOpacity', parent).$item.find('input').on('change', function () {
        that.backgroundOpacity = $(this).val().trim();
        that.backgroundOpacity = (that.backgroundOpacity === '') ? undefined : that.backgroundOpacity;
        that.updateAllElementsOpacity(that.elements, that.params.elements, 'element');
      });
    });

    // Get options from semantics, clone since we'll be changing values.
    this.elementFields = H5P.cloneObject(field.fields[0].field.fields, true);
    this.dropZoneFields = H5P.cloneObject(field.fields[1].field.fields, true);
    this.elementLibraryOptions = this.elementFields[0].options;
    this.elementDropZoneFieldWeight = 5;
    this.elementFields[this.elementDropZoneFieldWeight].options = [];
    this.dropZoneElementFieldWeight = 6;
    this.elementOptions = [];

    this.parent = parent;
    this.field = field;

    this.passReadies = true;
    parent.ready(function () {
      that.passReadies = false;
    });

    H5P.$window.on('resize', function () {
      if (that.size !== undefined && that.size.width !== undefined) {
        that.resize();
      }
    });
  }

  /**
   * Append field to wrapper.
   *
   * @param {jQuery} $wrapper
   * @returns {undefined}
   */
  C.prototype.appendTo = function ($wrapper) {
    var that = this;

    this.$item = $(this.createHtml()).appendTo($wrapper);
    this.$editor = this.$item.children('.h5peditor-dragquestion');
    this.$dnbWrapper = this.$item.children('.h5peditor-dragnbar');
    this.$dialog = this.$item.children('.h5peditor-fluid-dialog');
    this.$dialogInner = this.$dialog.children('.h5peditor-fd-inner');
    this.$errors = this.$item.children('.h5p-errors');

    // Handle click events for dialog buttons.
    this.$dialog.find('.h5peditor-done').click(function () {
      if (that.doneCallback() !== false) {
        that.hideDialog();
      }
      return false;
    }).end().find('.h5peditor-remove').click(function () {
      if (confirm(C.t('confirmRemoval'))) {
        that.removeCallback();
        that.hideDialog();
      }
      return false;
    });
  };

  /**
   * Create HTML for the field.
   *
   * @returns {String}
   */
  C.prototype.createHtml = function () {
    var html = '';
    if (this.field.label !== 0) {
      html += '<span class="h5peditor-label">' + this.field.label + '</span>';
    }

    html += '<div class="h5peditor-dragnbar"></div>' +
      '<div class="h5peditor-dragquestion">' + C.t('noTaskSize') + '</div>' +
      '<div class="h5peditor-fluid-dialog">' +
      '  <div class="h5peditor-fd-inner"></div>' +
      '  <div class="h5peditor-fd-buttons">' +
      '    <a href="#" class="h5peditor-fd-button h5peditor-done">' + C.t('done') + '</a>' +
      '    <a href="#" class="h5peditor-fd-button h5peditor-remove">' + C.t('remove') + '</a>' +
      '  </div>' +
      '</div>';

    if (this.field.description !== undefined) {
      html += '<div class="h5peditor-field-description">' + this.field.description + '</div>';
    }

    return H5PEditor.createItem(this.field.widget, html);
  };

  /**
   * Set current background.
   *
   * @param {Object} params
   * @returns {undefined}
   */
  C.prototype.setBackground = function (params) {
    var path = params === undefined ? '' : params.path;
    if (path !== '') {
      // Add correct base path
      path = 'url("' + H5P.getPath(path, H5PEditor.contentId) + '")';
    }

    this.$editor.css({
      backgroundImage: path
    });
  };

  /**
   * Set current dimensions.
   *
   * @param {Object} params
   * @returns {undefined}
   */
  C.prototype.setSize = function (params) {
    this.size = params;
  };

  /**
   * Apply new size to task editor once visible.
   *
   * @returns {undefined}
   */
  C.prototype.setActive = function () {
    if (this.size === undefined || this.size.width === undefined) {
      return;
    }

    if (this.dnb === undefined) {
      this.activateEditor();
    }

    this.resize();
  };

  /**
   * Adapt the editor when the window changes size.
   */
  C.prototype.resize = function () {
    if (!this.$editor.is(':visible')) {
      return;
    }
    if (this.fontSize === undefined) {
      // Get editor default font size.
      this.fontSize = parseInt(this.$editor.css('fontSize'));
    }

    var maxWidth = this.$item.width();
    var editorCSS;
    if (this.size.width < maxWidth) {
      editorCss = {
        width: this.size.width,
        height: this.size.height,
        fontSize: this.fontSize
      };
      this.$dnbWrapper.css({
        width: this.size.width
      });
    }
    else {
      editorCss = {
        width: '100%',
        height: maxWidth * (this.size.height / this.size.width),
        fontSize: this.fontSize * (maxWidth / this.size.width)
      };
      this.$dnbWrapper.css({
        width: '100%'
      });
    }

    this.$editor.css(editorCss);
    if (this.dnb !== undefined) {
      this.dnb.dnr.setContainerEm(editorCss.fontSize);
    }

    this.pToEm = (parseFloat(window.getComputedStyle(this.$editor[0]).width) / this.fontSize) / 100;
  };

  /**
   * Activate DragNBar and add elements.
   *
   * @returns {undefined}
   */
  C.prototype.activateEditor = function () {
    var that = this;
    this.$editor.html('').addClass('h5p-ready');

    // Create new bar
    this.dnb = new DragNBar(this.getButtons(), this.$editor, this.$item);
    that.dnb.dnr.snap = 10;

    // Add event handling
    this.dnb.stopMovingCallback = function (x, y) {
      // Update params when the element is dropped.
      var id = that.dnb.dnd.$element.data('id');
      var params = that.dnb.dnd.$element.hasClass('h5p-dq-dz') ? that.params.dropZones[id] : that.params.elements[id];
      params.x = x;
      params.y = y;
    };
    this.dnb.dnd.releaseCallback = function () {
      // Edit element when it is dropped.
      if (that.dnb.newElement) {
        setTimeout(function () {
          that.dnb.dnd.$element.dblclick();
          that.dnb.blurAll();
        }, 1);
      }
    };
    this.dnb.attach(this.$dnbWrapper);

    this.dnb.on('paste', function (event) {
      var pasted = event.data;
      var $element;

      if (pasted.from === clipboardKey) {
        // Pasted content comes from the same version of DQ

        if (!pasted.generic) {
          // Non generic part, must be a drop zone
          that.center(pasted.specific);
          that.params.dropZones.push(pasted.specific);
          $element = that.insertDropZone(that.params.dropZones.length - 1);
          setTimeout(function () {
            that.dnb.focus($element);
          });
        }
        else if (that.elementLibraryOptions.indexOf(pasted.generic.library) !== -1) {
          // Has generic part and the generic libray is supported
          that.center(pasted.specific);
          that.params.elements.push(pasted.specific);
          $element = that.insertElement(that.params.elements.length - 1);
          setTimeout(function () {
            that.dnb.focus($element);
          });
        }
        else {
          alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
        }
      }
      else if (pasted.generic) {
        if (that.elementLibraryOptions.indexOf(pasted.generic.library) !== -1) {
          // Supported library from another content type
          var id = C.getLibraryID(pasted.generic.library);
          var elementParams = C.getDefaultElementParams(id);
          elementParams.type = pasted.generic;
          elementParams.width = pasted.width * that.pToEm;
          elementParams.height = pasted.height * that.pToEm;

          that.center(elementParams);
          that.params.elements.push(elementParams);
          $element = that.insertElement(that.params.elements.length - 1);
          setTimeout(function () {
            that.dnb.focus($element);
          });
        }
        else {
          alert(H5PEditor.t('H5P.DragNBar', 'unableToPaste'));
        }
      }
    });

    /**
     * Update params on end of resize
     * Dimensions contains a data object where each dimensions is optional.
     */
    this.dnb.dnr.on('stoppedResizing', function (dimensions) {
      var id = that.dnb.$element.data('id');
      var params = that.dnb.$element.hasClass('h5p-dq-dz') ? that.params.dropZones[id] : that.params.elements[id];
      var containerStyle = window.getComputedStyle(that.$editor[0]);

      // Set dimensions if they were passed in
      if (dimensions.data.left) {
        params.x = dimensions.data.left / (parseFloat(containerStyle.width) / 100);
      }
      if (dimensions.data.top) {
        params.y = dimensions.data.top / (parseFloat(containerStyle.height) / 100);
      }
      if (dimensions.data.width) {
        params.width = dimensions.data.width;
      }
      if (dimensions.data.height) {
        params.height = dimensions.data.height;
      }
    });

    // Add Elements
    this.elements = [];
    for (var i = 0; i < this.params.elements.length; i++) {
      this.insertElement(i);
    }

    // Add Drop Zones
    this.dropZones = [];
    for (var j = 0; j < this.params.dropZones.length; j++) {
      this.insertDropZone(j);
    }
  };

  /**
   * Help center new elements
   * @param {object} params
   */
  C.prototype.center = function (params) {
    var size = window.getComputedStyle(this.dnb.$container[0]);
    var width = parseFloat(size.width);
    var height = parseFloat(size.height);
    var pos = {
      x: (width - (params.width * this.fontSize)) / 2,
      y: (height - (params.height * this.fontSize)) / 2
    };
    this.dnb.avoidOverlapping(pos, {
      width: params.width * this.fontSize,
      height: params.height * this.fontSize,
    });
    params.x = pos.x / (width / 100);
    params.y = pos.y / (height / 100);
  };

  /**
   * Generate sub forms that's ready to use in the dialog.
   *
   * @param {Object} semantics
   * @param {Object} params
   * @returns {Object} generatedForm
   */
  C.prototype.generateForm = function (semantics, params) {
    var $form = $('<div></div>');
    H5PEditor.processSemanticsChunk(semantics, params, $form, this);
    var $lib = $form.children('.library:first');
    if ($lib.length !== 0) {
      $lib.children('label, select, .h5peditor-field-description').hide().end().children('.libwrap').css('margin-top', '0');
    }

    return {
      $form: $form,
      children: this.children
    };
  };

  /**
   * Generate a list of buttons for DnB.
   *
   * @returns {Array} Buttons
   */
  C.prototype.getButtons = function () {
    var that = this;

    var buttons = [{
      id: 'dropzone',
      title: 'Drop Zone',
      createElement: function () {
        that.params.dropZones.push({
          x: 0,
          y: 0,
          width: 5,
          height: 2.5,
          correctElements: []
        });

        return that.insertDropZone(that.params.dropZones.length - 1);
      }
    }];

    for (var i = 0; i < this.elementLibraryOptions.length; i++) {
      buttons.push(this.getButton(this.elementLibraryOptions[i]));
    }

    return buttons;
  };

  /**
   * Creates a fresh object with default element parameters.
   * @returns {object}
   */
  C.getDefaultElementParams = function (id) {
    return {
      x: 0,
      y: 0,
      width: 5,
      height: id === 'text' ? 1.25 : 5,
      dropZones: []
    };
  };

  /**
   * Find generic library identifier without version name.
   *
   * @param {string} library
   * @returns {string}
   */
  C.getLibraryID = function (library) {
    return library.split(' ')[0].split('.')[1].toLowerCase();
  };

  /**
   * Generate a single element button for the DnB.
   *
   * @param {String} library Library name + version
   * @returns {Object} DnB button semantics
   */
  C.prototype.getButton = function (library) {
    var that = this;
    var id = C.getLibraryID(library);
    return {
      id: id,
      title: C.t('insertElement', {':type': id}),
      createElement: function () {
        var elementParams = C.getDefaultElementParams(id);
        elementParams.type = {
          library: library,
          params: {}
        };
        that.params.elements.push(elementParams);
        return that.insertElement(that.params.elements.length - 1);
      }
    };
  };

  /**
   * Insert element at given params index.
   *
   * @param {int} index
   * @returns {jQuery} The element's DOM
   */
  C.prototype.insertElement = function (index) {
    var that = this;
    var elementParams = this.params.elements[index];
    var element = this.generateForm(this.elementFields, elementParams);

    var library = this.children[0];

    // Get image aspect ratio
    var libraryChange = function () {
      if (library.children[0].field.type === 'image') {
        library.children[0].changes.push(function (params) {
          if (params === undefined) {
            return;
          }

          if (params.width !== undefined && params.height !== undefined) {
            var editorStyles = window.getComputedStyle(that.$editor[0]);
            var editorWidth = parseFloat(editorStyles.width);
            var editorHeight = parseFloat(editorStyles.height);

            var aspectRatio = params.height / params.width;
            if (editorHeight / editorWidth > aspectRatio) {
              elementParams.height = elementParams.width * aspectRatio;
            }
            else {
              elementParams.width = elementParams.height / aspectRatio;
            }

            element.$element.css({
              width: elementParams.width + 'em',
              height: elementParams.height + 'em'
            });
          }
        });
      }
    };

    if (library.children === undefined) {
      library.changes.push(libraryChange);
    }
    else {
      libraryChange();
    }

    element.$element = $('<div class="h5p-dq-element" style="width:' + elementParams.width + 'em;height:' + elementParams.height + 'em;top:' + elementParams.y + '%;left:' + elementParams.x + '%"></div>')
      .data('id', index)
      .appendTo(this.$editor)
      .dblclick(function () {
        that.editElement(element);
      }).hover(function () {
        C.setElementOpacity(element.$element, that.getElementOpacitySetting(elementParams));
      }, function () {
        // Need this timeout for firefox beeing able to get the css hover rule in place
        setTimeout(function () {
          C.setElementOpacity(element.$element, that.getElementOpacitySetting(elementParams));
        }, 1);
      });

    element.$innerElement = $('<div>', {
      'class': 'h5p-dq-element-inner'
    }).appendTo(element.$element);

    setTimeout(function () {
      var dnbElement = that.dnb.add(element.$element, DragNBar.clipboardify(clipboardKey, elementParams, 'type'));

      dnbElement.contextMenu.on('contextMenuEdit', function () {
        that.editElement(element);
        that.dnb.blurAll();
      });

      dnbElement.contextMenu.on('contextMenuRemove', function () {
        if (!confirm(C.t('confirmRemoval'))) {
          return;
        }
        var i, j, ce;
        var id = element.$element.data('id');

        // Remove element form
        H5PEditor.removeChildren(element.children);

        // Remove element
        element.$element.remove();
        that.elements.splice(id, 1);
        that.params.elements.splice(id, 1);

        // Remove from options
        that.elementOptions.splice(id, 1);

        // Update drop zone params
        for (i = 0; i < that.params.dropZones.length; i++) {
          ce = that.params.dropZones[i].correctElements;
          for (j = 0; j < ce.length; j++) {
            if (ce[j] === '' + id) {
              // Remove from correct answers
              ce.splice(j, 1);
            }
            else if (ce[j] > id) {
              // Adjust index for others
              ce[j] = '' + (ce[j] - 1);
            }
          }
        }

        that.updateInternalElementIDs(id);
        that.dnb.blurAll();
      });

      dnbElement.contextMenu.on('contextMenuBringToFront', function () {
        // Find element ID
        var id = element.$element.data('id');

        // Update visuals
        element.$element.appendTo(that.$editor);

        // Give new ID
        that.elements.push(that.elements.splice(id, 1)[0]);
        that.params.elements.push(that.params.elements.splice(id, 1)[0]);
        that.elementOptions.push(that.elementOptions.splice(id, 1)[0]);
        var newID = (that.elements.length - 1);

        // Update drop zone params
        for (i = 0; i < that.params.dropZones.length; i++) {
          ce = that.params.dropZones[i].correctElements;
          for (j = 0; j < ce.length; j++) {
            if (ce[j] === '' + id) {
              // Update ID in correct answers
              ce[j] = newID;
            }
            else if (ce[j] > id) {
              // Adjust index for others
              ce[j] = '' + (ce[j] - 1);
            }
          }
        }

        that.updateInternalElementIDs(id);
      });
      that.dnb.focus(element.$element);
    }, 0);

    // Update element
    that.updateElement(element, index);

    this.elements[index] = element;
    return element.$element;
  };

  /**
   * Sync the internal ID of each element.
   * @param {number} start
   */
  C.prototype.updateInternalElementIDs = function (start) {
    for (i = start; i < this.elements.length; i++) {
      this.elements[i].$element.data('id', i);
      this.elementOptions[i].value = '' + i;
    }
  };

  /**
   * Set callbacks and open dialog with the form for the given element.
   *
   * @param {Object} element
   * @returns {undefined}
   */
  C.prototype.editElement = function (element) {
    var that = this;
    var id = element.$element.data('id');

    this.doneCallback = function () {
      // Validate form
      var valid = true;
      for (var i = 0; i < element.children.length; i++) {
        if (element.children[i].validate() === false) {
          valid = false;
          break;
        }
      }
      if (!valid) {
        return false;
      }


      // Must be removed before dnb changes focus!
      if (H5PEditor.Html) {
        H5PEditor.Html.removeWysiwyg();
      }

      // Update element
      that.updateElement(element, id);
      that.dnb.focus(element.$element);
      that.dnb.pressed = undefined;
    };

    this.removeCallback = function () {
      var i, j, ce;

      // Remove element form
      H5PEditor.removeChildren(element.children);

      // Remove element
      element.$element.remove();
      that.elements.splice(id, 1);
      that.params.elements.splice(id, 1);

      // Remove from options
      this.elementOptions.splice(id, 1);

      // Update drop zone params
      for (i = 0; i < that.params.dropZones.length; i++) {
        ce = that.params.dropZones[i].correctElements;
        for (j = 0; j < ce.length; j++) {
          if (ce[j] === '' + id) {
            // Remove from correct answers
            ce.splice(j, 1);
          }
          else if (ce[j] > id) {
            // Adjust index for others
            ce[j] = '' + (ce[j] - 1);
          }
        }
      }

      // Change data index for "all" elements
      for (i = id; i < that.elements.length; i++) {
        that.elements[i].$element.data('id', i);
        that.elementOptions[i].value = '' + i;
      }
    };

    // Disable background opacity input if overriden globally
    var disableOpacityField = !!(that.params.elements[id].dropZones.length !== 0 && this.backgroundOpacity);
    H5PEditor.findField('backgroundOpacity', element).$item.find('input').prop({
      disabled: disableOpacityField,
      title: disableOpacityField ? C.t('backgroundOpacityOverridden') : ''
    });

    element.children[this.elementDropZoneFieldWeight].setActive();
    this.showDialog(element.$form);

    // Blur context menu when showing dialog.
    setTimeout(function () {
      that.dnb.blurAll();
    }, 10);
  };

  /**
   * Update the element with new data.
   *
   * @param {Object} element
   * @param {int} id
   * @returns {undefined}
   */
  C.prototype.updateElement = function (element, id) {
    var self = this;
    var params = this.params.elements[id];

    var type = (params.type.library.split(' ')[0] === 'H5P.AdvancedText' ? 'text' : 'image');
    var hasCk = (element.children[0].children !== undefined && element.children[0].children[0].ckeditor !== undefined);
    if (type === 'text' && hasCk) {
      // Create new text instance. Replace asterisk with spans
      element.instance = H5P.newRunnable({
        library: params.type.library,
        params: {
          text: params.type.params.text.replace(/\*([^*]+)\*/g, '<span class="h5p-dragquestion-placeholder">$1</span>')
        }
      }, H5PEditor.contentId, element.$innerElement);

      // Remove asterisk from params and input field
      params.type.params.text = params.type.params.text.replace(/\*([^*]+)\*/g, '$1');
      element.children[0].children[0].ckeditor.setData(params.type.params.text);
    }
    else {
      // Create new instance
      element.instance = H5P.newRunnable(params.type, H5PEditor.contentId, element.$innerElement);
    }

    // Find label text without html
    var label = (type === 'text' ? $('<div>' + params.type.params.text + '</div>').text() : params.type.params.alt + '');

    // Update correct element options
    this.elementOptions[id] = {
      value: '' + id,
      label: C.t(type) + ': ' + C.getLabel(label)
    };

    // Retain size after toggling class
    var toggleDraggable = function (addClass, $element) {

      var toggleClass = addClass !== $element.hasClass('h5p-draggable');
      if (!toggleClass) {
        return;
      }

      var prevWidth = $element.outerWidth();
      var prevHeight = $element.outerHeight();
      var id = $element.data('id');
      var params = $element.hasClass('h5p-dq-dz') ? self.params.dropZones[id] : self.params.elements[id];
      var fontSize = self.fontSize || parseFloat(self.$editor.css('font-size'));
      var newWidth = (prevWidth / fontSize);
      var newHeight = (prevHeight / fontSize);

      if (addClass) {
        $element.addClass('h5p-draggable');

        $element.outerWidth(prevWidth);
        $element.outerHeight(prevHeight);
      }
      else {
        $element.removeClass('h5p-draggable');

        element.$element.css('width', newWidth + 'em');
        element.$element.css('height', newHeight + 'em');
      }

      if ($element.hasClass('h5p-dragnbar-element')) {
        params.width = newWidth;
        params.height = newHeight;
      }
    };

    if (params.dropZones !== undefined && params.dropZones.length) {

      toggleDraggable(true, element.$element);
    }
    else {
      toggleDraggable(false, element.$element);

      if (type === 'text' && hasCk) {
        // When dialog closes, replace spans with drop zones
        this.hideDialogCallback = function () {
          var pWidth = self.$editor.width() / 100;
          var pHeight = self.$editor.height() / 100;
          element.$element.find('.h5p-dragquestion-placeholder').each(function () {
            var $span = $(this);
            var pos = $span.position();

            // Add new drop zone
            self.params.dropZones.push({
              x: params.x + ((pos.left - 3) / pWidth),
              y: params.y + ((pos.top - 2) / pHeight),
              width: ($span.width() / self.fontSize) + 0.5,
              height: ($span.height() / self.fontSize) + 0.3,
              backgroundOpacity: 0,
              correctElements: [],
              label: C.getLabel($span.text()),
              showLabel: false
            });
            self.insertDropZone(self.params.dropZones.length - 1);

            // Remove span
            $span.contents().unwrap();
          });
          delete self.hideDialogCallback;
        };
      }
    }

    C.setElementOpacity(element.$element, this.getElementOpacitySetting(params));
  };

  /**
   * Clips text at 32 chars
   *
   * @param {String} text
   * @returns {String}
   */
  C.getLabel = function (text) {
    return (text.length > 32 ? text.substr(0, 32) + '...' : text);
  };

  /**
   * Insert the drop zone at the given index.
   *
   * @param {int} index
   * @returns {H5P.jQuery}
   */
  C.prototype.insertDropZone = function (index) {
    var that = this,
      dropZoneParams = this.params.dropZones[index],
      dropZone = this.generateForm(this.dropZoneFields, dropZoneParams);

    dropZone.$dropZone = $('<div class="h5p-dq-dz" style="width:' + dropZoneParams.width + 'em;height:' + dropZoneParams.height + 'em;top:' + dropZoneParams.y + '%;left:' + dropZoneParams.x + '%"></div>')
      .appendTo(this.$editor)
      .data('id', index)
      .dblclick(function () {
        // Edit
        that.editDropZone(dropZone);
        that.dnb.blurAll();
      });

    // Add tip if any
    if (dropZoneParams.tip !== undefined && dropZoneParams.tip.trim().length > 0) {
      dropZone.$dropZone.append(H5P.JoubelUI.createTip(dropZoneParams.tip, {showSpeechBubble: false}));
    }

    // Add label
    this.updateDropZone(dropZone, index);

    // Add to dnb after element has been attached
    setTimeout(function () {

      var dropzoneDnBElement = that.dnb.add(dropZone.$dropZone, DragNBar.clipboardify(clipboardKey, dropZoneParams));

      // Register listeners for context menu buttons
      dropzoneDnBElement.contextMenu.on('contextMenuEdit', function () {
        that.editDropZone(dropZone);
        that.dnb.blurAll();
      });

      dropzoneDnBElement.contextMenu.on('contextMenuRemove', function () {
        if (!confirm(C.t('confirmRemoval'))) {
          return;
        }

        // Remove element form
        H5PEditor.removeChildren(dropZone.children);
        var i;
        var j;
        var id = dropZone.$dropZone.data('id');

        // Remove element
        dropZone.$dropZone.remove();
        that.dropZones.splice(id, 1);
        that.params.dropZones.splice(id, 1);

        // Remove from elements
        that.elementFields[that.elementDropZoneFieldWeight].options.splice(id, 1);

        // Remove dropZone from element params properly
        for (i = 0; i < that.params.elements.length; i++) {
          var dropZones = that.params.elements[i].dropZones;
          for (j = 0; j < dropZones.length; j++) {
            if (parseInt(dropZones[j]) === id) {
              // Remove from element drop zones
              dropZones.splice(j, 1);
              if (!dropZones.length) {
                that.elements[i].$element.removeClass('h5p-draggable');
              }
            }
            else if (dropZones[j] > id) {
              // Re index other drop zones
              dropZones[j] = '' + (dropZones[j] - 1);
            }
          }
        }

        that.updateInternalDropZoneIDs(id);
        that.dnb.blurAll();
      });

      dropzoneDnBElement.contextMenu.on('contextMenuBringToFront', function () {
        var id = dropZone.$dropZone.data('id');

        // Update visuals
        dropZone.$dropZone.appendTo(that.$editor);

        // Get new ID
        that.dropZones.push(that.dropZones.splice(id, 1)[0]);
        that.params.dropZones.push(that.params.dropZones.splice(id, 1)[0]);
        var options = that.elementFields[that.elementDropZoneFieldWeight].options;
        options.push(options.splice(id, 1)[0]);
        var newID = (that.dropZones.length - 1);

        // Update dropZone IDs in element params
        for (i = 0; i < that.params.elements.length; i++) {
          var dropZones = that.params.elements[i].dropZones;
          for (j = 0; j < dropZones.length; j++) {
            if (parseInt(dropZones[j]) === id) {
              // Update ID
              dropZones[j] = newID;
            }
            else if (dropZones[j] > id) {
              // Re-index other drop zones
              dropZones[j] = '' + (dropZones[j] - 1);
            }
          }
        }

        that.updateInternalDropZoneIDs(id);
      });
      that.dnb.focus(dropZone.$dropZone);
    }, 0);

    // Add tip if any
    if (dropZoneParams.tip !== undefined && dropZoneParams.tip.trim().length > 0) {
      dropZone.$dropZone.append(H5P.JoubelUI.createTip(dropZoneParams.tip, {showSpeechBubble: false}));
    }

    // Add label
    this.updateDropZone(dropZone, index);

    this.dropZones[index] = dropZone;
    return dropZone.$dropZone;
  };

  /**
   * Sync the internal ID of each drop zone.
   * @param {number} start
   */
  C.prototype.updateInternalDropZoneIDs = function (start) {
    for (i = start; i < this.dropZones.length; i++) {
      this.dropZones[i].$dropZone.data('id', i);
      this.elementFields[this.elementDropZoneFieldWeight].options[i].value = i + '';
    }
  };

  /**
   * Set callbacks and open dialog with the form for the given drop zone.
   *
   * @param {Object} dropZone
   * @returns {undefined}
   */
  C.prototype.editDropZone = function (dropZone) {
    var that = this;
    var i, j, id = dropZone.$dropZone.data('id');

    this.doneCallback = function () {
      // Validate form
      var valid = true;
      for (var i = 0; i < dropZone.children.length; i++) {
        if (dropZone.children[i].validate() === false) {
          valid = false;
          break;
        }
      }
      if (!valid) {
        return false;
      }

      // Must be removed before dnb changes focus!
      if (H5PEditor.Html) {
        H5PEditor.Html.removeWysiwyg();
      }

      that.updateDropZone(dropZone, id);
      that.dnb.focus(dropZone.$dropZone);
      that.dnb.pressed = undefined;
    };

    this.removeCallback = function () {
      // Remove element form
      H5PEditor.removeChildren(dropZone.children);

      // Remove element
      dropZone.$dropZone.remove();
      that.dropZones.splice(id, 1);
      that.params.dropZones.splice(id, 1);

      // Remove from elements
      this.elementFields[this.elementDropZoneFieldWeight].options.splice(id, 1);

      // Remove dropZone from element params properly
      for (i = 0; i < that.params.elements.length; i++) {
        var dropZones = that.params.elements[i].dropZones;
        for (j = 0; j < dropZones.length; j++) {
          if (parseInt(dropZones[j]) === id) {
            // Remove from element drop zones
            dropZones.splice(j, 1);
            if (!dropZones.length) {
              that.elements[i].$element.removeClass('h5p-draggable');
            }
          }
          else if (dropZones[j] > id) {
            // Re index other drop zones
            dropZones[j] = '' + (dropZones[j] - 1);
          }
        }
      }

      // Reindex all dropzones
      for (i = id; i < that.dropZones.length; i++) {
        that.dropZones[i].$dropZone.data('id', i);
        this.elementFields[this.elementDropZoneFieldWeight].options[i].value = i + '';
      }
    };

    // Add only available options
    var options = this.dropZoneFields[this.dropZoneElementFieldWeight].options = [];
    var dropZones;
    for (i = 0; i < this.elementOptions.length; i++) {
      dropZones = this.params.elements[i].dropZones;
      for (j = 0; j < dropZones.length; j++) {
        if (dropZones[j] === (id + '')) {
          options.push(this.elementOptions[i]);
          break;
        }
      }
    }

    dropZone.children[this.dropZoneElementFieldWeight].setActive();
    this.showDialog(dropZone.$form);

    // Blur context menu when showing dialog
    setTimeout(function () {
      that.dnb.blurAll();
    }, 10);
  };

  /**
   * Remove old label and add new.
   *
   * @param {Object} dropZone
   * @param {int} id
   * @returns {undefined}
   */
  C.prototype.updateDropZone = function (dropZone, id) {
    var params = this.params.dropZones[id];

    // Remove old label and add new.
    dropZone.$dropZone.children('.h5p-dq-dz-label').remove();
    if (params.showLabel === true) {
      $('<div class="h5p-dq-dz-label">' + params.label + '</div>').appendTo(dropZone.$dropZone);
      dropZone.$dropZone.addClass('h5p-has-label');
    }
    else {
      dropZone.$dropZone.removeClass('h5p-has-label');
    }

    // Update Tip:
    dropZone.$dropZone.children('.joubel-tip-container').remove();
    if (params.tip !== undefined && params.tip.trim().length > 0) {
      dropZone.$dropZone.append(H5P.JoubelUI.createTip(params.tip, {showSpeechBubble: false}));
    }

    this.elementFields[this.elementDropZoneFieldWeight].options[id] = {
      value: '' + id,
      label: params.label
    };

    C.setOpacity(dropZone.$dropZone.add(dropZone.$dropZone.children('.h5p-dq-dz-label')), 'background', params.backgroundOpacity);
  };

  /**
   * Attach form to dialog and show.
   *
   * @param {jQuery} $form
   * @returns {undefined}
   */
  C.prototype.showDialog = function ($form) {
    this.dnb.blurAll();
    this.$currentForm = $form;
    $form.appendTo(this.$dialogInner);
    this.$dialog.show();
    this.$editor.add(this.$dnbWrapper).hide();
  };

  /**
   * Hide dialog and detach form.
   *
   * @returns {undefined}
   */
  C.prototype.hideDialog = function () {
    // Attempt to find and close CKEditor instances before detaching.


    this.$currentForm.detach();
    this.$dialog.hide();
    this.$editor.add(this.$dnbWrapper).show();

    if (this.hideDialogCallback !== undefined) {
      this.hideDialogCallback();
    }
  };

  /**
   * Update transparency for background.
   *
   * @param {jQuery} $element
   * @param {Number} opacity
   */
  C.setElementOpacity = function ($element, opacity) {
    C.setOpacity($element, 'background', opacity);
    C.setOpacity($element, 'boxShadow', opacity);
    C.setOpacity($element, 'borderColor', opacity);
  };

  /**
   * Update all elements' opacity
   *
   * @param {Array} domElements
   * @param {Array} elements
   * @param {String} type
   */
  C.prototype.updateAllElementsOpacity = function (domElements, elements, type) {
    if (domElements === undefined) {
      return;
    }

    for (var i = 0; i < domElements.length; i++) {
      C.setElementOpacity(domElements[i]['$'+type], this.getElementOpacitySetting(elements[i]));
    }
  };

  /**
   * Get the opacity setting for a given element
   *
   * @param {Object} element
   * @returns {String} opacity
   */
  C.prototype.getElementOpacitySetting = function (element) {
    if ((element.dropZones !== undefined && element.dropZones.length === 0) ||
       (this.backgroundOpacity === undefined)) {
      return element.backgroundOpacity;
    }

    return this.backgroundOpacity;
  };

  /**
   * Makes element background, border and shadow transparent.
   *
   * @param {jQuery} $element
   * @param {String} property
   * @param {Number} opacity
   */
  C.setOpacity = function ($element, property, opacity) {
    if (property === 'background') {
      // Set both color and gradient.
      C.setOpacity($element, 'backgroundColor', opacity);
      C.setOpacity($element, 'backgroundImage', opacity);
      return;
    }

    opacity = (opacity === undefined ? 1 : opacity / 100);

    // Private. Get css properties objects.
    function getProperties(property, value) {
      switch (property) {
        case 'borderColor':
          return {
            borderTopColor: value,
            borderRightColor: value,
            borderBottomColor: value,
            borderLeftColor: value
          };

        default:
          var properties = {};
          properties[property] = value;
          return properties;
      }
    }

    // Reset css to be sure we're using CSS and not inline values.
    var properties = getProperties(property, '');
    $element.css(properties);

    for (var prop in properties) {
      break;
    }
    var style = $element.css(prop); // Assume all props are the same and use the first.
    style = C.setAlphas(style, 'rgba(', opacity); // Update rgba
    style = C.setAlphas(style, 'rgb(', opacity); // Convert rgb

    $element.css(getProperties(property, style));
  };

  /**
   * Updates alpha channel for colors in the given style.
   *
   * @param {String} style
   * @param {String} prefix
   * @param {Number} alpha
   */
  C.setAlphas = function (style, prefix, alpha) {
    // Style undefined
    if (!style) {
      return;
    }
    var colorStart = style.indexOf(prefix);

    while (colorStart !== -1) {
      var colorEnd = style.indexOf(')', colorStart);
      var channels = style.substring(colorStart + prefix.length, colorEnd).split(',');

      // Set alpha channel
      channels[3] = (channels[3] !== undefined ? parseFloat(channels[3]) * alpha : alpha);

      style = style.substring(0, colorStart) + 'rgba(' + channels.join(',') + style.substring(colorEnd, style.length);

      // Look for more colors
      colorStart = style.indexOf(prefix, colorEnd);
    }

    return style;
  };

  /**
   * Validate the current field.
   *
   * @returns {Boolean}
   */
  C.prototype.validate = function () {
    return true;
  };

  /**
   * Remove the field from DOM.
   *
   * @returns {undefined}
   */
  C.prototype.remove = function () {
    this.$item.remove();
  };

  /**
   * Collect functions to execute once the tree is complete.
   *
   * @param {function} ready
   * @returns {undefined}
   */
  C.prototype.ready = function (ready) {
    if (this.passReadies) {
      this.parent.ready(ready);
    }
    else {
      this.readies.push(ready);
    }
  };

  /**
   * Translate UI texts for this library.
   *
   * @param {String} key
   * @param {Object} vars
   * @returns {@exp;H5PEditor@call;t}
   */
  C.t = function (key, vars) {
    return H5PEditor.t('H5PEditor.DragQuestion', key, vars);
  };

  return C;
})(H5P.jQuery, H5P.DragNBar);

// Default english translations
H5PEditor.language['H5PEditor.DragQuestion'] = {
  libraryStrings: {
    insertElement: 'Insert :type',
    done: 'Done',
    remove: 'Remove',
    image: 'Image',
    text: 'Text',
    noTaskSize: 'Please specify task size first.',
    confirmRemoval: 'Are you sure you wish to remove this element?',
    backgroundOpacityOverridden: 'The background opacity is overridden'
  }
};
;var H5P = H5P || {};

/**
 * DragQuestion module.
 *
 * @param {jQuery} $
 */
H5P.DragQuestion = (function ($) {

  /**
   * Initialize module.
   *
   * @class
   * @extend H5P.Question
   * @param {Object} options Run parameters
   * @param {Number} id Content identification
   */
  function C(options, contentId, contentData) {
    var self = this;
    var i;
    this.id = this.contentId = contentId;
    H5P.Question.call(self, 'dragquestion');
    this.options = $.extend(true, {}, {
      scoreShow: 'Check',
      correct: 'Solution',
      tryAgain: 'Retry',
      feedback: "You placed @score out of @total correct.",
      question: {
        settings: {
          questionTitle: 'Drag and drop',
          size: {
            width: 620,
            height: 310
          }
        },
        task: {
          elements: [],
          dropZones: []
        }
      },
      behaviour: {
        enableRetry: true,
        preventResize: false,
        singlePoint: true,
        showSolutionsRequiresInput: true
      }
    }, options);

    this.draggables = [];
    this.dropZones = [];
    this.answered = (contentData && contentData.previousState !== undefined && contentData.previousState.answers !== undefined && contentData.previousState.answers.length);
    this.blankIsCorrect = true;

    this.backgroundOpacity = (this.options.backgroundOpacity === undefined || this.options.backgroundOpacity.trim() === '') ? undefined : this.options.backgroundOpacity;

    // Create map over correct drop zones for elements
    var task = this.options.question.task;
    this.correctDZs = [];
    for (i = 0; i < task.dropZones.length; i++) {
      var correctElements = task.dropZones[i].correctElements;
      for (var j = 0; j < correctElements.length; j++) {
        var correctElement = correctElements[j];
        if (this.correctDZs[correctElement] === undefined) {
          this.correctDZs[correctElement] = [];
        }
        this.correctDZs[correctElement].push(i);
      }
    }

    this.weight = 1;

    // TODO: Initialize elements and drop zones here!

    // Add draggable elements
    for (i = 0; i < task.elements.length; i++) {
      var element = task.elements[i];

      if (element.dropZones === undefined || !element.dropZones.length) {
        continue; // Not a draggable
      }

      if (this.backgroundOpacity !== undefined) {
        element.backgroundOpacity = this.backgroundOpacity;
      }

      // Restore answers from last session
      var answers = null;
      if (contentData && contentData.previousState !== undefined && contentData.previousState.answers !== undefined && contentData.previousState.answers[i] !== undefined) {
        answers = contentData.previousState.answers[i];
      }

      // Create new draggable instance
      this.draggables[i] = new Draggable(element, i, answers);
      this.draggables[i].on('interacted', function () {
        self.answered = true;
        self.triggerXAPIScored(self.getScore(), self.getMaxScore(), 'interacted');
      });
    }

    // Add drop zones
    for (i = 0; i < task.dropZones.length; i++) {
      var dropZone = task.dropZones[i];

      if (this.blankIsCorrect && dropZone.correctElements.length) {
        this.blankIsCorrect = false;
      }

      this.dropZones[i] = new DropZone(dropZone, i);
    }

    this.on('resize', self.resize, self);
    this.on('domChanged', function(event) {
      if (self.contentId === event.data.contentId) {
        self.trigger('resize');
      }
    });

    this.on('enterFullScreen', function () {
      if (self.$container) {
        self.$container.parents('.h5p-content').css('height', '100%');
        self.trigger('resize');
      }
    });

    this.on('exitFullScreen', function () {
      if (self.$container) {
        self.$container.parents('.h5p-content').css('height', 'auto');
        self.trigger('resize');
      }
    });
  }

  C.prototype = Object.create(H5P.Question.prototype);
  C.prototype.constructor = C;


  /**
  * Registers this question type's DOM elements before they are attached.
  * Called from H5P.Question.
  */
  C.prototype.registerDomElements = function () {
    var self = this;

    // Register introduction section
    self.setIntroduction('<p>' + self.options.question.settings.questionTitle + '</p>');


    // Set class if no background
    var contentClass = this.options.question.settings.background !== undefined ? '' : 'h5p-dragquestion-has-no-background';

    // Register task content area
    self.setContent(self.createQuestionContent(), {
      'class': contentClass
    });

    // ... and buttons
    self.registerButtons();

    setTimeout(function () {
      self.trigger('resize');
    }, 200);
  };
  
  /**
   * Add the question itselt to the definition part of an xAPIEvent
   */
  C.prototype.addQuestionToXAPI = function(xAPIEvent) {
    var definition = xAPIEvent.getVerifiedStatementValue(['object', 'definition']);
    definition.description = {
      // Remove tags, must wrap in div tag because jQuery 1.9 will crash if the string isn't wrapped in a tag.
      'en-US': $('<div>' + this.options.question.settings.questionTitle + '</div>').text()
    };
    definition.type = 'http://adlnet.gov/expapi/activities/cmi.interaction';
    definition.interactionType = 'matching';

    // Add sources, i.e. draggables
    definition.source = [];
    for (var i = 0; i < this.options.question.task.elements.length; i++) {
      var el = this.options.question.task.elements[i];
      if (el.dropZones && el.dropZones.length) {
        var desc = el.type.params.alt ? el.type.params.alt : el.type.params.text;
        
        definition.source.push({
          'id': i,
          'description': {
            // Remove tags, must wrap in div tag because jQuery 1.9 will crash if the string isn't wrapped in a tag.
            'en-US': $('<div>' + desc + '</div>').text()
          }
        });
      }
    }
    
    // Add targets, i.e. drop zones, and the correct response pattern.
    definition.correctResponsesPattern = [''];
    definition.target = [];
    var firstCorrectPair = true;
    for (var i = 0; i < this.options.question.task.dropZones.length; i++) {
      definition.target.push({
        'id': i,
        'description': {
          // Remove tags, must wrap in div tag because jQuery 1.9 will crash if the string isn't wrapped in a tag.
          'en-US': $('<div>' + this.options.question.task.dropZones[i].label + '</div>').text()
        }
      });
      if (this.options.question.task.dropZones[i].correctElements) {
        for (var j = 0; j < this.options.question.task.dropZones[i].correctElements.length; j++) {
          if (!firstCorrectPair) {
            definition.correctResponsesPattern += '[,]';
          }
          definition.correctResponsesPattern += i + '[.]' + this.options.question.task.dropZones[i].correctElements[j];
          firstCorrectPair = false;
        }
      }
    }
  };
  
  /**
   * Add the response part to an xAPI event
   *
   * @param {H5P.XAPIEvent} xAPIEvent
   *  The xAPI event we will add a response to
   */
  C.prototype.addResponseToXAPI = function(xAPIEvent) {
    var maxScore = this.getMaxScore();
    var score = this.getScore();
    var success = score == maxScore ? true : false;
    xAPIEvent.setScoredResult(score, maxScore, this, true, success);
    var response = '';
    var firstPair = true;
    // State system will be rewritten to use xAPI, but until it does we convert
    // the state to xAPI here...
    var state = this.getCurrentState();
    if (state.answers !== undefined) {
      for (var i = 0; i < state.answers.length; i++) {
        if (state.answers[i] !== undefined) {       
          for (var j = 0; j < state.answers[i].length; j++) {
            if (!firstPair) {
              response += '[,]';
            }
            response += state.answers[i][j].dz + '[.]' + i;
            firstPair = false;
          }
        }
      }
    }
    xAPIEvent.data.statement.result.response = response;
  };

  /**
   * Append field to wrapper.
   */
  C.prototype.createQuestionContent = function () {
    var i;
    // If reattaching, we no longer show solution. So forget that we
    // might have done so before.

    this.$container = $('<div class="h5p-inner"></div>');
    if (this.options.question.settings.background !== undefined) {
      this.$container.css('backgroundImage', 'url("' + H5P.getPath(this.options.question.settings.background.path, this.id) + '")');
    }

    var task = this.options.question.task;

    // Add elements (static and draggable)
    for (i = 0; i < task.elements.length; i++) {
      var element = task.elements[i];

      if (element.dropZones !== undefined && element.dropZones.length !== 0) {
        // Attach draggable elements
        this.draggables[i].appendTo(this.$container, this.id);
      }
      else {
        // Add static element
        var $element = this.addElement(element, 'static', i);
        H5P.newRunnable(element.type, this.id, $element);
        var timedOutOpacity = function($el, el) {
          setTimeout(function () {
            C.setOpacity($el, 'background', el.backgroundOpacity);
          }, 0);
        };
        timedOutOpacity($element, element);
      }
    }

    // Attach drop zones
    for (i = 0; i < this.dropZones.length; i++) {
      this.dropZones[i].appendTo(this.$container, this.draggables);
    }
    return this.$container;
  };

  C.prototype.registerButtons = function () {
    // Add show score button
    this.addSolutionButton();
    this.addRetryButton();
  };

  /**
   * Makes sure element gets correct opacity when hovered.
   *
   * @param {jQuery} $element
   * @param {Object} element
   */
  C.addHover = function ($element, backgroundOpacity) {
    $element.hover(function () {
      $element.addClass('h5p-draggable-hover');
      if (!$element.parent().hasClass('h5p-dragging')) {
        C.setElementOpacity($element, backgroundOpacity);
      }
    }, function () {
      if (!$element.parent().hasClass('h5p-dragging')) {
        setTimeout(function () {
          $element.removeClass('h5p-draggable-hover');
          C.setElementOpacity($element, backgroundOpacity);
        }, 1);
      }
    });
    C.setElementOpacity($element, backgroundOpacity);
  };

  /**
   * Makes element background transparent.
   *
   * @param {jQuery} $element
   * @param {Number} opacity
   */
  C.setElementOpacity = function ($element, opacity) {
    C.setOpacity($element, 'borderColor', opacity);
    C.setOpacity($element, 'boxShadow', opacity);
    C.setOpacity($element, 'background', opacity);
  };

  /**
   * Add solution button to our container.
   */
  C.prototype.addSolutionButton = function () {
    var that = this;

    this.addButton('check-answer', this.options.scoreShow, function () {
      that.showAllSolutions();
      that.showScore();
      var xAPIEvent = that.createXAPIEventTemplate('answered');
      that.addQuestionToXAPI(xAPIEvent);
      that.addResponseToXAPI(xAPIEvent);
      that.trigger(xAPIEvent);
    });
  };

  /**
   * Add retry button to our container.
   */
  C.prototype.addRetryButton = function () {
    var that = this;

    this.addButton('try-again', this.options.tryAgain, function () {
      that.resetTask();
      that.showButton('check-answer');
      that.hideButton('try-again');
    }, false);
  };

  /**
   * Add element/drop zone to task.
   *
   * @param {Object} element
   * @param {String} type Class
   * @param {Number} id
   * @returns {jQuery}
   */
  C.prototype.addElement = function (element, type, id) {
    return $('<div class="h5p-' + type + '" style="left:' + element.x + '%;top:' + element.y + '%;width:' + element.width + 'em;height:' + element.height + 'em"></div>').appendTo(this.$container).data('id', id);
  };

  /**
   * Set correct height of container
   */
  C.prototype.resize = function (e) {
    var self = this;
    // Make sure we use all the height we can get. Needed to scale up.
    if (this.$container === undefined) {
      // Not attached yet - nothing to resize....
      return;
    }

    // Check if decreasing iframe size
    var decreaseSize = e && e.data && e.data.decreaseSize;
    if (!decreaseSize) {
      this.$container.css('height', '99999px');
      self.$container.parents('.h5p-standalone.h5p-dragquestion').css('width', '');
    }

    var size = this.options.question.settings.size;
    var ratio = size.width / size.height;
    var parentContainer = this.$container.parent();
    // Use parent container as basis for resize.
    var width = parentContainer.width() - parseFloat(parentContainer.css('margin-left')) - parseFloat(parentContainer.css('margin-right'));

    // Check if we need to apply semi full screen fix.
    var $semiFullScreen = self.$container.parents('.h5p-standalone.h5p-dragquestion.h5p-semi-fullscreen');
    if ($semiFullScreen.length) {
      // Reset semi fullscreen width
      $semiFullScreen.css('width', '');

      // Decrease iframe size
      if (!decreaseSize) {
        self.$container.css('width', '10px');
        $semiFullScreen.css('width', '');

        // Trigger changes
        setTimeout(function () {
          self.trigger('resize', {decreaseSize: true});
        }, 200);
      }

      // Set width equal to iframe parent width, since iframe content has not been update yet.
      var $iframe = $(window.frameElement);
      if ($iframe) {
        var $iframeParent = $iframe.parent();
        width = $iframeParent.width();
        $semiFullScreen.css('width', width + 'px');
      }
    }

    var height = width / ratio;

    // Set natural size if no parent width
    if (width <= 0) {
      width = size.width;
      height = size.height;
    }

    this.$container.css({
      width: width + 'px',
      height: height + 'px',
      fontSize: (16 * (width / size.width)) + 'px'
    });
  };

  /**
   * Get css position in percentage.
   *
   * @param {jQuery} $container
   * @param {jQuery} $element
   * @returns {Object} CSS position
   */
  C.positionToPercentage = function ($container, $element) {
    return {
      top: (parseInt($element.css('top')) * 100 / $container.innerHeight()) + '%',
      left: (parseInt($element.css('left')) * 100 / $container.innerWidth()) + '%'
    };
  };

  /**
   * Disables all draggables.
   * @public
   */
  C.prototype.disableDraggables = function () {
    this.draggables.forEach(function (draggable) {
      draggable.disable();
    });
  };

  /**
   * Enables all draggables.
   * @public
   */
  C.prototype.enableDraggables = function () {

    this.draggables.forEach(function (draggable) {
      draggable.enable();
    });
  };

  /**
   * Shows the correct solutions on the boxes and disables input and buttons depending on settings.
   * @public
   * @params {Boolean} skipVisuals Skip visual animations.
   */
  C.prototype.showAllSolutions = function (skipVisuals) {
    this.points = 0;
    this.rawPoints = 0;

    for (var i = 0; i < this.draggables.length; i++) {
      var draggable = this.draggables[i];
      if (draggable === undefined) {
        continue;
      }

      //Disable all draggables in check mode.
      if (!skipVisuals) {
        draggable.disable();
      }

      // Find out where we are.
      this.points += draggable.results(skipVisuals, this.correctDZs[i]);
      this.rawPoints += draggable.rawPoints;
    }

    if (this.points < 0) {
      this.points = 0;
    }
    if (!this.answered && this.blankIsCorrect) {
      this.points = this.weight;
    }
    if (this.options.behaviour.singlePoint) {
      this.points = (this.points === this.calculateMaxScore() ? 1 : 0);
    }

    if (!skipVisuals) {
      this.hideButton('check-answer');
    }

    if (this.options.behaviour.enableRetry && !skipVisuals) {
      this.showButton('try-again');
    }

    if (this.hasButton('check-answer') && (this.options.behaviour.enableRetry === false || this.points === this.getMaxScore())) {
      // Max score reached, or the user cannot try again.
      this.hideButton('try-again');
    }
  };

  /**
   * Display the correct solutions, hides button and disables input.
   * Used in contracts.
   * @public
   */
  C.prototype.showSolutions = function () {
    this.showAllSolutions();
    //Hide solution button:
    this.hideButton('check-answer');
    this.hideButton('try-again');

    //Disable dragging during "solution" mode
    this.disableDraggables();
  };

  /**
   * Resets the task.
   * Used in contracts.
   * @public
   */
  C.prototype.resetTask = function () {
    this.points = 0;
    this.rawPoints = 0;
    this.answered = false;

    //Enables Draggables
    this.enableDraggables();

    //Reset position and feedback.
    this.draggables.forEach(function (draggable) {
      draggable.resetPosition();
    });

    //Show solution button
    this.showButton('check-answer');
    this.hideButton('try-again');
    this.setFeedback();

  };

  /**
   * Calculates the real max score.
   *
   * @returns {Number} Max points
   */
  C.prototype.calculateMaxScore = function () {
    var max = 0;

    var elements = this.options.question.task.elements;
    for (var i = 0; i < elements.length; i++) {
      var correctDropZones = this.correctDZs[i];

      if (correctDropZones === undefined || !correctDropZones.length) {
        continue;
      }

      if (elements[i].multiple) {
        max += correctDropZones.length;
      }
      else {
        max++;
      }
    }

    this.rawMax = max;
    if (this.blankIsCorrect) {
      return this.weight;
    }

    return max;
  };

  /**
   * Get maximum score.
   *
   * @returns {Number} Max points
   */
  C.prototype.getMaxScore = function () {
    return (this.options.behaviour.singlePoint ? this.weight : this.calculateMaxScore());
  };

  /**
   * Count the number of correct answers.
   * Only works while showing solution.
   *
   * @returns {Number} Points
   */
  C.prototype.getScore = function () {
    this.showAllSolutions(true);
    var points = this.points;
    delete this.points;
    return points;
  };

  /**
   * Checks if all has been answered.
   *
   * @returns {Boolean}
   */
  C.prototype.getAnswerGiven = function () {
    return !this.options.behaviour.showSolutionsRequiresInput || this.answered || this.blankIsCorrect;
  };

  /**
   * Shows the score to the user when the score button i pressed.
   */
  C.prototype.showScore = function () {
    var maxScore = this.calculateMaxScore();
    if (this.options.behaviour.singlePoint) {
      maxScore = 1;
    }
    var scoreText = this.options.feedback.replace('@score', this.points).replace('@total', maxScore);
    this.setFeedback(scoreText, this.points, maxScore);
  };

  /**
   * Packs info about the current state of the task into a object for
   * serialization.
   *
   * @public
   * @returns {object}
   */
  C.prototype.getCurrentState = function () {
    var state = {answers: []};
    for (var i = 0; i < this.draggables.length; i++) {
      var draggable = this.draggables[i];
      if (draggable === undefined) {
        continue;
      }

      var draggableAnswers = [];
      for (var j = 0; j < draggable.elements.length; j++) {
        var element = draggable.elements[j];
        if (element === undefined || element.dropZone === undefined) {
          continue;
        }

        // Store position and drop zone.
        draggableAnswers.push({
          x: Number(element.position.left.replace('%', '')),
          y: Number(element.position.top.replace('%', '')),
          dz: element.dropZone
        });
      }

      if (draggableAnswers.length) {
        // Add answers to state object for storage
        state.answers[i] = draggableAnswers;
      }
    }

    return state;
  };

  /**
   * Gather copyright information for the current content.
   *
   * @returns {H5P.ContentCopyright}
   */
  C.prototype.getCopyrights = function () {
    var self = this;
    var info = new H5P.ContentCopyrights();

    var background = self.options.question.settings.background;
    if (background !== undefined && background.copyright !== undefined) {
      var image = new H5P.MediaCopyright(background.copyright);
      image.setThumbnail(new H5P.Thumbnail(H5P.getPath(background.path, self.id), background.width, background.height));
      info.addMedia(image);
    }

    for (var i = 0; i < self.options.question.task.elements.length; i++) {
      var element = self.options.question.task.elements[i];
      var instance = H5P.newRunnable(element.type, self.id);

      if (instance.getCopyrights !== undefined) {
        var rights = instance.getCopyrights();
        rights.setLabel((element.dropZones.length ? 'Draggable ' : 'Static ') + (element.type.params.contentName !== undefined ? element.type.params.contentName : 'element'));
        info.addContent(rights);
      }
    }

    return info;
  };

  C.prototype.getTitle = function() {
    return H5P.createTitle(this.options.question.settings.questionTitle);
  };

  /**
   * Makes element background, border and shadow transparent.
   *
   * @param {jQuery} $element
   * @param {String} property
   * @param {Number} opacity
   */
  C.setOpacity = function ($element, property, opacity) {
    if (property === 'background') {
      // Set both color and gradient.
      C.setOpacity($element, 'backgroundColor', opacity);
      C.setOpacity($element, 'backgroundImage', opacity);
      return;
    }

    opacity = (opacity === undefined ? 1 : opacity / 100);

    // Private. Get css properties objects.
    function getProperties(property, value) {
      switch (property) {
        case 'borderColor':
          return {
            borderTopColor: value,
            borderRightColor: value,
            borderBottomColor: value,
            borderLeftColor: value
          };

        default:
          var properties = {};
          properties[property] = value;
          return properties;
      }
    }

    var original = $element.css(property);

    // Reset css to be sure we're using CSS and not inline values.
    var properties = getProperties(property, '');
    $element.css(properties);

    // Determine prop and assume all props are the same and use the first.
    for (var prop in properties) {
      break;
    }

    // Get value from css
    var style = $element.css(prop);
    if (style === '' || style === 'none') {
      // No value from CSS, fall back to original
      style = original;
    }

    style = C.setAlphas(style, 'rgba(', opacity); // Update rgba
    style = C.setAlphas(style, 'rgb(', opacity); // Convert rgb

    $element.css(getProperties(property, style));
  };

  /**
   * Updates alpha channel for colors in the given style.
   *
   * @param {String} style
   * @param {String} prefix
   * @param {Number} alpha
   */
  C.setAlphas = function (style, prefix, alpha) {
    // Style undefined
    if (!style) {
      return;
    }
    var colorStart = style.indexOf(prefix);

    while (colorStart !== -1) {
      var colorEnd = style.indexOf(')', colorStart);
      var channels = style.substring(colorStart + prefix.length, colorEnd).split(',');

      // Set alpha channel
      channels[3] = (channels[3] !== undefined ? parseFloat(channels[3]) * alpha : alpha);

      style = style.substring(0, colorStart) + 'rgba(' + channels.join(',') + style.substring(colorEnd, style.length);

      // Look for more colors
      colorStart = style.indexOf(prefix, colorEnd);
    }

    return style;
  };

  /**
   * Creates a new draggable instance.
   * Makes it easier to keep track of all instance variables and elements.
   *
   * @class
   * @param {object} element
   * @param {number} id
   * @param {array} [answers] from last session
   */
  function Draggable(element, id, answers) {
    var self = this;
    H5P.EventDispatcher.call(this);

    self.$ = $(self);
    self.id = id;
    self.elements = [];
    self.x = element.x;
    self.y = element.y;
    self.width = element.width;
    self.height = element.height;
    self.backgroundOpacity = element.backgroundOpacity;
    self.dropZones = element.dropZones;
    self.type = element.type;
    self.multiple = element.multiple;

    if (answers) {
      if (self.multiple) {
        // Add base element
        self.elements.push({});
      }

      // Add answers
      for (var i = 0; i < answers.length; i++) {
        self.elements.push({
          dropZone: answers[i].dz,
          position: {
            left: answers[i].x + '%',
            top: answers[i].y + '%'
          }
        });
      }
    }
  }

  Draggable.prototype = Object.create(H5P.EventDispatcher.prototype);
  Draggable.prototype.constructor = Draggable;

  /**
   * Insert draggable elements into the given container.
   *
   * @param {jQuery} $container
   * @param {Number} contentId
   * @returns {undefined}
   */
  Draggable.prototype.appendTo = function ($container, contentId) {
    var self = this;

    if (!self.elements.length) {
      self.attachElement(null, $container, contentId);
    }
    else {
      for (var i = 0; i < self.elements.length; i++) {
        self.attachElement(i, $container, contentId);
      }
    }
  };

  /**
   * Attach the given element to the given container.
   *
   * @param {Number} index
   * @param {jQuery} $container
   * @param {Number} contentId
   * @returns {undefined}
   */
  Draggable.prototype.attachElement = function (index, $container, contentId) {
    var self = this;

    var element;
    if (index === null) {
      // Create new element
      element = {};
      self.elements.push(element);
      index = self.elements.length - 1;
    }
    else {
      // Get old element
      element = self.elements[index];
    }

    // Attach element
    element.$ = $('<div/>', {
      class: 'h5p-draggable',
      css: {
        left: self.x + '%',
        top: self.y + '%',
        width: self.width + 'em',
        height: self.height + 'em'
      },
      appendTo: $container
    })
      .draggable({
        revert: function (dropZone) {
          $container.removeClass('h5p-dragging');
          var $this = $(this);

          $this.removeClass('h5p-dropped').data("uiDraggable").originalPosition = {
            top: self.y + '%',
            left: self.x + '%'
          };
          C.setElementOpacity($this, self.backgroundOpacity);
          return !dropZone;
        },
        start: function(event, ui) {
          var $this = $(this);

          if (self.multiple && element.dropZone === undefined) {
            // Leave a new element for next drag
            self.attachElement(null, $container, contentId);
          }

          // Send element to the top!
          $this.removeClass('h5p-wrong').detach().appendTo($container);
          $container.addClass('h5p-dragging');
          C.setElementOpacity($this, self.backgroundOpacity);
        },
        stop: function(event, ui) {
          var $this = $(this);

          // Convert position to % to support scaling.
          element.position = C.positionToPercentage($container, $this);
          $this.css(element.position);

          var addToZone = $this.data('addToZone');
          if (addToZone !== undefined) {
            $this.removeData('addToZone');

            if (self.multiple) {
              // Check that we're the only element here
              for (var i = 0; i < self.elements.length; i++) {
                if (i !== index && self.elements[i] !== undefined && self.elements[i].dropZone === addToZone) {
                  // Remove element
                  $this.remove();
                  delete self.elements[index];
                  return;
                }
              }
            }

            element.dropZone = addToZone;

            $this.addClass('h5p-dropped');
            C.setElementOpacity($this, self.backgroundOpacity);

            self.trigger('interacted');
          }
          else {
            if (self.multiple) {
              // Remove element
              $this.remove();
              delete self.elements[index];
            }
            else {
              // Reset position and drop zone.
              delete element.dropZone;
              delete element.position;
            }
          }
        }
      }).css('position', '');
    self.element = element;

    if (element.position) {
      // Restore last position
      element.$.css(element.position).addClass('h5p-dropped');
    }

    C.addHover(element.$, self.backgroundOpacity);
    H5P.newRunnable(self.type, contentId, element.$);

    // Update opacity when element is attached.
    setTimeout(function () {
      C.setElementOpacity(element.$, self.backgroundOpacity);
    }, 0);
  };

  /**
   * Check if this element can be dragged to the given drop zone.
   *
   * @param {Number} id
   * @returns {Boolean}
   */
  Draggable.prototype.hasDropZone = function (id) {
    var self = this;

    for (var i = 0; i < self.dropZones.length; i++) {
      if (parseInt(self.dropZones[i]) === id) {
        return true;
      }
    }

    return false;
  };

  /**
   * Resets the position of the draggable to its' original position.
   * @public
   */
  Draggable.prototype.resetPosition = function () {
    var self = this;

    this.elements.forEach(function (draggable) {
      //If the draggable is in a dropzone reset its' position and feedback.
      if (draggable.dropZone !== undefined) {
        var element = draggable.$;

        //Revert the button to initial position and then remove it.
        element.animate({
          left: self.x + '%',
          top: self.y + '%'
        }, function () {
          //Remove the draggable if it is an infinity draggable.
          if (self.multiple) {
            element.remove();
            //Delete the element from elements list to avoid a cluster of draggables on top of infinity draggable.
            if (self.elements.indexOf(draggable) >= 0) {
              delete self.elements[self.elements.indexOf(draggable)];
            }
          }
        });

        // Reset element style
        element.removeClass('h5p-wrong')
          .removeClass('h5p-correct')
          .removeClass('h5p-dropped')
          .css({
            border: '',
            background: ''
          });
        C.setElementOpacity(element, self.backgroundOpacity);
      }
    });
    // Draggable removed from dropzone.
    delete self.element.dropZone;
    // Reset style on initial element and delete the dropzone.
    self.element.$.removeClass('h5p-wrong')
      .removeClass('h5p-correct')
      .removeClass('h5p-dropped')
      .css({
        border: '',
        background: ''
      });
    C.setElementOpacity(self.element.$, self.backgroundOpacity);
  };

  /**
   * Check if the given draggable dom element is a part of this draggable.
   *
   * @param {Object} draggable
   * @returns {Boolean}
   */
  Draggable.prototype.is = function (draggable) {
    var self = this;

    for (var i = 0; i < self.elements.length; i++) {
      if (self.elements[i] !== undefined && self.elements[i].$.is(draggable)) {
        return true;
      }
    }

    return false;
  };

  /**
   * Detemine if any of our elements is in the given drop zone.
   *
   * @param {Number} id
   * @returns {Boolean}
   */
  Draggable.prototype.isInDropZone = function (id) {
    var self = this;

    for (var i = 0; i < self.elements.length; i++) {
      if (self.elements[i] !== undefined && self.elements[i].dropZone === id) {
        return true;
      }
    }

    return false;
  };

  /**
   * Disables the draggable.
   * @public
   */
  Draggable.prototype.disable = function () {
    var self = this;

    for (var i = 0; i < self.elements.length; i++) {
      if (self.elements[i] !== undefined) {
        self.elements[i].$.draggable('disable');
      }
    }
  };

  /**
   * Enables the draggable.
   * @public
   */
  Draggable.prototype.enable = function () {
    var self = this;

    for (var i = 0; i < self.elements.length; i++) {
      if (self.elements[i] !== undefined) {
        self.elements[i].$.draggable('enable');
      }
    }
  };

  /**
   * Calculate score for this draggable.
   *
   * @param {Boolean} skipVisuals
   * @param {Array} solutions
   * @returns {Number}
   */
  Draggable.prototype.results = function (skipVisuals, solutions) {
    var self = this;
    var i, j, element, dropZone, correct, points = 0;
    self.rawPoints = 0;

    if (solutions === undefined) {
      // We should not be anywhere.
      for (i = 0; i < self.elements.length; i++) {
        element = self.elements[i];
        if (element !== undefined && element.dropZone !== undefined) {
          // ... but we are!
          if (skipVisuals !== true) {
            element.$.addClass('h5p-wrong');
            C.setElementOpacity(element.$, self.backgroundOpacity);
          }
          points--;
        }
      }
      return points;
    }

    // Are we somewhere we should be?
    for (i = 0; i < self.elements.length; i++) {
      element = self.elements[i];

      if (element === undefined || element.dropZone === undefined) {
        continue; // We have not been placed anywhere, we're neither wrong nor correct.
      }

      correct = false;
      for (j = 0; j < solutions.length; j++) {
        if (element.dropZone === solutions[j]) {
          // Yepp!
          if (skipVisuals !== true) {
            element.$.addClass('h5p-correct').draggable('disable');
            C.setElementOpacity(element.$, self.backgroundOpacity);
          }
          correct = true;
          self.rawPoints++;
          points++;
          break;
        }
      }

      if (!correct) {
        // Nope, we're in another zone
        if (skipVisuals !== true) {
          element.$.addClass('h5p-wrong');
          C.setElementOpacity(element.$, self.backgroundOpacity);
        }
        points--;
      }
    }

    return points;
  };

  /**
   * Creates a new drop zone instance.
   * Makes it easy to keep track of all instance variables.
   *
   * @param {Object} dropZone
   * @param {Number} id
   * @returns {_L8.DropZone}
   */
  function DropZone(dropZone, id) {
    var self = this;

    self.id = id;
    self.showLabel = dropZone.showLabel;
    self.label = dropZone.label;
    self.x = dropZone.x;
    self.y = dropZone.y;
    self.width = dropZone.width;
    self.height = dropZone.height;
    self.backgroundOpacity = dropZone.backgroundOpacity;
    self.tip = dropZone.tip;
    self.single = dropZone.single;
  }

  /**
   * Insert drop zone in the given container.
   *
   * @param {jQuery} $container
   * @param {Array} draggables
   * @returns {undefined}
   */
  DropZone.prototype.appendTo = function ($container, draggables) {
    var self = this;

    // Prepare inner html
    var html = '<div class="h5p-inner"></div>';
    var extraClass = '';
    if (self.showLabel) {
      html = '<div class="h5p-label">' + self.label + '</div>' + html;
      extraClass = ' h5p-has-label';
    }

    // Create drop zone element
    var $dropZone = $('<div/>', {
      class: 'h5p-dropzone' + extraClass,
      css: {
        left: self.x + '%',
        top: self.y + '%',
        width: self.width + 'em',
        height: self.height + 'em'
      },
      html: html
    })
      .appendTo($container)
      .children('.h5p-inner')
        .droppable({
          activeClass: 'h5p-active',
          tolerance: 'intersect',
          accept: function (draggable) {
            var element;

            for (var i = 0; i < draggables.length; i++) {
              if (draggables[i] === undefined) {
                continue;
              }
              if (self.single && draggables[i].isInDropZone(self.id)) {
                // This drop zone is already occupied!
                return false;
              }
              if (draggables[i].is(draggable)) {
                // Found the draggable's instance
                element = draggables[i];
                if (!self.single) {
                  break;
                }
              }
            }

            if (element === undefined) {
              return;
            }

            // Check to see if the draggable can be dropped in this zone
            return element.hasDropZone(self.id);
          },
          drop: function (event, ui) {
            var $this = $(this);
            C.setOpacity($this.removeClass('h5p-over'), 'background', self.backgroundOpacity);
            ui.draggable.data('addToZone', self.id);
          },
          over: function (event, ui) {
            C.setOpacity($(this).addClass('h5p-over'), 'background', self.backgroundOpacity);
          },
          out: function (event, ui) {
            C.setOpacity($(this).removeClass('h5p-over'), 'background', self.backgroundOpacity);
          }
        })
        .end();

    // Add tip after setOpacity(), so this does not get background opacity:
    if (self.tip !== undefined && self.tip.trim().length) {
      $dropZone.append(H5P.JoubelUI.createTip(self.tip));
    }

    // Set element opacity when element has been appended
    setTimeout(function () {
      C.setOpacity($dropZone.children('.h5p-label'), 'background', self.backgroundOpacity);
      C.setOpacity($dropZone.children('.h5p-inner'), 'background', self.backgroundOpacity);
    }, 0);
  };

  return C;
})(H5P.jQuery);
;
/**
 * Drag Text module
 * @external {jQuery} $ H5P.jQuery
 */
H5P.DragText = (function ($, Question) {
  //CSS Main Containers:
  var MAIN_CONTAINER = "h5p-drag";
  var INNER_CONTAINER = "h5p-drag-inner";
  var TASK_CONTAINER = "h5p-drag-task";
  var WORDS_CONTAINER = "h5p-drag-droppable-words";
  var DROPZONE_CONTAINER = "h5p-drag-dropzone-container";
  var DRAGGABLES_CONTAINER = "h5p-drag-draggables-container";

  //Special Sub-containers:
  var DROPZONE = "h5p-drag-dropzone";
  var DRAGGABLE = "h5p-drag-draggable";
  var SHOW_SOLUTION_CONTAINER = "h5p-drag-show-solution-container";
  var DRAGGABLES_WIDE_SCREEN = 'h5p-drag-wide-screen';
  var DRAGGABLE_ELEMENT_WIDE_SCREEN = 'h5p-drag-draggable-wide-screen';

  //CSS Dropzone feedback:
  var CORRECT_FEEDBACK = 'h5p-drag-correct-feedback';
  var WRONG_FEEDBACK = 'h5p-drag-wrong-feedback';

  //CSS Draggable feedback:
  var DRAGGABLE_DROPPED = 'h5p-drag-dropped';
  var DRAGGABLE_FEEDBACK_CORRECT = 'h5p-drag-draggable-correct';
  var DRAGGABLE_FEEDBACK_WRONG = 'h5p-drag-draggable-wrong';

  /**
   * Initialize module.
   *
   * @class H5P.DragText
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {Number} contentId Content identification
   * @param {Object} contentData Object containing task specific content data
   *
   * @returns {Object} DragText Drag Text instance
   */
  function DragText(params, contentId, contentData) {
    this.$ = $(this);
    this.contentId = contentId;
    Question.call(this, 'drag-text');



	//h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
	
    // Set default behavior.
    this.params = $.extend({}, {
      taskDescription: "",
      textField: "",
      checkAnswer: "Check",
      tryAgain: "Retry",
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol,
        instantFeedback: false
      },
      score: "Score : @score of @total.",
      showSolution : "Show Solution"
    }, params);

    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined && this.contentData.previousState.length !== undefined) {
      this.previousState = this.contentData.previousState;
    }

    // Keeps track of if Question has been answered
    this.answered = false;

    // Init drag text task
    this.initDragText();

    this.on('resize', this.resize, this);
  }

  DragText.prototype = Object.create(Question.prototype);
  DragText.prototype.constructor = DragText;

  /**
   * Registers this question type's DOM elements before they are attached.
   * Called from H5P.Question.
   */
  DragText.prototype.registerDomElements = function () {
    // Register task introduction text
    this.setIntroduction('<p>' + this.params.taskDescription + '</p>');

    // Register task content area
    this.setContent(this.$inner);

    // Register buttons
    this.addButtons();
  };

  /**
   * Initialize drag text task
   */
  DragText.prototype.initDragText = function () {
    this.$inner = $('<div/>', {
      class: INNER_CONTAINER
    });

    // Create task
    this.addTaskTo(this.$inner);

    // Set stored user state
    this.setH5PUserState();
	
    return this.$inner;
  };

  /**
   * Changes layout responsively when resized.
   * @public
   */
  DragText.prototype.resize = function () {
    this.changeLayoutToFitWidth();
  };

  /**
  * Adds the draggables on the right side of the screen if widescreen is detected.
  * @public
  */
  DragText.prototype.changeLayoutToFitWidth = function () {
    var self = this;
    self.addDropzoneWidth();

    //Find ratio of width to em, and make sure it is less than the predefined ratio, make sure widest draggable is less than a third of parent width.
    if ((self.$inner.width() / parseFloat(self.$inner.css("font-size"), 10) > 43) && (self.widestDraggable <= (self.$inner.width() / 3))) {
      // Adds a class that floats the draggables to the right.
      self.$draggables.addClass(DRAGGABLES_WIDE_SCREEN);
      // Detach and reappend the wordContainer so it will fill up the remaining space left by draggables.
      //Commented h5pcustomize
      //self.$wordContainer.detach().appendTo(self.$taskContainer);
      //added - To put draggable words down
      self.$wordContainer.detach().prependTo(self.$taskContainer);
      
      // Set margin so the wordContainer does not expand when there are no more draggables left.
      self.$wordContainer.css({'margin-right': self.widestDraggable});
      // Set all draggables to be blocks
      self.draggables.forEach(function (draggable) {
        draggable.getDraggableElement().addClass(DRAGGABLE_ELEMENT_WIDE_SCREEN);
      });
    } else {
      // Remove the specific wide screen settings.
      self.$wordContainer.css({'margin-right': 0});
      self.$draggables.removeClass(DRAGGABLES_WIDE_SCREEN);
      self.$draggables.detach().appendTo(self.$taskContainer);
      self.draggables.forEach(function (draggable) {
        draggable.getDraggableElement().removeClass(DRAGGABLE_ELEMENT_WIDE_SCREEN);
      });
    }
  };

  /**
   * Add check solution, show solution and retry buttons, and their functionality.
   * @public
   */
  DragText.prototype.addButtons = function () {
    var self = this;

    // Checking answer button
    self.addButton('check-answer', self.params.checkAnswer, function () {
      if (!self.showEvaluation()) {
        if (self.params.behaviour.enableRetry) {
          self.showButton('try-again');
        }
        if (self.params.behaviour.enableSolutionsButton) {
          self.showButton('show-solution');
        }
        self.hideButton('check-answer');
        self.disableDraggables();
      } else {
        self.hideButton('show-solution');
        self.hideButton('try-again');
        self.hideButton('check-answer');
      }
    }, !self.params.behaviour.instantFeedback);

    //Retry button
    self.addButton('try-again', self.params.tryAgain, function () {
      // Reset and shuffle draggables if Question is answered
      if (self.answered) {
        self.resetDraggables();
        self.addDraggablesRandomly(self.$draggables);
      }
      self.answered = false;

      self.hideEvaluation();

      self.hideButton('try-again');
      self.hideButton('show-solution');

      if (self.params.behaviour.instantFeedback) {
        self.enableAllDropzonesAndDraggables();
      } else {
        self.showButton('check-answer');
        self.enableDraggables();
      }
      self.hideAllSolutions();
    }, self.initShowTryAgainButton || false);

    //Show Solution button
    self.addButton('show-solution', self.params.showSolution, function () {
      self.droppables.forEach(function (droppable) {
        droppable.showSolution();
      });
      self.disableDraggables();
      self.hideButton('show-solution');
    }, self.initShowShowSolutionButton || false);
  };

  /**
   * Shows feedback for dropzones.
   * @public
   */
  DragText.prototype.showDropzoneFeedback = function () {
    this.droppables.forEach(function (droppable) {
      droppable.addFeedback();
    });
  };

  /**
   * Evaluate task and display score text for word markings.
   *
   * @return {Boolean} Returns true if maxScore was achieved.
   */
  DragText.prototype.showEvaluation = function () {
    this.hideEvaluation();
    this.calculateScore();
    this.showDropzoneFeedback();

    var score = this.correctAnswers;
    var maxScore = this.droppables.length;

    this.triggerXAPIScored(score, maxScore, 'answered');

    var scoreText = this.params.score.replace(/@score/g, score.toString())
      .replace(/@total/g, maxScore.toString());

    if (score === maxScore) {
      //Hide buttons and disable task
      this.hideButton('check-answer');
      this.hideButton('show-solution');
      this.hideButton('try-again');
      this.disableDraggables();
    }
    this.trigger('resize');

    // Set feedback score
    this.setFeedback(scoreText, score, maxScore);

    return score === maxScore;
  };

  /**
   * Calculate score and store them in class variables.
   * @public
   */
  DragText.prototype.calculateScore = function () {
    var self = this;
    self.correctAnswers = 0;
    self.droppables.forEach(function (entry) {
      if (entry.isCorrect()) {
        self.correctAnswers += 1;
      }
    });
  };

  /**
   * Clear the evaluation text.
   */
  DragText.prototype.hideEvaluation = function () {
    this.setFeedback();
    this.trigger('resize');
  };

  /**
   * Hides solution text for all dropzones.
   */
  DragText.prototype.hideAllSolutions = function () {
    this.droppables.forEach(function (droppable) {
      droppable.hideSolution();
    });
    this.trigger('resize');
  };

  /**
   * Handle task and add it to container.
   * @public
   * @param {jQuery} $container The object which our task will attach to.
   */
  DragText.prototype.addTaskTo = function ($container) {
    var self = this;
    self.widest = 0;
    self.widestDraggable = 0;
    self.droppables = [];
    self.draggables = [];

    self.$taskContainer = $('<div/>', {
      'class': TASK_CONTAINER
    });

    self.$draggables = $('<div/>', {
      'class': DRAGGABLES_CONTAINER
    });

    self.$wordContainer = $('<div/>', {'class': WORDS_CONTAINER});
    self.handleText();

    self.addDraggablesRandomly(self.$draggables);
    self.$wordContainer.appendTo(self.$taskContainer);
    self.$draggables.appendTo(self.$taskContainer);
    self.$taskContainer.appendTo($container);
    self.addDropzoneWidth();
  };

  /**
   * Parses the text and sends identified dropzones to the addDragNDrop method for further handling.
   * Appends the parsed text to wordContainer.
   * @public
   */
  DragText.prototype.handleText = function () {
    var self = this;

    //Replace newlines with break line tag
    var textField = self.params.textField.replace(/(\r\n|\n|\r)/gm, "<br/>");

    // Go through the text and replace all the asterisks with input fields
    var dropStart = textField.indexOf('*');
    var dropEnd = -1;
    var currentIndex = 0;
    //While the start of a dropbox is found
    while (dropStart !== -1) {
      dropStart += 1;
      dropEnd = textField.indexOf('*', dropStart);
      if (dropEnd === -1) {
        dropStart = -1;
      } else {
        //Appends the text between each dropzone
        self.$wordContainer.append(textField.slice(currentIndex, dropStart - 1));

        //Adds the drag n drop functionality when an answer is found
        self.addDragNDrop(textField.substring(dropStart, dropEnd));
        dropEnd += 1;
        currentIndex = dropEnd;

        //Attempts to find the beginning of the next answer.
        dropStart = textField.indexOf('*', dropEnd);
      }
    }
    //Appends the remaining part of the text.
    self.$wordContainer.append(textField.slice(currentIndex, textField.length));
  };

  /**
   * Matches the width of all dropzones to the widest draggable, and sets widest class variable.
   * @public
   */
  DragText.prototype.addDropzoneWidth = function () {
    var self = this;
    var widest = 0;
    var widestDragagble = 0;
    var fontSize = parseInt(this.$inner.css('font-size'), 10);
    var staticMinimumWidth = 3 * fontSize;
    var staticPadding = fontSize; // Needed to make room for feedback icons

    //Find widest draggable
    this.draggables.forEach(function (draggable) {
      var $draggableElement = draggable.getDraggableElement();

      //Find the initial natural width of the draggable.
      var $tmp = $draggableElement.clone().css({
        'position': 'absolute',
        'white-space': 'nowrap',
        'width': 'auto',
        'padding': 0,
        'margin': 0
      }).html(draggable.getAnswerText())
        .appendTo($draggableElement.parent());
      var width = $tmp.width();

      widestDragagble = width > widestDragagble ? width : widestDragagble;

      // Measure how big truncated draggable should be
      if ($tmp.text().length >= 20) {
        $tmp.html(draggable.getShortFormat());
        width = $tmp.width();
      }

      if (width + staticPadding > widest) {
        widest = width + staticPadding;
      }
      $tmp.remove();
    });
    // Set min size
    if (widest < staticMinimumWidth) {
      widest = staticMinimumWidth;
    }
    this.widestDraggable = widestDragagble;
    this.widest = widest;

    //Adjust all droppable to widest size.
    this.droppables.forEach(function (droppable) {
    
      droppable.getDropzone().width(self.widest);
    });
    //h5pcustomize override width
    $(".h5p-drag-dropped").parent().css("width","auto");
  };

  /**
   * Makes a drag n drop from the specified text.
   * @public
   * @param {String} text Text for the drag n drop.
   */
  DragText.prototype.addDragNDrop = function (text) {
    var self = this;
    var tip;
    var answer = text;
    var answersAndTip = answer.split(':');

    if (answersAndTip.length > 0) {
      answer = answersAndTip[0];
      tip = answersAndTip[1];
    }

    //Make the draggable
    var $draggable = $('<div/>', {
      html: answer,
      'class': DRAGGABLE
    }).draggable({
      revert: function (isValidDrop) { 
        var dropzone = droppable;
        if (!isValidDrop) {
          if (!self.$draggables.children().length) {
            // Show draggables container
            self.$draggables.removeClass('hide');
          }

          self.moveDraggableToDroppable(draggable, null);
          return false;
        }
        if (self.params.behaviour.instantFeedback) {
          if (dropzone !== null) {
            dropzone.addFeedback();
          }
          self.instantFeedbackEvaluation();
        }
		 //h5pcustomize
		 if($(".h5p-presentation-wrapper").size() == 0) //only for video
		 {
		 	event.target.style = "width:auto;";
		 	$(".h5p-drag-dropped").parent().css("width","auto");
		 }        
        return false;
      },
      containment: self.$taskContainer
    });

    var draggable = new Draggable(answer, $draggable);
    draggable.on('addedToZone', function (event) {
      self.triggerXAPI('interacted');
    });

    //Make the dropzone
    var $dropzoneContainer = $('<div/>', {
      'class': DROPZONE_CONTAINER
    });
    var $dropzone = $('<div/>', {
      'class': DROPZONE
    }).appendTo($dropzoneContainer)
      .droppable({
        tolerance: 'pointer',
        drop: function (event, ui) {
        
          self.draggables.forEach(function (draggable) {
            if (draggable.getDraggableElement().is(ui.draggable)) {
              self.moveDraggableToDroppable(draggable, droppable);
              
            }
          });
          if (self.params.behaviour.instantFeedback) {
            droppable.addFeedback();
            if (!self.params.behaviour.enableRetry) {
              droppable.disableDropzoneAndContainedDraggable();
            }
            if (droppable.isCorrect()) {
              droppable.disableDropzoneAndContainedDraggable();
            }
          }
		  //h5pcustomize - adjust the width of the dropped zone based on the content
		  if($(".h5p-presentation-wrapper").size() == 0) //only for video
		  {
		  	event.target.style = "width:auto;";
		  	$(".h5p-drag-dropped").parent().css("width","auto");
		  }
		  
          // Hide draggables container if it is empty
          self.$draggables.toggleClass('hide', !self.$draggables.children().length);
        }
      });

    var droppable = new Droppable(answer, tip, $dropzone, $dropzoneContainer);
    droppable.appendDroppableTo(self.$wordContainer);

    self.draggables.push(draggable);
    self.droppables.push(droppable);
  };

  /**
   * Moves a draggable onto a droppable, and updates all parameters in the objects.
   * @public
   * @param {Draggable} draggable Draggable instance.
   * @param {Droppable} droppable The droppable instance the draggable is put on.
   */
  DragText.prototype.moveDraggableToDroppable = function (draggable, droppable) {
    draggable.removeFromZone();
    if (droppable !== null) {
      this.answered = true;
      droppable.appendInsideDroppableTo(this.$draggables);
      droppable.setDraggable(draggable);
      draggable.appendDraggableTo(droppable.getDropzone());
    } else {
      draggable.revertDraggableTo(this.$draggables);
    }
    this.trigger('resize');
  };

  /**
   * Adds the draggable words to the provided container in random order.
   * @public
   * @param {jQuery} $container Container the draggables will be added to.
   */
  DragText.prototype.addDraggablesRandomly = function ($container) {
    var tempArray = this.draggables.slice();
    var randIndex = 0;
    while (tempArray.length >= 1) {
      randIndex = parseInt(Math.random() * tempArray.length, 10);
      tempArray[randIndex].appendDraggableTo($container);
      tempArray.splice(randIndex, 1);
    }
  };

  /**
   * Feedback function for checking if all fields are filled, and show evaluation if that is the case.
   */
  DragText.prototype.instantFeedbackEvaluation = function () {
    var self = this;
    var allFilled = self.isAllAnswersFilled();

    if (allFilled) {
      //Shows "retry" and "show solution" buttons.
      if (self.params.behaviour.enableSolutionsButton) {
        self.showButton('show-solution');
      }
      if (self.params.behaviour.enableRetry) {
        self.showButton('try-again');
      }

      // Shows evaluation text
      self.showEvaluation();
    } else {
      //Hides "retry" and "show solution" buttons.
      self.hideButton('try-again');
      self.hideButton('show-solution');

      //Hides evaluation text.
      self.hideEvaluation();
    }
  };

  /**
   * Check if all answers are filled
   * @returns {boolean} allFilled Returns true if all answers are answered
   */
  DragText.prototype.isAllAnswersFilled = function () {
    var self = this;
    var allFilled = true;
    self.draggables.forEach(function (entry) {
      if (entry.insideDropzone === null) {
        allFilled = false;
      }
    });

    return allFilled;
  };

  /**
   * Enables all dropzones and all draggables.
   */
  DragText.prototype.enableAllDropzonesAndDraggables = function () {
    this.enableDraggables();
    this.droppables.forEach(function (droppable) {
      droppable.enableDropzone();
    });
  };

  /**
   * Disables all draggables, user will not be able to interact with them any more.
   * @public
   */
  DragText.prototype.disableDraggables = function () {
    this.draggables.forEach(function (entry) {
      entry.disableDraggable();
    });
  };

  /**
   * Enables all draggables, user will be able to interact with them again.
   * @public
   */
  DragText.prototype.enableDraggables = function () {
    this.draggables.forEach(function (entry) {
      entry.enableDraggable();
    });
  };

  /**
   * Used for contracts.
   * Checks if the parent program can proceed. Always true.
   * @public
   * @returns {Boolean} true
   */
  DragText.prototype.getAnswerGiven = function () {
    return this.answered;
  };

  /**
   * Used for contracts.
   * Checks the current score for this task.
   * @public
   * @returns {Number} The current score.
   */
  DragText.prototype.getScore = function () {
    this.calculateScore();
    return this.correctAnswers;
  };

  /**
   * Used for contracts.
   * Checks the maximum score for this task.
   * @public
   * @returns {Number} The maximum score.
   */
  DragText.prototype.getMaxScore = function () {
    return this.droppables.length;
  };

  /**
   * Get title of task
   * @returns {string} title
   */
  DragText.prototype.getTitle = function () {
    return H5P.createTitle(this.params.taskDescription);
  };

  /**
   * Used for contracts.
   * Sets feedback on the dropzones.
   * @public
   */
  DragText.prototype.showSolutions = function () {
    this.droppables.forEach(function (droppable) {
      droppable.addFeedback();
      droppable.showSolution();
    });
    this.disableDraggables();
    //Remove all buttons in "show solution" mode.
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.hideButton('check-answer');
    this.trigger('resize');
  };

  /**
   * Used for contracts.
   * Resets the complete task back to its' initial state.
   * @public
   */
  DragText.prototype.resetTask = function () {
    var self = this;
    // Reset task answer
    self.answered = false;
    //Reset draggables parameters and position
    self.resetDraggables();
    //Hides solution text and re-enable draggables
    self.hideEvaluation();
    self.enableAllDropzonesAndDraggables();
    //Show and hide buttons
    self.hideButton('try-again');
    self.hideButton('show-solution');

    if (!self.params.behaviour.instantFeedback) {
      self.showButton('check-answer');
    }
    self.hideAllSolutions();
    this.trigger('resize');
  };

  /**
   * Resets the position of all draggables.
   */
  DragText.prototype.resetDraggables = function () {
    var self = this;
    // Show draggables container
    self.$draggables.removeClass('hide');
    self.draggables.forEach(function (entry) {
      self.moveDraggableToDroppable(entry, null);
    });
    this.trigger('resize');
  };

  /**
   * Returns an object containing the dropped words
   * @returns {object} containing indexes of dropped words
   */
  DragText.prototype.getCurrentState = function () {
    var self = this;
    var draggedDraggablesIndexes = [];

    // Return undefined if task is not initialized
    if (this.draggables === undefined) {
      return undefined;
    }

    // Find draggables that has been dropped
    this.draggables.forEach(function (draggable, draggableIndex) {
      if (draggable.getInsideDropzone() !== null) {
        draggedDraggablesIndexes.push({draggable: draggableIndex, droppable: self.droppables.indexOf(draggable.getInsideDropzone())});
      }
    });
    return draggedDraggablesIndexes;
  };

  /**
   * Sets answers to current user state
   */
  DragText.prototype.setH5PUserState = function () {
    var self = this;

    // Do nothing if user state is undefined
    if (this.previousState === undefined) {
      return;
    }
     
    // Select words from user state
    //h5pcustomize. if retry is disabled, do not allow user to change
    if(self.params.behaviour.enableRetry == false && this.previousState.length > 0) // >0 attempted
    	self.params.behaviour.instantFeedback = true;
    this.previousState.forEach(function (draggedDraggableIndexes) {
      var draggableIndexIsInvalid = isNaN(draggedDraggableIndexes.draggable) ||
        draggedDraggableIndexes.draggable >= self.draggables.length ||
        draggedDraggableIndexes.draggable < 0;

      var droppableIndexIsInvalid = isNaN(draggedDraggableIndexes.droppable) ||
        draggedDraggableIndexes.droppable >= self.droppables.length ||
        draggedDraggableIndexes.droppable < 0;

      if (draggableIndexIsInvalid || droppableIndexIsInvalid) {
        throw new Error('Stored user state is invalid');
      }

      var moveDraggable = self.draggables[draggedDraggableIndexes.draggable];
      var moveToDroppable = self.droppables[draggedDraggableIndexes.droppable];
      self.moveDraggableToDroppable(moveDraggable, moveToDroppable);
      
      if (self.params.behaviour.instantFeedback) {
        // Add feedback to dropzone
        if (moveToDroppable !== null) {
          moveToDroppable.addFeedback();
        }

        // Add feedback to draggable
        if (moveToDroppable.isCorrect()) {
          moveToDroppable.disableDropzoneAndContainedDraggable();
        }
      }
    });

    // Show evaluation if task is finished
    if (self.params.behaviour.instantFeedback) {

      // Show buttons if not max score and all answers filled
      if (self.isAllAnswersFilled() && !self.showEvaluation()) {

        //Shows "retry" and "show solution" buttons.
        if (self.params.behaviour.enableSolutionsButton) {
          self.initShowShowSolutionButton = true;
        }
        if (self.params.behaviour.enableRetry) {
          self.initShowTryAgainButton = true;
        }
      }
    }
  };

  /**
   * Private class for keeping track of draggable text.
   * @private
   * @param {String} text String that will be turned into a selectable word.
   * @param {jQuery} draggable Draggable object.
   */
  function Draggable(text, draggable) {
    H5P.EventDispatcher.call(this);
    var self = this;
    self.text = text;
    self.insideDropzone = null;
    self.$draggable = $(draggable);

    self.shortFormat = self.text;
    //Shortens the draggable string if inside a dropbox.
    if (self.shortFormat.length > 20) {
      self.shortFormat = self.shortFormat.slice(0, 17) + '...';
    }
  }

  Draggable.prototype = Object.create(H5P.EventDispatcher.prototype);
  Draggable.prototype.constructor = Draggable;

  /**
   * Moves the draggable to the provided container.
   * @public
   * @param {jQuery} $container Container the draggable will append to.
   */
  Draggable.prototype.appendDraggableTo = function ($container) {
    this.$draggable.detach().css({left: 0, top: 0}).appendTo($container);
  };

  /**
   * Reverts the draggable to its' provided container.
   * @public
   * @params {jQuery} $container The parent which the draggable will revert to.
   */
  Draggable.prototype.revertDraggableTo = function ($container) {
    // get the relative distance between draggable and container.
    var offLeft = this.$draggable.offset().left - $container.offset().left;
    var offTop = this.$draggable.offset().top - $container.offset().top;

    // Prepend draggable to new container, but keep the offset,
    // then animate to new container's top:0, left:0
    this.$draggable.detach()
      .prependTo($container)
      .css({left: offLeft, top: offTop})
      .animate({left: 0, top: 0});
  };

  /**
   * Sets dropped feedback if the on the draggable if parameter is true.
   * @public
   * @params {Boolean} isDropped Decides whether the draggable has been dropped.
   */
  Draggable.prototype.toggleDroppedFeedback = function (isDropped) {
    if (isDropped) {
      this.$draggable.addClass(DRAGGABLE_DROPPED);
    } else {
      this.$draggable.removeClass(DRAGGABLE_DROPPED);
    }
  };

  /**
   * Disables the draggable, making it immovable.
   * @public
   */
  Draggable.prototype.disableDraggable = function () {
    this.$draggable.draggable({ disabled: true});
  };

  /**
   * Enables the draggable, making it movable.
   * @public
   */
  Draggable.prototype.enableDraggable = function () {
    this.$draggable.draggable({ disabled: false});
  };

  /**
   * Gets the draggable jQuery object for this class.
   * @public
   *
   * @returns {jQuery} Draggable item.
   */
  Draggable.prototype.getDraggableElement = function () {
    return this.$draggable;
  };

  /**
   * Removes this draggable from its dropzone, if it is contained in one.
   * @public
   */
  Draggable.prototype.removeFromZone = function () {
    if (this.insideDropzone !== null) {
      this.insideDropzone.removeFeedback();
      this.insideDropzone.removeDraggable();
    }
    this.toggleDroppedFeedback(false);
    this.removeShortFormat();
    this.insideDropzone = null;
  };

  /**
   * Adds this draggable to the given dropzone.
   * @public
   * @param {Droppable} droppable The droppable this draggable will be added to.
   */
  Draggable.prototype.addToZone = function (droppable) {
    this.trigger('addedToZone');
    if (this.insideDropzone !== null) {
      this.insideDropzone.removeDraggable();
    }
    this.toggleDroppedFeedback(true);
    this.insideDropzone = droppable;
    this.setShortFormat();
  };

  /**
   * Gets the answer text for this draggable.
   * @public
   *
   * @returns {String} The answer text in this draggable.
   */
  Draggable.prototype.getAnswerText = function () {
    return this.text;
  };

  /**
   * Sets short format of draggable when inside a dropbox.
   * @public
   */
  Draggable.prototype.setShortFormat = function () {
    this.$draggable.html(this.shortFormat);
  };

  /**
   * Get short format of draggable when inside a dropbox.
   * @returns {String|*}
   */
  Draggable.prototype.getShortFormat = function () {
    return this.shortFormat;
  };

  /**
   * Removes the short format of draggable when it is outside a dropbox.
   * @public
   */
  Draggable.prototype.removeShortFormat = function () {
    this.$draggable.html(this.text);
  };

  /**
   * Get the droppable this draggable is inside
   * @returns {Droppable} Droppable
   */
  Draggable.prototype.getInsideDropzone = function () {
    return this.insideDropzone;
  };

  /**
   * Private class for keeping track of droppable zones.
   * @private
   *
   * @param {String} text Correct text string for this drop box.
   * @param {undefined/String} tip Tip for this container, optional.
   * @param {jQuery} dropzone Dropzone object.
   * @param {jQuery} dropzoneContainer Container Container for the dropzone.
   */
  function Droppable(text, tip, dropzone, dropzoneContainer) {
    var self = this;
    self.text = text;
    self.tip = tip;
    self.containedDraggable = null;
    self.$dropzone = $(dropzone);
    self.$dropzoneContainer = $(dropzoneContainer);

    if (self.tip !== undefined) {
      self.$dropzone.append(H5P.JoubelUI.createTip(self.tip, self.$dropzone));
    }

    self.$showSolution = $('<div/>', {
      'class': SHOW_SOLUTION_CONTAINER
    }).appendTo(self.$dropzoneContainer).hide();
  }

  /**
   * Displays the solution next to the drop box if it is not correct.
   * @public
   */
  Droppable.prototype.showSolution = function () {
    if (!((this.containedDraggable !== null) && (this.containedDraggable.getAnswerText() === this.text))) {
      this.$showSolution.html(this.text);
      this.$showSolution.show();
    }
  };

  /**
   * Hides the solution.
   * @public
   */
  Droppable.prototype.hideSolution = function () {
    this.$showSolution.html('');
    this.$showSolution.hide();
  };

  /**
   * Appends the droppable to the provided container.
   * @public
   * @param {jQuery} $container Container which the dropzone will be appended to.
   */
  Droppable.prototype.appendDroppableTo = function ($container) {
    this.$dropzoneContainer.appendTo($container);
  };
  /**
   * Appends the draggable contained within this dropzone to the argument.
   * @public
   * @param {jQuery} $container Container which the draggable will append to.
   */
  Droppable.prototype.appendInsideDroppableTo = function ($container) {
    if (this.containedDraggable !== null) {
      this.containedDraggable.revertDraggableTo($container);
    }
  };

  /**
   * Sets the contained draggable in this drop box to the provided argument.
   * @public
   * @param {Draggable} droppedDraggable A draggable that has been dropped on this box.
   */
  Droppable.prototype.setDraggable = function (droppedDraggable) {
    var self = this;
    if (self.containedDraggable === droppedDraggable) {
      return;
    }
    if (self.containedDraggable !== null) {
      self.containedDraggable.removeFromZone();
    }
    self.containedDraggable = droppedDraggable;
    droppedDraggable.addToZone(self);
  };

  /**
   * Removes the contained draggable in this box.
   * @public
   */
  Droppable.prototype.removeDraggable = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable = null;
    }
  };

  /**
   * Checks if this drop box contains the correct draggable.
   * @public
   *
   * @returns {Boolean} True if this box has the correct answer.
   */
  Droppable.prototype.isCorrect = function () {
    if (this.containedDraggable === null) {
      return false;
    }
    return this.containedDraggable.getAnswerText() === this.text;
  };

  /**
   * Sets CSS styling feedback for this drop box.
   * @public
   */
  Droppable.prototype.addFeedback = function () {
    //Draggable is correct
    if (this.isCorrect()) {
      this.$dropzone.removeClass(WRONG_FEEDBACK).addClass(CORRECT_FEEDBACK);

      //Draggable feedback
      this.containedDraggable.getDraggableElement().removeClass(DRAGGABLE_FEEDBACK_WRONG).addClass(DRAGGABLE_FEEDBACK_CORRECT);
    } else if (this.containedDraggable === null) {
      //Does not contain a draggable
      this.$dropzone.removeClass(WRONG_FEEDBACK).removeClass(CORRECT_FEEDBACK);
    } else {
      //Draggable is wrong
      this.$dropzone.removeClass(CORRECT_FEEDBACK).addClass(WRONG_FEEDBACK);

      //Draggable feedback
      if (this.containedDraggable !== null) {
        this.containedDraggable.getDraggableElement().addClass(DRAGGABLE_FEEDBACK_WRONG).removeClass(DRAGGABLE_FEEDBACK_CORRECT);
      }
    }
  };

  /**
   * Removes all CSS styling feedback for this drop box.
   * @public
   */
  Droppable.prototype.removeFeedback = function () {
    this.$dropzone.removeClass(WRONG_FEEDBACK).removeClass(CORRECT_FEEDBACK);

    //Draggable feedback
    if (this.containedDraggable !== null) {
      this.containedDraggable.getDraggableElement().removeClass(DRAGGABLE_FEEDBACK_WRONG).removeClass(DRAGGABLE_FEEDBACK_CORRECT);
    }
  };

  /**
   * Sets short format of draggable when inside a dropbox.
   * @public
   */
  Droppable.prototype.setShortFormat = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.setShortFormat();
    }
  };

  /**
   * Disables dropzone and the contained draggable.
   */
  Droppable.prototype.disableDropzoneAndContainedDraggable = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.disableDraggable();
    }
    this.$dropzone.droppable({ disabled: true});
  };

  /**
   * Enable dropzone.
   */
  Droppable.prototype.enableDropzone = function () {
    this.$dropzone.droppable({ disabled: false});
  };

  /**
   * Removes the short format of draggable when it is outside a dropbox.
   * @public
   */
  Droppable.prototype.removeShortFormat = function () {
    if (this.containedDraggable !== null) {
      this.containedDraggable.removeShortFormat();
    }
  };

  /**
   * Gets this object's dropzone jQuery object.
   * @public
   *
   * @returns {jQuery} This object's dropzone.
   */
  Droppable.prototype.getDropzone = function () {
    return this.$dropzone;
  };

  return DragText;

}(H5P.jQuery, H5P.Question));
;/*global H5P*/

/**
 * Mark The Words module
 * @external {jQuery} $ H5P.jQuery
 */
H5P.MarkTheWords = (function ($, Question) {
  // CSS Main Containers:
  var MAIN_CONTAINER = "h5p-word";
  var INNER_CONTAINER = "h5p-word-inner";
  var WORDS_CONTAINER = "h5p-word-selectable-words";
  var BUTTON_CONTAINER = "h5p-button-bar";

  // CSS Classes for marking words:
  var MISSED_MARK = "h5p-word-missed";
  var CORRECT_MARK = "h5p-word-correct";
  var WRONG_MARK = "h5p-word-wrong";
  var SELECTED_MARK = "h5p-word-selected";
  var SELECTABLE_MARK = "h5p-word-selectable";
  var WORD_DISABLED = "h5p-word-disabled";

  /**
   * Initialize module.
   *
   * @class H5P.MarkTheWords
   * @extends H5P.Question
   * @param {Object} params Behavior settings
   * @param {Number} contentId Content identification
   * @param {Object} contentData Object containing task specific content data
   *
   * @returns {Object} MarkTheWords Mark the words instance
   */
  function MarkTheWords(params, contentId, contentData) {
    this.contentId = contentId;

    Question.call(this, 'mark-the-words');
    
    //h5pcustomize
	var showSol = false;
	var retry = false;
	try{
	if(contentData != undefined && contentData.parent != undefined && contentData.parent.override != undefined && contentData.parent.override.overrideButtons !=undefined &&  contentData.parent.override.overrideButtons == "true" || contentData.parent.override.overrideButtons == true)
	{
	
		if(contentData.parent.override.overrideShowSolutionButton == "true" || contentData.parent.override.overrideShowSolutionButton == true)
		{
	
			showSol =true;
		}
		if(contentData.parent.override.overrideRetry == "true" || contentData.parent.override.overrideRetry == true)
		{
	
			retry =true;
		}
			
	} 
	}catch(e){}
	
	
    // Set default behavior.
    this.params = $.extend({}, {
      taskDescription: "",
      textField: "This is a *nice*, *flexible* content type.",
      behaviour: {
        enableRetry: retry,
        enableSolutionsButton: showSol
      },
      checkAnswerButton: "Check",
      tryAgainButton: "Retry",
      showSolutionButton: "Show solution",
      score: "You got @score of @total points."
    }, params);

    this.contentData = contentData;
    if (this.contentData !== undefined && this.contentData.previousState !== undefined) {
      this.previousState = this.contentData.previousState;
    }

    this.initMarkTheWords();
    
  }
;
  MarkTheWords.prototype = Object.create(H5P.EventDispatcher.prototype);
  MarkTheWords.prototype.constructor = MarkTheWords;

  /**
   * Initialize Mark The Words task
   */
  MarkTheWords.prototype.initMarkTheWords = function () {
    this.$inner = $('<div class=' + INNER_CONTAINER + '></div>');

    this.addTaskTo(this.$inner);

    // Set user state
    this.setH5PUserState();
  };

  /**
   * Recursive function that creates html for the words
   * @method createHtmlForWords
   * @param  {Array}           nodes Array of dom nodes
   * @return {string}
   */
  MarkTheWords.prototype.createHtmlForWords = function (nodes) {
    var self = this;
    var html = '';
    for (var i = 0; i < nodes.length; i++) {
      var node = nodes[i];

      if (node instanceof Text) {
        var text = $(node).text();
        var selectableStrings = text.replace(/(&nbsp;|\r\n|\n|\r)/g, ' ')
          .match(/ \*[^\*]+\* |[^\s]+/g);

        if (selectableStrings) {
          selectableStrings.forEach(function (entry) {
            entry = entry.trim();

            // Words
            if (html) {
              // Add space before
              html += ' ';
            }

            // Remove prefix punctuations from word
            var prefix = entry.match(/^[\[\({⟨¿¡"«„]+/);
            var start = 0;
            if (prefix !== null) {
              start = prefix.length;
              html += prefix;
            }

            // Remove suffix punctuations from word
            var suffix = entry.match(/[",….:;?!\]\)}⟩»”]+$/);
            var end = entry.length - start;
            if (suffix !== null) {
              end -= suffix.length;
            }

            // Word
            entry = entry.substr(start, end);
            if (entry.length) {
              html += '<span class="' + SELECTABLE_MARK + '">' + entry + '</span>';
            }

            if (suffix !== null) {
              html += suffix;
            }
          });
        }
        else if ((selectableStrings !== null) && text.length) {
          html += '<span class="' + SELECTABLE_MARK + '">' + text + '</span>';
        }
      }
      else {
        if (node.nodeName === 'BR') {
          html += '<br/>';
        }
        else {
          var attributes = ' ';
          for (var j = 0; j < node.attributes.length; j++) {
            attributes +=node.attributes[j].name + '="' + node.attributes[j].nodeValue + '" ';
          }
          html += '<' + node.nodeName +  attributes + '>';
          html += self.createHtmlForWords(node.childNodes);
          html += '</' + node.nodeName + '>';
        }
      }
    }

    return html;
  };

  /**
   * Handle task and add it to container.
   * @param {jQuery} $container The object which our task will attach to.
   */
  MarkTheWords.prototype.addTaskTo = function ($container) {
    var self = this;
    self.selectableWords = [];
    self.answers = 0;

    // Wrapper
    var $wordContainer = $('<div/>', {
      'class': WORDS_CONTAINER,
      html: self.createHtmlForWords($.parseHTML(self.params.textField))
    });

    $wordContainer.find('.' + SELECTABLE_MARK).each(function () {
      var selectableWord = new Word($(this));
      selectableWord.on('xAPI', function (event) {
        if (event.getVerb() === 'interacted') {
          self.triggerXAPI('interacted');
        }
      });
      if (selectableWord.isAnswer()) {
        self.answers += 1;
      }
      self.selectableWords.push(selectableWord);
    });

    $wordContainer.appendTo($container);
    self.$wordContainer = $wordContainer;
  };

  /**
   * Add check solution and retry buttons.
   */
  MarkTheWords.prototype.addButtons = function () {
    var self = this;
    self.$buttonContainer = $('<div/>', {'class': BUTTON_CONTAINER});

    this.addButton('check-answer', this.params.checkAnswerButton, function () {
      self.setAllSelectable(false);
      self.feedbackSelectedWords();
      self.hideButton('check-answer');
      if (!self.showEvaluation()) {
        // Only show if a correct answer was not found.
        if (self.params.behaviour.enableSolutionsButton && (self.correctAnswers < self.answers)) {
          self.showButton('show-solution');
        }
        if (self.params.behaviour.enableRetry) {
          self.showButton('try-again');
        }
      }
    });

    this.addButton('try-again', this.params.tryAgainButton, function () {
      self.clearAllMarks();
      self.hideEvaluation();
      self.setAllSelectable(true);
      self.hideButton('try-again');
      self.hideButton('show-solution');
      self.showButton('check-answer');
    }, false);

    this.addButton('show-solution', this.params.showSolutionButton, function () {
      self.setAllSelectable(false);
      self.setAllMarks();
      self.hideButton('check-answer');
      self.hideButton('show-solution');
      if (self.params.behaviour.enableRetry) {
        self.showButton('try-again');
      }
    }, false);
  };

  /**
   * Set whether all the words should be selectable.
   * @public
   * @param {Boolean} selectable Set to true to make the words selectable.
   */
  MarkTheWords.prototype.setAllSelectable = function (selectable) {
    this.selectableWords.forEach(function (entry) {
      entry.setSelectable(selectable);
    });

  };

  /**
   * Mark the words as correct, wrong or missed.
   */
  MarkTheWords.prototype.setAllMarks = function () {
    this.selectableWords.forEach(function (entry) {
      entry.markCheck();
    });
    this.trigger('resize');
  };

  /**
   * Mark the selected words as correct or wrong.
   */
  MarkTheWords.prototype.feedbackSelectedWords = function () {
    this.selectableWords.forEach(function (entry) {
      if (entry.isSelected()) {
        entry.markCheck();
      }
    });
    this.trigger('resize');
  };

  /**
   * Evaluate task and display score text for word markings.
   *
   * @return {Boolean} Returns true if maxScore was achieved.
   */
  MarkTheWords.prototype.showEvaluation = function () {
    this.hideEvaluation();
    this.calculateScore();

    var score = ((this.correctAnswers - this.wrongAnswers) <= 0) ? 0 : (this.correctAnswers - this.wrongAnswers);
    //replace editor variables with values, uses regexp to replace all instances.
    var scoreText = this.params.score.replace(/@score/g, score.toString())
      .replace(/@total/g, this.answers.toString())
      .replace(/@correct/g, this.correctAnswers.toString())
      .replace(/@wrong/g, this.wrongAnswers.toString())
      .replace(/@missed/g, this.missedAnswers.toString());

    this.setFeedback(scoreText, score, this.answers);

    this.triggerXAPIScored(score, this.answers, 'answered');
    this.trigger('resize');
    return score === this.answers;
  };

  /**
   * Clear the evaluation text.
   */
  MarkTheWords.prototype.hideEvaluation = function () {
    this.setFeedback();
    this.trigger('resize');
  };

  /**
   * Calculate score and store them in class variables.
   */
  MarkTheWords.prototype.calculateScore = function () {
    var self = this;
    self.correctAnswers = 0;
    self.wrongAnswers = 0;
    self.missedAnswers = 0;
    self.selectableWords.forEach(function (entry) {
      if (entry.isCorrect()) {
        self.correctAnswers += 1;
      } else if (entry.isWrong()) {
        self.wrongAnswers += 1;
      } else if (entry.isMissed()) {
        self.missedAnswers += 1;
      }
    });
  };

  /**
   * Clear styling on marked words.
   */
  MarkTheWords.prototype.clearAllMarks = function () {
    this.selectableWords.forEach(function (entry) {
      entry.markClear();
    });
    this.trigger('resize');
  };

  /**
   * Needed for contracts.
   * Always returns true, since MTW has no required actions to give an answer. Also calculates score.
   *
   * @returns {Boolean} Always returns true.
   */
  MarkTheWords.prototype.getAnswerGiven = function () {
    return true;
  };

  /**
   * Needed for contracts.
   * Counts the score, which is correct answers subtracted by wrong answers.
   *
   * @returns {Number} score The amount of points achieved.
   */
  MarkTheWords.prototype.getScore = function () {
    this.calculateScore();
    return ((this.correctAnswers - this.wrongAnswers) <= 0) ? 0 : (this.correctAnswers - this.wrongAnswers);
  };

  /**
   * Needed for contracts.
   * Gets max score for this task.
   *
   * @returns {Number} maxScore The maximum amount of points achievable.
   */
  MarkTheWords.prototype.getMaxScore = function () {
    return this.answers;
  };

  /**
   * Get title
   * @returns {string}
   */
  MarkTheWords.prototype.getTitle = function () {
    return H5P.createTitle(this.params.taskDescription);
  };

  /**
   * Needed for contracts.
   * Display the evaluation of the task, with proper markings.
   */
  MarkTheWords.prototype.showSolutions = function () {
    this.showEvaluation();
    this.setAllMarks();
    this.hideAllButtons();
    this.setAllSelectable(false);
  };

  /**
   * Needed for contracts.
   * Resets the task back to its' initial state.
   */
  MarkTheWords.prototype.resetTask = function () {
    this.clearAllMarks();
    this.hideEvaluation();
    this.setAllSelectable(true);
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.showButton('check-answer');
  };

  /**
   * Hide all buttons. Used to disable further input for user.
   */
  MarkTheWords.prototype.hideAllButtons = function () {
    this.hideButton('try-again');
    this.hideButton('show-solution');
    this.hideButton('check-answer');
    this.trigger('resize');
  };

  /**
   * Returns an object containing the selected words
   *
   * @returns {object} containing indexes of selected words
   */
  MarkTheWords.prototype.getCurrentState = function () {
    var selectedWordsIndexes = [];
    if (this.selectableWords === undefined) {
      return undefined;
    }

    this.selectableWords.forEach(function (selectableWord, swIndex) {
      if (selectableWord.isSelected()) {
        selectedWordsIndexes.push(swIndex);
      }
    });
    return selectedWordsIndexes;
  };

  /**
   * Sets answers to current user state
   */
  MarkTheWords.prototype.setH5PUserState = function () {
    var self = this;

    // Do nothing if user state is undefined
    if (this.previousState === undefined || this.previousState.length === undefined) {
      return;
    }
    // Select words from user state
    this.previousState.forEach(function (answeredWordIndex) {
      if (isNaN(answeredWordIndex) || answeredWordIndex >= self.selectableWords.length || answeredWordIndex < 0) {
        throw new Error('Stored user state is invalid');
      }
      self.selectableWords[answeredWordIndex].toggleMark();
    });

	//h5pcustomize
 	if(self.params.behaviour.enableRetry == false && this.previousState.length > 0) //>0 means attempted
 	{   
		self.setAllSelectable(false);
    }
  
    
  };

  MarkTheWords.prototype.registerDomElements = function () {

    // Register description
    this.setIntroduction(this.params.taskDescription);

    // Register content
    this.setContent(this.$inner, {
      'class': MAIN_CONTAINER
    });

    // Register buttons
    this.addButtons();
  };

  /**
   * Private class for keeping track of selectable words.
   *
   * @class
   * @param {H5P.jQuery} $word
   */
  function Word($word) {
    var self = this;
    H5P.EventDispatcher.call(self);

    var input = $word.text();
    var handledInput = input;

    // Check if word is an answer
    var isAnswer = checkForAnswer();

    // Remove single asterisk and escape double asterisks.
    handleAsterisks();

    var isSelectable = true;
    var isSelected = false;

    if (isAnswer) {
      $word.text(handledInput);
    }

    // Handle click events
    $word.click(function () {
      if (!isSelectable) {
        return;
      }
      self.toggleMark();
    });

    /**
     * Checks if the word is an answer by checking the first, second to last and last character of the word.
     * @private
     * @return {Boolean} Returns true if the word is an answer.
     */
    function checkForAnswer() {
      // Check last and next to last character, in case of punctuations.
      var wordString = removeDoubleAsterisks(input);
      if (wordString.charAt(0) === ('*') && wordString.length > 2) {
        if (wordString.charAt(wordString.length - 1) === ('*')) {
          handledInput = input.slice(1, input.length - 1);
          return true;
        }
        // If punctuation, add the punctuation to the end of the word.
        else if(wordString.charAt(wordString.length - 2) === ('*')) {
          handledInput = input.slice(1, input.length - 2);
          return true;
        }
        return false;
      }
      return false;
    }

    /**
     * Removes double asterisks from string, used to handle input.
     * @private
     * @param {String} wordString The string which will be handled.
     * @results {String} slicedWord Returns a string without double asterisks.
     */
    function removeDoubleAsterisks(wordString) {
      var asteriskIndex = wordString.indexOf('*');
      var slicedWord = wordString;
      while (asteriskIndex !== -1) {
        if (wordString.indexOf('*', asteriskIndex + 1) === asteriskIndex + 1) {
          slicedWord = wordString.slice(0, asteriskIndex) + wordString.slice(asteriskIndex + 2, input.length);
        }
        asteriskIndex = wordString.indexOf('*', asteriskIndex + 1);
      }
      return slicedWord;
    }

    /**
     * Escape double asterisks ** = *, and remove single asterisk.
     * @private
     */
    function handleAsterisks() {
      var asteriskIndex = handledInput.indexOf('*');
      while (asteriskIndex !== -1) {
        handledInput = handledInput.slice(0, asteriskIndex) + handledInput.slice(asteriskIndex + 1, handledInput.length);
        asteriskIndex = handledInput.indexOf('*', asteriskIndex + 1);
      }
    }

    /**
     * Toggle the marking of a word.
     * @public
     */
    this.toggleMark = function () {
      self.triggerXAPI('interacted');
      $word.toggleClass(SELECTED_MARK);
      isSelected = !isSelected;
    };

    /**
     * Clears all marks from the word.
     * @public
     */
    this.markClear = function () {
      $word.removeClass(MISSED_MARK)
        .removeClass(CORRECT_MARK)
        .removeClass(WRONG_MARK)
        .removeClass(SELECTED_MARK);
      isSelected = false;
    };

    /**
     * Sets correct styling if word is an answer.
     * @public
     */
    this.showSolution = function () {
      $word.removeClass(MISSED_MARK)
        .removeClass(CORRECT_MARK)
        .removeClass(WRONG_MARK)
        .removeClass(SELECTED_MARK);
      if (isAnswer) {
        $word.addClass(CORRECT_MARK);
      }
    };

    /**
     * Check if the word is correctly marked and style it accordingly.
     * @public
     */
    this.markCheck = function () {
      if (isSelected) {
        if (isAnswer) {
          $word.addClass(CORRECT_MARK);
        } else {
          $word.addClass(WRONG_MARK);
        }
      } else if (isAnswer) {
        $word.addClass(MISSED_MARK);
      }
      $word.removeClass(SELECTED_MARK);
    };

    /**
     * Set whether the word should be selectable, and proper feedback.
     * @public
     * @param {Boolean} selectable Set to true to make word selectable.
     */
    this.setSelectable = function (selectable) {
      isSelectable = selectable;
      //Toggle feedback class
      if (selectable) {
        $word.removeClass(WORD_DISABLED);
      } else {
        $word.addClass(WORD_DISABLED);
      }
    };

    /**
     * Checks if the word is marked correctly.
     * @public
     * @returns {Boolean} True if the marking is correct.
     */
    this.isCorrect = function () {
      return (isAnswer && isSelected);
    };

    /**
     * Checks if the word is marked wrong.
     * @public
     * @returns {Boolean} True if the marking is wrong.
     */
    this.isWrong = function () {
      return (!isAnswer && isSelected);
    };

    /**
     * Checks if the word is correct, but has not been marked.
     * @public
     * @returns {Boolean} True if the marking is missed.
     */
    this.isMissed = function () {
      return (isAnswer && !isSelected);
    };

    /**
     * Checks if the word is an answer.
     * @public
     * @returns {Boolean} True if the word is an answer.
     */
    this.isAnswer = function () {
      return isAnswer;
    };

    /**
     * Checks if the word is selected.
     * @public
     * @returns {Boolean} True if the word is selected.
     */
    this.isSelected = function () {
      return isSelected;
    };
  }
  Word.prototype = Object.create(H5P.EventDispatcher.prototype);
  Word.prototype.constructor = Word;

  return MarkTheWords;
}(H5P.jQuery, H5P.Question));
