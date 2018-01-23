<?php 
//$courseId = db_query("select id from slt_course_template order by id desc limit 1 ")->fetchField();
$courseId = db_query("select id,code from slt_course_template order by id desc limit 2")->fetchField();
expDebug::dPrint("jooo-- ".print_r($courseId,true),1);
$testCases = array(

			// List GetCourseDetailsAPI test case Starts
			'GetCourseDetailsAPI' => array(
				'datasource' => array(
					array('input'=>array('courseId'=>$courseId[0]->id,'userid'=>1),
								'output'=>array('id','title','code','description','short_description','view_count'),
								'result'=>array('<results totalrecords =','<id>','<title>','<code>','<description>','<short_description>','<view_count>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('courseId'=>$courseId[0]->id,'courseCode'=>$courseId[1]->code,'userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','<short_msg>'."CourseID is Invalid.".'</short_msg>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
					array('input'=>array('courseId'=>'','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','<short_msg>'."Mandatory Fields are missing in the feed. List provided below:".'</short_msg>','courseId'),
								'output_type'=>'xml'
							)
				)
			),  // List GetCourseDetailsAPI test case Ends
			
		); // End of api test case main array
?>