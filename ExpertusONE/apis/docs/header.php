<?php 
define('DRUPAL_ROOT', $_SERVER['DOCUMENT_ROOT']);
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/entity.inc';
require_once DRUPAL_ROOT . '/modules/user/user.module';
include_once $_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/ip_ranges/ip_ranges.module';
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
$list = system_list('module_enabled'); // load enabled modules from cache
if(isset($list['ip_ranges']))
{
	ip_ranges_boot();
}
/*-- #0045521 - Check user has logged --*/
if(user_is_logged_in() == FALSE) {
	$content = '<div align="center" style="color: #333; padding: 40px; font-size: 16px;">';
	$content .= 'You are not Authorized to access this page.';
	$content .= '</div>';
	print $content;
	exit();
}

$image_path = '';
$util=new GlobalUtil();
$config=$util->getConfig();

$image_path_folder = isset($config['logo_image_path']) ? $config['logo_image_path'] :'';
$image_path = $image_path_folder.'/logo.png';
$favicon_path = $image_path_folder.'/favicon.ico';
?>
<!DOCTYPE html>
<html class="js">
<head>
 <title>Documentation | ExpertusONE Developers</title>

<link rel="shortcut icon" href="<?php echo $favicon_path;?>" type="image/x-icon">
<link type="text/css" rel="stylesheet" media="all" href="css/style.css">
<script type="text/javascript" src="javascript/general.js">

</script>
<script type="text/javascript">
jQuery.extend(Drupal.settings, {"basePath":"\/","googleanalytics":{"trackOutgoing":1,"trackMailto":1,"trackDownload":1,"trackDownloadExtensions":"7z|aac|avi|csv|doc|exe|flv|gif|gz|jpe?g|js|mp(3|4|e?g)|mov|pdf|phps|png|ppt|rar|sit|tar|torrent|txt|wma|wmv|xls|xml|zip"},"expertus_autosuggest":{"path":"\/expertus_autosuggest_callback"},"extlink":{"extTarget":"_blank","extClass":"ext","extSubdomains":0,"extExclude":"twitter\\.com\/intent\/","extInclude":"","extAlert":0,"extAlertText":"","mailtoClass":"mailto"}});
</script>
</head>
<body id="page-docs" class="not-front not-logged-in no-sidebars">
  <div id="page">
    <a href="#content-main" class="skip">Skip to Main Content Area</a>


  
  <div id="header-outer">
      <div id="header">
        <a href="index.php" id="logo" title="Home"><img src="<?php echo $image_path /*getDomainDoc();?>/sites/all/themes/core/expertusoneV2/logo.png*/?>" alt="Home" /></a>
<form action="#" accept-charset="UTF-8" method="post" id="search-theme-form">
<!--
<input id="search-q" name="search_theme_form" maxlength="128" size="15" value="Search" title="Enter search terms" placeholder="Search" type="text">
<input id="search-submit" name="op" value="Search" type="submit">
-->
<input name="form_build_id" id="form-7488da9640510bd9eb189c216e716fd9" value="form-7488da9640510bd9eb189c216e716fd9" type="hidden">
<input name="form_id" id="edit-search-theme-form" value="search_theme_form" type="hidden">

</form>
<!--  
<ul class="sf-js-enabled" id="title-nav"><li class="expanded first"><a href="#" title="">API Health</a><ul style="visibility: hidden; display: block;" class="menu"><li class="leaf first"><a href="#" title="">API Status</a></li>
<li class="leaf last"><a href="#" title="">API Issues</a></li>
</ul></li>
<li class="leaf"><a href="#" title="">Blog</a></li>
<li class="leaf"><a href="#" title="">Discussions</a></li>
<li class="expanded active-trail"><a href="#" title="" class="active">Documentation</a></li>
<li class="leaf last"><a href="#" title="">Sign in</a></li>
</ul>  
-->
</div>    </div> 
<?php 

function getDomainDoc()
{
     /*** get the url parts ***/
   $url = (!empty($_SERVER['HTTPS'])) ? "https://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'] : "http://".$_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI'];
    $parts = parse_url($url);
    /*** return the host domain ***/
    return $parts['scheme'].'://'.$parts['host'];
}


?>
