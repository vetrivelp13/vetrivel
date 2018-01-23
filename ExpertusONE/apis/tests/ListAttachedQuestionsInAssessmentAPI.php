<?php 
$type = "sry_det_typ_ass";
$id = db_query("select id from slt_survey where type =:type order by id DESC limit 1",array(":type"=>$type))->fetchAll();
expDebug::dPrint("joola ".print_r($id,true),1);
$quescount = db_query("SELECT COUNT(surveygroup.id) AS count FROM 
slt_survey_groups_questions surveygroup
LEFT OUTER JOIN slt_survey_questions surveyquestion ON surveygroup.question_id=surveyquestion.id
WHERE  (surveygroup.survey_id = :id) AND (surveyquestion.survey_type = :type) AND (surveyquestion.status = 'sry_qtn_sts_atv') ",array(":type"=>$type,":id"=>$id[0]->id))->fetchAll();
expDebug::dPrint("joola1 ".print_r($quescount,true),1);
$testCases = array(

			// ListAttachedQuestionsInAssessmentAPI test case Starts
			'ListAttachedQuestionsInAssessmentAPI' => array(
				'datasource' => array(
					array('input'=>array('survey_id'=>$id[0]->id,'userid'=>1),
								'output'=>array('survey_group_question_id','question_txt','survey_question_id','question_status','question_type','survey_group_title','survey_group_sequence'),
								'result'=>array('<results totalrecords =','<survey_group_question_id>','<question_txt>','<survey_question_id>','<question_status>','<question_type>','<survey_group_title>','<survey_group_sequence>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('survey_id'=>$id[0]->id,'userid'=>1),
								'output'=>array('survey_group_question_id','question_txt','survey_question_id','question_status','question_type','survey_group_title','survey_group_sequence'),
								'result'=>array('<results totalrecords =','<survey_group_question_id>','<question_txt>','<survey_question_id>','<question_status>','<question_type>','<survey_group_title>','<survey_group_sequence>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
					array('input'=>array('survey_id'=>'','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','<short_msg>'."Mandatory Fields are missing in the feed. List provided below:".'</short_msg>'),
								'output_type'=>'xml'
							),
					array('input'=>array('survey_id'=>'ABC','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','<short_msg>'."Invalid Survey id".'</short_msg>'),
								'output_type'=>'xml'
							),
					array('input'=>array('survey_id'=>$id[0]->id,'userid'=>1),
								'output'=>array('survey_group_question_id','question_txt','survey_question_id','question_status'),
								'result'=>array('<results totalrecords = "'.$quescount[0]->count.'">','<survey_group_question_id>','<question_txt>','<survey_question_id>','<question_status>'),
								'output_type'=>'xml'
							),
				)
			),  // ListAttachedQuestionsInAssessmentAPI  test case Ends
			
		); // End of api test case main array
?>