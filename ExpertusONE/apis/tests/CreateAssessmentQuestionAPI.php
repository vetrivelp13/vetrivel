<?php
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$survey_status='sry_qtn_sts_atv';
$lang_code='cre_sys_lng_eng';
$question_yno='sry_qtn_typ_yno';
$answer_yno='Yes';
$answer_list='Yes@@No@@';

$testCases = array(
		'CreateAssessmentQuestionAPI' => array(
				'datasource' => array(
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>$question_yno,
							'question_txt'=>$random,
							'answers_list'=>$answer_list,
							'correct_answer'=>$answer_yno
							
					),
						  'output'=>array('Id'),
						  'result'=>array('<Id>'),
						  'output_type'=>'xml'
				 	),
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>'sry_qtn_typ_dpn',
							'question_txt'=>'Select the correct IP Address?',
							'answers_list'=>'192.166.6.1@@192.166.6.2@@192.166.6.3@@192.166.6.4',
							'correct_answer'=>'192.166.6.2'
									
						),
							'output'=>array('Id'),
							'result'=>array('<Id>'),
							'output_type'=>'xml'
						),
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>'sry_qtn_typ_mch',
							'question_txt'=>'Which designs were shown during the training?',
							'answers_list'=>'Cloud@@Installed@@Web service',
							'correct_answer'=>'Cloud@@Web Service'
					
						),
							'output'=>array('Id'),
							'result'=>array('<Id>'),
							'output_type'=>'xml'
						),
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>'sry_qtn_typ_trf',
							'question_txt'=>'Was the customer satisfied with the answer provided by the sales person?',
							'answers_list'=>'True@@False',
							'correct_answer'=>'True'
			
						),
							'output'=>array('Id'),
							'result'=>array('<Id>'),
							'output_type'=>'xml'
						),
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>'sry_qtn_typ_trf',
							'question_txt'=>'There are 2 designs covered during this course',
							'answers_list'=>'True@@False',
							'correct_answer'=>'True'
		
						),
							'output'=>array('Id'),
							'result'=>array('<Id>'),
							'output_type'=>'xml'
						),
					array('input'=>array(
							'userid'=>1,
							'survey_status'=>$survey_status,
							'lang_code'=>$lang_code,
							'question_type'=>'sry_qtn_typ_yno',
							'question_txt'=>'Do you agree that you have understood the material covered during this training?',
							'answers_list'=>'Yes@@No',
							'correct_answer'=>'Yes'
						
						),
							'output'=>array('Id'),
							'result'=>array('<Id>'),
							'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_yno',
								'question_txt'=>'Do you agree that you have understood the material covered during this training?',
								'answers_list'=>'Yes@@No',
								'correct_answer'=>'Yes'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_mch',
								'question_txt'=>'What are the types of Groups available in ExpertusONE?',
								'answers_list'=>'Learner@@Manager@@Administrator@@Instructor',
								'correct_answer'=>'Learner@@Administrator'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_yno',
								'question_txt'=>'Does ExpertusONE support SCORM 1.2?',
								'answers_list'=>'Yes@@No',
								'correct_answer'=>'Yes'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_mch',
								'question_txt'=>'What are the delivery types supported in ExpertusONE?',
								'answers_list'=>'Classroon@@Virtual Class@@Video@@Web-Based@@OJT',
								'correct_answer'=>'Classroon@@Virtual Class@@Video@@Web-Based'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_dpn',
								'question_txt'=>'Team tab is associated with?',
								'answers_list'=>'Instructor@@Manager@@Learner',
								'correct_answer'=>'Manager'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_trf',
								'question_txt'=>'Learners have access to assigned Reports',
								'answers_list'=>'True@@False',
								'correct_answer'=>'True'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_mch',
								'question_txt'=>'How new users are created in ExpertusONE?',
								'answers_list'=>'Manual@@HR Feed@@API Integration@@Cronjob',
								'correct_answer'=>'Manual@@HR Feed@@API Integration'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_trf',
								'question_txt'=>'ExpertusONE support TinCan Content.',
								'answers_list'=>'True@@False',
								'correct_answer'=>'True'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_yno',
								'question_txt'=>'A manager can assign mandatory training to his direct and virtual report?',
								'answers_list'=>'Yes@@No',
								'correct_answer'=>'Yes'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'userid'=>1,
								'survey_status'=>$survey_status,
								'lang_code'=>$lang_code,
								'question_type'=>'sry_qtn_typ_dpn',
								'question_txt'=>'What type of question is not supported in ExpertusONE assessment',
								'answers_list'=>'Mutiple Choice@@Yes/No@@True/False@@Dropdown@@Timed question',
								'correct_answer'=>'Timed question'
						
						),
								'output'=>array('Id'),
								'result'=>array('<Id>'),
								'output_type'=>'xml'
						),
						
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_status'=>$survey_status,'lang_code'=>cre_sys_lng_eng,'question_type'=>'aa','question_txt'=>$random,'answers_list'=>$answer_list,'correct_answer'=>$answer_yno),
						  'output'=>array(),
						  'result'=>array('Invalid question_type entered.'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'survey_status'=>$survey_status,'lang_code'=>cre_sys_lng_eng,'question_type'=>$question_yno,'question_txt'=>$random,'answers_list'=>$answer_list,'correct_answer'=>'aa'),
						  'output'=>array('Id'),
						  'result'=>array('correct_answer must be in Yes or NO'),
						   'output_type'=>'xml'
							),
							 
				)
			)
		);
			
?>		