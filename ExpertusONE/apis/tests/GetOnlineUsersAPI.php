<?php 
$testCases = array(

			// GetOnlineUsersAPI test case Starts
			'GetOnlineUsersAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'rows'=>5),
						  'output'=>array('uid','job_title','full_name'),
						  'result'=>array('<results totalrecords =','<result>','<uid>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'rows'=>5),
						  'output'=>array('uid','job_title','full_name'),
						  'result'=>array('<results totalrecords =','<result>','<uid>'),
						  'output_type'=>'xml'
							),
						array(
								'input'=>array('userid'=>1,'rows'=>'three'),
								'output'=>array(),
								'result'=>array('<results totalrecords =','</results>'),
								'output_type'=>'xml'
						)
				   
				)
			),  // GetOnlineUsersAPI test case Ends
			
		); // End of api test case main array
?>