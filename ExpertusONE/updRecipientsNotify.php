<?php 
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$recipienList = db_query("select id,notification_sendto from slt_notification_info");
$recipienResult = $recipienList->fetchAll();


foreach($recipienResult as $recipienValue) {
  $name = $recipienValue->notification_sendto;
  //print($name);
  $newCode = explode(',',$name);
  foreach($newCode as $key => $value){
    if(strtolower(trim($value)) == 'user'){
      $newCode[$key] = 'cre_ntn_rpt_usr';
    }
    else if(strtolower(trim($value)) == 'manager'){
      $newCode[$key] = 'cre_ntn_rpt_mgr';
    }
    else if(strtolower(trim($value)) == 'instructor'){
      $newCode[$key] = 'cre_ntn_rpt_ins';
    }
    else if(strtolower(trim($value)) == 'admin'){
      $newCode[$key] = 'cre_ntn_rpt_adm';
    }
  }
  $newValue = implode(',',$newCode);
  $keywordInsert = db_query("update slt_notification_info set notification_sendto ='".$newValue."' where id = '".$recipienValue->id."' ;");
}

?>