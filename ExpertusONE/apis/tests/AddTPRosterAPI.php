<?php
$learnerid=db_query("select id from slt_person where status='cre_usr_sts_atv' order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$learnerid'.$learnerid);
$program_id=db_query("select id from slt_program where id in(select program_id from slt_module_crs_mapping) order by id desc limit 0,1")->fetchField();
$course_id=db_query("select course_id  from slt_module_crs_mapping where program_id=$program_id order by id asc")->fetchField();
$cllass_id=db_query("select id from slt_course_class where course_id=$course_id")->fetchcol();
$program_id_inactive=db_query("select id from slt_program order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$program_id'.$program_id);
expDebug::dPrint('$course_id'.$course_id);
expDebug::dPrint('$cllass_id'.print_r($cllass_id,true),5);
$testCases = array(
		'AddTPRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>$program_id),
						  'output'=>array('masterEnroll','userid','status','username'),
						  'result'=>array('<results totalrecords = "">',),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ClassIds'=>$cllass_id[0]),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
							 
						array('input'=>array('userid'=>1,'Learnerid'=>aa,'ProgramId'=>$program_id),
						       'output'=>array(),
							   'result'=>array('Invalid Learner id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid+1,'ClassIds'=>$class_ids[0],'ProgramId'=>$program_id),
						       'output'=>array(),
							   'result'=>array('The given Learner id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>aa),
						       'output'=>array(),
							   'result'=>array('Invalid program id'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>$program_id_inactive+1),
						       'output'=>array(),
							   'result'=>array('Program id is invalid or inactive status'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>$program_id,'ClassIds'=>aa),
						  'output'=>array(),
						  'result'=>array('Invalid Class id'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>$program_id,'ClassIds'=>$cllass_id[0]+1),
						  'output'=>array(),
						  'result'=>array('The given class id'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'Learnerid'=>$learnerid,'ProgramId'=>$program_id,'ClassIds'=>$cllass_id[0],'RegDate'=>1),
						  'output'=>array(),
						  'result'=>array('RegStatusDate Date is invalid'),
						   'output_type'=>'xml'
							)
						
				
				)
			)
		);
			
?>		