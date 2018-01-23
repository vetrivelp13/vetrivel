<?php
echo $argc;
if (in_array($argv, array('--help')) || $argc != 3) {
	?>
  This is a script to run cron from the command line.
 
  It takes 1 arguments, which must both be specified:
  
  
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php content_migration.php --document-root '/path/to/drupal/root'
  
 
<?php
} else {
set_time_limit(0);

// Loop through arguments and extract the cron key and drupal root path.
for ($i = 1; $i < $argc; $i++) {
	switch ($argv[$i]) {
		case '--document-root':
			$i++;
			$path = $argv[$i];
			break;
	}
}
echo $path;
// Sets script name
$_SERVER['SCRIPT_NAME'] = 'content_migration.php';

// Values as copied from drupal.sh
$_SERVER['HTTP_HOST'] = 'default';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['SERVER_SOFTWARE'] = NULL;
$_SERVER['REQUEST_METHOD'] = 'GET';
$_SERVER['QUERY_STRING'] = '';
$_SERVER['HTTP_USER_AGENT'] = 'console';
$_SERVER['DOCUMENT_ROOT'] = $path;


//define('DRUPAL_ROOT', $_SERVER["DOCUMENT_ROOT"]);
define('DRUPAL_ROOT', $path);
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// global $user;


// if($user->uid==1)
// {

	/**
	 * Content Type Update Block in Attendance tables
	 */
	//update  slt_attendance_details table content_type
	$qrydetails= "update slt_attendance_details as sad left join slt_content_version as scs ON scs.id = sad.content_version_id left join slt_survey as ss ON ss.id = sad.assessment_id set content_type = if(scs.content_sub_type is null, ss.type, scs.content_sub_type)";
	expDebug::dPrint("update  slt_attendance_details table content_type----------> ".print_r($qrydetails,true),4);
	db_query($qrydetails);
	
	
	//update  slt_attendance_summary table content_type
	$qrysummary= "update slt_attendance_summary as sas left join slt_content_version as scs ON scs.id = sas.content_version_id left join slt_survey as ss ON ss.id = sas.assessment_id set content_type = if(scs.content_sub_type is null, ss.type, scs.content_sub_type)";
	expDebug::dPrint("update  slt_attendance_summary table content_type----------> ".print_r($qrysummary,true),4);
	db_query($qrysummary);
	
	
	/**
	 * Summary Progress Update Block
	 */
	$selectsumStmt = db_select('slt_attendance_summary','sas');
	$selectsumStmt->addField('sas','id', 'id');
	$selectsumStmt->addField('sas','status', 'status');
	$selectsumStmt->addField('sas','content_type', 'content_type');
	$selectsumStmt->addField('sas','suspend_data', 'suspend_data');
	expDebug::dPrintDBAPI('summary progress query------>', $selectsumStmt);
	$resultsum = $selectsumStmt->execute()->fetchAll();
	foreach($resultsum as $sum_details){
		$progress = 0;
		if($sum_details->status=='lrn_crs_cmp_cmp' || $sum_details->content_type == 'sry_det_typ_ass'){ // update 100% for assessments and completed contents
			$progress = 100;
		}else if($sum_details->content_type =='lrn_cnt_typ_vod'){
			$prev_suspend_data =    json_decode(rawurldecode($sum_details->suspend_data), true);
			if($prev_suspend_data['progress']!=null && $prev_suspend_data['progress']!='')
			$progress = $prev_suspend_data['progress'];
		}
		//update  slt_attendance_summary table progress
		$qrysumpro= "update slt_attendance_summary set progress=".$progress." where id = ".$sum_details->id;
		expDebug::dPrint("update  slt_attendance_summary table progress----------> ".print_r($qrysumpro,true),4);
		db_query($qrysumpro);
	}
	
	/**
	 * Enrollment Progress Update Block
	 */
	$selectenrStmt = db_select('slt_enrollment','se');
	$selectenrStmt->addField('se','id', 'id');
	$selectenrStmt->addField('se','user_id', 'user_id');
	$selectenrStmt->addField('se','course_id', 'course_id');
	$selectenrStmt->addField('se','class_id', 'class_id');
	$selectenrStmt->addField('se','comp_status', 'comp_status');
	$selectenrStmt->addField('se','reg_status', 'reg_status');
	expDebug::dPrintDBAPI('Enrollment progress query--->', $selectenrStmt);
	$resultenr = $selectenrStmt->execute()->fetchAll();
	foreach($resultenr as $enr_details){
		$progress = 0;
		if($enr_details->comp_status=='lrn_crs_cmp_cmp'){
			$progress = 100;
		}else{
			if($enr_details->reg_status=='lrn_crs_reg_cnf'){
				$sid = $enr_details->user_id."-".$enr_details->course_id."-".$enr_details->class_id;
				$ClassDelType = getClassDeliveryType($enr_details->class_id, 'Short');
				include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
				$launch_detail = ($ClassDelType=='wbt' || $ClassDelType=='vc' || $ClassDelType=='vod')?getLaunchDetail($enr_details->id,'',$sid):getLaunchDetail($enr_details->id);
				$progress = get_progress($launch_detail, $enr_details->id,1);
			}
		}
		//update  slt_attendance_summary table progress
		$qrysumpro= "update slt_enrollment set progress=".$progress." where id = ".$enr_details->id;
		expDebug::dPrint("update  slt_attendance_summary table progress----------> ".print_r($qrysumpro,true),4);
		db_query($qrysumpro);
	}
	
	
	/**
	 * Master Enrollment Progress Update Block
	 */
	$selectmenrStmt = db_select('slt_master_enrollment','sme');
	$selectmenrStmt->addField('sme','id', 'id');
	$selectmenrStmt->addField('sme','user_id', 'user_id');
	$selectmenrStmt->addField('sme','program_id', 'program_id');
	$selectmenrStmt->addField('sme','overall_status', 'overall_status');
	//$selectmenrStmt->addField('sme','overall_status', 'overall_status');
	expDebug::dPrintDBAPI('Enrollment progress query--->', $selectmenrStmt);
	$resultmenr = $selectmenrStmt->execute()->fetchAll();
	foreach($resultmenr as $menr_details){
		$progress = 0;
		if($menr_details->overall_status=='lrn_tpm_ovr_cmp'){
			$progress = 100;
			//update  slt_attendance_summary table progress
			$qrysumpro= "update slt_master_enrollment set overall_progress=".$progress." where id = ".$menr_details->id;
			expDebug::dPrint("update  slt_attendance_summary table progress----------> ".print_r($qrysumpro,true),4);
			db_query($qrysumpro);
		}else{
			if($menr_details->overall_status!='lrn_tpm_ovr_cln'){
					updateTPEnrollmentOverallPercentComplete($menr_details->program_id, $menr_details->id,'',1);
			}
		}
		
	}
	syncSolrData('Enrollment');
	echo 'completed';
// }
// else
// {
// 	print "You are not Authorized to access this Service.";
// }
}	
?>