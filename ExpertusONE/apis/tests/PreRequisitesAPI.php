<?php
$entity_id=db_query("select id from slt_program where id not in (select id1 from slt_common_mapping) limit 0,1")->fetchField();
$entity_id_crs=db_query("select course_id from slt_course_class where course_id not in (select id1 from slt_common_mapping) limit 0,1")->fetchField();
$entitytype_trp='cre_sys_obt_trp';
$entitytype_crsp='cre_sys_obt_crs';
$pre_requisite_id=db_query("select course_id from slt_course_class where status='lrn_cls_sts_atv' order by id desc limit 0,1")->fetchField();
$pre_requisite_id_trp=db_query("select id from slt_program where status='lrn_cls_sts_atv' limit 0,1 ")->fetchField();
$pre_requisite_type_crs='cre_sys_pre_crs';
$pre_requisite_type_trp='cre_sys_pre_trp';
$already_exist=db_query("select id1,id2,object_type,prereq_type from slt_common_mapping limit 0,1")->fetchAll();
expDebug::dPrint('$already_exist:::'.print_r($already_exist,true),5);
$testCases = array(
		'PreRequisitesAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'EntityId'=>$entity_id,'EnitityType'=>$entitytype_trp,'PrequisitesId'=>$pre_requisite_id,'PrequisitesType'=>$pre_requisite_type_crs),
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
						array('input'=>array('userid'=>1,'EntityId'=>$entity_id,'EnitityType'=>aaaa,'PrequisitesId'=>$pre_requisite_id,'PrequisitesType'=>$pre_requisite_type_crs),
						  'output'=>array(),
						  'result'=>array('Invalid Entity Type'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'EntityId'=>$entity_id,'EnitityType'=>$entitytype_trp,'PrequisitesId'=>$pre_requisite_id,'PrequisitesType'=>aaa),
						  'output'=>array(),
						  'result'=>array('Invalid Pre-requisite Type'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'EntityId'=>$already_exist[0]->id1,'EnitityType'=>$already_exist[0]->object_type,'PrequisitesId'=>$already_exist[0]->id2,'PrequisitesType'=>$already_exist[0]->prereq_type),
						  'output'=>array(),
						  'result'=>array('Same Course already exist'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'EntityId'=>$entity_id_crs,'EnitityType'=>$entitytype_crs,'PrequisitesId'=>$pre_requisite_id_trp,'PrequisitesType'=>$pre_requisite_type_trp),
						  'output'=>array('id'),
						  'result'=>array(),
						   'output_type'=>'xml'
							)
							 				
							 						
						
				
				)
			)
		);
			
?>		