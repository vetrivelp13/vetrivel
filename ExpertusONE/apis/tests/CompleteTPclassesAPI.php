<?php 

$enr_id=db_query("select id from slt_enrollment order by id desc limit 0,1")->fetchField();
$user_id=db_query("select user_id from slt_enrollment where id=$enr_id")->fetchField();
$class_id=db_query("select class_id from slt_enrollment where id=$enr_id")->fetchField();
$today=date('m-d-Y');
$learning_type='class';
$learning_type_tp='Training Plan';
$score=10;

$master_enr_id=db_query("select master_enrollment_id from slt_enrollment order by master_enrollment_id desc limit 0,1 ")->fetchField();
$enr_id_tp=db_query("select id from slt_enrollment where master_enrollment_id=$master_enr_id ")->fetchField();
$class_id_tp=db_query("select class_id from slt_enrollment where id=$enr_id_tp")->fetchField();

$testCases = array(
		'CompleteTPclassesAPI' => array(
				'datasource' => array(
						array('input'=>array('user_id'=>$user_id,'userid'=>$user_id,'Enrolled_id'=>$enr_id,'classId'=>$class_id,'completionDate'=>$today,'learning_type'=>$learning_type,'score'=>$score),
						  'output'=>array('title','code','description','type','status'),
						  'result'=>array('<results totalrecords = "">','<result>','<title>','<code>','<description>','<type>','<status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$user_id,'userid'=>$user_id,'Enrolled_id'=>$enr_id_tp,'classId'=>$class_id_tp,'completionDate'=>$today,'learning_type'=>$learning_type_tp,'score'=>$score),
								'output'=>array('title','code','description','type','status'),
								'result'=>array('<results totalrecords = "">','<result>','<title>','<code>','<description>','<type>','<status>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed.'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$user_id,'userid'=>$user_id,'Enrolled_id'=>$enr_id+1,'classId'=>$class_id,'completionDate'=>$today,'learning_type'=>$learning_type,'score'=>$score),
								'output'=>array(),
								'result'=>array('Enrollment Id,Course Id ,Class Id,User Id-> Are not assoicated with prticaular Enrollement. Kindly Check it.'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$user_id,'userid'=>$user_id,'Enrolled_id'=>$enr_id_tp+1,'classId'=>$class_id_tp,'completionDate'=>$today,'learning_type'=>$learning_type_tp,'score'=>$score),
								'output'=>array(),
								'result'=>array('Given Enrollment Id,Course Id ,Class Id,User Id-> Are not assoicated with prticaular Traning Plan Enrollement. Kindly Check it.'),
								'output_type'=>'xml'
						),

			)
		)
	);
?>