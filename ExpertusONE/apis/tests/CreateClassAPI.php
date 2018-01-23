<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$coursedet = db_query('select id,code from slt_course_template order by id desc limit 1')->fetchAll();
$location =  db_query('select id from slt_location order by id desc limit 1')->fetchField();
$inactiveloc =  db_query('select id from slt_location where status = "lrn_res_loc_del" OR status = "lrn_res_loc_itv"   order by id desc limit 1')->fetchField();
expDebug::dPrint('$totalEnrollment : '.$location);
$activeloc =  db_query('select id from slt_location where is_active = 1 order by id desc limit 1')->fetchField();
$random = getRandomWord();

$coursedet1= db_query("select id from slt_course_template where code = 'CRSD-LDRBP-I'")->fetchField();
$coursedet2= db_query("select id from slt_course_template where code = 'CRSD-LDRBP-II'")->fetchField();
$coursedet3= db_query("select id from slt_course_template where code = 'CRSD-ELDBW'")->fetchField();
$coursedet4= db_query("select id from slt_course_template where code = 'CRSD-SMSFL'")->fetchField();
$coursedet5= db_query("select id from slt_course_template where code = 'CRSD-RLMKS-INTRO'")->fetchField();
$coursedet6= db_query("select id from slt_course_template where code = 'CRSD-RLMKS-ADV'")->fetchField();
$coursedet7= db_query("select id from slt_course_template where code = 'CRSD-BMS'")->fetchField();
$coursedet8= db_query("select id from slt_course_template where code = 'CRSD-SMKPL'")->fetchField();
$coursedet9= db_query("select id from slt_course_template where code = 'CRSD-RLMKP'")->fetchField();
$coursedet10= db_query("select id from slt_course_template where code = 'CRSD-SSMGR'")->fetchField();
$coursedet11= db_query("select id from slt_course_template where code = 'CRSD-VEXPS'")->fetchField();
$coursedet12= db_query("select id from slt_course_template where code = 'CRSD-LGLCM'")->fetchField();
$coursedet13= db_query("select id from slt_course_template where code = 'CRSD-FKDSE'")->fetchField();
$coursedet14= db_query("select id from slt_course_template where code = 'CRSD-CCS-001'")->fetchField();
$coursedet15= db_query("select id from slt_course_template where code = 'CRSD-CCW-001'")->fetchField();
$coursedet16= db_query("select id from slt_course_template where code = 'CRSD-CCV-001'")->fetchField();
$coursedet17= db_query("select id from slt_course_template where code = 'CRSD-CCILT-001'")->fetchField();
$coursedet18= db_query("select id from slt_course_template where code = 'CRSD-CCTP-001'")->fetchField();
$coursedet19= db_query("select id from slt_course_template where code = 'CRSD-AGO'")->fetchField();
$coursedet20= db_query("select id from slt_course_template where code = 'CRSD-EONE-MOBILE'")->fetchField();
$coursedet21= db_query("select id from slt_course_template where code = 'CRSD-EONEMT-MOBILE'")->fetchField();
$coursedet22= db_query("select id from slt_course_template where code = 'BSH-CRSE'")->fetchField();
$coursedet23= db_query("select id from slt_course_template where code = 'HI-CRSE'")->fetchField();
$coursedet24= db_query("select id from slt_course_template where code = 'AHI-CRSE'")->fetchField();
$coursedet25= db_query("select id from slt_course_template where code = 'BAF-CRSE'")->fetchField();
$coursedet26= db_query("select id from slt_course_template where code = 'WAB-CRSE'")->fetchField();
$coursedet27= db_query("select id from slt_course_template where code = 'DBS-CRSE'")->fetchField();
$coursedet28= db_query("select id from slt_course_template where code = 'ITSPP-CRSE'")->fetchField();
$coursedet29= db_query("select id from slt_course_template where code = 'EIS-CRSE'")->fetchField();
$coursedet30= db_query("select id from slt_course_template where code = 'ERA-CRSE'")->fetchField();

