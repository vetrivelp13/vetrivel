 <?php 
$org_id = db_query("select id from slt_organization limit 0,1")->fetchField();
$testCases = array(

			// UpdateOrganizationAPI test case Starts
			'UpdateOrganizationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'Activate'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'Activate'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('userid'=>1,'id'=>'','name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_ext','status'=>'Activate'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','id'),
						  'output_type'=>'xml'
							),
				    array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','name'),
						'output_type'=>'xml'
					  		),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_ext','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','description'),
						'output_type'=>'xml'
							),
				    array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','type'),
						'output_type'=>'xml'
						),
				    array(	
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'ttytt','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','<short_msg>Organization Type does not exist.</short_msg>'),
						'output_type'=>'xml'
 							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>''),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','status'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'activ'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','<short_msg>Invalid status. Please use Activate or InActivate</short_msg>'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>'Activate'),
						'output'=>array('id'),
						'result'=>array('<results totalrecords =','<result>','<id>'),
						'output_type'=>'xml'
						    ),
				    array(
						  'input'=>array('userid'=>1,'id'=>'','name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_int','status'=>'Activate'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','id'),
						  'output_type'=>'xml'
							),
				    array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','name'),
						'output_type'=>'xml'
					  		),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_int','status'=>'Activate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','description'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>''),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','status'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>'activ'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','<short_msg>Invalid status. Please use Activate or InActivate</short_msg>'),
						'output_type'=>'xml'
							),
				    array(
				    	'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'InActivate'),
						'output'=>array('id'),
						'result'=>array('<results totalrecords =','<result>','<id>'),
					 	'output_type'=>'xml'
							),
				    array(
						'input'=>array('userid'=>1,'id'=>'','name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_ext','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','id'),
						'output_type'=>'xml'
						 	),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_ext','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','name'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_ext','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','description'),
						'output_type'=>'xml'
						  	),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','type'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'ttytt','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','<short_msg>Organization Type does not exist.</short_msg>'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>'InActivate'),
						'output'=>array('id'),
						'result'=>array('<results totalrecords =','<result>','<id>'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>'','name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_int','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','id'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'','description'=>'Dynamic_Org_VERSION','type'=>'cre_org_typ_int','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','name'),
						'output_type'=>'xml'
							),
					array(
						'input'=>array('userid'=>1,'id'=>$org_id,'name'=>'Dynamic_Org_Updated_VERSION','description'=>'','type'=>'cre_org_typ_int','status'=>'InActivate'),
						'output'=>array(),
						'result'=>array('<error>','<error_code>','description'),
						'output_type'=>'xml'
							)
				)
			),  // UpdateOrganizationAPI test case Ends
			
		); // End of api test case main array
?>