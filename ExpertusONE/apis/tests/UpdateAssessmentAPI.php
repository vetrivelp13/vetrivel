<?php
    $assess = db_query("select * from slt_survey where type='sry_det_typ_ass' order by id DESC limit 1 ")->fetchAll();
	$count=count($assess);
	expDebug::dPrint(" Checking ".print_r($count));
	expDebug::dPrint("CHECKING".print_r($assess, 1));
	
	$testCases = array(
         
		//$count is 1 and so count-1 is 0. assess[count-1] is  the last updated value.
			'UpdateAssessmentAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
							),
					 array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 'output'=>array('Id'),
								'result'=>array('jsonResponse','Id'),
								'output_type'=>'json'
							)
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
					// one field is missing	
						array('input'=>array('userid'=>1,'survey_id'=>'','survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 //'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_id'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>'','survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 //'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_title'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>'','value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 //'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_code'),
								'output_type'=>'xml'
						),
						//SQL_INJECTION_ERROR is given by simulator when "Value" field is missing
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>'','survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						// 'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','value'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>'','survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						// 'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','status'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>'','survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						 //'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>'','min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
						// 'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_qust_per_page'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>'','max_marks'=>$assess[$count-1]->max_mark),
						 //'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','min_marks'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>''),
						// 'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','max_marks'),
								'output_type'=>'xml'
						),
						
			  //Data type validation
				        array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>"abc",'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>$assess[$count-1]->max_mark),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','survey_qust_per_page Enter only numeric values in Questions per page'),
								'output_type'=>'xml'
						),	
						array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>"abc",'max_marks'=>$assess[$count-1]->max_mark),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','min_marks Enter only numeric values in Min Score'),
								'output_type'=>'xml'
						),	
						array('input'=>array('input'=>array('userid'=>1,'survey_id'=>$assess[$count-1]->id,'survey_title'=>$assess[$count-1]->title,'survey_code'=>$assess[$count-1]->code,'value'=>$assess[$count-1]->description,'survey_status'=>$assess[$count-1]->status,'survey_language'=>$assess[$count-1]->lang_code,'survey_qust_per_page'=>$assess[$count-1]->question_per_page,'min_marks'=>$assess[$count-1]->min_mark,'max_marks'=>"abc"),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','max_marks Enter only numeric values in Max Score'),
								'output_type'=>'xml'
						),	
				
			)
		)
	)
    
   ) 
    
    
?>