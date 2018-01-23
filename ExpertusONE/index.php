<?php

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

/**
 * Root directory of Drupal installation.
 */
global $tTrack;

$tTrack['start']['index_start'] = microtime(true);
$tTrack['end']['Request URI'] = $_SERVER['REQUEST_URI'];
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
try {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	//enable_disable_custom_https_pages();
	menu_execute_active_handler();
	$tTrack['end']['index_end'] = (microtime(true) - $tTrack['start']['index_start']);
	expDebug::dPrint("Time Tracker -- ".print_r($tTrack['end'],true), 4);
} catch(Exception $ex) {
// 	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/Trace.php";
	// #59489 - catch exception error while bootup & write it on PHP's system logger.
	$errorMessage =  'Message: ' . $ex->getMessage() . ' | Code: ' . $ex->getCode() . ' | File : ' . $ex->getFile() . ' | Line : ' . $ex->getLine() . ' | Trace : '  . $ex->getTraceAsString();
// 	expDebug::dPrint('Redirect to support.php due to '. $errorMessage);
	error_log($errorMessage, 0);
	header("Location:/support.php");
}


/*
 * Custom implementation for redirecting the account related pages to https
 * Ticket No: #6231 by Suresh
 */
function enable_disable_custom_https_pages()
{
	sso_redirection_execution();
	$config=getConfig('exp_sp');

 	if(isset($config["custom_https_pages"]) && trim(strtolower($config["custom_https_pages"]))== 1)
	{
		$s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : ""; 
		$protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s; 

		$uri_path=$_SERVER["REQUEST_URI"];
		//if(!strpos($uri_path,"system/ajax")>0)
		{
			if(secure_needed($protocol))
			{
				expDebug::dPrint("inside if secure", 5);	
				if($protocol=="http" )
				{
					expDebug::dPrint("inside http", 5);	
					$selfUrlStr="https".getSelfURLWithoutProtocol(true);
					header("location:".$selfUrlStr);
				}
			}
			else
			{
				expDebug::dPrint("inside if secure else", 5);	
				if($protocol=="https" )
				{
					expDebug::dPrint("inside https", 5);
					$selfUrlStr="http".getSelfURLWithoutProtocol(false);
					header("location:".$selfUrlStr);
				}
			}
		}
	}

}

function sso_redirection_execution()
{
	global $user;
	if($user->uid!="" && $user->uid!=0)
	{
		$uri_path=$_SERVER["REQUEST_URI"];
		expDebug::dPrint("uri path::".$uri_path, 5);
		if(strpos($uri_path,"user/login/sso")>0 )
		{
			$s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
			$protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s; 
			$port = ($_SERVER["SERVER_PORT"] == "80") ? "" : (":".$_SERVER["SERVER_PORT"]);
			$currentDomain=$protocol."://".$_SERVER['SERVER_NAME'].$port;
			header("Location:".$currentDomain);
			exit();
		}   
		
	}
	else
	{
		//SSO Integration
		$uri_path=$_SERVER["REQUEST_URI"];
		if($uri_path=="/" || strpos($uri_path,"sso_err=")>0 )
		{
			$sso_enabled=false;
			$ip_ranges="";
			$sso_server_login_url="";
			if(module_exists('ldap_authentication'))
			{
				$auth_conf = ldap_authentication_get_valid_conf();
				$sso_enabled = $auth_conf->ssoEnabled;
				$ip_ranges=$auth_conf->ip_ranges;
				$sso_server_login_url=$auth_conf->sso_login_url;
			}
			if($sso_enabled==true)
			{
				$redirectionNeeded=false;
				if(trim($ip_ranges)=="")
					$redirectionNeeded=true; //redirect all request to sso portal
				if($redirectionNeeded==false)
				{
					$incoming_ip=getClientIPAddress();//$_SERVER["REMOTE_ADDR"];
					expDebug::dPrint("Calling IP:".$incoming_ip, 5);
					$ip_ranges_arr=explode(" ",$ip_ranges);
					foreach ($ip_ranges_arr as $ip_range) 
					{
	 					 if($ip_range==$incoming_ip)
	 					 {
	 					 		$redirectionNeeded=true;
	 					 		break;
	 					 } 
	 					 else
	 					 {
	 					 		$incoming_ip_arr=explode(".",$incoming_ip);
	 					 		$chk_ip_range=explode(".",$ip_range);
	 					 		
	 					 		if( ($incoming_ip_arr[0]==$chk_ip_range[0] ||  $chk_ip_range[0]=='*') && 
	 					 			($incoming_ip_arr[1]==$chk_ip_range[1] ||  $chk_ip_range[1]=='*')  &&
	 					 			($incoming_ip_arr[2]==$chk_ip_range[2] ||  $chk_ip_range[2]=='*')  &&
	 					 			($incoming_ip_arr[3]==$chk_ip_range[3] ||  $chk_ip_range[3]=='*') )
	 					 			{
	 					 				$redirectionNeeded=true;
	 					 				break;
	 					 			}
	 					 }
					}
				}
				
				$config=getConfig('exp_sp');
				
				if($redirectionNeeded)
				{
					if(strpos($uri_path,"sso_err=")>0 )
					{
						header("Location:".$sso_server_login_url."?timestamp=". time()."&".substr($uri_path,2));
						exit();
					}
					else
					{	
						header("Location:".$sso_server_login_url."?timestamp=". time());
						exit();
					}
				}
			}
		}	
	}
	
}
/*
 * Custom implementation for redirecting the account related pages to https
 * Ticket No: #6231 by Suresh
 */
 
