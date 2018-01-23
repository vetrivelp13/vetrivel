<?php 
//$user_id = db_query("select id from slt_person order by id desc limit 0,1")->fetchField();
$testCases = array(

			// AddUserListValuesAPI test case Starts
			'AddUserListValuesAPI' => array(
				'datasource' => array(
					 array('input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_dpt'),
						  'output'=>array('id','code'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							), 
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_etp'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_ptp'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_dpt'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Project Manager','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Project Manager','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Account Manager','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Program Manager','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'System Analyst','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'L&D Administrator','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'L&D Administrator','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Solutions Lead','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'System Engineer','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Market Analyst','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Sales Engineer','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Director - Strategic Accounts','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Director - Business Development','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'VP Client Services','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'VP Marketing','code'=>'cre_usr_jrl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Solutions Engineer','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Director','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Vice President','code'=>'cre_usr_jtl'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Sales','code'=>'cre_usr_dpt'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Client Services','code'=>'cre_usr_dpt'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'name'=>'Sandbox_User','code'=>'cre_usr_ptp'),
							'output'=>array('id','code'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
						
				),
				'validate' => array(
					array('input'=>array('userid'=>1,'name'=>'','code'=>'cre_usr_dpt'),
						  'output'=>array('id','code'),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
						  ),
					array(
						'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>''),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						'output_type'=>'xml'
						),
					array(
						'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','code'=>'cre_usr_dpt_bio'),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Invalid Code</short_msg>'),
						'output_type'=>'xml'
						),
					array(
						'input'=>array('userid'=>1,'name'=>'','code'=>'cre_usr_etp'),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						'output_type'=>'xml'
						),
					array(
						'input'=>array('userid'=>1,'name'=>'','code'=>'cre_usr_jrl'),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						'output_type'=>'xml'
						),
					array(
						'input'=>array('userid'=>1,'name'=>'','code'=>'cre_usr_jtl'),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						'output_type'=>'xml'
						),
					array(
						'input'=>array('userid'=>1,'name'=>'','code'=>'cre_usr_ptp'),
						'output'=>array('id','code'),
						'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						'output_type'=>'xml'
						)
							
						
				   
				)
			),  // AddUserListValuesAPI test case Ends
			
		); // End of api test case main array
?>