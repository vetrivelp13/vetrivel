<?php 
$iltclass = db_query('select id,course_id from slt_course_class  where delivery_type = "lrn_cls_dty_ilt" order by id desc limit 1')->fetchAll();
$sessiondetails = db_query('select course_id,class_id,date(start_date) as start_date,start_time,end_time from slt_course_class_session order by id desc limit 1')->fetchAll();
$activeenrollments =db_query('select enr.course_id as crsid ,enr.class_id as clsid from slt_enrollment enr LEFT JOIN slt_course_class cls on (cls.id = enr.class_id AND cls.course_id = enr.course_id)where cls.delivery_type = "lrn_cls_dty_ilt" AND(enr.reg_status IN  ("lrn_crs_reg_wtl", "lrn_crs_reg_ppm", "lrn_crs_reg_cnf", "lrn_crs_reg_ppv") ) order by enr.id desc limit 1')->fetchAll();
$activeins = db_query('select id from slt_person where is_instructor ="Y" and status = "cre_usr_sts_atv" order by id desc limit 1')->fetchAll();
$invalidins = db_query('select id from slt_person where is_instructor ="Y" and status = "cre_usr_sts_itv" order by id desc limit 1')->fetchAll();

$classdet1 = db_query("select id,course_id from slt_course_class where code = 'CLSD-LDRBP-II-ILT-001'")->fetchAll();
$classdet2 = db_query("select id,course_id from slt_course_class where code = 'CLSD-ELDBW-ILT-001'")->fetchAll();
$classdet3 = db_query("select id,course_id from slt_course_class where code = 'CLSD-RLMKS-INTRO-ILT-001'")->fetchAll();
$classdet4 = db_query("select id,course_id from slt_course_class where code = 'CLSD-RLMKP-ILT-001'")->fetchAll();
$classdet5 = db_query("select id,course_id from slt_course_class where code = 'CRSD-FKDSE-ILT-001'")->fetchAll();
$classdet6 = db_query("select id,course_id from slt_course_class where code = 'AHI-CRSE-ILT-001'")->fetchAll();
$classdet7 = db_query("select id,course_id from slt_course_class where code = 'AHI-CRSE-ILT1'")->fetchAll();
$classdet8 = db_query("select id,course_id from slt_course_class where code = 'ERA-CRSE-ILT'")->fetchAll();
$classdet9 = db_query("select id,course_id from slt_course_class where code = 'ITSPP-CRSE-ILT'")->fetchAll();
$classdet10 = db_query("select id,course_id from slt_course_class where code = 'EIS-CRSE-ILT'")->fetchAll();

$manager1 = db_query("select id from slt_person where user_name = 'mariat' and status = 'cre_usr_sts_atv'")->fetchField();
$manager2 = db_query("select id from slt_person where user_name = 'williamb' and status = 'cre_usr_sts_atv'")->fetchField();
$manager3 = db_query("select id from slt_person where user_name = 'sarahj' and status = 'cre_usr_sts_atv'")->fetchField();
$testCases = array(

			
			'SessionCreationILTAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'session_name'=>'newsession','start_date'=>'2017-11-09','start_hours'=>'00:00','end_hours'=>'00:15'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet1[0]->course_id,
								'class_id'=>$classdet1[0]->id,
								'instructor_id'=> $manager2,
								'session_name'=>'Leadership best practice session',
								'start_date'=>'2015-07-29',
								'start_hours'=>'09:00',
								'end_hours'=>'16:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						     ),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet2[0]->course_id,
								'class_id'=>$classdet2[0]->id,
								'instructor_id'=> $manager2,
								'session_name'=>'Establishing leadership in the best way session',
								'start_date'=>'2015-11-11',
								'start_hours'=>'09:00',
								'end_hours'=>'17:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet3[0]->course_id,
								'class_id'=>$classdet3[0]->id,
								'instructor_id'=> $manager1,
								'session_name'=>'Relationship marketing strategy - Introduction',
								'start_date'=>'2015-04-28',
								'start_hours'=>'10:30',
								'end_hours'=>'17:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet4[0]->course_id,
								'class_id'=>$classdet4[0]->id,
								'instructor_id'=> $manager3,
								'session_name'=>'Relationship sales in practice session',
								'start_date'=>'2015-12-30',
								'start_hours'=>'12:00',
								'end_hours'=>'13:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet5[0]->course_id,
								'class_id'=>$classdet5[0]->id,
								'instructor_id'=> $manager3,
								'session_name'=>'The four keys to developing self-esteem session',
								'start_date'=>'2015-03-23',
								'start_hours'=>'09:00',
								'end_hours'=>'16:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet6[0]->course_id,
								'class_id'=>$classdet6[0]->id,
								'session_name'=>'Advanced Health Information session',
								'start_date'=>'2015-11-18',
								'start_hours'=>'09:30',
								'end_hours'=>'18:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet7[0]->course_id,
								'class_id'=>$classdet7[0]->id,
								'session_name'=>'Advanced Health Information- session',
								'start_date'=>'2015-08-26',
								'start_hours'=>'05:15',
								'end_hours'=>'07:15'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet8[0]->course_id,
								'class_id'=>$classdet8[0]->id,
								'session_name'=>'Enterprise Risk Analytics (10 Units) session',
								'start_date'=>'2015-11-04',
								'start_hours'=>'09:00',
								'end_hours'=>'13:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet9[0]->course_id,
								'class_id'=>$classdet9[0]->id,
								'session_name'=>'IT Security Policies and Procedures session',
								'start_date'=>'2015-09-26',
								'start_hours'=>'09:30',
								'end_hours'=>'16:30'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$classdet10[0]->course_id,
								'class_id'=>$classdet10[0]->id,
								'session_name'=>'Enterprise Information Security session',
								'start_date'=>'2015-10-14',
								'start_hours'=>'09:30',
								'end_hours'=>'16:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'20-05-2017','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid date.</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$sessiondetails[0]->course_id,'class_id'=>$sessiondetails[0]->class_id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>$sessiondetails[0]->start_date,'start_hours'=>$sessiondetails[0]->start_time,'end_hours'=>$sessiondetails[0]->end_time),
								'output'=>array('id'),
								'result'=>array('<id>Location has already been booked for the entered session time.</id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>10,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>$sessiondetails[0]->start_date,'start_hours'=>$sessiondetails[0]->start_time,'end_hours'=>$sessiondetails[0]->end_time),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid course or class id</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$activeenrollments[0]->crsid,'class_id'=>$activeenrollments[0]->clsid,'session_name'=>'nuisec','start_date'=>'2016-12-11','start_hours'=>'22:00','end_hours'=>'23:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Session cannot be added as there are active enrollments for this class.</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'00:00','end_hours'=>'00:00'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid session time.</id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>'','class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>'','instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>''),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>'','session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'11:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<id>The end time must be greater than the start time.</id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>$activeins[0]->id,'session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$iltclass[0]->course_id,'class_id'=>$iltclass[0]->id,'instructor_id'=>$invalidins[0]->id,'session_name'=>'nuisec','start_date'=>'2017-05-25','start_hours'=>'09:00','end_hours'=>'10:00'),
								'output'=>array('id'),
								'result'=>array('<id>Invalid Instructor'),
								'output_type'=>'xml'
						),
						
				)
			),  
			
		); 
?>