<html>
<head>
	<meta charset="UTF-8">
</head>
<body class='mainclass'>
<?php 
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

global $user;
if($user->uid == 1) { // Authorised only for Super admin Access
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";

	$util = new GlobalUtil();
	$config = $util->getConfig();

	$db_url = $config["db_url"];
	$info = parse_url($db_url);
	// server name
	$servername = $info['host'];
	if(isset($info["port"])){
		$servername.=":".$info["port"];
	}
	$username =$info['user']; // username
	$password =$info['pass']; // password
	// get db name
	$tmp1    = explode("@",$db_url);
	$dbname  = substr($tmp1[1],stripos($tmp1[1],"/")+1,strlen($tmp1[1]));
	expDebug::dPrint('$dbname : '.$dbname,4);
	$num_rec_per_page=50; // Setting 50 for Each page
	// Connect to sql
	$con = new mysqli($servername, $username, $password, $dbname);

	if (mysqli_connect_errno())
	{
		echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}

	if (isset($_GET["page"]) || !empty($_GET["page"])) {
		$page  = $_GET["page"];
	} else { $page=1;
	}; // page Count

	$start_from = ($page-1) * $num_rec_per_page;
	// select the Slt_version
	$sql = "SELECT * FROM slt_version LIMIT $start_from, $num_rec_per_page";
	$rs_result =  $con->query($sql);//run the query

	?>
<!--  Header file For Expertusone -->
<div id="header-outer">
	<div id="header">
		<a href="index.php" id="logo" title="Home"><img
			src="<?php /* echo getDomainDoc();?>/sites/all/themes/core/expertusoneV2/logo.png"*/ echo get_logo_path();?>"
			alt="Home" /> </a>
	</div>
</div>
<!-- Table Fetched Query data -->
<div id="showcontainer">
	<table>
		<tr>
			<th>Id</th>
			<th>Release_no</th>
			<th>Release_name</th>
			<th>Applied_on</th>
			<th>Remark</th>
		</tr>
		<!-- <div id ='headersec'><div>Id</div><div>Release_no</div><div>Release_name</div><div>Applied_on</div><div>Remark</div></div> -->
		<?php 
		while ($row = mysqli_fetch_assoc($rs_result)) {
			?>
		<tr>
			<td><?php echo $row['id']; ?></td>
			<td><?php echo $row['release_no']; ?></td>
			<td><?php echo $row['release_name']; ?></td>
			<td><?php echo $row['applied_on']; ?></td>
			<td><?php echo $row['remark']; ?></td>
		</tr>
		<?php 
		}
		?>
	</table>
	<?php 
	// pagination Concept
	$sql = "SELECT * FROM slt_version";

	$fullRecords =  $con->query($sql);//run the query

	$total_records = mysqli_num_rows($fullRecords);  //count number of records

	$total_pages = ceil($total_records / $num_rec_per_page);

	/* pagination Starts */
	echo "<div class='pagination'>";

	if($total_pages > 1){
		// echo "<a href='version.php?page=1'>".'First page'."</a> "; // Goto 1st page
		for ($i=1; $i<=$total_pages; $i++) {
			$currentClass = ($i == $page) ? 'active' : '';
			echo "<a class ='page ".$currentClass."' href='version.php?page=".$i."'>".$i."</a> ";
		}
		// echo "<a href='version.php?page=$total_pages'>".' Last page'."</a> "; // Goto last page
	}

	echo "</div></div>";
	$con->close();
}
else
{
	print "You are not Authorized to access this Service.";
}

// Header Logo Reference newadminapi.php
function getDomainDoc()
{
	/*** get the url parts ***/
	$url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'] : "http://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'];
	$parts = parse_url($url);
	/*** return the host domain ***/
	return $parts['scheme'].'://'.$parts['host'];
}
?>
</body>
</html>
<!-- Style Code Starts here -->
<style>
body {
	overflow-x: hidden;
	width: 100%;
}

#header-outer {
	border-bottom: 1px solid #ccc;
	padding-bottom: 15px;
}

#showcontainer {
	margin: 0 auto;
	width: 60%;
}

table {
	font-family: verdana, arial, sans-serif;
	font-size: 11px;
	color: #333333;
	border-width: 1px;
	border-color: #000;
	/* border-collapse: collapse; */
	font-size: 18px;
	padding-top: 10px;
}

table th {
	background: #f18cc1;
	border-width: 1px;
	padding: 8px;
	border-style: solid;
	border-color: #000;
}

table td { /* background:#23285f;
	background:#1999d6;
	*/
	background: #23285f;
	border-width: 1px;
	padding: 8px;
	border-style: solid;
	border-color: #000;
	color: #fff;
}

.page {
	background: none repeat scroll 0 0 #e9e9e9;
	border: 1px solid #c0c0c0;
	border-radius: 3px;
	box-shadow: 0 1px 0 rgba(255, 255, 255, 0.8) inset, 0 1px 3px
		rgba(0, 0, 0, 0.1);
	color: #717171;
	display: inline-block;
	font-size: 0.875em;
	font-weight: bold;
	margin-right: 4px;
	padding: 0 9px;
	text-decoration: none;
	text-shadow: 0 1px 0 rgba(255, 255, 255, 1);
}

.page.active {
	background: none repeat scroll 0 0 #616161;
	border: medium none;
	box-shadow: 0 0 8px rgba(0, 0, 0, 0.5) inset, 0 1px 0
		rgba(255, 255, 255, 0.8);
	color: #f0f0f0;
	text-shadow: 0 0 3px rgba(0, 0, 0, 0.5);
}

.pagination {
	margin-bottom: 20px;
	padding: 20px;
}
</style>