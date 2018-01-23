<?php
$delete_content=db_query("select content_id, class_id from slt_course_content_mapper where class_id not in (select class_id from slt_enrollment) order by content_id desc limit 0,1")->fetchAll();
expDebug::dPrint('Delete content details : '.print_r($delete_content , true), 5);

$active_enr_content=db_query("select content_id, class_id from slt_course_content_mapper where class_id  in (select class_id from slt_enrollment) order by content_id desc limit 0,1")->fetchAll();
expDebug::dPrint('Already enrolled class &  content details : '.print_r($active_enr_content , true), 5);

$content_ids=db_query("select id from slt_content_master order by id desc")->fetchcol();
expDebug::dPrint('Content ids : '.print_r($content_ids , true), 5);

$class_id=db_query("select id from slt_course_class order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$class_id: '.$class_id);

$classid_not_attach=db_query("select class_id from slt_course_content_mapper where class_id not in (select class_id from slt_enrollment) limit 0,1")->fetchField();
expDebug::dPrint('$classid_not_attach '.$classid_not_attach );
$content_id_attach=db_query("select content_id from slt_course_content_mapper where class_id=$classid_not_attach")->fetchcol();
expDebug::dPrint('$content_id_attach: '.print_r($content_id_attach , true), 5);

$content_not_attached=array_diff($content_ids,$content_id_attach);
expDebug::dPrint('$content_not_attached: '.print_r($content_not_attached , true), 5);

$testCases = array(
		'RemoveAssociatedContentwithWBTClassAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'ContentId'=>$delete_content[0]->content_id,'ClassId'=>$delete_content[0]->class_id),
						  'output'=>array('status'),
						  'result'=>array('<results totalrecords =','<status>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1,'ClassId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ContentId'=>''),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ContentId'=>$active_enr_content[0]->content_id,'ClassId'=>$active_enr_content[0]->class_id),
						  'output'=>array(),
						  'result'=>array('Active Enrollments available'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ContentId'=>$content_ids[0]+11,'ClassId'=>$delete_content[0]->class_id),
						  'output'=>array(),
						  'result'=>array('Content Id invalid'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ContentId'=>$content_ids[0],'ClassId'=>$class_id+1),
						  'output'=>array(),
						  'result'=>array('Class Id invalid'),
						   'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'ContentId'=>$content_not_attached[0],'ClassId'=>$classid_not_attach),
						  'output'=>array(),
						  'result'=>array('Content is not attached to this class'),
						   'output_type'=>'xml'
							),
						)
					)
				);
?>