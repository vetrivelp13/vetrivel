<?php
$master_enroll_id=db_query("select master_enrollment_id from slt_enrollment where master_enrollment_id!='' order by master_enrollment_id desc limit 0,1")->fetchField();
$program_id=db_query("select program_id from slt_module_crs_mapping where course_id in (select course_id from slt_enrollment where master_enrollment_id=$master_enroll_id)")->fetchField();
$date = date("Y-m-d");
expDebug::dPrint('$date::'.$date);
expDebug::dPrint('$master_enroll_id::'.$master_enroll_id);
expDebug::dPrint('$program_id::'.$program_id);

$testCases = array(
		'UpdateTPRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'action'=>Completed,'MasterEnrollId'=>$master_enroll_id,'ProgramId'=>$program_id,'CompletionDate'=>$date,'RegStatusDate'=>$date),
						  'output'=>array('Id','Status','UserId','UserName'),
						  'result'=>array('<results totalrecords = "'),
						   'output_type'=>'xml'
							)
				),
			   'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'action'=>aaa,'MasterEnrollId'=>$master_enroll_id,'ProgramId'=>$program_id,'CompletionDate'=>$date,'RegStatusDate'=>$date),
						  'output'=>array(),
						  'result'=>array('action Type is invalid'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'action'=>Completed,'MasterEnrollId'=>$master_enroll_id,'ProgramId'=>$program_id,'CompletionDate'=>00,'RegStatusDate'=>$date),
						  'output'=>array(),
						  'result'=>array('Completion Date should be in yyyy-mm-dd format'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'action'=>Completed,'MasterEnrollId'=>$master_enroll_id,'ProgramId'=>$program_id,'CompletionDate'=>$date,'RegStatusDate'=>00),
						  'output'=>array(),
						  'result'=>array('RegStatus Date should be in yyyy-mm-dd format'),
						   'output_type'=>'xml'
							),
						
							 	
				)
			)
		);
			
?>		