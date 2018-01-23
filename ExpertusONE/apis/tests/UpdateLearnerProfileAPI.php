<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);

    $usr = db_query("select * from users order by uid DESC limit 1  ")->fetchAll();
   // expDebug::dPrint(" CheckingAbhi ".print_r($usr));
    $testCases = array(
    		'UpdateLearnerProfileAPI' => array(
    				'datasource' => array(
    						array('input'=>array('userid'=>$usr[0]->uid,'timezone'=>$usr[0]->timezone,'oldtimezone'=>$usr[0]->timezone,'imgdata'=>$usr[0]->picture),
    								'output'=>array('status'),
    								'result'=>array('<results totalrecords =','<status>'),
    								'output_type'=>'xml'
    						),
    						array('input'=>array('userid'=>$usr[0]->uid,'timezone'=>$usr[0]->timezone,'oldtimezone'=>$usr[0]->timezone,'imgdata'=>$usr[0]->picture),
    								'output'=>array('status'),
    								'result'=>array('jsonResponse','status'),
    								'output_type'=>'json'
    						)
    				),
    				
    		        'validate' => array(
    						array('input'=>array('userid'=>$usr[0]->uid,'timezone'=>'','oldtimezone'=>$usr[0]->timezone,'imgdata'=>$usr[0]->picture),
    		        		    'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed.','timezone'),
    		                    'output_type'=>'xml'
    						),
    						array('input'=>array('userid'=>$usr[0]->uid,'timezone'=>$usr[0]->timezone,'oldtimezone'=>'','imgdata'=>$usr[0]->picture),
    		        		    'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed.','oldtimezone'),
    		                    'output_type'=>'xml'
    						),
    		        		array('input'=>array('userid'=>$usr[0]->uid,'timezone'=>$usr[0]->timezone,'oldtimezone'=>$usr[0]->timezone,'imgdata'=>''),
    		        				'result'=>array('<error>','<error_code>'."5".'</error_code>','Invalid Timezone','Invalid Timezone'),
    		        				'output_type'=>'xml'
    		        		),
    			)
    		)
    
    )
    
    
 ?>