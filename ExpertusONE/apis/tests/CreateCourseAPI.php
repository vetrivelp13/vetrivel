<?php 
function getRandomWord($len = 10) {
	$word = array_merge(range('a', 'z'), range('A', 'Z'));
	shuffle($word);
	return substr(implode($word), 0, $len);
}
$random = getRandomWord();
$testCases = array(

			// CreateCourseAPI test case Starts
			'CreateCourseAPI' => array(
				'datasource' => array(
					array('input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array('id'),
						  'result'=>array('<results totalrecords =','<result>','<id>'),
						  'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Leadership best practice - Module 1',
											'crs_code'=>'CRSD-LDRBP-I',
											'value'=>'This training helps in:
														* Carrying out a leadership inventory.
														* Stimulating leadership in your company.
														* Identifying leadership counter-indications in order to deal with them.
														Target audiences:
														* Top managers, Board members, Managers of managers,  Directors of Business, Managers of large-scale projects.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Leadership best practice - Module 2',
											'crs_code'=>'CRSD-LDRBP-II',
											'value'=>'This training helps in:
														* Leadership inventory Management.
														* Stimulating leadership.
														* Business Decisions.
														Target audiences:
														* Top managers, Board members, Managers of managers,  Directors of Business, Managers of large-scale projects.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Establishing leadership in the best way',
											'crs_code'=>'CRSD-ELDBW',
											'value'=>'This training helps in:
														-Defining the rules of the game in your leadership zone.
														-Avoiding mistakes in the leadership zone.
											            The objectives of the E-Learning program:
														To transform your executive board into a high-performance management team.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Securing managerial support for leadership',
											'crs_code'=>'CRSD-SMSFL',
											'value'=>'This training helps in:
														Clarifying the roles of managers in terms of developing leadership.
														Involving managers by including them.
														Coaching managers so that they succeed in helping their staff to succeed.
														The objectives of the E-Learning program:
														To ensure that managers of managers and operational managers adopt leadership attitudes: to encourage them to bring out leadership potential in those around and alongside them.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Relationship marketing strategy - Introduction',
											'crs_code'=>'CRSD-RLMKS-INTRO',
											'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Relationship marketing strategy - Intermediate',
											'crs_code'=>'CRSD-RLMKS-INTER',
											'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Relationship marketing strategy - Advanced',
											'crs_code'=>'CRSD-RLMKS-ADV',
											'value'=>'Program:
														Focusing on an effective customer loyalty programme.
														Building and measuring a customer relations programme.
														Implementing a multi-channel marketing plan.
														Designing a winning multi-channel strategy.
														The objectives of the E-Learning program:
														To design a relationship marketing strategy that allows you to identify and secure the loyalty of strategic customers.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Discover the hidden world of your buyer',
											'crs_code'=>'CRSD-SMKPL',
											'value'=>'This training helps in:
														Professional and personal motivations of buyers.
														Asking the right questions.
														Building a closer relationship with buyers.
														Active listening to understand buyers better.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Relationship sales in practice',
											'crs_code'=>'CRSD-RLMKP',
											'value'=>'Relationship selling is all about building a friendship or relationship with your prospects and listening to their needs. Once you\'ve built that relationship, shown you care, and earned their trust, you are on the road to making them a customer. Knowing their needs and finding out their secret fears.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'The situational skills of the manager',
											'crs_code'=>'CRSD-SSMGR',
											'value'=>'Basic concepts in systemic analysis.
														Analysing a situation or conflict using a systemic approach.
														Handling all managerial situations effectively.
														The objectives of the E-Learning program
														To manage effectively using a systemic approach.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Valuing your experienced staff',
											'crs_code'=>'CRSD-VEXPS',
											'value'=>'Managing the jobs and skills within your company effectively by anticipating demographic changes.
														Being able to make the most of the experience of experienced staff.
														Introducing measures designed to secure the loyalty of experienced staff.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Legal compliance module',
											'crs_code'=>'CRSD-LGLCM',
											'value'=>'Legal compliance is the umbrella term covering an organization\'s approach across many areas. Being closely related concerns, governance, risk and compliance activities are increasingly being integrated and aligned to some extent in order to avoid conflicts, wasteful overlaps and gaps',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'The four keys to developing self-esteem',
											'crs_code'=>'CRSD-FKDSE',
											'value'=>'Recognising and reinforcing your sense of importance and uniqueness.
														Developing your relationship boundaries and reinforcing your feeling of inner security.
														Discovering your own fundamental identity.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Business Management Skills',
											'crs_code'=>'CRSD-BMS',
											'value'=>'To run a successful business you need a diverse range of business management skills. When you start your business it’s likely that your responsibilities will include:
														sales and marketing;
														accounts;
														human resources; and
														information technology (IT).
														How confident do you feel in your ability to manage them?
														It’s a good idea to plan ahead of time how you’re going to manage each area which may include delegating various functions to a business partner, undertaking additional training or contracting a specialist advisor such as a bookkeeper, graphic designer or merchandiser.
														Remember that although you need to understand, manage and take responsibility for every aspect of your business, you don’t have to do everything yourself. Some of the key areas you’ll need to think about are outlined below.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'ExpertusONE Course/Class Structure',
											'crs_code'=>'CRSD-CCS-001',
											'value'=>'ExpertusONE Course/Class Structure',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Create a Course - Web Based Training (WBT)',
											'crs_code'=>'CRSD-CCW-001',
											'value'=>'Create a Course - Web Based Training (WBT)',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Create a Course - Virtual Class (VC)',
											'crs_code'=>'CRSD-CCV-001',
											'value'=>'Create a Course - Virtual Class (VC)',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Create a Course - Instructor Led Training',
											'crs_code'=>'CRSD-CCILT-001',
											'value'=>'Create a Course - Instructor Led Training',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'How to create Training Plan',
											'crs_code'=>'CRSD-CCTP-001',
											'value'=>'How to create Training Plan',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Achieving Goals and Objectives',
											'crs_code'=>'CRSD-AGO',
											'value'=>'Having work that connects to larger goals is very meaningful. When we are clear on why our work matters and how it contributes to the company we have a greater sense of purpose and an increase in personal motivation. While sometimes it may be hard to see, everyone in the company has a purpose. You were hired for a reason—to help the company achieves its goals.
													  What you need to do is to clearly understand this connection and find out what you’re responsible to do to help the business achieve success. This is an important thing to do. Once again, when you are clear on what you must do to contribute to business results, your work takes on greater meaning. You feel more valued by the organization and you develop new levels of motivation to perform at your best, and this of course is a great thing for both you and the company.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'ExpertusONE Meetings (Mobile Compatible - Knowledge Content)',
											'crs_code'=>'CRSD-EONEMT-MOBILE',
											'value'=>'As the first LMS with built-in virtual conferencing, ExpertusONE provides the easiest to use online training environment with exceptional cost-savings.
														Now you can provide learners with on-demand video conferencing,
														whiteboarding, chat and more… right from your LMS… without having to buy and integrate a third-party tool!',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'ExpertusONE Mobile Presence Sensing video (Mobile Friendly)',
											'crs_code'=>'CRSD-EONE-MOBILE',
											'value'=>'ExpertusONE Mobile Presence Sensing video with mobile compatible content',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Biomedical Sciences and Health',
											'crs_code'=>'BSH-CRSE',
											'value'=>'This course is designed for IT professionals, and those training to be IT professionals, who are preparing for careers in healthcare-related IT (Health Informatics). This course provides a high-level introduction into basic concepts of biomedicine and familiarizes students with the structure and organization of American healthcare system and the roles played by IT in that system. The course introduces medical terminology, human anatomy and physiology, disease processes, diagnostic modalities, and treatments associated with common disease processes. IT case studies demonstrate the key roles of health informatics and how IT tools and resources help medical professionals integrate multiple sources of information to make diagnostic and therapeutic decisions.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Health Informatics',
											'crs_code'=>'HI-CRSE',
											'value'=>'This course presents the technological fundamentals and integrated clinical applications of modern Biomedical IT. The first part of the course covers the technological fundamentals and the scientific concepts behind modern medical technologies, such as digital radiography, CT, nuclear medicine, ultrasound imaging, etc. It also presents various medical data and patient records, and focuses on various techniques for processing medical images. This part also covers medical computer networks and systems and data security and protection. The second part of the course focuses on actual medical applications that are used in health care and biomedical research',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Advanced Health Information',
											'crs_code'=>'AHI-CRSE',
											'value'=>'This course presents the details of information processing in hospitals, hospital information systems (HIS), and more broadly health information systems. It presents the architecture, design, and user requirements of information systems in health care environment. It focuses on Information Technology aspects of Health Informatics specifically addressing the design, development, operation, and management of HIS. The first part of this course covers the introductory concepts including information processing needs, and information management in health care environment. The second part covers detailed description of HIS including hospital process modeling, architecture, quality assessment, and applicable tools. The final part of the course covers management of HIS and related issues and extension of this topic to other health care organizations. The course will have a term project providing students a hands-on experience in design and research of HIS.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Business Analytical Foundation (10 Units)',
											'crs_code'=>'BAF-CRSE',
											'value'=>'This course presents fundamental knowledge and skills for applying business analytics to managerial decision-making in corporate environments. Topics include descriptive analytics (techniques for categorizing, characterizing, consolidation, and classifying data for conversion into useful information for the purposes of understanding and analyzing business performance), predictive analytics (techniques for detection of hidden patterns in large quantities of data to segment and group data into coherent sets in order to predict behavior and trends), prescriptive analytics (techniques for identification of best alternatives for maximizing or minimizing business objectives). Students will learn how to use data effectively to drive rapid, precise, and profitable analytics-based decisions. The framework of using interlinked data-inputs, analytics models, and decision-support tools will be applied within a proprietary business analytics shell and demonstrated with examples from different functional areas of the enterprise.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Enterprise Risk Analytics (10 Units)',
											'crs_code'=>'ERA-CRSE',
											'value'=>'The course offers an overview of the key current and emerging enterprise risk analytical approaches used by corporations and governmental institutions and is focused on understanding and implementing the enterprise risk management framework on how to leverage the opportunities around a firm to increase firm value. The major risk categories of the enterprise risk management such as financial risk, strategic risk and operational risk will be discussed and risk analytics approaches for each of these risks will be covered. Students will learn how to use interlinked data-inputs, analytics models, business statistics, optimization techniques, simulation, and decision-support tools. An integrated enterprise risk analytics approach will be demonstrated with examples from different functional areas of the enterprise.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Web Analytics for Business (5 Units)',
											'crs_code'=>'WAB-CRSE',
											'value'=>'Explore web analytics, text mining, web mining, and practical application domains. The web analytics part of the course studies the metrics of websites, their content, user behavior, and reporting. The Google analytics tool is used for collection of website data and doing the analysis. The text mining module covers the analysis of text including content extraction, string matching, clustering, classification, and recommendation systems. The web mining module presents how web crawlers process and index the content of web sites, how search works, and how results are ranked. Application areas mining the social web and game metrics will be extensively investigated.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Database Security',
											'crs_code'=>'DBS-CRSE',
											'value'=>'The course provides a strong foundation in database security and auditing. This course utilizes Oracle scenarios and step-by-step examples. The following topics are covered: security, profiles, password policies, privileges and roles, Virtual Private Databases, and auditing. The course also covers advanced topics such as SQL injection, database management security issues such as securing the DBMS, enforcing access controls, and related issues.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'IT Security Policies and Procedures',
											'crs_code'=>'ITSPP-CRSE',
											'value'=>'This course enables IT professional leaders to identify emerging security risks and implement highly secure networks to support organizational goals. Discussion of methodologies for identifying, quantifying, mitigating and controlling risks. Students implement a comprehensive IT risk management plans (RMP) that identify alternate sites for processing mission-critical applications, and techniques to recover infrastructure, systems, networks, data and user access. The course also discusses related topics such as: disaster recovery, handling information security; protection of property, personnel and facilities; protection of sensitive and classified information, privacy issues, and criminal terrorist and hostile activities.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							),
					array(
							'input'=>array(	'userid'=>1,
											'crs_title'=>'Enterprise Information Security',
											'crs_code'=>'EIS-CRSE',
											'value'=>'The course provides an in-depth presentation of security issues in computer systems, networks, and applications. Formal security models are presented and illustrated on operating system security aspects, more specifically memory protection, access control and authentication, file system security, backup and recovery management, intrusion and virus protection mechanisms. Application level security focuses on language level security and various security policies; conventional and public keys encryption, authentication, message digest and digital signatures. Internet and intranet topics include security in IP, routers, proxy servers, and firewalls, application- level gateways, Web servers, file and mail servers. Discussion of remote access issues, such as dial-up servers, modems, VPN gateways and clients.',
											'crs_status'=>'lrn_crs_sts_atv',
											'crs_language'=>'cre_sys_lng_eng'),
							'output'=>array('id'),
							'result'=>array('<results totalrecords =','<result>','<id>'),
							'output_type'=>'xml'
							)
						
				),
				'validate' => array(
 					array('input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						  'output'=>array(),
						  'result'=>array('<error>','<error_code>','<short_msg>Code already exists.</short_msg>'),
						  'output_type'=>'xml'
							),
				    array(
				    	   'input'=>array('userid'=>1,'crs_title'=>'','crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						   'output'=>array(),
						   'result'=>array('<error>','<error_code>','crs_title'),
						   'output_type'=>'xml'
						    ),
					array(
						   'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>'','value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						   'output'=>array(),
						   'result'=>array('<error>','<error_code>','crs_code'),
						   'output_type'=>'xml'
						    ),
					array(
						   'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'','crs_status'=>'lrn_crs_sts_itv','crs_language'=>'cre_sys_lng_eng'),
						   'output'=>array(),
						   'result'=>array('<error>','<error_code>','value'),
						   'output_type'=>'xml'
						    ),
					array(
						   'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'','crs_language'=>'cre_sys_lng_eng'),
						   'output'=>array(),
						   'result'=>array('<error>','<error_code>','crs_status'),
					       'output_type'=>'xml'
						    ),
				    array(
				    		'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_itv','crs_language'=>''),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','crs_language'),
							'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','<short_msg>Code already exists.</short_msg>'),
							'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'crs_title'=>'','crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','crs_title'),
							'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>'','value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','crs_code'),
							'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'','crs_status'=>'lrn_crs_sts_atv','crs_language'=>'cre_sys_lng_eng'),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','value'),
							'output_type'=>'xml'
						    ),
				    array(
				    		'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'lrn_crs_sts_atv','crs_language'=>''),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','crs_language'),
							'output_type'=>'xml'
						    ),
					array(
							'input'=>array('userid'=>1,'crs_title'=>$random,'crs_code'=>$random,'value'=>'Sample Description','crs_status'=>'tyttt','crs_language'=>'cre_sys_lng_eng'),
							'output'=>array(),
						    'result'=>array('<error>','<error_code>','<short_msg>Invalid course status.</short_msg>'),
							'output_type'=>'xml'
						)
						
				)
			),  // CreateCourseAPI test case Ends
			
		); // End of api test case main array
?>