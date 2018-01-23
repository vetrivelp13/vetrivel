<?php
$program_id=db_query("select program_id from slt_module_crs_mapping where course_id in (select course_id from slt_enrollment) order by program_id desc limit 0,1")->fetchField();
expDebug::dPrint("Program Id taken::".$program_id);
$course_id=db_query("select course_id from slt_module_crs_mapping where course_id in (select course_id from slt_enrollment) order by course_id desc limit 0,1")->fetchField();
$pr_id=db_query("select program_id from slt_module_crs_mapping where course_id in (select course_id from slt_enrollment) order by program_id desc limit 0,1")->fetchField();
$user_name=db_query("select user_name from slt_person where id in(select user_id from slt_enrollment where course_id in (select course_id from slt_module_crs_mapping where program_id=$pr_id))")->fetchField();
expDebug::dPrint("User Name taken::".$user_name);
expDebug::dPrint("Program Id with user name taken::".$pr_id);
$testCases = array(
		'ListTPRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'ProgramId'=>$program_id),
						  'output'=>array('id','registered_user_id','user_name','full_name','overall_status','overall_status_name','reg_date','comp_date','cancel_date','updated_on'),
						  'result'=>array('<results totalrecords = "','<id>','<registered_user_id>','<user_name>','<full_name>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ProgramId'=>$program_id,'date_from'=>aa),
						  'output'=>array(),
						  'result'=>array('From date should be in yyyy-mm-dd format'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ProgramId'=>$program_id,'date_to'=>aa),
						  'output'=>array(),
						  'result'=>array('To date should be in yyyy-mm-dd format'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ProgramId'=>$program_id,'UserName'=>$user_name),
						  'output'=>array('id','registered_user_id','user_name','full_name','overall_status','overall_status_name','reg_date','comp_date','cancel_date','updated_on'),
						  'result'=>array('<results totalrecords = "','<id>','<registered_user_id>','<user_name>','<full_name>'),
						   'output_type'=>'xml'
							)
							 				
				
				)
			)
		);
			
?>		