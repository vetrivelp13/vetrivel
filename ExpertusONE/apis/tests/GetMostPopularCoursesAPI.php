<?php 
$testCases = array(

			// GetMostPopularCoursesAPI test case Starts
			'GetMostPopularCoursesAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'learnerid'=>40,'limit'=>10),
						  'output'=>array('id','title','short_description','code','entity_type'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'learnerid'=>40,'limit'=>10),
						  'output'=>array('id','title','short_description','code','entity_type'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'learnerid'=>'','limit'=>10),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
					 		)
				   
				)
			),  // GetMostPopularCoursesAPI test case Ends
			
		); // End of api test case main array
?>