<?php
//include_once($_SERVER['DOCUMENT_ROOT']. '/expertusone_auth.php');

/* Added by ganeshbabuv to implement the salesforce cookieless option (#0054508) on 30th Sep 2015 10:40 AM
 * If cookie doesn't set in ExpertusONE, load the drupal bootstrap module */

if((!isset($_COOKIE) || empty($_COOKIE) || trim($_COOKIE['SPLearnerInfo'])=="" || trim($_COOKIE['SPCertificate'])=="") && $_REQUEST["from_salesforce"]=="1" && trim($_REQUEST["exp_sess_id"])!="" || $_REQUEST["from_ptp"]=="1"){
	define('DRUPAL_ROOT', getcwd());
	require_once 'includes/bootstrap.inc';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
}else{
	include_once($_SERVER['DOCUMENT_ROOT']. '/expertusone_auth.php');
} 

define('DRUPAL_ROOT', getcwd());

include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once "./includes/bootstrap.inc";
include_once "./sites/all/services/GlobalUtil.php";

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

/* Added by ganeshbabuv to implement the salesforce cookieless option (#0054508) on 30th Sep 2015 10:40 AM
 * If cookie doesn't set in ExpertusONE, it gets the session id from salesforce and reload the session in ExpertusONE according to session id  */

if(module_exists('salesforce_integration')){
	if((!isset($_COOKIE) || empty($_COOKIE) || trim($_COOKIE['SPLearnerInfo'])=="" || trim($_COOKIE['SPCertificate'])=="") && $_REQUEST["from_salesforce"]=="1" && trim($_REQUEST["exp_sess_id"])!=""){  
		$exp_sess_id=trim($_REQUEST['exp_sess_id']); 
		expDebug::dPrint("Salesforce Session Id in Print Certificate Request = ".print_r($exp_sess_id,true),5);
		if($exp_sess_id!=''){ 
			get_previous_session_for_sf_cookieless($exp_sess_id);
		}
	}
}

$obj = new GlobalUtil();
$conf = $obj->getConfig();


/******* Author: Rajesh.N 
  Descriptiopn: United Rentals PTP Page 
  Date: 27-Sep-2017
  *******/
if($_REQUEST["from_ptp"]=="1") {

  $referrer_url = trim($_SERVER['HTTP_REFERER']);

  if($referrer_url != ''){

    $referrer_parsed_url=parse_url($referrer_url);  

    $conv_allowed_domains = explode(',', $conf['allowed_domains']);

    if(in_array($referrer_parsed_url['host'], $conv_allowed_domains))
    {

      if($_REQUEST["token"]){
          $userId = $_REQUEST['userid'];
          $vClassId = core_decrypt_ptp($_REQUEST['classid'], $userId);
          $vEnrollmentId  = core_decrypt_ptp($_REQUEST['enrollid'], $userId);
      } else{
          drupal_goto("access_denied");
      }

    } else {
        drupal_goto("access_denied");
    }
  }
} else { 

global $user;

$vClassId       	= core_decrypt($_REQUEST['classid']); // URL decryption added while print certificate
$vEnrollmentId   	= core_decrypt($_REQUEST['enrollid']);
$userId				= $_REQUEST['userid'];

$vPrint = $_REQUEST['print'];
if($userId != getIdOfLoggedInUser()) {
	expDebug::dPrint("user id $userId from request doesnt match with logged in user id ".getIdOfLoggedInUser(), 4);
	drupal_goto("access_denied");
}
}
if($_REQUEST["from_ptp"]=="1") {
	$fullname = '';
	if($_REQUEST["full_name"])
		$fullname = str_replace('_', ' ', $_REQUEST["full_name"]);
} else {
if($user->uid){
  $select = db_select('slt_person','person');
  $select->addField('person','full_name');
 // $select->addField('person','preferred_language');
  $select->condition('user_name', $user->name);
//   expDebug::dPrintDBAPI('printcertificate.php: $select', $select);
  $userRec = $select->execute()->fetch();
//   expDebug::dPrint('printcertificate.php: $userRec = ' . print_r($userRec, true));

  $fullname = $userRec->full_name;
 // $preferred_language = $userRec->preferred_language;
}
}
$result_flag = ''; // modified by joolavasavi #52141
if (!empty($_REQUEST['certifyfrom']) && $_REQUEST['certifyfrom'] == 'LP') {
  $entityType = 'cre_sys_obt_trp';
  // Fetch the training plan title to display in certificate, its completion date to display in certificate
  $select = db_select('slt_master_enrollment', 'menr');
  $select->leftJoin('slt_program', 'prg', 'menr.program_id = prg.id');
  $select->addField('prg', 'title', 'prg_title');
  $select->addField('prg', 'code', 'prg_code');
  $select->addField('menr', 'comp_date', 'compdate');
  $select->addField('prg', 'id', 'program_id');
  $select->addField('prg', 'object_type', 'object_type');
  $select->addField('menr', 'score', 'score');
  $select->addExpression("if(expires_in_value = '',NULL,(DATE_FORMAT(
  		IF (prg.expires_in_unit = 'days', DATE_ADD(menr.comp_date, INTERVAL prg.expires_in_value DAY),
  		IF (prg.expires_in_unit = 'months', DATE_ADD(menr.comp_date, INTERVAL prg.expires_in_value MONTH),
  		DATE_ADD(menr.comp_date, INTERVAL prg.expires_in_value YEAR))),
  		'%d-%m-%Y')))", "expires_in");
  $select->condition('menr.id', $vEnrollmentId);
  $select->condition('menr.user_id', $userId);
  $select->condition('menr.overall_status', 'lrn_tpm_ovr_cmp');
  
  expDebug::dPrintDBAPI('select completed enrollment from slt_master_enrollment', $select, 4);

  $infoRec = $select->execute()->fetch();
  expDebug::dPrint('completed enrollment from slt_master_enrollment result' . print_r($infoRec, true), 4);
  if(empty($infoRec)) {
  	drupal_goto("access_denied");
  }

