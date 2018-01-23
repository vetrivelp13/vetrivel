<?php
  function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();
//$random_no = rand(0,1000);
expDebug::dPrint(" Checking ".print_r($random_no));

//Data from BSG for Survey
$survey1 = db_query("select id from slt_survey where code = 'CERT-DSC' AND type ='sry_det_typ_sry' order by id desc limit 1")->fetchAll();
$survey2 = db_query("select id from slt_survey where code = 'DEMSUR-VC'AND type ='sry_det_typ_sry' order by id desc limit 1")->fetchAll();
$survey3 = db_query("select id from slt_survey where code = 'NEO' AND type ='sry_det_typ_sry' order by id desc limit 1")->fetchAll();
$survey4 = db_query("select id from slt_survey where code = 'DEMSUR' AND type ='sry_det_typ_sry' order by id desc limit 1")->fetchAll();
$survey5 = db_query("select id from slt_survey where code = 'VISDU' AND type ='sry_det_typ_sry' order by id desc limit 1")->fetchAll();
//Data from BSG for Assessment
$Assess1 = db_query("select id from slt_survey where code = 'CRSD-DEMO' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
$Assess2 = db_query("select id from slt_survey where code = 'ASST-R&A'AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
$Assess3 = db_query("select id from slt_survey where code = 'ASST-SC-001' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
$Assess4 = db_query("select id from slt_survey where code = 'ASST-SC' AND type ='sry_det_typ_ass' order by id desc limit 1")->fetchAll();
$testCases = array(

		'CreateSurveyGroupAPI' => array(
				'datasource' => array(
						array('input'=>array('survey_id'=>$survey1[0]->id,'survey_group'=>$random_word,'userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey1[0]->id,'survey_group'=>'Overall Experience','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey1[0]->id,'survey_group'=>'Delivery Method','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey1[0]->id,'survey_group'=>'Content','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey1[0]->id,'survey_group'=>'Knowledge','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey2[0]->id,'survey_group'=>'Level of Interest','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey2[0]->id,'survey_group'=>'Time Management','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey2[0]->id,'survey_group'=>'Training Material','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey2[0]->id,'survey_group'=>'Course Objectives','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey3[0]->id,'survey_group'=>'Satisfaction level','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey3[0]->id,'survey_group'=>'Presentation','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey3[0]->id,'survey_group'=>'Result','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey3[0]->id,'survey_group'=>'Employee Handbook','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey4[0]->id,'survey_group'=>'Time Utilization','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey4[0]->id,'survey_group'=>'Location','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey4[0]->id,'survey_group'=>'Instructor','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey5[0]->id,'survey_group'=>'Video Quality','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey5[0]->id,'survey_group'=>'Content','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$survey5[0]->id,'survey_group'=>'Content and Material','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess1[0]->id,'survey_group'=>'Network','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess1[0]->id,'survey_group'=>'General','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess1[0]->id,'survey_group'=>'Design','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess2[0]->id,'survey_group'=>'Acknowledge','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess3[0]->id,'survey_group'=>'Group','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess3[0]->id,'survey_group'=>'Content','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess3[0]->id,'survey_group'=>'Delivery Type','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess3[0]->id,'survey_group'=>'Team','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess3[0]->id,'survey_group'=>'Report','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess4[0]->id,'survey_group'=>'Followup','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess4[0]->id,'survey_group'=>'Content','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess4[0]->id,'survey_group'=>'Manager','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$Assess4[0]->id,'survey_group'=>'Assessment','userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
				),
				'validate' => array(
	               //all fields missing 
						array('input'=>array('survey_id'=>'','survey_group'=>'','userid'=>''),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
						),
					//field missing 	
						array('input'=>array('survey_id'=>'','survey_group'=>$random_word,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_id'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>35,'survey_group'=>'','userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','survey_group'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>35,'survey_group'=>$random_word,'userid'=>''),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
						),
						//Invalid data type for id
						array('input'=>array('survey_id'=>$random_word,'survey_group'=>$random_word,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Invalid survey Id','Invalid survey Id'),
								'output_type'=>'xml'
						),
						// wrong user ID 
						array('input'=>array('survey_id'=>35,'survey_group'=>$random_word,'userid'=>2),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Permission restricted','Don\'t have access admin API'),
								'output_type'=>'xml'
						),
						
						
  
        )
	)
 )
  
  
?>