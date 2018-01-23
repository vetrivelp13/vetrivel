<?php 
$course = db_query("select id from slt_course_template order by id desc limit 1 ")->fetchAll();
$class = db_query("select id from slt_course_class order by id desc limit 1 ")->fetchAll();
$organisation = db_query("select entity_id  from slt_custom_fields where entity_type = 'organization' order by id desc limit 1 ")->fetchAll();
$trainingplan = db_query("select id from slt_program order by id desc limit 1 ")->fetchAll();
$survey = db_query("select id from slt_survey order by id desc limit 1 ")->fetchAll();
$person = db_query("select entity_id  from slt_custom_fields where entity_type = 'user' order by id desc limit 1 ")->fetchAll();
$location = db_query("select id from slt_location order by id desc limit 1 ")->fetchAll();
$testCases = array(

		
		'ListCustomFieldAPI' => array(
				'datasource' => array(
						array('input'=>array('entity_type'=>'course','entity_id'=>$course[0]->id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('entity_type'=>'class','entity_id'=>$class[0]->id,'limit'=>'10','userid'=>1),
								'output'=>array('id',),
								'result'=>array('<results totalrecords ='),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_id'=>$class[0]->id,'limit'=>'10','userid'=>1),
								'output'=>array('id',),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'class','limit'=>'10','userid'=>1),
								'output'=>array('id',),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'organization','entity_id'=>$organisation[0]->entity_id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'trainingplan','entity_id'=>$trainingplan[0]->entity_id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'survey','entity_id'=>$survey[0]->entity_id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'user','entity_id'=>$person[0]->entity_id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'location','entity_id'=>$location[0]->entity_id,'limit'=>'10','userid'=>1),
								'output'=>array('id','field_label','field_value'),
								'result'=>array('<results totalrecords =','<id>','<field_label>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'random','entity_id'=>10,'limit'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid EntityType</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'course','entity_id'=>count($course)+1,'limit'=>'10','userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid EntityId or Not Found</short_msg>'),
								'output_type'=>'xml'
						),
				)
		),
			
);
?>