<?php
$id=db_query("select id from slt_common_mapping where type=6 order by id desc limit 0,1")->fetchField();
$testCases = array(
		'RemoveCourseEquivalenceAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$id),
						  'output'=>array('status'),
						  'result'=>array('<results totalrecords = "','<status>'),
						   'output_type'=>'xml'
							)
				),
				'validate'=>array(
						
						array('input'=>array('userid'=>1),
						       'output'=>array(),
							   'result'=>array('Mandatory Fields are missing in the feed'),
							   'output_type'=>'xml'
							 ),
						array('input'=>array('userid'=>1,'id'=>$id+1),
						       'output'=>array(),
							   'result'=>array('Invalid ID'),
							   'output_type'=>'xml'
							 ),
							 			 
				)
			)
		);
			