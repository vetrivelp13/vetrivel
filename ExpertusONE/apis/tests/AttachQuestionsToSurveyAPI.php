<?php

   function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
   $random_word = getRandomWord();
   $random_num = rand(2,1000);
  // $surv = db_query("select id from slt_survey where type='sry_det_typ_sry' and status='sry_det_sry_atv' order by id DESC limit 1  ")->fetchField();
   $surv = db_query("select id from slt_survey where type='sry_det_typ_sry' order by id DESC limit 1  ")->fetchField();
   $ques = db_query("select id from slt_survey_questions where status='sry_qtn_sts_atv' order by id DESC limit 1  ")->fetchField();
   $ques_active_n_inactive = db_query("select id from slt_survey_questions order by id DESC limit 1  ")->fetchField();
   
   $grp = db_query("select id from slt_survey_groups where status='sry_det_grp_atv' order by id DESC limit 1  ")->fetchField();
   
   $count_surv = count($surv);
   $count_ques = count($ques);
   $count_grp = count($grp);
   
   expDebug::dPrint(" Checking ".print_r($surv));
   expDebug::dPrint(" Checking ".print_r($grp));
   expDebug::dPrint(" Checking ".print_r($ques));
   
/* ****BSG Data**** */
 //Survey ID
   $survey1 = db_query("select id from slt_survey where code = 'CERT-DSC' order by id desc limit 1")->fetchAll();
   $survey2 = db_query("select id from slt_survey where code = 'DEMSUR-VC' order by id desc limit 1")->fetchAll();
   $survey3 = db_query("select id from slt_survey where code = 'NEO' order by id desc limit 1")->fetchAll();
   $survey4 = db_query("select id from slt_survey where code = 'DEMSUR' order by id desc limit 1")->fetchAll();
   $survey5 = db_query("select id from slt_survey where code = 'VISDU' order by id desc limit 1")->fetchAll();
 //Survey Question ID  
   $surveyque1  = db_query("select id from slt_survey_questions where question_txt = 'Overall experience with this course' order by id desc limit 1")->fetchField();
   $surveyque2  = db_query("select id from slt_survey_questions where question_txt = 'The delivery method of this course (Classroom, Computer, Video) was effective way for me to learn this subject matter.' order by id desc limit 1")->fetchField();
   $surveyque3  = db_query("select id from slt_survey_questions where question_txt = 'How much would you rate the content?(Where 5 being the highest)' order by id desc limit 1")->fetchField();
   $surveyque4  = db_query("select id from slt_survey_questions where question_txt = 'The knowledge and/or skills gained through this course are directly applicable to my job' order by id desc limit 1")->fetchField();
   $surveyque5  = db_query("select id from slt_survey_questions where question_txt = 'Please rate your level of interest in the topic' order by id desc limit 1")->fetchField();
   $surveyque6  = db_query("select id from slt_survey_questions where question_txt = 'The instructor managed the class effectively' order by id desc limit 1")->fetchField();
   $surveyque7  = db_query("select id from slt_survey_questions where question_txt = 'Please rate the learning material used in this training and its ability to engage you.' order by id desc limit 1")->fetchField();
   $surveyque8  = db_query("select id from slt_survey_questions where question_txt = 'I clearly understood the course objectives' order by id desc limit 1")->fetchField();
   $surveyque9  = db_query("select id from slt_survey_questions where question_txt = 'How satisfied were you with Scheduling of the orientation' order by id desc limit 1")->fetchField();
   $surveyque10 = db_query("select id from slt_survey_questions where question_txt = 'Clarity and variety of presentations' order by id desc limit 1")->fetchField();
   $surveyque11 = db_query("select id from slt_survey_questions where question_txt = 'Clear and understandable presentation of benefits package' order by id desc limit 1")->fetchField();
   $surveyque12 = db_query("select id from slt_survey_questions where question_txt = 'Did you receive a copy of the employee handbook?' order by id desc limit 1")->fetchField();
   $surveyque13 = db_query("select id from slt_survey_questions where question_txt = 'In terms of time, was the average session' order by id desc limit 1")->fetchField();
   $surveyque14 = db_query("select id from slt_survey_questions where question_txt = 'Please rate the Location where the class was conducted' order by id desc limit 1")->fetchField();
   $surveyque15 = db_query("select id from slt_survey_questions where question_txt = 'Please rate the ability of the instructor to to effectively answer questions' order by id desc limit 1")->fetchField();
   $surveyque16 = db_query("select id from slt_survey_questions where question_txt = 'Please rate the quality of the video used' order by id desc limit 1")->fetchField();
   $surveyque17 = db_query("select id from slt_survey_questions where question_txt = 'How much would you rate the content?(Where 5 being the highest)' order by id desc limit 1")->fetchField();
   $surveyque18 = db_query("select id from slt_survey_questions where question_txt = 'Based only on the content and material, could you confidently recommend to other students?' order by id desc limit 1")->fetchField();
 //Survey Group ID
   $surveygrpid1  = db_query("select id from slt_survey_groups where code = 'Overall Experience' order by id desc limit 1")->fetchField();
   $surveygrpid2  = db_query("select id from slt_survey_groups where code = 'Delivery Method' order by id desc limit 1")->fetchField();
   $surveygrpid3  = db_query("select id from slt_survey_groups where code = 'Content' order by id desc limit 1")->fetchField();
   $surveygrpid4  = db_query("select id from slt_survey_groups where code = 'Knowledge' order by id desc limit 1")->fetchField();
   $surveygrpid5  = db_query("select id from slt_survey_groups where code = 'Level of Interest' order by id desc limit 1")->fetchField();
   $surveygrpid6  = db_query("select id from slt_survey_groups where code = 'Time Management' order by id desc limit 1")->fetchField();
   $surveygrpid7  = db_query("select id from slt_survey_groups where code = 'Training Material' order by id desc limit 1")->fetchField();
   $surveygrpid8  = db_query("select id from slt_survey_groups where code = 'Course Objectives' order by id desc limit 1")->fetchField();
   $surveygrpid9  = db_query("select id from slt_survey_groups where code = 'Satisfaction level' order by id desc limit 1")->fetchField();
   $surveygrpid10 = db_query("select id from slt_survey_groups where code = 'Presentation' order by id desc limit 1")->fetchField();
   $surveygrpid11 = db_query("select id from slt_survey_groups where code = 'Result' order by id desc limit 1")->fetchField();
   $surveygrpid12 = db_query("select id from slt_survey_groups where code = 'Employee Handbook' order by id desc limit 1")->fetchField();
   $surveygrpid13 = db_query("select id from slt_survey_groups where code = 'Time Utilization' order by id desc limit 1")->fetchField();
   $surveygrpid14 = db_query("select id from slt_survey_groups where code = 'Location' order by id desc limit 1")->fetchField();
   $surveygrpid15 = db_query("select id from slt_survey_groups where code = 'Instructor' order by id desc limit 1")->fetchField();
   $surveygrpid16 = db_query("select id from slt_survey_groups where code = 'Video Quality' order by id desc limit 1")->fetchField();
   $surveygrpid17 = db_query("select id from slt_survey_groups where code = 'Content' order by id desc limit 1")->fetchField();
   $surveygrpid18 = db_query("select id from slt_survey_groups where code = 'Content and Material' order by id desc limit 1")->fetchField();
   
   $testCases = array(
         
		//$count is 1 and so count-1 is 0. ques[count-1] is  the last updated value.
			'AttachQuestionsToSurveyAPI' => array(
				'datasource' => array(
					//array('input'=>array('userid'=>1,'survey_id'=>$surv[0]->id,'survey_group_id'=>$grp[0]->id,'survey_question_id'=>$ques[0]->id),
					//array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
				array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y'),
						 'output'=>array('Id'),
						 'result'=>array('<results totalrecords = ','<Id>'),
						 'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'survey_id'=>$survey1[0]->id,'survey_group_id'=>$surveygrpid1,'survey_question_id'=>$surveyque1,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey1[0]->id,'survey_group_id'=>$surveygrpid2,'survey_question_id'=>$surveyque2,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey1[0]->id,'survey_group_id'=>$surveygrpid3,'survey_question_id'=>$surveyque3,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey1[0]->id,'survey_group_id'=>$surveygrpid4,'survey_question_id'=>$surveyque4,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey2[0]->id,'survey_group_id'=>$surveygrpid5,'survey_question_id'=>$surveyque5,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey2[0]->id,'survey_group_id'=>$surveygrpid6,'survey_question_id'=>$surveyque6,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey2[0]->id,'survey_group_id'=>$surveygrpid7,'survey_question_id'=>$surveyque7,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey2[0]->id,'survey_group_id'=>$surveygrpid8,'survey_question_id'=>$surveyque8,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey3[0]->id,'survey_group_id'=>$surveygrpid9,'survey_question_id'=>$surveyque9,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey3[0]->id,'survey_group_id'=>$surveygrpid10,'survey_question_id'=>$surveyque10,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey3[0]->id,'survey_group_id'=>$surveygrpid11,'survey_question_id'=>$surveyque11,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey3[0]->id,'survey_group_id'=>$surveygrpid12,'survey_question_id'=>$surveyque12,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey4[0]->id,'survey_group_id'=>$surveygrpid13,'survey_question_id'=>$surveyque13,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey4[0]->id,'survey_group_id'=>$surveygrpid14,'survey_question_id'=>$surveyque14,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey4[0]->id,'survey_group_id'=>$surveygrpid15,'survey_question_id'=>$surveyque15,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey5[0]->id,'survey_group_id'=>$surveygrpid16,'survey_question_id'=>$surveyque16,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey5[0]->id,'survey_group_id'=>$surveygrpid17,'survey_question_id'=>$surveyque17,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$survey5[0]->id,'survey_group_id'=>$surveygrpid18,'survey_question_id'=>$surveyque18,'mandatoryOption'=>'N'),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						
				),
				
				'validate'=>array(
				//Field missing
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),	
							 
						array('input'=>array('userid'=>1,'survey_id'=>'','survey_group_id'=>$grp,'survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>'','survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_group_id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>''),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_question_id'),
							   'output_type'=>'xml'
							 ),
				//Invalid data type input
				        array('input'=>array('userid'=>1,'survey_id'=>$random_word,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Id','Invalid Survey Id'),
							   'output_type'=>'xml'
							 ),	 
						array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$random_word,'survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$random_word),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Question Id','Invalid Survey Question Id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>2,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Permission restricted','Don\'t have access admin API'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>'','survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','INVALID ACCESS','Userid is invalid'),
							   'output_type'=>'xml'
							 ),	 
							 
					//Invalid input
					 array('input'=>array('userid'=>1,'survey_id'=>$surv + 1,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Id','Invalid Survey Id'),
							   'output_type'=>'xml'
							 ),	 
					 array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp + 1,'survey_question_id'=>$ques),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
							   'output_type'=>'xml'
							 ),	 
					 array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=> $ques_active_n_inactive + 1),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Question Id','Invalid Survey Question Id'),
							   'output_type'=>'xml'
							 ),	 
				)
				
				
      )
	)
   
   
?>