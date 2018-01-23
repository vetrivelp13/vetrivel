<?php 

//$discountstatus = db_query("select is_active from uc_discounts order by discount_id desc limit 1")->fetchField();

$testCases = array(

		'ListDiscountAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>1),
								'output'=>array('id','is_active'),
								'result'=>array('<results totalrecords =','<is_active>'."Yes".'</is_active>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>0),
								'output'=>array('id','is_active'),
								'result'=>array('<results totalrecords =','<is_active>'."No".'</is_active>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>0,'discount_type'=>'percentage_off'),
								'output'=>array('id','is_active','type'),
								'result'=>array('<results totalrecords =',' <type>'."percentage_off".'</type>','<is_active>'."No".'</is_active>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>0,'discount_type'=>'fixed_amount_off'),
								'output'=>array('id','is_active','type'),
								'result'=>array('<results totalrecords =',' <type>'."fixed_amount_off".'</type>','<is_active>'."No".'</is_active>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>1,'discount_type'=>'percentage_off'),
								'output'=>array('id','is_active','type'),
								'result'=>array('<results totalrecords =',' <type>'."percentage_off".'</type>','<is_active>'."Yes".'</is_active>'),
								'output_type'=>'xml'
						),
						array(
								'input'=>array('userid'=>1,'discount_status'=>1,'discount_type'=>'fixed_amount_off'),
								'output'=>array('id','is_active','type'),
								'result'=>array('<results totalrecords =',' <type>'."fixed_amount_off".'</type>','<is_active>'."Yes".'</is_active>'),
								'output_type'=>'xml'
						),
						/*array('input'=>array('userid'=>1,'sortby'=>'AZ'),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'sortby'=>'ZA'),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'sortby'=>'dateOld'),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'sortby'=>'type'),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						)*/
				),
				'validate' => array(
						array('input'=>array('userid'=>1,'limit'=>10),
								'output'=>array('id','name','description','type','is_active','value','max_uses','min_price','max_uses_per_user','max_uses_per_code','code','for_all_trainings','for_all_users','combine_with_others','trainings','groups'),
								'result'=>array('<results totalrecords =','<id>','<name>','<description>','<type>','<is_active>','<value>','<max_uses>','<min_price>','<max_uses_per_user>','<max_uses_per_code>','<code>','<for_all_trainings>','<for_all_users>','<combine_with_others>','<trainings>','<groups>'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>''),
								'output'=>array(),
								'result'=>array('<error>','<error_code>'."L_010".'</error_code>','<short_msg>'."INVALID ACCESS".'</short_msg>','<long_msg>'."Userid is invalid".'</long_msg>','id'),
								'output_type'=>'xml'
						)

				)
		),
			
);
?>