
<?php 
expDebug::dPrint('$price_start---->');
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$qry = "select user_name,email from slt_person order by id desc limit 1";
$userdetails = db_query($qry)->fetchAll();
$person = db_query("select id,user_name from slt_person where status =  'cre_usr_sts_itv' order by id desc limit 1")->fetchAll();
$orgdetails = db_query("select id from slt_organization where name = 'IBM - Europe' and status = 'cre_org_sts_act'")->fetchField();
$Expertusorg = db_query("select id from slt_organization where name = 'Expertus' and status = 'cre_org_sts_act'")->fetchField();
$InfoTech = db_query("select id from slt_organization where name = 'Information Technology' and status = 'cre_org_sts_act'")->fetchField();
$deptdetails = db_query("select code from  slt_profile_list_items where name = 'Human resources' and code like '%cre_usr_dpt%' and is_active = 'Y' ")->fetchField();
$jobtitle = db_query("select code from  slt_profile_list_items where name = 'Project Manager' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$jobrole = db_query("select code from  slt_profile_list_items where name = 'Project Manager' and code like '%cre_usr_jrl%' and is_active = 'Y'")->fetchField();
$usertype = db_query("select code from  slt_profile_list_items where name = 'Employee' and code like '%cre_usr_ptp%' and is_active = 'Y'")->fetchField();
$orgdetails2 = db_query("select id from slt_organization where name = 'Logicalis Pte. Ltd.' and status = 'cre_org_sts_act'")->fetchField();
$manager1 = db_query("select id from slt_person where user_name = 'williamb' and status = 'cre_usr_sts_atv'")->fetchField();
$manager2 = db_query("select id from slt_person where user_name = 'mariat' and status = 'cre_usr_sts_atv'")->fetchField();
$mohana = db_query("select id from slt_person where user_name = 'mohanark' and status = 'cre_usr_sts_atv'")->fetchField();
$pierre = db_query("select id from slt_person where user_name = 'pierrek' and status = 'cre_usr_sts_atv'")->fetchField();
$gordon = db_query("select id from slt_person where user_name = 'gordonj' and status = 'cre_usr_sts_atv'")->fetchField();
$andreash = db_query("select id from slt_person where user_name = 'Andreash' and status = 'cre_usr_sts_atv'")->fetchField();
$sarahj = db_query("select id from slt_person where user_name = 'sarahj' and status = 'cre_usr_sts_atv'")->fetchField();
$fredb = db_query("select id from slt_person where user_name = 'fredb' and status = 'cre_usr_sts_atv'")->fetchField();
$dept1 = db_query("select code from slt_profile_list_items where name = 'IT Department' and code like '%cre_usr_dpt%' and is_active = 'Y'")->fetchField();
$usertype2 = db_query("select code from  slt_profile_list_items where name = 'Customer' and code like '%cre_usr_ptp%' and is_active = 'Y'")->fetchField();
$jobtitle2 = db_query("select code from  slt_profile_list_items where name = 'Account Manager' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$financedept = db_query("select code from  slt_profile_list_items where name = 'Financial/Accounting' and code like '%cre_usr_dpt%' and is_active = 'Y'")->fetchField();
$programManager = db_query("select code from  slt_profile_list_items where name = 'Program Manager' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$systemAnalyst = db_query("select code from  slt_profile_list_items where name = 'System Analyst' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$jobtitle3 = db_query("select * from  slt_profile_list_items where name = 'L&D Administrator' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchAll();
$jobrole3 = db_query("select code from  slt_profile_list_items where name = 'L&D Administrator' and code like '%cre_usr_jrl%' and is_active = 'Y' ")->fetchAll();
$Mop = db_query("select code from  slt_profile_list_items where name = 'Maintenance/Operations' and code like '%cre_usr_dpt%' and is_active = 'Y'")->fetchField();
$solutionslead = db_query("select code from  slt_profile_list_items where name = 'Solutions Lead' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$SystemEngineer = db_query("select code from  slt_profile_list_items where name = 'System Engineer' and code like '%cre_usr_jrl%' and is_active = 'Y'")->fetchField();
$marketing = db_query("select code from  slt_profile_list_items where name = 'Marketing' and code like '%cre_usr_dpt%' and is_active = 'Y'")->fetchField();
$marketAnalyst = db_query("select code from  slt_profile_list_items where name = 'Market Analyst' and code like '%cre_usr_jtl%'and is_active = 'Y'")->fetchField();
$SalesEngineer = db_query("select code from  slt_profile_list_items where name = 'Sales Engineer' and code like '%cre_usr_jrl%' and is_active = 'Y'")->fetchField();
$SolutionsEngineer =db_query("select code from  slt_profile_list_items where name = 'Solutions Engineer' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$partner =  db_query("select code from  slt_profile_list_items where name = 'Partner' and code like '%cre_usr_ptp%' and is_active = 'Y'")->fetchField();
$datamanager = db_query("select code from  slt_profile_list_items where name = 'Data Manager' and code like '%cre_usr_jtl%' and is_active = 'Y'")->fetchField();
$expertusVP = db_query("select code from  slt_profile_list_items where name = 'VP Client Services' and code like '%cre_usr_jrl%' and is_active = 'Y' ")->fetchAll();
$titleVP = db_query("select code from  slt_profile_list_items where name = 'Vice President' and code like '%cre_usr_jtl%' and is_active = 'Y' ")->fetchAll();
$ClientServices = db_query("select code from  slt_profile_list_items where name = 'Client Services' and code like '%cre_usr_dpt%' and is_active = 'Y' ")->fetchAll();
$Sales = db_query("select code from  slt_profile_list_items where name = 'Sales' and code like '%cre_usr_dpt%' and is_active = 'Y' ")->fetchAll();
$Director = db_query("select code from  slt_profile_list_items where name = 'Director' and code like '%cre_usr_jtl%' and is_active = 'Y' ")->fetchAll();
$Directorjrl = db_query("select code from  slt_profile_list_items where name = 'Director - Strategic Accounts' and code like '%cre_usr_jrl%' and is_active = 'Y' ")->fetchAll();
$Directorjrl2 = db_query("select code from  slt_profile_list_items where name = 'Director - Business Development' and code like '%cre_usr_jrl%' and is_active = 'Y' ")->fetchAll();
$VPMarketing= db_query("select code from  slt_profile_list_items where name = 'VP Marketing' and code like '%cre_usr_jrl%' and is_active = 'Y' ")->fetchAll();

$testCases = array(
		'UserCreationAPI' => array(
				'datasource' => array(
						 /* array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),  */
						array('input'=>array('first_name'=>'William',
								'last_name'=>'Brown',
								'user_name'=>'williamb',
								'password' => 'welcome',
								'email' => 'williamb@demo.com',
								'status' => 'cre_usr_sts_atv',
								'country' => 'US',
								'org_id' => $orgdetails,
								'usertype' => $usertype,
								'jobrole'  => $jobrole,
								'jobtitle' =>  $jobtitle,
								'roles'    => 'instructor,manager',
								'deptcode'  => $deptdetails ,
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Maria','last_name'=>'Torres','user_name'=>'mariat','password' => 'welcome','email' => 'mariat@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $orgdetails2,'manager_id'  => $manager1,'usertype' => $usertype2,'jobrole'  => $jobrole,
								'jobtitle' =>  $jobtitle2,'roles'    => 'instructor,manager','deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Andreas','last_name'=>'Hartmann','user_name'=>'Andreash','password' => 'welcome','email' => 'Andreash@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'GB','org_id' => $orgdetails,'manager_id'  => $$manager1,'usertype' => $usertype,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $SolutionsEngineer,'deptcode'  => $Mop ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Sarah','last_name'=>'Jones','user_name'=>'sarahj','password' => 'welcome','email' => 'sarah@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $InfoTech,'manager_id'  => $andreash,'other_managers' =>$manager1,'usertype' => $partner,'jobrole'  =>$jobrole,
								'jobtitle' =>  $programManager,'deptcode'  => $dept1 ,'roles'    => 'instructor,manager','userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Darrin','last_name'=>'Johnson','user_name'=>'darrinj','password' => 'welcome','email' => 'darrinj@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'GB','org_id' => $orgdetails2,'manager_id'  => $manager2,'other_managers' => $manager1,'usertype' => $usertype2,'jobrole'  => $jobrole,
								'jobtitle' =>  $systemAnalyst,'deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
					  array('input'=>array('first_name'=>'Julie','last_name'=>'Banks','user_name'=>'Julieb','password' => 'welcome','email' => 'Julieb@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'NL','org_id' => $orgdetails2,'manager_id'  => $manager2,'other_managers' => $manager1,'usertype' => $usertype2,'jobrole'  => $jobrole,
								'jobtitle' =>  $programManager,'deptcode'  => $financedept ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Jack','last_name'=>'Martin','user_name'=>'jackk','password' => 'welcome','email' => 'jackk@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'DE','org_id' => $orgdetails2,'manager_id'  => $manager2,'other_managers' => $manager1,'usertype' => $usertype2,'jobrole'  => $jobrole3,
								'jobtitle' =>  $jobtitle3[0]->code,'deptcode'  => $deptdetails ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Keith','last_name'=>'Martinez','user_name'=>'keithm','password' => 'welcome','email' => 'keithm@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'ES','org_id' => $orgdetails2,'manager_id'  => $manager2,'other_managers' => $manager1,'usertype' => $usertype2,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $solutionslead,'deptcode'  => $Mop ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						 array('input'=>array('first_name'=>'Frank','last_name'=>'Griffin','user_name'=>'frankg','password' => 'welcome','email' => 'frankg@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'FR','org_id' => $orgdetails2,'manager_id'  => $manager2,'other_managers' => $manager1,'usertype' => $usertype2,'jobrole'  => $SalesEngineer,
								'jobtitle' =>  $marketAnalyst,'deptcode'  => $marketing ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
							array('input'=>array('first_name'=>'Mike','last_name'=>'Cameron','user_name'=>'mikec','password' => 'welcome','email' => 'mikec@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'FR','org_id' => $orgdetails,'manager_id'  => $fredb,'other_managers' => $andreash,'usertype' => $usertype,'jobrole'  => $SalesEngineer,
								'jobtitle' =>  $solutionslead,'deptcode'  => $marketing ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						 array('input'=>array('first_name'=>'Mark','last_name'=>'McDaniel','user_name'=>'markm','password' => 'welcome','email' => 'markm@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'FR','org_id' => $orgdetails,'manager_id'  => $fredb,'other_managers' => $andreash,'usertype' => $usertype,'jobrole'  => $SalesEngineer,
								'jobtitle' =>  $marketAnalyst,'deptcode'  => $marketing ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Chang Jiang','last_name'=>'Xu','user_name'=>'Changj','password' => 'welcome','email' => 'Changj@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'CA','org_id' => $orgdetails,'manager_id'  => $fredb,'other_managers' => $andreash,'usertype' => $usertype,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $SolutionsEngineer,'deptcode'  =>$Mop ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Damon','last_name'=>'Meeks','user_name'=>'Damonm','password' => 'welcome','email' => 'Damonm@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'IT','org_id' => $orgdetails,'manager_id'  => $andreash,'other_managers' =>$manager1,'usertype' => $usertype,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $solutionslead,'deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						 array('input'=>array('first_name'=>'Gustav','last_name'=>'Bella','user_name'=>'gustavb','password' => 'welcome','email' => 'gustavb@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'CN','org_id' => $orgdetails,'manager_id'  => $manager1,'other_managers' =>$andreash,'usertype' => $usertype,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $systemAnalyst,'deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
					  array('input'=>array('first_name'=>'David','last_name'=>'Cowie','user_name'=>'Davidc','password' => 'welcome','email' => 'Davidc@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $orgdetails,'manager_id'  => $manager1,'other_managers' =>$andreash,'usertype' => $usertype,'jobrole'  => $jobrole3[0]->code,
								'jobtitle' =>  $jobtitle3[0]->code,'deptcode'  => $deptdetails ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						 array('input'=>array('first_name'=>'John','last_name'=>'David','user_name'=>'johnd','password' => 'welcome','email' => 'johnd@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'AU','org_id' => $InfoTech,'manager_id'  => $sarahj,'other_managers' =>$andreash,'usertype' => $partner,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $datamanager,'deptcode'  => $Mop ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Paul','last_name'=>'Smith','user_name'=>'pauls','password' => 'welcome','email' => 'pauls@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'AU','org_id' => $InfoTech,'manager_id'  => $sarahj,'other_managers' =>$fredb,'usertype' => $partner,'jobrole'  => $jobrole3[0]->code,
								'jobtitle' =>  $datamanager,'deptcode'  => $deptdetails ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
			        array('input'=>array('first_name'=>'Eric','last_name'=>'Scott','user_name'=>'erics','password' => 'welcome',	'email' => 'erics@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $InfoTech,'manager_id'  => $sarahj,'other_managers' =>$fredb,'usertype' => $partner,'jobrole'  => $SalesEngineer,
								'jobtitle' =>  $programManager,'deptcode'  => $Mop ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Jun','last_name'=>'Wu','user_name'=>'Junw','password' => 'welcome','email' => 'Junw@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'GB','org_id' => $InfoTech,'manager_id'  => $sarahj,'other_managers' =>$fredb,'usertype' => $partner,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $solutionslead,'deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						
						array('input'=>array('first_name'=>'Gary','last_name'=>'Elrick','user_name'=>'Garye','password' => 'welcome','email' => 'Garye@demo.com','status' => 'cre_usr_sts_atv',
								'country' => 'GB','org_id' => $InfoTech,'manager_id'  => $andreash,'other_managers' =>$fredb,'usertype' => $partner,'jobrole'  => $SystemEngineer,
								'jobtitle' =>  $solutionslead,'deptcode'  => $dept1 ,'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						
			// Kindly disable  'THE NEW USER CREATED BY ADMIN' notification before executing 
						
						/* array('input'=>array('first_name'=>'Mohana','last_name'=>'Radhakrishnan','user_name'=>'mohanark','password' => 'welcome','email' => 'mohanark@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $Expertusorg,'usertype' => $usertype,'jobrole'  => $expertusVP,
								'jobtitle' =>  $titleVP,'deptcode'  => $ClientServices ,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Caleb','last_name'=>'Johnson','user_name'=>'calebj','password' => 'welcome','email' => 'calebj@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,'jobrole'  => $Directorjrl,
								'jobtitle' =>  $Director,'deptcode'  => $Sales ,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Pierre','last_name'=>'Kergall','user_name'=>'pierrek','password' => 'welcome','email' => 'pierrek@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  => $Sales ,
								'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Thomas','last_name'=>'Bronikowski','user_name'=>'thomasb','password' => 'welcome','email' => 'thomasb@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,'jobrole'  => $Directorjrl2,
								'jobtitle' =>  $Director,'deptcode'  => $Sales ,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Wendy','last_name'=>'Sullivan','user_name'=>'wendys','password' => 'welcome','email' => 'wendys@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,
								'deptcode'  => $Sales ,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Kimberly','last_name'=>'Trattner','user_name'=>'kimt','password' => 'welcome','email' => 'kimt@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  => $ClientServices ,
								'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Holly','last_name'=>'Sheldon','user_name'=>'hollys','password' => 'welcome','email' => 'hollys@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  => $ClientServices ,
								'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Gordon','last_name'=>'Johnson','user_name'=>'gordonj','password' => 'welcome','email' => 'gordonj@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','org_id' => $Expertusorg,'usertype' => $usertype,'jobrole'  =>$VPMarketing,'jobtitle' =>  $titleVP,'deptcode'  => $marketing ,
								'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Tina','last_name'=>'Shibue','user_name'=>'tinas','password' => 'welcome','email' => 'tinas@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $gordon,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  =>  $marketing,
								'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Calvin','last_name'=>'Choi','user_name'=>'calvinc','password' => 'welcome','email' => 'calvinc@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $gordon,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  =>  $Sales,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Julien','last_name'=>'Febvre','user_name'=>'julienf','password' => 'welcome','email' => 'julienf@expertus.com','status' => 'cre_usr_sts_atv',
								'country' => 'US','manager_id' => $pierre,'org_id' => $Expertusorg,'usertype' => $usertype,'deptcode'  =>  $Sales,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>'Dan','last_name'=>'Randle','user_name'=>'danrandle','password' => 'welcome','email' => 'danrandle@expertus.com',
								'status' => 'cre_usr_sts_atv','country' => 'US','manager_id' => $mohana,'org_id' => $Expertusorg,
								'usertype' => $usertype,'deptcode'  =>  $Sales,'roles'    => 'instructor,manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						), */
						 /* array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'addr1' => 'kilpauk',
								'addr2' => 'chennai',
								'country' => 'US',
								'state'  => 'CA',
								'city'  => 'Mountain View' ,
								'zip'     => '600010',
								'phone_no' => '9999999999',
								'mobile_no' => '9999999999',
								'manager_id'  => '',
								'org_id' => '5',
								'usertype' => 'cre_usr_ptp_emp',
								'jobrole'  => 'cre_usr_jrl_rl1',
								'jobtitle' =>  'cre_usr_jtl_dtm',
								'empltype' => 'cre_usr_etp_ftm',
								'roles'    => 'instructor',
								'deptcode'  => 'cre_usr_dpt_mkt' ,
								'other_organization' => '3',
								'preferred_timezone' => 'cre_sys_tmz_007',
								'preferred_language' => 'cre_sys_lng_eng',
								'preferred_currency' => 'cre_sys_crn_usd',
								'register_sms' => '0',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						),  */
						array(
								'input'=>array(	'first_name'=>'abhirami',
										'last_name'=>'reddy',
										'user_name'=>'abhi',
										'password' => 'welcome',
										'email' => 'abhire@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'sukesh',
										'last_name'=>'reddy',
										'user_name'=>'sukeshr',
										'password' => 'welcome',
										'email' => 'sukesh@gmail.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'bhanu',
										'last_name'=>'theja',
										'user_name'=>'bhanu',
										'password' => 'welcome',
										'email' => 'bhanu@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'sumana',
										'last_name'=>'reddy',
										'user_name'=>'suma',
										'password' => 'welcome',
										'email' => 'sumana@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'sakthi',
										'last_name'=>'vel',
										'user_name'=>'sakthi',
										'password' => 'welcome',
										'email' => 'sakthi@gmail.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'nithya',
										'last_name'=>'sri',
										'user_name'=>'nitya',
										'password' => 'welcome',
										'email' => 'nithya@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'rahul',
										'last_name'=>'reddy',
										'user_name'=>'rahul',
										'password' => 'welcome',
										'email' => 'rahul@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'keerthi',
										'last_name'=>'priya',
										'user_name'=>'keerthi',
										'password' => 'welcome',
										'email' => 'keerthi@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'sobitha',
										'last_name'=>'reddy',
										'user_name'=>'sobitha',
										'password' => 'welcome',
										'email' => 'sobitha@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),
						array(
								'input'=>array(	'first_name'=>'arun',
										'last_name'=>'kumar',
										'user_name'=>'arun',
										'password' => 'welcome',
										'email' => 'arun@demo.com',
										'status' => 'cre_usr_sts_atv',
										'userid' =>1
										),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
								),

				),
				'validate' =>array(
						/* array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
						), */
						array('input'=>array('first_name'=>$random,
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
						array('input'=>array('first_name'=>$random,
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
						
						array('input'=>array('first_name'=>$random,
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
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'manager_id' => 'san' ,
								'userid' =>1
						),
								'output'=>array(''),
								'result'=>array('<short_msg>Invalid Manager'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>getRandomWord(),
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
						array('input'=>array('first_name'=>getRandomWord(),
								'last_name'=>'expee',
								'user_name'=>getRandomWord(),
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'random',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given status random doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'jobrole' => 'jobb',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given job role jobb doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.$random.'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'usertype' => 'usertype',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given user type usertype doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>getRandomWord(),
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
						array('input'=>array('first_name'=>getRandomWord(),
								'last_name'=>getRandomWord(),
								'user_name'=>getRandomWord(),
								'password' => 'welcome',
								'email' => ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_currency' => 'us dollars',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array('<short_msg>The given preferred currency is invalid or inactive status</short_msg>'),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>getRandomWord(),
								'last_name'=>getRandomWord(),
								'user_name'=>getRandomWord(),
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_language' => 'dothraki',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given preferred language dothraki doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'preferred_timezone' => 'cre_sys_tmz_100',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given preferred timezone cre_sys_tmz_100 doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' => ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'deptcode'  => 'it dept' ,
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given department it dept doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'jobtitle' =>  'manager',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given job title manager doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'empltype' => 'parttime',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given employment type parttime doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
								'status' => 'cre_usr_sts_atv',
								'country' => 'USA',
								'userid' =>1
						),
								'output'=>array('Id'),
								'result'=>array("<short_msg>The given country USA doesn't exists or inactive</short_msg>"),
								'output_type'=>'xml'
						),
						array('input'=>array('first_name'=>$random,
								'last_name'=>$random,
								'user_name'=>$random,
								'password' => 'welcome',
								'email' =>  ''.getRandomWord().'@kant.com',
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