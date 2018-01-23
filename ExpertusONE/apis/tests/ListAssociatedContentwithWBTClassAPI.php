<?php

$details=db_query("select class_id,course_id,content_id from slt_course_content_mapper group by class_id having count(id)=1 limit 0,1")->fetchAll();


expDebug::dPrint('List of content ides attached '.print_r($details,true),5);


$testCases = array(
		'ListAssociatedContentwithWBTClassAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'CourseId'=>$details[0]->course_id,'ClassId'=>$details[0]->class_id),
						  'output'=>array('id','content_id','lession_id','code'),
						  'result'=>array('<results totalrecords = "1">','<id>','</id>','<content_id>'.$details[0]->content_id.'</content_id>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1,'CourseId'=>$course_details),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'ClassId'=>$class_details),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
				
						
						
						
						)
					)
				);
?>