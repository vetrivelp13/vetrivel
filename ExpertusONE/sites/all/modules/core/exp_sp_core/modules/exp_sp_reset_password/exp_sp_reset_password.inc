<?php 
function insertPaswordToPassPolicy($password){
  $txn = db_transaction();
  try{
  global $user;
  /*Code belongs to password policy, to update hitory table*/
  $password     = user_hash_password(trim($password));
  if (module_exists('password_policy')) {
    $usrPassPolicy  = db_insert('password_policy_history');
    $usrPassPolicy->fields(array(
                    'uid' => $user->uid,
                    'pass' =>$password,
                    'created' => time(),
                  ));
    $usrPassPolicy->execute();
  }
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('insertPaswordToPassPolicy', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);  
}
?>