//   expDebug::dPrint('printcertificate.php: TP $infoRec = ' . print_r($infoRec, true));
if($infoRec->score != '0.00'){
  $score = $infoRec->score;
}
else {
	$score = '';
}
  $title = $infoRec->prg_title;
  $code = $infoRec->prg_code;
  $date = date_create($infoRec->compdate);
  $entityId = $infoRec->program_id;
  $validtill = $infoRec->expires_in;
  $select = db_select('slt_attendance_summary','sas');
  $select->leftJoin('slt_enrollment', 'se', 'sas.enrollment_id = se.id');
  $select->addField('sas','content_status','content_status');
  $select->condition('se.master_enrollment_id', $vEnrollmentId,'=');
//   expDebug::dPrintDBAPI('printcertificate.php: TP $select', $select);
  $contentStatusArr = $select->execute()->fetchAll();

  expDebug::dPrint('$contentStatusArr ---->' . print_r($contentStatusArr, true),4);
  if(count($contentStatusArr)>0){
  	//$result_flag = '';
	  foreach ($contentStatusArr as $content_status){
	  	if($content_status->content_status=='passed')
	  		$result_flag = 'Passed';
	  	if($content_status->content_status=='failed'){ // Make content_status fails, when any onf of the lession failed in multilession SCROM and AICC content
	  		$result_flag = 'Failed';
	  		break;
	  	}
  	}
  }
}
else{
  $entityType = 'cre_sys_obt_crs';
  // Fetch the course title to display in certificate, its completion date to display in certificate and its id
  $select = db_select('slt_enrollment', 'enr');
  $select->leftJoin('slt_course_template', 'crs', 'enr.course_id = crs.id');
  $select->addField('crs', 'title', 'crs_title');
  $select->addField('crs', 'code', 'crs_code');
  $select->addField('enr', 'comp_date', 'compdate');
  $select->addField('crs', 'validity_date', 'validitydate');
  $select->addField('crs', 'validity_days', 'validitydays');
  $select->addField('crs', 'is_compliance', 'iscompliance');
  $select->addField('crs', 'id', 'course_id');
  $select->addField('enr', 'score', 'score');
  $select->condition('enr.id', $vEnrollmentId);
  $select->condition('enr.class_id', $vClassId);
  $select->condition('enr.user_id', $userId);
  $select->condition('enr.comp_status', 'lrn_crs_cmp_cmp');

  expDebug::dPrintDBAPI('select completed enrollment from slt_enrollment', $select, 4);

  $infoRec = $select->execute()->fetch();
  expDebug::dPrint('completed enrollment from slt_enrollment result' . print_r($infoRec, true), 4);
  if(empty($infoRec)) {
  	drupal_goto("access_denied");
  }
  $validtill = '';
  if($infoRec->iscompliance == 1){
  	$complianceValidityDate   = '';
  	if($infoRec->validitydate != '') {
  		$cvdate = new DateTime($infoRec->validitydate);
  		$complianceValidityDate = $cvdate->format('d-m-Y');
  	}else if(empty($complianceValidityDate) && !empty($infoRec->validitydays)) {
  		$complianceValidityDateFrm = strtotime(date("d-m-Y", strtotime($infoRec->compdate)) . "  +".$infoRec->validitydays." days");
  		$complianceValidityDate 	= strftime("%d-%m-%Y",$complianceValidityDateFrm);
  	}
  	$validtill = $complianceValidityDate;
  }
  if($infoRec->score != '0.00'){
  $score = $infoRec->score;
  }
  else {
  	$score = '';
  }
  $title = $infoRec->crs_title;
  $code = $infoRec->crs_code;
  $date = date_create($infoRec->compdate);
  $entityId = $infoRec->course_id;
  $select = db_select('slt_course_class','class');
  $select->addField('class','scheduled_duration');
  $select->condition('class.id', $vClassId);
//   expDebug::dPrintDBAPI('sabee CRS $select', $select);
  $durRec = $select->execute()->fetch();
  $duration = $durRec->scheduled_duration;
	if(!empty($duration)) {
  	  $hours = floor($duration / 60);
  	  $minutes = ($duration % 60);
  	  $durHrs = sprintf('%02d:%02d',$hours,$minutes).' ' .t('LBL422');
	}
	else {
  	  $duration = '';
  	  $durHrs = '';
   } 
//   expDebug::dPrint('$durRec CRS $infoRec' . print_r($durRec, true));

  $select = db_select('slt_attendance_summary','sas');
  $select->addField('sas','content_status','content_status');
  $select->condition('sas.enrollment_id', $vEnrollmentId,'=');
  $contentStatusArr = $select->execute()->fetchAll();
//   expDebug::dPrint('$contentStatusArr ---->' . print_r($contentStatusArr, true),4);

  if(count($contentStatusArr)>0){
  	//$result_flag = '';
	  foreach ($contentStatusArr as $content_status){
	  	if($content_status->content_status=='passed')
	  		$result_flag = 'Passed';
	  	if($content_status->content_status=='failed'){ // Make content_status fails, when any onf of the lession failed in multilession SCROM and AICC content
	  		$result_flag = 'Failed';
	  		break;
	  	}
}
  }
}

