<?php
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();
$random_num = rand(2,1000);
//$surv = db_query("select id from slt_survey where type='sry_det_typ_sry' and status='sry_det_sry_atv' order by id DESC limit 1  ")->fetchField();
//$ques = db_query("select id from slt_survey_questions where status='sry_qtn_sts_atv' order by id DESC limit 1  ")->fetchField();
$ques= db_query("select id from slt_survey_questions where status='sry_qtn_sts_atv' order by id DESC limit 1  ")->fetchField();
 
$grp = db_query("select id from slt_survey_groups where status='sry_det_grp_atv' order by id DESC limit 1  ")->fetchField();
 
$count_surv = count($surv);
$count_ques = count($ques);
$count_grp = count($grp);
 
//expDebug::dPrint(" Checking ".print_r($surv));
expDebug::dPrint(" Checking ".print_r($grp));
expDebug::dPrint(" Checking ".print_r($ques));
$testCases = array(
		 
		
		'UpdateAttachedQuestionsInSurveyAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'survey_group_id'=>$grp,'group_question_id'=>$ques,'mandatoryOption'=>'Y'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						)
					),
				
				 'validate'=>array(
						//Field missing
						array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
				
						array('input'=>array('userid'=>1,'survey_group_id'=>'','group_question_id'=>$ques,'mandatoryOption'=>'Y'),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
								'output_type'=>'xml'
						),
				 		array('input'=>array('userid'=>1,'survey_group_id'=>$grp,'group_question_id'=>'','mandatoryOption'=>'Y'),
				 				'output'=>array(),
				 				'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','group_question_id'),
				 				'output_type'=>'xml'
				 		),
				 		array('input'=>array('userid'=>1,'survey_group_id'=>$grp,'group_question_id'=>$ques,'mandatoryOption'=>''),
				 				'output'=>array(),
				 				'result'=>array('<error>','<error_code>','Mandatory Option must be Y or N','Mandatory Option must be Y or N'),
				 				'output_type'=>'xml'
				 		),
				 //Invalid data type input
				 		array('input'=>array('userid'=>1,'survey_group_id'=>$grp,'group_question_id'=>$random_word,'mandatoryOption'=>'Y'),
				 				'output'=>array(),
				 				'result'=>array('<error>','<error_code>','Invalid Survey Group Question Id','Invalid Survey Group Question Id'),
				 				'output_type'=>'xml'
				 		),
				 		array('input'=>array('userid'=>1,'survey_group_id'=>$random_word,'group_question_id'=>$ques,'mandatoryOption'=>'Y'),
				 				'output'=>array(),
				 				'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
				 				'output_type'=>'xml'
				 		),
				 		array('input'=>array('userid'=>1,'survey_group_id'=>$grp,'group_question_id'=>$ques,'mandatoryOption'=>$random_word),
				 				'output'=>array(),
				 				'result'=>array('<error>','<error_code>','Mandatory Option must be Y or N','Mandatory Option must be Y or N'),
				 				'output_type'=>'xml'
				 		),
				 		
				)
		)
  )
?>