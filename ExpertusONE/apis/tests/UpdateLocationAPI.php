<?php
 function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
 }
 $random_word = getRandomWord();
 $random_num = rand(1,10000);
    $loc = db_query("select * from slt_location order by id DESC limit 1  ")->fetchAll();
	$count=count($loc);
	expDebug::dPrint(" Checking ".print_r($count));
$testCases = array(
		//$count is 1 and so count-1 is 0. $loc[count-1] is  the last updated value.
			'UpdateLocationAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'output_type'=>'xml'
							)
				),
			
			'validate' => array(
				array('input'=>array('userid'=>1,'loc_id'=>'','name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','loc_id'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>'' ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','name'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>'','city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','capacity'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>'','state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','city'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>'','country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','state'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>'','zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','country'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>'','status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','zipcode'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>'','timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','status'),
								'output_type'=>'xml'
							),	
					array('input'=>array('userid'=>1,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>''),
						 'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed','timezone'),
								'output_type'=>'xml'
							),	
					
			// loc_id is not correct(any random word)	
			  	
					array('input'=>array('userid'=>1,'loc_id'=>$random_word,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."5".'</error_code>','Location does not exist','Location does not exist'),
								'output_type'=>'xml'
							),		
			// loc_id is not correct(any random number)	
					array('input'=>array('userid'=>1,'loc_id'=>$random_num,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."5".'</error_code>','Location does not exist','Location does not exist'),
								'output_type'=>'xml'
							),	
					 array('input'=>array('userid'=>'','loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_010".'</error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
							),	
					 array('input'=>array('userid'=>$random_word,'loc_id'=>$loc[$count-1]->id,'name'=>$loc[$count-1]->name ,'capacity'=>$loc[$count-1]->capacity,'city'=>$loc[$count-1]->city,'state'=>$loc[$count-1]->state,'country'=>$loc[$count-1]->country,'zipcode'=>$loc[$count-1]->zipcode,'status'=>$loc[$count-1]->status,'timezone'=>$loc[$count-1]->timezone),
						 'result'=>array('<error>','<error_code>'."L_010".'</error_code>','INVALID ACCESS','Userid is invalid'),
								'output_type'=>'xml'
							),	
					 
						)
				
				
				
				
			)
		)

    
    
    
    
    
    
?>