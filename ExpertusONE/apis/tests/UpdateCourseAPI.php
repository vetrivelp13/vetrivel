<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$randomDate = date("d M Y", $timestamp);
$course_id = db_query("select id from slt_course_template limit 0,1")->fetchField();
$testCases = array(

			// UpdateCourseAPI test case Starts
			'UpdateCourseAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>'','crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_id'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>'','crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_title'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>'','value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_code'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','value'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_status'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>''),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_language'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>'','crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_id'),
						  'output_type'=>'xml'
						    ),
				    array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>'','crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_title'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>'','value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_code'),
						  'output_type'=>'xml'
						    ),
				    array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','value'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>''),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','crs_language'),
						  'output_type'=>'xml'
						    ),
					array(
						  'input'=>array('userid'=>1,'crs_id'=>$course_id,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'tyty','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Invalid course status.</short_msg>'),
						  'output_type'=>'xml'
						    )
						
						
						
						
				)
			),  // UpdateCourseAPI test case Ends
			
		); // End of api test case main array
?>