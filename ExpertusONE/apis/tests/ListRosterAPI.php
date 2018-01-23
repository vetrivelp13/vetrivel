<?php
$course_id=db_query("select course_id from slt_enrollment limit 0,1")->fetchField();
$class_id=db_query("select class_id from slt_enrollment limit 0,1")->fetchField();
$reg_date= db_query("select left(reg_date,10) from slt_enrollment where course_id=$course_id and class_id=$class_id limit 0,1")->fetchField();
$qry="select id, user_id,reg_status,comp_status,reg_status_date,score,comp_date,updated_on from slt_enrollment where course_id= $course_id and class_id= $class_id";
$enrollmentdetails=db_query($qry)->fetchAll();
expDebug::dPrint("Latha test --enrollmentdetails--- ".print_r($enrollmentdetails,true),1);
$qry2="select user_name,full_name from slt_person where id in(select user_id from slt_enrollment where course_id= $course_id and class_id= $class_id)";
$userdetails=db_query($qry2)->fetchAll();
expDebug::dPrint("Latha test --userdetails--- ".print_r($userdetails,true),1);
$qry3="select delivery_type from slt_course_class where id = $class_id and course_id= $course_id";
$classtype=db_query($qry3)->fetchAll();
expDebug::dPrint("Latha test --classtype--- ".print_r($classtype,true),1);
$usercount=count($userdetails);
expDebug::dPrint("Latha test -- usercount---".$usercount);
$qry4="select name from slt_profile_list_items where code in (select reg_status from slt_enrollment where course_id= $course_id and class_id= $class_id )";
$regstatusname=db_query($qry4)->fetchfield();
expDebug::dPrint("Latha test -- regstatusname---".print_r($regstatusname,true),1);
$qry5="select name from slt_profile_list_items where code in (select comp_status from slt_enrollment where course_id= $course_id and class_id= $class_id)";
$compstatusname=db_query($qry5)->fetchfield();
expDebug::dPrint("Latha test -- compstatusname---".print_r($compstatusname,true),1);
$qr6="select id from slt_enrollment where course_id=$course_id and class_id=$class_id  and left(reg_date,10)='2016-07-18'";
$useridDate= db_query($qr6)->fetchAll();
$userCountDate=count($useridDate);
expDebug::dPrint("$userCountDate".$userCountDate);


$testCases = array(
		'ListRosterAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,),
						  'output'=>array('id','registered_user_id','user_name','full_name','delivery_type','reg_status','reg_status_name','comp_status','comp_status_name','score','reg_status_date','comp_date','updated_on'),
						  'result'=>array('<results totalrecords =','<id>','<registered_user_id>','<user_name>','<full_name>','<delivery_type>','<reg_status>','<reg_status_name>','<comp_status>','<comp_status_name>','<score>','<reg_status_date>','<comp_date>','<updated_on>'),
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
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'UserName'=>$userdetails[0]->user_name),
						      'output'=>array('id','registered_user_id','user_name','full_name','delivery_type','reg_status','reg_status_name','comp_status','comp_status_name','score','reg_status_date','comp_date','updated_on'),
						       'result'=>array('<results totalrecords = "1">','<id>','<registered_user_id>','<user_name>','<full_name>','<delivery_type>','<reg_status>','<reg_status_name>','<comp_status>','<comp_status_name>','<score>','<reg_status_date>','<comp_date>','<updated_on>'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'IncludeProfileData'=>123),
						      'result'=>array('IncludeProfileData Value should be either 1 or 0'), 
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'IncludeProfileData'=>0,'date_from'=>07-01-2015),
						      'result'=>array('From date should be in yyyy-mm-dd format'), 
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'IncludeProfileData'=>0,'date_to'=>07-01-2015),
						      'result'=>array('To date should be in yyyy-mm-dd format'), 
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'IncludeProfileData'=>0),
						       'output'=>array('id','registered_user_id','user_name','full_name','delivery_type','reg_status','reg_status_name','comp_status','comp_status_name','score','reg_status_date','comp_date','updated_on'),
							   'result'=>array('<results totalrecords = "'.$usercount.'">','<id>'.$enrollmentdetails[0]->id.'</id>','<registered_user_id>'.$enrollmentdetails[0]->user_id.'</registered_user_id>','<user_name>'.$userdetails[0]->user_name.'</user_name>','<full_name>'.$userdetails[0]->full_name.'</full_name>','<delivery_type>'.$classtype[0]->delivery_type.'</delivery_type>','<reg_status>'.$enrollmentdetails[0]->reg_status.'</reg_status>','<reg_status_name>'.$regstatusname.'</reg_status_name>','<comp_status>'.$enrollmentdetails[0]->comp_status.'</comp_status>','<comp_status_name>'.$compstatusname.'</comp_status_name>','<score>'.$enrollmentdetails[0]->score.'</score>','<reg_status_date>'.$enrollmentdetails[0]->reg_status_date.'</reg_status_date>','<comp_date>'.$enrollmentdetails[0]->comp_date.'</comp_date>','<updated_on>'.$enrollmentdetails[0]->updated_on.'</updated_on>'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_id,'CourseId'=>$course_id,'IncludeProfileData'=>0,'date_from'=>$reg_date),
						       'output'=>array('id','registered_user_id','user_name','full_name','delivery_type','reg_status','reg_status_name','comp_status','comp_status_name','score','reg_status_date','comp_date','updated_on'),
							   'result'=>array('<results totalrecords = "'.$userCountDate.'">','<id>'.$enrollmentdetails[0]->id.'</id>','<registered_user_id>'.$enrollmentdetails[0]->user_id.'</registered_user_id>','<user_name>'.$userdetails[0]->user_name.'</user_name>','<full_name>'.$userdetails[0]->full_name.'</full_name>','<delivery_type>'.$classtype[0]->delivery_type.'</delivery_type>','<reg_status>'.$enrollmentdetails[0]->reg_status.'</reg_status>','<reg_status_name>'.$regstatusname.'</reg_status_name>','<comp_status>'.$enrollmentdetails[0]->comp_status.'</comp_status>','<comp_status_name>'.$compstatusname.'</comp_status_name>','<score>'.$enrollmentdetails[0]->score.'</score>','<reg_status_date>'.$enrollmentdetails[0]->reg_status_date.'</reg_status_date>','<comp_date>'.$enrollmentdetails[0]->comp_date.'</comp_date>','<updated_on>'.$enrollmentdetails[0]->updated_on.'</updated_on>'),
							   'output_type'=>'xml'
							 ),
						
						
						
						
				
				)
			)
		);
			
?>		