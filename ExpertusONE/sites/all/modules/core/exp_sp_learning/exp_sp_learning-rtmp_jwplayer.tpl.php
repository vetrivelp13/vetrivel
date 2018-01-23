<?php expDebug::dPrint('jwplayer learning tpl file : $html_id = ' . print_r($html_id, true), 4);?>
<?php expDebug::dPrint('jwplayer learning tpl file : $upload_dir = ' . print_r($upload_dir, true), 4);?>
<?php expDebug::dPrint('jwplayer learning tpl file : $video_foldername = ' . print_r($video_foldername, true), 4);?>
<?php expDebug::dPrint('jwplayer learning tpl file : $video_basename = ' . print_r($video_basename, true), 4);?>
<?php expDebug::dPrint('jwplayer learning tpl file : $base_url = ' . print_r($base_url, true), 4);?>
<?php expDebug::dPrint('jwplayer learning tpl file : $base_url = ' . print_r( '/' . $video_foldername . '/' .$video_basename, true),4); 
/*
* { bitrate: 300, file:  "<?php print '/contentupload/contentupload/contentupload/'. $video_foldername . '/256/' .$video_basename?>.mp4", },
* { bitrate: 600, file:  "<?php print '/contentupload/contentupload/contentupload/'. $video_foldername . '/512/' .$video_basename?>.mp4", },
*
<div id='<?php print $html_id; ?>' class='exp-jwplayer' data-exp-jwplayer-settings='{
"skin" : "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/skins/newtubedark.zip",
"stretching" : "uniform",

"modes": [
{type: "flash", src: "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/player.swf",}
],
"levels" : [
{ bitrate: 300, file:  "<?php print '/contentupload/contentupload/contentupload/'. $video_foldername . '/256/' .$video_basename?>.mp4", },
{ bitrate: 900, file:  "<?php print '/contentupload/contentupload/contentupload/'. $video_foldername . '/900/' .$video_basename?>.mp4", }
],
provider: "rtmp",
"streamer": "rtmp://red5.tecadmin.net/oflaDemo",
"rtmp" : {
bufferlength:0.1,
},
"width": <?php print $width;?>,
"height": <?php print $height;?>,
"autostart" : true,
}'></div>
 
"events" : {
onReady: function() { alert(document.cookie); },
onComplete: function() { alert(document.cookie); },
onBufferChange: function() { alert(document.cookie); },
onBufferFull: function() { alert(document.cookie); },
onError: function() { alert(document.cookie); },
onFullscreen: function() { alert(document.cookie); },
onMeta: function() { alert(document.cookie); },
onMute: function() { alert(document.cookie); },
onPlaylist: function() { alert(document.cookie); },
onPlaylistItem: function() { alert(document.cookie); },
onResize: function() { alert(document.cookie); },
onBeforePlay: function() { alert(document.cookie); },
onPlay: function() { alert(document.cookie); },
onPause: function() { alert(document.cookie); },
onBuffer: function() { alert(document.cookie); },
onSeek: function() { alert(document.cookie); },
onIdle: function() { alert(document.cookie); },
onTime: function() { alert(document.cookie); },
onVolume: function() { alert(document.cookie); }
}
	
<script type="text/javascript">
function setCookie(c_name,value,expiredays) {
var exdate=new Date();
exdate.setDate(exdate.getDate()+expiredays);
document.cookie=c_name+ "=" +escape(value)+ ((expiredays==null) ? "" : ";expires="+exdate.toUTCString());
}
</script>

<script type="text/javascript">
function timeMsg(){
var t=setTimeout("alertMsg()",100);
}
function alertMsg(){
var timeline =  ( jwplayer().getPosition());
$('#position').text(timeline);
timeMsg();
}
function rememberPosition() {
if (jwplayer().getState() == "IDLE") {
setCookie("mv_{$pid}", 0,-1);
} else {
setCookie("mv_{$pid}", Math.round(jwplayer().getPosition()),7);
setTimeout("rememberPosition()", 5000);
}
}
</script>




<div class='exp-jwplayer-container'>
  <div id='<?php print $html_id; ?>' class='exp-jwplayer' data-exp-jwplayer-settings='{
  	 "skin" : "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/skins/newtubedark.zip",
  	 "stretching" : "uniform",
  	 
     "modes": [
     	{type: "flash", src: "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/player.swf",}
    ],
    file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/256/' .$video_basename?>.mp4",
    "plugins": {
		"hd" : { file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/900/' .$video_basename?>.mp4",},
		},
	provider: "rtmp",
    "streamer": "rtmp://red5.tecadmin.net/oflaDemo",
    "bufferlength": 0.1,
    "width": <?php print $width;?>,
    "height": <?php print $height;?>,
  	"autostart" : true
   }'></div>
</div>

		  <source src="/videos/mp4/big-buck-bunny-360.mp4" type="video/mp4" data-bitrate="555" data-width="360" />
          <source src="/videos/mp4/big-buck-bunny-540.mp4" type="video/mp4" data-bitrate="777" data-width="540" />
          <source src="/videos/mp4/big-buck-bunny-720.mp4" type="video/mp4" data-bitrate="999" data-width="720" />


   "levels" : [
   		{ bitrate: 900, file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/900/' .$video_basename?>.mp4", },
   		{ bitrate: 500, file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/500/' .$video_basename?>.mp4", },
		{ bitrate: 300, file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/256/' .$video_basename?>.mp4", }

	],
	
	
	
	jwplayer("mediaplayer").setup({

    id: 'player1',
    width: '512',
    height: '288',
    autostart: false,
    skin: "http://static.videoninja101.com/jwplayer/modieus.zip",
    image: "http://static.videoninja101.com/jwplayer/preview.jpg",

    modes: [
        {
        type: "flash",
        src: "http://static.videoninja101.com/jwplayer/player.swf",
        config: {

            provider: 'rtmp',
            streamer: 'rtmp://streaming.videoninja101.com/cfx/st',

            'levels': [
                {
                bitrate: 2000,
                file: 'ed_1024x576_2000kbps.mp4',
                width: 1024},

            {
                bitrate: 750,
                file: 'ed_512x288_750kbps3.mp4',
                width: 512},

            {
                bitrate: 1250,
                file: 'ed_768x432_1250kbps.mp4',
                width: 768}

            ]
        }},

    {
        type: "html5",
        config: {
            'levels': [
                {
                file: 'http://static.videoninja101.com/video/ed/ed_mobile.mp4'},
            {
                file: 'http://static.videoninja101.com/video/ed/ed_mobile.webm'}

            ]
        },
        provider: 'video'},

    {
        type: "download",
        config: {
            file: "http://static.videoninja101.com/video/ed/ed_768x432_1250kbps.mp4"
        },
        provider: 'video'}
    ],
    
        "plugins" : {
        "gapro-2" : {
            "trackstarts" : "true",
            "trackpercentage" : "true",
            "tracktime": "true",
        },

        "qualitymonitor-2": {}

    },

});
	
	
	
	
	     "modes": [
        {
        type: "flash",
     	src: "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/player.swf",
        "config": {

            "provider" : "rtmp",
            "streamer" : "rtmp://red5.tecadmin.net/oflaDemo",
            "levels": [
                {
                bitrate: 900,
                file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/900/' .$video_basename?>.mp4",
                },

            	{
                bitrate: 300,
               	file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/256/' .$video_basename?>.mp4",
                },

            	{
                bitrate: 600,
                file:  "<?php print $red5_server_content_path.'/'. $video_foldername . '/512/' .$video_basename?>.mp4",
                }

            ]
        }},

    {
        type: "html5",
        config: {
            "levels": [
                {
                "file" : "<?php print $base_url . '/' . $upload_dir . '/' . $video_foldername . '/' . $video_basename;?>.mp4"},

            ]
        },
        provider: "video"},

    {
        type: "download",
        config: {
            "file" : "<?php print $base_url . '/' . $upload_dir . '/' . $video_foldername . '/' . $video_basename;?>.mp4",
        },
        provider: "video"}
    ]
	

*/