$obj = new GlobalUtil();
$conf = $obj->getConfig();
$tmp_url = $conf["pdf_temp_dir"];
$site_name = $conf["notification_image_path"];
 if(module_exists('theme_override')) {
$cert_path = get_logo_path('folder');
}else {
	$cert_path = $base_url. '/'. path_to_theme() . '/images/certificate/';
}
//$cert_path = get_logo_path('folder');
expDebug::dPrint("Certificate path:->".$cert_path);
$selectTemplate = db_select('slt_entity_notification_mapping', 'nmap');
$selectTemplate->leftJoin('slt_notification_info', 'ninfo', 'nmap.notify_id = ninfo.id');
$selectTemplate->leftJoin('slt_notification_frame', 'nframe', 'ninfo.id = nframe.notification_id');
$selectTemplate->addField('nframe', 'notification_template', 'template');
$selectTemplate->addField('nframe', 'lang_code', 'lang_code');
$selectTemplate->condition('nmap.entity_id', $entityId);
$selectTemplate->condition('nmap.entity_type', $entityType);
//$selectTemplate->condition('nframe.lang_code', $preferred_language );
$selectTemplate->condition('nmap.notify_send_type', 'Certificate');
$selectTemplate->condition('ninfo.status', 'cre_ntn_sts_atv');
$selectTemplate->condition('nmap.status', 'Y');
// expDebug::dPrintDBAPI('printcertificate.php: $selectTemplate', $selectTemplate);

