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
	
	for($i=0;$i<sizeOf($headerArray);$i++){
	
		header($headerArray[$i],false);
	
	}
  }
  
  $qString=$_SERVER['QUERY_STRING'];
  
  //For iFrame - To check the valid url whether invoked or not. According to invoked url, set the canvas url and render the pages. 
  expDebug::dPrint('$qString = '.$qString,5); 
  
  if(stristr($qString,'q=salesforce/widget/MyEnrollment')== TRUE) {
  	 $qString='q=canvas/mylearning';
  } 
  expDebug::dPrint('$iFrame_qString = '.$qString,5);
  
  global $salesforce_type;
  
  expDebug::dPrint("In widget.tpl.php : Salesforce_type = ".$salesforce_type,5);
 
  $class_details_page="";
  $lp_details_page="";
  
  
  // Chane the body class attributes for every page . It is applicable only for Canvas
  // Set atttibutes for body class. Initially it is empty
  
  $attributes="";
  
  if($salesforce_type=="canvas"){  
  	//For Reports Page
  	if($qString == 'q=canvas/reports'){
  		$attributes="no-sidebars page-reports section-reports";
  	}  
  }
  
  
 
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

 <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <?php print $head; ?>
  
  <title><?php print $head_title; ?></title>
  <?php print drupal_get_css(); ?>

  <?php /*?>
  <!-- Below HTML5 Shiv line added for videojs 3.1.0. 'Must be in <head> to support older browsers. see video.js file of videojs 3.1.0' -->
  <script  type="text/javascript">var document = window.document; document.createElement("video"); document.createElement("audio");</script>
  <?php */ ?>
<!--[if lte IE 7]>
<style type="text/css" media="all">
@import url("/sites/all/themes/core/expertusone/expertusone-internals/css/ie7_admin.css");
</style>
<![endif]-->
<style>


<?php if($qString == 'q=canvas/catalog' ||  $qString == 'q=canvas/myteam' || $qString == 'q=canvas/reports'){ 
	$attributes = 'canvas-catalog-myteam-reports';
	?>
<?php } // End of $qString if  

  if(stristr($qString,'q=canvas/class-details')!= FALSE) {
	$attributes = 'canvas-class-details';
	//If class details page ?>
<?php  
$class_details_page="1"; 
} 
  if(stristr($qString,'q=canvas/course-details')!= FALSE) { //If course details page ?>
$attributes = 'canvas-course-details';
<?php  
$course_details_page="1"; 
}  
if(stristr($qString,'q=canvas/learning-plan-details')!= FALSE) { //If LP details page ?>  
	$attributes = 'canvas-lp-details';
<?php 	 $lp_details_page="1";    
 }  
