<?php
class LDAPService
{
	private $LDAP_URL="expertuschn.com";
	private $LDAP_DOMAIN="expertuschn.com";
	private $LDAP_DN="DC=expertuschn,DC=com";
	private $LDAP_PORT="389";
	private $BIND_USERNAME="";
	private $DOMAIN="";
	private $BIND_PWD="";
	public function getData($username,$password)
	{
		 $results=db_query("select sid,name,address,port,basedn,binddn,bindpw from ldap_servers");
	     foreach($results as $row)
	     {
	     	$this->LDAP_URL=$row->address;
	     	$this->LDAP_DOMAIN=$row->sid;
	     	$this->LDAP_PORT=trim($row->port);
	     	$this->BIND_USERNAME=trim($row->binddn);
	     	$this->DOMAIN=trim($row->name);
	     	$this->BIND_PWD=ldap_servers_decrypt(trim($row->bindpw));
	     	//  CN=expprod ldap,DC=expertuschn,DC=com
	     	$tmp_arr=explode(",",$this->BIND_USERNAME);
	     	$tmp1=explode("=",$tmp_arr[0]);
	     	$extract_username=explode(" ",$tmp1[1]);
	     	
	     	expDebug::dPrint(" user name ...". $extract_username[0]);
	     	$this->BIND_USERNAME=$extract_username[0]."@".$this->DOMAIN;
	     	$tmp_basedn_arr=explode("\"",$row->basedn);
	     
	     	$basedn=$tmp_basedn_arr[1];
	     	$this->LDAP_DN=$basedn;
	     }
	     expDebug::dPrint("LDAP URL..".$this->LDAP_URL);
	     expDebug::dPrint("LDAP_DOMAIN..".$this->LDAP_DOMAIN);
	     expDebug::dPrint("LDAP_DN..".$this->LDAP_DN);
	     expDebug::dPrint("LDAP PORT..".$this->LDAP_PORT);
	     expDebug::dPrint("LDAP USERNAME..".$this->BIND_USERNAME);
	   //  expDebug::dPrint("LDAP BIND_PWD..".$this->BIND_PWD);
	   
		$ds = ldap_connect( $this->LDAP_URL,$this->LDAP_PORT );
		ldap_set_option($ds, LDAP_OPT_PROTOCOL_VERSION, 3);
		ldap_set_option($ds, LDAP_OPT_REFERRALS, 0);
		
		
		// now try a real login
		//$login = ldap_bind( $ds, "$username@$this->LDAP_DOMAIN", $password );
		$login = ldap_bind( $ds, $this->BIND_USERNAME, $this->BIND_PWD );
		try{
			
			$results=db_query("select id,lms_field_name,ismandatory,ldap_field_name from slt_ldap_field_mapping");
     		
     		$attributes=array();
     		$i=0;
			foreach($results as $row)
     		{
     			$attributes[$i]=$row->ldap_field_name;
     			$i++;
     		}
     		expDebug::dPrint("attributes...");
     		expDebug::dPrint($attributes);
		/*$attributes = array("displayname", "mail",
		"department",
		"title",
		"sn",
		"givenname","telephonenumber","extensionattribute1"); */
     		
		$filter = "(&(objectCategory=person)(sAMAccountName=$username))";
		
		$result = ldap_search($ds, $this->LDAP_DN, $filter, $attributes);
		//print_r($result);
		$entries = ldap_get_entries($ds, $result);
		$row=new stdClass();
		if($entries["count"] > 0)
		{
			foreach($attributes as $v)
			{
				expDebug::dPrint(" attributes..".$v);	
				$row->$v=$entries[0][$v][0];
			} 
			/*$row->displayname=$entries[0]['displayname'][0];
			$row->email=$entries[0]['mail'][0];
			$row->department=$entries[0]['department'][0];
			$row->title=$entries[0]['title'][0];
			$row->firstname=$entries[0]['givenname'][0];
			$row->lastname=$entries[0]['sn'][0];
			$row->telephoneno=$entries[0]['telephonenumber'][0];
			$row->timezone=$entries[0]['extensionattribute1'][0]; */
		}
		}catch(Exception $e)
		{
			ldap_unbind($ds);
			return;
		}
		ldap_unbind($ds);
		expDebug::dPrint("LDAP Servicd..");
		expDebug::dPrint($row);
		return $row;
	}
}
  

  ?>
