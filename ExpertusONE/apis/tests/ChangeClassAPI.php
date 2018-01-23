<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$enroll_id = db_query("select id from slt_enrollment order by id desc")->fetchField();
$course_id = db_query("select course_id from slt_enrollment where id =:id",array(":id"=>$enroll_id))->fetchField();
$oldclassid = db_query("select id from slt_enrollment where id =:id and course_id=:course_id",array(":id"=>$enroll_id,":course_id"=>$course_id))->fetchField();
$newclass_id = db_query("select id from slt_course_class where course_id=:course_id and id != :cls_id",array(":course_id"=>$course_id,":cls_id"=>$oldclassid))->fetchField();
$UserId = db_query("select user_id from slt_enrollment where id =:id",array(":id"=>$enroll_id))->fetchField();

//$max_seats=db_query("select max_seats from slt_course_class where delivery_type='lrn_cls_dty_ilt' and id =:id",array(":id"=>$newclass_id))->fetchField();

$testCases = array(

		// ChangeClassAPI API test case Starts
		'ChangeClassAPI' => array(
				'datasource' => array(
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>$newclass_id,'TP_or_class'=>'class','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<results totalrecords =','<status>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>$newclass_id,'TP_or_class'=>'class','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<results totalrecords =','<status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('UserId'=> $UserId,'enrollid'=>'','Classid' =>$newclass_id,'TP_or_class'=>'class','userid'=>1),
								'output'=>array(),
								'result'=>array('<short_msg> Mandatory Fields are missing in the feed. List provided below:</short_msg><long_msg>1.enrollid'),
								'output_type'=>'xml'
						),
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>'','TP_or_class'=>'class','userid'=>1),
								'output'=>array(),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg><long_msg>1.Classid'),
								'output_type'=>'xml'
						),
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>$newclass_id,'TP_or_class'=>'','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg><long_msg>1.TP_or_class'),
								'output_type'=>'xml'
						),
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>$random,'TP_or_class'=>'class','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Class Id does not belongs to same course</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('UserId'=> $UserId,'enrollid'=>$enroll_id,'Classid' =>$newclass_id,'TP_or_class'=>'class','userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
						)
				)
		),  // ChangeClassAPI API test case Ends
			
); // End of api test case main array
?>