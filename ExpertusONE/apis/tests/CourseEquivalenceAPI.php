<?php
$entity_id=db_query("select course_id from slt_course_class where course_id not in (select id1 from slt_common_mapping) limit 0,1")->fetchField();
expDebug::dPrint('$entity_id:::'. $entity_id);
$entity_type='cre_sys_obt_crs';
$equivalence_id=db_query("select course_id from slt_course_class where course_id!=$entity_id limit 0,1")->fetchField();
$already_added=db_query("select id1,id2 from slt_common_mapping  where type=6 limit 0,1")->fetchAll();
expDebug::dPrint('$already_added:::'.print_r($already_added,true),5);
$testCases = array(
		'CourseEquivalenceAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'EntityId'=>$entity_id,'EnitityType'=>$entity_type,'EquivalenceId'=>$equivalence_id),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "','<id>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'EntityId'=>$already_added[0]->id1,'EnitityType'=>$entity_type,'EquivalenceId'=>$already_added[0]->id2),
						  'output'=>array(),
						  'result'=>array('Same Course has already been added as equivalence'),
						   'output_type'=>'xml'
							)
							 	
				
				)
			)
		);
			
?>		