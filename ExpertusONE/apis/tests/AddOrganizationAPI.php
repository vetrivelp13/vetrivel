<?php 

$testCases = array(

			// AddOrganizationAPI test case Starts
			'AddOrganizationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int'),
							'output'=>array(id),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Logicalis Pte. Ltd.','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'IBM - Europe','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Information Technology','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Expertus','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
				    array(
							'input'=>array('userid'=>1,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','name'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'','type'=>'cre_org_typ_ext'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','description'),
							'output_type'=>'xml'
							),
				    array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>''),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','type'),
							'output_type'=>'xml'
							),
				    array(	
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'ttytt'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Organization Type does not exist.</short_msg>'),
							'output_type'=>'xml'
 							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'','type'=>'cre_org_typ_int'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','description'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int'),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','name'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('userid'=>1,'name'=>'Dynamic_Org_VERSION','description'=>'Dynamic_Org_VERSION','type'=>''),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','type'),
							'output_type'=>'xml'
							)
				)
			),  // AddOrganizationAPI test case Ends
			
		); // End of api test case main array
?>