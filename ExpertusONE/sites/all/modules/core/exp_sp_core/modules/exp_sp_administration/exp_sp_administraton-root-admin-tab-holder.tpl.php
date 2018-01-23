<?php global $theme_key,$language;
expDebug::dPrint('root admin tab holder tpl file : adminSubLinksArr = ' . print_r($admin_sub_links_arr, true),4);
$renderedRoundedFooter = theme('root_admin_rounded_footer');?> 

<div class="admin-tab-bg" id="admin-tab-bg"> <!-- start bg -->
	 	<div id="admin-sub-links"><!--  Start - admin sub menu link tabular column  -->
		     <div class="block-title-left">
		  		<div class="block-title-right">
		  			<div class="block-title-middle">
		  			<?php if(($language->language != 'ru' || $tab_title != t('Manage')) || $theme_key == 'expertusoneV2'){?>
		    				<h2 class="block-title"><?php print $tab_title;?></h2>
		    				<?php } ?>  
			  		<div id="admin-maincontent_tab">
						<?php if($theme_key != 'expertusoneV2') {?>
						<div id="sort-bar-V2">
						<?php $countSubMenu=count($admin_sub_links_arr);
									$maxList=6;
                  			if($countSubMenu>0 && !empty($admin_sub_links_arr)){ $i=1; ?>
                  			  <ul class="AdminsublinktabNavigation">
                  			  <?php if(count($admin_sub_links_arr)>$maxList){ ?>
												  	<li class="first "> <a href="javascript:void(0);" onclick="scorllMainTabPrev()" class="page-menu-tab-navigator page-menu-tab-navigator-prev"> << </a> </li>
												  <?php } ?>
                  			  <?php foreach($admin_sub_links_arr as $sub_link_index => $sub_link_arr_val){ 
                  			          $tmp_admin_link_id='admin-tab-' . end(explode('/',trim($sub_link_arr_val["path"])));
				                  			  if($i>$maxList){ ?>
																	 <li class="hidden-main-tab"><a href="/?q=<?php print $sub_link_arr_val["path"];?>" id="<?php print $tmp_admin_link_id;?>" data="{'link_path':'<?php print $sub_link_arr_val["path"];?>'}"><span><span><?php print $sub_link_arr_val["title"];?></span></span></a></li>
								             <?php }else{ ?>
																	 <li class="visible-main-tab"><a href="/?q=<?php print $sub_link_arr_val["path"];?>" id="<?php print $tmp_admin_link_id;?>" data="{'link_path':'<?php print $sub_link_arr_val["path"];?>'}"><span><span><?php print $sub_link_arr_val["title"];?></span></span></a></li>
								             <?php }
												          $i++;                                
                  			  }?>
                  			  <?php if(count($admin_sub_links_arr)>$maxList){ ?>
											  	<li class="last"> <a href="javascript:void(0);" onclick="scorllMainTabNext();" class="page-menu-tab-navigator page-menu-tab-navigator-next"> >> </a> </li>
											  <?php } ?>
											  <div id='tab-scroller' style='width: 0px; height: 0px; margin-right: 0px; float:right;'></div>
                  			 </ul>
                               <?php } ?>
                          </div>
                             <?php } //print $rendered_narrow_search_actionbar; ?> 		
						</div>  
					</div>
				</div>
		     </div>
		</div><!--  End - admin sub menu link tabular column --> 
		<?php if($theme_key == 'expertusoneV2') {?>
		<div id="sort-bar-V2">
        <?php 
        $countSubMenu=count($admin_sub_links_arr);
        //#custom_attribute_0078975
        $manage_tab_active = strpos(current(array_column($admin_sub_links_arr, 'path')),'manage');
        $custom_tab_active = strpos(end(array_column($admin_sub_links_arr, 'path')),'customattribute');
        if($manage_tab_active != false && $custom_tab_active != false && $countSubMenu > 5 && $language->language != 'ja' && $language->language != 'ko' && $language->language != 'zh-hans') { ?>
        <div id="carousel_container">
            <div class="first"><a id="tab-left-arrow" onclick="MoveTabPrev(<?php print $countSubMenu; ?>);" class="page-menu-tab-navigator page-menu-tab-navigator-prev"> << </a></div>
                <div id="carousel_inner">
            <?php } 
			$maxList=6;
			if($countSubMenu>0 && !empty($admin_sub_links_arr)){ $i=1;?>
			  <ul class="AdminsublinktabNavigation">
              
              <?php if(count($admin_sub_links_arr)>$maxList && strpos($admin_sub_links_arr[0]['path'],'manage') == false){ ?>
			  <li class="first "> <a href="javascript:void(0);" onclick="scorllMainTabPrev()" class="page-menu-tab-navigator page-menu-tab-navigator-prev"> << </a> </li>
			  <?php } ?>
			  <?php foreach($admin_sub_links_arr as $sub_link_index => $sub_link_arr_val){ 
			          $tmp_admin_link_id='admin-tab-' . end(explode('/',trim($sub_link_arr_val["path"])));
			          if($i>$maxList){ ?>
			 <li class="hidden-main-tab"><a href="/?q=<?php print $sub_link_arr_val["path"];?>" id="<?php print $tmp_admin_link_id;?>" data="{'link_path':'<?php print $sub_link_arr_val["path"];?>'}"><span><span><?php print $sub_link_arr_val["title"];?></span></span></a></li>
             <?php }else{ ?>
			          	 
			 <li class="visible-main-tab"><a href="/?q=<?php print $sub_link_arr_val["path"];?>" id="<?php print $tmp_admin_link_id;?>" data="{'link_path':'<?php print $sub_link_arr_val["path"];?>'}"><span><span><?php print $sub_link_arr_val["title"];?></span></span></a></li>
             <?php }
			          $i++;
			   }?>
               
               <?php if(count($admin_sub_links_arr)>$maxList && strpos($admin_sub_links_arr[0]['path'],'manage') == false){ ?>
			  	<li class="last"> <a href="javascript:void(0);" onclick="scorllMainTabNext();" class="page-menu-tab-navigator page-menu-tab-navigator-next"> >> </a> </li>
			  <?php } ?>
			  <div id='tab-scroller' style='width: 0px; height: 0px; margin-right: 0px; float:right;'></div>
			 </ul>
             <?php if($manage_tab_active != false && $custom_tab_active != false && $countSubMenu > 5 && $language->language != 'ja' && $language->language != 'ko' && $language->language != 'zh-hans') { ?>
             </div>
                <div class="last"><a id="tab-right-arrow" onclick="MoveTabNext(<?php print $countSubMenu; ?>);" class="page-menu-tab-navigator page-menu-tab-navigator-next"> >> </a></div>
             </div>
             <?php } } ?>
             <?php //print $rendered_narrow_search_actionbar; ?> 
		</div>
		<?php }?>
		<div class="clearBoth"></div> 
		<div id="tab-content-main" class='tab-content-main'></div><!--  Results --> 
		 
</div><!-- end bg -->
        <?php print $renderedRoundedFooter; ?>   
          
	       