function secure_needed($protocol)
{
	$context="";
	if(arg(0)=="portalpages")
	{
		$context="admin";
	}
	$is_secure=false;
	$uri_path="";
	
	
	
	global $user;
	if($user->uid=="" || $user->uid==0)
	{
		$uri_path=" home_page_before_login";
	}
	else
	{
		$uri_path=$_SERVER["REQUEST_URI"];
		
	}
	
	expDebug::dPrint("protocol.".$protocol."===userid ".$user->uid." and uri_path=".$uri_path."###", 5);	
        
	if($protocol=="https")
       {
               if(strpos($uri_path,"system/ajax")>0 )        
               {
                       $is_secure=true;
                       return $is_secure;
               }                
       }
       
	if($protocol=="http")
       {
               if(strpos($uri_path,"ajax/cart")>0 )        
               {
                       $is_secure=false;
                       return $is_secure;
               }                
       }

	$config=getConfig('exp_sp');
	
	if(isset($config["https_protected_url"]))
	{
		$https_protected_url_arr=explode(",",$config["https_protected_url"]);
		for($i=0;$i<count($https_protected_url_arr);$i++)
		{
			expDebug::dPrint("str poss....".strpos($uri_path,trim($https_protected_url_arr[$i])), 5);
			
			if(strpos($uri_path,trim($https_protected_url_arr[$i]))>0)
			{
				
				$is_secure=true;
				break;
			}
		}
	}
	
	return $is_secure;
	
}



/*
 * Custom implementation for redirecting the account related pages to https
 * Ticket No: #6231 by Suresh
 */
function getSelfURLWithoutProtocol($securetype) 
{ 
	$s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : ""; 
	$protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s; 
	$config=getConfig('exp_sp');

  	$port="";
	if($securetype==true)
	{
		$port=":".$config["https_port"];
		}
	else
	{
		if($config["http_port"]=="80")
			$port="";
		else
			$port=$config["http_port"];
	}
	//$port = ($_SERVER["SERVER_PORT"] == "80") ? "" : (":".$_SERVER["SERVER_PORT"]); 
	return "://".$_SERVER['SERVER_NAME'].$port.$_SERVER['REQUEST_URI']; 
} 

function getClientIPAddress() 
{
	$ip="";
	if (getenv("HTTP_CLIENT_IP"))
	$ip = getenv("HTTP_CLIENT_IP");
	else if(getenv("HTTP_X_FORWARDED_FOR"))
	$ip = getenv("HTTP_X_FORWARDED_FOR");
	else if(getenv("REMOTE_ADDR"))
	$ip = getenv("REMOTE_ADDR");
	else
	$ip = "UNKNOWN";
	return $ip;
} 




/*
if (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') {
  global $base_url;
  $base_url = str_replace('http://', 'https://', $base_url);
}
 */