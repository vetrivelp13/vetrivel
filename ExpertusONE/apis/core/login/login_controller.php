<?php
/**
    * It is an controller for login module. It is getting called when user logged in through OpenID or OAuth.
    * @author Sureshkumar.v
*/

include_once "login_dao.php";
include_once "../BaseAPI.php";


class LoginController
{
	/**
	 *It is used to store the client who is calling this api either Mobile client or Web Browser.
	 *@access private
	 *@var string
	 *
	 */
	private $useragent="";
	
		
	/*
    * It is used to collect all the $_POST parameter  values into params object.
    * @param none 
    * @return stdClass
    */
	public function getParams()
	{
		$gotopage=$_POST["goto"];
		$username=$_POST["username"];
		$password=$_POST["password"];
		$oauth_token=$_POST["oauth_token"];
		$access_str=$_GET["access"];
		
		
		$params=new stdClass();
		$params->gotopage=$gotopage;
		$params->username=$username;
		$params->password=$password;
		$params->oauth_token=$oauth_token;
		$params->access_str=$access_str;

		$ua=BaseAPI::getBrowser();
		$this->useragent= $ua['name'].'/' . $ua['version'];
		return $params;			
	}
	
	/*
    * This method redirects requests into appropriate classes.
    * @param none 
    * @return none
    */
	public function doExecute()
	{
		// now try it
		
		 $paramobj=$this->getParams();
		 if ($_POST['openid_action'] == "login")
		 { 
		 	// Get identity from user and redirect browser to OpenID Server
			$openid = new SimpleOpenID;
			$openid->SetIdentity($_POST['openid_url']);
			$openid->SetTrustRoot('http://' . $_SERVER["HTTP_HOST"]);
			$openid->SetRequiredFields(array('email','fullname'));
			$openid->SetOptionalFields(array('dob','gender','postcode','country','language','timezone'));
			
			if ($openid->GetOpenIDServer()){
				//$openid->SetApprovedURL('http://' . $_SERVER["HTTP_HOST"] . $_SERVER["PATH_INFO"]);  	// Send Response from OpenID server to this script
				$openid->SetApprovedURL('https://v7299.exphosted.com/apis/core/login/login_controller.php?goto='.$_POST["goto"]);  	// Send Response from OpenID server to this script
				$openid->Redirect(); 	// This will redirect user to OpenID Server
			}else{
				$error = $openid->GetError();
				header('Location: /apis/core/login/login_view.php?goto='.$_POST["goto"].'&errorcode='.$error['code']."&error_description=".$error['description'] );
				
				//echo "ERROR CODE: " . $error['code'] . "<br>";
				//echo "ERROR DESCRIPTION: " . $error['description'] . "<br>";
			}
			exit;
		}
		else if($_GET['openid_mode'] == 'id_res'){ 	// Perform HTTP Request to OpenID server to validate key
			$openid = new SimpleOpenID;
			$goto=$_GET["goto"];
			$openid->SetIdentity($_GET['openid_identity']);
			$openid_validation_result = $openid->ValidateWithServer();
			if ($openid_validation_result == true){ 		// OK HERE KEY IS VALID
				
				$con = mysql_connect("localhost","expuser","expuser");
				mysql_select_db("expertus", $con);
				
				
				$result = mysql_query("SELECT uid FROM authmap where authname='".$_GET['openid_identity']."'");
				echo "SELECT uid FROM authmap where authname='".$_GET['openid_identity']."'";
				$uid="";
				while($row = mysql_fetch_array($result))
			  	{
			  		$uid=$row["uid"];	
			  	}
			  	mysql_close($con);
			  	
			  	$con = mysql_connect("localhost","expuser","expuser");
				mysql_select_db("expertus", $con);
				
				$result = mysql_query("SELECT * FROM users where uid='".$uid."' and status=1");
				while($row = mysql_fetch_array($result))
			  	{
			  			$oauth_token=$_GET["oauth_token"];
			  			header('Location: login_disclaimer_view.php?goto='.$goto.'&oauth_token='.$oauth_token);
			  	}
			  	mysql_close($con);
			  	
			  	
			  	
			  	
			  	
				echo "VALID";
			}else if($openid->IsError() == true){			// ON THE WAY, WE GOT SOME ERROR
				$error = $openid->GetError();
				//echo "ERROR CODE: " . $error['code'] . "<br>";
				//echo "ERROR DESCRIPTION: " . $error['description'] . "<br>";
				header('Location: /apis/core/login/login_view.php?goto='.$_GET["goto"].'&errorcode='.$error['code']."&error_description=".$error['description'] );
			}else{	
														// Signature Verification Failed
				header('Location: /apis/core/login/login_view.php?goto='.$_GET["goto"].'&errorcode='.'L_003');
				//echo "INVALID AUTHORIZATION";
			}
		}else if ($_GET['openid_mode'] == 'cancel'){ // User Canceled your Request
			header('Location: /apis/core/login/login_view.php?goto='.$_GET["goto"].'&errorcode='.'L_002');	
			//echo "USER CANCELED REQUEST";
		}
		 else if($_GET["access"]=="granted")
		 {
			 	$goto=$_GET["goto"];
			  	$oauth_token=$_GET["oauth_token"];
			  	session_start();
			  	$_SESSION['authorized'] = true;
			  	$_SESSION["userid"]=$_GET["userid"];
				
				//header('Location: ' .$goto."?oauth_token=".$oauth_token);
				header('Location: ' .$goto);
				//header('Location: ' . "http://www.externalsite.com/oauth/callback.php?oauth_token=".$oauth_token);//$_REQUEST['goto']);
		 }
		 else
		 {
			 $dao=new LoginDAO();
			 $result=$dao->doLogin($paramobj);
			 $len = count($result);
			 //print_r($result);
			 for($i=0;$i<$len;$i++)
			 {
			 		$row=$result[$i];
		  			header('Location: login_disclaimer_view.php?goto='.$_POST["goto"].'&oauth_token='.$_POST["oauth_token"]."&userid=".$row["USERID"]);	
			 }
			 if($len <=0)
			 {
					header('Location: /apis/core/login/login_view.php?goto='.$_POST["goto"].'&errorcode='.'L_001');	
			
			 } 
		 }
		 	  	
		 
	}
}
$obj=new LoginController();
$obj->doExecute();
