<?php 
/*
 * This is sample interface for content launch in mobile device. It needs to be redesigned after SCORM content test passed.
*/

?>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

	<script src="../sites/all/themes/core/AdministrationTheme/helpContent/js/jquery-1.3.2.min.js" type="text/javascript"></script>
	<script src="../sites/all/themes/core/AdministrationTheme/helpContent/js/JQuery-ui.js" type="text/javascript"></script>

  <script src="../apis/core/SCORM-API-WRAPPER-MOBILE.js"></script>
  <script src="../sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/js/exp_sp_registrationdetail/SCORM12-LMS-API.js"></script>
  <script src="../sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/js/exp_sp_registrationdetail/SCORM2004-LMS-API.js"></script>
  <title>ExpertusONE - Video Player</title>
</head>
    
<?php 
header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header("Cache-Control: no-cache"); 
header("Pragma: no-cache"); 
$url="";
$id="";
$title="";
$contenttype="";
$status="";
$courseId="";
$classId="";
$regid="";
$userId="";
$prevContentStatus="";
$suspendData = "";
$show_button = "";
$version = "";

$url=$_GET["url"];
$id=$_GET["id"];
$title=$_GET["title"];
$contenttype=$_GET["contenttype"];
$status=$_GET["status"];
$prevContentStatus=$_GET["status"];
$courseId=$_GET["courseid"];
$classId=$_GET["classid"];
$regid=$_GET["regid"];
$userId=$_GET["userid"];
$app_server_host=$_GET["app_server_host"];
$video_url=$_GET["video_url"];
$lessonLocation=$_GET["LessonLocation"];
$launch_data=$_GET["launch_data"];
$launchId=$_GET["launchId"];
$versionId=$_GET["versionId"];
$contentId=$_GET["contentId"];
$host = $_GET["host"];
$AICC_SID = $_GET["AICC_SID"];
$suspendData = $_GET["SuspendData"];
$suspendData = str_replace("done@@","done=",$suspendData);
$access_token = $_GET["access_token"];
$show_button = $_GET["show_button"];
$version = $_GET["version"];
$query = $_SERVER['REQUEST_URI'];
$masteryscore = $_GET["masteryscore"];

?>

