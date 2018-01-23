<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random_word = getRandomWord();
$tax = db_query("select id from slt_tax_settings")->fetchAll();
$count=count($tax);
$testCases = array(

			
			'ListTaxAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1),
								'output'=>array('id','country','states','vat_numbers','status'),
								'result'=>array('<results totalrecords =','<id>','<country>','<states>','<vat_numbers>','<status>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1),
								'output'=>array('id','country','states','vat_numbers','status'),
								'result'=>array('jsonResponse','id','country','states','vat_numbers','status'),
								'output_type'=>'json'
							),
				),
			//count the number of tax setups.		
				'validate' => array(
						array('input'=>array('userid'=>1,'start'=>'','limit'=>''),
								'output'=>array('id','country','states','vat_numbers','status'),
								'result'=>array('<results totalrecords = "'.$count.'">','id','country','states','vat_numbers','status'),
								'output_type'=>'xml'
						),
					
			)
	  )
	)
?>