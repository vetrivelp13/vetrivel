<?php 

$completedsession = db_query("select sess.class_id as classid,concat(date_format(sess.start_date,'%Y-%m-%d'),' ',sess.start_time) as startdate,GROUP_CONCAT(enr.id SEPARATOR ',') as reg_id ,insdet.instructor_id as insid
from slt_course_class_session sess
left join slt_enrollment enr on enr.class_id = sess.class_id
left join slt_session_instructor_details insdet on (insdet.class_id= sess.class_id and insdet.session_id = sess.id)
where insdet.session_id is not null and enr.id is not null 
and concat(date_format(sess.start_date,'%Y-%m-%d'),' ',sess.start_time) <= NOW()
group by sess.class_id")->fetchAll();
expDebug::dPrint('"Mark useridd count: '.print_r($completedsession, true) , 4);
$futuresession = db_query("select sess.class_id as classid,concat(date_format(sess.start_date,'%Y-%m-%d'),' ',sess.start_time) as startdate,GROUP_CONCAT(enr.id SEPARATOR ',') as reg_id ,insdet.instructor_id as insid
from slt_course_class_session sess
left join slt_enrollment enr on enr.class_id = sess.class_id
left join slt_session_instructor_details insdet on (insdet.class_id= sess.class_id and insdet.session_id = sess.id)
where insdet.session_id is not null and enr.id is not null
and concat(date_format(sess.start_date,'%Y-%m-%d'),' ',sess.start_time) > NOW()
group by sess.class_id")->fetchAll();
expDebug::dPrint('"Mark feature '.print_r($futuresession, true) , 4);
$testCases = array(


		'InstructorMarkCompletionAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'user_id'=>$completedsession[0]->insid,'regids'=>$completedsession[0]->reg_id,'classid'=>$completedsession[0]->classid,'completiondate'=>date('m-d-Y'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						)
						),
				'validate' => array(
						array('input'=>array('userid'=>1,'user_id'=>'','regids'=>$completedsession[0]->reg_id,'classid'=>$completedsession[0]->classid,'completiondate'=>date('m-d-Y'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$completedsession[0]->insid,'regids'=>'','classid'=>$completedsession[0]->classid,'completiondate'=>date('m-d-Y'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$completedsession[0]->insid,'regids'=>$completedsession[0]->reg_id,'classid'=>'','completiondate'=>date('m-d-Y'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>'','regids'=>$completedsession[0]->reg_id,'classid'=>$completedsession[0]->classid,'completiondate'=>'','score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$completedsession[1]->insid,'regids'=>$completedsession[1]->reg_id,'classid'=>$completedsession[1]->classid,'completiondate'=>date('Y-m-d'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid date format.It should be mm-dd-yyyy'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$futuresession[0]->insid,'regids'=>$futuresession[0]->reg_id,'classid'=>$futuresession[0]->classid,'completiondate'=>date('m-d-Y'),'score'=>'100','grade'=> 'pass','attendancetype'=>'completed'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Session Not finished'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						)
				),
		);
?>