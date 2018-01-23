<?php 
// $url = $_SERVER['DOCUMENT_ROOT'].'?'.$_SERVER['REQUEST_URI'];
// print_r($_SERVER);
// echo "sadasdfdaf".$url;
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Encryption.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/dao/AbstractDAO.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/modules/core/exp_sp_core/exp_sp_core.module";



$hostName = $_SERVER['HTTP_HOST']; 
$protocol = isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off' ? 'https://' : 'http://';
$url = $protocol.$hostName;

	$current_url = $_REQUEST['q'];

	$url_params = strstr($current_url, 'learn');
		expDebug::dPrint('$url_params ==> '.print_r($url_params,true),5);	

	$alter_url = explode('/', strstr($current_url, 'surveylink'));
		expDebug::dPrint('$alter_url ==> '.print_r($alter_url,true),5);
		
	// $surlink_names = array("SurUrlName","SurLinkType","SurUserId","SurObjectId","SurObjectType","SurEnrollId","SurEnrollStatus","SurSurveyId");
	// 	expDebug::dPrint('$surlink_names ==> '.print_r($surlink_names,true),5);
	//
	// $survey_attr = array_combine($surlink_names, $alter_url);
	// expDebug::dPrint('$survey_attr ==> '.print_r($survey_attr,true),5);
	// $decrypted = new Encrypt();
	$decrypted1 = new Encrypt();
	$decrypted2 = new Encrypt();
	$decrypted3 = new Encrypt();
	// $decrypted4 = new Encrypt();
	
	
	$surlink_names = array("SurUrlName","SurLinkType","SurObjectId","SurObjectType","SurSurveyId");
	$survey_attr = array_combine($surlink_names, $alter_url);	
		
	 $survey_attr['SurObjectId'] = ($survey_attr['SurObjectId'] != '') ? $decrypted1->decrypt($survey_attr['SurObjectId']) : $survey_attr['SurObjectId'];
	$survey_attr['SurSurveyId'] = ($survey_attr['SurSurveyId'] != '') ? $decrypted2->decrypt($survey_attr['SurSurveyId']) : $survey_attr['SurSurveyId'];
	$survey_attr['SurObjectType'] = ($survey_attr['SurObjectType'] != '') ? $decrypted3->decrypt($survey_attr['SurObjectType']) : $survey_attr['SurObjectType'];
	
	
	// $survey_attr['SurUserId'] = ($survey_attr['SurUserId'] != 'null') ? $decrypted1->decrypt($survey_attr['SurUserId']) : $survey_attr['SurUserId'];
	// 	expDebug::dPrint('$surUserId ==> '.print_r($surUserId,true),5);
	// $survey_attr['SurObjectId'] = ($survey_attr['SurObjectId'] != 'null') ? $decrypted2->decrypt($survey_attr['SurObjectId']) : $survey_attr['SurObjectId'];
	// 	expDebug::dPrint('$surObjectId ==> '.print_r($surObjectId,true),5);
	// $survey_attr['SurEnrollId'] = ($survey_attr['SurEnrollId'] != 'null') ? $decrypted3->decrypt($survey_attr['SurEnrollId']) : $survey_attr['SurEnrollId'];
	// 	expDebug::dPrint('$surEnrollId ==> '.print_r($surEnrollId,true),5);
	// $survey_attr['SurSurveyId'] = ($survey_attr['SurSurveyId'] != 'null') ? $decrypted4->decrypt($survey_attr['SurSurveyId']) : $survey_attr['SurSurveyId'];
	// 	expDebug::dPrint('$surSurveyId ==> '.print_r($surSurveyId,true),5);

	expDebug::dPrint('$survey_attr ==> '.print_r($survey_attr,true),5);
	expDebug::dPrint('current_url ==> '.print_r($current_url,true),5);



	if(strpos($current_url,'share/training')>=0){
		$arg = explode('/',$current_url);
		expDebug::dPrint('argument2 == '.$arg[2]);
		
		$pdao=new AbstractDAO();
	    $pdao->connect();
	    $qry = "SELECT widget.type AS widget_type, widget.mode AS widget_mode, widget.parameters AS widget_parameters FROM slt_widget widget WHERE  (widget.url = '".$arg[2]."')";
	    expDebug::dPrint("Query == ".$qry,2);
		$res = $pdao->query($qry);
		$param=$pdao->fetchResult();
		$pdao->closeconnect();
		expDebug::dPrint('param111 ==> '.print_r($param,true),5);
						expDebug::dPrint('parameters ==> '.$param->widget_parameters);
		
		$parameters = unserialize($param->widget_parameters);
		
		expDebug::dPrint('parameters ==> '.print_r($parameters,true));
		expDebug::dPrint('parameters ==> '.$parameters['viewlink']);
		
		$sharelink = $parameters['viewlink'];
		
	}



function decrypt($value){
		//echo $value;
		$decrypted = new Encrypt();
		return $decrypted->decrypt($value);
}

