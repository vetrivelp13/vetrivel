<?php
   // $surv_id = db_query("select id from slt_survey ")->fetchAll();
    $surv_id = db_query("select * from slt_survey where type='sry_det_typ_sry' order by id DESC limit 1  ")->fetchAll();
	$count=count($surv_id);
	//expDebug::dPrint(" Checking ".print_r($surv_id));
	expDebug::dPrint(" Checking ".print_r($count));
	//expDebug::dPrint("CHECKING".print_r($surv_id, 1));
$testCases = array(
         // For this test case to pass, survey must be active i.e. questions must be attached and survey must be published.
		//$count is 1 and so count-1 is 0. surv_id[count-1] is  the last updated value.
			'UpdateSurveyAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
							)
				),
				
				'validate' => array(
						array('input'=>array('userid'=>1,'survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'survey_id'=>'','survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page),
						// 'output'=>array('Id'),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','survey_id'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','title'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
					   array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','value'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_code'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'survey_qust_per_page'=>$surv_id[$count-1]->question_per_page,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_title'),
								'output_type'=>'xml'
						),
		//two fields missing
		
		               array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_status'=>$surv_id[$count-1]->status,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','survey_language'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'value'=>$surv_id[$count-1]->description,'survey_language'=>$surv_id[$count-1]->lang_code,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','survey_status'),
								'output_type'=>'xml'
						),
						array('input'=>array('survey_id'=>$surv_id[$count-1]->id,'survey_title'=>$surv_id[$count-1]->title,'survey_code'=>$surv_id[$count-1]->code,'survey_status'=>$surv_id[$count-1]->status,'survey_language'=>$surv_id[$count-1]->lang_code,'userid'=>1),
								//'output'=>array('Id'),
								'result'=>array('<error>','<error_code>','Fields are missing','survey_qust_per_page','value'),
								'output_type'=>'xml'
						),				
					)
		)
	)
?>