/**
	* jwplayer_tracker.js
 *
 * @version 1.0
 *
 * @author Uday kiran
 * @author Uday kiran<udaykiran.vit@gmail.com>
 *
 * @url <https://github.com/udayakiran/jwplayer-tracker>
 */

(function (jwplayer) {
  var template = function (player, config, div) {
    var progressTracker = new VideoProgressTracker({
      config: config,
      player: player,
      progress: config.progress,
      url: config.url,
      frequency: config.frequency,
      video_duration_callback: player.getDuration,
      resume_from_last: config.resume_from_last,
      progress_only: config.progress_only,
      debug: config.debug,
      additional_data: config.additional_data
    });

    player.onReady(_setup);

    function _setup(event) {
      videoTrackerProgress = null;	//reinitiate progress at start which will be available in global 
      progressTracker.log("_setup");
      if (progressTracker.hasValidOptions()) {
        progressTracker.log("hasValidOptions");
        player.onComplete(_completeUpdate);
        player.onPlay(_initPosition);
        player.onSeek(_onSeek);
      }
      if (progressTracker.hasProgressUpdateUrl()) {
        player.onTime(_updateProgress);
      }
    }

    function _updateProgress(event) {
      progressTracker.log("_updateProgress");
      //$('.ctool-video-modal').find('#loaderdivmodal-content').css('visibility','hidden');
      if(event.position == 0 && progressTracker.prevPosition > 0) {
    	  event.position = progressTracker.prevPosition;
      }
      var current_position = parseInt(event.position);
      progressTracker.updateProgress(current_position);
    }

    function _initPosition(event) {
      progressTracker.log("_initPosition");
      progressTracker.initPosition();
    }

    function _completeUpdate(event) {
      progressTracker.log("_completeUpdate");
      var duration = parseInt(player.getDuration());
	  if(!isNaN(duration) && duration > 0) {
        progressTracker.log("_completeUpdate" + duration);
		progressTracker.updateProgress(duration, true);
	  } else {
		progressTracker.log("_completeUpdate. video duration is incorrect");
		progressTracker.log("_completeUpdate. is ShockWave is enabled? " + (navigator.plugins['Shockwave Flash'] !== undefined ? 'Yes' : 'No'));
		progressTracker.log("_completeUpdate. ShockWave version = " + (navigator.plugins['Shockwave Flash'].version !== undefined ? navigator.plugins['Shockwave Flash'].version : 'Couldn\'t get verion of Shockwave'));
		progressTracker.log("Check it once here - " + "https://www.adobe.com/software/flash/about/");
	  }
    }

    function _onSeek(event) {
		//console.log('poition '+this.getPosition());
		//console.log('seek offset '+event.offset);
      progressTracker.log("_onSeek");
      if (event.offset) {
        progressTracker.updateProgress(parseInt(event.offset), true, true);
      }
    }
  };

  jwplayer().registerPlugin('jwplayer_tracker', template);

})(jwplayer);


function VideoProgressTracker(options) {
  this.progressPercentage = options.progress;
  this.progressUpdateUrl = options.url;
  this.prevPosition = 0;
  this.frequency = (options.frequency !== 'undefined' && options.frequency > 0) ? options.frequency : 10;
  this.player = options.player;
  this.video_duration_callback = options.video_duration_callback;
  this.resume_from_last = options.resume_from_last;
  this.debug = (options.debug === true) ? options.debug : false;
  this.additional_data = options.additional_data;
  this.total_session_duration = 0;
  this.progress_only = (options.progress_only === true) ? options.progress_only : false;
  this.session_id = guid();
  this.current_progress_percent = 0;
  this.last_updated_progress = 0;

  function guid() {
    function _p8(s) {
      var p = (Math.random().toString(16) + "000000000").substr(2, 8);
      return s ? "-" + p.substr(0, 4) + "-" + p.substr(4, 4) : p;
    }
    return _p8() + _p8(true) + _p8(true) + _p8();
  }
}

VideoProgressTracker.prototype.videoDuration = function () {
  return this.video_duration_callback.apply(this.player);
};

