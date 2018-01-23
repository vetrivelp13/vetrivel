<div id="innerWidgetTagEnroll"> <!--  start innerWidget -->
<?php 

//Render mylearning page blocks

//Render My Enrollments Block
$block_rendered_my_enrollment = drupal_render($variables['widgetarray'][0]);

//Render My Programs Block
$block_rendered_my_programs = drupal_render($variables['widgetarray'][1]);

//Render Announcement Block
//$block_rendered_announcement = drupal_render($variables['widgetarray'][2]);

//Render My Calender Block
$block_rendered_my_calender = drupal_render($variables['widgetarray'][2]);

//Render My Transscript Block
$block_rendered_my_transcript = drupal_render($variables['widgetarray'][3]);

 ?>
			
 
 <div id="main-wrapper">
      <div id="content" class="column"> <!-- start left -->
          <div class="section"><!-- start section -->
              <div class="region region-highlight"> <!-- start myenrollment and my programs-->

               <div class="region region-highlight"> <!-- start enrollment and learning plan blocks -->
                       

                    <!-- start my enrollment block-->
                       <?php print $block_rendered_my_enrollment; ?>
                    <!-- end my enrollment block-->

                    <!-- start my programs block-->
						<?php print $block_rendered_my_programs; ?>
		   			 <!-- end my programs block-->

               </div><!-- end enrollment and learning plan blocks -->
              
	     <div class="breadcrumb"><h2 class="element-invisible">You are here</h2>
             <a href="/">Home</a> › </div>  
              </div><!-- end myenrollment and my programs-->
           </div> <!-- end section -->
 

      </div><!-- end left -->

      <div id="navigation"><div class="section clearfix"> <!-- start center -->
		<h2 class="element-invisible">Main menu</h2><ul class="links inline clearfix" id="main-menu"><li class="menu-198 first"><a title="" href="/">HOME</a></li>
		<li class="menu-345"><a title="" href="/?q=learning/catalog-search">CATALOG</a></li>
		<li class="menu-951"><a title="" href="/?q=learning/my-profile">PROFILE</a></li>
		<li class="menu-624"><a title="" href="/?q=learning/myteam-search">TEAM</a></li>
		<li class="menu-956"><a title="" href="/?q=learning/forum-list">DISCUSSIONS</a></li>
		<li class="menu-940"><a title="" href="/?q=reports">REPORTS</a></li>
		<li class="menu-346 active-trail last active"><a class="active-trail active" title="" href="/?q=learning/enrollment-search">MY LEARNING</a></li>
		</ul>
       </div> </div><!-- end center -->

      <div class="region region-sidebar-second column sidebar"> <!-- start right --> 
 
                    <!-- start announcement block-->
                      <?php //print $block_rendered_announcement; ?>
                    <!-- end announcement block-->

                    <!-- start my calender block-->
					 <?php print $block_rendered_my_calender; ?>
				    <!-- end my calender block-->
		
				    <!-- start my transcript block-->
				    <span id='salesforce_my_transcripts' style='display:none;'></span>
					 <?php print $block_rendered_my_transcript; ?>
         	       <!-- end my transcript block-->
         	       
         	       <div class="block-footer-left">
					  <div class="block-footer-right">
						<div class="block-footer-middle">&nbsp;</div>
					  </div>
					</div>
	

      </div><!-- end right -->

   </div>
   
   </div><!--  end innerWidget -->