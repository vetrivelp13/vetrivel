  <?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_name = getRandomWord();
$org_id = db_query("select id from slt_organization limit 0,1")->fetchField();
expDebug::dPrint("org test -- ".print_r($org_id,true),1);
$group_id = db_query("select id from slt_groups order by id desc limit 0,1")->fetchField();
expDebug::dPrint("group test -- ".$group_id);
$testCases = array(

			// GroupUpdateAPI test case Starts
			'GroupUpdateAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
						  'output'=>array('Id','name'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
						  )
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
						  'output'=>array('Id','name'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('userid'=>1,'id'=>'','name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>'','org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'partnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given user type partnr doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'part','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given employment type part doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'India','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given country India doesn\'t exists</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>'tamilnadu','department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given state tamilnadu doesn\'t exists</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'finance','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given department finance doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'jobrole2','language'=>'cre_sys_lng_eng','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given job role jobrole2 doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'english','role'=>'ins'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given preferred language english doesn\'t exists or inactive</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'id'=>$group_id,'name'=>$random_name,'org_id'=>$org_id,'user_type'=>'cre_usr_ptp_pnr','employment_type'=>'cre_usr_etp_ptm','country'=>'IN','state'=>25,'department'=>'cre_usr_dpt_fin','job_role'=>'cre_usr_jrl_rl2','language'=>'cre_sys_lng_eng','role'=>'instructor'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>The given role is invalid</short_msg>'),
							'output_type'=>'xml'
							)
					
				)
			),  // GroupUpdateAPI test case Ends
			
		); // End of api test case main array
?>