<?php

function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$classid= db_query("select id from slt_course_class order by id desc limit 1")->fetchField();;
$sessionid = db_query("select id from slt_course_class_session where class_id =:class_id order by id desc limit 1",array(":class_id"=>$classid))->fetchField();
expDebug::dprint("test-->1".print_r($classid,true),1);
expDebug::dprint("test-->22".print_r($sessionid,true),1);
$random = getRandomWord();
$testCases = array(
			// List GetClassDetailsAPI test case Starts
			'GetClassDetailsAPI' => array(
				'datasource' => array(
					array('input'=>array('classId'=>$classid[0]->id,'userid'=>1),
								'output'=>array('Title','Code','classid','crs_id','description','duration','clslang','delivery_type_code','price','currency','currency_code','currency_symbol','maxcapacity','availablibleseatcount','enrollmentvalidity','mro_id','prerequisites','equivalence','sessionDetailInfo','available_seats','waitlist_seats','multi_register'),
								'result'=>array('<results totalrecords =','<Title>','<Code>','<classid>','<crs_id>','<description>','<duration>','<clslang>','<delivery_type_code>','<price>','<currency>','<currency_code>','<currency_symbol>','<maxcapacity>','<availablibleseatcount>','<enrollmentvalidity>','<mro_id>','<prerequisites>','<equivalence>','<sessionDetailInfo>','<available_seats>','<waitlist_seats>','<multi_register>'),
								'output_type'=>'xml'
							)
				),
				'validate' => array(
					array('input'=>array('classId'=>$random,'sessionId'=>$sessionid[0]->id,'userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."5".'</error_code>','<short_msg>'."ClassID is Invalid.".'</short_msg>'),
								'output_type'=>'xml'
							),
					array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
							),
					array('input'=>array('classId'=>'','userid'=>1),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','<short_msg>'."Mandatory Fields are missing in the feed. List provided below:".'</short_msg>','classId'),
								'output_type'=>'xml'
							)
				)
			),  // List GetClassDetailsAPI test case Ends
			
		); // End of api test case main array
?>