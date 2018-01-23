<?php

$country_code = db_query("select country_code from slt_country where country_code NOT IN (select country from slt_tax_settings) order by id desc limit 0,1")->fetchField();
expDebug::dPrint('$country_code_check$country_code_check'.$country_code);
$country_code_check = db_query("select country_code from slt_country where country_code NOT IN (select country from slt_tax_settings)order by id asc limit 0,1")->fetchField();
expDebug::dPrint('$country_code_check$country_code_check'.$country_code_check);
$state = db_query("select state_code from slt_state limit 0,1 ")->fetchField();

$ilt_code = 'ABC';
$wbt_code = 'DEF';
$vcl_code = 'GHI';
$vod_code = 'JKL';
$trp_code = 'trp';
$status = 'cme_tax_sts_atv';
$already_added_code = db_query("select country from slt_tax_settings order by id desc limit 0,1")->fetchField();
$testCases = array(
		'AddTaxAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'country'=>$country_code,'vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>$status),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "">','<result>'),
								'output_type'=>'xml'
						)
				),
				'validate'=>array(
				
						array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'country'=>'%%','vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>$status),
								'output'=>array(),
								'result'=>array('Invalid Country Code.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'country'=>$already_added_code,'vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>$status),
								'output'=>array(),
								'result'=>array('Tax already added to this Country.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'country'=>$country_code_check,'states'=>$state,'vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>$status),
								'output'=>array(),
								'result'=>array('States only can add For United States or Canada.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'country'=>'US','states'=>'A','vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>$status),
								'output'=>array(),
								'result'=>array('Invalid State'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'country'=>$country_code_check,'vat_number'=>'VAT200','lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>01,'status'=>'aa'),
								'output'=>array(),
								'result'=>array('Invalid Status',),
								'output_type'=>'xml'
						)

			)
		)
	);
?>