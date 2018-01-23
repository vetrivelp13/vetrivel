<?php 
/*
 * This is sample interface for content launch in mobile device. It needs to be redesigned after SCORM content test passed.
*/

?>
<html>
<head>
  <title>ExpertusONE - SSO Launch</title>
</head>
    
<?php 
header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT"); // Date in the past
header("Cache-Control: no-cache"); 
header("Pragma: no-cache"); 
$url="";
$url=$_GET["url"];
?>
<body id="bdy">
	<DIV STYLE="WIDTH:100%;text-align:center;color:#dddddd;background:#ECECEC">
		<h1>CORPORATE SSO LOGIN</h1>
	</DIV>
	 
<script> 
var ifrm = document.createElement("IFRAME");
ifrm.setAttribute("src", "<?php echo $url;?>");
document.body.appendChild(ifrm); 
alert(4); 
</script> 
</body>
</html>
