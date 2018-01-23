<?php
$courseid= db_query("select id from slt_course_template order by id desc limit 1")->fetchField();
expDebug::dprint("test-->1".print_r($courseid,true),1);
$testCases = array(
			// List CoursesDetailsAPI test case Starts
			'CoursesDetailsAPI' => array(
				'datasource' => array(
					array('input'=>array('CourseId'=>$courseid,'userid'=>1),
								'output'=>array('cls_id','cls_title','cls_code','cls_delivery_type','cls_status','cls_language'),
								'result'=>array('<results totalrecords =','<cls_id>','<cls_title>','<cls_code>','<cls_delivery_type>','<cls_status>','<cls_language>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
					array('input'=>array('classId'=>'','userid'=>1),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg><long_msg>1.CourseId'),
								'output_type'=>'xml'
							)
				)
			),  // List CoursesDetailsAPI test case Ends
			
		); // End of api test case main array
?>