<?php 
if($contenttype=="hostedvideo")
{

  ?>
<script>

function loadVideo()
{
	//alert(urlparams["video_url"]);
	jwplayer('mediaplayer').setup({
		    'id': 'playerID',
		    'width': '480',
		    'height': '270',
		    'file': urlparams["video_url"],
		    
		    'modes': [
		        {type: 'html5'},
		        {type: 'download'}
		    ]
		  });
}

function closeVideoWindow()
{
	var crid					= "<?php echo $courseId;?>";
	var clid					= "<?php echo $classId;?>";
	var lesid					= "<?php echo $id;?>";
	var enrollid				= "<?php echo $regid;?>";
	var stid 					= "<?php echo $userId;?>";
	var prevcontentstatus 		= "<?php echo $prevContentStatus;?>";
	var regId					= "<?php echo $regid;?>";     

	$launchId=$_GET["launchId"];
	$versionId=$_GET["versionId"];
	$contentId=$_GET["contentId"];
	
	var launchId					= "<?php echo $launchId;?>";     
	var versionId					= "<?php echo 	$versionId;?>";     
	var contentId					= "<?php echo $contentId;?>";     
	
	
	var courseCompletionStatus="lrn_crs_cmp_cmp";
	var score=0;
	var grade='pass';
	var loc="";
	
	var passUrl = 'regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&status='+courseCompletionStatus;
        passUrl+= 'launchId='+launchId+'&versionId='+versionId+'&contentId='+contentId+'&score='+score+'&grade='+grade+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseCompletionStatus+"&action=updatescore&contenttype=<?php echo $contenttype;?>";  //+'&sestime='+sestime;
    var appUrl="x-expertusone-oauth://callback?"+passUrl;
	
	window.location.href=appUrl;
	
}
</script>

 <div class="exp-jwplayer-container">
 	<div id="vod_112_12_wrapper" 
		style="position: relative; width: 640px; height: 380px;">
		<object width="100%" 
			height="100%" 
			type="application/x-shockwave-flash" 
			data="<?php echo $app_server_host;?>/sites/all/modules/core/exp_sp_core/js/jwplayer/player.swf" 				
			bgcolor="#000000" 
			id="videoplayer" 
			name="videoplayer" 
			tabindex="0">
		<param name="allowfullscreen" 
			value="true">
		<param name="allowscriptaccess" 
			value="always">
		<param name="seamlesstabbing" 
			value="true">
		<param name="wmode" 
			value="opaque">
		<param name="flashvars" 
			value="netstreambasepath=<?php echo $app_server_host;?>%2F%3Fq%3Dlearning%2Fenrollment-search&amp;id=vod_112_12&amp;className=exp-jwplayer%20exp-jwplayer-initialized&amp;skin=<?php echo $app_server_host;?>%2Fsites%2Fall%2Fmodules%2Fcore%2Fexp_sp_core%2Fjs%2Fjwplayer%2Fskins%2Fnewtubedark.zip&amp;stretching=fill&amp;image=<?php echo $app_server_host;?>%2Fsites%2Fdefault%2Ffiles%2Fcontentupload%2FTestVideo5%2FTestVideo5_thumbnail.png&amp;file=<?php echo $app_server_host."".$video_url;?>&amp;controlbar.position=over&amp;logo.file=<?php echo $app_server_host;?>%2Fsites%2Fall%2Fthemes%2Fcore%2Fexpertusone%2Flogo.png&amp;logo.link=<?php echo $app_server_host;?>&amp;logo.linktarget=_blank&amp;logo.position=top-right&amp;logo.margin=12&amp;logo.timeout=5">
		</object>
	</div>
	</div>
	<div style='position: relative; width: 640px; height: 380px;text-align:center;padding-top:5px;'>
		<input type='button' name='Close' value='Close' onclick='closeVideoWindow()'></input>
	</div>

<?php }else{

     ?>
    
        <script type="text/javascript">
        var childWin="",interval=0,idx=0;
        var passUrl="";var launchParam="";
		var scorm_api_init = null;
		
       
	function go()
	{
				var test="<? echo $contenttype;?>";
				// alert(test);
            	//http://macdev04.expertusone.com/sites/default/files/contentupload/bop/player.html
            	//childWin=window.open("http://macdev04.expertusone.com/sites/default/files/contentupload/bop/player.html","mywin","location=0,toolbar=0,status=0,menubar=0");
    			<?php 	if($contenttype=="Knowledge Content") {?>
    	            childWin=window.open("<?php echo $url; ?>","mywin","location=0,toolbar=0,status=0,menubar=0"); 
                <?php }else { ?>
 	                var params 		= {'Id':'<?php echo $id;?>','enrollId':'<?php echo $regid;?>','url1':'<?php echo $url;?>','courseId':'<?php echo $courseId;?>','classId':'<?php echo $classId;?>','url2':'','ErrMsg':'','contentType':'<?php echo $contenttype;?>','Status':'<?php echo $status;?>','LessonLocation':'<?php echo $lessonLocation;?>','SuspendData':'<?php echo $suspendData;?>','launch_data':'<?php echo $launch_data;?>','host':'<?php echo $host;?>','aicc_sid':'<?php echo $AICC_SID;?>','versionid':'<?php echo $versionId; ?>','access_token':'<?php echo $access_token; ?>','show_button':'<?php echo $show_button;?>','version':'<?php echo $version;?>','query_string':'<?php echo $query;?>','masteryscore':'<?php echo $masteryscore;?>' };
	                launchWBTContent(params);
                <?php }?>	
    }
	function fnc()
	{
			    // alert(1);
			    <?php 	if($contenttype!="hostedvideo") {?>
  	            var buttonnode= document.createElement('input');
  	            // alert(2);
                  buttonnode.setAttribute('type','button');
                  buttonnode.setAttribute('name','sal');
                  buttonnode.setAttribute('style','display:none;');
                  // alert(3);
                  document.body.appendChild(buttonnode);
                  // alert(4);
                  buttonnode.onclick = go;
                  // alert(5);
                  setTimeout(function() { buttonnode.click() }, 100);
                  <?php } else {?>
                    $("#msg").html("");
            		//alert(outstr);
            		//alert("before load video");
            		//loadVideo();
            		alert($("#mediaplayer").size());
            		loadVideo();
            		//$("body").append('<div id="mediaplayer"></div>');
            		$("body").append('<input type="button" name="Close" value="Close" onclick="closeVideoWindow()"></input>');
            		<?php } ?>
                
    }

	function launchWBTContent(data)
    {
    
		var pmType 		= unescape(data.contentType);
	    var classid 	= data.classId;
	    var courseid 	= data.courseId;
	    var stdid		= <?php echo $userId; ?>;
	    var stdname 	= "<?php echo 'admin'; ?>";
	    var pmPath 		= data.url1;
        var suspenddata = unescape(data.SuspendData);
        var host        = data.host;
        var id			= data.Id; //lessonid for aicc
        var enrollId    = data.enrollId;
        var versionid   = data.versionid
        var aicc_sid    = data.aicc_sid
        var query_string = data.query_string;
        var contenturl = query_string.substring(query_string.indexOf('&url=')+5,query_string.indexOf('&title='));
        pmPath = unescape(contenturl);
        pmPath = decodeURIComponent(pmPath);
    	this.launchParam =  {
						        'launchUrl'			: data.url1,
						        'userId' 			: stdid,
						        'username'			: stdname,
						        'courseid' 			: courseid,
						        'classid' 			: classid,
						        'lessonid' 			: data.Id,
						        'enrollid' 			: data.enrollId,
						        'versionid' 		: data.versionid,//this.regId,
						        'prevcontentstatus' : data.Status,
						        'aicc_sid'          : data.aicc_sid,
						        'host'              : data.host,
						        'access_token'      : data.access_token,
						        'show_button'      : data.show_button,
    							'version'      : data.version
						        };
   

//alert('launchwbt content....3');
		var contentVersion = "";
		var contentType 	= "";
		var pmTypeVer = "";
		
	    if(pmType.toLowerCase() == 'scorm 1.2' || pmType.toLowerCase()== 'scorm') 
	    {
	    	pmType				= pmType+" 1.2";
	    	pmTypeVer 		= pmType.split(' ');
	        contentType  	= pmTypeVer[0];
	        contentVersion 	= pmTypeVer[1];
	      // alert('launchwbt content....4');
	    } else if(pmType.toLowerCase() == 'scorm 2004'   ) {
	    	//pmType				= pmType+" 2004";
	    	
	    	pmTypeVer 		= pmType.split(' ');
	        contentType  	= pmTypeVer[0];
	        contentVersion 	= pmTypeVer[1];
	        
	    }
	    else  
	    {
	         contentVersion 	= '';
	         contentType 	= pmType;
	     // alert('launchwbt content....5');
	    }
	    

	  
	    
	    var x	= {version: contentVersion,type:contentType,host:host,callback:''};
	    scoobj 	= new SCORM_API_WRAPPER(x);

	
	    // var lessonlocation=data.LessonLocation==null||data.LessonLocation==undefined ||data.LessonLocation==''?" ":data.LessonLocation;
	    var lessonlocation=data.LessonLocation==null||data.LessonLocation==undefined ||data.LessonLocation=='null'||data.LessonLocation==''?"":data.LessonLocation;

	    // if(lessonlocation == '' || lessonlocation == null || lessonlocation == "null" || lessonlocation == 'undefined' || lessonlocation == 'NaN')
		// {
	    	// lessonlocation = 0;
		// }
		
        var launch_data=data.launch_data==null||data.launch_data==undefined ||data.launch_data=='null'||data.launch_data==''?"":data.launch_data;
		
		    
		var aicc_id =stdid+"-"+courseid+"-"+classid+"-"+id+"-"+versionid+"-"+enrollId+"-"+lessonlocation;
		    
		if(scoobj._type.toLowerCase() == 'scorm') {
			scorm_api_init = new Object();
			scorm_api_init.session_time = '0000:00:00.00';
			scorm_api_init.lesson_location = lessonlocation;
			scorm_api_init.launch_data = launch_data;
			scorm_api_init.suspend_data = suspenddata;
			scorm_api_init.version = scoobj._version;
			if(scoobj._version == '2004') {
				scorm_api_init.completion_status = 'not attempted';
				scorm_api_init.success_status = 'unknown';
			} else {
				scorm_api_init.lesson_status = 'not attempted';
			}
		}
		  
		  
	    var data1 = {
	        url				 : pmPath,
	        callback		 : this.updateScore,
	        learnerId		 : stdid,
	        learnerName		 : stdname,
	        completionStatus : 'not attempted',
	        scoreMax		 : '0',
	        scoreMin		 : '0',
	        score			 : '0',
	        location		 : lessonlocation,
	        suspend_data	 : unescape(suspenddata),
	        host             : host,
	        aicc_sid         : aicc_sid,
	        courseid         : courseid,
            classid          : classid,
            versionid        : versionid,
            enrollid         : enrollId,
            lessonid		 : id,
            aicc_id          :aicc_id,
            access_token	 : data.access_token,
            show_button      : data.show_button,
            version          : data.version,
            masteryscore     : data.masteryscore
            
         
	    };	  
	    
	    
	    scoobj.Initialize(data1);
	    
	    <?php if($_GET["testmode"]==true || $_GET["testmode"]== "true"){ ?>
            setTimeout("closeWindow()", <?php echo $_GET["testmode_timeout"]; ?>);
        <?php } ?>
		
	    
	}


	function updateScore()
	{ 
		//find out the content type
		 if(scoobj._type == "AICC" || scoobj._type == "AICC Course Structure") 
		{
		console.log("AIcc update score")
	        	var sid = launchParam.aicc_sid;//scoobj._learnerId+"-"+scoobj._courseid+"-"+scoobj._classid+"-"+scoobj._lessonid+"-"+scoobj._versionid+"-"+scoobj._regId;
	        	var passUrl="session_id="+sid+"&command=UPDATELMSDATA";
	        	var aicc_update_url = launchParam.host+"/sites/all/commonlib/AICC_Handler.php";
	        	var post_data={"session_id":scoobj._callback,"command":"UPDATELMSDATA"};
	    		/*$.ajax({
	    			type: "POST",
	    			url: aicc_update_url,
	    			data:  post_data,
	    			success: function(result) {
						console.log("result"+result);
	    			},
	    			error: function(XMLHttpRequest, textStatus, errorThrown) {
						console.log("XMLHttpRequest"+JSON.stringify(XMLHttpRequest));
						console.log("textStatus"+textStatus);
						console.log("errorThrown"+errorThrown);
	    			}
	    			 });*/
		}
		 window.setTimeout(function(){
    
    
		
	    var sestime					= '0';
        var courseComplitionStatus 	= '';//$('#courseComplitionStatus').val();
        
        var crid					= "<?php echo $courseId;?>";
        var clid					= "<?php echo $classId;?>";
        var lesid					= "<?php echo $id;?>";
        var enrollid				= "<?php echo $regid;?>";
        var stid 					= "<?php echo $userId;?>";
        var prevcontentstatus 		= "<?php echo $prevContentStatus;?>";
        var regId					= "<?php echo $regid;?>";        
        var contentType				= unescape("<?php echo $contenttype;?>");
        var contentVersion			= '';
        var result					= scoobj.Finish();
        var status 					= result.status;
        var scmax 					= '';
        var scmin 					= '';
        var score 					= result.score;
        var sestimear 				= result.sessionTime;
        var loc 					= result.location;
        var compstatus				= result.completionStatus;
		var launch_data		 		= result.launch_data;
		var exit			 		= result.exit;
        var grade					= '';
        var xstatus					= '';

        var launchId					= "<?php echo $launchId;?>";     
    	var versionId					= "<?php echo 	$versionId;?>";     
    	var contentId					= "<?php echo $contentId;?>";     
    	var suspend_data            = result.suspendData; //'Varfirst_done=yes;';
    	var aicc_id = '';
    	if(scoobj._type == "Tin Can" || scoobj._type == "tin can"){
    	    	aicc_id = scoobj.aicc_id ;
    	}
    	
		
		var is_scorm_api_changed = true;
    	 if(scoobj._type != "Knowledge Content" && (scoobj._type != "Tin Can" && scoobj._type != "tin can")) {
        	if(prevcontentstatus !='Completed')	{
        		if(scoobj._type.toLowerCase() == 'scorm' && scoobj._version == '2004') {
        			if(((status.toLowerCase()=='unknown' || status.toLowerCase()=='passed' ) && (compstatus.toLowerCase()=='completed' || compstatus.toLowerCase()=='unknown')) ||(compstatus.toLowerCase()=='completed' && status.toLowerCase()=='failed' )){
    		        	xstatus='lrn_crs_cmp_cmp';
    		        }else{
    		        	xstatus='lrn_crs_cmp_inp';
    		        }
        		}
        		else {
        			if(status.toLowerCase()=='completed' || status.toLowerCase() =='passed' 	|| status.toLowerCase()=='unknown' || status.toLowerCase()=='failed'){
    		        	xstatus='lrn_crs_cmp_cmp';
    		        }else{
    		        	xstatus='lrn_crs_cmp_inp';
    		        }
        		}
		       
        	}
        	else{
        		xstatus='lrn_crs_cmp_cmp';
        	}
			
			if(scoobj._type.toLowerCase() == 'scorm') {
				
				
				if(scoobj._version == '2004') {

					if(scorm_api_init.session_time == sestimear && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data && scorm_api_init.completion_status == compstatus && scorm_api_init.success_status == status) {
						is_scorm_api_changed = false;
					}
				} else {
					//comparing initialized values with the current scorm result
					if(scorm_api_init.session_time == sestimear && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data && scorm_api_init.lesson_status == status) {
						is_scorm_api_changed = false;
					}
				}
			}
			
			
        }else if(scoobj._type == "Tin Can" || scoobj._type == "tin can" ){
        	xstatus='lrn_crs_cmp_inp';
        }else{
        	xstatus='lrn_crs_cmp_cmp';
        	compstatus="Completed";
        }
    	
    	
    	/*
    	
        if(scoobj._type != "Knowledge Content" && (scoobj._type != "Tin Can" && scoobj._type != "tin can")) 
        {
        	if(prevcontentstatus !='Completed')	
        	{
		        if(status.toLowerCase()=='completed' || status.toLowerCase()=='passed'	|| compstatus.toLowerCase()=='completed' || compstatus.toLowerCase()=='passed')
		        {
		        	xstatus='lrn_crs_cmp_cmp';
		        }else
		        {
		        	xstatus='lrn_crs_cmp_inp';
		       	}
        	}
        	else
        	{
        		xstatus='lrn_crs_cmp_cmp';
        	}
        }else
        {
        	xstatus='lrn_crs_cmp_cmp';
        }

		*/
        //pending values from wrapper completionStatus totalTime
        if(score==undefined || score=='') 
        {
            score='0';
            grade='';
        }
       /* 
	       	var tmp=sestimear.split(":");
	        if(tmp[0]>0)
	        {
	            sestime = Number(sestime)+Number(tmp[0])*60;
	        }
	        if(tmp[1]>0) 
	        {
	            sestime = Number(sestime)+Number(tmp[1]);
	        }
	        if(tmp[2]>0) 
	        {
	            sestime += "."+Math.round(Number(tmp[2]));
	        }
		*/
         
        passUrl = 'regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&status='+xstatus+'&max='+scmax+'&min='+scmin+'&aicc_id='+aicc_id;
        passUrl = passUrl+'&session_data='+scoobj._callback+'&launchId='+launchId+'&versionId='+versionId+'&contentId='+contentId+'&score='+score+'&grade='+grade+'&location='+loc+'&suspend_data='+suspend_data+'&launch_data='+launch_data+'&is_scorm_api_changed='+is_scorm_api_changed+'&enrollid='+enrollid+'&courseComplitionStatus='+courseComplitionStatus+'&compstatus='+compstatus+'&success_status='+status+"&action=updatescore&contenttype=<?php echo $contenttype;?>";  //+'&sestime='+sestime;
       var appUrl="x-expertusone-oauth://callback?"+passUrl;
       // alert('test:'+xstatus);
        window.location=appUrl;
        //window.location.href="http://www.google.com";
        window.clearInterval(interval);
		//window.location.href=appUrl;
		//alert('test'+result.score);
		//window.location.href=appUrl;
		//alert(appUrl);
		
		
		//scormwindow.close();
		//window.close();
		 },10);
        
     }	

	function forKCContent()
	{
   		try
   		{
       		if(childWin!="" && childWin.closed==false)
       		{
       		}
       		else
       		{
       			<?php 
       					$params="";
    					$app_url="";
    					$params="lessonid=".$id;
    					$params.="&classid=".$classId;
    					$params.="&courseid=".$courseId;
    					$params.="&regid=".$regid;
    					$params.="&action=updatescore";
    					$params.="&contenttype=KnowledgeContent";
    					$params.="&launchId=".$launchId;
    					$params.="&versionId=".$versionId;
    					$params.="&contentId=".$contentId;
    						
						
    					$app_url="x-expertusone-oauth://callback?status=Completed&score=0&".$params;
    					?>
    					window.location.href="<?php echo $app_url;?>";
    					window.clearInterval(interval);
    					window.close();
    		}
       	}catch(e)
   		{
       		<?php 
       			$params="";
				$app_url="";
				$params="lessonid=".$id;
				$params.="&classid=".$classId;
				$params.="&courseid=".$courseId;
				$params.="&regid=".$regid;
				$params.="&action=updatescore";
				$params.="&contenttype=KnowledgeContent";
				$params.="&launchId=".$launchId;
				$params.="&versionId=".$versionId;
				$params.="&contentId=".$contentId;
				$app_url="x-expertusone-oauth://callback?status=Completed&score=0&".$params;
    
			?>
				window.location.href="<?php echo $app_url;?>";
				window.clearInterval(interval);
   		}
    	
	}

	function forScormContent()
	{
   		try
   		{
       		if(scormwindow!="" && scormwindow.closed==false)
       		{
       		}
       		else
       		{
       			updateScore();
       		}
       	}catch(e)
   		{
				updateScore();
   		}
    		
	}
	
	function closeWindow()
    {
        //window.close();
        //setTimeout("window.close();",500);
        updateScore();
       
    }
		
	
    function isClosedd()
    {
        //alert('<?php echo $contenttype;?>');
        <?php 
        if($contenttype=="Knowledge Content")
        {
        	?>
        		forKCContent();
        	<?php 
        }
        else {
        ?>
        		forScormContent();
        <?php }?>
    }
    <?php 
            if($contenttype!="hostedvideo"){?>
    			//interval=window.setInterval("isClosedd();",1000);
    <?php }?>
//code Added by Ashutosh

//  function toggleplayerheader(){
//     	if($('#player_header').is(':visible')){
//     	setTimeout(function(){
//     		$('#player_header').fadeOut();
//     		}, 5000);
//     	}else{
//     		$('#player_header').fadeIn();
//     		toggleplayerheader();
//     	}
//     	}

    function show_play(){
    	
    	var player_width = ($( window ).width() / 2) - 60;
$('.play_icon').css('left',player_width+'px');
    	$('.play_icon').show();
    	}
    function load_player(){
    	$('.play_icon').hide();
    	window.location.href = 'playvideo://callback';
    	}

    function closeWindow1(){
    	 window.location='closekccontent://';
    	}
    
//End of the Code
        </script>
      
      <script>
window.location.hash="no-back-button";
window.location.hash="Again-No-back-button";//again because google chrome don't insert first hash into history
window.onhashchange=function(){window.location.hash="no-back-button";}
</script>  
       
<link href='https://fonts.googleapis.com/css?family=Roboto:500' rel='stylesheet' type='text/css'>      
<style>
#player_header{
position:absolute;
top:0px;
right:0px;
z-index:999999;
 background-color: rgba(153, 153, 153, 0.2); 
height: 13.1vmin;
width:100%;
border: 0px solid #999999;
font-family: 'Roboto', sans-serif;
display:none;

}
/* #content_header{ */
/*  text-shadow: 1px 0 0 #000, 0 -1px 0 #000, 0 1px 0 #000, -1px 0 0 #000; */
/*  } */

