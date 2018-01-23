<div id="innerWidgetTagEnroll"> <!--  start innerWidget --> 
<?php 
global $theme_key, $mylearning_right, $salesforce_type;

//Render mylearning page blocks

//Render My Enrollments Block
//$block_rendered_my_enrollment = drupal_render($variables['widgetarray']['tab_my_enrollment']);

//Render My Programs Block
//$block_rendered_my_programs = drupal_render($variables['widgetarray']['tab_my_learningplan']);

//Render Announcement Block
//$block_rendered_announcement = drupal_render($variables['widgetarray'][2]);


//Render My Calender Block
//$block_rendered_my_calender = drupal_render($variables['widgetarray']['tab_my_calendar']);

//Render My Transscript Block
//$block_rendered_my_transcript = drupal_render($variables['widgetarray']['mytranscript']);

$mylearning_right = true;
$user_preference = getUserPreference('', true);

// expDebug::dPrint('user preference list in sf tpl: ' . print_r($user_preference, 1));

if(count($user_preference['mylearning_right'])<=0){
	$mylearning_right = false;
}
if ($salesforce_type == 'iframe') { // make iframe to full width
	$mylearning_right = false;
}
drupal_add_js(array('mylearning_right' =>  $mylearning_right), 'setting');
 ?>
 
<div id="page-container"> <!-- start page container -->
	<div id="page-wrapper"> <!-- start page wrapper -->
             <div id="page"> <!-- start page -->
                 <div id="header_new"></div> <!-- start and end header-->



						 <div id="main-wrapper"><!-- start main-wrapper -->
							 <div id="main" class="clearfix with-navigation"><!-- start main -->
								     <!-- Start BR settings Links Paint -->
		            <?php // if(($qString == 'q=canvas/mylearning' || $qString == 'q=salesforce/canvas/authenticate') &&  $salesforce_type=="canvas" ){
		            	 if((stristr($qString,'q=canvas/mylearning')!= FALSE || stristr($qString,'q=salesforce/canvas/authenticate')!= FALSE) &&  $salesforce_type=="canvas"){ 
		            	?>
			  			  <div class="block block-system block-menu first odd" id="block-system-main-menu"> 
					         <?php 
					         if(stristr($qString,'q=canvas/mylearning')!= FALSE){
					         //if($qString=='q=canvas/mylearning'){
						         foreach($variables['panel_configuration'] as $item) {
						         	print drupal_render($item); 
						         }
					         }  // print panel add block in my learning only ?>
						  </div>
					  <?php } ?>
					  <!-- End BR settings Links  -->
								   <div id="content" class="column <?php if($mylearning_right===false) print 'clsContentFull'; ?>"> <!-- start left content -->
									      <div class="section"> <!-- start section-->
									      <div class="region region-highlight"> <!-- start region --> 
												<ul id="highlight-list" class="sortable-list">  <!-- start region list --> 
												   <!-- start my enrollment block-->
												       <?php //print $block_rendered_my_enrollment; ?>
												    <!-- end my enrollment block-->
				
												    <!-- start my programs block-->
													 <?php  //print $block_rendered_my_programs; ?>
													 
													 <?php 
													 	// render left side blocks
													  	asort($user_preference['mylearning_left']);
													  	foreach($user_preference['mylearning_left'] as $key => $block_weight) {
													 		print drupal_render($variables['widgetarray'][$key]);
													 	}
													 ?>
												     <!-- end my programs block-->
						  						</ul>	<!-- end region list --> 
											 </div> <!-- end region -->
											<a id="main-content"></a> 
									    </div> <!-- end  section-->
								     </div><!-- end  left content -->
				
								     <div id="navigation"><div class="section clearfix"> <!-- start center navigation -->
									<h2 class="element-invisible">Main menu</h2><ul class="links inline clearfix" id="main-menu"><li class="menu-198 first"><a title="" href="/">HOME</a></li>
									<li class="menu-345"><a title="" href="/?q=learning/catalog-search">CATALOG</a></li>
									<li class="menu-951"><a title="" href="/?q=learning/my-profile">PROFILE</a></li>
									<li class="menu-624"><a title="" href="/?q=learning/myteam-search">TEAM</a></li>
									<li class="menu-956"><a title="" href="/?q=learning/forum-list">DISCUSSIONS</a></li>
									<li class="menu-940"><a title="" href="/?q=reports">REPORTS</a></li>
									<li class="menu-346 active-trail last active"><a class="active-trail active" title="" href="/?q=learning/enrollment-search">MY LEARNING</a></li>
									</ul>
								     </div></div> <!-- end center navigation-->
				
									<?php if(count($user_preference['mylearning_right']) > 0): ?>
								    <div class="region region-sidebar-second column sidebar"> <!-- start right second column -->
								    	<ul id="sidebar_second-list" class="sortable-list">  <!-- start sidebar region list --> 
								     <!-- start announcement block-->
								      <?php //print $block_rendered_announcement; ?>
								    <!-- end announcement block-->
				
								    <!-- start my calender block-->
											 <?php  //print $block_rendered_my_calender; ?>
										    <!-- end my calender block-->
						
										    <!-- start my transcript block-->
										     <span id='salesforce_my_transcripts' style='display:none;'></span>
											 <?php // print $block_rendered_my_transcript; ?>
							 	       <!-- end my transcript block-->
							 	       
							 	       <?php 
													 	// render left side blocks
							 	       					asort($user_preference['mylearning_right']);
													 	foreach($user_preference['mylearning_right'] as $key => $block_weight) {
													 		print drupal_render($variables['widgetarray'][$key]);
													 	} 
													 ?>
							 	       
							 	      <!--  <div class="block-footer-left">
											  <div class="block-footer-right">
												<div class="block-footer-middle">&nbsp;</div>
											  </div>
											</div>  -->
										</ul> <!-- end sidebar region list --> 
							     </div> <!-- end right second column -->
							     <?php endif; ?>
						       </div> <!-- end  main-->
						</div>	<!-- end main-wrapper -->  


                  
             </div> <!-- end page -->
	</div> <!-- end page Wrapper -->
