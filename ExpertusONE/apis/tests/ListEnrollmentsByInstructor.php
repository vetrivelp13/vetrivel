<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();
$class = db_query("select class_id from slt_enrollment se join slt_person sp on se.updated_by=sp.id and sp.is_instructor='Y' order by se.id desc limit 1")->fetchField();
expDebug::dPrint(" Checking ".print_r($class));
$testCases = array(

			
		'ListEnrollmentsByInstructor' => array(
				'datasource' => array(
						array('input'=>array('classid'=>$class,'userid'=>1,'limit'=>10),
								'output'=>array('enrollid','learnerid','masterenrollid','status','compstatus','classtitle','fullname','name','username',
										'jobtitle','email','phone_no','mobile_no','mandatory','enrolleddate','statustext','compstatustext',
										'enrolledcount','profileimage_path','surveyStatus','assessmentStatus','score','max_score','attendance','reached'
								),
								'result'=>array('<results totalrecords =','<enrollid>','<learnerid>','<masterenrollid>','<status>','<compstatus>','<classtitle>','<fullname>','<name>','<username>',
										'<jobtitle>','<email>','<phone_no>','<mobile_no>','<mandatory>','<enrolleddate>','<statustext>','<compstatustext>',
										'<enrolledcount>','<profileimage_path>','<surveyStatus>','<assessmentStatus>','<score>','<max_score>','<attendance>','<reached>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('classid'=>$class,'userid'=>1,'limit'=>10),
								'output'=>array('enrollid','learnerid','masterenrollid','status','compstatus','classtitle','fullname','name','username',
										'jobtitle','email','phone_no','mobile_no','mandatory','enrolleddate','statustext','compstatustext',
										'enrolledcount','profileimage_path','surveyStatus','assessmentStatus','score','max_score','attendance','reached'),
								'result'=>array('jsonResponse','enrollid','learnerid','masterenrollid','status','compstatus','classtitle','fullname','name','username',
										'jobtitle','email','phone_no','mobile_no','mandatory','enrolleddate','statustext','compstatustext',
										'enrolledcount','profileimage_path','surveyStatus','assessmentStatus','score','max_score','attendance','reached'),
								'output_type'=>'json'
						),
					),
			// Mandatory fields missing	
				'validate' => array(
						array('input'=>array('classid'=>'','userid'=>1,'limit'=>10),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','classid'),
								'output_type'=>'xml'
						),
						array('input'=>array('classid'=>$class,'userid'=>1,'limit'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','limit'),
								'output_type'=>'xml'
						),
			//Invalid data type.
						array('input'=>array('classid'=>$class,'userid'=>1,'limit'=>10,'date_from'=>$random_word),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','From date should be in yyyy-mm-dd format','From date should be in yyyy-mm-dd format'),
								'output_type'=>'xml'
						),
						array('input'=>array('classid'=>$class,'userid'=>1,'limit'=>10,'date_to'=>$random_word),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','To date should be in yyyy-mm-dd format','To date should be in yyyy-mm-dd format'),
								'output_type'=>'xml'
						),
			// If random value is given to 'classid', it throws "Website encountered an unexpected error"			
						array('input'=>array('classid'=>$random_word,'userid'=>1,'limit'=>10),
								'output'=>array(),
								'result'=>array('<error>','<error_code></error_code>','Website encountered an unexpected error','Website encountered an unexpected error'),
								'output_type'=>'xml'
						),
						
			)
	)
 )
?>
		
			