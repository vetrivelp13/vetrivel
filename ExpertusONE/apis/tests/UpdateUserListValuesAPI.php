<?php 

$data1 = db_query("select id from slt_profile_list_items where code LIKE '%cre_usr_dpt_%' AND name LIKE '%Dynamic%' order by id desc limit 5")->fetchAll();
$data2 = db_query("select id from slt_profile_list_items where code LIKE '%cre_usr_jtl_%' AND name LIKE '%Dynamic%' order by id desc limit 5")->fetchAll();
$data3 = db_query("select id from slt_profile_list_items where code LIKE '%cre_usr_jrl_%' AND name LIKE '%Dynamic%' order by id desc limit 5")->fetchAll();
$data4 = db_query("select id from slt_profile_list_items where code LIKE '%cre_usr_ptp_%' AND name LIKE '%Dynamic%' order by id desc limit 5")->fetchAll();
$data5 = db_query("select id from slt_profile_list_items where code LIKE '%cre_usr_etp_%' AND name LIKE '%Dynamic%' order by id desc limit 5")->fetchAll();
expDebug::dPrint('$result for department = ' . print_r($data1, true));
$testCases = array(

			// UpdateUserListValuesAPI test case Starts
			'UpdateUserListValuesAPI' => array(
				'datasource' => array(
					array('input'=>array('id'=>$data1[0]->id,'code'=>'cre_usr_dpt','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "">','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data1[1]->id,'code'=>'cre_usr_dpt','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data2[0]->id,'code'=>'cre_usr_jtl','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data2[1]->id,'code'=>'cre_usr_jtl','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data3[0]->id,'code'=>'cre_usr_jrl','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data3[1]->id,'code'=>'cre_usr_jrl','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data4[0]->id,'code'=>'cre_usr_ptp','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data4[1]->id,'code'=>'cre_usr_ptp','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data5[0]->id,'code'=>'cre_usr_etp','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('id'=>$data5[1]->id,'code'=>'cre_usr_etp','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array('id'),
							'result'=>array('<results totalrecords = "">','<result>','<id>'),
							'output_type'=>'xml'
							)
						
				),
				'validate' => array(
 					array('input'=>array('id'=>'','code'=>'cre_usr_dpt','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('id'=>$data5[1]->id,'code'=>'','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
					  		),
					array(
							'input'=>array('id'=>$data5[1]->id,'code'=>'cre_usr_dpt','name'=>'','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('id'=>$data5[1]->id,'code'=>'cre_usr_dpt_007','name'=>'Dynamic_List_Updated_VERSION','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Invalid Code</short_msg>'),
							'output_type'=>'xml'
							)

				)
			),  // UpdateUserListValuesAPI test case Ends
			
		); // End of api test case main array
?>