<?php 
$testCases = array(

		'listCountriesAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'user_id'=>5),
								'output'=>array('country_code','country_name'),
								'result'=>array('<results totalrecords =','<country_code>','<country_name>'),
								'output_type'=>'xml'
						)
				), // End of datasource
				'validate' => array(
						array('input'=>array('userid'=>1,'user_id'=>5),
								'output'=>array('country_code','country_name'),
								'result'=>array('<results totalrecords =','<country_code>','<country_name>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed.'),
								'output_type'=>'xml'
						)
						

				) // End of validate
					
		), // End of listCountriesAPI array
	); // End of main array
?>