<?php 

$testCases = array(

			// ListUserListValuesAPI test case Starts
			'ListUserListValuesAPI' => array(
				'datasource' => array(
					array('input'=>array('code'=>'cre_usr_dpt','userid'=>1),
						  'output'=>array('id','name','code'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('code'=>'cre_usr_etp','userid'=>1),
							'output'=>array('id','name','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('code'=>'cre_usr_jrl','userid'=>1),
							'output'=>array('id','name','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('code'=>'cre_usr_jtl','userid'=>1),
							'output'=>array('id','name','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('code'=>'cre_usr_ptp','userid'=>1),
							'output'=>array('id','name','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
				),
				'validate' => array(
 					array('input'=>array('code'=>'','userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
				
				)
			),  // ListUserListValuesAPI test case Ends
			
		); // End of api test case main array
?>