<?php 
//$org_name = db_query("select name from slt_organization limit 0,1;")->fetchField();
$testCases = array(

			// ListOfGroup test case Starts
			'ListOfGroup' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'limit'=>10),
						  'output'=>array('Id','name','grpstatus','is_admin','users_list','grporg','role','grpjobrole','grpusrtyp','grpempl','grpdep','grplang','grpcontry','grploc'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'limit'=>10),
						  'output'=>array('Id','name','grpstatus','is_admin','users_list','grporg','role','grpjobrole','grpusrtyp','grpempl','grpdep','grplang','grpcontry','grploc'),
						  'result'=>array('<results totalrecords =','<result>','<Id>'),
						  'output_type'=>'xml'
							),	
// 						array(
// 								'input'=>array('userid'=>1,'limit'=>10,'grpstatus'=>'jhgd'),
// 								'output'=>array(),
// 								'result'=>array('<results totalrecords =','<result>','</result>'),
// 								'output_type'=>'xml'
// 						),
						array('input'=>array('userid'=>1,'limit'=>10,'grpstatus'=>'cre_sec_sts_itv','grpjobrole'=>'Job Role 1','grpusrtyp'=>'Employee','grpempl'=>'Full Time','grpdep'=>'IT Department','grplang'=>'cre_sys_lng_eng','grpcontry'=>'India'),
								'output'=>array('Id','name','grpstatus','is_admin','users_list','grporg','role','grpjobrole','grpusrtyp','grpempl','grpdep','grplang','grpcontry','grploc'),
								'result'=>array('<results totalrecords =','<result>','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'limit'=>10,'grpstatus'=>'','grpjobrole'=>'Job Role 1','grpusrtyp'=>'Employee','grpempl'=>'Full Time','grpdep'=>'IT Department','grplang'=>'cre_sys_lng_eng','grpcontry'=>'India'),
								'output'=>array('Id','name','grpstatus','is_admin','users_list','grporg','role','grpjobrole','grpusrtyp','grpempl','grpdep','grplang','grpcontry','grploc'),
								'result'=>array('<results totalrecords =','<result>','<Id>'),
								'output_type'=>'xml'
						)
				)
			),  // ListOfGroup test case Ends
			
		); // End of api test case main array
?>