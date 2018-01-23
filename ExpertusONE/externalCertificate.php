<?php 
define('DRUPAL_ROOT', getcwd());
require_once 'includes/bootstrap.inc';
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once "./sites/all/services/GlobalUtil.php";

drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);

//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_myteam/exp_sp_myteam.module";
//include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_myteam/exp_sp_myteam.inc";
$encryted_id = $_GET['cretid'];

$cert_id = db_query("select certificate_id from slt_external_certificate where encrypted_id = :enc_id ", array('enc_id' => $encryted_id) )->fetchField();
//echo "select certificate_id from slt_external_certificate where encrypted_id = $encryted_id";

?>
<html>
<head>
	
<script type="text/javascript" src="/misc/jquery.js?v=1.4.4"></script>
<script type="text/javascript" src="/misc/jquery.once.js?v=1.2"></script>
<script type="text/javascript" src="/misc/drupal.js?oydwxt"></script>
<script type="text/javascript" src="/misc/ui/jquery.ui.core.min.js?v=1.8.7"></script>
<script type="text/javascript" src="/misc/ui/jquery.ui.widget.min.js?v=1.8.7"></script>
<script type="text/javascript" src="/sites/all/modules/core/exp_sp_core/js/exp_sp_jquery/jquery.metadata.js?oydwxt"></script>
<script type="text/javascript" src="/sites/all/modules/core/exp_sp_core/js/AbstractDetailsWidgetUi.js?oydwxt"></script>
<script type="text/javascript" src="/sites/all/modules/core/exp_sp_core/js/AbstractManagerUi.js?oydwxt"></script>
<script type="text/javascript" src="/misc/ajax.js?v=7.4"></script>
<script type="text/javascript" src="/sites/all/modules/core/exp_sp_widget/exp_sp_widget.js?oydwxt"></script>
<script type="text/javascript" src="/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_myteam/exp_sp_myteam_myapproval.js?oydwxt"></script>
	
</head>
<body>
	
<div id="lnr-myteam-approval" class="yeslink" style="display: block;">

<div id="reject" class="yeslink" style="display: inline-block; text-decoration:none">
	<a href="javascript:void(0)" class="team-listLink" style = "font-family: arial,helvetica,sans-serif;
    font-size: 15px;
    font-weight: bold;
    color: #808080;
    text-decoration: none;"
		name="Assing Learning"
		onclick="jQuery('#lnr-myteam-approval').data('lnrmyteamapproval').VerifyRejectCert(<?php print $cert_id;?>,'Rejected');"><?php print t('No');?></a>
</div>

<div id="verify"  class="yeslink" style="display: inline-block; text-decoration:none">
	<a href="javascript:void(0)" class="team-listLink"  style = "font-family: arial,helvetica,sans-serif;
    font-size: 15px;
    font-weight: bold;
    color: #008000;
    text-decoration: none;"
		name="Assing Learning"

		onclick="jQuery('#lnr-myteam-approval').data('lnrmyteamapproval').VerifyRejectCert(<?php print $cert_id;?>,'Verified');"><?php print 'Yes';?></a>
	
</div>


</div>
	
</body>
</html>