$templateInfoRec = $selectTemplate->execute()->fetch();
// expDebug::dPrint('printcertificate.php: $templateInfoRec = ' . print_r($templateInfoRec, true));

if (empty($templateInfoRec)) { // When there is no nmap entry, use the first available English Certificate template
  $selectTemplate = db_select('slt_notification_frame', 'nframe');
  $selectTemplate->leftJoin('slt_notification_info', 'ninfo', 'nframe.notification_id = ninfo.id');
  $selectTemplate->addField('nframe', 'notification_template', 'template');
  $selectTemplate->addField('ninfo', 'lang_code', 'lang_code');
  $selectTemplate->condition('nframe.send_options', 'Certificate', '=');
  $selectTemplate->condition('ninfo.lang_code', 'cre_sys_lng_eng', '=');
  $selectTemplate->condition('ninfo.status', 'cre_ntn_sts_atv');
  $selectTemplate->condition('ninfo.notification_code', 'prncert', '=');
  $selectTemplate->range(0,1);

//   expDebug::dPrintDBAPI('printcertificate.php: when no nmap record, $selectTemplate', $selectTemplate);

  $templateInfoRec = $selectTemplate->execute()->fetch();
//   expDebug::dPrint('printcertificate.php: when no nmap record, $templateInfoRec = ' . print_r($templateInfoRec, true));
}

if (empty($templateInfoRec)) {
	// There is no template to use.
	expertusErrorThrow(new Exception("No template for certificate"));
}

$template = $templateInfoRec->template;
$templateLangCode = $templateInfoRec->lang_code;
	$confdateformat = getConfigValue('date_format');
	$defdateformat = 'd-m-Y';
	$setdate = empty($confdateformat)? $defdateformat : $confdateformat;
	$valdate = '';
	$valsetdate = '';
	$expires_on = '';	//modified by joolavasavi #52141
	if(($entityType == 'cre_sys_obt_trp' && $infoRec->object_type == 'cre_sys_obt_crt' && !empty($validtill)) || ($infoRec->iscompliance == 1 && $entityType == 'cre_sys_obt_crs')){
		$valdate = date_create($validtill);
		$valsetdate = empty($confdateformat)? $defdateformat : $confdateformat;
		$expires_on = date_format($valdate,$valsetdate);
	 }/* else{
		$valdate = '';
		$valsetdate = '';
	}*/
// To find the key words in the template

$keyword = array(
    '#@full_name@#'              => $fullname,
    //'#@course_title@#'           => wordwrap($title, 40, PHP_EOL, true),
    '#@course_title@#'           => sanitize_data($title),
	'#@course_code@#'            => sanitize_data($code),
    '#@course_completed_date@#'  => date_format($date,$setdate),
    '#@certificate_image_path@#' => $cert_path,
    '#@expires_on@#'             => $expires_on,
	'#@scheduled_duration@#'     => $durHrs,
	'#@score@#'                  => $score,
	'#@success_status@#'         => $result_flag
);
$duration_seperator = GetBetween($template,t('LBL248'),'#@scheduled_duration@#');
$score_seperator = GetBetween($template,t('LBL668'),'#@score@#');
if(empty($duration) && $duration == '') {
	$template = str_replace(t('LBL248').$duration_seperator, '', $template);
}
if(empty($score) && $score == '') {
	$template = str_replace(t('LBL668').$score_seperator, '', $template);
}
foreach ($keyword as $k => $v) {
  $template = str_replace($k, $v, $template);
};
if(($infoRec->iscompliance != 1 && $entityType == 'cre_sys_obt_crs') || ($entityType == 'cre_sys_obt_trp' && $infoRec->object_type != 'cre_sys_obt_crt') || empty($expires_on)){
	$template = str_replace('Valid Till:', '', $template);
}
expDebug::dPrint("printcertificate.php: Certificate HTML = " . print_r($template, true), 5);

require_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
$util=new GlobalUtil();
$config=$util->getConfig();
expDebug::dPrint('printcertificate.php: $config = ' . print_r($config, true), 5);
$requestPerMinute = 0;
$regularFont = ''; //$config["pdf_font_regular"];
$boldFont = ''; //$config["pdf_font_bold"];
$utf8Fonts = '';
if (!empty($regularFont)) {
	$utf8Fonts = $regularFont;
	if (!empty($boldFont)) {
		$utf8Fonts .= ', ' . $boldFont;
	}
}
expDebug::dPrint('printcertificate.php: $utf8Fonts = ' . $utf8Fonts, 5);

