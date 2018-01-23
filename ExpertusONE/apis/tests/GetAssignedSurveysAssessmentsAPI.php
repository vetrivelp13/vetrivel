<?php
$attachedsurvey = db_query("select enr.id as enrollid , enr.class_id as classid , ssm.id as id ,ssm.object_type as objtype
from slt_enrollment enr 
left join slt_survey_mapping ssm on  (ssm.object_id = enr.class_id)
LEFT OUTER JOIN slt_survey surv ON surv.id = ssm.survey_id
where enr.master_enrollment_id is null and reg_status = 'lrn_crs_reg_cnf' and ssm.object_type = 'cre_sys_obt_cls' and ssm.id is not null 
AND surv.type = 'sry_det_typ_sry'")->fetchAll();

$attachedassess =db_query("select enr.id as enrollid , enr.class_id as classid , ssm.id as id ,ssm.object_type as objtype
from slt_enrollment enr 
left join slt_survey_mapping ssm on  (ssm.object_id = enr.class_id)
LEFT OUTER JOIN slt_survey surv ON surv.id = ssm.survey_id
where enr.master_enrollment_id is null and reg_status = 'lrn_crs_reg_cnf' and ssm.object_type = 'cre_sys_obt_cls' and ssm.id is not null 
AND surv.type = 'sry_det_typ_ass'")->fetchAll();
expDebug::dPrint('getMaxAttemptssandhyaa '.print_r($attachedsurvey, true) , 4);
$testCases = array(
		'GetAssignedSurveysAssessmentsAPI' => array(
				'datasource' => array(
						 array('input'=>array('userid'=>1,'classid'=>$attachedsurvey[0]->classid,'surveytype'=>'survey','enrollId'=>$attachedsurvey[0]->enrollid),
						  'output'=>array('surveyid','surveytitle','suveycode','aftercompletion','objecttype','objectid','prestatus','surveycount','no_of_attempts','attempts','scoreConflict','presurveycount','preassCount'),
						  'result'=>array('<results totalrecords =','<result>','<surveyid>'),
						  'output_type'=>'xml'
						), 
						array('input'=>array('userid'=>1,'classid'=>$attachedassess[0]->classid,'surveytype'=>'assessment','enrollId'=>$attachedassess[0]->enrollid),
								'output'=>array('surveyid','surveytitle','suveycode','aftercompletion','objecttype','objectid','prestatus','surveycount','no_of_attempts','attempts','scoreConflict','presurveycount','preassCount'),
								'result'=>array('<results totalrecords =','<result>','<surveyid>'),
								'output_type'=>'xml'
						),
				),
				'validate' => array(
					//
				)
		), 
			
); 
?>
