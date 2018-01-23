<?php
$user_id=db_query("select user_id from slt_enrollment where master_enrollment_id=1 limit 0,1")->fetchField();
$userid=db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$programid=db_query("select program_id from slt_module_crs_mapping where course_id in (select course_id from slt_enrollment where user_id=$user_id)")->fetchField();
$testCases = array(
		'getTPdetailsAPI' => array(
				'datasource' => array(
						array('input'=>array('user_id'=>$user_id,'userid'=>$userid,'programID'=>$programid),
						  'output'=>array('course_data'),
						  'result'=>array('<results totalrecords = "">','<result>','<course_data>'),
								'output_type'=>'xml'
						)
				),
				
						)
						);
							
						?>
?>