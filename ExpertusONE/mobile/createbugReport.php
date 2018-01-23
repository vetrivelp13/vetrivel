<?php

$errorlog = $_REQUEST['errorlog'];
$errorlog = base64_decode($errorlog);
$date = date("Y-m-d");
$userid = $_REQUEST['user_id'];
$hostUrl = $_REQUEST['hosturl'];
$filename = 'bugreport';
//echo $filename;
$content = nl2br('Crashlog:-'.$errorlog. '  Host URL :-'.$hostUrl. '  UserId :-'.$userid . '  date: ' . $date) ;
$fp = fopen($_SERVER['DOCUMENT_ROOT'] ."/sites/default/files/tmpreports/$filename.txt","a");
fwrite($fp,"\n". $content);
fclose($fp);


?> 
