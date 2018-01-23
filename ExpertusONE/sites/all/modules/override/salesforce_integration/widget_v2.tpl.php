<?php
/**
 * @file
 * Zen theme's implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation. $language->dir
 *   contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $jump_link_target: The HTML ID of the element that the "Jump to Navigation"
 *   link should jump to. Defaults to "main-menu".
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It should be placed within the <body> tag. When selecting through CSS
 *   it's recommended that you use the body tag, e.g., "body.front". It can be
 *   manipulated through the variable $classes_array from preprocess functions.
 *   The default values can contain one or more of the following:
 *   - front: Page is the home page.
 *   - not-front: Page is not the home page.
 *   - logged-in: The current viewer is logged in.
 *   - not-logged-in: The current viewer is not logged in.
 *   - node-type-[node type]: When viewing a single node, the type of that node.
 *     For example, if the node is a Blog entry, this would be "node-type-blog".
 *     Note that the machine name of the content type will often be in a short
 *     form of the human readable label.
 *   The following only apply with the default sidebar_first and sidebar_second
 *   block regions:
 *     - two-sidebars: When both sidebars have content.
 *     - no-sidebars: When no sidebar content exists.
 *     - one-sidebar and sidebar-first or sidebar-second: A combination of the
 *       two classes when only one of the two sidebars have content.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see zen_preprocess_html()
 * @see template_process()
 */
 header('P3P: CP="NOI ADM DEV COM NAV OUR STP"');
  global $user;
  global $widgetLogin;
  $widgetLogin='login_for_widget';
  exp_sp_ajax_sample_login_success();
  if(!empty($variables['headerarray'])){
	$headerArray=$variables['headerarray'];
	expDebug::dPrint('$headerArray = ' . print_r($headerArray, true),4);
	for($i=0;$i<sizeOf($headerArray);$i++){
	
		header($headerArray[$i],false);
	
	}
  }
  
  expDebug::dPrint("QUERY STRING SERVER REQUEST=".print_r($_SERVER,true),5);
  
  expDebug::dPrint("REQUEST=".print_r($_REQUEST,true),5); 
 
  $qString="q=".$_REQUEST['q']; 
 
  $actual_qString = $qString;
  
  expDebug::dPrint('$actual_qString = '.$actual_qString,1);
  
  //For iFrame - To check the valid url whether invoked or not. According to invoked url, set the canvas url and render the pages. 
  expDebug::dPrint('$qString = '.$qString,1); 
  
  if(stristr($qString,'q=salesforce/widget/MyEnrollment')== TRUE) {
  	 $qString='q=canvas/mylearning';
  	 expDebug::dPrint('$iFrame_qString = '.$qString,1);
  }  
  
  global $salesforce_type;
  
  expDebug::dPrint("In widget_v2.tpl.php : Salesforce_type = ".$salesforce_type,1);
 
  //If canvas 
  if(stristr($qString,'q=salesforce/canvas/authenticate')!= FALSE && $salesforce_type=="canvas") {
  	$qString='q=canvas/mylearning';
  }
  
  
  $class_details_page="";
  
  $lp_details_page="";
  
  
  // Chane the body class attributes for every page . It is applicable only for Canvas 
  // Set atttibutes for body class. Initially it is empty
  
  $attributes="";
  
  if($salesforce_type=="canvas"){

  	// For My Learning Page
  	//if($qString == 'q=canvas/mylearning'){
  	if(stristr($qString,'q=canvas/mylearning')!= FALSE){
  		$attributes="html not-front logged-in one-sidebar sidebar-second page-learning page-learning-enrollment-search section-learning";  		
  	}
  	
  	// For Catalog Page
  	//if($qString == 'q=canvas/catalog'){
  	if(stristr($qString,'q=canvas/catalog')!= FALSE){
  		$attributes="no-sidebars page-learning page-learning-catalog-search section-learning";
  	}
  	
  	//For Reports Page
  	//if($qString == 'q=canvas/reports'){
  	if(stristr($qString,'q=canvas/reports')!= FALSE){
  		$attributes="no-sidebars page-reports section-reports";
  	} 
  	
  	// For Class Details Page 
  	if(stristr($qString,'q=canvas/class-details')!= FALSE) {
  		$attributes="page-learning-class-details";
  		$class_details_page="1";
  	}
  	
  	// For Course Details Page
  	if(stristr($qString,'q=canvas/course-details')!= FALSE) {
  		$attributes="page-learning-course-details";
  		$course_details_page="1";
  	}
  	
  	//For Learning Plan Details Page
  	if(stristr($qString,'q=canvas/learning-plan-details')!= FALSE) {
  		$attributes="page-learning-learning-plan-details";
  		$lp_details_page="1";
  	} 
  	
  	
  	 
  } 
  
  //Get Reports URL from exp_sp.ini file for salesforce report
  $config=getConfig("exp_sp");
  $sf_enr_crs_report_url=trim($config["sf_enr_crs_report_url"]);
  $sf_cmp_crs_report_url=trim($config["sf_cmp_crs_report_url"]);
  $sf_enr_tp_report_url=trim($config["sf_enr_tp_report_url"]);
  $sf_cmp_tp_report_url=trim($config["sf_cmp_tp_report_url"]);
  
  if(trim($sf_enr_crs_report_url)==""){
  	$sf_enr_crs_report_url="javascript:void(0);";
  }
  
  if(trim($sf_cmp_crs_report_url)==""){
  	$sf_cmp_crs_report_url="javascript:void(0);";
  }
  
  if(trim($sf_enr_tp_report_url)==""){
  	$sf_enr_tp_report_url="javascript:void(0);";
  }
  
  if(trim($sf_cmp_tp_report_url)==""){
  	$sf_cmp_tp_report_url="javascript:void(0);";
  }
  
  
 
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print drupal_get_css(); ?>
  
  <!--[if IE 9]>
  	<script type="text/javascript" src="/sites/all/modules/core/exp_sp_core/js/videojs/video.js"></script>
  <![endif]-->

  <?php /*?>
  <!-- Below HTML5 Shiv line added for videojs 3.1.0. 'Must be in <head> to support older browsers. see video.js file of videojs 3.1.0' -->
  <script  type="text/javascript">var document = window.document; document.createElement("video"); document.createElement("audio");</script>
  <?php */ ?>
