<?php
$learnerId=db_query("select id from slt_person order by id desc limit 1")->fetchField();
//$getClsTPid=db_query("select entity_id from slt_node_learning_activity order by entity_id desc limit 1")->fetchField();
$getClsTPTypeAndId=db_query("select entity_id, entity_type from slt_node_learning_activity where entity_type='cre_sys_obt_crs' 
or entity_type='cre_sys_obt_cls' or entity_type='cre_sys_obt_crt' or entity_type='cre_sys_obt_trn' or entity_type='cre_sys_obt_cur'
order by entity_id desc limit 1")->fetchAll();
$getEmailId=db_query("select email from slt_person order by email desc limit 1,2")->fetchField();
$testCases = array(

		//shareClassAPI test case Starts
		'shareClassAPI' => array(
				'datasource' => array(
					array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<results totalrecords =','<result>','<status>'),
								'output_type'=>'xml'
					),
					array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('jsonResponse','results','status'),
								'output_type'=>'json'
					)
				),
				'validate'=>array(
						array('input'=>array('user_id'=>'','entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>'','entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>'','referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>'','referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>'','ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>'','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>''),
								'output'=>array('status'),
								'result'=>array('<short_msg>INVALID ACCESS</short_msg>'),
								'output_type'=>'xml'
				       ),
					   array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>testId),
								'output'=>array('status'),
								'result'=>array('<short_msg>INVALID ACCESS</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>email,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Please enter a valid email in referFrom</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>email,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Please enter a valid email in referTo</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('user_id'=>$learnerId,'entityId'=>$getClsTPTypeAndId[0]->entity_id,'entityType'=>$getClsTPTypeAndId[0]->entity_type,'referFrom'=>$getEmailId,'referTo'=>$getEmailId,'ccopy'=>2,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<status>success</status>'),
								'output_type'=>'xml'
						)
				
			         )
			),//shareClassAPI test case Ends
		);// End of api test case main array
?>