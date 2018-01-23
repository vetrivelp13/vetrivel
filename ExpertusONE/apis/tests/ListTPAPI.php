<?php 
$tps = db_query("select id from slt_program")->fetchAll();
$count=count($tps);
$testCases = array(

			// ListTPAPI test case Starts
			'ListTPAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','code','object_type','short_desc','price','currency','currency_code','currency_symbol','status_code','status','lang'),
								'result'=>array('<results totalrecords =','<id>','<title>','<code>','<object_type>','<short_desc>','<price>','<currency>','<currency_code>','<currency_symbol>','<status_code>','<status>','<lang>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('id','title','code','object_type','short_desc','price','currency','currency_code','currency_symbol','status_code','status','lang'),
								'result'=>array('<results totalrecords =','<id>','<title>','<code>','<object_type>','<short_desc>','<price>','<currency>','<currency_code>','<currency_symbol>','<status_code>','<status>','<lang>'),
								'output_type'=>'xml'
							),
				    array('input'=>array('userid'=>1,'start'=>''),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','code','object_type'),
								'result'=>array('<results totalrecords = ','<id>','<title>','<code>','<object_type>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'limit'=>''),
								'output'=>array('id','title','code','object_type','status'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<title>','<code>','<object_type>','<status>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','code','status'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<code>','<status>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							)
				)
			),  // ListTPAPI test case Ends
			
		); // End of api test case main array
?>