global $theme_key;
global $language;
/************start***************/
$cert_css_path = $base_url. '/'. drupal_get_path('module', 'theme_override');
$filename = $_SERVER['DOCUMENT_ROOT'].'/'. drupal_get_path('module', 'theme_override').'/css/expertusone_en_override.css';

$backgroundcss = getConfigValue('use_css_back_ground_image');

if ((function_exists('theme_override_init')) && (file_exists($filename))) { ?>
<link href="<?php echo $cert_css_path; ?>/css/expertusone_en_override.css" rel="stylesheet">
<?php } else {?>
	<style>
		#page-container .white-btn-bg-left {
			display: none;
		}
		#page-container .admin-save-button-middle-bg {
		  background:#27245e;
		  border:1px solid #27245e;
		  border-radius:6px;
		}
		#page-container .admin-save-button-right-bg {
		  display:none
		}
		#page-container .admin-save-button-left-bg {
		  display:none
		}
	</style>
<?php }
/************end***************/
?>
<!DOCTYPE html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta http-equiv="Cache-Control" content="no-store, no-cache, must-revalidate, post-check=0, pre-check=0" />
<meta http-equiv="Pragma" content="no-cache">
<link type="text/css" rel="stylesheet" media="all"
	href="<?php print $site_name;?>/style.css" />
<style>
<?php
if($language->language == 'ru'){
	?>
.printcert-btn {
  width: 132px
}
	<?php
}else{
	?>
		.printcert-btn {
  width: 93px
}
	<?php

}
?>
@font-face {
    font-family: "ProximaNovaBold";
    src: url("/sites/all/themes/core/expertusoneV2/expertusone-internals/font/ProximaNova-Bold.ttf");
 }
#printcert_maincont h1 {
 background: none;
 width: 1024px;
 margin: 0 auto
}
.admin-save-button-left-bg {
  float: left;
  height: 22px;
  width: 7px;
}
.admin-save-button-right-bg {
  float: left;
  height: 22px;
  width: 7px
}
.printcert-btn {
  border: 0;
  cursor: pointer;
  color: #FFF;
  float: left;
  font-size: 12px;
  font-weight: normal;
  padding: 2px 0 6px 0;
  margin: 0;
  text-align: center;

}
.closetext {
  float: left;
  margin-right: 10px
}
.printcert-close {
  margin-right: 8px
}
.closetext a {
  background: none;
  border: 0 solid #FF801B;
  color: #4686DD;
  font-family: Verdana,Arial,sans-serif;
  font-size: 12px;
  font-weight: normal;
  text-align: center;
  text-decoration: none;
  line-height: 24px
}
#printcert_button {
  width: auto;
  float: right;
  margin-bottom: 10px
}
#page-container #printcert_button {
  width: auto
 }
.cert-button-wrapper {
  overflow: hidden
}
#page-container .white-btn-bg-left {
  float: left;
  background: url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/search_bg.png") no-repeat 0 -380px;
  height: 34px;
  width: 15px;
  display:none
}
#page-container .white-btn-bg-right {
  float: left;
  width: 15px;
  background: url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/search_bg.png") no-repeat scroll 0 -452px;
  height: 34px;
  margin-right: 8px;
  display:none
}
#page-container .white-btn-bg-middle {
  font-family:ProximaNovaBold;
  font-size:12px;
  color:#474747;
  text-align:center;
  text-transform:uppercase;
  line-height:24px;
  background:#fff;
  margin:0;
  border:1px solid #dedede;
  border-radius:6px;
  padding:3px 12px 2px;
  float:left;
  cursor:pointer;
  width:auto
}
#page-container .admin-save-button-left-bg {
  float: left;
  height: 34px;
  width: 15px;
}
#page-container .admin-save-button-middle-bg {
  font-family:ProximaNovaBold;
  font-size:12px;
  color:#fff;
  text-align:center;
  text-transform:uppercase;
  line-height:22px;
  background:#27245e;
  margin:0;
  padding:3px 12px 2px;
  float:left;
  cursor:pointer;
  width:auto
}
#page-container .admin-save-button-right-bg {
  float: left;
  height: 34px;
  width: 15px;
}
#printcert_maincont > div > div > div > div img {
  width: auto;
  height: auto
}
@media screen and (-webkit-min-device-pixel-ratio:0) {
.printcert-btn {
  padding: 4px 0
}
}
</style>
<!--[if IE]>
<style>
.printcert-btn {
  line-height: 20px;
  padding: 0 0 6px 0;
}
</style>
<![endif]-->
<!--[if (gt IE 8)|!(IE)]>
<style>
.printcert-btn {
  line-height: 19px;
}
</style>
<![endif]-->
<style>
/* Below hack for IE10 (this applies also to IE9) */
@media screen and (min-width:0\0) {
.printcert-btn {
  line-height: 22px;
  padding:0
}
#page-container .admin-save-button-middle-bg {
  line-height: 24px
}
}
</style>

