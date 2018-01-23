<?php 
$loc_name = db_query("select name from slt_location limit 0,1")->fetchField();
$testCases = array(

			// ListInstructorClassesAPI test case Starts
			'ListInstructorClassesAPI' => array(
				'datasource' => array(
					array('input'=>array('UserID'=>1,'regstatuschk'=>'','limit'=>1,'userid'=>1),
						  'output'=>array('id','courseid','course_title','cls_title','cls_code','description','descriptionfull','delivery_type','delivery_type_code','basetype','classprice','language','updated_by','updated_by_name','is_compliance','mro,mro_id','MarkComplete','launch','sessionDetails','classStatus','survey_status','survey_completed_count','assessment_status','assessment_completed_count'),
						  'result'=>array('<results totalrecords =','<result>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('UserID'=>'','regstatuschk'=>'','limit'=>1,'userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('UserID'=>'','regstatuschk'=>'','limit'=>1,'userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('UserID'=>'','regstatuschk'=>'','limit'=>'','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							)
						
					
				   
				)
			),  // ListInstructorClassesAPI test case Ends
			
		); // End of api test case main array
?>