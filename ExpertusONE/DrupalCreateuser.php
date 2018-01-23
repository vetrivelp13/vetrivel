<?php
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
include_once "./sites/all/services/Encryption.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

global $user;

if($user->uid==1)
{
    $sql="SELECT user_name,password,email FROM slt_person WHERE user_name NOT IN(select name from users)";
    $sqlResult=db_query($sql);
    while($userinfo=db_fetch_object($sqlResult))
    {
        $enc = new Encrypt();
        $uname=$userinfo->user_name;
        $decrypt=$enc->decrypt($userinfo->password);
        $pass=$decrypt;
        $email=$userinfo->email;
        
        $details = array(
                'name' => $uname,
                'pass' => $pass,
                'mail' => $email,
                'access' => 1, 
                'status' => 1
              );
              $user = user_save(null, $details);
    } 
print "Completed Successfully"; 
}
else
{
    print "You are not Authorized to access this Service.";             
}

?>