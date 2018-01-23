<?php
$survey_id = db_query("select survey_id from slt_survey_mapping order by id desc limit 0,1")->fetchField();
$object_id = db_query("select object_id from slt_survey_mapping where survey_id = $survey_id ")->fetchField();
$user_id = db_query("select id from slt_person where last_name not in ('Admin','User') order by id desc limit 0,1 ")->fetchField();
expDebug::dPrint('$object_id = = =  ' . $object_id);
$testCases = array(
		'GetSurveyAssessmentQuestionsAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>$user_id,'SurveyID'=>$survey_id),
						  'output'=>array('ID','Code','Title','QuestionPerPage','Attempt','Groups'),
						  'result'=>array('<results totalrecords = "',),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>$user_id,'SurveyID'=>$survey_id,'ObjectId'=>$object_id),
								'output'=>array('ID','Code','Title','QuestionPerPage','Attempt','Groups'),
								'result'=>array('<results totalrecords = "',),
								'output_type'=>'xml'
						),
						
				),
				'validate'=>array(

						array('input'=>array('userid'=>$user_id),
								'output'=>array(),
								'result'=>array('<results totalrecords = "0">'),
								'output_type'=>'xml'
						),

				)
			)
		);
?>