/** @namespace H5P */
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
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-label").hide(); //Interactive editor title
	  H5P.jQuery(".h5peditor-form > .tree > .wizard > .h5peditor-tabs").hide(); //tabs
	  console.log("self::",self);
      if (self.appendTo !== undefined) {
        //alert(self.attachVideoJS);
    	    console.log('self.....',self);
    	    var playerType = "flowplayer";
    	    if(sources[0].path.indexOf("rtmp://")==0)
    	    		playerType = "videojs";
    	    else if(sources[0].path.indexOf("vimeo")>0 )
    	    		playerType = "vimeo"; 
    	    if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
    	    		playerType = "videojs";
        if(playerType == "videojs") // && (window.parent.$("#iframe_editor_wrapper_container").size() == 0)) // && window.parent.$("#contentauthor-presentation-addedit-form").size() == 0))//only for preview
        		self.attachVideoJS($container);
        else if(playerType == "vimeo")
        		self.attachVideoJS($container);
        else
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
     console.log("sources:",sources);
     console.log("handlers:",handlers);
     
     if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size()>0) //for presentation editor and preview only use videojs
    	 {
    	 	var handler = handlers[2]; //flash videojs player
    	 	handler.call(self, sources, {
	            controls: parameters.visuals.controls,
	            autoplay: parameters.playback.autoplay,
	            loop: parameters.playback.loop,
	            fit: parameters.visuals.fit,
	            poster:undefined, //h5pcustomize parameters.visuals.poster === undefined ? undefined : H5P.getPath(parameters.visuals.poster.path, id),
	            startAt: parameters.startAt || 0,
	            tracks: tracks
	          }, parameters.l10n);
    	 	return "";
    	 }
     else
    	 {
	      for (var i = 0; i < handlers.length; i++) {
	      	
	        var handler = handlers[i];
	        if (handler.canPlay !== undefined && handler.canPlay(sources))
	         {
	          handler.call(self, sources, {
	            controls: parameters.visuals.controls,
	            autoplay: parameters.playback.autoplay,
	            loop: parameters.playback.loop,
	            fit: parameters.visuals.fit,
	            poster:undefined, //h5pcustomize parameters.visuals.poster === undefined ? undefined : H5P.getPath(parameters.visuals.poster.path, id),
	            startAt: parameters.startAt || 0,
	            tracks: tracks
	          }, parameters.l10n); 
	          return;
	        }
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
