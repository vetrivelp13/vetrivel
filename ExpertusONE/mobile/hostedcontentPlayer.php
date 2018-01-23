<!DOCTYPE html>
<html>
    <meta name="viewport" content="width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=0;" />
    <script type="text/javascript" src="jquery.min.js"></script>
    
        <link href="videojs/videojs.css" rel="stylesheet">
        <script src="videojs/video.js"></script>
        <script src="videojs/vjs_tracker.js"></script>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
        <script src="https://code.jquery.com/mobile/1.4.5/jquery.mobile-1.4.5.min.js"></script>
        
        <style>
        
        body{
            background:grey;
            /* border:1px solid red; */
        }
        
        .video-js {padding-top: 56.25%}
        .vjs-fullscreen {padding-top: 0px}

.outer {
    display: table;
    position: absolute;
    height: 100%;
    width: 100%;
}
        
        .middle {
            display: table-cell;
            vertical-align: middle;
        }
        
        .inner {
            margin-left: auto;
            margin-right: auto;
            width: /*whatever width you want*/;
        }

        #player_header{
            position:absolute;
            top:0px;
            right:0px;
            z-index:999999;
            height:45px;
            width:100%;
            border: 0px solid #999999;
            display:none;
            
        }
        

        </style>
        
        

   <body>
     

<div id="player_header">
                <div>
                    <span style='margin-top: 12px; display:block;'><img id='imglogoback' src='icons/icon_back_16W.png' style='width:20px;height:20px' onclick="closevideo1();"></span>
                    <span style='margin-top: -25px;font-size: 14px; color: #FFFFFF;padding-left: 25px; display:block;width: 60%;height: 20px;overflow: hidden;' id='content_header' class="Roboto_Medium"></span>
                    <span style="float:right;color:#FFFFFF;margin-top: -20px;margin-right: 10px;" id="player_count"></span>
                </div>
            </div>
            
            <div onclick="toggleplayerheader();" id="call_header"></div>
      
            <div onclick="pausevideo();" id="pause_video"></div>
      <script>
      function toggleplayerheader(){

      }
          
    //code added 
    
      function getParam(name)
      {
          
          name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
                                var regexS = "[\\?&]"+name+"=([^&#]*)";
                                var regex = new RegExp( regexS );
                                var results = regex.exec(window.location.href);
                                if(results == null)
                                return "";  
                                else    
                                return results[1];
                                
    }
                                
    function closevideo(){
                                $(".video-js")[0].player.pause();
                             //   alert(JSON.stringify(video_data));
         /* $('.vimeo').remove(); */
         /* window.location.href = "closevideo://expertus.com?status=success"; */
         window.location.href='closevideo://expertus.com?data='+JSON.stringify(video_data);
    }
    
      								  function closevideo1(){
                                      $(".video-js")[0].player.pause();
                                      window.location.href='changeplayerpage://expertus.com?data='+JSON.stringify(video_data);
      
                                      }
                                      
                                      function pausevideo(){
                                      //alert(12)
                                      window.location.href='pausevideo://expertus.com?data='+JSON.stringify(video_data);
                                      }
                                      
                                      function playVideo(){
                                      window.location.href='playvideo://expertus.com?data='+JSON.stringify(video_data);
                                      }
                                       
    
    var contentType = getParam("type");
    var videourl = getParam("videourl");
    var hosturl = getParam("hosturl");
    var previous_position = getParam("prevPosition");
    
    
                                var previous_position = getParam("prevPosition");
                                var current_position = 0;
                                var video_duration = 0;
                                var progress = 0;
                                var session_id = guid();
                                var interval = '';
                                
                                var video_data = {
                                session_id: session_id,
                                previous_position: previous_position,
                                current_position: current_position,
                                progress: progress,
                                video_duration: video_duration,
                                from: 'mobile',
                                videoType:contentType
                                };
                                
                                
                                function guid() {
                                function _p8(s) {
                                var p = (Math.random().toString(16) + "000000000").substr(2, 8);
                                return s ? "-" + p.substr(0, 4) + "-" + p.substr(4, 4) : p;
                                }
                                return _p8() + _p8(true) + _p8(true) + _p8();
                                }
                                
                                console.log(' session_id  '+session_id);



                                jQuery(document).ready(function () {
                                                       
                                                       var height = $(window).height();
                                                       var width = $(window).width();
                                                       
                                                      var playerHtml ='<video playsinline id="example_video_1" class="video-js vjs-default-skin vjs-fullscreen" controls preload="none"  width="auto" height="auto"  webkit-playsinline       poster=""   data-setup="{}">'
                                                       playerHtml += '<source src="'+videourl+'" type="video/mp4" />'
                                                       playerHtml +='</video>';
                                                       
                                                       
                                                    $(".inner").append(playerHtml);
                                                       
                                                       
                                                       var video = _V_("example_video_1");
                                                       var v = new VjsTracker(video,
                                                                              {
                                                                              url: 'http://requestb.in/usx6xfus',
                                                                              progress: previous_position,
                                                                              resume_from_last: true,
                                                                              debug: true,
                                                                              frequency: 10,
                                                                              progress_only: true
                                                                              });
                                                       });
                                



 
//                                       window.addEventListener("orientationchange", function() {
//                                       window.setTimeout(function(){
//                                         if(window.innerHeight > window.innerWidth){
//                                         $('#player_header').show();
//                                         }
//                                         else
//                                         {
//                                         $('#player_header').hide();
//                                         }
//                                         },300);
//                                       }, false);
                                      


          </script>
      



<div class="outer">
    <div class="middle">
        <div class="inner">
            
            
        </div>
    </div>
</div>



<!--<video id="example_video_1" class="video-js vjs-default-skin" controls preload="auto" width="640" height="264"-->
<!--    poster="http://video-js.zencoder.com/oceans-clip.png"-->
<!--    data-setup="{}">-->
<!--    <source src="https://mnqa.exphosted.com/sites/default/files/contentupload/Course-ClasslStructurelv1a_6/Course-ClasslStructurelv1a.mp4" type='video/mp4' />-->
<!--    -->
<!--    <p class="vjs-no-js">To view this video please enable JavaScript, and consider upgrading to a web browser that <a href="http://videojs.com/html5-video-support/" target="_blank">supports HTML5 video</a></p>-->
<!--</video>-->


<div onclick="closevideo1();" id="closebtn"  style="position:absolute;top:0px;right:0px"></div>



</body>
</html>
