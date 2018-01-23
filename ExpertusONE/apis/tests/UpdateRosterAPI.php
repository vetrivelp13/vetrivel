<?php
$course_id=db_query("select course_id from slt_enrollment order by id desc limit 0,1")->fetchField();
$class_id=db_query("select class_id from slt_enrollment order by id desc limit 0,1")->fetchField();
$id=db_query("select id from slt_enrollment order by id desc limit 0,1")->fetchField();
$class_id_wbt=db_query("select id from slt_course_class where delivery_type='lrn_cls_dty_wbt' order by id desc limit 0,1")->fetchField();
$course_id_wbt=db_query("select course_id from slt_course_class where id=$class_id_wbt")->fetchField();
$learnerid_wbt=db_query("select id from slt_enrollment where class_id=$class_id_wbt order by id desc limit 0,1")->fetchField();
$id_tp=db_query("select id from slt_enrollment where master_enrollment_id != '' order by master_enrollment_id desc limit 0,1")->fetchField();
$course_id_tp=db_query("select course_id from slt_enrollment where master_enrollment_id != '' order by master_enrollment_id desc limit 0,1")->fetchField();
$class_id_tp=db_query("select class_id from slt_enrollment where master_enrollment_id != '' order by master_enrollment_id desc limit 0,1")->fetchField();
$testCases = array(
		'UpdateRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'EnrollId'=>$id,'course_id'=>$course_id,'class_id'=>$class_id,'action'=>cancelled),
						  'output'=>array('Id','Status','UserId','UserName'),
						  'result'=>array('<results totalrecords =','<Id>','<Status>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1,'class_id'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'course_id'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'EnrollId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'action'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'action'=>xyz,'EnrollId'=>$id,'course_id'=>$course_id,'class_id'=>$class_id),
						       'output'=>array(),
							   'result'=>array('Action can be any of cancelled,completed, noshow, incompleted'),
							   'output_type'=>'xml', 
							 ),
						array('input'=>array('userid'=>1,'action'=>noshow,'EnrollId'=>$learnerid_wbt,'course_id'=>$course_id_wbt,'class_id'=>$class_id_wbt),
						       'output'=>array(),
							   'result'=>array('Web based and Video classes can not be marked as noshow.'),
							   'output_type'=>'xml', 
							 ),
						array('input'=>array('userid'=>1,'action'=>cancelled,'EnrollId'=>$id,'course_id'=>$course_id,'class_id'=>0),
						       'output'=>array(),
							   'result'=>array('Invalid Class ID'),
							   'output_type'=>'xml', 
							 ),
						array('input'=>array('userid'=>1,'action'=>cancelled,'EnrollId'=>$id_tp,'course_id'=>$course_id_tp,'class_id'=>$class_id_tp),
						       'output'=>array(),
							   'result'=>array('Enrollment cannot be cancelled,Associated to Training Plan'),
							   'output_type'=>'xml', 
							 ),
						
						
						
				
				)
			)
		);
			
?>		