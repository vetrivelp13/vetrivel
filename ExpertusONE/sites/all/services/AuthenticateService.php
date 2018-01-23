<?php

/**
 * @class or PHP Name		: AuthenticateService
 * @author(s)				: Vincent.S
 * Version         			: 1.0
 * Date						: 09/07/2009
 * PHP Version          		: 5.2.6
 * Description     			: Authentication Web Service is used to authenticate the user in SmartPortal. If user name and password are valid this web
 * service will generate a encrypted token which should be given as input to all SmartPortal web services.
 */

include "../dao/AuthenticateDAO.php";
include_once "ServiceBase.php";
include_once "Encryption.php";
include_once "GlobalUtil.php";

class AuthenticateService extends ServiceBase{

  private $adminKey;
  private $learnerKey;

  public function __construct(){

  }

  public function authenticate($requestData){
    try{
      expDebug::dPrint("authenticate");
      $inObj= new stdClass();
      $enc = new Encrypt();
      $dao = new AuthenticateDAO();
      $inObj->username=$requestData->Username;
      //$deVal = $enc->encrypt($requestData->Password);
      $deVal = md5($requestData->Password);
      $inObj->password=$deVal;
      $inObj->isGlobal=$requestData->isGlobal;
      expDebug::dPrint("DEBUGGING AUTHENTICATE service " . $inObj->username);	
      $results=$dao->fetch($inObj);
      expDebug::dPrint('$results = ' . print_r($results, true), 4);
/*      if ($results->position == null || $results->position == 'null' || $results->position == ''){
        $results->position = $dao->getProfileIdFields();
      }*/
      if(strtolower($results->username)==strtolower($requestData->Username)){
        $sitename = "Portal";
        $time = date("Y-m-d h:i:s");
        $timems = strtotime($time);
        if(($requestData->ValidityInMin!=null || $requestData->ValidityInMin!='') && ($requestData->ValidityInMin > 0) )
        $valid=$requestData->ValidityInMin;
        else
        $valid=120;
        $loginTime = $timems+($valid*60);
        $token = $results->username."^#".$sitename."^#".$requestData->Locale."^#".$loginTime;
        expDebug::dPrint("Token : ".$token);
        $enc1 = new Encrypt();
        $deVal = $enc1->encrypt($token);
        expDebug::dPrint("Token Encrypt : ".$deVal);
        expDebug::dPrint("check https:".$_SERVER["HTTPS"]);
        setcookie("SPCertificate", $deVal, null, "/", "",
                         (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),
                         (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));

        $learnerInfo =        $results->id .
                       "##" . $results->username .
                       "##" . $results->fullname .
                       "##" . $results->position .
                       "##" . $results->email .
                       "##" . $results->firstname .
                       "##" . $results->lastname .
                       "##" . $results->savedfullname;
        expDebug::dPrint("LEARNER INFO --------".$learnerInfo);
				$enc11 = new Encrypt();
        $deVal1 = $enc11->encrypt($learnerInfo);
        setcookie("SPLearnerInfo", $deVal1, null, "/", "",
                       (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"] == 'on'? TRUE : ""),
                       (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"] == 'on'? TRUE : ""));

        //Arthi - added to put the admin and learner license keys to cookies - start
        $this->setLicenseKeys();
        //Arthi - added to put the admin and learner license keys to cookies - end

        //return $deVal;
        //Arthi - changed to cater to response complex type - start
        /*$outData = '<Certificate Id="'.$results->id.'" UserName="'.$results->username.' Name='.$results->fullname.'">'.$deVal.'</Certificate>';
         $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);*/
         
        //$certArray=array("_" => $deVal, "Id"=>$results->id,"UserName"=>$results->username,"Name"=>$results->fullname);
        //$outData=array("Certificate" =>$certArray);

        $cert=Array();
        $cert[0]=new StdClass();
        $cert[0]->SPCertificate=$deVal;
        $cert[0]->SPLearnerLicense=$this->learnerKey;
        $cert[0]->SPAdminLicense=$this->adminKey;
        $cert[0]->SPLearnerInfo=$deVal1;
        $cert[0]->Name=$results->fullname;
        $cert[0]->UserName=$results->username;
        $cert[0]->UserId=$results->id;

        $outData=$cert;
        //Arthi - changed to cater to response complex type - end
        return $outData;
      }else{
        expDebug::dPrint("Authentication failed. Your username and password is wrong.");
        throw new SoapFault("ExpPortal","Authentication failed. Your username and password is wrong.");
      }
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  //Arthi - added to put the admin and learner license keys to cookies - start
  private function setLicenseKeys(){
    $util=new GlobalUtil();
    $conf=$util->getConfig();
    $admin_license=$conf["admin_licensekey"];
    $learner_license=$conf["learner_licensekey"];
    $this->adminKey=$admin_license;
    $this->learnerKey=$learner_license;
    setcookie("SPAdminLicense",$admin_license,null,"/","",(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
    setcookie("SPLearnerLicense",$learner_license,null,"/","",(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""),(isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"]=='on'?TRUE:""));
  }
  //Arthi - added to put the admin and learner license keys to cookies - end

  public function validateUser($reqData){
    $isvalid="false";
    $enc = new Encrypt();
    $validateMsg='';
    try{
      $name=$reqData->name;
      $pass=$reqData->pass;
      $isValidUser;
      expDebug::dPrint("received ".$pass);
      $dao = new AuthenticateDAO();
      $results = $dao->getUserPwd($reqData);
      if($results && sizeOf($results) == 1){
        $decrypt=$enc->decrypt($results[0]->Password);
        expDebug::dPrint("decrypt ".$decrypt);
        expDebug::dPrint($pass==$decrypt);
        expDebug::dPrint(" Status : ".$results[0]->Status);
        expDebug::dPrint(" AccountLock : ".$results[0]->AccountLock);
        $AccountLock = split('#',$results[0]->AccountLock);
        $isValidUser="true";
        /*if(($results[0]->Status == 'Active') and (($AccountLock[1]<4) or ($results[0]->AccountLock == '')) and ($pass==$decrypt)){
         $isvalid="true";
         $results = $dao->updateUserAttempt($reqData,'ResetAttempt');
         expDebug::dPrint("User Attempt Reset :".$results[0]->Message);
         $validateMsg = $results[0]->Message;
         */
        if($pass==$decrypt){
          $isvalid="true";
          $results = $dao->updateUserAttempt($reqData,'ResetAttempt');
          expDebug::dPrint("User Attempt Reset :".$results[0]->Message);
          //if(stripos($results[0]->Message,"AL")>=0) {
          list($AcMsg,$AcCnt) = split("#",$results[0]->Message);
          if($AcMsg =="AL") {
            $isvalid="false";
          } else {
            $isvalid="true";
          }
          expDebug::dPrint("isvalid :".$isvalid);
          $validateMsg = $results[0]->Message;
        }else{
          $results = $dao->updateUserAttempt($reqData,'UpdateAttempt');
          expDebug::dPrint("User Attempt Update :".$results[0]->Message);
          $validateMsg = $results[0]->Message;
        }
      } else {
        $isValidUser="false";
        expDebug::dPrint("Invalid user");
        $validateMsg = "User not found";

      }
      $outData = '<UserValidateResponse>';
      $outData .= '<ValidateUserServiceResponse>'.$isvalid.'</ValidateUserServiceResponse>';
      $outData .= '<ValidationMessage>'.$validateMsg.'</ValidationMessage>';
      $outData .= '</UserValidateResponse>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
      	
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function generateCaptcha($reqData) {
    try{
      $length=6;
      $_rand_src = array(
      array(48,57) //digits
      , array(97,122) //lowercase chars
      //        , array(65,90) //uppercase chars
      );
      srand ((double) microtime() * 1000000);
      $random_string = "";
      for($i=0;$i<$length;$i++){
        $i1=rand(0,sizeof($_rand_src)-1);
        $random_string .= chr(rand($_rand_src[$i1][0],$_rand_src[$i1][1]));
      }
      $enc = new Encrypt();
      $random_string = $enc->encrypt($random_string);
      $outData = '<GenerateCaptcha>'.$random_string.'</GenerateCaptcha>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
    }catch (Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function validateCaptcha($reqData){
    try{
      $entered=trim($reqData->captcha_entered).'';
      $val=$reqData->captcha;
      $enc = new Encrypt();
      $valid='false';
      $entered=$enc->encrypt($entered);
      $check=($entered==$val);
      if ($check){
        $valid='true';
      } else {
        $valid='false';
      }
      $outData = '<ValidateCaptcha>'.$valid.'</ValidateCaptcha>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
    }catch (Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function validateUsername($reqData){
    $isvalid="false";
    $enc = new Encrypt();
    try{
      $name=$reqData->name;
      expDebug::dPrint("received ".$name);
      $dao = new AuthenticateDAO();
      $results = $dao->getUserPwd($reqData);
      if($results && sizeOf($results) == 1){
        $isvalid="false";	//already in use
      } else {
        $isvalid=true; //not is use
      }
      $outData = '<ValidateUsername>'.$isvalid.'</ValidateUsername>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
      	
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function validateEmail($reqData){
    $isvalid="false";
    $enc = new Encrypt();
    try{
      $email=$reqData->email;
      expDebug::dPrint("received ".$email);
      $dao = new AuthenticateDAO();
      $results = $dao->getUserEmail($reqData);
      if($results && sizeOf($results) >= 1){
        $isvalid="false";	//already in use
      } else {
        $isvalid=true; //not is use
      }
      $outData = '<ValidateEmail>'.$isvalid.'</ValidateEmail>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
      	
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function validatePassword($reqData){
    $isvalid="false";
    $enc = new Encrypt();
    try{
      $email=$reqData->email;
      expDebug::dPrint("received ".$email);
      $dao = new AuthenticateDAO();
      $results = $dao->getUserEmail($reqData);
      if($results && sizeOf($results) == 1){
        $isvalid="false";	//already in use
      } else {
        $isvalid=true; //not is use
      }
      $outData = '<ValidateEmail>'.$isvalid.'</ValidateEmail>';
      $outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
      return $outData;
      	
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

}

$server = new SoapServer("../modules/core/exp_sp_core/wsdl/authenticateservice.wsdl");
$server->setClass("AuthenticateService");
$server->handle();

?>