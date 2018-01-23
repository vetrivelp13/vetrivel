<?php
$class_id=db_query("select id from slt_course_class where delivery_type='lrn_cls_dty_wbt' and id not in (select class_id from slt_enrollment) order by id desc limit 0,1")->fetchField();
expDebug::dPrint('Form state class id: '.$class_id);
$course_id=db_query("select course_id from slt_course_class where id=$class_id")->fetchField();
$wbt_content=db_query("select id from slt_content_master where type!='lrn_cnt_typ_vod' and status='lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField();
expDebug::dPrint('Form state class id: '.$content_id);
$class_details_enr=db_query("select class_id, course_id from slt_enrollment where class_id in (select id from slt_course_class where delivery_type='lrn_cls_dty_wbt' and status='lrn_cls_sts_atv') order by id desc limit 0,1")->fetchAll();
$class_details_notwbt=db_query("select id, course_id from slt_course_class where delivery_type='lrn_cls_dty_ilt' order by id desc limit 0,1")->fetchAll();
$content_id_invalid=db_query("select id from slt_content_master where status='lrn_cnt_sts_itv' order by id desc limit 0,1")->fetchField();
$already_added_details=db_query("select course_id,class_id,content_id from slt_course_content_mapper order by class_id asc limit 0,1")->fetchAll();
expDebug::dPrint('Form state : '.print_r($already_added_details , true), 5);
$wbt_id=db_query("select id,course_id from slt_course_class where delivery_type='lrn_cls_dty_wbt' and id not in (select class_id from slt_enrollment)  limit 0,1")->fetchAll(); 
$vod_id=db_query("select id,course_id from slt_course_class where delivery_type='lrn_cls_dty_vod' and id not in (select class_id from slt_enrollment)  limit 0,1")->fetchAll();
expDebug::dPrint('Web based Course details: '.print_r($wbt_id , true), 5);
expDebug::dPrint('VODCourse details: '.print_r($vod_id , true), 5);
$vod_content=db_query("select id from slt_content_master where type='lrn_cnt_typ_vod' and status='lrn_cnt_sts_atv' limit 0,1")->fetchField();
$class_course_mismatch=db_query("Select id, course_id from slt_course_class  where delivery_type='lrn_cls_dty_wbt' and id not in (select class_id from slt_enrollment) GROUP BY course_id HAVING COUNT(id)= 1 order by id desc limit 0,1 ")->fetchAll();
expDebug::dPrint('$class_course_mismatch:: '.print_r($class_course_mismatch , true), 5);

// Types of Content
$content_scorm_2004 = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_srm_s24' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // Scorm 2004
$content_id_scorm12 = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_srm_s12' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // Scorm 1.2
$content_id_aicc = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_aic' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // AICC
$content_id_knc = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_knc' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // Knowledge Content
$content_id_vod = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_vod' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // Viedo On Demand
$content_id_tnc = db_query("select id from slt_content_master where type = 'lrn_cnt_typ_srm_tnc' and status = 'lrn_cnt_sts_atv' order by id desc limit 0,1")->fetchField(); // TinCans 

// BSG data
$class1 = db_query("select id from slt_course_class where code = 'CLSD-ELDBW-WBT-001'")->fetchField();
$course1 = db_query("select course_id from slt_course_class where id = $class1")->fetchField();
$content1 = db_query("select id from slt_content_master where code like 'Quick Course_SCORM 2004%' and type = 'lrn_cnt_typ_srm_s24'")->fetchField();

$class2 = db_query("select id from slt_course_class where code = 'CLSD-SMSFL-WBT-001'")->fetchField();
$course2 = db_query("select course_id from slt_course_class where id = $class2")->fetchField();
$content2 = db_query("select id from slt_content_master where code like 'Vado Web Content_Scorm1.2%' and type = 'lrn_cnt_typ_srm_s12'")->fetchField();

$class3 = db_query("select id from slt_course_class where code = 'CRSD-RLMKS-INTER-WBT-001'")->fetchField();
$course3 = db_query("select course_id from slt_course_class where id = $class3")->fetchField();
$content3 = db_query("select id from slt_content_master where code like 'MultipleLessons_Scorm1.2%' and type = 'lrn_cnt_typ_srm_s12'")->fetchField();

$class4 = db_query("select id from slt_course_class where code = 'CRSD-RLMKS-ADV-VOD-001'")->fetchField();
$course4 = db_query("select course_id from slt_course_class where id = $class4")->fetchField();
$content4 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class5 = db_query("select id from slt_course_class where code = 'CRSD-BMS-VOD'")->fetchField();
$course5 = db_query("select course_id from slt_course_class where id = $class5")->fetchField();
$content5 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class6 = db_query("select id from slt_course_class where code = 'CLSD-SMKPL-VOD-001'")->fetchField();
$course6 = db_query("select course_id from slt_course_class where id = $class6")->fetchField();
$content6 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class7 = db_query("select id from slt_course_class where code = 'CRSD-VEXPS-WBT-001'")->fetchField();
$course7 = db_query("select course_id from slt_course_class where id = $class7")->fetchField();
$content7 = db_query("select id from slt_content_master where code like 'Brochure_ExpertusONE Meetings & ExpertusONE Meetings - Key Highlights%' and type = 'lrn_cnt_typ_knc'")->fetchField();

$class8 = db_query("select id from slt_course_class where code = 'CRSD-LGLCM-VOD-001'")->fetchField();
$course8 = db_query("select course_id from slt_course_class where id = $class8")->fetchField();
$content8 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class9 = db_query("select id from slt_course_class where code = 'CRSD-CCV-001'")->fetchField();
$course9 = db_query("select course_id from slt_course_class where id = $class9")->fetchField();
$content9 = db_query("select id from slt_content_master where code like 'Create a Course - VC%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class10 = db_query("select id from slt_course_class where code = 'CRSD-CCS-001'")->fetchField();
$course10 = db_query("select course_id from slt_course_class where id = $class10")->fetchField();
$content10 = db_query("select id from slt_content_master where code like 'Course-Class Structure%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class11= db_query("select id from slt_course_class where code = 'CRSD-CCW-001'")->fetchField();
$course11 = db_query("select course_id from slt_course_class where id = $class11")->fetchField();
$content11 = db_query("select id from slt_content_master where code like 'Create a Course - WBT%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class12 = db_query("select id from slt_course_class where code = 'CRSD-ILT'")->fetchField();
$course12 = db_query("select course_id from slt_course_class where id = $class12")->fetchField();
$content12 = db_query("select id from slt_content_master where code like 'Create a Course - ILT%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class13 = db_query("select id from slt_course_class where code = 'CRSD-CCR-001'")->fetchField();
$course13 = db_query("select course_id from slt_course_class where id = $class13")->fetchField();
$content13 = db_query("select id from slt_content_master where code like 'How to Create a Curriculum%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class14 = db_query("select id from slt_course_class where code = 'CRSD-AGO-WBT'")->fetchField();
$course14 = db_query("select course_id from slt_course_class where id = $class14")->fetchField();
$content14 = db_query("select id from slt_content_master where code like 'Vado Web Content_Scorm1.2%' and type = 'lrn_cnt_typ_srm_s12'")->fetchField();

$class15 = db_query("select id from slt_course_class where code = 'CRSD-EONE-MOBILE-VOD'")->fetchField();
$course15 = db_query("select course_id from slt_course_class where id = $class15")->fetchField();
$content15 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class16 = db_query("select id from slt_course_class where code = 'CRSD-EONEMT-MOBILE-WBT'")->fetchField();
$course16 = db_query("select course_id from slt_course_class where id = $class16")->fetchField();
$content16 = db_query("select id from slt_content_master where code like 'Brochure_Expertusone Meetings%' and type = 'lrn_cnt_typ_knc'")->fetchField();

$class17 = db_query("select id from slt_course_class where code = 'BSH-CRSE-VOD'")->fetchField();
$course17 = db_query("select course_id from slt_course_class where id = $class17")->fetchField();
$content17 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class18 = db_query("select id from slt_course_class where code = 'BSH-CRSE-WBT'")->fetchField();
$course18 = db_query("select course_id from slt_course_class where id = $class18")->fetchField();
$content18 = db_query("select id from slt_content_master where code like 'Quick Course_SCORM 2004%' and type = 'lrn_cnt_typ_srm_s24'")->fetchField();

$class19 = db_query("select id from slt_course_class where code = 'HI-CLS-WBT'")->fetchField();
$course19 = db_query("select course_id from slt_course_class where id = $class19")->fetchField();
$content19 = db_query("select id from slt_content_master where code like 'Quick Course_SCORM 2004%' and type = 'lrn_cnt_typ_srm_s24'")->fetchField();

$class20 = db_query("select id from slt_course_class where code = 'AHI-CRSE-VOD'")->fetchField();
$course20 = db_query("select course_id from slt_course_class where id = $class20")->fetchField();
$content20 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class21 = db_query("select id from slt_course_class where code = 'AHI-CRSE-WBT'")->fetchField();
$course21 = db_query("select course_id from slt_course_class where id = $class21")->fetchField();
$content21 = db_query("select id from slt_content_master where code like 'Quick Course_SCORM 2004%' and type = 'lrn_cnt_typ_srm_s24'")->fetchField();

$class22 = db_query("select id from slt_course_class where code = 'BAF-CRSE-VOD'")->fetchField();
$course22 = db_query("select course_id from slt_course_class where id = $class22")->fetchField();
$content22 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class23 = db_query("select id from slt_course_class where code = 'BAF-CRSE-WBT'")->fetchField();
$course23 = db_query("select course_id from slt_course_class where id = $class23")->fetchField();
$content23 = db_query("select id from slt_content_master where code like 'Brochure_ExpertusONE Meetings%' and type = 'lrn_cnt_typ_knc'")->fetchField();

$class24 = db_query("select id from slt_course_class where code = 'ERA-CRSE-WBT'")->fetchField();
$course24 = db_query("select course_id from slt_course_class where id = $class24")->fetchField();
$content24 = db_query("select id from slt_content_master where code like 'MultipleLessons_Scorm1.2%' and type = 'lrn_cnt_typ_srm_s12'")->fetchField();

$class25 = db_query("select id from slt_course_class where code = 'WAB-CRSE-VOD'")->fetchField();
$course25 = db_query("select course_id from slt_course_class where id = $class25")->fetchField();
$content25 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class26 = db_query("select id from slt_course_class where code = 'WAB-CRSE-VOD_1'")->fetchField();
$course26 = db_query("select course_id from slt_course_class where id = $class26")->fetchField();
$content26 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class27 = db_query("select id from slt_course_class where code = 'DBS-CLS-WBT'")->fetchField();
$course27 = db_query("select course_id from slt_course_class where id = $class27")->fetchField();
$content27 = db_query("select id from slt_content_master where code like 'Quick Course_SCORM 2004%' and type = 'lrn_cnt_typ_srm_s24'")->fetchField();

$class28 = db_query("select id from slt_course_class where code = 'ITSPP-CLS-VOD'")->fetchField();
$course28 = db_query("select course_id from slt_course_class where id = $class28")->fetchField();
$content28 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class29 = db_query("select id from slt_course_class where code = 'EIS-CRSE-VOD'")->fetchField();
$course29 = db_query("select course_id from slt_course_class where id = $class29")->fetchField();
$content29 = db_query("select id from slt_content_master where code like 'Mobile Presence Sensing Video%' and type = 'lrn_cnt_typ_vod'")->fetchField();

$class30 = db_query("select id from slt_course_class where code = 'EIS-CRSE-WBT'")->fetchField();
$course30 = db_query("select course_id from slt_course_class where id = $class29")->fetchField();
$content30 = db_query("select id from slt_content_master where code like 'Globex SCORM Colors Course - Tin Can%' and type = 'lrn_cnt_typ_srm_tnc'")->fetchField();






$testCases = array(
		'AssociateContentwithWBTClassAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'CourseId'=>$course_id,'ClassId'=>$class_id,'ContentId'=>$wbt_content),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "">'),
						   'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'CourseId'=>$course1,'ClassId'=>$class1,'ContentId'=>$content1),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course2,'ClassId'=>$class2,'ContentId'=>$content2),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course3,'ClassId'=>$class3,'ContentId'=>$content3),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course4,'ClassId'=>$class4,'ContentId'=>$content4),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course5,'ClassId'=>$class5,'ContentId'=>$content5),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course6,'ClassId'=>$class6,'ContentId'=>$content6),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course7,'ClassId'=>$class7,'ContentId'=>$content7),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course8,'ClassId'=>$class8,'ContentId'=>$content8),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course9,'ClassId'=>$class9,'ContentId'=>$content9),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course10,'ClassId'=>$class10,'ContentId'=>$content10),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course11,'ClassId'=>$class11,'ContentId'=>$content11),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course12,'ClassId'=>$class12,'ContentId'=>$content12),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course13,'ClassId'=>$class13,'ContentId'=>$content13),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course14,'ClassId'=>$class14,'ContentId'=>$content14),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course15,'ClassId'=>$class15,'ContentId'=>$content15),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course16,'ClassId'=>$class16,'ContentId'=>$content16),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course17,'ClassId'=>$class17,'ContentId'=>$content17),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course18,'ClassId'=>$class18,'ContentId'=>$content18),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course19,'ClassId'=>$class19,'ContentId'=>$content19),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course20,'ClassId'=>$class20,'ContentId'=>$content20),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course21,'ClassId'=>$class21,'ContentId'=>$content21),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course22,'ClassId'=>$class22,'ContentId'=>$content22),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course23,'ClassId'=>$class23,'ContentId'=>$content23),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course24,'ClassId'=>$class24,'ContentId'=>$content24),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course25,'ClassId'=>$class25,'ContentId'=>$content25),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course26,'ClassId'=>$class26,'ContentId'=>$content26),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course27,'ClassId'=>$class27,'ContentId'=>$content27),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course28,'ClassId'=>$class28,'ContentId'=>$content28),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course29,'ClassId'=>$class29,'ContentId'=>$content29),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'CourseId'=>$course30,'ClassId'=>$class30,'ContentId'=>$content30),
								'output'=>array('id'),
								'result'=>array('<results totalrecords = "">'),
								'output_type'=>'xml'
						)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1,'ClassId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'CourseId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ContentId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'CourseId'=>$class_details_enr[0]->course_id,'ClassId'=>$class_details_enr[0]->class_id,'ContentId'=>$wbt_content),
						       'output'=>array(),
							   'result'=>array('Cannot associate content since active enrollment available for the class'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'CourseId'=>$class_course_mismatch[0]->course_id,'ClassId'=>$class_course_mismatch[0]->id+1,'ContentId'=>$wbt_content),
						       'output'=>array(),
							   'result'=>array('Class is not attached in this Course.'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'CourseId'=>$class_details_notwbt[0]->course_id,'ClassId'=>$class_details_notwbt[0]->id,'ContentId'=>$wbt_content),
						       'output'=>array(),
							   'result'=>array('Invalid class'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'CourseId'=>$course_id,'ClassId'=>$class_id,'ContentId'=>$wbt_content,'MaxAttempts'=>0),
						  'output'=>array(),
						  'result'=>array('MaxAttempts should be greater than 0 or Numeric'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'CourseId'=>$course_id,'ClassId'=>$class_id,'ContentId'=>$wbt_content,'MaxAttempts'=>5,'ValidityDays'=>aa),
						  'output'=>array(),
						  'result'=>array('ValidityDays should be greater than 0 or Numeric'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'CourseId'=>$course_id,'ClassId'=>$class_id,'ContentId'=>$content_id_invalid),
						  'output'=>array(),
						  'result'=>array('Invalid content'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'CourseId'=>$already_added_details[0]->course_id,'ClassId'=>$already_added_details[0]->class_id,'ContentId'=>$already_added_details[0]->content_id),
						  'output'=>array(),
						  'result'=>array('Already added this content'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'CourseId'=>$wbt_id[0]->course_id,'ClassId'=>$wbt_id[0]->id,'ContentId'=>$vod_content),
						  'output'=>array(),
						  'result'=>array('The selected class is a wbt class and cannot attach a video content'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'CourseId'=>$vod_id[0]->course_id,'ClassId'=>$vod_id[0]->id,'ContentId'=>$wbt_content),
						  'output'=>array(),
						  'result'=>array('The selected class is a vod class and cannot attach a web based content'),
						   'output_type'=>'xml'
							),
						
						
						
					
						
						
						
						
						
				
				)
			)
		);
			
?>		