<?php
if(footerIsActive()) {
?>
	<div class="footer-container" style="display:none;">
		<div class="footer-phone-info">Phone: </div>
		<div class="footer-email-info">Email: expertusonesite@expertus.com</div>
		<div class="footer-version-info">Version: 4.3.20.6.1</div>
		<div class="region region-footer">
		<div class="block block-exp-sp-footer first odd" id="block-exp-sp-footer-footer">

		<div class="content">
		<div class="footer-web-links"><a onclick="window.open('http://twitter.com/ExpertusONE','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-twitter" href="javascript:void(0);"></a><a onclick="window.open('http://linkedin.com/company/Expertus','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-linked" href="javascript:void(0);"></a><a onclick="window.open('http://facebook.com/ExpertusONE','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-facebook" href="javascript:void(0);"></a><a onclick="window.open('https://plus.google.com/101458231351087018974','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-google-plus" href="javascript:void(0);"></a><a onclick="window.open('http://pinterest.com/expertus','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-persent" href="javascript:void(0);"></a><a onclick="window.open('http://youtube.com/expertusinc','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-you-tube" href="javascript:void(0);"></a><a onclick="window.open('http://expertusone.wordpress.com','footerlinkwin','toolbar=1,location=1,statusbar=1,resizable =1,scrollbars=1,width=900,height=900');" class="web-linnk-word-press" href="javascript:void(0);"></a></div>  </div>
		</div><!-- /.block -->
		<div class="block block-menu last even" id="block-menu-menu-footer-menu">
		<h2 class="title">Footer Menu</h2>

		<div class="content">
		<ul class="menu"><li class="first leaf"><a target="_blank" title="" href="/?q=node/1">About Us</a></li>
		<li class="leaf"><a target="_blank" title="" href="/?q=node/2">Feedback</a></li>
		<li class="leaf"><a target="_blank" title="" href="/?q=node/5">Legal Notices</a></li>
		<li class="leaf"><a target="_blank" title="" href="/?q=node/3">Privacy Policy</a></li>
		<li class="leaf"><a target="_blank" title="" href="/?q=node/4">Trademark</a></li>
		<li class="last leaf"><a target="_blank" title="" href="/?q=learning/mobile-app">Mobile</a></li>
		</ul>  </div>
		</div>
		<!-- /.block -->
		</div><!-- /.region -->
	</div>
<?php } ?>
</div><!-- end page container --> 
		   
			 
   
   </div><!--  end innerWidget -->