?>

<!DOCTYPE html>
<!--
Hosung Hwang
From various sources

Setting URL Scheme
iOS :
http://www.idev101.com/code/Objective-C/custom_url_schemes.html

Android :
<activity android:name="MyApp" android:label="@string/app_name" 
          android:screenOrientation="portrait"
          android:launchMode="singleTop"
          android:configChanges="orientation|keyboardHidden">

    <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>

    <intent-filter>
      <data android:scheme="myprotocol" />
      <action android:name="android.intent.action.VIEW" />
      <category android:name="android.intent.category.BROWSABLE" />
      <category android:name="android.intent.category.DEFAULT" />
    </intent-filter>
</activity>
-->
<html>
<head>
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link href='https://fonts.googleapis.com/css?family=Roboto:500' rel='stylesheet' type='text/css'>
	<style>
		@import url(http://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300ita‌​lic,400italic,500,500italic,700,700italic,900italic,900);
		html, html * {
		  font-family: Roboto;
		}
		
		
		
	body{
		
		margin:0px;
		background :#d3d3d3;
         background-origin: content-box;
         background-repeat: no-repeat;
	}
	
	#prompt{
	position: absolute;
    width: 100%;
    bottom: 20px;
	}
	
	#header{
	text-align: center;
    position: absolute;
    width: 100%;
    top: 30px;
    color: #fff;
		
	}
	
	#phone_img {
    position: absolute;
    width: 100%;
    top: 100px;
    bottom: 140px;
    overflow: hidden;
	}
	
	#phone_img img {
    width: 200px;
    display:table ! important;
    margin:0 auto ! important;
	}
	
	#text-sec{
		width:100%;
		right:0px;
	    bottom: 0px;
		top:0px;
		position:absolute;
	}
	
	
	
	@media only screen and (min-device-width: 320px) 
                   and (max-device-width: 767px) 
                   and (orientation: landscape) {
					   
					   
					   
				   	#phone_img {
				       position: absolute;
				       width: 50%;
				       top: 25px;
				       bottom: 25px;
				       overflow: hidden;
				   	}
	
				   	#phone_img img {
				       width: 200px;
					   margin:0 auto;
					   display:table;
				   	}
					
					#text-sec{
						width:50%;
						right:0px;
						top:0px;
						bottom:0px;
						position:absolute;
					}
	}
	
	
	@media all and (device-width: 768px) and (device-height: 1024px) {
	  
	   	#phone_img img {
	       width: 350px;
		   margin:0 auto;
		   display:table;
	   	}
		
	  
	  
	}

	
	</style>
	

