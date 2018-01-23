<?php

define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/entity.inc';
require_once DRUPAL_ROOT . '/modules/user/user.module';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
include_once DRUPAL_ROOT .'/apis/expertusone_oauth/config.php';
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";

/*-- Check user has mange api admin permission --*/
if(user_access('Admin API Perm', $user) == false) {
 print "You are not Authorized to access this page.";
 exit();        
}

$util=new GlobalUtil();
$config=$util->getConfig();

// Added by Vincent for OAuth2 authentication on Nov 03, 2014
$fields_string = '';
$access_token='';
// $url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'] : "http://".$_SERVER['SERVER_NAME'];
$url = $GLOBALS['base_url'];	//for 56836: Site API Simulator not working
$data = array("grant_type"=>"client_credentials",
								'client_id'=>CLIENT_ID,
								'client_secret'=>CLIENT_SECRET
							);
										
foreach($data as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
	rtrim($fields_string, '&');

$peerVerify = $config['peer_verify'] == 0 ? FALSE : TRUE;
$context = stream_context_create(array(
		'http' => array(
				'method' => 'POST',
				'header' => 'Content-Type: application/x-www-form-urlencoded',
				'content' => $fields_string),
		'ssl' => array('verify_peer'=>$peerVerify, 
           'verify_peer_name'=>$peerVerify)
));

try{
	$response = file_get_contents($url.'/apis/oauth2/Token.php', false, $context);
	$response = json_decode($response,true);
	$access_token = $response['access_token'];
}catch(Exception $e){
	throw Exception($e->getMessage());
}

$image_path = '';

$image_path_folder = isset($config['logo_image_path']) ? $config['logo_image_path'] :'';
$image_path = $image_path_folder.'/logo.png';
$favicon_path = $image_path_folder.'/favicon.ico';
		

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
 <link type="text/css" rel="stylesheet" media="all" href="<?php  echo $GLOBALS['base_url'];?>/apis/docs/css/style.css">
<div id="header-outer">
	<div id="header">
		<a href="index.php" id="logo" title="Home"><img	id="logo_image" src="<?php print $image_path; ?>"alt="Home" /> </a>
	</div>
</div>
<title>Dynamic API Simulator</title>
<script src="jquery161.js"></script>
<script>
function loadParamForm(){
	
	var apin = $("#apilist").val();
	$.get("sim_engine.php", { actionkey: "getParamForm", apiname: apin },
			   function(data){
						$("#paramdata").html(data);
						$('#api-response').html('');
			   });
}

function loadAPI(){
	$.get("sim_engine.php", { actionkey: "getAPI" },
			   function(data){
			     $("#apilist").html(data);
			   });
}

function getData()
{
	// Added by Vincent for OAuth2 support on Nov 03, 2014
	
	var access_token = "<?php print $access_token;?>";
	if(access_token == null || access_token == undefined || access_token == ''){
		$("#api-response").html('OAuth Authentication Failed');
		return false;
	} 
	
	//var paramstring = decodeURIComponent($('form').serialize());
	var paramstring = $('form').serialize();
	//var paramstring = serializeForm(document.getElementById('simulator_form'));
	paramstring += "&access_token="+access_token;
	$("#api-response").html("Sending request..., please wait.");
	$.get('simulator_invoker.php?'+paramstring, function(data) 
	{ 
				//$('#api-response').html(formatXml1(data));
				//alert(formatXml(data));
				document.getElementById("api-response").innerHTML=formatXml($.trim(data));
				//$('#api-response').html(" "+data+" ");
	});
}

function formatXml(xml) {
	
    var formatted = '';
    var reg = /(>)(<)(\/*)/g;
    xml = xml.replace(reg, '$1\r\n$2$3');
    var pad = 0;
    
    $.each(xml.split('\r\n'), function(index, node) {
       // alert(node);
        var indent = 0;
        if (node.match( /.+<\/\w[^>]*>$/ )) {
            indent = 0;
        } else if (node.match( /^<\/\w/ )) {
            if (pad != 0) {
                pad -= 1;
            }
        } else if (node.match( /^<\w[^>]*[^\/]>.*$/ )) {
            indent = 1;
        } else {
            indent = 0;
        }

        var padding = '';
        for (var i = 0; i < pad; i++) {
            padding += '  ';
        }

        formatted += padding + node + '\r\n';
        pad += indent;
    });
    
    return formatted;
}
function RunTests(){
	$("#apilisttest").html("");
	$("#hidetest").html("");
	$.get("sim_engine.php", { actionkey: "testapiname" },
			   function(data){
			     var obj = jQuery.parseJSON(data);
			     console.log(obj);
			     jQuery.each(obj, function(dat) {
			    	 loadParamFormtest(dat);
			      });
			   });
}

function loadParamFormtest(apiname){
	
	var apin = apiname;
	$.get("sim_engine.php", { actionkey: "getParamForm", apiname: apin },
			   function(data){
						$("#paramdatatest").html(data);
						$("#paramdatatest").css("visibility","hidden");
						getDatatest();
			   });
}

function getDatatest()
{
	var paramstring = unescape($('form').serialize());
	$.get('simulator_invoker.php?'+paramstring, function(data) 
	{ 
		 console.log(paramstring);
		 var newparam = paramstring + "&xmlstring="+data;
		 $.get('test_engine.php?'+newparam, function(data1) {
		 	$("#apilisttest").append(data1+" - ");
		 });
	});
}
//  function used to encode url components  
function serializeForm(form) {
    var kvpairs = [];
    for ( var i = 0; i < form.elements.length; i++ ) {
        var e = form.elements[i];
        if(!e.name) continue; // Shortcut, may not be suitable for values = 0 (considered as false)
        switch (e.type) {
            case 'text':
                kvpairs.push(encodeURIComponent(e.name) + "=" + encodeURIComponent(e.value));
                break;
            default:
                kvpairs.push(e.name + "=" + e.value);
                break;    
        }
    }
    return kvpairs.join("&");
}
</script>
</head>
<body>
<div id ='page' style="width:60%;clear:both;margin:auto;">
<div id="main-content" >
<h1 class='headtitle' >Dynamic API Simulator</h1>
<!--<div><input type="button" onclick="RunTests();" value="Run Tests"> Test Result: <span id="apilisttest"></span></div>
--><div id="hidetest">
<fieldset class ='Fieldsetstyler'><legend><b>List of Available API's</b></legend>
<div id="apilist-div" >
<select	id="apilist" onchange="loadParamForm()">
</select></div>
</fieldset>
<div>
<fieldset class ='Fieldsetstyler'><legend><b>Request:</b></legend>
<div id="paramdata" >
Please Select an API.
</div>
</fieldset>
<fieldset class ='Fieldsetstyler'><legend><b>Response:</b></legend>
<div id="apiresponse-div" ><textarea id="api-response" rows=23 cols=75></textarea></div>
</fieldset>
</div>
</div>
</div>
</div>
</body>
<script>loadAPI();</script>
</html>
