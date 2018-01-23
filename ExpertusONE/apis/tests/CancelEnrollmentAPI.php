<?php 
$user_id = db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$testCases = array(

			// CancelEnrollmentAPI test case Starts
			'CancelEnrollmentAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'enrollId'=>5),
						  'output'=>array('status','msg'),
						  'result'=>array('<results totalrecords =','<result>','<status>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'enrollId'=>5),
						  'output'=>array('status','msg'),
						  'result'=>array('<results totalrecords =','<result>','<status>'),
						  'output_type'=>'xml'
							),
				   
				)
			),  // CancelEnrollmentAPI test case Ends
			
		); // End of api test case main array
?>