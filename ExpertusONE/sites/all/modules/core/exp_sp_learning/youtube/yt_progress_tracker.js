// var contentType = 'youtube';
var videourl;
var height;
var video_progress;
// var vimeo;
var session_id;
var update_frequency;

if(Drupal.settings.youtubesetting.url!= ''){
	videourl = Drupal.settings.youtubesetting.url;
	height = Drupal.settings.youtubesetting.height;
	width = Drupal.settings.youtubesetting.width;
	video_progress = Drupal.settings.youtubesetting.video_progress;     
	// vimeo = Drupal.settings.youtubesetting.vimeo; 
	session_id = Drupal.settings.youtubesetting.session_id;
	update_frequency = Drupal.settings.youtubesetting.update_frequency;
}
// console.log('update_frequency-->'+update_frequency);
var previous_position	= 0;
var current_position	= 0;
var video_duration		= 0;
var progress			= 0;
interval				= null;
ajaxInterval			= null;
updateStatusToDB        = null;


var data = {
	session_id: session_id,
	previous_position: previous_position,
	current_position: current_position,
	progress: progress,
	video_duration: video_duration,
	from: 'web',
	// videoType: contentType,
	content_type : 'youtube'
};

videoTrackerProgress = {
	session_id: session_id,
	previous_position: previous_position,
	current_position: current_position,
	progress: progress,
	video_duration: video_duration,
	additional_data: {video_session_id: session_id},
	content_type : 'youtube'
};   
var ytPlayer;
// call trigger onYouTubeIframeAPIReady() manually as it has been called already
window.YouTubeIframeAPIReady !== undefined && window.YouTubeIframeAPIReady && onYouTubeIframeAPIReady();
function onYouTubeIframeAPIReady() {
	window.YouTubeIframeAPIReady = true;
	// console.log('on youtube ready');
	setTimeout(function(){
		// console.log("first");
		$('#loaderdivmodal-content').css('display','none');
		// console.log("second");
	}, 3000);
	ytPlayer = new YT.Player( 'ytPlayer', {
		events: { 'onStateChange': onPlayerStateChange }
	});
}
function onYouTubePlayerAPIReady() {
	// console.log('onYouTubePlayerAPIReady');
}

function onPlayerStateChange(event) {
	switch(event.data) {
		case -1:
			// console.log('unstarted');
			// console.log('seeking to ' + previous_position);
			ytPlayer.seekTo(previous_position, true);
			data.video_duration = ytPlayer.getDuration();
			videoTrackerProgress.video_duration = data.video_duration;
		break;
		case 0:
			// console.log('ended');
			//var percentage 	= (ytPlayer.getCurrentTime() / ytPlayer.getDuration()) * 100;
			var percentage = 100; // when player cursor moved at the end.
			var progress 	= isNaN(percentage) == false ? percentage : 0;
			videoTrackerProgress = {
				session_id: session_id,
				current_position: ytPlayer.getCurrentTime(),
				progress: progress,
				video_duration: ytPlayer.getDuration(),
				additional_data: {video_session_id: session_id},
				content_type : 'youtube'
			};  
			updateVideoProgress();
			clearInterval(interval);
			clearInterval(ajaxInterval);
		break;
		case 1:
			// console.log('playing');
			updateyoutube();
		break;
		case 2:
			// console.log('paused');
			clearInterval(interval);
			clearInterval(ajaxInterval);
		break;
	}
}
// -1 – unstarted
// 0 – ended
// 1 – playing
// 2 – paused
// 3 – buffering
// 5 – video cued
/*
    function record(data){
	// Do what you want with your data
	var p = document.createElement("p");
	p.appendChild(document.createTextNode(data));
	document.body.appendChild(p);
    }
*/
function updateyoutube() {
	//console.log(data);
	if(interval != null) {
		clearInterval(interval);
	}
	if(ajaxInterval != null) {
		clearInterval(ajaxInterval);
	}
	
	data.video_duration = ytPlayer.getDuration();
	videoTrackerProgress.video_duration = data.video_duration;
	
	interval = setInterval(function(){
        if(document.getElementById('ytPlayer')){
			// console.log("updateyoutube----player--->"+data.progress);            
            if(data.progress == 100){
				return;
			}
            data.previous_position = data.current_position;
            data.current_position = ytPlayer.getCurrentTime();
            videoTrackerProgress.previous_position = data.previous_position;
            videoTrackerProgress.current_position = data.current_position;
            
            var percentage 	= (ytPlayer.getCurrentTime() / ytPlayer.getDuration()) * 100;
			data.progress 	= isNaN(percentage) == false ? percentage : video_progress;
			// data.progress = percentage*100;
			videoTrackerProgress.progress = data.progress;         
			// if(data.progress == null || data.progress == undefined ){
			// data.progress = video_progress;
			// }
		}
	}, 1000);
	ajaxInterval = setInterval(function() {
		updateVideoProgress();
	}, update_frequency);
}
function updateVideoProgress() {
	$.ajax({
		url:"?q=ajax/update-launch/vod",
		type: 'POST',
		data: videoTrackerProgress,
		success: function() {
			// console.log("Call is successful.");
		},
		error: function() {
			// console.log("Call is unsuccessful.");
		},
		complete: function() {
			// console.log("Server update complete.");
		}
	});
}
setTimeout(function(){  $('#loaderdivmodal-content').css('display','none'); }, 3000);