#content_header{
text-shadow: 0px 0px #CCCCCC;
}
  </style> 
 
    </head>
    <body onload="fnc()">
    <h1 id='msg'>Loading Content... Please wait</h1>
    <div id="mediaplayer"></div>
    <!-- <input type='button' value='Close' id='closebtn' onClick='closeWindow();'></input> -->
   
    <div onclick="closeWindow()" id="closebtn"></div>
   
    <div onclick="closeWindow1()" id="closebtn1"></div>
   
 <div onclick="toggleplayerheader();" id="call_header1"></div>   
 <div id='change_scorm' onClick='changeVideoWindow();'></div>
    <div id="player_header">
 	<div>
 	<span style='margin-top: 2.9vmin; display:block;'><img id='imglogoback' src='icons/icon_back_16W.png' style='width:7.9vmin;height:7.9vmin;' onclick="closeWindow()"></span>
 	<span style='margin-top: -7.8vmin;font-size: 6.3vmin; color: #FFFFFF;padding-left:  5.9vw; display:block;font-family:'Helvetica Neue';' id='content_header'>Content header title</span>
 	</div>
 	</div>
    <div id="show_icon" onclick="show_play();"></div>
    <div style="position: absolute; z-index: 999999; margin-top: 10%;display:none;" class="play_icon"><img src="icons/youtube.png" onclick="load_player();"></div>
    

 </body>
	<?php
}

?>


</html>
