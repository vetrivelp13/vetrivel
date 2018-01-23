<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head>
<title>Froogaloop2 JavaScript API Example</title>

<meta name="viewport"
	content="width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no" />
<style>
iframe {
	float: left;
	border: 0;
	left: 0px;
	top:0px;
	position:absolute;
}

h4 {
	clear: both;
}

a {
	display: block;
}

.parent{
    display:table;
    width:100%;
    height:100%
}
.child{
    display:table-cell;
    vertical-align:middle;
    width:100%
}
body{
background:grey;
/* border:1px solid red; */
}
</style>
<!-- <script	src="http://ajax.googleapis.com/ajax/libs/jquery/1.6.2/jquery.min.js"></script> -->
<!-- <script src="http://a.vimeocdn.com/js/froogaloop2.min.js"></script> -->
<!--  <script src="https://www.youtube.com/iframe_api"></script> -->
<script	src="jquery.min.js"></script>
<script src="froogaloop2.min.js"></script>
 <script src="yt.js"></script>


<script type="text/javascript">



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
	/* $('.vimeo').remove(); */
	/* window.location.href = "closevideo://expertus.com?status=success"; */
	window.location.href='closevideo://expertus.com?data='+JSON.stringify(data);
}
  
     	var contentType = getParam("type");
     	var videourl = getParam("videourl");
     	var hosturl = getParam("hosturl");
     	
     	var previous_position = getParam("prevPosition");
     	var current_position = 0;
     	var video_duration = 0;
     	var progress = 0;
     	var session_id = guid();
     	var interval = '';

        var data = {
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
         
         
         function updateStatus(){
        	 
        	 console.log(1111111);
        	 
 			interval = setInterval(function(){        
         		 $.ajax({
                 url:"http://requestb.in/z10iiwz1",
                 method: 'POST',
                 data: data,
                 statusCode: {
                   404: function () {
                     console.log("URL not found.");
                   },
                   500: function () {
                     console.log("Error on the server.");
                   }
                 }
               }).done(function (data) {
                 console.log("Server update complete.");
                 
               });
			 }, 3000);
        
        	 
         }
         
 		var player;
		function onYouTubeIframeAPIReady() {
			player = new YT.Player( 'player', {
				events: { 'onStateChange': onPlayerStateChange }
			});
			
		}
		
		function onPlayerStateChange(event) {
			
			switch(event.data) {
			    case -1:
			    	player.seekTo(previous_position,true);
				break;
				case 0:
					setTimeout(function(){ clearInterval(interval); }, 4000);
					break;
				case 1:
					updateyoutube();
					break;
				case 2:
					clearInterval(interval);
					break;
			}
		}

		function record(data){
		    // Do what you want with your data
			var p = document.createElement("p");
			p.appendChild(document.createTextNode(data));
			document.body.appendChild(p);
		}

		function updateyoutube(){
			
			console.log(data);
			data.video_duration = player.getDuration();
 			interval = setInterval(function(){   

 	 			if(data.progress == 100){
				return;
 	 	 		}
 				
 					data.previous_position = data.current_position;
					data.current_position = player.getCurrentTime();
/* 					data.progress = res.percent*100;
 */
 				
 				var percentage = (player.getCurrentTime()/player.getDuration());
                 data.progress = percentage*100;
 				console.log("current"+data.current_position);
 				console.log("previous"+data.previous_position);
 				console.log("total"+data.video_duration);
 				console.log("percentage"+percentage*100);
 				
 				
//         		 $.ajax({
//                 url:"http://requestb.in/z10iiwz1",
//                 method: 'POST',
//                 data: data,
//                 statusCode: {
//                   404: function () {
//                     console.log("URL not found.");
//                   },
//                   500: function () {
//                     console.log("Error on the server.");
//                   }
//                 }
//               }).done(function (data) {
//                 console.log("Server update complete.");
                
//               });
			 }, 2000);

 			console.log("player.getPlayerState()"+player.getPlayerState())
			
		}

        
        jQuery(document).ready(function() {
         	
        	
        	if(contentType.toLowerCase() == 'youtube'){
        		
        		$('.vimeo').css('display','none');
        		$('.youtube').attr('src',videourl+'&enablejsapi=1');
        	}
        	else{
        		
        		$('.youtube').css('display','none');
        		$('.vimeo').attr('src',videourl+'?api=1&player_id=player1');
         	// Enable the API on each Vimeo video
            jQuery('iframe.vimeo').each(function(){
                Froogaloop(this).addEvent('ready', ready);
            });
            
         	

            function ready(playerID){
                console.log(playerID + ' is ready');

                // Add event listerns
                // http://developer.vimeo.com/player/js-api#events
                Froogaloop(playerID).addEvent('play', play(playerID));
                Froogaloop(playerID).addEvent('playProgress', playprogress);
                Froogaloop(playerID).addEvent('finish', onFinish);
                // Fire an API method
                // http://developer.vimeo.com/player/js-api#methods
//                 Froogaloop(playerID).api('play');
//                 Froogaloop(playerID).api('seekTo',previous_position);
            }
            function play(playerID){
                console.log(playerID + ' is playing');
                Froogaloop(playerID).api('seekTo',previous_position);
//                 Froogaloop(playerID).addEvent('seek', 50);
//                 updateStatus()
            }
            function seek(data, playerID) {
                console.log(playerID + ' is seeking');
                console.log(data);
                $('.status_'+playerID).append(playerID + ' is seeking ');
            }
            function playprogress(res,playerID) {
/*                 console.log(playerID + ' is seeking');
                console.log(data);
                
 */               
 	 			if(data.progress == 100){
 					return;
 	 	 	 	}
 
 					 data.previous_position = data.current_position;
 					data.current_position = res.seconds;
 					data.video_duration = res.duration;
 					data.progress = res.percent*100;
                $('.status_'+playerID).append(playerID + ' is playing ' +res.duration +'---- '+res.seconds+'----'+res.percent);
                
            }
            function onFinish() {
            	clearInterval(interval);
            }
       }

  
        });
</script>

</head>
<body>

	<script>
// ajax call   
//          function getstatus(){
        	 
//              $.ajax({
//                  url:"http://requestb.in/usx6xfus",
//                  method: 'POST',
//                  data: data,
//                  statusCode: {
//                    404: function () {
//                      console.log("URL not found.");
//                    },
//                    500: function () {
//                      console.log("Error on the server.");
//                    }
//                  }
//                }).done(function (data) {
//                  console.log("Server update complete.");
                 
//                });

//          }
         </script>

	<!-- <a href="javascript:getstatus();">getstatus</a> -->



<div class="parent" ><div class="child">
	<!-- The id="player_1" of the IFRAME must match the query string parameter "playerID=player_1". You also need to api=1 set. -->
 	<iframe class="vimeo" id="player1"
		src="'+videourl+'"
		frameborder="0" style="overflow:hidden;height:100%;width:100%" height="100%" width="100%" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>
		
		<iframe class="youtube" id="player" src="'+videourl+'" frameborder="0" style="overflow:hidden;height:100%;width:100%" height="100%" width="100%"></iframe>
		
		</div></div>

	<!--         The id="player_2" of the IFRAME must match the query string parameter "playerID=player_2". You also need to api=1 set. 
        <iframe class="vimeo" id="player2" src="http://player.vimeo.com/video/240975?api=1&player_id=player2" width="500" height="281" frameborder="0"></iframe>
 -->


	<!-- <h4>By Drew Baker @ Funkhaus Design</h4> -->
	
	<div onclick="closevideo();" id="closebtn"  style="position:absolute;top:0px;right:0px"></div>
</body>
</html>