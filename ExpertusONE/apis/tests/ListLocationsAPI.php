<?php 
$loc_name = db_query("select name from slt_location limit 0,1")->fetchField();
$testCases = array(

			// ListLocationsAPI test case Starts
			'ListLocationsAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'textfilter'=>$loc_name),
						  'output'=>array('location_id','title','type','status'),
						  'result'=>array('<results totalrecords =','<result>','<location_id>','<title>','<type>','<status>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1),
							'output'=>array('location_id','title','type','status'),
							'result'=>array('<results totalrecords =','<result>','<location_id>','<title>','<type>','<status>'),
							'output_type'=>'xml'
					)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'textfilter'=>$loc_name),
						  'output'=>array('location_id','title','type','status'),
						  'result'=>array('<results totalrecords =','<result>','<location_id>','<title>','<type>','<status>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('userid'=>1,'textfilter'=>''),
						  'output'=>array('location_id','title','type','status'),
						  'result'=>array('<results totalrecords =','<result>','<location_id>','<title>','<type>','<status>'),
						  'output_type'=>'xml'
						)
				   
				)
			),  // ListLocationsAPI test case Ends
			
		); // End of api test case main array
?>