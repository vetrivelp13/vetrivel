<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $learner_id = db_query("select id from slt_person order by id DESC limit 1 ")->fetchField();
	//$count=count($surv_ques);
	expDebug::dPrint(" Checking ".print_r($learner_id));

 $testCases = array(
			'LearnerProfileAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'learnerid'=>$learner_id,'action'=>"Login",'rows'=>4,'page'=>4,'device_token'=>"Registered device token",'currency_code'=>"USD"),
						 'output'=>array('id','first_name','last_name','email','phone_no','mobile_no','timezone','is_instructor','is_manager','is_training_admin','about_me','job_title','person_pref_language','person_location_city','person_location_name','avatarimage','totalpoints','badges','skillset','activitydetails','courselevelview','commerce_enabled','enabledLanguages','iosurl','androidurl','cdn_status','video_streaming','notification_notviewed','preferred_currency','preferred_currency_name','min_price','max_price','enabled_currency','activitycount'),
						 'result'=>array('<results totalrecords =','<id>','<first_name>','<last_name>','<email>','<phone_no>','<mobile_no>','<timezone>','<is_instructor>','<is_manager>','<is_training_admin>','<about_me>','<job_title>','<person_pref_language>','<person_location_city>','<person_location_name>','<avatarimage>','<totalpoints>','<badges>','<skillset>','<activitydetails>','<courselevelview>','<commerce_enabled>','<enabledLanguages>','<iosurl>','<androidurl>','<cdn_status>','<video_streaming>','<notification_notviewed>','<preferred_currency>','<preferred_currency_name>','<min_price>','<max_price>','<enabled_currency>','<activitycount>'),
						  'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'learnerid'=>$learner_id,'action'=>"Login",'rows'=>4,'page'=>4,'device_token'=>"Registered device token",'currency_code'=>"USD"),
								'output'=>array('id','first_name','last_name','email','phone_no','mobile_no','timezone','is_instructor','is_manager','is_training_admin','about_me','job_title','person_pref_language','person_location_city','person_location_name','avatarimage','totalpoints','badges','skillset','activitydetails','courselevelview','commerce_enabled','enabledLanguages','iosurl','androidurl','cdn_status','video_streaming','notification_notviewed','preferred_currency','preferred_currency_name','min_price','max_price','enabled_currency','activitycount'),
								'result'=>array('jsonResponse','id','first_name','last_name','email','phone_no','mobile_no','timezone','is_instructor','is_manager','is_training_admin','about_me','job_title','person_pref_language','person_location_city','person_location_name','avatarimage','totalpoints','badges','skillset','activitydetails','courselevelview','commerce_enabled','enabledLanguages','iosurl','androidurl','cdn_status','video_streaming','notification_notviewed','preferred_currency','preferred_currency_name','min_price','max_price','enabled_currency','activitycount'),
								'output_type'=>'json'
						),
				),
			
				'validate' => array(
						array('input'=>array('userid'=>1,'learnerid'=>'','action'=>"Login",'rows'=>4,'page'=>4,'device_token'=>"Registered device token",'currency_code'=>"USD"),
							  'output'=>array(),
							  'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed.','learnerid'),
							  'output_type'=>'xml'
							  ),
						array('input'=>array('userid'=>1,'learnerid'=>$learner_id,'action'=>"Login",'rows'=>4,'page'=>4,'device_token'=>"Registered device token",'currency_code'=>"USD"),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_005".'</error_code>','Mandatory Fields are missing in the feed.','learnerid'),
								'output_type'=>'xml'
						),
			)
 		)
 	)
 		
 	?>