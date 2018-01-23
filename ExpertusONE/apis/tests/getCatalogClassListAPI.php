<?php 
$User_id = db_query("select id from slt_person where status = 'cre_usr_sts_atv' order by id desc limit 5")->fetchAll();
expDebug::dPrint('$result for department = '. print_r($User_id, true));
 

$testCases = array(

			// getCatalogClassListAPI test case Starts
			'getCatalogClassListAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>$User_id[0]->id,'user_id'=>1,'filter_text'=>1),
						  'output'=>array('cls_title','cls_id','crs_id','dl_type'),
						  'result'=>array('<results totalrecords =','<result>','<cls_title>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>$User_id[1]->id,'user_id'=>1),
							'output'=>array('cls_title','cls_id','crs_id','dl_type'),
							'result'=>array('<results totalrecords =','<result>','<cls_title>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>$User_id[2]->id,'user_id'=>1),
							'output'=>array('cls_title','cls_id','crs_id','dl_type'),
							'result'=>array('<results totalrecords =','<result>','<cls_title>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>$User_id[3]->id,'user_id'=>1),
							'output'=>array('cls_title','cls_id','crs_id','dl_type'),
							'result'=>array('<results totalrecords =','<result>','<cls_title>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>$User_id[4]->id,'user_id'=>1),
							'output'=>array('cls_title','cls_id','crs_id','dl_type'),
							'result'=>array('<results totalrecords =','<result>','<cls_title>'),
							'output_type'=>'xml'
							),
				),
				'validate' => array(
					array(	'input'=>array('userid'=>'','user_id'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>INVALID ACCESS</short_msg>'),
							'output_type'=>'xml'
							),
					
				   
				)
			),  // getCatalogClassListAPI test case Ends
			
		); // End of api test case main array
?>