<!--[if lte IE 7]>
<style type="text/css" media="all">
@import url("/sites/all/themes/core/expertusoneV2/expertusone-internals/css/ie7_admin.css");
</style>
<![endif]-->
<style>
 /* Start Menu Css Changes */ 

<?php  

/* Start iFrame Css */

$hide_menu_attr="";
$hide_left_side_menu="";
// If salesforce type is iframe and my learning, then following css will be applied
if($qString == 'q=canvas/mylearning' && $salesforce_type=="iframe"){
 		$attributes="page-learning-enrollment-search section-learning iframe-mylearning";
 		$hide_menu_attr="class='hide_cat_menu'";
		$hide_left_side_menu="style='display:none;'";
} // End of $qString if 

?>
		
/* End iFrame Css */ 

<?php 

$highlight_sub_menu_css_mylearning="child_menu_div";
$highlight_sub_menu_css_catalog="child_menu_div";
$highlight_report="child_menu_div";

$mylearning_page="0";
$catalog_page="0";
 
//exit();
if($salesforce_type=="canvas"){
  if($qString == 'q=canvas/mylearning' || $qString == 'q=salesforce/canvas/authenticate'){
     $highlight_sub_menu_css_mylearning="child_menu_div_highlighted";
	 $mylearning_page="1";
  }
  
  if($qString == 'q=canvas/catalog' || $class_details_page=="1" || $course_details_page=="1" || $lp_details_page=="1"){
     $highlight_sub_menu_css_catalog="child_menu_div_highlighted";
	 $catalog_page="1";
  } 
  
}

if($qString == 'q=canvas/mylearning' || $qString == 'q=salesforce/canvas/authenticate'){} ?>  
<?php if($qString == 'q=canvas/catalog' && $salesforce_type=="canvas" ){} ?>  
<?php if($qString != 'q=canvas/reports'){}else{}

 ?> 
 
 <?php 	
	if(trim($_REQUEST['exp_sess_id'])!=""){
		 $prev_ses_id=trim($_REQUEST['exp_sess_id']); 
		 //$prev_ses_id=session_id();
	}else{
		$prev_ses_id=session_id();
	}
	
	//if cookie set then clear the session id
		
    if(isset($_COOKIE) && count($_COOKIE)>0 && trim($_COOKIE['SPLearnerInfo'])!="" && trim($_COOKIE['SPCertificate'])!=""){
		$prev_ses_id="";
	} 
	 
 ?>

