<?php
$program_id=db_query("select id from slt_program  where id not in(select program_id from slt_module_crs_mapping) order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$program_id:::'.$program_id);

$course_id=db_query("select course_id from slt_course_class where status='lrn_cls_sts_atv' order by course_id desc limit 0,1")->fetchField();
expDebug::dPrint('$course_id:::'.$course_id);

$program_id_enr=db_query("select distinct(program_id) from slt_module_crs_mapping where course_id in(select course_id from slt_enrollment where master_enrollment_id!='') limit 0,1")->fetchField();
expDebug::dPrint('$program_id_enr:::'.$program_id_enr);

$course_id_latest=db_query("select course_id from slt_course_class order by course_id desc limit 0,1")->fetchField();

$already_attached=db_query("select program_id,course_id from slt_module_crs_mapping where course_id not in (select course_id from slt_enrollment) limit 0,1")->fetchAll();
expDebug::dPrint('$already_attached:::'.print_r($already_attached,true),5);


//BSG datasource
$pr_id_1=db_query("select id from slt_program where code='CERD-SEC'")->fetchField();
$pr_id_2=db_query("select id from slt_program where code='CURD-NHO'")->fetchField();
$pr_id_3=db_query("select id from slt_program where code='LEPD-IMP'")->fetchField();
$pr_id_4=db_query("select id from slt_program where code='CURR-HOW'")->fetchField();
$pr_id_5=db_query("select id from slt_program where code='CHI-CUR'")->fetchField();
$pr_id_6=db_query("select id from slt_program where code='CABA-CERT'")->fetchField();
$pr_id_7=db_query("select id from slt_program where code='IS-LP'")->fetchField();


$crs_id_1=db_query("select id from slt_course_template where code='CRSD-LDRBP-I'")->fetchField();
$crs_id_2=db_query("select id from slt_course_template where code='CRSD-ELDBW'")->fetchField();
$crs_id_3=db_query("select id from slt_course_template where code='CRSD-SMSFL'")->fetchField();
$crs_id_4=db_query("select id from slt_course_template where code='CRSD-RLMKS-INTRO'")->fetchField();
$crs_id_5=db_query("select id from slt_course_template where code='CRSD-SMKPL'")->fetchField();
$crs_id_6=db_query("select id from slt_course_template where code='CRSD-BMS'")->fetchField();
$crs_id_7=db_query("select id from slt_course_template where code='CRSD-CCS-001'")->fetchField();
$crs_id_8=db_query("select id from slt_course_template where code='CRSD-CCW-001'")->fetchField();
$crs_id_9=db_query("select id from slt_course_template where code='CRSD-CCV-001'")->fetchField();
$crs_id_10=db_query("select id from slt_course_template where code='CRSD-CCILT-001'")->fetchField();
$crs_id_11=db_query("select id from slt_course_template where code='CRSD-CCTP-001'")->fetchField();
$crs_id_12=db_query("select id from slt_course_template where code='BSH-CRSE'")->fetchField();
$crs_id_13=db_query("select id from slt_course_template where code='HI-CRSE'")->fetchField();
$crs_id_14=db_query("select id from slt_course_template where code='AHI-CRSE'")->fetchField();
$crs_id_15=db_query("select id from slt_course_template where code='BAF-CRSE'")->fetchField();
$crs_id_16=db_query("select id from slt_course_template where code='ERA-CRSE'")->fetchField();
$crs_id_17=db_query("select id from slt_course_template where code='WAB-CRSE'")->fetchField();
$crs_id_18=db_query("select id from slt_course_template where code='DBS-CRSE'")->fetchField();
$crs_id_19=db_query("select id from slt_course_template where code='ITSPP-CRSE'")->fetchField();
$crs_id_20=db_query("select id from slt_course_template where code='EIS-CRSE'")->fetchField();




$testCases = array(
		'AttachCourseToTPAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'program_id'=>$program_id,'course_id'=>$course_id,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "','<id>'),
						   'output_type'=>'xml'
							),
						/*TP-1*/
					array('input'=>array('userid'=>1,'program_id'=>$pr_id_1,'course_id'=>$crs_id_1,'groupname'=>'Leadership','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
					array('input'=>array('userid'=>1,'program_id'=>$pr_id_1,'course_id'=>$crs_id_2,'groupname'=>'Leadership','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
					
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_1,'course_id'=>$crs_id_3,'groupname'=>'Leadership','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-2*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_2,'course_id'=>$crs_id_4,'groupname'=>'Market Strategy','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_2,'course_id'=>$crs_id_5,'groupname'=>'Buyer','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_2,'course_id'=>$crs_id_6,'groupname'=>'Business Mgmt Skills','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-3*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_3,'course_id'=>$crs_id_2,'groupname'=>'Leadership','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_3,'course_id'=>$crs_id_5,'groupname'=>'Buyer','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_3,'course_id'=>$crs_id_3,'groupname'=>'Manager Support','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-4*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_4,'course_id'=>$crs_id_7,'groupname'=>'Course Class Structure','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_4,'course_id'=>$crs_id_8,'groupname'=>'Create Web based Training','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_4,'course_id'=>$crs_id_9,'groupname'=>'Create Virtual Class','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_4,'course_id'=>$crs_id_10,'groupname'=>'Create Instructor Led Training','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_4,'course_id'=>$crs_id_11,'groupname'=>'Create Training Plan','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-5*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_5,'course_id'=>$crs_id_12,'groupname'=>'Biomedical','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_5,'course_id'=>$crs_id_13,'groupname'=>'Health Informatics','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_5,'course_id'=>$crs_id_14,'groupname'=>'Adv.Health Information','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-6*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_15,'groupname'=>'Business Analytics','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_16,'groupname'=>'Risk Analytics','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_17,'groupname'=>'Business Analytics','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						/*TP-7*/
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_18,'groupname'=>'Database Security','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_19,'groupname'=>'Policies and Procedures','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'program_id'=>$pr_id_6,'course_id'=>$crs_id_20,'groupname'=>'Information Security','isrequired'=>0),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "','<id>'),
								'output_type'=>'xml'
						),
						
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'program_id'=>$program_id,'course_id'=>$course_id,'groupname'=>group1,'isrequired'=>a),
						  'output'=>array(),
						  'result'=>array('Is required should be 0 or 1'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$program_id,'course_id'=>$course_id,'groupname'=>'++','isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('Group name contains invalid characters'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>aa,'course_id'=>$course_id,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('Invalid program id'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$program_id+1,'course_id'=>$course_id,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('Program id is invalid or doesn\'t exist'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$program_id_enr,'course_id'=>$course_id,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('The given training program has active enrollments.'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$program_id,'course_id'=>aa,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('Invalid course id'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$program_id,'course_id'=>$course_id_latest+1,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('Course id is invalid or doesn\'t exist'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'program_id'=>$already_attached[0]->program_id,'course_id'=>$already_attached[0]->course_id,'groupname'=>group1,'isrequired'=>0),
						  'output'=>array(),
						  'result'=>array('The given course has already been attached to the given training plan'),
						   'output_type'=>'xml'
							)
						
							
							
						
							 						
						
				
				)
			)
		);
			
?>		