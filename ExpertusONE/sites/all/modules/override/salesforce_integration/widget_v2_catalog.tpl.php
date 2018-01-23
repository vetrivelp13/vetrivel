<div id="innerWidgetTagEnroll"> <!--  start innerWidget -->
<?php 
//Render catalog Block
$block_rendered_catalog= drupal_render($variables['widgetarray'][0]);
 ?>
<div style="display:none;" id="search_searchtext"></div><!--  search box -->

<div id="page-container"> <!-- start page container -->
	<div id="page-wrapper"> <!-- start page wrapper -->
             <div id="page"> <!-- start page -->
                 <div id="header_new"></div> <!-- start and end header-->



						 <div id="main-wrapper"><!-- start main-wrapper -->
							 <div id="main" class="clearfix with-navigation"><!-- start main -->
								   
								   <div id="content" class="column"> <!-- start left content -->
									      <div class="section"> <!-- start section-->
											<div class="region region-highlight"> <!-- start region --> 
												   <!-- start my enrollment block-->
												       <?php print $block_rendered_catalog; ?>
												    <!-- end my enrollment block-->  
											 </div> <!-- end region -->
											<a id="main-content"></a> 
									    </div> <!-- end  section-->
								     </div><!-- end  left content -->  
								    
							     
						       </div> <!-- end  main-->
						</div>	<!-- end main-wrapper -->  



                  
             </div> <!-- end page -->
	</div> <!-- end page Wrapper -->
<?php
if(footerIsActive()) {
?>
	<div class="footer-container" style="display:none;"><!-- start footer -->
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
	</div> <!-- end footer -->
<?php } ?>
</div><!-- end page container --> 
		   
			 
   
   </div><!--  end innerWidget -->
