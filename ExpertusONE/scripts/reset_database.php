<?php 

include_once $_SERVER['DOCUMENT_ROOT']."/includes/common.inc";
include_once $_SERVER['DOCUMENT_ROOT']."/includes/database/database.inc";
include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

global $user;

if($user->uid != 1){
	echo "You are not authorized to access this page.";
	exit;
}

$db_url=getConfigValue("db_url");

$info=parse_url($db_url);

$username = $info["user"];
$pass = $info["pass"];
$host = $info["host"];
$port = !isset($info["port"]) ? 3306 : $info["port"];
$host = $host.':'.$port;
$db = basename($info['path']);

$fileContent = file_get_contents($_SERVER['DOCUMENT_ROOT'].'/scripts/reset_data.sql');

$allQry = explode('$$',$fileContent);

$i=0;
$j=0;
$finalQry = array();
foreach($allQry as $k => $v){
	if(stripos(trim($v),'-- ')!== false){
		unset($allQry[$k]);
	}else{
		$finalQry[$i][$j] = trim($v);
		$j++;
		if($j==15){
			$j=0; // reset array index
			$i++;
		}
	}
}

$mysqli = new mysqli($host,$username,$pass,$db);

if ($mysqli->connect_errno) {
    printf("Connect failed: %s\n", mysqli_connect_error());
    exit();
}
//echo "<br/>Execution started...";

foreach($finalQry as $k => $v){
	expDebug::dPrint("Final Query List before execute $k ---> ".print_r($v,true),4);
	$query = implode(";",$v);
	//expDebug::dPrint($query,4);
	expDebug::dPrint("Query Length -- ".print_r(strlen($query),true),4);
	if ($mysqli->multi_query($query)) {
    do {
        if ($result = $mysqli->store_result()) {
            $result->free();
        }
    } while ($mysqli->next_result());
	}
}

if($mysqli->error){
	echo "Error in data clean up ".$mysqli->error;
}

$mysqli->close();
echo "<br/>Data reset completed successfully";
?>