/**
 * vjs_tracker.js
 *
 * @version 1.0
 *
 * @author Uday kiran
 * @author Uday kiran<udaykiran.vit@gmail.com>
 *
 * @url <https://github.com/udayakiran/videojs-tracker>
 * 
 * API Reference - https://github.com/videojs/video.js/tree/release/4-1/docs
 */

function VjsTracker(player, config) {
	
	$(window).on("orientationchange",function(event){
    setTimeout(function(){
    if(window.innerHeight > window.innerWidth)
    {
    var player_width = ($( window ).width() / 2) - 10;
    var player_height = ($( window ).height() / 2 )- 40;
    $('.vjs-big-play-button').css('left',player_width+'px');
    $('.vjs-big-play-button').css('top',player_height+'px');
    }else{
    var player_width = ($( window ).width() / 2) - 10;
    var player_height = ($( window ).height() / 2 )- 40;
    $('.vjs-big-play-button').css('left',player_width+'px');
    $('.vjs-big-play-button').css('top',player_height+'px');
    }
    }, 100);
        });

var player_width = $( window ).width();
var player_width = ($( window ).width() / 2) - 10;
var player_height = ($( window ).height() / 2 )- 40;
$('.vjs-big-play-button').css('left',player_width+'px');
$('.vjs-big-play-button').css('top',player_height+'px');
	
  var player = player;
  var count = 1;
    
  var progressTracker = new VideoProgressTracker({
    config: config,
    player: player,
    progress: config.progress,
    url: config.url,
    frequency: config.frequency,
    video_duration_callback: player.duration,
    resume_from_last: config.resume_from_last,
    progress_only: config.progress_only,
    debug: config.debug,
    additional_data: config.additional_data
  });

  _setup();

  function _setup() {
    progressTracker.log("_setup");
    if (progressTracker.hasValidOptions()) {
      progressTracker.log("hasValidOptions");
     player.on('timeupdate', _updateProgress);
      player.on('play', _initPosition);
      player.on('durationchange', _initPositionAndDuration);
      player.on('ended', _completeUpdate);
      player.on('seeking', _onSeek);
      player.on('pause', _onpause);
    }
  }
  ;

  function _updateProgress() {
    progressTracker.log("_updateProgress");
      if(count == 1){
      var Myplayer = progressTracker.player;
      Myplayer.pause();
      Myplayer.currentTime(Math.floor((progressTracker.progressPercentage/ 100) * progressTracker.videoDuration()));
      Myplayer.play();
          count++;
      }

      
    var current_position = parseInt(player.currentTime());
    progressTracker.updateProgress(current_position);
  }
  ;

  function _initPosition() {
    progressTracker.log("_initPosition");
    progressTracker.initPosition();
    playVideo()

  }
  ;

  function _initPositionAndDuration() {
    progressTracker.log("_initPositionAndDuration");
    progressTracker.initPrevPosition();
  }
  ;

  function _completeUpdate(event) {
    progressTracker.log("_completeUpdate");
    _setPrevPosition(); //different compared to jw
    var duration = parseInt(progressTracker.videoDuration());
    progressTracker.updateProgress(duration, true);
  }
  ;

  function _setPrevPosition() {
    progressTracker.log("_setPrevPosition");
    if (progressTracker.prevPostion <= 0) {
      var player_duration = progressTracker.videoDuration();
      if (player_duration !== 'undefined')
        progressTracker.initPosition();
    }
  };

  function _onSeek(event) {
    progressTracker.log("_onSeek");
    progressTracker.updateProgress(parseInt(player.currentTime()), true, true);
  }
};



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
  if (this.resume_from_last === true) {
    if (this.prevPosition === 0) {
      this.prevPosition = Math.floor((this.progressPercentage / 100) * this.videoDuration());
    }
    this.player.currentTime(this.prevPosition);
    this.resume_from_last = false;
  }
};

VideoProgressTracker.prototype.initPosition = function () {
  if (this.prevPosition === 0 && this.resume_from_last === true) {
    this.prevPosition = Math.floor((this.progressPercentage / 100) * this.videoDuration());
  }

  if (this.currentPosition >= parseInt(this.videoDuration())) {
    this.currentPosition = 0;
  }
  if (this.prevPosition >= parseInt(this.videoDuration())) {
    this.prevPosition = 0;
  }
};

VideoProgressTracker.prototype.hasValidOptions = function () {
  return this.hasProgressUpdateUrl();
};

VideoProgressTracker.prototype.hasProgressUpdateUrl = function () {
  return this.progressUpdateUrl !== "";
};

VideoProgressTracker.prototype.updateProgress = function (position, force, seeked) {
    console.log("updateProgress111");
  if (seeked) {
    var duration_played = this.currentPosition - this.prevPosition;
  }

  this.currentPosition = position;

  if (!seeked) {
    var duration_played = this.currentPosition - this.prevPosition;
  }

//  if (force === true || (!this.progress_only) || (this.progress_only && (this.currentPosition - this.prevPosition) >= this.frequency)) {
    this.frequency_counter = 0;
    var video_duration = parseInt(this.videoDuration());

    //just an edge case
    if (this.currentPosition > video_duration) {
      this.currentPosition = video_duration;
    }

    this.current_progress_percent = Math.round((this.currentPosition / video_duration) * 100);
    this.total_session_duration = this.total_session_duration + (duration_played > 0 ? duration_played : 0);

    if (!this.progress_only || (this.progress_only && this.last_updated_progress < this.current_progress_percent)) {
      this.saveProgress();
      this.last_updated_progress = this.current_progress_percent;
    
    video_data.previous_position = this.prevPosition ;

    this.prevPosition = this.currentPosition;
//  }
    
    video_data.current_position = this.currentPosition ;
    video_data.progress = this.current_progress_percent ;
    video_data.video_duration = parseInt(this.videoDuration());
    }

};


VideoProgressTracker.prototype.saveProgress = function () {
  this.log("Updating progress to the server.");

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
  
  if(this.additional_data) {
    data.additional_data = this.additional_data;
  }

  this.log("progress info - " + console.log(JSON.parse(JSON.stringify(data))));

  $.ajax({
    url: this.progressUpdateUrl,
    method: 'POST',
    data: data,
    statusCode: {
      404: function () {
        self.log("URL not found.");
      },
      500: function () {
        self.log("Error on the server.");
      }
    }
  }).done(function (data) {
    self.log("Server update complete.");
    if (typeof server_update_callback !== 'undefined') {
      this.server_update_callback.apply(data);
    }
    ;
  });
};

VideoProgressTracker.prototype.log = function (msg) {
  if (this.debug === true && !(typeof window.console === 'undefined')) {
    console.log(msg);
  }
};
