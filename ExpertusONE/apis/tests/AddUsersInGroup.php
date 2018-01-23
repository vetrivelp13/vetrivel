<?php 
$user_name1 = db_query("select user_name from slt_person where full_name = 'abhirami reddy'")->fetchField();
$user_name2 = db_query("select user_name from slt_person where full_name = 'sukesh reddy'")->fetchField();
$user_name3 = db_query("select user_name from slt_person where full_name = 'bhanu theja'")->fetchField();
$user_name4 = db_query("select user_name from slt_person where full_name = 'sumana reddy'")->fetchField();
$user_name5 = db_query("select user_name from slt_person where full_name = 'sakthi vel'")->fetchField();
$user_name6 = db_query("select user_name from slt_person where full_name = 'nithya sri'")->fetchField();
$user_name7 = db_query("select user_name from slt_person where full_name = 'rahul reddy'")->fetchField();
$user_name8 = db_query("select user_name from slt_person where full_name = 'keerthi priya'")->fetchField();
$user_name9 = db_query("select user_name from slt_person where full_name = 'sobitha reddy'")->fetchField();
$user_name10 = db_query("select user_name from slt_person where full_name = 'arun kumar'")->fetchField();

$testCases = array(

			// AddUsersInGroup test case Starts
			'AddUsersInGroup' => array(
				'datasource' => array(
					array('input'=>array('group_name'=>'api group','user_name'=>'user1','add_or_remove'=>'Add','userid'=>1),
						  'output'=>array('Id'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'api group','user_name'=>'user1','add_or_remove'=>'remove','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name1,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name2,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name3,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name4,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name5,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name6,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name7,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name8,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name9,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'users group','user_name'=>$user_name10,'add_or_remove'=>'add','userid'=>1),
							'output'=>array('Id'),
							'result'=>array('<results totalrecords =','<result>','<Id>'),
							'output_type'=>'xml'
							)
						
				),
				'validate' => array(
 					array('input'=>array('group_name'=>'','user_name'=>'user1','add_or_remove'=>'Add','userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('group_name'=>'api group','user_name'=>'','add_or_remove'=>'Add','userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('group_name'=>'api group','user_name'=>'user1','add_or_remove'=>'','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							)
				)
			),  // AddUsersInGroup test case Ends
			
		); // End of api test case main array
?>