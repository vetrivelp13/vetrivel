<?php
    
$banner = db_query("select id from slt_person")->fetchAll();
$count=count($banner);
expDebug::dPrint(" Checking ".print_r($count));
$testCases = array(

			
			'ListBannersAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','shortdesc','banner_thumbnail','banner_large','display_from','display_to','banner_sequence','language','status'),
								'result'=>array('<results totalrecords =','<id>','<title>','<shortdesc>','<banner_thumbnail>','<banner_large>','<display_from>','<display_to>','<banner_sequence>','<language>','<status>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','title','shortdesc','banner_thumbnail','banner_large','display_from','display_to','banner_sequence','language','status'),
								'result'=>array('jsonResponse','id','title','shortdesc','banner_thumbnail','banner_large','display_from','display_to','banner_sequence','language','status'),
								'output_type'=>'json'
							),
				),
			'validate' => array(
				array('input'=>array('userid'=>'','limit'=>''),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
							),	
				//$count returns the number of existing users. Hence, "$count+ 1" gives the ID of user who has not been created till now.	
				array('input'=>array('userid'=>$count + 1,'limit'=>''),
								'result'=>array('<error>','<error_code>'."L_007".'</error_code>','Invalid User','Invalid User'),
								'output_type'=>'xml'
							),
				
				
			)
		)	
	)
?>