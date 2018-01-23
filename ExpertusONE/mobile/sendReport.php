<?php
function sendcrashReport(){
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
require_once($_SERVER['DOCUMENT_ROOT'].'/sites/all/commonlib/phpmailer/PHPMailerAutoload.php');
$phpMailer = new PHPMailer();
$phpMailer->CharSet = 'UTF-8';
$date = date("Y_m_d");
$hostname = $_SERVER['HTTP_HOST'];
$prev_date = date('d.m.Y',strtotime("-1 days"));
$phpMailer->setFrom('mobileadmin@expertus.com', 'mobile Admin');
$phpMailer->addAddress('mobileadmin@expertus.com', 'My Reports');
$phpMailer->Subject  = 'ExpertusONE App Crash report_'.$hostname;
$phpMailer->Body     = 'Crash Report_'.$prev_date;
$path = $_SERVER['DOCUMENT_ROOT'] ."/sites/default/files/tmpreports/bugreport.txt";
$flName = 'bugreport.txt';
if(file_exists($path)){
$phpMailer->addAttachment($path, $flName, 'base64', 'application/txt');
if(!$phpMailer->send()) {
expDebug::dPrint('Message was not sent.');	
expDebug::dPrint('Mailer error:'.$phpMailer->ErrorInfo);	
} else {
expDebug::dPrint('crashreports send sucessfully.');
if(unlink($path)) expDebug::dPrint('Deleted file.');
}
}
}
?>
