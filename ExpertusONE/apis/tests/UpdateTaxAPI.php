<?php 
$id = db_query("select id from slt_tax_settings order by id desc limit 0,1")->fetchField();
$country_code = db_query("select country from slt_tax_settings where id = $id")->fetchField();
expDebug::dPrint('Country Coddee--'.$country_code);
$id_India = db_query("select id from slt_tax_settings where country = 'IN'")->fetchField();
$id_US = db_query("select id from slt_tax_settings where country = 'US'")->fetchField();

$vat_num = db_query("select vat_numbers from slt_tax_settings where id = $id")->fetchField();
$ilt_code = db_query("select tax_product_code from slt_tax_product_code_mapping where tax_settings_id=$id and delivery_type='lrn_cls_dty_ilt' ")->fetchField();
$vod_code = db_query("select tax_product_code from slt_tax_product_code_mapping where tax_settings_id=$id and delivery_type='lrn_cls_dty_vod' ")->fetchField();
$vcl_code = db_query("select tax_product_code from slt_tax_product_code_mapping where tax_settings_id=$id and delivery_type='lrn_cls_dty_vcl' ")->fetchField();
$wbt_code = db_query("select tax_product_code from slt_tax_product_code_mapping where tax_settings_id=$id and delivery_type='lrn_cls_dty_wbt' ")->fetchField();
$trp_code = db_query("select tax_product_code from slt_tax_product_code_mapping where tax_settings_id=$id and delivery_type='cre_sys_obt_trp' ")->fetchField();

$country_code_new = db_query("select country_code from slt_country where country_code NOT IN (select country from slt_tax_settings)order by id asc limit 0,1")->fetchField();

$active_status = 'cme_tax_sts_atv';
$inactive_status = 'cme_tax_sts_itv';

$testCases = array(
		'UpdateTaxAPI' => array(
				'datasource' => array(
						array('input'=>array('userid'=>1,'id'=>$id,'country'=>$country_code,'vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords = "'),
								'output_type'=>'xml'
						)
				),
				'validate'=>array(

						array('input'=>array('userid'=>1),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id+1,'country'=>$country_code,'vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
								'output'=>array(),
								'result'=>array('Tax id does not exist.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id,'country'=>'%','vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
								'output'=>array(),
								'result'=>array('Invalid Country Code.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id,'country'=>$country_code_new,'vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
								'output'=>array(),
								'result'=>array('Country Code will not change.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id_India,'country'=>'IN','states'=>01,'vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
								'output'=>array(),
								'result'=>array('States only can add For United States or Canada.'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id_US,'country'=>'US','states'=>'%','vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>$inactive_status),
								'output'=>array(),
								'result'=>array('Invalid State'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'id'=>$id,'country'=>$country_code,'vat_number'=>$vat_num,'lrn_cls_dty_ilt'=>$ilt_code,'lrn_cls_dty_vcl'=>$vcl_code,'lrn_cls_dty_vod'=>$vod_code,'lrn_cls_dty_wbt'=>$wbt_code,'cre_sys_obt_trp'=>$trp_code,'status'=>'aa'),
								'output'=>array(),
								'result'=>array('Invalid Status'),
								'output_type'=>'xml'
						)
						

  )
 )
);
?>