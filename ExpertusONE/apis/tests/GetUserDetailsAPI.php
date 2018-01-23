<?php 
$user_name1 = db_query("select user_name from slt_person order by id limit 2,5")->fetchAll();
$organization_name1 = db_query("select name from slt_organization order by id limit 5")->fetchAll();
$job_title_name1 = db_query("select name from slt_profile_list_items where code LIKE '%cre_usr_jtl_%' limit 5")->fetchAll();
$job_role_name1 = db_query("select name from slt_profile_list_items where code LIKE '%cre_usr_jrl_%' limit 5")->fetchAll();
$department_name1 = db_query("select name from slt_profile_list_items where code LIKE '%cre_usr_dpt_%' limit 5")->fetchAll();
$testCases = array(

			// GetUserDetailsAPI test case Starts
			'GetUserDetailsAPI' => array(
				'datasource' => array(
					// User
					array('input'=>array('name'=>$user_name1[0]->user_name,'type'=>'user','userid'=>1),
						  'output'=>array('object_type','id','username','firstname','lastname','email','mobile_no','orgname','status_code','statecode','countrycode','manager_id','dottedmanagersid','dottedorganizationid','timezonecode','preferredlanguagecode','usertype','employeeno','jobrole','jobtitle','deptcode','groups','org_id','org_name','org_description','org_type','org_status_code','org_status','object_id','object_name','object_code'),
						  'result'=>array('<results totalrecords =','<result>','<object_type>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$user_name1[1]->user_name,'type'=>'user','userid'=>1),
							'output'=>array('object_type','id','username','firstname','lastname','email','mobile_no','orgname','status_code','statecode','countrycode','manager_id','dottedmanagersid','dottedorganizationid','timezonecode','preferredlanguagecode','usertype','employeeno','jobrole','jobtitle','deptcode','groups','org_id','org_name','org_description','org_type','org_status_code','org_status','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$user_name1[2]->user_name,'type'=>'user','userid'=>1),
							'output'=>array('object_type','id','username','firstname','lastname','email','mobile_no','orgname','status_code','statecode','countrycode','manager_id','dottedmanagersid','dottedorganizationid','timezonecode','preferredlanguagecode','usertype','employeeno','jobrole','jobtitle','deptcode','groups','org_id','org_name','org_description','org_type','org_status_code','org_status','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$user_name1[3]->user_name,'type'=>'user','userid'=>1),
							'output'=>array('object_type','id','username','firstname','lastname','email','mobile_no','orgname','status_code','statecode','countrycode','manager_id','dottedmanagersid','dottedorganizationid','timezonecode','preferredlanguagecode','usertype','employeeno','jobrole','jobtitle','deptcode','groups','org_id','org_name','org_description','org_type','org_status_code','org_status','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$user_name1[4]->user_name,'type'=>'user','userid'=>1),
							'output'=>array('object_type','id','username','firstname','lastname','email','mobile_no','orgname','status_code','statecode','countrycode','manager_id','dottedmanagersid','dottedorganizationid','timezonecode','preferredlanguagecode','usertype','employeeno','jobrole','jobtitle','deptcode','groups','org_id','org_name','org_description','org_type','org_status_code','org_status','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
						
					// Organization
					array(
							'input'=>array('name'=>$organization_name1[0]->name,'type'=>'organization','userid'=>1),
							'output'=>array('org_id','org_name','org_description','org_type','org_status_code','org_status'),
							'result'=>array('<results totalrecords =','<result>','<org_id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$organization_name1[1]->name,'type'=>'organization','userid'=>1),
							'output'=>array('org_id','org_name','org_description','org_type','org_status_code','org_status'),
							'result'=>array('<results totalrecords =','<result>','<org_id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$organization_name1[2]->name,'type'=>'organization','userid'=>1),
							'output'=>array('org_id','org_name','org_description','org_type','org_status_code','org_status'),
							'result'=>array('<results totalrecords =','<result>','<org_id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$organization_name1[3]->name,'type'=>'organization','userid'=>1),
							'output'=>array('org_id','org_name','org_description','org_type','org_status_code','org_status'),
							'result'=>array('<results totalrecords =','<result>','<org_id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$organization_name1[4]->name,'type'=>'organization','userid'=>1),
							'output'=>array('org_id','org_name','org_description','org_type','org_status_code','org_status'),
							'result'=>array('<results totalrecords =','<result>','<org_id>'),
							'output_type'=>'xml'
							),
						
					// Job Title
					array(
							'input'=>array('name'=>$job_title_name1[0]->name,'type'=>'job_title','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_title_name1[1]->name,'type'=>'job_title','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_title_name1[2]->name,'type'=>'job_title','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_title_name1[3]->name,'type'=>'job_title','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_title_name1[4]->name,'type'=>'job_title','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
						
					// Job Role
					array(
							'input'=>array('name'=>$job_role_name1[0]->name,'type'=>'job_role','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_role_name1[1]->name,'type'=>'job_role','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_role_name1[2]->name,'type'=>'job_role','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_role_name1[3]->name,'type'=>'job_role','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$job_role_name1[4]->name,'type'=>'job_role','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
							),
					
					// Department
					array(
							'input'=>array('name'=>$department_name1[0]->name,'type'=>'department','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array('name'=>$department_name1[1]->name,'type'=>'department','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array('name'=>$department_name1[2]->name,'type'=>'department','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array('name'=>$department_name1[3]->name,'type'=>'department','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
					),
					array(
							'input'=>array('name'=>$department_name1[4]->name,'type'=>'department','userid'=>1),
							'output'=>array('object_type','object_id','object_name','object_code'),
							'result'=>array('<results totalrecords =','<result>','<object_type>'),
							'output_type'=>'xml'
					),
					
				
				),
				'validate' => array(
 					array('input'=>array('name'=>'','type'=>'user','userid'=>1),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>$user_name1[0],'type'=>'user','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array('name'=>'Employee','type'=>'user_type','userid'=>1),
							'output'=>array(),
							'result'=>array('<error>','<error_code>','<short_msg>type can be user or organization or job_role or job_title or department</short_msg>'),
							'output_type'=>'xml'
					)
				)
			),  // GetUserDetailsAPI test case Ends
			
		); // End of api test case main array
?>