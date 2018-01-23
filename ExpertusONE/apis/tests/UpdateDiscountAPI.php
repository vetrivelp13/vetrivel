<?php

    function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
    }
    $random_word = getRandomWord();
    $random_num = rand(2,1000);
    $disc = db_query("select * from uc_discounts order by discount_id DESC limit 1  ")->fetchAll();
    $code = db_query("select * from uc_discounts_codes order by discount_id DESC limit 1  ")->fetchAll();
    $currency = db_query("select * from slt_discounts order by uc_discount_id DESC limit 1  ")->fetchAll();
    
    
    $count=count($disc);
	expDebug::dPrint(" CheckingAbhi ".print_r($disc));
	expDebug::dPrint(" Checking ".print_r($count));
    if ($disc[0]->discount_type == 2)
    {
    	$typ = "percentage_off";
    	$curren = '';
    }
    else if ($disc[0]->discount_type == 3)
    {
    	$typ = "fixed_amount_off";
        $curren = $currency[0]->currency_type;
    }
 $testCases = array(
			'UpdateDiscountAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$curren,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
						 'output'=>array('Id'),
								'result'=>array('<results totalrecords =','<Id>'),
								'output_type'=>'xml'
							)
				),
					
				'validate' => array(
					array('input'=>array('userid'=>1,'id'=>'','name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
						'output'=>array(),
						 'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','id'),
						 'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>'','description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','name'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>'','type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','description'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>'','is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','type'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>'','value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','is_active'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>'','currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','value'),
								'output_type'=>'xml'
						),
						// need not check for "percentage-off"
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>'','max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Currency Type should not be null for fixed_amount_off','Currency Type should not be null for fixed_amount_off'),
								'output_type'=>'xml'
						),
 
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>'','min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','max_uses'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>'','max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','min_price'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>'','max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','max_uses_per_user'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>'','code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','max_uses_per_code'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>'','for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','code'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>'','for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','for_all_trainings'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>'','combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','for_all_users'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Mandatory Fields are missing in the feed','combine_with_others'),
								'output_type'=>'xml'
						),
				//INVALID DATA TYPES
						array('input'=>array('userid'=>1,'id'=>$random_word,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$typ,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','No discount found for given id.','No discount found for given id.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$disc[0]->discount_id,'name'=>$disc[0]->name,'description'=>$disc[0]->description,'type'=>$random_word,'is_active'=>$disc[0]->is_active,'value'=>$disc[0]->discount_amount,'currency_type'=>$currency[0]->currency_type,'max_uses'=>$disc[0]->max_uses,'min_price'=>$disc[0]->qualifying_amount,'max_uses_per_user'=>$disc[0]->max_uses_per_user,'max_uses_per_code'=>$disc[0]->max_uses_per_code,'code'=>$code[0]->code,'for_all_trainings'=>1,'for_all_users'=>1,'combine_with_others'=>$disc[0]->can_be_combined_with_other_discounts),
								'output'=>array(),
								'result'=>array('<error>','<error_code>','Type can be either percentage_off or fixed_amount_off','Type can be either percentage_off or fixed_amount_off'),
								'output_type'=>'xml'
						),
						
						
			)
		 )
 		)
 	?>