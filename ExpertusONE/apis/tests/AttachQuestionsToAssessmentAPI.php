 <?php
 function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
   $random_word = getRandomWord();
   $random_num = rand(2,1000);
  // $asses = db_query("select id from slt_survey where type='sry_det_typ_ass' and status='sry_det_sry_atv' order by id DESC limit 1  ")->fetchField();
   $asses = db_query("select id from slt_survey where type='sry_det_typ_ass' order by id DESC limit 1  ")->fetchField();
    
   // The question ID should not be repeated for the same Survey_id {but this query is not effective for that}
   //$ques = db_query("select id from slt_survey_questions where status='sry_qtn_sts_atv' and id not in(select survey_group_id from slt_survey_groups_questions where survey_id = :asses) order by id DESC ",array(':asses'=> $asses))->fetchField();
  $ques = db_query("select id from slt_survey_questions where status='sry_qtn_sts_atv' order by id DESC limit 1  ")->fetchField();
  
   $ques_active_n_inactive = db_query("select id from slt_survey_questions order by id DESC limit 1  ")->fetchField();
   $grp = db_query("select id from slt_survey_groups where status='sry_det_grp_atv' order by id DESC limit 1  ")->fetchField();
   
   $count_surv = count($asses);
   $count_ques = count($ques);
   $count_grp = count($grp);
   
   expDebug::dPrint(" Checking ".print_r($asses));
   expDebug::dPrint(" Checking ".print_r($grp));
   expDebug::dPrint(" Checking ".print_r($ques));
   
   /* ****BSG Data**** */
   //Assessment ID
   $Assess1 = db_query("select id from slt_survey where code = 'CRSD-DEMO' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
   $Assess2 = db_query("select id from slt_survey where code = 'ASST-R&A'AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
   $Assess3 = db_query("select id from slt_survey where code = 'ASST-SC-001' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
   $Assess4 = db_query("select id from slt_survey where code = 'ASST-SC' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
   
   //Assessment Question ID
   $Assessque1  = db_query("select id from slt_survey_questions where question_txt = 'Select the correct IP Address?' order by id desc limit 1")->fetchField();
   $Assessque2  = db_query("select id from slt_survey_questions where question_txt = 'Which designs were shown during the training?' order by id desc limit 1")->fetchField();
   $Assessque3  = db_query("select id from slt_survey_questions where question_txt = 'Was the customer satisfied with the answer provided by the sales person?' order by id desc limit 1")->fetchField();
   $Assessque4  = db_query("select id from slt_survey_questions where question_txt = 'There are 2 designs covered during this course' order by id desc limit 1")->fetchField();
   $Assessque5  = db_query("select id from slt_survey_questions where question_txt = 'Do you agree that you have understood the material covered during this training?' order by id desc limit 1")->fetchField();
  // $Assessque6  = db_query("select id from slt_survey_questions where question_txt = 'Do you agree that you have understood the material covered during this training?' order by id desc limit 1")->fetchField();
   $Assessque7  = db_query("select id from slt_survey_questions where question_txt = 'What are the types of Groups available in ExpertusONE?' order by id desc limit 1")->fetchField();
   $Assessque8  = db_query("select id from slt_survey_questions where question_txt = 'Does ExpertusONE support SCORM 1.2?' order by id desc limit 1")->fetchField();
   $Assessque9  = db_query("select id from slt_survey_questions where question_txt = 'What are the delivery types supported in ExpertusONE?' order by id desc limit 1")->fetchField();
   $Assessque10 = db_query("select id from slt_survey_questions where question_txt = 'Team tab is associated with?' order by id desc limit 1")->fetchField();
   $Assessque11 = db_query("select id from slt_survey_questions where question_txt = 'Learners have access to assigned Reports' order by id desc limit 1")->fetchField();
   $Assessque12 = db_query("select id from slt_survey_questions where question_txt = 'How new users are created in ExpertusONE?' order by id desc limit 1")->fetchField();
   $Assessque13 = db_query("select id from slt_survey_questions where question_txt = 'ExpertusONE support TinCan Content.' order by id desc limit 1")->fetchField();
   $Assessque14 = db_query("select id from slt_survey_questions where question_txt = 'A manager can assign mandatory training to his direct and virtual report?' order by id desc limit 1")->fetchField();
   $Assessque15 = db_query("select id from slt_survey_questions where question_txt = 'What type of question is not supported in ExpertusONE assessment' order by id desc limit 1")->fetchField();
   
   //Assessment Group ID
   $Assessgrpid1  = db_query("select id from slt_survey_groups where code = 'Network' order by id desc limit 1")->fetchField();
   $Assessgrpid2  = db_query("select id from slt_survey_groups where code = 'General' order by id desc limit 1")->fetchField();
   $Assessgrpid3  = db_query("select id from slt_survey_groups where code = 'Design' order by id desc limit 1")->fetchField();
   $Assessgrpid4  = db_query("select id from slt_survey_groups where code = 'Course Material' order by id desc limit 1")->fetchField();
   $Assessgrpid5  = db_query("select id from slt_survey_groups where code = 'Group' order by id desc limit 1")->fetchField();
   $Assessgrpid6  = db_query("select id from slt_survey_groups where code = 'Content' order by id desc limit 1")->fetchField();
   $Assessgrpid7  = db_query("select id from slt_survey_groups where code = 'Delivery Type' order by id desc limit 1")->fetchField();
   $Assessgrpid8  = db_query("select id from slt_survey_groups where code = 'Team' order by id desc limit 1")->fetchField();
   $Assessgrpid9  = db_query("select id from slt_survey_groups where code = 'Report' order by id desc limit 1")->fetchField();
   $Assessgrpid10 = db_query("select id from slt_survey_groups where code = 'Followup' order by id desc limit 1")->fetchField();
  // $Assessgrpid11 = db_query("select id from slt_survey_groups where code = 'Content' order by id desc limit 1")->fetchField();
   $Assessgrpid12 = db_query("select id from slt_survey_groups where code = 'Manager' order by id desc limit 1")->fetchField();
   $Assessgrpid13 = db_query("select id from slt_survey_groups where code = 'Assessment' order by id desc limit 1")->fetchField();
   
   $testCases = array(
         
		//$count is 1 and so count-1 is 0. ques[count-1] is  the last updated value.
			'AttachQuestionsToAssessmentAPI' => array(
			// because same question is being attached again to the same survey Id, the datasource test case is getting failed.
				'datasource' => array(
					//array('input'=>array('userid'=>1,'survey_id'=>$surv[0]->id,'survey_group_id'=>$grp[0]->id,'survey_question_id'=>$ques[0]->id),
					//array('input'=>array('userid'=>1,'survey_id'=>$surv,'survey_group_id'=>$grp,'survey_question_id'=>$ques),
				array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						 'output'=>array('Id'),
						 'result'=>array('<results totalrecords = ','<Id>'),
						 'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess1[0]->id,'survey_group_id'=>$Assessgrpid1,'survey_question_id'=>$Assessque1,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>20),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess1[0]->id,'survey_group_id'=>$Assessgrpid1,'survey_question_id'=>$Assessque2,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>20),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess1[0]->id,'survey_group_id'=>$Assessgrpid2,'survey_question_id'=>$Assessque3,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>20),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess1[0]->id,'survey_group_id'=>$Assessgrpid3,'survey_question_id'=>$Assessque4,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>20),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess1[0]->id,'survey_group_id'=>$Assessgrpid2,'survey_question_id'=>$Assessque5,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>20),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess2[0]->id,'survey_group_id'=>$Assessgrpid4,'survey_question_id'=>$Assessque5,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>100),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess3[0]->id,'survey_group_id'=>$Assessgrpid5,'survey_question_id'=>$Assessque7,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess3[0]->id,'survey_group_id'=>$Assessgrpid6,'survey_question_id'=>$Assessque8,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess3[0]->id,'survey_group_id'=>$Assessgrpid7,'survey_question_id'=>$Assessque9,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess3[0]->id,'survey_group_id'=>$Assessgrpid8,'survey_question_id'=>$Assessque10,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess3[0]->id,'survey_group_id'=>$Assessgrpid9,'survey_question_id'=>$Assessque11,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess4[0]->id,'survey_group_id'=>$Assessgrpid10,'survey_question_id'=>$Assessque12,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess4[0]->id,'survey_group_id'=>$Assessgrpid6,'survey_question_id'=>$Assessque13,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess4[0]->id,'survey_group_id'=>$Assessgrpid12,'survey_question_id'=>$Assessque14,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords = ','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$Assess4[0]->id,'survey_group_id'=>$Assessgrpid13,'survey_question_id'=>$Assessque15,'mandatoryOption'=>'N','sequence'=>1,'survey_score'=>25),
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
							 
						array('input'=>array('userid'=>1,'survey_id'=>'','survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>'','survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_group_id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>'','mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_question_id'),
							   'output_type'=>'xml'
							 ),
				        array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>'','survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','sequence'),
							   'output_type'=>'xml'
							 ),
					     array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>''),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_score'),
							   'output_type'=>'xml'
							 ),
						
						
						
				//Invalid data type input
				        array('input'=>array('userid'=>1,'survey_id'=>$random_word,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Id','Invalid Survey Id'),
							   'output_type'=>'xml'
							 ),	 
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$random_word,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$random_word,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Question Id','Invalid Survey Question Id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>2,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Permission restricted','Don\'t have access admin API'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>'','survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','INVALID ACCESS','Userid is invalid'),
							   'output_type'=>'xml'
							 ),
						// because same question is being attached again to the same survey Id, these two test cases fail.	 	
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>$random_word,'survey_score'=>25),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Survey sequence must be greater than 0 or Numeric','Survey sequence must be greater than 0 or Numeric'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>$random_word),
						       'output'=>array(),
							   'result'=>array('<error>','<error_code>','Survey score must be greater than 0 or Numeric','Survey score must be greater than 0 or Numeric'),
							   'output_type'=>'xml'
							 ),	 
					//Invalid input
					 array('input'=>array('userid'=>1,'survey_id'=>$asses + 1,'survey_group_id'=>$grp,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Id','Invalid Survey Id'),
							   'output_type'=>'xml'
							 ),	 
					 array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp + 1,'survey_question_id'=>$ques,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Group Id','Invalid Survey Group Id'),
							   'output_type'=>'xml'
							 ),	 
					 array('input'=>array('userid'=>1,'survey_id'=>$asses,'survey_group_id'=>$grp,'survey_question_id'=> $ques_active_n_inactive + 1,'mandatoryOption'=>'Y','sequence'=>1,'survey_score'=>25),
						'output'=>array(),
							   'result'=>array('<error>','<error_code>','Invalid Survey Question Id','Invalid Survey Question Id'),
							   'output_type'=>'xml'
							 ),	 		  
							 
				
				
				
				
			)
		)
	)
?>