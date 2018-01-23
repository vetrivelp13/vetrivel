<?php
// Commented by Vincent on Aug 14, 2012 for issue #0015901: Dynamic APIs create too many PHP sessions and bring down the server
// session_start(); 


/*
 * Simple 'user management'
 */
define ('USERNAME', 'sysadmin');
define ('PASSWORD', 'sysadmin');


/*
 * Always announce XRDS OAuth discovery
 */

header('X-XRDS-Location: http://' . $_SERVER['SERVER_NAME'] . '/apis/core/services.xrds');



function assert_logged_in()
{
	
	if (empty($_SESSION['authorized']))
	{
		$uri = rawurldecode($_SERVER['REQUEST_URI']);
		//echo $uri;exit();
		
		//header('Location: /apis/core/login/login_view?goto=' . urlencode($uri));
		include_once "BaseAPI.php";
		$sitename=BaseAPI::getURLParamValue($uri,"sitename");
		$redirect_uri=BaseAPI::getURLParamValue($uri,"redirect_uri");
		$pos = strpos($uri, "&sitename=");
		$goto=substr($uri,0,$pos);
		
		
		header('Location: /?source=external&goto=' . urlencode($goto).'&sitename='.$sitename."&redirect_uri=".$redirect_uri);
		exit();
	}
	

}

function assert_request_vars()
{
	foreach(func_get_args() as $a)
	{
		if (!isset($_REQUEST[$a]))
		{
			header('HTTP/1.1 400 Bad Request');
			echo 'Bad request.';
			exit;
		}
	}
}

function assert_request_vars_all()
{
	foreach($_REQUEST as $row)
	{
		foreach(func_get_args() as $a)
		{
			if (!isset($row[$a]))
			{
				header('HTTP/1.1 400 Bad Request');
				echo 'Bad request.';
				exit;
			}
		}
	}
}

?>