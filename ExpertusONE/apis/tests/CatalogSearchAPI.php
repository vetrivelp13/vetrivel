<?php 

$coursedetail = db_query("select id,code,lang_code  from slt_course_template where status = 'lrn_crs_sts_atv' order by id desc limit 1 ")->fetchAll();
expDebug::dPrint("Vincen test -- ".print_r($coursedetail,true),1);
$classdetails =  db_query("select id,code,delivery_type,lang_code  from slt_course_class where status = 'lrn_cls_sts_atv' order by id desc limit 1 ")->fetchAll();
$testCases = array(

			
		'CatalogSearchAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course"),
								'output'=>array('object_type','crs_id','cls_id','cls_code','cls_title','cls_short_description','delivery_type_code','delivery_type_name','status','language','language_code','location',
										'node_id','base_price','base_currency_type','base_currency_symbol','currency_type','currency_symbol','price','avgvote','country_name','country_code','prm_end_date','mro_id','mro','registration_end_on',
										'registration_ended','sessioninfo','prerequisites','enrolled_id','waitlist_status','rating','available_seats','waitlist_seats','class_status','tp_courses','class_list','is_compliance','cls_count','is_cart_added','launchdetails','AssessmentStatus','waived_status'),
								'result'=>array('<results totalrecords =','<object_type>','<crs_id>','<cls_id>','<cls_code>','<cls_title>','<cls_short_description>','<delivery_type_code>','<delivery_type_name>','<status>','<language>','<language_code>','<location>',
										'<node_id>','<base_price>','<base_currency_type>','<base_currency_symbol>','<currency_type>','<currency_symbol>','<price>','<avgvote>','<country_name>','<country_code>','<prm_end_date>','<mro_id>','<mro>','<registration_end_on>',
										'<registration_ended>','<sessioninfo>','<prerequisites>','<enrolled_id>','<waitlist_status>','<rating>','<available_seats>','<waitlist_seats>','<class_status>','<tp_courses>','<class_list>','<is_compliance>','<cls_count>','<is_cart_added>','<launchdetails>','<AssessmentStatus>','<waived_status>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class"),
								'output'=>array('object_type','crs_id','cls_id','cls_code','cls_title','cls_short_description','delivery_type_code','delivery_type_name','status','language','language_code','location',
										'node_id','base_price','base_currency_type','base_currency_symbol','currency_type','currency_symbol','price','avgvote','country_name','country_code','prm_end_date','mro_id','mro','registration_end_on',
										'registration_ended','sessioninfo','prerequisites','enrolled_id','waitlist_status','rating','available_seats','waitlist_seats','class_status','tp_courses','class_list','is_compliance','cls_count','is_cart_added','launchdetails','AssessmentStatus','waived_status'),
								'result'=>array('<results totalrecords =','<object_type>','<crs_id>','<cls_id>','<cls_code>','<cls_title>','<cls_short_description>','<delivery_type_code>','<delivery_type_name>','<status>','<language>','<language_code>','<location>',
										'<node_id>','<base_price>','<base_currency_type>','<base_currency_symbol>','<currency_type>','<currency_symbol>','<price>','<avgvote>','<country_name>','<country_code>','<prm_end_date>','<mro_id>','<mro>','<registration_end_on>',
										'<registration_ended>','<sessioninfo>','<prerequisites>','<enrolled_id>','<waitlist_status>','<rating>','<available_seats>','<waitlist_seats>','<class_status>','<tp_courses>','<class_list>','<is_compliance>','<cls_count>','<is_cart_added>','<launchdetails>','<AssessmentStatus>','<waived_status>'),
								'type'=>'DataSource','output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => ""),
								'output'=>array('object_type'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'course_id'=>$coursedetail[0]->id),
								'output'=>array('object_type','cls_id','cls_code','crs_id'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>'.$coursedetail[0]->id.'</cls_id>','<cls_code>'.$coursedetail[0]->code.'</cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'title'=>$coursedetail[0]->code),
								'output'=>array('object_type','cls_id','cls_code','crs_id'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'title'=>$classdetails[0]->code),
								'output'=>array('object_type','cls_id','cls_code','crs_id'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'title'=>$classdetails[0]->code),
								'output'=>array('object_type','cls_id','cls_code','crs_id'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'dl_type'=>$classdetails[0]->delivery_type),
								'output'=>array('object_type','cls_id','cls_code','crs_id','delivery_type_code'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>','<delivery_type_code>'.$classdetails[0]->delivery_type.'</delivery_type_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'lg_type'=>$classdetails[0]->lang_code),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>','<language_code>'.$classdetails[0]->lang_code.'</language_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'lg_type'=>$coursedetail[0]->lang_code),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>','<language_code>'.$coursedetail[0]->lang_code.'</language_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'sortby'=>'AZ'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'sortby'=>'ZA'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'sortby'=>'dateOld'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'sortby'=>'type'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'sortby'=>'type'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Class",'sortby'=>'dateOld'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Class</object_type>','<cls_id>','<cls_code>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'ob_type'=>'cre_sys_obt_cur'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<cls_id>','<cls_code>','<object_type>cre_sys_obt_cur</object_type>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'ob_type'=>'cre_sys_obt_crt'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<cls_id>','<cls_code>','<object_type>cre_sys_obt_crt</object_type>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'ob_type'=>'cre_sys_obt_trn'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<cls_id>','<cls_code>','<object_type>cre_sys_obt_trn</object_type>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'rating_type'=>'100'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>','<rating>100</rating>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'object_type' => "Course",'compliance_mandatory'=>'1'),
								'output'=>array('object_type','cls_id','cls_code','crs_id','language_code'),
								'result'=>array('<object_type>Course</object_type>','<cls_id>','<cls_code>','<is_compliance>1</is_compliance>'),
								'type'=>'DataSource','output_type'=>'xml'
						),

							
				)
		),
			
);
?>