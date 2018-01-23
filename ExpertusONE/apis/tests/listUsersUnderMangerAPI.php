<?php 

$person = db_query("select id from slt_person where is_manager = 'Y' limit 1")->fetchField();
$notmanager = db_query("select id from slt_person where is_manager = 'N' limit 1")->fetchField();

$testCases = array(


		'listUsersUnderMangerAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'user_id'=>$person),
								'output'=>array('id','first_name','last_name','full_name','user_name','phone_no','mobile_no','email,manager_id','report_type','job_title','job_role','points','badges','enroll_count','user_picture'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'user_id'=>''),
								'output'=>array('id','first_name','last_name','full_name','user_name','phone_no','mobile_no','email,manager_id','report_type','job_title','job_role','points','badges','enroll_count','user_picture'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$notmanager),
								'output'=>array('id'),
								'result'=>array('<short_msg>The given user is not a manager</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$person,'reporttype' => 'direct,virtual'),
								'output'=>array('id','first_name','last_name','full_name','user_name','phone_no','mobile_no','email','manager_id','report_type','job_title','job_role','points','badges','enroll_count','user_picture'),
								'result'=>array('<results totalrecords =','<result>','<id>','<first_name>','<last_name>','<full_name>','<user_name>','<phone_no>','<mobile_no>','<email>','<manager_id>','<report_type>','<job_title>','<job_role>','<points>','<badges>','<enroll_count>','<user_picture>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$person,'reporttype' => 'direct'),
								'output'=>array('id','first_name','last_name','full_name','user_name','phone_no','mobile_no','email','manager_id','report_type','job_title','job_role','points','badges','enroll_count','user_picture'),
								'result'=>array('<results totalrecords =','<result>','<id>','<first_name>','<last_name>','<full_name>','<user_name>','<phone_no>','<mobile_no>','<email>','<manager_id>','<report_type>','<job_title>','<job_role>','<points>','<badges>','<enroll_count>','<user_picture>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$person,'reporttype' => 'virtual'),
								'output'=>array('id','first_name','last_name','full_name','user_name','phone_no','mobile_no','email','manager_id','report_type','job_title','job_role','points','badges','enroll_count','user_picture'),
								'result'=>array('<results totalrecords =','<result>','<id>','<first_name>','<last_name>','<full_name>','<user_name>','<phone_no>','<mobile_no>','<email>','<manager_id>','<report_type>','<job_title>','<job_role>','<points>','<badges>','<enroll_count>','<user_picture>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						)
				),
		);

?>