<?php
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$id=db_query("select id from slt_program order by id desc limit 0,1")->fetchField();
$value='Sample description';
$status='lrn_lpn_sts_atv';
$object_type=db_query("select object_type from slt_program where id=$id")->fetchField();
$lrp_details=db_query("select id from slt_program where object_type='cre_sys_obt_trn' limit 0,1")->fetchField();
$testCases = array(
		'UpdateTPAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$id,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>$object_type,'lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "','<result>','<id>'),
						   'output_type'=>'xml'
							)
				),
			   'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'id'=>'a','title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>$object_type,'lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('Invalid ID'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$id,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>'aa','object_type'=>$object_type,'lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('Please Enter the correct Status'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$id,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>'aa','lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('object Type incorrect.'),
						   'output_type'=>'xml'
							),
					 array('input'=>array('userid'=>1,'id'=>$lrp_details,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>'cre_sys_obt_trn','lang_code'=>'cre_sys_lng_eng','end_date'=>'aa'),
						  'output'=>array(),
						  'result'=>array('Complete By Invalid date.'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$id,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>$object_type,'lang_code'=>'cre_sys_lng_eng','price'=>10),
						  'output'=>array(),
						  'result'=>array('Currency type should not empty'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$id,'title'=>$random,'code'=>$random,'value'=>$value,'status'=>$status,'object_type'=>$object_type,'lang_code'=>'cre_sys_lng_eng','price'=>10,'currency_type'=>'aa'),
						  'output'=>array(),
						  'result'=>array('Currency Type is not Valid'),
						   'output_type'=>'xml'
							),
						
							
							 	
				)
			)
		);
			
?>		