<?php
$testCases = array(

		
		'GetHighlyRatedCoursesAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'limit'=>10),
						  'output'=>array('id','title','short_description','code','entity_type','vote_value'),
						  'result'=>array('<results totalrecords =','<result>','<id>','<title>','<short_description>','<code>','<entity_type>','<vote_value>'),
						  'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1),
						  'output'=>array('id','title','short_description','code','entity_type','vote_value'),
						  'result'=>array('<results totalrecords =','<result>','<id>','<title>','<short_description>','<code>','<entity_type>','<vote_value>'),
						  'output_type'=>'xml'
						)
						
							
				)
		),  
			
);
?>