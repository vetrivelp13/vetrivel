<?php
// $Header: /root/Product-bkup/SPP/Construction/ExpertusONE/sites/all/commonlib/html2pdf/utils_number.php,v 1.1 2011-01-03 13:10:13 srprabhu Exp $

function arabic_to_roman($num) {
  $arabic = array(1,4,5,9,10,40,50,90,100,400,500,900,1000); 
  $roman = array("I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M");
  $i = 12;
  $result = "";
  while ($num) { 
    while ($num >= $arabic[$i]) { 
      $num -= $arabic[$i]; 
      $result .= $roman[$i];
    } 
    $i--; 
  } 

  return $result;
}
?>