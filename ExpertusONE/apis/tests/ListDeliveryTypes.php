<?php 

$testCases = array(

			// ListDeliveryTypes test case Starts
			'ListDeliveryTypes' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'deliveryTypeCode'=>'lrn_cls_dty','limit'=>10),
						  'output'=>array('name','code','attr1','attr2'),
						  'result'=>array('<results totalrecords =','<result>','<name>','<code>','<attr1>','<attr2>'),
						  'output_type'=>'xml'
							)
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'deliveryTypeCode'=>'lrn_cls_dty','limit'=>10),
						  'output'=>array('name','code','attr1','attr2'),
						  'result'=>array('<results totalrecords =','<result>','<name>','<code>','<attr1>','<attr2>'),
						  'output_type'=>'xml'
							),
					array(
						  'input'=>array('userid'=>1,'deliveryTypeCode'=>'','limit'=>10),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>','deliveryTypeCode'),
					      'output_type'=>'xml'
						),
					array(
						  'input'=>array('userid'=>1,'deliveryTypeCode'=>'asde','limit'=>10),
						  'output'=>array(),
					      'result'=>array('<error>','<error_code>','<short_msg>deliveryType Code is invalid </short_msg>'),
					      'output_type'=>'xml'
						)
				)
			),  // ListDeliveryTypes test case Ends
			
		); // End of api test case main array
?>