?> 
<?php 
// If salesforce type is iframe and my learning, then following css will be applied
if($qString == 'q=canvas/mylearning' && $salesforce_type=="iframe"){ $attributes = 'iframe-mylearning';?>
<?php } // End of $qString if ?>
<?php if($qString != 'q=canvas/reports'){ //if not reports ?>
<?php } ?>
<?php if($qString == 'q=canvas/mylearning' || $qString == 'q=salesforce/canvas/authenticate'){ ?>
<?php } ?>
<?php if($qString == 'q=canvas/catalog' && $salesforce_type=="canvas" ){ ?>
<?php } ?>
</style>
</head>
<body style="background-image:none;background-color:#fff;!important;" class="salesforce-widget <?php echo $attributes;?>" >
  
  <!-- Start Menu Links Paint -->
  <div id="widget"><!--  start -->
			  <div class="block block-system block-menu first odd" id="block-system-main-menu">
				  <div class="content">
					<ul class="menu">
						<?php 
						//echo "<pre>";print_r($variables);echo "</pre>";
							foreach($variables['canvasmenu'] as $menuitem){
								$menu_label=$menuitem['title'];
								$menu_href="/?q=".$menuitem['href'];
								
								//echo "<pre>";print_r($qString); echo "</pre>";
			                    
								// Query string has the authenticate url first time while render the expertusone app- Added by ganeshbabuv on aug 14th 2014 12:30 pm
								/*if($qString=='q=salesforce/canvas/authenticate'){
								  $qString='q=canvas/mylearning';
								}*/
								
								if(stristr($qString,'q=salesforce/canvas/authenticate') != FALSE) { 
									$qString='q=canvas/mylearning';
								}
								
								
								$tmp_qry_string="/?".$qString;
								$tmp_cls="";
								if($tmp_qry_string==$menu_href){
								  $tmp_cls="class='active'";
								} 
								
								//Set catalog menu as default for class , course and learning plan details page
								if(($class_details_page=="1" || $course_details_page=="1" || $lp_details_page=="1") && $menu_href=="/?q=canvas/catalog"){
									$tmp_cls="class='active'";
								}    
			
						?>
							<li class="leaf"><a title="" <?php echo $tmp_cls;?>  href="<?php print($menu_href); ?> "><?php print($menu_label); ?> </a></li>
						<?php } ?>
					</ul>  
				  </div>
				  
			  </div>
			  <!-- End Menu Links  -->
			  
			  <?php print $page_top; ?>
			  <div id="outerWidgetTag">
			  <?php  
					  //check the page that whether mylearning page or not
					  //echo $qString;
					  
					  if($qString=='q=canvas/mylearning'){ //If my learning
					  	   
					  	//echo "<div id='test_div_id'><a class='use-ajax ctools-modal-ctools-login-style ajax-processed' href='?q=ctools_ajax_sample/ajax/user_login' title='test dialog box'>Test Dialog Box</a></div>";
					  	require_once 'widget_myenrollment.tpl.php';  
					  	    
					  }else if($qString=='q=canvas/catalog'){ //If catalog 
					  	   require_once 'widget_catalog.tpl.php';  
					  	   drupal_add_js(" 
						  	   		   $('a.spotlight-item-title').live('mouseover',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/class-details','?q=canvas/class-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 
						  	   		    $('a.spotlight-item-title').live('click',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/class-details','?q=canvas/class-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 		
					  	   		
					  	   		 $('a.spotlight-item-title').live('mouseover',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/course-details','?q=canvas/course-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 
						  	   		    $('a.spotlight-item-title').live('click',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/course-details','?q=canvas/course-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 		
					  	   		
					  	   		
					  	   		 $('a.spotlight-item-title').live('mouseover',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/learning-plan-details','?q=canvas/learning-plan-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 
						  	   		    $('a.spotlight-item-title').live('click',function(){
						                 $('a.spotlight-item-title').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/learning-plan-details','?q=canvas/learning-plan-details'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               }); 	 
					  	   		
					  	   		","inline");  
					  	   
					  	   
					  }else if($qString=='q=canvas/reports'){ //If Reports
					  	   	require_once 'widget_reports.tpl.php'; 
					  }else if($class_details_page=="1" || $course_details_page=="1" || $lp_details_page=="1") { //If class/course/TP details page
					  	   require_once 'widget_reports.tpl.php';
					  	   $replace_str="javascript:location.href='/?q=canvas/catalog-search'";   
					  	   drupal_add_js(" 
					  	    
					  	   		 $('a.detail-back-button').live('mouseover',function(){
						                 $('a.detail-back-button').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','?q=canvas/catalog'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               });  
					  	   		
					  	   		 $('a.detail-back-button').live('click',function(){
						                 $('a.detail-back-button').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','?q=canvas/catalog'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               });  
					  	   		
					  	   		$('a.detail-back-button').live('click',function(){
						                 $('a.detail-back-button').each(function(){ 
						  	   		           var a_href = $(this).attr('href');  
						  	   		    	   var a_href_change = a_href.replace('?q=learning/catalog-search','?q=canvas/catalog'); 
						  	   				   $(this).attr('href',a_href_change);
									  	   });
						               });  
					  	   		
					  	   		 $('input.detail-back-button').live('mouseover',function(){
					  	   			  $('input.detail-back-button').each(function(){ 
					  	   		      		  $(this).removeAttr('onclick'); 
					  	   				 })
						          });   

					  	   		 $('input.detail-back-button').live('click',function(){
					  	   			  $('input.detail-back-button').each(function(){ 
					  	   		      		   $(this).removeAttr('onclick');
					  	   						self.location='/?q=canvas/catalog';   
					  	   				 })
						          });    
					  	   		
					  	   		","inline");  
					  	   
					  	     
					  }else{ // if others
						  	 /** Print the sections in the Widget **/
						  	 $nr_of_sections=isset($variables['widgetarray'])?count($variables['widgetarray']):0;
						  	
							  for($i=0;$i<$nr_of_sections;$i++){  ?>
								<div id="innerWidgetTagEnroll">
									<?php $block_rendered = drupal_render($variables['widgetarray'][$i]);
									print $block_rendered; ?>
								</div>
							  <?php }  /** End of painting the sections */ 
					  }
			 ?>
			  </div>
			
			<!-- footer Section -->
			 
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


  <?php print $page_bottom; ?>
    <?php print $_SESSION['exp_sp_scripts']; ?>  
  <?php print drupal_get_js(); ?>
  <?php print isset($closure)?$closure:''; ?>  
 
</body>
</html>