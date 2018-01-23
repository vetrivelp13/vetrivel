<?php
    include_once "./sites/all/services/GlobalUtil.php";
    $oConfig  =  new GlobalUtil();
	$aConfig = $oConfig->getConfig();
	$memServer = $aConfig['memcache_server1'];
	$memSrvDt = explode(':',$memServer);
//vincent test for idev
	// Connect to the memcache server
	$memcache = new Memcache;
    $connectedToMemCache = $memcache->connect($memSrvDt[0], $memSrvDt[1]);
    //$memcache->connect("localhost",11211);

    echo "Connected to memcache server - ".$memSrvDt[0]. " : ". $memSrvDt[1]."<br />\n";
    echo "  Memcache server's version: " . $memcache->getVersion() . "<br />\n";

    echo "Invalidating all memcache data...<br />\n";
    $memcache->flush();
    echo "  ...all memcache data is now invalidated.<br />\n";

    $memcache->close();
    echo "Closed connection to memcache server.<br />\n";
?>