$location1 = db_query('select id from slt_location where name ="Cleveland Ohio" and  is_active = 1 order by id desc limit 1')->fetchField();
$location2 = db_query('select id from slt_location where name ="The Park" and  is_active = 1 order by id desc limit 1')->fetchField();
$location3 = db_query('select id from slt_location where name ="Golden Tulip Hotel" and  is_active = 1 order by id desc limit 1')->fetchField();
$location4 = db_query('select id from slt_location where name ="Waterloo Conference Room" and  is_active = 1 order by id desc limit 1')->fetchField();
$location5 = db_query('select id from slt_location where name ="Sheraton Imperial Hotel" and  is_active = 1 order by id desc limit 1')->fetchField();
$testCases = array(

			
			'CreateClassAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet1,
								'title'=>'Leadership best practice - Module 1',
								'code'=>'CLSD-LDRBP-I-VC-001',
								'value'=>'This training helps in:
	                                           * Carrying out a leadership inventory.
	                                           * Stimulating leadership in your company.
	                                           * Identifying leadership counter-indications in order to deal with them.
	                                           Target audiences:
	                                           * Top managers, Board members, Managers of managers,  Directors of Business, Managers of large-scale projects.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>15,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet2,
								'title'=>'Leadership best practice - Module 2',
								'code'=>'CLSD-LDRBP-II-ILT-001',
								'value'=>'This training helps in:
											   * Leadership inventory Management.
											   * Stimulating leadership.
											   * Business Decisions.
											   Target audiences:
											   * Top managers, Board members, Managers of managers,  Directors of Business, Managers of large-scale projects.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location1,
								'currency_type'  => 'cre_sys_crn_usd',
								'price' => 750,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet3,
								'title'=>'Establishing leadership in the best way_Scorm 2004',
								'code'=>'CLSD-ELDBW-WBT-001',
								'value'=>'This training helps in:
														-Defining the rules of the game in your leadership zone.
														-Avoiding mistakes in the leadership zone.
											            The objectives of the E-Learning program:
														To transform your executive board into a high-performance management team.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet3,
								'title'=>'Establishing leadership in the best way',
								'code'=>'CLSD-ELDBW-ILT-001',
								'value'=>'This training helps in:
														-Defining the rules of the game in your leadership zone.
														-Avoiding mistakes in the leadership zone.
											            The objectives of the E-Learning program:
														To transform your executive board into a high-performance management team.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location1,
								'currency_type'  => 'cre_sys_crn_usd',
								'price' => 650,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet4,
								'title'=>'Securing managerial support for leadership_Scorm 1.2',
								'code'=>'CLSD-SMSFL-WBT-001',
								'value'=>'This training helps in:
														Clarifying the roles of managers in terms of developing leadership.
														Involving managers by including them.
														Coaching managers so that they succeed in helping their staff to succeed.
														The objectives of the E-Learning program:
														To ensure that managers of managers and operational managers adopt leadership attitudes: to encourage them to bring out leadership potential in those around and alongside them.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet4,
								'title'=>'Securing managerial support for leadership',
								'code'=>'CLSD-SMSFL-VC-001',
								'value'=>'This training helps in:
														Clarifying the roles of managers in terms of developing leadership.
														Involving managers by including them.
														Coaching managers so that they succeed in helping their staff to succeed.
														The objectives of the E-Learning program:
														To ensure that managers of managers and operational managers adopt leadership attitudes: to encourage them to bring out leadership potential in those around and alongside them.',
								'status'=>'lrn_cls_sts_itv',
								'max_seats'=>50,
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet5,
								'title'=>'Relationship marketing strategy - Introduction',
								'code'=>'CLSD-RLMKS-INTRO-ILT-001',
								'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location2,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet5,
								'title'=>'Relationship marketing strategy - Intermediate_Scorm 1.2',
								'code'=>'CRSD-RLMKS-INTER-WBT-001',
								'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet6,
								'title'=>'Relationship marketing strategy - Advanced',
								'code'=>'CRSD-RLMKS-ADV-VOD-001',
								'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet7,
								'title'=>'Business Management Skills',
								'code'=>'CRSD-BMS-VOD',
								'value'=>'To run a successful business you need a diverse range of business management skills. When you start your business it’s likely that your responsibilities will include:
														sales and marketing;
														accounts;
														human resources; and
														information technology (IT).
														How confident do you feel in your ability to manage them?
														It’s a good idea to plan ahead of time how you’re going to manage each area which may include delegating various functions to a business partner, undertaking additional training or contracting a specialist advisor such as a bookkeeper, graphic designer or merchandiser.
														Remember that although you need to understand, manage and take responsibility for every aspect of your business, you don’t have to do everything yourself. Some of the key areas you’ll need to think about are outlined below.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet8,
								'title'=>'Discover the hidden world of your buyer',
								'code'=>'CLSD-SMKPL-VC-001',
								'value'=>'This training helps in:
														Professional and personal motivations of buyers.
														Asking the right questions.
														Building a closer relationship with buyers.
														Active listening to understand buyers better.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'currency_type'  => 'cre_sys_crn_usd',
								'price' => 800,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet8,
								'title'=>'Discover the hidden world of your buyer',
								'code'=>'CLSD-SMKPL-VOD-001',
								'value'=>'This training helps in:
														Professional and personal motivations of buyers.
														Asking the right questions.
														Building a closer relationship with buyers.
														Active listening to understand buyers better.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet9,
								'title'=>'Relationship sales in practice',
								'code'=>'CLSD-RLMKP-ILT-001',
								'value'=>'Relationship selling is all about building a friendship or relationship with your prospects and listening to their needs. Once you\'ve built that relationship, shown you care, and earned their trust, you are on the road to making them a customer. Knowing their needs and finding out their secret fears.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location3,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet10,
								'title'=>'The situational skills of the manager',
								'code'=>'CRSD-SSMGR-VC-001',
								'value'=>'Basic concepts in systemic analysis.
														Analysing a situation or conflict using a systemic approach.
														Handling all managerial situations effectively.
														The objectives of the E-Learning program
														To manage effectively using a systemic approach.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet11,
								'title'=>'Valuing your experienced staff - Post Assessment_KC (pdf)',
								'code'=>'CRSD-VEXPS-WBT-001',
								'value'=>'Managing the jobs and skills within your company effectively by anticipating demographic changes.
														Being able to make the most of the experience of experienced staff.
														Introducing measures designed to secure the loyalty of experienced staff.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'currency_type'  => 'cre_sys_crn_usd',
								'price' => 550,
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet12,
								'title'=>'Legal compliance module',
								'code'=>'CRSD-LGLCM-VOD-001',
								'value'=>'Legal compliance is the umbrella term covering an organization\'s approach across many areas. Being closely related concerns, governance, risk and compliance activities are increasingly being integrated and aligned to some extent in order to avoid conflicts, wasteful overlaps and gaps',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet13,
								'title'=>'The four keys to developing self-esteem',
								'code'=>'CRSD-FKDSE-ILT-001',
								'value'=>'Recognising and reinforcing your sense of importance and uniqueness.
														Developing your relationship boundaries and reinforcing your feeling of inner security.
														Discovering your own fundamental identity.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location3,
								'currency_type'  => 'cre_sys_crn_usd',
								'price' => 600,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet14,
								'title'=>'ExpertusONE Course/Class Structure',
								'code'=>'CRSD-CCS-001',
								'value'=>'ExpertusONE Course/Class Structure',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet15,
								'title'=>'Create a Course - Web Based Training (WBT)',
								'code'=>'CRSD-CCW-001',
								'value'=>'Create a Course - Web Based Training (WBT)',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet16,
								'title'=>'Create a Course - Virtual Class (VC)',
								'code'=>'CRSD-CCV-001',
								'value'=>'Create a Course - Virtual Class (VC)',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet17,
								'title'=>'Create a Course - Instructor Led Training',
								'code'=>'CRSD-ILT',
								'value'=>'Create a Course - Instructor Led Training',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet18,
								'title'=>'How to create Training ',
								'code'=>'CRSD-CCR-001',
								'value'=>'How to create Training Plan',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet19,
								'title'=>'Achieving Goals and Objectives_Scorm 1.2',
								'code'=>'CRSD-AGO-WBT',
								'value'=>'Having work that connects to larger goals is very meaningful. When we are clear on why our work matters and how it contributes to the company we have a greater sense of purpose and an increase in personal motivation. While sometimes it may be hard to see, everyone in the company has a purpose. You were hired for a reason—to help the company achieves its goals.
													  What you need to do is to clearly understand this connection and find out what you’re responsible to do to help the business achieve success. This is an important thing to do. Once again, when you are clear on what you must do to contribute to business results, your work takes on greater meaning. You feel more valued by the organization and you develop new levels of motivation to perform at your best, and this of course is a great thing for both you and the company.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet20,
								'title'=>'ExpertusONE Mobile Presence Sensing video (Mobile Compatible Content) - (Use mobile presence sensing video for this)',
								'code'=>'CRSD-EONE-MOBILE-VOD',
								'value'=>'ExpertusONE Mobile Presence Sensing video with mobile compatible content',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet21,
								'title'=>'ExpertusONE Meetings (Mobile Compatible - Knowledge Content)_KC (pdf)',
								'code'=>'CRSD-EONEMT-MOBILE-WBT',
								'value'=>'As the first LMS with built-in virtual conferencing, ExpertusONE provides the easiest to use online training environment with exceptional cost-savings.
														Now you can provide learners with on-demand video conferencing,
														whiteboarding, chat and more… right from your LMS… without having to buy and integrate a third-party tool!',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet22,
								'title'=>'Biomedical Sciences and Health',
								'code'=>'BSH-CRSE-VOD',
								'value'=>'This course is designed for IT professionals, and those training to be IT professionals, who are preparing for careers in healthcare-related IT (Health Informatics). This course provides a high-level introduction into basic concepts of biomedicine and familiarizes students with the structure and organization of American healthcare system and the roles played by IT in that system. The course introduces medical terminology, human anatomy and physiology, disease processes, diagnostic modalities, and treatments associated with common disease processes. IT case studies demonstrate the key roles of health informatics and how IT tools and resources help medical professionals integrate multiple sources of information to make diagnostic and therapeutic decisions.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet22,
								'title'=>'Biomedical Sciences and Health_Scorm 2004',
								'code'=>'BSH-CRSE-WBT',
								'value'=>'This course is designed for IT professionals, and those training to be IT professionals, who are preparing for careers in healthcare-related IT (Health Informatics). This course provides a high-level introduction into basic concepts of biomedicine and familiarizes students with the structure and organization of American healthcare system and the roles played by IT in that system. The course introduces medical terminology, human anatomy and physiology, disease processes, diagnostic modalities, and treatments associated with common disease processes. IT case studies demonstrate the key roles of health informatics and how IT tools and resources help medical professionals integrate multiple sources of information to make diagnostic and therapeutic decisions.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet23,
								'title'=>'Health Informatics_Scorm 2004',
								'code'=>'HI-CLS-WBT',
								'value'=>'This course presents the technological fundamentals and integrated clinical applications of modern Biomedical IT. The first part of the course covers the technological fundamentals and the scientific concepts behind modern medical technologies, such as digital radiography, CT, nuclear medicine, ultrasound imaging, etc. It also presents various medical data and patient records, and focuses on various techniques for processing medical images. This part also covers medical computer networks and systems and data security and protection. The second part of the course focuses on actual medical applications that are used in health care and biomedical research',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet23,
								'title'=>'Health Informatics',
								'code'=>'HI-CLS-VC',
								'value'=>'This course presents the technological fundamentals and integrated clinical applications of modern Biomedical IT. The first part of the course covers the technological fundamentals and the scientific concepts behind modern medical technologies, such as digital radiography, CT, nuclear medicine, ultrasound imaging, etc. It also presents various medical data and patient records, and focuses on various techniques for processing medical images. This part also covers medical computer networks and systems and data security and protection. The second part of the course focuses on actual medical applications that are used in health care and biomedical research',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet24,
								'title'=>'Advanced Health Information',
								'code'=>'AHI-CRSE-VOD',
								'value'=>'This course presents the details of information processing in hospitals, hospital information systems (HIS), and more broadly health information systems. It presents the architecture, design, and user requirements of information systems in health care environment. It focuses on Information Technology aspects of Health Informatics specifically addressing the design, development, operation, and management of HIS. The first part of this course covers the introductory concepts including information processing needs, and information management in health care environment. The second part covers detailed description of HIS including hospital process modeling, architecture, quality assessment, and applicable tools. The final part of the course covers management of HIS and related issues and extension of this topic to other health care organizations. The course will have a term project providing students a hands-on experience in design and research of HIS.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet24,
								'title'=>'Advanced Health Information_Scorm 2004',
								'code'=>'AHI-CRSE-WBT',
								'value'=>'This course presents the details of information processing in hospitals, hospital information systems (HIS), and more broadly health information systems. It presents the architecture, design, and user requirements of information systems in health care environment. It focuses on Information Technology aspects of Health Informatics specifically addressing the design, development, operation, and management of HIS. The first part of this course covers the introductory concepts including information processing needs, and information management in health care environment. The second part covers detailed description of HIS including hospital process modeling, architecture, quality assessment, and applicable tools. The final part of the course covers management of HIS and related issues and extension of this topic to other health care organizations. The course will have a term project providing students a hands-on experience in design and research of HIS.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet24,
								'title'=>'Advanced Health Information',
								'code'=>'AHI-CRSE-ILT-001',
								'value'=>'This course presents the details of information processing in hospitals, hospital information systems (HIS), and more broadly health information systems. It presents the architecture, design, and user requirements of information systems in health care environment. It focuses on Information Technology aspects of Health Informatics specifically addressing the design, development, operation, and management of HIS. The first part of this course covers the introductory concepts including information processing needs, and information management in health care environment. The second part covers detailed description of HIS including hospital process modeling, architecture, quality assessment, and applicable tools. The final part of the course covers management of HIS and related issues and extension of this topic to other health care organizations. The course will have a term project providing students a hands-on experience in design and research of HIS.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location4,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet24,
								'title'=>'Advanced Health Information',
								'code'=>'AHI-CRSE-ILT1',
								'value'=>'This course presents the details of information processing in hospitals, hospital information systems (HIS), and more broadly health information systems. It presents the architecture, design, and user requirements of information systems in health care environment. It focuses on Information Technology aspects of Health Informatics specifically addressing the design, development, operation, and management of HIS. The first part of this course covers the introductory concepts including information processing needs, and information management in health care environment. The second part covers detailed description of HIS including hospital process modeling, architecture, quality assessment, and applicable tools. The final part of the course covers management of HIS and related issues and extension of this topic to other health care organizations. The course will have a term project providing students a hands-on experience in design and research of HIS.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location5,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet25,
								'title'=>'Business Analytical Foundation (10 Units)',
								'code'=>'BAF-CRSE-VOD',
								'value'=>'This course presents fundamental knowledge and skills for applying business analytics to managerial decision-making in corporate environments. Topics include descriptive analytics (techniques for categorizing, characterizing, consolidation, and classifying data for conversion into useful information for the purposes of understanding and analyzing business performance), predictive analytics (techniques for detection of hidden patterns in large quantities of data to segment and group data into coherent sets in order to predict behavior and trends), prescriptive analytics (techniques for identification of best alternatives for maximizing or minimizing business objectives). Students will learn how to use data effectively to drive rapid, precise, and profitable analytics-based decisions. The framework of using interlinked data-inputs, analytics models, and decision-support tools will be applied within a proprietary business analytics shell and demonstrated with examples from different functional areas of the enterprise.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet25,
								'title'=>'Business Analytical Foundation (10 Units)_KC (pdf)',
								'code'=>'BAF-CRSE-WBT',
								'value'=>'This course presents fundamental knowledge and skills for applying business analytics to managerial decision-making in corporate environments. Topics include descriptive analytics (techniques for categorizing, characterizing, consolidation, and classifying data for conversion into useful information for the purposes of understanding and analyzing business performance), predictive analytics (techniques for detection of hidden patterns in large quantities of data to segment and group data into coherent sets in order to predict behavior and trends), prescriptive analytics (techniques for identification of best alternatives for maximizing or minimizing business objectives). Students will learn how to use data effectively to drive rapid, precise, and profitable analytics-based decisions. The framework of using interlinked data-inputs, analytics models, and decision-support tools will be applied within a proprietary business analytics shell and demonstrated with examples from different functional areas of the enterprise.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet30,
								'title'=>'Enterprise Risk Analytics (10 Units)_Scorm 1.2',
								'code'=>'ERA-CRSE-WBT',
								'value'=>'This course presents fundamental knowledge and skills for applying business analytics to managerial decision-making in corporate environments. Topics include descriptive analytics (techniques for categorizing, characterizing, consolidation, and classifying data for conversion into useful information for the purposes of understanding and analyzing business performance), predictive analytics (techniques for detection of hidden patterns in large quantities of data to segment and group data into coherent sets in order to predict behavior and trends), prescriptive analytics (techniques for identification of best alternatives for maximizing or minimizing business objectives). Students will learn how to use data effectively to drive rapid, precise, and profitable analytics-based decisions. The framework of using interlinked data-inputs, analytics models, and decision-support tools will be applied within a proprietary business analytics shell and demonstrated with examples from different functional areas of the enterprise.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet30,
								'title'=>'Enterprise Risk Analytics (10 Units)',
								'code'=>'ERA-CRSE-ILT',
								'value'=>'This course presents fundamental knowledge and skills for applying business analytics to managerial decision-making in corporate environments. Topics include descriptive analytics (techniques for categorizing, characterizing, consolidation, and classifying data for conversion into useful information for the purposes of understanding and analyzing business performance), predictive analytics (techniques for detection of hidden patterns in large quantities of data to segment and group data into coherent sets in order to predict behavior and trends), prescriptive analytics (techniques for identification of best alternatives for maximizing or minimizing business objectives). Students will learn how to use data effectively to drive rapid, precise, and profitable analytics-based decisions. The framework of using interlinked data-inputs, analytics models, and decision-support tools will be applied within a proprietary business analytics shell and demonstrated with examples from different functional areas of the enterprise.',
								'status'=>'lrn_cls_sts_itv',
								'max_seats'=>50,
								'class_location' => $location5,
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet26,
								'title'=>'Web Analytics for Business (5 Units)',
								'code'=>'WAB-CRSE-VOD',
								'value'=>'Explore web analytics, text mining, web mining, and practical application domains. The web analytics part of the course studies the metrics of websites, their content, user behavior, and reporting. The Google analytics tool is used for collection of website data and doing the analysis. The text mining module covers the analysis of text including content extraction, string matching, clustering, classification, and recommendation systems. The web mining module presents how web crawlers process and index the content of web sites, how search works, and how results are ranked. Application areas mining the social web and game metrics will be extensively investigated.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet26,
								'title'=>'Web Analytics for Business (5 Units)',
								'code'=>'WAB-CRSE-VC',
								'value'=>'Explore web analytics, text mining, web mining, and practical application domains. The web analytics part of the course studies the metrics of websites, their content, user behavior, and reporting. The Google analytics tool is used for collection of website data and doing the analysis. The text mining module covers the analysis of text including content extraction, string matching, clustering, classification, and recommendation systems. The web mining module presents how web crawlers process and index the content of web sites, how search works, and how results are ranked. Application areas mining the social web and game metrics will be extensively investigated.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet26,
								'title'=>'Web Analytics for Business (5 Units)',
								'code'=>'WAB-CRSE-VOD_1',
								'value'=>'Explore web analytics, text mining, web mining, and practical application domains. The web analytics part of the course studies the metrics of websites, their content, user behavior, and reporting. The Google analytics tool is used for collection of website data and doing the analysis. The text mining module covers the analysis of text including content extraction, string matching, clustering, classification, and recommendation systems. The web mining module presents how web crawlers process and index the content of web sites, how search works, and how results are ranked. Application areas mining the social web and game metrics will be extensively investigated.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet27,
								'title'=>'Database Security',
								'code'=>'DBS-CLS-VC',
								'value'=>'The course provides a strong foundation in database security and auditing. This course utilizes Oracle scenarios and step-by-step examples. The following topics are covered: security, profiles, password policies, privileges and roles, Virtual Private Databases, and auditing. The course also covers advanced topics such as SQL injection, database management security issues such as securing the DBMS, enforcing access controls, and related issues.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet27,
								'title'=>'Database Security_Scorm 2004',
								'code'=>'DBS-CLS-WBT',
								'value'=>'The course provides a strong foundation in database security and auditing. This course utilizes Oracle scenarios and step-by-step examples. The following topics are covered: security, profiles, password policies, privileges and roles, Virtual Private Databases, and auditing. The course also covers advanced topics such as SQL injection, database management security issues such as securing the DBMS, enforcing access controls, and related issues.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet28,
								'title'=>'IT Security Policies and Procedures',
								'code'=>'ITSPP-CLS-VOD',
								'value'=>'This course enables IT professional leaders to identify emerging security risks and implement highly secure networks to support organizational goals. Discussion of methodologies for identifying, quantifying, mitigating and controlling risks. Students implement a comprehensive IT risk management plans (RMP) that identify alternate sites for processing mission-critical applications, and techniques to recover infrastructure, systems, networks, data and user access. The course also discusses related topics such as: disaster recovery, handling information security; protection of property, personnel and facilities; protection of sensitive and classified information, privacy issues, and criminal terrorist and hostile activities.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet28,
								'title'=>'IT Security Policies and Procedures',
								'code'=>'ITSPP-CRSE-ILT',
								'value'=>'This course enables IT professional leaders to identify emerging security risks and implement highly secure networks to support organizational goals. Discussion of methodologies for identifying, quantifying, mitigating and controlling risks. Students implement a comprehensive IT risk management plans (RMP) that identify alternate sites for processing mission-critical applications, and techniques to recover infrastructure, systems, networks, data and user access. The course also discusses related topics such as: disaster recovery, handling information security; protection of property, personnel and facilities; protection of sensitive and classified information, privacy issues, and criminal terrorist and hostile activities.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location1,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet29,
								'title'=>'Enterprise Information Security',
								'code'=>'EIS-CRSE-VOD',
								'value'=>'The course provides an in-depth presentation of security issues in computer systems, networks, and applications. Formal security models are presented and illustrated on operating system security aspects, more specifically memory protection, access control and authentication, file system security, backup and recovery management, intrusion and virus protection mechanisms. Application level security focuses on language level security and various security policies; conventional and public keys encryption, authentication, message digest and digital signatures. Internet and intranet topics include security in IP, routers, proxy servers, and firewalls, application- level gateways, Web servers, file and mail servers. Discussion of remote access issues, such as dial-up servers, modems, VPN gateways and clients.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_vod'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet29,
								'title'=>'Enterprise Information Security',
								'code'=>'EIS-CRSE-ILT',
								'value'=>'The course provides an in-depth presentation of security issues in computer systems, networks, and applications. Formal security models are presented and illustrated on operating system security aspects, more specifically memory protection, access control and authentication, file system security, backup and recovery management, intrusion and virus protection mechanisms. Application level security focuses on language level security and various security policies; conventional and public keys encryption, authentication, message digest and digital signatures. Internet and intranet topics include security in IP, routers, proxy servers, and firewalls, application- level gateways, Web servers, file and mail servers. Discussion of remote access issues, such as dial-up servers, modems, VPN gateways and clients.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'max_seats'=>50,
								'class_location' => $location2,
								'delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,
								'course_id'=>$coursedet29,
								'title'=>'Enterprise Information Security',
								'code'=>'EIS-CRSE-WBT',
								'value'=>'The course provides an in-depth presentation of security issues in computer systems, networks, and applications. Formal security models are presented and illustrated on operating system security aspects, more specifically memory protection, access control and authentication, file system security, backup and recovery management, intrusion and virus protection mechanisms. Application level security focuses on language level security and various security policies; conventional and public keys encryption, authentication, message digest and digital signatures. Internet and intranet topics include security in IP, routers, proxy servers, and firewalls, application- level gateways, Web servers, file and mail servers. Discussion of remote access issues, such as dial-up servers, modems, VPN gateways and clients.',
								'status'=>'lrn_cls_sts_itv',
								'lang_code'=>'cre_sys_lng_eng',
								'delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
				),
				'validate' => array(
						
						array('input'=>array('userid'=>1,'course_id'=>'','title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>'','code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>'','code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>'','value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed:'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>''),
								'output'=>array('id'),
								'result'=>array('<short_msg>Mandatory Fields are missing in the feed. List provided below:'),
								'type'=>'DataSource','output_type'=>'xml'
						),
					array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'web-based'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid/Inactive Delivery Type.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
							),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_atv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Invalid class status.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','currency_type'=>'dollars'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Currency Type is not Valid</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','reg_end_date'=>'09-09-2016'),
								'output'=>array('id'),
								'result'=>array('<short_msg>Incorrect date format. It should be in YYYY-MM-DD</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt'),
								'output'=>array('id'),
								'result'=>array('<short_msg>max_seats is required for ILT and VC class delivery types.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_vcl'),
								'output'=>array('id'),
								'result'=>array('<short_msg>max_seats is required for ILT and VC class delivery types.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','min_seats'=>20,'max_seats'=>10),
								'output'=>array('id'),
								'result'=>array('<short_msg>Maximum capacity must be greater than minimum capacity.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','min_seats'=>20,'max_seats'=>100),
								'output'=>array('id'),
								'result'=>array('<short_msg>class_location is required for ilt class delivery type.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','min_seats'=>20,'max_seats'=>100,'class_location'=>'chennai'),
								'output'=>array('id'),
								'result'=>array('<short_msg>The class location id must be a valid numeric.</short_msg>'),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','min_seats'=>20,'max_seats'=>100,'class_location'=>$location+1),
								'output'=>array('id'),
								'result'=>array("<short_msg>The given location id doesn't resolve to a location.</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','price'=>10),
								'output'=>array('id'),
								'result'=>array("<short_msg>Currency type should not empty</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','price'=>10,'currency_type'=>'asasaa'),
								'output'=>array('id'),
								'result'=>array("<short_msg>Currency Type is not Valid</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$coursedet[0]->code,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','price'=>10,'currency_type'=>'cre_sys_crn_usd'),
								'output'=>array('id'),
								'result'=>array("<short_msg>Code already exists.</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>($coursedet[0]->id)+1,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_wbt','price'=>100,'currency_type'=>'cre_sys_crn_usd'),
								'output'=>array('id'),
								'result'=>array("<short_msg>Course Id does not exists.</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						),
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>getRandomWord(),'code'=>getRandomWord(),'value'=>getRandomWord(),'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','class_location'=>$activeloc,'max_seats'=>30),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						) ,
						array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>getRandomWord(),'code'=>getRandomWord(),'value'=>getRandomWord(),'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_vcl','max_seats'=>30),
								'output'=>array('id'),
								'result'=>array('<results totalrecords =','<id>'),
								'type'=>'DataSource','output_type'=>'xml'
						) ,
				)
			),  
			
		); 

if(!empty($inactiveloc)){
	$testCases['CreateClassAPI']['validate'][] = array('input'=>array('userid'=>1,'course_id'=>$coursedet[0]->id,'title'=>$random,'code'=>$random,'value'=>$random,'status'=>'lrn_cls_sts_itv','lang_code'=>'cre_sys_lng_eng','delivery_type'=>'lrn_cls_dty_ilt','min_seats'=>20,'max_seats'=>100,'class_location'=>$inactiveloc),
								'output'=>array('id'),
								'result'=>array("<short_msg>The given location id doesn't resolve to a location.</short_msg>"),
								'type'=>'DataSource','output_type'=>'xml'
						);

}

?>