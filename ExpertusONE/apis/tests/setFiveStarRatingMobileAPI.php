<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $star = db_query("select * from slt_node_learning_activity where entity_type='cre_sys_obt_cls' order by id DESC limit 1 ")->fetchAll();
	$usrID = db_query("select user_id from slt_enrollment_content_mapping where class_id = :clsID order by id DESC limit 1 ",array(':clsID' => $star[0]->id))->fetchField();
	
	//expDebug::dPrint(" Checking ".print_r($star));

 $testCases = array(
			'setFiveStarRatingMobileAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>"cre_sys_obt_cls"),
						 'output'=>array('average_rating','vote_value','vote_time','classId','entity_type','no_of_rating'),
						 'result'=>array('<average_rating>','<vote_value>','<vote_time>','<classId>','<entity_type>','<no_of_rating>'),
						  'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>"cre_sys_obt_cls"), 
						 'output'=>array('average_rating','vote_value','vote_time','classId','entity_type','no_of_rating'),
						 'result'=>array('average_rating','vote_value','vote_time','classId','entity_type','no_of_rating'),
							'output_type'=>'json'
						),
					),
				'validate' => array(
					   array('input'=>array('userid'=>1,'user_id'=>"",'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>"cre_sys_obt_cls"), 
							'output'=>array(),
							 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','user_id'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'user_id'=>"",'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>"cre_sys_obt_cls"),
								'output'=>array(),
								'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
								'output_type'=>'json'
								),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>"",'rating'=>40,'entityType'=>"cre_sys_obt_cls"), 
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','classId'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>"",'rating'=>40,'entityType'=>"cre_sys_obt_cls"),
								'output'=>array(),
								'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
								'output_type'=>'json'
						),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>"",'entityType'=>"cre_sys_obt_cls"),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','rating'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>"",'entityType'=>"cre_sys_obt_cls"),
								'output'=>array(),
								'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
								'output_type'=>'json'
						),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>""), 
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','entityType'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'user_id'=>$usrID,'classId'=>$star[0]->entity_id,'rating'=>40,'entityType'=>""),
								'output'=>array(),
								'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
								'output_type'=>'json'
						),
				),
 		)
 	)

						
						
?>