<script type="text/javascript">

</script>
</head>

<?php if($vPrint!='1'){ ?>
<?php if($theme_key == "expertusoneV2"){?>
<div id="page-container">
<?php }?>
<div class="cert-button-wrapper">
<div id="printcert_button">
	<span class="closetext">
	<?php if($theme_key == "expertusoneV2"){?>
	<div class="white-btn-bg-left"></div>
	<a href="javascript:void(0);" id="Close" class="white-btn-bg-middle" onclick="javascript:window.close();"><?php print t('LBL123'); ?></a>
	<div class="white-btn-bg-right"></div>
	<?php }else{?>
	<a href="javascript:void(0);" id="Close" class="printcert-close" onclick="javascript:window.close();"><?php print t('LBL123'); ?></a>
	<?php }?>
	</span>
	<span>
		<div class="admin-save-button-left-bg"></div>
		<input type="button" class="printcert-btn admin-save-button-middle-bg " value="<?php print t('LBL673');?>" name="convert_to_pdf"	id="convert_to_pdf" onclick="javascript:getPdfConverter();">
		<div class="admin-save-button-right-bg"></div>
	</span>
</div>
</div>


<?php }  ?>

<div class="printcert_maincont" id="printcert_maincont"
	style="width: auto; margin: 0 auto;">
  <div<?php if (!empty($utf8Fonts)) { print ' style="font-family:'. $utf8Fonts . '"'; } ?>><?php print $template; ?></div>
</div>
<div id = "printcert_contlang" style="display:none">
  <?php print $templateLangCode; ?>
</div>
<?php if($theme_key == "expertusoneV2"){?>
</div>
<?php }?>
<?php 
function GetBetween($content,$start,$end){
	$r = explode($start, $content);
	if (isset($r[1])){
		$r = explode($end, $r[1]);
		return $r[0];
	}
	return '';
}
?>
<!--

/*
   * Start # 0041917 -  Qa link is shown in salesforce app
   * Added By : Ganesh Babu V, Dec 9th 2014 5:00 PM
   * Description: Remove the below js code due to getting issue when implement print certificate in Popup for salesforce
   * Ticket : #0041917 -  Qa link is shown in salesforce app
   */
  -->

  <?php
  //modified by joolavasavi #52141
  if(!isset($_REQUEST["from_salesforce"]) || trim($_REQUEST["from_salesforce"])!="1") { ?>
    	<script src="sites/all/modules/core/exp_sp_core/js/exp_sp_jquery/jquery.min.js"></script>
   <?php } ?>

 <!--  End # 0041917 -  Qa link is shown in salesforce app  -->
<script>
  function getPdfConverter() {
    var oContent = document.getElementById("printcert_maincont").innerHTML;
    var oLanguage = document.getElementById('printcert_contlang').innerHTML;
    $('body').append("<form style=\"display:none;\" id=\"postcertihtmlform\" name=\"printform\" method=\"POST\" action=\"/sites/all/commonlib/pdf_template.php\" target=\"_self\"></form>");
    $('#postcertihtmlform').append($('<input type="hidden" name="bodyContent"></input>').attr('value', oContent));
    $('#postcertihtmlform').append($('<input type="hidden" name="contentLanguage"></input>').attr('value', oLanguage));
    $('#postcertihtmlform').submit();
    $('#postcertihtmlform').remove();    
  }
</script>