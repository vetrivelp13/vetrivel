<?php 
$session = db_query('select id,class_id from slt_course_class_session order by id desc limit 1')->fetchAll();
expDebug::dPrint("Vincen test -- ".print_r(($session[0]->id)+1,true),1);
$activeenrollments = db_query('select enr.course_id as crsid ,enr.class_id as clsid ,sess.id as session_id
from slt_enrollment enr 
LEFT JOIN slt_course_class_session sess on (sess.class_id = enr.class_id AND sess.course_id = enr.course_id)
LEFT JOIN slt_course_class cls on (cls.id = enr.class_id AND cls.course_id = enr.course_id)
where (cls.delivery_type = "lrn_cls_dty_ilt")
 AND(enr.reg_status IN  ("lrn_crs_reg_wtl", "lrn_crs_reg_ppm", "lrn_crs_reg_cnf", "lrn_crs_reg_ppv") )
order by enr.id desc limit 1
		')->fetchAll();
$activeins = db_query('select id from slt_person where is_instructor ="Y" and status = "cre_usr_sts_atv" order by id desc limit 1')->fetchAll();
$invalidins = db_query('select id from slt_person where is_instructor ="Y" and status = "cre_usr_sts_itv" order by id desc limit 1')->fetchAll();
$testCases = array(


			'SessionUpdationILTAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
							)
				),
				'validate' => array(
				array('input'=>array('userid'=>1,'hid_session_id'=>($session[0]->id)+1,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid Id</id>'),
								'type'=>'DataSource','output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'hid_session_id'=>$activeenrollments[0]->session_id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Session cannot be updated as there are active enrollments for this class.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'09:00'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid session time.</id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>'','session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'test session','start_date'=>'','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>''),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'instructor_id'=>$activeins[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid Id</id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'hid_session_id'=>$session[0]->id,'instructor_id'=>$invalidins[0]->id,'session_name'=>'test session','start_date'=>'2017-09-04','start_hours'=>'09:00','end_hours'=>'10:30'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid Instructor'),
								'type'=>'DataSource','output_type'=>'xml'
						)
				)
			), 
			
		); 
?>