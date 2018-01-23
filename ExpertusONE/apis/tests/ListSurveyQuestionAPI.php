<?php 
$type = "sry_det_typ_sry";
$ques = db_query("select id,survey_type from slt_survey_questions where survey_type =:type",array(":type"=>$type))->fetchAll();
$count=count($ques);
expDebug::dPrint("joola1 ".print_r($ques,true),1);
$testCases = array(

			// List Survey Questions API test case Starts
			'ListSurveyQuestionAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('id','question_txt','status','language','question_type'),
								'result'=>array('<results totalrecords =','<id>','<question_txt>','<status>','<language>','<question_type>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('id','question_txt','status','language','question_type'),
								'result'=>array('<results totalrecords =','<id>','<question_txt>','<status>','<language>','<question_type>'),
								'output_type'=>'xml'
							),
				    array('input'=>array('userid'=>1,'start'=>''),
								'output'=>array('id','question_txt','status'),
								'result'=>array('<results totalrecords =','<id>','<question_txt>','<status>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','status','language','question_type'),
								'result'=>array('<results totalrecords = ','<id>','<status>','<language>','<question_type>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'limit'=>''),
								'output'=>array('id','language','question_type'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<language>','<question_type>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'start'=>''),
								'output'=>array('id','question_txt'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<question_txt>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							)
				)
			),  // List Survey Questions API test case Ends
			
		); // End of api test case main array
?>