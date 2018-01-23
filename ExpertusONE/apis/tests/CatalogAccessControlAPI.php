<?php 
// Courses Access
$cou1 = db_query("select id from slt_course_template where code = 'CRSD-LDRBP-I'")->fetchField();
$cou2 = db_query("select id from slt_course_template where code = 'CRSD-LDRBP-II'")->fetchField();
$cou3 = db_query("select id from slt_course_template where code = 'CRSD-ELDBW'")->fetchField();
$cou4 = db_query("select id from slt_course_template where code = 'CRSD-SMSFL'")->fetchField();
$cou5 = db_query("select id from slt_course_template where code = 'CRSD-RLMKS-INTRO'")->fetchfield();
$cou6 = db_query("select id from slt_course_template where code = 'CRSD-RLMKS-INTER'")->fetchField();
$cou7 = db_query("select id from slt_course_template where code = 'CRSD-RLMKS-ADV'")->fetchField();
$cou8 = db_query("select id from slt_course_template where code = 'CRSD-LGLCM'")->fetchFeild();
$cou9 = db_query("select id from slt_course_template where code = 'CRSD-EONEMT-MOBILE'")->fetchField();
$cou10 = db_query("select id from slt_course_template where code = 'CRSD-EONE-MOBILE'")->fetchField();

// Class Access
$cla1 = db_query("select id from slt_course_class where code = 'CRSD-EONE-MOBILE-VOD'")->fetchField();
$cla2 = db_query("select id from slt_course_class where code = 'CRSD-EONEMT-MOBILE'")->fetchField();

// User Access


$testCases = array(

			// CatalogAccessControlAPI test case Starts
			'CatalogAccessControlAPI' => array(
				'datasource' => array(
						// Course
					array(	'input'=>array(	'userid'=>1,
											'entity_id'=>$cou1,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_rec,Learner_Employee::cre_sys_inv_man,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(	
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou2,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_rec,Learner_Employee::cre_sys_inv_man,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou3,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_man,Learner_Employee::cre_sys_inv_man,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou4,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_opt,Learner_Employee::cre_sys_inv_man,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou5,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_opt,Learner_Employee::cre_sys_inv_opt,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou6,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_opt,Learner_Employee::cre_sys_inv_opt,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou7,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_opt,Learner_Employee::cre_sys_inv_opt,Learner_Partner::cre_sys_inv_opt'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou8,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Learner_Customer::cre_sys_inv_opt,Learner_Employee::cre_sys_inv_rec,Learner_Partner::cre_sys_inv_man'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou9,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Sandbox User::cre_sys_inv_rec'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cou10,
											'entity_type'=>'cre_sys_obt_crs',
											'group_mro'=>'Sandbox User::cre_sys_inv_rec'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					// Class
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cla1,
											'entity_type'=>'cre_sys_obt_cls',
											'group_mro'=>'Sandbox User::cre_sys_inv_man'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'entity_id'=>$cla2,
											'entity_type'=>'cre_sys_obt_cls',
											'group_mro'=>'Sandbox User::cre_sys_inv_man'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					
					
						
				),
				'validate' => array(
//  					array('input'=>array('userid'=>1,'deliveryTypeCode'=>'lrn_cls_dty','limit'=>10),
// 						  'output'=>array('name','code','attr1','attr2'),
// 						  'result'=>array('<results totalrecords =','<result>','<name>','<code>','<attr1>','<attr2>'),
// 						  'output_type'=>'xml'
// 							),
				)
			),  // CatalogAccessControlAPI test case Ends
			
		); // End of api test case main array
?>