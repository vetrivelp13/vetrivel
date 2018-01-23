<?php 

/**
 * Added by Vincent on Jun 27, 2016.
 * getallheraders is not available in php-fpm and nginx kind of server
 * an alternate is checking if the function exists or not if not
 * then create this function. 
 */
if (!function_exists('getallheaders')){ 
    function getallheaders(){ 
       $headers = ''; 
       foreach ($_SERVER as $name => $value){ 
           if (substr($name, 0, 5) == 'HTTP_'){ 
               $headers[str_replace(' ', '-', ucwords(strtolower(str_replace('_', ' ', substr($name, 5)))))] = $value; 
           } 
       } 
       return $headers; 
    } 
} 



?>