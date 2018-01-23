<?php
$user_id=db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$programid=db_query("select program_id from slt_module_crs_mapping order by id desc limit 0,1")->fetchField();
$object_type=db_query("select object_type from slt_module_crs_mapping where program_id=$programid")->fetchField();
$testCases = array(
		'getTPClassesAPI' => array(
				'datasource' => array(
						array('input'=>array('user_id'=>$user_id,'userid'=>$user_id,'programID'=>$programid,'object_type'=>$object_type),
						  'output'=>array('tp_courses','class_list','enrolled_classes'),
						  'result'=>array('<results totalrecords = "">','<result>','<class_list>'),
								'output_type'=>'xml'
						)
				),
				'validate'=>array(
				
						array('input'=>array('userid'=>$user_id,'user_id'=>$user_id),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
					)
			)
		);
?>
