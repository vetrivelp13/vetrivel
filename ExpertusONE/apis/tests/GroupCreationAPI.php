<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_name = getRandomWord();
$org_id = db_query("select id from slt_organization limit 0,1")->fetchField();
$org_id1 = db_query("select id from slt_organization where name = 'Expertus'")->fetchField();
$org_id2 = db_query("select id from slt_organization where name = 'Information Technology'")->fetchField();
$org_id3 = db_query("select id from slt_organization where name = 'Logicalis Pte. Ltd.'")->fetchField();
$org_id4 = db_query("select id from slt_organization where name = 'IBM - Europe'")->fetchField();
$job_role1 = db_query("select name from slt_profile_list_items where code = 'cre_usr_jtl_019'")->fetchField();
$user_type1 = db_query("select name from slt_profile_list_items where code = 'cre_usr_ptp_007'")->fetchField();

$testCases = array(

			// GroupCreationAPI test case Starts
			'GroupCreationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
						  'output'=>array('Id','name'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
			  				),
					array(
							'input'=>array('userid'=>1,'name'=>'Learning Admin - Europe','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Learning Admin - Europe','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'People Admin - US','is_admin'=>1,'department'=>'cre_usr_dpt_hrs','country'=>'US'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Super Admin','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'System Admin','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Commerce Admin','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Report Admin','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Content Admin','is_admin'=>1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'People Admin - Europe','is_admin'=>1,'department'=>'cre_usr_dpt_hrs','country'=>'FR'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Learning Admin - APAC','is_admin'=>1,'department'=>'cre_usr_dpt_hrs','country'=>'PL'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Expertus_Admin','is_admin'=>1,'org_id'=>$org_id1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'All_Learners','is_admin'=>0),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Learner_Partner','is_admin'=>0,'org_id'=>$org_id2,'user_type'=>'cre_usr_ptp_pnr'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Learner_Employee','is_admin'=>0,'org_id'=>$org_id4,'user_type'=>'cre_usr_ptp_emp'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Learner_Customer','is_admin'=>0,'org_id'=>$org_id3,'user_type'=>'cre_usr_ptp_cmr'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Employee Admin','job_role'=>$job_role1,'is_admin'=>1,'org_id'=>$org_id4,'department'=>'cre_usr_dpt_hrs'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Customer Admin','job_role'=>$job_role1,'is_admin'=>1,'org_id'=>$org_id3,'department'=>'cre_usr_dpt_hrs'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Partner Admin','job_role'=>$job_role1,'is_admin'=>1,'org_id'=>$org_id2,'department'=>'cre_usr_dpt_hrs'),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Sandbox User','is_admin'=>0,'user_type'=>$user_type1),
							'output'=>array('Id','name'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),		
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
						  'output'=>array(),
						  'result'=>array('<results totalrecords =','<result>','<Id>','<name>The group name '.$random_name.' already exist. Choose another group name.</name>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'','org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'sdsf','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given user type sdsf doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'ftmd','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given employment type ftmd doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'indi','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given country indi doesn\'t exists</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>'rwrw','department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given state rwrw doesn\'t exists</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'IT dept','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given department IT dept doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'role1','language'=>'cre_sys_lng_eng','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given job role role1 doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'english','role'=>'mgr'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given preferred language english doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>$random_name,'org_id'=>$org_id,'is_admin'=>1,'user_type'=>'cre_usr_ptp_emp','employment_type'=>'cre_usr_etp_ftm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_mis','job_role'=>'cre_usr_jrl_rl1','language'=>'cre_sys_lng_eng','role'=>'manager'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given role is invalid</short_msg>'),
							'output_type'=>'xml'
							)
				   
				)
			),  // GroupCreationAPI test case Ends
			
		); // End of api test case main array
?>