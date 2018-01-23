<?php
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();

$testCases = array(

		'CreateSurveyAPI' => array(
				'datasource' => array(
						array('input'=>array(
								'survey_title'=>$random_word,
								'survey_code'=>$random_word,
								'value'=>"try",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>4,
								'userid'=>1
								
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Survey - WBT',
								'survey_code'=>'CERT-DSC',
								'value'=>"SURVEY - WBT",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'userid'=>1
						
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Survey - VC',
								'survey_code'=>'DEMSUR-VC',
								'value'=>"Survey - VC",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'userid'=>1
						
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Onboarding Survey',
								'survey_code'=>'NEO',
								'value'=>"Onboarding Survey",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'userid'=>1
						
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Survey-ILT',
								'survey_code'=>'DEMSUR',
								'value'=>"Survey-ILT",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'userid'=>1
						
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Survey-VOD',
								'survey_code'=>'VISDU',
								'value'=>"Survey-VOD",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'userid'=>1
						
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						)
				),
			
	'validate' => array(
	//all fields missing (Simulator gives SQL Injection error, which is not correct)
						array('input'=>array('userid'=>1),
								'output'=>array('Id'),
								'result'=>array('<error_code>XSS_SQL_INJECTION_ERROR</error_code>'),
								'output_type'=>'xml'
						),
    // one field missing
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_status'=>"sry_det_sry_atv",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
					   array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','value'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'value'=>"try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_code'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_code'=>$random_word,'value'=>"try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_title'),
								'output_type'=>'xml'
						),
		//two fields missing				
						
					array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_status'=>"sry_det_sry_atv",'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"try",'survey_language'=>"cre_sys_lng_eng",'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','survey_status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','value'),
								'output_type'=>'xml'
						),
				)
		),		
			
);
?>