VideoProgressTracker.prototype.initPrevPosition = function () {
  if (this.prevPosition === 0) {
    this.prevPosition = Math.floor((this.progressPercentage / 100) * this.videoDuration());
  }
};

VideoProgressTracker.prototype.initPosition = function () {
	try {
  if (this.prevPosition === 0 && this.resume_from_last === true) {
    this.prevPosition = Math.floor((this.progressPercentage / 100) * this.videoDuration());
  }

  if (this.currentPosition >= parseInt(this.videoDuration())) {
    this.currentPosition = 0;
  }
  if (this.prevPosition >= parseInt(this.videoDuration())) {
    this.prevPosition = 0;
  }

  if (this.resume_from_last === true) {
	  //console.log('seek from = '+this.prevPosition);
    player1 = this;
	// var random = Math.floor(Math.random() * (370 - 100 + 1)) + 100;
	//console.log('rand = '+random);
	// //console.log(player1.player.getState());
	setTimeout(function() {
	player1.player.seek(player1.prevPosition);
	}, 600);
	// //console.log(player1.player.getState());
    this.resume_from_last = false;
  }
	} catch(e) {
		// console.log(e);
	}
};

VideoProgressTracker.prototype.hasValidOptions = function () {
  return this.hasProgressUpdateUrl();
};

VideoProgressTracker.prototype.hasProgressUpdateUrl = function () {
  return this.progressUpdateUrl !== "";
};

VideoProgressTracker.prototype.updateProgress = function (position, force, seeked) {
  if (seeked) {
    var duration_played = this.currentPosition - this.prevPosition;
  }

  this.currentPosition = position;

  if (!seeked) {
    var duration_played = this.currentPosition - this.prevPosition;
  }

  if (force === true || (!this.progress_only && (duration_played >= this.frequency)) || (this.progress_only && (this.currentPosition - this.prevPosition) >= this.frequency)) {
    this.frequency_counter = 0;
    var video_duration = parseInt(this.videoDuration());

    //just an edge case
    if (this.currentPosition > video_duration) {
      this.currentPosition = video_duration;
    }

    this.current_progress_percent = Math.round((this.currentPosition / video_duration) * 100);
    this.total_session_duration = this.total_session_duration + (duration_played > 0 ? duration_played : 0);

    if (this.progress_only && this.last_updated_progress < this.current_progress_percent) {
      this.saveProgress();
      this.last_updated_progress = this.current_progress_percent;
    }
    this.prevPosition = this.currentPosition;
  }
  var self = this;
  // progress is saved to global variable
  videoTrackerProgress = {
		    session_id: self.session_id,
		    previous_position: self.prevPosition,
		    current_position: self.currentPosition,
		    progress: self.current_progress_percent,
		    video_duration: parseInt(self.videoDuration()),
		    additional_data: self.additional_data
		  };
};
VideoProgressTracker.prototype.saveProgress = function (toServer) {
  this.log("Updating progress to the server.");
  //console.log('date = '+new Date().toString());
  var self = this;

  var data = {
    session_id: self.session_id,
    previous_position: self.prevPosition,
    current_position: self.currentPosition,
    progress: self.current_progress_percent,
    video_duration: parseInt(this.videoDuration())
  };

  if (!this.progress_only) {
    data.total_session_duration = self.total_session_duration;
  }

  if (this.additional_data) {
    data.additional_data = this.additional_data;
  }
//  //console.log(JSON.parse(JSON.stringify(data)))
  this.log("progress info - " + JSON.stringify(data));
  
  if(toServer !== undefined && toServer == false) {
	  return;
  }
  $.ajax({
    url: this.progressUpdateUrl,
    type: 'POST',
	dataType: 'json',
    data: data,
  	success: function() {
  		self.log("Call is successful.");
  	},
  	error: function() {
  		self.log("Call is unsuccessful.");
  	},
  	complete: function() {
  		self.log("Server update complete.");
  	    if (typeof server_update_callback !== 'undefined') {
  	      this.server_update_callback.apply(data);
  	    }
	}
  });
};

VideoProgressTracker.prototype.log = function (msg) {
  if (this.debug === true && !(typeof window.console === 'undefined')) {
  }
};