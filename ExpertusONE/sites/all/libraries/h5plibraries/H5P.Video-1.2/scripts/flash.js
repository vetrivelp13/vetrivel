/** @namespace H5P */

/*
 * h5pcustomize - Removed flowplayer support and added videojs for flash
 * objects.Backup filename for flowplayer flash.js_backup
 */

H5P.VideoFlash = (function ($) {

  /**
	 * Flash video player for H5P.
	 * 
	 * @class
	 * @param {Array}
	 *            sources Video files to use
	 * @param {Object}
	 *            options Settings for the player
	 */
  function Flash(sources, options) {
    console.log("sources:",sources);
    var self = this;
    var vimeoHeight =0,vimeoWidth = 0;
    var vimeoDuration = 0,vimeoCurrentTime = 0,vimeoBuffered = 0,vimeoVolume = 0;
    var videoSeen = 0;
    //var playerType = "flowplayer";
    var playerType = "videojs";
    if(sources[0].path.indexOf("rtmp://")==0)
    		playerType = "videojs";
    else if(sources[0].path.indexOf("vimeo")>0 )
    		playerType = "vimeo";
    
   // playerType = "videojs";
   // var playerType = (sources[0].path.indexOf("rtmp://")==0 ||
	// sources[0].path.indexOf("vimeo")>0 )?"videojs":"flowplayer";
    console.log("playerType:",playerType);

    // Player wrapper
    var $wrapper = $('<div/>', {
      'class': 'h5p-video-flash',
      'id':'h5p-video-flash',
      css: {
        width: '100%',
        height: '100%'
      }
    });

    /**
	 * Used to display error messages
	 * 
	 * @private
	 */
    var $error = $('<div/>', {
      'class': 'h5p-video-error'
    });

    /**
	 * Keep track of current state when changing quality.
	 * 
	 * @private
	 */
    var stateBeforeChangingQuality;
    var currentTimeBeforeChangingQuality;

    // Sort sources into qualities
    // var qualities = getQualities(sources);
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
        rtmp: {
          url: "http://releases.flowplayer.org/swf/flowplayer.rtmp-3.2.13.swf"
        },
        controls: { display: 'none'},
        
      },
  	  autoplay: true,
      play: null, // Disable overlay controls
      onPlaylistReplace: function () {
        that.playlistReplaced();
      }
    };

    if (options.controls) {
      playerOptions.plugins.controls = {};
      delete playerOptions.play;
    }
    var fp = null;
 	if(playerType == "flowplayer")
 	{
      var fp = flowplayer($wrapper[0], {
      src: "http://releases.flowplayer.org/swf/flowplayer-3.2.16.swf",
      wmode: "opaque"
    }, playerOptions); 
	}
	
	
    /**
	 * Appends the video player to the DOM.
	 * 
	 * @public
	 * @param {jQuery}
	 *            $container
	 */
    self.appendTo = function ($container) {
    	
      $wrapper.appendTo($container);
    };

 	self.attachVideoJS = function($container){
 	console.log("$container:",$container);	
 	$wrapper.appendTo($container);
 	window.parent.$(".loadercontent").remove();
    var h = window.parent.$("#iframedata").height()-38;
 	var w = window.parent.$("#iframedata").width();
 	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/video.js");
 	loadCssFilesH5p('/sites/all/modules/core/exp_sp_core/js/videojs/video-js.min.css');

 	
 	
 	videojs.options.flash.swf = "/sites/all/modules/core/exp_sp_core/js/videojs/video-js.swf";
	
	H5P.jQuery(".h5p-interactive-video").css("height","inherit");
	H5P.jQuery(".h5p-video-wrapper").css("height","inherit");
	window.parent.$(".loadercontent").remove();	
	
	if(sources[0].path.indexOf("vimeo") > 0)
	{
	 	loadScriptsForTinyMCE("/sites/all/modules/core/exp_sp_core/js/videojs/Vimeo.js");
	 	loadScriptsForTinyMCE("/sites/all/libraries/h5plibraries/H5P.Video-1.2/scripts/vimeoplayer.js");

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
		var video_dom_leng = H5P.jQuery(".my_video_presentation").size() == 0?1:H5P.jQuery(".my_video_presentation").size()+1;
		
		if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
			html =   "<video id='my_video_presentation"+video_dom_leng+"' class='video-js my_video_presentation  vjs-default-skin vjs-big-play-centered  vjs-has-started vjs-nofull vjs-paused vjs-ended vjs-user-active ' style='height:100%;width:100%;'  autoplay=false   data-setup=\"{}\">"; //style='height:inherit;width:auto;'
		else
			html =   "<video id='my_video_2' class='Fullsc video-js' autoplay=true controls='false' preload='auto' width='"+w+"' height='"+h+"' data-setup=\"{}\">";
	// html += "<source
	// src='rtmp://192.241.173.210/oflaDemo/Saravanan_4/256/Saravanan.mp4'
	// type='rtmp/mp4' />"; //what about webm?

	    html += "<source src='"+sources[0].path+"' type='video/mp4' />";
	    html += "<source src='"+sources[0].path+"' type='video/webm' />";
	    html += "<source src='"+sources[0].path+"' type='rtmp/mp4' />";
		html +="</video>";
	
	$container.find(".h5p-video-flash").append(html);

	console.log("1115:"+sources[0].path);
	var playing = false,firstTime = true;
	
	if(window.parent.$("#contentauthor-presentation-addedit-form").size() > 0 || H5P.jQuery(".h5p-course-presentation").size() > 0)
		{
		fp = videojs("my_video_presentation"+video_dom_leng,{controls: options.controls,autoplay:options.autoplay}).ready(function(){});
		
		}
	else
		{
	fp = videojs("my_video_2",{controls: options.controls,autoplay:options.autoplay}).ready(function(){
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
	 * Set current playback quality. Not available until after play. Listen to
	 * event "qualityChange" to check if successful.
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
	 * @param {Number}
	 *            time
	 */
    self.seek = function (time) {
     if(playerType == "flowplayer")
     	fp.seek(time);
     else if(playerType  == "videojs")
     	fp.currentTime(time);
     else if(playerType == "vimeo")
    	 {
    	 fp.setCurrentTime(time).then(function(seconds) {
    		    // seconds = the actual time that the player seeked to
    		}).catch(function(error) {
    		    switch (error.name) {
    		        case 'RangeError':
    		            // the time was less than 0 or greater than the video’s
						// duration
    		            break;

    		        default:
    		            // some other error occurred
    		            break;
    		    }
    		});
    	 }
      
    };

    /**
	 * Get elapsed time since video beginning.
	 * 
	 * @public
	 * @returns {Number}
	 */
    self.getCurrentTime = function () {
        if(playerType == "flowplayer")
        		return fp.getTime();
        else if(playerType  == "videojs")
        		return fp.currentTime();
        else if(playerType == "vimeo")
        	 	return vimeoCurrentTime;
    };

    /**
	 * Get total video duration time.
	 * 
	 * @public
	 * @returns {Number}
	 */
    self.getDuration = function () {
		if(playerType == "flowplayer")
    		return fp.getClip().metaData.duration;
    	else if(playerType  == "videojs")
    		return fp.duration();
    	else if(playerType == "vimeo")
    	{
    		return vimeoDuration;
    	}
    };

    /**
	 * Get percentage of video that is buffered.
	 * 
	 * @public
	 * @returns {Number} Between 0 and 100
	 */
    self.getBuffered = function () {
    	 console.log("self.getbuffered called...:"+vimeoBuffered+"===="+playerType);
       // console.log("fp.currentTime()/fp.duration()*100:"+fp.currentTime()/fp.duration()*100);
		if(playerType == "flowplayer")
			return fp.getClip().buffer;
    	else if(playerType  == "videojs")
    		return fp.currentTime()/fp.duration()*100;
    	else if(playerType == "vimeo")
    		return vimeoBuffered;
    };

    /**
	 * Turn off video sound.
	 * 
	 * @public
	 */
    self.mute = function () {
    	  console.log("mute:");
    	  player.getVolume().then(function(volume) {
    		     vimeoVolume = volume;
    		}).catch(function(error) {
    		    // an error occurred
    		});
    	  
    	  if(playerType == "vimeo")
    	  {
    		  fp.setVolume(0.0).then(function(volume) {
    			    // volume was set
    			}).catch(function(error) {
    			    switch (error.name) {
    			        case 'RangeError':
    			            // the volume was less than 0 or greater than 1
    			            break;

    			        default:
    			            // some other error occurred
    			            break;
    			    }
    			});
    	  }
    	  else
      fp.mute();
    };

    /**
	 * Turn on video sound.
	 * 
	 * @public
	 */
    self.unMute = function () {
    	console.log("unMute:");
    	 if(playerType == "vimeo")
   	  {
   		  fp.setVolume(vimeoVolume).then(function(volume) {
   			    // volume was set
   			}).catch(function(error) {
   			    switch (error.name) {
   			        case 'RangeError':
   			            // the volume was less than 0 or greater than 1
   			            break;

   			        default:
   			            // some other error occurred
   			            break;
   			    }
   			});
   	  }
    	 else
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
    	 console.log("getVolume:"+fp.volumeLevel);
      return fp.volumeLevel * 100;
    };

    /**
	 * Set video sound level.
	 * 
	 * @public
	 * @param {Number}
	 *            volume Between 0 and 100.
	 */
    self.setVolume = function (level) {
      fp.volume(level / 100);
    };

    // Handle resize events
    self.on('resize', function () {
		if(playerType == "flowplayer"){
	      var $object = H5P.jQuery(fp.getParent()).children('object');
    	  var clip = fp.getClip();

      	  if (clip !== undefined) {
        	$object.css('height', $object.width() * (clip.metaData.height / clip.metaData.width));
      	  }
    	}else  if(playerType  == "videojs")
    	{
    	
    	var $object = H5P.jQuery("#my_video_2").children('object');
    	// alert(fp.height()+"==="+fp.width());
    	$object.css('height', $object.width() * (fp.height() / fp.width()));
    	}
    	else if(playerType == "vimeo"){
    		var $object = H5P.jQuery("#my_video_2");
    		console.log("vimeoWidth:"+vimeoWidth+"===vimeoHeight:"+vimeoHeight);
    		// $object.css('height', vimeoWidth* (vimeoHeight / vimeoWidth));
    		
    		
        	
    	}
    });
  }

  /**
	 * Check to see if we can play any of the given sources.
	 * 
	 * @public
	 * @static
	 * @param {Array}
	 *            sources
	 * @returns {Boolean}
	 */
  Flash.canPlay = function (sources) {
    // Cycle through sources
    
    for (var i = 0; i < sources.length; i++) {
    	  // vimeo support
      if (sources[i].mime == "video/vimeo" || sources[i].mime === 'video/mp4' || /\.mp4$/.test(sources[i].mime)) {
        return true; // We only play mp4
      }
    }
  };

  return Flash;
})(H5P.jQuery);

// Register video handler
H5P.videoHandlers = H5P.videoHandlers || [];
H5P.videoHandlers.push(H5P.VideoFlash);
