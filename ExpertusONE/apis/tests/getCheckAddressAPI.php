<?php
$userID = db_query("select uid from uc_orders ORDER BY order_id DESC limit 1")->fetchField();
expDebug::dPrint("arulg ".print_r($userID));
$testCases=array(
	'getCheckAddressAPI' => array(
		'datasource' => array(
			array(
				'input'=>array('user_id'=>$userID,'userid'=>'1'),
				'output'=>array('mailing_name','mailing_company','mailing_street1','mailing_street2','mailing_city','mailing_zone','mailing_postal_code','mailing_country'),
				'result'=>array('<results totalrecords =','<mailing_name>','<mailing_company>','<mailing_street1>','<mailing_street2>','<mailing_city>','<mailing_zone>','<mailing_postal_code>','<mailing_country>'),
				'output_type'=>'xml'
			),
			array(
				'input'=>array('user_id'=>$userID,'userid'=>'1'),
				'output'=>array('mailing_name','mailing_company','mailing_street1','mailing_street2','mailing_city','mailing_zone','mailing_postal_code','mailing_country'),
					'result'=>array('jsonResponse','mailing_name','mailing_company','mailing_street1','mailing_street2','mailing_city','mailing_zone','mailing_postal_code','mailing_country'),
					'output_type'=>'json'
			)
		),
		'validate' => array(
			array(
				'input'=>array('user_id'=>'','userid'=>'1'),
				'output'=>array(),
				'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed.','user_id'),
				'output_type'=>'xml'
			)
		)
	)
)
?>