<?php 
$User_id = db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$tp_id = db_query("select id from slt_program where price != 0 order by id desc limit 0,1")->fetchField();
$course_id = db_query("select course_id from slt_module_crs_mapping where program_id = $tp_id")->fetchField();
$class_id = db_query("select id from slt_course_class where course_id = $course_id")->fetchField();
$object_type = db_query("select object_type from slt_program where price != 0 order by id desc limit 0,1")->fetchField();

expDebug::dPrint('personid'.$User_id);
expDebug::dPrint('programid'.$tp_id);
expDebug::dPrint('courseId'.$course_id);
expDebug::dPrint('classId'.$class_id);
expDebug::dPrint('objectType'.$object_type);


$testCases = array(

			// addTPToCartAPI test case Starts
			'addTPToCartAPI' => array(
				'datasource' => array(
					array('input'=>array('user_id'=>$User_id,'tpId'=>$tp_id,'Classids'=>$class_id,'object_type'=>$object_type,'userid'=>1),
						  'output'=>array('status','class_list','tp_courses'),
						  'result'=>array('<results totalrecords =','<result>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('user_id'=>$User_id,'tpId'=>$tp_id,'object_type'=>$object_type,'userid'=>1),
							'output'=>array('status','class_list','tp_courses'),
							'result'=>array('<results totalrecords =','<result>','<status>'),
							'output_type'=>'xml'
							)
				),
				'validate' => array(
					array(	'input'=>array('user_id'=>'','tpId'=>$tp_id,'Classids'=>$class_id,'object_type'=>$object_type,'userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('user_id'=>$User_id,'tpId'=>'','Classids'=>$class_id,'object_type'=>$object_type,'userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('user_id'=>$User_id,'tpId'=>$tp_id,'Classids'=>$class_id,'object_type'=>'','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							)
				   
				)
			),  // addTPToCartAPI test case Ends
			
		); // End of api test case main array
?>