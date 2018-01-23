<?php 
$course = db_query("select id from slt_course_template order by id desc limit 1 ")->fetchAll();
$class = db_query("select id from slt_course_class order by id desc limit 1 ")->fetchAll();
$organisation = db_query("select id  from slt_organization  order by id desc limit 1 ")->fetchAll();
$trainingplan = db_query("select id from slt_program order by id desc limit 1 ")->fetchAll();
$survey = db_query("select id from slt_survey where type = 'sry_det_typ_sry' order by id desc limit 1 ")->fetchAll();
$assessment = db_query("select id from slt_survey where type = 'sry_det_typ_ass' order by id desc limit 1 ")->fetchAll();
$person = db_query("select id  from slt_person order by id desc limit 1 ")->fetchAll();
$location = db_query("select id from slt_location order by id desc limit 1 ")->fetchAll();
$userlabel = db_query("select entity_id as id ,LOWER(label) as label  from slt_custom_fields where entity_type = 'course' order by id desc limit 1 ")->fetchAll();
expDebug::dPrint('LIST $result123 = ' . print_r($userlabel, true) , 3);
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$longlabel = "Quantitatively driven answers. Achievements couched in dates/revenue made/money saved. Technologies used to build things that made a COMMERCIAL IMPACT. And one/two extra-curricular things that pertain to you being an all-rounder.Don't think of it in terms of word count. Think of it it in terms of pages";
$testCases = array(

		// List User API test case Starts
		'CreateCustomFieldAPI' => array(
				'datasource' => array(
						array('input'=>array('entity_type'=>'course','entity_id'=>$course[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('entity_id'=>$class[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'class','custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'class','entity_id'=>$class[0]->id,'custom_label'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'class','entity_id'=>$class[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'organization','entity_id'=>$organisation[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'trainingplan','entity_id'=>$trainingplan[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'survey','entity_id'=>$survey[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'assessment','entity_id'=>$assessment[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'user','entity_id'=>$person[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'location','entity_id'=>$location[0]->id,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('id'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'junk','entity_id'=>10,'custom_label'=>$random,'custom_value'=>$random,'userid'=>1),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid EntityType</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'course','entity_id'=>$userlabel[0]->id,'custom_label'=>$userlabel[0]->label,'custom_value'=>$random,'userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Label already exist. Enter a new label</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('entity_type'=>'course','entity_id'=>$userlabel[0]->id,'custom_label'=>$longlabel,'custom_value'=>$random,'userid'=>1),
								'output'=>array(''),
								'result'=>array('<short_msg>Only 256 characters allowed to Custom Label</short_msg>'),
								'output_type'=>'xml'
						),
				)
		),
			
);
?>

