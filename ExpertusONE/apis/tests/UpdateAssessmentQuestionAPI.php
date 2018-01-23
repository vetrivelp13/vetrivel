<?php
$id=db_query("select id from slt_survey_questions where survey_type='sry_det_typ_ass' order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$$id'.$id);

$survery_status='sry_qtn_sts_itv';
$lang_code='cre_sys_lng_eng';

$qn_type=db_query("select question_type from slt_survey_questions where id=$id")->fetchField();
expDebug::dPrint('$$$qn_type'.$qn_type);

$qn_txt=db_query("select question_txt from slt_survey_questions where id=$id")->fetchField();
expDebug::dPrint('$$$$qn_txt'.$qn_txt);

$answer_list=db_query("select answer_choice_txt from slt_survey_questions where id=$id")->fetchField();
$answer_list=str_replace('#','@',$answer_list);
expDebug::dPrint('$$$$$answer_list'.$answer_list);

$right_answer=db_query("select right_answer from slt_survey_questions where id=$id")->fetchField();
expDebug::dPrint('$$$$$answer_list'.$right_answer);

$testCases = array(
		'UpdateAssessmentQuestionAPI' => array(
				'datasource' => array(
						array('input'=>array('id'=>$id,'survey_status'=>$survery_status,'lang_code'=>$lang_code,'question_type'=>$qn_type,'question_txt'=>$qn_txt,'answers_list'=>$answer_list,'correct_answer'=>$right_answer,'userid'=>1),
						  'output'=>array('Id'),
						  'result'=>array('<results totalrecords = "">','<result>','<Id>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						      array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed.'),
								'output_type'=>'xml'
						        ),
						    array('input'=>array('id'=>$id,'survey_status'=>$survery_status,'lang_code'=>$lang_code,'question_type'=>$qn_type,'question_txt'=>$qn_txt,'answers_list'=>$answer_list,'correct_answer'=>0,'userid'=>1),
								'output'=>array(),
								'result'=>array('Entered Correct_answer not in answers_list'),
								'output_type'=>'xml'
						),
						 
		)
	)
);
?>