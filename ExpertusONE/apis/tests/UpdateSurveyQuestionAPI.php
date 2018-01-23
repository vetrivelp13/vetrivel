<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $surv_ques = db_query("select * from slt_survey_questions where survey_type='sry_det_typ_sry' order by id DESC limit 1  ")->fetchAll();
	$count=count($surv_ques);
	expDebug::dPrint(" Checking ".print_r($count));

 $testCases = array(
			'UpdateSurveyQuestionAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
							)
				),
					
				'validate' => array(
					array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						   ),
					array('input'=>array('userid'=>1,'id'=>'','survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
						 'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','id'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>'','lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_status'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>$surv_ques[0]->status,'lang_code'=>'','question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','lang_code'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>'','question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','question_type'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>'','answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','question_txt'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','answers_list'),
								'output_type'=>'xml'
						),
				//Invalid data type		
						array('input'=>array('userid'=>1,'id'=>$random_word,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','Invalid id','Invalid id'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$random_word,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','Invalid id','Invalid id'),
								'output_type'=>'xml'
						),
				//ID does not exist. One more than the latest updated ID value.
						array('input'=>array('userid'=>1,'id'=>$surv_ques[0]->id + 1,'survey_status'=>$surv_ques[0]->status,'lang_code'=>$surv_ques[0]->lang_code,'question_type'=>$surv_ques[0]->question_type,'question_txt'=>$surv_ques[0]->question_txt,'answers_list'=>$surv_ques[0]->answer_choice_txt),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','Invalid id','Invalid id'),
								   'output_type'=>'xml'
						),
			
			)
		)
	)
	
 ?>
		