<script type="text/javascript">






		// Market address for android and iphone
		var market_a = "https://play.google.com/store/apps/details?id=com.expertus.mobilelms.v45&hl=en";
		var market_i = "https://itunes.apple.com/us/app/expertusone-mobile-4.5/id975820681?mt=8";
		var root = "<?php echo $url; ?>"
		var IS_IPAD = navigator.userAgent.match(/iPad/i) != null,
			IS_IPHONE = !IS_IPAD && ((navigator.userAgent.match(/iPhone/i) != null) || (navigator.userAgent.match(/iPod/i) != null)),
			IS_IOS = IS_IPAD || IS_IPHONE,
			IS_ANDROID = !IS_IOS && navigator.userAgent.match(/android/i) != null,
			IS_MOBILE = IS_IOS || IS_ANDROID;
			

		function checkAppInstall() {
			//schema of the app
			var params =  getParam('q');
			var params_array = params.split('/');
			
			var module = params_array[1];
			var intenturl = '';
			if(module == 'surveylink'){
				var SurUrlName = "<?php echo $survey_attr['SurUrlName']; ?>";
				var SurLinkType = "<?php echo $survey_attr['SurLinkType']; ?>";
				var SurUserId = "<?php echo $survey_attr['SurUserId']; ?>";
				var SurObjectId = "<?php echo $survey_attr['SurObjectId']; ?>";
				var SurObjectType = "<?php echo $survey_attr['SurObjectType']; ?>";
				var SurEnrollId = "<?php echo $survey_attr['SurEnrollId']; ?>";
				var SurEnrollStatus = "<?php echo $survey_attr['SurEnrollStatus']; ?>";
				var SurSurveyId = "<?php echo $survey_attr['SurSurveyId']; ?>";				
				intenturl = "survey?SurUrlName="+SurUrlName+"&SurLinkType="+SurLinkType+"&SurUserId="+SurUserId+"&SurObjectId="+SurObjectId+"&SurObjectType="+SurObjectType+"&SurEnrollId="+SurEnrollId+"&SurEnrollStatus="+SurEnrollStatus+"&SurSurveyId="+SurSurveyId+"&surveylink=1";
			}else if(module == 'learning'){
				var entityType = params_array[2];
				var entityID = params_array[3]
				intenturl = "share?id="+entityID+"&entityType="+entityType+"&shareclass=1";
			}
			else if(module == 'training'){
				var sharelink = "<?php echo $sharelink; ?>";
				var urlparam = getURLParam('q',sharelink);
				var arg = urlparam.split('/');
				var entityType = arg[2];
				var entityID = arg[3]
				intenturl = "share?id="+entityID+"&entityType="+entityType+"&shareclass=1";
			}else{
				visitWebsite();
				return false;
			}
			
			if(IS_ANDROID) {
				var url = window.location.href;    
				if (url.indexOf('?') > -1){
				   url += '&Appinstalled=0'
				}else{
				   url += '?Appinstalled=0'
				}
				window.location.href="intent://"+intenturl+"#Intent;scheme=expertusone;package=com.expertus.mobilelms.v45;S.browser_fallback_url="+url+";end"
			} else if(IS_IOS) {
			
				var url = "expertusone://"+intenturl;
				
				setTimeout( function() {
					document.getElementById('prompt').style.display = 'block';
				}, 3000);
		 
				//location.href = url;
				
				window.open(url)
				
			} else {
				document.getElementById('prompt').style.display = 'block';
// 				alert("android and iOS only");
			}
		 
			return false;
		}
		 
		 

		 
		// open market
		function goMarket() {
			if(IS_ANDROID) {
				location.href=market_a;
			} else if(IS_IOS) {
				location.href=market_i;
			} else {
				// do nothing
			}
		}

		function dovalidation(){
		

		
			document.body.style.backgroundImage = "url("+root+"/mobile/icons/App_bg.png)";


			if(IS_ANDROID) {
				document.getElementById('OS_APP_LOGO').src=root+"/mobile/icons/and.png";
				document.getElementById('OS_STORE_LOGO').src=root+"/mobile/icons/playstore.png";
			} else if(IS_IOS) {
				document.getElementById('OS_APP_LOGO').src=root+"/mobile/icons/iph.png";
				document.getElementById('OS_STORE_LOGO').src=root+"/mobile/icons/appstore.png";
			}
				var siteurl = window.location.href;
				if(siteurl.indexOf('visitwebsite') > -1){
				  visitWebsite();
				return false;
				}
				
			
			
			window.setTimeout(function(){	
			if(getParam('Appinstalled') == ""){
				checkAppInstall();
			}
			else{
				document.getElementById('prompt').style.display = 'block';
				
			}
			},1000);
		}




		function openMarket(){
			if(IS_ANDROID) {
				window.location.href="market://details?id=com.expertus.mobilelms.v45";
			} else if(IS_IOS) {
				location.href=market_i;
			} else {
				// do nothing
			}
			
		}

		function visitWebsite(){
			var url = window.location.href;    
			if (url.indexOf('?') > -1){
			   url += '&visitwebsite=1'
			}else{
			   url += '?visitwebsite=1'
			}
			window.location.href=url;
		}
		
		
		
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
				
		
		function getURLParam(name,url)
		{  

		    name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");  
		    var regexS = "[\\?&]"+name+"=([^&#]*)";  
		    var regex = new RegExp( regexS );  
		    var results = regex.exec(url);
		    if(results == null)
		        return "";  
		    else    
		        return results[1];
		       
		}
			
				
		
	</script>
</head>

<body onload="dovalidation()">
	
	
	<div id="phone_img">
		<img id="OS_APP_LOGO" src="icons/iph.png" style="" alt="ExpertusONE"></img>
	</div>
	
	
<div id="text-sec">
	<div id="header">
		<div style="font-size:25px">Get the ExpertusONE App </div>
		<div>Easy to learn anytime, anywhere</div>
	</div>
	
	
	<div id="prompt" style="display:block">
	<!-- <h1>Do you want to install the mobile app or visit website</h1><br> -->
	<div onclick="openMarket()" style="padding:5px"><img id="OS_STORE_LOGO" src="icons/appstore.png" style="display:table;margin:0 auto;width:120px;height"></img></div>
	<!-- <div onclick="openMarket()" style="background:none;border:1px solid #0096F8;max-width:60%;display:table;margin:0 auto;padding:5px;border-radius:3px;font-size:14px;color:#0096F8;width:50%;text-align:center">Get the App</div> -->
	<div onclick="visitWebsite()" style="max-width:60%;display:table;margin:0 auto;font-size:14px;color:#0096F8;font-family:Roboto">Continue to Website</div><br>
	</div>
	
</div>
	<!-- <a id="android_btn" href="intent://scan/#Intent;scheme=expertusone;package=com.expertus.mobilelms.v45;S.browser_fallback_url=https%3A%2F%2Fplay.google.com%2Fstore%2Fapps%2Fdetails%3Fid%3Dcom.expertus.mobilelms.v45%26hl%3Den;end"> Take a QR code </a> -->
	
 	
 
	<!-- <div id="invisible_div"  ><img src="" id="appimg" style="display:table;margin:0 auto;width:50%"></img></div>
	<div id="display"> </div> -->
</body>  
</html>
