<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $order = db_query("select * from slt_order_items order by id DESC limit 1 ")->fetchAll();
	//$count=count($surv_ques);
	//expDebug::dPrint(" Checking ".print_r($order));

 $testCases = array(
			'deleteItemsFromCartAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'user_id'=>$order[0]->user_id,'slt_orderID'=>$order[0]->order_id,'classID'=>$order[0]->class_id,'action'=>"delete"), //,'deliverytypename'=>$order[0]->program_type),
						 'output'=>array('status'),
						 'result'=>array('<status>success</status>'),
						  'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'user_id'=>$order[0]->user_id,'slt_orderID'=>$order[0]->order_id,'classID'=>$order[0]->class_id,'action'=>"delete"),  //'deliverytypename'=>$order[0]->program_type),
								'output'=>array('status'),
								'result'=>array('jsonResponse','"status":"success"'),
								'output_type'=>'json'
						 ),
						),
					'validate' => array(
						  array('input'=>array('userid'=>1,'user_id'=>"",'slt_orderID'=>$order[0]->order_id,'classID'=>$order[0]->class_id,'action'=>"delete",'deliverytypename'=>$order[0]->program_type),
						 'output'=>array(),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','user_id'),
						  'output_type'=>'xml'
							),
							array('input'=>array('userid'=>1,'user_id'=>"",'slt_orderID'=>$order[0]->order_id,'classID'=>$order[0]->class_id,'action'=>"delete",'deliverytypename'=>$order[0]->program_type),
									'output'=>array(),
	                            	'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
									'output_type'=>'json'
							),
						  array('input'=>array('userid'=>1,'user_id'=>$order[0]->user_id,'slt_orderID'=>"",'classID'=>$order[0]->class_id,'action'=>"delete",'deliverytypename'=>$order[0]->program_type),
									'output'=>array(),
									'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','slt_orderID'),
									'output_type'=>'xml'
							),
							array('input'=>array('userid'=>1,'user_id'=>$order[0]->user_id,'slt_orderID'=>"",'classID'=>$order[0]->class_id,'action'=>"delete",'deliverytypename'=>$order[0]->program_type),
									'output'=>array(),
									'result'=>array('jsonResponse','"iserror":"true"','"errorcode":".L_005."','"shortmessage":"Mandatory Fields are missing in the feed. List provided below:"'),
									'output_type'=>'json'
							),
					)
				)
 		)
 	?>