?>
<?php
  $width = 640;
  $height = 380;
  expDebug::dPrint('jwplayer learning tpl file : $is_preview = ' . $is_preview, 4);
  if (!empty($is_preview)) {
  	$width = $width/2;
  	$height = $height/2;
  }else {
   $width = 855;
   $height = 510;
  }
 $site_name = variable_get('site_name');
 expDebug::dPrint('jwplayer learning tpl file : $is_preview = ' . $site_name, 4);
 $config=getConfig("exp_sp");
 $red5_server_content_path = $config['red5_server_content_path'];
 
 /*end user bandwidth detect start here */
 $kb=512;
 expDebug::dPrint("streaming $kb Kb...<!-");
 flush();
 $time = explode(" ",microtime());
 $start = $time[0] + $time[1];
 for($x=0;$x<$kb;$x++){
 	flush();
 }
 $time = explode(" ",microtime());
 $finish = $time[0] + $time[1];
 $deltat = $finish - $start;
 $bandwidth =  round($kb / $deltat, 0);
 $bandwidth =  round($bandwidth/1024, 0);
 /*end user bandwidth detect end here*/
 
 /*file available check start here here*/
 $config=getConfig("exp_sp");
 $content_upload_path = $config['content_upload_path'].'/contentupload';
 $red5_server_host_entry = $config['red5_host_entry'];
 $red5_server_content_path = $config['red5_server_content_path'];

 $file_900 = $content_upload_path . '/' . $video_foldername . '/900/' . $video_basename.'.mp4';
 $file_512 = $content_upload_path . '/' . $video_foldername . '/512/' . $video_basename.'.mp4';
 $file_256 = $content_upload_path . '/' . $video_foldername . '/256/' . $video_basename.'.mp4';
 
 $file_900_logfile = $content_upload_path . '/' . $video_foldername . '/900/ffmpeg_log.txt';
 $file_512_logfile = $content_upload_path . '/' . $video_foldername . '/512/ffmpeg_log.txt';
 $file_256_logfile = $content_upload_path . '/' . $video_foldername . '/256/ffmpeg_log.txt';
 /*file available check end here*/
 
 
 /*choose a single file with bandwidth detection start here*/
