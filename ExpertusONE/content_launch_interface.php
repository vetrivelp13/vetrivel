<?php 
/*
 * This is sample interface for content launch in mobile device. It needs to be redesigned after SCORM content test passed.
*/

?>
<html>
<head>

	<script src="sites/all/themes/core/AdministrationTheme/helpContent/js/jquery-1.3.2.min.js" type="text/javascript"></script>
	<script src="sites/all/themes/core/AdministrationTheme/helpContent/js/JQuery-ui.js" type="text/javascript"></script>

  <script src="apis/core/SCORM-API-WRAPPER-MOBILE.js"></script>
  <script src="sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/js/exp_sp_registrationdetail/SCORM12-LMS-API.js"></script>
  <script src="sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/js/exp_sp_registrationdetail/SCORM2004-LMS-API.js"></script>
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

$url=$_GET["url"];
$id=$_GET["id"];
$title=$_GET["title"];
$contenttype=$_GET["contenttype"];
$status=$_GET["status"];
$courseId=$_GET["courseid"];
$classId=$_GET["classid"];
$regid=$_GET["regid"];
$userId=$_GET["userid"];
$app_server_host=$_GET["app_server_host"];
$video_url=$_GET["video_url"];

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
	var courseCompletionStatus="lrn_crs_cmp_cmp";
	var score=0;
	var grade='pass';
	var loc="";
	
	var passUrl = 'regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&status='+courseCompletionStatus;
        passUrl+= '&score='+score+'&grade='+grade+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseCompletionStatus+"&action=updatescore&contenttype=<?php echo $contenttype;?>";  //+'&sestime='+sestime;
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
        var passUrl="";
       
	function go()
	{
				var test="<? echo $contenttype;?>";
				// alert(test);
            	//https://v7299.exphosted.com/sites/default/files/contentupload/bop/player.html
            	//childWin=window.open("https://v7299.exphosted.com/sites/default/files/contentupload/bop/player.html","mywin","location=0,toolbar=0,status=0,menubar=0");
    			<?php 	if($contenttype=="Knowledge Content") {?>
    	            childWin=window.open("<?php echo $url; ?>","mywin","location=0,toolbar=0,status=0,menubar=0"); 
                <?php }else { ?>
	                var params 		= {'Id':'<?php echo $id;?>','enrollId':'<?php echo $regid;?>','url1':'<?php echo $url;?>','courseId':'<?php echo $courseId;?>','classId':'<?php echo $classId;?>','url2':'','ErrMsg':'','contentType':'<?php echo $contenttype;?>','Status':'<?php echo $status;?>'};
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
		var pmType 		= data.contentType;
	    var classid 	= data.classId;
	    var courseid 	= data.courseId;
	    var stdid		= <?php echo $userId; ?>;
	    var stdname 	= "<?php echo 'admin'; ?>";
	    var pmPath 		= data.url1;
	    
    	var launchParam =  {
						        'launchUrl'			: data.url1,
						        'userId' 			: stdid,
						        'username'			: stdname,
						        'courseid' 			: courseid,
						        'classid' 			: classid,
						        'lessonid' 			: data.Id,
						        'enrollid' 			: data.enrollId,//this.regId,
						        'prevcontentstatus' : data.Status
						    };   
   

	    if(pmType.toLowerCase() == 'scorm 1.2' || pmType.toLowerCase() == 'scorm 2004' || pmType.toLowerCase()== 'scorm') 
	    {
	    	pmType				= pmType+" 1.2";
	    	var pmTypeVer 		= pmType.split(' ');
	        var contentType  	= pmTypeVer[0];
	        var contentVersion 	= pmTypeVer[1];
	    } 
	    else  
	    {
	        var contentVersion 	= '';
	        var contentType 	= pmType;
	    }
	    
	    var x	= {version: contentVersion,type:contentType};
	    scoobj 	= new SCORM_API_WRAPPER(x);

	
	    var lessonlocation=data.LessonLocation==null||data.LessonLocation==undefined ||data.LessonLocation==''?" ":data.LessonLocation;
	    var data1 = {
	        url				 : pmPath,
	        callback		 : updateScore,
	        learnerId		 : stdid,
	        learnerName		 : stdname,
	        completionStatus : 'not attempted',
	        scoreMax		 : '0',
	        scoreMin		 : '0',
	        score			 : '0',
	        location		 : lessonlocation
	    };	    
	    scoobj.Initialize(data1);
	    
	    <?php if($_GET["testmode"]==true || $_GET["testmode"]== "true"){ ?>
            setTimeout("closeWindow()", <?php echo $_GET["testmode_timeout"]; ?>);
        <?php } ?>
		
	    
	}


	function updateScore ()
	{
	    var sestime					= '0';
        var courseComplitionStatus 	= '';//$('#courseComplitionStatus').val();
        
        var crid					= "<?php echo $courseId;?>";
        var clid					= "<?php echo $classId;?>";
        var lesid					= "<?php echo $id;?>";
        var enrollid				= "<?php echo $regid;?>";
        var stid 					= "<?php echo $userId;?>";
        var prevcontentstatus 		= "<?php echo $prevContentStatus;?>";
        var regId					= "<?php echo $regid;?>";        
        var contentType				= "scorm 1.2";
        var contentVersion			= '';
        var result					= scoobj.Finish();
        var status 					= result.status;
        var scmax 					= '';
        var scmin 					= '';
        var score 					= result.score;
        var sestimear 				= result.sessionTime;
        var loc 					= result.location;
        var compstatus				= result.completionStatus;
        var grade					= '';
        var xstatus					= '';

       	
        if(scoobj._type != "Knowledge Content") 
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
 
         
        passUrl = 'regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&status='+xstatus+'&max='+scmax+'&min='+scmin;
        passUrl = passUrl+'&score='+score+'&grade='+grade+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseComplitionStatus+"&action=updatescore&contenttype=<?php echo $contenttype;?>";  //+'&sestime='+sestime;
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
        </script>
       
        
    </head>
    <body onload="fnc()">
    <h1 id='msg'>Loading Content... Please wait</h1>
    <div id="mediaplayer"></div>
    <!-- <input type='button' value='Close' id='closebtn' onClick='closeWindow();'></input> -->
    </body>
	<?php
}

?>


</html>
