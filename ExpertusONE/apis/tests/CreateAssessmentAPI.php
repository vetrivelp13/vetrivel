<?php
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();
  $testCases = array(

		'CreateAssessmentAPI' => array(
				'datasource' => array(
						array('input'=>array(
								'survey_title'=>$random_word,
								'survey_code'=>$random_word,
								'value'=>"Just try",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>4,
								'min_marks'=>10,
								'max_marks'=> 100,
								'userid'=>1,
								
						),
								'output'=>array('Id','msg'),
								'result'=>array('<results totalrecords =','<Id>','<msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Networking Assessment',
								'survey_code'=>'CRSD-DEMO',
								'value'=>"Demo Assessment",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>3,
								'min_marks'=>60,
								'max_marks'=> 100,
								'userid'=>1,
						
						),
								'output'=>array('Id','msg'),
								'result'=>array('<results totalrecords =','<Id>','<msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Read & Acknowledgement',
								'survey_code'=>'ASST-R&A',
								'value'=>"Read & Acknowledgement Assessment",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>1,
								'min_marks'=>1,
								'max_marks'=> 100,
								'userid'=>1,
						
						),
								'output'=>array('Id','msg'),
								'result'=>array('<results totalrecords =','<Id>','<msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Post - Assessment',
								'survey_code'=>'ASST-SC-001',
								'value'=>"Post Assessment",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'min_marks'=>50,
								'max_marks'=> 125,
								'userid'=>1,
						
						),
								'output'=>array('Id','msg'),
								'result'=>array('<results totalrecords =','<Id>','<msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'survey_title'=>'Pre - Assessment',
								'survey_code'=>'ASST-SC',
								'value'=>"Pre - Assessment",
								'survey_status'=>"sry_det_sry_atv",
								'survey_language'=>"cre_sys_lng_eng",
								'survey_qust_per_page'=>2,
								'min_marks'=>50,
								'max_marks'=> 100,
								'userid'=>1,
						
						),
								'output'=>array('Id','msg'),
								'result'=>array('<results totalrecords =','<Id>','<msg>'),
								'output_type'=>'xml'
						),
				),
				
			    'validate' => array(
			    
				        array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'max_marks'=> 100,'userid'=>1,),
								//'output'=>array('Id','msg'),
								'result'=>array('<error>','<error_code>','Fields are missing','min_marks'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'userid'=>1,),
								//'output'=>array('Id','msg'),
								'result'=>array('<error>','<error_code>','Fields are missing','max_marks'),
								'output_type'=>'xml'
						),
				
			            array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
					   array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','value'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_title'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_code'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_title'),
								'output_type'=>'xml'
						),
						
					// data type conflicts
					   array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>$random_word,'min_marks'=>10,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','survey_qust_per_page Enter only numeric values in Questions per page'),
								'output_type'=>'xml'
						),	
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>$random_word,'max_marks'=> 100,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','min_marks Enter only numeric values in Min Score'),
								'output_type'=>'xml'
						),	
						array('input'=>array('survey_title'=>$random_word,'survey_code'=>$random_word,'value'=>"Just try",'survey_status'=>"sry_det_sry_atv",'survey_language'=>"cre_sys_lng_eng",'survey_qust_per_page'=>4,'min_marks'=>20, 'max_marks'=> $random_word, 'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','max_marks Enter only numeric values in Max Score'),
								'output_type'=>'xml'
						),	
				
				)
				
				
				
				
				
			)
		)
?>