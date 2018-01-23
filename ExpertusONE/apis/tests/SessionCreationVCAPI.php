<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_name = getRandomWord();
$cour_id = db_query("select id from slt_course_template order by id desc limit 0,1")->fetchField();
$clas_id = db_query("select id from slt_course_class order by id desc limit 0,1")->fetchField();

$classdet1 = db_query("select id,course_id from slt_course_class where code = 'CLSD-LDRBP-I-VC-001'")->fetchAll();
$classdet2 = db_query("select id,course_id from slt_course_class where code = 'CLSD-SMSFL-VC-001'")->fetchAll();
$classdet3 = db_query("select id,course_id from slt_course_class where code = 'CLSD-SMKPL-VC-001'")->fetchAll();
$classdet4 = db_query("select id,course_id from slt_course_class where code = 'CRSD-SSMGR-VC-001'")->fetchAll();
$classdet5 = db_query("select id,course_id from slt_course_class where code = 'HI-CLS-VC'")->fetchAll();
$classdet6 = db_query("select id,course_id from slt_course_class where code = 'WAB-CRSE-VC'")->fetchAll();
$classdet7 = db_query("select id,course_id from slt_course_class where code = 'DBS-CLS-VC'")->fetchAll();

$manager1 = db_query("select id from slt_person where user_name = 'mariat' and status = 'cre_usr_sts_atv'")->fetchField();
$manager2 = db_query("select id from slt_person where user_name = 'williamb' and status = 'cre_usr_sts_atv'")->fetchField();
$manager3 = db_query("select id from slt_person where user_name = 'sarahj' and status = 'cre_usr_sts_atv'")->fetchField();

$testCases = array(

			// SessionCreationVCAPI test case Starts
			'SessionCreationVCAPI' => array(
				'datasource' => array(
					array('input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
						array('input'=>array('course_id'=>$classdet1[0]->course_id,
								'class_id'=>$classdet1[0]->id,
								'session_name'=>'Leadership best practice - Module 1 session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'instructor_id'=> $manager1,
								'presenter_id'=> $manager1,
								'start_date'=>'2015-09-20',
								'start_hours'=>'09:00',
								'end_hours'=>'11:00',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet2[0]->course_id,
								'class_id'=>$classdet2[0]->id,
								'session_name'=>'Securing managerial support for leadership vc session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'instructor_id'=> $manager1,
								'presenter_id'=> $manager1,
								'start_date'=>'2015-06-16',
								'start_hours'=>'10:00',
								'end_hours'=>'11:00',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet3[0]->course_id,
								'class_id'=>$classdet3[0]->id,
								'session_name'=>'Discover the hidden world of your buyer vc session',
								'meeting_type'=>'lrn_cls_vct_oth',
								'time_zone'=>'cre_sys_tmz_017',
								'instructor_id'=> $manager2,
								'presenter_id'=> $manager2,
								'session_presenter_url' => 'http://expertus.com',
								'session_attendee_url' => 'http://trunk.com',
								'start_date'=>'2015-07-24',
								'start_hours'=>'09:30',
								'end_hours'=>'10:30',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet4[0]->course_id,
								'class_id'=>$classdet4[0]->id,
								'session_name'=>'The situational skills of the manager vc session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'instructor_id'=> $manager2,
								'presenter_id'=> $manager2,
								'start_date'=>'2015-09-15',
								'start_hours'=>'11:30',
								'end_hours'=>'12:30',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet5[0]->course_id,
								'class_id'=>$classdet5[0]->id,
								'session_name'=>'Health Informatics vc session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'start_date'=>'2015-09-23',
								'start_hours'=>'11:00',
								'end_hours'=>'12:00',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet6[0]->course_id,
								'class_id'=>$classdet6[0]->id,
								'session_name'=>'Web Analytics for Business (5 Units) vc session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'start_date'=>'2015-09-30',
								'start_hours'=>'08:00',
								'end_hours'=>'13:00',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('course_id'=>$classdet7[0]->course_id,
								'class_id'=>$classdet7[0]->id,
								'session_name'=>'Database Security vc session',
								'meeting_type'=>'lrn_cls_vct_exp',
								'time_zone'=>'cre_sys_tmz_059',
								'start_date'=>'2015-10-12',
								'start_hours'=>'10:00',
								'end_hours'=>'12:00',
								'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						
				),
				'validate' => array(
 					array('input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
						array(
								'input'=>array('course_id'=>'','class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>'','session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'expertus meeting','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Invalid meeting_type</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'kolkata timezone','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Invalid time_zone</short_msg>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11','end_hours'=>'12:00','userid'=>1),
								'output'=>array(),
								'result'=>array('<results totalrecords =','<result>','<id>Invalid session time.</id>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('course_id'=>$cour_id,'class_id'=>$clas_id,'session_name'=>'checking vc','meeting_type'=>'lrn_cls_vct_exp','time_zone'=>'cre_sys_tmz_059','start_date'=>'2012-08-30','start_hours'=>'11:00','end_hours'=>'12','userid'=>1),
								'output'=>array(),
								'result'=>array('<results totalrecords =','<result>','<id>Invalid session time.</id>'),
								'output_type'=>'xml'
						),
				   
				)
			),  // SessionCreationVCAPI test case Ends
			
		); // End of api test case main array
?>