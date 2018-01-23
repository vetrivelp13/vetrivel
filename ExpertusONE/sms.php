
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8" />
        <title>ExpertusONE Mobile</title>
        <meta name="viewport" content="width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no" />
		
        <script>
              var IS_IPAD = navigator.userAgent.match(/iPad/i) != null,
            IS_IPHONE = !IS_IPAD && ((navigator.userAgent.match(/iPhone/i) != null) || (navigator.userAgent.match(/iPod/i) != null)),
            IS_IOS = IS_IPAD || IS_IPHONE,
    		IS_ANDROID = !IS_IOS && navigator.userAgent.match(/android/i) != null,
   			IS_MOBILE = IS_IOS || IS_ANDROID;
   			
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
     
   			
   			
function call(){

       var id = getParam("id");
       document.getElementById("msgdetails").innerHTML = "Please Install ExpertusONE Mobile app in your device to see the message details.";
       if (IS_IOS) {
		 
        window.location = "expertusone://view?id="+id;
    } else if (IS_ANDROID) {
        window.location = 'expertusone://view?id='+id;
    }
      
          }
            </script>

    </head>
    <body onload="call();">
    	
		<div id="msg" style="display:block;font-size:20px;float:left;margin-top:5px; font-weight:lighter; text-shadow:none;"><div id="msgdetails" class="font_14" style="width:100% ;text-align:center;">Launching ExpertusONE Mobile...<br/><br/> </div></div>
		
		
		
    </body>
    
</html>