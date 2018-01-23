<?php
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
require_once DRUPAL_ROOT."/sites/all/services/Trace.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class TranslationMigration {
  public function __construct(){
  	try{
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }

  public function process(){
  	try {
  	    $this->translateCustomAttributeLabels();
  	}
  	catch(Exception $e){
  		expDebug::dPrint($e);
  	}
       
  }
  private function translateCustomAttributeLabels()
  {
  	try {
  		global $user;
  		//Dynamic translation
  		 $loggedInUserId = getIdOfLoggedInUser();
  		 $result = db_query("select id,cattr_name,cattr_help_txt from slt_custom_attr c where c.id not in(select l.entity_id from slt_labels_store l where l.entity_type='cre_sys_obt_cattr' )")->fetchAll();
  		 for($i = 0; $i < count($result);$i++)
  		 {
  		 $insertArr = array();
  		 $insertArr["entity_id"] = $result[$i]->id;
  		 $insertArr["entity_type"] = "cre_sys_obt_cattr";
  		 $insertArr["entity_field_name"] = "cattr_name";
  		 $insertArr["src_label"] = $result[$i]->cattr_name;
  		 $insertArr["src_lang"] = "en";
  		 
  		 $insertArr['created_on'] = now();
  		 $insertArr['created_by'] = $loggedInUserId;
  		 $insertArr['updated_on'] = now();
  		 $insertArr['updated_by'] = $loggedInUserId;
  		 db_insert("slt_labels_store")->fields($insertArr)->execute();
  		 
  		 $insertArrHelpTxt = array();
  		 $insertArrHelpTxt["entity_id"] = $result[$i]->id;
  		 $insertArrHelpTxt["entity_type"] = "cre_sys_obt_cattr";
  		 $insertArrHelpTxt["entity_field_name"] = "cattr_help_txt";
  		 $insertArrHelpTxt["src_label"] = $result[$i]->cattr_help_txt;
  		 $insertArrHelpTxt["src_lang"] = "en";
  		 
  		 $insertArrHelpTxt['created_on'] = now();
  		 $insertArrHelpTxt['created_by'] = $loggedInUserId;
  		 $insertArrHelpTxt['updated_on'] = now();
  		 $insertArrHelpTxt['updated_by'] = $loggedInUserId;
  		 db_insert("slt_labels_store")->fields($insertArrHelpTxt)->execute();
  		 
  		 }
  		 expDebug::dPrint('Migration results for labels store::'.print_r($result,true),5);
  		 
  		 
  		 $result = db_query("select id,cattr_id,opt_name from slt_custom_attr_options c where c.id not in(select l.entity_id from slt_labels_store l where l.entity_type='cre_sys_obt_cattr_options' )")->fetchAll();
  		 for($i = 0; $i < count($result);$i++)
  		 {
  		 $insertArr = array();
  		 $insertArr["entity_id"] = $result[$i]->id;
  		 $insertArr["entity_type"] = "cre_sys_obt_cattr_options";
  		 $insertArr["entity_field_name"] = "opt_name";
  		 $insertArr["src_label"] = $result[$i]->opt_name;
  		 $insertArr["src_lang"] = "en";
  		 
  		 $insertArr['created_on'] = now();
  		 $insertArr['created_by'] = $loggedInUserId;
  		 $insertArr['updated_on'] = now();
  		 $insertArr['updated_by'] = $loggedInUserId;
  		 db_insert("slt_labels_store")->fields($insertArr)->execute();
  		 }
  		 expDebug::dPrint('Ending Migration Process for Custom Attribute',1);
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  
  
  }
}

global $user;
if($user->uid==1)
{
    $drupalServ=new TranslationMigration();
  $rtn =  $drupalServ->process();
  print "Completed Successfully<br>";
}
else
{
  print "You are not Authorized to access this Service.";         
}
?>