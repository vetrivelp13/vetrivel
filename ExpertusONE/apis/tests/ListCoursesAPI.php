<?php 
$status = "lrn_crs_sts_itv";
$limit = '8';
$classes = db_query("select id,status from slt_course_template where status =:status",array(":status"=>$status))->fetchAll();
$count=count($classes);
expDebug::dPrint("san test123 -- ".print_r($classes,true),1);
foreach($classes as $result){
	if($result->status == "lrn_crs_sts_itv"){
		$result_status = "Unpublished";
	}
	else {
		$result_status= "Published";
	}
}
$testCases = array(

		'ListCoursesAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'limit'=>100,'catalogtype'=>'Course'),
								'output'=>array('id','title','code','short_desc','status','lang','promote'),
								'result'=>array('<results totalrecords =','<id>','<title>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'limit'=>100,'catalogtype'=>'Course'),
								'output'=>array('id','title','code','short_desc','status','lang','promote'),
								'result'=>array('<results totalrecords =','<id>','<title>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>100,'catalogtype'=>'Course','catalogcoursestatus' => $status),
								'output'=>array('id','title','code','short_desc','status','lang','promote'),
								'result'=>array('<results totalrecords = "'.$count.'">','<id>','<title>','<status>'.$result_status.'</status>'),
								'output_type'=>'xml'
						),
						

				)
					
		),
			
);
?>