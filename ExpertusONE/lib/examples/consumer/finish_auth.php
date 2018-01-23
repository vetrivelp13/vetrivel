<?php

require_once "common.php";
session_start();

function escape($thing) {
    return htmlentities($thing);
}

function run() {
    $consumer = getConsumer();

    // Complete the authentication process using the server's
    // response.
    $return_to = getReturnTo();
    $response = $consumer->complete($return_to);

    // Check the response status.
    if ($response->status == Auth_OpenID_CANCEL) {
        // This means the authentication was cancelled.
        $msg = 'Verification cancelled.';
        displayError($msg);
    } else if ($response->status == Auth_OpenID_FAILURE) {
        // Authentication failed; display the error message.
        $msg = "OpenID authentication failed: " . $response->message;
        displayError($msg);
    } else if ($response->status == Auth_OpenID_SUCCESS) {
        // This means the authentication succeeded; extract the
        // identity URL and Simple Registration data (if it was
        // returned).
        $openid = $response->getDisplayIdentifier();
        $esc_identity = escape($openid);

        $success = sprintf('You have successfully verified ' .
                           '<a href="%s">%s</a> as your identity.',
                           $esc_identity, $esc_identity);
        include_once $_SERVER["DOCUMENT_ROOT"]."/apis/core/login/login_dao.php";
                           
		$login_dao=new LoginDAO();
        $result=$login_dao->getUserIdbyIdentity($openid);
        $len = count($result);
			 //print_r($result);
		$userid="";
		$uname="";
		$pass="";
		for($i=0;$i<$len;$i++)
		{
			$row=$result[$i];
			
			if($_REQUEST["source"]=="external")
			{
				expDebug::dPrint("user id::".$row["uid"]);
				if($row["uid"]!="")
				{
					$result1=$login_dao->verifyUser($row["uid"]);
					$len1 = count($result1);
					for($j=0;$j<$len1;$j++)
					{
						$row1=$result1[$i];
						$userid=$row1["USERID"];
					}
				}
			} 
			$uname=$row["name"];
			$pass=$row["pass"];
		}
	/*	if($_REQUEST["source"]=="external")
		{
			header('Location: https://v7299.exphosted.com/apis/core/login/login_disclaimer_view.php?goto=/apis/core/oauth/authorize?oauth_token='.$_REQUEST["oauth_token"]."&userid=".$userid); //.'&oauth_token='.$_POST["oauth_token"]."&userid=".$row["USERID"]);
			
		}
		else */
		{
			$goto="/apis/core/oauth/authorize?oauth_token=".$_REQUEST["oauth_token"];
			echo "<html><body onload=calltolmsfunc()>";
			$pass="welcome";//testing purpose
			echo "<h2>Redirecting you to LMS Server, please wait...</h2>";
			echo "<form name='calltolms' action='https://v7299.exphosted.com' method='post'>";
			echo "<input type='hidden' name='username' value='".$uname."'></input>";
			echo "<input type='hidden' name='password' value='".$pass."'></input>";
			echo "<input type='hidden' name='goto' value='".$goto."'></input>";
			echo "<input type='hidden' name='authtype' value='openid'></input>";
			echo "<input type='hidden' name='source' value='".$_REQUEST["source"]."'></input>";
			echo "</form></body>";
			echo "<script> ";
			echo "function calltolmsfunc()";
			echo "{";
			echo "document.calltolms.submit();";
			echo "}";
			echo "</script>";
			echo "</html>";
			
		}
/*		header("username:johnp");
		header("password:welcome");
		header("authtype:openid");
		header("Location: https://v7299.exphosted.com?username=johnp&password=welcome&authtype=openid");
	*/	
		
        // header('Location: https://v7299.exphosted.com/apis/core/login/login_disclaimer_view.php?goto=/apis/core/oauth/authorize?oauth_token='.$_REQUEST["oauth_token"]."&userid=".$userid); //.'&oauth_token='.$_POST["oauth_token"]."&userid=".$row["USERID"]);                  

        if ($response->endpoint->canonicalID) {
            $escaped_canonicalID = escape($response->endpoint->canonicalID);
            $success .= '  (XRI CanonicalID: '.$escaped_canonicalID.') ';
        }

        $sreg_resp = Auth_OpenID_SRegResponse::fromSuccessResponse($response);

        $sreg = $sreg_resp->contents();

        if (@$sreg['email']) {
            $success .= "  You also returned '".escape($sreg['email']).
                "' as your email.";
        }

        if (@$sreg['nickname']) {
            $success .= "  Your nickname is '".escape($sreg['nickname']).
                "'.";
        }

        if (@$sreg['fullname']) {
            $success .= "  Your fullname is '".escape($sreg['fullname']).
                "'.";
        }

	$pape_resp = Auth_OpenID_PAPE_Response::fromSuccessResponse($response);

/*	if ($pape_resp) {
            if ($pape_resp->auth_policies) {
                $success .= "<p>The following PAPE policies affected the authentication:</p><ul>";

                foreach ($pape_resp->auth_policies as $uri) {
                    $escaped_uri = escape($uri);
                    $success .= "<li><tt>$escaped_uri</tt></li>";
                }

                $success .= "</ul>";
            } else {
                $success .= "<p>No PAPE policies affected the authentication.</p>";
            }

            if ($pape_resp->auth_age) {
                $age = escape($pape_resp->auth_age);
                $success .= "<p>The authentication age returned by the " .
                    "server is: <tt>".$age."</tt></p>";
            }

            if ($pape_resp->nist_auth_level) {
                $auth_level = escape($pape_resp->nist_auth_level);
                $success .= "<p>The NIST auth level returned by the " .
                    "server is: <tt>".$auth_level."</tt></p>";
            }

	} else {
            $success .= "<p>No PAPE response was sent by the provider.</p>";
	} */
    }

//    include 'index.php';
}

run();

?>