<?php 
$da = array();
$da = [
        ["English"=>"Welcome","German"=>"Answer1","Chinese"=>"Welcome","Spanish"=>"Welcome","Russian"=>"Welcome"],
        ["English"=>"Welcome","German"=>"Welcome","Chinese"=>"Welcome","Spanish"=>"Welcome","Russian"=>"Welcome"],
        ["English"=>"Welcome","German"=>"Welcome","Chinese"=>"Welcome","Spanish"=>"Welcome","Russian"=>"Welcome"],
      ];

$results = ["sEcho" => 1,
        	"iTotalRecords" => 3,
        	"iTotalDisplayRecords" =>3,
        "aaData" => $da
        ];
    echo json_encode($results);


?>