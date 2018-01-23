<?php 
$customfiled = db_query("select entity_id,entity_type,label from slt_custom_fields order by id desc limit 1")->fetchAll();

$testCases = array(

			
			'deleteCustomFieldAPI' => array(
				'datasource' => array(
					array('input'=>array('entity_type'=>$customfiled[0]->entity_type,'entity_id'=>$customfiled[0]->entity_id,'custom_label'=>$customfiled[0]->label,'userid'=>1),
								'output'=>array('status'),
								'result'=>array('<status>Success</status>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
						array('input'=>array('entity_id'=>10,'custom_label'=>'random','userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'random','custom_label'=>'random','userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'random','entity_id'=>10,'userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
					array('input'=>array('entity_type'=>$customfiled[0]->entity_type,'entity_id'=>$customfiled[0]->entity_id,'custom_label'=>'random','userid'=>1),
								'output'=>array('status'),
								'result'=>array('<short_msg>Invalid Custom Label to this Entity or Not Found</short_msg>'),
								'output_type'=>'xml'
							),
						array('input'=>array('entity_type'=>'random','entity_id'=>10,'custom_label'=>'random','userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Invalid EntityType</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>$customfiled[0]->entity_type,'entity_id'=>10,'custom_label'=>'random','userid'=>1),
								'output'=>array(''),
								'result'=>array('<error>'),
								'output_type'=>'xml'
						),
				)
			), 
			
		);
?>
