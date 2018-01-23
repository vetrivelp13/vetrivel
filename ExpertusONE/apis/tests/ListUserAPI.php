<?php 
$qry = "select id, name from slt_organization where id=1";
$org_name = db_query($qry)->fetchAll();
expDebug::dPrint("Vincen test -- ".print_r($org_name,true),1);
$qry = "select id, user_name from slt_person where org_id=1";
$users = db_query($qry)->fetchAll();
$person = db_query("select id,user_name from slt_person where status =  'cre_usr_sts_itv' order by id desc limit 1")->fetchAll();
$manager = db_query("select id,user_name from slt_person where is_manager= 'Y' order by id asc limit 1")->fetchAll();
$testCases = array(

		'ListUserAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'limit'=>10),
								'output'=>array('id','username','firstname','lastname','email'),
								'result'=>array('<results totalrecords =','<firstname>','<email>'),
								'output_type'=>'xml'
						)
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'limit'=>10),
								'output'=>array('id','username','firstname','lastname','email','mobile_no','orgname','status_code','groups','deptcode','jobtitle','jobrole','usertype','currency','preferredlanguagecode','timezonecode','manager_id','othermanagerid'),
								'result'=>array('<results totalrecords =','<firstname>','<email>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'userorg'=>$org_name[0]->name),
								'output'=>array('id','username','firstname','lastname','email','orgname'),
								'result'=>array('<results totalrecords =','<firstname>','<email>','<orgname>'.$org_name[0]->name.'</orgname>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'userstatus'=>'cre_usr_sts_itv'),
								'output'=>array('id','username','firstname','lastname','email','orgname'),
								'result'=>array('<results totalrecords =','<firstname>','<email>','<id>'.$person[0]->id.'</id>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'mgrusername'=>$manager[0]->user_name ),
								'output'=>array('id','username','firstname','lastname','email','orgname','manager_id'),
								'result'=>array('<results totalrecords =','<firstname>','<email>','<manager_id>'.$manager[0]->id.'</manager_id>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'location'=>'chennai' ),
								'output'=>array('id','username','firstname','lastname','email','orgname','manager_id','city'),
								'result'=>array('<results totalrecords =','<firstname>','<email>','<city>chennai</city>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'usertype'=>'cre_usr_ptp_emp' ),
								'output'=>array('id','username','firstname','lastname','email','orgname','manager_id','usertype'),
								'result'=>array('<results totalrecords =','<firstname>','<email>','<usertype>Employee</usertype>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'limit'=>10,'empltype'=>'cre_usr_ptp_emp' ),
								'output'=>array('id','username','firstname','lastname','email','orgname','manager_id','usertype'),
								'result'=>array('<results totalrecords =','<firstname>','<email>'),
								'output_type'=>'xml'
						),

				)
		),
			
);
?>