</style>
</head>
<body id='salesforce_canvas_widget' data-from-salesforce="1" style="background-image:none;background-color:#fff;!important;" class="salesforce-widget <?php echo $attributes;?>">
<div id="sf_exp_work_area"> <!--  start sf_exp_work_area --> 
	
	<div id="widget_cat_menu" <?php echo $hide_left_side_menu;?>> <!--  start widget_cat_menu-->
	   
	   
				   <div id="Learn" class="parent_menu_item"><!-- start Learn-->
							 <a id="parent_menu_img_anc" href="#">
								<img id='menu_arrow_cat_img' class="menu_arrow_close_img" border="0px" src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_close.gif">
							 </a>
							 <a id="parent_menu_text" class="parent_menu_text_link" href="?q=canvas/mylearning&exp_sess_id=<?php echo $prev_ses_id;?>"><span class='parent_menu_text_link_selected1'>Learn</span></a> 
							 
							 <div id='Learn_Child' class='child_menu_item' style='display:none;'><!-- start Learn Child--> 
								 <div class="<?php echo $highlight_sub_menu_css_catalog;?>">
									<a id="sub_menu_catalog_id" class='child_menu_text_link' href="?q=canvas/catalog&exp_sess_id=<?php echo $prev_ses_id;?>">Catalog</a>
								</div> 	
								
								 <div class="<?php echo $highlight_sub_menu_css_mylearning;?>">
									<a id="sub_menu_mylearning_id" class='child_menu_text_link' href="?q=canvas/mylearning&exp_sess_id=<?php echo $prev_ses_id;?>">My Learning</a>
								</div> 	
							</div><!-- end Learn Child-->  
					 </div><!-- end Learn -->
					 
					<?php if(isset($_SESSION['sf']['org_enr_syncup_option']) && trim($_SESSION['sf']['org_enr_syncup_option'])=="1") { ?>
					 
						 <div id="Report" class="parent_menu_item"><!-- start Report -->
								 <a id="parent_menu_img_anc" href="#">
									<img id='menu_arrow_report_img' class="menu_arrow_close_img" border="0px" src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_close.gif">
								 </a>
								 <a id="parent_menu_text" class="parent_menu_text_link" href="javascript:void(0);"><span class='parent_menu_text_link_selected1'>Reports</span></a> 
								 
								 <div id='Report_Child' class='child_menu_item' style='display:none;'><!-- start Report Child--> 
									
									<div class="<?php echo $highlight_report;?>">
										<a id="sub_menu_report_id_1" class='child_menu_text_link' target='_blank' href="<?php echo $sf_enr_crs_report_url;?>">Enrolled Courses</a>
									</div> 
									
									<div class="<?php echo $highlight_report;?>">
										<a id="sub_menu_report_id_2" class='child_menu_text_link' target='_blank' href="<?php echo $sf_cmp_crs_report_url;?>">Completed Courses</a>
									</div> 
									
												
									<div class="<?php echo $highlight_report;?>">
										<a id="sub_menu_report_id_3" class='child_menu_text_link' target='_blank' href="<?php echo $sf_enr_tp_report_url;?>">Enrolled Programs</a>
									</div>
	
									  <div class="<?php echo $highlight_report;?>">
										<a id="sub_menu_report_id_4" class='child_menu_text_link' target='_blank' href="<?php echo $sf_cmp_tp_report_url;?>">Completed Programs</a>
									</div>	 
								 </div><!-- end Report Child--> 
									  
	                   </div><!-- end Report -->
	   
	   <?php } ?>
					
					 
	   
     </div> <!--  end left widget_cat_menu -->

	 
	  <div id="widget" <?php if($prev_ses_id!=''){ echo "data-exp-sess-id=\"$prev_ses_id\""; }?>> <!--  start --> 
	 
		          
					  
					  <?php print $page_top; ?>
					  <div id="outerWidgetTag">
					  <?php  
							  //check the page that whether mylearning page or not
					           if(stristr($qString,'q=canvas/mylearning')!= FALSE){
							 // if($qString=='q=canvas/mylearning'){ //If my learning
							 
					           	   expDebug::dPrint("Rendering My Learning Blocks = ".$qString,5);
					           	    
							  	   require_once 'widget_v2_myenrollment.tpl.php';
							  	   
							  	   if($salesforce_type=="iframe"){ //If iFrame
							  	   	  
							  	   	drupal_add_js(" 					     
							  	   			$('#paintEnrollmentResults a.enroll-launch-more').live('click',function(){					  	   				
							  	   			
							  	   			   $(this).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
												      'position':'absolute','margin-right':'0','z-index':'1000'
												    });
												$(this).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
												      'position':'absolute','margin-right':'0','z-index':'1000'
												    });
												$(this).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
												      'position':'relative','margin-right':'0px','z-index':'0'
												    });
												$(this).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
												      'position':'relative','margin-right':'0px','z-index':'0'
												 });  
							  	   			
							  	   			});
							  	   			
							  	   			$('#paintEnrollmentLPResults a.enroll-launch-more').live('click',function(){					  	   				
							  	   			
												$(this).siblings('.enroll-action').closest('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
												      'position':'absolute','margin-right':'0','z-index':'1000'
												    });
												$(this).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
												      'position':'absolute','margin-right':'0','z-index':'1000'
												    });
												$(this).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
												      'position':'absolute','margin-right':'0px','z-index':'0'
												    });
												$(this).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
												      'position':'absolute','margin-right':'0px','z-index':'0'
												    });
							  	   			
							  	   			});
							  	   			
							  	   			","inline");
							  	   	
							  	   	
							  	   }  
							  	   
							  	   
							  }else if(stristr($qString,'q=canvas/catalog')!= FALSE){  
							  	
							  	expDebug::dPrint("Rendering Catalog Blocks = ".$qString,5);
							  	
							  	//if($qString=='q=canvas/catalog'){ //If catalog  //if(stristr($qString,'q=canvas/catalog')!= FALSE){
							  	
							  	$add_sess_qs="?exp_sess_id=".$prev_ses_id."&"; 
							  	$cls_details_qs=$add_sess_qs."q=canvas/class-details";
							  	$crs_details_qs=$add_sess_qs."q=canvas/course-details";  //?q=canvas/course-details
							  	$lp_details_qs=$add_sess_qs."q=canvas/learning-plan-details";  //?q=canvas/learning-plan-details'
							  	
							  	   require_once 'widget_v2_catalog.tpl.php'; 
							  	   drupal_add_js(" 
								  	   		   $('a.spotlight-item-title').live('mouseover',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/class-details','$cls_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 
								  	   		    $('a.spotlight-item-title').live('click',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/class-details','$cls_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 		
							  	   		
							  	   		
							  	   		 $('a.spotlight-item-title').live('mouseover',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/course-details','$crs_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 
								  	   		    $('a.spotlight-item-title').live('click',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/course-details','$crs_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 		
							  	   		
							  	   		
							  	   		
							  	   		 $('a.spotlight-item-title').live('mouseover',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/learning-plan-details','$lp_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 
								  	   		    $('a.spotlight-item-title').live('click',function(){
								                 $('a.spotlight-item-title').each(function(){ 
								  	   		           var a_href = $(this).attr('href');  
								  	   		    	   var a_href_change = a_href.replace('?q=learning/learning-plan-details','$lp_details_qs'); 
								  	   				   $(this).attr('href',a_href_change);
											  	   });
								               }); 	 
							  	   		
							  	   		","inline");  
							  }else if($qString=='q=canvas/reports'){ //If reports
							  	   	require_once 'widget_v2_reports.tpl.php';
							  }else if($class_details_page=="1" || $course_details_page=="1" || $lp_details_page=="1" ) { //If class/ course / Training plandetails page
							  			
							  		   expDebug::dPrint("Rendering Class/Course/Lp details Blocks = ".$qString,5);
							  	
							  			$add_sess_qs="?exp_sess_id=".$prev_ses_id."&";
							  			$catalog_pg_qs=$add_sess_qs."q=canvas/catalog"; //?q=canvas/catalog
							  	
							  	   		require_once 'widget_v2_reports.tpl.php';
									  	drupal_add_js(" 
									  	   		 $('a.class-detail-back-button').live('mouseover',function(){
										                 $('a.class-detail-back-button').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });  
									  	   		
									  	   		 $('a.class-detail-back-button').live('click',function(){
										                 $('a.class-detail-back-button').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });  
									  			
									  			$('a.course-details-backbutton').live('mouseover',function(){
										                 $('a.course-details-backbutton').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });  
									  	   		
									  	   		 $('a.course-details-backbutton').live('click',function(){
										                 $('a.course-details-backbutton').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });   
									  			
									  			$('a.course-details-backbutton').live('mouseover',function(){
										                 $('a.course-details-backbutton').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('/?q=/','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });  
									  	   		
									  	   		 $('a.course-details-backbutton').live('click',function(){
										                 $('a.course-details-backbutton').each(function(){ 
										  	   		           var a_href = $(this).attr('href');  
										  	   		    	   var a_href_change = a_href.replace('/?q=/','$catalog_pg_qs'); 
										  	   				   $(this).attr('href',a_href_change);
													  	   });
										               });    
									  			
									  	   		
									  	   		","inline");
		
									  	
							  	   	
							  }else{ // if others
							  	
							 	 expDebug::dPrint("Rendering Other Blocks = ".$qString,5); 
							  	 
								  	 /** Print the sections in the Widget **/
								  	 $nr_of_sections=isset($variables['widgetarray'])?count($variables['widgetarray']):0;
								  	
									  for($i=0;$i<$nr_of_sections;$i++){  ?>
										<div id="innerWidgetTagEnroll">
											<?php $block_rendered = drupal_render($variables['widgetarray'][$i]);
											print $block_rendered; ?>
										</div>
									  <?php }  /** End of painting the sections */  
							  }
							  
							  
							  
							  
							  //if canvas, disable the right click option
							  
							  if($salesforce_type=="canvas"){
								  	drupal_add_js("
								  			$('div#sf_exp_work_area').live('mouseover',function(){
										  			$('div#sf_exp_work_area').each(function(){
											  			$(this).bind('contextmenu', function(e) {
											  				return false;
											  			});
								  			});
											
											
								  	});
								  	","inline"); 
									
									
									drupal_add_js(" $(document).ready(function(){  
										  $('#menu_arrow_cat_img').click(function(){ 
											  $(this).toggleClass('menu_arrow_open_img');  
											  var isOpen=$(this).hasClass('menu_arrow_open_img');  
											  if(isOpen==true){ 
												 $(this).attr('src','/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_open.gif');
												 $('#Learn_Child').show();
											  }else{        
											     $(this).attr('src','/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_close.gif');
												 $('#Learn_Child').hide();
											  } 
										  });  
										  
										  $('#menu_arrow_report_img').click(function(){ 
											  $(this).toggleClass('menu_arrow_open_img');  
											  var isOpen=$(this).hasClass('menu_arrow_open_img');  
											  if(isOpen==true){ 
												 $(this).attr('src','/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_open.gif');
												 $('#Report_Child').show();
											  }else{        
											     $(this).attr('src','/sites/all/themes/core/expertusoneV2/expertusone-internals/images/sf_menu_arrow_close.gif');
												 $('#Report_Child').hide();
											  } 
										  });  
										  
									 });
									 ","inline"); 
									
									 if($mylearning_page=="1" || $catalog_page=="1"){
											   drupal_add_js("
											   $(document).ready(function(){
														$('#menu_arrow_cat_img').trigger('click');
												});		
														","inline");
									 } 
									 
							  } 
							  
							  
					 ?>
					  </div>
					
					<!-- Start Footer Section -->
					 
							<div id="block-exp-sp-footer-footer" class="block block-exp-sp-footer contextual-links-region last even">
							  
							  <div class="content">
							    <div id="footer">
								    			<div class="copyright-page">
								    				<div class="poweredby">
													<a href="http://www.expertus.com"><img height="30" width="185" src="/sites/all/themes/core/expertusone/images/powered-by.png"></a>
													</div>
												</div>				
								</div>
							  </div>  
							</div> 
				 	 
				<!-- End Footer Section -->
		</div> <!--  End widget -->
  </div>  <!--  End sf_exp_work_area --> 
 
 
 <?php 
 
  if(stristr($actual_qString,'q=salesforce/canvas/authenticate')!= FALSE){
		  	
		  drupal_add_js("
					 			$(document).ready(function(){
					 			   
		  							var no_of_enr = $('table#paintEnrollmentResults').attr('data-enr-count'); 
		  		
		  							if (typeof no_of_enr !== typeof undefined && no_of_enr !== false) {
									    // alert('no_of_enr='+no_of_enr);
									  	if(no_of_enr<=0){
							 		        // alert('Hi I am here');
									  		$('#enroll-noresult-msg').css('display','block !important');
									  	}
									   
									}
		  		
							     });
		 			","inline");   
 }
 
 drupal_add_js("  
 		 $(document).ready(function(){    
 		
 
  			$( window ).scroll(function() { 
 								var rightPanleH=$('#widget').height();
						 		var leftPanleH=$('#widget_cat_menu').height(); 
						 		$('#widget_cat_menu').height(rightPanleH); 
		 						$('#widget_cat_menu').css('height', + rightPanleH +'!important;'); 


            });
        });
		 		
 		",
 		"inline"); 

 ?>
 

<?php print $page_bottom; ?>
<?php print $_SESSION['exp_sp_scripts']; ?>    
<?php print drupal_get_js(); ?>
<?php print isset($closure)?$closure:""; ?> 

</body>
</html>