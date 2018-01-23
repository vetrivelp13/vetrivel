<?php 
$learnerId=db_query("select user_id from slt_enrollment order by user_id desc limit 1")->fetchField();
$instructorId=db_query("select instructor_id from slt_session_instructor_details order by instructor_id desc limit 1")->fetchField();
$courseId=db_query("select course_id from slt_course_class limit 0,1")->fetchField();
$classId=db_query("select id from slt_course_class limit 0,1")->fetchField();
$deliveryType=db_query("select delivery_type from slt_course_class limit 0,1")->fetchField();

$chkNotEnrollUsr=db_query("select id from slt_person where id not in (select user_id from slt_enrollment where class_id in(select max(id) from slt_course_class))order by id desc limit 1 ")->fetchField();

$checkPastDate = db_query('select cls.id ,cls.course_id,cls.delivery_type,sess.start_date from slt_course_class cls left join slt_course_class_session sess on sess.course_id = cls.course_id and sess.class_id = cls.id and price > 0  
where TIMESTAMP(sess.start_date,sess.end_time) <= NOW() order by start_date asc limit 1')->fetchAll();

$checkRegCls=db_query("select  per.id, enr.user_id, cls.id, cls.course_id,  cls.delivery_type, cls.title  FROM  slt_enrollment enr  LEFT JOIN slt_person per  ON enr.user_id = per.id  LEFT JOIN slt_course_class cls
ON enr.class_id = cls.id and cls.price > 0 order by enr.user_id desc limit 1")->fetchAll();

$addedToCart=db_query("select  per.id, enr.user_id, cls.id, cls.course_id,  cls.delivery_type, cls.title  FROM  slt_enrollment enr  LEFT JOIN slt_person per  ON enr.user_id != per.id  LEFT JOIN slt_course_class cls
ON enr.class_id != cls.id and cls.price > 0 order by enr.user_id asc limit 1")->fetchAll();

$chkConflictCls=db_query("select per.id, cls.delivery_type, sess.class_id, sess.course_id, ses_ins.instructor_id from 
slt_course_class_session sess
LEFT OUTER JOIN slt_session_instructor_details ses_ins ON ses_ins.session_id = sess.id
LEFT OUTER JOIN slt_course_class cls ON cls.id = sess.class_id
LEFT OUTER JOIN slt_person per ON per.id = ses_ins.instructor_id
LEFT OUTER JOIN slt_profile_list_items splt_timezone ON splt_timezone.code=sess.timezone
WHERE cls.price > 0 and (cls.status NOT IN  ('lrn_cls_sts_dld', 'lrn_cls_sts_del', 'lrn_cls_sts_can')) OR (ses_ins.instructor_id = per.id) order by class_id desc")->fetchAll();

$chkInstnotreg=db_query("select per.id, inst.class_id, inst.course_id, cls.delivery_type from slt_session_instructor_details inst
left join slt_course_class cls on (cls.delivery_type='lrn_cls_dty_vcl' or cls.delivery_type='lrn_cls_dty_ilt') and cls.price > 0
left join slt_course_class_session sess on sess.course_id = cls.course_id and sess.class_id = cls.id
left join slt_person per on per.id = inst.instructor_id
where TIMESTAMP(sess.start_date,sess.end_time) >= NOW() order by start_date asc limit 1")->fetchAll();

$chkseatsavail=db_query("SELECT group_concat(user_id) AS userid,count(ses.capacity_max) AS class_max_capacity, ses.waitlist_count AS class_waitlist_count, COUNT(enr.user_id) AS user_id , enr.course_id, enr.class_id, cls.delivery_type
FROM 
slt_course_class_session ses
left join slt_enrollment enr on ses.course_id = enr.course_id and ses.class_id = enr.class_id
left join slt_course_class cls on cls.id = enr.class_id and (cls.delivery_type='lrn_cls_dty_vcl' or cls.delivery_type='lrn_cls_dty_ilt' )
WHERE cls.price > 0
GROUP BY class_id
HAVING COUNT(ses.capacity_max) = COUNT(enr.user_id) order by cls.id desc")->fetchAll();

$chkWaitlist=db_query("SELECT group_concat(user_id) AS userid,count(ses.capacity_max) AS class_max_capacity, ses.waitlist_count AS class_waitlist_count, COUNT(enr.user_id) AS user_id , enr.course_id, enr.class_id, cls.delivery_type
FROM 
slt_course_class_session ses
left join slt_enrollment enr on ses.course_id = enr.course_id and ses.class_id = enr.class_id
left join slt_course_class cls on cls.id = enr.class_id and (cls.delivery_type='lrn_cls_dty_vcl' or cls.delivery_type='lrn_cls_dty_ilt' )
WHERE cls.price > 0
GROUP BY class_id
HAVING COUNT(enr.user_id) >= COUNT(ses.capacity_max) order by cls.id desc")->fetchAll();

//$chkwaitlist=db_query("")->fetchAll();

/* expDebug::dPrint('Sho checking --->1'.print_r($checkPastDate,true));
expDebug::dPrint('$$courseId'.$courseId);
expDebug::dPrint('$$classId'.$classId);
expDebug::dPrint('$$deliveryType'.$deliveryType);
expDebug::dPrint('$$$sessiondetails'.$sessiondetails); */
expDebug::dPrint('$$$$$addedToCart'.$addedToCart);
$testCases = array(
//addClassToCartAPI test case Start
		'addClassToCartAPI' =>array(
		'datasource' => array(
						array('input'=>array('user_id'=>$learnerId,'Courseid'=>$courseId,'Classid'=>$classId,'deliveryType'=>$deliveryType,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<results totalrecords =','<result>','<status>'),
								'output_type'=>'xml'
								)
						),
				'validate'=>array(
						 array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Classid'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$courseId,'Classid'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$courseId,'Classid'=>$classId,'deliveryType'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$courseId,'Classid'=>$classId,'deliveryType'=>$deliveryType,'user_id'=>''),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>'','Courseid'=>$courseId,'Classid'=>$classId,'deliveryType'=>$deliveryType,'user_id'=>$learnerId),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$checkPastDate[0]->course_id,'Classid'=>$checkPastDate[0]->id,'deliveryType'=>$checkPastDate[0]->delivery_type,'user_id'=>$learnerId),
								'output'=>array(),
								'result'=>array('<short_msg>Class has been delivered</short_msg>'),
								'output_type'=>'xml'
						),						 
						array('input'=>array('userid'=>1,'Courseid'=>$checkRegCls[0]->course_id,'Classid'=>$checkRegCls[0]->id,'deliveryType'=>$checkRegCls[0]->delivery_type,'user_id'=>$checkRegCls[0]->user_id),
								'output'=>array('status'),
								'result'=>array('<status>You have already registered for this class</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$courseId,'Classid'=>vv,'deliveryType'=>$deliveryType,'user_id'=>$learnerId),
								'output'=>array('status'),
								'result'=>array('<status>Invalid user.</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$courseId,'Classid'=>$classId,'deliveryType'=>$deliveryType,'user_id'=>vv),
								'output'=>array('status'),
								'result'=>array('<status>Invalid user.</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>vv,'Classid'=>$classId,'deliveryType'=>$deliveryType,'user_id'=>$learnerId),
								'output'=>array('status'),
								'result'=>array('<status>Invalid class.</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$addedToCart[0]->course_id,'Classid'=>$addedToCart[0]->id,'deliveryType'=>$addedToCart[0]->delivery_type,'user_id'=>$addedToCart[0]->user_id),
								'output'=>array('status'),
								'result'=>array('<status>CartAdded</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$chkInstnotreg[0]->course_id,'Classid'=>$chkInstnotreg[0]->class_id,'deliveryType'=>$chkInstnotreg[0]->delivery_type,'user_id'=>$chkInstnotreg[0]->id),
								'output'=>array('status'),
								'result'=>array('<status>You cannot register as you are the instructor for this class.</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$chkConflictCls[0]->course_id,'Classid'=>$chkConflictCls[0]->class_id,'deliveryType'=>$chkConflictCls[0]->delivery_type,'user_id'=>$chkConflictCls[0]->id),
								'output'=>array('status'),
								'result'=>array('<status>You have a conflicting class.</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Courseid'=>$chkseatsavail[0]->course_id,'Classid'=>$chkseatsavail[0]->class_id,'deliveryType'=>$chkseatsavail[0]->delivery_type,'user_id'=>$chkNotEnrollUsr),
								'output'=>array('status'),
								'result'=>array('<status>No seats available.</status>'),
								'output_type'=>'xml'
						) ,
						array('input'=>array('userid'=>1,'Courseid'=>$chkWaitlist[0]->course_id,'Classid'=>$chkWaitlist[0]->class_id,'deliveryType'=>$chkWaitlist[0]->delivery_type,'user_id'=>$chkNotEnrollUsr),
								'output'=>array('status'),
								'result'=>array('<status>waitlisted</status>'),
								'output_type'=>'xml'
						)
						
						
						
				)		
				
		),// addClassToCartAPI test case Ends
);

?>