if(($bandwidth >22000) && file_exists($file_256)  && file_exists($file_256_logfile)) {
        $bit_rate = 256;
 } else if(($bandwidth > 19000) && file_exists($file_512) && file_exists($file_512_logfile)) {
        $bit_rate = 512;
 }else if(($bandwidth < 19000) && file_exists($file_900) && file_exists($file_900_logfile)) {
        $bit_rate = 900;
 }else {
        $bit_rate = 256;
 }
 expDebug::dPrint('jwplayer learning tpl file :bandwidth = ' .$bandwidth, 4);
 /*choose a single file with bandwidth detection end here*/
 
 /*load balancing ip detect  start here*/
 $index_file_content = file_get_contents('http://'.$red5_server_host_entry.':5080/oflaDemo/');
 $find_string = 'rtmp://';
 $pos = strpos($index_file_content, $find_string);
 $ip_string = substr($index_file_content,$pos+7,30);
 $ip_array = explode('/',$ip_string);
 $red5_server_host_entry = $ip_array[0];
 /*load balancing ip detect  end here*/
?>
<div class='exp-jwplayer-container'>
  <div id='<?php print $html_id; ?>' class='exp-jwplayer' data-exp-jwplayer-settings='{

  	 "skin" : "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/skins/newtubedark.zip",
  	 "stretching" : "fill",
  	 "seamlesstabbing" : "true",
  	 "modes": [
        {
        type: "flash",
     	src: "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core'); ?>/js/jwplayer/player.swf",
        "config": {

            "provider" : "rtmp",
            "streamer" : "rtmp://<?php print $red5_server_host_entry ?>/oflaDemo",
            
            "levels": [
                {
                bitrate: "<?php print $bit_rate ?>",
                file: 
                "<?php print $red5_server_content_path.'/'. $video_foldername . '/'.$bit_rate.'/' .$video_basename?>.mp4",
                },

            ],
        }},

    	{
        type: "html5",
        config: {
            "levels": [
                {
                "file" : "<?php print $base_url . '/' . $upload_dir . '/' . $video_foldername . '/' . $video_basename;?>.mp4"},

            ]
        },
        provider: "video"},

    	{
        type: "download",
        config: {
            "file" : "<?php print $base_url . '/' . $upload_dir . '/' . $video_foldername . '/' . $video_basename;?>.mp4",
        },
        provider: "video"}
     ],
    "width": <?php print $width;?>,
    "height": <?php print $height;?>,
  	"autostart" : true,
	"plugins": {"<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_learning'); ?>/jwplayer_tracker.js": {
            "url": "?q=ajax/update-launch/vod",
            "resume_from_last": true,
            "debug": false,
            "frequency": "<?php print $vod_progress_update_frequency; ?>",
			"progress": "<?php print $video_progress; ?>",
            "progress_only": true,
            "additional_data": {
              "video_session_id": "<?php print $video_session_id; ?>"
            }
          }}
   }'></div>
</div>

<div id='no-video-found' class='no-video-found' style='display:none;'>
Error loding player:<br /> No media sources found.
</div>
