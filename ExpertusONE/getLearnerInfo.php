<?php 
//include "./sites/all/services/Trace.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Encryption.php";

class GetLearnerInfo{
	
	private $cook;
	private $cookVal;
	// sample cookVal: 203##guest##Guest User User##notifytest_balajir@notifytest_expertus.com
	function __construct(){
			$this->cook = isset($_COOKIE['SPLearnerInfo'])?$_COOKIE['SPLearnerInfo']:'';
			if($this->cook!=''){
				$dcre = new Encrypt();
				$this->cookVal = $dcre->Decrypt($this->cook);
			}else{
				$this->cookVal='';
			}
	}
	
	public function getValue($prm){
		if(isset($_SESSION['logged_user_id']) && !empty($_SESSION['logged_user_id']) && $prm=='userid')
			return $_SESSION['logged_user_id'];
		if ($this->cookVal != '') {
			switch (strtolower($prm)) {
				case 'userid': return $this->getUserId(); break;
				case 'username': return $this->getUserName(); break;
				case 'userfullname': return $this->getUserFullName(); break;
				case 'userrole': return $this->getUserRole(); break;
				case 'usermail': return $this->getUserMail(); break;
				case 'userfirstname': return $this->getUserFirstName(); break;
				case 'userlastname': return $this->getUserLastName(); break;
				case 'usersavedfullname': return $this->getUserSavedFullName(); break;
				default: return ''; break;
		  }
		}
		else{
		    if(strtolower($prm) == "userid") {
			  return 0;
			}else{
			  return '';
			}
		}
		
	}
	
	public function setValue($prm){
		$id;
		$name;
		$fullname;
		$role;
		$email;
		$firstname;
		$lastname;
		$savedfullname;
		if ($this->cookVal != '') {
			list($id, $name, $fullname, $role, $email, $firstname, $lastname, $savedfullname) = explode("##", $this->cookVal);
			foreach ($prm as $key => $val) {
	      switch (strtolower($key)) {
	        case 'userid': $id = $val; break;
	        case 'username': $name = $val; break;
	        case 'userfullname': $fullname = $val; break;
	        case 'userrole': $role = $val; break;
	        case 'usermail': $email = $val; break;
	        case 'userfirstname': $firstname = $val; break;
	        case 'userlastname': $lastname = $val; break;
	        case 'usersavedfullname': $savedfullname = $val; break;
	      }
			}
			$tmp =        $id .
			       "##" . $name .
			       "##" . $fullname .
			       "##" . $role .
			       "##" . $email .
			       "##" . $firstname .
			       "##" . $lastname .
			       "##" . $savedfullname;
			//expDebug::dPrint("cookie new value :".$tmp);
			$dcre = new Encrypt();
			$tval = $dcre->encrypt($tmp);
			setcookie("SPLearnerInfo",$tval,null,"/","",(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
			return $tval;
		}else{
			return '';
		}
		
	}
	
	private function getUserId(){
		$val = explode("##", $this->cookVal);
		//0041315: Forgot Password is not functioning,DB Query is displayed
		if(!empty($val[0]))
		  return $val[0];
		else
		  return 0;
	} 
	
	private function getUserName(){
		$val = explode("##", $this->cookVal);
		return $val[1];
	}

	private function getUserFullName(){
		$val = explode("##", $this->cookVal);
		return $val[2];
	}
	
	private function getUserRole(){
		$val = explode("##", $this->cookVal);
		return $val[3];
	}
	
	private function getUserMail(){
		$val = explode("##", $this->cookVal);
		return $val[4];
	}
	
    private function getUserFirstName(){
    	$val = explode("##", $this->cookVal);
    	return $val[5];
    }
  
    private function getUserLastName(){
   		$val = explode("##", $this->cookVal);
    	return $val[6];
    }
  
    private function getUserSavedFullName(){
    	$val = explode("##",$this->cookVal);
    	return $val[7];
    }
    
}

$param = isset($_GET['getvalue'])?$_GET['getvalue']:'';
$detail = new GetLearnerInfo();
if($param!=''){
	echo header('Content-Type:text/plain; charset=utf-8');
	echo header("Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");
	echo header("Pragma: no-cache");
	echo $detail->getValue($param);
}
?>