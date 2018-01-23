<?php 
$user_id = db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$c_id= db_query("select id,course_id from slt_course_class order by id desc limit 0,1;")->fetchAll();
$a = $c_id[0]->id;
$b = $c_id[0]->course_id;
//expDebug::dPrint('$learnerid'.$c_id[]);
expDebug::dPrint('$cllass_id'.print_r($c_id,true),5);
expDebug::dPrint('value of a '.$a);
expDebug::dPrint('value of b '.$b);
$testCases = array(

			// RegistrationAPI test case Starts
			'RegistrationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>$b,'Classid'=>$a),
						  //'output'=>array('id','status','launchdetails'),
						  'output'=>array('id','status'),
						  'result'=>array('<results totalrecords =','<result>','<id>','<status>Reserved</status>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>$b,'Classid'=>$a),
						  'output'=>array('id','status'),
						  'result'=>array('<results totalrecords =','<result>','<id>','<status>AlreadyEnrolled</status>'),
						  'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'Learnerid'=>'','Courseid'=>$b,'Classid'=>$a),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>'','Classid'=>$a),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>$b,'Classid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>$b,'Classid'=>3),
								'output'=>array('id','status'),
								'result'=>array('<results totalrecords =','<result>','<id>','</id>','<status>Invalid</status>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'Learnerid'=>$user_id,'Courseid'=>71,'Classid'=>$a),
								'output'=>array('id','status'),
								'result'=>array('<results totalrecords =','<result>','<id>','</id>','<status>Invalid</status>'),
								'output_type'=>'xml'
						),
						
				   
				)
			),  // RegistrationAPI test case Ends
			
		); // End of api test case main array
?>