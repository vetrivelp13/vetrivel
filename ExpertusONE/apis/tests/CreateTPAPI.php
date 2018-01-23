<?php 
$Today=date('m-d-Y');
$NewDate=Date('m-d-Y', strtotime("+4 days"));
$program_code=db_query("select code from slt_program limit 0,1")->fetchField();
$title=db_query("select title from slt_program limit 0,1")->fetchField();
expDebug::dPrint(' addNewTPByRestAPI created tp:' . print_r($program_code,true) ,4);
expDebug::dPrint(' addNewTPByRestAPI created tp:' . print_r($title,true) ,4);
expDebug::dPrint(' addNewTPByRestAPI created tp:' . print_r($NewDate,true) ,4);
$days=10;
$price=10;
$testCases = array(

			// CreateTPAPI test case Starts
			'CreateTPAPI' => array(
				'datasource' => array(
					
			/*1*/	array('input'=>array('userid'=>1,
					'title'=>'Sales Engineer Certification',
					'code'=>'CERD-SEC',
					'value'=>'The Sales Engineer Alliance is created for the sole purpose of advancing the role of the Sales Engineer. .  A necessary component of this is accreditation of training programs that develop these specialized non-technical skills.',
					'status'=>'lrn_lpn_sts_itv',
					'object_type'=>'cre_sys_obt_crt',
					'lang_code'=>'cre_sys_lng_eng',
					'expires_in_value'=>$days,
					'expires_in_unit'=>'days'),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						
						

		   /*2*/   array('input'=>array('userid'=>1,
		   		'title'=>'New Hire Orientation',
		   		'code'=>'CURD-NHO',
		   		'value'=>'Effectively orienting new employees to the campus and to their positions is critical to establishing successful, productive working relationships. The employees first interactions with you should create a positive impression of your department and the campus.',
		   		'status'=>'lrn_lpn_sts_itv',
		   		'object_type'=>'cre_sys_obt_cur',
		   		'lang_code'=>'cre_sys_lng_eng'),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
						
		 /*3*/   array('input'=>array('userid'=>1,
		 		'title'=>'Innovation Management Program:  High-Level Overview',
		 		'code'=>'LEPD-IMP',
		 		'value'=>'The Innovation Management module helps solution seekers publish their needs, collect and validate ideas from problem solvers, and enrich and mature the best ideas into smartly screened proposals. Ultimately, it helps you make the crucial Go/No-Go decisions with confidence',
		 		'status'=>'lrn_lpn_sts_itv',
		 		'object_type'=>'cre_sys_obt_trn',
		 		'lang_code'=>'cre_sys_lng_eng',
		 		'end_date'=>$NewDate),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
		/*4*/   array('input'=>array('userid'=>1,
				'title'=>'ExpertusONE – How To Curriculum for Admins',
				'code'=>'CURR-HOW','value'=>'ExpertusONE- How to Curriculum for Admins are only a few minutes long videos that will help you quickly understand ExpertusONE terminologies and just how easy it is to interact with the Administration features such as, "How to Create a Course and Class", “Setting up your own Certification/Training”, "Managing eLearning and Video Content" and more.',
				'status'=>'lrn_lpn_sts_itv',
				'object_type'=>'cre_sys_obt_cur',
				'lang_code'=>'cre_sys_lng_eng'),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
		/*5*/	array('input'=>array('userid'=>1,
				'title'=>'Health Informatics - Curriculum',
				'code'=>'CHI-CUR',
				'value'=>'The objective of this program is to expose students to modern health IT, including heath data collection, processing, and storage. This program primarily focuses on electronic medical data generated and stored in health care and public health organizations. Individuals who complete the Health Informatics certificate program will have a solid knowledge of health information technology and systems and electronic health records as well as a solid exposure to the latest medical technologies. Students who complete the Graduate Certificate in Health Informatics will be able to demonstrate: An understanding of the American health care system, medical terminology, basic human anatomy and physiology, disease processes, diagnostic modalities, and treatments associated with common disease processes.Advanced knowledge of the functionality, technical infrastructure, and best-practice deployment of health care IT, including medical algorithms, electronic health records, privacy and security, and regulations.Proficiency in managing, processing, and analyzing medical data.Competence sufficient to lead health IT initiatives, to conduct biomedical research, and to design, implement, and manage advanced solutions.',
				'status'=>'lrn_lpn_sts_itv',
				'object_type'=>'cre_sys_obt_cur',
				'lang_code'=>'cre_sys_lng_eng'),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
		/*6*/	array('input'=>array('userid'=>1,
				'title'=>'Certificate in Applied Business Analytics (25 Units)',
				'code'=>'CABA-CERT',
				'value'=>'The online Graduate Certificate in Applied Business Analytics provides comprehensive coverage of data analytics concepts, techniques, and state-of-the-art tools used in the process of data-driven business decision-making. Students have the opportunity to gain hands-on experience with a variety of analytical models and decision-support tools and to apply them to interlinked data-inputs and large data sets in the areas of marketing, operations, product and technology innovations, financial services, human resources management, and others. The curriculum covers advanced software tools and functions such as predictive modeling, text and data mining, visual analytics, simulations, and OLAP tables. Graduates of the program will be able to analyze data-driven business processes, select appropriate analytical methods to monitor and identify performance issues, and propose optimal data-based solutions.Students who complete the Graduate Certificate in Applied Business Analytics will be able to demonstrate:The knowledge and skills to better utilize available information in operational, tactical, and strategic decision making in organizations.The ability to apply various powerful emerging technologies and techniques for increasing the value of both in-house and third party data sets.An understanding of how organizations are using interlinked data-inputs, analytics models, and decision-support tools to better understand their operations, customers, and markets.The skills to understand web analytics and metrics, procure and process unstructured text, and delve into their hidden patterns.The expertise to facilitate knowledge discovery using data mining and visualization techniques over vast amounts of data.',
				'status'=>'lrn_lpn_sts_itv',
				'object_type'=>'cre_sys_obt_crt',
				'lang_code'=>'cre_sys_lng_eng',
				'expires_in_value'=>$days,
				'expires_in_unit'=>'days'),
								'output'=>array('id'),
								//'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),

		/*7*/	array('input'=>array('userid'=>1,
				'title'=>'Information Security',
				'code'=>'IS-LP',
				'value'=>'In general, information security means protecting information from unauthorized access, disclosure, or destruction. Information security is important in today’s world because almost all organizations, including government agencies, hospitals, insurance companies, and private businesses, store various kinds of information about their employees and customers. If any business-related information is confidential, the security of that data becomes crucial. For example, if critical business data (e.g., credit card numbers) is leaked to competitors or hackers, it could lead to loss of business, lawsuits, and even bankruptcy. The online Graduate Certificate in Information Security will touch upon various aspects of information security, including IT security policies and how digital forensics can help in investigating a security breach. Students will also obtain a good understanding of how information is stored in a database and what services are available to protect it. Students who complete the Graduate Certificate in Information Security will be able to demonstrate: Advanced knowledge of information security concepts, governance, biometric systems, and database systems security, as well as network security and cryptography. Proficiency in risk management, such as asset assessments, architectural solutions, modeling, and design. Competence in security policies, processes, technology, and operations.',
				'status'=>'lrn_lpn_sts_itv',
				'object_type'=>'cre_sys_obt_trn',
				'lang_code'=>'cre_sys_lng_eng',
				'end_date'=>$NewDate),
								'output'=>array('id'),
							//	'result'=>array('<results totalrecords =','<result>','<id>'),
								'output_type'=>'xml'
						),
				
		
			
				),
				'validate' => array(
 				array('input'=>array('userid'=>1,'title'=>$title,'code'=>$program_code,'value'=>'Sample Description','status'=>'lrn_lpn_sts_itv','object_type'=>'cre_sys_obt_cur','lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('The Program code is not unique'),
						  'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'title'=>'TATPP','code'=>'one','value'=>'Sample Description','status'=>'aaa','object_type'=>'cre_sys_obt_cur','lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('Please Enter the correct Status'),
						  'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'title'=>'TATPP','code'=>'two','value'=>'Sample Description','status'=>'lrn_lpn_sts_itv','object_type'=>'aaa','lang_code'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('object Type incorrect.'),
						  'output_type'=>'xml'
							),
					array('input'=>array('userid'=>1,'title'=>'TATPP','code'=>'three','value'=>'Sample Description','status'=>'lrn_lpn_sts_itv','object_type'=>'cre_sys_obt_trn','lang_code'=>'cre_sys_lng_eng','end_date'=>'aa'),
						  'output'=>array(),
						  'result'=>array('Complete By Invalid date.'),
						  'output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'title'=>'TATPP','code'=>'four','value'=>'Sample Description','status'=>'lrn_lpn_sts_itv','object_type'=>'cre_sys_obt_cur','lang_code'=>'cre_sys_lng_eng','price'=>$price,'currency_type'=>'aa'),
								'output'=>array(),
								'result'=>array('The given Currency type is invalid or inactive status'),
								'output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'title'=>'TATPP','code'=>'five','value'=>'Sample Description','status'=>'lrn_lpn_sts_itv','object_type'=>'cre_sys_obt_cur','lang_code'=>'cre_sys_lng_eng','price'=>$price),
								'output'=>array(),
								'result'=>array('Currency type should not empty'),
								'output_type'=>'xml'
						),
									
				)
			), 
			
		); 
?>