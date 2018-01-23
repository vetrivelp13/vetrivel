<?php

//expDebug::dPrint(" Checking ".print_r($count));
$testCases = array(

			
			'getJobDetailsAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jtl",'type'=>"count"),
								'output'=>array('id','code','name'),
								'result'=>array('<results totalrecords =','<id>','<code>','<name>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jtl",'type'=>"count"),
								'output'=>array('id','code','name'),
								'result'=>array('jsonResponse','id','code','name'),
								'output_type'=>'json'
							),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jtl",'type'=>"list"),
								'output'=>array('id','code','name'),
								'result'=>array('<results totalrecords =','<id>','<code>','<name>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jtl",'type'=>"list"),
								'output'=>array('id','code','name'),
								'result'=>array('jsonResponse','id','code','name'),
								'output_type'=>'json'
						),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jrl",'type'=>"count"),
								'output'=>array('id','code','name'),
								'result'=>array('<results totalrecords =','<id>','<code>','<name>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jrl",'type'=>"count"),
								'output'=>array('id','code','name'),
								'result'=>array('jsonResponse','id','code','name'),
								'output_type'=>'json'
						),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jrl",'type'=>"list"),
								'output'=>array('id','code','name'),
								'result'=>array('<results totalrecords =','<id>','<code>','<name>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jrl",'type'=>"list"),
								'output'=>array('id','code','name'),
								'result'=>array('jsonResponse','id','code','name'),
								'output_type'=>'json'
						),
				),
					
					'validate' => array(
							array('input'=>array('userid'=>1,'entitytype'=>"",'type'=>"count"),
									'output'=>array(),
									'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','entitytype'),
									'output_type'=>'xml'
							),
							array('input'=>array('userid'=>1,'entitytype'=>"",'type'=>"list"),
									'output'=>array(),
									'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','entitytype'),
									'output_type'=>'xml'
							),
							array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jtl",'type'=>""),
									'output'=>array(),
									'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','type'),
									'output_type'=>'xml'
							),
							array('input'=>array('userid'=>1,'entitytype'=>"cre_usr_jrl",'type'=>""),
									'output'=>array(),
									'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','type'),
									'output_type'=>'xml'
							),
			)
		)
	)			
					
?>