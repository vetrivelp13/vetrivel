<?php
/**
 * @class or PHP Name		: AuthenticateDAO
 * @author(s)				: Vincent.S
 * Version         			: 1.0
 * Date						: 09/07/2009
 * PHP Version          		: 5.2.6
 * Description     			: DAO Class to do basic CRUD operations in user table.
 */

include_once "AbstractDAO.php";

class AuthenticateDAO extends AbstractDAO{
  public function __construct(){
    parent::__construct();
  }

  public function fetch($inObj){
    try{
      //	$dispfield = '"id,user_name username,full_name fullname"';
      //$qry = " select id,username from user_profile where is_active = 'Y' and username= '".$inObj->username."' and password='".$inObj->password."' " ;
      /*$qry = "call slp_person_sel(";
       $qry .= "null";
       $qry .= ",".$dispfield;
       if(isset($inObj->isGlobal) && $inObj->isGlobal){
       $qry .= '," status=\'cre_usr_sts_atv\' and BINARY user_name=\''.$inObj->username.'\'"';
       }  else {
       $qry .= '," status=\'cre_usr_sts_atv\' and BINARY user_name=\''.$inObj->username.'\' and password = \''.$inObj->password.'\'"';
       }
       	
       $qry .= ',null';
       $qry .= ")";
       */
      expDebug::dPrint("DEBUGGING AUTHENTICATE DAO ".$inObj->username);	
      $qry = "call slp_login_sel(";
      /*if(isset($inObj->isGlobal) && $inObj->isGlobal){
       $qry .= '"\''.$inObj->username.'\'"';
       $qry .= ',null';
       $qry .= ',null';
       }  else {
       $qry .= '"\''.$inObj->username.'\'"';
       //$qry .= ',"\''.$inObj->password.'\'"';
       $qry .= ',"\'$S$C.qQZv.uu5Vsv9McXLQJo9UDZ0QazNExghrwrtc70GhO5SGdgJ58\'"';
       $qry .= ',"\'no\'"';
       }*/
      $qry .= "'".addslashes($inObj->username)."'";
      $qry .= ',null';
      $qry .= ',null';
      $qry .= ")";
      	
      expDebug::dPrint("AuthenticateDAO Query :".$qry);
      $this->connect();
      $res = $this->query($qry);
      $result = $this->fetchResult();
      $result->savedfullname = $result->fullname;
      $result->fullname  = $result->firstname." ".mb_substr($result->lastname,0,1,'UTF-8');
      $this->closeconnect();
      return $result;
    }catch(Exception $e){
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }


  public function getUserPwd($inObj){
    expDebug::dPrint($inObj->pass);
    try{
      $fieldlist='"id Id,user_name UserName,status Status,custom4 AccountLock"';
      $qry = "call slp_person_sel(";
      $qry .= "null";
      $qry .= ",$fieldlist";
      $qry .= ', " user_name = \''.$inObj->name.'\'"';
      $qry .= ",null";
      $qry .= ");";
      expDebug::dPrint("query ".$qry);
      $this->connect();
      $res = $this->query($qry);
      $result=$this->fetchAllResults();
      $this->closeconnect();
      return $result;
    }catch(Exception $e){
      $this->rollbackTrans();
      $this->closeconnect();
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function getUserEmail($inObj){
    expDebug::dPrint($inObj->email);
    try{
      $fieldlist='"id Id,user_name UserName"';
      $qry = "call slp_person_sel(";
      $qry .= "null";
      $qry .= ",$fieldlist";
      $qry .= ', " email = \''.$inObj->email.'\'"';
      $qry .= ",null";
      $qry .= ");";
      expDebug::dPrint("query ".$qry);
      $this->connect();
      $res = $this->query($qry);
      $result=$this->fetchAllResults();
      $this->closeconnect();
      return $result;
    }catch(Exception $e){
      $this->rollbackTrans();
      $this->closeconnect();
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }

  public function updateUserAttempt($inObj,$action){
    try{
      $qry = "call slp_person_attempt_upd(";
      $qry .= "'".$inObj->name."'";
      $qry .= ", '".$action."'";
      $qry .= ");";
      expDebug::dPrint("query ".$qry);
      $this->connect();
      $res = $this->query($qry);
      $result=$this->fetchAllResults();
      $this->closeconnect();
      return $result;
    }catch(Exception $e){
      $this->rollbackTrans();
      $this->closeconnect();
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }
/*
  public function getProfileIdFields(){
    try{
      $qry = "SELECT id FROM slt_profile_list_items WHERE code = 'cre_usr_ptp_gst' AND lang_code = 'cre_sys_lng_eng'";
      expDebug::dPrint("profile id -  ".$qry);
      $this->connect();
      $res = $this->query($qry);
      $result=$this->fetchResult();
      expDebug::dPrint("profile resultid -  ".$result->id);
      	
      $this->closeconnect();
      return $result->id;
    }catch(Exception $e){
      $this->rollbackTrans();
      $this->closeconnect();
      throw new SoapFault("SPLMS",$e->getMessage());
    }
  }
  */

}
?>