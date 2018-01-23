<?php 
$userdetails = db_query("select id,user_name from slt_person where is_manager = 'N' order by id desc limit 1")->fetchAll();

$testCases = array(

			
		'ListEnrollmentsAPI' => array(
				'datasource' => array(
						array('input'=>array('UserID'=>$userdetails[0]->id,'regstatuschk'=>'lrn_crs_cmp_enr','rows'=>10,'userid'=>1),
								'output'=>array('id','master_enrollment_id','user_id','user_name','class_id','course_id','reg_status','comp_status','valid_from','valid_to','score','title','cls_title','code','description','delivery_type','basetype','classprice',
										'currency','currency_code','currency_symbol','language','rating','created_by','created_by_name','updated_by','updated_by_name','update_date','launch','sessiondetails','is_compliance','mro','preassessment_status','compliance_complete_date',
										'compliance_complate_days','compliance_validity_date','compliance_validity_days','created_by_ins_mngr_slf','updated_by_ins_mngr_slf','tp_details','tp_course_data','class_status','user_rating','preassesment_completion_status',
										'pre_total_attempts','pre_attempts','post_total_attempts','post_attempts','waived_status'
								),
								'result'=>array('<results totalrecords =','<id>','<user_id>','<reg_status>Enrolled</reg_status>'),
								'type'=>'DataSource','output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_cmp','rows'=>10,'userid'=>1),
								'output'=>array('id','master_enrollment_id','user_id','user_name','class_id','course_id','reg_status','comp_status'),
								'result'=>array('<results totalrecords =','<id>','<user_id>','<reg_status>Completed</reg_status>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_inp','rows'=>10,'userid'=>1),
								'output'=>array('id','master_enrollment_id','user_id','user_name','class_id','course_id','reg_status','comp_status'),
								'result'=>array('<results totalrecords =','<id>','<user_id>','<reg_status>In progress</reg_status>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','delivery_type_code'=>'lrn_cls_dty_wbt','rows'=>10,'userid'=>1),
								'output'=>array('id','master_enrollment_id','user_id','user_name','class_id','course_id','reg_status','comp_status'),
								'result'=>array('<results totalrecords =','<id>','<user_id>','<reg_status>Enrolled</reg_status>','<delivery_type>Web-based</delivery_type>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','delivery_type_code'=>'lrn_cls_dty_wbt','rows'=>'ten','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>rows should be a number</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','delivery_type_code'=>'lrn_cls_dty_wbt','rows'=>'ten','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>rows should be a number</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','date_from'=>'04-08-2016','rows'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>From date should be in yyyy-mm-dd format</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','date_from'=>'2016-04-08','date_to'=>'04-08-2016','rows'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>To date should be in yyyy-mm-dd format</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','date_from'=>'2016-08-04','date_to'=>'2016-08-03','rows'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array(' <short_msg>To date should be greater than or equal to from date</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('UserID'=>1,'regstatuschk'=>'lrn_crs_cmp_enr','date_from'=>'2016-08-04','date_to'=>'2016-08-05','rows'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array(' <id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),

				)
		),
			
);
?>