<?php
$class_id=db_query("select id from slt_course_class limit 0,1")->fetchField();
$course_id=db_query("select course_id from slt_course_class  where id = $class_id")->fetchField();

$class_id_mismatch=db_query("select id from slt_course_class where course_id=$course_id order by id desc limit 0,1")->fetchField();
$learner_id=db_query("select id from slt_person order by id desc limit 0,1")->fetchField();

expDebug::dPrint('Country code----'.$course_id);
expDebug::dPrint('Country code----'.$course_id);
expDebug::dPrint('Country code----'.$class_id_mismatch);
expDebug::dPrint('Country code----'.$learner_id);
$testCases = array(
		'AddRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'Learnerid'=>$learner_id,'Courseid'=>$course_id,'Classid'=>$class_id),
						  'output'=>array('id','userid','status'),
						  'result'=>array('<id>','<userid>','<status>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1,'Classid'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Courseid'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learner_id,'Courseid'=>$course_id,'Classid'=>$class_id_mismatch+1),
						       'output'=>array(),
							   'result'=>array('Class Id and Course Id do not match'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learner_id,'Courseid'=>$course_id,'Classid'=>$class_id,'RegDate'=>'000'),
						       'output'=>array(),
							   'result'=>array('Reg Date should be in yyyy-mm-dd format'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'Learnerid'=>$learner_id,'Courseid'=>$course_id,'Classid'=>$class_id),
						  'output'=>array('id','userid','status'),
						  'result'=>array('<id>','<userid>'."$learner_id".'</userid>','<status>'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'Learnerid'=>$learner_id+1,'Courseid'=>$course_id,'Classid'=>$class_id),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>'."5".'</error_code>','<short_msg>'."Invalid User ID".'</short_msg>','<long_msg>'."Invalid User ID".'</long_msg>'),
						   'output_type'=>'xml'
							)
					
						
						
				
				)
			)
		);
			
?>		