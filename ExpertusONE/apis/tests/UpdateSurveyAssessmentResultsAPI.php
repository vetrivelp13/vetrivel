<?php 

$details = db_query("
select enr.id as enrid , enr.user_id as userid,
enr.class_id as classid,smap.survey_id , ques.id as questionid , ques.question_type,ques.question_txt  from slt_enrollment enr
INNER JOIN slt_survey_mapping smap on  (smap.object_id = enr.class_id and smap.object_type = 'cre_sys_obt_cls')
LEFT JOIN slt_survey_groups_questions sg ON  (sg.survey_id = smap.survey_id)
INNER JOIN slt_survey_questions ques ON (ques.id = sg.question_id)  
order by enr.id desc")->fetchAll();
expDebug::dPrint("site lalala ".print_r($details,true),4);

$anwers = $details[0]->survey_id."~^".$details[0]->questionid."~^".$details[0]->question_type."~^".$details[0]->question_txt."~^True" ; 

$testCases = array(
		'UpdateSurveyAssessmentResultsAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>$details[0]->userid,'type'=>'survey','enrollId' => $details[0]->enrid,'prestatus'=> '', 'ObjectId' => $details[0]->classid , 'ObjectType' => 'cre_sys_obt_cls', 'answers' =>$anwers),
								'output'=>array('status','score','minmark','maxmark','completionstatus','completion_percentage'),
								'result'=>array('<results totalrecords =','<status>','<score>','<minmark>','<maxmark>','<completionstatus>','<completion_percentage>'),
								'output_type'=>'xml'
						)
						),
				'validate' => array(
						array('input'=>array('userid'=>$details[1]->userid,'type'=>'','enrollId' => '','prestatus'=> '', 'ObjectId' => '' , 'ObjectType' => '', 'answers' => ''),
								'output'=>array('status','score','minmark','maxmark','completionstatus','completion_percentage'),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>$details[1]->userid,'type'=>'survey','enrollId' => '','prestatus'=> '', 'ObjectId' => '' , 'ObjectType' => '', 'answers' => ''),
								'output'=>array('status','score','minmark','maxmark','completionstatus','completion_percentage'),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>$details[1]->userid,'type'=>'survey','enrollId' => $details[1]->enrid,'prestatus'=> '', 'ObjectId' => '' , 'ObjectType' => '', 'answers' => ''),
								'output'=>array('status','score','minmark','maxmark','completionstatus','completion_percentage'),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>$details[1]->userid,'type'=>'survey','enrollId' => $details[1]->enrid,'prestatus'=> '', 'ObjectId' => $details[0]->classid  , 'ObjectType' => '', 'answers' => ''),
								'output'=>array('status','score','minmark','maxmark','completionstatus','completion_percentage'),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						)
				),
		);
?>