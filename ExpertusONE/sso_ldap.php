<?php 

//include_once($_SERVER['DOCUMENT_ROOT']. '/expertusone_auth.php');

error_reporting(E_ERROR | E_PARSE);

$identity="";
 
$obj=new LDAP_Service();
$stdObj=$obj->login($_REQUEST["username"],$_REQUEST["pwd"]);
if($stdObj->authenticated=="true")
{
	$sFullName = $stdObj->displayname;//"Firstname Lastname"; 
	$sEmail = $stdObj->email; "youremail@yourdomain.com"; 
	$sExternalID = $_REQUEST["username"];  //user id
	$sOrganization = ""; 
 
	 /* Insert the Autentication Token here */
	$sToken = "52713bf24cc564c72asd249feb7de52de341195c"; 
 
	 /* Build the message */
	$sTimestamp = isset($_GET['timestamp']) ? $_GET['timestamp'] : time(); 
	$sMessage = $sFullName.$sEmail.$sExternalID.$sOrganization.$sToken.$sTimestamp; 
	$sHash = MD5($sMessage);  // use the hash in ExpertusONE link

	$details=base64_encode("fullname=".$sFullName."&email=".$sEmail."&externalid=".$sExternalID."&organization=".$sOrganization."&timestamp=".$sTimestamp);	
	$identity=$sHash;
	echo "Login success. Please visit the applications";
}
else
{
	echo "Login failure";exit();
}


$s = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
$protocol = strleft(strtolower($_SERVER["SERVER_PROTOCOL"]), "/").$s; 
$port = ($_SERVER["SERVER_PORT"] == "80") ? "" : (":".$_SERVER["SERVER_PORT"]);
$currentDomain=$protocol."://".$_SERVER['SERVER_NAME'].$port;
			
?>
<?php if($stdObj->authenticated=="true") {?>
	<a href="<?php echo $currentDomain;?>?q=user/login/sso/<?php echo $identity;?>/<?php echo $details;?>">ExpertusONE</a>
<?php }?>

<?php
class LDAP_Service
{
	private $LDAP_URL="115.111.237.118";//expertuschn.com";
	private $LDAP_DOMAIN="expertuschn.com";
	private $LDAP_DN="DC=expertuschn,DC=com";
	private $LDAP_PORT="8000";// "389";
	public function login($username,$password)
	{
		
		$ds = ldap_connect( $this->LDAP_URL,$this->LDAP_PORT );//389
		ldap_set_option($ds, LDAP_OPT_PROTOCOL_VERSION, 3);
		ldap_set_option($ds, LDAP_OPT_REFERRALS, 0);
		
		//$username = "sureshkumarv";
		//must always check that password length > 0
		//$password = "welcome6@";
		
		// now try a real login
		//$login = ldap_bind( $ds, "lmsldaptest3@expertuschn.com", "suresh1@" );
		//print_r($login);
		$login = ldap_bind( $ds, "$username@$this->LDAP_DOMAIN", $password );
		//echo '- Logged In Successfully<br/><br/>';
		try{
		$attributes = array("displayname", "mail",
		"department",
		"title",
		"sn","extensionattribute1",
		"givenname");
		$filter = "(&(objectCategory=person)(sAMAccountName=$username))";
		
		$result = ldap_search($ds, $this->LDAP_DN, $filter, $attributes);
		//print_r($result);
		$entries = ldap_get_entries($ds, $result);
		$row=new stdClass();
		if($entries["count"] > 0)
		{
			$row->displayname=$entries[0]['displayname'][0];
			$row->email=$entries[0]['mail'][0];
			$row->department=$entries[0]['department'][0];
			$row->title=$entries[0]['title'][0];
			$row->firstname=$entries[0]['givenname'][0];
			$row->lastName=$entries[0]['sn'][0];
			$row->timezone=$entries[0]['extensionattribute1'][0];
			$row->authenticated="true";
		}
		}catch(Exception $e)
		{
			ldap_unbind($ds);
			return;
		}
		ldap_unbind($ds);
		return $row;
	}
}
 



function Encrypt($string, $key)
{
	$result = '';
	for($i=1; $i<=strlen($string); $i++)
	{
		$char = substr($string, $i-1, 1);
		$keychar = substr($key, ($i % strlen($key))-1, 1);
		$char = chr(ord($char)+ord($keychar));
		$result.=$char;
	}
	return $result;
}

function Decrypt($string, $key)
{
	$result = '';
	for($i=1; $i<=strlen($string); $i++)
	{
		$char = substr($string, $i-1, 1);
		$keychar = substr($key, ($i % strlen($key))-1, 1);
		$char = chr(ord($char)-ord($keychar));
		$result.=$char;
	}
	return $result;
}


//echo $identity;


/*
 * Custom implementation for redirecting the account related pages to https
 * Ticket No: #6231 by Suresh
 */
function strleft($s1, $s2) 
{ 
	return substr($s1, 0, strpos($s1, $s2)); 
}
/*
 * End
 */
?>


