<?php 
$activestatus = "lrn_cls_sts_atv";
$inactivestatus="lrn_cls_sts_itv";
$webbased="lrn_cls_dty_wbt";
$classroom="lrn_cls_dty_ilt";
$video="lrn_cls_dty_vod";
$virtualclass="lrn_cls_dty_vcl";
$catalogtype="Class";
$language ="cre_sys_lng_eng";
$location =  db_query('select name from slt_location order by id desc limit 1')->fetchField();
$limit = 8;
//$classes = db_query("select id,status from slt_course_class where status =:status",array(":status"=>$status))->fetchAll();
//$count=count($classes);
//expDebug::dPrint("jooooooo1 ".print_r($classes,true),1);

$testCases = array(

		'ListClassesAPI' => array(
				'datasource' => array(
						array('input'=>array('catalogtype'=>'Class','userid'=>1),
								'output'=>array('class_id','id','title','type','code','delivery_type','price','currency','currency_code','currency_symbol','short_desc','status','status_code','location_name','lang','promote','duration','author_vendor','is_compliance'),
								'result'=>array('<results totalrecords =','<class_id>','<id>','<title>','<type>','<code>','<delivery_type>','<price>','<currency>','<currency_code>','<currency_symbol>','<short_desc>','<status>','<status_code>','<location_name>','<lang>','<promote>','<duration>','<author_vendor>','<is_compliance>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'start'=>1,
								'limit'=>100,
								'rows'=>'',
								'catalogtype'=>$catalogtype,
								'textfilter'=>'',
								'catalogclassstatus'=>'',
								'deliverytype'=>$webbased,
								'classlangtype'=>$language,
								'classLocation'=>'',
								'userid'=>1
								
						),
								'output'=>array('class_id','id','title','type','code','delivery_type','price','currency','currency_code','currency_symbol','short_desc','status','status_code','location_name','lang','promote','duration','author_vendor','is_compliance'),
								'result'=>array('<results totalrecords =','<class_id>','<id>','<title>','<type>','<code>','<delivery_type>','<price>','<currency>','<currency_code>','<currency_symbol>','<short_desc>','<status>','<status_code>','<location_name>','<lang>','<promote>','<duration>','<author_vendor>','<is_compliance>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'start'=>1,
								'limit'=>100,
								'rows'=>'',
								'catalogtype'=>$catalogtype,
								'textfilter'=>'',
								'catalogclassstatus'=>'',
								'deliverytype'=>$classroom,
								'classlangtype'=>$language,
								'classLocation'=>$location,
								'userid'=>1
						
						),
								'output'=>array('class_id','id','title','type','code','delivery_type','price','currency','currency_code','currency_symbol','short_desc','status','status_code','location_name','lang','promote','duration','author_vendor','is_compliance'),
								'result'=>array('<results totalrecords =','<class_id>','<id>','<title>','<type>','<code>','<delivery_type>','<price>','<currency>','<currency_code>','<currency_symbol>','<short_desc>','<status>','<status_code>','<location_name>','<lang>','<promote>','<duration>','<author_vendor>','<is_compliance>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'start'=>1,
								'limit'=>100,
								'rows'=>'',
								'catalogtype'=>$catalogtype,
								'textfilter'=>'',
								'catalogclassstatus'=>'',
								'deliverytype'=>$video,
								'classlangtype'=>$language,
								'classLocation'=>'',
								'userid'=>1
						
						),
								'output'=>array('class_id','id','title','type','code','delivery_type','price','currency','currency_code','currency_symbol','short_desc','status','status_code','location_name','lang','promote','duration','author_vendor','is_compliance'),
								'result'=>array('<results totalrecords =','<class_id>','<id>','<title>','<type>','<code>','<delivery_type>','<price>','<currency>','<currency_code>','<currency_symbol>','<short_desc>','<status>','<status_code>','<location_name>','<lang>','<promote>','<duration>','<author_vendor>','<is_compliance>'),
								'output_type'=>'xml'
						),
						array('input'=>array(
								'start'=>1,
								'limit'=>100,
								'rows'=>'',
								'catalogtype'=>$catalogtype,
								'textfilter'=>'',
								'catalogclassstatus'=>'',
								'deliverytype'=>$virtualclass,
								'classlangtype'=>$language,
								'classLocation'=>'',
								'userid'=>1
						
						),
								'output'=>array('class_id','id','title','type','code','delivery_type','price','currency','currency_code','currency_symbol','short_desc','status','status_code','location_name','lang','promote','duration','author_vendor','is_compliance'),
								'result'=>array('<results totalrecords =','<class_id>','<id>','<title>','<type>','<code>','<delivery_type>','<price>','<currency>','<currency_code>','<currency_symbol>','<short_desc>','<status>','<status_code>','<location_name>','<lang>','<promote>','<duration>','<author_vendor>','<is_compliance>'),
								'output_type'=>'xml'
						),
				),
				'validate' => array(
						array('input'=>array(
								'start'=>1,
								'limit'=>100,
								'rows'=>'',
								'catalogtype'=>'',
								'textfilter'=>'',
								'catalogclassstatus'=>'',
								'deliverytype'=>$video,
								'classlangtype'=>$language,
								'classLocation'=>'',
								'userid'=>1
						),
								'output'=>array(),
								'result'=>array('Mandatory Fields are missing in the feed'),
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