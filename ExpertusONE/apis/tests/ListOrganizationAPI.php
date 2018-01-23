<?php 
$orgs = db_query("select id from slt_organization")->fetchAll();
$count=count($orgs);
expDebug::dPrint("joola ".print_r($count));
expDebug::dPrint("joola1 ".print_r($orgs,true),1);
$testCases = array(

			// List Organization API test case Starts
			'ListOrganizationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('start','limit','userid'),
								'result'=>array('<results totalrecords =','<start>','<limit>','<userid>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('start','limit','userid'),
								'result'=>array('<results totalrecords =','<start>','<limit>','<userid>'),
								'output_type'=>'xml'
							),
				    array('input'=>array('userid'=>1,'start'=>''),
								'output'=>array('start','userid'),
								'result'=>array('<results totalrecords =','<start>','<userid>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('start','limit','userid','id','name','description','type','status','childorgs','lnrcount'),
								'result'=>array('<results totalrecords = ','<start>','<limit>','<userid>','<id>','<name>','<description>','<type>','<status>','<childorgs>','<lnrcount>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('start','limit','userid','id'),
								'result'=>array('<results totalrecords = "'.$count.'">','<start>','<limit>','<userid>','<id>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							)
				)
			),  // List Organization API test case Ends
			
		); // End of api test case main array
?>