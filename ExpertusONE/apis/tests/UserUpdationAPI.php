<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$qry = "select user_name,email from slt_person where id =10";
$userdetails = db_query($qry)->fetchAll();

$person = db_query("select id,user_name from slt_person where status =  'cre_usr_sts_itv' order by id desc limit 1")->fetchAll();
$updateperson = db_query("select id,user_name from slt_person limit 10,1")->fetchAll();
expDebug::dPrint("Vincen test222 -- ".print_r($updateperson,true),1);
$testCases = array(
		'UserUpdationAPI' => array(
				'datasource' => array(
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => 'user1200@exp.com',
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'.$updateperson[0]->id.'</Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => 'userexpertus@exp.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_currency' => 'cre_sys_crn_usd',
								'preferred_timezone' => 'cre_sys_tmz_007',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'.$updateperson[0]->id.'</Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => 'userexpertus@exp.com',
								'status' => 'cre_usr_sts_atv',
								'addr1' => 'kilpauk',
								'addr2' => 'chennai',
								'country' => 'US',
								'state'  => 'CA',
								'city'  => 'Mountain View' ,
								'zip'     => '600010',
								'phone_no' => '9999999999',
								'mobile_no' => '9999999999',
								'manager_id'  => '186',
								'org_id' => '5',
								'usertype' => 'cre_usr_ptp_emp',
								'jobrole'  => 'cre_usr_jrl_rl1',
								'jobtitle' =>  'cre_usr_jtl_dtm',
								'empltype' => 'cre_usr_etp_ftm',
								'roles'    => 'instructor',
								'deptcode'  => 'cre_usr_dpt_mkt' ,
								'other_managers' => '150',
								'other_organization' => '3',
								'preferred_timezone' => 'cre_sys_tmz_007',
								'preferred_language' => 'cre_sys_lng_eng',
								'preferred_currency' => 'cre_sys_crn_usd',
								'register_sms' => '0',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'.$updateperson[0]->id.'</Id>'),
								'output_type'=>'xml'
						),
							

				),
				'validate' =>array(
						
						array( 'input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => $userdetails[0]->email,
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>Email id already exists.</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => $random,
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>Email id is not valid.</short_msg>'),
								'output_type'=>'xml'
						),
						
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'manager_id' => $person[0]->id ,
								'userid' =>1
						),
								'output'=>array(''),
								'result'=>array('<short_msg>Invalid Manager'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>getRandomWord(),
								'last_name'=>'exp',
								'user_name'=>$userdetails[0]->user_name,
								'password' => 'welcome',
								'email' => 'sasa345@kant.com',
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>UserName already exists.</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>getRandomWord(),
								'last_name'=>'expee',
								'user_name'=>getRandomWord(),
								'password' => 'welcome',
								'email' => 'sasa@kant.com',
								'status' => 'random',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given status random doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'jobrole' => 'jobb',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given job role jobb doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'usertype' => 'usertype',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given user type usertype doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>getRandomWord(),
								'last_name'=>'expertususer1',
								'user_name'=>getRandomWord(),
								'password' => 'welcome',
								'email' => 'sasa123@kant.com',
								'status' => 'cre_usr_sts_atv',
								'roles'  => 'roles',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>The given role is invalid</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_currency' => 'us dollars',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>The given preferred currency is invalid or inactive status</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_language' => 'dothraki',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given preferred language dothraki doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_timezone' => 'cre_sys_tmz_100',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given preferred timezone cre_sys_tmz_100 doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'deptcode'  => 'it dept' ,
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given department it dept doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'jobtitle' =>  'manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given job title manager doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'empltype' => 'parttime',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given employment type parttime doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'country' => 'USA',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given country USA doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('id'=>$updateperson[0]->id,
								'first_name'=>'expertususer1',
								'last_name'=>'expertususer1',
								'user_name'=>'expertususer1',
								'password' => 'welcome',
								'email' => 'expertususer1@kant.com',
								'status' => 'cre_usr_sts_atv',
								'state' => 'CA',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>Country field should not be empty</short_msg>'),
								'output_type'=>'xml'
						),
				
				
				),

		)
);



?>