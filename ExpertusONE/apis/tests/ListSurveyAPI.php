<?php
$surv = db_query("select id from slt_survey")->fetchAll();
$count=count($surv);
expDebug::dPrint(" Checking ".print_r($count));
$testCases = array(

			
			'ListSurveyAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('<results totalrecords =','<id>','<title>','<code>','<type>','<short_description>','<status>','<language>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('jsonResponse','id','title','code','type','short_description','status','language'),
								'output_type'=>'json'
							),
				),
				
				'validate' => array(
					array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('<results totalrecords =','<id>','<title>','<code>','<type>','<short_description>','<status>','<language>'),
								'output_type'=>'xml'
							),
				    array('input'=>array('userid'=>'','start'=>'','limit'=>''),
								//'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>'2','start'=>'','limit'=>''),
								//'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('<error>','<error_code>'."L_008".'</error_code>','Permission restricted','Permission restricted'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('id','title','code','type','short_description','status','language'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<title>','<code>','<type>','<short_description>','<status>','<language>'),
								'output_type'=>'xml'
							),
									
							
							
				)
				
				
				
				
			)
		)
   
   
   
   
   
   
   
   
   
   
   
   
   
   
?>